
#include "ProtectionKey_Version1.h"
#include "..\common\TimeHelper.h"
#include "SoftwareServerInstance.h"

const unsigned short ProtectionKey_Version1::CELL_ACTIVATION_SLOT_LIST[] = {
	CELL_ACTIVATION_SLOT_00, CELL_ACTIVATION_SLOT_01, CELL_ACTIVATION_SLOT_02, CELL_ACTIVATION_SLOT_03, 
	CELL_ACTIVATION_SLOT_04, CELL_ACTIVATION_SLOT_05, CELL_ACTIVATION_SLOT_06, CELL_ACTIVATION_SLOT_07, 
	CELL_ACTIVATION_SLOT_08, CELL_ACTIVATION_SLOT_09, CELL_ACTIVATION_SLOT_10, CELL_ACTIVATION_SLOT_11, 
	CELL_ACTIVATION_SLOT_12, CELL_ACTIVATION_SLOT_13, CELL_ACTIVATION_SLOT_14, CELL_ACTIVATION_SLOT_15, 
	CELL_ACTIVATION_SLOT_16, CELL_ACTIVATION_SLOT_17, CELL_ACTIVATION_SLOT_18, CELL_ACTIVATION_SLOT_19
};

ProtectionKey_Version1::ProtectionKey_Version1()
: ProtectionKey()
{
}

ProtectionKey_Version1::ProtectionKey_Version1(_bstr_t virtualKeyIdent, const ProtectionKey_Version1 &k)
: ProtectionKey(virtualKeyIdent, k)
{
}


ProtectionKey_Version1::ProtectionKey_Version1(_bstr_t physicalKeyIdent, KeySpec *keyspec, RainbowDriver *driver)
: ProtectionKey(physicalKeyIdent, physicalKeyIdent, keyspec, driver, true)
{
}

ProtectionKey_Version1::~ProtectionKey_Version1()
{
}

ProtectionKey_Version1* ProtectionKey_Version1::Copy(_bstr_t virtualKeyIdent)
{
	return new ProtectionKey_Version1(virtualKeyIdent, *this);
}

//Maybe remove...
bool ProtectionKey_Version1::TimesUp()
{
   /*
//OutputDebugStringW(L"ProtectionKey_Version1::TimesUp()");

	//Check to see if an hour has been passed since the current time/date on the key (check cache)
	//if an hour has passed, 
	//	decrement all the hour counters for all the activeSlot's, 0xfff is expired, 
	//	refresh cache for key
	{
		//Test
		unsigned long value;
		ReadBits(0x09, 4, &value, false);

		//wchar_t tmpBuf[256];
		//wsprintf(tmpBuf, L"ProtectionKey_Version1::TimesUp() - ", hr);
//OutputDebugStringW(tmpBuf);
	}

	bool bTemp(false);
	bTemp = true;
	bTemp = false;
	return bTemp;
   */
   return false;
}

//Called to reduce hours until expire.
HRESULT ProtectionKey_Version1::DecrementTrialHours()
{
//OutputDebugStringW(L"ProtectionKey_Version1::DecrementTrialHours()");

	//Update current date, and decrement all the hours counters.  Decrement by the difference of current timedate
	// and last touch.  If an hour hasn't passed, return E_FAIL
	HRESULT hr(E_FAIL);
	SafeMutex mutex(license_use_lock);
//wchar_t debug_buf[1024];
	try
	{
		//YYY Reset to zero
		//if(_wcsicmp(m_physicalKeyIdent, L"0100-1001") == 0)
		//{
		//	for(int idx=0; idx<_countof(CELL_ACTIVATION_SLOT_LIST); idx++)
		//	{
		//		hr = this->SetSoftwareActivitySlotCurrentActivation(idx, idx);
		//		hr = this->SetSoftwareActivitySlotHoursToExpiration(idx, idx*idx);
		//	}
		//}

		time_t currentTimeT = time(NULL);
		time_t keyTimeT = time_t(0);
		hr = this->GetSoftwareCurrentDateTime(&keyTimeT);
		if(FAILED(hr)) throw hr;


		VARIANT currentTimeVT = TimeHelper::TimeTToVariant(currentTimeT, false);
		SYSTEMTIME currentSystime;
		VariantTimeToSystemTime(currentTimeVT.date, &currentSystime);
		wchar_t currentTimestamp[256];
		TimeHelper::SystemTimeToString(currentTimestamp, _countof(currentTimestamp), currentSystime);
//_snwprintf_s(debug_buf, 1024, L"ProtectionKey_Version1::DecrementTrialHours() - currentTimestamp: %s", currentTimestamp);
//OutputDebugStringW(debug_buf);

		VARIANT keyTimeVT = TimeHelper::TimeTToVariant(keyTimeT, false);
		//SYSTEMTIME keySystime;
		//VariantTimeToSystemTime(keyTimeVT.date, &keySystime);
		//wchar_t keyTimestamp[256];
		//TimeHelper::SystemTimeToString(keyTimestamp, _countof(keyTimestamp), keySystime);
//_snwprintf_s(debug_buf, 1024, L"ProtectionKey_Version1::DecrementTrialHours() -     keyTimestamp: %s, keyTimeT: 0x%x == %d", keyTimestamp, keyTimeT, keyTimeT);
//OutputDebugStringW(debug_buf);
		if(keyTimeT == -1)
			keyTimeT = 0;

		double timeDifferenceInSeconds = difftime(currentTimeT/*endTime*/, keyTimeT/*startTime*/);
		if(timeDifferenceInSeconds == 0)
		{
			DWORD lastError = errno;
//_snwprintf_s(debug_buf, 1024, L"ProtectionKey_Version1::DecrementTrialHours() - lastError: %d", lastError);
//OutputDebugStringW(debug_buf);
			//g_licenseController.GenerateMessage(m_physicalKeyIdent, MT_ERROR, LicenseServerError::EHR_KEY_WRITE_ACCESS_DENIED, time(0), MessageKeyWriteFailure);
			g_licenseController.GenerateMessage(m_physicalKeyIdent, MT_ERROR, E_FAIL, time(0), MessageGenericError, L"Error with the time on the system, please send Solimar License Diagnostic Data.");
			if(lastError != 0)
				throw E_FAIL;
		}
		
//		_snwprintf_s(debug_buf, 1024, L"ProtectionKey_Version1::DecrementTrialHours() - difftime(%d, %d) = timeDifferenceInSeconds: %f, inMinutes: %d, inHours: %d", currentTimeT, keyTimeT, timeDifferenceInSeconds, int(timeDifferenceInSeconds/60.0), int(timeDifferenceInSeconds/TimeHelper::ONE_HOUR_IN_SECONDS));
//OutputDebugStringW(debug_buf);
		int timeDifferenceInHours = int(timeDifferenceInSeconds / TimeHelper::ONE_HOUR_IN_SECONDS);
		if(timeDifferenceInHours > 0)
		{
			unsigned long currentActivation;
			unsigned long activationHoursLeft;
			for(int idx=0; idx<_countof(CELL_ACTIVATION_SLOT_LIST); idx++)
			{
				
				hr = this->GetSoftwareActivitySlotCurrentActivation(idx, &currentActivation);
				if(FAILED(hr)) throw hr;
				hr = this->GetSoftwareActivitySlotHoursToExpiration(idx, &activationHoursLeft);
				if(FAILED(hr)) throw hr;

//_snwprintf_s(debug_buf, 1024, L"ProtectionKey_Version1::DecrementTrialHours() - Query - idx: %d, currentActivation: %d, activationHoursLeft: %d", idx, currentActivation, activationHoursLeft);
//OutputDebugStringW(debug_buf);
				if(activationHoursLeft > 0)
				{
					//Don't allow hours to wrap past zero
					hr = this->SetSoftwareActivitySlotHoursToExpiration(idx, max(int(activationHoursLeft-timeDifferenceInHours), 0));
					if(FAILED(hr)) throw hr;

//hr = this->GetSoftwareActivitySlotHoursToExpiration(idx, &activationHoursLeft);
//if(FAILED(hr)) throw hr;
//_snwprintf_s(debug_buf, 1024, L"ProtectionKey_Version1::DecrementTrialHours() - Decrement - idx: %d, currentActivation: %d, activationHoursLeft: %d", idx, currentActivation, activationHoursLeft);
//OutputDebugStringW(debug_buf);
				}
			}
			//Increment CurrentDateTime by the number of hours that has passed since last time it was written to.
			hr = this->SetSoftwareCurrentDateTime(keyTimeT + (timeDifferenceInHours * TimeHelper::ONE_HOUR_IN_SECONDS));
			if(FAILED(hr)) throw hr;
		}
	}
	catch(HRESULT &eHr)
	{
		hr = eHr;
//_snwprintf_s(debug_buf, 1024, L"ProtectionKey_Version1::DecrementTrialHours() - errorHr: 0x%x", hr);
//OutputDebugStringW(debug_buf);
	}
	catch(_com_error &e)
	{
		hr = e.Error();
	}

	//CR.FIX.15610 - No longer set to E_FAIL
	//hr = E_FAIL;	//Reset to E_FAIL 
	return hr;
}

//there are two mutexs to use cells_lock & license_use_lock
//SafeMutex mutex();

HRESULT ProtectionKey_Version1::GetSoftwareKeyCode(BSTR *key_code /*Is a GUID*/)
{
	return m_driver->GetSoftwareKeyCode(m_physicalKeyIdent, key_code);
}
HRESULT ProtectionKey_Version1::SetSoftwareKeyCode(BSTR key_code /*Is a GUID*/)
{
	return m_driver->SetSoftwareKeyCode(m_physicalKeyIdent, key_code);
}

HRESULT ProtectionKey_Version1::GetSoftwarePacketCreationDateTime(time_t *creation_datetime)
{
	return ReadBits(CELL_LAST_PACKET_CREATION_DATE, CELL_BIT_SIZE_DATE, (unsigned long*)creation_datetime, false);
}
HRESULT ProtectionKey_Version1::SetSoftwarePacketCreationDateTime(time_t creation_datetime)
{
	return WriteBits(CELL_LAST_PACKET_CREATION_DATE, CELL_BIT_SIZE_DATE, (unsigned long)creation_datetime);
}

HRESULT ProtectionKey_Version1::GetSoftwareCurrentDateTime(time_t *current_datetime)
{
	return ReadBits(CELL_CURRENT_DATE, CELL_BIT_SIZE_DATE, (unsigned long*)current_datetime, false);
}
HRESULT ProtectionKey_Version1::SetSoftwareCurrentDateTime(time_t current_datetime)
{
	return WriteBits(CELL_CURRENT_DATE, CELL_BIT_SIZE_DATE, (unsigned long)current_datetime);
}

HRESULT ProtectionKey_Version1::GetHistoryNumber(unsigned long *history_number)
{
	return ReadBits(CELL_HEADER_HISTORY_NUMBER, CELL_BIT_SIZE_HEADER, history_number, false);
}
HRESULT ProtectionKey_Version1::SetHistoryNumber(unsigned short history_number)
{
	return WriteBits(CELL_HEADER_HISTORY_NUMBER, CELL_BIT_SIZE_HEADER, history_number);
}
HRESULT ProtectionKey_Version1::GetSoftwareActivitySlotCurrentActivation(unsigned short activity_index, unsigned long* current_activation)
{
	return (unsigned short)ReadBits(CELL_ACTIVATION_SLOT_LIST[activity_index], CELL_BIT_SIZE_ACTIVATION_CURRENT, current_activation, false);
}
HRESULT ProtectionKey_Version1::SetSoftwareActivitySlotCurrentActivation(unsigned short activity_index, unsigned short current_activation)
{
	return WriteBits(CELL_ACTIVATION_SLOT_LIST[activity_index], CELL_BIT_SIZE_ACTIVATION_CURRENT, current_activation);
}
HRESULT ProtectionKey_Version1::GetSoftwareActivitySlotHoursToExpiration(unsigned short activity_index, unsigned long* hours_to_expiration)
{
	return (unsigned short)ReadBits(CELL_ACTIVATION_SLOT_LIST[activity_index] + (CELL_BIT_SIZE_ACTIVATION_CURRENT * CELL_BIT_SIZE_TO_SUBCELL), CELL_BIT_SIZE_ACTIVATION_HOURS_TO_EXPIRE, hours_to_expiration, false);
}
HRESULT ProtectionKey_Version1::SetSoftwareActivitySlotHoursToExpiration(unsigned short activity_index, unsigned short hours_to_expiration)
{
	return WriteBits(CELL_ACTIVATION_SLOT_LIST[activity_index] + (CELL_BIT_SIZE_ACTIVATION_CURRENT * CELL_BIT_SIZE_TO_SUBCELL), CELL_BIT_SIZE_ACTIVATION_HOURS_TO_EXPIRE, hours_to_expiration);
}
HRESULT ProtectionKey_Version1::UseSoftwareActivation(unsigned short activity_index, unsigned short increase_hours_to_expiration)
{
	HRESULT hr(E_INVALIDARG);
	SafeMutex mutex(license_use_lock);
	try
	{
		//make sure activity_index is in range of valid activity_indexs
		if(activity_index >= _countof(CELL_ACTIVATION_SLOT_LIST))
			throw E_INVALIDARG;
		
		//increase_hours_to_expiration must be no greater than 0xfff
		if(increase_hours_to_expiration > 0xeff)
			throw E_INVALIDARG;

		unsigned long currentActivation(0);
		unsigned long activationHoursLeft(0);
		hr = this->GetSoftwareActivitySlotCurrentActivation(activity_index, &currentActivation);
		if(FAILED(hr)) throw hr;

		hr = this->GetSoftwareActivitySlotHoursToExpiration(activity_index, &activationHoursLeft);
		if(FAILED(hr)) throw hr;
		
		unsigned long new_hours_to_expiration = activationHoursLeft+increase_hours_to_expiration;	//use unsigned long to prevent wrapping
		//make sure there are enough activations and that activationHoursLeft+increase_hours_to_expiration < 0xfff
		if((currentActivation - 1 < 0) || (new_hours_to_expiration >= 0xfff))
			throw E_INVALIDARG;

		hr = this->SetSoftwareActivitySlotCurrentActivation(activity_index, currentActivation-1);
		if(FAILED(hr)) throw hr;

		hr = this->SetSoftwareActivitySlotHoursToExpiration(activity_index, (unsigned short)new_hours_to_expiration);
		if(FAILED(hr)) throw hr;
	}
	catch(HRESULT &eHr)
	{
		hr = eHr;
	}
	catch(_com_error &e)
	{
		hr = e.Error();
	}
	return hr;
}

//private
//bit length can be between 1 to 32, will always return an unsigned long
HRESULT ProtectionKey_Version1::ReadBits(unsigned short offset, unsigned short bits, unsigned long* return_value, bool physical)
{
	HRESULT hr(S_OK);
	SafeMutex mutex(license_use_lock);
//wchar_t tmpBuf[256];
	try
	{
		*return_value = 0;
		unsigned long value = 0;
		// check that the offset is properly aligned by 4
		if (offset % 4 != 0)
			throw _com_error(E_INVALIDARG);

		//Terminology for naming
		//4 bits per cell
		//16 subcells per cell
		unsigned short start_cell_idx = offset / 16;	// first cell number
		unsigned short start_subcell_idx = offset % 16; //offset to read
		unsigned short number_of_subcells_to_read_total = 4 * bits;
		unsigned short number_of_cells_total = (((offset + number_of_subcells_to_read_total) / 16) + 1) - start_cell_idx;
		unsigned long cell_value = 0;
		unsigned short subcells_to_read = 0;
		unsigned short bits_to_shift_right = 0;
		unsigned short bits_to_shift_left = 0;
		for(int idx=0; idx<number_of_cells_total && number_of_subcells_to_read_total>0; idx++)
		{
			//Read Cell
			if(physical)
				cell_value = ReadCellPhysical(start_cell_idx + idx);
			else
				cell_value = ReadCellCache(start_cell_idx + idx);
//wsprintf(tmpBuf, L"ProtectionKey_Version1::ReadBits(0x%x, %d) - 1) cell_value: 0x%x", offset, bits, cell_value);
//OutputDebugStringW(tmpBuf);

			//calculate how to shift cell value to extract
			subcells_to_read = min(16 - start_subcell_idx, number_of_subcells_to_read_total);
			bits_to_shift_right = (16 - start_subcell_idx) - subcells_to_read;
			bits_to_shift_left = max(0, number_of_subcells_to_read_total - subcells_to_read);

			//Mask out part of cell not concerned with
			if(start_subcell_idx == 12) //0xc
				cell_value &= 0x000f;
			else if(start_subcell_idx == 8)
				cell_value &= 0x00ff;
			else if(start_subcell_idx ==4)
				cell_value &= 0x0fff;
			else if(start_subcell_idx ==0)
				cell_value &= 0xffff;

//wsprintf(tmpBuf, L"ProtectionKey_Version1::ReadBits(0x%x, %d) - 2) cell_value: 0x%x", offset, bits, cell_value);
//OutputDebugStringW(tmpBuf);

			//shift cell accordingly
			if(bits_to_shift_right != 0)
				cell_value = cell_value >> bits_to_shift_right;
			else if(bits_to_shift_left != 0)
				cell_value = cell_value << bits_to_shift_left;

//wsprintf(tmpBuf, L"ProtectionKey_Version1::ReadBits(0x%x, %d) - 3) cell_value: 0x%x", offset, bits, cell_value);
//OutputDebugStringW(tmpBuf);

			value +=cell_value;

//wsprintf(tmpBuf, L"ProtectionKey_Version1::ReadBits(0x%x, %d) - 4) value: 0x%x", offset, bits, value);
//OutputDebugStringW(tmpBuf);

			start_subcell_idx = 0;	//the start_subcell_idx will always be 0 except for the first time.
			number_of_subcells_to_read_total -= subcells_to_read;
		}
		*return_value = value;

//wsprintf(tmpBuf, L"ProtectionKey_Version1::ReadBits(0x%x, %d) = 0x%x", offset, bits, *return_value);
//OutputDebugStringW(tmpBuf);

	}
	catch(HRESULT &eHr)
	{
		hr = eHr;
		*return_value = 0;
	}
	catch(_com_error &e)
	{
		hr = e.Error();
		*return_value = 0;
	}
	return hr;
}

HRESULT ProtectionKey_Version1::WriteBits(unsigned short offset, unsigned short bits, unsigned long value)
{
//wchar_t tmpBuf[256];
//wsprintf(tmpBuf, L"ProtectionKey_Version1::WriteBits(0x%x, %d) = 0x%x", offset, bits, value);
//OutputDebugStringW(tmpBuf);
	HRESULT hr(S_OK);
	SafeMutex mutex(license_use_lock);
	try
	{
		// check that the offset is properly aligned by 4
		if (offset % 4 != 0)
			throw _com_error(E_INVALIDARG);

		// only allow for version 1 keys.  Do not allow ever for 
		// version 0 keys, passwords should be only was to update its cells
		unsigned short keyVersion = 0;
		HRESULT hr = GetKeyVersion(&keyVersion);
		if(FAILED(hr))
			throw _com_error(hr);
		if(keyVersion != 1)
			throw _com_error(E_INVALIDARG);

		//Terminology for naming
		//4 bits per cell
		//16 subcells per cell
		unsigned long value_to_write_orig = value;
		unsigned short value_to_write_in_subcells = 0;
		unsigned long new_cell_value = 0;
		unsigned short start_cell_idx = offset / 16;	// first cell number
		unsigned short start_subcell_idx = offset % 16; //offset to write
		unsigned short number_of_subcells_to_write_total = 4 * bits;
		unsigned short number_of_subcells_to_write_in_cell = (start_subcell_idx + number_of_subcells_to_write_total > 16) ? 16 - start_subcell_idx: number_of_subcells_to_write_total;
		unsigned short number_of_cells_total = (((offset + number_of_subcells_to_write_total) / 16) + 1) - start_cell_idx;
		unsigned long cell_value = 0;
		unsigned short subcells_to_write = 0;
		unsigned short bits_to_shift_right = 0;
		unsigned short bits_to_shift_left = 0;
		for(int idx=0; idx<number_of_cells_total && number_of_subcells_to_write_total>0; idx++)
		{
			//Read Cell
			cell_value = ReadCellPhysical(start_cell_idx + idx);

			//Calculate how to shift incoming value to fit in to cell, opposite of read
			subcells_to_write = min(16 - start_subcell_idx, number_of_subcells_to_write_total);
			bits_to_shift_left = min((16 - start_subcell_idx) - subcells_to_write, 16 - start_subcell_idx);
			bits_to_shift_right = max(0, number_of_subcells_to_write_total - subcells_to_write);

			// Keep the part to the left of start index
			// The part before what needs to be written
			if(start_subcell_idx == 12) //0xc
				new_cell_value = cell_value & 0xfff0;
			else if(start_subcell_idx == 8)
				new_cell_value = cell_value & 0xff00;
			else if(start_subcell_idx ==4)
				new_cell_value = cell_value & 0xf000;
			else if(start_subcell_idx ==0)
				new_cell_value = 0;	//keep nothing from old cell

//wsprintf(tmpBuf, L"ProtectionKey_Version1::WriteBits(0x%x, %d) 1 - new_cell_value: 0x%x", offset, bits, new_cell_value);
//OutputDebugStringW(tmpBuf);

			if(bits_to_shift_right != 0)
			{
				value_to_write_in_subcells = (unsigned short)(value_to_write_orig >> bits_to_shift_right);
				value_to_write_orig -= value_to_write_in_subcells << bits_to_shift_right;
			}
			else if(bits_to_shift_left != 0)
			{
				value_to_write_in_subcells = (unsigned short)(value_to_write_orig << bits_to_shift_left);
				value_to_write_orig -= value_to_write_in_subcells >> bits_to_shift_left;
			}
			else
			{
				value_to_write_in_subcells = (unsigned short)value_to_write_orig;
				value_to_write_orig -= value_to_write_in_subcells;
			}
			new_cell_value += value_to_write_in_subcells;

//wsprintf(tmpBuf, L"ProtectionKey_Version1::WriteBits(0x%x, %d) 2 - new_cell_value: 0x%x", offset, bits, new_cell_value);
//OutputDebugStringW(tmpBuf);

			// Keep the part after the end
			// The part after what needs to be written
			// The amount we need to shift the value to write also says what needs to be saved
			int cell_to_keep = max(16-(start_subcell_idx + number_of_subcells_to_write_in_cell), 0);
			if(cell_to_keep == 12)
				new_cell_value += cell_value & 0x0fff;
			else if(cell_to_keep == 8)
				new_cell_value += cell_value & 0x00ff;
			else if(cell_to_keep == 4)
				new_cell_value += cell_value & 0x000f;

//wsprintf(tmpBuf, L"ProtectionKey_Version1::WriteBits(0x%x, %d) 3 - new_cell_value: 0x%x, orig_cell_value: 0x%x", offset, bits, new_cell_value, cell_value);
//OutputDebugStringW(tmpBuf);
			WriteCellPhysical(start_cell_idx + idx, (unsigned short)new_cell_value);

			number_of_subcells_to_write_total -= number_of_subcells_to_write_in_cell;
			start_subcell_idx = 0;
			number_of_subcells_to_write_in_cell = (start_subcell_idx + number_of_subcells_to_write_total > 16) ? 16 - start_subcell_idx: number_of_subcells_to_write_total;
		}
	}
	catch(HRESULT &eHr)
	{
		hr = eHr;
	}
	catch(_com_error &e)
	{
		hr = e.Error();
	}
	return hr;
}

unsigned short ProtectionKey_Version1::ReadCellCache(unsigned short cell)
{
	SafeMutex mutex(cells_lock);
	if (cell>=KeyCellCount)
		throw _com_error(E_INVALIDARG);
	return cells[cell];
}