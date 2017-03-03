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
	public ref class DomainValue sealed : INotifyPropertyChanged
	{
	private:
		InfoDataType m_type;
		InfoStatus m_status;
		bool m_modified;
		bool m_selected;
		String^ m_id;
		String^ m_rev;
		String^ m_desc;
		String^ m_varsigle;
		String^ m_indsigle;
		String^ m_datasetsigle;
		Object^ m_val;
	private:
		void OnPropertyChanged(String^ propertyName);
	internal:
		DomainValue(IMap<String^, Object^>^ oMap);
		IMap<String^, Object^>^ GetMap(void);
	public:
		DomainValue();
		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler^ PropertyChanged;
	public:
		property InfoDataType VariableType {
			InfoDataType get();
			void set(InfoDataType value);
		}
		property String^ VariableTypeString {
			String^ get();
		}
		//
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
		property String^ VariableSigle {
			String^ get();
			void set(String^ value);
		}
		property String^ IndivSigle {
			String^ get();
			void set(String^ value);
		}
		property Object^ Value {
			Object^ get();
			void set(Object^ value);
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
		DomainValue^ Clone();
		void Clear(void);
		void NotifyAll(void);
		void Copy(DomainValue ^pDest);
	};

}// namespace GenDoaminData
