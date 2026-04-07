//----------------------------------------------------------------------------------------*
// filename -       dosutils.cpp                                                          *
//                                                                                        *
//             This module contains some helper functions dealing with DOS file           *
//             naming conventions and other misc. routines                                *
//                                                                                        *
// Functions:                                                                             *
//    mkLineStr()                                                                         *
//    verifyDir()                                                                         *
//    makeFName()                                                                         *
//    copyFile()                                                                          *
//    percentOf()                                                                         *
//    fixDosName()                                                                        *
//    fixFileName()                                                                       *
//    xatoi()                                                                             *
//    SCreateDirectory()                                                                  *
//	StringMatch()                                                                       *
//    GetWin32ErrorMessage()                                                              *
//                                                                                        *
//                                                                                        *
//    Copyright (c) 1993-1998 by Solimar Systems, Inc.  All Rights Reserved.              *
//                                                                                        *
// Release Info:                                                                          *
// Nm     Date         Discription                                                        *
// SLB 26.nov.97  Added xatoi().                                                          *
// SLB   28.oct.97   SPD Nt port.                                                         *
// SLB   30.jun.2004 Engine port.  I am not sure if a lot of these routines are even      *
//                   used anymore.  For this port, I have commented out most of the       *
//                   functionality in many of these routines.  It will be added as we     *
//                   determine what is actuall needed                                     *
// SLB 08.jan.2007   Added moved GetWin32ErrorMessage() from CSpdFwMessaging to here.     *
//                                                                                        *
//----------------------------------------------------------------------------------------*
#define STRICT
#define __SPDUTILS_CPP
#include "L_SpdUtils.h"
#include "L_CNtfsFolderName.h"
#include <locale> // std::locale
#include <sddl.h>                                                                   // CR.28872.V02; added.
#include <comsvcs.h>                                                                // CR.28872.V02; added.

using namespace SpdException;

/* GetCurrentUserAndDomain() - Obtains the user name, domain and SID for the current process.
 * Throws hr_exception on failure/error.
 *
 * SLB 17.jan.2020 CR.28872.V02; moved into SpdUtils from CSSIApplication (Dispatcher class). This method is used 
 * in multiple .exes, so I consolidated it here.
---------------------------------------------------------------------------------------------*/
void SpdUtils::GetCurrentUserAndDomain(
   CSpdCString &wsUserName,
   CSpdCString &wsDomainName,
   CSpdCString &wsSid)
{
   HANDLE hToken = NULL;
   PTOKEN_USER ptiUser = NULL;
   try
   {
      // Get the calling thread's access token.
      BOOL bRES;
      DWORD le;
      bRES = OpenThreadToken(GetCurrentThread(), TOKEN_QUERY, TRUE, &hToken);
      le = GetLastError();
      if (bRES == FALSE && le == ERROR_NO_TOKEN)
      {
         if (le != ERROR_NO_TOKEN) ThrowHr(HRESULT_FROM_WIN32(le), __WFUNCTION__, L"Could not open thread token.");

         bRES = OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken);
         le = GetLastError();
      }
      if (bRES == FALSE) ThrowHr(HRESULT_FROM_WIN32(le), __WFUNCTION__, L"Could not open thread OR process token.");

      // Obtain the size of the user information in the token.
      // Note: This means we expect the call to fail with le==ERROR_INSUFFICIENT_BUFFER. 
      DWORD cbti = 0;
      bRES = GetTokenInformation(hToken, TokenUser, NULL, 0, &cbti);
      le = GetLastError();
      if (bRES == TRUE || le != ERROR_INSUFFICIENT_BUFFER) ThrowHr(HRESULT_FROM_WIN32(le), __WFUNCTION__, L"Unexpected result retrieving needed token information's buffer size.");

      // Allocate buffer for user information in the token.
      ptiUser = (PTOKEN_USER)HeapAlloc(GetProcessHeap(), 0, cbti);
      if (!ptiUser) ThrowHr(E_POINTER, __WFUNCTION__, L"Problem allocating Token's user information buffer.");

      // Retrieve the user information from the token.
      bRES = GetTokenInformation(hToken, TokenUser, ptiUser, cbti, &cbti);
      le = GetLastError();
      if (bRES == FALSE) ThrowHr(HRESULT_FROM_WIN32(le), __WFUNCTION__, L"Could not retrieve user information from token.");

      // Retrieve user name and domain name based on user's SID. If either username or domain name exceed
      // 512 characters, this method will fail.
      DWORD dwUsername = 512, dwDomain = 512;
      wchar_t wcs_user_name[512];
      wchar_t wcs_domain_name[512];
      SID_NAME_USE snu;
      bRES = LookupAccountSidW(NULL, ptiUser->User.Sid, wcs_user_name, &dwUsername, wcs_domain_name, &dwDomain, &snu);
      le = GetLastError();
      if (bRES == FALSE) ThrowHr(HRESULT_FROM_WIN32(le), __WFUNCTION__, L"Could not lookup account name from user's SID.");

      wsUserName = wcs_user_name;
      wsDomainName = wcs_domain_name;
      wsSid = SidToString(ptiUser->User.Sid);

      // Free resources.
      CloseHandle(hToken);
      HeapFree(GetProcessHeap(), 0, ptiUser);
   }
   catch (SpdException::hr_exception &e)
   {
      // Free resources, if needed.
      if (hToken) CloseHandle(hToken);
      if (ptiUser) HeapFree(GetProcessHeap(), 0, ptiUser);

      // Generate some debug feedback.
      CEnterExitMsg::TraceMsg(L"(%08X) Error retrieving current user's name and domain. %s Location: %s()",
         e.GetHr(),
         e.GetMessage().c_str(),
         e.GetMethod().c_str());
      // rethrow the exception.
      throw (e);
   }
}



/* SidToString() - Converts an SID to string format
 *
 * SLB 17.jan.2020 CR.28872.V02; moved into SpdUtils from CSSIApplication (Dispatcher class). This method is used
 * in multiple .exes, so I consolidated it here.
 ---------------------------------------------------------------------------------------------*/
CSpdCString SpdUtils::SidToString(
   PSID pSid)
{
   // Validate the binary SID object.
   BOOL bRES;
   bRES = IsValidSid(pSid);
   if (bRES == FALSE) ThrowHr(E_INVALIDARG, __WFUNCTION__, L"SID ptr is invalid.");

   DWORD le(0);
   wchar_t *pwcsSid(NULL);
   bRES = ConvertSidToStringSidW(pSid, &pwcsSid);
   le = GetLastError();
   if (bRES == FALSE) ThrowHr(HRESULT_FROM_WIN32(le), __WFUNCTION__, L"Unable to convert SID ptr to a string.");

   // Success. cleanup and return value.
   CSpdCString wsSid = pwcsSid;
   LocalFree((HLOCAL)pwcsSid);
   return wsSid;
}

/* GetComSessionsSid() - determines the sid of the COM Session.
 *
 *   If COM context is not valid, returns the SID of the process's running identity.
 *
 * SLB 11.jul.2006 CR.6639; added. Now CSpdAppClient is responsible for setting
 * sid of the SessionToken before passing it down to the s_app_client object.
 * SLB 03.feb.2020 CR.28872.V02; Moved here from SpdApplicationCLient code/project. Modified to return 
 * the SID in string form. Now throw's hr_exception on error. Converted all com_error exceptions to hr_exceptions.
 * SLB 14.feb.2020 CR.29006; Enhanced to return the SID of the process if no COM context is active yet.
---------------------------------------------------------------------------------------------*/
CSpdCString SpdUtils::GetComSessionsSid(
   bool bUsesLogonAuthentication,
   const CSpdCString &wsLogonAccountName,
   const CSpdCString &wsLogonDomainName,
   const CSpdCString &wsLogonPassword,
   CSpdCString &wsAccountName) 
{

   if (!bUsesLogonAuthentication)
   {
      // No authentication needed.  Uses COM+ Windows SID.
      //
      // SLB 29.nov.2005 CR.5254; Modified this method to return more info (namely user name and machine name).  Renamed it as 
      // well.
      // SLB 11.jul.2006 CR.????; moved old ApplicationClient::GetProxyUserInfo() into this method.
      /*
      ISecurityIdentityColl
      SID The security identifier of the caller.  "SID" V_ARRAY
      Account Name The account name that the caller is using.  "AccountName" V_BSTR
      Authentication Service The authentication service used by the caller, such as NTLMSSP, Kerberos, or SSL.  "AuthenticationService" V_I4
      Impersonation Level The impersonation level, which indicates how much authority the caller has been given to act on a client's behalf.  "ImpersonationLevel" V_I4
      Authentication Level The authentication level used by the caller, which indicates the amount of protection given during the call.  "AuthenticationLevel" V_I4
      */

      /*
      Direct Caller               The immediate caller of the object.                           "DirectCaller"            A SecurityIdentity object
      Original Caller               The caller that originated the chain of calls to the object.      "OriginalCaller"         A SecurityIdentity object
      Minimum Authentication Level   The lowest authentication level used in the chain of calls.         "MinAuthenticationLevel"   Long
      Number of Callers            The number of callers in the chain of calls to the object.         "NumCallers"            Long
      Callers                     The callers in the chain of calls that ends with the current call.   "Callers"               A SecurityCallers object
      */

      ISecurityIdentityColl *pSecIdentColl = 0;
      try
      {
         CComVariant vtCaller, vtCallerSID, vtCallerUsername;
         ISecurityCallContext *pCallCtx = 0;

         HRESULT hr;
         hr = CoGetCallContext(IID_ISecurityCallContext, (void **)&pCallCtx);
         if (FAILED(hr))
         {
            if (hr != E_NOINTERFACE) ThrowHr(hr, __WFUNCTION__, L"Failed to retrieve COM+ context info."); // CR.22906; modified..

            // CR.22906; Not an error. Just no COM context.
            // FinalConstruct() has not completed or this is not a COM call, Get service accountname and Sid from process info.
            CSpdCString wsUser, wsDomain, wsSid;
            GetCurrentUserAndDomain(wsUser, wsDomain, wsSid);

            // success. set output and exit.
            wsAccountName = wsDomain + L"\\" + wsUser;
            return wsSid;
         }

         hr = pCallCtx->get_Item(CComBSTR(L"OriginalCaller"), &vtCaller);
         pCallCtx->Release();
         if (FAILED(hr)) ThrowHr(hr, __WFUNCTION__, L"Failed to retrieve Caller Identity from COM+ context info.");

         hr = (vtCaller.punkVal)->QueryInterface(IID_ISecurityIdentityColl, (void**)&pSecIdentColl);
         if (FAILED(hr)) ThrowHr(hr, __WFUNCTION__, L"Failed to retrieve security identity fomr Caller info.");

         hr = pSecIdentColl->get_Item(CComBSTR(L"SID"), &vtCallerSID);
         if (FAILED(hr)) ThrowHr(hr, __WFUNCTION__, L"Failed to retrieve SID from Security info.");

         hr = pSecIdentColl->get_Item(CComBSTR(L"AccountName"), &vtCallerUsername);
         if (FAILED(hr)) ThrowHr(hr, __WFUNCTION__, L"Failed to retrieve Account name from Security info.");

         pSecIdentColl->Release();
         pSecIdentColl = 0;

         // success. set output and exit.
         wsAccountName = vtCallerUsername.bstrVal;
         return SidToString((PSID)vtCallerSID.parray->pvData);
      }
      catch (hr_exception &e)
      {
         // Clean up.
         if (pSecIdentColl) pSecIdentColl->Release();

         // Rethrow the exception.
         ThrowHr(e.GetHr() /*E_SPDE_SEC_AUTHENTICATION_FAILED*/, e.GetMethod().c_str(), L"Problem (%08X) retrieving calling process' identity. %s", //\n%s",
            e.GetHr(),
            e.GetMessage().c_str()/*,
            CSpdHResult::GetDescription(e.GetHr()).c_str()*/);
      }
   }

   // Using alternate logon... 
   // Need to authenticate account name and retrieve its Windows SID.

   // CR.6639; Authenticate the USERNAME and PASSWORD.
   try
   {
      BOOL bRES;
      HANDLE hLogon(INVALID_HANDLE_VALUE);
      bRES = LogonUserW(
         wsLogonAccountName.c_str(),
         wsLogonDomainName.c_str(),
         wsLogonPassword.c_str(),
         LOGON32_LOGON_NETWORK, //LOGON32_LOGON_INTERACTIVE, 
         LOGON32_PROVIDER_DEFAULT,
         &hLogon);
      if (bRES == FALSE) ThrowHr(HRESULT_FROM_WIN32(GetLastError()), __WFUNCTION__, L"Logon failed, invalid username or password?");

      // Authenticated the Username and Password. 
      CloseHandle(hLogon);

      // Use Win32 method LookupAccountName() to get the SID from the account name.
      CSpdCString wsFullAccountName(wsLogonDomainName + CSpdCString(L"\\") + wsLogonAccountName);
      BYTE pbSidBuf[1024];
      DWORD dwSidBufLen(sizeof(pbSidBuf));
      wchar_t wcsRefDNBuf[1024];
      DWORD dwRefDNBufLen(sizeof(wcsRefDNBuf) / sizeof(wchar_t));
      SID_NAME_USE snuUse;
      bRES = LookupAccountNameW(
         NULL,                      //LPCTSTR lpSystemName,
         wsFullAccountName.c_str(), //LPCTSTR lpAccountName,
         pbSidBuf,                  //PSID Sid,
         &dwSidBufLen,              //LPDWORD cbSid,
         wcsRefDNBuf,               //LPTSTR ReferencedDomainName,
         &dwRefDNBufLen,            //LPDWORD cchReferencedDomainName,
         &snuUse);                  //  PSID_NAME_USE peUse
      if (bRES == FALSE) ThrowHr(HRESULT_FROM_WIN32(::GetLastError()), __WFUNCTION__, L"Error retrieving SID for Username %s.", wsFullAccountName.c_str());

      // success.
      wsAccountName = wsFullAccountName;
      return SidToString(pbSidBuf);
   }
   catch (hr_exception &e)
   {
      // Rethrow the exception.
      ThrowHr(e.GetHr() /*E_SPDE_SEC_AUTHENTICATION_FAILED*/, e.GetMethod().c_str(), L"Alternate logon problem (%08X). %s", //\n%s",
         e.GetHr(),
         e.GetMessage().c_str()/*,
         CSpdHResult::GetDescription(e.GetHr()).c_str()*/);
   }

   // should never reach this.
   return L"Invalid SID";
}


/* SidToString() - Converts an SID object to string format 
 *
 * SLB 17.jan.2020 CR.xxx; moved into SpdUtils from CSSIApplication (Dispatcher class). This method is used
 * in multiple .exes, so I consolidated it here.
---------------------------------------------------------------------------------------------*/
BOOL SidToString(
   PSID pSid,
   LPWSTR pwcSid,
   LPDWORD lpdwBufferLen)
{
   // Validate the binary SID.
   if (!IsValidSid(pSid)) return FALSE;

   // Get the identifier authority value from the SID.
   PSID_IDENTIFIER_AUTHORITY psia;
   psia = GetSidIdentifierAuthority(pSid);

   // Get the number of subauthorities in the SID.
   DWORD dwSubAuthorities;
   dwSubAuthorities = *GetSidSubAuthorityCount(pSid);

   // Compute the buffer length.
   // S-SID_REVISION- + IdentifierAuthority- + subauthorities- + NULL
   DWORD dwSidSize;
   dwSidSize = (15 + 12 + (12 * dwSubAuthorities) + 1) * sizeof(WCHAR);

   // Check input buffer length.
   // If too small, indicate the proper size and set last error.
   if (*lpdwBufferLen < dwSidSize)
   {
      *lpdwBufferLen = dwSidSize;
      SetLastError(ERROR_INSUFFICIENT_BUFFER);
      return FALSE;
   }

   // Add 'S' prefix and revision number to the string.
   DWORD dwSidRev = SID_REVISION;
   dwSidSize = wsprintfW(pwcSid, L"S-%lu-", dwSidRev);

   // Add SID identifier authority to the string.

   if ((psia->Value[0] != 0) || (psia->Value[1] != 0))
   {
      dwSidSize += wsprintfW(pwcSid + wcslen(pwcSid),
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
      dwSidSize += wsprintfW(pwcSid + wcslen(pwcSid),
         L"%lu",
         (ULONG)(psia->Value[5]) +
         (ULONG)(psia->Value[4] << 8) +
         (ULONG)(psia->Value[3] << 16) +
         (ULONG)(psia->Value[2] << 24));
   }

   // Add SID subauthorities to the string.
   DWORD dwCounter;
   for (dwCounter = 0; dwCounter < dwSubAuthorities; dwCounter++)
   {
      dwSidSize += wsprintfW(
         pwcSid + dwSidSize,
         L"-%lu",
         *GetSidSubAuthority(pSid, dwCounter));
   }

   return TRUE;
}

/* GetComputerNameW() - Gets computer name from OS.
 *
 * SLB 23.feb.2019 CR.26999.Item82; Created. This functionality was spread throughout SPDE code.
 * This method centralizes it.
 * SLB 04.mar.2020 CR.29058; Modified to return fully qualified machine name. 
 * Optimization made to save value in static string and return reference to that, once it is initialized.
 * SLB 25.jun.2020 CR.29471; Make sure to support machines not on a domain.
------------------------------------------------------------------------*/
const CSpdCString & SpdUtils::GetComputerNameW()                                    // CR.29058; modified.
{
   static CSpdCString wsComputerName;                                               // CR.29058; added.
   if (!wsComputerName.empty()) return wsComputerName;

   // SLB CR.29471 implementation note: GetComputerNameEx() always returns TRUE. Regardless of request or whether
   // system is on a domain.  Also, specifying ComputerNameDnsFullyQualified is all that is needed... If not on a 
   // domain, then the NETBIOS values is returned.
   wchar_t wcsComputerName[1024];
   DWORD dwTcharSize((DWORD)(sizeof(wcsComputerName)/sizeof(wchar_t))); 
   GetComputerNameEx(ComputerNameDnsFullyQualified, wcsComputerName, &dwTcharSize); // CR.29058; modified.
   DWORD le(GetLastError());

   wsComputerName = wcsComputerName;                                                // CR.29058; modified.

   if (wsComputerName.empty()) CEnterExitMsg::TraceMsg(L"CR.29471; GetComputerNameEx(ComputerNameDnsFullyQualified) Failed. NtErr <%u>", le);

   BOOL bRES;
   // SLB CR.29471; Generate debug msgs for all possible GetComputerNameEx() requests... These may come in handy for trouble shooting
   // complex domain setups.
   dwTcharSize = (DWORD)(sizeof(wcsComputerName) / sizeof(wchar_t)); wcsComputerName[0] = 0;
   bRES = GetComputerNameEx(ComputerNameNetBIOS, wcsComputerName, &dwTcharSize);
   CEnterExitMsg::TraceMsg(L"CR.29471; GetComputerNameEx(ComputerNameNetBIOS) Res<%u>  Name<%s>", bRES, wcsComputerName);

   dwTcharSize = (DWORD)(sizeof(wcsComputerName) / sizeof(wchar_t)); wcsComputerName[0] = 0;
   bRES = GetComputerNameEx(ComputerNameDnsHostname, wcsComputerName, &dwTcharSize);
   CEnterExitMsg::TraceMsg(L"CR.29471; GetComputerNameEx(ComputerNameDnsHostname) Res<%u>  Name<%s>", bRES, wcsComputerName);

   dwTcharSize = (DWORD)(sizeof(wcsComputerName) / sizeof(wchar_t)); wcsComputerName[0] = 0;
   bRES = GetComputerNameEx(ComputerNameDnsDomain, wcsComputerName, &dwTcharSize);
   CEnterExitMsg::TraceMsg(L"CR.29471; GetComputerNameEx(ComputerNameDnsDomain) Res<%u>  Name<%s>", bRES, wcsComputerName);

   dwTcharSize = (DWORD)(sizeof(wcsComputerName) / sizeof(wchar_t)); wcsComputerName[0] = 0;
   bRES = GetComputerNameEx(ComputerNameDnsFullyQualified, wcsComputerName, &dwTcharSize);
   CEnterExitMsg::TraceMsg(L"CR.29471; GetComputerNameEx(ComputerNameDnsFullyQualified) Res<%u>  *Name*<%s>", bRES, wcsComputerName);

   dwTcharSize = (DWORD)(sizeof(wcsComputerName) / sizeof(wchar_t)); wcsComputerName[0] = 0;
   bRES = GetComputerNameEx(ComputerNamePhysicalNetBIOS, wcsComputerName, &dwTcharSize);
   CEnterExitMsg::TraceMsg(L"CR.29471; GetComputerNameEx(ComputerNamePhysicalNetBIOS) Res<%u>  *Name*<%s>", bRES, wcsComputerName);

   dwTcharSize = (DWORD)(sizeof(wcsComputerName) / sizeof(wchar_t)); wcsComputerName[0] = 0;
   bRES = GetComputerNameEx(ComputerNamePhysicalDnsHostname, wcsComputerName, &dwTcharSize);
   CEnterExitMsg::TraceMsg(L"CR.29471; GetComputerNameEx(ComputerNamePhysicalDnsHostname) Res<%u>  Name<%s>", bRES, wcsComputerName);

   dwTcharSize = (DWORD)(sizeof(wcsComputerName) / sizeof(wchar_t)); wcsComputerName[0] = 0;
   bRES = GetComputerNameEx(ComputerNamePhysicalDnsDomain, wcsComputerName, &dwTcharSize);
   CEnterExitMsg::TraceMsg(L"CR.29471; GetComputerNameEx(ComputerNamePhysicalDnsDomain) Res<%u>  Name<%s>", bRES, wcsComputerName);

   dwTcharSize = (DWORD)(sizeof(wcsComputerName) / sizeof(wchar_t)); wcsComputerName[0] = 0;
   bRES = GetComputerNameEx(ComputerNamePhysicalDnsFullyQualified, wcsComputerName, &dwTcharSize);
   CEnterExitMsg::TraceMsg(L"CR.29471; GetComputerNameEx(ComputerNamePhysicalDnsFullyQualified) Res<%u>  Name<%s>", bRES, wcsComputerName);
   // SLB CR.29471; end of debug msgs.

   return wsComputerName;                                                           // CR.29058; modified.
}

/* GetComputerNameBIOS() - Generates a the simple form of computer name.
 *
 * SLB 18.jun.2025 CR.34040; Created. the shor form is used to generate
 * license data files as well as check if passed in value is the local machine.
------------------------------------------------------------------------*/
SSILIB1_EXPORT const CSpdCString& SpdUtils::GetComputerNameBIOS()
{
	static CSpdCString wsComputerName;  
	if (!wsComputerName.empty()) return wsComputerName;

	wchar_t wcsComputerName[MAX_COMPUTERNAME_LENGTH+1];
	DWORD dwTcharSize((DWORD)(sizeof(wcsComputerName) / sizeof(wchar_t)));
	::GetComputerNameW(wcsComputerName, &dwTcharSize);
	DWORD le(::GetLastError());

	wsComputerName = wcsComputerName;
	if (wsComputerName.empty()) CEnterExitMsg::TraceMsg(L"CR.29471; GetComputerNameEx(ComputerNameDnsFullyQualified) Failed. NtErr <%u>", le);

	return wsComputerName;
}

/* GenerateGUID() - Generates a GUID string.
 *
 * SLB 23.feb.2019 CR.26999.Item82; Created. This functionality was spread throughout SPDE code.
 * This method centralizes it.
 * SLB 16.jul.2020 CR.29523; iCONVERT does not use curly brackets in GUID and it is desireable to 
 * remove them from SPDE GUIDs as well, so modified code to do this.
------------------------------------------------------------------------*/
CSpdCString SpdUtils::GenerateGUID()
{
   HRESULT hr; 
   GUID guidVal; 
   hr = CoCreateGuid(&guidVal); 

   LPOLESTR pbstr(NULL); 
   hr = StringFromCLSID(guidVal, &pbstr); 
   if (!pbstr) return L"";

   // CoCreateGuid() always puts { } brackets around GUID. Remove those.
   // Remove opening bracket by intializing wsId starting at offset 1.
   CSpdCString wsId(pbstr + 1);                                                     // CR.29523; modified.
   CoTaskMemFree(pbstr); 

   // remove closing bracket.
   size_t stPos(wsId.find_first_of(L'}'));                                          // CR.29523; added.
   wsId.erase(stPos);                                                               // CR.29523; added.

   return wsId; 
}

/* VerifyFolder() - Verifies that directory string exists.
 *               Added by SLB, 9-May-93 for detection of network drives
 *               which opendir() cannot do.
 * SLB 17.nov.2005 - removed call IsFolderAccessOk().  I believe this is un-necessary and
 * it was not getting passed meaningful info anyhow.
------------------------------------------------------------------------*/
bool SpdUtils::VerifyFolder(
   const std::wstring &wsDir, 
   bool bWriteRights)
{
   CNtfsFolderName fnFolder(wsDir.c_str(), bWriteRights); 
   DWORD dwNtErr(0);
   return fnFolder.DoesFolderExist(dwNtErr);
} 

/* VerifyFolderA() - Verifies that directory string exists.
 * Multi-byte version.
------------------------------------------------------------------------*/
bool SpdUtils::VerifyFolderA(
   const char *szDir, 
   bool bWriteRights)
{  
   wchar_t wcsFolder[_MAX_PATH];
   _snwprintf_s(wcsFolder, _TRUNCATE, L"%S",
      szDir);
   return VerifyFolder(wcsFolder, bWriteRights);
} // end of VerifyDirA()




const wchar_t *reserveddosnames[] =
{
   L"CLOCK$",
   L"CON",
   L"AUX",
   L"COM1",
   L"COM2",
   L"COM3",
   L"COM4",
   L"LPT1",
   L"LPT2",
   L"LPT3",
   L"LPT4",
   L"NUL",
   L"PRN",
   NULL,
};

// SLB 97.aug.2005 - Created for use by FixFileChar().
// Note: this is only 128 chars deep.  Methods using this table need
// to ensure that they do not index into this array any deeper.
// SLB 07.aug.2006 - the "+" char is valid for NTFS.  Changed it back to true.
// SLB 21.apr.2008 - CR.9893; the "," char is valid for NTFS. Changed it back to true.
const bool bValidNtfsFileNameChars[128] = 
{
   false, false, false, false, false, false, false, false, // 00-0f
   false, false, false, false, false, false, false, false,
   false, false, false, false, false, false, false, false, // 10-1f
   false, false, false, false, false, false, false, false,
   true,  true,  false, true,  true,  true,  true,  true,  // 20-2f  "\""
   true,  true,  false, true,  true, true,  true,  false,  //        "*" "/"  
   true,  true,  true,  true,  true,  true,  true,  true,  // 30-3f
   true,  true,  false, true,  false, true,  false, false, //        ":" "<" ">" "?" 
   true,  true,  true,  true,  true,  true,  true,  true,  // 40-4f
   true,  true,  true,  true,  true,  true,  true,  true,  
   true,  true,  true,  true,  true,  true,  true,  true,  // 50-5f
   true,  true,  true,  true,  false, true,  true,  true,  //        "\"
   true,  true,  true,  true,  true,  true,  true,  true,  // 60-6f
   true,  true,  true,  true,  true,  true,  true,  true,  
   true,  true,  true,  true,  true,  true,  true,  true,  // 70-7f
   true,  true,  true,  true,  false, true,  true,  false  //        "|" DEL 
}; 

/* IsFileCharOk()
 *    This is a local helper function which returns if the specified character is
 *    Ok for the os type specified.
 * 
 * SLB 12.may.2014 CR.17941; Allow for all chars above 128 to be used for NTFS files systems.
 * This allows foreign languages to name files using their native languages. This presumes that UTF-16 
 * is being used and that all values above 0x128 are valid.
 * SLB 14.dec.2018 CR.7698; Added option to force leaving slash chars alone.
 * SLB 08.aug.2023 CR.32574; Repurposed the old FixFileChar() method into this method.
------------------------------------------------------------------------------*/
bool IsFileCharOk(
   wchar_t c, 
   DWORD os_type_flags,
   bool bSlashesOk = false)                                                         // CR.7698; added.
{
   if (os_type_flags & ostDOS)
   {
      bool valid_dos = false;
      switch (c)
      {
         case L'_': case L'^': case L'$': case L'~': case L'!': case L'#': case L'%': case L'&':
         case L'-': case L'{': case L'}': case L'(': case L')': case L'@': case L'\'': case L'`':
            return true;
			case L'\\': case L'/': 
				if (bSlashesOk) return true;
				return false;
			default:
				if (isalnum(c)) return true;
				break;
      }
		return false;
   }

   if (os_type_flags & ostNTFS)
   {  
      // SLB 97.aug.2005 - modified to use a look up table vs. using isalnum().  Profiling
      // results showed that isalnum() was very expensive.
      bool bValidNtfs(true); 
		if (c >= 128) return true;
		if (c == L'\\' || c == L'/')
		{
			if (bSlashesOk) return true;
			return false;
		}
      if (c < 128 && bValidNtfsFileNameChars[c & 0x7F]) return true;

      return false;
   }

   if (os_type_flags & ostUNIX)
   {
      // DCM - I posted a question on a couple of UNIX newsgroups asking what
      // characters cannot be used in UNIX filenames. The consensus was that
      // all characters except '/' and NULL are valid filename characters.
      // One person said that non-printable characters are invalid. Most
      // people did mention that filenames with nonstandard characters must
      // be properly quoted, although this refers to UNIX shell rules, not
      // FTP or LPR rules. The AIX documentation says there are "certian
      // characters that should be avoided when naming files." These characters
      // are:
      //    '/'   '\\'  '\''  '"'   '*'   ';'   '-'   '?'
      //    '['   ']'   '('   ')'   '~'   '!'   '$'   '{'
      //    '}'   '<'   '>'   '#'   '@'   '&'   '|'
      // The following characters (all included in the AIX list as well) are
      // special UNIX shell metacharacters:
      //    '*'   '('   ')'   '?'   '['   ']'   '}'   '{'
      // We want the most restrictive list of invalid UNIX filename characters,
      // so we currently use the AIX list in conjunction with non-printable
      // characters.
      bool valid_unix = false;
      switch (c)
      {
         case L'`': case L'%': case L'^': case L'_': case L'=': case L'+': case L':': case L',':
         case L'.': case L' ':
				return true;
			case L'\\': case L'/': 
				if (bSlashesOk) return true;
         default:
				if (isalnum(c)) return true;
            break;
      }
		return false;
   }

	// Doesn't evaluate this OS filename type.
	// allow caller to treat it as valid.
	return true;
}

/* FixFileChar()
 *    This is a local helper function for fixFileName(), and will replace
 *    the character with a valid one according to the os_type_flags. Note
 *    that the replacement character for one type might be an invalid
 *    character for another type (e.g., '_' is invalid for ostXeroxDJDE),
 *    thus the order of types is important, and when changes are made to
 *    this function one should check the logic of the type order to make
 *    sure the function will work properly for every combination of
 *    OsTypeFlags.
 * SLB 08.aug.2023 CR.32574; Moved most of this method into the IsFileCharOk() 
 * helper function. the IsFileChar() method is then called here now.
 * method.
------------------------------------------------------------------------------*/
void SpdUtils::FixFileChar(
	wchar_t& c,
	DWORD os_type_flags,
	bool bSlashesOk)
{
	if (!IsFileCharOk(c, os_type_flags, bSlashesOk))
	{
		c = '_';
		return;
	}

	// IsFileCharOk() doesn't check for ostXeroxDJDE type. 
	// ostXeroxDJDE requires some special handling to fix it up.
	if (os_type_flags & ostXeroxDJDE)
	{
		// The following characters are valid Xerox DJDE characters:
		// SLB 14.dec.01 Added '$' to list.  call.fix.39634
		//    'A'-'Z'  '0'-'9'  ' '   ','   '.'   '('   ')'   '='   '''  '$'

		// Convert lower case characters to upper case.
		if (c >= 'a' && c <= 'z')
			c -= char(0x20);
		// Convert all other invalid characters to '0'.
		else
		{
			bool valid_xrx =
				((c >= L'A' && c <= L'Z') || (c >= L'0' && c <= L'9') || c == L' ' ||
					c == L',' || c == L'.' || c == L'(' || c == L')' || c == L'=' || c == L'\'' ||
					c == L'$');
			if (!valid_xrx)
				c = L'0';
		}
	}
}

/* FixDosName()
 *    This function is included for backward compatibility, use fixFileName()
 *    instead.
------------------------------------------------------------------------------*/
std::wstring SpdUtils::FixDosName( 
   const std::wstring &wsFileName, 
   bool bLongFileNamesOk) 
{
   return SpdUtils::FixFileName(wsFileName, bLongFileNamesOk ? ostNTFS : ostDOS);
}

/* FixFileName() - 
 *    This function will modify the fname passed to it, making the fname a
 *    valid filename for the operating system(s) specified by the os_type_flags.
 *    SLB 25.sep.97 added space char.
 *    SLB 13.jan.97 Added support for extension.
 *    DCM 16.dec.98 - Rewrote function, changing name from fixDosName() to
 *    fixFileName(), and replacing longfilenames parameter with os_type_flags
 *    parameter.
 *    SLB 02.jan.02 - Now assumes that path is not included in file name.  This allows
 *    '/' and '\' chars to passed and converted in the jobname (does not apply for ostDOS
 *    type ).  Call.Fix.57730
 *    SLB 21.aug.2023 CR.32574.V01; Added optimization for when the osTypes flags
 *    Specifies ostNoRestricts.
------------------------------------------------------------------------------*/
std::wstring SpdUtils::FixFileName( 
   const std::wstring &wsInputFileName, 
   DWORD dwOsTypes) 
{
	if (dwOsTypes & ostNoRestricts) return wsInputFileName;									// CR.32574.V01; added.

   std::wstring::iterator it; 
   std::wstring wsFileName(wsInputFileName); 

   if (dwOsTypes & ostDOS)
   {
      //char drive[MAXDRIVE], path[MAXPATH], name[MAXFILE], ext[MAXEXT];
      std::wstring wsDrive; 
      std::wstring wsPath; 
      std::wstring wsName; 
      std::wstring wsExt; 

      fnsplit(wsFileName, wsDrive, wsPath, wsName, wsExt); 
      if (wsName.length() > 8) // force file name to 8 bytes.
         wsName.erase( wsName.begin()+8, wsName.end()); 
      if (wsExt.length() > 4)// force ext to 4 bytes in case it is longer (includes the '.' char)
         wsExt.erase( wsExt.begin()+4, wsExt.end()); 

      for (it = wsName.begin(); it != wsName.end(); it++) 
      {
         wchar_t fc(*it); 

         SpdUtils::FixFileChar(fc, dwOsTypes); 
         *it = fc; 
      }
      
      if (wsExt.length() > 0)
      {  // for the extension, skip the '.' char before checking
         for (it = wsExt.begin() + 1; it != wsExt.end(); it++) 
         {
            wchar_t fc(*it); 
            
            SpdUtils::FixFileChar(fc, dwOsTypes); 
            *it = fc; 
         }
      }

      fnmerge(wsFileName, wsDrive, wsPath, wsName, wsExt); 
   }
   else
   {
      for (it = wsFileName.begin(); it != wsFileName.end(); it++) 
      {
         wchar_t fc(*it); 
         
         SpdUtils::FixFileChar(fc, dwOsTypes); 
         *it = fc; 
      }
   }

   if (dwOsTypes&ostDOS || dwOsTypes&ostNTFS)
   {  
      // compare name to any DOS reserved names
      for (int i = 0; reserveddosnames[i]; i++)
      {  
         // SLB 11.jun.2005 CR.4028.Item56 - Turns out that NTFS file names can(at least on XP)
         // begin with a reserved name(unlike DOS file names).  Modified the code to test
         // differently for DOS or NTFS file naming type.
         if ((dwOsTypes & ostDOS && wsFileName.find(reserveddosnames[i]) == 0) ||
            (dwOsTypes & ostNTFS && wsFileName == reserveddosnames[i]))
         {  
            // wsFileName begins with a reserved filename(DOS)
            // or wsFileName == a reserved filename(NTFS)
            wsFileName[0] = L'$'; 
            break; 
         }
      }

      // NTFS file names, especially, should not end with a '.' char or a ' ' char
      while (
         wsFileName.size() &&  
         (wsFileName[wsFileName.length() - 1] == L'.' || wsFileName[wsFileName.length() - 1] == L' '))
      {  
         // filename ends with a '.' or ' ' char, remove it.
         wsFileName.erase(wsFileName.end() - 1); 
      }
   }
   return wsFileName; 
}


/* FixFileNameNtfs() - 
 *    This function will modify the fname passed to it, making the fname a
 *    valid filename for the operating system(s) specified by the os_type_flags.
 * 
 * SLB 27.jul.2023 CR.32574; Replacement string support added.
 * SLB 12.sep.2023 CR.32574.V02; bSlashesOk param added. This is needed so caller
 * can indicate if slashes should get replaced with replacement string. 
 * The CFileNameMaker class needed this feature.
------------------------------------------------------------------------------*/
void SpdUtils::FixFileNameNtfs(
   const SpdStreamable::CSpdCString &wsInputFileName, 
	const SpdStreamable::CSpdCString& wsReplacementString,									// CR.32574; added.
	SpdStreamable::CSpdCString &wsOutputFileName,
	bool bSlashesOk)																						// CR.32574.V02; added.
{
	// CR.32574; First check to see if there are no invalid chars. If so, then use
	// optimization of just copying input to output.
	size_t stPos = 0;
	for (; stPos < wsInputFileName.wcslen(); stPos++)
	{
		if (!IsFileCharOk(wsInputFileName[stPos], ostNTFS, bSlashesOk)) break;			// CR.32574.V02; modified.
	}
	
	if (stPos == wsInputFileName.wcslen())
	{
		// No invalid chars were found.
		wsOutputFileName = wsInputFileName;
	}
	else
	{
		// At least one invalid char exists in the input. 
		// Replace all invalid characters with the specified string.
		wsOutputFileName.clear();
		for (stPos = 0; stPos < wsInputFileName.wcslen(); stPos++)
		{
			if (IsFileCharOk(wsInputFileName[stPos], ostNTFS, bSlashesOk))
				wsOutputFileName += wsInputFileName[stPos];
			else
				wsOutputFileName += wsReplacementString;
		}
	}

   // invalid chars are taken care of... now
   // search for reserved os names.
   for (int i = 0; reserveddosnames[i]; i++)
   {  
      // SLB 11.jun.2005 CR.4028.Item56 - Turns out that NTFS file names can(at least on XP)
      // begin with a reserved name(unlike DOS file names).  Modified the code to test
      // differently for DOS or NTFS file naming type.
      if (wsOutputFileName == CSpdCString(reserveddosnames[i]))
      {  
         // wsOutputFileName begins with a reserved filename(DOS)
         // or wsOutputFileName == a reserved filename(NTFS)
         wsOutputFileName[0] = L'$'; 
         break; 
      }
   }

   // NTFS file names, especially, should not end with a '.' char or a ' ' char
   while (  
      wsOutputFileName.wcslen() &&  
      (wsOutputFileName[wsOutputFileName.wcslen() - 1] == L'.' || wsOutputFileName[wsOutputFileName.wcslen() - 1] == L' '))
   {  
      // filename ends with a '.' or ' ' char, remove it.
      wsOutputFileName.erase(wsOutputFileName.wcslen() - 1); 
   }
}



/* newStr() -
 * 
 * SLB 25.sep.97 added space char.
------------------------------------------------------------------------------*/
char * SpdUtils::newStr(const char *sz)
{
   size_t stBufLen(strlen(sz)+1); 
   char *nsz = new char[stBufLen]; 

   if (nsz)
      strcpy_s(nsz, stBufLen, sz);																	// CR.13870; modified to use safe version 
   return nsz; 
} // newStr()

/* GetFullPath() - 
--------------------------------------------------------------------------------*/
void SpdUtils::GetFullPath(
   std::wstring &wsNewPath) 
{
   if (wsNewPath.empty()) return;

   std::wstring wsDrive;
   std::wstring wsPath; 
   std::wstring wsTmp1; 
   std::wstring wsTmp2; 
   std::wstring wsTmp3; 
   std::wstring wsMyDir; 

   // Split path name up into its elements.  note, only interested
   // in drive letter here.
   fnsplit(wsNewPath, wsDrive, wsTmp1, wsTmp2, wsTmp3); 

   //check if full path
   if (wsDrive.length() > 0) 
   {  
      // check if relative to root or current directory
      wchar_t wcszMyDir[256]; 
      memset(wcszMyDir, 0, sizeof(wcszMyDir)); 
      // Initialize startup directory string with the current directory.
      if (GetCurrentDirectoryW(sizeof(wcszMyDir)/sizeof(wchar_t) - 1, wcszMyDir))
      {
         std::wstring wsMyDir(wcszMyDir); 

      	fnsplit(wsMyDir, wsDrive, wsPath, wsTmp1, wsTmp2);
      }
      if (wsNewPath[0] != '\\')  
      {  
         //if ( newpath[0] != '\\' ) //relative to current directory
      	wsPath += wsNewPath; 
      }
      else
      {
      	wsPath = wsNewPath;  
      }

      fnmerge(wsNewPath, wsDrive, wsPath, L"", L"");
   }

   // Now remove trailing \ from path name.
   if (*(wsNewPath.end() - 1) == L'\\') 
      wsNewPath.erase(wsNewPath.end() - 1, wsNewPath.end()); 
}



/* CreateFolder() - 
 *
 * SLB 17.jul.2020 CR.29523; Cleaned up method.
--------------------------------------------------------------------------------*/
bool SpdUtils::CreateFolder(
   const std::wstring &wsNewPath, 
   const LPSECURITY_ATTRIBUTES psaSecAttribs)
{
   DWORD dwNtErr(0); 
   CNtfsFolderName fnFolder(wsNewPath.c_str());
   if (fnFolder.DoesFolderExist(dwNtErr)) return true;

   // Folder does not exist.. Create it.
   bool bRes(false);
   bRes = fnFolder.CreateFolder(dwNtErr, psaSecAttribs);
   return bRes;
} 


/* wsplitpath() - an stl version of the crt _wsplitpath() routine.
 * SLB 08.may.2004 - created. At this time this is just a wrapper arount the crt 
 * version of the function.
---------------------------------------------------------------------------*/
void SpdUtils::wsplitpath(   
   const std::wstring &wsPath, 
   std::wstring &wsDrive,
   std::wstring &wsDir,
   std::wstring &wsFName,
   std::wstring &wsExt)
{
   wchar_t szDrive[MAX_PATH]; 
   wchar_t szDir[MAX_PATH*2]; 
   wchar_t szFName[MAX_PATH*2]; 
   wchar_t szExt[MAX_PATH*2]; 

   szDrive[0] = 0; 
   szDir[0] = 0; 
   szFName[0] = 0; 
   szExt[0] = 0; 
   _wsplitpath_s(                         // CR.13870; modified to use safe version 
      wsPath.c_str(),
      szDrive,
      sizeof(szDrive) / sizeof(wchar_t),    // CR.13870; added.    
      szDir,
      sizeof(szDir) / sizeof(wchar_t),      // CR.13870; added.  
      szFName,
      sizeof(szFName) / sizeof(wchar_t),    // CR.13870; added.  
      szExt,
      sizeof(szExt) / sizeof(wchar_t));       // CR.13870; added.  
   wsDrive = szDrive; 
   wsDir = szDir; 
   wsFName = szFName; 
   wsExt = szExt; 
}

/* wmakepath() - an stl version of the crt _wmakepath() routine.
 * SLB 08.may.2004 - created. At this time this is just a wrapper arount the crt 
 * version of the function.
---------------------------------------------------------------------------*/
void SpdUtils::wmakepath( 
   std::wstring &wsPath,
   const std::wstring &wsDrive,
   const std::wstring &wsDir,
   const std::wstring &wsFName,
   const std::wstring &wsExt)
{
   wchar_t szPath[MAX_PATH*2]; 

   _wmakepath_s(        // CR.13870; modified to use safe version 
      szPath, sizeof(szPath)/sizeof(wchar_t),
      wsDrive.c_str(),
      wsDir.c_str(),
      wsFName.c_str(),
      wsExt.c_str()); 
   wsPath = szPath; 
}

// ExtractFileDrive()
// Implemented as a STL of the borland ExtractFileDrive() method.
// SLB 01.jul.04 - created.
//-----------------------------------------------------------------------------
std::wstring SpdUtils::ExtractFileDrive(const std::wstring &wsPathName)
{
   std::wstring wsDrive; 
   std::wstring wsDir; 
   std::wstring wsFName; 
   std::wstring wsExt; 

	SpdUtils::wsplitpath(wsPathName, wsDrive, wsDir, wsFName, wsExt); 
	return wsDrive; 
}


// MaxFileNameLength() - Returns the max length of a job name, or directory for
//		the given drive string.
//---------------------------------------------------------------------------
DWORD SpdUtils::MaxFileNameLength(const std::wstring &wsDriveString)
{
   wchar_t wcBuf[MAX_PATH]; 
   DWORD dwLength(0); 
   DWORD dwFlags(0); 
   std::wstring wsDrive; 
   BOOL bGetVolInfo(false);                                        
   // JMF 02.mar.04 - This function will extract the root from both paths with mapped drives
   // and from paths which use UNC
   wsDrive = SpdUtils::ExtractFileDrive(wsDriveString); 
   wsDrive += L"\\"; // Required for GetVolumeInformation()

	bGetVolInfo = GetVolumeInformationW(	
      wsDrive.c_str(), 
		wcBuf, 
		sizeof(wcBuf)/sizeof(wchar_t), 
		NULL, 
		&dwLength, 
		&dwFlags, 
		wcBuf, 
		sizeof(wcBuf)/sizeof(wchar_t)); 
	if (bGetVolInfo)
    	return dwLength;
	else
    	return 0;
}



/* wstrupr()
 *    Convert the string s to upper case.
------------------------------------------------------------------------------*/
std::wstring SpdUtils::wstrupr(std::wstring& s)
{
   std::locale loc;

   for (std::wstring::iterator i = s.begin(), e = s.end(); i != e; i++)
      *i = std::toupper(*i, loc);
   return s;
}


// WildcardCompare()
//    Compare a string to a wildcard mask containing '*' and '?' characters.
//    Return true for a match, false otherwise.
//
// SLB 22.oct.2012 CR.547; This CR was generated under the SDI module but it had to do
// with job filtering. Essentially, wcsWildCmp would return true for the following:
// str = L"TEST.TXT", mask = "*TXT.". 
// Fix for case when end of str has been reached but not end of mask. Last char of mask
// is the '.' char and it is the only char of the mask left to match. Method was returning
// true in this case but should be false.
//-----------------------------------------------------------------------------
bool wcsWildCmp(const wchar_t* str, const wchar_t* mask)
{
   // If end of string is reached, there is a match only if all that's left
   // in the mask are '*' chars. We also a single '.' char to appear within
   // the '*' chars, since masks such as "*.*" and "s*.*" are common and need
   // to match file names without extensions.
   // CR.547; handle special case where all that is left in mask is a '.' char.
   if (*str == 0)
   {
      if (*mask == 0)
      {  
         // if both mask and str are at the end, test has succeeded.
         return true; 
      }
      else
      {
         bool dot_found = false;
         bool star_found = false;         // CR.547; added.

         while (*mask == L'*' || (*mask == L'.' && !dot_found))
         {
            if (*mask == L'*')            // CR.547; added.
               star_found = true;         // CR.547; added.
            if (*mask == L'.')
               dot_found = true;
            mask++;
         }
         //return (*mask == 0);           // CR.547; replaced.
         if (*mask == 0 && star_found)    // CR.547; modified.
            return true;                  // CR.547; modified.
      }
      return false; 
   }

   // If the end of the mask is reached before the end of the string, this is
   // not a match.
   if (*mask == 0)
      return false;

   switch (*mask)
   {
      case '*':
         return
            wcsWildCmp(str, mask+1) ||       // '*' matches 0 chars
            wcsWildCmp(str+1, mask+1) ||     // '*' matches 1 char
            wcsWildCmp(str+1, mask);         // '*' matches >1 char
      case '?':
         return wcsWildCmp(str+1, mask+1);   // '?' matches 1 char
      default:
         return (*str == *mask) ? wcsWildCmp(str+1, mask+1) : false;
   }
}


// WildcardCompare()
//-----------------------------------------------------------------------------
bool SpdUtils::WildcardCompare(
   const CSpdCString& wsStr, 
   const CSpdCString& wsMask, 
   bool case_sensitive)
{
   // Convert strings to upper case for case insensitive search.
   std::wstring str = wsStr.c_str();
   std::wstring mask = wsMask.c_str();
   if (!case_sensitive)
   {
      SpdUtils::wstrupr(str);
      SpdUtils::wstrupr(mask);
   }

   return wcsWildCmp(str.c_str(), mask.c_str());
}


// XmlEscapeString()
//-----------------------------------------------------------------------------
bool SpdUtils::XmlEscapeString(
   const wchar_t *pwcsIn, 
   wchar_t *pwcsOut, 
   size_t stOutBufLen, 
   size_t &stOutStringLen)
{
   // initialize output to invalid value.
   stOutStringLen = (size_t) - 1; 

   // make sure buffer ptrs are valid.
   if (!pwcsIn || !pwcsOut)
   {  
      sol_assert(pwcsIn && pwcsOut); 
      return false;
   }

   // begin search and replace.
   size_t stInPos(0); 
   size_t stOutPos(0); 
   for (stInPos = stOutPos = 0; pwcsIn[stInPos] && stOutPos < stOutBufLen - 1; stInPos++)
   {
      const wchar_t *pwcsNewOut(NULL); 
      size_t stOutLen(0); 

      // check for special characters
      wchar_t wcsNewBuf[256]; 
      wchar_t wcInChar = pwcsIn[stInPos]; 
      if (wcInChar < 0x20 && wcInChar != 0x09 && wcInChar != 0x0A && wcInChar != 0x0D)
      {
         // We don't put a leading zero for 0x0 - 0xF since the .net XML classes
         // don't do this.
         stOutLen = _snwprintf_s(wcsNewBuf, _TRUNCATE, L"&#x%X;", 
            (int)wcInChar); 
         pwcsNewOut = wcsNewBuf; 
      }
      else
      {
         switch (wcInChar)
         {
         case L'&': 
            stOutLen = 5; 
            pwcsNewOut =  L"&amp;"; 
            break; 
         case L'>':
            stOutLen = 4; 
            pwcsNewOut =  L"&gt;"; 
            break; 
         case L'<':
            stOutLen = 4; 
            pwcsNewOut =  L"&lt;"; 
            break; 
         case L'\"':
            stOutLen = 6; 
            pwcsNewOut =  L"&quot;"; 
            break; 
         case L'\'':
            stOutLen = 6; 
            pwcsNewOut =  L"&apos;"; 
            break; 
         default: 
            // Not a special character.
            // Take care of copyin the data here.
            stOutLen = 0; 
            pwcsOut[stOutPos] = wcInChar; 
            stOutPos++; 
            break; 
         }
      }

      if (pwcsNewOut && stOutBufLen > stOutPos+stOutLen)
      {  
         // take care of copying special character's escaped value here.
         wmemcpy(pwcsOut + stOutPos, pwcsNewOut, stOutLen); 
      }

      // update output position.
      // Note: this will only be non-zero for escaped characters.
      stOutPos += stOutLen; 
   }

   if (!(stOutPos < stOutBufLen - 1))
   {  
      // problem: output buffer was not large enough to include terminating 0.
      if (stOutBufLen) pwcsOut[0] = 0;
      return false;
   }

   pwcsOut[stOutPos] = 0; 
   stOutStringLen = stOutPos; 
   return true; 
}

// XmlUnescapeString()
//-----------------------------------------------------------------------------
bool SpdUtils::XmlUnescapeString(
   const wchar_t *pwcsIn, 
   wchar_t *pwcsOut, 
   size_t stOutBufLen, 
   size_t &stOutStringLen)
{
   bool bRetVal(false); 

   // initialize output to invalid value.
   stOutStringLen = (size_t)- 1; 
   if (!pwcsIn || !pwcsOut)
   {  
      // problem: invalid buffer object(s) specified.
      sol_assert(pwcsIn && pwcsOut); 
   }
   else
   {  
      size_t stInPos(0); 
      size_t stOutPos(0); 

      // begin search and replace.
      for (stInPos = stOutPos = 0; pwcsIn[stInPos] && stOutPos < stOutBufLen - 1; )
      {  
         // Note all escaped strings begin with the '&' char and are terminated by the ';' char.
         switch (pwcsIn[stInPos])
         {
         case L'&':
         {
            size_t stSubStrLen(0); 

            stSubStrLen = wcscspn(&pwcsIn[stInPos], L";") + 1;  // find position of next ';' char
            if (!wcsncmp(&pwcsIn[stInPos], L"&gt;", stSubStrLen))
            {
               pwcsOut[stOutPos++] = L'>'; 
               stInPos += 4; 
            }
            else if (!wcsncmp(&pwcsIn[stInPos], L"&lt;", stSubStrLen))
            {
               pwcsOut[stOutPos++] = L'<'; 
               stInPos += 4; 
            }
            else if (!wcsncmp(&pwcsIn[stInPos], L"&amp;", stSubStrLen))
            {
               pwcsOut[stOutPos++] = L'&'; 
               stInPos += 5; 
            }
            else if (!wcsncmp(&pwcsIn[stInPos], L"&quot;", stSubStrLen))
            {
               pwcsOut[stOutPos++] = L'\"'; 
               stInPos += 6; 
            }
            else if (!wcsncmp(&pwcsIn[stInPos], L"&apos;", stSubStrLen))
            {
               pwcsOut[stOutPos++] = L'\''; 
               stInPos += 6; 
            }
            else if (!wcsncmp(&pwcsIn[stInPos], L"&#x", stSubStrLen)) // very rare
            {
               int escChar; 
               if (
                  _snwscanf_s(&pwcsIn[stInPos], stSubStrLen, L"&#x%X;", &escChar) > 0  // CR.13870; modified to use safe version 
                  )
               {
                  pwcsOut[stOutPos++] = (wchar_t)escChar; 
                  stInPos += stSubStrLen; 
               }
            }
            else
            {
               pwcsOut[stOutPos++] = pwcsIn[stInPos++]; 
            }
            break;
         }
         default:
            pwcsOut[stOutPos++] = pwcsIn[stInPos++]; 
            break; 
         }
      }
      if (!(stOutPos < stOutBufLen - 1))
      {  
         // problem: output buffer was not large enough to include terminating 0.
         if (stOutBufLen)
            pwcsOut[0] = 0; 
         bRetVal = false; 
      }
      else
      {
         pwcsOut[stOutPos] = 0; 
         stOutStringLen = stOutPos; 
         bRetVal = true; 
      }
   }
   return bRetVal; 
}

// This map contains characters which must be escaped in XML, and their corresponding escape strings.
//
// SLB 21.nov.2019 CR.28624; Augmented list to include unprintable characters. For better performance,
// Moved Xml Escape encodings from a C++ array into a map because the number of entries is 
// significantly increased.
typedef std::map <wchar_t, CSpdCString> TEscapeEncodings;                          // CR.28624; added.
typedef TEscapeEncodings::const_iterator TItcEscapeEncodings;                       // CR.28624; added.

TEscapeEncodings g_EscapeEncodings =                                                // CR.28624; modified.
{
   { L'&',  L"&amp;"  },
   { L'>',  L"&gt;"   },
   { L'<',  L"&lt;"   },
   { L'"',  L"&quot;" },
   { L'\'', L"&apos;" },
   { 0x00,  L"&#0;"   },                                                            // CR.28624; added.
   { 0x01,  L"&#1;"   },                                                            // CR.28624; added.
   { 0x02,  L"&#2;"   },                                                            // CR.28624; added.
   { 0x03,  L"&#3;"   },                                                            // CR.28624; added.
   { 0x04,  L"&#4;"   },                                                            // CR.28624; added.
   { 0x05,  L"&#5;"   },                                                            // CR.28624; added.
   { 0x06,  L"&#6;"   },                                                            // CR.28624; added.
   { 0x07,  L"&#7;"   },                                                            // CR.28624; added.
   { 0x08,  L"&#8;"   },                                                            // CR.28624; added.
   { 0x09,  L"&#9;"   },                                                            // CR.28624; added.
   { 0x0A,  L"&#10;"  },                                                            // CR.28624; added.
   { 0x0B,  L"&#11;"  },                                                            // CR.28624; added.
   { 0x0C,  L"&#12;"  },                                                            // CR.28624; added.
   { 0x0D,  L"&#13;"  },                                                            // CR.28624; added.
   { 0x0E,  L"&#14;"  },                                                            // CR.28624; added.
   { 0x0F,  L"&#15;"  },                                                            // CR.28624; added.
   { 0x10,  L"&#16;"  },                                                            // CR.28624; added.
   { 0x11,  L"&#17;"  },                                                            // CR.28624; added.
   { 0x12,  L"&#18;"  },                                                            // CR.28624; added.
   { 0x13,  L"&#19;"  },                                                            // CR.28624; added.
   { 0x14,  L"&#20;"  },                                                            // CR.28624; added.
   { 0x15,  L"&#21;"  },                                                            // CR.28624; added.
   { 0x16,  L"&#22;"  },                                                            // CR.28624; added.
   { 0x17,  L"&#23;"  },                                                            // CR.28624; added.
   { 0x18,  L"&#24;"  },                                                            // CR.28624; added.
   { 0x19,  L"&#25;"  },                                                            // CR.28624; added.
   { 0x1A,  L"&#26;"  },                                                            // CR.28624; added.
   { 0x1B,  L"&#27;"  },                                                            // CR.28624; added.
   { 0x1C,  L"&#28;"  },                                                            // CR.28624; added.
   { 0x1D,  L"&#29;"  },                                                            // CR.28624; added.
   { 0x1E,  L"&#30;"  },                                                            // CR.28624; added.
   { 0x1F,  L"&#31;"  },                                                            // CR.28624; added.
};


/* XmlEscapeString() - puts escaped version of string in wsEscaped.
 * returns true if escaping occurred.
 * if bOnlySetOutputIfEscaped==true, then wsEscaped is not set if no escaping occurred.
 *
 * Note: This method breaks the rules of C++ by modifying the const input string if it has unprintable chars. 
 * This is done for performance reasons only.
 * 
 * SLB 20.jul.2016 CR.21572; Added feature to method to help with performance optimization. New parm
 * bOnlySetOutputIfEscaped added.
 * SLB 21.nov.2019 CR.28624; Made changes to use the new g_EscapeEncodings object. Fundamentally changed
 * how the scfFound list gets populated for performance reasons. 
 -----------------------------------------------------------------------------*/
bool SpdUtils::XmlEscapeString(                                                     // CR.21572; modified.
   const CSpdCString &wsUnescaped, 
   CSpdCString &wsEscaped,
   bool bOnlySetOutputIfEscaped)                                                    // CR.21572; added.
{
   // begin search and replace.
   // CR.28624; Now this loop checks each char of the unescaped buffer, individually, for chars which need to
   // be escaped. Uses a map for chars to be escaped and their corresponding encoding strings.
   typedef std::map <size_t, TItcEscapeEncodings> TSpecialCharsFound;               // CR.28624; modified.
   TSpecialCharsFound scfFound; 
   size_t stAddLength(0); 
   for (size_t i = 0; i < wsUnescaped.wcslen(); i++)
   {
      TItcEscapeEncodings itEncoding(g_EscapeEncodings.find(wsUnescaped[i]));
      if (itEncoding == g_EscapeEncodings.end()) continue; 

      // found a char which should be escaped. Add it to the "found" list.
      scfFound.emplace(i, itEncoding);     
      stAddLength += itEncoding->second.wcslen() - 1;
   }

   if (scfFound.empty()) 
   { 
      // No chars to escape. 
      if (!bOnlySetOutputIfEscaped) wsEscaped = wsUnescaped;                        // CR.21572; modified.

      return false;  
   } 

   // There is work to do. 
   wchar_t *pwcsTmp(new wchar_t [wsUnescaped.size() + stAddLength+1]); 
   size_t stWrtPos(0); 
   size_t stRdPos(0); 
   for (TSpecialCharsFound::const_iterator it(scfFound.begin()); it != scfFound.end(); it++)
   {
      // copy data buf first.
      size_t stCpyLen(it->first - stRdPos); 
      wmemcpy(pwcsTmp + stWrtPos, wsUnescaped.c_str() + stRdPos, stCpyLen); 
      if (stCpyLen) stWrtPos += stCpyLen;  
      stRdPos = it->first + 1; 

      // copy escape string
      const CSpdCString &wsReplace(it->second->second);                             // CR.28624; modified.
      wmemcpy(pwcsTmp + stWrtPos, wsReplace.c_str(), wsReplace.size()); 
      stWrtPos += wsReplace.size(); 
   }
   if (stRdPos < wsUnescaped.size())
   {
      // There is a leftover data buf segment to copy (occurs if string does not end with an escape char)
      size_t stCpyLen(wsUnescaped.size() - stRdPos); 
      wmemcpy(pwcsTmp + stWrtPos, wsUnescaped.c_str() + stRdPos, stCpyLen); 
      stWrtPos += stCpyLen;  
   }
   pwcsTmp[stWrtPos] = 0; 
   wsEscaped.assign(pwcsTmp); 
   delete pwcsTmp;                                                                  // CR.Fix.22146.v01: Fixed memory leak.
   return true;                                                                     // CR.21572; added.
}


/* XmlUnescapeString()
 *
 * SLB 28.apr.2017 CR.22874; Minor optimization made to call emplace() vs. insert(). 
 * SLB 21.nov.2019 CR.28624; Made changes to use the new g_EscapeEncodings object.
-----------------------------------------------------------------------------*/
void SpdUtils::XmlUnescapeString(const CSpdCString &wsEscaped, CSpdCString &wsUnescaped)
{
   // begin search and replace.
   typedef std::map <size_t, TItcEscapeEncodings> TSpecialCharsFound;               // CR.28624; modified.
   TSpecialCharsFound scfFound; 
   size_t stSubLength(0); 
   for (TItcEscapeEncodings it(g_EscapeEncodings.begin()); it!= g_EscapeEncodings.end(); it++) // CR.28624; modified.
   {
      // Unescaped string has been determined...  the loop variable, it, does this.
      //
      // The following loop searches for all instances of the current escaped string...
      // it->second.  if the escaped string is found, its position is stored in the scfFound list and the 
      // subtract length value is updated.
      size_t stEscapePos(0);
      for (size_t stEscapePos = wsEscaped.find(it->second);                         // CR.28624; modified.
         stEscapePos != CSpdCString::npos;
         stEscapePos = wsEscaped.find(it->second, stEscapePos + 1))                 // CR.28624; modified.
      {
         scfFound.emplace(stEscapePos, it);                                         // CR.28624; modified. CR.22874; modified.
         stSubLength += it->second.wcslen() - 1;                                    // CR.28624; modified.
      }
   }

   if (scfFound.empty()) { wsUnescaped = wsEscaped; return; }

   // There is work to do. 
   wchar_t *pwcsTmp(new wchar_t [wsEscaped.wcslen() - stSubLength + 1]);
   size_t stWrtPos(0); 
   size_t stRdPos(0); 
   for (TSpecialCharsFound::const_iterator it(scfFound.begin()); it != scfFound.end(); it++) 
   {
      size_t stCpyLen(it->first - stRdPos); 
      wmemcpy(pwcsTmp + stWrtPos, wsEscaped.c_str() + stRdPos, stCpyLen); 
      if (stCpyLen) stWrtPos += stCpyLen;  
      stRdPos = it->first + it->second->second.wcslen(); 

      const wchar_t &wcReplace(it->second->first);                                  // CR.28624; modified.
      pwcsTmp[stWrtPos++] = wcReplace;                                              // CR.28624; modified.
   }

   if (stRdPos < wsEscaped.wcslen())
   {
      // There is a leftover segment to copy (occurs if string does not end with an escape string)
      size_t stCpyLen(wsEscaped.wcslen() - stRdPos); 
      wmemcpy(pwcsTmp + stWrtPos, wsEscaped.c_str() + stRdPos, stCpyLen); 
      stWrtPos += stCpyLen;  
   }
   pwcsTmp[stWrtPos] = 0; 
   wsUnescaped.assign(pwcsTmp, stWrtPos);                                           // CR.28624; modified.
   delete pwcsTmp;                                                                  // CR.Fix.22146.v01: Fixed memory leak.
}

SpdStreamable::CSpdCString SpdUtils::XmlEscapeString(
   const SpdStreamable::CSpdCString &wsUnescaped) 
{
   SpdStreamable::CSpdCString ws; 
   XmlEscapeString(wsUnescaped, ws); 
   return ws; 
}
SpdStreamable::CSpdCString SpdUtils::XmlUnescapeString(
   const SpdStreamable::CSpdCString &wsEscaped) 
{
   SpdStreamable::CSpdCString ws; 
   XmlUnescapeString(wsEscaped, ws); 
   return ws; 
}

/* GetWin32ErrorMessage() - 
 *
 * SLB 28.apr.2017 CR.22874; Modified format in which NT errors are displayed to be consistent with
 * other parts of SPDE code. 
 * SLB 20.feb.2018 CR.17862.V07; Fixed up to support retrieving messags for both NtErrors and 
 * HRESULTS.
 -----------------------------------------------------------------------------------------*/
void SpdUtils::GetWin32ErrorMessage(
   std::wstring &wsMsg, 
   DWORD dwErrCode)
{
   // clear message to begin with.
   wsMsg.clear();

   // Get last error for the calling thread.
   if (!dwErrCode) dwErrCode = GetLastError();
   if (!dwErrCode) return;

   // Get error code's message.
   wchar_t *pwcErrMsg(NULL);
   void *ppErrMsg(&pwcErrMsg);
   DWORD dwMsgSize(0);
   dwMsgSize = FormatMessageW(
      FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
      NULL,
      dwErrCode,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      (wchar_t *)ppErrMsg, 0,
      NULL);
   if (!dwMsgSize) return;
   if (!pwcErrMsg) return;

   // initialize header, either for an HRESULT or NT error
   wchar_t wcHeader[32];
   if (dwErrCode & 0x80000000)
      _snwprintf_s(wcHeader, _TRUNCATE, L"(%08X): ", dwErrCode);
   else
      _snwprintf_s(wcHeader, _TRUNCATE, L"(NT%04u): ", dwErrCode);

   // Construct message.
   wsMsg = std::wstring(wcHeader) + std::wstring(pwcErrMsg); 

   // Free memory allocated by FormatMessage.
   LocalFree(pwcErrMsg); 

   // Replace cr/lf with spaces.
   wchar_t pCrLf[] = { 0x000D, 0x000A, 0x0000 }; //cr/lf
   size_t stSearchStartPos(0);
   stSearchStartPos = wsMsg.find(pCrLf, stSearchStartPos, 2); 
   if (stSearchStartPos != std::wstring::npos)
   {
      wsMsg.replace(stSearchStartPos, 2, L""); 
   }
}

// SLB 11.nov.2008 Added to aid in creating some exception handling code.
void SpdUtils::GetWin32ErrorMessage(
   wchar_t *pwcsMsg, 
   DWORD dwStringLen, 
   DWORD dwNtErr)
{
   if (!pwcsMsg || !dwStringLen) return;

   std::wstring wsMsg; 
   GetWin32ErrorMessage(wsMsg, dwNtErr); 
   _snwprintf_s(pwcsMsg, dwStringLen, _TRUNCATE, L"%s",
      wsMsg.c_str()); 
}

SSILIB1_EXPORT SpdStreamable::CSpdCString SpdUtils::GetWin32ErrorMessage(DWORD dwNtErr)
{
   std::wstring wsMsg;
   GetWin32ErrorMessage(wsMsg, dwNtErr);
   return wsMsg;
}

SSILIB1_EXPORT SpdStreamable::CSpdCString SpdUtils::GetHRErrorMessage(HRESULT hr)
{
   CSpdCString wsMsg/*(CSpdHResult::GetDescription(hr))*/; 
   if (!wsMsg.empty()) return wsMsg; 

   if (HRESULT_FACILITY(hr) == FACILITY_WIN32) return GetWin32ErrorMessage(HRESULT_CODE(hr)); 
   if (hr && FACILITY_NT_BIT) return GetWin32ErrorMessage(HRESULT_CODE(hr));  
   return wsMsg; 
}
   
/* tolower() - returns lowercase version of std::wstring passed to it.
 *
 * SLB 12.sep.2013 CR.17124.Item46; Created.
-------------------------------------------------------------------------------------------------*/
std::wstring SpdUtils::tolower(const std::wstring &ws)
{
   std::wstring wsNew(ws); 

   // iterate through each char and convert to lowercase.
   for (std::wstring::iterator it(wsNew.begin()); it != wsNew.end(); it++)
   {
      *it = ::tolower(*it); 
   }
   return wsNew; 
}

#pragma warning (push)
#pragma warning (disable: 4793)


/* ThrowHr() -
 *
 * SLB 15.may.2018 CR.17862.V07; Created.
 * SLB 27.sep.2018 CR.18557.V02; Moved this from the CSpdFWRegistered class to be under the SpdUtils 
 * namespace... This makes it more accessible to different code bases.
 * SLB 26.feb.2020 CR.28354; Fix for not throwing exception when format string is empty.
 -------------------------------------------------------------------------------------------*/
void SpdUtils::ThrowHr(
   HRESULT hr, 
   const wchar_t * pwcsFunc, 
   const wchar_t * pwcsFormat, 
   ...)
{
   if (!pwcsFormat) return;

   // If format string is empty, just throw exception with empty string for message parm.
   if (!pwcsFormat[0]) throw (SpdException::hr_exception(hr, L"", pwcsFunc));

   wchar_t wcsMsg[2048];
   int nSize;
   try
   {
      // Format the message.
      va_list pArg;
      va_start(pArg, pwcsFormat);
      nSize = _vsnwprintf_s(wcsMsg, _TRUNCATE, pwcsFormat, pArg);
      va_end(pArg);
   }
   catch (...)
   {
      nSize = - 1;
   }

   if (nSize < 0) CEnterExitMsg::TraceMsg(L"%s() Internal error occurred formatting string.", __WFUNCTION__);

   // build and throw the exception
   throw (SpdException::hr_exception(hr, wcsMsg, pwcsFunc));
}


//
// This 2-D array contains XML escape sequences and there corresponding un-escaped values.
//
const std::wstring wsNonPrintableStrings[][2] =
{
	{ std::wstring(L"&#x1"), std::wstring(1, (wchar_t)1) },
	{ std::wstring(L"&#x2"), std::wstring(1, (wchar_t)2) },
	{ std::wstring(L"&#x3"), std::wstring(1, (wchar_t)3) },
	{ std::wstring(L"&#x4"), std::wstring(1, (wchar_t)4) },
	{ std::wstring(L"&#x5"), std::wstring(1, (wchar_t)5) },
	{ std::wstring(L"&#x6"), std::wstring(1, (wchar_t)6) },
	{ std::wstring(L"&#x7"), std::wstring(1, (wchar_t)7) },
	{ std::wstring(L"&#x8"), std::wstring(1, (wchar_t)8) },
	//{ std::wstring(L"&#x9"), std::wstring(1, (wchar_t)9) },
	//{ std::wstring(L"&#xA"), std::wstring(1, (wchar_t)10) },
	{ std::wstring(L"&#xB"), std::wstring(1, (wchar_t)11) },
	{ std::wstring(L"&#xC"), std::wstring(1, (wchar_t)12) },
	//{ std::wstring(L"&#xD"), std::wstring(1, (wchar_t)13) },
	{ std::wstring(L"&#xE"), std::wstring(1, (wchar_t)14) },
	{ std::wstring(L"&#xF"), std::wstring(1, (wchar_t)15) },
	{ std::wstring(L"&#x10"), std::wstring(1, (wchar_t)16) },
	{ std::wstring(L"&#x11"), std::wstring(1, (wchar_t)17) },
	{ std::wstring(L"&#x12"), std::wstring(1, (wchar_t)18) },
	{ std::wstring(L"&#x13"), std::wstring(1, (wchar_t)19) },
	{ std::wstring(L"&#x14"), std::wstring(1, (wchar_t)20) },
	{ std::wstring(L"&#x15"), std::wstring(1, (wchar_t)21) },
	{ std::wstring(L"&#x16"), std::wstring(1, (wchar_t)22) },
	{ std::wstring(L"&#x17"), std::wstring(1, (wchar_t)23) },
	{ std::wstring(L"&#x18"), std::wstring(1, (wchar_t)24) },
	{ std::wstring(L"&#x19"), std::wstring(1, (wchar_t)25) },
	{ std::wstring(L"&#x1A"), std::wstring(1, (wchar_t)26) },
	{ std::wstring(L"&#x1B"), std::wstring(1, (wchar_t)27) },
	{ std::wstring(L"&#x1C"), std::wstring(1, (wchar_t)28) },
	{ std::wstring(L"&#x1D"), std::wstring(1, (wchar_t)29) },
	{ std::wstring(L"&#x1E"), std::wstring(1, (wchar_t)30) },
	{ std::wstring(L"&#x1F"), std::wstring(1, (wchar_t)31) },
	{ std::wstring(L""),    std::wstring(L"")  } // array terminator.
};
// ConvertNonPrintableToHex()
//-----------------------------------------------------------------------------
SSILIB1_EXPORT std::wstring __stdcall SpdUtils::ConvertNonPrintableToHex(
	const std::wstring& wsRawString)
{
	std::wstring wsRetVal;
	size_t i(0);
	//
	// begin search and replace.
	//
	wsRetVal = wsRawString;
	for (
		i = 0;
		wsNonPrintableStrings[i][1].length();
		i++
		)
	{
		size_t stEscapePos(0);
		//
		// special char which needs to be escaped has been determined...  the loop 
		// variable, i, does this.
		//
		// The following loop searches for all instances of this char...
		// (wsEscapedStrings[i][1]).  When this char is found, it is replaced
		// with the corresponding escaped string.
		//
		for (stEscapePos = wsRetVal.find(wsNonPrintableStrings[i][1]);
			stEscapePos != std::wstring::npos;
			stEscapePos = wsRetVal.find(wsNonPrintableStrings[i][1], stEscapePos))
		{
			//
			// At the position where the special char was found, replace the
			// char with the corresponding escape string (wsEscapedStrings[i][0])
			//
			wsRetVal.replace(
				stEscapePos,
				wsNonPrintableStrings[i][1].length(),
				wsNonPrintableStrings[i][0]
			);
			//
			// update the search position to be beyond the replacement string.
			//
			stEscapePos += wsNonPrintableStrings[i][0].length();
		}
		//
		// All instances of the current special char have been replaced.
		// continue to search and replace the next escape string.
		//
	}

	return wsRetVal;
}

#pragma warning (pop)
