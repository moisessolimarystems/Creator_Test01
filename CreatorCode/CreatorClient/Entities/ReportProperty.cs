using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Client.Creator.CreatorService;

namespace Client.Creator
{
    public class ReportProperty
    {
        //dictionary to translate enums to ls enums
        public static readonly IDictionary<string, ConditionName>
        _filterNames = new Dictionary<string, ConditionName>
                {        
                    {"Customer", ConditionName.Customer},
                    {"License Server", ConditionName.LicenseServer},
                    {"License Type", ConditionName.LicenseType},
                    {"Product License", ConditionName.ProductLicense},
                    {"Expiration Date", ConditionName.ExpirationDate},
                    {"Product", ConditionName.Product},
                    {"Product Version", ConditionName.ProductVersion},
                    {"State", ConditionName.State}
                };

        public static readonly IDictionary<string, ConditionOperator>
            _filterOperators = new Dictionary<string, ConditionOperator>
                {
                    {"is", ConditionOperator.Equal},
                    {"is not", ConditionOperator.NotEqual},
                    {"is less than", ConditionOperator.LessThan},
                    {"is greater than", ConditionOperator.GreaterThan},
                    {"contains", ConditionOperator.Contains}
                };

        public enum ReportType
        {
            LicenseServer,
            ProductLicense
        }

        private string  _id;
        private ReportType _type;
        private List<Condition> _conditions;

        public ReportProperty()
        {
            _conditions = new List<Condition>();
        }

        public string ID
        {
            get { return _id; }
            set { _id = value; }
        }

        public ReportType Type
        {
            get { return _type; }
            set { _type = value; }
        }

        public List<Condition> Conditions
        {
            get { return _conditions; }
            set { _conditions = value; }
        }

        public string GetConditionsString()
        {
            string conditionString = "";
            return conditionString;
        }
    }

}
