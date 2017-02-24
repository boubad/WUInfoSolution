using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using InfoTestData;
using InfoDomain;
//
namespace UnitTestCS
{
    [TestClass]
    public class DomainManagerTest
    {
        static readonly string st_baseUrl = "http://localhost:5984/";
        static readonly string st_databaseName = "test";
        //
        [TestMethod]
        public async Task Test_dman_conso()
        {
            TestData src = new TestData();
            DomainManager pMan = new DomainManager(st_baseUrl, st_databaseName);
            string setsigle = src.ConsoName;
            Dataset pSet = await pMan.FindDatasetBySigleAsync(setsigle);
            if (pSet == null)
            {
                Dataset xSet = new Dataset()
                {
                    Sigle = setsigle,
                    Name = "Conso Data Name",
                    Observations = "Conso Data Description",
                    Status = InfoStatus.Normal
                };
                bool bRet = await pMan.MaintainsDatasetAsync(xSet);
                Assert.IsTrue(bRet);
                pSet = await pMan.FindDatasetBySigleAsync(setsigle);
                Assert.IsNotNull(pSet);
            }// pSet
            Assert.IsTrue(pSet.IsPersisted);
            List<Variable> vars = new List<Variable>();
            var cols = src.ConsoColsNames;
            foreach (var varsigle in cols)
            {
                Variable v = await pMan.FindVariableBySiglesAsync(setsigle, varsigle);
                if (v == null)
                {
                    Variable xVar = new Variable(pSet, varsigle)
                    {
                        Name = varsigle + " name",
                        Observations = varsigle + " description",
                        VariableKind = InfoKind.Normal,
                        VariableType = InfoDataType.Integer,
                        Status = InfoStatus.Normal
                    };
                    vars.Add(xVar);
                }// v
            }// s
            if (vars.Count > 0)
            {
                bool b = await pMan.MaintainsVariablesAsync(vars,false);
            }// vars
            List<Indiv> inds = new List<Indiv>();
            var rows = src.ConsoRowsNames;
            foreach (var indsigle in rows)
            {
                Indiv v = await pMan.FindIndivBySiglesAsync(setsigle, indsigle);
                if (v == null)
                {
                    Indiv xInd = new Indiv(pSet, indsigle)
                    {
                        Name = indsigle + " name",
                        Observations = indsigle + " description",
                        Status = InfoStatus.Normal
                    };
                    inds.Add(xInd);
                }// v
            }// indsigle
            if (inds.Count > 0)
            {
                bool b = await pMan.MaintainsIndivsAsync(inds,false);
            }
            List<InfoValue> vals = new List<InfoValue>();
            var data = src.ConsoData;
            int[] oAr = (new List<int>(data)).ToArray();
            int nCols = src.ConsoColsCount;
            foreach (var indsigle in rows)
            {
                Indiv pInd = await pMan.FindIndivBySiglesAsync(setsigle, indsigle);
                Assert.IsNotNull(pInd);
                Assert.IsTrue(pInd.IsPersisted);
                int irow = rows.IndexOf(indsigle);
                foreach (var varsigle in cols)
                {
                    Variable pVar = await pMan.FindVariableBySiglesAsync(setsigle, varsigle);
                    Assert.IsNotNull(pVar);
                    Assert.IsTrue(pVar.IsPersisted);
                    int icol = cols.IndexOf(varsigle);
                    InfoValue v = await pMan.FindValueBySiglesAsync(setsigle, indsigle, varsigle);
                    if (v == null)
                    {
                        InfoValue xVal = new InfoValue(pInd, pVar);
                        int pos = irow * nCols + icol;
                        Assert.IsTrue(pos < oAr.Length);
                        int ival = oAr[pos];
                        xVal.Value = new InfoDataValue(ival);
                        vals.Add(xVal);
                    }// v
                }// varsigle
            }// indsigle
            if (vals.Count > 0)
            {
                bool b = await pMan.MaintainsValuesAsync(vals,false);
            }
        }//test_dman_conso
         //
        [TestMethod]
        public async Task Test_dman_mortal()
        {
            TestData src = new TestData();
            DomainManager pMan = new DomainManager(st_baseUrl, st_databaseName);
            string setsigle = src.MortalName;
            Dataset pSet = await pMan.FindDatasetBySigleAsync(setsigle);
            if (pSet == null)
            {
                Dataset xSet = new Dataset()
                {
                    Sigle = setsigle,
                    Name = "Conso Data Name",
                    Observations = "Conso Data Description",
                    Status = InfoStatus.Normal
                };
                bool bRet = await pMan.MaintainsDatasetAsync(xSet);
                Assert.IsTrue(bRet);
                pSet = await pMan.FindDatasetBySigleAsync(setsigle);
                Assert.IsNotNull(pSet);
            }// pSet
            Assert.IsTrue(pSet.IsPersisted);
            List<Variable> vars = new List<Variable>();
            var cols = src.MortalColsNames;
            foreach (var varsigle in cols)
            {
                Variable v = await pMan.FindVariableBySiglesAsync(setsigle, varsigle);
                if (v == null)
                {
                    Variable xVar = new Variable(pSet, varsigle)
                    {
                        Name = varsigle + " name",
                        Observations = varsigle + " description",
                        VariableKind = InfoKind.Normal,
                        VariableType = InfoDataType.Integer,
                        Status = InfoStatus.Normal
                    };
                    vars.Add(xVar);
                }// v
            }// s
            if (vars.Count > 0)
            {
                bool b = await pMan.MaintainsVariablesAsync(vars, false);
            }// vars
            List<Indiv> inds = new List<Indiv>();
            var rows = src.MortalRowsNames;
            foreach (var indsigle in rows)
            {
                Indiv v = await pMan.FindIndivBySiglesAsync(setsigle, indsigle);
                if (v == null)
                {
                    Indiv xInd = new Indiv(pSet, indsigle)
                    {
                        Name = indsigle + " name",
                        Observations = indsigle + " description",
                        Status = InfoStatus.Normal
                    };
                    inds.Add(xInd);
                }// v
            }// indsigle
            if (inds.Count > 0)
            {
                bool b = await pMan.MaintainsIndivsAsync(inds, false);
            }
            List<InfoValue> vals = new List<InfoValue>();
            var data = src.MortalData;
            int[] oAr = (new List<int>(data)).ToArray();
            int nCols = src.MortalColsCount;
            foreach (var indsigle in rows)
            {
                Indiv pInd = await pMan.FindIndivBySiglesAsync(setsigle, indsigle);
                Assert.IsNotNull(pInd);
                Assert.IsTrue(pInd.IsPersisted);
                int irow = rows.IndexOf(indsigle);
                foreach (var varsigle in cols)
                {
                    Variable pVar = await pMan.FindVariableBySiglesAsync(setsigle, varsigle);
                    Assert.IsNotNull(pVar);
                    Assert.IsTrue(pVar.IsPersisted);
                    int icol = cols.IndexOf(varsigle);
                    InfoValue v = await pMan.FindValueBySiglesAsync(setsigle, indsigle, varsigle);
                    if (v == null)
                    {
                        InfoValue xVal = new InfoValue(pInd, pVar);
                        int pos = irow * nCols + icol;
                        Assert.IsTrue(pos < oAr.Length);
                        int ival = oAr[pos];
                        xVal.Value = new InfoDataValue(ival);
                        vals.Add(xVal);
                    }// v
                }// varsigle
            }// indsigle
            if (vals.Count > 0)
            {
                bool b = await pMan.MaintainsValuesAsync(vals, false);
            }
        }//test_dman_mortal
        //
        [TestMethod]
        public async Task Test_dman_load()
        {
            TestData src = new TestData();
            DomainManager pMan = new DomainManager(st_baseUrl, st_databaseName);
            string setsigle = src.MortalName;
            Dataset pSet = await pMan.LoadDatasetAsync(setsigle);
            Assert.IsNotNull(pSet);
            Assert.IsTrue(pSet.IsPersisted);
            //
            var vars = pSet.Variables;
            Assert.IsNotNull(vars);
            Assert.IsTrue(vars.Count > 0);
            foreach (Variable pVar in vars)
            {
                Assert.IsNotNull(pVar);
                Assert.IsTrue(pVar.IsPersisted);
                Assert.IsNotNull(pVar.Set);
                Assert.AreEqual(pSet.Id, pVar.Set.Id);
                var vals = pVar.Values;
                Assert.IsNotNull(vals);
                Assert.IsTrue(vals.Count > 0);
                foreach (InfoValue pVal in vals)
                {
                    Assert.IsNotNull(pVal);
                    Assert.IsTrue(pVal.IsPersisted);
                    Assert.IsNotNull(pVal.Var);
                    Assert.AreEqual(pVar.Id, pVal.Var.Id);
                    Assert.IsNotNull(pVal.Ind);
                    Assert.IsTrue(pVal.Ind.IsPersisted);
                }// pVal
            }// pVar
             //
            var inds = pSet.Indivs;
            Assert.IsNotNull(inds);
            Assert.IsTrue(inds.Count > 0);
            foreach (Indiv pInd in inds)
            {
                Assert.IsNotNull(pInd);
                Assert.IsTrue(pInd.IsPersisted);
                Assert.IsNotNull(pInd.Set);
                Assert.AreEqual(pSet.Id, pInd.Set.Id);
                var vals = pInd.Values;
                Assert.IsNotNull(vals);
                Assert.IsTrue(vals.Count > 0);
                foreach (InfoValue pVal in vals)
                {
                    Assert.IsNotNull(pVal);
                    Assert.IsTrue(pVal.IsPersisted);
                    Assert.IsNotNull(pVal.Ind);
                    Assert.AreEqual(pInd.Id, pVal.Ind.Id);
                    Assert.IsNotNull(pVal.Var);
                    Assert.IsTrue(pVal.Var.IsPersisted);
                }// pVal
            }// pVar
        }//Test_dman_load
    }// class DomainManagerTest
}
