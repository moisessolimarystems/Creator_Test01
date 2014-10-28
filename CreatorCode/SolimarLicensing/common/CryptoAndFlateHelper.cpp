#include "CryptoAndFlateHelper.h"
#include "StringUtils.h"



HRESULT CryptoAndFlateHelper::StreamToEncryptCompressByteArray(
	BSTR bstrLicCode, 
	const BYTE *pByteListHashCode, 
	const DWORD byteListHashCodeLength, 
	BYTE *pByteListEncrypKey, 
	DWORD byteListEncrypKeyLength, 
	BSTR bstrStream, 
	VARIANT* pVtByteArray)
{
	HRESULT hr(S_OK);
	try
	{
		_bstr_t packet_string;
		//Can't use "\r\n" as a delimiter, c++ ToString() for license attribs adds "\n" for xml separation, the c# ToString uses " "
		wchar_t delimiter[] = L"\1";

		// write the bstrLicCode
		packet_string += bstrLicCode;
		packet_string += delimiter;

		// write the streamed license package attribs
		packet_string += bstrStream;
		packet_string += delimiter;

		// compute a hash of the file
		CryptoHelper context;
		CryptoHelper::Digest digest;
		CryptoHelper::Key packet_sign_key(context, pByteListHashCode, byteListHashCodeLength);
		
		_variant_t vtSignature;
		hr = context.HashData((BYTE*)(wchar_t*)packet_string, (packet_string.length())*sizeof(wchar_t), digest);
		if (FAILED(hr)) throw hr;

		// sign the hash
		hr = context.SignHash(digest, &vtSignature);
		if (FAILED(hr)) throw hr;

		// add the hash and the signature to the string
		packet_string += StringUtils::BinaryToString(digest.m_digest, digest.DIGEST_SIZE);
		packet_string += delimiter;

		BYTE *pSignature = 0;
		if (FAILED(hr = SafeArrayAccessData(vtSignature.parray, (void**)&pSignature))) return hr;
		packet_string += StringUtils::BinaryToString(pSignature, vtSignature.parray->rgsabound[0].cElements);
		packet_string += delimiter;
		SafeArrayUnaccessData(vtSignature.parray);

		std::string data = StringUtils::WstringToString(std::wstring(packet_string));
		long compressedStreamSize(0);

		unsigned char* pCompressedStream = FlateHelper::CompressStream((char*)(data.c_str()), (long)data.length(), &compressedStreamSize);
		if(pCompressedStream != NULL)
		{
			// package the string in to a variant
			pVtByteArray->vt = VT_ARRAY | VT_UI1;
			pVtByteArray->parray = SafeArrayCreateVector(VT_UI1, 0, (compressedStreamSize+1)*sizeof(char));

			BYTE *pPacketData = 0;
			if (FAILED(hr = SafeArrayAccessData(pVtByteArray->parray, (void**)&pPacketData))) return hr;
			memcpy(pPacketData, pCompressedStream, (compressedStreamSize+1)*sizeof(char));
			
			//// encrypt the password packet
			hr = context.EncryptData(	pByteListEncrypKey, 
										byteListEncrypKeyLength, 
										pPacketData, 
										(compressedStreamSize+1)*sizeof(char));
			delete [] pCompressedStream;
		}

		SafeArrayUnaccessData(pVtByteArray->parray);
	}
	catch (HRESULT &ehr)
	{
		hr = ehr;
	}
	catch (...)
	{
		hr = E_FAIL;
	}
	return hr;
}

HRESULT CryptoAndFlateHelper::EncryptCompressByteArrayToStream(
	BSTR bstrLicCode, 
	const BYTE *pByteListHashCode, 
	const DWORD byteListHashCodeLength, 
	BYTE *pByteListEncrypKey, 
	DWORD byteListEncrypKeyLength, 
	VARIANT vtByteArray, 
	BSTR *pBstrStream,
	bool bExpectDateInByteArray)
{
	HRESULT hr(E_INVALIDARG);
	wchar_t *pPacketString1 = 0;
	wchar_t *pPacketString2 = 0;
	try
	{
		
		// sanity check
		if (!(vtByteArray.vt & VT_ARRAY) || !(vtByteArray.vt & VT_UI1))
			throw E_INVALIDARG;

		// create a copy of the password packet for decrypting
		BYTE *pPacket = 0;
		if (FAILED(hr = SafeArrayAccessData(vtByteArray.parray, (void**)&pPacket)))
			throw hr;

		//Must inflate first...
		unsigned char* uncompressedBuf = NULL;
		long uncompressedBufSize(0);
		unsigned char* uncompressedBufNullTerm = NULL;
		DWORD password_packet_length = vtByteArray.parray->rgsabound[0].cElements;

		{
			CryptoHelper context;
			hr = context.DecryptData(pByteListEncrypKey, byteListEncrypKeyLength, (BYTE*)pPacket, password_packet_length);
			if(FAILED(hr))
				throw E_INVALIDARG;
		}


		//Up to caller to free value from UnCompressStream
		try
		{
			uncompressedBuf = FlateHelper::UnCompressStream((char*)pPacket, password_packet_length, &uncompressedBufSize);
		}
		catch(...)
		{
			// we're done with the source variant
			SafeArrayUnaccessData(vtByteArray.parray);
			throw E_INVALIDARG;
		}
		// we're done with the source variant
		pPacket = 0;
		SafeArrayUnaccessData(vtByteArray.parray);
		if(uncompressedBuf == NULL)
			throw E_INVALIDARG;
		
		uncompressedBufNullTerm = new unsigned char[uncompressedBufSize+1];
		memcpy(uncompressedBufNullTerm, uncompressedBuf, (uncompressedBufSize)*sizeof(unsigned char));
		uncompressedBufNullTerm[uncompressedBufSize] = '\0';

		std::wstring tmpString = StringUtils::StringToWstring(std::string((char*)uncompressedBufNullTerm));

		//
		// decrypt the password packet, this copy is used for strtok
		pPacketString1 = (wchar_t*)new BYTE[(uncompressedBufSize+1)*sizeof(wchar_t)];
		// this copy is used to verify the hash of the packet data stored in the password packet
		pPacketString2 = (wchar_t*)new BYTE[(uncompressedBufSize+1)*sizeof(wchar_t)];
		memset(pPacketString1, 0, (uncompressedBufSize+1)*sizeof(wchar_t));
		memset(pPacketString2, 0, (uncompressedBufSize+1)*sizeof(wchar_t));
		memcpy(pPacketString1, tmpString.c_str(), (uncompressedBufSize)*sizeof(wchar_t));
		memcpy(pPacketString2, tmpString.c_str(), (uncompressedBufSize)*sizeof(wchar_t));
		delete [] uncompressedBuf;
		delete [] uncompressedBufNullTerm;

		// parse the password packet
		wchar_t *pNextToken;
		wchar_t* pToken(0);
		wchar_t delimiter[] = L"\1";
		if (!(pToken = wcstok_s(pPacketString1, delimiter,&pNextToken)) || wcscmp(pToken, bstrLicCode)!=0) 
			throw E_INVALIDARG;

		wchar_t *expiresLicensePacket = NULL;
		if(bExpectDateInByteArray == true)
		{
			if (!(expiresLicensePacket = wcstok_s(NULL, delimiter, &pNextToken))) 
				throw E_INVALIDARG;
		}

		wchar_t *stream = NULL;
		if (!(stream = wcstok_s(NULL, delimiter,&pNextToken))) 
			throw E_INVALIDARG;

		// grab the hash and the hash signature
		wchar_t *hash = NULL;
		wchar_t *signature = NULL;
		if (!(hash = wcstok_s(NULL, delimiter,&pNextToken))) 
			throw E_INVALIDARG;
		if (!(signature = wcstok_s(NULL, delimiter,&pNextToken))) 
			throw E_INVALIDARG;
		
		// calculate the number of bytes to used to calculate the hash
		DWORD hashed_data_size = (DWORD)((BYTE*)hash - (BYTE*)pPacketString1);

		// hash the string
		CryptoHelper context;
		CryptoHelper::Digest computed_digest, provided_digest;
		CryptoHelper::Key signature_verify_key(context, pByteListHashCode, byteListHashCodeLength);
		hr = context.HashData((BYTE*)pPacketString2, hashed_data_size, computed_digest);
		if (FAILED(hr)) throw(hr);
		
		// compare the signed hash and the computed hash
		if (StringUtils::StringToBinaryLength(hash)!=provided_digest.DIGEST_SIZE) throw(E_INVALIDARG);
		StringUtils::StringToBinary(hash, provided_digest.m_digest, provided_digest.DIGEST_SIZE);
		if (computed_digest!=provided_digest) throw(E_INVALIDARG);
		
		// verify the hash signature
		_variant_t vtSignature;
		VariantInit(&vtSignature);
		vtSignature.vt = VT_ARRAY | VT_UI1;
		vtSignature.parray = SafeArrayCreateVector(VT_UI1, 0, (DWORD)wcslen(signature)/2);
		BYTE *pSignatureData = 0;
		if (SUCCEEDED(hr = SafeArrayAccessData(vtSignature.parray, (void**)&pSignatureData)))
		{
			StringUtils::StringToBinary(signature, pSignatureData, (DWORD)wcslen(signature)/2);
			SafeArrayUnaccessData(vtSignature.parray);
		}
		hr = context.VerifySignature(computed_digest, &vtSignature, signature_verify_key);
		if (FAILED(hr)) throw(hr);

		*pBstrStream = SysAllocString(stream);
	}
	catch (HRESULT &ehr)
	{
		hr = ehr;
	}
	catch (...)
	{
		hr = E_FAIL;
	}
	if (pPacketString1) 
		delete [] pPacketString1;
	if (pPacketString2) 
		delete [] pPacketString2;

	return hr;
}

//_bstr_t CryptoAndFlateHelper::BinaryToString(BYTE *pData, DWORD length)
//{
//	size_t sizeOfStr = (length*2)+1;
//	wchar_t *pStr = new wchar_t[sizeOfStr];
//	memset(pStr,0,sizeOfStr);
//	wchar_t wTmpBuf[3];
//	for (unsigned int b = 0; b < length; ++b)
//	{
//		_snwprintf_s(wTmpBuf, sizeof(wTmpBuf)/sizeof(wchar_t), L"%02x", (unsigned int)pData[b]);
//		wTmpBuf[2] = 0;
//		wcscat_s(pStr, sizeOfStr, wTmpBuf);
//
//		//this line caused memory corruption that would be displayed on subsquint calls.
//		//swprintf_s(pStr+2*b, sizeOfStr,L"%02x",(unsigned int)pData[b]);
//	}
//	return pStr;
//}
