// SolimarLicenseMgr.h : Declaration of the CSolimarLicenseMgr

#pragma once

#include "stdafx.h"
#include "resource.h"       // main symbols
#include <time.h>
#include "..\SolimarLicenseServer\KeySpec.h"
#include "..\common\IObjectAuthentication.h"
#include "..\common\ILicensingMessage.h"
#include "..\common\ChallengeResponseHelper.h"
#include "..\common\ss_rpc_failed.h"
#include "..\Common\LicAttribsCPP\Lic_PackageAttribs.h"

#import "..\SolimarLicenseServer\_SolimarLicenseServer.tlb" no_smart_pointers no_namespace raw_interfaces_only exclude("IObjectAuthentication","ILicensingMessage")

#include "..\common\apctimer.h"

#include <comutil.h>
#include <vector>
#include <map>
#include <list>

#include "..\common\InProcPtr.h"
#include "ISolimarLicenseMgr.h"
#include "..\common\LicensingMessage.h"
#include "..\common\LicensingSoftwareMessage.h"
#include "..\common\GIT.h"

/*
 * SS_SLSERVER_FTCALL 
 *	Fault tolerant call for SolimarLicenseServer
 */
#define SS_SLSERVER_FTCALL(srvInfoObj, func, plist) \
	{ /* begin hr scope */ \
	HRESULT hr; \
	SS_SLSERVER_FTCALL_HR(srvInfoObj func, plist, hr) \
	if(FAILED(hr)) \
		throw _com_error(hr); \
	} /* end hr scope */ \

/*
 * SS_SLSERVER__HR 
 *	Fault tolerant call which sets an HRESULT.
 */
#define SS_SLSERVER_FTCALL_HR(srvInfoObj, func, plist, hr) \
{ /* begin hr scope */ \
	bool __retry; \
	bool __connected = true; \
	do { \
		try \
		{ \
			__retry = false; \
			HRESULT connectHR = LicenseServerError::EHR_CLIENT_TIMEOUT; \
			if (!__connected ) \
			{ \
				connectHR = srvInfoObj.Reconnect(); \
				ReConnect(&srvInfoObj); \
			} \
			if (__connected || SUCCEEDED(connectHR)) \
			{ \
				hr = srvInfoObj.LicenseServer->##func##plist; \
				if (SS_RPC_FAILED(hr)) \
					throw _com_error(hr); \
			} \
			else \
				throw _com_error(connectHR); \
		} \
		catch (_com_error& e) \
		{ \
			__connected = false; \
			/* TRACE("FUNCTION FAILED");*/ \
			/* TRACE("	Function name	= " #func);*/ \
			/* TRACE("	Parameters		= " #plist);*/ \
			/* TRACE("	HRESULT			= %X", e.Error());*/ \
			if (SS_RPC_FAILED(e.Error())) \
			{ \
				if (SUCCEEDED(srvInfoObj.Reconnect())) \
				{ \
					ReConnect(&srvInfoObj); \
					__retry = true; \
					__connected = true; \
				} \
			} \
			if (!__retry) \
				hr = e.Error(); \
		} \
	} while (__retry); \
} /* end scope */ \

/*
 * SS_SLSERVER_ON_INTERFACE_FTCALL
 *	Fault tolerant call on an interface from srvInfoObj
 */
#define SS_SLSERVER_ON_INTERFACE_FTCALL(interfaceType, srvInfoObj, func, plist) \
{ /* begin hr scope */ \
	HRESULT hr; \
	SS_SLSERVER_ON_INTERFACE_FTCALL_HR(interfaceType, srvInfoObj, func, plist, hr); \
	if(FAILED(hr)) \
		throw _com_error(hr); \
} /* end hr scope */ \
/*
 * SS_SLSERVER_ON_INTERFACE_FTCALL_HR 
 *	Fault tolerant call on an interface from srvInfoObj
 */
#define SS_SLSERVER_ON_INTERFACE_FTCALL_HR(interfaceType, srvInfoObj, func, plist, hr) \
{ /* begin hr scope */ \
	bool __retry; \
	bool __connected = true; \
	do { \
		try \
		{ \
			__retry = false; \
			HRESULT connectHR = LicenseServerError::EHR_CLIENT_TIMEOUT; \
			if (!__connected ) \
			{ \
				connectHR = srvInfoObj.Reconnect(); \
				ReConnect(&srvInfoObj); \
			} \
			if (__connected || SUCCEEDED(connectHR)) \
			{ \
				/* Query for interfaceType */ \
				##interfaceType* pLocalInterface = NULL; \
				hr = srvInfoObj.LicenseServer->QueryInterface(__uuidof(##interfaceType), (void**)&pLocalInterface); \
				if(FAILED(hr)) throw hr; \
				hr = pLocalInterface->##func##plist; \
				pLocalInterface->Release(); \
				if(FAILED(hr)) throw _com_error(hr); \
			} \
			else \
				throw _com_error(connectHR); \
		} \
		catch (_com_error& e) \
		{ \
			__connected = false; \
			if (SS_RPC_FAILED(e.Error())) \
			{ \
				if (SUCCEEDED(srvInfoObj.Reconnect())) \
				{ \
					ReConnect(&srvInfoObj); \
					__retry = true; \
					__connected = true; \
				} \
			} \
			if (!__retry) \
				hr = e.Error(); \
				/*throw e; /* Pass to outer catch block. */ \
		} \
	} while (__retry); \
} /* end scope */ \

#define SS_GENERATE_AND_DISPATCH_MESSAGE(MessageValue, MessageType, MessageErrorCode, MessageId) \
{ /* begin scope */ \
	SS_GENERATE_AND_DISPATCH_MESSAGE_WITH_SERVER(L"", MessageValue, MessageType, MessageErrorCode, MessageId) \
} /* end scope */ \

#define SS_GENERATE_AND_DISPATCH_MESSAGE_WITH_SERVER(LicenseServer, MessageValue, MessageType, MessageErrorCode, MessageId) \
{ /* begin scope */ \
	static const int MAX_MESSAGE_SIZE = 0x2000; \
	wchar_t message[MAX_MESSAGE_SIZE]; \
	_snwprintf_s(message, sizeof(message)/sizeof(wchar_t), MessageValue); \
	message[MAX_MESSAGE_SIZE-1] = 0; \
	/* convert the time_t in to a variant date */ \
	time_t timestamp = time(0); \
	_variant_t vtTimestamp; \
	vtTimestamp = TimeHelper::TimeTToVariant(timestamp, false); \
	DispatchLicenseMessage(_bstr_t(LicenseServer), _bstr_t(L""), MessageType, MessageErrorCode, vtTimestamp, _bstr_t(message), MessageId); \
} /* end scope */ \

#define SS_GENERATE_NOW_TO_VARIANT_TIME_DATE(vtTimestamp) \
{ /* begin scope */ \
	/* convert the time_t in to a variant date */ \
	vtTimestamp = TimeHelper::TimeTToVariant(time(0), false); \
} /* end scope */ \

#define SS_CONVERT_LICENSE_HR_TO_LICENSE_MESSAGE(hr, wcs_licensing_message_buffer, int_lic_message, bstr_server) \
{ /* begin scope */ \
	if(hr == LicenseServerError::EHR_KEY_BASE_NOT_MATCHING) \
	{ \
		int_lic_message = MessageBaseKeysNotMatchingModuleByModule;	\
		swprintf_s(	wcs_licensing_message_buffer, \
					sizeof(wcs_licensing_message_buffer)/sizeof(wchar_t), \
					LicensingMessageStringTable[MessageBaseKeysNotMatchingModuleByModule], \
					(wchar_t*)bstr_server); \
	} \
	else if(hr == LicenseServerError::EHR_KEY_NOT_MATCHING_INSTANCES) \
	{ \
		int_lic_message = MessageKeysNotMatchingApplicationInstance; \
		swprintf_s(	wcs_licensing_message_buffer, \
				sizeof(wcs_licensing_message_buffer)/sizeof(wchar_t), \
				LicensingMessageStringTable[MessageKeysNotMatchingApplicationInstance], \
				(wchar_t*)bstr_server); \
	} \
	else if(hr == LicenseServerError::EHR_KEY_NO_FREE_APP_INSTANCE) \
	{ \
		int_lic_message = MessageKeysNoApplicationInstance; \
		swprintf_s(	wcs_licensing_message_buffer, \
				sizeof(wcs_licensing_message_buffer)/sizeof(wchar_t), \
				LicensingMessageStringTable[MessageKeysNoApplicationInstance], \
				(wchar_t*)bstr_server); \
	} \
	else if(hr == LicenseServerError::EHR_KEY_NO_BASE_KEY) \
	{ \
		int_lic_message = MessageKeysNoBaseKey; \
		swprintf_s(	wcs_licensing_message_buffer, \
				sizeof(wcs_licensing_message_buffer)/sizeof(wchar_t), \
				LicensingMessageStringTable[MessageKeysNoApplicationInstance], \
				(wchar_t*)bstr_server); \
	} \
	else \
	{ \
		swprintf_s(	wcs_licensing_message_buffer,  \
				sizeof(wcs_licensing_message_buffer)/sizeof(wchar_t), \
				L"%s", \
				LicenseServerError::GetErrorMessage(hr)); \
	} \
} /* end scope */ \

// CSolimarLicenseMgr

[
	coclass,
	threading(free),
	support_error_info("ISolimarLicenseMgr7"),
	vi_progid("SolimarLicenseManager.SolimarLicenseMgr"),
	progid("SolimarLicenseManager.SolimarLicenseM.1"),
	version(1.0),
	uuid("986A969C-F4C2-4EB7-85E6-F049201AEBB9"),
	helpstring("SolimarLicenseMgr Class")
]
class ATL_NO_VTABLE CSolimarLicenseMgr : 
	public ISolimarLicenseMgr7,
	public IObjectAuthentication,
	public ILicensingMessage,
	public ChallengeResponseHelper
{
public:
	CSolimarLicenseMgr();
	~CSolimarLicenseMgr();
	
	
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	
	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}
	
public:
	
	// IObjectAuthentication
	STDMETHOD(Challenge)(VARIANT vtChallenge, VARIANT *pvtResponse);
	STDMETHOD(GetChallenge)(VARIANT *pvtChallenge);
	STDMETHOD(PutResponse)(VARIANT vtResponse);
	
	// ISolimarLicenseMgr
    STDMETHOD(Connect)(BSTR server);
    STDMETHOD(Disconnect)();
	STDMETHOD(Initialize)(long product, long prod_ver_major, long prod_ver_minor, VARIANT_BOOL single_key, BSTR specific_single_key_ident, VARIANT_BOOL lock_keys, long auto_ui_level);
	STDMETHOD(ValidateLicense)(VARIANT_BOOL *license_valid);
	STDMETHOD(ModuleLicenseTotal)(long module_id, long *count);
	STDMETHOD(ModuleLicenseInUse)(long module_id, long *count);
	STDMETHOD(ModuleLicenseObtain)(long module_id, long count);
	STDMETHOD(ModuleLicenseRelease)(long module_id, long count);

	// ISolimarLicenseMgr2
	STDMETHOD(ModuleLicenseSerialNumbers)(long module_id, VARIANT *pvtSerialNumberList);

	// ISolimarLicenseMgr3
	STDMETHOD(ModuleLicenseCounterDecrement)(long module_id, long license_count);

	// ISolimarLicenseMgr4
	STDMETHOD(Initialize2)(long product, long prod_ver_major, long prod_ver_minor, VARIANT_BOOL single_key, BSTR specific_single_key_ident, VARIANT_BOOL lock_keys, long auto_ui_level, unsigned long grace_period_minutes);

	// ISolimarLicenseMgr5
	STDMETHOD(Connect2)(BSTR server, VARIANT_BOOL bUseOnlySharedLicenses, VARIANT_BOOL bUseAsBackup);
	STDMETHOD(Initialize3)(BSTR application_instance, long product, long prod_ver_major, long prod_ver_minor, VARIANT_BOOL single_key, BSTR specific_single_key_ident, VARIANT_BOOL lock_keys, long auto_ui_level, unsigned long grace_period_minutes, VARIANT_BOOL b_app_instance_lock_key, VARIANT_BOOL b_bypass_remote_key_restriction);
	STDMETHOD(GetVersionLicenseManager) (long* p_ver_major, long* p_ver_minor, long* p_ver_build);
	STDMETHOD(GetVersionLicenseServer) (BSTR server, long* p_ver_major, long* p_ver_minor, long* p_ver_build);
	STDMETHOD(ConnectByProduct)(long product, VARIANT_BOOL bUseSharedLicenseServers);
	STDMETHOD(KeyProductExists)(long product, long prod_ver_major, long prod_ver_minor, VARIANT_BOOL *p_bool_key_product_exists);

	// ISolimarLicenseMgr6
	STDMETHOD(ModuleLicenseInUse_ByApp)(long module_id, long *count);

	// ISolimarLicenseMgr7
	STDMETHOD(Connect3)(BSTR server, long connectionFlags);
	STDMETHOD(SoftwareLicense_Initialize)(BSTR application_instance, long product, long prod_ver_major, long prod_ver_minor, long auto_ui_level, unsigned long grace_period_minutes);
	STDMETHOD(SoftwareLicense_InitializeViewOnly)(BSTR application_instance, long product, long prod_ver_major, long prod_ver_minor);
	STDMETHOD(GetInfoByProduct)(long product, VARIANT_BOOL use_shared_licenses_servers, BSTR* p_server, BSTR* p_backup_server, VARIANT_BOOL* p_bool_use_test_dev_licensing);

	// ILicensingMessage
	STDMETHOD(GetLicenseMessageList)(VARIANT_BOOL clear_messages, VARIANT *pvtMessageList);
	STDMETHOD(DispatchLicenseMessageList)(VARIANT_BOOL clear_messages);

	static bool InitTimerThreadCB(EInitReason reason, void *pArg) {return static_cast<CSolimarLicenseMgr*>(pArg)->InitTimerThread(reason);}
private:

	/* InitPoolCleanerThread()
	Initialize the pool cleaner thread for COM. */
	bool InitTimerThread(EInitReason reason)
	{
		if (reason == irStartup)
			CoInitializeEx(NULL, COINIT_MULTITHREADED); 
		else if (reason == irShutdown) 
			CoUninitialize();
		return true;
	}
	
	static BYTE challenge_key_manager_thisauthuser_public[];
	static BYTE challenge_key_manager_userauththis_private[];
	static BYTE challenge_key_server_thisauthuser_private[];
	static BYTE challenge_key_server_userauththis_public[];
	
	typedef std::map<long, long> ModuleLicenseMap;

	class KeyInfo
	{
	public:
		KeyInfo();
		KeyInfo(const KeyInfo &k);

		ModuleLicenseMap licenses_total;
		ModuleLicenseMap licenses_allocated;
		ModuleLicenseMap licenses_inuse;
		ModuleLicenseMap licenses_inuse_byapp;
		
		bool KeyPresent;
		bool KeyActive;
		bool KeyValid;
		bool KeyLocked;
		bool KeyObtained;
		
		// keep track of trial key information
		bool KeyTrialInfoInitialized;
		bool KeyIsTrial;
		long KeyTrialHoursLeft;
		long KeyExpiresDaysLeft;
	};

	class SoftwareLicenseInfo
	{
	public:
		SoftwareLicenseInfo();
		SoftwareLicenseInfo(const SoftwareLicenseInfo &s);

		ModuleLicenseMap licenses_total;
		ModuleLicenseMap licenses_allocated;
		ModuleLicenseMap licenses_inuse;

		bool SoftwareLicenseValid;
	};

	class ServerInfo
	{
	public:
		ServerInfo();
		ServerInfo(const ServerInfo &s);
		ServerInfo(_bstr_t servername, 
			bool useOnlySharedLicenses, //Use Shared Licensing when accessing Protection Keys
			bool useSoftwareLicensing,
			bool useOnlyLicenseViewer,
			GITPtr<ISolimarLicenseSvr4> pILicenseServer);

		~ServerInfo();

		_bstr_t name;
		bool bUseOnlySharedLicenses;
		bool bUseSoftwareLicensing;	//Not used yet, maybe not needed
		bool bUseOnlyLicenseViewer;
		typedef std::map<_bstr_t, KeyInfo> KeyList;
		SoftwareLicenseInfo software_license;
		KeyList keys;

		GITPtr<ISolimarLicenseSvr4> LicenseServer;

		HRESULT Reconnect();

		HRESULT Disconnect()
		{
			return S_OK;
		}
	};
		
	
	enum {
		UI_IGNORE =				0x00,
		UI_LEVEL_ALL =			0x01,
		UI_LEVEL_RESPONSE =		0x02,
		UI_LEVEL_CRITICAL =		0x04,
		UI_STYLE_DIALOG =		0x10,
		UI_STYLE_EVENT_LOG =	0x20,
	};

	enum {
		CF_NONE =	0x00,
		CF_BACKUP_SERVER = 0x01,
		CF_ONLY_SHARED_LICENSE = 0x02,
		CF_SOFTWARE_LICENSING = 0x04,
		CF_ONLY_LICENSE_VIEWER = 0x08,
	};
	
	DWORD m_ui_level;
	
	HANDLE ServerListLock;
	HANDLE MessageListLock;
	HANDLE GracePeriodLock;
	
	//Call after an RPC error, clear all allocated licenses so they will have to be re-obtained
	HRESULT ReConnect(ServerInfo* pSrvInfoObj);

	HRESULT Initialize_Internal(VARIANT_BOOL b_configure_for_software_licenses, BSTR application_instance, long product, long prod_ver_major, long prod_ver_minor, VARIANT_BOOL single_key, BSTR specific_single_key_ident, VARIANT_BOOL lock_keys, long auto_ui_level, unsigned long grace_period_minutes, VARIANT_BOOL b_app_instance_lock_key, VARIANT_BOOL b_bypass_remote_key_restriction, VARIANT_BOOL b_view_licenses_only);

	// refresh the list of the keys on the currently connected servers
	HRESULT RefreshLicensingFromLicServers(bool _bLogError = false);

	HRESULT RefreshKeyListFromLicServers(bool _bLogError = false);

	HRESULT RefreshSoftwareLicenseFromLicServers(bool _bLogError = false);

	// if m_bLockKeyByAppInstance then makes sure that all the keys types match up module by module.
	// also verifies that each key configuration (unique by the modules the keys are licensed for) all have the same number of application instances
	HRESULT ValidateKeyList(ServerInfo* pServerInfo, VARIANT *pvtKeyList, long count);

	// locks application instance with each keys configuration (unique by the modules the keys are licensed for)
	// adds locked keys to the cache
	HRESULT LockOneOfEachKeyConfiguration(ServerInfo* pServerInfo, VARIANT *pvtKeyList, long count, bool bLogError);

	// associates application instance with a base key
	// adds all keys to the cache
	HRESULT AssociateAppInstanceToBaseKey(ServerInfo* pServerInfo, VARIANT *pvtKeyList, long count, bool bLogError);

	_bstr_t GetAppInstanceFromKey(ServerInfo* pServerInfo, BSTR bstrKeyIdent, bool bLogError);

	// adds an individual key to the cache
	HRESULT AddKeyToCache(ServerInfo* pServerInfo, BSTR bstrKeyIdent, bool bLogError);

	// removes all keys from the cache, and disassociates all keys with the given Application Instance to empty...
	HRESULT FreeAllKeys(ServerInfo* pServerInfo, VARIANT *pvtKeyList, long count, bool bLogError);

	HRESULT AssociateAppInstanceToSoftwareServer(ServerInfo* pServerInfo, _bstr_t appInstance, bool bLogError);
	HRESULT FreeAllSoftwareLicenseFromCache(ServerInfo* pServerInfo, bool bLogError);

	// Sets the appropiate modules on keys to be unlimited.  Calculate: #of base keys X unlimited number for modules.
	HRESULT SetUnlimitedModulesOnKeys(ServerInfo* pServerInfo, VARIANT *pvtKeyList, long count, bool bLogError);

	// refresh ApplicationInstance InUse list
	HRESULT RefreshApplicationInstanceInUseCache(ServerInfo* pServerInfo, bool bLogError);

	// checks that all licenses checked out are accounted for by some key
	HRESULT ValidateLicenseCache(ModuleLicenseMap &outstanding_licenses);

	// checks if there is valid licensing, 2nd Param tells whether to use backup servers or enter Grace/Violation Period if FAILS
	HRESULT ValidateLicenseInternal(VARIANT_BOOL *license_valid, bool use_back_up_on_error, bool bLogError);

	// re-allocates any unallocated or invalidated licenses (if possible)
	HRESULT ReallocateLicenses();

	// checks if licenses are valid, if not, an attempt to reallocate licenses is made
	HRESULT RefreshLicenses();

	// removes any keys from the cache that are not present and have no obtained licenses
	HRESULT RemoveObsoleteKeysFromCache();

	HRESULT ModuleLicenseTotalInternal(long module_id, long *count);

	// Calculates the modules in use by the given Connection to the License Servers, not by the 
	// Application Instance.
	HRESULT ModuleLicenseInUseInternal(long module_id, long *count);

	// Calculates the modules in use by the given Application Instance.
	// For Software License & Protection Key Licensing, goes to the License Server for all the process that are connected
	HRESULT ModuleLicenseInUse_ByAppInternal(long module_id, long *count);

	// attempts to allocate licenses on the known-good keys in the cache
	//HRESULT ObtainLicensesInternal(ModuleLicenseMap &licenses);
	HRESULT ObtainLicensesInternal(long module_id, long license_count);

	// attempts to deallocate licenses on keys that have 
	HRESULT ReleaseLicensesInternal(long module_id, long license_count);

	// cycles through all servers in server list seeing if they have a newer
	//software spec than the currently loaded one.  Should call only once probably...
	//Also, only retrieves the Lic_ProductSoftwareSpecAttribs for the product the
	//LicenseManager is initialized for...
	HRESULT RefreshSoftwareSpec(Lic_PackageAttribs::Lic_SoftwareSpecAttribs* pSoftwareSpec);


	// use to determine license scheme (Software License vs Protection Key)
	// Side Effect - sets pServerInfo->useSoftwareLicensing
	//HRESULT RefreshLicensingSchemeFromLicenseServer(ServerInfo* pServerInfo, VARIANT *pvtKeyList, long count, bool bLogError);
	HRESULT LicenseServerContainsProtectionKeyLicensingForProduct(long product_id, ServerInfo* pServerInfo, bool* pbHasLicensing);
	HRESULT LicenseServerContainsSoftwareLicensingForProduct(long product_id, ServerInfo* pServerInfo, bool* pbHasLicensing);

	//Cycles through all the Software Licenses that has the productID, and returns only the first error encountered
	HRESULT LicenseServerSoftwareLicenseStatusFirstErrorForProduct(long product_id, ServerInfo* pServerInfo);

	// adds a message to the list of unretrieved messages
	HRESULT AddLicensingMessage(LicensingMessage &message);
	
	bool IsLocalMachine(_bstr_t server);


	LicensingMessageList licensing_message_cache;
	LicensingSoftwareMessageList licensing_software_message_cache;
	
	typedef std::map<_bstr_t,ServerInfo> ServerList;
	typedef std::map<_bstr_t,bool> KeyIdentList;	

	bool ManagesKey(_bstr_t key_ident);
	bool ProductKey(_bstr_t key_ident);
	
	APCTimer *HeartbeatThread;
	static void HeartbeatThreadFunction(void* pvThis);
	void SendHeartbeat();
	
	Lic_PackageAttribs::Lic_SoftwareSpecAttribs m_softwareSpec;
	KeySpec m_keyspec;
	ServerList m_servers;			// List of Primary License Servers, the current software Licensing Scheme only supports a single item in the list
	ServerList m_backupServers;	// List of Backup License Servers, the current software Licensing Scheme only supports a single item in the list
	bool m_bBypassRemoteKeyRestrictions;
	bool m_bUsingBackupServers;	// Licensing is using Backup License Servers for its licensing needs
	bool m_bInViolationPeriod;		// Licensing is in violation
	std::list<std::wstring> m_licenseMsgEventLogCache;				// Cache of license messages written to the event log, so event log will not get flooded with the same message
	KeyIdentList m_productkeys;

	bool m_single_key, m_lock_keys, m_initialized;	
	bool m_bViewLicenseOnly;
	_bstr_t m_specific_single_key_ident;
	_bstr_t m_current_single_key;
	long m_product, m_prod_ver_major, m_prod_ver_minor;
	long m_productKeyID;	//For backward compatibility of new products working with old protection keys
	ModuleLicenseMap m_allocated_licenses;
	time_t m_dtGracePeriodStart;
	unsigned long m_dtGracePeriod;	//in minutes
	bool m_bLockKeyByAppInstance;
	_bstr_t m_applicationInstance;
	_bstr_t m_applicationInstanceKey;	//For backward compatibility of new products working with old protection keys
	_bstr_t m_headerInformation;	//contains header information important for event log messages.

	//bool m_bConfiguredForSoftwareLicense; //either configured for ProtectionKeys or Software Licenses.
	
	//Side effect, sets m_productKeyID & m_applicationInstanceKey & bUseOnlySharedLicenses in ServerInfo objs
	void InternalCalculateLegacyProtectionKeyInfo(long _productID);

	time_t m_dtRefreshLicenses;

	bool GracePeriodHadEnded();
	bool GracePeriodHasStarted();
	void StartGracePeriod();
	void StopGracePeriod();
	std::map<unsigned int, int> appInstanceInUseModuleCacheMap;

	
		
	// default key event/message handlers
	static bool MessageQualifiesForAutoDispatch(DWORD ui_level, long message_type);
	static bool isAutoUiStyleDialog(DWORD ui_level);
	static bool isAutoUiStyleEventLog(DWORD ui_level);
	void DispatchLicenseMessage(BSTR key_ident, long message_type, long error, VARIANT vtTimestamp, BSTR message, long message_id);
	void DispatchLicenseMessage(BSTR license_server, BSTR key_ident, long message_type, long error, VARIANT vtTimestamp, BSTR message, long message_id);
	void KeyMessageShowDialog(BSTR key_ident, unsigned int message_type, HRESULT error, VARIANT vtTimestamp, BSTR message);
	void KeyMessageWriteEventLog(BSTR key_ident, unsigned int message_type, HRESULT error, VARIANT vtTimestamp, BSTR message, long message_id);
	
	typedef struct {
		_bstr_t caption, message;
	} LicenseDialogParams;
	HRESULT ShowLicenseDialog(_bstr_t caption, _bstr_t message);
	
	static BOOL CALLBACK LicenseDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
};

