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
				public AttribsMemberStringList domainNameList = new AttribsMemberStringList("dLt", new ArrayList());
				public AttribsMemberStringList operatingSystemList = new AttribsMemberStringList("osLt", new ArrayList());
				public AttribsMemberBOOL bPartOFDomain = new AttribsMemberBOOL("pD", true);
				public AttribsMemberStringList systemManufacturerList = new AttribsMemberStringList("sMFLt", new ArrayList());
				public AttribsMemberStringList systemModelList = new AttribsMemberStringList("sMLt", new ArrayList());
				public AttribsMemberStringList systemTypeList = new AttribsMemberStringList("sTLt", new ArrayList());
				public AttribsMemberStringList systemUuidList = new AttribsMemberStringList("sULt", new ArrayList());
			
			};
			
		}
	}
}
