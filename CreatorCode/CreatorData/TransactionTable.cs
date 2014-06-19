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

        public static IList<TransactionTable> GetTransactionsByLicenseName(string licenseName)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                return db.TransactionTables.Where(c => c.LicenseTable.LicenseName.Equals(licenseName)).ToList();
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

        public static IList<TransactionTable> GetTransactionsByProductLicenseID(int productLicenseID)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                return db.TransactionTables.Where(c => c.taOrderID == productLicenseID).ToList();
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

        public static void CreateAllTransactions(IList<TransactionTable> transactions)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.TransactionTables.InsertAllOnSubmit(transactions);
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

        public static void UpdateAllTransactions(IList<TransactionTable> transactions)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                foreach (TransactionTable transaction in transactions)
                {
                    db.TransactionTables.Attach(transaction, true);
                    db.SubmitChanges();
                }
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
