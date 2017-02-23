#pragma once
#include <memory>
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Platform::Collections;
////////////////////////
namespace InfoDomain
{
	//
	public enum class InfoStatus :int { Unknown=0, Normal=1, 
		Tail=2, Disabled=4, Info=8, Inactive=16, 
		Inserted=32, Updated=64, Deleted=128 };
	public enum class InfoKind :int{ 
		Unknown = 0, Normal=1, Modal=2, Ordinal = 4 };
	public enum class InfoDataType :int {
		Unknown = 0, Logical=1, Integer=2, Real=4, Text=8, Other=16 };
	//
	class InfoDataValueImpl {
	private:
		InfoDataType m_type;
		union {
			bool m_bval;
			int  m_ival;
			double m_dval;
		};
		  std::wstring m_sval;
	public:
		InfoDataValueImpl() :m_type(InfoDataType::Unknown) {
			m_bval = false;
			m_ival = 0;
			m_dval = 0;
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
			return ref new String(m_sval.c_str());
		}
		void set_StringValue(String^ v) {
			m_sval = std::wstring{ v->Data() };
			m_type = InfoDataType::Text;
		}
		String^ ToString(void) const {
			Object^ oRet = nullptr;
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
			return (oRet != nullptr)? oRet->ToString(): "";
		}
		Object^ get_Value(void) const {
			Object^ oRet = nullptr;
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
			m_sval.clear();
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
		virtual bool get_IsPersisted(void) const;
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
		String^ ToString(void) const {
			return (m_sigle == nullptr) ? "" : m_sigle;
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
		virtual bool get_IsStoreable(void) const override;
		virtual void GetMap(IMap<String^, Object^>^ oMap) const override;
	};// class SigleNamedItemImpl
	class DatasetImpl : public SigleNamedItemImpl {
	private:
		String^ m_annee;
	public:
		DatasetImpl();
		DatasetImpl(IMap<String^, Object^>^ pMap);
		DatasetImpl(String^ sSigle);
		virtual ~DatasetImpl();
		virtual void GetMap(IMap<String^, Object^>^ oMap) const;
		String^ get_Annee(void) const {
			return m_annee;
		}
		void set_Annee(String^ s) {
			m_annee = s;
		}
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
		virtual bool get_IsStoreable(void) const override;
		virtual void GetMap(IMap<String^, Object^>^ oMap) const override;
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
		IVector<String^>^ m_modalites;
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
		IVector<String^>^ get_Modalites(void);
		void set_Modalites(IVector<String^>^ pVec);
		virtual bool get_IsStoreable(void) const override;
		virtual void GetMap(IMap<String^, Object^>^ oMap) const override;
	};// class VariableImpl
	//
	class EtudiantImpl : public InfoItemImpl {
	private:
		String^ m_annee;
		String^ m_dossier;
		String^ m_sexe;
		String^ m_birth;
		String^ m_firstname;
		String^ m_lastname;
		String^ m_dep;
		String^ m_ville;
		String^ m_lycee;
		String^ m_groupe;
		String^ m_seriebac;
		String^ m_optionbac;
		String^ m_mentionbac;
		String^ m_apb;
		String^ m_sup;
		String^ m_red;
		String^ m_avatar;
		String^ m_email;
		IVector<String^>^ m_sets;
	public:
		EtudiantImpl();
		EtudiantImpl(IMap<String^, Object^>^ pMap);
		virtual ~EtudiantImpl();
	public:
		String^ get_Dossier(void) const {
			return m_dossier;
		}
		void set_Dossier(String^ s) {
			m_dossier = s;
		}
		String^ get_Sexe(void) const {
			return m_sexe;
		}
		void set_Sexe(String^ s) {
			m_sexe = s;
		}
		String^ get_Birth(void) const {
			return m_birth;
		}
		void set_Birth(String^ s) {
			m_birth = s;
		}
		String^ get_Firstname(void) const {
			return m_firstname;
		}
		void set_Firstname(String^ s) {
			m_firstname = s;
		}
		String^ get_Lastname(void) const {
			return m_lastname;
		}
		void set_Lastname(String^ s) {
			m_lastname = s;
		}
		String^ get_Departement(void) const {
			return m_dep;
		}
		void set_Departement(String^ s) {
			m_dep = s;
		}
		String^ get_Ville(void) const {
			return m_ville;
		}
		void set_Ville(String^ s) {
			m_ville = s;
		}
		String^ get_Lycee(void) const {
			return m_lycee;
		}
		void set_Lycee(String^ s) {
			m_lycee = s;
		}
		String^ get_Groupe(void) const {
			return m_groupe;
		}
		void set_Groupe(String^ s) {
			m_groupe = s;
		}
		String^ get_Annee(void) const {
			return m_annee;
		}
		void set_Annee(String^ s) {
			m_annee = s;
		}
		String^ get_SerieBac(void) const {
			return m_seriebac;
		}
		void set_SerieBac(String^ s) {
			m_seriebac = s;
		}
		String^ get_OptionBac(void) const {
			return m_optionbac;
		}
		void set_OptionBac(String^ s) {
			m_optionbac = s;
		}
		String^ get_MentionBac(void) const {
			return m_mentionbac;
		}
		void set_MentionBac(String^ s) {
			m_mentionbac = s;
		}
		String^ get_EtudesSuperieures(void) const {
			return m_sup;
		}
		void set_EtudesSuperieures(String^ s) {
			m_sup = s;
		}
		String^ get_Apb(void) const {
			return m_apb;
		}
		void set_Apb(String^ s) {
			m_apb = s;
		}
		String^ get_Redoublant(void) const {
			return m_red;
		}
		void set_Redoublant(String^ s) {
			m_red = s;
		}
		String^ get_Avatar(void) const {
			return m_avatar;
		}
		void set_Avatar(String^ s) {
			m_avatar = s;
		}
		String^ get_Email(void) const {
			return m_email;
		}
		void set_Email(String^ s) {
			m_email = s;
		}
		IVector<String^>^ get_DatasetsSigles(void){
			if (m_sets == nullptr) {
				m_sets = ref new Vector<String^>();
			}
			return m_sets;
		}
		void set_DatasetsSigles(IVector<String^>^ pVec) {
			m_sets = pVec;
		}
		virtual bool get_IsStoreable(void) const override;
		virtual void GetMap(IMap<String^, Object^>^ oMap) const;
		virtual String^ get_Type(void) const override;
	};// class EtudiantImpl
	//
	}// namespace InfoDommain
