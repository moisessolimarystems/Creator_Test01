using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Client.Creator.CreatorService;

namespace Client.Creator
{
    public class ReportProperty
    {
        public static readonly IDictionary<string, ConditionName>
        _filterHTNames = new Dictionary<string, ConditionName>
                {  
                    {"Activated Date", ConditionName.ActivatedDate},
                    {"Customer", ConditionName.Customer},       
                    {"Deactivated Date", ConditionName.DeactivatedDate},
                    {"Hardware ID", ConditionName.HardwareID},
                    {"License Server", ConditionName.LicenseServer},
                    {"State", ConditionName.State}
                };

        public static readonly IDictionary<string, ConditionName>
            _filterLSNames = new Dictionary<string, ConditionName>
                {  
                    {"Active", ConditionName.Active},
                    {"Customer", ConditionName.Customer},        
                    {"License Server", ConditionName.LicenseServer},
                };

        public static readonly IDictionary<string, ConditionName>
            _filterPLNames = new Dictionary<string, ConditionName>
                {  
                    {"Activation Amount",ConditionName.ActivationAmount}, 
                    {"Activation Total", ConditionName.Activation},
                    {"Active", ConditionName.Active},        
                    {"Customer", ConditionName.Customer},  
                    {"Extension Count", ConditionName.Extension},
                    {"Expiration Date", ConditionName.ExpirationDate},
                    {"Product", ConditionName.Product},
                    {"Product License", ConditionName.ProductLicense},
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

        public static readonly IDictionary<string, ConditionOperator>
            _filterDateOperators = new Dictionary<string, ConditionOperator>
                {
                    {"is", ConditionOperator.Equal},
                    {"is not", ConditionOperator.NotEqual},
                    {"is after", ConditionOperator.LessThan},
                    {"is before", ConditionOperator.GreaterThan},
                    {"is in the last", ConditionOperator.Contains},
                    {"is not in the last", ConditionOperator.Contains},
                    {"is in the next", ConditionOperator.Contains},
                    {"is in the range of", ConditionOperator.Contains}
                };

        public static readonly IDictionary<string, ConditionOperator>
            _filterStringOperators = new Dictionary<string, ConditionOperator>
                {
                    {"contains", ConditionOperator.Equal},
                    {"does not contain", ConditionOperator.NotEqual},
                    {"is", ConditionOperator.LessThan},
                    {"is not", ConditionOperator.GreaterThan},
                    {"starts with", ConditionOperator.Contains},
                    {"ends with", ConditionOperator.Contains}
                };

        public static readonly IDictionary<string, ConditionOperator>
            _filterBoolOperators = new Dictionary<string, ConditionOperator>
                {
                    {"is true", ConditionOperator.Equal},
                    {"is false", ConditionOperator.NotEqual},
                };

        public enum ReportType
        {
            LicenseServer,
            ProductLicense,
            HardwareToken
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

        public ConditionName GetFilterName(string filterName)
        {
            ConditionName conditionName = ConditionName.UnKnown;
            switch (Type)
            {
                case ReportType.LicenseServer:
                    conditionName = _filterLSNames[filterName];
                    break;
                case ReportType.ProductLicense:
                    conditionName = _filterPLNames[filterName];
                     break;
                case ReportType.HardwareToken:
                    conditionName = _filterHTNames[filterName];
                     break;
                default: break;
            }
            return conditionName;
        }

        public string GetFilterKey(CreatorService.Condition userCondition)
        {
            string conditionString = "";
            IDictionary<string, ConditionName> filterNames = null;
            if (Type == ReportType.LicenseServer)
                filterNames = _filterLSNames;
            else if (Type == ReportType.ProductLicense)
                filterNames = _filterPLNames;
            else
                filterNames = _filterHTNames;
            foreach (KeyValuePair<string, ConditionName> kvp in filterNames)
            {
                if (kvp.Value == userCondition.Name)
                {
                    conditionString = kvp.Key;
                    break;
                }
            }
            return conditionString;
        }
    }

}
