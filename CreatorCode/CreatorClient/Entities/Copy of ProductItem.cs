using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Solimar.Licensing.Attribs;
using System.ComponentModel;
using System.Text.RegularExpressions;

namespace CreatorClient.Collections
{
    [TypeConverter(typeof(PropertySorterTypeConverter))]
    class ProductItem<T> : BaseGridItem
    {
        private string name;
        private uint id;
        private string version;
        private DateTime expirationDate;
        private uint appInst;
        private SortableCollection<T> moduleList;

        public ProductItem()
        {
            expirationDate = DateTime.Today;
            moduleList = new SortableCollection<T>();
        }

        public ProductItem(Lic_PackageAttribs.Lic_ProductInfoAttribs product, CommunicationLink commLink)
        {
            
            id = product.productID.TVal;
            name = commLink.GetProductName(id);
            version = string.Concat(product.product_Major.SVal, ".",product.product_Minor.SVal, ".",product.product_SubMajor.SVal, ".", product.product_SubMinor.SVal);
            appInst = product.productAppInstance;
            expirationDate = product.productExpirationDate;
            moduleList = new SortableCollection<T>();
            //Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs modSpec;
            for (int moduleIndex = 0; moduleIndex < product.moduleList.TVal.Count; moduleIndex++)
            {   //add only defaults
               // modSpec = commLink.GetModuleSpec(
                Lic_PackageAttribs.Lic_ModuleInfoAttribs foundModule = (Lic_PackageAttribs.Lic_ModuleInfoAttribs)product.moduleList.TVal[moduleIndex];               
                T module = new T(foundModule, commLink);
                moduleList.Add(module);
            }
        }

        [Category("Product"),PropertyOrder(1)]
        [DisplayName("Name")]
        [Description("Name")]
        [Browsable(true)]
        [ReadOnly(true)]
        public override string Name
        {
            //need to translate the id to name;
            get { return name; }
        }

        [Category("Product"),PropertyOrder(2)]
        [DisplayName("Product ID")]
        [Description("Product ID")]
        [Browsable(false)]
        [ReadOnly(true)]
        public uint ID
        {
            get { return id; }
            set { id = value; }
        }

        [Category("Product"),PropertyOrder(3)]
        [DisplayName("Version")]
        [Description("Version")]
        [Browsable(true)]
        public string Version
        {
            get { return version; }
            set 
            {
                Regex reg = new Regex(@"([01]?\d\d|2[0-4]\d|25[0-5])\." + @"([01]?\d\d|2[0-4]\d|25[0-5])\." + @"([01]?\d\d|2[0-4]\d|25[0-5])\." + @"([01]?\d\d|2[0-4]\d|25[0-5])");
                if (!reg.IsMatch(Convert.ToString(value)))
                {
                    throw new FormatException("Your input is not a standard version format!");
                }
                version = value; 
            } 
        }

        [Category("Product"),PropertyOrder(4)]
        [DisplayName("AppInstance")]
        [Description("AppInstance")]
        [Browsable(true)]
        public uint AppInstance
        {
            get { return appInst; }
            set { appInst = value; }
        }

        [Category("Product"),PropertyOrder(5)]
        [DisplayName("Expiration Date")]
        [Description("Expiration Date")]
        [Browsable(true)]
        public DateTime ExpirationDate
        {
            get { return expirationDate; }
            set { expirationDate = value; } 
        }

        //[Editor(typeof(CustomCollectionEditor<ModuleItem>), typeof(System.Drawing.Design.UITypeEditor))]
        [TypeConverter(typeof(ModuleCollectionConverter))]
        [Category("Product Modules")]
        [DisplayName("ModuleList")]
        [Description("ModuleList")]
        [Browsable(true)]
        public SortableCollection<T> ModuleList
        {
            get { return moduleList; }
            set { moduleList = value; }
        }

        public override string ToString()
        {
            return name;
        }
    }
    // This is a special type converter which will be associated with the SortableCollection<ModuleItem> class.
    // It converts an SortableCollection<ModuleItem> object to string representation for use in a property grid.
    internal class ModuleCollectionConverter : PropertySorterTypeConverter
    {
        public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destType)
        {
            if (destType == typeof(string) && value is SortableCollection<T>)
            {
                return "(Collection)";
            }
            return base.ConvertTo(context, culture, value, destType);
        }
    }
}
