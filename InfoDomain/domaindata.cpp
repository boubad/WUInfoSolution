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
	InfoBlob::InfoBlob(Platform::String^ name, Platform::String^ mime, IVector<uint8>^ data) :
		m_name(name), m_mime(mime), m_data(data) {
	}
	Platform::String^ InfoBlob::Name::get() {
		return (m_name);
	}
	void InfoBlob::Name::set(Platform::String^ s) {
		m_name = s;
	}
	Platform::String^ InfoBlob::MimeType::get() {
		return (m_mime);
	}
	void InfoBlob::MimeType::set(Platform::String^ s) {
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
	InfoDataValue::InfoDataValue(Platform::String^ b) : m_impl(b) {}
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
	Platform::String^ InfoDataValue::StringValue::get() {
		return m_impl.get_StringValue();
	}
	void InfoDataValue::StringValue::set(Platform::String^ b) {
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
	Dataset::Dataset(IMap<Platform::String^, Object^>^ pMap) : m_pimpl(new DatasetImpl{ pMap }) {}
	Dataset::Dataset(Platform::String^ sSigle) : m_pimpl(new DatasetImpl{ sSigle }) {}
	//
	Platform::String^ Dataset::Id::get() {
		return m_pimpl->get_Id();
	}
	Platform::String^ Dataset::Rev::get() {
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
	Platform::String^ Dataset::Observations::get() {
		return m_pimpl->get_Observations();
	}
	void Dataset::Observations::set(Platform::String^ value) {
		m_pimpl->set_Observations(value);
	}
	Platform::String^ Dataset::Type::get() {
		return InfoStrings::TYPE_DATASET;
	}// Type
	bool Dataset::IsStoreable::get() {
		return m_pimpl->get_IsStoreable();
	}// IsStoreable
	Platform::String^ Dataset::Sigle::get() {
		return m_pimpl->get_Sigle();
	}
	void Dataset::Sigle::set(Platform::String^ value) {
		m_pimpl->set_Sigle(value);
	}
	Platform::String^ Dataset::Name::get() {
		return m_pimpl->get_Name();
	}
	void Dataset::Name::set(Platform::String^ value) {
		m_pimpl->set_Name(value);
	}
	IMap<Platform::String^, Object^>^ Dataset::GetMap(void) {
		Map<Platform::String^, Object^>^ oMap = ref new Map<Platform::String^, Object^>();
		m_pimpl->GetMap(oMap);
		return oMap;
	}
	Platform::String^ Dataset::ToString(void) {
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
		return m_vars;
	}
	void Dataset::Variables::set(IVector<Variable^>^ value) {
		m_vars = value;
	}
	IVector<Indiv^>^ Dataset::Indivs::get() {
		return m_inds;
	}
	void Dataset::Indivs::set(IVector<Indiv^>^ value) {
		m_inds = value;
	}
	Platform::String^ Dataset::Annee::get() {
		return m_pimpl->get_Annee();
	}
	void Dataset::Annee::set(Platform::String^ value) {
		m_pimpl->set_Annee(value);
	}
	//////////////////////////////////
	Indiv::Indiv() :m_pimpl(new IndivImpl{}) {}
	Indiv::Indiv(Dataset^ pSet, Platform::String^ sigle) : m_pimpl(new IndivImpl{ pSet->Sigle, sigle }) {
		m_set = pSet;
	}
	Indiv::Indiv(IMap<Platform::String^, Object^>^ pMap) : m_pimpl(new IndivImpl{ pMap }) {}
	Platform::String^ Indiv::DatasetSigle::get() {
		return m_pimpl->get_DatasetSigle();
	}
	Platform::String^ Indiv::Id::get() {
		return m_pimpl->get_Id();
	}
	Platform::String^ Indiv::Rev::get() {
		return m_pimpl->get_Rev();
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
	Platform::String^ Indiv::Observations::get() {
		return m_pimpl->get_Observations();
	}
	void Indiv::Observations::set(Platform::String^ value) {
		m_pimpl->set_Observations(value);
	}
	Platform::String^ Indiv::Type::get() {
		return InfoStrings::TYPE_INDIV;
	}// Type
	bool Indiv::IsStoreable::get() {
		return m_pimpl->get_IsStoreable();
	}
	Platform::String^ Indiv::Sigle::get() {
		return m_pimpl->get_Sigle();
	}
	void Indiv::Sigle::set(Platform::String^ value) {
		m_pimpl->set_Sigle(value);
	}
	Platform::String^ Indiv::Name::get() {
		return m_pimpl->get_Name();
	}
	void Indiv::Name::set(Platform::String^ value) {
		m_pimpl->set_Name(value);
	}// name
	IMap<Platform::String^, Object^>^ Indiv::GetMap(void) {
		Map<Platform::String^, Object^>^ oMap = ref new Map<Platform::String^, Object^>();
		m_pimpl->GetMap(oMap);
		return oMap;
	}
	Platform::String^ Indiv::ToString(void) {
		return m_pimpl->ToString();
	}
	IVector<InfoValue^>^ Indiv::Values::get() {
		return m_vals;
	}
	void Indiv::Values::set(IVector<InfoValue^>^ value) {
		m_vals = value;
	}
	///////////////////////////////////////
	Variable::Variable() :m_pimpl(new VariableImpl{}) {}
	Variable::Variable(Dataset^ pSet, Platform::String^ sigle) : m_pimpl(new VariableImpl{ pSet->Sigle, sigle }) {
		m_set = pSet;
	}
	Variable::Variable(IMap<Platform::String^, Object^>^ pMap) : m_pimpl(new VariableImpl{ pMap }) {}
	Platform::String^ Variable::DatasetSigle::get() {
		return m_pimpl->get_DatasetSigle();
	}
	Platform::String^ Variable::Id::get() {
		return m_pimpl->get_Id();
	}
	Platform::String^ Variable::Rev::get() {
		return m_pimpl->get_Rev();
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
	Platform::String^ Variable::Observations::get() {
		return m_pimpl->get_Observations();
	}
	void Variable::Observations::set(Platform::String^ value) {
		m_pimpl->set_Observations(value);
	}
	Platform::String^ Variable::Type::get() {
		return InfoStrings::TYPE_VARIABLE;
	}// Type
	bool Variable::IsStoreable::get() {
		return m_pimpl->get_IsStoreable();
	}
	Platform::String^ Variable::Sigle::get() {
		return m_pimpl->get_Sigle();
	}
	void Variable::Sigle::set(Platform::String^ value) {
		m_pimpl->set_Sigle(value);
	}
	Platform::String^ Variable::Name::get() {
		return m_pimpl->get_Name();
	}
	void Variable::Name::set(Platform::String^ value) {
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
	IVector<Platform::String^>^ Variable::Modalites::get() {
		return m_pimpl->get_Modalites();
	}
	void Variable::Modalites::set(IVector<Platform::String^>^ value) {
		m_pimpl->set_Modalites(value);
	}
	IMap<Platform::String^, Object^>^ Variable::GetMap(void) {
		Map<Platform::String^, Object^>^ oMap = ref new Map<Platform::String^, Object^>();
		m_pimpl->GetMap(oMap);
		return oMap;
	}
	Platform::String^ Variable::ToString(void) {
		return m_pimpl->ToString();
	}
	IVector<InfoValue^>^ Variable::Values::get() {
		return m_vals;
	}
	void Variable::Values::set(IVector<InfoValue^>^ value) {
		m_vals = value;
	}
	////////////////////////////////////
	InfoValue::InfoValue() :m_status(InfoStatus::Unknown) {}
	InfoValue::InfoValue(IMap<Platform::String^, Object^>^ oMap) : m_status(InfoStatus::Unknown) {
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
		if (oMap->HasKey(InfoStrings::KEY_VALUE)) {
			Object^ oval = oMap->Lookup(InfoStrings::KEY_VALUE);
			if (oMap->HasKey(InfoStrings::KEY_DATATYPE) && (oval != nullptr)) {
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
							Platform::String^ sv = oval->ToString();
							m_value = ref new InfoDataValue(sv);
						}
						break;
						case InfoDataType::Logical:
						{
							bool b = (bool)oval;
							m_value = ref new InfoDataValue(b);
						}
						break;
						case InfoDataType::Real:
						{
							double b = (double)oval;
							m_value = ref new InfoDataValue(b);
						}
						break;
						case InfoDataType::Integer:
						{
							int b = (int)oval;
							m_value = ref new InfoDataValue(b);
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
	Platform::String^ InfoValue::Id::get() {
		return m_id;
	}
	Platform::String^ InfoValue::Rev::get() {
		return m_rev;
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
	Platform::String^ InfoValue::Observations::get() {
		return m_desc;
	}
	void InfoValue::Observations::set(Platform::String^ value) {
		m_desc = value;
	}
	Platform::String^ InfoValue::DatasetSigle::get() {
		return m_datasetsigle;
	}
	Platform::String^ InfoValue::IndivSigle::get() {
		return m_indivsigle;
	}
	Platform::String^ InfoValue::VariableSigle::get() {
		return m_variablesigle;
	}
	Platform::String^ InfoValue::Type::get() {
		return InfoStrings::TYPE_VALUE;
	}
	InfoDataValue^ InfoValue::Value::get() {
		return m_value;
	}
	Platform::String^ InfoValue::ToString(void) {
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
	IMap<Platform::String^, Object^>^ InfoValue::GetMap(void) {
		Map<Platform::String^, Object^>^ oMap = ref new Map<Platform::String^, Object^>();
		oMap->Insert(InfoStrings::KEY_TYPE, this->Type);
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
	/////////////////////////////////////
}// Namespace InfoDomain
