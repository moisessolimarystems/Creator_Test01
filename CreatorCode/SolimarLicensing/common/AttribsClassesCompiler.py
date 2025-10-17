import re, string, xml.dom.minidom

from AttribsOutputCPP import *
from AttribsOutputCS import *
from AttribsOutputClassType import *
from AttribsXMLParser import *


# I wanted a non-recursive version. The dom version of this function is recursive.
def getChildElementsByTagName(element, tagname):
	return filter(lambda node: (node.nodeType==1 and node.nodeName==tagname),element.childNodes)


# parse the xml document
dom = xml.dom.minidom.parse("LicAttribsClasses.xml")

# get the root element
root = dom.documentElement

output_classes = [AttribsOutputCPP(),AttribsOutputCS(),AttribsOutputClassType()]

parser = AttribsXMLParser()
classes = {}
parser.ParseXML(root,classes)
#print classes['SModuleType0']['includes']

for output_class in output_classes:
	output_class.ProcessXML(classes)
