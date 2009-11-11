
using System;

namespace Solimar
{
	namespace Licensing
	{
		public class ClassTypeInfo
		{
			public class AttribInfo
			{
				public AttribInfo(string name, string sec_permit_read, string sec_permit_write) {this.name = name; this.sec_permit_read = sec_permit_read; this.sec_permit_write = sec_permit_write;}
				public string name = null;
				public string sec_permit_read = null;
				public string sec_permit_write = null;
			}
			
			public class ClassInfoBase
			{
				public ClassInfoBase() {;}
				public ClassInfoBase(string name, string base_class, string descriptive_name) {this.name = name; this.base_class = base_class; this.descriptive_name = descriptive_name;}
				public string name = null;
				public string base_class = null;
				public string descriptive_name = null;
				public System.Collections.SortedList attribs = new System.Collections.SortedList();
			}
			
			public class ClassInfoTarget : ClassInfoBase
			{
				public ClassInfoTarget(string name, string descriptive_name) : base(name,"target",descriptive_name) {;}
			}
			
			public class ClassInfoPath : ClassInfoBase
			{
				public ClassInfoPath(string name, string descriptive_name) : base(name,"path",descriptive_name) {;}
			}
			
			public class ClassInfoModule : ClassInfoBase
			{
				public ClassInfoModule(string name, string descriptive_name, string subtype, int module_id, string alternate_license_class) : base(name, "module", descriptive_name) {this.subtype = subtype; this.module_id = module_id; this.alternate_license_class = alternate_license_class;}
				string alternate_license_class;
				public string subtype;
				public int module_id;
			}
			
			public class ClassInfoResource : ClassInfoBase
			{
				public ClassInfoResource(string name, string descriptive_name, int module_id) : base(name, "resource", descriptive_name) {this.module_id = module_id;}
				public int module_id;
			}
			
			public ClassTypeInfo()
			{
				map_class_base = new System.Collections.SortedList();
				map_base_classes = new System.Collections.SortedList();
				map_module_subtype_classes = new System.Collections.SortedList();
						map_class_base["L_KyA"] = new ClassInfoBase("L_KyA","","License Key Attributes");
				if (!map_base_classes.ContainsKey("")) map_base_classes[""] = new System.Collections.ArrayList();
				((System.Collections.ArrayList)map_base_classes[""]).Add("L_KyA");
				((ClassInfoBase)map_class_base["L_KyA"]).attribs["currentActivations"] = new AttribInfo("currentActivations","","");
				((ClassInfoBase)map_class_base["L_KyA"]).attribs["keyName"] = new AttribInfo("keyName","","");
				((ClassInfoBase)map_class_base["L_KyA"]).attribs["layout"] = new AttribInfo("layout","","");
				((ClassInfoBase)map_class_base["L_KyA"]).attribs["modifiedDate"] = new AttribInfo("modifiedDate","","");
				((ClassInfoBase)map_class_base["L_KyA"]).attribs["verificationCode"] = new AttribInfo("verificationCode","","");
				map_class_base["L_PkgA"] = new ClassInfoBase("L_PkgA","","License Validation Attributes");
				if (!map_base_classes.ContainsKey("")) map_base_classes[""] = new System.Collections.ArrayList();
				((System.Collections.ArrayList)map_base_classes[""]).Add("L_PkgA");
				((ClassInfoBase)map_class_base["L_PkgA"]).attribs["licLicenseInfoAttribs"] = new AttribInfo("licLicenseInfoAttribs","","");
				((ClassInfoBase)map_class_base["L_PkgA"]).attribs["licSoftwareSpecAttribs"] = new AttribInfo("licSoftwareSpecAttribs","","");
				map_class_base["L_SysInA"] = new ClassInfoBase("L_SysInA","","System Info Attributes");
				if (!map_base_classes.ContainsKey("")) map_base_classes[""] = new System.Collections.ArrayList();
				((System.Collections.ArrayList)map_base_classes[""]).Add("L_SysInA");
				((ClassInfoBase)map_class_base["L_SysInA"]).attribs["computerNameList"] = new AttribInfo("computerNameList","","");
				((ClassInfoBase)map_class_base["L_SysInA"]).attribs["systemUuidList"] = new AttribInfo("systemUuidList","","");
				((ClassInfoBase)map_class_base["L_SysInA"]).attribs["systemModelList"] = new AttribInfo("systemModelList","","");
				((ClassInfoBase)map_class_base["L_SysInA"]).attribs["systemManufacturerList"] = new AttribInfo("systemManufacturerList","","");
				((ClassInfoBase)map_class_base["L_SysInA"]).attribs["licenseServerVersion"] = new AttribInfo("licenseServerVersion","","");
				((ClassInfoBase)map_class_base["L_SysInA"]).attribs["macAddressList"] = new AttribInfo("macAddressList","","");
				((ClassInfoBase)map_class_base["L_SysInA"]).attribs["operatingSystemList"] = new AttribInfo("operatingSystemList","","");
				((ClassInfoBase)map_class_base["L_SysInA"]).attribs["systemTypeList"] = new AttribInfo("systemTypeList","","");
				((ClassInfoBase)map_class_base["L_SysInA"]).attribs["domainNameList"] = new AttribInfo("domainNameList","","");
				((ClassInfoBase)map_class_base["L_SysInA"]).attribs["biosSerialNumberList"] = new AttribInfo("biosSerialNumberList","","");
				((ClassInfoBase)map_class_base["L_SysInA"]).attribs["bPartOFDomain"] = new AttribInfo("bPartOFDomain","","");
				map_class_base["L_LcSysA"] = new ClassInfoBase("L_LcSysA","","License System Attributes");
				if (!map_base_classes.ContainsKey("")) map_base_classes[""] = new System.Collections.ArrayList();
				((System.Collections.ArrayList)map_base_classes[""]).Add("L_LcSysA");
				((ClassInfoBase)map_class_base["L_LcSysA"]).attribs["ListOfStreamed_InfoAttribs"] = new AttribInfo("ListOfStreamed_InfoAttribs","","");
				((ClassInfoBase)map_class_base["L_LcSysA"]).attribs["Streamed_SystemInfoAttribs"] = new AttribInfo("Streamed_SystemInfoAttribs","","");
				((ClassInfoBase)map_class_base["L_LcSysA"]).attribs["Streamed_SoftwareSpecAttribs"] = new AttribInfo("Streamed_SoftwareSpecAttribs","","");
				((ClassInfoBase)map_class_base["L_LcSysA"]).attribs["Streamed_ServerDataAttribs"] = new AttribInfo("Streamed_ServerDataAttribs","","");
				((ClassInfoBase)map_class_base["L_LcSysA"]).attribs["createdDate"] = new AttribInfo("createdDate","","");
				((ClassInfoBase)map_class_base["L_LcSysA"]).attribs["ListOfStreamed_KeyAttribs"] = new AttribInfo("ListOfStreamed_KeyAttribs","","");
				map_class_base["L_SvrDtA"] = new ClassInfoBase("L_SvrDtA","","License Server Data Attributes");
				if (!map_base_classes.ContainsKey("")) map_base_classes[""] = new System.Collections.ArrayList();
				((System.Collections.ArrayList)map_base_classes[""]).Add("L_SvrDtA");
				((ClassInfoBase)map_class_base["L_SvrDtA"]).attribs["versionSubMinor"] = new AttribInfo("versionSubMinor","","");
				((ClassInfoBase)map_class_base["L_SvrDtA"]).attribs["versionSubMajor"] = new AttribInfo("versionSubMajor","","");
				((ClassInfoBase)map_class_base["L_SvrDtA"]).attribs["bInClockViol"] = new AttribInfo("bInClockViol","","");
				((ClassInfoBase)map_class_base["L_SvrDtA"]).attribs["fileInfoList"] = new AttribInfo("fileInfoList","","");
				((ClassInfoBase)map_class_base["L_SvrDtA"]).attribs["clockViolCount"] = new AttribInfo("clockViolCount","","");
				((ClassInfoBase)map_class_base["L_SvrDtA"]).attribs["clockViolLastDate"] = new AttribInfo("clockViolLastDate","","");
				((ClassInfoBase)map_class_base["L_SvrDtA"]).attribs["versionMajor"] = new AttribInfo("versionMajor","","");
				((ClassInfoBase)map_class_base["L_SvrDtA"]).attribs["versionMinor"] = new AttribInfo("versionMinor","","");
				((ClassInfoBase)map_class_base["L_SvrDtA"]).attribs["clockViolHistoryList"] = new AttribInfo("clockViolHistoryList","","");
				((ClassInfoBase)map_class_base["L_SvrDtA"]).attribs["lastTouchDate"] = new AttribInfo("lastTouchDate","","");
			}
			
			public System.Collections.ArrayList GetModuleSubtypeList(string module_subtype)
			{
				if (!map_module_subtype_classes.ContainsKey(module_subtype))
					return new System.Collections.ArrayList();
				return (System.Collections.ArrayList)map_module_subtype_classes[module_subtype];
			}
			
			public bool ClassExists(string class_name)
			{
				return (map_class_base.ContainsKey(class_name));
			}
			
			public ClassInfoBase GetClassInfo(string class_name)
			{
				return (ClassInfoBase)map_class_base[class_name];
			}
						
			public string GetBaseClass(string class_name)
			{
				if (!map_class_base.ContainsKey(class_name))
					return "";
				return ((ClassInfoBase)map_class_base[class_name]).base_class;
			}
			
			public System.Collections.ArrayList GetClasses(string base_class_name)
			{
				if (!map_base_classes.ContainsKey(base_class_name))
					return new System.Collections.ArrayList();
				return (System.Collections.ArrayList)map_base_classes[base_class_name];
			}
			
			private System.Collections.SortedList map_class_base;
			private System.Collections.SortedList map_base_classes;
			private System.Collections.SortedList map_module_subtype_classes;
		}
	}
}
