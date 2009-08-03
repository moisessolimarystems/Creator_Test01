using System;
using System.Collections.Generic;
using System.Linq;
using System.Data.Linq;
using System.Text;
using System.IO;

namespace CreatorData
{
    public partial class CustomerTable
    {
        public static IList<CustomerTable> GetAllCustomers(string searchString, bool enableLoadOptions)
        {
            IList<CustomerTable> list, destList;
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                if (enableLoadOptions)
                {
                    DataLoadOptions dlo = new DataLoadOptions();
                    dlo.LoadWith<CustomerTable>(id => id.LicenseTables);
                    db.LoadOptions = dlo;
                }
                
                if (searchString.Length == 0)
                {
                    list = db.CustomerTables.OrderBy(c => c.SCRnumber).ToList();
                }
                else
                {
                    IList<DestinationNameTable> destNames = db.DestinationNameTables.Where(d => d.DestName.Contains(searchString)).ToList();
                    list = db.CustomerTables.Where(c => c.LicenseTables.Where(d => d.LicenseName.Contains(searchString)).Count() > 0 || 
                                                          c.SCRname.Contains(searchString)).ToList();
                    foreach (DestinationNameTable destname in destNames)
                    {
                        destList = db.CustomerTables.Where(c => c.LicenseTables.Where(d => d.DestinationID.Equals(destname.DestID) &&
                                                                             d.SCRnumber.Equals(destname.CustID)).Count() > 0).ToList();
                        foreach (CustomerTable custRec in destList)
                        {
                            if(list.Where(cr => cr.SCRnumber == custRec.SCRnumber).Count() < 1)
                                list.Add(custRec);                            
                        }
                    }                                                                                                        
                }

                return list;
            }
        }

        public static IList<string> GetAllCustomerNames(string searchString)
        {
            IList<string> list;
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                if (searchString.Length == 0)
                {
                    list = db.CustomerTables.OrderBy(c => c.SCRnumber).Select(n => n.SCRname).ToList();
                }
                else
                {
                    IList<DestinationNameTable> destNames = db.DestinationNameTables.Where(d => d.DestName.Contains(searchString)).ToList();
                    list = db.CustomerTables.Where(c => c.LicenseTables.Where(d => d.LicenseName.Contains(searchString)).Count() > 0 ||
                                                          c.SCRname.Contains(searchString)).Select(n => n.SCRname).ToList();
                }
                return list;
            }
        }

        public static int GetCustomerCount(string searchString, bool enableLoadOptions)
        {
            int result;
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                if (enableLoadOptions)
                {
                    DataLoadOptions dlo = new DataLoadOptions();
                    dlo.LoadWith<CustomerTable>(id => id.LicenseTables);
                    db.LoadOptions = dlo;
                }   
                if (searchString.Length == 0)
                {
                    result = db.CustomerTables.OrderBy(c => c.SCRnumber).Count();
                }
                else if (Int32.TryParse(searchString, out result))
                {
                    result = db.CustomerTables.Where(c => c.SCRnumber.ToString().Contains(searchString)).Count();
                }
                else
                {
                    result = db.CustomerTables.Where(c => c.SCRname.Contains(searchString)).Count();
                }
                //query.Skip((iPageNum - 1) * iPageSize).Take(iPageSize);
                return result;
            }
        }

        public static CustomerTable GetCustomer(string searchString, bool enableLoadOptions)
        {
            int result;
            CustomerTable rec;
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                if (enableLoadOptions)
                {
                    DataLoadOptions dlo = new DataLoadOptions();
                    dlo.LoadWith<CustomerTable>(id => id.LicenseTables);
                    db.LoadOptions = dlo;
                }  
                if (Int32.TryParse(searchString, out result))
                {
                    rec = db.CustomerTables.Where(c => c.SCRnumber.ToString().Equals(searchString)).SingleOrDefault();
                }
                else
                {
                    rec = db.CustomerTables.Where(c => c.SCRname.Equals(searchString)).SingleOrDefault();
                }
            }
            return rec;
        }

        public static void CreateCustomer(CustomerTable custRecord)
        {
            using (CreatorDataContext dataContext = new CreatorDataContext())
            {
                StringWriter writer = new StringWriter();
                dataContext.CustomerTables.InsertOnSubmit(custRecord);
                dataContext.SubmitChanges();
            }
        }

        public static void UpdateCustomer(CustomerTable custRecord)
        {
            using (CreatorDataContext dataContext = new CreatorDataContext())
            {
                dataContext.CustomerTables.Attach(custRecord, true);
                dataContext.SubmitChanges();
            }
        }

        public static void DeleteCustomer(CustomerTable custRecord)
        {
            using (CreatorDataContext dc = new CreatorDataContext())
            {
                dc.CustomerTables.Attach(custRecord);
                dc.CustomerTables.DeleteOnSubmit(custRecord);
                dc.SubmitChanges();
            }
        }

        public static int GetLastCustomerID()
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                return db.CustomerTables.OrderByDescending(c => c.SCRnumber).First().SCRnumber;
            }
        }
    }
}
