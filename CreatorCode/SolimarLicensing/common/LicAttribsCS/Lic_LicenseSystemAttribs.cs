using System;
using System.Collections;
using System.Collections.Specialized;

namespace Solimar
{
	namespace Licensing
	{
		namespace Attribs
		{
			public class Lic_LicenseSystemAttribs : LicensingAttribsBase
			{
				public static string ClassName = "L_LcSysA";
				
				public Lic_LicenseSystemAttribs() : 
					base(ClassName)
				{
					;
				}
				
				
				public AttribsMemberStringList ListOfStreamed_KeyAttribs = new AttribsMemberStringList("kLt", new ArrayList());
				public AttribsMemberString Streamed_ServerDataAttribs = new AttribsMemberString("sD", "");
				public AttribsMemberString Streamed_SystemInfoAttribs = new AttribsMemberString("sI", "");
				public AttribsMemberString Streamed_SoftwareSpecAttribs = new AttribsMemberString("sS", "");
				public AttribsMemberStringList ListOfStreamed_InfoAttribs = new AttribsMemberStringList("iLt", new ArrayList());
				public AttribsMemberDateTime createdDate = new AttribsMemberDateTime("cDt", AttribFormat.ConvertStringToDateTime("1900-01-01 00:00:00.0000"));
			
			};
			
		}
	}
}
