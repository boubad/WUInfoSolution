#pragma once
#include "domainimpl.h"
#include "infostrings.h"

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Platform::Collections;
using namespace Platform::Metadata;
//
namespace InfoDomain {
	//
	ref class Dataset;
	ref class Indiv;
	ref class Variable;
	ref class InfoValue;
	//
	public ref class InfoBlob sealed {
	private:
		String^ m_name;
		String^ m_mime;
		IVector<uint8>^ m_data;
	public:
		InfoBlob();
		InfoBlob(String^ name, String^ mime, IVector<uint8>^ data);
		//
		property String^ Name {
			String^ get();
			void set(String^ value);
		}// Name
		property String^ MimeType {
			String^ get();
			void set(String^ value);
		}// MimeType
		property IVector<uint8>^ Data{
			IVector<uint8>^ get();
			void set(IVector<uint8>^ value);
		}// Data
		property String^ InfoType {
			String^ get() {
				return  InfoStrings::TYPE_BLOB;
			}
		}
	};// class InfoBlob
	//
	public ref class InfoDataValue sealed {
	private:
		InfoDataValueImpl m_impl;
	public:
		InfoDataValue();
		InfoDataValue(bool b);
		InfoDataValue(int b);
		InfoDataValue(double b);
		[Windows::Foundation::Metadata::DefaultOverloadAttribute]
		InfoDataValue(String^ b);
		//
		property InfoDataType DataType {
			InfoDataType get();
		}
		property bool HasValue {
			bool get();
		}// HasValue
		property bool IsEmpty {
			bool get() {
				return (!m_impl.get_HasValue());
			}
		}
		property bool IsBoolean {
			bool get() {
				return m_impl.get_IsBoolean();
			}
		}
		property bool IsInteger {
			bool get() {
				return m_impl.get_IsInteger();
			}
		}
		property bool IsDouble {
			bool get() {
				return m_impl.get_IsDouble();
			}
		}
		property bool IsString {
			bool get() {
				return m_impl.get_IsString();
			}
		}
		property bool IsNumber {
			bool get() {
				return m_impl.get_IsNumber();
			}
		}
		property bool BoolValue {
			bool get();
			void set(bool b);
		}// BoolValue
		property int IntValue {
			int get();
			void set(int b);
		}// IntValue
		property double DoubleValue {
			double get();
			void set(double b);
		}// DoubleValue
		property String^ StringValue {
			String^ get();
			void set(String^ b);
		}// StringValue
		property Object^ Value {
			Object^ get();
		}// Value
		void Clear(void);
		virtual String^ ToString(void) override;
	};// class InfoDataValue
	  ///////////////////////////////
	public ref class Dataset sealed {
	private:
		std::unique_ptr<DatasetImpl> m_pimpl;
		IVector<Indiv^>^ m_inds;
		IVector<Variable^>^ m_vars;
		IVector<InfoBlob^>^ m_blobs;
	internal:
		Dataset(IMap<String^, Object^>^ pMap);
		IMap<String^, Object^>^ GetMap(void);
	public:
		Dataset();
		[Windows::Foundation::Metadata::DefaultOverloadAttribute]
		Dataset(String^ sSigle);
		//
		property bool IsModified {
			bool get(){
				return m_pimpl->get_IsModified();
			}
			void set(bool b) {
				m_pimpl->set_IsModified(b);
			}
		}// IsModified
		property bool IsSelected {
			bool get() {
				return m_pimpl->get_IsSelected();
			}
			void set(bool b) {
				m_pimpl->set_IsSelected(b);
			}
		}// IsSelected
		//
		property String^ Id {
			String^ get();
		}// Id
		property String^ Rev {
			String^ get();
		}// rev
		property bool IsPersisted {
			bool get();
		}// IsPersisted
		property InfoStatus Status {
			InfoStatus get();
			void set(InfoStatus value);
		}// Status
		property String^ Observations {
			String^ get();
			void set(String^ value);
		}// Observations
		property String^ InfoType {
			String^ get() {
				return m_pimpl->get_Type();
			}
		}// Type
		property bool IsStoreable {
			bool get();
		}// IsStoreable
		property String^ Sigle {
			String^ get();
			void set(String^ value);
		}// Sigle
		property String^ Name {
			String^ get();
			void set(String^ value);
		}// name
		virtual String^ ToString(void) override;
		property IVector<Variable^>^ Variables {
			IVector<Variable^>^ get();
		void set(IVector<Variable^>^ value);
		}
		property IVector<Indiv^>^ Indivs {
			IVector<Indiv^>^ get();
			void set(IVector<Indiv^>^ value);
		}
		Variable^ FindVariable(String^ sigle);
		Indiv^ FindIndiv(String^ sigle);
		property String^ Annee {
			String^ get();
			void set(String^ value);
		}// name
		property IVector<InfoBlob^>^ Blobs {
			IVector<InfoBlob^>^ get();
			void set(IVector<InfoBlob^>^ value);
		}
	};// class Dataset
	  //////////////////////////////////
	public ref class Indiv sealed {
	private:
		std::unique_ptr<IndivImpl> m_pimpl;
		Dataset^ m_set;
		IVector<InfoValue^>^ m_vals;
		IVector<InfoBlob^>^ m_blobs;
	internal:
		Indiv(IMap<String^, Object^>^ pMap);
		IMap<String^, Object^>^ GetMap(void);
	public:
		Indiv();
		Indiv(Dataset^ pSet, String^ sigle);
		property bool IsModified {
			bool get() {
				return m_pimpl->get_IsModified();
			}
			void set(bool b) {
				m_pimpl->set_IsModified(b);
			}
		}// IsModified
		property bool IsSelected {
			bool get() {
				return m_pimpl->get_IsSelected();
			}
			void set(bool b) {
				m_pimpl->set_IsSelected(b);
			}
		}// IsSelected
		property String^ DatasetSigle {
			String^ get();
		}// DatasetSigle
		property String^ Id {
			String^ get();
		}// Id
		property String^ Rev {
			String^ get();
		}// rev
		property bool IsPersisted {
			bool get();
		}// IsPersisted
		property InfoStatus Status {
			InfoStatus get();
			void set(InfoStatus value);
		}// Status
		property String^ Observations {
			String^ get();
			void set(String^ value);
		}// Observations
		property String^ InfoType {
			String^ get() {
				return m_pimpl->get_Type();
			}
		}// Type
		property bool IsStoreable {
			bool get();
		}// IsStoreable
		property String^ Sigle {
			String^ get();
			void set(String^ value);
		}// Sigle
		property String^ Name {
			String^ get();
			void set(String^ value);
		}// name
		virtual String^ ToString(void) override;
		property Dataset^ Set {
			Dataset^ get() {
				return m_set;
			}
			void set(Dataset^ value) {
				m_set = value;
			}
		}
		property IVector<InfoValue^>^ Values {
			IVector<InfoValue^>^ get();
			void set(IVector<InfoValue^>^ value);
		}
		property IVector<InfoBlob^>^ Blobs {
			IVector<InfoBlob^>^ get();
			void set(IVector<InfoBlob^>^ value);
		}
	};// Indiv
	  ///////////////////////////////////////
	public ref class Variable sealed {
	private:
		std::unique_ptr<VariableImpl> m_pimpl;
		Dataset^ m_set;
		IVector<InfoValue^>^ m_vals;
		IVector<InfoBlob^>^ m_blobs;
	internal:
		Variable(IMap<String^, Object^>^ pMap);
		IMap<String^, Object^>^ GetMap(void);
	public:
		Variable();
		Variable(Dataset^ pSet, String^ sigle);
		property bool IsModified {
			bool get() {
				return m_pimpl->get_IsModified();
			}
			void set(bool b) {
				m_pimpl->set_IsModified(b);
			}
		}// IsModified
		property bool IsSelected {
			bool get() {
				return m_pimpl->get_IsSelected();
			}
			void set(bool b) {
				m_pimpl->set_IsSelected(b);
			}
		}// IsSelected
		property String^ DatasetSigle {
			String^ get();
		}// DatasetSigle
		property String^ Id {
			String^ get();
		}// Id
		property String^ Rev {
			String^ get();
		}// rev
		property bool IsPersisted {
			bool get();
		}// IsPersisted
		property InfoStatus Status {
			InfoStatus get();
			void set(InfoStatus value);
		}// Status
		property String^ Observations {
			String^ get();
			void set(String^ value);
		}// Observations
		property String^ InfoType {
			String^ get() {
				return m_pimpl->get_Type();
			}
		}// Type
		property bool IsStoreable {
			bool get();
		}// IsStoreable
		property String^ Sigle {
			String^ get();
			void set(String^ value);
		}// Sigle
		property String^ Name {
			String^ get();
			void set(String^ value);
		}// name
		property InfoDataType  VariableType {
			InfoDataType get();
			void set(InfoDataType value);
		}// VariableType
		property InfoKind      VariableKind {
			InfoKind get();
			void set(InfoKind value);
		}// VariableKind
		property IVector<String^>^ Modalites {
			IVector<String^>^ get();
			void set(IVector<String^>^ value);
		}// Modalites
		virtual String^ ToString(void) override;
		property Dataset^ Set {
			Dataset^ get() {
				return m_set;
			}
			void set(Dataset^ value) {
				m_set = value;
			}
		}
		property IVector<InfoValue^>^ Values {
			IVector<InfoValue^>^ get();
			void set(IVector<InfoValue^>^ value);
		}
		property IVector<InfoBlob^>^ Blobs {
			IVector<InfoBlob^>^ get();
			void set(IVector<InfoBlob^>^ value);
		}
	};// Variable
	  ////////////////////////////////////
	public ref class InfoValue sealed {
	private:
		bool m_modified;
		bool m_selected;
		InfoStatus m_status;
		String^ m_id;
		String^ m_rev;
		String^ m_desc;
		String^ m_datasetsigle;
		String^ m_indivsigle;
		String^ m_variablesigle;
		InfoDataValue^ m_value;
		Variable^ m_var;
		Indiv^ m_ind;
		IVector<InfoBlob^>^ m_blobs;
	internal:
		InfoValue(IMap<String^, Object^>^ oMap);
		IMap<String^, Object^>^ GetMap(void);
	public:
		InfoValue();
		InfoValue(Indiv^ pInd, Variable^ pVar);
		InfoValue(Indiv^ pInd, Variable^ pVar, InfoDataValue^ val);
		//
		property bool IsModified {
			bool get() {
				return m_modified;
			}
			void set(bool b) {
				m_modified = b;
			}
		}// IsModified
		property bool IsSelected {
			bool get() {
				return m_selected;
			}
			void set(bool b) {
				m_selected = false;
			}
		}// IsSelected
		//
		property String^ Id {
			String^ get();
		}// Id
		property String^ Rev {
			String^ get();
		}// rev
		property bool IsPersisted {
			bool get();
		}// IsPersisted
		property InfoStatus Status {
			InfoStatus get();
			void set(InfoStatus value);
		}// Status
		property String^ Observations {
			String^ get();
			void set(String^ value);
		}// Observations
		property String^ DatasetSigle {
			String^ get();
		}// DatasetSigle
		property String^ IndivSigle {
			String^ get();
		}// IndivSigle
		property String^ VariableSigle {
			String^ get();
		}// VariableSigle
		property String^ InfoType {
			String^ get() {
				return InfoStrings::TYPE_VALUE;
			}
		}// Type
		property InfoDataValue^ Value {
			InfoDataValue^ get();
			void set(InfoDataValue^ value);
		}// Value
		property bool IsStoreable {
			bool get();
		}// IsStoreable
		virtual String^ ToString(void) override;
		property Variable^ Var {
			Variable^ get() {
				return m_var;
			}
			void set(Variable^ value) {
				m_var = value;
			}
		}
		property Indiv^ Ind {
			Indiv^ get() {
				return m_ind;
			}
			void set(Indiv^ value) {
				m_ind = value;
			}
		}
		property bool HasValue {
			bool get() {
				return (m_value != nullptr) && m_value->HasValue;
			}
		}
		property bool IsEmpty {
			bool get() {
				return (!this->HasValue);
			}
		}
		property bool IsBoolean {
			bool get() {
				return (m_value != nullptr) && m_value->IsBoolean;
			}
		}
		property bool BoolValue {
			bool get() {
				return (m_value != nullptr) && m_value->BoolValue;
			}
			void set(bool value) {
				if (m_value == nullptr) {
					m_value = ref new InfoDataValue{value};
				}
				else {
					m_value->BoolValue = value;
				}
			}
		}
		property bool IsInteger {
			bool get() {
				return (m_value != nullptr) && m_value->IsInteger;
			}
		}
		property int IntValue {
			int get() {
				return (m_value != nullptr) ? m_value->IntValue : 0;
			}
			void set(int value) {
				if (m_value == nullptr) {
					m_value = ref new InfoDataValue{ value };
				}
				else {
					m_value->IntValue = value;
				}
			}
		}
		property bool IsDouble {
			bool get() {
				return (m_value != nullptr) && m_value->IsDouble;
			}
		}
		property double DoubleValue {
			double get() {
				return (m_value != nullptr) ? m_value->DoubleValue : 0;
			}
			void set(double value) {
				if (m_value == nullptr) {
					m_value = ref new InfoDataValue{ value };
				}
				else {
					m_value->DoubleValue = value;
				}
			}
		}
		property bool IsString {
			bool get() {
				return (m_value != nullptr) && m_value->IsString;
			}
		}
		property String^ StringValue {
			String^ get() {
				return (m_value != nullptr) ? m_value->StringValue : "";
			}
			void set(String^ value) {
				if (m_value == nullptr) {
					m_value = ref new InfoDataValue{ value };
				}
				else {
					m_value->StringValue = value;
				}
			}
		}
		property InfoDataType DataType  {
			InfoDataType get() {
				return (m_value != nullptr) ? m_value->DataType : InfoDataType::Unknown;
			}
		}
		property IVector<InfoBlob^>^ Blobs {
			IVector<InfoBlob^>^ get();
			void set(IVector<InfoBlob^>^ value);
		}
	};// InfoValue
	  /////////////////////////////////////
}// Namespace InfoDomain
