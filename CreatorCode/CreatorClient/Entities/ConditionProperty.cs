using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Client.Creator
{
    public class ConditionProperty
    {
        private Creator.CreatorService.ConditionName _name;
        private Creator.CreatorService.ConditionOperator _operator;
        private string _value;

        public Creator.CreatorService.ConditionName Name
        {
            get { return _name; }
            set { _name = value; }
        }

        //numeric operators - =, >, <
        //string operators - is, contains, is not, does not contain
        public Creator.CreatorService.ConditionOperator Operator
        {
            get { return _operator;  }
            set { _operator = value; }
        }

        //can be numeric or string       
        public string Value
        {
            get { return _value; }
            set { _value = value; }
        }
    }
}
