using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CreatorData
{
    public partial class ProductLicenseType
    {
        public static IList<ProductLicenseType> GetAllProductLicenseTypes()
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                return db.ProductLicenseTypes.ToList();
            }
        }
    }
}
