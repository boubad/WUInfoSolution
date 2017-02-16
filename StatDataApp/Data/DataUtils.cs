using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using InfoDomain;
//
namespace StatDataApp.Data
{
    class DataUtils
    {
        public static DBDataset DatasetR2L(Dataset p)
        {
            DBDataset pRet = null;
            if (p != null)
            {
                pRet = new DBDataset();
                pRet.RemoteId = p.Id;
                pRet.RemoteVersion = p.Rev;
                pRet.Description = p.Observations;
                pRet.Status = (int)p.Status;
                pRet.Sigle = p.Sigle;
                pRet.Name = p.Name;
            }//p
            return pRet;
        }//DatasetR2L
        public static DBIndiv IndivR2L(Indiv p)
        {
            DBIndiv pRet = null;
            if (p != null)
            {
                pRet = new DBIndiv();
                pRet.RemoteId = p.Id;
                pRet.RemoteVersion = p.Rev;
                pRet.Description = p.Observations;
                pRet.Status = (int)p.Status;
                pRet.Sigle = p.Sigle;
                pRet.Name = p.Name;
            }//p
            return pRet;
        }//IndivR2L
        public static DBVariable VariableR2L(Variable p)
        {
            DBVariable pRet = null;
            if (p != null)
            {
                pRet = new DBVariable();
                pRet.RemoteId = p.Id;
                pRet.RemoteVersion = p.Rev;
                pRet.Description = p.Observations;
                pRet.Status = (int)p.Status;
                pRet.Sigle = p.Sigle;
                pRet.Name = p.Name;
                pRet.VariableType = (int)p.VariableType;
                pRet.VariableKind = (int)p.VariableKind;
                var ss = p.Modalites;
                string sx = "";
                if (ss != null)
                {
                    foreach (var x in ss)
                    {
                        if (!string.IsNullOrEmpty(sx))
                        {
                            sx += ";";
                        }
                        sx += x;
                    }// x
                }// ss
                pRet.ModalitesString = sx;
            }//p
            return pRet;
        }//IndivR2L
        public static DBValue ValueR2L(InfoValue p)
        {
            DBValue pRet = null;
            if (p != null)
            {
                pRet = new DBValue();
                pRet.RemoteId = p.Id;
                pRet.RemoteVersion = p.Rev;
                pRet.Description = p.Observations;
                pRet.Status = (int)p.Status;
                var v = p.Value;
                if (v != null)
                {
                    pRet.VariableType = (int)v.DataType;
                    pRet.StringValue = v.ToString();
                }
            }//p
            return pRet;
        }//IndivR2L
    }// class DataUtils
}
