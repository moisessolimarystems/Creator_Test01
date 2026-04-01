using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CreatorData
{
    public partial class TransactionTable
    {
        public static IList<TransactionTable> GetAllTransactions()
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                return db.TransactionTables.ToList();
            }
        }

        //sublicense transactions should be <= standardlicense transactions
        public static void UpdateSubLicenseTransactionsByOrder(int stdLicOrderID, int subLicOrderID)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                var stdTransanctions = db.TransactionTables.Where(c => c.taOrderID.Equals(stdLicOrderID));
                foreach(var trans in stdTransanctions)
                {
                    var foundTransaction = db.TransactionTables.Where(c => c.taDateCreated.Equals(trans.taDateCreated) && c.taOrderID.Equals(subLicOrderID)).FirstOrDefault();
                    if(foundTransaction == null)
                    {
                        var order = db.OrderTables.Where(o => o.ID.Equals(subLicOrderID)).FirstOrDefault();
                        TransactionTable newTransaction = new TransactionTable();
                        newTransaction.taDateCreated = trans.taDateCreated;
                        newTransaction.taDescription = trans.taDescription;
                        newTransaction.taLicenseID = order.LicenseID;
                        newTransaction.taOrderID = subLicOrderID;
                        newTransaction.taPacketID = null;
                        newTransaction.taType = trans.taType;
                        newTransaction.taUnits = trans.taUnits;
                        newTransaction.taUser = trans.taUser;
                        CreateTransaction(newTransaction);
                    }
                }
            }
        }

        public static IList<TransactionTable> GetTransactionsByPacketID(int packetID) 
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                return db.TransactionTables.Where(c => c.taPacketID.Equals(packetID)).ToList();
            }
        }

        public static IList<TransactionTable> GetNewTransactionsByLicenseName(string licenseName)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                return db.TransactionTables.Where(c => c.LicenseTable.LicenseName.Equals(licenseName) && !c.taPacketID.HasValue).ToList();
            }
        }

        public static void CreateTransaction(TransactionTable transaction)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.TransactionTables.InsertOnSubmit(transaction);
                db.SubmitChanges();
            }
        }

        public static void UpdateTransaction(TransactionTable transaction)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.TransactionTables.Attach(transaction, true);
                db.SubmitChanges();
            }
        }

        public static void DeleteTransaction(TransactionTable transaction)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.TransactionTables.Attach(transaction, false);
                db.TransactionTables.DeleteOnSubmit(transaction);
                db.SubmitChanges();
            }
        }   
    }
}
