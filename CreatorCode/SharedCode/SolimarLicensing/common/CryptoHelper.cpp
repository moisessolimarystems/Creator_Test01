#include "CryptoHelper.h"
#include "FromSpd\l_SpdException.h"																	// CR.34312.V01; added.

#define ENCRYPT_BLOCK_SIZE	8
// CR.32662.V01; These defines are not formatted correctly now. See new
// definitions in the CryptoHelper.h file.
//#define KEY_PAIR_KEYLENGTH  0x04000000
//#define ENCRYPT_KEYLENGTH	0x00800000
//#define ENCRYPT_ALGORITHM	AT_KEYEXCHANGE		//CALG_RSA_KEYX //AT_KEYEXCHANGE	//CALG_RC4
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
// 
// SLB 30.jul.2025 CR.34312.V01; changed OutputDebugStringW() call to CEnterExitMsg::TraceMsg() so 
// debug messages will have threadId and be formatted.
CryptoHelper::Key::Key(const CryptoHelper &context) : m_error(0), m_pKey(0)
{
	//CEnterExitMsg eem(L"EncryptKey1", L"CR.32662.V01 tested/used.");						// CR.34312.V01; modified.

	//HCRYPTKEY hkey;
	//
	////xxx 2048 bit key
	////xxx if (!CryptGenKey(context.m_context, AT_KEYEXCHANGE, 0x08000000 | CRYPT_EXPORTABLE, &hkey))
	//if (!CryptGenKey(context.m_context, ENCRYPT_ALGORITHM, KEY_PAIR_KEYLENGTH | CRYPT_EXPORTABLE, &hkey))
	//{
	//	m_error = HRESULT_FROM_WIN32(::GetLastError());
	//}
	//else
	//{
	//	m_pKey = new KeyRef(hkey);
	//}

	////xxx 2048 bit key
	m_pKey = new KeyRef(context.m_context, ENCRYPT_ALGORITHM, KEY_PAIR_KEYLENGTH2);
	m_error = m_pKey->GetError();

}

// Duplicates a key
// 
// SLB 30.jul.2025 CR.34312.V01; changed OutputDebugStringW() call to CEnterExitMsg::TraceMsg() so 
// debug messages will have threadId and be formatted.
CryptoHelper::Key::Key(const Key &k) : m_error(0), m_pKey(0)
{
	m_pKey = k.m_pKey;
	m_pKey->AddRef();
}

// Uses CryptGenKey
CryptoHelper::Key::Key(
	const CryptoHelper &context, 
	const DWORD key_length) : 
	m_error(S_OK), 
	m_pKey(NULL)
{
	//CEnterExitMsg eem(L"EncryptKey2", L"CR.32662.V01 tested/used.");						// CR.34312.V01; modified.

	//HCRYPTKEY hkey;

	//// make sure that the key length is not too large
	//if (key_length & 0xFFFF0000)
	//{
	//	m_error = E_INVALIDARG;
	//	return;
	//}
	//
	//if (!CryptGenKey(context.m_context, AT_KEYEXCHANGE, (key_length << 16) | CRYPT_EXPORTABLE, &hkey))
	//{
	//	m_error = HRESULT_FROM_WIN32(::GetLastError());
	//}
	//else
	//{
	//	m_pKey = new KeyRef(hkey);
	//}

	m_pKey = new KeyRef(context.m_context, AT_KEYEXCHANGE, key_length);
	m_error = m_pKey->GetError();
}

// Uses CryptImportKey
/* CryptoHelper::Key constructor
 *
 * SLB 21.mar.2024 CR.32662.V01; refactored code, no functionality changes intended.
 * SLB 21.mar.2024 CR.32662.V01; tested / verified.
 -----------------------------------------------------------------------------------*/
	CryptoHelper::Key::Key(
	const CryptoHelper &context, 
	const BYTE *pKeyData, 
	const DWORD key_data_length) : 
	m_error(0), 
	m_pKey(0)
{
	//CEnterExitMsg eem(L"EncryptKey3", L"CR.32662.V01 tested/used.");						// CR.34312.V01; modified.

	m_pKey = new KeyRef(context.m_context, pKeyData, key_data_length);
	m_error = m_pKey->GetError();
}

// Uses CryptImportKey
/* CryptoHelper::Key constructor
 *
 * SLB 21.mar.2024 CR.32662.V01; refactored code, no functionality changes intended.
 -----------------------------------------------------------------------------------*/
CryptoHelper::Key::Key(
	const CryptoHelper &context, 
	std::wstring key_file) : 
	m_error(S_OK), 
	m_pKey(NULL)
{
	//CEnterExitMsg eem(L"EncryptKey4", L"CR.32662.V01 tested/used.");						// CR.34312.V01; modified.

	// Declare outside of try {} so they can be cleaned up in catch () {}.
	HANDLE hFile(INVALID_HANDLE_VALUE);
	PBYTE pKeyBuffer(NULL);

	try
	{
		// open the source file
		hFile = CreateFileW(key_file.c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
		if (hFile == INVALID_HANDLE_VALUE) throw HRESULT_FROM_WIN32(::GetLastError());

		// read the file in to a bufffer. Anything greater than 0x80000 is considered to 
		// be an excessive maximum key data size.
		DWORD bytesread = 0;
		DWORD bytestoread = GetFileSize(hFile, 0);
		if (bytestoread >= 0x80000) throw E_INVALIDARG;

		pKeyBuffer = new BYTE[bytestoread];
		BOOL bRES = ReadFile(hFile, pKeyBuffer, bytestoread, &bytesread, 0);
		if (bRES == FALSE || bytesread != bytestoread) throw HRESULT_FROM_WIN32(::GetLastError()); 

		//// import the key from the buffer
		//HCRYPTKEY hkey;
		//bRES = CryptImportKey(context.m_context, pKeyBuffer, bytesread, 0, 0/*CRYPT_EXPORTABLE */, &hkey);
		//if (bRES == FALSE) throw HRESULT_FROM_WIN32(::GetLastError());

		m_pKey = new KeyRef(context.m_context, pKeyBuffer, bytesread);
		if (FAILED(m_pKey->GetError())) throw m_pKey->GetError();

		// free up temp resources
		CloseHandle(hFile);
		delete[] pKeyBuffer;
	}
	catch (HRESULT e)
	{
		if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
		if (pKeyBuffer) delete[] pKeyBuffer;
		
		m_error = e;
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

/* ExportPrivateKeySize()
 *
 * SLB 21.mar.2024 CR.32662.V01; refactored code, no functionality changes intended.
 -----------------------------------------------------------------------------------*/
HRESULT CryptoHelper::Key::ExportPrivateKeySize(DWORD *buffer_size)
{
	*buffer_size = 0;
	BOOL bRES = CryptExportKey((HCRYPTKEY)(*m_pKey), 0, PRIVATEKEYBLOB, 0, 0, buffer_size);
	if (bRES == FALSE && ::GetLastError() != ERROR_MORE_DATA) return HRESULT_FROM_WIN32(::GetLastError());

	return S_OK;
}

/* ExportPrivateKey()
 *
 * SLB 21.mar.2024 CR.32662.V01; refactored code, no functionality changes intended.
 -----------------------------------------------------------------------------------*/
HRESULT CryptoHelper::Key::ExportPrivateKey(
	BYTE *pBuffer, 
	DWORD buffer_size)
{
	BOOL bRES = CryptExportKey((HCRYPTKEY)(*m_pKey), 0, PRIVATEKEYBLOB, 0, pBuffer, &buffer_size);
	if (bRES == FALSE) return HRESULT_FROM_WIN32(::GetLastError());

	return S_OK;
}

CryptoHelper::CryptoHelper() : m_context(0), m_error(S_OK)
{
#ifdef _SOLIMAR_SERVICE_
	// CR.FIX.19122 - Service cannon run as a "Mandatory Profile" because of it needs more than the limited functionality provides
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
#elif _MSC_VER < 1400
	wcscpy(tstr_guid, wstr_guid);
#else
	wcscpy_s(tstr_guid, wstr_guid);
#endif
	
#if _MSC_VER < 1400
	_stprintf(m_str_container,TEXT("Solimar Key Container %s"),tstr_guid);
#else
	_stprintf_s(m_str_container,TEXT("Solimar Key Container %s"),tstr_guid);
#endif

	m_str_container[sizeof(m_str_container)/sizeof(TCHAR)-1] = 0;		// ensure null termination

	// CR.FIX.19122 - This creates the key tied to the profile on the system
	if (!CryptAcquireContext(&m_context, m_str_container, PROVIDER, PROVIDER_TYPE, CRYPT_NEWKEYSET))
#else
	// CR.FIX.19122 - User running as a "Mandatory Profile" has to use CRYPT_VERIFYCONTEXT, but has limited functionality
	// CR.FIX.19122 - This creates the key in memory, no key to delete from the profile when done
	if (!CryptAcquireContext(&m_context, NULL, MS_DEF_PROV, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
#endif

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
#ifdef _SOLIMAR_SERVICE_
		HCRYPTPROV deleted_context(0);
		if (!CryptAcquireContext(&deleted_context, m_str_container, PROVIDER, PROVIDER_TYPE, CRYPT_DELETEKEYSET))
			hr = HRESULT_FROM_WIN32(::GetLastError());
#endif
		CryptReleaseContext(m_context, 0);
		m_context=0;
	}
}

HRESULT CryptoHelper::GetLastError() const
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

/* EncryptData()
 *
 * SLB 21.mar.2024 CR.32662.V01; refactored code, no functionality changes intended.
 * SLB 27.mar.2024 CR.32662.V02; tested.
 -----------------------------------------------------------------------------------*/
HRESULT CryptoHelper::EncryptData(
	BYTE *password, 
	DWORD password_size, 
	BYTE *pData, 
	DWORD data_size)
{
	//CEnterExitMsg eem(__WFUNCTION__, L"CR.32662.V01 tested/used.");						// CR.34312.V01; modified.

	// Declare outside of try {} so they can be cleaned up in catch () {}.
	HCRYPTHASH hPasswordHash(0);
	HCRYPTKEY hKey(0);
	PBYTE pBuffer(NULL);

	try
	{
		// CR.FIX.19122 - return initialization error
		if (FAILED(GetLastError())) throw GetLastError();

		// sanity check
		if (!pData || !password) throw E_INVALIDARG;

		// hash the password
		BOOL bRES = CryptCreateHash(m_context, CALG_MD5, 0, 0, &hPasswordHash);
		if (bRES == FALSE) throw HRESULT_FROM_WIN32(::GetLastError());

		bRES = CryptHashData(hPasswordHash, password, password_size, 0);
		if (bRES == FALSE) throw HRESULT_FROM_WIN32(::GetLastError());

		// derive a session key off of the password
		bRES = CryptDeriveKey(m_context, CALG_RC4, hPasswordHash, (ENCRYPT_KEYLENGTH2 << 16) | CRYPT_EXPORTABLE, &hKey);
		if (bRES == FALSE) throw HRESULT_FROM_WIN32(::GetLastError());

		// set up the block encryption buffers
		DWORD block_length(1000 - 1000 % ENCRYPT_BLOCK_SIZE);
		DWORD buffer_length(block_length + ENCRYPT_BLOCK_SIZE);
		DWORD count(0), input_cursor(0);
		pBuffer = new BYTE[buffer_length];

		// encrypt the data one block at a time
		do
		{
			count = ((data_size - input_cursor) < block_length ? (data_size - input_cursor) : block_length);
			memset(pBuffer, 0, buffer_length);
			memcpy(pBuffer, pData + input_cursor, count);

			// encrypt
			bRES = CryptEncrypt(hKey, 0, (input_cursor + count >= data_size ? TRUE : FALSE), 0, pBuffer, &count, buffer_length);
			if (bRES == FALSE) throw HRESULT_FROM_WIN32(::GetLastError());

			// copy to output buffer
			memcpy(pData + input_cursor, pBuffer, count);
			input_cursor += count;
		} while (input_cursor < data_size);

		// Clean-up resources.
		delete[] pBuffer;
		CryptDestroyKey(hKey);
		CryptDestroyHash(hPasswordHash);
	}
	catch (HRESULT e)
	{
		if (pBuffer) delete[](pBuffer);
		if (hKey) CryptDestroyKey(hKey);
		if (hPasswordHash) CryptDestroyHash(hPasswordHash);

		return e;
	}

	return S_OK;
}
/* DecryptData() 
 * 
 * SLB 21.mar.2024 CR.32662.V01; refactored code, no functionality changes intended.
 -----------------------------------------------------------------------------------*/
HRESULT CryptoHelper::DecryptData(
	BYTE *password, 
	DWORD password_size, 
	BYTE *pData, 
	DWORD data_size)
{
	//CEnterExitMsg eem(__WFUNCTION__, L"CR.32662.V01 tested/used.");						// CR.34312.V01; modified.

	// Declare outside of try {} so they can be cleaned up in catch () {}.
	PBYTE pBuffer(NULL);
	HCRYPTHASH hPasswordHash(0);
	HCRYPTKEY hKey(0);

	try
	{
		// CR.FIX.19122 - return initialization error
		if (FAILED(GetLastError())) throw GetLastError();

		// sanity check
		if (!pData || !password) throw E_INVALIDARG;

		// hash the password
		BOOL bRES = CryptCreateHash(m_context, CALG_MD5, 0, 0, &hPasswordHash);
		if (bRES == FALSE) throw HRESULT_FROM_WIN32(::GetLastError());

		bRES = CryptHashData(hPasswordHash, password, password_size, 0);
		if (bRES == FALSE) throw HRESULT_FROM_WIN32(::GetLastError());

		// derive a session key off of the password
		bRES = CryptDeriveKey(m_context, CALG_RC4, hPasswordHash, (ENCRYPT_KEYLENGTH2 << 16) | CRYPT_EXPORTABLE, &hKey);
		if (bRES == FALSE) throw HRESULT_FROM_WIN32(::GetLastError());

		// set up the block encryption buffers
		DWORD block_length(1000 - 1000 % ENCRYPT_BLOCK_SIZE);
		DWORD buffer_length(block_length + ENCRYPT_BLOCK_SIZE);
		DWORD count(0), input_cursor(0);
		pBuffer = new BYTE[buffer_length];

		// encrypt the data one block at a time
		do
		{
			count = ((data_size - input_cursor) < block_length ? (data_size - input_cursor) : block_length);
			memset(pBuffer, 0, buffer_length);
			memcpy(pBuffer, pData + input_cursor, count);

			// encrypt
			bRES = CryptDecrypt(hKey, 0, (input_cursor + count >= data_size ? TRUE : FALSE), 0, pBuffer, &count);
			if (bRES == FALSE) throw HRESULT_FROM_WIN32(::GetLastError());

			// copy to output buffer
			memcpy(pData + input_cursor, pBuffer, count);
			input_cursor += count;
		} while (input_cursor < data_size);

		// Clean-up resources.
		CryptDestroyHash(hPasswordHash);
		delete[] (pBuffer);
		CryptDestroyKey(hKey);
	}
	catch (HRESULT e)
	{
		if (hPasswordHash) CryptDestroyHash(hPasswordHash);
		if (pBuffer) delete[] (pBuffer);
		if (hKey) CryptDestroyKey(hKey);

		return e;
	}
	
	return S_OK;
}

/* HashData()
 *
 * SLB 21.mar.2024 CR.32662.V01; refactored code, no functionality changes intended.
 * SLB 21.mar.2024 CR.32662.V01; tested/verified.
 -----------------------------------------------------------------------------------*/
HRESULT CryptoHelper::HashData(
	const BYTE *pbData, 
	size_t data_size, 																					// CR.32661.V01; modified.
	Digest &digest)
{
	//CEnterExitMsg eem(__WFUNCTION__, L"CR.32662.V01 tested/used.");						// CR.34312.V01; modified.

	// Declare outside of try {} so they can be cleaned up in catch () {}.
	HCRYPTHASH hHash(0);

	try
	{
		// CR.FIX.19122 - return initialization error
		if (FAILED(GetLastError())) throw GetLastError();

		// create a hash object (based on MD5)
		BOOL bRES = CryptCreateHash(m_context, CALG_MD5, 0, 0, &hHash);
		if (bRES == FALSE) throw HRESULT_FROM_WIN32(::GetLastError());

		bRES = CryptHashData(hHash, pbData, (DWORD)data_size, 0);
		if (bRES == FALSE) throw HRESULT_FROM_WIN32(::GetLastError());

		digest = Digest(CALG_MD5);
		DWORD digest_size = digest.DIGEST_SIZE;
		bRES = CryptGetHashParam(hHash, HP_HASHVAL, (BYTE*)digest.m_digest, &digest_size, 0);
		if (bRES == FALSE) throw HRESULT_FROM_WIN32(::GetLastError());

		// Clean-up resources.
		CryptDestroyHash(hHash);
	}
	catch (HRESULT e)
	{
		if (hHash != 0) CryptDestroyHash(hHash);
		return e;
	}
	return S_OK;
}

/* SignHash()
 *
 * SLB 21.mar.2024 CR.32662.V01; refactored code, no functionality changes intended.
 * SLB 21.mar.2024 CR.32662.V01; tested/verified.
 -----------------------------------------------------------------------------------*/
HRESULT CryptoHelper::SignHash(
	const Digest &digest, 
	VARIANT *pvtSignature) const
{
	//CEnterExitMsg eem(L"SignHash1", L"CR.32662.V01 tested/used.");						// CR.34312.V01; modified.

	// Declare outside of try {} so they can be cleaned up in catch () {}.
	PBYTE pbBuf(NULL);

	try
	{
		// CR.FIX.19122 - return initialization error
		if (FAILED(GetLastError())) throw GetLastError();

		HRESULT hr;
		DWORD dwBuflen(0);
		hr = SignHash(digest, pbBuf, dwBuflen);
		if (FAILED(hr)) throw hr;

		VariantInit(pvtSignature);
		pvtSignature->vt = VT_ARRAY | VT_UI1;
		pvtSignature->parray = SafeArrayCreateVector(VT_UI1, 0, dwBuflen);
		PBYTE pData = 0;
		hr = SafeArrayAccessData(pvtSignature->parray, (void**)&pData);
		if (FAILED(hr)) throw hr;

		memcpy(pData, pbBuf, dwBuflen);
		SafeArrayUnaccessData(pvtSignature->parray);

		// Clean-up resources.
		delete[](pbBuf);
	}
	catch (HRESULT e)
	{
		if (pbBuf) delete[](pbBuf);
		return e;
	}
	
	return S_OK;
}

/* VerifySignature()
 *
 * SLB 21.mar.2024 CR.32662.V01; refactored code, no functionality changes intended.
 -----------------------------------------------------------------------------------*/
HRESULT CryptoHelper::VerifySignature(
	const Digest &digest, 
	const VARIANT *pvtSignature, 
	const Key &public_key) const
{
	//CEnterExitMsg eem(L"VerifySignature1", L"CR.32662.V01 tested/used.");						// CR.34312.V01; modified.

	// CR.FIX.19122 - return initialization error
	if (FAILED(GetLastError())) return GetLastError();
	
	if (!pvtSignature || !pvtSignature->parray || !(pvtSignature->vt & VT_ARRAY) || !(pvtSignature->vt & VT_UI1))
		return E_INVALIDARG;
	
	BYTE *pSignatureData = 0;
	HRESULT hr = SafeArrayAccessData(pvtSignature->parray, (void**)&pSignatureData);
	if (FAILED(hr)) return hr;

	hr = VerifySignature(digest, pSignatureData, pvtSignature->parray->rgsabound[0].cElements, public_key);
	SafeArrayUnaccessData(pvtSignature->parray);
	if (FAILED(hr)) return hr;
	
	return S_OK;
}

/* VerifySignature()
 *
 * SLB 21.mar.2024 CR.32662.V01; refactored code, no functionality changes intended.
 * SLB 21.mar.2024 CR.32662.V01; tested/verified.
 -----------------------------------------------------------------------------------*/
HRESULT CryptoHelper::VerifySignature(
	const Digest &digest, 
	const BYTE *pSignatureBuffer, 
	const DWORD signatureBufferSize, 
	const Key &public_key) const
{
	//CEnterExitMsg eem(L"VerifySignature2", L"CR.32662.V01 tested/used.");						// CR.34312.V01; modified.

	// Declare outside of try {} so they can be cleaned up in catch () {}.
	HCRYPTHASH hHash(0);

	try
	{
		// CR.FIX.19122 - return initialization error
		if (FAILED(GetLastError())) throw GetLastError();

		// create a hash object (based on MD5)
		BOOL bRES = CryptCreateHash(m_context, digest.m_algorithm, 0, 0, &hHash);
		if (bRES == FALSE) throw HRESULT_FROM_WIN32(::GetLastError());

		bRES = CryptSetHashParam(hHash, HP_HASHVAL, (BYTE*)digest.m_digest, 0);
		if (bRES == FALSE) throw HRESULT_FROM_WIN32(::GetLastError());

		bRES = CryptVerifySignature(hHash, pSignatureBuffer, signatureBufferSize, public_key, 0, 0);
		if (bRES == FALSE) throw HRESULT_FROM_WIN32(::GetLastError());

		// Clean-up resources.
		CryptDestroyHash(hHash);
	}
	catch (HRESULT e)
	{
		if (hHash != 0) CryptDestroyHash(hHash);
		return e;
	}
	return S_OK;
}


HRESULT CryptoHelper::GenerateRandomBytes(BYTE *pData, DWORD buffer_size)
{
	// CR.FIX.19122 - return initialization error
	if (FAILED(GetLastError())) return GetLastError();

	if (!CryptGenRandom(m_context, buffer_size, pData))
		return HRESULT_FROM_WIN32(::GetLastError());
	return S_OK;
}

/* SignHash() - returns signed hash in allocated buffer.
 * Caller is responsible for freeing the buffer.
 * 
 * SLB 20.mar.2024 CR.32662.V01; created.
 * SLB 21.mar.2024 CR.32662.V01; tested/verified.
 * SLB 22.sep.2025 CR.34426; tested/verified.
 -----------------------------------------------------------------------------------*/
HRESULT CryptoHelper::SignHash(
	const Digest& digest, 
	PBYTE& pBuf, 
	DWORD& dwBufLen) const
{
	// Clear outputs.
	pBuf = NULL;
	dwBufLen = 0;

	// Declare outside of try {} so they can be cleaned up in catch () {}.
	HCRYPTHASH hHash = 0;
	try
	{
		// create a hash object (based on MD5)
		BOOL bRES = CryptCreateHash(m_context, digest.m_algorithm, 0, 0, &hHash);
		if (bRES == FALSE) throw HRESULT_FROM_WIN32(::GetLastError());

		bRES = CryptSetHashParam(hHash, HP_HASHVAL, (BYTE*)digest.m_digest, 0);
		if (bRES == FALSE) throw HRESULT_FROM_WIN32(::GetLastError());

		// Call CryptSignHash() first time to Determine length of buffer needed.
		bRES = CryptSignHash(hHash, AT_KEYEXCHANGE, 0, 0, 0, &dwBufLen);
		if (bRES == FALSE) throw HRESULT_FROM_WIN32(::GetLastError());

		// Create the buffer needed and call CryptSignHash() for real output.
		pBuf = new BYTE[dwBufLen];
		bRES = CryptSignHash(hHash, AT_KEYEXCHANGE, 0, 0, pBuf, &dwBufLen);
		if (bRES == FALSE) throw HRESULT_FROM_WIN32(::GetLastError());

		// Clean-up resources.
		CryptDestroyHash(hHash);
	}
	catch (HRESULT e)
	{
		if (hHash != 0) CryptDestroyHash(hHash);
		if (pBuf) delete[] (pBuf);

		return e;
	}
	return S_OK;
}

// CR.FIX.19122.v3 - Verify that the running user is not a ManagedProfile
HRESULT CryptoHelper::VerifyProfileRights()
{
	//CEnterExitMsg eem(__WFUNCTION__, L"CR.32662.V01 tested/used.");						// CR.34312.V01; modified.

	HRESULT hr = S_OK;

	TCHAR local_container[128];
	HCRYPTPROV local_context;

	wchar_t wstr_guid[64];
	TCHAR tstr_guid[64];
	
	GUID guid_container;
	CoCreateGuid(&guid_container);
	
	memset(local_container,0,sizeof(local_container));
	memset(tstr_guid, 0, sizeof(tstr_guid));
	memset(wstr_guid, 0, sizeof(wstr_guid));
	StringFromGUID2(guid_container, wstr_guid, 64);
	
// suport for both UNICODE and non-unicode builds. 
// Can't just use UNICODE because of the PROVIDER string definition being tied to the _UNICODE/UNICODE defines
#if !defined(UNICODE) && !defined(_UNICODE)
	wcstombs(tstr_guid, wstr_guid, sizeof(tstr_guid)/sizeof(TCHAR));
	tstr_guid[sizeof(tstr_guid)/sizeof(TCHAR)-1]=0;
#elif _MSC_VER < 1400
	wcscpy(tstr_guid, wstr_guid);
#else
	wcscpy_s(tstr_guid, wstr_guid);
#endif
	
#if _MSC_VER < 1400
	_stprintf(m_str_container,TEXT("Solimar Key Container %s"),tstr_guid);
#else
	_stprintf_s(local_container,TEXT("Solimar Key Container %s"),tstr_guid);
#endif

	local_container[sizeof(local_container)/sizeof(TCHAR)-1] = 0;		// ensure null termination

	// CR.FIX.19122.v3 - This creates the key tied to the profile on the system, it will fail if the user is a "Mandatory Profile"
	if (!CryptAcquireContext(&local_context, local_container, PROVIDER, PROVIDER_TYPE, CRYPT_NEWKEYSET))
	{
		hr = HRESULT_FROM_WIN32(::GetLastError());
		local_context = 0;
	}

	// Cleanup
	if (local_context!=0)
	{
		HCRYPTPROV deleted_context(0);

		// Try to delete key that was just created, ignore any errors.
		CryptAcquireContext(&deleted_context, local_container, PROVIDER, PROVIDER_TYPE, CRYPT_DELETEKEYSET);

		CryptReleaseContext(local_context, 0);
		local_context=0;
	}

	return hr;
}
