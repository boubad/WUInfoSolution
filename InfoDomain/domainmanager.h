#pragma once
#include "domaindata.h"
//
using namespace InfoCouchDB;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
////////////////////////////////
namespace InfoDomain {
/////////////////////////
	public ref class DomainManager sealed {
		using byte = uint8;
	private:
		CouchDBManager^ m_pman;
	public:
		DomainManager(String^ baseUrl, String^ databaseName);
		property CouchDBManager^ Manager{
			CouchDBManager^ get();
		}
	public:
		IAsyncOperation<int>^ GetDatasetsCountAsync(void);
		IAsyncOperation<IVector<Dataset^>^>^ GetDatasetsAsync(int offset, int count);
		IAsyncOperation<Dataset^>^ FindDatasetAsync(Dataset^ model);
		IAsyncOperation<Dataset^>^ FindDatasetBySigleAsync(String^ sigle);
		IAsyncOperation<bool>^ MaintainsDatasetAsync(Dataset^ model);
		IAsyncOperation<bool>^ RemoveDatasetAsync(Dataset^ model);
		IAsyncOperation<bool>^ MaintainsDatasetsAsync(IVector<Dataset^>^ oVec, bool bDelete);
		//
		IAsyncOperation<int>^ GetDatasetVariablesCountAsync(Dataset^ pSet);
		IAsyncOperation<IVector<Variable^>^>^ GetDatasetVariables(Dataset^ pSet, int offset, int count);
		IAsyncOperation<Variable^>^ FindVariable(Variable^ model);
		IAsyncOperation<Variable^>^ FindVariableBySiglesAsync(String^ setsigle, String^ sigle);
		IAsyncOperation<bool>^ MaintainsVariableAsync(Variable^ model);
		IAsyncOperation<bool>^ RemoveVariableAsync(Variable^ model);
		IAsyncOperation<bool>^ MaintainsVariablesAsync(IVector<Variable^>^ oVec, bool bDelete);
		//
		IAsyncOperation<int>^ GetDatasetIndivsCountAsync(Dataset^ pSet);
		IAsyncOperation<IVector<Indiv^>^>^ GetDatasetIndivs(Dataset^ pSet, int offset, int count);
		IAsyncOperation<Indiv^>^ FindIndiv(Indiv^ model);
		IAsyncOperation<Indiv^>^ FindIndivBySiglesAsync(String^ setsigle, String^ sigle);
		IAsyncOperation<bool>^ MaintainsIndivAsync(Indiv^ model);
		IAsyncOperation<bool>^ RemoveIndivAsync(Indiv^ model);
		IAsyncOperation<bool>^ MaintainsIndivsAsync(IVector<Indiv^>^ oVec, bool bDelete);
		//
		IAsyncOperation<int>^ GetDatasetValuesCountAsync(Dataset^ model);
		IAsyncOperation<IVector<InfoValue^>^>^ GetDatasetValuesAsync(Dataset ^model,int offset, int count);
		IAsyncOperation<int>^ GetVariableValuesCountAsync(Variable^ model);
		IAsyncOperation<IVector<InfoValue^>^>^ GetVariableValuesAsync(Variable ^model, int offset, int count);
		IAsyncOperation<int>^ GetIndivValuesCountAsync(Indiv^ model);
		IAsyncOperation<IVector<InfoValue^>^>^ GetIndivValuesAsync(Indiv ^model, int offset, int count);
		IAsyncOperation<InfoValue^>^ FindValue(InfoValue^ model);
		IAsyncOperation<InfoValue^>^ FindValueBySiglesAsync(String^ setsigle, String^ indsigle, String^ varsigle);
		IAsyncOperation<bool>^ MaintainsValueAsync(InfoValue^ model);
		IAsyncOperation<bool>^ RemoveValueAsync(InfoValue^ model);
		IAsyncOperation<bool>^ MaintainsValuesAsync(IVector<InfoValue^>^ oVec, bool bDelete);
		//
		IAsyncOperation<bool>^ MaintainsDocumentAttachmentAsync(String^ docid, String^ attachmentName, String^ mimetype, IBuffer^ data);
		IAsyncOperation<bool>^ MaintainsDocumentAttachmentAsync(String^ docid, String^ attachmentName, IStorageFile^ file);
		IAsyncOperation<IMap<String^, String^> ^>^ GetDocumentAttachmentNamesAsync(String^ docid);
		IAsyncOperation<bool>^ RemoveDocumentAttachmentAsync(String^ docid, String^ attachmentName);
		IAsyncOperation<IBuffer^>^ GetDocumentAttachmentDataAsync(String^ docid, String^ attachmentName);
		IAsyncOperation<IVector<byte>^>^ GetDocumentAttachmentDataVectorAsync(String^ docid, String^ attachmentName);
	};// class DomainManager
//////////////////////////
}// namespace InfoDomain
