
#pragma once

#include <windows.h>
#include "rainbow\spromeps.h"
#include <comdef.h>
#include <vector>
#include <map>

class RainbowDriver
{
public:
	RainbowDriver();
	virtual ~RainbowDriver();
	
	// used during key programming, the key identifier changes during the programming process from a guid to a string based on customer and key numbers
	HRESULT ComputeCurrentKeyIdent(_bstr_t key, BSTR *physical_key_ident);
	HRESULT RefreshKeyList();
	HRESULT ReadCell(_bstr_t key, unsigned short cell, unsigned short *value);
	HRESULT WriteCell(_bstr_t key, unsigned short cell, unsigned short value);
	HRESULT FormatCell(_bstr_t key, unsigned short cell);
	HRESULT Query(_bstr_t key, unsigned short cell, void* query, void* response, unsigned short length);
	HRESULT Activate(_bstr_t key, unsigned short cell);
	HRESULT WriteKeyUnprogrammedIdentifier(_bstr_t key);
	HRESULT ClearKeyUnprogrammedIdentifier(_bstr_t key);
	
	typedef std::map<_bstr_t,RBP_SPRO_APIPACKET> KeyList;
	HANDLE keys_lock;
	KeyList keys;
	
	typedef struct
	{
		DWORD password;
		unsigned int units_licensed;
		unsigned int pages_per_minute_struct;
		unsigned int password_number;
		unsigned short version;
		unsigned int type;
		unsigned int module;
		unsigned short key;
		unsigned short customer;
		unsigned short extend_days;
		unsigned short extend_num;
		bool legacy;
	} ArgumentList;
	static HRESULT ParsePassword(_bstr_t password, ArgumentList &arguments);
	
	typedef enum {READWRITE=0, READONLY=1, COUNTER=2, ALGORITHM=3} AccessCode;
	static const AccessCode accessCode[64];

	HRESULT TranslateRainbowError(unsigned short int rnboError);

private:
	
	static const unsigned short DEVELOPER_ID = 0xFEA8;
	static const unsigned short WRITE_PASSWORD = 0x4AFC;
	static const unsigned short ACTIVATE_PASSWORD1 = 0xFED1;
	static const unsigned short ACTIVATE_PASSWORD2 = 0x1A00;
#if (defined(_DEBUG) || defined(_SOLIMAR_INTERNAL))
	static const unsigned short OVERWRITE_PASSWORD1 = 0xC822;
	static const unsigned short OVERWRITE_PASSWORD2 = 0x85A2;
#else
	static const unsigned short OVERWRITE_PASSWORD1 = 0x0000;
	static const unsigned short OVERWRITE_PASSWORD2 = 0x0000;
#endif
	
	static const unsigned short CELL_SERIAL_NUMBER = (unsigned short)0x01;
	static const unsigned short CELL_KEY_NUMBER = (unsigned short)0x3F;
	static const unsigned short CELL_CUSTOMER_NUMBER = (unsigned short)0x3E;
	static const unsigned short CELL_PASSWORD_NUMBER = (unsigned short)0x35;
	static const unsigned short CELL_KEY_GUID = (unsigned short)0x30;
	//Unprogrammed keys have a GUID written from CELL_KEY_GUID to CELL_KEY_GUID+7
	//There was a bug that the cells CELL_KEY_GUID to CELL_KEY_GUID+7 were not being
	//cleared out when a key got programmed.
	
	// Unique key id management for unprogrammed keys
	HRESULT WriteKeyTempGUID(RBP_SPRO_APIPACKET packet, GUID &id);
	HRESULT ReadKeyTempGUID(RBP_SPRO_APIPACKET packet, GUID &id);
	HRESULT ClearKeyTempGUID(RBP_SPRO_APIPACKET packet);
};
