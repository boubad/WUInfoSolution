#pragma once
namespace InfoDomain {
	using namespace Platform;
////////////////////////////
	public ref class InfoStrings sealed {
	public:
		static property String^ KEY_ID {
			String^ get();
		}
		static property String^ KEY_REV {
			String^ get();

		}
		static property String^ KEY_TYPE {
			String^ get();

		}
		static property String^ KEY_DESC {
			String^ get();

		}
		static property String^ KEY_STATUS {
			String^ get();

		}
		static property String^ KEY_SIGLE {
			String^ get();

		}
		static property String^ KEY_NAME {
			String^ get();
		}
		static property String^ KEY_DATASETSIGLE {
			String^ get();
		}
		static property String^ KEY_VARIABLESIGLE {
			String^ get();
		}
		static property String^ KEY_INDIVSIGLE {
			String^ get();
		}
		static property String^ KEY_DATATYPE {
			String^ get();
		}
		static property String^ KEY_VARIABLEKIND {
			String^ get();
		}
		static property String^ KEY_MODALITES {
			String^ get();
		}
		static property String^ KEY_VALUE {
			String^ get();
		}
		static property String^ TYPE_DATASET {
			String^ get();
		}
		static  property String^ TYPE_VARIABLE{
			String^ get();
		}
		static property String^ TYPE_INDIV {
			String^ get();
		}
		static property String^ TYPE_VALUE {
			String^ get();
		}
	};// class InfoStrings
////////////////////////////
}// namespace InfoDoamin
