from AttribsOutput import *
import re,string,xml.dom.minidom


# outputs an unmanaged c++ and a managed c# class that exposes a function to determine,
# given an SPD object class (eg.: SPathType0) what base kind of object it is {'path','target','module'}
class AttribsOutputClassType(AttribsOutput):
	def __init__(self):
		AttribsOutput.__init__(self)
	
	def ProcessXML(self, class_list):
		
		# preamble
		cs_text = """
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
		"""
		
		cpp_text = """
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
"""
		
		# write the constructor bodies
		i = 0
		for attribs_class in class_list.values():
			class_name = attribs_class['name']
			class_base = attribs_class['base']
			class_description = ''
			try:
				class_description = attribs_class['descriptive_name']
			except:
				pass
			
			if (class_base == 'target'):
				cs_text += """\t\t\t\tmap_class_base["%s"] = new ClassInfoTarget("%s","%s");\n\t\t\t\tif (!map_base_classes.ContainsKey("%s")) map_base_classes["%s"] = new System.Collections.ArrayList();\n\t\t\t\t((System.Collections.ArrayList)map_base_classes["%s"]).Add("%s");\n""" % (class_name,class_name,class_description,class_base,class_base,class_base,class_name)
			elif (class_base == 'path'):
				cs_text += """\t\t\t\tmap_class_base["%s"] = new ClassInfoPath("%s","%s");\n\t\t\t\tif (!map_base_classes.ContainsKey("%s")) map_base_classes["%s"] = new System.Collections.ArrayList();\n\t\t\t\t((System.Collections.ArrayList)map_base_classes["%s"]).Add("%s");\n""" % (class_name,class_name,class_description,class_base,class_base,class_base,class_name)
			elif (class_base == 'module'):
				module_subtype = attribs_class['subtype']
				module_id_string = attribs_class['module_id']
				module_alternate_license_class = attribs_class['alternate_license_class']
				module_id = -1
				if (len(module_id_string)>0):
					module_id = int(module_id_string)
				cs_text += """\t\t\t\tmap_class_base["%s"] = new ClassInfoModule("%s","%s","%s",%d,"%s");\n\t\t\t\tif (!map_base_classes.ContainsKey("%s")) map_base_classes["%s"] = new System.Collections.ArrayList();\n\t\t\t\t((System.Collections.ArrayList)map_base_classes["%s"]).Add("%s");\n""" % (class_name,class_name,class_description,module_subtype,module_id,module_alternate_license_class,class_base,class_base,class_base,class_name)
				
				cs_text += """\t\t\t\tif (!map_module_subtype_classes.ContainsKey("%s")) map_module_subtype_classes["%s"] = new System.Collections.ArrayList();\n\t\t\t\t((System.Collections.ArrayList)map_module_subtype_classes["%s"]).Add("%s");\n""" % (module_subtype,module_subtype,module_subtype,class_name)
			elif (class_base == 'resource'):
				module_id_string = attribs_class['module_id']
				module_id = -1
				if (len(module_id_string)>0):
					module_id = int(module_id_string)
				cs_text += """\t\t\t\tmap_class_base["%s"] = new ClassInfoResource("%s","%s",%d);\n\t\t\t\tif (!map_base_classes.ContainsKey("%s")) map_base_classes["%s"] = new System.Collections.ArrayList();\n\t\t\t\t((System.Collections.ArrayList)map_base_classes["%s"]).Add("%s");\n""" % (class_name,class_name,class_description,module_id,class_base,class_base,class_base,class_name)
			else:
				cs_text += """\t\t\t\tmap_class_base["%s"] = new ClassInfoBase("%s","%s","%s");\n\t\t\t\tif (!map_base_classes.ContainsKey("%s")) map_base_classes["%s"] = new System.Collections.ArrayList();\n\t\t\t\t((System.Collections.ArrayList)map_base_classes["%s"]).Add("%s");\n""" % (class_name,class_name,class_base,class_description,class_base,class_base,class_base,class_name)
				
				
			# process the attributes, add the attribute specific info in to the attribute list
			for attrib in attribs_class['members'].values():
				attrib_name = attrib['name']
				attrib_sec_permit_read = ''
				attrib_sec_permit_write = ''
				
				try:
					attrib_sec_permit_read = attrib['sec_permit_read']
				except:
					pass
				try:
					attrib_sec_permit_write = attrib['sec_permit_write']
				except:
					pass
				
				cs_text += """\t\t\t\t((ClassInfoBase)map_class_base["%s"]).attribs["%s"] = new AttribInfo("%s","%s","%s");\n""" % (class_name,attrib_name,attrib_name,attrib_sec_permit_read,attrib_sec_permit_write)
			
			cpp_text += '\t\t\t\tconstructor_helper_%d();\n' % (i,)
			i += 1
		
		# close out the constructor bodies
		cs_text += """\t\t\t}\n"""
		cpp_text += """\t\t\t}\n"""
		
		
		
		# write the c++ destructor body
		cpp_text += """
			~ClassTypeInfo()
			{
				while(!map_class_base.empty())
				{
					ClassInfoBase* item = map_class_base.begin()->second;
					map_class_base.erase(map_class_base.begin());
					delete item;
				}
			}\n"""
		
		
		# write the c++ constructor helper functions
		i = 0
		for attribs_class in class_list.values():
			class_name = attribs_class['name']
			class_base = attribs_class['base']
			class_description = ''
			try:
				class_description = attribs_class['descriptive_name']
			except:
				pass
			
			cpp_text += '\t\t\tvoid constructor_helper_%d()\n\t\t\t{\n' % (i,)
			
			if (class_base == 'target'):
				cpp_text += """\t\t\t\t\tmap_class_base[L"%s"] = new ClassInfoTarget(L"%s",L"%s");\n\t\t\t\t\tmap_base_classes.insert(std::make_pair(L"%s",L"%s"));\n""" % (class_name,class_name,class_description,class_base,class_name)
			elif (class_base == 'path'):
				cpp_text += """\t\t\t\t\tmap_class_base[L"%s"] = new ClassInfoPath(L"%s",L"%s");\n\t\t\t\tmap_base_classes.insert(std::make_pair(L"%s",L"%s"));\n""" % (class_name,class_name,class_description,class_base,class_name)
			elif (class_base == 'module'):
				module_subtype = attribs_class['subtype']
				module_id_string = attribs_class['module_id']
				module_alternate_license_class = attribs_class['alternate_license_class']
				module_id = -1
				if (len(module_id_string)>0):
					module_id = int(module_id_string)
				cpp_text += """\t\t\t\t\tmap_class_base[L"%s"] = new ClassInfoModule(L"%s",L"%s",L"%s",%d,L"%s");\n\t\t\t\t\tmap_base_classes.insert(std::make_pair(L"%s",L"%s"));\n""" % (class_name,class_name,class_description,module_subtype,module_id,module_alternate_license_class,class_base,class_name)
				
				cpp_text += """\t\t\t\t\tmap_module_subtype_classes.insert(std::make_pair(L"%s",L"%s"));\n""" % (module_subtype,class_name)
			elif (class_base == 'resource'):
				module_id_string = attribs_class['module_id']
				module_id = -1
				if (len(module_id_string)>0):
					module_id = int(module_id_string)
				cpp_text += """\t\t\t\t\tmap_class_base[L"%s"] = new ClassInfoResource(L"%s",L"%s",%d);\n\t\t\t\t\tmap_base_classes.insert(std::make_pair(L"%s",L"%s"));\n""" % (class_name,class_name,class_description,module_id,class_base,class_name)
			else:
				cpp_text += """\t\t\t\t\tmap_class_base[L"%s"] = new ClassInfoBase(L"%s",L"%s",L"%s");\n\t\t\t\t\tmap_base_classes.insert(std::make_pair(L"%s",L"%s"));\n""" % (class_name,class_name,class_base,class_description,class_base,class_name)
				
				
			# process the attributes, add the attribute specific info in to the attribute list
			for attrib in attribs_class['members'].values():
				attrib_name = attrib['name']
				attrib_sec_permit_read = ''
				attrib_sec_permit_write = ''
				
				try:
					attrib_sec_permit_read = attrib['sec_permit_read']
				except:
					pass
				try:
					attrib_sec_permit_write = attrib['sec_permit_write']
				except:
					pass
				
				cpp_text += """\t\t\t\t\tmap_class_base[L"%s"]->attribs[L"%s"] = AttribInfo(L"%s",L"%s",L"%s");\n""" % (class_name,attrib_name,attrib_name,attrib_sec_permit_read,attrib_sec_permit_write)
				
			cpp_text += '\t\t\t}\n'
			
			i += 1
		
		# declare the utility functions
		
		cs_text += """			
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
"""
		cpp_text += """			
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

"""
		
		self.WriteFileIfDifferent('LicAttribsCS\\ClassTypeInfo.cs', cs_text)
		self.WriteFileIfDifferent('LicAttribsCPP\\ClassTypeInfo.h', cpp_text)
		
