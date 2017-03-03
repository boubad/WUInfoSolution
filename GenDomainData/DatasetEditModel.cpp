#include "pch.h"
#include "DatasetEditModel.h"
#include <algorithm>
using namespace GenDomainData;
using namespace Platform;
using namespace concurrency;

DatasetEditModel::DatasetEditModel()
{
}
void DatasetEditModel::OnPropertyChanged(String^ propertyName)
{
	PropertyChanged(this, ref new PropertyChangedEventArgs(propertyName));
}
DomainDBManager^ DatasetEditModel::Manager::get() {
	if (m_pman == nullptr) {
		m_pman = ref new DomainDBManager();
	}
	return m_pman;
}
String^ DatasetEditModel::BaseUrl::get() {
	return Manager->BaseUrl;
}
void DatasetEditModel::BaseUrl::set(String^ value) {
	Manager->BaseUrl = value;
	OnPropertyChanged("BaseUrl");
}
String^ DatasetEditModel::DatabaseName::get() {
	return Manager->DatabaseName;
}
void DatasetEditModel::DatabaseName::set(String^ value) {
	Manager->DatabaseName = value;
	OnPropertyChanged("DatabaseName");
}
void DatasetEditModel::RefreshDatasets(void) {
	if (!Manager->IsValid) {
		return;
	}
	DomainDataset^ pPrev = m_dataset;
	IObservableVector<DomainDataset^>^ pRet = nullptr;
	try {
		DomainDataset^ model = ref new DomainDataset();
		int n = create_task(Manager->GetDatasetsCountAsync(model)).get();
		if (n > 0) {
			auto oList =
				create_task(Manager->GetDatasetsAsync(model, 0, n)).get();
			if ((oList != nullptr) && (oList->Size > 0)) {
				std::sort(begin(oList), end(oList), [](DomainDataset^ p1, DomainDataset^ p2)->bool {
					return (p1->Sigle < p2->Sigle);
				});
				pRet = ref new Vector<DomainDataset^>{};
				auto it = oList->First();
				while (it->HasCurrent) {
					pRet->Append(it->Current);
					it->MoveNext();
				}
			}// oList
		}
	} catch(...){}
	DomainDataset^ pCur = nullptr;
	if ((pRet != nullptr) && (pRet->Size > 0)) {
		if ((pPrev != nullptr) && pPrev->IsPersisted) {
			String^ id = pPrev->Id;
			auto it = std::find_if(begin(pRet), end(pRet), [id](DomainDataset^ p)->bool {
				return (p->Id == id);
			});
			if (it != end(pRet)) {
				pCur = *it;
			}
		}// pPrev
		if (pCur == nullptr) {
			auto jt = pRet->First();
			if (jt->HasCurrent) {
				pCur = jt->Current;
			}
		}// pCur
	}// pRet
	m_datasets = pRet;
	OnPropertyChanged("Datasets");
	CurrentDataset = pCur;
}// RefreshDatasets
IObservableVector<DomainDataset^>^ DatasetEditModel::Datasets::get() {
	if (m_datasets == nullptr) {
		m_datasets = ref new Vector<DomainDataset^>();
	}
	return m_datasets;
}
IObservableVector<DomainVariable^>^ DatasetEditModel::Variables::get() {
	if (m_variables == nullptr) {
		m_variables = ref new Vector<DomainVariable^>();
	}
	return m_variables;
}
IObservableVector<DomainIndiv^>^ DatasetEditModel::Indivs::get() {
	if (m_indivs == nullptr) {
		m_indivs = ref new Vector<DomainIndiv^>();
	}
	return m_indivs;
}
IObservableVector<DomainValue^>^ DatasetEditModel::VariableValues::get() {
	if (m_varvalues == nullptr) {
		m_varvalues = ref new Vector<DomainValue^>();
	}
	return m_varvalues;
}
IObservableVector<DomainValue^>^ DatasetEditModel::IndivValues::get() {
	if (m_indvalues == nullptr) {
		m_indvalues = ref new Vector<DomainValue^>();
	}
	return m_indvalues;
}
DomainDataset^ DatasetEditModel::CurrentDataset::get()
{
	if (m_dataset == nullptr) {
		m_dataset = ref new DomainDataset();
		m_dataset->Status = InfoStatus::Normal;
	}
	return m_dataset;
}
void DatasetEditModel::CurrentDataset::set(DomainDataset^ value)
{
	if (value != m_dataset) {
		m_dataset = value;
		OnPropertyChanged("CurrentDataset");
		RefreshVariables();
		RefreshIndivs();
	}
}
void DatasetEditModel::RefreshVariables(void) {
	if (!Manager->IsValid) {
		return;
	}
	DomainVariable^ pPrev = m_variable;
	IObservableVector<DomainVariable^>^ pRet = nullptr;
	DomainDataset^ pSet = CurrentDataset;
	if ((pSet != nullptr) && pSet->IsPersisted) {
		try {
			int n = create_task(Manager->GetDatasetVariablesCountAsync(pSet)).get();
			if (n > 0) {
				auto oList =
					create_task(Manager->GetDatasetVariablesAsync(pSet, 0, n)).get();
				if ((oList != nullptr) && (oList->Size > 0)) {
					std::sort(begin(oList), end(oList), [](DomainVariable^ p1, DomainVariable^ p2)->bool {
						return (p1->Sigle < p2->Sigle);
					});
					pRet = ref new Vector<DomainVariable^>{};
					auto it = oList->First();
					while (it->HasCurrent) {
						pRet->Append(it->Current);
						it->MoveNext();
					}
				}// oList
			}
		}
		catch (...) {}
	}// pSet
	DomainVariable^ pCur = nullptr;
	if ((pRet != nullptr) && (pRet->Size > 0)) {
		if ((pPrev != nullptr) && pPrev->IsPersisted) {
			String^ id = pPrev->Id;
			auto it = std::find_if(begin(pRet), end(pRet), [id](DomainVariable^ p)->bool {
				return (p->Id == id);
			});
			if (it != end(pRet)) {
				pCur = *it;
			}
		}// pPrev
		if (pCur == nullptr) {
			auto jt = pRet->First();
			if (jt->HasCurrent) {
				pCur = jt->Current;
			}
		}// pCur
	}// pRet
	m_variables = pRet;
	OnPropertyChanged("Variables");
	CurrentVariable = pCur;
}// RefreshVariables
void DatasetEditModel::RefreshIndivs(void) {
	if (!Manager->IsValid) {
		return;
	}
	DomainIndiv^ pPrev = m_indiv;
	IObservableVector<DomainIndiv^>^ pRet = nullptr;
	DomainDataset^ pSet = CurrentDataset;
	if ((pSet != nullptr) && pSet->IsPersisted) {
		try {
			int n = create_task(Manager->GetDatasetIndivsCountAsync(pSet)).get();
			if (n > 0) {
				auto oList =
					create_task(Manager->GetDatasetIndivsAsync(pSet, 0, n)).get();
				if ((oList != nullptr) && (oList->Size > 0)) {
					std::sort(begin(oList), end(oList), [](DomainIndiv^ p1, DomainIndiv^ p2)->bool {
						return (p1->Sigle < p2->Sigle);
					});
					pRet = ref new Vector<DomainIndiv^>{};
					auto it = oList->First();
					while (it->HasCurrent) {
						pRet->Append(it->Current);
						it->MoveNext();
					}
				}// oList
			}
		}
		catch (...) {}
	}// pSet
	DomainIndiv^ pCur = nullptr;
	if ((pRet != nullptr) && (pRet->Size > 0)) {
		if ((pPrev != nullptr) && pPrev->IsPersisted) {
			String^ id = pPrev->Id;
			auto it = std::find_if(begin(pRet), end(pRet), [id](DomainIndiv^ p)->bool {
				return (p->Id == id);
			});
			if (it != end(pRet)) {
				pCur = *it;
			}
		}// pPrev
		if (pCur == nullptr) {
			auto jt = pRet->First();
			if (jt->HasCurrent) {
				pCur = jt->Current;
			}
		}// pCur
	}// pRet
	m_indivs = pRet;
	OnPropertyChanged("Indivs");
	CurrentIndiv = pCur;
}// RefreshIndivs
DomainVariable^ DatasetEditModel::CurrentVariable::get()
{
	if (m_variable == nullptr) {
		m_variable = ref new DomainVariable();
		m_variable->DatasetSigle = CurrentDataset->Sigle;
		m_variable->Status = InfoStatus::Normal;
		m_variable->VariableKind = InfoKind::Normal;
		m_variable->VariableType = InfoDataType::Real;
	}
	return m_variable;
}
void DatasetEditModel::CurrentVariable::set(DomainVariable^ value)
{
	if (value != m_variable) {
		m_variable = value;
		OnPropertyChanged("CurrentVariable");
		RefreshVariableValues();
	}
}
DomainIndiv^ DatasetEditModel::CurrentIndiv::get()
{
	if (m_indiv == nullptr) {
		m_indiv = ref new DomainIndiv();
		m_indiv->DatasetSigle = CurrentDataset->Sigle;
		m_indiv->Status = InfoStatus::Normal;
	}
	return m_indiv;
}
void DatasetEditModel::CurrentIndiv::set(DomainIndiv^ value)
{
	if (value != m_indiv) {
		m_indiv = value;
		OnPropertyChanged("CurrentIndiv");
		RefreshIndivValues();
	}
}
void DatasetEditModel::RefreshVariableValues(void) {
	if (!Manager->IsValid) {
		return;
	}
	IVector<DomainValue^>^ oList = nullptr;
	DomainVariable^ pVar = CurrentVariable;
	if ((pVar != nullptr) && pVar->IsPersisted) {
		try {
			int n = create_task(Manager->GetVariableValuesCountAsync(pVar)).get();
			if (n > 0) {
				oList = create_task(Manager->GetVariableValuesAsync(pVar, 0, n)).get();
			}
		}
		catch (...) {}
	}// pSet
	if (oList == nullptr) {
		oList = ref new Vector<DomainValue^>();
	}
	IObservableVector<DomainValue^>^ pRet = ref new Vector<DomainValue^>();
	auto inds = Indivs;
	auto it = inds->First();
	while (it->HasCurrent) {
		DomainIndiv^ pInd = it->Current;
		String^ sigle = pInd->Sigle;
		auto jt = std::find_if(begin(oList), end(oList), [sigle](DomainValue^ p)->bool {
			return (p->IndivSigle == sigle);
		});
		DomainValue^ pVal = nullptr;
		if (jt != end(oList)) {
			pVal = *jt;
		}
		else {
			pVal = ref new DomainValue();
			pVal->DatasetSigle = pVar->DatasetSigle;
			pVal->VariableSigle = pVar->Sigle;
			pVal->IndivSigle = sigle;
			pVal->VariableType = pVar->VariableType;
			pVal->Status = InfoStatus::Normal;
		}
		pRet->Append(pVal);
		it->MoveNext();
	}// it
	m_varvalues = pRet;
	OnPropertyChanged("VariableValues");
}// RefreshVariableValues
void DatasetEditModel::RefreshIndivValues(void) {
	if (!Manager->IsValid) {
		return;
	}
	IVector<DomainValue^>^ oList = nullptr;
	DomainIndiv^ pInd = CurrentIndiv;
	if ((pInd != nullptr) && pInd->IsPersisted) {
		try {
			int n = create_task(Manager->GetIndivValuesCountAsync(pInd)).get();
			if (n > 0) {
				oList = create_task(Manager->GetIndivValuesAsync(pInd, 0, n)).get();
			}
		}
		catch (...) {}
	}// pSet
	if (oList == nullptr) {
		oList = ref new Vector<DomainValue^>();
	}
	IObservableVector<DomainValue^>^ pRet = ref new Vector<DomainValue^>();
	auto vars = Variables;
	auto it = vars->First();
	while (it->HasCurrent) {
		DomainVariable^ pVar = it->Current;
		String^ sigle = pVar->Sigle;
		auto jt = std::find_if(begin(oList), end(oList), [sigle](DomainValue^ p)->bool {
			return (p->VariableSigle == sigle);
		});
		DomainValue^ pVal = nullptr;
		if (jt != end(oList)) {
			pVal = *jt;
		}
		else {
			pVal = ref new DomainValue();
			pVal->DatasetSigle = pVar->DatasetSigle;
			pVal->VariableSigle = pVar->Sigle;
			pVal->IndivSigle = pInd->Sigle;
			pVal->VariableType = pVar->VariableType;
			pVal->Status = InfoStatus::Normal;
		}
		pRet->Append(pVal);
		it->MoveNext();
	}// it
	m_varvalues = pRet;
	OnPropertyChanged("IndivValues");
}// RefreshIndivValues
