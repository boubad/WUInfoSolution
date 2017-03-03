#include "pch.h"
#include "StringUtils.h"
#include <sstream>
#include <string>
#include <cctype>
#include <algorithm>
#include <locale>
//////////////////////////
using namespace Platform::Collections;
namespace GenDomainData {
	/////////////////////////
	namespace internal {
		static std::wstring info_trim(const  std::wstring &s) {
			std::wstring sRet{};
			const size_t n = s.length();
			if (n > 0) {
				auto pStart = s.c_str();
				auto pEnd = pStart + n - 1;
				while ((*pStart != 0) && std::isblank(*pStart)) {
					pStart++;
				}	// while
				while ((pStart < pEnd) && std::isblank(*pEnd)) {
					pEnd--;
				}
				pEnd++;
				sRet = std::wstring{ pStart, pEnd };
			}	// not empty
			return (sRet);
		}	// trim
		static std::wstring info_tolower(const std::wstring &s) {
			if (s.empty()) {
				return (std::wstring{});
			}
			std::wstring sRet{ s };
			std::locale loc{};
			std::transform(s.begin(), s.end(), sRet.begin(), [loc](auto c) {
				return (std::tolower(c));
			});
			return (sRet);
		}	// info_tolower
		static std::wstring info_toupper(const std::wstring &s) {
			if (s.empty()) {
				return (std::wstring{});
			}
			std::wstring sRet{ s };
			std::locale loc{};
			std::transform(s.begin(), s.end(), sRet.begin(), [loc](auto c) {
				return (std::toupper(c));
			});
			return (sRet);
		}	// info_toupper
		static void info_splitstring(const std::wstring &s, std::vector<std::wstring> &vec, const std::wstring &delim) {
			vec.clear();
			std::wstring ss{ s };
			size_t pos = ss.find_first_of(delim);
			while (pos != std::wstring::npos) {
				std::wstring sx{ ss.begin(), ss.begin() + pos };
				vec.push_back(info_trim(sx));
				ss = std::wstring{ ss.begin() + pos + 1, ss.end() };
				pos = ss.find_first_of(delim);
			} // while
			if (!ss.empty()) {
				vec.push_back(info_trim(ss));
			}
		} //info_split_string
	}// namespace internal
	/////////////////////////////////
	bool StringUtils::StringToBoolean(String^ s) {
		bool bRet{ false };
		if ((s != nullptr) && (!s->IsEmpty())) {
			std::wstringstream in(s->Data());
			try {
				in >> bRet;
			}
			catch (...) {}
		}
		return bRet;
	}
	int StringUtils::StringToInteger(String^ s) {
		int bRet{ 0 };
		if ((s != nullptr) && (!s->IsEmpty())) {
			std::wstringstream in(s->Data());
			try {
				in >> bRet;
			}
			catch (...) {}
		}
		return bRet;
	}
	double StringUtils::StringToDouble(String^ s) {
		double bRet{ 0 };
		if ((s != nullptr) && (!s->IsEmpty())) {
			std::wstringstream in(s->Data());
			try {
				in >> bRet;
			}
			catch (...) {}
		}
		return bRet;
	}
	///////////////////////////////
	String^ StringUtils::Trim(String^ src) {
		if ((src == nullptr) || src->IsEmpty()) {
			return ref new String{};
		}
		std::wstring s{ src->Data() };
		std::wstring ss = internal::info_trim(s);
		return ref new String{ ss.c_str() };
	}// trim
	String^ StringUtils::ToUpper(String^ src) {
		if ((src == nullptr) || src->IsEmpty()) {
			return ref new String{};
		}
		std::wstring s{ src->Data() };
		std::wstring ss = internal::info_toupper(s);
		return ref new String{ ss.c_str() };
	}
	String^ StringUtils::ToLower(String^ src) {
		if ((src == nullptr) || src->IsEmpty()) {
			return ref new String{};
		}
		std::wstring s{ src->Data() };
		std::wstring ss = internal::info_tolower(s);
		return ref new String{ ss.c_str() };
	}
	String^ StringUtils::ToUpperFormat(String^ src) {
		if ((src == nullptr) || src->IsEmpty()) {
			return ref new String{};
		}
		std::wstring s{ src->Data() };
		std::wstring ss = internal::info_toupper(internal::info_trim(s));
		return ref new String{ ss.c_str() };
	}
	String^ StringUtils::FormatName(String^ src) {
		if ((src == nullptr) || src->IsEmpty()) {
			return ref new String{};
		}
		std::wstring s{ src->Data() };
		std::wstring ss = internal::info_trim(s);
		size_t n = ss.length();
		if (n > 1) {
			ss = internal::info_toupper(ss.substr(0, 1)) + internal::info_tolower(ss.substr(1));
		}
		else if (n > 0) {
			ss = internal::info_toupper(ss);
		}
		return ref new String{ ss.c_str() };
	}
	IVector<String^>^ StringUtils::SplitString(String^ src, String^ delim) {
		IVector<String^>^ oRet = ref new Vector<String^>();
		if ((src == nullptr) || src->IsEmpty()) {
			return oRet;
		}
		String^ sDelim = ((delim == nullptr) || delim->IsEmpty()) ? ";,:\t " : delim;
		std::wstring wd{ sDelim->Data() };
		std::wstring s{ src->Data() };
		std::vector<std::wstring> vec{};
		internal::info_splitstring(s, vec, wd);
		for (auto it = vec.begin(); it != vec.end(); ++it) {
			std::wstring sx = *it;
			String^ ss = ref new String{ sx.c_str() };
			oRet->Append(ss);
		}// it
		return oRet;
	}
	//
	String^ StringUtils::StatusToString(InfoStatus st) {
		String^ sRet{};
		switch (st) {
		case InfoStatus::Normal:
			sRet = "Normal";
			break;
		case InfoStatus::Tail:
			sRet = "Tail";
			break;
		case InfoStatus::Disabled:
			sRet = "Disabled";
			break;
		case InfoStatus::Info:
			sRet = "Info";
			break;
		case InfoStatus::Inactive:
			sRet = "Inactive";
			break;
		case InfoStatus::Inserted:
			sRet = "Inserted";
		case InfoStatus::Updated:
			sRet = "Updated";
			break;
		case InfoStatus::Deleted:
			sRet = "Deleted";
		default:
			sRet = "Unknown";
			break;
		}// et
		return (sRet);
	}
	InfoStatus StringUtils::StringToStatus(String^ sx) {
		String^ s = StringUtils::FormatName(sx);
		InfoStatus r{ InfoStatus::Unknown };
		if ((s != nullptr) && (!s->IsEmpty())) {
			if (s == "Normal") {
				r = InfoStatus::Normal;
			}
			else if (s == "Tail") {
				r = InfoStatus::Tail;
			}
			else if (s == "Disabled") {
				r = InfoStatus::Disabled;
			}
			else if (s == "Info") {
				r = InfoStatus::Info;
			}
			else if (s == "Inactive") {
				r = InfoStatus::Inactive;
			}
			else if (s == "Inserted") {
				r = InfoStatus::Inserted;
			}
			else if (s == "Updated") {
				r = InfoStatus::Updated;
			}
			else if (s == "Deleted") {
				r = InfoStatus::Deleted;
			}
		}// s
		return r;
	}
	IVector<String^>^ StringUtils::AllStatusStrings(void) {
		Vector<String^>^ pRet = ref new Vector<String^>{
			"Unknown","Normal","Tail","Disabled","Info","Inactive",
			"Inserted","Updated","Deleted" };
		return pRet;
	}
	//
	String^ StringUtils::DataTypeToString(InfoDataType st) {
		String^ sRet{};
		switch (st) {
		case InfoDataType::Logical:
			sRet = "Logical";
			break;
		case InfoDataType::Integer:
			sRet = "Integer";
			break;
		case InfoDataType::Real:
			sRet = "Real";
			break;
		case InfoDataType::Text:
			sRet = "Text";
			break;
		case InfoDataType::Other:
			sRet = "Other";
			break;
		default:
			sRet = "Unknown";
			break;
		}
		return sRet;
	}
	InfoDataType StringUtils::StringToDataType(String^ ss) {
		String^ s = StringUtils::FormatName(ss);
		InfoDataType r{ InfoDataType::Unknown };
		if ((s != nullptr) && (!s->IsEmpty())) {
			if (s == "Logical") {
				r = InfoDataType::Logical;
			}
			else if (s == "Integer") {
				r = InfoDataType::Integer;
			}
			else if (s == "Real") {
				r = InfoDataType::Real;
			}
			else if (s == "Text") {
				r = InfoDataType::Text;
			}
			else if (s == "Other") {
				r = InfoDataType::Other;
			}
		}
		return r;
	}
	IVector<String^>^ StringUtils::AllDataTypeStrings(void) {
		Vector<String^>^ pRet = ref new Vector<String^>{
			"Unknown","Logical","Integer","Real","Text","other" };
		return pRet;
	}
	//
	String^ StringUtils::DataKindToString(InfoKind st) {
		String^ sRet{};
		switch (st)
		{
		case InfoKind::Normal:
			sRet = "Normal";
			break;
		case InfoKind::Modal:
			sRet = "Modal";
			break;
		case InfoKind::Ordinal:
			sRet = "Ordinal";
			break;
		default:
			sRet = "Unknown";
			break;
		}
		return sRet;
	}
	InfoKind StringUtils::StringToDataKind(String^ ss) {
		String^ s = StringUtils::FormatName(ss);
		InfoKind r{ InfoKind::Unknown };
		if ((s != nullptr) || (!s->IsEmpty())) {
			if (s == "Normal") {
				r = InfoKind::Normal;
			}
			else if (s == "Modal") {
				r = InfoKind::Modal;
			}
			else if (s == "Ordinal") {
				r = InfoKind::Ordinal;
			}
		}
		return r;
	}
	IVector<String^>^ StringUtils::AllDataKindStrings(void) {
		Vector<String^>^ pRet = ref new Vector<String^>{
			"Unknown","Normal","Modal","Ordinal" };
		return pRet;
	}
	/////////////////////////////
}// namespace InfoDomain
