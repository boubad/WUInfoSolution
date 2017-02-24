#pragma once
using namespace Platform;
using namespace Windows::Foundation::Collections;
///////////////////////
namespace InfoDomain {
	/////////////////
	public ref class StringUtils sealed
	{
	public:
		static String^ Trim(String^ src);
		static String^ ToUpper(String^ src);
		static String^ ToLower(String^ src);
		static String^ ToUpperFormat(String^ src);
		static String^ FormatName(String^ src);
		static IVector<String^>^ SplitString(String^ src, String^ delim);
	};// class StringUtils
}// namespace InfoDomain
