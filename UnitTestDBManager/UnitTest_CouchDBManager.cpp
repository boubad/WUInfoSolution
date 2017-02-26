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
using namespace InfoCouchDB;
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
	TEST_MODULE_INITIALIZE(ModuleInitialize)
	{
		create_task(CouchDBManager::CheckDatabaseAsync(st_baseUrl,st_databaseName)).wait();
	}

	TEST_MODULE_CLEANUP(ModuleCleanup)
	{
		//Logger::WriteMessage("In Module Cleanup");
	}
	//
	TEST_CLASS(UnitTestDBManager)
	{
	public:
		CouchDBManager^ m_pman;
		IMap<String^, Object^>^ m_map;
		IMap<String^, Object^>^ m_xmap;
		IMap<String^, Object^>^ m_rmap;
	public:
		TEST_METHOD_INITIALIZE(SetUp)
		{
			m_pman = ref new CouchDBManager(st_baseUrl, st_databaseName);
			Assert::IsNotNull(m_pman);
			//
			m_map = ref new Map<String^, Object^>();
			Assert::IsNotNull(m_pman);
			m_map->Insert("boolval", true);
			m_map->Insert("intval", (int)123);
			m_map->Insert("doubleval", (double)5.678);
			m_map->Insert("stringval", "Hello World!");
			m_map->Insert("type", "testtype");
			IVector<String^>^ vec = ref new Vector<String^>();
			vec->Append("zero");
			vec->Append("one");
			vec->Append("two");
			vec->Append("three");
			vec->Append("four");
			vec->Append("five");
			m_map->Insert("vectorval", vec);
			//
			IMap<String^, Object^>^ m_xmap = ref new Map<String^, Object^>();
			m_xmap->Insert("type", "testtype");
			//
			int nc = create_task(m_pman->GetDocumentsCountAsync(m_xmap)).get();
			if (nc < 1) {
				bool bRet = create_task(m_pman->MaintainsDocumentAsync(m_map)).get();
				Assert::IsTrue(bRet);
				nc = create_task(m_pman->GetDocumentsCountAsync(m_xmap)).get();
				Assert::IsTrue(nc > 0);
			}// nc
			IVector<IMap<String^, Object^>^>^ vv = create_task(m_pman->GetDocumentsAsync(m_xmap, 0, 1)).get();
			Assert::IsTrue(vv != nullptr);
			auto it = vv->First();
			Assert::IsTrue(it->HasCurrent);
			m_rmap = it->Current;
			Assert::IsNotNull(m_rmap);
			Assert::IsTrue(m_rmap->HasKey("_id"));
			String^ id = m_rmap->Lookup("_id")->ToString();
			Assert::IsFalse(id->IsEmpty());
			Assert::IsTrue(m_rmap->HasKey("_rev"));
			String^ res = m_rmap->Lookup("_rev")->ToString();
			Assert::IsFalse(res->IsEmpty());
		}// Setup
		TEST_METHOD_CLEANUP(TearDown)
		{
			if ((m_rmap != nullptr) && (m_pman != nullptr)) {
				Assert::IsTrue(m_rmap->HasKey("_id"));
				String^ id = m_rmap->Lookup("_id")->ToString();
				Assert::IsFalse(id->IsEmpty());
				bool b = create_task(m_pman->DeleteDocumentByIdAsync(id)).get();
				Assert::IsTrue(b);
			}// m_pman
			m_rmap = nullptr;
			m_xmap = nullptr;
			m_map = nullptr;
			m_pman = nullptr;
		}// TearDown
	public:
		TEST_METHOD(TestDBManagerIsAlive)
		{
			bool bRet = create_task(m_pman->IsAliveAsync()).get();
			Assert::IsTrue(bRet);
		}// TestDBManagerIsAlive
		TEST_METHOD(TestGetTestImageFile)
		{
			InfoTestData::TestData^ fixture = ref new InfoTestData::TestData();
			Assert::IsNotNull(fixture);
			String^ filename = fixture->TestImageFileName;
			Assert::IsNotNull(filename);
			IStorageFile^ file = fixture->TestImageFile;
			Assert::IsNotNull(file);
			IBuffer^ pBuf = fixture->TestImageBuffer;
			Assert::IsNotNull(pBuf);
			Assert::IsTrue(pBuf->Length > 0);
		}//TestGetTestImageFile
		TEST_METHOD(TestCouchDBAttachment_Buffer)
		{
			String^ id = m_rmap->Lookup("_id")->ToString();
			Assert::IsFalse(id->IsEmpty());
			//
			InfoTestData::TestData^ fixture = ref new InfoTestData::TestData();
			Assert::IsNotNull(fixture);
			String^ attachmentName = fixture->AttachmentName;
			Assert::IsNotNull(attachmentName);
			Assert::IsFalse(attachmentName->IsEmpty());
			String^ mime = fixture->AttachmentMime;
			Assert::IsNotNull(mime);
			Assert::IsFalse(mime->IsEmpty());
			IBuffer^ data = fixture->TestImageBuffer;
			Assert::IsNotNull(data);
			int n0 = static_cast<int>(data->Length);
			Assert::IsTrue(n0 > 0);
			bool b = create_task(m_pman->MaintainsDocumentAttachmentAsync(id, attachmentName, mime, data)).get();
			Assert::IsTrue(b);
			IMap<String^, String^>^ names = create_task(m_pman->GetDocumentAttachmentNamesAsync(id)).get();
			Assert::IsNotNull(names);
			Assert::IsTrue(names->Size > 0);
			IVector<byte>^ xdata = create_task(m_pman->GetDocumentAttachmentDataVectorAsync(id, attachmentName)).get();
			Assert::IsNotNull(xdata);
			int nx = static_cast<int>(xdata->Size);
			Assert::AreEqual(n0, nx);
			b = create_task(m_pman->RemoveDocumentAttachmentAsync(id, attachmentName)).get();
			Assert::IsTrue(b);
		}//TestCouchDBAttachment
		TEST_METHOD(TestCouchDBAttachment_File)
		{
			String^ id = m_rmap->Lookup("_id")->ToString();
			Assert::IsFalse(id->IsEmpty());
			//
			InfoTestData::TestData^ fixture = ref new InfoTestData::TestData();
			Assert::IsNotNull(fixture);
			String^ attachmentName = fixture->AttachmentName;
			Assert::IsNotNull(attachmentName);
			Assert::IsFalse(attachmentName->IsEmpty());
			IStorageFile^ file = fixture->TestImageFile;
			Assert::IsNotNull(file);
			bool b = create_task(m_pman->MaintainsDocumentAttachmentAsync(id, attachmentName, file)).get();
			Assert::IsTrue(b);
			IMap<String^, String^>^ names = create_task(m_pman->GetDocumentAttachmentNamesAsync(id)).get();
			Assert::IsNotNull(names);
			Assert::IsTrue(names->Size > 0);
			IVector<byte>^ xdata = create_task(m_pman->GetDocumentAttachmentDataVectorAsync(id, attachmentName)).get();
			Assert::IsNotNull(xdata);
			int nx = static_cast<int>(xdata->Size);
			Assert::IsTrue(nx > 0);
			b = create_task(m_pman->RemoveDocumentAttachmentAsync(id, attachmentName)).get();
			Assert::IsTrue(b);
		}//TestCouchDBAttachment
	};
}