using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using Solimar.Licensing.Attribs;
using System.Reflection;
using Client.Creator.CreatorService;
using Client.Creator.ServiceProxy;
using System.Security.Principal;

namespace Client.Creator
{
    [TypeConverter(typeof(PropertySorterTypeConverter))]
    public class LicenseServer : BaseGridItem
    {
        #region Fields

        Lic_PackageAttribs.Lic_LicenseInfoAttribs _licInfo;
        string                                    _destinationName;
        string                                    _comments;
        PermissionsTable                          _permissions;
        bool                                      _isActive;
        string                                    _userLock;
        int                                      _productLicenseIndex;

        #endregion

        #region Constructor
        /// <summary>
        /// Initializes an empty instance of the LicenseServer class. 
        /// Represents a license object that holds the entire contents of a license server(tokens, product licenses, modules)
        /// </summary>
        public LicenseServer()
        {
            _licInfo = new Lic_PackageAttribs.Lic_LicenseInfoAttribs();            
        }

        /// <summary>
        /// Initializes a new instance of the LicenseServer class.
        /// Represents a license object that holds the entire contents of a license server(tokens, product licenses, modules)
        /// Uses information from the database to initialize the fields of the LicenseServer.
        /// </summary>
        /// <param name="license">LicenseTable object from database.</param>
        /// <param name="permissions">PermissionTable object from datbase.</param>
        public LicenseServer(LicenseTable license, PermissionsTable permissions)
        {
            Lic_PackageAttribs licPackage = new Lic_PackageAttribs();
            licPackage.Stream = license.LicenseInfo;            
            _licInfo = new Lic_PackageAttribs.Lic_LicenseInfoAttribs();
            _licInfo = licPackage.licLicenseInfoAttribs;
            _isActive = license.IsActive;
            _comments = license.LicenseComments;
            _productLicenseIndex = license.OrderIndex;
            Permissions = permissions;
        }
        #endregion

        #region Properties
        #region UnBrowsable
        /// <summary>
        /// Gets or sets the PermissionsTable to retrieve user rights.
        /// </summary>
        [Browsable(false)]
        [System.ComponentModel.RefreshProperties(RefreshProperties.All)]
        public PermissionsTable Permissions
        {
            get { return _permissions; }
            set { _permissions = value; }
        }

        /// <summary>
        /// Gets or sets the LicenseInfoAttribs object to hold license server information.
        /// </summary>
        [Browsable(false)]
        public Lic_PackageAttribs.Lic_LicenseInfoAttribs LicInfo
        {
            get { return _licInfo; }
            set { _licInfo = value; }
        }

        /// <summary>
        /// Gets the modified status of the license server.
        /// </summary>
        [Browsable(false)]
        public bool IsModified
        {
            get
            {
                bool bValue = false;
                Service<ICreator>.Use((client) =>
                {
                    bValue = client.IsLicenseModified(Name);
                });
                return bValue;
            }
        }

        /// <summary>
        /// Gets or sets the active status of the license server.
        /// </summary>
        [Browsable(false)]
        public bool IsActive
        {
            get { return _isActive; }
            set { _isActive = value; }
        }

        /// <summary>
        /// Gets the enabled status for the license server. Enabled if valid tokens have been set for the license server.
        /// </summary>
        [Browsable(false)]
        public bool IsEnabled
        {
            get
            {
                bool bEnabled = true;
                Service<ICreator>.Use((client) =>
                {
                    IList<TokenTable> tokenList = client.GetTokensByLicenseName(Name);
                    if(tokenList != null && tokenList.Count > 0)
                    {
                        foreach (TokenTable token in tokenList)
                        {
                            if (token.TokenType == (byte)Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID)
                            {
                                if (token.TokenStatus != (byte)TokenStatus.Active)
                                    bEnabled = false;
                                break;
                            }
                        }
                    }
                    else
                        bEnabled = false;
                });
                return bEnabled;
            }
        }

        /// <summary>
        /// Gets the enabled status for the license server. Enabled if valid tokens have been set for the license server.
        /// </summary>
        [Browsable(false)]
        public string UserLock
        {
            //LS locked if userlock is not "", current user has lock if userlock == current user
            get { return _userLock ?? string.Empty; } //side effect...requires call of LockStatus to get value           
        }

        /// <summary>
        /// Gets the enabled status for the license server. Enabled if valid tokens have been set for the license server.
        /// </summary>
        [Browsable(false)]
        public bool LockStatus 
        {
            get
            {   //sets UserLock 
                bool bLocked = true;
                Service<ICreator>.Use((client) =>
                {
                    LicenseTable lt = client.GetLicenseByName(Name, false);
                    if (lt != null)
                    {
                        if (lt.UserLock == null) 
                            bLocked = false;                        
                        //_userLock = (lt.UserLock != null) ? lt.UserLock.ToLower() : null;
                        _userLock = lt.UserLock ?? null; //set ToLower()
                    }
                });
                return bLocked;
            }
            set
            {
                Service<ICreator>.Use((client) =>
                { 
                    LicenseTable lt = client.GetLicenseByName(Name, false);
                    if (lt != null)
                        lt.UserLock = (value) ? WindowsIdentity.GetCurrent().Name.ToLower() : null;
                    client.UpdateLicense(lt, false);
                });
            }
        }

        /// <summary>
        /// Gets the active status of any hardware token included with the license server.
        /// </summary>
        [Browsable(false)]
        public bool HasActiveHardwareToken
        {
            get
            {
                bool bHardware = false;
                Service<ICreator>.Use((client) =>
                {
                    IList<TokenTable> tokenList = client.GetTokensByLicenseName(Name);
                    if (tokenList != null && tokenList.Count > 0)
                    {
                        foreach (TokenTable token in tokenList)
                        {
                            if (token.TokenType == (byte)Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID)
                            {
                                if (token.TokenStatus == (byte)TokenStatus.Active)
                                    bHardware = true;
                                break;
                            }
                        }
                    }
                    else
                        bHardware = false;
                });
                return bHardware;
            }
        }

        /// <summary>
        /// Gets lost status of any hardware token included with the license server.
        /// </summary>
        [Browsable(false)]
        public bool HasLostToken
        {
            get
            {
                bool bLost = false;
                Service<ICreator>.Use((client) =>
                {
                    IList<TokenTable> tokenList = client.GetTokensByLicenseName(Name);
                    if (tokenList != null && tokenList.Count > 0)
                    {
                        foreach (TokenTable token in tokenList)
                        {
                            if (token.TokenType == (byte)Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID)
                            {
                                if (token.TokenStatus == (byte)TokenStatus.Lost)
                                    bLost = true;
                                break;
                            }
                        }
                    }
                    else
                        bLost = false;
                });
                return bLost;
            }
        }

        /// <summary>
        /// Gets or sets the customer ID. Represents the first section of the license server ID.
        /// </summary>
        [Browsable(false)]
        public uint CustID
        {
            get { return _licInfo.customerID.TVal; }
            set { _licInfo.customerID.TVal = value; }
        }

        /// <summary>
        /// Gets or sets the destination ID. Represents the middle section of the license server ID.
        /// </summary>
        [Browsable(false)]
        public uint DestID
        {
            get { return _licInfo.destinationID.TVal; }
            set { _licInfo.destinationID.TVal = value; }
        }

        /// <summary>
        /// Gets or sets the group ID. Represents the last section of the license server ID.
        /// </summary>
        [Browsable(false)]
        public uint GroupID
        {
            get { return _licInfo.softwareGroupLicenseID.TVal; }
            set { _licInfo.softwareGroupLicenseID.TVal = value; }
        }

        [Browsable(false)]
        public int ProductLicenseIndex
        {
            get { return _productLicenseIndex; }
            set { _productLicenseIndex = value; }
        }

        /// <summary>
        /// Gets or sets the user comments for the license server.
        /// </summary>
        [Browsable(false)]
        public string Comments
        {
            get { return _comments ?? string.Empty; }
            set
            {
                if (_comments != value)
                {
                    Service<ICreator>.Use((client) =>
                    {
                        LicenseTable lt = client.GetLicenseByName(Name, false);
                        if (lt != null)
                        {
                            lt.LicenseComments = value;
                            client.UpdateLicense(lt, false);
                        }
                    });
                    _comments = value;
                }
            }
        }
        #endregion

        #region Browsable
        /// <summary>
        /// Gets the license server ID. Uses helper function to format ID from license server object.
        /// </summary>
        [Category("License Server"), PropertyOrder(1)]
        [DisplayName("Name")]
        [Description("Unique identifier for the license server.")]
        [ReadOnly(true)]
        public override string Name
        {
            get { return Lic_LicenseInfoAttribsHelper.GetDisplayLabel(_licInfo); }
        }

        /// <summary>
        /// Gets or sets the destination name for the license server.
        /// </summary>
        [Category("License Server"), PropertyOrder(2)]
        [DisplayName("Destination Name")]
        [Description("Name of the end user for the license server.")]
        [ReadOnly(true)]
        public string DestName
        {
            get 
            {
                Service<ICreator>.Use((client) =>
                {
                    DestinationNameTable dest = client.GetDestinationName((int)CustID, (int)DestID);
                    if (dest != null)
                        _destinationName = dest.DestName;
                });
                return _destinationName; 
            }
        }

        /// <summary>
        /// Gets or sets the user notes for the license server. Full string held in Comments property.
        /// </summary>
        [Editor(typeof(System.ComponentModel.Design.MultilineStringEditor), typeof(System.Drawing.Design.UITypeEditor))]
        [Category("License Server Notes"), PropertyOrder(1)]
        [DisplayName("Notes")]
        [Description("User notes for license server.")]
        public string Description
        {
            get
            {
                if (Comments.Contains("|"))
                {
                    //split return first half
                    string[] notes = Comments.Split("|".ToCharArray());
                    return notes[0];
                }
                return Comments;
            }
            set
            {
                if (Comments.Contains("|"))
                {
                    string[] notes = Comments.Split("|".ToCharArray());
                    notes[0] = value;
                    Comments = string.Format("{0}|{1}", notes[0], notes[1]);
                }
                else
                    Comments = value;
            }
        }

        /// <summary>
        /// Gets or sets the additional user notes for the license server. Full string held in Comments property.
        /// </summary>
        [Editor(typeof(System.ComponentModel.Design.MultilineStringEditor), typeof(System.Drawing.Design.UITypeEditor))]
        [Category("License Server Notes"), PropertyOrder(2)]
        [DisplayName("Additional Notes")]
        [Description("User notes for license server.")]
        public string AdditionalDescription
        {
            get
            {
                if (Comments.Contains("|"))
                {
                    //split return first half
                    string[] notes = Comments.Split("|".ToCharArray());
                    return notes[1];
                }
                return string.Empty;
            }
            set
            {
                if (Comments.Contains("|"))
                {
                    string[] notes = Comments.Split("|".ToCharArray());
                    notes[1] = value;
                    Comments = string.Format("{0}|{1}", notes[0], notes[1]);
                }
                else
                {
                    if (value.Length > 0)
                    {
                        Comments = string.Format("{0}|{1}", _comments, value);
                    }
                }
            }
        }
        #endregion
        #endregion

        #region Methods
        public void Sync()
        {
            //lock status, 
        }

        public bool LockedByCurrentUser()
        {
            return (string.Compare(UserLock, WindowsIdentity.GetCurrent().Name, true) == 0);// UserLock == WindowsIdentity.GetCurrent().Name.ToLower();
        }

        public bool LockedByAnyUser()
        {
            return UserLock.Length > 0;
        }

        public bool LockedByAdmin()
        {
            return LockedByAnyUser() && _permissions.pt_permanent_pwd.Value;
        }
        #endregion
    }
}
