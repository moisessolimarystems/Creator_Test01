#pragma once

#ifndef _WIN32_DCOM
#define _WIN32_DCOM
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0403
#endif

#include <windows.h>
#include <wincrypt.h>
#include <string>
#include <tchar.h>

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
		public:
			// takes ownership of a key
			KeyRef(HCRYPTKEY key) : m_refcount(1), m_key(key) {;}
			virtual ~KeyRef() {if (m_key) CryptDestroyKey(m_key); m_key = 0;}
			
			//Attach();
			//Detach();
			
			unsigned int AddRef() {InterlockedIncrement(reinterpret_cast<long*>(&m_refcount)); return m_refcount;}
			unsigned int Release() {if (!InterlockedDecrement(reinterpret_cast<long*>(&m_refcount))) {delete this; return 0;} return m_refcount;}
			unsigned int Refcount() {return m_refcount;}
			
			operator HCRYPTKEY() const {return m_key;}
			
		private:
			unsigned int m_refcount;
			HCRYPTKEY m_key;
		};
		
		HRESULT m_error;
		KeyRef *m_pKey;
	};

	CryptoHelper();
	virtual ~CryptoHelper();

	HRESULT GetLastError();
	
	// encrypts/decrypts a buffer of data in place
	//static HRESULT EncryptDataSize(wchar_t *password, DWORD data_size, DWORD *buffer_size);
	//static HRESULT DecryptDataSize(const Key &key, DWORD data_size, DWORD *buffer_size);
	HRESULT EncryptData(BYTE *password, DWORD password_size, BYTE *pData, DWORD data_size);
	HRESULT DecryptData(BYTE *password, DWORD password_size, BYTE *pData, DWORD data_size);
	HRESULT HashData(const BYTE *pData, const DWORD data_size, Digest &digest);
	HRESULT SignHash(const Digest &digest, VARIANT *pvtSignature);
	HRESULT VerifySignature(const Digest &digest, const VARIANT *pvtSignature, const Key &public_key);
	HRESULT VerifySignature(const Digest &digest, const BYTE *pSignatureBuffer, const DWORD signatureBufferSize, const Key &public_key);
	HRESULT GenerateRandomBytes(BYTE *pData, const DWORD buffer_size);

	// CR.FIX.19122.v3 - Verify that the running user is not a ManagedProfile
	static HRESULT VerifyProfileRights();
	
public:
	HRESULT m_error;
	TCHAR m_str_container[128];
	HCRYPTPROV m_context;
};