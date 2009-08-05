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
            _module.moduleExpirationDate.TVal = new DateTime(DateTime.Today.Year, DateTime.Today.Month, DateTime.Today.Day, 10, 0, 0); //DateTime.Today;
        }

        public ModuleProperty(Lic_PackageAttribs.Lic_ModuleInfoAttribs module)
        {
            _module = module;                      
        }

        public void SetModuleProperty(ModuleProperty module)
        {
            _module.moduleID.TVal = module.ID;
            _module.moduleExpirationDate.TVal = module.ExpirationDate.ToUniversalTime();
            _module.moduleAppInstance.TVal = module.AppInstance;
            _module.moduleValue.TVal = module.Units;
            _module.moduleState.TVal = module.ModuleState;
            _module.contractNumber.TVal = module.OrderNumber;
        }

        public override bool Equals(object obj)
        {
            return _module.Equals(obj as Lic_PackageAttribs.Lic_ModuleInfoAttribs);
        }

        public bool IsAllowedRemoveModule
        {
            get
            {
                if (_module.moduleValue.TVal > 0)
                {
                    if (_module.moduleState.TVal == Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msPerm)
                    {
                        if (!CreatorForm.s_CommLink.IsDefaultModule(_productID, _module.moduleID.TVal))
                            return true;
                    }
                    else
                    {
                        if (_module.moduleState.TVal == Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msAddOn)
                            return true;
                    }
                }
                return false;
            }
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

        public DateTime ExpirationDate
        {
            get { return _module.moduleExpirationDate.TVal.ToLocalTime(); }
            set { _module.moduleExpirationDate.TVal = value.ToUniversalTime(); }
        }

        public Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState ModuleState
        {
            get { return _module.moduleState.TVal; }
            set { _module.moduleState.TVal = value; }
        }

        public string OrderNumber
        {
            get { return _module.contractNumber.TVal; }
            set { _module.contractNumber.TVal = value; }
        }

        public override string ToString()
        {
            return _name;
        }
    }
}
