using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Collections.ObjectModel;
using InfoDomain;

namespace InfoControls
{
    class DatasetEditModel
    {
        //
        private ObservableCollection<Dataset> _datasets;
        private Dataset _currentDataset;
        private Dataset _oldset;
        ObservableCollection<String> _status;
        String _baseUrl = "http://localhost:5984/";
        String _databaseName = "test";
        DomainManager _m_pman;
        //
        ObservableCollection<String> _datatypes;
        ObservableCollection<String> _datakind;
        ObservableCollection<Variable> _variables;
        Variable _currentVariable;
        Variable _oldVariable;
        ObservableCollection<Indiv> _indivs;
        Indiv _currentIndiv;
        Indiv _oldIndiv;
        //
        ObservableCollection<InfoValue> _varvalues;
        ObservableCollection<InfoValue> _indvalues;
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
            }
        }// DatabaseName
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
            set { }
        }// Manager
        public ObservableCollection<String> StatusStrings
        {
            get
            {
                if (_status == null)
                {
                    _status = new ObservableCollection<string>();
                    _status.Add("Normal");
                    _status.Add("Inactif");
                    _status.Add("Désactivé");
                    _status.Add("Inconnu");
                }
                return _status;
            }
            set { }
        }
        //
        public async Task RefreshDatasets()
        {
            _currentDataset = null;
            _oldset = null;
            DomainManager pMan = Manager;
            _datasets = new ObservableCollection<Dataset>();
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
                            _datasets = new ObservableCollection<Dataset>(oList.OrderBy(x => x.Sigle));
                        }
                        else
                        {
                            _datasets = new ObservableCollection<Dataset>(oList);
                        }
                    }// oList
                }// n
            }// pMan
            if (_datasets.Count() > 0)
            {
                SelectDataset(_datasets.First());
            }
            else
            {
                SelectDataset(null);
            }
        }
        public async Task PerformDatasetSave()
        {
            DomainManager pMan = Manager;
            if ((pMan != null) && (CurrentDataset != null) && CurrentDataset.IsStoreable)
            {
                await pMan.MaintainsDatasetAsync(CurrentDataset);
                await RefreshDatasets();
            }
        }// PerformSave
        public async Task PerformRemoveDataset()
        {
            DomainManager pMan = Manager;
            if ((pMan != null) && (OldDataset != null) && OldDataset.IsPersisted)
            {
                await pMan.RemoveDatasetAsync(OldDataset);
                await RefreshDatasets();
            }
        }// Performremove
        public void PerformDatasetNew()
        {
            _currentDataset = null;
        }
        public void PerformDatasetCancel()
        {
            SelectDataset(OldDataset);
        }
        public bool IsDatasetStoreable
        {
            get
            {
                return (CurrentDataset != null) && CurrentDataset.IsStoreable;
            }
            set { }
        }
        public bool IsDatasetRemoveable
        {
            get
            {
                return (OldDataset != null) && OldDataset.IsPersisted;
            }
            set { }
        }
        public ObservableCollection<Dataset> Datasets
        {
            get
            {
                if (_datasets == null)
                {
                    RefreshDatasets().Wait();
                }
                return _datasets;
            }
            set { }
        }
        public String DatasetDetailTitle
        {
            get
            {
                return CurrentDataset.Name;
            }
            set { }
        }
        public String DatasetAnnee
        {
            get
            {
                return CurrentDataset.Annee;
            }
            set
            {
                CurrentDataset.Annee = value;
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
                CurrentDataset.Sigle = value;
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
                CurrentDataset.Name = value;
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
                CurrentDataset.Observations = value;
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

            }
        }
        public Dataset CurrentDataset
        {
            get
            {
                if (_currentDataset == null)
                {
                    _currentDataset = new Dataset();
                    _currentDataset.Status = InfoStatus.Normal;
                }
                return _currentDataset;
            }
            set
            {
                SelectDataset(value);
            }
        }
        public Dataset OldDataset
        {
            get
            {
                return _oldset;
            }
            set { }
        }
        public void SelectDataset(Dataset pSet)
        {
            _oldset = pSet;
            if (pSet != null)
            {
                _currentDataset = new Dataset(pSet.Sigle);
                _currentDataset.Name = pSet.Name;
                _currentDataset.Observations = pSet.Observations;
                _currentDataset.Status = pSet.Status;
            }
            else
            {
                _currentDataset = new Dataset();
                _currentDataset.Status = InfoStatus.Normal;
            }
            RefreshVariables().Wait();
            RefreshIndivs().Wait();
        }// SelectDataset
         //
        public String VariableDetailTitle
        {
            get
            {
                return CurrentVariable.Name;
            }
            set { }
        }
        public ObservableCollection<String> DataTypeStrings
        {
            get
            {
                if (_datatypes == null)
                {
                    _datatypes = new ObservableCollection<string>();
                    _datatypes.Add("Réel");
                    _datatypes.Add("Entier");
                    _datatypes.Add("Logique");
                    _datatypes.Add("Texte");
                    _datatypes.Add("Autre");
                }// _datatypes
                return _datatypes;
            }
            set { }
        }//DataTypeSStrings
        public ObservableCollection<String> DataKindStrings
        {
            get
            {
                if (_datakind == null)
                {
                    _datakind = new ObservableCollection<string>();
                    _datakind.Add("Normal");
                    _datakind.Add("Modal");
                    _datakind.Add("Ordinal");
                }// _datakind
                return _datakind;
            }
            set { }
        }//DataKindStrings
        public void SelectVariable(Variable pVar)
        {
            _oldVariable = pVar;
            _varvalues = null;
            if (pVar != null)
            {
                _currentVariable = new Variable(CurrentDataset, pVar.Sigle);
                _currentVariable.Name = pVar.Name;
                _currentVariable.Observations = pVar.Observations;
                _currentVariable.Status = pVar.Status;
                _currentVariable.VariableKind = pVar.VariableKind;
                _currentVariable.VariableType = pVar.VariableType;
            }
            else
            {
                _currentVariable = new Variable(CurrentDataset, "")
                {
                    Status = InfoStatus.Normal,
                    VariableKind = InfoKind.Normal,
                    VariableType = InfoDataType.Real
                };
            }
        }// SelectVariable
        public async Task RefreshVariables()
        {
            _currentVariable = null;
            _oldVariable = null;
            DomainManager pMan = Manager;
            _variables = new ObservableCollection<Variable>();
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
                            _variables = new ObservableCollection<Variable>(oList.OrderBy(x => x.Sigle));
                        }
                        else
                        {
                            _variables = new ObservableCollection<Variable>(oList);
                        }
                    }// oList
                }// n
            }// pMan
            if (_variables.Count() > 0)
            {
                SelectVariable(_variables.First());
            }
            else
            {
                SelectVariable(null);
            }
        }
        public ObservableCollection<Variable> Variables
        {
            get
            {
                if (_variables == null)
                {
                    RefreshVariables().Wait();
                }
                return _variables;
            }
            set { }
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
                SelectVariable(value);
            }
        }// Currentvariable
        public Variable OldVariable
        {
            get
            {
                return _oldVariable;
            }
            set { }
        }// OldDataset
        public String VariableSigle
        {
            get
            {
                return CurrentVariable.Sigle;
            }
            set
            {
                CurrentVariable.Sigle = value;
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
                CurrentVariable.Name = value;
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
                CurrentVariable.Observations = value;
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
            }
        }
        public async Task PerformVariableSave()
        {
            DomainManager pMan = Manager;
            if ((pMan != null) && (CurrentVariable != null) && CurrentVariable.IsStoreable)
            {
                await pMan.MaintainsVariableAsync(CurrentVariable);
                await RefreshVariables();
            }
        }// PerformVariableSave
        public async Task PerformVariableRemove()
        {
            DomainManager pMan = Manager;
            if ((pMan != null) && (OldVariable != null) && OldVariable.IsPersisted)
            {
                await pMan.RemoveVariableAsync(OldVariable);
                await RefreshVariables();
            }
        }// Performremove
        public async Task RefreshVariableValues()
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
            _varvalues = new ObservableCollection<InfoValue>(bList.OrderBy(x => x.IndivSigle));
        }
        public ObservableCollection<InfoValue> VariableValues
        {
            get
            {
                if (_varvalues == null)
                {
                    RefreshVariableValues().Wait();
                }
                return _varvalues;
            }
            set
            {
                _varvalues = null;
            }
        }// VariableValues
        public void PerformVariableNew()
        {
            _currentVariable = null;
        }
        public void PerformVariableCancel()
        {
            SelectVariable(OldVariable);
        }
        public bool IsVariableStoreable
        {
            get
            {
                return (CurrentVariable != null) && CurrentVariable.IsStoreable;
            }
            set { }
        }
        public bool IsVariableRemoveable
        {
            get
            {
                return (OldVariable != null) && OldVariable.IsPersisted;
            }
            set { }
        }
        //
        public String IndivDetailTitle
        {
            get
            {
                return CurrentIndiv.Name;
            }
            set { }
        }
        public async Task RefreshIndivs()
        {
            _currentIndiv = null;
            _oldIndiv = null;
            DomainManager pMan = Manager;
            _indivs = new ObservableCollection<Indiv>();
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
                            _indivs = new ObservableCollection<Indiv>(oList.OrderBy(x => x.Sigle));
                        }
                        else
                        {
                            _indivs = new ObservableCollection<Indiv>(oList);
                        }
                    }// oList
                }// n
            }// pMan
            if (_indivs.Count() > 0)
            {
                SelectIndiv(_indivs.First());
            }
            else
            {
                SelectIndiv(null);
            }
        }// RefreshIndivs
        public void SelectIndiv(Indiv pInd)
        {
            _oldIndiv = pInd;
            _indvalues = null;
            if (pInd != null)
            {
                _currentIndiv = new Indiv(CurrentDataset, pInd.Sigle);
                _currentIndiv.Name = pInd.Name;
                _currentIndiv.Observations = pInd.Observations;
                _currentIndiv.Status = pInd.Status;
            }
            else
            {
                _currentIndiv = new Indiv();
            }
        }// SelectIndiv
        public async Task PerformIndivSave()
        {
            DomainManager pMan = Manager;
            if ((pMan != null) && (CurrentIndiv != null) && CurrentIndiv.IsStoreable)
            {
                await pMan.MaintainsIndivAsync(CurrentIndiv);
                await RefreshIndivs();
            }
        }// PerformIndivSave
        public async Task PerformIndivRemove()
        {
            DomainManager pMan = Manager;
            if ((pMan != null) && (OldIndiv != null) && OldIndiv.IsPersisted)
            {
                await pMan.RemoveIndivAsync(OldIndiv);
                await RefreshIndivs();
            }
        }// Performremoveindiv
        public ObservableCollection<Indiv> Indivs
        {
            get
            {
                if (_indivs == null)
                {
                    RefreshIndivs().Wait();
                }
                return _indivs;
            }
            set { }
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
                SelectIndiv(value);
            }
        }// CurrentIndiv
        public Indiv OldIndiv
        {
            get
            {
                return _oldIndiv;
            }
            set { }
        }// OldDataset
        public String IndivSigle
        {
            get
            {
                return CurrentIndiv.Sigle;
            }
            set
            {
                CurrentIndiv.Sigle = value;
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
                CurrentIndiv.Name = value;
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
                CurrentIndiv.Observations = value;
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

            }
        }
        public async Task RefreshIndivValues()
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
            _indvalues = new ObservableCollection<InfoValue>(bList.OrderBy(x => x.IndivSigle));
        }
        public ObservableCollection<InfoValue> IndivValues
        {
            get
            {
                if (_indvalues == null)
                {
                    RefreshIndivValues().Wait();
                }
                return _indvalues;
            }
            set
            {
                _indvalues = null;
            }
        }// VariableValues
        public void PerformIndivNew()
        {
            _currentIndiv = null;
        }
        public void PerformIndivCancel()
        {
            SelectIndiv(OldIndiv);
        }
        public bool IsIndivStoreable
        {
            get
            {
                return (CurrentIndiv != null) && CurrentIndiv.IsStoreable;
            }
            set { }
        }
        public bool IsIndivRemoveable
        {
            get
            {
                return (OldIndiv != null) && OldIndiv.IsPersisted;
            }
            set { }
        }

    }// class DatasetEditModel
}// namespace InfoControls
