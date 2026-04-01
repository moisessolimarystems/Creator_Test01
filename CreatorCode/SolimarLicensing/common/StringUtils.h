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
			size_t characters;
			#if _MSC_VER >= 1400	
				mbstowcs_s(&characters, 0, 0, s.c_str(), 256);
			#else
				characters = mbstowcs(0, s.c_str(), 256);
			#endif

			wchar_t* ws = new wchar_t[characters+1];
			ws[0]=0;
			
			#if _MSC_VER >= 1400	
				size_t tmpValue;
				mbstowcs_s(	&tmpValue,
							ws, 
							characters,	//size in words
							s.c_str(),
							characters);
			#else
				mbstowcs(ws,s.c_str(),characters);
			#endif
			ws[characters]=0;
			std::wstring ret = ws;
			delete [] ws;
			return ret;
		}

		static std::string WstringToString(const std::wstring &ws)
		{
			size_t characters;
			#if _MSC_VER >= 1400	
				wcstombs_s(&characters, 0, 0, ws.c_str(), 256);
			#else
				characters = wcstombs(0, ws.c_str(), 256);
			#endif

			char* s = new char[characters+1];
			s[0]=0;
			
			#if _MSC_VER >= 1400	
				size_t tmpValue;
				wcstombs_s(	&tmpValue,
							s, 
							characters,	//size in words
							ws.c_str(),
							characters);
			#else
				wcstombs(s,ws.c_str(),characters);
			#endif
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
			return pStr;
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
