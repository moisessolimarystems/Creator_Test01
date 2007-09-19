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

# outputs the c++ version of the hresults class header files
class HResultOutputH(AttribsOutput):
	def __init__(self):
		AttribsOutput.__init__(self)
	
	def ProcessXML(self, class_list):
		# for each attribs class
		for hresult_class in class_list.values():   
			self.GenerateGenericHResultsFile(hresult_class)
    

        def GenerateHResultsClass(self, hresults_class):
		# class declaration header

		class_text =  '{\n'
		class_text += '\t/* Custom Error Base   HR Error Codes should all offset from this base. */\n'
		
		# Offset defines
		for offset in OutputOrderDictionaryValues(hresults_class['offsets']):
                    if(offset['base'] == 'HRESULT_BASE' or offset['base'] == ''):    #no hresult base for cpp file
                        class_text += '\tconst unsigned long %s = %s\n' % (offset['name'],offset['value'])
                    else:
			class_text += '\tconst unsigned long %s = %s+%s\n' % (offset['name'],offset['base'],offset['value'])

                class_text += '\n#ifndef SP_ACCESS_DENIED\t//RAINBOW API Error Codes\n'

		# Rainbow Error Codes
		for rnbo in OutputOrderDictionaryValues(hresults_class['rnbos']):  
                        class_text += '\tconst unsigned long %s = %s\n' % (rnbo['name'],rnbo['value'])

                class_text += '#endif\n'

                #Custom HRESULTS
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
        		    class_text += "\tconst unsigned long %s = %s+%s;\n" % (hresult_group['ecode'], hresult_group['offset'],hresult_group['value'])
        		    i+=1                  

                #HRESULTS defines
                class_text += '\n\t/* HRESULTS defined from custom error codes above. */'
                for ordered_hresults in OutputOrderDictionaryValues(hresults_class['hresults']):
                    i=0
                    class_text += '\n'
                    for hresult_group in ordered_hresults.values():
                        if(i < len(ordered_hresults)-1):
        		    class_text += "\tconst HRESULT %s = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, %s);\n" % (hresult_group['name'], hresult_group['ecode'])
                            i+=1

                #struct _SL_ERROR                                
                class_text += '\n\t/*\n\t * SL_ERROR - Solimar License Error\n\t*/\n'
                class_text += '\ttypedef struct _SL_ERROR {\n'
                class_text += '\t\tunsigned long\tEC;\n'
                class_text += '\t\tHRESULT\t\tHR;\n'
                class_text += '\t\twchar_t*\t\tmessage;\n'
                class_text += '\t} SL_ERROR;\n\n'

                class_text += '\tstd::wstring GetErrorMessage(HRESULT hr);\n'
                class_text += '\tstd::wstring GetECMessage(unsigned long echr);\n'
                class_text += '\tHRESULT WriteEventLog(wchar_t *event_log_msg, unsigned int event_type);\n'

                #define SL_IS_LIC_HR (hr)
                class_text += '\n\n\t/*\n\t* SL_IS_LIC_HR()\n\t*\n\t* Determine if hr is a Lic HR\n\t*/\n'
                class_text += '\n\t#define SL_IS_LIC_HR(hr) \\\n'
                class_text += '\t\t((hr) & ITF_CR_MIN)\n'

                #define SL_EC_FROM_EHR (hr)
                class_text += '\t/*\n\t* SL_EC_FROM_EHR()\n\t*\n\t* Extracts the error code portion from an HRESULT.\n\t*/\n'
                class_text += '\t#define SL_EC_FROM_EHR(hr) \\\n'
                class_text += '\t\t((hr) & 0x0000FFFF)\n'

                #close out class declaration
                class_text += '};\n'
                class_text += '#endif'
                
		return class_text		
	
	def ComputeFilePath(self, filename_base):
		return self.ComputeFileName(filename_base)
	def ComputeFileName(self, filename_base):
		return filename_base + '.h'
	
	def GenerateGenericHResultsFile(self, hresults_class):#include <string>\n\n
		# generate and write the header file text
		self.WriteFileIfDifferent(self.ComputeFilePath(hresults_class['filename']),     \
                                          """#ifndef _LicenseError_h_9515ABE0E22648899A90CF0001BBBB05_\n#define _LicenseError_h_9515ABE0E22648899A90CF0001BBBB05_\n\n#include <windows.h>\n#include <string>\n\nnamespace LicenseServerError\n%s\n""" %  \
                                          (self.GenerateHResultsClass(hresults_class)))
			

		

	


