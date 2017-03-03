#include "pch.h"
#include "StringUtils.h"
#include <string>
#include <cctype>
#include <algorithm>
#include <locale>
//////////////////////////
using namespace Platform::Collections;
namespace InfoDomain {
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
		static void info_splitstring(const std::wstring &s,std::vector<std::wstring> &vec, const std::wstring &delim) {
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
		std::wstring wd{sDelim->Data()};
		std::wstring s{ src->Data() };
		std::vector<std::wstring> vec{};
		internal::info_splitstring(s, vec, wd);
		for (auto it = vec.begin(); it != vec.end(); ++it) {
			std::wstring sx = *it;
			String^ ss = ref new String{sx.c_str()};
			oRet->Append(ss);
		}// it
		return oRet;
	}
	/////////////////////////////
}// namespace InfoDomain
