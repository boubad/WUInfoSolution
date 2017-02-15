using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.EntityFrameworkCore;

namespace StatDataApp.Data
{
    class LocalDataManager
    {
        public LocalDataManager() { }
        public async Task<int> GetDatasetsCountAsync()
        {
            using (var ctx = new StatDataAppContext())
            {
                var pp = await ctx.DBDatasets.ToListAsync();
                return pp.Count();
            }// ctx
        }//GetDatasetsCountAsync
        public async Task<IList<DBDataset> > GetDatasetsAsync(int offset = 0, int count = 0)
        {
           using (var ctx = new StatDataAppContext())
            {
                if (offset < 0)
                {
                    offset = 0;
                }
                if (count < 1)
                {
                    count = 25;
                }
                IList<DBDataset> oRet = await ctx.DBDatasets.Skip(0).Take(count).ToListAsync();
                return oRet;
            }// ctx
        }//GetDatasetsAsync
        public async Task<DBDataset> FindDatasetAsync(DBDataset p)
        {
            DBDataset pRet = null;
            if (p != null)
            {
                using (var ctx = new StatDataAppContext())
                {
                    int nId = p.DBDatasetId;
                    if (nId != 0)
                    {
                        var l = await ctx.DBDatasets.Where(x => x.DBDatasetId == nId).ToListAsync();
                         if (l.Count() > 0)
                        {
                            pRet = l.First();
                        }
                    }// nId
                    if (pRet == null)
                    {
                        string s = p.Sigle;
                        if (!string.IsNullOrEmpty(s))
                        {
                            string ss = s.Trim().ToUpper();
                            var l = await ctx.DBDatasets.Where(x => x.Sigle == ss).ToListAsync();
                            if (l.Count() > 0)
                            {
                                pRet = l.First();
                            }
                        }// s
                    }// pRet
                }// ctx
            }// p
            return pRet;
        }//FindDatasetAsync
        public async Task<bool> MaintainsDatasetAsync(DBDataset p)
        {
            bool bRet = false;
            if (p != null)
            {
                string s1 = p.Sigle;
                string s2 = p.Name;
                if ((!string.IsNullOrEmpty(s1)) && (!string.IsNullOrEmpty(s2)))
                {
                    string sigle = s1.Trim().ToUpper();
                    string name = s2.Trim();
                    if ((!string.IsNullOrEmpty(sigle)) && (!string.IsNullOrEmpty(name)))
                    {
                        using(var ctx = new StatDataAppContext())
                        {
                            var l = await ctx.DBDatasets.Where(x => x.Sigle == sigle).ToListAsync();
                            if (l.Count() > 0)
                            {
                                DBDataset pp = l.First();
                                pp.Sigle = sigle;
                                pp.Name = name;
                                pp.Description = p.Description;
                                pp.Status = p.Status;
                                pp.RemoteId = p.RemoteId;
                                pp.RemoteVersion = p.RemoteVersion;
                            } else
                            {
                                int nId = p.DBDatasetId;
                                if (nId != 0) {
                                    var lx = await ctx.DBDatasets.Where(x => x.DBDatasetId == nId).ToListAsync();
                                    if (lx.Count() > 0)
                                    {
                                        DBDataset pp = lx.First();
                                        pp.Sigle = sigle;
                                        pp.Name = name;
                                        pp.Description = p.Description;
                                        pp.Status = p.Status;
                                        pp.RemoteId = p.RemoteId;
                                        pp.RemoteVersion = p.RemoteVersion;
                                    } else
                                    {
                                        DBDataset pp = new DBDataset();
                                        pp.Sigle = sigle;
                                        pp.Name = name;
                                        pp.Description = p.Description;
                                        pp.Status = p.Status;
                                        pp.RemoteId = p.RemoteId;
                                        pp.RemoteVersion = p.RemoteVersion;
                                        ctx.Add(pp);
                                    }
                                } else
                                {
                                    DBDataset pp = new DBDataset();
                                    pp.Sigle = sigle;
                                    pp.Name = name;
                                    pp.Description = p.Description;
                                    pp.Status = p.Status;
                                    pp.RemoteId = p.RemoteId;
                                    pp.RemoteVersion = p.RemoteVersion;
                                    ctx.Add(pp);
                                }
                            }
                            await ctx.SaveChangesAsync();
                            bRet = true;
                        }// ctx
                    }// sigle && name
                }// s1 && s2
            }// p
            return bRet;
        }//MaintainsDatasetAsync
        public async Task<bool> RemoveDatasetAsync(DBDataset p)
        {
            bool bRet = false;
            if (p != null)
            {
                using (var ctx = new StatDataAppContext())
                {
                    bool done = false;
                    int nId = p.DBDatasetId;
                    if (nId != 0)
                    {
                        var l = await ctx.DBDatasets.Where(x => x.DBDatasetId == nId).ToListAsync();
                        if (l.Count() > 0)
                        {
                            DBDataset pRet = l.First();
                            ctx.Remove(pRet);
                            done = true;
                        }
                    }// nId
                    if (!done)
                    {
                        string s = p.Sigle;
                        if (!string.IsNullOrEmpty(s))
                        {
                            string ss = s.Trim().ToUpper();
                            var l = await ctx.DBDatasets.Where(x => x.Sigle == ss).ToListAsync();
                            if (l.Count() > 0)
                            {
                                DBDataset pRet = l.First();
                                ctx.Remove(pRet);
                                done = true;
                            }
                        }// s
                    }//
                    if (done)
                    {
                        await ctx.SaveChangesAsync();
                        bRet = true;
                    }
                }// ctx
            }// p
            return bRet;
        }//RemoveDatasetAsync
        //
        public async Task<int> GetDatasetVariablesCountAsync(DBDataset pSet)
        {
            int nRet = 0;
            if ((pSet != null) && (pSet.DBDatasetId != 0))
            {
                using (var ctx = new StatDataAppContext())
                {
                    var l = await ctx.DBVariables.Where(x => x.DBDatasetId == pSet.DBDatasetId).ToListAsync();
                    nRet = l.Count();
                }// ctx
            }// pSet
            return nRet;
        }//GetDatasetVariablesCountAsync
        public async Task<IList<DBVariable>> GetDatasetVariablesAsync(DBDataset pSet, int offset = 0, int count = 0)
        {
            IList<DBVariable> oRet = null;
            if ((pSet != null) && (pSet.DBDatasetId != 0))
            {
                if (offset < 0)
                {
                    offset = 0;
                }
                if (count < 1)
                {
                    count = 25;
                }
                using (var ctx = new StatDataAppContext())
                {
                    oRet = await ctx.DBVariables.Where(x => x.DBDatasetId == pSet.DBDatasetId).Skip(offset).Take(count).ToListAsync();
                }// ctx
            }// pSet
            return oRet;
        }//GetDatasetVariablesAsync
        public async Task<DBVariable> FindVariableAsync(DBVariable p)
        {
            DBVariable pRet = null;
            if (p != null)
            {
                using (var ctx = new StatDataAppContext())
                {
                    int nId = p.DBVariableId;
                    if (nId != 0)
                    {
                        var l = await ctx.DBVariables.Where(x => x.DBVariableId == nId).ToListAsync();
                        if (l.Count() > 0)
                        {
                            pRet = l.First();
                        }
                    }// nId
                    if ((pRet == null) && (p.DBDatasetId != 0))
                    {
                        string s = p.Sigle;
                        if (!string.IsNullOrEmpty(s))
                        {
                            string ss = s.Trim().ToUpper();
                            var l = await ctx.DBVariables.Where(x => (x.Sigle == ss) && (x.DBDatasetId == p.DBDatasetId)).ToListAsync();
                            if (l.Count() > 0)
                            {
                                pRet = l.First();
                            }
                        }// s
                    }// pRet
                }// ctx
            }// p
            return pRet;
        }// FindVariableAsync
    }// class LocalDataManager
}
