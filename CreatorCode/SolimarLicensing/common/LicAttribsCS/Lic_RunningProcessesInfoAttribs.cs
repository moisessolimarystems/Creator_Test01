using System;
using System.Collections;
using System.Collections.Specialized;

namespace Solimar
{
	namespace Licensing
	{
		namespace Attribs
		{
			public class Lic_RunningProcessesInfoAttribs : LicensingAttribsBase
			{
				public static string ClassName = "L_RPInA";
				
				public Lic_RunningProcessesInfoAttribs() : 
					base(ClassName)
				{
					;
				}
				
				public class Lic_ServicesInfoAttribs : LicensingAttribsBase
				{
					public static string ClassName = "L_SIA";
					
					public Lic_ServicesInfoAttribs() : 
						base(ClassName)
					{
						;
					}
					
					
					public AttribsMemberString name = new AttribsMemberString("nm", "");
					public AttribsMemberString path = new AttribsMemberString("pt", "");
					public AttribsMemberBOOL bStarted = new AttribsMemberBOOL("bS", false);
					public AttribsMemberString display = new AttribsMemberString("dn", "");
					public AttribsMemberString description = new AttribsMemberString("dc", "");
					public AttribsMemberString serviceType = new AttribsMemberString("st", "");
				
				};
				
				public class AttribsMemberAttribsClass_Lic_ServicesInfoAttribs : AttribsMemberAttribsClass
				{
					public AttribsMemberAttribsClass_Lic_ServicesInfoAttribs(string keyName, Lic_ServicesInfoAttribs defaultVal) : 
						base(keyName, defaultVal)
					{
						;
					}
					
					public static implicit operator Lic_ServicesInfoAttribs(AttribsMemberAttribsClass_Lic_ServicesInfoAttribs t)
					{
						return (Lic_ServicesInfoAttribs)t.m_tVal;
					}
				
					public Lic_ServicesInfoAttribs TVal
					{
						get {return this;}
						set {m_tVal = (Object)value;}
					}
				}
					
				public class Lic_ServicesInfoAttribsList : AttribsMemberGenericList {public Lic_ServicesInfoAttribsList(string keyName, ArrayList defaultVal) : base(keyName, typeof(Lic_ServicesInfoAttribs), defaultVal){;} }
					
				public Lic_ServicesInfoAttribsList servicesList = new Lic_ServicesInfoAttribsList("sLt", new ArrayList());
			
			};
			
		}
	}
}
