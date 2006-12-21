#include "ProtectionKey.h"
#include "KeyError.h"
#include "KeyMessages.h"
#include "..\common\MultidimensionalSafeArray.h"
#include "..\common\SafeMutex.h"
#include "..\common\Version.h"
#include "..\common\CryptoHelper.h"
#include "..\common\TimeHelper.h"
#include "KeyServerInstance.h"
#include <time.h>

/* KDPasswordText[]
 *    This is the text for Test/Dev key types which can be any key.
 */
const char* ProtectionKey::KDPasswordText[] =
{                        			// Extension Unit Value
   "30 days/85 hrs - TL",        // 1 Elements #1-#12 are used to deliver prorated Test Licenses (30 days/85 hour increments)
   "60 days/170 hrs - TL",       // 2
   "90 days/255 hrs - TL",       // 3
   "120 days/340 hrs - TL",      // 4
   "150 days/425 hrs - TL",      // 5
   "180 days/510 hrs - TL",      // 6
   "210 days/595 hrs - TL",      // 7
   "240 days/680 hrs - TL",      // 8
   "270 days/765 hrs - TL",      // 9
   "300 days/850 hrs - TL",      // 10
   "330 days/935 hrs - TL",      // 11
   "365 days/1000 hrs - TL",     // 12
   "365 days/9000 hrs - DL",     // 13 Element #13 is the Development License
   "365 days/250 hrs - PDSL",    // 14 Elements #14-#17 are Partner Demo and Support Licenses
   "365 days/375 hrs - PDSL",    // 15
   "365 days/500 hrs - PDSL",    // 16
   "365 days/750 hrs - PDSL",    // 17
   "730 days/18000 hrs - MYDL",  // 18 Elements #18-#21 are multi-year prepaid Development Licenses
   "1095 days/27000 hrs - MYDL", // 19
   "1460 days/36000 hrs - MYDL", // 20
   "1825 days/45000 hrs - MYDL", // 21
   "730 days/2000 hrs - MYTL",   // 22 Elements #22-#25 are multi-year prepaid Test License
   "1095 days/3000 hrs - MYTL",  // 23
   "1460 days/4000 hrs - MYTL",  // 24
   "1825 days/5000 hrs - MYTL",  // 25

};
/* KDPasswordDays[]
 *    This is the text for Test/Dev key types which can be any key.
 */
const unsigned short ProtectionKey::KDPasswordDays[] =
{        // Extension Unit Value
   30,  	// 1 Elements #1-#12 are used to deliver prorated Test Licenses (30 days/85 hour increments)
   60,  	// 2
   90, 	// 3
   120, 	// 4
   150, 	// 5
   180, 	// 6
   210, 	// 7
   240, 	// 8
   270, 	// 9
   300, 	// 10
   330, 	// 11
   365, 	// 12
   365, 	// 13 Element #13 is the Development License
   365, 	// 14 Elements #14-#17 are Partner Demo and Support Licenses
   365, 	// 15
   365, 	// 16
   365, 	// 17
   730, 	// 18 Elements #18-#21 are multi-year prepaid Development Licenses
   1095, // 19
   1460, // 20
   1825, // 21
   730,  // 22 Elements #22-#25 are multi-year prepaid Test License
   1095, // 23
   1460, // 24
   1825, // 25
};

/* KDPasswordHours[]
 *    This is the text for Test/Dev key types which can be any key.
 */
const unsigned short ProtectionKey::KDPasswordHours[] =
{           // Extension Unit Value
   85,      // 1 Elements #1-#12 are used to deliver prorated Test Licenses (30 days/85 hour increments)
   170,     // 2
   255,     // 3
   340,     // 4
   425,     // 5
   510,     // 6
   595,     // 7
   680,     // 8
   765,     // 9
   850,     // 10
   935,     // 11
   1000,    // 12
   9000,    // 13 Element #13 is the Development License
   250,    	// 14 Elements #14-#17 are Partner Demo and Support Licenses
   375,    	// 15
   500,    	// 16
   750,    	// 17
   18000,  	// 18 Elements #18-#21 are multi-year prepaid Development Licenses
   27000, 	// 19
   36000, 	// 20
   45000, 	// 21
   2000,  	// 22 Elements #22-#25 are multi-year prepaid Test License
   3000,  	// 23
   4000,   	// 24
   5000, 	// 25
};

const BYTE ProtectionKey::predefined_queries[8][5] = 
{
	{0x14, 0xD0, 0x15, 0x20, 0x30}, // primary1
	{0xA3, 0xBF, 0xD2, 0xA0, 0xF1}, // primout1
	{0xA1, 0x4C, 0xDB, 0x15, 0x23}, // primary2
	{0x2B, 0xD4, 0x29, 0x8D, 0xA7}, // primout2
	{0xFF, 0xAB, 0xCE, 0xDD, 0x84}, // secondary1
	{0x92, 0x08, 0x36, 0x2F, 0x3D}, // secondout1
	{0xF2, 0xFA, 0xAB, 0xEC, 0x14}, // secondary2
	{0x75, 0x2E, 0xB1, 0xBF, 0x2E}  // secondout2
};


void OutputFormattedDebugString(const wchar_t *fmt, ...)
{
	va_list list;
	va_start(list, fmt);
	wchar_t buf[1024];
	memset(buf, 0, sizeof(buf));
	_vsnwprintf(buf, 1024, fmt, list);
	OutputDebugStringW(buf);
}



ProtectionKey::ProtectionKey() : m_keyspec(0), m_driver(0)
{
	;
}

ProtectionKey::ProtectionKey(const ProtectionKey &k) : 
	key_owned(false),
	cells_lock(CreateMutex(0,0,0)),
	license_use_lock(CreateMutex(0,0,0)),
	m_keyident(k.m_keyident), 
	m_keyspec(k.m_keyspec), 
	m_driver(k.m_driver)
{
	try
	{
		UpdateCellCache();
	}
	catch (_com_error &e)
	{
		OutputDebugStringW(L"ProtectionKey::ProtectionKey() - catch (_com_error &e)");
		e.Error();
	}
}

ProtectionKey::ProtectionKey(_bstr_t keyident, KeySpec *keyspec, RainbowDriver *driver) : 
	key_owned(false),
	cells_lock(CreateMutex(0,0,0)),
	license_use_lock(CreateMutex(0,0,0)),
	m_keyident(keyident),
	m_keyspec(keyspec),
	m_driver(driver)
{
	try
	{
		UpdateCellCache();
	}
	catch (_com_error &e)
	{
		OutputDebugStringW(L"ProtectionKey::ProtectionKey() - catch (_com_error &e)");
		e.Error();
	}
}


HRESULT ProtectionKey::TrialExpires(VARIANT *expire_date)
{
	*expire_date = _variant_t(0.0,VT_DATE);
	try
	{
		// convert the time_t on the key in to a variant date
		*expire_date = TimeHelper::TimeTToVariant(ReadHeaderCache(L"Expiration Date").ulVal);
	}
	catch(_com_error &e)
	{
		return e.Error();
	}
	return S_OK;
}

HRESULT ProtectionKey::TrialHours(long *trial_hours)
{
	HRESULT hr = S_OK;
	
	*trial_hours = 0;
	
	// see TimesUp() for similar code
	// check time if key is trial
	if(isOnTrial())
	{
		unsigned short key_type = ReadHeaderCache(L"Key Type").uiVal;
		unsigned short key_status = ReadHeaderCache(L"Status").uiVal;
		unsigned short initial_counter = ReadHeaderCache(_bstr_t(L"Initial Counter")).uiVal;
		unsigned short extended_counter = ReadHeaderCache(_bstr_t(L"Extended Counter")).uiVal;
		
		unsigned short counter = (key_status==INITIAL_TRIAL ? initial_counter : extended_counter);
		
		//xxx do we need a special case for KEYDevelopment keys?
		*trial_hours = counter;
	}
	
	return hr;
}

HRESULT ProtectionKey::IsActive(VARIANT_BOOL *key_active)
{
	try
	{
		// key has a customer number, key number, and is not an expired key
		*key_active = ((!isOnTrial() || !TimesUp())
			? VARIANT_TRUE : VARIANT_FALSE);
	}
	catch(_com_error &e)
	{
		return e.Error();
	}

	return S_OK;
}

HRESULT ProtectionKey::IsProgrammed(VARIANT_BOOL *key_programmed)
{
	*key_programmed = VARIANT_FALSE;
	try
	{
		*key_programmed = (ReadHeaderCache(L"Customer Number").uiVal || ReadHeaderCache(L"Key Number").uiVal ? VARIANT_TRUE : VARIANT_FALSE);
	}
	catch(_com_error &e)
	{
		return e.Error();
	}
	return S_OK;
}

HRESULT ProtectionKey::HeaderQuery(long header_ident, VARIANT *value)
{
	HRESULT hr = S_OK;
	try
	{
		*value = ReadHeaderCache(header_ident).Detach();
	}
	catch (_com_error &e)
	{
		hr = e.Error();
	}

	return hr;
}

HRESULT ProtectionKey::IsPresent(VARIANT_BOOL *key_present)
{
	HRESULT hr = S_OK;

	*key_present = VARIANT_FALSE;

	try
	{
		KeySpec::Header &header(m_keyspec->headers[L"Key Number"]);
		unsigned short value;
		hr = m_driver->ReadCell(m_keyident, header.offset >> 4, &value);
		if (FAILED(hr))
			throw _com_error(hr);
		
		// we rely on a successful key read to determine that the key is still attached.
		*key_present = VARIANT_TRUE;
	}
	catch (_com_error &e)
	{
		hr = e.Error();
	}
	
	if (hr == LicenseServerError::EHR_SP_UNIT_NOT_FOUND)
	{
		*key_present = VARIANT_FALSE;
		return S_OK;
	}
	
	return hr;
}

HRESULT ProtectionKey::Lock(BSTR license_id)
{
	SafeMutex mutex(license_use_lock);
	if (key_owned)
	{
		return LicenseServerError::EHR_KEY_LOCKED;
	}
	else
	{
		key_owned = true;
		key_owner = _bstr_t(license_id,true);
	}
	return S_OK;
}

HRESULT ProtectionKey::Unlock(BSTR license_id)
{
	SafeMutex mutex(license_use_lock);
	if (key_owned)
	{
		if (key_owner==_bstr_t(license_id,true))
		{
			key_owned = false;
		}
		else
		{
			// error case, some other client owns the key
			// it cannot be unlocked by this client
			return E_FAIL;
		}
	}
	return S_OK;
}

HRESULT ProtectionKey::Obtain(BSTR license_id)
{
	SafeMutex mutex(license_use_lock);
	key_use[_bstr_t(license_id,true)]++;
	return S_OK;
}

HRESULT ProtectionKey::Release(BSTR license_id)
{
	SafeMutex mutex(license_use_lock);
	key_use[_bstr_t(license_id,true)]--;
	if (key_use[_bstr_t(license_id,true)]<0)
	{
		keyserver.GenerateMessage(m_keyident, MT_INFO, S_OK, time(0), MessageTooManyReleases);
		key_use[_bstr_t(license_id,true)]=0;
	}
	return S_OK;
}


HRESULT ProtectionKey::ValidateLicense(BSTR license_id, VARIANT_BOOL *license_valid)
{
	HRESULT hr = S_OK;
	
	// is still present, activated, and programmed
	hr = IsPresent(license_valid);
	if (FAILED(hr)) return hr;
	if (*license_valid==VARIANT_FALSE) return S_OK;
	
	hr = IsProgrammed(license_valid);
	if (FAILED(hr)) return hr;
	if (*license_valid==VARIANT_FALSE) return S_OK;
	
	hr = IsActive(license_valid);
	if (FAILED(hr)) return hr;
	if (*license_valid==VARIANT_FALSE) return S_OK;
	
	
	// check that none of the keys are in license violation (obtained licenses do not exceed total licenses)
	
	// get the product info
	unsigned short product_ident = ReadHeaderCache(L"Product ID").uiVal;
	KeySpec::Product &product(m_keyspec->products[product_ident]);
	
	// for each module
	for (KeySpec::Product::data_list_t::iterator module = product.data.begin(); module != product.data.end(); ++module)
	{
		// if this is a license module
		if (module->isLicense)
		{
			_variant_t module_value = ReadModuleCache(module->id);
			SafeMutex mutex(license_use_lock);
			
			long module_use=0;
			for (LicenseUseList::iterator use = license_use.begin(); use != license_use.end(); ++use)
			{
				module_use += use->second[module->id];
			}
			if (module_use > (long)LicenseEffectiveValue(module->id))
			{
				*license_valid = VARIANT_FALSE;
				break;
			}
		}
	}
	
	return S_OK;
}

HRESULT ProtectionKey::ModuleEnumerate(VARIANT *pvtModuleList)
{
	HRESULT hr = S_OK;
	
	VariantInit(pvtModuleList);
	
	try
	{
		// get product object out of the key spec
		KeySpec::Product &product(m_keyspec->products[ReadHeaderCache(L"Product ID").uiVal]);
		
		MultidimensionalSafeArray::DimensionsType dims,index;
		dims.push_back((unsigned int)product.data.size());			// number of elements
		dims.push_back((unsigned int)2);							// [id,name]
		
		hr = MultidimensionalSafeArray::CreateMultidimensionalSafearray(pvtModuleList, dims);
		
		if (SUCCEEDED(hr))
		{
			for (unsigned int module = 0; module<dims[0]; ++module)
			{
				VARIANT *pElement;
				index.push_back(module);
				
				// id
				index.push_back(0);
				MultidimensionalSafeArray::AccessMultidimensionalSafearray(pvtModuleList, index, &pElement);
				VariantCopy(pElement,&_variant_t(product.data[module].id));
				MultidimensionalSafeArray::UnaccessMultidimensionalSafearray(pvtModuleList, index);
				index.pop_back();
				
				// name
				index.push_back(1);
				MultidimensionalSafeArray::AccessMultidimensionalSafearray(pvtModuleList, index, &pElement);
				VariantCopy(pElement,&_variant_t(_bstr_t(product.data[module].name)));
				MultidimensionalSafeArray::UnaccessMultidimensionalSafearray(pvtModuleList, index);
				index.pop_back();
				
				index.pop_back();
			}
		}
	}
	catch(_com_error &e)
	{
		hr = e.Error();
	}

	return hr;
}

HRESULT ProtectionKey::ModuleQuery(long module_ident, VARIANT *vtValue)
{
	HRESULT hr = S_OK;
	
	VariantInit(vtValue);
	
	try
	{
		*vtValue = ReadModuleCache(module_ident).Detach();
	}
	catch (_com_error &e)
	{
		hr = e.Error();
	}
	
	return hr;
}

HRESULT ProtectionKey::ModuleLicenseTotal(BSTR license_id, long module_ident, long* license_count)
{
	// lock a license
	HRESULT hr = S_OK;
	
	*license_count=0;
	
	try
	{
		_variant_t module_value = ReadModuleCache(module_ident);
		KeySpec::Module &module(m_keyspec->products[ReadHeaderCache(L"Product ID").uiVal][module_ident]);
		if (!module.isLicense)
			return E_INVALIDARG;
		
		unsigned int effective_value = LicenseEffectiveValue(module.id);
		if (*license_count >= 0x7FFFFFFF || effective_value>= 0x7FFFFFFF)
			*license_count = 0x7FFFFFFF;
		else
			*license_count += (long) effective_value;
	}
	catch (_com_error &e)
	{
		hr = e.Error();
	}
	
	return hr;
}


//Counts the ModuleInUse based on the license_id
HRESULT ProtectionKey::ModuleLicenseInUse(BSTR license_id, long module_ident, long* license_count)
{
	// lock a license
	HRESULT hr = S_OK;
	try
	{
		_variant_t module_value = ReadModuleCache(module_ident);
		KeySpec::Module &module(m_keyspec->products[ReadHeaderCache(L"Product ID").uiVal][module_ident]);
		SafeMutex mutex(license_use_lock);
		ModuleLicenseUseList &module_license_use = license_use[license_id];
		*license_count = module_license_use[module.id];
	}
	catch (_com_error &e)
	{
		hr = e.Error();
	}
	return hr;
}

//Counts up all the ModuleInUse on the key.
HRESULT ProtectionKey::ModuleInUse(long module_ident, long* license_count)
{
	// lock a license
	HRESULT hr = S_OK;
	
	*license_count=0;
	
	try
	{
		KeySpec::Module &module(m_keyspec->products[ReadHeaderCache(L"Product ID").uiVal][module_ident]);
		
		if (!module.isLicense)
			return E_INVALIDARG;
		
		SafeMutex mutex(license_use_lock);
		// return the number of licenses that are in use
		for (LicenseUseList::iterator i = license_use.begin(); i != license_use.end(); ++i)
		{
			*license_count += i->second[module.id];
		}
	}
	catch (_com_error &e)
	{
		hr = e.Error();
	}

	return hr;
}

HRESULT ProtectionKey::ModuleLicenseObtain(BSTR license_id, long module_ident, long license_count)
{
	// lock a license
	HRESULT hr = S_OK;
	
	try
	{
		_variant_t module_value = ReadModuleCache(module_ident);
		KeySpec::Module &module(m_keyspec->products[ReadHeaderCache(L"Product ID").uiVal][module_ident]);
		
		SafeMutex mutex(license_use_lock);
		
		ModuleLicenseUseList &module_license_use = license_use[license_id];
		if (module_license_use[module.id] + license_count <= (long)LicenseEffectiveValue(module.id))
		{
			module_license_use[module.id] += license_count;
		}
		else
		{
			throw _com_error(LicenseServerError::EHR_LICENSE_INSUFFICIENT);
		}
	}
	catch (_com_error &e)
	{
		hr = e.Error();
	}
	return hr;
}

HRESULT ProtectionKey::ModuleLicenseRelease(BSTR license_id, long module_ident, long license_count)
{
	HRESULT hr = S_OK;
	
	try
	{
		_variant_t module_value = ReadModuleCache(module_ident);
		KeySpec::Module &module(m_keyspec->products[ReadHeaderCache(L"Product ID").uiVal][module_ident]);
		
		SafeMutex mutex(license_use_lock);
		
		LicenseUseList::iterator i = license_use.find(license_id);
		if (i!=license_use.end())
			i->second[module.id] = max(i->second[module.id]-license_count,0);
	}
	catch (_com_error &e)
	{
		hr = e.Error();
	}
	
	return hr;
}
//returns E_FAIL if there is TrialKey
HRESULT ProtectionKey::ModuleLicenseDecrementCounter(BSTR license_id, long module_ident, long license_count)
{
	HRESULT hr = E_FAIL;
	try
	{
		SafeMutex mutex(license_use_lock);
		unsigned short key_status = ReadHeaderCache(_bstr_t(L"Status")).uiVal;
		KeySpec::Module &module(m_keyspec->products[ReadHeaderCache(L"Product ID").uiVal][module_ident]);

		//Only decrement counter if license count is not unlimited and not key status is not initial_trial
		if(module.isCounter && !LicenseIsUnlimited(module.id) && key_status != INITIAL_TRIAL)
		{
			hr = S_OK;
			if(module.fn_read(ReadModuleCache(module.id).ulVal) >= module.counter+license_count)
			{
				module.counter+=license_count;
				hr = S_OK;
				if(module.counter / module.fn_read(1))	//Actually decrement from the key.
				{
					hr = WriteLicenseDecrementCounter(module_ident, module.counter);
					module.counter = module.counter % module.fn_read(1); 
				}
			}
			else
			{
				throw _com_error(LicenseServerError::EHR_LICENSE_INSUFFICIENT);
			}
		}
	}
	catch (_com_error &e)
	{
		hr = e.Error();
	}
	return hr;
}
HRESULT ProtectionKey::LicenseReleaseAll(BSTR license_id)
{
	SafeMutex mutex(license_use_lock);
	
	// release the key for this license id
	key_use.erase(_bstr_t(license_id,true));
	
	// release the licenses obtained for this license id
	license_use.erase(_bstr_t(license_id,true));
	
	// unlock the key if this id has the lock
	if (key_owned && key_owner==_bstr_t(license_id,true))
		key_owned = false;
	
	return S_OK;
}

// Sets all writable cells on a key to zero
HRESULT ProtectionKey::Format(BSTR *new_key_ident)
{
	HRESULT hr = S_OK;
	
	// lock the driver's key list so that these operations will not affect the key_ident we reference here until finished
	SafeMutex mutex(m_driver->keys_lock);
	
	// zero out the module and header regions of the key (leaving only the 
	// for each region of the key storage
	for (KeySpec::KeyRegionList::iterator region = m_keyspec->regions.begin(); region != m_keyspec->regions.end(); ++region)
	{
		// if the region is module storage
		if (0==wcsncmp(region->type,L"module",7) || 0==wcsncmp(region->type,L"header",7))
		{
			// zero out each cell in the region
			for (unsigned int cell=(region->offset/16); cell<(unsigned int)((region->offset + region->bits)/16); ++cell)
			{
				try{FormatCellPhysical(cell);}
				catch(_com_error &e){return e.Error();}
			}
		}
	}
	
	// write a guid identifier to the key
	hr = m_driver->WriteKeyUnprogrammedIdentifier(_bstr_t(m_keyident,true));
	
	// obtain the string representation of the guid to use as the key identifier
	if (new_key_ident)
	{
		hr = m_driver->ComputeCurrentKeyIdent(_bstr_t(m_keyident,true), new_key_ident);
	}
	
	return S_OK;
}



// Programs a key
	/*
	VARIANT
		SAFEARRAY			
			VARIANT				(entry per each module)
				SAFEARRAY	
					VARIANT		(module id)
					VARIANT		(license count/value)
			VARIANT
				SAFEARRAY	
					VARIANT		(module id)
					VARIANT		(license count/value)
			...
	*/
HRESULT ProtectionKey::Program(long customer_number, long key_number, long product_ident, long ver_major, long ver_minor, long key_type, long days, VARIANT module_value_list, BSTR *new_key_ident)
{
	HRESULT hr = S_OK;
	
//xxx debug
OutputFormattedDebugString(L"ProtectionKey::Program(%d, %d, %d, %d, %d, %d, %d, module_list size: %d, ?)", customer_number, key_number, product_ident, ver_major, ver_minor, key_type, days, (module_value_list.vt & VT_ARRAY) ? module_value_list.parray->rgsabound[0].cElements : 0);
	
	unsigned short product_version = (unsigned short)(Version::TinyVersion(Version::ModuleVersion(ver_major, ver_minor, 0, 0)).program);
	
	// lock the driver's key list so that these operations will not affect the key_ident we reference here until finished
	SafeMutex mutex(m_driver->keys_lock);
	
	hr = Format();
	if (FAILED(hr)) return hr;
	
//xxx debug
OutputFormattedDebugString(L"ProtectionKey::Program() -- Writing headers (Product: %d, Version: %08x, Key Type: %d, Status: INITIAL_TRIAL", product_ident, product_version, key_type);

	// write the header data
	WriteHeader(L"Product ID",(unsigned int)(unsigned short)product_ident);
	WriteHeader(L"Product Version",(unsigned int)(unsigned short)product_version);
	WriteHeader(L"Key Type",(unsigned int)(unsigned short)key_type);
	WriteHeader(L"Status",(unsigned int)(unsigned short)INITIAL_TRIAL);
	
//xxx debug
OutputFormattedDebugString(L"ProtectionKey::Program() -- Call WriteCounterDays(%d)", days);
	
	// write the initial/extended counter
	hr = WriteCounterDays((unsigned short)days);		if (FAILED(hr)) throw _com_error(hr);
	
//xxx debug
OutputFormattedDebugString(L"ProtectionKey::Program() -- Call WriteExpirationDays(%d)", days);
	
	// write the expiration date
	hr = WriteExpirationDays((unsigned short)days);		if (FAILED(hr)) throw _com_error(hr);
	
//xxx debug
OutputFormattedDebugString(L"ProtectionKey::Program() -- Writing headers (Customer: %x, Key Number %x)", customer_number, key_number);
	
	WriteHeader(L"Customer Number",(unsigned int)(unsigned short)customer_number);
	WriteHeader(L"Key Number",(unsigned int)(unsigned short)key_number);
	
	// get the new key id for this key now that the customer number and key number are written
	if (new_key_ident)
	{
//xxx debug
OutputFormattedDebugString(L"ProtectionKey::Program() -- Calling ComputeCurrentKeyIdent()");
		
	    hr = m_driver->ComputeCurrentKeyIdent(m_keyident, new_key_ident);
		if (FAILED(hr)) throw _com_error(hr);
	}

	m_driver->ClearKeyUnprogrammedIdentifier(m_keyident);
	InitializePasswordNumber();	// Set the module password number to 0
	
//xxx debug
OutputFormattedDebugString(L"ProtectionKey::Program() -- Writing headers (Primary descriptor, primary password, secondary descriptor)");
	
	KeySpec::Header &header_primary_descriptor = m_keyspec->headers[L"Primary Descriptor"];
	KeySpec::Header &header_secondary_descriptor = m_keyspec->headers[L"Secondary Descriptor"];
	KeySpec::Header &header_primary_password = m_keyspec->headers[L"Primary Password"];
	WriteHeader(header_primary_descriptor.id, header_primary_descriptor.default_value);
	WriteHeader(header_primary_password.id, header_primary_password.default_value);
	WriteHeader(header_secondary_descriptor.id, header_secondary_descriptor.default_value);
			
	
	

//xxx debug
OutputFormattedDebugString(L"ProtectionKey::Program() -- Writing default module data");
	
	// write the default module data if provided
	//xxx I don't know if this is supposed to happen
	//xxx review what happens when you program a key
	// for each module for this product
	KeySpec::Product &product(m_keyspec->products[product_ident]);
	for (KeySpec::Product::data_list_t::iterator module = product.data.begin(); module != product.data.end(); ++module)
	{
//xxx debug
OutputFormattedDebugString(L"ProtectionKey::Program() -- Writing default module data (<%04x><%02d>  %08x)", module->offset, module->bits, (unsigned int)module->default_license);
		WriteBitsPhysical(module->offset, module->bits, (unsigned int)module->default_license);
	}
	
//xxx debug
	OutputFormattedDebugString(L"ProtectionKey::Program() -- Writing creator provided module default overrides");
	
	// write the values specified in the module value list if appropriate
	if (module_value_list.vt==(VT_ARRAY | VT_VARIANT))
	{
		MultidimensionalSafeArray::DimensionsType dims, index;
		hr = MultidimensionalSafeArray::SizeofMultidimensionalSafearray(&module_value_list, dims);
		if (SUCCEEDED(hr) && dims.size()==2)
		{
			// for each module license
			for (unsigned int row=0; row<dims[0]; ++row)
			{
				VARIANT *pElement = 0;
				unsigned int module_id = 0;
				unsigned int module_value = 0;
				index.push_back(row);
					index.push_back(0);
						MultidimensionalSafeArray::AccessMultidimensionalSafearray(&module_value_list, index, &pElement);
						module_id = (unsigned int)pElement->ulVal;
						MultidimensionalSafeArray::UnaccessMultidimensionalSafearray(&module_value_list, index);
					index.pop_back();
					index.push_back(1);
						MultidimensionalSafeArray::AccessMultidimensionalSafearray(&module_value_list, index, &pElement);
						module_value = (unsigned int)pElement->ulVal;
						MultidimensionalSafeArray::UnaccessMultidimensionalSafearray(&module_value_list, index);
					index.pop_back();
				index.pop_back();
				
				try
				{
					KeySpec::Module &module(m_keyspec->products[product_ident][module_id]);
					if (module.isLicense)
					{
//xxx debug
OutputFormattedDebugString(L"ProtectionKey::Program() -- Writing creator provided module default overrides WriteLicense(%d, %08x)", module_id, module_value);
						WriteLicense(module_id, module_value);
					}
					else
					{
//xxx debug
OutputFormattedDebugString(L"ProtectionKey::Program() -- Writing creator provided module default overrides WriteModule(%d, %08x)", module_id, module_value);
						WriteModule(module_id, module_value);
					}
				}
				catch (_com_error &e)
				{
					//xxx debug
					OutputFormattedDebugString(L"ProtectionKey::Program() -- Invalid Argument: WriteLicense or WriteModule (%d, %d)", module_id, module_value);
					return e.Error();
				}
			}
		}
	}
	else if (module_value_list.vt != VT_EMPTY)
	{
		//xxx debug
		OutputFormattedDebugString(L"ProtectionKey::Program() Invalid Argument: module_value_list.vt == %08x", module_value_list.vt);
		return E_INVALIDARG;
	}
OutputFormattedDebugString(L"ProtectionKey::Program() -- Leave");	
	return S_OK;
}

// Reads raw data off of the key
HRESULT ProtectionKey::ReadRaw(VARIANT *pvtKeyData)
{
	HRESULT hr = S_OK;
	
	// package up the cell cache data in to a safearray
	BYTE *pKeyData;
	DWORD *pKeyDataHeader;
	VariantInit(pvtKeyData);
	pvtKeyData->vt = VT_ARRAY | VT_UI1;
	pvtKeyData->parray = SafeArrayCreateVector(VT_UI1, 0, sizeof(DWORD)*8+sizeof(cells)+sizeof(cells_valid)+sizeof(RainbowDriver::accessCode));
	hr = SafeArrayAccessData(pvtKeyData->parray, (void**)&pKeyData);
	if (FAILED(hr)) return hr;
	pKeyDataHeader=(DWORD*)pKeyData;
	pKeyDataHeader[0] = 1;											// magic value, indicates data format (currently this is the only format)
	pKeyDataHeader[1] = sizeof(cells)/sizeof(cells[0]);				// the number of key cells
	pKeyDataHeader[2] =	8*sizeof(DWORD);							// offset of the cells data
	pKeyDataHeader[3] = pKeyDataHeader[2] + sizeof(cells);			// offset of the cells_valid data
	pKeyDataHeader[4] = pKeyDataHeader[3] + sizeof(cells_valid);	// offset of the accessCode data
	pKeyDataHeader[5] = 0;											// not currently used
	pKeyDataHeader[6] = 0;											// not currently used
	pKeyDataHeader[7] = 0;											// not currently used
	
	{// get a lock on the cached data
		SafeMutex mutex(cells_lock);
		memcpy(pKeyData+pKeyDataHeader[2], cells, sizeof(cells));
		memcpy(pKeyData+pKeyDataHeader[3], cells_valid, sizeof(cells_valid));
	}

	memcpy(pKeyData+pKeyDataHeader[4], RainbowDriver::accessCode, sizeof(RainbowDriver::accessCode));
	hr = SafeArrayUnaccessData(pvtKeyData->parray);
	
	return hr;
}

unsigned short ProtectionKey::ReadCellCache(unsigned short cell)
{
	SafeMutex mutex(cells_lock);
	if (cell>=KeyCellCount)
		throw _com_error(E_INVALIDARG);
	
	if (FAILED(cells_valid[cell]))
		throw _com_error(cells_valid[cell]);
	
	return cells[cell];
}

// storage sizes in bits must be a power of 2 and no greater than 32 {1,2,...16,32}
// offsets must be aligned to an integral multiple of bits
_variant_t ProtectionKey::ReadBitsCache(unsigned short offset, unsigned short bits)
{
	return ReadBits(offset, bits, false);
}


void ProtectionKey::UpdateCellCache()
{
	// Reads the key contents in to a temporary cache, 
	// then copies the temporary cache to the persistent cache.
	// The extra level of indirection is for performance reasons,
	// reading from the physical key takes some time, the cache
	// should be highly available and not blocking on key reads.

	bool some_valid_cells = false;
	
	unsigned short tmp_cells[KeyCellCount];
	HRESULT tmp_cells_valid[KeyCellCount];
	
	memset(tmp_cells,0,sizeof(tmp_cells));
	memset(tmp_cells_valid,0,sizeof(tmp_cells));
	for (unsigned int i=0; i<ProtectionKey::KeyCellCount; ++i)
	{
		if (RainbowDriver::accessCode[i]==RainbowDriver::ALGORITHM)
		{
			tmp_cells[i] = 0;
			tmp_cells_valid[i] = LicenseServerError::EHR_SP_ACCESS_DENIED;
		}
		else
		{
			tmp_cells_valid[i]=S_OK;
			try {tmp_cells[i]=ReadCellPhysical(i);}
			catch (_com_error &e) {tmp_cells_valid[i]=e.Error();}
			if (SUCCEEDED(tmp_cells_valid[i]))
				some_valid_cells = true;
		}
	}

	{ // copy from the temp cache to the persistent cache
		SafeMutex mutex(cells_lock);
		memcpy(cells,tmp_cells,sizeof(cells));
		memcpy(cells_valid,tmp_cells_valid,sizeof(cells_valid));
	}
	
	if (!some_valid_cells)
		throw _com_error(E_FAIL);
}

void ProtectionKey::UpdateCellCache(unsigned int cell)
{
	// update the cell cache
	unsigned short c_value = 0;
	HRESULT c_valid = S_OK;
	try {c_value=ReadCellPhysical(cell);}
	catch (_com_error &e) {c_valid=e.Error();}
	
	{ // lock and update
		SafeMutex mutex(cells_lock);
		cells[cell] = c_value;
		cells_valid[cell] = c_valid;
	}
}


// returns true if a trial key has expired
bool ProtectionKey::TimesUp()
{	
	if(isOnTrial())
	{
		unsigned short key_type = ReadHeaderCache(L"Key Type").uiVal;
		unsigned short key_status = ReadHeaderCache(L"Status").uiVal;
		unsigned short initial_counter = ReadHeaderCache(_bstr_t(L"Initial Counter")).uiVal;
		unsigned short extended_counter = ReadHeaderCache(_bstr_t(L"Extended Counter")).uiVal;
		time_t expiration_date = (time_t)ReadHeaderCache(_bstr_t(L"Expiration Date")).ulVal;
		time_t cur_date = time(NULL);
		
		unsigned short counter = (key_status==INITIAL_TRIAL ? initial_counter : extended_counter);
		
		if (key_type == KEYDevelopment)
		{
			if (cur_date >= expiration_date || counter == 0)
			{
				return true;
			}
		}
		else
		{
			if (cur_date >= expiration_date)
			{
				return true;
			}
		}
	}
	return false;
}

// returns true if it looks like the pc clock has been set back
bool ProtectionKey::TimesUpClockViolation()
{
	//check time if key is trial
	if(isOnTrial())
	{
		unsigned short key_type = ReadHeaderCache(L"Key Type").uiVal;
		unsigned short key_status = ReadHeaderCache(L"Status").uiVal;
		unsigned short initial_counter = ReadHeaderCache(_bstr_t(L"Initial Counter")).uiVal;
		unsigned short extended_counter = ReadHeaderCache(_bstr_t(L"Extended Counter")).uiVal;
		time_t expiration_date = (time_t)ReadHeaderCache(_bstr_t(L"Expiration Date")).ulVal;
		time_t cur_date = time(NULL);
		
		unsigned short counter = (key_status==INITIAL_TRIAL ? initial_counter : extended_counter);
		
		if (key_type != KEYDevelopment)
		{
			// if the clock has been rolled back more than two hours behind
			unsigned int pc_hours_left = (unsigned int)((expiration_date - cur_date)/(unsigned int)(SECONDS_PER_HOUR));
			unsigned int key_hours_left = counter;
			if (pc_hours_left > key_hours_left+2) // date was set back
			{
				return true;
			}
		}
	}
	return false;
}


HRESULT ProtectionKey::DecrementTrialHours()
{
	//yyy Unfinished. Which key types modify the expiration date here?
	try
	{
		unsigned short key_type = ReadHeaderCache(L"Key Type").uiVal;
		unsigned short key_status = ReadHeaderCache(_bstr_t(L"Status")).uiVal;
		unsigned short initial_counter = ReadHeaderCache(_bstr_t(L"Initial Counter")).uiVal;
		unsigned short extended_counter = ReadHeaderCache(_bstr_t(L"Extended Counter")).uiVal;
		
		switch(key_status)
		{
		case INITIAL_TRIAL:
			////Causes key to expire immediately, not sure why in is in here.
			//if (key_type==KEYDevelopment)
			//	WriteHeader(_bstr_t(L"Expiration Date"), (unsigned int)time(NULL));
			WriteHeader(_bstr_t(L"Initial Counter"), initial_counter ? --initial_counter : 0);
			break;
		case EXTENDED_TRIAL:
		case EXTENDED_TRIAL2:
		case EXTENDED_TRIAL3:
		case EXTENDED_TRIAL4:
		case EXTENDED_TRIAL5:
			////Causes key to expire immediately, not sure why in is in here.
			//if (key_type==KEYDevelopment)
			//	WriteHeader(_bstr_t(L"Expiration Date"), (unsigned int)time(NULL));
			WriteHeader(_bstr_t(L"Extended Counter"), extended_counter ? --extended_counter : 0);
			break;
		default:
			throw _com_error(E_FAIL);
		}
	}
	catch(_com_error &e)
	{
		return e.Error();
	}
	
	return S_OK;
}

HRESULT ProtectionKey::EnterPassword(BSTR password)
{
	HRESULT hr = S_OK;
	
	try
	{
		RainbowDriver::ArgumentList password_arguments;
		hr = RainbowDriver::ParsePassword(password, password_arguments);
		if (FAILED(hr))
			return hr;
		
		DWORD user_password = password_arguments.password;	
		unsigned short key_number = ReadHeaderCache(L"Key Number").uiVal;
		unsigned short customer_number = ReadHeaderCache(L"Customer Number").uiVal;
		unsigned short product_id = ReadHeaderCache(L"Product ID").uiVal;
		unsigned short product_version = ReadHeaderCache(L"Product Version").uiVal;
		unsigned short key_type = ReadHeaderCache(L"Key Type").uiVal;
		unsigned short key_status = ReadHeaderCache(L"Status").uiVal;
		unsigned short password_number = ReadHeaderCache(L"Password Number").uiVal;
		unsigned int headersInitialized = ReadHeaderCache(L"Headers Initialized").ulVal;
				
		bool permanent_allowed_key = (key_type!=KEYDevelopment && key_type!=KEYBackup && key_type!=KEYAddon);
		bool trial_key = isOnTrial();
		bool base_key = key_status == BASE;
		
// Base passwords:			<password,16>				new: <password,16>-0-1-<customer_number,16>-<key_number,16>
// Extension passwords:		<password,16>				new: <password,16>-0-2-<customer_number,16>-<key_number,16>-<extend_days,10>-<extension_num,10>
// Version passwords:		<password,16>-<version,16>	new: <password,16>-<version,16>-3-<customer_number,16>-<key_number,16>
// Module passwords:		<password,16>-<units,10/16>	new: <password,16>-<units,10/16>-4-<customer_number,16>-<key_number,16>-<module.id,10>
// Module passwords:		<password,16>-<units,10/16>	new: <password,16>-<units,10/16>-4-<customer_number,10>-<key_number,10>-<module.id,10>-<password_number,10>
		// new style password entry (specifies password type and all the arguments to the password generate function)
		if (!password_arguments.legacy)
		{
			// the password contains the customer number and key number for which it applies, check these first
			if (password_arguments.customer!=customer_number || password_arguments.key!=key_number)
			{
				return E_INVALIDARG;
			}
			
			switch(password_arguments.type)
			{
				// base password
				case 1:
				{
					if (EnterBasePassword(user_password, trial_key, base_key, permanent_allowed_key, customer_number, key_number))
						return S_OK;
					break;
				}
				// extension password
				case 2:
				{
					if (EnterExtensionPassword(user_password, trial_key, base_key, permanent_allowed_key, customer_number, key_number, key_status, password_arguments.extend_days, password_arguments.extend_num))
						return S_OK;
					break;
				}
				// version password
				case 3:
				{
					if (EnterProductVersionPassword(user_password, trial_key, base_key, permanent_allowed_key, customer_number, key_number, password_arguments.version))
						return S_OK;
					break;
				}
				// module password
				case 4:
				{
					// SPD and iConvert legacy code
					if (product_id==0 || product_id==6 || product_id==11 || product_id==7)
					{
						// code from SpdProtectionKey::checkLicensingPasswords()
						if (EnterSPDOutputPassword(user_password, trial_key, base_key, permanent_allowed_key, customer_number, key_number, password_arguments.units_licensed))
							return S_OK;
						
						// if SPD, check for pages per minute password (no such thing on iconvert keys)
						if (product_id==0 || product_id==6 || product_id==11)
						{
							if (EnterSPDPagesPerMinutePassword(user_password, trial_key, base_key, permanent_allowed_key, customer_number, key_number, password_arguments.pages_per_minute_struct))
								return S_OK;
						}
						
						if (EnterSPDModulePassword(user_password, trial_key, base_key, permanent_allowed_key, customer_number, key_number, password_arguments.module, password_arguments.units_licensed))
							return S_OK;
					}
					// SolSearcher legacy code
					if (product_id==8)
					{
						// The solsearcher specific (legacy) password for modules {0,1,2,3,4}
						// any future modules should be handled by the default/generic case
						if (password_arguments.module <= 4 && EnterSolSearcherModulePassword(user_password, trial_key, base_key, permanent_allowed_key, customer_number, key_number, password_arguments.module, password_arguments.units_licensed))
							return S_OK;
					}
					// SolScript legacy code (based on SPD keys for older modules)
					if (product_id==9)
					{
						// for spd modules (all modules are currently legacy spd style modules)
						if (EnterSPDModulePassword(user_password, trial_key, base_key, permanent_allowed_key, customer_number, key_number, password_arguments.module, password_arguments.units_licensed))
							return S_OK;
					}
					
					//for non-legacy keys, if headersInitialized!=0xdeadbeef (HEADER_INITIALIZED), then the values in the
					//unused headers is garabage, so set all unused header to 0x00 and set headersInitialized to HEADER_INITIALIZED
					//there is a slim chance (1:0xffffffff) that the garabage value in headerInitialized could be 0xdeadbeef (HEADER_INITIALIZED)
					if(headersInitialized != HEADER_INITIALIZED)
					{
						password_number = 0;	// set to zero, because it was probably a garbage value before.
						unsigned short afpdsPS_PPM = 0;
						unsigned short xchangeIPDS_PPM = 0;
						unsigned short xchangePCL_PPM = 0;
						unsigned short xchangePS_PPM = 0;
						unsigned short xchangePSDBCS_PPM = 0;
						if (product_id==12)		// SPDE
						{
							//Keep track of:
							//	AFPDS::PS PPM
							//	XCHANGE::IPDS PPM
							//	XCHANGE::PCL PPM
							//	XCHANGE::PS PPM
							//	XCHANGE::PS(DBCS) PPM
							//Because it is stored in the cells that will be erased.
							afpdsPS_PPM = ReadModuleCache(L"AFPDS::PS PPM").uiVal;
							xchangeIPDS_PPM = ReadModuleCache(L"XCHANGE::IPDS PPM").uiVal;
							xchangePCL_PPM = ReadModuleCache(L"XCHANGE::PCL PPM").uiVal;
							xchangePS_PPM = ReadModuleCache(L"XCHANGE::PS PPM").uiVal;
							xchangePSDBCS_PPM = ReadModuleCache(L"XCHANGE::PS(DBCS) PPM").uiVal;
						}
						hr = m_driver->ClearKeyUnprogrammedIdentifier(m_keyident);
						InitializePasswordNumber();

						if (product_id==12)		// SPDE
						{
							WriteModule(L"AFPDS::PS PPM", afpdsPS_PPM);
							WriteModule(L"XCHANGE::IPDS PPM", xchangeIPDS_PPM);
							WriteModule(L"XCHANGE::PCL PPM", xchangePCL_PPM);
							WriteModule(L"XCHANGE::PS PPM", xchangePS_PPM);
							WriteModule(L"XCHANGE::PS(DBCS) PPM", xchangePSDBCS_PPM);
						}
					}

					// Generic module password code
					//when password_arguments.password_number==0 && password_number==0 then Pre-incremental Password Numbering
					if((password_arguments.password_number==0 && password_number==0) ||
						password_arguments.password_number > password_number)
					{
						if (EnterModulePassword(user_password, trial_key, base_key, permanent_allowed_key, customer_number, key_number, password_arguments.module, password_arguments.units_licensed, password_arguments.password_number))
							return S_OK;

					}
					break;
				}
				default:
				{
					break;
				}			
			}
			
			// the password didn't match (see NO_MATCH) in the original code
			return E_INVALIDARG;
		}
		// legacy password entry
		else
		{
			// The following code applies to SPD module licensing passwords
			// Passwords will be generated using this code for SPD products as well as iConvert
			// SPD (0 SPD  6 Connectivity   11 Quantum) and iConvert (7 iConvert)
			if (product_id==0 || product_id==6 || product_id==11 || product_id==7)
			{
				// code from SpdProtectionKey::checkLicensingPasswords()
				if (EnterSPDOutputPassword(user_password, trial_key, base_key, permanent_allowed_key, customer_number, key_number, password_arguments.units_licensed))
					return S_OK;
				
				if (product_id==0 || product_id==6 || product_id==11)
				{
					if (EnterSPDPagesPerMinutePassword(user_password, trial_key, base_key, permanent_allowed_key, customer_number, key_number, password_arguments.pages_per_minute_struct))
						return S_OK;
				}
				
				// check for module password (also covers 'zero module' passwords, whatever that means)
				for (KeySpec::Product::data_list_t::iterator module = m_keyspec->products[product_id].data.begin(); module != m_keyspec->products[product_id].data.end(); ++module)
				{
					if (EnterSPDModulePassword(user_password, trial_key, base_key, permanent_allowed_key, customer_number, key_number, module->id, password_arguments.units_licensed))
						return S_OK;
				}
			}
			
			// The following code applies to SolSearcher module licensing
			if (product_id==8)
			{
				// foreach solsearcher module
				for (KeySpec::Product::data_list_t::iterator module = m_keyspec->products[product_id].data.begin(); module != m_keyspec->products[product_id].data.end(); ++module)
				{
					if (EnterSolSearcherModulePassword(user_password, trial_key, base_key, permanent_allowed_key, customer_number, key_number, module->id, password_arguments.units_licensed))
						return S_OK;
				}
			}
			
			// SolScript legacy code (based on SPD keys for older modules)
			if (product_id==9)
			{
				// foreach solscript module
				for (KeySpec::Product::data_list_t::iterator module = m_keyspec->products[product_id].data.begin(); module != m_keyspec->products[product_id].data.end(); ++module)
				{
					// for spd modules (all modules are currently legacy spd style modules)
					if (EnterSPDModulePassword(user_password, trial_key, base_key, permanent_allowed_key, customer_number, key_number, module->id, password_arguments.units_licensed))
						return S_OK;
				}
			}
			
			
			// section from ProtectionKey::checkLicensingPasswords()
			// if the user password matches the generated base password
			if (EnterBasePassword(user_password, trial_key, base_key, permanent_allowed_key, customer_number, key_number))
				return S_OK;
			
			// section from ProtectionKey::checkProductVersionPassword()
			// if the user password matches the generated version password
			if (EnterProductVersionPassword(user_password, trial_key, base_key, permanent_allowed_key, customer_number, key_number, password_arguments.version))
				return S_OK;
			
			// generate passwords for extension_num in {0,4} until the user password matches one
			for (unsigned short extension_num = 0; extension_num<5; ++extension_num)
			{
				// generate passwords for extend_days in {0,MAX_EXTENSION_DAYS} until the user password matches one
				for (unsigned short extend_days = 0; extend_days <= MAX_EXTENSION_DAYS; extend_days++)
				{
					if (EnterExtensionPassword(user_password, trial_key, base_key, permanent_allowed_key, customer_number, key_number, key_status, extend_days, extension_num))
						return S_OK;
				}
			}
			
			// the password didn't match (see NO_MATCH) in the original code
			return E_INVALIDARG;
		} // 'if legacy password'
	}
	catch (_com_error &e)
	{
		hr = e.Error();
	}
	
	return hr;
}


// non product-specific base password entry
bool ProtectionKey::EnterBasePassword(DWORD user_password, bool trial_key, bool base_key, bool permanent_allowed_key, unsigned short customer_number, unsigned short key_number)
{
	HRESULT hr = S_OK;
	
	// section from ProtectionKey::checkLicensingPasswords()
	// if the user password matches the generated base password
	if (GetBasePassword(customer_number, key_number) == user_password)
	{
		if (!permanent_allowed_key)
		{
			keyserver.GenerateMessage(m_keyident, MT_WARNING, LicenseServerError::EC_KEY_WRITE_ACCESS_DENIED, time(0), MessageKeyWriteAccessDenied);
			throw _com_error(LicenseServerError::EC_KEY_WRITE_ACCESS_DENIED);
		}
		else if (!trial_key)
		{
			keyserver.GenerateMessage(m_keyident, MT_WARNING, LicenseServerError::EC_KEY_WRITE_ACCESS_DENIED, time(0), MessageKeyWriteAccessDenied);
			throw _com_error(E_FAIL);
		}
		else
		{
			try
			{
				hr = m_driver->Activate(m_keyident, m_keyspec->headers[L"Primary Descriptor"].offset >> 4);
				if (FAILED(hr)) throw _com_error(hr);
				hr = WriteStatus(BASE);
				if (FAILED(hr)) throw _com_error(hr);
			}
			catch (_com_error &e)
			{
				keyserver.GenerateMessage(m_keyident, MT_ERROR, LicenseServerError::EC_KEY_WRITE_ACCESS_DENIED, time(0), MessageKeyWriteAccessDenied);
				throw e;
			}
			
			// success
			return true;
		}
	}
	
	return false;
}

bool ProtectionKey::EnterProductVersionPassword(DWORD user_password, bool trial_key, bool base_key, bool permanent_allowed_key, unsigned short customer_number, unsigned short key_number, unsigned short product_version)
{
	// section from ProtectionKey::checkProductVersionPassword()
	// if the user password matches the generated version password
	if (GetProductVersionPassword(customer_number, key_number, product_version) == user_password)
	{
		try
		{
			WriteHeader(L"Product Version", product_version);
		}
		catch (_com_error &e)
		{
			keyserver.GenerateMessage(m_keyident, MT_ERROR, LicenseServerError::EHR_KEY_WRITE_ACCESS_DENIED, time(0), MessageKeyWriteFailure);
			throw e;
		}
		
		// success
		return true;
	}
	
	return false;
}

bool ProtectionKey::EnterExtensionPassword(DWORD user_password, bool trial_key, bool base_key, bool permanent_allowed_key, unsigned short customer_number, unsigned short key_number, unsigned short key_status, unsigned short extend_days, unsigned short extension_num)
{
	HRESULT hr = S_OK;

	// check extension passwords
	short new_extension_num;
	KeyStatus new_status;
	bool try_extension_password = true;
	switch(key_status)
	{
	case UNINITIALIZED_TRIAL:
	case INITIAL_TRIAL:
		new_extension_num = 0;
		new_status = EXTENDED_TRIAL;
		break;
	case EXTENDED_TRIAL:
		new_extension_num = 1;
		new_status = EXTENDED_TRIAL2;
		break;
	case EXTENDED_TRIAL2:
		new_extension_num = 2;
		new_status = EXTENDED_TRIAL3;
		break;
	case EXTENDED_TRIAL3:
		new_extension_num = 3;
		new_status = EXTENDED_TRIAL4;
		break;
	case EXTENDED_TRIAL4:
		new_extension_num = 4;
		new_status = EXTENDED_TRIAL5;
		break;
	case EXTENDED_TRIAL5:
	default:
		try_extension_password = false;
		break;
	}
	
	// if the extension password generated matches user password
	if (try_extension_password && new_extension_num == extension_num && GetExtensionPassword(customer_number, key_number, extend_days, extension_num) == user_password)
	{
		// Activate the algorithm, change key status to
		// new_status, and change the hour counter and
		// expiration date to reflect the new trial period
		try
		{
			hr = m_driver->Activate(m_keyident, m_keyspec->headers[L"Primary Descriptor"].offset >> 4);
			if (FAILED(hr)) throw _com_error(hr);
			hr = WriteStatus(new_status);
			if (FAILED(hr)) throw _com_error(hr);
			hr = WriteCounterDays(extend_days);
			if (FAILED(hr)) throw _com_error(hr);
			hr = WriteExpirationDays(extend_days);
			if (FAILED(hr)) throw _com_error(hr);
		}
		catch (_com_error &e)
		{
			keyserver.GenerateMessage(m_keyident, MT_ERROR, LicenseServerError::EHR_KEY_WRITE_ACCESS_DENIED, time(0), MessageKeyWriteFailure);
			throw e;
		}
		
		// success
		return true;
	}
	
	return false;
}

bool ProtectionKey::EnterModulePassword(DWORD user_password, bool trial_key, bool base_key, bool permanent_allowed_key, unsigned short customer_number, unsigned short key_number, unsigned int module_id, unsigned int units_licensed, unsigned int password_number)
{
	HRESULT hr = S_OK;

	// Generic module password code
	// if trial key
	if (GetModulePassword(customer_number, key_number, module_id, units_licensed, password_number) == user_password)
	{
		if (trial_key && permanent_allowed_key)
		{
			// the password is for a trial key -> activate the algorithm,
			// initialize licensing, and set status to BASE
			try
			{
				hr = m_driver->Activate(m_keyident, m_keyspec->headers[L"Primary Descriptor"].offset >> 4);
				if (FAILED(hr)) throw _com_error(hr);
				hr = WriteStatus(BASE);
				if (FAILED(hr)) throw _com_error(hr);
			}
			catch (_com_error &e)
			{
				keyserver.GenerateMessage(m_keyident, MT_ERROR, LicenseServerError::EHR_KEY_WRITE_ACCESS_DENIED, time(0), MessageKeyWriteFailure);
				throw e;
			}
		}
		
		if (trial_key || base_key)
		{
			try
			{
				WriteLicense(module_id, units_licensed);

				//if password_number > 0 then write it into the PASSWORD_NUMBER cell.
				if(password_number > 0)
					WriteHeader(L"Password Number", password_number);
			}
			catch (_com_error &e)
			{
				keyserver.GenerateMessage(m_keyident, MT_ERROR, LicenseServerError::EHR_KEY_WRITE_ACCESS_DENIED, time(0), MessageKeyWriteFailure);
				throw e;
			}
			
			// success
			return true;
		}
	}
	
	return false;
}

// SPD/iConvert specific	
bool ProtectionKey::EnterSPDModulePassword(DWORD user_password, bool trial_key, bool base_key, bool permanent_allowed_key, unsigned short customer_number, unsigned short key_number, unsigned int module_id, unsigned int units_licensed)
{
	HRESULT hr = S_OK;
	
	KeySpec::Module &module(m_keyspec->products[ReadHeaderCache(_bstr_t(L"Product ID")).ulVal][module_id]);
	
	// If units_licensed_decimal is set, that means the <password>-<units licensed>
	// syntax was used to enter the password. Since getModulePassword()
	// expects the current units licensed, not the desired units licensed,
	// we subtract 1 from the units_licensed_decimal value. If units_
	// licensed_decimal is not set, we simply pass the current units
	// licensed for the module.
	unsigned int mod_units_licensed = (units_licensed==0 ? 0 : units_licensed-1);	
	if (GetSPDModulePassword(customer_number, key_number, module_id, mod_units_licensed) == user_password)
	{
		if (trial_key)
		{
			// the password is for a trial key -> activate the algorithm,
			// initialize licensing, and set status to BASE
			try
			{
				hr = m_driver->Activate(m_keyident, m_keyspec->headers[L"Primary Descriptor"].offset >> 4);
				if (FAILED(hr)) throw _com_error(hr);
				hr = WriteStatus(BASE);
				if (FAILED(hr)) throw _com_error(hr);
			}
			catch (_com_error &e)
			{
				keyserver.GenerateMessage(m_keyident, MT_ERROR, LicenseServerError::EHR_KEY_WRITE_ACCESS_DENIED, time(0), MessageKeyWriteFailure);
				throw e;
			}
		}
		
		if (trial_key || base_key)
		{
			try {WriteLicense(module_id, units_licensed);}
			catch(_com_error &e) {throw e;}
			
			// success
			return true;
		}
	}
	
	return false;
}

bool ProtectionKey::EnterSPDOutputPassword(DWORD user_password, bool trial_key, bool base_key, bool permanent_allowed_key, unsigned short customer_number, unsigned short key_number, unsigned short output_units)
{
	HRESULT hr = S_OK;

	// check for output password
	if (GetSPDOutputPassword(customer_number, key_number, output_units) == user_password)
	{
		if (trial_key && permanent_allowed_key)
		{
			// the password is for a trial key -> activate the algorithm,
			// initialize licensing, and set status to BASE
			try
			{
				hr = m_driver->Activate(m_keyident, m_keyspec->headers[L"Primary Descriptor"].offset >> 4);
				if (FAILED(hr)) throw _com_error(hr);
				hr = WriteStatus(BASE);
				if (FAILED(hr)) throw _com_error(hr);
			}
			catch (_com_error &e)
			{
				keyserver.GenerateMessage(m_keyident, MT_ERROR, LicenseServerError::EHR_KEY_WRITE_ACCESS_DENIED, time(0), MessageKeyWriteFailure);
				throw e;
			}
		}
		if (trial_key || base_key)
		{
			try {WriteLicense(L"Output Pool", output_units);}
			catch(_com_error &e) {throw e;}
			
			// success
			return true;
		}
	}

	return false;
}

bool ProtectionKey::EnterSPDPagesPerMinutePassword(DWORD user_password, bool trial_key, bool base_key, bool permanent_allowed_key, unsigned short customer_number, unsigned short key_number, unsigned int pages_per_minute_struct)
{
	HRESULT hr = S_OK;

	// check for the pages per minute password
	if (trial_key || base_key)
	{
		unsigned short ppm_extensions = ReadModuleCache(_bstr_t(L"Pages Per Minute Extensions")).bVal;
		unsigned short ppm_module_id = (pages_per_minute_struct & 0xFF000) >> 12;
		unsigned short ppm_pages = (pages_per_minute_struct & 0x00FFF);
		
		// the module id in the struct is the non-ppm module that the ppm password is associated with
		// translate this to the new ppm module that it maps to. For a ppm password for "XCHANGE::PS" 
		// will map to the "XCHANGE::PS PPM" module
		/*
				<Module id="2"	offset="0x088" bits="4" name="XCHANGE::IPDS"/>
				<Module id="35"	offset="0x10c" bits="4" name="XCHANGE::PCL"/>
				<Module id="39"	offset="0x11c" bits="4" name="XCHANGE::PS"/>
				<Module id="44"	offset="0x130" bits="4" name="XCHANGE::PS (DBCS)"/>
				<Module id="47"	offset="0x13c" bits="4" name="AFPDS::PS"/>

				// previously (no passwords were released using these, deprecated)
				<Module id="56" offset="0x160" bits="8" name="XCHANGE::IPDS PPM" licenseRead="(value * 25)" licenseWrite="(value / 25)"/>
				<Module id="58" offset="0x168" bits="8" name="XCHANGE::PCL PPM" licenseRead="(value * 25)" licenseWrite="(value / 25)"/>
				<Module id="60" offset="0x170" bits="8" name="XCHANGE::PS PPM" licenseRead="(value * 25)" licenseWrite="(value / 25)"/>
				<Module id="64" offset="0x180" bits="8" name="XCHANGE::PS (DBCS) PPM" licenseRead="(value * 25)" licenseWrite="(value / 25)"/>
				<Module id="66" offset="0x188" bits="8" name="AFPDS::PS PPM" licenseRead="(value * 25)" licenseWrite="(value / 25)"/>
				
				// now, these should be the PPM modules
				<Module id="68" offset="0x190" bits="8" name="XCHANGE::IPDS PPM" licenseRead="(value * 2)" licenseWrite="((value / 2) + (value % 2))" unlimited="500"/>
				<Module id="70" offset="0x198" bits="8" name="XCHANGE::PCL PPM" licenseRead="(value * 2)" licenseWrite="((value / 2) + (value % 2))" unlimited="500"/>
				<Module id="160" offset="0x300" bits="8" name="XCHANGE::PS PPM" licenseRead="(value * 2)" licenseWrite="((value / 2) + (value % 2))" unlimited="500"/>
				<Module id="64" offset="0x180" bits="8" name="XCHANGE::PS (DBCS) PPM" licenseRead="(value * 2)" licenseWrite="((value / 2) + (value % 2))" unlimited="500"/>
				<Module id="66" offset="0x188" bits="8" name="AFPDS::PS PPM" licenseRead="(value * 2)" licenseWrite="((value / 2) + (value % 2))" unlimited="500"/>
		*/
		switch(ppm_module_id)
		{
		case 2:
			ppm_module_id = 68;		break;
		case 35:
			ppm_module_id = 70;		break;
		case 39:
			ppm_module_id = 160;		break;
		case 44:
			ppm_module_id = 64;		break;
		case 47:
			ppm_module_id = 66;		break;
		default:
			return false;
		}

		ppm_extensions++;
		
		if (GetSPDPagesPerMinutePassword(customer_number, key_number, ppm_extensions) == user_password)
		{
			// Write the pages per minute license
			WriteLicense(ppm_module_id, ppm_pages);
			
			// Write the pages per minute extension count license
			KeySpec::Module &extensions_module(m_keyspec->products[ReadHeaderCache(L"Product ID").uiVal][L"Pages Per Minute Extensions"]);
			WriteBitsPhysical(extensions_module.offset, extensions_module.bits, _variant_t((long)ppm_extensions));
			
			// success 
			return true;
		}
	}
	
	return false;
}

// SolSearcher specific
bool ProtectionKey::EnterSolSearcherModulePassword(DWORD user_password, bool trial_key, bool base_key, bool permanent_allowed_key, unsigned short customer_number, unsigned short key_number, unsigned int module_id, unsigned int units_licensed)
{
	HRESULT hr = S_OK;

	if (trial_key || base_key)
	{
		// if the module license password matches what the user provided
		if (GetSolsearcherModulePassword(customer_number, key_number, module_id, units_licensed)==user_password)
		{
			// If the key is on a trial period, activate the algorithm, initialize
			// licensing, and set status to BASE.
			try
			{
				if (trial_key && permanent_allowed_key)
				{
					hr = m_driver->Activate(m_keyident, m_keyspec->headers[L"Primary Descriptor"].offset >> 4);
					if (FAILED(hr)) throw _com_error(hr);
					hr = WriteStatus(BASE);
					if (FAILED(hr)) throw _com_error(hr);
				}
			
				WriteLicense(module_id, units_licensed);
			}
			catch(_com_error &e)
			{
				keyserver.GenerateMessage(m_keyident, MT_ERROR, LicenseServerError::EHR_KEY_WRITE_ACCESS_DENIED, time(0), MessageKeyWriteFailure);
				throw e;
			}
			
			// success
			return true;
		}
	}
	
	return false;
}



// Password specifications:
// Base passwords:			<password,16>				new: <password,16>-0-1-<customer_number,10>-<key_number,10>
// Extension passwords:		<password,16>				new: <password,16>-0-2-<customer_number,10>-<key_number,10>-<extend_days,10>-<extension_num,10>
// Version passwords:		<password,16>-<version,16>	new: <password,16>-<version,16>-3-<customer_number,10>-<key_number,10>
// Module passwords:		<password,16>-<units,10>	new: <password,16>-<units,10>-4-<customer_number,10>-<key_number,10>-<module.id,10>

HRESULT ProtectionKey::GenerateBasePassword(long customer_number, long key_number, BSTR *password)
{
	DWORD password_hash;
	wchar_t password_string[128];
	
	password_string[0]=0;
	
	password_hash = GetBasePassword((unsigned short)customer_number, (unsigned short)key_number);
	swprintf(password_string, L"%x-0-1-%x-%x", password_hash, customer_number, key_number);
	
	*password = SysAllocString(password_string);
	
	return S_OK;
}

HRESULT ProtectionKey::GenerateVersionPassword(long customer_number, long key_number, long ver_major, long ver_minor, BSTR *password)
{
	DWORD password_hash;
	wchar_t password_string[128];
	
	unsigned short version = Version::TinyVersion(Version::ModuleVersion(ver_major, ver_minor, 0, 0)).program;
	
	password_string[0]=0;
	
	password_hash = GetProductVersionPassword((unsigned short)customer_number, (unsigned short)key_number, (unsigned short)version);
	swprintf(password_string, L"%x-%04x-3-%x-%x", password_hash, version, customer_number, key_number);
	
	*password = SysAllocString(password_string);
	
	return S_OK;
}

HRESULT ProtectionKey::GenerateExtensionPassword(long customer_number, long key_number, long extend_days, long extension_num, BSTR *password)
{
	DWORD password_hash;
	wchar_t password_string[128];
	
	password_string[0]=0;
	
	password_hash = GetExtensionPassword((unsigned short)customer_number, (unsigned short)key_number, (unsigned short)extend_days, (unsigned short)extension_num);
	swprintf(password_string, L"%x-0-2-%x-%x-%d-%d", password_hash, customer_number, key_number, extend_days, extension_num);
	
	*password = SysAllocString(password_string);
	
	return S_OK;
}

HRESULT ProtectionKey::GenerateModulePassword(long customer_number, long key_number, long product_ident, long module_ident, long license_count, BSTR *password)
{
	bool password_generated = false;
	DWORD password_hash;
	wchar_t password_string[128];
	KeySpec::Product &product(m_keyspec->products[product_ident]);
	KeySpec::Module &module(product[module_ident]);
	
	password_string[0]=0;
	
	// different products have different module password functions for reasons of legacy code compatiblity
	switch(product.id)
	{
		// SPD and iConvert
		case 0:		// SPD
		case 6:		// Connectivity
		case 11:	// Quantum
		case 7:		// iConvert
		{
			// we need differentiation here between pages per minute and output passwords (pooled password issue)
			//	_variant_t GetSPDOutputPassword(unsigned short customer_number, unsigned short key_number, unsigned short output_units);
			//	_variant_t GetSPDModulePassword(unsigned short customer_number, unsigned short key_number, unsigned int module_id, unsigned int units_licensed);
			//	_variant_t GetSPDPagesPerMinutePassword(unsigned short customer_number, unsigned short key_number, unsigned short ext);
			
			// if the module is the 'max lu/output pool' module
			if (module.id==128)
			{
				password_hash = GetSPDOutputPassword((unsigned short)customer_number, (unsigned short)key_number, (unsigned int)license_count);
				swprintf(password_string, L"%x-%d-4-%x-%x-%d", password_hash, license_count, customer_number, key_number, module.id);
				password_generated = true;
			}
			// if the module is a pages per minute module (andt he product is an spd or spd derivative)
			else if
			(
				/* Spd Product */
				(product.id==0 || product.id==6 || product.id==11)
				&&
				/* Pages Per Minute Module */
				(module.id==68 || module.id==70 || module.id==160 || module.id==64 || module.id==66)
			)
			{
				// The license_count parameter for pages per minute must consist of the following:
				// bottom 4 bits: ppm extension
				// top 4 bits: pages per minute
				unsigned short ppm_pages = (unsigned short)((license_count & 0xFFFF0000) >> 16);
				unsigned short ppm_extension = (unsigned short)(license_count & 0x0000FFFF);
				
				// translate the pages per minute module id to the underlying module id (eg XCHANGE::PS PPM module id ->  XCHANGE::PS module id)
				unsigned int ppm_underlying_module;
				switch(module.id)
				{
				case 68:
					ppm_underlying_module = 2;		break;
				case 70:
					ppm_underlying_module = 35;		break;
				case 160:
					ppm_underlying_module = 39;		break;
				case 64:
					ppm_underlying_module = 44;		break;
				case 66:
					ppm_underlying_module = 47;		break;
				default:
					return E_INVALIDARG;
				}
				
				password_hash = GetSPDPagesPerMinutePassword((unsigned short)customer_number, (unsigned short)key_number, (unsigned int)ppm_extension);
				
				unsigned int pages_per_minute_struct = ((ppm_underlying_module & 0xFF) << 12) | (ppm_pages & 0x00FFF);
				
				swprintf(password_string, L"%x-%x-4-%x-%x-%d", password_hash, pages_per_minute_struct, customer_number, key_number, module.id);
				
				password_generated = true;
			}
			// otherwise, regular SPD module password
			else
			{
				password_hash = GetSPDModulePassword((unsigned short)customer_number, (unsigned short)key_number, (unsigned int)module.id, (unsigned int)license_count);
				swprintf(password_string, L"%x-%d-4-%x-%x-%d", password_hash, license_count+1, customer_number, key_number, module.id);
				password_generated = true;
			}
			break;
		}
		// SolSearcher
		case 8:
		{
			// The solsearcher specific (legacy) password generation function 
			// only generates passwords for modules {0,1,2,3,4}
			// any future modules should be handled by the default/generic case
			if (module.id<=4)
			{
				password_hash = GetSolsearcherModulePassword((unsigned short)customer_number, (unsigned short)key_number, (unsigned int)module.id, (unsigned int)license_count);
				swprintf(password_string, L"%x-%d-4-%x-%x-%d", password_hash, license_count, customer_number, key_number, module.id);
				password_generated = true;
			}
			break;
		}
		// SolScript
		case 9:	
		{
			// for spd modules (all modules are currently legacy spd style modules)
			password_hash = GetSPDModulePassword((unsigned short)customer_number, (unsigned short)key_number, (unsigned int)module.id, (unsigned int)license_count);
			swprintf(password_string, L"%x-%d-4-%x-%x-%d", password_hash, license_count+1, customer_number, key_number, module.id);
			password_generated = true;
			break;
		}
	}
		
	// all other products and modules
	if (!password_generated)
	{
		password_hash = GetModulePassword((unsigned short)customer_number, (unsigned short)key_number, (unsigned int)module.id, (unsigned int)license_count);
		swprintf(password_string, L"%x-%d-4-%x-%x-%d", password_hash, license_count, customer_number, key_number, module.id);
	}
	
	*password = SysAllocString(password_string);
	
	return S_OK;
}
HRESULT ProtectionKey::GenerateModulePassword(long customer_number, long key_number, long product_ident, long module_ident, long license_count, long password_number, BSTR *password)
{
	bool password_generated = false;
	DWORD password_hash;
	wchar_t password_string[128];
	KeySpec::Product &product(m_keyspec->products[product_ident]);
	KeySpec::Module &module(product[module_ident]);
	
	password_string[0]=0;
	
	//Generate old style passwords if password number is 0, else force the 
	//use of new style passwords
	if(password_number == 0)
	{
		// different products have different module password functions for reasons of legacy code compatiblity
		switch(product.id)
		{
			// SPD and iConvert
			case 0:		// SPD
			case 6:		// Connectivity
			case 11:	// Quantum
			case 7:		// iConvert
			{
				// we need differentiation here between pages per minute and output passwords (pooled password issue)
				//	_variant_t GetSPDOutputPassword(unsigned short customer_number, unsigned short key_number, unsigned short output_units);
				//	_variant_t GetSPDModulePassword(unsigned short customer_number, unsigned short key_number, unsigned int module_id, unsigned int units_licensed);
				//	_variant_t GetSPDPagesPerMinutePassword(unsigned short customer_number, unsigned short key_number, unsigned short ext);
				
				// if the module is the 'max lu/output pool' module
				if (module.id==128)
				{
					password_hash = GetSPDOutputPassword((unsigned short)customer_number, (unsigned short)key_number, (unsigned int)license_count);
					swprintf(password_string, L"%x-%d-4-%x-%x-%d", password_hash, license_count, customer_number, key_number, module.id);
					password_generated = true;
				}
				// if the module is a pages per minute module (andt he product is an spd or spd derivative)
				else if
				(
					/* Spd Product */
					(product.id==0 || product.id==6 || product.id==11)
					&&
					/* Pages Per Minute Module */
					(module.id==68 || module.id==70 || module.id==160 || module.id==64 || module.id==66)
				)
				{
					// The license_count parameter for pages per minute must consist of the following:
					// bottom 4 bits: ppm extension
					// top 4 bits: pages per minute
					unsigned short ppm_pages = (unsigned short)((license_count & 0xFFFF0000) >> 16);
					unsigned short ppm_extension = (unsigned short)(license_count & 0x0000FFFF);
					
					// translate the pages per minute module id to the underlying module id (eg XCHANGE::PS PPM module id ->  XCHANGE::PS module id)
					unsigned int ppm_underlying_module;
					switch(module.id)
					{
					case 68:
						ppm_underlying_module = 2;		break;
					case 70:
						ppm_underlying_module = 35;		break;
					case 160:
						ppm_underlying_module = 39;		break;
					case 64:
						ppm_underlying_module = 44;		break;
					case 66:
						ppm_underlying_module = 47;		break;
					default:
						return E_INVALIDARG;
					}
					
					password_hash = GetSPDPagesPerMinutePassword((unsigned short)customer_number, (unsigned short)key_number, (unsigned int)ppm_extension);
					
					unsigned int pages_per_minute_struct = ((ppm_underlying_module & 0xFF) << 12) | (ppm_pages & 0x00FFF);
					
					swprintf(password_string, L"%x-%x-4-%x-%x-%d", password_hash, pages_per_minute_struct, customer_number, key_number, module.id);
					
					password_generated = true;
				}
				// otherwise, regular SPD module password
				else
				{
					password_hash = GetSPDModulePassword((unsigned short)customer_number, (unsigned short)key_number, (unsigned int)module.id, (unsigned int)license_count);
					swprintf(password_string, L"%x-%d-4-%x-%x-%d", password_hash, license_count+1, customer_number, key_number, module.id);
					password_generated = true;
				}
				break;
			}
			// SolSearcher
			case 8:
			{
				// The solsearcher specific (legacy) password generation function 
				// only generates passwords for modules {0,1,2,3,4}
				// any future modules should be handled by the default/generic case
				if (module.id<=4)
				{
					password_hash = GetSolsearcherModulePassword((unsigned short)customer_number, (unsigned short)key_number, (unsigned int)module.id, (unsigned int)license_count);
					swprintf(password_string, L"%x-%d-4-%x-%x-%d", password_hash, license_count, customer_number, key_number, module.id);
					password_generated = true;
				}
				break;
			}
			// SolScript
			case 9:	
			{
				// for spd modules (all modules are currently legacy spd style modules)
				password_hash = GetSPDModulePassword((unsigned short)customer_number, (unsigned short)key_number, (unsigned int)module.id, (unsigned int)license_count);
				swprintf(password_string, L"%x-%d-4-%x-%x-%d", password_hash, license_count+1, customer_number, key_number, module.id);
				password_generated = true;
				break;
			}
		}
	}
		
	// all other products and modules
	if (!password_generated)
	{
		if(password_number == 0)
		{
			password_hash = GetModulePassword((unsigned short)customer_number, (unsigned short)key_number, (unsigned int)module.id, (unsigned int)license_count);
			swprintf(password_string, L"%x-%d-4-%x-%x-%d", password_hash, license_count, customer_number, key_number, module.id);
		}
		else
		{
			password_hash = GetModulePassword((unsigned short)customer_number, (unsigned short)key_number, (unsigned int)module.id, (unsigned int)license_count, (unsigned int)password_number);
			swprintf(password_string, L"%x-%d-4-%x-%x-%d-%d", password_hash, license_count, customer_number, key_number, module.id, password_number);
		}
	}
	
	*password = SysAllocString(password_string);
	
	return S_OK;
}


unsigned short ProtectionKey::ReadCellPhysical(unsigned short cell)
{
	HRESULT hr = S_OK;
	unsigned short value;
	hr = m_driver->ReadCell(m_keyident, cell, &value);
	if (FAILED(hr))
		throw _com_error(hr);
	return value;
}

void ProtectionKey::WriteCellPhysical(unsigned short cell, unsigned short value)
{
	HRESULT hr = S_OK;
	hr = m_driver->WriteCell(m_keyident, cell, value);
	if (FAILED(hr))
		throw _com_error(hr);
	
	UpdateCellCache(cell);
}

void ProtectionKey::FormatCellPhysical(unsigned short cell)
{
	HRESULT hr = S_OK;
	hr = m_driver->FormatCell(m_keyident, cell);
	if (FAILED(hr))
		throw _com_error(hr);
	
	UpdateCellCache(cell);
}

_variant_t ProtectionKey::ReadBits(unsigned short offset, unsigned short bits, bool physical)
{
	HRESULT hr = S_OK;
	unsigned int value=0;
	
	// check bits for validity
	if (bits!=1 && bits!=2 && bits!=4 && bits!=8 && bits!=16 && bits!=32)
		throw _com_error(E_INVALIDARG);
	// check that the offset is properly aligned given the number of bits specified
	if (offset % bits != 0)
		throw _com_error(E_INVALIDARG);
	
	// calculate the addresses on the key to read from
	unsigned short cell_first = offset/16;						// first cell number
	unsigned short cell_count = bits/16 + (bits % 16 ? 1 : 0);	// number of cells to read
	unsigned short shift = offset%16;							// number of bits to shift the value read (for bits<16)
	
	// read from the key
	for (unsigned short i=0; i<cell_count && SUCCEEDED(hr); ++i)
	{
		if (physical)
			*((unsigned short*)(&value)+i) = ReadCellPhysical(cell_first+i);
		else
			*((unsigned short*)(&value)+i) = ReadCellCache(cell_first+i);
	}
	// shift and mask bits as necessary
	value = value >> shift;
	value &= (unsigned int)0xFFFFFFFF >> (32-bits);
	
	if (FAILED(hr))
		throw _com_error(hr);
	
	return BitsToVariant(value,bits);
}

_variant_t ProtectionKey::ReadBitsPhysical(unsigned short offset, unsigned short bits)
{
	return ReadBits(offset, bits, true);
}

void ProtectionKey::WriteBitsPhysical(unsigned short offset, unsigned short bits, _variant_t vtValue)
{
	HRESULT hr = S_OK;
	
	// check bits for validity
	if (bits!=1 && bits!=2 && bits!=4 && bits!=8 && bits!=16 && bits!=32)
		throw _com_error(E_INVALIDARG);
	// check that the offset is properly aligned given the number of bits specified
	if (offset % bits != 0)
		throw _com_error(E_INVALIDARG);
	
	// calculate the addresses on the key to read from
	unsigned short cell_first = offset/16;						// first cell number
	unsigned short cell_count = bits/16 + (bits % 16 ? 1 : 0);	// number of cells to read
	unsigned short shift = offset%16;							// number of bits to shift the value written (for bits<16)
	
	// if writing to only a part of a cell (bits<16)
	if (bits<16)
	{
		// read the cell that is being written to
		unsigned short value = ReadCellPhysical(cell_first);
		
		// mask out (zero) the bits in that cell that are being written over
		value &= ~(((unsigned short)(0xFFFF) >> (16 - bits)) << shift);
		
		// write in the bits at the specified location
		value |= vtValue.uiVal << shift;
		
		// send the modified cell back to the key
		WriteCellPhysical(cell_first, value);
	}
	// otherwise, just write whole cells
	else
	{
		for (unsigned short i=0; i<cell_count && SUCCEEDED(hr); ++i)
		{
			WriteCellPhysical(cell_first+i,*((unsigned short*)(&vtValue.ulVal)+i));
		}
	}
	
	if (FAILED(hr))
		throw _com_error(hr);
}

_variant_t ProtectionKey::ReadHeaderCache(wchar_t* id)
{
	KeySpec::Header &header(m_keyspec->headers[id]);
	return ReadBitsCache(header.offset, header.bits);
}

_variant_t ProtectionKey::ReadModuleCache(wchar_t* id)
{
	KeySpec::Module &module(m_keyspec->products[ReadHeaderCache(L"Product ID").uiVal][id]);
	return ReadBitsCache(module.offset, module.bits);
}

_variant_t ProtectionKey::ReadHeaderCache(unsigned int id)
{
	KeySpec::Header &header(m_keyspec->headers[id]);
	return ReadBitsCache(header.offset, header.bits);
}

_variant_t ProtectionKey::ReadModuleCache(unsigned int id)
{
	KeySpec::Module &module(m_keyspec->products[ReadHeaderCache(L"Product ID").uiVal][id]);
	return ReadBitsCache(module.offset, module.bits);
}

void ProtectionKey::WriteHeader(wchar_t* id, unsigned int value)
{
	KeySpec::Header &header(m_keyspec->headers[id]);
	WriteBitsPhysical(header.offset, header.bits, _variant_t(value));
}

void ProtectionKey::WriteModule(wchar_t* id, unsigned int value)
{
	KeySpec::Module &module(m_keyspec->products[ReadHeaderCache(L"Product ID").uiVal][id]);
	WriteBitsPhysical(module.offset, module.bits, _variant_t(value));
}

void ProtectionKey::WriteHeader(unsigned int id, unsigned int value)
{
	KeySpec::Header &header(m_keyspec->headers[id]);
	WriteBitsPhysical(header.offset, header.bits, _variant_t(value));
}

void ProtectionKey::WriteModule(unsigned int id, unsigned int value)
{
	KeySpec::Module &module(m_keyspec->products[ReadHeaderCache(L"Product ID").uiVal][id]);
	WriteBitsPhysical(module.offset, module.bits, _variant_t(value));
}

void ProtectionKey::WriteLicense(wchar_t* id, unsigned int value)
{
	KeySpec::Module &module(m_keyspec->products[ReadHeaderCache(L"Product ID").uiVal][id]);
	if(module.isCounter)
	{
		long writeValue = module.fn_read(ReadModuleCache(id).ulVal) + value;
		WriteBitsPhysical(module.offset, module.bits, _variant_t(module.fn_write(writeValue)));
	}
	else
		WriteBitsPhysical(module.offset, module.bits, _variant_t(module.fn_write(value)));
}


void ProtectionKey::WriteLicense(unsigned int id, unsigned int value)
{
	KeySpec::Module &module(m_keyspec->products[ReadHeaderCache(L"Product ID").uiVal][id]);
	if(module.isCounter)
	{
		long writeValue = module.fn_read(ReadModuleCache(id).ulVal) + value;
		WriteBitsPhysical(module.offset, module.bits, _variant_t(module.fn_write(writeValue)));
	}
	else
		WriteBitsPhysical(module.offset, module.bits, _variant_t(module.fn_write(value)));
}

HRESULT ProtectionKey::WriteLicenseDecrementCounter(wchar_t* id, unsigned int value)
{
	HRESULT hr(S_OK);
	KeySpec::Module &module(m_keyspec->products[ReadHeaderCache(L"Product ID").uiVal][id]);
	if(module.isCounter && !LicenseIsUnlimited(id))
	{
		long writeValue = module.fn_write(module.fn_read(ReadModuleCache(id).ulVal) - value) + ((value % module.fn_read(1)) > 0 ? 1 : 0);
		if(module.fn_read(ReadModuleCache(id).ulVal) >= value)
			WriteBitsPhysical(module.offset, module.bits, _variant_t(writeValue));
		else	//Case where they try to decrement more licenses than are on the key
			WriteBitsPhysical(module.offset, module.bits, _variant_t(module.fn_write(0)));
	}
	else
		hr = E_INVALIDARG;
	return hr;
}
HRESULT ProtectionKey::WriteLicenseDecrementCounter(unsigned int id, unsigned int value)
{
	HRESULT hr(S_OK);
	KeySpec::Module &module(m_keyspec->products[ReadHeaderCache(L"Product ID").uiVal][id]);
	if(module.isCounter && !LicenseIsUnlimited(id))
	{
		long writeValue = module.fn_write(module.fn_read(ReadModuleCache(id).ulVal) - value) + ((value % module.fn_read(1)) > 0 ? 1 : 0);
		if(module.fn_read(ReadModuleCache(id).ulVal) >= value)
			WriteBitsPhysical(module.offset, module.bits, _variant_t(writeValue));
		else	//Case where they try to decrement more licenses than are on the key
			WriteBitsPhysical(module.offset, module.bits, _variant_t(module.fn_write(0)));
	}
	else
		hr = E_INVALIDARG;
	return hr;
}


// License information helpers
bool ProtectionKey::LicenseIsUnlimited(wchar_t* id)
{
	KeySpec::Module &module(m_keyspec->products[ReadHeaderCache(L"Product ID").uiVal][id]);
	
	// not a pooled license
	if (!LicenseIsPooled(id))
	{
		return (module.isUnlimited(module.fn_read(ReadModuleCache(id).ulVal)));
	}
	else
	{
		KeySpec::Module &pool(m_keyspec->products[ReadHeaderCache(L"Product ID").uiVal][module.pool]);
		return (module.isUnlimited(module.fn_read(ReadModuleCache(id).ulVal)) && pool.isUnlimited(module.fn_read(ReadModuleCache(pool.id).ulVal)));
	}
}

bool ProtectionKey::LicenseIsUnlimited(unsigned int id)
{
	KeySpec::Module &module(m_keyspec->products[ReadHeaderCache(L"Product ID").uiVal][id]);
	
	// not a pooled license
	if (!LicenseIsPooled(id))
	{
		return (module.isUnlimited(module.fn_read(ReadModuleCache(id).ulVal)));
	}
	else
	{
		KeySpec::Module &pool(m_keyspec->products[ReadHeaderCache(L"Product ID").uiVal][module.pool]);
		return (module.isUnlimited(module.fn_read(ReadModuleCache(id).ulVal)) && pool.isUnlimited(module.fn_read(ReadModuleCache(pool.id).ulVal)));
	}
}

bool ProtectionKey::LicenseIsPooled(wchar_t* id)
{
	KeySpec::Module &module(m_keyspec->products[ReadHeaderCache(L"Product ID").uiVal][id]);
	return (module.pool!=0);
}

bool ProtectionKey::LicenseIsPooled(unsigned int id)
{
	KeySpec::Module &module(m_keyspec->products[ReadHeaderCache(L"Product ID").uiVal][id]);
	return (module.pool!=0);
}

unsigned int ProtectionKey::LicenseEffectiveValue(wchar_t* id)
{
	KeySpec::Module &module(m_keyspec->products[ReadHeaderCache(L"Product ID").uiVal][id]);
	
	// assumes that 2 billion is much greater than valid license values
	if (LicenseIsUnlimited(id))
	{
		return 0x7FFFFFFF;
	}
	else
	{
		if (LicenseIsPooled(id))
		{
			KeySpec::Module &pool(m_keyspec->products[ReadHeaderCache(L"Product ID").uiVal][module.pool]);
			
			// pooled licenses need to have their own value set 
			// at unlimited in order to participate in the pool
			if (module.isUnlimited(module.fn_read(ReadModuleCache(id).ulVal)))
			{
				return LicenseEffectiveValue(pool.id);
			}
			else
			{
				return 0;
			}
		}
		else if(module.isCounter)
		{
			//if(module.id == 10)
			//	return module.fn_read(65536) - module.counter;
			//else

			// module.counter is not tied to a key on purpose, the only side effect
			// is that if you have multiple keys attached to a system, then -module.counter
			// is applied to all keys.
			return module.fn_read(ReadModuleCache(id).ulVal) - module.counter;
		}
		else
		{
			return module.fn_read(ReadModuleCache(id).ulVal);
		}
	}
}

unsigned int ProtectionKey::LicenseEffectiveValue(unsigned int id)
{
	KeySpec::Module &module(m_keyspec->products[ReadHeaderCache(L"Product ID").uiVal][id]);
	
	// assumes that 2 billion is much greater than valid license values
	if (LicenseIsUnlimited(id))
	{
		return 0x7FFFFFFF;
	}
	else
	{
		if (LicenseIsPooled(id))
		{
			KeySpec::Module &pool(m_keyspec->products[ReadHeaderCache(L"Product ID").uiVal][module.pool]);
			
			// pooled licenses need to have their own value set 
			// at unlimited in order to participate in the pool
			if (module.isUnlimited(module.fn_read(ReadModuleCache(id).ulVal)))
			{
				return LicenseEffectiveValue(pool.id);
			}
			else
			{
				return 0;
			}
		}
		else if(module.isCounter)
		{
			//if(module.id == 10)
			//	return module.fn_read(65536) - module.counter;
			//else
			return module.fn_read(ReadModuleCache(id).ulVal) - module.counter;
		}
		else
		{
			return module.fn_read(ReadModuleCache(id).ulVal);
		}
	}
}



// Given a set of bits (stored in an unsigned int), package it up in to a VARIANT that 
// takes the number of bits (type) in to account
_variant_t ProtectionKey::BitsToVariant(unsigned int value, unsigned int bits)
{
	VARIANT vtValue;
	VariantInit(&vtValue);
	
	switch(bits)
	{
		case 1: case 2: case 4: case 8:
		{
			vtValue.vt = VT_UI1;
			vtValue.ulVal = (BYTE)value;
			break;
		}
		case 16:
		{
			vtValue.vt = VT_UI2;
			vtValue.ulVal = (unsigned short)value;
			break;
		}
		case 32:
		{
			vtValue.vt = VT_UI4;
			vtValue.ulVal = (unsigned int)value;
			break;
		}
	}
	
	return vtValue;
}

bool ProtectionKey::isOnTrial()
{
	unsigned short key_status = ReadHeaderCache(_bstr_t(L"Status")).uiVal;
	
	bool ontrial = (
		key_status==INITIAL_TRIAL ||
		key_status==EXTENDED_TRIAL ||
		key_status==EXTENDED_TRIAL2 ||
		key_status==EXTENDED_TRIAL3 ||
		key_status==EXTENDED_TRIAL4 ||
		key_status==EXTENDED_TRIAL5 ||
		key_status==UNINITIALIZED_TRIAL
		);
	
	return ontrial;
}

bool ProtectionKey::KeyInUse()
{
	SafeMutex mutex(license_use_lock);
	
	for (KeyUseList::iterator user = key_use.begin(); user != key_use.end(); ++user)
	{
		if (user->second>0)
		{
			return true;
		}
	}
	
	for (LicenseUseList::iterator user = license_use.begin(); user != license_use.begin(); ++user)
	{
		for (ModuleLicenseUseList::iterator module = user->second.begin(); module != user->second.end(); ++module)
		{
			if (module->second>0)
			{
				return true;
			}
		}
	}
	
	return false;
}


/* GetPassword()
 *    Generates a password for a particular query to the Secondary Algorithm cell.  */
DWORD ProtectionKey::GetPassword(DWORD query)
{
	HRESULT hr = S_OK;
	
	_variant_t password;
	
	password.vt = VT_UI4;
	password.ulVal = 0;
	
	BYTE query_string[4];
	for (short i=0, j=24; j >= 0 ; i++, j=short(j-8))
		query_string[i] = BYTE(BYTE(query>>j) & 0xFF);
	
	KeySpec::Header &header(m_keyspec->headers[L"Secondary Descriptor"]);
	hr = m_driver->Query(m_keyident, header.offset >> 4, query_string, &(password.ulVal), sizeof(password.ulVal));
	if (FAILED(hr))
		throw _com_error(hr);
	
	return password;
}


/* GetProductVersionPassword()
 *    Calculate and return the password for the specified product version
 *    by querying the secondary algorithm of the protection key.  If
 *    successful, returns the password for this version, otherwise
 *    returns 0.
 *
 *    Query = 01234567, where
 *       012 = Customer Number, first digit always equals 0, to avoid
 *             conflict with extension and SP/D module passwords.
 *       34  = Key Number
 *       567 = 3 most significant digits of product version
 *
 *    This query will produce a password that is therefore unique
 *    for this customer, with this key, and for this particular
 *    product version. Okay, not really. Note that since the first
 *    digit of the customer number is set to 0, customers 105 and
 *    205 will have the same product version passwords. Also, since the
 *    last digit of the version is ignored, versions 3.20A-3.20F
 *    will share the same password.  What can I say?  I ran out of
 *    digits! 
 */
DWORD ProtectionKey::GetProductVersionPassword(unsigned short customer_number, unsigned short key_number, unsigned short product_version)
{
	DWORD query = 
		(((DWORD)(customer_number)&0xFF) << 20) |
		(((DWORD)(key_number)&0xFF) << 12) |
		((DWORD)(product_version) >> 4);
	
	return GetPassword(query);
}

/* GetModulePassword()
 *    Calculate and return the password for a module license.
 *    An md5 hash is computed on the struct formed by:
 *    struct
 *	  {
 *		unsigned short customer_number, key_number;
 *		unsigned int module_id, units_licensed;
 *	  };
 *    The value used for the query is the first 4 bytes of the hash.
 */
DWORD ProtectionKey::GetModulePassword(unsigned short customer_number, unsigned short key_number, unsigned int module_id, unsigned int units_licensed)
{
	HRESULT hr = S_OK;
	class Query
	{
	public:
		Query(unsigned short customer_number, unsigned short key_number, unsigned int module_id, unsigned int units_licensed) : 
			m_customer_number(customer_number), m_key_number(key_number), m_module_id(module_id), m_units_licensed(units_licensed)
		{
			;
		}
	private:
		unsigned short m_customer_number, m_key_number;
		unsigned int m_module_id, m_units_licensed;
	};
	
	CryptoHelper crypto;
	CryptoHelper::Digest digest;
	Query query(customer_number, key_number, module_id, units_licensed);
	
	hr = crypto.HashData((BYTE*)&query, sizeof(Query), digest);
	if (FAILED(hr)) throw _com_error(hr);
	
	return GetPassword(*((DWORD*)digest.m_digest));
}

/* GetModulePassword()
 *    Calculate and return the password for a module license.
 *    An md5 hash is computed on the struct formed by:
 *    struct
 *	  {
 *		unsigned short customer_number, key_number;
 *		unsigned int module_id, units_licensed, password_number;
 *	  };
 *    The value used for the query is the first 4 bytes of the hash.
 */

DWORD ProtectionKey::GetModulePassword(unsigned short customer_number, unsigned short key_number, unsigned int module_id, unsigned int units_licensed, unsigned int password_number)
{
	HRESULT hr = S_OK;
	if(password_number == 0)	//Pre-incremental Password Numbering
		return GetModulePassword(customer_number, key_number, module_id, units_licensed);
	class QueryFiveElements
	{
	public:
		QueryFiveElements(unsigned short customer_number, unsigned short key_number, unsigned int module_id, unsigned int units_licensed, unsigned int password_number) : 
			m_customer_number(customer_number), m_key_number(key_number), m_module_id(module_id), m_units_licensed(units_licensed), m_password_number(password_number)
		{
			;
		}
	private:
		unsigned short m_customer_number, m_key_number;
		unsigned int m_module_id, m_units_licensed, m_password_number;
	};
	
	CryptoHelper crypto;
	CryptoHelper::Digest digest;
	QueryFiveElements query(customer_number, key_number, module_id, units_licensed, password_number);
	
	hr = crypto.HashData((BYTE*)&query, sizeof(QueryFiveElements), digest);
	if (FAILED(hr)) throw _com_error(hr);
	
	return GetPassword(*((DWORD*)digest.m_digest));
}


/* GetExtensionPassword()
 *    Calculate and return the password for the specified number of
 *    extension_days by querying the secondary algorithm of the protection
 *    key.  If successful, returns the password for this extension,
 *    otherwise returns 0.
 *
 *    Query = 01234567, where
 *       012 = Customer Number
 *       34  = Key Number
 *       5   = 0xF for 1st extension, 0xE for 2nd, 0xD for 3rd, ...
 *       67  = extension days
 *
 *    This query will produce a password that is therefore unique
 *    for this customer, with this key, for this particular extension.
 *
 *    DCM 28.nov.98 - Well, that used to be true. But with the advent
 *    of multiple extension passwords, there is a possible overlap
 *    with module passwords. Fortunately, because extension passwords
 *    are only valid when the key is on temporary license, the only
 *    overlap could be with "modzero" passwords, which could only happen
 *    if you were giving the 15th extension password. In other words,
 *    this overlap is no problem.
 */
DWORD ProtectionKey::GetExtensionPassword(unsigned short customer_number, unsigned short key_number, unsigned short extend_days, unsigned short extension_num)
{
	if (extension_num > MAX_EXTENSION_NUM)
		return (long)(0);
	
	DWORD query = 
         (((DWORD)(customer_number)&0xFFF) << 20) |
         (((DWORD)(key_number)&0xFF) << 12) |
         ((DWORD)(0xF - extension_num) << 8) |
         ((DWORD)(extend_days)&0xFF);
	
	return GetPassword(query);
}


/* getBasePassword()
 *    Calculate and return the password to change to key's status to base
 *    by querying the secondary algorithm of the protection key.  If
 *    successful, returns the base password, otherwise returns 0.
 *
 *    Query = 01234567, where
 *       012 = Customer Number
 *       34  = Key Number
 *       567 = 0xEFF
 *
 *    This query will produce a base password that is therefore unique
 *    for this customer and this key. 
 */
DWORD ProtectionKey::GetBasePassword(unsigned short customer_number, unsigned short key_number)
{
	DWORD query = 
		((DWORD(customer_number)&0xFFF) << 20) |
		((DWORD(key_number)&0xFF) << 12) | 0xEFF;
	
	return GetPassword(query);
}


//
// SPD specific
//

/* GetOutputPassword()
 *
 *
 *
 *   Query = 01234567, where
 *      012 = Customer Number
 *      34  = Key Number
 *      567 = Number of output units
 *
 *    This query will produce a password that is therefore unique
 *    for this customer, with this key, and with this many output units
 *    licensed.  AMAZING!
 -----------------------------------------------------------------------------*/
DWORD ProtectionKey::GetSPDOutputPassword(unsigned short customer_number, unsigned short key_number, unsigned short output_units)
{
	DWORD query = 
		((0xF << 28) |
		(((DWORD)(customer_number)&0xFF) << 20) |
		(((DWORD)(key_number)&0xFF) << 12) |
		((DWORD)(output_units)&0xFFF));
	
	return GetPassword(query);
}



/* GetModulePassword()
 *    Calculate and return the password for module mod_id by querying the
 *    secondary algorithm of the protection key.  If successful, returns
 *    the password for module mod_id, otherwise returns 0. Note that this
 *    function uses the units_licensed parameter to generate a password
 *    for the "next" incremental license. For example, if units_licensed
 *    is 2, the password this function returns should license the user for
 *    3 units.
 *
 *    Query = 01234567, where
 *       012 = Customer Number
 *       34  = Key Number
 *       5   = current units_licensed for module #mod_id, 0 for trial key
 *       67  = mod_id
 *
 *    This query will produce a password that is therefore unique
 *    for this customer, with this key, for this module, and with
 *    this many units licensed.
---------------------------------------------------------------------------*/
DWORD ProtectionKey::GetSPDModulePassword(unsigned short customer_number, unsigned short key_number, unsigned int mod_id, unsigned int units_licensed)
{
	DWORD query = 
		(((DWORD)(customer_number)&0xFFF) << 20) |
		(((DWORD)(key_number)&0xFF) << 12) |
		(((DWORD)(units_licensed)&0xF) << 8) |
		((DWORD)(mod_id)&0xFF);
	
	return GetPassword(query);
}



//==============================================================================
// Function:    GetPagesPerMinutePassword()
// Purpose:     This function will create and return a password for
//
//              Query = 01234567, where
//                      012 = Customer Number
//                      34  = Key Number
//                      56 = A0 (denotes Pages Per Minute)
//                      7 = Number of extensions
//
//              MAX NUMBER OF EXTENSION IS 14
//
//              This query will produce a password that is therefore unique
//              for this customer, with this key, and with this many pages
//              licensed.  AMAZING!
// Parameters:  ushort - # of extensions
// Returns:     ulong - password
//==============================================================================
DWORD ProtectionKey::GetSPDPagesPerMinutePassword(unsigned short customer_number, unsigned short key_number, unsigned int ppm_extensions)
{
	DWORD query = 
		(((DWORD)(customer_number)&0xFFF) << 20) |
		(((DWORD)(key_number)&0xFF) << 12) |
		((0xA0 &0xFF) << 4) |
		((DWORD)(ppm_extensions));
	
	return GetPassword(query);
}


// 
// SolSearcher specific
// 

DWORD ProtectionKey::GetSolsearcherModulePassword(unsigned short customer_number, unsigned short key_number, unsigned int module_id, unsigned int units_licensed)
{
	DWORD query = 
		(((DWORD)(customer_number)&0xFFF) << 20) |
		(((DWORD)(key_number)&0xFF) << 12);

	DWORD QueryModule_5 = 0;
	switch(module_id)
	{
		case 0:	// index servers
			QueryModule_5 = 0x8;
			query |= 
				(((DWORD)(QueryModule_5)&0xF) << 8) |
				((DWORD)(units_licensed)&0xFF);
			break;
		case 1: // report servers
			QueryModule_5 = 0x9;
			query |= 
				(((DWORD)(QueryModule_5)&0xF) << 8) |
				((DWORD)(units_licensed)&0xFF);
			break;
		case 2: // concurrent users
			QueryModule_5 = 0x7;
			query |=					//[solsearcher][concurrent users]
				((DWORD)(m_keyspec->products[8][2].fn_write(units_licensed))&0x7FF);
			break;
		case 3: // application databases
			QueryModule_5 = 0xA;
			query |= 
				(((DWORD)(QueryModule_5)&0xF) << 8) |
				((DWORD)(units_licensed)&0xFF);
			break;
		case 4: // document assembler
			QueryModule_5 = 0xB;
			query |= 
				(((DWORD)(QueryModule_5)&0xF) << 8) |
				((DWORD)(units_licensed)&0xFF);
			break;
		default:
			throw _com_error(E_INVALIDARG);
	}
	
	return GetPassword(query);
}



/* writeStatus()
 *    Change the status of the key to key_status for both the protection
 *    key and the local data member.  Returns SUCCESS or FAILURE.
---------------------------------------------------------------------------*/
HRESULT ProtectionKey::WriteStatus(unsigned short new_key_status)
{
	HRESULT hr = S_OK;
	
	try
	{
		if (isOnTrial() && new_key_status == BASE)
		{
			TrialToPermanent();
		}
		
		// revisit this code... I don't understand the
		// need for reading the current key status, 
		// nor the if statement in the original code
		
		//KeySpec::Header &header(m_keyspec->headers[L"Status"]);
		//WriteCellPhysical(header.offset >> 4, new_key_status);
		WriteHeader(L"Status", new_key_status);
	}
	catch (_com_error &e)
	{
		hr = e.Error();
	}
	
	return hr;
}

void ProtectionKey::TrialToPermanent()
{
	// zero out the modules
	// for each region of the key storage
	for (KeySpec::KeyRegionList::iterator region = m_keyspec->regions.begin(); region != m_keyspec->regions.end(); ++region)
	{
		// if the region is module storage
		if (0==wcsncmp(region->type,L"module",7))
		{
			// zero out each cell in the region
			for (unsigned int cell=(region->offset/16); cell<(unsigned int)((region->offset + region->bits)/16); ++cell)
			{
				WriteCellPhysical(cell, 0x0000);
			}
		}
	}
	
	// set the default module values
    // for each module for this product
	KeySpec::Product &product(m_keyspec->products[ReadHeaderCache(L"Product ID").uiVal]);
	for (KeySpec::Product::data_list_t::iterator module = product.data.begin(); module != product.data.end(); ++module)
	{
		WriteBitsPhysical(module->offset, module->bits, (unsigned int)module->default_license);
	}
}

void ProtectionKey::InitializePasswordNumber()
{
	WriteHeader(L"Password Number", _variant_t(0));
	WriteHeader(L"Headers Initialized", _variant_t(HEADER_INITIALIZED));
}



HRESULT ProtectionKey::WriteCounterDays(unsigned short extend_days)
{
	HRESULT hr = S_OK;
	
	unsigned short initialCounter, extendedCounter;
	unsigned short key_status = ReadHeaderCache(L"Status").uiVal;
	unsigned short key_type = ReadHeaderCache(L"Key Type").uiVal;
	
	if (key_type==KEYDevelopment && (extend_days < 0 || extend_days >= sizeof(KDPasswordHours)/sizeof(KDPasswordHours[0])))
		return E_INVALIDARG;
	
	if (key_type == KEYDevelopment)
	{
		initialCounter = KDPasswordHours[extend_days];
		extendedCounter = KDPasswordHours[extend_days];
	}
	else
	{
		initialCounter = (unsigned short)((extend_days+EXTRA_DAY_BUFFER)*(unsigned short)HOURS_PER_DAY);
		extendedCounter = (unsigned short)(extend_days*(unsigned short)HOURS_PER_DAY);
	}
	
	// setCounterDaysByHours(trial_days)
	// trial_days is actually hours to program for this case
	switch(key_status)
	{
		case INITIAL_TRIAL:
			WriteHeader(L"Initial Counter", initialCounter);
			//WriteCellPhysical(m_keyspec->headers[L"Initial Counter"].offset >> 4, initialCounter);
			break;
		case EXTENDED_TRIAL:
		case EXTENDED_TRIAL2:
		case EXTENDED_TRIAL3:
		case EXTENDED_TRIAL4:
		case EXTENDED_TRIAL5:
			WriteHeader(L"Extended Counter", extendedCounter);
			//WriteCellPhysical(m_keyspec->headers[L"Extended Counter"].offset >> 4, extendedCounter);
			break;
	}
	
	return S_OK;
}

/* writeExpirationDays()
 *    Set the expiration date to trial_days + the current day for both the
 *    protection key and the expirationDate data member.  Returns SUCCESS
 *    or FAILURE.
 */
HRESULT ProtectionKey::WriteExpirationDays(unsigned short extend_days)
{
	unsigned short key_type = ReadHeaderCache(L"Key Type").uiVal;
	unsigned short key_status = ReadHeaderCache(L"Status").uiVal;
	unsigned short trial_days;
	
	if (key_type==KEYDevelopment && (extend_days < 0 || extend_days >= sizeof(KDPasswordDays)/sizeof(KDPasswordDays[0])))
		return E_INVALIDARG;
	
	if (key_type == KEYDevelopment)
		trial_days = KDPasswordDays[extend_days];
	else
		trial_days = extend_days;
	
	if ((key_status == INITIAL_TRIAL || key_status == UNINITIALIZED_TRIAL) && key_type!=KEYDevelopment)
		trial_days += EXTRA_DAY_BUFFER;
	
	DWORD expirationDate = (DWORD)time(NULL) + (DWORD)trial_days * (DWORD)SECONDS_PER_DAY;
	
	try
	{
		WriteHeader(L"Expiration Date", _variant_t(expirationDate));
	}
	catch (_com_error &e)
	{
		return e.Error();
	}
	
	return S_OK;
}

