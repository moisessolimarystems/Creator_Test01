#ifndef _Challenge_Response_Helper_251D1FAF43FE48e5B7D149AF5F691419_h__
#define _Challenge_Response_Helper_251D1FAF43FE48e5B7D149AF5F691419_h__

#pragma once

#include "ChallengeResponse.h"
#include <comdef.h>

class ChallengeResponseHelper
{
public:
	// constructor, takes in the keys needed for authentication
	ChallengeResponseHelper(const BYTE* server_auth_this_private_key_buffer, const DWORD server_auth_this_private_key_buffer_size, const BYTE* this_auth_server_public_key_buffer, const DWORD this_auth_server_public_key_buffer_size);
	~ChallengeResponseHelper();
	
	//
	// Server authenticates client
	//

	// Called by clients to prove identity to a server that supports IObjectAuthentication
	HRESULT AuthenticateToServer(IDispatch *pServer);
	// Called by the server to help determine whether the client has successfully authenticated
	HRESULT GetChallengeInternal(VARIANT *pvtChallenge);
	// Called by the server to help determine whether the client has successfully authenticated
	HRESULT PutResponseInternal(VARIANT vtResponse);
	// Called by the server to determine if the client has successfully authenticated
	bool ChallengePassedByClient();

	//
	// Client authenticates server
	// 

	// Called by clients to prove the identity of a server
	HRESULT AuthenticateServer(IDispatch *pServer);
		
private:
	const BYTE* m_server_auth_this_private_key_buffer;
	const DWORD m_server_auth_this_private_key_buffer_size;
	const BYTE* m_this_auth_server_public_key_buffer;
	const DWORD m_this_auth_server_public_key_buffer_size;
	bool m_challenge_passed, m_challenge_issued;
	
	ChallengeResponse::Message m_challenge;
};

#endif