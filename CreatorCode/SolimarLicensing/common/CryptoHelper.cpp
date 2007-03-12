#include "CryptoHelper.h"

#define ENCRYPT_BLOCK_SIZE	8
#define KEY_PAIR_KEYLENGTH  0x04000000
#define ENCRYPT_KEYLENGTH	0x00800000
#define ENCRYPT_ALGORITHM	AT_KEYEXCHANGE		//CALG_RSA_KEYX //AT_KEYEXCHANGE	//CALG_RC4
#define PROVIDER			MS_STRONG_PROV		//MS_STRONG_PROV	//MS_DEF_PROV
#define PROVIDER_TYPE		PROV_RSA_FULL		//PROV_RSA_FULL

#include <tchar.h>
#include <stdio.h>

CryptoHelper::Digest::Digest() : m_algorithm(CALG_MD5)
{
	memset(m_digest, 0, DIGEST_SIZE);
}

CryptoHelper::Digest::Digest(DWORD alg) : m_algorithm(CALG_MD5)
{
	memset(m_digest, 0, DIGEST_SIZE);
}

CryptoHelper::Digest::Digest(const Digest &digest)
{
	m_algorithm = digest.m_algorithm;
	memcpy(m_digest, digest.m_digest, DIGEST_SIZE);
}

// comparison operators
bool CryptoHelper::Digest::operator==(const Digest &d)
{
	return (m_algorithm == d.m_algorithm && 0==memcmp(m_digest, d.m_digest, DIGEST_SIZE));
}

bool CryptoHelper::Digest::operator<(const Digest &d)
{
	if (m_algorithm>d.m_algorithm)
		return false;
	else if (m_algorithm<d.m_algorithm)
		return true;
	else 
		return (0>memcmp(m_digest, d.m_digest, DIGEST_SIZE));
}

bool CryptoHelper::Digest::operator<=(const Digest &d)
{
	return (*this==d || *this<d);
}

bool CryptoHelper::Digest::operator>(const Digest &d)
{
	return !(*this<=d);
}

bool CryptoHelper::Digest::operator>=(const Digest &d)
{
	return !(*this<d);
}

bool CryptoHelper::Digest::operator!=(const Digest &d)
{
	return !(*this==d);
}

//DWORD m_algorithm;
//BYTE m_digest[DIGEST_SIZE];


// Uses CryptGenKey using arbitrarily defined defaults
CryptoHelper::Key::Key(const CryptoHelper &context) : m_error(0), m_pKey(0)
{
	HCRYPTKEY hkey;
	
	//xxx 2048 bit key
	//xxx if (!CryptGenKey(context.m_context, AT_KEYEXCHANGE, 0x08000000 | CRYPT_EXPORTABLE, &hkey))
	if (!CryptGenKey(context.m_context, ENCRYPT_ALGORITHM, KEY_PAIR_KEYLENGTH | CRYPT_EXPORTABLE, &hkey))
	{
		m_error = HRESULT_FROM_WIN32(::GetLastError());
	}
	else
	{
		m_pKey = new KeyRef(hkey);
	}
}

// Duplicates a key
CryptoHelper::Key::Key(const Key &k) : m_error(0), m_pKey(0)
{
	m_pKey = k.m_pKey;
	m_pKey->AddRef();
}

// Uses CryptGenKey
CryptoHelper::Key::Key(const CryptoHelper &context, const DWORD key_length) : m_error(0), m_pKey(0)
{
	HCRYPTKEY hkey;

	// make sure that the key length is not too large
	if (key_length & 0xFFFF0000)
	{
		m_error = E_INVALIDARG;
		return;
	}
	
	if (!CryptGenKey(context.m_context, AT_KEYEXCHANGE, (key_length << 16) | CRYPT_EXPORTABLE, &hkey))
	{
		m_error = HRESULT_FROM_WIN32(::GetLastError());
	}
	else
	{
		m_pKey = new KeyRef(hkey);
	}
}

// Uses CryptImportKey
CryptoHelper::Key::Key(const CryptoHelper &context, const BYTE *pKeyData, const DWORD key_data_length) : m_error(0), m_pKey(0)
{
	HCRYPTKEY hkey;

	if (!CryptImportKey(context.m_context, pKeyData, key_data_length, 0, 0/*CRYPT_EXPORTABLE */, &hkey))
	{
		m_error = HRESULT_FROM_WIN32(::GetLastError());
	}
	else
	{
		//xxxyyy
		//DWORD keylen(128);
		//CryptSetKeyParam(hkey, KP_EFFECTIVE_KEYLEN, (BYTE*)&keylen, 0);
		m_pKey = new KeyRef(hkey);
	}
}

// Uses CryptImportKey
CryptoHelper::Key::Key(const CryptoHelper &context, std::wstring key_file) : m_error(0), m_pKey(0)
{
	HCRYPTKEY hkey;

	// open the source file
	HANDLE hFile = CreateFileW(key_file.c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		BYTE *pKeyBuffer=0;
		
		// read the file in to a bufffer
		DWORD bytestoread=0, bytesread=0;
		bytestoread = GetFileSize(hFile,0);
		if (bytestoread < 0x80000)	// reasonable maximum key data size. This allows for an encoding and headers that are 8x the max key size
		{
			pKeyBuffer = new BYTE[bytestoread];
			if (!ReadFile(hFile, pKeyBuffer, bytestoread, &bytesread, 0) || bytesread!=bytestoread)
			{
				m_error = HRESULT_FROM_WIN32(::GetLastError());
			}
		}
		else
		{
			m_error = E_INVALIDARG;
		}
		
		// close the file
		CloseHandle(hFile);
		
		// import the key from the buffer
		if (SUCCEEDED(m_error) && !CryptImportKey(context.m_context, pKeyBuffer, bytestoread, 0, 0/*CRYPT_EXPORTABLE */, &hkey))
		{
			m_error = HRESULT_FROM_WIN32(::GetLastError());
		}
		if (SUCCEEDED(m_error))
		{
			//xxxyyy
			//DWORD keylen(128);
			//CryptSetKeyParam(hkey, KP_EFFECTIVE_KEYLEN, (BYTE*)&keylen, 0);
			m_pKey = new KeyRef(hkey);
		}
		
		// free up the key buffer
		delete [] pKeyBuffer;
	}
	else
	{
		m_error = HRESULT_FROM_WIN32(::GetLastError());
	}	
}

CryptoHelper::Key::~Key()
{
	if (m_pKey)
		m_pKey->Release();
}

HRESULT CryptoHelper::Key::GetLastError()
{
	return m_error;
}

CryptoHelper::Key::operator HCRYPTKEY() const
{
	if (!m_pKey)
		return 0;
	return (const HCRYPTKEY)(*m_pKey);
}

HRESULT CryptoHelper::Key::ExportPublicKeySize(DWORD *buffer_size)
{
	*buffer_size = 0;
	if (!CryptExportKey((HCRYPTKEY)(*m_pKey), 0, PUBLICKEYBLOB, 0, 0, buffer_size) && ERROR_MORE_DATA!=::GetLastError())
	{
		return HRESULT_FROM_WIN32(::GetLastError());
	}
	return S_OK;
}

HRESULT CryptoHelper::Key::ExportPublicKey(BYTE *pBuffer, DWORD buffer_size)
{
	DWORD key_buffer_size = buffer_size;
	if (!CryptExportKey((HCRYPTKEY)(*m_pKey), 0, PUBLICKEYBLOB, 0, pBuffer, &key_buffer_size))
	{
		return HRESULT_FROM_WIN32(::GetLastError());
	}
	return S_OK;
}

HRESULT CryptoHelper::Key::ExportPrivateKeySize(DWORD *buffer_size)
{
	*buffer_size = 0;
	if (!CryptExportKey((HCRYPTKEY)(*m_pKey), 0, PRIVATEKEYBLOB, 0, 0, buffer_size) && ERROR_MORE_DATA!=::GetLastError())
	{
		return HRESULT_FROM_WIN32(::GetLastError());
	}
	return S_OK;
}

HRESULT CryptoHelper::Key::ExportPrivateKey(BYTE *pBuffer, DWORD buffer_size)
{
	DWORD key_buffer_size = buffer_size;
	if (!CryptExportKey((HCRYPTKEY)(*m_pKey), 0, PRIVATEKEYBLOB, 0, pBuffer, &key_buffer_size))
	{
		return HRESULT_FROM_WIN32(::GetLastError());
	}
	return S_OK;
}


CryptoHelper::CryptoHelper() : m_context(0), m_error(S_OK)
{
	wchar_t wstr_guid[64];
	TCHAR tstr_guid[64];
	
	GUID guid_container;
	CoCreateGuid(&guid_container);
	
	memset(m_str_container,0,sizeof(m_str_container));
	memset(tstr_guid, 0, sizeof(tstr_guid));
	memset(wstr_guid, 0, sizeof(wstr_guid));
	StringFromGUID2(guid_container, wstr_guid, 64);
	
// suport for both UNICODE and non-unicode builds. 
// Can't just use UNICODE because of the PROVIDER string definition being tied to the _UNICODE/UNICODE defines
#if !defined(UNICODE) && !defined(_UNICODE)
	wcstombs(tstr_guid, wstr_guid, sizeof(tstr_guid)/sizeof(TCHAR));
	tstr_guid[sizeof(tstr_guid)/sizeof(TCHAR)-1]=0;
#else
	wcscpy(tstr_guid, wstr_guid);
#endif
	
	_stprintf(m_str_container,TEXT("Solimar Key Container %s"),tstr_guid);
	m_str_container[sizeof(m_str_container)/sizeof(TCHAR)-1] = 0;		// ensure null termination
	
	if (!CryptAcquireContext(&m_context, m_str_container, PROVIDER, PROVIDER_TYPE, CRYPT_NEWKEYSET))
	{
		m_error = HRESULT_FROM_WIN32(::GetLastError());
		m_context = 0;
	}
}

CryptoHelper::~CryptoHelper()
{
	HRESULT hr = S_OK;
	if (m_context!=0)
	{
		HCRYPTPROV deleted_context(0);
		if (!CryptAcquireContext(&deleted_context, m_str_container, PROVIDER, PROVIDER_TYPE, CRYPT_DELETEKEYSET))
			hr = HRESULT_FROM_WIN32(::GetLastError());
		CryptReleaseContext(m_context, 0);
		m_context=0;
	}
}

HRESULT CryptoHelper::GetLastError()
{
	return m_error;
}

/*
HRESULT CryptoHelper::EncryptDataSize(wchar_t *password, DWORD data_size, DWORD *buffer_size)
{
	HRESULT hr = S_OK;
	
	if (!CryptEncrypt(key, 0, TRUE, 0, 0, &data_size, *buffer_size) && ::GetLastError()!=ERROR_MORE_DATA)
	{
		hr = HRESULT_FROM_WIN32(::GetLastError());
	}
	*buffer_size = data_size;
	
	return hr;
}
*/

/*
HRESULT CryptoHelper::DecryptDataSize(const Key &key, DWORD data_size, DWORD *buffer_size)
{
	HRESULT hr = S_OK;
	
	*buffer_size = data_size;

	return hr;
}
*/

HRESULT CryptoHelper::EncryptData(BYTE *password, DWORD password_size, BYTE *pData, DWORD data_size)
{
	HRESULT hr = S_OK;
	
	// sanity check
	if (!pData || !password)
		return E_INVALIDARG;
	
	// hash the password
	HCRYPTHASH hPasswordHash=0;
	if(SUCCEEDED(hr) && !CryptCreateHash(m_context, CALG_MD5, 0, 0, &hPasswordHash))
		hr = HRESULT_FROM_WIN32(::GetLastError());
	if(SUCCEEDED(hr) && !CryptHashData(hPasswordHash, password, password_size, 0))
		hr = HRESULT_FROM_WIN32(::GetLastError());
	
	// derive a session key off of the password
	HCRYPTKEY hKey(0);
	if(SUCCEEDED(hr) && !CryptDeriveKey(m_context, CALG_RC4, hPasswordHash, CRYPT_EXPORTABLE | ENCRYPT_KEYLENGTH, &hKey))
		hr = HRESULT_FROM_WIN32(::GetLastError());
	
	// destroy the hash
    if (hPasswordHash)
		CryptDestroyHash(hPasswordHash);
	
	// set up the block encryption buffers
	DWORD block_length(1000 - 1000 % ENCRYPT_BLOCK_SIZE);
	DWORD buffer_length(block_length + ENCRYPT_BLOCK_SIZE);
	DWORD count(0),input_cursor(0);
	BYTE *pBuffer = new BYTE[buffer_length];

	// encrypt the data one block at a time
	if (SUCCEEDED(hr))
	{
		do
		{
			count = ((data_size-input_cursor)< block_length ? (data_size-input_cursor) : block_length);
			memset(pBuffer, 0, buffer_length);
			memcpy(pBuffer, pData + input_cursor, count);
			
			// encrypt
			if (!CryptEncrypt(hKey, 0, (input_cursor + count >= data_size ? TRUE : FALSE), 0, pBuffer, &count, buffer_length))
				hr = HRESULT_FROM_WIN32(::GetLastError());
			
			// copy to output buffer
			if (SUCCEEDED(hr))
			{
				memcpy(pData + input_cursor, pBuffer, count);
				input_cursor += count;
			}
		} while (input_cursor<data_size && SUCCEEDED(hr));
	}
	
	if (hKey)
		CryptDestroyKey(hKey);

	return hr;
}

HRESULT CryptoHelper::DecryptData(BYTE *password, DWORD password_size, BYTE *pData, DWORD data_size)
{
	HRESULT hr = S_OK;
	
	// sanity check
	if (!pData || !password)
		return E_INVALIDARG;

	// hash the password
	HCRYPTHASH hPasswordHash=0;
	if(SUCCEEDED(hr) && !CryptCreateHash(m_context, CALG_MD5, 0, 0, &hPasswordHash))
		hr = HRESULT_FROM_WIN32(::GetLastError());
	if(SUCCEEDED(hr) && !CryptHashData(hPasswordHash, password, password_size, 0))
		hr = HRESULT_FROM_WIN32(::GetLastError());
	
	// derive a session key off of the password
	HCRYPTKEY hKey(0);
	if(SUCCEEDED(hr) && !CryptDeriveKey(m_context, CALG_RC4, hPasswordHash, CRYPT_EXPORTABLE | ENCRYPT_KEYLENGTH, &hKey))
		hr = HRESULT_FROM_WIN32(::GetLastError());
	
	// destroy the hash
    if (hPasswordHash)
		CryptDestroyHash(hPasswordHash);
	
	// set up the block encryption buffers
	DWORD block_length(1000 - 1000 % ENCRYPT_BLOCK_SIZE);
	DWORD buffer_length(block_length + ENCRYPT_BLOCK_SIZE);
	DWORD count(0),input_cursor(0);
	BYTE *pBuffer = new BYTE[buffer_length];
	
	// encrypt the data one block at a time
	if (SUCCEEDED(hr))
	{
		do
		{
			count = ((data_size-input_cursor)< block_length ? (data_size-input_cursor) : block_length);
			memset(pBuffer, 0, buffer_length);
			memcpy(pBuffer, pData + input_cursor, count);
			
			// encrypt
			if (!CryptDecrypt(hKey, 0, (input_cursor + count >= data_size ? TRUE : FALSE), 0, pBuffer, &count))
				hr = HRESULT_FROM_WIN32(::GetLastError());
			
			// copy to output buffer
			if (SUCCEEDED(hr))
			{
				memcpy(pData + input_cursor, pBuffer, count);
				input_cursor += count;
			}
		} while (input_cursor<data_size && SUCCEEDED(hr));
	}
	
	if (hKey)
		CryptDestroyKey(hKey);
	
	return hr;
}

HRESULT CryptoHelper::HashData(const BYTE *pData, const DWORD data_size, Digest &digest)
{
	HRESULT hr = S_OK;
	
	digest = Digest(CALG_MD5);
	
	// create a hash object (based on MD5)
	HCRYPTHASH hHash=0;
	if(!CryptCreateHash(m_context, CALG_MD5, 0, 0, &hHash))
	{
		hr = HRESULT_FROM_WIN32(::GetLastError());
	}
	else
	{
		if(!CryptHashData(hHash, pData, data_size, 0))
		{
			hr = HRESULT_FROM_WIN32(::GetLastError());
		}
		else
		{
			DWORD digest_size = digest.DIGEST_SIZE;
			if (!CryptGetHashParam(hHash, HP_HASHVAL, (BYTE*)digest.m_digest, &digest_size, 0))
			{
				hr = HRESULT_FROM_WIN32(::GetLastError());
			}
		}
		CryptDestroyHash(hHash);
	}
	
	return hr;
}

HRESULT CryptoHelper::SignHash(const Digest &digest, VARIANT *pvtSignature)
{
	HRESULT hr = S_OK;
	
	VariantInit(pvtSignature);
	
	// create a hash object (based on MD5)
	HCRYPTHASH hHash=0;
	if(!CryptCreateHash(m_context, digest.m_algorithm, 0, 0, &hHash))
	{
		hr = HRESULT_FROM_WIN32(::GetLastError());
	}
	else
	{
		if (!CryptSetHashParam(hHash, HP_HASHVAL, (BYTE*)digest.m_digest, 0))
		{
			hr = HRESULT_FROM_WIN32(::GetLastError());
		}
		
		DWORD signature_length(0);
		if (SUCCEEDED(hr) && !CryptSignHash(hHash, AT_KEYEXCHANGE, 0, 0, 0, &signature_length))
			hr = HRESULT_FROM_WIN32(::GetLastError());
		
		if (SUCCEEDED(hr))
		{
			pvtSignature->vt = VT_ARRAY | VT_UI1;
			pvtSignature->parray = SafeArrayCreateVector(VT_UI1, 0, signature_length);
			BYTE *pData = 0;
			if (SUCCEEDED(hr = SafeArrayAccessData(pvtSignature->parray, (void**)&pData)))
			{
	            if (!CryptSignHash(hHash, AT_KEYEXCHANGE, 0, 0, pData, &signature_length))
					hr = HRESULT_FROM_WIN32(::GetLastError());
				SafeArrayUnaccessData(pvtSignature->parray);
			}
		}
		CryptDestroyHash(hHash);
	}
	
	return hr;
}

HRESULT CryptoHelper::VerifySignature(const Digest &digest, const VARIANT *pvtSignature, const Key &public_key)
{
	HRESULT hr = S_OK;
	
	if (!pvtSignature || !pvtSignature->parray || !(pvtSignature->vt & VT_ARRAY) || !(pvtSignature->vt & VT_UI1))
		return E_INVALIDARG;
	
	BYTE *pSignatureData = 0;
	if (SUCCEEDED(hr = SafeArrayAccessData(pvtSignature->parray, (void**)&pSignatureData)))
	{
		hr = VerifySignature(digest, pSignatureData, pvtSignature->parray->rgsabound[0].cElements, public_key);
		SafeArrayUnaccessData(pvtSignature->parray);
	}
	
	return hr;
}

HRESULT CryptoHelper::VerifySignature(const Digest &digest, const BYTE *pSignatureBuffer, const DWORD signatureBufferSize, const Key &public_key)
{
	HRESULT hr = S_OK;
	
	// create a hash object (based on MD5)
	HCRYPTHASH hHash=0;
	if(!CryptCreateHash(m_context, digest.m_algorithm, 0, 0, &hHash))
	{
		hr = HRESULT_FROM_WIN32(::GetLastError());
	}
	else
	{
		if (!CryptSetHashParam(hHash, HP_HASHVAL, (BYTE*)digest.m_digest, 0))
		{
			hr = HRESULT_FROM_WIN32(::GetLastError());
		}
		
		if (!CryptVerifySignature(hHash, pSignatureBuffer, signatureBufferSize, public_key, 0, 0))
			hr = HRESULT_FROM_WIN32(::GetLastError());
		
		CryptDestroyHash(hHash);
	}
	
	return hr;
}


HRESULT CryptoHelper::GenerateRandomBytes(BYTE *pData, DWORD buffer_size)
{
	if (!CryptGenRandom(m_context, buffer_size, pData))
		return HRESULT_FROM_WIN32(::GetLastError());
	return S_OK;
}
