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
                var licenses = db.LicenseTables.Where(conditionStrings).ToList();                
                
                return licenses;
            }
        }

        public static IList<LicenseTable> GetLicensesByID(int custID, int destID, int groupID, bool enableLoadOptions)
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
                var licenses = from l in db.LicenseTables
                               where l.GroupID.Equals(groupID) && 
                                     l.DestinationID.Equals(destID) &&
                                     l.SCRnumber.Equals(custID)                                                                         
                               select l;
                return licenses.ToList();                               
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

        public static IList<LicenseTable> GetAllLicenses(string searchString, bool enableLoadOptions)
        {
            IList<LicenseTable> list;
            int result;
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                if (enableLoadOptions)
                {
                    DataLoadOptions dlo = new DataLoadOptions();
                    dlo.LoadWith<LicenseTable>(id => id.PacketTables);
                    db.LoadOptions = dlo;
                }
                if (searchString.Length == 0)
                {
                    list = db.LicenseTables.OrderBy(c => c.ID).ToList();
                    
                }
                else
                {
                    Int32.TryParse(searchString, out result);
                    list = db.LicenseTables.Where(c => c.LicenseName.Contains(searchString) ||    
                                                  c.CustomerTable.SCRname.Contains(searchString) ||                                                                                                                          
                                                  c.LicenseComments.Contains(searchString)).ToList();
                }

                return list;
            }
        }

        public static int GetLicenseCountByID(int custID, int destID, int groupID)
        {
            int count = 0;
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                var licenses = from l in db.LicenseTables
                               where l.GroupID.Equals(groupID) && 
                                     l.DestinationID.Equals(destID) &&  
                                     l.SCRnumber.Equals(custID) 
                               select l;
                count = licenses.Count();                          
            }
            return count;
        }

        public static uint GetLastGroupID(uint custID, uint destID)
        {
            uint groupID = 0;
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                var license = from l in db.LicenseTables
                               where l.DestinationID.Equals(destID) &&
                                     l.SCRnumber.Equals(custID) &&
                                     l.GroupID == db.LicenseTables.Max(g => g.GroupID)
                               select l;
                if(license.Count() > 0)
                    groupID = (uint) license.FirstOrDefault().GroupID;
            }
            return groupID;
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

        public static uint GetLastDestinationID(uint custID)
        {
            uint destinationID = 0;
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                var license = from l in db.LicenseTables
                              where l.SCRnumber.Equals(custID) &&
                                    l.DestinationID == db.LicenseTables.Max(d => d.DestinationID)
                              select l;
                if(license.Count() > 0)
                    destinationID = (uint) license.FirstOrDefault().DestinationID;
            }
            return destinationID;
        }

        public static int GetLicCountByType(uint custID, uint destID, uint groupID, Byte licType)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;

                var licenses = from l in db.LicenseTables
                               where l.GroupID.Equals(groupID) &&
                                     l.DestinationID.Equals(destID)  &&
                                     l.SCRnumber.Equals(custID) &&
                                     l.LicenseType.Equals(licType)
                               select l;
                return licenses.Count();
            }
        }

        public static int GetDerivedLicenseCount(uint custID, uint destID, uint groupID, Byte licType)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;

                var licenses = from l in db.LicenseTables
                               where l.GroupID.Equals(groupID) &&
                                     l.DestinationID.Equals(destID) &&
                                     l.SCRnumber.Equals(custID) &&
                                     l.LicenseType != licType
                               select l;
                return licenses.Count();
            }
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
                    var licenses = db.LicenseTables.Where(c => c.SCRnumber.Equals(license.SCRnumber) &&
                                                          c.DestinationID.Equals(license.DestinationID) &&
                                                          c.GroupID.Equals(license.GroupID) &&
                                                          c.LicenseType.Equals(license.LicenseType) &&
                                                          c.TransactionTables.Where(t => t.taPacketID.HasValue != true).Count() > 0).ToList();

                    if (licenses.Count > 0)
                        return true;
                }
                return false;
            }
        }

        
        public static bool IsLicenseUpdated(string licenseName)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                //license is up to date if standard license transaction latest transaction date matches latest secondary transaction date
                //order or product transaction
                var license = db.LicenseTables.Where(c => c.LicenseName.Equals(licenseName)).FirstOrDefault();
                if (license != null)
                {
                    var standardLicense = db.LicenseTables.Where(c => c.SCRnumber.Equals(license.SCRnumber) &&
                                          c.DestinationID.Equals(license.DestinationID) &&
                                          c.GroupID.Equals(license.GroupID) &&
                                          c.LicenseType.Equals(0)).FirstOrDefault();
                    switch (license.LicenseType)
                    {
                        case 0:
                            //std
                            break;
                        case 1:
                            //failover
                            var latestStdTransaction = db.TransactionTables.Where(t => t.taLicenseID.Equals(standardLicense.ID) &&
                                                                              (t.taType.Equals(1) || t.taType.Equals(2))).OrderByDescending(d => d.taDateCreated).FirstOrDefault();
                            var latestLicTransaction = db.TransactionTables.Where(t => t.taLicenseID.Equals(license.ID) &&
                                                                              (t.taType.Equals(1) || t.taType.Equals(2))).OrderByDescending(d => d.taDateCreated).FirstOrDefault();
                            if (latestLicTransaction == null && latestStdTransaction != null)
                                return false;
                            if (latestLicTransaction != null && latestStdTransaction != null)
                            {
                                if (latestStdTransaction.taDateCreated.CompareTo(latestLicTransaction.taDateCreated) > 0)
                                    return false;
                            }
                            break;
                        case 2:
                        //disaster                        
                        case 3:
                            //test dev
                            //find each order in TD/DR
                            //compare latest order transaction for TD/DR vs latest std order transaction
                            //
                            int standardOrder = 0;
                            var orders = db.OrderTables.Where(c => c.LicenseID.Equals(license.ID));
                            foreach (var order in orders)
                            {
                                //issue -> corresponding order id for standard is necessary
                                standardOrder = db.OrderTables.Where(c => c.LicenseID.Equals(standardLicense.ID) &&
                                                                          c.OrderNumber.Contains("S")).FirstOrDefault().ID;
                                var stdTransaction = db.TransactionTables.Where(c => c.taLicenseID.Equals(standardLicense.ID) &&
                                                                                     c.taOrderID.Equals(standardOrder) &&
                                                                                     c.taType.Equals(2)).OrderByDescending(d => d.taDateCreated).FirstOrDefault();
                                var licTransaction = db.TransactionTables.Where(c => c.taLicenseID.Equals(license.ID) &&
                                                                                     c.taOrderID.Equals(order.ID) &&
                                                                                     c.taType.Equals(2)).OrderByDescending(d => d.taDateCreated).FirstOrDefault();

                                if (licTransaction == null && stdTransaction != null)
                                    return false;
                                if (licTransaction != null && stdTransaction != null)
                                {
                                    //standard order trans newer then license trans then it is out of date
                                    if (stdTransaction.taDateCreated.CompareTo(licTransaction.taDateCreated) > 0)
                                        return false;
                                }
                            }
                            break;
                        case 4:
                            break;
                        //subscription
                        default: break;
                    }
                }                                      
            }
            return true;
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
