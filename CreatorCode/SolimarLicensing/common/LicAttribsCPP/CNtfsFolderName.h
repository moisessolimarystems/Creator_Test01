// CSpdMediumManagerNTFS.h : TODO: Add file description here.
//

#pragma once
#ifndef UNICODE
#define UNICODE
#endif

#include "SpdStreamable.h"
#include "SpdUtils.h"
//#include <string>
//*******************************************************************
//
// class CNtfsFolderName
//
// description: 
// 
// Functionality/Responsiblity:
//    Class which encapsulates UNC naming and Drive Letter mapping folder 
//    names.
//
//*******************************************************************
using namespace SpdStreamable ;
class SPDUTILS_IMP_EXP CNtfsFolderName
{
public:
   typedef enum TFolderNameProblem
   {
      fnpOk,
      fnpTooLong,
      fnpIllegalChar,
      fnpInvalid
   } ;
   typedef std::map<DWORD,std::wstring> TAccessFlagDescriptionMap ;
   typedef TAccessFlagDescriptionMap::iterator TItAccessBitDescriptionMap ;

private: 
   bool m_bIsUncFormat ;
   bool m_bWriteAccessDesired ;
   CSpdCStringG16 m_scsDriveLetter ;
   CSpdCStringG128 m_scsUncHostName ;
   CSpdCStringG128 m_scsUncShareName ;
   CSpdCStringG256 m_scsPathName ;
   CSpdCStringG256 m_scsFileName ;
   CSpdCStringG64 m_scsFileExt ;
   static TAccessFlagDescriptionMap s_AccessFlagDescriptions ;
   static bool s_bIsExtendedLoggingOn ;
public:
   CNtfsFolderName() ;
   CNtfsFolderName(
      const CNtfsFolderName &cpy
      ) ;
   CNtfsFolderName(
      const CSpdCStringBase &scsFullFolderName,
      bool bWriteAccessDesired = true
      ) ;
   CNtfsFolderName(  
      const CSpdCStringBase &scsDriveLetter,
      const CSpdCStringBase &scsPathName,
      const CSpdCStringBase &scsFileName,
      const CSpdCStringBase &scsFileExt,
      bool bWriteAccessDesired = true
      ) ;
   CNtfsFolderName(
      const CSpdCStringBase &wsUncHostName,
      const CSpdCStringBase &wsUncShareName,
      const CSpdCStringBase &wsUncPathName,
      const CSpdCStringBase &wsUncFileName,
      const CSpdCStringBase &wsUncFileExt,
      bool bWriteAccessDesired = true
      ) ;
   //CNtfsFolderName(
   //   const std::wstring &wsFullFolderName,
   //   bool bWriteAccessDesired = true
   //   ) ;
   CNtfsFolderName(  
      const std::wstring &wsDriveLetter,
      const std::wstring &wsPathName,
      const std::wstring &wsFileName,
      const std::wstring &wsFileExt,
      bool bWriteAccessDesired = true
      ) ;
   CNtfsFolderName(
      const std::wstring &wsUncHostName,
      const std::wstring &wsUncShareName,
      const std::wstring &wsUncPathName,
      const std::wstring &wsUncFileName,
      const std::wstring &wsUncFileExt,
      bool bWriteAccessDesired = true
      ) ;
   CNtfsFolderName & operator =(const CNtfsFolderName &cpy) ;
   CNtfsFolderName & operator =(const CSpdCStringBase &wsFolderName) ;

   bool IsUncFormat() const { return m_bIsUncFormat ; }

   const CSpdCStringBase & GetDriveLetter() const { return m_scsDriveLetter ; }
   const CSpdCStringBase & GetUncHostName() const { return m_scsUncHostName ; }
   const CSpdCStringBase & GetUncShareName() const { return m_scsUncShareName ; }
   const CSpdCStringBase & GetPathName() const { return m_scsPathName ; }
   const CSpdCStringBase & GetFileExt() const { return m_scsFileExt ; }
   
   void GetFileName(CSpdCStringBase &scs, bool bIncludeExt = false) const ;
   CSpdCStringG256 GetFileName(bool bIncludeExt) const { CSpdCStringG256 scs ; GetFileName(scs, bIncludeExt) ; return scs ; }
   void GetFullFileName(std::wstring &ws, bool bIncludeExt = true, bool bDriverFormatIsDesired = false, bool bExtraLongName = false, bool bLocalMapFolder = false) const ;
   void GetFullFileName(CSpdCStringBase &scs, bool bIncludeExt = true, bool bDriverFormatIsDesired = false, bool bExtraLongName = false, bool bLocalMapFolder = false) const ;
   CSpdCStringG512 GetFullFileName(bool bIncludeExt = true, bool bDriverFormatIsDesired = false, bool bExtraLongName = false, bool bLocalMapFolder = false) const 
   { 
      CSpdCStringG512 scs ; 
      GetFullFileName(scs, bIncludeExt, bDriverFormatIsDesired, bExtraLongName, bLocalMapFolder) ; 
      return scs ; 
   }
   bool GetWriteAccessDesired() const ;

   void GetFullFolderName(std::wstring &ws, bool bAddEndingWhack, bool bDriverFormatIsDesired = false, bool bExtraLongName = false) const 
   {
      CSpdCStringG256 scs ;
      GetFullFolderName(scs, bAddEndingWhack, bDriverFormatIsDesired) ;
      ws = scs.c_str() ;
   }
   void GetFullFolderName(CSpdCStringBase &scs, bool bAddEndingWhack, bool bDriverFormatIsDesired = false, bool bExtraLongName=false) const ;
   CSpdCStringG512 GetFullFolderName(bool bAddEndingWhack, bool bDriverFormatIsDesired = false, bool bExtraLongName = false) const
   {
      CSpdCStringG512 scs ;
      GetFullFolderName(scs, bAddEndingWhack, bDriverFormatIsDesired, bExtraLongName) ;
      return scs ;
   }

   void SetDriveLetter(const CSpdCStringBase &scs) ;
   void SetUncHostName(const CSpdCStringBase &scs) ;
   void SetUncShareName(const CSpdCStringBase &scs) ;
   void SetPathName(const CSpdCStringBase &scs) ;
   void SetFileName(const CSpdCStringBase &scs, bool bHasExtension, bool bFixFileName=true) ;
   void SetFileExt(const CSpdCStringBase &scs) ;
   void SetFullFileName(const std::wstring &scs) ;
   void SetFullFileName(const CSpdCStringBase &scs, bool bFixFileName=true) ;
   void SetFullFolderName(const std::wstring &scs) ;
   void SetFullFolderName(const CSpdCStringBase &scs) ;
   void SetWriteAccessDesired(bool bWrite) ;
   bool ChangeDirectory(const CSpdCStringBase &scs) ;
   TFolderNameProblem IsFullFileNameOk() const ;
   bool DoesFolderExist(DWORD &dwNtErr) const ;
   bool DoesFileExist(DWORD &dwNtErr) const ;
   bool IsFolderAccessOk(DWORD dwAccessRights, DWORD &dwNtErr) const ;
   bool IsSystemFolder() const ;
   bool IsSystemFolder(const wchar_t *pwcsFolderName) const ;
   bool CreateFolder(DWORD &dwNtErr, const LPSECURITY_ATTRIBUTES psaSecAttribs) const ;
   bool DeleteFolder(DWORD &dwNtErr) const ;
   void clear() ;
   bool MapFolderToLocalUncName(CSpdCStringBase &scs, DWORD &dwNtErr, bool bAddEndingWhack=false) const ;
   bool MapFolderToLocalUncName(std::wstring &ws, DWORD &dwNtErr, bool bAddEndingWhack=false) const
   {
      bool bRetVal(false) ;
      CSpdCStringG256 scs ;

      bRetVal = MapFolderToLocalUncName(scs, dwNtErr) ;
      ws = scs.c_str() ;
      return bRetVal ;
   }
   bool UncNameToLocalMapFolder(CSpdCStringBase &scs, DWORD &dwNtErr, bool bAddEndingWhack=false) const ;
   bool UncNameToLocalMapFolder(std::wstring &ws, DWORD &dwNtErr, bool bAddEndingWhack=false) const
   {
      bool bRetVal(false) ;
      CSpdCStringG256 scs ;

      bRetVal = UncNameToLocalMapFolder(scs, dwNtErr) ;
      ws = scs.c_str() ;
      return bRetVal ;
   }
   static void EnableExtendedLogging() { s_bIsExtendedLoggingOn = true ; }
   static void DisableExtendedLogging() { s_bIsExtendedLoggingOn = false ; }
private:
   void CdRoot() ;
   void CdBack() ;
   void CdForward(const CSpdCStringBase &scsFolderName) ;
   static DWORD TestFolderAccessRight(const wchar_t *pwcsFolderName, DWORD dwAccessRight) ;
   static std::wstring GetAccessFlagDescription(DWORD dwAccessFlag) ;
   static void BuildAccessFlagDescriptionMap() ;
} ;

