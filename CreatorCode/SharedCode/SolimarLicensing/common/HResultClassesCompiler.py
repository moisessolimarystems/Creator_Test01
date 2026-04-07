import re, string, xml.dom.minidom

from HResultOutputCPP import *
from HResultOutputH import *
from HResultOutputCS import *
from HResultXMLParser import *


# I wanted a non-recursive version. The dom version of this function is recursive.
def getChildElementsByTagName(element, tagname):
	return filter(lambda node: (node.nodeType==1 and node.nodeName==tagname),element.childNodes)

print 'Lic HRESULTs XML compiler started'															# CR.32662.V02; added.

# CR.32662.V02; setup the current working directory if one was passed as a 
# parameter.
if len(sys.argv)==2 :																					# CR.32662.V02; added.
   os.chdir(sys.argv[1])

# parse the xml document
dom = xml.dom.minidom.parse("HResultClass.xml")

# get the root element
root = dom.documentElement

output_classes = [HResultOutputH(),HResultOutputCPP(),HResultOutputCS()]

parser = HResultXMLParser()
classes = {}
parser.ParseXML(root,classes)
#print classes['SModuleType0']['includes']

for output_class in output_classes:
	output_class.ProcessXML(classes)
