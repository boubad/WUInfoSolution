#pragma once
//
#include "Common.h"
//
using namespace Platform;
using namespace Windows::Foundation::Collections;
///////////////////////
namespace GenDomainData {
	/////////////////
	public ref class StringUtils sealed
	{
	public:
		static bool StringToBoolean(String^ s);
		static int StringToInteger(String^ s);
		static double StringToDouble(String^ s);
		//
		static String^ Trim(String^ src);
		static String^ ToUpper(String^ src);
		static String^ ToLower(String^ src);
		static String^ ToUpperFormat(String^ src);
		static String^ FormatName(String^ src);
		static IVector<String^>^ SplitString(String^ src, String^ delim);
		//
		static String^ StatusToString(InfoStatus st);
		static InfoStatus StringToStatus(String^ s);
		static IVector<String^>^ AllStatusStrings(void);
		//
		static String^ DataTypeToString(InfoDataType st);
		static InfoDataType StringToDataType(String^ s);
		static IVector<String^>^ AllDataTypeStrings(void);
		//
		static String^ DataKindToString(InfoKind st);
		static InfoKind StringToDataKind(String^ s);
		static IVector<String^>^ AllDataKindStrings(void);
		//
	};// class StringUtils
}// namespace InfoDomain
