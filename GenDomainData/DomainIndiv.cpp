#include "pch.h"
#include "DomainIndiv.h"
#include "StringUtils.h"
#include "infostrings.h"
//
using namespace GenDomainData;
using namespace Platform;


DomainIndiv::DomainIndiv()
{
	m_modified = false;
	m_selected = false;
	m_status = InfoStatus::Unknown;
}
DomainIndiv::DomainIndiv(IMap<String^, Object^>^ oMap) {
	m_modified = false;
	m_selected = false;
	m_status = InfoStatus::Unknown;
	if ((oMap != nullptr) && (oMap->Size > 0)) {
		if (oMap->HasKey(InfoStrings::KEY_ID)) {
			m_id = oMap->Lookup(InfoStrings::KEY_ID)->ToString();
		}
		if (oMap->HasKey(InfoStrings::KEY_REV)) {
			m_rev = oMap->Lookup(InfoStrings::KEY_REV)->ToString();
		}
		if (oMap->HasKey(InfoStrings::KEY_DESC)) {
			m_desc = StringUtils::Trim(oMap->Lookup(InfoStrings::KEY_DESC)->ToString());
		}
		if (oMap->HasKey(InfoStrings::KEY_SIGLE)) {
			m_sigle = StringUtils::ToUpperFormat(oMap->Lookup(InfoStrings::KEY_SIGLE)->ToString());
		}
		if (oMap->HasKey(InfoStrings::KEY_NAME)) {
			m_name = StringUtils::FormatName(oMap->Lookup(InfoStrings::KEY_NAME)->ToString());
		}
		if (oMap->HasKey(InfoStrings::KEY_STATUS)) {
			Object^ o = oMap->Lookup(InfoStrings::KEY_STATUS);
			if (o != nullptr) {
				String^ s = o->ToString();
				int n = StringUtils::StringToInteger(s);
				m_status = static_cast<InfoStatus>(n);
			}
		}// status
		if (oMap->HasKey(InfoStrings::KEY_DATASETSIGLE)) {
			m_datasetsigle = oMap->Lookup(InfoStrings::KEY_DATASETSIGLE)->ToString();
		}

	}// oMap
}// DomainVariable
IMap<String^, Object^>^  DomainIndiv::GetMap(void) {
	IMap<String^, Object^>^ oMap = ref new Map<String^, Object^>();
	oMap->Insert(InfoStrings::KEY_TYPE, InfoStrings::TYPE_INDIV);
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
	if ((m_sigle != nullptr) && (!m_sigle->IsEmpty())) {
		oMap->Insert(InfoStrings::KEY_SIGLE, m_sigle);
	}
	if ((m_name != nullptr) && (!m_name->IsEmpty())) {
		oMap->Insert(InfoStrings::KEY_NAME, m_name);
	}
	if ((m_datasetsigle != nullptr) && (!m_datasetsigle->IsEmpty())) {
		oMap->Insert(InfoStrings::KEY_DATASETSIGLE, m_datasetsigle);
	}
	return oMap;
}// GetMap
String ^ DomainIndiv::ToString()
{
	return Sigle;
}
void DomainIndiv::OnPropertyChanged(String^ propertyName)
{
	PropertyChanged(this, ref new PropertyChangedEventArgs(propertyName));
}
String^ DomainIndiv::Id::get() {
	return m_id;
}
IVector<String^>^ DomainIndiv::AllStatusStrings::get()
{
	return StringUtils::AllStatusStrings();
}
void DomainIndiv::Id::set(String^ value) {
	String^ old = ((m_id == nullptr) || m_id->IsEmpty()) ? "" : m_id;
	String^ cur = StringUtils::Trim(value);
	if (old != cur) {
		m_id = cur;
		OnPropertyChanged("Id");
		OnPropertyChanged("IsPersisted");
		OnPropertyChanged("IsNew");
		IsModified = true;
	}
}
String^ DomainIndiv::Rev::get() {
	return m_rev;
}
void DomainIndiv::Rev::set(String^ value) {
	String^ old = ((m_rev == nullptr) || m_rev->IsEmpty()) ? "" : m_rev;
	String^ cur = StringUtils::Trim(value);
	if (old != cur) {
		m_rev = cur;
		OnPropertyChanged("Rev");
		OnPropertyChanged("IsPersisted");
		OnPropertyChanged("IsNew");
		IsModified = true;
	}
}
//
InfoStatus DomainIndiv::Status::get() {
	return m_status;
}
void DomainIndiv::Status::set(InfoStatus value) {
	if (value != m_status) {
		m_status = value;
		OnPropertyChanged("Status");
		IsModified = true;
	}
}
String^ DomainIndiv::StatusString::get() {
	return StringUtils::StatusToString(m_status);
}
void DomainIndiv::StatusString::set(String^ value) {
	String^ old = StatusString;
	String^ cur = StringUtils::FormatName(value);
	if (old != cur) {
		Status = StringUtils::StringToStatus(cur);
		OnPropertyChanged("StatusString");
	}
}
//
String^ DomainIndiv::Observations::get() {
	return m_desc;
}
void DomainIndiv::Observations::set(String^ value) {
	String^ old = ((m_desc == nullptr) || m_desc->IsEmpty()) ? "" : m_desc;
	String^ cur = StringUtils::FormatName(value);
	if (old != cur) {
		m_desc = cur;
		OnPropertyChanged("Observations");
		IsModified = true;
	}
}
String^ DomainIndiv::DatasetSigle::get() {
	return m_datasetsigle;
}
void DomainIndiv::DatasetSigle::set(String^ value) {
	String^ old = ((m_datasetsigle == nullptr) || m_datasetsigle->IsEmpty()) ? "" : m_datasetsigle;
	String^ cur = StringUtils::ToUpperFormat(value);
	if (old != cur) {
		m_datasetsigle = cur;
		OnPropertyChanged("DatasetSigle");
		IsModified = true;
	}
}
String^ DomainIndiv::Sigle::get() {
	return m_sigle;
}
void DomainIndiv::Sigle::set(String^ value) {
	String^ old = ((m_sigle == nullptr) || m_sigle->IsEmpty()) ? "" : m_sigle;
	String^ cur = StringUtils::ToUpperFormat(value);
	if (old != cur) {
		m_sigle = cur;
		OnPropertyChanged("Sigle");
		IsModified = true;
	}
}
String^ DomainIndiv::Name::get() {
	return m_name;
}
void DomainIndiv::Name::set(String^ value) {
	String^ old = ((m_name == nullptr) || m_name->IsEmpty()) ? "" : m_name;
	String^ cur = StringUtils::ToUpperFormat(value);
	if (old != cur) {
		m_name = cur;
		OnPropertyChanged("Name");
		IsModified = true;
	}
}
bool DomainIndiv::IsModified::get() {
	return m_modified;
}
void DomainIndiv::IsModified::set(bool value) {
	if (m_modified != value) {
		m_modified = value;
		OnPropertyChanged("IsModified");
		OnPropertyChanged("IsStoreable");
	}
}
bool DomainIndiv::IsSelected::get() {
	return m_selected;
}
bool DomainIndiv::IsNew::get() {
	return (!IsPersisted);
}
void DomainIndiv::IsSelected::set(bool value) {
	if (m_selected != value) {
		m_selected = value;
		OnPropertyChanged("IsSelected");
	}
}
bool DomainIndiv::IsPersisted::get() {
	return (Id != nullptr) && (Rev != nullptr) &&
		(!Id->IsEmpty()) && (!Rev->IsEmpty());
}
bool DomainIndiv::IsStoreable::get() {
	return (Sigle != nullptr) && (Name != nullptr) && (DatasetSigle != nullptr) &&
		(!DatasetSigle->IsEmpty()) &&
		(!Sigle->IsEmpty()) && (!Name->IsEmpty()) &&
		(Status != InfoStatus::Unknown) && IsModified;
}
DomainIndiv^ DomainIndiv::Clone() {
	DomainIndiv^ p = ref new DomainIndiv();
	Copy(p);
	return p;
}// clone
void DomainIndiv::NotifyAll(void) {
	OnPropertyChanged("Id");
	OnPropertyChanged("Rev");
	OnPropertyChanged("Status");
	OnPropertyChanged("StatusString");
	OnPropertyChanged("Observations");
	OnPropertyChanged("Sigle");
	OnPropertyChanged("Name");
	OnPropertyChanged("DatasetSigle");
	OnPropertyChanged("IsSelected");
	OnPropertyChanged("IsPersisted");
	OnPropertyChanged("IsModified");
	OnPropertyChanged("IsStoreable");
}// NotifyAll
void DomainIndiv::Clear(void) {
	m_modified = false;
	m_selected = false;
	m_status = InfoStatus::Unknown;
	m_id = nullptr;
	m_rev = nullptr;
	m_desc = nullptr;
	m_datasetsigle = nullptr;
	m_sigle = nullptr;
	m_name = nullptr;
	NotifyAll();
}
void DomainIndiv::Copy(DomainIndiv ^p) {
	if (p != nullptr) {
		p->m_status = this->m_status;
		p->m_modified = this->m_modified;
		p->m_selected = this->m_selected;
		p->m_id = ((m_id != nullptr) && (!m_id->IsEmpty())) ? ref new String(m_id->Data()) : nullptr;
		p->m_rev = ((m_rev != nullptr) && (!m_rev->IsEmpty())) ? ref new String(m_rev->Data()) : nullptr;
		p->m_desc = ((m_desc != nullptr) && (!m_desc->IsEmpty())) ? ref new String(m_desc->Data()) : nullptr;
		p->m_datasetsigle = ((m_datasetsigle != nullptr) && (!m_datasetsigle->IsEmpty())) ? ref new String(m_datasetsigle->Data()) : nullptr;
		p->m_sigle = ((m_sigle != nullptr) && (!m_sigle->IsEmpty())) ? ref new String(m_sigle->Data()) : nullptr;
		p->m_name = ((m_name != nullptr) && (!m_name->IsEmpty())) ? ref new String(m_name->Data()) : nullptr;
		p->m_modified = false;
		p->NotifyAll();
	}// p
}// Copy
