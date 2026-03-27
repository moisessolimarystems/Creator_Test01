
import re, string, xml.dom.minidom, copy

def NameToDefineName(prefix,n):
	s = prefix + ' ' + n
	s = string.upper(s)
	s = re.sub(' +','_',s)
	return s

def isNumeric(s):
	return re.search(r'\d+',s)

def XMLProcessKeyFormat(node, formats):
	for child in node.childNodes:
		if (child.nodeType==1):
			if (child.nodeName==u'KeyStorage'):
				XMLProcessKeyStorage(child, formats['regions'])
			if (child.nodeName==u'KeyHeader'):
				XMLProcessKeyHeader(child, formats['headers'])
			if (child.nodeName==u'KeyLayouts'):
				XMLProcessKeyLayouts(child, formats['products'])

def XMLProcessKeyStorage(node, regions):
	for child in node.childNodes:
		if (child.nodeType==1):
			if (child.nodeName==u'KeyStorageRegion'):
				XMLProcessKeyStorageRegion(child, regions)
	
def XMLProcessKeyStorageRegion(node, regions):
	attribs = {}
	for i in range(0,node.attributes.length):
		attribs[node.attributes.item(i).nodeName]=node.attributes.item(i).nodeValue
	regions.append(attribs)
	
def XMLProcessKeyHeader(node, headers):
	for child in node.childNodes:
		if (child.nodeType==1):
			if (child.nodeName==u'HeaderField'):
				XMLProcessHeaderField(child, headers)

def XMLProcessHeaderField(node, headers):
	attribs = {'default': '0'}
	for i in range(0,node.attributes.length):
		attribs[node.attributes.item(i).nodeName]=node.attributes.item(i).nodeValue
	headers.append(attribs)

def XMLProcessKeyLayouts(node, products):
	for child in node.childNodes:
		if (child.nodeType==1):
			if (child.nodeName==u'KeyLayout'):
				XMLProcessKeyLayout(child, products)

def XMLProcessKeyLayout(node, products):
	
	all_modules = []
	
	# collect the module definitions
	for child in node.childNodes:
		if (child.nodeType==1):
			if (child.nodeName==u'ModuleList'):
				XMLProcessModuleList(child, all_modules)
	
	# collect the product list, and provide it with the module definitions
	for child in node.childNodes:
		if (child.nodeType==1):
			if (child.nodeName==u'ProductList'):
				XMLProcessProductList(child, products, all_modules)

def XMLProcessModuleList(node, modules):
	for child in node.childNodes:
		if (child.nodeType==1):
			if (child.nodeName==u'Module'):
				XMLProcessModule(child, modules)

def XMLProcessModule(node, modules):
	attribs = {}
	for i in range(0,node.attributes.length):
		attribs[node.attributes.item(i).nodeName]=node.attributes.item(i).nodeValue
	modules.append(attribs)

def XMLProcessProductList(node, products, all_modules):
	for child in node.childNodes:
		if (child.nodeType==1):
			if (child.nodeName==u'Product'):
				XMLProcessProduct(child, products, all_modules)

def XMLProcessProduct(node, products, all_modules):
	# get the name and id of the product
	product = {}
	modules = []
	
	for i in range(0,node.attributes.length):
		product[node.attributes.item(i).nodeName]=node.attributes.item(i).nodeValue
	
	# get the modules for the product
	for child in node.childNodes:
		if (child.nodeType==1):
			if (child.nodeName==u'ModuleInclude'):
				XMLProcessModuleInclude(child, modules, all_modules)
	
	product['modules'] = modules
	products.append(product)

def XMLProcessModuleInclude(node, prod_modules, all_modules):
	
	# read this xml ModuleInclude
	this_module = {'default':'0', 'unlimited':0, 'pool':'0', 'isPool':'false', 'isLicense':'true'}
	for i in range(0,node.attributes.length):
		this_module[node.attributes.item(i).nodeName]=node.attributes.item(i).nodeValue
		
	# use the id of the xml ModuleInclude and match it with the corresponding xml Module
	for module in all_modules:
		if (module['id']==this_module['id']):
			# copy the properties from the Module tag to the ModuleInclude tag
			for key in module.keys():
				this_module[key]=copy.deepcopy(module[key])
			break
	
	prod_modules.append(this_module)

# pass in a prefix string and a list of strings that form the name of the variable
# returns a c compatible identifier
# eg.:  MakeCIdentifier('Prefix',['name1 and stuff','things']) -> 'Prefix_name1_and_stuff_things_9830981298309'
# user is required to make sure that Prefix begins with a valid C identifier 
#   (eg.: 'Name name' is okay, but '4name' and '%name name' are not)
def MakeCIdentifier(prefix,namelist):
	ident = prefix
	for i in namelist:
		ident += ' '+i
	return re.sub(r'\s+','_',re.sub(r'[^a-zA-Z0-9\s]+','',ident))
	
	
##############################
# Parse the KeySpec.xml file #
##############################

formats={'headers':[], 'products':[], 'regions':[]}

dom = xml.dom.minidom.parse('.\\resources\\KeySpec.xml')
root = dom.firstChild

XMLProcessKeyFormat(root, formats)


######################################################
# create the module read/write function include file #
######################################################

fout = open('.\\resources\\KeySpecFunctions.h','w')

for product in formats['products']:
	for module in product['modules']:
		# static unsigned int ReadWrite_IdentityFunction(const unsigned int value) { return value; }
		function_name_read = MakeCIdentifier('ReadWrite',[product['name'],product['id'],module['name'],module['id'],'Read'])
		function_name_write = MakeCIdentifier('ReadWrite',[product['name'],product['id'],module['name'],module['id'],'Write'])
		
		if (module.has_key('licenseRead') and len(module['licenseRead'])>0):
			fout.write('\t\tstatic unsigned int %s(const unsigned int value) { return (%s); }\n' % (function_name_read,module['licenseRead']))
		if (module.has_key('licenseWrite') and len(module['licenseWrite'])>0):
			fout.write('\t\tstatic unsigned int %s(const unsigned int value) { return (%s); }\n' % (function_name_write,module['licenseWrite']))

fout.close()



#########################################################
# create the data structure initialization include file #
#########################################################

fout = open('.\\resources\\KeySpecInitialize.h','w')

for region in formats['regions']:
	fout.write('\tregions.push_back(KeyRegion());\n')
	fout.write('\tregions.back().type = L"%s";\n' % (region['type'],))
	fout.write('\tregions.back().offset = %s;\n' % (region['offset'],))
	fout.write('\tregions.back().bits = %s;\n' % (region['bits'],))
fout.write('\t\n')

for header in formats['headers']:
	fout.write('\theaders.data.push_back(Header());\n')
	fout.write('\theaders.data.back().id = %s;\n' % (header['id'],))
	fout.write('\theaders.data.back().name = L"%s";\n' % (header['name'],))
	fout.write('\theaders.data.back().offset = %s;\n' % (header['offset'],))
	fout.write('\theaders.data.back().bits = %s;\n' % (header['bits'],))
	fout.write('\theaders.data.back().default_value = %s;\n' % (header['default'],))
fout.write('\theaders.Initialize();\n\n')

for product in formats['products']:
	fout.write('\tproducts.data.push_back(Product());\n')
	fout.write('\tproducts.data.back().id = %s;\n' % (product['id'],))
	fout.write('\tproducts.data.back().name = L"%s";\n' % (product['name'],))
	for module in product['modules']:
		# static unsigned int ReadWrite_IdentityFunction(const unsigned int value) { return value; }
		function_name_read = MakeCIdentifier('ReadWrite',[product['name'],product['id'],module['name'],module['id'],'Read'])
		function_name_write = MakeCIdentifier('ReadWrite',[product['name'],product['id'],module['name'],module['id'],'Write'])
		
		fout.write('\tproducts.data.back().data.push_back(Module());\n')
		fout.write('\tproducts.data.back().data.back().id = %s;\n' % (module['id'],))
		fout.write('\tproducts.data.back().data.back().name = L"%s";\n' % (module['name'],))
		fout.write('\tproducts.data.back().data.back().offset = %s;\n' % (module['offset'],))
		fout.write('\tproducts.data.back().data.back().bits = %s;\n' % (module['bits'],))
		fout.write('\tproducts.data.back().data.back().default_license = %s;\n' % (module['default'],))
		fout.write('\tproducts.data.back().data.back().unlimited = %s;\n' % (module['unlimited'],))
		fout.write('\tproducts.data.back().data.back().pool = %s;\n' % (module['pool'],))
		fout.write('\tproducts.data.back().data.back().isPool = %s;\n' % (module['isPool'],))
		fout.write('\tproducts.data.back().data.back().isLicense = %s;\n' % (module['isLicense'],))
		
		if (module.has_key('licenseRead') and len(module['licenseRead'])>0):
			fout.write('\tproducts.data.back().data.back().fn_read = Module::%s;\n' % (function_name_read,))
		else:
			fout.write('\tproducts.data.back().data.back().fn_read = Module::ReadWrite_IdentityFunction;\n')
		if (module.has_key('licenseWrite') and len(module['licenseWrite'])>0):
			fout.write('\tproducts.data.back().data.back().fn_write = Module::%s;\n' % (function_name_write,))
		else:
			fout.write('\tproducts.data.back().data.back().fn_write = Module::ReadWrite_IdentityFunction;\n')
		
	fout.write('\tproducts.data.back().Initialize();\n')
fout.write('\tproducts.Initialize();\n\n')

#####################################################################################
# Analysis of the key spec data. Reports back inconsistencies in the key structures #
#####################################################################################

def union(a,b):
	u = copy.deepcopy(a)
	for i in b:
		u.append(copy.deepcopy(i))
	return u

for product in formats['products']:
	used_key_bits = {}
	#print 'Product:', product['name']
	#for storage in union(formats['headers'],product['modules']):
	for storage in product['modules']:
		#print storage['name']
		offset = int(string.replace(storage['offset'],'0x',''),16)
		for i in range(0,int(storage['bits'])):
			blah = '<module id="%s" name="%s"/>' % (storage['id'],storage['name'])
			if (used_key_bits.has_key(offset+i)):
				pass
				#print 'Conflict:',used_key_bits[offset+1],blah
			used_key_bits[offset+i]=blah


fout.close()



fout = open('.\\resources\\KeySpec.html','w')

def ModuleDetailsHtml(module):
	s = ""
	for key in ['name','id','offset','bits','isLicense','default','unlimited','isPool','pool','licenseRead','licenseWrite']:
		if (module.has_key(key)):
			s = s + key + ": " + str(module[key]) + "<br/>"
	return s

def HeaderDetailsHtml(module):
	s = ""
	for key in ['name','id','offset','bits']:
		if (module.has_key(key)):
			s = s + key + ": " + str(module[key]) + "<br/>"
	return s
	
def CellColor(cell_number, regions):
	for region in formats['regions']:
		offset = int(region['offset'],16)
		bits = int(region['bits'])
		type = region['type']
		if ((cell_number*16)>=offset and (cell_number*16)<offset+bits):
			if (type == 'serial'):
				return '#00FFFF'
			if (type == 'inaccessible'):
				return '#DFDFDF'
			if (type == 'module'):
				return '#8080FF'
			if (type == 'header'):
				return '#FF00FF'
	return '#000000'

fout.write("""
<html>
<head>
<script language="JavaScript">

function show(obj)
{
	document.all(obj).style.visibility = 'visible';
}

function hide(obj)
{
	document.all(obj).style.visibility = 'hidden';
}

function toggleDetail(obj)
{
	if (document.all(obj).style.visibility == 'visible')
	{
		hide(obj);
	}
	else
	{
		show(obj);
	}
}

</script>
</head>
<body style="font-family: Arial;">
<br/>
Legend:<br/>
<div id="legend_0" style="overflow: hidden; width: 80px; height: 50px; border-width: 1px; border-style: solid; background-color: #00FFFF;"><div style="font-size: 10pt; font-face: Arial;">Serial Number</div></div>
<div id="legend_1" style="overflow: hidden; width: 80px; height: 50px; border-width: 1px; border-style: solid; background-color: #DFDFDF;"><div style="font-size: 10pt; font-face: Arial;">Inaccessible</div></div>
<div id="legend_2" style="overflow: hidden; width: 80px; height: 50px; border-width: 1px; border-style: solid; background-color: #FF00FF;"><div style="font-size: 10pt; font-face: Arial;">Key Header</div></div>
<div id="legend_3" style="overflow: hidden; width: 80px; height: 50px; border-width: 1px; border-style: solid; background-color: #8080FF;"><div style="font-size: 10pt; font-face: Arial;">Module</div></div>
""")
for product in formats['products']:
	fout.write("""<br/>%s (%s)<br/>""" % (product['name'],product['id']))
	fout.write("""<div style="position: relative; width: 700px; height: 460px;">""")
	for i in range(0,8):
		fout.write("""<div style="position: absolute; left: %d; top: %d; width: 80px; height: 60px; font-size: 10pt; font-family: Arial; border-width: 1px; border-style: solid;"><div style="position: absolute; top: 20px; left: 20px;">%s</div></div>""" % (60+i*80,0,"0x%03x" % (i*16,)))
		fout.write("""<div style="position: absolute; left: %d; top: %d; width: 60px; height: 50px; font-size: 10pt; font-family: Arial; border-width: 1px; border-style: solid;"><div style="position: absolute; top: 15px; left: 10px;">%s</div></div>""" % (0,60+i*50,"0x%03x" % (i*16*8,)))
	fout.write("""<div id="product_%s" style="position: absolute; left: 60px; top: 60px; width: 660px; height: 420px;">\n""" % (product['id'],))
	for i in range(0,8):
		for j in range(0,8):
			fout.write("""<div id="cell_%d_%d" style="position: absolute; overflow: hidden; left: %dpx; top: %dpx; width: 80px; height: 50px; border-width: 1px; border-style: solid; background-color: %s; text-align: right; vertical-align: bottom;"><div style="position: absolute; top: 30px; left: 60px;">%d</div></div>\n""" % (i,j,i*80,j*50,CellColor(j*8+i,formats['regions']),j*8+i))
	for header in formats['headers']:
		fout.write("""<div id="header_%s_%s" style="padding: 2px 2px 2px 2px; position: absolute; filter:alpha(opacity=66); overflow: hidden; left: %d; top: %d; width: %dpx; height: 50px; border-width: 1px; font-size: 8pt; border-style: solid; background-color: #FFFFFF;" onMouseOver="show('header_detail_%s_%s');" onMouseOut="hide('header_detail_%s_%s');">%s</div>\n""" % (product['id'], header['id'],int(header['offset'],16)%128 * 5,int(header['offset'],16)/128 * 50,int(header['bits'])*5,product['id'],header['id'],product['id'],header['id'],header['name']))
		fout.write("""<div id="header_detail_%s_%s" style="padding: 2px 2px 2px 2px; position: absolute; visibility: hidden; left: 640px; top: 0px; width: 200px; height: 150px; border-width: 1px; font-size: 10pt; border-style: solid; background-color: #FFDFDF;">%s</div>\n""" % (product['id'], header['id'], HeaderDetailsHtml(header)))
	for module in product['modules']:
		fout.write("""<div id="module_%s_%s" style="padding: 2px 2px 2px 2px; position: absolute; filter:alpha(opacity=66); overflow: hidden; left: %d; top: %d;  width: %dpx; height: 50px; border-width: 1px; font-size: 8pt; border-style: solid; background-color: #FFFFFF;" onMouseOver="show('module_detail_%s_%s');" onMouseOut="hide('module_detail_%s_%s');">%s</div>\n""" % (product['id'], module['id'],int(module['offset'],16)%128 * 5,int(module['offset'],16)/128 * 50,int(module['bits'])*5,product['id'],module['id'],product['id'],module['id'],module['name']))
		fout.write("""<div id="module_detail_%s_%s" style="padding: 2px 2px 2px 2px; position: absolute; visibility: hidden; left: 640px; top: 0px; width: 200px; height: 150px; border-width: 1px; font-size: 10pt; border-style: solid; background-color: #FFDFDF;">%s</div>\n""" % (product['id'], module['id'], ModuleDetailsHtml(module)))
	fout.write("</div>\n")
	fout.write("""</div>""")



fout.write("""</body></html>""")