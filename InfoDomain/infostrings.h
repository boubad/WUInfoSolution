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
		static property String^ KEY_DATASETID {
			String^ get();
		}
		static property String^ KEY_INDIVID {
			String^ get();
		}
		static property String^ KEY_VARIABLEID {
			String^ get();
		}
		static property String^ KEY_MIMETYPE {
			String^ get();
		}
		static property String^ KEY_DATA {
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
		static property String^ KEY_ANNEE {
			String^ get();
		}
		static property String^ TYPE_GENERAL {
			String^ get() {
				return "gen";
			}
		}
		static property String^ TYPE_ETUDIANT {
			String^ get() {
				return "etud";
			}
		}
		static property String^ KEY_FIRSTNAME {
			String^ get() {
				return "firstname";
			}
		}
		static property String^ KEY_LASTNAME {
			String^ get() {
				return "lastname";
			}
		}
		static property String^ KEY_BIRTYEAR {
			String^ get() {
				return "birthyear";
			}
		}
		static property String^ KEY_SEXE {
			String^ get() {
				return "sexe";
			}
		}
		static property String^ KEY_AVATAR {
			String^ get() {
				return "lastname";
			}
		}
		static property String^ KEY_DOSSIER {
			String^ get() {
				return "dossier";
			}
		}
		static property String^ KEY_APB {
			String^ get() {
				return "apb";
			}
		}
		static property String^ KEY_SERIEBAC {
			String^ get() {
				return "seriebac";
			}
		}
		static property String^ KEY_OPTIONBAC {
			String^ get() {
				return "optionbac";
			}
		}
		static property String^ KEY_MENTIONBAC {
			String^ get() {
				return "mentionbac";
			}
		}
		static property String^ KEY_DATASETS {
			String^ get() {
				return "sets";
			}
		}
		static property String^ KEY_SUP {
			String^ get() {
				return "etudsup";
			}
		}
		static property String^ KEY_REDOUBLANT {
			String^ get() {
				return "redoublant";
			}
		}
		static property String^ KEY_DEPARTEMENT {
			String^ get() {
				return "dep";
			}
		}
		static property String^ KEY_VILLE {
			String^ get() {
				return "ville";
			}
		}
		static property String^ KEY_LYCEE {
			String^ get() {
				return "etab";
			}
		}
		static property String^ KEY_EMAIL {
			String^ get() {
				return "email";
			}
		}
		static property String^ KEY_GROUPE {
			String^ get() {
				return "groupe";
			}
		}
		//
	};// class InfoStrings
////////////////////////////
}// namespace InfoDoamin
