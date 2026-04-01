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
			
			};
			
		}
	}
}
