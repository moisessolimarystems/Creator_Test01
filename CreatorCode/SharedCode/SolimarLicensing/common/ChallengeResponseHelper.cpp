
#include "ChallengeResponseHelper.h"
#include "..\common\IObjectAuthentication.h"														// CR.32662; modified.
#include "..\common\COMUtils.h"
#include <atlbase.h>																						// CR.32662.V01; added.


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
	if (!m_challenge_issued) return E_FAIL;
	
	HRESULT hr = S_OK;
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
	return AuthenticateToServer(pServer, NULL);
}

/* AuthenticateToServer() - Used by clients. Satisfies the server's client 
 * authentication requirements by requesting and responding to the server's 
 * challenge.
 *
 * SLB 21.mar.2024 CR.32662.V01; refactored code, no functionality changes intended.
 * Now wraps VARIANT objects in ATL wrapper CComVARIANT, to manage smart clean-up.
 * SLB 21.mar.2024 CR.32662.V01; tested/verified.
 -----------------------------------------------------------------------------------*/
HRESULT ChallengeResponseHelper::AuthenticateToServer(
	IDispatch *pServer, 
	COAUTHINFO* pAuthInfo)
{
	//OutputDebugStringW(L"CR.32662.V01 AuthenticateToServer1() used/tested.");

	// Declare outside of try {} so they can be cleaned up in catch () {}.
	IObjectAuthentication *pAuthentication(NULL);
	try
	{
		HRESULT hr;
		hr = pServer->QueryInterface(__uuidof(IObjectAuthentication), (void**)&pAuthentication);
		if (FAILED(hr)) throw hr;

		hr = COMUtils::SetProxyBlanket(pAuthentication, pAuthInfo);
		if (FAILED(hr)) throw hr;

		// get the challenge from the server
		CComVariant vtChallenge;
		hr = pAuthentication->GetChallenge(&vtChallenge);
		if (FAILED(hr)) throw hr;
		
		// generate the response
		ChallengeResponse CR(m_server_auth_this_private_key_buffer, m_server_auth_this_private_key_buffer_size);
		ChallengeResponse::Message challenge(vtChallenge), response;
		hr = CR.GenerateResponse(challenge, response);
		if (FAILED(hr)) throw hr;
		
		// send the response back to the server
		CComVariant vtResponse((VARIANT)response);
		hr = pAuthentication->PutResponse(vtResponse);
		if (FAILED(hr)) throw hr;

		if (pAuthentication) pAuthentication->Release();
	}
	catch (HRESULT &e)
	{
		if (pAuthentication) pAuthentication->Release();

		return e;
	}
	
	return S_OK;
}

HRESULT ChallengeResponseHelper::AuthenticateServer(IDispatch *pServer)
{
	return AuthenticateServer(pServer, NULL);
}

/* AuthenticateServer() -  Called by clients to prove the identity of a server
 *
 * SLB 21.mar.2024 CR.32662.V01; refactored code, no functionality changes intended.
 * Now wraps VARIANT objects in ATL wrapper CComVARIANT, to manage smart clean-up.
 * SLB 21.mar.2024 CR.32662.V01; tested/verified.
 -----------------------------------------------------------------------------------*/
HRESULT ChallengeResponseHelper::AuthenticateServer(
	IDispatch *pServer, 
	COAUTHINFO* pAuthInfo)
{
	//OutputDebugStringW(L"CR.32662.V01 AuthenticateServer1() used/tested.");

	// Declare outside of try {} so they can be cleaned up in catch () {}.
	IObjectAuthentication *pAuthentication(NULL);
	try
	{
		HRESULT hr = S_OK;
		hr = pServer->QueryInterface(__uuidof(IObjectAuthentication), (void**)&pAuthentication);
		if (FAILED(hr)) throw hr;

		hr = COMUtils::SetProxyBlanket(pAuthentication, pAuthInfo);
		if (FAILED(hr)) throw hr;

		// First generate the challenge message.
		ChallengeResponse::Message challenge;
		ChallengeResponse CR(m_this_auth_server_public_key_buffer, m_this_auth_server_public_key_buffer_size);
		hr = CR.GenerateChallenge(challenge);
		if (FAILED(hr)) throw hr;
		
		// Challenge the server to get its response.
		CComVariant vtChallenge((VARIANT)challenge);												// CR.32662.V01; added.
		CComVariant vtResponse;																			// CR.32662.V01; added.
		hr = pAuthentication->Challenge(vtChallenge, &vtResponse);
		if (FAILED(hr)) throw hr;
		
		// Convert the response into a message object and verify it.
		ChallengeResponse::Message response = ChallengeResponse::Message(vtResponse);
		hr = CR.VerifyResponse(challenge, response);
		if (FAILED(hr)) throw hr;

		pAuthentication->Release();
	}
	catch (HRESULT &e)
	{
		if (pAuthentication) pAuthentication->Release();

		return e;
	}
	
	return S_OK;
}

