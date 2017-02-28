#include "pch.h"
#include "domainimpl.h"
#include "infostrings.h"
/////////////////////////////
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Platform::Collections;
using namespace InfoDomain;
/////////////////////////////
InfoItemImpl::InfoItemImpl() :m_modified(false),m_selected(false),m_status(InfoStatus::Unknown) {

}
InfoItemImpl::InfoItemImpl(IMap<String^, Object^>^ oMap) : m_modified(false), m_selected(false), m_status(InfoStatus::Unknown) {
	if (oMap->HasKey(InfoStrings::KEY_ID)) {
		m_id = oMap->Lookup(InfoStrings::KEY_ID)->ToString();
	}
	if (oMap->HasKey(InfoStrings::KEY_REV)) {
		m_rev = oMap->Lookup(InfoStrings::KEY_REV)->ToString();
	}
	if (oMap->HasKey(InfoStrings::KEY_DESC)) {
		m_desc = StringUtils::Trim(oMap->Lookup(InfoStrings::KEY_DESC)->ToString());
	}
	if (oMap->HasKey(InfoStrings::KEY_STATUS)) {
		Object^ o = oMap->Lookup(InfoStrings::KEY_STATUS);
		if (o != nullptr) {
			String^ s = o->ToString();
			std::wstring ss{s->Data()};
			std::wstringstream in{ ss };
			int n = 0;
			in >> n;
			m_status = static_cast<InfoStatus>(n);
		}
	}// status
}
InfoItemImpl::~InfoItemImpl() {

}
String^ InfoItemImpl::get_Type(void) const {
	String^ oRet;
	return (oRet);
}
bool InfoItemImpl::get_IsStoreable(void) const {
	String^ s = this->get_Type();
	return (s != nullptr) && (!s->IsEmpty()) && (this->get_Status() != InfoStatus::Unknown);
}// InfoItemImpl
bool InfoItemImpl::get_IsPersisted(void) const {
	return (m_id != nullptr) && (!m_id->IsEmpty()) &&
		(m_rev != nullptr) && (!m_rev->IsEmpty());
}
void InfoItemImpl::GetMap(IMap<String^, Object^>^ oMap) const {
	String^ s = this->get_Type();
	if ((s != nullptr) && (!s->IsEmpty())) {
		oMap->Insert(InfoStrings::KEY_TYPE, s);
	}
	if (m_status != InfoStatus::Unknown) {
		int ival = static_cast<int>(m_status);
		oMap->Insert(InfoStrings::KEY_STATUS, ival);
	}
	if ((m_id != nullptr) && (!m_id->IsEmpty())) {
		oMap->Insert(InfoStrings::KEY_ID, m_id);
	}
	if ((m_rev != nullptr) && (!m_rev->IsEmpty())) {
		oMap->Insert(InfoStrings::KEY_REV, m_rev);
	}
	if ((m_desc != nullptr) && (!m_desc->IsEmpty())) {
		oMap->Insert(InfoStrings::KEY_DESC, m_desc);
	}
}//GetMap
//////////////////////////////
SigleNamedItemImpl::SigleNamedItemImpl() {

}
SigleNamedItemImpl::SigleNamedItemImpl(IMap<String^, Object^>^ pMap) : InfoItemImpl(pMap) {
	if (pMap->HasKey(InfoStrings::KEY_SIGLE)) {
		m_sigle = StringUtils::ToUpperFormat(pMap->Lookup(InfoStrings::KEY_SIGLE)->ToString());
	}
	if (pMap->HasKey(InfoStrings::KEY_NAME)) {
		m_name = StringUtils::FormatName(pMap->Lookup(InfoStrings::KEY_NAME)->ToString());
	}
}
SigleNamedItemImpl::SigleNamedItemImpl(String^ sSigle) {
	m_sigle = StringUtils::ToUpperFormat(sSigle);
}
SigleNamedItemImpl::~SigleNamedItemImpl() {
}
bool SigleNamedItemImpl::get_IsStoreable(void) const {
	return (m_sigle != nullptr) && (m_name != nullptr) &&
		(!m_sigle->IsEmpty()) && (!m_name->IsEmpty()) && InfoItemImpl::get_IsStoreable();
}
void SigleNamedItemImpl::GetMap(IMap<String^, Object^>^ oMap) const {
	InfoItemImpl::GetMap(oMap);
	if ((m_sigle != nullptr) && (!m_sigle->IsEmpty())) {
		oMap->Insert(InfoStrings::KEY_SIGLE, m_sigle);
	}
	if ((m_name != nullptr) && (!m_name->IsEmpty())) {
		oMap->Insert(InfoStrings::KEY_NAME, m_name);
	}
}
///////////////////////////////
DatasetImpl::DatasetImpl() {

}
DatasetImpl::DatasetImpl(IMap<String^, Object^>^ pMap) : SigleNamedItemImpl(pMap) {
	if (pMap->HasKey(InfoStrings::KEY_ANNEE)) {
		m_annee = pMap->Lookup(InfoStrings::KEY_ANNEE)->ToString();
	}
}
DatasetImpl::DatasetImpl(String^ sSigle) : SigleNamedItemImpl(sSigle) {

}
DatasetImpl::~DatasetImpl() {

}
void DatasetImpl::GetMap(IMap<String^, Object^>^ oMap) const {
	SigleNamedItemImpl::GetMap(oMap);
	if ((m_annee != nullptr) && (!m_annee->IsEmpty())) {
		oMap->Insert(InfoStrings::KEY_ANNEE, m_annee);
	}
}
String^ DatasetImpl::get_Type(void) const {
	return InfoStrings::TYPE_DATASET;
}
/////////////////////////
DatasetChildItemImpl::DatasetChildItemImpl() {

}
DatasetChildItemImpl::DatasetChildItemImpl(IMap<String^, Object^>^ pMap) :SigleNamedItemImpl(pMap) {
	if (pMap->HasKey(InfoStrings::KEY_DATASETSIGLE)) {
		m_datasetsigle = StringUtils::ToUpperFormat(pMap->Lookup(InfoStrings::KEY_DATASETSIGLE)->ToString());
	}
}
DatasetChildItemImpl::DatasetChildItemImpl(String^ setsigle, String^ sigle) : SigleNamedItemImpl(sigle) {
	m_datasetsigle = StringUtils::ToUpperFormat(setsigle);
}
DatasetChildItemImpl::~DatasetChildItemImpl() {

}
bool DatasetChildItemImpl::get_IsStoreable(void) const {
	return (m_datasetsigle != nullptr) &&
		(!m_datasetsigle->IsEmpty()) && SigleNamedItemImpl::get_IsStoreable();
}
void DatasetChildItemImpl::GetMap(IMap<String^, Object^>^ oMap) const {
	SigleNamedItemImpl::GetMap(oMap);
	if ((m_datasetsigle != nullptr) && (!m_datasetsigle->IsEmpty())) {
		oMap->Insert(InfoStrings::KEY_DATASETSIGLE, m_datasetsigle);
	}
}
/////////////////////////////
IndivImpl::IndivImpl() {

}
IndivImpl::IndivImpl(IMap<String^, Object^>^ pMap) :DatasetChildItemImpl(pMap) {

}
IndivImpl::IndivImpl(String^ setsigle, String^ sigle) : DatasetChildItemImpl(setsigle, sigle) {

}
IndivImpl::~IndivImpl() {

}
String^ IndivImpl::get_Type(void) const {
	return InfoStrings::TYPE_INDIV;
}
/////////////////////////
VariableImpl::VariableImpl() :m_datatype(InfoDataType::Unknown), m_kind(InfoKind::Unknown) {

}
VariableImpl::VariableImpl(IMap<String^, Object^>^ pMap) : DatasetChildItemImpl(pMap),
m_datatype(InfoDataType::Unknown), m_kind(InfoKind::Unknown) {
	if (pMap->HasKey(InfoStrings::KEY_DATATYPE)) {
		Object^ o = pMap->Lookup(InfoStrings::KEY_DATATYPE);
		if (o != nullptr) {
			String^ s = o->ToString();
			std::wstring ss{ s->Data() };
			std::wstringstream in{ ss };
			int n = 0;
			in >> n;
			m_datatype = static_cast<InfoDataType>(n);
		}
	}// datatype
	if (pMap->HasKey(InfoStrings::KEY_VARIABLEKIND)) {
		Object^ o = pMap->Lookup(InfoStrings::KEY_VARIABLEKIND);
		if (o != nullptr) {
			String^ s = o->ToString();
			std::wstring ss{ s->Data() };
			std::wstringstream in{ ss };
			int n = 0;
			in >> n;
			m_kind = static_cast<InfoKind>(n);
		}
	}// kind
	if (pMap->HasKey(InfoStrings::KEY_MODALITES)) {
		Object^ oo = pMap->Lookup(InfoStrings::KEY_MODALITES);
		if (oo != nullptr) {
			IVector<Object^>^ o = (IVector<Object^>^)oo;
			m_modalites = ref new Vector<String^>();
			auto it = o->First();
			while (it->HasCurrent) {
				if (it->Current != nullptr) {
					String^ s = it->Current->ToString();
					if (!s->IsEmpty()) {
						m_modalites->Append(s);
					}
				}
				it->MoveNext();
			}// it
		}
	}
}// datatype
VariableImpl::VariableImpl(String^ setsigle, String^ sigle) :DatasetChildItemImpl(setsigle, sigle),
m_datatype(InfoDataType::Unknown), m_kind(InfoKind::Unknown) {

}
VariableImpl:: ~VariableImpl() {

}
bool VariableImpl::get_IsStoreable(void) const {
	return (m_datatype != InfoDataType::Unknown) && (m_kind != InfoKind::Unknown) &&
		DatasetChildItemImpl::get_IsStoreable();
}
void VariableImpl::GetMap(IMap<String^, Object^>^ oMap) const {
	DatasetChildItemImpl::GetMap(oMap);
	if (m_datatype != InfoDataType::Unknown) {
		int n = static_cast<int>(m_datatype);
		oMap->Insert(InfoStrings::KEY_DATATYPE, n);
	}
	if (m_kind != InfoKind::Unknown) {
		int n = static_cast<int>(m_kind);
		oMap->Insert(InfoStrings::KEY_VARIABLEKIND, n);
	}
	if ((m_modalites != nullptr) && (m_modalites->Size > 0)) {
		oMap->Insert(InfoStrings::KEY_MODALITES, m_modalites);
	}// not empty
}// GetMap
IVector<String^>^ VariableImpl::get_Modalites(void)  {
	if (m_modalites == nullptr) {
		m_modalites = ref new Vector<String^>();
	}
	return m_modalites;
}
void VariableImpl::set_Modalites(IVector<String^>^ pVec) {
	m_modalites = pVec;
}//set_Modalites
String^ VariableImpl::get_Type(void) const {
	return InfoStrings::TYPE_VARIABLE;
}
/////////////////////////////
EtudiantImpl::EtudiantImpl(){

}
EtudiantImpl::EtudiantImpl(IMap<String^, Object^>^ pMap) :InfoItemImpl(pMap) {
	if (pMap->HasKey(InfoStrings::KEY_ANNEE)) {
		m_annee = pMap->Lookup(InfoStrings::KEY_ANNEE)->ToString();
	}
	if (pMap->HasKey(InfoStrings::KEY_DOSSIER)) {
		m_dossier = pMap->Lookup(InfoStrings::KEY_DOSSIER)->ToString();
	}
	if (pMap->HasKey(InfoStrings::KEY_SEXE)) {
		m_sexe = pMap->Lookup(InfoStrings::KEY_SEXE)->ToString();
	}
	if (pMap->HasKey(InfoStrings::KEY_BIRTHYEAR)) {
		m_birth = pMap->Lookup(InfoStrings::KEY_BIRTHYEAR)->ToString();
	}
	if (pMap->HasKey(InfoStrings::KEY_FIRSTNAME)) {
		m_firstname = pMap->Lookup(InfoStrings::KEY_FIRSTNAME)->ToString();
	}
	if (pMap->HasKey(InfoStrings::KEY_LASTNAME)) {
		m_lastname = pMap->Lookup(InfoStrings::KEY_LASTNAME)->ToString();
	}
	if (pMap->HasKey(InfoStrings::KEY_DEPARTEMENT)) {
		m_dep = pMap->Lookup(InfoStrings::KEY_DEPARTEMENT)->ToString();
	}
	if (pMap->HasKey(InfoStrings::KEY_VILLE)) {
		m_ville = pMap->Lookup(InfoStrings::KEY_VILLE)->ToString();
	}
	if (pMap->HasKey(InfoStrings::KEY_LYCEE)) {
		m_lycee = pMap->Lookup(InfoStrings::KEY_LYCEE)->ToString();
	}
	if (pMap->HasKey(InfoStrings::KEY_GROUPE)) {
		m_groupe = pMap->Lookup(InfoStrings::KEY_GROUPE)->ToString();
	}
	if (pMap->HasKey(InfoStrings::KEY_SERIEBAC)) {
		m_seriebac = pMap->Lookup(InfoStrings::KEY_SERIEBAC)->ToString();
	}
	if (pMap->HasKey(InfoStrings::KEY_OPTIONBAC)) {
		m_optionbac = pMap->Lookup(InfoStrings::KEY_OPTIONBAC)->ToString();
	}
	if (pMap->HasKey(InfoStrings::KEY_MENTIONBAC)) {
		m_mentionbac = pMap->Lookup(InfoStrings::KEY_MENTIONBAC)->ToString();
	}
	if (pMap->HasKey(InfoStrings::KEY_APB)) {
		m_apb = pMap->Lookup(InfoStrings::KEY_APB)->ToString();
	}
	if (pMap->HasKey(InfoStrings::KEY_SUP)) {
		m_sup = pMap->Lookup(InfoStrings::KEY_SUP)->ToString();
	}
	if (pMap->HasKey(InfoStrings::KEY_REDOUBLANT)) {
		m_red = pMap->Lookup(InfoStrings::KEY_REDOUBLANT)->ToString();
	}
	if (pMap->HasKey(InfoStrings::KEY_AVATAR)) {
		m_avatar = pMap->Lookup(InfoStrings::KEY_AVATAR)->ToString();
	}
	if (pMap->HasKey(InfoStrings::KEY_EMAIL)) {
		m_email = pMap->Lookup(InfoStrings::KEY_EMAIL)->ToString();
	}
	if (pMap->HasKey(InfoStrings::KEY_DATASETS)) {
		Object^ oo = pMap->Lookup(InfoStrings::KEY_DATASETS);
		if (oo != nullptr) {
			IVector<Object^>^ o = (IVector<Object^>^)oo;
			m_sets = ref new Vector<String^>();
			auto it = o->First();
			while (it->HasCurrent) {
				if (it->Current != nullptr) {
					String^ s = it->Current->ToString();
					if (!s->IsEmpty()) {
						m_sets->Append(s);
					}
				}
				it->MoveNext();
			}// it
		}
	}
}
EtudiantImpl::~EtudiantImpl() {

}
bool EtudiantImpl::get_IsStoreable(void) const {
	return (m_dossier != nullptr) && (!m_dossier->IsEmpty()) &&
		(m_firstname != nullptr) && (!m_firstname->IsEmpty()) &&
		(m_lastname != nullptr) && (!m_lastname->IsEmpty()) &&
		InfoItemImpl::get_IsStoreable();
}
void EtudiantImpl::GetMap(IMap<String^, Object^>^ oMap) const {
	InfoItemImpl::GetMap(oMap);
	if ((m_annee != nullptr) && (!m_annee->IsEmpty())) {
		oMap->Insert(InfoStrings::KEY_ANNEE, m_annee);
	}
	if ((m_dossier != nullptr) && (!m_dossier->IsEmpty())) {
		oMap->Insert(InfoStrings::KEY_DOSSIER, m_dossier);
	}
	if ((m_sexe != nullptr) && (!m_sexe->IsEmpty())) {
		oMap->Insert(InfoStrings::KEY_SEXE, m_sexe);
	}
	if ((m_birth != nullptr) && (!m_birth->IsEmpty())) {
		oMap->Insert(InfoStrings::KEY_BIRTHYEAR, m_birth);
	}
	if ((m_firstname != nullptr) && (!m_firstname->IsEmpty())) {
		oMap->Insert(InfoStrings::KEY_FIRSTNAME, m_firstname);
	}
	if ((m_lastname != nullptr) && (!m_lastname->IsEmpty())) {
		oMap->Insert(InfoStrings::KEY_LASTNAME, m_lastname);
	}
	if ((m_dep != nullptr) && (!m_dep->IsEmpty())) {
		oMap->Insert(InfoStrings::KEY_DEPARTEMENT, m_dep);
	}
	if ((m_ville != nullptr) && (!m_ville->IsEmpty())) {
		oMap->Insert(InfoStrings::KEY_VILLE, m_ville);
	}
	if ((m_lycee != nullptr) && (!m_lycee->IsEmpty())) {
		oMap->Insert(InfoStrings::KEY_LYCEE, m_lycee);
	}
	if ((m_groupe != nullptr) && (!m_groupe->IsEmpty())) {
		oMap->Insert(InfoStrings::KEY_GROUPE, m_groupe);
	}
	if ((m_seriebac != nullptr) && (!m_seriebac->IsEmpty())) {
		oMap->Insert(InfoStrings::KEY_SERIEBAC, m_seriebac);
	}
	if ((m_optionbac != nullptr) && (!m_optionbac->IsEmpty())) {
		oMap->Insert(InfoStrings::KEY_OPTIONBAC, m_optionbac);
	}
	if ((m_mentionbac != nullptr) && (!m_mentionbac->IsEmpty())) {
		oMap->Insert(InfoStrings::KEY_MENTIONBAC, m_mentionbac);
	}
	if ((m_apb != nullptr) && (!m_apb->IsEmpty())) {
		oMap->Insert(InfoStrings::KEY_APB, m_apb);
	}
	if ((m_sup != nullptr) && (!m_sup->IsEmpty())) {
		oMap->Insert(InfoStrings::KEY_SUP, m_sup);
	}
	if ((m_red != nullptr) && (!m_red->IsEmpty())) {
		oMap->Insert(InfoStrings::KEY_REDOUBLANT, m_red);
	}
	if ((m_avatar != nullptr) && (!m_avatar->IsEmpty())) {
		oMap->Insert(InfoStrings::KEY_AVATAR, m_avatar);
	}
	if ((m_email != nullptr) && (!m_email->IsEmpty())) {
		oMap->Insert(InfoStrings::KEY_EMAIL, m_email);
	}
	if ((m_sets != nullptr) && (m_sets->Size > 0)) {
		oMap->Insert(InfoStrings::KEY_DATASETS, m_sets);
	}// not empty
}// GetMap
String^ EtudiantImpl::get_Type(void) const {
	return InfoStrings::TYPE_ETUDIANT;
}
