#include "pch.h"
#include "DomainDataset.h"
#include "StringUtils.h"
#include "infostrings.h"
//
using namespace GenDomainData;
using namespace Platform;

DomainDataset::DomainDataset()
{
	m_modified = false;
	m_selected = false;
	m_status = InfoStatus::Unknown;
}
DomainDataset::DomainDataset(IMap<String^, Object^>^ oMap) {
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
		if (oMap->HasKey(InfoStrings::KEY_ANNEE)) {
			m_annee = oMap->Lookup(InfoStrings::KEY_ANNEE)->ToString();
		}
	}// oMap
}// DomainDataset
IMap<String^, Object^>^  DomainDataset::GetMap(void) {
	IMap<String^, Object^>^ oMap = ref new Map<String^, Object^>();
	oMap->Insert(InfoStrings::KEY_TYPE, InfoStrings::TYPE_DATASET);
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
	if ((m_annee != nullptr) && (!m_annee->IsEmpty())) {
		oMap->Insert(InfoStrings::KEY_ANNEE, m_annee);
	}
	return oMap;
}// GetMap
String ^ DomainDataset::ToString()
{
	return Sigle;
}
void DomainDataset::OnPropertyChanged(String^ propertyName)
{
	PropertyChanged(this, ref new PropertyChangedEventArgs(propertyName));
}
String^ DomainDataset::Id::get() {
	return m_id;
}
void DomainDataset::Id::set(String^ value) {
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
String^ DomainDataset::Rev::get() {
	return m_rev;
}
void DomainDataset::Rev::set(String^ value) {
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
InfoStatus DomainDataset::Status::get() {
	return m_status;
}
void DomainDataset::Status::set(InfoStatus value) {
	if (value != m_status) {
		m_status = value;
		OnPropertyChanged("Status");
		IsModified = true;
	}
}
String^ DomainDataset::StatusString::get() {
	return StringUtils::StatusToString(m_status);
}
void DomainDataset::StatusString::set(String^ value) {
	String^ old = StatusString;
	String^ cur = StringUtils::FormatName(value);
	if (old != cur) {
		Status = StringUtils::StringToStatus(cur);
		OnPropertyChanged("StatusString");
	}
}
String^ DomainDataset::Observations::get() {
	return m_desc;
}
void DomainDataset::Observations::set(String^ value) {
	String^ old = ((m_desc == nullptr) || m_desc->IsEmpty()) ? "" : m_desc;
	String^ cur = StringUtils::FormatName(value);
	if (old != cur) {
		m_desc = cur;
		OnPropertyChanged("Observations");
		IsModified = true;
	}
}
String^ DomainDataset::Annee::get() {
	return m_annee;
}
void DomainDataset::Annee::set(String^ value) {
	String^ old = ((m_annee == nullptr) || m_annee->IsEmpty()) ? "" : m_annee;
	String^ cur = StringUtils::ToUpperFormat(value);
	if (old != cur) {
		m_annee = cur;
		OnPropertyChanged("Annee");
		IsModified = true;
	}
}
String^ DomainDataset::Sigle::get() {
	return m_sigle;
}
void DomainDataset::Sigle::set(String^ value) {
	String^ old = ((m_sigle == nullptr) || m_sigle->IsEmpty()) ? "" : m_sigle;
	String^ cur = StringUtils::ToUpperFormat(value);
	if (old != cur) {
		m_sigle = cur;
		OnPropertyChanged("Sigle");
		IsModified = true;
	}
}
String^ DomainDataset::Name::get() {
	return m_name;
}
void DomainDataset::Name::set(String^ value) {
	String^ old = ((m_name == nullptr) || m_name->IsEmpty()) ? "" : m_name;
	String^ cur = StringUtils::ToUpperFormat(value);
	if (old != cur) {
		m_name = cur;
		OnPropertyChanged("Name");
		IsModified = true;;
	}
}
bool DomainDataset::IsModified::get() {
	return m_modified;
}
void DomainDataset::IsModified::set(bool value) {
	if (m_modified != value) {
		m_modified = value;
		OnPropertyChanged("IsModified");
		OnPropertyChanged("IsStoreable");
	}
}
bool DomainDataset::IsNew::get() {
	return (!IsPersisted);
}
bool DomainDataset::IsSelected::get() {
	return m_selected;
}
void DomainDataset::IsSelected::set(bool value) {
	if (m_selected != value) {
		m_selected = value;
		OnPropertyChanged("IsSelected");
	}
}
bool DomainDataset::IsPersisted::get() {
	return (Id != nullptr) && (Rev != nullptr) &&
		(!Id->IsEmpty()) && (!Rev->IsEmpty());
}
bool DomainDataset::IsStoreable::get() {
	return (Sigle != nullptr) && (Name != nullptr) &&
		(!Sigle->IsEmpty()) && (!Name->IsEmpty()) &&
		(Status != InfoStatus::Unknown) && IsModified;
}
DomainDataset^ DomainDataset::Clone() {
	DomainDataset^ p = ref new DomainDataset();
	Copy(p);
	return p;
}// clone
void DomainDataset::NotifyAll(void) {
	OnPropertyChanged("Id");
	OnPropertyChanged("Rev");
	OnPropertyChanged("Status");
	OnPropertyChanged("StatusString");
	OnPropertyChanged("Observations");
	OnPropertyChanged("Sigle");
	OnPropertyChanged("Name");
	OnPropertyChanged("Annee");
	OnPropertyChanged("IsSelected");
	OnPropertyChanged("IsPersisted");
	OnPropertyChanged("IsModified");
	OnPropertyChanged("IsStoreable");
}// NotifyAll
void DomainDataset::Clear(void) {
	m_modified = false;
	m_selected = false;
	m_status = InfoStatus::Unknown;
	m_id = nullptr;
	m_rev = nullptr;
	m_desc = nullptr;
	m_annee = nullptr;
	m_sigle = nullptr;
	m_name = nullptr;
	NotifyAll();
}
void DomainDataset::Copy(DomainDataset ^p) {
	if (p != nullptr) {
		p->m_status = this->m_status;
		p->m_modified = this->m_modified;
		p->m_selected = this->m_selected;
		p->m_id = ((m_id != nullptr) && (!m_id->IsEmpty())) ? ref new String(m_id->Data()) : nullptr;
		p->m_rev = ((m_rev != nullptr) && (!m_rev->IsEmpty())) ? ref new String(m_rev->Data()) : nullptr;
		p->m_desc = ((m_desc != nullptr) && (!m_desc->IsEmpty())) ? ref new String(m_desc->Data()) : nullptr;
		p->m_annee = ((m_annee != nullptr) && (!m_annee->IsEmpty())) ? ref new String(m_annee->Data()) : nullptr;
		p->m_sigle = ((m_sigle != nullptr) && (!m_sigle->IsEmpty())) ? ref new String(m_sigle->Data()) : nullptr;
		p->m_name = ((m_name != nullptr) && (!m_name->IsEmpty())) ? ref new String(m_name->Data()) : nullptr;
		p->NotifyAll();
	}// p
}// Copy
IVector<String^>^ DomainDataset::AllStatusStrings::get()
{
	return StringUtils::AllStatusStrings();
}
