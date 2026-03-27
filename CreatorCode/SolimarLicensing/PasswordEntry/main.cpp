#define _WIN32_DCOM
#define _WIN32_WINNT 0x0400

#define UNICODE
#define _UNICODE

#include <windows.h>
#include <comutil.h>
#include <tchar.h>
#include <stdio.h>
#include "..\SolimarLicenseServer\_SolimarLicenseServer.h"

int wmain(int argc, wchar_t** argv)
{
	CoInitializeEx(0, COINIT_MULTITHREADED);
	HRESULT hr = S_OK;
	
	ISolimarLicenseSvr *pServer=0;
	
	hr = CoCreateInstance(__uuidof(CSolimarLicenseSvr), NULL, CLSCTX_LOCAL_SERVER, __uuidof(ISolimarLicenseSvr), (void**)&pServer);
	
	while(1)
	{
		wchar_t password_buffer[1024];
		wscanf(L"%s",password_buffer);
		if (wcslen(password_buffer)>1 && 0!=wcsicmp(password_buffer,L"quit") && 0!=wcsicmp(password_buffer,L"exit"))
		{
			hr = pServer->EnterPassword(_bstr_t(password_buffer));			//L"129C0F1F-2150"
			if (FAILED(hr) || hr == S_FALSE)
			{
				wprintf(L"Error: %08x\r\n",hr);
			}
			else
			{
				wprintf(L"Success.\r\n");
			}
		}
		else
		{
			break;
		}
	}

	if (pServer)
		pServer->Release();
	
	CoUninitialize();
	return 0;
}