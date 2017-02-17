using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.EntityFrameworkCore;

namespace InfoLocalDB
{
    public class LocalDBContext : DbContext
    {
        public LocalDBContext()
        {
        }
        public LocalDBContext(DbContextOptions<LocalDBContext> options):
            base(options)
        {
        }
        //
        public DbSet<LocalDataset> LocalDatasets { get; set; }
        public DbSet<LocalVariable> LocalVariables { get; set; }
        public DbSet<LocalIndiv> LocalIndivs { get; set; }
        public DbSet<LocalValue> LocalValues { get; set; }
        //
        protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
        {
            optionsBuilder.UseSqlite("Filename=StatInfo.db");
        }//OnConfiguring

    }// class LocalDBContext
}
