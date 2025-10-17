import string, re, os
import xml.dom.minidom

def IndentTextTabs(text,count):
	return IndentText(text,count,'\t')
	
def IndentText(text,count,character):
	whitespace = ''
	for i in range(0,count):
		whitespace += character
	return string.join(map(lambda tab: (whitespace+tab),string.split(text,'\n')),'\n')

def OutputOrderDictionaryValues(typedefs_or_members_dict):
	ordered_values = []
	ordered_keys = None
	d = {}
	for entry in typedefs_or_members_dict.values():
		d[entry['output_order']] = entry
	ordered_keys = d.keys()
	ordered_keys.sort()
	for key in ordered_keys:
		ordered_values.append(d[key])
	return ordered_values

#########################
# xml utility functions #
#########################

# The dom version of this function is recursive. I wanted a non-recursive version.
# static
def getChildElementsByTagName(element, tagname):
		return filter(lambda node: (node.nodeType==1 and node.nodeName==tagname),element.childNodes)
	
# base class for generating an attribs class output
class AttribsOutput:
	def __init__(self):
		pass
	
	def ProcessXML(self, class_list):
		pass
	
	def WriteFileIfDifferent(self, path, data):
		try:
			fin = open(path,'r')
			existing_data = fin.read()
			fin.close()
			if (data != existing_data):
				raise 0
		except:
			try:
				os.system('attrib -r "%s"' % path)
			except:
				pass
			fout = open(path,'w')
			fout.write(data)
			fout.close()
		
