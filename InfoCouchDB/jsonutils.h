#pragma once
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Data::Json;
using namespace Windows::Web::Http;
/////////////////////////////
namespace InfoCouchDB {
	class JsonUtils {
	public:
		static IJsonValue^ ConvertObject(Object^ obj);
		static String^ ConvertFindFilter(IMap<String^, Object^>^ oFetch,
			IVector<String^>^ oFields, int skip = 0, int count = 0);
		static String^ MapToJson(IMap<String^, Object^>^ oMap);
		static String^ MapToJson(IVector<IMap<String^, Object^>^>^ oAr);
		static Object^ ConvertJsonObject(IJsonValue^ jsonVal);
	};// class JsonUtils
}// namespace InfoCouchDB
