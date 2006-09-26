#ifndef _LicensingWrapper_h_97082379082347905__
#define _LicensingWrapper_h_97082379082347905__

#pragma warning(disable:4786)


#include <string>
#include "..\SolimarLicenseManager\_SolimarLicenseManager.h"
#include "..\SolimarLicenseServer\KeySpec.h"
#include "..\common\ChallengeResponseHelper.h"
#include "..\common\LicensingMessage.h"

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

class LicensingWrapper : public ChallengeResponseHelper
{
public:
	LicensingWrapper();
	LicensingWrapper(const LicensingWrapper &o);
	LicensingWrapper& operator=(const LicensingWrapper &o);
	virtual ~LicensingWrapper();
	bool Connect(std::wstring server);

	enum {
		UI_IGNORE =				0x00,
		UI_LEVEL_ALL =			0x01,
		UI_LEVEL_RESPONSE =		0x02,
		UI_LEVEL_CRITICAL =		0x04,
		UI_STYLE_DIALOG =		0x10,
		UI_STYLE_EVENT_LOG =	0x20,
	};
	
	typedef void (*LicenseMessageCallbackPtr)(void* pContext, const wchar_t* key_ident, unsigned int message_type, HRESULT error, VARIANT *pvtTimestamp, const wchar_t* message);
	
	bool Initialize(long product, long prod_ver_major, long prod_ver_minor, bool single_key, std::wstring &specific_single_key_ident, bool lock_keys, DWORD ui_level = UI_IGNORE);
	//bool GetLicenseMessageList(LicensingMessageList &message_list);
	bool RegisterMessageCallback(void* pContext, LicenseMessageCallbackPtr LicenseMessageCallback);
	bool UnregisterMessageCallback();
	bool ModuleLicenseTotal(long module, long* license_count);
	bool ModuleLicenseInUse(long module, long* license_count);
	bool ModuleLicenseObtain(long module, long license_count);
	bool ModuleLicenseRelease(long module, long license_count);
	bool ModuleLicenseCounterDecrement(long module, long license_count);
	bool ValidateLicense();
	
	long LookupProductID(std::wstring product);
	long LookupProductID(std::string product);
	long LookupModuleID(long product_id, std::wstring module);
	long LookupModuleID(long product_id, std::string module);
	long LookupHeaderID(std::wstring header);
	long LookupHeaderID(std::string header);
	
	static KeySpec keyspec;
	
private:
	
	static BYTE challenge_key_manager_userauththis_public[];
	static BYTE challenge_key_manager_thisauthuser_private[];
	
	HANDLE m_MessageDispatchThread;
	static DWORD WINAPI MessageDispatchThreadProc(LPVOID pWrapper);
	HRESULT MessageDispatchThreadProc();
	
	HANDLE m_LicenseValidityThread;
	static DWORD WINAPI LicenseValidityThreadProc(LPVOID pWrapper);
	HRESULT LicenseValidityThreadProc();
	
	// synchronizing access to member variables
	HANDLE m_MemberLock;
	HANDLE m_ThreadKillEvent;
	
	void* m_license_message_callback_context;
	LicenseMessageCallbackPtr m_license_message_callback;
	
	ISolimarLicenseMgr4 *pLicenseManager;
	ILicensingMessage *pLicenseManagerMessages;	

	std::wstring StringToWstring(const std::string &s);
};

};

#endif //_LicensingWrapper_h_97082379082347905__
