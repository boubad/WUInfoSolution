#pragma once
#include "domainimpl.h"
////////////////////
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Platform::Collections;
//
namespace InfoDomain {
	public ref class Etudiant sealed
	{
	private:
		std::unique_ptr<EtudiantImpl> m_pimpl;
	public:
		Etudiant();
		Etudiant(IMap<Platform::String^, Object^>^ pMap);
		//
		property String^ Id {
			String^ get() {
				return m_pimpl->get_Id();
			}
		}// Id
		property String^ Rev {
			String^ get() {
				return m_pimpl->get_Rev();
			}
		}// rev
		property InfoStatus Status {
			InfoStatus get() {
				return m_pimpl->get_Status();
			}
			void set(InfoStatus value) {
				m_pimpl->set_Status(value);
			}
		}// Status
		property String^ Observations {
			String^ get() {
				return m_pimpl->get_Observations();
			}
			void set(String^ value) {
				m_pimpl->set_Observations(value);
			}
		}// Observations
		property String^ Type {
			String^ get() {
				return m_pimpl->get_Type();
			}
		}// Type
		property bool IsStoreable {
			bool get() {
				return m_pimpl->get_IsStoreable();
			}
		}// IsStoreable
		property bool IsPersisted {
			bool get() {
				return m_pimpl->get_IsPersisted();
			}
		}// IsPersisted
		property String^ Annee {
			String^ get() {
				return m_pimpl->get_Annee();
			}
			void set(String^ value) {
				m_pimpl->set_Annee(value);
			}
		}// name
		property String^ Dossier {
			String^ get() {
				return m_pimpl->get_Dossier();
			}
			void set(String^ value) {
				m_pimpl->set_Dossier(value);
			}
		}
		property String^ Sexe {
			String^ get() {
				return m_pimpl->get_Sexe();
			}
			void set(String^ value) {
				m_pimpl->set_Sexe(value);
			}
		}
		property String^ Birth {
			String^ get() {
				return m_pimpl->get_Birth();
			}
			void set(String^ value) {
				m_pimpl->set_Birth(value);
			}
		}
		property String^ Firstname {
			String^ get() {
				return m_pimpl->get_Firstname();
			}
			void set(String^ value) {
				m_pimpl->set_Firstname(value);
			}
		}
		property String^ Lastname {
			String^ get() {
				return m_pimpl->get_Lastname();
			}
			void set(String^ value) {
				m_pimpl->set_Lastname(value);
			}
		}
		property String^ Fullname {
			String^ get();
		}
		property String^ Departement {
			String^ get() {
				return m_pimpl->get_Departement();
			}
			void set(String^ value) {
				m_pimpl->set_Departement(value);
			}
		}
		property String^ Ville {
			String^ get() {
				return m_pimpl->get_Ville();
			}
			void set(String^ value) {
				m_pimpl->set_Ville(value);
			}
		}
		property String^ Lycee {
			String^ get() {
				return m_pimpl->get_Lycee();
			}
			void set(String^ value) {
				m_pimpl->set_Lycee(value);
			}
		}
		property String^ Groupe {
			String^ get() {
				return m_pimpl->get_Groupe();
			}
			void set(String^ value) {
				m_pimpl->set_Groupe(value);
			}
		}
		property String^ SerieBac {
			String^ get() {
				return m_pimpl->get_SerieBac();
			}
			void set(String^ value) {
				m_pimpl->set_SerieBac(value);
			}
		}
		property String^ OptionBac {
			String^ get() {
				return m_pimpl->get_OptionBac();
			}
			void set(String^ value) {
				m_pimpl->set_OptionBac(value);
			}
		}
		property String^ MentionBac {
			String^ get() {
				return m_pimpl->get_MentionBac();
			}
			void set(String^ value) {
				m_pimpl->set_MentionBac(value);
			}
		}
		property String^ EtudesSuperieures {
			String^ get() {
				return m_pimpl->get_EtudesSuperieures();
			}
			void set(String^ value) {
				m_pimpl->set_EtudesSuperieures(value);
			}
		}
		property String^ Apb {
			String^ get() {
				return m_pimpl->get_Apb();
			}
			void set(String^ value) {
				m_pimpl->set_Apb(value);
			}
		}
		property String^ Redoublant {
			String^ get() {
				return m_pimpl->get_Redoublant();
			}
			void set(String^ value) {
				m_pimpl->set_Redoublant(value);
			}
		}
		property String^ Avatar {
			String^ get() {
				return m_pimpl->get_Avatar();
			}
			void set(String^ value) {
				m_pimpl->set_Avatar(value);
			}
		}
		property String^ Email {
			String^ get() {
				return m_pimpl->get_Email();
			}
			void set(String^ value) {
				m_pimpl->set_Email(value);
			}
		}
		property IVector<String^>^ DatasetsSigles {
			IVector<String^>^ get() {
				return m_pimpl->get_DatasetsSigles();
			}
			void set(IVector<String^>^ value) {
				m_pimpl->set_DatasetsSigles(value);
			}
		}
		//
		IMap<String^, Object^>^ GetMap(void);
		virtual String^ ToString(void) override;
	};// class Etudiant
}// namespace InfoDomain
