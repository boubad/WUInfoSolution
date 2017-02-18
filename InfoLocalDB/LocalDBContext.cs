using System;
using Microsoft.EntityFrameworkCore;

namespace InfoLocalDB
{
    public class LocalDBContext : DbContext
    {
        static private Object lockObj = new object();
        public LocalDBContext()
        {
            lock (lockObj)
            {
                Database.EnsureCreated();
            }// lock
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
            base.OnConfiguring(optionsBuilder);
        }//OnConfiguring

    }// class LocalDBContext
}
