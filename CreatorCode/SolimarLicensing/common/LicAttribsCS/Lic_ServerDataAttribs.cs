using System;
using System.Collections;
using System.Collections.Specialized;

namespace Solimar
{
	namespace Licensing
	{
		namespace Attribs
		{
			public class Lic_ServerDataAttribs : LicensingAttribsBase
			{
				public static string ClassName = "L_SvrDtA";
				
				public Lic_ServerDataAttribs() : 
					base(ClassName)
				{
					;
				}
				
				public class Lic_ClockViolationInfoAttribs : LicensingAttribsBase
				{
					public static string ClassName = "cV";
					
					public Lic_ClockViolationInfoAttribs() : 
						base(ClassName)
					{
						;
					}
					
					
					public AttribsMemberDateTime fileDate = new AttribsMemberDateTime("fDt", AttribFormat.ConvertStringToDateTime("1900-01-01 00:00:00.0000"));
					public AttribsMemberDateTime systemDate = new AttribsMemberDateTime("sDt", AttribFormat.ConvertStringToDateTime("1900-01-01 00:00:00.0000"));
				
				};
				
				public class AttribsMemberAttribsClass_Lic_ClockViolationInfoAttribs : AttribsMemberAttribsClass
				{
					public AttribsMemberAttribsClass_Lic_ClockViolationInfoAttribs(string keyName, Lic_ClockViolationInfoAttribs defaultVal) : 
						base(keyName, defaultVal)
					{
						;
					}
					
					public static implicit operator Lic_ClockViolationInfoAttribs(AttribsMemberAttribsClass_Lic_ClockViolationInfoAttribs t)
					{
						return (Lic_ClockViolationInfoAttribs)t.m_tVal;
					}
				
					public Lic_ClockViolationInfoAttribs TVal
					{
						get {return this;}
						set {m_tVal = (Object)value;}
					}
				}
					public class Lic_ServerDataFileInfoAttribs : LicensingAttribsBase
				{
					public static string ClassName = "fi";
					
					public Lic_ServerDataFileInfoAttribs() : 
						base(ClassName)
					{
						;
					}
					
					public class Lic_ActivationInfoAttribs : LicensingAttribsBase
					{
						public static string ClassName = "ai";
						
						public Lic_ActivationInfoAttribs() : 
							base(ClassName)
						{
							;
						}
						
						
						public AttribsMemberString productLicenseNumber = new AttribsMemberString("pLB", "");
						public AttribsMemberWORD activationSlotId = new AttribsMemberWORD("aI", 0);
						public AttribsMemberWORD activationSlotCurrentActivation = new AttribsMemberWORD("aC", 0);
						public AttribsMemberWORD activationSlotHoursToExpire = new AttribsMemberWORD("aE", 0);
					
					};
					
					public class AttribsMemberAttribsClass_Lic_ActivationInfoAttribs : AttribsMemberAttribsClass
					{
						public AttribsMemberAttribsClass_Lic_ActivationInfoAttribs(string keyName, Lic_ActivationInfoAttribs defaultVal) : 
							base(keyName, defaultVal)
						{
							;
						}
						
						public static implicit operator Lic_ActivationInfoAttribs(AttribsMemberAttribsClass_Lic_ActivationInfoAttribs t)
						{
							return (Lic_ActivationInfoAttribs)t.m_tVal;
						}
					
						public Lic_ActivationInfoAttribs TVal
						{
							get {return this;}
							set {m_tVal = (Object)value;}
						}
					}
						
					public AttribsMemberString LicFileName = new AttribsMemberString("fN", "");
					public AttribsMemberString LicFileLicenseCode = new AttribsMemberString("fV", "");
					public AttribsMemberString LicName = new AttribsMemberString("lB", "");
					public AttribsMemberDateTime LicModifiedDate = new AttribsMemberDateTime("mDt", AttribFormat.ConvertStringToDateTime("1900-01-01 00:00:00.0000"));
					public AttribsMemberBOOL bHideFromUI = new AttribsMemberBOOL("bHide", false);
					public AttribsMemberDateTime LicCurrentDate = new AttribsMemberDateTime("cDt", AttribFormat.ConvertStringToDateTime("1900-01-01 00:00:00.0000"));
					public AttribsMemberString Streamed_ActivationAttribs = new AttribsMemberString("saA", "");
				
				};
				
				public class AttribsMemberAttribsClass_Lic_ServerDataFileInfoAttribs : AttribsMemberAttribsClass
				{
					public AttribsMemberAttribsClass_Lic_ServerDataFileInfoAttribs(string keyName, Lic_ServerDataFileInfoAttribs defaultVal) : 
						base(keyName, defaultVal)
					{
						;
					}
					
					public static implicit operator Lic_ServerDataFileInfoAttribs(AttribsMemberAttribsClass_Lic_ServerDataFileInfoAttribs t)
					{
						return (Lic_ServerDataFileInfoAttribs)t.m_tVal;
					}
				
					public Lic_ServerDataFileInfoAttribs TVal
					{
						get {return this;}
						set {m_tVal = (Object)value;}
					}
				}
					
				public class Lic_ServerDataFileInfoAttribsList : AttribsMemberGenericList {public Lic_ServerDataFileInfoAttribsList(string keyName, ArrayList defaultVal) : base(keyName, typeof(Lic_ServerDataFileInfoAttribs), defaultVal){;} }
					
				public class Lic_ClockViolationInfoAttribsList : AttribsMemberGenericList {public Lic_ClockViolationInfoAttribsList(string keyName, ArrayList defaultVal) : base(keyName, typeof(Lic_ClockViolationInfoAttribs), defaultVal){;} }
					public class Lic_AlertInfoAttribs : LicensingAttribsBase
				{
					public static string ClassName = "aia";
					
					public Lic_AlertInfoAttribs() : 
						base(ClassName)
					{
						;
					}
					
					public class Lic_EmailAlertMailAttribs : LicensingAttribsBase
					{
						public static string ClassName = "eA";
						
						public Lic_EmailAlertMailAttribs() : 
							base(ClassName)
						{
							;
						}
						
						
						public AttribsMemberString id = new AttribsMemberString("eID", "");
						public AttribsMemberString name = new AttribsMemberString("eN", "");
						public AttribsMemberBOOL bActive = new AttribsMemberBOOL("bA", true);
						public AttribsMemberStringList recipentsList = new AttribsMemberStringList("rL", new ArrayList());
						public AttribsMemberDWORDList eventIdList = new AttribsMemberDWORDList("eL", new ArrayList());
					
					};
					
					public class AttribsMemberAttribsClass_Lic_EmailAlertMailAttribs : AttribsMemberAttribsClass
					{
						public AttribsMemberAttribsClass_Lic_EmailAlertMailAttribs(string keyName, Lic_EmailAlertMailAttribs defaultVal) : 
							base(keyName, defaultVal)
						{
							;
						}
						
						public static implicit operator Lic_EmailAlertMailAttribs(AttribsMemberAttribsClass_Lic_EmailAlertMailAttribs t)
						{
							return (Lic_EmailAlertMailAttribs)t.m_tVal;
						}
					
						public Lic_EmailAlertMailAttribs TVal
						{
							get {return this;}
							set {m_tVal = (Object)value;}
						}
					}
						
					public class Lic_EmailAlertMailAttribsList : AttribsMemberGenericList {public Lic_EmailAlertMailAttribsList(string keyName, ArrayList defaultVal) : base(keyName, typeof(Lic_EmailAlertMailAttribs), defaultVal){;} }
						public class Lic_AlertMailServerAttribs : LicensingAttribsBase
					{
						public static string ClassName = "aMS";
						
						public Lic_AlertMailServerAttribs() : 
							base(ClassName)
						{
							;
						}
						
						[FlagsAttribute]
						public enum TAuthenticationType : uint
						{
							ttAnonymous = 0,
							ttBasic = 1,
						};
						
						public class AttribsMemberEnum_TAuthenticationType : AttribsMemberEnum
						{
							public AttribsMemberEnum_TAuthenticationType(string keyName, TAuthenticationType defaultVal) :
								base(keyName, defaultVal, typeof(TAuthenticationType))
							{
								;
							}
							
							protected static SortedList m_MapAliasToEnum;
							protected static SortedList m_MapEnumToAlias;
							protected static SortedList m_MapOrderingIndexToAlias;
							protected static SortedList m_MapAliasToIndex;
							protected static SortedList m_MapEnumToIndex;
							static AttribsMemberEnum_TAuthenticationType()
							{
								m_MapAliasToEnum = new SortedList();
								m_MapEnumToAlias = new SortedList();
								m_MapOrderingIndexToAlias = new SortedList();	// map of the ordering indexes from the xml file to aliases. The ordering indexes are not guranteed to be continuous or 0 based. 
								m_MapAliasToIndex = new SortedList();	// This is different than the ordering index. This function takes an alias and returns what index it is in the GetAliases() list. 
								m_MapEnumToIndex = new SortedList();	// This is different than the ordering index. This function takes an alias and returns what index it is in the GetAliases() list. 
								m_MapAliasToEnum.Add("Anonymous",TAuthenticationType.ttAnonymous);
								m_MapEnumToAlias.Add(TAuthenticationType.ttAnonymous,"Anonymous");
								m_MapOrderingIndexToAlias.Add(1,"Anonymous");
								m_MapAliasToEnum.Add("Basic",TAuthenticationType.ttBasic);
								m_MapEnumToAlias.Add(TAuthenticationType.ttBasic,"Basic");
								m_MapOrderingIndexToAlias.Add(2,"Basic");
								m_MapAliasToIndex.Add("Anonymous",0);
								m_MapAliasToIndex.Add("Basic",1);
								m_MapEnumToIndex.Add(TAuthenticationType.ttAnonymous,0);
								m_MapEnumToIndex.Add(TAuthenticationType.ttBasic,1);
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
						
							public static implicit operator TAuthenticationType(AttribsMemberEnum_TAuthenticationType t)
							{
								return (TAuthenticationType)t.m_tVal;
							}
							
							public TAuthenticationType TVal
							{
								get {return this;}
								set {m_tVal = (Object)value;}
							}
						}
						
							
						public AttribsMemberString mailServerName = new AttribsMemberString("msn", "");
						public AttribsMemberString fromEmail = new AttribsMemberString("fe", "");
						public AttribsMemberString fromDisplayName = new AttribsMemberString("fD", "");
						public AttribsMemberDWORD portNumber = new AttribsMemberDWORD("pn", 25);
						public AttribsMemberEnum_TAuthenticationType authenticationType = new AttribsMemberEnum_TAuthenticationType("aT", TAuthenticationType.ttAnonymous);
						public AttribsMemberString authBasicUserName = new AttribsMemberString("uN", "");
						public AttribsMemberString authBasicUserPassword = new AttribsMemberString("uP", "");
					
					};
					
					public class AttribsMemberAttribsClass_Lic_AlertMailServerAttribs : AttribsMemberAttribsClass
					{
						public AttribsMemberAttribsClass_Lic_AlertMailServerAttribs(string keyName, Lic_AlertMailServerAttribs defaultVal) : 
							base(keyName, defaultVal)
						{
							;
						}
						
						public static implicit operator Lic_AlertMailServerAttribs(AttribsMemberAttribsClass_Lic_AlertMailServerAttribs t)
						{
							return (Lic_AlertMailServerAttribs)t.m_tVal;
						}
					
						public Lic_AlertMailServerAttribs TVal
						{
							get {return this;}
							set {m_tVal = (Object)value;}
						}
					}
						
					public Lic_EmailAlertMailAttribsList emailAlertsList = new Lic_EmailAlertMailAttribsList("emLt", new ArrayList());
					public AttribsMemberAttribsClass_Lic_AlertMailServerAttribs mailServer = new AttribsMemberAttribsClass_Lic_AlertMailServerAttribs("mS", new Lic_AlertMailServerAttribs());
				
				};
				
				public class AttribsMemberAttribsClass_Lic_AlertInfoAttribs : AttribsMemberAttribsClass
				{
					public AttribsMemberAttribsClass_Lic_AlertInfoAttribs(string keyName, Lic_AlertInfoAttribs defaultVal) : 
						base(keyName, defaultVal)
					{
						;
					}
					
					public static implicit operator Lic_AlertInfoAttribs(AttribsMemberAttribsClass_Lic_AlertInfoAttribs t)
					{
						return (Lic_AlertInfoAttribs)t.m_tVal;
					}
				
					public Lic_AlertInfoAttribs TVal
					{
						get {return this;}
						set {m_tVal = (Object)value;}
					}
				}
					
				public Lic_ServerDataFileInfoAttribsList fileInfoList = new Lic_ServerDataFileInfoAttribsList("fiLt", new ArrayList());
				public Lic_ClockViolationInfoAttribsList clockViolHistoryList = new Lic_ClockViolationInfoAttribsList("cVLt", new ArrayList());
				public AttribsMemberDWORD versionMajor = new AttribsMemberDWORD("vMj", 1);
				public AttribsMemberDWORD versionMinor = new AttribsMemberDWORD("vMn", 0);
				public AttribsMemberDWORD versionSubMajor = new AttribsMemberDWORD("vSMj", 0);
				public AttribsMemberDWORD versionSubMinor = new AttribsMemberDWORD("vSMn", 0);
				public AttribsMemberDateTime lastTouchDate = new AttribsMemberDateTime("lDt", AttribFormat.ConvertStringToDateTime("1900-01-01 00:00:00.0000"));
				public AttribsMemberDWORD clockViolCount = new AttribsMemberDWORD("cVC", 0);
				public AttribsMemberDateTime clockViolLastDate = new AttribsMemberDateTime("cVD", AttribFormat.ConvertStringToDateTime("1900-01-01 00:00:00.0000"));
				public AttribsMemberBOOL bInClockViol = new AttribsMemberBOOL("iCV", false);
				public AttribsMemberAttribsClass_Lic_AlertInfoAttribs alertInfo = new AttribsMemberAttribsClass_Lic_AlertInfoAttribs("aI", new Lic_AlertInfoAttribs());
				public AttribsMemberString streamed_EmulationInfoAttribs = new AttribsMemberString("sEA", "");
			
			};
			
		}
	}
}
