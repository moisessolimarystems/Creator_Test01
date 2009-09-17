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
        string _licenseName;
        ProductLicenseTable _plRec;
        ProductProperty _product;
        Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.TProductLicenseType _productLicType;
        PermissionsTable _permissions;
        Solimar.Licensing.GlobalSoftwareSpec globalSwSpec;
        DateTime _currentExpirationDate;
        ProductLicenseState _plStatus;

        public enum ProductLicenseAttributes
        {
            Status,
            ExpirationDate,
            ProductVersion,
            ParentID,
            AppInstance
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
                    _licenseName = currentLicense.LicenseName;
            });           
            _plRec = plData;
            _product = product;
            _plStatus = (ProductLicenseState)plData.plState;
            ProductLicenseType = GetProductSpec(product.Product.productID.TVal).productLicType.TVal;  
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
        public string LicenseName
        {
            get { return _licenseName; }
            set { _licenseName = value; }
        }

        [Browsable(false)]
        public int Index
        {
            get { return _plRec.plIndex; }
            set { _plRec.plIndex = value; }
        }

        [Browsable(false)]
        public ProductProperty Product
        {
            get { return _product; }
            set { _product = value; }
        }

        [Browsable(false)]
        public bool IsActive
        {
            get { return (Status != ProductLicenseState.Deactivated) ? true : false; }
        }
        #endregion

        #region Browsable Properties
        [Category("Product License"), PropertyOrder(1)]
        [DisplayName("Product License Number")]
        [ReadOnly(true)]
        public string ID
        {
            get { return string.Format("{0}-{1}", LicenseName, Index); }
            set { _plRec.plID = value; }
        }

        [Category("Product License"), PropertyOrder(2)]
        [DisplayName("Status")]
        [ReadOnly(true)]
        public ProductLicenseState Status
        {
            get
            {
                SetReadOnlyAttribStatus(ProductLicenseAttributes.Status, !_permissions.pt_permanent_pwd.Value);
                if (LicenseName.Contains("T") || LicenseName.Contains("D") || LicenseName.Contains("F"))
                {
                    if (_permissions.pt_permanent_pwd.Value)
                        SetReadOnlyAttribStatus(ProductLicenseAttributes.Status, true);
                }
                return _plStatus;
            }
            set
            {
                //be able to change status to deactivated 
                DateTime? setDate = null;
                LicenseServerType lsType = LicenseServerType.Deactivated;
                Service<ICreator>.Use((client) =>
                {
                    lsType = (LicenseServerType)client.GetLicenseType(LicenseName);
                });
                //only set if value changed.
                if (!_plStatus.Equals(value))
                {   
                    switch (value)
                    {             
                        case ProductLicenseState.Licensed:
                            //was trial now licensed
                            //clear modules of non-defaults and set defaults           
                            //plstate is stored in terms of enums.productlicensestate
                            if (_plRec.plState == (byte)ProductLicenseState.Trial)
                                _product.SetTrialToLicensed(ID, lsType);
                            else if (_plRec.plState == (byte)ProductLicenseState.AddOn)
                                _product.SetAddOnToLicensed(ID, ParentID);
                            else
                                throw new Exception("invalid conversion");
                            setDate = (lsType != LicenseServerType.Subscription) ? null : ExpirationDate;
                            break;
                        case ProductLicenseState.Trial:
                            //was perm now trial
                            if (_plRec.plState == (byte)ProductLicenseState.Licensed)
                                _product.SetLicensedToTrial(ID);                                     
                            else
                                throw new Exception("Invalid Conversion");
                            setDate = CurrentExpirationDate;
                            break;
                        case ProductLicenseState.AddOn:
                            //need to allow for initial case and exclude changing from perm->addon, trial->addon
                            //initial case is always 0 -> perm
                            if (((!_plRec.ExpirationDate.HasValue &&
                                  _plRec.plState == (byte)ProductLicenseState.Licensed &&
                                  _plRec.plID != null)) ||
                               ((_plRec.ExpirationDate.HasValue &&
                                 _plRec.plState == (byte)ProductLicenseState.Trial)))
                                throw new Exception("Invalid Conversion");
                            setDate = CurrentExpirationDate;
                            break;                        
                        default:
                            break;
                    }
                    _plStatus = value;
                    if (_plStatus != ProductLicenseState.Deactivated)
                    {
                        _plRec.plState = (byte)_plStatus;
                        ExpirationDate = setDate;
                    }
                }
            }
        }

        [Browsable(false)]
        [Category("Product License"), PropertyOrder(3)]
        [DisplayName("Parent Product License Number")]
        [ReadOnly(true)]
        public string ParentID
        {
            get 
            {
                if (_plRec.ParentProductLicenseID != null)
                    SetBrowsableAttribStatus(ProductLicenseAttributes.ParentID, true);
                else
                    SetBrowsableAttribStatus(ProductLicenseAttributes.ParentID, false);
                return _plRec.ParentProductLicenseID; 
            }
            set { _plRec.ParentProductLicenseID = value; }
        }

        [EditorAttribute(typeof(DateTimeEditor), typeof(UITypeEditor))]
        [TypeConverter(typeof(DateTimeConverter))]
        [Category("Product License"), PropertyOrder(4)]
        [DisplayName("Expiration Date")]
        [ReadOnly(true)]
        public DateTime? ExpirationDate
        {
            get
            {
                SetReadOnlyAttribStatus(ProductLicenseAttributes.ExpirationDate, !_permissions.pt_permanent_pwd.Value);
                if (LicenseName.Contains("D") || LicenseName.Contains("F"))
                {
                    if (_permissions.pt_permanent_pwd.Value)
                        SetReadOnlyAttribStatus(ProductLicenseAttributes.ExpirationDate, true);
                }                
                if(_plRec.ExpirationDate.HasValue)
                    return _plRec.ExpirationDate.Value.ToLocalTime();
                return null;
            }
            set 
            {
                if (_plRec.plState == (byte)ProductLicenseState.Licensed &&
                    !_plRec.ExpirationDate.HasValue)
                    throw new Exception("Can't set expiration date for permanent type");
                if ((LicenseName.Contains("T") || _plRec.plState == (byte)ProductLicenseState.Trial) && !value.HasValue)                   
                    throw new Exception("Please set a valid expiration date");
                if (value.HasValue)
                {
                    _product.SetAllModulesExpDate(ID, new DateTime(value.Value.Year, value.Value.Month, value.Value.Day, 10, 0, 0).ToUniversalTime());
                    _plRec.ExpirationDate = new DateTime(value.Value.Year, value.Value.Month, value.Value.Day, 10, 0, 0).ToUniversalTime();
                }
                else
                {
                    _product.SetAllModulesExpDate(ID, value);
                    _plRec.ExpirationDate = value;
                }
            }
        }

        [Editor(typeof(MultilineStringEditor), typeof(UITypeEditor))]
        [Category("Product License"), PropertyOrder(5)]
        [DisplayName("Description")]
        public string Description
        {
            get { return _plRec.Description; }
            set { _plRec.Description = value; }
        }

        [Category("Product"), PropertyOrder(1)]
        [DisplayName("Name")]
        public String ProductName
        {
            get { return _product.Name; }
        }

        [Category("Product"), PropertyOrder(2)]
        [DisplayName("Version")]
        [TypeConverter(typeof(VersionConverter))]
        [ReadOnly(true)]
        public LicenseVersion ProductVersion
        {
            get 
            {
                SetReadOnlyAttribStatus(ProductLicenseAttributes.ProductVersion, !_permissions.pt_permanent_pwd.Value);
                if (LicenseName.Contains("T") || LicenseName.Contains("D") || LicenseName.Contains("F"))
                {
                    if (_permissions.pt_permanent_pwd.Value)
                        SetReadOnlyAttribStatus(ProductLicenseAttributes.ProductVersion, true);
                }    
                return _product.Version; 
            }
            set { _product.Version = value; }
        }

        [Browsable(false)]
        [Category("Product"), PropertyOrder(3)]
        [DisplayName("Product Connections")]
        public uint AppInstance
        {
            get 
            {
                SetReadOnlyAttribStatus(ProductLicenseAttributes.AppInstance, !_permissions.pt_permanent_pwd.Value);
                if (LicenseName.Contains("T") || LicenseName.Contains("D") || LicenseName.Contains("F"))
                {
                    if (_permissions.pt_permanent_pwd.Value)
                        SetReadOnlyAttribStatus(ProductLicenseAttributes.AppInstance, true);
                }   
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
                    if (module.moduleState.TVal != Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msAddOn)
                        module.moduleAppInstance.TVal = value;
                    else
                        module.moduleAppInstance.TVal = 0;
                }
                _product.Product.productAppInstance.TVal = value;
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
