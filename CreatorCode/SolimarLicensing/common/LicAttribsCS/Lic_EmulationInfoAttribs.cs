using System;
using System.Collections;
using System.Collections.Specialized;

namespace Solimar
{
	namespace Licensing
	{
		namespace Attribs
		{
			public class Lic_EmulationInfoAttribs : LicensingAttribsBase
			{
				public static string ClassName = "L_EmInA";
				
				public Lic_EmulationInfoAttribs() : 
					base(ClassName)
				{
					;
				}
				
				
				public AttribsMemberBOOL bDongleEmulatorDetected = new AttribsMemberBOOL("dED", false);
				public AttribsMemberBOOL bBypassDongleEmulatorCheck = new AttribsMemberBOOL("bdEC", false);
				public AttribsMemberStringList knownEmulatorServicesList = new AttribsMemberStringList("kEL", new ArrayList());
				public AttribsMemberString excludeService = new AttribsMemberString("exS", "");
				public AttribsMemberBOOL bCheckEmulatorCall_FoundEmulator = new AttribsMemberBOOL("ceFE", false);
				public AttribsMemberDWORD checkEmulatorCall_StatusCode = new AttribsMemberDWORD("ceSC", 0);
			
			};
			
		}
	}
}
