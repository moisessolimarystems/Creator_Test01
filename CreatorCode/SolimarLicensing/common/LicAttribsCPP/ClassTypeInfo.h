
#ifndef __ClassTypeInfo_h__
#define __ClassTypeInfo_h__


#include <vector>
#include <map>
#include <utility>
#include <string>

namespace Solimar
{
	namespace Licensing
	{
		class ClassTypeInfo
		{
		public:
			typedef std::vector<std::wstring> StringList;
			
			class AttribInfo
			{
			public:
				AttribInfo() {;}
				AttribInfo(std::wstring name, std::wstring sec_permit_read, std::wstring sec_permit_write) : name(name), sec_permit_read(sec_permit_read), sec_permit_write(sec_permit_write) {;}
				std::wstring name;
				std::wstring sec_permit_read;
				std::wstring sec_permit_write;
			};
			typedef std::map<std::wstring,AttribInfo> AttribInfoMap;
			
			class ClassInfoBase
			{
			public:
				ClassInfoBase() {;}
				ClassInfoBase(std::wstring name, std::wstring base_class, std::wstring descriptive_name) : name(name), base_class(base_class), descriptive_name(descriptive_name) {;}
				std::wstring name;
				std::wstring base_class;
				std::wstring descriptive_name;
				AttribInfoMap attribs;
			};
			
			class ClassInfoTarget : public ClassInfoBase
			{
			public:				
				ClassInfoTarget(std::wstring name, std::wstring descriptive_name) : ClassInfoBase(name,L"target",descriptive_name) {;}
			};
			
			class ClassInfoPath : public ClassInfoBase
			{
			public:
				ClassInfoPath(std::wstring name, std::wstring descriptive_name) : ClassInfoBase(name,L"path",descriptive_name) {;}
			};
			
			class ClassInfoModule : public ClassInfoBase
			{
			public:
				ClassInfoModule(std::wstring name, std::wstring descriptive_name, std::wstring subtype, int module_id, std::wstring alternate_license_class) : ClassInfoBase(name, L"module", descriptive_name), subtype(subtype), module_id(module_id), alternate_license_class(alternate_license_class) {;}
				std::wstring alternate_license_class;
				std::wstring subtype;
				int module_id;
			};
			
			class ClassInfoResource : public ClassInfoBase
			{
			public:
				ClassInfoResource(std::wstring name, std::wstring descriptive_name, int module_id) : ClassInfoBase(name, L"resource", descriptive_name), module_id(module_id) {;}
				int module_id;
			};
			
			ClassTypeInfo()
			{
				constructor_helper_0();
				constructor_helper_1();
				constructor_helper_2();
				constructor_helper_3();
				constructor_helper_4();
				constructor_helper_5();
			}

			~ClassTypeInfo()
			{
				while(!map_class_base.empty())
				{
					ClassInfoBase* item = map_class_base.begin()->second;
					map_class_base.erase(map_class_base.begin());
					delete item;
				}
			}
			void constructor_helper_0()
			{
					map_class_base[L"L_KyA"] = new ClassInfoBase(L"L_KyA",L"",L"License Key Attributes");
					map_base_classes.insert(std::make_pair(L"",L"L_KyA"));
					map_class_base[L"L_KyA"]->attribs[L"keyVersion"] = AttribInfo(L"keyVersion",L"",L"");
					map_class_base[L"L_KyA"]->attribs[L"layout"] = AttribInfo(L"layout",L"",L"");
					map_class_base[L"L_KyA"]->attribs[L"historyNumber"] = AttribInfo(L"historyNumber",L"",L"");
					map_class_base[L"L_KyA"]->attribs[L"keyName"] = AttribInfo(L"keyName",L"",L"");
					map_class_base[L"L_KyA"]->attribs[L"licenseCode"] = AttribInfo(L"licenseCode",L"",L"");
					map_class_base[L"L_KyA"]->attribs[L"currentDate"] = AttribInfo(L"currentDate",L"",L"");
					map_class_base[L"L_KyA"]->attribs[L"packetCreationDate"] = AttribInfo(L"packetCreationDate",L"",L"");
					map_class_base[L"L_KyA"]->attribs[L"activationInfoList"] = AttribInfo(L"activationInfoList",L"",L"");
			}
			void constructor_helper_1()
			{
					map_class_base[L"L_LcSysA"] = new ClassInfoBase(L"L_LcSysA",L"",L"License System Attributes");
					map_base_classes.insert(std::make_pair(L"",L"L_LcSysA"));
					map_class_base[L"L_LcSysA"]->attribs[L"ListOfStreamed_InfoAttribs"] = AttribInfo(L"ListOfStreamed_InfoAttribs",L"",L"");
					map_class_base[L"L_LcSysA"]->attribs[L"Streamed_SystemInfoAttribs"] = AttribInfo(L"Streamed_SystemInfoAttribs",L"",L"");
					map_class_base[L"L_LcSysA"]->attribs[L"Streamed_SoftwareSpecAttribs"] = AttribInfo(L"Streamed_SoftwareSpecAttribs",L"",L"");
					map_class_base[L"L_LcSysA"]->attribs[L"Streamed_ServerDataAttribs"] = AttribInfo(L"Streamed_ServerDataAttribs",L"",L"");
					map_class_base[L"L_LcSysA"]->attribs[L"Streamed_UsageInfoAttribs"] = AttribInfo(L"Streamed_UsageInfoAttribs",L"",L"");
					map_class_base[L"L_LcSysA"]->attribs[L"createdDate"] = AttribInfo(L"createdDate",L"",L"");
					map_class_base[L"L_LcSysA"]->attribs[L"ListOfStreamed_KeyAttribs"] = AttribInfo(L"ListOfStreamed_KeyAttribs",L"",L"");
			}
			void constructor_helper_2()
			{
					map_class_base[L"L_UsInA"] = new ClassInfoBase(L"L_UsInA",L"",L"License Usage Info Attributes");
					map_base_classes.insert(std::make_pair(L"",L"L_UsInA"));
					map_class_base[L"L_UsInA"]->attribs[L"productList"] = AttribInfo(L"productList",L"",L"");
			}
			void constructor_helper_3()
			{
					map_class_base[L"L_PkgA"] = new ClassInfoBase(L"L_PkgA",L"",L"License Validation Attributes");
					map_base_classes.insert(std::make_pair(L"",L"L_PkgA"));
					map_class_base[L"L_PkgA"]->attribs[L"licLicenseInfoAttribs"] = AttribInfo(L"licLicenseInfoAttribs",L"",L"");
					map_class_base[L"L_PkgA"]->attribs[L"licSoftwareSpecAttribs"] = AttribInfo(L"licSoftwareSpecAttribs",L"",L"");
			}
			void constructor_helper_4()
			{
					map_class_base[L"L_SysInA"] = new ClassInfoBase(L"L_SysInA",L"",L"System Info Attributes");
					map_base_classes.insert(std::make_pair(L"",L"L_SysInA"));
					map_class_base[L"L_SysInA"]->attribs[L"computerNameList"] = AttribInfo(L"computerNameList",L"",L"");
					map_class_base[L"L_SysInA"]->attribs[L"systemUuidList"] = AttribInfo(L"systemUuidList",L"",L"");
					map_class_base[L"L_SysInA"]->attribs[L"systemModelList"] = AttribInfo(L"systemModelList",L"",L"");
					map_class_base[L"L_SysInA"]->attribs[L"systemManufacturerList"] = AttribInfo(L"systemManufacturerList",L"",L"");
					map_class_base[L"L_SysInA"]->attribs[L"licenseServerVersion"] = AttribInfo(L"licenseServerVersion",L"",L"");
					map_class_base[L"L_SysInA"]->attribs[L"macAddressList"] = AttribInfo(L"macAddressList",L"",L"");
					map_class_base[L"L_SysInA"]->attribs[L"operatingSystemList"] = AttribInfo(L"operatingSystemList",L"",L"");
					map_class_base[L"L_SysInA"]->attribs[L"systemTypeList"] = AttribInfo(L"systemTypeList",L"",L"");
					map_class_base[L"L_SysInA"]->attribs[L"domainNameList"] = AttribInfo(L"domainNameList",L"",L"");
					map_class_base[L"L_SysInA"]->attribs[L"biosSerialNumberList"] = AttribInfo(L"biosSerialNumberList",L"",L"");
					map_class_base[L"L_SysInA"]->attribs[L"bPartOFDomain"] = AttribInfo(L"bPartOFDomain",L"",L"");
			}
			void constructor_helper_5()
			{
					map_class_base[L"L_SvrDtA"] = new ClassInfoBase(L"L_SvrDtA",L"",L"License Server Data Attributes");
					map_base_classes.insert(std::make_pair(L"",L"L_SvrDtA"));
					map_class_base[L"L_SvrDtA"]->attribs[L"versionSubMinor"] = AttribInfo(L"versionSubMinor",L"",L"");
					map_class_base[L"L_SvrDtA"]->attribs[L"versionSubMajor"] = AttribInfo(L"versionSubMajor",L"",L"");
					map_class_base[L"L_SvrDtA"]->attribs[L"bInClockViol"] = AttribInfo(L"bInClockViol",L"",L"");
					map_class_base[L"L_SvrDtA"]->attribs[L"fileInfoList"] = AttribInfo(L"fileInfoList",L"",L"");
					map_class_base[L"L_SvrDtA"]->attribs[L"clockViolCount"] = AttribInfo(L"clockViolCount",L"",L"");
					map_class_base[L"L_SvrDtA"]->attribs[L"clockViolLastDate"] = AttribInfo(L"clockViolLastDate",L"",L"");
					map_class_base[L"L_SvrDtA"]->attribs[L"versionMajor"] = AttribInfo(L"versionMajor",L"",L"");
					map_class_base[L"L_SvrDtA"]->attribs[L"versionMinor"] = AttribInfo(L"versionMinor",L"",L"");
					map_class_base[L"L_SvrDtA"]->attribs[L"clockViolHistoryList"] = AttribInfo(L"clockViolHistoryList",L"",L"");
					map_class_base[L"L_SvrDtA"]->attribs[L"lastTouchDate"] = AttribInfo(L"lastTouchDate",L"",L"");
			}
			
			StringList GetModuleSubtypeList(std::wstring &module_subtype)
			{
				StringList ret;
				BaseTypeMultiMap::iterator a = map_module_subtype_classes.lower_bound(module_subtype), b = map_base_classes.upper_bound(module_subtype);
				for (;a!=map_module_subtype_classes.end() && a!=b;++a)
				{
					ret.push_back(a->second);
				}
				return ret;
			}
			
			bool ClassExists(std::wstring &class_name)
			{
				return (map_class_base.find(class_name)!=map_class_base.end());
			}
			
			ClassInfoBase* GetClassInfo(std::wstring &class_name)
			{
				return map_class_base[class_name];
			}
			std::wstring GetBaseClass(std::wstring &class_name)
			{
				ClassTypeMap::iterator i = map_class_base.find(class_name);
				if (i==map_class_base.end())
					return L"";
				return i->second->base_class;
			}

			StringList GetClasses(std::wstring &base_class_name)
			{
				StringList ret;
				BaseTypeMultiMap::iterator a = map_base_classes.lower_bound(base_class_name), b = map_base_classes.upper_bound(base_class_name);
				for (;a!=map_base_classes.end() && a!=b;++a)
				{
					ret.push_back(a->second);
				}
				return ret;
			}
			
		private:
			typedef std::map<std::wstring,ClassInfoBase*> ClassTypeMap;
			typedef std::multimap<std::wstring,std::wstring> BaseTypeMultiMap;
			ClassTypeMap map_class_base;
			BaseTypeMultiMap map_base_classes;
			BaseTypeMultiMap map_module_subtype_classes;
		};
	};
};

#endif // __ClassTypeInfo_h__

