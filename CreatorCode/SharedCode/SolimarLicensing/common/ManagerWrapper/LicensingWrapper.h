#pragma once
//#ifndef _LicensingWrapper_h_97082379082347905__
//#define _LicensingWrapper_h_97082379082347905__

#pragma warning(disable:4786)


#include <string>
//#include "..\SolimarLicenseManager\_SolimarLicenseManager.h"								// CR.32662; commented out.
#include "..\ISolimarLicenseMgr.h"
#include "..\ILicensingMessage.h"

#include "..\KeySpec.h"
#include "..\ChallengeResponseHelper.h"
#include "..\LicensingMessage.h"
#include "..\GIT.h"
#include "..\LicenseError.h"
#include "..\FromSpd\L_SpdException.h"																// CR.32662.V01; added.

// class ComError - simple wrapper for _com_error to include a context description.
//
// SLB 21.feb.2024 CR.32662; created.
// SLB 29.feb.2024 CR.32662.V01 Added the ThrowManagedEx() method to ease management
// from unmanged exception class to a managed one.
// SLB 10.sep.2025 CR.34426.V01; This file is used in unmanaged projects outside of Licensing. Added
// conditional use of ThrowManagedEx(). If CLR is enabled at compile time, then the function 
// is defined.
class ComError : public _com_error
{
	std::wstring m_wsErrorContext;
public:
	ComError(HRESULT hr, const wchar_t* pwcsErrContext) : _com_error(hr), m_wsErrorContext(pwcsErrContext) {}
	std::wstring GetErrorContext() const { return m_wsErrorContext; }
	#ifdef __CLR_VER																						// CR.34426.V01; added #ifdef/#endif.
	void ThrowManagedEx() const																		// CR.32662.V01; added.
	{
		// This method replaces code that looked like this:
		// throw gcnew System::Runtime::InteropServices::COMException(gcnew System::String(LicenseServerError::GetErrorMessage(Error()).c_str()), Error());
		throw gcnew System::Runtime::InteropServices::COMException(gcnew System::String(GetErrorContext().c_str()), Error());
	}
	#endif
};

using namespace SpdException;																			// CR.32662.V01; added.


// Usage:
// 1) Create a LicensingWrapper object.
// 2) Call Connect once for each license server that you wish to use keys from.
//    Generally, this will be called once, using "localhost" to use keys on the local machine.
// 3) Call Initialize to specify what product and product version is required.
// 4) Call ModuleLicenseObtain to obtain licenses for a specific module
// 5) Call RegisterMessageCallback to register for key server events if desired.
// 6) Periodically call ValidateLicense() to determine whether the obtained licenses are still valid.

typedef SSILicensing::GITPtr<ISolimarLicenseMgr8> ISolimarLicenseMgr8Ptr;				// CR.34426.V01; modified. CR.32662; added.

namespace SolimarLicenseManagerWrapper
{
#if _MSC_VER >= 1400	
	#define _snwprintf _snwprintf_s
#endif

// SLB 30.jul.2025 CR.34312.V01; changed OutputDebugStringW() call to CEnterExitMsg::TraceMsg() so 
// debug messages will have threadId and be formatted.
#define LOG_ERROR_HR(header, hr) \
	{ /* begin scope */ \
		if (FAILED(hr)) \
		{ \
			CEnterExitMsg::TraceMsg(L"%s - %s", header, LicenseServerError::GetErrorMessage(hr).c_str()); \
		} \
	} /* end scope */ 


#define ENSURE_LICENSING_CONSTRUCTED_SUCCESSFULLY(hr) \
	{ if (FAILED(hr)) throw ComError(hr, L"SolimarLicenseManagerWrapper::LicensingWrapper - Error creating Solimar License Manager"); } 

// CR.32662.V01; Created LOG_ERROR_HR2(), EX_WRAPPER_BEGIN() and EX_WRAPPER_END to 
// leverage the CEnterExitMsg debug logging functionality and handle ComError exceptions.
#define LOG_ERROR_HR2(func, e) \
	{ CEnterExitMsg::TraceMsg(L"%08X  %s\n%s", e.Error(), e.GetErrorContext().c_str(), (const char*)e.Description()); } 

#define EX_WRAPPER_BEGIN \
	try \
	{

#define EX_WRAPPER_END \
	} \
	catch (ComError& e) { LOG_ERROR_HR2(__WFUNCTION__, e); return false; } \
	return true; 

/* class LicensingWrapper
 *
 * CR.34426.V01 Note: In the Licensing solution, this class is only used by the 
 * DOT NET wrapper assembly. However this class is shared to, and used by, other 
 * products in unmanaged targets (e.g. SSE).
 *
 * SLB 29.feb.2024 CR.32662.V01; Modified ConnectEx() methods to return void and 
 * throw ComError when there's a problem. Made corresponding changes to macros
 * and inline wrapper methods to handls this.
 * SLB 20.feb.2025 CR.32662.V06; Work to requirement that License Server be
 * installed on the current machine, when License Viewer needs to connect to a
 * remote server.
 * Cleaned up some of the class interface to use consistent string
 * type (std::wstring). In many cases using references and const references.
 * Some methods now (e.g ConnectEx()) throw exceptions instead of return HRESULTs.
 * Some cosmetic formatting changes to these methods.
 */
class LicensingWrapper : public ChallengeResponseHelper
{

public:
	LicensingWrapper();
	LicensingWrapper(
		const std::wstring &domain, 																	// CR.32662.V06; modified.
		const std::wstring &username,																	// CR.32662.V06; modified.
		const std::wstring &password,																	// CR.32662.V06; modified.
		long authenticationLevel,
		long impersonationLevel);
	LicensingWrapper(const LicensingWrapper &o);
	LicensingWrapper& operator=(const LicensingWrapper &o);
	virtual ~LicensingWrapper();

	//
	//Simplified versions, call Ex version of function return true/false, maintain for backward compatibility
	//

	bool Connect(const std::wstring &server) { EX_WRAPPER_BEGIN; ConnectEx(server); EX_WRAPPER_END; }
	bool Connect(const std::wstring &server, bool bUseOnlySharedLicenses, bool bUseAsBackup) { EX_WRAPPER_BEGIN; ConnectEx(server, bUseOnlySharedLicenses, bUseAsBackup); EX_WRAPPER_END; }
	bool ConnectByProduct(long product, bool bUseSharedLicenseServers = false) { EX_WRAPPER_BEGIN; ConnectByProductEx(product, bUseSharedLicenseServers); EX_WRAPPER_END; }
	bool GetInfoByProduct(long product, std::wstring &server, std::wstring &backup_server, bool &bTestDev, bool bUseSharedLicenseServers = false) {return GetInfoByProduct(product, server, backup_server, bTestDev, bUseSharedLicenseServers);}
	bool Disconnect() {return SUCCEEDED(DisconnectEx());}

	//unsigned long grace_period_minutes - How long license can still be validated after there is a violation because of no keys on the system.
	//bool app_instance_lock_key - Will lock the first base on each license server.  Will lock all add-on key, all bases keys on the system must match.
	//bool bypass_remote_key_restriction - True means the restriction of remote license managers using non-remote keys has been lifted.
	bool Initialize(long product, long prod_ver_major, long prod_ver_minor, bool single_key, const std::wstring &specific_single_key_ident, bool lock_keys, DWORD ui_level = UI_IGNORE, unsigned long grace_period_minutes=0)	{return SUCCEEDED(InitializeEx(product, prod_ver_major, prod_ver_minor, single_key, specific_single_key_ident, lock_keys, ui_level, grace_period_minutes));}
	bool Initialize(const std::wstring &app_instance, long product, long prod_ver_major, long prod_ver_minor, bool single_key, const std::wstring specific_single_key_ident, bool lock_keys, DWORD ui_level = UI_IGNORE, unsigned long grace_period_minutes=0, bool app_instance_lock_keys=false, bool bypass_remote_key_restriction=false){return SUCCEEDED(InitializeEx(app_instance, product, prod_ver_major, prod_ver_minor, single_key, specific_single_key_ident, lock_keys, ui_level, grace_period_minutes, app_instance_lock_keys, bypass_remote_key_restriction));}

	bool KeyProductExists(long product, long prod_ver_major, long prod_ver_minor, bool* bKeyExists) {return SUCCEEDED(KeyProductExistsEx(product, prod_ver_major, prod_ver_minor, bKeyExists));}
	bool ModuleLicenseTotal(long module, long* license_count) {return SUCCEEDED(ModuleLicenseTotalEx(module, license_count));}
	bool ModuleLicenseInUse(long module, long* license_count) {return SUCCEEDED(ModuleLicenseInUseEx(module, license_count));}
	bool ModuleLicenseInUse_ByApp(long module, long* license_count) {return SUCCEEDED(ModuleLicenseInUse_ByAppEx(module, license_count));}
	bool ModuleLicenseObtain(long module, long license_count) {return SUCCEEDED(ModuleLicenseObtainEx(module, license_count));}
	bool ModuleLicenseRelease(long module, long license_count) {return SUCCEEDED(ModuleLicenseReleaseEx(module, license_count));}
	bool ModuleLicenseCounterDecrement(long module, long license_count) {return SUCCEEDED(ModuleLicenseCounterDecrementEx(module, license_count));}
	bool ValidateLicense() 
	{
		bool bValid;
		HRESULT hr = ValidateLicenseEx(&bValid);
		return SUCCEEDED(hr) && bValid;	
	}

	bool GetVersionLicenseManager(
		long* p_ver_major, 
		long* p_ver_minor, 
		long* p_ver_build) 
	{ return SUCCEEDED(GetVersionLicenseManagerEx(p_ver_major, p_ver_minor, p_ver_build));}
	bool GetVersionLicenseServer(
		const std::wstring &server, 
		long* p_ver_major, 
		long* p_ver_minor, 
		long* p_ver_build) 
	{ return SUCCEEDED(GetVersionLicenseServerEx(server, p_ver_major, p_ver_minor, p_ver_build));}
	
	//
	//Ex versions return the hresult instead of bool
	//

	void ConnectEx(
		const std::wstring &server);																	// CR.32662.V06; modified.
	void ConnectEx(
		const std::wstring &server, 																	// CR.32662.V06; modified.
		bool bUseOnlySharedLicenses, 
		bool bUseAsBackup, 
		bool bUseOnlyLicenseViewer = false);
	void ConnectByProductEx(
		long product, 
		bool bUseSharedLicenseServers = false);
	void GetInfoByProductEx(
		long product, 
		std::wstring &server, 																			// CR.32662.V06; modified.
		std::wstring &backup_server, 																	// CR.32662.V06; modified.
		bool &bTestDev, 																					// CR.32662.V06; modified.
		bool bUseSharedLicenseServers = false);

	HRESULT DisconnectEx();
	HRESULT InitializeEx(
		long product, 
		long prod_ver_major, 
		long prod_ver_minor, 
		bool single_key, 
		const std::wstring &specific_single_key_ident, 											// CR.32662.V06; modified.
		bool lock_keys, 
		DWORD ui_level = UI_IGNORE, 
		unsigned long grace_period_minutes=0);
	HRESULT InitializeEx(
		const std::wstring &app_instance, 															// CR.32662.V06; modified.
		long product, 
		long prod_ver_major, 
		long prod_ver_minor, 
		bool single_key, 
		const std::wstring &specific_single_key_ident, 											// CR.32662.V06; modified.
		bool lock_keys, 
		DWORD ui_level = UI_IGNORE, 
		unsigned long grace_period_minutes=0, 
		bool app_instance_lock_keys=false, 
		bool bypass_remote_key_restriction=false);
	HRESULT InitializeViewOnly(
		const std::wstring &app_instance, 															// CR.32662.V06; modified.
		long product, 
		long prod_ver_major, 
		long prod_ver_minor);

	HRESULT KeyProductExistsEx(long product, long prod_ver_major, long prod_ver_minor, bool* bKeyExists);
	HRESULT ModuleLicenseTotalEx(long module, long* license_count);
	HRESULT ModuleLicenseInUseEx(long module, long* license_count);
	HRESULT ModuleLicenseInUse_ByAppEx(long module, long* license_count);
	HRESULT ModuleLicenseObtainEx(long module, long license_count);
	HRESULT ModuleLicenseReleaseEx(long module, long license_count);
	HRESULT ModuleLicenseCounterDecrementEx(long module, long license_count);
	HRESULT ValidateLicenseEx(bool* b_valid);
	HRESULT GetVersionLicenseManagerEx(
		long* p_ver_major, 
		long* p_ver_minor, 
		long* p_ver_build);
	HRESULT GetVersionLicenseServerEx(
		const std::wstring &server, 
		long* p_ver_major, 
		long* p_ver_minor, 
		long* p_ver_build);

	HRESULT StartLicensingSessionEx(long* session_id);
	HRESULT ModuleLicenseObtainLicensingSessionEx(long session_id, long module, long license_count);
	HRESULT ModuleLicenseReleaseLicensingSessionEx(long session_id, long module, long license_count);
	HRESULT ModuleLicenseInUseLicensingSessionEx(long session_id, long module, long* license_count);
	HRESULT EndLicensingSessionEx(long session_id);
	
	// Note the use of __stdcall for compatibility with managed code
	typedef void (__stdcall *LicenseMessageCallbackPtr)(void* pContext, const wchar_t* key_ident, unsigned int message_type, HRESULT error, VARIANT *pvtTimestamp, const wchar_t* message);
	typedef void (__stdcall *LicenseInvalidCallbackPtr)(void* pContext, HRESULT lastHr, const wchar_t* message);

	bool RegisterMessageCallback(void* pContext, LicenseMessageCallbackPtr LicenseMessageCallback);
	bool UnregisterMessageCallback();

	bool RegisterInvalidLicenseCallback(void* pContext, LicenseInvalidCallbackPtr LicenseInvalidCallback);
	bool UnregisterInvalidLicenseCallback();

	long LookupProductID(const std::wstring &product);
	long LookupProductID(const std::string &product);
	long LookupModuleID(long product_id, const std::wstring &module);
	long LookupModuleID(long product_id, const std::string &module);
	long LookupHeaderID(const std::wstring &header);
	long LookupHeaderID(const std::string &header);

	static KeySpec keyspec;

	//MOVE FOR TESTING
	ISolimarLicenseMgr8Ptr m_licenseManagerPtr;													// CR.34426.V01; modified. 

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
	HRESULT m_constructorHR;
	
	void* m_license_message_callback_context;
	LicenseMessageCallbackPtr m_license_message_callback;

	void* m_license_invalid_callback_context;
	LicenseInvalidCallbackPtr m_license_invalid_callback;

	//GITPtr<ISolimarLicenseMgr5> m_licenseManagerPtr;
	SSILicensing::GITPtr<ILicensingMessage> m_licenseMessagePtr;							// CR.34426.V01; modified. 

	std::wstring StringToWstring(const std::string &s);


	std::map<int/*sessionID*/, std::map<int/*modID*/, int/*value*/>*> m_session_obtained_map;

	void Initialize(
		const std::wstring &domain,																	// CR.32662.V06; modified.
		const std::wstring &username,																	// CR.32662.V06; modified.
		const std::wstring &password,																	// CR.32662.V06; modified.
		long authenticationLevel,
		long impersonationLevel);

};

};



//#endif //_LicensingWrapper_h_97082379082347905__
