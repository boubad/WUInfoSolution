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
	public ref class DomainIndiv sealed : INotifyPropertyChanged
	{
	private:
		InfoStatus m_status;
		bool m_modified;
		bool m_selected;
		String^ m_id;
		String^ m_rev;
		String^ m_desc;
		String^ m_sigle;
		String^ m_name;
		String^ m_datasetsigle;
	private:
		void OnPropertyChanged(String^ propertyName);
	internal:
		DomainIndiv(IMap<String^, Object^>^ oMap);
		IMap<String^, Object^>^ GetMap(void);
	public:
		DomainIndiv();
		//
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
		property String^ DatasetSigle {
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
		DomainIndiv^ Clone();
		void Clear(void);
		void NotifyAll(void);
		void Copy(DomainIndiv ^pDest);
		
	};
}// namespace GenDomaindata
