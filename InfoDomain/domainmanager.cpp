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
CouchDBManager^ DomainManager::Manager::get() {
	return this->m_pman;
}
IAsyncOperation<int>^ DomainManager::GetDatasetsCountAsync(void) {
	CouchDBManager^ pMan = this->m_pman;
	return create_async([pMan]()->int {
		IMap<String^, Object^>^ oMap = ref new Map<String^, Object^>();
		oMap->Insert(InfoStrings::KEY_TYPE, InfoStrings::TYPE_DATASET);
		int nRet = create_task(pMan->GetDocumentsCountAsync(oMap)).get();
		return (nRet);
	});
}//GetDatasetsCountAsync
IAsyncOperation<IVector<Dataset^>^>^ DomainManager::GetDatasetsAsync(int offset, int count) {
	CouchDBManager^ pMan = this->m_pman;
	return create_async([pMan, offset, count]()->IVector<Dataset^>^ {
		IMap<String^, Object^>^ oMap = ref new Map<String^, Object^>();
		oMap->Insert(InfoStrings::KEY_TYPE, InfoStrings::TYPE_DATASET);
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
}//FindDatasetAsync
IAsyncOperation<Dataset^>^ DomainManager::FindDatasetBySigleAsync(String^ sigle) {
	if (sigle == nullptr) {
		throw ref new InvalidArgumentException();
	}
	CouchDBManager^ pMan = this->m_pman;
	return create_async([pMan, sigle]()->Dataset^ {
		Dataset^ oRet = nullptr;
		IMap<String^, Object^>^ oFetch = ref new Map<String^, Object^>();
		oFetch->Insert(InfoStrings::KEY_SIGLE, sigle);
		oFetch->Insert(InfoStrings::KEY_TYPE, InfoStrings::TYPE_DATASET);
		IMap<String^, Object^>^ oMap = create_task(pMan->FindDocumentAsync(oFetch)).get();
		if (oMap != nullptr) {
			if (oMap->HasKey(InfoStrings::KEY_ID) && oMap->HasKey(InfoStrings::KEY_REV)) {
				oRet = ref new Dataset(oMap);
			}
		}// oMap
		return (oRet);
	});
}//FindDatasetBySigleAsync
IAsyncOperation<bool>^ DomainManager::MaintainsDatasetAsync(Dataset^ model) {
	if (model == nullptr) {
		throw ref new InvalidArgumentException("Null Argument");
	}
	if (!model->IsStoreable) {
		throw ref new InvalidArgumentException("Not Storeable Dataset");
	}
	CouchDBManager^ pMan = this->m_pman;
	return create_async([pMan, model]()->bool {
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
IAsyncOperation<bool>^  DomainManager::MaintainsDatasetsAsync(IVector<Dataset^>^ oVec, bool bDelete) {
	if (oVec == nullptr) {
		throw ref new InvalidArgumentException();
	}
	CouchDBManager^ pMan = this->m_pman;
	return create_async([this, pMan, oVec, bDelete]()->bool {
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
}// MaintainsDatasetsAsync
//
IAsyncOperation<int>^ DomainManager::GetDatasetVariablesCountAsync(Dataset^ pSet) {
	if (pSet == nullptr) {
		throw ref new InvalidArgumentException("Null Dataset");
	}
	if (pSet->Sigle->IsEmpty()) {
		throw ref new InvalidArgumentException("Invalid dataset sigle");
	}
	CouchDBManager^ pMan = this->m_pman;
	return create_async([pMan, pSet]()->int {
		IMap<String^, Object^>^ oMap = ref new Map<String^, Object^>();
		oMap->Insert(InfoStrings::KEY_TYPE, InfoStrings::TYPE_VARIABLE);
		oMap->Insert(InfoStrings::KEY_DATASETSIGLE, pSet->Sigle);
		int nRet = create_task(pMan->GetDocumentsCountAsync(oMap)).get();
		return (nRet);
	});
}//GetDatasetVariablesCountAsyn
IAsyncOperation<IVector<Variable^>^>^ DomainManager::GetDatasetVariables(Dataset^ pSet, int offset, int count) {
	if (pSet == nullptr) {
		throw ref new InvalidArgumentException("Null Dataset");
	}
	if (pSet->Sigle->IsEmpty()) {
		throw ref new InvalidArgumentException("Invalid dataset sigle");
	}
	CouchDBManager^ pMan = this->m_pman;
	return create_async([pMan, pSet, offset, count]()->IVector<Variable^>^ {
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
	if (model == nullptr) {
		throw ref new InvalidArgumentException();
	}
	CouchDBManager^ pMan = this->m_pman;
	return create_async([pMan, model]()->Variable^ {
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
	if ((setsigle == nullptr) || (sigle == nullptr)) {
		throw ref new InvalidArgumentException();
	}
	CouchDBManager^ pMan = this->m_pman;
	return create_async([pMan, setsigle,sigle]()->Variable^ {
		Variable^ oRet = nullptr;
		IMap<String^, Object^>^ oFetch = ref new Map<String^, Object^>();
		oFetch->Insert(InfoStrings::KEY_SIGLE,sigle);
		oFetch->Insert(InfoStrings::KEY_DATASETSIGLE, setsigle);
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
IAsyncOperation<bool>^ DomainManager::MaintainsVariableAsync(Variable^ model) {
	if (model == nullptr) {
		throw ref new InvalidArgumentException("Null Argument");
	}
	if (!model->IsStoreable) {
		throw ref new InvalidArgumentException("Not Storeable Variable");
	}
	CouchDBManager^ pMan = this->m_pman;
	return create_async([pMan, model]()->bool {
		IMap<String^, Object^>^ oFetch = model->GetMap();
		bool bRet = create_task(pMan->MaintainsDocumentAsync(oFetch)).get();
		return (bRet);
	});
}//MaintainsVariableAsync
IAsyncOperation<bool>^ DomainManager::RemoveVariableAsync(Variable^ model) {
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
	CouchDBManager^ pMan = this->m_pman;
	return create_async([pMan, model]()->bool {
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
	if (oVec == nullptr) {
		throw ref new InvalidArgumentException();
	}
	CouchDBManager^ pMan = this->m_pman;
	return create_async([this, pMan, oVec, bDelete]()->bool {
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
//
IAsyncOperation<int>^ DomainManager::GetDatasetIndivsCountAsync(Dataset^ pSet) {
	if (pSet == nullptr) {
		throw ref new InvalidArgumentException("Null Dataset");
	}
	if (pSet->Sigle->IsEmpty()) {
		throw ref new InvalidArgumentException("Invalid dataset sigle");
	}
	CouchDBManager^ pMan = this->m_pman;
	return create_async([pMan, pSet]()->int {
		IMap<String^, Object^>^ oMap = ref new Map<String^, Object^>();
		oMap->Insert(InfoStrings::KEY_TYPE, InfoStrings::TYPE_INDIV);
		oMap->Insert(InfoStrings::KEY_DATASETSIGLE, pSet->Sigle);
		int nRet = create_task(pMan->GetDocumentsCountAsync(oMap)).get();
		return (nRet);
	});
}//GetDatasetVariablesCountAsync
IAsyncOperation<IVector<Indiv^>^>^ DomainManager::GetDatasetIndivs(Dataset^ pSet, int offset, int count) {
	if (pSet == nullptr) {
		throw ref new InvalidArgumentException("Null Dataset");
	}
	if (pSet->Sigle->IsEmpty()) {
		throw ref new InvalidArgumentException("Invalid dataset sigle");
	}
	CouchDBManager^ pMan = this->m_pman;
	return create_async([pMan, pSet, offset, count]()->IVector<Indiv^>^ {
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
	if (model == nullptr) {
		throw ref new InvalidArgumentException();
	}
	CouchDBManager^ pMan = this->m_pman;
	return create_async([pMan, model]()->Indiv^ {
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
	if ((setsigle == nullptr) || (sigle == nullptr)) {
		throw ref new InvalidArgumentException();
	}
	CouchDBManager^ pMan = this->m_pman;
	return create_async([pMan, setsigle, sigle]()->Indiv^ {
		Indiv^ oRet = nullptr;
		IMap<String^, Object^>^ oFetch = ref new Map<String^, Object^>();
		oFetch->Insert(InfoStrings::KEY_SIGLE, sigle);
		oFetch->Insert(InfoStrings::KEY_DATASETSIGLE, setsigle);
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
IAsyncOperation<bool>^ DomainManager::MaintainsIndivAsync(Indiv^ model) {
	if (model == nullptr) {
		throw ref new InvalidArgumentException("Null Argument");
	}
	if (!model->IsStoreable) {
		throw ref new InvalidArgumentException("Not Storeable Indiv");
	}
	CouchDBManager^ pMan = this->m_pman;
	return create_async([pMan, model]()->bool {
		IMap<String^, Object^>^ oFetch = model->GetMap();
		bool bRet = create_task(pMan->MaintainsDocumentAsync(oFetch)).get();
		return (bRet);
	});
}//MaintainsIndivAsync
IAsyncOperation<bool>^ DomainManager::RemoveIndivAsync(Indiv^ model) {
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
	CouchDBManager^ pMan = this->m_pman;
	return create_async([pMan, model]()->bool {
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
	if (oVec == nullptr) {
		throw ref new InvalidArgumentException();
	}
	CouchDBManager^ pMan = this->m_pman;
	return create_async([this, pMan, oVec, bDelete]()->bool {
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
	if (model == nullptr) {
		throw ref new InvalidArgumentException("Null Dataset");
	}
	if (model->Sigle->IsEmpty()) {
		throw ref new InvalidArgumentException("Invalid dataset sigle");
	}
	CouchDBManager^ pMan = this->m_pman;
	return create_async([pMan, model]()->int {
		IMap<String^, Object^>^ oMap = ref new Map<String^, Object^>();
		oMap->Insert(InfoStrings::KEY_TYPE, InfoStrings::TYPE_VALUE);
		oMap->Insert(InfoStrings::KEY_DATASETSIGLE, model->Sigle);
		int nRet = create_task(pMan->GetDocumentsCountAsync(oMap)).get();
		return (nRet);
	});
}//GetDatasetValuesCountAsync
IAsyncOperation<IVector<InfoValue^>^>^ DomainManager::GetDatasetValuesAsync(Dataset ^model, int offset, int count) {
	if (model == nullptr) {
		throw ref new InvalidArgumentException("Null Dataset");
	}
	if (model->Sigle->IsEmpty()) {
		throw ref new InvalidArgumentException("Invalid dataset sigle");
	}
	CouchDBManager^ pMan = this->m_pman;
	return create_async([pMan, model, offset, count]()->IVector<InfoValue^>^ {
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
	if (model == nullptr) {
		throw ref new InvalidArgumentException("Null Variable");
	}
	if (model->Sigle->IsEmpty()) {
		throw ref new InvalidArgumentException("Invalid variable sigle");
	}
	CouchDBManager^ pMan = this->m_pman;
	return create_async([pMan, model]()->int {
		IMap<String^, Object^>^ oMap = ref new Map<String^, Object^>();
		oMap->Insert(InfoStrings::KEY_TYPE, InfoStrings::TYPE_VALUE);
		oMap->Insert(InfoStrings::KEY_DATASETSIGLE, model->DatasetSigle);
		oMap->Insert(InfoStrings::KEY_VARIABLESIGLE, model->Sigle);
		int nRet = create_task(pMan->GetDocumentsCountAsync(oMap)).get();
		return (nRet);
	});
}//GetVariableValuesCountAsync
IAsyncOperation<IVector<InfoValue^>^>^ DomainManager::GetVariableValuesAsync(Variable ^model, int offset, int count) {
	if (model == nullptr) {
		throw ref new InvalidArgumentException("Null Variable");
	}
	if (model->Sigle->IsEmpty()) {
		throw ref new InvalidArgumentException("Invalid variable sigle");
	}
	CouchDBManager^ pMan = this->m_pman;
	return create_async([pMan, model, offset, count]()->IVector<InfoValue^>^ {
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
	if (model == nullptr) {
		throw ref new InvalidArgumentException("Null Indiv");
	}
	if (model->Sigle->IsEmpty()) {
		throw ref new InvalidArgumentException("Invalid indiv sigle");
	}
	CouchDBManager^ pMan = this->m_pman;
	return create_async([pMan, model]()->int {
		IMap<String^, Object^>^ oMap = ref new Map<String^, Object^>();
		oMap->Insert(InfoStrings::KEY_TYPE, InfoStrings::TYPE_VALUE);
		oMap->Insert(InfoStrings::KEY_DATASETSIGLE, model->DatasetSigle);
		oMap->Insert(InfoStrings::KEY_INDIVSIGLE, model->Sigle);
		int nRet = create_task(pMan->GetDocumentsCountAsync(oMap)).get();
		return (nRet);
	});
}//GetIndivValuesCountAsyn
IAsyncOperation<IVector<InfoValue^>^>^ DomainManager::GetIndivValuesAsync(Indiv ^model, int offset, int count) {
	if (model == nullptr) {
		throw ref new InvalidArgumentException("Null Indiv");
	}
	if (model->Sigle->IsEmpty()) {
		throw ref new InvalidArgumentException("Invalid indiv sigle");
	}
	CouchDBManager^ pMan = this->m_pman;
	return create_async([pMan, model, offset, count]()->IVector<InfoValue^>^ {
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
	if (model == nullptr) {
		throw ref new InvalidArgumentException();
	}
	CouchDBManager^ pMan = this->m_pman;
	return create_async([pMan, model]()->InfoValue^ {
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
IAsyncOperation<InfoValue^>^ DomainManager::FindValueBySiglesAsync(String^ setsigle, String^ indsigle,String^ varsigle) {
	if ((setsigle == nullptr) || (indsigle == nullptr) || (varsigle == nullptr)) {
		throw ref new InvalidArgumentException();
	}
	CouchDBManager^ pMan = this->m_pman;
	return create_async([pMan, setsigle, indsigle,varsigle]()->InfoValue^ {
		InfoValue^ oRet = nullptr;
		IMap<String^, Object^>^ oFetch = ref new Map<String^, Object^>();
		oFetch->Insert(InfoStrings::KEY_INDIVSIGLE, indsigle);
		oFetch->Insert(InfoStrings::KEY_VARIABLESIGLE, varsigle);
		oFetch->Insert(InfoStrings::KEY_DATASETSIGLE, setsigle);
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
IAsyncOperation<bool>^ DomainManager::MaintainsValueAsync(InfoValue^ model) {
	if (model == nullptr) {
		throw ref new InvalidArgumentException("Null Argument");
	}
	if (!model->IsStoreable) {
		throw ref new InvalidArgumentException("Not Storeable InfoValue");
	}
	CouchDBManager^ pMan = this->m_pman;
	return create_async([pMan, model]()->bool {
		IMap<String^, Object^>^ oFetch = model->GetMap();
		bool bRet = create_task(pMan->MaintainsDocumentAsync(oFetch)).get();
		return (bRet);
	});
}//MaintainsValueAsync
IAsyncOperation<bool>^ DomainManager::RemoveValueAsync(InfoValue^ model) {
	if (model == nullptr) {
		throw ref new InvalidArgumentException("Null Argument");
	}
	if (!model->IsPersisted) {
		throw ref new InvalidArgumentException("Not Persisted InfoValue");
	}
	CouchDBManager^ pMan = this->m_pman;
	return create_async([pMan, model]()->bool {
		String^ docid = model->Id;
		bool bRet = create_task(pMan->DeleteDocumentByIdAsync(docid)).get();
		return (bRet);
	});
}//RemoveValueA
IAsyncOperation<bool>^ DomainManager::MaintainsValuesAsync(IVector<InfoValue^>^ oVec, bool bDelete) {
	if (oVec == nullptr) {
		throw ref new InvalidArgumentException();
	}
	CouchDBManager^ pMan = this->m_pman;
	return create_async([this, pMan, oVec, bDelete]()->bool {
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
IAsyncOperation<bool>^ DomainManager::MaintainsDocumentAttachmentAsync(String^ docid, String^ attachmentName, String^ mimetype, IBuffer^ data) {
	return m_pman->MaintainsDocumentAttachmentAsync(docid, attachmentName, mimetype, data);
}//MaintainsDocumentAttachmentAsync
IAsyncOperation<bool>^ DomainManager::MaintainsDocumentAttachmentAsync(String^ docid, String^ attachmentName, IStorageFile^ file) {
	return m_pman->MaintainsDocumentAttachmentAsync(docid, attachmentName, file);
}//MaintainsDocumentAttachmentAsync
IAsyncOperation<IMap<String^, String^> ^>^ DomainManager::GetDocumentAttachmentNamesAsync(String^ docid) {
	return m_pman->GetDocumentAttachmentNamesAsync(docid);
}//GetDocumentAttachmentNamesAsync
IAsyncOperation<bool>^ DomainManager::RemoveDocumentAttachmentAsync(String^ docid, String^ attachmentName) {
	return m_pman->RemoveDocumentAttachmentAsync(docid, attachmentName);
}//RemoveDocumentAttachmentAsync
IAsyncOperation<IBuffer^>^ DomainManager::GetDocumentAttachmentDataAsync(String^ docid, String^ attachmentName) {
	return m_pman->GetDocumentAttachmentDataAsync(docid, attachmentName);
}//GetDocumentAttachmentDataAsync
IAsyncOperation<IVector<byte>^>^ DomainManager::GetDocumentAttachmentDataVectorAsync(String^ docid, String^ attachmentName) {
	return m_pman->GetDocumentAttachmentDataVectorAsync(docid, attachmentName);
}//GetDocumentAttachmentDataVectorAsync
