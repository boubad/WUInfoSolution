
using System;
using System.Collections.Generic;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using InfoLocalDB;
using InfoTestData;
using InfoCouchDB;
using InfoDomain;
namespace UnitTestCS
{

    [TestClass]
    public class UnitTestLocal
    {
        [ClassInitialize()]
        public static void ClassInit(TestContext context)
        {
            using (var man = new LocalDBManager())
            {
                string mime_type = "application/octet-stream";
                byte[] blobData = new byte[128];
                for (int i = 0; i < blobData.Length; ++i)
                {
                    blobData[i] = (byte)i;
                }// i
                string sigle = "testset";
                LocalDataset xSet = new LocalDataset(sigle);
                Assert.IsTrue(xSet.IsStoreable);
                man.MaintainsDatasetAsync(xSet).Wait();
                LocalDataset pSet = man.FindDatasetAsync(xSet).Result;
                Assert.IsNotNull(pSet);
                int nId = pSet.LocalDatasetId;
                Assert.IsTrue(nId != 0);
                LocalBlob b1 = new LocalBlob() { Name = pSet.Sigle, MimeType = mime_type, Data = blobData};
                man.MaintainsDatasetBlobAsync(pSet, b1).Wait();
                //
                string varsigle = "testvar";
                LocalVariable xVar = new LocalVariable(pSet, varsigle)
                {
                    VariableType = (int)InfoDataType.Real,
                    VariableKind = (int)InfoKind.Normal
                };
                Assert.IsTrue(xVar.IsStoreable);
                man.MaintainsVariableAsync(xVar).Wait();
                LocalVariable pVar = man.FindVariableAsync(xVar).Result;
                Assert.IsNotNull(pVar);
                LocalVariable pVar2 = man.FindVariableBySiglesAsync(pSet.Sigle, pVar.Sigle).Result;
                Assert.IsNotNull(pVar2);
                Assert.AreEqual(pVar.LocalVariableId, pVar2.LocalVariableId);
                LocalBlob b2 = new LocalBlob() { Name = pVar.Sigle, MimeType = mime_type, Data = blobData };
                man.MaintainsVariableBlobAsync(pVar, b2).Wait();
                //
                string indsigle = "testind";
                LocalIndiv xInd = new LocalIndiv(pSet, indsigle);
                Assert.IsTrue(xInd.IsStoreable);
                man.MaintainsIndivAsync(xInd).Wait();
                LocalIndiv pInd = man.FindIndivAsync(xInd).Result;
                Assert.IsNotNull(pInd);
                LocalIndiv pInd2 = man.FindIndivBySiglesAsync(pSet.Sigle, pInd.Sigle).Result;
                Assert.IsNotNull(pInd2);
                Assert.AreEqual(pInd.LocalIndivId, pInd2.LocalIndivId);
                LocalBlob b3 = new LocalBlob() { Name = pInd.Sigle, MimeType = mime_type, Data = blobData };
                man.MaintainsIndivBlobAsync(pInd, b3).Wait();
                //
                string sval = string.Format("{0}", 3.14159);
                LocalValue xVal = new LocalValue(pInd, pVar)
                {
                    VariableType = pVar.VariableType,
                    StringValue = sval
                };
                Assert.IsTrue(xVal.IsStoreable);
                man.MaintainsValueAsync(xVal).Wait();
                LocalValue pVal = man.FindValueAsync(xVal).Result;
                Assert.IsNotNull(pVal);
                Assert.IsTrue(pVal.LocalValueId != 0);
                //
            }// man
        }// class Init
        [TestMethod]
        public void Test_Local_Conso()
        {
            TestData src = new TestData();
            using (var man = new LocalDBManager())
            {
                string setsigle = src.ConsoName;
                LocalDataset pSet = man.FindDatasetBySigleAsync(setsigle).Result;
                if (pSet == null)
                {
                    LocalDataset xSet = new LocalDataset()
                    {
                        Sigle = setsigle,
                        Name = "Conso Data Name",
                        Description = "Conso Data Description",
                        Status = (int)InfoStatus.Normal
                    };
                    man.MaintainsDatasetAsync(xSet).Wait();
                    pSet = man.FindDatasetBySigleAsync(setsigle).Result;
                    Assert.IsNotNull(pSet);
                }// pSet
                Assert.IsTrue(pSet.LocalDatasetId != 0);
                List<LocalVariable> vars = new List<LocalVariable>();
                var cols = src.ConsoColsNames;
                foreach (var varsigle in cols)
                {
                    LocalVariable v = man.FindVariableBySiglesAsync(setsigle, varsigle).Result;
                    if (v == null)
                    {
                        LocalVariable xVar = new LocalVariable(pSet, varsigle)
                        {
                            Name = varsigle + " name",
                            Description = varsigle + " description",
                            VariableKind = (int)InfoKind.Normal,
                            VariableType = (int)InfoDataType.Integer
                        };
                        vars.Add(xVar);
                    }// v
                }// s
                if (vars.Count > 0)
                {
                    man.MaintainsVariablesAsync(vars).Wait();
                }// vars
                List<LocalIndiv> inds = new List<LocalIndiv>();
                var rows = src.ConsoRowsNames;
                foreach (var indsigle in rows)
                {
                    LocalIndiv v = man.FindIndivBySiglesAsync(setsigle, indsigle).Result;
                    if (v == null)
                    {
                        LocalIndiv xInd = new LocalIndiv(pSet, indsigle)
                        {
                            Name = indsigle + " name",
                            Description = indsigle + " description",
                            Status = (int)InfoStatus.Normal
                        };
                        inds.Add(xInd);
                    }// v
                }// indsigle
                if (inds.Count > 0)
                {
                    man.MaintainsIndivsAsync(inds).Wait();
                }
                List<LocalValue> vals = new List<LocalValue>();
                var data = src.ConsoData;
                int[] oAr = (new List<int>(data)).ToArray();
                int nCols = src.MortalColsCount;
                foreach (var indsigle in rows)
                {
                    LocalIndiv pInd = man.FindIndivBySiglesAsync(setsigle, indsigle).Result;
                    Assert.IsNotNull(pInd);
                    Assert.IsTrue(pInd.LocalIndivId != 0);
                    int irow = rows.IndexOf(indsigle);
                    foreach (var varsigle in cols)
                    {
                        LocalVariable pVar = man.FindVariableBySiglesAsync(setsigle, varsigle).Result;
                        Assert.IsNotNull(pVar);
                        Assert.IsTrue(pVar.LocalVariableId != 0);
                        int icol = cols.IndexOf(varsigle);
                        LocalValue v = man.FindValueBySiglesAsync(setsigle, indsigle, varsigle).Result;
                        if (v == null)
                        {
                            LocalValue xVal = new LocalValue(pInd, pVar);
                            int pos = irow * nCols + icol;
                            Assert.IsTrue(pos < oAr.Length);
                            int ival = oAr[pos];
                            string sval = String.Format("{0}", ival);
                            xVal.StringValue = sval;
                            vals.Add(xVal);
                        }// v
                    }// varsigle
                }// indsigle
                if (vals.Count > 0)
                {
                    man.MaintainsValuesAsync(vals).Wait();
                }
            }// man
        }//Test_Local_Conso
        [TestMethod]
        public void Test_Local_Mortal()
        {
            TestData src = new TestData();
            using (var man = new LocalDBManager())
            {
                string setsigle = src.MortalName;
                LocalDataset pSet = man.FindDatasetBySigleAsync(setsigle).Result;
                if (pSet == null)
                {
                    LocalDataset xSet = new LocalDataset() {
                        Sigle = setsigle,
                        Name = "Mortal Data Name",
                        Description = "Mortal Data Description",
                        Status = (int)InfoStatus.Normal
                    };
                    man.MaintainsDatasetAsync(xSet).Wait();
                    pSet = man.FindDatasetBySigleAsync(setsigle).Result;
                    Assert.IsNotNull(pSet);
                }// pSet
                Assert.IsTrue(pSet.LocalDatasetId != 0);
                List<LocalVariable> vars = new List<LocalVariable>();
                var cols = src.MortalColsNames;
                foreach (var varsigle in cols)
                {
                    LocalVariable v = man.FindVariableBySiglesAsync(setsigle, varsigle).Result;
                    if (v == null)
                    {
                        LocalVariable xVar = new LocalVariable(pSet, varsigle)
                        {
                            Name = varsigle + " name",
                            Description = varsigle + " description",
                            VariableKind = (int)InfoKind.Normal,
                            VariableType = (int)InfoDataType.Integer
                        };
                        vars.Add(xVar);
                    }// v
                }// s
                if (vars.Count > 0)
                {
                   man.MaintainsVariablesAsync(vars).Wait();
                }// vars
                List<LocalIndiv> inds = new List<LocalIndiv>();
                var rows = src.MortalRowsNames;
                foreach (var indsigle in rows)
                {
                    LocalIndiv v = man.FindIndivBySiglesAsync(setsigle, indsigle).Result;
                    if (v == null)
                    {
                        LocalIndiv xInd = new LocalIndiv(pSet, indsigle)
                        {
                            Name = indsigle + " name",
                            Description = indsigle + " description",
                            Status = (int)InfoStatus.Normal
                        };
                        inds.Add(xInd);
                    }// v
                }// indsigle
                if (inds.Count > 0)
                {
                    man.MaintainsIndivsAsync(inds).Wait();
                }
                List<LocalValue> vals = new List<LocalValue>();
                var data = src.MortalData;
                int[] oAr = (new List<int>(data)).ToArray();
                int nCols = src.MortalColsCount;
                foreach (var indsigle in rows)
                {
                    LocalIndiv pInd = man.FindIndivBySiglesAsync(setsigle, indsigle).Result;
                    Assert.IsNotNull(pInd);
                    Assert.IsTrue(pInd.LocalIndivId != 0);
                    int irow = rows.IndexOf(indsigle);
                    foreach (var varsigle in cols)
                    {
                        LocalVariable pVar = man.FindVariableBySiglesAsync(setsigle, varsigle).Result;
                        Assert.IsNotNull(pVar);
                        Assert.IsTrue(pVar.LocalVariableId != 0);
                        int icol = cols.IndexOf(varsigle);
                        LocalValue v = man.FindValueBySiglesAsync(setsigle, indsigle, varsigle).Result;
                        if (v == null)
                        {
                            LocalValue xVal = new LocalValue(pInd, pVar);
                            int pos = irow * nCols + icol;
                            Assert.IsTrue(pos < oAr.Length);
                            int ival = oAr[pos];
                            string sval = String.Format("{0}", ival);
                            xVal.StringValue = sval;
                            vals.Add(xVal);
                        }// v
                    }// varsigle
                }// indsigle
                if (vals.Count > 0)
                {
                    man.MaintainsValuesAsync(vals).Wait();
                }
            }// man
        }//Test_Local_Mortal
        [TestMethod]
        public void Test_Local_DatasetsCount()
        {
            using (var man = new LocalDBManager())
            {
                int nx = man.GetDatasetsCountAsync().Result;
                Assert.IsTrue(nx >= 0);
                if (nx > 0)
                {
                    IList<LocalDataset> pp = man.GetDatasetsAsync(0, nx).Result;
                    Assert.AreEqual(nx, pp.Count);
                    foreach (var pSet in pp)
                    {
                        IList<LocalBlob> b1 = man.GetDatasetBlobsAsync(pSet).Result;
                        Assert.IsNotNull(b1);
                        int nxx = man.GetDatasetValuesCountAsync(pSet).Result;
                        if (nxx > 0)
                        {
                            IList<LocalValue> vv = man.GetDatasetValuesAsync(pSet, 0, nxx).Result;
                            Assert.AreEqual(nxx, vv.Count);
                        }// nxx
                        int nv = man.GetDatasetVariablesCountAsync(pSet).Result;
                        if (nv > 0)
                        {
                            IList<LocalVariable> pv = man.GetDatasetVariablesAsync(pSet, 0, nv).Result;
                            Assert.AreEqual(nv, pv.Count);
                            foreach (var pVar in pv)
                            {
                                IList<LocalBlob> b2 = man.GetVariableBlobsAsync(pVar).Result;
                                Assert.IsNotNull(b2);
                                int nvx = man.GetVariableValuesCountAsync(pVar).Result;
                                if (nvx > 0)
                                {
                                    IList<LocalValue> vv = man.GetVariableValuesAsync(pVar, 0, nvx).Result;
                                    Assert.AreEqual(nvx, vv.Count);
                                }// nvx
                            }// pv
                        }// nv
                        int nr = man.GetDatasetIndivsCountAsync(pSet).Result;
                        if (nr > 0)
                        {
                            IList<LocalIndiv> pv = man.GetDatasetIndivsAsync(pSet, 0, nv).Result;
                            Assert.AreEqual(nr, pv.Count);
                            foreach (var pInd in pv)
                            {
                                IList<LocalBlob> b3 = man.GetIndivBlobsAsync(pInd).Result;
                                Assert.IsNotNull(b3);
                                int nvx = man.GetIndivValuesCountAsync(pInd).Result;
                                if (nvx > 0)
                                {
                                    IList<LocalValue> vv = man.GetIndivValuesAsync(pInd, 0, nvx).Result;
                                    Assert.AreEqual(nvx, vv.Count);
                                }// nvx
                            }// pv
                        }// nv
                    }// pp
                }// nx
            }// man
        }//Test_Local_DatasetsCount

    }// class UnitTestLocal

}
