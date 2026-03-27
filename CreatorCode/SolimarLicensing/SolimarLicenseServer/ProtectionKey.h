
#pragma once

#include <comutil.h>
#include "KeySpec.h"
#include "RainbowDriver.h"
#include <map>

class ProtectionKey
{
public:
	
	ProtectionKey();
	ProtectionKey(const ProtectionKey &k);
	ProtectionKey(_bstr_t keyident, KeySpec *keyspec, RainbowDriver *driver);
	
	HRESULT TrialExpires(VARIANT *expire_date);
	HRESULT TrialHours(long *trial_hours);
	HRESULT IsActive(VARIANT_BOOL *key_active);
	HRESULT IsProgrammed(VARIANT_BOOL *key_programmed);
	HRESULT HeaderQuery(long header_ident, VARIANT *value);
	HRESULT IsPresent(VARIANT_BOOL *key_present);
	HRESULT EnterPassword(BSTR password);

private:
	// Password Entry

	// generic, non product-specific
	bool EnterBasePassword(DWORD user_password, bool trial_key, bool base_key, bool permanent_allowed_key, unsigned short customer_number, unsigned short key_number);
	bool EnterProductVersionPassword(DWORD user_password, bool trial_key, bool base_key, bool permanent_allowed_key, unsigned short customer_number, unsigned short key_number, unsigned short product_version);
	bool EnterExtensionPassword(DWORD user_password, bool trial_key, bool base_key, bool permanent_allowed_key, unsigned short customer_number, unsigned short key_number, unsigned short key_status, unsigned short extend_days, unsigned short extension_num);
	bool EnterModulePassword(DWORD user_password, bool trial_key, bool base_key, bool permanent_allowed_key, unsigned short customer_number, unsigned short key_number, unsigned int module_id, unsigned int units_licensed);
	
    // SPD/iConvert specific	
	bool EnterSPDModulePassword(DWORD user_password, bool trial_key, bool base_key, bool permanent_allowed_key, unsigned short customer_number, unsigned short key_number, unsigned int module_id, unsigned int units_licensed);
	bool EnterSPDOutputPassword(DWORD user_password, bool trial_key, bool base_key, bool permanent_allowed_key, unsigned short customer_number, unsigned short key_number, unsigned short output_units);
	bool EnterSPDPagesPerMinutePassword(DWORD user_password, bool trial_key, bool base_key, bool permanent_allowed_key, unsigned short customer_number, unsigned short key_number, unsigned int pages_per_minute_struct);
	
	// SolSearcher specific
	bool EnterSolSearcherModulePassword(DWORD user_password, bool trial_key, bool base_key, bool permanent_allowed_key, unsigned short customer_number, unsigned short key_number, unsigned int module_id, unsigned int units_licensed);

public:

	HRESULT GenerateBasePassword(long customer_number, long key_number, BSTR *password);
	HRESULT GenerateVersionPassword(long customer_number, long key_number, long ver_major, long ver_minor, BSTR *password);
	HRESULT GenerateExtensionPassword(long customer_number, long key_number, long extend_days, long extension_num, BSTR *password);
	HRESULT GenerateModulePassword(long customer_number, long key_number, long product_ident, long module_ident, long license_count, BSTR *password);
	
	HRESULT Lock(BSTR license_id);
	HRESULT Unlock(BSTR license_id);
	HRESULT Obtain(BSTR license_id);
	HRESULT Release(BSTR license_id);
	
	HRESULT ValidateLicense(BSTR license_id, VARIANT_BOOL *license_valid);
	HRESULT ModuleEnumerate(VARIANT *pvtModuleList);
	HRESULT ModuleQuery(long module_ident, VARIANT *vtValue);
	HRESULT ModuleLicenseTotal(BSTR license_id, long module_ident, long* license_count);
	HRESULT ModuleLicenseInUse(BSTR license_id, long module_ident, long* license_count);
	HRESULT ModuleLicenseObtain(BSTR license_id, long module_ident, long license_count);
	HRESULT ModuleLicenseRelease(BSTR license_id, long module_ident, long license_count);
	HRESULT LicenseReleaseAll(BSTR license_id);
	
	// Sets all writable cells on a key to zero
	HRESULT Format(BSTR *new_key_ident=0);
	// Programs a key
	HRESULT Program(long customer_number, long key_number, long product_ident, long ver_major, long ver_minor, long key_type, long days, VARIANT module_value_list, BSTR *new_key_ident);
	// Reads raw data off of the key
	HRESULT ReadRaw(VARIANT *pvtKeyData);

	void UpdateCellCache();
	void UpdateCellCache(unsigned int cell);
	HRESULT DecrementTrialHours();
	bool TimesUp();
	bool TimesUpClockViolation();
	
	// returns true if the key is a trial key
	bool isOnTrial();
	// returns true if the key is currently obtained by at least one application
	bool KeyInUse();

private:
	
	_bstr_t m_keyident;
	KeySpec *m_keyspec;
	RainbowDriver *m_driver;
	
	static const char* KDPasswordText[];
	static const unsigned short KDPasswordDays[];
	static const unsigned short KDPasswordHours[];
	
	typedef enum
	{
		STATE_UNKNOWN =			0,
		STATE_CONNECTED =		1,
		STATE_DISCONNECTED =	2,
		STATE_ERROR =			3
	} KeyState;
	
	typedef enum
	{
		UNINITIALIZED_TRIAL=3, INITIAL_TRIAL=0, EXTENDED_TRIAL=1,
		EXTENDED_TRIAL2=4, EXTENDED_TRIAL3=5, EXTENDED_TRIAL4=6,
		EXTENDED_TRIAL5=7, BASE=2, UNUSED=10, DEACTIVATED=11
	} KeyStatus;
	
	typedef enum {
		KEYNone			=0x00000000,  //reserved state
		KEYAddon		=1,
		KEYBase			=2,
		KEYReplacement	=3,
		KEYRelicense	=4,
		KEYInventory	=5,
		KEYBackup		=6,
		KEYEmergency	=7,
		//KEYSolimar	=8,
		KEYCustom		=9,
		KEYLoan			=10,
		KEYDemo			=11,
		KEYRental		=12,
		KEYDevelopment	=13,
		KEYReserved		=14,
	} KeyTypeFlagX;
	
	static const short MAX_EXTENSION_NUM = 4;
	static const short INITIAL_TRIAL_DAYS = 30;
	static const short EXTRA_DAY_BUFFER = 10;
	static const short MAX_EXTENSION_DAYS = 255;  //before 3/17/98 MAX_EXTENSION_DAYS = 90;
	
public:
	static const unsigned int DAYS_PER_YEAR = 365;
	static const unsigned int HOURS_PER_DAY = 24;
	static const unsigned int SECONDS_PER_HOUR = 3600;
	static const unsigned int SECONDS_PER_DAY = 86400;
	
private:
	HANDLE cells_lock;
	static const unsigned int KeyCellCount = 64;
	unsigned short cells[KeyCellCount];
	HRESULT cells_valid[KeyCellCount];

	HANDLE license_use_lock;
	typedef std::map<unsigned int,long> ModuleLicenseUseList;
	typedef std::map<_bstr_t, ModuleLicenseUseList> LicenseUseList;		// maps license identifier guid to a module use list
	typedef std::map<_bstr_t, long> KeyUseList;							// maps license identifier guid to the number of times the key has been obtained by that ident
	bool key_owned;
	_bstr_t key_owner;
	KeyUseList key_use;
	LicenseUseList license_use;
	
	// predefined queries
	typedef enum {PRIMARY_1=0, PRIMARY_2=1, SECONDARY_1=2, SECONDARY_2=3} PredefinedQuery;
	static const BYTE predefined_queries[8][5];
	
	// read a cell (a 16 bit segment of data) off of the protection key
	unsigned short ReadCellCache(unsigned short cell);
	
	// storage sizes in bits must be a power of 2 and no greater than 32 {1,2,...16,32}
	// offsets must be aligned to an integral multiple of bits
	_variant_t ReadBitsCache(unsigned short offset, unsigned short bits);
	
	// Interaction with the data on the physical key
	_variant_t ReadBits(unsigned short offset, unsigned short bits, bool physical);
	unsigned short ReadCellPhysical(unsigned short cell);
	void WriteCellPhysical(unsigned short cell, unsigned short value);
	void FormatCellPhysical(unsigned short cell);
	_variant_t ReadBitsPhysical(unsigned short offset, unsigned short bits);
	void WriteBitsPhysical(unsigned short offset, unsigned short bits, _variant_t vtValue);
	
	_variant_t ReadHeaderCache(wchar_t* id);
	_variant_t ReadModuleCache(wchar_t* id);
	_variant_t ReadHeaderCache(unsigned int id);
	_variant_t ReadModuleCache(unsigned int id);
	
	void WriteHeader(wchar_t* id, unsigned int value);
	void WriteModule(wchar_t* id, unsigned int value);
	void WriteHeader(unsigned int id, unsigned int value);
	void WriteModule(unsigned int id, unsigned int value);
	void WriteLicense(wchar_t* id, unsigned int value);
	void WriteLicense(unsigned int id, unsigned int value);
	
	// License information helpers
	bool LicenseIsUnlimited(wchar_t* id);
	bool LicenseIsUnlimited(unsigned int id);
	bool LicenseIsPooled(wchar_t* id);
	bool LicenseIsPooled(unsigned int id);
	unsigned int LicenseEffectiveValue(wchar_t* id);
	unsigned int LicenseEffectiveValue(unsigned int id);
	
	// Utility functions
	
	// Given a set of bits (stored in an unsigned int), package it up in to a VARIANT that 
	// takes the number of bits (type) in to account
	_variant_t BitsToVariant(unsigned int value, unsigned int bits);
	DWORD GetPassword(DWORD query);
	DWORD GetBasePassword(unsigned short customer_number, unsigned short key_number);
	DWORD GetProductVersionPassword(unsigned short customer_number, unsigned short key_number, unsigned short product_version);
	DWORD GetExtensionPassword(unsigned short customer_number, unsigned short key_number, unsigned short extend_days, unsigned short extension_num);
	
	// generic, non product-specific module password generation
	DWORD GetModulePassword(unsigned short customer_number, unsigned short key_number, unsigned int module_id, unsigned int units_licensed);
	
	// SPD/iConvert specific
	DWORD GetSPDOutputPassword(unsigned short customer_number, unsigned short key_number, unsigned short output_units);
	DWORD GetSPDModulePassword(unsigned short customer_number, unsigned short key_number, unsigned int module_id, unsigned int units_licensed);
	DWORD GetSPDPagesPerMinutePassword(unsigned short customer_number, unsigned short key_number, unsigned int ppm_extensions);
	
	// SolSearcher specific
	DWORD GetSolsearcherModulePassword(unsigned short customer_number, unsigned short key_number, unsigned int module_id, unsigned int units_licensed);
	
	void TrialToPermanent();
	HRESULT WriteStatus(unsigned short new_key_status);
	HRESULT WriteCounterDays(unsigned short extend_days);
	HRESULT WriteExpirationDays(unsigned short extend_days);
};

