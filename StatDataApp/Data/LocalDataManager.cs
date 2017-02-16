using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.EntityFrameworkCore;

namespace StatDataApp.Data
{
    class LocalDataManager : IDisposable
    {
        private readonly int MAX_COUNT = 20;
        private StatDataAppContext m_ctx;
        //
        public LocalDataManager()
        {
            m_ctx = new StatDataAppContext();
        }
        public LocalDataManager(StatDataAppContext ctx)
        {
            m_ctx = ctx;
        }
        public void Dispose()
        {
            if (m_ctx != null)
            {
                ((IDisposable)m_ctx).Dispose();
                m_ctx = null;
            }
        }
        public async Task<int> GetDatasetsCountAsync()
        {
            var pp = await m_ctx.DBDatasets.ToListAsync();
            return pp.Count();
        }//GetDatasetsCountAsync
        public async Task<IList<DBDataset>> GetDatasetsAsync(int offset = 0, int count = 0)
        {
            if (offset < 0)
            {
                offset = 0;
            }
            if (count < 1)
            {
                count = MAX_COUNT;
            }
            IList<DBDataset> oRet = await m_ctx.
                DBDatasets.Skip(0).Take(count).
                OrderBy(x => x.Sigle).ToListAsync();
            return oRet;
        }//GetDatasetsAsync
        public async Task<DBDataset> FindDatasetAsync(DBDataset p)
        {
            if (p == null)
            {
                throw new ArgumentNullException();
            }
            DBDataset pRet = null;
            int nId = p.DBDatasetId;
            if (nId != 0)
            {
                pRet = await m_ctx.DBDatasets.FindAsync(nId);
                if (pRet != null)
                {
                    return pRet;
                }
            }
            string s = p.Sigle;
            if (!string.IsNullOrEmpty(s))
            {
                string sigle = s.Trim().ToUpper();
                if (!string.IsNullOrEmpty(sigle))
                {
                    pRet = await m_ctx.DBDatasets.Where(x => x.Sigle == sigle).SingleAsync();
                }// sigle
            }// s
            return pRet;
        }//FindDatasetAsync
        public async Task MaintainsDatasetAsync(DBDataset p,bool bCommit = true)
        {
            if (p == null)
            {
                throw new ArgumentNullException();
            }
            string s1 = p.Sigle;
            string s2 = p.Name;
            if (string.IsNullOrEmpty(s1) || string.IsNullOrEmpty(s2))
            {
                throw new InvalidOperationException();
            }
            string sigle = s1.Trim().ToUpper();
            string name = s2.Trim();
            if (string.IsNullOrEmpty(sigle) || string.IsNullOrEmpty(name))
            {
                throw new InvalidOperationException();
            }
            DBDataset pOldId = null;
            if (p.DBDatasetId != 0)
            {
                pOldId = await m_ctx.DBDatasets.FindAsync(p.DBDatasetId);
            }
            DBDataset pOldSigle = null;
            var l1 = await m_ctx.DBDatasets.Where(x => x.Sigle == sigle).ToListAsync();
            if (l1.Count() > 0)
            {
                pOldSigle = l1.First();
            }
            if ((pOldId != null) && (pOldSigle != null) &&
                (pOldSigle.DBDatasetId != pOldId.DBDatasetId))
            {
                throw new InvalidOperationException();
            }
            DBDataset pp = null;
            if (pOldId != null)
            {
                pp = pOldId;
            }
            else if (pOldSigle != null)
            {
                pp = pOldSigle;
            }
            if (pp == null)
            {
                pp = new DBDataset();
                m_ctx.DBDatasets.Add(pp);
            }
            pp.Sigle = sigle;
            pp.Name = name;
            pp.Description = p.Description;
            pp.Status = p.Status;
            pp.RemoteId = p.RemoteId;
            pp.RemoteVersion = p.RemoteVersion;
            if (bCommit)
            {
                await m_ctx.SaveChangesAsync();
            }
        }//MaintainsDatasetAsync
        public async Task RemoveDatasetAsync(DBDataset p,bool bCommit = false)
        {
            if (p == null)
            {
                throw new ArgumentNullException();
            }
            DBDataset pRet = null;
            int nId = p.DBDatasetId;
            if (nId != 0)
            {
                pRet = await m_ctx.DBDatasets.FindAsync(nId);
            }
            if (pRet == null)
            {
                string s = p.Sigle;
                if (!string.IsNullOrEmpty(s))
                {
                    string sigle = s.Trim().ToUpper();
                    if (!string.IsNullOrEmpty(sigle))
                    {
                        pRet = await m_ctx.DBDatasets.Where(x => x.Sigle == sigle).SingleAsync();
                    }// sigle
                }// s
            }
            if (pRet != null)
            {
                m_ctx.DBDatasets.Remove(pRet);
                if (bCommit)
                {
                    await m_ctx.SaveChangesAsync();
                }
            }
        }//RemoveDatasetAsync
        //
        public async Task<int> GetDatasetVariablesCountAsync(DBDataset pSet)
        {
            if (pSet == null)
            {
                throw new ArgumentNullException();
            }
            DBDataset pp = await this.FindDatasetAsync(pSet);
            if (pp == null)
            {
                throw new ArgumentException();
            }
            var l = await m_ctx.DBVariables.Where(x => x.DBDatasetId == pp.DBDatasetId).ToListAsync();
            return l.Count();
        }//GetDatasetVariablesCountAsync
        public async Task<IList<DBVariable>> GetDatasetVariablesAsync(DBDataset pSet, int offset = 0, int count = 0)
        {
            if (pSet == null)
            {
                throw new ArgumentNullException();
            }
            DBDataset pp = await this.FindDatasetAsync(pSet);
            if (pp == null)
            {
                throw new ArgumentException();
            }
            if (offset < 0)
            {
                offset = 0;
            }
            if (count < 1)
            {
                count = MAX_COUNT;
            }
            IList<DBVariable> oRet = await m_ctx.DBVariables.Where(x => x.DBDatasetId == pSet.DBDatasetId).Skip(offset).Take(count).
          OrderBy(y => y.Sigle).ToListAsync();
            return oRet;
        }//GetDatasetVariablesAsync
        public async Task<DBVariable> FindVariableAsync(DBVariable p)
        {
            if (p == null)
            {
                throw new ArgumentNullException();
            }
            DBVariable pRet = null;
            int nId = p.DBVariableId;
            if (nId != 0)
            {
                pRet = await m_ctx.DBVariables.FindAsync(nId);
                if (pRet != null)
                {
                    return pRet;
                }
            }
            string s = p.Sigle;
            int nSetId = p.DBDatasetId;
            if ((nSetId != 0) && (!string.IsNullOrEmpty(s)))
            {
                string sigle = s.Trim().ToUpper();
                if (!string.IsNullOrEmpty(sigle))
                {
                    pRet = await m_ctx.DBVariables.Where(x =>(x.DBDatasetId == nSetId) && (x.Sigle == sigle)).SingleAsync();
                }// sigle
            }// s
            return pRet;
        }// FindVariableAsync
        public async Task MaintainsVariableAsync(DBVariable p,bool bCommit = true)
        {
            if (p == null)
            {
                throw new ArgumentNullException();
            }
            string s1 = p.Sigle;
            string s2 = p.Name;
            if (string.IsNullOrEmpty(s1) || string.IsNullOrEmpty(s2))
            {
                throw new InvalidOperationException();
            }
            string sigle = s1.Trim().ToUpper();
            string name = s2.Trim();
            if (string.IsNullOrEmpty(sigle) || string.IsNullOrEmpty(name))
            {
                throw new InvalidOperationException();
            }
            DBVariable pOldId = null;
            if (p.DBVariableId != 0)
            {
                pOldId = await m_ctx.DBVariables.FindAsync(p.DBVariableId);
            }
            DBVariable pOldSigle = null;
            var l1 = await m_ctx.DBVariables.Where(x =>(x.DBDatasetId == p.DBDatasetId) && (x.Sigle == sigle)).ToListAsync();
            if (l1.Count() > 0)
            {
                pOldSigle = l1.First();
            }
            if ((pOldId != null) && (pOldSigle != null) &&
                (pOldSigle.DBVariableId != pOldId.DBVariableId))
            {
                throw new InvalidOperationException();
            }
            DBVariable pp = null;
            if (pOldId != null)
            {
                pp = pOldId;
            }
            else if (pOldSigle != null)
            {
                pp = pOldSigle;
            }
            if (pp == null)
            {
                DBDataset xSet = await m_ctx.DBDatasets.FindAsync(p.DBDatasetId);
                if (xSet == null)
                {
                    throw new InvalidOperationException();
                }
                pp = new DBVariable();
                pp.DBDataset = xSet;
                m_ctx.DBVariables.Add(pp);
            }
            pp.Sigle = sigle;
            pp.Name = name;
            pp.Description = p.Description;
            pp.Status = p.Status;
            pp.VariableKind = p.VariableKind;
            pp.VariableType = p.VariableType;
            pp.RemoteId = p.RemoteId;
            pp.RemoteVersion = p.RemoteVersion;
            if (bCommit)
            {
                await m_ctx.SaveChangesAsync();
            }
        }//MaintainsVariableAsync
        public async Task RemoveVariableAsync(DBVariable p,bool bCommit = true)
        {
            if (p == null)
            {
                throw new ArgumentNullException();
            }
            DBVariable pRet = await FindVariableAsync(p);
            if (pRet != null)
            {
                m_ctx.DBVariables.Remove(pRet);
                if (bCommit)
                {
                    await m_ctx.SaveChangesAsync();
                }
            }
        }//RemoveVariableAsync
        public async Task MaintainsVariablesAsync(IList<DBVariable> oVec, bool bDelete = false, bool bCommit = true)
        {
            if (oVec == null)
            {
                throw new ArgumentNullException();
            }
            bool done = false;
            foreach (var p in oVec)
            {
                if (p != null)
                {
                    if (bDelete)
                    {
                        await this.RemoveVariableAsync(p, false);
                        done = true;
                    } else
                    {
                        await this.MaintainsVariableAsync(p, false);
                        done = true;
                    }
                }// p
            }// p
            if (done && bCommit)
            {
                await m_ctx.SaveChangesAsync();
            }
        }//MaintainsVariablesAsync
        //
        public async Task<int> GetDatasetIndivsCountAsync(DBDataset pSet)
        {
            if (pSet == null)
            {
                throw new ArgumentNullException();
            }
            DBDataset pp = await this.FindDatasetAsync(pSet);
            if (pp == null)
            {
                throw new ArgumentException();
            }
            var l = await m_ctx.DBIndivs.Where(x => x.DBDatasetId == pp.DBDatasetId).ToListAsync();
            return l.Count();
        }//GetDatasetIndivsCountAsync
        public async Task<IList<DBIndiv>> GetDatasetIndivsAsync(DBDataset pSet, int offset = 0, int count = 0)
        {
            if (pSet == null)
            {
                throw new ArgumentNullException();
            }
            DBDataset pp = await this.FindDatasetAsync(pSet);
            if (pp == null)
            {
                throw new ArgumentException();
            }
            if (offset < 0)
            {
                offset = 0;
            }
            if (count < 1)
            {
                count = MAX_COUNT;
            }
            IList<DBIndiv> oRet = await m_ctx.DBIndivs.Where(x => x.DBDatasetId == pSet.DBDatasetId).Skip(offset).Take(count).
          OrderBy(y => y.Sigle).ToListAsync();
            return oRet;
        }//GetDatasetIndivsAsync
        public async Task<DBIndiv> FindIndivAsync(DBIndiv p)
        {
            if (p == null)
            {
                throw new ArgumentNullException();
            }
            DBIndiv pRet = null;
            int nId = p.DBIndivId;
            if (nId != 0)
            {
                pRet = await m_ctx.DBIndivs.FindAsync(nId);
                if (pRet != null)
                {
                    return pRet;
                }
            }
            string s = p.Sigle;
            int nSetId = p.DBDatasetId;
            if ((nSetId != 0) && (!string.IsNullOrEmpty(s)))
            {
                string sigle = s.Trim().ToUpper();
                if (!string.IsNullOrEmpty(sigle))
                {
                    pRet = await m_ctx.DBIndivs.Where(x => (x.DBDatasetId == nSetId) && (x.Sigle == sigle)).SingleAsync();
                }// sigle
            }// s
            return pRet;
        }// FindIndivAsync
        public async Task MaintainsIndivAsync(DBIndiv p, bool bCommit = true)
        {
            if (p == null)
            {
                throw new ArgumentNullException();
            }
            string s1 = p.Sigle;
            string s2 = p.Name;
            if (string.IsNullOrEmpty(s1) || string.IsNullOrEmpty(s2))
            {
                throw new InvalidOperationException();
            }
            string sigle = s1.Trim().ToUpper();
            string name = s2.Trim();
            if (string.IsNullOrEmpty(sigle) || string.IsNullOrEmpty(name))
            {
                throw new InvalidOperationException();
            }
            DBIndiv pOldId = null;
            if (p.DBIndivId != 0)
            {
                pOldId = await m_ctx.DBIndivs.FindAsync(p.DBIndivId);
            }
            DBIndiv pOldSigle = null;
            var l1 = await m_ctx.DBIndivs.Where(x => (x.DBDatasetId == p.DBDatasetId) && (x.Sigle == sigle)).ToListAsync();
            if (l1.Count() > 0)
            {
                pOldSigle = l1.First();
            }
            if ((pOldId != null) && (pOldSigle != null) &&
                (pOldSigle.DBIndivId != pOldId.DBIndivId))
            {
                throw new InvalidOperationException();
            }
            DBIndiv pp = null;
            if (pOldId != null)
            {
                pp = pOldId;
            }
            else if (pOldSigle != null)
            {
                pp = pOldSigle;
            }
            if (pp == null)
            {
                DBDataset xSet = await m_ctx.DBDatasets.FindAsync(p.DBDatasetId);
                if (xSet == null)
                {
                    throw new InvalidOperationException();
                }
                pp = new DBIndiv();
                pp.DBDataset = xSet;
                m_ctx.DBIndivs.Add(pp);
            }
            pp.Sigle = sigle;
            pp.Name = name;
            pp.Description = p.Description;
            pp.Status = p.Status;
            pp.RemoteId = p.RemoteId;
            pp.RemoteVersion = p.RemoteVersion;
            if (bCommit)
            {
                await m_ctx.SaveChangesAsync();
            }
        }//MaintainsIndiveAsync
        public async Task RemoveIndivAsync(DBIndiv p, bool bCommit = true)
        {
            if (p == null)
            {
                throw new ArgumentNullException();
            }
            DBIndiv pRet = await FindIndivAsync(p);
            if (pRet != null)
            {
                m_ctx.DBIndivs.Remove(pRet);
                if (bCommit)
                {
                    await m_ctx.SaveChangesAsync();
                }
            }
        }//RemoveIndivAsync
        public async Task MaintainsIndivsAsync(IList<DBIndiv> oVec, bool bDelete = false, bool bCommit = true)
        {
            if (oVec == null)
            {
                throw new ArgumentNullException();
            }
            bool done = false;
            foreach (var p in oVec)
            {
                if (p != null)
                {
                    if (bDelete)
                    {
                        await this.RemoveIndivAsync(p, false);
                        done = true;
                    }
                    else
                    {
                        await this.MaintainsIndivAsync(p, false);
                        done = true;
                    }
                }// p
            }// p
            if (done && bCommit)
            {
                await m_ctx.SaveChangesAsync();
            }
        }//MaintainsIndivsAsync
        //
        public async Task<int> GetDatasetValuesCountAsync(DBDataset pSet)
        {
            if (pSet == null)
            {
                throw new ArgumentNullException();
            }
            DBDataset pp = await this.FindDatasetAsync(pSet);
            if (pp == null)
            {
                throw new ArgumentException();
            }
            int nId = pp.DBDatasetId;
            var l = await m_ctx.DBValues.Where(x =>
            (x.DBVariable.DBDatasetId == nId)).ToListAsync();
            return l.Count();
        }//GetDatasetValuesCountAsync
        public async Task<IList<DBValue>> GetDatasetValuesAsync(DBDataset pSet,int offset = 0, int count = 0)
        {
            if (pSet == null)
            {
                throw new ArgumentNullException();
            }
            DBDataset pp = await this.FindDatasetAsync(pSet);
            if (pp == null)
            {
                throw new ArgumentException();
            }
            if (offset < 0)
            {
                offset = 0;
            }
            if (count < 1)
            {
                count = MAX_COUNT;
            }
            IList<DBValue> oRet = await m_ctx.DBValues.Where(x =>
            x.DBVariable.DBDatasetId == pp.DBDatasetId).Skip(offset).Take(count).
            OrderBy(y => y.DBIndiv.Sigle).
            OrderBy(z => z.DBVariable.Sigle).ToListAsync();
            return oRet;
        }//GetDatasetValuesAsync
        public async Task<int> GetVariableValuesCountAsync(DBVariable pVar)
        {
            if (pVar == null)
            {
                throw new ArgumentNullException();
            }
            DBVariable pp = await this.FindVariableAsync(pVar);
            if (pp == null)
            {
                throw new ArgumentException();
            }
            int nId = pp.DBVariableId;
            var l = await m_ctx.DBValues.Where(x =>
            (x.DBVariable.DBVariableId == nId)).ToListAsync();
            return l.Count();
        }//GetVariableValuesCountAsync
        public async Task<IList<DBValue>> GetVariableValuesAsync(DBVariable pVar, int offset = 0, int count = 0)
        {
            if (pVar == null)
            {
                throw new ArgumentNullException();
            }
            DBVariable pp = await this.FindVariableAsync(pVar);
            if (pp == null)
            {
                throw new ArgumentException();
            }
            if (offset < 0)
            {
                offset = 0;
            }
            if (count < 1)
            {
                count = MAX_COUNT;
            }
            IList<DBValue> oRet = await m_ctx.DBValues.Where(x =>
            x.DBVariable.DBVariableId == pp.DBVariableId).Skip(offset).Take(count).
            OrderBy(y => y.DBIndiv.Sigle).ToListAsync();
            return oRet;
        }//GetVaribleValuesAsync
        public async Task<int> GetIndivValuesCountAsync(DBIndiv pInd)
        {
            if (pInd == null)
            {
                throw new ArgumentNullException();
            }
            DBIndiv pp = await this.FindIndivAsync(pInd);
            if (pp == null)
            {
                throw new ArgumentException();
            }
            int nId = pp.DBIndivId;
            var l = await m_ctx.DBValues.Where(x =>
            (x.DBIndiv.DBIndivId == nId)).ToListAsync();
            return l.Count();
        }//GetIndivValuesCountAsync
        public async Task<IList<DBValue>> GetIndivValuesAsync(DBIndiv pInd, int offset = 0, int count = 0)
        {
            if (pInd == null)
            {
                throw new ArgumentNullException();
            }
            DBIndiv pp = await this.FindIndivAsync(pInd);
            if (pp == null)
            {
                throw new ArgumentException();
            }
            if (offset < 0)
            {
                offset = 0;
            }
            if (count < 1)
            {
                count = MAX_COUNT;
            }
            IList<DBValue> oRet = await m_ctx.DBValues.Where(x =>
            x.DBIndiv.DBIndivId == pp.DBIndivId).Skip(offset).Take(count).
            OrderBy(y => y.DBVariable.Sigle).ToListAsync();
            return oRet;
        }//GetIndivValuesAsync
        public async Task<DBValue> FindValueAsync(DBValue p)
        {
            if (p == null)
            {
                throw new ArgumentNullException();
            }
            DBValue pRet = null;
            int nId = p.DBValueId;
            if (nId != 0)
            {
                pRet = await m_ctx.DBValues.FindAsync(nId);
                if (pRet != null)
                {
                    return pRet;
                }
            }
            int nVarId = p.DBVariableId;
            int nIndId = p.DBIndivId;
            if ((nVarId != 0) && (nIndId != 0))
            {
                var l = await m_ctx.DBValues.Where(x =>
                (x.DBVariableId == nVarId) && (x.DBIndivId == nIndId)).ToListAsync();
                if (l.Count() > 0)
                {
                    pRet = l.First();
                }
            }// s
            return pRet;
        }// FindValueAsync
        public async Task MaintainsValueAsync(DBValue p, bool bCommit = true)
        {
            if (p == null)
            {
                throw new ArgumentNullException();
            }
            DBValue pp = await this.FindValueAsync(p);
            if (pp == null)
            {
                int nVarId = p.DBVariableId;
                int nIndId = p.DBIndivId;
                if ((nVarId == 0) || (nIndId == 0))
                {
                    throw new InvalidOperationException();
                }
                DBVariable pVar = await m_ctx.DBVariables.FindAsync(nVarId);
                DBIndiv pInd = await m_ctx.DBIndivs.FindAsync(nIndId);
                if ((pVar == null) || (pInd == null))
                {
                    throw new InvalidOperationException();
                }
                pp = new DBValue();
                pp.DBIndiv = pInd;
                pp.DBVariable = pVar;
                m_ctx.DBValues.Add(pp);
            }
            pp.VariableType = p.VariableType;
            pp.StringValue = p.StringValue;
            pp.Status = p.Status;
            pp.Description = p.Description;
            pp.RemoteId = p.RemoteId;
            pp.RemoteVersion = p.RemoteVersion;
            if (bCommit)
            {
                await m_ctx.SaveChangesAsync();
            }
        }//MaintainsValueAsync
        public async Task RemoveValueAsync(DBValue p, bool bCommit = true)
        {
            if (p == null)
            {
                throw new ArgumentNullException();
            }
            DBValue pRet = await FindValueAsync(p);
            if (pRet != null)
            {
                m_ctx.DBValues.Remove(pRet);
                if (bCommit)
                {
                    await m_ctx.SaveChangesAsync();
                }
            }
        }//RemoveValueAsync
        public async Task MaintainsValuesAsync(IList<DBValue> oVec, bool bDelete = false, bool bCommit = true)
        {
            if (oVec == null)
            {
                throw new ArgumentNullException();
            }
            bool done = false;
            foreach (var p in oVec)
            {
                if (p != null)
                {
                    if (bDelete)
                    {
                        await RemoveValueAsync(p, false);
                        done = true;
                    }
                    else
                    {
                        await MaintainsValueAsync(p, false);
                        done = true;
                    }
                }// p
            }// p
            if (done && bCommit)
            {
                await m_ctx.SaveChangesAsync();
            }
        }//MaintainsValuesAsync
    }// class LocalDataManager
}
