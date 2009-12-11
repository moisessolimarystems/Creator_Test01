using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CreatorData
{
    public partial class DestinationNameTable
    {
        public static IList<DestinationNameTable> GetDestNamesByCustID(int custID)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                return db.DestinationNameTables.Where(c => c.CustID.Equals(custID)).ToList();
            }
        }

        public static uint GetNextDestinationID(uint custID)
        {
            int destinationID = 0;
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                var destNames = (from l in db.DestinationNameTables
                               where l.CustID.Equals(custID)
                               orderby l.DestID
                               select l).ToList();
                if (destNames.Count() > 0 && destNames.FirstOrDefault().DestID == 0)
                {
                    foreach (var destName in destNames)
                    {
                        if (destName.DestID - destinationID > 1)
                        {
                            destinationID = destinationID + 1;
                            break;
                        }
                        destinationID = destName.DestID;
                    }
                    if (destinationID == destNames.Last().DestID)
                        destinationID = destinationID + 1;
                }
            }
            return (uint)destinationID;
        }

        public static DestinationNameTable GetDestinationName(int custID, int destID)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                return db.DestinationNameTables.Where(c => c.CustID.Equals(custID) &&
                                                      c.DestID.Equals(destID)).SingleOrDefault();
            }
        }

        public static DestinationNameTable GetDestinationID(int custID, string destName)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                return db.DestinationNameTables.Where(c => c.CustID.Equals(custID) &&
                                                      c.DestName.Equals(destName)).SingleOrDefault();
            }
        }

        public static void DeleteAllDestinationNamesByCustomer(int custID)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                IList<DestinationNameTable> dnRecords = (from d in db.DestinationNameTables
                                                         where d.CustID.Equals(custID)
                                                         select d).ToList();
                db.DestinationNameTables.DeleteAllOnSubmit(dnRecords);
                db.SubmitChanges();
            }
        }

        public static void CreateDestinationName(DestinationNameTable destName)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.DestinationNameTables.InsertOnSubmit(destName);
                db.SubmitChanges();
            }
        }

        public static void UpdateDestinationName(DestinationNameTable destName)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.DestinationNameTables.Attach(destName, true);
                db.SubmitChanges();
            }
        }

        public static void DeleteDestinationName(DestinationNameTable destName)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.DestinationNameTables.Attach(destName);
                db.DestinationNameTables.DeleteOnSubmit(destName);
                db.SubmitChanges();
            }
        }
    }
}
