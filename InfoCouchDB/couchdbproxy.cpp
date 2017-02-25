#include "pch.h"
#include "couchdbproxy.h"
//////////////////////////////
using namespace Platform::Collections;
using namespace Windows::Web::Http;
using namespace Windows::Web::Http::Headers;
using namespace InfoCouchDB;
using namespace concurrency;
/////////////////////////
String^ CouchDBProxy::SLASH = "/";
String^ CouchDBProxy::REV = "?rev=";
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
String^ CouchDBProxy::ARG_ATTACHMENTS = "?attachments=true";
String^  CouchDBProxy::STRING_INDEX_URI = "/_index";
//
String^ CouchDBProxy::KEY_SELECTOR = "selector";
String^ CouchDBProxy::KEY_FIELDS = "fields";
String^ CouchDBProxy::KEY_LIMIT = "limit";
String^ CouchDBProxy::KEY_SKIP = "skip";
//
String^  CouchDBProxy::KEY_INDEX = "index";
String^  CouchDBProxy::KEY_DESIGNDOC = "ddoc";
String^  CouchDBProxy::KEY_NAME = "name";
///////////////////////
CouchDBProxy::CouchDBProxy(String^ url, String^ database) {
	m_url = url;
	m_database = database;
	m_client = ref new HttpClient();
	m_client->DefaultRequestHeaders->Accept->Clear();
	m_client->DefaultRequestHeaders->Accept->Append(ref new HttpMediaTypeWithQualityHeaderValue(JSON_MIME_TYPE));
	this->check_database();
}
CouchDBProxy::~CouchDBProxy() {
	m_url = nullptr;
	m_database = nullptr;
	m_client = nullptr;
}
void CouchDBProxy::check_database(void) {
	String^ sUri = this->m_url + this->m_database;
	Uri^ uri = ref new Uri(sUri);
	HttpRequestMessage^ req = ref new HttpRequestMessage(HttpMethod::Head, uri);
	auto myop = m_client->SendRequestAsync(req);
	bool bRet = create_task(myop).then([](HttpResponseMessage^ response) {
		auto status = response->StatusCode;
		if ((status != HttpStatusCode::NotFound) && (status != HttpStatusCode::Ok)) {
			throw ref new OperationCanceledException();
		}
		return task_from_result(status == HttpStatusCode::Ok);
	}).get();
	if (!bRet) {
		HttpRequestMessage^ req2 = ref new HttpRequestMessage(HttpMethod::Put, uri);
		auto myop2 = m_client->SendRequestAsync(req2);
		create_task(myop2).then([](HttpResponseMessage^ response) {
			auto status = response->StatusCode;
			if (status != HttpStatusCode::Created) {
				throw ref new OperationCanceledException();
			}
		}).wait();
	}
}
////////////////////////////////////////////////
task<IBuffer^> CouchDBProxy::GetDocumentAttachmentDataAsync(String^ docid, String^ attachmentName) {
	return task<IBuffer^>{[this, docid, attachmentName]()->IBuffer^ {
		String^ rev = this->GetDocumentVersionAsync(docid).get();
		IBuffer^ oRet;
		if ((rev == nullptr) || rev->IsEmpty()) {
			return oRet;
		}
		String^ sUri = this->m_url + this->m_database + SLASH + Uri::EscapeComponent(docid) + SLASH + Uri::EscapeComponent(attachmentName) + REV + rev;
		Uri^ uri = ref new Uri(sUri);
		auto myop = this->m_client->GetAsync(uri);
		auto operationTask = create_task(myop);
		oRet = create_task(myop).then([](HttpResponseMessage^ response)->IBuffer^ {
			IBuffer^ r;
			auto status = response->StatusCode;
			if ((status == HttpStatusCode::NotModified) || (status == HttpStatusCode::Ok)) {
				r = create_task(response->Content->ReadAsBufferAsync()).get();
			}// ok
			return (r);
		}).get();
		return (oRet);
	}};
}//GetDocumentAttachmentData
task<bool> CouchDBProxy::RemoveDocumentAttachmentAsync(String^ docid, String^ attachmentName) {
	return task<bool>{[this, docid, attachmentName]()->bool {
		bool bRet{ false };
		String^ rev = this->GetDocumentVersionAsync(docid).get();
		if ((rev == nullptr) || rev->IsEmpty()) {
			return false;
		}
		String^ sUri = this->m_url + this->m_database + SLASH + Uri::EscapeComponent(docid) + SLASH + Uri::EscapeComponent(attachmentName) + REV + rev;
		Uri^ uri = ref new Uri(sUri);
		auto myop = this->m_client->DeleteAsync(uri);
		auto operationTask = create_task(myop);
		bRet = operationTask.then([](HttpResponseMessage^ response) {
			bool b = false;
			auto status = response->StatusCode;
			if ((status == HttpStatusCode::Accepted) || (status == HttpStatusCode::Ok)) {
				b = true;
			}
			return task_from_result(b);
		}).get();
		return (bRet);
	}};
}//RemoveDocumentAttachment
task<IMap<String^, String^>^> CouchDBProxy::GetDocumentAttachmentNamesAsync(String^ docid) {
	return task<IMap<String^, String^>^>{[this, docid]()->IMap<String^, String^>^ {
		String^ sUri = this->m_url + this->m_database + SLASH + Uri::EscapeComponent(docid) + ARG_ATTACHMENTS;
		Uri^ uri = ref new Uri(sUri);
		auto myop = this->m_client->GetAsync(uri);
		auto operationTask = create_task(myop);
		IMap<String^, String^>^ pRet = operationTask.then([](HttpResponseMessage^ response) {
			IMap<String^, String^>^ oRet;
			auto status = response->StatusCode;
			if ((status == HttpStatusCode::Ok) || (status == HttpStatusCode::NotModified)) {
				String^ json = create_task(response->Content->ReadAsStringAsync()).get();
				oRet = StReadNamesMimes(json);
			}
			return task_from_result(oRet);
		}).get();
		return (pRet);
	}};
}//GetDocumentAttachmentNames
task<bool> CouchDBProxy::MaintainsDocumentAttachmentAsync(String^ docid, String^ attachmentName,
	IStorageFile^ file) {
	return task<bool>{[this, docid, attachmentName, file]()->bool {
		bool bRet = create_task(file->OpenReadAsync()).
			then([this, docid, attachmentName](IRandomAccessStreamWithContentType^ stream) {
			String^ mime = stream->ContentType;
			unsigned int  n = static_cast<unsigned int>(stream->Size);
			IInputStream^ ss = stream->GetInputStreamAt(0);
			DataReader^ reader = ref new DataReader(ss);
			create_task(reader->LoadAsync(n)).wait();
			IBuffer^ pBuf = reader->DetachBuffer();
			bool b = this->MaintainsDocumentAttachmentAsync(docid, attachmentName, mime, pBuf).get();
			return (b);
		}).get();
		return (bRet);
	}};

}//MaintainsDocumentAttachment
task<bool> CouchDBProxy::MaintainsDocumentAttachmentAsync(String^ docid, String^ attachmentName, String^ mimetype, IBuffer^ data) {
	return task<bool>{[this, docid, attachmentName, mimetype, data]()->bool {
		String^ rev = this->GetDocumentVersionAsync(docid).get();
		if ((rev == nullptr) || rev->IsEmpty() || (data->Length < 1)) {
			throw ref new InvalidArgumentException();
		}
		HttpBufferContent^ sc = ref new HttpBufferContent(data);
		sc->Headers->ContentType->MediaType = mimetype;
		String^ sUri = this->m_url + this->m_database + SLASH + Uri::EscapeComponent(docid) + SLASH + Uri::EscapeComponent(attachmentName) + REV + rev;
		Uri^ uri = ref new Uri(sUri);
		auto myop = this->m_client->PutAsync(uri, sc);
		bool bRet = create_task(myop).then([](HttpResponseMessage^ response) {
			bool b = false;
			auto status = response->StatusCode;
			if ((status == HttpStatusCode::Accepted) || (status == HttpStatusCode::Ok)) {
				b = true;
			}
			return task_from_result(b);
		}).get();
		return bRet;
	}};
}// MaintainsDocumentAttachmentAsync
task<bool> CouchDBProxy::RemoveDocumentsAsync(IMap<String^, Object^>^ oFetch) {
	return create_task([this, oFetch]()->bool {
		String^ sUri = this->m_url + this->m_database + STRING_FIND;
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
			int nx = operationTask.then([this](HttpResponseMessage^ response) {
				int nRet = 0;
				auto status = response->StatusCode;
				if (status == HttpStatusCode::Ok) {
					auto myopx = response->Content->ReadAsStringAsync();
					create_task(myopx).then([this, &nRet](String^ jsonText) {
						JsonArray^ oRetArray = ref new JsonArray();
						JsonValue^ val = JsonValue::Parse(jsonText);
						if (val->ValueType == JsonValueType::Object) {
							JsonObject^ obj = val->GetObject();
							if (obj->HasKey(KEY_DOCS)) {
								IJsonValue^ vx = obj->Lookup(KEY_DOCS);
								if (vx->ValueType == JsonValueType::Array) {
									JsonArray^ oAr = vx->GetArray();
									nRet = (int)oAr->Size;
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
								}// array
							}// docs
						}// obj
						if (oRetArray->Size > 0) {
							JsonObject^ queryObj = ref new JsonObject();
							queryObj->Insert(KEY_DOCS, oRetArray);
							String^ sp = queryObj->Stringify();
							String^ sUri2 = m_url + m_database + STRING_BULKDOCS;
							Uri^ uri2 = ref new Uri(sUri2);
							HttpStringContent^ sc = ref new HttpStringContent(sp);
							sc->Headers->ContentType->MediaType = JSON_MIME_TYPE;
							auto myop = this->m_client->PostAsync(uri2, sc);
							auto operationTask = create_task(myop);
							bool bRet = operationTask.then([](HttpResponseMessage^ response) {
								bool b = false;
								auto status = response->StatusCode;
								if ((status == HttpStatusCode::Created) || (status == HttpStatusCode::Ok)) {
									b = true;
								}
								return (b);
							}).get();
						}// size
						return task_from_result(nRet);
					}).get();
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
		return true;
	});
}
task<bool> CouchDBProxy::CreateIndexAsync(IVector<String^>^ fields, String ^ name, String ^ designDoc)
{
	return task<bool>{[this, fields, name, designDoc]()->bool {
		if (fields == nullptr) {
			throw ref new InvalidArgumentException();
		}
		if (fields->Size < 1) {
			throw ref new InvalidArgumentException();
		}
		JsonObject^ oRet = ref new JsonObject();
		JsonArray^ oAr = ref new JsonArray();
		auto it = fields->First();
		while (it->HasCurrent) {
			String^ s = it->Current;
			if ((s == nullptr) || s->IsEmpty()) {
				throw ref new InvalidArgumentException();
			}
			oAr->Append(JsonValue::CreateStringValue(s));
			it->MoveNext();
		}// it
		JsonObject^ oIndex = ref new JsonObject();
		oIndex->Insert(KEY_FIELDS, oAr);
		oRet->Insert(KEY_INDEX, oIndex);
		if ((name != nullptr) && (!name->IsEmpty())) {
			oRet->Insert(KEY_NAME, JsonValue::CreateStringValue(name));
		}
		if ((designDoc != nullptr) && (!designDoc->IsEmpty())) {
			oRet->Insert(KEY_DESIGNDOC, JsonValue::CreateStringValue(designDoc));
		}
		String^ sp = oRet->Stringify();
		String^ sUri = this->m_url + this->m_database + STRING_INDEX_URI;
		Uri^ uri = ref new Uri(sUri);
		HttpStringContent^ sc = ref new HttpStringContent(sp);
		sc->Headers->ContentType->MediaType = JSON_MIME_TYPE;
		auto myop = this->m_client->PostAsync(uri, sc);
		auto operationTask = create_task(myop);
		bool bRet = operationTask.then([](HttpResponseMessage^ response) {
			bool b = false;
			auto status = response->StatusCode;
			if (status == HttpStatusCode::Ok) {
				b = true;
			}
			return task_from_result(b);
		}).get();
		return bRet;
	}};
}// CreateIndex
//RemoveDocumentsAsync
task<bool> CouchDBProxy::MaintainsDocumentsAsync(IVector<IMap<String^, Object^>^>^ oVec, bool bDelete /*= false*/) {
	return task<bool>{[this, oVec, bDelete]()->bool {
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
						String^ rev = this->GetDocumentVersionAsync(docid).get();
						if ((rev != nullptr) && (!rev->IsEmpty())) {
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
		return operationTask.then([](HttpResponseMessage^ response) {
			bool b = false;
			auto status = response->StatusCode;
			if ((status == HttpStatusCode::Created) || (status == HttpStatusCode::Ok)) {
				b = true;
			}
			return b;
		}).get();
	}};

}// MaiSntainsDocuments
//////////////////////////////////////////////
task<bool>  CouchDBProxy::DeleteDocumentByIdAsync(String^ docid) {
	return task<bool>{[this, docid]()->bool {
		return this->GetDocumentVersionAsync(docid).then([this, docid](String^ rev) {
			if ((rev == nullptr) || (rev->IsEmpty())) {
				throw ref new InvalidArgumentException();
			}
			String^ sUri = this->m_url + m_database + SLASH + Uri::EscapeComponent(docid) + REV + rev;
			Uri^ uri = ref new Uri(sUri);
			auto myop = this->m_client->DeleteAsync(uri);
			return create_task(myop);
		}).then([](HttpResponseMessage^ response) {
			bool b = false;
			auto status = response->StatusCode;
			if ((status == HttpStatusCode::Ok) || (status == HttpStatusCode::Accepted)) {
				b = true;
			}
			return task_from_result(b);
		}).get();
	}};
}//DeleteDocumentByIdAsync
task<bool> CouchDBProxy::MaintainsDocumentAsync(IMap<String^, Object^>^ oMap) {
	if (!oMap->HasKey(KEY_ID)) {
		return this->InsertDocumentAsync(oMap);
	}
	return this->UpdateDocumentAsync(oMap);
}//MaintainsDocumentAsync
task<bool> CouchDBProxy::UpdateDocumentAsync(IMap<String^, Object^>^ oMap) {
	return task<bool>{[this, oMap]()->bool {
		if (!oMap->HasKey(KEY_ID)) {
			return (false);
		}
		if (oMap->HasKey(KEY_REV)) {
			oMap->Remove(KEY_REV);
		}
		String^ docid = oMap->Lookup(KEY_ID)->ToString();
		return this->GetDocumentVersionAsync(docid).then([this, docid, oMap](String^ rev) {
			if ((rev == nullptr) || (rev->IsEmpty())) {
				throw ref new InvalidArgumentException();
			}
			String^ sUri = this->m_url + this->m_database + SLASH + Uri::EscapeComponent(docid) + REV + rev;
			Uri^ uri = ref new Uri(sUri);
			String^ sp = MapToJson(oMap);
			HttpStringContent^ sc = ref new HttpStringContent(sp);
			sc->Headers->ContentType->MediaType = JSON_MIME_TYPE;
			auto myop = this->m_client->PutAsync(uri, sc);
			return create_task(myop);
		}).then([](HttpResponseMessage^ response) {
			bool bRet = false;
			auto status = response->StatusCode;
			if ((status == HttpStatusCode::Created) || (status == HttpStatusCode::Accepted)) {
				bRet = true;
			}
			return task_from_result(bRet);
		}).get();
	}};
}//UpdateDocumentAsync
task<bool> CouchDBProxy::InsertDocumentAsync(IMap<String^, Object^>^ oMap) {
	return task<bool>{[this, oMap]()->bool {
		if (oMap->HasKey(KEY_REV)) {
			oMap->Remove(KEY_REV);
		}
		String^ sUri = this->m_url + this->m_database;
		Uri^ uri = ref new Uri(sUri);
		String^ sp = MapToJson(oMap);
		HttpStringContent^ sc = ref new HttpStringContent(sp);
		sc->Headers->ContentType->MediaType = JSON_MIME_TYPE;
		auto myop = this->m_client->PostAsync(uri, sc);
		auto operationTask = create_task(myop);
		return operationTask.then([](HttpResponseMessage^ response) {
			bool b = false;
			auto status = response->StatusCode;
			if ((status == HttpStatusCode::Created) || (status == HttpStatusCode::Accepted)) {
				b = true;
			}
			return task_from_result(b);
		}).get();
	}};
}//InsertDocumentAsync
task<IMap<String^, Object^>^> CouchDBProxy::ReadDocumentByIdAsync(String^ docid) {
	return task<IMap<String^, Object^>^>{[this, docid]()->IMap<String^, Object^>^ {
		String^ sUri = this->m_url + m_database + SLASH + Uri::EscapeComponent(docid);
		Uri^ uri = ref new Uri(sUri);
		auto myop = m_client->GetAsync(uri);
		auto operationTask = create_task(myop);
		return operationTask.then([](HttpResponseMessage^ response) {
			auto status = response->StatusCode;
			if ((status != HttpStatusCode::NotModified) && (status != HttpStatusCode::Ok)) {
				String^ s;
				return task_from_result(s);
			}
			else {
				return create_task(response->Content->ReadAsStringAsync());
			}
		}).then([](String^ jsonText) {
			IMap<String^, Object^>^ oRet;
			if ((jsonText != nullptr) && (!jsonText->IsEmpty())) {
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
			}
			return task_from_result(oRet);
		}).get();
	}};

}//ReadDocumentByIdAsync
/////////////////////////////////////////////
task<String^>  CouchDBProxy::GetDocumentVersionAsync(String^ docid) {
	return task<String^>{[this, docid]()->String^ {
		String^ sUri = this->m_url + this->m_database + SLASH + Uri::EscapeComponent(docid);
		Uri^ uri = ref new Uri(sUri);
		HttpRequestMessage^ req = ref new HttpRequestMessage(HttpMethod::Head, uri);
		auto myop = this->m_client->SendRequestAsync(req);
		auto operationTask = create_task(myop);
		return operationTask.then([](HttpResponseMessage^ response) {
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
			return task_from_result(srev);
		}).get();
	}};
}//GetDocumentVersionAsync
///////////////////////////////////////////
task<bool> CouchDBProxy::IsAliveAsync(void) {
	return task<bool>{[this]()->bool {
		String^ sUri = this->m_url + this->m_database;
		Uri^ uri = ref new Uri(sUri);
		HttpRequestMessage^ req = ref new HttpRequestMessage(HttpMethod::Head, uri);
		auto myop = this->m_client->SendRequestAsync(req);
		auto operationTask = create_task(myop);
		return operationTask.then([this](HttpResponseMessage^ response) {
			auto status = response->StatusCode;
			return task_from_result(status == HttpStatusCode::Ok);
		}).get();
	}};
}// IsAliveAsync
////////////////////////////////////////
task<int> CouchDBProxy::GetCountFilterAsync(IMap<String^, Object^>^ oFetch) {
	return task<int>{[this, oFetch]()->int {
		String^ sUri = this->m_url + this->m_database + STRING_FIND;
		Uri^ uri = ref new Uri(sUri);
		int nTotal{ 0 };
		int nCount{ 128 };
		int nOffset{ 0 };
		bool done{ false };
		IVector<String^>^ pFields = ref new Vector<String^>();
		pFields->Append(KEY_ID);
		do {
			String^ sp = ConvertFindFilter(oFetch, pFields, nOffset, nCount);
			HttpStringContent^ sc = ref new HttpStringContent(sp);
			sc->Headers->ContentType->MediaType = JSON_MIME_TYPE;
			auto operationTask = create_task(this->m_client->PostAsync(uri, sc));
			int nx = operationTask.then([](HttpResponseMessage^ response)->int {
				int nRet{ 0 };
				auto status = response->StatusCode;
				if (status == HttpStatusCode::Ok) {
					auto myopx = response->Content->ReadAsStringAsync();
					nRet = create_task(myopx).then([](String^ jsonText)->int {
						return StDocsCount(jsonText);
					}).get();
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
		return nTotal;
	}};
}//GetCountFilterAsync
task<IMap<String^, Object^>^> CouchDBProxy::FindDocumentAsync(IMap<String^, Object^>^ oFetch) {
	return task<IMap<String^, Object^>^>{[this, oFetch]()->IMap<String^, Object^>^ {
		return this->ReadDocumentsAsync(oFetch, 0, 1).then([](IVector<IMap<String^, Object^>^>^ vv) {
			IMap<String^, Object^>^ oRet;
			if (vv != nullptr) {
				if (vv->Size > 0) {
					auto it = vv->First();
					if (it->HasCurrent) {
						oRet = it->Current;
					}
				}
			}
			return task_from_result(oRet);
		}).get();
	}};
}//FindDocumentAsync
task<IVector<IMap<String^, Object^>^>^> CouchDBProxy::ReadDocumentsAsync(IMap<String^, Object^>^ oFetch, int offset, int count) {
	return task<IVector<IMap<String^, Object^>^>^>{[this,oFetch,offset,count]()->IVector<IMap<String^, Object^>^>^ {
		String^ sUri = this->m_url + this->m_database + STRING_FIND;
		Uri^ uri = ref new Uri(sUri);
		IVector<String^>^ pFields = ref new Vector<String^>();
		String^ sp = ConvertFindFilter(oFetch, pFields, offset, count);
		HttpStringContent^ sc = ref new HttpStringContent(sp);
		sc->Headers->ContentType->MediaType = JSON_MIME_TYPE;
		auto myop = this->m_client->PostAsync(uri, sc);
		auto operationTask = create_task(myop);
		return operationTask.then([](HttpResponseMessage^ response) {
			auto status = response->StatusCode;
			if (status != HttpStatusCode::Ok) {
				String^ s;
				return task_from_result(s);
			}
			else {
				return create_task(response->Content->ReadAsStringAsync());
			}
		}).then([](String^ jsonText) {
			IVector<IMap<String^, Object^>^>^ xRet = StReadDocs(jsonText);
			return task_from_result(xRet);
		}).get();
	}};
}//ReadDocumentsAsync
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
	if (oFetch != nullptr) {
		auto it = oFetch->First();
		while (it->HasCurrent) {
			auto p = it->Current;
			String^ key = p->Key;
			IJsonValue^ val = ConvertObject(p->Value);
			oSel->Insert(key, val);
			it->MoveNext();
		}// it
	}// oFetch
	oRet->Insert(KEY_SELECTOR, oSel);
	if ((oFields != nullptr) && (oFields->Size > 0)) {
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
			oRet->Insert(KEY_FIELDS, oAr);
		}
	}
	if (skip > 0) {
		oRet->Insert(KEY_SKIP, JsonValue::CreateNumberValue(skip));
	}
	if (count > 0) {
		oRet->Insert(KEY_LIMIT, JsonValue::CreateNumberValue(count));
	}
	return (oRet->Stringify());
}//ConvertFindFilter
String^ CouchDBProxy::MapToJson(IMap<String^, Object^>^ oMap) {
	JsonObject^ oRet = ref new JsonObject();
	if (oMap != nullptr) {
		auto it = oMap->First();
		while (it->HasCurrent) {
			auto p = it->Current;
			String^ key = p->Key;
			IJsonValue^ val = ConvertObject(p->Value);
			oRet->Insert(key, val);
			it->MoveNext();
		}// it
	}// nullptr
	return (oRet->Stringify());
}//MapToJson
String^ CouchDBProxy::MapToJson(IVector<IMap<String^, Object^>^>^ oAr) {
	JsonArray^ oRet = ref new JsonArray();
	if (oAr != nullptr) {
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
	}// null
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
IMap<String^, String^>^ CouchDBProxy::StReadNamesMimes(String^ jsonText) {
	IMap<String^, String^>^ oRet = ref new Map<String^, String^>();
	if ((jsonText != nullptr) & (!jsonText->IsEmpty())) {
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
	}// not empty
	return oRet;
}//StReadNamesMimes
int CouchDBProxy::StDocsCount(String^ jsonText) {
	int nRet = 0;
	if ((jsonText == nullptr) || (jsonText->IsEmpty())) {
		return nRet;
	}
	JsonValue^ val = JsonValue::Parse(jsonText);
	if (val->ValueType == JsonValueType::Object) {
		JsonObject^ obj = val->GetObject();
		if (obj->HasKey(KEY_DOCS)) {
			IJsonValue^ vx = obj->Lookup(KEY_DOCS);
			if (vx->ValueType == JsonValueType::Array) {
				JsonArray^ oAr = vx->GetArray();
				nRet = (int)oAr->Size;
			}// array
		}// docs
	}// obj
	return nRet;
}//StDocsCount
IVector<IMap<String^, Object^>^>^ CouchDBProxy::StReadDocs(String^ jsonText) {
	IVector<IMap<String^, Object^>^>^ xRet = ref new Vector<IMap<String^, Object^>^>();
	if ((jsonText == nullptr) || (jsonText->IsEmpty())) {
		return (xRet);
	}
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
						xRet->Append(cur);
					}// obj
					it->MoveNext();
				}// it
			}// array
		}// docs
	}// obj
	return (xRet);
}//StReadDocs

