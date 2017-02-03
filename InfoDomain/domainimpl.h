#pragma once
#include <memory>
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
namespace InfoDomain
{
	//
	public enum class InfoStatus { Unknown, Normal, Tail, Disabled, Info, Inactive, Inserted, Updated, Deleted };
	public enum class InfoKind { Unknown, Normal, Modal, Ordinal };
	public enum class InfoDataType { Unknown, Logical, Integer, Real, Text, Other };
	//
	class InfoDataValueImpl {
	private:
		InfoDataType m_type;
		union {
			bool m_bval;
			int  m_ival;
			double m_dval;
			std::wstring m_sval;
		};
	public:
		InfoDataValueImpl() :m_type(InfoDataType::Unknown) {
		}
		InfoDataValueImpl(bool b) :m_type(InfoDataType::Logical) {
			m_bval = b;
		}
		InfoDataValueImpl(int b) :m_type(InfoDataType::Integer) {
			m_ival = b;
		}
		InfoDataValueImpl(double b) :m_type(InfoDataType::Real) {
			m_dval = b;
		}
		InfoDataValueImpl(Platform::String^ b) :m_type(InfoDataType::Text) {
			m_sval = std::wstring{b->Data()};
		}
		~InfoDataValueImpl(){}
		InfoDataType get_DataType(void) const {
			return (m_type);
		}
		bool get_HasValue(void) const {
			return (m_type != InfoDataType::Unknown);
		}
		bool get_BoolValue(void) const {
			return (this->m_bval);
		}
		void set_BoolValue(bool b) {
			m_bval = b;
			m_type = InfoDataType::Logical;
		}
		int get_IntValue(void) const {
			return (m_ival);
		}
		void set_IntValue(int v) {
			m_ival = v;
			m_type = InfoDataType::Integer;
		}
		double get_DoubleValue(void) const {
			return (m_dval);
		}
		void set_DoubleValue(double v) {
			m_dval = v;
			m_type = InfoDataType::Real;
		}
		String^ get_StringValue(void) const {
			String^ r = ref new String(m_sval.c_str());
			return r;
		}
		void set_StringValue(String^ v) {
			m_sval = std::wstring{ v->Data() };
			m_type = InfoDataType::Text;
		}
		Object^ get_Value(void) const {
			Object^ oRet;
			switch (m_type) {
			case InfoDataType::Logical:
				oRet = m_bval;
				break;
			case InfoDataType::Integer:
				oRet = m_ival;
				break;
			case InfoDataType::Real:
				oRet = m_dval;
				break;
			case InfoDataType::Text:
				oRet = this->get_StringValue();
				break;
			}// type
			return (oRet);
		}
		void Clear(void) {
			m_type = InfoDataType::Unknown;
		}
	};// class IInfoDataValueImpl
	//
	class InfoItemImpl {
	private:
		InfoStatus m_status;
		String^ m_id;
		String^ m_rev;
		String^ m_desc;
	protected:
		InfoItemImpl();
		InfoItemImpl(IMap<String^, Object^>^ pMap);
	public:
		virtual ~InfoItemImpl();
		String^ get_Id(void) const {
			return this->m_id;
		}
		void set_Id(String^ id) {
			this->m_id = id;
		}
		String^ get_Rev(void) const {
			return this->m_rev;
		}
		void set_Rev(String^ s) {
			this->m_rev = s;
		}
		InfoStatus get_Status(void) const {
			return this->m_status;
		}
		void set_Status(InfoStatus t) {
			this->m_status = t; 
		}
		String^ get_Observations(void) const {
			return this->m_desc;
		}
		void set_Observations(String^ s) {
			this->m_desc = s;
		}
		//
		virtual bool get_IsStoreable(void) const;
		virtual String^ get_Type(void) const;
		virtual void GetMap(IMap<String^, Object^>^ oMap) const;
	};// InfoItemImpl
	class SigleNamedItemImpl : public InfoItemImpl {
	private:
		String^ m_sigle;
		String^ m_name;
	protected:
		SigleNamedItemImpl();
		SigleNamedItemImpl(String^ sSigle);
		SigleNamedItemImpl(IMap<String^, Object^>^ pMap);
	public:
		virtual ~SigleNamedItemImpl();
		String^ get_Sigle(void) const {
			return m_sigle;
		}
		void set_Sigle(String^ s) {
			m_sigle = s; 
		}
		String^ get_Name(void) const {
			return m_name;
		}
		void set_Name(String^ name) {
			m_name = name;
		}
		virtual bool get_IsStoreable(void) const;
		virtual void GetMap(IMap<String^, Object^>^ oMap) const;
	};// class SigleNamedItemImpl
	class DatasetImpl : public SigleNamedItemImpl {
	public:
		DatasetImpl();
		DatasetImpl(IMap<String^, Object^>^ pMap);
		DatasetImpl(String^ sSigle);
		virtual ~DatasetImpl();
	};// class DatasetImpl
	//
	class DatasetChildItemImpl : public SigleNamedItemImpl {
	private:
		String^ m_datasetsigle;
	protected:
		DatasetChildItemImpl();
		DatasetChildItemImpl(IMap<String^, Object^>^ pMap);
		DatasetChildItemImpl(String^ setsigle, String^ sigle);
	public:
		virtual ~DatasetChildItemImpl();
		String^ get_DatasetSigle(void) const {
			return m_datasetsigle;
		}
		void set_DatasetSigle(String^ s) {
			m_datasetsigle = s;
		}
		virtual bool get_IsStoreable(void) const;
		virtual void GetMap(IMap<String^, Object^>^ oMap) const;
	};
	//
	class IndivImpl : public DatasetChildItemImpl {
	public:
		IndivImpl();
		IndivImpl(IMap<String^, Object^>^ pMap);
		IndivImpl(String^ setsigle, String^ sigle);
		virtual ~IndivImpl();
	};// class DatasetChildItemImp
	//
	class VariableImpl : public DatasetChildItemImpl {
	private:
		InfoDataType m_datatype;
		InfoKind m_kind;
		std::vector<std::wstring> m_modalites;
	public:
		VariableImpl();
		VariableImpl(IMap<String^, Object^>^ pMap);
		VariableImpl(String^ setsigle, String^ sigle);
		virtual ~VariableImpl();
		//
		InfoDataType get_VariableType(void) const {
			return m_datatype;
		}
		void set_VariableType(InfoDataType p) {
			m_datatype = p;
		}
		InfoKind get_VariableKind(void) const {
			return m_kind;
		}
		void set_VariableKind(InfoKind k) {
			m_kind = k;
		}
		IVector<String^>^ get_Modalites(void) const;
		void set_Modalites(IVector<String^>^ pVec);
		virtual bool get_IsStoreable(void) const;
		virtual void GetMap(IMap<String^, Object^>^ oMap) const;
	};// class VariableImpl
	//
	class InfoValueImpl : public InfoItemImpl {
	private:
		String^ m_datasetsigle;
		String^ m_indivsigle;
		String^ m_variablesigle;
	};// class InfoValueImpl
}// namespace InfoDommain
