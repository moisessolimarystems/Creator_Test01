
#define _WIN32_DCOM
#define _WIN32_WINNT 0x0500
#define SECURITY_WIN32

#include <windows.h>
#include <security.h>
#include <objbase.h>
#include <comdef.h>
#include <comutil.h>
#include <map>
#include "c:\sandbox\solsearcher\current\solsearchercode\3prty\rainbow\winnt\spromeps.h"

static const unsigned short int DEVELOPER_ID = (unsigned short int)0xFEA8;

const unsigned short CELL_KEY_NUMBER = (unsigned short)0x3F;
const unsigned short CELL_CUSTOMER_NUMBER = (unsigned short)0x3E;


WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	HRESULT hr=S_OK;
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

for (int jerk = 0; jerk<10; ++jerk)
{
	typedef std::map<_bstr_t,RBP_SPRO_APIPACKET> KeyList;
	KeyList newkeys;

	
	RBP_SPRO_APIPACKET packet = new RB_SPRO_APIPACKET;

	if (!packet)
		hr = E_FAIL;
	
	if (SUCCEEDED(hr) && RNBOsproFormatPacket(packet, sizeof(RB_SPRO_APIPACKET)))
		hr = E_FAIL;
	
	if (SUCCEEDED(hr) && RNBOsproInitialize(packet))
		hr = E_FAIL;
    
	bool key_found = (bool)SUCCEEDED(hr);
	int key_count = 0;
	
	// count the keys
	for (key_found = !RNBOsproFindFirstUnit(packet, DEVELOPER_ID); 
			key_found; 
			key_found = !RNBOsproFindNextUnit(packet))
	{
		++key_count;
	}
	
	if (packet)
	{
		//RNBOsproReleaseLicense(packet, 0, NULL);
		delete packet;
	}

	// run through the list of keys backwards (see note above) storing each key's packet
	for (int i = key_count - 1; i >= 0; i--)
	{
		RBP_SPRO_APIPACKET keypacket = new RB_SPRO_APIPACKET;
		if (!keypacket)
			hr = E_FAIL;
		
		if (SUCCEEDED(hr) && RNBOsproFormatPacket(keypacket, sizeof(RB_SPRO_APIPACKET)))
			hr = E_FAIL;
		
		if (SUCCEEDED(hr) && RNBOsproInitialize(keypacket))
			hr = E_FAIL;
		
		// iterate to the ith key
		int j;
		for (j = 0, key_found = !RNBOsproFindFirstUnit(keypacket, DEVELOPER_ID);
			j < i && SUCCEEDED(hr);
			j++, key_found = !RNBOsproFindNextUnit(keypacket));
		
		// if the key packet was obtained
		if (SUCCEEDED(hr) && key_found)
		{
			unsigned short customer, keynumber;
			if (0==RNBOsproRead(keypacket, CELL_CUSTOMER_NUMBER, &customer) && 0==RNBOsproRead(keypacket, CELL_KEY_NUMBER, &keynumber))
			{
				wchar_t key_id[128];
				_snwprintf(key_id, 128, L"%x-%x", customer, keynumber);
				key_id[127]=0;
				newkeys[key_id] = keypacket;
				keypacket = 0;
			}
		}
		
		// cleanup
		if (keypacket)
		{
			//RNBOsproReleaseLicense(packet, 0, NULL);
			delete keypacket;
		}
	}

	for (KeyList::iterator j = newkeys.begin(); j!=newkeys.end(); ++j)
	{
		delete j->second;
	}
	

}
	
	
if (0)
{
	RBP_SPRO_APIPACKET pPacket = new RB_SPRO_APIPACKET;

	if (SUCCEEDED(hr) && RNBOsproFormatPacket(pPacket, sizeof(RB_SPRO_APIPACKET)))
		hr = E_FAIL;
	
	if (SUCCEEDED(hr) && RNBOsproInitialize(pPacket))
		hr = E_FAIL;
	
	// to contact a remote key
	// RNBOsproSetContactServer(p_packet, (char*)server_name))

	// enumerate the keys
	bool key_found = (bool)SUCCEEDED(hr);
	unsigned int key_count = 0;
	for (key_found = !RNBOsproFindFirstUnit(pPacket, DEVELOPER_ID); 
			key_found; 
			key_found = !RNBOsproFindNextUnit(pPacket))
	{
		++key_count;

		for (unsigned int i=0; i<key_count; ++i)
		{
			unsigned short int key_data_buffer[8*1024];
			unsigned short int key_data_read[8*1024];

			memset(key_data_buffer, 0, sizeof(key_data_buffer));
			memset(key_data_read, 0, sizeof(key_data_read));
			for (unsigned int address=0; address<512; ++address)
			{
				key_data_read[address] = RNBOsproRead(pPacket, address, key_data_buffer+address);
			}
		}
	}
	
	
	RNBOsproReleaseLicense(pPacket, 0, NULL);

	delete [] pPacket;
}
		
	CoUninitialize();
	return 0;
}
