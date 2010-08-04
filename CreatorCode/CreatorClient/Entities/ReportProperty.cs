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
            _filterLPNames = new Dictionary<string, ConditionName>
                { 
                    {"Customer", ConditionName.Customer},
                    {"License Packet", ConditionName.LicensePacket},
                    {"Date Created", ConditionName.DateCreated},        
                    {"Expiration Date", ConditionName.ExpirationDate},                   
                    {"Verified", ConditionName.Verified},
                    {"Verified By", ConditionName.VerifiedBy},
                    {"Notes", ConditionName.Notes}
                };

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
                    {"Notes", ConditionName.Notes},
                    {"Validation", ConditionName.Validation},
                    {"Verified", ConditionName.Verified}
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
                    {"Module", ConditionName.Module},
                    {"Module Value", ConditionName.ModuleValue},
                    {"Notes", ConditionName.Notes},
                    {"Product", ConditionName.Product},
                    {"Product License", ConditionName.ProductLicense},
                    {"Product Version", ConditionName.ProductVersion},
                    {"State", ConditionName.State},
                    {"Verified", ConditionName.Verified}
                };

        public static readonly IDictionary<string, ConditionOperator>
            _filterOperators = new Dictionary<string, ConditionOperator>
                {
                    {"is", ConditionOperator.Equal},
                    {"is not", ConditionOperator.NotEqual}
                };

        public static readonly IDictionary<string, ConditionOperator>
            _filterNumberOperators = new Dictionary<string, ConditionOperator>
                {
                    {"is", ConditionOperator.Equal},
                    {"is not", ConditionOperator.NotEqual},
                    {"is less than", ConditionOperator.LessThan},
                    {"is greater than", ConditionOperator.GreaterThan}
                };

        public static readonly IDictionary<string, ConditionOperator>
            _filterDateOperators = new Dictionary<string, ConditionOperator>
                {
                    {"is", ConditionOperator.Equal},
                    {"is not", ConditionOperator.NotEqual},
                    {"is after", ConditionOperator.GreaterThan},
                    {"is before", ConditionOperator.LessThan},
                    {"is in the last", ConditionOperator.IsInTheLast},
                    //{"is not in the last", ConditionOperator.Contains},
                    {"is in the next", ConditionOperator.IsInTheNext}//,
                    //{"is in the range of", ConditionOperator.Contains}
                };

        public static readonly IDictionary<string, ConditionOperator>
            _filterStringOperators = new Dictionary<string, ConditionOperator>
                {
                    {"contains", ConditionOperator.Contains},
                    //{"does not contain", ConditionOperator.NotEqual},
                    {"is", ConditionOperator.Equal},
                    {"is not", ConditionOperator.NotEqual}//,
                    //{"starts with", ConditionOperator.Contains},
                    //{"ends with", ConditionOperator.Contains}
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
            HardwareToken,
            LicensePacket
        }

        public enum ConditionNameType
        {
            Date,
            String,
            Number,
            Bool,
            Defined
        }

        public enum ExpirationValueType
        {
            days,
            weeks,
            months
        }
       
        public enum ValidationType
        {
            Hardware,
            Software
        }

        private string  _id;
        private ReportType _type;
        private List<Condition> _conditions;
        private CommunicationLink _commLink;
        private bool _matchAll;

        public ReportProperty(CommunicationLink commLink)
        {
            _conditions = new List<Condition>();
            _commLink = commLink;
            _matchAll = true;
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

        public List<Condition> DatabaseConditions
        {
            get { return GetDatabaseConditions(); }
        }

        public bool MatchAll
        {
            get { return _matchAll; }
            set { _matchAll = value; }
        }

        //user value to database value which is digits
        private List<Condition> GetDatabaseConditions()
        {
            List<Condition> dbConditionList = new List<Condition>();
            foreach (Condition userCondition in Conditions)
            {
                Condition dbCondition = new Condition();
                string strVal = userCondition.Value;
                if (_type == ReportType.LicenseServer || _type == ReportType.LicensePacket)
                {
                }
                else if (_type == ReportType.ProductLicense)
                {
                    if (userCondition.Name == Client.Creator.CreatorService.ConditionName.State)
                    {
                        try
                        {
                            int currentState = -1;
                            foreach (ProductLicenseState pls in Enum.GetValues(typeof(ProductLicenseState)))
                            {
                                if (userCondition.Value == Enums.GetEnumDescription(pls))
                                {
                                    currentState = (int)pls;
                                    break;
                                }
                            }
                            //convert description to productlicensestate
                            strVal = currentState.ToString(); 
                        }
                        catch (Exception) { }
                    }
                    else if (userCondition.Name == Client.Creator.CreatorService.ConditionName.Product)
                    {
                        strVal = _commLink.GetProductID(userCondition.Value).ToString();
                    }
                    else if (userCondition.Name == Client.Creator.CreatorService.ConditionName.Module)
                    {
                        //module = productid, modid
                        //issue don't know the product the module is associated with by knowning just the modID
                        uint productID = _commLink.GetProductIDByModuleName(userCondition.Value);
                        strVal = string.Format("{0},{1}",productID, _commLink.GetModuleID(productID, userCondition.Value));
                    }
                }
                else //HardwareToken
                {
                    if (userCondition.Name == Client.Creator.CreatorService.ConditionName.Customer)
                    {
                        ServiceProxy.Service<CreatorService.ICreator>.Use((client) =>
                        {
                            CreatorService.CustomerTable dbCustomer = client.GetCustomer(userCondition.Value, false);
                            if(dbCustomer != null)
                                strVal = dbCustomer.SCRnumber.ToString();
                        });
                    }
                    else if (userCondition.Name == Client.Creator.CreatorService.ConditionName.State)
                    {
                        try
                        {
                            strVal = ((int)Enum.Parse(typeof(TokenStatus), userCondition.Value)).ToString();
                        }
                        catch (Exception) { }
                    }
                }
                dbCondition.Name = userCondition.Name;
                dbCondition.Operator = userCondition.Operator;
                dbCondition.Value = strVal;
                dbCondition.ValueType = userCondition.ValueType;
                dbConditionList.Add(dbCondition);
            }
            return dbConditionList;
        }

        //date types - days, weeks, months
        public ConditionNameType GetConditionNameType(ConditionName cn)
        {
            //3 types - string -> bool + contains, date, number, bool
            ConditionNameType cnt = ConditionNameType.String;
            switch (cn)
            {
                case ConditionName.ActivatedDate:
                case ConditionName.DeactivatedDate:
                case ConditionName.ExpirationDate:
                case ConditionName.DateCreated:
                    return ConditionNameType.Date;
                    break;
                case ConditionName.Activation:
                case ConditionName.ActivationAmount:
                case ConditionName.Extension:
                case ConditionName.ModuleValue:
                    return ConditionNameType.Number;
                    break;
                case ConditionName.Customer:
                case ConditionName.HardwareID:
                case ConditionName.LicenseServer:
                case ConditionName.LicensePacket:
                case ConditionName.Module:
                case ConditionName.Notes:
                case ConditionName.ProductLicense:
                case ConditionName.ProductVersion:
                case ConditionName.VerifiedBy:
                    return ConditionNameType.String;
                    break;
                case ConditionName.Active:
                case ConditionName.Verified:
                    return ConditionNameType.Bool;
                    break;
                default : // ConditionName.Product, ConditionName.State(PL, HW), ConditionName.Validation
                    return ConditionNameType.Defined;
                    break;
            }
            return cnt;
        }

        public string[] GetDefinedValues(ConditionName cn)
        {
            List<string> definedValueList = new List<string>();
            if (cn == ConditionName.Product)
            {
                definedValueList.AddRange(CreatorForm.s_CommLink.GetProductNameList());
            }
            else if (cn == ConditionName.Validation)
            {
                definedValueList.AddRange(Enum.GetNames(typeof(ValidationType)));
            }
            else
            {
                if (_type == ReportType.ProductLicense)
                {
                    foreach(ProductLicenseState pls in Enum.GetValues(typeof(ProductLicenseState)))
                    {
                        definedValueList.Add(Enums.GetEnumDescription(pls));
                    }
                }
                else
                {
                    definedValueList.AddRange(Enum.GetNames(typeof(TokenStatus)));
                }
            }
            return definedValueList.ToArray();
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
                case ReportType.LicensePacket:
                     conditionName = _filterLPNames[filterName];
                     break;
                default: break;
            }
            return conditionName;
        }

        public ConditionOperator GetFilterOperator(ConditionName cn, string filterOperator)
        {
            ConditionOperator conditionOp = ConditionOperator.Equal;
            ConditionNameType cnt = GetConditionNameType(cn);
            switch (cnt)
            {
                case ConditionNameType.Date:
                    conditionOp = _filterDateOperators[filterOperator];
                    break;
                case ConditionNameType.Number:
                    conditionOp = _filterNumberOperators[filterOperator];
                    break;
                case ConditionNameType.String:
                    conditionOp = _filterStringOperators[filterOperator];
                    break;
                case ConditionNameType.Defined:
                    conditionOp = _filterOperators[filterOperator];
                    break;
                case ConditionNameType.Bool:
                    conditionOp = _filterBoolOperators[filterOperator];
                    break;
                default: break;
            }
            return conditionOp;
        }

        public string GetFilterKey(CreatorService.Condition userCondition)
        {
            string conditionString = "";
            IDictionary<string, ConditionName> filterNames = null;
            if (Type == ReportType.LicenseServer)
                filterNames = _filterLSNames;
            else if (Type == ReportType.ProductLicense)
                filterNames = _filterPLNames;
            else if (Type == ReportType.HardwareToken)
                filterNames = _filterHTNames;
            else
                filterNames = _filterLPNames;
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

        public string GetOperatorKey(CreatorService.Condition userCondition)
        {
            ConditionNameType cnt = GetConditionNameType(userCondition.Name);
            IDictionary<string, ConditionOperator> filterOperators = null;
            string conditionString = "";
            if (cnt == ConditionNameType.Date)
                filterOperators = _filterDateOperators;
            else if (cnt == ConditionNameType.Bool)
                filterOperators = _filterBoolOperators;
            else if (cnt == ConditionNameType.Number)
                filterOperators = _filterNumberOperators;
            else if (cnt == ConditionNameType.String)
                filterOperators = _filterStringOperators;
            else
                filterOperators = _filterOperators;
            foreach (KeyValuePair<string, ConditionOperator> kvp in filterOperators)
            {
                if (kvp.Value == userCondition.Operator)
                {
                    conditionString = kvp.Key;
                    break;
                }
            }
            return conditionString;
        }

        public ICollection<string> GetOperatorKeys(ConditionName cn)
        {
            ConditionNameType cnt = GetConditionNameType(cn);
            ICollection<string> operatorCollection = _filterOperators.Keys;
            switch(cnt)
            {
                case ConditionNameType.Bool:
                    return _filterBoolOperators.Keys;
                    break;
                case ConditionNameType.Date:
                    return _filterDateOperators.Keys;
                    break;
                case ConditionNameType.Number:
                    return _filterNumberOperators.Keys;
                    break;
                case ConditionNameType.String:
                    return _filterStringOperators.Keys;
                default: break; //return string operators
            }
            return operatorCollection;
        }

        public ICollection<string> GetNameKeys()
        {
            ICollection<string> nameCollection = _filterLSNames.Keys;
            switch (_type)
            {
                case ReportType.ProductLicense:
                    return _filterPLNames.Keys;
                    break;
                case ReportType.HardwareToken:
                    return _filterHTNames.Keys;
                    break;
                case ReportType.LicensePacket:
                    return _filterLPNames.Keys;
                    break;
                default: break; //return license server names
            }
            return nameCollection;
        }
    }

}
