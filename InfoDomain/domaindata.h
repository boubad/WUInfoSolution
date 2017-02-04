#pragma once
#include "domainimpl.h"

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Platform::Metadata;
//
namespace InfoDomain {
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
		InfoDataValue(Platform::String^ b);
		//
		property InfoDataType DataType {
			InfoDataType get();
		}
		property bool HasValue {
			bool get();
		}// HasValue
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
		property Platform::String^ StringValue {
			Platform::String^ get();
			void set(Platform::String^ b);
		}// StringValue
		property Object^ Value {
			Object^ get();
		}// Value
		void Clear(void);
	};// class InfoDataValue
	  ///////////////////////////////
	public ref class Dataset sealed {
	private:
		std::unique_ptr<DatasetImpl> m_pimpl;
	public:
		Dataset();
		Dataset(IMap<Platform::String^, Object^>^ pMap);
		[Windows::Foundation::Metadata::DefaultOverloadAttribute]
		Dataset(Platform::String^ sSigle);
		//
		property Platform::String^ Id {
			Platform::String^ get();
		}// Id
		property Platform::String^ Rev {
			Platform::String^ get();
		}// rev
		property bool IsPersisted {
			bool get();
		}// IsPersisted
		property InfoStatus Status {
			InfoStatus get();
			void set(InfoStatus value);
		}// Status
		property Platform::String^ Observations {
			Platform::String^ get();
			void set(Platform::String^ value);
		}// Observations
		property Platform::String^ Type {
			Platform::String^ get();
		}// Type
		property bool IsStoreable {
			bool get();
		}// IsStoreable
		property Platform::String^ Sigle {
			Platform::String^ get();
			void set(Platform::String^ value);
		}// Sigle
		property Platform::String^ Name {
			Platform::String^ get();
			void set(Platform::String^ value);
		}// name
		IMap<Platform::String^, Object^>^ GetMap(void);
	};// class Dataset
	  //////////////////////////////////
	public ref class Indiv sealed {
	private:
		std::unique_ptr<IndivImpl> m_pimpl;
	public:
		Indiv();
		Indiv(Dataset^ pSet, Platform::String^ sigle);
		Indiv(IMap<Platform::String^, Object^>^ pMap);
		property Platform::String^ DatasetSigle {
			Platform::String^ get();
		}// DatasetSigle
		property Platform::String^ Id {
			Platform::String^ get();
		}// Id
		property Platform::String^ Rev {
			Platform::String^ get();
		}// rev
		property bool IsPersisted {
			bool get();
		}// IsPersisted
		property InfoStatus Status {
			InfoStatus get();
			void set(InfoStatus value);
		}// Status
		property Platform::String^ Observations {
			Platform::String^ get();
			void set(Platform::String^ value);
		}// Observations
		property Platform::String^ Type {
			Platform::String^ get();
		}// Type
		property bool IsStoreable {
			bool get();
		}// IsStoreable
		property Platform::String^ Sigle {
			Platform::String^ get();
			void set(Platform::String^ value);
		}// Sigle
		property Platform::String^ Name {
			Platform::String^ get();
			void set(Platform::String^ value);
		}// name
		IMap<Platform::String^, Object^>^ GetMap(void);
	};// Indiv
	  ///////////////////////////////////////
	public ref class Variable sealed {
	private:
		std::unique_ptr<VariableImpl> m_pimpl;
	public:
		Variable();
		Variable(Dataset^ pSet, Platform::String^ sigle);
		Variable(IMap<Platform::String^, Object^>^ pMap);
		property Platform::String^ DatasetSigle {
			Platform::String^ get();
		}// DatasetSigle
		property Platform::String^ Id {
			Platform::String^ get();
		}// Id
		property Platform::String^ Rev {
			Platform::String^ get();
		}// rev
		property bool IsPersisted {
			bool get();
		}// IsPersisted
		property InfoStatus Status {
			InfoStatus get();
			void set(InfoStatus value);
		}// Status
		property Platform::String^ Observations {
			Platform::String^ get();
			void set(Platform::String^ value);
		}// Observations
		property Platform::String^ Type {
			Platform::String^ get();
		}// Type
		property bool IsStoreable {
			bool get();
		}// IsStoreable
		property Platform::String^ Sigle {
			Platform::String^ get();
			void set(Platform::String^ value);
		}// Sigle
		property Platform::String^ Name {
			Platform::String^ get();
			void set(Platform::String^ value);
		}// name
		property InfoDataType  VariableType {
			InfoDataType get();
			void set(InfoDataType value);
		}// VariableType
		property InfoKind      VariableKind {
			InfoKind get();
			void set(InfoKind value);
		}// VariableKind
		property IVector<Platform::String^>^ Modalites {
			IVector<Platform::String^>^ get();
			void set(IVector<Platform::String^>^ value);
		}// Modalites
		IMap<Platform::String^, Object^>^ GetMap(void);
	};// Variable
	  ////////////////////////////////////
	public ref class InfoValue sealed {
	private:
		InfoStatus m_status;
		Platform::String^ m_id;
		Platform::String^ m_rev;
		Platform::String^ m_desc;
		Platform::String^ m_datasetsigle;
		Platform::String^ m_indivsigle;
		Platform::String^ m_variablesigle;
		InfoDataValue^ m_value;
	public:
		InfoValue();
		InfoValue(IMap<Platform::String^, Object^>^ oMap);
		InfoValue(Indiv^ pInd, Variable^ pVar);
		InfoValue(Indiv^ pInd, Variable^ pVar, InfoDataValue^ val);
		//
		property Platform::String^ Id {
			Platform::String^ get();
		}// Id
		property Platform::String^ Rev {
			Platform::String^ get();
		}// rev
		property bool IsPersisted {
			bool get();
		}// IsPersisted
		property InfoStatus Status {
			InfoStatus get();
			void set(InfoStatus value);
		}// Status
		property Platform::String^ Observations {
			Platform::String^ get();
			void set(Platform::String^ value);
		}// Observations
		property Platform::String^ DatasetSigle {
			Platform::String^ get();
		}// DatasetSigle
		property Platform::String^ IndivSigle {
			Platform::String^ get();
		}// IndivSigle
		property Platform::String^ VariableSigle {
			Platform::String^ get();
		}// VariableSigle
		property Platform::String^ Type {
			Platform::String^ get();
		}// Type
		property InfoDataValue^ Value {
			InfoDataValue^ get();
			void set(InfoDataValue^ value);
		}// Value
		property bool IsStoreable {
			bool get();
		}// IsStoreable
		IMap<Platform::String^, Object^>^ GetMap(void);
	};// InfoValue
	  /////////////////////////////////////
}// Namespace InfoDomain
