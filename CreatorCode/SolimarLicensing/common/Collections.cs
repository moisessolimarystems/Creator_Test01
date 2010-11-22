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
            Item1 = _t1;
            Item2 = _t2;
        }

        /// <summary>First Item</summary>
        public T1 Item1 { get; set; }

        /// <summary>Second Item</summary>
        public T2 Item2 { get; set; }
    }

    /// <summary>Represents a strongly typed triplet of objects</summary>
    public class Tuple<T1, T2, T3> : Tuple<T1, T2>
    {
        public Tuple(T1 _t1, T2 _t2, T3 _t3)
            : base(_t1, _t2)
        {
            Item3 = _t3;
        }

        /// <summary>Third Item</summary>
        public T3 Item3 { get; set; }
    }

    /// <summary>Represents a strongly typed quartet of objects</summary>
    public class Tuple<T1, T2, T3, T4> : Tuple<T1, T2, T3>
    {
        public Tuple(T1 _t1, T2 _t2, T3 _t3, T4 _t4)
            : base(_t1, _t2, _t3)
        {
            Item4 = _t4;
        }

        /// <summary>Fourth Item</summary>
        public T4 Item4 { get; set; }
    }
}
