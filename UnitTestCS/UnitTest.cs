
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
                string sigle = "testset";
                LocalDataset xSet = new LocalDataset(sigle);
                Assert.IsTrue(xSet.IsStoreable);
                man.MaintainsDatasetAsync(xSet).Wait();
                LocalDataset pSet = man.FindDatasetAsync(xSet).Result;
                Assert.IsNotNull(pSet);
                int nId = pSet.LocalDatasetId;
                Assert.IsTrue(nId != 0);
                //
                string varsigle = "testvar";
                LocalVariable xVar = new LocalVariable(pSet, varsigle);
                xVar.VariableType = (int)InfoDataType.Real;
                xVar.VariableKind = (int)InfoKind.Normal;
                Assert.IsTrue(xVar.IsStoreable);
                man.MaintainsVariableAsync(xVar).Wait();
                LocalVariable pVar = man.FindVariableAsync(xVar).Result;
                Assert.IsNotNull(pVar);
                LocalVariable pVar2 = man.FindVariableBySiglesAsync(pSet.Sigle, pVar.Sigle).Result;
                Assert.IsNotNull(pVar2);
                Assert.AreEqual(pVar.LocalVariableId, pVar2.LocalVariableId);
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
                //
                string sval = string.Format("{0}", 3.14159);
                LocalValue xVal = new LocalValue(pInd, pVar);
                xVal.VariableType = pVar.VariableType;
                xVal.StringValue = sval;
                Assert.IsTrue(xVal.IsStoreable);
                man.MaintainsValueAsync(xVal).Wait();
                LocalValue pVal = man.FindValueAsync(xVal).Result;
                Assert.IsNotNull(pVal);
                Assert.IsTrue(pVal.LocalValueId != 0);
                //
            }// man
        }// class Init

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
