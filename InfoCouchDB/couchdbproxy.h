#pragma once
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Data::Json;
using namespace Windows::Web::Http;
using namespace Windows::Storage::Streams;
//////////////////////////////
namespace InfoCouchDB {
	///////////////////////
	using byte = uint8;
///////////////////////////
	class CouchDBProxy {
	private:
		String^ m_url;
		String^ m_database;
		HttpClient^ m_client;
		//
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
		//
		static IJsonValue^ ConvertObject(Object^ obj);
		static String^ ConvertFindFilter(IMap<String^, Object^>^ oFetch,
			IVector<String^>^ oFields, int skip = 0, int count = 0);
		static String^ MapToJson(IMap<String^, Object^>^ oMap);
		static String^ MapToJson(IVector<IMap<String^, Object^>^>^ oAr);
		static Object^ ConvertJsonObject(IJsonValue^ jsonVal);
		static byte *GetPointerToData(IBuffer^ pixelData, unsigned int *length);
		//
	public:
		CouchDBProxy(String^ url, String^ database);
		~CouchDBProxy();
	public:
		//
		bool IsAlive(void);
		//
		int GetCountFilter(IMap<String^, Object^>^ oFetch);
		IVector<IMap<String^, Object^>^>^ ReadDocuments(IMap<String^, Object^>^ oFetch, int offset = 0, int count = 0);
		IMap<String^, Object^>^ FindDocument(IMap<String^, Object^>^ oFetch);
		//
		String^ GetDocumentVersion(String^ docid);
		IMap<String^, Object^>^ ReadDocumentById(String^ docid);
		bool InsertDocument(IMap<String^, Object^>^ oMap);
		bool UpdateDocument(IMap<String^, Object^>^ oMap);
		bool MaintainsDocument(IMap<String^, Object^>^ oMap);
		bool DeleteDocumentById(String^ docid);
		//
		bool MaintainsDocuments(IVector<IMap<String^, Object^>^>^ oVec, bool bDelete = false);
		bool RemoveDocuments(IMap<String^, Object^>^ oFetch);
		//
		bool MaintainsDocumentAttachment(String^ docid, String^ attachmentName, String^ mimetype, IBuffer^ data);
		IMap<String^, String^> ^ GetDocumentAttachmentNames(String^ docid);
		bool RemoveDocumentAttachment(String^ docid, String^ attachmentName);
		IBuffer^ GetDocumentAttachmentData(String^ docid, String^ attachmentName);

	};// class CouchDBProxy
//////////////////////////////
}// namespace InfoCouchDB
