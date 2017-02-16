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
InfoItemImpl::InfoItemImpl() :m_status(InfoStatus::Unknown) {

}
InfoItemImpl::InfoItemImpl(IMap<String^, Object^>^ oMap) : m_status(InfoStatus::Unknown) {
	if (oMap->HasKey(InfoStrings::KEY_ID)) {
		m_id = oMap->Lookup(InfoStrings::KEY_ID)->ToString();
	}
	if (oMap->HasKey(InfoStrings::KEY_REV)) {
		m_rev = oMap->Lookup(InfoStrings::KEY_REV)->ToString();
	}
	if (oMap->HasKey(InfoStrings::KEY_DESC)) {
		m_desc = oMap->Lookup(InfoStrings::KEY_DESC)->ToString();
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
	return (s->Length() > 0);
}// InfoItemImpl
void InfoItemImpl::GetMap(IMap<String^, Object^>^ oMap) const {
	String^ s = this->get_Type();
	if (!s->IsEmpty()) {
		oMap->Insert(InfoStrings::KEY_TYPE, s);
	}
	if (m_status != InfoStatus::Unknown) {
		int ival = static_cast<int>(m_status);
		oMap->Insert(InfoStrings::KEY_STATUS, ival);
	}
	if (!m_id->IsEmpty()) {
		oMap->Insert(InfoStrings::KEY_ID, m_id);
	}
	if (!m_rev->IsEmpty()) {
		oMap->Insert(InfoStrings::KEY_REV, m_rev);
	}
	if (!m_desc->IsEmpty()) {
		oMap->Insert(InfoStrings::KEY_DESC, m_desc);
	}
}//GetMap
//////////////////////////////
SigleNamedItemImpl::SigleNamedItemImpl() {

}
SigleNamedItemImpl::SigleNamedItemImpl(IMap<String^, Object^>^ pMap) : InfoItemImpl(pMap) {
	if (pMap->HasKey(InfoStrings::KEY_SIGLE)) {
		m_sigle = pMap->Lookup(InfoStrings::KEY_SIGLE)->ToString();
	}
	if (pMap->HasKey(InfoStrings::KEY_NAME)) {
		m_sigle = pMap->Lookup(InfoStrings::KEY_NAME)->ToString();
	}
}
SigleNamedItemImpl::SigleNamedItemImpl(String^ sSigle) {
	m_sigle = sSigle;
}
SigleNamedItemImpl::~SigleNamedItemImpl() {
}
bool SigleNamedItemImpl::get_IsStoreable(void) const {
	return (!m_sigle->IsEmpty()) && (!m_name->IsEmpty()) && InfoItemImpl::get_IsStoreable();
}
void SigleNamedItemImpl::GetMap(IMap<String^, Object^>^ oMap) const {
	InfoItemImpl::GetMap(oMap);
	if (!m_sigle->IsEmpty()) {
		oMap->Insert(InfoStrings::KEY_SIGLE, m_sigle);
	}
	if (!m_name->IsEmpty()) {
		oMap->Insert(InfoStrings::KEY_NAME, m_name);
	}
}
///////////////////////////////
DatasetImpl::DatasetImpl() {

}
DatasetImpl::DatasetImpl(IMap<String^, Object^>^ pMap) : SigleNamedItemImpl(pMap) {

}
DatasetImpl::DatasetImpl(String^ sSigle) : SigleNamedItemImpl(sSigle) {

}
DatasetImpl::~DatasetImpl() {

}
/////////////////////////
DatasetChildItemImpl::DatasetChildItemImpl() {

}
DatasetChildItemImpl::DatasetChildItemImpl(IMap<String^, Object^>^ pMap) :SigleNamedItemImpl(pMap) {
	if (pMap->HasKey(InfoStrings::KEY_DATASETSIGLE)) {
		m_datasetsigle = pMap->Lookup(InfoStrings::KEY_DATASETSIGLE)->ToString();
	}
}
DatasetChildItemImpl::DatasetChildItemImpl(String^ setsigle, String^ sigle) : SigleNamedItemImpl(sigle), m_datasetsigle(setsigle) {

}
DatasetChildItemImpl::~DatasetChildItemImpl() {

}
bool DatasetChildItemImpl::get_IsStoreable(void) const {
	return (!m_datasetsigle->IsEmpty()) && SigleNamedItemImpl::get_IsStoreable();
}
void DatasetChildItemImpl::GetMap(IMap<String^, Object^>^ oMap) const {
	SigleNamedItemImpl::GetMap(oMap);
	if (!m_datasetsigle->IsEmpty()) {
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
			auto it = o->First();
			while (it->HasCurrent) {
				String^ s = it->Current->ToString();
				if (!s->IsEmpty()) {
					std::wstring ss{ s->Data() };
					m_modalites.push_back(ss);
				}
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
	if (!m_modalites.empty()) {
		IVector<String^> ^ mods = ref new Vector<String^>();
		for (auto it = m_modalites.begin(); it != m_modalites.end(); ++it) {
			std::wstring s = *it;
			String^ ss = ref new String(s.c_str());
			mods->Append(ss);
		}// it
		oMap->Insert(InfoStrings::KEY_MODALITES, mods);
	}// not empty
}// GetMap
IVector<String^>^ VariableImpl::get_Modalites(void) const {
	IVector<String^> ^ mods;
	if (!m_modalites.empty()) {
		mods = ref new Vector<String^>();
		for (auto it = m_modalites.begin(); it != m_modalites.end(); ++it) {
			std::wstring s = *it;
			String^ ss = ref new String(s.c_str());
			mods->Append(ss);
		}// it
	}// not empty
	return mods;
}
void VariableImpl::set_Modalites(IVector<String^>^ pVec) {
	m_modalites.clear();
	if (pVec == nullptr) {
		return;
	}
	auto it = pVec->First();
	while (it->HasCurrent) {
		String^ s = it->Current->ToString();
		if (!s->IsEmpty()) {
			std::wstring ss{ s->Data() };
			m_modalites.push_back(ss);
		}
	}// it
}//set_Modalites
/////////////////////////////
