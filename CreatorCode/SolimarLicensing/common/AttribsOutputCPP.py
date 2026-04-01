from AttribsOutput import *
import re,string,xml.dom.minidom,sys


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
	return '<CBuffer><Size>%d</Size><Data>%s</Data></CBuffer>' % (len(s)/2,s[0:len(s)/2 * 2])

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
class AttribsOutputCPP(AttribsOutput):
	def __init__(self):
		AttribsOutput.__init__(self)
		self.type_map = {'string': 'SpdAttribs::WStringAttrib', 'unsigned __int64': 'SpdAttribs::QwordAttrib', 'unsigned __int32': 'SpdAttribs::DwordAttrib', 'unsigned __int16': 'SpdAttribs::WordAttrib', 'unsigned __int8': 'SpdAttribs::ByteAttrib', '__int64': 'SpdAttribs::QintAttrib',  '__int32': 'SpdAttribs::IntAttrib', '__int16': 'SpdAttribs::SintAttrib', '__int8': 'SpdAttribs::CharAttrib', 'bool': 'SpdAttribs::BoolAttrib', 'time_t': 'SpdAttribs::Time_tAttrib', 'size_t': 'SpdAttribs::Size_tAttrib', 'float': 'SpdAttribs::FloatAttrib', 'double': 'SpdAttribs::DoubleAttrib', 'list(unsigned __int32)': 'SpdAttribs::VectorDwordAttrib', 'list(string)': 'SpdAttribs::VectorStringAttrib', 'map(unsigned __int32, unsigned __int32)': 'SpdAttribs::MapDwordDwordAttrib', 'map(unsigned __int32, string)': 'SpdAttribs::MapDwordStringAttrib', 'map(string, unsigned __int32)': 'SpdAttribs::MapStringDwordAttrib', 'map(string, string)': 'SpdAttribs::MapStringStringAttrib', 'byte[]': 'SpdAttribs::BufferAttrib', 'list(byte[])': 'SpdAttribs::VectorBufferAttrib', 'DateTime': 'SpdAttribs::WStringAttrib'}
		self.type_map_underlying = {'string': 'SpdAttribs::WStringObj', 'unsigned __int64': 'SpdAttribs::QwordObj', 'unsigned __int32': 'SpdAttribs::DwordObj', 'unsigned __int16': 'SpdAttribs::WordObj', 'unsigned __int8': 'SpdAttribs::ByteObj', '__int64': 'SpdAttribs::QintObj',  '__int32': 'SpdAttribs::IntObj', '__int16': 'SpdAttribs::SintObj', '__int8': 'SpdAttribs::CharObj', 'bool': 'SpdAttribs::BoolObj', 'time_t': 'SpdAttribs::Time_tObj', 'size_t': 'SpdAttribs::Size_tObj', 'float': 'SpdAttribs::FloatObj', 'double': 'SpdAttribs::DoubleObj', 'list(unsigned __int32)': 'SpdAttribs::VectorDwordObj', 'list(string)': 'SpdAttribs::VectorStringObj', 'map(unsigned __int32, unsigned __int32)': 'SpdAttribs::MapDwordDwordObj', 'map(unsigned __int32, string)': 'SpdAttribs::MapDwordStringObj', 'map(string, unsigned __int32)': 'SpdAttribs::MapStringDwordObj', 'map(string, string)': 'SpdAttribs::MapStringStringObj', 'byte[]': 'SpdAttribs::BufferObj', 'list(byte[])': 'SpdAttribs::VectorBufferObj', 'DateTime': 'SpdAttribs::WStringObj'}
		#self.type_map = {'string': 'SpdWStringAttrib',  'unsigned __int64': 'SpdQWORDAttrib',  'unsigned __int32': 'SpdDWORDAttrib', 'unsigned __int16': 'SpdWORDAttrib', 'unsigned __int8': 'SpdBYTEAttrib', '__int64': 'SpdQINTAttrib',  '__int32': 'SpdINTAttrib', '__int16': 'SpdSINTAttrib', '__int8': 'SpdCHARAttrib', 'bool': 'SpdBOOLAttrib', 'time_t': 'SpdTIME_TAttrib', 'size_t': 'SpdSIZE_TAttrib', 'float': 'SpdFloatAttrib', 'double': 'SpdDoubleAttrib', 'list(unsigned __int32)': 'SpdDWORD_VECTORAttrib', 'list(string)': 'SpdSTRING_VECTORAttrib', 'map(unsigned __int32, unsigned __int32)': 'SpdDWORD_DWORD_MAPAttrib', 'map(unsigned __int32, string)': 'SpdDWORD_STRING_MAPAttrib', 'map(string, unsigned __int32)': 'SpdSTRING_DWORD_MAPAttrib', 'map(string, string)': 'SpdSTRING_STRING_MAPAttrib', 'byte[]': 'SpdBUFFERAttrib', 'list(byte[])': 'SpdBUFFERAttribVECTOR'}
	
	def ProcessXML(self, class_list):
		# for each attribs class
		for attribs_class in class_list.values():
			self.GenerateGenericAttribsFile(attribs_class)
	
	def GenerateMemberInitializer(self, member_info, default_value_override = None):
		class_text = ''
		if (not default_value_override):
			default_value_override = member_info['default']
		
		#attribs class member variables need to use the alternate copy constructor syntax
		if (member_info['typedef_type']=='class'):
			class_text += """,\n\t\t%s(m_mapAttribObjs, L"%s", L"%s", %s)""" % (member_info['name'],member_info['typedef']['inner_class']['name'],member_info['tagname'],default_value_override)
		elif (member_info['typedef_type']=='derived'):
			class_text += """,\n\t\t%s(m_mapAttribObjs, L"%s", L"%s", %s)""" % (member_info['name'],member_info['typedef']['derived_class']['name'],member_info['tagname'],default_value_override)
		elif (member_info['typedef_type']=='collection'):
			class_text += """,\n\t\t%s(m_mapAttribObjs, L"%s", %s)""" % (member_info['name'],member_info['tagname'],default_value_override)
		else:
			class_text = """,\n\t\t%s(m_mapAttribObjs, L"%s", %s)""" % (member_info['name'],member_info['tagname'],default_value_override)
		return class_text
	
	def GenerateMemberInitializerForCopyConstructor(self, member_info, copy_object_name):
		return self.GenerateMemberInitializer(member_info, "%s.%s" % (copy_object_name,member_info['name']))
	
	
	def GenerateAttribsClass(self, attribs_class, inner_class, derived_class):
		
		class_text = ""
		
		# comments
		class_text = CreateComments(attribs_class['comments'])
		
		# this class does not derive off of other attributes classes. 
		# It does not need to initialize base class attribs members via constructors
		if (not derived_class):
			# class declaration header
			class_text += """class %s : public SpdAttribs::CAttribsBase\n{\npublic:\n\tstatic wchar_t* GetAttribsClassName()\n\t{\n\t\t return L"%s";\n\t}\n\t\n""" % (attribs_class['code_class_name'],attribs_class['name'])
		# this class derives off of another attributes class
		# It needs to initialize base class attribs members via constructor parameters to the base class constructor
		elif (derived_class):
			# class declaration header
			class_text += """class %s : public %s\n{\npublic:\n\tstatic wchar_t* GetAttribsClassName()\n\t{\n\t\t return L"%s";\n\t}\n\t\n""" % (attribs_class['code_class_name'],attribs_class['derived_base_code_class'],attribs_class['name'])
		
		# a function to return the descriptive name of the class (if any)
		class_text += '\n\tstatic const wchar_t* DescriptiveName()\n\t{\n\t\treturn L"%s";\n\t}\n' % (EscapeCStringLiteral(attribs_class['descriptive_name']),)
		class_text += '\n\tvirtual const wchar_t* GetDescriptiveName() const\n\t{\n\t\treturn DescriptiveName();\n\t}\n'
		
		# assignment operator
		class_text += "\n\t%s& operator =(const CWStringStream &wsData)\n\t{\n\t\tInitFromString(wsData);\n\t\treturn *this ;\n\t}\n" % (attribs_class['code_class_name'],)
		
		#class_text += "{;}\n\t\n"
		# declare typedefs (nested attribs classes and enums) and collect them for use later
		if (not derived_class):
			typedefs = ''
			for typedef in OutputOrderDictionaryValues(attribs_class['typedefs']):
				typedefs += self.GenerateTypedef(typedef)
			class_text += IndentTextTabs(typedefs,1)
			
			class_text += "\t\n"
			
			# declare the various member variables
			for member in OutputOrderDictionaryValues(attribs_class['members']):
				member_info = self.ComputeMemberInfo(attribs_class,member)
				class_text += '\t%s %s;\n' % (member_info['type'],member_info['name'])
		
		class_text += "\n\t\n"
		
		# assignment operator
		class_text += "\n\t%s& operator =(const %s &c)\n\t{\n" % (attribs_class['code_class_name'],attribs_class['code_class_name'])
		for member in OutputOrderDictionaryValues(attribs_class['members']):
			class_text += '\t\t%s = c.%s;\n' % (member['name'],member['name'])
		class_text += "\t\treturn *this ;\n\t}\n\n"
		
		
		################
		# Constructors #
		################
		
		# this class does not derive off of other attributes classes. 
		# It does not need to initialize base class attribs members via constructors
		if (not derived_class):
			#
			# default constructor
			#
			class_text += """\t%s() : \n\t\tSpdAttribs::CAttribsBase(GetAttribsClassName())""" % (attribs_class['code_class_name'],) #,attribs_class['code_class_name'])
			
			# initialize the member variables
			for member in OutputOrderDictionaryValues(attribs_class['members']):
				class_text += self.GenerateMemberInitializer(self.ComputeMemberInfo(attribs_class, member))
			class_text += "{;}\n\t\n"
			
			#
			# copy constructor
			#
			class_text += """\t%s(const %s &c) : \n\t\tSpdAttribs::CAttribsBase(GetAttribsClassName())""" % (attribs_class['code_class_name'],attribs_class['code_class_name']) #,attribs_class['code_class_name'])
			
			# initialize the member variables
			for member in OutputOrderDictionaryValues(attribs_class['members']):
				class_text += self.GenerateMemberInitializerForCopyConstructor(self.ComputeMemberInfo(attribs_class, member),'c')
			class_text += "{;}\n\t\n"
			
			
			#
			# attribs class linkage constructor
			#
			class_text += """\t%s(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : \n\t\tSpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName)""" % (attribs_class['code_class_name'])
			# initialize the member variables
			for member in OutputOrderDictionaryValues(attribs_class['members']):
				class_text += self.GenerateMemberInitializer(self.ComputeMemberInfo(attribs_class, member))
			class_text += "{;}\n\t\n"
			
			
			#
			# attribs class linkage constructor with copy constructor combination
			#
			class_text += """\t%s(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const %s &c) : \n\t\tSpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName)""" % (attribs_class['code_class_name'],attribs_class['code_class_name'])
			# initialize the member variables
			for member in OutputOrderDictionaryValues(attribs_class['members']):
				class_text += self.GenerateMemberInitializerForCopyConstructor(self.ComputeMemberInfo(attribs_class, member),'c')
			class_text += "{;}\n\t\n"
			
			
			
			if (len(OutputOrderDictionaryValues(attribs_class['members']))>0):
				#
				# override default parameters constructor
				#
				class_text += """\t%s(const CWStringStream &wsAttribsClassName""" % (attribs_class['code_class_name'])
				# create parameters for each of the members that can be overridden
				for member in OutputOrderDictionaryValues(attribs_class['members']):
					member_info = self.ComputeMemberInfo(attribs_class, member)
					# attribs class member variables need to use the two parameter constructor that they expose
					if (member_info['typedef_type']!='class' and member_info['typedef_type']!='derived' and member_info['typedef_type']!='enum'):
                                                #removed ::TypeT from const %s::TypeT
						class_text += ",\n\t\tconst %s &param_%s" % (member_info['type'], member_info['name'])
					elif (member_info['typedef_type']=='enum'):
						class_text += ",\n\t\tconst %s &param_%s" % (member_info['type_underlying'], member_info['name'])
					elif (member_info['typedef_type']=='class' or member_info['typedef_type']=='derived'):
						class_text += ",\n\t\tconst %s &param_%s" % (member_info['type'], member_info['name'])
				class_text += ") : \n\t\t\tSpdAttribs::CAttribsBase(wsAttribsClassName)"
				# sets defaults for non-inherited members
				for member in OutputOrderDictionaryValues(attribs_class['members']):
					member_info = self.ComputeMemberInfo(attribs_class, member)
					# attribs class member variables need to use the two parameter constructor that they expose
					if (member_info['typedef_type']!='class' and member_info['typedef_type']!='derived'):
						class_text += """,\n\t\t\t%s(m_mapAttribObjs, L"%s", param_%s)""" % (member_info['name'],member_info['tagname'],member_info['name'])
					elif (member_info['typedef_type']=='class'):
						class_text += """,\n\t\t\t%s(m_mapAttribObjs, L"%s", L"%s", param_%s)""" % (member_info['name'],member_info['typedef']['inner_class']['name'],member_info['tagname'],member_info['name'])
					elif (member_info['typedef_type']=='derived'):
						class_text += """,\n\t\t\t%s(m_mapAttribObjs, L"%s", L"%s", param_%s)""" % (member_info['name'],member_info['typedef']['derived_class']['name'],member_info['tagname'],member_info['name'])
				class_text += "{;}\n\t\n"
				
				
				#
				# override default parameters constructor with attribs class linkage
				#
				class_text += """\t%s(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName""" % (attribs_class['code_class_name'],)
				
				# create parameters for each of the members that can be overridden
				for member in OutputOrderDictionaryValues(attribs_class['members']):
					member_info = self.ComputeMemberInfo(attribs_class, member)
					# attribs class member variables need to use the two parameter constructor that they expose
					if (member_info['typedef_type']!='class' and member_info['typedef_type']!='derived' and member_info['typedef_type']!='enum'):
                                                # Removed ::TypeT from const %s::TypeT
						class_text += ",\n\t\tconst %s &param_%s" % (member_info['type'], member_info['name'])
					elif (member_info['typedef_type']=='enum'):
						class_text += ",\n\t\tconst %s &param_%s" % (member_info['type_underlying'], member_info['name'])
					elif (member_info['typedef_type']=='class' or member_info['typedef_type']=='derived'):
						class_text += ",\n\t\tconst %s &param_%s" % (member_info['type'], member_info['name'])
				class_text += ") : \n\t\t\tSpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName)"
				# sets defaults for non-inherited members
				for member in OutputOrderDictionaryValues(attribs_class['members']):
					member_info = self.ComputeMemberInfo(attribs_class, member)
					# attribs class member variables need to use the two parameter constructor that they expose
					if (member_info['typedef_type']!='class' and member_info['typedef_type']!='derived'):
						class_text += """,\n\t\t\t%s(m_mapAttribObjs, L"%s", param_%s)""" % (member_info['name'],member_info['tagname'],member_info['name'])
					elif (member_info['typedef_type']=='class'):
						class_text += """,\n\t\t\t%s(m_mapAttribObjs, L"%s", L"%s", param_%s)""" % (member_info['name'],member_info['typedef']['inner_class']['name'],member_info['tagname'],member_info['name'])
					elif (member_info['typedef_type']=='derived'):
						class_text += """,\n\t\t\t%s(m_mapAttribObjs, L"%s", L"%s", param_%s)""" % (member_info['name'],member_info['typedef']['derived_class']['name'],member_info['tagname'],member_info['name'])
				class_text += "{;}\n\t\n"
				
				
		# this class derives off of another attributes class
		# It needs to initialize base class attribs members via constructor parameters to the base class constructor
		elif (derived_class):
			#
			# default constructor
			#
			class_text += """\t%s() : \n\t\t%s(GetAttribsClassName()""" % (attribs_class['code_class_name'],attribs_class['derived_base_code_class'])
			# initialize the base class member variables via constructor arguments
			for member in OutputOrderDictionaryValues(attribs_class['members']):
				member_info = self.ComputeMemberInfo(attribs_class, member)
				#if (member_info['typedef_type']!='class' and member_info['typedef_type']!='derived'):
				class_text += """,\n\t\t\t%s""" % (member_info['default'],)
			class_text += ")"
			class_text += "{;}\n\t\n"
			
			
			#
			# copy constructor
			#
			class_text += """\t%s(const %s &c) : \n\t\t%s(GetAttribsClassName()""" % (attribs_class['code_class_name'],attribs_class['code_class_name'],attribs_class['derived_base_code_class'])
			# initialize the base class member variables via constructor arguments
			for member in OutputOrderDictionaryValues(attribs_class['members']):
				member_info = self.ComputeMemberInfo(attribs_class, member)
				#if (member_info['typedef_type']!='class' and member_info['typedef_type']!='derived'):
				class_text += """,\n\t\t\tc.%s""" % (member_info['name'],)
			class_text += ")"
			class_text += "{;}\n\t\n"
			
			
			#
			# attribs class linkage constructor
			#
			class_text += """\t%s(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : \n\t\t%s(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName""" % (attribs_class['code_class_name'],attribs_class['derived_base_code_class'])
			# initialize the base class member variables via constructor arguments
			for member in OutputOrderDictionaryValues(attribs_class['members']):
				member_info = self.ComputeMemberInfo(attribs_class, member)
				#if (member_info['typedef_type']!='class' and member_info['typedef_type']!='derived'):
				class_text += """,\n\t\t\t%s""" % (member_info['default'],)
			class_text += ")"
			class_text += "{;}\n\t\n"
			
			
			#
			# attribs class linkage constructor with copy constructor combination
			#
			class_text += """\t%s(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const %s &c) : \n\t\t%s(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName""" % (attribs_class['code_class_name'],attribs_class['code_class_name'],attribs_class['derived_base_code_class'])
			# initialize the base class member variables via constructor arguments
			for member in OutputOrderDictionaryValues(attribs_class['members']):
				member_info = self.ComputeMemberInfo(attribs_class, member)
				#if (member_info['typedef_type']!='class' and member_info['typedef_type']!='derived'):
				class_text += """,\n\t\t\t%s.%s""" % ('c',member_info['name'])
			class_text += ")"
			class_text += "{;}\n\t\n"
			
			
		
		class_text += "\n"
		
		# close out the class declaration
		class_text += '};\n'
		
		return class_text
	
	# typedef = {}			# {name, type, enum, inner_class, derived_class}
	def GenerateTypedef(self,typedef):
		typedef_text = ''
		if (typedef['type'] == 'enum'):
			typedef_text += 'typedef enum {\n'
			for enum in typedef['enum']:
				typedef_text += '\t%s = %s,\n' % (enum['name'],enum['value'])
			typedef_text += '} %s;\n\n' % (typedef['name'],)
		elif (typedef['type'] == 'class'):
			typedef_text += self.GenerateAttribsClass(typedef['inner_class'],1,0)
			typedef_text += '\n'
		elif (typedef['type'] == 'derived'):
			typedef_text += self.GenerateAttribsClass(typedef['derived_class'],1,1)
			typedef_text += '\n'
		elif (typedef['type'] == 'collection' and typedef['collection']['type']=='List'):
			value_type = typedef['collection']['value_type']
			if (self.TranslateMemberTypeUnderlying(typedef['collection']['value_type'])):
				value_type = self.TranslateMemberTypeUnderlying(typedef['collection']['value_type'])
			typedef_text += 'typedef std::vector< %s > TVector_%s;\n' % (value_type, typedef['name'])
			typedef_text += 'typedef SpdAttribs::CStreamableVectors< TVector_%s  > Vector_%s_Obj;\n' % (typedef['name'], typedef['name'])
			typedef_text += 'typedef SpdAttribs::CAttribMemberBaseU< Vector_%s_Obj, TVector_%s > %s;\n' % (typedef['name'], typedef['name'], typedef['name'])
		elif (typedef['type'] == 'collection' and typedef['collection']['type']=='Map'):
			key_type = typedef['collection']['key_type']
			if (self.TranslateMemberTypeUnderlying(typedef['collection']['key_type'])):
				key_type = self.TranslateMemberTypeUnderlying(typedef['collection']['key_type'])
			value_type = typedef['collection']['value_type']
			if (self.TranslateMemberTypeUnderlying(typedef['collection']['value_type'])):
				value_type = self.TranslateMemberTypeUnderlying(typedef['collection']['value_type'])
			typedef_text += 'typedef std::map< %s , %s > TMap_%s;\n' % (key_type, value_type, typedef['name'])
			typedef_text += 'typedef SpdAttribs::CStreamableMaps< TMap_%s  > Map_%s_Obj;\n' % (typedef['name'], typedef['name'])
			typedef_text += 'typedef SpdAttribs::CAttribMemberBaseU< Map_%s_Obj , TMap_%s >  %s;\n' % (typedef['name'], typedef['name'], typedef['name'])
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
		
		attrib_translated_type = self.TranslateMemberType(attrib_type)
		
		# translate the default value
		if (attribs_class['typedefs'].has_key(attrib_type)):
			typedef = attribs_class['typedefs'][attrib_type]
			typedef_name = typedef['name']
			typedef_type = typedef['type']
			try:
				typedef_collection_type = typedef['collection']['type']
			except:
				pass
			
			if (typedef_type == 'class'):
				attrib_default = '%s()' % (attrib_type,)
			elif (typedef_type == 'derived'):
				attrib_default = '%s()' % (attrib_type,)
			elif (typedef_type == 'enum'):
				attrib_default = '%s' % (attrib_default,)
			elif (typedef_type == 'collection'):
				if (typedef_collection_type == 'List'):
					attrib_default = 'Vector_%s_Obj()' % (typedef_name,)
				elif (typedef_collection_type == 'Map'):
					attrib_default = 'Map_%s_Obj()' % (typedef_name,)
			else:
				attrib_default = '0 /* Unknown typedef %s %s */' % (attrib_type,str(typedef))
		elif (attrib_type == 'string'):
			attrib_default = 'SpdAttribs::WStringObj(L"%s")' % (EscapeCStringLiteral(attrib_default),)
		elif (attrib_type == 'DateTime'):
			attrib_default = 'SpdAttribs::WStringObj(L"%s")' % (attrib_default,)
		elif (attrib_type[:5] == 'list('):
			attrib_default = '%s()' % (attrib_translated_type[:-6] + 'Obj',)
		elif (attrib_type[:4] == 'map('):
			attrib_default = '%s()' % (attrib_translated_type[:-6] + 'Obj',)
		elif (attrib_type == 'byte[]'):
			attrib_default = '%s(L"%s")' % (attrib_translated_type[:-6] + 'Obj', EncodeBufferLiteral(attrib_default))
		
		# translate the type
		if (attrib_translated_type):
			attrib_type = attrib_translated_type
		else:
			if (attribs_class['typedefs'].has_key(attrib_type)):
				if (typedef_type == 'enum'):
					attrib_type = 'SpdAttribs::AttribEnumTypeMember<%s>' % (attrib_type,)
				elif (typedef_type == 'class'):
					attrib_type = '%s' % (attrib_type,)
				elif (typedef_type == 'derived'):
					attrib_type = '%s' % (attrib_type,)
				elif (typedef_type == 'collection'):
					attrib_type = '%s' % (attrib_type,)
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
				else:
					attrib_type = '%s /* Unknown type (0) */' % (attrib_type,)
			else:
				attrib_type = '%s /* Unknown type (1) */' % (attrib_type,)
		
		return {'name': attrib_name, 'typedef_type': typedef_type, 'type': attrib_type, 'type_underlying': attrib_type_underlying, 'default': attrib_default, 'tagname': attrib_tagname, 'typedef': typedef, 'collection_info': collection_info}
		
	
	def ComputeFilePath(self, filename_base):
		return 'LicAttribsCPP\\' + self.ComputeFileName(filename_base)
	def ComputeFileName(self, filename_base):
		return filename_base + '.h'
	
	def GenerateGenericAttribsFile(self, attribs_class):
		define_protect_name = "__%sAttribs_h__" % (attribs_class['name'],)
		output_path = self.ComputeFilePath(attribs_class['filename'])
		
		# generate and write the header file text
		data = ''
		data += '#ifndef %s\n#define %s\n\n#include "LicAttribsBase.h"\n' % (define_protect_name,define_protect_name)
		for include in attribs_class['includes']:
			data += '#include "%s"\n' % self.ComputeFileName(include)
		data += self.GenerateAttribsClass(attribs_class,0,0)
		data += '\n'
		data += '#endif //%s\n' % (define_protect_name,)
		self.WriteFileIfDifferent(output_path, data)
		
	def TranslateMemberType(self,type):
		if (self.type_map.has_key(type)):
			return self.type_map[type]
		return None
	
	
	def TranslateMemberTypeUnderlying(self,type):
		if (self.type_map_underlying.has_key(type)):
			return self.type_map_underlying[type]
		return None
	


