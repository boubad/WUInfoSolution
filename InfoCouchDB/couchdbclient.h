#pragma once
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Platform::Collections;
using namespace Windows::Data::Json;
using namespace concurrency;
using namespace Windows::Web::Http;
//
namespace InfoCouchDB
{
     class CouchDBClient  {
	 public:
		 String^ m_url;
		 String^ m_database;
		 HttpClient^ m_client;
		 //
		 CouchDBClient();
		 ~CouchDBClient();
		//
		CouchDBClient(String^ url, String^ database);
		Platform::Object^ ConvertValue(IJsonValue^ val);
		//////////////////////////////////////
		Platform::Object^ ReadGetValue(String^ sUri);
		///////////////////////////////////////////
		bool IsServerAlive(void);
		Platform::Object^ GetServerInfo(void);
		Platform::Object^ GetDatabaseInfo(String^ db);
		Platform::Object^ ReadDocumentById(String^ id);
		bool IsDatabaseExists(String^ databaseName);
		bool CreateDatabase(String^ databaseName);
		bool DeleteDatabase(String^ databaseName);
		//
		bool CreateIndex(String^ indexname, IMap<String^, bool>^ fields);
		//
		int GetCountFilter(IMap<String^, Object^>^ oFetch);
		IVector<IMap<String^, Object^>^>^ ReadDocuments(IMap<String^, Object^>^ oFetch, int offset = 0, int count = 0);
	};// class CouchDBClient
	//
}// namespace InfoWRC
