#include "stdafx.h"
#include "CommunicationLink.h"
#include "..\SolimarLicenseServer\_SolimarLicenseServer_i.c"
#include "..\common\ChallengeResponseHelper.h"

BYTE challenge_key_manager_userauththis_public[] = {
#include "..\common\keys\SolimarLicenseServer.UserAuthThis.public.key.txt"
};

BYTE challenge_key_manager_thisauthuser_private[] = {
#include "..\common\keys\SolimarLicenseServer.ThisAuthUser.private.key.txt"
};



//maps to the IDs specified in KeySpec.xml
const long ProductID = 10021;
const long ProductVersionID = 10022;
const long LicenseID = 10025;	

//initialize the member variables
CommunicationLink::CommunicationLink()
{
	pTheServer = NULL;
	NumKeys = 0;
	NumModules = 0;
	Module = NULL;
	KeyNumber = NULL;
	ModInitialized = false;
}

//Since all of the dynamic allocation is done in the initialize functions
//the uninitialize functions take care of freeing up the memory
CommunicationLink::~CommunicationLink()
{
}

//connects to the solimar license server
HRESULT CommunicationLink::Connect()
{
	HRESULT hr = 0;	
	void* temp = 0;

	//connects to the solimar license server
	hr = CoCreateInstance(
						  __uuidof(CSolimarLicenseSvr),
						  NULL, 
						  CLSCTX_LOCAL_SERVER, 
						  __uuidof(ISolimarLicenseSvr2),
						  &temp
						 );
	if(SUCCEEDED(hr))
	{
		//if cocreateinstance succeeds save the pointer to the server
		pTheServer = reinterpret_cast<ISolimarLicenseSvr2*>(temp);
	}

	ChallengeResponseHelper cr(challenge_key_manager_thisauthuser_private, sizeof(challenge_key_manager_thisauthuser_private), challenge_key_manager_userauththis_public, sizeof(challenge_key_manager_userauththis_public));

	hr = cr.AuthenticateServer(pTheServer);
	if(!SUCCEEDED(hr))
      return hr;

	// let the license server authenticate this client
	hr = cr.AuthenticateToServer(pTheServer);
		
	return hr;
}

void CommunicationLink::Disconnect()
{
	//uninitialize the COM object
	CoUninitialize();
}

HRESULT CommunicationLink::CheckPassword(String* InputString)
{
	HRESULT hr = 0;
	_variant_t retval;

	//converts the string* into a variant
	System::Runtime::InteropServices::Marshal::
			GetNativeVariantForObject(InputString, &retval);

	//checks the password. if hr == false then invalid pwd. if hr==ok then valid password
	//else the enterpassword function failed
	hr = pTheServer->EnterPassword(retval.bstrVal);
	return hr;
}

//Get the list of keys
HRESULT CommunicationLink::InitializeKeyInfoConnection()
{
	HRESULT hr = 0;
	pKeyList = new VARIANT();

	//get the list of keys
	hr = pTheServer->KeyEnumerate(pKeyList);

	return hr;
}

HRESULT CommunicationLink::EnterPasswordPacket(VARIANT* TheData, BSTR* Verification_Code)
{
	return pTheServer->EnterPasswordPacket(*TheData, Verification_Code);
}
//Gets the module list
HRESULT CommunicationLink::InitializeModuleLicenseConnection(Object* TheKeyNumber)
{
		_variant_t retval;
		
		ModuleList = new VARIANT();

		//store the key number
		KeyNumber = TheKeyNumber;

		//convert the TheKeyNumber from an Object to a variant
		System::Runtime::InteropServices::Marshal::
			GetNativeVariantForObject(TheKeyNumber, &retval);

		//converts the variant to a bstr
		retval.ChangeType(VT_BSTR);
		
		VARIANT temp;
      VariantInit(&temp);
		HRESULT hr = 0;

		//get the list of modules
		hr = pTheServer->KeyModuleEnumerate(retval.bstrVal, &temp);
		
		//if we successfully got the list of modules store it in our member
		if(SUCCEEDED(hr))
			*ModuleList = temp;

		void* vp;

		// SafeArrayAccessData and SafeArrayUnaccessData are win32 functions 
		//needed to access safe array data
		if (SUCCEEDED(SafeArrayAccessData(ModuleList->parray,&vp)))
		{
			//if we successfully got a ptr to the first module, store it in
			//our member variable
			Module = reinterpret_cast<VARIANT*>(vp);
		}
		VariantClear(&temp);
		ModInitialized = true;
		return hr;
}

//Free up the memory allocated in the initialize function
HRESULT CommunicationLink::UnLockModuleList()
{
	HRESULT hr = 0;

	// release the lock on the ModuleList
	hr = SafeArrayUnaccessData(ModuleList->parray);

	if(ModuleList)
	{
      VariantClear(ModuleList);
		delete ModuleList;
	}
   return hr;
}

void CommunicationLink::UnInitializeModuleLicenseConnection()
{
	ModInitialized = false;
}

bool CommunicationLink::IsModInitialized()
{
	return ModInitialized;
}

//Free up the memory allocated in the init function
void CommunicationLink::UnInitializeKeyInfoConnection()
{
	if(pKeyList)
	{
		delete pKeyList;
		pKeyList = NULL;
	}
}

//returns the number of keys
int CommunicationLink::GetNumKeys()
{
	HRESULT hr = 0;

	//get the list of keys
	hr = pTheServer->KeyEnumerate(pKeyList);

	if(SUCCEEDED(hr))
		return pKeyList->parray->rgsabound[0].cElements;

	return 0;
}

//returns the number of modules in the mod list
int CommunicationLink::GetNumModules()
{
   int numModules = 0;
	if(SUCCEEDED(InitializeModuleLicenseConnection(KeyNumber)))
   {
		numModules = ModuleList->parray->rgsabound[0].cElements;
      
      // release the lock on the ModuleList
   	if(IsModInitialized())
	   	UnLockModuleList();
   }

	return numModules;
}

//Fills the KeyInfo Struct with the values read from the key
void CommunicationLink::GetKeyInfoStructure(KeyInfoStructure& TheKeyInfoStruct,
										    unsigned int KeyIDIndex
										   )
{
		VARIANT* pKeyName;
		HRESULT hr = 0;

      //get the list of keys
	   hr = pTheServer->KeyEnumerate(pKeyList);

		// SafeArrayAccessData and SafeArrayUnaccessData are win32 functions 
		//needed to access safe array data
		if (SUCCEEDED(SafeArrayAccessData(pKeyList->parray, (void**)&pKeyName)))
		{
			// grab the ith key identifier out of the array
			_bstr_t key_identifier = _bstr_t(pKeyName[KeyIDIndex].bstrVal,true);

			//Fill up the structure appropriately
			TheKeyInfoStruct.KeyNumber.SetString(key_identifier);

			//using the solimar license server, query the key header for the specified info
			hr = pTheServer->KeyHeaderQuery(key_identifier, 
											ProductID, 
											&(TheKeyInfoStruct.ProductID)
										   );

			hr = pTheServer->KeyHeaderQuery(key_identifier, 
											ProductVersionID, 
											&(TheKeyInfoStruct.ProductVersion)
										   );

			hr = pTheServer->KeyHeaderQuery(key_identifier, 
											LicenseID, 
											&(TheKeyInfoStruct.License)
										   );


			hr = pTheServer->KeyTrialHours(key_identifier, 
										   &(TheKeyInfoStruct.HoursLeft)
										  );
			
			hr = pTheServer->KeyIsActive(key_identifier, 
										 &(TheKeyInfoStruct.Active.boolVal)
										);

			hr = pTheServer->KeyTrialExpires(key_identifier, 
											&(TheKeyInfoStruct.ExpirationDate)
										   );
		}
		// release the lock on the KeyList
		SafeArrayUnaccessData(pKeyList->parray);
}

//Fills the ModLicesne Struct with the values read from the key
void CommunicationLink::GetModuleLicensingStructureArray(ModuleLicensingStructure& TheModStruct,
														 unsigned int ModuleIndex
														)
{
		VARIANT* ModuleInfo;
		_variant_t retval;

      InitializeModuleLicenseConnection(KeyNumber);
		if (SUCCEEDED(SafeArrayAccessData(Module[ModuleIndex].parray,
											(void**)(&ModuleInfo))))
		{
			TheModStruct.ModuleID = ModuleInfo[0];
			TheModStruct.ModuleName = ModuleInfo[1];

			//convert the TheKeyNumber from an Object to a variant
			System::Runtime::InteropServices::Marshal::
				GetNativeVariantForObject(KeyNumber, &retval);

			//converts the variant to a bstr
			retval.ChangeType(VT_BSTR);

			pTheServer->KeyModuleLicenseTotal(retval.bstrVal, 
											  (TheModStruct.ModuleID).lVal, 
											  &(TheModStruct.TotalLicenses)
											 );

			pTheServer->KeyModuleInUse(retval.bstrVal,
											  (TheModStruct.ModuleID).lVal,
											  &(TheModStruct.LicensesInUse)
											  );
		}

		// release the lock on the current module
		SafeArrayUnaccessData(Module[ModuleIndex].parray);

      // release the lock on the ModuleList
   	if(IsModInitialized())
	   	   UnLockModuleList();
}

long CommunicationLink::GetLicensesInUse(Object* ModuleID, Object* KeyNumber)
{
	long retval = 0;
	_variant_t KeyNumVar;
	_variant_t ModIDVar;

	//convert the TheKeyNumber from an Object to a variant
	System::Runtime::InteropServices::Marshal::GetNativeVariantForObject(KeyNumber, &KeyNumVar);
	System::Runtime::InteropServices::Marshal::GetNativeVariantForObject(ModuleID, &ModIDVar);
	
	//change the variant to an int
	ModIDVar.ChangeType(VT_I4);
	pTheServer->KeyModuleLicenseInUse(KeyNumVar.bstrVal, ModIDVar.lVal, &retval);

	return retval;
}

long CommunicationLink::GetTotalLicenses(Object* ModuleID, Object* KeyNumber)
{
	long retval = 0;
	_variant_t KeyNumVar;
	_variant_t ModIDVar;

	//convert the TheKeyNumber from an Object to a variant
	System::Runtime::InteropServices::Marshal::GetNativeVariantForObject(KeyNumber, &KeyNumVar);
	System::Runtime::InteropServices::Marshal::GetNativeVariantForObject(ModuleID, &ModIDVar);

	//change the Module ID variant to an int
	ModIDVar.ChangeType(VT_I4);
	pTheServer->KeyModuleLicenseInUse(KeyNumVar.bstrVal, ModIDVar.lVal, &retval);

	pTheServer->KeyModuleLicenseTotal(KeyNumVar.bstrVal, ModIDVar.lVal, &retval);

	return retval;
}

//==============================================================================
// Function:    programmed()
// Purpose:     Determines if key is programmed. Returns True if key is programmed,
//              False otherwise or for key error.
// Parameters:  None
// Returns:     bool
// Note:
//==============================================================================
bool CommunicationLink::KeyIsProgrammed(BSTR* KeyID)
{
	VARIANT_BOOL IsPresent;
	HRESULT hr = 0;

	if(pTheServer)
	{
		hr = pTheServer->KeyIsProgrammed(*KeyID, &IsPresent);
        if(SUCCEEDED(hr))
			return (IsPresent == VARIANT_TRUE ? true : false);
	}

	return false;
}
