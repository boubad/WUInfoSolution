#pragma once
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Data::Json;
using namespace Windows::Web::Http;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace concurrency;
//////////////////////////////
namespace InfoCouchDB {
///////////////////////////
	class CouchDBProxy  {
	private:
		String^ m_url;
		String^ m_database;
		HttpClient^ m_client;
		//
		void check_database(void);
		//
		static String^ SLASH;
		static String^ REV;
		static String^ JSON_MIME_TYPE;
		static String^ KEY_ID;
		static String^ KEY_REV;
		static String^ KEY_DOCS;
		static String^ STRING_FIND;
		static String^ KEY_DELETED;
		static String^ STRING_BULKDOCS;
		static String^ KEY_ETAG;
		static String^ KEY_ATTACHMENTS;
		static String^ KEY_CONTENT_TYPE;
		static String^ ARG_ATTACHMENTS;
		static String^ STRING_INDEX_URI;
		//
		static String^ KEY_SELECTOR;
		static String^ KEY_FIELDS;
		static String^ KEY_LIMIT;
		static String^ KEY_SKIP;
		//
		static String^ KEY_INDEX;
		static String^ KEY_DESIGNDOC;
		static String^ KEY_NAME;
		//
		static IJsonValue^ ConvertObject(Object^ obj);
		static String^ ConvertFindFilter(IMap<String^, Object^>^ oFetch,
			IVector<String^>^ oFields, int skip = 0, int count = 0);
		static String^ MapToJson(IMap<String^, Object^>^ oMap);
		static String^ MapToJson(IVector<IMap<String^, Object^>^>^ oAr);
		static Object^ ConvertJsonObject(IJsonValue^ jsonVal);
		static IMap<String^, Object^>^ StReadObject(IJsonValue^ json);
		static IMap<String^, String^>^ StReadNamesMimes(String^ jsonText);
		static int StDocsCount(String^ jsonText);
		static IVector<IMap<String^, Object^>^>^ StReadDocs(String^ jsonText);
		//
	public:
		CouchDBProxy(String^ url, String^ database);
		~CouchDBProxy();
	public:
		//
		task<IMap<String^, Object^>^> ReadDocumentByIdAsync(String^ docid);
		task<bool> DeleteDocumentByIdAsync(String^ docid);
		
		task<String^> GetDocumentVersionAsync(String^ docid);
		task<IMap<String^, Object^>^> FindDocumentAsync(IMap<String^, Object^>^ oFetch);
		task<IVector<IMap<String^, Object^>^>^> ReadDocumentsAsync(IMap<String^, Object^>^ oFetch, int offset = 0, int count = 0);
		//
		task<bool> MaintainsDocumentAsync(IMap<String^, Object^>^ oMap);
		task<bool> UpdateDocumentAsync(IMap<String^, Object^>^ oMap);
		task<bool> InsertDocumentAsync(IMap<String^, Object^>^ oMap);
		task<bool> IsAliveAsync(void);
		task<int> GetCountFilterAsync(IMap<String^, Object^>^ oFetch);
		//
		task<bool> MaintainsDocumentAttachmentAsync(String^ docid, String^ attachmentName,
			String^ mimetype, IBuffer^ data);
		task<bool> MaintainsDocumentAttachmentAsync(String^ docid, String^ attachmentName,
			IStorageFile^ file);
		task<IMap<String^, String^> ^> GetDocumentAttachmentNamesAsync(String^ docid);
		task<IBuffer^> GetDocumentAttachmentDataAsync(String^ docid, String^ attachmentName);
		task<bool> RemoveDocumentAttachmentAsync(String^ docid, String^ attachmentName);
		//
		task<bool> MaintainsDocumentsAsync(IVector<IMap<String^, Object^>^>^ oVec, bool bDelete = false);
		task<bool> RemoveDocumentsAsync(IMap<String^, Object^>^ oFetch);
		//
		task<bool> CreateIndexAsync(IVector<String^>^ fields, String^ name = nullptr, String^ designDoc = nullptr);
	};// class CouchDBProxy
//////////////////////////////
}// namespace InfoCouchDB
