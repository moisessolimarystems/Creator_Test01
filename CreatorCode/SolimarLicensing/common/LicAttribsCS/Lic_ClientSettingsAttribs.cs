using System;
using System.Collections;
using System.Collections.Specialized;

namespace Solimar
{
	namespace Licensing
	{
		namespace Attribs
		{
			public class Lic_ClientSettingsAttribs : LicensingAttribsBase
			{
				public static string ClassName = "L_csA";
				
				public Lic_ClientSettingsAttribs() : 
					base(ClassName)
				{
					;
				}
				
				public class Lic_ProductConnectionAttribs : LicensingAttribsBase
				{
					public static string ClassName = "L_pcA";
					
					public Lic_ProductConnectionAttribs() : 
						base(ClassName)
					{
						;
					}
					
					
					public AttribsMemberDWORD productID = new AttribsMemberDWORD("pI", 0);
					public AttribsMemberString serverName = new AttribsMemberString("sn", "");
					public AttribsMemberString backupServerName = new AttribsMemberString("bsn", "");
					public AttribsMemberBOOL bTestDev = new AttribsMemberBOOL("bTd", false);
					public AttribsMemberBOOL bInstalled = new AttribsMemberBOOL("bI", false);
				
				};
				
				public class AttribsMemberAttribsClass_Lic_ProductConnectionAttribs : AttribsMemberAttribsClass
				{
					public AttribsMemberAttribsClass_Lic_ProductConnectionAttribs(string keyName, Lic_ProductConnectionAttribs defaultVal) : 
						base(keyName, defaultVal)
					{
						;
					}
					
					public static implicit operator Lic_ProductConnectionAttribs(AttribsMemberAttribsClass_Lic_ProductConnectionAttribs t)
					{
						return (Lic_ProductConnectionAttribs)t.m_tVal;
					}
				
					public Lic_ProductConnectionAttribs TVal
					{
						get {return this;}
						set {m_tVal = (Object)value;}
					}
				}
					
				public class Lic_ProductConnectionAttribsList : AttribsMemberGenericList {public Lic_ProductConnectionAttribsList(string keyName, ArrayList defaultVal) : base(keyName, typeof(Lic_ProductConnectionAttribs), defaultVal){;} }
					
				public Lic_ProductConnectionAttribsList prodConnList = new Lic_ProductConnectionAttribsList("pcLt", new ArrayList());
			
			};
			
		}
	}
}
