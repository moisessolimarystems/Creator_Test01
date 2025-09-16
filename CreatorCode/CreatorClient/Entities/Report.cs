// Report.cs
//
// SLB 15.sep.2025 CR.34456; Changes for new attribs code (Licensing 3.4+) to work.
// Refactored some if () {} else if() {} code to be switch() statements.
// Renamed class member variables to have m_ prefix vs. _. 

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Client.Creator.CreatorService;

namespace Client.Creator
{
	public class Report
	{

		public static readonly IDictionary<string, ConditionName>
			m_filterLPNames = new Dictionary<string, ConditionName>					// CR.34456; modified.
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
			m_filterHTNames = new Dictionary<string, ConditionName>					// CR.34456; modified.
				{  
					{"Activated Date", ConditionName.ActivatedDate},
					{"Customer", ConditionName.Customer},       
					{"Deactivated Date", ConditionName.DeactivatedDate},
					{"Hardware ID", ConditionName.HardwareID},
					{"License Server", ConditionName.LicenseServer},
					{"State", ConditionName.State}
				};

		public static readonly IDictionary<string, ConditionName>
			m_filterLSNames = new Dictionary<string, ConditionName>					// CR.34456; modified.
				{  
					{"Active", ConditionName.Active},
					{"Customer", ConditionName.Customer}, 
					{"Date Created", ConditionName.DateCreated},        
					{"License Server", ConditionName.LicenseServer},                   
					{"Notes", ConditionName.Notes},
					{"Validation", ConditionName.Validation},
					{"Verified", ConditionName.Verified},
					{"Token", ConditionName.Token}
				};

		public static readonly IDictionary<string, ConditionName>
			m_filterPLNames = new Dictionary<string, ConditionName>					// CR.34456; modified.
				{  
					{"Activation Amount",ConditionName.ActivationAmount}, 
					{"Activation Total", ConditionName.Activation},
					{"Active", ConditionName.Active},        
					{"Customer", ConditionName.Customer}, 
					{"Date Created", ConditionName.DateCreated},  
					{"Extension Count", ConditionName.Extension},
					{"Expiration Date", ConditionName.ExpirationDate},
					{"Module", ConditionName.Module},
					{"Module Value", ConditionName.ModuleValue},
					{"Notes", ConditionName.Notes},
					{"Product", ConditionName.Product},
					{"Product License", ConditionName.ProductLicense},
					{"Product Version", ConditionName.ProductVersion},
					{"State", ConditionName.State},
					{"Validation", ConditionName.Validation},
					{"Verified", ConditionName.Verified},
					{"Type", ConditionName.ProductLicenseType}
				};

		public static readonly IDictionary<string, ConditionOperator>
			m_filterOperators = new Dictionary<string, ConditionOperator>			// CR.34456; modified.
				{
					{"is", ConditionOperator.Equal},
					{"is not", ConditionOperator.NotEqual}
				};

		public static readonly IDictionary<string, ConditionOperator>
			m_filterNumberOperators = new Dictionary<string, ConditionOperator>		// CR.34456; modified.
				{
					{"is", ConditionOperator.Equal},
					{"is not", ConditionOperator.NotEqual},
					{"is less than", ConditionOperator.LessThan},
					{"is greater than", ConditionOperator.GreaterThan}
				};

		public static readonly IDictionary<string, ConditionOperator>
			m_filterDateOperators = new Dictionary<string, ConditionOperator>		// CR.34456; modified.
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
			m_filterStringOperators = new Dictionary<string, ConditionOperator>		// CR.34456; modified.
				{
					{"contains", ConditionOperator.Contains},
					//{"does not contain", ConditionOperator.NotEqual},
					{"is", ConditionOperator.Equal},
					{"is not", ConditionOperator.NotEqual}//,
					//{"starts with", ConditionOperator.Contains},
					//{"ends with", ConditionOperator.Contains}
				};

		public static readonly IDictionary<string, ConditionOperator>
			m_filterBoolOperators = new Dictionary<string, ConditionOperator>		// CR.34456; modified.
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

		private string m_id;														// CR.34456; modified.
		private ReportType m_type;													// CR.34456; modified.
		private List<Condition> m_conditions;										// CR.34456; modified.
		private CommunicationLink m_commLink;										// CR.34456; modified.
		private bool m_matchAll;													// CR.34456; modified.

		public Report(CommunicationLink commLink)
		{
			m_conditions = new List<Condition>();
			m_commLink = commLink;
			m_matchAll = true;
		}

		public string ID
		{
			get { return m_id; }
			set { m_id = value; }
		}

		public ReportType Type
		{
			get { return m_type; }
			set { m_type = value; }
		}

		public List<Condition> Conditions
		{
			get { return m_conditions; }
			set { m_conditions = value; }
		}

		public List<Condition> DatabaseConditions
		{
			get { return GetDatabaseConditions(); }
		}

		public bool MatchAll
		{
			get { return m_matchAll; }
			set { m_matchAll = value; }
		}

		//user value to database value which is digits
		private List<Condition> GetDatabaseConditions()
		{
			List<Condition> dbConditionList = new List<Condition>();
			foreach (Condition userCondition in Conditions)
			{
				Condition dbCondition = new Condition();
				string strVal = userCondition.Value;
				if (m_type == ReportType.LicenseServer || m_type == ReportType.LicensePacket)
				{
				}
				else if (m_type == ReportType.ProductLicense)
				{
					if (userCondition.Name == ConditionName.State)
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
					else if (userCondition.Name == ConditionName.Product)
					{
						strVal = m_commLink.GetProductID(userCondition.Value).ToString();
					}
					else if (userCondition.Name == ConditionName.Module)
					{
						//module = productid, modid
						//issue don't know the product the module is associated with by knowning just the modID
						uint productID = m_commLink.GetProductIDByModuleName(userCondition.Value);
						strVal = string.Format("{0},{1}",productID, m_commLink.GetModuleID(productID, userCondition.Value));
					}
					else if (userCondition.Name == ConditionName.ProductLicenseType)
					{
						strVal = string.Format("{0}", ProductLicense.GetProductLicenseTypeID(userCondition.Value));                         
					}
				}
				else //HardwareToken
				{
					if (userCondition.Name == ConditionName.Customer)
					{
						ServiceProxy.Service<CreatorService.ICreator>.Use((client) =>
						{
							CreatorService.CustomerTable dbCustomer = client.GetCustomer(userCondition.Value, false);
							if(dbCustomer != null)
								strVal = dbCustomer.SCRnumber.ToString();
						});
					}
					else if (userCondition.Name == ConditionName.State)
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
			//ConditionNameType cnt = ConditionNameType.String;						// CR.34456; commented.
			switch (cn)
			{
				case ConditionName.ActivatedDate:
				case ConditionName.DeactivatedDate:
				case ConditionName.ExpirationDate:
				case ConditionName.DateCreated:
					return ConditionNameType.Date;
				case ConditionName.Activation:
				case ConditionName.ActivationAmount:
				case ConditionName.Extension:
				case ConditionName.ModuleValue:
					return ConditionNameType.Number;
				case ConditionName.Customer:
				case ConditionName.HardwareID:
				case ConditionName.LicenseServer:
				case ConditionName.LicensePacket:
				case ConditionName.Notes:
				case ConditionName.ProductLicense:
				case ConditionName.ProductVersion:
				case ConditionName.VerifiedBy:
				case ConditionName.Token:
				//case ConditionName.Module:
					return ConditionNameType.String;
				case ConditionName.Active:
				case ConditionName.Verified:
					return ConditionNameType.Bool;
			}

			// ConditionName.Product, ConditionName.State(PL, HW), ConditionName.Validation, ConditionName.Module
			return ConditionNameType.Defined;										// CR.34456; moved.
		}

		public string[] GetDefinedValues(ConditionName cn)
		{
			List<string> definedValueList = new List<string>();
			if (cn == ConditionName.Product)
			{
				definedValueList.AddRange(m_commLink.GetProductNameList());
			}
			else if (cn == ConditionName.Validation)
			{
				definedValueList.AddRange(Enum.GetNames(typeof(ValidationType)));
			}
			else if (cn == ConditionName.ProductLicenseType)
			{
				definedValueList.AddRange(ProductLicense.ProductLicenseTypeList);
			}
			else if (cn == ConditionName.Module)
			{
				definedValueList.AddRange(m_commLink.GetAllModuleNamesList());
			}
			else
			{
				if (m_type == ReportType.ProductLicense)
				{
					foreach (ProductLicenseState pls in Enum.GetValues(typeof(ProductLicenseState)))
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
					conditionName = m_filterLSNames[filterName];
					break;
				case ReportType.ProductLicense:
					conditionName = m_filterPLNames[filterName];
						break;
				case ReportType.HardwareToken:
					conditionName = m_filterHTNames[filterName];
						break;
				case ReportType.LicensePacket:
						conditionName = m_filterLPNames[filterName];
						break;
				default: break;
			}
			return conditionName;
		}

		public ConditionOperator GetFilterOperator(ConditionName cn, string filterOperator)
		{
			ConditionNameType cnt = GetConditionNameType(cn);
			switch (cnt)
			{
				case ConditionNameType.Date: return m_filterDateOperators[filterOperator];       // CR.34456; modified.
				case ConditionNameType.Number: return m_filterNumberOperators[filterOperator];   // CR.34456; modified.
				case ConditionNameType.String: return m_filterStringOperators[filterOperator];   // CR.34456; modified.
				case ConditionNameType.Defined: return m_filterOperators[filterOperator];		// CR.34456; modified.
				case ConditionNameType.Bool: return m_filterBoolOperators[filterOperator];		// CR.34456; modified.
			}
			return ConditionOperator.Equal;
		}

		public string GetFilterKey(Condition userCondition)
		{
			IDictionary<string, ConditionName> filterNames = null;
			switch(Type)
			{
				case ReportType.LicenseServer: filterNames = m_filterLSNames; break; // CR.34456; modified.
				case ReportType.ProductLicense: filterNames = m_filterPLNames; break; // CR.34456; modified.
				case ReportType.HardwareToken: filterNames = m_filterHTNames; break; // CR.34456; modified.
				default: filterNames = m_filterLPNames; break;						// CR.34456; modified.
			}

			foreach (KeyValuePair<string, ConditionName> kvp in filterNames)
			{
				if (kvp.Value != userCondition.Name) continue;						// CR.34456; modified.

				// return the condition string
				return kvp.Key;														// CR.34456; modified.
			}
			return string.Empty;
		}

		public string GetOperatorKey(Condition userCondition)
		{
			ConditionNameType cnt = GetConditionNameType(userCondition.Name); 
			IDictionary<string, ConditionOperator> filterOperators = null;
			switch (cnt)															// CR.34456; added.
			{
				case ConditionNameType.Date: filterOperators = m_filterDateOperators; break; // CR.34456; modified.
				case ConditionNameType.Bool: filterOperators = m_filterBoolOperators; break; // CR.34456; modified.
				case ConditionNameType.Number: filterOperators = m_filterNumberOperators; break; // CR.34456; modified.

				//special case for modules that are a string type, but can't use "contain" operator
				case ConditionNameType.String: filterOperators = m_filterStringOperators; break; // CR.34456; modified.
				default: filterOperators = m_filterOperators; break;					// CR.34456; modified.
			}
          
			foreach (KeyValuePair<string, ConditionOperator> kvp in filterOperators)
			{
				if (kvp.Value != userCondition.Operator) continue;					// CR.34456; modified.

				// return the condition string
				return kvp.Key;														// CR.34456; modified.
			}
			return string.Empty;													// CR.34456; modified.
		}

		public ICollection<string> GetOperatorKeys(ConditionName cn)
		{            
			ConditionNameType cnt = GetConditionNameType(cn);
			ICollection<string> operatorCollection = m_filterOperators.Keys;
			switch(cnt)
			{
				case ConditionNameType.Bool: 
					return m_filterBoolOperators.Keys;
				case ConditionNameType.Date:
					return m_filterDateOperators.Keys;
				case ConditionNameType.Number:
					return m_filterNumberOperators.Keys;
				case ConditionNameType.String:
					if (cn == ConditionName.Module) //special case for modules, can't use "Contain" op
						return m_filterOperators.Keys;
					return m_filterStringOperators.Keys;
			}
			return operatorCollection;
		}

		public ICollection<string> GetNameKeys()
		{
			ICollection<string> nameCollection = m_filterLSNames.Keys;
			switch (m_type)
			{
				case ReportType.ProductLicense: return m_filterPLNames.Keys;			// CR.34456; modified.
				case ReportType.HardwareToken: return m_filterHTNames.Keys;			// CR.34456; modified.
				case ReportType.LicensePacket: return m_filterLPNames.Keys;			// CR.34456; modified.
			}
			//return license server names
			return nameCollection;
		}
	}

}
