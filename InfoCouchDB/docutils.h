#pragma once
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Data::Json;
//////////////////////
namespace InfoCouchDB
{
	class SortField {
	private:
		bool	 m_dir;
		std::wstring m_name;
	public:
		SortField(const std::wstring &s, bool bDir = true):m_dir(bDir),m_name(s){}
		SortField(const SortField &other):m_dir(other.m_dir),m_name(other.m_name){}
		SortField & operator=(const SortField &other) {
			if (this != &other) {
				this->m_dir = other.m_dir;
				this->m_name = other.m_name;
			}
			return (*this);
		}
		~SortField(){}
		IJsonValue^ to_json(void) {
			JsonObject^ obj = ref new JsonObject();
			String^ key = ref new String(m_name.c_str());
			String^ val = (m_dir) ? "asc" : "desc";
			obj->Insert(key, JsonValue::CreateStringValue(val));
			return (obj);
		}// to_json
	};// class SortField
	////////////////////////
	class IndexData {
	public:
		String^ Name;
		String^ DesignDocument;
	private:
		std::vector<SortField> m_fields;
	public:
		IndexData(){}
		~IndexData(){}
		void AddField(String^ name, bool b = true) {
			if (m_fields.empty()) {
				std::wstring s{ name->Data() };
				m_fields.push_back(SortField{ s,b });
			}
		}// AddField
		IJsonValue^ to_json(void) {
			JsonObject^ obj = ref new JsonObject();
			JsonObject^ o = ref new JsonObject();
			JsonArray^ oAr = ref new JsonArray();
			for (auto it = m_fields.begin(); it != m_fields.end(); ++it) {
				IJsonValue^ p = (*it).to_json();
				oAr->Append(p);
			}// it
			o->Insert("fields", oAr);
			obj->Insert("index", o);
			if (!Name->IsEmpty()) {
				obj->Insert("name", JsonValue::CreateStringValue(Name));
			}
			if (!DesignDocument->IsEmpty()) {
				obj->Insert("ddoc", JsonValue::CreateStringValue(DesignDocument));
			}
			return (obj);
		}// to_json
	};// class IndexData
	//////////////////////////
}// namespace InfoWRC
