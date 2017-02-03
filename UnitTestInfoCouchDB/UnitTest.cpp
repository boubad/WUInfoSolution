#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace InfoCouchDB;
using namespace Platform;
using namespace concurrency;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Platform::Collections;
////////////////////////////////////
namespace UnitTestInfoCouchDB
{
    TEST_CLASS(UnitTestCoucDB)
    {
	public:
		String^ m_baseUrl;
		String^ m_database;
		CouchDB^ pMan;
	public:
		TEST_METHOD_INITIALIZE(SetUp) {
			m_baseUrl = "http://localhost:5984/";
			m_database = "test";
			pMan = ref new CouchDB(m_baseUrl, m_database);
		}// SetUp
		TEST_METHOD_CLEANUP(TearDown) {
		}// TearDown
	public:
		TEST_METHOD(TestDummy)
		{
			bool bVal = true;
			int ival = 123;
			double dval = 5.67;
			String^ sval = "sval";
			Object^ obval = bVal;
			Object^ oival = ival;
			Object^ odval = dval;
			Object^ osval = sval;
			//
			Type^ tbool = obval->GetType();
			auto r1 = Type::GetTypeCode(obval->GetType());
			auto r11 = Type::GetTypeCode(bVal.GetType());
			//
			auto r2 = Type::GetTypeCode(oival->GetType());
			auto r22 = Type::GetTypeCode(ival.GetType());
			//
			auto r3 = Type::GetTypeCode(odval->GetType());
			auto r33 = Type::GetTypeCode(dval.GetType());
			//
			auto r4 = Type::GetTypeCode(osval->GetType());
			auto r44 = Type::GetTypeCode(sval->GetType());
			//
			bool dummy = false;
		}// TestDummy
		TEST_METHOD(CouchDB_TestIsServerAlive)
		{
			auto b = pMan->IsServerAliveAsync();
			create_task(b).then([](bool bRet) {
				Assert::IsTrue(bRet);
			}).wait();
			//
		}// CouchDB_TestIsServerAlive
		TEST_METHOD(CouchDB_TestIsDatabaseExists)
		{
			auto b = pMan->IsDatabaseExistsAsync(m_database);
			create_task(b).then([](bool bRet) {
				Assert::IsTrue(bRet);
			}).wait();
			//
		}// CouchDB_TestIsDatabaseExists
		TEST_METHOD(CouchDB_TestGetServerInfo)
		{
			auto b = pMan->GetServerInfoAsync();
			create_task(b).then([](Object^ obj) {
				IObservableMap<String^, Object^>^ pMap = (IObservableMap<String^, Object^>^)obj;
				auto it = pMap->First();
				Assert::IsTrue(it->HasCurrent);
				while (it->HasCurrent) {
					auto p = it->Current;
					String^ key = p->Key;
					Object^ val = p->Value;
					String^ s = key + " = " + val->ToString();
					Logger::WriteMessage(s->Data());
					it->MoveNext();
				}// it
			}).wait();
			//
		}// CouchDB_TestGetServerInfo
		TEST_METHOD(CouchDB_TestGetDatabaseInfo)
		{
			auto b = pMan->GetDatabaseInfoAsync(m_database);
			create_task(b).then([](Object^ obj) {
				IObservableMap<String^, Object^>^ pMap = (IObservableMap<String^, Object^>^)obj;
				auto it = pMap->First();
				Assert::IsTrue(it->HasCurrent);
				while (it->HasCurrent) {
					auto p = it->Current;
					String^ key = p->Key;
					Object^ val = p->Value;
					String^ s = key + " = " + val->ToString();
					Logger::WriteMessage(s->Data());
					it->MoveNext();
				}// it
			}).wait();
			//
		}// CouchDB_TestGetDatabaseInfo
		TEST_METHOD(CouchDB_CreateDeleteDatabase)
		{
			String^ dbName = "xxtextxx";
			CouchDB^ pman = pMan;
			auto b = pMan->IsDatabaseExistsAsync(dbName);
			create_task(b).then([pman, dbName](bool bRet) {
				if (!bRet) {
					auto bb = pman->CreateDatabaseAsync(dbName);
					create_task(bb).then([](bool bx) {
						Assert::IsTrue(bx);
					}).wait();
				}// create
			}).wait();
			//
			auto br = pMan->DeleteDatabaseAsync(dbName);
			create_task(br).then([pman, dbName](bool bRet) {
				Assert::IsTrue(bRet);
			}).wait();
		}// CouchDB_TestIsDatabaseExists
		TEST_METHOD(CouchDB_CreateIndex)
		{
			String^ indexname = "i_testindex";
			Map<String^, bool>^ fields = ref new Map<String^, bool>();
			fields->Insert("field1", false);
			//fields->Insert("field2", true);
			//fields->Insert("field3", false);
			CouchDB^ pman = pMan;
			auto b = pMan->CreateIndexAsync(indexname, fields);
			create_task(b).then([](bool bRet) {
				Assert::IsTrue(bRet);
			}).wait();
		}// CouchDB_CreateIndex
    };
}