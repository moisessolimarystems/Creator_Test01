#pragma once

#include "..\common\FromSpd\L_CNtfsFolderName.h"												// CR.32662.V01; added.
#include "CryptoHelper.h"
#include "FlateHelper.h"

/*
 * CSmartBuffer 
 * 
 * Simplifies management of simple type buffers.
 * 
 * SLB 25.mar.2024 CR.32662.V02; Created to simplify Buffer management
 * in the Crypto helper classes.
 * SLB 15.jul.2025 CR.34222; Fix for exception that occured when Solimar License 
 * Diagnostic Data Viewer would load the same license data file, 2 or more times in
 * a row. 
 */
template <class T>
class CSmartBuffer
{
private:
	T *m_pTBuf;
	DWORD m_dwLen;

private:
	// protect against using the default constructor.
	CSmartBuffer() {}
public:
	CSmartBuffer(
		const T *pTBuf,
		DWORD dwLen) :
		m_pTBuf(NULL),
		m_dwLen(0)
	{
		if (!dwLen) ThrowHr(E_INVALIDARG, __WFUNCTION__, L"Constructor requires a non-zero buffer length.");

		InitBuffer(pTBuf, dwLen);
	}
	CSmartBuffer(
		const CSmartBuffer<T>& c):
		m_pTBuf(NULL),
		m_dwLen(0)																							// CR.34222; added.
	{
		InitBuffer(c.Buffer(), c.Length());
	}
	~CSmartBuffer()
	{
		if (m_pTBuf) delete[](m_pTBuf);
	}
	CSmartBuffer& operator = (const CSmartBuffer<T>& c)
	{
		InitBuffer(c.Buffer(), c.Length());
		return *this;
	}
	T* Buffer() const { return m_pTBuf; }
	DWORD Length() const { return m_dwLen; }

private:
	void InitBuffer(
		const T *pTData,
		DWORD dwLen)
	{
		// If new size is different than old, reallocate the buffer.
		if (dwLen != m_dwLen)
		{
			m_dwLen = dwLen;
			if (m_pTBuf) delete[] m_pTBuf;
			m_pTBuf = new T[dwLen];
		}

		// if pTData==NULL, then just clear buffer
		// otherwise, copy pTData into m_pbBuf.
		if (!pTData)
			memset(m_pTBuf, 0, dwLen * sizeof(T));
		else
			memcpy(m_pTBuf, pTData, dwLen * sizeof(T));
	}

};

using namespace SpdStreamable;

typedef CSmartBuffer<BYTE> CSmartBufferBYTE;
typedef CSmartBuffer<DWORD> CSmartBufferDWORD;

/*
 * CryptoAndFlateHelper
 *
 *
 * SLB 25.mar.2024 CR.32662.V02; Created to help manage heap cleanup for
 * the BYTE buffers used by the CryptoAndFlate helper methods.
 */
class CryptoAndFlateHelper
{
public:
	//static HRESULT StreamToEncryptCompressByteArray(
	//	BSTR bstrLicCode, 
	//	const BYTE *pByteListHashCode, 
	//	const DWORD byteListHashCodeLength, 
	//	BYTE *pByteListEncrypKey, 
	//	DWORD byteListEncrypKeyLength, 
	//	BSTR bstrStream, 
	//	VARIANT* pVtByteArray);
	//static HRESULT EncryptCompressByteArrayToStream(
	//	BSTR bstrLicCode,
	//	const BYTE* pByteListHashCode,
	//	const DWORD byteListHashCodeLength,
	//	BYTE* pByteListEncrypKey,
	//	DWORD byteListEncrypKeyLength,
	//	VARIANT vtByteArray,
	//	BSTR* pBstrStream,
	//	bool bExpectDateInByteArray = false);
	static HRESULT StreamToEncryptCompressByteArray(											// CR.32662.V02; added.
		const CSpdCString& wsLicCode,
		const CSmartBufferBYTE& hash_Code,
		const CSmartBufferBYTE& encrypt_Key,
		const CSpdCString& wsStream,	
		CSmartBufferBYTE& out_buffer);

	static HRESULT StreamToEncryptCompressByteArray(
		const CSpdCString &wsLicCode,																	// CR.32662.V01; modified.
		const BYTE* pByteListHashCode,
		const DWORD byteListHashCodeLength,
		BYTE* pByteListEncrypKey,
		DWORD byteListEncrypKeyLength,
		const CSpdCString &wsStream,																	// CR.32662.V01; modified.
		VARIANT* pVtByteArray);

	static HRESULT EncryptCompressByteArrayToStream(											// CR.32662.V02; added.
		const CSpdCString &wsLicCode,	
		const CSmartBufferBYTE &hash_code,
		const CSmartBufferBYTE &encrypt_key,
		const CSmartBufferBYTE &data_buffer,
		CSpdCString &wsOutStream,																		// CR.32662.V01; modified.
		bool bExpectDateInByteArray = false);
	
	static HRESULT EncryptCompressByteArrayToStream(
		const CSpdCString& wsLicCode,																	// CR.32662.V01; modified.
		const BYTE* pByteListHashCode,
		DWORD byteListHashCodeLength,
		const BYTE* pByteListEncrypKey,
		DWORD byteListEncrypKeyLength,
		VARIANT vtByteArray,
		CSpdCString& wsOutStream,																		// CR.32662.V01; modified.
		bool bExpectDateInByteArray = false);
	
};
