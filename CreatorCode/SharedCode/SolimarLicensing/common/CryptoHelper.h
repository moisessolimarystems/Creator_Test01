#pragma once

#ifndef _WIN32_DCOM
#define _WIN32_DCOM
#endif

// SLB 06.feb.2025 CR.32662.V05; Commented out definition of _WIN32_WINNT. This 
// defintion can cause the following error when building in projects which use /clr. 
//   "error LNK2022: metadata operation failed (8013118D) : Inconsistent layout information in duplicated types (_nlsversioninfo): (0x02000315)."
//#ifndef _WIN32_WINNT
//#define _WIN32_WINNT 0x0601																			// CR.32662; modified.
//#endif

#include <windows.h>
#include <wincrypt.h>
#include <string>
#include <tchar.h>

// CR.32662.V01; changed the definitions for the KEYLENGTH macros to represent
// the desired length vs. the encoded version (length shifted to the MSW)
// I Added the "2" at the end so new code coulde be distinguished from old.
#define KEY_PAIR_KEYLENGTH2 0x00000400
#define ENCRYPT_KEYLENGTH2	0x00000080
#define ENCRYPT_ALGORITHM	AT_KEYEXCHANGE		//CALG_RSA_KEYX //AT_KEYEXCHANGE	//CALG_RC4

class CryptoHelper
{
public:
	
	// Cryptographic hash digest
	class Digest
	{
	public:
		// digest size in bytes
		enum {DIGEST_SIZE = 16};

		Digest();
		Digest(DWORD alg);
		Digest(const Digest &digest);
		
		// comparison operators
		bool operator==(const Digest &d);
		bool operator<(const Digest &d);
		bool operator<=(const Digest &d);
		bool operator>(const Digest &d);
		bool operator>=(const Digest &d);
		bool operator!=(const Digest &d);
		
		DWORD m_algorithm;
		BYTE m_digest[DIGEST_SIZE];
	};


	// Encapsulates an asymetric encryption key handle
	class Key
	{
	public:
		// Uses CryptGenKey using arbitrarily defined defaults
		Key(const CryptoHelper &context);
		
		// Duplicates a key
		Key(const Key &k);
		
		// Uses CryptGenKey
		Key(const CryptoHelper &context, const DWORD key_length);
		// Uses CryptImportKey
		Key(const CryptoHelper &context, const BYTE *pKeyData, const DWORD key_data_length);
		// Uses CryptImportKey
		Key(const CryptoHelper &context, std::wstring key_file);
		
		virtual ~Key();
		
		HRESULT GetLastError();
		
		operator HCRYPTKEY() const;
		
		HRESULT ExportPublicKeySize(DWORD *buffer_size);
		HRESULT ExportPublicKey(BYTE *pBuffer, DWORD buffer_size);
		HRESULT ExportPrivateKeySize(DWORD *buffer_size);
		HRESULT ExportPrivateKey(BYTE *pBuffer, DWORD buffer_size);
		
	private:

		// a shareable key handle that implements reference counting
		class KeyRef
		{
		private:
			unsigned int m_refcount;
			HCRYPTKEY m_key;
			HRESULT m_error;

		public:
			// takes ownership of a key
			KeyRef(HCRYPTKEY key) : m_refcount(1), m_key(key), m_error(S_OK) {;}

			// Creates a key by importing the provided data.
			KeyRef(
				HCRYPTPROV context,
				const BYTE* pKeyData,
				const DWORD key_data_length) :
				m_refcount(0),
				m_key(0),
				m_error(S_OK)
			{
				HCRYPTKEY hkey;
				BOOL bRES = CryptImportKey(context, pKeyData, key_data_length, 0, 0/*CRYPT_EXPORTABLE */, &hkey);
				if (bRES == FALSE) { m_error = HRESULT_FROM_WIN32(::GetLastError()); return; }

				m_key = hkey;
				m_refcount = 1;
			}

			// Creates a key by generating the data.
			KeyRef(
				HCRYPTPROV context,
				ALG_ID alg_id = ENCRYPT_ALGORITHM,
				DWORD key_length = KEY_PAIR_KEYLENGTH2) :
				m_refcount(0),
				m_key(0),
				m_error(S_OK)
			{
				// Make sure key_length is not out of range.
				if (key_length & 0xFFFF0000) { m_error = E_INVALIDARG; return; }

				HCRYPTKEY hkey;
				BOOL bRES = CryptGenKey(context, alg_id, (key_length << 16) | CRYPT_EXPORTABLE, &hkey);
				if (bRES == FALSE) { m_error = HRESULT_FROM_WIN32(::GetLastError()); return; }

				m_key = hkey;
				m_refcount = 1;
			}


			virtual ~KeyRef() {if (m_key) CryptDestroyKey(m_key); m_key = 0;}
			
			HRESULT GetError() const { return m_error; }
			unsigned int AddRef() {InterlockedIncrement(reinterpret_cast<long*>(&m_refcount)); return m_refcount;}
			unsigned int Release() {if (!InterlockedDecrement(reinterpret_cast<long*>(&m_refcount))) {delete this; return 0;} return m_refcount;}
			unsigned int Refcount() {return m_refcount;}
			
			operator HCRYPTKEY() const {return m_key;}
		};

	public:
		KeyRef* m_pKey;
		HRESULT m_error;

	};

	CryptoHelper();
	virtual ~CryptoHelper();

	HRESULT GetLastError() const;																		// CR.32662.V01; modified.
	
	// encrypts/decrypts a buffer of data in place
	//static HRESULT EncryptDataSize(wchar_t *password, DWORD data_size, DWORD *buffer_size);
	//static HRESULT DecryptDataSize(const Key &key, DWORD data_size, DWORD *buffer_size);
	HRESULT EncryptData(
		BYTE *password, 
		DWORD password_size, 
		BYTE *pData, 
		DWORD data_size);
	HRESULT DecryptData(
		BYTE *password, 
		DWORD password_size, 
		BYTE *pData, 
		DWORD data_size);
	HRESULT HashData(
		const BYTE *pData, 
		size_t data_size, 
		Digest &digest);
	HRESULT VerifySignature(
		const Digest &digest, 
		const VARIANT *pvtSignature, 
		const Key &public_key) const;
	HRESULT VerifySignature(
		const Digest &digest, 
		const BYTE *pSignatureBuffer, 
		const DWORD signatureBufferSize, 
		const Key &public_key) const;
	HRESULT SignHash(
		const Digest& digest,
		VARIANT* pvtSignature) const;
	HRESULT SignHash(
		const Digest& digest, 
		PBYTE &ppBuf, 
		DWORD &dwBufLen) const;

	HRESULT GenerateRandomBytes(
		BYTE* pData, 
		const DWORD buffer_size);

	// CR.FIX.19122.v3 - Verify that the running user is not a ManagedProfile
	static HRESULT VerifyProfileRights();
	
public:
	HRESULT m_error;
	TCHAR m_str_container[128];
	HCRYPTPROV m_context;
};