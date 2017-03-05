#pragma once
//
#include "DomainDataset.h"
#include "DomainEtudiant.h"
#include "DomainIndiv.h"
#include "DomainValue.h"
#include "DomainVariable.h"
/////////////////////////////////
//
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
//
using namespace InfoCouchDB;
//
namespace GenDomainData
{
	using byte = uint8;
	//
	[Windows::Foundation::Metadata::WebHostHidden]
	[Windows::UI::Xaml::Data::Bindable]
	public ref class DomainDBManager sealed : INotifyPropertyChanged
	{
	private:
		//
		String^ m_base;
		String^ m_data;
		CouchDBManager^ m_pman;
		//
		void check_photo(DomainEtudiant ^p);
		void internal_check_index(String^ field, String^ name);
		void check_indexes(void);
		property CouchDBManager^ Manager {
			CouchDBManager^ get();
		}
		void OnPropertyChanged(String^ propertyName);
	public:
		DomainDBManager();
		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler^ PropertyChanged;
	public:
		property String^ BaseUrl {
			String^ get();
			void set(String^ value);
		}
		property String^ DatabaseName {
			String^ get();
			void set(String^ value);
		}
		property bool IsValid {
			bool get();
		}
	public:
		static IAsyncAction^ CheckDatabaseAsync(String^ url, String^ databaseName);
		static IAsyncOperation<bool>^ ExistsDatabaseAsync(String^ url, String^ databaseName);
		IAsyncOperation<int>^ GetDatasetsCountAsync(DomainDataset^ model);
		IAsyncOperation<IVector<DomainDataset^>^>^ GetDatasetsAsync(DomainDataset^ model, int offset, int count);
		IAsyncOperation<DomainDataset^>^ FindDatasetAsync(DomainDataset^ model);
		IAsyncOperation<DomainDataset^>^ FindDatasetBySigleAsync(String^ sigle);
		IAsyncOperation<bool>^ MaintainsDatasetAsync(DomainDataset^ model);
		IAsyncOperation<bool>^ RemoveDatasetAsync(DomainDataset^ model);
		IAsyncOperation<bool>^ MaintainsDatasetsAsync(IVector<DomainDataset^>^ oVec, bool bDelete);
		IAsyncOperation<IVector<String^>^>^ GetAllDatasetsSigles(void);
		//
		IAsyncOperation<int>^ GetDatasetVariablesCountAsync(DomainDataset^ pSet);
		IAsyncOperation<IVector<DomainVariable^>^>^ GetDatasetVariablesAsync(DomainDataset^ pSet, int offset, int count);
		IAsyncOperation<DomainVariable^>^ FindVariable(DomainVariable^ model);
		IAsyncOperation<DomainVariable^>^ FindVariableBySiglesAsync(String^ setsigle, String^ sigle);
		IAsyncOperation<bool>^ MaintainsVariableAsync(DomainVariable^ model);
		IAsyncOperation<bool>^ RemoveVariableAsync(DomainVariable^ model);
		IAsyncOperation<bool>^ MaintainsVariablesAsync(IVector<DomainVariable^>^ oVec, bool bDelete);
		//
		IAsyncOperation<int>^ GetDatasetIndivsCountAsync(DomainDataset^ pSet);
		IAsyncOperation<IVector<DomainIndiv^>^>^ GetDatasetIndivsAsync(DomainDataset^ pSet, int offset, int count);
		IAsyncOperation<DomainIndiv^>^ FindIndiv(DomainIndiv^ model);
		IAsyncOperation<DomainIndiv^>^ FindIndivBySiglesAsync(String^ setsigle, String^ sigle);
		IAsyncOperation<bool>^ MaintainsIndivAsync(DomainIndiv^ model);
		IAsyncOperation<bool>^ RemoveIndivAsync(DomainIndiv^ model);
		IAsyncOperation<bool>^ MaintainsIndivsAsync(IVector<DomainIndiv^>^ oVec, bool bDelete);
		//
		IAsyncOperation<int>^ GetDatasetValuesCountAsync(DomainDataset^ model);
		IAsyncOperation<IVector<DomainValue^>^>^ GetDatasetValuesAsync(DomainDataset ^model, int offset, int count);
		IAsyncOperation<int>^ GetVariableValuesCountAsync(DomainVariable^ model);
		IAsyncOperation<IVector<DomainValue^>^>^ GetVariableValuesAsync(DomainVariable ^model, int offset, int count);
		IAsyncOperation<int>^ GetIndivValuesCountAsync(DomainIndiv^ model);
		IAsyncOperation<IVector<DomainValue^>^>^ GetIndivValuesAsync(DomainIndiv ^model, int offset, int count);
		IAsyncOperation<DomainValue^>^ FindValue(DomainValue^ model);
		IAsyncOperation<DomainValue^>^ FindValueBySiglesAsync(String^ setsigle, String^ indsigle, String^ varsigle);
		IAsyncOperation<bool>^ MaintainsValueAsync(DomainValue^ model);
		IAsyncOperation<bool>^ RemoveValueAsync(DomainValue^ model);
		IAsyncOperation<bool>^ MaintainsValuesAsync(IVector<DomainValue^>^ oVec, bool bDelete);
		//
		IAsyncOperation<DomainEtudiant^>^ FindEtudiantByDossierAsync(String^ dossier);
		IAsyncOperation<int>^ GetEtudiantsCountAsync(DomainEtudiant^ pModel);
		IAsyncOperation<IVector<DomainEtudiant^>^>^ GetEtudiantsAsync(DomainEtudiant^ pModel, int offset, int count);
		IAsyncOperation<bool>^ MaintainsEtudiantAsync(DomainEtudiant^ pModel);
		IAsyncOperation<bool>^ RemoveEtudiantAsync(DomainEtudiant^ pModel);
		IAsyncOperation<bool>^ MaintainsEtudiantsAsync(IVector<DomainEtudiant^>^ pVec, bool bDelete);
		//
		IAsyncOperation<bool>^ MaintainsDocumentAttachmentAsync(String^ docid, String^ attachmentName, String^ mimetype, IBuffer^ data);
		IAsyncOperation<bool>^ MaintainsDocumentAttachmentAsync(String^ docid, String^ attachmentName, IStorageFile^ file);
		IAsyncOperation<IMap<String^, String^> ^>^ GetDocumentAttachmentNamesAsync(String^ docid);
		IAsyncOperation<bool>^ RemoveDocumentAttachmentAsync(String^ docid, String^ attachmentName);
		IAsyncOperation<IBuffer^>^ GetDocumentAttachmentDataAsync(String^ docid, String^ attachmentName);
		IAsyncOperation<IVector<byte>^>^ GetDocumentAttachmentDataVectorAsync(String^ docid, String^ attachmentName);
		//
		IAsyncOperation<IObservableVector<String^>^>^ GetFieldsDistinctAsync(String^ fname);
		//
	};// class DomainDBManager
}// namespace GenDomainData

