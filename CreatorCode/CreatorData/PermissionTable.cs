using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CreatorData
{
    public partial class PermissionsTable
    {
        //each license can only have at most one of each token type
        public static PermissionsTable GetPermissionByUser(string user)
        {
            PermissionsTable userPermissions = null;
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                userPermissions = db.PermissionsTables.Where(c => c.pt_login.Equals(user)).SingleOrDefault();
                if (userPermissions == null)
                {
                    userPermissions = new PermissionsTable();
                    userPermissions.pt_create_modify_key = false;
                    userPermissions.pt_extension_pwd = false;
                    userPermissions.pt_login = user;
                    userPermissions.pt_module_pwd = false;
                    userPermissions.pt_permanent_pwd = false;
                    userPermissions.pt_version_pwd = false;
                }
            }
            return userPermissions;
        }
    }
}
