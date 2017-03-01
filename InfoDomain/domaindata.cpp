#include "pch.h"
#include "domaindata.h"
#include "domainimpl.h"
#include "infostrings.h"
//////////////////////////////
#include <sstream>
//////////////////////////////////
//
using namespace Platform;
using namespace Platform::Collections;
//
namespace InfoDomain {
	///////////////////////////////////////
	InfoBlob::InfoBlob() {

	}
	InfoBlob::InfoBlob(String^ name, String^ mime, IVector<uint8>^ data) :
		m_name(name), m_mime(mime), m_data(data) {
	}
	String^ InfoBlob::Name::get() {
		return (m_name);
	}
	void InfoBlob::Name::set(String^ s) {
		m_name = s;
	}
	String^ InfoBlob::MimeType::get() {
		return (m_mime);
	}
	void InfoBlob::MimeType::set(String^ s) {
		m_mime = s;
	}
	IVector<uint8>^ InfoBlob::Data::get() {
		return (m_data);
	}
	void InfoBlob::Data::set(IVector<uint8>^ data) {
		m_data = data;
	}
	///////////////////////////////////////
	InfoDataValue::InfoDataValue() {
	}
	InfoDataValue::InfoDataValue(bool b) :m_impl(b) {
	}
	InfoDataValue::InfoDataValue(int b) : m_impl(b) {}
	InfoDataValue::InfoDataValue(double b) : m_impl(b) {}
	InfoDataValue::InfoDataValue(String^ b) : m_impl(b) {}
	//
	InfoDataType InfoDataValue::DataType::get() {
		return m_impl.get_DataType();
	}
	bool InfoDataValue::HasValue::get() {
		return m_impl.get_HasValue();
	}// HasValue
	bool InfoDataValue::BoolValue::get() {
		return m_impl.get_BoolValue();
	}
	void InfoDataValue::BoolValue::set(bool b) {
		m_impl.set_BoolValue(b);
	}// BoolValue
	int  InfoDataValue::IntValue::get() {
		return m_impl.get_IntValue();
	}
	void InfoDataValue::IntValue::set(int b) {
		m_impl.set_IntValue(b);
	}
	double InfoDataValue::DoubleValue::get() {
		return m_impl.get_DoubleValue();
	}
	void InfoDataValue::DoubleValue::set(double b) {
		m_impl.set_DoubleValue(b);
	}
	String^ InfoDataValue::StringValue::get() {
		return m_impl.get_StringValue();
	}
	void InfoDataValue::StringValue::set(String^ b) {
		m_impl.set_StringValue(b);
	}
	Object^ InfoDataValue::Value::get() {
		return m_impl.get_Value();
	}
	void InfoDataValue::Clear(void) {
		m_impl.Clear();
	}
	String^ InfoDataValue::ToString(void) {
		return m_impl.ToString();
	}
	///////////////////////////////
	Dataset::Dataset() :m_pimpl(new DatasetImpl{}) {}
	Dataset::Dataset(IMap<String^, Object^>^ pMap) : m_pimpl(new DatasetImpl{ pMap }) {}
	Dataset::Dataset(String^ sSigle) : m_pimpl(new DatasetImpl{ sSigle }) {}
	//
	String^ Dataset::Id::get() {
		return m_pimpl->get_Id();
	}
	void Dataset::Id::set(String ^s) {
		m_pimpl->set_Id(s);
	}
	void Dataset::Rev::set(String ^s) {
		m_pimpl->set_Rev(s);
	}
	String^ Dataset::Rev::get() {
		return m_pimpl->get_Rev();
	}

	bool Dataset::IsPersisted::get() {
		return m_pimpl->get_IsPersisted();
	}// IsPersisted
	InfoStatus Dataset::Status::get() {
		return m_pimpl->get_Status();
	}
	void Dataset::Status::set(InfoStatus value) {
		m_pimpl->set_Status(value);
	}
	String^ Dataset::Observations::get() {
		return m_pimpl->get_Observations();
	}
	void Dataset::Observations::set(String^ value) {
		m_pimpl->set_Observations(value);
	}
	bool Dataset::IsStoreable::get() {
		return m_pimpl->get_IsStoreable();
	}// IsStoreable
	String^ Dataset::Sigle::get() {
		return m_pimpl->get_Sigle();
	}
	void Dataset::Sigle::set(String^ value) {
		m_pimpl->set_Sigle(value);
	}
	String^ Dataset::Name::get() {
		return m_pimpl->get_Name();
	}
	void Dataset::Name::set(String^ value) {
		m_pimpl->set_Name(value);
	}
	IMap<String^, Object^>^ Dataset::GetMap(void) {
		Map<String^, Object^>^ oMap = ref new Map<String^, Object^>();
		m_pimpl->GetMap(oMap);
		return oMap;
	}
	String^ Dataset::ToString(void) {
		return m_pimpl->ToString();
	}
	Variable ^ Dataset::FindVariable(String ^ sigle)
	{
		Variable^ pRet = nullptr;
		if ((sigle != nullptr) && (!sigle->IsEmpty())) {
			auto it = this->Variables->First();
			while ((pRet == nullptr) && it->HasCurrent) {
				Variable^ p = it->Current;
				if (p->Sigle == sigle) {
					pRet = p;
					break;
				}
				it->MoveNext();
			}// while
		}// sigle
		return pRet;
	}
	Indiv ^ Dataset::FindIndiv(String ^ sigle)
	{
		Indiv^ pRet = nullptr;
		if ((sigle != nullptr) && (!sigle->IsEmpty())) {
			auto it = this->Indivs->First();
			while ((pRet == nullptr) && it->HasCurrent) {
				Indiv^ p = it->Current;
				if (p->Sigle == sigle) {
					pRet = p;
					break;
				}
				it->MoveNext();
			}// while
		}// sigle
		return pRet;
	}
	IVector<Variable^>^ Dataset::Variables::get() {
		if (m_vars == nullptr) {
			m_vars = ref new Vector<Variable^>();
		}
		return m_vars;
	}
	void Dataset::Variables::set(IVector<Variable^>^ value) {
		m_vars = value;
	}
	IVector<Indiv^>^ Dataset::Indivs::get() {
		if (m_inds == nullptr) {
			m_inds = ref new Vector<Indiv^>();
		}
		return m_inds;
	}
	void Dataset::Indivs::set(IVector<Indiv^>^ value) {
		m_inds = value;
	}
	String^ Dataset::Annee::get() {
		return m_pimpl->get_Annee();
	}
	void Dataset::Annee::set(String^ value) {
		m_pimpl->set_Annee(value);
	}
	IVector<InfoBlob^>^ Dataset::Blobs::get() {
		if (m_blobs == nullptr) {
			m_blobs = ref new Vector<InfoBlob^>();
		}
		return m_blobs;
	}
	void Dataset::Blobs::set(IVector<InfoBlob^>^ value) {
		m_blobs = value;
	}
	//////////////////////////////////
	Indiv::Indiv() :m_pimpl(new IndivImpl{}) {}
	Indiv::Indiv(Dataset^ pSet, String^ sigle) : m_pimpl(new IndivImpl{ pSet->Sigle, sigle }) {
		m_set = pSet;
	}
	Indiv::Indiv(IMap<String^, Object^>^ pMap) : m_pimpl(new IndivImpl{ pMap }) {}
	String^ Indiv::DatasetSigle::get() {
		return m_pimpl->get_DatasetSigle();
	}
	String^ Indiv::Id::get() {
		return m_pimpl->get_Id();
	}
	String^ Indiv::Rev::get() {
		return m_pimpl->get_Rev();
	}
	void Indiv::Id::set(String ^s) {
		m_pimpl->set_Id(s);
	}
	void Indiv::Rev::set(String ^s) {
		m_pimpl->set_Rev(s);
	}
	bool Indiv::IsPersisted::get() {
		return m_pimpl->get_IsPersisted();
	}
	InfoStatus Indiv::Status::get() {
		return m_pimpl->get_Status();
	}
	void Indiv::Status::set(InfoStatus value) {
		m_pimpl->set_Status(value);
	}
	String^ Indiv::Observations::get() {
		return m_pimpl->get_Observations();
	}
	void Indiv::Observations::set(String^ value) {
		m_pimpl->set_Observations(value);
	}
	bool Indiv::IsStoreable::get() {
		return m_pimpl->get_IsStoreable();
	}
	String^ Indiv::Sigle::get() {
		return m_pimpl->get_Sigle();
	}
	void Indiv::Sigle::set(String^ value) {
		m_pimpl->set_Sigle(value);
	}
	String^ Indiv::Name::get() {
		return m_pimpl->get_Name();
	}
	void Indiv::Name::set(String^ value) {
		m_pimpl->set_Name(value);
	}// name
	IMap<String^, Object^>^ Indiv::GetMap(void) {
		Map<String^, Object^>^ oMap = ref new Map<String^, Object^>();
		m_pimpl->GetMap(oMap);
		return oMap;
	}
	String^ Indiv::ToString(void) {
		return m_pimpl->ToString();
	}
	IVector<InfoValue^>^ Indiv::Values::get() {
		if (m_vals == nullptr) {
			m_vals = ref new Vector<InfoValue^>();
		}
		return m_vals;
	}
	void Indiv::Values::set(IVector<InfoValue^>^ value) {
		m_vals = value;
	}
	IVector<InfoBlob^>^ Indiv::Blobs::get() {
		if (m_blobs == nullptr) {
			m_blobs = ref new Vector<InfoBlob^>();
		}
		return m_blobs;
	}
	void Indiv::Blobs::set(IVector<InfoBlob^>^ value) {
		m_blobs = value;
	}
	///////////////////////////////////////
	Variable::Variable() :m_pimpl(new VariableImpl{}) {}
	Variable::Variable(Dataset^ pSet, String^ sigle) : m_pimpl(new VariableImpl{ pSet->Sigle, sigle }) {
		m_set = pSet;
	}
	Variable::Variable(IMap<String^, Object^>^ pMap) : m_pimpl(new VariableImpl{ pMap }) {}
	String^ Variable::DatasetSigle::get() {
		return m_pimpl->get_DatasetSigle();
	}
	String^ Variable::Id::get() {
		return m_pimpl->get_Id();
	}
	String^ Variable::Rev::get() {
		return m_pimpl->get_Rev();
	}
	void Variable::Id::set(String ^s) {
		m_pimpl->set_Id(s);
	}
	void Variable::Rev::set(String ^s) {
		m_pimpl->set_Rev(s);
	}
	bool Variable::IsPersisted::get() {
		return m_pimpl->get_IsPersisted();
	}// IsPersisted
	InfoStatus Variable::Status::get() {
		return m_pimpl->get_Status();
	}
	void Variable::Status::set(InfoStatus value) {
		m_pimpl->set_Status(value);
	}
	String^ Variable::Observations::get() {
		return m_pimpl->get_Observations();
	}
	void Variable::Observations::set(String^ value) {
		m_pimpl->set_Observations(value);
	}
	bool Variable::IsStoreable::get() {
		return m_pimpl->get_IsStoreable();
	}
	String^ Variable::Sigle::get() {
		return m_pimpl->get_Sigle();
	}
	void Variable::Sigle::set(String^ value) {
		m_pimpl->set_Sigle(value);
	}
	String^ Variable::Name::get() {
		return m_pimpl->get_Name();
	}
	void Variable::Name::set(String^ value) {
		m_pimpl->set_Name(value);
	}
	InfoDataType  Variable::VariableType::get() {
		return m_pimpl->get_VariableType();
	}
	void Variable::VariableType::set(InfoDataType value) {
		m_pimpl->set_VariableType(value);
	}
	InfoKind  Variable::VariableKind::get() {
		return m_pimpl->get_VariableKind();
	}
	void Variable::VariableKind::set(InfoKind value) {
		m_pimpl->set_VariableKind(value);
	}
	IVector<String^>^ Variable::Modalites::get() {
		return m_pimpl->get_Modalites();
	}
	void Variable::Modalites::set(IVector<String^>^ value) {
		m_pimpl->set_Modalites(value);
	}
	IMap<String^, Object^>^ Variable::GetMap(void) {
		Map<String^, Object^>^ oMap = ref new Map<String^, Object^>();
		m_pimpl->GetMap(oMap);
		return oMap;
	}
	String^ Variable::ToString(void) {
		return m_pimpl->ToString();
	}
	IVector<InfoValue^>^ Variable::Values::get() {
		if (m_vals == nullptr) {
			m_vals = ref new Vector<InfoValue^>();
		}
		return m_vals;
	}
	void Variable::Values::set(IVector<InfoValue^>^ value) {
		m_vals = value;
	}
	IVector<InfoBlob^>^ Variable::Blobs::get() {
		if (m_blobs == nullptr) {
			m_blobs = ref new Vector<InfoBlob^>();
		}
		return m_blobs;
	}
	void Variable::Blobs::set(IVector<InfoBlob^>^ value) {
		m_blobs = value;
	}
	////////////////////////////////////
	InfoValue::InfoValue() :m_modified(false),m_selected(false),m_status(InfoStatus::Unknown) {}
	InfoValue::InfoValue(IMap<String^, Object^>^ oMap) : m_modified(false), m_selected(false), m_status(InfoStatus::Unknown) {
		if (oMap->HasKey(InfoStrings::KEY_ID)) {
			m_id = oMap->Lookup(InfoStrings::KEY_ID)->ToString();
		}
		if (oMap->HasKey(InfoStrings::KEY_REV)) {
			m_rev = oMap->Lookup(InfoStrings::KEY_REV)->ToString();
		}
		if (oMap->HasKey(InfoStrings::KEY_DESC)) {
			m_desc = oMap->Lookup(InfoStrings::KEY_DESC)->ToString();
		}
		if (oMap->HasKey(InfoStrings::KEY_STATUS)) {
			Object^ o = oMap->Lookup(InfoStrings::KEY_STATUS);
			if (o != nullptr) {
				String^ s = o->ToString();
				std::wstring ss{ s->Data() };
				std::wstringstream in{ ss };
				int n = 0;
				in >> n;
				m_status = static_cast<InfoStatus>(n);
			}
		}// status
		if (oMap->HasKey(InfoStrings::KEY_DATASETSIGLE)) {
			m_datasetsigle = oMap->Lookup(InfoStrings::KEY_DATASETSIGLE)->ToString();
		}
		if (oMap->HasKey(InfoStrings::KEY_INDIVSIGLE)) {
			m_indivsigle = oMap->Lookup(InfoStrings::KEY_INDIVSIGLE)->ToString();
		}
		if (oMap->HasKey(InfoStrings::KEY_VARIABLESIGLE)) {
			m_variablesigle = oMap->Lookup(InfoStrings::KEY_VARIABLESIGLE)->ToString();
		}
		if (oMap->HasKey(InfoStrings::KEY_VALUE) && oMap->HasKey(InfoStrings::KEY_DATATYPE)) {
			Object^ oval = oMap->Lookup(InfoStrings::KEY_VALUE);
			if (oval != nullptr) {
				String^ sv = oval->ToString();
				Object^ ov = oMap->Lookup(InfoStrings::KEY_DATATYPE);
				if (ov != nullptr) {
					String^ s = ov->ToString();
					std::wstring ss{ s->Data() };
					std::wstringstream in{ ss };
					int n = 0;
					in >> n;
					InfoDataType t = static_cast<InfoDataType>(n);
					if (t != InfoDataType::Unknown) {
						switch (t) {
						case InfoDataType::Text:
						{
							m_value = ref new InfoDataValue(sv);
						}
						break;
						case InfoDataType::Logical:
						{
							std::wstring ssx{ sv->Data() };
							std::wstringstream in2{ ssx };
							bool n{ false };
							in2 >> n;
							m_value = ref new InfoDataValue(n);
						}
						break;
						case InfoDataType::Real:
						{
							std::wstring ssx{ sv->Data() };
							std::wstringstream in2{ ssx };
							double n = 0;
							in2 >> n;
							m_value = ref new InfoDataValue(n);
						}
						break;
						case InfoDataType::Integer:
						{
							std::wstring ssx{ sv->Data() };
							std::wstringstream in2{ ssx };
							int n = 0;
							in2 >> n;
							m_value = ref new InfoDataValue(n);
						}
						break;
						default:
							break;
						}// m_datatype
					}// type
				} // ov
			}// datatype
		}// val
	}
	InfoValue::InfoValue(Indiv^ pInd, Variable^ pVar) :m_status(InfoStatus::Unknown) {
		if (pInd != nullptr) {
			m_datasetsigle = pInd->DatasetSigle;
			m_indivsigle = pInd->Sigle;
			m_ind = pInd;
		}
		if (pVar != nullptr) {
			m_var = pVar;
			m_variablesigle = pVar->Sigle;
			if (m_datasetsigle->IsEmpty()) {
				m_datasetsigle = pVar->DatasetSigle;
			}
		}
	}
	InfoValue::InfoValue(Indiv^ pInd, Variable^ pVar, InfoDataValue^ val) :m_status(InfoStatus::Unknown) {
		if (pInd != nullptr) {
			m_datasetsigle = pInd->DatasetSigle;
			m_indivsigle = pInd->Sigle;
		}
		if (pVar != nullptr) {
			m_variablesigle = pVar->Sigle;
			if (m_datasetsigle->IsEmpty()) {
				m_datasetsigle = pVar->DatasetSigle;
			}
		}
		m_value = val;
	}
	//
	String^ InfoValue::Id::get() {
		return m_id;
	}
	String^ InfoValue::Rev::get() {
		return m_rev;
	}
	void InfoValue::Id::set(String ^s) {
		m_id = s;
	}
	void InfoValue::Rev::set(String ^s) {
		m_rev = s;
	}
	bool InfoValue::IsPersisted::get() {
		return (m_id != nullptr) && (m_rev != nullptr) &&
			(!m_id->IsEmpty()) && (!m_rev->IsEmpty());
	}// IsPersisted
	InfoStatus InfoValue::Status::get() {
		return m_status;
	}
	void InfoValue::Status::set(InfoStatus value) {
		m_status = value;
	}
	String^ InfoValue::Observations::get() {
		return m_desc;
	}
	void InfoValue::Observations::set(String^ value) {
		m_desc = value;
	}
	String^ InfoValue::DatasetSigle::get() {
		return m_datasetsigle;
	}
	String^ InfoValue::IndivSigle::get() {
		return m_indivsigle;
	}
	String^ InfoValue::VariableSigle::get() {
		return m_variablesigle;
	}
	InfoDataValue^ InfoValue::Value::get() {
		return m_value;
	}
	String^ InfoValue::ToString(void) {
		return (m_value != nullptr) ? m_value->ToString(): "";
	}
	void InfoValue::Value::set(InfoDataValue^ value) {
		m_value = value;
	}
	bool InfoValue::IsStoreable::get() {
		return (m_datasetsigle != nullptr) && (m_indivsigle != nullptr) &&
			(m_variablesigle != nullptr) && (!m_datasetsigle->IsEmpty()) && (!m_indivsigle->IsEmpty()) &&
			(!m_variablesigle->IsEmpty());
	}
	IMap<String^, Object^>^ InfoValue::GetMap(void) {
		Map<String^, Object^>^ oMap = ref new Map<String^, Object^>();
		oMap->Insert(InfoStrings::KEY_TYPE, this->InfoType);
		if (m_status != InfoStatus::Unknown) {
			int ival = static_cast<int>(m_status);
			oMap->Insert(InfoStrings::KEY_STATUS, ival);
		}
		if ((m_id != nullptr) && (!m_id->IsEmpty())) {
			oMap->Insert(InfoStrings::KEY_ID, m_id);
		}
		if ((m_rev != nullptr) && (!m_rev->IsEmpty())) {
			oMap->Insert(InfoStrings::KEY_REV, m_rev);
		}
		if ((m_desc != nullptr) && (!m_desc->IsEmpty())) {
			oMap->Insert(InfoStrings::KEY_DESC, m_desc);
		}
		if ((m_datasetsigle != nullptr) && (!m_datasetsigle->IsEmpty())) {
			oMap->Insert(InfoStrings::KEY_DATASETSIGLE, m_datasetsigle);
		}
		if ((m_indivsigle != nullptr) && (!m_indivsigle->IsEmpty())) {
			oMap->Insert(InfoStrings::KEY_INDIVSIGLE, m_indivsigle);
		}
		if ((m_variablesigle != nullptr) && (!m_variablesigle->IsEmpty())) {
			oMap->Insert(InfoStrings::KEY_VARIABLESIGLE, m_variablesigle);
		}
		if (m_value != nullptr) {
			if (m_value->HasValue) {
				InfoDataType t = m_value->DataType;
				Object^ vo = m_value->Value;
				if ((t != InfoDataType::Unknown) && (vo != nullptr)) {
					int ival = static_cast<int>(t);
					oMap->Insert(InfoStrings::KEY_DATATYPE, ival);
					oMap->Insert(InfoStrings::KEY_VALUE, vo);
				}
			}// value
		}// value
		return oMap;
	}// GetMap
	IVector<InfoBlob^>^ InfoValue::Blobs::get() {
		if (m_blobs == nullptr) {
			m_blobs = ref new Vector<InfoBlob^>();
		}
		return m_blobs;
	}
	void InfoValue::Blobs::set(IVector<InfoBlob^>^ value) {
		m_blobs = value;
	}
	/////////////////////////////////////
}// Namespace InfoDomain
