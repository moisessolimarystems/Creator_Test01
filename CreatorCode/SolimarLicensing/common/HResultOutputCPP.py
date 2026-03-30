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
class HResultOutputCPP(AttribsOutput):
	def __init__(self):
		AttribsOutput.__init__(self)
	
	def ProcessXML(self, class_list):
		# for each attribs class
		for hresult_class in class_list.values():   
			self.GenerateGenericHResultsFile(hresult_class)
    

        def GenerateHResultsClass(self, hresults_class):

                class_text = 'const SL_ERROR SLErrors[] =\n'
                class_text += '{\n'
                class_text += '\t//----------------------------------------------------------------------------------------------\n'
                class_text += '\t// Error Code\t\t\t\t\t\tHRESULT\t\t\t\t\t\tMessage\n'
                class_text += '\t//----------------------------------------------------------------------------------------------\n'
                class_text += '\t\n\t// key server errors'
                
                #Custom HRESULTS
                for ordered_hresults in OutputOrderDictionaryValues(hresults_class['hresults']):
                    i=0
                    class_text += '\n'
                    for hresult_group in ordered_hresults.values():
                        if(i < len(ordered_hresults)-1):
        		    class_text += "\t{%s,\t\t\t\t\t\t%s,\t\t\t\t\t\tL\"%s\"),\n" % (hresult_group['ecode'], hresult_group['name'],hresult_group['alias'])
                            i+=1

                class_text += '};\n'
                class_text += '\nconst unsigned long SL_ERROR_COUNT = sizeof(SLErrors) / sizeof(SL_ERROR);\n\n\n'

                #function: HRESULT WriteEventLog
                class_text += 'HRESULT WriteEventLog(wchar_t *event_log_msg, unsigned int event_type)\n'
                class_text += '{\n'
                class_text += '\tHRESULT hr = S_OK;\n'
                class_text += '\tHANDLE h;\n\n'
                class_text += '\th = RegisterEventSource(NULL, TEXT("Solimar License Server"));\n'
                class_text += '\tif (h == NULL) return HRESULT_FROM_WIN32(GetLastError());\n\n'
                class_text += '\tif (!ReportEventW(h,           // event log handle\n'
                class_text += '\t\t\t(WORD)(event_type),   // event type\n'
                class_text += '\t\t\t0,                    // category zero\n'
                class_text += '\t\t\t0,			   // event identifier\n'
                class_text += '\t\t\tNULL,                 // no user security identifier\n'
                class_text += '\t\t\t1,                    // one substitution string\n'
                class_text += '\t\t\t0,                    // no data\n'
                class_text += '\t\t\t{LPCWSTR*)&event_log_msg,       // pointer to string array\n'
                class_text += '\t\t\tNULL))                // pointer to data\n'
                class_text += '\t\thr = HRESULT_FROM_WIN32(GetLastError());\n\n'
                class_text += '\tDeregisterEventSource(h);\n\n'
                class_text += '\treturn hr;\n}\n'

                #function: HRESULT WriteEventLog                
                class_text += 'std::wstring GetErrorMessage(HRESULT hr)\n{\n'
                class_text += '\twchar_t hrErrMsg[256];\n'
                class_text += '\tswprintf(hrErrMsg, L" [0x%08X]", hr);\n'
                class_text += '\tstd::wstring retVal(L"Unknown Error");\n\n'
                class_text += '\tif(SL_IS_LIC_HR(hr))\n\t{\n'
                class_text += '\t\t//Detect a Solimar License error\n\t\tretVal = GetECMessage(SL_EC_FROM_EHR(hr));\n\t}\n'
                class_text += '\telse if((hr & 0x80070000) == 0x80070000)  //that location is where win32 errors set their value\n\t{\n'
                class_text += '\t\t//Detect if it is a Win32 error\n\t\tHRESULT tempHr(hr & 0x0000FFFF); //Convert, take the lower 4\n\n'
                class_text += '\t\tLPVOID lpMsgBuf;\n'
                class_text += '\t\tFormatMessageW(\n'
                class_text += '\t\t\tFORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,\n'
                class_text += '\t\t\tNULL,\n'
                class_text += '\t\t\ttempHr,\n'
                class_text += '\t\t\tMAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language\n'
                class_text += '\t\t\t(wchar_t*)&lpMsgBuf,\n'
                class_text += '\t\t\t0,\n'
                class_text += '\t\t\tNULL);\n'
                class_text += '\t\tstd::wstring tempMsg = (wchar_t*)lpMsgBuf;\n'
                class_text += '\t\tsize_t index(tempMsg.length());\n'
                class_text += '\t\twhile(tempMsg[index]==L\'\\n\' || tempMsg[index]==L\'\\r\')\n\t\t{\n'
                class_text += '\t\t\ttempMsg[index] = L\'\\0\';\n'
                class_text += '\t\t\tindex--;\n\t\t}\n'
                class_text += '\t\tretVal = std::wstring(tempMsg.c_str());\n\n'
                class_text += '\t\t// Free the buffer.\n\t\tLocalFree(lpMsgBuf);\n\t}\n'
                class_text += '\telse\n\t{\n'
                class_text += '\t\tIErrorInfo* pIErrorInfo = NULL;\n'
                class_text += '\t\tGetErrorInfo(0, &pIErrorInfo);\n'
                class_text += '\t\tif (pIErrorInfo)\n\t\t{\n'
                class_text += '\t\t\tBSTR des;\n'
                class_text += '\t\t\tif (SUCCEEDED(pIErrorInfo->GetDescription(&des)))\n\t\t\t{\n'
                class_text += '\t\t\t\tretVal = std::wstring(des);\n'
                class_text += '\t\t\t\tSysFreeString(des);\n\t\t\t}\n'
                class_text += '\t\t\tpIErrorInfo->Release();\n\t\t}\n\t}\n'
                class_text += '\t//TRACE(AnsiString(retVal + AnsiString(hrErrMsg)).c_str());\n'
                class_text += '\treturn retVal + std::wstring(hrErrMsg);\n}\n'

                #function GetECMessage                
                class_text += '/*\n *\n * GetECMessage()\n *\n * Looks up a custom Solimar License Error message given the HR error code.\n */\n'
                class_text += 'std::wstring GetECMessage(unsigned long echr)\n{\n'
                class_text += '\tunsigned long ec = echr & 0x0000FFFF;\n'
                class_text += '\tfor (int i=0; i<SL_ERROR_COUNT; i++) {\n'
                class_text += '\t\tif (SLErrors[i].EC == ec)\n'
                class_text += '\t\t\treturn std::wstring(SLErrors[i].message);\n\t}\n'
                class_text += '\treturn std::wstring(SLErrors[i].message);\n}\n'

                #close class
                class_text += '};'
                                
		return class_text		
	
	def ComputeFilePath(self, filename_base):
		return self.ComputeFileName(filename_base)
	def ComputeFileName(self, filename_base):
		return filename_base + '.cpp'
	
	def GenerateGenericHResultsFile(self, hresults_class):
		# generate and write the header file text
		self.WriteFileIfDifferent(self.ComputeFilePath(hresults_class['filename']),     \
                                          """#include "LicenseError.h"\nnamespace LicenseServerError\n{\n/*\n * SLErrors[]\n *\n * custom errors.\n */\n%s\n""" %  \
                                          (self.GenerateHResultsClass(hresults_class)))		
