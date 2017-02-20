#include "pch.h"
////////////////////
#include "couchdbmanager.h"
#include <robuffer.h>
/////////////////////////////
using namespace InfoCouchDB;
using namespace concurrency;
using namespace Microsoft::WRL;
/////////////////////////////////
CouchDBManager::byte *CouchDBManager::GetPointerToBufferData(IBuffer^ pBuffer, unsigned int *pLength) {
	if (pLength != nullptr)
	{
		*pLength = pBuffer->Length;
	}
	// Query the IBufferByteAccess interface.
	ComPtr<IBufferByteAccess> bufferByteAccess;
	reinterpret_cast<IInspectable*>(pBuffer)->QueryInterface(IID_PPV_ARGS(&bufferByteAccess));
	// Retrieve the buffer data.
	byte* pixels = nullptr;
	bufferByteAccess->Buffer(&pixels);
	return pixels;
}//GetPointerToBufferData
/////////////////////////
CouchDBManager::CouchDBManager() {

}
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
	m_url = ref new String(xurl.c_str());
	m_dataBase = databaseName;
}//CouchDBManager
void CouchDBManager::SetUrlDatabase(String^ url, String^ database) {
	m_url = url;
	m_dataBase = database;
	m_proxy.reset();
}//SetUrlDatabase
String^ CouchDBManager::BaseUrl::get() {
	return m_url;
}
void CouchDBManager::BaseUrl::set(String ^baseUrl) {
	if (baseUrl == nullptr) {
		throw ref new InvalidArgumentException();
	}
	if (baseUrl->IsEmpty()) {
		throw ref new InvalidArgumentException();
	}
	std::wstring xurl{ baseUrl->Data() };
	auto it = xurl.end();
	--it;
	if ((*it) != L'/') {
		xurl = xurl + L"/";
	}
	m_url = ref new String(xurl.c_str());
	m_proxy.reset();
}
String^ CouchDBManager::DatabaseName::get() {
	return m_dataBase;
}
void CouchDBManager::DatabaseName::set(String ^s) {
	if (s == nullptr) {
		throw ref new InvalidArgumentException();
	}
	if (s->IsEmpty()) {
		throw ref new InvalidArgumentException();
	}
	m_dataBase = s;
	m_proxy.reset();
}
CouchDBProxy *CouchDBManager::GetProxy(void) {
	CouchDBProxy *p = m_proxy.get();
	if (p == nullptr) {
		if ((m_url != nullptr) && (m_dataBase != nullptr) &&
			(!m_url->IsEmpty()) && (!m_dataBase->IsEmpty())) {
			m_proxy.reset(new CouchDBProxy{m_url,m_dataBase});
			p = m_proxy.get();
		}
	}// p
	if (p == nullptr) {
		throw ref new FailureException("CouchDBProxy object not created.");
	}
	return p;
}// GetProxt
/////////////////////////////////////
IAsyncOperation<bool>^ CouchDBManager::MaintainsDocumentAttachmentAsync(String^ docid, String^ attachmentName,IStorageFile^ file) {
	if ((docid == nullptr) || (attachmentName == nullptr) || (file == nullptr)) {
		throw ref new InvalidArgumentException();
	}
	if (docid->IsEmpty() || attachmentName->IsEmpty()) {
		throw ref new InvalidArgumentException();
	}
	CouchDBProxy *pProxy = GetProxy();
	return create_async([pProxy, docid, attachmentName, file]()->bool {
		return pProxy->MaintainsDocumentAttachmentAsync(docid, attachmentName, file).get();
	});
}//:MaintainsDocumentAttachmentAsync
IAsyncOperation<bool>^ CouchDBManager::MaintainsDocumentAttachmentAsync(String^ docid, String^ attachmentName, String^ mimetype, IBuffer^ data) {
	if ((docid == nullptr) || (attachmentName == nullptr) || (mimetype == nullptr) || (data == nullptr)) {
		throw ref new InvalidArgumentException();
	}
	if (docid->IsEmpty() || attachmentName->IsEmpty() || mimetype->IsEmpty()) {
		throw ref new InvalidArgumentException();
	}
	if (data->Length < 1) {
		throw ref new  InvalidArgumentException();
	}
	CouchDBProxy *pProxy = GetProxy();
	return create_async([pProxy, docid, attachmentName, mimetype, data]()->bool {
		return pProxy->MaintainsDocumentAttachmentAsync(docid, attachmentName, mimetype, data).get();
	});
}//:MaintainsDocumentAttachmentAsync
IAsyncOperation<IMap<String^, String^> ^>^ CouchDBManager::GetDocumentAttachmentNamesAsync(String^ docid) {
	if (docid == nullptr) {
		throw ref new InvalidArgumentException();
	}
	if (docid->IsEmpty()) {
		throw ref new InvalidArgumentException();
	}
	CouchDBProxy *pProxy = GetProxy();
	return create_async([pProxy, docid]()->IMap<String^, String^>^ {
		return pProxy->GetDocumentAttachmentNamesAsync(docid).get();
	});
}//CouchDBManager::
IAsyncOperation<bool>^ CouchDBManager::RemoveDocumentAttachmentAsync(String^ docid, String^ attachmentName) {
	if ((docid == nullptr) || (attachmentName == docid)) {
		throw ref new InvalidArgumentException();
	}
	if (docid->IsEmpty() || attachmentName->IsEmpty()) {
		throw ref new InvalidArgumentException();
	}
	CouchDBProxy *pProxy = GetProxy();
	return create_async([pProxy, docid, attachmentName]()->bool {
		return pProxy->RemoveDocumentAttachmentAsync(docid, attachmentName).get();
	});
}//RemoveDocumentAttachmentAsync
IAsyncOperation<IBuffer^>^ CouchDBManager::GetDocumentAttachmentDataAsync(String^ docid, String^ attachmentName) {
	if ((docid == nullptr) || (attachmentName == docid)) {
		throw ref new InvalidArgumentException();
	}
	if (docid->IsEmpty() || attachmentName->IsEmpty()) {
		throw ref new InvalidArgumentException();
	}
	CouchDBProxy *pProxy = GetProxy();
	return create_async([pProxy, docid, attachmentName]()->IBuffer^ {
		return pProxy->GetDocumentAttachmentDataAsync(docid, attachmentName).get();
	});
}//GetDocumentAttachmentDataAsync
IAsyncOperation<IVector< CouchDBManager::byte>^>^ CouchDBManager::GetDocumentAttachmentDataVectorAsync(String^ docid, String^ attachmentName) {
	if ((docid == nullptr) || (attachmentName == docid)) {
		throw ref new InvalidArgumentException();
	}
	if (docid->IsEmpty() || attachmentName->IsEmpty()) {
		throw ref new InvalidArgumentException();
	}
	CouchDBProxy *pProxy = GetProxy();
	return create_async([pProxy, docid, attachmentName]()->IVector<byte>^ {
		IVector<byte>^ pRet = nullptr;
		IBuffer^ pBuf = pProxy->GetDocumentAttachmentDataAsync(docid, attachmentName).get();
		if (pBuf != nullptr) {
			unsigned int len{ 0 };
			byte *pData = GetPointerToBufferData(pBuf, &len);
			if ((pData != nullptr) && (len > 0)) {
				pRet = ref new Platform::Collections::Vector<byte>(pData, len);
			}
		}// pBuf
		return (pRet);
	});
}// GetDocumentAttachmentDataVectorAsync
//////////////////////////////////
IAsyncOperation<bool>^ CouchDBManager::IsAliveAsync(void) {
	CouchDBProxy *pProxy = GetProxy();
	return create_async([pProxy]()->bool {
		return pProxy->IsAliveAsync().get();
	});
}//IsAliveAsync
IAsyncOperation<int>^ CouchDBManager::GetDocumentsCountAsync(IMap<String^, Object^>^ oFetch) {
	if (oFetch == nullptr) {
		throw ref new InvalidArgumentException("Null filter data.");
	}
	if (oFetch->Size < 1) {
		throw ref new InvalidArgumentException("Empty filter data");
	}
	CouchDBProxy *pProxy = GetProxy();
	return create_async([pProxy, oFetch]()->int {
		return pProxy->GetCountFilterAsync(oFetch).get();
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
	CouchDBProxy *pProxy = GetProxy();
	return create_async([pProxy, oFetch, offset, count]()->IVector<IMap<String^, Object^>^>^ {
		return pProxy->ReadDocumentsAsync(oFetch, offset, count).get();
	});
}//GetDocumentsAsync
IAsyncOperation<IMap<String^, Object^>^>^ CouchDBManager::FindDocumentAsync(IMap<String^, Object^>^ oFetch) {
	if (oFetch == nullptr) {
		throw ref new InvalidArgumentException("Null filter data.");
	}
	CouchDBProxy *pProxy = GetProxy();
	return create_async([pProxy, oFetch]()->IMap<String^, Object^>^ {
		return pProxy->FindDocumentAsync(oFetch).get();
	});
}//FindDocumentAsync
IAsyncOperation<IMap<String^, Object^>^>^ CouchDBManager::ReadDocumentByIdAsync(String^ docid) {
	if (docid == nullptr) {
		throw ref new InvalidArgumentException("Null document id.");
	}
	if (docid->IsEmpty()) {
		throw ref new InvalidArgumentException("Empty document id.");
	}
	CouchDBProxy *pProxy = GetProxy();
	return create_async([pProxy, docid]()->IMap<String^, Object^>^ {
		return pProxy->ReadDocumentByIdAsync(docid).get();
	});
}//ReadDocumentByIdAsync
IAsyncOperation<bool>^ CouchDBManager::MaintainsDocumentAsync(IMap<String^, Object^>^ oMap) {
	if (oMap == nullptr) {
		throw ref new InvalidArgumentException("Null document map.");
	}
	CouchDBProxy *pProxy = GetProxy();
	return create_async([pProxy, oMap]()->bool {
		return pProxy->MaintainsDocumentAsync(oMap).get();
	});
}//:MaintainsDocumentAsync
IAsyncOperation<bool>^ CouchDBManager::DeleteDocumentByIdAsync(String^ docid) {
	if (docid == nullptr) {
		throw ref new InvalidArgumentException("Null document id.");
	}
	if (docid->IsEmpty()) {
		throw ref new InvalidArgumentException("Empty document id.");
	}
	CouchDBProxy *pProxy = GetProxy();
	return create_async([pProxy, docid]()->bool {
		return pProxy->DeleteDocumentByIdAsync(docid).get();
	});
}//DeleteDocumentByIdAsync
IAsyncOperation<bool>^ CouchDBManager::MaintainsDocumentsAsync(IVector<IMap<String^, Object^>^>^ oVec, bool bDelete) {
	if (oVec == nullptr) {
		throw ref new InvalidArgumentException("Null documents vector.");
	}
	CouchDBProxy *pProxy = GetProxy();
	return create_async([pProxy, oVec, bDelete]()->bool {
		return pProxy->MaintainsDocumentsAsync(oVec, bDelete).get();
	});
}//MaintainsDocumentsAsync
IAsyncOperation<bool>^ CouchDBManager::RemoveDocumentsAsync(IMap<String^, Object^>^ oFetch) {
	if (oFetch == nullptr) {
		throw ref new InvalidArgumentException("Null filter data.");
	}
	CouchDBProxy *pProxy = GetProxy();
	return create_async([pProxy, oFetch]()->bool {
		return pProxy->RemoveDocumentsAsync(oFetch).get();
	});
}//:RemoveDocumentsAsync
