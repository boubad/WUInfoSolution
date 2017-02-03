#include "pch.h"
#include "couchdbclient.h"
#include "docutils.h"
#include "jsonutils.h"
/////////////////////
using namespace InfoCouchDB;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Web::Http;
using namespace Windows::Web::Http::Headers;
using namespace Windows::Data::Json;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace concurrency;
//
CouchDBClient::CouchDBClient() {

}
CouchDBClient::~CouchDBClient() {

}
CouchDBClient::CouchDBClient(String^ url, String^ database) :m_url(url), m_database(database) {

	m_client = ref new HttpClient();
	m_client->DefaultRequestHeaders->Accept->Clear();
	m_client->DefaultRequestHeaders->Accept->Append(ref new HttpMediaTypeWithQualityHeaderValue("application/json"));
}
Platform::Object^ CouchDBClient::ConvertValue(IJsonValue^ val) {
	Platform::Object^ r;
	auto xtype = val->ValueType;
	if (xtype == JsonValueType::String) {
		r = val->GetString();
		return (r);
	}
	else if (xtype == JsonValueType::Number) {
		r = val->GetNumber();
		return (r);
	}
	else if (xtype == JsonValueType::Boolean) {
		r = val->GetBoolean();
		return (r);
	}
	else if (xtype == JsonValueType::Array) {
		JsonArray^ oAr = val->GetArray();
		Vector<Platform::Object^>^ v = ref new Vector<Platform::Object^>();
		auto it = oAr->First();
		while (it->HasCurrent) {
			IJsonValue^ x = it->Current;
			Platform::Object^ xx = this->ConvertValue(x);
			v->Append(xx);
			it->MoveNext();
		}
		return (v);
	}
	else if (xtype == JsonValueType::Object) {
		JsonObject^ obj = val->GetObject();
		Map<String^, Object^>^ m = ref new Map<String^, Object^>();
		auto it = obj->First();
		while (it->HasCurrent) {
			auto p = it->Current;
			String^ key = p->Key;
			Object^ v = this->ConvertValue(p->Value);
			m->Insert(key, v);
			it->MoveNext();
		}
		return (m);
	}
	return (r);
}//ConvertValue
Platform::Object^ CouchDBClient::ReadGetValue(String^ sUri) {
	Uri^ uri = ref new Uri(sUri);
	auto myop = this->m_client->GetAsync(uri);
	auto operationTask = create_task(myop);
	return operationTask.then([this](HttpResponseMessage^ response) {
		Object^ oRet;
		auto status = response->StatusCode;
		if (status == HttpStatusCode::Ok) {
			try {
				auto myop = response->Content->ReadAsStringAsync();
				oRet = create_task(myop).then([this](String^ jsonText) {
					JsonValue^ val = JsonValue::Parse(jsonText);
					return this->ConvertValue(val);
				}).get();
			}
			catch (Exception^ e) {
				(void)e;
			}
		}// ok
		return oRet;
	}).get();
}// ReadGetValue
bool CouchDBClient::IsServerAlive(void) {
	String^ sUri = this->m_url;
	Uri^ uri = ref new Uri(sUri);
	auto myop = this->m_client->GetAsync(uri);
	auto operationTask = create_task(myop);
	bool sRet = operationTask.then([](HttpResponseMessage^ response) {
		auto status = response->StatusCode;
		return (status == HttpStatusCode::Ok);
	}).get();
	return (sRet);
}//IsServerAlive
Platform::Object^ CouchDBClient::GetServerInfo(void) {
	String^ sUri = this->m_url;
	return this->ReadGetValue(sUri);
}//GetServerInfo
Platform::Object^ CouchDBClient::GetDatabaseInfo(String^ db) {
	String^ sUri = this->m_url + db;
	return this->ReadGetValue(sUri);
}//GetServerInfo
Platform::Object^ CouchDBClient::ReadDocumentById(String^ id) {
	String ^sUri = this->m_url + this->m_database + "/" + id;
	return this->ReadGetValue(sUri);
}//ReadDocumentById
bool CouchDBClient::IsDatabaseExists(String^ databaseName) {
	String^ sUri = this->m_url + databaseName;
	Uri^ uri = ref new Uri(sUri);
	HttpRequestMessage^ req = ref new HttpRequestMessage(HttpMethod::Head, uri);
	auto myop = this->m_client->SendRequestAsync(req);
	auto operationTask = create_task(myop);
	return operationTask.then([this](HttpResponseMessage^ response) {
		auto status = response->StatusCode;
		return (status == HttpStatusCode::Ok);
	}).get();
}// IsDatabaseExists
bool CouchDBClient::CreateDatabase(String^ databaseName) {
	String^ sUri = this->m_url + databaseName;
	Uri^ uri = ref new Uri(sUri);
	String^ s = ref new String();
	HttpStringContent^ sc = ref new HttpStringContent(s);
	auto myop = this->m_client->PutAsync(uri, sc);
	auto operationTask = create_task(myop);
	return operationTask.then([this](HttpResponseMessage^ response) {
		auto status = response->StatusCode;
		return (status == HttpStatusCode::Created);
	}).get();
}// CreateDatabase
bool CouchDBClient::DeleteDatabase(String^ databaseName) {
	String^ sUri = this->m_url + databaseName;
	Uri^ uri = ref new Uri(sUri);
	auto myop = this->m_client->DeleteAsync(uri);
	auto operationTask = create_task(myop);
	return operationTask.then([this](HttpResponseMessage^ response) {
		auto status = response->StatusCode;
		return (status == HttpStatusCode::Ok);
	}).get();
}//DeleteDatabase
/////////////////////////
bool CouchDBClient::CreateIndex(String^ indexname, IMap<String^, bool>^ fields) {
	IndexData oData{};
	oData.Name = indexname;
	auto it = fields->First();
	while (it->HasCurrent) {
		auto p = it->Current;
		String^ f = p->Key;
		bool b = p->Value;
		oData.AddField(f, b);
		it->MoveNext();
	}
	String^ sp = oData.to_json()->Stringify();
	HttpStringContent^ sc = ref new HttpStringContent(sp);
	sc->Headers->ContentType->MediaType = "application/json";
	String^ sUri = this->m_url + m_database  + "/_index";
	Uri^ uri = ref new Uri(sUri);
	auto myop = this->m_client->PostAsync(uri, sc);
	auto operationTask = create_task(myop);
	return operationTask.then([](HttpResponseMessage^ response) {
		auto status = response->StatusCode;
		auto reason = response->ReasonPhrase;
		return ((status == HttpStatusCode::Created) || (status == HttpStatusCode::Ok));
	}).get();
}//CreateIndex
int CouchDBClient::GetCountFilter(IMap<String^, Object^>^ oFetch) {
	String^ sUri = this->m_url + m_database + "/_index";
	Uri^ uri = ref new Uri(sUri);
	int nTotal = 0;
	int nCount = 128;
	int nOffset = 0;
	bool done = false;
	String^ idField = "_id";
	IVector<String^>^ pFields = ref new Vector<String^>();
	pFields->Append(idField);
	do {
		String^ sp = JsonUtils::ConvertFindFilter(oFetch, pFields, nOffset, nCount);
		HttpStringContent^ sc = ref new HttpStringContent(sp);
		sc->Headers->ContentType->MediaType = "application/json";
		auto myop = this->m_client->PostAsync(uri, sc);
		auto operationTask = create_task(myop);
		int nx =  operationTask.then([](HttpResponseMessage^ response) {
			int nRet = 0;
			auto status = response->StatusCode;
			if (status == HttpStatusCode::Ok) {
				try {
					auto myopx = response->Content->ReadAsStringAsync();
					create_task(myopx).then([&nRet](String^ jsonText) {
						JsonValue^ val = JsonValue::Parse(jsonText);
						if (val->ValueType == JsonValueType::Object) {
							JsonObject^ obj = val->GetObject();
							if (obj->HasKey("docs")) {
								IJsonValue^ vx = obj->Lookup("docs");
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
IVector<IMap<String^, Object^>^>^ CouchDBClient::ReadDocuments(IMap<String^, Object^>^ oFetch, int offset /*= 0*/, int count /*= 0*/) {
	String^ sUri = this->m_url + m_database + "/_index";
	Uri^ uri = ref new Uri(sUri);
	IVector<String^>^ pFields = ref new Vector<String^>();
	String^ sp = JsonUtils::ConvertFindFilter(oFetch, pFields, offset, count);
	HttpStringContent^ sc = ref new HttpStringContent(sp);
	sc->Headers->ContentType->MediaType = "application/json";
	auto myop = this->m_client->PostAsync(uri, sc);
	auto operationTask = create_task(myop);
	return operationTask.then([](HttpResponseMessage^ response) {
		IVector<IMap<String^, Object^>^>^ oRet = ref new Vector<IMap<String^, Object^>^>();
		auto status = response->StatusCode;
		if (status == HttpStatusCode::Ok) {
			try {
				auto myopx = response->Content->ReadAsStringAsync();
				create_task(myopx).then([oRet](String^ jsonText) {
					JsonValue^ val = JsonValue::Parse(jsonText);
					if (val->ValueType == JsonValueType::Object) {
						JsonObject^ obj = val->GetObject();
						if (obj->HasKey("docs")) {
							IJsonValue^ vx = obj->Lookup("docs");
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
											Object^ o = JsonUtils::ConvertJsonObject(p->Value);
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
