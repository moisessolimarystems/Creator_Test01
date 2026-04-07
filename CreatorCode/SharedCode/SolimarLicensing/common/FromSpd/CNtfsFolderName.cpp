// filename: CNtfsFolderName.cpp 
//
#include "L_CNtfsFolderName.h"
#include "L_Win32Sec.h"
#include <shlobj.h>
#include <lm.h>
#include "L_CSpdBlock.h"                                                              // CR.16991; added.
#include <list>                                                                     // CR.29220; added.

#pragma warning(disable: 26812)  // The enum type 'xxxx' is unscoped. Prefer 'enum class' over 'enum'

using namespace SpdUtils;                                                           // CR.26999.Item55; added.
//*******************************************************************
//
// class CNtfsFolderName: 
//
// Note: this may get added to the SpdUtils namespace, it depends
// on how functional this class proves to be.
//                         
//
// description: 
// 
// Functionality/Responsiblity:
//
//*******************************************************************
CNtfsFolderName::TAccessFlagDescriptionMap CNtfsFolderName::s_AccessFlagDescriptions; 
bool CNtfsFolderName::s_bIsExtendedLoggingOn(false); 
Win32Sec::CSecurityContext CNtfsFolderName::s_scSecurityContext(Win32Sec::SIDPtr::GetProcessSID()); 
// CR.31266; No longer used named mutex for CNtfsFolderName because other 
// processes use this DLL and this will cause performance issues if they all need 
// to sync up with a global mutex.
CSmartMutex CNtfsFolderName::s_hScMutex;                                            // CR.31266; modified. CR.29294; modified. CR.16991; added.
bool CNtfsFolderName::s_bStaticsAreInitialized = false;                             // CR.29412; added.
CNtfsFolderName::TCStringSet CNtfsFolderName::s_SpecialShares;                      // CR.29412; added.

/* constructor - default constructor.
 *
 * SLB 01.apr.2017 CR.23305; initialize new members 
 * SLB 03.jun.2020 CR.29412; Now calls InitializeStatics() so that static collections will get 
 * initialized, if they have not been already.
 * SLB 23.jan.2023 CR.32082; Added support for tracking folder/file name
 * initialization.
 *------------------------------------------------------------------*/
CNtfsFolderName::CNtfsFolderName() : 
   m_wsUncHostName(L"."),
   m_wsPathName(L"\\"),
   m_bIsFolderInitialized(false),                                                   // CR.32082; added.
   m_bIsFileInitialized(false),                                                     // CR.32082; added.
   m_bIsUncFormat(true),
   m_bWriteAccessDesired(true),                                                     // CR.23305; added.
   m_bMappedLocalPathInitialized(false)                                             // CR.23305; added.
{
   InitializeStatics();                                                             // CR.29412; added.
}

/* constructor - copy constructor.
 *------------------------------------------------------------------*/
CNtfsFolderName::CNtfsFolderName(const CNtfsFolderName &c)
{
   *this = c; 
}

/* constructor - constructor which parses a known folder string and poplulates
 * this object with the contents found.
 * 
 * SLB 23.jan.2023 CR.32082; Added support for tracking folder/file name
 * initialization.
 *------------------------------------------------------------------*/
CNtfsFolderName::CNtfsFolderName(
   const CSpdCString &wsFullFolderName,
   bool bWriteAccessDesired
   ) :
   m_bIsFolderInitialized(false),                                                   // CR.32082; added.
   m_bIsFileInitialized(false),                                                     // CR.32082; added.
   m_bIsUncFormat(false),
   m_bWriteAccessDesired(bWriteAccessDesired),
   m_bMappedLocalPathInitialized(false)                                             // CR.23305; added.
{
   SetFullFolderName(wsFullFolderName); 
}


/* constructor - constructor which takes individual paramaters, for 
 * the drive map format, to populate this object.
 * 
 * SLB 18.jan.2023 CR.32082; Now sets folder initialized = true after setting the
 * full path up.
 * SLB 23.jan.2023 CR.32082; Added support for tracking folder/file name
 * initialization.
 *------------------------------------------------------------------*/
CNtfsFolderName::CNtfsFolderName(
   const CSpdCString &wsDriveLetter,
   const CSpdCString &wsPathName,
   const CSpdCString &wsFileName,
   const CSpdCString &wsFileExt,
   bool bWriteAccessDesired
   ) :
   m_bIsFolderInitialized(false),                                                   // CR.32082; added.
   m_bIsFileInitialized(false),                                                     // CR.32082; added.
   m_bIsUncFormat(true),
   m_bWriteAccessDesired(bWriteAccessDesired),
   m_bMappedLocalPathInitialized(false)                                             // CR.23305; added.
{
   SetUncHostName(L"."); 
   SetDriveLetter(wsDriveLetter); 
   SetPathName(wsPathName); 
   SetFileName(wsFileName, false); 
   SetFileExt(wsFileExt); 
   m_bIsFolderInitialized = true;                                                   // CR.32082; added.
}

/* constructor - constructor which takes individual paramaters, for 
 * the UNC map format, to populate this object.
 * 
 * SLB 18.jan.2023 CR.32082; Now sets folder initialized = true after setting the
 * full path up.
 *------------------------------------------------------------------*/
CNtfsFolderName::CNtfsFolderName(
   const CSpdCString &wsUncHostName,
   const CSpdCString &wsUncShareName,
   const CSpdCString &wsPathName,
   const CSpdCString &wsFileName,
   const CSpdCString &wsFileExt,
   bool bWriteAccessDesired
   ) :
   m_bIsFolderInitialized(false),                                                   // CR.32082; added.
   m_bIsFileInitialized(false),                                                     // CR.32082; added.
   m_bIsUncFormat(true),
   m_bWriteAccessDesired(bWriteAccessDesired),
   m_bMappedLocalPathInitialized(false)                                             // CR.23305; added.
{
   SetUncHostName(wsUncHostName); 
   SetUncShareName(wsUncShareName); 
   SetPathName(wsPathName); 
   SetFileName(wsFileName, false); 
   SetFileExt(wsFileExt); 
   m_bIsFolderInitialized = true;                                                   // CR.32082; added.
}



/* operator =() -
 * 
 * SLB 23.jan.2023 CR.32082; Added support for tracking folder/file name
 * initialization.
 *------------------------------------------------------------------*/
CNtfsFolderName & CNtfsFolderName::operator =(const CNtfsFolderName &c)
{
   m_bIsFolderInitialized = c.m_bIsFolderInitialized;                               // CR.32082; added.
   m_bIsFileInitialized = c.m_bIsFileInitialized;                                   // CR.32082; added.

   m_bIsUncFormat = c.m_bIsUncFormat; 
   m_bWriteAccessDesired = c.m_bWriteAccessDesired; 
   m_wsDriveLetter = c.m_wsDriveLetter; 
   m_wsUncHostName = c.m_wsUncHostName; 
   m_wsUncShareName = c.m_wsUncShareName; 
   m_wsPathName = c.m_wsPathName; 
   m_wsFileName = c.m_wsFileName; 
   m_wsFileExt = c.m_wsFileExt; 
   m_wsMappedLocalPathName = c.m_wsMappedLocalPathName;                             // CR.23305; added.
   m_bMappedLocalPathInitialized = c.m_bMappedLocalPathInitialized;                 // CR.23305; added.
   return *this; 
}

/* operator =() -
 *------------------------------------------------------------------*/
CNtfsFolderName & CNtfsFolderName::operator =(const CSpdCString &ws)
{
   SetFullFileName(ws); 
   return *this; 
}


/* IsFolderNameInitialized_Check() - Throws exception if the folder name has not
 * been initialized yet.
 *
 * SLB 08.nov.2022 CR.32082; created.
 *------------------------------------------------------------------*/
void CNtfsFolderName::IsFolderNameInitialized_Check(
   const wchar_t* pwcsFunc) const
{
   if (!IsFolderNameInitialized()) ThrowHr(E_INVALIDARG, pwcsFunc, L"Internal error. Cannot execute method when Folder name info has not been initialized.");
}

/* IsFileNameInitialized_Check() - Throws exception if the file name has not
 * been initialized yet.
 *
 * SLB 08.nov.2022 CR.32082; created.
 *------------------------------------------------------------------*/
void CNtfsFolderName::IsFileNameInitialized_Check(
   const wchar_t* pwcsFunc) const
{
   if (!IsFileNameInitialized()) ThrowHr(E_INVALIDARG, pwcsFunc, L"Internal error. Cannot execute method when File name info has not been initialized.");
}

/* IsPathNameSet_Check() - Throws exception if the pathname member is empty.
 *
 * SLB 08.mar.2023 CR.32260; created.
 *------------------------------------------------------------------*/
void CNtfsFolderName::IsPathNameSet_Check(
   const wchar_t* pwcsFunc) const
{
   // m_wsPathName must not be empty. And should end with '\' char.
   if (m_wsPathName.empty()) ThrowHr(E_INVALIDARG, __WFUNCTION__, L"Path value is required in order to change directory.");
   if (m_wsPathName[m_wsPathName.wcslen() - 1] != L'\\') ThrowHr(E_INVALIDARG, __WFUNCTION__, L"Path value is not in a valid format. Path <%s>", m_wsPathName.c_str());
}

/* GetFileName() -
 *------------------------------------------------------------------*/
void CNtfsFolderName::GetFileName(
   CSpdCString &ws, 
   bool bIncludeExt) const 
{ 
   ws = m_wsFileName;  
   if (bIncludeExt && !m_wsFileExt.empty())
   {
      ws += L"."; 
      ws += m_wsFileExt; 
   }
}


/* GetFullFileName() -
 * 19.jun.2006 - CR.6469; added option to request a file name using a locally mapped
 * driver letter vs. a UNC path. 
 * 22.jun.2006 - CR.????; Performance issues occuring with changes from CR.6469.
 * SLB 03.apr.2012 CR.15937; Cleaned out old commented code and debug message.
 * SLB 23.jan.2023 CR.32082; Now throws an exception if this object's Foldername or
 * Filename is not initialized.
 *------------------------------------------------------------------*/
void CNtfsFolderName::GetFullFileName(
   CSpdCString &ws, 
   bool bIncludeExt, 
   bool bDriverFormatIsDesired, 
   bool bExtraLongName, 
   bool bLocalMapFolder) const 
{
   IsFileNameInitialized_Check(__WFUNCTION__);                                      // CR.32082; added.
   IsFolderNameInitialized_Check(__WFUNCTION__);                                    // CR.32082; added.

   GetFullFolderName(ws, true, bDriverFormatIsDesired, bExtraLongName);
   ws += m_wsFileName; 
   if (bIncludeExt && !m_wsFileExt.empty())
   {
      ws += L"."; 
      ws += m_wsFileExt; 
   }
}

/* GetFolderRoot() -
 * 
 * SLB 08.nov.2022 CR.32082; Created.
 * SLB 23.jan.2023 CR.32082; Now throws an exception if this object's Foldername 
 * is not initialized.
 *------------------------------------------------------------------*/
CSpdCString CNtfsFolderName::GetFolderRoot(
   bool bAddEndingWhack,
   bool bDriverFormatIsDesired,
   bool bExtraLongName) const
{
   IsFolderNameInitialized_Check(__WFUNCTION__);                                    // CR.32082; added.

   if (bExtraLongName && bDriverFormatIsDesired) ThrowHr(E_INVALIDARG, __WFUNCTION__, L"Internal error. bDriverFormatIsDesired and bExtraLongNames are both specified.");

   CSpdCString wsRoot;                                                              // CR.15602; added.

   // if extra long format is desired, prepend required string.
   if (bDriverFormatIsDesired)                                                      // CR.15602; added.
   {
      // For driver/kernel format folder name is structured:
      // "\??\<DriveLetter>:\"
      wsRoot = L"\\??\\";                                                           // CR.15937; modified.
   }
   else if (bExtraLongName)                                                         // CR.15602; added.
   {
      // For Long file names:
      // Local Filename root folder format:  "\\?\<DriveLetter>:\"
      // UNC Filename root folder format:    "\\?\UNC\<server>\<share>\"
      wsRoot = L"\\\\?\\";
   }

   if (m_bIsUncFormat)
   {

      // UNC format
      if (GetUncHostName().empty()) ThrowHr(E_INVALIDARG, __WFUNCTION__, L"Internal error. Format is set to UNC but hostname is empty.");
      if (GetUncShareName().empty()) ThrowHr(E_INVALIDARG, __WFUNCTION__, L"Internal error. Format is set to UNC but sharename is empty.");

      if (bDriverFormatIsDesired)
      {
         // Driver format requires UNC path be mapped to a local mapping.
         // Create a UNC path name for user feedback in messages.
         CSpdCString wsMsgUncRoot = GetUncHostName() + CSpdCString(L"\\") + GetUncShareName();
            
         // Create Locally mapped form of folder name.
         CSpdCString wsLocalFolderName;
         DWORD dwNtErr(ERROR_SUCCESS);
         if (!UncNameToLocalMapFolder(wsLocalFolderName, dwNtErr)) ThrowHr(E_INVALIDARG, __WFUNCTION__, L"Error(%04X): Driver format specified, but unable to map share <%s> to a local drive.", dwNtErr, wsMsgUncRoot.c_str());

         // Append Locally mapped folder name to the existing prefix.
         CNtfsFolderName fnLocalFolderName(wsLocalFolderName);
         wsRoot += fnLocalFolderName.GetDriveLetter();
         wsRoot += L":";
      }
      else
      {
         if (bExtraLongName)
         {
            //  SLB 14.mar.2006; Support for extra long file names.
            //  from MSDN: To specify such a UNC path(over 256 chars), use the "\\?\UNC\" prefix. For 
            //  example, "\\?\UNC\<server>\<share>". 
            wsRoot += L"UNC\\";                                                     // CR.15602; modified.
         }
         else
         {
            wsRoot = L"\\\\";
         }
         wsRoot += m_wsUncHostName;

         // Add share name to path string.
         // After this, the path string is ready to add folders.
         wsRoot += L"\\";
         wsRoot += m_wsUncShareName;
      }
   }
   else
   {
      // Drive letter format
      if (m_wsDriveLetter.empty()) ThrowHr(E_INVALIDARG, __WFUNCTION__, L"Internal error. Local mapping format is set but the drive letter is empty.");

      // Add Drive letter to path string.
      // After this, the path string is ready to add folders.
      wsRoot += m_wsDriveLetter;
      wsRoot += L":";
   }

   if (bAddEndingWhack) wsRoot += L"\\";

   return wsRoot;
}


/* GetFullFolderName() -
 *
 * SLB 15.aug.2010 CR.13337; Changed usage of _snwprintf() to _snwprintf_s().
 * SLB 04.jan.2012 CR.15602; Overhauled to support local folder naming better...
 * Now stores changes in temp variable before setting output. Output only gets set if no errors occurred. If errors
 * do occur, a debug message is generated describing the error.
 * Begins building this by adding extra long formatting information first. Then adds drive/UNC info as appropriate.
 * SLB 03.apr.2012 CR.15937; Driver formatted file names were not being generated correctly. Two things:
 * 1) needed to support generating a driver formatted file name, even if current format is UNC
 * 2) Generated name was beginning with "\?\" vs. "\??\". 
 * SLB 23.jan.2023 CR.32082; Now throws an exception if this object's Foldername
 * is not initialized.
 *------------------------------------------------------------------------------------------------------*/
void CNtfsFolderName::GetFullFolderName(
   CSpdCString &ws, 
   bool bAddEndingWhack, 
   bool bDriverFormatIsDesired, 
   bool bExtraLongNames) const
{
   IsFolderNameInitialized_Check(__WFUNCTION__);                                    // CR.32082; added.

   // clear output string before continuing.
   ws.clear(); 

   // Initialize our foldername string with the folder root, no ending "\" char.
   // Note: GetFolderRoot() throws hr_exceptions.
   CSpdCString wsTempName = GetFolderRoot(false, bDriverFormatIsDesired, bExtraLongNames); // CR.32082x; modified. CR.15602; added.

   // Now Add folder info to path string.
   if (m_wsPathName.empty()) ThrowHr(E_INVALIDARG, __WFUNCTION__, L"Internal error. Retrieving folder name, but path is not set.");

   wsTempName += m_wsPathName; 
   if (!bAddEndingWhack)
   {  
      // no ending '\\' desired...
      // erase it if this is not the root folder
      if (m_wsPathName.wcslen() > 1)
      {  
         // not the root folder, erase the ending whack.
         wsTempName.erase(wsTempName.wcslen() -1); 
      }
   }

   // construction of path string is complete.
   // assign output member the constructed value.
   ws = wsTempName; 
}


/* SetDriveLetter() - Stores Drive Letter and ensures it is stored in the 
 * correct internal format.  Then sets UNC share name to the drive letter as
 * well.
 *
 * Internal format: Drive letter consists of only the drive letter.  It does 
 * not have a ':' or '\\' char. in it.
 *------------------------------------------------------------------*/
void CNtfsFolderName::SetDriveLetter(
   const CSpdCString &ws) 
{  
   size_t stPos(0); 
   wchar_t wcsLetter[2] = {0,0}; 

   // clear current Driver letter setting.
   m_wsDriveLetter.clear(); 

   // setup wcLetter
   if (ws.wcslen() > 0 &&
      isalpha(ws.c_str()[0]))
   {
      wcsLetter[0] = towupper(ws.c_str()[0]); 

      if (ws.wcslen()>1 && 
         ws.c_str()[1]==L':')
      {
         m_wsDriveLetter.assign(wcsLetter); 
         m_bIsUncFormat = false; 
      }
      else if (ws.wcslen()==1)
      {
         m_wsDriveLetter.assign(wcsLetter); 
         m_bIsUncFormat = false; 
      }
      else
      {  
         // problem: this is not a valid drive letter format.
      }
   }
}


/* SetUncHostName() - Stores UNC host name and ensures it is stored in the 
 * correct internal format.
 *
 * Internal format: Unc host name does not have beginning or ending '\\' chars.
 *
 * SLB 01.apr.2017 CR.23305; Now Resets m_bMappedLocalPathInitialized.
 * SLB 05.mar.2020 CR.29058; Now converts hostname to a fully qualified network
 * name, if caller specifies.
 * SLB 12.mar.2020 CR.29148; Special handling of generating fully qualified machine names..
 * Now handles the localhost case by converting it the the current computer name.
 * SLB 18.mar.2020 CR.29058.V01; Moved FQDN conversion code into a static method, and now
 * call that method to convert.
 *------------------------------------------------------------------*/
void CNtfsFolderName::SetUncHostName(
   const CSpdCString &ws,
   bool bMakeFullyQualified)
{  
   m_wsUncHostName = ws; 

   // If host name gets reset, it invalidates any local mappings done.
   m_bMappedLocalPathInitialized = false;                                           // CR.23305; added.

   // remove any preceding '\\' chars.
   size_t stPos(0);
   stPos = m_wsUncHostName.find_first_not_of(L'\\');
   if (stPos != CSpdCString::npos)
   {
      m_wsUncHostName.erase(0, stPos); 
   }

   // ensure any trailing '\\' chars are removed
   while (
      m_wsUncHostName.wcslen() &&
      m_wsUncHostName.c_str()[m_wsUncHostName.wcslen() - 1] == L'\\')
   {
      m_wsUncHostName.erase(m_wsUncHostName.wcslen() - 1); 
   }

   // All inappropriate chars removed. Check to see if there still is a name.
   if (m_wsUncHostName.empty()) return;

   // There is.
   m_bIsUncFormat = true; 

   if (!bMakeFullyQualified) return;                                                // CR.29058; added.

   // CR.29148; handle localhost situation.
   if (m_wsUncHostName.compare(L"localhost", false) == 0) m_wsUncHostName = GetComputerName(); // CR.29148; added.

   m_wsUncHostName = GetMacineNameFQ(m_wsUncHostName);                              // CR.29058.V01; added.
}


/* SetUncShareName() - Stores UNC Share name and ensures it is stored in the 
 * correct internal format.
 *
 * Internal format: Unc share name does not have beginning or ending '\\' chars.
 *
 * SLB 01.apr.2017 CR.23305; Always calls InitializeMappedShare() when share is changed.
 *------------------------------------------------------------------*/
void CNtfsFolderName::SetUncShareName(
   const CSpdCString &ws) 
{ 
   size_t stPos(0);  
   m_wsUncShareName = ws; 

   // remove any preceding '\\' chars.
   stPos = m_wsUncShareName.find_first_not_of(L'\\'); 
   if (stPos != CSpdCString::npos)
   {
      m_wsUncShareName.erase(0, stPos); 
   }

   // ensure any trailing '\\' chars are removed
   while (
      m_wsUncShareName.wcslen() &&
      m_wsUncShareName.c_str()[m_wsUncShareName.wcslen() -1] == L'\\')
   {
      m_wsUncShareName.erase(m_wsUncShareName.wcslen() -1); 
   }
   if (m_wsUncShareName.wcslen())
   {
      m_bIsUncFormat = true; 
   }
   InitializeMappedShare();                                                         // CR.23305; added.
}


/* SetPathName() - Stores pathname and ensures it is stored in the 
 * correct internal format.
 *
 * Internal format: Path name always begins and ends with a '\\' char and ends with
 * a '\\' char.
 * If the the path is referencing the root folder, then there will only be
 * one '\\' char.
 *
 * SLB 02.apr.2020 CR.29220; Now "scrubs" the path before assigning it.
 *------------------------------------------------------------------*/
void CNtfsFolderName::SetPathName(
   const CSpdCString &ws) 
{  
   m_wsPathName.clear();

   // CR.29220; Moved path scrubbing (CR.17124.Item20) here from SetFullFolderName().
   // CR.17124.Item20; scrub path here.
   // convert forward slashes to back slashes
   CSpdCString wsPath(ws);
   for (size_t stPos = wsPath.find(L"//"); stPos != CSpdCString::npos; stPos = wsPath.find(L"//")) // CR.17124.Item20; added for(;;).
   {
      wsPath[stPos] = L'\\';
   }

   // remove duplicate slashes from path name.
   for (size_t stPos = wsPath.find(L"\\\\"); stPos != CSpdCString::npos; stPos = wsPath.find(L"\\\\")) // CR.17124.Item20; added for(;;).
   {
      wsPath.erase(stPos, 1);
   }

   // SLB 02.apr.2020 scrubbing note: 
   // it might be a good idea to add checking for the "." and ".." chars and adjust the pathname
   // accordingly. But it is been fine so far, so I am not bothering with that.

   const CSpdCString wsWhack(L"\\"); 

   // ensure pathname always begins with a '\'.
   if (wsPath.empty() || wsPath[0] != L'\\')
   {
      m_wsPathName += wsWhack; 
   }

   m_wsPathName += wsPath;

   // ensure pathname always ends with a '\'
   if (m_wsPathName[m_wsPathName.wcslen() - 1] != L'\\')
   {
      m_wsPathName += wsWhack; 
   }
}


/* SetFileName() - Stores file name and ensures it is stored in the 
 * correct internal format.
 *
 * Internal format: file name does not have any '\\' chars.  It never ends
 * with a '.' char
 * 
 * SLB 07.nov.2016 CR.22523; Fix for not resetting extension when input has not extension, but bHasExtension is true.
 * SLB 01.apr.2020 CR.29196; Fix for not clearing existing values.
 * SLB 23.jan.2023 CR.32082; Now sets filename initialized to true.
 *------------------------------------------------------------------------------------------------------*/
void CNtfsFolderName::SetFileName(
   const CSpdCString &wsFileName, 
   bool bHasExtension, 
   bool bFixFileName,
	const CSpdCString& wsFixNameReplaceChars)														// CR.32574; added.
{  
   // Clear current values. 
   ClearFileName(bHasExtension);                                                    // CR.29196; added.

   CSpdCString wsFixedName; 
   const CSpdCString *pws(bFixFileName ? &wsFixedName : &wsFileName); 

   size_t stExtPos(0); 

   if (bFixFileName)
      SpdUtils::FixFileNameNtfs(wsFileName, wsFixNameReplaceChars, wsFixedName);		// CR.32574; modified.
   
   // CR.32082; if Filename was set, then make sure to indicate that it is initialized.
   if (!pws->empty()) m_bIsFileInitialized = true;                                  // CR.32082; added.

   stExtPos = pws->find_last_of(L'.'); 
   if (!bHasExtension || stExtPos == CSpdCString::npos)
   {  
      // There is no extension for this filename.
      // If "has extension" is specified, and there is none, make sure clear extension.
      m_wsFileName = *pws; 
      if (bHasExtension) SetFileExt(L"");                                           // CR.22523; added.
      return; 
   }

   // FileName has an extension.
   pws->substr(m_wsFileName, 0, stExtPos); 
   if (stExtPos + 1 == pws->wcslen() )
   {  
      // '.' char was the last char in the file name.  There
      // is no extension for this file name.
      SetFileExt(L"");                                                              // CR.22523; added.
   }
   else
   {
      CSpdCString wsExt; 
      pws->substr(wsExt, stExtPos + 1, pws->wcslen() - (stExtPos + 1)); 
      SetFileExt(wsExt); 
   }
}


/* SetFileExt() -
 *------------------------------------------------------------------------------------------------------*/
void CNtfsFolderName::SetFileExt(
   const CSpdCString &ws) 
{  
   m_wsFileExt = ws; 
}


/* SetWriteAccessDesired() -
 *------------------------------------------------------------------------------------------------------*/
void CNtfsFolderName::SetWriteAccessDesired(
   bool bWrite) 
{
   m_bWriteAccessDesired = bWrite; 
}


/* CdRoot() - Manipulates the current pathname by resetting it to the root folder.
 * 
 * SLB 23.jan.2023 CR.32082; Now throws an exception if this object's Foldername
 * is not initialized.
 * SLB 08.mar.2023 CR.32260; Removed requirement for all CdXxxx() methods (Change Directory) that the 
 * Foldername be initialized.
 *------------------------------------------------------------------------------------------------------*/
void CNtfsFolderName::CdRoot() 
{  
   // root folder is represented by an empty path string.
   m_wsPathName = L"\\"; 
}

/* CdBack() - Manipulates current pathname by removing the last folder from the 
 * path.
 *
 * SLB 23.jan.2023 CR.32082; Now throws an exception if this object's Foldername
 * is not initialized.
 * SLB 08.mar.2023 CR.32260; Removed requirement for all CdXxxx() methods (Change Directory) that the
 * Foldername be initialized. Instead ensure m_wsPathName is valid.
 *------------------------------------------------------------------------------------------------------*/
void CNtfsFolderName::CdBack() 
{
   IsPathNameSet_Check(__WFUNCTION__);                                              // CR.32260; modified. CR.32082; added.

   // If path is already at root, do nothing.
   if (m_wsPathName.wcslen() == 1) return;

   // erase the terminating '\'
   m_wsPathName.erase(m_wsPathName.wcslen()-1); 
      
   // find last '\' char in the path
   size_t stPos(0);
   stPos = m_wsPathName.find_last_of(L'\\');

   // no '\' found, then the path should represent the root folder now.
   if (stPos == CSpdCString::npos)
   {
      CdRoot();
      return;
   }

   // remove the last foldername, leave the preceding '\' which becomes the 
   // ending '\'.
   m_wsPathName.erase(stPos + 1); 
}

/* CdForward() - Manipulates the current pathname by adding the specified subfolder 
 * to the path. 
 * Specified folder must not contain path delimeters.
 *
 * SLB 23.jan.2023 CR.32082; Now throws an exception if this object's Foldername
 * is not initialized.
 * SLB 08.mar.2023 CR.32260; Removed requirement for all CdXxxx() methods (Change Directory) that the
 * Foldername be initialized. Instead ensure m_wsPathName is valid.
 * Now throws excpetion if specified wsFolderName does not meet the requirements.
 *------------------------------------------------------------------------------------------------------*/
void CNtfsFolderName::CdForward(
   const CSpdCString &wsFolderName) 
{
   IsPathNameSet_Check(__WFUNCTION__);                                              // CR.32260; modified. CR.32082; added.

   // wsFolderName is not permitted to have any path delimeters in it. 
   if (wsFolderName.find(L'\\') != CSpdCString::npos || wsFolderName.find(L':') != CSpdCString::npos)
      ThrowHr(E_INVALIDARG, __WFUNCTION__, L"Folder specified has a path delimeter in it. Folder <%s>", wsFolderName.c_str()); // CR.32260; added.
   
   // Add folder along with ending whack to m_wsPathName.
   m_wsPathName += wsFolderName; 
   m_wsPathName += CSpdCString(L"\\"); 
}

/* SetFullFileName() -
 *
 * SLB 22.mar.2005 - implemented.
 * SLB 21.apr.2008 - CR.9893; call to SetFileName() was not passing along
 * setting for bFixFileName. Now it is.
 * SLB 04.jan.2012 CR.15602; Added check for local drive, root folder case. Now ensures that the  "\" char 
 * is passed to the SetFullFolderName() method.
 *------------------------------------------------------------------------------------------------------*/
void CNtfsFolderName::SetFullFileName(
   const CSpdCString &wsFullFolderName, 
   bool bFixFileName) 
{  
   // Reset current values. Including file name.
   ClearFullFolder(true);                                                           // CR.29196; added.

   size_t stFilePos(0);   

   stFilePos = wsFullFolderName.find_last_of(L'\\'); 
   if (stFilePos == CSpdCString::npos)
   {  
      // No '\\' char found.  No folder information included, 
      // entire string is the file name
      SetFileName(wsFullFolderName, true, bFixFileName); 
      return;
   }
   
   // check for local drive, root folder, case.
   CSpdCString wsTemp;
   if (stFilePos == 2)                                                              // CR.15602; added.
   {  
      // SetFullFolderName needs a min of 3 chars. Include the trailing "\" for
      // this case (probably a file in the root folder)
      wsFullFolderName.substr(wsTemp, 0, stFilePos+1); 
   }
   else
   {  
      wsFullFolderName.substr(wsTemp, 0, stFilePos); 
   }
   SetFullFolderName(wsTemp); 

   if (stFilePos + 1 == wsFullFolderName.wcslen())
   {  
      // '\\' char is the last char of the folder name.  There
      // is no file name.
   }
   else
   {
      wsFullFolderName.substr(wsTemp, stFilePos+1, wsFullFolderName.wcslen() - (stFilePos + 1)); 
      SetFileName(wsTemp, true, bFixFileName);                                      // CR.9893
   }
}


/* SetFullFolderName() -
 * 
 * SLB 04.jan.2012 CR.15602; Fixes to handle both UNC and Local folder naming conventions better.
 * SLB 16.jul.2013 CR.17124.Item20; Modified to make sure that consecutive back slash characters are not 
 * stored in the path name.
 * SLB 01.apr.2020 CR.29196; Fix for not clearing existing values if passed in folder name is invalid.
 * SLB 02.apr.2020 CR.29220; Fix for allowing path to be left as an empty string when the root path 
 * is specified (because this condition was treated as an error). Also, Moved path scrubbing code into SetPathName().
 * SLB 18.jan.2023 CR.32082; Now sets folder initialized = true after setting the
 * full path up.
 *------------------------------------------------------------------------------------------------------*/
void CNtfsFolderName::SetFullFolderName(
   const CSpdCString &wsFullFolderName) 
{  
   const CSpdCString wsDot(L"."); 

   // Reset current values.
   ClearFullFolder();                                                               // CR.29196; added.

   // Make sure new folder is long enough to be a valid folder.
   if (wsFullFolderName.wcslen() < 3) return; 

   // first determine if the foldername is in mapped or UNC format.
   // Then initialize Local or UNC info. In either case, stStartOfPathPos should
   // be set up for initialization of path logic.
   size_t stStartOfPathPos(CSpdCString::npos);                                      // CR.15602; added.
   size_t stColonOffset(wsFullFolderName.find(L':')); 

   if (stColonOffset == CSpdCString::npos)
   {  
      // No colon char found. Treat Specified folder name as UNC format
      m_bIsUncFormat = true;

      CSpdCString wsSubStr;
      size_t stStartPos(0); 
      size_t stEndPos(0); 
      size_t stLen(CSpdCString::npos); 

      // Retrieve Server Name
      if (wsFullFolderName[0] != L'\\' || wsFullFolderName[1] != L'\\')
      {  
         // Problem: no server name specified, presume it is local_host
         SetUncHostName(wsDot);  
      }
      else 
      {  
         // determine if extra long format was specified.
         stStartPos = wsFullFolderName.find(L"\\\\?\\UNC\\"); 
         if (stStartPos != CSpdCString::npos)
         {  
            // extra long format was specified.  Skip past the extralong header.
            stStartPos += 8; 
         }
         else
         {  
            // Extra long format was not specified. skip past the double slash.
            stStartPos = 2; 
         }

         // Look for next slash.  It signals  the end of the host name.
         stEndPos = wsFullFolderName.find(L'\\', stStartPos); 
         if (stEndPos == CSpdCString::npos)
         {
            stLen = stEndPos; 
         }
         else
         {
            stLen = stEndPos - stStartPos; 
         }
         wsFullFolderName.substr(wsSubStr, stStartPos, stLen); 
         SetUncHostName(wsSubStr); 

         // Setup start pos for looking for the share name.
         // skip the slash that terminated the host name.
         stStartPos = stEndPos + 1; 
      }

      // Retrieve the share name.
      if (stEndPos == CSpdCString::npos ||
         stStartPos >= wsFullFolderName.wcslen())
      {  
         // problem. no more data to scan. Can't look for UNC share name.
         // This is an incomplete path specification.
      }
      else
      {  
         // Good. there is more data to scan. 
         stEndPos = wsFullFolderName.find(L'\\', stStartPos); 
         if (stEndPos == CSpdCString::npos)
            stLen = stEndPos; 
         else
            stLen = stEndPos - stStartPos; 
         wsFullFolderName.substr(wsSubStr, stStartPos, stLen); 
         SetUncShareName(wsSubStr);                                                 // CR.15602; modified.

         // Make sure start position is setup for determining path name.
         if (stEndPos != CSpdCString::npos)                                         // CR.15602; modified.
         {  
            // End of string was not encountered...
            // Set start of path position.
            stStartOfPathPos = stEndPos + 1; 
         }
      }
   }
   else                                                                             // CR.15602; added.
   {  
      // Specified folder name is in mapped drive letter format.
      CSpdCString wsDrive; 
      size_t stStartPos(0); 

      m_bIsUncFormat = false; 

      // determine if file is using "Long" format.
      stStartPos = wsFullFolderName.find(L'?'); 
      if (stStartPos == CSpdCString::npos)
      {  
         // Not using "Long" format.
         // e.g. "C:\temp"
         wsFullFolderName.substr(wsDrive, 0, 2); 
         stStartOfPathPos = 3; 
      }
      else
      {  
         // Using "Long" format.
         // e.g. "\\?\C:\temp"
         wsFullFolderName.substr(wsDrive, 4, 2); 
         stStartOfPathPos = 7; 
      }
      SetDriveLetter(wsDrive); 
   }

   if (stStartOfPathPos > wsFullFolderName.wcslen()) stStartOfPathPos = CSpdCString::npos; // CR.29220; added.

   // retrieve the path name.
   // CR.15602; moved this code outside of previous if() else() so that it may be used for
   // CR.29220; Removed code checking for validity of stStartOfPathPos
   // both UNC and Local folder naming conventions.
   CSpdCString wsPathSubStr; 
   wsFullFolderName.substr(wsPathSubStr, stStartOfPathPos); 
  
   SetPathName(wsPathSubStr); 
   m_bIsFolderInitialized = true;                                                   // CR.32082; added.
}

/* ChangeDirectory() -
 * 
 * SLB 26999.Item73; Now returns void. Modified to support and empty NewDir. Just does nothing in that case.
 * SLB 23.jan.2023 CR.32082; Now throws an exception if this object's Foldername
 * is not initialized.
 * SLB 08.mar.2023 CR.32260; Don't verify folder has been initialized in this method.
 * Users of this class can use it to manipulate path strings if they like.
 *------------------------------------------------------------------------------------------------------*/
void CNtfsFolderName::ChangeDirectory(                                              // CR.26999.Item73; modified.
   const CSpdCString &wsNewDir) 
{  
   //IsFolderNameInitialized_Check(__WFUNCTION__);                                    // CR.32082; added.
   if (wsNewDir.empty()) return;                                                    // CR.26999.Item73; added.

   size_t stStartPos(0); 

   if (wsNewDir[stStartPos] == L'\\')
   {
      CdRoot(); 
      stStartPos++; 
   }

   bool bNotDone(true);
   size_t stEndPos(CSpdCString::npos);
   size_t stLen(0);
   while (bNotDone && stStartPos != wsNewDir.wcslen() )
   {
      stEndPos = wsNewDir.find( L'\\', stStartPos); 

      // calculate the length of this substr.
      if (stEndPos==CSpdCString::npos)
      {  
         // no '\\' char found.
         stLen = wsNewDir.wcslen() - stStartPos; 
         bNotDone = false; 
      }
      else
      {
         stLen = stEndPos - stStartPos; 
      }
      CSpdCString wsSubfolder; 
      wsNewDir.substr(wsSubfolder, stStartPos, stLen); 
      stStartPos += stLen + 1; 

      // folder token has been obtained.  Take action accordingly.
      // if folder string is empty, then this is either illegal or the end of the string has been reached.
      if (wsSubfolder.empty()) return;

      // Change directory as required by this sub-folder name.
      if (wsSubfolder == L".") continue;
      if (wsSubfolder == CSpdCString(L"..")) CdBack();
      else CdForward(wsSubfolder); 
   }
}


/* IsFullFileNameOk() -
 * SLB 25.mar.2006 Added.
 * SLB 28.apr.2017 CR.22874; fixed to use 255 vs 256 per NTFS documentation.
 *------------------------------------------------------------------------------------------------------*/
CNtfsFolderName::TFolderNameProblem CNtfsFolderName::IsFullFileNameOk() const 
{
   TFolderNameProblem fnpRetVal(fnpOk); 
   
   if (m_wsFileExt.wcslen() + m_wsFileName.wcslen() + 1 > 255)                    // CR.22874; modified.
   {  
      //
      // NTFS does not support file names > 256 chars
      //
      fnpRetVal = fnpTooLong; 
   }

   return fnpRetVal; 
}

/* DoesFolderExist() -
 * SLB 22.jun.2005 Fixed up access rights for writing\reading.  Added
 * DELETE to file share rights.
 * SLB 12.jan.2007 CR.7666; Fixed DELETE access rights.  Was using the FILE_DELETE_CHILD access flag,
 * this was changed to the DELETE flag.  The FILE_DELETE_CHILD access right is only assigned
 * to the owner of the NTFS object or Administrator (typically).
 * SLB 19.jan.2007 CR.7666; Further refinements to DELETE access rights fix.  Turns out that if
 * attempt to access folder fails with DELETE, the access should be attempted again with 
 * FILE_DELETE_CHILD. Note: tried setting both flags together and this does not work.
 * SLB 31.aug.2007 CR.6135; Changed mechanism for testing access rights.  Now calls IsFolderAccessOk(). Logic
 * for checking for access rights is in that method.
 * SLB 16.apr.2008 CR.9374.V02; Modified to check so it can determine if the folder exists... Then checks if folder is accessible
 * with desired permissions. If folder exists but access is not granted, then ensures that ACCESS_DENIED is returned.
 * SLB 25.jan.2019 CR.26999.Item55; Fixed so it validates that the folder name is truely a folder (it would succeed before
 * if it was actually a filename). 
 * Added parm to specify if access check needs to be performed. Default is true, because that was previous 
 * functionality.
 * SLB 19.jun.2020 CR.29471; Added debug messages to troubleshoot MAYO issue. These should be removed
 * once issue is resolved.
 * SLB 23.jan.2023 CR.32082; Now throws an exception if this object's Foldername
 * is not initialized.
 *------------------------------------------------------------------------------------------------------*/
bool CNtfsFolderName::DoesFolderExist(
   DWORD &dwNtErr,
   bool bPerformAccessCheck) const                                                  // CR.26999.Item55; added.
{
   IsFolderNameInitialized_Check(__WFUNCTION__);                                    // CR.32082; added.

   // Retrieve File Attributes of folder.
   DWORD dwAttribs;                                                                 // CR.26999.Item55; added.
   bool bRes;                                                                       // CR.26999.Item55; added.
   bRes = Ntfs_GetFileAttributes(dwAttribs, true);                                  // CR.26999.Item55; added.
   dwNtErr = GetLastError();
   if (!bRes)
   {
      CEnterExitMsg::TraceMsg(L"%s() CR.29471; fails because Ntfs_GetFileAttributes() failed. ErrCode <%u>  Folder <%s>", __WFUNCTION__, dwNtErr, GetFullFolderName(false, false, true).c_str()); // CR.29471; added.
      return false;
   }

   // Check if Attributes include the DIRECTORY attribute.
   dwNtErr = 0;
   if ((dwAttribs & FILE_ATTRIBUTE_DIRECTORY) == 0) 
   { 
      CEnterExitMsg::TraceMsg(L"%s() CR.29471; fails because no FILE_ATTRIBUTE_DIRECTORY flag was set for the foldername. Folder <%s>", __WFUNCTION__, GetFullFolderName(false, false, true).c_str()); // CR.29471; added.
      dwNtErr = ERROR_DIRECTORY; return false;
   } // CR.26999.Item55; added.
   if (!bPerformAccessCheck) return true;

   // Perform access checks.
   dwNtErr = ERROR_SUCCESS; 
   bRes = IsFolderAccessOk(FILE_LIST_DIRECTORY, dwNtErr);  // validates the folder is accessible with READ permissions.
   if (!bRes) return false;

   if (!m_bWriteAccessDesired) return true;

   // Write Access is desired. Check for it.
   // SLB 15.jan.2007 CR.7666; removed FILE_DELETE_CHILD, added DELETE and FILE_WRITE_ATTRIBUTES
   DWORD dwAccessRights(FILE_LIST_DIRECTORY | FILE_READ_ATTRIBUTES | FILE_READ_DATA);
   dwAccessRights |= FILE_ADD_SUBDIRECTORY | FILE_ADD_FILE | DELETE | FILE_WRITE_DATA | FILE_WRITE_ATTRIBUTES;
   bRes = IsFolderAccessOk(dwAccessRights, dwNtErr);
   if (bRes) return true;

   // denied. convert err code to access denied if it is set to Not found.
   if (dwNtErr == ERROR_PATH_NOT_FOUND || dwNtErr == ERROR_FILE_NOT_FOUND)
      dwNtErr = ERROR_ACCESS_DENIED; 

   return false; 
}

/* FolderExists_Check() -
 *
 * SLB 11.apr.2018 Added.
 * SLB 04.jan.2019 CR.26999.Item3; added optional folder description and use that in 
 * hr_exception's error message.
 * SLB 14.jan.2019 CR.26999.Item55; Added ability to specify func name. Modified to use ThrowHr().
 * SLB 25.jan.2019 CR.26999.Item55; Does not perform access check on folder.
 * SLB 22.dec.2022 CR.32082; renamed from FolderExistsCheck() to FolderExists_Check()
 * Now throws an exception if this object's Foldername is not initialized.
*------------------------------------------------------------------------------------------------------*/
void CNtfsFolderName::FolderExists_Check(                                           // CR.32082; modified.
   const CSpdCString &wsFunc,                                                       // CR.26999.Item55; added.
   const CSpdCString &wsFolderDescription) const                                    // CR.26999.Item3; added.
{
   IsFolderNameInitialized_Check(__WFUNCTION__);                                    // CR.32082; added.

   DWORD dwNtErr;
   bool bRes;
   bRes = DoesFolderExist(dwNtErr, false);                                          // CR.26999.Item55; modified.
   if (bRes) return;

   // Folder does not exist. Format and throw exception.
   CSpdCString wsDescription(wsFolderDescription);                                  // CR.26999.Item3; added.
   if (wsDescription.IsEmpty()) wsDescription = L"Folder";                          // CR.26999.Item3; added.
   ThrowHr(HRESULT_FROM_WIN32(dwNtErr), wsFunc.c_str(), L"%s '%s' does not exist.", wsDescription.c_str(), GetFullFolderName(false).c_str());
}

/* FolderNoExist_Check() -
 *
 * SLB 14.jan.2019 CR.26999.Item55; Created.
 * SLB 25.jan.2019 CR.26999.Item55; Does not perform access check on folder.
 * SLB 23.jan.2023 CR.32082; Now throws an exception if this object's Foldername
 * is not initialized.
 *------------------------------------------------------------------------------------------------------*/
void CNtfsFolderName::FolderNoExist_Check(
   const CSpdCString &wsFunc,   
   const CSpdCString &wsFolderDescription) const 
{
   IsFolderNameInitialized_Check(__WFUNCTION__);                                    // CR.32082; added.

   DWORD dwNtErr;
   bool bRes;
   bRes = DoesFolderExist(dwNtErr, false);                                          // CR.26999.Item55; modified.
   if (!bRes) return;

   // Folder exists. Format and throw exception.
   CSpdCString wsDescription(wsFolderDescription);  
   if (wsDescription.IsEmpty()) wsDescription = L"Folder";
   ThrowHr(HRESULT_FROM_WIN32(dwNtErr), wsFunc.c_str(), L"%s '%s' exists already.", wsDescription.c_str(), GetFullFolderName(false).c_str());
}

/* GetFolderUniqueId() - This method returns a unique Id for the current folder.
 * Uses Win32 calls to get unique parameters.
 *
 * SLB 15.sep.2010 CR.13616; created.
 * SLB 15.sep.2010 CR.13616.V01; modified to output Win32 error code on error.
 * SLB 09.jun.2011 QBE.test; Modified Unique Id's construction so that Volume Id is first, then 
 * File Index info. This makes reading logs easier.
 * SLB 26.jul.2017 NoCR; re-structured method to be up-to-date. Uses exception, internally, for errors.
 * SLB 02.apr.2020 CR.29220; Modified so CSmartHandle is used for auto cleanup of CreateFile handle.
 * SLB 23.jan.2023 CR.32082; Now throws an exception if this object's Foldername
 * is not initialized.
 *------------------------------------------------------------------------------------------------------*/
bool CNtfsFolderName::GetFolderUniqueId(
   std::wstring &wsUniqueId, 
   DWORD &dwNtErr) const                                                            // CR.13616.V01; added.
{
   IsFolderNameInitialized_Check(__WFUNCTION__);                                    // CR.32082; added.

   wsUniqueId.clear();                                                              // CR.13616.V01;

   CSpdCString wsFolderName; 
   GetFullFolderName(wsFolderName, false, false, true); 

   try
   {
      DWORD dwAccessRights(FILE_LIST_DIRECTORY|FILE_READ_ATTRIBUTES|FILE_READ_DATA); 
      CSmartHandle shFolder;                                                        // CR.29220; modified.
      shFolder = CreateFileW(
         wsFolderName.c_str(),
         dwAccessRights,
         FILE_SHARE_DELETE|FILE_SHARE_READ|FILE_SHARE_WRITE,
         NULL,
         OPEN_EXISTING,
         FILE_FLAG_BACKUP_SEMANTICS,   // needed for opening a folder.
         NULL); 
      dwNtErr = GetLastError(); 
      if (shFolder == INVALID_HANDLE_VALUE) ThrowHr(HRESULT_FROM_NT(dwNtErr), __WFUNCTION__, L"CreateFile() failed opening folder."); 

      // successfully opened folder. Now retrieve file info.
      BOOL bRES; 
      BY_HANDLE_FILE_INFORMATION bhfiFolder; 
      bRES = GetFileInformationByHandle(shFolder, &bhfiFolder); 
      dwNtErr = GetLastError(); 
      if (bRES == FALSE) ThrowHr(HRESULT_FROM_NT(dwNtErr), __WFUNCTION__, L"GetFileInformationByHandle() failed."); 

      // file info retrieved successfully.
      // build the folder id string using the folder's Volume Serial Numbers and File Index.
      wchar_t wcsId[64]; 
      _snwprintf_s(wcsId, _TRUNCATE, L"%08X.%08X.%08X",
         bhfiFolder.dwVolumeSerialNumber,          // QBE.test; modified.
         bhfiFolder.nFileIndexHigh,
         bhfiFolder.nFileIndexLow); 

      // set method's output values.
      wsUniqueId = wcsId; 
      dwNtErr = ERROR_SUCCESS; 
      return true; 
   }
   catch (hr_exception &e)
   {
      CEnterExitMsg::TraceMsg(L"(%08X) GetFolderUniqueId() error. %s. Foldername: %s",
         e.GetHr(),
         e.GetMessage().c_str(),
         wsFolderName.c_str()); 
   }
   return false; 
}

/* FileExists_Check() - 
 *
 * SLB 11.apr.2018 Added.
 * SLB 04.jan.2019 CR.26999.Item3; Added optional folder description and use that in 
 * hr_exception's error message.
 * SLB 14.jan.2019 CR.26999.Item55; Modified to Use ThrowHr().
 * SLB 23.jan.2023 CR.32082; Now throws an exception if this object's Foldername or
 * Filename is not initialized.
 * Modified message so that "file" is automatically in it. This means that the
 * wsFileDescription string should not have the word "file" in it.
 *------------------------------------------------------------------------------------------------------*/
void CNtfsFolderName::FileExists_Check(
   const CSpdCString &wsFunc,                                                       // CR.26999.Item55; added.
   bool bNoAccessCheck,
   const CSpdCString &wsFileDescription) const                                      // CR.16999.Item3; added.
{
   IsFileNameInitialized_Check(__WFUNCTION__);                                      // CR.32082; added.
   IsFolderNameInitialized_Check(__WFUNCTION__);                                    // CR.32082; added.

   DWORD dwNtErr;
   bool bRes;
   bRes = DoesFileExist(dwNtErr, bNoAccessCheck);
   if (bRes) return;

   // build exception and throw it.
   CSpdCString wsDescription(wsFileDescription);                                    // CR.26999.Item3; added.
   //if (wsDescription.IsEmpty()) wsDescription = L"File";                          // CR.32082; reomoved. CR.26999.Item3; added.
   ThrowHr(HRESULT_FROM_WIN32(dwNtErr), wsFunc.c_str(), L"%s file '%s' does not exist.", wsDescription.c_str(), GetFullFileName().c_str()); // CR.32082 modified.
}

/* FileNoExists_Check() -
 *
 * SLB 14.jan.2019 CR.26999.Item55; Created.
 * SLB 22.feb.2021 CR.29859.V01; modified to throw ERROR_ALREADY_EXISTS (HR form) vs E_FAIL  when
 * the file exists.
 * SLB 23.jan.2023 CR.32082; Now throws an exception if this object's Foldername or
 * Filename is not initialized.
 * Modified message so that "file" is automatically in it. This means that the
 * wsFileDescription string should not have the word "file" in it.
 *------------------------------------------------------------------------------------------------------*/
void CNtfsFolderName::FileNoExists_Check(
   const CSpdCString &wsFunc,  
   const CSpdCString & wsFileDescription) const
 {
   IsFileNameInitialized_Check(__WFUNCTION__);                                      // CR.32082; added.
   IsFolderNameInitialized_Check(__WFUNCTION__);                                    // CR.32082; added.

   DWORD dwNtErr;
   bool bRes;
   bRes = DoesFileExist(dwNtErr, true);
   if (!bRes) return;

   // build exception and throw it.
   CSpdCString wsDescription(wsFileDescription); 
   //if (wsDescription.IsEmpty()) wsDescription = L"File";                          // CR.32082; removed.
   ThrowHr(HRESULT_FROM_WIN32(ERROR_ALREADY_EXISTS), wsFunc.c_str(), L"%s file '%s' already exists.", wsDescription.c_str(), GetFullFileName().c_str()); // CR.32082; modified.
}

/* DoesFileExistSpecifyAccess() -
 *
 * SLB 22.jun.2005 Fixed up access rights for writing\reading.  
 * SLB 17.jun.2006 Modified to request local drive mapping, if available, when file
 * name is constructed.  This is done for performance on Win XP, SP2 systems.
 * SLB 16.jul.2009 CR.11810; Renamed this method. This method now gets its "Write Access" check via the call
 * params vs. the member variable.
 * SLB 02.apr.2020 CR.29220; Modified so CSmartHandle is used for auto cleanup of CreateFile handle.
 * SLB 17.mar.2021 CR.29449.V01; The point of this method is determine if the file exists and the caller 
 * has access permissions to the file... Sharing violation issues are not covered by this umbrella. So,
 * fixed this method so it will avoid sharing violations as much as possible. 
 * SLB 23.jan.2023 CR.32082; Now throws an exception if this object's Foldername or
 * Filename is not initialized.
 *------------------------------------------------------------------------------------------------------*/
bool CNtfsFolderName::DoesFileExistSpecifyAccess(
   bool bCheckForReadAccess, 
   bool bCheckForWriteAccess, 
   DWORD &dwNtErr) const
{
   IsFileNameInitialized_Check(__WFUNCTION__);                                      // CR.32082; added.
   IsFolderNameInitialized_Check(__WFUNCTION__);                                    // CR.32082; added.

   dwNtErr = ERROR_SUCCESS;

   DWORD dwAccessRights(0);                                                         // CR.11810;
   if (bCheckForReadAccess)                                                         // CR.11810;
   {  
      dwAccessRights |= GENERIC_READ; 
   }
   if (bCheckForWriteAccess)                                                        // CR.11810;
   {
      dwAccessRights |= GENERIC_WRITE; 
   }

   // Use long file name format ("\\?\UNC\.....")
   CSmartHandle shFile;                                                             // CR.29220; modified.
   CSpdCString wsFileName; 
   GetFullFileName(wsFileName, true, false, true, true); 
   shFile = CreateFileW( 
      wsFileName.c_str(),
      dwAccessRights, 
      FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,                       // CR.29449.V01; modified.
      NULL,
      OPEN_EXISTING,
      0, 
      NULL); 
   dwNtErr = GetLastError(); 
   if (shFile == INVALID_HANDLE_VALUE) return false;

   // opened the file successfully!
   return true;
}


/* IsFolderAccessOk() -
 *
 * SLB 31.aug.2007 CR.6135; Modified to use the folder's security descriptor to 
 * check the access rights. This mechanism takes into account the folder's owner
 * and inherited settings which CreateFile() seems to ignore.
 * SLB 14.apr.2008 CR.9374; Make sure that dwNtErr is initialized for all error
 * situations.
 * SLB 21.jan.2008 CR.9374.V01; Modified code to handle the case where the Token buffer could be larger
 * than expected. Basically, call the method once and get the size needed. Then allocate an appropriate
 * buffer and call it again. Also, added debug message output for most failure points within this method to
 * help track this down.
 * SLB 12.aug.2008 Call.414535; Added an additional debug message when IsFolderAccessOk() fails because of insufficient permissions. 
 * SLB 03.mar.2009 CR.11156; Restructured so that security context is static now. No longer need
 * to retrieve the token of the process in order to find its SID. The token retrieval code has been moved into
 * the Win32Sec2::SIDPtr::GetProcessSID() method.
 * SLB 15.aug.2010 CR.13337; In some environments, older NTFS file systems are hosted by NT 4.0 still. These 
 * systems do not support GetNamedSecurityInfo() method. Modified so that if GetNamedSecurityInfo() fails with 
 * a specific error code (ERROR_BAD_NET_RESP), then an attempt is made to access the folder via CreateFile(). 
 * Also, changed usage of _snwprintf() to _snwprintf_s().
 * SLB 03.dec.2010 CR.13337.V01; Further error codes were being returned by GetNamedSecurityInfo() call on 
 * NT 4.0 system (NTFS 1.5), modified secondary call to occur regardless of the error type (so why not remove
 * GetNamedSecurityInfo() all together. right?)
 * SLB 30.may.2013 CR.16991; Added code to make sure access to the security descriptor is protected. This fixes
 * problem where Access Violation was apparently happening in the TCPIP input module when creating a CSpdPath
 * sub-stream. It may resolve other random issues that were never tracked down.
 * SLB 16.jul.2013 CR.17124.Item10; No changes to this routine.. However, I discovered that the 
 * GetNamedSecurityInfo() method will return ERR_INVALID_PATH if the path constructed has consecutive 
 * back slashes. Windows command line routines do not have a problem with this, but apparently more internal
 * methods, like GetNamedSecurityInfo(), do. This means that other class methods need to take steps to ensure 
 * that the path name does not have consecutive back slash characters.
 * SLB 19.jun.2020 CR.29471; debug msgs added when access is not granted. 
 * SLB 23.jan.2023 CR.32082; Now throws an exception if this object's Foldername
 * is not initialized.
 *------------------------------------------------------------------------------------------------------*/
bool CNtfsFolderName::IsFolderAccessOk(
   DWORD dwAccessRights, 
   DWORD &dwNtErr) const 
{
   IsFolderNameInitialized_Check(__WFUNCTION__);                                    // CR.32082; added.

   dwNtErr = 0; 
   CSpdCString wsFolderName(GetFullFolderName(false, false, true));

   // CR.6135; Retrieve folder's Security Descriptor.
   // Note: get everything except for the SACL. The SACL is only used to determine auditing settings.
   PSECURITY_DESCRIPTOR psdFolder(NULL); 
   dwNtErr = GetNamedSecurityInfo(
      wsFolderName.c_str(),
      SE_FILE_OBJECT,
      OWNER_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION, 
      NULL,
      NULL,
      NULL,
      NULL,
      &psdFolder); 
   if (dwNtErr != ERROR_SUCCESS)                                                    // CR.13337.V01; modified
   {  
      CEnterExitMsg::TraceMsg(L"%s() CR.29471; GetNamedSecurityInfo() failed. NtErr(%u) Folder <%s>", __WFUNCTION__, dwNtErr, wsFolderName.c_str());
      // problem: GetNamedSecurityInfo() failed with ERROR_BAD_NET_RESP.
      // CR.13337; Folks with old NT 4.0 files NTFS systems will get this error. Use 
      // a more primitive way to determine if access is available. 
      // CR.13337.V01; modified to drop into this section on error, regardless of 
      // error code.
      // CR.13524; Reset the NT error code as it is a return value.

      // Open the folder.
      CSmartHandle shFolder;
      shFolder = CreateFileW(
         wsFolderName.c_str(),
         dwAccessRights,
         FILE_SHARE_DELETE|FILE_SHARE_READ|FILE_SHARE_WRITE,
         NULL,
         OPEN_EXISTING,
         FILE_FLAG_BACKUP_SEMANTICS,   // needed for opening a folder.
         NULL);
      dwNtErr = GetLastError(); 
      if (shFolder == INVALID_HANDLE_VALUE)
      {
         CEnterExitMsg::TraceMsg(L"%s() CR.29471; CreateFile(%08X) failed. NtErr(%u) Folder <%s>", __WFUNCTION__, dwAccessRights, dwNtErr, wsFolderName.c_str()); 
         return false;
      }

      // Success. 
      return true;
   }

   // CR.11156
   // SLB 16.feb.2022 CR.31266; Now throws exception if ownership is not obtained. Added because some 
   // odd debugmsgs were noticed, though no noticeable effect was seen in the product at the time. 
   // Note: This mutex was changed from a global named mutex at this time as well.
   DWORD dwGrantedAccessRights(0); 
   {                                                                                // CR.16991; added.
      // create scope for obtaining ownership of the Security Context.
      CSpdBlock block(s_hScMutex);                                                  // CR.16991; added.
      if (!block.Locked()) ThrowHr(E_FAIL, __WFUNCTION__, L"Error: Security context named mutex is no longer valid."); // CR.31266; added.

      dwNtErr = s_scSecurityContext.GetEffectivePermissions2(                       // CR.9374;
         psdFolder,                                                                 // CR.11156; 
         dwAccessRights,
         &dwGrantedAccessRights); 

      // Clean up memory allocation from GetNamedSecurityInfo().
      LocalFree(psdFolder); 
   }                                                                                // CR.16991; added.
   if (dwNtErr != ERROR_SUCCESS)                                                    // CR.9374
   {  
      // problem: GetEffectivePermissions2() failed.
      CEnterExitMsg::TraceMsg(L"%s() CR.29471; GetEffectivePermissions2(%08X) failed. NtErr(%u)  Folder <%s>", __WFUNCTION__, dwAccessRights, dwNtErr, wsFolderName.c_str());
      return false;
   }

   // Successfully retrieved permissions.
   // Check to see if retrieved permissions match desired permissions.
   if (dwGrantedAccessRights == dwAccessRights) return true;

   // problem: Granted access rights don't match up with requested access rights.
   // Call.414535; V0 changes.
   dwNtErr = ERROR_ACCESS_DENIED;                                                   // CR.9374
   CEnterExitMsg::TraceMsg(L"%s() CR.29471 - Access Denied. Desired Access(%08X)  Granted Access(%08X)  Folder <%s>", __WFUNCTION__, dwAccessRights, dwGrantedAccessRights, wsFolderName.c_str());

   // Call.414535; V1 changes. Attempts to output descriptions of flags so support can
   // better track down the issues.
   DWORD dwMissingFlags(dwAccessRights^dwGrantedAccessRights); 
   DWORD dwFlag(0); 
   for (DWORD i=0, dwFlag=1; i<32; i++, dwFlag<<1)
   {
      if (dwFlag & dwMissingFlags)
      {
         CEnterExitMsg::TraceMsg(L"%s() CR.29471 - Missing required access flag %u: %s", __WFUNCTION__, dwFlag, GetAccessFlagDescription(dwFlag).c_str()); 
      }
   }
   return false;
}

/* IsSystemFolder() - returns true if the folder is considered a system folder.
 *
 * First, For ALL folder types, checks if the SYSTEM or HIDDEN attributes flags are set. If so this is considered a system folder.
 * Second, if the Folder is in UNC format then the following rules are observed:
 *    1) If the folder is not on the local server, it is automatically NOT considered a system folder.
 *    2) If it is on the local server AND is NOT in the list of SpecialShares, then it is NOT considered a system folder.
 * Third, If still checking at this point, then the path itself is compared against the predefined list of "System" paths
 * managed by the IsSystemFolder(CSpdCString) over-load method.
 *
 * SLB 03.jun.2020 CR.29412; Modified to treat UNC paths as a special case. Added basic checks of File attributes.
 * SLB 05.jun.2020 CR.29412.V01; Fix for checking for file attributes when folder didn't exist.
 * SLB 23.jan.2023 CR.32082; Now throws an exception if this object's Foldername is 
 * not initialized.
 *------------------------------------------------------------------------------------------------------*/
bool CNtfsFolderName::IsSystemFolder() const 
{
   IsFolderNameInitialized_Check(__WFUNCTION__);                                    // CR.32082; added.

   // First make simple file attribute check. But only if the folder exists.
   DWORD dwNtErr(0);
   bool bRes(DoesFolderExist(dwNtErr, false));                                      // CR.29412.V01; added.
   if (bRes)                                                                        // CR.29412.V01; added if () around existing functionality.
   {
      DWORD dwFileAttribs(0);                                                       // CR.29412; added.
      Ntfs_GetFileAttributes(dwFileAttribs, true);                                  // CR.29412; added.
      if (dwFileAttribs & FILE_ATTRIBUTE_SYSTEM) return true;                       // CR.29412; added.
      if (dwFileAttribs & FILE_ATTRIBUTE_HIDDEN) return true;                       // CR.29412; added.
   }

   if (IsUncFormat())                                                               // CR.29412; added if () {}.
   {
      // If unc is not on this server, automatically not considered a system folder.
      if (GetUncHostName().compare(SpdUtils::GetComputerName(), false) != 0) return false;

      // UNC is from this server.
      // If share is not an admin share, then any folder under it is not considered a system folder (though that is not necessarily true) 
      // by this method.
      if (s_SpecialShares.find(GetUncShareName()) == s_SpecialShares.end()) return false;

      // This is an admin share. So path is subject to a SystemFolder() check.
   }

   bRes = IsSystemFolder(GetPathName()); 
   return bRes; 
}

/* IsSystemFolder() - helper method.
 * This method expects a scrubbed absolute folder name with a terminating '\' char at the end of it. No drive/unc info.
 *
 * SLB 15.mar.2006 - CR.6006 removed root folder because Clients need to be able to
 * validate Share Paths.
 * SLB 02.apr.2020 CR.29220; Re-added root folder as a system folder. Modified test to match the beginning of the 
 * foldername... So, anything that begins with \Windows\ will be considered a system folder.
 * Added additional test for default SPDE data folder.
 *------------------------------------------------------------------------------------------------------*/
bool CNtfsFolderName::IsSystemFolder(
   const CSpdCString &wsFolderName) const 
{
   // If the folder length is 1 char, then is presumed it is the "root" folder.
   if (wsFolderName.wcslen() == 1) return true;                                     // CR.29220; added.

   // Any folder that begins with and doesn't equal to these strings are considered non-system folder    
   static std::list<CSpdCString> s_TempFolders =                                    // CR.29957; added.
   {
      L"\\Windows\\Temp\\"                // so that a sub-folder of 'Windows\temp\' can be removed, which may be used as temp folder
   };

   for (std::list<CSpdCString>::const_iterator it(s_TempFolders.begin()); it != s_TempFolders.end(); it++)
   {
      // see if the beginning of the foldername matches one of the predefined temp folders.
      if(_wcsnicmp(wsFolderName.c_str(), it->c_str(), it->wcslen()) == 0 && wsFolderName.wcslen() != it->wcslen()) 
         return false;
   }

   // Any folders that begin with these strings are considerd system folders.
   static std::list<CSpdCString> s_SystemFolders =                                  // CR.29220; modified.
   {
      L"\\Windows\\",
      L"\\Program Files\\",
      L"\\ProgramData\\",
      L"\\Spd\\" // don't allow because this is where SLB has his source file sandboxes.
   }; 
   
   for (std::list<CSpdCString>::const_iterator it(s_SystemFolders.begin()); it != s_SystemFolders.end(); it++)
   {
      // see if the beginning of the foldername matches one of the predefined system folders.
      if (_wcsnicmp(wsFolderName.c_str(), it->c_str(), it->wcslen()) != 0) continue;// CR.29220; modified.

      // this is a match to system folder.
      return true;
   }

   // CR.29220; Added special folders check.
   // Any folders that match with these strings are considerd system folders.
   static std::list<CSpdCString> s_SpecialFolders =                                  // CR.29220; modified.
   {
      L"\\Spde\\",  // defualt spde data folder.
   };

   for (std::list<CSpdCString>::const_iterator it(s_SpecialFolders.begin()); it != s_SpecialFolders.end(); it++)
   {
      // see if the beginnig of the foldername matches one of the predefined system folders.
      if (wsFolderName.compare(*it, false) != 0) continue;

      // this is a match to system folder.
      return true;
   }

   return false;
}

/* IsSystemFile() -
 *
 * SLB 28.jun.2013 CR.17124.Item1; added.
 * SLB 23.jan.2023 CR.32082; Now throws an exception if this object's Foldername or 
 * Filename is not initialized.
 *------------------------------------------------------------------------------------------------------*/
bool CNtfsFolderName::IsSystemFile() const 
{
   IsFileNameInitialized_Check(__WFUNCTION__);                                      // CR.32082; added.
   IsFolderNameInitialized_Check(__WFUNCTION__);                                    // CR.32082; added.

   return IsSystemFile(GetFullFileName()); 
}


/* IsSystemFile() - Always returns false.
 *
 * SLB 28.jun.2013 CR.17124.Item1; added. Does nothing right now. Not sure what form the functionality should 
 * take. 
 *------------------------------------------------------------------------------------------------------*/
bool CNtfsFolderName::IsSystemFile(
   const CSpdCString &wsFileName) const
{
   bool bRetVal(false); 
   return bRetVal; 
}

   
/* clear() -
 *
 * SLB 01.apr.2017 CR.23305; Reset members related to Mapped Local Path.
 *------------------------------------------------------------------------------------------------------*/
void CNtfsFolderName::clear()
{
   ClearFullFolder(true);                                                           // CR.32082; modified.
}

/* ClearFullPath() - Clears all settings related to a full path.
 *
 * SLB 01.apr.2020 CR.29196; Created.
 * SLB 23.jan.2023 CR.32082; Added support for tracking folder/file name 
 * initialization.
 *------------------------------------------------------------------------------------------------------*/
void CNtfsFolderName::ClearFullFolder(
   bool bFileNameAlso)
{
   m_wsDriveLetter.clear();
   m_wsPathName.clear();
   m_wsUncHostName.clear();
   m_wsUncShareName.clear();
   m_bIsUncFormat = false;
   m_wsMappedLocalPathName.clear();
   m_bMappedLocalPathInitialized = false;
   m_bIsFolderInitialized = false;                                                  // CR.32082; added.

   if (!bFileNameAlso) return;

   // requested filename gets cleared.
   ClearFileName(true);
}

/* ClearFileName() - Clears all settings related to a filename
 *
 * SLB 01.apr.2020 CR.29196; Created.
 * SLB 23.jan.2023 CR.32082; Added support for tracking folder/file name
 * initialization.
 *------------------------------------------------------------------------------------------------------*/
void CNtfsFolderName::ClearFileName(
   bool bExtAlso)
{
   m_wsFileName.clear();
   if (!bExtAlso) return;

   // requested extension gets cleared.
   m_wsFileExt.clear();
   m_bIsFileInitialized = false;                                                    // CR.32082; added.
}

/* InitializeStatics() - Initializes the static collections this class depends upon (because these members have 
 * other, run time, dependencies that have to be initialized before they are setup)..
 *
 * SLB 03.jun.2020 CR.29412; Created.
 *------------------------------------------------------------------------------------------------------*/
void CNtfsFolderName::InitializeStatics()
{
   // Only do work once!
   if (s_bStaticsAreInitialized) return;

   s_bStaticsAreInitialized = true;

   // Do the work.
   BuildAccessFlagDescriptionMap();
   InitializeNetShareStatics();
}

/* InitializeNetShareStatics() - Initializes the s_SpecialShares collection with all admin shares that Windows reports.
 *
 * SLB 03.jun.2020 CR.29412; Created. 
 * SLB 22.mar.2021 NoCR; Added debug msgs for all admin shares found... This could be helpful when 
 * customers run into problems with our "system" folder detection.
 *------------------------------------------------------------------------------------------------------*/
void CNtfsFolderName::InitializeNetShareStatics()
{
   CEnterExitMsg eem(__WFUNCTION__, L"Identifying Admin shares. Using NetShare level 502."); // NoCR; added.

   // Do the work to search through Windows shares and determine which shares are "Special" System/Admin shares. 
   // Outer loop for each call to NetShareEnum().
   DWORD dwResume_Handle(0);                                // assigned by first call to NetShareEnum() and referenced by further calls to it.
   NET_API_STATUS res(ERROR_MORE_DATA);							// return value from NetShareEnum
   while (res == ERROR_MORE_DATA)
   {
      // Call the NetShareEnum function; specify level 502. 
      // it is possible (though unlikely) that it will take more than one call to NetShareEnum() to find the share.
      PSHARE_INFO_502 BufPtr(NULL);				               // ptr to allocated object returned from NetShareEnum
      DWORD dwEntriesRead(0), dwTotalEntries(0);
      res = NetShareEnum(NULL, 502, (LPBYTE *)&BufPtr, -1, &dwEntriesRead, &dwTotalEntries, &dwResume_Handle); 
      DWORD le(GetLastError());
      Win32Sec::CSmartNetApiBuf smartp(BufPtr);   
      if (res != ERROR_SUCCESS && res != ERROR_MORE_DATA)
      {
         // NetShareEnum() problem occurred.
         CEnterExitMsg::TraceMsg(L"%s() - Critical error. Could not enumerate and search for admin shares. Ntfs Error (%06u)", __WFUNCTION__, le);
         return;
      }

      // inner loop: go through entries returned by NetShareEnum().
      for (DWORD i = 0; i < dwEntriesRead; i++)
      {
         PSHARE_INFO_502 p(BufPtr + i);
         // only interested in DISK shares (not print Qs, Comm devices, etc.)
         if ((p->shi502_type & STYPE_MASK) != STYPE_DISKTREE) continue;

         // Must have the "special" flag to be an admin share.
         if ((p->shi502_type & STYPE_SPECIAL) == 0) continue;

         // Found an admin share. Add it to the static list.
         s_SpecialShares.emplace(p->shi502_netname);
         CEnterExitMsg::TraceMsg(L"Admin share: %s = %s", p->shi502_netname, p->shi502_path); // NoCR; added.
      }
   }
   
   return;
}

/* MapFolderToLocalUncName() - Uese NetApi lib to retrieve shares and map the local drive to an existing share.
 * Note: At some point we may want to convert this from NetApi to WNet (Windows Networking functions). The WNet
 * method is WNetGetUniversalName().
 *
 * SLB 14.aug.2019 CR.28242; Code refactored and commented.
 * SLB 04.may.2020 NoCR; Noted some issues with the implementation (passes empty string instead of 
 * NULL for server name... to the NetApi function). Modified to use the SmartPtr for the NetApi buffer.
 * SLB 20.may.2020 CR.29294; Changes to help with resource/memory leaks. Now uses a CSmartNetApiBuf to
 * ensure that the NetApi buffer gets freed.
 * SLB 03.jun.2020 CR.29412; Fix for accessing PSHARE_INFO_502 buffer outside of returned buffer size.
 * SLB 23.jan.2023 CR.32082; Now throws an exception if this object's Foldername is
 * not initialized.
 *------------------------------------------------------------------------------------------------------*/
bool CNtfsFolderName::MapFolderToLocalUncName(
   CSpdCString &ws, 
   DWORD &dwNtErr, 
   bool bAddEndingWhack) const
{
   IsFolderNameInitialized_Check(__WFUNCTION__);                                    // CR.32082; added.

   // clear outputs
   ws.clear();	
   dwNtErr = NO_ERROR;

   if (IsUncFormat())
   {
      // not a problem: The format is already UNC.  No mapping is required.
      // Just retrieve the full folder name and return it.
      GetFullFolderName(ws, bAddEndingWhack);
      return true;
   }

   // validation. 
   // SLB - Drive letter validation is performed else where (initialization of class).  IsUncFormat() returns 
   // true if this is unc formatted and drive letter string is empty if it is invalid.
   // SLB: If Drive letter is empty; should probably set dwNtErr to something. But not sure what.
   CSpdCString wsDriveLetter(GetDriveLetter()); 
   if (wsDriveLetter.empty()) return false;

   // prepare wsDriveLetter for comparing against NetShareEnum() return values. Only maps to default share.
   wsDriveLetter += L":\\";

   // Driver letter is valid. Do the work to search through Windows shares and determine what the default sharename is for this 
   // drive.
   // Outer loop for each call to NetShareEnum().
   DWORD dwResume_Handle(0);                                // assigned by first call to NetShareEnum() and referenced by further calls to it.
   NET_API_STATUS res(ERROR_MORE_DATA);							// return value from NetShareEnum
   while (res == ERROR_MORE_DATA)
   {
      // Call the NetShareEnum function; specify level 502. 
      // it is possible (though unlikely) that it will take more than one call to NetShareEnum() to find the share.
      PSHARE_INFO_502 BufPtr(NULL);				               // ptr to allocated object returned from NetShareEnum
      DWORD dwEntriesRead(0), dwTotalEntries(0); 
      res = NetShareEnum(NULL, 502, (LPBYTE *) &BufPtr, -1, &dwEntriesRead, &dwTotalEntries, &dwResume_Handle); // CR.29294; modified.
      Win32Sec::CSmartNetApiBuf smartp(BufPtr);                                     // CR.29294; added.
      if (res != ERROR_SUCCESS && res != ERROR_MORE_DATA)
      {
         // NetShareEnum() problem occurred.
         dwNtErr = GetLastError();
         return false;
      }

      // inner loop: go through entries returned by NetShareEnum().
      for (DWORD i = 0; i < dwEntriesRead; i++)                                     // CR.29412; modified.
      {
         PSHARE_INFO_502 p(BufPtr + i);

         // only interested in DISK shares (not print Qs, Comm devices, etc.)
         if ((p->shi502_type & STYPE_MASK) != STYPE_DISKTREE) continue;             // CR.29412; added.
         
         // only interested if the path contains the drive letter being mapped.
         if (wsDriveLetter.compare(p->shi502_path, false) != 0) continue;

         // found a drive letter match. Now build the output UNC path, using this entry's share name.
         // (format looks like: \\<machine name>\<share name>\<path>).
         // p->shi502_netname is the share name for the path 
         ws = L"\\\\"; 
         ws += GetComputerName();
         ws += L"\\";
         ws += p->shi502_netname;
         ws += m_wsPathName;
         if (!bAddEndingWhack && m_wsPathName.wcslen() > 1)
         {  
            // no ending '\\' desired and this is not the root folder...
            // erase the ending whack.
            ws.erase(ws.wcslen() - 1); 
         }
         return true;
      }

      // the drive letter was not found in the output from NetShareEnum(). Continue to another call to NetShareEnum().
   }

   // if we have reached this point; the drive letter was not found. is m_wsDriveLetter corrupt?
   // What to set dwNtErr too?
   return false;
}


/* ConvertToLocalPath() -
 *
 * SLB 02.apr.2014 CR.6522; added.
 * SLB 01.apr.2017 CR.23305; If needed, calls InitializeMappedShare() now.
 * SLB 23.jan.2023 CR.32082; Now throws an exception if this object's Foldername is
 * not initialized.
 *------------------------------------------------------------------------------------------------------*/
bool CNtfsFolderName::ConvertToLocalPath(
   DWORD &dwNtErr) 
{
   IsFolderNameInitialized_Check(__WFUNCTION__);                                    // CR.32082; added.

   // The path is not in UNC format, return success.
   if (GetUncShareName().empty()) return true; 

   // Ensure that Mapped Local path is initialized.
   if (!m_bMappedLocalPathInitialized) InitializeMappedShare();                     // CR.23305; added.

   // convert this foldername to its local form (if possible).
   CSpdCString wsLocalPathName;  
   if (!UncNameToLocalMapFolder(wsLocalPathName, dwNtErr, true)) return false; 

   // if successful, re-set the current foldername, but with the "local" form of the foldername.
   SetFullFolderName(wsLocalPathName); 

   return true; 
}


/* InitializeMappedShare() - 
 *
 * SLB 31.mar.2017 CR.23305; added.
 * SLB 04.mar.2020 CR.29058; Modified to use SpdUtils version of GetComputerName(). Now fully qualified 
 * machine names will be used in UNC paths.
 * SLB 06.may.2020 CR.29294; Modified to use smart NetApi class to manage destruction of the NetApi buffer.
 * Fixed infinite loop issue by structuring loop exit logic similar to that of MapFolderToLocalUncName().
 * Fix for dwResume_Handle getting reset if more than one pass required with NetShareEnum().
 * SLB 03.jun.2020 CR.29412; Fix for accessing PSHARE_INFO_2 buffer outside of returned buffer size.
 * Updated NetShareEnum() loop code to mirror that in other methods.
 *------------------------------------------------------------------------------------------------------*/
void CNtfsFolderName::InitializeMappedShare()
{
   m_bMappedLocalPathInitialized = false;

   // Can't do anything if there is no UNC share..
   if (GetUncShareName().empty()) return; 

   // Make sure the share is for this computer.
   // CR.29058 comment: This logic would not work if the m_wsUncHostName is not a fully qualified
   // name. 
   const CSpdCString &wsComputerName(GetComputerName());                            // CR.29058; modified.
   if (wsComputerName.empty()) return;                                              // CR.29058; modified.
   if (m_wsUncHostName.compare(wsComputerName, false) != 0) return;                 // CR.29058; modified.

   // Good so far. We have a UNC share name that is associated with this computer.
   // Call the NetShareEnum function; specify level 2. 
   // Loops in case NetShareEnum() returns ERROR_MORE_DATA.
   CSpdCString wsUncShareName(GetUncShareName());
   // CR.29294; Moved these variable declarations outside of loop context.
   DWORD dwResume_Handle(0);                                // assigned by first call to NetShareEnum() and referenced by further calls to it.
   NET_API_STATUS res(ERROR_MORE_DATA);							// return value from NetShareEnum
   while (res == ERROR_MORE_DATA)
   {
      DWORD dwEntriesRead(0), dwTotalEntries(0);                                    // CR.29294; modified.
      PSHARE_INFO_2 BufPtr(NULL); // ptr to structure returned from NetShareEnum
      res = NetShareEnum(NULL, 2, (LPBYTE *) &BufPtr, MAX_PREFERRED_LENGTH, &dwEntriesRead, &dwTotalEntries, &dwResume_Handle); // CR.29294; modified.
      Win32Sec::CSmartNetApiBuf smartp(BufPtr);                                     // CR.29294; added.
      if (res != ERROR_SUCCESS && res != ERROR_MORE_DATA) return;

      // Search through the entries returned.
      for (DWORD i = 0; i < dwEntriesRead; i++)                                     // CR.29412; modified.
      {
         PSHARE_INFO_2 p(BufPtr + i);                                               // CR.29412; added.

#if 0
         CEnterExitMsg::TraceMsg(L"%s() Share: %s  Mapping: %s", __WFUNCTION__, pNetShareBuf[i].shi2_netname, pNetShareBuf[i].shi2_path);
#endif
         // only interested in DISK shares (not print Qs, Comm devices, etc.)
         if ((p->shi2_type & STYPE_MASK) != STYPE_DISKTREE) continue;               // CR.29412; added.

         // only interested if the path contains the drive letter being mapped.
         if (wsUncShareName.compare(p->shi2_netname, false) != 0) continue;         // CR.29058; modified.

         // share names match! Initialize the mapped share name members.
         m_wsMappedLocalPathName = p->shi2_path;                                    // CR.29412; modified.

         // Erase ending whack, if present, since m_wsPathName starts with whack.
         if (m_wsMappedLocalPathName.wcslen() && m_wsMappedLocalPathName[m_wsMappedLocalPathName.wcslen()-1] == L'\\')
            m_wsMappedLocalPathName.erase(m_wsMappedLocalPathName.wcslen() - 1);
         m_bMappedLocalPathInitialized = true; 
         return;                                                                    // CR.29294; modified.
      }
   }
}

/* UncNameToLocalMapFolder() - returns true if able to map UNC path to a local path.
 *
 * SLB 01.apr.2017 CR.23305; Much simplified. Modified to use existing mapping if it 
 * exists. If it does not, it returns false.
 * SLB 23.jan.2023 CR.32082; Now throws an exception if this object's Foldername is
 * not initialized.
 *------------------------------------------------------------------------------------------------------*/
bool CNtfsFolderName::UncNameToLocalMapFolder(
   CSpdCString &ws, 
   DWORD &dwNtErr, 
   bool bAddEndingWhack) const
{
   IsFolderNameInitialized_Check(__WFUNCTION__);                                    // CR.32082; added.

   ws.clear();
   dwNtErr = 0; 

   // If the format is already non-UNC.  No conversion is required.
   // Just retrieve the full folder name and return it.
   // Exit immediately if this has already been attempted and the share name is empty 
   // This method is const. if the share map has not been initialized, return false.
   if (!IsUncFormat()) { GetFullFolderName(ws, bAddEndingWhack);  return true;  }
   if (!m_bMappedLocalPathInitialized) return false; 
   if (m_bMappedLocalPathInitialized && m_wsMappedLocalPathName.empty()) return false; 

   // Build the path to be returned.
   ws = m_wsMappedLocalPathName + m_wsPathName; 
   if (bAddEndingWhack) return true; 

   // no ending '\\' desired... erase it if this is not the root folder
   if (m_wsPathName.wcslen() > 1) ws.erase(ws.wcslen() -1); 
   return true; 
}

/* CreateFolder() -
 *
 * SLB 18.nov.2019 CR.30115; Cleaned method up
 * SLB 23.jan.2023 CR.32082; Now throws an exception if this object's Foldername is
 * not initialized.
 *------------------------------------------------------------------------------------------------------*/
bool CNtfsFolderName::CreateFolder(
   DWORD &dwNtErr, 
   const LPSECURITY_ATTRIBUTES psaSecAttribs) const 
{
   IsFolderNameInitialized_Check(__WFUNCTION__);                                    // CR.32082; added.

   dwNtErr = 0;

   // Create the folder. 
   // Note: Uses shell version of CreateDirectory(), SHCreateDirectoryEx(), so that any non-existent base
   // directories will get created as well.
   CSpdCString wsFolderName;
   GetFullFolderName(wsFolderName, false);
   dwNtErr = SHCreateDirectoryEx(NULL, wsFolderName.c_str(), psaSecAttribs);
   switch (dwNtErr)
   {
   case ERROR_SUCCESS:         // 0
   case ERROR_ALREADY_EXISTS:  // 183
   case ERROR_FILE_EXISTS:     // 80
   case ERROR_FILE_NOT_FOUND:  // 2
      return true;
   }

   return false; 
}


/* DeleteFolder() -
 *
 * SLB 14.mar.2019 CR.26999.Item100; cleaned up this method.
 * SLB 02.apr.2020 CR.29220; Added protection against deleting system folders.
 * SLB 23.jan.2023 CR.32082; Now throws an exception if this object's Foldername is
 * not initialized.
 *------------------------------------------------------------------------------------------------------*/
bool CNtfsFolderName::DeleteFolder(
   DWORD &dwNtErr) const 
{
   CEnterExitMsg eem(__WFUNCTION__, L"Folder <%s>", GetFullFolderName(false).c_str());

   IsFolderNameInitialized_Check(__WFUNCTION__);                                    // CR.32082; added.

   if (IsSystemFolder()) return false;                                              // CR.29220; added.

   BOOL bRES;
   bRES = RemoveDirectoryW(GetFullFolderName(false).c_str());
   dwNtErr = GetLastError();
   if (bRES == FALSE) return false;

   return true; 
}


/* DeleteFilesAndFolders() - Deletes all files and subfolders. Deletes current folder, if specified.
 * This is a recursive method and returns no notice if errors occurred or not. Does not throw exceptions either.
 *
 * SLB 27.mar.2019 CR.26999.Item100; created.
 * SLB 02.apr.2020 CR.29220; Added protection against deleting system folders and their files.
 * SLB 23.jan.2023 CR.32082; Now throws an exception if this object's Foldername is
 * not initialized.
 ----------------------------------------------------------------------------------------------*/
void CNtfsFolderName::DeleteFilesAndFolders(
   bool bIncludeThisFolder) const
{
   IsFolderNameInitialized_Check(__WFUNCTION__);                                    // CR.32082; added.

   DWORD dwNtErr;
   if (!DoesFolderExist(dwNtErr, false)) return;
   if (IsSystemFolder()) return;                                                    // CR.29220; added.

   // Use temp object so filename can be set.
   CNtfsFolderName fnFolder(*this);
   fnFolder.SetFileName(L"*.*", true, false);

   // Delete all subfolders and there files too..
   // Open a handle for scanning folder objects.
   // exit if this cannot be opened (not a folder object?)
   HANDLE hFind;
   WIN32_FIND_DATAW fiwFileInfo;
   hFind = FindFirstFileExW(
      fnFolder.GetFullFileName(true).c_str(),
      FindExInfoStandard,
      &fiwFileInfo,
      FindExSearchNameMatch,
      NULL,
      0);
   if (hFind == INVALID_HANDLE_VALUE) return;

   // Use smart ptr class to ensure that the handle allocated by FindFirstFileExW()
   // will get closed, even if an exception occurs.
   CSmartFindHandle sfhHandle(hFind);
   for (BOOL bIsThereAFile(TRUE); bIsThereAFile == TRUE; bIsThereAFile = FindNextFileW(hFind, &fiwFileInfo))
   {
      // Find a folder. Can't be a system folder. Can't be '.' or '..' folders.
      if ((fiwFileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) continue;
      if ((fiwFileInfo.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != 0) continue;
      if (fiwFileInfo.cFileName[0] == L'.') continue;

      // Setup folder object to point to this folder. Then make recursive call to delete that folder.
      CNtfsFolderName fnSubFolder(*this);
      fnSubFolder.ChangeDirectory(fiwFileInfo.cFileName);
      fnSubFolder.DeleteFilesAndFolders(true);
   }

   // Done walking through all sub-folders.
   // Delete all files in this folder.
   fnFolder.Ntfs_DeleteFiles();

   // Exit if not supposed to delete this folder as well.
   if (!bIncludeThisFolder) return;

   // Make sure READ_ONLY and HIDDEN attributes get cleared, Then delete this folder.
   DWORD dwAttributes(0);
   Ntfs_GetFileAttributes(dwAttributes, true);
   Ntfs_SetFileAttributes(dwAttributes & ~(FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY), true);
   Ntfs_DeleteFolder();
}


/* GetWriteAccessDesired() -
 *------------------------------------------------------------------*/
bool CNtfsFolderName::GetWriteAccessDesired() const
{
   return m_bWriteAccessDesired; 
}

/* GetFileSize() -
 *
 * SLB 09.apr.2002 CR.29234.V02; added.
 *------------------------------------------------------------------*/
QUADWORD CNtfsFolderName::GetFileSize(
   HANDLE hFile,
   const CSpdCString &wsFileName) const
{
   BOOL bRES(FALSE);
   LARGE_INTEGER liFileSize;
   bRES = GetFileSizeEx(hFile, &liFileSize);
   DWORD le = GetLastError();
   if (bRES == FALSE) ThrowHr(HRESULT_FROM_WIN32(le), __WFUNCTION__, L"Problem retrieving files size. Call to Win32 GetFileSize() failed for %s.", wsFileName.c_str());

   QUADWORD qwRetVal(MAKEQUADWORD(liFileSize.LowPart, liFileSize.HighPart));
   return qwRetVal;
}

/* GetFileSize() -
 *
 * SLB 01.nov.2010 CR.13842; added.
 * SLB 14.jan.2019 CR.26999.Item55; restructured code to throw exceptions.
 * SLB 10.apr.2020 CR.29234.V02; Moved some functionality into new overload, which takes
 * a handle. Now leverages that method.
 * SLB 23.jan.2023 CR.32082; Now throws an exception if this object's Foldername or
 * Filename is not initialized.
 *------------------------------------------------------------------*/
QUADWORD CNtfsFolderName::GetFileSize() const
{
   IsFolderNameInitialized_Check(__WFUNCTION__);                                    // CR.32082; added.
   IsFileNameInitialized_Check(__WFUNCTION__);                                      // CR.32082; added.

   CSpdCString wsFileName(GetFullFileName(true, false, true));
   CSmartHandle shFile;
   shFile = CreateFileW(
      wsFileName.c_str(),
      GENERIC_READ,
      FILE_SHARE_DELETE|FILE_SHARE_READ|FILE_SHARE_WRITE,
      NULL,
      OPEN_EXISTING,
      0,
      NULL); 
   DWORD le(GetLastError());
   if (shFile == INVALID_HANDLE_VALUE) ThrowHr(HRESULT_FROM_WIN32(le), __WFUNCTION__, L"Problem retrieving files size. Call to Win32 CreateFile() failed for %s.", wsFileName.c_str());

   // successfully opened file. now retrieve file size.
   return GetFileSize(shFile, wsFileName);                                          // CR.29234.V02; added.
}


/* GetFolderFileCount() - returns total number of files in this folder.
 * Throws hr_exception on error.
 *
 * SLB 16.aug.2019 CR.28247; created.
 * SLB 23.jan.2023 CR.32082; Now throws an exception if this object's Foldername is
 * not initialized.
 *------------------------------------------------------------------------------------------------------*/
size_t CNtfsFolderName::GetFolderFileCount() const
{
   IsFolderNameInitialized_Check(__WFUNCTION__);                                    // CR.32082; added.

   CSpdCString wsFileMask(GetFullFolderName(true));
   wsFileMask += L"*";

   HANDLE hFind;
   WIN32_FIND_DATAW fiwFileInfo;
   hFind = FindFirstFileExW(
      wsFileMask.c_str(),
      FindExInfoBasic,
      &fiwFileInfo,
      FindExSearchNameMatch,
      NULL,
      0);
   if (hFind == INVALID_HANDLE_VALUE) ThrowHr(HRESULT_FROM_WIN32(GetLastError()), __WFUNCTION__, L"Problem opening Find handle for the folder.");

   CSmartFindHandle sfhHandle(hFind);
   size_t stFileCount(0);
   for (BOOL bIsThereAFile(TRUE); bIsThereAFile == TRUE; bIsThereAFile = FindNextFileW(hFind, &fiwFileInfo))
   {
      // Does not caree about folders or system objects
      if ((fiwFileInfo.dwFileAttributes & (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_SYSTEM))) continue;

      stFileCount++;
   }
   return stFileCount;
}

/* Ntfs_SetFileAttributes() - Sets current filenames attributes on disk using SetFileAttributesW(). Can retry if desired.
 *
 * SLB 28.mar.2017 CR.23304; created.
 * SLB 28.apr.2017 CR.22874; made method const.
 * SLB 23.jan.2023 CR.32082; Now throws an exception if this object's Foldername or
 * Filename is not initialized.
 *------------------------------------------------------------------------------------------------------*/
bool CNtfsFolderName::Ntfs_SetFileAttributes(
   DWORD dwFileAttributes,
   bool bUseFolderName,                                                             // CR.26999.Item55; added.
   DWORD dwMaxRetryCount, 
   DWORD dwTimeBetweenRetriesMS) const                                              // CR.22874; modified.
{
   IsFolderNameInitialized_Check(__WFUNCTION__);                                    // CR.32082; added.
   if (!bUseFolderName) IsFileNameInitialized_Check(__WFUNCTION__);                 // CR.32082; added.

   CSpdCString wsFileName;
   if (!bUseFolderName) wsFileName = GetFullFileName(true, false, true, true);
   else wsFileName = GetFullFolderName(false);

   DWORD dwTriesLeft(1 + dwMaxRetryCount);
   while (true)
   {
      BOOL bRES; 
      bRES = SetFileAttributesW(wsFileName.c_str(), dwFileAttributes); 
      if (bRES != FALSE) return true; 

      // Failed. exit if no tries left.
      dwTriesLeft--;
      if (dwTriesLeft == 0) return false;

      // Give up processor for delay interval before retrying again.
      Sleep(dwTimeBetweenRetriesMS); 
   }

   // Should never reach this code, but return false.
   return false; 
}

/* Ntfs_GetFileAttributes() - Gets current file attributes using GetFileAttributesW(). Can retry if desired.
 *
 * SLB 25.jan.2019 CR.26999.Item55; Added.
 * SLB 23.jan.2023 CR.32082; Now throws an exception if this object's Foldername or
 * is Filename is not initialized.
 *------------------------------------------------------------------------------------------------------*/
bool CNtfsFolderName::Ntfs_GetFileAttributes(
   DWORD & dwFileAttributes, 
   bool bUseFolderName, 
   DWORD dwMaxRetryCount, 
   DWORD dwTimeBetweenRetriesMS) const
{
   // CR.32082; Note: Foldername must be initialized, but filename does not... 
   // Because the attributes of a folder can be retrieved as well.
   IsFolderNameInitialized_Check(__WFUNCTION__);                                    // CR.32082; added.
   if (!bUseFolderName) IsFileNameInitialized_Check(__WFUNCTION__);                 // CR.32082; added.

   CSpdCString wsFileName;
   if (!bUseFolderName) wsFileName = GetFullFileName(true, false, true, true);
   else wsFileName = GetFullFolderName(false);

   DWORD dwTriesLeft(1 + dwMaxRetryCount);
   while (true)
   {
      dwFileAttributes = GetFileAttributesW(wsFileName.c_str()); 
      if (dwFileAttributes != INVALID_FILE_ATTRIBUTES) return true;

      // Failed. exit if no tries left.
      dwTriesLeft--;
      if (dwTriesLeft == 0) return false;

      // Give up processor for delay interval before retrying again.
      Sleep(dwTimeBetweenRetriesMS); 
   }

   // Should never reach this code, but return false.
   return false; 
}

/* Ntfs_MoveFile() - Moves the current filename on disk using MoveFileEx(). Can retry if desired.
 * Can use wildcards to move multiple files as long as DestFileName is a valid folder.
 *
 * SLB 17.mar.2017 CR.23216.V01; created.
 * SLB 28.mar.2017 CR.23304; Moved from disk spooler code to be more accessible.
 * SLB 20.apr.2017 CR.23380; Fix for not breaking out of retry loop when MoveFileExW() fails.
 * SLB 28.apr.2017 CR.22874; made method const.
 * SLB 02.apr.2020 CR.29220; Added protection against moving systems files.
 * SLB 05.feb.2021 CR.29859.V01; Modifications to ensure that GetLastError() will be valid after 
 * this method has completed.
 * SLB 23.jan.2023 CR.32082; Now throws an exception if this object's Foldername or
 * Filename is not initialized.
----------------------------------------------------------------------------------------------*/
bool CNtfsFolderName::Ntfs_MoveFile(
   const CSpdCString &wsDestFileName,
   DWORD dwMoveFlags,
   DWORD dwMaxRetryCount,
   DWORD dwTimeBetweenRetriesMS) const                                              // CR.22874; modified.
{
   IsFolderNameInitialized_Check(__WFUNCTION__);                                    // CR.32082; added.
   IsFileNameInitialized_Check(__WFUNCTION__);                                      // CR.32082; added.

   if (IsSystemFile()) return false;                                                // CR.29220; added.

   CSpdCString wsSourceFileName(GetFullFileName(true, false, true));
   DWORD dwTriesLeft(1 + dwMaxRetryCount);
   while (true) 
   {
      // Attempt the Move. If successful, return true.
      BOOL bRES; 
      bRES = MoveFileExW(wsSourceFileName.c_str(), wsDestFileName.c_str(), dwMoveFlags); 
      if (bRES != FALSE) return true;

      // Cache Last Error code before calling  TraceMsg().
      DWORD le(GetLastError());                                                     // CR.29859.V01; added.
      CEnterExitMsg::TraceMsg(L"(%08X) %s() Call to MoveFileEx() failed. Operation may be retried.\n  SrcFile <%s>\n  DstFile <%s>\n  Flags <%08X>", 
         HRESULT_FROM_WIN32(le), 
         __WFUNCTION__,
         wsSourceFileName.c_str(), 
         wsDestFileName.c_str(),
         dwMoveFlags);

      // Failed. exit if no tries left.
      dwTriesLeft--;

      // Restore Last Error code before exiting.
      SetLastError(le);                                                             // CR.29859.V01; added.
      if (dwTriesLeft == 0) return false;

      // Give up processor for delay interval before retrying again.
      Sleep(dwTimeBetweenRetriesMS); 
   }

   // Should never reach this code, but return false.
   return false; 
}

/* Ntfs_MoveFolder() - Moves the current folder on disk using MoveFileEx(). Can retry if desired.
 * Always moves the current folder. If only the files need to be moved, then use Ntfs_MoveFile() 
 * with filename set to *.*.
 * Implementation note: This method is a modified copy of Ntfs_MoveFile().
 * 
 * SLB 13.mar.2025 CR.32662.V06; created.
----------------------------------------------------------------------------------------------*/
bool CNtfsFolderName::Ntfs_MoveFolder(
	const CSpdCString& wsDestFolderName, 
	DWORD dwMoveFlags, 
	DWORD dwMaxRetryCount, 
	DWORD dwTimeBetweenRetriesMS) const
{
	IsFolderNameInitialized_Check(__WFUNCTION__);

	if (IsSystemFolder()) return false; 

	CSpdCString wsSourceFolderName(GetFullFolderName(false));
	DWORD dwTriesLeft(1 + dwMaxRetryCount);
	while (true)
	{
		// Attempt the Move. If successful, return true.
		BOOL bRES;
		bRES = MoveFileExW(wsSourceFolderName.c_str(), wsDestFolderName.c_str(), dwMoveFlags);
		if (bRES != FALSE) return true;

		// Cache Last Error code before calling  TraceMsg().
		DWORD le(GetLastError()); 
		CEnterExitMsg::TraceMsg(L"(%08X) %s() Call to MoveFileEx() failed. Operation may be retried.\n  SrcFolder <%s>\n  DstFolder <%s>\n  Flags <%08X>",
			HRESULT_FROM_WIN32(le),
			__WFUNCTION__,
			wsSourceFolderName.c_str(),
			wsDestFolderName.c_str(),
			dwMoveFlags);

		// Failed. exit if no tries left.
		dwTriesLeft--;

		// Restore Last Error code before exiting.
		SetLastError(le); 
		if (dwTriesLeft == 0) return false;

		// Give up processor for delay interval before retrying again.
		Sleep(dwTimeBetweenRetriesMS);
	}

	// Should never reach this code, but return false.
	return false;
}

/* Ntfs_CopyFile() - Copies the current filename on disk using CopyFileW(). Can retry if desired.
 *
 * SLB 03.apr.2018 created.
 * SLB 02.apr.2020 CR.29220; Added protection against copying systems files.
 * SLB 23.jan.2023 CR.32082; Now throws an exception if this object's Foldername or
 * Filename is not initialized.
 ----------------------------------------------------------------------------------------------*/
bool CNtfsFolderName::Ntfs_CopyFile(
   const CSpdCString &wsDestFileName,
   bool bFailIfExists,
   DWORD dwMaxRetryCount,
   DWORD dwTimeBetweenRetriesMS) const 
{
   IsFolderNameInitialized_Check(__WFUNCTION__);                                    // CR.32082; added.
   IsFileNameInitialized_Check(__WFUNCTION__);                                      // CR.32082; added.

   if (IsSystemFile()) return false;                                                // CR.29220; added.

   CSpdCString wsSourceFileName(GetFullFileName(true, false, true));
   DWORD dwTriesLeft(1 + dwMaxRetryCount);
   while (true)
   {
      // Attempt the Copy. If successful, return true.
      BOOL bRES;
      bRES = CopyFileW(wsSourceFileName.c_str(), wsDestFileName.c_str(), bFailIfExists?TRUE:FALSE);
      if (bRES != FALSE) return true;

      // Failed. exit if no tries left.
      dwTriesLeft--;
      if (dwTriesLeft == 0) return false;

      // Give up processor for delay interval before retrying again.
      Sleep(dwTimeBetweenRetriesMS);
   }

   // Should never reach this code, but return false.
   return false;
}

/* Ntfs_DeleteFile() - Deletes the current filename on disk using DeleteFileW(). Can retry if desired.
 *
 * SLB 13.dec.2018 CR.7698; created.
 * SLB 02.apr.2020 CR.29220; Added protection against deleting systems files.
 * SLB 23.jan.2023 CR.32082; Now throws an exception if this object's Foldername or
 * Filename is not initialized.
 ----------------------------------------------------------------------------------------------*/
bool CNtfsFolderName::Ntfs_DeleteFile(
   DWORD dwMaxRetryCount,
   DWORD dwTimeBetweenRetriesMS) const
{
   IsFolderNameInitialized_Check(__WFUNCTION__);                                    // CR.32082; added.
   IsFileNameInitialized_Check(__WFUNCTION__);                                      // CR.32082; added.

   if (IsSystemFile()) return false;                                                // CR.29220; added.

   DWORD dwTriesLeft(1 + dwMaxRetryCount);
   while (true)
   {
      // Attempt the Delete. If successful, return true.
      BOOL bRES;
      bRES = DeleteFileW(GetFullFileName(true, false, true).c_str());
      if (bRES != FALSE) return true;

      // Failed. exit if no tries left.
      dwTriesLeft--;
      if (dwTriesLeft == 0) return false;

      Sleep(dwTimeBetweenRetriesMS);
   }

   // Should never reach this code, but return false.
   return false;
}

/* Ntfs_CopyFolder() - Copies all files in the current folder to the destination folder.
 *
 * SLB 17.aug.2018 CR.17862.V08; created.
 * SLB 31.may.2019 CR.27815; Added more details to exception message to help identify the problematic file.
 * SLB 02.apr.2020 CR.29220; Added protection against copying system folders.
 * SLB 23.jan.2023 CR.32082; Now throws an exception if this object's Foldername is
 * not initialized.
 ----------------------------------------------------------------------------------------------*/
void CNtfsFolderName::Ntfs_CopyFolder(
   const CNtfsFolderName & fnDestFolder, 
   bool bOverWriteExisting, 
   DWORD dwMaxRetryCount, 
   DWORD dwTimeBetweenRetriesMS) const
{
   IsFolderNameInitialized_Check(__WFUNCTION__);                                    // CR.32082; added.

   if (IsSystemFolder()) return;                                                    // CR.29220; added.

   DWORD dwNtErr;
   if (!DoesFolderExist(dwNtErr)) ThrowHr(E_FAIL, __WFUNCTION__, L"Folder copy failed. Source folder does not exist. %s", GetFullFolderName(false).c_str());
   if (!fnDestFolder.DoesFolderExist(dwNtErr)) ThrowHr(E_FAIL, __WFUNCTION__, L"Folder copy failed. Destination folder does not exist. %s", fnDestFolder.GetFullFolderName(false).c_str());

   // Open a handle for scanning folder objects.
   // exit if this cannot be opened (not a folder object?)
   HANDLE hFind;
   CNtfsFolderName fnSource(*this);
   fnSource.SetFileName(L"*.*", true, false);
   WIN32_FIND_DATAW fiwFileInfo; 
   hFind = FindFirstFileExW(
      fnSource.GetFullFileName().c_str(), 
      FindExInfoStandard, 
      &fiwFileInfo, 
      FindExSearchNameMatch, 
      NULL, 
      0);   
   if (hFind == INVALID_HANDLE_VALUE) return;

   // Use smart ptr class to ensure that the handle allocated by FindFirstFileExW()
   // will get closed, even if an exception occurs.
   CSmartFindHandle sfhHandle(hFind);   
   CNtfsFolderName fnDest(fnDestFolder);
   for (BOOL bIsThereAFile(TRUE); bIsThereAFile == TRUE; bIsThereAFile = FindNextFileW(hFind, &fiwFileInfo))
   {
      // Does not delete folders or system objects
      if ((fiwFileInfo.dwFileAttributes & (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_SYSTEM))) continue;

      // Setup filenames.
      fnSource.SetFileName(fiwFileInfo.cFileName, true);
      fnDest.SetFileName(fiwFileInfo.cFileName, true);
      bool bRes;
      bRes = fnSource.Ntfs_CopyFile(fnDest.GetFullFileName(), !bOverWriteExisting, dwMaxRetryCount, dwTimeBetweenRetriesMS);

      // copy errors are ignored.
   }
}

/* Ntfs_DeleteFiles() - Deletes file(s) currently specified in object. Note: this can be multiple files if 
 * filename and/or fileext are wildcards. This method will clear HIDDEN and READONLY flags to allow for files to 
 * be deleted.
 *
 * SLB 17.aug.2018 CR.17862.V08; created.
 * SLB 02.apr.2020 CR.29220; Added protection against deleting files from system folders.
 * SLB 23.jan.2023 CR.32082; Now throws an exception if this object's Foldername is
 * not initialized.
 ----------------------------------------------------------------------------------------------*/
void CNtfsFolderName::Ntfs_DeleteFiles() const
{
   IsFolderNameInitialized_Check(__WFUNCTION__);                                    // CR.32082; added.

   if (IsSystemFolder()) return;                                                    // CR.29220; added.

   // Open a handle for scanning folder objects.
   // exit if this cannot be opened (not a folder object?)
   HANDLE hFind;
   WIN32_FIND_DATAW fiwFileInfo; 
   hFind = FindFirstFileExW(
      GetFullFileName().c_str(), 
      FindExInfoStandard, 
      &fiwFileInfo, 
      FindExSearchNameMatch, 
      NULL, 
      0);   
   if (hFind == INVALID_HANDLE_VALUE) return;

   // Use smart ptr class to ensure that the handle allocated by FindFirstFileExW()
   // will get closed, even if an exception occurs.
   CSmartFindHandle sfhHandle(hFind);   
   CNtfsFolderName fnFile(*this);
   for (BOOL bIsThereAFile(TRUE); bIsThereAFile == TRUE; bIsThereAFile = FindNextFileW(hFind, &fiwFileInfo))
   {
      // Does not delete folders or system objects
      if ((fiwFileInfo.dwFileAttributes & (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_SYSTEM))) continue;

      // Setup filename.
      // Make sure READ_ONLY and HIDDEN attributes get cleared so system can delete the file.
      fnFile.SetFileName(fiwFileInfo.cFileName, true);
      if ((fiwFileInfo.dwFileAttributes & (FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY)))
      {
         fnFile.Ntfs_SetFileAttributes(FILE_ATTRIBUTE_NORMAL);
      }

      // Use Win32 to delete the file.
      fnFile.Ntfs_DeleteFile();
   }
}


/* Ntfs_DeleteFolder() - Deletes the current folder on disk using DeleteFolder(). Can retry if desired.
 *
 * SLB 14.mar.2019 CR.26999.Item100; created.
 * SLB 02.apr.2020 CR.29220; Added protection against deleting system folders.
 * SLB 23.jan.2023 CR.32082; Now throws an exception if this object's Foldername is
 * not initialized.
 ----------------------------------------------------------------------------------------------*/
bool CNtfsFolderName::Ntfs_DeleteFolder(
   DWORD dwMaxRetryCount,
   DWORD dwTimeBetweenRetriesMS) const
{
   IsFolderNameInitialized_Check(__WFUNCTION__);                                    // CR.32082; added.

   if (IsSystemFolder()) return false;                                              // CR.29220; added.

   DWORD dwTriesLeft(1 + dwMaxRetryCount);
   while (true)
   {
      // Attempt the Delete. If successful, return true.
      DWORD dwNtErr;
      if (DeleteFolder(dwNtErr))
      {
         if (dwTriesLeft <= dwMaxRetryCount) CEnterExitMsg::TraceMsg(L"%s() - Retries were needed.", __WFUNCTION__);
         return true;
      }

      // Failed. exit if no tries left.
      SetLastError(dwNtErr);
      dwTriesLeft--;
      if (dwTriesLeft == 0) return false;

      Sleep(dwTimeBetweenRetriesMS);
   }

   // Should never reach this code, but return false.
   return false;
}


/* TestFolderAccessRight() - This method checks the specified folder to see if at has rights 
 * to the specified AccessRight.  Return value is a Win32 error code.  0 = access right is 
 * granted.
 *
 * SLB 15.jan.2007 CR.7666; Added.
 *
 * SLB 15.aug.2010 CR.13337; Changed usage of _snwprintf() to _snwprintf_s().
 * SLB 02.apr.2020 CR.29220; Modified so CSmartHandle is used for auto cleanup of CreateFile handle.
 *------------------------------------------------------------------------------------------------------*/
DWORD CNtfsFolderName::TestFolderAccessRight(
   const wchar_t *pwcsFolderName, 
   DWORD dwAccessRight)
{
   DWORD dwNtErr(ERROR_SUCCESS); 
   CSmartHandle shFolder;                                                           // CR.29220; modified.
   shFolder = CreateFileW(
      pwcsFolderName,
      dwAccessRight,
      FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
      NULL,
      OPEN_EXISTING,
      FILE_FLAG_BACKUP_SEMANTICS, // required for opening directories
      NULL); 
   if (shFolder == INVALID_HANDLE_VALUE)
   {  
      dwNtErr = GetLastError(); 
      if (s_bIsExtendedLoggingOn)
      {
         CSpdCString wsWin32Error(SpdUtils::GetWin32ErrorMessage(dwNtErr));
         CEnterExitMsg::TraceMsg(L"SPDE Test: CNtfsFolderName::TestFolderAccessRight(%08X) - disabled: %s.\n%s", // CR.13337
            dwAccessRight,
            GetAccessFlagDescription(dwAccessRight).c_str(),
            wsWin32Error.c_str()); 
      }
      return dwNtErr;
   }

   // opened the folder successfully!
   if (s_bIsExtendedLoggingOn)
   {
      CEnterExitMsg::TraceMsg(L"SPDE Test: CNtfsFolderName::TestFolderAccessRight(%08X) - ENABLED : %s.",
         dwAccessRight,
         GetAccessFlagDescription(dwAccessRight).c_str()); 
   }
   return ERROR_SUCCESS; 
}


/* BuildAccessFlagDescriptionMap() - Initializes the s_AccessFlagDescriptions map which
 * is used for generating user readable messages.
 *
 * SLB 15.jan.2007 CR.7666; Added.
 * SLB 31.aug.2007 CR.6135; Removed some duplicate entries added to array.
 *------------------------------------------------------------------------------------------------------*/
void CNtfsFolderName::BuildAccessFlagDescriptionMap()
{  
   if (!s_AccessFlagDescriptions.empty()) return;

   s_AccessFlagDescriptions[FILE_LIST_DIRECTORY] = L"FILE_LIST_DIRECTORY | FILE_READ_DATA"; 
   s_AccessFlagDescriptions[FILE_WRITE_DATA] = L"FILE_WRITE_DATA  | FILE_ADD_FILE"; 
   s_AccessFlagDescriptions[FILE_APPEND_DATA] = L"FILE_APPEND_DATA  | FILE_ADD_SUBDIRECTORY |  FILE_CREATE_PIPE_INSTANCE"; 
   s_AccessFlagDescriptions[FILE_READ_EA] = L"FILE_READ_EA"; 
   s_AccessFlagDescriptions[FILE_WRITE_EA] = L"FILE_WRITE_EA"; 
   s_AccessFlagDescriptions[FILE_EXECUTE] = L"FILE_EXECUTE | TRAVERSE"; 
   s_AccessFlagDescriptions[FILE_DELETE_CHILD] = L"FILE_DELETE_CHILD"; 
   s_AccessFlagDescriptions[FILE_READ_ATTRIBUTES] = L"FILE_READ_ATTRIBUTES"; 
   s_AccessFlagDescriptions[FILE_WRITE_ATTRIBUTES] = L"FILE_WRITE_ATTRIBUTES"; 
   s_AccessFlagDescriptions[DELETE] = L"DELETE"; 
   s_AccessFlagDescriptions[READ_CONTROL] = L"READ_CONTROL"; 
   s_AccessFlagDescriptions[WRITE_DAC] = L"WRITE_DAC"; 
   s_AccessFlagDescriptions[WRITE_OWNER] = L"WRITE_OWNER"; 
   s_AccessFlagDescriptions[SYNCHRONIZE] = L"SYNCHRONIZE"; 
   s_AccessFlagDescriptions[ACCESS_SYSTEM_SECURITY] = L"ACCESS_SYSTEM_SECURITY"; 
   s_AccessFlagDescriptions[MAXIMUM_ALLOWED] = L"MAXIMUM_ALLOWED"; 
   s_AccessFlagDescriptions[GENERIC_ALL] = L"GENERIC_ALL"; 
   s_AccessFlagDescriptions[GENERIC_EXECUTE] = L"GENERIC_EXECUTE"; 
   s_AccessFlagDescriptions[GENERIC_WRITE] = L"GENERIC_WRITE"; 
   s_AccessFlagDescriptions[GENERIC_READ] = L"GENERIC_READ"; 
}

/* GetAccessFlagDescription() - retrieves the user readable discription for an access flag 
 * entry.  Note, only one bit flag should be set per call to this method.
 *
 * SLB 15.jan.2007 CR.7666; Added.
 *------------------------------------------------------------------------------------------------------*/
const CSpdCString & CNtfsFolderName::GetAccessFlagDescription(
   DWORD dwAccessFlag)
{
   BuildAccessFlagDescriptionMap(); 

   static CSpdCString s_InvalidFlag(L"Invalid flag");

   TItcAccessBitDescriptionMap it(s_AccessFlagDescriptions.find(dwAccessFlag));
   if (it == s_AccessFlagDescriptions.end()) return s_InvalidFlag;

   return it->second;
}

/* EnableExtendedLogging() 
 * JWL 30.jul.2012 CR.17124.Item37 - Can't have inline functions that use statics in an exported function,
 * so moved from .h to a .cpp
 *------------------------------------------------------------------------------------------------------*/
void CNtfsFolderName::EnableExtendedLogging() 
{ 
   s_bIsExtendedLoggingOn = true;  
}

/* DisableExtendedLogging() 
 * JWL 30.jul.2012 CR.17124.Item37 - Can't have inline functions that use statics in an exported function,
 * so moved from .h to a .cpp
 *------------------------------------------------------------------------------------------------------*/
void CNtfsFolderName::DisableExtendedLogging() 
{ 
   s_bIsExtendedLoggingOn = false;  
}

/* GetMacineNameFQ() - static method which converts machine name into a fully qualified Network Name. 
 *
 * Note: treats empty string as localhost.
 * 
 * SLB 18.mar.2020 CR.29058.V01; created. Converted existing code in SetUncHostName() into this static method.
 * SLB 25.jun.2020 CR.29471; Modified to use GetComputerNameEx() to retrieve current machine's Domain name vs.
 * calculating it from the Fully Qualified machine name. 
 * Also, now checks to see if requested machine name is the Domain Controller... and takes that into account 
 * when constructing the FQDN of the machine.
 *------------------------------------------------------------------------------------------------------*/
CSpdCString CNtfsFolderName::GetMacineNameFQ(
    const CSpdCString & wsMachineName)
{
   // CR.29148; handle localhost situation.
   if (wsMachineName.compare(L"localhost", false) == 0) return GetComputerNameW();  
   if (wsMachineName.empty()) return GetComputerNameW();

   // CR.29058 functionality. 
   // Check to see if this is a fully qualified name. If not, convert it.
   // Presumes that if there is a '.' character, then this is a fully qualified name.
   size_t stPos;
   stPos = wsMachineName.find_first_of(L'.');
   if (stPos != CSpdCString::npos) return wsMachineName;

   // Needs conversion. Get fully qualified domain name (of this machine).
   // If GetComputerNameEx() returns FALSE, then presumption is that this machine is not 
   // on a domain so return target's machine name untouched.
   static CSpdCString s_wsDomain;                                                   // CR.29471; added.
   static CSpdCString s_wsDC;                                                       // CR.29471; added.
   if (s_wsDomain.empty())
   {
      // Initialize the Statics. 
      BOOL bRES;
      wchar_t wcsFQDN[1024];
      DWORD dwTcharSize((DWORD)(sizeof(wcsFQDN) / sizeof(wchar_t)));
      bRES = GetComputerNameEx(ComputerNamePhysicalDnsDomain, wcsFQDN, &dwTcharSize); // CR.29471; added.
      if (bRES == FALSE) return wsMachineName;                                      // CR.29471; added.

      // Domain name retrieved successfully
      s_wsDomain = wcsFQDN;

      // Determine Domain Controller machine name from this.
      stPos = s_wsDomain.find_first_of(L'.');
      s_wsDC = s_wsDomain.substr(0, stPos);

      CEnterExitMsg::TraceMsg(L"CR.29471; %s() initialized Domain <%s> DC<%s>", __WFUNCTION__, s_wsDomain.c_str(), s_wsDC.c_str());
   }

   // CR.29471; If domain is empty, then return machine name by itself (this will be the NETBIOS name)
   if (s_wsDomain.empty()) return wsMachineName;

   // CR.29471; Check to see if this machine name is the Domain Controller. In which case, FQDN and Domain are 
   // one and the same.
   if (wsMachineName.compare(s_wsDC, false) == 0) return s_wsDomain;                // CR.29471; added.

   // Domain is set and this machine is NOT the DC. Build fully qualified name and return.
   return wsMachineName + L"." + s_wsDomain;                                        // CR.29471; modified.
}



