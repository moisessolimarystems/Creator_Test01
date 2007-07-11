#ifndef _WIN32_DCOM
#define _WIN32_DCOM
#endif 

#include "ChallengeResponse.h"

//
// ChallengeResponse::Message class
//
		
ChallengeResponse::Message::Message() : data_size(MESSAGE_BUFFER_SIZE)
{
	// generate random data to use as a random challenge message
	memset(data,0,MESSAGE_BUFFER_SIZE);
	CryptoHelper crypt;
	crypt.GenerateRandomBytes(data, MESSAGE_BUFFER_SIZE);
}
		
// duplicate the Message o
ChallengeResponse::Message::Message(const Message& o) : data_size(o.data_size)
{
	memset(data,0,MESSAGE_BUFFER_SIZE);
	memcpy(data,o.data,MESSAGE_BUFFER_SIZE);
}

// reconstruct the message from a VARIANT SafeArray
ChallengeResponse::Message::Message(const VARIANT &vtMessage)
{
	// if the variant contains a safe array that matches the expected form of a Message, copy it
	if (vtMessage.vt==(VT_ARRAY | VT_UI1) && vtMessage.parray!=0 && vtMessage.parray->rgsabound[0].cElements==(sizeof(data_size)+MESSAGE_BUFFER_SIZE))
	{
		data_size = 0;
		memset(data,0,MESSAGE_BUFFER_SIZE);
		
		BYTE *pSAData=0;
		if (SUCCEEDED(SafeArrayAccessData(vtMessage.parray, (void**)&pSAData)))
		{
			data_size = ntohl(*((DWORD*)pSAData));
			pSAData+=sizeof(data_size);
			memcpy(data,pSAData,MESSAGE_BUFFER_SIZE);
			SafeArrayUnaccessData(vtMessage.parray);
		}
	}
}

ChallengeResponse::Message::~Message()
{
	;
}

bool ChallengeResponse::Message::operator==(const Message& o)
{
	// they must be the same size
	if (data_size!=o.data_size)
		return false;
	
	// they must contain the same bytes
	return (0==memcmp(data,o.data,data_size));
}

bool ChallengeResponse::Message::operator!=(const Message& o)
{
	return !(*this==o);
}

ChallengeResponse::Message::operator VARIANT()
{
	HRESULT hr;
	VARIANT v;
	BYTE* pData=0;
	
	VariantInit(&v);
	SAFEARRAY *pSA = SafeArrayCreateVector(VT_UI1, 0, sizeof(data_size) + MESSAGE_BUFFER_SIZE);
	hr = SafeArrayAccessData(pSA, (void**)&pData);
	if (SUCCEEDED(hr))
	{
		// Write the data_size member
        DWORD network_data_size = htonl(data_size);
		memcpy(pData, &network_data_size, sizeof(data_size));
		pData+=sizeof(data_size);
		
		// Write the data member
		memcpy(pData, data, MESSAGE_BUFFER_SIZE);
		pData=0;
		
		SafeArrayUnaccessData(pSA);
		
		v.vt = VT_ARRAY | VT_UI1;
		v.parray = pSA;
	}
	else
	{
		v.vt = VT_NULL;
	}
	
	return v;
}

DWORD ChallengeResponse::Message::size()
{
	return data_size;
}

bool ChallengeResponse::Message::isValid()
{
	if (data_size==0)
		return false;
	
	bool all_zeros=true;
	// scan the data for non-zeros
	for (unsigned int i=0; i<data_size; ++i)
	{
		if (data[i]!=0)
		{
			all_zeros=false;
			break;
		}
	}
	return !all_zeros;
}

HRESULT ChallengeResponse::Message::Sign(CryptoHelper &context, const CryptoHelper::Key &key)
{
	HRESULT hr = S_OK;
	
	// compute a hash of the message
	CryptoHelper::Digest digest;
	hr = context.HashData(data, data_size, digest);
	if (FAILED(hr)) return hr;
	
	// sign the hash
	VARIANT vtSignature;
	VariantInit(&vtSignature);
	hr = context.SignHash(digest, &vtSignature);
	if (FAILED(hr)) return hr;
	
	// copy the signature to this message object
	BYTE *pSignature = 0;
	if (SUCCEEDED(hr = SafeArrayAccessData(vtSignature.parray, (void**)&pSignature)))
	{
		if (vtSignature.parray->rgsabound[0].cElements>MESSAGE_BUFFER_SIZE) return E_INVALIDARG;
		data_size = vtSignature.parray->rgsabound[0].cElements;
		memcpy(data,pSignature,data_size);
		SafeArrayUnaccessData(vtSignature.parray);
	}
	
	VariantClear(&vtSignature);
	
	return hr;
}

HRESULT ChallengeResponse::Message::VerifySignature(CryptoHelper &context, const CryptoHelper::Key &key, const ChallengeResponse::Message &signature)
{
	HRESULT hr = S_OK;
	
	// compute a hash of the message
	CryptoHelper::Digest digest;
	hr = context.HashData(data, data_size, digest);
	if (FAILED(hr)) return hr;
	
	// verify the signature against the hash
	hr = context.VerifySignature(digest, signature.data, signature.data_size, key);
	
	return hr;
}	




//
// ChallengeResponse class
//


ChallengeResponse::ChallengeResponse(std::wstring &key_path) : m_crypt(), m_key(m_crypt, key_path)
{
	;
}
	
ChallengeResponse::ChallengeResponse(const BYTE *pKeyBuffer, const DWORD key_buffer_size) : m_crypt(), m_key(m_crypt, pKeyBuffer, key_buffer_size)
{
	;
}

ChallengeResponse::~ChallengeResponse()
{
	;
}


HRESULT ChallengeResponse::GenerateChallenge(/*[out]*/ Message& challenge)
{
	HRESULT hr = S_OK;
	
	// Generate a message (this is a hash for the responder to sign)
	challenge = Message();
	
	return hr;
}

HRESULT ChallengeResponse::GenerateResponse(/*[in]*/ const Message& challenge, /*[out]*/ Message& response)
{
	HRESULT hr = S_OK;

	// sign the challenge, the challenger will verify the signature
	response = challenge;
	hr = response.Sign(m_crypt, m_key);
	
	return hr;
}

HRESULT ChallengeResponse::VerifyResponse(/*[in]*/ Message& challenge, /*[in]*/ Message& response)
{
	HRESULT hr = S_OK;
	
	hr = challenge.VerifySignature(m_crypt, m_key, response);
	
	return hr;
}
