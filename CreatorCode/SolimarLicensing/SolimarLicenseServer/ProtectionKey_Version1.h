#pragma once
#include "ProtectionKey.h"

//This class is to access Protection Keys at Key Version 1
//Keys used as Validation Tokens for Software Licensing are of this type
class ProtectionKey_Version1 : public ProtectionKey
{
public:
	ProtectionKey_Version1();
	ProtectionKey_Version1(_bstr_t virtualKeyIdent, const ProtectionKey_Version1 &k);
	ProtectionKey_Version1(_bstr_t physicalKeyIdent, KeySpec *keyspec, RainbowDriver *driver);
	virtual ~ProtectionKey_Version1();
	virtual ProtectionKey_Version1* Copy(_bstr_t virtualKeyIdent);

	// returns true if a trial key has expired
	virtual bool TimesUp();
	virtual bool isOnTrial() 
	{
//OutputDebugStringW(L"ProtectionKey_Version1::isOnTrial()");
//ReadBits(0x090, 8, false);
//ReadBits(0x090, 7, false);
//ReadBits(0x090, 6, false);
//ReadBits(0x090, 5, false);

//WriteBits(0x090, 1, 0x0009);
//WriteBits(0x090, 2, 0x0098);
//WriteBits(0x090, 3, 0x0987);
//WriteBits(0x090, 4, 0x9876);
//
//WriteBits(0x094, 1, 0x0009);
//WriteBits(0x094, 2, 0x0098);
//WriteBits(0x094, 3, 0x0987);
//WriteBits(0x094, 4, 0x9876);

//ReadBits(0x200, 4, false);
//ReadBits(0x20, 4, false);
//WriteBits(0x200, 1, 0x0009);
//WriteBits(0x204, 2, 0x0087);
//WriteBits(0x20c, 3, 0x00654);
//WriteBits(0x180, 8, 0x12345678);
//WriteBits(0x09c, 3, 0x0987);
//WriteBits(0x09c, 4, 0x9876);
//WriteBits(0x09c, 5, 0x98765);
//WriteBits(0x09c, 6, 0x987654);
//WriteBits(0x09c, 7, 0x9876543);
//WriteBits(0x09c, 8, 0x98765432);
		return true;
	}
	//virtual bool isOnTrial() {return true;}	//Always decrement hours if possible
	virtual bool KeyInUse() {return true;}	//Always decrement hours if possible
	virtual HRESULT DecrementTrialHours();

	HRESULT GetSoftwareKeyCode(BSTR *key_code);	// Is a GUID
	HRESULT SetSoftwareKeyCode(BSTR key_code);	// Is a GUID

	// Down cast 64-bit time_t to 32-bit only unsigned long, time will wrap in year 2038.
	HRESULT GetSoftwarePacketCreationDateTime(time_t *creation_datetime);
	HRESULT SetSoftwarePacketCreationDateTime(time_t creation_datetime);
	HRESULT GetSoftwareCurrentDateTime(time_t *current_datetime);
	HRESULT SetSoftwareCurrentDateTime(time_t current_datetime);

	//Activity Index is governed by the space on the physical usb key
	//Activity Index has a range from 0 to 15
	//An Activity Index's Current Activation has a range of 0 to 0xff
	//An Activity Index's Hours to Expiration has a range of 0 to 0xffe with 0 and 0xfff as being expired
	//An Activity Index's Hours to Expiration is calculated by looking and the GetSoftwareCurrentDateTime() value
	HRESULT GetSoftwareActivitySlotCurrentActivation(unsigned short activity_index, unsigned long* current_activation);
	HRESULT SetSoftwareActivitySlotCurrentActivation(unsigned short activity_index, unsigned short current_activation);
	HRESULT GetSoftwareActivitySlotHoursToExpiration(unsigned short activity_index, unsigned long* hours_to_expiration);
	HRESULT SetSoftwareActivitySlotHoursToExpiration(unsigned short activity_index, unsigned short hours_to_expiration);
	HRESULT UseSoftwareActivation(unsigned short activity_index, unsigned short increase_hours_to_expiration);

	HRESULT GetHistoryNumber(unsigned long *history_number);
	HRESULT SetHistoryNumber(unsigned short history_number);
	//HRESULT GetSoftwareCurrentActivations(_bstr_t key, unsigned short *current_activations);
	//HRESULT SetSoftwareCurrentActivations(_bstr_t key, unsigned short current_activations);

private:
	// KeySpec for Key Version 1
	//static const unsigned short CELL_SOFTWARE_CODE_MODIFIED_DATE = (unsigned short)0x100;
	static const unsigned short CELL_LAST_PACKET_CREATION_DATE = (unsigned short)0x2e0;	//Add new modules to the end
	static const unsigned short CELL_HEADER_HISTORY_NUMBER = (unsigned short)0x3d0;

	static const unsigned short CELL_CURRENT_DATE = (unsigned short)0x080;
	static const unsigned short CELL_ACTIVATION_SLOT_00 = (unsigned short)0x0a0;
	static const unsigned short CELL_ACTIVATION_SLOT_01 = (unsigned short)0x0b4;
	static const unsigned short CELL_ACTIVATION_SLOT_02 = (unsigned short)0x0c8;
	static const unsigned short CELL_ACTIVATION_SLOT_03 = (unsigned short)0x0dc;
	static const unsigned short CELL_ACTIVATION_SLOT_04 = (unsigned short)0x0f0;
	static const unsigned short CELL_ACTIVATION_SLOT_05 = (unsigned short)0x104;
	static const unsigned short CELL_ACTIVATION_SLOT_06 = (unsigned short)0x118;
	static const unsigned short CELL_ACTIVATION_SLOT_07 = (unsigned short)0x12c;
	static const unsigned short CELL_ACTIVATION_SLOT_08 = (unsigned short)0x140;
	static const unsigned short CELL_ACTIVATION_SLOT_09 = (unsigned short)0x154;
	static const unsigned short CELL_ACTIVATION_SLOT_10 = (unsigned short)0x168;
	static const unsigned short CELL_ACTIVATION_SLOT_11 = (unsigned short)0x17c;
	static const unsigned short CELL_ACTIVATION_SLOT_12 = (unsigned short)0x190;
	static const unsigned short CELL_ACTIVATION_SLOT_13 = (unsigned short)0x1a4;
	static const unsigned short CELL_ACTIVATION_SLOT_14 = (unsigned short)0x1b8;
	static const unsigned short CELL_ACTIVATION_SLOT_15 = (unsigned short)0x1cc;
	static const unsigned short CELL_ACTIVATION_SLOT_16 = (unsigned short)0x1e0;
	static const unsigned short CELL_ACTIVATION_SLOT_17 = (unsigned short)0x1f4;
	static const unsigned short CELL_ACTIVATION_SLOT_18 = (unsigned short)0x208;
	static const unsigned short CELL_ACTIVATION_SLOT_19 = (unsigned short)0x21c;
	static const unsigned short CELL_ACTIVATION_SLOT_LIST[];
	//static const unsigned short CELL_ACTIVATION_SLOT_20 = (unsigned short)0x230;
	//static const unsigned short CELL_ACTIVATION_SLOT_21 = (unsigned short)0x244;

	static const unsigned short CELL_BIT_SIZE_DATE = 8;
	static const unsigned short CELL_BIT_SIZE_HEADER = 4;
	static const unsigned short CELL_BIT_SIZE_ACTIVATION_CURRENT = 2;
	static const unsigned short CELL_BIT_SIZE_ACTIVATION_HOURS_TO_EXPIRE = 3;
	static const unsigned short CELL_BIT_SIZE_TO_SUBCELL = 4;
	static const unsigned short CELL_BIT_SIZE_ACTIVATION_ENTIRE_SLOT = CELL_BIT_SIZE_ACTIVATION_CURRENT + CELL_BIT_SIZE_ACTIVATION_HOURS_TO_EXPIRE;


	// Interaction with the data on the physical key
	HRESULT ReadBits(unsigned short offset, unsigned short bits, unsigned long* value, bool physical);
	HRESULT WriteBits(unsigned short offset, unsigned short bits, unsigned long value);
public:
	unsigned short ReadCellCache(unsigned short cell);
};
