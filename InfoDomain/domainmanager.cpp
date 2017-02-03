#include  "pch.h"
////////////////////
#include "domainmanager.h"
#include "infostrings.h"
////////////////////
using namespace Platform::Collections;
using namespace concurrency;
using namespace InfoCouchDB;
using namespace InfoDomain;
///////////////////////////
DomainManager::DomainManager(String^ baseUrl, String^ databaseName) {
	if ((baseUrl == nullptr) || (databaseName == nullptr)) {
		throw ref new InvalidArgumentException("Null baseUrl and (or) databaseBame");
	}
	if (baseUrl->IsEmpty() || databaseName->IsEmpty()) {
		throw ref new InvalidArgumentException("Empty baseUrl and (or) databaseBame");
	}
	m_pman = ref new CouchDBManager(baseUrl, databaseName);
}//DomainManager
IAsyncOperation<int>^ DomainManager::GetDatasetsCountAsync(void) {
	CouchDBManager^ pMan = this->m_pman;
	return create_async([pMan]()->int {
		Dataset^ pSet = ref new Dataset();
		IMap<String^, Object^>^ oMap = pSet->GetMap();
		int nRet = create_task(pMan->GetDocumentsCountAsync(oMap)).get();
		return (nRet);
	});
}//GetDatasetsCountAsync
IAsyncOperation<IVector<Dataset^>^>^ DomainManager::GetDatasetsAsync(int offset, int count) {
	CouchDBManager^ pMan = this->m_pman;
	return create_async([pMan, offset, count]()->IVector<Dataset^>^ {
		Dataset^ pSet = ref new Dataset();
		IMap<String^, Object^>^ oMap = pSet->GetMap();
		IVector<IMap<String^, Object^>^>^ pVec = create_task(pMan->GetDocumentsAsync(oMap, offset, count)).get();
		IVector<Dataset^>^ pRet = ref new Vector<Dataset^>();
		if (pVec != nullptr) {
			auto it = pVec->First();
			while (it->HasCurrent) {
				IMap<String^, Object^>^ m = it->Current;
				if (m != nullptr) {
					Dataset^ p = ref new Dataset(m);
					pRet->Append(p);
					it->MoveNext();
				}
			}// it
		}// pVec
		return (pRet);
	});
}//GetDatasetsAsync
IAsyncOperation<Dataset^>^ DomainManager::FindDatasetAsync(Dataset^ model) {
	if (model == nullptr) {
		throw ref new InvalidArgumentException();
	}
	CouchDBManager^ pMan = this->m_pman;
	return create_async([pMan, model]()->Dataset^ {
		Dataset^ oRet;
		IMap<String^, Object^>^ oFetch = model->GetMap();
		IMap<String^, Object^>^ oMap = create_task(pMan->FindDocumentAsync(oFetch)).get();
		if (oMap != nullptr) {
			if (oMap->HasKey(InfoStrings::KEY_ID) && oMap->HasKey(InfoStrings::KEY_REV)) {
				oRet = ref new Dataset(oMap);
			}
		}// oMap
		return (oRet);
	});
}//FindDatasetAsy
IAsyncOperation<bool>^ DomainManager::MaintainsDatasetAsync(Dataset^ model) {
	if (model == nullptr) {
		throw ref new InvalidArgumentException("Null Argument");
	}
	if (!model->IsStoreable) {
		throw ref new InvalidArgumentException("Not Storeable Dataset");
	}
	CouchDBManager^ pMan = this->m_pman;
	return create_async([pMan, model]()->bool {
		Dataset^ oRet;
		IMap<String^, Object^>^ oFetch = model->GetMap();
		bool bRet = create_task(pMan->MaintainsDocumentAsync(oFetch)).get();
		return (bRet);
	});
}//MaintainsDatasetAsync
IAsyncOperation<bool>^ DomainManager::RemoveDatasetAsync(Dataset^ model) {
	if (model == nullptr) {
		throw ref new InvalidArgumentException("Null Argument");
	}
	if (!model->IsPersisted) {
		throw ref new InvalidArgumentException("Not Persisted Dataset");
	}
	String^ sigle = model->Sigle;
	if (sigle->IsEmpty()) {
		throw ref new InvalidArgumentException("Invalid Dataset sigle");
	}
	CouchDBManager^ pMan = this->m_pman;
	return create_async([pMan, model]()->bool {
		IMap<String^, Object^>^ oFetch = ref new Map<String^, Object^>();
		oFetch->Insert(InfoStrings::KEY_DATASETSIGLE, model->Sigle);
		bool bRet = create_task(pMan->RemoveDocumentsAsync(oFetch)).get();
		if (bRet) {
			String^ docid = model->Id;
			bRet = create_task(pMan->DeleteDocumentByIdAsync(docid)).get();
		}
		return (bRet);
	});
}//RemoveDatasetAsync
