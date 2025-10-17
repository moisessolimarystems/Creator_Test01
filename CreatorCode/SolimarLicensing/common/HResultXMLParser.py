from AttribsOutput import *
import re,string,xml.dom.minidom
import copy


"""
classes = {}			# maps name to: class
class = {}			# {name, descriptive_name, base, filename, code_class_name, comments, typedefs, members, derived_base, subtype, module_id}
bases = {}			# maps base name to : {name, value, base} 
offsets = {}                    # maps offset name to : {name, value, base}
hresults = {}                   # maps hresult name to : {offset, name, value, alias}
"""


# outputs the c++ version of the derived attribs class header files
class HResultXMLParser:
	def __init__(self):
		pass
	
	def ParseXML(self, xmlroot, known_classes):
		# for each hresults class
		for hresult_class in getChildElementsByTagName(xmlroot,'HResultClass'):
			cls = self.ParseXMLClass(hresult_class, known_classes)
			known_classes[hresult_class.getAttribute('name')] = cls
		
	def ParseXMLClass(self, class_xml, known_classes):
                #dictionary
    		attributes = {} 
		attributes['name'] = class_xml.getAttribute('name')
		attributes['filename'] = class_xml.getAttribute('filename')
		attributes['code_class_name'] = class_xml.getAttribute('code_class_name')
                attributes['bases'] = {}
		attributes['offsets'] = {}
		attributes['rnbos'] = {}
		attributes['hresults'] = {}
		
		# decide on the class name as it appears in code
		if (len(attributes['code_class_name'])==0):
			attributes['code_class_name'] = attributes['name']

       		i = 0   #counter for order elements are outputted
                for base in getChildElementsByTagName(class_xml, 'Base'):       #bases for offsets
                        attributes['bases'][base.getAttribute('name')] = self.ParseXMLOffset(base,known_classes)
			attributes['bases'][base.getAttribute('name')]['output_order'] = i
			i+=1	                        
                
                #parse the offset variables
		for offset in getChildElementsByTagName(class_xml,'Offset'):    #each attribute name is a key to a attribute value
			attributes['offsets'][offset.getAttribute('name')] = self.ParseXMLOffset(offset,known_classes)
			attributes['offsets'][offset.getAttribute('name')]['output_order'] = i
			i+=1

                #parse the rnbo variables			
		for rnbo in getChildElementsByTagName(class_xml,'RNBO'):    #each attribute name is a key to a attribute value
			attributes['rnbos'][rnbo.getAttribute('name')] = self.ParseXMLOffset(rnbo,known_classes)
			attributes['rnbos'][rnbo.getAttribute('name')]['output_order'] = i
			i+=1
			
		# parse the hresult variables                                    #each hresult key has a hresult value - attribute name
		for hresult_type in getChildElementsByTagName(class_xml,'HResultType'):    #each attribute name is a key to a attribute value
			attributes['hresults'][hresult_type.getAttribute('offset')] = self.ParseXMLHResult(hresult_type,known_classes)
			attributes['hresults'][hresult_type.getAttribute('offset')]['output_order'] = i
			i+=1
		
		return attributes
			
	def ParseXMLHResult(self, hresult_xml, known_classes):
                HResult = {}
                offset = hresult_xml.getAttribute('offset')
                i=0
                for hresult in getChildElementsByTagName(hresult_xml,'HResult'):
		    HResultElement={}
		    HResultElement['offset']=offset
		    HResultElement['name']=hresult.getAttribute('name')
		    HResultElement['ecode']=hresult.getAttribute('ecode')		    
		    HResultElement['value']=hresult.getAttribute('value')
		    HResultElement['alias']=hresult.getAttribute('alias')
                    HResult[i] = HResultElement
                    i+=1
		return HResult

	def ParseXMLOffset(self, hresult_xml, known_classes):
		offsets = {}
		offsets['name'] = hresult_xml.getAttribute('name')
		offsets['value'] = hresult_xml.getAttribute('value')
		offsets['base'] = hresult_xml.getAttribute('base')
		return offsets

