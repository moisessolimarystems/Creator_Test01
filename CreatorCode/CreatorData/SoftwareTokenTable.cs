using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CreatorData
{
    public partial class SoftwareTokenTable
    {
        public static IList<SoftwareTokenTable> GetAllSoftwareTokens()
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                return db.SoftwareTokenTables.ToList();
            }
        }

        public static SoftwareTokenTable GetSoftwareToken(string tokenType)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                return db.SoftwareTokenTables.Where(t => t.TokenType.Equals(tokenType)).Single();
            }
        }

        public static void UpdateProductTable(SoftwareTokenTable token)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.SoftwareTokenTables.Attach(token, true);
                db.SubmitChanges();
            }
        }
    }
}
