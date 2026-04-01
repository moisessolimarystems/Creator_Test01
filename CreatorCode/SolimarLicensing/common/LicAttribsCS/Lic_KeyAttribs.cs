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
				
				
				public AttribsMemberBufferList layout = new AttribsMemberBufferList("lo", new ArrayList());
				public AttribsMemberString keyName = new AttribsMemberString("kN", "");
				public AttribsMemberString verificationCode = new AttribsMemberString("vC", "");
				public AttribsMemberDateTime modifiedDate = new AttribsMemberDateTime("mDt", AttribFormat.ConvertStringToDateTime("1900-01-01 00:00:00.0000"));
				public AttribsMemberDWORD currentActivations = new AttribsMemberDWORD("cA", 0);
			
			};
			
		}
	}
}
