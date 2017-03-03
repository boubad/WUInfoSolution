using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Runtime.CompilerServices;
//
using InfoDomain;
//
namespace InfoControls
{
    public class DatasetEditModel : INotifyPropertyChanged
    {
        private readonly Object _syncObj = new object();
        private readonly Object _syncBusyObj = new object();
        private ObservableCollection<Dataset> _datasets = null;
        private Dataset _currentDataset = null;
        private Dataset _oldset = null;
        private String _baseUrl = "http://localhost:5984/";
        private String _databaseName = "test";
        private DomainManager _m_pman = null;
        private ObservableCollection<Variable> _variables = null;
        private Variable _currentVariable = null;
        private Variable _oldVariable = null;
        private ObservableCollection<Indiv> _indivs = null;
        private Indiv _currentIndiv = null;
        private Indiv _oldIndiv = null;
        private bool _bnewdataset = false;
        private bool _bnewvariable = false;
        private bool _bnewindiv = false;
        private ObservableCollection<InfoValue> _varvalues = null;
        private ObservableCollection<InfoValue> _indvalues = null;
        private String _workurl = null;
        private String _workdatabase = null;
        private bool _busy = false;
        //
        public event PropertyChangedEventHandler PropertyChanged;
        protected void NotifyPropertyChanged([CallerMemberName] String propertyName = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
        //
        public DatasetEditModel()
        {

        }
        public DatasetEditModel(String databaseName)
        {
            _databaseName = databaseName;
        }
        public DatasetEditModel(String baseUrl, String databaseName)
        {
            _baseUrl = baseUrl;
            _databaseName = databaseName;
        }
        //
        public bool IsBusy
        {
            get
            {
                return _busy;
            }
            set
            {
                if (_busy != value)
                {
                    lock (_syncBusyObj)
                    {
                        _busy = value;
                    }
                    NotifyPropertyChanged("IsBusy");
                    NotifyPropertyChanged("IsNotBusy");
                }
            }
        }// IsBusy
        public bool IsNotBusy
        {
            get
            {
                return (!IsBusy);
            }
            set { }
        }// IsNotBusy
        public String BaseUrl
        {
            get
            {
                return (!string.IsNullOrEmpty(_baseUrl)) ? _baseUrl : "http://localhost:5984/";
            }
            set
            {
                _baseUrl = value;
                _m_pman = null;
                _datasets = null;
                _currentDataset = null;
                _oldset = null;
                _variables = null;
                _currentVariable = null;
                _oldVariable = null;
                _indivs = null;
                _currentIndiv = null;
                _oldIndiv = null;
                NotifyPropertyChanged("BaseUrl");
                NotifyPropertyChanged("Manager");
                NotifyPropertyChanged("Datasets");
            }
        }// BaseUrl 
        public String DatabaseName
        {
            get
            {
                return (!string.IsNullOrEmpty(_databaseName)) ? _databaseName : "test";
            }
            set
            {
                _databaseName = (string.IsNullOrEmpty(value)) ? "test" : value.Trim().ToLower();
                _m_pman = null;
                _currentDataset = null;
                _oldset = null;
                _variables = null;
                _currentVariable = null;
                _oldVariable = null;
                _indivs = null;
                _currentIndiv = null;
                _oldIndiv = null;
                NotifyPropertyChanged("DatabaseName");
                NotifyPropertyChanged("Manager");
                NotifyPropertyChanged("Datasets");
            }
        }// DatabaseName
        public String WorkBaseUrl
        {
            get
            {
                if (string.IsNullOrEmpty(_workurl))
                {
                    _workurl = BaseUrl;
                }
                return _workurl;
            }
            set
            {
                String s1 = string.IsNullOrEmpty(WorkBaseUrl) ? "" : WorkBaseUrl;
                String s2 = (string.IsNullOrEmpty(value)) ? "" : value.Trim();
                if (s1 != s2)
                {
                    _workurl = s2;
                    NotifyPropertyChanged("WorkBaseUrl");
                }
            }
        }
        public String WorkDatabaseName
        {
            get
            {
                if (string.IsNullOrEmpty(_workdatabase))
                {
                    _workdatabase = DatabaseName;
                }
                return _workdatabase;
            }
            set
            {
                String s1 = string.IsNullOrEmpty(WorkDatabaseName) ? "" : WorkDatabaseName;
                String s2 = (string.IsNullOrEmpty(value)) ? "" : value.Trim();
                if (s1 != s2)
                {
                    _workdatabase = s2;
                    NotifyPropertyChanged("WorkDatabaseName");
                }
            }
        }
        public DomainManager Manager
        {
            get
            {
                if (_m_pman == null)
                {
                    _m_pman = new DomainManager(BaseUrl, DatabaseName);
                }
                return _m_pman;
            }
            set {
                NotifyPropertyChanged("Manager");
            }
        }// Manager
        //
        public void SelectDataset(Dataset pSet)
        {
            _bnewdataset = false;
            OldDataset = pSet;
            if (pSet != null)
            {
                CurrentDataset = pSet.Clone();
            }
            else
            {
                CurrentDataset = new Dataset()
                {
                    Status = InfoStatus.Normal,
                    IsModified = false
                };
            }
            RefreshVariables();
            RefreshIndivs();
            NotifyPropertyChanged("IsDatasetCreatable");
            NotifyPropertyChanged("IsDatasetCancellable");
            NotifyPropertyChanged("IsDatasetRemoveable");
            NotifyPropertyChanged("IsDatasetStoreable");
        }// SelectDataset
        public async void RefreshDatasets()
        {
            if (!IsBusy)
            {
                try
                {
                    IsBusy = true;
                    ObservableCollection<Dataset> col = null;
                    DomainManager pMan = Manager;
                    if (pMan != null)
                    {
                        Dataset model = new Dataset();
                        int n = await pMan.GetDatasetsCountAsync(model);
                        if (n > 0)
                        {
                            var oList = await pMan.GetDatasetsAsync(model, 0, n);
                            if (oList != null)
                            {
                                if (oList.Count() > 1)
                                {
                                    col = new ObservableCollection<Dataset>(oList.OrderBy(x => x.Sigle));
                                }
                                else
                                {
                                    col = new ObservableCollection<Dataset>(oList);
                                }
                            }// oList
                        }// n
                    }// pMan
                    if (col == null)
                    {
                        col = new ObservableCollection<Dataset>();
                    }
                    IsBusy = false;
                    Datasets = col;
                    if (Datasets.Count() > 0)
                    {
                        SelectDataset(Datasets.First());
                    }
                    else
                    {
                        SelectDataset(null);
                    }
                } catch(Exception /* ex */)
                {

                }
                IsBusy = false;
            }// busy
           
        }
        public async void PerformDatasetSave()
        {
            if (!IsBusy)
            {
                try
                {
                    IsBusy = true;
                    DomainManager pMan = Manager;
                    if ((pMan != null) && (CurrentDataset != null) && CurrentDataset.IsStoreable)
                    {
                        bool b = await pMan.MaintainsDatasetAsync(CurrentDataset);
                        IsBusy = false;
                        RefreshDatasets();
                    }
                }
                catch(Exception /* ex */) { }
                IsBusy = false;
            }
            
        }// PerformSave
        public async void PerformDatasetRemove()
        {
            if (!IsBusy)
            {
                try
                {
                    IsBusy = true;
                    DomainManager pMan = Manager;
                    if ((pMan != null) && (OldDataset != null) && OldDataset.IsPersisted)
                    {
                        bool b = await pMan.RemoveDatasetAsync(OldDataset);
                        IsBusy = false;
                        RefreshDatasets();
                    }
                } catch(Exception /* ex */) { }
                IsBusy = false;
            }// busy
            
        }// Performremove
        public void PerformDatasetNew()
        {
            SelectDataset(null);
            _bnewdataset = true;
            NotifyPropertyChanged("IsDatasetCreatable");
            NotifyPropertyChanged("IsDatasetCancellable");
            NotifyPropertyChanged("IsDatasetRemoveable");
        }
        public void PerformDatasetCancel()
        {
            SelectDataset(OldDataset);
            _bnewdataset = false;
            NotifyPropertyChanged("IsDatasetCreatable");
            NotifyPropertyChanged("IsDatasetCancellable");
            NotifyPropertyChanged("IsDatasetRemoveable");
        }
        public bool IsDatasetStoreable
        {
            get
            {
                return (Manager != null) && (CurrentDataset != null) && CurrentDataset.IsStoreable && (!IsBusy);
            }
            set {
                NotifyPropertyChanged("IsDatasetStoreable");
            }
        }
        public bool IsDatasetRemoveable
        {
            get
            {
                return (Manager != null) && (OldDataset != null) && OldDataset.IsPersisted && (!_bnewdataset) && (!IsBusy);
            }
            set {
                NotifyPropertyChanged("IsDatasetRemoveable");
            }
        }
        public bool IsDatasetCancellable
        {
            get
            {
                return (OldDataset != null) && OldDataset.IsPersisted && _bnewdataset && (!IsBusy);
            }
            set
            {
                NotifyPropertyChanged("IsDatasetCancellable");
            }
        }
        public bool IsDatasetCreatable
        {
            get
            {
                return (!_bnewdataset) && (!IsBusy);
            }
            set
            {
                NotifyPropertyChanged("IsDatasetCreatable");
            }
        }
        public ObservableCollection<Dataset> Datasets
        {
           get
            {
                return _datasets ?? new ObservableCollection<Dataset>();
            }
            set
            {
                _datasets = value;
                NotifyPropertyChanged("Datasets");
            }
        }
        public Dataset CurrentDataset
        {
            get
            {
                if (_currentDataset == null)
                {
                    _currentDataset = new Dataset()
                    {
                        Status = InfoStatus.Normal
                    };
                }
                return _currentDataset;
            }
            set
            {
                _currentDataset = value;
                NotifyPropertyChanged("CurrentDataset");
                NotifyPropertyChanged("IsDatasetStoreable");
                NotifyPropertyChanged("IsDatasetCreatable");
                NotifyPropertyChanged("IsDatasetCancellable");
                NotifyPropertyChanged("IsDatasetRemoveable");
            }
        }
        public Dataset OldDataset
        {
            get
            {
                return _oldset;
            }
            set
            {
                _oldset = value;
                NotifyPropertyChanged("OldDataset");
                NotifyPropertyChanged("IsDatasetRemoveable");
                NotifyPropertyChanged("IsDatasetCancelleble");
            }
        }
        //
        public async void PerformVariableSave()
        {
            if (!IsBusy)
            {
                try
                {
                    IsBusy = true;
                    DomainManager pMan = Manager;
                    if ((pMan != null) && (CurrentVariable != null) && CurrentVariable.IsStoreable)
                    {
                        bool b = await pMan.MaintainsVariableAsync(CurrentVariable);
                        IsBusy = false;
                        RefreshVariables();
                    }
                }
                catch(Exception /* ex */) { }
                IsBusy = false;
            }// busy
           
        }// PerformVariableSave
        public  async void PerformVariableRemove()
        {
            if (!IsBusy)
            {
                try
                {
                    IsBusy = true;
                    DomainManager pMan = Manager;
                    if ((pMan != null) && (OldVariable != null) && OldVariable.IsPersisted)
                    {
                        bool b = await pMan.RemoveVariableAsync(OldVariable);
                        IsBusy = false;
                        RefreshVariables();
                    }
                }
                catch(Exception /* ex */) { }
                IsBusy = false;
            }// busy
        }// Performremove
        public  async void RefreshVariableValues()
        {
            if (!IsBusy)
            {
                try
                {
                    IsBusy = true;
                    List<InfoValue> bList = new List<InfoValue>();
                    DomainManager pMan = Manager;
                    if ((pMan != null) && (OldVariable != null) && OldVariable.IsPersisted)
                    {
                        IList<InfoValue> oList = new List<InfoValue>();
                        int n = await pMan.GetVariableValuesCountAsync(OldVariable);
                        if (n > 0)
                        {
                            oList = await pMan.GetVariableValuesAsync(OldVariable, 0, n);
                            if (oList == null)
                            {
                                oList = new List<InfoValue>();
                            }
                        }// n
                        var inds = this.Indivs;
                        foreach (var v in inds)
                        {
                            String sigle = v.Sigle;
                            var col = oList.Where(x => x.IndivSigle == sigle);
                            if (col.Count() > 0)
                            {
                                bList.Add(col.First());
                            }
                            else
                            {
                                bList.Add(new InfoValue(v, OldVariable));
                            }
                        }
                    }// pMan
                    VariableValues = new ObservableCollection<InfoValue>(bList.OrderBy(x => x.IndivSigle));
                } catch(Exception /* ex */) { }
                IsBusy = false;
            }// isBusy
            
        }
        public void SelectVariable(Variable pVar)
        {
            _bnewvariable = false;
            OldVariable = pVar;
            if (pVar != null)
            {
                CurrentVariable = pVar.Clone();
            }
            else
            {
                CurrentVariable = new Variable(CurrentDataset, "")
                {
                    Status = InfoStatus.Normal,
                    VariableKind = InfoKind.Normal,
                    VariableType = InfoDataType.Real,
                    IsModified = false
                };
            }
            RefreshVariableValues();
            NotifyPropertyChanged("IsVariableCreatable");
            NotifyPropertyChanged("IsVariableCancellable");
            NotifyPropertyChanged("IsVariableRemoveable");
        }// SelectVariable
        public async void RefreshVariables()
        {
            if (!IsBusy)
            {
                try
                {
                    IsBusy = true;
                    ObservableCollection<Variable> col = null;
                    DomainManager pMan = Manager;
                    Dataset pSet = OldDataset;
                    if ((pMan != null) && (pSet != null) && pSet.IsPersisted)
                    {
                        int n = await pMan.GetDatasetVariablesCountAsync(pSet);
                        if (n > 0)
                        {
                            var oList = await pMan.GetDatasetVariablesAsync(pSet, 0, n);
                            if (oList != null)
                            {
                                if (oList.Count() > 1)
                                {
                                    col = new ObservableCollection<Variable>(oList.OrderBy(x => x.Sigle));
                                }
                                else
                                {
                                    col = new ObservableCollection<Variable>(oList);
                                }
                            }// oList
                        }// n
                    }// pMan
                    if (col == null)
                    {
                        col = new ObservableCollection<Variable>();
                    }
                    Variables = col;
                    IsBusy = false;
                    if (Variables.Count() > 0)
                    {
                        SelectVariable(Variables.First());
                    }
                    else
                    {
                        SelectVariable(null);
                    }
                } catch(Exception /* ex */) { }
                IsBusy = false;
            }// isBusy
            
        }
        public void PerformVariableNew()
        {
            SelectVariable(null);
            _bnewvariable = true;
            NotifyPropertyChanged("IsVariableCreatable");
            NotifyPropertyChanged("IsVariableCancellable");
            NotifyPropertyChanged("IsVariableRemoveable");
        }
        public void PerformVariableCancel()
        {
            SelectVariable(OldVariable);
            _bnewvariable = false;
            NotifyPropertyChanged("IsVariableCreatable");
            NotifyPropertyChanged("IsVariableCancellable");
            NotifyPropertyChanged("IsVariableRemoveable");
        }
        public ObservableCollection<Variable> Variables
        {
            get
            {
                return _variables ?? new ObservableCollection<Variable>();
            }
            set {
                _variables = value;
                NotifyPropertyChanged("Variables");
            }
        }// variables
        public Variable CurrentVariable
        {
            get
            {
                if (_currentVariable == null)
                {
                    _currentVariable = new Variable(CurrentDataset, "")
                    {
                        Status = InfoStatus.Normal,
                        VariableKind = InfoKind.Normal,
                        VariableType = InfoDataType.Real
                    };
                }
                return _currentVariable;
            }
            set
            {
                _currentVariable = value;
                NotifyPropertyChanged("CurrentVariable");
                NotifyPropertyChanged("IsVariableStoreable");
            }
        }// Currentvariable
        public Variable OldVariable
        {
            get
            {
                return _oldVariable;
            }
            set {
                _oldVariable = value;
                NotifyPropertyChanged("OldVariable");
                NotifyPropertyChanged("IsVariableRemoveable");
                NotifyPropertyChanged("IsVariableCancellable");
            }
        }// OldDataset
        public ObservableCollection<InfoValue> VariableValues
        {
            get
            {
                if (_varvalues == null)
                {
                    _varvalues = new ObservableCollection<InfoValue>();
                }
                return _varvalues;
            }
            set
            {
                _varvalues = value;
                NotifyPropertyChanged("VariableValues");
            }
        }// VariableValues
        public bool IsVariableCreatable
        {
            get
            {
                return (!_bnewvariable) && (!IsBusy);
            }
            set
            {
                NotifyPropertyChanged("IsVariableCreatable");
            }
        }
        public bool IsVariableStoreable
        {
            get
            {
                return (CurrentVariable != null) && CurrentVariable.IsStoreable && (!IsBusy);
            }
            set {
                NotifyPropertyChanged("IsVariableStoreable");
            }
        }
        public bool IsVariableRemoveable
        {
            get
            {
                return (OldVariable != null) && OldVariable.IsPersisted && (!_bnewvariable) && (!IsBusy);
            }
            set {
                NotifyPropertyChanged("IsVariableRemoveable");
            }
        }
        public bool IsVariableCancellable
        {
            get
            {
                return (OldVariable != null) && OldVariable.IsPersisted && _bnewvariable && (!IsBusy);
            }
            set
            {
                NotifyPropertyChanged("IsVariableCancellable");
            }
        }
        //
        public  async void RefreshIndivs()
        {
            if (!IsBusy)
            {
                try
                {
                    IsBusy = true;
                    ObservableCollection<Indiv> col = null;
                    DomainManager pMan = Manager;
                    Dataset pSet = OldDataset;
                    if ((pMan != null) && (pSet != null) && pSet.IsPersisted)
                    {
                        int n = await pMan.GetDatasetIndivsCountAsync(pSet);
                        if (n > 0)
                        {
                            var oList = await pMan.GetDatasetIndivsAsync(pSet, 0, n);
                            if (oList != null)
                            {
                                if (oList.Count() > 1)
                                {
                                    col = new ObservableCollection<Indiv>(oList.OrderBy(x => x.Sigle));
                                }
                                else
                                {
                                    col = new ObservableCollection<Indiv>(oList);
                                }
                            }// oList
                        }// n
                    }// pMan
                    if (col == null)
                    {
                        col = new ObservableCollection<Indiv>();
                    }
                    Indivs = col;
                    IsBusy = false;
                    if (Indivs.Count() > 0)
                    {
                        SelectIndiv(Indivs.First());
                    }
                    else
                    {
                        SelectIndiv(null);
                    }
                } catch(Exception /* ex */) { }
                IsBusy = false;
            }// busy
            
        }// RefreshIndivs
        public void SelectIndiv(Indiv pInd)
        {
            _bnewindiv = false;
            OldIndiv = pInd;
            if (pInd != null)
            {
                CurrentIndiv = pInd.Clone();
            }
            else
            {
                CurrentIndiv = new Indiv(CurrentDataset,"")
                {
                    Status = InfoStatus.Normal,
                    IsModified = false
                };
            }
            RefreshIndivValues();
            NotifyPropertyChanged("IsIndivCreatable");
            NotifyPropertyChanged("IsIndivCancellable");
            NotifyPropertyChanged("IsIndivRemoveable");
        }// SelectIndiv
        public async void PerformIndivSave()
        {
            if (!IsBusy)
            {
                try
                {
                    IsBusy = true;
                    DomainManager pMan = Manager;
                    if ((pMan != null) && (CurrentIndiv != null) && CurrentIndiv.IsStoreable)
                    {
                        bool b = await pMan.MaintainsIndivAsync(CurrentIndiv);
                        IsBusy = false;
                        RefreshIndivs();
                    }
                }
                catch(Exception /* ex */) { }
                IsBusy = false;
            }// busy
        }// PerformIndivSave
        public async void PerformIndivRemove()
        {
            if (!IsBusy)
            {
                try
                {
                    IsBusy = true;
                    DomainManager pMan = Manager;
                    if ((pMan != null) && (OldIndiv != null) && OldIndiv.IsPersisted)
                    {
                        bool b = await pMan.RemoveIndivAsync(OldIndiv);
                        IsBusy = false;
                        RefreshIndivs();
                    }
                } catch(Exception /* ex */) { }
                IsBusy = false;
            }// isBusy
            
        }// Performremoveindiv
        public void PerformIndivNew()
        {
            SelectIndiv(null);
            _bnewindiv = true;
            NotifyPropertyChanged("IsIndivCreatable");
            NotifyPropertyChanged("IsIndivCancellable");
            NotifyPropertyChanged("IsIndivRemoveable");
        }
        public void PerformIndivCancel()
        {
            SelectIndiv(OldIndiv);
            _bnewvariable = false;
            NotifyPropertyChanged("IsIndivCreatable");
            NotifyPropertyChanged("IsIndivCancellable");
            NotifyPropertyChanged("IsIndivRemoveable");
        }
        public  async void RefreshIndivValues()
        {
            if (!IsBusy)
            {
                try
                {
                    IsBusy = true;
                    List<InfoValue> bList = new List<InfoValue>();
                    DomainManager pMan = Manager;
                    if ((pMan != null) && (OldIndiv != null) && OldIndiv.IsPersisted)
                    {
                        IList<InfoValue> oList = new List<InfoValue>();
                        int n = await pMan.GetIndivValuesCountAsync(OldIndiv);
                        if (n > 0)
                        {
                            oList = await pMan.GetIndivValuesAsync(OldIndiv, 0, n);
                            if (oList == null)
                            {
                                oList = new List<InfoValue>();
                            }
                        }// n
                        var vars = this.Variables;
                        foreach (var v in vars)
                        {
                            String sigle = v.Sigle;
                            var col = oList.Where(x => x.VariableSigle == sigle);
                            if (col.Count() > 0)
                            {
                                bList.Add(col.First());
                            }
                            else
                            {
                                bList.Add(new InfoValue(OldIndiv, v));
                            }
                        }
                    }// pMan
                    IndivValues = new ObservableCollection<InfoValue>(bList.OrderBy(x => x.IndivSigle));
                }
                catch(Exception /* ex */) { }
                IsBusy = false;
            }// busy
        }
        public ObservableCollection<Indiv> Indivs
        {
            get
            {
                if (_indivs == null)
                {
                    _indivs = new ObservableCollection<Indiv>();
                }
                return _indivs;
            }
            set {
                _indivs = value;
                NotifyPropertyChanged("Indivs");
            }
        }// indivs
        public Indiv CurrentIndiv
        {
            get
            {
                if (_currentIndiv == null)
                {
                    _currentIndiv = new Indiv(CurrentDataset, "")
                    {
                        Status = InfoStatus.Normal
                    };
                }
                return _currentIndiv;
            }
            set
            {
                _currentIndiv = value;
                NotifyPropertyChanged("CurrentIndiv");
                NotifyPropertyChanged("IsIndivStoreable");
            }
        }// CurrentIndiv
        public Indiv OldIndiv
        {
            get
            {
                return _oldIndiv;
            }
            set {
                _oldIndiv = value;
                NotifyPropertyChanged("OldIndiv");
                NotifyPropertyChanged("IsIndivRemoveable");
                NotifyPropertyChanged("IsIndivCancellable");
            }
        }// OldIndiv
        public ObservableCollection<InfoValue> IndivValues
        {
            get
            {
                return _indvalues ?? new ObservableCollection<InfoValue>();
            }
            set
            {
                _indvalues = value;
                NotifyPropertyChanged("IndivValues");
            }
        }// VariableValues
        public bool IsIndivStoreable
        {
            get
            {
                return (CurrentIndiv != null) && CurrentIndiv.IsStoreable && (!IsBusy);
            }
            set {
                NotifyPropertyChanged("IsIndivStoreable");
            }
        }
        public bool IsIndivRemoveable
        {
            get
            {
                return (OldIndiv != null) && OldIndiv.IsPersisted && (!_bnewindiv) && (!IsBusy);
            }
            set {
                NotifyPropertyChanged("IsIndivRemoveable");
            }
        }
        public bool IsIndivCreatable
        {
            get
            {
                return (!_bnewindiv);
            }
            set
            {
                NotifyPropertyChanged("IsIndivCreatable");
            }
        }
        public bool IsIndivCancellable
        {
            get
            {
                return (OldIndiv != null) && OldIndiv.IsPersisted && _bnewindiv;
            }
            set
            {
                NotifyPropertyChanged("IsIndivCancellable");
            }
        }
    }// class DatasetEditModel
}// namespace InfoControls
