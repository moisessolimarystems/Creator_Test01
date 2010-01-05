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

        public string GetFilterKey(CreatorService.Condition userCondition)
        {
            string conditionString = "";
            if (Type == ReportType.LicenseServer)
            {
                foreach (KeyValuePair<string, ConditionName> kvp in _filterLSNames)
                {
                    if (kvp.Value == userCondition.Name)
                    {
                        conditionString = kvp.Key;
                        break;
                    }
                }
            }
            else
            {
                foreach (KeyValuePair<string, ConditionName> kvp in _filterPLNames)
                {
                    if (kvp.Value == userCondition.Name)
                    {
                        conditionString = kvp.Key;
                        break;
                    }
                }
            }            
            return conditionString;
        }
    }

}
