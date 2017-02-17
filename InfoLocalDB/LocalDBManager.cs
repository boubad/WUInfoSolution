using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.EntityFrameworkCore;
//
namespace InfoLocalDB
{
    public class LocalDBManager : IDisposable
    {
        private readonly int MAX_COUNT = 20;
        private LocalDBContext m_ctx;

        public void Dispose()
        {
            if (m_ctx != null)
            {
                ((IDisposable)m_ctx).Dispose();
                m_ctx = null;
            }
        }
        public LocalDBManager()
        {
            m_ctx = new LocalDBContext();
        }
        public void Migrate()
        {
            m_ctx.Database.Migrate();
        }
        public async Task<int> GetDatasetsCountAsync()
        {
            var pp = await m_ctx.LocalDatasets.ToListAsync();
            return pp.Count();
        }//GetDatasetsCountAsync
        public async Task<IList<LocalDataset>> GetDatasetsAsync(int offset = 0, int count = 0)
        {
            if (offset < 0)
            {
                offset = 0;
            }
            if (count < 1)
            {
                count = MAX_COUNT;
            }
            IList<LocalDataset> oRet = await m_ctx.
                LocalDatasets.Skip(0).Take(count).
                OrderBy(x => x.Sigle).ToListAsync();
            return oRet;
        }//GetDatasetsAsync
        public async Task<LocalDataset> FindDatasetAsync(LocalDataset p)
        {
            if (p == null)
            {
                throw new ArgumentNullException();
            }
            LocalDataset pRet = null;
            int nId = p.LocalDatasetId;
            if (nId != 0)
            {
                pRet = await m_ctx.LocalDatasets.FindAsync(nId);
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
                    pRet = await m_ctx.LocalDatasets.Where(x => x.Sigle == sigle).SingleAsync();
                }// sigle
            }// s
            return pRet;
        }//FindDatasetAsync
        public async Task MaintainsDatasetAsync(LocalDataset p, bool bCommit = true)
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
            LocalDataset pOldId = null;
            if (p.LocalDatasetId != 0)
            {
                pOldId = await m_ctx.LocalDatasets.FindAsync(p.LocalDatasetId);
            }
            LocalDataset pOldSigle = null;
            var l1 = await m_ctx.LocalDatasets.Where(x => x.Sigle == sigle).ToListAsync();
            if (l1.Count() > 0)
            {
                pOldSigle = l1.First();
            }
            if ((pOldId != null) && (pOldSigle != null) &&
                (pOldSigle.LocalDatasetId != pOldId.LocalDatasetId))
            {
                throw new InvalidOperationException();
            }
            LocalDataset pp = null;
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
                pp = new LocalDataset();
                m_ctx.LocalDatasets.Add(pp);
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
        public async Task RemoveDatasetAsync(LocalDataset p, bool bCommit = false)
        {
            if (p == null)
            {
                throw new ArgumentNullException();
            }
            LocalDataset pRet = null;
            int nId = p.LocalDatasetId;
            if (nId != 0)
            {
                pRet = await m_ctx.LocalDatasets.FindAsync(nId);
            }
            if (pRet == null)
            {
                string s = p.Sigle;
                if (!string.IsNullOrEmpty(s))
                {
                    string sigle = s.Trim().ToUpper();
                    if (!string.IsNullOrEmpty(sigle))
                    {
                        pRet = await m_ctx.LocalDatasets.Where(x => x.Sigle == sigle).SingleAsync();
                    }// sigle
                }// s
            }
            if (pRet != null)
            {
                m_ctx.LocalDatasets.Remove(pRet);
                if (bCommit)
                {
                    await m_ctx.SaveChangesAsync();
                }
            }
        }//RemoveDatasetAsync
        public async Task<LocalDataset> FindDatasetBySigleAsync(string sigle)
        {
            LocalDataset pRet = null;
            if (!string.IsNullOrEmpty(sigle))
            {
                string ss = sigle.Trim().ToUpper();
                if (!string.IsNullOrEmpty(ss))
                {
                    var l = await m_ctx.LocalDatasets.Where(x => x.Sigle == ss).ToListAsync();
                    if (l.Count() > 0)
                    {
                        pRet = l.First();
                    }
                }// ss
            }// sigle
            return pRet;
        }//FindDatasetBySigleAsync
        //
        public async Task<int> GetDatasetVariablesCountAsync(LocalDataset pSet)
        {
            if (pSet == null)
            {
                throw new ArgumentNullException();
            }
            LocalDataset pp = await this.FindDatasetAsync(pSet);
            if (pp == null)
            {
                throw new ArgumentException();
            }
            var l = await m_ctx.LocalVariables.Where(x => x.LocalDatasetId == pp.LocalDatasetId).ToListAsync();
            return l.Count();
        }//GetDatasetVariablesCountAsync
        public async Task<IList<LocalVariable>> GetDatasetVariablesAsync(LocalDataset pSet, int offset = 0, int count = 0)
        {
            if (pSet == null)
            {
                throw new ArgumentNullException();
            }
            LocalDataset pp = await this.FindDatasetAsync(pSet);
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
            IList<LocalVariable> oRet = await m_ctx.LocalVariables.Where(x => x.LocalDatasetId == pSet.LocalDatasetId).Skip(offset).Take(count).
          OrderBy(y => y.Sigle).ToListAsync();
            return oRet;
        }//GetDatasetVariablesAsync
        public async Task<LocalVariable> FindVariableAsync(LocalVariable p)
        {
            if (p == null)
            {
                throw new ArgumentNullException();
            }
            LocalVariable pRet = null;
            int nId = p.LocalVariableId;
            if (nId != 0)
            {
                pRet = await m_ctx.LocalVariables.FindAsync(nId);
                if (pRet != null)
                {
                    return pRet;
                }
            }
            string s = p.Sigle;
            int nSetId = p.LocalDatasetId;
            if ((nSetId != 0) && (!string.IsNullOrEmpty(s)))
            {
                string sigle = s.Trim().ToUpper();
                if (!string.IsNullOrEmpty(sigle))
                {
                    pRet = await m_ctx.LocalVariables.Where(x => (x.LocalDatasetId == nSetId) && (x.Sigle == sigle)).SingleAsync();
                }// sigle
            }// s
            return pRet;
        }// FindVariableAsync
        public async Task<LocalVariable> FindVariableBySiglesAsync(string setsigle, string sigle)
        {
            LocalVariable pRet = null;
            if ((!string.IsNullOrEmpty(setsigle)) && (!string.IsNullOrEmpty(sigle)))
            {
                string s1 = setsigle.Trim().ToUpper();
                string s2 = sigle.Trim().ToUpper();
                if ((!string.IsNullOrEmpty(s1)) && (!string.IsNullOrEmpty(s2)))
                {
                    var l = await m_ctx.LocalVariables.
                        Where(x => (x.Sigle == s2) && (x.LocalDataset.Sigle == s1)).ToListAsync();
                    if (l.Count() > 0)
                    {
                        pRet = l.First();
                    }
                }// s1, s2
            }// sigles
            return pRet;
        }//FindVariableBySiglesAsync
        public async Task MaintainsVariableAsync(LocalVariable p, bool bCommit = true)
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
            LocalVariable pOldId = null;
            if (p.LocalVariableId != 0)
            {
                pOldId = await m_ctx.LocalVariables.FindAsync(p.LocalVariableId);
            }
            LocalVariable pOldSigle = null;
            var l1 = await m_ctx.LocalVariables.Where(x => (x.LocalDatasetId == p.LocalDatasetId) && (x.Sigle == sigle)).ToListAsync();
            if (l1.Count() > 0)
            {
                pOldSigle = l1.First();
            }
            if ((pOldId != null) && (pOldSigle != null) &&
                (pOldSigle.LocalVariableId != pOldId.LocalVariableId))
            {
                throw new InvalidOperationException();
            }
            LocalVariable pp = null;
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
                LocalDataset xSet = await m_ctx.LocalDatasets.FindAsync(p.LocalDatasetId);
                if (xSet == null)
                {
                    throw new InvalidOperationException();
                }
                pp = new LocalVariable();
                pp.LocalDataset = xSet;
                m_ctx.LocalVariables.Add(pp);
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
        public async Task RemoveVariableAsync(LocalVariable p, bool bCommit = true)
        {
            if (p == null)
            {
                throw new ArgumentNullException();
            }
            LocalVariable pRet = await FindVariableAsync(p);
            if (pRet != null)
            {
                m_ctx.LocalVariables.Remove(pRet);
                if (bCommit)
                {
                    await m_ctx.SaveChangesAsync();
                }
            }
        }//RemoveVariableAsync
        public async Task MaintainsVariablesAsync(IList<LocalVariable> oVec, bool bDelete = false, bool bCommit = true)
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
                    }
                    else
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
        public async Task<int> GetDatasetIndivsCountAsync(LocalDataset pSet)
        {
            if (pSet == null)
            {
                throw new ArgumentNullException();
            }
            LocalDataset pp = await this.FindDatasetAsync(pSet);
            if (pp == null)
            {
                throw new ArgumentException();
            }
            var l = await m_ctx.LocalIndivs.Where(x => x.LocalDatasetId == pp.LocalDatasetId).ToListAsync();
            return l.Count();
        }//GetDatasetIndivsCountAsync
        public async Task<IList<LocalIndiv>> GetDatasetIndivsAsync(LocalDataset pSet, int offset = 0, int count = 0)
        {
            if (pSet == null)
            {
                throw new ArgumentNullException();
            }
            LocalDataset pp = await this.FindDatasetAsync(pSet);
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
            IList<LocalIndiv> oRet = await m_ctx.LocalIndivs.Where(x => x.LocalDatasetId == pSet.LocalDatasetId).Skip(offset).Take(count).
          OrderBy(y => y.Sigle).ToListAsync();
            return oRet;
        }//GetDatasetIndivsAsync
        public async Task<LocalIndiv> FindIndivAsync(LocalIndiv p)
        {
            if (p == null)
            {
                throw new ArgumentNullException();
            }
            LocalIndiv pRet = null;
            int nId = p.LocalIndivId;
            if (nId != 0)
            {
                pRet = await m_ctx.LocalIndivs.FindAsync(nId);
                if (pRet != null)
                {
                    return pRet;
                }
            }
            string s = p.Sigle;
            int nSetId = p.LocalDatasetId;
            if ((nSetId != 0) && (!string.IsNullOrEmpty(s)))
            {
                string sigle = s.Trim().ToUpper();
                if (!string.IsNullOrEmpty(sigle))
                {
                    pRet = await m_ctx.LocalIndivs.Where(x => (x.LocalDatasetId == nSetId) && (x.Sigle == sigle)).SingleAsync();
                }// sigle
            }// s
            return pRet;
        }// FindIndivAsync
        public async Task<LocalIndiv> FindIndivBySiglesAsync(string setsigle, string sigle)
        {
            LocalIndiv pRet = null;
            if ((!string.IsNullOrEmpty(setsigle)) && (!string.IsNullOrEmpty(sigle)))
            {
                string s1 = setsigle.Trim().ToUpper();
                string s2 = sigle.Trim().ToUpper();
                if ((!string.IsNullOrEmpty(s1)) && (!string.IsNullOrEmpty(s2)))
                {
                    var l = await m_ctx.LocalIndivs.
                        Where(x => (x.Sigle == s2) && (x.LocalDataset.Sigle == s1)).ToListAsync();
                    if (l.Count() > 0)
                    {
                        pRet = l.First();
                    }
                }// s1, s2
            }// sigles
            return pRet;
        }//FindIndivBySiglesAsync
        public async Task MaintainsIndivAsync(LocalIndiv p, bool bCommit = true)
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
            LocalIndiv pOldId = null;
            if (p.LocalIndivId != 0)
            {
                pOldId = await m_ctx.LocalIndivs.FindAsync(p.LocalIndivId);
            }
            LocalIndiv pOldSigle = null;
            var l1 = await m_ctx.LocalIndivs.Where(x => (x.LocalDatasetId == p.LocalDatasetId) && (x.Sigle == sigle)).ToListAsync();
            if (l1.Count() > 0)
            {
                pOldSigle = l1.First();
            }
            if ((pOldId != null) && (pOldSigle != null) &&
                (pOldSigle.LocalIndivId != pOldId.LocalIndivId))
            {
                throw new InvalidOperationException();
            }
            LocalIndiv pp = null;
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
                LocalDataset xSet = await m_ctx.LocalDatasets.FindAsync(p.LocalDatasetId);
                if (xSet == null)
                {
                    throw new InvalidOperationException();
                }
                pp = new LocalIndiv();
                pp.LocalDataset = xSet;
                m_ctx.LocalIndivs.Add(pp);
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
        public async Task RemoveIndivAsync(LocalIndiv p, bool bCommit = true)
        {
            if (p == null)
            {
                throw new ArgumentNullException();
            }
            LocalIndiv pRet = await FindIndivAsync(p);
            if (pRet != null)
            {
                m_ctx.LocalIndivs.Remove(pRet);
                if (bCommit)
                {
                    await m_ctx.SaveChangesAsync();
                }
            }
        }//RemoveIndivAsync
        public async Task MaintainsIndivsAsync(IList<LocalIndiv> oVec, bool bDelete = false, bool bCommit = true)
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
        public async Task<int> GetDatasetValuesCountAsync(LocalDataset pSet)
        {
            if (pSet == null)
            {
                throw new ArgumentNullException();
            }
            LocalDataset pp = await this.FindDatasetAsync(pSet);
            if (pp == null)
            {
                throw new ArgumentException();
            }
            int nId = pp.LocalDatasetId;
            var l = await m_ctx.LocalValues.Where(x =>
            (x.LocalVariable.LocalDatasetId == nId)).ToListAsync();
            return l.Count();
        }//GetDatasetValuesCountAsync
        public async Task<IList<LocalValue>> GetDatasetValuesAsync(LocalDataset pSet, int offset = 0, int count = 0)
        {
            if (pSet == null)
            {
                throw new ArgumentNullException();
            }
            LocalDataset pp = await this.FindDatasetAsync(pSet);
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
            IList<LocalValue> oRet = await m_ctx.LocalValues.Where(x =>
            x.LocalVariable.LocalDatasetId == pp.LocalDatasetId).Skip(offset).Take(count).
            OrderBy(y => y.LocalIndiv.Sigle).
            OrderBy(z => z.LocalVariable.Sigle).ToListAsync();
            return oRet;
        }//GetDatasetValuesAsync
        public async Task<int> GetVariableValuesCountAsync(LocalVariable pVar)
        {
            if (pVar == null)
            {
                throw new ArgumentNullException();
            }
            LocalVariable pp = await this.FindVariableAsync(pVar);
            if (pp == null)
            {
                throw new ArgumentException();
            }
            int nId = pp.LocalVariableId;
            var l = await m_ctx.LocalValues.Where(x =>
            (x.LocalVariable.LocalVariableId == nId)).ToListAsync();
            return l.Count();
        }//GetVariableValuesCountAsync
        public async Task<IList<LocalValue>> GetVariableValuesAsync(LocalVariable pVar, int offset = 0, int count = 0)
        {
            if (pVar == null)
            {
                throw new ArgumentNullException();
            }
            LocalVariable pp = await this.FindVariableAsync(pVar);
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
            IList<LocalValue> oRet = await m_ctx.LocalValues.Where(x =>
            x.LocalVariable.LocalVariableId == pp.LocalVariableId).Skip(offset).Take(count).
            OrderBy(y => y.LocalIndiv.Sigle).ToListAsync();
            return oRet;
        }//GetVaribleValuesAsync
        public async Task<int> GetIndivValuesCountAsync(LocalIndiv pInd)
        {
            if (pInd == null)
            {
                throw new ArgumentNullException();
            }
            LocalIndiv pp = await this.FindIndivAsync(pInd);
            if (pp == null)
            {
                throw new ArgumentException();
            }
            int nId = pp.LocalIndivId;
            var l = await m_ctx.LocalValues.Where(x =>
            (x.LocalIndiv.LocalIndivId == nId)).ToListAsync();
            return l.Count();
        }//GetIndivValuesCountAsync
        public async Task<IList<LocalValue>> GetIndivValuesAsync(LocalIndiv pInd, int offset = 0, int count = 0)
        {
            if (pInd == null)
            {
                throw new ArgumentNullException();
            }
            LocalIndiv pp = await this.FindIndivAsync(pInd);
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
            IList<LocalValue> oRet = await m_ctx.LocalValues.Where(x =>
            x.LocalIndiv.LocalIndivId == pp.LocalIndivId).Skip(offset).Take(count).
            OrderBy(y => y.LocalVariable.Sigle).ToListAsync();
            return oRet;
        }//GetIndivValuesAsync
        public async Task<LocalValue> FindValueAsync(LocalValue p)
        {
            if (p == null)
            {
                throw new ArgumentNullException();
            }
            LocalValue pRet = null;
            int nId = p.LocalValueId;
            if (nId != 0)
            {
                pRet = await m_ctx.LocalValues.FindAsync(nId);
                if (pRet != null)
                {
                    return pRet;
                }
            }
            int nVarId = p.LocalVariableId;
            int nIndId = p.LocalIndivId;
            if ((nVarId != 0) && (nIndId != 0))
            {
                var l = await m_ctx.LocalValues.Where(x =>
                (x.LocalVariableId == nVarId) && (x.LocalIndivId == nIndId)).ToListAsync();
                if (l.Count() > 0)
                {
                    pRet = l.First();
                }
            }// s
            return pRet;
        }// FindValueAsync
        public async Task<LocalValue> FindValueBySiglesAsync(string setsigle, string indsigle, string varsigle)
        {
            LocalValue pRet = null;
            if ((!string.IsNullOrEmpty(setsigle)) && (!string.IsNullOrEmpty(varsigle)) &&
                (!string.IsNullOrEmpty(indsigle)))
            {
                string s1 = setsigle.Trim().ToUpper();
                string s2 = indsigle.Trim().ToUpper();
                string s3 = varsigle.Trim().ToUpper();
                if ((!string.IsNullOrEmpty(s1)) && (!string.IsNullOrEmpty(s2)) &&
                    (!string.IsNullOrEmpty(s3)))
                {
                    var l = await m_ctx.LocalValues.
                        Where(x => (x.LocalIndiv.Sigle == s2) &&
                        (x.LocalVariable.Sigle == s3) &&
                        (x.LocalVariable.LocalDataset.Sigle == s1)).ToListAsync();
                    if (l.Count() > 0)
                    {
                        pRet = l.First();
                    }
                }// s1, s2
            }// sigles
            return pRet;
        }//FindValuesBySiglesAsync
        public async Task MaintainsValueAsync(LocalValue p, bool bCommit = true)
        {
            if (p == null)
            {
                throw new ArgumentNullException();
            }
            LocalValue pp = await this.FindValueAsync(p);
            if (pp == null)
            {
                int nVarId = p.LocalVariableId;
                int nIndId = p.LocalIndivId;
                if ((nVarId == 0) || (nIndId == 0))
                {
                    throw new InvalidOperationException();
                }
                LocalVariable pVar = await m_ctx.LocalVariables.FindAsync(nVarId);
                LocalIndiv pInd = await m_ctx.LocalIndivs.FindAsync(nIndId);
                if ((pVar == null) || (pInd == null))
                {
                    throw new InvalidOperationException();
                }
                pp = new LocalValue();
                pp.LocalIndiv = pInd;
                pp.LocalVariable = pVar;
                m_ctx.LocalValues.Add(pp);
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
        public async Task RemoveValueAsync(LocalValue p, bool bCommit = true)
        {
            if (p == null)
            {
                throw new ArgumentNullException();
            }
            LocalValue pRet = await FindValueAsync(p);
            if (pRet != null)
            {
                m_ctx.LocalValues.Remove(pRet);
                if (bCommit)
                {
                    await m_ctx.SaveChangesAsync();
                }
            }
        }//RemoveValueAsync
        public async Task MaintainsValuesAsync(IList<LocalValue> oVec, bool bDelete = false, bool bCommit = true)
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
    }
}
