using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CreatorData
{
    public partial class ProductTable
    {
        public static IList<ProductTable> GetAllProducts()
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                return db.ProductTables.ToList();
            }
        }

        public static ProductTable GetProduct(string product)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                return db.ProductTables.Where(p => p.pName.Equals(product)).Single();
            }
        }

        public static void UpdateProductTable(ProductTable product)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ProductTables.Attach(product, true);
                db.SubmitChanges();
            }
        }
    }
}
