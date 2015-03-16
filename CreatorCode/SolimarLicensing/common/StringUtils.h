#pragma once

#include <string>
#include <sstream>
//#include <wstring>

class StringUtils
{
	public:
		//static std::wstring StringToWstring(const std::string &s);
		//static std::string WstringToString(const std::wstring &ws);

		// Returns true if any multi-byte characters, such as foreign language characters
		static bool DetectMultiByteCharacters(const std::string &s)
		{
			return (s.compare(WstringToString(StringToWstring(s))) != 0);
		}
		static bool DetectMultiByteCharacters(const std::wstring &ws)
		{
			return (ws.compare(StringToWstring(WstringToString(ws))) != 0);
		}

		static std::wstring StringToWstring(const std::string &s)
		{
			// Only works for ASCII chars, can't use.
			//return std::wstring(s.begin(), s.end());

			std::wstringstream wsStream;
			wsStream << s.c_str();
			return wsStream.str();
		}

		static std::string WstringToString(const std::wstring &ws/*, long* pErrorNo = 0*/)
		{
			// This will not convert MultiByte characters correctly
			return std::string(ws.begin(), ws.end());
		}

		static std::string ConvertUTF16ToUTF8(std::wstring utf16)
		{
			std::string utf8 = "";
			if (utf16.length() > 0)
			{
				int utf16Len = utf16.length();
				int utf8Len = WideCharToMultiByte(CP_UTF8, 0, utf16.c_str(), utf16Len, NULL, 0, NULL, NULL);
				char* pData = new char[utf8Len + 1];
				WideCharToMultiByte(CP_UTF8, 0, utf16.c_str(), utf16Len, pData, utf8Len, 0, 0);
				pData[utf8Len] = '\0';
				utf8 = pData;
				delete[] pData;
			}
			return utf8;
		}

		static std::string WstringToStringErrorOnBinary(const std::wstring &ws, long* pErrorNo = 0)
		{
			size_t characters;
			long errorNo = 0;
			#if _MSC_VER >= 1400	
				errorNo = wcstombs_s(&characters, 0, 0, ws.c_str(), 256);
			#else
				characters = wcstombs(0, ws.c_str(), 256);
			#endif

			char* s = new char[characters+1];
			s[0]=0;
			
			#if _MSC_VER >= 1400	
				size_t tmpValue;
				errorNo = wcstombs_s(	&tmpValue,
							s, 
							characters,	//size in words
							ws.c_str(),
							characters);
			#else
				wcstombs(s,ws.c_str(),characters);
			#endif

			if(pErrorNo != 0)
				*pErrorNo = errorNo;
			s[characters]=0;
			std::string ret = s;
			delete [] s;
			return ret;
		}
		
		static _bstr_t BinaryToString(BYTE *pData, DWORD length)
		{
			size_t sizeOfStr = (length*2)+1;
			wchar_t *pStr = new wchar_t[sizeOfStr];
			memset(pStr,0,sizeOfStr);
			wchar_t wTmpBuf[3];
			for (unsigned int b = 0; b < length; ++b)
			{
				_snwprintf_s(wTmpBuf, _countof(wTmpBuf), L"%02x", (unsigned int)pData[b]);
				wTmpBuf[_countof(wTmpBuf)-1] = 0;
				wcscat_s(pStr, sizeOfStr, wTmpBuf);

				//this line caused memory corruption that would be displayed on subsquint calls.
				//swprintf_s(pStr+2*b, sizeOfStr,L"%02x",(unsigned int)pData[b]);
			}
			//return pStr;
			_bstr_t retVal = pStr;
			delete[] pStr;
			return retVal;
		}
		static DWORD StringToBinaryLength(_bstr_t str)
		{
			return str.length()/2;
		}

static void StringToBinary(_bstr_t str, BYTE *pData, DWORD length)
{
	if (length>str.length()/2)
		return;
	memset(pData, 0, length);
	for (unsigned int b = 0; b < length; ++b)
	{
		unsigned int i(0);
		swscanf_s(((wchar_t*)str)+2*b, L"%02x", &i);
		pData[b] = (BYTE)i;
	}
}



};
