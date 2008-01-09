#ifndef _LicensingWrapper_h_97082379082347905__
#define _LicensingWrapper_h_97082379082347905__

#pragma warning(disable:4786)


#include <string>
#include "..\SolimarLicenseManager\_SolimarLicenseManager.h"
#include "..\SolimarLicenseManager\ISolimarLicenseMgr.h"
#include "..\SolimarLicenseServer\KeySpec.h"
#include "..\common\ChallengeResponseHelper.h"
#include "..\common\LicensingMessage.h"
#include "..\Common\GIT.h"
#include "..\common\LicenseError.h"

// Usage:
// 1) Create a LicensingWrapper object.
// 2) Call Connect once for each license server that you wish to use keys from.
//    Generally, this will be called once, using "localhost" to use keys on the local machine.
// 3) Call Initialize to specify what product and product version is required.
// 4) Call ModuleLicenseObtain to obtain licenses for a specific module
// 5) Call RegisterMessageCallback to register for key server events if desired.
// 6) Periodically call ValidateLicense() to determine whether the obtained licenses are still valid.

namespace SolimarLicenseManagerWrapper
{
#if _MSC_VER >= 1400	
	#define _snwprintf _snwprintf_s
#endif

#define LOG_ERROR_HR(header, hr) \
	{ /* begin scope */ \
	if(FAILED(hr)) \
	{ \
		wchar_t debug_buf[1024]; \
		_snwprintf(debug_buf, sizeof(debug_buf)/sizeof(wchar_t), L"%s - %s", header, LicenseServerError::GetErrorMessage(hr).c_str()); \
		debug_buf[1023] = 0; \
		OutputDebugStringW(debug_buf); \
	} \
	} /* end scope */ \

class LicensingWrapper : public ChallengeResponseHelper
{
public:
	LicensingWrapper();
	LicensingWrapper(const LicensingWrapper &o);
	LicensingWrapper& operator=(const LicensingWrapper &o);
	virtual ~LicensingWrapper();

	//
	//Simplified versions, call Ex version of function return true/false, maintain for backward compatibility
	//

	bool Connect(std::wstring server) {return SUCCEEDED(ConnectEx(server));}
	bool Connect(std::wstring server, bool bUseOnlySharedLicenses, bool bUseAsBackup) {return SUCCEEDED(ConnectEx(server, bUseOnlySharedLicenses, bUseAsBackup));}
	bool ConnectByProduct(long product, bool bUseSharedLicenseServers = false) {return SUCCEEDED(ConnectByProductEx(product, bUseSharedLicenseServers));}
	bool Disconnect() {return SUCCEEDED(DisconnectEx());}

	//unsigned long grace_period_minutes - How long license can still be validated after there is a violation because of no keys on the system.
	//bool app_instance_lock_key - Will lock the first base on each license server.  Will lock all add-on key, all bases keys on the system must match.
	//bool bypass_remote_key_restriction - True means the restriction of remote license managers using non-remote keys has been lifted.
	bool Initialize(long product, long prod_ver_major, long prod_ver_minor, bool single_key, std::wstring specific_single_key_ident, bool lock_keys, DWORD ui_level = UI_IGNORE, unsigned long grace_period_minutes=0)	{return SUCCEEDED(InitializeEx(product, prod_ver_major, prod_ver_minor, single_key, specific_single_key_ident, lock_keys, ui_level, grace_period_minutes));}
	bool Initialize(std::wstring application_instance, long product, long prod_ver_major, long prod_ver_minor, bool single_key, std::wstring specific_single_key_ident, bool lock_keys, DWORD ui_level = UI_IGNORE, unsigned long grace_period_minutes=0, bool application_instance_lock_keys=false, bool bypass_remote_key_restriction=false){return SUCCEEDED(InitializeEx(application_instance, product, prod_ver_major, prod_ver_minor, single_key, specific_single_key_ident, lock_keys, ui_level, grace_period_minutes, application_instance_lock_keys, bypass_remote_key_restriction));}
	bool KeyProductExists(long product, long prod_ver_major, long prod_ver_minor, bool* bKeyExists) {return SUCCEEDED(KeyProductExistsEx(product, prod_ver_major, prod_ver_minor, bKeyExists));}
	bool ModuleLicenseTotal(long module, long* license_count) {return SUCCEEDED(ModuleLicenseTotalEx(module, license_count));}
	bool ModuleLicenseInUse(long module, long* license_count) {return SUCCEEDED(ModuleLicenseInUseEx(module, license_count));}
	bool ModuleLicenseObtain(long module, long license_count) {return SUCCEEDED(ModuleLicenseObtainEx(module, license_count));}
	bool ModuleLicenseRelease(long module, long license_count) {return SUCCEEDED(ModuleLicenseReleaseEx(module, license_count));}
	bool ModuleLicenseCounterDecrement(long module, long license_count) {return SUCCEEDED(ModuleLicenseCounterDecrementEx(module, license_count));}
	bool ValidateLicense() 
	{
		bool bValid;
		HRESULT hr = ValidateLicenseEx(&bValid);
		return SUCCEEDED(hr) && bValid;	
	}

	bool GetVersionLicenseManager(long* p_ver_major, long* p_ver_minor, long* p_ver_build) {return SUCCEEDED(GetVersionLicenseManagerEx(p_ver_major, p_ver_minor, p_ver_build));}
	bool GetVersionLicenseServer(std::wstring server, long* p_ver_major, long* p_ver_minor, long* p_ver_build) {return SUCCEEDED(GetVersionLicenseServerEx(server, p_ver_major, p_ver_minor, p_ver_build));}
	
	//
	//Ex versions return the hresult instead of bool
	//

	HRESULT ConnectEx(std::wstring server);
	HRESULT ConnectEx(std::wstring server, bool bUseOnlySharedLicenses, bool bUseAsBackup);
	HRESULT ConnectByProductEx(long product, bool bUseSharedLicenseServers = false);
	HRESULT DisconnectEx();
	HRESULT InitializeEx(long product, long prod_ver_major, long prod_ver_minor, bool single_key, std::wstring specific_single_key_ident, bool lock_keys, DWORD ui_level = UI_IGNORE, unsigned long grace_period_minutes=0);
	HRESULT InitializeEx(std::wstring application_instance, long product, long prod_ver_major, long prod_ver_minor, bool single_key, std::wstring specific_single_key_ident, bool lock_keys, DWORD ui_level = UI_IGNORE, unsigned long grace_period_minutes=0, bool application_instance_lock_keys=false, bool bypass_remote_key_restriction=false);
	HRESULT KeyProductExistsEx(long product, long prod_ver_major, long prod_ver_minor, bool* bKeyExists);
	HRESULT ModuleLicenseTotalEx(long module, long* license_count);
	HRESULT ModuleLicenseInUseEx(long module, long* license_count);
	HRESULT ModuleLicenseObtainEx(long module, long license_count);
	HRESULT ModuleLicenseReleaseEx(long module, long license_count);
	HRESULT ModuleLicenseCounterDecrementEx(long module, long license_count);
	HRESULT ValidateLicenseEx(bool* b_valid);
	HRESULT GetVersionLicenseManagerEx(long* p_ver_major, long* p_ver_minor, long* p_ver_build);
	HRESULT GetVersionLicenseServerEx(std::wstring server, long* p_ver_major, long* p_ver_minor, long* p_ver_build);

	typedef void (*LicenseMessageCallbackPtr)(void* pContext, const wchar_t* key_ident, unsigned int message_type, HRESULT error, VARIANT *pvtTimestamp, const wchar_t* message);
	typedef void (*LicenseInvalidCallbackPtr)(void* pContext, HRESULT lastHr, const wchar_t* message);

	bool RegisterMessageCallback(void* pContext, LicenseMessageCallbackPtr LicenseMessageCallback);
	bool UnregisterMessageCallback();

	bool RegisterInvalidLicenseCallback(void* pContext, LicenseInvalidCallbackPtr LicenseInvalidCallback);
	bool UnregisterInvalidLicenseCallback();

	long LookupProductID(std::wstring product);
	long LookupProductID(std::string product);
	long LookupModuleID(long product_id, std::wstring module);
	long LookupModuleID(long product_id, std::string module);
	long LookupHeaderID(std::wstring header);
	long LookupHeaderID(std::string header);	

	static KeySpec keyspec;

	//MOVE FOR TESTING
	//GITPtr<ISolimarLicenseMgr5> m_licenseManagerPtr;

private:
	
	static BYTE challenge_key_manager_userauththis_public[];
	static BYTE challenge_key_manager_thisauthuser_private[];
	
	HANDLE m_MessageDispatchThread;
	static DWORD WINAPI MessageDispatchThreadProc(LPVOID pWrapper);
	HRESULT MessageDispatchThreadProc(ILicensingMessage* pLicenseMessages);
	
	HANDLE m_LicenseValidityThread;
	static DWORD WINAPI LicenseValidityThreadProc(LPVOID pWrapper);
	HRESULT LicenseValidityThreadProc();
	
	// synchronizing access to member variables
	HANDLE m_MemberLock;
	HANDLE m_ThreadKillEvent;
	HRESULT m_validityCheck_LastHr;
	
	void* m_license_message_callback_context;
	LicenseMessageCallbackPtr m_license_message_callback;

	void* m_license_invalid_callback_context;
	LicenseInvalidCallbackPtr m_license_invalid_callback;

	GITPtr<ISolimarLicenseMgr5> m_licenseManagerPtr;
	GITPtr<ILicensingMessage> m_licenseMessagePtr;

	std::wstring StringToWstring(const std::string &s);
};

};

#endif //_LicensingWrapper_h_97082379082347905__
