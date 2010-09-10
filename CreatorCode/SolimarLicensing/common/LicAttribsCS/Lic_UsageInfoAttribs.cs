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
					
					
					public class Lic_UsModuleInfoAttribsList : AttribsMemberGenericList {public Lic_UsModuleInfoAttribsList(string keyName, ArrayList defaultVal) : base(keyName, typeof(Lic_UsModuleInfoAttribs), defaultVal){;} }
						
					public AttribsMemberString applicationInstance = new AttribsMemberString("aI", "");
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
