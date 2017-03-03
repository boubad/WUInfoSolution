#include "pch.h"
#include "DomainVariable.h"
#include "StringUtils.h"
#include "infostrings.h"
//
using namespace GenDomainData;
using namespace Platform;


DomainVariable::DomainVariable()
{
	m_modified = false;
	m_selected = false;
	m_status = InfoStatus::Unknown;
	m_type = InfoDataType::Unknown;
	m_kind = InfoKind::Unknown;
}
DomainVariable::DomainVariable(IMap<String^, Object^>^ oMap) {
	m_modified = false;
	m_selected = false;
	m_status = InfoStatus::Unknown;
	m_type = InfoDataType::Unknown;
	m_kind = InfoKind::Unknown;
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
		if (oMap->HasKey(InfoStrings::KEY_DATATYPE)) {
			Object^ o = oMap->Lookup(InfoStrings::KEY_DATATYPE);
			if (o != nullptr) {
				String^ s = o->ToString();
				int n = StringUtils::StringToInteger(s);
				m_type = static_cast<InfoDataType>(n);
			}
		}// type
		if (oMap->HasKey(InfoStrings::KEY_VARIABLEKIND)) {
			Object^ o = oMap->Lookup(InfoStrings::KEY_VARIABLEKIND);
			if (o != nullptr) {
				String^ s = o->ToString();
				int n = StringUtils::StringToInteger(s);
				m_kind = static_cast<InfoKind>(n);
			}
		}// type
		if (oMap->HasKey(InfoStrings::KEY_DATASETSIGLE)) {
			m_datasetsigle = oMap->Lookup(InfoStrings::KEY_DATASETSIGLE)->ToString();
		}
		if (oMap->HasKey(InfoStrings::KEY_MODALITES)) {
			Object^ oo = oMap->Lookup(InfoStrings::KEY_MODALITES);
			if (oo != nullptr) {
				IVector<Object^>^ o = dynamic_cast<IVector<Object^>^>(oo);
				if ((o != nullptr) && (o->Size > 0)) {
					IMap<String^, String^>^ px = ref new Map<String^, String^>();
					auto it = o->First();
					while (it->HasCurrent) {
						if (it->Current != nullptr) {
							String^ s = StringUtils::ToUpperFormat(it->Current->ToString());
							if ((s != nullptr) && (!s->IsEmpty())) {
								px->Insert(s, s);
							}
						}
						it->MoveNext();
					}// it
					if (px->Size > 0) {
						m_modalites = ref new Vector<String^>();
						auto jt = px->First();
						while (jt->HasCurrent) {
							auto p = jt->Current;
							m_modalites->Append(p->Key);
							jt->MoveNext();
						}// jt
					}// px
				}// o
			}//oo
		}
	}// oMap
}// DomainVariable
IMap<String^, Object^>^  DomainVariable::GetMap(void) {
	IMap<String^, Object^>^ oMap = ref new Map<String^, Object^>();
	oMap->Insert(InfoStrings::KEY_TYPE, InfoStrings::TYPE_VARIABLE);
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
	if (m_type != InfoDataType::Unknown) {
		int ival = static_cast<int>(m_type);
		oMap->Insert(InfoStrings::KEY_DATATYPE, ival);
	}
	if (m_kind != InfoKind::Unknown) {
		int ival = static_cast<int>(m_kind);
		oMap->Insert(InfoStrings::KEY_VARIABLEKIND, ival);
	}
	if ((m_modalites != nullptr) && (m_modalites->Size > 0)) {
		oMap->Insert(InfoStrings::KEY_MODALITES, m_modalites);
	}
	return oMap;
}// GetMap
String ^ DomainVariable::ToString()
{
	return Sigle;
}
void DomainVariable::OnPropertyChanged(String^ propertyName)
{
	PropertyChanged(this, ref new PropertyChangedEventArgs(propertyName));
}
String^ DomainVariable::Id::get() {
	return m_id;
}
void DomainVariable::Id::set(String^ value) {
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
String^ DomainVariable::Rev::get() {
	return m_rev;
}
void DomainVariable::Rev::set(String^ value) {
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
InfoStatus DomainVariable::Status::get() {
	return m_status;
}
void DomainVariable::Status::set(InfoStatus value) {
	if (value != m_status) {
		m_status = value;
		m_modified = true;
		OnPropertyChanged("Status");
		OnPropertyChanged("IsModified");
		OnPropertyChanged("IsStoreable");
	}
}
String^ DomainVariable::StatusString::get() {
	return StringUtils::StatusToString(m_status);
}
void DomainVariable::StatusString::set(String^ value) {
	String^ old = StatusString;
	String^ cur = StringUtils::FormatName(value);
	if (old != cur) {
		Status = StringUtils::StringToStatus(cur);
		OnPropertyChanged("StatusString");
	}
}
//
InfoDataType DomainVariable::VariableType::get() {
	return m_type;
}
void DomainVariable::VariableType::set(InfoDataType value) {
	if (value != m_type) {
		m_type = value;
		m_modified = true;
		OnPropertyChanged("VariableType");
		OnPropertyChanged("IsModified");
		OnPropertyChanged("IsStoreable");
	}
}
String^ DomainVariable::VariableTypeString::get() {
	return StringUtils::DataTypeToString(m_type);
}
void DomainVariable::VariableTypeString::set(String^ value) {
	String^ old = VariableTypeString;
	String^ cur = StringUtils::FormatName(value);
	if (old != cur) {
		VariableType = StringUtils::StringToDataType(cur);
		OnPropertyChanged("VariableTypeString");
	}
}
//
InfoKind DomainVariable::VariableKind::get() {
	return m_kind;
}
void DomainVariable::VariableKind::set(InfoKind value) {
	if (value != m_kind) {
		m_kind = value;
		m_modified = true;
		OnPropertyChanged("VariableKind");
		OnPropertyChanged("IsModified");
		OnPropertyChanged("IsStoreable");
	}
}
String^ DomainVariable::VariableKindString::get() {
	return StringUtils::DataKindToString(m_kind);
}
void DomainVariable::VariableKindString::set(String^ value) {
	String^ old = VariableKindString;
	String^ cur = StringUtils::FormatName(value);
	if (old != cur) {
		VariableKind = StringUtils::StringToDataKind(cur);
		OnPropertyChanged("VariableKindString");
	}
}
//
IVector<String^>^ DomainVariable::Modalites::get() {
	if (m_modalites == nullptr) {
		m_modalites = ref new Vector<String^>();
	}
	return m_modalites;
}
void DomainVariable::Modalites::set(IVector<String^>^ value) {
	m_modalites = nullptr;
	if ((value == nullptr) || (value->Size < 1)) {
		OnPropertyChanged("Modalites");
		return;
	}
	IMap<String^, String^>^ px = ref new Map<String^, String^>();
	auto it = value->First();
	while (it->HasCurrent) {
		if (it->Current != nullptr) {
			String^ s = StringUtils::ToUpperFormat(it->Current->ToString());
			if ((s != nullptr) && (!s->IsEmpty())) {
				px->Insert(s, s);
			}
		}
		it->MoveNext();
	}// it
	if (px->Size > 0) {
		m_modalites = ref new Vector<String^>();
		auto jt = px->First();
		while (jt->HasCurrent) {
			auto p = jt->Current;
			m_modalites->Append(p->Key);
			jt->MoveNext();
		}// jt
	}// px
	OnPropertyChanged("Modalites");
}
//
String^ DomainVariable::Observations::get() {
	return m_desc;
}
void DomainVariable::Observations::set(String^ value) {
	String^ old = ((m_desc == nullptr) || m_desc->IsEmpty()) ? "" : m_desc;
	String^ cur = StringUtils::FormatName(value);
	if (old != cur) {
		m_desc = cur;
		m_modified = true;
		OnPropertyChanged("Observations");
		OnPropertyChanged("IsModified");
	}
}
String^ DomainVariable::DatasetSigle::get() {
	return m_datasetsigle;
}
void DomainVariable::DatasetSigle::set(String^ value) {
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
String^ DomainVariable::Sigle::get() {
	return m_sigle;
}
void DomainVariable::Sigle::set(String^ value) {
	String^ old = ((m_sigle == nullptr) || m_sigle->IsEmpty()) ? "" : m_sigle;
	String^ cur = StringUtils::ToUpperFormat(value);
	if (old != cur) {
		m_sigle = cur;
		m_modified = true;
		OnPropertyChanged("Sigle");
		OnPropertyChanged("IsModified");
		OnPropertyChanged("IsStoreable");
	}
}
String^ DomainVariable::Name::get() {
	return m_name;
}
void DomainVariable::Name::set(String^ value) {
	String^ old = ((m_name == nullptr) || m_name->IsEmpty()) ? "" : m_name;
	String^ cur = StringUtils::ToUpperFormat(value);
	if (old != cur) {
		m_name = cur;
		m_modified = true;
		OnPropertyChanged("Name");
		OnPropertyChanged("IsModified");
		OnPropertyChanged("IsStoreable");
	}
}
bool DomainVariable::IsModified::get() {
	return m_modified;
}
void DomainVariable::IsModified::set(bool value) {
	if (m_modified != value) {
		m_modified = value;
		OnPropertyChanged("IsModified");
	}
}
bool DomainVariable::IsSelected::get() {
	return m_selected;
}
void DomainVariable::IsSelected::set(bool value) {
	if (m_selected != value) {
		m_selected = value;
		OnPropertyChanged("IsSelected");
	}
}
bool DomainVariable::IsPersisted::get() {
	return (m_id != nullptr) && (m_rev != nullptr) &&
		(!m_id->IsEmpty()) && (!m_rev->IsEmpty());
}
bool DomainVariable::IsNew::get() {
	return (!IsPersisted);
}
bool DomainVariable::IsStoreable::get() {
	return (m_sigle != nullptr) && (m_name != nullptr) && (m_datasetsigle != nullptr) &&
		(!m_datasetsigle->IsEmpty()) && (m_type != InfoDataType::Unknown) &&
		(m_kind != InfoKind::Unknown) &&
		(!m_sigle->IsEmpty()) && (!m_name->IsEmpty()) &&
		(m_status != InfoStatus::Unknown) && m_modified;
}
DomainVariable^ DomainVariable::Clone() {
	DomainVariable^ p = ref new DomainVariable();
	Copy(p);
	return p;
}// clone
void DomainVariable::NotifyAll(void) {
	OnPropertyChanged("Id");
	OnPropertyChanged("Rev");
	OnPropertyChanged("Status");
	OnPropertyChanged("StatusString");
	OnPropertyChanged("VariableType");
	OnPropertyChanged("VariableTypeString");
	OnPropertyChanged("VariableKind");
	OnPropertyChanged("VariableKindString");
	OnPropertyChanged("Observations");
	OnPropertyChanged("Sigle");
	OnPropertyChanged("Name");
	OnPropertyChanged("DatasetSigle");
	OnPropertyChanged("Modalites");
	OnPropertyChanged("IsSelected");
	OnPropertyChanged("IsPersisted");
	OnPropertyChanged("IsNew");
	OnPropertyChanged("IsModified");
	OnPropertyChanged("IsStoreable");
}// NotifyAll
void DomainVariable::Clear(void) {
	m_modified = false;
	m_selected = false;
	m_status = InfoStatus::Unknown;
	m_type = InfoDataType::Unknown;
	m_kind = InfoKind::Unknown;
	m_id = nullptr;
	m_rev = nullptr;
	m_desc = nullptr;
	m_datasetsigle = nullptr;
	m_sigle = nullptr;
	m_name = nullptr;
	m_modalites = nullptr;
	NotifyAll();
}
void DomainVariable::Copy(DomainVariable ^p) {
	if (p != nullptr) {
		p->m_type = m_type;
		p->m_kind = m_kind;
		p->m_status = this->m_status;
		p->m_modified = this->m_modified;
		p->m_selected = this->m_selected;
		p->m_id = ((m_id != nullptr) && (!m_id->IsEmpty())) ? ref new String(m_id->Data()) : nullptr;
		p->m_rev = ((m_rev != nullptr) && (!m_rev->IsEmpty())) ? ref new String(m_rev->Data()) : nullptr;
		p->m_desc = ((m_desc != nullptr) && (!m_desc->IsEmpty())) ? ref new String(m_desc->Data()) : nullptr;
		p->m_datasetsigle = ((m_datasetsigle != nullptr) && (!m_datasetsigle->IsEmpty())) ? ref new String(m_datasetsigle->Data()) : nullptr;
		p->m_sigle = ((m_sigle != nullptr) && (!m_sigle->IsEmpty())) ? ref new String(m_sigle->Data()) : nullptr;
		p->m_name = ((m_name != nullptr) && (!m_name->IsEmpty())) ? ref new String(m_name->Data()) : nullptr;
		p->Modalites = m_modalites;
		p->m_modified = false;
		p->NotifyAll();
	}// p
}// Copy
