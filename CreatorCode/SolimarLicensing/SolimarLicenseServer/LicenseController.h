#pragma once

#include <comdef.h>
#include <string>
#include <map>

#include "KeySpec.h"
#include "ProtectionKey.h"
#include "RainbowDriver.h"
#include "..\common\SafeMutex.h"
#include "..\common\apctimer.h"

//#include "..\common\ILicensingMessage.h"
#include "KeyMessages.h"
#include "..\common\LicensingMessage.h"
#include "..\common\LicensingSoftwareMessage.h"
#include "..\common\CryptoHelper.h"

#include "SoftwareServer.h"
#include "KeyServer.h"

#include "USBNotification.h"

class LicenseController : public USBNotification
{
public:
	LicenseController();
	~LicenseController();
	
	HRESULT Heartbeat(BSTR license_id);
	HRESULT RemoveHeartbeat(BSTR license_id);
	HRESULT GetLicenseServerTime(VARIANT *pvtSystemTime);

	// ILicensingMessage
	HRESULT GetLicenseMessageList(BSTR license_id, VARIANT *pvtMessageList);

	//ISoftwareLicensingMessage
	HRESULT GetSoftwareLicenseMessageList(BSTR license_id, VARIANT *pvtMessageList);

	// for reporting messages to clients and the event log
	void GenerateMessage(const wchar_t* key_ident, EMessageType message_type, HRESULT error, time_t timestamp, const unsigned int MessageLookupID, ...);
	//void GenerateMessageInternal(const wchar_t* key_ident, EMessageType message_type, HRESULT error, time_t timestamp, const unsigned int MessageLookupID, const wchar_t* message);

	void GenerateSoftwareLicenseMessage(const wchar_t* softwareLicID, long productID, EMessageType messageType, HRESULT error, time_t timestamp, const unsigned int MessageLookupID, ...);
	//void GenerateSoftwareLicenseInternal(const wchar_t* softwareLicID, long productID, EMessageType messageType, HRESULT error, time_t timestamp, const unsigned int MessageLookupID, const wchar_t* message);

	//Maybe implement something like
	void GenerateMessageInternal(const wchar_t* license_source, bool bGenerateSoftwareLicMsg, long productID, EMessageType message_type, HRESULT error, time_t timestamp, const unsigned int MessageLookupID, const wchar_t* message);

	// called periodically by the TimesUpThread
	HRESULT TimesUp();
private:

	HRESULT RemoveFromNotification(BSTR license_id);

	static const unsigned int CheckHealthThreadPeriod = 2*60*1000;		//(ms) - 2 Minutes
	//CR.12791 - For very large license server files on slow computers, generating/viewing diagnostic data can take minutes, which may hit the deadlock timer
	static const unsigned int MutexDeadlockTimeout= 7*60*1000;				//(ms) - 7 Minutes

	static const unsigned int TrialKeyDecrementCheckPeriod = 60*1000;	//(ms)
	static const unsigned int UpdateKeysThreadPeriod = 60*1000;			//(ms)
	static const unsigned int UpdateKeysThreadHighPeriodSeconds = 60;	//(sec) - 1 Minute
	static const unsigned int UpdateKeysThreadLowPeriodSeconds = 300;	//(sec) - 5 Minutes

	static const unsigned int HeartbeatCheckThreadPeriod = 60*1000;		//(ms)
	static const unsigned int HeartbeatKillClientPeriod = 60;			// seconds before a non-responding client's licenses are revolked

	static const unsigned int ProcessAlertEmailThreadPeriod = 60*1000;		//(ms)
	
	_bstr_t server_host_name;

	typedef std::map<_bstr_t, LicensingMessageList> MessageClientList;
	typedef std::map<_bstr_t, LicensingSoftwareMessageList> SoftwareMessageClientList;
	
	// support for generating Password Packet files
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
	
	HANDLE HeartbeatListLock;
	HANDLE MessageClientListLock;
	HANDLE PasswordPacketListLock;		//xxx initialize this member
	HANDLE ProcessAlertEmailLock;
	
	HRESULT CheckHealth(); //Checks for mutex deadlocks, if any are hit, will recycle process.
	
	// support for blocking brute force attempts at password cracking
	long failed_password_attempts;
	// returns the number of milliseconds to delay before checking a password
	//static DWORD PasswordEntryDelay(long failed_attempts);
	void HeartbeatCheck();
	
	// support for decrementing trial keys.
	// The TrialTimeInfo contains the last time that the hours counter on a 
	// key was decremented, as well as a flag indicating whether the mapped 
	// key was in use during the last hour.
	APCTimer *TimesUpThread;
	static void TimesUpThreadFunction(void* pvThis);
	typedef struct {time_t last_decrement; bool key_obtained;} TrialTimeInfo;
	typedef std::map<_bstr_t, TrialTimeInfo> TrialTimeInfoList;

	// called periodically by the UpdateKeysThread
	HRESULT UpdateKeys();

	// called periodically to send all alert e-mails that have been queued up.
	HRESULT ProcessAllAlertEmails();
	
	typedef std::vector<_bstr_t> StringList;
	typedef std::map<_bstr_t, ProtectionKey*> KeyList;

	struct EmailAttribs
	{
		long productId;
		unsigned int eventId;
		std::wstring message;
	};

	typedef std::vector<EmailAttribs> EmailAttribsStack;
	EmailAttribsStack toSendEmailStack;
	
	// maps a license_id to a proxy to its LicenseManager's ILicensingManager interface
	//yyy _COM_SMARTPTR_TYPEDEF(ILicensingMessage,__uuidof(ILicensingMessage));
	//yyy typedef std::map<_bstr_t, ILicensingMessagePtr> MessageClientList;
	
	_bstr_t BinaryToString(BYTE *pData, DWORD length);
	DWORD StringToBinaryLength(_bstr_t str);
	void StringToBinary(_bstr_t str, BYTE *pData, DWORD length);
	
	// maps license_id to a time_t
	typedef std::map<_bstr_t, DWORD> HeartbeatList;
	
	APCTimer *CheckHealthThread;
	APCTimer *UpdateKeysThread;
	APCTimer *HeartbeatCheckThread;
	APCTimer *ProcessAlertEmailThread;
	static void CheckHealthThreadFunction(void* pvThis);
	static void UpdateKeysThreadFunction(void* pvThis);
	static void HeartbeatCheckThreadFunction(void* pvThis);
	static void ProcessAlertEmailCheckThreadFunction(void* pvThis);

	static bool InitTimerThreadCB(EInitReason reason, void *pArg) ;
	virtual void USBEventCallback(LPVOID pContext);		// supports usb device insert/remove notification

	KeySpec keyspec;
	KeyList keys;
	StringToStringMap virtual_key_to_physical_key_list;
	HeartbeatList heartbeats;
	TrialTimeInfoList trial_keys;
	MessageClientList message_clients;
	SoftwareMessageClientList message_software_clients;
	ClientPasswordPacketList password_packets;
	
	//static BYTE crypto_key_password_packet_private[];
	//static BYTE crypto_key_password_packet_public[];
	//static BYTE crypto_key_password_packet_password[];

	public:
		RainbowDriver driver;

		SoftwareServer softwareServer;
		KeyServer keyserver;
};