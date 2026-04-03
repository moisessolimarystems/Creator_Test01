using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Collections;

namespace Client.Creator
{
    /// <summary>
    /// CustomPropertyCollection
    /// </summary>
    public class CustomPropertyCollection : CollectionBase, ICustomTypeDescriptor
    {
        /// <summary>
        /// Add CustomProperty to CollectionBase
        /// </summary>
        /// <param name="Value"></param>
        public void Add(CustomProperty Value)
        {
            base.List.Add(Value);
        }
        /// <summary>
        /// Remove CustomProperty from CollectionBase
        /// </summary>
        /// <param name="Name"></param>
        public void Remove(string Name)
        {
            foreach (CustomProperty property in base.List)
            {
                if (property.Name == Name)
                {
                    base.List.Remove(property);
                    return;
                }
            }
        }
        /// <summary>
        /// CustomProperty CollectionBase indexer 
        /// </summary>
        /// <param name="index"></param>
        /// <returns></returns>
        public CustomProperty this[int index]
        {
            get { return (CustomProperty)base.List[index]; }
            set { base.List[index] = value; }
        }

        #region TypeDescriptor Implementation
        /// <summary>
        /// Returns CustomProperty class name
        /// </summary>
        /// <returns></returns>
        public string GetClassName()
        {
            return TypeDescriptor.GetClassName(this, true);
        }

        /// <summary>
        /// Gets CustomProperty attributes
        /// </summary>
        /// <returns></returns>
        public AttributeCollection GetAttributes()
        {
            return TypeDescriptor.GetAttributes(this, true);
        }

        /// <summary>
        /// Get CustomProperty ComponentName
        /// </summary>
        /// <returns></returns>
        public string GetComponentName()
        {
            return TypeDescriptor.GetComponentName(this, true);
        }

        /// <summary>
        /// Get CustomProperty TypeConverter
        /// </summary>
        /// <returns></returns>
        public TypeConverter GetConverter()
        {
            return TypeDescriptor.GetConverter(this, true);
        }

        /// <summary>
        /// Get CustomProperty Default Event
        /// </summary>
        /// <returns></returns>
        public EventDescriptor GetDefaultEvent()
        {
            return TypeDescriptor.GetDefaultEvent(this, true);
        }

        /// <summary>
        /// Get CustomProperty Default Property
        /// </summary>
        /// <returns></returns>
        public PropertyDescriptor GetDefaultProperty()
        {
            return TypeDescriptor.GetDefaultProperty(this, true);
        }

        /// <summary>
        /// Get CustomProperty Editor
        /// </summary>
        /// <param name="editorBaseType"></param>
        /// <returns></returns>
        public object GetEditor(Type editorBaseType)
        {
            return TypeDescriptor.GetEditor(this, editorBaseType, true);
        }

        /// <summary>
        /// Get CustomProperty Events
        /// </summary>
        /// <param name="attributes"></param>
        /// <returns></returns>
        public EventDescriptorCollection GetEvents(Attribute[] attributes)
        {
            return TypeDescriptor.GetEvents(this, attributes, true);
        }

        /// <summary>
        /// Get CustomProperty Events
        /// </summary>
        /// <returns></returns>
        public EventDescriptorCollection GetEvents()
        {
            return TypeDescriptor.GetEvents(this, true);
        }

        /// <summary>
        /// Get CustomProperty properties 
        /// </summary>
        /// <param name="attributes"></param>
        /// <returns></returns>
        public PropertyDescriptorCollection GetProperties(Attribute[] attributes)
        {
            PropertyDescriptor[] newProperties = new PropertyDescriptor[this.Count];
            for (int i = 0; i < this.Count; i++)
            {
                CustomProperty property = this[i] as CustomProperty;
                newProperties[i] = new CustomPropertyDescriptor(ref property, attributes);
            }
            return new PropertyDescriptorCollection(newProperties);
        }

        /// <summary>
        /// Get CustomProperty Properties
        /// </summary>
        /// <returns></returns>
        public PropertyDescriptorCollection GetProperties()
        {
            return TypeDescriptor.GetProperties(this, true);
        }

        public object GetPropertyOwner(PropertyDescriptor pd)
        {
            return this;
        }
        #endregion
    }
    /// <summary>
    /// Custom Property to that represents an object used in a propertygrid.
    /// </summary>
    public class CustomProperty
    {
        private string name = string.Empty;
        private bool isReadOnly = false;
        private bool isVisible = true;
        private object objValue = null;
        private string category = string.Empty;
        private string description = string.Empty;
        private Type type;

        /// <summary>
        /// Class with attributes used in a propertygrid.
        /// </summary>
        /// <param name="name"> </param>
        /// <param name="isReadOnly"></param>
        /// <param name="isVisible"></param>
        /// <param name="objValue"></param>
        /// <param name="category"></param>
        /// <param name="description"></param>
        public CustomProperty(string name, object objValue, string category, string description, Type type, bool isReadOnly, bool isVisible)
        {
            this.name = name;
            this.objValue = objValue;
            this.category = category;
            this.description = description;
            this.type = type;
            this.isReadOnly = isReadOnly;
            this.isVisible = isVisible;
        }

        public string Name
        {
            get { return name; }
        }

        public object Value
        {
            get { return objValue; }
            set { objValue = value; }
        }

        public string Category
        {
            get { return category; }
        }

        public string Description
        {
            get { return description; }
        }

        public Type Type
        {
            get { return type;}
        }

        public bool ReadOnly
        {
            get { return isReadOnly; }
        }

        public bool Visible
        {
            get { return Visible; }
        }
    }

    /// <summary>
    /// Custom PropertyDescriptor
    /// </summary>
    public class CustomPropertyDescriptor : PropertyDescriptor
    {
        CustomProperty customProperty;

        public CustomPropertyDescriptor(ref CustomProperty customProperty, Attribute[] attributes)
            : base(customProperty.Name, attributes)
        {
            this.customProperty = customProperty;
        }

        #region PropertyDescriptor Implementation
        public override bool CanResetValue(object component)
        {
            return true;
        }

        public override Type ComponentType
        {
            get { return null; }
        }

        public override object GetValue(object component)
        {
            return customProperty.Value;
        }

        public override string Description
        {
            get { return customProperty.Description; }
        }

        public override string Category
        {
            get { return customProperty.Category; }
        }

        public override string DisplayName
        {
            get { return customProperty.Name; }
        }

        public override bool IsReadOnly
        {
            get { return customProperty.ReadOnly; }
        }

        public override void ResetValue(object component)
        {          
        }

        public override bool ShouldSerializeValue(object component)
        {
            return false;
        }

        public override void SetValue(object component, object value)
        {
            customProperty.Value = value;
        }

        public override Type PropertyType
        {
            get { return customProperty.Type; }
        }
        #endregion
    }
}
