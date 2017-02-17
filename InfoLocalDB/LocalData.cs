using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel.DataAnnotations;

namespace InfoLocalDB
{
    public sealed class LocalDataset
    {
        //
        private string _sigle;
        private string _name;
        private string _desc;
        //
        public LocalDataset() { }
        public LocalDataset(string sigle)
        {
            this.Sigle = sigle;
            this.Name = sigle;
        }
        //
        [Key]
        public int LocalDatasetId { get; set; }
        [MaxLength(127)]
        public string RemoteId { get; set; }
        [MaxLength(127)]
        public string RemoteVersion { get; set; }
        [Required]
        [MinLength(1)]
        [MaxLength(31)]
        public string Sigle
        {
            get
            {
                return _sigle;
            }
            set
            {
                string s = string.IsNullOrEmpty(value) ? "" : value.Trim().ToUpper();
                if (s.Length > 31)
                {
                    s = s.Substring(0, 31).Trim();
                }
                _sigle = s;
            }
        }
        [Required]
        [MinLength(1)]
        [MaxLength(63)]
        public string Name
        {
            get
            {
                return _name;
            }
            set
            {
                string s = (string.IsNullOrEmpty(value)) ? "" : value.Trim();
                if (s.Length > 63)
                {
                    s = s.Substring(0, 63).Trim();
                }
                _name = s;
            }
        }
        public int Status { get; set; }
        [MaxLength(255)]
        public string Description
        {
            get
            {
                return _desc;
            }
            set
            {
                string s = (string.IsNullOrEmpty(value)) ? "" : value.Trim();
                if (s.Length > 255)
                {
                    s = s.Substring(0, 63).Trim();
                }
                _desc = s;
            }
        }
        //
        public IList<LocalIndiv> localIndivs { get; set; }
        public IList<LocalVariable> LocalVariables { get; set; }
    }// DBDataset
    public sealed class LocalIndiv
    {
        //
        private string _sigle;
        private string _name;
        private string _desc;
        //
        public LocalIndiv() { }
        public LocalIndiv(LocalDataset pSet, string sigle)
        {
            if (pSet == null)
            {
                throw new ArgumentNullException();
            }
            this.LocalDataset = pSet;
            this.LocalDatasetId = pSet.LocalDatasetId;
            this.Sigle = sigle;
            this.Name = sigle;
        }
        //
        [Key]
        public int LocalIndivId { get; set; }
        [MaxLength(127)]
        public string RemoteId { get; set; }
        [MaxLength(127)]
        public string RemoteVersion { get; set; }
        [Required]
        [MinLength(1)]
        [MaxLength(31)]
        public string Sigle
        {
            get
            {
                return _sigle;
            }
            set
            {
                string s = string.IsNullOrEmpty(value) ? "" : value.Trim().ToUpper();
                if (s.Length > 31)
                {
                    s = s.Substring(0, 31).Trim();
                }
                _sigle = s;
            }
        }
        [Required]
        [MinLength(1)]
        [MaxLength(63)]
        public string Name
        {
            get
            {
                return _name;
            }
            set
            {
                string s = (string.IsNullOrEmpty(value)) ? "" : value.Trim();
                if (s.Length > 63)
                {
                    s = s.Substring(0, 63).Trim();
                }
                _name = s;
            }
        }
        public int Status { get; set; }
        [MaxLength(255)]
        public string Description
        {
            get
            {
                return _desc;
            }
            set
            {
                string s = (string.IsNullOrEmpty(value)) ? "" : value.Trim();
                if (s.Length > 255)
                {
                    s = s.Substring(0, 63).Trim();
                }
                _desc = s;
            }
        }
        //
        public int LocalDatasetId { get; set; }
        public LocalDataset LocalDataset { get; set; }
        //
        public IList<LocalValue> LocalValues { get; set; }
    }// LocalIndiv
     //
    public sealed class LocalVariable
    {
        //
        private string _sigle;
        private string _name;
        private string _desc;
        //
        public LocalVariable() { }
        public LocalVariable(LocalDataset pSet, string sigle)
        {
            if (pSet == null)
            {
                throw new ArgumentNullException();
            }
            this.LocalDataset = pSet;
            this.LocalDatasetId = pSet.LocalDatasetId;
        }
        //
        [Key]
        public int LocalVariableId { get; set; }
        [Timestamp]
        public byte[] Timestamp { get; set; }
        //
        [Required]
        [MinLength(1)]
        [MaxLength(31)]
        public string Sigle
        {
            get
            {
                return _sigle;
            }
            set
            {
                string s = string.IsNullOrEmpty(value) ? "" : value.Trim().ToUpper();
                if (s.Length > 31)
                {
                    s = s.Substring(0, 31).Trim();
                }
                _sigle = s;
            }
        }
        [Required]
        [MinLength(1)]
        [MaxLength(63)]
        public string Name
        {
            get
            {
                return _name;
            }
            set
            {
                string s = (string.IsNullOrEmpty(value)) ? "" : value.Trim();
                if (s.Length > 63)
                {
                    s = s.Substring(0, 63).Trim();
                }
                _name = s;
            }
        }
        public int Status { get; set; }
        [MaxLength(255)]
        public string Description
        {
            get
            {
                return _desc;
            }
            set
            {
                string s = (string.IsNullOrEmpty(value)) ? "" : value.Trim();
                if (s.Length > 255)
                {
                    s = s.Substring(0, 63).Trim();
                }
                _desc = s;
            }
        }

        public int VariableType { get; set; }
        public int VariableKind { get; set; }
        public string ModalitesString { get; set; }
        //
        [MaxLength(127)]
        public string RemoteId { get; set; }
        [MaxLength(127)]
        public string RemoteVersion { get; set; }
        //
        public int LocalDatasetId { get; set; }
        public LocalDataset LocalDataset { get; set; }
        public IList<LocalValue> LocalValues { get; set; }
    }// LocalVariable
    //
    public sealed class LocalValue
    {
        //
        private string _desc;
        private string _sval;
        //
        public LocalValue() { }
        public LocalValue(LocalIndiv pInd, LocalVariable pVar)
        {
            if ((pInd == null) || (pVar == null))
            {
                throw new ArgumentNullException();
            }
            this.LocalIndiv = pInd;
            this.LocalIndivId = pInd.LocalIndivId;
            this.LocalVariable = pVar;
            this.LocalVariableId = pVar.LocalVariableId;
        }
        //
        [Key]
        public int LocalValueId { get; set; }
        [Timestamp]
        public byte[] Timestamp { get; set; }
        //
        public int LocalIndivId { get; set; }
        public LocalIndiv LocalIndiv { get; set; }
        public int LocalVariableId { get; set; }
        public LocalVariable LocalVariable { get; set; }
        //
        public int VariableType { get; set; }
        [MaxLength(15)]
        public string StringValue
        {
            get
            {
                return _sval;
            }
            set
            {
                string s = (string.IsNullOrEmpty(value)) ? "" : value.Trim().ToUpper();
                if (s.Length > 15)
                {
                    s = s.Substring(0, 15).Trim();
                }
                _sval = s;
            }
        }
        [MaxLength(255)]
        public string Description
        {
            get
            {
                return _desc;
            }
            set
            {
                string s = (string.IsNullOrEmpty(value)) ? "" : value.Trim();
                if (s.Length > 255)
                {
                    s = s.Substring(0, 63).Trim();
                }
                _desc = s;
            }
        }
        public int Status { get; set; }
        //
        [MaxLength(127)]
        public string RemoteId { get; set; }
        [MaxLength(127)]
        public string RemoteVersion { get; set; }
    }// DBValue
}
