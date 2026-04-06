using System;
using System.Collections;
using System.Collections.Specialized;

namespace Solimar
{
	namespace Licensing
	{
		namespace Attribs
		{
			public class Lic_TestMailServer : LicensingAttribsBase
			{
				public static string ClassName = "L_TMS";
				
				public Lic_TestMailServer() : 
					base(ClassName)
				{
					;
				}
				
				
				public AttribsMemberString subject = new AttribsMemberString("mS", "Test E-mail");
				public AttribsMemberString body = new AttribsMemberString("mB", "This is an e-mail sent by Solimar Systems, Inc. testing Mail Server settings.");
				public AttribsMemberStringList recipentsList = new AttribsMemberStringList("rL", new ArrayList());
			
			};
			
		}
	}
}
