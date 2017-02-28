#include "pch.h"
#include "CppUnitTest.h"
//////////////////////////////////////
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage::Streams;
using namespace Windows::Storage::FileProperties;
using namespace Windows::Graphics::Imaging;
using namespace Windows::Storage;
using namespace Microsoft::WRL;
using namespace InfoDomain;
using namespace InfoTestData;
using namespace concurrency;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
//////////////////////////
namespace UnitTestDBManager
{
	using byte = uint8;
	//
	static String^ st_baseUrl = "http://localhost:5984";
	static String^ st_databaseName = "test";
	//
	TEST_CLASS(DomainManagerTest)
	{
	public:
		DomainManager^ m_pman;
		TestData^ m_fixture;
		//
		void import_dataset(String^ name, IVector<String^>^ rows, IVector<String^>^ cols,
			IVector<int>^ data) {
			//
			Assert::IsNotNull(name);
			Assert::IsFalse(name->IsEmpty());
			Assert::IsNotNull(rows);
			Assert::IsNotNull(cols);
			Assert::IsNotNull(data);
			int nCols = static_cast<int>(cols->Size);
			Assert::IsTrue(nCols > 0);
			int nRows = static_cast<int>(rows->Size);
			Assert::IsTrue(nRows > 0);
			int nSize = static_cast<int>(data->Size);
			Assert::IsTrue(nSize >= (nCols * nRows));
			//
			DomainManager^ pMan = this->m_pman;
			Assert::IsNotNull(pMan);
			Dataset^ pSet = create_task(pMan->FindDatasetBySigleAsync(name)).get();
			if (pSet == nullptr)
			{
				Dataset^ xSet = ref new Dataset();
				Assert::IsNull(xSet);
				xSet->Sigle = name;
				xSet->Name = "Conso Data Name";
				xSet->Observations = "Conso Data Description";
				xSet->Status = InfoStatus::Normal;
				bool bRet = create_task(pMan->MaintainsDatasetAsync(xSet)).get();
				Assert::IsTrue(bRet);
				pSet = create_task(pMan->FindDatasetBySigleAsync(name)).get();
				Assert::IsNotNull(pSet);
			}// pSet
			Assert::IsTrue(pSet->IsPersisted);
			IMap<String^, Variable^>^ vars = ref new Map<String^, Variable^>();
			Assert::IsNotNull(vars);
			Assert::IsNotNull(vars);
			auto it = cols->First();
			while (it->HasCurrent) {
				String^ varsigle = it->Current;
				Assert::IsNotNull(varsigle);
				Assert::IsFalse(varsigle->IsEmpty());
				Variable^ v = create_task(pMan->FindVariableBySiglesAsync(pSet->Sigle, varsigle)).get();
				if (v == nullptr) {
					Variable^ xVar = ref new Variable(pSet, varsigle);
					xVar->Name = varsigle + " name";
					xVar->Observations = varsigle + " description";
					xVar->VariableKind = InfoKind::Normal;
					xVar->VariableType = InfoDataType::Integer;
					xVar->Status = InfoStatus::Normal;
					Assert::IsTrue(xVar->IsStoreable);
					bool bRet = create_task(pMan->MaintainsVariableAsync(xVar)).get();
					Assert::IsTrue(bRet);
					v = create_task(pMan->FindVariableBySiglesAsync(pSet->Sigle, varsigle)).get();
					Assert::IsNotNull(v);
				}// v
				Assert::IsTrue(v->IsPersisted);
				vars->Insert(varsigle, v);
				it->MoveNext();
			}// it cols
			IMap<String^, Indiv^>^ inds = ref new Map<String^, Indiv^>();
			Assert::IsNotNull(inds);
			auto jt = rows->First();
			while (jt->HasCurrent) {
				String^ indsigle = jt->Current;
				Assert::IsNotNull(indsigle);
				Assert::IsFalse(indsigle->IsEmpty());
				Indiv^ v = create_task(pMan->FindIndivBySiglesAsync(pSet->Sigle, indsigle)).get();
				if (v == nullptr) {
					Indiv^ xInd = ref new Indiv(pSet, indsigle);
					xInd->Name = indsigle + " name";
					xInd->Observations = indsigle + " description";
					xInd->Status = InfoStatus::Normal;
					Assert::IsTrue(xInd->IsStoreable);
					bool bRet = create_task(pMan->MaintainsIndivAsync(xInd)).get();
					Assert::IsTrue(bRet);
					v = create_task(pMan->FindIndivBySiglesAsync(pSet->Sigle, indsigle)).get();
					Assert::IsNotNull(v);
				}// v
				Assert::IsTrue(v->IsPersisted);
				inds->Insert(indsigle, v);
				jt->MoveNext();
			}// jt rows
			IVector<InfoValue^>^ vals = ref new Vector<InfoValue^>();
			Assert::IsNotNull(vals);
			Vector<int>^ oAr = dynamic_cast<Vector<int>^>(data);
			Assert::IsNotNull(oAr);
			auto ki = rows->First();
			int irow = 0;
			while (ki->HasCurrent) {
				String^ indsigle = ki->Current;
				Indiv^ pInd = inds->Lookup(indsigle);
				Assert::IsNotNull(pInd);
				int icol = 0;
				auto kv = cols->First();
				while (kv->HasCurrent) {
					String^ varsigle = kv->Current;
					Variable^ pVar = vars->Lookup(varsigle);
					Assert::IsNotNull(pVar);
					InfoValue^ v = create_task(pMan->FindValueBySiglesAsync(pSet->Sigle, pInd->Sigle, pVar->Sigle)).get();
					if (v == nullptr) {
						int pos = irow * nCols + icol;
						int ival = oAr->GetAt(pos);
						InfoDataValue^ vv = ref new InfoDataValue(ival);
						Assert::IsNotNull(vv);
						v = ref new InfoValue(pInd, pVar, vv);
						Assert::IsNotNull(v);
						Assert::IsTrue(v->IsStoreable);
						vals->Append(v);
					}// v
					kv->MoveNext();
					icol++;
				}// kv
				ki->MoveNext();
				irow++;
			}// ki
			if (vals->Size > 0)
			{
				bool bRet = create_task(pMan->MaintainsValuesAsync(vals, false)).get();
				Assert::IsTrue(bRet);
			}
			String^ id = pSet->Id;
			IVector<InfoBlob^>^ blobs = create_task(pMan->GetDocumentBlobsAsync(id)).get();
			if ((blobs == nullptr) || (blobs->Size < 1)) {
				String^ attName = m_fixture->AttachmentName;
				Assert::IsNotNull(attName);
				Assert::IsFalse(attName->IsEmpty());
				String^ attMime = m_fixture->AttachmentMime;
				Assert::IsNotNull(attMime);
				Assert::IsFalse(attName->IsEmpty());
				IBuffer^ pBuf = m_fixture->TestImageBuffer;
				Assert::IsNotNull(pBuf);
				Assert::IsTrue(pBuf->Length > 0);
				bool bRet = create_task(pMan->MaintainsDocumentAttachmentAsync(id, attName, attMime, pBuf)).get();
				Assert::IsTrue(bRet);
			}// blobs
		}// import_dataset
	public:
		TEST_METHOD_INITIALIZE(SetUp)
		{
			m_pman = ref new DomainManager(st_baseUrl, st_databaseName);
			Assert::IsNotNull(m_pman);
			m_fixture = ref new TestData();
			Assert::IsNotNull(m_fixture);
		}// SetUp
		TEST_METHOD_CLEANUP(TearDown)
		{
			m_pman = nullptr;
		}// TearDown
	public:
		TEST_METHOD(Test_Domain_Mortal)
		{
			String^ name = m_fixture->MortalName;
			IVector<String^>^ rows = m_fixture->MortalRowsNames;
			IVector<String^>^ cols = m_fixture->MortalColsNames;
			IVector<int>^ data = m_fixture->MortalData;
			//
			this->import_dataset(name, rows, cols, data);
		}//Test_Domain_Mortal
		TEST_METHOD(Test_Domain_Conso)
		{
			String^ name = m_fixture->ConsoName;
			IVector<String^>^ rows = m_fixture->ConsoRowsNames;
			IVector<String^>^ cols = m_fixture->ConsoColsNames;
			IVector<int>^ data = m_fixture->ConsoData;
			//
			this->import_dataset(name, rows, cols, data);
		}//Test_Domain_Mortal
	};// class DomainManagerTest
}// namespace UnitTestDBManager