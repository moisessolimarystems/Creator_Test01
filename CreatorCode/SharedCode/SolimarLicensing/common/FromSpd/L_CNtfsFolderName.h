//
// CNtfsFolderName.h : Contains CNtfsFolderName class definition.
//

#pragma once

#ifndef _CNtfsFolderName_h																				// CR.32662.V04; added.
#define _CNtfsFolderName_h																				// CR.32662.V04; added.


#pragma warning(disable: 4091)  /* 'typedef' ignored when no variable is declared. */ // CR.24131; added.

#ifndef UNICODE
#define UNICODE
#endif

#include "L_SpdStreamable4.h"
#include "L_SpdUtils.h"
#include <vector>                                                                   // CR.26999.Item55; added.

using namespace SpdUtils;                                                           // CR.28242; added.

/*
 * class CSmartHandle
 *
 * This class wraps a Windows style handle. When the destructor is called, it closes the file.
 * 
 * SLB 16.jan.2015 CR.17862; Created.
 * SLB 13.sep.2018 CR.18557; Added new methods to help with needs for ANSI file names: GetFullFolderNameA() and GetFullFileNameA()
 */
class CSmartHandle
{
private:
   HANDLE m_h; 
public:
   CSmartHandle() : m_h(INVALID_HANDLE_VALUE) 
   {
   }
   CSmartHandle(HANDLE h) : m_h(h) 
   {
   }
   void Close()
   {
      if (m_h != INVALID_HANDLE_VALUE) CloseHandle(m_h); 
      m_h = INVALID_HANDLE_VALUE;
   }
   virtual ~CSmartHandle()
   {
      Close();
   }
   HANDLE & operator = (HANDLE h)
   {
      m_h = h; 
      return m_h; 
   }
   operator HANDLE () const
   {
      return m_h; 
   }
}; 

/*
 * class CSmartFindHandle
 *
 * This class wraps a Windows FindFiles() handle. When the destructor is called, it closes the file.
 * 
 * SLB 02.jun.2015 CR.19093; Created.
 */
class CSmartFindHandle
{
private:
   HANDLE m_h; 
public:
   CSmartFindHandle() : m_h(INVALID_HANDLE_VALUE) 
   {
   }
   CSmartFindHandle(HANDLE h) : m_h(h) 
   {
   }
   virtual ~CSmartFindHandle()
   {
      if (m_h!=INVALID_HANDLE_VALUE) FindClose(m_h); 
   }
   HANDLE & operator = (HANDLE h)
   {
      m_h = h; 
      return m_h; 
   }
   operator HANDLE () const
   {
      return m_h; 
   }
}; 


using namespace SpdStreamable; 


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
// SLB 15.sep.2010 CR.13616; Added GetFolderUniqueId(). This helps resolve multiple folder mappings 
// pointing to the same physical folder.
// SLB 30.may.2013 CR.16991; Added mutex to make sure access to the static security descriptor is 
// protected. This fixes problem where Access Violation was apparently happening in the TCPIP input 
// module when creating a CSpdPath sub-stream. It may resolve other random issues that were 
// never tracked down.
// SLB 28.jun.2013 CR.17124.Item1; Added IsSystemFile() methods. Needed for SPDE's File resource manager.
// SLB 08.apr.2014 CR.6522; Added method which will cause the object to convert its storage of a UNC path to 
// a local path form.
// SLB 28.mar.2017 CR.23304; Added new methods which wrap WIN32 calls for Moving a file and setting its Attributes.
// These methods will retry the operation if requested.
// SLB 01.apr.2017 CR.23305; Added new param to DoesFileExist() that allows check to not specify 
// any access rights. 
// Added new members and method which initializes them which caches mapped local path for a share.
// SLB 28.apr.2017 CR.22874; Made Ntfs_xxx() methods const. They do not modify the object.
// SLB 28.mar.2019 CR.26999.Item100; Added method which will delete all files and subfolders of 
// the current folder.
// SLB 05.mar.2020 CR.29058; Major change in unc host name functionality. Now, by default, will convert from
// a NetBIOS host name to a fully qualified machine name. 
// CR.11156; Need to declare the CSecurityContext class for compiling purposes.
// CR.29096; Added method to convert machine names into FQDN format.
// SLB 29.may.2020 CR.29294; Attempts to reduce handle leaks. Converted Mutex HANDLEs to use CSmartMutex.
// SLB 03.jun.2020 CR.29412; Enhanced capability for detecting System Folders. Now initializes a static array of all
// Admin shares on the system, which is queried against.
// Updated how statics collections get initialized. 
// SLB 05.feb.2021 CR.29859.V01; minor changes.
// SLB 05.jan.2023 CR.32082; Fairly significant re-work of all public "substream" 
// methods to additionally support file systems which do not have Alternate Data 
// Stream (ADS) support. Added some new public methods as well which are to be 
// used, instead of the Ntfs_xxx() methods, for true Substream support.
// IsFolderNameSet() deprecated and replaced by IsFolderNameInitialized().
// Added new functionality to track if Filename and Foldername values have been
// initialized (indicating that the object is prepared for calls to certain methods 
// which require them to be valid). 
// SLB 26.jan.2023 CR.32082.V00.01; Added non-static version of DoesShareSupportAds().
// SLB 16.feb.2023 CR.32082.V01; Added some methods to help with creation of the
// substream folder for NoAds support.
// SLB 08.mar.2023 CR.32260; Fix for CdXxxx() methods being to stringent and requiring 
// the Fullfoldername to be initialized. Some code, like the task scheduler code, 
// uses the class to manipulate path-like strings without ever specifying drive/hostname
// info. Modified these methods so this is not required anymore, but they will 
// throw exceptions if certain formatting issues are encountered.
namespace Win32Sec
{
   class CSecurityContext;  
} 

class SSILIB1_EXPORT CNtfsFolderName                                                // CR.13870; Now uses SSILIB1_EXPORT to define linkage.
{
public:
   typedef enum TFolderNameProblem
   {
      fnpOk,
      fnpTooLong,
      fnpIllegalChar,
      fnpInvalid
   }; 
   typedef std::map<DWORD, CSpdCString> TAccessFlagDescriptionMap; 
   typedef TAccessFlagDescriptionMap::const_iterator TItcAccessBitDescriptionMap; 

   typedef std::vector<CSpdCString> TNameList;                                      // CR.26999.Item55; added.
   typedef TNameList::iterator TItNameList;                                         // CR.26999.Item55; added.
   typedef TNameList::const_iterator TItcNameList;                                  // CR.26999.Item55; added.

   typedef std::set<CSpdCString> TCStringSet;                                       // CR.29412; added.
   typedef TCStringSet::const_iterator TItcCStringSet;                              // CR.29412; added.

private: 
   bool m_bIsFolderInitialized;                                                     // CR.32082; added.
   bool m_bIsFileInitialized;                                                       // CR.32082; added.
   bool m_bIsUncFormat; 
   bool m_bWriteAccessDesired; 
   CSpdCString m_wsDriveLetter; 
   CSpdCString m_wsUncHostName; 
   CSpdCString m_wsUncShareName; 
   CSpdCString m_wsPathName; 
   CSpdCString m_wsFileName; 
   CSpdCString m_wsFileExt; 
   CSpdCString m_wsMappedLocalPathName;                                             // CR.23305; added.
   bool m_bMappedLocalPathInitialized;                                              // CR.23305; added.

   static bool s_bIsExtendedLoggingOn; 
   static Win32Sec::CSecurityContext s_scSecurityContext;                           // CR.11156
   static CSmartMutex s_hScMutex;                                                   // CR.29294; modified. CR.16991; added.

   static bool s_bStaticsAreInitialized;                                            // CR.29412; added.
   static TAccessFlagDescriptionMap s_AccessFlagDescriptions;
   static TCStringSet s_SpecialShares;                                              // CR.29412; added.

   // ADS supported?
   // TShareSupportsAds std::map<sharename, bSupportsAds>
   typedef std::map<CSpdCString, bool> TShareSupportsAds;                           // CR.32082; added.
   typedef TShareSupportsAds::iterator TItShareSupportsAds;                         // CR.32082; added.
   
   // Collection which tracks which shares support ADS and which do not.
   static TShareSupportsAds s_AdsSupportFolders;                                    // CR.32082; added.
public:
   CNtfsFolderName(); 
   CNtfsFolderName(
      const CNtfsFolderName &cpy); 
   CNtfsFolderName(
      const CSpdCString &wsFullFolderName,
      bool bWriteAccessDesired = true); 
   CNtfsFolderName(  
      const CSpdCString &wsDriveLetter,
      const CSpdCString &wsPathName,
      const CSpdCString &wsFileName,
      const CSpdCString &wsFileExt,
      bool bWriteAccessDesired = true); 
   CNtfsFolderName(
      const CSpdCString &wsUncHostName,
      const CSpdCString &wsUncShareName,
      const CSpdCString &wsUncPathName,
      const CSpdCString &wsUncFileName,
      const CSpdCString &wsUncFileExt,
      bool bWriteAccessDesired = true); 
   CNtfsFolderName(  
      const std::wstring &wsDriveLetter,
      const std::wstring &wsPathName,
      const std::wstring &wsFileName,
      const std::wstring &wsFileExt,
      bool bWriteAccessDesired = true); 
   CNtfsFolderName(
      const std::wstring &wsUncHostName,
      const std::wstring &wsUncShareName,
      const std::wstring &wsUncPathName,
      const std::wstring &wsUncFileName,
      const std::wstring &wsUncFileExt,
      bool bWriteAccessDesired = true); 
   CNtfsFolderName & operator =(const CNtfsFolderName &cpy); 
   CNtfsFolderName & operator =(const CSpdCString &wsFolderName); 

   bool IsUncFormat() const { return m_bIsUncFormat; }

   bool IsFolderNameInitialized() const { return m_bIsFolderInitialized; }          // CR.32082; added.
   void IsFolderNameInitialized_Check(                                              // CR.32082; added.
      const wchar_t* pwcsFunc) const;
   bool IsFileNameInitialized() const { return m_bIsFileInitialized; }              // CR.32082; added.
   void IsFileNameInitialized_Check(                                                // CR.32082; added.
      const wchar_t* pwcsFunc) const;
   void IsPathNameSet_Check(                                                        // CR.32260; added.
      const wchar_t* pwcsFunc) const;


   const CSpdCString & GetDriveLetter() const { return m_wsDriveLetter; }
   const CSpdCString & GetUncHostName() const { return m_wsUncHostName; }
   const CSpdCString & GetUncShareName() const { return m_wsUncShareName; }
   const CSpdCString & GetPathName() const { return m_wsPathName; }
   const CSpdCString & GetFileExt() const { return m_wsFileExt; }
   
   void GetFileName(
      CSpdCString &ws, 
      bool bIncludeExt = false) const; 
   CSpdCString GetFileName(
      bool bIncludeExt) const 
   { 
      CSpdCString ws;  
      GetFileName(ws, bIncludeExt);  
      return ws;  
   }

   /* GetFullFileName() methods.
    *
    * SLB 11.sep.2018 CR.18557; Added GetFullFileNameA() to return a muli-byte string version
    * of the file name.
    -----------------------------------------------------------------------------------------*/
   void GetFullFileName(
      std::wstring &ws, 
      bool bIncludeExt = true, 
      bool bDriverFormatIsDesired = false, 
      bool bExtraLongName = false, 
      bool bLocalMapFolder = false) const; 
   void GetFullFileName(
      CSpdCString &ws, 
      bool bIncludeExt = true, 
      bool bDriverFormatIsDesired = false, 
      bool bExtraLongName = false,
      bool bLocalMapFolder = false) const; 
   CSpdCString GetFullFileName(
      bool bIncludeExt = true, 
      bool bDriverFormatIsDesired = false, 
      bool bExtraLongName = false, 
      bool bLocalMapFolder = false) const 
   { 
      CSpdCString ws;  
      GetFullFileName(ws, bIncludeExt, bDriverFormatIsDesired, bExtraLongName, bLocalMapFolder);  
      return ws;  
   }
   std::string GetFullFileNameA(                                                    // CR.18557; created.
      bool bIncludeExt = true,
      bool bDriverFormatIsDesired = false,
      bool bExtraLongName = false,
      bool bLocalMapFolder = false) const
   {
      CSpdCString ws; 
      GetFullFileName(ws, bIncludeExt, bDriverFormatIsDesired, bExtraLongName); 

      errno_t etErr;
      char szFileName[1024] = {0}; 
      size_t stCharCount(0); 
      etErr = wcstombs_s(   
         &stCharCount,       
         szFileName, 
         sizeof(szFileName), 
         ws.c_str(),
         _TRUNCATE); 
      if (etErr != 0) ThrowHr(HRESULT_FROM_WIN32(etErr), __WFUNCTION__, L"Problem converting folder name into multi-byte string.");

      return szFileName; 
   }

   CSpdCString GetFolderRoot(                                                       // CR.32082; added.
      bool bAddEndingWhack,
      bool bDriverFormatIsDesired = false,
      bool bExtraLongName = false) const;

   /* GetFullFolderName() methods.
    *
    * SLB 11.sep.2018 CR.18557; Added GetFullFolderNameA() to return a muli-byte string version
    * of the folder name.
    -----------------------------------------------------------------------------------------*/
   void GetFullFolderName(
      std::wstring &ws, 
      bool bAddEndingWhack, 
      bool bDriverFormatIsDesired = false, 
      bool bExtraLongName = false) const 
   {
      CSpdCString wsLocal; 
      GetFullFolderName(wsLocal, bAddEndingWhack, bDriverFormatIsDesired, bExtraLongName); 
      ws = wsLocal.c_str(); 
   }
   void GetFullFolderName(
      CSpdCString &ws, 
      bool bAddEndingWhack, 
      bool bDriverFormatIsDesired = false, 
      bool bExtraLongName = false) const; 
   CSpdCString GetFullFolderName(
      bool bAddEndingWhack, 
      bool bDriverFormatIsDesired = false, 
      bool bExtraLongName = false) const
   {
      CSpdCString ws; 
      GetFullFolderName(ws, bAddEndingWhack, bDriverFormatIsDesired, bExtraLongName); 
      return ws; 
   }
   std::string GetFullFolderNameA(                                                  // CR.18557; created.
      bool bAddEndingWhack, 
      bool bDriverFormatIsDesired = false, 
      bool bExtraLongName = false) const
   {
      CSpdCString ws; 
      GetFullFolderName(ws, bAddEndingWhack, bDriverFormatIsDesired, bExtraLongName); 

      errno_t etErr;
      char szFolder[1024] = {0}; 
      size_t stCharCount(0); 
      etErr = wcstombs_s(   
         &stCharCount,       
         szFolder, 
         sizeof(szFolder), 
         ws.c_str(),
         _TRUNCATE); 
      if (etErr != 0) ThrowHr(HRESULT_FROM_WIN32(etErr), __WFUNCTION__, L"Problem converting folder name into multi-byte string.");

      return szFolder; 
   }

   void SetDriveLetter(
      const CSpdCString &ws); 
   void SetUncHostName(
      const CSpdCString &ws,
      bool bMakeFullyQualified = true);                                             // CR.29058; added.
   void SetUncShareName(
      const CSpdCString &ws); 
   void SetPathName(
      const CSpdCString &ws); 
   void SetFileName(
      const CSpdCString &ws, 
      bool bHasExtension, 
      bool bFixFileName = true,
		const CSpdCString& wsFixNameReplaceChars = L"_");										// CR.32574; added.
   void SetFileExt(
      const CSpdCString &ws); 
   void SetFullFileName(
      const CSpdCString &ws, 
      bool bFixFileName=true); 
   void SetFullFolderName(
      const CSpdCString &ws); 
   void SetWriteAccessDesired(
      bool bWrite); 
   void ChangeDirectory(                                                            // CR.26999.Item73; modified.
      const CSpdCString &ws); 
   bool ConvertToLocalPath(
      DWORD &dwNtErr);                                                              // CR.6522; added.
   bool ConvertToLocalPath()                                                        // CR.23305; added.
   {
      DWORD dwNtErr; 
      return ConvertToLocalPath(dwNtErr); 
   }
   TFolderNameProblem IsFullFileNameOk() const; 
   bool DoesFolderExist(
      DWORD &dwNtErr,
      bool bPerformAccessCheck=true) const;                                         // CR.26999.Item55; added.
   void FolderExists_Check(                                                         // CR.32082; modified.
      const CSpdCString &wsFunc,                                                    // CR.26999.Item55; added.
      const CSpdCString &wsFolderDescription = L"") const;                          // CR.26999.Item3; added.
   void FolderNoExist_Check(                                                         // CR.26999.Item55; added.
      const CSpdCString &wsFunc,
      const CSpdCString &wsFolderDescription = L"") const; 
   static bool AreFolderLocationsEqual(
      const CSpdCString &wsFolder1,
      const CSpdCString &wsFolder2); 
   bool GetFolderUniqueId(                                                          // CR.13616; added.
      std::wstring &wsUniqueId, 
      DWORD &dwNtErr) const;                                                        // CR.13616.V01; added.
   bool DoesFileExist(
      DWORD &dwNtErr, 
      bool bNoAccessCheck = false) const                                            // CR.23305; modified.CR.11810;
   {
      if (bNoAccessCheck) return DoesFileExistSpecifyAccess(false, false, dwNtErr); // CR.23305; added.
      else return DoesFileExistSpecifyAccess(true, m_bWriteAccessDesired, dwNtErr); 
   }
   void FileExists_Check(                                                           // CR.32082; modified.
      const CSpdCString &wsFunc,                                                    // CR.26999.Item55; added.
      bool bNoAccessCheck = false,
      const CSpdCString &wsFileDescription = L"") const;                            // CR.26999.Item3; added.
   void FileNoExists_Check(                                                         // CR.32082; modified. CR.26999.Item55; added.
      const CSpdCString &wsFunc,
      const CSpdCString &wsFileDescription = L"") const; 
   bool DoesFileExistSpecifyAccess(                                                 // CR.11810;
      bool bCheckForReadAccess, 
      bool bCheckForWriteAccess, 
      DWORD &dwNtErr) const;  
   bool IsFolderAccessOk(
      DWORD dwAccessRights, 
      DWORD &dwNtErr) const; 
   bool IsSystemFolder() const; 
   bool IsSystemFile() const;                                                       // CR.17124.Item1; added.
   bool CreateFolder(
      DWORD &dwNtErr, 
      const LPSECURITY_ATTRIBUTES psaSecAttribs) const; 
   bool DeleteFolder(
      DWORD &dwNtErr) const; 
   void DeleteFilesAndFolders(                                                      // CR.26999.Item100; added.
      bool bIncludeThisFolder = false) const;
   bool GetWriteAccessDesired() const; 
   QUADWORD GetFileSize(                                                            // CR.29234.V02; added.
      HANDLE hFile,
      const CSpdCString &wsFileName = L"?") const;
   QUADWORD GetFileSize() const;                                                    // CR.13842; added.
   QUADWORD GetFileSubstreamSize(                                                   // CR.26999.Item55; added.
      const CSpdCString wsSubstream) const;  
   size_t GetFolderFileCount() const;                                               // CR.28247; added.
   void clear(); 
   void ClearFullFolder(
      bool bFileNameAlso = false);
   void ClearFileName(
      bool bExtAlso = false);

   static void InitializeStatics();                                                 // CR.29412; added.
   static void InitializeNetShareStatics();                                         // CR.29412; added.
   bool MapFolderToLocalUncName(
      CSpdCString &ws, 
      DWORD &dwNtErr, 
      bool bAddEndingWhack = false) const; 
   bool MapFolderToLocalUncName(
      std::wstring &ws, 
      DWORD &dwNtErr, 
      bool bAddEndingWhack = false) const
   {
      CSpdCString wsLocal; 
      if (!MapFolderToLocalUncName(wsLocal, dwNtErr)) return false; 

      ws = wsLocal.c_str(); 
      return true; 
   }
   bool UncNameToLocalMapFolder(
      CSpdCString &ws, 
      DWORD &dwNtErr, 
      bool bAddEndingWhack = false) const; 
   bool UncNameToLocalMapFolder(
      std::wstring &ws, 
      DWORD &dwNtErr, 
      bool bAddEndingWhack = false) const
   {
      bool bRetVal(false); 
      CSpdCString wsLocal; 

      bRetVal = UncNameToLocalMapFolder(wsLocal, dwNtErr); 
      ws = wsLocal.c_str(); 
      return bRetVal; 
   }

   // Higher level methods which maintain substreams; regardless of whether ADS
   // is supported or not.
   bool DoesShareSupportAds() const;                                                // CR.32082.V00.01; added.
   void AddNoAdsFolder(                                                             // CR.32082.V01.01; added.
      const CSpdCString &wsFolderDescription) const; 
   void MoveFileWithSS(                                                             // CR.32082; created.
      const CSpdCString& wsDestFileName,
      DWORD dwMoveFlags,
      const CSpdCString& wsSrcFileDescription,
      const CSpdCString& wsDestFileDescription,
      DWORD dwMaxRetryCount = 0,
      DWORD dwTimeBetweenRetriesMS = 100) const;
   void CopyFileWithSS(                                                             // CR.32082; created.
      const CSpdCString& wsDestFileName,
      bool bFailIfExists,
      const CSpdCString& wsSrcFileDescription,
      const CSpdCString& wsDestFileDescription,
      DWORD dwMaxRetryCount = 0,
      DWORD dwTimeBetweenRetriesMS = 100) const;
   void DeleteFileWithSS(                                                           // CR.32082; created.
      const CSpdCString& wsFileDescription,
      bool bNoExceptions = true,
      DWORD dwMaxRetryCount = 0,
      DWORD dwTimeBetweenRetriesMS = 100) const;
   HANDLE CreateSubstream(
      const CSpdCString& wsStreamName,
      const CSpdCString& wsFileDescription,
      const CSpdCString& wsStreamDescription) const;
   HANDLE OpenSubstream(
      const CSpdCString& wsStreamName,
      DWORD dwNtfsDesiredAccess,
      DWORD dwNtfsShareMode,
      bool bNoExceptionForNoExist,                                                  // For CR.29324.V02 support;
      const CSpdCString& wsFileDescription,
      const CSpdCString& wsStreamDescription) const;
   TNameList GetFileSubstreams(                                                     // CR.26999.Item55; added.
      const CSpdCString& wsFileDescription) const;
   size_t GetFileSubstreamCount(                                                    // CR.26999.Item55; added.
      const CSpdCString& wsFileDescription) const;
   CSpdCString GetFileSubstreamName(                                                // CR.26999.Item55; added.
      size_t stStreamIndex,
      const CSpdCString& wsFileDescription) const;
   CSpdCString GetFileFullSubstreamName(                                            // CR.32082; added.
      const CSpdCString& wsSubstream,
      const CSpdCString& wsFileDescription) const;
   bool DoesSubstreamExist(                                                         // CR.27336; added.
      const CSpdCString& wsSubstream) const;
   void DeleteFileSubstream(                                                        // CR.32082; added.
      const CSpdCString& wsSubstream,
      const CSpdCString& wsFileDescription,
      const CSpdCString& wsStreamDescription,
      bool bNoExceptionOnError = false) const;
   void CopyFileSubstream(                                                          // CR.32082; added.
      const CSpdCString &wsSubStream,
      const CNtfsFolderName &fnDestFile,
      bool bFailIfExists,
      const CSpdCString& wsSrcFileDescription,
      const CSpdCString& wsDestFileDescription,
      const CSpdCString &wsStreamDescription,
      DWORD dwMaxRetryCount = 0,
      DWORD dwTimeBetweenRetriesMS = 100) const;
   void MoveFileSubstream(                                                          // CR.32082; added.
      const CSpdCString& wsSubStream,
      const CNtfsFolderName& fnDestFile,
      bool bFailIfExists,
      const CSpdCString& wsSrcFileDescription,
      const CSpdCString& wsDestFileDescription,
      const CSpdCString& wsStreamDescription,
      DWORD dwMaxRetryCount = 0,
      DWORD dwTimeBetweenRetriesMS = 100) const;

   // Straight calls to NTFS via WIN32 methods.
   bool Ntfs_SetFileAttributes(                                                     // CR.23304; added.
      DWORD dwFileAttributes, 
      bool bUseFolderName = false,                                                  // CR.26999.Item55; added.
      DWORD dwMaxRetryCount = 0, 
      DWORD dwTimeBetweenRetriesMS = 100) const;                                    // CR.22874; modified.
   bool Ntfs_GetFileAttributes(                                                     // CR.26999.Item55; added.
      DWORD &dwFileAttributes, 
      bool bUseFolderName = false, 
      DWORD dwMaxRetryCount = 0, 
      DWORD dwTimeBetweenRetriesMS = 100) const; 
	bool Ntfs_MoveFile(                                                              // CR.23304; added.
		const CSpdCString& wsDestFileName,
		DWORD dwMoveFlags,
		DWORD dwMaxRetryCount = 0,
		DWORD dwTimeBetweenRetriesMS = 100) const;                                    // CR.22874; modified.
	bool Ntfs_MoveFolder(                                                            // CR.32662.V06; added.
      const CSpdCString &wsDestFolderName,
      DWORD dwMoveFlags,
      DWORD dwMaxRetryCount = 0,
      DWORD dwTimeBetweenRetriesMS = 100) const; 
   typedef enum TNtfsMoveFileFlags                                                  // CR.29859.V01; added typedef.
   {
      mff_REPLACE_EXISTING =      0x00000001,
      mff_COPY_ALLOWED =          0x00000002,
      mff_DELAY_UNTIL_REBOOT =    0x00000004,
      mff_WRITE_THROUGH =         0x00000008,
      mff_CREATE_HARDLINK =       0x00000010,
      mff_FAIL_IF_NOT_TRACKABLE = 0x00000020,
   };

   bool Ntfs_CopyFile(
      const CSpdCString &wsDestFileName,
      bool bFailIfExists,
      DWORD dwMaxRetryCount = 0,
      DWORD dwTimeBetweenRetriesMS = 100) const; 
   bool Ntfs_DeleteFile(
      DWORD dwMaxRetryCount = 0,
      DWORD dwTimeBetweenRetriesMS = 100) const;
   void Ntfs_CopyFolder(
      const CNtfsFolderName &fnDestFolder,
      bool bOverWriteExisting,
      DWORD dwMaxRetryCount = 0,
      DWORD dwTimeBetweenRetriesMS = 100) const; 
   void Ntfs_DeleteFiles() const;
   bool Ntfs_DeleteFolder(
      DWORD dwMaxRetryCount = 0,
      DWORD dwTimeBetweenRetriesMS = 100) const;
   static void EnableExtendedLogging(); 
   static void DisableExtendedLogging(); 
   static CSpdCString GetMacineNameFQ(                                              // CR.29096; added.
      const CSpdCString &wsMachineName);

private:

   CSpdCString GetSubstreamFileNameAds(                                             // CR.32082; added.
      const CSpdCString& wsStreamName) const;
   HANDLE CreateSubstreamAds(                                                       // CR.32082; added.
      const CSpdCString& wsStreamName,
      const CSpdCString& wsFileDescription,
      const CSpdCString& wsStreamDescription) const;
   HANDLE OpenSubstreamAds(                                                         // CR.32082; added.
      const CSpdCString& wsStreamName,
      DWORD dwNtfsDesiredAccess,
      DWORD dwNtfsShareMode,
      bool bNoExceptionForNoExist,                                                  // For CR.29324.V02 support;
      const CSpdCString& wsFileDescription,
      const CSpdCString& wsStreamDescription) const;
   void DeleteFileSubstreamAds(                                                     // CR.32082; added.
      const CSpdCString& wsStreamName,
      const CSpdCString& wsFileDescription,
      const CSpdCString& wsStreamDescription,
      bool bNoExceptionOnError = false) const;
   TNameList GetFileSubstreamsAds(                                                  // CR.32082; added.
      const CSpdCString& wsFileDescription) const;

public:
   CNtfsFolderName GetSubstreamFolderNoAds() const;                                 // CR.32082.V01; added.

private:
   CSpdCString GetSubstreamFileNameNoAds(                                           // CR.32082; added.
      const CSpdCString& wsStreamName) const;
   CSpdCString GetSubstreamFromFileNameNoAds(                                       // CR.32082; added.
      const CSpdCString& wsFileName) const;
   HANDLE CreateSubstreamNoAds(                                                     // CR.32082; added.
      const CSpdCString& wsStreamName,
      const CSpdCString& wsFileDescription,
      const CSpdCString& wsStreamDescription) const;
   HANDLE OpenSubstreamNoAds(                                                       // CR.32082; added.
      const CSpdCString &wsStreamName,
      DWORD dwNtfsDesiredAccess,
      DWORD dwNtfsShareMode,
      bool bNoExceptionForNoExist,                                                  // For CR.29324.V02 support;
      const CSpdCString& wsFileDescription,
      const CSpdCString& wsStreamDescription) const;
   void DeleteFileSubstreamNoAds(                                                   // CR.32082; added.
      const CSpdCString& wsStreamName,
      const CSpdCString& wsFileDescription,
      const CSpdCString& wsStreamDescription,
      bool bNoExceptionOnError = false) const;
   void CreateSubstreamFolderNoAds(                                                 // CR.32082.V01; added.
      const CSpdCString& wsFolderDescription) const;

   TNameList GetFileSubstreamsNoAds(                                                // CR.32082; added.
      const CSpdCString& wsFileDescription) const;

   bool IsSystemFolder(
      const CSpdCString &wsFolderName) const;
   bool IsSystemFile(                                                               // CR.17124.Item1; added.
      const CSpdCString &wsFileName) const;
   void CdRoot();
   void CdBack(); 
   void CdForward(
      const CSpdCString &wsFolderName); 
   void InitializeMappedShare();                                                    // CR.23305; added.
   static DWORD TestFolderAccessRight(
      const wchar_t *pwcsFolderName, 
      DWORD dwAccessRight); 
   static const CSpdCString & GetAccessFlagDescription(                             // CR.29412; modified.
      DWORD dwAccessFlag); 
   static void BuildAccessFlagDescriptionMap(); 

   static bool DoesShareSupportAds(                                                 // CR.32082; added.
      const CSpdCString& wsShareName);
}; 

/*
 * class CSmartFileAttribs
 *
 * This class wraps a File SetAttributes() call so that the Attribs changes get restored. When the destructor is called, the attribs are restored.
 * Note: it does not report errors for restoring Attributes. This is ignored because the file may have been moved or renamed since instantiation 
 * of this object.
 * 
 * SLB 16.dec.2014 CR.17862; created.
 * SLB 05.apr.2017 CR.23348; minor performance enhancement. Changed to use CSpdCSTring vs. std::wstring param.
 * SLB 18.jan.2023 CR.32082; changes in "protected" constructors to eliminate compiler warning msg 26495.
 */
class CSmartFileAttribs
{
private:
   DWORD m_dwSavedAttribs; 
   DWORD m_dwNtErr; 
   CNtfsFolderName m_fnFile;                                                        // CR.23348; modified.
private:

   // Do not allow default constructor.
   CSmartFileAttribs() : m_dwNtErr(0), m_dwSavedAttribs(0) {}                       // CR.32082; modified.
public:
   CSmartFileAttribs(
      const CNtfsFolderName &fnFile,                                                // CR.23348; modified.
      DWORD dwNewAttribs
      ) : 
      m_fnFile(fnFile), 
      m_dwSavedAttribs(0), 
      m_dwNtErr(ERROR_SUCCESS)
   {
      bool bRes;
      bRes = m_fnFile.Ntfs_GetFileAttributes(m_dwSavedAttribs);
      if (!bRes) { m_dwNtErr = GetLastError(); return; }

      bRes = m_fnFile.Ntfs_SetFileAttributes(dwNewAttribs);
      if (!bRes) m_dwNtErr = GetLastError();  
   }
   virtual ~CSmartFileAttribs()
   {
      if (WasError()) return; 
      m_fnFile.Ntfs_SetFileAttributes(m_dwSavedAttribs);
   }
   bool WasError() const { return GetNtError() != ERROR_SUCCESS; }
   DWORD GetNtError() const { return m_dwNtErr; }
}; 

#endif

