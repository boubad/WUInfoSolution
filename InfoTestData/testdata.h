#pragma once
using namespace Platform;
///////////////////////////////
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
///////////////////////////
namespace InfoTestData
{
    public ref class TestData sealed
    {
	private:
		static String^ TESTFILE_NAME;
    public:
        TestData();
		//
		property String^ MortalName {
			String^ get();
		}
		property int MortalRowsCount {
			int get();
		}
		property int MortalColsCount {
			int get();
		}
		property IVector<String^>^ MortalRowsNames {
			IVector<String^>^ get();
		}
		property IVector<String^>^ MortalColsNames {
			IVector<String^>^ get();
		}
		property IVector<int>^ MortalData {
			IVector<int>^ get();
		}
		//
		property String^ ConsoName {
			String^ get();
		}
		property int ConsoRowsCount {
			int get();
		}
		property int ConsoColsCount {
			int get();
		}
		property IVector<String^>^ ConsoRowsNames {
			IVector<String^>^ get();
		}
		property IVector<String^>^ ConsoColsNames {
			IVector<String^>^ get();
		}
		property IVector<int>^ ConsoData {
			IVector<int>^ get();
		}
		//
		property IStorageFile^ TestImageFile {
			IStorageFile^ get();
		}
		property IBuffer^ TestImageBuffer {
			IBuffer^ get();
		}
    }; // Class TestData
}
