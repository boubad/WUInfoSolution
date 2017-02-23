#pragma once
using namespace Platform;
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
	};// class StringUtils
}// namespace InfoDomain
