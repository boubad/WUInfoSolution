#pragma once
#include "couchdbproxy.h"
/////////////////////////////
#include <memory>
////////////////////////////
namespace InfoCouchDB {
	/////////////////////////////
	[Windows::Foundation::Metadata::WebHostHiddenAttribute]
	public ref class CouchDBManager sealed {
	private:
		std::unique_ptr<CouchDBProxy> m_proxy;
	public:
		CouchDBManager(String^ baseUrl, String^ databaseName);
	public:
		IAsyncOperation<bool>^ IsAliveAsync(void);
		IAsyncOperation<int>^ GetDocumentsCountAsync(IMap<String^, Object^>^ oFetch);
		IAsyncOperation<IVector<IMap<String^, Object^>^>^>^ GetDocumentsAsync(IMap<String^, Object^>^ oFetch, int offset, int count);
		IAsyncOperation<IMap<String^, Object^>^>^ FindDocumentAsync(IMap<String^, Object^>^ oFetch);
		IAsyncOperation<IMap<String^, Object^>^>^ ReadDocumentByIdAsync(String^ docid);
		IAsyncOperation<bool>^ MaintainsDocumentAsync(IMap<String^, Object^>^ oMap);
		IAsyncOperation<bool>^ DeleteDocumentByIdAsync(String^ docid);
		IAsyncOperation<bool>^ MaintainsDocumentsAsync(IVector<IMap<String^, Object^>^>^ oVec, bool bDelete);
		IAsyncOperation<bool>^ RemoveDocumentsAsync(IMap<String^, Object^>^ oFetch);
	};// class CouchDBManager
	/////////////////////////////////
}// InfoCouchDN
