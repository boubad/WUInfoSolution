using System.Collections.Generic;
using Microsoft.EntityFrameworkCore;
using System.ComponentModel.DataAnnotations;
//
namespace StatDataApp.Data
{
    public class DBDataset
    {
        [Key]
        public int DBDatasetId { get; set; }
        [MaxLength(127)]
        public string RemoteId { get; set; }
        [MaxLength(127)]
        public string RemoteVersion { get; set; }
        [Required]
        [MinLength(1)]
        [MaxLength(31)]
        public string Sigle { get; set; }
        [Required]
        [MinLength(1)]
        [MaxLength(63)]
        public string Name { get; set; }
        public int Status { get; set; }
        [MaxLength(255)]
        public string Description { get; set; }
        //
        public List<DBIndiv> DBIndivs { get; set; }
    }// DBDataset
    //
    public class DBIndiv
    {
        [Key]
        public int DBIndivId { get; set; }
        [MaxLength(127)]
        public string RemoteId { get; set; }
        [MaxLength(127)]
        public string RemoteVersion { get; set; }
        [Required]
        [MinLength(1)]
        [MaxLength(31)]
        public string Sigle { get; set; }
        [Required]
        [MinLength(1)]
        [MaxLength(63)]
        public string Name { get; set; }
        public int Status { get; set; }
        [MaxLength(255)]
        public string Description { get; set; }
        //
        public int DBDatasetId { get; set; }
        public DBDataset DBDataset { get; set; }
        //
        public List<DBValue> DBValues { get; set; }
    }// DBIndiv
     //
    public class DBVariable
    {
        [Key]
        public int DBVariableId { get; set; }
        [Timestamp]
        public byte[] Timestamp { get; set; }
        //
        [Required]
        [MinLength(1)]
        [MaxLength(31)]
        public string Sigle { get; set; }
        [Required]
        [MinLength(1)]
        [MaxLength(31)]
        public string Name { get; set; }
        public int Status { get; set; }
        public int VariableType { get; set; }
        public int VariableKind { get; set; }
        [MaxLength(255)]
        public string Description { get; set; }
        public string ModalitesString { get; set; }
        //
        [MaxLength(127)]
        public string RemoteId { get; set; }
        [MaxLength(127)]
        public string RemoteVersion { get; set; }
        //
        public int DBDatasetId { get; set; }
        public DBDataset DBDataset { get; set; }
        public List<DBValue> DBValues { get; set; }
    }// DBVariable
    //
    public class DBValue
    {
        [Key]
        public int DBValueId { get; set; }
        [Timestamp]
        public byte[] Timestamp { get; set; }
        //
        public int DBIndivId { get; set; }
        public DBIndiv DBIndiv { get; set; }
        public int DBVariableId { get; set; }
        public DBVariable DBVariable { get; set; }
        //
        public int VariableType { get; set; }
        [MaxLength(15)]
        public string StringValue { get; set; }
        [MaxLength(255)]
        public string Description { get; set; }
        public int Status { get; set; }
        //
        [MaxLength(127)]
        public string RemoteId { get; set; }
        [MaxLength(127)]
        public string RemoteVersion { get; set; }
    }// DBValue
     //
    public class StatDataAppContext : DbContext
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
    }// class StatDataAppContext
}
