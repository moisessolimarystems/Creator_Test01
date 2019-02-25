
#include "ChallengeResponseHelper.h"
#include "..\common\_IObjectAuthentication.h"

ChallengeResponseHelper::ChallengeResponseHelper(const BYTE* server_auth_this_private_key_buffer, const DWORD server_auth_this_private_key_buffer_size, const BYTE* this_auth_server_public_key_buffer, const DWORD this_auth_server_public_key_buffer_size) : 
	m_server_auth_this_private_key_buffer(server_auth_this_private_key_buffer),
	m_server_auth_this_private_key_buffer_size(server_auth_this_private_key_buffer_size),
	m_this_auth_server_public_key_buffer(this_auth_server_public_key_buffer),
	m_this_auth_server_public_key_buffer_size(this_auth_server_public_key_buffer_size),
	m_challenge_passed(false),
	m_challenge_issued(false)
{
	;
}

ChallengeResponseHelper::~ChallengeResponseHelper()
{
	;
}

HRESULT ChallengeResponseHelper::GetChallengeInternal(VARIANT *pvtChallenge)
{
	HRESULT hr = S_OK;
	
	ChallengeResponse CR(m_this_auth_server_public_key_buffer, m_this_auth_server_public_key_buffer_size);
	
	hr = CR.GenerateChallenge(m_challenge);
	
	if (SUCCEEDED(hr))
	{
		*pvtChallenge = m_challenge;
		m_challenge_issued = true;
		m_challenge_passed = false;
	}
	
	return hr;
}

HRESULT ChallengeResponseHelper::PutResponseInternal(VARIANT vtResponse)
{
	HRESULT hr = S_OK;

	if (!m_challenge_issued)
		return E_FAIL;
	
	ChallengeResponse CR(m_this_auth_server_public_key_buffer, m_this_auth_server_public_key_buffer_size);
	
	hr = CR.VerifyResponse(m_challenge, ChallengeResponse::Message(vtResponse));
	if (FAILED(hr)) return hr;
	
	m_challenge_passed = true;
	return S_OK;
}

bool ChallengeResponseHelper::ChallengePassedByClient()
{
	return m_challenge_passed;
}

// Used by clients. Satisfies the server's client authentication requirements 
// by requesting and responding to the server's challenge.
HRESULT ChallengeResponseHelper::AuthenticateToServer(IDispatch *pServer)
{
	HRESULT hr = S_OK;
	IObjectAuthentication *pAuthentication = 0;
	
	hr = pServer->QueryInterface(__uuidof(IObjectAuthentication), (void**)&pAuthentication);
	if (FAILED(hr)) return hr;
	
	VARIANT vtChallenge, vtResponse;
	VariantInit(&vtChallenge);
	VariantInit(&vtResponse);
	
	try
	{
		// get the challenge from the server
		hr = pAuthentication->GetChallenge(&vtChallenge);
		if (FAILED(hr)) throw hr;
		
		// generate the response
		ChallengeResponse CR(m_server_auth_this_private_key_buffer, m_server_auth_this_private_key_buffer_size);
		ChallengeResponse::Message challenge(vtChallenge), response;
		
		hr = CR.GenerateResponse(challenge, response);
		if (FAILED(hr)) throw hr;
		vtResponse = (VARIANT)response;
		
		// send the response back to the server
		hr = pAuthentication->PutResponse(vtResponse);
		if (FAILED(hr)) throw hr;
	}
	catch(HRESULT &ehr)
	{
		return hr = ehr;
	}
	
	VariantClear(&vtChallenge);
	VariantClear(&vtResponse);
	
	pAuthentication->Release();
	
	return hr;
}


// Called by clients to prove the identity of a server
HRESULT ChallengeResponseHelper::AuthenticateServer(IDispatch *pServer)
{
	HRESULT hr = S_OK;
	IObjectAuthentication *pAuthentication = 0;
	
	hr = pServer->QueryInterface(__uuidof(IObjectAuthentication), (void**)&pAuthentication);
	if (FAILED(hr)) return hr;
	
	VARIANT vtChallenge, vtResponse;
	VariantInit(&vtChallenge);
	VariantInit(&vtResponse);
	
	try
	{
		ChallengeResponse::Message challenge, expected_response;
		ChallengeResponse CR(m_this_auth_server_public_key_buffer, m_this_auth_server_public_key_buffer_size);
		
		hr = CR.GenerateChallenge(challenge);
		if (FAILED(hr)) throw hr;
		
		vtChallenge = (VARIANT)challenge;
		
		hr = pAuthentication->Challenge(vtChallenge, &vtResponse);
		if (FAILED(hr)) throw hr;
		
		ChallengeResponse::Message response = ChallengeResponse::Message(vtResponse);
		
		if (FAILED(hr = CR.VerifyResponse(challenge, response)))
			throw hr;
	}
	catch(HRESULT &ehr)
	{
		return hr = ehr;
	}
	
	VariantClear(&vtChallenge);
	VariantClear(&vtResponse);
	
	pAuthentication->Release();
	
	return hr;
}

