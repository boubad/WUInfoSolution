#pragma once
#include "couchdbproxy.h"
/////////////////////////////
namespace InfoCouchDB {
	/////////////////////////////
	public ref class CouchDBManager sealed {
		using byte = uint8;
	private:
		String^ m_url;
		String^ m_dataBase;
		std::unique_ptr<CouchDBProxy> m_proxy;
		CouchDBProxy *GetProxy(void);
		static byte *GetPointerToBufferData(IBuffer^ pBuffer, unsigned int *pLength);
	public:
		CouchDBManager();
		CouchDBManager(String^ baseUrl, String^ databaseName);
		property String^ BaseUrl {
			String^ get();
			void set(String^ value);
		}
		property String^ DatabaseName {
			String^ get();
			void set(String^ value);
		}
	public:
		void SetUrlDatabase(String^ url, String^ database);
		//
		static IAsyncAction^ CheckDatabaseAsync(String^ url, String^ databaseName);
		static IAsyncOperation<bool>^ ExistsDatabaseAsync(String^ url, String^ databaseName);
		IAsyncOperation<bool>^ IsAliveAsync(void);
		IAsyncOperation<int>^ GetDocumentsCountAsync(IMap<String^, Object^>^ oFetch);
		IAsyncOperation<IVector<IMap<String^, Object^>^>^>^ GetDocumentsAsync(IMap<String^, Object^>^ oFetch, int offset, int count);
		IAsyncOperation<IMap<String^, Object^>^>^ FindDocumentAsync(IMap<String^, Object^>^ oFetch);
		IAsyncOperation<IMap<String^, Object^>^>^ ReadDocumentByIdAsync(String^ docid);
		IAsyncOperation<bool>^ MaintainsDocumentAsync(IMap<String^, Object^>^ oMap);
		IAsyncOperation<bool>^ DeleteDocumentByIdAsync(String^ docid);
		IAsyncOperation<bool>^ MaintainsDocumentsAsync(IVector<IMap<String^, Object^>^>^ oVec, bool bDelete);
		IAsyncOperation<bool>^ RemoveDocumentsAsync(IMap<String^, Object^>^ oFetch);
		//
		IAsyncOperation<bool>^ MaintainsDocumentAttachmentAsync(String^ docid, String^ attachmentName, String^ mimetype, IBuffer^ data);
		IAsyncOperation<bool>^ MaintainsDocumentAttachmentAsync(String^ docid, String^ attachmentName, IStorageFile^ file);
		IAsyncOperation<IMap<String^, String^> ^>^ GetDocumentAttachmentNamesAsync(String^ docid);
		IAsyncOperation<bool>^ RemoveDocumentAttachmentAsync(String^ docid, String^ attachmentName);
		IAsyncOperation<IBuffer^>^ GetDocumentAttachmentDataAsync(String^ docid, String^ attachmentName);
		IAsyncOperation<IVector<byte>^>^ GetDocumentAttachmentDataVectorAsync(String^ docid, String^ attachmentName);
		//
		IAsyncOperation<bool>^ CreateIndexAsync(IVector<String^>^ fields, String^ name, String^ designDoc);
	};// class CouchDBManager
	/////////////////////////////////
}// InfoCouchDN
