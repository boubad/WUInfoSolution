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
    class DatasetEditModel : INotifyPropertyChanged
    {
        //
        private readonly Object _syncObj = new object();
        private ObservableCollection<Dataset> _datasets = null;
        private Dataset _currentDataset = null;
        private Dataset _oldset = null;
        private ObservableCollection<String> _status = null;
        private String _baseUrl = "http://localhost:5984/";
        private String _databaseName = "test";
        private DomainManager _m_pman = null;
        private ObservableCollection<String> _datatypes = null;
        private ObservableCollection<String> _datakind = null;
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
        private async Task<DomainManager> GetDomainManagerAsync()
        {
            DomainManager pRet = null;
            try
            {
                await DomainManager.CheckDatabaseAsync(BaseUrl,DatabaseName);
                pRet = new DomainManager(BaseUrl, DatabaseName);
            }catch(Exception /* e */) { }
            return pRet;
        }//GetDomainManagerAsync
        //
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
        public DomainManager Manager
        {
            get
            {
                if (_m_pman == null)
                {
                    _m_pman = GetDomainManagerAsync().Result;
                }
                return _m_pman;
            }
            set {
                NotifyPropertyChanged("Manager");
            }
        }// Manager
        public ObservableCollection<String> StatusStrings
        {
            get
            {
                if (_status == null)
                {
                    lock (_syncObj)
                    {
                        _status = new ObservableCollection<string>
                        {
                            "Normal",
                            "Inactif",
                            "Désactivé",
                            "Inconnu"
                        };
                    }// lock
                }
                return _status;
            }
            set {
                NotifyPropertyChanged("StatusStrings");
            }
        }
        //
        public async Task SelectDatasetAsync(Dataset pSet)
        {
            _bnewdataset = false;
            OldDataset = pSet;
            if (pSet != null)
            {
                CurrentDataset = new Dataset(pSet.Sigle)
                {
                    Name = pSet.Name,
                    Annee = pSet.Annee,
                    Observations = pSet.Observations,
                    Status = pSet.Status,
                    IsModified = true
                };
            }
            else
            {
                CurrentDataset = new Dataset()
                {
                    Status = InfoStatus.Normal,
                    IsModified = true
                };
            }
            await RefreshVariablesAsync();
            await RefreshIndivsAsync();
            NotifyPropertyChanged("IsDatasetCreatable");
            NotifyPropertyChanged("IsDatasetCancellable");
            NotifyPropertyChanged("IsDatasetRemoveable");
        }// SelectDataset
        public async Task RefreshDatasetsAsync()
        {
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
            Datasets = col;
            if (Datasets.Count() > 0)
            {
                await SelectDatasetAsync(Datasets.First());
            }
            else
            {
                await SelectDatasetAsync(null);
            }
        }
        public async Task PerformDatasetSaveAsync()
        {
            DomainManager pMan = Manager;
            if ((pMan != null) && (CurrentDataset != null) && CurrentDataset.IsStoreable)
            {
                await pMan.MaintainsDatasetAsync(CurrentDataset);
                await RefreshDatasetsAsync();
            }
        }// PerformSave
        public async Task PerformDatasetRemoveAsync()
        {
            DomainManager pMan = Manager;
            if ((pMan != null) && (OldDataset != null) && OldDataset.IsPersisted)
            {
                await pMan.RemoveDatasetAsync(OldDataset);
                await RefreshDatasetsAsync();
            }
        }// Performremove
        public async Task PerformDatasetNewAsync()
        {
            await SelectDatasetAsync(null);
            _bnewdataset = true;
            NotifyPropertyChanged("IsDatasetCreatable");
            NotifyPropertyChanged("IsDatasetCancellable");
            NotifyPropertyChanged("IsDatasetRemoveable");
        }
        public async Task PerformDatasetCancelAsync()
        {
            await SelectDatasetAsync(OldDataset);
            _bnewdataset = false;
            NotifyPropertyChanged("IsDatasetCreatable");
            NotifyPropertyChanged("IsDatasetCancellable");
            NotifyPropertyChanged("IsDatasetRemoveable");
        }
        public bool IsDatasetStoreable
        {
            get
            {
                return (Manager != null) && (CurrentDataset != null) && CurrentDataset.IsStoreable && CurrentDataset.IsModified;
            }
            set {
                NotifyPropertyChanged("IsDatasetStoreable");
            }
        }
        public bool IsDatasetRemoveable
        {
            get
            {
                return (Manager != null) && (OldDataset != null) && OldDataset.IsPersisted && (!_bnewdataset);
            }
            set {
                NotifyPropertyChanged("IsDatasetRemoveable");
            }
        }
        public bool IsDatasetCancellable
        {
            get
            {
                return (OldDataset != null) && OldDataset.IsPersisted && _bnewdataset;
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
                return (!_bnewdataset);
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
                if (_datasets == null)
                {
                    RefreshDatasetsAsync().Wait();
                }
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
                NotifyPropertyChanged("DatasetDetailTitle");
                NotifyPropertyChanged("DatasetAnnee");
                NotifyPropertyChanged("DatasetSigle");
                NotifyPropertyChanged("DatasetName");
                NotifyPropertyChanged("DatasetObservations");
                NotifyPropertyChanged("DatasetStatus");
                NotifyPropertyChanged("IsDatasetStoreable");
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
        public String DatasetDetailTitle
        {
            get
            {

                return CurrentDataset.Name;
            }
            set {
                NotifyPropertyChanged("DatasetDetailTitle");
            }
        }
        public String DatasetAnnee
        {
            get
            {
                return CurrentDataset.Annee;
            }
            set
            {
                String old = DatasetAnnee;
                CurrentDataset.Annee = value;
                String cur = DatasetAnnee;
                if (cur != old)
                {
                    CurrentDataset.IsModified = true;
                    NotifyPropertyChanged("DatasetAnnee");
                    NotifyPropertyChanged("IsDatasetStoreable");
                }
            }
        }
        public String DatasetSigle
        {
            get
            {
                return CurrentDataset.Sigle;
            }
            set
            {
                string old = DatasetSigle;
                CurrentDataset.Sigle = value;
                string cur = DatasetSigle;
                if (cur != old)
                {
                    CurrentDataset.IsModified = true;
                    NotifyPropertyChanged("DatasetSigle");
                    NotifyPropertyChanged("IsDatasetStoreable");
                }
            }
        }
        public String DatasetName
        {
            get
            {
                return CurrentDataset.Name;
            }
            set
            {
                string old = DatasetName;
                CurrentDataset.Name = value;
                string cur = DatasetName;
                if (cur != old)
                {
                    CurrentDataset.IsModified = true;
                    NotifyPropertyChanged("DatasetName");
                    NotifyPropertyChanged("IsDatasetStoreable");
                }
            }
        }
        public String DatasetObservations
        {
            get
            {
                return CurrentDataset.Observations;
            }
            set
            {
                string old = DatasetObservations;
                CurrentDataset.Observations = value;
                string cur = DatasetObservations;
                if (cur != old)
                {
                    NotifyPropertyChanged("DatasetObservations");
                    NotifyPropertyChanged("IsDatasetStoreable");
                }
            }
        }
        public String DatasetStatus
        {
            get
            {
                String sRet;
                InfoStatus st = CurrentDataset.Status;
                switch (st)
                {
                    case InfoStatus.Inactive:
                        sRet = "Inactif";
                        break;
                    case InfoStatus.Disabled:
                        sRet = "Désactivé";
                        break;
                    case InfoStatus.Unknown:
                        sRet = "Inconnu";
                        break;
                    default:
                        sRet = "Normal";
                        break;
                }// st
                return sRet;
            }
            set
            {
                string old = DatasetStatus;
                InfoStatus st = InfoStatus.Unknown;
                if (value == "Désactivé")
                {
                    st = InfoStatus.Disabled;
                    CurrentDataset.Status = st;
                }
                else if (value == "Inactif")
                {
                    st = InfoStatus.Inactive;
                    CurrentDataset.Status = st;
                }
                else if (value == "Inconnu")
                {
                    st = InfoStatus.Unknown;
                    CurrentDataset.Status = st;
                }
                else if (value == "Normal")
                {
                    st = InfoStatus.Normal;
                    CurrentDataset.Status = st;
                }
                if (value != old)
                {
                    CurrentDataset.IsModified = true;
                    NotifyPropertyChanged("DatasetStatus");
                    NotifyPropertyChanged("IsDatasetStoreable");
                }

            }
        }
        //
        public async Task PerformVariableSaveAsync()
        {
            DomainManager pMan = Manager;
            if ((pMan != null) && (CurrentVariable != null) && CurrentVariable.IsStoreable)
            {
                await pMan.MaintainsVariableAsync(CurrentVariable);
                await RefreshVariablesAsync();
            }
        }// PerformVariableSave
        public async Task PerformVariableRemoveAsync()
        {
            DomainManager pMan = Manager;
            if ((pMan != null) && (OldVariable != null) && OldVariable.IsPersisted)
            {
                await pMan.RemoveVariableAsync(OldVariable);
                await RefreshVariablesAsync();
            }
        }// Performremove
        public async Task RefreshVariableValuesAsync()
        {
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
        }
        public async Task SelectVariableAsync(Variable pVar)
        {
            _bnewvariable = false;
            OldVariable = pVar;
            if (pVar != null)
            {
                CurrentVariable = new Variable(CurrentDataset, pVar.Sigle)
                {
                    Name = pVar.Name,
                    Observations = pVar.Observations,
                    Status = pVar.Status,
                    VariableKind = pVar.VariableKind,
                    VariableType = pVar.VariableType,
                    IsModified = false
                };
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
            await RefreshVariableValuesAsync();
            NotifyPropertyChanged("IsVariableCreatable");
            NotifyPropertyChanged("IsVariableCancellable");
            NotifyPropertyChanged("IsVariableRemoveable");
        }// SelectVariable
        public async Task RefreshVariablesAsync()
        {
            ObservableCollection<Variable> col = null;
            DomainManager pMan = Manager;
            Dataset pSet = CurrentDataset;
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
            if (Variables.Count() > 0)
            {
                await SelectVariableAsync(Variables.First());
            }
            else
            {
                await SelectVariableAsync(null);
            }
        }
        public async Task PerformVariableNewAsync()
        {
            await SelectVariableAsync(null);
            _bnewvariable = true;
            NotifyPropertyChanged("IsVariableCreatable");
            NotifyPropertyChanged("IsVariableCancellable");
            NotifyPropertyChanged("IsVariableRemoveable");
        }
        public async Task PerformVariableCancelAsync()
        {
            await SelectVariableAsync(OldVariable);
            _bnewvariable = false;
            NotifyPropertyChanged("IsVariableCreatable");
            NotifyPropertyChanged("IsVariableCancellable");
            NotifyPropertyChanged("IsVariableRemoveable");
        }
        public String VariableDetailTitle
        {
            get
            {
                if (_bnewdataset)
                {
                    return "Nouvelle Variable";
                }
                else
                {
                    return String.IsNullOrEmpty(CurrentVariable.Name) ? CurrentVariable.Sigle : CurrentVariable.Name;
                }
            }
            set {
            }
        }
        public ObservableCollection<String> DataTypeStrings
        {
            get
            {
                if (_datatypes == null)
                {
                    lock (_syncObj)
                    {
                        _datatypes = new ObservableCollection<string>
                    {
                        "Réel",
                        "Entier",
                        "Logique",
                        "Texte",
                        "Autre"
                    };
                    }// lock
                }// _datatypes
                return _datatypes;
            }
            set {
                NotifyPropertyChanged("DataTypeStrings");
            }
        }//DataTypeSStrings
        public ObservableCollection<String> DataKindStrings
        {
            get
            {
                if (_datakind == null)
                {
                    lock (_syncObj)
                    {
                        _datakind = new ObservableCollection<string>
                    {
                        "Normal",
                        "Modal",
                        "Ordinal"
                    };
                    }// lock
                }// _datakind
                return _datakind;
            }
            set { }
        }//DataKindStrings
        public ObservableCollection<Variable> Variables
        {
            get
            {
                if (_variables == null)
                {
                    RefreshVariablesAsync().Wait();
                }
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
                NotifyPropertyChanged("VariableDetailTitle");
                NotifyPropertyChanged("VariableSigle");
                NotifyPropertyChanged("VariableName");
                NotifyPropertyChanged("VariableObservations");
                NotifyPropertyChanged("VariableStatus");
                NotifyPropertyChanged("VariableType");
                NotifyPropertyChanged("VariableKind");
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
        public String VariableSigle
        {
            get
            {
                return CurrentVariable.Sigle;
            }
            set
            {
                string old = VariableSigle;
                CurrentVariable.Sigle = value;
                string cur = VariableSigle;
                if (old != cur)
                {
                    CurrentVariable.IsModified = true;
                    NotifyPropertyChanged("VariableSigle");
                    NotifyPropertyChanged("IsVariableStoreable");
                }
            }
        }// VariableSigle
        public String VariableName
        {
            get
            {
                return CurrentVariable.Name;
            }
            set
            {
                string old = VariableName;
                CurrentVariable.Name = value;
                string cur = VariableName;
                if (old != cur)
                {
                    CurrentVariable.IsModified = true;
                    NotifyPropertyChanged("VariableName");
                    NotifyPropertyChanged("IsVariableStoreable");
                }
            }
        }// VariableName
        public String VariableObservations
        {
            get
            {
                return CurrentVariable.Observations;
            }
            set
            {
                string old = VariableObservations;
                CurrentVariable.Observations = value;
                string cur = VariableObservations;
                if (cur != old)
                {
                    CurrentVariable.IsModified = true;
                    NotifyPropertyChanged("VariableObservations");
                    NotifyPropertyChanged("IsVariableStoreable");
                }
            }
        }// VariableObservations
        public String VariableStatus
        {
            get
            {
                String sRet;
                InfoStatus st = CurrentVariable.Status;
                switch (st)
                {
                    case InfoStatus.Inactive:
                        sRet = "Inactif";
                        break;
                    case InfoStatus.Disabled:
                        sRet = "Désactivé";
                        break;
                    case InfoStatus.Unknown:
                        sRet = "Inconnu";
                        break;
                    default:
                        sRet = "Normal";
                        break;
                }// st
                return sRet;
            }
            set
            {
                string old = VariableStatus;
                InfoStatus st = InfoStatus.Unknown;
                if (value == "Désactivé")
                {
                    st = InfoStatus.Disabled;
                    CurrentVariable.Status = st;
                }
                else if (value == "Inactif")
                {
                    st = InfoStatus.Inactive;
                    CurrentVariable.Status = st;
                }
                else if (value == "Inconnu")
                {
                    st = InfoStatus.Unknown;
                    CurrentVariable.Status = st;
                }
                else if (value == "Normal")
                {
                    st = InfoStatus.Normal;
                    CurrentVariable.Status = st;
                }
                if (old != value)
                {
                    CurrentVariable.IsModified = true;
                    NotifyPropertyChanged("VariableStatus");
                    NotifyPropertyChanged("IsVariableStoreable");
                }
            }
        }
        public String VariableType
        {
            get
            {
                String sRet;
                switch (CurrentVariable.VariableType)
                {
                    case InfoDataType.Integer:
                        sRet = "Entier";
                        break;
                    case InfoDataType.Logical:
                        sRet = "Logique";
                        break;
                    case InfoDataType.Text:
                        sRet = "Texte";
                        break;
                    case InfoDataType.Other:
                        sRet = "Autre";
                        break;
                    default:
                        sRet = "Réel";
                        break;
                }// status
                return sRet;
            }
            set
            {
                string old = VariableType;
                if (value == "Entier")
                {
                    CurrentVariable.VariableType = InfoDataType.Integer;
                }
                else if (value == "Logique")
                {
                    CurrentVariable.VariableType = InfoDataType.Logical;
                }
                else if (value == "Texte")
                {
                    CurrentVariable.VariableType = InfoDataType.Text;
                }
                else if (value == "Réel")
                {
                    CurrentVariable.VariableType = InfoDataType.Real;
                }
                else if (value == "Autre")
                {
                    CurrentVariable.VariableType = InfoDataType.Other;
                }
                if (old != value)
                {
                    CurrentVariable.IsModified = true;
                    NotifyPropertyChanged("VariableType");
                    NotifyPropertyChanged("IsVariableStoreable");
                }
            }
        }
        public String VariableKind
        {
            get
            {
                String sRet;
                switch (CurrentVariable.VariableKind)
                {
                    case InfoKind.Normal:
                        sRet = "Normal";
                        break;
                    case InfoKind.Modal:
                        sRet = "Modal";
                        break;
                    case InfoKind.Ordinal:
                        sRet = "Ordinal";
                        break;
                    default:
                        sRet = "Normal";
                        break;
                }
                return sRet;
            }
            set
            {
                string old = VariableKind;
                if (value == "Normal")
                {
                    CurrentVariable.VariableKind = InfoKind.Normal;
                }
                else if (value == "Modal")
                {
                    CurrentVariable.VariableKind = InfoKind.Modal;
                }
                else if (value == "Ordinal")
                {
                    CurrentVariable.VariableKind = InfoKind.Ordinal;
                }
                if (old != value)
                {
                    CurrentVariable.IsModified = true;
                    NotifyPropertyChanged("VariableKind");
                    NotifyPropertyChanged("IsVariableStoreable");
                }
            }
        }
        public ObservableCollection<InfoValue> VariableValues
        {
            get
            {
                if (_varvalues == null)
                {
                    RefreshVariableValuesAsync().Wait();
                }
                return _varvalues ?? new ObservableCollection<InfoValue>();
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
                return (!_bnewvariable);
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
                return (CurrentVariable != null) && CurrentVariable.IsStoreable && CurrentVariable.IsModified;
            }
            set {
                NotifyPropertyChanged("IsVariableStoreable");
            }
        }
        public bool IsVariableRemoveable
        {
            get
            {
                return (OldVariable != null) && OldVariable.IsPersisted && (!_bnewvariable);
            }
            set {
                NotifyPropertyChanged("IsVariableRemoveable");
            }
        }
        public bool IsVariableCancellable
        {
            get
            {
                return (OldVariable != null) && OldVariable.IsPersisted && _bnewvariable;
            }
            set
            {
                NotifyPropertyChanged("IsVariableCancellable");
            }
        }
        //
        public async Task RefreshIndivsAsync()
        {
            ObservableCollection<Indiv> col = null;
            DomainManager pMan = Manager;
            Dataset pSet = CurrentDataset;
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
            if (Indivs.Count() > 0)
            {
               await  SelectIndivAsync(Indivs.First());
            }
            else
            {
                await SelectIndivAsync(null);
            }
        }// RefreshIndivs
        public async Task SelectIndivAsync(Indiv pInd)
        {
            _bnewindiv = false;
            OldIndiv = pInd;
            if (pInd != null)
            {
                CurrentIndiv = new Indiv(CurrentDataset, pInd.Sigle)
                {
                    Name = pInd.Name,
                    Observations = pInd.Observations,
                    Status = pInd.Status,
                    IsModified = false
                };
            }
            else
            {
                CurrentIndiv = new Indiv(CurrentDataset,"")
                {
                    Status = InfoStatus.Normal,
                    IsModified = false
                };
            }
            await RefreshIndivValuesAsync();
            NotifyPropertyChanged("IsIndivCreatable");
            NotifyPropertyChanged("IsIndivCancellable");
            NotifyPropertyChanged("IsIndivRemoveable");
        }// SelectIndiv
        public async Task PerformIndivSaveAsync()
        {
            DomainManager pMan = Manager;
            if ((pMan != null) && (CurrentIndiv != null) && CurrentIndiv.IsStoreable)
            {
                await pMan.MaintainsIndivAsync(CurrentIndiv);
                await RefreshIndivsAsync();
            }
        }// PerformIndivSave
        public async Task PerformIndivRemoveAsync()
        {
            DomainManager pMan = Manager;
            if ((pMan != null) && (OldIndiv != null) && OldIndiv.IsPersisted)
            {
                await pMan.RemoveIndivAsync(OldIndiv);
                await RefreshIndivsAsync();
            }
        }// Performremoveindiv
        public async Task PerformIndivNewAsync()
        {
            await SelectIndivAsync(null);
            _bnewindiv = true;
            NotifyPropertyChanged("IsIndivCreatable");
            NotifyPropertyChanged("IsIndivCancellable");
            NotifyPropertyChanged("IsIndivRemoveable");
        }
        public async Task PerformIndivCancelAsync()
        {
            await SelectIndivAsync(OldIndiv);
            _bnewvariable = false;
            NotifyPropertyChanged("IsIndivCreatable");
            NotifyPropertyChanged("IsIndivCancellable");
            NotifyPropertyChanged("IsIndivRemoveable");
        }
        public async Task RefreshIndivValuesAsync()
        {
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
        public String IndivDetailTitle
        {
            get
            {
                if (_bnewindiv)
                {
                    return "Nouvel Individu";
                }
                else
                {
                    return CurrentIndiv.Name;
                }
            }
            set
            {
                NotifyPropertyChanged("IndivDetailTitle");
            }
        }
        public ObservableCollection<Indiv> Indivs
        {
            get
            {
                if (_indivs == null)
                {
                    RefreshIndivsAsync().Wait();
                }
                return _indivs ?? new ObservableCollection<Indiv>();
            }
            set {
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
                NotifyPropertyChanged("IndivDetailTitle");
                NotifyPropertyChanged("IndivSigle");
                NotifyPropertyChanged("IndivName");
                NotifyPropertyChanged("IndivObservations");
                NotifyPropertyChanged("IndivStatus");
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
        public String IndivSigle
        {
            get
            {
                return CurrentIndiv.Sigle;
            }
            set
            {
                string old = IndivSigle;
                CurrentIndiv.Sigle = value;
                string cur = IndivSigle;
                if (old != cur)
                {
                    CurrentIndiv.IsModified = true;
                    NotifyPropertyChanged("IndivSigle");
                    NotifyPropertyChanged("IsIndivStoreable");
                }
            }
        }// IndivSigle
        public String IndivName
        {
            get
            {
                return CurrentIndiv.Name;
            }
            set
            {
                string old = IndivName;
                CurrentIndiv.Name = value;
                string cur = IndivName;
                if (old != cur)
                {
                    CurrentIndiv.IsModified = true;
                    NotifyPropertyChanged("IndivName");
                    NotifyPropertyChanged("IsIndivStoreable");
                }
            }
        }// IndivName
        public String IndivObservations
        {
            get
            {
                return CurrentIndiv.Observations;
            }
            set
            {
                string old = IndivObservations;
                CurrentIndiv.Observations = value;
                string cur = IndivObservations;
                if (cur != old)
                {
                    CurrentIndiv.IsModified = true;
                    NotifyPropertyChanged("IndivObservations");
                    NotifyPropertyChanged("IsIndivStoreable");
                }
            }
        }// IndivObservations
        public String IndivStatus
        {
            get
            {
                String sRet;
                InfoStatus st = CurrentIndiv.Status;
                switch (st)
                {
                    case InfoStatus.Inactive:
                        sRet = "Inactif";
                        break;
                    case InfoStatus.Disabled:
                        sRet = "Désactivé";
                        break;
                    case InfoStatus.Unknown:
                        sRet = "Inconnu";
                        break;
                    default:
                        sRet = "Normal";
                        break;
                }// st
                return sRet;
            }
            set
            {
                string old = IndivStatus;
                InfoStatus st = InfoStatus.Unknown;
                if (value == "Désactivé")
                {
                    st = InfoStatus.Disabled;
                    CurrentIndiv.Status = st;
                }
                else if (value == "Inactif")
                {
                    st = InfoStatus.Inactive;
                    CurrentIndiv.Status = st;
                }
                else if (value == "Inconnu")
                {
                    st = InfoStatus.Unknown;
                    CurrentIndiv.Status = st;
                }
                else if (value == "Normal")
                {
                    st = InfoStatus.Normal;
                    CurrentIndiv.Status = st;
                }
                if (value != old)
                {
                    CurrentIndiv.IsModified = true;
                    NotifyPropertyChanged("IndivStatus");
                    NotifyPropertyChanged("IsIndivStoreable");
                }
            }
        }
        public ObservableCollection<InfoValue> IndivValues
        {
            get
            {
                if (_indvalues == null)
                {
                    RefreshIndivValuesAsync().Wait();
                }
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
                return (CurrentIndiv != null) && CurrentIndiv.IsStoreable && CurrentIndiv.IsModified;
            }
            set {
                NotifyPropertyChanged("IsIndivStoreable");
            }
        }
        public bool IsIndivRemoveable
        {
            get
            {
                return (OldIndiv != null) && OldIndiv.IsPersisted && (!_bnewindiv);
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
