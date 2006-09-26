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

#import "..\SolimarLicenseServer\_SolimarLicenseServer.tlb" no_namespace raw_interfaces_only exclude("IObjectAuthentication","ILicensingMessage")
//#include "..\SolimarLicenseServer\SolimarLicenseSvr.h"
//#include "..\SolimarLicenseServer\_SolimarLicenseServer.h"


#include "..\common\apctimer.h"

#include <comutil.h>
#include <vector>
#include <map>

#include "..\common\InProcPtr.h"
#include "ISolimarLicenseMgr.h"
#include "..\common\LicensingMessage.h"


/*
 * SS_SLSERVER_FTCALL 
 *	Fault tolerant call for SolimarLicenseServer
 */
#define SS_SLSERVER_FTCALL(srvInfoObj, func, plist) \
	{ /* begin hr scope */ \
	HRESULT hr; \
	SS_SLSERVER_FTCALL_HR(srvInfoObj func, plist, hr) \
	} /* end hr scope */ \

/*
 * SS_SLSERVER__HR 
 *	Fault tolerant call which sets an HRESULT.
 */
#define SS_SLSERVER_FTCALL_HR(srvInfoObj, func, plist, hr) \
	bool __retry; \
	bool __connected = true; \
	do { \
		try \
		{ \
			__retry = false; \
			HRESULT connectHR = LicenseServerError::EHR_CLIENT_TIMEOUT; \
			if (__connected || SUCCEEDED(connectHR = srvInfoObj.Connect())) \
			{ \
				hr = srvInfoObj.LicenseServer->##func##plist; \
				srvInfoObj.Disconnect(); \
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
 			srvInfoObj.Disconnect(); \
			if (SS_RPC_FAILED(e.Error())) \
			{ \
				if (SUCCEEDED(srvInfoObj.Connect())) \
				{ \
					__retry = true; \
					__connected = true; \
				} \
			} \
			if (!__retry) \
				throw e; /* Pass to outer catch block. */ \
		} \
	} while (__retry);


// CSolimarLicenseMgr

[
	coclass,
	threading("free"),
	support_error_info("ISolimarLicenseMgr4"),
	vi_progid("SolimarLicenseManager.SolimarLicenseMgr"),
	progid("SolimarLicenseManager.SolimarLicenseM.1"),
	version(1.0),
	uuid("986A969C-F4C2-4EB7-85E6-F049201AEBB9"),
	helpstring("SolimarLicenseMgr Class")
]
class ATL_NO_VTABLE CSolimarLicenseMgr : 
	public ISolimarLicenseMgr4,
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

	// ILicensingMessage
	STDMETHOD(GetLicenseMessageList)(VARIANT_BOOL clear_messages, VARIANT *pvtMessageList);
	STDMETHOD(DispatchLicenseMessageList)(VARIANT_BOOL clear_messages);
	
private:
	
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

	class ServerInfo
	{
	public:
		ServerInfo();
		ServerInfo(const ServerInfo &s);
		ServerInfo(_bstr_t servername, ISolimarLicenseSvrPtr pILicenseServer);
		~ServerInfo();

		_bstr_t name;
		typedef std::map<_bstr_t, KeyInfo> KeyList;
		KeyList keys;
		
		ISolimarLicenseSvrPtr LicenseServer;

		HRESULT Connect();

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
	
	DWORD m_ui_level;
	
	HANDLE ServerListLock;
	HANDLE MessageListLock;
	HANDLE GracePeriodLock;
	
	// refresh the list of the keys on the currently connected servers
	HRESULT RefreshKeyList(bool _bLogError = false);

	// checks that all licenses checked out are accounted for by some key
	HRESULT ValidateLicenseCache(ModuleLicenseMap &outstanding_licenses);

	// re-allocates any unallocated or invalidated licenses (if possible)
	HRESULT ReallocateLicenses();

	// checks if licenses are valid, if not, an attempt to reallocate licenses is made
	HRESULT RefreshLicenses();

	// removes any keys from the cache that are not present and have no obtained licenses
	HRESULT RemoveObsoleteKeysFromCache();

	// attempts to allocate licenses on the known-good keys in the cache
	//HRESULT ObtainLicensesInternal(ModuleLicenseMap &licenses);
	HRESULT ObtainLicensesInternal(long module_id, long license_count);

	// attempts to deallocate licenses on keys that have 
	HRESULT ReleaseLicensesInternal(long module_id, long license_count);

	// adds a message to the list of unretrieved messages
	HRESULT AddLicensingMessage(LicensingMessage &message);
	
	LicensingMessageList licensing_message_cache;
	
	typedef std::map<_bstr_t,ServerInfo> ServerList;
	bool ManagesKey(_bstr_t key_ident);
	
	APCTimer *HeartbeatThread;
	static void HeartbeatThreadFunction(void* pvThis);
	void SendHeartbeat();
	
	KeySpec m_keyspec;
	ServerList m_servers;
	bool m_single_key, m_lock_keys, m_initialized;	
	_bstr_t m_specific_single_key_ident;
	_bstr_t m_current_single_key;
	long m_product, m_prod_ver_major, m_prod_ver_minor;
	ModuleLicenseMap m_allocated_licenses;
	time_t m_dtGracePeriodStart;
	unsigned long m_dtGracePeriod;	//in minutes

	bool InViolationPeriod();
	bool GracePeriodHasStarted();
	void StartGracePeriod();
	void StopGracePeriod();
		
	// default key event/message handlers
	static bool MessageQualifiesForAutoDispatch(DWORD ui_level, long message_type);
	static bool isAutoUiStyleDialog(DWORD ui_level);
	static bool isAutoUiStyleEventLog(DWORD ui_level);
	void DispatchLicenseMessage(BSTR key_ident, long message_type, long error, VARIANT vtTimestamp, BSTR message);
	void KeyMessageShowDialog(BSTR key_ident, unsigned int message_type, HRESULT error, VARIANT vtTimestamp, BSTR message);
	void KeyMessageWriteEventLog(BSTR key_ident, unsigned int message_type, HRESULT error, VARIANT vtTimestamp, BSTR message);
	
	typedef struct {
		_bstr_t caption, message;
	} LicenseDialogParams;
	HRESULT ShowLicenseDialog(_bstr_t caption, _bstr_t message);
	
	static BOOL CALLBACK LicenseDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
};

