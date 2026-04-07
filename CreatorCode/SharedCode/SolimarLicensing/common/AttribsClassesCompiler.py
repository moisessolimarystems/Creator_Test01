# AttribsClassesCompiler.py - Top level script which determines which xml files get parsed,
# then which output scripts get called to generate the needed attribs classes to build 
# the licensing project.
#
# SLB 11.jun.2024 CR.32662.V03; Modifications to pass working directory to the script.
# Removed calls to generate output types as they are a femnant from SPDE needs, but
# are not used in the licensing implementation.

import re, string, xml.dom.minidom

from AttribsOutput4CPP import *
from AttribsOutputCS import *
from AttribsXMLParser import *


# I wanted a non-recursive version. The dom version of this function is recursive.
def getChildElementsByTagName(element, tagname):
	return filter(lambda node: (node.nodeType==1 and node.nodeName==tagname),element.childNodes)

print 'Lic Attribs XML compiler started'															# CR.32662; added.

# CR.32662; setup the current working directory if one was passed asa parameter.
if len(sys.argv)==2 :
   os.chdir(sys.argv[1])

# parse the xml document
dom = xml.dom.minidom.parse("LicAttribsClasses.xml")

# get the root element
root = dom.documentElement

output_classes = [AttribsOutputCPP(),AttribsOutputCS()]										# CR.32662.V03; modified.

parser = AttribsXMLParser()
classes = {}
parser.ParseXML(root,classes)
#print classes['SModuleType0']['includes']

for output_class in output_classes:
	output_class.ProcessXML(classes)
	
print 'Lic Attribs XML compiler completed'														# CR.32662; added.

