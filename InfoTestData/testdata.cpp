#include "pch.h"
#include "testdata.h"
#include <cassert>
using namespace Platform;
using namespace Platform::Collections;
using namespace concurrency;
////////////////////////////
namespace InfoTestData {
	String^ TestData::TESTFILE_NAME = "testfile.dat";
	///////////////////////////////
	TestData::TestData()
	{
	}
	//
	String^ TestData::MortalName::get() {
		return  "mortal_data";
	}
	int TestData::MortalRowsCount::get() {
		return 19;
	}
	int TestData::MortalColsCount::get() {
		return  6;
	}
	IVector<String^>^ TestData::MortalRowsNames::get() {
		Vector<String^>^ oRet = ref new Vector<String^>({ "AUTRICHE","FRANCE","PORTUGAL","ALLFEDER","BELGIQUE",
			"FINLANDE","SUEDE","SUISSE","ITALIE","IRLNORD",
			"DANNEMARK","ISLANDE","ECOSSE","ESPAGNE","NORVEGE",
			"IRLSUD","PABAS","ANGGALLE","USA" });
		return oRet;
	}
	IVector<String^>^ TestData::MortalColsNames::get() {
		Vector<String^>^ oRet = ref new Vector<String^>({ "SUICI","HOMIC","AROUT","AINDU","AAUTR","CIRFO" });
		return oRet;
	}
	IVector<int>^ TestData::MortalData::get() {
		Vector<int>^ oRet = ref new Vector<int>({ 241,16,330,43,363,325,
			156, 9, 225,10,535,328,
			85,19,349,7,281,345,
			210,12,230,21,298,269,
			156,10,260,13,367,144,
			251, 26,180,29,387,55,
			194,11,151,13,384,122,
			225, 9, 195,26,276,128,
			54,11,219,19,224,319,
			40,136,215,18,320,43,
			241,6,168,11,230,107,
			101,5,179,23,380,9,
			82,15,155,18,342,59,
			40,4,136,17,237,225,
			104,6,138,22,346,41,
			38,7,182,32,314,37,
			89,7,169,10,218,47,
			79,10,130,14,203,36,
			121,102,220,26,273,158
		});
		return oRet;
	}
	//
	String^ TestData::ConsoName::get() {
		return "conso_data";
	}
	int TestData::ConsoRowsCount::get() {
		return 12;
	}
	int TestData::ConsoColsCount::get() {
		return 7;
	}
	IVector<String^>^ TestData::ConsoRowsNames::get() {
		Vector<String^>^ oRet = ref new Vector<String^>({ "MA2","EM2","CA2","MA3","EM3",
			"CA3","MA4","EM4","CA4","MA5","EM5","CA5" });
		return oRet;
	}
	IVector<String^>^ TestData::ConsoColsNames::get() {
		Vector<String^>^ oRet = ref new Vector<String^>({ "PAIN","LEGUMES","FRUITS","VIANDES","VOLAILLES","LAIT","VIN" });
		return oRet;
	}
	IVector<int>^ TestData::ConsoData::get() {
		Vector<int>^ oRet = ref new Vector<int>({
			332,428,354,1437,526,247,437,
			293,559,388,1527,567,239,258,
			372,767,562,1948,927,235,433,
			406,563,341,1507,544,324,407,
			386,608,396,1501,558,319,363,
			438,843,689,2345,1148,243,341,
			534,660,367,1620,638,414,407,
			460,699,484,1856,762,400,416,
			385,789,621,2366,1149,304,282,
			655,776,423,1848,759,495,486,
			584,995,548,2056,893,518,319,
			515,1097,887,2630,1187,561,284
		});
		return oRet;
	}
	//
	String^ TestData::TestImageFileName::get() {
		using byte = uint8;
		StorageFolder^ folder = Windows::Storage::ApplicationData::Current->LocalFolder;
		assert(folder != nullptr);
		IStorageFile^ pFile = create_task(folder->CreateFileAsync(TESTFILE_NAME, CreationCollisionOption::ReplaceExisting)).get();
		assert(pFile != nullptr);
		int nMax{ 256 };
		Array<byte>^ oAr = ref new Array<byte>(nMax);
		assert(oAr != nullptr);
		for (int i = 0; i < nMax; ++i) {
			oAr[i] = (byte)i;
		}// i
		create_task(Windows::Storage::FileIO::WriteBytesAsync(pFile, oAr)).wait();
		String^ sRet = pFile->Path;
		return sRet;
	}
	IStorageFile^ TestData::TestImageFile::get() {
		String^ filepath = this->TestImageFileName;
		auto op = StorageFile::GetFileFromPathAsync(filepath);
		IStorageFile^ pRet = create_task(op).get();
		return pRet;
	}
	String^ TestData::AttachmentName::get() {
		return "attachName";
	}
	String^ TestData::AttachmentMime::get() {
		IStorageFile^ p = this->TestImageFile;
		assert(p != nullptr);
		String^ sRet = p->ContentType;
		if ((sRet == nullptr) || sRet->IsEmpty()) {
			sRet = "application/octet-stream";
		}
		return sRet;
	}
	IBuffer^ TestData::TestImageBuffer::get() {
		String^ filepath = this->TestImageFileName;
		IStorageFile^ file = create_task(StorageFile::GetFileFromPathAsync(filepath)).get();
		if (file == nullptr) {
			throw ref new InvalidArgumentException();
		}
		IRandomAccessStreamWithContentType^ stream = create_task(file->OpenReadAsync()).get();
		unsigned int  n = static_cast<unsigned int>(stream->Size);
		IInputStream^ ss = stream->GetInputStreamAt(0);
		DataReader^ reader = ref new DataReader(ss);
		create_task(reader->LoadAsync(n)).get();
		IBuffer^ pBuf = reader->DetachBuffer();
		return pBuf;
	}
}// namespace InfoTestData