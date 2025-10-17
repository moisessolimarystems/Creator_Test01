#pragma once

#include <comdef.h>
#include <string>
#include <map>

#include "KeySpec.h"
#include "ProtectionKey.h"
#include "ProtectionKey_Version1.h"
#include "RainbowDriver.h"
#include "..\common\SafeMutex.h"
#include "..\common\apctimer.h"

//#include "..\common\ILicensingMessage.h"
#include "KeyMessages.h"
#include "..\common\LicensingMessage.h"
#include "..\common\CryptoHelper.h"
#include "..\common\LicAttribsCPP\Lic_KeyAttribs.h"

//#include "USBNotification.h"

class KeyServer// : public USBNotification
{
public:
	
	KeyServer();
	~KeyServer();

	HRESULT Initialize(RainbowDriver* pDriver);

	HRESULT ResynchronizeKeys(bool bForceRefresh = false);
	
	// Top level functions
	HRESULT AddApplicationInstance(BSTR license_id, BSTR key_ident, BSTR application_instance, VARIANT_BOOL b_app_instance_lock_key);
	HRESULT RemoveApplicationInstance(BSTR license_id, BSTR key_ident, BSTR application_instance);
	HRESULT GetApplicationInstanceList(BSTR license_id, BSTR key_ident, VARIANT *pvtAppInstanceList);

	HRESULT KeyEnumerate(VARIANT *keylist);
	HRESULT EnterPassword(BSTR password);
	HRESULT EnterPasswordPacket(VARIANT vtPasswordPacket, BSTR *verification_code);
	HRESULT GenerateBasePassword(long customer_number, long key_number, BSTR *password);
	HRESULT GenerateApplicationInstancePassword(long customer_number, long key_number, long license_count, long password_number, BSTR *password);
	HRESULT GenerateVersionPassword(long customer_number, long key_number, long ver_major, long ver_minor, BSTR *password);
	HRESULT GenerateExtensionPassword(long customer_number, long key_number, long extend_days, long extension_num, BSTR *password);
	HRESULT GenerateModulePassword(long customer_number, long key_number, long product_ident, long module_ident, long license_count, BSTR *password);
	HRESULT GenerateModulePassword(long customer_number, long key_number, long product_ident, long module_ident, long license_count, BSTR *password, long password_number);
	
	// Password packet management
	HRESULT PasswordPacketInitialize(BSTR license_id);
	HRESULT PasswordPacketAppendPassword(BSTR license_id, VARIANT vtExpires, BSTR password);
	HRESULT PasswordPacketFinalize(BSTR license_id);
	HRESULT PasswordPacketGetPacket(BSTR license_id, VARIANT *pvtPacketData);
	HRESULT PasswordPacketGetVerificationCode(BSTR license_id, BSTR *verification_code);

	// Key specific functions
	HRESULT KeyTrialExpires(BSTR key_ident, VARIANT *expire_date);
	HRESULT KeyTrialHours(BSTR key_ident, long *trial_hours);
	HRESULT KeyIsActive(BSTR key_ident, VARIANT_BOOL *key_active);
	HRESULT KeyIsProgrammed(BSTR key_ident, VARIANT_BOOL *key_programmed);
	HRESULT KeyHeaderQuery(BSTR key_ident, long header_ident, VARIANT *value);
	HRESULT KeyIsPresent(BSTR key_ident, VARIANT_BOOL *key_present);
	
	// License locking support
	// locking a key prevents it from being used by others until it is unlocked
	HRESULT KeyLock(BSTR license_id, BSTR key_ident);
	HRESULT KeyUnlock(BSTR license_id, BSTR key_ident);
	// indicates that if the key is trial, to start using up hours until it is released
	HRESULT KeyObtain(BSTR license_id, BSTR key_ident);
	HRESULT KeyRelease(BSTR license_id, BSTR key_ident);
	
	// Module information and locking
	HRESULT KeyValidateLicense(BSTR license_id, BSTR key_ident, VARIANT_BOOL *license_valid);
	HRESULT KeyModuleEnumerate(BSTR key_ident, VARIANT *key_module_list);
	HRESULT KeyModuleQuery(BSTR key_ident, long module_ident, VARIANT *vtValue);
	HRESULT KeyModuleLicenseTotal(BSTR license_id, BSTR key_ident, long module_ident, long* license_count);
	HRESULT KeyModuleLicenseInUse(BSTR license_id, BSTR key_ident, long module_ident, long* license_count);
	HRESULT KeyModuleLicenseInUse_ByApp(BSTR license_id, BSTR key_ident, long module_ident, long* license_count);
	HRESULT KeyModuleLicenseObtain(BSTR license_id, BSTR key_ident, long module_ident, long license_count);
	HRESULT KeyModuleLicenseRelease(BSTR license_id, BSTR key_ident, long module_ident, long license_count);
	HRESULT KeyModuleLicenseCounterDecrement(BSTR license_id, BSTR key_ident, long module_ident, long license_count);
	HRESULT LicenseReleaseAll(BSTR license_id);
	HRESULT KeyModuleInUse(BSTR key_ident, long module_ident, long* license_count);
	HRESULT KeyModuleLicenseUnlimited(BSTR license_id, BSTR key_ident, long module_ident, VARIANT_BOOL b_module_is_unlimited);
	
	// Sets all writable cells on a key to zero
	HRESULT KeyFormat(BSTR key_ident, BSTR *new_key_ident);
	// Programs a key
	HRESULT KeyProgram(BSTR key_ident, long customer_number, long key_number, long product_ident, long ver_major, long ver_minor, long key_type, long application_instances, long days, VARIANT module_value_list, BSTR *new_key_ident);
	// Reads raw data off of the key
	HRESULT KeyReadRaw(BSTR key_ident, VARIANT *pvtKeyData);
	
	//// ILicensingMessage
	//HRESULT GetLicenseMessageList(BSTR license_id, VARIANT *pvtMessageList);

	//// for reporting messages to clients and the event log
	//void GenerateMessage(const wchar_t* key_ident, EMessageType message_type, HRESULT error, time_t timestamp, const unsigned int MessageLookupID, ...);
	//void GenerateMessageInternal(const wchar_t* key_ident, EMessageType message_type, HRESULT error, time_t timestamp, const unsigned int MessageLookupID, const wchar_t* message);


	// For Software Server to access Validation Keys
	HRESULT GetKeyInfoAttribs(BSTR key_ident, Lic_KeyAttribs* pKeyAttribs);
	// For Software Server to access Validation Keys, will only work on keys of version 1
	// bForceActivitySlotUpdate = true will also update the currentDate, even if bForceCurrentDateUpdate = false;
	HRESULT SetKeyInfoAttribs(BSTR key_ident, Lic_KeyAttribs keyAttribs, bool bForceCurrentDateUpdate, bool bForceActivitySlotUpdate);

	HRESULT CheckHealth(unsigned int timeout);

	// CR.18131 - Detect DongleEmulator
	HRESULT VerifyNoDongleEmulator(int* pCheckEmulatorStatusCode, bool* pbCheckEmulatorFound, char* pEmulatorName, char* pExceptionServiceName);
private:
	//static const unsigned int TrialKeyDecrementCheckPeriod = 60*1000;	//(ms)
	//static const unsigned int UpdateKeysThreadPeriod = 60*1000;			//(ms)
	//static const unsigned int UpdateKeysThreadHighPeriodSeconds = 60;	//(sec) - 1 Minute
	//static const unsigned int UpdateKeysThreadLowPeriodSeconds = 300;	//(sec) - 5 Minutes

	//static const unsigned int HeartbeatCheckThreadPeriod = 60*1000;		//(ms)
	//static const unsigned int HeartbeatKillClientPeriod = 60;			// seconds before a non-responding client's licenses are revolked
	//
	//_bstr_t server_host_name;

	//typedef std::map<_bstr_t, LicensingMessageList> MessageClientList;
	//
	//// support for generating Password Packet files
	typedef std::map<_bstr_t, _bstr_t> KeyValueMap;
	typedef std::map<_bstr_t, _bstr_t> StringToStringMap;
	typedef std::pair<_variant_t, _bstr_t> ExpirePasswordPair;
	typedef std::vector<ExpirePasswordPair> PasswordList;
	class PasswordPacket
	{
	public:
		KeyValueMap headers; 
		PasswordList passwords;
		_variant_t completed_packet;
		_bstr_t verification_code;
	};
	typedef std::map<_bstr_t, PasswordPacket> ClientPasswordPacketList;
	//
	HANDLE KeyListLock;
	//HANDLE HeartbeatListLock;
	//HANDLE MessageClientListLock;
	HANDLE PasswordPacketListLock;		//xxx initialize this member
	
	
	// support for blocking brute force attempts at password cracking
	long failed_password_attempts;
	// returns the number of milliseconds to delay before checking a password
	static DWORD PasswordEntryDelay(long failed_attempts);
	
	
	// support for decrementing trial keys.
	// The TrialTimeInfo contains the last time that the hours counter on a 
	// key was decremented, as well as a flag indicating whether the mapped 
	// key was in use during the last hour.
	HANDLE KeyTrialTimeInfoLock;
	//APCTimer *TimesUpThread;
	//static void TimesUpThreadFunction(void* pvThis);
	typedef struct {time_t last_decrement; bool key_obtained;} TrialTimeInfo;
	typedef std::map<_bstr_t, TrialTimeInfo> TrialTimeInfoList;
	// called periodically by the TimesUpThread
	
	
	typedef std::vector<_bstr_t> StringList;
	typedef std::map<_bstr_t, ProtectionKey*> KeyList;
	
	// maps a license_id to a proxy to its LicenseManager's ILicensingManager interface
	//yyy _COM_SMARTPTR_TYPEDEF(ILicensingMessage,__uuidof(ILicensingMessage));
	//yyy typedef std::map<_bstr_t, ILicensingMessagePtr> MessageClientList;
	
	// maps license_id to a time_t
	typedef std::map<_bstr_t, DWORD> HeartbeatList;
	
	//APCTimer *UpdateKeysThread;
	//APCTimer *HeartbeatCheckThread;
	//static void UpdateKeysThreadFunction(void* pvThis);
	//static void HeartbeatCheckThreadFunction(void* pvThis);
	//virtual void USBEventCallback(LPVOID pContext);		// supports usb device insert/remove notification

	KeySpec keyspec;
	KeyList keys;
	StringToStringMap virtual_key_to_physical_key_list;
	HeartbeatList heartbeats;
	TrialTimeInfoList trial_keys;
	//MessageClientList message_clients;
	ClientPasswordPacketList password_packets;
	
	static BYTE crypto_key_password_packet_private[];
	static BYTE crypto_key_password_packet_public[];
	static BYTE crypto_key_password_packet_password[];
	static unsigned int packet_magic_number_int[];	//a static BYTE [] still adds string to image, and can be seen in process explorer, this value is a GUID that is easily read

	RainbowDriver* pRainbowDriver;
	//public:
	//	RainbowDriver* pRainbowDriver;
	public:
		HRESULT TimesUp();
		//void HeartbeatCheck();
};
