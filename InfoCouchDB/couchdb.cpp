#include "pch.h"
#include "couchdb.h"
///////////////////////////
using namespace InfoCouchDB;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Web::Http;
using namespace Windows::Data::Json;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace concurrency;
//
 /////////////////////////
CouchDB::CouchDB(String^ url, String^ database) {
	if (url->IsEmpty()) {
		throw ref new InvalidArgumentException("Bad BaseUrl");
	}
	if (database->IsEmpty()) {
		throw ref new InvalidArgumentException("Bad Database name.");
	}
	std::wstring xurl{ url->Data() };
	auto it = xurl.end();
	--it;
	if ((*it) != L'/') {
		xurl = xurl + L"/";
	}
	String^ purl = ref new String(xurl.c_str());
	m_client.reset(new CouchDBClient(purl, database));
}// CouchDB

IAsyncOperation<bool>^ CouchDB::IsServerAliveAsync(void) {
	return create_async([this]()->bool {
		return this->m_client->IsServerAlive();
	});
}
IAsyncOperation<Object^ >^ CouchDB::GetServerInfoAsync(void) {
	return create_async([this]()->Object^ {
		return this->m_client->GetServerInfo();
	});
}//GetServerInfoAsync
IAsyncOperation<Object^ >^ CouchDB::GetDatabaseInfoAsync(String^ db) {
	return create_async([this,db]()->Object^ {
		return this->m_client->GetDatabaseInfo(db);
	});
}//GetDatabaseInfoAsync
IAsyncOperation<Object^ >^ CouchDB::ReadDocumentByIdAsync(String^ id) {
	return create_async([this, id]()->Object^ {
		return this->m_client->ReadDocumentById(id);
	});
}//ReadDocumentById
IAsyncOperation<bool>^  CouchDB::IsDatabaseExistsAsync(String^ databaseName) {
	return create_async([this,databaseName]()->bool {
		return this->m_client->IsDatabaseExists(databaseName);
	});
}//IsDatabaseExistsAsync
IAsyncOperation<bool>^ CouchDB::CreateDatabaseAsync(String^ databaseName) {
	return create_async([this, databaseName]()->bool {
		return this->m_client->CreateDatabase(databaseName);
	});
}//CreateDatabaseAsync
IAsyncOperation<bool>^ CouchDB::DeleteDatabaseAsync(String^ databaseName) {
	return create_async([this, databaseName]()->bool {
		return this->m_client->DeleteDatabase(databaseName);
	});
}//DeleteDatabaseAsync
IAsyncOperation<bool>^ CouchDB::CreateIndexAsync(String^ indexname, IMap<String^, bool>^ fields) {
	return create_async([this, indexname,fields]()->bool {
		return this->m_client->CreateIndex(indexname, fields);
	});
}//CreateIndexAsync
