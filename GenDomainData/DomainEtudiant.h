#pragma once
//////////////////////
#include "Common.h"
/////////////////////////
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml::Data;
//
namespace GenDomainData
{
	//
	[Windows::Foundation::Metadata::WebHostHidden]
	[Windows::UI::Xaml::Data::Bindable]
	public ref class DomainEtudiant sealed : INotifyPropertyChanged
	{
	private:
		InfoStatus m_status;
		bool m_modified;
		bool m_selected;
		String^ m_id;
		String^ m_rev;
		String^ m_desc;
		String^ m_annee;
		String^ m_dossier;
		String^ m_sexe;
		String^ m_birth;
		String^ m_firstname;
		String^ m_lastname;
		String^ m_dep;
		String^ m_ville;
		String^ m_lycee;
		String^ m_groupe;
		String^ m_seriebac;
		String^ m_optionbac;
		String^ m_mentionbac;
		String^ m_apb;
		String^ m_sup;
		String^ m_red;
		String^ m_avatar;
		String^ m_email;
		String^ m_photo;
		IVector<String^>^ m_sets;
	private:
		void OnPropertyChanged(String^ propertyName);
	internal:
		DomainEtudiant(IMap<String^, Object^>^ oMap);
		IMap<String^, Object^>^ GetMap(void);
	public:
		DomainEtudiant();
		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler^ PropertyChanged;
	public:
		property String^ Id {
			String^ get();
			void set(String^ value);
		}
		property String^ Rev {
			String^ get();
			void set(String^ value);
		}
		property InfoStatus Status {
			InfoStatus get();
			void set(InfoStatus value);
		}
		property String^ StatusString {
			String^ get();
			void set(String^ value);
		}
		property String^ Observations {
			String^ get();
			void set(String^ value);
		}
		property bool IsModified {
			bool get();
			void set(bool value);
		}
		property bool IsSelected {
			bool get();
			void set(bool value);
		}
		property bool IsPersisted {
			bool get();
		}
		property bool IsNew {
			bool get();
		}
		property bool IsStoreable {
			bool get();
		}
		//
		property String^ Sexe {
			String^ get();
			void set(String^ value);
		}
		property bool IsMale {
			bool get();
			void set(bool b);
		}
		property bool IsFemale {
			bool get();
			void set(bool b);
		}
		property String^ PhotoUrl {
			String^ get();
			void set(String^ value);
		}// name
		property String^ Annee {
			String^ get();
			void set(String^ value);
		}// name
		property String^ Dossier {
			String^ get();
			void set(String^ value);
		}
		property String^ Birth {
			String^ get();
			void set(String^ value);
		}
		property String^ Firstname {
			String^ get();
			void set(String^ value);
		}
		property String^ Lastname {
			String^ get();
			void set(String^ value);
		}
		property String^ Fullname {
			String^ get();
		}
		property String^ Departement {
			String^ get();
			void set(String^ value);
		}
		property String^ Ville {
			String^ get();
			void set(String^ value);
		}
		property String^ Lycee {
			String^ get();
			void set(String^ value);
		}
		property String^ Groupe {
			String^ get();
			void set(String^ value);
		}
		property String^ SerieBac {
			String^ get();
			void set(String^ value);
		}
		property String^ OptionBac {
			String^ get();
			void set(String^ value);
		}
		property String^ MentionBac {
			String^ get();
			void set(String^ value);
		}
		property String^ EtudesSuperieures {
			String^ get();
			void set(String^ value);
		}
		property String^ Apb {
			String^ get();
			void set(String^ value);
		}
		property String^ Redoublant {
			String^ get();
			void set(String^ value);
		}
		property String^ Avatar {
			String^ get();
			void set(String^ value);
		}
		property String^ Email {
			String^ get();
			void set(String^ value);
		}
		property IVector<String^>^ DatasetsSigles {
			IVector<String^>^ get();
			void set(IVector<String^>^ value);
		}
		//
		virtual String^ ToString() override;
		DomainEtudiant^ Clone();
		void Clear(void);
		void NotifyAll(void);
		void Copy(DomainEtudiant ^pDest);
		void AddDatasetSigle(String^ s);
	};
}// namespace GenDomainData

