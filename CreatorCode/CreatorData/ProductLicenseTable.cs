using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Linq.Dynamic;

namespace CreatorData
{
    public partial class ProductLicenseTable
    {
        public static IList<ProductLicenseTable> GetProductLicensesByConditions(string conditionStrings)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                var licenses = db.ProductLicenseTables.Where(conditionStrings).ToList();
                return licenses;
            }
        }

         public static IList<ProductLicenseTable> GetAllProductLicenses()
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                return db.ProductLicenseTables.ToList();
            }
        }

        public static IList<ProductLicenseTable> GetProductLicenses(string licenseServerName)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                return db.ProductLicenseTables.Where(c => c.LicenseID > 0 &&                                                                                                                 
                                                          c.LicenseTable.LicenseName.Equals(licenseServerName)).ToList();
            }
        }

        public static ProductLicenseTable GetProductLicense(string productLicenseID)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                return db.ProductLicenseTables.Where(c => c.LicenseID > 0 && c.plID.Equals(productLicenseID)).SingleOrDefault();
            }
        }

        public static ProductLicenseTable GetProductLicenseByID(int productLicenseID)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                return db.ProductLicenseTables.Where(c => c.LicenseID > 0 && c.ID.Equals(productLicenseID)).SingleOrDefault();
            }
        }

        public static IList<ProductLicenseTable> GetProductLicenses(string licenseServerName, int productID)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {                
                db.ObjectTrackingEnabled = false;
                return db.ProductLicenseTables.Where(c => c.LicenseID > 0 && c.LicenseTable.LicenseName.Equals(licenseServerName) &&
                                           c.ProductID.Equals(productID)).ToList();
            }
        }

        public static IList<string> GetDeactivateProductLicenses(string licenseServerName)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;                
                var plDeactive = from c in db.ProductLicenseTables
                                 where c.LicenseID > 0  
                                 where c.LicenseTable.LicenseName.Equals(licenseServerName)                                 
                                 where !c.IsActive  
                                 select c.plID;
                return plDeactive.ToList();
            }
        }

        public static IList<String> GetAddOnProductLicenses(string productLicenseID)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                var addonOrderNames = from c in db.ProductLicenseTables
                                      where c.LicenseID > 0
                                      where c.ParentProductLicenseID.Equals(productLicenseID)
                                      select c.plID;
                return addonOrderNames.ToList();                
            }
        }

        public static void DeactivateProductLicense(string plID)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                ProductLicenseTable plt = db.ProductLicenseTables.Where(c => c.plID.Equals(plID)).First();
                if (plt != null)
                {
                    plt.plState = 3;
                    UpdateProductLicense(plt);
                }
            }
        }

        //how to get unique order index
        //save last used index in license table
        //update everytime order gets added 
        public static int GetNextProductLicenseIndex(string licenseServerName)
        {            
            //int orderIndex = 0;
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                LicenseTable license = db.LicenseTables.Where(c => c.LicenseName.Equals(licenseServerName)).FirstOrDefault();
                if (license != null)                
                    return license.OrderIndex;                
                return 0;
            }
        }

        public static int GetProductVersionFromTable(byte productID)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                var versions = from p in db.ProductTables
                              where p.pId.Equals(productID)
                              select p;
                if (versions.Count() > 0)
                    return versions.FirstOrDefault().pVersion;
                return 0;
            }
        }

        public static void DeleteAllProductLicenses(string licenseServer)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                IList<ProductLicenseTable> pltRecords = (from c in db.ProductLicenseTables
                                 where c.plID.Contains(licenseServer)
                                 select c).ToList();
                //automatically delete associated modules
                foreach (ProductLicenseTable plt in pltRecords)
                {
                    ModuleTable.DeleteAllModules(plt.ID);
                }
                db.ProductLicenseTables.DeleteAllOnSubmit(pltRecords);
                db.SubmitChanges();
            }
        }

        public static void CreateProductLicense(ProductLicenseTable plt)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ProductLicenseTables.InsertOnSubmit(plt);
                db.SubmitChanges();
            }
        }

        public static void UpdateProductLicense(ProductLicenseTable plt)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ProductLicenseTables.Attach(plt, true);
                db.SubmitChanges();
            }
        }

        public static void DeleteProductLicense(ProductLicenseTable plt)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                ModuleTable.DeleteAllModules(plt.ID);
                db.ProductLicenseTables.Attach(plt);
                db.ProductLicenseTables.DeleteOnSubmit(plt);
                db.SubmitChanges();
            }
        }
    }
}
