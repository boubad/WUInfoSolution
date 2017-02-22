#include "pch.h"
#include "VarDesc.h"

using namespace InfoProcs;
using namespace Platform;

VarDesc::VarDesc()
{
}
void InfoProcs::VarDesc::Add(IndivDesc ^ pInd, Object ^ val)
{
	if (pInd == nullptr) {
		throw ref new Platform::InvalidArgumentException();
	}
	String^ key = pInd->Id;
	if (key->IsEmpty()) {
		throw ref new Platform::InvalidArgumentException();
	}
	IMap<String^, Object^>^ pMap = this->Data;
	pMap->Insert(key, val);
}
Object ^ InfoProcs::VarDesc::GetValue(IndivDesc ^ pInd)
{
	if (pInd == nullptr) {
		throw ref new Platform::InvalidArgumentException();
	}
	String^ key = pInd->Id;
	if (key->IsEmpty()) {
		throw ref new Platform::InvalidArgumentException();
	}
	IMap<String^, Object^>^ pMap = this->Data;
	Object^ oRet = nullptr;
	if (pMap->HasKey(key)) {
		oRet = pMap->Lookup(key);
	}
	return oRet;
}
IVector<Object^>^ VarDesc::Values::get() {
	IVector<Object^>^ oRet = ref new Vector<Object^>{};
	IMap<String^, Object^>^ pMap = this->Data;
	auto it = pMap->First();
	while (it->HasCurrent) {
		auto p = it->Current;
		if (p->Value != nullptr) {
			oRet->Append(p->Value);
		}
		it->MoveNext();
	}// it
	return oRet;
}// Values
IndivDesc::IndivDesc() {

}
void InfoProcs::IndivDesc::Add(VarDesc ^ pVar, Object ^ val)
{
	if (pVar == nullptr) {
		throw ref new Platform::InvalidArgumentException();
	}
	String^ key = pVar->Id;
	if (key->IsEmpty()) {
		throw ref new Platform::InvalidArgumentException();
	}
	IMap<String^, Object^>^ pMap = this->Data;
	pMap->Insert(key, val);
}

Object ^ InfoProcs::IndivDesc::GetValue(VarDesc ^ pVar)
{
	if (pVar == nullptr) {
		throw ref new Platform::InvalidArgumentException();
	}
	String^ key = pVar->Id;
	if (key->IsEmpty()) {
		throw ref new Platform::InvalidArgumentException();
	}
	IMap<String^, Object^>^ pMap = this->Data;
	Object^ oRet = nullptr;
	if (pMap->HasKey(key)) {
		oRet = pMap->Lookup(key);
	}
	return oRet;
}

VarDesc ^ InfoProcs::DatasetDesc::FindVariable(String ^ sigle)
{
	VarDesc^ pRet = nullptr;
	if ((sigle != nullptr) && (!sigle->IsEmpty())) {
		auto it = this->Variables->First();
		while ((pRet == nullptr) && it->HasCurrent) {
			VarDesc^ pVar = it->Current;
			if (pVar->Id == sigle) {
				pRet = pVar;
				break;
			}
			it->MoveNext();
		}// it
	}
	return pRet;
}

IndivDesc ^ InfoProcs::DatasetDesc::FindIndiv(String ^ sigle)
{
	IndivDesc^ pRet = nullptr;
	if ((sigle != nullptr) && (!sigle->IsEmpty())) {
		auto it = this->Indivs->First();
		while ((pRet == nullptr) && it->HasCurrent) {
			IndivDesc^ pInd = it->Current;
			if (pInd->Id == sigle) {
				pRet = pInd;
				break;
			}
			it->MoveNext();
		}// it
	}
	return pRet;
}

void InfoProcs::DatasetDesc::check_varindex(void)
{
	this->VarIndexes->Clear();
	auto it = this->Variables->First();
	while (it->HasCurrent) {
		VarDesc^ p = it->Current;
		this->VarIndexes->Append(p->Index);
		it->MoveNext();
	}// it
}

void InfoProcs::DatasetDesc::check_indindex(void)
{
	this->IndivIndexes->Clear();
	auto it = this->Indivs->First();
	while (it->HasCurrent) {
		IndivDesc^ p = it->Current;
		this->IndivIndexes->Append(p->Index);
		it->MoveNext();
	}// it
}

InfoProcs::DatasetDesc::DatasetDesc()
{

}

void InfoProcs::DatasetDesc::Add(String ^ indId, String ^ varId, Object ^ val)
{
	if ((indId == nullptr) || (varId == nullptr)) {
		throw ref new Platform::InvalidArgumentException();
	}
	if (indId->IsEmpty() || varId->IsEmpty()) {
		throw ref new Platform::InvalidArgumentException();
	}
	VarDesc^ pVar = this->FindVariable(varId);
	if (pVar == nullptr) {
		pVar = ref new VarDesc{};
		pVar->Id = varId;
		this->Variables->Append(pVar);
		pVar->Index = static_cast<int>(this->Variables->Size - 1);
		this->check_varindex();
	}
	IndivDesc^ pInd = this->FindIndiv(indId);
	if (pInd == nullptr) {
		pInd = ref new IndivDesc{};
		pInd->Id = indId;
		this->Indivs->Append(pInd);
		pInd->Index = static_cast<int>(this->Indivs->Size - 1);
		this->check_indindex();
	}
	pVar->Add(pInd, val);
	pInd->Add(pVar, val);
}

Object^ InfoProcs::DatasetDesc::GetValue(IndivDesc ^ pInd, VarDesc ^ pVar)
{
	Object^ pRet = nullptr;
	if ((pInd == nullptr) || (pVar == nullptr)) {
		throw ref new Platform::InvalidArgumentException();
	}
	VarDesc^ var = this->FindVariable(pVar->Id);
	IndivDesc^ ind = this->FindIndiv(pInd->Id);
	if ((var == nullptr) || (ind == nullptr)) {
		return pRet;
	}
	pRet = var->GetValue(ind);
	return pRet;
}
VarDesc ^ InfoProcs::DatasetDesc::AddVariable(String ^ varid)
{
	if (varid == nullptr) {
		throw ref new Platform::InvalidArgumentException();
	}
	if (varid->IsEmpty()) {
		throw ref new Platform::InvalidArgumentException();
	}
	VarDesc^ pRet = nullptr;
	auto it = this->Variables->First();
	while ((pRet == nullptr) && it->HasCurrent) {
		VarDesc^ p = it->Current;
		if (p->Id == varid) {
			pRet = p;
			break;
		}
		it->MoveNext();
	}// it
	if (pRet == nullptr) {
		pRet = ref new VarDesc{};
		pRet->Id = varid;
		this->Variables->Append(pRet);
		pRet->Index = static_cast<int>(this->Variables->Size - 1);
		this->check_varindex();
	}// pRet
	return pRet;
}
IndivDesc ^ InfoProcs::DatasetDesc::AddIndiv(String ^ indid)
{
	if (indid == nullptr) {
		throw ref new Platform::InvalidArgumentException();
	}
	if (indid->IsEmpty()) {
		throw ref new Platform::InvalidArgumentException();
	}
	IndivDesc^ pRet = nullptr;
	auto it = this->Indivs->First();
	while ((pRet == nullptr) && it->HasCurrent) {
		IndivDesc^ p = it->Current;
		if (p->Id == indid) {
			pRet = p;
			break;
		}
		it->MoveNext();
	}// it
	if (pRet == nullptr) {
		pRet = ref new IndivDesc{};
		pRet->Id = indid;
		this->Indivs->Append(pRet);
		pRet->Index = static_cast<int>(this->Indivs->Size - 1);
		this->check_indindex();
	}// pRet
	return pRet;
}

VarDesc ^ InfoProcs::DatasetDesc::FindVariableAt(int pos)
{
	VarDesc^ pRet = nullptr;
	auto it = this->Variables->First();
	while (it->HasCurrent && (pRet == nullptr)) {
		VarDesc^ p = it->Current;
		if (p->Index == pos) {
			pRet = p;
			break;
		}
	}// it
	return pRet;
}

IndivDesc ^ InfoProcs::DatasetDesc::FindIndivAt(int pos)
{
	IndivDesc^ pRet = nullptr;
	auto it = this->Indivs->First();
	while (it->HasCurrent && (pRet == nullptr)) {
		IndivDesc^ p = it->Current;
		if (p->Index == pos) {
			pRet = p;
			break;
		}
	}// it
	return pRet;
}

