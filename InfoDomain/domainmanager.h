#pragma once
#include "domaindata.h"
//
using namespace InfoCouchDB;
////////////////////////////////
namespace InfoDomain {
/////////////////////////
	public ref class DomainManager sealed {
	private:
		CouchDBManager^ m_pman;
	public:
		DomainManager(String^ baseUrl, String^ databaseName);
	public:
		IAsyncOperation<int>^ GetDatasetsCountAsync(void);
		IAsyncOperation<IVector<Dataset^>^>^ GetDatasetsAsync(int offset, int count);
		IAsyncOperation<Dataset^>^ FindDatasetAsync(Dataset^ model);
		IAsyncOperation<bool>^ MaintainsDatasetAsync(Dataset^ model);
		IAsyncOperation<bool>^ RemoveDatasetAsync(Dataset^ model);
		IAsyncOperation<bool>^ MaintainsDatasetsAsync(IVector<Dataset^>^ oVec, bool bDelete);
		//
		IAsyncOperation<int>^ GetDatasetVariablesCountAsync(Dataset^ pSet);
		IAsyncOperation<IVector<Variable^>^>^ GetDatasetVariables(Dataset^ pSet, int offset, int count);
		IAsyncOperation<Variable^>^ FindVariable(Variable^ model);
		IAsyncOperation<bool>^ MaintainsVariableAsync(Variable^ model);
		IAsyncOperation<bool>^ RemoveVariableAsync(Variable^ model);
		IAsyncOperation<bool>^ MaintainsVariablesAsync(IVector<Variable^>^ oVec, bool bDelete);
		//
		IAsyncOperation<int>^ GetDatasetIndivsCountAsync(Dataset^ pSet);
		IAsyncOperation<IVector<Indiv^>^>^ GetDatasetIndivs(Dataset^ pSet, int offset, int count);
		IAsyncOperation<Indiv^>^ FindIndiv(Indiv^ model);
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
		IAsyncOperation<bool>^ MaintainsValueAsync(InfoValue^ model);
		IAsyncOperation<bool>^ RemoveValueAsync(InfoValue^ model);
		IAsyncOperation<bool>^ MaintainsValuesAsync(IVector<InfoValue^>^ oVec, bool bDelete);
	};// class DomainManager
//////////////////////////
}// namespace InfoDomain
