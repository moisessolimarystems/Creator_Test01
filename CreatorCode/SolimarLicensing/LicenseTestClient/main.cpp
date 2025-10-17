#define _WIN32_DCOM
#define _WIN32_WINNT 0x0500

#define UNICODE
#define _UNICODE

#include <windows.h>
#include <comutil.h>
#include <tchar.h>
#include <stdio.h>
#include <comdef.h>
#include <vector>
#include <utility>
#include <dbt.h>
#include "..\common\CryptoHelper.h"
#include "..\common\ChallengeResponse.h"
#include "..\SolimarLicenseServer\_SolimarLicenseServer.h"
//#include "..\SolimarLicenseServer\_SolimarLicenseServer_i.c"
//#include "..\SolimarLicenseManager\_SolimarLicenseManager.h"
//#include "..\SolimarLicenseManager\_SolimarLicenseManager_i.c"

#include "..\SolimarLicenseServer\KeySpec.h"

#include "..\SolimarLicenseServer\rainbow\spromeps.h"

#include "..\manager_wrapper\LicensingWrapper.h"

const unsigned short int DEVELOPER_ID = (unsigned short int)0xFEA8;
const unsigned short CELL_KEY_NUMBER = (unsigned short)0x3F;
const unsigned short CELL_CUSTOMER_NUMBER = (unsigned short)0x3E;

void PrintModuleUse(ISolimarLicenseMgr *pMgr, long module)
{
	HRESULT hr;
	long total,inuse;
	hr = pMgr->ModuleLicenseTotal(module, &total);
	hr = pMgr->ModuleLicenseInUse(module, &inuse);
	wprintf(L"Module: %d\ttotal: %d\tinuse: %d\r\n",module,total,inuse);
}

void LicenseCallback(void* pContext, const wchar_t* key_ident, unsigned int message_type, HRESULT error, VARIANT *pvtTimestamp, const wchar_t *message)
{
	;
}

TCHAR *USBNotifyWindowClassName = TEXT("USB Device Insert/Remove Notificationafsdfindowk");

static BYTE key_public_data[] = {
#include "c:\temp\stupid.public.key.txt"
};
static BYTE key_private_data[] = {
#include "c:\temp\stupid.private.key.txt"
};


LRESULT CALLBACK USBNotifyCallback(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch(msg)
	{
		case WM_DEVICECHANGE:
		{
			switch(wp)
			{
				case DBT_DEVNODES_CHANGED:
				{
					return 0;
				}
			}
			break;
		}
	}
	return DefWindowProc(hwnd,msg,wp,lp);
}

int wmain(int argc, wchar_t** argv)
{
	CoInitializeEx(0, COINIT_MULTITHREADED);
	HRESULT hr = S_OK;

if (0)
{
	HRESULT hr = S_OK;

	// create the window class responsible for usb insert/remove notifications
	WNDCLASS wndclass;
	memset(&wndclass,0,sizeof(wndclass));
	wndclass.lpfnWndProc = USBNotifyCallback;
	wndclass.lpszClassName = USBNotifyWindowClassName;
	wndclass.cbWndExtra	= sizeof(void*);	// allocate some space for the 'this' pointer
	
	// register the window class, ignore the case that the class already exists
	if (!RegisterClass(&wndclass) && ERROR_CLASS_ALREADY_EXISTS!=GetLastError())
	{
		hr = HRESULT_FROM_WIN32(::GetLastError());
		return 0;
	}
/*
	// create a window that will serve as the callback for usb notification
	HWND hwnd(0);
	MSG msg;
	WNDCLASS wndclass;
	
	wchar_t *window_class = L"USB Device Insert/Remove Notification Window";
	
	memset(&wndclass,0,sizeof(wndclass));
	wndclass.lpfnWndProc = NotificationCallback;
	wndclass.lpszClassName = window_class;
	wndclass.cbWndExtra	= sizeof(void*);	// allocate some space for the 'this' pointer
	
	
	if (!RegisterClass(&wndclass))
	{
		hr = HRESULT_FROM_WIN32(::GetLastError());
	}
		
	DWORD joe = 0x56781234;
	
	hwnd = CreateWindow(window_class, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	if (!hwnd) hr = HRESULT_FROM_WIN32(::GetLastError());
	
	SetWindowLongPtr(hwnd, 0, (LONG_PTR)(&joe));
	DWORD* pjoe = (DWORD*)(LONG_PTR)GetWindowLongPtr(hwnd, 0);
	
	// usb guid is {36fc9e60-c465-11cf-8056-444553540000}
	DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
	
	GUID usb_class_guid = {0x36fc9e60, 0xc465, 0x11cf, {0x80, 0x56, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};
	ZeroMemory( &NotificationFilter, sizeof(NotificationFilter) );
	NotificationFilter.dbcc_size		= sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	NotificationFilter.dbcc_devicetype	= DBT_DEVTYP_DEVICEINTERFACE;
	NotificationFilter.dbcc_classguid	= usb_class_guid;
	
	HDEVNOTIFY hdevnotify = RegisterDeviceNotification(hwnd, &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE);
	hr = HRESULT_FROM_WIN32(::GetLastError());
	
	while(GetMessage(&msg, hwnd, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return TRUE;
*/
}

if (1)
{
	bool fun;
	{
		LicensingWrapper license;
		fun = license.Connect(L"localhost");
		fun = license.Initialize(8, 2, 11, false, std::wstring(L""), false, LicensingWrapper::UI_LEVEL_ALL | LicensingWrapper::UI_STYLE_DIALOG | LicensingWrapper::UI_STYLE_EVENT_LOG);
		fun = license.RegisterMessageCallback(0, LicenseCallback);
		LicensingMessageList msgs;
		while(1)
		{
			fun = license.ModuleLicenseObtain(0,3);
			Sleep(3*1000);
			fun = license.ModuleLicenseRelease(0,3);
			Sleep(3*1000);
		}
	}
}

if (0)
{
	// encrypt a file and decrypt a file
	_bstr_t crypto_path(L"c:\\temp\\junk.encrypted");
	DWORD buffer_size = 1024*1024;
	BYTE *plaintext_buffer = new BYTE[buffer_size], *cipher_buffer = new BYTE[buffer_size];
	memset(plaintext_buffer,0,buffer_size);
	wchar_t *plaintext=L"ECDBE6C6054746378D93B400527C21EE\n4\n4\nuser\nCRosner\ndate\nApril 14, 2004\nnobody\nhome\nwhere\nis\n38300.33\npassword1-234-234-23-42\n38301.59\npassword2-3402983409820\n99999.99\npassword3-2903803-29309283\n38310.78\npassword4-20920982098 09809\n";
	memcpy(plaintext_buffer, plaintext, wcslen(plaintext)*2+2);
	
	CryptoHelper context_sign, context_verify;
	CryptoHelper::Key private_key(context_sign, std::wstring(L"SolimarLicenseManager.ThisAuthUser.private.key"));
	CryptoHelper::Key public_key(context_verify, std::wstring(L"SolimarLicenseManager.ThisAuthUser.public.key"));
	
	CryptoHelper::Digest digest;
	hr = context_sign.HashData(plaintext_buffer, (DWORD)(wcslen(plaintext)*2+2), digest);
	VARIANT vtSignature;
	hr = context_sign.SignHash(digest, &vtSignature);
	//digest.m_digest[0] = ~digest.m_digest[0];
	hr = context_verify.VerifySignature(digest, &vtSignature, public_key);


	//hr = CryptoHelper::EncryptData(key, plaintext_buffer, cipher_buffer, (wcslen(plaintext)+1)*sizeof(wchar_t), buffer_size);
/*	
BOOL WINAPI CryptVerifySignature(
  HCRYPTHASH hHash,
  BYTE* pbSignature,
  DWORD dwSigLen,
  HCRYPTKEY hPubKey,
  LPCTSTR sDescription,
  DWORD dwFlags
);
*/

}

if (0)
{
	ISolimarLicenseSvr *pServer=0;
	
	hr = CoCreateInstance(__uuidof(CSolimarLicenseSvr), NULL, CLSCTX_LOCAL_SERVER, __uuidof(ISolimarLicenseSvr), (void**)&pServer);
	
	VARIANT vtRawData;
	hr = pServer->PasswordPacketInitialize();
	hr = pServer->PasswordPacketAppendPassword(_variant_t(38100.0,VT_DATE),_bstr_t(L"password1-password1-password1"));
	hr = pServer->PasswordPacketAppendPassword(_variant_t(38090.0,VT_DATE),_bstr_t(L"password2-password2-password2"));
	//hr = pServer->PasswordPacketFinalize(&vtRawData);
	
	HANDLE hFile = CreateFile(L"c:\\temp\\password.packet.txt", GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);
	if (hFile == INVALID_HANDLE_VALUE)
		hr = HRESULT_FROM_WIN32(GetLastError());
	DWORD bytes_written(0);
	if (!WriteFile(hFile, vtRawData.parray->pvData, vtRawData.parray->rgsabound[0].cElements, &bytes_written, 0))
		hr = HRESULT_FROM_WIN32(GetLastError());
	CloseHandle(hFile);
	//hr = pServer->KeyReadRaw(_bstr_t(L"100-25b"),&vtRawData);
	hr = hr;
}

if (0)
{
	ISolimarLicenseSvr *pServer=0;
	
	hr = CoCreateInstance(__uuidof(CSolimarLicenseSvr), NULL, CLSCTX_LOCAL_SERVER, __uuidof(ISolimarLicenseSvr), (void**)&pServer);
	
	VARIANT vtRawData;
	vtRawData.vt = VT_ARRAY | VT_UI1;
	vtRawData.parray = SafeArrayCreateVector(VT_UI1, 0, 828);

	HANDLE hFile = CreateFile(L"c:\\temp\\password.packet.txt", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (hFile == INVALID_HANDLE_VALUE)
		hr = HRESULT_FROM_WIN32(GetLastError());
	DWORD bytes_read(0);
	if (!ReadFile(hFile, vtRawData.parray->pvData, 828, &bytes_read, 0))
		hr = HRESULT_FROM_WIN32(GetLastError());
	CloseHandle(hFile);
	//hr = pServer->EnterPasswordPacket(vtRawData);
	
	//hr = pServer->KeyReadRaw(_bstr_t(L"100-25b"),&vtRawData);
	hr = hr;
}

if(0)
{
#define KEYLENGTH  0x04000000
#define ENCRYPT_ALGORITHM	AT_KEYEXCHANGE		//CALG_RSA_KEYX		//AT_KEYEXCHANGE	//CALG_RC4
#define PROVIDER			MS_STRONG_PROV		//MS_STRONG_PROV	//MS_DEF_PROV
#define PROVIDER_TYPE		PROV_RSA_FULL		//PROV_RSA_FULL

	HRESULT hr = S_OK;

	HCRYPTPROV m_context;
	if (!CryptAcquireContext(&m_context, NULL, PROVIDER, PROVIDER_TYPE, CRYPT_VERIFYCONTEXT /*CRYPT_NEWKEYSET*/))
	{
		m_context = 0;
	}
	
	wchar_t password[128];
	memset(password,0,sizeof(password));
	swprintf(password,L"fuck");
	
	// create a hash object (based on MD5)
	HCRYPTHASH hHash=0;
	if(!CryptCreateHash(m_context, CALG_MD5, 0, 0, &hHash))
	{
		hr = HRESULT_FROM_WIN32(::GetLastError());
	}
	else
	{
		if(!CryptHashData(hHash, (BYTE*)password, sizeof(password), 0))
		{
			hr = HRESULT_FROM_WIN32(::GetLastError());
		}
	}
	
	// create a key from the hash
	HCRYPTKEY hKey(0);
	if (!CryptDeriveKey(m_context, CALG_RC4, hHash, CRYPT_EXPORTABLE, &hKey))
	{
		hr = HRESULT_FROM_WIN32(::GetLastError());
	}

	CryptDestroyHash(hHash);
		
	// export the hash derived key
	BYTE key_public[1024], key_private[1024];
	DWORD key_public_size = 0, key_private_size = 0;
	
	memset(key_public, 0, 1024);
	memset(key_private, 0, 1024);
	
	key_private_size = 0;
	if (!CryptExportKey(hKey, 0,  PRIVATEKEYBLOB, 0, key_private, &key_private_size) && ERROR_MORE_DATA==GetLastError())
	{
		if (!CryptExportKey(hKey, 0,  PRIVATEKEYBLOB, 0, key_private, &key_private_size))
			hr = HRESULT_FROM_WIN32(GetLastError());
	}
	else
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
	}

	key_public_size = 0;
	if (!CryptExportKey(hKey, 0, PUBLICKEYBLOB, 0, key_public, &key_public_size) && ERROR_MORE_DATA==GetLastError())
	{
		if (!CryptExportKey(hKey, 0, PUBLICKEYBLOB, 0, key_public, &key_public_size))
			hr = HRESULT_FROM_WIN32(GetLastError());
	}
	else
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
	}
	
	CryptDestroyKey(hKey);
	CryptReleaseContext(m_context, 0);
}


if(0)
{
	ISolimarLicenseSvr *pServer=0;
	
	hr = CoCreateInstance(__uuidof(CSolimarLicenseSvr), NULL, CLSCTX_LOCAL_SERVER, __uuidof(ISolimarLicenseSvr), (void**)&pServer);
	
	//_bstr_t packet_path(L"I:\\Chris R\\password_packets\\2004430231028_packet.txt");
	//_bstr_t packet_path(L"I:\\Chris R\\password_packets\\200443023122_packet.txt");
	//_bstr_t packet_path(L"I:\\Chris R\\password_packets\\2004430231250_packet.txt");
	//_bstr_t packet_path(L"I:\\Chris R\\password_packets\\200443023137_packet.txt");
	//_bstr_t packet_path(L"I:\\Chris R\\password_packets\\2004430231430_packet.txt");
	//_bstr_t packet_path(L"I:\\Chris R\\password_packets\\200443023166_packet.txt");
	//_bstr_t packet_path(L"I:\\Chris R\\password_packets\\2004430231837_packet.txt");
	//_bstr_t packet_path(L"I:\\Chris R\\password_packets\\2004430231931_packet.txt");
	_bstr_t packet_path(L"I:\\chris r\\password_packets\\20045204336_packet.txt");
	
	HRESULT hr = S_OK;
	
	// open the password packet file
	HANDLE hFile = CreateFileW((wchar_t*)packet_path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	DWORD file_size = GetFileSize(hFile, 0);
	
	// create a variant safearray
	VARIANT vtPacket;
	VariantInit(&vtPacket);
	vtPacket.vt = VT_ARRAY | VT_UI1;
	vtPacket.parray = SafeArrayCreateVector(VT_UI1, 0, file_size);

	// access the safearray
	BYTE *pData = 0;
	if (SUCCEEDED(hr = SafeArrayAccessData(vtPacket.parray, (void**)&pData)))
	{
		// read the file in to the safearray
		DWORD bytes_read(0);
		if (!ReadFile(hFile, pData, file_size, &bytes_read, 0))
		{
			hr = HRESULT_FROM_WIN32(::GetLastError());
		}
		SafeArrayUnaccessData(vtPacket.parray);
		pData = 0;
	}
	
	// do something with the variant
	BSTR verification;
	hr = pServer->EnterPasswordPacket(vtPacket, &verification);
	
	// free up the variant safearray
	VariantClear(&vtPacket);
	
	pServer->Release();
}

if(0)
{
	ISolimarLicenseSvr *pServer=0;
	
	hr = CoCreateInstance(__uuidof(CSolimarLicenseSvr), NULL, CLSCTX_LOCAL_SERVER, __uuidof(ISolimarLicenseSvr), (void**)&pServer);
	
	SYSTEMTIME now, date_past, date_future, date_infinity;
	GetSystemTime(&now);
	memset(&date_past, 0, sizeof(SYSTEMTIME));
	memset(&date_future, 0, sizeof(SYSTEMTIME));
	memset(&date_infinity, 0, sizeof(SYSTEMTIME));
	
	date_past.wDay = 1;
	date_past.wMonth = 4;
	date_past.wYear = 2004;

	date_future.wDay = 10;
	date_future.wMonth = 5;
	date_future.wYear = 2004;
	
	VARIANT vtDatePast, vtDateFuture, vtDateInfinity;
	VariantInit(&vtDatePast);
	VariantInit(&vtDateFuture);
	VariantInit(&vtDateInfinity);
	
	vtDatePast.vt = VT_DATE;
	vtDateFuture.vt = VT_DATE;
	SystemTimeToVariantTime(&date_past, &(vtDatePast.dblVal));
	SystemTimeToVariantTime(&date_future, &(vtDateFuture.dblVal));
	vtDateInfinity.vt = VT_DATE; vtDateInfinity.dblVal = 0.0;
	
	
	hr = pServer->PasswordPacketInitialize();
	
	
	hr = pServer->PasswordPacketAppendPassword(vtDateFuture, _bstr_t(L"955c8a54-23000-4-100-0-58"));
	hr = pServer->PasswordPacketAppendPassword(vtDateFuture, _bstr_t(L"67377487-270fa-4-100-0-60"));
	
	//hr = pServer->PasswordPacketAppendPassword(vtDatePast, _bstr_t(L"23a126ca-2c104-4-100-0-64"));
	
	//hr = pServer->PasswordPacketAppendPassword(vtDateInfinity, _bstr_t(L"C8FB042"));
	//hr = pServer->PasswordPacketAppendPassword(vtDateFuture, _bstr_t(L"6AA2B800"));
	
	//hr = pServer->PasswordPacketAppendPassword(vtDatePast, _bstr_t(L"C8FB042"));
	//hr = pServer->PasswordPacketAppendPassword(vtDateInfinity, _bstr_t(L"6AA2B800"));
	
	//hr = pServer->PasswordPacketAppendPassword(vtDateFuture, _bstr_t(L"25A8C698"));
	//hr = pServer->PasswordPacketAppendPassword(vtDatePast, _bstr_t(L"EA5F14B0"));
	
	//hr = pServer->PasswordPacketAppendPassword(vtDatePast, _bstr_t(L"25A8C698"));
	//hr = pServer->PasswordPacketAppendPassword(vtDatePast, _bstr_t(L"B1C84822"));

	//hr = pServer->PasswordPacketAppendPassword(vtDateInfinity, _bstr_t(L"25A8C698"));
	//hr = pServer->PasswordPacketAppendPassword(vtDateInfinity, _bstr_t(L"B1C84822"));

	
	hr = pServer->PasswordPacketFinalize();
	
	BSTR verification;
	hr = pServer->PasswordPacketGetVerificationCode(&verification);
	VARIANT vtPacket;
	hr = pServer->PasswordPacketGetPacket(&vtPacket);
	
	wchar_t filename_base[128], filename_verify[128], filename_packet[128];
	swprintf(filename_base, L"i:\\chris r\\password_packets\\%d%d%d%d%d%d",now.wYear,now.wMonth,now.wDay,now.wHour,now.wMinute,now.wSecond);
	swprintf(filename_verify, L"%s_verification.txt",filename_base);
	swprintf(filename_packet, L"%s_packet.txt",filename_base);
	
	HANDLE hVerify = CreateFileW(filename_verify,GENERIC_WRITE,FILE_SHARE_READ,0,CREATE_NEW,0,0);
	HANDLE hPacket = CreateFileW(filename_packet,GENERIC_WRITE,FILE_SHARE_READ,0,CREATE_NEW,0,0);
	
	DWORD bytes_written, error(0);
	_bstr_t v = _bstr_t(verification,true);
	if (!WriteFile(hVerify,(char*)v,v.length(),&bytes_written, 0))
		error = GetLastError();
	
	if (!WriteFile(hPacket,vtPacket.parray->pvData,vtPacket.parray->rgsabound[0].cElements,&bytes_written, 0))
		error = GetLastError();
	
	CloseHandle(hVerify);
	CloseHandle(hPacket);
	
	/*
	BSTR pw;
	DWORD pages = 250;
	DWORD ext = 2;
	DWORD ppm_struct = (((pages & 0x0000FFFF) << 16) | (ext & 0x0000FFFF));
	hr = pServer->GenerateModulePassword(0x100, 0x00, 0, 60, (long)ppm_struct, &pw);
	pw = pw;
	*/
	
	/*
	BSTR *pKeyIdent = new BSTR;
	*pKeyIdent = _bstr_t(L"{2BE29B9E-803A-4F1C-B21B-AEC8193771D2}").Detach();
	BSTR new_key_ident;
	hr = pServer->KeyProgram(*pKeyIdent, 100, 999, 8, 2, 11, 0, 99, _variant_t(), &new_key_ident);
	*/
	
}

	CoUninitialize();
	return 0;
}









