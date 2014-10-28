#pragma once

#include <string>
//#include <wstring>

class StringUtils
{
	public:
		//static std::wstring StringToWstring(const std::string &s);
		//static std::string WstringToString(const std::wstring &ws);

		static std::wstring StringToWstring(const std::string &s)
		{
			return std::wstring(s.begin(), s.end());
		}

		static std::string WstringToString(const std::wstring &ws/*, long* pErrorNo = 0*/)
		{
			return std::string(ws.begin(), ws.end());
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
