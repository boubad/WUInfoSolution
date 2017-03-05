using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
//
using GenDomainData;
using GestInfoApp.Common;

namespace GestInfoApp.Model
{
    public class StatEditModel : InfoObservable
    {
        private bool m_varvalueschanged;
        private bool m_indvalueschanged;
        private string m_error;
        private string m_url;
        private string m_database;
        private DomainDBManager m_pman;
        private ObservableCollection<DomainDataset> m_datasets;
        private DomainDataset m_dataset;
        private ObservableCollection<DomainVariable> m_variables;
        private DomainVariable m_variable;
        private ObservableCollection<DomainIndiv> m_indivs;
        private DomainIndiv m_indiv;
        private ObservableCollection<DomainValue> m_varvalues;
        private ObservableCollection<DomainValue> m_indvalues;
        private DomainValue m_varval;
        private DomainValue m_indval;
        //
        public StatEditModel() { }
        public StatEditModel(String url, string database) {
            BaseUrl = url;
            DatabaseName = database;
        }
        //
        public void NotifyChanged(String propName)
        {
            NotifyPropertyChanged(propName);
        }
        //
        public DomainValue CurrentVariableValue
        {
            get
            {
                if (m_varval == null)
                {
                    m_varval = new DomainValue()
                    {
                        DatasetSigle = CurrentDataset.Sigle,
                        VariableSigle = CurrentVariable.Sigle,
                        IndivSigle = CurrentIndiv.Sigle,
                        VariableType = CurrentVariable.VariableType,
                        Status = InfoStatus.Normal
                    };
                }
                return m_varval;
            }
            set
            {
                if (m_varval != value)
                {
                    m_varval = value;
                    NotifyPropertyChanged("CurrentVariableValue");
                }
            }
        }//CurrentVariableValue
        public DomainValue CurrentIndivValue
        {
            get
            {
                if (m_indval == null)
                {
                    m_indval = new DomainValue()
                    {
                        DatasetSigle = CurrentDataset.Sigle,
                        VariableSigle = CurrentVariable.Sigle,
                        IndivSigle = CurrentIndiv.Sigle,
                        VariableType = CurrentVariable.VariableType,
                        Status = InfoStatus.Normal
                    };
                }
                return m_indval;
            }
            set
            {
                if (m_indval != value)
                {
                    m_indval = value;
                    NotifyPropertyChanged("CurrentIndivValue");
                }
            }
        }//CurrentIndivValue
        //
        public bool IsVariableValuesModified
        {
            get
            {
                return m_varvalueschanged;
            }
            set
            {
                if (m_varvalueschanged != value)
                {
                    m_varvalueschanged = value;
                    NotifyPropertyChanged("IsVariableValuesModified");
                }
            }
        }//IsVariablesValuesModified
        public bool IsIndivValuesModified
        {
            get
            {
                return m_indvalueschanged;
            }
            set
            {
                if (m_indvalueschanged != value)
                {
                    m_indvalueschanged = value;
                    NotifyPropertyChanged("IsIndivValuesModified");
                }
            }
        }//IsIndivValuesModified
        public String ErrorString
        {
            get
            {
                return m_error;
            }
            set
            {
                if (m_error != value)
                {
                    m_error = value;
                    NotifyPropertyChanged("HasError");
                    NotifyPropertyChanged("ErrorString");
                }
            }
        }// ErrorString
        public bool HasError
        {
            get
            {
                return (!string.IsNullOrEmpty(ErrorString));
            }
            set { }
        }// HasError
        public DomainDBManager Manager
        {
            get
            {
                if (m_pman == null)
                {
                    m_pman = new DomainDBManager()
                    {
                        BaseUrl = BaseUrl,
                        DatabaseName = DatabaseName
                    };
                }
                return m_pman;
            }
            set
            {
                if (m_pman != value)
                {
                    m_pman = value;
                    NotifyPropertyChanged("Manager");
                }
            }
        }// Manager
        public string BaseUrl
        {
            get
            {
                if (string.IsNullOrEmpty(m_url))
                {
                    m_url = "http://localhost:5984/";
                }
                return m_url;
            }
            set
            {
                string old = BaseUrl;
                string cur = (string.IsNullOrEmpty(value)) ? string.Empty : value.Trim();
                if (old != cur)
                {
                    m_url = cur;
                    Manager = null;
                    NotifyPropertyChanged("BaseUrl");
                }
            }
        }// BaseUrl
        public string DatabaseName
        {
            get
            {
                if (string.IsNullOrEmpty(m_database))
                {
                    m_database = "test";
                }
                return m_database;
            }
            set
            {
                string old = DatabaseName;
                string cur = (string.IsNullOrEmpty(value)) ? string.Empty : value.Trim().ToLower();
                if (old != cur)
                {
                    m_database = cur;
                    Manager = null;
                    NotifyPropertyChanged("DatabaseName");
                }
            }
        }// DatabaseName
        //
        public ObservableCollection<DomainDataset> Datasets
        {
            get
            {
                if (m_datasets == null)
                {
                    m_datasets = new ObservableCollection<DomainDataset>();
                }
                return m_datasets;
            }
            set
            {
                if (m_datasets != value)
                {
                    m_datasets = value;
                    NotifyPropertyChanged("Datasets");
                    DomainDataset pCur = null;
                    if (CurrentDataset.IsPersisted || CurrentDataset.IsStoreable)
                    {
                        var col = Datasets;
                        foreach (var p in col)
                        {
                            if ((p.Sigle == CurrentIndiv.Sigle) || (p.Id == CurrentIndiv.Id))
                            {
                                pCur = p;
                                break;
                            }
                        }// p
                    }
                    CurrentDataset = pCur;
                }
            }
        }// Datasets
        public DomainDataset CurrentDataset
        {
            get
            {
                if (m_dataset == null)
                {
                    m_dataset = new DomainDataset()
                    {
                        Status = InfoStatus.Normal
                    };
                }
                return m_dataset;
            }
            set
            {
                if (m_dataset != value)
                {
                    m_dataset = value;
                    NotifyPropertyChanged("CurrentDataset");
                    Variables = null;
                    Indivs = null;
                }
            }
        }// CurrentDataset
        public ObservableCollection<DomainVariable> Variables
        {
            get
            {
                if (m_variables == null)
                {
                    m_variables = new ObservableCollection<DomainVariable>();
                }
                return m_variables;
            }
            set
            {
                if (m_variables != value)
                {
                    m_variables = value;
                    NotifyPropertyChanged("Variables");
                    DomainVariable pCur = null;
                    if (CurrentVariable.IsPersisted || CurrentVariable.IsStoreable)
                    {
                        var col = Variables;
                        foreach (var p in col)
                        {
                            if ((p.Sigle == CurrentIndiv.Sigle) || (p.Id == CurrentIndiv.Id))
                            {
                                pCur = p;
                                break;
                            }
                        }// p
                    }
                    CurrentVariable = pCur;
                }
            }
        }// Variables
        public ObservableCollection<DomainIndiv> Indivs
        {
            get
            {
                if (m_indivs == null)
                {
                    m_indivs = new ObservableCollection<DomainIndiv>();
                }
                return m_indivs;
            }
            set
            {
                if (m_indivs != value)
                {
                    m_indivs = value;
                    NotifyPropertyChanged("Indivs");
                    DomainIndiv pCur = null;
                    if (CurrentIndiv.IsPersisted || CurrentIndiv.IsStoreable)
                    {
                        var col = Indivs;
                        foreach (var p in col)
                        {
                            if ((p.Sigle == CurrentIndiv.Sigle) || (p.Id == CurrentIndiv.Id))
                            {
                                pCur = p;
                                break;
                            }
                        }// p
                    }
                    CurrentIndiv = pCur;
                }
            }
        }// Indivs
        public DomainIndiv CurrentIndiv
        {
            get
            {
                if (m_indiv == null)
                {
                    m_indiv = new DomainIndiv()
                    {
                        DatasetSigle = CurrentDataset.Sigle,
                        Status = InfoStatus.Normal
                    };
                }
                return m_indiv;
            }
            set
            {
                if (m_indiv != value)
                {
                    m_indiv = value;
                    NotifyPropertyChanged("CurrentIndiv");
                    IndivValues = null;
                }
            }
        }// CurrentIndiv
        public DomainVariable CurrentVariable
        {
            get
            {
                if (m_variable == null)
                {
                    m_variable = new DomainVariable()
                    {
                        DatasetSigle = CurrentDataset.Sigle,
                        VariableKind = InfoKind.Normal,
                        VariableType = InfoDataType.Real,
                        Status = InfoStatus.Normal
                    };
                }
                return m_variable;
            }
            set
            {
                if (m_variable != value)
                {
                    m_variable = value;
                    NotifyPropertyChanged("CurrentVariable");
                    VariableValues = null;
                }
            }
        }// CurrentVariable
        public bool CanIndivValues
        {
            get
            {
                return (IndivValues.Count > 0);
            }
            set { }
        }
        public ObservableCollection<DomainValue> IndivValues
        {
            get
            {
                if (m_indvalues == null)
                {
                    m_indvalues = new ObservableCollection<DomainValue>();
                }
                return m_indvalues;
            }
            set
            {
                if (m_indvalues != value)
                {
                    m_indvalues = value;
                    if (m_indvalues != null)
                    {
                        foreach (DomainValue p in m_indvalues)
                        {
                            p.PropertyChanged += P_PropertyChanged_IndValues;
                        }// p
                    }// val
                    NotifyPropertyChanged("IndivValues");
                    NotifyPropertyChanged("CanIndivValues");
                    CurrentIndivValue = null;
                    IsIndivValuesModified = false;
                }
            }
        }// IndivValues

        private void P_PropertyChanged_IndValues(object sender, PropertyChangedEventArgs e)
        {
            if ((e.PropertyName == "IsModified") && (sender is DomainValue))
            {
                DomainValue p = sender as DomainValue;
                if (p.IsModified)
                {
                    IsIndivValuesModified = true;
                }
            }
        }

        public bool CanVariableValues
        {
            get
            {
                return (VariableValues.Count > 0);
            }
            set { }
        }
        public ObservableCollection<DomainValue> VariableValues
        {
            get
            {
                if (m_varvalues == null)
                {
                    m_varvalues = new ObservableCollection<DomainValue>();
                }
                return m_varvalues;
            }
            set
            {
                if (m_varvalues != value)
                {
                    m_varvalues = value;
                   if (m_varvalues != null)
                    {
                        foreach (DomainValue p in m_varvalues)
                        {
                            p.PropertyChanged += P_PropertyChanged_VarValue;
                        }
                    }// values
                    NotifyPropertyChanged("VariableValues");
                    NotifyPropertyChanged("CanVariableValues");
                    CurrentVariableValue = null;
                    IsVariableValuesModified = false;
                }
            }
        }// VariableValues

        private void P_PropertyChanged_VarValue(object sender, PropertyChangedEventArgs e)
        {
            if ((e.PropertyName == "IsModified") && (sender is DomainValue))
            {
                DomainValue p = sender as DomainValue;
                if (p.IsModified)
                {
                    IsVariableValuesModified = true;
                }
            }
        }

        //
        public async void RefreshDatasets()
        {
            try
            {
                ErrorString = null;
                ObservableCollection<DomainDataset> pp = null;
                int n = await Manager.GetDatasetsCountAsync(null);
                if (n > 0)
                {
                    var oList = await Manager.GetDatasetsAsync(null, 0, n);
                    pp = new ObservableCollection<DomainDataset>(oList.OrderBy(x => x.Sigle));
                }
                Datasets = pp;
            } catch(Exception ex)
            {
                ErrorString = ex.Message;
            }
        }//RefreshDatasetsAsync
        public async void RefreshVariables()
        {
            try
            {
                ErrorString = null;
                ObservableCollection<DomainVariable> pp = null;
                if (CurrentDataset.IsPersisted)
                {
                    int n = await Manager.GetDatasetVariablesCountAsync(CurrentDataset);
                    if (n > 0)
                    {
                        var oList = await Manager.GetDatasetVariablesAsync(CurrentDataset, 0, n);
                        pp = new ObservableCollection<DomainVariable>(oList.OrderBy(x => x.Sigle));
                    }
                }
                Variables = pp;
            }catch(Exception ex)
            {
                ErrorString = ex.Message;
            }
        }//RefreshVariablesAsync
        public async void RefreshIndivs()
        {
            try
            {
                ErrorString = null;
                ObservableCollection<DomainIndiv> pp = null;
                if (CurrentDataset.IsPersisted)
                {
                    int n = await Manager.GetDatasetIndivsCountAsync(CurrentDataset);
                    if (n > 0)
                    {
                        var oList = await Manager.GetDatasetIndivsAsync(CurrentDataset, 0, n);
                        pp = new ObservableCollection<DomainIndiv>(oList.OrderBy(x => x.Sigle));
                    }
                }
                Indivs = pp;
            }catch(Exception ex)
            {
                ErrorString = ex.Message;
            }
        }//RefreshIndivsAsync
        public async void RefreshVariableValues()
        {
            try
            {
                ErrorString = null;
                ObservableCollection<DomainValue> pp = null;
                if (CurrentVariable.IsPersisted)
                {
                    int n = await Manager.GetVariableValuesCountAsync(CurrentVariable);
                    if (n > 0)
                    {
                        var oList = await Manager.GetVariableValuesAsync(CurrentVariable, 0, n);
                        pp = new ObservableCollection<DomainValue>();
                        foreach (var pInd in Indivs)
                        {
                            DomainValue p = null;
                            foreach (var x in oList)
                            {
                                if (x.IndivSigle == pInd.Sigle)
                                {
                                    p = x;
                                    break;
                                }
                            }// x
                            if (p == null)
                            {
                                p = new DomainValue()
                                {
                                    DatasetSigle = CurrentVariable.DatasetSigle,
                                    VariableSigle = CurrentVariable.Sigle,
                                    IndivSigle = pInd.Sigle,
                                    Status = InfoStatus.Normal
                                };
                            }// p
                            pp.Add(p);
                        }// pInd
                    }
                }
                VariableValues = pp;
            } catch(Exception ex)
            {
                ErrorString = ex.Message;
            }
        }//RefreshVariableValuesAsync
        public async void RefreshIndivValues()
        {
            try
            {
                ErrorString = null;
                ObservableCollection<DomainValue> pp = null;
                if (CurrentIndiv.IsPersisted )
                {
                    int n = await Manager.GetIndivValuesCountAsync(CurrentIndiv);
                    if (n > 0)
                    {
                        var oList = await Manager.GetIndivValuesAsync(CurrentIndiv, 0, n);
                        pp = new ObservableCollection<DomainValue>();
                        foreach (var pVar in Variables)
                        {
                            DomainValue p = null;
                            foreach (var x in oList)
                            {
                                if (x.VariableSigle == pVar.Sigle)
                                {
                                    p = x;
                                    break;
                                }
                            }// x
                            if (p == null)
                            {
                                p = new DomainValue()
                                {
                                    DatasetSigle = CurrentIndiv.DatasetSigle,
                                    VariableSigle = pVar.Sigle,
                                    IndivSigle = CurrentIndiv.Sigle,
                                    Status = InfoStatus.Normal
                                };
                            }// p
                            pp.Add(p);
                        }// pInd
                    }
                }
                IndivValues = pp;
            }catch(Exception ex)
            {
                ErrorString = ex.Message;
            }
        }//RefreshIndivValuesAsync
        public async void MaintainsDataset()
        {
            try
            {
                ErrorString = null;
                if (CurrentDataset.IsStoreable)
                {
                    bool b = await Manager.MaintainsDatasetAsync(CurrentDataset);
                    if (b)
                    {
                        RefreshDatasets();
                    }
                }
            } catch (Exception ex)
            {
                ErrorString = ex.Message;
            }
        }//MaintainsDatasetAsync
        public async void MaintainsVariable()
        {
            try
            {
                ErrorString = null;
                if (CurrentVariable.IsStoreable)
                {
                    bool b = await Manager.MaintainsVariableAsync(CurrentVariable);
                    if (b)
                    {
                        RefreshVariables();
                    }
                }
            }catch(Exception ex)
            {
                ErrorString = ex.Message;
            }
        }//MaintainsVariableAsync
        public async void MaintainsIndiv()
        {
            try
            {
                ErrorString = null;
                if (CurrentIndiv.IsStoreable)
                {
                    bool b = await Manager.MaintainsIndivAsync(CurrentIndiv);
                    if (b)
                    {
                        RefreshIndivs();
                    }
                }
            }catch(Exception ex)
            {
                ErrorString = ex.Message;
            }
        }//MaintainsIndivAsync
        public async void RemoveDataset()
        {
            try
            {
                ErrorString = null;
                if (CurrentDataset.IsPersisted)
                {
                    bool b = await Manager.RemoveDatasetAsync(CurrentDataset);
                    if (b)
                    {
                        RefreshDatasets();
                    }
                }
            }catch(Exception ex)
            {
                ErrorString = ex.Message;
            }
        }//RemoveDatasetAsync
        public async void RemoveVariable()
        {
            try
            {
                ErrorString = null;
                if (CurrentVariable.IsPersisted)
                {
                    bool b = await Manager.RemoveVariableAsync(CurrentVariable);
                    if (b)
                    {
                        RefreshVariables();
                    }
                }
            }catch(Exception ex)
            {
                ErrorString = ex.Message;
            }
        }//RemoveVariableAsync
        public async void RemoveIndiv()
        {
            try
            {
                ErrorString = null;
                if (CurrentIndiv.IsPersisted)
                {
                    bool b = await Manager.RemoveIndivAsync(CurrentIndiv);
                    if (b)
                    {
                        RefreshIndivs();
                    }
                }
            } catch(Exception ex)
            {
                ErrorString = ex.Message;
            }
        }//RemoveIndivAsync
        //
        public async void MaintainsVariableValues()
        {
            try
            {
                ErrorString = null;
                List<DomainValue> oDelList = new List<DomainValue>();
                List<DomainValue> oUpdateList = new List<DomainValue>();
                bool bDone = false;
                foreach (DomainValue p in VariableValues)
                {
                    if (p.IsModified)
                    {
                        if (p.IsPersisted && (p.Value == null))
                        {
                            oDelList.Add(p);
                        } else if (p.IsStoreable)
                        {
                            oUpdateList.Add(p);
                        }
                    }// modified
                }// p
                if (oDelList.Count > 0)
                {
                    bDone = await Manager.MaintainsValuesAsync(oDelList, true);
                }
                if (oUpdateList.Count > 0)
                {
                    bool bx = await Manager.MaintainsValuesAsync(oUpdateList, false);
                    bDone |= bx;
                }
                if (bDone)
                {
                    RefreshVariableValues();
                }
            }catch(Exception ex)
            {
                ErrorString = ex.Message;
            }
        }// MaintainsVariableValues
        public async void MaintainsIndivValues()
        {
            try
            {
                ErrorString = null;
                List<DomainValue> oDelList = new List<DomainValue>();
                List<DomainValue> oUpdateList = new List<DomainValue>();
                bool bDone = false;
                foreach (DomainValue p in IndivValues)
                {
                    if (p.IsModified)
                    {
                        if (p.IsPersisted && (p.Value == null))
                        {
                            oDelList.Add(p);
                        }
                        else if (p.IsStoreable)
                        {
                            oUpdateList.Add(p);
                        }
                    }// modified
                }// p
                if (oDelList.Count > 0)
                {
                    bDone = await Manager.MaintainsValuesAsync(oDelList, true);
                }
                if (oUpdateList.Count > 0)
                {
                    bool bx = await Manager.MaintainsValuesAsync(oUpdateList, false);
                    bDone |= bx;
                }
                if (bDone)
                {
                    RefreshIndivValues();
                }
            }
            catch (Exception ex)
            {
                ErrorString = ex.Message;
            }
        }// MaintainsVariableValues
    }// class StatEditModel
}// namespace GestInfoApp.Model
