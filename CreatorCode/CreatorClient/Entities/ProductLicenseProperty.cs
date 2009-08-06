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
        OrderTable _orderRec;
        ProductProperty _product;
        Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.TProductLicenseType _productLicType;
        PermissionsTable _permissions;
        Solimar.Licensing.GlobalSoftwareSpec globalSwSpec;
        DateTime _currentExpirationDate;

        public enum OrderAttributes
        {
            OrderStatus,
            ExpirationDate,
            ProductVersion,
            AppInstance
        }

        public ProductLicenseProperty()
        {
            _orderRec = new OrderTable();
            _product = new ProductProperty();
            globalSwSpec = new Solimar.Licensing.GlobalSoftwareSpec();
        }

        public ProductLicenseProperty(OrderTable orderData, ProductProperty product, PermissionsTable permissions)
        {
            Permissions = permissions;
            globalSwSpec = new Solimar.Licensing.GlobalSoftwareSpec();
            Service<ICreator>.Use((client) => 
            {
                LicenseTable currentLicense = client.GetLicenseByID(orderData.LicenseID, false);
                if(currentLicense != null)
                    _licenseName = currentLicense.LicenseName;
            });           
            _orderRec = orderData;
            _product = product;
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

        public void SetReadOnlyOrderAttribStatus(OrderAttributes property, bool value)
        {
            PropertyDescriptor descriptor = TypeDescriptor.GetProperties(this.GetType())[property.ToString()];
            ReadOnlyAttribute attrib = (ReadOnlyAttribute)descriptor.Attributes[typeof(ReadOnlyAttribute)];
            FieldInfo isReadOnly = attrib.GetType().GetField("isReadOnly", BindingFlags.NonPublic | BindingFlags.Instance);
            isReadOnly.SetValue(attrib, value);
        }

        public void SetBrowsableOrderAttribStatus(OrderAttributes property, bool value)
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
            get 
            {
                return _productLicType; 
            }
            set 
            {
                if (value == Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.TProductLicenseType.pltClient)
                    SetBrowsableOrderAttribStatus(OrderAttributes.AppInstance, true);
                else
                    SetBrowsableOrderAttribStatus(OrderAttributes.AppInstance, false);
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
        public int OrderIndex
        {
            get { return _orderRec.OrderIndex; }
            set { _orderRec.OrderIndex = value; }
        }

        [Browsable(false)]
        public ProductProperty Product
        {
            get { return _product; }
            set { _product = value; }
        }
        #endregion

        #region Browsable Properties
        [Category("Product License"), PropertyOrder(1)]
        [DisplayName("Product License Number")]
        [ReadOnly(true)]
        public string OrderNumber
        {
            get { return string.Format("{0}-{1}", LicenseName, OrderIndex); }
            set { _orderRec.OrderNumber = value; }
        }

        [Category("Product License"), PropertyOrder(2)]
        [DisplayName("Status")]
        [ReadOnly(true)]
        public Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState OrderStatus
        {
            get 
            {
                SetReadOnlyOrderAttribStatus(OrderAttributes.OrderStatus, !_permissions.pt_permanent_pwd.Value);
                if (LicenseName.Contains("T") || LicenseName.Contains("U"))
                {
                    if (_permissions.pt_permanent_pwd.Value)
                        SetReadOnlyOrderAttribStatus(OrderAttributes.OrderStatus, true);
                }
                return (Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState)_orderRec.OrderState; 
            }
            set 
            {
                DateTime? setDate = null;
                //only set if value changed.
                if (!_orderRec.OrderState.Equals((byte)value))
                {
                    switch (value)
                    {
                        case Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msPerm:                           
                        //was trial now perm    
                        //clear modules of non-defaults and set defaults             
                            if (_orderRec.OrderState == (byte)Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msTrial)
                                _product.SetTrialToPerm(OrderNumber);
                            else if (_orderRec.OrderState == (byte)Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msAddOn)
                                _product.SetAddOnToPerm(OrderNumber, ParentOrderNumber);
                            else
                                throw new Exception("invalid conversion");
                            setDate = null;
                            break;
                        case Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msTrial:
                            //was perm now trial
                            if (_orderRec.OrderState == (byte)Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msPerm)
                            {
                                _product.SetPermToTrial(OrderNumber);
                                if(_licenseName.Contains("U01")) //standard - subscription
                                    SetReadOnlyOrderAttribStatus(OrderAttributes.OrderStatus, true);
                            }
                            else
                                throw new Exception("Invalid Conversion");
                            setDate = CurrentExpirationDate;
                            break;
                        case Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msAddOn:                                                                    
                            //need to allow for initial case and exclude changing from perm->addon, trial->addon
                            //initial case is always 0 -> perm
                            if (((!_orderRec.ExpirationDate.HasValue &&
                                  _orderRec.OrderState == (byte)Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msPerm && 
                                  _orderRec.OrderNumber != null)) ||
                               ((_orderRec.ExpirationDate.HasValue && 
                                 _orderRec.OrderState == (byte)Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msTrial)))                                
                                throw new Exception("Invalid Conversion");
                            setDate = CurrentExpirationDate;
                            break;
                        default:
                            break;
                    }                   
                    _orderRec.OrderState = (byte)value;
                    ExpirationDate = setDate;
                }
            }
        }

        [Category("Product License"), PropertyOrder(3)]
        [DisplayName("Parent Product License Number")]
        [ReadOnly(true)]
        public string ParentOrderNumber
        {
            get { return _orderRec.ParentOrderNumber; }
            set { _orderRec.ParentOrderNumber = value; }
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
                SetReadOnlyOrderAttribStatus(OrderAttributes.ExpirationDate, !_permissions.pt_extension_pwd.Value);
                if(_orderRec.ExpirationDate.HasValue)
                    return _orderRec.ExpirationDate.Value.ToLocalTime();
                return null;
            }
            set 
            {
                if (_orderRec.OrderState == (byte)Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msPerm &&
                    !_orderRec.ExpirationDate.HasValue)
                    throw new Exception("Can't set expiration date for permanent type");
                if ((LicenseName.Contains("T") || _orderRec.OrderState == (byte)Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msTrial) && !value.HasValue)                   
                    throw new Exception("Please set a valid expiration date");
                if (value.HasValue)
                {
                    _product.SetAllModulesExpDate(OrderNumber, new DateTime(value.Value.Year, value.Value.Month, value.Value.Day, 10, 0, 0).ToUniversalTime());
                    _orderRec.ExpirationDate = new DateTime(value.Value.Year, value.Value.Month, value.Value.Day, 10, 0, 0).ToUniversalTime();
                }
                else
                {
                    _product.SetAllModulesExpDate(OrderNumber, value);
                    _orderRec.ExpirationDate = value;
                }
            }
        }

        [Editor(typeof(MultilineStringEditor), typeof(UITypeEditor))]
        [Category("Product License"), PropertyOrder(5)]
        [DisplayName("Description")]
        public string Description
        {
            get { return _orderRec.Description; }
            set { _orderRec.Description = value; }
        }

        [Category("Product"), PropertyOrder(1)]
        [DisplayName("Name")]
        public String ProductName
        {
            get 
            {
                return _product.Name; 
            }
        }

        [Category("Product"), PropertyOrder(2)]
        [DisplayName("Version")]
        [TypeConverter(typeof(VersionConverter))]
        public LicenseVersion ProductVersion
        {
            get 
            {
                return _product.Version; 
            }
            set 
            {
                _product.Version = value; 
            }
        }

        [Browsable(false)]
        [Category("Product"), PropertyOrder(3)]
        [DisplayName("Product Connections")]
        public uint AppInstance
        {
            get
            {
                return _product.Product.productAppInstance.TVal;
            }
            set
            {
                foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs module in _product.Product.moduleList.TVal)
                {
                    module.moduleAppInstance.TVal = value;
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
