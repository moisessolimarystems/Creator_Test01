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
    public class LicenseProperty : BaseGridItem
    {
        #region Private Variables

        private Lic_PackageAttribs.Lic_LicenseInfoAttribs _licInfo;
        private string _destinationName;
        private string _comments;
        private PermissionsTable _permissions;

        #endregion

        public LicenseProperty()
        {
            _licInfo = new Lic_PackageAttribs.Lic_LicenseInfoAttribs();
        }

        public LicenseProperty(LicenseProperty license)
        {
            _licInfo = new Lic_PackageAttribs.Lic_LicenseInfoAttribs();
            _licInfo.Stream = license.LicInfo.Stream;
            _comments = license.Comments;
            _permissions = license.Permissions;
            _destinationName = license.DestName;
        }

        //requires a licPackage stream for licStream not just licLicenseInfoAttribs.
        public LicenseProperty(LicenseTable license, PermissionsTable permissions)
        {            
            Lic_PackageAttribs licPackage = new Lic_PackageAttribs();
            licPackage.Stream = license.LicenseInfo;            
            _licInfo = new Lic_PackageAttribs.Lic_LicenseInfoAttribs();
            _licInfo = licPackage.licLicenseInfoAttribs;           
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
                PropertyDescriptor descriptor = TypeDescriptor.GetProperties(this.GetType())["ActivationCurrent"];
                ReadOnlyAttribute attrib = (ReadOnlyAttribute)descriptor.Attributes[typeof(ReadOnlyAttribute)];
                FieldInfo isReadOnly = attrib.GetType().GetField("isReadOnly", BindingFlags.NonPublic | BindingFlags.Instance);
                isReadOnly.SetValue(attrib, !_permissions.pt_create_modify_key.Value);
                descriptor = TypeDescriptor.GetProperties(this.GetType())["ActivationTotal"];
                attrib = (ReadOnlyAttribute)descriptor.Attributes[typeof(ReadOnlyAttribute)];
                isReadOnly = attrib.GetType().GetField("isReadOnly", BindingFlags.NonPublic | BindingFlags.Instance);
                isReadOnly.SetValue(attrib, !_permissions.pt_create_modify_key.Value);
                descriptor = TypeDescriptor.GetProperties(this.GetType())["ActivationAmtInDays"];
                attrib = (ReadOnlyAttribute)descriptor.Attributes[typeof(ReadOnlyAttribute)];
                isReadOnly = attrib.GetType().GetField("isReadOnly", BindingFlags.NonPublic | BindingFlags.Instance);
                isReadOnly.SetValue(attrib, !_permissions.pt_create_modify_key.Value);
            }
        }   

        [Browsable(false)]
        public Lic_PackageAttribs.Lic_LicenseInfoAttribs LicInfo
        {
            get { return _licInfo; }
            set { _licInfo = value; }
        }
        [Browsable(false)]
        public bool HasUpdates
        {
            get
            {
                bool bValue = false;
                Service<ICreator>.Use((client) =>
                {
                    bValue = client.IsLicenseUpdated(Name);
                });
                return !bValue;
            }
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
        [Browsable(false)]
        public bool IsStandardLicenseType
        {
            get
            {
                if (LicType == Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltStandard ||
                    LicType == Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltStandardSubscription)
                    return true;
                return false;
            }
        }
        [Browsable(false)]
        public bool IsBackupLicenseType
        {
            get
            {
                if (LicType == Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltDisasterRecovery ||
                    LicType == Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltFailover)
                    return true;
                return false;
            }
        }
        [Browsable(false)]
        public bool IsFailoverLicenseType
        {
            get
            {
                if (LicType == Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltFailover)
                    return true;
                return false;
            }
        }
        [Browsable(false)]
        public bool IsTestDevelopmentLicenseType
        {
            get
            {
                if (LicType == Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltTestDev)
                    return true;
                return false;
            }
        }
        [Browsable(false)]
        public bool IsEnabled
        {
            get
            {
                if (_licInfo.licVerificationAttribs.TVal.validationTokenList.TVal.Count > 1)
                    return true;
                return false;
            }
        }
        [Browsable(false)]
        public int FailoverLicenseCount
        {
            get
            {
                int count = 0;
                Service<ICreator>.Use((client) =>
                {
                    count = client.GetLicenseCountByType(CustID, DestID, GroupID, Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltFailover);
                });
                return count;
            }
        }

        [Category("License"), PropertyOrder(1)]
        [DisplayName("Name")]
        [Description("Name")] 
        [ReadOnly(true)]
        public override string Name
        {
            get
            {
                return Lic_LicenseInfoAttribsHelper.GetDisplayLabel(_licInfo);
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

        [Category("License"), PropertyOrder(2)]
        [DisplayName("Destination Name")]
        [Description("Destination Name")]
        [ReadOnly(true)]
        public string DestName
        {
            get { return _destinationName; }
            set { _destinationName = value; }
        }

        [Category("License"), PropertyOrder(3)]
        [DisplayName("Type")]
        [Description("Type")]
        [ReadOnly(true)]
        public Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType LicType
        {
            get { return _licInfo.softwareLicType.TVal; }
            set { _licInfo.softwareLicType.TVal = value; }
        }

        [Editor(typeof(System.ComponentModel.Design.MultilineStringEditor), typeof(System.Drawing.Design.UITypeEditor))]
        [Category("License"), PropertyOrder(4)]
        [DisplayName("Comments")]
        [Description("Comments")]
        public string Comments
        {
            get { return _comments; }
            set { _comments = value; }
        }

        [Category("User Activations"), PropertyOrder(1)]
        [DisplayName("Current")]
        [Description("Current")]
        [ReadOnly(true)]
        public uint ActivationCurrent
        {
            get { return _licInfo.activationCurrent.TVal; }
            set { _licInfo.activationCurrent.TVal = value; }
        }


        [Category("User Activations"), PropertyOrder(2)]
        [DisplayName("Total")]
        [Description("Total")]
        [ReadOnly(true)]
        public uint ActivationTotal
        {
            get { return _licInfo.activationTotal.TVal; }
            set { _licInfo.activationTotal.TVal = value; }
        }

        [Category("User Activations"), PropertyOrder(3)]
        [DisplayName("Amount in Days")]
        [Description("Amount in Days")]
        [ReadOnly(true)]
        public uint ActivationAmtInDays
        {
            get { return _licInfo.activationAmountInDays.TVal; }
            set { _licInfo.activationAmountInDays.TVal = value; }
        }
        #endregion

    }
}
