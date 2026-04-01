using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Client.Creator
{
    public class ConditionProperty
    {
        public enum Name
        {
            Customer,
            DestinationID,
            GroupID,
            LicenseType
        }

        public enum Operator
        { 
            Equals,
            Contains,
            LessThan,
            GreaterThan
        }

        private Name? _name;
        private Operator? _operator;
        private string _value;

        public Name? ConditionName
        {
            get { return _name; }
            set { _name = value; }
        }

        //numeric operators - =, >, <
        //string operators - is, contains, is not, does not contain
        public Operator? ConditionOperator
        {
            get { return _operator;  }
            set { _operator = value; }
        }

        //can be numeric or string       
        public string ConditionValue
        {
            get { return _value; }
            set { _value = value; }
        }
    }
}
