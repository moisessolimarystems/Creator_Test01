using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;
using System.ComponentModel;
using System.Runtime.InteropServices;

namespace Client.Creator
{
    public enum SortDirection
    {
        ASC = -1,
        DESC = 1
    }

    public class SortableCollection<T> : CollectionBase, ICloneable, ICustomTypeDescriptor
    {
        #region Collection Members

        public virtual T this[int index]
        {
            get { return (T)this.List[index]; }
            set { this.List[index] = value; }
        }

        public virtual int IndexOf(T item)
        {
            return this.List.IndexOf(item);
        }

        public virtual int Add(T item)
        {
            return this.List.Add(item);
        }

        public virtual void Remove(T item)
        {
            this.List.Remove(item);
        }

        public virtual void CopyTo(Array array, int index)
        {
            this.List.CopyTo(array, index);
        }

        public virtual void AddRange(SortableCollection<T> collection)
        {
            this.InnerList.AddRange(collection);
        }

        public virtual void AddRange(T[] collection)
        {
            this.InnerList.AddRange(collection);
        }

        public virtual bool Contains(T item)
        {
            return this.List.Contains(item);
        }

        public virtual void Insert(int index, T item)
        {
            this.List.Insert(index, item);
        }

        public void Sort(string sortExpression, SortDirection sortDirection)
        {
            InnerList.Sort(new Comparer(sortExpression, sortDirection));
        }

        public void Sort(string sortExpression)
        {
            InnerList.Sort(new Comparer(sortExpression));
        }

        protected override void OnInsert(int index, object value)
        {
            base.OnInsert(index, value);
        }
        
        #endregion

        #region ICloneable Members

        public object Clone()
        {
            SortableCollection<T> clone = new SortableCollection<T>();
            clone.AddRange(this);
            return clone;
        }

        #endregion

        #region ICustomTypeDescriptor impl

        public String GetClassName()
        {
            return TypeDescriptor.GetClassName(this, true);
        }

        public AttributeCollection GetAttributes()
        {
            return TypeDescriptor.GetAttributes(this, true);
        }

        public String GetComponentName()
        {
            return TypeDescriptor.GetComponentName(this, true);
        }

        public TypeConverter GetConverter()
        {
            return TypeDescriptor.GetConverter(this, true);
        }

        public EventDescriptor GetDefaultEvent()
        {
            return TypeDescriptor.GetDefaultEvent(this, true);
        }

        public PropertyDescriptor GetDefaultProperty()
        {
            return TypeDescriptor.GetDefaultProperty(this, true);
        }

        public object GetEditor(Type editorBaseType)
        {
            return TypeDescriptor.GetEditor(this, editorBaseType, true);
        }

        public EventDescriptorCollection GetEvents(Attribute[] attributes)
        {
            return TypeDescriptor.GetEvents(this, attributes, true);
        }

        public EventDescriptorCollection GetEvents()
        {
            return TypeDescriptor.GetEvents(this, true);
        }

        public object GetPropertyOwner(PropertyDescriptor pd)
        {
            return this;
        }

        /// <summary>
        /// Called to get the properties of this type. Returns properties with certain
        /// attributes. this restriction is not implemented here.
        /// </summary>
        /// <param name="attributes"></param>
        /// <returns></returns>
        public PropertyDescriptorCollection GetProperties(Attribute[] attributes)
        {
            return GetProperties();
        }

        /// <summary>
        /// Called to get the properties of this type.
        /// </summary>
        /// <returns></returns>
        public PropertyDescriptorCollection GetProperties()
        {
            // Create a collection object to hold property descriptors
            PropertyDescriptorCollection pds = new PropertyDescriptorCollection(null);

            // Iterate the list of employees
            for (int i = 0; i < this.List.Count; i++)
            {
                // Create a property descriptor for the item and add to the property descriptor collection
                GenericCollectionPropertyDescriptor<T> pd = new GenericCollectionPropertyDescriptor<T>(this, i);
                pds.Add(pd);
            }
            // return the property descriptor collection
            return pds;
        }

        #endregion
    }

    public class Comparer : IComparer
    {
        string m_SortPropertyName ;
        SortDirection m_SortDirection ;

        public Comparer(string sortPropertyName)
        {
            this.m_SortPropertyName = sortPropertyName ;
            this.m_SortDirection = SortDirection.ASC ;
            // default to ascending order
        }
       
        public Comparer(string sortPropertyName, SortDirection sortDirection)
        {
            this.m_SortPropertyName = sortPropertyName ;
            this.m_SortDirection = sortDirection ;
        }

        public int Compare(object x, object y)
        {
            // Get the values of the relevant property on the x and y objects

            object valueOfX = x.GetType().GetProperty(m_SortPropertyName).GetValue(x, null) ;
            object valueOfY = y.GetType().GetProperty(m_SortPropertyName).GetValue(y, null) ;

            IComparable comp = valueOfY as IComparable ;

            // Flip the value from whatever it was to the opposite.
            return Flip(comp.CompareTo(valueOfX)) ;
        }

        private int Flip(int i)
        {
            return (i * (int)m_SortDirection) ;
        }
    }
}