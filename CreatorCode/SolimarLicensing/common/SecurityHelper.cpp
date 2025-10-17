#include "SecurityHelper.h"

/* Creates a buffer holding the security id of the given user.
 * pwcUserName  is the name of the user or group to look up, 
 *              eg.: Corp\Crosner or Crosner or Administrators
 * pwcMachine   is the optional machine to search for this username. Defaults 
 *              to NULL indicating the local machine and its domain controllers
 * Does not handle domains perfectly when the name is 
 * defined in multiple places 
 * (eg.: Administrators group might be returned from some 
 * arbitrary computer and not from the domain controller)
 * This object needs to be deleted with delete() when done.
 */
BYTE * SecurityHelper::GetSid(LPWSTR pwcUserName, LPWSTR pwcMachine)
{
    DWORD dwSidSize = 0, dwDomainSize = 0;
    SID_NAME_USE use;

    // first call is to get the size
    BOOL bRet = LookupAccountNameW(
                            pwcMachine,      // system name
                            pwcUserName,     // account name
                            NULL,            // security identifier
                            &dwSidSize,      // size of security identifier
                            NULL,            // domain name
                            &dwDomainSize,   // size of domain name
                            &use             // SID-type indicator
                              );    

    if(bRet == FALSE && ERROR_INSUFFICIENT_BUFFER != GetLastError())\
        return NULL;

    BYTE * buff = new BYTE[dwSidSize];

    if(buff == NULL)
        return NULL;

    WCHAR * pwcDomain = new WCHAR[dwDomainSize];

    if(pwcDomain == NULL)
    {
        delete [] buff;
        return NULL;
    }

    // Call to LookupAccountNameW actually gets the SID
    bRet = LookupAccountNameW(
                            pwcMachine,     // system name
                            pwcUserName,    // account name
                            buff,           // security identifier
                            &dwSidSize,     // size of security identifier
                            pwcDomain,      // domain name
                            &dwDomainSize,  // size of domain name
                            &use            // SID-type indicator
                              );    

    delete [] pwcDomain;

    if(bRet == FALSE)
    {
        delete [] buff;
        return NULL;
    }

    return buff;
}

/* Creates two buffers holding the account & domain for the given SID.
 * pSid			is the SID to loopup. 
 * pwcMachine   is the optional machine to search for this account. Defaults 
 *              to NULL indicating the local machine and its domain controllers
 * The returned buffers need to be deleted with delete() when done.
 * Returns FALSE if the account isn't found.
 */
BOOL SecurityHelper::GetSidAccount(PSID pSid, LPWSTR* ppwcAccountName, LPWSTR* ppwcDomain, LPWSTR pwcMachine)
{
    DWORD dwAccountNameSize = 0, dwDomainSize = 0;
    SID_NAME_USE use;

    // first call is to get the size
    BOOL bRet = LookupAccountSidW(
					pwcMachine,			// system name
                    pSid,				// PSID
                    NULL,				// account name 
                    &dwAccountNameSize, // size of account buffer
                    NULL,				// domain name 
                    &dwDomainSize,		// size of domain name
                    &use);				// SID-type indicator
    if (bRet == FALSE && ERROR_INSUFFICIENT_BUFFER != GetLastError())
        return NULL;

    *ppwcAccountName	= new WCHAR[dwAccountNameSize];
    *ppwcDomain			= new WCHAR[dwDomainSize];

    if(!*ppwcAccountName || !*ppwcDomain)
        return NULL;

    // Call to LookupAccountNameW actually gets the values.
    bRet = LookupAccountSidW(
				pwcMachine,			// system name
                pSid,				// PSID
                *ppwcAccountName,	// account name 
                &dwAccountNameSize,	// size of username buffer
                *ppwcDomain,		// domain name 
                &dwDomainSize,		// size of domain name
                &use);				// SID-type indicator

    if(bRet == FALSE)
    {
        delete[] ppwcAccountName;
        delete[] ppwcDomain;
        return FALSE;
    }

    return TRUE;
}

/* 
 * Converts an SID to string format 
 */
BOOL SecurityHelper::SidToString(PSID pSid, LPWSTR pwcSid, LPDWORD lpdwBufferLen)
{
    PSID_IDENTIFIER_AUTHORITY psia;

    DWORD dwSubAuthorities;
    DWORD dwSidRev=SID_REVISION;
    DWORD dwCounter;
    DWORD dwSidSize;

    // Validate the binary SID.

    if(!IsValidSid(pSid)) return FALSE;

    // Get the identifier authority value from the SID.

    psia = GetSidIdentifierAuthority(pSid);

    // Get the number of subauthorities in the SID.

    dwSubAuthorities = *GetSidSubAuthorityCount(pSid);

    // Compute the buffer length.
    // S-SID_REVISION- + IdentifierAuthority- + subauthorities- + NULL

    dwSidSize=(15 + 12 + (12 * dwSubAuthorities) + 1) * sizeof(WCHAR);

    // Check input buffer length.
    // If too small, indicate the proper size and set last error.

    if (*lpdwBufferLen < dwSidSize)
    {
        *lpdwBufferLen = dwSidSize;
        SetLastError(ERROR_INSUFFICIENT_BUFFER);
        return FALSE;
    }

    // Add 'S' prefix and revision number to the string.

    dwSidSize=wsprintfW(pwcSid, L"S-%lu-", dwSidRev );

    // Add SID identifier authority to the string.

    if ( (psia->Value[0] != 0) || (psia->Value[1] != 0) )
    {
        dwSidSize+=wsprintfW(pwcSid + wcslen(pwcSid),
                    L"0x%02hx%02hx%02hx%02hx%02hx%02hx",
                    (USHORT)psia->Value[0],
                    (USHORT)psia->Value[1],
                    (USHORT)psia->Value[2],
                    (USHORT)psia->Value[3],
                    (USHORT)psia->Value[4],
                    (USHORT)psia->Value[5]);
    }
    else
    {
        dwSidSize+=wsprintfW(pwcSid + wcslen(pwcSid),
                    L"%lu",
                    (ULONG)(psia->Value[5]      )   +
                    (ULONG)(psia->Value[4] <<  8)   +
                    (ULONG)(psia->Value[3] << 16)   +
                    (ULONG)(psia->Value[2] << 24)   );
    }

    // Add SID subauthorities to the string.
    //
    for (dwCounter=0 ; dwCounter < dwSubAuthorities ; dwCounter++)
    {
        dwSidSize+=wsprintfW(pwcSid + dwSidSize, L"-%lu",
                    *GetSidSubAuthority(pSid, dwCounter) );
    }

    return TRUE;
}


/*
 * Converts an SID to a and allocates a BSTR
 * Caller must call SysFreeString on the BSTR if it is not null.
 */
BOOL SecurityHelper::SidToBSTR(PSID pSid, BSTR *pSidString)
{
	DWORD buffersize=0;
	wchar_t *pBuffer=NULL;
	
	*pSidString=NULL;

	SecurityHelper::SidToString(pSid, NULL, &buffersize);
	
	if (buffersize>0)
	{
		pBuffer=new wchar_t[buffersize];
		BOOL retval = SecurityHelper::SidToString(pSid, pBuffer, &buffersize);
		if (retval)
			*pSidString=SysAllocString(pBuffer);
		delete [] pBuffer;
		
		return retval;
	}
	
	return FALSE;
}


/* 
 * Converts an SID string to an SID.
 */
BOOL SecurityHelper::SidStringToSid(LPWSTR pwcSid, PSID* ppSid)
{
    SID_IDENTIFIER_AUTHORITY identAuthority;
    WCHAR buffer[1024];
    int   i;

    LPWSTR ptr, ptr1;


    BYTE  nByteAuthorityCount = 0;
    DWORD dwSubAuthority[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    ZeroMemory(&identAuthority, sizeof(identAuthority));

    wcscpy_s(buffer, pwcSid);

    // S-SID_REVISION- + identifierauthority- + subauthorities- + NULL

    // Skip S
    if (!(ptr = wcschr(buffer, L'-')))
    {
        return FALSE;
    }

    // Skip -
    ptr++;

    // Skip SID_REVISION
    if (!(ptr = wcschr(ptr, L'-')))
    {
        return FALSE;
    }

    // Skip -
    ptr++;

    // Skip identifierauthority
    if (!(ptr1 = wcschr(ptr, L'-')))
    {
        return FALSE;
    }
    *ptr1= 0;

    if ((*ptr == '0') && (*(ptr+1) == 'x'))
    {
        swscanf_s(ptr, L"0x%02hx%02hx%02hx%02hx%02hx%02hx",
            &identAuthority.Value[0],
            &identAuthority.Value[1],
            &identAuthority.Value[2],
            &identAuthority.Value[3],
            &identAuthority.Value[4],
            &identAuthority.Value[5]);
    }
    else
    {
        DWORD value;

        swscanf_s(ptr, L"%lu", &value);

        identAuthority.Value[5] = (BYTE)(value & 0x000000FF);
        identAuthority.Value[4] = (BYTE)((value & 0x0000FF00) >> 8);
        identAuthority.Value[3] = (BYTE)((value & 0x00FF0000) >> 16);
        identAuthority.Value[2] = (BYTE)((value & 0xFF000000) >> 24);
    }

    // Skip -
    *ptr1 = '-';
    ptr = ptr1;
    ptr1++;

    for (i = 0; i < 8; i++)
    {
        // get subauthority
        if (!(ptr = wcschr(ptr, L'-')))
        {
            break;
        }
        *ptr=0;
        ptr++;
        nByteAuthorityCount++;
    }

    for (i = 0; i < nByteAuthorityCount; i++)
    {
        // Get subauthority.
        swscanf_s(ptr1, L"%lu", &dwSubAuthority[i]);
        ptr1 += wcslen(ptr1) + 1;
    }

    if (!AllocateAndInitializeSid(&identAuthority,
        nByteAuthorityCount,
        dwSubAuthority[0],
        dwSubAuthority[1],
        dwSubAuthority[2],
        dwSubAuthority[3],
        dwSubAuthority[4],
        dwSubAuthority[5],
        dwSubAuthority[6],
        dwSubAuthority[7],
        ppSid))
    {
        *ppSid = 0;
    }

    return *ppSid ? TRUE : FALSE;
}

/* Creates a new security attributes object with the following properties:
 * The object has no group set, the owner is set to pSID, and there is one 
 * ACL entry for full access granted to pSID, effectively making this 
 * object only useable by the user/group specified by pSID.
 */
SECURITY_ATTRIBUTES * SecurityHelper::GetExclusiveSecurityAttributes(PSID pSID)
{
	BOOL good=TRUE;

	// create an ACL with space for a single full access entry
	ACL *acl;
	DWORD aclSize = sizeof (ACL);
	aclSize += (sizeof(ACCESS_ALLOWED_ACE) - sizeof(DWORD) + GetLengthSid(pSID));
	acl = (ACL*)(new unsigned char[aclSize]);

	if (acl==NULL)
		return NULL;
	
	// initialize the ACL
	good = good & InitializeAcl(acl, aclSize, ACL_REVISION );	//ACL_REVISION_DS

	// add a full access entry to the ACL 
	good = good & AddAccessAllowedAce(acl, ACL_REVISION, GENERIC_ALL, pSID);
	
	if (!good)
	{
		delete [] acl;
		return NULL;
	}

	// create a security descriptor to assign to the file
	SECURITY_DESCRIPTOR *fsd=new SECURITY_DESCRIPTOR;

	if (fsd==NULL)
	{
		delete [] acl;
		return NULL;
	}

	good = good & InitializeSecurityDescriptor(fsd, SECURITY_DESCRIPTOR_REVISION);
	good = good & SetSecurityDescriptorGroup(fsd, NULL, FALSE);
	good = good & SetSecurityDescriptorOwner(fsd, pSID, FALSE);
	good = good & SetSecurityDescriptorDacl(fsd, TRUE, acl, FALSE);

	if (!good)
	{
		delete fsd;
		delete [] acl;
		return NULL;
	}
	
	// create a security attributes for the file based on the descriptor
	SECURITY_ATTRIBUTES *fsa = new SECURITY_ATTRIBUTES;

	if (fsa==NULL)
	{
		delete fsd;
		delete [] acl;
		return NULL;
	}

	fsa->nLength=sizeof(SECURITY_ATTRIBUTES);
	fsa->lpSecurityDescriptor=fsd;
	fsa->bInheritHandle=FALSE;
	
	return fsa;
}

/* Destroys all the resources used by the SECURITY_ATTRIBUTES object created
 * in a call to GetExclusiveSecurityAttributes().
 * This includes the underlying ACL, and the SECURITY_DESCRIPTOR objects
 * as well as the SECURITY_ATTRIBUTES object itself.
 */
void SecurityHelper::DeleteSecurityAttributes(SECURITY_ATTRIBUTES * pSA)
{
	SECURITY_DESCRIPTOR * pSD;
	ACL * pACL;

	if (pSA==NULL)
		return;

	// recurse through the structure SA->SD->ACL
	pSD=(SECURITY_DESCRIPTOR*)pSA->lpSecurityDescriptor;
	if (pSD!=NULL)
	{
		pACL=pSD->Dacl;
		if (pACL!=NULL)
		{
			delete [] pACL;
		}

		delete pSD;
	}
	
	delete pSA;
}



/* Obtains the currently logged in user name with domain for the current process.
 * Caller must call SysFreeString on the out-params. These values may be NULL on failure.
 * Returns FALSE on failure/error.
 */
BOOL SecurityHelper::GetCurrentUserAndDomain(BSTR *pbstrUsername, BSTR *pbstrDomain)
{

	BOOL         fSuccess = FALSE;
	HANDLE       hToken   = NULL;
	PTOKEN_USER  ptiUser  = NULL;
	DWORD        cbti     = 0;
	SID_NAME_USE snu;
	wchar_t *pUsername=NULL, *pDomain=NULL;
	

	while(1)
	{
		*pbstrUsername=NULL;
		*pbstrDomain=NULL;

		// Get the calling thread's access token.
		if (!OpenThreadToken(GetCurrentThread(), TOKEN_QUERY, TRUE, &hToken))
		{
			if (GetLastError() != ERROR_NO_TOKEN)
				break;
			
			// Retry against process token if no thread token exists.
			if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
				break;
		}

		// Obtain the size of the user information in the token.
		if (GetTokenInformation(hToken, TokenUser, NULL, 0, &cbti))
		{
			// Call should have failed due to zero-length buffer.
			break;
		}
		else
		{
			// Call should have failed due to zero-length buffer.
			if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
				break;
		}

		// Allocate buffer for user information in the token.
		ptiUser = (PTOKEN_USER) HeapAlloc(GetProcessHeap(), 0, cbti);
		if (!ptiUser)
			break;

		// Retrieve the user information from the token.
		if (!GetTokenInformation(hToken, TokenUser, ptiUser, cbti, &cbti))
			break;

		DWORD cchUsername=0, cchDomain=0, err=0;

		// Retrieve user name and domain name based on user's SID.
		if (!LookupAccountSidW(NULL, ptiUser->User.Sid, pUsername, &cchUsername, pDomain, &cchDomain, &snu))
		{
			err = GetLastError();
			if (err != ERROR_MORE_DATA && err != ERROR_INSUFFICIENT_BUFFER)
			{
				break;
			}
			else
			{
				pUsername=new wchar_t[cchUsername];
				pDomain=new wchar_t[cchDomain];
			}
		}

		if (!LookupAccountSidW(NULL, ptiUser->User.Sid, pUsername, &cchUsername, pDomain, &cchDomain, &snu))
		{
			break;
		}

		fSuccess = TRUE;
		break;
	}

	if (pUsername)
	{
		*pbstrUsername=SysAllocString(pUsername);
		delete [] pUsername;
	}

	if (pDomain)
	{
		*pbstrDomain=SysAllocString(pDomain);
		delete [] pDomain;
	}
	
	// Free resources.
	if (hToken)
		CloseHandle(hToken);
	
	if (ptiUser)
		HeapFree(GetProcessHeap(), 0, ptiUser);
		
	return fSuccess;
}

/** returns true if the client proxy blanket has an authentication 
	level at least as high as min_authn_level and an impersonation 
	level at least as high as min_imp_level.
	See RPC_C_AUTHN_LEVEL_xxx and RPC_C_IMP_LEVEL_xxx in MSDN. */
HRESULT SecurityHelper::TestClientProxyBlanketLevel(DWORD min_authn_level, DWORD min_imp_level)
{
	HRESULT hr = E_FAIL;
	DWORD authn_level, imp_level;
	
	hr = CoQueryClientBlanket(NULL, NULL, NULL, &authn_level, &imp_level, NULL, NULL);
	
	if (SUCCEEDED(hr))
	{
		bool test_success=true;
		if (min_authn_level!=RPC_C_AUTHN_LEVEL_DEFAULT && authn_level!=RPC_C_AUTHN_LEVEL_DEFAULT)
			test_success = test_success && (authn_level>=min_authn_level);
		if (min_imp_level!=RPC_C_IMP_LEVEL_DEFAULT && imp_level!=RPC_C_IMP_LEVEL_DEFAULT)
			test_success = test_success && (min_imp_level>=imp_level);
		
		if (test_success)
			return S_OK;
		if (!test_success)
			return S_FALSE;
	}
	
	return hr;
}


/** For explanation of parameters, see the documentation for CoSetProxyBlanket */
SecurityHelper::ProxyBlanket::ProxyBlanket(
		IUnknown * pProxy,         //Indicates the proxy to set
		DWORD dwAuthnSvc,          //Authentication service to use
		DWORD dwAuthzSvc,          //Authorization service to use
		WCHAR * pServerPrincName,  //Server principal name to use with 
								   // the authentication service
		DWORD dwAuthnLevel,        //Authentication level to use
		DWORD dwImpLevel,          //Impersonation level to use
		DWORD dwCapabilities,       //Capability flags
		const wchar_t *pDomain,
		const wchar_t *pUsername,
		const wchar_t *pPassword
	) :
	m_pProxy(pProxy),
	m_bRestoreProxyBlanket(false)
{
	m_hr = SaveBlanket();
	if (FAILED(m_hr)) return;
	m_bRestoreProxyBlanket=true;
	

    COAUTHIDENTITY  *pAuthident=0;

	if (pDomain || pUsername || pPassword)
	{
		pAuthident=new COAUTHIDENTITY;
		memset((void *)pAuthident,0,sizeof(COAUTHIDENTITY));

		if (pDomain)
		{
			pAuthident->DomainLength=(ULONG)wcslen(pDomain);
			pAuthident->Domain=(unsigned short*)pDomain;
		}
		if (pUsername)
		{
			pAuthident->UserLength=(ULONG)wcslen(pUsername);
			pAuthident->User=(unsigned short*)pUsername;
		}
		if (pPassword)
		{
			pAuthident->PasswordLength=(ULONG)wcslen(pPassword);
			pAuthident->Password=(unsigned short*)pPassword;
		}
		pAuthident->Flags = SEC_WINNT_AUTH_IDENTITY_UNICODE;
	}
	
	m_hr = CoSetProxyBlanket(
		m_pProxy, 
		dwAuthnSvc, 
		dwAuthzSvc, 
		pServerPrincName,
		dwAuthnLevel,
		dwImpLevel,
		pAuthident,
		dwCapabilities);

	if (pAuthident)
	{
		memset((void*)pAuthident,0,sizeof(COAUTHIDENTITY));
		delete pAuthident;
	}
}

SecurityHelper::ProxyBlanket::ProxyBlanket(
		IUnknown * pProxy,         //Indicates the proxy to set
		DWORD dwAuthnLevel,        //Authentication level to use
		DWORD dwImpLevel,           //Impersonation level to use
		const wchar_t *pDomain,
		const wchar_t *pUsername,
		const wchar_t *pPassword
	) : 
	m_pProxy(pProxy),
	m_bRestoreProxyBlanket(false)
{
	m_hr = SaveBlanket();
	if (FAILED(m_hr)) return;
	m_bRestoreProxyBlanket=true;
	

    COAUTHIDENTITY  *pAuthident=0;

	if (pDomain || pUsername || pPassword)
	{
		pAuthident=new COAUTHIDENTITY;
		memset((void *)pAuthident,0,sizeof(COAUTHIDENTITY));

		if (pDomain)
		{
			pAuthident->DomainLength=(ULONG)wcslen(pDomain);
			pAuthident->Domain=(unsigned short*)pDomain;
		}
		if (pUsername)
		{
			pAuthident->UserLength=(ULONG)wcslen(pUsername);
			pAuthident->User=(unsigned short*)pUsername;
		}
		if (pPassword)
		{
			pAuthident->PasswordLength=(ULONG)wcslen(pPassword);
			pAuthident->Password=(unsigned short*)pPassword;
		}
		pAuthident->Flags = SEC_WINNT_AUTH_IDENTITY_UNICODE;
	}
	
	m_hr = CoSetProxyBlanket(
		m_pProxy, 
		m_dwAuthnSvc, 
		m_dwAuthzSvc, 
		NULL,
		dwAuthnLevel,
		dwImpLevel,
		pAuthident,
		m_grfCaps);

	if (pAuthident)
	{
		memset((void*)pAuthident,0,sizeof(COAUTHIDENTITY));
		delete pAuthident;
	}
}

SecurityHelper::ProxyBlanket::~ProxyBlanket()
{
	HRESULT hr;
	if (m_bRestoreProxyBlanket) 
		hr = CoSetProxyBlanket(
			m_pProxy, 
			m_dwAuthnSvc, 
			m_dwAuthzSvc, 
			NULL,
			m_dwAuthnLvl,
			m_dwImpLevl,
			m_sIdentityHandle,
			m_grfCaps);
}

HRESULT SecurityHelper::ProxyBlanket::SaveBlanket()
{
	return CoQueryProxyBlanket(
		m_pProxy, 
		&m_dwAuthnSvc, 
		&m_dwAuthzSvc, 
		0, 
		&m_dwAuthnLvl, 
		&m_dwImpLevl, 
		&m_sIdentityHandle, 
		&m_grfCaps);
}


