#include "NetworkUtils.h"
//#include <string>
//#include <stdio.h>
//#include <tchar.h>

HRESULT NetworkUtils::SolimarNetworkHelper::GetIPAddresses(_bstr_t computerName, std::vector<_bstr_t> &ipList)
{
	HRESULT hr = S_OK;

	ipList.clear();
	
	WSADATA wsaData;
	wchar_t ip_addr_buffer[32];
	if (!WSAStartup(MAKEWORD(2,0),&wsaData))
	{
		hostent *pHostInfo = gethostbyname((char*)computerName);
		BYTE *ip;
		if (pHostInfo)
		{
			unsigned int addr_index = 0;
			while(1)
			{
				// if the ip pointer is the null, break out
				ip = (BYTE*)(pHostInfo->h_addr_list[addr_index]);
				if (ip==0)
					break;
				
				_bstr_t address;
				for (int i=0; i<pHostInfo->h_length; ++i)
				{
					swprintf_s(ip_addr_buffer, (i+1!=pHostInfo->h_length) ? L"%d." : L"%d", (unsigned int)(unsigned char)ip[i]);
					address += ip_addr_buffer;
				}
				ipList.push_back(address);

				++addr_index;
			}
		}
		else
		{
			hr = HRESULT_FROM_WIN32(WSAGetLastError());
		}

		WSACleanup();
	}

	return hr;
}