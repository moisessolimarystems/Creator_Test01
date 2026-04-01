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
				
				public class Lic_ServerDataFileInfoAttribs : LicensingAttribsBase
				{
					public static string ClassName = "fi";
					
					public Lic_ServerDataFileInfoAttribs() : 
						base(ClassName)
					{
						;
					}
					
					
					public AttribsMemberString LicFileName = new AttribsMemberString("fN", "");
					public AttribsMemberString LicFileVerificationCode = new AttribsMemberString("fV", "");
					public AttribsMemberString LicName = new AttribsMemberString("lB", "");
					public AttribsMemberDateTime LicModifiedDate = new AttribsMemberDateTime("mDt", AttribFormat.ConvertStringToDateTime("1900-01-01 00:00:00.0000"));
					public AttribsMemberDWORD LicCurrentActivations = new AttribsMemberDWORD("cA", 0);
				
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
					
				public Lic_ServerDataFileInfoAttribsList fileInfoList = new Lic_ServerDataFileInfoAttribsList("fiLt", new ArrayList());
				public AttribsMemberDWORD versionMajor = new AttribsMemberDWORD("vMj", 1);
				public AttribsMemberDWORD versionMinor = new AttribsMemberDWORD("vMn", 0);
				public AttribsMemberDWORD versionSubMajor = new AttribsMemberDWORD("vSMj", 0);
				public AttribsMemberDWORD versionSubMinor = new AttribsMemberDWORD("vSMn", 0);
				public AttribsMemberDateTime lastTouchDate = new AttribsMemberDateTime("lDt", AttribFormat.ConvertStringToDateTime("1900-01-01 00:00:00.0000"));
			
			};
			
		}
	}
}
