#ifndef __SecurityHelper9987686566_H
#define __SecurityHelper9987686566_H

#ifndef _WIN32_DCOM
#define _WIN32_DCOM
#endif

#include <windows.h>
#include <stdio.h>

class SecurityHelper
{
public:
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
	static BYTE * GetSid(LPWSTR pwcUserName, LPWSTR pwcMachine=NULL);
	
	/* Creates two buffers holding the account & domain for the given SID.
	 * pSid			is the SID to loopup. 
	 * pwcMachine   is the optional machine to search for this account. Defaults 
	 *              to NULL indicating the local machine and its domain controllers
	 * The returned buffers need to be deleted with delete() when done.
	 * Returns FALSE if the account isn't found.
	 */
	static BOOL GetSidAccount(PSID pSid, LPWSTR* ppwcUserName, LPWSTR* ppwcUserDomain, LPWSTR pwcMachine=NULL);

	/* Creates a new security attributes object with the following properties:
	 * The object has no group set, the owner is set to pSID, and there is one 
	 * ACL entry for full access granted to pSID, effectively making this 
	 * object only useable by the user/group specified by pSID.
	 */
	static SECURITY_ATTRIBUTES * GetExclusiveSecurityAttributes(PSID pSID);

	/* Destroys all the resources used by the SECURITY_ATTRIBUTES object created
	 * in a call to GetExclusiveSecurityAttributes().
	 * This includes the underlying ACL, and the SECURITY_DESCRIPTOR objects
	 * as well as the SECURITY_ATTRIBUTES object itself.
	 */
	static void DeleteSecurityAttributes(SECURITY_ATTRIBUTES * pSA);

	/* 
	 * Converts an SID to string format 
	 */
	static BOOL SidToString(PSID pSid, LPWSTR pwcSid, LPDWORD lpdwBufferLen);

	/* 
	 *Converts an SID string to an SID 
	 */
	static BOOL SidStringToSid(LPWSTR pwcSid, PSID* ppSid);

	/*
	 * Converts an SID to a and allocates a BSTR
	 * Caller must call SysFreeString on the BSTR if it is not null.
	 */
	static BOOL SidToBSTR(PSID pSid, BSTR *pSidString);
	
	/* Obtains the currently logged in user name with domain for the current process.
	 * Caller must call SysFreeString on the out-params. These values may be NULL on failure.
	 * Returns FALSE on failure/error.
	 */
	static BOOL SecurityHelper::GetCurrentUserAndDomain(BSTR *pbstrUsername, BSTR *pbstrDomain);
	
	/** Returns S_OK if the client proxy blanket has an authentication 
	    level at least as high as min_authn_level and an impersonation 
	    level at least as high as min_imp_level.
		Returns S_FALSE if the client proxy blanket does not have at 
		least the specified authentication level.
		Returns an error HRESULT if some failure occurs trying to check
		the client proxy authentication level.
	    See RPC_C_AUTHN_LEVEL_xxx and RPC_C_IMP_LEVEL_xxx in MSDN. */
	static HRESULT TestClientProxyBlanketLevel(DWORD min_authn_level, DWORD min_imp_level);
	

	class ProxyBlanket
	{
	public:
		/** For explanation of parameters, see the documentation for CoSetProxyBlanket */
		ProxyBlanket(
			IUnknown * pProxy,         //Indicates the proxy to set
			DWORD dwAuthnSvc,          //Authentication service to use
			DWORD dwAuthzSvc,          //Authorization service to use
			WCHAR * pServerPrincName,  //Server principal name to use with 
									   // the authentication service
			DWORD dwAuthnLevel,        //Authentication level to use
			DWORD dwImpLevel,          //Impersonation level to use
			DWORD dwCapabilities,      //Capability flags
			const wchar_t *pDomain=NULL,
			const wchar_t *pUsername=NULL,
			const wchar_t *pPassword=NULL
		);
		ProxyBlanket(
			IUnknown * pProxy,         //Indicates the proxy to set
			DWORD dwAuthnLevel,        //Authentication level to use
			DWORD dwImpLevel,          //Impersonation level to use
			const wchar_t *pDomain=NULL,
			const wchar_t *pUsername=NULL,
			const wchar_t *pPassword=NULL
		);
		virtual ~ProxyBlanket();
		
		HRESULT GetStatus() { return m_hr; }
	protected:
		IUnknown * m_pProxy;
		bool m_bRestoreProxyBlanket;
		DWORD m_dwAuthnSvc, m_dwAuthzSvc, m_dwImpLevl, m_grfCaps, m_dwAuthnLvl;
		RPC_AUTH_IDENTITY_HANDLE m_sIdentityHandle;
		
		HRESULT SaveBlanket();

		HRESULT m_hr;
	};
	
};



#endif
