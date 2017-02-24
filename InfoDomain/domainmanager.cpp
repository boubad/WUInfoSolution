#include  "pch.h"
////////////////////
#include "domainmanager.h"
#include "infostrings.h"
#include "StringUtils.h"
////////////////////
using namespace Platform::Collections;
using namespace concurrency;
using namespace InfoCouchDB;
using namespace InfoDomain;
////////////////////////////
#include <mutex>
#include <atomic>
////////////////////////////
static std::atomic<bool> st_initalized{ false };
static std::mutex st_mutex{};
///////////////////////////
DomainManager::DomainManager() {
}
DomainManager::DomainManager(String^ baseUrl, String^ databaseName) {
	this->BaseUrl = baseUrl;
	this->DatabaseName = databaseName;
}//DomainManager
String^ DomainManager::BaseUrl::get() {
	return m_base;
}
void DomainManager::BaseUrl::set(String^ s) {
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
}// baseUrl
String^ DomainManager::DatabaseName::get() {
	return m_data;
}
void DomainManager::DatabaseName::set(String^ s) {
	String^ ss = StringUtils::ToLower(StringUtils::Trim(s));
	if (ss->IsEmpty()) {
		throw ref new InvalidArgumentException("Null database name");
	}
	m_data = ss;
	m_pman = nullptr;
}
CouchDBManager^ DomainManager::Manager::get() {
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
void DomainManager::internal_check_index(String^ field, String^ name) {
	CouchDBManager^ pMan = this->Manager;
	IVector<String^>^ ff = ref new Vector<String^>();
	ff->Append(field);
	String^ designDoc;
	(void)create_task(pMan->CreateIndexAsync(ff, name, designDoc)).get();
}//internal_check_index
void DomainManager::check_indexes(void) {
	if (st_initalized) {
		return;
	}
	{
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
	}
}// check_indexes
///////////////////////////////
IAsyncOperation<Etudiant^>^ DomainManager::FindEtudiantByDossierAsync(String^ dossier) {
	return create_async([this, dossier]()->Etudiant^ {
		if ((dossier == nullptr) || dossier->IsEmpty()) {
			throw ref new InvalidArgumentException();
		}
		CouchDBManager^ pMan = this->Manager;
		Etudiant^ oArg = ref new Etudiant();
		oArg->Dossier = dossier;
		Etudiant^ oRet;
		IMap<String^, Object^>^ oFetch = oArg->GetMap();
		IMap<String^, Object^>^ oMap = create_task(pMan->FindDocumentAsync(oFetch)).get();
		if ((oMap != nullptr) && (oMap->Size > 0)) {
			if (oMap->HasKey(InfoStrings::KEY_ID) && oMap->HasKey(InfoStrings::KEY_REV)) {
				oRet = ref new Etudiant{ oMap };
			}
		}// oMap
		return (oRet);
	});
}//FindEtudiantByDossierAsync
IAsyncOperation<int>^ DomainManager::GetEtudiantsCountAsync(Etudiant^ pModel) {
	return create_async([this, pModel]()->int {
		CouchDBManager^ pMan = this->Manager;
		Etudiant^ p = (pModel != nullptr) ? pModel : ref new Etudiant();
		IMap<String^, Object^>^ oMap = p->GetMap();
		int nRet = create_task(pMan->GetDocumentsCountAsync(oMap)).get();
		return (nRet);
	});
}//GetEtudiantsCountAsync
IAsyncOperation<IVector<Etudiant^>^>^ DomainManager::GetEtudiantsAsync(Etudiant^ pModel, int offset, int count) {
	return create_async([this, pModel, offset, count]()->IVector<Etudiant^>^ {
		CouchDBManager^ pMan = this->Manager;
		Etudiant^ px = (pModel != nullptr) ? pModel : ref new Etudiant();
		IMap<String^, Object^>^ oMap = px->GetMap();
		IVector<IMap<String^, Object^>^>^ pVec = create_task(pMan->GetDocumentsAsync(oMap, offset, count)).get();
		IVector<Etudiant^>^ pRet = ref new Vector<Etudiant^>();
		if (pVec != nullptr) {
			auto it = pVec->First();
			while (it->HasCurrent) {
				IMap<String^, Object^>^ m = it->Current;
				if (m != nullptr) {
					Etudiant^ p = ref new Etudiant{ m };
					pRet->Append(p);
					it->MoveNext();
				}
			}// it
		}// pVec
		return (pRet);
	});
}//GetEtudiantsAsync
IAsyncOperation<bool>^ DomainManager::MaintainsEtudiantAsync(Etudiant^ model) {
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
IAsyncOperation<bool>^ DomainManager::RemoveEtudiantAsync(Etudiant^ model) {
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
IAsyncOperation<bool>^ DomainManager::MaintainsEtudiantsAsync(IVector<Etudiant^>^ oVec, bool bDelete) {
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
				Etudiant^ p = it->Current;
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
				Etudiant^ p = it->Current;
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
////////////////////////////////
IAsyncOperation<int>^ DomainManager::GetDatasetsCountAsync(Dataset^ model) {
	return create_async([this, model]()->int {
		CouchDBManager^ pMan = this->Manager;
		Dataset^ px = (model != nullptr) ? model : ref new Dataset{};
		IMap<String^, Object^>^ oMap = px->GetMap();
		int nRet = create_task(pMan->GetDocumentsCountAsync(oMap)).get();
		return (nRet);
	});
}//GetDatasetsCountAsync
IAsyncOperation<IVector<Dataset^>^>^ DomainManager::GetDatasetsAsync(Dataset^ model, int offset, int count) {
	return create_async([this, model, offset, count]()->IVector<Dataset^>^ {
		CouchDBManager^ pMan = this->Manager;
		Dataset^ px = (model != nullptr) ? model : ref new Dataset{};
		IMap<String^, Object^>^ oMap = px->GetMap();
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
	return create_async([this, model]()->Dataset^ {
		if (model == nullptr) {
			throw ref new InvalidArgumentException();
		}
		CouchDBManager^ pMan = this->Manager;
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
}//FindDatasetAsync
IAsyncOperation<Dataset^>^ DomainManager::FindDatasetBySigleAsync(String^ sigle) {
	return create_async([this, sigle]()->Dataset^ {
		if (sigle == nullptr) {
			throw ref new InvalidArgumentException();
		}
		CouchDBManager^ pMan = this->Manager;
		Dataset^ oArg = ref new Dataset{};
		oArg->Sigle = sigle;
		Dataset^ oRet = nullptr;
		IMap<String^, Object^>^ oFetch = oArg->GetMap();
		IMap<String^, Object^>^ oMap = create_task(pMan->FindDocumentAsync(oFetch)).get();
		if (oMap != nullptr) {
			if (oMap->HasKey(InfoStrings::KEY_ID) && oMap->HasKey(InfoStrings::KEY_REV)) {
				oRet = ref new Dataset{ oMap };
			}
		}// oMap
		return (oRet);
	});
}//FindDatasetBySigleAsync
IAsyncOperation<bool>^ DomainManager::MaintainsDatasetAsync(Dataset^ model) {
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
IAsyncOperation<bool>^ DomainManager::RemoveDatasetAsync(Dataset^ model) {
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
IAsyncOperation<bool>^  DomainManager::MaintainsDatasetsAsync(IVector<Dataset^>^ oVec, bool bDelete) {
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
				Dataset^ p = it->Current;
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
				Dataset^ p = it->Current;
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
IAsyncOperation<Dataset^>^ DomainManager::LoadDatasetAsync(String ^ sigle)
{
	return create_async([this, sigle]()->Dataset^ {
		Dataset^ pRet = create_task(this->FindDatasetBySigleAsync(sigle)).get();
		if (pRet == nullptr) {
			return pRet;
		}
		int nv = create_task(this->GetDatasetVariablesCountAsync(pRet)).get();
		if (nv > 0) {
			IVector<Variable^>^ vv = create_task(this->GetDatasetVariablesAsync(pRet, 0, nv)).get();
			if (vv != nullptr) {
				auto it = vv->First();
				while (it->HasCurrent) {
					Variable^ p = it->Current;
					p->Set = pRet;
					it->MoveNext();
				}// it
				pRet->Variables = vv;
			}// vv
		}// nv
		int nr = create_task(this->GetDatasetIndivsCountAsync(pRet)).get();
		if (nr > 0) {
			IVector<Indiv^>^ vv = create_task(this->GetDatasetIndivsAsync(pRet, 0, nr)).get();
			if (vv != nullptr) {
				auto it = vv->First();
				while (it->HasCurrent) {
					Indiv^ p = it->Current;
					p->Set = pRet;
					it->MoveNext();
				}// it
				pRet->Indivs = vv;
			}// vv
		}// nv
		int nx = create_task(this->GetDatasetValuesCountAsync(pRet)).get();
		if (nx > 0) {
			IVector<InfoValue^>^ vv = create_task(this->GetDatasetValuesAsync(pRet, 0, nx)).get();
			if (vv != nullptr) {
				auto it = vv->First();
				while (it->HasCurrent) {
					InfoValue^ p = it->Current;
					Variable^ pVar = pRet->FindVariable(p->VariableSigle);
					if (pVar != nullptr) {
						p->Var = pVar;
						pVar->Values->Append(p);
					}// pVar
					Indiv^ pInd = pRet->FindIndiv(p->IndivSigle);
					if (pInd != nullptr) {
						p->Ind = pInd;
						pInd->Values->Append(p);
					}
					it->MoveNext();
				}// it
			}// vv
		}// nx
		return pRet;
	});
}
IAsyncOperation<IVector<String^>^>^ DomainManager::GetAllDatasetsSigles(void)
{
	return create_async([this]()->IVector<String^>^ {
		int n = create_task(this->GetDatasetsCountAsync(nullptr)).get();
		IVector<String^>^ pRet = ref new Vector<String^>();
		if (n > 0) {
			IVector<Dataset^>^ vv = create_task(this->GetDatasetsAsync(nullptr, 0, n)).get();
			if (vv != nullptr) {
				auto it = vv->First();
				while (it->HasCurrent) {
					Dataset^ p = it->Current;
					pRet->Append(p->Sigle);
					it->MoveNext();
				}// it
			}// vv
		}// n
		return pRet;
	});
}
// MaintainsDatasetsAsync
//
IAsyncOperation<int>^ DomainManager::GetDatasetVariablesCountAsync(Dataset^ pSet) {
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
IAsyncOperation<IVector<Variable^>^>^ DomainManager::GetDatasetVariablesAsync(Dataset^ pSet, int offset, int count) {
	return create_async([this, pSet, offset, count]()->IVector<Variable^>^ {
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
		IVector<Variable^>^ pRet = ref new Vector<Variable^>();
		if (pVec != nullptr) {
			auto it = pVec->First();
			while (it->HasCurrent) {
				IMap<String^, Object^>^ m = it->Current;
				if (m != nullptr) {
					Variable^ p = ref new Variable(m);
					pRet->Append(p);
					it->MoveNext();
				}
			}// it
		}// pVec
		return (pRet);
	});
}//GetDatasetVariables
IAsyncOperation<Variable^>^ DomainManager::FindVariable(Variable^ model) {
	return create_async([this, model]()->Variable^ {
		if (model == nullptr) {
			throw ref new InvalidArgumentException();
		}
		CouchDBManager^ pMan = this->Manager;
		Variable^ oRet;
		IMap<String^, Object^>^ oFetch = model->GetMap();
		IMap<String^, Object^>^ oMap = create_task(pMan->FindDocumentAsync(oFetch)).get();
		if (oMap != nullptr) {
			if (oMap->HasKey(InfoStrings::KEY_ID) && oMap->HasKey(InfoStrings::KEY_REV)) {
				oRet = ref new Variable(oMap);
			}
		}// oMap
		return (oRet);
	});
}//FindVariable
IAsyncOperation<Variable^>^ DomainManager::FindVariableBySiglesAsync(String^ setsigle, String^ sigle) {
	return create_async([this, setsigle, sigle]()->Variable^ {
		if ((setsigle == nullptr) || (sigle == nullptr)) {
			throw ref new InvalidArgumentException();
		}
		CouchDBManager^ pMan = this->Manager;
		Variable^ oRet = nullptr;
		IMap<String^, Object^>^ oFetch = ref new Map<String^, Object^>();
		oFetch->Insert(InfoStrings::KEY_SIGLE, StringUtils::ToUpperFormat(sigle));
		oFetch->Insert(InfoStrings::KEY_DATASETSIGLE, StringUtils::ToUpperFormat(setsigle));
		oFetch->Insert(InfoStrings::KEY_TYPE, InfoStrings::TYPE_VARIABLE);
		IMap<String^, Object^>^ oMap = create_task(pMan->FindDocumentAsync(oFetch)).get();
		if (oMap != nullptr) {
			if (oMap->HasKey(InfoStrings::KEY_ID) && oMap->HasKey(InfoStrings::KEY_REV)) {
				oRet = ref new Variable(oMap);
			}
		}// oMap
		return (oRet);
	});
}//FindVariableBySiglesAsync
IAsyncOperation<Variable^>^ DomainManager::LoadVariableBySiglesAsync(String^ setsigle, String^ sigle) {
	return create_async([this, setsigle, sigle]()->Variable^ {
		if ((setsigle == nullptr) || (sigle == nullptr)) {
			throw ref new InvalidArgumentException();
		}
		CouchDBManager^ pMan = this->Manager;
		Variable^ oRet = nullptr;
		IMap<String^, Object^>^ oFetch = ref new Map<String^, Object^>();
		oFetch->Insert(InfoStrings::KEY_SIGLE, StringUtils::ToUpperFormat(sigle));
		oFetch->Insert(InfoStrings::KEY_DATASETSIGLE, StringUtils::ToUpperFormat(setsigle));
		oFetch->Insert(InfoStrings::KEY_TYPE, InfoStrings::TYPE_VARIABLE);
		IMap<String^, Object^>^ oMap = create_task(pMan->FindDocumentAsync(oFetch)).get();
		if (oMap != nullptr) {
			if (oMap->HasKey(InfoStrings::KEY_ID) && oMap->HasKey(InfoStrings::KEY_REV)) {
				oRet = ref new Variable(oMap);
				Dataset^ pSet = create_task(this->FindDatasetBySigleAsync(oRet->DatasetSigle)).get();
				pSet->Variables->Append(oRet);
				oRet->Set = pSet;
				int nv = create_task(this->GetVariableValuesCountAsync(oRet)).get();
				if (nv > 0) {
					oRet->Values = create_task(this->GetVariableValuesAsync(oRet, 0, nv)).get();
					auto it = oRet->Values->First();
					while (it->HasCurrent) {
						InfoValue^ pVal = it->Current;
						pVal->Var = oRet;
						Indiv^ pInd = create_task(this->FindIndivBySiglesAsync(pSet->Sigle, pVal->IndivSigle)).get();
						pInd->Values->Append(pVal);
						pInd->Set = pSet;
						pVal->Ind = pInd;
						pSet->Indivs->Append(pInd);
						it->MoveNext();
					}// it
				}// nv
			}// var
		}// oMap
		return (oRet);
	});
}//LoadVariableBySiglesAsync
IAsyncOperation<bool>^ DomainManager::MaintainsVariableAsync(Variable^ model) {
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
IAsyncOperation<bool>^ DomainManager::RemoveVariableAsync(Variable^ model) {
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
IAsyncOperation<bool>^  DomainManager::MaintainsVariablesAsync(IVector<Variable^>^ oVec, bool bDelete) {
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
				Variable^ p = it->Current;
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
				Variable^ p = it->Current;
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
IAsyncOperation<int>^ DomainManager::GetDatasetIndivsCountAsync(Dataset^ pSet) {
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
IAsyncOperation<IVector<Indiv^>^>^ DomainManager::GetDatasetIndivsAsync(Dataset^ pSet, int offset, int count) {
	return create_async([this, pSet, offset, count]()->IVector<Indiv^>^ {
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
		IVector<Indiv^>^ pRet = ref new Vector<Indiv^>();
		if (pVec != nullptr) {
			auto it = pVec->First();
			while (it->HasCurrent) {
				IMap<String^, Object^>^ m = it->Current;
				if (m != nullptr) {
					Indiv^ p = ref new Indiv(m);
					pRet->Append(p);
					it->MoveNext();
				}
			}// it
		}// pVec
		return (pRet);
	});
}//GetDatasetIndivs
IAsyncOperation<Indiv^>^ DomainManager::FindIndiv(Indiv^ model) {
	return create_async([this, model]()->Indiv^ {
		if (model == nullptr) {
			throw ref new InvalidArgumentException();
		}
		CouchDBManager^ pMan = this->Manager;
		Indiv^ oRet;
		IMap<String^, Object^>^ oFetch = model->GetMap();
		IMap<String^, Object^>^ oMap = create_task(pMan->FindDocumentAsync(oFetch)).get();
		if (oMap != nullptr) {
			if (oMap->HasKey(InfoStrings::KEY_ID) && oMap->HasKey(InfoStrings::KEY_REV)) {
				oRet = ref new Indiv(oMap);
			}
		}// oMap
		return (oRet);
	});
}//FindIndiv
IAsyncOperation<Indiv^>^ DomainManager::FindIndivBySiglesAsync(String^ setsigle, String^ sigle) {
	return create_async([this, setsigle, sigle]()->Indiv^ {
		if ((setsigle == nullptr) || (sigle == nullptr)) {
			throw ref new InvalidArgumentException();
		}
		CouchDBManager^ pMan = this->Manager;
		Indiv^ oRet = nullptr;
		IMap<String^, Object^>^ oFetch = ref new Map<String^, Object^>();
		oFetch->Insert(InfoStrings::KEY_SIGLE, StringUtils::ToUpperFormat(sigle));
		oFetch->Insert(InfoStrings::KEY_DATASETSIGLE, StringUtils::ToUpperFormat(setsigle));
		oFetch->Insert(InfoStrings::KEY_TYPE, InfoStrings::TYPE_INDIV);
		IMap<String^, Object^>^ oMap = create_task(pMan->FindDocumentAsync(oFetch)).get();
		if (oMap != nullptr) {
			if (oMap->HasKey(InfoStrings::KEY_ID) && oMap->HasKey(InfoStrings::KEY_REV)) {
				oRet = ref new Indiv(oMap);
			}
		}// oMap
		return (oRet);
	});
}//FindIndivBySiglesAsync
IAsyncOperation<Indiv^>^ DomainManager::LoadIndivBySiglesAsync(String^ setsigle, String^ sigle) {
	return create_async([this, setsigle, sigle]()->Indiv^ {
		if ((setsigle == nullptr) || (sigle == nullptr)) {
			throw ref new InvalidArgumentException();
		}
		CouchDBManager^ pMan = this->Manager;
		Indiv^ oRet = nullptr;
		IMap<String^, Object^>^ oFetch = ref new Map<String^, Object^>();
		oFetch->Insert(InfoStrings::KEY_SIGLE, StringUtils::ToUpperFormat(sigle));
		oFetch->Insert(InfoStrings::KEY_DATASETSIGLE, StringUtils::ToUpperFormat(setsigle));
		oFetch->Insert(InfoStrings::KEY_TYPE, InfoStrings::TYPE_INDIV);
		IMap<String^, Object^>^ oMap = create_task(pMan->FindDocumentAsync(oFetch)).get();
		if (oMap != nullptr) {
			if (oMap->HasKey(InfoStrings::KEY_ID) && oMap->HasKey(InfoStrings::KEY_REV)) {
				oRet = ref new Indiv(oMap);
				Dataset^ pSet = create_task(this->FindDatasetBySigleAsync(oRet->DatasetSigle)).get();
				pSet->Indivs->Append(oRet);
				oRet->Set = pSet;
				int nv = create_task(this->GetIndivValuesCountAsync(oRet)).get();
				if (nv > 0) {
					oRet->Values = create_task(this->GetIndivValuesAsync(oRet, 0, nv)).get();
					auto it = oRet->Values->First();
					while (it->HasCurrent) {
						InfoValue^ pVal = it->Current;
						pVal->Ind = oRet;
						Variable^ pVar = create_task(this->FindVariableBySiglesAsync(pSet->Sigle, pVal->VariableSigle)).get();
						pVar->Values->Append(pVal);
						pVar->Set = pSet;
						pVal->Var = pVar;
						pSet->Variables->Append(pVar);
						it->MoveNext();
					}// it
				}// nv
			}// var
		}// oMap
		return (oRet);
	});
}//LoadIndivBySiglesAsync
IAsyncOperation<bool>^ DomainManager::MaintainsIndivAsync(Indiv^ model) {
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
IAsyncOperation<bool>^ DomainManager::RemoveIndivAsync(Indiv^ model) {
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
IAsyncOperation<bool>^ DomainManager::MaintainsIndivsAsync(IVector<Indiv^>^ oVec, bool bDelete) {
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
				Indiv^ p = it->Current;
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
				Indiv^ p = it->Current;
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
IAsyncOperation<int>^ DomainManager::GetDatasetValuesCountAsync(Dataset^ model) {
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
IAsyncOperation<IVector<InfoValue^>^>^ DomainManager::GetDatasetValuesAsync(Dataset ^model, int offset, int count) {
	return create_async([this, model, offset, count]()->IVector<InfoValue^>^ {
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
		IVector<InfoValue^>^ pRet = ref new Vector<InfoValue^>();
		if (pVec != nullptr) {
			auto it = pVec->First();
			while (it->HasCurrent) {
				IMap<String^, Object^>^ m = it->Current;
				if (m != nullptr) {
					InfoValue^ p = ref new InfoValue(m);
					pRet->Append(p);
					it->MoveNext();
				}
			}// it
		}// pVec
		return (pRet);
	});
}//GetDatasetValuesAsync
IAsyncOperation<int>^ DomainManager::GetVariableValuesCountAsync(Variable^ model) {
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
IAsyncOperation<IVector<InfoValue^>^>^ DomainManager::GetVariableValuesAsync(Variable ^model, int offset, int count) {
	return create_async([this, model, offset, count]()->IVector<InfoValue^>^ {
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
		IVector<InfoValue^>^ pRet = ref new Vector<InfoValue^>();
		if (pVec != nullptr) {
			auto it = pVec->First();
			while (it->HasCurrent) {
				IMap<String^, Object^>^ m = it->Current;
				if (m != nullptr) {
					InfoValue^ p = ref new InfoValue(m);
					pRet->Append(p);
					it->MoveNext();
				}
			}// it
		}// pVec
		return (pRet);
	});
}// GetVariableValuesAsync(
IAsyncOperation<int>^ DomainManager::GetIndivValuesCountAsync(Indiv^ model) {
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
IAsyncOperation<IVector<InfoValue^>^>^ DomainManager::GetIndivValuesAsync(Indiv ^model, int offset, int count) {
	return create_async([this, model, offset, count]()->IVector<InfoValue^>^ {
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
		IVector<InfoValue^>^ pRet = ref new Vector<InfoValue^>();
		if (pVec != nullptr) {
			auto it = pVec->First();
			while (it->HasCurrent) {
				IMap<String^, Object^>^ m = it->Current;
				if (m != nullptr) {
					InfoValue^ p = ref new InfoValue(m);
					pRet->Append(p);
					it->MoveNext();
				}
			}// it
		}// pVec
		return (pRet);
	});
}//GetIndivValuesAsync
IAsyncOperation<InfoValue^>^ DomainManager::FindValue(InfoValue^ model) {
	return create_async([this, model]()->InfoValue^ {
		if (model == nullptr) {
			throw ref new InvalidArgumentException();
		}
		CouchDBManager^ pMan = this->Manager;
		InfoValue^ oRet;
		IMap<String^, Object^>^ oFetch = model->GetMap();
		IMap<String^, Object^>^ oMap = create_task(pMan->FindDocumentAsync(oFetch)).get();
		if (oMap != nullptr) {
			if (oMap->HasKey(InfoStrings::KEY_ID) && oMap->HasKey(InfoStrings::KEY_REV)) {
				oRet = ref new InfoValue(oMap);
			}
		}// oMap
		return (oRet);
	});
}//FindVal
IAsyncOperation<InfoValue^>^ DomainManager::FindValueBySiglesAsync(String^ setsigle, String^ indsigle, String^ varsigle) {
	return create_async([this, setsigle, indsigle, varsigle]()->InfoValue^ {
		if ((setsigle == nullptr) || (indsigle == nullptr) || (varsigle == nullptr)) {
			throw ref new InvalidArgumentException();
		}
		CouchDBManager^ pMan = this->Manager;
		InfoValue^ oRet = nullptr;
		IMap<String^, Object^>^ oFetch = ref new Map<String^, Object^>();
		oFetch->Insert(InfoStrings::KEY_INDIVSIGLE, StringUtils::ToUpperFormat(indsigle));
		oFetch->Insert(InfoStrings::KEY_VARIABLESIGLE, StringUtils::ToUpperFormat(varsigle));
		oFetch->Insert(InfoStrings::KEY_DATASETSIGLE, StringUtils::ToUpperFormat(setsigle));
		oFetch->Insert(InfoStrings::KEY_TYPE, InfoStrings::TYPE_VALUE);
		IMap<String^, Object^>^ oMap = create_task(pMan->FindDocumentAsync(oFetch)).get();
		if (oMap != nullptr) {
			if (oMap->HasKey(InfoStrings::KEY_ID) && oMap->HasKey(InfoStrings::KEY_REV)) {
				oRet = ref new InfoValue(oMap);
			}
		}// oMap
		return (oRet);
	});
}//FindValueBySiglesAsync
IAsyncOperation<InfoValue^>^ DomainManager::LoadValueBySiglesAsync(String^ setsigle, String^ indsigle, String^ varsigle) {
	return create_async([this, setsigle, indsigle, varsigle]()->InfoValue^ {
		if ((setsigle == nullptr) || (indsigle == nullptr) || (varsigle == nullptr)) {
			throw ref new InvalidArgumentException();
		}
		CouchDBManager^ pMan = this->Manager;
		InfoValue^ oRet = nullptr;
		IMap<String^, Object^>^ oFetch = ref new Map<String^, Object^>();
		oFetch->Insert(InfoStrings::KEY_INDIVSIGLE, StringUtils::ToUpperFormat(indsigle));
		oFetch->Insert(InfoStrings::KEY_VARIABLESIGLE, StringUtils::ToUpperFormat(varsigle));
		oFetch->Insert(InfoStrings::KEY_DATASETSIGLE, StringUtils::ToUpperFormat(setsigle));
		oFetch->Insert(InfoStrings::KEY_TYPE, InfoStrings::TYPE_VALUE);
		IMap<String^, Object^>^ oMap = create_task(pMan->FindDocumentAsync(oFetch)).get();
		if (oMap != nullptr) {
			if (oMap->HasKey(InfoStrings::KEY_ID) && oMap->HasKey(InfoStrings::KEY_REV)) {
				oRet = ref new InfoValue(oMap);
				Variable^ pVar = create_task(this->FindVariableBySiglesAsync(oRet->DatasetSigle, oRet->VariableSigle)).get();
				oRet->Var = pVar;
				pVar->Values->Append(oRet);
				Indiv^ pInd = create_task(this->FindIndivBySiglesAsync(oRet->DatasetSigle, oRet->IndivSigle)).get();
				oRet->Ind = pInd;
				pInd->Values->Append(oRet);
				Dataset^ pSet = create_task(this->FindDatasetBySigleAsync(oRet->DatasetSigle)).get();
				pVar->Set = pSet;
				pInd->Set = pSet;
				pSet->Variables->Append(pVar);
				pSet->Indivs->Append(pInd);
			}// id
		}// oMap
		return (oRet);
	});
}//FindValueBySiglesAsync
IAsyncOperation<bool>^ DomainManager::MaintainsValueAsync(InfoValue^ model) {
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
IAsyncOperation<bool>^ DomainManager::RemoveValueAsync(InfoValue^ model) {
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
IAsyncOperation<bool>^ DomainManager::MaintainsValuesAsync(IVector<InfoValue^>^ oVec, bool bDelete) {
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
				InfoValue^ p = it->Current;
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
				InfoValue^ p = it->Current;
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
IAsyncOperation<bool>^ DomainManager::MaintainsDocumentAttachmentAsync(String^ docid,
	String^ attachmentName, String^ mimetype, IBuffer^ data) {
	return create_async([this, docid, attachmentName, mimetype, data]()->bool {
		CouchDBManager^ pMan = this->Manager;
		bool bRet = create_task(pMan->MaintainsDocumentAttachmentAsync(docid, attachmentName, mimetype, data)).get();
		return bRet;
	});

}//MaintainsDocumentAttachmentAsync
IAsyncOperation<bool>^ DomainManager::MaintainsDocumentAttachmentAsync(String^ docid, String^ attachmentName, IStorageFile^ file) {
	return create_async([this, docid, attachmentName, file]()->bool {
		CouchDBManager^ pMan = this->Manager;
		bool bRet = create_task(pMan->MaintainsDocumentAttachmentAsync(docid, attachmentName, file)).get();
		return bRet;
	});
}//MaintainsDocumentAttachmentAsynck
IAsyncOperation<IMap<String^, String^> ^>^ DomainManager::GetDocumentAttachmentNamesAsync(String^ docid) {
	return create_async([this, docid]()->IMap<String^, String^> ^ {
		CouchDBManager^ pMan = this->Manager;
		IMap<String^, String^> ^  bRet = create_task(pMan->GetDocumentAttachmentNamesAsync(docid)).get();
		return bRet;
	});
	return m_pman->GetDocumentAttachmentNamesAsync(docid);
}//GetDocumentAttachmentNamesAsync
IAsyncOperation<bool>^ DomainManager::RemoveDocumentAttachmentAsync(String^ docid, String^ attachmentName) {
	return create_async([this, docid, attachmentName]()->bool {
		CouchDBManager^ pMan = this->Manager;
		bool bRet = create_task(pMan->RemoveDocumentAttachmentAsync(docid, attachmentName)).get();
		return bRet;
	});
	return m_pman->RemoveDocumentAttachmentAsync(docid, attachmentName);
}//RemoveDocumentAttachmentAsync
IAsyncOperation<IBuffer^>^ DomainManager::GetDocumentAttachmentDataAsync(String^ docid, String^ attachmentName) {
	return create_async([this, docid, attachmentName]()->IBuffer^ {
		CouchDBManager^ pMan = this->Manager;
		IBuffer^ pRet = create_task(pMan->GetDocumentAttachmentDataAsync(docid, attachmentName)).get();
		return pRet;
	});
	return m_pman->GetDocumentAttachmentDataAsync(docid, attachmentName);
}//GetDocumentAttachmentDataAsync
IAsyncOperation<IVector<byte>^>^ DomainManager::GetDocumentAttachmentDataVectorAsync(String^ docid, String^ attachmentName) {
	return create_async([this, docid, attachmentName]()->IVector<byte>^ {
		CouchDBManager^ pMan = this->Manager;
		IVector<byte>^ pRet = create_task(pMan->GetDocumentAttachmentDataVectorAsync(docid, attachmentName)).get();
		return pRet;
	});
}//GetDocumentAttachmentDataVectorAsync
