using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Client.Creator.CreatorService;

namespace Client.Creator
{
    public class ReportProperty
    {
        private string _name;
        private List<Condition> _conditions;

        public ReportProperty()
        {
            _conditions = new List<Condition>();
        }

        public string Name
        {
            get { return _name; }
            set { _name = value; }
        }

        public List<Condition> Conditions
        {
            get { return _conditions; }
            set { _conditions = value; }
        }
    }

}
