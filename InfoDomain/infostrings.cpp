#include "pch.h"
#include "infostrings.h"
///////////////////////////////
namespace InfoDomain {
	String^ InfoStrings::KEY_ID::get() {
		return "_id";
	}
	String^ InfoStrings::KEY_REV::get() {
		return "_rev";
	}
	String^ InfoStrings::KEY_TYPE::get() {
		return  "type";
	}
	String^ InfoStrings::KEY_DESC::get() {
		return "desc";
	}
	String^ InfoStrings::KEY_STATUS::get() {
		return "status";
	}
	String^ InfoStrings::KEY_SIGLE::get() {
		return "sigle";
	}
	String^ InfoStrings::KEY_NAME::get() {
		return "name";
	}
	String^ InfoStrings::KEY_DATASETSIGLE::get() {
		return "setsigle";
	}
	String^ InfoStrings::KEY_VARIABLESIGLE::get() {
		return "varsigle";
	}
	String^ InfoStrings::KEY_INDIVSIGLE::get() {
		return "indsigle";
	}
	String^ InfoStrings::KEY_DATATYPE::get() {
		return "dtype";
	}
	String^ InfoStrings::KEY_VARIABLEKIND::get() {
		return "kind";
	}
	String^ InfoStrings::KEY_MODALITES::get() {
		return "modalites";
	}
	String^ InfoStrings::KEY_VALUE::get() {
		return "val";
	}
	String^ InfoStrings::TYPE_DATASET::get() {
		return "set";
	}
	String^ InfoStrings::TYPE_VARIABLE::get() {
		return "var";
	}
	String^ InfoStrings::TYPE_INDIV::get() {
		return "ind";
	}
	String^ InfoStrings::TYPE_VALUE::get() {
		return  "val";
	}
}// namespace InfoDomain
