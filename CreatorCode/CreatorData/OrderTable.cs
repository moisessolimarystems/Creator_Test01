using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CreatorData
{
    public partial class OrderTable
    {
        public static IList<OrderTable> GetAllOrders()
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                return db.OrderTables.ToList();
            }
        }

        public static IList<OrderTable> GetOrdersByLicenseName(string licenseName)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                return db.OrderTables.Where(c => c.LicenseID > 0 && c.LicenseTable.LicenseName.Equals(licenseName)).ToList();
            }
        }

        public static OrderTable GetOrderByOrderName(string orderNumber)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                return db.OrderTables.Where(c => c.LicenseID > 0 && c.OrderNumber.Equals(orderNumber)).SingleOrDefault();
            }
        }

        public static IList<OrderTable> GetOrdersEqualsProduct(string licenseName, string productName)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {                
                db.ObjectTrackingEnabled = false;
                return db.OrderTables.Where(c => c.LicenseID > 0 && c.LicenseTable.LicenseName.Equals(licenseName) &&
                                           c.ProductName.Equals(productName)).ToList();
            }
        }

        //how to get unique order index
        //save last used index in license table
        //update everytime order gets added 
        public static int GetNextOrderIndex(string licenseName)
        {            
            //int orderIndex = 0;
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                LicenseTable license = db.LicenseTables.Where(c => c.LicenseName.Equals(licenseName)).FirstOrDefault();
                if (license != null)                
                    return license.OrderIndex;                
                return 0;

                //var order = (from l in db.Orders
                //               where l.License.LicenseName.Equals(licenseName)
                //               orderby l.OrderIndex
                //               select l).ToList();
                //if (orders.Count() > 0 && orders.FirstOrDefault().OrderIndex == 0)
                //{
                //    //foreach (var order in orders)
                //    //{
                //    //    //0 case -> covered
                //    //    //middle case -> covered
                //    //    //end case 
                //    //    if ((order.OrderIndex - orderIndex) > 1)
                //    //    {   //increment to next available group ID
                //    //        orderIndex = orderIndex + 1;
                //    //        break;
                //    //    }
                //    //    orderIndex = order.OrderIndex;
                //    //}
                //    //if (orderIndex == orders.Last().OrderIndex)
                //    //    orderIndex = orderIndex + 1;
                //    orderIndex = orders.Last().OrderIndex + 1;
                //}   
            }
            //return orderIndex;
        }

        public static int GetProductVersionFromTable(int productID)
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

        public static void CreateOrder(OrderTable contract)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.OrderTables.InsertOnSubmit(contract);
                db.SubmitChanges();
            }
        }

        public static void UpdateOrder(OrderTable contract)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.OrderTables.Attach(contract, true);
                db.SubmitChanges();
            }
        }

        public static void DeleteOrder(OrderTable contract)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.OrderTables.Attach(contract);
                db.OrderTables.DeleteOnSubmit(contract);
                db.SubmitChanges();
            }
        }  
    }
}
