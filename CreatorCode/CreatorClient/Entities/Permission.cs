using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Client.Creator.CreatorService;

namespace Client.Creator
{
    public class Permission
    {
        #region Fields
        PermissionsTable _permissionRec;
        #endregion

        #region Constructor
        public Permission()
        {
        }

        public Permission(PermissionsTable permRec)
        {
            _permissionRec = permRec;
        }
        #endregion

    }
}
