// SolimarLicenseMgr.h : Declaration of the CSolimarLicenseMgr

#pragma once

#include "stdafx.h"
#include "resource.h"       // main symbols
#include "..\SolimarLicenseServer\KeySpec.h"
#include "..\common\IObjectAuthentication.h"
#include "..\common\ILicensingMessage.h"
#include "..\common\ChallengeResponseHelper.h"

#import "..\SolimarLicenseServer\_SolimarLicenseServer.tlb" no_namespace raw_interfaces_only exclude("IObjectAuthentication","ILicensingMessage")
//#include "..\SolimarLicenseServer\SolimarLicenseSvr.h"
//#include "..\SolimarLicenseServer\_SolimarLicenseServer.h"


#include "..\common\apctimer.h"

#include <comutil.h>
#include <vector>
#include <map>

#include "../common/InProcPtr.h"

// ISolimarLicenseMgr
[
	object,
	uuid("E76FF720-125E-4FE3-AAF4-05E8CBF6040A"),
	dual,	helpstring("ISolimarLicenseMgr Interface"),
	pointer_default(unique)
]
__interface ISolimarLicenseMgr : IDispatch
{
	// Connects to a key monitor server.
	// Can be called multiple times in succession to connect to and aggregate multiple key servers
    [id(1),helpstring("method Connect")] HRESULT Connect([in] BSTR server);
	// Disconnects from all servers, and releases all licenses
	[id(2),helpstring("method Disconnect")] HRESULT Disconnect();
	
	// Sets the product and product version required. Call this function after calling Connect()
	[id(3),helpstring("method Initialize")] HRESULT Initialize([in] long product, [in] long prod_ver_major, [in] long prod_ver_minor, [in] VARIANT_BOOL single_key, [in] BSTR specific_single_key_ident, [in] VARIANT_BOOL lock_keys, [in] long auto_ui_level);	
	// Checks whether the licenses obtained so far are still valid
	[id(4),helpstring("method ValidateLicense")] HRESULT ValidateLicense([out,retval] VARIANT_BOOL *license_valid);
	
	// Licensing info
	[id(6),helpstring("method ModuleLicenseTotal")] HRESULT ModuleLicenseTotal([in] long module_id, [out,retval] long *count);
	[id(7),helpstring("method ModuleLicenseInUse")] HRESULT ModuleLicenseInUse([in] long module_id, [out,retval] long *count);
	[id(8),helpstring("method ModuleLicenseObtain")] HRESULT ModuleLicenseObtain([in] long module_id, [in] long count);
	[id(9),helpstring("method ModuleLicenseRelease")] HRESULT ModuleLicenseRelease([in] long module_id, [in] long count);
	
};

// CSolimarLicenseMgr

[
	coclass,
	threading("free"),
	support_error_info("ISolimarLicenseMgr"),
	vi_progid("SolimarLicenseManager.SolimarLicenseMgr"),
	progid("SolimarLicenseManager.SolimarLicenseM.1"),
	version(1.0),
	uuid("986A969C-F4C2-4EB7-85E6-F049201AEBB9"),
	helpstring("SolimarLicenseMgr Class")
]
class ATL_NO_VTABLE CSolimarLicenseMgr : 
	public ISolimarLicenseMgr,
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
	
	// ILicensingMessage
	STDMETHOD(GetLicenseMessageList)(VARIANT *pvtMessageList);
	
private:
	
	//typedef void (*LicenseMessageCallbackPtr)(void* pContext, BSTR key_ident, unsigned int message_type, HRESULT error, VARIANT vtTimestamp, BSTR message);	
	//_COM_SMARTPTR_TYPEDEF(ISolimarLicenseSvr,__uuidof(ISolimarLicenseSvr));
	
	static BYTE challenge_key_manager_thisauthuser_public[];
	static BYTE challenge_key_manager_userauththis_private[];
	static BYTE challenge_key_server_thisauthuser_private[];
	static BYTE challenge_key_server_userauththis_public[];
	
	class ServerInfo
	{
	public:
		ServerInfo();
		ServerInfo(const ServerInfo &s);
		ServerInfo(_bstr_t servername, ISolimarLicenseSvrPtr pILicenseServer);
		~ServerInfo();

		_bstr_t name;
		typedef std::vector<_bstr_t> KeyList;
		KeyList keys;
		
		ISolimarLicenseSvrPtr LicenseServer;
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
	
	typedef std::map<_bstr_t,ServerInfo> ServerList;
	bool ManagesKey(_bstr_t key_ident);
	
	APCTimer *HeartbeatThread;
	static void HeartbeatThreadFunction(void* pvThis);
	void SendHeartbeat();
	
	KeySpec m_keyspec;
	ServerList m_servers;
	bool m_single_key, m_lock_keys, m_initialized;	
	_bstr_t m_specific_single_key_ident;
	
	// user defined callback used by ILicensingMessage
	//void* m_license_message_callback_context;
	//LicenseMessageCallbackPtr m_license_message_callback;
	
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

