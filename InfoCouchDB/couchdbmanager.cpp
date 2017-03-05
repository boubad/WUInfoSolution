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
	if ((baseUrl == nullptr) || baseUrl->IsEmpty()) {
		throw ref new InvalidArgumentException("Bad BaseUrl");
	}
	if ((baseUrl == nullptr) || databaseName->IsEmpty()) {
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
IAsyncAction^ CouchDBManager::CheckDatabaseAsync(String^ baseUrl, String^ databaseName) {
	return create_async([baseUrl, databaseName]() {
		if ((baseUrl == nullptr) || baseUrl->IsEmpty()) {
			throw ref new InvalidArgumentException("Bad BaseUrl");
		}
		if ((baseUrl == nullptr) || databaseName->IsEmpty()) {
			throw ref new InvalidArgumentException("Bad Database name.");
		}
		std::wstring xurl{ baseUrl->Data() };
		auto it = xurl.end();
		--it;
		if ((*it) != L'/') {
			xurl = xurl + L"/";
		}
		String^ sUrl = ref new String(xurl.c_str());
		String^ sBase = databaseName;
		CouchDBProxy::CheckDatabaseAsync(sUrl, sBase).wait();
	});
}//CheckDatabaseAsync
IAsyncOperation<bool>^ CouchDBManager::ExistsDatabaseAsync(String^ baseUrl, String^ databaseName) {
	return create_async([baseUrl, databaseName]() ->bool {
		if ((baseUrl == nullptr) || baseUrl->IsEmpty()) {
			throw ref new InvalidArgumentException("Bad BaseUrl");
		}
		if ((baseUrl == nullptr) || databaseName->IsEmpty()) {
			throw ref new InvalidArgumentException("Bad Database name.");
		}
		std::wstring xurl{ baseUrl->Data() };
		auto it = xurl.end();
		--it;
		if ((*it) != L'/') {
			xurl = xurl + L"/";
		}
		String^ sUrl = ref new String(xurl.c_str());
		String^ sBase = databaseName;
		CouchDBManager^ pMan = ref new CouchDBManager(sUrl, sBase);
		CouchDBProxy pp{sUrl,sBase};
		bool bRet = create_task(pp.ExistsDatabaseAsync(sBase)).get();
		return (bRet);
	});
}//ExistsDatabaseAsync
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
			m_proxy.reset(new CouchDBProxy{ m_url,m_dataBase });
			p = m_proxy.get();
		}
	}// p
	if (p == nullptr) {
		throw ref new FailureException("CouchDBProxy object not created.");
	}
	return p;
}// GetProxt
/////////////////////////////////////
IAsyncOperation<IVector<Object^>^>^ CouchDBManager::GetFieldsDistinctAsync(String^ fname) {
	return create_async([this, fname]()->IVector<Object^>^ {
		if ((fname == nullptr) || fname->IsEmpty()) {
			throw ref new InvalidArgumentException();
		}
		CouchDBProxy *pProxy = GetProxy();
		IVector<Object^>^ pRet = pProxy->GetFieldsDistinctAsync(fname).get();
		return (pRet);
	});
}//GetFieldsDistinctAsync
IAsyncOperation<bool>^ CouchDBManager::MaintainsDocumentAttachmentAsync(String^ docid, String^ attachmentName, IStorageFile^ file) {
	return create_async([this, docid, attachmentName, file]()->bool {
		if ((docid == nullptr) || (attachmentName == nullptr) || (file == nullptr)) {
			throw ref new InvalidArgumentException();
		}
		if (docid->IsEmpty() || attachmentName->IsEmpty()) {
			throw ref new InvalidArgumentException();
		}
		CouchDBProxy *pProxy = GetProxy();
		return pProxy->MaintainsDocumentAttachmentAsync(docid, attachmentName, file).get();
	});
}//:MaintainsDocumentAttachmentAsync
IAsyncOperation<bool>^ CouchDBManager::MaintainsDocumentAttachmentAsync(String^ docid, String^ attachmentName, String^ mimetype, IBuffer^ data) {
	return create_async([this, docid, attachmentName, mimetype, data]()->bool {
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
		bool b =  pProxy->MaintainsDocumentAttachmentAsync(docid, attachmentName, mimetype, data).get();
		return (b);
	});
}//:MaintainsDocumentAttachmentAsync
IAsyncOperation<IMap<String^, String^> ^>^ CouchDBManager::GetDocumentAttachmentNamesAsync(String^ docid) {
	return create_async([this, docid]()->IMap<String^, String^>^ {
		if (docid == nullptr) {
			throw ref new InvalidArgumentException();
		}
		if (docid->IsEmpty()) {
			throw ref new InvalidArgumentException();
		}
		CouchDBProxy *pProxy = GetProxy();
		return pProxy->GetDocumentAttachmentNamesAsync(docid).get();
	});
}//CouchDBManager::
IAsyncOperation<bool>^ CouchDBManager::RemoveDocumentAttachmentAsync(String^ docid, String^ attachmentName) {
	return create_async([this, docid, attachmentName]()->bool {
		if ((docid == nullptr) || (attachmentName == docid)) {
			throw ref new InvalidArgumentException();
		}
		if (docid->IsEmpty() || attachmentName->IsEmpty()) {
			throw ref new InvalidArgumentException();
		}
		CouchDBProxy *pProxy = GetProxy();
		return pProxy->RemoveDocumentAttachmentAsync(docid, attachmentName).get();
	});
}//RemoveDocumentAttachmentAsync
IAsyncOperation<IBuffer^>^ CouchDBManager::GetDocumentAttachmentDataAsync(String^ docid, String^ attachmentName) {
	return create_async([this, docid, attachmentName]()->IBuffer^ {
		if ((docid == nullptr) || (attachmentName == docid)) {
			throw ref new InvalidArgumentException();
		}
		if (docid->IsEmpty() || attachmentName->IsEmpty()) {
			throw ref new InvalidArgumentException();
		}
		CouchDBProxy *pProxy = GetProxy();
		return pProxy->GetDocumentAttachmentDataAsync(docid, attachmentName).get();
	});
}//GetDocumentAttachmentDataAsync
IAsyncOperation<IVector< CouchDBManager::byte>^>^ CouchDBManager::GetDocumentAttachmentDataVectorAsync(String^ docid, String^ attachmentName) {
	return create_async([this, docid, attachmentName]()->IVector<byte>^ {
		if ((docid == nullptr) || (attachmentName == docid)) {
			throw ref new InvalidArgumentException();
		}
		if (docid->IsEmpty() || attachmentName->IsEmpty()) {
			throw ref new InvalidArgumentException();
		}
		CouchDBProxy *pProxy = GetProxy();
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
}
IAsyncOperation<bool>^ CouchDBManager::CreateIndexAsync(IVector<String^>^ fields, String ^ name, String ^ designDoc)
{
	return create_async([this, fields, name, designDoc]()->bool {
		CouchDBProxy *pProxy = GetProxy();
		return pProxy->CreateIndexAsync(fields, name, designDoc).get();
	});
}
// GetDocumentAttachmentDataVectorAsync
//////////////////////////////////
IAsyncOperation<bool>^ CouchDBManager::IsAliveAsync(void) {
	return create_async([this]()->bool {
		CouchDBProxy *pProxy = GetProxy();
		return pProxy->IsAliveAsync().get();
	});
}//IsAliveAsync
IAsyncOperation<int>^ CouchDBManager::GetDocumentsCountAsync(IMap<String^, Object^>^ oFetch) {
	return create_async([this, oFetch]()->int {
		if (oFetch == nullptr) {
			throw ref new InvalidArgumentException("Null filter data.");
		}
		if (oFetch->Size < 1) {
			throw ref new InvalidArgumentException("Empty filter data");
		}
		CouchDBProxy *pProxy = GetProxy();
		return pProxy->GetCountFilterAsync(oFetch).get();
	});
}//GetDocumentsCountAsync
IAsyncOperation<IVector<IMap<String^, Object^>^>^>^ CouchDBManager::GetDocumentsAsync(IMap<String^, Object^>^ oFetch,
	int offset, int count) {
	return create_async([this, oFetch, offset, count]()->IVector<IMap<String^, Object^>^>^ {
		if (oFetch == nullptr) {
			throw ref new InvalidArgumentException("Null filter data.");
		}
		if (oFetch->Size < 1) {
			throw ref new InvalidArgumentException("Empty filter data");
		}
		CouchDBProxy *pProxy = GetProxy();
		return pProxy->ReadDocumentsAsync(oFetch, offset, count).get();
	});
}//GetDocumentsAsync
IAsyncOperation<IMap<String^, Object^>^>^ CouchDBManager::FindDocumentAsync(IMap<String^, Object^>^ oFetch) {
	return create_async([this, oFetch]()->IMap<String^, Object^>^ {
		if (oFetch == nullptr) {
			throw ref new InvalidArgumentException("Null filter data.");
		}
		CouchDBProxy *pProxy = GetProxy();
		return pProxy->FindDocumentAsync(oFetch).get();
	});
}//FindDocumentAsync
IAsyncOperation<IMap<String^, Object^>^>^ CouchDBManager::ReadDocumentByIdAsync(String^ docid) {
	return create_async([this, docid]()->IMap<String^, Object^>^ {
		if (docid == nullptr) {
			throw ref new InvalidArgumentException("Null document id.");
		}
		if (docid->IsEmpty()) {
			throw ref new InvalidArgumentException("Empty document id.");
		}
		CouchDBProxy *pProxy = GetProxy();
		return pProxy->ReadDocumentByIdAsync(docid).get();
	});
}//ReadDocumentByIdAsync
IAsyncOperation<bool>^ CouchDBManager::MaintainsDocumentAsync(IMap<String^, Object^>^ oMap) {
	return create_async([this, oMap]()->bool {
		if (oMap == nullptr) {
			throw ref new InvalidArgumentException("Null document map.");
		}
		CouchDBProxy *pProxy = GetProxy();
		return pProxy->MaintainsDocumentAsync(oMap).get();
	});
}//:MaintainsDocumentAsync
IAsyncOperation<bool>^ CouchDBManager::DeleteDocumentByIdAsync(String^ docid) {
	return create_async([this, docid]()->bool {
		if (docid == nullptr) {
			throw ref new InvalidArgumentException("Null document id.");
		}
		if (docid->IsEmpty()) {
			throw ref new InvalidArgumentException("Empty document id.");
		}
		CouchDBProxy *pProxy = GetProxy();
		return pProxy->DeleteDocumentByIdAsync(docid).get();
	});
}//DeleteDocumentByIdAsync
IAsyncOperation<bool>^ CouchDBManager::MaintainsDocumentsAsync(IVector<IMap<String^, Object^>^>^ oVec, bool bDelete) {
	return create_async([this, oVec, bDelete]()->bool {
		if (oVec == nullptr) {
			throw ref new InvalidArgumentException("Null documents vector.");
		}
		CouchDBProxy *pProxy = GetProxy();

		return pProxy->MaintainsDocumentsAsync(oVec, bDelete).get();
	});
}//MaintainsDocumentsAsync
IAsyncOperation<bool>^ CouchDBManager::RemoveDocumentsAsync(IMap<String^, Object^>^ oFetch) {
	return create_async([this, oFetch]()->bool {
		if (oFetch == nullptr) {
			throw ref new InvalidArgumentException("Null filter data.");
		}
		CouchDBProxy *pProxy = GetProxy();
		return pProxy->RemoveDocumentsAsync(oFetch).get();
	});
}//:RemoveDocumentsAsync
