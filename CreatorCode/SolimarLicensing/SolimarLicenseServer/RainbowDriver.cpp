#include "RainbowDriver.h"
#include "..\common\LicenseError.h"
#include "..\common\SafeMutex.h"
#include <objbase.h>


/* accessCode[]
 *    The accessCode array tells the access code of each cell of the
 *    protection key.  */
const RainbowDriver::AccessCode RainbowDriver::accessCode[] =
{
	// 0x00 -> 0x07   FIRST ROW OF CELLS (INACCESSIBLE)
	READONLY,  READONLY,  ALGORITHM, ALGORITHM, ALGORITHM, ALGORITHM, ALGORITHM, ALGORITHM,
	// 0x08 -> 0x0F   FIRST ROW OF ACCESSABLE CELLS
	READWRITE, READWRITE, READWRITE, READWRITE, READWRITE, READWRITE, READWRITE, READWRITE,
	// 0x10 -> 0x17   SECOND ROW OF ACCESSABLE CELLS
	READWRITE, READWRITE, READWRITE, READWRITE, READWRITE, READWRITE, READWRITE, READWRITE,
	// 0x18 -> 0x1F   THIRD ROW OF ACCESSABLE CELLS
	READWRITE, READWRITE, READWRITE, READWRITE, ALGORITHM, ALGORITHM, ALGORITHM, ALGORITHM,
	// 0x20 -> 0x27   FOURTH ROW OF ACCESSABLE CELLS
	READWRITE, READWRITE, READWRITE, READWRITE, READWRITE, READWRITE, READWRITE, READWRITE,
	// 0x28 -> 0x2F   FIFTH ROW OF ACCESSABLE CELLS
	READWRITE, READWRITE, ALGORITHM, ALGORITHM, READWRITE, READWRITE, READWRITE, READWRITE,
	// 0x30 -> 0x37   SIXTH ROW OF ACCESSABLE CELLS
	READWRITE, READWRITE, READWRITE, READWRITE, READWRITE, READWRITE, READWRITE, READWRITE,
	// 0x38 -> 0x3F   LAST ROW OF ACCESSABLE CELLS
	READWRITE, READWRITE, READWRITE, READWRITE, READWRITE, READWRITE, READONLY, READONLY,
};




RainbowDriver::RainbowDriver() : 
	keys_lock(CreateMutex(0, 0, 0)),
	bAtLeastOneParallelKey(true)
{
	;
}

RainbowDriver::~RainbowDriver()
{
	if (keys_lock!=INVALID_HANDLE_VALUE)
		CloseHandle(keys_lock);
}

HRESULT RainbowDriver::ComputeCurrentKeyIdent(_bstr_t key, BSTR *physical_key_ident)
{
	SafeMutex mutex(keys_lock);	
	
	*physical_key_ident = key.copy();
	
	KeyList::iterator k = keys.find(key);
	if (k!=keys.end())
	{
		unsigned short customer, keynumber;
		if (0==RNBOsproRead(k->second, RainbowDriver::CELL_CUSTOMER_NUMBER, &customer) && 0==RNBOsproRead(k->second, RainbowDriver::CELL_KEY_NUMBER, &keynumber))
		{
			wchar_t key_id[128];

			// if the key is programmed
			if (keynumber!=0 || customer!=0)
			{
				_snwprintf(key_id, 128, L"%03x-%02x", customer, keynumber);
				key_id[127]=0;
				*physical_key_ident = _bstr_t(key_id).Detach();
			}
			// if the key is not programmed
			else
			{
				GUID key_guid;
				if (SUCCEEDED(ReadKeyTempGUID(k->second, key_guid)))
				{
					StringFromGUID2(key_guid, key_id, 128);
					key_id[127]=0;
					*physical_key_ident = _bstr_t(key_id).Detach();
				}
			}
		}
		else
		{
			return E_FAIL;
		}
	}
	else
	{
		return E_INVALIDARG;
	}
	
	return S_OK;
}

HRESULT RainbowDriver::RefreshKeyList()
{
//OutputDebugStringW(L"RainbowDriver::RefreshKeyList() - Enter");
	// DCM 12.nov.02 - In version 6.3 of the SuperPro libraries, Rainbow
	// changed the way that the FindFirst/FindNext functions work. My opinion
	// is that the new behavior is flawed, but Rainbow insists that this
	// "license based implementation" is by design. In 6.3, calling FindNext
	// on a packet removes the license acquire results from the previous FindXXX
	// call. With our previous code, we called FindFirst on a packet, copied
	// the packet to the first ProtectionKey's packet member, then called
	// FindNext on the packet, copied the packet to the second ProtectionKey's
	// packet member, etc. This no longer works with the 6.3 libraries, since
	// the first ProtectionKey's packet will have its license revoked when
	// FindNext is called for the second ProtectionKey. Also in 6.3, once
	// a key's license has been acquired by a packet, any FindFirst/FindNext
	// call for that key will fail. To get around these issues, we need to
	// jump through some hoops. First, we call FindFirst/FindNext with a
	// temporary packet to determine how many keys are cascaded. Then, for
	// each key, starting with the last key in the chain to avoid FindXXX
	// errors, we create a packet and call FindFirst/FindNext the appropriate
	// number of times to get to that key. While ugly, this is the method
	// recommended by Rainbow.

	HRESULT hr = S_OK;
	KeyList newkeys;
	RBP_SPRO_APIPACKET packet = new RB_SPRO_APIPACKET;
	bool bLocalAtLeastOneParallelKey = false;
	if (!packet)
		hr = E_FAIL;
	
	if (SUCCEEDED(hr))
		hr = TranslateRainbowError(RNBOsproFormatPacket(packet, sizeof(RB_SPRO_APIPACKET)));
	
	if (SUCCEEDED(hr))
		hr = TranslateRainbowError(RNBOsproInitialize(packet));
	if (SUCCEEDED(hr))	//Turn off chek for Terminal Services (Remote Desktop)
		hr = TranslateRainbowError(RNBOsproCheckTerminalservice(packet, SP_TERM_SERV_CHECK_OFF));
	if (SUCCEEDED(hr))
		hr = TranslateRainbowError(RNBOsproSetContactServer(packet, "RNBO_STANDALONE"));
		//hr = TranslateRainbowError(RNBOsproSetContactServer(packet, "RNBO_SPN_LOCAL"));

wchar_t tmpBuf[256];
//wsprintf(tmpBuf, L"RainbowDriver::RefreshKeyList() - RNBOsproSetContactServer() - hr = %x", hr);
//OutputDebugStringW(tmpBuf);

	bool key_found = (bool)SUCCEEDED(hr);
	int key_count = 0;
	
	// count the keys
	unsigned short retVal = RNBOsproFindFirstUnit(packet, DEVELOPER_ID);
	if(retVal != 0)
	{
//wsprintf(tmpBuf, L"RainbowDriver::RefreshKeyList() - 1st RNBOsproFindFirstUnit() - retVal = %d", retVal);
//OutputDebugStringW(tmpBuf);
	}

	while(retVal == 0)
	{
		++key_count;
		retVal = RNBOsproFindNextUnit(packet);
//wsprintf(tmpBuf, L"RainbowDriver::RefreshKeyList() - 1st RNBOsproFindNextUnit() - retVal = %d", retVal);
//OutputDebugStringW(tmpBuf);
	}
	
	if (packet)
	{
		delete packet;
	}
	// run through the list of keys backwards (see note above) storing each key's packet
	for (int i = key_count - 1; i >= 0; i--)
	{
		RBP_SPRO_APIPACKET keypacket = new RB_SPRO_APIPACKET;
		if (!keypacket)
			hr = E_FAIL;
		
		if (SUCCEEDED(hr))
			hr = TranslateRainbowError(RNBOsproFormatPacket(keypacket, sizeof(RB_SPRO_APIPACKET)));
		if (SUCCEEDED(hr))
			hr = TranslateRainbowError(RNBOsproInitialize(keypacket));

		if (SUCCEEDED(hr))	//Turn off chek for Terminal Services (Remote Desktop)
			hr = TranslateRainbowError(RNBOsproCheckTerminalservice(keypacket, SP_TERM_SERV_CHECK_OFF));
		
		if (SUCCEEDED(hr))
			TranslateRainbowError(RNBOsproSetContactServer(keypacket, "RNBO_STANDALONE"));
			//TranslateRainbowError(RNBOsproSetContactServer(keypacket, "RNBO_SPN_LOCAL"));
			
		// iterate to the ith key
		unsigned short retVal = RNBOsproFindFirstUnit(keypacket, DEVELOPER_ID);
		//wsprintf(tmpBuf, L"RainbowDriver::RefreshKeyList() - 2nd RNBOsproFindFirstUnit() - retVal = %d", retVal);
		//OutputDebugStringW(tmpBuf);
		int j = 0;
		while(retVal == 0 && j<i)
		{
			retVal = RNBOsproFindNextUnit(keypacket);
			//wsprintf(tmpBuf, L"RainbowDriver::RefreshKeyList() - 2nd RNBOsproFindNextUnit() - retVal = %d", retVal);
			//OutputDebugStringW(tmpBuf);
			key_found = retVal == 0;
			j++;
		}

		// if the key packet was obtained
		if (SUCCEEDED(hr) && key_found)
		{
			// create the key identifier and add the key to the list of keys

			unsigned short customer, keynumber;
			if (0==RNBOsproRead(keypacket, RainbowDriver::CELL_CUSTOMER_NUMBER, &customer) && 0==RNBOsproRead(keypacket, RainbowDriver::CELL_KEY_NUMBER, &keynumber))
			{
				bool valid_key_id = false;
				wchar_t key_id[128];

				// for making unique key identifiers, we write a guid to 
				// unprogrammed keys to serve as part of their key identifier
				// this guid is wiped out once the key is programmed, and then
				// the customer number and key number are used as the key identifier

				// if key is not programmed
				if (keynumber==0 && customer==0)
				{
					// check for the guid written to the key
					// this guid lives in cells CELL_KEY_GUID to CELL_KEY_GUID+7 inclusive
					GUID key_guid, zero_guid;
					memset(&zero_guid,0,sizeof(GUID));
					
					if (SUCCEEDED(ReadKeyTempGUID(keypacket, key_guid)))
					{
						HRESULT hr = S_OK;
						
						// if the key has no valid unique id, write one
						if (key_guid == zero_guid)
						{
							hr = CoCreateGuid(&key_guid);
							if (SUCCEEDED(hr))
								hr = WriteKeyTempGUID(keypacket, key_guid);
						}
						
						if (SUCCEEDED(hr))
						{
							StringFromGUID2(key_guid, key_id, 128);
							key_id[127]=0;
							valid_key_id = true;
						}
					}
				}
				// if key is programmed, use the key number and customer number as the key id
				else
				{
					_snwprintf(key_id, 128, L"%03x-%02x", customer, keynumber);
//wsprintf(tmpBuf, L"Key Found: %s", key_id);
//OutputDebugStringW(tmpBuf);
					key_id[127]=0;
					valid_key_id = true;
				}
				
				
				if(!bLocalAtLeastOneParallelKey)
				{
					// determine if key is usb or parallel
					unsigned short keyFamily;
					unsigned short keyFormFactor;
					unsigned short keyMemorySize;
					hr = TranslateRainbowError(RNBOsproGetKeyType(
									keypacket,
									&keyFamily,
									&keyFormFactor,
									&keyMemorySize));
					if(SUCCEEDED(hr))
					{
						/*
						* Key Family - The key family parameter will return 0 or 1, where 0 denotes the
						*		SuperPro keys (the SSP keys) and 1 denotes the UltraPro keys (the SUP keys).
						* Form Factor - The form factor parameter will return 0 or 1, where 0 denotes the
 						*		parallel keys and 1 denotes the USB keys.
						* Memory Size - The number of cells (inclusive of the reserved cells).
						*/
						bLocalAtLeastOneParallelKey = keyFormFactor == 0;
					}
				}

				if (valid_key_id)
				{
					newkeys[key_id] = keypacket;
					keypacket = 0;
				}
			}
		}
		// cleanup
		if (keypacket)
		{
			delete keypacket;
		}
	}
	
	
	// if we could enumerate the keys, swap out the key list
	if (SUCCEEDED(hr))
	{
		SafeMutex mutex(keys_lock);
		bAtLeastOneParallelKey = bLocalAtLeastOneParallelKey;
		// delete old key packets
		while(!keys.empty())
		{
			RNBOsproReleaseLicense(keys.begin()->second, 0, NULL);	//release previous license into key
			delete keys.begin()->second;
			keys.erase(keys.begin());
		}
		keys.insert(newkeys.begin(), newkeys.end());
		newkeys.clear();
	}
	else
	{
		// there was an error building the key list, delete any keys we obtained
		while(!newkeys.empty())
		{
			delete newkeys.begin()->second;
			newkeys.erase(newkeys.begin());
		}
	}
//OutputDebugStringW(L"RainbowDriver::RefreshKeyList() - Leave");
	return hr;
}

bool RainbowDriver::AtLeastOneParallelKey()
{
	SafeMutex mutex(keys_lock);
	return bAtLeastOneParallelKey;
}
HRESULT RainbowDriver::ReadCell(_bstr_t key, unsigned short cell, unsigned short *value)
{
	SafeMutex mutex(keys_lock);
	
	KeyList::iterator k = keys.find(key);
	if (k!=keys.end())
	{
		return TranslateRainbowError(RNBOsproRead(k->second, cell, value));
	}
	else
	{
		return E_INVALIDARG;
	}
}

HRESULT RainbowDriver::WriteCell(_bstr_t key, unsigned short cell, unsigned short value)
{
	SafeMutex mutex(keys_lock);	

	KeyList::iterator k = keys.find(key);
	if (k!=keys.end())
	{
		return TranslateRainbowError(RNBOsproWrite(k->second, WRITE_PASSWORD, cell, value, accessCode[cell]));
	}
	else
	{
		return E_INVALIDARG;
	}
}

HRESULT RainbowDriver::FormatCell(_bstr_t key, unsigned short cell)
{
	SafeMutex mutex(keys_lock);	

	KeyList::iterator k = keys.find(key);
	if (k!=keys.end())
	{
		return TranslateRainbowError(RNBOsproOverwrite(k->second, WRITE_PASSWORD, OVERWRITE_PASSWORD1, OVERWRITE_PASSWORD2, cell, 0, READWRITE));
	}
	else
	{
		return E_INVALIDARG;
	}
}

HRESULT RainbowDriver::Query(_bstr_t key, unsigned short cell, void* query, void* response, unsigned short length)
{
	SafeMutex mutex(keys_lock);
	
	KeyList::iterator k = keys.find(key);
	if (k!=keys.end())
	{
		RB_DWORD result_32;
		return TranslateRainbowError(RNBOsproQuery(k->second, cell, query, response, &result_32, length));
	}
	else
	{
		return E_INVALIDARG;
	}
}

HRESULT RainbowDriver::Activate(_bstr_t key, unsigned short cell)
{
	SafeMutex mutex(keys_lock);
	
	KeyList::iterator k = keys.find(key);
	if (k!=keys.end())
	{
		return TranslateRainbowError(RNBOsproActivate(k->second, WRITE_PASSWORD, ACTIVATE_PASSWORD1, ACTIVATE_PASSWORD2, cell));
	}
	else
	{
		return E_INVALIDARG;
	}
}

HRESULT RainbowDriver::WriteKeyUnprogrammedIdentifier(_bstr_t key)
{
	HRESULT hr = S_OK;

	SafeMutex mutex(keys_lock);
	
	KeyList::iterator k = keys.find(key);
	if (k!=keys.end())
	{
		// create and write a guid to the key
		// the guid lives in cells CELL_KEY_GUID to CELL_KEY_GUID+7 inclusive
		GUID key_guid;
		hr = CoCreateGuid(&key_guid);
		if (SUCCEEDED(hr)) hr = WriteKeyTempGUID(k->second, key_guid);
	}
	else
	{
		return E_INVALIDARG;
	}
	
	return hr;
}
HRESULT RainbowDriver::ClearKeyUnprogrammedIdentifier(_bstr_t key)
{
	HRESULT hr = S_OK;

	SafeMutex mutex(keys_lock);
	
	KeyList::iterator k = keys.find(key);
	if (k!=keys.end())
	{
		// the guid lives in cells CELL_KEY_GUID to CELL_KEY_GUID+7 inclusive
		hr = ClearKeyTempGUID(k->second);
	}
	else
	{
		return E_INVALIDARG;
	}
	
	return hr;
}


// Password specifications:
// Base passwords:			<password,16>				new: <password,16>-0-1-<customer_number,10>-<key_number,10>
// Extension passwords:		<password,16>				new: <password,16>-0-2-<customer_number,10>-<key_number,10>-<extend_days,10>-<extension_num,10>
// Version passwords:		<password,16>-<version,16>	new: <password,16>-<version,16>-3-<customer_number,10>-<key_number,10>
// Module passwords:		<password,16>-<units,10/16>	new: <password,16>-<units,10/16>-4-<customer_number,10>-<key_number,10>-<module.id,10>
// Module passwords:		<password,16>-<units,10/16>	new: <password,16>-<units,10/16>-4-<customer_number,10>-<key_number,10>-<module.id,10>-<password_number,10>

//HRESULT RainbowDriver::ParsePassword(_bstr_t password, unsigned int *pPassword, unsigned short *pArg1_decimal, unsigned short *pArg1_hex)
HRESULT RainbowDriver::ParsePassword(_bstr_t password, ArgumentList &arguments)
{
	// initialization
	memset(&arguments, 0, sizeof(arguments));
	arguments.legacy = true;
	arguments.password_number = 0;
	
	// copy the password in to an editable buffer for use with strtok
	unsigned int tokbuflen = min(password.length()+1,100);
	wchar_t *tokbuf = new wchar_t[tokbuflen];
	wcsncpy(tokbuf,(wchar_t*)password,tokbuflen-1);
	tokbuf[tokbuflen-1]=0;
	
	wchar_t* pass_txt = wcstok(tokbuf,L"- ");
	wchar_t* arg_txt = (pass_txt ? wcstok(NULL,L"- ") : 0);
	
	if (pass_txt)
	{
		swscanf(pass_txt, L"%x", &arguments.password);
	}
	if (arg_txt)
	{
		unsigned int arg1_decimal, arg1_hex;
		swscanf(arg_txt, L"%d", &arg1_decimal);
		swscanf(arg_txt, L"%x", &arg1_hex);
		arguments.units_licensed = arg1_decimal;
		arguments.pages_per_minute_struct = arg1_hex;
		arguments.version = (unsigned short)arg1_hex;
	}
	
	for (unsigned int extended_arg = 0; arg_txt = (arg_txt ? wcstok(NULL,L"- ") : 0); ++extended_arg)
	{
		arguments.legacy = false;
		
		switch(extended_arg)
		{
		// password type
		case 0:
			arguments.type=(unsigned int)_wtoi(arg_txt);
			break;
		// customer number
		case 1:
			swscanf(arg_txt, L"%hx", &arguments.customer);
			//arguments.customer=(unsigned short)_wtoi(arg_txt);
			break;
		case 2:
			swscanf(arg_txt, L"%hx", &arguments.key);
			//arguments.key=(unsigned short)_wtoi(arg_txt);
			break;
		case 3:
			switch (arguments.type)
			{
			// extension case
			case 2:
				arguments.extend_days=(unsigned short)_wtoi(arg_txt);
				break;
			// module case
			case 4:
				arguments.module=(unsigned int)_wtoi(arg_txt);
				break;
			}
			break;
		case 4:
			switch (arguments.type)
			{
			// extension case
			case 2:
				arguments.extend_num=(unsigned short)_wtoi(arg_txt);
				break;
			// module case
			case 4:
				arguments.password_number=(unsigned int)_wtoi(arg_txt);
				break;
			}
			break;
		default:
			break;
		}
	}

	delete [] tokbuf;
	
	if (!pass_txt)
		return E_FAIL;
	
	return S_OK;
}

HRESULT RainbowDriver::WriteKeyTempGUID(RBP_SPRO_APIPACKET packet, GUID &id)
{
	// write a guid to the key
	// this guid lives in cells CELL_KEY_GUID to CELL_KEY_GUID+7 inclusive
	for (unsigned int i = 0; i<sizeof(GUID)/sizeof(unsigned short); ++i)
	{
		unsigned short cell = CELL_KEY_GUID+i;
		unsigned short value = *(((unsigned short*)&id)+i);
		HRESULT hr = TranslateRainbowError(RNBOsproWrite(packet, WRITE_PASSWORD, cell, value, accessCode[cell]));
		if (FAILED(hr)) return hr;
	}
	return S_OK;
}

HRESULT RainbowDriver::ReadKeyTempGUID(RBP_SPRO_APIPACKET packet, GUID &id)
{
	// check for the guid written to the key
	// this guid lives in cells CELL_KEY_GUID to CELL_KEY_GUID+7 inclusive
	memset(&id,0,sizeof(GUID));
	for (unsigned int i = 0; i<sizeof(GUID)/sizeof(unsigned short); ++i)
	{
		HRESULT hr = TranslateRainbowError(RNBOsproRead(packet, CELL_KEY_GUID+i, (((unsigned short*)&id)+i)));
		if (FAILED(hr)) return hr;
	}
	return S_OK;
}
HRESULT RainbowDriver::ClearKeyTempGUID(RBP_SPRO_APIPACKET packet)
{
	// erase the guid on the key
	// this guid lives in cells CELL_KEY_GUID to CELL_KEY_GUID+7 inclusive
	for (unsigned int i = 0; i<sizeof(GUID)/sizeof(unsigned short); ++i)
	{
		unsigned short cell = CELL_KEY_GUID+i;
		HRESULT hr = TranslateRainbowError(RNBOsproWrite(packet, WRITE_PASSWORD, cell, 0, accessCode[cell]));
		if (FAILED(hr)) return hr;
	}
	return S_OK;
}



HRESULT RainbowDriver::TranslateRainbowError(unsigned short rnboError)
{
	if (0==rnboError) return S_OK;
	return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, LicenseServerError::ITF_MIN + LicenseServerError::ITF_RNBO_OFFSET + rnboError);
}
