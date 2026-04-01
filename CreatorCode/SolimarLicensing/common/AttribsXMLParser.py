from AttribsOutput import *
import re,string,xml.dom.minidom
import copy


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
class AttribsXMLParser:
	def __init__(self):
		pass
	
	def ParseXML(self, xmlroot, known_classes):
		# for each attribs class
		for attribs_class in getChildElementsByTagName(xmlroot,'AttribsClass'):
			cls = self.ParseXMLClass(attribs_class, 0, known_classes)
			known_classes[attribs_class.getAttribute('name')] = cls
			# compute the includes of other classes needed by this class (when creating a derived class is currently when this happens)
			self.ParseXMLComputeClassIncludes(cls, cls)
		
		
	def ParseXMLClass(self, class_xml, inner_class, known_classes):
		attributes = {}
		attributes['name'] = class_xml.getAttribute('name')
		attributes['descriptive_name'] = class_xml.getAttribute('descriptive_name')
		attributes['base'] = class_xml.getAttribute('base')
		attributes['filename'] = class_xml.getAttribute('filename')
		attributes['code_class_name'] = class_xml.getAttribute('code_class_name')
		attributes['comments'] = ''
		attributes['typedefs'] = {}
		attributes['members'] = {}
		attributes['derived_base'] = class_xml.getAttribute('derived_base')
		attributes['subtype'] = class_xml.getAttribute('subtype')
		attributes['module_id'] = class_xml.getAttribute('module_id')
		attributes['includes'] = class_xml.getAttribute('includes') 
		attributes['alternate_license_class'] = class_xml.getAttribute('alternate_license_class')
		
		# decide on the class name as it appears in code
		if (len(attributes['code_class_name'])==0):
			if (not inner_class):
				attributes['code_class_name'] = "%sAttribs" % (attributes['name'],)
			else:
				attributes['code_class_name'] = attributes['name']
		
		# decide on the filename prefix to use for the generated code files
		if (len(attributes['filename'])==0):
			attributes['filename'] = "%sAttribs" % (class_xml.getAttribute('name'),)
		
		# get the comments
		try:
			node = getChildElementsByTagName(class_xml,'comments')[0]
			for child in node.childNodes:
				# if child is text node
				if (child.nodeType == 3):
					attributes['comments'] += child.data

		except:
			attributes['comments'] = ''
		
		# parse the typedefs
		i = 0
		for typedef in getChildElementsByTagName(class_xml,'typedef'):
			attributes['typedefs'][typedef.getAttribute('name')] = self.ParseXMLTypedef(typedef, known_classes)
			attributes['typedefs'][typedef.getAttribute('name')]['output_order'] = i
			i+=1
		
		# parse the member variables
		for attrib in getChildElementsByTagName(class_xml,'attrib'):
			attributes['members'][attrib.getAttribute('name')] = self.ParseXMLAttribute(attrib, known_classes)
			attributes['members'][attrib.getAttribute('name')]['output_order'] = i
			i+=1
		
		return attributes
	
	def ParseXMLTypedef(self, typedef_xml, known_classes):
		attributes = {}
		attributes['name'] = typedef_xml.getAttribute('name')
		attributes['type'] = typedef_xml.getAttribute('type')
		attributes['enum'] = None
		attributes['inner_class'] = None
		attributes['derived_class'] = None
		attributes['collection'] = None
		#typedef = {}			# {name, type, enum, inner_class, derived_class}
		
		if (attributes['type'] == 'enum'):
			attributes['enum'] = []
			max_order = 0
			for enum in getChildElementsByTagName(typedef_xml,'enum'):
				order = max_order+1
				if (len(enum.getAttribute('order'))==0):
					max_order += 1
				else:
					order = int(enum.getAttribute('order'))
					if (order>max_order):
						max_order = order
				alias = enum.getAttribute('name')
				if (len(enum.getAttribute('alias'))>0):
					alias = enum.getAttribute('alias')
				attributes['enum'].append({'name': enum.getAttribute('name'), 'value': enum.getAttribute('value'), 'alias': alias, 'order': order})
		elif (attributes['type'] == 'class'):
			for attribs_class in getChildElementsByTagName(typedef_xml,'AttribsClass'):
				attributes['inner_class'] = self.ParseXMLClass(attribs_class, 1, known_classes)
				break
		elif (attributes['type'] == 'derived'):
			for attribs_class in getChildElementsByTagName(typedef_xml,'AttribsClassDerived'):
				# copy the class that this class derives from
				attributes['derived_class'] = copy.deepcopy(known_classes[attribs_class.getAttribute('base_class')])
				# set the name and derived from name
				attributes['derived_class']['name'] = attribs_class.getAttribute('name')
				attributes['derived_class']['derived_base_code_class'] = attributes['derived_class']['code_class_name']
				attributes['derived_class']['code_class_name'] = attribs_class.getAttribute('code_class_name')
				if (len(attributes['derived_class']['code_class_name'])==0):
					attributes['derived_class']['code_class_name'] = attributes['derived_class']['name']
				attributes['derived_class']['base'] = attribs_class.getAttribute('base')
				attributes['derived_class']['derived_base'] = attribs_class.getAttribute('base_class')
				# overwrite the member defaults
				for default_override in getChildElementsByTagName(attribs_class,'attrib'):
					if (attributes['derived_class']['members'].has_key(default_override.getAttribute('name'))):
						attributes['derived_class']['members'][default_override.getAttribute('name')]['default'] = default_override.getAttribute('default')
				break
		elif (attributes['type'] == 'collection'):
			for collection in getChildElementsByTagName(typedef_xml,'List'):
				attributes['collection'] = {'type': 'List', 'value_type': collection.getAttribute('value_type')}
				break
			for collection in getChildElementsByTagName(typedef_xml,'Map'):
				attributes['collection'] = {'type': 'Map', 'key_type': collection.getAttribute('key_type'), 'value_type': collection.getAttribute('value_type')}
				break
		else:
			print "Unknown typedef: %s %s " % (attributes['name'],attributes['type'])
		return attributes
		
	def ParseXMLAttribute(self, attrib_xml, known_classes):
		attributes = {}
		attributes['name'] = attrib_xml.getAttribute('name')
		attributes['type'] = attrib_xml.getAttribute('type')
		attributes['tagname'] = attrib_xml.getAttribute('tagname')
		attributes['default'] = attrib_xml.getAttribute('default')
		attributes['sec_permit_read'] = attrib_xml.getAttribute('sec_permit_read')
		attributes['sec_permit_write'] = attrib_xml.getAttribute('sec_permit_write')
		return attributes
	
	def ParseXMLComputeClassIncludes(self, top_level_class, attribs_class):
		for typedef in attribs_class['typedefs'].values():
			if (typedef['type'] == 'derived'):
				top_level_class['includes'].append(typedef['derived_class']['filename'])
			if (typedef['type'] == 'class'):
				self.ParseXMLComputeClassIncludes(top_level_class, typedef['inner_class'])
	
	
