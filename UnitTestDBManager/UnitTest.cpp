#include "pch.h"
#include "CppUnitTest.h"
//////////////////////////////////////
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace InfoCouchDB;
using namespace concurrency;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestDBManager
{
	TEST_CLASS(UnitTestDBManager)
	{
	public:
		CouchDBManager^ m_pman;
		IMap<String^, Object^>^ m_map;
	public:
		TEST_METHOD_INITIALIZE(SetUp)
		{
			String^ baseUrl = "http://localhost:5984/";
			String^ databaseName = "test";
			m_pman = ref new CouchDBManager(baseUrl, databaseName);
			//
			m_map = ref new Map<String^, Object^>();
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
		}// Setup
		TEST_METHOD_CLEANUP(TearDown)
		{
			m_map = nullptr;
			m_pman = nullptr;
		}// TearDown
	public:
		TEST_METHOD(TestDBManagerIsAlive)
		{
			bool bRet = create_task(m_pman->IsAliveAsync()).get();
			Assert::IsTrue(bRet);
		}// TestDBManagerIsAlive
		TEST_METHOD(TestDBManagerMaintains)
		{
			IMap<String^, Object^>^ xMap = ref new Map<String^, Object^>();
			xMap->Insert("type", "testtype");
			//
			int nc = create_task(m_pman->GetDocumentsCountAsync(xMap)).get();
			if (nc < 1) {
				bool bRet = create_task(m_pman->MaintainsDocumentAsync(m_map)).get();
				Assert::IsTrue(bRet);
			}// nc
			IVector<IMap<String^, Object^>^>^ vv = create_task(m_pman->GetDocumentsAsync(xMap,0,nc)).get();
			Assert::IsTrue(vv != nullptr);
			auto it = vv->First();
			Assert::IsTrue(it->HasCurrent);
			IMap<String^, Object^>^ pMap = it->Current;
			Assert::IsTrue(pMap->HasKey("_id"));
			String^ id = pMap->Lookup("_id")->ToString();
			Assert::IsFalse(id->IsEmpty());
			Assert::IsTrue(pMap->HasKey("_rev"));
			String^ res = pMap->Lookup("_rev")->ToString();
			Assert::IsFalse(res->IsEmpty());
			bool bRet = create_task(m_pman->MaintainsDocumentAsync(pMap)).get();
			Assert::IsTrue(bRet);
		}//TestDBManagerMaintains
	};
}