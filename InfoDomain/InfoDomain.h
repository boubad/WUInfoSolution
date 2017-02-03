#pragma once
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace InfoDomain
{
    public enum class InfoStatus{Unknown,Normal,Tail,Disabled,Info,Inactive,Inserted,Updated,Deleted};
	public enum class InfoKind {Unknown,Normal,Modal,Ordinal};
	public enum class InfoDataType{Unknown,Logical,Integer,Real,Text,Other};
	//
	public interface class IInfoDataValue {
		property InfoDataType DataType;
		property bool HasValue;
		property bool BoolValue;
		property int  IntValue;
		property double DoubleValue;
		property String^ StringValue;
		property Object^ Value;
		void Clear(void);
	};// interface IInfoDataValue
	//
	public interface class IInfoItem {
		property String^ Id;
		property String^ Rev;
		property InfoStatus Status;
		property String^ Observations;
		property String^ Type;
		IMap<String^, Object^>^ GetMap(void);
	};// interfaceI InfoItem
	public interface class ISigleNamedItem : public IInfoItem {
		property String^ Sigle;
		property String^ Name;
	};// interface ISigleNamedItem
	public interface class IDataset : public ISigleNamedItem
	{
	};// interface IDataset
	public interface class IDatasetChildItem : public ISigleNamedItem {
		property String^ DatasetSigle;
	};// interface IDatasetChildItem
	public interface class IIndiv : public IDatasetChildItem {

	};// interface IIndiv
	public interface class IVariable : public IDatasetChildItem {
		property InfoDataType  VariableType;
		property InfoKind      VariableKind;
		property IVector<String^>^ Modalites;
	};// interface IVariable
	public interface class IValue : IInfoItem {
		property String^ DatasetSigle;
		property String^ IndivSigle;
		property String^ VariableSigle;
		property IInfoDataValue^ Data;
	};// interface IValue
	public interface class IEtudiant : public IInfoItem {
		property String^ Dossier;
		property String^ Sexe;
		property String^ Lastname;
		property String^ Firstname;
		property String^ Birth;
		property String^ Departement;
		property String^ Ville;
		property String^ Etablissement;
		property String^ AnneeBac;
		property String^ SerieBac;
		property String^ OptionBac;
		property String^ MentionBac;
		property String^ EtudesSuperieures;
		property String^ Apb;
		property String^ Redoublant;
		property String^ Avatar;
		property IVector<String^>^ Datasets;
	};// interface IEtudiant
}// namespace InfoDomain
