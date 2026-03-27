#define _WIN32_DCOM
#define _WIN32_WINNT 0x0400

#define UNICODE
#define _UNICODE

#include <windows.h>
#include <comutil.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include "..\SolimarLicenseServer\_SolimarLicenseServer.h"
#include "..\SolimarLicenseManager\_SolimarLicenseManager.h"
#include "..\SolimarLicenseServer\KeySpec.h"


int wmain(int argc, wchar_t** argv)
{
	CoInitializeEx(0, COINIT_MULTITHREADED);
	HRESULT hr = S_OK;
	
	KeySpec keyspec;
	ISolimarLicenseSvr *pServer=0;
	
	hr = CoCreateInstance(__uuidof(CSolimarLicenseSvr), NULL, CLSCTX_LOCAL_SERVER, __uuidof(ISolimarLicenseSvr), (void**)&pServer);
	
	while(1)
	{
		// get the key list
		int num_keys = 0;
		VARIANT vtKeys;
		hr = pServer->KeyEnumerate(&vtKeys);
		
		// clear the screen
		system("cls");
		
		// print the column headers
		wprintf(L"Key\t\tIndex\tReport\tUsers\tApps\tValid\t\r\n");
		
		// for each key
		num_keys = vtKeys.parray->rgsabound[0].cElements;
		for (int i=0; i<num_keys; ++i)
		{
			_bstr_t key_ident = ((VARIANT*)vtKeys.parray->pvData)[i].bstrVal;
			
			VARIANT vtKeyNum, vtCustNum;
			hr = pServer->KeyHeaderQuery(key_ident, keyspec.headers[L"Key Number"].id, &vtKeyNum);
			hr = pServer->KeyHeaderQuery(key_ident, keyspec.headers[L"Customer Number"].id, &vtCustNum);
			VARIANT_BOOL license_valid;
			hr = pServer->KeyIsPresent(key_ident, &license_valid);
			
			wprintf(L"%s\t\t%d\r\n",(wchar_t*)key_ident,license_valid);
		}
		wprintf(L"%d\r\n",GetTickCount());
		
		VariantClear(&vtKeys);
		
		Sleep(1*1000);
	}
	
	while(1)
	{
		
		// display the licenses (Currently only grabs solsearcher licenses)
		
		wprintf(L"hey\r\n");
		
		Sleep(1000);
	}
	
	if (pServer)
		pServer->Release();

	CoUninitialize();
	return 0;
}
