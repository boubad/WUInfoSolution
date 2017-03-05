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
	public ref class DomainVariable sealed : INotifyPropertyChanged
	{
	private:
		InfoDataType m_type;
		InfoKind m_kind;
		InfoStatus m_status;
		bool m_modified;
		bool m_selected;
		String^ m_id;
		String^ m_rev;
		String^ m_desc;
		String^ m_sigle;
		String^ m_name;
		String^ m_datasetsigle;
		IVector<String^>^ m_modalites;
		String^ m_modalitesstring;
	private:
		void OnPropertyChanged(String^ propertyName);
	internal:
		DomainVariable(IMap<String^, Object^>^ oMap);
		IMap<String^, Object^>^ GetMap(void);
	public:
		DomainVariable();
		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler^ PropertyChanged;
	public:
		property IVector<String^>^ AllStatusStrings {
			IVector<String^>^ get();
		}
		property IVector<String^>^ AllDataTypeStrings {
			IVector<String^>^ get();
		}
		property IVector<String^>^ AllDataKindStrings{
			IVector<String^>^ get();
		}
		property InfoDataType VariableType {
			InfoDataType get();
			void set(InfoDataType value);
		}
		property String^ VariableTypeString {
			String^ get();
			void set(String^ value);
		}
		property InfoKind VariableKind {
			InfoKind get();
			void set(InfoKind value);
		}
		property String^ VariableKindString {
			String^ get();
			void set(String^ value);
		}
		property IVector<String^>^ Modalites {
			IVector<String^>^ get();
			void set(IVector<String^>^ value);
		}
		property String^ ModalitesString {
			String^ get();
			void set(String^ value);
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
		DomainVariable^ Clone();
		void Clear(void);
		void NotifyAll(void);
		void Copy(DomainVariable ^pDest);
	};
}// namespace GenDomainData

