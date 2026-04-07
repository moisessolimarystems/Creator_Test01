#include "CryptoAndFlateHelper.h"
#include "StringUtils.h"



/* StreamToEncryptCompressByteArray()
 *
 * SLB 26.mar.2024 CR.32662.V02; refactored code, no functionality changes intended.
 * Replaced usage of VARIANT with CSmartBufferBYTE.
 * SLB 27.mar.2024 CR.32662.V02; Tested.
 * SLB 08.may.2025 CR.33975; Method is well tested now. Modified to throw hr_exceptions 
 * so better debug logs can be generated, but generating debug logs has to be manually 
 * uncommented.
 -----------------------------------------------------------------------------------*/
HRESULT CryptoAndFlateHelper::StreamToEncryptCompressByteArray(
	const CSpdCString& wsLicCode, 
	const CSmartBufferBYTE& hash_code,
	const CSmartBufferBYTE& encrypt_key,
	const CSpdCString& wsStream,
	CSmartBufferBYTE& out_buffer)
{

	//CEnterExitMsg eem(__WFUNCTION__, L"Input stream size <%u>", wsStream.wcslen());
	//eem.SetHr();

	// Resource ptrs which may need to be cleaned up in catch () {}
	try
	{
		CSpdCString packet_string;
		// Can't use "\r\n" as a delimiter, c++ ToString() for license attribs adds 
		// "\n" for xml separation, the c# ToString uses " "
		wchar_t delimiter[] = L"\1";

		// write the bstrLicCode
		packet_string += wsLicCode;
		packet_string += delimiter;

		// write the streamed license package attribs
		packet_string += wsStream;
		packet_string += delimiter;

		// compute a hash of the file
		CryptoHelper context;
		CryptoHelper::Digest digest;
		CryptoHelper::Key packet_sign_key(context, hash_code.Buffer(), hash_code.Length());
		
		HRESULT hr(S_OK);
		hr = context.HashData((BYTE*)packet_string.c_str(), packet_string.wcslen()*sizeof(wchar_t), digest);
		if (FAILED(hr)) ThrowHr(hr, __WFUNCTION__, L"context.HashData() call failed.");

		// Generate the signed hash. Responsible for destroying pbBuf, allocated by SignHash().
		PBYTE pbBuf(NULL);
		DWORD dwBufLen(0);
		hr = context.SignHash(digest, pbBuf, dwBufLen);
		if (FAILED(hr)) ThrowHr(hr, __WFUNCTION__, L"context.SignHash() call failed.");

		// store signature (signed hash) into a smart buffer and free the one 
		// allocated by SignHash().
		CSmartBufferBYTE signed_hash(pbBuf, dwBufLen);
		delete[](pbBuf);

		// add the hash and the signature to the string
		packet_string += StringUtils::BinaryToStringWS(digest.m_digest, digest.DIGEST_SIZE);
		packet_string += delimiter;

		packet_string += StringUtils::BinaryToStringWS(signed_hash.Buffer(), signed_hash.Length());
		packet_string += delimiter;

		// packet_string is complete. Convert it to single byte (ANSI) string before 
		// compressing.
		std::string data = StringUtils::WstringToString(packet_string.c_str());

		PBYTE pbCompressedStream(NULL);
		long compressedStreamSize(0);
		pbCompressedStream = FlateHelper::CompressStream(data.c_str(), (long)data.length(), &compressedStreamSize);
		if (pbCompressedStream == NULL) ThrowHr(E_FAIL, __WFUNCTION__, L"Call to CompressStream() returned null.");

		// Store compressed data into the output buffer and free the one allocated by 
		// CompressStream().
		out_buffer = CSmartBufferBYTE(pbCompressedStream, compressedStreamSize);		// CR.32662.V02; added.
		delete[] pbCompressedStream;

		// encrypt the password packet
		hr = context.EncryptData(
			encrypt_key.Buffer(), encrypt_key.Length(),											// CR.32662.V02; modified.
			out_buffer.Buffer(), out_buffer.Length());											// CR.32662.V02; modified.
		if (FAILED(hr)) ThrowHr(hr, __WFUNCTION__, L"Call to context.EncryptData() failed."); 
	}
	catch (SpdException::hr_exception& e)
	{
		//CEnterExitMsg::TraceMsg(L"(%08X) %s() caught exception. %s", e.GetHr(), __WFUNCTION__, e.GetMessage().c_str());
		//eem.SetHr(e.GetHr());
		return e.GetHr();
	}
	catch (HRESULT &ehr)
	{
		//CEnterExitMsg::TraceMsg(L"(%08X) %s() caught exception.", ehr, __WFUNCTION__);
		//eem.SetHr(ehr);
		return ehr;
	}
	catch (...)
	{
		//CEnterExitMsg::TraceMsg(L"%s() caught general exception.", __WFUNCTION__);
		//eem.SetHr(E_FAIL);
		return E_FAIL;
	}

	return S_OK;
}

/* StreamToEncryptCompressByteArray()
 *
 * SLB 21.mar.2024 CR.32662.V01; refactored code, no functionality changes intended.
 * SLB 08.may.2025 CR.33975; Method is well tested now. Commented out debug msgs. 
 -----------------------------------------------------------------------------------*/
HRESULT CryptoAndFlateHelper::StreamToEncryptCompressByteArray(
	const CSpdCString& wsLicCode,
	const BYTE* pByteListHashCode,
	const DWORD byteListHashCodeLength,
	BYTE* pByteListEncrypKey,
	DWORD byteListEncrypKeyLength,
	const CSpdCString& wsStream,
	VARIANT* pVtByteArray)
{
	//CEnterExitMsg eem(__WFUNCTION__, L"(1) CR.32662.V02 used/tested.");

	HRESULT hr;
	CSmartBufferBYTE encrypted_data(NULL, 1);
	hr = StreamToEncryptCompressByteArray(
		wsLicCode,
		CSmartBufferBYTE(pByteListHashCode, byteListHashCodeLength),
		CSmartBufferBYTE(pByteListEncrypKey, byteListEncrypKeyLength),
		wsStream,
		encrypted_data);

	SAFEARRAY* psa = SafeArrayCreateVector(VT_UI1, 0, encrypted_data.Length());
	BYTE* pData(NULL);
	hr = SafeArrayAccessData(psa, (void**)&pData);
	if (FAILED(hr)) return hr;

	memcpy(pData, encrypted_data.Buffer(), encrypted_data.Length());
	SafeArrayUnaccessData(psa);

	hr = CComVariant(psa).Detach(pVtByteArray);
	if (FAILED(hr)) return hr;

	//CEnterExitMsg::TraceMsg(L"Encrypted/Compressed output data len is <%u> bytes", encrypted_data.Length());
	//eem.SetHr();
	
	return S_OK;
}

/* EncryptCompressByteArrayToStream() - Expects an encrypted and compressed Packet Stream,
 * Decrypts, decompresses, then validates Packet Stream info.
 * outputs 
 * 
 * SLB 18.mar.2024 CR.32662.V01; Cut out excess usage of COM constructs in favor
 * of standard C++ usage. Pretty heavy changes, though over all flow of method 
 * should be the same.
 * Substitutued throw HRESULTs with using ThrowHr().
 * SLB 25.mar.2024 CR.32662.V02; Fixed bugs related to calculating the hashed_data_size
 * and use of the StringToBinaryWS() method.
 * Modified to use CBuffer object for various byte buffers used. Helps with 
 * managing heap cleanup on exceptions.
 * Tested!
 -----------------------------------------------------------------------------------*/
HRESULT CryptoAndFlateHelper::EncryptCompressByteArrayToStream(
	const CSpdCString &wsLicCode, 
	const CSmartBufferBYTE& hash_code,																// CR.32662.V02; modified.
	const CSmartBufferBYTE& encrypt_key,															// CR.32662.V02; modified.
	const CSmartBufferBYTE& data_buffer,															// CR.32662.V02; modified.
	CSpdCString &wsOutStream,
	bool bExpectDateInByteArray)
{
	// CR.34040; tested.
	// CEnterExitMsg eem(__WFUNCTION__, L"(1) CR.32662.V02 used/tested.");

	// Declare outside of try {} so they can be cleaned up in catch () {}.
	PBYTE pbUncompressedBuf(NULL);

	try
	{
		// be safe, clear existing output data.
		wsOutStream.clear();																				// CR.32662.V01; added.

		// create a copy of the data buffer for decrypting
		HRESULT hr;
		CSmartBufferBYTE decrypted_buffer(data_buffer);											// CR.32662.V02; added.

		// Must flate first...
		{
			CryptoHelper context;
			hr = context.DecryptData(
				encrypt_key.Buffer(), encrypt_key.Length(),										// CR.32662.V02; modified.
				decrypted_buffer.Buffer(), decrypted_buffer.Length());						// CR.32662.V02; modified.
			if (FAILED(hr)) ThrowHr(hr, __WFUNCTION__, L"Decryption error.");
		}

		// Up to caller to free value from UnCompressStream
		long uncompressedBufSize(0);
		try
		{
			pbUncompressedBuf = FlateHelper::UnCompressStream(
				(char *)decrypted_buffer.Buffer(), decrypted_buffer.Length(),				// CR.32662.V02; modified.
				&uncompressedBufSize);
			if (pbUncompressedBuf == NULL) ThrowHr(E_INVALIDARG, __WFUNCTION__, L"Expected uncompressed buffer to be valid. check input.");
		}
		catch (...)
		{
			ThrowHr(E_INVALIDARG, __WFUNCTION__, L"UnCompressStream error exception occurred.");
		}
		
		// Here begins licensing specific code. Uncompressed data is expected to be 
		// a license packet.
		CSmartBufferBYTE raw_buf(NULL, uncompressedBufSize + 1);								// CR.32662.V02; modified.
		memcpy(raw_buf.Buffer(), pbUncompressedBuf, uncompressedBufSize);					// CR.32662.V02; modified.
		raw_buf.Buffer()[uncompressedBufSize] = 0;												// CR.32662.V02; modified.
		
		// Done with pbUncompressedBuf. Release the memory allocated by FlateHelper.
		delete[](pbUncompressedBuf);
		pbUncompressedBuf = NULL;

		// Create 2 copies of the uncompressed buffer in whcar_t format.
		CSpdCString wsPacketString = StringUtils::StringToWstring((const char *)raw_buf.Buffer());
		CSpdCString wsPacketString2(wsPacketString);

		// parse the password packet. Use wsPacketString1 for this.
		wchar_t *pNextToken(NULL);
		wchar_t delimiter[] = L"\1";

		// first string token will be the packet license code. It must match the 
		// passed in License code.
		// *Note* wcstok_s() will modify the wsPacketString2 object's string. 
		// This is expected, hence we have the original (wsPacketString) when needed.
		CSpdCString wsToken = wcstok_s((wchar_t *)wsPacketString2.c_str(), delimiter, &pNextToken);
		int nRes = wsToken.compare(wsLicCode, true);
		if (nRes != 0) ThrowHr(E_INVALIDARG, __WFUNCTION__, L"Invalid argument error 4. License code mismatch.");

		if (bExpectDateInByteArray)
		{
			// Next token will be License packet's expiration date. Verification here is 
			// to simply to make sure a token was found.
			wsToken == wcstok_s(NULL, delimiter, &pNextToken);
			if (wsToken.empty()) ThrowHr(E_INVALIDARG, __WFUNCTION__, L"Invalid arguemnt 5. Not enough tokens found.");
		}

		// Retrieve the nugget of data stored in this stream.
		CSpdCString wsStream = wcstok_s(NULL, delimiter, &pNextToken);
		if (wsStream.empty()) ThrowHr(E_INVALIDARG, __WFUNCTION__, L"Invalid argument 6. Not enough tokens found.");

		// Before we return this buffer, validate the hash values.
		// grab the hash and the hash signature
		wchar_t * pwcsHash = wcstok_s(NULL, delimiter, &pNextToken);
		CSpdCString wsHash(pwcsHash);
		if (wsHash.empty()) ThrowHr(E_INVALIDARG, __WFUNCTION__, L"Invalid argument 7. Not enough tokens found.");
		
		CSpdCString wsSignature = wcstok_s(NULL, delimiter, &pNextToken);
		if (wsSignature.empty()) ThrowHr(E_INVALIDARG, __WFUNCTION__, L"Invalid argument 8. Not enough tokens found.");
		
		// calculate the number of bytes to use to calculate the hash
		DWORD hashed_data_size = (DWORD)((BYTE *)pwcsHash - (BYTE *)wsPacketString2.c_str()); // CR.32662.V02; modified. CR.32662.V01; modified.

		// Compute the hash from the packet data. 
		// Note: Use the original wsPacketString var vs wsPacketString2 which has been 
		// hacked by the use of wcstok_s().
		CryptoHelper context;
		CryptoHelper::Digest computed_digest;
		hr = context.HashData((BYTE*)wsPacketString.c_str(), hashed_data_size, computed_digest);
		if (FAILED(hr)) ThrowHr(hr, __WFUNCTION__, L"HashData() call failed.");

		// Check to see that the hash string length is that expected by the digest.
		DWORD dwLen = StringUtils::StringToBinaryLength(wsHash.c_str());
		if (dwLen != computed_digest.DIGEST_SIZE) ThrowHr(E_INVALIDARG, __WFUNCTION__, L"Invalid argument 9. Hash length mismatch.");

		// Build the digest with the provided hash buffer.
		CryptoHelper::Digest provided_digest;
		StringUtils::StringToBinary(wsHash.c_str(), provided_digest.m_digest, provided_digest.DIGEST_SIZE);

		// compare the provided (signed?) digest against the computed digest
		if (computed_digest != provided_digest) ThrowHr(E_INVALIDARG, __WFUNCTION__, L"Invalid argument 10. Hash digest mismatch.");

		// Verify the hash signature
		// CR.32662.V01; removed usage of _variant_t here. Just allocate a C++ BYTE 
		// buffer to simplfy this code.
		// CR.32662.V02; Now use new CBuffer class vs. a raw BYTE buffer.
		// Note: BinarySignature expects a 2:1 length differential between the
		// input and output buffer length (each binary BYTE is stored as 2 chars in 
		// text).
		CSmartBufferBYTE BinarySignature(NULL, (DWORD)wsSignature.wcslen() / sizeof(wchar_t));	// CR.32662.V02; modified.
		StringUtils::StringToBinaryWS(
			wsSignature, 
			BinarySignature.Buffer(),																	// CR.32662.V02; modified.
			BinarySignature.Length());																	// CR.32662.V02; modified.

		// Signature buffer is ready. Verify the signature. 
		// Build the verification (public) key from provided hash code.
		CryptoHelper::Key signature_verify_key(context, hash_code.Buffer(), hash_code.Length()); // CR.32662.V02; moved.
		if (signature_verify_key.GetLastError() != 0) ThrowHr(HRESULT_FROM_WIN32(signature_verify_key.GetLastError()), __WFUNCTION__, L"Invalid argument 11. Hash key initialization error.");

		hr = context.VerifySignature(																	// CR.32662.V01; modified.
			computed_digest,
			BinarySignature.Buffer(), BinarySignature.Length(),								// CR.32662.V02; modified.
			signature_verify_key);
		if (FAILED(hr)) ThrowHr(hr, __WFUNCTION__, L"Hash verification failed.");

		// Success. The Password packet is valid. Set output and exit.
		wsOutStream = wsStream;
		return S_OK;
	}
	catch (SpdException::hr_exception &e)
	{
		if (pbUncompressedBuf) delete[] pbUncompressedBuf;

		CEnterExitMsg::TraceMsg(L"(%08X) Exception caught in %s(). %s", e.GetHr(), __WFUNCTION__, e.GetMessage().c_str());
		return e.GetHr();
	}
	catch (...)
	{
		if (pbUncompressedBuf) delete[] pbUncompressedBuf;

		CEnterExitMsg::TraceMsg(L"Unexpected exception caught in %s().", __WFUNCTION__);

		return E_FAIL;
	}

	return S_OK;
}

/* EncryptCompressByteArrayToStream() - overload.
 *
 * SLB 18.mar.2024 CR.32662.V01; Cut out excess usage of COM constructs in favor
 * of standard C++ usage. Pretty heavy changes, though over all flow of method
 * should be the same.
 * Substitutued throw HRESULTs with using ThrowHr().
 * SLB 25.mar.2024 CR.32662.V02; Created this overload for backward compatibility.
 * It just thunks call to version with CBuffer params.
 * Tested.
 * SLB 15.ju.2025 CR.34222; Code rearranging so that SAFEARRAY is locked for as 
 * breif a period as possible. Added a win32 exception handler.
 -----------------------------------------------------------------------------------*/
HRESULT CryptoAndFlateHelper::EncryptCompressByteArrayToStream(
	const CSpdCString& wsLicCode,
	const BYTE* pbByteListHashCode,
	DWORD byteListHashCodeLength,
	const BYTE* pbByteListEncrypKey,
	DWORD byteListEncrypKeyLength,
	VARIANT vtByteArray,
	CSpdCString& wsOutStream,
	bool bExpectDateInByteArray)
{
	// CR.34040; tested.
	CEnterExitMsg eem(__WFUNCTION__);
	eem.SetHr();
	
	try
	{
		// sanity check
		if (!(vtByteArray.vt & VT_ARRAY)) ThrowHr(E_INVALIDARG, __WFUNCTION__, L"Input byte array is not configured correctly(1).");
		if (!(vtByteArray.vt & VT_UI1)) ThrowHr(E_INVALIDARG, __WFUNCTION__, L"Input byte array is not configured correctly(2).");

		CSmartBufferBYTE sbHashcodeBuf(pbByteListHashCode, byteListHashCodeLength);	// CR.34222; added.
		CSmartBufferBYTE sbEncryptkeyBuf(pbByteListEncrypKey, byteListEncrypKeyLength);// CR.34222; added.

		// Pull packet data from Safearray and save in CSmartBufferBYTE buffer.
		// release the safearray as soon as the buffer copy has been created.
		PBYTE pbPacket(NULL);
		HRESULT hr;
		hr = SafeArrayAccessData(vtByteArray.parray, (void**)&pbPacket);
		if (FAILED(hr)) ThrowHr(hr, __WFUNCTION__, L"Call to SafeArrayAccessData() failed.");

		CSmartBufferBYTE sbInputBuf(pbPacket, vtByteArray.parray->rgsabound[0].cElements); // CR.34222; added.
		SafeArrayUnaccessData(vtByteArray.parray);

		//CEnterExitMsg::TraceMsg(L"DEBUG: buflens: Hashcode<%u>  Encryptkey<%u>  InputBuf<%u> CR.34222", sbHashcodeBuf.Length(), sbEncryptkeyBuf.Length(), sbInputBuf.Length());

		hr = EncryptCompressByteArrayToStream(wsLicCode, sbHashcodeBuf, sbEncryptkeyBuf, sbInputBuf, wsOutStream, bExpectDateInByteArray);
		if (FAILED(hr)) ThrowHr(hr, __WFUNCTION__, L"Call to EncryptCompressByteArrayToStream() failed.");

	}
	catch (SpdException::hr_exception &e)
	{
		CEnterExitMsg::TraceMsg(L"DEBUG %s() hr_exception caught.", __WFUNCTION__);
		return e.GetHr();
	}
	catch (SpdException::win32_exception& e)														// CR.34222; added.
	{
		// Catches AccessViolations and divide by zeroes.
		CEnterExitMsg::TraceMsg(L"DEBUG %s() win32_exception caught <%s>.", __WFUNCTION__, e.what());
		return E_FAIL;
	}
	return S_OK;
}