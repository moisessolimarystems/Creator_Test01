using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CreatorData
{
    public partial class ModuleTable
    {
        public static IList<ModuleTable> GetAllModules(string productLicenseName)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                return db.ModuleTables.Where(c => c.ProductLicenseTable.plID == productLicenseName).ToList();
            }
        }
        public static ModuleTable GetModule(string productLicenseName, int modID)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                return db.ModuleTables.Where(c => c.ProductLicenseTable.plID == productLicenseName && c.ModID == modID).FirstOrDefault();
            }
        }
        public static void CreateAllModules(IList<ModuleTable> modules)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ModuleTables.InsertAllOnSubmit(modules);
                db.SubmitChanges();
            }
        }
        public static void UpdateAllModules(IList<ModuleTable> modules)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                foreach (ModuleTable module in modules)
                {
                    db.ModuleTables.Attach(module, true);
                    db.SubmitChanges();
                }
            }
        }
        //public static void DeleteAllModules(IList<ModuleTable> modules)
        //{
        //    using (CreatorDataContext db = new CreatorDataContext())
        //    {
        //        db.ModuleTables.DeleteAllOnSubmit(modules);
        //        db.SubmitChanges();
        //    }
        //}
        public static void DeleteAllModules(int productlicenseID)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                IList<ModuleTable> modules = db.ModuleTables.Where(m => m.ProductLicenseID == productlicenseID).ToList() ;
                db.ModuleTables.DeleteAllOnSubmit(modules);
                db.SubmitChanges();
            }
        }
        public static void CreateModule(ModuleTable module)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ModuleTables.InsertOnSubmit(module);
                db.SubmitChanges();
            }
        }
        public static void UpdateModule(ModuleTable module)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ModuleTables.Attach(module, true);
                db.SubmitChanges();
            }
        }
        public static void DeleteModule(ModuleTable module)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ModuleTables.Attach(module);
                db.ModuleTables.DeleteOnSubmit(module);
                db.SubmitChanges();
            }
        }
    }
}
