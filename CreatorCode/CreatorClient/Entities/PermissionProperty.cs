using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Client.Creator.CreatorService;

namespace Client.Creator
{
    public class PermissionProperty
    {
        #region Fields
        PermissionsTable _permissionRec;
        #endregion

        #region Constructor
        public PermissionProperty()
        {
        }

        public PermissionProperty(PermissionsTable permRec)
        {
            _permissionRec = permRec;
        }
        #endregion

    }
}
