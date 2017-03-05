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
	[Windows::Foundation::Metadata::WebHostHidden]
	[Windows::UI::Xaml::Data::Bindable]
	public ref class DomainDataset sealed : INotifyPropertyChanged
    {
	private:
		InfoStatus m_status;
		bool m_modified;
		bool m_selected;
		String^ m_id;
		String^ m_rev;
		String^ m_desc;
		String^ m_annee;
		String^ m_sigle;
		String^ m_name;
	private:
		void OnPropertyChanged(String^ propertyName);
	internal:
		DomainDataset(IMap<String^, Object^>^ oMap);
		IMap<String^, Object^>^ GetMap(void);
    public:
        DomainDataset();
		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler^ PropertyChanged;
	public:
		property IVector<String^>^ AllStatusStrings {
			IVector<String^>^ get();
		}
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
		property String^ Annee {
			String^ get();
			void set(String^ value);
		}
		property String^ Sigle {
			String^ get();
			void set(String^ value);
		}
		property String^ Name {
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
		virtual String^ ToString() override;
		DomainDataset^ Clone();
		void Clear(void);
		void NotifyAll(void);
		void Copy(DomainDataset ^pDest);
    };// class DomainDataset
}// namespace GenDomainData
