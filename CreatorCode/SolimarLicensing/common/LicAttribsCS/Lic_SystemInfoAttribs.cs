using System;
using System.Collections;
using System.Collections.Specialized;

namespace Solimar
{
	namespace Licensing
	{
		namespace Attribs
		{
			public class Lic_SystemInfoAttribs : LicensingAttribsBase
			{
				public static string ClassName = "L_SysInA";
				
				public Lic_SystemInfoAttribs() : 
					base(ClassName)
				{
					;
				}
				
				
				public AttribsMemberString licenseServerVersion = new AttribsMemberString("lV", "");
				public AttribsMemberStringList computerNameList = new AttribsMemberStringList("cLt", new ArrayList());
				public AttribsMemberStringList macAddressList = new AttribsMemberStringList("mLt", new ArrayList());
				public AttribsMemberStringList biosSerialNumberList = new AttribsMemberStringList("bLt", new ArrayList());
			
			};
			
		}
	}
}
