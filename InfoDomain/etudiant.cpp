#include "pch.h"
#include "etudiant.h"
//////////////////////////////
using namespace InfoDomain;

Etudiant::Etudiant() :m_pimpl(new EtudiantImpl{})
{
}
Etudiant::Etudiant(IMap<Platform::String^, Object^>^ pMap):
	m_pimpl(new EtudiantImpl{pMap}) {

}
String^ Etudiant::Fullname::get() {
	String^ s1 = (this->Lastname != nullptr) ? this->Lastname : "";
	String^ s2 = (this->Firstname != nullptr) ? this->Firstname : "";
	String^ sRet = s1 + " " + s2;
	return sRet;
}
IMap<String^, Object^>^ Etudiant::GetMap(void) {
	Map<String^, Object^>^ oMap = ref new Map<String^, Object^>();
	m_pimpl->GetMap(oMap);
	return oMap;
}
String^ Etudiant::ToString(void) {
	return this->Fullname;
}