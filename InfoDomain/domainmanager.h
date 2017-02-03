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
	};// class DomainManager
//////////////////////////
}// namespace InfoDomain
