// This is the main project file for VC++ application project 
// generated using an Application Wizard.
#define _WIN32_DCOM
#define _WIN32_WINNT 0x0500

#include "stdafx.h"
#include <windows.h>
#include <comutil.h>
#include <comdef.h>

#include "..\manager_wrapper\LicensingWrapper.h"

//#include <string>
#include "..\SolimarLicenseServer\_SolimarLicenseServer.h"
#include "..\SolimarLicenseManager\_SolimarLicenseManager.h"
//#include "..\SolimarLicenseServer\KeySpec.h"
//#include "..\common\ChallengeResponseHelper.h"
#include "..\common\LicensingMessage.h"
#include "..\SolimarLicenseServer\KeySpec.h"

#using <mscorlib.dll>



using namespace System;

BYTE challenge_key_manager_thisauthuser_public[] = {
#include "..\common\keys\SolimarLicenseManager.ThisAuthUser.public.key.txt"
};

BYTE challenge_key_manager_userauththis_private[] = {
#include "..\common\keys\SolimarLicenseManager.UserAuthThis.private.key.txt"
};

BYTE challenge_key_server_thisauthuser_private[] = {
#include "..\common\keys\SolimarLicenseServer.ThisAuthUser.private.key.txt"
};

BYTE challenge_key_server_userauththis_public[] = {
#include "..\common\keys\SolimarLicenseServer.UserAuthThis.public.key.txt"
};

const unsigned short int DEVELOPER_ID = (unsigned short int)0xFEA8;
const unsigned short CELL_KEY_NUMBER = (unsigned short)0x3F;
const unsigned short CELL_CUSTOMER_NUMBER = (unsigned short)0x3E;


void licenseMessageCallback(
  void* context,
  const wchar_t* keyIdent,
  unsigned int messageType,
  HRESULT error,
  VARIANT* pvtTimestamp,
  const wchar_t* message
)
{
  SolimarLicenseManagerWrapper::LicensingWrapper& license = *((SolimarLicenseManagerWrapper::LicensingWrapper*) context);

  int i;
  char m[512];
  m[0] = 0;
  for (i = 0; message[i]; ++i) {
    if (i >= sizeof(m)) {
      break;
    }
    m[i] = (char) message[i];
    m[i + 1] = 0;
  }

  char keyID[512];
  keyID[0] = 0;
  for (i = 0; keyIdent[i]; ++i) {
    if (i >= sizeof(keyID)) {
      break;
    }
    keyID[i] = (char) keyIdent[i];
    keyID[i + 1] = 0;
  }

  char tmpBuf[256];
  sprintf_s(tmpBuf, sizeof(tmpBuf), "%MessageCallBack - s: %s\n",keyID,m);
  Console::WriteLine(tmpBuf);

}


int _tmain()
{
    CoInitializeEx(0, COINIT_MULTITHREADED);

	//My Key
	
	unsigned int ProductID = 12;	//Rubika
	//ProductID = 8;	//SOLsearcher
	ProductID = 14;	//SPDE
	unsigned int CustomerNumber = 256;
	//unsigned int KeyNumber = 0x41E;//Alan's Test Key
	unsigned int KeyNumber = 0x554;//Test Key
	System::String* inputValue;
	char tmpbuf[256];
	//std::wstring licenseServer(L"crosner3");
	std::wstring licenseServer(L"jlan5");
	//std::wstring licenseServer(L"192.168.10.130");
	//std::wstring licenseServer(L"bjackson5");
	//std::wstring backUpLicenseServer(L"jlan3");//jlan3
	std::wstring backUpLicenseServer(L"");
	//std::wstring appInstance(L"jlan5");
	std::wstring appInstance(L"");
	bool bAppInstanceLockKey(true);
	bool bUseOnlySharedLicenses(false);
	bool bBypassRemoteKeyRestrictions(true);

	bool bSuccess;
	SolimarLicenseManagerWrapper::LicensingWrapper license;
	
	///*
	//bSuccess = license.Connect(licenseServer.c_str(), false, false);
	//bSuccess = license.Connect(backUpLicenseServer.c_str(), false, true);
	bSuccess = license.Connect(licenseServer.c_str(), bUseOnlySharedLicenses, false);
	if(!bSuccess)
	{
		sprintf_s(tmpbuf, "* Failed to connect to %S", licenseServer.c_str());
		Console::WriteLine(tmpbuf);
	}
	if(backUpLicenseServer.length() != 0)
	{
		bSuccess = license.Connect(backUpLicenseServer.c_str(), bUseOnlySharedLicenses, true);
		if(!bSuccess)
		{
			sprintf_s(tmpbuf, "* Failed to connect to %S", backUpLicenseServer.c_str());
			Console::WriteLine(tmpbuf);
		}
	}

	
	//bSuccess = license.Initialize(ProductID, 1, 0, false, std::wstring(L""), false, 0);
	//bSuccess = license.Initialize(L"jlan5", ProductID, 1, 0, false, std::wstring(L""), false, UI_LEVEL_ALL | UI_STYLE_EVENT_LOG, 0, true);
	//bSuccess = license.Initialize(L"jlan5", ProductID, 1, 0, false, std::wstring(L""), false, UI_LEVEL_ALL | UI_STYLE_EVENT_LOG, 0, false);
	//bSuccess = license.Initialize(L"jlan2", ProductID, 1, 0, false, std::wstring(L""), false, UI_LEVEL_ALL | UI_STYLE_EVENT_LOG, 0, true);
	bSuccess = license.Initialize(appInstance.c_str(), ProductID, 1, 0, false, std::wstring(L""), true, UI_LEVEL_ALL | UI_STYLE_EVENT_LOG, 0, bAppInstanceLockKey, bBypassRemoteKeyRestrictions);
	//*/
	//bSuccess = license.Initialize(ProductID, 1, 0, false, std::wstring(L""), false, UI_LEVEL_ALL | UI_STYLE_EVENT_LOG);
	//license.Initialize(12, 5, 0, false, std::wstring(L""), false, UI_IGNORE);

/*	
LicensingMessageList message_list;
VARIANT vtMessageList;
VariantInit(&vtMessageList);
ILicensingMessage* pLicenseMessages = NULL; 
HRESULT hr = license.m_licenseManagerPtr->QueryInterface(__uuidof(ILicensingMessage), (void**)&pLicenseMessages);
hr = pLicenseMessages->GetLicenseMessageList(VARIANT_TRUE, &vtMessageList);
pLicenseMessages->Release();
std::wstring errMsg;
if(FAILED(hr))
{
	errMsg = LicenseServerError::GetErrorMessage(hr);
	Console::WriteLine(errMsg.c_str());
}
if (vtMessageList.vt & VT_ARRAY)
	message_list = vtMessageList;
VariantClear(&vtMessageList);
for (LicensingMessageList::iterator m = message_list.begin(); m != message_list.end(); ++m)
{
	//m_license_message_callback(m_license_message_callback_context, m->key_ident.c_str(), m->message_type, m->error, &m->timestamp, m->message.c_str());
	Console::WriteLine(m->message.c_str());
}
//*/
//	license.RegisterMessageCallback(&license,licenseMessageCallback);
	//license.
	
/*
	LicensingMessageList message_list;
	VARIANT vtMessageList;
	VariantInit(&vtMessageList);
	hr = pLicenseMessages->GetLicenseMessageList(VARIANT_TRUE, &vtMessageList);
	if (FAILED(hr)) return hr;
	if (vtMessageList.vt & VT_ARRAY)
		message_list = vtMessageList;
	VariantClear(&vtMessageList);
*/
//long moduleID = 31;	//Process Builder - RUBIKA
long moduleID = 105;	//SPDE - SCSI SDI Output - value 4
long moduleID2 = 28;	//SPDE - SMART Output - value 1 (Unlimited)
long moduleID3 = 240;	//SPDE - Output - value 10 (Unlimited)
long moduleID4 = 400;	//SPDE - Cuncurrent Operator - value 255 (Unlimited) - isShared
long incrementAmount = 1;

	if(!bSuccess)
	{
		CoUninitialize();

		Console::WriteLine(S"FAILED - license.Initialize()");
		Console::WriteLine(S"Finish... Hit Any Key To Exit");
		Console::ReadLine();

		return 0;
	}

/*
//long moduleID = 20;
long moduleID = 400;
long moduleID2 = 401;
long incrementAmount = 20;
//*/

	for(;;)
	{
		Console::WriteLine("*****************************************");
		//sprintf_s(tmpbuf, "* Configed for ProductID %d - Key Number %3x-%3x", ProductID, CustomerNumber, KeyNumber);
		sprintf_s(tmpbuf, "* Configed for ProductID %d", ProductID);
		Console::WriteLine(tmpbuf);
		sprintf_s(tmpbuf, "* Key Server: %S, Backup Key Server: %S", licenseServer.c_str(), backUpLicenseServer.c_str());
		Console::WriteLine(tmpbuf);
		sprintf_s(	tmpbuf, 
			"* Application Instance: %S, bAppInstanceLockKey: %S, bUseOnlySharedLicenses: %S, bBypassRemoteKeyRestrictions: %S", 
					appInstance.c_str(),
					bAppInstanceLockKey ? L"true" : L"false",
					bUseOnlySharedLicenses ? L"true" : L"false",
					bBypassRemoteKeyRestrictions ? L"true" : L"false");

		Console::WriteLine(tmpbuf);
		//Console::WriteLine("*");
		Console::WriteLine("*");
		sprintf_s(tmpbuf,"* Module %d - SCSI SDI; non-on/off", moduleID);
		Console::WriteLine(tmpbuf);
		sprintf_s(tmpbuf,"*     o1 - Obtain Licensing Module %d", moduleID);
		Console::WriteLine(tmpbuf);
		sprintf_s(tmpbuf,"*     r1 - Release Licensing Module %d", moduleID);
		Console::WriteLine(tmpbuf);
		sprintf_s(tmpbuf,"*     q1 - In Use Licensing Module %d", moduleID);
		Console::WriteLine(tmpbuf);
		sprintf_s(tmpbuf,"*     t1 - Total Licensing Module %d", moduleID);
		Console::WriteLine(tmpbuf);
		sprintf_s(tmpbuf,"* Module %d - SMART Output; on/off", moduleID2);
		Console::WriteLine(tmpbuf);
		/*
		sprintf_s(tmpbuf,"*     o2 - Obtain Licensing Module %d", moduleID2);
		Console::WriteLine(tmpbuf);
		sprintf_s(tmpbuf,"*     r2 - Release Licensing Module %d", moduleID2);
		Console::WriteLine(tmpbuf);
		sprintf_s(tmpbuf,"*     q2 - In Use Licensing Module %d", moduleID2);
		Console::WriteLine(tmpbuf);
		sprintf_s(tmpbuf,"*     t2 - Total Licensing Module %d", moduleID2);
		Console::WriteLine(tmpbuf);
		sprintf_s(tmpbuf,"* Module %d - Output; non-on/off; Unlimited(10)", moduleID3);
		Console::WriteLine(tmpbuf);
		sprintf_s(tmpbuf,"*     o3 - Obtain Licensing Module %d", moduleID3);
		Console::WriteLine(tmpbuf);
		sprintf_s(tmpbuf,"*     r3 - Release Licensing Module %d", moduleID3);
		Console::WriteLine(tmpbuf);
		sprintf_s(tmpbuf,"*     q3 - In Use Licensing Module %d", moduleID3);
		Console::WriteLine(tmpbuf);
		sprintf_s(tmpbuf,"*     t3 - Total Licensing Module %d", moduleID3);
		Console::WriteLine(tmpbuf);
		sprintf_s(tmpbuf,"* Module %d - Concurrent Operator; non-on/off; Unlimited(255); isShared", moduleID4);
		Console::WriteLine(tmpbuf);
		sprintf_s(tmpbuf,"*     o4 - Obtain Licensing Module %d", moduleID4);
		Console::WriteLine(tmpbuf);
		sprintf_s(tmpbuf,"*     r4 - Release Licensing Module %d", moduleID4);
		Console::WriteLine(tmpbuf);
		sprintf_s(tmpbuf,"*     q4 - In Use Licensing Module %d", moduleID4);
		Console::WriteLine(tmpbuf);
		sprintf_s(tmpbuf,"*     t4 - Total Licensing Module %d", moduleID4);
		Console::WriteLine(tmpbuf);
		*/
		sprintf_s(tmpbuf,"* gp - Generate Password");
		Console::WriteLine(tmpbuf);
		/*
		sprintf_s(tmpbuf,"* o - Obtain Licensing Module %d", moduleID);
		Console::WriteLine(tmpbuf);
		sprintf_s(tmpbuf,"* r - Release Licensing Module %d", moduleID);
		Console::WriteLine(tmpbuf);
		sprintf_s(tmpbuf,"* q - In Use Licensing Module %d", moduleID);
		Console::WriteLine(tmpbuf);
		sprintf_s(tmpbuf,"* t - Total Licensing Module %d", moduleID);
		Console::WriteLine(tmpbuf);
		sprintf_s(tmpbuf,"* d - Decrement Module %d", moduleID);
		Console::WriteLine(tmpbuf);
		sprintf_s(tmpbuf,"* a - Obtain Licensing Module %d", moduleID2);
		Console::WriteLine(tmpbuf);
		sprintf_s(tmpbuf,"* b - Release Licensing Module %d", moduleID2);
		Console::WriteLine(tmpbuf);
		sprintf_s(tmpbuf,"* c - In Use Licensing Module %d", moduleID2);
		Console::WriteLine(tmpbuf);
		sprintf_s(tmpbuf,"* t2 - Total Licensing Module %d", moduleID2);
		Console::WriteLine(tmpbuf);
		*/
		Console::WriteLine("* v - Validate Licensing");
		//sprintf_s(tmpbuf,"* ip - Apply Increment Password for Module %d", moduleID);
		//Console::WriteLine(tmpbuf);
		//Console::WriteLine("* ai - Apply Application Instance Password");
		//Console::WriteLine("* s - Start Multiple Threads");
		Console::WriteLine("* lmv - License Manager Version");
		Console::WriteLine("* lsv - License Server Version");
		Console::WriteLine("* x - Exit");
		//Console::WriteLine("* endless - Start An Endless loop of obtain & release calls");
		//Console::WriteLine("*");
		Console::WriteLine("*****************************************");

		///*	
LicensingMessageList message_list;
sprintf_s(tmpbuf, "* Licensing Messages", ProductID);
Console::WriteLine(tmpbuf);
VARIANT vtMessageList;
VariantInit(&vtMessageList);
ILicensingMessage* pLicenseMessages = NULL; 
HRESULT hr = license.m_licenseManagerPtr->QueryInterface(__uuidof(ILicensingMessage), (void**)&pLicenseMessages);
hr = pLicenseMessages->GetLicenseMessageList(VARIANT_TRUE, &vtMessageList);
pLicenseMessages->Release();
std::wstring errMsg;
if(FAILED(hr))
{
	errMsg = LicenseServerError::GetErrorMessage(hr);
	Console::WriteLine(errMsg.c_str());
}
if (vtMessageList.vt & VT_ARRAY)
	message_list = vtMessageList;
VariantClear(&vtMessageList);
for (LicensingMessageList::iterator m = message_list.begin(); m != message_list.end(); ++m)
{
	//m_license_message_callback(m_license_message_callback_context, m->key_ident.c_str(), m->message_type, m->error, &m->timestamp, m->message.c_str());
	Console::WriteLine(m->message.c_str());
}
Console::WriteLine("*****************************************");
//*/


		inputValue = Console::ReadLine();

		
		if(System::String::Compare(inputValue, "x", true) == 0)
			break;

		if(	System::String::Compare(inputValue, "o1", true) == 0 ||
			System::String::Compare(inputValue, "o2", true) == 0 ||
			System::String::Compare(inputValue, "o3", true) == 0 ||
			System::String::Compare(inputValue, "o4", true) == 0)

		{
			int tmpMod = -1;
			if(System::String::Compare(inputValue, "o1", true) == 0)
				tmpMod = moduleID;
			if(System::String::Compare(inputValue, "o2", true) == 0)
				tmpMod = moduleID2;
			if(System::String::Compare(inputValue, "o3", true) == 0)
				tmpMod = moduleID3;
			if(System::String::Compare(inputValue, "o4", true) == 0)
				tmpMod = moduleID4;
			bSuccess = license.ModuleLicenseObtain(tmpMod,incrementAmount);
			sprintf_s(tmpbuf, "* Module: %d - license.ModuleLicenseObtain = %i", tmpMod, bSuccess);
			Console::WriteLine(tmpbuf);
		}

		if(	System::String::Compare(inputValue, "r1", true) == 0 ||
			System::String::Compare(inputValue, "r2", true) == 0 ||
			System::String::Compare(inputValue, "r3", true) == 0 ||
			System::String::Compare(inputValue, "r4", true) == 0)

		{
			int tmpMod = -1;
			if(System::String::Compare(inputValue, "r1", true) == 0)
				tmpMod = moduleID;
			if(System::String::Compare(inputValue, "r2", true) == 0)
				tmpMod = moduleID2;
			if(System::String::Compare(inputValue, "r3", true) == 0)
				tmpMod = moduleID3;
			if(System::String::Compare(inputValue, "r4", true) == 0)
				tmpMod = moduleID4;
			bSuccess = license.ModuleLicenseRelease(tmpMod,incrementAmount);
			sprintf_s(tmpbuf, "* Module: %d - license.ModuleLicenseRelease = %i", tmpMod, bSuccess);
			Console::WriteLine(tmpbuf);
		}

		if(	System::String::Compare(inputValue, "q1", true) == 0 ||
			System::String::Compare(inputValue, "q2", true) == 0 ||
			System::String::Compare(inputValue, "q3", true) == 0 ||
			System::String::Compare(inputValue, "q4", true) == 0)

		{
			int tmpMod = -1;
			if(System::String::Compare(inputValue, "q1", true) == 0)
				tmpMod = moduleID;
			if(System::String::Compare(inputValue, "q2", true) == 0)
				tmpMod = moduleID2;
			if(System::String::Compare(inputValue, "q3", true) == 0)
				tmpMod = moduleID3;
			if(System::String::Compare(inputValue, "q4", true) == 0)
				tmpMod = moduleID4;

			long licenseCount;
			bSuccess = license.ModuleLicenseInUse(tmpMod,&licenseCount);
			sprintf_s(tmpbuf, "* Module: %d - license.ModuleLicenseInUse = %i, inUse = %i", tmpMod, bSuccess, licenseCount);
			Console::WriteLine(tmpbuf);
		}

		if(	System::String::Compare(inputValue, "t1", true) == 0 ||
			System::String::Compare(inputValue, "t2", true) == 0 ||
			System::String::Compare(inputValue, "t3", true) == 0 ||
			System::String::Compare(inputValue, "t4", true) == 0)

		{
			int tmpMod = -1;
			if(System::String::Compare(inputValue, "t1", true) == 0)
				tmpMod = moduleID;
			if(System::String::Compare(inputValue, "t2", true) == 0)
				tmpMod = moduleID2;
			if(System::String::Compare(inputValue, "t3", true) == 0)
				tmpMod = moduleID3;
			if(System::String::Compare(inputValue, "t4", true) == 0)
				tmpMod = moduleID4;

			long licenseCount;
			bSuccess = license.ModuleLicenseTotal(tmpMod,&licenseCount);
			sprintf_s(tmpbuf, "* Module: %d - license.ModuleLicenseTotal = %i, total = %i", tmpMod, bSuccess, licenseCount);
			Console::WriteLine(tmpbuf);
		}



		if(System::String::Compare(inputValue, "o", true) == 0)
		{
			//for(int x1=0; x1<50; x1++)
			//{
			bSuccess = license.ModuleLicenseObtain(moduleID,incrementAmount);
			sprintf_s(tmpbuf, "* license.ModuleLicenseObtain = %i", bSuccess);
			Console::WriteLine(tmpbuf);
			//}
		}

		if(System::String::Compare(inputValue, "r", true) == 0)
		{
			//bSuccess = license.ModuleLicenseRelease(20,20);
			bSuccess = license.ModuleLicenseRelease(moduleID,incrementAmount);
			sprintf_s(tmpbuf, "* license.ModuleLicenseRelease = %i", bSuccess);
			Console::WriteLine(tmpbuf);
		}

		if(System::String::Compare(inputValue, "q", true) == 0)
		{
			long licenseCount;
			bSuccess = license.ModuleLicenseInUse(moduleID,&licenseCount);
			sprintf_s(tmpbuf, "* license.ModuleLicenseInUse = %i, inUse = %i", bSuccess, licenseCount);
			Console::WriteLine(tmpbuf);
		}
		if(System::String::Compare(inputValue, "t", true) == 0)
		{
			long licenseCount;
			bSuccess = license.ModuleLicenseTotal(moduleID,&licenseCount);
			sprintf_s(tmpbuf, "* license.ModuleLicenseTotal = %i, total = %i", bSuccess, licenseCount);
			Console::WriteLine(tmpbuf);
		}
		if(System::String::Compare(inputValue, "d", true) == 0)
		{
			bSuccess = license.ModuleLicenseCounterDecrement(moduleID,20);
			//bSuccess = license.ModuleLicenseCounterDecrement(20,20000);
			sprintf_s(tmpbuf, "* license.ModuleLicenseCounterDecrement = %i", bSuccess);
			Console::WriteLine(tmpbuf);
		}
		if(System::String::Compare(inputValue, "a", true) == 0)
		{
			bSuccess = license.ModuleLicenseObtain(moduleID2,20);
			sprintf_s(tmpbuf, "* license.ModuleLicenseObtain = %i", bSuccess);
			Console::WriteLine(tmpbuf);
		}

		if(System::String::Compare(inputValue, "b", true) == 0)
		{
			bSuccess = license.ModuleLicenseRelease(moduleID2,20);
			sprintf_s(tmpbuf, "* license.ModuleLicenseRelease = %i", bSuccess);
			Console::WriteLine(tmpbuf);
		}

		if(System::String::Compare(inputValue, "c", true) == 0)
		{
			long licenseCount;
			bSuccess = license.ModuleLicenseInUse(moduleID2,&licenseCount);
			sprintf_s(tmpbuf, "* license.ModuleLicenseObtain = %i, inUse = %i", bSuccess, licenseCount);
			Console::WriteLine(tmpbuf);
		}
		/*
		if(System::String::Compare(inputValue, "t2", true) == 0)
		{
			long licenseCount;
			bSuccess = license.ModuleLicenseTotal(moduleID2,&licenseCount);
			sprintf_s(tmpbuf, "* license.ModuleLicenseTotal = %i, total = %i", bSuccess, licenseCount);
			Console::WriteLine(tmpbuf);
		}
		*/


		if(System::String::Compare(inputValue, "v", true) == 0)
		{
			bSuccess = license.ValidateLicense();
			sprintf_s(tmpbuf, "* license.ValidateLicense = %i", bSuccess);
			Console::WriteLine(tmpbuf);
		}

		if(System::String::Compare(inputValue, "gp", true) == 0)
		{
			for(;;)
			{
				ISolimarLicenseSvr *pServer=0;
				HRESULT hr = CoCreateInstance(__uuidof(CSolimarLicenseSvr), NULL, CLSCTX_LOCAL_SERVER, __uuidof(ISolimarLicenseSvr), (void**)&pServer);
				if(FAILED(hr))
					break;

				ChallengeResponseHelper CR(challenge_key_server_thisauthuser_private, sizeof(challenge_key_server_thisauthuser_private), challenge_key_server_userauththis_public, sizeof(challenge_key_server_userauththis_public));
				// try to authenticate the license server
				hr = CR.AuthenticateServer(pServer);
				if (FAILED(hr)) break;
				
				// let the license server authenticate this manager
				hr = CR.AuthenticateToServer(pServer);
				if (FAILED(hr)) break;

				BSTR pw1;
				Console::WriteLine("Trying to Generating Password");
				int passwordNumber = 1;
				int moduleAmount = 2;
				hr = pServer->GenerateModulePassword2(CustomerNumber, KeyNumber, ProductID, moduleID, moduleAmount, passwordNumber, &pw1);
				if(FAILED(hr))
				{
					sprintf_s(tmpbuf, "* Failed GenerateModulePassword2 = %x", hr);
					Console::WriteLine(tmpbuf);
					break;
				}

				sprintf_s(tmpbuf, "* pServer->GenerateModulePassword2(%d, %d, %d) = %S", moduleID, moduleAmount, passwordNumber, pw1);
				Console::WriteLine(tmpbuf);
				//Console::WriteLine("Successfully Generated Password");

				///*
				hr = pServer->EnterPassword(pw1);
				if(FAILED(hr))
				{
					sprintf_s(tmpbuf, "* Failed EnterPassword = %x", hr);
					Console::WriteLine(tmpbuf);
					break;
				}

				Console::WriteLine("Password Successfully Applied");
				//*/

				break;//Unconditional break
			}
		}

		if(System::String::Compare(inputValue, "ip", true) == 0)
		{
			for(;;)
			{
				ISolimarLicenseSvr *pServer=0;
				HRESULT hr = CoCreateInstance(__uuidof(CSolimarLicenseSvr), NULL, CLSCTX_LOCAL_SERVER, __uuidof(ISolimarLicenseSvr), (void**)&pServer);
				if(FAILED(hr))
					break;

				ChallengeResponseHelper CR(challenge_key_server_thisauthuser_private, sizeof(challenge_key_server_thisauthuser_private), challenge_key_server_userauththis_public, sizeof(challenge_key_server_userauththis_public));
				// try to authenticate the license server
				hr = CR.AuthenticateServer(pServer);
				if (FAILED(hr)) break;
				
				// let the license server authenticate this manager
				hr = CR.AuthenticateToServer(pServer);
				if (FAILED(hr)) break;

				BSTR pw1;
				Console::WriteLine("Trying to Generating Password");
				hr = pServer->GenerateModulePassword2(CustomerNumber, KeyNumber, ProductID, 20, 4096, 1, &pw1);
				if(FAILED(hr))
				{
					sprintf_s(tmpbuf, "* Failed GenerateModulePassword2 = %x", hr);
					Console::WriteLine(tmpbuf);
					break;
				}

				Console::WriteLine("Successfully Generated Password");

				hr = pServer->EnterPassword(pw1);
				if(FAILED(hr))
				{
					sprintf_s(tmpbuf, "* Failed EnterPassword = %x", hr);
					Console::WriteLine(tmpbuf);
					break;
				}

				Console::WriteLine("Password Successfully Applied");
				break;
			
			//bSuccess = license.ValidateLicense();
			//sprintf(tmpbuf, "* license.ValidateLicense = %i", bSuccess);
			//Console::WriteLine(tmpbuf);
			}
		}
		if(System::String::Compare(inputValue, "ai", true) == 0)
		{
			for(;;)
			{
				ISolimarLicenseSvr *pServer=0;
				HRESULT hr = CoCreateInstance(__uuidof(CSolimarLicenseSvr), NULL, CLSCTX_LOCAL_SERVER, __uuidof(ISolimarLicenseSvr), (void**)&pServer);
				if(FAILED(hr))
					break;

				ChallengeResponseHelper CR(challenge_key_server_thisauthuser_private, sizeof(challenge_key_server_thisauthuser_private), challenge_key_server_userauththis_public, sizeof(challenge_key_server_userauththis_public));
				// try to authenticate the license server
				hr = CR.AuthenticateServer(pServer);
				if (FAILED(hr)) break;
				
				// let the license server authenticate this manager
				hr = CR.AuthenticateToServer(pServer);
				if (FAILED(hr)) break;

				BSTR pw1 = _bstr_t(L"1e8f3b6c-2-5-100-41e-10024-1");
				/*
				Console::WriteLine("Trying to Generating Password");
				hr = pServer->(CustomerNumber, KeyNumber, ProductID, 20, 4096, 1, &pw1);
				if(FAILED(hr))
				{
					sprintf_s(tmpbuf, "* Failed GenerateModulePassword2 = %x", hr);
					Console::WriteLine(tmpbuf);
					break;
				}
				*/

				hr = pServer->EnterPassword(pw1);
				if(FAILED(hr))
				{
					sprintf_s(tmpbuf, "* Failed EnterPassword = %x", hr);
					Console::WriteLine(tmpbuf);
					break;
				}

				Console::WriteLine("Successfully Generated Password");

				Console::WriteLine("Password Successfully Applied");
				break;	//Unconditional Break
			}
		}
		

		if(System::String::Compare(inputValue, "s", true) == 0)
		{
			//sprintf(tmpbuf, "url:http://www.google.com");
			HINSTANCE hInstance = ShellExecute(NULL, "open", "http://www.google.com", NULL, NULL, SW_SHOWNORMAL);
			//HINSTANCE hInstance = ShellExecute(NULL, "open", tmpbuf, NULL, NULL, SW_SHOWNORMAL);
			sprintf_s(tmpbuf, "* hInstance = %d", hInstance);
			Console::WriteLine(tmpbuf);
		}

		if(System::String::Compare(inputValue, "lmv", true) == 0)
		{
			long majorVersion;
			long minorVersion;
			long buildVersion;
			license.GetVersionLicenseManager(&majorVersion, &minorVersion, &buildVersion);
			sprintf_s(tmpbuf, "* license.GetVersionLicenseManager = %i.%i.%i", majorVersion, minorVersion, buildVersion);
			Console::WriteLine(tmpbuf);
		}
		if(System::String::Compare(inputValue, "lsv", true) == 0)
		{
			
			long majorVersion;
			long minorVersion;
			long buildVersion;
			license.GetVersionLicenseServer(licenseServer.c_str(), &majorVersion, &minorVersion, &buildVersion);
			sprintf_s(tmpbuf, "* license.GetVersionLicenseServer - %S = %i.%i.%i", licenseServer.c_str(), majorVersion, minorVersion, buildVersion);
			Console::WriteLine(tmpbuf);
		}



		if(System::String::Compare(inputValue, "endless", true) == 0)
		{
			int count = 0;
         /*
			SolimarLicenseManagerWrapper::LicensingWrapper licenseTmp;
				bSuccess = licenseTmp.Connect(L"localhost", true);
				if(!bSuccess)
					break;
				bSuccess = licenseTmp.Initialize(ProductID, 1, 0, false, std::wstring(L""), false, 0);
				if(!bSuccess)
					break;
            */
			for(;;)
			{
				sprintf_s(tmpbuf,"*License Loop %d", count);
				Console::WriteLine(tmpbuf);
            OutputDebugString(tmpbuf);
				count++;
            OutputDebugString("*License Loop Create LicensingWrapper");
				SolimarLicenseManagerWrapper::LicensingWrapper licenseTmp;
//            OutputDebugString("*License Loop Connect");
//				bSuccess = licenseTmp.Connect(L"localhost", true);
				if(!bSuccess)
					break;
//            OutputDebugString("*License Loop Initialize");
//				bSuccess = licenseTmp.Initialize(ProductID, 1, 0, false, std::wstring(L""), false, 0);
				if(!bSuccess)
					break;

//            OutputDebugString("*License Loop ModuleLicenseObtain");
//				bSuccess = licenseTmp.ModuleLicenseObtain(moduleID,incrementAmount);
				if(!bSuccess)
					break;

//            OutputDebugString("*License Loop ModuleLicenseRelease");
//				bSuccess = licenseTmp.ModuleLicenseRelease(moduleID,incrementAmount);
				//*/
				//if(!bSuccess)
				//	break;
			}
		}
		
	}

	CoUninitialize();

	Console::WriteLine(S"Finish... Hit Any Key To Exit");
	Console::ReadLine();

	return 0;
}