using System;
using System.Collections.Generic;
using System.Linq;
using System.Data.Linq;
using System.Linq.Dynamic;
using System.Text;
namespace CreatorData
{
    public partial class LicenseTable
    {
        public static IList<LicenseTable> GetLicensesByConditions(string conditionStrings)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                DataLoadOptions dlo = new DataLoadOptions();
                dlo.LoadWith<LicenseTable>(id => id.PacketTables);
                dlo.LoadWith<LicenseTable>(id => id.TokenTables);
                db.LoadOptions = dlo;
                var licenses = db.LicenseTables.Where(conditionStrings).ToList();                                
                return licenses;
            }
        }

        public static LicenseTable GetLicenseByName(string licenseName, bool enableLoadOptions)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                if (enableLoadOptions)
                {
                    DataLoadOptions dlo = new DataLoadOptions();
                    dlo.LoadWith<LicenseTable>(id => id.TransactionTables);
                    db.LoadOptions = dlo;
                }
                return (LicenseTable)db.LicenseTables.Where(c => c.LicenseName.Equals(licenseName)).SingleOrDefault();
            }
        }

        public static LicenseTable GetLicenseByID(int licID, bool enableLoadOptions)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                if (enableLoadOptions)
                {
                    DataLoadOptions dlo = new DataLoadOptions();
                    dlo.LoadWith<LicenseTable>(id => id.TransactionTables);
                    db.LoadOptions = dlo;
                }
                return (LicenseTable)db.LicenseTables.Where(c => c.ID.Equals(licID)).SingleOrDefault();
            }
        }

        public static IList<LicenseTable> GetLicensesByCustomer(string custName)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                //need to be able to translate destid to destname from searchstring
                var licenseFilter = from l in db.LicenseTables
                                    where l.CustomerTable.SCRname.Equals(custName)
                                    select l;
                return licenseFilter.ToList();
            }
        }

        public static IList<LicenseTable> GetLicensesByDestination(string custName, string destName, string searchString, bool enableLoadOptions)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                if (enableLoadOptions)
                {
                    DataLoadOptions dlo = new DataLoadOptions();
                    dlo.LoadWith<LicenseTable>(id => id.PacketTables);
                    db.LoadOptions = dlo;
                }
                //need to be able to translate destid to destname from searchstring
                var licenseFilter = from l in db.LicenseTables
                                    join d in db.DestinationNameTables on
                                    new { DestID = l.DestinationID, CustID = l.SCRnumber }
                                    equals
                                    new { DestID = d.DestID, CustID = d.CustID }
                                    where l.CustomerTable.SCRname.Equals(custName) &&
                                          d.DestName.Equals(destName) &&                                          
                                          (l.LicenseName.Contains(searchString) ||
                                          l.ProductLicenseTables.Count(o => (o.plID.Contains(searchString) &&
                                                                                          o.LicenseID.Equals(l.ID))) > 0)
                                    select l;
                return licenseFilter.ToList();
            }
        }

        public static uint GetNextGroupID(uint custID, uint destID)
        {
            int groupID = 0;
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                var licenses = (from l in db.LicenseTables
                               where l.DestinationID.Equals(destID) &&
                                     l.SCRnumber.Equals(custID)                                                                                                               
                               orderby l.GroupID                               
                               select l).ToList();
                if (licenses.Count() > 0 && licenses.FirstOrDefault().GroupID == 0)
                {
                    foreach (var lic in licenses)
                    {
                        //0 case -> covered
                        //middle case -> covered
                        //end case 
                        if ((lic.GroupID - groupID) > 1)
                        {   //increment to next available group ID
                            groupID = groupID + 1;
                            break;
                        }                        
                        groupID = lic.GroupID;
                    }
                    if (groupID == licenses.Last().GroupID)
                        groupID = groupID + 1;
                }                
            }
            return (uint)groupID;
        }

        public static int GetLicenseCountByDestName(uint custID, uint destID)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                var licenses = from l in db.LicenseTables
                               where l.DestinationID.Equals(destID) &&
                                     l.SCRnumber.Equals(custID) 
                               select l;
                return licenses.Count();
            }
        }

        public static bool IsLicenseModified(string licenseName)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                var license = db.LicenseTables.Where(c => c.LicenseName.Equals(licenseName)).FirstOrDefault();
                if (license != null)
                {
                    if (license.IsDirty)
                        return true;
                }
            }
            return false;
        }

        public static IList<string> GetModifiedLicenseNamesByCustomer(string custName)
        {
            //return all license names for a customer where a license has transactions that have no packetid value
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;

                var licenses = from c in db.LicenseTables
                               where c.CustomerTable.SCRname.Equals(custName) &&
                                     c.IsDirty
                               select c.LicenseName;                
                return licenses.ToList();
            }
        }

        public static IList<string> GetEnabledLicenseNamesByCustomer(string custName)
        {
            //return all license names for a customer where a license has transactions that have no packetid value
            //enabled = SW tokens present not enabled = SW tokens not present
            //enabled = HW token active, not enabled = HW token deactive or not present
            using (CreatorDataContext db = new CreatorDataContext())
            {
                //find all licenses that have (sw tokens - reserved) or (hw tokens - active)
                db.ObjectTrackingEnabled = false;
                var licenses = from l in db.LicenseTables
                               from t in db.TokenTables
                               where l.CustomerTable.SCRname.Equals(custName) &&
                                     ((t.TokenType == 1 && t.TokenStatus == 1) || (t.TokenType > 1 && t.TokenStatus == 0)) &&
                                     t.LicenseID == l.ID
                               select l.LicenseName;
                return licenses.Distinct().ToList();
            }
        }  

        public static void CreateLicense(LicenseTable license)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.LicenseTables.InsertOnSubmit(license);
                db.SubmitChanges();
            }
        }

        public static void UpdateLicense(LicenseTable license)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.LicenseTables.Attach(license, true);
                db.SubmitChanges();
            }
        }

        public static void DeleteLicense(LicenseTable license)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.LicenseTables.Attach(license);
                db.LicenseTables.DeleteOnSubmit(license);
                db.SubmitChanges();
            }
        }        
    }
}
