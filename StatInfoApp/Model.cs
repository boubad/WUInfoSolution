using System.Collections.Generic;
using Microsoft.EntityFrameworkCore;
//
namespace StatInfoApp
{
    public class DBDataset
    {
        public string DBDatasetId { get; set; }
        public string Version { get; set; }
        public string Sigle { get; set; }
        public string Name { get; set; }
        public int Status { get; set; }
        public string Description { get; set; }
        //
        public List<DBIndiv> DBIndivs { get; set; } 
    }// DBDataset
    //
    public class DBIndiv
    {
        public string DBIndivId { get; set; }
        public string Version { get; set; }
        public string Sigle { get; set; }
        public string Name { get; set; }
        public int Status { get; set; }
        public string Description { get; set; }
        //
        public string DBDatasetId { get; set; }
        public DBDataset DBDataset { get; set; }
        //
        public List<DBValue> DBValues { get; set; }
    }// DBIndiv
     //
    public class DBVariable
    {
        public string DBIndivId { get; set; }
        public string Version { get; set; }
        public string Sigle { get; set; }
        public string Name { get; set; }
        public int Status { get; set; }
        public int VariableType { get; set; }
        public int VariableKind{ get; set; }
        public string Description { get; set; }
        public string Modalites { get; set; }
        //
        public string DBDatasetId { get; set; }
        public DBDataset DBDataset { get; set; }
        //
        public List<DBValue> DBValues { get; set; }
    }// DBVariable
    //
    public class DBValue
    {
        public string DBValueId { get; set; }
        public string Version { get; set; }
        public int Status { get; set; }
        public int VariableType { get; set; }
        public string Description { get; set; }
        public string StringValue { get; set; }
        //
        public string DBIndivId { get; set; }
        public DBIndiv DBIndiv { get; set; }
        public string DBVariableId { get; set; }
        public DBVariable DBVariable { get; set; }
    }// DBValue
     //
    public class StatInfoAppContext : DbContext
    {
        public DbSet<DBDataset> DBDatasets { get; set; }
        public DbSet<DBVariable> DBVariables { get; set; }
        public DbSet<DBIndiv> DBIndivs { get; set; }
        public DbSet<DBValue> DBValues { get; set; }
        //
        protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
        {
            //base.OnConfiguring(optionsBuilder);
            optionsBuilder.UseSqlite("Filename=StatInfo.db");
        }//OnConfiguring
    }// class StatInfoAppContext
}// StatInfoApp
