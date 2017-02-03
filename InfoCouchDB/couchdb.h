#pragma once
#include "couchdbclient.h"
//
namespace InfoCouchDB
{
	[Windows::Foundation::Metadata::WebHostHiddenAttribute]
	public ref class CouchDB sealed {
		std::unique_ptr<CouchDBClient> m_client;
	public:
		CouchDB(String^ url, String^ database);
		//
		IAsyncOperation<bool>^ IsServerAliveAsync(void);
		IAsyncOperation<Object^ >^ ReadDocumentByIdAsync(String^ id);
		IAsyncOperation<Object^ >^ GetServerInfoAsync(void);
		//
		IAsyncOperation<bool>^  IsDatabaseExistsAsync(String^ databaseName);
		IAsyncOperation<Object^ >^ GetDatabaseInfoAsync(String^ db);
		IAsyncOperation<bool>^ CreateDatabaseAsync(String^ databaseName);
		IAsyncOperation<bool>^ DeleteDatabaseAsync(String^ databaseName);
		//
		IAsyncOperation<bool>^ CreateIndexAsync(String^ indexname, IMap<String^, bool>^ fields);
		//
	};
}// namespace InfoWRC
