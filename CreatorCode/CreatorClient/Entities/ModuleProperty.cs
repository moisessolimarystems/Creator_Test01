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
        #region Fields
        byte _productID;
        ModuleTable _module;
        CommunicationLink _commLink;
        #endregion

        #region Constructor
        /// <summary>
        /// Initializes a new instance of the ModuleProperty class. 
        /// Represents an individual module of a product. 
        /// </summary>
        /// <param name="module">ModuleTable entry from the database.</param>
        public ModuleProperty(ModuleTable module)
        {
            _module = module;
            _commLink = CreatorForm.s_CommLink;
        }
        #endregion
        //public override bool Equals(object obj)
        //{
        //    return _module.Equals(obj as Lic_PackageAttribs.Lic_ModuleInfoAttribs);
        //}

        #region Properties
        /// <summary>
        /// Gets or sets the product ID used to determine the details of the module.
        /// </summary>
        public byte ProductID
        {
            get { return _productID; }
            set { _productID = value; }
        }

        /// <summary>
        /// Gets or sets the module ID.
        /// </summary>
        public short ID
        {
            get { return _module.ModID; }
            set { _module.ModID = value; }
        }

        /// <summary>
        /// Gets the module name determined by the ID and product ID.
        /// </summary>
        public virtual string Name
        {
            //need to translate the id to name;
            get { return _commLink.GetModuleName(_productID, ID); }
        }

        /// <summary>
        /// Gets or sets the units for the module.
        /// </summary>
        public short Value
        {
            get { return _module.Value; }
            set { _module.Value = value; }
        }

        /// <summary>
        /// Gets or sets the product connections for the module.
        /// </summary>
        public byte AppInstance
        {
            get{ return _module.AppInstance;}
            set { _module.AppInstance = value; }
        }

        /// <summary>
        /// Gets the unlimited value for the module.
        /// </summary>
        public uint UnlimitedValue
        {
            get 
            {
                return (uint)_commLink.GetUnlimitedModuleValue(ProductID, (uint)ID);               
            }
        }

        /// <summary>
        /// Gets or sets the trial value for the module.
        /// </summary>
        public short TrialValue
        {
            get
            {
                return _commLink.GetModuleTrialValue(ProductID, ID);
            }
        }
        #endregion

        #region Overrides
        public override string ToString()
        {
            return _commLink.GetModuleName(_productID, ID);
        }
        #endregion
    }
}
