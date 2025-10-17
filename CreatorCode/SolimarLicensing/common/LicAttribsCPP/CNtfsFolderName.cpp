// CSpdMediumManagerNTFS.h : TODO: Add file description here.
//
#include "CNtfsFolderName.h"
#include "Win32Sec.h"

#include <shlobj.h>
#include <Shlwapi.h>
#include <lm.h>

#define wcsicmp _wcsicmp
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
CNtfsFolderName::TAccessFlagDescriptionMap CNtfsFolderName::s_AccessFlagDescriptions ;
bool CNtfsFolderName::s_bIsExtendedLoggingOn(false) ;


/* constructor - default constructor.
 *------------------------------------------------------------------*/
CNtfsFolderName::CNtfsFolderName() : 
   m_scsUncHostName(L"."),
   m_scsPathName(L"\\"),
   m_bIsUncFormat(true),
   m_bWriteAccessDesired(true)
{
}

/* constructor - copy constructor.
 *------------------------------------------------------------------*/
CNtfsFolderName::CNtfsFolderName(const CNtfsFolderName &c)
{
   *this = c ;
}

/* constructor - constructor which parses a known folder string and poplulates
 * this object with the contents found.
 *------------------------------------------------------------------*/
CNtfsFolderName::CNtfsFolderName(const CSpdCStringBase &scsFullFolderName,
                                 bool bWriteAccessDesired
                                 ) :
   m_bIsUncFormat(false),
   m_bWriteAccessDesired(bWriteAccessDesired)
{
   SetFullFolderName(scsFullFolderName) ;
}


/* constructor - constructor which takes individual paramaters, for 
 * the drive map format, to populate this object.
 *------------------------------------------------------------------*/
CNtfsFolderName::CNtfsFolderName(const CSpdCStringBase &scsDriveLetter,
                                 const CSpdCStringBase &scsPathName,
                                 const CSpdCStringBase &scsFileName,
                                 const CSpdCStringBase &scsFileExt,
                                 bool bWriteAccessDesired
                                 ) :
   m_bIsUncFormat(true),
   m_bWriteAccessDesired(bWriteAccessDesired)

{
   
   SetUncHostName(CSpdCStringF16(L".")) ;
   SetDriveLetter(scsDriveLetter) ;
   SetPathName(scsPathName) ;
   SetFileName(scsFileName, false) ;
   SetFileExt(scsFileExt) ;
}

/* constructor - constructor which takes individual paramaters, for 
 * the UNC map format, to populate this object.
 *------------------------------------------------------------------*/
CNtfsFolderName::CNtfsFolderName(const CSpdCStringBase &scsUncHostName,
                                 const CSpdCStringBase &scsUncShareName,
                                 const CSpdCStringBase &scsPathName,
                                 const CSpdCStringBase &scsFileName,
                                 const CSpdCStringBase &scsFileExt,
                                 bool bWriteAccessDesired
                                 ) :
   m_bIsUncFormat(true),
   m_bWriteAccessDesired(bWriteAccessDesired)
{
   SetUncHostName(scsUncHostName) ;
   SetUncShareName(scsUncShareName) ;
   SetPathName(scsPathName) ;
   SetFileName(scsFileName, false) ;
   SetFileExt(scsFileExt) ;
}



/* operator =() -
 *------------------------------------------------------------------*/
CNtfsFolderName & CNtfsFolderName::operator =(const CNtfsFolderName &c)
{
   m_bIsUncFormat = c.m_bIsUncFormat ;
   m_bWriteAccessDesired = c.m_bWriteAccessDesired ;
   m_scsDriveLetter = c.m_scsDriveLetter ;
   m_scsUncHostName = c.m_scsUncHostName ;
   m_scsUncShareName = c.m_scsUncShareName ;
   m_scsPathName = c.m_scsPathName ;
   m_scsFileName = c.m_scsFileName ;
   m_scsFileExt = c.m_scsFileExt ;
   return *this ;
}

/* operator =() -
 *------------------------------------------------------------------*/
CNtfsFolderName & CNtfsFolderName::operator =(const CSpdCStringBase &scs)
{
   SetFullFileName(scs) ;
   return *this ;
}


/* GetFileName() -
 *------------------------------------------------------------------*/
void CNtfsFolderName::GetFileName(CSpdCStringBase &scs, bool bIncludeExt) const 
{ 
   scs = m_scsFileName ; 
   if (bIncludeExt && !m_scsFileExt.empty())
   {
      scs += CSpdCStringF16(L".") ;
      scs += m_scsFileExt ;
   }
}


/* GetFullFileName() -
 * 19.jun.2006 - CR.6469; added option to request a file name using a locally mapped
 * driver letter vs. a UNC path. 
 * 22.jun.2006 - CR.????; Performance issues occuring with changes from CR.6469.
 *------------------------------------------------------------------*/
void CNtfsFolderName::GetFullFileName(CSpdCStringBase &scs, bool bIncludeExt, bool bDriverFormatIsDesired, bool bExtraLongName, bool bLocalMapFolder) const 
{
   //if (bLocalMapFolder)
   //{
   //   DWORD dwNtErr(DWORD(-1)) ;
   //   //
   //   // SLB local mapped drive requested.  
   //   // Only retrieve local map if extra long format is not specified, or if it is, the total path name + file name is
   //   // less than 256 bytes.
   //   //
   //   if (   
   //      !(bExtraLongName && (m_scsPathName.wcslen()+m_scsFileName.wcslen()+m_scsFileExt.wcslen()>=256))
   //      )
   //   {
   //      UncNameToLocalMapFolder(scs, dwNtErr, true) ;
   //   }
   //   if (dwNtErr!=ERROR_SUCCESS)
   //   {  //
   //      // folder name was not mapped successfully.
   //      // Use UNC path.
   //      //
   //      GetFullFolderName(scs, true, bDriverFormatIsDesired, bExtraLongName) ;
   //   }
   //}
   //else
   {
      GetFullFolderName(scs, true, bDriverFormatIsDesired, bExtraLongName) ;
   }
   scs += m_scsFileName ;
   if (bIncludeExt && !m_scsFileExt.empty())
   {
      scs += CSpdCStringF16(L".") ;
      scs += m_scsFileExt ;
   }
}


/* GetFullFolderName() -
 *------------------------------------------------------------------*/
void CNtfsFolderName::GetFullFolderName(CSpdCStringBase &scs, bool bAddEndingWhack, bool bDriverFormatIsDesired, bool bExtraLongNames) const
{
   //
   // clear output string before continuing.
   //
   scs.clear() ;

   if (m_bIsUncFormat)
   {  //
      // UNC format
      //
      if (!m_scsUncHostName.empty())
      {

         if (!bDriverFormatIsDesired)
         {  //
            if (bExtraLongNames)
            {  //
               //  SLB 14.mar.2006; Support for extra long file names.
               //  from MSDN: To specify such a UNC path(over 256 chars), use the "\\?\UNC\" prefix. For 
               //  example, "\\?\UNC\<server>\<share>". 
               //
               scs = CSpdCStringF16(L"\\\\?\\UNC\\") ;
            }
            else
            {
               scs = CSpdCStringF16(L"\\\\") ;
            }
            scs += m_scsUncHostName ;
            if (!m_scsUncShareName.empty())
            {
               scs += CSpdCStringF16(L"\\") ;
               scs += m_scsUncShareName ;
            }
         }
         else
         {  //
            // Kernel mode file names must begine with \?\
            //
            wchar_t wcs[256] ;

            _snwprintf(
               wcs,
               sizeof(wcs)/sizeof(wchar_t),
               L"\\??\\%c:",
               m_scsUncShareName.c_str()[0]
               ) ;
            wcs[sizeof(wcs)/sizeof(wchar_t) -1] = 0 ;
            scs = CSpdCStringG256(wcs) ;
         }
      }
   }
   else
   {  //
      // Drive letter format
      //
      if (!m_scsDriveLetter.empty())
      {
         scs = m_scsDriveLetter ;
         scs += CSpdCStringF16(L":") ;
      }
   }

   if (!scs.empty() && !m_scsPathName.empty())
   {
      scs += m_scsPathName ;
      if (!bAddEndingWhack)
      {  //
         // no ending '\\' desired...
         // erase it if this is not the root folder
         //
         if (m_scsPathName.wcslen() > 1)
         {  //
            // not the root folder, erase the ending whack.
            //
            scs.erase(scs.wcslen() -1) ;
         }
      }
   }
}



/* GetWriteAccessDesired() -
 *------------------------------------------------------------------*/
bool CNtfsFolderName::GetWriteAccessDesired() const
{
   return m_bWriteAccessDesired ;
}


/* SetDriveLetter() - Stores Drive Letter and ensures it is stored in the 
 * correct internal format.  Then sets UNC share name to the drive letter as
 * well.
 *
 * Internal format: Drive letter consists of only the drive letter.  It does 
 * not have a ':' or '\\' char. in it.
 *------------------------------------------------------------------*/
void CNtfsFolderName::SetDriveLetter(const CSpdCStringBase &scs) 
{  
   //const CSpdCString16 scsWhack(L"\\") ;
   size_t stPos(0) ;
   wchar_t wcsLetter[2] = {0,0} ;
   //
   // clear current Driver letter setting.
   //
   m_scsDriveLetter.clear() ;
   //
   // setup wcLetter
   //
   if (
      scs.wcslen() > 0 &&
      isalpha(scs.c_str()[0])
      )
   {
      wcsLetter[0] = towupper(scs.c_str()[0]) ;

      if (
         scs.wcslen()>1 && 
         scs.c_str()[1]==L':'
         )
      {
         m_scsDriveLetter.assign(wcsLetter) ;
         m_bIsUncFormat = false ;
      }
      else if (scs.wcslen()==1)
      {
         m_scsDriveLetter.assign(wcsLetter) ;
         m_bIsUncFormat = false ;
      }
      else
      {  //
         // problem: this is not a valid drive letter format.
         //
      }
   }
   //CSpdCString16 scsUncShareName(m_scsDriveLetter) ;
   //scsUncShareName += CSpdCString16(L":") ;
   //SetUncShareName(scsUncShareName) ;
}


/* SetUncHostName() - Stores UNC host name and ensures it is stored in the 
 * correct internal format.
 *
 * Internal format: Unc host name does not have beginning or ending '\\' chars.
 *------------------------------------------------------------------*/
void CNtfsFolderName::SetUncHostName(const CSpdCStringBase &scs) 
{  
   size_t stPos(0) ; 
   m_scsUncHostName = scs ;
   
   //
   // remove any preceding '\\' chars.
   //
   stPos = m_scsUncHostName.find_first_not_of(L'\\') ;
   if (stPos != CSpdCStringBase::npos)
   {
      m_scsUncHostName.erase(0, stPos) ;
   }

   //
   // ensure any trailing '\\' chars are removed
   //
   while (
      m_scsUncHostName.wcslen() &&
      m_scsUncHostName.c_str()[m_scsUncHostName.wcslen() -1] == L'\\'
      )
   {
      m_scsUncHostName.erase(m_scsUncHostName.wcslen() -1) ;
   }
   if (m_scsUncHostName.wcslen())
   {
      m_bIsUncFormat = true ;
   }
}


/* SetUncShareName() - Stores UNC Share name and ensures it is stored in the 
 * correct internal format.
 *
 * Internal format: Unc share name does not have beginning or ending '\\' chars.

 *------------------------------------------------------------------*/
void CNtfsFolderName::SetUncShareName(const CSpdCStringBase &scs) 
{ 
   size_t stPos(0) ; 
   m_scsUncShareName = scs ;
   
   //
   // remove any preceding '\\' chars.
   //
   stPos = m_scsUncShareName.find_first_not_of(L'\\') ;
   if (stPos != CSpdCStringBase::npos)
   {
      m_scsUncShareName.erase(0, stPos) ;
   }

   //
   // ensure any trailing '\\' chars are removed
   //
   while (
      m_scsUncShareName.wcslen() &&
      m_scsUncShareName.c_str()[m_scsUncShareName.wcslen() -1] == L'\\'
      )
   {
      m_scsUncShareName.erase(m_scsUncShareName.wcslen() -1) ;
   }
   if (m_scsUncShareName.wcslen())
   {
      m_bIsUncFormat = true ;
   }
}


/* SetPathName() - Stores pathname and ensures it is stored in the 
 * correct internal format.
 *
 * Internal format: Path name always begins and ends with a '\\' char.
 * If the the path is referencing the root folder, then there will only be
 * one '\\' char.
 *------------------------------------------------------------------*/
void CNtfsFolderName::SetPathName(const CSpdCStringBase &scs) 
{  //
   // TODO: need to decide on an internal format for storing this, then
   // this method can ensure that the format stored is correct.
   //
   const CSpdCStringF16 scsWhack(L"\\") ;
   m_scsPathName.clear() ;

   //
   // ensure pathname always begins with a \.
   //
   if (scs.empty() || scs[0] != L'\\')
   {
      m_scsPathName += scsWhack ;
   }

   m_scsPathName += scs ;

   //
   // ensure pathname always ends with a \
   //
   if (m_scsPathName[m_scsPathName.wcslen() - 1] != L'\\')
   {
      m_scsPathName += scsWhack ;
   }
}


/* SetFileName() - Stores file name and ensures it is stored in the 
 * correct internal format.
 *
 * Internal format: file name does not have any '\\' chars.  It never ends
 * with a '.' char
 * 
 *------------------------------------------------------------------*/
void CNtfsFolderName::SetFileName(const CSpdCStringBase &scsFileName, bool bHasExtension, bool bFixFileName) 
{  
   CSpdCStringG256 scsFixedName ;
   const CSpdCStringBase *pscs(bFixFileName ? &scsFixedName : &scsFileName) ;

   size_t stExtPos(0) ;

   if (bFixFileName)
      SpdUtils::FixFileNameNtfs(scsFileName, scsFixedName) ;
   
   stExtPos = pscs->find_last_of(L'.') ;
   if (!bHasExtension || stExtPos == CSpdCStringBase::npos)
   {  //
      // There is no extension for this filename.
      //
      m_scsFileName = *pscs ;
   }
   else
   {
      pscs->substr(m_scsFileName, 0, stExtPos) ;
      if (stExtPos+1 == pscs->wcslen() )
      {  //
         // '.' char was the last char in the file name.  There
         // is no extension for this file name.
         //
      }
      else
      {
         CSpdCStringG32 scsExt ;

         pscs->substr(scsExt, stExtPos+1, pscs->wcslen()-(stExtPos+1)) ;
         SetFileExt(scsExt) ;
      }
   }
}


/* SetFileExt() -
 *------------------------------------------------------------------*/
void CNtfsFolderName::SetFileExt(const CSpdCStringBase &scs) 
{  //
   // TODO: need to decide on an internal format for storing this, then
   // this method can ensure that the format stored is correct.
   //
   m_scsFileExt = scs ;
}


/* SetWriteAccessDesired() -
 *------------------------------------------------------------------*/
void CNtfsFolderName::SetWriteAccessDesired(bool bWrite) 
{
   m_bWriteAccessDesired = bWrite ;
}


/* CdRoot() -
 *------------------------------------------------------------------*/
void CNtfsFolderName::CdRoot() 
{  //
   // root folder is represented by an empty path string.
   //
   m_scsPathName = L"\\" ;
}

/* CdBack() -
 *------------------------------------------------------------------*/
void CNtfsFolderName::CdBack() 
{
   if (m_scsPathName.empty())
   {  //
      // problem: This is an error, should at least have a '\\' char.
      //
   }
   else if (m_scsPathName.wcslen() == 1)
   {  //
      // path is already at root, do nothing.
      //
   }
   else
   {
      size_t stPos(0) ;
      // 
      // erase the terminating '\\'
      //
      m_scsPathName.erase(m_scsPathName.wcslen()-1) ;
      
      //
      // find last '\\' char in the path
      //
      stPos = m_scsPathName.find_last_of(L'\\') ;
      if (stPos != CSpdCStringBase::npos)
      {  //
         // remove the last foldername, leave the preceding '\\'.
         // This becomes the ending '\\'.
         //
         m_scsPathName.erase(stPos+1) ;
      }
      else
      {  //
         // no '\\' found, so the path should represent the root folder now.
         //
         m_scsPathName = L"\\" ;
      }
   }
}



/* CdForward() -
 *------------------------------------------------------------------*/
void CNtfsFolderName::CdForward(const CSpdCStringBase &scsFolderName) 
{
   if (
      scsFolderName.find(L'\\') == CSpdCStringBase::npos &&
      scsFolderName.find(L':') == CSpdCStringBase::npos
      )
   {  //
      // foldername appears to be of valid format
      //
      m_scsPathName += scsFolderName ;
      m_scsPathName += CSpdCStringF16(L"\\") ;
   }
}

/* SetFullFileName() -
 *------------------------------------------------------------------*/
void CNtfsFolderName::SetFullFileName(const std::wstring &ws)
{
   CSpdCStringG512 scs(ws.c_str()) ;
   SetFullFileName(scs) ;
}

/* SetFullFileName() -
 * SLB 22.mar.2005 - implemented.
 *------------------------------------------------------------------*/
void CNtfsFolderName::SetFullFileName(const CSpdCStringBase &scsFullFolderName, bool bFixFileName) 
{  
   size_t stFilePos(0) ;  

   stFilePos = scsFullFolderName.find_last_of(L'\\') ;
   if (stFilePos == CSpdCStringBase::npos)
   {  //
      // No '\\' char found.  No folder information included, 
      // entire string is the file name
      //
      SetFileName(scsFullFolderName, true, bFixFileName) ;
   }
   else
   {
      CSpdCStringG512 scsTemp ;

      scsFullFolderName.substr(scsTemp, 0, stFilePos-0) ;
      SetFullFolderName(scsTemp) ;
      if (stFilePos+1 == scsFullFolderName.wcslen())
      {  //
         // '\\' char is the last char of the folder name.  There
         // is no file name.
         //
      }
      else
      {
         scsFullFolderName.substr(scsTemp, stFilePos+1, scsFullFolderName.wcslen()-(stFilePos+1)) ;
         SetFileName(scsTemp, true) ;
      }
   }

}

/* SetFullFolderName() -
 *------------------------------------------------------------------*/
void CNtfsFolderName::SetFullFolderName(const std::wstring &ws)
{
   CSpdCStringG512 scs(ws.c_str()) ;
   SetFullFolderName(scs) ;
}

/* SetFullFolderName() -
 *------------------------------------------------------------------*/
void CNtfsFolderName::SetFullFolderName(const CSpdCStringBase &scsFullFolderName) 
{  
   const CSpdCStringF16 scsDot(L".") ;
   //
   // first determine if the foldername is in mapped or UNC format.
   //
   if (scsFullFolderName.wcslen() >= 3)
   {
      CSpdCStringG512 scsSubStr ; // 
      size_t stStartPos(0) ;
      size_t stEndPos(0) ;
      size_t stLen(CSpdCStringBase::npos) ;
      //
      // Retrieve Server Name
      //
      if (scsFullFolderName[0] != L'\\' || scsFullFolderName[1] != L'\\')
      {  //
         // no server name specified, presume it is local_host
         //
         SetUncHostName(scsDot) ;
      }
      else 
      {  //
         // determine if extra long format was specified.
         //
         stStartPos = scsFullFolderName.find(L"\\\\?\\UNC\\") ;
         if (stStartPos!=CSpdCStringBase::npos)
         {  //
            // extra long format was specified.  skip past the extralong header.
            //
            stStartPos = 8 ;
         }
         else
         {  //
            // Extra long format was not specified. skip past the double slash.
            //
            stStartPos = 2 ;
         }
         //
         // Look for next slash.  It signals  the end of the host name.
         //
         stEndPos = scsFullFolderName.find(L'\\', stStartPos) ;
         if (stEndPos == CSpdCStringBase::npos)
            stLen = stEndPos ;
         else
            stLen = stEndPos - stStartPos ;
         
         scsFullFolderName.substr(scsSubStr, stStartPos, stLen) ;
         SetUncHostName(scsSubStr) ;
         //
         // Setup start pos for looking for the share name.
         // skip the slash that terminated the host name.
         //
         stStartPos = stEndPos + 1 ;
      }
      //
      // Retrieve the share name.
      //
      if (
         stEndPos!=CSpdCStringBase::npos && 
         stStartPos<scsFullFolderName.wcslen()
         )
      {  
         //
         // there is more data to scan.
         //
         stEndPos = scsFullFolderName.find(L'\\', stStartPos) ;
         if (stEndPos == CSpdCStringBase::npos)
            stLen = stEndPos ;
         else
            stLen = stEndPos - stStartPos ;
         scsFullFolderName.substr(scsSubStr, stStartPos, stLen) ;
         //
         // These two methods will determine whether this is a UNC path or not.
         // (setUncShareName() & SetDriveLetter()
         // SLB 02.nov.2006 CR.7271; Sometimes a share name can be one letter and looks like
         // a driver letter.  Need to look for the colon at the end before calling 
         // SetDriverLetter() in this case.  So modified to check for a valid length & the 
         // colon char.  Otherwise, calls SetUncShareName().
         //
         if (
            scsSubStr.wcslen() == 2 &&
            scsSubStr.c_str()[1] == L':'
            )
         {
            SetDriveLetter(scsSubStr) ;
         }
         else
         {
            SetUncShareName(scsSubStr) ;
         }

         stStartPos = stEndPos + 1 ;
         //
         // retrieve the path name.
         //
         if (
            stEndPos!=CSpdCStringBase::npos && 
            stStartPos<scsFullFolderName.wcslen()
            )
         {  
            scsFullFolderName.substr(scsSubStr, stStartPos, CSpdCStringBase::npos) ;
            SetPathName(scsSubStr) ;
         }
         else
         {  //
            // SLB 10.aug.2006 CR.6844; If no path is specified, then treat it as the root folder.
            //
            SetPathName(CSpdCStringF16(L"\\")) ;
         }
      }
   }
}

/* ChangeDirectory() -
 *------------------------------------------------------------------*/
bool CNtfsFolderName::ChangeDirectory(const CSpdCStringBase &scs) 
{  
   bool bRetVal(false) ;
   size_t stStartPos(0) ;
   size_t stEndPos(CSpdCStringBase::npos) ;
   size_t stLen(0) ;
   bool bNotDone(true) ;
   CSpdCStringG256 scsShortFolder ;
   //
   // TODO: implement
   //
   if (scs[stStartPos] == L'\\')
   {
      CdRoot() ;
      stStartPos++ ;
   }

   while (bNotDone && stStartPos != scs.wcslen() )
   {
      stEndPos = scs.find( L'\\', stStartPos) ;
      //
      // calculate the length of this substr.
      //
      if (stEndPos==CSpdCStringBase::npos)
      {  //
         // no '\\' char found.
         //
         stLen = scs.wcslen() - stStartPos ;
         bNotDone = false ;
      }
      else
      {
         stLen = stEndPos - stStartPos ;
      }
      
      scs.substr(scsShortFolder, stStartPos, stLen) ;
      stStartPos += stLen + 1 ;

      //
      // folder token has been obtained.  Take action accordingly.
      //
      if (scsShortFolder.empty())
      {  //
         // this is either illegal or the end of the string has been reached.
         //
         bNotDone = false ;
      }
      else
      {
         // 
         // check this sub-folder name for special folders ('.' and '..') 
         //
         if (scsShortFolder == CSpdCStringF16(L".."))
         {  //
            // go back one folder (remove one sub-folder from the current folder)
            //
            CdBack() ;
         }
         else if (scsShortFolder[0] == L'.')
         {  //
            // stay in this folder (do not change the current folder)
            //
         }
         else
         {  //
            // append this folder to current folder
            //
            CdForward(scsShortFolder) ;
         }
      }
   }
   return bRetVal ;
}


/* IsFullFileNameOk() -
 * SLB 25.mar.2006 Added.
 *------------------------------------------------------------------*/
CNtfsFolderName::TFolderNameProblem CNtfsFolderName::IsFullFileNameOk() const 
{
   TFolderNameProblem fnpRetVal(fnpOk) ;
   
   if (m_scsFileExt.wcslen() + m_scsFileName.wcslen() + 1 > 256)
   {  //
      // NTFS does not support file names > 256 chars
      //
      fnpRetVal = fnpTooLong ;
   }

   return fnpRetVal ;
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
 *------------------------------------------------------------------*/
bool CNtfsFolderName::DoesFolderExist(DWORD &dwNtErr) const 
{
   bool bRetVal(false) ;
   DWORD dwAccessRights(FILE_LIST_DIRECTORY|FILE_READ_ATTRIBUTES|FILE_READ_DATA) ;
   
   dwNtErr = ERROR_SUCCESS ;
   if (m_bWriteAccessDesired)
   {  //
      // SLB 15.jan.2007 CR.7666; removed FILE_DELETE_CHILD, added DELETE and FILE_WRITE_ATTRIBUTES
      //
      dwAccessRights |= FILE_ADD_SUBDIRECTORY | FILE_ADD_FILE | DELETE | FILE_WRITE_DATA | FILE_WRITE_ATTRIBUTES;
   }

   bRetVal = IsFolderAccessOk(dwAccessRights, dwNtErr) ;
   return bRetVal ;
}


/* DoesFileExist() -
 * SLB 22.jun.2005 Fixed up access rights for writing\reading.  
 * SLB 17.jun.2006 Modified to request local drive mapping, if available, when file
 * name is constructed.  This is done for performance on Win XP, SP2 systems.
 *------------------------------------------------------------------*/
bool CNtfsFolderName::DoesFileExist(DWORD &dwNtErr) const
{
   bool bRetVal(false) ;
   CSpdCStringG512 scsFileName ;
   HANDLE hFile(INVALID_HANDLE_VALUE) ;
   DWORD dwAccessRights(GENERIC_READ) ; 
   
   dwNtErr = 0 ;

   if (m_bWriteAccessDesired)
   {
      dwAccessRights |= GENERIC_WRITE ;
   }
   //
   // Use long file name format ("\\?\UNC\.....")
   //
   GetFullFileName(scsFileName, true, false, true, true) ;
   hFile = CreateFileW( 
      scsFileName.c_str(),
      dwAccessRights, 
      0,
      NULL,
      OPEN_EXISTING,
      0, 
      NULL
      ) ;

   if (hFile==INVALID_HANDLE_VALUE)
   {  
      dwNtErr = GetLastError() ;
      bRetVal = false ;
   }
   else
   {  //
      // opened the folder successfully!
      // cleanup.
      //
      CloseHandle(hFile) ;
      bRetVal = true ;
   }
   return bRetVal ;
}


/* IsFolderAccessOk() -
 *
 * SLB 31.aug.2007 CR.6135; Modified to use the folder's security descriptor to 
 * check the access rights. This mechanism takes into account the folder's owner
 * and inherited settings which CreateFile() seems to ignore.
 *------------------------------------------------------------------*/
bool CNtfsFolderName::IsFolderAccessOk(DWORD dwAccessRights, DWORD &dwNtErr) const 
{
   bool bRetVal(false) ;
   CSpdCStringG512 scsFolderName ;

   dwNtErr = 0 ;
   GetFullFolderName(scsFolderName, false, false, true) ;
   //
   // CR.6135; Retrieve folder's Security Descriptor.
   //
   PSECURITY_DESCRIPTOR psdFolder(NULL) ;
   //
   // Note: get everything except for the SACL. The SACL is only used to determine auditing settings.
   //
   dwNtErr = GetNamedSecurityInfo(
      (wchar_t *)scsFolderName.c_str(),
      SE_FILE_OBJECT,
      OWNER_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION, 
      NULL,
      NULL,
      NULL,
      NULL,
      &psdFolder
      ) ;
   if (dwNtErr != ERROR_SUCCESS)
   {  //
      // problem: GetNamedSecurityInfo() failed.
      //
   }
   else
   {  //
      // CR.6135; Need current process's SID
      //  i) First retrieve process token
      //  ii) Retrieve SID from token
      //  iii) close process token
      //
      BOOL bRes(FALSE) ;
      HANDLE hProcessToken(INVALID_HANDLE_VALUE) ;
      //
      // CR.6135; Retrieve current process's token.
      //
      bRes = OpenProcessToken(
         GetCurrentProcess(),
         TOKEN_QUERY,
         &hProcessToken
         ) ;
      if (!bRes)
      {  //
         // problem: OpenProcessToken() failed.
         //
         dwNtErr = GetLastError() ;
      }
      else
      {
         BYTE ucTokenInfoBuf[2048] ;
         DWORD dwNeededBufLen(0) ;
         //
         // CR.6135; Retrieve the SID for the current process's identity.
         //
         bRes = GetTokenInformation(
            hProcessToken,
            TokenUser, 
            (LPVOID)&ucTokenInfoBuf,
            sizeof(ucTokenInfoBuf),
            &dwNeededBufLen
            ) ;
         if (!bRes)
         {  //
            // problem: GetTokenInformation() failed.
            //
            dwNtErr = GetLastError() ;
         }
         else
         {  //
            // CR.6135: retrieve effective permissions.
            //
            HRESULT hr(S_OK) ;
            Win32Sec::SecurityDescriptorPtr sdFolder(psdFolder) ;
            DWORD dwGrantedAccessRights(0) ;
            TOKEN_USER *ptoBuf((TOKEN_USER *)&ucTokenInfoBuf) ;
            Win32Sec::SIDPtr sid((const SID *)ptoBuf->User.Sid, NULL) ;
            const wchar_t *pwcsSid(sid.Sid()) ;

            hr = sdFolder.GetEffectivePermissions(
               sid, 
               dwAccessRights,
               &dwGrantedAccessRights, 
               false 
               ) ;
            if (FAILED(hr))
            {  //
               // problem: GetEffectivePermissions() failed.
               //
            }
            else
            {  //
               // Successfully retrieved permissions.
               // Check to see if retrieved permissions match desired permissions.
               //
               if (dwGrantedAccessRights != dwAccessRights)
               {  //
                  // problem: Granted access rights don't match up with requested access rights.
                  //
               }
               else
               {  //
                  // Success. 
                  //
                  bRetVal = true ; 
               }
            }
         }
         //
         // clean up handle allocation from OpenProcessToken().
         //
         CloseHandle(hProcessToken) ;
      }
      //
      // Clean up memory allocation from GetNamedSecurityInfo().
      //
      LocalFree(psdFolder) ;
   }
  
   return bRetVal ;
}

/* IsSystemFolder() -
 *------------------------------------------------------------------*/
bool CNtfsFolderName::IsSystemFolder() const 
{
   bool bRetVal(false) ;

   bRetVal = IsSystemFolder(GetPathName().c_str()) ;
   return bRetVal ;
}


/* IsSystemFolder() -
 * SLB 15.mar.2006 - CR.6006 removed root folder because Clients need to be able to
 * validate Share Paths.
 *------------------------------------------------------------------*/
bool CNtfsFolderName::IsSystemFolder(const wchar_t *pwcsFolderName) const 
{
   bool bRetVal(false) ;
   int i(0) ;
   wchar_t *pwcsSystemFolders[] = 
   {
      //L"\\",
      L"\\WINDOWS\\",
      L"\\WINDOWS\\SYSTEM\\",
      L"\\WINDOWS\\SYSTEM32\\",
      L"\\WINDOWS\\SYSTEM32\\DRIVERS\\",
      NULL
   } ;
   
   for (
      i=0; 
      pwcsSystemFolders[i] && wcsicmp(pwcsSystemFolders[i], pwcsFolderName) ;
      i++
      )
   {
   }

   bRetVal = pwcsSystemFolders[i]!=NULL ? true : false ;
   return bRetVal ;
}

   
/* clear() -
 *------------------------------------------------------------------*/
void CNtfsFolderName::clear()
{
   m_scsDriveLetter.clear() ;
   m_scsFileExt.clear() ;
   m_scsFileName.clear() ;
   m_scsPathName.clear() ;
   m_scsUncHostName.clear() ;
   m_scsUncShareName.clear() ;
}

/* MapFolderToLocalUncName() -
 *------------------------------------------------------------------*/
bool CNtfsFolderName::MapFolderToLocalUncName(CSpdCStringBase &scs, DWORD &dwNtErr, bool bAddEndingWhack) const
{
   PSHARE_INFO_502 BufPtr(NULL), p(NULL);				//ptrs to structure returned from NetShareEnum
   NET_API_STATUS res;									//return value from NetShareEnum
   DWORD dwEntriesRead(0),dwTotalEntries(0),dwResume_Handle(0), i(0); 
   bool bRetVal(true);									

   CSpdCStringG512 scsUncPathName;

   //GetFullFolderName(scsFolderName, false);
   scs.clear();	

   if(!IsUncFormat())
   {	
      //
		// validation. 
      // SLB - Driver letter validation is performed else where (initialization of class).  IsUncFormat() returns 
      // true if this is unc formatted and drive letter string is empty if it is invalid.
      //
		//if ( toupper(scsFolderName.c_str()[0]) < 'A' || toupper(scsFolderName.c_str()[0]) > 'Z' )			
		//	bRetVal = false; 
		//if ( scsFolderName.c_str()[1] != ':' || scsFolderName.c_str()[2] != '\\' ) 
		//	bRetVal = false; 
      GetUncHostName() ;
      GetDriveLetter() ;
      if (GetDriveLetter().empty())
      {  //
         // problem: driver letter was not initialized.
         //
         bRetVal = false ;
      }
      else
      {
         CSpdCStringF16 scsDriveLetter(GetDriveLetter()) ;
         WCHAR wcComputerName[MAX_COMPUTERNAME_LENGTH + 1]; 
		   DWORD dwNameLen = sizeof(wcComputerName);

		   if(GetComputerNameW(wcComputerName, &dwNameLen))   			
		   {
			   scsUncPathName += CSpdCStringF16(L"\\\\"); 
			   scsUncPathName += CSpdCStringG256(wcComputerName);
			   //
			   // Call the NetShareEnum function; specify level 502.
			   //
			   size_t stLen(0);	
			   for(res=ERROR_MORE_DATA;res == ERROR_MORE_DATA;)
			   {
				   res = NetShareEnum (L"", 502, (LPBYTE *) &BufPtr, -1, &dwEntriesRead, &dwTotalEntries, &dwResume_Handle);
				   if(res == ERROR_SUCCESS || res == ERROR_MORE_DATA)
				   {
					   p=BufPtr;
					   for(i=1;i<=dwEntriesRead;i++)
					   {
						   stLen = wcslen((WCHAR*)p->shi502_path);
						   if(
                        scsDriveLetter.c_str()[0] == (p->shi502_path)[0] &&  //compare drive letters			   
					         stLen == 3												    //[0] : Drive Name [1] : colon [2] : slash 
						   )													 
						   {
							   scsUncPathName += CSpdCStringF16(L"\\");
							   scsUncPathName += CSpdCStringG256(p->shi502_netname);
							   break;
						   }
						   p++;
					   }
					   NetApiBufferFree(BufPtr);
				   }
				   else 
				   {
					   dwNtErr = GetLastError();
					   bRetVal = false;
				   }
			   }
			   if(bRetVal != false)
			   {	
				   //scsUncPathName += CSpdCString16(L"\\");
				   scsUncPathName += m_scsPathName;
               if (!bAddEndingWhack)
               {  //
                  // no ending '\\' desired...
                  // erase it if this is not the root folder
                  //
                  if (m_scsPathName.wcslen() > 1)
                  {  //
                     // not the root folder, erase the ending whack.
                     //
                     scsUncPathName.erase(scsUncPathName.wcslen() -1) ;
                  }
               }
				   scs = scsUncPathName;
			   }
		   }
		   else
		   {
			   dwNtErr = GetLastError();
			   bRetVal = false;
		   }
      }
   }
   else
   {  //
      // not a problem: The format is already UNC.  No mapping is required.
      // Just retrieve the full folder name and return it.
      //
		GetFullFolderName(scs, false) ;
   }
   return bRetVal;
}

/* UncNameToLocalMapFolder() -
 *------------------------------------------------------------------*/
bool CNtfsFolderName::UncNameToLocalMapFolder(CSpdCStringBase &scs, DWORD &dwNtErr, bool bAddEndingWhack) const
{
   PSHARE_INFO_502 BufPtr(NULL), p(NULL); //ptrs to structure returned from NetShareEnum
   NET_API_STATUS res; //return value from NetShareEnum
   DWORD dwEntriesRead(0),dwTotalEntries(0),dwResume_Handle(0), i(0); 
   bool bRetVal(true);									

   CSpdCStringG512 scsPathName;

   scs.clear();	

   if (IsUncFormat())
   {	
      if (GetUncShareName().empty())
      {  //
         // problem: UNC share name was not initialized.
         //
         bRetVal = false;
      }
      else
      {
         CSpdCStringG16 scsUncShareName(GetUncShareName());
         WCHAR wcComputerName[MAX_COMPUTERNAME_LENGTH + 1]; 
         DWORD dwNameLen = sizeof(wcComputerName);

         if (GetComputerNameW(wcComputerName, &dwNameLen))   			
         {
            // Error out if m_scsUncHostName doesn't match wcComputerName.
            if (wcsicmp(m_scsUncHostName.c_str(), wcComputerName))
               bRetVal = false;
            else
            {  //
               // Call the NetShareEnum function; specify level 502.
               //
               for (res=ERROR_MORE_DATA;res == ERROR_MORE_DATA;)
               {
                  res = NetShareEnum(L"", 502, (LPBYTE *) &BufPtr, -1, &dwEntriesRead, &dwTotalEntries, &dwResume_Handle);
                  if (res == ERROR_SUCCESS || res == ERROR_MORE_DATA)
                  {
                     p=BufPtr;
                     for (i=1;i<=dwEntriesRead;i++)
                     {
                        if (!wcsicmp(scsUncShareName.c_str(), p->shi502_netname)) // compare share names
                        {
                           scsPathName += CSpdCStringG256(p->shi502_path);
                           
                           // Erase ending whack, if present, since m_scsPathName starts with whack.
                           if (scsPathName.wcslen() && scsPathName[scsPathName.wcslen()-1] == L'\\')
                              scsPathName.erase(scsPathName.wcslen()-1);
                           break;
                        }
                        p++;
                     }
                     NetApiBufferFree(BufPtr);
                  }
                  else 
                  {
                     dwNtErr = GetLastError();
                     bRetVal = false;
                  }
               }
               if (bRetVal != false)
               {	
                  scsPathName += m_scsPathName;
                  if (!bAddEndingWhack)
                  {  //
                     // no ending '\\' desired...
                     // erase it if this is not the root folder
                     //
                     if (m_scsPathName.wcslen() > 1)
                     {  //
                        // not the root folder, erase the ending whack.
                        //
                        scsPathName.erase(scsPathName.wcslen() -1);
                     }
                  }
                  scs = scsPathName;
               }
            }
         }
         else
         {
            dwNtErr = GetLastError();
            bRetVal = false;
         }
      }
   }
   else
   {  //
      // not a problem: The format is already non-UNC.  No conversion is required.
      // Just retrieve the full folder name and return it.
      //
      GetFullFolderName(scs, bAddEndingWhack);
   }
   return bRetVal;
}

/* CreateFolder() -
 *------------------------------------------------------------------*/
bool CNtfsFolderName::CreateFolder(DWORD &dwNtErr, const LPSECURITY_ATTRIBUTES psaSecAttribs) const 
{
   bool bRetVal(false) ;
   CSpdCStringG512 scsFolderName ;

   dwNtErr = 0 ;
   GetFullFolderName(scsFolderName, false) ;
	
   dwNtErr = SHCreateDirectoryExW(NULL, scsFolderName.c_str(), psaSecAttribs) ;
   if (dwNtErr!=ERROR_SUCCESS)
   {  //
      // failed to create the folder
      //
	   if (
         (dwNtErr==ERROR_FILE_NOT_FOUND ) ||  //  2
		   (dwNtErr==ERROR_ALREADY_EXISTS) || //  183
		   (dwNtErr==ERROR_FILE_EXISTS) 
         )//ERROR_FILE_EXISTS //this is ok
		   bRetVal = true;
	  else
		   bRetVal = false ;
   }
   else
   {
      bRetVal = true ;
   }

   return bRetVal ;
}


/* DeleteFolder() -
 *------------------------------------------------------------------*/
bool CNtfsFolderName::DeleteFolder(DWORD &dwNtErr) const 
{
   bool bRetVal(false) ;
   CSpdCStringG512 scsFolderName ;

   dwNtErr = 0 ;
   GetFullFolderName(scsFolderName, false) ;

   if (!(RemoveDirectoryW(scsFolderName.c_str()))) 
   {  //
      // failed to remove the folder
      //
      dwNtErr = GetLastError() ;
      bRetVal = false ;
   }
   else
   {
      bRetVal = true ;
   }

   return bRetVal ;
}


/* TestFolderAccessRight() - This method checks the specified folder to see if at has rights 
 * to the specified AccessRight.  Return value is a Win32 error code.  0 = access right is 
 * granted.
 *
 * SLB 15.jan.2007 CR.7666; Added.
 *------------------------------------------------------------------*/
DWORD CNtfsFolderName::TestFolderAccessRight(const wchar_t *pwcsFolderName, DWORD dwAccessRight)
{
   DWORD dwNtErr(ERROR_SUCCESS) ;
   HANDLE hFolder(INVALID_HANDLE_VALUE) ;
   wchar_t wcsMsg[512] ;

   hFolder = CreateFileW(
      pwcsFolderName,
      dwAccessRight,
      FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
      NULL,
      OPEN_EXISTING,
      FILE_FLAG_BACKUP_SEMANTICS, // required for opening directories
      NULL
      ) ;
   if (hFolder == INVALID_HANDLE_VALUE)
   {  
      dwNtErr = GetLastError() ;
      if (s_bIsExtendedLoggingOn)
      {
         std::wstring wsWin32Error ;
         SpdUtils::GetWin32ErrorMessage(wsWin32Error, dwNtErr) ;
         _snwprintf(
            wcsMsg, sizeof(wcsMsg)/sizeof(wchar_t),
            L"\nSPDE Test: CNtfsFolderName::TestFolderAccessRight(%08X) - disabled: %s.",
            dwAccessRight,
            GetAccessFlagDescription(dwAccessRight).c_str()
            ) ;
         TERMINATE_WCS(wcsMsg) ;
         OutputDebugStringW(wcsMsg) ;
      }
   }
   else
   {  //
      // opened the folder successfully!
      // cleanup.
      //
      CloseHandle(hFolder) ;
      if (s_bIsExtendedLoggingOn)
      {
         _snwprintf(
            wcsMsg, sizeof(wcsMsg)/sizeof(wchar_t),
            L"\nSPDE Test: CNtfsFolderName::TestFolderAccessRight(%08X) - ENABLED : %s.",
            dwAccessRight,
            GetAccessFlagDescription(dwAccessRight).c_str()
            ) ;
         TERMINATE_WCS(wcsMsg) ;
         OutputDebugStringW(wcsMsg) ;
      }
   }
   return dwNtErr ;
}

/* BuildAccessFlagDescriptionMap() - Initializes the s_AccessFlagDescriptions map which
 * is used for generating user readable messages.
 *
 * SLB 15.jan.2007 CR.7666; Added.
 * SLB 31.aug.2007 CR.6135; Removed some duplicate entries added to array.
 *------------------------------------------------------------------*/
void CNtfsFolderName::BuildAccessFlagDescriptionMap()
{  
   if (s_AccessFlagDescriptions.empty())
   {
      s_AccessFlagDescriptions[FILE_LIST_DIRECTORY] = std::wstring(L"FILE_LIST_DIRECTORY | FILE_READ_DATA") ;
      s_AccessFlagDescriptions[FILE_WRITE_DATA] = std::wstring(L"FILE_WRITE_DATA  | FILE_ADD_FILE") ;
      s_AccessFlagDescriptions[FILE_APPEND_DATA] = std::wstring(L"FILE_APPEND_DATA  | FILE_ADD_SUBDIRECTORY |  FILE_CREATE_PIPE_INSTANCE") ;
      s_AccessFlagDescriptions[FILE_READ_EA] = std::wstring(L"FILE_READ_EA") ;
      s_AccessFlagDescriptions[FILE_WRITE_EA] = std::wstring(L"FILE_WRITE_EA") ;
      s_AccessFlagDescriptions[FILE_EXECUTE] = std::wstring(L"FILE_EXECUTE | TRAVERSE") ;
      s_AccessFlagDescriptions[FILE_DELETE_CHILD] = std::wstring(L"FILE_DELETE_CHILD") ;
      s_AccessFlagDescriptions[FILE_READ_ATTRIBUTES] = std::wstring(L"FILE_READ_ATTRIBUTES") ;
      s_AccessFlagDescriptions[FILE_WRITE_ATTRIBUTES] = std::wstring(L"FILE_WRITE_ATTRIBUTES") ;
      s_AccessFlagDescriptions[DELETE] = std::wstring(L"DELETE") ;
      s_AccessFlagDescriptions[READ_CONTROL] = std::wstring(L"READ_CONTROL") ;
      s_AccessFlagDescriptions[WRITE_DAC] = std::wstring(L"WRITE_DAC") ;
      s_AccessFlagDescriptions[WRITE_OWNER] = std::wstring(L"WRITE_OWNER") ;
      s_AccessFlagDescriptions[SYNCHRONIZE] = std::wstring(L"SYNCHRONIZE") ;
      s_AccessFlagDescriptions[ACCESS_SYSTEM_SECURITY] = std::wstring(L"ACCESS_SYSTEM_SECURITY") ;
      s_AccessFlagDescriptions[MAXIMUM_ALLOWED] = std::wstring(L"MAXIMUM_ALLOWED") ;
      s_AccessFlagDescriptions[GENERIC_ALL] = std::wstring(L"GENERIC_ALL") ;
      s_AccessFlagDescriptions[GENERIC_EXECUTE] = std::wstring(L"GENERIC_EXECUTE") ;
      s_AccessFlagDescriptions[GENERIC_WRITE] = std::wstring(L"GENERIC_WRITE") ;
      s_AccessFlagDescriptions[GENERIC_READ] = std::wstring(L"GENERIC_READ") ;
   }
}

/* GetAccessFlagDescription() - retrieves the user readable discription for an access flag 
 * entry.  Note, only one bit flag should be set per call to this method.
 *
 * SLB 15.jan.2007 CR.7666; Added.
 *------------------------------------------------------------------*/
std::wstring CNtfsFolderName::GetAccessFlagDescription(DWORD dwAccessFlag)
{
   std::wstring wsRetVal(L"NO DEFINITION") ;
   BuildAccessFlagDescriptionMap() ;
   if (s_AccessFlagDescriptions.find(dwAccessFlag) == s_AccessFlagDescriptions.end())
   {
   }
   else
   {
      wsRetVal = s_AccessFlagDescriptions[dwAccessFlag] ;
   }
   return wsRetVal ;
}

