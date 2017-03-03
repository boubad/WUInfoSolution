#include "pch.h"
#include "DomainDBManager.h"
#include "StringUtils.h"
#include "infostrings.h"
////////////////////////////
#include <mutex>
#include <atomic>
////////////////////////////
static std::atomic<bool> st_initalized{ false };
static std::mutex st_mutex{};
///////////////////////////
using namespace GenDomainData;
using namespace Platform;
using namespace concurrency;
//
DomainDBManager::DomainDBManager()
{
}
void DomainDBManager::OnPropertyChanged(String^ propertyName)
{
	PropertyChanged(this, ref new PropertyChangedEventArgs(propertyName));
}
String^ DomainDBManager::BaseUrl::get() {
	return m_base;
}
void DomainDBManager::BaseUrl::set(String^ s) {
	String^ ss = StringUtils::Trim(s);
	if (ss->IsEmpty()) {
		throw ref new InvalidArgumentException("Null baseUrl");
	}
	std::wstring sx{ ss->Data() };
	auto it = sx.end();
	it--;
	if ((*it) != L'/') {
		sx += L'/';
		ss = ref new String{ sx.c_str() };
	}
	m_base = ss;
	m_pman = nullptr;
	OnPropertyChanged("BaseUrl");
}// baseUrl
String^ DomainDBManager::DatabaseName::get() {
	return m_data;
}
void DomainDBManager::DatabaseName::set(String^ s) {
	String^ ss = StringUtils::ToLower(StringUtils::Trim(s));
	if (ss->IsEmpty()) {
		throw ref new InvalidArgumentException("Null database name");
	}
	m_data = ss;
	m_pman = nullptr;
	OnPropertyChanged("DatabaseName");
}
bool DomainDBManager::IsValid::get() {
	if ((m_base == nullptr) || (m_data == nullptr)) {
		return false;
	}
	if (m_base->IsEmpty() || m_data->IsEmpty()) {
		return false;
	}
	return true;
}
CouchDBManager^ DomainDBManager::Manager::get() {
	if (m_pman == nullptr) {
		if ((m_base == nullptr) || (m_data == nullptr)) {
			throw ref new OperationCanceledException();
		}
		if (m_base->IsEmpty() || m_data->IsEmpty()) {
			throw ref new OperationCanceledException();
		}
		m_pman = ref new CouchDBManager(m_base, m_data);
		if (!st_initalized) {
			check_indexes();
		}
	}
	return this->m_pman;
}// Manager
void DomainDBManager::internal_check_index(String^ field, String^ name) {
	CouchDBManager^ pMan = this->Manager;
	IVector<String^>^ ff = ref new Vector<String^>();
	ff->Append(field);
	String^ designDoc;
	(void)create_task(pMan->CreateIndexAsync(ff, name, designDoc)).get();
}//internal_check_index
void DomainDBManager::check_indexes(void) {
	try {
		std::lock_guard<std::mutex> oLock{ st_mutex };
		internal_check_index(InfoStrings::KEY_TYPE, "i_type");
		internal_check_index(InfoStrings::KEY_STATUS, "i_status");
		internal_check_index(InfoStrings::KEY_SIGLE, "i_sigle");
		internal_check_index(InfoStrings::KEY_DATASETSIGLE, "i_setsigle");
		internal_check_index(InfoStrings::KEY_ANNEE, "i_annee");
		internal_check_index(InfoStrings::KEY_VARIABLESIGLE, "i_varsigle");
		internal_check_index(InfoStrings::KEY_INDIVSIGLE, "i_indsigle");
		internal_check_index(InfoStrings::KEY_DOSSIER, "i_dossier");
		internal_check_index(InfoStrings::KEY_FIRSTNAME, "i_firstname");
		internal_check_index(InfoStrings::KEY_LASTNAME, "i_lastname");
		internal_check_index(InfoStrings::KEY_BIRTHYEAR, "i_birthyear");
		internal_check_index(InfoStrings::KEY_SEXE, "i_sexe");
		internal_check_index(InfoStrings::KEY_SERIEBAC, "i_seriebac");
		internal_check_index(InfoStrings::KEY_OPTIONBAC, "i_optionbac");
		internal_check_index(InfoStrings::KEY_MENTIONBAC, "i_type");
		internal_check_index(InfoStrings::KEY_SUP, "i_sup");
		internal_check_index(InfoStrings::KEY_LYCEE, "i_lycee");
		internal_check_index(InfoStrings::KEY_APB, "i_apb");
		internal_check_index(InfoStrings::KEY_REDOUBLANT, "i_redoublant");
		internal_check_index(InfoStrings::KEY_GROUPE, "i_groupe");
		internal_check_index(InfoStrings::KEY_DEPARTEMENT, "i_departement");
		internal_check_index(InfoStrings::KEY_VILLE, "i_ville");
		internal_check_index(InfoStrings::KEY_SUP, "i_etudessuperieures");
		internal_check_index(InfoStrings::KEY_REDOUBLANT, "i_redoublant");
		st_initalized = true;
	} catch(...){}
}// check_indexes
void DomainDBManager::check_photo(DomainEtudiant ^p) {
	if ((p != nullptr) && p->IsPersisted) {
		String^ attachmentName = p->Avatar;
		if ((attachmentName != nullptr) && (!attachmentName->IsEmpty())) {
			String^ sUri = m_base + m_data + "/" + Uri::EscapeComponent(p->Id) + "/" + Uri::EscapeComponent(attachmentName);
			p->PhotoUrl = sUri;
			p->IsModified = false;
		}// name
	}// p
}//check_photo
IAsyncAction^ DomainDBManager::CheckDatabaseAsync(String^ url, String^ databaseName) {
	return create_async([url, databaseName]() {
		create_task(CouchDBManager::CheckDatabaseAsync(url, databaseName)).wait();
	});
}//CheckDatabaseAsync
///////////////////////////////
IAsyncOperation<DomainEtudiant^>^ DomainDBManager::FindEtudiantByDossierAsync(String^ dossier) {
	return create_async([this, dossier]()->DomainEtudiant^ {
		if ((dossier == nullptr) || dossier->IsEmpty()) {
			throw ref new InvalidArgumentException();
		}
		CouchDBManager^ pMan = this->Manager;
		DomainEtudiant^ oArg = ref new DomainEtudiant();
		oArg->Dossier = dossier;
		DomainEtudiant^ oRet;
		IMap<String^, Object^>^ oFetch = oArg->GetMap();
		IMap<String^, Object^>^ oMap = create_task(pMan->FindDocumentAsync(oFetch)).get();
		if ((oMap != nullptr) && (oMap->Size > 0)) {
			if (oMap->HasKey(InfoStrings::KEY_ID) && oMap->HasKey(InfoStrings::KEY_REV)) {
				oRet = ref new DomainEtudiant{ oMap };
				this->check_photo(oRet);
				
			}
		}// oMap
		return (oRet);
	});
}//FindEtudiantByDossierAsync
IAsyncOperation<int>^ DomainDBManager::GetEtudiantsCountAsync(DomainEtudiant^ pModel) {
	return create_async([this, pModel]()->int {
		CouchDBManager^ pMan = this->Manager;
		DomainEtudiant^ p = (pModel != nullptr) ? pModel : ref new DomainEtudiant();
		IMap<String^, Object^>^ oMap = p->GetMap();
		int nRet = create_task(pMan->GetDocumentsCountAsync(oMap)).get();
		return (nRet);
	});
}//GetEtudiantsCountAsync
IAsyncOperation<IVector<DomainEtudiant^>^>^ DomainDBManager::GetEtudiantsAsync(DomainEtudiant^ pModel, int offset, int count) {
	return create_async([this, pModel, offset, count]()->IVector<DomainEtudiant^>^ {
		CouchDBManager^ pMan = this->Manager;
		DomainEtudiant^ px = (pModel != nullptr) ? pModel : ref new DomainEtudiant();
		IMap<String^, Object^>^ oMap = px->GetMap();
		IVector<IMap<String^, Object^>^>^ pVec = create_task(pMan->GetDocumentsAsync(oMap, offset, count)).get();
		IVector<DomainEtudiant^>^ pRet = ref new Vector<DomainEtudiant^>();
		if (pVec != nullptr) {
			auto it = pVec->First();
			while (it->HasCurrent) {
				IMap<String^, Object^>^ m = it->Current;
				if (m != nullptr) {
					DomainEtudiant^ p = ref new DomainEtudiant{ m };
					this->check_photo(p);
					pRet->Append(p);
					it->MoveNext();
				}
			}// it
		}// pVec
		return (pRet);
	});
}//GetEtudiantsAsync
IAsyncOperation<bool>^ DomainDBManager::MaintainsEtudiantAsync(DomainEtudiant^ model) {
	return create_async([this, model]()->bool {
		if (model == nullptr) {
			throw ref new InvalidArgumentException("Null Argument");
		}
		if (!model->IsStoreable) {
			throw ref new InvalidArgumentException("Not Storeable Etudiant");
		}
		CouchDBManager^ pMan = this->Manager;
		IMap<String^, Object^>^ oFetch = model->GetMap();
		bool bRet = create_task(pMan->MaintainsDocumentAsync(oFetch)).get();
		return (bRet);
	});
}//MaintainsEtudiantAsync
IAsyncOperation<bool>^ DomainDBManager::RemoveEtudiantAsync(DomainEtudiant^ model) {
	return create_async([this, model]()->bool {
		if (model == nullptr) {
			throw ref new InvalidArgumentException("Null Argument");
		}
		if (!model->IsPersisted) {
			throw ref new InvalidArgumentException("Not Persisted Etudiant");
		}
		String^ docid = model->Id;
		CouchDBManager^ pMan = this->Manager;
		bool bRet = create_task(pMan->DeleteDocumentByIdAsync(docid)).get();
		return (bRet);
	});
}//RemoveDatasetAsync
IAsyncOperation<bool>^ DomainDBManager::MaintainsEtudiantsAsync(IVector<DomainEtudiant^>^ oVec, bool bDelete) {
	if (oVec == nullptr) {
		throw ref new InvalidArgumentException();
	}
	CouchDBManager^ pMan = this->Manager;
	return create_async([this, pMan, oVec, bDelete]()->bool {
		bool bRet = true;
		IVector<IMap<String^, Object^>^>^ vv = ref new Vector<IMap<String^, Object^>^>();
		if (!bDelete) {
			auto it = oVec->First();
			while (it->HasCurrent) {
				DomainEtudiant^ p = it->Current;
				if (p != nullptr) {
					if (p->IsStoreable) {
						IMap<String^, Object^>^ m = p->GetMap();
						vv->Append(m);
					}
				}// p
				it->MoveNext();
			}// it
			if (vv->Size > 0) {
				bRet = create_task(pMan->MaintainsDocumentsAsync(vv, false)).get();
			}
		}
		else {
			auto it = oVec->First();
			while (it->HasCurrent) {
				DomainEtudiant^ p = it->Current;
				if (p != nullptr) {
					if (p->IsPersisted) {
						IMap<String^, Object^>^ m = ref new Map<String^, Object^>();
						m->Insert(InfoStrings::KEY_ID, p->Id);
						m->Insert(InfoStrings::KEY_REV, p->Rev);
						vv->Append(m);
					}
				}// p
				it->MoveNext();
			}// it
			if (vv->Size > 0) {
				bRet = create_task(pMan->MaintainsDocumentsAsync(vv, true)).get();
			}
		}
		return (bRet);
	});
}//MaintainsEtudiantsAsync
 ///////////////////////////////
IAsyncOperation<int>^ DomainDBManager::GetDatasetsCountAsync(DomainDataset^ model) {
	return create_async([this, model]()->int {
		CouchDBManager^ pMan = this->Manager;
		DomainDataset^ px = (model != nullptr) ? model : ref new DomainDataset{};
		IMap<String^, Object^>^ oMap = px->GetMap();
		int nRet = create_task(pMan->GetDocumentsCountAsync(oMap)).get();
		return (nRet);
	});
}//GetDatasetsCountAsync
IAsyncOperation<IVector<DomainDataset^>^>^ DomainDBManager::GetDatasetsAsync(DomainDataset^ model, int offset, int count) {
	return create_async([this, model, offset, count]()->IVector<DomainDataset^>^ {
		CouchDBManager^ pMan = this->Manager;
		DomainDataset^ px = (model != nullptr) ? model : ref new DomainDataset{};
		IMap<String^, Object^>^ oMap = px->GetMap();
		IVector<IMap<String^, Object^>^>^ pVec = create_task(pMan->GetDocumentsAsync(oMap, offset, count)).get();
		IVector<DomainDataset^>^ pRet = ref new Vector<DomainDataset^>();
		if (pVec != nullptr) {
			auto it = pVec->First();
			while (it->HasCurrent) {
				IMap<String^, Object^>^ m = it->Current;
				if (m != nullptr) {
					DomainDataset^ p = ref new DomainDataset(m);
					pRet->Append(p);
					it->MoveNext();
				}
			}// it
		}// pVec
		return (pRet);
	});
}//GetDatasetsAsync
IAsyncOperation<DomainDataset^>^ DomainDBManager::FindDatasetAsync(DomainDataset^ model) {
	return create_async([this, model]()->DomainDataset^ {
		if (model == nullptr) {
			throw ref new InvalidArgumentException();
		}
		CouchDBManager^ pMan = this->Manager;
		DomainDataset^ oRet;
		IMap<String^, Object^>^ oFetch = model->GetMap();
		IMap<String^, Object^>^ oMap = create_task(pMan->FindDocumentAsync(oFetch)).get();
		if (oMap != nullptr) {
			if (oMap->HasKey(InfoStrings::KEY_ID) && oMap->HasKey(InfoStrings::KEY_REV)) {
				oRet = ref new DomainDataset(oMap);
			}
		}// oMap
		return (oRet);
	});
}//FindDatasetAsync
IAsyncOperation<DomainDataset^>^ DomainDBManager::FindDatasetBySigleAsync(String^ sigle) {
	return create_async([this, sigle]()->DomainDataset^ {
		if (sigle == nullptr) {
			throw ref new InvalidArgumentException();
		}
		CouchDBManager^ pMan = this->Manager;
		DomainDataset^ oArg = ref new DomainDataset{};
		oArg->Sigle = sigle;
		DomainDataset^ oRet = nullptr;
		IMap<String^, Object^>^ oFetch = oArg->GetMap();
		IMap<String^, Object^>^ oMap = create_task(pMan->FindDocumentAsync(oFetch)).get();
		if (oMap != nullptr) {
			if (oMap->HasKey(InfoStrings::KEY_ID) && oMap->HasKey(InfoStrings::KEY_REV)) {
				oRet = ref new DomainDataset{ oMap };
			}
		}// oMap
		return (oRet);
	});
}//FindDatasetBySigleAsync
IAsyncOperation<bool>^ DomainDBManager::MaintainsDatasetAsync(DomainDataset^ model) {
	return create_async([this, model]()->bool {
		if (model == nullptr) {
			throw ref new InvalidArgumentException("Null Argument");
		}
		if (!model->IsStoreable) {
			throw ref new InvalidArgumentException("Not Storeable Dataset");
		}
		CouchDBManager^ pMan = this->Manager;
		IMap<String^, Object^>^ oFetch = model->GetMap();
		bool bRet = create_task(pMan->MaintainsDocumentAsync(oFetch)).get();
		return (bRet);
	});
}//MaintainsDatasetAsync
IAsyncOperation<bool>^ DomainDBManager::RemoveDatasetAsync(DomainDataset^ model) {
	return create_async([this, model]()->bool {
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
		CouchDBManager^ pMan = this->Manager;
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
IAsyncOperation<bool>^  DomainDBManager::MaintainsDatasetsAsync(IVector<DomainDataset^>^ oVec, bool bDelete) {
	return create_async([this, oVec, bDelete]()->bool {
		if (oVec == nullptr) {
			throw ref new InvalidArgumentException();
		}
		CouchDBManager^ pMan = this->Manager;
		bool bRet = true;
		if (!bDelete) {
			IVector<IMap<String^, Object^>^>^ vv = ref new Vector<IMap<String^, Object^>^>();
			auto it = oVec->First();
			while (it->HasCurrent) {
				DomainDataset^ p = it->Current;
				if (p != nullptr) {
					if (p->IsStoreable) {
						IMap<String^, Object^>^ m = p->GetMap();
						vv->Append(m);
					}
				}// p
				it->MoveNext();
			}// it
			if (vv->Size > 0) {
				bRet = create_task(pMan->MaintainsDocumentsAsync(vv, false)).get();
			}
		}
		else {
			std::vector<task<bool> > oTasks{};
			auto it = oVec->First();
			while (it->HasCurrent) {
				DomainDataset^ p = it->Current;
				if (p != nullptr) {
					if (p->IsPersisted) {
						oTasks.push_back(create_task(this->RemoveDatasetAsync(p)));
					}
				}// p
				it->MoveNext();
			}// it
			if (!oTasks.empty()) {
				std::vector<bool> bb = when_all(oTasks.begin(), oTasks.end()).get();
				for (bool b : bb) {
					if (!b) {
						bRet = false;
						break;
					}
				}
			}// oTasks
		}
		return (bRet);
	});
}
IAsyncOperation<IVector<String^>^>^ DomainDBManager::GetAllDatasetsSigles(void)
{
	return create_async([this]()->IVector<String^>^ {
		int n = create_task(this->GetDatasetsCountAsync(nullptr)).get();
		IVector<String^>^ pRet = ref new Vector<String^>();
		if (n > 0) {
			IVector<DomainDataset^>^ vv = create_task(this->GetDatasetsAsync(nullptr, 0, n)).get();
			if (vv != nullptr) {
				auto it = vv->First();
				while (it->HasCurrent) {
					DomainDataset^ p = it->Current;
					pRet->Append(p->Sigle);
					it->MoveNext();
				}// it
			}// vv
		}// n
		return pRet;
	});
}
// MaintainsDatasetsAsync
///////////////////////////////////
IAsyncOperation<int>^ DomainDBManager::GetDatasetVariablesCountAsync(DomainDataset^ pSet) {
	return create_async([this, pSet]()->int {
		if (pSet == nullptr) {
			throw ref new InvalidArgumentException("Null Dataset");
		}
		if (pSet->Sigle->IsEmpty()) {
			throw ref new InvalidArgumentException("Invalid dataset sigle");
		}
		CouchDBManager^ pMan = this->Manager;
		IMap<String^, Object^>^ oMap = ref new Map<String^, Object^>();
		oMap->Insert(InfoStrings::KEY_TYPE, InfoStrings::TYPE_VARIABLE);
		oMap->Insert(InfoStrings::KEY_DATASETSIGLE, pSet->Sigle);
		int nRet = create_task(pMan->GetDocumentsCountAsync(oMap)).get();
		return (nRet);
	});
}//GetDatasetVariablesCountAsync
IAsyncOperation<IVector<DomainVariable^>^>^ DomainDBManager::GetDatasetVariablesAsync(DomainDataset^ pSet, int offset, int count) {
	return create_async([this, pSet, offset, count]()->IVector<DomainVariable^>^ {
		if (pSet == nullptr) {
			throw ref new InvalidArgumentException("Null Dataset");
		}
		if (pSet->Sigle->IsEmpty()) {
			throw ref new InvalidArgumentException("Invalid dataset sigle");
		}
		CouchDBManager^ pMan = this->Manager;
		IMap<String^, Object^>^ oMap = ref new Map<String^, Object^>();
		oMap->Insert(InfoStrings::KEY_TYPE, InfoStrings::TYPE_VARIABLE);
		oMap->Insert(InfoStrings::KEY_DATASETSIGLE, pSet->Sigle);
		IVector<IMap<String^, Object^>^>^ pVec = create_task(pMan->GetDocumentsAsync(oMap, offset, count)).get();
		IVector<DomainVariable^>^ pRet = ref new Vector<DomainVariable^>();
		if (pVec != nullptr) {
			auto it = pVec->First();
			while (it->HasCurrent) {
				IMap<String^, Object^>^ m = it->Current;
				if (m != nullptr) {
					DomainVariable^ p = ref new DomainVariable(m);
					pRet->Append(p);
					it->MoveNext();
				}
			}// it
		}// pVec
		return (pRet);
	});
}//GetDatasetVariables
IAsyncOperation<DomainVariable^>^ DomainDBManager::FindVariable(DomainVariable^ model) {
	return create_async([this, model]()->DomainVariable^ {
		if (model == nullptr) {
			throw ref new InvalidArgumentException();
		}
		CouchDBManager^ pMan = this->Manager;
		DomainVariable^ oRet;
		IMap<String^, Object^>^ oFetch = model->GetMap();
		IMap<String^, Object^>^ oMap = create_task(pMan->FindDocumentAsync(oFetch)).get();
		if (oMap != nullptr) {
			if (oMap->HasKey(InfoStrings::KEY_ID) && oMap->HasKey(InfoStrings::KEY_REV)) {
				oRet = ref new DomainVariable(oMap);
			}
		}// oMap
		return (oRet);
	});
}//FindVariable
IAsyncOperation<DomainVariable^>^ DomainDBManager::FindVariableBySiglesAsync(String^ setsigle, String^ sigle) {
	return create_async([this, setsigle, sigle]()->DomainVariable^ {
		if ((setsigle == nullptr) || (sigle == nullptr)) {
			throw ref new InvalidArgumentException();
		}
		CouchDBManager^ pMan = this->Manager;
		DomainVariable^ oRet = nullptr;
		IMap<String^, Object^>^ oFetch = ref new Map<String^, Object^>();
		oFetch->Insert(InfoStrings::KEY_SIGLE, StringUtils::ToUpperFormat(sigle));
		oFetch->Insert(InfoStrings::KEY_DATASETSIGLE, StringUtils::ToUpperFormat(setsigle));
		oFetch->Insert(InfoStrings::KEY_TYPE, InfoStrings::TYPE_VARIABLE);
		IMap<String^, Object^>^ oMap = create_task(pMan->FindDocumentAsync(oFetch)).get();
		if (oMap != nullptr) {
			if (oMap->HasKey(InfoStrings::KEY_ID) && oMap->HasKey(InfoStrings::KEY_REV)) {
				oRet = ref new DomainVariable(oMap);
			}
		}// oMap
		return (oRet);
	});
}//FindVariableBySiglesAsync
IAsyncOperation<bool>^ DomainDBManager::MaintainsVariableAsync(DomainVariable^ model) {
	return create_async([this, model]()->bool {
		if (model == nullptr) {
			throw ref new InvalidArgumentException("Null Argument");
		}
		if (!model->IsStoreable) {
			throw ref new InvalidArgumentException("Not Storeable Variable");
		}
		CouchDBManager^ pMan = this->Manager;
		IMap<String^, Object^>^ oFetch = model->GetMap();
		bool bRet = create_task(pMan->MaintainsDocumentAsync(oFetch)).get();
		return (bRet);
	});
}//MaintainsVariableAsync
IAsyncOperation<bool>^ DomainDBManager::RemoveVariableAsync(DomainVariable^ model) {
	return create_async([this, model]()->bool {
		if (model == nullptr) {
			throw ref new InvalidArgumentException("Null Argument");
		}
		if (!model->IsPersisted) {
			throw ref new InvalidArgumentException("Not Persisted Variable");
		}
		String^ sigle = model->Sigle;
		if (sigle->IsEmpty()) {
			throw ref new InvalidArgumentException("Invalid Variable sigle");
		}
		CouchDBManager^ pMan = this->Manager;
		IMap<String^, Object^>^ oFetch = ref new Map<String^, Object^>();
		oFetch->Insert(InfoStrings::KEY_VARIABLESIGLE, model->Sigle);
		bool bRet = create_task(pMan->RemoveDocumentsAsync(oFetch)).get();
		if (bRet) {
			String^ docid = model->Id;
			bRet = create_task(pMan->DeleteDocumentByIdAsync(docid)).get();
		}
		return (bRet);
	});
}//RemoveVariableAsync
IAsyncOperation<bool>^  DomainDBManager::MaintainsVariablesAsync(IVector<DomainVariable^>^ oVec, bool bDelete) {
	return create_async([this, oVec, bDelete]()->bool {
		if (oVec == nullptr) {
			throw ref new InvalidArgumentException();
		}
		CouchDBManager^ pMan = this->Manager;
		bool bRet = true;
		if (!bDelete) {
			IVector<IMap<String^, Object^>^>^ vv = ref new Vector<IMap<String^, Object^>^>();
			auto it = oVec->First();
			while (it->HasCurrent) {
				DomainVariable^ p = it->Current;
				if (p != nullptr) {
					if (p->IsStoreable) {
						IMap<String^, Object^>^ m = p->GetMap();
						vv->Append(m);
					}
				}// p
				it->MoveNext();
			}// it
			if (vv->Size > 0) {
				bRet = create_task(pMan->MaintainsDocumentsAsync(vv, false)).get();
			}
		}
		else {
			std::vector<task<bool> > oTasks{};
			auto it = oVec->First();
			while (it->HasCurrent) {
				DomainVariable^ p = it->Current;
				if (p != nullptr) {
					if (p->IsPersisted) {
						oTasks.push_back(create_task(this->RemoveVariableAsync(p)));
					}
				}// p
				it->MoveNext();
			}// it
			if (!oTasks.empty()) {
				std::vector<bool> bb = when_all(oTasks.begin(), oTasks.end()).get();
				for (bool b : bb) {
					if (!b) {
						bRet = false;
						break;
					}
				}
			}// oTasks
		}
		return (bRet);
	});
}// MaintainsVariablesAsync
 //
IAsyncOperation<int>^ DomainDBManager::GetDatasetIndivsCountAsync(DomainDataset^ pSet) {
	return create_async([this, pSet]()->int {
		if (pSet == nullptr) {
			throw ref new InvalidArgumentException("Null Dataset");
		}
		if (pSet->Sigle->IsEmpty()) {
			throw ref new InvalidArgumentException("Invalid dataset sigle");
		}
		CouchDBManager^ pMan = this->Manager;
		IMap<String^, Object^>^ oMap = ref new Map<String^, Object^>();
		oMap->Insert(InfoStrings::KEY_TYPE, InfoStrings::TYPE_INDIV);
		oMap->Insert(InfoStrings::KEY_DATASETSIGLE, pSet->Sigle);
		int nRet = create_task(pMan->GetDocumentsCountAsync(oMap)).get();
		return (nRet);
	});
}//GetDatasetVariablesCountAsync
IAsyncOperation<IVector<DomainIndiv^>^>^ DomainDBManager::GetDatasetIndivsAsync(DomainDataset^ pSet, int offset, int count) {
	return create_async([this, pSet, offset, count]()->IVector<DomainIndiv^>^ {
		if (pSet == nullptr) {
			throw ref new InvalidArgumentException("Null Dataset");
		}
		if (pSet->Sigle->IsEmpty()) {
			throw ref new InvalidArgumentException("Invalid dataset sigle");
		}
		CouchDBManager^ pMan = this->Manager;
		IMap<String^, Object^>^ oMap = ref new Map<String^, Object^>();
		oMap->Insert(InfoStrings::KEY_TYPE, InfoStrings::TYPE_INDIV);
		oMap->Insert(InfoStrings::KEY_DATASETSIGLE, pSet->Sigle);
		IVector<IMap<String^, Object^>^>^ pVec = create_task(pMan->GetDocumentsAsync(oMap, offset, count)).get();
		IVector<DomainIndiv^>^ pRet = ref new Vector<DomainIndiv^>();
		if (pVec != nullptr) {
			auto it = pVec->First();
			while (it->HasCurrent) {
				IMap<String^, Object^>^ m = it->Current;
				if (m != nullptr) {
					DomainIndiv^ p = ref new DomainIndiv(m);
					pRet->Append(p);
					it->MoveNext();
				}
			}// it
		}// pVec
		return (pRet);
	});
}//GetDatasetIndivs
IAsyncOperation<DomainIndiv^>^ DomainDBManager::FindIndiv(DomainIndiv^ model) {
	return create_async([this, model]()->DomainIndiv^ {
		if (model == nullptr) {
			throw ref new InvalidArgumentException();
		}
		CouchDBManager^ pMan = this->Manager;
		DomainIndiv^ oRet;
		IMap<String^, Object^>^ oFetch = model->GetMap();
		IMap<String^, Object^>^ oMap = create_task(pMan->FindDocumentAsync(oFetch)).get();
		if (oMap != nullptr) {
			if (oMap->HasKey(InfoStrings::KEY_ID) && oMap->HasKey(InfoStrings::KEY_REV)) {
				oRet = ref new DomainIndiv(oMap);
			}
		}// oMap
		return (oRet);
	});
}//FindIndiv
IAsyncOperation<DomainIndiv^>^ DomainDBManager::FindIndivBySiglesAsync(String^ setsigle, String^ sigle) {
	return create_async([this, setsigle, sigle]()->DomainIndiv^ {
		if ((setsigle == nullptr) || (sigle == nullptr)) {
			throw ref new InvalidArgumentException();
		}
		CouchDBManager^ pMan = this->Manager;
		DomainIndiv^ oRet = nullptr;
		IMap<String^, Object^>^ oFetch = ref new Map<String^, Object^>();
		oFetch->Insert(InfoStrings::KEY_SIGLE, StringUtils::ToUpperFormat(sigle));
		oFetch->Insert(InfoStrings::KEY_DATASETSIGLE, StringUtils::ToUpperFormat(setsigle));
		oFetch->Insert(InfoStrings::KEY_TYPE, InfoStrings::TYPE_INDIV);
		IMap<String^, Object^>^ oMap = create_task(pMan->FindDocumentAsync(oFetch)).get();
		if (oMap != nullptr) {
			if (oMap->HasKey(InfoStrings::KEY_ID) && oMap->HasKey(InfoStrings::KEY_REV)) {
				oRet = ref new DomainIndiv(oMap);
			}
		}// oMap
		return (oRet);
	});
}//FindIndivBySiglesAsync
IAsyncOperation<bool>^ DomainDBManager::MaintainsIndivAsync(DomainIndiv^ model) {
	return create_async([this, model]()->bool {
		if (model == nullptr) {
			throw ref new InvalidArgumentException("Null Argument");
		}
		if (!model->IsStoreable) {
			throw ref new InvalidArgumentException("Not Storeable Indiv");
		}
		CouchDBManager^ pMan = this->Manager;
		IMap<String^, Object^>^ oFetch = model->GetMap();
		bool bRet = create_task(pMan->MaintainsDocumentAsync(oFetch)).get();
		return (bRet);
	});
}//MaintainsIndivAsync
IAsyncOperation<bool>^ DomainDBManager::RemoveIndivAsync(DomainIndiv^ model) {
	return create_async([this, model]()->bool {
		if (model == nullptr) {
			throw ref new InvalidArgumentException("Null Argument");
		}
		if (!model->IsPersisted) {
			throw ref new InvalidArgumentException("Not Persisted Indiv");
		}
		String^ sigle = model->Sigle;
		if (sigle->IsEmpty()) {
			throw ref new InvalidArgumentException("Invalid Indiv sigle");
		}
		CouchDBManager^ pMan = this->Manager;
		IMap<String^, Object^>^ oFetch = ref new Map<String^, Object^>();
		oFetch->Insert(InfoStrings::KEY_INDIVSIGLE, model->Sigle);
		bool bRet = create_task(pMan->RemoveDocumentsAsync(oFetch)).get();
		if (bRet) {
			String^ docid = model->Id;
			bRet = create_task(pMan->DeleteDocumentByIdAsync(docid)).get();
		}
		return (bRet);
	});
}//RemoveIndivAsync
IAsyncOperation<bool>^ DomainDBManager::MaintainsIndivsAsync(IVector<DomainIndiv^>^ oVec, bool bDelete) {
	return create_async([this, oVec, bDelete]()->bool {
		if (oVec == nullptr) {
			throw ref new InvalidArgumentException();
		}
		CouchDBManager^ pMan = this->Manager;
		bool bRet = true;
		if (!bDelete) {
			IVector<IMap<String^, Object^>^>^ vv = ref new Vector<IMap<String^, Object^>^>();
			auto it = oVec->First();
			while (it->HasCurrent) {
				DomainIndiv^ p = it->Current;
				if (p != nullptr) {
					if (p->IsStoreable) {
						IMap<String^, Object^>^ m = p->GetMap();
						vv->Append(m);
					}
				}// p
				it->MoveNext();
			}// it
			if (vv->Size > 0) {
				bRet = create_task(pMan->MaintainsDocumentsAsync(vv, false)).get();
			}
		}
		else {
			std::vector<task<bool> > oTasks{};
			auto it = oVec->First();
			while (it->HasCurrent) {
				DomainIndiv^ p = it->Current;
				if (p != nullptr) {
					if (p->IsPersisted) {
						oTasks.push_back(create_task(this->RemoveIndivAsync(p)));
					}
				}// p
				it->MoveNext();
			}// it
			if (!oTasks.empty()) {
				std::vector<bool> bb = when_all(oTasks.begin(), oTasks.end()).get();
				for (bool b : bb) {
					if (!b) {
						bRet = false;
						break;
					}
				}
			}// oTasks
		}
		return (bRet);
	});
}//MaintainsIndivsAsync
 //
IAsyncOperation<int>^ DomainDBManager::GetDatasetValuesCountAsync(DomainDataset^ model) {
	return create_async([this, model]()->int {
		if (model == nullptr) {
			throw ref new InvalidArgumentException("Null Dataset");
		}
		if (model->Sigle->IsEmpty()) {
			throw ref new InvalidArgumentException("Invalid dataset sigle");
		}
		CouchDBManager^ pMan = this->Manager;
		IMap<String^, Object^>^ oMap = ref new Map<String^, Object^>();
		oMap->Insert(InfoStrings::KEY_TYPE, InfoStrings::TYPE_VALUE);
		oMap->Insert(InfoStrings::KEY_DATASETSIGLE, model->Sigle);
		int nRet = create_task(pMan->GetDocumentsCountAsync(oMap)).get();
		return (nRet);
	});
}//GetDatasetValuesCountAsync
IAsyncOperation<IVector<DomainValue^>^>^ DomainDBManager::GetDatasetValuesAsync(DomainDataset ^model, int offset, int count) {
	return create_async([this, model, offset, count]()->IVector<DomainValue^>^ {
		if (model == nullptr) {
			throw ref new InvalidArgumentException("Null Dataset");
		}
		if (model->Sigle->IsEmpty()) {
			throw ref new InvalidArgumentException("Invalid dataset sigle");
		}
		CouchDBManager^ pMan = this->Manager;
		IMap<String^, Object^>^ oMap = ref new Map<String^, Object^>();
		oMap->Insert(InfoStrings::KEY_TYPE, InfoStrings::TYPE_VALUE);
		oMap->Insert(InfoStrings::KEY_DATASETSIGLE, model->Sigle);
		IVector<IMap<String^, Object^>^>^ pVec = create_task(pMan->GetDocumentsAsync(oMap, offset, count)).get();
		IVector<DomainValue^>^ pRet = ref new Vector<DomainValue^>();
		if (pVec != nullptr) {
			auto it = pVec->First();
			while (it->HasCurrent) {
				IMap<String^, Object^>^ m = it->Current;
				if (m != nullptr) {
					DomainValue^ p = ref new DomainValue(m);
					pRet->Append(p);
					it->MoveNext();
				}
			}// it
		}// pVec
		return (pRet);
	});
}//GetDatasetValuesAsync
IAsyncOperation<int>^ DomainDBManager::GetVariableValuesCountAsync(DomainVariable^ model) {
	return create_async([this, model]()->int {
		if (model == nullptr) {
			throw ref new InvalidArgumentException("Null Variable");
		}
		if (model->Sigle->IsEmpty()) {
			throw ref new InvalidArgumentException("Invalid variable sigle");
		}
		CouchDBManager^ pMan = this->Manager;
		IMap<String^, Object^>^ oMap = ref new Map<String^, Object^>();
		oMap->Insert(InfoStrings::KEY_TYPE, InfoStrings::TYPE_VALUE);
		oMap->Insert(InfoStrings::KEY_DATASETSIGLE, model->DatasetSigle);
		oMap->Insert(InfoStrings::KEY_VARIABLESIGLE, model->Sigle);
		int nRet = create_task(pMan->GetDocumentsCountAsync(oMap)).get();
		return (nRet);
	});
}//GetVariableValuesCountAsync
IAsyncOperation<IVector<DomainValue^>^>^ DomainDBManager::GetVariableValuesAsync(DomainVariable ^model, int offset, int count) {
	return create_async([this, model, offset, count]()->IVector<DomainValue^>^ {
		if (model == nullptr) {
			throw ref new InvalidArgumentException("Null Variable");
		}
		if (model->Sigle->IsEmpty()) {
			throw ref new InvalidArgumentException("Invalid variable sigle");
		}
		CouchDBManager^ pMan = this->Manager;
		IMap<String^, Object^>^ oMap = ref new Map<String^, Object^>();
		oMap->Insert(InfoStrings::KEY_TYPE, InfoStrings::TYPE_VALUE);
		oMap->Insert(InfoStrings::KEY_DATASETSIGLE, model->DatasetSigle);
		oMap->Insert(InfoStrings::KEY_VARIABLESIGLE, model->Sigle);
		IVector<IMap<String^, Object^>^>^ pVec = create_task(pMan->GetDocumentsAsync(oMap, offset, count)).get();
		IVector<DomainValue^>^ pRet = ref new Vector<DomainValue^>();
		if (pVec != nullptr) {
			auto it = pVec->First();
			while (it->HasCurrent) {
				IMap<String^, Object^>^ m = it->Current;
				if (m != nullptr) {
					DomainValue^ p = ref new DomainValue(m);
					pRet->Append(p);
					it->MoveNext();
				}
			}// it
		}// pVec
		return (pRet);
	});
}// GetVariableValuesAsync(
IAsyncOperation<int>^ DomainDBManager::GetIndivValuesCountAsync(DomainIndiv^ model) {
	return create_async([this, model]()->int {
		if (model == nullptr) {
			throw ref new InvalidArgumentException("Null Indiv");
		}
		if (model->Sigle->IsEmpty()) {
			throw ref new InvalidArgumentException("Invalid indiv sigle");
		}
		CouchDBManager^ pMan = this->Manager;
		IMap<String^, Object^>^ oMap = ref new Map<String^, Object^>();
		oMap->Insert(InfoStrings::KEY_TYPE, InfoStrings::TYPE_VALUE);
		oMap->Insert(InfoStrings::KEY_DATASETSIGLE, model->DatasetSigle);
		oMap->Insert(InfoStrings::KEY_INDIVSIGLE, model->Sigle);
		int nRet = create_task(pMan->GetDocumentsCountAsync(oMap)).get();
		return (nRet);
	});
}//GetIndivValuesCountAsyn
IAsyncOperation<IVector<DomainValue^>^>^ DomainDBManager::GetIndivValuesAsync(DomainIndiv ^model, int offset, int count) {
	return create_async([this, model, offset, count]()->IVector<DomainValue^>^ {
		if (model == nullptr) {
			throw ref new InvalidArgumentException("Null Indiv");
		}
		if (model->Sigle->IsEmpty()) {
			throw ref new InvalidArgumentException("Invalid indiv sigle");
		}
		CouchDBManager^ pMan = this->Manager;
		IMap<String^, Object^>^ oMap = ref new Map<String^, Object^>();
		oMap->Insert(InfoStrings::KEY_TYPE, InfoStrings::TYPE_VALUE);
		oMap->Insert(InfoStrings::KEY_DATASETSIGLE, model->DatasetSigle);
		oMap->Insert(InfoStrings::KEY_INDIVSIGLE, model->Sigle);
		IVector<IMap<String^, Object^>^>^ pVec = create_task(pMan->GetDocumentsAsync(oMap, offset, count)).get();
		IVector<DomainValue^>^ pRet = ref new Vector<DomainValue^>();
		if (pVec != nullptr) {
			auto it = pVec->First();
			while (it->HasCurrent) {
				IMap<String^, Object^>^ m = it->Current;
				if (m != nullptr) {
					DomainValue^ p = ref new DomainValue(m);
					pRet->Append(p);
					it->MoveNext();
				}
			}// it
		}// pVec
		return (pRet);
	});
}//GetIndivValuesAsync
IAsyncOperation<DomainValue^>^ DomainDBManager::FindValue(DomainValue^ model) {
	return create_async([this, model]()->DomainValue^ {
		if (model == nullptr) {
			throw ref new InvalidArgumentException();
		}
		CouchDBManager^ pMan = this->Manager;
		DomainValue^ oRet;
		IMap<String^, Object^>^ oFetch = model->GetMap();
		IMap<String^, Object^>^ oMap = create_task(pMan->FindDocumentAsync(oFetch)).get();
		if (oMap != nullptr) {
			if (oMap->HasKey(InfoStrings::KEY_ID) && oMap->HasKey(InfoStrings::KEY_REV)) {
				oRet = ref new DomainValue(oMap);
			}
		}// oMap
		return (oRet);
	});
}//FindVal
IAsyncOperation<DomainValue^>^ DomainDBManager::FindValueBySiglesAsync(String^ setsigle, String^ indsigle, String^ varsigle) {
	return create_async([this, setsigle, indsigle, varsigle]()->DomainValue^ {
		if ((setsigle == nullptr) || (indsigle == nullptr) || (varsigle == nullptr)) {
			throw ref new InvalidArgumentException();
		}
		CouchDBManager^ pMan = this->Manager;
		DomainValue^ oRet = nullptr;
		IMap<String^, Object^>^ oFetch = ref new Map<String^, Object^>();
		oFetch->Insert(InfoStrings::KEY_INDIVSIGLE, StringUtils::ToUpperFormat(indsigle));
		oFetch->Insert(InfoStrings::KEY_VARIABLESIGLE, StringUtils::ToUpperFormat(varsigle));
		oFetch->Insert(InfoStrings::KEY_DATASETSIGLE, StringUtils::ToUpperFormat(setsigle));
		oFetch->Insert(InfoStrings::KEY_TYPE, InfoStrings::TYPE_VALUE);
		IMap<String^, Object^>^ oMap = create_task(pMan->FindDocumentAsync(oFetch)).get();
		if (oMap != nullptr) {
			if (oMap->HasKey(InfoStrings::KEY_ID) && oMap->HasKey(InfoStrings::KEY_REV)) {
				oRet = ref new DomainValue(oMap);
			}
		}// oMap
		return (oRet);
	});
}//FindValueBySiglesAsync
IAsyncOperation<bool>^ DomainDBManager::MaintainsValueAsync(DomainValue^ model) {
	return create_async([this, model]()->bool {
		if (model == nullptr) {
			throw ref new InvalidArgumentException("Null Argument");
		}
		if (!model->IsStoreable) {
			throw ref new InvalidArgumentException("Not Storeable InfoValue");
		}
		CouchDBManager^ pMan = this->Manager;
		IMap<String^, Object^>^ oFetch = model->GetMap();
		bool bRet = create_task(pMan->MaintainsDocumentAsync(oFetch)).get();
		return (bRet);
	});
}//MaintainsValueAsync
IAsyncOperation<bool>^ DomainDBManager::RemoveValueAsync(DomainValue^ model) {
	return create_async([this, model]()->bool {
		if (model == nullptr) {
			throw ref new InvalidArgumentException("Null Argument");
		}
		if (!model->IsPersisted) {
			throw ref new InvalidArgumentException("Not Persisted InfoValue");
		}
		CouchDBManager^ pMan = this->Manager;
		String^ docid = model->Id;
		bool bRet = create_task(pMan->DeleteDocumentByIdAsync(docid)).get();
		return (bRet);
	});
}//RemoveValueA
IAsyncOperation<bool>^ DomainDBManager::MaintainsValuesAsync(IVector<DomainValue^>^ oVec, bool bDelete) {
	return create_async([this, oVec, bDelete]()->bool {
		if (oVec == nullptr) {
			throw ref new InvalidArgumentException();
		}
		CouchDBManager^ pMan = this->Manager;
		bool bRet = true;
		IVector<IMap<String^, Object^>^>^ vv = ref new Vector<IMap<String^, Object^>^>();
		if (!bDelete) {
			auto it = oVec->First();
			while (it->HasCurrent) {
				DomainValue^ p = it->Current;
				if (p != nullptr) {
					if (p->IsStoreable) {
						IMap<String^, Object^>^ m = p->GetMap();
						vv->Append(m);
					}
				}// p
				it->MoveNext();
			}// it
			if (vv->Size > 0) {
				bRet = create_task(pMan->MaintainsDocumentsAsync(vv, false)).get();
			}
		}
		else {
			auto it = oVec->First();
			while (it->HasCurrent) {
				DomainValue^ p = it->Current;
				if (p != nullptr) {
					if (p->IsPersisted) {
						IMap<String^, Object^>^ m = ref new Map<String^, Object^>();
						m->Insert(InfoStrings::KEY_ID, p->Id);
						m->Insert(InfoStrings::KEY_REV, p->Rev);
						vv->Append(m);
					}
				}// p
				it->MoveNext();
			}// it
			if (vv->Size > 0) {
				bRet = create_task(pMan->MaintainsDocumentsAsync(vv, true)).get();
			}
		}
		return (bRet);
	});
}//MaintainsValuesAsyn
 //
IAsyncOperation<bool>^ DomainDBManager::MaintainsDocumentAttachmentAsync(String^ docid,
	String^ attachmentName, String^ mimetype, IBuffer^ data) {
	return create_async([this, docid, attachmentName, mimetype, data]()->bool {
		CouchDBManager^ pMan = this->Manager;
		bool bRet = create_task(pMan->MaintainsDocumentAttachmentAsync(docid, attachmentName, mimetype, data)).get();
		return bRet;
	});

}//MaintainsDocumentAttachmentAsync
IAsyncOperation<bool>^ DomainDBManager::MaintainsDocumentAttachmentAsync(String^ docid, String^ attachmentName, IStorageFile^ file) {
	return create_async([this, docid, attachmentName, file]()->bool {
		CouchDBManager^ pMan = this->Manager;
		bool bRet = create_task(pMan->MaintainsDocumentAttachmentAsync(docid, attachmentName, file)).get();
		return bRet;
	});
}//MaintainsDocumentAttachmentAsynck
IAsyncOperation<IMap<String^, String^> ^>^ DomainDBManager::GetDocumentAttachmentNamesAsync(String^ docid) {
	return create_async([this, docid]()->IMap<String^, String^> ^ {
		CouchDBManager^ pMan = this->Manager;
		IMap<String^, String^> ^  bRet = create_task(pMan->GetDocumentAttachmentNamesAsync(docid)).get();
		return bRet;
	});
	return m_pman->GetDocumentAttachmentNamesAsync(docid);
}//GetDocumentAttachmentNamesAsync
IAsyncOperation<bool>^ DomainDBManager::RemoveDocumentAttachmentAsync(String^ docid, String^ attachmentName) {
	return create_async([this, docid, attachmentName]()->bool {
		CouchDBManager^ pMan = this->Manager;
		bool bRet = create_task(pMan->RemoveDocumentAttachmentAsync(docid, attachmentName)).get();
		return bRet;
	});
	return m_pman->RemoveDocumentAttachmentAsync(docid, attachmentName);
}//RemoveDocumentAttachmentAsync
IAsyncOperation<IBuffer^>^ DomainDBManager::GetDocumentAttachmentDataAsync(String^ docid, String^ attachmentName) {
	return create_async([this, docid, attachmentName]()->IBuffer^ {
		CouchDBManager^ pMan = this->Manager;
		IBuffer^ pRet = create_task(pMan->GetDocumentAttachmentDataAsync(docid, attachmentName)).get();
		return pRet;
	});
	return m_pman->GetDocumentAttachmentDataAsync(docid, attachmentName);
}//GetDocumentAttachmentDataAsync
IAsyncOperation<IVector<byte>^>^ DomainDBManager::GetDocumentAttachmentDataVectorAsync(String^ docid, String^ attachmentName) {
	return create_async([this, docid, attachmentName]()->IVector<byte>^ {
		CouchDBManager^ pMan = this->Manager;
		IVector<byte>^ pRet = create_task(pMan->GetDocumentAttachmentDataVectorAsync(docid, attachmentName)).get();
		return pRet;
	});
}//GetDocumentAttachmentDataVectorAsync
///////////////////////////////////////