#pragma once
using namespace Platform;
using namespace Windows::Foundation::Collections;
using namespace Platform::Collections;
//

//
namespace InfoProcs
{
	ref class IndivDesc;
	ref class VarDesc;
	ref class DatasetDesc;
	//
	public ref class VarDesc sealed 
    {
	private:
		int m_index = -1;
		bool  m_bool = false;
		bool m_nominal = false;
		bool m_number = true;
		bool m_info = false;
		bool m_active = true;
		String^ m_id;
		IMap<String^, Object^>^ m_data;
		//
		void check(void) {
			if ((!m_bool) && (!m_nominal) && (!m_number)) {
				m_number = true;
			}
		}
    public:
        VarDesc();
		property String^ Id 
		{
			String^ get() {
				if (m_id == nullptr) {
					m_id = ref new String{};
				}
				return m_id;
			}
			void set(String^ value) {
				m_id =  value;
			}
		}// Id
		property int Index {
			int get() {
				return m_index;
			}
			void set(int value) {
				m_index = value;
			}
		}// index
		property bool IsNominal {
			bool get() {
				return m_nominal;
			}
			void set(bool value) {
				m_nominal = value;
				if (m_nominal) {
					m_bool = false;
					m_number = false;
				}
				this->check();
			}
		}// IsNominal
		property bool IsBoolean {
			bool get() {
				return m_bool;
			}
			void set(bool value) {
				m_bool = value;
				if (m_bool) {
					m_nominal = false;
					m_number = false;
				}
				this->check();
			}
		}// IsBoolean
		property bool IsNumber {
			bool get() {
				return m_number;
			}
			void set(bool value) {
				m_number = value;
				if (m_number) {
					m_bool = false;
					m_nominal = false;
				}
				this->check();
			}
		}// IsNumber
		property bool IsInfo {
			bool get() {
				return m_info;
			}
			void set(bool value) {
				m_info = value;
			}
		}// IsInfo
		property bool IsActive {
			bool get() {
				return m_active;
			}
			void set(bool value) {
				m_active = value;
			}
		}// IsInfo
		//
		property IMap<String^, Object^>^ Data {
			IMap < String^, Object^>^ get() {
				if (m_data == nullptr) {
					m_data = ref new Map<String^, Object^>{};
				}
				return m_data;
			}
		}
		void Add(IndivDesc^ pInd, Object^ val);
		Object^ GetValue(IndivDesc^ pInd);
		property IVector<Object^>^ Values {
			IVector<Object^>^ get();
		}
    }; // class VarDesc
	//
	public ref class IndivDesc sealed {
	private:
		int m_index = -1;
		bool m_lowertail = false;
		bool m_uppertail = false;
		bool m_sup = false;
		bool m_info = false;
		bool m_active = false;
		String^ m_id;
		IMap<String^, Object^>^ m_data;
		//
	public:
		IndivDesc();
		property String^ Id
		{
			String^ get() {
				if (m_id == nullptr) {
					m_id = ref new String{};
				}
				return m_id;
			}
			void set(String^ value) {
				m_id =  value;
			}
		}// Id
		property int Index {
			int get() {
				return m_index;
			}
			void set(int value) {
				m_index = value;
			}
		}// index
		property bool IsLowerTail {
			bool get() {
				return m_lowertail;
			}
			void set(bool value) {
				m_lowertail = value;
				if (m_lowertail) {
					m_uppertail = false;
				}
			}
		}// IsNominal
		property bool IsUpperTail {
			bool get() {
				return m_uppertail;
			}
			void set(bool value) {
				m_uppertail = value;
				if (m_uppertail) {
					m_lowertail = false;
				}
			}
		}// IsBoolean
		property bool IsSup {
			bool get() {
				return m_sup;
			}
			void set(bool value) {
				m_sup = value;
			}
		}// IsNumber
		property bool IsInfo {
			bool get() {
				return m_info;
			}
			void set(bool value) {
				m_info = value;
			}
		}// IsInfo
		property bool IsActive {
			bool get() {
				return m_active;
			}
			void set(bool value) {
				m_active = value;
			}
		}// IsInfo
		//
		property IMap<String^, Object^>^ Data {
			IMap < String^, Object^>^ get() {
				if (m_data == nullptr) {
					m_data = ref new Map<String^, Object^>{};
				}
				return m_data;
			}
		}
		void Add(VarDesc^ pVar, Object^ val);
		Object^ GetValue(VarDesc^ pVar);
	}; // class IndivDesc
	//
	public ref class DatasetDesc sealed {
	private:
		String^ m_name;
		IVector<VarDesc^>^ m_vars;
		IVector<IndivDesc^>^ m_inds;
		IVector<int>^ m_varindex;
		IVector<int>^ m_indindex;
		//
		void check_varindex(void);
		void check_indindex(void);
	public:
		DatasetDesc();
		property String^ Name
		{
			String^ get() {
				if (m_name == nullptr) {
					m_name = ref new String{};
				}
				return m_name;
			}
			void set(String^ value) {
				m_name = value;
			}
		}// Name
		property IVector<VarDesc^>^ Variables {
			IVector<VarDesc^>^ get() {
				if (m_vars == nullptr) {
					m_vars = ref new Vector<VarDesc^>{};
				}
				return m_vars;
			}
		}// Variables
		property IVector<IndivDesc^>^ Indivs {
			IVector<IndivDesc^>^ get() {
				if (m_inds == nullptr) {
					m_inds = ref new Vector<IndivDesc^>{};
				}
				return m_inds;
			}
		}// Variables
		property IVector<int>^ VarIndexes {
			IVector<int>^ get() {
				if (m_varindex == nullptr) {
					m_varindex = ref new Vector<int>{};
				}
				return m_varindex;
			}
		}
		property IVector<int>^ IndivIndexes {
			IVector<int>^ get() {
				if (m_indindex == nullptr) {
					m_indindex = ref new Vector<int>{};
				}
				return m_indindex;
			}
		}
		property int VariablesCount {
			int get() {
				return static_cast<int>(this->Variables->Size);
			}
		}
		property int IndivsCount {
			int get() {
				return static_cast<int>(this->Indivs->Size);
			}
		}
		//
		VarDesc^ FindVariable(String^ sigle);
		IndivDesc^ FindIndiv(String^ sigle);
		void Add(String^ indId, String^ varId, Object^ val);
		Object^ GetValue(IndivDesc^ pInd, VarDesc^ pVar);
		VarDesc^ AddVariable(String^ varid);
		IndivDesc^ AddIndiv(String^ indid);
		VarDesc^ FindVariableAt(int pos);
		IndivDesc^ FindIndivAt(int pos);
	};// class DatasetDesc
	//
}// namespace InfoProcs
