using System;
using System.Collections;
using System.Collections.Specialized;

namespace Solimar
{
	namespace Licensing
	{
		namespace Attribs
		{
			public class Lic_KeyAttribs : LicensingAttribsBase
			{
				public static string ClassName = "L_KyA";
				
				public Lic_KeyAttribs() : 
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
					
					
					public AttribsMemberWORD activationSlotId = new AttribsMemberWORD("aS", 0);
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
					
				public class Lic_ActivationInfoAttribsList : AttribsMemberGenericList {public Lic_ActivationInfoAttribsList(string keyName, ArrayList defaultVal) : base(keyName, typeof(Lic_ActivationInfoAttribs), defaultVal){;} }
					
				public AttribsMemberBufferList layout = new AttribsMemberBufferList("lo", new ArrayList());
				public AttribsMemberString keyName = new AttribsMemberString("kN", "");
				public AttribsMemberDWORD keyVersion = new AttribsMemberDWORD("kV", 1);
				public AttribsMemberString licenseCode = new AttribsMemberString("lC", "");
				public AttribsMemberDateTime packetCreationDate = new AttribsMemberDateTime("pDt", AttribFormat.ConvertStringToDateTime("1900-01-01 00:00:00.0000"));
				public AttribsMemberDateTime currentDate = new AttribsMemberDateTime("cDt", AttribFormat.ConvertStringToDateTime("1900-01-01 00:00:00.0000"));
				public AttribsMemberWORD historyNumber = new AttribsMemberWORD("hN", 0);
				public Lic_ActivationInfoAttribsList activationInfoList = new Lic_ActivationInfoAttribsList("aiLt", new ArrayList());
			
			};
			
		}
	}
}
