using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using Solimar.Licensing;
using Solimar.Licensing.Attribs;
using Client.Creator.ServiceProxy;
using Client.Creator.CreatorService;

namespace Client.Creator
{
    [TypeConverter(typeof(PropertySorterTypeConverter))]
    public class ModuleProperty
    {
        private string _name;
        //static to allow ProductProperty to set what product the module belongs before being initialized
        private ModuleTable _module;
        public static byte _productID;
        public CommunicationLink _commLink;

        public ModuleProperty(ModuleTable module)
        {
            _module = module;
            _commLink = CreatorForm.s_CommLink;
        }

        public override bool Equals(object obj)
        {
            return _module.Equals(obj as Lic_PackageAttribs.Lic_ModuleInfoAttribs);
        }

        public byte ProductID
        {
            get { return _productID; }
            set { _productID = value; }
        }

        public short ID
        {
            get { return _module.ModID; }
            set { _module.ModID = value; }
        }
     
        public virtual string Name
        {
            //need to translate the id to name;
            get { return _commLink.GetModuleName(_productID, ID); }
            set { _name = value; }
        }

        public short Value
        {
            get { return _module.Value; }
            set { _module.Value = value; }
        }

        public byte AppInstance
        {
            get{ return _module.AppInstance;}
            set { _module.AppInstance = value; }
        }

        public CommunicationLink CommLink
        { 
            get { return _commLink; } 
        }

        public uint UnlimitedValue
        {
            get 
            {
                return (uint)_commLink.GetUnlimitedModuleValue(ProductID, (uint)ID);               
            }
        }

        public short TrialValue
        {
            get
            {
                return _commLink.GetModuleTrialValue(ProductID, ID);
            }
        }

        public override string ToString()
        {
            return _name;
        }
    }
}
