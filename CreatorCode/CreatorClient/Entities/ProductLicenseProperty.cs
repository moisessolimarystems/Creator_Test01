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

namespace Client.Creator
{
    [TypeConverter(typeof(PropertySorterTypeConverter))]
    public class ProductLicenseProperty
    {
        string _licenseServer;
        ProductLicenseTable _plRec;
        ProductProperty _product;
        Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.TProductLicenseType _productLicType;
        PermissionsTable _permissions;
        Solimar.Licensing.GlobalSoftwareSpec globalSwSpec;
        DateTime _currentExpirationDate;
        int _productLicenseDataBaseID;        

        public enum ProductLicenseAttributes
        {
            Status,
            ExpirationDate,
            ProductVersion,
            ParentID,
            AppInstance,
            ActivationTotal,
            ActivationAmountInDays
        }

        public ProductLicenseProperty()
        {
            _plRec = new ProductLicenseTable();
            _product = new ProductProperty();          
            globalSwSpec = new Solimar.Licensing.GlobalSoftwareSpec();
        }

        public ProductLicenseProperty(ProductLicenseTable plData, ProductProperty product, PermissionsTable permissions)
        {
            Permissions = permissions;
            globalSwSpec = new Solimar.Licensing.GlobalSoftwareSpec();
            Service<ICreator>.Use((client) => 
            {
                LicenseTable currentLicense = client.GetLicenseByID(plData.LicenseID, false);
                if(currentLicense != null)
                    _licenseServer = currentLicense.LicenseName;
            });           
            _plRec = plData;
            _product = product;
            _productLicenseDataBaseID = plData.ID;
            ProductLicenseType = GetProductSpec(product.Product.productID.TVal).productLicType.TVal;
        }

        public bool HasHardwareToken()
        {
            bool bRetVal = false;
            Service<ICreator>.Use((client) =>
            {
                TokenTable tt = client.GetTokenByLicenseName(_licenseServer, (byte)Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID);
                if (tt != null)
                    bRetVal = true;
            });
            return bRetVal;
        }
        //fill product entity with default modules
        //only trial/addon order types
        public void InitializeProductEntity()
        {
            Lic_PackageAttribs.Lic_ModuleInfoAttribs module;
            Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec = GetProductSpec(Product.ID);
            foreach(Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs moduleSpec in productSpec.moduleSpecMap.TVal.Values)
            {
                module = new Lic_PackageAttribs.Lic_ModuleInfoAttribs();
                module.moduleID.TVal = moduleSpec.moduleID.TVal;
                if (Status == ProductLicenseState.Trial)
                {
                    module.moduleValue.TVal = moduleSpec.moduleTrialLicense.TVal;
                    module.moduleAppInstance.TVal = 1;
                }
                if (module.moduleValue.TVal > 0)
                    Product.ModuleList.TVal.Add(module);
                ModuleTable mt = new ModuleTable();
                mt.ModID = (short)module.moduleID.TVal;
                mt.Value = (short)module.moduleValue.TVal;
                mt.AppInstance = (short)module.moduleAppInstance.TVal;
                mt.ProductLicenseID = ProductLicenseDatabaseID;
                Service<ICreator>.Use((client) =>
                {
                    client.CreateModule(mt);
                });
            }
        }
        public uint GetTotalAddOnModuleAppInstance(uint modID)
        {
            uint totalValue = 0;
            string stdProductLicense = "";
            IList<string> addOnOrders = null;
            LicenseTable lt = null;
            Lic_PackageAttribs licPackage = new Lic_PackageAttribs();
            Service<ICreator>.Use((client) =>
            {
                lt = client.GetLicenseByName(LicenseServer, false);
                licPackage.Stream = lt.LicenseInfo;
                stdProductLicense = (_product.Product.productState.TVal != Lic_PackageAttribs.Lic_ProductInfoAttribs.TProductState.psAddOn) ? ID : ParentID;
                addOnOrders = client.GetAddOnProductLicenses(stdProductLicense);
            });
            foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs product in licPackage.licLicenseInfoAttribs.TVal.productList.TVal)
            {
                if (addOnOrders.Contains(product.contractNumber.TVal))
                {
                    foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs modRec in product.moduleList.TVal)
                    {
                        if (modRec.moduleID.TVal.Equals(modID))
                        {
                            totalValue += modRec.moduleAppInstance.TVal;
                            break;
                        }
                    }
                }
            }
            return totalValue;
        }
        public int GetTotalModuleAppInstance(uint modID)
        {
            uint totalValue = 0;
            LicenseTable lt = null;
            Lic_PackageAttribs licPackage = new Lic_PackageAttribs();
            Service<ICreator>.Use((client) =>
            {
                lt = client.GetLicenseByName(LicenseServer, false);
                licPackage.Stream = lt.LicenseInfo;
            });
            foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs product in licPackage.licLicenseInfoAttribs.TVal.productList.TVal)
            {
                if (product.productID.TVal == _product.Product.productID.TVal)
                {
                    foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs modRec in product.moduleList.TVal)
                    {
                        if (modRec.moduleID.TVal.Equals(modID))
                        {
                            totalValue += modRec.moduleAppInstance.TVal;
                            break;
                        }
                    }
                }
            }
            return (int)totalValue;
        }
        public void DecrementAddOnModuleAppInstance(uint modID)
        {
            IList<string> addOnOrders = null;
            Service<ICreator>.Use((client) =>
            {
                addOnOrders = client.GetAddOnProductLicenses(ID);
                if (addOnOrders != null)
                {
                    if (addOnOrders.Count > 0)
                    {
                        LicenseTable lt = null;
                        Lic_PackageAttribs licPackage = new Lic_PackageAttribs();
                        lt = client.GetLicenseByName(LicenseServer, false);
                        licPackage.Stream = lt.LicenseInfo;
                        foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs product in licPackage.licLicenseInfoAttribs.TVal.productList.TVal)
                        {
                            if (addOnOrders.Contains(product.contractNumber.TVal))
                            {
                                foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs storedMod in product.moduleList.TVal)
                                {
                                    if (storedMod.moduleID.TVal == modID)
                                    {
                                        storedMod.moduleAppInstance.TVal = 0;
                                        ModuleTable mt = client.GetModule(ID, (int)storedMod.moduleID.TVal);
                                        mt.AppInstance = 0;
                                        client.UpdateModule(mt);
                                    }
                                }
                            }
                        }
                        lt.LicenseInfo = licPackage.Stream;
                        client.UpdateLicense(lt, true);
                    }
                }
            });
        }
        public void IncrementAddOnModuleAppInstance(uint modID)
        {
            //increment first add-on found
            IList<string> addOnOrders = null;
            Service<ICreator>.Use((client) =>
            {
                addOnOrders = client.GetAddOnProductLicenses(ID);
                if (addOnOrders != null)
                {
                    if (addOnOrders.Count > 0)
                    {
                        LicenseTable lt = null;
                        Lic_PackageAttribs licPackage = new Lic_PackageAttribs();
                        lt = client.GetLicenseByName(LicenseServer, false);
                        licPackage.Stream = lt.LicenseInfo;
                        foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs product in licPackage.licLicenseInfoAttribs.TVal.productList.TVal)
                        {
                            if (addOnOrders.Contains(product.contractNumber.TVal))
                            {
                                foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs storedMod in product.moduleList.TVal)
                                {
                                    if (storedMod.moduleID.TVal == modID)
                                    {
                                        if (storedMod.moduleValue.TVal > 0)
                                        {
                                            storedMod.moduleAppInstance.TVal = 1;
                                            ModuleTable mt = client.GetModule(ID, (int)storedMod.moduleID.TVal);
                                            mt.AppInstance = 1;
                                            client.UpdateModule(mt);
                                        }
                                        break;
                                    }
                                }
                                break;
                            }
                        }
                        lt.LicenseInfo = licPackage.Stream;
                        client.UpdateLicense(lt, true);
                    }
                }
            });
        }
        public Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs GetProductSpec(uint productID)
        {
            foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in globalSwSpec.softwareSpec.productSpecMap.TVal.Values)
            {
                if (productSpec.productID.TVal.Equals(productID))
                {
                    return productSpec;
                }
            }
            return null;
        }
        public void SetModule(ModuleProperty module)
        {
            Product.SetModule(ID, module);
        }
        public bool RemoveModule(ModuleProperty module)
        {
            return Product.RemoveModule(ID, module);
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

        #region Properties

        #region NonBrowsable Properties
        [Browsable(false)]
        public ProductLicenseTable ProductLicenseData
        {
            get { return _plRec; }
            set { _plRec = value; }
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
            get { return new DateTime(DateTime.Today.Year, DateTime.Today.Month, DateTime.Today.Day, 10, 0, 0); }
        }

        [Browsable(false)]
        [RefreshProperties(RefreshProperties.All)]
        public Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.TProductLicenseType ProductLicenseType
        {
            get {  return _productLicType; }
            set 
            {
                if (value == Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.TProductLicenseType.pltClient)                   
                    SetBrowsableAttribStatus(ProductLicenseAttributes.AppInstance, true);
                else
                    SetBrowsableAttribStatus(ProductLicenseAttributes.AppInstance, false);
                _productLicType = value; 
            }
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
            get { return _licenseServer; }
            set { _licenseServer = value; }
        }

        [Browsable(false)]
        public int Index
        {
            get { return _plRec.plIndex; }
            set { _plRec.plIndex = value; }
        }

        [Browsable(false)]
        public string ParentID
        {
            get { return _plRec.ParentProductLicenseID;  }
            set { _plRec.ParentProductLicenseID = value; }
        }

        [Browsable(false)]
        public ProductProperty Product
        {
            get { return _product; }
            set { _product = value; }
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
        
        #endregion

        #region Browsable Properties
        [Category("Product License"), PropertyOrder(1)]
        [DisplayName("Number")]
        [Description("Unique identifier for a product license.")]
        [ReadOnly(true)]
        public string ID
        {
            get { return Lic_LicenseInfoAttribsHelper.GenerateProductLicenseName(LicenseServer, Index); }
            set
            {
                _plRec.plID = value;
                _product.Product.contractNumber.TVal = value;
            }
        }

        [Category("Product License"), PropertyOrder(2)]
        [DisplayName("Product")]
        [Description("Name of Solimar product associated with product license.")]
        public String ProductName
        {
            get 
            {
                //SetBrowsableAttribStatus(ProductLicenseAttributes.ActivationTotal, _product.Name.Contains("Test"));
                //SetBrowsableAttribStatus(ProductLicenseAttributes.ActivationAmountInDays, _product.Name.Contains("Test"));
                return _product.Name; 
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
                SetReadOnlyAttribStatus(ProductLicenseAttributes.ProductVersion, !_permissions.pt_permanent_pwd.Value);
                return _product.Version;
            }
            set { _product.Version = value; }
        }

        [Browsable(false)]
        [Category("Product License"), PropertyOrder(4)]
        [DisplayName("Product Connections")]
        [Description("Number of possible products able to use product license.")]
        public uint AppInstance
        {
            get
            {
                SetReadOnlyAttribStatus(ProductLicenseAttributes.AppInstance, !_permissions.pt_permanent_pwd.Value);
                return _product.Product.productAppInstance.TVal;
            }
            set
            {
                //need appinstance to be set for each product license
                //non-client modules - 1
                //add-on modules - always 0 unless total is 0 then 1
                //client modules - matches product
                //add-on modules - always 0 unless total is 0 then 1
                foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs module in _product.Product.moduleList.TVal)
                {
                    if (_product.Product.productState.TVal != Lic_PackageAttribs.Lic_ProductInfoAttribs.TProductState.psAddOn)
                        module.moduleAppInstance.TVal = value;
                    else
                        module.moduleAppInstance.TVal = 0;
                }
                _product.Product.productAppInstance.TVal = value;
            }
        }

        [Category("Product License"), PropertyOrder(5)]
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
                DateTime? setDate = null;
                //only set if value changed.
                if (!(_plRec.plState == (byte)value))
                {
                    System.Windows.Forms.DialogResult dlgResult = System.Windows.Forms.DialogResult.No;
                    if(value != ProductLicenseState.AddOn)
                        dlgResult = System.Windows.Forms.MessageBox.Show(string.Format("Please Verify Status Change from {0} to {1}.", Status.ToString(), value.ToString()), "Confirmation", System.Windows.Forms.MessageBoxButtons.YesNo, System.Windows.Forms.MessageBoxIcon.Information);
                    if(dlgResult == System.Windows.Forms.DialogResult.Yes || value == ProductLicenseState.AddOn)
                    {
                        string errorMsg = "";
                        switch (value)
                        {
                            case ProductLicenseState.Licensed:
                                //was trial now licensed
                                //clear modules of non-defaults and set defaults           
                                //plstate is stored in terms of enums.productlicensestate
                                if (_plRec.plState == (byte)ProductLicenseState.Trial)
                                    _product.SetTrialToLicensed(ID);
                                else if (_plRec.plState != (byte)ProductLicenseState.AddOn)
                                    errorMsg = string.Format("{0} to {1} is not valid.", ((ProductLicenseState)_plRec.plState).ToString(), value.ToString());
                                break;
                            case ProductLicenseState.Trial:
                                //was perm now trial
                                if (_plRec.plState == (byte)ProductLicenseState.Licensed)
                                    _product.SetLicensedToTrial(ID);
                                else
                                    errorMsg = string.Format("{0} to {1} is not valid.", ((ProductLicenseState)_plRec.plState).ToString(), value.ToString());
                                break;
                            case ProductLicenseState.AddOn:
                                //need to allow for initial case and exclude changing from perm->addon, trial->addon
                                //initial case is always 0 -> perm
                                if (((!_plRec.ExpirationDate.HasValue &&
                                      _plRec.plState == (byte)ProductLicenseState.Licensed &&
                                      _plRec.plID != null)) ||
                                   ((_plRec.ExpirationDate.HasValue &&
                                     _plRec.plState == (byte)ProductLicenseState.Trial)))
                                    errorMsg = string.Format("{0} to {1} is not valid.", ((ProductLicenseState)_plRec.plState).ToString(), value.ToString());
                                break;
                            default:
                                break;
                        }
                        if (errorMsg.Length == 0)
                        {
                            _plRec.plState = (byte)value;
                            if (value == ProductLicenseState.Licensed)
                            {
                                if(!(Product.Name.Contains("Test")) && HasHardwareToken() && ParentID == "")
                                    if (System.Windows.Forms.MessageBox.Show("Clear Expiration Date?", "Confirmation", System.Windows.Forms.MessageBoxButtons.YesNo, System.Windows.Forms.MessageBoxIcon.Question) == System.Windows.Forms.DialogResult.Yes)
                                        ExpirationDate = null;
                            }
                            else
                                ExpirationDate = CurrentExpirationDate;
                        }
                        else
                            throw new Exception(errorMsg);
                            //System.Windows.Forms.MessageBox.Show(errorMsg, "Status Error", System.Windows.Forms.MessageBoxButtons.OK, System.Windows.Forms.MessageBoxIcon.Error);
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
                SetReadOnlyAttribStatus(ProductLicenseAttributes.ExpirationDate, !_permissions.pt_permanent_pwd.Value);             
                if(_plRec.ExpirationDate.HasValue)
                    return _plRec.ExpirationDate.Value.ToLocalTime();
                return null;
            }
            set 
            {
                string errorMsg = "";
                if (_plRec.plState == (byte)ProductLicenseState.Licensed &&
                    !value.HasValue && !HasHardwareToken()) 
                    errorMsg = "Can't remove expiration date for non-hardware token validation.";
                if ((Product.Name.Contains("Test") || _plRec.plState == (byte)ProductLicenseState.Trial) && !value.HasValue)                   
                    errorMsg = "Please set a valid expiration date";
                if (value.HasValue && value.Value.CompareTo(DateTime.Today.AddDays(1).AddYears(1)) > 0)
                    errorMsg = string.Format("{0} exceeds the expiration date limit of one year.", value.Value);
                if (errorMsg.Length == 0)
                {
                    if (ActivationTotal > 0)
                    {
                        if (value != _plRec.ExpirationDate)
                            _product.Product.bActivationOverrideCurrent.TVal = true;
                        _product.Product.bUseActivations.TVal = true;
                    }
                    if (value.HasValue)
                    {
                        _plRec.ExpirationDate = new DateTime(value.Value.Year, value.Value.Month, value.Value.Day, 10, 0, 0).ToUniversalTime();
                        _product.Product.expirationDate.TVal = _plRec.ExpirationDate.Value;
                        _product.Product.bUseExpirationDate.TVal = true;
                    }
                    else
                    {
                        _plRec.ExpirationDate = value;
                        _product.Product.expirationDate.TVal = new DateTime(1900, 1, 1);
                        _product.Product.bUseExpirationDate.TVal = false;
                    }
                }
                else
                {
                    throw new Exception(errorMsg);
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

        [Category("Product License Activations"), PropertyOrder(1)]
        [DisplayName("Total")]
        [Description("The number of activations for the product license. The max value allowed for the activation total is 254.")]
        [RefreshProperties(RefreshProperties.All)]
        public uint ActivationTotal
        {
            get             
            {
                return _product.Product.activationTotal.TVal; 
            }
            set 
            {
                if (value >= AppConstants.MaxActivations)
                    throw new Exception(string.Format("{0} exceeds the max value for activation total"));
                    //System.Windows.Forms.MessageBox.Show(string.Format("{0} exceeds the max value for activation total", value), "Extension Error", System.Windows.Forms.MessageBoxButtons.OK, System.Windows.Forms.MessageBoxIcon.Error);
                else
                {
                    if (_product.Product.activationTotal.TVal == 0 && value > 0)
                    {
                        IList<ProductLicenseTable> plList = null;
                        Service<ICreator>.Use((client) =>
                        {
                            plList = client.GetProductLicenses(LicenseServer);
                        });
                        if (plList.Where(c => c.IsActive && c.Activations > 0).Count() >= AppConstants.MaxProductLicenses)
                            throw new Exception(string.Format("License Server ({0}) has reached the maximum number of product licenses with activations allowed.\nPlease remove another product license with activations before adding activations to product license ({1})", LicenseServer, ID));
                    }
                    if (value > 0)
                        _product.Product.bUseActivations.TVal = true;
                    if (value != _product.Product.activationTotal.TVal)
                        _product.Product.bActivationOverrideCurrent.TVal = true;
                    _product.Product.activationTotal.TVal = value;
                    //set to force license to use activation exp date instead of regular exp date.
                }
            }
        }
        [Category("Product License Activations"), PropertyOrder(2)]
        [DisplayName("Amount In Days")]
        [Description("The amount in days per activation for the product license. The max value allowed for the amount in days is 170.")]
        [RefreshProperties(RefreshProperties.All)]
        public uint ActivationAmountInDays
        {
            get 
            {
                return _product.Product.activationAmountInDays.TVal; 
            }
            set 
            {
                if (value >= AppConstants.MaxActivationAmountInDays)
                {
                    throw new Exception(string.Format("{0} exceeds the max value for the amount in days value per activation.", value));
                    //System.Windows.Forms.MessageBox.Show(string.Format("{0} exceeds the max value for the amount in days value per activation.", value), "Extension Error", System.Windows.Forms.MessageBoxButtons.OK, System.Windows.Forms.MessageBoxIcon.Error);
                }
                else
                {
                    if (_product.Product.activationAmountInDays.TVal == 0 && value > 0)
                    {
                        IList<ProductLicenseTable> plList = null;
                        Service<ICreator>.Use((client) =>
                        {
                            plList = client.GetProductLicenses(LicenseServer);
                        });
                        if (plList.Where(c => c.IsActive && c.Activations > 0).Count() >= AppConstants.MaxProductLicenses)
                            throw new Exception(string.Format("License Server ({0})} has reached the maximum number of product licenses with activations allowed.\nPlease remove another product license with activations before adding activations to product license ({1})", LicenseServer, ID));
                    }
                    if (value != _product.Product.activationTotal.TVal)
                        _product.Product.bActivationOverrideCurrent.TVal = true;
                    _product.Product.activationAmountInDays.TVal = value;
                    if (value > 0)
                        _product.Product.bUseActivations.TVal = true;
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
                if(_plRec.Description.Contains("|"))
                {
                    //split return first half
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
                    _plRec.Description = string.Format("{0}|{1}", notes[0], notes[1]) ;
                }
                else
                    _plRec.Description = value; 
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
                {
                    //split return first half
                    string[] notes = _plRec.Description.Split("|".ToCharArray());
                    return notes[1];
                }
                return "";         
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
                        _plRec.Description = string.Format("{0}|{1}",_plRec.Description,value);
                    }
                }
            }
        }     
        #endregion

        #endregion
    }


    class DateTimeConverter : TypeConverter
    {
        //private bool isNull = true;
        public override object ConvertFrom(ITypeDescriptorContext context,
            System.Globalization.CultureInfo culture, object value)
        {
            if (value.ToString() == "")            
                return null;            
            else
            {
                int nvResult;
                if (!(context.Instance as ProductLicenseProperty).ExpirationDate.HasValue)
                    throw new Exception("Unable to set expiration date for permanent type");
                if (Int32.TryParse((string)value, out nvResult))  //user enter number of days 
                    return (context.Instance as ProductLicenseProperty).ExpirationDate.Value.AddDays(Int32.Parse((string)value));
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
                return "";
            }
            return (value as DateTime?).Value.ToShortDateString();
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
}
