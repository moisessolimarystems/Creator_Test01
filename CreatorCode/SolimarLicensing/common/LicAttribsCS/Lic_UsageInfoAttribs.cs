using System;
using System.Collections;
using System.Collections.Specialized;

namespace Solimar
{
	namespace Licensing
	{
		namespace Attribs
		{
			public class Lic_UsageInfoAttribs : LicensingAttribsBase
			{
				public static string ClassName = "L_UsInA";
				
				public Lic_UsageInfoAttribs() : 
					base(ClassName)
				{
					;
				}
				
				public class Lic_UsModuleInfoAttribs : LicensingAttribsBase
				{
					public static string ClassName = "L_UMA";
					
					public Lic_UsModuleInfoAttribs() : 
						base(ClassName)
					{
						;
					}
					
					
					public AttribsMemberDWORD moduleID = new AttribsMemberDWORD("mI", 0);
					public AttribsMemberDWORD moduleUsage = new AttribsMemberDWORD("mU", 0);
					public AttribsMemberDWORD moduleTotal = new AttribsMemberDWORD("mT", 0);
				
				};
				
				public class AttribsMemberAttribsClass_Lic_UsModuleInfoAttribs : AttribsMemberAttribsClass
				{
					public AttribsMemberAttribsClass_Lic_UsModuleInfoAttribs(string keyName, Lic_UsModuleInfoAttribs defaultVal) : 
						base(keyName, defaultVal)
					{
						;
					}
					
					public static implicit operator Lic_UsModuleInfoAttribs(AttribsMemberAttribsClass_Lic_UsModuleInfoAttribs t)
					{
						return (Lic_UsModuleInfoAttribs)t.m_tVal;
					}
				
					public Lic_UsModuleInfoAttribs TVal
					{
						get {return this;}
						set {m_tVal = (Object)value;}
					}
				}
					public class Lic_UsAppInstanceInfoAttribs : LicensingAttribsBase
				{
					public static string ClassName = "L_UAA";
					
					public Lic_UsAppInstanceInfoAttribs() : 
						base(ClassName)
					{
						;
					}
					
					[FlagsAttribute]
					public enum TUsageFlag : uint
					{
						ufNone = 0,
						ufUsePrimaryLic = 1,
						ufUseFailoverLic = 2,
					};
					
					public class AttribsMemberEnum_TUsageFlag : AttribsMemberEnum
					{
						public AttribsMemberEnum_TUsageFlag(string keyName, TUsageFlag defaultVal) :
							base(keyName, defaultVal, typeof(TUsageFlag))
						{
							;
						}
						
						protected static SortedList m_MapAliasToEnum;
						protected static SortedList m_MapEnumToAlias;
						protected static SortedList m_MapOrderingIndexToAlias;
						protected static SortedList m_MapAliasToIndex;
						protected static SortedList m_MapEnumToIndex;
						static AttribsMemberEnum_TUsageFlag()
						{
							m_MapAliasToEnum = new SortedList();
							m_MapEnumToAlias = new SortedList();
							m_MapOrderingIndexToAlias = new SortedList();	// map of the ordering indexes from the xml file to aliases. The ordering indexes are not guranteed to be continuous or 0 based. 
							m_MapAliasToIndex = new SortedList();	// This is different than the ordering index. This function takes an alias and returns what index it is in the GetAliases() list. 
							m_MapEnumToIndex = new SortedList();	// This is different than the ordering index. This function takes an alias and returns what index it is in the GetAliases() list. 
							m_MapAliasToEnum.Add("None",TUsageFlag.ufNone);
							m_MapEnumToAlias.Add(TUsageFlag.ufNone,"None");
							m_MapOrderingIndexToAlias.Add(1,"None");
							m_MapAliasToEnum.Add("Using Primary Licensing",TUsageFlag.ufUsePrimaryLic);
							m_MapEnumToAlias.Add(TUsageFlag.ufUsePrimaryLic,"Using Primary Licensing");
							m_MapOrderingIndexToAlias.Add(2,"Using Primary Licensing");
							m_MapAliasToEnum.Add("Using Failover Licensing",TUsageFlag.ufUseFailoverLic);
							m_MapEnumToAlias.Add(TUsageFlag.ufUseFailoverLic,"Using Failover Licensing");
							m_MapOrderingIndexToAlias.Add(3,"Using Failover Licensing");
							m_MapAliasToIndex.Add("None",0);
							m_MapAliasToIndex.Add("Using Primary Licensing",1);
							m_MapAliasToIndex.Add("Using Failover Licensing",2);
							m_MapEnumToIndex.Add(TUsageFlag.ufNone,0);
							m_MapEnumToIndex.Add(TUsageFlag.ufUsePrimaryLic,1);
							m_MapEnumToIndex.Add(TUsageFlag.ufUseFailoverLic,2);
						}
						
					
						public static string GetAlias(System.Enum enum_value)
						{
							return (string)m_MapEnumToAlias[enum_value];
						}
						public static StringCollection GetAliases()
						{
							StringCollection alias_list = new StringCollection();
							foreach (string alias in m_MapOrderingIndexToAlias.Values)
							{
								alias_list.Add(alias);
							}
							return alias_list;
						}
						public static System.Enum GetEnumValueFromAlias(string alias)
						{
							return (System.Enum)m_MapAliasToEnum[alias];
						}
						public static int GetIndexFromAlias(string alias)
						{
							return (int)m_MapAliasToIndex[alias];
						}
						public static int GetIndexFromEnum(System.Enum enum_value)
						{
							return (int)m_MapEnumToIndex[enum_value];
						}
						
						public string GetAlias()
						{
							return GetAlias(EVal);
						}
						public void SetEnumValueFromAlias(string alias)
						{
							EVal = GetEnumValueFromAlias(alias);
						}
					
						public static implicit operator TUsageFlag(AttribsMemberEnum_TUsageFlag t)
						{
							return (TUsageFlag)t.m_tVal;
						}
						
						public TUsageFlag TVal
						{
							get {return this;}
							set {m_tVal = (Object)value;}
						}
					}
					
						
					public class Lic_UsModuleInfoAttribsList : AttribsMemberGenericList {public Lic_UsModuleInfoAttribsList(string keyName, ArrayList defaultVal) : base(keyName, typeof(Lic_UsModuleInfoAttribs), defaultVal){;} }
						
					public AttribsMemberString applicationInstance = new AttribsMemberString("aI", "");
					public AttribsMemberEnum_TUsageFlag usageFlag = new AttribsMemberEnum_TUsageFlag("uF", TUsageFlag.ufNone);
					public Lic_UsModuleInfoAttribsList moduleList = new Lic_UsModuleInfoAttribsList("mLt", new ArrayList());
				
				};
				
				public class AttribsMemberAttribsClass_Lic_UsAppInstanceInfoAttribs : AttribsMemberAttribsClass
				{
					public AttribsMemberAttribsClass_Lic_UsAppInstanceInfoAttribs(string keyName, Lic_UsAppInstanceInfoAttribs defaultVal) : 
						base(keyName, defaultVal)
					{
						;
					}
					
					public static implicit operator Lic_UsAppInstanceInfoAttribs(AttribsMemberAttribsClass_Lic_UsAppInstanceInfoAttribs t)
					{
						return (Lic_UsAppInstanceInfoAttribs)t.m_tVal;
					}
				
					public Lic_UsAppInstanceInfoAttribs TVal
					{
						get {return this;}
						set {m_tVal = (Object)value;}
					}
				}
					public class Lic_UsProductInfoAttribs : LicensingAttribsBase
				{
					public static string ClassName = "L_UPA";
					
					public Lic_UsProductInfoAttribs() : 
						base(ClassName)
					{
						;
					}
					
					
					public class Lic_UsAppInstanceInfoAttribsList : AttribsMemberGenericList {public Lic_UsAppInstanceInfoAttribsList(string keyName, ArrayList defaultVal) : base(keyName, typeof(Lic_UsAppInstanceInfoAttribs), defaultVal){;} }
						
					public AttribsMemberDWORD productID = new AttribsMemberDWORD("pI", 0);
					public Lic_UsAppInstanceInfoAttribsList appInstanceList = new Lic_UsAppInstanceInfoAttribsList("aLt", new ArrayList());
				
				};
				
				public class AttribsMemberAttribsClass_Lic_UsProductInfoAttribs : AttribsMemberAttribsClass
				{
					public AttribsMemberAttribsClass_Lic_UsProductInfoAttribs(string keyName, Lic_UsProductInfoAttribs defaultVal) : 
						base(keyName, defaultVal)
					{
						;
					}
					
					public static implicit operator Lic_UsProductInfoAttribs(AttribsMemberAttribsClass_Lic_UsProductInfoAttribs t)
					{
						return (Lic_UsProductInfoAttribs)t.m_tVal;
					}
				
					public Lic_UsProductInfoAttribs TVal
					{
						get {return this;}
						set {m_tVal = (Object)value;}
					}
				}
					
				public class Lic_UsProductInfoAttribsList : AttribsMemberGenericList {public Lic_UsProductInfoAttribsList(string keyName, ArrayList defaultVal) : base(keyName, typeof(Lic_UsProductInfoAttribs), defaultVal){;} }
					
				public Lic_UsProductInfoAttribsList productList = new Lic_UsProductInfoAttribsList("pLt", new ArrayList());
			
			};
			
		}
	}
}
