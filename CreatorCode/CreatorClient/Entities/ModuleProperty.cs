using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using Solimar.Licensing;
using Solimar.Licensing.Attribs;

namespace Client.Creator
{
    [TypeConverter(typeof(PropertySorterTypeConverter))]
    public class ModuleProperty
    {
        private string _name;
        //static to allow ProductProperty to set what product the module belongs before being initialized
        private Lic_PackageAttribs.Lic_ModuleInfoAttribs _module;
        public static uint _productID;

        public ModuleProperty()
        {
            _module.moduleExpirationDate.TVal = DateTime.Today;
        }

        public ModuleProperty(Lic_PackageAttribs.Lic_ModuleInfoAttribs module)
        {
            _module = module;                      
        }

        public void SetModuleProperty(ModuleProperty module)
        {
            _module.moduleID.TVal = module.ID;
            _module.moduleExpirationDate.TVal = module.ExpirationDate;
            _module.moduleAppInstance.TVal = module.AppInstance;
            _module.moduleValue.TVal = module.Units;
            _module.moduleState.TVal = module.ModuleState;
            _module.contractNumber.TVal = module.OrderNumber;
        }

        public override bool Equals(object obj)
        {
            return _module.Equals(obj as Lic_PackageAttribs.Lic_ModuleInfoAttribs);
        }

        //[Browsable(false)]
        public uint ProductID
        {
            get { return _productID; }
            set { _productID = value; }
        }

        //[Browsable(false)]
        public uint ID
        {
            get { return _module.moduleID.TVal; }
            set { _module.moduleID.TVal = value; }
            //get { return _id; }
            //set { _id = value; }
        }

        //need to turn this into a type of product
        //that way when it is converted into a string it con infer from the 
        //[Category("Module"), PropertyOrder(1)]
        //[DisplayName("Module Name")]
        //[Description("Module Name")]
        //[Browsable(true)]
        //[TypeConverter(typeof(ModuleNameConverter))]        
        public virtual string Name
        {
            //need to translate the id to name;
            get { return _name; }
            set 
            {
                if ((value as string).Equals("{New Module}"))
                {
                    throw new FormatException("Please set a module");
                }
                //TODO : need to account for case where _productID is bad.
                _module.moduleID.TVal = (uint)CreatorForm.s_CommLink.GetModuleID(_productID, value);
                _name = value; 
            }
        }

        //[Category("Module"), PropertyOrder(3)]
        //[DisplayName("Units")]
        //[Description("Units")]
        //[Browsable(true)]
        public uint Units
        {
            get { return _module.moduleValue.TVal; } //_units; }
            set { _module.moduleValue.TVal = value; }
            //set { _units = value; }
        }

        //[Category("Module"), PropertyOrder(4)]
        //[DisplayName("AppInstance")]
        //[Description("AppInstance")]
        //[Browsable(true)]
        public uint AppInstance
        {
            get{ return _module.moduleAppInstance.TVal;}
            set { _module.moduleAppInstance.TVal = value; }
            //get { return _appInst; }
            //set { _appInst = value; }
        }

        //[Category("Module"), PropertyOrder(5)]
        //[DisplayName("Expiration Date")]
        //[Description("Expiration Date")]
        //[Browsable(true)]
        public DateTime ExpirationDate
        {
            get { return _module.moduleExpirationDate.TVal; }
            set { _module.moduleExpirationDate.TVal = value; }
            //get{return _expirationDate.Date;}
            //set{_expirationDate = value;}
        }

        public Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState ModuleState
        {
            get { return _module.moduleState.TVal; }
            set { _module.moduleState.TVal = value; }
            //get { return _moduleState; }
            //set { _moduleState = value; }
        }

        public string OrderNumber
        {
            get { return _module.contractNumber.TVal; }
            set { _module.contractNumber.TVal = value; }
            //get { return _orderNumber; }
            //set { _orderNumber = value; }
        }

        public override string ToString()
        {
            return _name;
        }

        ////hooks up to name property of ModuleProperty
        //internal class ModuleNameConverter : StringConverter
        //{
        //    public override bool GetStandardValuesSupported(ITypeDescriptorContext context)
        //    {
        //        //true means show a combobox
        //        return true;
        //    }

        //    public override bool GetStandardValuesExclusive(ITypeDescriptorContext context)
        //    {
        //        //true will limit to list. false will show the list, but allow free-form entry
        //        return true;
        //    }
        //    //needs to know the product the modules belong to
        //    //where can I get that information
        //    //modulenameconverter placed on the collection
        //    public override StandardValuesCollection GetStandardValues(ITypeDescriptorContext context)
        //    {
        //        //return all values from software spec
        //        List<string> moduleList = new List<string>();
        //        GlobalSoftwareSpec globalSwSpec = new GlobalSoftwareSpec();
        //        foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in globalSwSpec.softwareSpec.productSpecMap.TVal.Values)
        //        {
        //            if (productSpec.productID == _productID)
        //            {
        //                foreach (Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs moduleSpec in productSpec.moduleSpecMap.TVal.Values)
        //                {
        //                    moduleList.Add(moduleSpec.moduleName);
        //                }
        //            }
        //        }
        //        return new StandardValuesCollection(moduleList.ToArray());
        //    }
        //}
    }
}
