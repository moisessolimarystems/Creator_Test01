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

        public ModuleProperty(Lic_PackageAttribs.Lic_ModuleInfoAttribs module)
        {
            _module = module;                      
        }

        public void SetModuleProperty(ModuleProperty module)
        {
            _module.moduleID.TVal = module.ID;
            _module.moduleAppInstance.TVal = module.AppInstance;
            _module.moduleValue.TVal = module.Units;
        }

        public override bool Equals(object obj)
        {
            return _module.Equals(obj as Lic_PackageAttribs.Lic_ModuleInfoAttribs);
        }

        public uint ProductID
        {
            get { return _productID; }
            set { _productID = value; }
        }

        public uint ID
        {
            get { return _module.moduleID.TVal; }
            set { _module.moduleID.TVal = value; }
        }
     
        public virtual string Name
        {
            //need to translate the id to name;
            get { return CreatorForm.s_CommLink.GetModuleName(_productID, ID); }
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

        public uint Units
        {
            get { return _module.moduleValue.TVal; }
            set { _module.moduleValue.TVal = value; }
        }

        public uint AppInstance
        {
            get{ return _module.moduleAppInstance.TVal;}
            set { _module.moduleAppInstance.TVal = value; }
        }

        public override string ToString()
        {
            return _name;
        }
    }
}
