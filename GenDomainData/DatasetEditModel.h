#pragma once
//
#include "DomainDataset.h"
#include "DomainEtudiant.h"
#include "DomainIndiv.h"
#include "DomainValue.h"
#include "DomainVariable.h"
#include "DomainDBManager.h"
/////////////////////////////////
//
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
//
//
namespace GenDomainData
{
	[Windows::Foundation::Metadata::WebHostHidden]
	[Windows::UI::Xaml::Data::Bindable]
	public ref class DatasetEditModel sealed : INotifyPropertyChanged
	{
	private:
		DomainDBManager^ m_pman;
		IObservableVector<DomainDataset^>^ m_datasets;
		DomainDataset^ m_dataset;
		IObservableVector<DomainVariable^>^ m_variables;
		DomainVariable^ m_variable;
		IObservableVector<DomainIndiv^>^ m_indivs;
		DomainIndiv^ m_indiv;
		IObservableVector<DomainValue^>^ m_varvalues;
		IObservableVector<DomainValue^>^ m_indvalues;
		//
		void OnPropertyChanged(String^ propertyName);
	public:
		DatasetEditModel();
		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler^ PropertyChanged;
	public:
		property DomainDBManager^ Manager {
			DomainDBManager^ get();
		}
		property String^ BaseUrl {
			String^ get();
			void set(String^ value);
		}
		property String^ DatabaseName {
			String^ get();
			void set(String^ value);
		}
		property IObservableVector<DomainDataset^>^ Datasets {
			IObservableVector<DomainDataset^>^ get();
		}
		property DomainDataset^ CurrentDataset {
			DomainDataset^ get();
			void set(DomainDataset^ value);
		}
		property IObservableVector<DomainVariable^>^ Variables {
			IObservableVector<DomainVariable^>^ get();
		}
		property DomainVariable^ CurrentVariable {
			DomainVariable^ get();
			void set(DomainVariable^ value);
		}
		property IObservableVector<DomainIndiv^>^ Indivs {
			IObservableVector<DomainIndiv^>^ get();
		}
		property DomainIndiv^ CurrentIndiv {
			DomainIndiv^ get();
			void set(DomainIndiv^ value);
		}
		property IObservableVector<DomainValue^>^ VariableValues {
			IObservableVector<DomainValue^>^ get();
		}
		property IObservableVector<DomainValue^>^ IndivValues {
			IObservableVector<DomainValue^>^ get();
		}
		//
		void RefreshDatasets(void);
		void RefreshVariables(void);
		void RefreshIndivs(void);
		void RefreshVariableValues(void);
		void RefreshIndivValues(void);
	};
}// namespace GenDomainData
