#include "pch.h"
#include "DomainEtudiant.h"
#include "StringUtils.h"
#include "infostrings.h"
//
using namespace GenDomainData;
using namespace Platform;

DomainEtudiant::DomainEtudiant()
{
	m_modified = false;
	m_selected = false;
	m_status = InfoStatus::Unknown;
}
DomainEtudiant::DomainEtudiant(IMap<String^, Object^>^ oMap) {
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
		if (oMap->HasKey(InfoStrings::KEY_STATUS)) {
			Object^ o = oMap->Lookup(InfoStrings::KEY_STATUS);
			if (o != nullptr) {
				String^ s = o->ToString();
				int n = StringUtils::StringToInteger(s);
				m_status = static_cast<InfoStatus>(n);
			}
		}// status
		if (oMap->HasKey(InfoStrings::KEY_ANNEE)) {
			m_annee = StringUtils::ToUpperFormat(oMap->Lookup(InfoStrings::KEY_ANNEE)->ToString());
		}
		if (oMap->HasKey(InfoStrings::KEY_DOSSIER)) {
			m_dossier = StringUtils::ToUpperFormat(oMap->Lookup(InfoStrings::KEY_DOSSIER)->ToString());
		}
		if (oMap->HasKey(InfoStrings::KEY_SEXE)) {
			m_sexe = StringUtils::ToUpperFormat(oMap->Lookup(InfoStrings::KEY_SEXE)->ToString());
		}
		if (oMap->HasKey(InfoStrings::KEY_BIRTHYEAR)) {
			m_birth = StringUtils::ToUpperFormat(oMap->Lookup(InfoStrings::KEY_BIRTHYEAR)->ToString());
		}
		if (oMap->HasKey(InfoStrings::KEY_FIRSTNAME)) {
			m_firstname = StringUtils::FormatName(oMap->Lookup(InfoStrings::KEY_FIRSTNAME)->ToString());
		}
		if (oMap->HasKey(InfoStrings::KEY_LASTNAME)) {
			m_lastname = StringUtils::ToUpperFormat(oMap->Lookup(InfoStrings::KEY_LASTNAME)->ToString());
		}
		if (oMap->HasKey(InfoStrings::KEY_DEPARTEMENT)) {
			m_dep = StringUtils::ToUpperFormat(oMap->Lookup(InfoStrings::KEY_DEPARTEMENT)->ToString());
		}
		if (oMap->HasKey(InfoStrings::KEY_VILLE)) {
			m_ville = StringUtils::ToUpperFormat(oMap->Lookup(InfoStrings::KEY_VILLE)->ToString());
		}
		if (oMap->HasKey(InfoStrings::KEY_LYCEE)) {
			m_lycee = StringUtils::ToUpperFormat(oMap->Lookup(InfoStrings::KEY_LYCEE)->ToString());
		}
		if (oMap->HasKey(InfoStrings::KEY_GROUPE)) {
			m_groupe = StringUtils::ToUpperFormat(oMap->Lookup(InfoStrings::KEY_GROUPE)->ToString());
		}
		if (oMap->HasKey(InfoStrings::KEY_SERIEBAC)) {
			m_seriebac = StringUtils::ToUpperFormat(oMap->Lookup(InfoStrings::KEY_SERIEBAC)->ToString());
		}
		if (oMap->HasKey(InfoStrings::KEY_OPTIONBAC)) {
			m_optionbac = StringUtils::ToUpperFormat(oMap->Lookup(InfoStrings::KEY_OPTIONBAC)->ToString());
		}
		if (oMap->HasKey(InfoStrings::KEY_MENTIONBAC)) {
			m_mentionbac = StringUtils::ToUpperFormat(oMap->Lookup(InfoStrings::KEY_MENTIONBAC)->ToString());
		}
		if (oMap->HasKey(InfoStrings::KEY_APB)) {
			m_apb = StringUtils::ToUpperFormat(oMap->Lookup(InfoStrings::KEY_APB)->ToString());
		}
		if (oMap->HasKey(InfoStrings::KEY_SUP)) {
			m_sup = StringUtils::ToUpperFormat(oMap->Lookup(InfoStrings::KEY_SUP)->ToString());
		}
		if (oMap->HasKey(InfoStrings::KEY_REDOUBLANT)) {
			m_red = StringUtils::ToUpperFormat(oMap->Lookup(InfoStrings::KEY_REDOUBLANT)->ToString());
		}
		if (oMap->HasKey(InfoStrings::KEY_AVATAR)) {
			m_avatar = StringUtils::Trim(oMap->Lookup(InfoStrings::KEY_AVATAR)->ToString());
		}
		if (oMap->HasKey(InfoStrings::KEY_EMAIL)) {
			m_email = StringUtils::Trim(oMap->Lookup(InfoStrings::KEY_EMAIL)->ToString());
		}
		if (oMap->HasKey(InfoStrings::KEY_DATASETS)) {
			Object^ oo = oMap->Lookup(InfoStrings::KEY_DATASETS);
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
						m_sets = ref new Vector<String^>();
						auto jt = px->First();
						while (jt->HasCurrent) {
							auto p = jt->Current;
							m_sets->Append(p->Key);
							jt->MoveNext();
						}// jt
					}// px
				}// o
			}//oo
		}
	}// oMap
}// DomainEtudiant
IMap<String^, Object^>^  DomainEtudiant::GetMap(void) {
	IMap<String^, Object^>^ oMap = ref new Map<String^, Object^>();
	oMap->Insert(InfoStrings::KEY_TYPE, InfoStrings::TYPE_ETUDIANT);
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
		oMap->Insert(InfoStrings::KEY_MENTIONBAC, m_sexe);
	}
	if ((m_apb != nullptr) && (!m_apb->IsEmpty())) {
		oMap->Insert(InfoStrings::KEY_APB, m_sexe);
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
	if ((m_sup != nullptr) && (!m_sup->IsEmpty())) {
		oMap->Insert(InfoStrings::KEY_SUP, m_sup);
	}
	if ((m_sets != nullptr) && (m_sets->Size > 0)) {
		oMap->Insert(InfoStrings::KEY_DATASETS, m_sets);
	}
	return oMap;
}// GetMap
String ^ DomainEtudiant::ToString()
{
	return Fullname;
}
void DomainEtudiant::OnPropertyChanged(String^ propertyName)
{
	PropertyChanged(this, ref new PropertyChangedEventArgs(propertyName));
}
String^ DomainEtudiant::Id::get() {
	return m_id;
}
void DomainEtudiant::Id::set(String^ value) {
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
String^ DomainEtudiant::Rev::get() {
	return m_rev;
}
void DomainEtudiant::Rev::set(String^ value) {
	String^ old = ((m_rev == nullptr) || m_rev->IsEmpty()) ? "" : m_rev;
	String^ cur = StringUtils::Trim(value);
	if (old != cur) {
		m_rev = cur;
		OnPropertyChanged("Rev");
		OnPropertyChanged("IsPersisted");
		OnPropertyChanged("IsModified");
		OnPropertyChanged("IsNew");
		IsModified = true;
	}
}
InfoStatus DomainEtudiant::Status::get() {
	return m_status;
}
void DomainEtudiant::Status::set(InfoStatus value) {
	if (value != m_status) {
		m_status = value;
		OnPropertyChanged("Status");
		IsModified = true;
	}
}
String^ DomainEtudiant::StatusString::get() {
	return StringUtils::StatusToString(m_status);
}
void DomainEtudiant::StatusString::set(String^ value) {
	String^ old = StatusString;
	String^ cur = StringUtils::FormatName(value);
	if (old != cur) {
		Status = StringUtils::StringToStatus(cur);
		OnPropertyChanged("StatusString");
	}
}
IVector<String^>^ DomainEtudiant::DatasetsSigles::get() {
	if (m_sets == nullptr) {
		m_sets = ref new Vector<String^>();
	}
	return m_sets;
}
void DomainEtudiant::DatasetsSigles::set(IVector<String^>^ value) {
	m_sets = nullptr;
	if ((value == nullptr) || (value->Size < 1)) {
		OnPropertyChanged("DatasetsSigles");
		IsModified = true;
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
		m_sets = ref new Vector<String^>();
		auto jt = px->First();
		while (jt->HasCurrent) {
			auto p = jt->Current;
			m_sets->Append(p->Key);
			jt->MoveNext();
		}// jt
	}// px
	OnPropertyChanged("DatasetSigles");
	IsModified = true;
}
//
String^ DomainEtudiant::Observations::get() {
	return m_desc;
}
void DomainEtudiant::Observations::set(String^ value) {
	String^ old = ((m_desc == nullptr) || m_desc->IsEmpty()) ? "" : m_desc;
	String^ cur = StringUtils::FormatName(value);
	if (old != cur) {
		m_desc = cur;
		OnPropertyChanged("Observations");
		IsModified = true;
	}
}
String^ DomainEtudiant::PhotoUrl::get() {
	return m_photo;
}
void DomainEtudiant::PhotoUrl::set(String^ value) {
	m_photo = value;
}
String^ DomainEtudiant::Annee::get() {
	return m_annee;
}
void DomainEtudiant::Annee::set(String^ value) {
	String^ old = ((m_annee == nullptr) || m_annee->IsEmpty()) ? "" : m_annee;
	String^ cur = StringUtils::ToUpperFormat(value);
	if (old != cur) {
		m_annee = cur;
		OnPropertyChanged("Annee");
		IsModified = true;
	}
}
String^ DomainEtudiant::Dossier::get() {
	return m_dossier;
}
void DomainEtudiant::Dossier::set(String^ value) {
	String^ old = ((m_dossier == nullptr) || m_dossier->IsEmpty()) ? "" : m_dossier;
	String^ cur = StringUtils::ToUpperFormat(value);
	if (old != cur) {
		m_dossier = cur;
		OnPropertyChanged("Dossier");
		IsModified = true;
	}
}
String^ DomainEtudiant::Sexe::get() {
	if ((m_sexe == nullptr) || m_sexe->IsEmpty()) {
		m_sexe = "M";
	}
	return m_sexe;
}
void DomainEtudiant::Sexe::set(String^ value) {
	String^ old = ((m_sexe == nullptr) || m_sexe->IsEmpty()) ? "" : m_sexe;
	String^ cur = StringUtils::ToUpperFormat(value);
	if (old != cur) {
		m_sexe = cur;
		OnPropertyChanged("Sexe");
		OnPropertyChanged("IsMale");
		OnPropertyChanged("IsFemale");
		IsModified = true;
	}
}
bool DomainEtudiant::IsMale::get() 
{
	return (Sexe == "M");
}
void DomainEtudiant::IsMale::set(bool value) {
	Sexe = (value) ? "M" : "F";
}
bool DomainEtudiant::IsFemale::get()
{
	return (Sexe != "M");
}
void DomainEtudiant::IsFemale::set(bool value) {
	Sexe = (value) ? "F" : "M";
}
String^ DomainEtudiant::Birth::get() {
	return m_birth;
}
void DomainEtudiant::Birth::set(String^ value) {
	String^ old = ((m_birth == nullptr) || m_birth->IsEmpty()) ? "" : m_birth;
	String^ cur = StringUtils::ToUpperFormat(value);
	if (old != cur) {
		m_birth = cur;
		OnPropertyChanged("Birth");
		IsModified = true;
	}
}
String^ DomainEtudiant::Firstname::get() {
	return m_firstname;
}
void DomainEtudiant::Firstname::set(String^ value) {
	String^ old = ((m_firstname == nullptr) || m_firstname->IsEmpty()) ? "" : m_firstname;
	String^ cur = StringUtils::FormatName(value);
	if (old != cur) {
		m_firstname = cur;
		OnPropertyChanged("Firstname");
		OnPropertyChanged("Fullname");
		IsModified = true;
	}
}
String^ DomainEtudiant::Lastname::get() {
	return m_lastname;
}
void DomainEtudiant::Lastname::set(String^ value) {
	String^ old = ((m_lastname == nullptr) || m_lastname->IsEmpty()) ? "" : m_lastname;
	String^ cur = StringUtils::ToUpperFormat(value);
	if (old != cur) {
		m_lastname = cur;
		OnPropertyChanged("Lastname");
		OnPropertyChanged("Fullname");
		IsModified = true;
	}
}
String^ DomainEtudiant::Fullname::get() {
	String^ s1 = (Lastname == nullptr) ? "" : Lastname;
	String^ s2 = (Firstname == nullptr) ? "" : Firstname;
	return StringUtils::Trim(s1 + " " + s2);
}
String^ DomainEtudiant::Departement::get() {
	return m_dep;
}
void DomainEtudiant::Departement::set(String^ value) {
	String^ old = ((m_dep == nullptr) || m_dep->IsEmpty()) ? "" : m_dep;
	String^ cur = StringUtils::ToUpperFormat(value);
	if (old != cur) {
		m_dep = cur;
		OnPropertyChanged("Departement");
		IsModified = true;
	}
}
String^ DomainEtudiant::Ville::get() {
	return m_ville;
}
void DomainEtudiant::Ville::set(String^ value) {
	String^ old = ((m_ville == nullptr) || m_ville->IsEmpty()) ? "" : m_ville;
	String^ cur = StringUtils::ToUpperFormat(value);
	if (old != cur) {
		m_ville = cur;
		OnPropertyChanged("Ville");
		IsModified = true;
	}
}
String^ DomainEtudiant::Lycee::get() {
	return m_lycee;
}
void DomainEtudiant::Lycee::set(String^ value) {
	String^ old = ((m_lycee == nullptr) || m_lycee->IsEmpty()) ? "" : m_lycee;
	String^ cur = StringUtils::ToUpperFormat(value);
	if (old != cur) {
		m_lycee = cur;
		OnPropertyChanged("Lycee");
		IsModified = true;
	}
}
String^ DomainEtudiant::Groupe::get() {
	return m_groupe;
}
void DomainEtudiant::Groupe::set(String^ value) {
	String^ old = ((m_groupe == nullptr) || m_groupe->IsEmpty()) ? "" : m_groupe;
	String^ cur = StringUtils::ToUpperFormat(value);
	if (old != cur) {
		m_groupe = cur;
		OnPropertyChanged("Groupe");
		IsModified = true;
	}
}
String^ DomainEtudiant::SerieBac::get() {
	return m_seriebac;
}
void DomainEtudiant::SerieBac::set(String^ value) {
	String^ old = ((m_seriebac == nullptr) || m_seriebac->IsEmpty()) ? "" : m_seriebac;
	String^ cur = StringUtils::ToUpperFormat(value);
	if (old != cur) {
		m_seriebac = cur;
		OnPropertyChanged("SerieBac");
		IsModified = true;
	}
}
String^ DomainEtudiant::OptionBac::get() {
	return m_optionbac;
}
void DomainEtudiant::OptionBac::set(String^ value) {
	String^ old = ((m_optionbac == nullptr) || m_optionbac->IsEmpty()) ? "" : m_optionbac;
	String^ cur = StringUtils::ToUpperFormat(value);
	if (old != cur) {
		m_optionbac = cur;
		OnPropertyChanged("OptionBac");
		IsModified = true;
	}
}
String^ DomainEtudiant::MentionBac::get() {
	return m_mentionbac;
}
void DomainEtudiant::MentionBac::set(String^ value) {
	String^ old = ((m_mentionbac == nullptr) || m_mentionbac->IsEmpty()) ? "" : m_mentionbac;
	String^ cur = StringUtils::ToUpperFormat(value);
	if (old != cur) {
		m_mentionbac = cur;
		OnPropertyChanged("MentionBac");
		IsModified = true;
	}
}
String^ DomainEtudiant::EtudesSuperieures::get() {
	return m_sup;
}
void DomainEtudiant::EtudesSuperieures::set(String^ value) {
	String^ old = ((m_sup == nullptr) || m_sup->IsEmpty()) ? "" : m_sup;
	String^ cur = StringUtils::ToUpperFormat(value);
	if (old != cur) {
		m_sup = cur;
		OnPropertyChanged("EtudesSuperieures");
		IsModified = true;
	}
}
String^ DomainEtudiant::Apb::get() {
	return m_apb;
}
void DomainEtudiant::Apb::set(String^ value) {
	String^ old = ((m_apb == nullptr) || m_apb->IsEmpty()) ? "" : m_apb;
	String^ cur = StringUtils::ToUpperFormat(value);
	if (old != cur) {
		m_apb = cur;
		OnPropertyChanged("Apb");
		IsModified = true;
	}
}
String^ DomainEtudiant::Redoublant::get() {
	return m_red;
}
void DomainEtudiant::Redoublant::set(String^ value) {
	String^ old = ((m_red == nullptr) || m_red->IsEmpty()) ? "" : m_red;
	String^ cur = StringUtils::ToUpperFormat(value);
	if (old != cur) {
		m_red = cur;
		OnPropertyChanged("Redoublant");
		IsModified = true;
	}
}
String^ DomainEtudiant::Avatar::get() {
	return m_avatar;
}
void DomainEtudiant::Avatar::set(String^ value) {
	String^ old = ((m_avatar == nullptr) || m_avatar->IsEmpty()) ? "" : m_avatar;
	String^ cur = StringUtils::Trim(value);
	if (old != cur) {
		m_avatar = cur;
		OnPropertyChanged("Avatar");
		IsModified = true;
	}
}
String^ DomainEtudiant::Email::get() {
	return m_email;
}
void DomainEtudiant::Email::set(String^ value) {
	String^ old = ((m_email == nullptr) || m_email->IsEmpty()) ? "" : m_email;
	String^ cur = StringUtils::Trim(value);
	if (old != cur) {
		m_email = cur;
		OnPropertyChanged("Email");
		IsModified = true;
	}
}
//
bool DomainEtudiant::IsModified::get() {
	return m_modified;
}
bool DomainEtudiant::IsNew::get() {
	return (!IsPersisted);
}
void DomainEtudiant::IsModified::set(bool value) {
	if (m_modified != value) {
		m_modified = value;
		OnPropertyChanged("IsModified");
		OnPropertyChanged("IsStoreable");
	}
}
bool DomainEtudiant::IsSelected::get() {
	return m_selected;
}
void DomainEtudiant::IsSelected::set(bool value) {
	if (m_selected != value) {
		m_selected = value;
		OnPropertyChanged("IsSelected");
	}
}
bool DomainEtudiant::IsPersisted::get() {
	return (m_id != nullptr) && (m_rev != nullptr) &&
		(!m_id->IsEmpty()) && (!m_rev->IsEmpty());
}
bool DomainEtudiant::IsStoreable::get() {
	return (m_dossier != nullptr) && (m_firstname != nullptr) && (m_lastname != nullptr) &&
		(!m_dossier->IsEmpty()) &&
		(!m_firstname->IsEmpty()) && (!m_lastname->IsEmpty()) &&
		(m_status != InfoStatus::Unknown) && m_modified;
}
DomainEtudiant^ DomainEtudiant::Clone() {
	DomainEtudiant^ p = ref new DomainEtudiant();
	Copy(p);
	return p;
}// clone
void DomainEtudiant::NotifyAll(void) {
	OnPropertyChanged("Id");
	OnPropertyChanged("Rev");
	OnPropertyChanged("Status");
	OnPropertyChanged("StatusString");
	OnPropertyChanged("Observations");
	OnPropertyChanged("PhotoUrl");
	OnPropertyChanged("Annee");
	OnPropertyChanged("Dossier");
	OnPropertyChanged("Sexe");
	OnPropertyChanged("IsMale");
	OnPropertyChanged("IsFemale");
	OnPropertyChanged("Birth");
	OnPropertyChanged("Firstname");
	OnPropertyChanged("Lastname");
	OnPropertyChanged("Fullname");
	OnPropertyChanged("Departement");
	OnPropertyChanged("Ville");
	OnPropertyChanged("Lycee");
	OnPropertyChanged("Groupe");
	OnPropertyChanged("SerieBac");
	OnPropertyChanged("OptionBac");
	OnPropertyChanged("MentionBac");
	OnPropertyChanged("EtudesSuperieures");
	OnPropertyChanged("Apb");
	OnPropertyChanged("Redoublant");
	OnPropertyChanged("Avatar");
	OnPropertyChanged("Email");
	OnPropertyChanged("DatasetsSigles");
	OnPropertyChanged("IsSelected");
	OnPropertyChanged("IsPersisted");
	OnPropertyChanged("IsModified");
	OnPropertyChanged("IsStoreable");
}// NotifyAll
void DomainEtudiant::Clear(void) {
	m_modified = false;
	m_selected = false;
	m_status = InfoStatus::Unknown;
	m_id = nullptr;
	m_rev = nullptr;
	m_desc = nullptr;
	m_annee = nullptr;
	m_dossier = nullptr;
	m_sexe = nullptr;
	m_birth = nullptr;
	m_firstname = nullptr;
	m_lastname = nullptr;
	m_dep = nullptr;
	m_ville = nullptr;
	m_lycee = nullptr;
	m_groupe = nullptr;
	m_seriebac = nullptr;
	m_optionbac = nullptr;
	m_optionbac = nullptr;
	m_apb = nullptr;
	m_sup = nullptr;
	m_red = nullptr;
	m_avatar = nullptr;
	m_email = nullptr;
	m_photo = nullptr;
	m_sets = nullptr;
	NotifyAll();
}
void DomainEtudiant::Copy(DomainEtudiant ^p) {
	if (p != nullptr) {
		p->m_status = this->m_status;
		p->m_modified = this->m_modified;
		p->m_selected = this->m_selected;
		p->m_id = ((m_id != nullptr) && (!m_id->IsEmpty())) ? ref new String(m_id->Data()) : nullptr;
		p->m_rev = ((m_rev != nullptr) && (!m_rev->IsEmpty())) ? ref new String(m_rev->Data()) : nullptr;
		p->m_desc = ((m_desc != nullptr) && (!m_desc->IsEmpty())) ? ref new String(m_desc->Data()) : nullptr;
		p->m_annee = ((m_annee != nullptr) && (!m_annee->IsEmpty())) ? ref new String(m_annee->Data()) : nullptr;
		p->m_dossier = ((m_dossier != nullptr) && (!m_dossier->IsEmpty())) ? ref new String(m_dossier->Data()) : nullptr;
		p->m_sexe = ((m_sexe != nullptr) && (!m_sexe->IsEmpty())) ? ref new String(m_sexe->Data()) : nullptr;
		p->m_birth = ((m_birth != nullptr) && (!m_birth->IsEmpty())) ? ref new String(m_birth->Data()) : nullptr;
		p->m_firstname = ((m_firstname != nullptr) && (!m_firstname->IsEmpty())) ? ref new String(m_firstname->Data()) : nullptr;
		p->m_lastname = ((m_lastname != nullptr) && (!m_lastname->IsEmpty())) ? ref new String(m_lastname->Data()) : nullptr;
		p->m_dep = ((m_dep != nullptr) && (!m_dep->IsEmpty())) ? ref new String(m_dep->Data()) : nullptr;
		p->m_ville = ((m_ville != nullptr) && (!m_ville->IsEmpty())) ? ref new String(m_ville->Data()) : nullptr;
		p->m_lycee = ((m_lycee != nullptr) && (!m_lycee->IsEmpty())) ? ref new String(m_lycee->Data()) : nullptr;
		p->m_groupe = ((m_groupe != nullptr) && (!m_groupe->IsEmpty())) ? ref new String(m_groupe->Data()) : nullptr;
		p->m_seriebac = ((m_seriebac != nullptr) && (!m_seriebac->IsEmpty())) ? ref new String(m_seriebac->Data()) : nullptr;
		p->m_optionbac = ((m_optionbac != nullptr) && (!m_optionbac->IsEmpty())) ? ref new String(m_optionbac->Data()) : nullptr;
		p->m_mentionbac = ((m_mentionbac != nullptr) && (!m_mentionbac->IsEmpty())) ? ref new String(m_mentionbac->Data()) : nullptr;
		p->m_apb = ((m_apb != nullptr) && (!m_apb->IsEmpty())) ? ref new String(m_apb->Data()) : nullptr;
		p->m_sup = ((m_sup != nullptr) && (!m_sup->IsEmpty())) ? ref new String(m_sup->Data()) : nullptr;
		p->m_red = ((m_red != nullptr) && (!m_red->IsEmpty())) ? ref new String(m_red->Data()) : nullptr;
		p->m_avatar = ((m_avatar != nullptr) && (!m_avatar->IsEmpty())) ? ref new String(m_avatar->Data()) : nullptr;
		p->m_email = ((m_email != nullptr) && (!m_email->IsEmpty())) ? ref new String(m_email->Data()) : nullptr;
		p->m_photo = ((m_photo != nullptr) && (!m_photo->IsEmpty())) ? ref new String(m_photo->Data()) : nullptr;
		p->DatasetsSigles = m_sets;
		p->m_modified = false;
		p->NotifyAll();
	}// p
}// Copy
void DomainEtudiant::AddDatasetSigle(String^ s) {
	String^ ss = StringUtils::ToUpperFormat(s);
	if ((ss == nullptr) || ss->IsEmpty()) {
		return;
	}
	if (m_sets == nullptr) {
		m_sets = ref new Vector<String^>();
	}
	else {
		auto it = m_sets->First();
		while (it->HasCurrent) {
			String^ x = it->Current;
			if (x == ss) {
				return;
			}
			it->MoveNext();
		}// it
	}
	m_sets->Append(ss);
	m_modified = true;
	OnPropertyChanged("IsModified");
	OnPropertyChanged("DatasetsSigles");
	OnPropertyChanged("IsStoreable");
}//AddDatasetSigle
IVector<String^>^ DomainEtudiant::AllStatusStrings::get()
{
	return StringUtils::AllStatusStrings();
}
