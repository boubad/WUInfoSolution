#include "pch.h"
////////////////////
#include "couchdbmanager.h"
/////////////////////////////
using namespace InfoCouchDB;
using namespace concurrency;
/////////////////////////
CouchDBManager::CouchDBManager(String^ baseUrl, String^ databaseName) {
	if (baseUrl->IsEmpty()) {
		throw ref new InvalidArgumentException("Bad BaseUrl");
	}
	if (databaseName->IsEmpty()) {
		throw ref new InvalidArgumentException("Bad Database name.");
	}
	std::wstring xurl{ baseUrl->Data() };
	auto it = xurl.end();
	--it;
	if ((*it) != L'/') {
		xurl = xurl + L"/";
	}
	String^ purl = ref new String(xurl.c_str());
	m_proxy.reset(new CouchDBProxy(purl, databaseName));
}//CouchDBManager
//////////////////////////////////
IAsyncOperation<bool>^ CouchDBManager::IsAliveAsync(void) {
	CouchDBProxy *pProxy = m_proxy.get();
	if (pProxy == nullptr) {
		throw ref new FailureException("CouchDBProxy object not created.");
	}
	return create_async([pProxy]()->bool {
		return pProxy->IsAlive();
	});
}//IsAliveAsync
IAsyncOperation<int>^ CouchDBManager::GetDocumentsCountAsync(IMap<String^, Object^>^ oFetch) {
	if (oFetch == nullptr) {
		throw ref new InvalidArgumentException("Null filter data.");
	}
	if (oFetch->Size < 1) {
		throw ref new InvalidArgumentException("Empty filter data");
	}
	CouchDBProxy *pProxy = m_proxy.get();
	if (pProxy == nullptr) {
		throw ref new FailureException("CouchDBProxy object not created.");
	}
	return create_async([pProxy, oFetch]()->int {
		return pProxy->GetCountFilter(oFetch);
	});
}//GetDocumentsCountAsync
IAsyncOperation<IVector<IMap<String^, Object^>^>^>^ CouchDBManager::GetDocumentsAsync(IMap<String^, Object^>^ oFetch,
	int offset, int count) {
	if (oFetch == nullptr) {
		throw ref new InvalidArgumentException("Null filter data.");
	}
	if (oFetch->Size < 1) {
		throw ref new InvalidArgumentException("Empty filter data");
	}
	CouchDBProxy *pProxy = m_proxy.get();
	if (pProxy == nullptr) {
		throw ref new FailureException("CouchDBProxy object not created.");
	}
	return create_async([pProxy, oFetch, offset, count]()->IVector<IMap<String^, Object^>^>^ {
		return pProxy->ReadDocuments(oFetch, offset, count);
	});
}//GetDocumentsAsync
IAsyncOperation<IMap<String^, Object^>^>^ CouchDBManager::FindDocumentAsync(IMap<String^, Object^>^ oFetch) {
	if (oFetch == nullptr) {
		throw ref new InvalidArgumentException("Null filter data.");
	}
	CouchDBProxy *pProxy = m_proxy.get();
	if (pProxy == nullptr) {
		throw ref new FailureException("CouchDBProxy object not created.");
	}
	return create_async([pProxy, oFetch]()->IMap<String^, Object^>^ {
		return pProxy->FindDocument(oFetch);
	});
}//FindDocumentAsync
IAsyncOperation<IMap<String^, Object^>^>^ CouchDBManager::ReadDocumentByIdAsync(String^ docid) {
	if (docid == nullptr) {
		throw ref new InvalidArgumentException("Null document id.");
	}
	if (docid->IsEmpty()) {
		throw ref new InvalidArgumentException("Empty document id.");
	}
	CouchDBProxy *pProxy = m_proxy.get();
	if (pProxy == nullptr) {
		throw ref new FailureException("CouchDBProxy object not created.");
	}
	return create_async([pProxy, docid]()->IMap<String^, Object^>^ {
		return pProxy->ReadDocumentById(docid);
	});
}//ReadDocumentByIdAsync
IAsyncOperation<bool>^ CouchDBManager::MaintainsDocumentAsync(IMap<String^, Object^>^ oMap) {
	if (oMap == nullptr) {
		throw ref new InvalidArgumentException("Null document map.");
	}
	CouchDBProxy *pProxy = m_proxy.get();
	if (pProxy == nullptr) {
		throw ref new FailureException("CouchDBProxy object not created.");
	}
	return create_async([pProxy, oMap]()->bool {
		return pProxy->MaintainsDocument(oMap);
	});
}//:MaintainsDocumentAsync
IAsyncOperation<bool>^ CouchDBManager::DeleteDocumentByIdAsync(String^ docid) {
	if (docid == nullptr) {
		throw ref new InvalidArgumentException("Null document id.");
	}
	if (docid->IsEmpty()) {
		throw ref new InvalidArgumentException("Empty document id.");
	}
	CouchDBProxy *pProxy = m_proxy.get();
	if (pProxy == nullptr) {
		throw ref new FailureException("CouchDBProxy object not created.");
	}
	return create_async([pProxy, docid]()->bool {
		return pProxy->DeleteDocumentById(docid);
	});
}//DeleteDocumentByIdAsync
IAsyncOperation<bool>^ CouchDBManager::MaintainsDocumentsAsync(IVector<IMap<String^, Object^>^>^ oVec, bool bDelete) {
	if (oVec == nullptr) {
		throw ref new InvalidArgumentException("Null documents vector.");
	}
	CouchDBProxy *pProxy = m_proxy.get();
	if (pProxy == nullptr) {
		throw ref new FailureException("CouchDBProxy object not created.");
	}
	return create_async([pProxy, oVec, bDelete]()->bool {
		return pProxy->MaintainsDocuments(oVec, bDelete);
	});
}//MaintainsDocumentsAsync
IAsyncOperation<bool>^ CouchDBManager::RemoveDocumentsAsync(IMap<String^, Object^>^ oFetch) {
	if (oFetch == nullptr) {
		throw ref new InvalidArgumentException("Null filter data.");
	}
	CouchDBProxy *pProxy = m_proxy.get();
	if (pProxy == nullptr) {
		throw ref new FailureException("CouchDBProxy object not created.");
	}
	return create_async([pProxy, oFetch]()->bool {
		return pProxy->RemoveDocuments(oFetch);
	});
}//:RemoveDocumentsAsync