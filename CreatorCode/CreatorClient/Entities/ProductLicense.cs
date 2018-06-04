using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.ComponentModel.Design;
using System.Drawing.Design;
using Solimar.Licensing.Attribs;
using System.Reflection;
using Client.Creator.CreatorService;
using Client.Creator.ServiceProxy;
using System.Globalization;
using System.Windows.Forms;
using System.Text.RegularExpressions;

namespace Client.Creator
{
    [TypeConverter(typeof(PropertySorterTypeConverter))]
    public class ProductLicense
    {
        #region Fields
        private const uint UNLIMITED = 999999;
        public enum ProductLicenseAttributes
        {
            Status,
            ExpirationDate,
            ProductVersion,
            ParentID,
            ProductConnection,
            ActivationTotal,
            ActivationAmountInDays
        }

        string _licenseServer;
        ProductLicenseTable _plRec;
        PermissionsTable _permissions;
        CommunicationLink _commLink;
        List<ModuleTable> _moduleList;
        LicenseVersion _version;
        byte _parentProductConnection;

        public static List<ProductLicenseType> _productLicenseTypes;

        #endregion

        #region Constructor
        public ProductLicense(ProductLicenseTable plData, PermissionsTable permissions)
        {
            _commLink = CreatorForm.s_CommLink;
            if (_productLicenseTypes == null)
                ListTypeConverter.SetList(ProductLicenseTypeList);            
            Permissions = permissions;
            ProductLicenseData = plData;
            _version = new LicenseVersion(_plRec.ProductVersion);
            _parentProductConnection = 0; //used by add-on product licenses to set module product connections.
            if(Status == ProductLicenseState.AddOn)
            {
                Service<ICreator>.Use((client) =>
                {
                    ProductLicenseTable plt = client.GetProductLicense(_plRec.ParentProductLicenseID);
                    _parentProductConnection = (plt != null) ? plt.ProductConnection : (byte)0;
                });
            }          
        }


        #endregion

        #region Properties
        #region NonBrowsable Properties

        [Browsable(false)]
        public byte ProductID
        {
            get 
            { 
                if(_plRec != null)
                    return _plRec.ProductID;
                return 0;
            }
        }

        [Browsable(false)]
        public ProductLicenseTable ProductLicenseData
        {
            get { return _plRec; }
            set 
            {   //Product connections can be modified if type is client, PLP, or an internal Solimar product license
                if (_commLink.GetProductSpec(value.ProductID).productLicType.TVal == Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.TProductLicenseType.pltClient || 
                   (value.plID.StartsWith("100")) ||
                   (ProductName.ToUpper().EndsWith("PLP")))
                    SetBrowsableAttribStatus(ProductLicenseAttributes.ProductConnection, true);
                else
                    SetBrowsableAttribStatus(ProductLicenseAttributes.ProductConnection, false);
                _plRec = value; 
            }
        }

        [Browsable(false)]
        public int ProductLicenseDatabaseID
        {
            get { return _plRec.ID; }
            set { _plRec.ID = value; }
        }

        [Browsable(false)]
        public DateTime CurrentExpirationDate
        {
            get { return new DateTime(DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day, 10, 0, 0); }
        }

        [Browsable(false)]
        [RefreshProperties(RefreshProperties.All)]
        public PermissionsTable Permissions
        {
            get { return _permissions; }
            set { _permissions = value; }
        }

        [Browsable(false)]
        public string LicenseServer
        {
            get
            {
                Service<ICreator>.Use((client) =>
                {
                    LicenseTable lt = client.GetLicenseByID(_plRec.LicenseID, false);
                    if (lt != null) _licenseServer = lt.LicenseName;
                });
                return _licenseServer;
            }
        }

        [Browsable(false)]
        public int Index
        {
            get { return _plRec.plIndex; }
            set { _plRec.plIndex = value; }
        }

        [Browsable(false)]
        public byte ParentProductConnection
        {
            get { return _parentProductConnection; }
            set { _parentProductConnection = value; }
        }

        [Browsable(false)]
        public string ParentID
        {
            get { return _plRec.ParentProductLicenseID; }
            set { _plRec.ParentProductLicenseID = value; }
        }

        [Browsable(false)]
        public string FullDescription
        {
            get { return _plRec.Description; }
        }

        [Browsable(false)]
        public bool IsActive
        {
            get { return _plRec.IsActive; }
        }
       
        [Browsable(false)]
        public bool IsExpired
        {
            get
            {
                if (ExpirationDate.HasValue)
                {
                    if (ExpirationDate.Value.CompareTo(CurrentExpirationDate) < 0)
                        return true;
                }
                return false;
            }
        }

        bool ValidModuleList()
        {
            if (_moduleList == null)
                return true;
            bool bRetVal = true;
            List<Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs> moduleSpecList = _commLink.GetModuleSpecForProductVersion(ProductID, ProductVersion.Major, ProductVersion.Minor);
            /*foreach (ModuleTable module in _moduleList)
            {
                if(moduleSpecList.Where(m => m.moduleID == module.ModID).Count() == 0)
                {
                    return false;
                }
            }*/
            foreach (Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs moduleSpec in moduleSpecList)
            {
                if (_moduleList.Where(m => m.ModID == moduleSpec.moduleID).Count() == 0)
                {
                    return false;
                }
            }
            return bRetVal;
        }

        [Browsable(false)]
        public List<ModuleTable> ModuleList
        {
            get
            {
                Service<ICreator>.Use((client) =>
                {
                    _moduleList = client.GetModulesByProductLicense(_plRec.plID, false);
                    if (!ValidModuleList())
                    {
                        List<ProductLicenseTable> productLicenses = client.GetProductLicensesByProduct(LicenseServer, ProductID);
                        UpdateModules(ProductVersion, productLicenses);
                        _moduleList = client.GetModulesByProductLicense(_plRec.plID, false);
                    }
                });
                return _moduleList;
            }
        }

        [Browsable(false)]
        public List<ModuleTable> AllModuleList
        {
            get
            {
                Service<ICreator>.Use((client) =>
                {                    
                    _moduleList = client.GetModulesByProductLicense(
                                                                    (ParentID != null) ? ParentID : ID, 
                                                                    true
                                                                    );
                    if (!ValidModuleList())
                    {
                        List<ProductLicenseTable> productLicenses = client.GetProductLicensesByProduct(LicenseServer, ProductID);
                        UpdateModules(ProductVersion, productLicenses);
                        _moduleList = client.GetModulesByProductLicense(_plRec.plID, false);
                    }
                });
                return _moduleList;
            }
        }

        public static List<string> ProductLicenseTypeList
        {
            get
            {
                if(_productLicenseTypes == null)
                {
                   Service<ICreator>.Use((client) =>
                    {
                        _productLicenseTypes = client.GetAllProductLicenseTypes();               
                    });
                }                
                return _productLicenseTypes.OrderBy(p => p.Type).Select(t => t.Type).ToList();
            }
        }

        #endregion

        #region Browsable Properties
        [Category("Product License"), PropertyOrder(1)]
        [DisplayName("Number")]
        [Description("Unique identifier for a product license.")]
        [ReadOnly(true)]
        public string ID
        {
            get { return _plRec.plID; }
            set { _plRec.plID = value; }
        }

        [Category("Product License"), PropertyOrder(2)]
        [DisplayName("Product")]
        [Description("Name of Solimar product associated with product license.")]
        public String ProductName
        {
            get
            {
                return _commLink.GetProductName(ProductID);
            }
        }

        [Category("Product License"), PropertyOrder(3)]
        [DisplayName("Version")]
        [Description("Solimar product version associated with product license.")]
        [TypeConverter(typeof(VersionConverter))]
        [ReadOnly(true)]
        public LicenseVersion ProductVersion
        {
            get
            {
                SetReadOnlyAttribStatus(ProductLicenseAttributes.ProductVersion, !_permissions.pt_version_pwd.Value);
                return new LicenseVersion(_plRec.ProductVersion);
            }
            set
            {
                if (IsActive)
                {
                    if (_version != value)
                    {
                        IList<ProductLicenseTable> productLicenses = null;
                        Service<ICreator>.Use((client) =>
                        {
                            if (value.ToString() != ProductVersion.ToString())
                                TransactionManager.CreateTransaction(TransactionType.Status,
                                                                      string.Empty,
                                                                      ID,
                                                                      string.Format("Edit {0} Version", ProductName),
                                                                      value.ToString(),
                                                                      ProductVersion.ToString());
                            productLicenses = client.GetProductLicensesByProduct(LicenseServer, ProductID);
                            if (productLicenses != null)
                            {
                                foreach (ProductLicenseTable pl in productLicenses)
                                {
                                    pl.ProductVersion = value.ToString();
                                    client.UpdateProductLicense(pl);
                                }
                            }
                            client.MarkDirty(LicenseServer);
                        });
                        UpdateModules(value, productLicenses); //needs to reload module list
                        _plRec.ProductVersion = value.ToString();
                        _version = value;
                    }
                }
            }
        }

        [Browsable(false)]
        [Category("Product License"), PropertyOrder(4)]
        [DisplayName("Product Connections")]
        [Description("Number of possible products able to use product license.")]
        public byte ProductConnection
        {
            get
            {
                SetReadOnlyAttribStatus(ProductLicenseAttributes.ProductConnection, !_permissions.pt_create_modify_key.Value);      
                return _plRec.ProductConnection; //should match module's app instance
            }
            set
            {
                //need product connection to be set for each product license
                //non-client modules - 1
                //add-on modules - always 0 unless total is 0 then 1
                //client modules - matches product
                //add-on modules - always 0 unless total is 0 then 1
                if (value > 0)
                {
                    if (IsActive)
                    {   //only set by client products since it is exposed only to them
                        if (ModuleList != null)
                        {
                            int baseValue;
                            foreach (ModuleTable module in _moduleList)
                            {   //needs to update database
                                //module.AppInstance = (Status != ProductLicenseState.AddOn ) ? value : (byte)0;
                                if (module.Value > 0)
                                {
                                    if (!_commLink.IsClientType(ProductID) && !_commLink.IsPLPModule(ProductID, module.ModID)) //Don't need to multiply non-client or PLP module values
                                    {
                                        baseValue = (module.AppInstance > 1) ? module.Value / module.AppInstance : module.Value;
                                        module.Value = (module.Value + ((value - module.AppInstance) * baseValue));
                                    }
                                    //if value = mod.appinstance then no need to change
                                    module.AppInstance = value; //multiply?
                                }
                            }
                        }
                        _plRec.ProductConnection = value;
                        Service<ICreator>.Use((client) =>
                        {
                            client.UpdateAllModules(_moduleList);
                            ProductLicenseTable plt = client.GetProductLicense(ID);
                            if (plt != null)
                            {
                                TransactionManager.CreateTransaction(TransactionType.Status,
                                                                      string.Empty,
                                                                      ID,
                                                                      string.Format("Edit {0} Product Connection", ProductName),
                                                                      value.ToString(),
                                                                      plt.ProductConnection.ToString());
                                plt.ProductConnection = _plRec.ProductConnection;
                                client.UpdateProductLicense(plt);
                                client.MarkDirty(LicenseServer);
                            }
                        });
                    }
                }
                else
                    throw new Exception("Product Connection must be a value greater then 0.");
            }
        }

        [Category("Product License"), PropertyOrder(5)]
        [TypeConverter(typeof(StateEnumComboBoxConverter))]
        [DisplayName("Status")]
        [Description("State of product license.")]
        [ReadOnly(true)]
        public ProductLicenseState Status
        {
            get
            {
                SetReadOnlyAttribStatus(ProductLicenseAttributes.Status, !_permissions.pt_permanent_pwd.Value);
                return (ProductLicenseState)_plRec.plState;
            }
            set
            {
                //be able to change status to deactivated 
                //only set if value changed.
                if (IsActive)
                {
                    if (!(_plRec.plState == (byte)value))
                    {
                        DialogResult dlgResult = DialogResult.No;
                        if (value != ProductLicenseState.AddOn)
                            dlgResult = MessageBox.Show(string.Format("Please Verify Status Change from {0} to {1}.", Status.ToString(), value.ToString()), "Confirmation", MessageBoxButtons.YesNo, MessageBoxIcon.Information);
                        if (dlgResult == DialogResult.Yes || value == ProductLicenseState.AddOn)
                        {
                            string errorMsg = string.Empty;
                            switch (value)
                            {
                                case ProductLicenseState.Licensed: //was trial now licensed       
                                    bool bReset = true;
                                    if (!IsTrialModuleValues()) //clear modules of non-defaults and set defaults    
                                        if (MessageBox.Show("Keep the current module values?", "Confirmation", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
                                            bReset = false;
                                    if (!SetTrialToLicensed(bReset))
                                        errorMsg = "Failed to change status from trial to licensed.";
                                    break;
                                case ProductLicenseState.Trial:  //was perm now trial
                                    if (!SetLicensedToTrial())
                                        errorMsg = "Failed to change status from licensed.";
                                    break;
                                case ProductLicenseState.AddOn:
                                    if (ParentID == null)
                                        errorMsg = string.Format("Status change from {0} to {1} is not valid.", ((ProductLicenseState)_plRec.plState).ToString(), value.ToString());
                                    break;
                                case ProductLicenseState.LicensedAddOn:
                                    if (!SetAddonToLicensed())
                                        errorMsg = "Failed to change status from add-on to licensed.";
                                    break;
                                default:
                                    break;
                            }
                            if (errorMsg.Length == 0)
                            {
                                _plRec.plState = (byte)value;
                                if (value == ProductLicenseState.Licensed)
                                {
                                    if (!(ProductName.Contains("Test")) && HasHardwareToken() && ParentID == null)
                                        if (MessageBox.Show("Clear Expiration Date?", "Confirmation", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
                                            ExpirationDate = null;
                                }
                                else
                                    ExpirationDate = CurrentExpirationDate.AddDays(30);
                                Service<ICreator>.Use((client) =>
                                {
                                    ProductLicenseTable plt = client.GetProductLicense(ID);
                                    if (plt != null)
                                    {
                                        TransactionManager.CreateTransaction(TransactionType.Status,
                                                                              string.Empty,
                                                                              ID,
                                                                              string.Format("Edit {0} Status", ProductName),
                                                                              ((ProductLicenseState)value).ToString(),
                                                                              ((ProductLicenseState)plt.plState).ToString());
                                        plt.plState = _plRec.plState;
                                        client.UpdateProductLicense(plt);
                                        client.MarkDirty(LicenseServer);
                                    }
                                });
                            }
                            else
                                throw new Exception(errorMsg);
                        }
                    }
                }
            }
        }

        [EditorAttribute(typeof(DateTimeEditor), typeof(UITypeEditor))]
        [TypeConverter(typeof(DateTimeConverter))]
        [Category("Product License"), PropertyOrder(6)]
        [DisplayName("Expiration Date")]
        [Description("The date the product license will expire. The maximum allowed expiration date is one year.")]
        [ReadOnly(true)]
        public DateTime? ExpirationDate
        {
            get
            {
                SetReadOnlyAttribStatus(ProductLicenseAttributes.ExpirationDate, !_permissions.pt_extension_pwd.Value);
                if (_plRec.ExpirationDate.HasValue)
                    return _plRec.ExpirationDate.Value.ToLocalTime();
                return null;
            }
            set
            {
                if (IsActive)
                {
                    if (_plRec.ExpirationDate != value)
                    {
                        string errorMsg = string.Empty;
                        if (_plRec.plState == (byte)ProductLicenseState.Licensed &&
                            !value.HasValue && !HasHardwareToken())
                        {
                            errorMsg = "Can't remove expiration date for non-hardware token validation.";
                        }
                        if ((ProductName.Contains("Test") || _plRec.plState == (byte)ProductLicenseState.Trial) && !value.HasValue)
                        {
                            errorMsg = "Please set a valid expiration date.";
                        }
                        if (value.HasValue && value.Value.CompareTo(DateTime.Today.AddDays(1).AddMonths(6).AddYears(1)) > 0)
                        {
                            errorMsg = string.Format("{0} exceeds the expiration date limit of 18 months.", value.Value);
                        }
                        if (errorMsg.Length == 0)
                        {
                            Service<ICreator>.Use((client) =>
                            {
                                ProductLicenseTable plt = client.GetProductLicense(ID);
                                if (plt != null)
                                {
                                    string storedValue = "None", newValue = "None";
                                    if (ExpirationDate.HasValue)
                                        storedValue = ExpirationDate.Value.ToShortDateString();
                                    if (value.HasValue)
                                        newValue = value.Value.ToShortDateString();
                                    TransactionManager.CreateTransaction(TransactionType.ExpirationDate,
                                                      string.Empty,
                                                      ID,
                                                      string.Format("Modify {0} Expiration Date", ProductName),
                                                      newValue,
                                                      storedValue);
                                    if (value.HasValue)
                                        plt.ExpirationDate = new DateTime(value.Value.Year, value.Value.Month, value.Value.Day, 10, 0, 0).ToUniversalTime();
                                    else
                                        plt.ExpirationDate = value;
                                    client.UpdateProductLicense(plt);
                                    client.MarkDirty(LicenseServer);
                                    _plRec.ExpirationDate = plt.ExpirationDate;
                                }
                            });
                        }
                        else
                        {
                            throw new Exception(errorMsg);
                        }
                    }
                }
            }
        }

        [Category("Product License"), PropertyOrder(7)]
        [DisplayName("Extensions")]
        [Description("The number of extensions given.")]
        [ReadOnly(true)]
        public byte Extension
        {
            get { return _plRec.Extensions; }
        }

        [Category("Product License"), PropertyOrder(8)]
        [TypeConverter(typeof(ListTypeConverter))]
        [DisplayName("Type")]
        [Description("Type")]
        public string Type
        {
            get { return GetProductLicenseType(_plRec.TypeID); }
            set 
            {
                if (IsActive)
                {
                    if (Type != value)
                    {
                        Service<ICreator>.Use((client) =>
                        {
                            ProductLicenseTable plt = client.GetProductLicense(ID);
                            if (plt != null)
                            {
                                TransactionManager.CreateTransaction(TransactionType.Type,
                                                                      string.Empty,
                                                                      ID,
                                                                      string.Format("Edit {0} Type", ProductName),
                                                                      value,
                                                                      Type);
                                _plRec.TypeID = GetProductLicenseTypeID(value);
                                plt.TypeID = _plRec.TypeID;
                                client.UpdateProductLicense(plt);
                            }
                        });
                    }
                }
            }
        }

        [Category("Product License Activations"), PropertyOrder(1)]
        [TypeConverter(typeof(ByteConverter))]
        [DisplayName("Total")]
        [Description("The number of activations for the product license. The max value allowed for the activation total is 254.")]
        [RefreshProperties(RefreshProperties.All)]
        public byte ActivationTotal
        {
            get
            {
                SetReadOnlyAttribStatus(ProductLicenseAttributes.ActivationTotal, !_permissions.pt_extension_pwd.Value);
                return _plRec.Activations;
            }
            set
            {
                if (IsActive)
                {
                    if (value >= AppConstants.MaxActivations)
                        throw new Exception(string.Format("{0} exceeds the max value for activation total"));
                    else
                    {
                        Service<ICreator>.Use((client) =>
                        {
                            if (_plRec.Activations == 0 && value > 0)
                            {
                                IList<ProductLicenseTable> plList = null;
                                plList = client.GetProductLicenses(LicenseServer, false);
                                if (HasHardwareToken() && 
                                    plList.Where(c => c.IsActive && c.Activations > 0).Count() >= AppConstants.MaxProductLicenses)
                                    throw new Exception(string.Format("License Server ({0}) has reached the maximum number of product licenses with activations allowed a hardware token.\nPlease remove another product license with activations before adding activations to product license ({1})", LicenseServer, ID));
                            }
                            _plRec.Activations = value;
                            ProductLicenseTable plt = client.GetProductLicense(ID);
                            if (plt != null)
                            {
                                TransactionManager.CreateTransaction(TransactionType.ActivationTotal,
                                                                     string.Empty,
                                                                     ID,
                                                                     string.Format("Modify {0} Activation Total", ProductName),
                                                                     value.ToString(),
                                                                     plt.Activations.ToString());
                                plt.Activations = _plRec.Activations;
                                client.UpdateProductLicense(plt);
                                client.MarkDirty(LicenseServer);
                            }
                        });
                    }
                }
            }
        }

        [Category("Product License Activations"), PropertyOrder(2)]
        [TypeConverter(typeof(ByteConverter))]
        [DisplayName("Amount In Days")]
        [Description("The amount in days per activation for the product license. The max value allowed for the amount in days is 170.")]
        [RefreshProperties(RefreshProperties.All)]
        public byte ActivationAmountInDays
        {
            get
            {
                SetReadOnlyAttribStatus(ProductLicenseAttributes.ActivationAmountInDays, !_permissions.pt_extension_pwd.Value);
                return _plRec.ActivationAmount;
            }
            set
            {
                if (IsActive)
                {
                    try
                    {
                        if (value >= AppConstants.MaxActivationAmountInDays)
                        {
                            throw new Exception(string.Format("{0} exceeds the max value for the amount in days value per activation.", value));
                        }
                        else
                        {
                            Service<ICreator>.Use((client) =>
                            {
                                if (_plRec.ActivationAmount == 0 && value > 0)
                                {
                                    IList<ProductLicenseTable> plList = null;
                                    plList = client.GetProductLicenses(LicenseServer, false);
                                    if (plList.Where(c => c.IsActive && c.Activations > 0).Count() >= AppConstants.MaxProductLicenses && ActivationTotal == 0)
                                        throw new Exception(string.Format("License Server ({0})} has reached the maximum number of product licenses with activations allowed.\nPlease remove another product license with activations before adding activations to product license ({1})", LicenseServer, ID));
                                }
                                _plRec.ActivationAmount = value;
                                ProductLicenseTable plt = client.GetProductLicense(ID);
                                if (plt != null)
                                {
                                    TransactionManager.CreateTransaction(TransactionType.ActivationAmount,
                                                                         string.Empty,
                                                                         ID,
                                                                         string.Format("Modify {0} Activation Amount in Days", ProductName),
                                                                         value.ToString(),
                                                                         plt.ActivationAmount.ToString());
                                    plt.ActivationAmount = value;
                                    client.UpdateProductLicense(plt);
                                    client.MarkDirty(LicenseServer);
                                }
                            });
                        }
                    }
                    catch (Exception ex)
                    {
                        throw new Exception("Invalid value for Activation Amount");
                    }
                }
            }
        }

        [Editor(typeof(MultilineStringEditor), typeof(UITypeEditor))]
        [Category("Product License Notes"), PropertyOrder(1)]
        [DisplayName("Notes")]
        [Description("User notes for product license.")]
        public string Description
        {
            get
            {
                if (_plRec.Description.Contains("|"))
                {                    //split return first half
                    string[] notes = _plRec.Description.Split("|".ToCharArray());
                    return notes[0];
                }
                return _plRec.Description;
            }
            set
            {
                if (_plRec.Description != null && _plRec.Description.Contains("|"))
                {
                    string[] notes = _plRec.Description.Split("|".ToCharArray());
                    notes[0] = value;
                    _plRec.Description = string.Format("{0}|{1}", notes[0], notes[1]);
                }
                else
                    _plRec.Description = value;
                Service<ICreator>.Use((client) =>
                {
                    ProductLicenseTable plt = client.GetProductLicense(ID);
                    if (plt != null)
                    {
                        plt.Description = _plRec.Description;
                        client.UpdateProductLicense(plt);
                    }
                });
            }
        }

        [Editor(typeof(MultilineStringEditor), typeof(UITypeEditor))]
        [Category("Product License Notes"), PropertyOrder(2)]
        [DisplayName("Additional Notes")]
        [Description("User notes for product license.")]
        public string AdditionalDescription
        {
            get
            {
                if (_plRec.Description.Contains("|"))
                {                    //split return first half
                    string[] notes = _plRec.Description.Split("|".ToCharArray());
                    return notes[1];
                }
                return string.Empty;
            }
            set
            {
                if (_plRec.Description.Contains("|"))
                {
                    string[] notes = _plRec.Description.Split("|".ToCharArray());
                    notes[1] = value;
                    _plRec.Description = string.Format("{0}|{1}", notes[0], notes[1]);
                }
                else
                {
                    if (value.Length > 0)
                    {
                        _plRec.Description = string.Format("{0}|{1}", _plRec.Description, value);
                    }
                }
                Service<ICreator>.Use((client) =>
                {
                    ProductLicenseTable plt = client.GetProductLicense(ID);
                    if (plt != null)
                    {
                        plt.Description = _plRec.Description;
                        client.UpdateProductLicense(plt);
                    }
                });
            }
        }
        #endregion
        #endregion

        #region Methods

        public static string GetProductLicenseType(int typeID)
        {
            if (_productLicenseTypes == null)
                ListTypeConverter.SetList(ProductLicenseTypeList);  
            ProductLicenseType type = _productLicenseTypes.Single(t => t.ID == typeID);
            return (type != null) ? type.Type : "";
        }

        public static int GetProductLicenseTypeID(string type)
        {
            if (_productLicenseTypes == null)
                ListTypeConverter.SetList(ProductLicenseTypeList);  
            ProductLicenseType plType = _productLicenseTypes.Single(t => t.Type == type);
            return (plType != null) ? plType.ID : 1;
        }

        private void UpdateModules(LicenseVersion version, IList<ProductLicenseTable> pltList)
        {
            if (IsActive)
            {
                Service<ICreator>.Use((client) =>
                {
                    List<ModuleTable> dbModuleList = client.GetAllActiveModulesByProduct(LicenseServer, ProductID);
                    RemoveDeprecatedModules(version, dbModuleList);
                    AddIntroducedModules(version, dbModuleList, pltList);
                });
            }
        }

        private void RemoveDeprecatedModules(LicenseVersion version, List<ModuleTable> dbModuleList)
        {
            Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs moduleSpec = null;
            List<ModuleTable> removeModuleList = new List<ModuleTable>();
            foreach (ModuleTable module in dbModuleList)
            {
                bool bDeprecated = false;
                moduleSpec = CreatorForm.s_CommLink.GetModuleSpec(ProductID, module.ModID);
                if (moduleSpec != null)
                {
                    if (moduleSpec.moduleVersionDeprecated_Major.TVal > 0)
                    {
                        if (version.Major > moduleSpec.moduleVersionDeprecated_Major)
                            bDeprecated = true;
                        else if (version.Major == moduleSpec.moduleVersionDeprecated_Major)
                        {
                            if (version.Minor >= moduleSpec.moduleVersionDeprecated_Minor)
                                bDeprecated = true;
                        }
                    }
                    //version major < introduced
                    if (version.Major < moduleSpec.moduleVersionIntroduced_Major)
                        bDeprecated = true;
                    else if (version.Major == moduleSpec.moduleVersionIntroduced_Major)
                    {
                        if (version.Minor < moduleSpec.moduleVersionIntroduced_Minor)
                            bDeprecated = true;
                    }
                }
                if (bDeprecated) removeModuleList.Add(module);
            }
            if (removeModuleList.Count > 0)
            {
                Service<ICreator>.Use((client) =>
                {
                    foreach (ModuleTable mt in removeModuleList)
                    {
                        client.DeleteModule(mt);
                    }
                    if (removeModuleList.Count > 0)
                        client.MarkDirty(LicenseServer);
                });
            }
        }

        private void AddIntroducedModules(LicenseVersion version, List<ModuleTable> dbModuleList, IList<ProductLicenseTable> pltList)
        {
            Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.Lic_ModuleSoftwareSpecAttribsMap moduleSpecList = CreatorForm.s_CommLink.GetModuleSpecList(ProductID);
            List<ModuleTable> addModuleList = new List<ModuleTable>();
            List<ModuleTable> removeModuleList = new List<ModuleTable>();

            foreach (Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs moduleSpec in moduleSpecList.TVal.Values)
            {
                if (pltList != null)
                {
                    foreach (ProductLicenseTable plt in pltList.Where(p => p.IsActive)) //Add introduced modules to active product licenses.
                    {
                        bool bIntroduced = false, bDeprecated = false;
                        if (dbModuleList.Where(m => (m.ModID == (short)moduleSpec.moduleID.TVal) && (m.ProductLicenseID == plt.ID)).Count() == 0)
                        {
                            if (moduleSpec.moduleVersionDeprecated_Major.TVal > 0)
                            {
                                if (version.Major > moduleSpec.moduleVersionDeprecated_Major)
                                    bDeprecated = true;
                                else if (version.Major == moduleSpec.moduleVersionDeprecated_Major)
                                {
                                    if (version.Minor >= moduleSpec.moduleVersionDeprecated_Minor)
                                        bDeprecated = true;
                                }
                            }
                            if (version.Major > moduleSpec.moduleVersionIntroduced_Major)
                                bIntroduced = true;
                            else if (version.Major == moduleSpec.moduleVersionIntroduced_Major)
                            {
                                if (version.Minor >= moduleSpec.moduleVersionIntroduced_Minor)
                                    bIntroduced = true;
                            }
                        }
                        if (bIntroduced && !bDeprecated)
                        {
                            uint moduleValue = (plt.plState == (byte)ProductLicenseState.Trial) ? moduleSpec.moduleTrialLicense.TVal : moduleSpec.moduleDefaultLicense.TVal;                      
                            ModuleTable mt = new ModuleTable();
                            mt.ModID = (short)moduleSpec.moduleID.TVal;
                            mt.Value = (int)moduleValue;
                            mt.AppInstance = (moduleValue > 0) ? ProductConnection : (byte)0;
                            mt.ProductLicenseID = plt.ID;
                            addModuleList.Add(mt);
                        }
                    }
                }
            }            
            if (addModuleList.Count > 0)
            {
                Service<ICreator>.Use((client) =>
                {
                    client.CreateAllModules(addModuleList);
                    client.MarkDirty(LicenseServer);
                });
            }
        }

        public bool HasHardwareToken()
        {
            bool bRetVal = false;
            Service<ICreator>.Use((client) =>
            {
                TokenTable tt = client.GetTokenByLicenseName(LicenseServer, (byte)Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID);
                if (tt != null)
                    bRetVal = true;
            });
            return bRetVal;
        }

        public bool SetTrialToLicensed(bool bResetModules)
        {
            bool bRetVal = true;
            if (bResetModules)
            {
                Service<ICreator>.Use((client) =>
                {
                    List<ModuleTable> mtList = new List<ModuleTable>();
                    int multiplier = _commLink.IsClientType(ProductID) ? 1 : ProductConnection;
                    foreach (ModuleTable module in ModuleList)
                    {
                        uint moduleValue = _commLink.GetDefaultModuleValue(ProductID, module.ModID);
                        ModuleTable mt = client.GetModule(ID, module.ModID);
                        if (mt != null)
                        {
                            mt.Value = (int)(moduleValue * multiplier);
                            if (moduleValue == 0) mt.AppInstance = 0;
                            mtList.Add(mt);
                        }
                        else
                            bRetVal = false;
                    }
                    if (bRetVal)
                        client.UpdateAllModules(mtList);
                });
            }
            return bRetVal;
        }

        //Need to not touch PLP modules, they need to stay fixed. Question is when there are multiple. 
        public bool SetLicensedToTrial()
        {
            bool bRetVal = true;
            foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in CreatorForm.s_CommLink.m_softwareSpec.productSpecMap.TVal.Values)
            {
                if (productSpec.productID.TVal == ProductID)
                {
                    Service<ICreator>.Use((client) =>
                    {
                        List<ModuleTable> mtList = new List<ModuleTable>();
                        int multiplier = _commLink.IsClientType(ProductID) ? 1 : ProductConnection;
                        foreach (ModuleTable module in ModuleList)
                        {
                            ModuleTable mt = client.GetModule(ID, module.ModID);                            
                            if (mt != null)
                            {
                                mt.Value = (int)(_commLink.GetModuleTrialValue(ProductID, module.ModID) * multiplier);                                
                                mt.AppInstance = (byte)((mt.Value == 0) ? 0 : ProductConnection);
                                mtList.Add(mt);
                            }
                            else
                                bRetVal = false;
                        }
                        if (bRetVal)
                            client.UpdateAllModules(mtList);
                    });
                    break;
                }
            }
            return bRetVal;
        }
        //issue need to add -> addon modules to licensed modules -> do within product license
        //delete add-on modules -> do within product license
        //delete add-on product license -> needs to be done outside of product license by form or manager
        public bool SetAddonToLicensed()
        {
            bool bRetVal = true;
            string previousValue, moduleName;
            Service<ICreator>.Use((client) =>
            {   //Retrieve only modules belonging to Parent PL
                List<ModuleTable> mtList = client.GetModulesByProductLicense(ParentID, false);
                List<ModuleTable> updateModuleList = new List<ModuleTable>();
                if (ModuleList != null)
                {   //add-on module list = _moduleList, parent module list = mtlist
                    foreach (ModuleTable module in _moduleList)
                    {
                        if (module.Value > 0)
                        {
                            ModuleTable mt = mtList.Where(m => m.ModID == module.ModID).First();
                            if (mt != null)
                            {
                                if (module.Value > 0)
                                {
                                    previousValue = mt.Value.ToString();
                                    mt.Value += module.Value;
                                    //bump product connection of parent module if it is currently 0
                                    if (_commLink.IsClientType(ProductID))
                                    {    //client add-on must add product connections to modules and product license
                                        mt.AppInstance += module.AppInstance;
                                    }
                                    else
                                    {
                                        if (mt.Value > 0 && mt.AppInstance == 0)
                                            mt.AppInstance = 1;
                                    }
                                    updateModuleList.Add(mt);
                                    moduleName = _commLink.GetModuleName(ProductID, mt.ModID);    
                                    //Create transaction for add-on PL modules being merged into parent PL 
                                    TransactionManager.CreateTransaction(TransactionType.Module,
                                                                          string.Empty,
                                                                          ParentID,
                                                                          string.Format("Merge {0} - {1} [{2}]", ProductName, moduleName, ID),
                                                                          mt.Value.ToString(),
                                                                          previousValue);
                                }
                            }
                        }
                    }
                    if (updateModuleList.Count > 0)
                    {   //update product license product connections if it is a client type
                        if (_commLink.IsClientType(ProductID))
                        {
                            ProductLicenseTable plt = client.GetProductLicense(ParentID);
                            if (plt != null)
                            {
                                plt.ProductConnection = updateModuleList[0].AppInstance;
                                client.UpdateProductLicense(plt);
                            }
                        }
                        client.UpdateAllModules(updateModuleList);
                    }
                    client.DeleteAllModulesByProductLicense(ProductLicenseDatabaseID);
                    //update product license if add-on is client
                }
            });
            return bRetVal;
        }

        public uint GetTotalAddOnModuleAppInstance(short modID)
        {
            uint totalValue = 0;
            Service<ICreator>.Use((client) =>
            {
                string stdProductLicense = (Status != ProductLicenseState.AddOn) ? ID : ParentID;
                List<ModuleTable> moduleList = client.GetModulesByProductLicense(stdProductLicense, false);
                foreach (ModuleTable module in moduleList.Where(m => m.ProductLicenseID != ProductLicenseDatabaseID && m.ModID == modID))
                {
                    totalValue += module.AppInstance;
                }
            });
            return totalValue;
        }

        public int GetTotalModuleAppInstance(short modID)
        {
            uint totalValue = 0;
            Service<ICreator>.Use((client) =>
            {
                string stdProductLicense = (Status != ProductLicenseState.AddOn) ? ID : ParentID;
                List<ModuleTable> moduleList = client.GetModulesByProductLicense(stdProductLicense, false);
                foreach (ModuleTable module in moduleList.Where(m => m.ModID == modID))
                {
                    totalValue += module.AppInstance;
                }
            });
            return (int)totalValue;
        }

        public void DecrementAddOnModuleAppInstance(short modID)
        {
            Service<ICreator>.Use((client) =>
            {
                string stdProductLicense = (Status != ProductLicenseState.AddOn) ? ID : ParentID;
                List<ModuleTable> moduleList = client.GetModulesByProductLicense(stdProductLicense, false);
                //Get AddOn modules with modID and set them to 0;
                foreach (ModuleTable module in moduleList.Where(m => m.ProductLicenseID != ProductLicenseDatabaseID && m.ModID == modID))
                {
                    module.AppInstance = 0;
                    client.UpdateModule(module);
                }
            });
        }
        public void IncrementAddOnModuleAppInstance(short modID)
        {
            Service<ICreator>.Use((client) =>
            {
                string stdProductLicense = (Status != ProductLicenseState.AddOn) ? ID : ParentID;
                List<ModuleTable> moduleList = client.GetModulesByProductLicense(stdProductLicense, false);
                List<int> updatedModuleList = new List<int>(); //list to keep track of one set of modules to increment product connection.
                //increment should occur for only 1 add-on module set not all add-on module sets
                foreach (ModuleTable module in moduleList.Where(m => m.ProductLicenseID != ProductLicenseDatabaseID && m.ModID == modID))
                {
                    if (!updatedModuleList.Exists(u => u == module.ModID))
                    {
                        module.AppInstance = 1;
                        client.UpdateModule(module);
                        updatedModuleList.Add(module.ModID);
                    }
                }
            });
        }

        public uint GetTotalModuleValue(short moduleID)
        {
            uint totalValue = 0;
            Service<ICreator>.Use((client) =>
            {
                string plID = (ParentID != null) ? ParentID : ID;
                totalValue = (uint)client.GetTotalModuleValue(plID, ProductID, moduleID);
            });
            return totalValue;
        }

        public uint GetAvailableModuleUnits(Module module)
        {
            if (module.UnlimitedValue == 0)
                return UNLIMITED;
            //GetTotalModuleValue contacts database - SPDE calls this 50+ times
            //need to better way to get total module value
            //total value = primary pl + any add on pl modules
            //should not contact database
            uint multiplier = (uint)((_commLink.IsClientType(ProductID) || Status == ProductLicenseState.AddOn) ? 1 : ProductConnection);
            return ((module.UnlimitedValue * multiplier) - GetTotalModuleValue(module.ID));
        }

        public uint GetAvailableModuleUnits(Module module, uint totalModuleValue)
        {
            if (module.UnlimitedValue == 0)
                return UNLIMITED;
            //GetTotalModuleValue contacts database - SPDE calls this 50+ times
            //need to better way to get total module value
            //total value = primary pl + any add on pl modules
            //should not contact database
            uint multiplier = (uint)((_commLink.IsClientType(ProductID) || Status == ProductLicenseState.AddOn) ? 1 : ProductConnection);
            return ((module.UnlimitedValue * multiplier) - totalModuleValue);
        }

        public bool SetModule(Module module)
        {
            bool bSuccess = false;
            int totalModAppInstance = GetTotalModuleAppInstance(module.ID); //Total = module prodConnect + add-on module prodConnect
            if (module.Value > 0)
            {
                if (totalModAppInstance == 0)
                {//need to use parent product connections if add-on and total product connections are 0
                    //use parent product PC if add-on                     
                    module.AppInstance = (Status == ProductLicenseState.AddOn) ?  ParentProductConnection : ProductConnection; //always > 0 except for add-on, then use parent product connection
                    /*if (ProductConnection > 0)
                        module.AppInstance = (_commLink.IsClientType(ProductID)) ? ProductConnection : (byte)1;
                    else
                        module.AppInstance = 1;*/
                }
                else //productconnections >= 1
                {
                    if (_commLink.IsClientType(ProductID)) //client type product connection can be > 1
                    {
                        module.AppInstance = ProductConnection;
                    }
                    else
                    {
                        if (totalModAppInstance > 1)
                        {
                            module.AppInstance = (byte)((Status == ProductLicenseState.AddOn) ? 0 : ProductConnection);
                        }
                        else
                        {
                            if (Status == ProductLicenseState.AddOn)
                            {    //client + add-on should set app instance to ProductConnection
                                if (module.AppInstance != 1) //only set add-on product connection to 0 if parent product connection is >=1
                                    module.AppInstance = 0;
                            }
                            else
                            {
                                module.AppInstance = ProductConnection;
                                DecrementAddOnModuleAppInstance(module.ID); //decrement any add-on module product connection that may be > 0
                            }
                        }
                    }
                }
            }
            else
            {
                //if decrement std then increment add-on
                //if decrement add-on don't decrement appinstance unless it is 1
                module.AppInstance = 0;
                if (Status != ProductLicenseState.AddOn)
                {
                    if (GetTotalAddOnModuleAppInstance(module.ID) == 0)
                        IncrementAddOnModuleAppInstance(module.ID);
                }
            }
            Service<ICreator>.Use((client) =>
            {
                ModuleTable mt = client.GetModule(ID, module.ID);
                if (mt != null)
                {
                    string previousValue = (module != null) ? mt.Value.ToString() : "0";
                    TransactionManager.CreateTransaction(TransactionType.Module,
                                                          string.Empty,
                                                          ID,
                                                          string.Format("Modify {0} - {1}", ProductName, module.Name),
                                                          module.Value.ToString(),
                                                          previousValue);
                    mt.Value = (module.Value > 0) ? module.Value : 0;
                    mt.AppInstance = (module.Value > 0) ? module.AppInstance : (byte)0;
                    client.UpdateModule(mt);
                    client.MarkDirty(LicenseServer);
                    bSuccess = true;
                }
            });
            return bSuccess;
        }

        public bool IsAllowedRemoveModule(short modID)
        {
            if (ModuleList != null)
            {
                foreach (ModuleTable module in _moduleList)
                {
                    if (module.ModID == modID)
                    {
                        if (module.Value > 0)
                        {
                            if (Status == ProductLicenseState.Licensed)
                            {
                                if (!_commLink.IsDefaultModule(ProductID, modID))
                                    return true;
                            }
                            else
                            {
                                if (Status == ProductLicenseState.AddOn)
                                    return true;
                            }
                        }
                    }
                }
            }
            return false;
        }

        #region Helper Methods

        bool IsTrialModuleValues()
        {
            bool bRetVal = true;
            uint moduleValue;
            List<ModuleTable> mtList = new List<ModuleTable>();
            foreach (ModuleTable module in ModuleList)
            {
                moduleValue = _commLink.GetModuleTrialValue(ProductID, module.ModID);
                if (module.Value != moduleValue)
                {
                    bRetVal = false;
                    break;
                }
                
            }
            return bRetVal;
        }

        public bool CanIncrementModule(Module module)
        {
            return GetAvailableModuleUnits(module) > 0 && !_commLink.IsDefaultModule(ProductID, module.ID);
        }

        public bool CanDecrementModule(Module module)
        {
            return (module.Value > 0) && !_commLink.IsDefaultModule(ProductID, module.ID);
        }

        public void SetReadOnlyAttribStatus(ProductLicenseAttributes property, bool value)
        {
            PropertyDescriptor descriptor = TypeDescriptor.GetProperties(this.GetType())[property.ToString()];
            ReadOnlyAttribute attrib = (ReadOnlyAttribute)descriptor.Attributes[typeof(ReadOnlyAttribute)];
            FieldInfo isReadOnly = attrib.GetType().GetField("isReadOnly", BindingFlags.NonPublic | BindingFlags.Instance);
            isReadOnly.SetValue(attrib, value);
        }

        public void SetBrowsableAttribStatus(ProductLicenseAttributes property, bool value)
        {
            PropertyDescriptor descriptor = TypeDescriptor.GetProperties(this.GetType())[property.ToString()];
            BrowsableAttribute attrib = (BrowsableAttribute)descriptor.Attributes[typeof(BrowsableAttribute)];
            FieldInfo isBrowsable = attrib.GetType().GetField("browsable", BindingFlags.Instance | BindingFlags.NonPublic);
            isBrowsable.SetValue(attrib, value);
        }
        #endregion

        #endregion
    }

    #region Helper Classes
    [TypeConverter(typeof(VersionConverter))]
    public class LicenseVersion
    {
        private uint _major = 0;
        private uint _minor = 0;

        public LicenseVersion() { }
        public LicenseVersion(string licenseVersion)
        {
            string[] split = licenseVersion.Split(".".ToCharArray());
            if (split.Count() > 0)
            {
                _major = UInt32.Parse(split[0]);
                _minor = UInt32.Parse(split[1]);
            }
        }
        public LicenseVersion(uint major, uint minor) { _major = major; _minor = minor; }

        public LicenseVersion(int decimalVersion)
        {
            string hexVersion, minorVersion, majorVersion;
            if (decimalVersion >= 4096)
            {
                hexVersion = string.Format("{0:x}", decimalVersion);
                //hex 0A01
                minorVersion = hexVersion.Substring(hexVersion.Length - 3, 2); //01
                majorVersion = hexVersion.Substring(0, hexVersion.Length - (minorVersion.Length + 1)); //0A

                _major = UInt32.Parse(majorVersion, System.Globalization.NumberStyles.HexNumber); //UInt32.Parse(majorVersion);
                _minor = UInt32.Parse(minorVersion, System.Globalization.NumberStyles.HexNumber);  //UInt32.Parse(minorVersion);
            }
        }

        public uint Major
        {
            get { return _major; }
            set { _major = value; }
        }

        public uint Minor
        {
            get { return _minor; }
            set { _minor = value; }
        }

        public static bool IsValid(string version)
        {
            Regex reg = new Regex(@"\d+\.\d+$");
            if (!reg.IsMatch(version))
                return false;
            return true;
        }

        public override bool Equals(Object lv)
        {
            LicenseVersion _lv = lv as LicenseVersion;
            if (_lv != null)
            {
                if (_lv.Major == _major && _lv.Minor == _minor)
                    return true;
            }
            return false;
        }

        public override string ToString()
        {
            return string.Format("{0}.{1}", _major, _minor);
        }
    }

    internal class ByteConverter : TypeConverter
    {
        //private bool isNull = true;
        public override object ConvertFrom(ITypeDescriptorContext context,
            System.Globalization.CultureInfo culture, object value)
        {
            if (value.ToString() == string.Empty)
                return null;
            else
            {
                try 
                { 
                    return Byte.Parse((string)value); 
                }
                catch (FormatException ex) 
                { 
                    throw new Exception(string.Format("Invalid number format for {0}", context.PropertyDescriptor.DisplayName)); 
                }
            }
        }
        
        public override object ConvertTo(ITypeDescriptorContext context,
            System.Globalization.CultureInfo culture, object value, Type destinationType)
        {            
            if (value == null)
            {
                return 0;
            }
            return ((Byte)value).ToString();            
        }
        public override bool CanConvertFrom(ITypeDescriptorContext context, Type sourceType)
        {
            if (sourceType == typeof(string))
            {
                return true;
            }
            return base.CanConvertFrom(context, sourceType);
        }
        public override bool CanConvertTo(ITypeDescriptorContext context, Type destinationType)
        {
            if (destinationType == typeof(Byte))
            {
                return true;
            }
            return base.CanConvertTo(context, destinationType);
        }
    }

    internal class DateTimeConverter : TypeConverter
    {
        //private bool isNull = true;
        public override object ConvertFrom(ITypeDescriptorContext context,
            System.Globalization.CultureInfo culture, object value)
        {
            if (value.ToString() == string.Empty)            
                return null;            
            else
            {
                int nvResult;
                if (!(context.Instance as ProductLicense).ExpirationDate.HasValue)
                    throw new Exception("Unable to set expiration date for Licensed type");
                if (Int32.TryParse((string)value, out nvResult))  //user enter number of days 
                    return (context.Instance as ProductLicense).ExpirationDate.Value.AddDays(Int32.Parse((string)value));
                else 
                {   //user entered a new date value
                    try { return DateTime.Parse((string)value); }
                    catch (FormatException ex) { throw new Exception("Invalid Date Format"); }
                }        
            }
        }

        public override object ConvertTo(ITypeDescriptorContext context,
            System.Globalization.CultureInfo culture, object value, Type destinationType)
        {
            if (value == null)
            {
                return string.Empty;
            }
            return (value as DateTime?).Value.ToLongDateString();
        }
        public override bool CanConvertFrom(ITypeDescriptorContext context, Type sourceType)
        {
            if (sourceType == typeof(string))
            {
                return true;
            }
            return base.CanConvertFrom(context, sourceType);
        }
        public override bool CanConvertTo(ITypeDescriptorContext context, Type destinationType)
        {
            if (destinationType == typeof(DateTime))
            {
                return true;
            }
            return base.CanConvertTo(context, destinationType);
        }
    }

    internal class VersionConverterTest : ExpandableObjectConverter
    {
        public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destType)
        {
            if (destType == typeof(string) && value is LicenseVersion)
            {
                LicenseVersion version = value as LicenseVersion;
                return version.Major + "." + version.Minor;
            }
            return base.ConvertTo(context, culture, value, destType);
        }
    }

    internal class VersionConverter : TypeConverter
    {
        public override bool CanConvertTo(ITypeDescriptorContext context, Type destinationType)
        {
            if (destinationType == typeof(LicenseVersion))
                return true;            
            return base.CanConvertTo(context, destinationType);
        }
        public override bool CanConvertFrom(ITypeDescriptorContext context, Type t)
        {
            if (t == typeof(string))
                return true;
            return base.CanConvertFrom(context, t);
        }

        public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destType)
        {
            if (value == null)
            {
                return "0.0";
            }
            return value.ToString();
        }

        public override Object ConvertFrom(ITypeDescriptorContext context, System.Globalization.CultureInfo info, Object value)
        {
            //Debug.WriteLine("ConvertFrom: ");
            if (value is String)
            {
                Regex reg = new Regex(@"\d+\.\d+$");
                string version = value.ToString();
                if (!reg.IsMatch(version))
                {
                    throw new Exception("Please enter a valid version!");
                }
                return new LicenseVersion(version);
            }
            return base.ConvertFrom(context, info, value);
        }
    }

    internal class StateEnumComboBoxConverter : EnumConverter
    {
        private Type _enumType;
        /// Initializing instance
        /// type Enum
        ///this is only one function, that you must
        ///to change. All another functions for enums
        ///you can use by Ctrl+C/Ctrl+V

        public StateEnumComboBoxConverter(Type type)
        : base(type)
        {
            _enumType = type;
        }

        public override bool CanConvertTo(ITypeDescriptorContext context,Type destType)
        {
            return destType == typeof(string);
        }

        public override object ConvertTo(ITypeDescriptorContext context,CultureInfo culture,object value, Type destType)
        {
            FieldInfo fi = _enumType.GetField(Enum.GetName(_enumType, value));
            DescriptionAttribute dna =(DescriptionAttribute)Attribute.GetCustomAttribute(fi, typeof(DescriptionAttribute)); 
            if (dna != null)
                return dna.Description;
            else
                return value.ToString();
        }

        public override bool CanConvertFrom(ITypeDescriptorContext context,Type srcType)
        {
            return srcType == typeof(string);
        } 

        public override object ConvertFrom(ITypeDescriptorContext context,CultureInfo culture,object value)
        {
            foreach (FieldInfo fi in _enumType.GetFields())
            {
                DescriptionAttribute dna = (DescriptionAttribute)Attribute.GetCustomAttribute(fi, typeof(DescriptionAttribute)); 
                if ((dna != null) && ((string)value == dna.Description))
                return Enum.Parse(_enumType, fi.Name);
            }
            return Enum.Parse(_enumType, (string)value);
        }

        public override bool GetStandardValuesSupported(ITypeDescriptorContext context)
        {
            return true;
        }


        public override StandardValuesCollection GetStandardValues(ITypeDescriptorContext context)
        {
            //Allow trial, licensed or add-on, licensed
            ProductLicense plp = context.Instance as ProductLicense;
            List<ProductLicenseState> stateList = new List<ProductLicenseState>();
            if (plp.ParentID == null)
            {
                stateList.Add(ProductLicenseState.Trial);
                stateList.Add(ProductLicenseState.Licensed);
            }
            else
            {
                stateList.Add(ProductLicenseState.AddOn);
                stateList.Add(ProductLicenseState.LicensedAddOn);
            }
            return new StandardValuesCollection(stateList);
        }
    }

    public class ListTypeConverter : TypeConverter
    {
        private static List<string> m_list = new List<string>();
        public override bool GetStandardValuesSupported(ITypeDescriptorContext context)
        {
            return true;
        }

        public override bool GetStandardValuesExclusive(ITypeDescriptorContext context)
        {
            return true;
        }
        private StandardValuesCollection GetValues()
        {
            return new StandardValuesCollection(m_list);
        }
        public static void SetList(List<string> list)
        {
            m_list = list;
        }

        public override StandardValuesCollection GetStandardValues(ITypeDescriptorContext context)
        {
            return GetValues();
        }
    }
    #endregion
}
