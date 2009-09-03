using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Solimar.Licensing.Attribs;
using System.ComponentModel;
using System.Text.RegularExpressions;
using System.Drawing.Design;

namespace Client.Creator
{
    [TypeConverter(typeof(PropertySorterTypeConverter))]
    public class ProductProperty
    {
        private LicenseVersion _version;
        Lic_PackageAttribs.Lic_ProductInfoAttribs _product;

        public ProductProperty()
        {                       
            _version = new LicenseVersion();
            _product = new Lic_PackageAttribs.Lic_ProductInfoAttribs();
            _product.productAppInstance.TVal = 1;   
        }

        public ProductProperty(Lic_PackageAttribs.Lic_ProductInfoAttribs product)
        {
            _product = new Lic_PackageAttribs.Lic_ProductInfoAttribs();
            _product.productID.TVal = product.productID.TVal;
            Version = new LicenseVersion(product.product_Major.TVal, product.product_Minor.TVal);
            _product.productAppInstance.TVal = product.productAppInstance.TVal;
            _product.moduleList.TVal = product.moduleList.TVal;
        }

        [Browsable(false)]
        public uint ID
        {
            get { return _product.productID.TVal; }
            set { _product.productID.TVal = value; }
        }

        [Category("Product"),PropertyOrder(1)]
        [DisplayName("Name")]
        [Description("Name")]
        [ReadOnly(true)]
        public string Name
        {
            //need to translate the id to name;
            get { return CreatorForm.s_CommLink.GetProductName(ID); }            
        }

        [Category("Product"),PropertyOrder(2)]
        [DisplayName("Version")]
        [Description("Version")]
        [TypeConverter(typeof(VersionConverter))]
        public LicenseVersion Version
        {
            get { return _version; }
            set 
            {
                //Regex reg = new Regex(@"([01]?\d\d|2[0-4]\d|25[0-5])\." + @"([01]?\d\d|2[0-4]\d|25[0-5])\." + @"([01]?\d\d|2[0-4]\d|25[0-5])\." + @"([01]?\d\d|2[0-4]\d|25[0-5])");
                /*Regex reg = new Regex(@"[d+]\."+@"[d+]\."+@"[d+]\.");
                if (!reg.IsMatch(Convert.ToString(value)))
                {
                    throw new FormatException("Your input is not a standard version format!");
                }
                 */         
                _product.product_Major.TVal = value.Major;
                _product.product_Minor.TVal = value.Minor;
                _version = value;
            } 
        }

        [Category("Product"),PropertyOrder(3)]
        [DisplayName("Product Connections")]
        [Description("Number of connections allowed per product.")]
        public uint AppInstance
        {
            get { return _product.productAppInstance.TVal; }
            set { _product.productAppInstance.TVal = value; }
        }
        
        [Browsable(false)]
        public Lic_PackageAttribs.Lic_ProductInfoAttribs.Lic_ModuleInfoAttribsList ModuleList 
        {
            get { return _product.moduleList; }
            set { _product.moduleList = value; }
        }

        [Browsable(false)]
        public Lic_PackageAttribs.Lic_ProductInfoAttribs Product
        {
            get { return _product; }
            set { _product = value; }
        }

        public override string ToString()
        {
            return Name;
        }

        //expects universal time
        public void SetAllModulesExpDate(string plNumber, DateTime? expDate)
        {
            foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs module in _product.moduleList.TVal)
            {
                if (module.contractNumber.TVal.Equals(plNumber))
                {
                    if (expDate.HasValue)
                        module.moduleExpirationDate.TVal = expDate.Value;
                    else
                        module.moduleExpirationDate.TVal = new DateTime(1900, 1, 1);
                }
            }
        }

        //1) set trial modules to perm
        //2) clean up trial order node and listview after saving license.
        public void SetTrialToLicensed(string plNumber, LicenseServerType lsType)
        {
            foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs module in _product.moduleList.TVal)
            {
                if (module.contractNumber.TVal.Equals(plNumber))
                {
                    if (lsType != LicenseServerType.Subscription)
                        module.moduleExpirationDate.TVal = new DateTime(1900, 1, 1);
                    module.moduleState.TVal = Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msLicensed;
                    if (CreatorForm.s_CommLink.IsDefaultModule(ID, module.moduleID.TVal))
                    {
                        module.moduleValue.TVal = (uint)CreatorForm.s_CommLink.GetDefaultModuleValue(ID, module.moduleID.TVal);
                    }
                    else
                    {
                        module.moduleValue.TVal = 0;
                        module.moduleAppInstance.TVal = 0;
                    }
                }                
            }
        }

        //1) set add-on modules to perm
        //2) clean up add-on order node and listview after saving license.
        public void SetAddOnToLicensed(string plNumber, string parentOrderNumber)
        {            
            foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs module in _product.moduleList.TVal)
            {
                //remove any non default module
                if (module.contractNumber.TVal.Equals(plNumber))
                {
                    //module value, module app instance
                    if (module.moduleState.TVal.Equals(Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msAddOn))
                    {                            
                        module.moduleState.TVal = Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msLicensed;
                        module.moduleExpirationDate.TVal = new DateTime(1900, 1, 1);
                        module.contractNumber.TVal = parentOrderNumber;
                    }
                }
            }
        }

        public void SetLicensedToTrial(string plNumber)
        {
            Lic_PackageAttribs.Lic_ModuleInfoAttribs module;
            //delete perm contract and any add-ons
            _product.moduleList.TVal.Clear();
            foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in CreatorForm.s_CommLink.m_softwareSpec.productSpecMap.TVal.Values)
            {
                if (productSpec.productID.TVal == ID)
                {
                    foreach (Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs moduleSpec in productSpec.moduleSpecMap.TVal.Values)
                    {
                        module = new Lic_PackageAttribs.Lic_ModuleInfoAttribs();
                        module.moduleID.TVal = moduleSpec.moduleID.TVal;
                        module.moduleValue.TVal = moduleSpec.moduleTrialLicense.TVal;                       
                        module.moduleExpirationDate.TVal = new DateTime(DateTime.Today.Year, DateTime.Today.Month, DateTime.Today.Day, 10, 0, 0).ToUniversalTime();
                        module.moduleAppInstance.TVal = 1;
                        module.contractNumber.TVal = plNumber;
                        module.moduleState.TVal = Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msTrial;
                        _product.moduleList.TVal.Add(module);
                    }
                }
            }
        }
    }

    public class LicenseVersion
    {
        private uint _major = 0;
        private uint _minor = 0;

        public LicenseVersion() { }
        public LicenseVersion(uint major, uint minor) { _major = major; _minor = minor; }
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

        public override bool Equals(Object lv)
        {
            LicenseVersion _lv = lv as LicenseVersion;
            if (_lv.Major == _major && _lv.Minor == _minor)
                return true;
            return false;
        }

        public override string ToString()
        {
            return string.Format("{0}.{1}", _major, _minor);
        }
    }

    // This is a special type converter which will be associated with the Employee class.
    // It converts an Employee object to string representation for use in a property grid.
    internal class VersionConverter : ExpandableObjectConverter
    {
        public override bool CanConvertTo(ITypeDescriptorContext context, Type destinationType)
        {
            if (destinationType == typeof(string))
            {
                return true;
            }
            return base.CanConvertTo(context, destinationType);
        }

        public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destType)
        {
            if (destType == typeof(string) && value is LicenseVersion)
            {
                // Cast the value to an LicenseVersion
                LicenseVersion version = (LicenseVersion)value;

                // Return Major and Minor separated by period.
                return version.Major + "." + version.Minor;
            }
            return base.ConvertTo(context, culture, value, destType);
        }
        public override bool CanConvertFrom(ITypeDescriptorContext context, Type t)
        {
            if (t == typeof(string))
                return true;
            return base.CanConvertFrom(context, t);
        }

        public override Object ConvertFrom(ITypeDescriptorContext context, System.Globalization.CultureInfo info, Object value)
        {
            //Debug.WriteLine("ConvertFrom: ");
            if (value is String)
            {
                string[] versionString = value.ToString().Split(".".ToCharArray());
                if(versionString.Count() != 2)
                     throw new Exception("Please enter a valid version!");
                return new LicenseVersion(UInt32.Parse(versionString[0]), UInt32.Parse(versionString[1]));
            }
            return base.ConvertFrom(context, info, value);
        }
    }
}
