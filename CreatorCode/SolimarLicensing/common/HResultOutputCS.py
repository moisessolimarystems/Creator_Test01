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

# outputs the cs version of the derived attribs class header files
class HResultOutputCS(AttribsOutput):
	def __init__(self):
		AttribsOutput.__init__(self)

	def ProcessXML(self, class_list):
		# for each attribs class
		for hresults_class in class_list.values():
			self.GenerateGenericHResultsFile(hresults_class)
	
	def GenerateHResultsClass(self, hresults_class):
		# class declaration header
		
		class_text = 'public class %s\n{\n\t#region Defines for Custom HRESULTS\n\t\n' % (hresults_class['code_class_name'])
                    
		# set the default values in the constructor
		for base in OutputOrderDictionaryValues(hresults_class['bases']):
			class_text += '\tprivate const uint %s = %s;\n' % (base['name'],base['value'])

		for offset in OutputOrderDictionaryValues(hresults_class['offsets']):
                    if(offset['base'] == ''): 
                        class_text += '\tprivate const uint %s = %s;\n' % (offset['name'],offset['value'])
                    else:
                        class_text += '\tprivate const uint %s = %s+%s;\n' % (offset['name'],offset['base'],offset['value'])

		class_text += '\n\t#region Rainbow Error Codes\n\t\n' 

                for rnbo in OutputOrderDictionaryValues(hresults_class['rnbos']):
                        class_text += '\tpublic const uint %s = %s;\n' % (rnbo['name'],rnbo['value'])

		class_text += '\n\t#endregion\n'
		
                for ordered_hresults in OutputOrderDictionaryValues(hresults_class['hresults']):
                    i=0
                    class_text += '\n'
                    for hresult_group in ordered_hresults.values():
                        if(i==0) :
                            if hresult_group['offset'] == 'ITF_LIC_SVR_OFFSET' :
                                class_text += '\t/* Error Codes used for custom HRESULTs. */\n'
                            else :
                                class_text += '\t// Rainbow driver errors\n'
                        if(i < len(ordered_hresults)-1):
        		    class_text += "\tprivate const uint %s = %s+%s;\n" % (hresult_group['name'], hresult_group['offset'],hresult_group['value'])
                            i+=1        

		class_text += '\t#endregion\n\t\n' 
		class_text += '\t#region Defines for Custom HRESULTS Messages\n\n'
		class_text += '\tprivate static System.Collections.Hashtable SolLicErrors = null;\n'
		class_text += '\tstatic %s()\n' % (hresults_class['code_class_name'])
		class_text += '\t{\n'
		class_text += '\t\tSolLicErrors = new System.Collections.Hashtable();\n'

                for ordered_hresults in OutputOrderDictionaryValues(hresults_class['hresults']):
                    i=0
                    for hresult_group in ordered_hresults.values():
                        if(i < len(ordered_hresults)-1):
        		    class_text += '\t\tSolLicErrors.Add(%s,"%s");\n' % (hresult_group['name'], hresult_group['alias'])
                            i+=1
                        
		class_text += '\t}\n\t#endregion\n'
                #function : IsCustomHR(uint _hresult)
		class_text += '\tpublic static bool IsCustomHR(uint _hresult)\n'
                class_text += '\t{\n'
                class_text += '\t\treturn (_hresult & ITF_LIC_MIN) > 0;\n'
                class_text += '\t}\n'
		
                #function : GetHRMessage(uint _hresult)    
		class_text += '\tpublic static string GetHRMessage(uint _hresult)\n'
		class_text += '\t{\n'
		class_text += '\t\tstring ErrorMessage = "";\n'
		class_text += '\t\tif(SolLicErrors!=null && SolLicErrors.Contains(_hresult))\n'
		class_text += '\t\t\tErrorMessage = (string)SolLicErrors[_hresult];\n'
		class_text += '\t\treturn ErrorMessage;\n'
		class_text += '\t}\n'

                #function : IsEqual(int errorCode, uint hresult)
		class_text += '\tpublic static bool IsEqual(int errorCode, uint hresult)\n'
		class_text += '\t{\n'
		class_text += '\t\treturn ((uint)errorCode) == hresult;\n'
		class_text += '\t}\n'
		
		# close out the class declaration
		class_text += '}'
		
		return class_text		
	
	def ComputeFilePath(self, filename_base):
		return self.ComputeFileName(filename_base)
	def ComputeFileName(self, filename_base):
		return filename_base + '.cs'
	
	def GenerateGenericHResultsFile(self, hresults_class):
		# generate and write the header file text
		self.WriteFileIfDifferent(self.ComputeFilePath(hresults_class['filename']), """using System;\nnamespace Solimar\n{\n\tnamespace Licensing\n\t{\n%s\n\t}\n}\n""" % (IndentTextTabs(self.GenerateHResultsClass(hresults_class),2),))
		
