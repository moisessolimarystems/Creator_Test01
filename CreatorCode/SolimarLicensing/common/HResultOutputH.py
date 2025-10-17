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
                        class_text += '\tconst unsigned long %s = %s;\n' % (offset['name'],offset['value'])
                    else:
			class_text += '\tconst unsigned long %s = %s+%s;\n' % (offset['name'],offset['base'],offset['value'])

                class_text += '\n#ifndef SP_ACCESS_DENIED\t//RAINBOW API Error Codes\n'

		# Rainbow Error Codes
		for rnbo in OutputOrderDictionaryValues(hresults_class['rnbos']):
			if(rnbo['name'] != 'SP_INVALID_STATUS'):
	                        class_text += '\tconst unsigned long %s = %s;\n' % (rnbo['name'],rnbo['value'])

                class_text += '#endif\n'

		class_text += '\n'
                class_text += '\t//07-01-11 - JWL\n'
		class_text += '\t//Note: This is a rainbow error that is not defined in current 3rd party .h file.\n'
		class_text += '\t//If receive a new .h file and receive an error because it is already defined, move\n'
		class_text += '\t//this above.\n'
		class_text += '\tconst unsigned long SP_INVALID_STATUS = 255;\n'


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
                class_text += '\t\tconst wchar_t*\t\tmessage;\n'  #CR.33570.V01; modified.
                class_text += '\t} SL_ERROR;\n\n'

                class_text += '\t/*\n'
                class_text += '\t * GetErrorMessage should be used by programs to translate error HRESULTS\n'
                class_text += '\t */\n'
                class_text += '\tstd::wstring GetErrorMessage(HRESULT hr);\n'
                class_text += '\tstd::wstring GetECMessage(unsigned long echr);\n'
                class_text += '\n'
                class_text += '\t/*\n'
                class_text += '\t * GenerateErrorMessage should be used by programs to translate error HRESULTS\n'
                class_text += '\t */\n'
                class_text += '\tstd::wstring GenerateErrorMessage(HRESULT hr, std::wstring wstrHeaderMsg=L"", bool bAppendExistingErrorInfo=true);\n'
                class_text += '\n'

                class_text += '\t/*\n'
                class_text += '\t * SAVE_ERRORINFO, RESTORE_ERRORINFO\n'
                class_text += '\t * Helper macros to save and restore IErrorInfo objects in return blocks.\n'
                class_text += '\t * For instance, if a COM object is released between a COM error occurring, \n'
                class_text += '\t * and returning an errored hr from a function, the IErrorInfo object may \n'
                class_text += '\t * be cleared (similar to the lasterror for Win32 calls). Use these\n'
                class_text += '\t * macros to help store the IErrorInfo during COM cleanup, and then\n'
                class_text += '\t *restore IErrorInfo before returning.\n'
                class_text += '\t */\n'
                #define SAVE_ERRORINFO
                class_text += '#define SAVE_ERRORINFO \\\n'
                class_text += '\tIErrorInfo* save_error_info = NULL; \\\n'
                class_text += '\tGetErrorInfo(0, &save_error_info); \\\n'
                class_text += '\tif (save_error_info) \\\n'
                class_text += '\t\tSetErrorInfo(0, save_error_info);\n'
                #define RESTORE_ERRORINFO
                class_text += '#define RESTORE_ERRORINFO \\\n'
                class_text += '\tif (save_error_info) \\\n'
                class_text += '\t{ \\\n'
                class_text += '\t\tSetErrorInfo(0, save_error_info); \\\n'
                class_text += '\t\tsave_error_info->Release(); \\\n'
                class_text += '\t}\n'
                class_text += '\n'

                class_text += '\tHRESULT WriteEventLog(const wchar_t *event_log_msg, unsigned int event_type, long event_id);\n' #CR.33570.V01; modified.
                class_text += '\tHRESULT WriteEventLog(const wchar_t *event_log_msg, unsigned int event_type, long event_id, long product_id);\n'  #CR.33570.V01; modified.
                class_text += '\tHRESULT TReadEventLog();\n'
                class_text += '\n'

                class_text += '\t/*\n'
                class_text += '\t * LIC_PROPAGATE_CUSTOM_ERROR()\n'
                class_text += '\t *\n'
                class_text += '\t * Pulls the error message from a given HR, and sets the\n'
                class_text += '\t * calling objects ISupportErrorInfo with the information.\n'
                class_text += '\t * This macro can be used to easily propagate error info\n'
                class_text += '\t * from object to object within Licensing.\n'
                class_text += '\t */\n'
                #define LIC_PROPAGATE_CUSTOM_ERROR(hr, clsid, iid)
                class_text += '#define LIC_PROPAGATE_CUSTOM_ERROR(hr, clsid, iid) \\\n'
                class_text += '\tif(FAILED(hr)) \\\n'
                class_text += '\t\tAtlReportError((clsid), (BSTR)(LicenseServerError::GetErrorMessage(hr).c_str()), (iid), (hr));\n'
                class_text += '\n'
                #define LIC_PROPAGATE_CUSTOM_ERROR_MESSAGE(hr, wstrMsg, clsid, iid)
                class_text += '#define LIC_PROPAGATE_CUSTOM_ERROR_MESSAGE(hr, wstrMsg, clsid, iid) \\\n'
                class_text += '\tif(FAILED(hr)) \\\n'
                class_text += '\t\tAtlReportError((clsid), (BSTR)(wstrMsg.c_str()), (iid), (hr));\n'

                #define SL_IS_LIC_HR (hr)
                class_text += '\n\n\t/*\n\t* SL_IS_LIC_HR()\n\t*\n\t* Determine if hr is a Lic HR\n\t*/\n'
                class_text += '\n\t#define SL_IS_LIC_HR(hr) \\\n'
                class_text += '\t\t(((hr) & 0x0000F000) == ITF_LIC_MIN)\n'

                #define SL_EC_FROM_EHR (hr)
                class_text += '\t/*\n\t* SL_EC_FROM_EHR()\n\t*\n\t* Extracts the error code portion from an HRESULT.\n\t*/\n'
                class_text += '\t#define SL_EC_FROM_EHR(hr) \\\n'
                class_text += '\t\t((hr) & 0x0000FFFF)\n'

                #close out class declaration
                class_text += '};\n'
                
		return class_text		
	
	def ComputeFilePath(self, filename_base):
		return self.ComputeFileName(filename_base)
	def ComputeFileName(self, filename_base):
		return filename_base + '.h'
	
	def GenerateGenericHResultsFile(self, hresults_class):#include <string>\n\n
		# generate and write the header file text
		self.WriteFileIfDifferent(self.ComputeFilePath(hresults_class['filename']),     \
                                          """// File auto generated by HRESULTClassesCompiler.py\n#pragma once\n\n#include <windows.h>\n#include <string>\nnamespace LicenseServerError\n%s\n""" %  \
                                          (self.GenerateHResultsClass(hresults_class)))
			

		

	


