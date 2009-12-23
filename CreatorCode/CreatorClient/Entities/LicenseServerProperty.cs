using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using Solimar.Licensing.Attribs;
using System.Reflection;
using Client.Creator.CreatorService;
using Client.Creator.ServiceProxy;

namespace Client.Creator
{
    [TypeConverter(typeof(PropertySorterTypeConverter))]
    public class LicenseServerProperty : BaseGridItem
    {
        #region Private Variables

        private Lic_PackageAttribs.Lic_LicenseInfoAttribs _licInfo;
        private string _destinationName;
        private string _comments;
        private PermissionsTable _permissions;
        private bool _isActive;

        #endregion

        public LicenseServerProperty()
        {
            _licInfo = new Lic_PackageAttribs.Lic_LicenseInfoAttribs();
        }

        public LicenseServerProperty(LicenseServerProperty license)
        {
            _licInfo = new Lic_PackageAttribs.Lic_LicenseInfoAttribs();
            _licInfo.Stream = license.LicInfo.Stream;
            _comments = license.Comments;
            _permissions = license.Permissions;
            _isActive = license.IsActive;
            _destinationName = license.DestName;
        }

        //requires a licPackage stream for licStream not just licLicenseInfoAttribs.
        public LicenseServerProperty(LicenseTable license, PermissionsTable permissions)
        {            
            Lic_PackageAttribs licPackage = new Lic_PackageAttribs();
            licPackage.Stream = license.LicenseInfo;            
            _licInfo = new Lic_PackageAttribs.Lic_LicenseInfoAttribs();
            _licInfo = licPackage.licLicenseInfoAttribs;
            _isActive = license.IsActive;
            _comments = license.LicenseComments;
            Permissions = permissions;
            Service<ICreator>.Use((client) => 
            {
                DestinationNameTable dest = client.GetDestinationName((int)CustID, (int)DestID);
                if(dest != null)
                    _destinationName = dest.DestName;
            });
        }

        #region Properties
        [Browsable(false)]
        [System.ComponentModel.RefreshProperties(RefreshProperties.All)]
        public PermissionsTable Permissions
        {
            get { return _permissions; }
            set
            {
                _permissions = value;
            }
        }   

        [Browsable(false)]
        public Lic_PackageAttribs.Lic_LicenseInfoAttribs LicInfo
        {
            get { return _licInfo; }
            set { _licInfo = value; }
        }

        [Browsable(false)]
        public Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType SoftwareLicenseType
        {
            get { return _licInfo.softwareLicType.TVal; }
            set { _licInfo.softwareLicType.TVal = value; }
        }     

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
        //Deactivation sets to false otherwise retrieve value from database.
        [Browsable(false)]
        public bool IsActive
        {
            get { return _isActive; }
            set { _isActive = value; }
        }

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
                                if (token.TokenStatus != 1)
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

        [Browsable(false)]
        public uint CustID
        {
            get { return _licInfo.customerID.TVal; }
            set { _licInfo.customerID.TVal = value; }
        }

        [Browsable(false)]
        public uint DestID
        {
            get { return _licInfo.destinationID.TVal; }
            set { _licInfo.destinationID.TVal = value; }
        }

        [Browsable(false)]
        public uint GroupID
        {
            get { return _licInfo.softwareGroupLicenseID.TVal; }
            set { _licInfo.softwareGroupLicenseID.TVal = value; }
        }

        [Category("License Server"), PropertyOrder(1)]
        [DisplayName("Name")]
        [Description("Unique identifier for the license server.")]
        [ReadOnly(true)]
        public override string Name
        {
            get
            {
                return Lic_LicenseInfoAttribsHelper.GetDisplayLabel(_licInfo);
            }
        }

        [Category("License Server"), PropertyOrder(2)]
        [DisplayName("Destination Name")]
        [Description("Name of the end user for the license server.")]
        [ReadOnly(true)]
        public string DestName
        {
            get { return _destinationName; }
            set { _destinationName = value; }
        }

        [Browsable(false)]
        public string Comments
        {
            get { return _comments; }
            set { _comments = value; }
        }

        [Editor(typeof(System.ComponentModel.Design.MultilineStringEditor), typeof(System.Drawing.Design.UITypeEditor))]
        [Category("License Server Notes"), PropertyOrder(1)]
        [DisplayName("Notes")]
        [Description("User notes for license server.")]
        public string Description
        {
            get
            {
                if (_comments.Contains("|"))
                {
                    //split return first half
                    string[] notes = _comments.Split("|".ToCharArray());
                    return notes[0];
                }
                return _comments;
            }
            set
            {
                if (_comments.Contains("|"))
                {
                    string[] notes = _comments.Split("|".ToCharArray());
                    notes[0] = value;
                    _comments = string.Format("{0}|{1}", notes[0], notes[1]);
                }
                else
                    _comments = value;
            }
        }
        [Editor(typeof(System.ComponentModel.Design.MultilineStringEditor), typeof(System.Drawing.Design.UITypeEditor))]
        [Category("License Server Notes"), PropertyOrder(2)]
        [DisplayName("Additional Notes")]
        [Description("User notes for license server.")]
        public string AdditionalDescription
        {
            get
            {
                if (_comments.Contains("|"))
                {
                    //split return first half
                    string[] notes = _comments.Split("|".ToCharArray());
                    return notes[1];
                }
                return "";
            }
            set
            {
                if (_comments.Contains("|"))
                {
                    string[] notes = _comments.Split("|".ToCharArray());
                    notes[1] = value;
                    _comments = string.Format("{0}|{1}", notes[0], notes[1]);
                }
                else
                {
                    if (value.Length > 0)
                    {
                        _comments = string.Format("{0}|{1}", _comments, value);
                    }
                }
            }
        }    
        #endregion

    }
}
