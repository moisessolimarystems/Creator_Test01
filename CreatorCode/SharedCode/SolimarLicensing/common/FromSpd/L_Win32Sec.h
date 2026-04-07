#pragma once
#include "L_SSILib1Dll.h"

#include <atlcomcli.h>
#include <sddl.h>
#include <comdef.h>
#include <authz.h>
#include <atlsecurity.h>
#include <lm.h>
#include <dsgetdc.h>                                                                // CR.8222; added.
#include <vector>
#include <string>
#include <set>                                                                      // CR.18485.V01; added.
#include "L_SpdException.h"
#ifndef SPDUTILS_IMP_EXP
#endif

#ifndef WIDEN                                                                       // CR.17862; added.
#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)
#define __WFUNCTION__ WIDEN(__FUNCTION__)
#endif

using namespace ATL;
using namespace SpdException;                                                       // CR.18485; added.
using namespace SpdUtils;                                                           // CASE.723895; added.
//#define SECURITY_OUTPUT_DEBUG // uncomment for verbose debug messages

namespace Win32Sec
{

   //
   // Helper method to convert HRESULT back into Win32 error value.
   //
   // SLB 14.apr.2008 CR.9374; Added.
   // SLB 20.feb.2014 CR.17478; Moved out of class and made this a Win32Sec namespace
   // method.
   // SLB 16.jan.2014 CR.17862; Modified so that WIN32_FROM_HRESULT() methods are now defined 
   // in a Win32Sec.cpp file. This results in project's, which use these methods, needing to include 
   // the Win32Sec.cpp file in their source file list.
   //
   bool WIN32_FROM_HRESULT(HRESULT hr, DWORD *pdwWin32);                            // CR.17862; modified.
   
                                                                                    //
   // Helper method to convert HRESULT back into Win32 error value.
   //
   // SLB 20.feb.2014 CR.17478; added.
   //
   DWORD WIN32_FROM_HRESULT(HRESULT hr);                                            // CR.17862; modified.

   /*
    * class CSmartNetApiBuf
    *
    * This class wraps a buffer ptr allocated by one of the NetApi methods so that the buffer gets cleaned up 
    * according to the requirements of NetApi Buffers.
    *
    * SLB 09.mar.2020 CR.29058.V01; created.
    * SLB 18.jan.2023 CR.32082; changes in "protected" constructors to eliminate
    * compiler warning msg 26495.
    */
   class CSmartNetApiBuf
   {
   private:
      void *m_pBuf;
      
      // Hide default constructor. It should never be used.
   private:
      CSmartNetApiBuf() : m_pBuf(NULL) {}                                           // CR.32082; modified.

   public:
      CSmartNetApiBuf(void *p) : m_pBuf(p) {}
      ~CSmartNetApiBuf()
      {
         if (m_pBuf) NetApiBufferFree(m_pBuf);
         m_pBuf = NULL;
      }
   };

   //
   // A Win32SecError object represents an exception condition detected by
   // the Win32Sec functions. This class encapsulates an HRESULT error code
   // and an associated message describing the error.
   // 11.nov.2008 SLB - added a second text field which can have additional info.
   //
   class Win32SecError
   {
   private:
      HRESULT hr;
      wchar_t message[512];
      wchar_t message2[256];                                                        // 11.nov.2008 SLB

   public:
      Win32SecError(HRESULT _hr, const wchar_t* msg, const wchar_t* msg2=NULL) throw()
      {
         hr = _hr;
#if _MSC_VER < 1400
         wcscpy(message, msg);
         if (msg2)
            wcscpy(message2, msg2);  // 11.nov.2008 SLB
         else
            *message2 = 0;
#else
         wcscpy_s(message, msg);
         if (msg2)
            wcscpy_s(message2, msg2);  // 11.nov.2008 SLB
         else
            *message2 = 0;
#endif
      }

      Win32SecError(const Win32SecError& that) throw()
      {
         hr = that.hr;
#if _MSC_VER < 1400
         wcscpy(message, that.message);
         wcscpy(message2, that.message2);                                           // 11.nov.2008 SLB
#else
         wcscpy_s(message, that.message);
         wcscpy_s(message2, that.message2);                                         // 11.nov.2008 SLB
#endif
      }

      Win32SecError& operator =(const Win32SecError& that) throw()
      {
         if (this != &that)
         {
            hr = that.hr;
#if _MSC_VER < 1400
            wcscpy(message, that.message);
            wcscpy(message2, that.message2);                                        // 11.nov.2008 SLB
#else
            wcscpy_s(message, that.message); 
            wcscpy_s(message2, that.message2);                                      // 11.nov.2008 SLB
#endif
         }
         return *this;
      }

      HRESULT GetHRESULT() const throw()
      {
         return hr;
      }

      const wchar_t* GetMessage() const throw()
      {
         return message;
      }

      const wchar_t* GetMessage2() const throw()                                    // 11.nov.2008 SLB
      {
         return message2; 
      }

   };

#pragma warning( push )                                                             // CR.32082.V00.01; added.
#pragma warning( disable:4793 )                                                     // CR.32082.V00.01; added.

   /* ThrowE() - Builds and throws a Win32SecError object from the format list provided.
    * The Message string in the exception will have: "Func(parm0) - " pre-pended to it.
    *
    * SLB 06.feb.2020 CASE.723895; Created from ThrowHr() code located in SpdUtils namespace.
    -------------------------------------------------------------------------------------------*/
   static void ThrowE(
      HRESULT hr,
      const wchar_t * pwcsFunc,
      const wchar_t * pwcsParm0,
      const wchar_t * pwcsFormat,
      ...)
   {
      if (!pwcsFormat || !pwcsParm0 || !pwcsFormat[0]) return;

      // Do additional formatting of output. Should look like: "Func(parm0) - " + Format string.
      wchar_t wcsFormat[512];
      _snwprintf_s(wcsFormat, _TRUNCATE, L"%s(%s) - %s", pwcsFunc, pwcsParm0, pwcsFormat);

      wchar_t wcsMsg[2048];
      int nSize;
      try
      {
         // Format the message.
         va_list pArg;
         va_start(pArg, pwcsFormat);                                                // Set pArg to the first stack parm AFTER pwcsFormat
         nSize = _vsnwprintf_s(wcsMsg, _TRUNCATE, wcsFormat, pArg);
         va_end(pArg);
      }
      catch (...)
      {
         nSize = -1;
      }

      if (nSize < 0) CEnterExitMsg::TraceMsg(L"%s() Internal error occurred formatting string.", __WFUNCTION__);

      // build and throw the exception
      throw (Win32SecError(hr, wcsMsg));
   }

#pragma warning (pop)                                                               // CR.32082.V00.01; added.

   typedef std::set<CSpdCString> TSidSet;                                           // CR.29142; added.
   typedef TSidSet::iterator TItSidSet;                                             // CR.29142; added.
   typedef TSidSet::const_iterator TItcSidSet;                                      // CR.29142; added.


   /*
    * class SIDPtr
    *
    * SLB 11.mar.2020 CR.29142; Restructured code sod that SIDPtr could use/take advantage of the 
    * CSecurityContext class (in new method GetGroups2().
    * This entailed splitting out code from class definition.
    * SLB 18.nov.2021 CR.30540; Added const modifier to some methods. Needed for 
    * calling code chages.
    */
   class SIDPtr : public CSid
   {
   public:
      // CR.29142; renamed and changed containter type from vector<> to set <>.
      typedef std::set<SIDPtr> TSIDObjList;                                         // CR.29142; modified. CASE.723895; added.
      typedef TSIDObjList::iterator TItSIDObjList;                                  // CR.29142; modified. CASE.723895; added.
      typedef TSIDObjList::const_iterator TItcSIDObjList;                           // CR.29142; modified. CASE.723895; added.

   public:
      SIDPtr();
      SIDPtr(const SIDPtr &c);

      // SLB 31.aug.2007 CR.6135; Added. 
      SIDPtr(const SID *pSid, LPCTSTR pwcsSystem = NULL);
      SIDPtr(const wchar_t *sid_str);

      
      operator _bstr_t() const;                                                      // CASE.723895; modified.

      bool IsValid() const;                                                          // CASE.723895; modified.

      TRUSTEE Trustee();

      // EnumToString() 
      //    returns a string description of the enum value.
      //
      static const wchar_t *EnumToString(SID_NAME_USE val);                          // CASE.723895; modified.

      // GetSidNameUse()
      //    Returns a SID_NAME_USE value that indicates the type of account
      //    for this user. Some interesting SID_NAME_USE values are:
      //       SidTypeUser
      //          User account.
      //       SidTypeGroup
      //          Global group, such as Domain Admins. SidTypeGroup can contain
      //          only SidTypeUser as members.
      //       SidTypeAlias
      //          Local (SID prefix + RID) or built-in (S-1-5-32 + RID) group,
      //          such as Administrators. SidTypeAlias can contain SidTypeGroup
      //          or SidTypeUser as members. 
      //       SidTypeWellKnownGroup
      //          A hardcoded group, such as Everyone, System, or Authenticated
      //          Users.
      //
      SID_NAME_USE GetSidNameUse() const;                                           // CR.30540; modified.

      // AddToLocalGroup()
      //    Add this user or global group to the specified local group. If
      //    this SID is already in the local group, an error is not returned.
      //    If throw_win32sec_error is true and an error occurs, the function
      //    throws a Win32SecError exception rather than returning the invalid
      //    HRESULT.
      //
      HRESULT AddToLocalGroup(
         LPCWSTR group_name,
         bool throw_win32sec_error);

      // RemoveFromLocalGroup()
      //    Remove this user or global group from the specified local group.
      //    If this SID is not in the local group to begin with, an error is
      //    not returned. If throw_win32sec_error is true and an error occurs,
      //    the function throws a Win32SecError exception rather than returning
      //    the invalid HRESULT.
      // 
      HRESULT RemoveFromLocalGroup(
         LPCWSTR group_name,
         bool throw_win32sec_error);
            
      // UserGetGroupsNetApi() - 
      //    Gets the local machine groups that this user belongs to, using the 
      //    NetApi methods. Sets group_list with an array of SID string representing
      //    the groups.
      //    If throw_win32sec_error is true and an error occurs, the function
      //    throws a Win32SecError exception rather than returning the invalid
      //    HRESULT.
      // 
      // Formerly UserGetGroups().
      //
      HRESULT UserGetGroupsNetApi(
         TSidSet& group_list,
         bool throw_win32sec_error) const;                                          // CR.30540; modified.


      // UserGetGroupsAuthz()
      //    Gets the local, global, and universal groups that this user belongs
      //    to, using the Authz API methods. Sets group_list with an array of SID 
      //    string representing the groups.
      //    Results of this method depend on the process Id having permissions 
      //    on domain controller. If it is a local machine SID, only local machine
      //    group membership is returned.
      //    Always throws a Win32sec_error on error.
      //
      void UserGetGroupsAuthz(
         TSidSet& group_list);


      // GetUserName()
      //    Builds a fully qualified user name for this SID.
      //
      CSpdCString GetQualifiedUserName() const;                                    // CASE.723895; modified.

      // LocalGroupGetMembers()
      //    Gets a list of the members of the specified local group. Local group
      //    members can be users or global groups. Sets member_list with an array
      //    of SIDPtrs representing the members. If throw_win32sec_error is true
      //    and an error occurs, the function throws a Win32SecError exception
      //    rather than returning the invalid HRESULT.
      //
      static HRESULT LocalGroupGetMembers(
         LPCWSTR group_name,
         TSIDObjList& member_list,
         bool throw_win32sec_error);

      // GetProcessSID()
      // Retrieves the SID for the process.
      // 
      static SIDPtr GetProcessSID();

      // GetProcessUserName() - Retrieves the fully qualified user name of the current running process.
      // 
      // Throws Win32SecError object on error.
      // 
      // SLB 18.mar.2011 CALL.514725; Added this simple but helpful method
      static CSpdCString GetProcessUserName();

      // DebugPrint() - Goes through SID list and prints formatted info about it to the debug stream.
      //
      static void DebugPrint(
         const TSidSet &list,
         const CSpdCString &wsListDescription);
   };

   typedef SIDPtr::TSIDObjList TSIDList;                                               // CASE.723895; added.
   typedef SIDPtr::TItSIDObjList TItSIDList;                                           // CASE.723895; added.
   typedef SIDPtr::TItcSIDObjList TItcSIDList;                                         // CASE.723895; added.


   /*
    * class CSecurityContext
    * This class maintains a security context which can then be used to check access for
    * security descriptors passed to it.
    *
    * SLB 03.mar.2009 CR.11156; Created to help performance of folder access validation.
    * SLB 11.mar.2020 CR.29142; Restructured code sod that SIDPtr could use/take advantage of this 
    * class. 
    * This entailed splitting out code from class definition and moving class definition in front of 
    * code definitions fo the SIDPtr class.
    */
   class CSecurityContext
   {
   private:
      AUTHZ_RESOURCE_MANAGER_HANDLE m_authz_resource_manager;
      AUTHZ_CLIENT_CONTEXT_HANDLE m_authz_client;
      const SIDPtr &m_sid;                                                          // CASE.723895; added.

   public:
      CSecurityContext(const SIDPtr &sid);
      ~CSecurityContext();

      bool IsValid() const;

      // WIN32_FROM_HRESULT()
      // Helper method to convert HRESULT back into Win32 error value.
      //
      static bool WIN32_FROM_HRESULT(
         HRESULT hr,
         DWORD *pdwWin32);

      // GetEffectivePermissions()
      //    If throw_win32sec_error is true and an error occurs, the function
      //    throws a Win32SecError exception rather than returning the invalid
      //    HRESULT.
      //
      HRESULT GetEffectivePermissions(
         PSECURITY_DESCRIPTOR psd,
         unsigned long desired_access_mask,
         unsigned long *granted_access_mask,
         bool throw_win32sec_error) const;

      // GetEffectivePermissions2() - Wrapper for GetEffectivePermissions() which return a Win32 error
      // code vs an HRESULT.
      //
      DWORD GetEffectivePermissions2(
         PSECURITY_DESCRIPTOR psd,
         unsigned long desired_access_mask,
         unsigned long *granted_access_mask) const;

      // UserGetGroups() - Gets all groups this context's SID is a member of, using the Authz API... This includes 
      // groups which are no longer in use.
      //
      void UserGetGroups(                                                           // CR.29142; added.
         TSidSet& group_list,
         bool recurse_groups) const;

   private:
      // InitializeAuthzContextData() 
      // This method is responsible for initializing m_authz_resource_manager and m_authz_client members.
      // 
      void InitializeAuthzContextData(
         const SIDPtr &sid);
   };



   //////////////////////////////////

   SIDPtr::SIDPtr() : CSid()
   {
      ; 
   }

	SIDPtr::SIDPtr(const SIDPtr &c) : CSid(c)
   {
      ; 
   }
   
   // SLB 31.aug.2007 CR.6135; Added. 
	SIDPtr::SIDPtr(const SID *pSid, LPCTSTR pszSystem) : CSid(pSid, pszSystem)
   {
   }

	SIDPtr::SIDPtr(const wchar_t *sid_str)
   {
      PSID psid = 0;
      if (ConvertStringSidToSidW(sid_str, &psid) && psid != 0)
      {
         LoadAccount((const SID*)psid);
      }
         
      if (psid)
      {
         LocalFree(psid);
         psid = 0;
      }
   }
      
	SIDPtr::operator _bstr_t() const                                                      // CASE.723895; modified.
   {
      return _bstr_t(Sid());
   }

	bool SIDPtr::IsValid() const                                                          // CASE.723895; modified.
   {
      return (0 != GetPSID() && IsValidSid((PSID)GetPSID()));
   }

	TRUSTEE SIDPtr::Trustee()
   {
      TRUSTEE trustee;
      memset(&trustee, 0, sizeof(TRUSTEE));
         
      if (!IsValid()) ThrowE(E_FAIL, __WFUNCTION__, L"", L"SIDPtr object is not valid.");
         
      BuildTrusteeWithSid(&trustee, (PSID)GetPSID());
      return trustee;
   }

   // EnumToString() 
   //    returns a string description of the enum value.
   //
	const wchar_t *SIDPtr::EnumToString(SID_NAME_USE val)                            // CASE.723895; modified.
   {
      switch (val)
      {
      case SidTypeUser: return L"User";
      case SidTypeGroup: return L"Group";
      case SidTypeDomain: return L"Domain";
      case SidTypeAlias: return L"Alias";
      case SidTypeWellKnownGroup: return L"Well known group";
      case SidTypeDeletedAccount: return L"Deleted account";
      case SidTypeInvalid: return L"Invalid";
      case SidTypeUnknown: return L"Unknown";
      case SidTypeComputer: return L"Computer";
      case SidTypeLabel: return L"Label";
      }
      return L"Undefined";
   }

   // GetSidNameUse()
   //    Returns a SID_NAME_USE value that indicates the type of account
   //    for this user. Some interesting SID_NAME_USE values are:
   //       SidTypeUser
   //          User account.
   //       SidTypeGroup
   //          Global group, such as Domain Admins. SidTypeGroup can contain
   //          only SidTypeUser as members.
   //       SidTypeAlias
   //          Local (SID prefix + RID) or built-in (S-1-5-32 + RID) group,
   //          such as Administrators. SidTypeAlias can contain SidTypeGroup
   //          or SidTypeUser as members. 
   //       SidTypeWellKnownGroup
   //          A hardcoded group, such as Everyone, System, or Authenticated
   //          Users.
   //
   // Remarks:
   //    The SidNameUse() function of CSid is strange. It only returns a
   //    valid value if its m_sidnameuse member has been set by a previous
   //    function call such as Domain() or AccountName(), or if the object
   //    was initialized using the account name in the constructor or
   //    LoadAccount(). We therefore need to define a function that returns
   //    a valid value under any circumstances.
   //
   // SLB 18.nov.2021 CR.30540; method is now const.
	SID_NAME_USE SIDPtr::GetSidNameUse() const                                       // CR.30540; modified.
   {
      // Calling AccountName() is the fastest way to ensure that SidNameUse()
      // will return a valid value.
      AccountName();

      return SidNameUse();
   }

   // AddToLocalGroup()
   //    Add this user or global group to the specified local group. If
   //    this SID is already in the local group, an error is not returned.
   //    If throw_win32sec_error is true and an error occurs, the function
   //    throws a Win32SecError exception rather than returning the invalid
   //    HRESULT.
   //
   // SLB 06.feb.2020 CASE.723895; Changed to use ThrowE() and added some parms to give helpful info.
   // SLB 12.oct.2022 CR.31786; Updated ThrowE() text to include Qualified Username 
   // and the name_use like the first ThrowE() does.
	HRESULT SIDPtr::AddToLocalGroup(
      LPCWSTR group_name, 
      bool throw_win32sec_error)
   {
      // This function only supports user and global group SIDs.
      SID_NAME_USE sid_name_use = GetSidNameUse();
      if (sid_name_use != SidTypeUser && sid_name_use != SidTypeGroup)
      {
         HRESULT hr = E_INVALIDARG;
         if (throw_win32sec_error) ThrowE(hr, __WFUNCTION__, Sid(), L"Function only supports user and global group SIDs. \nAccount<%s>  Accnt type<%s>", GetQualifiedUserName().c_str(), EnumToString(sid_name_use)); 
         return hr;
      }

      LOCALGROUP_MEMBERS_INFO_0 local_group_members;
      local_group_members.lgrmi0_sid = (PSID)GetPSID();
      NET_API_STATUS net_api_status = NetLocalGroupAddMembers(
         NULL, group_name, 0, (LPBYTE)&local_group_members, 1);
      if (net_api_status != NERR_Success && net_api_status != ERROR_MEMBER_IN_ALIAS)
      {
         HRESULT hr = HRESULT_FROM_WIN32(net_api_status);
         if (throw_win32sec_error) ThrowE(hr, __WFUNCTION__, Sid(), L"Problem occurred adding this SID from local group. \nGroup<%s>\nAccnt<%s> Accnt type<%s>", group_name, GetQualifiedUserName().c_str(), EnumToString(GetSidNameUse())); // CR.31876; modified.
         return hr;
      }

      return S_OK;
   }

   // RemoveFromLocalGroup()
   //    Remove this user or global group from the specified local group.
   //    If this SID is not in the local group to begin with, an error is
   //    not returned. If throw_win32sec_error is true and an error occurs,
   //    the function throws a Win32SecError exception rather than returning
   //    the invalid HRESULT.
   // 
   // SLB 06.feb.2020 CASE.723895; converted to use ThrowE() when throwing Win32SecError() objects. Also, now take advantage of string formatting 
   // provided by ThrowE(), so more variable info is in the message string.
   // SLB 12.oct.2022 CR.31786; Updated ThrowE() text to include Qualified Username 
   // and the name_use like the first ThrowE() does.
	HRESULT SIDPtr::RemoveFromLocalGroup(
      LPCWSTR group_name, 
      bool throw_win32sec_error)
   {
      if (!group_name) return S_OK;                                              // CASE.723895; added.

      // This function only supports user and global group SIDs.
      SID_NAME_USE sid_name_use = GetSidNameUse();
      if (sid_name_use != SidTypeUser && sid_name_use != SidTypeGroup)
      {
         HRESULT hr = E_INVALIDARG;
         if (throw_win32sec_error) ThrowE(hr, __WFUNCTION__, Sid(), L"Function only supports user or global group SIDs. \nAccnt<%s> Accnt type<%s>", GetQualifiedUserName().c_str(), EnumToString(GetSidNameUse())); // CR.31786; modified.
         return hr;
      }

      LOCALGROUP_MEMBERS_INFO_0 local_group_members;
      local_group_members.lgrmi0_sid = (PSID)GetPSID();
      NET_API_STATUS net_api_status = NetLocalGroupDelMembers(
         NULL, group_name, 0, (LPBYTE)&local_group_members, 1);
      if (net_api_status != NERR_Success && net_api_status != ERROR_MEMBER_NOT_IN_ALIAS)
      {
         HRESULT hr = HRESULT_FROM_WIN32(net_api_status);
         if (throw_win32sec_error) ThrowE(hr, __WFUNCTION__, Sid(), L"Problem occurred deleting this SID from local group. \nGroup<%s>\nAccnt<%s> Accnt type<%s>", group_name, GetQualifiedUserName().c_str(), EnumToString(GetSidNameUse()));
         return hr;
      }

      return S_OK;
   }

   // UserGetGroupsNetApi()
   //    Gets the local, global, and universal groups that this user belongs
   //    to, using the NetApi methods. Sets group_list with an array of SID string representing the groups.
   //    If throw_win32sec_error is true and an error occurs, the function
   //    throws a Win32SecError exception rather than returning the invalid
   //    HRESULT.
   //
   //    Formerly UserGetGroups()
   //
   // Remarks:
   //    Domain local groups are not supported since NetUserGetGroups()
   //    does not return these groups. Calling NetUserGetLocalGroups() on
   //    the domain will return domain local groups along with the domain
   //    controller's local groups, so that is not particularly helpful.
   //
   //    This function does not currently support much indirection. If
   //    there is a global group G that contains the user, and a local group
   //    L that contains G, then both L and G will be returned (this is because
   //    we specify LG_INCLUDE_INDIRECT). But if there is a global group G that
   //    contains the user, and another global group G2 that contains G, only
   //    G will be returned. We could probably remedy this to include all
   //    nested groups through the use of recursion, enumerating all groups
   //    and using functions like NetGroupGetUsers() and NetLocalGroupGetMembers().
   //    This was not done in the first version of this function due to time
   //    constraints. It is odd that there is not a Windows function to do what
   //    this function does. Unless you have the process token, getting all of
   //    the groups a user is effectively a member of is a daunting task.
   //
   // SLB 07.aug.2007 CR.8222; Modified so that if a groups can not be obtained for 
   // the specified SID, this is not considered an error condition.  Calls that should
   // succeed, but fail, still return an error code however. Also, modified to retrieve the
   // closest domain controller vs. the primary domain controller. This fixes issues when
   // the primary domain controller goes down and there is a secondary domain controller. 
   // SLB 16.mar.2011; CALL.514725; added more info to exceptions when they are thrown.
   // SLB 18.mar.2011; CALL.514725; Created new method which gets called (GetQualifiedUserName())
   // and now retrieves the Process's user name and adds it to error messages to help with 
   // trouble shooting.
   // SLB 05.feb.2020 CASE.723895; Modified to use new ThrowE() method to help put more info into the 
   // exception's message string. This removed the need for using _snwprintf_s().
   // Cosmetic code reformatting as well.
   // SLB 09.mar.2020 CR.29058.V01; updated to use CSmartNetApiBuf for managing NetApi allocated buffer(s). Should fix
   // some potential access violation issues when an error occurs.
   // Updated exception data to include more info.
   // SLB 11.mar.2020 CR.29142; Modified to output TSidSet.
   // SLB 18.nov.2021 CR.30540; This method had been deprecated... No longer the 
   // case. Instead, it has been repurposed to retrieve local group membership 
   // (useful when the current process Id is a local SID, or just doesn't have 
   // rights on the domain controller). 
   // Method is now const.
   // SLB 12.oct.2022 CR.31786; Updated ThrowE() text to have Qualified Username 
   // vs. just Username.
	HRESULT SIDPtr::UserGetGroupsNetApi(
      TSidSet& group_list,                                                          // CR.29142; modified.
      bool throw_win32sec_error) const                                              // CR.30540; modified.
   {
      HRESULT hr(S_OK);
      
      // CR.30540; Code below was "restored" from a previous revision of this file.

      // This function only supports user SIDs.
      std::wstring wsUserName(AccountName());                                       // CALL.514725; added.
      if (GetSidNameUse() != SidTypeUser)
      {
         hr = E_INVALIDARG;
         if (throw_win32sec_error) ThrowE(hr, __WFUNCTION__, Sid(), L"Function only supports user SIDs.\nAccnt<%s> Accnt type<%s>", GetQualifiedUserName().c_str(), EnumToString(GetSidNameUse())); // CR.31876; modified.
         return hr;
      }

      // CALL.514725;  Now calls GetQualifiedUserName() instead of constructing it explicitly.
      std::wstring wsQualifiedUserName(GetQualifiedUserName());                     // CALL.514725; added.

      LPLOCALGROUP_USERS_INFO_0 plocalbuf(NULL);
      DWORD entries_read(0), total_entries(0);
      NET_API_STATUS net_api_status;
      net_api_status = NetUserGetLocalGroups(
         NULL,                 // execute on local machine
         wsQualifiedUserName.c_str(),                                               // CR.29058; modified.
         0,
         0, //LG_INCLUDE_INDIRECT,  // Note: will fail with ACCESS_DENIED if not a domain account.
         (LPBYTE*)&plocalbuf,
         MAX_PREFERRED_LENGTH, // Buffer of needed size will be allocated automatically.
         &entries_read,
         &total_entries);
      CSmartNetApiBuf smartbuf(plocalbuf);                                          // CR.29058.V01; added.
      if (net_api_status != NERR_Success)
      {
         hr = HRESULT_FROM_WIN32(net_api_status);

         if (throw_win32sec_error) ThrowE(hr, __WFUNCTION__, Sid(), L"WIN32 API NetUserGetLocalGroups() Failed.\nUserName <%s>", wsQualifiedUserName.c_str());
         return hr;
      }

      bool bDebugRes(false);
      if (bDebugRes) ThrowE(hr, __WFUNCTION__, Sid(), L"WIN32 API NetUserGetLocalGroups() Failed.\nUserName <%s>", wsQualifiedUserName.c_str());


      // success. Get group sids from names and add to output list.
      DWORD i(0);
      LPLOCALGROUP_USERS_INFO_0 plocal_group_info(NULL);
      for (i = 0, plocal_group_info = plocalbuf;
         i < entries_read && plocal_group_info;
         i++, plocal_group_info++)
      {
         SIDPtr group_sid;
         if (!group_sid.LoadAccount(plocal_group_info->lgrui0_name)) continue;

         group_list.emplace(group_sid.Sid());
      }

      return S_OK;
   }


   // UserGetGroupsAuthz()
   //    Gets the local, global, and universal groups that this user belongs
   //    to, using the Authz API methods (via the CSecurityContext class). Sets group_list 
   //    with an array of SID string representing the groups.
   // 
   //    Always throws a Win32sec_error on error.
   //
   // Remarks
   //    This method may returns some unexpected groups, like those that have been "deleted" in 
   //    ActiveDirectory. A google search turned up that this is intentionally done, for "Authorization 
   //    purposes only".
   //    In my testing on the Solimar CORP domain, this method returns all groups... local and domain as well.
   //    I have not had enough time to investigate indirect group membership... 
   //
   // SLB 11.mar.2020 CR.29142; created.
   // SLB 22.sep.2022 CR.31876; Show SID/Username in the debug msg.
	void SIDPtr::UserGetGroupsAuthz(
      TSidSet& group_list)
   {
      CEnterExitMsg eem(__WFUNCTION__, L"%s <%s>", Sid(), GetQualifiedUserName().c_str()); // CR.31876; modified.
      CSecurityContext sec_context(*this);

      group_list.clear();
      sec_context.UserGetGroups(group_list, true);
   }


   // GetUserName()
   //    Builds a fully qualified user name for this SID.
   //
   // SLB 18.mar.2011 CALL.xxxxx; added.
	CSpdCString SIDPtr::GetQualifiedUserName() const                                 // CASE.723895; modified.
   {
      // Get the local groups from the local computer.
      wchar_t wcsQualifiedUserName[1024]; 
#if _MSC_VER < 1400
      _snwprintf(wcsQualifiedUserName, sizeof(wcsQualifiedUserName)/sizeof(wchar_t), L"%s\\%s", Domain(), AccountName()); 
      //
      // ensure NULL termination
      //
      wcsQualifiedUserName[sizeof(wcsQualifiedUserName)/sizeof(wchar_t)-1] = 0; 
#else
      _snwprintf_s(wcsQualifiedUserName, _TRUNCATE, L"%s\\%s", 
         Domain(), 
         AccountName()); 
#endif
      return wcsQualifiedUserName; 
   }

   // LocalGroupGetMembers() - static method.
   //    Gets a list of the members of the specified local group. Local group
   //    members can be users or global groups. Sets member_list with an array
   //    of SIDPtrs representing the members. If throw_win32sec_error is true
   //    and an error occurs, the function throws a Win32SecError exception
   //    rather than returning the invalid HRESULT.
   //
   // SLB 05.feb.2020 CASE.723895; Modified to use new ThrowE() method to help put more info into the 
   // exception's message string. This removed the need for using _snwprintf_s().
   // Cosmetic code reformatting as well.
   // SLB 09.mar.2020 CR.29058.V01; updated to use CSmartNetApiBuf for managing NetApi allocated buffer(s). 
   // Updated exception data to include more info.
	HRESULT SIDPtr::LocalGroupGetMembers(
      LPCWSTR group_name, 
      TSIDObjList& member_list,
      bool throw_win32sec_error)
   {
      LPLOCALGROUP_MEMBERS_INFO_0 plocalbuf(NULL);
      DWORD entries_read, total_entries;
      NET_API_STATUS net_api_status = NetLocalGroupGetMembers(
         NULL, 
         group_name, 
         0, 
         (LPBYTE*)&plocalbuf, 
         MAX_PREFERRED_LENGTH,
         &entries_read, 
         &total_entries, 
         NULL);
      CSmartNetApiBuf smartbuf(plocalbuf);
      if (net_api_status != NERR_Success)
      {
         HRESULT hr = HRESULT_FROM_WIN32(net_api_status);
         if (throw_win32sec_error)
         {
            // For troubleshooting, gather Process identity parms.
            CSpdCString wsPDomain, wsPAccount, wsPSID;                              // CR.29058.V01; added.
            GetCurrentUserAndDomain(wsPAccount, wsPDomain, wsPSID);                 // CR.29058.V01; added.
               
            ThrowE(hr, __WFUNCTION__, L"NA", L"WIN32 API NetLocalGroupGetMembers() failed. Failed to retrieve local group members.\nGroup name <%s>\n  Process Account <%s\\%s>  Process SID<%s>",
               group_name,                                                          // CR.29058.V01; added.
               wsPDomain.c_str(),                                                   // CR.29058.V01; added.
               wsPAccount.c_str(),                                                  // CR.29058.V01; added.
               wsPSID.c_str());                                                     // CR.29058.V01; added.
         }
         return hr;
      }

      DWORD i;
      LPLOCALGROUP_MEMBERS_INFO_0 plocal_group_members;
      for (i = 0, plocal_group_members = plocalbuf;
         i < entries_read && plocal_group_members;
         i++, plocal_group_members++)
      {
         SIDPtr member_sid;
         if (member_sid.LoadAccount((const SID*)plocal_group_members->lgrmi0_sid))
            member_list.emplace(member_sid); 
      }

      return S_OK;
   }

   // GetProcessSID() - static method.
   // Retrieves the SID for the process.
   // 
   // SLB 03.mar.2009 CR.11156; Created. Code was moved here from the CNtfsFolderName::IsFolderAccessOk() method.
   // SLB 06.feb.2020 CASE.723895; Changed to use ThrowE() and added some parms to give helpful info.
   //
	SIDPtr SIDPtr::GetProcessSID()
   {
      HRESULT hr;
      //
      // CR.6135; Retrieve current process's token.
      //
      BOOL bRES(FALSE);
      HANDLE hProcessToken(INVALID_HANDLE_VALUE);
      bRES = OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hProcessToken); 
      if (!bRES)
      {  
         // problem: OpenProcessToken() failed.
         hr = HRESULT_FROM_WIN32(GetLastError()); 
         ThrowE(hr, __WFUNCTION__, L"", L"Call to OpenProcessToken() failed.");
      }

      // CR.6135; Retrieve the SID for the current process's identity.
      // First get the needed buffer size.
      DWORD dwNeededBufLen(0);
      bRES = GetTokenInformation(
         hProcessToken,
         TokenUser, 
         NULL,
         0,
         &dwNeededBufLen); 
      if (!dwNeededBufLen)
      {  
         // problem: GetTokenInformation() did not calculate the size needed for a user token buffer.
         hr = HRESULT_FROM_WIN32(GetLastError());
         ThrowE(hr, __WFUNCTION__, L"", L"Call to GetTokenInformation0() failed (buffer size not returned).");
      }

      // allocate the token buffer.
      DWORD dwBufLen(dwNeededBufLen); 
      BYTE *pucTokenInfoBuf(new BYTE[dwNeededBufLen]); 

      // CR.9374; Find out how big the User token buffer needs to be.
      bRES = GetTokenInformation(
         hProcessToken,
         TokenUser, 
         (LPVOID)pucTokenInfoBuf,
         dwBufLen,
         &dwNeededBufLen); 
      if (!bRES)
      {  
         // problem: GetTokenInformation() failed.
         hr = HRESULT_FROM_WIN32(GetLastError());
         ThrowE(hr, __WFUNCTION__, L"", L"Call to GetTokenInformation0() failed.");
      }

      // Success. Setup return value with SID
      SIDPtr sidRetVal;
      TOKEN_USER *ptoBuf((TOKEN_USER *)pucTokenInfoBuf); 
      sidRetVal = (const SID *)ptoBuf->User.Sid; 

      // destroy allocated token buffer.
      delete[](pucTokenInfoBuf); 
      pucTokenInfoBuf = NULL; 

      // clean up handle allocation from OpenProcessToken().
      CloseHandle(hProcessToken); 
      return sidRetVal; 
   }

   // GetProcessUserName() - Retrieves the fully qualified user name of the current running process.
   // 
   // Throws Win32SecError object on error.
   // 
   // SLB 18.mar.2011 CALL.514725; Added this simple but helpful method
   //
	CSpdCString SIDPtr::GetProcessUserName()
   {
      SIDPtr sidProc(SIDPtr::GetProcessSID()); 

      return sidProc.GetQualifiedUserName(); 
   }


   // DebugPrint() - Dumps debug info regarding the each SID passed in the sid list.
   // 
   // SLB mar.11.2020 CR.29142; created.
   // SLB 12.oct.2022 CR.31876; Updated ThrowE() call to add qualified username vs. 
   // just username.
   void SIDPtr::DebugPrint(
      const TSidSet& list, 
      const CSpdCString & wsListDescription)
   {
      CEnterExitMsg eem(__WFUNCTION__, L"Dumping %s list: %u entries", wsListDescription.c_str(), (DWORD)list.size());

      for (TItcSidSet it(list.begin()); it != list.end(); it++)
      {
         SIDPtr sid(it->c_str());
         CEnterExitMsg::TraceMsg(L"SID <%s> AccountName <%s>  NameUse <%s> ", sid.Sid(), sid.GetQualifiedUserName().c_str(), EnumToString(sid.GetSidNameUse())); // CR.31876; modified.
      }
   }


   // InitializeAuthzClientContext() - Tries to initialize resource manager's client context in two ways, for 
   // the provided SID. First it tries to initialize with group membership info. However, if that fails because of
   // Access denied, it presumes that this is due to lack of permisisons on the Domain Controller (for the provided SID). 
   // So then, it retries, skipping group membership.
   //
   // Throws Win32SecError object on error.
   //
   // SLB 07.aug.2007 Created from existing code in GetEffectivePermissions. CR.8222; Added this 
   // method so as to obfuscate the retry of AuthzInitializeContextFromSid() that occurs if the first 
   // attempt fails with ACCESS_DENIED. This fixes issue when a computer is on the domain, a domain 
   // client is attempting to connect but the server is running as a local identity (vs. one on the 
   // domain). I set this up to return an HR vs. throw an exception. I am not sure which method Dylan 
   // would have preferred.  It can be changed though. 
   // SLB 31.aug.2007 CR.6135; added _try/_finally block.
   // SLB 06.feb.2020 CASE.723895; Now static. Now shared by both the SecurityDescriptorPtr class as well as the 
   // CSecurityContext class.
   // Now throws Win32SecError on error. 
   // SLB 05.feb.2021 CASE.730993; Debug log for Sogica is showing an error code of 0. Thinking that 
   // another method used by TraceMsg() could reset this, I moved the calls to GetLastError() 
   // so that they are immediately after the method that was supposed to SetLastError(). 
   // SLB 18.nov.2021 CR.30540; Fix for not adding local groups to the security context, when the
   // current process id does not have access rights to the domain controller (e.g. when it is 
   // a local machine SID). Method now handles this case by using the NetApi() version of UserGetGroups().
   // SLB 12.oct.2022 CR.31876; Added SID's qualified username into exception messages. 
   //********************************************************************************************************************
   static void InitializeAuthzClientContext(                                        // CASE.723895; modified.
      const SIDPtr &sid,
      AUTHZ_RESOURCE_MANAGER_HANDLE authz_resource_manager,
      AUTHZ_CLIENT_CONTEXT_HANDLE &authz_client)
   {
      // CR.8222; Comment below was in the original code in GetEffectivePermissions(). Moved the comment
      // here. I am not experiencing the exceptions described, though I am testing in the same environment 
      // described in the comment.
      // CR.6135; I am now getting this exception ("procedure number xxx is out of range"), I suspect it is a 
      // debug environment thing. Client identity is the same as the Server identity, which is a domain identity
      // (corp\sbailey). It does not seem to be affecting the results of the code.
      //
      //************
      // DCM 28.mar.06 - When running the Security application as a local
      // user when logged in as a domain user, this function is throwing
      // exceptions that are not caught by the catch blocks in the calling
      // code.
      //************
      BOOL bRES(FALSE);
      LUID luid = { 0x0,0x0 };
      bRES = AuthzInitializeContextFromSid(
         0,
         (const PSID)sid.GetPSID(),
         authz_resource_manager,
         NULL,
         luid,
         NULL,
         &authz_client);
      DWORD le = GetLastError();                                                    // CASE.730993; added.
      if (bRES != FALSE) return;

      // CASE.723895; Added Trace message to warn about group membership.
      CEnterExitMsg::TraceMsg(L"%s() - Failed to initialize security context with Group info for SID. Security will not support group membership for this Id. \nSID<%s> Account <%s>",
         __WFUNCTION__,
         sid.Sid(),
         sid.GetQualifiedUserName().c_str());

      // Failed.
      // CR.8222; the AuthzInitializeContextFromSid() S4U logon probably failed (called when it tries to
      // determine what groups the SID belongs too).  Though the group info could not be retrieved, the context
      // should be created with the SID alone. Retry the AuthzInitializeContextFromSid() call, but tell it to 
      // skip looking for the SIDs group membership.
      if (le != ERROR_ACCESS_DENIED) ThrowE(HRESULT_FROM_WIN32(le), __WFUNCTION__, sid.Sid(), L"Call to AuthzInitializeContextFromSid(0) failed for Account <%s>", sid.GetQualifiedUserName().c_str()); // CR.31876; modified. CASE.730993; modified.

      bRES = AuthzInitializeContextFromSid(
         AUTHZ_SKIP_TOKEN_GROUPS,
         (const PSID)sid.GetPSID(),
         authz_resource_manager,
         NULL,
         luid,
         NULL,
         &authz_client);
      le = GetLastError();                                                          // CASE.730993; added.
      if (bRES == FALSE) ThrowE(HRESULT_FROM_WIN32(le), __WFUNCTION__, sid.Sid(), L"Call to AuthzInitializeContextFromSid(AUTHZ_SKIP_TOKEN_GROUPS) failed for Account <%s>", sid.GetQualifiedUserName().c_str()); // CR.31876; modified. CR.30450; modified.

      // CR.30540; Retrieve local groups that this SID is a member of and add them 
      // to the security context.
      
      // Use NetApi to retrieve any local groups the sid might be a member of.
      TSidSet groups;
      sid.UserGetGroupsNetApi(groups, true);
      if (groups.empty()) return;

      CEnterExitMsg eem(__WFUNCTION__,  L"SID is a member of %u local groups.  <CR.30540>", groups.size());

      // This SID is a member of some local groups. Add these groups to the 
      // security context.
      // First create and initialize an array of SID_AND_ATTRIBUTES objects.
      PSID_AND_ATTRIBUTES psiaArray = new SID_AND_ATTRIBUTES[groups.size()];
      SIDPtr* psidPtrs = new SIDPtr[groups.size()];

      // try/catch block is to ensure that above memory allocations get cleaned 
      // up on error.
      try
      {
         if (!psiaArray || !psidPtrs) ThrowE(HRESULT_FROM_WIN32(E_POINTER), __WFUNCTION__, sid.Sid(), L"Assigning local groups to security context failed for Account <%s>", sid.GetQualifiedUserName().c_str()); // CR.31876; modified.

         DWORD i(0);
         for (TItcSidSet it(groups.begin()); it != groups.end(); it++, i++)
         {
            //CEnterExitMsg::TraceMsg(L"GroupName <%s>  SID <%s>  <CR.30540>", pPtrs[i]->AccountName(), pPtrs[i]->Sid());
            psidPtrs[i] = SIDPtr(it->c_str());
            CEnterExitMsg::TraceMsg(L"GroupName <%s>  SID <%s>  <CR.30540>", psidPtrs[i].AccountName(), psidPtrs[i].Sid());

            psiaArray[i].Sid = (PSID)psidPtrs[i].GetPSID();
            psiaArray->Attributes = SE_GROUP_ENABLED;
         }

         // Now add them to the security context. 
         // Note: this actually creates a copy of the context provided, and
         // adds them to that.
         AUTHZ_CLIENT_CONTEXT_HANDLE authz_client_new;
         bRES = AuthzAddSidsToContext(authz_client, psiaArray, (DWORD)groups.size(), NULL, 0, &authz_client_new);
         le = GetLastError();
         if (bRES == FALSE) ThrowE(HRESULT_FROM_WIN32(le), __WFUNCTION__, sid.Sid(), L"Call to AuthzAddSidsToContext() failed when trying to add local group SIDs for Account <%s>", sid.GetQualifiedUserName().c_str()); // CR.31876; modified.

         // make sure to cleanup heap allocations
         delete[](psidPtrs);
         delete[](psiaArray);

         // Clean up original client context and assign the new one to the output.
         AuthzFreeContext(authz_client);
         authz_client = authz_client_new;
      }
      catch (Win32SecError &e)
      {
         // clean-up mem allocations and re-throw.
         delete[](psidPtrs);
         delete[](psiaArray);
         throw(e);
      }

   }

   /*
    * class SecurityDescriptorPtr
    *
    *
    */
   
   class SecurityDescriptorPtr : 
      public CSecurityDesc
   {
   public:
      SecurityDescriptorPtr() : CSecurityDesc()
      {
        ; 
      }

      SecurityDescriptorPtr(const SecurityDescriptorPtr &c) : CSecurityDesc(c)
      {
        ; 
      }

      SecurityDescriptorPtr(PSECURITY_DESCRIPTOR psd) : CSecurityDesc(*((const SECURITY_DESCRIPTOR*)psd))
      {
        ; 
      }
      
      SecurityDescriptorPtr(const wchar_t *sd_str)
      {
         FromString(sd_str);
      }

      SecurityDescriptorPtr(const CSpdCString &wsSd)                               // CR.18845; added.
      {
         FromString(wsSd.c_str());
      }

      bool IsValid() const 
      {
         return (0 != GetPSECURITY_DESCRIPTOR() && IsValidSecurityDescriptor((PSECURITY_DESCRIPTOR)GetPSECURITY_DESCRIPTOR()));
      }

      operator CSpdCString() const                                                 // CR.18845; added.
      {
         CString cs; 
         if (!IsValid()) return L""; 
         if (!ToString(&cs)) return L""; 

         return (const wchar_t *)cs; 
      }

      operator _bstr_t() const
      {
         CString cs;
         if (IsValid() && ToString(&cs))
         {
            return _bstr_t((const wchar_t*)cs);
         }
         else
         {
            return _bstr_t(L"");
         }
      }

      /* RemoveDaclInheritedDuplicateAces() - Looks for ACE entries in DACL, which have duplicate entries where one is 
       * inherited and the other is not(it is explicit). Removes the explicit ACE. 
       *
       * SLB 10.apr.2017 CR.18485.V01; Created.
      ------------------------------------------------------------------------------------------------------*/
      DWORD RemoveDaclInheritedDuplicateAces()
      {
         if (!IsValid()) ThrowHr(E_POINTER, __WFUNCTION__, L"Security Descriptor object is not initialized."); 

         CDacl Dacl; 
         bool bPresent; 
         bool bDefaulted; 
         bool bRes; 
         bRes = GetDacl(&Dacl, &bPresent, &bDefaulted); 
         if (!bRes) ThrowHr(E_FAIL, __WFUNCTION__, L"GetDacl() call failed."); 

         CAcl::CAceFlagArray faAceFlags; 
         CAcl::CAccessMaskArray maAccessMasks; 
         CAcl::CAceTypeArray taTypes; 
         CSid::CSidArray saSids; 
         Dacl.GetAclEntries(&saSids, &maAccessMasks, &taTypes, &faAceFlags); 

         typedef std::set<DWORD> TRemoveList; 
         TRemoveList removeList; 
         for (DWORD i = 0; i < faAceFlags.GetCount(); i++)
         {
            if ((faAceFlags[i]&INHERITED_ACE) == 0) continue; 

            // Found an inherited ACE. now search for duplicates of it.
            for (DWORD n = 0; n < faAceFlags.GetCount(); n++)
            {
               if (n == i) continue; 
               if ((faAceFlags[i]&faAceFlags[n]) != faAceFlags[n]) continue; 
               if (saSids[i] != saSids[n]) continue; 
               if (maAccessMasks[i] != maAccessMasks[n]) continue; 
               if (taTypes[i] != taTypes[n]) continue; 

               // A match has been found. Add it to the remove ACE list
               removeList.insert(n); 
            }
         }

         DWORD dwRemoveCount(0); 
         for (TRemoveList::iterator it(removeList.begin()); it != removeList.end(); it++)
         {
            Dacl.RemoveAce(*it - dwRemoveCount); 
            dwRemoveCount++; 
         }
         if (dwRemoveCount) SetDacl(Dacl, bDefaulted); 

         return dwRemoveCount; 
      }

      /* RemoveInheritedAces() - removes DACL's ACEs by flags set. Typcially, SPDE Security App uses this to remove 
       * ACEs with the INHERITED_ACE flag. 
       *
       * SLB 10.apr.2017 CR.18485; Created.
      ------------------------------------------------------------------------------------------------------*/
      DWORD RemoveDaclAces(
         DWORD dwAceFlags)
      {
         if (!IsValid()) ThrowHr(E_POINTER, __WFUNCTION__, L"Security Descriptor object is not initialized."); 

         CDacl Dacl; 
         bool bPresent; 
         bool bDefaulted; 
         bool bRes; 
         bRes = GetDacl(&Dacl, &bPresent, &bDefaulted); 
         if (!bRes) ThrowHr(E_FAIL, __WFUNCTION__, L"GetDacl() call failed."); 

         CAcl::CAceFlagArray faAceFlags; 
         CSid::CSidArray saSids; 
         Dacl.GetAclEntries(&saSids, NULL, NULL, &faAceFlags); 

         DWORD dwRemovedCount(0); 
         for (DWORD i = 0; i < faAceFlags.GetCount(); i++)
         {
            if ((faAceFlags[i]&dwAceFlags) == 0) continue; 
            Dacl.RemoveAce(i - dwRemovedCount); 
            dwRemovedCount++; 
         }

         // Only update this object's Dacl if an ace was removed.
         if (dwRemovedCount) SetDacl(Dacl, bDefaulted); 

         return dwRemovedCount; 
      }


      // GetEffectivePermissions()
      //    If throw_win32sec_error is true and an error occurs, the function
      //    throws a Win32SecError exception rather than returning the invalid
      //    HRESULT.
      // SLB 01.feb.2008 CR.9374; Added code to track down exception errors occuring here.
      // SLB 05.feb.2020 CASE.723895; Re-structured error handling and exception messages. 
      // _try {}/ _finally() replaced with // try/catch(). Internally, code throws exceptions on error, but 
      // catches it. 
      // catch() handler uses the throw_win32sec_error to determine whether to re-throw the exception or just return an HRESULT.
      // Removed code which translates the error code to a user description. Leaves that to caller now.
      // Found/fixed potential issue in _finally() block. Was potentially dereferencing a null ptr:
      //    if (authz_resource_manager->inuse) AuthzFreeResourceManager();
      // SLB 05.feb.2021 CASE.730993; As a stab at the Sogica issue, I thought that maybe certain calls
      // within the catch () handler could be throwing exceptions, so I wrapped those calls so exceptions
      // there would get trapped and "eaten".
      // I also added a general exception catch(), just in case something other than a Win32SecError is getting 
      // thrown (e.g. Access violation).
      // Added some debug code which I could use to introduce exceptions, when attached with the debugger. 
      // SLB 18.nov.2021 CR.30540; Simplified method by using the CSecurityContext class's GetEffectivePermissions()
      // method.
      HRESULT GetEffectivePermissions(
         SIDPtr &sid, 
         unsigned long desired_access_mask,
         unsigned long *granted_access_mask, 
         bool throw_win32sec_error) const                                           // CASE.723895; modified.
      {
         DWORD dwState(0);                                                          // CR.9374
         #ifdef SECURITY_OUTPUT_DEBUG
         CEnterExitMsg::TraceMsg(L"   SecurityDescriptorPtr::GetEffectivePermissions ***BEGIN***\r\n");
         wchar_t scratchbuf[512];
         _snwprintf_s(scratchbuf, _TRUNCATE, L"      SID: %s, Domain: %s, Account Name: %s\r\n",
            sid.Sid(), sid.Domain(), sid.AccountName());
         CEnterExitMsg::TraceMsg(scratchbuf);
         #endif

         // clear output.
         *granted_access_mask = 0;

         HRESULT hr;                                                                // CR.9374
         CSecurityContext scContext(sid);
         hr = scContext.GetEffectivePermissions(                                    // CR.30540; added.
            (PSECURITY_DESCRIPTOR)GetPSECURITY_DESCRIPTOR(), 
            desired_access_mask, 
            granted_access_mask, 
            throw_win32sec_error); 
         if (FAILED(hr)) return hr;

         #ifdef SECURITY_OUTPUT_DEBUG
         CEnterExitMsg::TraceMsg(L"   SecurityDescriptorPtr::GetEffectivePermissions ***END***\r\n");
         #endif

         return S_OK;

         #if 0  // DCM 05.oct.05 - KEEP GetEffectiveRightsFromAcl() code.         
         TRUSTEE trustee = sid.Trustee();
         
         bool dacl_present, dacl_defaulted;
         CDacl dacl;
         
         if (!IsValid())
         {
            CEnterExitMsg::TraceMsg(L"CSpdSecuritySetup::Win32Sec::SecurityDescriptorPtr::GetEffectivePermissions() encountered an null Security Descriptor pointer or an empty Security Descriptor. Granting full access.\r\n");
            *granted_access_mask = 0xFFFFFFFF;
            return S_FALSE;
         }
         
         if (!GetDacl(&dacl, &dacl_present, &dacl_defaulted))
            return HRESULT_FROM_WIN32(GetLastError());
         
         if (dacl_present && dacl)
         {
            return HRESULT_FROM_WIN32(GetEffectiveRightsFromAcl((PACL)dacl.GetPACL(), &trustee, granted_access_mask));
         }
         else
         {
            CEnterExitMsg::TraceMsg(L"CSpdSecuritySetup::Win32Sec::SecurityDescriptorPtr::GetEffectivePermissions() encountered a null or empty DACL, granting full access.\r\n");
            *granted_access_mask = 0xFFFFFFFF;
            return S_FALSE;
         }
         #endif
      }

      // 
      // GetEffectivePermissions2() - Wrapper for GetEffectivePermissions() which return a Win32 error
      // code vs an HRESULT.
      //
      // SLB 14.apr.2008 CR.9374; Added.
      //
      DWORD GetEffectivePermissions2(
         SIDPtr &sid, 
         unsigned long desired_access_mask,
         unsigned long *granted_access_mask) const
      {
         HRESULT hr(S_OK); 
         DWORD dwNtErr(0); 

         hr = GetEffectivePermissions(sid, desired_access_mask, granted_access_mask, false); 
         WIN32_FROM_HRESULT(hr, &dwNtErr); 
         return dwNtErr; 
      }

      PSECURITY_DESCRIPTOR Copy()
      {
         //if (!IsValid()) return 0;
         //PSECURITY_DESCRIPTOR psd = (PSECURITY_DESCRIPTOR)GetPSECURITY_DESCRIPTOR();
         return 0;
      }

   };




   //
   // CSecurityContext methods
   // This class maintains a security context which can then be used to check access for 
   // security descriptors passed to it.
   //

   // CSecurityContext construtor.
   //
   CSecurityContext::CSecurityContext(const SIDPtr &sid) :
      m_sid(sid),                                                                   // CASE.723895; added.
      m_authz_resource_manager(0),
      m_authz_client(0)
   {
      InitializeAuthzContextData(sid);
   }

   // CSecurityContext destructor.
   //
   CSecurityContext::~CSecurityContext()
   {
      if (m_authz_client) AuthzFreeContext(m_authz_client);
      if (m_authz_resource_manager) AuthzFreeResourceManager(m_authz_resource_manager);
   }

   // IsValid() - returns false if authz api has not been initialized successfully.
   //
   bool CSecurityContext::IsValid() const
   {
      if (m_authz_client == 0) return false;
      if (m_authz_resource_manager == 0) return false;

      return true;
   }

   // WIN32_FROM_HRESULT() - static Helper method to convert HRESULT back into Win32 error value.
   //
   // SLB 14.apr.2008 CR.9374; Added.
   //
   bool CSecurityContext::WIN32_FROM_HRESULT(
      HRESULT hr, 
      DWORD *pdwWin32) 
   {
      if ((hr & 0xFFFF0000) == MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, 0))
      {
         // Could have come from many values, but we choose this one
         *pdwWin32 = HRESULT_CODE(hr);
         return true;
      }
      if (hr == S_OK)
      {
         *pdwWin32 = HRESULT_CODE(hr);
         return true;
      }

      //assert(false);  // SLB 25.apr.2008 - this does not compile in 2003 gui code.
      // otherwise, we got an impossible value
      return false;
   }


   // GetEffectivePermissions()
   //    If throw_win32sec_error is true and an error occurs, the function
   //    throws a Win32SecError exception rather than returning the invalid
   //    HRESULT.
   //
   // SLB 01.feb.2008 CR.9374; Added code to track down exception errors occuring here.
   // SLB 03.mar.2009 CR.11156; Code adapted from SecurityDescriptorPtr class. Code is simplified though,
   // because the SID's security context is already established. Instead, the security descriptor ptr is passed
   // to determine the access rights (vs. this class wrapping the security desciptor ptr and creating the 
   // security context on the fly). This improves performance significantly.
   // SLB 06.feb.2020 CASE.723895; Refactoring and exception handling changes. 
   // SLB 05.feb.2021 CASE.730993; As a stab at the Sogica issue, I thought that maybe certain calls
   // within the catch () handler could be throwing exceptions, so I wrapped those calls so exceptions
   // there would get trapped and "eaten".
   // I also added a general exception catch(), just in case something other than a Win32SecError is getting 
   // thrown (e.g. Access violation).
   // SLB 18.nov.2021 CR.30540; Minor fix to general exception catch (...) handler. Fixed so throw_win32sec_error
   // is checked to determine if an exception should be thrown.
   //
   HRESULT CSecurityContext::GetEffectivePermissions(
      PSECURITY_DESCRIPTOR psd,
      unsigned long desired_access_mask,
      unsigned long *granted_access_mask,
      bool throw_win32sec_error) const
   {

      *granted_access_mask = 0;

      // DCM CR.Fix.4838.v01 - Due to a Microsoft bug in GetEffectiveRightsFromAcl(),
      // we must use AuthzAccessCheck() to get effective permissions. This is
      // somewhat more complex than using GetEffectiveRightsFromAcl(), the MSDN
      // documentation for these functions is poor, and there is very little info
      // regarding these functions on the web, so it is possible there are some
      // bugs in this code.
      AUTHZ_ACCESS_CHECK_RESULTS_HANDLE authz_handle = 0;
      try
      {
         // Set up AUTHZ_ACCESS_REQUEST and AUTHZ_ACCESS_REPLY structures.
         AUTHZ_ACCESS_REQUEST access_request;
         access_request.DesiredAccess = 0; // set below
         access_request.PrincipalSelfSid = NULL; // SPD objects are not principals (e.g., a user)
         access_request.ObjectTypeList = NULL;
         access_request.ObjectTypeListLength = 0;
         access_request.OptionalArguments = NULL;

         AUTHZ_ACCESS_REPLY access_reply;
         BYTE buffer[64];
         RtlZeroMemory(buffer, sizeof(buffer));
         access_reply.ResultListLength = 1;
         access_reply.GrantedAccessMask = (PACCESS_MASK)buffer;
         access_reply.SaclEvaluationResults = (PDWORD)(buffer + sizeof(ACCESS_MASK));
         access_reply.Error = (PDWORD)(buffer + sizeof(DWORD) + sizeof(ACCESS_MASK));

         // Call AuthzAccessCheck() to determine which access bits can be granted to
         // the SID for this object's security descriptor. Since the DesiredAccess
         // parameter of AuthzAccessCheck() works on an AND basis (all access bits
         // must have access or function returns 0 for GrantedAccessMask) and
         // GetEffectivePermissions() is expected to set granted_access_mask with
         // any bits that have access, we must perform the cumbersome task of calling
         // AuthzAccessCheck() one time for each set bit in the desired_access_mask.
         BOOL bRES(FALSE);
         for (unsigned long i = 1; i; i <<= 1)  // for every bit
         {
            // if bit is not set in the desired access mask, then go to next bit.
            if ((i & desired_access_mask) == 0) continue;                           // CASE.723895; modified.

            // this access flag is being requested. Check to see if it is 
            // authorized by the security descriptor.
            access_request.DesiredAccess = i;
            *access_reply.GrantedAccessMask = 0;

            // The first call is to AuthzAccessCheck() and subsequent calls are to
            // AuthzCachedAccessCheck(). Specifying AUTHZ_ACCESS_CHECK_NO_DEEP_COPY_SD
            // in AuthzAccessCheck() might be slightly more efficient, but that flag
            // is poorly documented so we don't currently use it.
            if (!authz_handle)
            {
               bRES = AuthzAccessCheck(
                  0, m_authz_client, &access_request,
                  NULL,
                  psd,
                  NULL,
                  0,
                  &access_reply,
                  &authz_handle);
               if (bRES == FALSE) ThrowE(HRESULT_FROM_WIN32(GetLastError()), __WFUNCTION__, m_sid.Sid(), L"Call to AuthzAccessCheck() failed for access flag <0x%x> Accnt <%s>.", i, m_sid.GetQualifiedUserName().c_str());
            }
            else
            {
               bRES = AuthzCachedAccessCheck(
                  0, authz_handle, &access_request,
                  NULL,
                  &access_reply);
               if (bRES == FALSE) ThrowE(HRESULT_FROM_WIN32(GetLastError()), __WFUNCTION__, m_sid.Sid(), L"Call to AuthzAccessCheck() failed for access flag <0x%x> Accnt <%s>.", i, m_sid.GetQualifiedUserName().c_str());
            }

            *granted_access_mask |= *access_reply.GrantedAccessMask;
         }

         // cleanup
         if (authz_handle) AuthzFreeHandle(authz_handle);
      }
      catch (Win32SecError &e)
      {
         // cleanup. eat exceptions which occur while trying to clean-up.
         if (authz_handle) try { AuthzFreeHandle(authz_handle); } catch (...) {}    // CASE.730993; added try/catch(...) wrapper.

         if (throw_win32sec_error) throw(e);
         return e.GetHRESULT();
      }
      catch (...)                                                                   // CASE.730993; added catch(...) {}
      {
         // cleanup. eat exceptions which occur while trying to clean-up.
         if (authz_handle) try { AuthzFreeHandle(authz_handle); } catch(...) {}

         if (throw_win32sec_error) ThrowE(E_FAIL, __WFUNCTION__, L"", L"General exception caught."); // CR.30540; modified.
         return E_FAIL;                                                             // CR.30540; added.
      }
      return S_OK;
   }

   // GetEffectivePermissions2() - Wrapper for GetEffectivePermissions() which return a Win32 error
   // code vs an HRESULT.
   //
   // SLB 14.apr.2008 CR.9374; Added.
   //
   DWORD CSecurityContext::GetEffectivePermissions2(
      PSECURITY_DESCRIPTOR psd,
      unsigned long desired_access_mask,
      unsigned long *granted_access_mask) const
   {
      HRESULT hr(S_OK);
      DWORD dwNtErr(0);

      hr = GetEffectivePermissions(psd, desired_access_mask, granted_access_mask, false);
      WIN32_FROM_HRESULT(hr, &dwNtErr);
      return dwNtErr;
   }


   // UserGetGroups() - 
   //    Gets all groups this context's SID is a member of, using the Authz API... This includes 
   //    groups which are no longer in use.
   //    Re-curses into groups to get nested group membership.
   //    Sets group_list with an array of SID string representing the groups.
   // 
   //    Always throws a Win32sec_error on error.
   //
   // Remarks
   //    This method may return some unexpected groups, like those that have been "deleted" in 
   //    ActiveDirectory. A google search turned up that this is intentionally done, for "Authorization 
   //    purposes only".
   //    In my testing on the Solimar CORP domain, this method returns all groups... local and domain as well.
   //    I have not had enough time to investigate indirect group membership... 
   //
   // SLB 11.mar.2020 CR.29142; Created.
   // SLB 04.sep.2020 CR.29727; No longer throws exception whan a recursive call fails.
   // SLB 05.feb.2021 CASE.730993; As a stab at the Sogica issue, I added a general exception catch(), 
   // just in case something other than a Win32SecError is getting thrown.
   // SLB 18.nov.2021 CR.30540; Fix for not including groups (in the output list) 
   // which have either the SE_GROUP_MANDATORY or SE_GROUP_ENABLED_BY_DEFAULT 
   // flags set.
   // SLB 12.oct.2022 CR.31876; Added SID's qualified username into exception messages. 
   void CSecurityContext::UserGetGroups(
      TSidSet& group_list,
      bool recurse_groups) const
   {
      if (!IsValid()) ThrowE(E_FAIL, __WFUNCTION__, m_sid.Sid(), L"CSecurityContext is not in a valid state.");

      // Need make an Authz api call to get groups the context SID is a member of.
      // First make call so correct buffer size can be allocated.
      DWORD dwBufLen(0);
      BOOL bRES;
      bRES = AuthzGetInformationFromContext(
         m_authz_client,
         AUTHZ_CONTEXT_INFORMATION_CLASS::AuthzContextInfoGroupsSids,
         0,
         &dwBufLen,
         NULL);
      DWORD le(GetLastError());
      if (le != ERROR_INSUFFICIENT_BUFFER) ThrowE(HRESULT_FROM_WIN32(le), __WFUNCTION__, m_sid.Sid(), L"Call to AuthzGetInformationFromContext() failed when asking for Group SIDs. Accnt<%s>", m_sid.GetQualifiedUserName().c_str()); // CR.31876; modified.

      // Allocate needed buffer.
      PVOID pBuf = new BYTE[dwBufLen + 1];
      if (!pBuf) ThrowE(E_OUTOFMEMORY, __WFUNCTION__, m_sid.Sid(), L"Failed allocate buffer required for call to AuthzGetInformationFromContext(). Accnt<%s>", m_sid.GetQualifiedUserName().c_str()); // CR.31876; modified.

      try
      {
         bRES = AuthzGetInformationFromContext(
            m_authz_client,
            AUTHZ_CONTEXT_INFORMATION_CLASS::AuthzContextInfoGroupsSids,
            dwBufLen,
            &dwBufLen,
            pBuf);
         if (bRES == FALSE) ThrowE(HRESULT_FROM_WIN32(GetLastError()), __WFUNCTION__, m_sid.Sid(), L"Call to AuthzGetInformationFromContext() failed when asking for Group SIDs. Accnt<%s>", m_sid.GetQualifiedUserName().c_str()); // CR.31876; modified

         TOKEN_GROUPS *pgroups = (TOKEN_GROUPS *)pBuf;
         for (DWORD i(0); i < pgroups->GroupCount; i++)
         {
            const SID_AND_ATTRIBUTES &info(pgroups->Groups[i]);
            SIDPtr sid((const SID*)info.Sid);

            //CEnterExitMsg::TraceMsg(L"Group %u of %u: GroupSID <%s>  GroupName <%s>  Attributes <%08X>", i+1, pgroups->GroupCount, sid.Sid(), sid.AccountName(), info.Attributes);

            // If the group is not enabled, don't add it to the list.
            // CR.30540; There are multiple flags to check for this... MANDATORY 
            // and ENABLED_BY_DEFAULT added.
            if (!(info.Attributes & (SE_GROUP_MANDATORY|SE_GROUP_ENABLED_BY_DEFAULT|SE_GROUP_ENABLED))) continue; // CR.30540; modified.

            // Prevent infinite recursion... if this SID is already in the 
            // group_list, continue to the next one.
            if (group_list.find(sid.Sid()) != group_list.end()) continue;

            // Add this SID to this group list.
            group_list.emplace(sid.Sid());
            if (!recurse_groups) continue;

            // Don't recurse if this SID is a User type.
            if (sid.GetSidNameUse() == SID_NAME_USE::SidTypeUser) continue;

            CEnterExitMsg eem(L"Recursing UserGetGroups", L"Group member: Name <%s\\%s> SID <%s> NameUse <%s>", sid.Domain(), sid.AccountName(), sid.Sid(), SIDPtr::EnumToString(sid.GetSidNameUse()));
            // Recurse into this group for further membership.
            // CR.29727; Don't allow exception retrieving group info to abort the operation. The successfully retrieved groups may be sufficient.
            try                                                                     // CR.29297; added try.
            {
               CSecurityContext sc_group(sid);
               sc_group.UserGetGroups(group_list, true);
            }
            catch (Win32SecError &e)                                                // CR.29297; added catch() {}.
            {
               CEnterExitMsg::TraceMsg(L"(%08X) Warning: Group (SID: %s) membership will not be used when determining Authorization. Exception caught while retrieving its member groups. \n%s",
                  e.GetHRESULT(),
                  sid.Sid(),
                  e.GetMessage());
            }
            catch (...)                                                             // CASE.730993; added catch (...) {}.
            {
               CEnterExitMsg::TraceMsg(L"Warning: Group (SID: %s) membership will not be used when determining Authorization. Geternal Exception caught while retrieving its member groups.",
                  sid.Sid());
            }
         }

         // cleanup allocated buffer before exiting.
         delete(pBuf);
      }
      catch (Win32SecError &e)
      {
         // clean-up allocated buffer and re-throw.
         delete(pBuf);
         throw(e);
      }
      catch (...)                                                                   // CASE.730993; added catch(...) {}.
      {
         // clean-up allocated buffer and re-throw.
         delete(pBuf);
         ThrowE(E_FAIL, __WFUNCTION__, L"", L"General exception caught. Groupcount (%zu)", group_list.size());
      }
   }

   // InitializeAuthzContextData() - private method
   // This method is responsible for initializing m_authz_resource_manager and m_authz_client members.
   // 
   // SLB 03.mar.2009 CR.11156; Created. Code adapted from SecurityDescriptorPtr class's GetEffectivePermissions() method.
   // SLB 05.feb.2021 CASE.730993; As a stab at the Sogica issue, I thought that maybe certain calls
   // within the catch () handler could be throwing exceptions, so I wrapped those calls so exceptions
   // there would get trapped and "eaten".
   // I also added a general exception catch(), just in case something other than a Win32SecError is getting 
   // thrown (e.g. Access violation).
   //
   void CSecurityContext::InitializeAuthzContextData(
      const SIDPtr &sid)
   {
      DWORD dwNtErr(ERROR_SUCCESS);
      HRESULT hr(S_OK);
      try
      {
         // Create the resource manager needed by AuthzInitializeContextFromSid().
         // Not sure if we should be using AUTHZ_RM_FLAG_INITIALIZE_UNDER_IMPERSONATION,
         // but a bitwise OR with this causes the function to fail.
         BOOL bRES;
         bRES = AuthzInitializeResourceManager(
            AUTHZ_RM_FLAG_NO_AUDIT,
            NULL,
            NULL,
            NULL,
            NULL,
            &m_authz_resource_manager);
         if (bRES == FALSE) ThrowE(HRESULT_FROM_WIN32(GetLastError()), __WFUNCTION__, sid.Sid(), L"Call to AuthzInitializeResourceManager() failed.");

         // Create a user-mode client context from the SID. 
         // Note: On error, InitializeAuthzClientContext() will throw Win32SecError object
         InitializeAuthzClientContext(
            sid,
            m_authz_resource_manager,
            m_authz_client);
      }
      catch (Win32SecError &e)                                                      // CASE.723895; modified.
      {
         // cleanup. eat exception which occur while trying to clean-up.
         if (m_authz_client) try { AuthzFreeContext(m_authz_client); } catch(...) {}// CASE.730993; added try/catch(...) wrapper.
         if (m_authz_resource_manager) try { AuthzFreeResourceManager(m_authz_resource_manager); } catch(...) {} // CASE.730993; added try/catch(...) wrapper. CASE.723895; modified.

         m_authz_client = 0;
         m_authz_resource_manager = 0;

         throw(e);
      }
      catch (...)                                                                   // CASE.730993; added catch(...) {}.
      {
         // cleanup. eat exception which occur while trying to clean-up.
         if (m_authz_client) try { AuthzFreeContext(m_authz_client); } catch(...) {}
         if (m_authz_resource_manager) try { AuthzFreeResourceManager(m_authz_resource_manager); } catch(...) {} 

         m_authz_client = 0;
         m_authz_resource_manager = 0;
         ThrowE(E_FAIL, __WFUNCTION__, sid.Sid(), L"General exception caught.");
      }
   }

}; 

