using System;
using System.Collections;
using System.Collections.Specialized;

namespace Solimar
{
	namespace Licensing
	{
		namespace Attribs
		{
			public class Sys_EventLogInfoAttribs : LicensingAttribsBase
			{
				public static string ClassName = "S_ElInA";
				
				public Sys_EventLogInfoAttribs() : 
					base(ClassName)
				{
					;
				}
				
				public class Sys_EventLogEntriesInfoAttribs : LicensingAttribsBase
				{
					public static string ClassName = "S_EEA";
					
					public Sys_EventLogEntriesInfoAttribs() : 
						base(ClassName)
					{
						;
					}
					
					
					public AttribsMemberString category = new AttribsMemberString("c", "");
					public AttribsMemberDWORD categoryNumber = new AttribsMemberDWORD("cN", 0);
					public AttribsMemberBuffer data = new AttribsMemberBuffer("d", new byte[]{ });
					public AttribsMemberDWORD entryType = new AttribsMemberDWORD("eT", 0);
					public AttribsMemberDWORD eventID = new AttribsMemberDWORD("eI", 0);
					public AttribsMemberDWORD index = new AttribsMemberDWORD("i", 0);
					public AttribsMemberDWORD instanceId = new AttribsMemberDWORD("iI", 0);
					public AttribsMemberString machineName = new AttribsMemberString("mN", "");
					public AttribsMemberString message = new AttribsMemberString("mG", "");
					public AttribsMemberStringList replacementStrings = new AttribsMemberStringList("rLt", new ArrayList());
					public AttribsMemberString source = new AttribsMemberString("sR", "");
					public AttribsMemberDateTime timeGenerated = new AttribsMemberDateTime("tG", AttribFormat.ConvertStringToDateTime("1900-01-01 00:00:00.0000"));
					public AttribsMemberDateTime timeWritten = new AttribsMemberDateTime("tW", AttribFormat.ConvertStringToDateTime("1900-01-01 00:00:00.0000"));
					public AttribsMemberString userName = new AttribsMemberString("uN", "");
				
				};
				
				public class AttribsMemberAttribsClass_Sys_EventLogEntriesInfoAttribs : AttribsMemberAttribsClass
				{
					public AttribsMemberAttribsClass_Sys_EventLogEntriesInfoAttribs(string keyName, Sys_EventLogEntriesInfoAttribs defaultVal) : 
						base(keyName, defaultVal)
					{
						;
					}
					
					public static implicit operator Sys_EventLogEntriesInfoAttribs(AttribsMemberAttribsClass_Sys_EventLogEntriesInfoAttribs t)
					{
						return (Sys_EventLogEntriesInfoAttribs)t.m_tVal;
					}
				
					public Sys_EventLogEntriesInfoAttribs TVal
					{
						get {return this;}
						set {m_tVal = (Object)value;}
					}
				}
					
				public class Sys_EventLogEntriesInfoAttribsList : AttribsMemberGenericList {public Sys_EventLogEntriesInfoAttribsList(string keyName, ArrayList defaultVal) : base(keyName, typeof(Sys_EventLogEntriesInfoAttribs), defaultVal){;} }
					
				public Sys_EventLogEntriesInfoAttribsList entryList = new Sys_EventLogEntriesInfoAttribsList("eLt", new ArrayList());
				public AttribsMemberString log = new AttribsMemberString("l", "");
				public AttribsMemberString logName = new AttribsMemberString("lN", "");
				public AttribsMemberString machineName = new AttribsMemberString("mN", "");
				public AttribsMemberDWORD maximumKilobytes = new AttribsMemberDWORD("mK", 0);
				public AttribsMemberDWORD minimumRetentionDays = new AttribsMemberDWORD("mR", 0);
				public AttribsMemberDWORD enumOverflowAction = new AttribsMemberDWORD("oA", 0);
				public AttribsMemberString source = new AttribsMemberString("s", "");
			
			};
			
		}
	}
}
