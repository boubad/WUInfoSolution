#include "pch.h"
#include "DomainValue.h"
#include "StringUtils.h"
#include "infostrings.h"
//
using namespace GenDomainData;
using namespace Platform;


DomainValue::DomainValue()
{
	m_modified = false;
	m_selected = false;
	m_status = InfoStatus::Unknown;
	m_type = InfoDataType::Unknown;
}
DomainValue::DomainValue(IMap<String^, Object^>^ oMap) {
	m_modified = false;
	m_selected = false;
	m_status = InfoStatus::Unknown;
	m_type = InfoDataType::Unknown;
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
		if (oMap->HasKey(InfoStrings::KEY_VARIABLESIGLE)) {
			m_varsigle = StringUtils::ToUpperFormat(oMap->Lookup(InfoStrings::KEY_VARIABLESIGLE)->ToString());
		}
		if (oMap->HasKey(InfoStrings::KEY_INDIVSIGLE)) {
			m_indsigle = StringUtils::FormatName(oMap->Lookup(InfoStrings::KEY_INDIVSIGLE)->ToString());
		}
		if (oMap->HasKey(InfoStrings::KEY_STATUS)) {
			Object^ o = oMap->Lookup(InfoStrings::KEY_STATUS);
			if (o != nullptr) {
				String^ s = o->ToString();
				int n = StringUtils::StringToInteger(s);
				m_status = static_cast<InfoStatus>(n);
			}
		}// status
		if (oMap->HasKey(InfoStrings::KEY_DATATYPE)) {
			Object^ o = oMap->Lookup(InfoStrings::KEY_DATATYPE);
			if (o != nullptr) {
				String^ s = o->ToString();
				int n = StringUtils::StringToInteger(s);
				m_type = static_cast<InfoDataType>(n);
			}
		}// type
		if (oMap->HasKey(InfoStrings::KEY_DATASETSIGLE)) {
			m_datasetsigle = oMap->Lookup(InfoStrings::KEY_DATASETSIGLE)->ToString();
		}
		if (oMap->HasKey(InfoStrings::KEY_VALUE)) {
			m_val = oMap->Lookup(InfoStrings::KEY_VALUE);
		}
	}// oMap
}// DomainValue
IMap<String^, Object^>^  DomainValue::GetMap(void) {
	IMap<String^, Object^>^ oMap = ref new Map<String^, Object^>();
	oMap->Insert(InfoStrings::KEY_TYPE, InfoStrings::TYPE_VALUE);
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
	if ((m_varsigle != nullptr) && (!m_varsigle->IsEmpty())) {
		oMap->Insert(InfoStrings::KEY_VARIABLESIGLE, m_varsigle);
	}
	if ((m_indsigle != nullptr) && (!m_indsigle->IsEmpty())) {
		oMap->Insert(InfoStrings::KEY_INDIVSIGLE, m_indsigle);
	}
	if ((m_datasetsigle != nullptr) && (!m_datasetsigle->IsEmpty())) {
		oMap->Insert(InfoStrings::KEY_DATASETSIGLE, m_datasetsigle);
	}
	if (m_type != InfoDataType::Unknown) {
		int ival = static_cast<int>(m_type);
		oMap->Insert(InfoStrings::KEY_DATATYPE, ival);
	}
	if (m_val != nullptr) {
		oMap->Insert(InfoStrings::KEY_VALUE, m_val);
	}
	return oMap;
}// GetMap
String ^ DomainValue::ToString()
{
	return (m_val == nullptr) ? "" : m_val->ToString();
}
void DomainValue::OnPropertyChanged(String^ propertyName)
{
	PropertyChanged(this, ref new PropertyChangedEventArgs(propertyName));
}
String^ DomainValue::Id::get() {
	return m_id;
}
void DomainValue::Id::set(String^ value) {
	String^ old = ((m_id == nullptr) || m_id->IsEmpty()) ? "" : m_id;
	String^ cur = StringUtils::Trim(value);
	if (old != cur) {
		m_id = cur;
		m_modified = true;
		OnPropertyChanged("Id");
		OnPropertyChanged("IsPersisted");
		OnPropertyChanged("IsModified");
	}
}
String^ DomainValue::Rev::get() {
	return m_rev;
}
void DomainValue::Rev::set(String^ value) {
	String^ old = ((m_rev == nullptr) || m_rev->IsEmpty()) ? "" : m_rev;
	String^ cur = StringUtils::Trim(value);
	if (old != cur) {
		m_rev = cur;
		m_modified = true;
		OnPropertyChanged("Rev");
		OnPropertyChanged("IsPersisted");
		OnPropertyChanged("IsModified");
	}
}
//
InfoStatus DomainValue::Status::get() {
	return m_status;
}
void DomainValue::Status::set(InfoStatus value) {
	if (value != m_status) {
		m_status = value;
		m_modified = true;
		OnPropertyChanged("Status");
		OnPropertyChanged("IsModified");
		OnPropertyChanged("IsStoreable");
	}
}
String^ DomainValue::StatusString::get() {
	return StringUtils::StatusToString(m_status);
}
void DomainValue::StatusString::set(String^ value) {
	String^ old = StatusString;
	String^ cur = StringUtils::FormatName(value);
	if (old != cur) {
		Status = StringUtils::StringToStatus(cur);
		OnPropertyChanged("StatusString");
	}
}
//
InfoDataType DomainValue::VariableType::get() {
	return m_type;
}
void DomainValue::VariableType::set(InfoDataType value) {
	if (value != m_type) {
		m_type = value;
		m_modified = true;
		OnPropertyChanged("VariableType");
		OnPropertyChanged("IsModified");
		OnPropertyChanged("IsStoreable");
	}
}
String^ DomainValue::VariableTypeString::get() {
	return StringUtils::DataTypeToString(m_type);
}
//
Object^ DomainValue::Value::get() {
	return m_val;
}
void DomainValue::Value::set(Object^ value) {
	if (m_val != value) {
		m_val = value;
		OnPropertyChanged("Value");
	}
}
//
String^ DomainValue::Observations::get() {
	return m_desc;
}
void DomainValue::Observations::set(String^ value) {
	String^ old = ((m_desc == nullptr) || m_desc->IsEmpty()) ? "" : m_desc;
	String^ cur = StringUtils::FormatName(value);
	if (old != cur) {
		m_desc = cur;
		m_modified = true;
		OnPropertyChanged("Observations");
		OnPropertyChanged("IsModified");
	}
}
String^ DomainValue::DatasetSigle::get() {
	return m_datasetsigle;
}
void DomainValue::DatasetSigle::set(String^ value) {
	String^ old = ((m_datasetsigle == nullptr) || m_datasetsigle->IsEmpty()) ? "" : m_datasetsigle;
	String^ cur = StringUtils::ToUpperFormat(value);
	if (old != cur) {
		m_datasetsigle = cur;
		m_modified = true;
		OnPropertyChanged("DatasetSigle");
		OnPropertyChanged("IsModified");
		OnPropertyChanged("IsStoreable");
	}
}
String^ DomainValue::VariableSigle::get() {
	return m_varsigle;
}
void DomainValue::VariableSigle::set(String^ value) {
	String^ old = ((m_varsigle == nullptr) || m_varsigle->IsEmpty()) ? "" : m_varsigle;
	String^ cur = StringUtils::ToUpperFormat(value);
	if (old != cur) {
		m_varsigle = cur;
		m_modified = true;
		OnPropertyChanged("VariableSigle");
		OnPropertyChanged("IsModified");
		OnPropertyChanged("IsStoreable");
	}
}
String^ DomainValue::IndivSigle::get() {
	return m_indsigle;
}
void DomainValue::IndivSigle::set(String^ value) {
	String^ old = ((m_indsigle == nullptr) || m_indsigle->IsEmpty()) ? "" : m_indsigle;
	String^ cur = StringUtils::ToUpperFormat(value);
	if (old != cur) {
		m_indsigle = cur;
		m_modified = true;
		OnPropertyChanged("IndivSigle");
		OnPropertyChanged("IsModified");
		OnPropertyChanged("IsStoreable");
	}
}
bool DomainValue::IsModified::get() {
	return m_modified;
}
void DomainValue::IsModified::set(bool value) {
	if (m_modified != value) {
		m_modified = value;
		OnPropertyChanged("IsModified");
	}
}
bool DomainValue::IsSelected::get() {
	return m_selected;
}
bool DomainValue::IsNew::get() {
	return (!IsPersisted);
}
void DomainValue::IsSelected::set(bool value) {
	if (m_selected != value) {
		m_selected = value;
		OnPropertyChanged("IsSelected");
	}
}
bool DomainValue::IsPersisted::get() {
	return (m_id != nullptr) && (m_rev != nullptr) &&
		(!m_id->IsEmpty()) && (!m_rev->IsEmpty());
}
bool DomainValue::IsStoreable::get() {
	return (m_varsigle != nullptr) && (m_indsigle != nullptr) && (m_datasetsigle != nullptr) &&
		(!m_datasetsigle->IsEmpty()) && (m_type != InfoDataType::Unknown) &&
		(!m_varsigle->IsEmpty()) && (!m_indsigle->IsEmpty()) &&
		(m_status != InfoStatus::Unknown) && m_modified;
}
DomainValue^ DomainValue::Clone() {
	DomainValue^ p = ref new DomainValue();
	Copy(p);
	return p;
}// clone
void DomainValue::NotifyAll(void) {
	OnPropertyChanged("Id");
	OnPropertyChanged("Rev");
	OnPropertyChanged("Status");
	OnPropertyChanged("StatusString");
	OnPropertyChanged("VariableType");
	OnPropertyChanged("VariableTypeString");
	OnPropertyChanged("Observations");
	OnPropertyChanged("VariableSigle");
	OnPropertyChanged("IndivSigle");
	OnPropertyChanged("DatasetSigle");
	OnPropertyChanged("Value");
	OnPropertyChanged("IsSelected");
	OnPropertyChanged("IsPersisted");
	OnPropertyChanged("IsNew");
	OnPropertyChanged("IsModified");
	OnPropertyChanged("IsStoreable");
}// NotifyAll
void DomainValue::Clear(void) {
	m_modified = false;
	m_selected = false;
	m_status = InfoStatus::Unknown;
	m_type = InfoDataType::Unknown;
	m_id = nullptr;
	m_rev = nullptr;
	m_desc = nullptr;
	m_datasetsigle = nullptr;
	m_varsigle = nullptr;
	m_indsigle = nullptr;
	m_val = nullptr;
	NotifyAll();
}
void DomainValue::Copy(DomainValue ^p) {
	if (p != nullptr) {
		p->m_type = m_type;
		p->m_status = this->m_status;
		p->m_modified = this->m_modified;
		p->m_selected = this->m_selected;
		p->m_id = ((m_id != nullptr) && (!m_id->IsEmpty())) ? ref new String(m_id->Data()) : nullptr;
		p->m_rev = ((m_rev != nullptr) && (!m_rev->IsEmpty())) ? ref new String(m_rev->Data()) : nullptr;
		p->m_desc = ((m_desc != nullptr) && (!m_desc->IsEmpty())) ? ref new String(m_desc->Data()) : nullptr;
		p->m_datasetsigle = ((m_datasetsigle != nullptr) && (!m_datasetsigle->IsEmpty())) ? ref new String(m_datasetsigle->Data()) : nullptr;
		p->m_varsigle = ((m_varsigle != nullptr) && (!m_varsigle->IsEmpty())) ? ref new String(m_varsigle->Data()) : nullptr;
		p->m_indsigle = ((m_indsigle != nullptr) && (!m_indsigle->IsEmpty())) ? ref new String(m_indsigle->Data()) : nullptr;
		p->m_val = m_val;
		p->NotifyAll();
	}// p
}// Copy
