using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using InfoDomain;
using InfoLocalDB;
//
namespace StatDataApp.Data
{
    class DataUtils
    {
        public static LocalDataset DatasetR2L(Dataset p)
        {
            LocalDataset pRet = null;
            if (p != null)
            {
                pRet = new LocalDataset()
                {
                    RemoteId = p.Id,
                    RemoteVersion = p.Rev,
                    Description = p.Observations,
                    Status = (int)p.Status,
                    Sigle = p.Sigle,
                    Name = p.Name
                };
            }//p
            return pRet;
        }//DatasetR2L
        public static LocalIndiv IndivR2L(Indiv p)
        {
            LocalIndiv pRet = null;
            if (p != null)
            {
                pRet = new LocalIndiv()
                {
                    RemoteId = p.Id,
                    RemoteVersion = p.Rev,
                    Description = p.Observations,
                    Status = (int)p.Status,
                    Sigle = p.Sigle,
                    Name = p.Name
                };
            }//p
            return pRet;
        }//IndivR2L
        public static LocalVariable VariableR2L(Variable p)
        {
           LocalVariable pRet = null;
            if (p != null)
            {
                pRet = new LocalVariable()
                {
                    RemoteId = p.Id,
                    RemoteVersion = p.Rev,
                    Description = p.Observations,
                    Status = (int)p.Status,
                    Sigle = p.Sigle,
                    Name = p.Name,
                    VariableType = (int)p.VariableType,
                    VariableKind = (int)p.VariableKind
                };
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
        public static LocalValue ValueR2L(InfoValue p)
        {
            LocalValue pRet = null;
            if (p != null)
            {
                pRet = new LocalValue()
                {
                    RemoteId = p.Id,
                    RemoteVersion = p.Rev,
                    Description = p.Observations,
                    Status = (int)p.Status
                };
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
