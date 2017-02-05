#include "pch.h"
#include "couchdbproxy.h"
//////////////////////////////
#include <robuffer.h>
/////////////////////////////
using namespace Platform::Collections;
using namespace Windows::Web::Http::Headers;
using namespace InfoCouchDB;
using namespace concurrency;
using namespace Microsoft::WRL;
/////////////////////////
String^ CouchDBProxy::JSON_MIME_TYPE = "application/json";
String^ CouchDBProxy::KEY_ID = "_id";
String^ CouchDBProxy::KEY_REV = "_rev";
String^ CouchDBProxy::KEY_DOCS = "docs";
String^ CouchDBProxy::STRING_FIND = "/_find";
String^ CouchDBProxy::KEY_DELETED = "_deleted";
String^ CouchDBProxy::STRING_BULKDOCS = "/_bulk_docs";
String^ CouchDBProxy::KEY_ETAG = "ETag";
String^ CouchDBProxy::KEY_ATTACHMENTS = "_attachments";
String^ CouchDBProxy::KEY_CONTENT_TYPE = "content_type";
///////////////////////
CouchDBProxy::CouchDBProxy(String^ url, String^ database) {
	m_url = url;
	m_database = database;
	m_client = ref new HttpClient();
	m_client->DefaultRequestHeaders->Accept->Clear();
	m_client->DefaultRequestHeaders->Accept->Append(ref new HttpMediaTypeWithQualityHeaderValue(JSON_MIME_TYPE));
}
CouchDBProxy::~CouchDBProxy() {
}//
////////////////////////////////////////////////
IBuffer^ CouchDBProxy::GetDocumentAttachmentData(String^ docid, String^ attachmentName) {
	String^ rev = this->GetDocumentVersion(docid);
	if (rev->IsEmpty()) {
		IBuffer^ oRet;
		return oRet;
	}
	String^ sUri = this->m_url + m_database + "/" + Uri::EscapeComponent(docid) + "/" + Uri::EscapeComponent(attachmentName) + "?rev=" + rev;
	Uri^ uri = ref new Uri(sUri);
	auto myop = this->m_client->GetAsync(uri);
	auto operationTask = create_task(myop);
	return operationTask.then([](HttpResponseMessage^ response) {
		IBuffer^ oRet;
		auto status = response->StatusCode;
		if ((status == HttpStatusCode::NotModified) || (status == HttpStatusCode::Ok)) {
			oRet  = create_task(response->Content->ReadAsBufferAsync()).get();
		}// ok
		return (oRet);
	}).get();
}//GetDocumentAttachmentData
bool CouchDBProxy::RemoveDocumentAttachment(String^ docid, String^ attachmentName) {
	String^ rev = this->GetDocumentVersion(docid);
	if (rev->IsEmpty()) {
		return false;
	}
	String^ sUri = this->m_url + m_database + "/" + Uri::EscapeComponent(docid) + "/" + Uri::EscapeComponent(attachmentName) + "?rev=" + rev;
	Uri^ uri = ref new Uri(sUri);
	auto myop = this->m_client->DeleteAsync(uri);
	auto operationTask = create_task(myop);
	bool bRet = operationTask.then([](HttpResponseMessage^ response) {
		bool b = false;
		auto status = response->StatusCode;
		if ((status == HttpStatusCode::Accepted) || (status == HttpStatusCode::Ok)) {
			b = true;
		}
		return (b);
	}).get();
	return (bRet);
}//RemoveDocumentAttachment
IMap<String^, String^> ^ CouchDBProxy::GetDocumentAttachmentNames(String^ docid) {
	String^ sUri = this->m_url + m_database + "/" + Uri::EscapeComponent(docid) + "?attachments=true";
	Uri^ uri = ref new Uri(sUri);
	auto myop = m_client->GetAsync(uri);
	auto operationTask = create_task(myop);
	return operationTask.then([](HttpResponseMessage^ response) {
		IMap<String^, String^>^ oRet;
		auto status = response->StatusCode;
		if ((status == HttpStatusCode::Ok) || (status == HttpStatusCode::NotModified)) {
			auto myopx = response->Content->ReadAsStringAsync();
			create_task(myopx).then([&oRet](String^ jsonText) {
				oRet = ref new Map<String^, String^>();
				JsonObject^ obj = JsonObject::Parse(jsonText);
				if (obj->HasKey(KEY_ATTACHMENTS)) {
					IJsonValue^ vo = obj->Lookup(KEY_ATTACHMENTS);
					if (vo->ValueType == JsonValueType::Object) {
						JsonObject^ oo = vo->GetObject();
						auto it = oo->First();
						while (it->HasCurrent) {
							auto p = it->Current;
							String^ key = p->Key;
							IJsonValue^ vx = p->Value;
							if (vx->ValueType == JsonValueType::Object) {
								JsonObject^ o = vx->GetObject();
								if (o->HasKey(KEY_CONTENT_TYPE)) {
									IJsonValue^ vz = o->Lookup(KEY_CONTENT_TYPE);
									if (vz->ValueType == JsonValueType::String) {
										String^ sval = vz->GetString();
										if (!sval->IsEmpty()) {
											oRet->Insert(key, sval);
										}
									}
								}//
							}//
							it->MoveNext();
						}// it
					}//oo
				}// attachments
			}).wait();
		}
		return (oRet);
	}).get();
}//GetDocumentAttachmentNames
bool CouchDBProxy::MaintainsDocumentAttachment(String^ docid, String^ attachmentName, String^ mimetype, IBuffer^ data){
	String^ rev = this->GetDocumentVersion(docid);
	if (rev->IsEmpty()) {
		return false;
	}
	unsigned int len = 0;
	byte *pData = CouchDBProxy::GetPointerToData(data, &len);
	if ((pData == nullptr) || (len < 1)) {
		return false;
	}
	HttpBufferContent^ sc = ref new HttpBufferContent(data);
	sc->Headers->ContentType->MediaType = mimetype;
	String^ sUri = this->m_url + m_database + "/" + Uri::EscapeComponent(docid) + "/" + Uri::EscapeComponent(attachmentName) + "?rev=" + rev;
	Uri^ uri = ref new Uri(sUri);
	auto myop = this->m_client->PutAsync(uri, sc);
	auto operationTask = create_task(myop);
	bool bRet = operationTask.then([](HttpResponseMessage^ response) {
		bool b = false;
		auto status = response->StatusCode;
		if ((status == HttpStatusCode::Accepted) || (status == HttpStatusCode::Ok)) {
			b = true;
		}
		return (b);
	}).get();
	return (bRet);
}// MaintainsDocumentAttachment
//////////////////////////////////////////////
bool CouchDBProxy::RemoveDocuments(IMap<String^, Object^>^ oFetch) {
	JsonArray^ oRetArray = ref new JsonArray();
	String^ sUri = this->m_url + m_database + STRING_FIND;
	Uri^ uri = ref new Uri(sUri);
	int nCount = 128;
	int nOffset = 0;
	bool done = false;
	IVector<String^>^ pFields = ref new Vector<String^>();
	pFields->Append(KEY_ID);
	pFields->Append(KEY_REV);
	do {
		String^ sp = ConvertFindFilter(oFetch, pFields, nOffset, nCount);
		HttpStringContent^ sc = ref new HttpStringContent(sp);
		sc->Headers->ContentType->MediaType = JSON_MIME_TYPE;
		auto myop = this->m_client->PostAsync(uri, sc);
		auto operationTask = create_task(myop);
		int nx = operationTask.then([oRetArray](HttpResponseMessage^ response) {
			int nRet = 0;
			auto status = response->StatusCode;
			if (status == HttpStatusCode::Ok) {
				try {
					auto myopx = response->Content->ReadAsStringAsync();
					create_task(myopx).then([&nRet, &oRetArray](String^ jsonText) {
						JsonValue^ val = JsonValue::Parse(jsonText);
						if (val->ValueType == JsonValueType::Object) {
							JsonObject^ obj = val->GetObject();
							if (obj->HasKey(KEY_DOCS)) {
								IJsonValue^ vx = obj->Lookup(KEY_DOCS);
								if (vx->ValueType == JsonValueType::Array) {
									JsonArray^ oAr = vx->GetArray();
									nRet = (int)oAr->Size;
									if (nRet > 0) {
										auto it = oAr->First();
										while (it->HasCurrent) {
											IJsonValue^ vx = it->Current;
											if (vx->ValueType == JsonValueType::Object) {
												JsonObject^ o = vx->GetObject();
												String^ sid;
												String^ srev;
												if (o->HasKey(KEY_ID)) {
													sid = o->Lookup(KEY_ID)->ToString();
												}
												if (o->HasKey(KEY_REV)) {
													srev = o->Lookup(KEY_REV)->ToString();
												}
												if ((!sid->IsEmpty()) && (!srev->IsEmpty())) {
													JsonObject^ oo = ref new JsonObject();
													oo->Insert(KEY_ID, JsonValue::CreateStringValue(sid));
													oo->Insert(KEY_REV, JsonValue::CreateStringValue(srev));
													oo->Insert(KEY_DELETED, JsonValue::CreateBooleanValue(true));
													oRetArray->Append(oo);
												}
											}// obj
											it->MoveNext();
										}// it
									}// nRet
								}// array
							}// docs
						}// obj
					}).get();
				}
				catch (Exception^ e) {
					(void)e;
				}
			}// status
			return (nRet);
		}).get();
		if (nx > 0) {
			nOffset += nx;
		}
		if (nx < nCount) {
			done = true;
			break;
		}
	} while (!done);
	if (oRetArray->Size < 1) {
		return (true);
	}
	JsonObject^ queryObj = ref new JsonObject();
	queryObj->Insert(KEY_DOCS, oRetArray);
	String^ sp = queryObj->Stringify();
	sUri = m_url + m_database + STRING_BULKDOCS;
	uri = ref new Uri(sUri);
	HttpStringContent^ sc = ref new HttpStringContent(sp);
	sc->Headers->ContentType->MediaType = JSON_MIME_TYPE;
	auto myop = this->m_client->PostAsync(uri, sc);
	auto operationTask = create_task(myop);
	bool bRet = operationTask.then([](HttpResponseMessage^ response) {
		bool b = false;
		auto status = response->StatusCode;
		if ((status == HttpStatusCode::Created) || (status == HttpStatusCode::Ok)) {
			b = true;
		}
		return (b);
	}).get();
	return (bRet);
}//RemoveDocuments
bool CouchDBProxy::MaintainsDocuments(IVector<IMap<String^, Object^>^>^ oVec, bool bDelete /*= false*/) {
	JsonArray^ oAr = ref new JsonArray();
	auto it = oVec->First();
	while (it->HasCurrent) {
		IMap<String^, Object^>^ p = it->Current;
		JsonObject^ doc = ref new JsonObject();
		if (bDelete) {
			if (p->HasKey(KEY_ID) && p->HasKey(KEY_REV)) {
				doc->Insert(KEY_ID, ConvertObject(p->Lookup(KEY_ID)));
				doc->Insert(KEY_REV, ConvertObject(p->Lookup(KEY_REV)));
				doc->Insert(KEY_DELETED, JsonValue::CreateBooleanValue(true));
			}
		}
		else {
			if (p->HasKey(KEY_ID)) {
				bool bFound = false;
				String^ docid = p->Lookup(KEY_ID)->ToString();
				if (!docid->IsEmpty()) {
					String^ rev = this->GetDocumentVersion(docid);
					if (!rev->IsEmpty()) {
						bFound = true;
						if (p->HasKey(KEY_REV)) {
							p->Remove(KEY_REV);
						}
						p->Insert(KEY_REV, rev);
					}// ok rev
				}// id
				if (!bFound) {
					if (p->HasKey(KEY_REV)) {
						p->Remove(KEY_REV);
					}
				}
			}
			auto jt = p->First();
			while (jt->HasCurrent) {
				auto px = jt->Current;
				String^ key = px->Key;
				IJsonValue^ v = ConvertObject(px->Value);
				doc->Insert(key, v);
				jt->MoveNext();
			}// jt
		}
		if (doc->Size > 0) {
			oAr->Append(doc);
		}
		it->MoveNext();
	}// it
	if (oAr->Size < 1) {
		return (false);
	}
	JsonObject^ queryObj = ref new JsonObject();
	queryObj->Insert(KEY_DOCS, oAr);
	String^ sp = queryObj->Stringify();
	String^ sUri = m_url + m_database + STRING_BULKDOCS;
	Uri^ uri = ref new Uri(sUri);
	HttpStringContent^ sc = ref new HttpStringContent(sp);
	sc->Headers->ContentType->MediaType = JSON_MIME_TYPE;
	auto myop = this->m_client->PostAsync(uri, sc);
	auto operationTask = create_task(myop);
	bool bRet = operationTask.then([](HttpResponseMessage^ response) {
		bool b = false;
		auto status = response->StatusCode;
		if ((status == HttpStatusCode::Created) || (status == HttpStatusCode::Ok)) {
			b = true;
		}
		return (b);
	}).get();
	return (bRet);
}// MaintainsDocuments
//////////////////////////////////////////////
bool CouchDBProxy::DeleteDocumentById(String^ docid) {
	String^ rev = GetDocumentVersion(docid);
	if (rev->IsEmpty()) {
		return (false);
	}
	String^ sUri = this->m_url + m_database + "/" + Uri::EscapeComponent(docid) + "?=" + rev;
	Uri^ uri = ref new Uri(sUri);
	auto myop = this->m_client->DeleteAsync(uri);
	auto operationTask = create_task(myop);
	bool bRet = operationTask.then([](HttpResponseMessage^ response) {
		bool b = false;
		auto status = response->StatusCode;
		if ((status == HttpStatusCode::Ok) || (status == HttpStatusCode::Accepted)) {
			b = true;
		}
		return (b);
	}).get();
	return (bRet);
}//DeleteDocumentById
bool CouchDBProxy::MaintainsDocument(IMap<String^, Object^>^ oMap) {
	if (!oMap->HasKey(KEY_ID)) {
		return InsertDocument(oMap);
	}
	String^ docid = oMap->Lookup(KEY_ID)->ToString();
	String^ rev = GetDocumentVersion(docid);
	if (!rev->IsEmpty()) {
		if (oMap->HasKey(KEY_REV)) {
			oMap->Remove(KEY_REV);
		}
		oMap->Insert(KEY_REV, rev);
	}
	return UpdateDocument(oMap);
}//MaintainsDocument
bool CouchDBProxy::UpdateDocument(IMap<String^, Object^>^ oMap) {
	if (!oMap->HasKey(KEY_ID)) {
		return false;
	}
	String^ docid = oMap->Lookup(KEY_ID)->ToString();
	String^ rev = GetDocumentVersion(docid);
	if (rev->IsEmpty()) {
		return false;
	}
	String^ sUri = this->m_url + m_database + "/" + Uri::EscapeComponent(docid) + "?rev=" + rev;
	Uri^ uri = ref new Uri(sUri);
	String^ sp = MapToJson(oMap);
	HttpStringContent^ sc = ref new HttpStringContent(sp);
	sc->Headers->ContentType->MediaType = JSON_MIME_TYPE;
	auto myop = this->m_client->PutAsync(uri, sc);
	auto operationTask = create_task(myop);
	bool bRet = operationTask.then([](HttpResponseMessage^ response) {
		bool b = false;
		auto status = response->StatusCode;
		if ((status == HttpStatusCode::Created) || (status == HttpStatusCode::Accepted)) {
			b = true;
		}
		return (b);
	}).get();
	return (bRet);
}//UpdateDocument
bool CouchDBProxy::InsertDocument(IMap<String^, Object^>^ oMap) {
	if (oMap->HasKey(KEY_REV)) {
		oMap->Remove(KEY_REV);
	}
	String^ sUri = this->m_url + m_database;
	Uri^ uri = ref new Uri(sUri);
	String^ sp = MapToJson(oMap);
	HttpStringContent^ sc = ref new HttpStringContent(sp);
	sc->Headers->ContentType->MediaType = JSON_MIME_TYPE;
	auto myop = this->m_client->PostAsync(uri, sc);
	auto operationTask = create_task(myop);
	bool bRet = operationTask.then([](HttpResponseMessage^ response) {
		bool b = false;
		auto status = response->StatusCode;
		if ((status == HttpStatusCode::Created) || (status == HttpStatusCode::Accepted)) {
			b = true;
		}
		return (b);
	}).get();
	return (bRet);
}//InsertDocument
IMap<String^, Object^>^  CouchDBProxy::ReadDocumentById(String^ docid) {
	String^ sUri = this->m_url + m_database + "/" + Uri::EscapeComponent(docid);
	Uri^ uri = ref new Uri(sUri);
	auto myop = m_client->GetAsync(uri);
	auto operationTask = create_task(myop);
	return operationTask.then([](HttpResponseMessage^ response) {
		IMap<String^, Object^>^ oRet;
		auto status = response->StatusCode;
		if ((status == HttpStatusCode::Ok) || (status == HttpStatusCode::NotModified)) {
			auto myopx = response->Content->ReadAsStringAsync();
			create_task(myopx).then([&oRet](String^ jsonText) {
				oRet = ref new Map<String^, Object^>();
				JsonObject^ obj = JsonObject::Parse(jsonText);
				auto it = obj->First();
				while (it->HasCurrent) {
					auto p = it->Current;
					String^ key = p->Key;
					Object^ o = ConvertJsonObject(p->Value);
					oRet->Insert(key, o);
					it->MoveNext();
				}// it
			}).wait();
		}
		return (oRet);
	}).get();
}// ReadDocumentById
/////////////////////////////////////////////
String^  CouchDBProxy::GetDocumentVersion(String^ docid) {
	String^ sUri = this->m_url + m_database + "/" + Uri::EscapeComponent(docid);
	Uri^ uri = ref new Uri(sUri);
	HttpRequestMessage^ req = ref new HttpRequestMessage(HttpMethod::Head, uri);
	auto myop = this->m_client->SendRequestAsync(req);
	auto operationTask = create_task(myop);
	String^ sRet = operationTask.then([](HttpResponseMessage^ response) {
		String^ srev;
		auto status = response->StatusCode;
		if (status == HttpStatusCode::Ok) {
			if (response->Headers->HasKey(KEY_ETAG)) {
				String^ s = response->Headers->Lookup(KEY_ETAG);
				if (s->Length() > 2) {
					std::wstring ss(s->Data());
					auto it1 = ss.begin();
					auto it2 = ss.end();
					it1++;
					it2--;
					std::wstring sx(it1, it2);
					srev = ref new String(sx.c_str());
				}// lenftg
			}// header ETag
		}// found
		return srev;
	}).get();
	return (sRet);
}//GetDocumentVersion
///////////////////////////////////////////
bool CouchDBProxy::IsAlive(void) {
	bool bRet = false;
	try {
		String^ sUri = this->m_url + m_database;
		Uri^ uri = ref new Uri(sUri);
		HttpRequestMessage^ req = ref new HttpRequestMessage(HttpMethod::Head, uri);
		auto myop = this->m_client->SendRequestAsync(req);
		auto operationTask = create_task(myop);
		bRet = operationTask.then([this](HttpResponseMessage^ response) {
			auto status = response->StatusCode;
			return (status == HttpStatusCode::Ok);
		}).get();
	}
	catch (Exception ^e) {
		(void)e;
	}
	return (bRet);
}// IsDatabaseExists
////////////////////////////////////////
int CouchDBProxy::GetCountFilter(IMap<String^, Object^>^ oFetch) {
	String^ sUri = this->m_url + m_database + STRING_FIND;
	Uri^ uri = ref new Uri(sUri);
	int nTotal = 0;
	int nCount = 128;
	int nOffset = 0;
	bool done = false;
	IVector<String^>^ pFields = ref new Vector<String^>();
	pFields->Append(KEY_ID);
	do {
		String^ sp = ConvertFindFilter(oFetch, pFields, nOffset, nCount);
		HttpStringContent^ sc = ref new HttpStringContent(sp);
		sc->Headers->ContentType->MediaType = JSON_MIME_TYPE;
		auto myop = this->m_client->PostAsync(uri, sc);
		auto operationTask = create_task(myop);
		int nx = operationTask.then([](HttpResponseMessage^ response) {
			int nRet = 0;
			auto status = response->StatusCode;
			if (status == HttpStatusCode::Ok) {
				try {
					auto myopx = response->Content->ReadAsStringAsync();
					create_task(myopx).then([&nRet](String^ jsonText) {
						JsonValue^ val = JsonValue::Parse(jsonText);
						if (val->ValueType == JsonValueType::Object) {
							JsonObject^ obj = val->GetObject();
							if (obj->HasKey(KEY_DOCS)) {
								IJsonValue^ vx = obj->Lookup(KEY_DOCS);
								if (vx->ValueType == JsonValueType::Array) {
									JsonArray^ oAr = vx->GetArray();
									nRet = (int)oAr->Size;
								}
							}// docs
						}// obj
					}).get();
				}
				catch (Exception^ e) {
					(void)e;
				}
			}// status
			return (nRet);
		}).get();
		if (nx > 0) {
			nTotal += nx;
			nOffset += nx;
		}
		if (nx < nCount) {
			done = true;
			break;
		}
	} while (!done);
	return (nTotal);
}//GetCountFilter
IMap<String^, Object^>^ CouchDBProxy::FindDocument(IMap<String^, Object^>^ oFetch) {
	IMap<String^, Object^>^ oRet;
	IVector<IMap<String^, Object^>^>^ vv = this->ReadDocuments(oFetch, 0, 1);
	if (vv != nullptr) {
		if (vv->Size > 0) {
			auto it = vv->First();
			if (it->HasCurrent) {
				oRet = it->Current;
			}
		}
	}
	return (oRet);
}//FindDocu
IVector<IMap<String^, Object^>^>^ CouchDBProxy::ReadDocuments(IMap<String^, Object^>^ oFetch, int offset /*= 0*/, int count /*= 0*/) {
	String^ sUri = this->m_url + m_database + STRING_FIND;
	Uri^ uri = ref new Uri(sUri);
	IVector<String^>^ pFields = ref new Vector<String^>();
	String^ sp = ConvertFindFilter(oFetch, pFields, offset, count);
	HttpStringContent^ sc = ref new HttpStringContent(sp);
	sc->Headers->ContentType->MediaType = JSON_MIME_TYPE;
	auto myop = this->m_client->PostAsync(uri, sc);
	auto operationTask = create_task(myop);
	return operationTask.then([](HttpResponseMessage^ response) {
		IVector<IMap<String^, Object^>^>^ oRet;
		auto status = response->StatusCode;
		if (status == HttpStatusCode::Ok) {
			try {
				auto myopx = response->Content->ReadAsStringAsync();
				create_task(myopx).then([&oRet](String^ jsonText) {
					oRet = ref new Vector<IMap<String^, Object^>^>();
					JsonValue^ val = JsonValue::Parse(jsonText);
					if (val->ValueType == JsonValueType::Object) {
						JsonObject^ obj = val->GetObject();
						if (obj->HasKey(KEY_DOCS)) {
							IJsonValue^ vx = obj->Lookup(KEY_DOCS);
							if (vx->ValueType == JsonValueType::Array) {
								JsonArray^ oAr = vx->GetArray();
								auto it = oAr->First();
								while (it->HasCurrent) {
									IJsonValue^ v = it->Current;
									if (v->ValueType == JsonValueType::Object) {
										IMap<String^, Object^>^ cur = ref new Map<String^, Object^>();
										JsonObject^ obj = v->GetObject();
										auto jt = obj->First();
										while (jt->HasCurrent) {
											auto p = jt->Current;
											String^ key = p->Key;
											Object^ o = ConvertJsonObject(p->Value);
											cur->Insert(key, o);
											jt->MoveNext();
										}// jt
										oRet->Append(cur);
									}// obj
									it->MoveNext();
								}// it
							}// array
						}// docs
					}// obj
				}).get();
			}
			catch (Exception^ e) {
				(void)e;
			}
		}// status
		return (oRet);
	}).get();
}//ReadDocuments
/////////////////////////////////////
//
IJsonValue^ CouchDBProxy::ConvertObject(Object^ obj) {
	if (obj == nullptr) {
		return JsonValue::CreateNullValue();
	}
	
	auto tx = Type::GetTypeCode(obj->GetType());
	bool bval = true;
	int ival = 12;
	double dval = 0.78;
	String^ sval = "yy";

	if (tx == Type::GetTypeCode(bval.GetType())) {
		bval = (bool)obj;
		return JsonValue::CreateBooleanValue(bval);
	}
	else if (tx == Type::GetTypeCode(ival.GetType())) {
		int ival = (int)obj;
		return JsonValue::CreateNumberValue((double)ival);
	}
	else if (tx == Type::GetTypeCode(dval.GetType())) {
		double dval = (double)obj;
		return JsonValue::CreateNumberValue(dval);
	}
	else if (tx == Type::GetTypeCode(sval->GetType())) {
		String^ s = (String^)obj;
		return JsonValue::CreateStringValue(s);
	}
	else {
		String^ sval = dynamic_cast<String^>(obj);
		if (sval != nullptr) {
			return JsonValue::CreateStringValue(sval);
		}
		IVector<Object^>^ vv = dynamic_cast<IVector<Object^>^>(obj);
		if (vv != nullptr) {
			JsonArray^ oAr = ref new JsonArray();
			auto it = vv->First();
			while (it->HasCurrent) {
				Object^ xo = it->Current;
				IJsonValue^ vz = ConvertObject(xo);
				oAr->Append(vz);
				it->MoveNext();
			}// it
			return (oAr);
		}
		IMap<String^, Object^>^ vva = dynamic_cast<IMap<String^, Object^> ^>(obj);
		if (vva != nullptr) {
			JsonObject^ oAr = ref new JsonObject();
			auto it = vva->First();
			while (it->HasCurrent) {
				auto p = it->Current;
				Object^ xo = p->Value;
				String^ key = p->Key;
				IJsonValue^ vz = ConvertObject(xo);
				oAr->Insert(key, vz);
				it->MoveNext();
			}// it
			return (oAr);
		}
	}
	return JsonValue::CreateNullValue();
}// ConvertObject
String^ CouchDBProxy::ConvertFindFilter(IMap<String^, Object^>^ oFetch,
	IVector<String^>^ oFields, int skip /*= 0*/, int count /*= 16*/) {
	JsonObject^ oRet = ref new JsonObject();
	JsonObject^ oSel = ref new JsonObject();
	{
		auto it = oFetch->First();
		while (it->HasCurrent) {
			auto p = it->Current;
			String^ key = p->Key;
			IJsonValue^ val = ConvertObject(p->Value);
			oSel->Insert(key, val);
			it->MoveNext();
		}// it
	}
	oRet->Insert("selector", oSel);
	if (oFields->Size > 0) {
		JsonArray^ oAr = ref new JsonArray();
		auto it = oFields->First();
		while (it->HasCurrent) {
			String^ key = it->Current;
			if (!key->IsEmpty()) {
				oAr->Append(JsonValue::CreateStringValue(key));
			}
			it->MoveNext();
		}// it
		if (oAr->Size > 0) {
			oRet->Insert("fields", oAr);
		}
	}
	if (skip > 0) {
		oRet->Insert("skip", JsonValue::CreateNumberValue(skip));
	}
	if (count > 0) {
		oRet->Insert("limit", JsonValue::CreateNumberValue(count));
	}
	return (oRet->Stringify());
}//ConvertFindFilter
String^ CouchDBProxy::MapToJson(IMap<String^, Object^>^ oMap) {
	JsonObject^ oRet = ref new JsonObject();
	auto it = oMap->First();
	while (it->HasCurrent) {
		auto p = it->Current;
		String^ key = p->Key;
		IJsonValue^ val = ConvertObject(p->Value);
		oRet->Insert(key, val);
		it->MoveNext();
	}// it
	return (oRet->Stringify());
}//MapToJson
String^ CouchDBProxy::MapToJson(IVector<IMap<String^, Object^>^>^ oAr) {
	JsonArray^ oRet = ref new JsonArray();
	auto it = oAr->First();
	while (it->HasCurrent) {
		IMap<String^, Object^>^ p = it->Current;
		JsonObject^ o = ref new JsonObject();
		auto jt = p->First();
		while (jt->HasCurrent) {
			auto px = jt->Current;
			String^ key = px->Key;
			IJsonValue^ val = ConvertObject(px->Value);
			o->Insert(key, val);
			jt->MoveNext();
		}// jt
		oRet->Append(o);
		it->MoveNext();
	}// it
	return (oRet->Stringify());
}//MapToJson
Object^CouchDBProxy::ConvertJsonObject(IJsonValue^ jsonVal) {
	if (jsonVal == nullptr) {
		Object^ o;
		return o;
	}
	if (jsonVal->ValueType == JsonValueType::Boolean) {
		return  jsonVal->GetBoolean();
	}
	else if (jsonVal->ValueType == JsonValueType::Number) {
		return jsonVal->GetNumber();
	}
	else if (jsonVal->ValueType == JsonValueType::String) {
		return jsonVal->GetString();
	}
	else if (jsonVal->ValueType == JsonValueType::Array) {
		IVector<Object^>^ oRet = ref new Vector<Object^>();
		JsonArray^ oAr = jsonVal->GetArray();
		auto it = oAr->First();
		while (it->HasCurrent) {
			Object^ o = ConvertJsonObject(it->Current);
			oRet->Append(o);
			it->MoveNext();
		}// it
		return (oRet);
	}
	else if (jsonVal->ValueType == JsonValueType::Object) {
		IMap<String^, Object^>^ oRet = ref new Map<String^, Object^>();
		JsonObject^ obj = jsonVal->GetObject();
		auto it = obj->First();
		while (it->HasCurrent) {
			auto p = it->Current;
			String^ key = p->Key;
			Object^ o = ConvertJsonObject(p->Value);
			oRet->Insert(key, o);
			it->MoveNext();
		}// it
		return (oRet);
	}
	else {
		Object^ o;
		return o;
	}
}//ConvertJsonObject
byte *CouchDBProxy::GetPointerToData(IBuffer^ pixelBuffer, unsigned int *length) {
	if (length != nullptr)
	{
		*length = pixelBuffer->Length;
	}
	// Query the IBufferByteAccess interface.
	ComPtr<IBufferByteAccess> bufferByteAccess;
	reinterpret_cast<IInspectable*>(pixelBuffer)->QueryInterface(IID_PPV_ARGS(&bufferByteAccess));
	// Retrieve the buffer data.
	byte* pixels = nullptr;
	bufferByteAccess->Buffer(&pixels);
	return pixels;
}//GetPointerToData
