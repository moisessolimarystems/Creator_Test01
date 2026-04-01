#ifndef _Solimar_Win32Sec_89651684354257756786987_
#define _Solimar_Win32Sec_89651684354257756786987_

#include <sddl.h>
#include <aclapi.h>
#include <windows.h>
#include <comdef.h>
#include <comutil.h>
#include <atlsecurity.h>
#include <atlstr.h>
#include <authz.h>
#include <lm.h>
#include <vector>
#include <dsgetdc.h> // CR.8222
using namespace ATL;

//#define SECURITY_OUTPUT_DEBUG // uncomment for verbose debug messages

namespace Win32Sec
{
   // A Win32SecError object represents an exception condition detected by
   // the Win32Sec functions. This class encapsulates an HRESULT error code
   // and an associated message describing the error.
   class Win32SecError
   {
   public:
      Win32SecError::Win32SecError(HRESULT _hr, const wchar_t* msg) throw()
      {
         hr = _hr;
#if _MSC_VER < 1400
         wcscpy(message, msg);
#else
         wcscpy_s(message, msg);
#endif
      }

      Win32SecError::Win32SecError(const Win32SecError& that) throw()
      {
         hr = that.hr;
#if _MSC_VER < 1400
         wcscpy(message, that.message);
#else
         wcscpy_s(message, that.message);
#endif
      }

      Win32SecError& Win32SecError::operator=(const Win32SecError& that) throw()
      {
         if (this != &that)
         {
            hr = that.hr;
#if _MSC_VER < 1400
            wcscpy(message, that.message);
#else
            wcscpy_s(message, that.message);
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

   private:
      HRESULT hr;
      wchar_t message[512];
   };

   class SIDPtr : public CSid
   {
   public:
      SIDPtr() : CSid()
      {
         ;
      }

      SIDPtr(const SIDPtr &c) : CSid(c)
      {
         ;
      }
      // SLB 31.aug.2007 CR.6135; Added. 
      SIDPtr(const SID *pSid, LPCTSTR pszSystem = NULL) : CSid(pSid, pszSystem)
      {
      }

      SIDPtr(const wchar_t *sid_str)
      {
         PSID psid = 0;
         if (ConvertStringSidToSidW(sid_str,&psid) && psid!=0)
         {
            this->LoadAccount((const SID*)psid);
         }
         
         if (psid)
         {
            LocalFree(psid);
            psid = 0;
         }
      }
      
      operator _bstr_t()
      {
         return _bstr_t(this->Sid());
      }

      bool IsValid()
      {
         return (0!=this->GetPSID() && IsValidSid((PSID)this->GetPSID()));
      }
      
      TRUSTEE Trustee()
      {
         TRUSTEE trustee;
         memset(&trustee, 0, sizeof(TRUSTEE));
         
         if (!IsValid())
            return trustee;
         
         BuildTrusteeWithSid(&trustee, (PSID)this->GetPSID());
         return trustee;
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
      SID_NAME_USE GetSidNameUse()
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
      HRESULT AddToLocalGroup(LPCWSTR group_name, bool throw_win32sec_error)
      {
         // This function only supports user and global group SIDs.
         SID_NAME_USE sid_name_use = GetSidNameUse();
         if (sid_name_use != SidTypeUser && sid_name_use != SidTypeGroup)
         {
            HRESULT hr = E_INVALIDARG;
            if (throw_win32sec_error)
               throw Win32SecError(
                  hr, L"SIDPtr::AddToLocalGroup() - Function only supports user and global group SIDs");
            else
               return hr;
         }

         LOCALGROUP_MEMBERS_INFO_0 local_group_members;
         local_group_members.lgrmi0_sid = (PSID)GetPSID();
         NET_API_STATUS net_api_status = NetLocalGroupAddMembers(
            NULL, group_name, 0, (LPBYTE)&local_group_members, 1);
         if (net_api_status != NERR_Success && net_api_status != ERROR_MEMBER_IN_ALIAS)
         {
            HRESULT hr = HRESULT_FROM_WIN32(net_api_status);
            if (throw_win32sec_error)
               throw Win32SecError(
                  hr, L"SIDPtr::AddToLocalGroup() - NetLocalGroupAddMembers() failed");
            else
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
      HRESULT RemoveFromLocalGroup(LPCWSTR group_name, bool throw_win32sec_error)
      {
         // This function only supports user and global group SIDs.
         SID_NAME_USE sid_name_use = GetSidNameUse();
         if (sid_name_use != SidTypeUser && sid_name_use != SidTypeGroup)
         {
            HRESULT hr = E_INVALIDARG;
            if (throw_win32sec_error)
               throw Win32SecError(
                  hr, L"SIDPtr::RemoveFromLocalGroup() - Function only supports user and global group SIDs");
            else
               return hr;
         }

         LOCALGROUP_MEMBERS_INFO_0 local_group_members;
         local_group_members.lgrmi0_sid = (PSID)GetPSID();
         NET_API_STATUS net_api_status = NetLocalGroupDelMembers(
            NULL, group_name, 0, (LPBYTE)&local_group_members, 1);
         if (net_api_status != NERR_Success && net_api_status != ERROR_MEMBER_IN_ALIAS)
         {
            HRESULT hr = HRESULT_FROM_WIN32(net_api_status);
            if (throw_win32sec_error)
               throw Win32SecError(
                  hr, L"SIDPtr::RemoveFromLocalGroup() - NetLocalGroupDelMembers() failed");
            else
               return hr;
         }
         return S_OK;
      }

      // UserGetGroups()
      //    Gets the local, global, and universal groups that this user belongs
      //    to. Sets group_list with an array of SIDPtrs representing the groups.
      //    If throw_win32sec_error is true and an error occurs, the function
      //    throws a Win32SecError exception rather than returning the invalid
      //    HRESULT.
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
      //
      HRESULT UserGetGroups(std::vector<SIDPtr>& group_list, bool throw_win32sec_error)
      {
         #ifdef SECURITY_OUTPUT_DEBUG
         OutputDebugString(L"   SIDPtr::UserGetGroups ***BEGIN***\r\n");
         wchar_t scratchbuf[512];
         _snwprintf(scratchbuf, sizeof(scratchbuf)/sizeof(wchar_t),
            L"      SID: %s, Domain: %s, Account Name: %s\r\n",
            Sid(), Domain(), AccountName());
         OutputDebugString(scratchbuf);
         #endif

         // The local system account is categorized as a SidTypeWellKnownGroup, which
         // was causing this function to return E_FAIL. Since the local system account
         // cannot be added to any groups, we treat this as a special case and return
         // with an empty group list. If it turns out that the local system account can
         // be a member of a group, this code will need to be revised.
         const wchar_t* local_system_sid = L"S-1-5-18";
#if _MSC_VER < 1400
         if (!wcsicmp(Sid(), local_system_sid))
#else
         if (!_wcsicmp(Sid(), local_system_sid))
#endif
         {
            #ifdef SECURITY_OUTPUT_DEBUG
            OutputDebugString(L"      Local System SID\r\n");
            OutputDebugString(L"   SIDPtr::UserGetGroups ***END1***\r\n");
            #endif

            return S_OK;
         }

         // This function only supports user SIDs.
         if (GetSidNameUse() != SidTypeUser)
         {
            #ifdef SECURITY_OUTPUT_DEBUG
            OutputDebugString(L"      Not a user SID\r\n");
            OutputDebugString(L"   SIDPtr::UserGetGroups ***END2***\r\n");
            #endif

            HRESULT hr = E_INVALIDARG;
            if (throw_win32sec_error)
               throw Win32SecError(
                  hr, L"SIDPtr::UserGetGroups() - Function only supports user SIDs");
            else
               return hr;
         }

         LPCTSTR domain = Domain();
         LPCTSTR user_name = AccountName();

         //
         // Get the local groups from the local computer.
         //
         wchar_t qualified_user_name[512];
#if _MSC_VER < 1400
         _snwprintf(qualified_user_name, sizeof(qualified_user_name)/sizeof(wchar_t), L"%s\\%s", domain, user_name);
#else
         _snwprintf_s(qualified_user_name, sizeof(qualified_user_name)/sizeof(wchar_t), L"%s\\%s", domain, user_name);
#endif
         LPLOCALGROUP_USERS_INFO_0 plocalbuf = NULL;
         DWORD entries_read, total_entries;
         NET_API_STATUS net_api_status = NetUserGetLocalGroups(
            NULL, qualified_user_name, 0, LG_INCLUDE_INDIRECT, (LPBYTE*)&plocalbuf,
            MAX_PREFERRED_LENGTH, &entries_read, &total_entries);
         if (net_api_status != NERR_Success)
         {
            HRESULT hr = HRESULT_FROM_WIN32(net_api_status);

            #ifdef SECURITY_OUTPUT_DEBUG
            _snwprintf(scratchbuf, sizeof(scratchbuf)/sizeof(wchar_t),
               L"      NetUserGetLocalGroups failed: 0x%lX, %ld\r\n", hr, net_api_status);
            OutputDebugString(scratchbuf);
            OutputDebugString(L"   SIDPtr::UserGetGroups ***END3***\r\n");
            #endif

            if (throw_win32sec_error)
               throw Win32SecError(
                  hr, L"SIDPtr::UserGetGroups() - NetUserGetLocalGroups() failed");
            else
               return hr;
         }

         #ifdef SECURITY_OUTPUT_DEBUG
         OutputDebugString(L"      Local Groups:\r\n");
         #endif

         DWORD i;
         LPLOCALGROUP_USERS_INFO_0 plocal_group_info;
         for (i = 0, plocal_group_info = plocalbuf;
            i < entries_read && plocal_group_info;
            i++, plocal_group_info++)
         {
            SIDPtr group_sid;
            if (group_sid.LoadAccount(plocal_group_info->lgrui0_name))
            {
               #ifdef SECURITY_OUTPUT_DEBUG
               _snwprintf(scratchbuf, sizeof(scratchbuf)/sizeof(wchar_t),
                  L"         SID: %s, Domain: %s, Account Name: %s\r\n",
                  group_sid.Sid(), group_sid.Domain(), group_sid.AccountName());
               OutputDebugString(scratchbuf);
               #endif

               group_list.push_back(group_sid);
            }
         }

         net_api_status = NetApiBufferFree(plocalbuf);
         if (net_api_status != NERR_Success)
         {
            HRESULT hr = HRESULT_FROM_WIN32(net_api_status);

            #ifdef SECURITY_OUTPUT_DEBUG
            _snwprintf(scratchbuf, sizeof(scratchbuf)/sizeof(wchar_t),
               L"      NetApiBufferFree failed: 0x%lX, %ld\r\n", hr, net_api_status);
            OutputDebugString(scratchbuf);
            OutputDebugString(L"   SIDPtr::UserGetGroups ***END4***\r\n");
            #endif

            if (throw_win32sec_error)
               throw Win32SecError(
                  hr, L"SIDPtr::UserGetGroups() - NetApiBufferFree(plocalbuf) failed");
            else
               return hr;
         }

         // CR.8222 begin
         // Get the name of the closest domain controller for the domain.
         PDOMAIN_CONTROLLER_INFO pdciDomainController = NULL ;
         DWORD dwNtErr(ERROR_SUCCESS) ;

         dwNtErr = DsGetDcName(NULL,domain,NULL,NULL,0,&pdciDomainController) ;
         if (dwNtErr == ERROR_NO_SUCH_DOMAIN)
         {
            #ifdef SECURITY_OUTPUT_DEBUG
            OutputDebugString(L"      Domain controller not found\r\n");
            OutputDebugString(L"   SIDPtr::UserGetGroups ***END5***\r\n");
            #endif

            return S_OK;
         }
         else if (dwNtErr != ERROR_SUCCESS)
         {
            HRESULT hr = HRESULT_FROM_WIN32(net_api_status);

            #ifdef SECURITY_OUTPUT_DEBUG
            _snwprintf(scratchbuf, sizeof(scratchbuf)/sizeof(wchar_t),
               L"      DsGetDcName failed: 0x%lX, %ld\r\n", hr, net_api_status);
            OutputDebugString(scratchbuf);
            OutputDebugString(L"   SIDPtr::UserGetGroups ***END6***\r\n");
            #endif

            if (throw_win32sec_error)
               throw Win32SecError(
                  hr, L"SIDPtr::UserGetGroups() - DsGetDcName() failed");
            else
               return hr;
         }

         //// Get the name of the primary domain controller for the domain.
         //LPBYTE pDomainController = NULL;
         //net_api_status = NetGetDCName(NULL, domain, &pDomainController);
         //if (net_api_status == NERR_DCNotFound || !_stricmp((const char *)pDomainController, "\\")) // we are not on a domain, return
         //{
         //   #ifdef SECURITY_OUTPUT_DEBUG
         //   OutputDebugString(L"      Domain controller not found\r\n");
         //   OutputDebugString(L"   SIDPtr::UserGetGroups ***END5***\r\n");
         //   #endif
         //
         //   return S_OK;
         //}
         //else if (net_api_status != NERR_Success)
         //{
         //   HRESULT hr = HRESULT_FROM_WIN32(net_api_status);
         //
         //   #ifdef SECURITY_OUTPUT_DEBUG
         //   _snwprintf(scratchbuf, sizeof(scratchbuf)/sizeof(wchar_t),
         //      L"      NetGetDCName failed: 0x%lX, %ld\r\n", hr, net_api_status);
         //   OutputDebugString(scratchbuf);
         //   OutputDebugString(L"   SIDPtr::UserGetGroups ***END6***\r\n");
         //   #endif
         //
         //   if (throw_win32sec_error)
         //      throw Win32SecError(
         //         hr, L"SIDPtr::UserGetGroups() - NetGetDCName() failed");
         //   else
         //      return hr;
         //}
         // CR.8222 end

         //
         // Get the global and universal groups from the domain controller.
         //
         LPGROUP_USERS_INFO_0 pglobalbuf = NULL;
         net_api_status = NetUserGetGroups(
            pdciDomainController->DomainControllerName, // (LPCWSTR)pDomainController, 
            user_name, 0,
            (LPBYTE*)&pglobalbuf, MAX_PREFERRED_LENGTH,
            &entries_read, &total_entries);
         if (net_api_status != NERR_Success)
         {
            HRESULT hr = HRESULT_FROM_WIN32(net_api_status);
            NetApiBufferFree(pdciDomainController) ;

            #ifdef SECURITY_OUTPUT_DEBUG
            _snwprintf(scratchbuf, sizeof(scratchbuf)/sizeof(wchar_t),
               L"      NetUserGetGroups failed: 0x%lX, %ld\r\n", hr, net_api_status);
            OutputDebugString(scratchbuf);
            OutputDebugString(L"   SIDPtr::UserGetGroups ***END7***\r\n");
            #endif
            if (net_api_status == ERROR_LOGON_FAILURE)
            {  // CR.8222; current identity is not authenticated on the specified domain, so it can not retrieve 
               // the Group info for the specified username.
               // Not an error. Treat this as an identity which does not belong to any groups.
               return S_OK ;
            }
            else
            {
               if (throw_win32sec_error)
                  throw Win32SecError(
                     hr, L"SIDPtr::UserGetGroups() - NetUserGetGroups() failed");
               else
                  return hr;
            }
         }

         // In a strange decision by Microsoft, NetUserGetGroups() returns
         // "None" if no groups are found. This is a valid undocumented local
         // group, kind of like the "Everyone" group, but it is not a valid
         // global group in Active Directory, which makes it difficult to
         // handle. It is possible to define a valid global group named "None."
         // Also, even if you assume there are no global groups named "None,"
         // detecting that there are no groups found by comparing to string
         // "None" makes the code English language dependent. I've found
         // complaints on this dating back to 1997, so it doesn't seem like
         // Microsoft has any plans to modify this behavior. We handle this
         // by calling LoadAccount() with a group name of "<domain>\None."
         // If this fails, we simply ignore the "None" group. For a local
         // user, this will succeed since there is always a local "None"
         // group. This means all local users will be in the local "None"
         // group, which isn't desirable but it probably won't hurt anything.
         // For domain users, if there is a global group named "None," it
         // will be added to the list even when "None" was returned for no
         // groups found. Even with these flaws, this seems like the best
         // solution since it will work in any locale.
         
         #ifdef SECURITY_OUTPUT_DEBUG
         OutputDebugString(L"      Global and Universal Groups:\r\n");
         #endif

         LPGROUP_USERS_INFO_0 pglobal_group_info;
         for (i = 0, pglobal_group_info = pglobalbuf;
            i < entries_read && pglobal_group_info;
            i++, pglobal_group_info++)
         {
            wchar_t domain_group_name[512];
#if _MSC_VER < 1400
            _snwprintf(domain_group_name, sizeof(domain_group_name)/sizeof(wchar_t), L"%s\\%s", domain, pglobal_group_info->grui0_name);
#else
            _snwprintf_s(domain_group_name, sizeof(domain_group_name)/sizeof(wchar_t), L"%s\\%s", domain, pglobal_group_info->grui0_name);
#endif
            SIDPtr group_sid;
            if (group_sid.LoadAccount(domain_group_name))
            {
               #ifdef SECURITY_OUTPUT_DEBUG
               _snwprintf(scratchbuf, sizeof(scratchbuf)/sizeof(wchar_t),
                  L"         SID: %s, Domain: %s, Account Name: %s\r\n",
                  group_sid.Sid(), group_sid.Domain(), group_sid.AccountName());
               OutputDebugString(scratchbuf);
               #endif

               group_list.push_back(group_sid);
            }
         }

         net_api_status = NetApiBufferFree(pglobalbuf);
         if (net_api_status != NERR_Success)
         {
            //NetApiBufferFree(pDomainController);
            NetApiBufferFree(pdciDomainController) ;
            HRESULT hr = HRESULT_FROM_WIN32(net_api_status);

            #ifdef SECURITY_OUTPUT_DEBUG
            _snwprintf(scratchbuf, sizeof(scratchbuf)/sizeof(wchar_t),
               L"      NetApiBufferFree failed: 0x%lX, %ld\r\n", hr, net_api_status);
            OutputDebugString(scratchbuf);
            OutputDebugString(L"   SIDPtr::UserGetGroups ***END8***\r\n");
            #endif

            if (throw_win32sec_error)
               throw Win32SecError(
                  hr, L"SIDPtr::UserGetGroups() - NetApiBufferFree(pglobalbuf) failed");
            else
               return hr;
         }

         //net_api_status = NetApiBufferFree(pDomainController);
         net_api_status = NetApiBufferFree(pdciDomainController) ;
         if (net_api_status != NERR_Success)
         {
            HRESULT hr = HRESULT_FROM_WIN32(net_api_status);

            #ifdef SECURITY_OUTPUT_DEBUG
            _snwprintf(scratchbuf, sizeof(scratchbuf)/sizeof(wchar_t),
               L"      NetApiBufferFree failed: 0x%lX, %ld\r\n", hr, net_api_status);
            OutputDebugString(scratchbuf);
            OutputDebugString(L"   SIDPtr::UserGetGroups ***END9***\r\n");
            #endif

            if (throw_win32sec_error)
               throw Win32SecError(
                  hr, L"SIDPtr::UserGetGroups() - NetApiBufferFree(pdciDomainController) failed");
            else
               return hr;
         }

         #ifdef SECURITY_OUTPUT_DEBUG
         OutputDebugString(L"   SIDPtr::UserGetGroups ***END***\r\n");
         #endif

         return S_OK;
      }

      // LocalGroupGetMembers()
      //    Gets a list of the members of the specified local group. Local group
      //    members can be users or global groups. Sets member_list with an array
      //    of SIDPtrs representing the members. If throw_win32sec_error is true
      //    and an error occurs, the function throws a Win32SecError exception
      //    rather than returning the invalid HRESULT.
      static HRESULT LocalGroupGetMembers(
         LPCWSTR group_name, std::vector<SIDPtr>& member_list,
         bool throw_win32sec_error)
      {
         LPLOCALGROUP_MEMBERS_INFO_0 plocalbuf = NULL;
         DWORD entries_read, total_entries;
         NET_API_STATUS net_api_status = NetLocalGroupGetMembers(
            NULL, group_name, 0, (LPBYTE*)&plocalbuf, MAX_PREFERRED_LENGTH,
            &entries_read, &total_entries, NULL);
         if (net_api_status != NERR_Success)
         {
            HRESULT hr = HRESULT_FROM_WIN32(net_api_status);
            if (throw_win32sec_error)
               throw Win32SecError(
                  hr, L"SIDPtr::LocalGroupGetMembers() - NetLocalGroupGetMembers() failed");
            else
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
               member_list.push_back(member_sid);
         }

         net_api_status = NetApiBufferFree(plocalbuf);
         if (net_api_status != NERR_Success)
         {
            HRESULT hr = HRESULT_FROM_WIN32(net_api_status);
            if (throw_win32sec_error)
               throw Win32SecError(
                  hr, L"SIDPtr::LocalGroupGetMembers() - NetApiBufferFree(plocalbuf) failed");
            else
               return hr;
         }

         return S_OK;
      }
   };

   class SecurityDescriptorPtr : public CSecurityDesc
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
         this->FromString(sd_str);
      }

      bool IsValid()
      {
         return (0!=this->GetPSECURITY_DESCRIPTOR() && IsValidSecurityDescriptor((PSECURITY_DESCRIPTOR)this->GetPSECURITY_DESCRIPTOR()));
      }

      operator _bstr_t()
      {
         CString cs;
         if (IsValid() && this->ToString(&cs))
         {
            return _bstr_t((const wchar_t*)cs);
         }
         else
         {
            return _bstr_t(L"");
         }
      }

      // GetEffectivePermissions()
      //    If throw_win32sec_error is true and an error occurs, the function
      //    throws a Win32SecError exception rather than returning the invalid
      //    HRESULT.
      HRESULT GetEffectivePermissions(
         SIDPtr &sid, unsigned long desired_access_mask,
         unsigned long *granted_access_mask, bool throw_win32sec_error) const
      {
         #ifdef SECURITY_OUTPUT_DEBUG
         OutputDebugString(L"   SecurityDescriptorPtr::GetEffectivePermissions ***BEGIN***\r\n");
         wchar_t scratchbuf[512];
         _snwprintf(scratchbuf, sizeof(scratchbuf)/sizeof(wchar_t),
            L"      SID: %s, Domain: %s, Account Name: %s\r\n",
            sid.Sid(), sid.Domain(), sid.AccountName());
         OutputDebugString(scratchbuf);
         #endif

         *granted_access_mask = 0x00000000;

         // DCM CR.Fix.4838.v01 - Due to a Microsoft bug in GetEffectiveRightsFromAcl(),
         // we must use AuthzAccessCheck() to get effective permissions. This is
         // somewhat more complex than using GetEffectiveRightsFromAcl(), the MSDN
         // documentation for these functions is poor, and there is very little info
         // regarding these functions on the web, so it is possible there are some
         // bugs in this code.
         AUTHZ_RESOURCE_MANAGER_HANDLE authz_resource_manager = 0;
         AUTHZ_CLIENT_CONTEXT_HANDLE authz_client = 0;
         AUTHZ_ACCESS_CHECK_RESULTS_HANDLE authz_handle = 0;
         __try
         {
            // Create the resource manager needed by AuthzInitializeContextFromSid().
            // Not sure if we should be using AUTHZ_RM_FLAG_INITIALIZE_UNDER_IMPERSONATION,
            // but a bitwise OR with this causes the function to fail.
            if (!AuthzInitializeResourceManager(
               AUTHZ_RM_FLAG_NO_AUDIT, NULL, NULL, NULL, NULL, &authz_resource_manager))
            {
               DWORD error = GetLastError();
               HRESULT hr = HRESULT_FROM_WIN32(error);

               #ifdef SECURITY_OUTPUT_DEBUG
               _snwprintf(scratchbuf, sizeof(scratchbuf)/sizeof(wchar_t),
                  L"      AuthzInitializeResourceManager failed: 0x%lX, %ld\r\n", hr, error);
               OutputDebugString(scratchbuf);
               OutputDebugString(L"   SecurityDescriptorPtr::GetEffectivePermissions ***END1***\r\n");
               #endif

               if (throw_win32sec_error)
                  throw Win32SecError(
                     hr, L"SecurityDescriptorPtr::GetEffectivePermissions() - AuthzInitializeResourceManager() failed");
               else
                  return hr;
            }

            // Create a user-mode client context from the SID.
            // SLB 
            HRESULT hr = InitializeAuthzClientContext((const PSID)sid.GetPSID(), authz_resource_manager, authz_client) ;
            if (FAILED(hr))
            {
               #ifdef SECURITY_OUTPUT_DEBUG
               _snwprintf(scratchbuf, sizeof(scratchbuf)/sizeof(wchar_t),
                  L"      AuthzInitializeContextFromSid failed: 0x%lX, %ld\r\n", hr, error);
               OutputDebugString(scratchbuf);
               OutputDebugString(L"   SecurityDescriptorPtr::GetEffectivePermissions ***END2***\r\n");
               #endif

               if (throw_win32sec_error)
                  throw Win32SecError(
                     hr, L"SecurityDescriptorPtr::GetEffectivePermissions() - InitializeAuthzClientContext() failed");
               else
                  return hr;
            }

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
            PSECURITY_DESCRIPTOR p_security_descriptor =
               (PSECURITY_DESCRIPTOR)this->GetPSECURITY_DESCRIPTOR();
            for (unsigned long i = 1; i; i <<= 1) // for every bit
            {
               if (i & desired_access_mask)
               {
                  access_request.DesiredAccess = i;
                  *access_reply.GrantedAccessMask = 0;
                  // The first call is to AuthzAccessCheck() and subsequent calls are to
                  // AuthzCachedAccessCheck(). Specifying AUTHZ_ACCESS_CHECK_NO_DEEP_COPY_SD
                  // in AuthzAccessCheck() might be slightly more efficient, but that flag
                  // is poorly documented so we don't currently use it.
                  if (!authz_handle)
                  {
                     if (!AuthzAccessCheck(
                        0, authz_client, &access_request, NULL, p_security_descriptor, NULL,
                        0, &access_reply, &authz_handle))
                     {
                        DWORD error = GetLastError();
                        HRESULT hr = HRESULT_FROM_WIN32(error);

                        #ifdef SECURITY_OUTPUT_DEBUG
                        _snwprintf(scratchbuf, sizeof(scratchbuf)/sizeof(wchar_t),
                           L"      AuthzAccessCheck failed: 0x%lX, %ld\r\n", hr, error);
                        OutputDebugString(scratchbuf);
                        OutputDebugString(L"   SecurityDescriptorPtr::GetEffectivePermissions ***END3***\r\n");
                        #endif

                        if (throw_win32sec_error)
                           throw Win32SecError(
                              hr, L"SecurityDescriptorPtr::GetEffectivePermissions() - AuthzAccessCheck() failed");
                        else
                           return hr;
                     }
                  }
                  else
                  {
                     if (!AuthzCachedAccessCheck(
                        0, authz_handle, &access_request, NULL, &access_reply))
                     {
                        DWORD error = GetLastError();
                        HRESULT hr = HRESULT_FROM_WIN32(error);

                        #ifdef SECURITY_OUTPUT_DEBUG
                        _snwprintf(scratchbuf, sizeof(scratchbuf)/sizeof(wchar_t),
                           L"      AuthzCachedAccessCheck failed: 0x%lX, %ld\r\n", hr, error);
                        OutputDebugString(scratchbuf);
                        OutputDebugString(L"   SecurityDescriptorPtr::GetEffectivePermissions ***END4***\r\n");
                        #endif

                        if (throw_win32sec_error)
                           throw Win32SecError(
                              hr, L"SecurityDescriptorPtr::GetEffectivePermissions() - AuthzCachedAccessCheck() failed");
                        else
                           return hr;
                     }
                  }
                  *granted_access_mask |= *access_reply.GrantedAccessMask;
               }
            }
         }
         __finally
         {
            if (AbnormalTermination())
            {
               OutputDebugStringW(L"\nGetEffectivePermissions(): caught exception") ;
            }
            if (authz_handle)
               AuthzFreeHandle(authz_handle);
            if (authz_client)
               AuthzFreeContext(authz_client);
            if (authz_resource_manager)
               AuthzFreeResourceManager(authz_resource_manager);
         }

         #ifdef SECURITY_OUTPUT_DEBUG
         OutputDebugString(L"   SecurityDescriptorPtr::GetEffectivePermissions ***END***\r\n");
         #endif

         return S_OK;

         /* DCM 05.oct.05 - KEEP GetEffectiveRightsFromAcl() code.         
         TRUSTEE trustee = sid.Trustee();
         
         bool dacl_present, dacl_defaulted;
         CDacl dacl;
         
         if (!IsValid())
         {
            OutputDebugString(L"CSpdSecuritySetup::Win32Sec::SecurityDescriptorPtr::GetEffectivePermissions() encountered an null Security Descriptor pointer or an empty Security Descriptor. Granting full access.\r\n");
            *granted_access_mask = 0xFFFFFFFF;
            return S_FALSE;
         }
         
         if (!this->GetDacl(&dacl, &dacl_present, &dacl_defaulted))
            return HRESULT_FROM_WIN32(GetLastError());
         
         if (dacl_present && dacl)
         {
            return HRESULT_FROM_WIN32(GetEffectiveRightsFromAcl((PACL)dacl.GetPACL(), &trustee, granted_access_mask));
         }
         else
         {
            OutputDebugString(L"CSpdSecuritySetup::Win32Sec::SecurityDescriptorPtr::GetEffectivePermissions() encountered a null or empty DACL, granting full access.\r\n");
            *granted_access_mask = 0xFFFFFFFF;
            return S_FALSE;
         }
         */
      }

      PSECURITY_DESCRIPTOR Copy()
      {
         //if (!IsValid()) return 0;
         //PSECURITY_DESCRIPTOR psd = (PSECURITY_DESCRIPTOR)GetPSECURITY_DESCRIPTOR();
         return 0;
      }
      private:
      // InitializeAuthzClientContext()
      //
      // SLB 07.aug.2007 Created from existing code in GetEffectivePermissions. CR.8222; Added this 
      // method so as to obfuscate the retry of AuthzInitializeContextFromSid() that occurs if the first 
      // attempt fails with ACCESS_DENIED. This fixes issue when a computer is on the domain, a domain 
      // client is attempting to connect but the server is running as a local identity (vs. one on the 
      // domain). I set this up to return an HR vs. throw an exception. I am not sure which method Dylan 
      // would have preferred.  It can be changed though. 
      // SLB 31.aug.2007 CR.6135; added _try/_finally block.
      //********************************************************************************************************************
      HRESULT InitializeAuthzClientContext(
         const PSID psid, 
         AUTHZ_RESOURCE_MANAGER_HANDLE authz_resource_manager, 
         AUTHZ_CLIENT_CONTEXT_HANDLE &authz_client
         ) const
      {
         LUID luid = {0x0,0x0} ;
         DWORD error(ERROR_SUCCESS) ;
         HRESULT hr(S_OK) ;
         //
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
         __try
         {
         if (!AuthzInitializeContextFromSid(
            0, psid, authz_resource_manager, NULL, luid, NULL,
            &authz_client))
         {  //
            // CR.8222; the AuthzInitializeContextFromSid() S4U logon probably failed (called when it tries to
            // determine what groups the SID belongs too).  Though the group info could not be retrieved, the context
            // should be created with the SID alone. Retry the AuthzInitializeContextFromSid() call, but tell it to 
            // skip looking for the SIDs group membership.
            //
            error = GetLastError() ;
            if (error != ERROR_ACCESS_DENIED)
            {
               hr = HRESULT_FROM_WIN32(error) ;
            }
            else 
            {
               if (!AuthzInitializeContextFromSid(
                  AUTHZ_SKIP_TOKEN_GROUPS, psid, authz_resource_manager, NULL, luid, NULL,
                  &authz_client))
               {
                  error = GetLastError() ;
                  hr = HRESULT_FROM_WIN32(error) ;
               }
            }
         }
         }
         __finally
         {
            if (AbnormalTermination())
            {
               OutputDebugStringW(L"\nInitializeAuthzClientContext(): caught exception") ;
               hr = E_FAIL ;
            }
         }
         return hr ;
      }
   };
};

#endif
