using System;
using System.Collections.Generic;
using System.Text;

namespace Solimar.Licensing.Common.Collections
{
    /// <summary>Represents a strongly typed pair of objects</summary>
    public class Tuple<T1, T2>
    {
        public List<int> i;
        public Tuple() { }
        public Tuple(T1 _t1, T2 _t2)
        {
            m_t1 = _t1;
            m_t2 = _t2;
        }

        /// <summary>First Item</summary>
        public T1 Item1
        {
            get { return m_t1; }
            set { m_t1 = value; }
        }

        /// <summary>Second Item</summary>
        public T2 Item2
        {
            get { return m_t2; }
            set { m_t2 = value; }
        }
        protected T1 m_t1;
        protected T2 m_t2;
    }

    /// <summary>Represents a strongly typed triplet of objects</summary>
    public class Tuple<T1, T2, T3> : Tuple<T1, T2>
    {
        public Tuple(T1 _t1, T2 _t2, T3 _t3) : base(_t1, _t2)
        {
            m_t3 = _t3;
        }

        /// <summary>Third Item</summary>
        public T3 Item3
        {
            get { return m_t3; }
            set { m_t3 = value; }
        }
        protected T3 m_t3;
    }
}
