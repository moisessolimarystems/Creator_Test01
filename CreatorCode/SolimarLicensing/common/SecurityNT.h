#ifndef __SECURITYNT_H
#define __SECURITYNT_H

#define SECURITY_WIN32
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <sspi.h>
#include <lm.h>
#include <Dsgetdc.h>
#include <comdef.h>

#include "SecurityHelper.h"

class SecurityDll {
public:
	SecurityDll();
	~SecurityDll();

	BOOL IsLoaded() { return m_bLoaded; }

	ACCEPT_SECURITY_CONTEXT_FN			_AcceptSecurityContext;
	ACQUIRE_CREDENTIALS_HANDLE_FN_W		_AcquireCredentialsHandle;
	COMPLETE_AUTH_TOKEN_FN				_CompleteAuthToken;
	DELETE_SECURITY_CONTEXT_FN			_DeleteSecurityContext;
	FREE_CONTEXT_BUFFER_FN				_FreeContextBuffer;
	FREE_CREDENTIALS_HANDLE_FN			_FreeCredentialsHandle;
	INITIALIZE_SECURITY_CONTEXT_FN_W	_InitializeSecurityContext;
	QUERY_SECURITY_PACKAGE_INFO_FN_W	_QuerySecurityPackageInfo;

private:
	HMODULE m_hModule;
	BOOL	m_bLoaded;
	BOOL	MapFuncs();
};

extern SecurityDll g_securityDll;

class SSPLogonUser {
public:
	typedef struct _AUTH_SEQ {
		BOOL fInitialized;
		BOOL fHaveCredHandle;
		BOOL fHaveCtxtHandle;
		CredHandle hcred;
		struct _SecHandle hctxt;
	} AUTH_SEQ, *PAUTH_SEQ;
	
	SSPLogonUser();
	~SSPLogonUser();

	BOOL Logon(WCHAR* pwcDomain, WCHAR* pwcUsername, WCHAR* pwcPassword);
	PCtxtHandle GetContextHandle();
	BOOL GetSIDString(LPWSTR* ppwcSid);
private:	
	BOOL		m_bInitialized;
	BOOL		m_bAuthenticated;
	AUTH_SEQ	m_asServer;
	AUTH_SEQ	m_asClient;
	PVOID       m_pClientBuf;
	PVOID       m_pServerBuf;
	DWORD       m_cbMaxToken;
	WCHAR		m_curDomain[MAX_PATH * sizeof(WCHAR)];
	WCHAR		m_curUser[128];

	void Reset();
	BOOL GenClientContext(PAUTH_SEQ pAS, PSEC_WINNT_AUTH_IDENTITY_W pAuthIdentity,
		PVOID pIn, DWORD cbIn, PVOID pOut, PDWORD pcbOut, PBOOL pfDone);
	BOOL GenServerContext(PAUTH_SEQ pAS, PVOID pIn, DWORD cbIn, PVOID pOut,
		PDWORD pcbOut, PBOOL pfDone);
};

class ImpersonateUser {
public:
	ImpersonateUser(); 
	~ImpersonateUser();
	BOOL Impersonate(WCHAR* pwcDomain, WCHAR* pwcUsername, WCHAR* pwcPassword);
	void RevertToSelf();
private:
	BOOL m_bImpersonating;
	SSPLogonUser m_user;	
};

#endif
