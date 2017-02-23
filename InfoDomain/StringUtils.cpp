#include "pch.h"
#include "StringUtils.h"
#include <cctype>
#include <algorithm>
#include <locale>
//////////////////////////
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
	/////////////////////////////
}// namespace InfoDomain
