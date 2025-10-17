from AttribsOutput import *
import re,string,xml.dom.minidom


def CreateComments(comments):
	if (len(comments)>0):
		comments = string.replace(comments,'*/','  ')
		comments = string.strip(comments)
		return "/*\n%s\n*/\n" % (comments,)
	return ''

def EscapeCStringLiteral(s):
	r=s
	r=string.replace(r,'\\','\\\\')
	r=string.replace(r,'\n','\\n')
	r=string.replace(r,'\t','\\t')
	r=string.replace(r,'"','\\"')
	r=string.replace(r,'\'','\\\'')
	return r
	
def EncodeBufferLiteral(s):
	r = '{ '
	for i in range(0,len(s)/2):
		r+= '0x' + s[i*2:(i+1)*2] + ', '
	if (len(r)>2):
		r = r[:-2]
	r += '}'
	return (r,len(s)/2)

"""
classes = {}			# maps name to: class
class = {}			# {name, descriptive_name, base, filename, code_class_name, comments, typedefs, members, derived_base, subtype, module_id}
typedefs = {}			# maps name to: typedef
typedef = {}			# {name, type, enum, inner_class, derived_class}
enum = []			# list of: {name, value}
inner_class == class
derived_class == class		# makes a copy of another class, sets the name and derived_base, etc. and changes the defaults in members
members = {}			# maps member name to: {name, type, tagname, default, sec_permit_read, sec_permit_write, }
"""


# outputs the c++ version of the derived attribs class header files
class AttribsOutputCS(AttribsOutput):
	def __init__(self):
		AttribsOutput.__init__(self)
		self.type_map = {'string': 'AttribsMemberString',  'unsigned __int64': 'AttribsMemberQWORD',  'unsigned __int32': 'AttribsMemberDWORD', 'unsigned __int16': 'AttribsMemberWORD', 'unsigned __int8': 'AttribsMemberBYTE', '__int64': 'AttribsMemberQINT',  '__int32': 'AttribsMemberINT', '__int16': 'AttribsMemberSINT', '__int8': 'AttribsMemberCHAR', 'bool': 'AttribsMemberBOOL', 'time_t': 'AttribsMemberDWORD', 'size_t': 'AttribsMemberDWORD', 'float': 'AttribsMemberFloat', 'double': 'AttribsMemberDouble', 'list(unsigned __int32)': 'AttribsMemberDWORDList', 'list(string)': 'AttribsMemberStringList', 'map(unsigned __int32, unsigned __int32)': 'AttribsMemberDWORDtoDWORDMap', 'map(unsigned __int32, string)': 'AttribsMemberDWORDtoStringMap', 'map(string, unsigned __int32)': 'AttribsMemberStringtoDWORDMap', 'map(string, string)': 'AttribsMemberStringtoStringMap', 'byte[]': 'AttribsMemberBuffer', 'list(byte[])': 'AttribsMemberBufferList', 'DateTime': 'AttribsMemberDateTime'}
		self.type_map_underlying = {'string': 'string',  'unsigned __int64': 'UInt64',  'unsigned __int32': 'UInt32', 'unsigned __int16': 'UInt16', 'unsigned __int8': 'Byte', '__int64': 'Int64',  '__int32': 'Int32', '__int16': 'Int16', '__int8': 'SByte', 'bool': 'bool', 'time_t': 'UInt32', 'size_t': 'UInt32', 'float': 'float', 'double': 'double', 'list(unsigned __int32)': 'ArrayList', 'list(string)': 'ArrayList', 'map(unsigned __int32, unsigned __int32)': 'SortedList', 'map(unsigned __int32, string)': 'SortedList', 'map(string, unsigned __int32)': 'SortedList', 'map(string, string)': 'SortedList', 'byte[]': 'byte[]', 'list(byte[])': 'ArrayList', 'DateTime': 'DateTime'}	# the list() and map() type translations may be erronious
	
	def ProcessXML(self, class_list):
		# for each attribs class
		for attribs_class in class_list.values():
			self.GenerateGenericAttribsFile(attribs_class)
	
	def GenerateAttribsClass(self, attribs_class, inner_class, derived_class):
		# class declaration header
		
		class_text = CreateComments(attribs_class['comments'])
		
		if (not derived_class):
			class_text += """public class %s : LicensingAttribsBase\n{\n\tpublic static string ClassName = "%s";\n\t\n""" % (attribs_class['code_class_name'], attribs_class['name'])
			class_text += """\tpublic %s() : \n\t\tbase(ClassName)\n\t{\n\t\t;\n\t}\n\t\n""" % (attribs_class['code_class_name'],)
		else:
			class_text += """public class %s : %s\n{\n\tpublic new static string ClassName = "%s";\n\t\n""" % (attribs_class['code_class_name'], attribs_class['derived_base_code_class'], attribs_class['name'])
			class_text += """\tpublic %s() : \n\t\tbase()\n\t{\n""" % (attribs_class['code_class_name'],)
			
			# set the derived class defaults (defaults are overriden here)
			class_text += "\t\t/* default overrides */\n"
			class_text += "\t\tm_ClassName = ClassName;\n"
			# set the default values in the constructor
			for member in OutputOrderDictionaryValues(attribs_class['members']):
				member_info = self.ComputeMemberInfo(attribs_class, member)
				class_text += '\t\t%s = new %s("%s", %s);\n' % (member_info['name'],member_info['type'],member_info['tagname'],member_info['default'])
			class_text += """\n\t}\n\t\n""" 
		
		# declare typedefs (nested attribs classes and enums) and collect them for use later
		if (not derived_class):
			for typedef in OutputOrderDictionaryValues(attribs_class['typedefs']):
				class_text += IndentTextTabs(self.GenerateTypedef(typedef),1)
			
			class_text += "\t\n"
			
			# declare the various member variables
			for member in OutputOrderDictionaryValues(attribs_class['members']):
				member_info = self.ComputeMemberInfo(attribs_class,member)
				class_text += '\tpublic %s %s = new %s("%s", %s);\n' % (member_info['type'],member_info['name'],member_info['type'],member_info['tagname'],member_info['default'])
		
		class_text += "\n"
		
		# close out the class declaration
		class_text += '};\n'
		
		return class_text
	
	# typedef = {}			# {name, type, enum, inner_class, derived_class}
	def GenerateTypedef(self,typedef):
		typedef_text = ''
		
		if (typedef['type'] == 'enum'):
			typedef_text += '[FlagsAttribute]\npublic enum %s : uint\n{\n' % (typedef['name'],)
			for enum in typedef['enum']:
				typedef_text += '\t%s = %s,\n' % (enum['name'],enum['value'])
			typedef_text += '};\n\n'
			
			# generate the AttribsMember class
			typedef_text += 'public class AttribsMemberEnum_%s : AttribsMemberEnum\n{\n\tpublic AttribsMemberEnum_%s(string keyName, %s defaultVal) :\n\t\tbase(keyName, defaultVal, typeof(%s))\n\t{\n\t\t;\n\t}\n\t\n' % (typedef['name'],typedef['name'],typedef['name'],typedef['name'])
			typedef_text += '\tprotected static SortedList m_MapAliasToEnum;\n'
			typedef_text += '\tprotected static SortedList m_MapEnumToAlias;\n'
			typedef_text += '\tprotected static SortedList m_MapOrderingIndexToAlias;\n'
			typedef_text += '\tprotected static SortedList m_MapAliasToIndex;\n'
			typedef_text += '\tprotected static SortedList m_MapEnumToIndex;\n'
			typedef_text += '\tstatic AttribsMemberEnum_%s()\n\t{\n' % (typedef['name'],)
			typedef_text += '\t\tm_MapAliasToEnum = new SortedList();\n'
			typedef_text += '\t\tm_MapEnumToAlias = new SortedList();\n'
			typedef_text += '\t\tm_MapOrderingIndexToAlias = new SortedList();	// map of the ordering indexes from the xml file to aliases. The ordering indexes are not guranteed to be continuous or 0 based. \n'
			typedef_text += '\t\tm_MapAliasToIndex = new SortedList();	// This is different than the ordering index. This function takes an alias and returns what index it is in the GetAliases() list. \n'
			typedef_text += '\t\tm_MapEnumToIndex = new SortedList();	// This is different than the ordering index. This function takes an alias and returns what index it is in the GetAliases() list. \n'
			alias_ordering = {}
			enum_ordering = {}
			for enum in typedef['enum']:
				typedef_text += '\t\tm_MapAliasToEnum.Add("%s",%s.%s);\n' % (enum['alias'],typedef['name'],enum['name'])
				typedef_text += '\t\tm_MapEnumToAlias.Add(%s.%s,"%s");\n' % (typedef['name'],enum['name'],enum['alias'])
				typedef_text += '\t\tm_MapOrderingIndexToAlias.Add(%s,"%s");\n' % (enum['order'],enum['alias'])
				# build the data used to create m_MapAliasToIndex
				alias_ordering[int(enum['order'])] = enum['alias']
				enum_ordering[int(enum['order'])] = '%s.%s' % (typedef['name'],enum['name'])
			index = 0
			for alias in alias_ordering.values():
				typedef_text += '\t\tm_MapAliasToIndex.Add("%s",%s);\n' % (alias,index)	#alias_ordering[alias],alias)
				index += 1
			index = 0
			for enum in enum_ordering.values():
				typedef_text += '\t\tm_MapEnumToIndex.Add(%s,%s);\n' % (enum,index)	#alias_ordering[alias],alias)
				index += 1
			
			typedef_text += '\t}\n\t\n'
			
			typedef_text += """
	public static string GetAlias(System.Enum enum_value)
	{
		return (string)m_MapEnumToAlias[enum_value];
	}
	public static StringCollection GetAliases()
	{
		StringCollection alias_list = new StringCollection();
		foreach (string alias in m_MapOrderingIndexToAlias.Values)
		{
			alias_list.Add(alias);
		}
		return alias_list;
	}
	public static System.Enum GetEnumValueFromAlias(string alias)
	{
		return (System.Enum)m_MapAliasToEnum[alias];
	}
	public static int GetIndexFromAlias(string alias)
	{
		return (int)m_MapAliasToIndex[alias];
	}
	public static int GetIndexFromEnum(System.Enum enum_value)
	{
		return (int)m_MapEnumToIndex[enum_value];
	}
	
	public string GetAlias()
	{
		return GetAlias(EVal);
	}
	public void SetEnumValueFromAlias(string alias)
	{
		EVal = GetEnumValueFromAlias(alias);
	}\n\n"""
			typedef_text += '\tpublic static implicit operator %s(AttribsMemberEnum_%s t)\n\t{\n\t\treturn (%s)t.m_tVal;\n\t}\n\t\n\tpublic %s TVal\n\t{\n\t\tget {return this;}\n\t\tset {m_tVal = (Object)value;}\n\t}\n}\n\n' % (typedef['name'],typedef['name'],typedef['name'],typedef['name'])
		elif (typedef['type'] == 'class'):
			typedef_text += self.GenerateAttribsClass(typedef['inner_class'],1,0)
			typedef_text += '\n'
			typedef_text += """public class AttribsMemberAttribsClass_%s : AttribsMemberAttribsClass\n{\n\tpublic AttribsMemberAttribsClass_%s(string keyName, %s defaultVal) : \n\t\tbase(keyName, defaultVal)\n\t{\n\t\t;\n\t}\n\t\n\tpublic static implicit operator %s(AttribsMemberAttribsClass_%s t)\n\t{\n\t\treturn (%s)t.m_tVal;\n\t}\n\n\tpublic %s TVal\n\t{\n\t\tget {return this;}\n\t\tset {m_tVal = (Object)value;}\n\t}\n}\n""" % (typedef['name'],typedef['name'],typedef['name'],typedef['name'],typedef['name'],typedef['name'],typedef['name'])
		elif (typedef['type'] == 'derived'):
			typedef_text += self.GenerateAttribsClass(typedef['derived_class'],1,1)
			typedef_text += '\n'
			typedef_text += """public class AttribsMemberAttribsClass_%s : AttribsMemberAttribsClass\n{\n\tpublic AttribsMemberAttribsClass_%s(string keyName, %s defaultVal) : \n\t\tbase(keyName, defaultVal)\n\t{\n\t\t;\n\t}\n\t\n\tpublic static implicit operator %s(AttribsMemberAttribsClass_%s t)\n\t{\n\t\treturn (%s)t.m_tVal;\n\t}\n\n\tpublic %s TVal\n\t{\n\t\tget {return this;}\n\t\tset {m_tVal = (Object)value;}\n\t}\n}\n""" % (typedef['name'],typedef['name'],typedef['name'],typedef['name'],typedef['name'],typedef['name'],typedef['name'])
		elif (typedef['type'] == 'collection' and typedef['collection']['type']=='List'):
			value_type = typedef['collection']['value_type']
			if (self.TranslateMemberTypeUnderlying(typedef['collection']['value_type'])):
				value_type = self.TranslateMemberTypeUnderlying(typedef['collection']['value_type'])
			typedef_text += '\npublic class %s : AttribsMemberGenericList {public %s(string keyName, ArrayList defaultVal) : base(keyName, typeof(%s), defaultVal){;} }\n' % (typedef['name'], typedef['name'], value_type)
		elif (typedef['type'] == 'collection' and typedef['collection']['type']=='Map'):
			key_type = typedef['collection']['key_type']
			if (self.TranslateMemberTypeUnderlying(typedef['collection']['key_type'])):
				key_type = self.TranslateMemberTypeUnderlying(typedef['collection']['key_type'])
			value_type = typedef['collection']['value_type']
			if (self.TranslateMemberTypeUnderlying(typedef['collection']['value_type'])):
				value_type = self.TranslateMemberTypeUnderlying(typedef['collection']['value_type'])
			typedef_text += '\npublic class %s : AttribsMemberGenericMap {public %s(string keyName, SortedList defaultVal) : base(keyName, typeof(%s), typeof(%s), defaultVal){;} }\n' % (typedef['name'], typedef['name'], key_type, value_type)
		else:
			pass
		
		return typedef_text
	
	# returns [name, typedef_type, type, default, tagname]
	def ComputeMemberInfo(self, attribs_class, member):
		attrib_name = member['name']
		attrib_tagname = member['tagname']
		attrib_default = member['default']
		attrib_type = member['type']
		attrib_type_underlying = member['type']
		typedef = None
		typedef_name = None
		typedef_type = None
		collection_info = None
		typedef_collection_type = None
		
		# translate the default value
		if (attribs_class['typedefs'].has_key(attrib_type)):
			typedef = attribs_class['typedefs'][attrib_type]
			typedef_name = typedef['name']
			typedef_type = typedef['type']
			try:
				typedef_collection_type = typedef['collection']['type']
			except:
				pass
			
			if (typedef['type']=='class'):
				attrib_default = 'new %s()' % (attrib_type,)
			elif (typedef['type']=='derived'):
				attrib_default = 'new %s()' % (attrib_type,)
			elif (typedef['type']=='enum'):
				attrib_default = '%s.%s' % (attrib_type,attrib_default)
			elif (typedef_type == 'collection'):
				if (typedef_collection_type == 'List'):
					attrib_default = 'new ArrayList()'
				elif (typedef_collection_type == 'Map'):
					attrib_default = 'new SortedList()'
			else:
				attrib_default = '0 /* Unknown typedef %s %s */' % (attrib_type,str(typedef))
		elif (attrib_type == 'string'):
			attrib_default = '"%s"' % (EscapeCStringLiteral(attrib_default),)
		elif (attrib_type == 'DateTime'):
			attrib_default = 'AttribFormat.ConvertStringToDateTime("%s")' % (attrib_default,)
		elif (attrib_type[:5] == 'list('):
			attrib_default = 'new ArrayList()'
		elif (attrib_type[:4] == 'map('):
			attrib_default = 'new SortedList()'
		elif (attrib_type == 'byte[]'):
			attrib_default = 'new byte[]%s' % (EncodeBufferLiteral(attrib_default)[0],)
		
		# translate the type
		attrib_translated_type = self.TranslateMemberType(attrib_type)
		if (attrib_translated_type):
			attrib_type = attrib_translated_type
		else:
			if (attribs_class['typedefs'].has_key(attrib_type)):
				if (typedef_type == 'enum'):
					attrib_type = 'AttribsMemberEnum_%s' % (attrib_type)
				elif (typedef_type == 'class'):
					attrib_type = 'AttribsMemberAttribsClass_%s' % (attrib_type)
				elif (typedef_type == 'derived'):
					attrib_type = 'AttribsMemberAttribsClass_%s' % (attrib_type)
				elif (typedef_type == 'collection'):
					if (typedef_collection_type == 'List'):
						value_type = typedef['collection']['value_type']
						if (self.TranslateMemberTypeUnderlying(typedef['collection']['value_type'])):
							value_type = self.TranslateMemberTypeUnderlying(typedef['collection']['value_type'])
						collection_info = {'type': typedef_collection_type, 'value_type': value_type}						
					elif (typedef_collection_type == 'Map'):
						key_type = typedef['collection']['key_type']
						if (self.TranslateMemberTypeUnderlying(typedef['collection']['key_type'])):
							key_type = self.TranslateMemberTypeUnderlying(typedef['collection']['key_type'])
						value_type = typedef['collection']['value_type']
						if (self.TranslateMemberTypeUnderlying(typedef['collection']['value_type'])):
							value_type = self.TranslateMemberTypeUnderlying(typedef['collection']['value_type'])
						collection_info = {'type': typedef_collection_type, 'key_type': key_type, 'value_type': value_type}
					# attrib_type = 'AttribsMemberGenericList' % (attrib_type,)
				else:
					attrib_type = '%s /* Unknown type (0) */' % (attrib_type,)
			else:
				attrib_type = '%s /* Unknown type (1) */' % (attrib_type,)
		
		return {'name': attrib_name, 'typedef_type': typedef_type, 'type': attrib_type, 'default': attrib_default, 'tagname': attrib_tagname, 'typedef': typedef, 'collection_info': collection_info}
		
	
	def ComputeFilePath(self, filename_base):
		return 'LicAttribsCS\\' + self.ComputeFileName(filename_base)
	def ComputeFileName(self, filename_base):
		return filename_base + '.cs'
	
	def GenerateGenericAttribsFile(self, attribs_class):
		# generate and write the header file text
		self.WriteFileIfDifferent(self.ComputeFilePath(attribs_class['filename']), """using System;\nusing System.Collections;\nusing System.Collections.Specialized;\n\nnamespace Solimar\n{\n\tnamespace Licensing\n\t{\n\t\tnamespace Attribs\n\t\t{\n%s\n\t\t}\n\t}\n}\n""" % (IndentTextTabs(self.GenerateAttribsClass(attribs_class,0,0),3),))
		
	def TranslateMemberType(self,type):
		if (self.type_map.has_key(type)):
			return self.type_map[type]
		return None
	
	def TranslateMemberTypeUnderlying(self,type):
		if (self.type_map_underlying.has_key(type)):
			return self.type_map_underlying[type]
		return None

