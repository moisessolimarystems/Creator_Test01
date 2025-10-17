#include "SecurityNT.h"

//
// Older versions of WinError.h does not have SEC_I_COMPLETE_NEEDED #define.
// So, in such SDK environment setup, we will include issperr.h which has the
// definition for SEC_I_COMPLETE_NEEDED. Include issperr.h only if
// SEC_I_COMPLETE_NEEDED is not defined.
//

#ifndef SEC_I_COMPLETE_NEEDED
#include <issperr.h>
#endif

static SecurityDll g_securityDll;

//
// SecurityDll()
//
//*******************************************************************
SecurityDll::SecurityDll() : 
	m_bLoaded(FALSE),
	m_hModule(NULL),
	_AcceptSecurityContext(NULL),
	_AcquireCredentialsHandle(NULL),
	_CompleteAuthToken(NULL),
	_DeleteSecurityContext(NULL),
	_FreeContextBuffer(NULL),
	_FreeCredentialsHandle(NULL),
	_InitializeSecurityContext(NULL),
	_QuerySecurityPackageInfo(NULL)
{
	TCHAR			dllName[MAX_PATH];
	OSVERSIONINFO	VerInfo;

	// 
	//  Find out which security DLL to use, depending on
	//  whether we are on NT or Win95 or 2000 or XP or Windows Server 2003
	//  We have to use security.dll on Windows NT 4.0.
	//  All other operating systems, we have to use Secur32.dll
	// 

	VerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if (!GetVersionEx(&VerInfo))   
		return;

	if (VerInfo.dwPlatformId == VER_PLATFORM_WIN32_NT &&
		VerInfo.dwMajorVersion == 4 &&
		VerInfo.dwMinorVersion == 0)
		_tcscpy(dllName, _T("security.dll"));
	else
		_tcscpy(dllName, _T("secur32.dll"));

	m_hModule = LoadLibrary(dllName);
	if (m_hModule && MapFuncs()) 
		m_bLoaded = TRUE;

	if (!m_bLoaded) {
		if (m_hModule) {
			FreeLibrary(m_hModule);
			m_hModule = NULL;
		}
		_AcceptSecurityContext		= NULL;
		_AcquireCredentialsHandle	= NULL;
		_CompleteAuthToken			= NULL;
		_DeleteSecurityContext		= NULL;
		_FreeContextBuffer			= NULL;
		_FreeCredentialsHandle		= NULL;
		_InitializeSecurityContext	= NULL;
		_QuerySecurityPackageInfo	= NULL;
	}
}

//
// ~SecurityDll()
//
//*******************************************************************
SecurityDll::~SecurityDll()
{
	if (m_hModule) 
		FreeLibrary(m_hModule);
}

//
// MapFuncs()
//
//*******************************************************************
BOOL SecurityDll::MapFuncs()
{
	_AcceptSecurityContext = (ACCEPT_SECURITY_CONTEXT_FN) 
		GetProcAddress(m_hModule, "AcceptSecurityContext");
	if (!_AcceptSecurityContext)
		return FALSE;

	_AcquireCredentialsHandle = (ACQUIRE_CREDENTIALS_HANDLE_FN_W)
		GetProcAddress(m_hModule, "AcquireCredentialsHandleW");
	if (!_AcquireCredentialsHandle)
		return FALSE;

	//
	// CompleteAuthToken is not present on Windows 9x Secur32.dll
	// Do not check for the availablity of the function if it is NULL;
	//

	_CompleteAuthToken = (COMPLETE_AUTH_TOKEN_FN) 
		GetProcAddress(m_hModule, "CompleteAuthToken");

	_DeleteSecurityContext = (DELETE_SECURITY_CONTEXT_FN) 
		GetProcAddress(m_hModule, "DeleteSecurityContext");
	if (!_DeleteSecurityContext)
		return FALSE;

	_FreeContextBuffer = (FREE_CONTEXT_BUFFER_FN) 
		GetProcAddress(m_hModule, "FreeContextBuffer");
	if (!_FreeContextBuffer)
		return FALSE;

	_FreeCredentialsHandle = (FREE_CREDENTIALS_HANDLE_FN) 
		GetProcAddress(m_hModule, "FreeCredentialsHandle");
	if (!_FreeCredentialsHandle)
		return FALSE;

	_InitializeSecurityContext = (INITIALIZE_SECURITY_CONTEXT_FN_W) 
		GetProcAddress(m_hModule, "InitializeSecurityContextW");
	if (!_InitializeSecurityContext)
		return FALSE;

	_QuerySecurityPackageInfo = (QUERY_SECURITY_PACKAGE_INFO_FN_W)
		GetProcAddress(m_hModule, "QuerySecurityPackageInfoW");
	if (!_QuerySecurityPackageInfo)
		return FALSE;

	return TRUE;
}

//
// SSPLogonUser()
//
//*******************************************************************
SSPLogonUser::SSPLogonUser() :
	m_pClientBuf(NULL),
	m_pServerBuf(NULL),
	m_bInitialized(FALSE),
	m_bAuthenticated(FALSE),
	m_cbMaxToken(0)
{
	if (!g_securityDll.IsLoaded())
		return;

	ZeroMemory(&m_asServer, sizeof(m_asServer));
	ZeroMemory(&m_asClient, sizeof(m_asClient));
	
	//
	// Get max token size.
	//

	PSecPkgInfoW pSPI = NULL;
    g_securityDll._QuerySecurityPackageInfo(L"NTLM", &pSPI);
    m_cbMaxToken = pSPI->cbMaxToken;
    g_securityDll._FreeContextBuffer(pSPI);

	//
	// Allocate buffers for client and server messages.
	//

	m_pClientBuf = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, m_cbMaxToken);
	m_pServerBuf = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, m_cbMaxToken);

	//
	// Prepare class for new logon.
	//
	
	Reset();

	m_bInitialized = TRUE;
}

//
// ~SSPLogonUser()
//
//*******************************************************************
SSPLogonUser::~SSPLogonUser()
{
	Reset();
}

//
// Reset()
//
// Prepares the class instance for a new logon.
//
//*******************************************************************
void SSPLogonUser::Reset()
{
	m_bAuthenticated = FALSE;

	if (m_asClient.fHaveCtxtHandle)
		g_securityDll._DeleteSecurityContext(&m_asClient.hctxt);

	if (m_asClient.fHaveCredHandle)
		g_securityDll._FreeCredentialsHandle(&m_asClient.hcred);

	if (m_asServer.fHaveCtxtHandle)
		g_securityDll._DeleteSecurityContext(&m_asServer.hctxt);

	if (m_asServer.fHaveCredHandle)
		g_securityDll._FreeCredentialsHandle(&m_asServer.hcred);

	ZeroMemory(&m_asServer, sizeof(m_asServer));
	ZeroMemory(&m_asClient, sizeof(m_asClient));
}

//
// Logon()
//
//*******************************************************************
BOOL SSPLogonUser::Logon(
	WCHAR* pwcDomain, 
	WCHAR* pwcUsername, 
	WCHAR* pwcPassword)
{
	BOOL        fDone      = FALSE;
	BOOL        fResult    = FALSE;
	DWORD       cbOut      = 0;
	DWORD       cbIn       = 0;

	//
	// Make sure the security.dll is loaded, etc.
	//
	
	if (!m_bInitialized)
		return FALSE;

	if (m_bAuthenticated)
		Reset();

	//
	// Initialize auth identity structure.
	//

	SEC_WINNT_AUTH_IDENTITY_W ai;
	ZeroMemory(&ai, sizeof(ai));
	ai.Domain			= (unsigned short*)pwcDomain; // Cast so it compiles under Builder.
	ai.DomainLength		= pwcDomain ? wcslen(pwcDomain) : 0;
	ai.User				= (unsigned short*)pwcUsername; // Cast so it compiles under Builder.
	ai.UserLength		= pwcUsername ? wcslen(pwcUsername) : 0;
	ai.Password			= (unsigned short*)pwcPassword; // Cast so it compiles under Builder.
	ai.PasswordLength	= pwcPassword ? wcslen(pwcPassword) : 0;
	ai.Flags			= SEC_WINNT_AUTH_IDENTITY_UNICODE;

	//
	// Prepare client message (negotiate).
	//

	cbOut = m_cbMaxToken;
	if (GenClientContext(&m_asClient, &ai, NULL, 0, m_pClientBuf, &cbOut, &fDone)) {

		//
		// Prepare server message (challenge).
		//

		cbIn = cbOut;
		cbOut = m_cbMaxToken;
		if (GenServerContext(&m_asServer, m_pClientBuf, cbIn, m_pServerBuf, &cbOut, &fDone)) {

			// Most likely failure: AcceptServerContext fails with SEC_E_LOGON_DENIED
			// in the case of bad szUser or szPassword.
			// Unexpected Result: Logon will succeed if you pass in a bad szUser and 
			// the guest account is enabled in the specified domain.

			//
			// Prepare client message (authenticate).
			//

			cbIn = cbOut;
			cbOut = m_cbMaxToken;
			if (GenClientContext(&m_asClient, &ai, m_pServerBuf, cbIn, m_pClientBuf, &cbOut, &fDone)) {

				//
				// Prepare server message (authentication).
				//

				cbIn = cbOut;
				cbOut = m_cbMaxToken;
				if (GenServerContext(&m_asServer, m_pClientBuf, cbIn, m_pServerBuf, &cbOut, &fDone)) {
					wcscpy(m_curUser, pwcUsername);
					if (pwcDomain)
						wcscpy(m_curDomain, pwcDomain);
					m_bAuthenticated = TRUE;
				}
			}
		}
	}

	return m_bAuthenticated;
}

//
// GetSIDStr()
//
// Caller is responsible for deleting returned buffer if 
// successful. 
//
//*******************************************************************
BOOL SSPLogonUser::GetSIDString(LPWSTR* ppwcSid)
{
	BOOL retVal = FALSE;
	
	if (!m_bAuthenticated)
		return FALSE;
	
	PDOMAIN_CONTROLLER_INFOW pDCInfo;
	if (DsGetDcNameW(
							NULL,
							m_curDomain,
							NULL,
							NULL,
							0,
							&pDCInfo) == ERROR_SUCCESS)
	{
		BYTE* pSidBuf = SecurityHelper::GetSid(m_curUser, pDCInfo->DomainControllerName);
		if (pSidBuf) {
			DWORD dwBufLen = 0;
			SecurityHelper::SidToString(reinterpret_cast<PSID>(pSidBuf), NULL, &dwBufLen);
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
				*ppwcSid = new WCHAR[dwBufLen];
				ZeroMemory(*ppwcSid, dwBufLen);
				if (SecurityHelper::SidToString(reinterpret_cast<PSID>(pSidBuf), *ppwcSid, &dwBufLen))
					retVal = TRUE;
				else {
					delete *ppwcSid;
					*ppwcSid = NULL;
				}
			}
			delete pSidBuf;
		}
		NetApiBufferFree(pDCInfo);
	}

	return retVal;
}

//
// GetContextHandle()
//
// If the user has been successfully authenticated, returns the
// users security context handle.
//
//*******************************************************************
PCtxtHandle SSPLogonUser::GetContextHandle()
{ 
	if (m_bAuthenticated)
		return &m_asServer.hctxt;
	else
		return NULL;
}

//
// GenClientContext()
//
// Optionally takes an input buffer coming from the server and returns
// a buffer of information to send back to the server.  Also returns
// an indication of whether or not the context is complete.
//
//*******************************************************************
BOOL SSPLogonUser::GenClientContext(
	PAUTH_SEQ pAS, 
	PSEC_WINNT_AUTH_IDENTITY_W pAuthIdentity,
	PVOID	pIn, 
	DWORD	cbIn, 
	PVOID	pOut, 
	PDWORD	pcbOut, 
	PBOOL	pfDone) 
{
	SECURITY_STATUS ss;
	TimeStamp       tsExpiry;
	SecBufferDesc   sbdOut;
	SecBuffer       sbOut;
	SecBufferDesc   sbdIn;
	SecBuffer       sbIn;
	ULONG           fContextAttr;

	if (!pAS->fInitialized) {      
		ss = g_securityDll._AcquireCredentialsHandle(
				NULL, 
				L"NTLM", 
				SECPKG_CRED_OUTBOUND, 
				NULL, 
				pAuthIdentity, 
				NULL, 
				NULL,
				&pAS->hcred, 
				&tsExpiry);
		if (ss < 0) 
			return FALSE;
		pAS->fHaveCredHandle = TRUE;
	}

	//
	// Prepare output buffer.
	//

	sbdOut.ulVersion	= 0;
	sbdOut.cBuffers		= 1;
	sbdOut.pBuffers		= &sbOut;

	sbOut.cbBuffer		= *pcbOut;
	sbOut.BufferType	= SECBUFFER_TOKEN;
	sbOut.pvBuffer		= pOut;

	//
	// Prepare input buffer.
	//

	if (pAS->fInitialized)  {
		sbdIn.ulVersion	= 0;
		sbdIn.cBuffers	= 1;
		sbdIn.pBuffers	= &sbIn;

		sbIn.cbBuffer	= cbIn;
		sbIn.BufferType	= SECBUFFER_TOKEN;
		sbIn.pvBuffer	= pIn;
	}

	ss = g_securityDll._InitializeSecurityContext(
			&pAS->hcred, 
			pAS->fInitialized ? &pAS->hctxt : NULL, 
			NULL, 
			0, 
			0, 
			SECURITY_NATIVE_DREP, 
			pAS->fInitialized ? &sbdIn : NULL,
			0, 
			&pAS->hctxt, 
			&sbdOut, 
			&fContextAttr, 
			&tsExpiry);
	if (ss < 0)
		return FALSE;

	pAS->fHaveCtxtHandle = TRUE;

	//
	// If necessary, complete token.
	//

	if (ss == SEC_I_COMPLETE_NEEDED || ss == SEC_I_COMPLETE_AND_CONTINUE) {
		if (g_securityDll._CompleteAuthToken) {
			ss = g_securityDll._CompleteAuthToken(&pAS->hctxt, &sbdOut);
			if (ss < 0)  
				return FALSE;
		}
		else 
			return FALSE;
	}
	*pcbOut = sbOut.cbBuffer;

	if (!pAS->fInitialized) 
		pAS->fInitialized = TRUE;
   
	*pfDone = !(ss == SEC_I_CONTINUE_NEEDED || ss == SEC_I_COMPLETE_AND_CONTINUE);

	return TRUE;
}

//
// GenServerContext()
//
// Takes an input buffer coming from the client and returns a buffer
// to be sent to the client.  Also returns an indication of whether or
// not the context is complete.
// 
//*******************************************************************
BOOL SSPLogonUser::GenServerContext(
	PAUTH_SEQ	pAS, 
	PVOID		pIn, 
	DWORD		cbIn, 
	PVOID		pOut,
	PDWORD		pcbOut, 
	PBOOL		pfDone) 
{
	SECURITY_STATUS ss;
	TimeStamp       tsExpiry;
	SecBufferDesc   sbdOut;
	SecBuffer       sbOut;
	SecBufferDesc   sbdIn;
	SecBuffer       sbIn;
	ULONG           fContextAttr;

	if (!pAS->fInitialized) {
		ss = g_securityDll._AcquireCredentialsHandle(
			NULL, 
			L"NTLM", 
            SECPKG_CRED_INBOUND, 
			NULL, 
			NULL, 
			NULL, 
			NULL, 
			&pAS->hcred, 
            &tsExpiry);
		if (ss < 0) 
			return FALSE;
		pAS->fHaveCredHandle = TRUE;
	}

	//
	// Prepare output buffer.
	//
   
	sbdOut.ulVersion	= 0;
	sbdOut.cBuffers		= 1;
	sbdOut.pBuffers		= &sbOut;

	sbOut.cbBuffer		= *pcbOut;
	sbOut.BufferType	= SECBUFFER_TOKEN;
	sbOut.pvBuffer		= pOut;

	//
	// Prepare input buffer.
	//

	sbdIn.ulVersion	= 0;
	sbdIn.cBuffers	= 1;
	sbdIn.pBuffers	= &sbIn;

	sbIn.cbBuffer	= cbIn;
	sbIn.BufferType	= SECBUFFER_TOKEN;
	sbIn.pvBuffer	= pIn;

	ss = g_securityDll._AcceptSecurityContext(
			&pAS->hcred, 
			pAS->fInitialized ? &pAS->hctxt : NULL,
			&sbdIn, 
			0, 
			SECURITY_NATIVE_DREP, 
			&pAS->hctxt, 
			&sbdOut, 
			&fContextAttr, 
			&tsExpiry);
	if (ss < 0)
		return FALSE;

	pAS->fHaveCtxtHandle = TRUE;

	//
	// If necessary, complete token.
	//

	if (ss == SEC_I_COMPLETE_NEEDED || ss == SEC_I_COMPLETE_AND_CONTINUE) {      
		if (g_securityDll._CompleteAuthToken) {
			ss = g_securityDll._CompleteAuthToken(&pAS->hctxt, &sbdOut);
			if (ss < 0)  
				return FALSE;
		}
		else 
			return FALSE;
	}

	*pcbOut = sbOut.cbBuffer;

	if (!pAS->fInitialized)
		pAS->fInitialized = TRUE;

	*pfDone = !(ss = SEC_I_CONTINUE_NEEDED || ss == SEC_I_COMPLETE_AND_CONTINUE);

	return TRUE;
}

//
// ImpersonateUser()
// 
//*******************************************************************
ImpersonateUser::ImpersonateUser() : m_bImpersonating(FALSE)
{
}

//
// ~ImpersonateUser()
// 
//*******************************************************************
ImpersonateUser::~ImpersonateUser() 
{ 
	RevertToSelf(); 
}

//
// Impersonate()
// 
//*******************************************************************
BOOL ImpersonateUser::Impersonate(
	WCHAR* pwcDomain, 
	WCHAR* pwcUsername, 
	WCHAR* pwcPassword)
{
	//
	// If already impersonating, revert, then impersonate new user.
	//
	
	if (m_bImpersonating)
		RevertToSelf();

	//
	// Logon the user. This will establish a security context 
	// with which we can impersonate.
	//
	
	if (!m_user.Logon(pwcDomain, pwcUsername, pwcPassword)) 
		return FALSE;

	if (ImpersonateSecurityContext(m_user.GetContextHandle()) == SEC_E_OK)
		m_bImpersonating = TRUE;

	return m_bImpersonating;
}

//
// RevertToSelf()
// 
//*******************************************************************
void ImpersonateUser::RevertToSelf()
{
	if (m_bImpersonating)
		RevertSecurityContext(m_user.GetContextHandle());
	m_bImpersonating = FALSE;
}

