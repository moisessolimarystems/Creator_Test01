
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif

#include "KeyServer.h"
#include "..\common\LicenseError.h"

#include <string>
#include <vector>
#include <time.h>
#include <stdarg.h>
#include <wchar.h>
#include <windows.h>

typedef _bstr_t String;
typedef std::vector<String> StringList;

#include "KeyMessages.h"
#include "..\common\MultidimensionalSafeArray.h"

// only include the password packet signing PRIVATE key on debug and solimar internal builds
BYTE KeyServer::crypto_key_password_packet_private[] = {
#if defined(_SOLIMAR_INTERNAL) || defined(_DEBUG)
#include "..\common\keys\SolimarLicensingPacket.private.key.txt"
#else
#include "..\common\keys\SolimarLicensingPacket.public.key.txt"
#endif
};
BYTE KeyServer::crypto_key_password_packet_public[] = {
#include "..\common\keys\SolimarLicensingPacket.public.key.txt"
};

BYTE KeyServer::crypto_key_password_packet_password[] = {
#include "..\common\keys\SolimarLicensingPacket.password.txt"
};


#pragma warning(disable:4355)
//APCTimer::APCTimer(APCFunc pfunc, void* arg, long interval, initfunc, void* initarg);

void KeyServer::UpdateKeysThreadFunction(void* pvThis)
{
//OutputDebugStringW(L"KeyServer::UpdateKeysThreadFunction() - Enter");
	HRESULT hr = S_OK;
	KeyServer *pThis = (KeyServer*)pvThis;
	hr = pThis->ResynchronizeKeys();
//OutputDebugStringW(L"KeyServer::UpdateKeysThreadFunction() - Leave");
}

void KeyServer::HeartbeatCheckThreadFunction(void* pvThis)
{
//OutputDebugStringW(L"KeyServer::HeartbeatCheckThreadFunction() - Enter");
	HRESULT hr = S_OK;
	KeyServer *pThis = (KeyServer*)pvThis;
	pThis->HeartbeatCheck();
//OutputDebugStringW(L"KeyServer::HeartbeatCheckThreadFunction() - Leave");
}

void KeyServer::TimesUpThreadFunction(void* pvThis)
{
//OutputDebugStringW(L"KeyServer::TimesUpThreadFunction() - Enter");
	HRESULT hr = S_OK;
	KeyServer *pThis = (KeyServer*)pvThis;
	hr = pThis->TimesUp();
//OutputDebugStringW(L"KeyServer::TimesUpThreadFunction() - Leave");
}

KeyServer::KeyServer() : 
	KeyListLock(CreateMutex(0,0,0)), 
	HeartbeatListLock(CreateMutex(0,0,0)), 
	MessageClientListLock(CreateMutex(0,0,0)),
	failed_password_attempts(0),
	USBNotification(this)
{
	_tzset();

	UpdateKeysThread = new APCTimer(UpdateKeysThreadFunction, this, UpdateKeysThreadPeriod);
	HeartbeatCheckThread = new APCTimer(HeartbeatCheckThreadFunction, this, HeartbeatCheckThreadPeriod);
	TimesUpThread = new APCTimer(TimesUpThreadFunction, this, TrialKeyDecrementCheckPeriod);

	{
	SafeMutex mutex(HeartbeatListLock);
	heartbeats.clear();
	}


	// get the local host name for use in licensing messages
	wchar_t hostname[1024];
	DWORD hostname_size = 0;
	if (GetComputerNameEx(ComputerNameDnsHostname, hostname, &(hostname_size=1024)) || GetComputerNameEx(ComputerNameNetBIOS, hostname, &(hostname_size=1024)))
	{
		server_host_name = hostname;
	}
	
	// trigger a key list resynchronize
	UpdateKeysThread->RevEnable(UpdateKeysThreadHighPeriodSeconds, UpdateKeysThreadLowPeriodSeconds, 20);
	UpdateKeysThread->Invoke();
}

KeyServer::~KeyServer()
{
	if (UpdateKeysThread)
		delete UpdateKeysThread;
	if (HeartbeatCheckThread)
		delete HeartbeatCheckThread;
	if (TimesUpThread)
		delete TimesUpThread;
	if (KeyListLock!=NULL)
		CloseHandle(KeyListLock);
	if (KeyTrialTimeInfoLock!=NULL)
		CloseHandle(KeyTrialTimeInfoLock);
	if (MessageClientListLock!=NULL)
		CloseHandle(MessageClientListLock);
}


HRESULT KeyServer::ResynchronizeKeys()
{
//OutputDebugStringW(L"KeyServer::ResynchronizeKeys() - Enter");
	HRESULT hr = S_OK;
	{// obtain a lock on the driver's key list
		SafeMutex mutex2(KeyListLock);
		SafeMutex mutex1(driver.keys_lock);
		driver.RefreshKeyList();
		if(driver.AtLeastOneParallelKey())
			UpdateKeysThread->RevUp();	//Kick up how often looking for keys.
		
		
		// first pass, add newly found keys
		for (RainbowDriver::KeyList::iterator dkey = driver.keys.begin(); dkey!=driver.keys.end(); ++dkey)
		{
			if (keys.find(dkey->first)==keys.end())
			{
				keys.insert(KeyList::value_type(dkey->first,ProtectionKey(dkey->first,&keyspec,&driver)));
			}
		}
		
		// second pass, remove keys that are no longer reported by the driver
		KeyServer::KeyList::iterator skey=keys.begin();
		while (skey!=keys.end())
		{
			if (driver.keys.find(skey->first)==driver.keys.end())
				skey = keys.erase(skey);
			else
				++skey;
		}
		
		// last pass, refresh all of the read-caches
		for (skey = keys.begin(); skey!=keys.end(); ++skey)
		{
			try
			{
				skey->second.UpdateCellCache();
			}
			catch (_com_error &e)
			{
				hr = e.Error();
				break;
			}
		}
	} // release the lock on the driver's key list
//OutputDebugStringW(L"KeyServer::ResynchronizeKeys() - Leave");
	return hr;
}

// Top level functions
HRESULT KeyServer::LicenseSessionInitialize(BSTR license_id)
{
	_bstr_t lid(license_id,true);
	
	SafeMutex mutex(MessageClientListLock);
	message_clients[lid];
	
	return S_OK;
}

HRESULT KeyServer::LicenseSessionUninitialize(BSTR license_id)
{
	_bstr_t lid(license_id,true);
	{
	SafeMutex mutex(MessageClientListLock);
	if (message_clients.find(lid)!=message_clients.end())
		message_clients.erase(lid);
	}

	//Needed to remove the license_id from the heartbeats list so the timeout
	//message is not added to the event log.
	{
	SafeMutex mutex2(HeartbeatListLock);

	HeartbeatList::iterator heartbeatIt = heartbeats.find(_bstr_t(license_id,true));
	if(heartbeatIt != heartbeats.end())
		heartbeats.erase(license_id);
	}
	return S_OK;
}

HRESULT KeyServer::Heartbeat(BSTR license_id)
{
	SafeMutex mutex(HeartbeatListLock);
	
	DWORD cur_time = (DWORD)time(0);	

//wchar_t debug_buf[1024];
//_snwprintf(debug_buf, 1024, L"KeyServer::Heartbeat (%s) cur_time=%d)", (BSTR)license_id, cur_time);
//debug_buf[1023] = 0;
//OutputDebugStringW(debug_buf);

	heartbeats[_bstr_t(license_id,true)]=cur_time;

	return S_OK;
}

HRESULT KeyServer::KeyEnumerate(VARIANT *keylist)
{
	HRESULT hr = S_OK;
	VariantInit(keylist);
	
	SafeMutex mutex(KeyListLock);

wchar_t debug_buf[1024];
//_snwprintf(debug_buf, 1024, L"KeyServer::Heartbeat (%s) cur_time=%d)", (BSTR)license_id, cur_time);
//debug_buf[1023] = 0;
//OutputDebugStringW(debug_buf);
	SAFEARRAY *pSA = SafeArrayCreateVector(VT_VARIANT, 0, (unsigned int)keys.size());
	VARIANT *pElement = 0;
	hr = SafeArrayAccessData(pSA, (void**)&pElement);
	if (SUCCEEDED(hr))
	{
		SafeMutex mutex(KeyListLock);
		for (KeyList::iterator i = keys.begin(); i!=keys.end(); ++i, ++pElement)
		{
			VariantInit(pElement);
			pElement->vt = VT_BSTR;
			pElement->bstrVal = i->first.copy();
//_snwprintf(debug_buf, 1024, L"KeyServer::KeyEnumerate keyName = %s", pElement->bstrVal);
//debug_buf[1023] = 0;
//OutputDebugStringW(debug_buf);

		}
		SafeArrayUnaccessData(pSA);
		
		keylist->vt = VT_VARIANT | VT_ARRAY;
		keylist->parray = pSA;
	}
	
	if (FAILED(hr))
	{
//OutputDebugStringW(L"KeyServer::KeyEnumerate - Failed");
		SafeArrayDestroy(pSA);
	}
	
	return hr;
}

HRESULT KeyServer::EnterPassword(BSTR password)
{
	HRESULT hr = S_OK;
	
	bool password_entered = false;
	
	// sleep a while if a lot of erronious passwords have been entered 
	// recently (an attempt to crack passwords most likely)
	DWORD sleep = PasswordEntryDelay(failed_password_attempts);
	if (sleep) Sleep(sleep);
	
	try
	{
		SafeMutex mutex(KeyListLock);
		for (KeyList::iterator key = keys.begin(); key != keys.end(); ++key)
		{
			hr = key->second.EnterPassword(_bstr_t(password,true));
			
			// update the failed password counter
			if (hr == S_OK)
			{
				password_entered = true;
				InterlockedExchange(&failed_password_attempts,0);
				break;
			}
			else
			{
				InterlockedIncrement(&failed_password_attempts);
			}
		}
	}
	catch(_com_error &e)
	{
		return e.Error();
	}
	
	return (password_entered ? S_OK : S_FALSE);
}

HRESULT KeyServer::EnterPasswordPacket(VARIANT vtPasswordPacket, BSTR *verification_code)
{
	HRESULT hr = S_OK;
	_bstr_t verification;
	
	// sanity check
	if (!(vtPasswordPacket.vt & VT_ARRAY) || !(vtPasswordPacket.vt & VT_UI1) || vtPasswordPacket.parray->rgsabound[0].cElements>=512*1024)
		return E_INVALIDARG;
	
	// create a copy of the password packet for decrypting
	BYTE *pPacket = 0;
	wchar_t *pPacketString1 = 0, *pPacketString2 = 0;
	if (FAILED(hr = SafeArrayAccessData(vtPasswordPacket.parray, (void**)&pPacket)))
		return hr;
	DWORD password_packet_length = vtPasswordPacket.parray->rgsabound[0].cElements;
	pPacketString1 = (wchar_t*)new BYTE[password_packet_length+2];
	pPacketString2 = (wchar_t*)new BYTE[password_packet_length+2];
	memset(pPacketString1, 0, password_packet_length+2);
	memset(pPacketString2, 0, password_packet_length+2);
	memcpy(pPacketString1,pPacket,password_packet_length);
	memcpy(pPacketString2,pPacket,password_packet_length);
	pPacket = 0;
	
	// decrypt the password packet (two copies)
	{
		CryptoHelper context;
		// this copy is used for strtok
		hr = context.DecryptData(crypto_key_password_packet_password, sizeof(crypto_key_password_packet_password), (BYTE*)pPacketString1, password_packet_length);
		// this copy is used to verify the hash of the packet data stored in the password packet
		hr = context.DecryptData(crypto_key_password_packet_password, sizeof(crypto_key_password_packet_password), (BYTE*)pPacketString2, password_packet_length);
	}
	
	// we're done with the source variant
	SafeArrayUnaccessData(vtPasswordPacket.parray);
	
	try
	{
		// if there is an error coming in to this block, skip the block
		if (FAILED(hr)) throw hr;

		// compute the verification code
		{
			CryptoHelper context;
			CryptoHelper::Digest digest;
			if (FAILED(hr = context.HashData((BYTE*)pPacketString1, password_packet_length, digest))) throw(hr);
			verification = std::wstring((wchar_t*)BinaryToString(digest.m_digest, digest.DIGEST_SIZE)).substr(0,8).c_str();
		}
		
		// parse the password packet
		PasswordPacket packet;
		wchar_t* pToken(0);
		if (!(pToken = wcstok(pPacketString1, L"\r\n")) || wcscmp(pToken,L"ECDBE6C6054746378D93B400527C21EE")!=0) throw(E_FAIL);	//xxx
		unsigned int expected_headers(0), expected_passwords(0);
		if (!(pToken = wcstok(NULL, L"\r\n"))) throw(E_FAIL);
		expected_headers = _wtoi(pToken);
		if (!(pToken = wcstok(NULL, L"\r\n"))) throw(E_FAIL);
		expected_passwords = _wtoi(pToken);
		
		for (unsigned int i = 0; i < expected_headers; ++i)
		{
			wchar_t *ident = 0, *value = 0;
			if (!(ident = wcstok(NULL, L"\r\n"))) throw(E_FAIL);
			if (!(value = wcstok(NULL, L"\r\n"))) throw(E_FAIL);
			packet.headers[ident] = value;
		}
		for (unsigned int j = 0; j < expected_passwords; ++j)
		{
			wchar_t *expires = 0, *password = 0;
			if (!(expires = wcstok(NULL, L"\r\n"))) throw(E_FAIL);
			if (!(password = wcstok(NULL, L"\r\n"))) throw(E_FAIL);
			// convert expires in to a floating point
			double dexpires = 1.0;
			dexpires = _wtof(expires);
			packet.passwords.push_back(ExpirePasswordPair(_variant_t(dexpires, VT_DATE),password));
		}
		
		// grab the hash and the hash signature
		wchar_t *hash = 0, *signature = 0;
		if (!(hash = wcstok(NULL, L"\r\n"))) throw(E_FAIL);
		if (!(signature = wcstok(NULL, L"\r\n"))) throw(E_FAIL);
		
		// calculate the number of bytes to used to calculate the hash
		DWORD hashed_data_size = (DWORD)((BYTE*)hash - (BYTE*)pPacketString1);

		// hash the string
		CryptoHelper context;
		CryptoHelper::Digest computed_digest, provided_digest;
		CryptoHelper::Key signature_verify_key(context, crypto_key_password_packet_public, sizeof(crypto_key_password_packet_public));
		hr = context.HashData((BYTE*)pPacketString2, hashed_data_size, computed_digest);
		if (FAILED(hr)) throw(hr);
		
		// compare the signed hash and the computed hash
		if (StringToBinaryLength(hash)!=provided_digest.DIGEST_SIZE) throw(E_INVALIDARG);
		StringToBinary(hash, provided_digest.m_digest, provided_digest.DIGEST_SIZE);
		if (computed_digest!=provided_digest) throw(E_INVALIDARG);
		
		// verify the hash signature
		_variant_t vtSignature;
		VariantInit(&vtSignature);
		vtSignature.vt = VT_ARRAY | VT_UI1;
		vtSignature.parray = SafeArrayCreateVector(VT_UI1, 0, (DWORD)wcslen(signature)/2);
		BYTE *pSignatureData = 0;
		if (SUCCEEDED(hr = SafeArrayAccessData(vtSignature.parray, (void**)&pSignatureData)))
		{
			StringToBinary(signature, pSignatureData, (DWORD)wcslen(signature)/2);
			SafeArrayUnaccessData(vtSignature.parray);
		}
		hr = context.VerifySignature(computed_digest, &vtSignature, signature_verify_key);
		if (FAILED(hr)) throw(hr);
		
		// finished with parse and validation, check for expired passwords
		bool expired_password = false;
		
		// get the current date/time
		_variant_t current_time(0.0,VT_DATE);
		SYSTEMTIME systime;
		GetSystemTime(&systime);
		if (!SystemTimeToVariantTime(&systime, &current_time.date)) throw(E_FAIL);
		
		for (PasswordList::iterator p = packet.passwords.begin(); p != packet.passwords.end(); ++p)
		{
			if (p->first.date!=0 && p->first.date<current_time.date)
			{
				expired_password = true;
				break;
			}
		}
		
		// if all of the passwords are non-expired, apply them sequentially
		// don't quit if any passwords aren't accepted because password packets 
		// now contain all passwords for the key.  Already applied passwords 
		// will fail.  Only return the HRESULT for the last entered password.
		if (!expired_password)
		{
			for (PasswordList::iterator p = packet.passwords.begin(); p != packet.passwords.end(); ++p)
			{
				hr = EnterPassword(p->second);
			}
		}
		else
		{
			// return an error indicating an expired password packet
			throw LicenseServerError::EHR_PACKET_EXPIRED;
		}
		
		// password entry succeeded, provide the verification code to the user
		*verification_code = verification.Detach();
	}
	catch (HRESULT &ehr)
	{
		hr = ehr;
	}
	catch (...)
	{
		hr = E_FAIL;
	}
	
	if (pPacketString1) delete [] pPacketString1;
	if (pPacketString2) delete [] pPacketString2;
	
	return hr;
}


HRESULT KeyServer::GenerateBasePassword(long customer_number, long key_number, BSTR *password)
{
	HRESULT hr = S_OK;

	SafeMutex mutex(KeyListLock);

	// find a programmed key, and use it to generate the password
	for (KeyList::iterator key = keys.begin(); key != keys.end(); ++key)
	{
		VARIANT_BOOL vtValid;
		hr = key->second.IsProgrammed(&vtValid);
		if (vtValid==VARIANT_TRUE)
		{
			BSTR temp_password=0;
			hr = key->second.GenerateBasePassword(customer_number, key_number, &temp_password);
			if (SUCCEEDED(hr))
			{
				*password = temp_password;
				return S_OK;
			}
			else
			{
				if (temp_password)
					SysFreeString(temp_password);
			}
		}
	}
	
	*password = SysAllocString(L"");
	
	return LicenseServerError::EHR_KEY_NO_SUITABLE_KEY;
}

HRESULT KeyServer::GenerateVersionPassword(long customer_number, long key_number, long ver_major, long ver_minor, BSTR *password)
{
	HRESULT hr = S_OK;

	SafeMutex mutex(KeyListLock);

	// find a programmed key, and use it to generate the password
	for (KeyList::iterator key = keys.begin(); key != keys.end(); ++key)
	{
		VARIANT_BOOL vtValid;
		hr = key->second.IsProgrammed(&vtValid);
		if (vtValid==VARIANT_TRUE)
		{
			BSTR temp_password=0;
			hr = key->second.GenerateVersionPassword(customer_number, key_number, ver_major, ver_minor, &temp_password);
			if (SUCCEEDED(hr))
			{
				*password = temp_password;
				return S_OK;
			}
			else
			{
				if (temp_password)
					SysFreeString(temp_password);
			}
		}
	}
	
	*password = SysAllocString(L"");
	
	return LicenseServerError::EHR_KEY_NO_SUITABLE_KEY;
}

HRESULT KeyServer::GenerateExtensionPassword(long customer_number, long key_number, long extend_days, long extension_num, BSTR *password)
{
	HRESULT hr = S_OK;

	SafeMutex mutex(KeyListLock);

	// find a programmed key, and use it to generate the password
	for (KeyList::iterator key = keys.begin(); key != keys.end(); ++key)
	{
		VARIANT_BOOL vtValid;
		hr = key->second.IsProgrammed(&vtValid);
		if (vtValid==VARIANT_TRUE)
		{
			BSTR temp_password=0;
			hr = key->second.GenerateExtensionPassword(customer_number, key_number, extend_days, extension_num, &temp_password);
			if (SUCCEEDED(hr))
			{
				*password = temp_password;
				return S_OK;
			}
			else
			{
				if (temp_password)
					SysFreeString(temp_password);
			}
		}
	}
	
	*password = SysAllocString(L"");
	
	return LicenseServerError::EHR_KEY_NO_SUITABLE_KEY;
}

HRESULT KeyServer::GenerateModulePassword(long customer_number, long key_number, long product_ident, long module_ident, long license_count, BSTR *password)
{
	HRESULT hr = S_OK;

	SafeMutex mutex(KeyListLock);

	// find a programmed key, and use it to generate the password
	for (KeyList::iterator key = keys.begin(); key != keys.end(); ++key)
	{
		VARIANT_BOOL vtValid;
		hr = key->second.IsProgrammed(&vtValid);
		if (vtValid==VARIANT_TRUE)
		{
			BSTR temp_password=0;
			hr = key->second.GenerateModulePassword(customer_number, key_number, product_ident, module_ident, license_count, &temp_password);
			if (SUCCEEDED(hr))
			{
				*password = temp_password;
				return S_OK;
			}
			else
			{
				if (temp_password)
					SysFreeString(temp_password);
			}
		}
	}
	
	*password = SysAllocString(L"");
	
	return LicenseServerError::EHR_KEY_NO_SUITABLE_KEY;
}
HRESULT KeyServer::GenerateModulePassword(long customer_number, long key_number, long product_ident, long module_ident, long license_count, BSTR *password, long password_number)
{
	HRESULT hr = S_OK;

	SafeMutex mutex(KeyListLock);

	// find a programmed key, and use it to generate the password
	for (KeyList::iterator key = keys.begin(); key != keys.end(); ++key)
	{
		VARIANT_BOOL vtValid;
		hr = key->second.IsProgrammed(&vtValid);
		if (vtValid==VARIANT_TRUE)
		{
			BSTR temp_password=0;
			hr = key->second.GenerateModulePassword(customer_number, key_number, product_ident, module_ident, license_count, password_number, &temp_password);
			if (SUCCEEDED(hr))
			{
				*password = temp_password;
				return S_OK;
			}
			else
			{
				if (temp_password)
					SysFreeString(temp_password);
			}
		}
	}
	
	*password = SysAllocString(L"");
	
	return LicenseServerError::EHR_KEY_NO_SUITABLE_KEY;
}

HRESULT KeyServer::GetLicenseServerTime(VARIANT *pvtSystemTime)
{
	// _variant_t( double dblSrc, VARTYPE vtSrc = VT_R8 );
	double dSysTime;
	SYSTEMTIME stSysTime;

	GetSystemTime(&stSysTime);
	if (SystemTimeToVariantTime(&stSysTime, &dSysTime))
	{
		*pvtSystemTime = _variant_t(dSysTime, VT_DATE).Detach();
		return S_OK;
	}
	else
	{
		VariantInit(pvtSystemTime);
		return E_FAIL;
	}
}

HRESULT KeyServer::GetLicenseMessageList(BSTR license_id, VARIANT *pvtMessageList)
{
	SafeMutex mutex(MessageClientListLock);
	
	*pvtMessageList = message_clients[_bstr_t(license_id, true)];
	message_clients[_bstr_t(license_id, true)].clear();
	
	return S_OK;
}


// Password packet management
HRESULT KeyServer::PasswordPacketInitialize(BSTR license_id)
{
	SafeMutex mutex(PasswordPacketListLock);
	_bstr_t lid(license_id,true);
	password_packets[lid] = PasswordPacket();
	
	return S_OK;
}

HRESULT KeyServer::PasswordPacketAppendPassword(BSTR license_id, VARIANT vtExpires, BSTR password)
{
	SafeMutex mutex(PasswordPacketListLock);
	_bstr_t lid(license_id,true);
	password_packets[lid].passwords.push_back(ExpirePasswordPair(vtExpires,_bstr_t(password,true)));
	
	return S_OK;
}

HRESULT KeyServer::PasswordPacketFinalize(BSTR license_id)
{
	// generate the password packet file
	HRESULT hr = S_OK;
	wchar_t buffer[64];	
	
	SafeMutex mutex(PasswordPacketListLock);
	_bstr_t lid(license_id,true);
	_bstr_t packet_string;
	
	// write the magic number
	packet_string += L"ECDBE6C6054746378D93B400527C21EE\r\n";

	// write the header information
	packet_string += _bstr_t(_variant_t((long)password_packets[lid].headers.size()));
	packet_string += L"\r\n";
	packet_string += _bstr_t(_variant_t((long)password_packets[lid].passwords.size()));
	packet_string += L"\r\n";
	
	for (KeyValueMap::iterator f = password_packets[lid].headers.begin(); f != password_packets[lid].headers.end(); ++f)
	{
		packet_string += f->first;
		packet_string += L"\r\n";
		packet_string += f->second;
		packet_string += L"\r\n";
	}

	// write the passwords
	for (PasswordList::iterator p = password_packets[lid].passwords.begin(); p != password_packets[lid].passwords.end(); ++p)
	{
		swprintf(buffer,L"%f",(double)p->first.date);
		packet_string += buffer;
		packet_string += L"\r\n";
		packet_string += p->second;
		packet_string += L"\r\n";
	}
	
	// compute a hash of the file
	CryptoHelper context;
	CryptoHelper::Digest digest;
	CryptoHelper::Key packet_sign_key(context, crypto_key_password_packet_private, sizeof(crypto_key_password_packet_private));
	_variant_t vtSignature;
	hr = context.HashData((BYTE*)(wchar_t*)packet_string, (packet_string.length())*sizeof(wchar_t), digest);
	if (FAILED(hr)) return hr;
	
	// sign the hash
	hr = context.SignHash(digest, &vtSignature);
	if (FAILED(hr)) return hr;
	
    // add the hash and the signature to the string
	packet_string += BinaryToString(digest.m_digest, digest.DIGEST_SIZE);
	packet_string += L"\r\n";
	
	BYTE *pSignature = 0;
	if (FAILED(hr = SafeArrayAccessData(vtSignature.parray, (void**)&pSignature))) return hr;
	packet_string += BinaryToString(pSignature, vtSignature.parray->rgsabound[0].cElements);
	packet_string += L"\r\n";
	SafeArrayUnaccessData(vtSignature.parray);
	
	// compute the verification code using the hash of the signature
	if (FAILED(hr = context.HashData((BYTE*)(wchar_t*)packet_string, (packet_string.length()+1)*sizeof(wchar_t), digest))) return hr;
	
	// take the first 8 characters from the hex representation of the digest as the verification code
	password_packets[lid].verification_code = std::wstring((wchar_t*)BinaryToString(digest.m_digest, digest.DIGEST_SIZE)).substr(0,8).c_str();
	
	// package the string in to a variant
	password_packets[lid].completed_packet.vt = VT_ARRAY | VT_UI1;
	password_packets[lid].completed_packet.parray = SafeArrayCreateVector(VT_UI1, 0, (packet_string.length()+1)*sizeof(wchar_t));
	BYTE *pPacketData = 0;
	if (FAILED(hr = SafeArrayAccessData(password_packets[lid].completed_packet.parray, (void**)&pPacketData))) return hr;
	memcpy(pPacketData, (wchar_t*)packet_string, (packet_string.length()+1)*sizeof(wchar_t));
	
	// encrypt the password packet
	hr = context.EncryptData(crypto_key_password_packet_password, sizeof(crypto_key_password_packet_password), pPacketData, (packet_string.length()+1)*sizeof(wchar_t));
	
	SafeArrayUnaccessData(password_packets[lid].completed_packet.parray);
	
	return hr;
}

HRESULT KeyServer::PasswordPacketGetPacket(BSTR license_id, VARIANT *pvtPacketData)
{
	SafeMutex mutex(PasswordPacketListLock);
	_bstr_t lid(license_id,true);
	
	// copy the password packet data to the outgoing parameter
	*pvtPacketData = _variant_t(password_packets[lid].completed_packet,true).Detach();
	
	return S_OK;
}

HRESULT KeyServer::PasswordPacketGetVerificationCode(BSTR license_id, BSTR *verification_code)
{
	SafeMutex mutex(PasswordPacketListLock);
	_bstr_t lid(license_id,true);
	
	// copy the password packet data to the outgoing parameter
	*verification_code = password_packets[lid].verification_code.copy();
	
	return S_OK;
}


// Key specific functions

HRESULT KeyServer::KeyTrialExpires(BSTR key_ident, VARIANT *expire_date)
{
	SafeMutex mutex(KeyListLock);
	// find the key in the key list
	KeyList::iterator key = keys.find(_bstr_t(key_ident,true));
	
	if (key!=keys.end())
	{
		return key->second.TrialExpires(expire_date);
	}
	else
	{
		return E_INVALIDARG;
	}
}

HRESULT KeyServer::KeyTrialHours(BSTR key_ident, long *trial_hours)
{
	SafeMutex mutex(KeyListLock);
	// find the key in the key list
	KeyList::iterator key = keys.find(_bstr_t(key_ident,true));
	
	if (key!=keys.end())
	{
		return key->second.TrialHours(trial_hours);
	}
	else
	{
		return E_INVALIDARG;
	}
}

HRESULT KeyServer::KeyIsActive(BSTR key_ident, VARIANT_BOOL *key_active)
{
	SafeMutex mutex(KeyListLock);
	// find the key in the key list
	KeyList::iterator key = keys.find(_bstr_t(key_ident,true));
	
	*key_active = VARIANT_FALSE;
	
	if (key!=keys.end())
	{
		return key->second.IsActive(key_active);
	}
	else
	{
		return E_INVALIDARG;
	}
}

HRESULT KeyServer::KeyIsProgrammed(BSTR key_ident, VARIANT_BOOL *key_programmed)
{
	SafeMutex mutex(KeyListLock);
	// find the key in the key list
	KeyList::iterator key = keys.find(_bstr_t(key_ident,true));
	
	*key_programmed = VARIANT_FALSE;
	
	if (key!=keys.end())
	{
		return key->second.IsProgrammed(key_programmed);
	}
	else
	{
		return E_INVALIDARG;
	}
}

HRESULT KeyServer::KeyHeaderQuery(BSTR key_ident, long header_ident, VARIANT *value)
{
wchar_t debug_buf1[1024];
//_snwprintf(debug_buf1, 1024, L"KeyServer::KeyHeaderQuery Enter - key_ident = %s, header_ident = %d", key_ident, header_ident);
//debug_buf1[1023] = 0;
//OutputDebugStringW(debug_buf1);
	SafeMutex mutex(KeyListLock);
	// find the key in the key list
	KeyList::iterator key = keys.find(_bstr_t(key_ident,true));
	
	if (key!=keys.end())
	{
		return key->second.HeaderQuery(header_ident, value);
	}
	else
	{
		return E_INVALIDARG;
	}
}

HRESULT KeyServer::KeyIsPresent(BSTR key_ident, VARIANT_BOOL *key_present)
{
	*key_present = VARIANT_FALSE;

	SafeMutex mutex(KeyListLock);
	// find the key in the key list
	KeyList::iterator key = keys.find(_bstr_t(key_ident,true));
	
	if (key!=keys.end())
	{
		return key->second.IsPresent(key_present);
	}
	else
	{
		return S_OK;
	}
}


// License locking support
// locking a key prevents it from being used by others until it is unlocked
HRESULT KeyServer::KeyLock(BSTR license_id, BSTR key_ident)
{
	SafeMutex mutex(KeyListLock);
	// find the key in the key list
	KeyList::iterator key = keys.find(_bstr_t(key_ident,true));
	
	if (key!=keys.end())
	{
		return key->second.Lock(license_id);
	}
	else
	{
		return E_INVALIDARG;
	}
}

HRESULT KeyServer::KeyUnlock(BSTR license_id, BSTR key_ident)
{
	SafeMutex mutex(KeyListLock);
	// find the key in the key list
	KeyList::iterator key = keys.find(_bstr_t(key_ident,true));
	
	if (key!=keys.end())
	{
		return key->second.Unlock(license_id);
	}
	else
	{
		return E_INVALIDARG;
	}
}

// indicates that if the key is trial, to start using up hours until it is released
HRESULT KeyServer::KeyObtain(BSTR license_id, BSTR key_ident)
{
	SafeMutex mutex(KeyListLock);
	// find the key in the key list
	KeyList::iterator key = keys.find(_bstr_t(key_ident,true));
	
	if (key!=keys.end())
	{
		return key->second.Obtain(license_id);
	}
	else
	{
		return E_INVALIDARG;
	}
}

HRESULT KeyServer::KeyRelease(BSTR license_id, BSTR key_ident)
{
	SafeMutex mutex(KeyListLock);
	// find the key in the key list
	KeyList::iterator key = keys.find(_bstr_t(key_ident,true));
	
	if (key!=keys.end())
	{
		return key->second.Release(license_id);
	}
	else
	{
		return E_INVALIDARG;
	}
}


// Module information and locking

HRESULT KeyServer::KeyValidateLicense(BSTR license_id, BSTR key_ident, VARIANT_BOOL *license_valid)
{
	SafeMutex mutex(KeyListLock);
	// find the key in the key list
	KeyList::iterator key = keys.find(_bstr_t(key_ident,true));
	
	if (key!=keys.end())
	{
		return key->second.ValidateLicense(license_id, license_valid);
	}
	else
	{
		return E_INVALIDARG;
	}
}

HRESULT KeyServer::KeyModuleEnumerate(BSTR key_ident, VARIANT *key_module_list)
{
	SafeMutex mutex(KeyListLock);
	// find the key in the key list
	KeyList::iterator key = keys.find(_bstr_t(key_ident,true));
	
	if (key!=keys.end())
	{
		return key->second.ModuleEnumerate(key_module_list);
	}
	else
	{
		return E_INVALIDARG;
	}
}

HRESULT KeyServer::KeyModuleQuery(BSTR key_ident, long module_ident, VARIANT *vtValue)
{
	SafeMutex mutex(KeyListLock);
	// find the key in the key list
	KeyList::iterator key = keys.find(_bstr_t(key_ident,true));
	
	if (key!=keys.end())
	{
		return key->second.ModuleQuery(module_ident, vtValue);
	}
	else
	{
		return E_INVALIDARG;
	}
}


HRESULT KeyServer::KeyModuleLicenseTotal(BSTR license_id, BSTR key_ident, long module_ident, long* license_count)
{
	SafeMutex mutex(KeyListLock);
	// find the key in the key list
	KeyList::iterator key = keys.find(_bstr_t(key_ident,true));
	
	if (key!=keys.end())
	{
		return key->second.ModuleLicenseTotal(license_id, module_ident, license_count);
	}
	else
	{
		return E_INVALIDARG;
	}
}

HRESULT KeyServer::KeyModuleLicenseInUse(BSTR license_id, BSTR key_ident, long module_ident, long* license_count)
{
	SafeMutex mutex(KeyListLock);

	// find the key in the key list
	KeyList::iterator key = keys.find(_bstr_t(key_ident,true));
	
	if (key!=keys.end())
	{
		return key->second.ModuleLicenseInUse(license_id, module_ident, license_count);
	}
	else
	{
		return E_INVALIDARG;
	}
}

HRESULT KeyServer::KeyModuleLicenseObtain(BSTR license_id, BSTR key_ident, long module_ident, long license_count)
{
	SafeMutex mutex(KeyListLock);
	// find the key in the key list
	KeyList::iterator key = keys.find(_bstr_t(key_ident,true));
	
	if (key!=keys.end())
	{
		return key->second.ModuleLicenseObtain(license_id, module_ident, license_count);
	}
	else
	{
		return E_INVALIDARG;
	}
}

HRESULT KeyServer::KeyModuleLicenseRelease(BSTR license_id, BSTR key_ident, long module_ident, long license_count)
{
	SafeMutex mutex(KeyListLock);
	// find the key in the key list
	KeyList::iterator key = keys.find(_bstr_t(key_ident,true));
	
	if (key!=keys.end())
	{
		return key->second.ModuleLicenseRelease(license_id, module_ident, license_count);
	}
	else
	{
		return E_INVALIDARG;
	}
}

HRESULT KeyServer::KeyModuleLicenseCounterDecrement(BSTR license_id, BSTR key_ident, long module_ident, long license_count)
{
	SafeMutex mutex(KeyListLock);
	// find the key in the key list
	KeyList::iterator key = keys.find(_bstr_t(key_ident,true));
	
	if (key!=keys.end())
	{
		return key->second.ModuleLicenseDecrementCounter(license_id, module_ident, license_count);
	}
	else
	{
		return E_INVALIDARG;
	}
}

HRESULT KeyServer::KeyModuleInUse(BSTR key_ident, long module_ident, long* license_count)
{
	SafeMutex mutex(KeyListLock);

	// find the key in the key list
	KeyList::iterator key = keys.find(_bstr_t(key_ident,true));
	
	if (key!=keys.end())
	{
		return key->second.ModuleInUse(module_ident, license_count);
	}
	else
	{
		return E_INVALIDARG;
	}
}

HRESULT KeyServer::LicenseReleaseAll(BSTR license_id)
{
	HRESULT hr = S_OK;

	SafeMutex mutex(KeyListLock);
	// for each key
	for (KeyList::iterator key = keys.begin(); key != keys.end(); ++key)
	{
		HRESULT h = key->second.LicenseReleaseAll(license_id);
		if (FAILED(h)) hr = h;
	}
	
	// return the last error encountered
	return hr;
}

// Sets all writable cells on a key to zero
HRESULT KeyServer::KeyFormat(BSTR key_ident, BSTR *new_key_ident)
{
	SafeMutex mutex(KeyListLock);
	// find the key in the key list
	KeyList::iterator key = keys.find(_bstr_t(key_ident,true));
	
	if (key!=keys.end())
	{
		return key->second.Format(new_key_ident);
	}
	else
	{
		return E_INVALIDARG;
	}
}

// Programs a key
HRESULT KeyServer::KeyProgram(BSTR key_ident, long customer_number, long key_number, long product_ident, long ver_major, long ver_minor, long key_type, long days, VARIANT module_value_list, BSTR *new_key_ident)
{
	HRESULT hr = S_OK;
	
	SafeMutex mutex(KeyListLock);
	// find the key in the key list
	KeyList::iterator key = keys.find(_bstr_t(key_ident,true));
	
	if (key!=keys.end())
	{
		hr = key->second.Program(customer_number, key_number, product_ident, ver_major, ver_minor, key_type, days, module_value_list, new_key_ident);
		if (FAILED(hr)) return hr;
		ResynchronizeKeys();
	}
	else
	{
		return E_INVALIDARG;
	}
	
	return hr;
}

// Reads raw data off of the key
HRESULT KeyServer::KeyReadRaw(BSTR key_ident, VARIANT *pvtKeyData)
{
	SafeMutex mutex(KeyListLock);
	// find the key in the key list
	KeyList::iterator key = keys.find(_bstr_t(key_ident,true));
	
	if (key!=keys.end())
	{
		return key->second.ReadRaw(pvtKeyData);
	}
	else
	{
		return E_INVALIDARG;
	}
}

void KeyServer::GenerateMessage(const wchar_t* key_ident, EMessageType message_type, HRESULT error, time_t timestamp, const unsigned int MessageLookupID, ...)
{
	static const int MAX_MESSAGE_SIZE = 1024;
	wchar_t message[MAX_MESSAGE_SIZE];
	va_list pArg;
	
	va_start(pArg, LicensingMessageStringTable[MessageLookupID]);
	_vsnwprintf(message, 1024, LicensingMessageStringTable[MessageLookupID], pArg);
	va_end(pArg);
	message[MAX_MESSAGE_SIZE-1] = 0;
	
	GenerateMessageInternal(key_ident, message_type, error, timestamp, MessageLookupID, message);
}

void KeyServer::GenerateMessageInternal(const wchar_t* key_ident, EMessageType message_type, HRESULT error, time_t timestamp, const unsigned int MessageLookupID, const wchar_t* message)
{
	static const int MAX_MESSAGE_SIZE = 1024;
	wchar_t event_log_msg[MAX_MESSAGE_SIZE];
	_variant_t vtTimestamp;
	
	// convert the time_t in to a variant date
	double vtimestamp;
	SYSTEMTIME systimestamp;
	memset(&systimestamp, 0, sizeof(systimestamp));
	tm * tm_struct = gmtime(&timestamp);
	systimestamp.wSecond = tm_struct->tm_sec;
	systimestamp.wMinute = tm_struct->tm_min;
	systimestamp.wHour = tm_struct->tm_hour;
	systimestamp.wDay = tm_struct->tm_mday;
	systimestamp.wMonth = tm_struct->tm_mon;
	systimestamp.wYear = tm_struct->tm_year;
	systimestamp.wDayOfWeek = tm_struct->tm_wday;
	if (SystemTimeToVariantTime(&systimestamp, &vtimestamp))
		vtTimestamp = _variant_t(vtimestamp, VT_DATE);
	else
		vtTimestamp = _variant_t(0.0, VT_DATE);
	
	_bstr_t str_timestamp;
	_bstr_t str_error_message;	// look in i:\chris r\samplehr.txt
	
	str_error_message = LicenseServerError::GetErrorMessage(error).c_str();
	
	// convert the date in to a string
	char cstr_timestamp[256];
	if (!GetTimeFormatA(LOCALE_USER_DEFAULT, TIME_FORCE24HOURFORMAT, &systimestamp, NULL, cstr_timestamp, 256))
		str_timestamp = cstr_timestamp;
	
	switch (message_type)
	{
	case MT_INFO:
		_snwprintf(event_log_msg, MAX_MESSAGE_SIZE, L"Solimar Systems, Inc.\r\nProduct Licensing Status Message\r\n%s\r\nKey: %s\r\n\r\n%s",
			str_timestamp,
			key_ident,
			message);
		break;
	case MT_ERROR:
		_snwprintf(event_log_msg, MAX_MESSAGE_SIZE, L"Solimar Systems, Inc.\r\nProduct Licensing Error Message\r\n%s\r\nKey: %s\r\n%08x %s\r\n\r\n%s",
			str_timestamp,
			key_ident,
			error,
			str_error_message,
			message);
		break;
	}
	
	unsigned int event_type = EVENTLOG_INFORMATION_TYPE;
	if (error & 0x8000000) event_type = EVENTLOG_ERROR_TYPE;
	LicenseServerError::WriteEventLog(event_log_msg, event_type);
	
	// notify the clients of the message
	SafeMutex mutex(MessageClientListLock);
	
	for (MessageClientList::iterator client = message_clients.begin(); client != message_clients.end(); ++client)
	{
		try
		{
			//xxx might need to make this asynchronous
			//yyy client->second->DispatchLicenseMessage(_bstr_t(key_ident), message_type, error, vtTimestamp, _bstr_t(message));
			//zzz add message to the message lists for all clients
			//xxx need to pass in the message id that indicates which message this is (eg. "trial key expired" "cannot write to key", etc.)
			LicensingMessage m(std::wstring(server_host_name), std::wstring(key_ident), vtTimestamp, message_type, MessageLookupID, std::wstring(message), error, 0, _variant_t(0.0,VT_DATE), 0);
			for (MessageClientList::iterator c = message_clients.begin(); c != message_clients.end(); ++c)
			{
				c->second.push_back(m);
				//xxx make sure that the message backlog is not excessive.
				//xxx if it is, cull some of the older messages
			}
		}
		catch (_com_error &e)
		{
			e.Error();
		}
	}
}



// support for blocking brute force attempts at password cracking
// returns the number of milliseconds to delay before checking a password
DWORD KeyServer::PasswordEntryDelay(long failed_attempts)
{
	if (failed_attempts<5)
		return 0;
	return (DWORD)(10*failed_attempts*failed_attempts*failed_attempts);
}

// checks if client heartbeats have exprired
// if they have expried, their licenses are revoked
void KeyServer::HeartbeatCheck()
{
	HRESULT hr = S_OK;
	
	SafeMutex mutex(HeartbeatListLock);
	
	HeartbeatList keepers;

	DWORD cur_time = (DWORD)time(0);
wchar_t debug_buf1[1024];
//_snwprintf(debug_buf1, 1024, L"KeyServer::HeartbeatCheck Enter - NumberOfHeartBeats = %d", heartbeats.size());
//debug_buf1[1023] = 0;
//OutputDebugStringW(debug_buf1);

	for (HeartbeatList::iterator heartbeat = heartbeats.begin(); heartbeat != heartbeats.end(); ++heartbeat)
	{
//_snwprintf(debug_buf1, 1024, L"KeyServer::HeartbeatCheck (%s) (heartbeat->second %d + HeartbeatKillClientPeriod %d < cur_time %d)", (BSTR)heartbeat->first, heartbeat->second, HeartbeatKillClientPeriod, cur_time);
//debug_buf1[1023] = 0;
//OutputDebugStringW(debug_buf1);

		if (heartbeat->second + HeartbeatKillClientPeriod < cur_time)
		{
			//xxx debug
			wchar_t debug_buf[1024];
			//_snwprintf(debug_buf, 1024, L"LicenseServerError::EHR_CLIENT_TIMEOUT (%s) (heartbeat->second %d + HeartbeatKillClientPeriod %d < cur_time %d)", (BSTR)heartbeat->first, heartbeat->second, HeartbeatKillClientPeriod, cur_time);
			_snwprintf(debug_buf, 1024, L"LicenseServerError::EHR_CLIENT_TIMEOUT  (heartbeat->second %d + HeartbeatKillClientPeriod %d < cur_time %d)", heartbeat->second, HeartbeatKillClientPeriod, cur_time);
			debug_buf[1023] = 0;
			OutputDebugStringW(debug_buf);

			GenerateMessage(L"", MT_INFO, LicenseServerError::EHR_CLIENT_TIMEOUT, time(0), MessageClientTimeout);
						
			hr = LicenseReleaseAll(heartbeat->first);
			
			// Stop notifying the client of messages
			SafeMutex mutex(MessageClientListLock);
			message_clients.erase(heartbeat->first);
		}
		else
		{
			HeartbeatList::iterator next_heartbeat = heartbeat;
			keepers.insert(heartbeat,++next_heartbeat);
		}
	}
	
	heartbeats = keepers;
//_snwprintf(debug_buf1, 1024, L"KeyServer::HeartbeatCheck Leave - NumberOfHeartBeats = %d", heartbeats.size());
//debug_buf1[1023] = 0;
//OutputDebugStringW(debug_buf1);

}

/*
	HANDLE KeyTrialTimeInfoLock;
	APCTimer *TimesUpThread;
	static void TimesUpThreadFunction(void* pvThis);
	typedef struct {time_t last_decrement; bool key_obtained;} TrialTimeInfo;
	typedef std::map<_bstr_t, time_t> TrialTimeInfoList;
	// called periodically by the TimesUpThread
	HRESULT TimesUp();
*/

HRESULT KeyServer::TimesUp()
{
	// get the key list and check if any keys in it are not in the trial key list
	{
		SafeMutex mutex1(KeyListLock);
		SafeMutex mutex2(KeyTrialTimeInfoLock);
		
		for (KeyList::iterator key = keys.begin(); key != keys.end(); ++key)
		{
			if (trial_keys.find(key->first)==trial_keys.end())
			{
				trial_keys[key->first].key_obtained = 0;
				trial_keys[key->first].last_decrement = 0;
			}
		}
	}
	
	// check for any keys that are both trial keys and are currently being used
	{
		SafeMutex mutex1(KeyListLock);
		SafeMutex mutex2(KeyTrialTimeInfoLock);		
		
		KeyList::iterator key;

		for (TrialTimeInfoList::iterator tk = trial_keys.begin(); tk != trial_keys.end(); ++tk)
		{
			if ((key = keys.find(tk->first))!=keys.end())
			{
				if (key->second.isOnTrial() && key->second.KeyInUse())
				{
					tk->second.key_obtained = true;
				}
			}
		}
	}
	
	// check all the keys for whether an hour has expired, or the key has just been obtained
	// if one of these conditions occurs, decrement an hour from the key
	{
		SafeMutex mutex(KeyTrialTimeInfoLock);

		for (TrialTimeInfoList::iterator tk = trial_keys.begin(); tk != trial_keys.end(); ++tk)
		{
			static const time_t ONE_HOUR = (time_t)(60*60);
			time_t cur_time = time(NULL);
			
			// if the key hasn't been decremented yet
			if (tk->second.last_decrement==0 || tk->second.last_decrement+ONE_HOUR<=cur_time)
			{
				// if the key was used at all since the last decrement
				if (tk->second.key_obtained)
				{
					// decrement the hours, and note when the last successful decrement was
					{
						SafeMutex mutex(KeyListLock);
						KeyList::iterator key;
						if ((key = keys.find(tk->first))!=keys.end())
						{
							// if the key is no longer a trial key, give up trying to decrement it
							if (!key->second.isOnTrial())
							{
								tk->second.key_obtained = false;
								tk->second.last_decrement = 0;
							}
							// key is a trial key, decrement the trial hours
							else
							{
								HRESULT hr = key->second.DecrementTrialHours();
								GenerateMessage((wchar_t*)tk->first, MT_INFO, hr, cur_time, MessageTempKeyDecrementing);
								if (SUCCEEDED(hr))
								{
									tk->second.key_obtained = false;
									tk->second.last_decrement = cur_time;
								}
							}
						}
					}
				}
			}
		}
	}
	
	//xxx check for expired keys?
	return S_OK;
}

// supports usb device insert/remove notification
void KeyServer::USBEventCallback(LPVOID pContext)
{
//OutputDebugStringW(L"KeyServer::USBEventCallback() - Enter");
	ResynchronizeKeys();
//OutputDebugStringW(L"KeyServer::USBEventCallback() - Leave");
}

_bstr_t KeyServer::BinaryToString(BYTE *pData, DWORD length)
{
	wchar_t *pStr = new wchar_t[(length*2)+1];
	memset(pStr,0,(length*2)+1);
	for (unsigned int b = 0; b < length; ++b)
	{
		swprintf(pStr+2*b,L"%02x",(unsigned int)pData[b]);
	}
    return pStr;
}

DWORD KeyServer::StringToBinaryLength(_bstr_t str)
{
	return str.length()/2;
}

void KeyServer::StringToBinary(_bstr_t str, BYTE *pData, DWORD length)
{
	if (length>str.length()/2)
		return;
	memset(pData, 0, length);
	for (unsigned int b = 0; b < length; ++b)
	{
		unsigned int i(0);
		swscanf(((wchar_t*)str)+2*b, L"%02x", &i);
		pData[b] = (BYTE)i;
	}
}

