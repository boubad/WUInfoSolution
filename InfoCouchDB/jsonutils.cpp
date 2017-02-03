#include "pch.h"
#include "jsonutils.h"
//////////////////////////
using namespace Platform::Collections;
using namespace InfoCouchDB;
///////////////////////
//
IJsonValue^ JsonUtils::ConvertObject(Object^ obj) {
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
	return JsonValue::CreateNullValue();
}// ConvertObject
String^ JsonUtils::ConvertFindFilter(IMap<String^, Object^>^ oFetch,
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
String^ JsonUtils::MapToJson(IMap<String^, Object^>^ oMap) {
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
String^ JsonUtils::MapToJson(IVector<IMap<String^, Object^>^>^ oAr) {
	JsonArray^ oRet = ref new JsonArray();
	auto it = oAr->First();
	while (it->HasCurrent) {
		IMap<String^,Object^>^ p = it->Current;
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
Object^JsonUtils::ConvertJsonObject(IJsonValue^ jsonVal) {
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
			Object^ o = ConvertObject(it->Current);
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
			Object^ o = ConvertObject(p->Value);
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