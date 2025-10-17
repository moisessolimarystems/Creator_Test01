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
//		StringMatch()                                                                       *
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
#ifndef NTDDI_VERSION
#define NTDDI_VERSION NTDDI_WINXP	//Version Constants for Kernel-Mode Drivers
#endif
#define _WIN32_WINNT  (0x0501)
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#define __SPDUTILS_CPP
#include "SpdUtils.h"
//#include "CNtfsFolderName.h"
#include <stdio.h>
//#include <dir.h>  // MAXDRIVE
#include <io.h> // _rtl_open(), _rtl_close(), _rtl_read(), access() ...
#include <fcntl.h> // O_RDONLY
#include <share.h>   // SH_DENYRW
#include <sys\stat.h> // S_IWRITE
#include <ctype.h> // isalnum()
#include <direct.h> // _getdrive()
#include <locale> // std::locale
#include <Shlwapi.h>	// For PathFileExists


/* VerifyFolder() - Verifies that directory string exists.
 *               Added by SLB, 9-May-93 for detection of network drives
 *               which opendir() cannot do.
 * SLB 17.nov.2005 - removed call IsFolderAccessOk().  I believe this is un-necessary and
 * it was not getting passed meaningful info anyhow.
------------------------------------------------------------------------*/
//SPDUTILS_IMP_EXP bool __stdcall SpdUtils::VerifyFolder(const std::wstring &wsDir, bool bWriteRights)
//{
//   CNtfsFolderName fnFolder(CSpdCStringF512(wsDir.c_str()), bWriteRights) ;
//   bool bRetVal(false);
//   //DWORD dwAccessRights( 0 ) ;
//   DWORD dwNtErr(0) ;
//
//   if (!fnFolder.DoesFolderExist(dwNtErr))
//   {  //
//      // this is not a valid folder name.
//      //
//      bRetVal = false ;
//   }
//   //else if (!fnFolder.IsFolderAccessOk(dwAccessRights, dwNtErr))
//   //{
//   //   //
//   //   // access is denied.
//   //   //
//   //   bRetVal = false ;
//   //}
//   else
//   {  //
//      // Folder name is valid and desired access is allowed.
//      //
//      bRetVal = true ;
//   }
//   return bRetVal ;
//} 

/* VerifyFolderA() - Verifies that directory string exists.
 * Multi-byte version.
------------------------------------------------------------------------*/
//SPDUTILS_IMP_EXP bool __stdcall SpdUtils::VerifyFolderA(const char *szDir, bool bWriteRights)
//{
//wchar_t wcFolder[_MAX_PATH];
//	_snwprintf( wcFolder,
//               sizeof(wcFolder)/sizeof(wchar_t),
//               L"%S",
//               szDir
//             );
//   return VerifyFolder(std::wstring(wcFolder), bWriteRights);
//} // end of VerifyDirA()


/*   MakeFileName() - make full path spec from directory string & file name.
 * SLB 11.dec.01 - call.fix.64955 changed to handle dst buffer length so
 * buffer is not over written.
------------------------------------------------------------------------*/
//SPDUTILS_IMP_EXP void __stdcall SpdUtils::MakeFileName(std::wstring &wsDest, const std::wstring &wsDir, const std::wstring &wsFileName) //SDEVICE_IMP_EXP void makeFName(char *dst, int dstlen, const char *dir, const char *fname) ;
//{
//   CNtfsFolderName fnFolder(CSpdCStringF512(wsDir.c_str())) ;
//   CSpdCStringF512 scsFullFolderName ;
//   
//   //
//   // Clear output string
//   //
//   wsDest.clear() ;
//
//   //
//   // create folder name string with ending '\\'char.
//   //
//   fnFolder.GetFullFolderName(scsFullFolderName, true) ;
//
//   //
//   // append filename to the end of it.
//   //
//   scsFullFolderName += CSpdCStringF512(wsFileName.c_str()) ;
//   wsDest = scsFullFolderName.c_str() ;
//} // makeFName()


const short cfBufLen = 16384;
/* CopyFile() - copy source file to dest file
---------------------------------------------------------------------------*/
SPDUTILS_IMP_EXP bool __stdcall SpdUtils::CopyFile(const std::wstring &wsDest, const std::wstring &wsSource)
{
BOOL bRetVal(FALSE) ;
	bRetVal = ::CopyFileW( wsSource.c_str(), wsDest.c_str(), false)  ;
	return (bRetVal==TRUE) ; // copy successful!
} // CopyFile()

/* PercentOf() - determine %x of y for to longs with out overflow.  If y is
 *               0, returns the value of x.
---------------------------------------------------------------------------*/
SPDUTILS_IMP_EXP QUADWORD __stdcall SpdUtils::PercentOf(QUADWORD qwX, QUADWORD qwY)
{
QUADWORD qwPp;
   if (!qwY)  // message->size is zero so don't divide.
      qwPp = qwX ;
   else if (qwX>0x10000L)
      qwPp = ( (qwX>>8) * 100 ) / (qwY>>8) ;
   else
      qwPp = (qwX*100) / qwY ;
   return qwPp ;
} // PercentOf()


/* Eof() - determine if file ptr is at EOF.  This function does not use
 *             eof(), which fails with file handles returned from Novell
 *             and NLMs are being used.
---------------------------------------------------------------------------*/
SPDUTILS_IMP_EXP bool __stdcall SpdUtils::Eof(int hfile)
{
long sz(_tell(hfile));
   // tell will determine if there are any problems with hfile.
   if ( sz<0 || sz!=_filelength(hfile))
      return sz != 0 ;
   else // hfile is ok and we are at EOF
      return 0 ;
} // Eof()

wchar_t *reserveddosnames[] =
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

//
// SLB 97.aug.2005 - Created for use by FixFileChar().
// Note: this is only 128 chars deep.  Methods using this table need
// to ensure that they do not index into this array any deeper.
// SLB 07.aug.2006 - the "+" char is valid for NTFS.  Changed it back to true.
const bool bValidNtfsFileNameChars[128] = 
{
   false, false, false, false, false, false, false, false, // 00-0f
   false, false, false, false, false, false, false, false,
   false, false, false, false, false, false, false, false, // 10-1f
   false, false, false, false, false, false, false, false,
   true,  true,  false, true,  true,  true,  true,  true,  // 20-2f  "\""
   true,  true,  false, true,  false, true,  true,  false, //        "*" "," "/"  
   true,  true,  true,  true,  true,  true,  true,  true,  // 30-3f
   true,  true,  false, true,  false, true,  false, false, //        ":" "<" ">" "?" 
   true,  true,  true,  true,  true,  true,  true,  true,  // 40-4f
   true,  true,  true,  true,  true,  true,  true,  true,  
   true,  true,  true,  true,  true,  true,  true,  true,  // 50-5f
   true,  true,  true,  true,  false, true,  true,  true,  //        "\"
   true,  true,  true,  true,  true,  true,  true,  true,  // 60-6f
   true,  true,  true,  true,  true,  true,  true,  true,  
   true,  true,  true,  true,  true,  true,  true,  true,  // 70-7f
   true,  true,  true,  true,  false, true,  true,  false //        "|" DEL 
} ;

/* FixFileChar()
 *    This is a local helper function for fixFileName(), and will replace
 *    the character with a valid one according to the os_type_flags. Note
 *    that the replacement character for one type might be an invalid
 *    character for another type (e.g., '_' is invalid for ostXeroxDJDE),
 *    thus the order of types is important, and when changes are made to
 *    this function one should check the logic of the type order to make
 *    sure the function will work properly for every combination of
 *    OsTypeFlags.
------------------------------------------------------------------------------*/
SPDUTILS_IMP_EXP void __stdcall SpdUtils::FixFileChar(wchar_t& c, DWORD os_type_flags)
{
   if (os_type_flags&ostNTFS)
   {  //
      // SLB 97.aug.2005 - modified to use a look up table vs. using isalnum().  Profiling
      // results showed that isalnum() was very expensive.
      //
      bool bValidNtfs ;

      if (c < 128)
      {
         bValidNtfs = bValidNtfsFileNameChars[c&0x7F] ;
      }
      else
      {
         bValidNtfs = false ;
      }
      if (!bValidNtfs)
         c = L'_' ;
   }
   else if (os_type_flags&ostNoRestricts)
   {
      // All characters valid for ostNoRestricts type.
   }
   else if (os_type_flags&ostDOS)
   {
      bool valid_dos = false;
      switch (c)
      {
         case L'_': case L'^': case L'$': case L'~': case L'!': case L'#': case L'%': case L'&':
         case L'-': case L'{': case L'}': case L'(': case L')': case L'@': case L'\'': case L'`':
            valid_dos = true;
            break;
         default:				
				valid_dos = isalnum(c) ? true : false ;
            break;
      }
      if (!valid_dos)
         c = L'_';
   } 
   else if (os_type_flags&ostUNIX)
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
            valid_unix = true;
            break;
         default:
				valid_unix = isalnum(c) ? true : false ;
            break;
      }
      if (!valid_unix)
         c = '_';
   }

   else if (os_type_flags&ostNovQueue)
   {
      // All characters valid for ostNovQueue type.
   } 
   else if (os_type_flags&ostXeroxDJDE)
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
             c == L',' || c == L'.' || c == L'(' || c == L')' || c == L'=' || c==L'\'' ||
             c == L'$' ) ;
         if (!valid_xrx)
            c = L'0';
      }
   }
}

/* FixDosName()
 *    This function is included for backward compatibility, use fixFileName()
 *    instead.
------------------------------------------------------------------------------*/
SPDUTILS_IMP_EXP std::wstring __stdcall SpdUtils::FixDosName( const std::wstring &wsFileName, bool bLongFileNamesOk) 
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
------------------------------------------------------------------------------*/
SPDUTILS_IMP_EXP std::wstring __stdcall SpdUtils::FixFileName( const std::wstring &wsInputFileName, DWORD dwOsTypes) 
{
   std::wstring::iterator it ;
   std::wstring wsFileName(wsInputFileName) ;

   if (dwOsTypes & ostDOS)
   {
   //char drive[MAXDRIVE], path[MAXPATH], name[MAXFILE], ext[MAXEXT];
      std::wstring wsDrive ;
      std::wstring wsPath ;
      std::wstring wsName ;
      std::wstring wsExt ;

      fnsplit(wsFileName, wsDrive, wsPath, wsName, wsExt) ;
      if (wsName.length() > 8) // force file name to 8 bytes.
         wsName.erase( wsName.begin()+8, wsName.end()) ;
      if (wsExt.length() > 4)// force ext to 4 bytes in case it is longer (includes the '.' char)
         wsExt.erase( wsExt.begin()+4, wsExt.end()) ;

      for (it = wsName.begin(); it != wsName.end() ; it++ ) 
      {
         wchar_t fc(*it) ;

         SpdUtils::FixFileChar(fc, dwOsTypes) ;
         *it = fc ;
      }
      
      if (wsExt.length() > 0)
      {// for the extension, skip the '.' char before checking
         for (it = wsExt.begin() + 1; it != wsExt.end() ; it++ ) 
         {
            wchar_t fc(*it) ;
            
            SpdUtils::FixFileChar(fc, dwOsTypes) ;
            *it = fc ;
         }
      }

      fnmerge(wsFileName, wsDrive, wsPath, wsName, wsExt) ;
   }
   else
   {
      for (it = wsFileName.begin(); it != wsFileName.end() ; it++ ) 
      {
         wchar_t fc(*it) ;
         
         SpdUtils::FixFileChar(fc, dwOsTypes) ;
         *it = fc ;
      }
   }

   if (dwOsTypes&ostDOS || dwOsTypes&ostNTFS)
   { // compare name to any DOS reserved names
      for (int i=0 ; reserveddosnames[i] ; i++ )
      {  //
         // SLB 11.jun.2005 CR.4028.Item56 - Turns out that NTFS file names can(at least on XP)
         // begin with a reserved name(unlike DOS file names).  Modified the code to test
         // differently for DOS or NTFS file naming type.
         // 
         if (
            (dwOsTypes&ostDOS && wsFileName.find(reserveddosnames[i])==0) ||
            (dwOsTypes&ostNTFS && wsFileName==std::wstring(reserveddosnames[i]))
            )
         {  // wsFileName begins with a reserved filename(DOS)
            // or wsFileName == a reserved filename(NTFS)
            wsFileName[0] = L'$' ;
            break ;
         }
      }
      //
      // NTFS file names, especially, should not end with a '.' char or a ' ' char
      //
      while (  
         wsFileName.size() &&  
         (wsFileName[wsFileName.length()-1]== L'.' || wsFileName[wsFileName.length()-1]== L' ')
         )
      {  //
         // filename ends with a '.' or ' ' char, remove it.
         //
         wsFileName.erase(wsFileName.end()-1) ;
      }
   }
   return wsFileName ;
}


/* FixFileName() - 
 *    This function will modify the fname passed to it, making the fname a
 *    valid filename for the operating system(s) specified by the os_type_flags.
------------------------------------------------------------------------------*/
//SPDUTILS_IMP_EXP void __stdcall SpdUtils::FixFileNameNtfs(const SpdStreamable::CSpdCStringBase &scsInputFileName, 
//                               SpdStreamable::CSpdCStringBase &scsOutputFileName
//                               ) 
//{
//   scsOutputFileName = scsInputFileName ;
//   size_t stPos(0) ;
//
//   for (
//      stPos = 0 ;
//      stPos < scsOutputFileName.wcslen() ;
//      stPos++
//      )
//   {
//      //wchar_t fc(*it) ;
//      
//      SpdUtils::FixFileChar(scsOutputFileName[stPos], ostNTFS) ;
//      //*it = fc ;
//   }
//
//   //
//   // invalid chars are taken care of... now
//   // search for reserved os names.
//   //
//   for (int i=0 ; reserveddosnames[i] ; i++ )
//   {  //
//      // SLB 11.jun.2005 CR.4028.Item56 - Turns out that NTFS file names can(at least on XP)
//      // begin with a reserved name(unlike DOS file names).  Modified the code to test
//      // differently for DOS or NTFS file naming type.
//      // 
//      if (scsOutputFileName==CSpdCStringF64(reserveddosnames[i]))
//      {  // scsOutputFileName begins with a reserved filename(DOS)
//         // or scsOutputFileName == a reserved filename(NTFS)
//         scsOutputFileName[0] = L'$' ;
//         break ;
//      }
//   }
//   //
//   // NTFS file names, especially, should not end with a '.' char or a ' ' char
//   //
//   while (  
//      scsOutputFileName.wcslen() &&  
//      (scsOutputFileName[scsOutputFileName.wcslen()-1]== L'.' || scsOutputFileName[scsOutputFileName.wcslen()-1]== L' ')
//      )
//   {  //
//      // filename ends with a '.' or ' ' char, remove it.
//      //
//      scsOutputFileName.erase(scsOutputFileName.wcslen()-1) ;
//   }
//}



/* newStr() -
 *    SLB 25.sep.97 added space char.
------------------------------------------------------------------------------*/
SPDUTILS_IMP_EXP char * __stdcall SpdUtils::newStr(const char *sz)
{
   size_t stBufLen(strlen(sz)+1) ;
   char *nsz = new char[stBufLen] ;

   if (nsz)
      strcpy_s(nsz, stBufLen, sz) ; // CR.13870; modified to use safe version 
   return nsz ;
} // newStr()

///* xatoi() - stupid function to convert hex string to int
//--------------------------------------------------------------------------------*/
//int __fastcall xatoi(const char *s)
//{
//int retval(0) ;
//int i(0) ;
//   if (!s)
//      return 0 ;
//   for (i=0; s[i]; i++)
//   { // main parsing loop
//      switch(s[i])
//      {
//         case '0': case '1': case '2': case '3': case '4':
//         case '5': case '6': case '7': case '8': case '9':
//            retval<<=4 ;
//            retval|= s[i]-'0' ;
//            break ;
//         case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
//            retval<<=4 ;
//            retval|= s[i]-'a'+10 ;
//            break ;
//         case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
//            retval<<=4 ;
//            retval|= s[i]-'A' + 10 ;
//            break ;
//         case ' ': case ',':
//            break ;
//         default:
//            return retval ;
//      }
//
//   }
//   return retval ;
//}

/* GetFullPath() - 
--------------------------------------------------------------------------------*/
SPDUTILS_IMP_EXP void __stdcall SpdUtils::GetFullPath( std::wstring &wsNewPath) //char* newpath )
{
   std::wstring wsDrive ;
   std::wstring wsPath ;
   std::wstring wsTmp1 ;
   std::wstring wsTmp2 ;
   std::wstring wsTmp3 ;
   std::wstring wsMyDir ;

   if (wsNewPath.empty())
      return ;

	//char sdrive[MAXDRIVE]={0};
   //char spath[MAXPATH]={0};
   //char myDir[256]={0};
   // Split path name up into its elements.  note, only interested
   // in drive letter here.
   fnsplit( wsNewPath, wsDrive, wsTmp1, wsTmp2, wsTmp3 );

   //check if full path
   if( wsDrive.length() > 0) // if (sdrive[0]=(char)toupper(sdrive[0])) == 0  )
   {  //check if relative to root or current directory
   wchar_t wcszMyDir[256] ;
      memset(wcszMyDir, 0, sizeof(wcszMyDir)) ;
      // Initialize startup directory string with the current directory.
      if( GetCurrentDirectoryW(sizeof(wcszMyDir)/sizeof(wchar_t) - 1, wcszMyDir) )
      {
      std::wstring wsMyDir(wcszMyDir) ;
      	fnsplit( wsMyDir, wsDrive, wsPath, wsTmp1, wsTmp2);
      }
      if (wsNewPath[0] != '\\')  
      {  //if( newpath[0] != '\\' ) //relative to current directory
      	wsPath += wsNewPath ; //strcat(spath, newpath);
      }
      else
      {
      	wsPath = wsNewPath ; //strcpy(spath, newpath);
      }

      fnmerge(wsNewPath, wsDrive, wsPath, L"", L"");
   }

   // Now remove trailing \ from path name.
   if (*(wsNewPath.end() -1) == L'\\') 
      wsNewPath.erase( wsNewPath.end()-1, wsNewPath.end() ) ;
   //if( strlen(newpath) && newpath[strlen(newpath)-1]=='\\' )
   //	newpath[strlen(newpath)-1]=0;
}



//bool SCreateDirectory(const char *newpath,  LPSECURITY_ATTRIBUTES lpsecattribs )
/* CreateFolder() - 
--------------------------------------------------------------------------------*/
//SPDUTILS_IMP_EXP bool __stdcall SpdUtils::CreateFolder(const std::wstring &wsNewPath, const LPSECURITY_ATTRIBUTES psaSecAttribs )
//{
//   bool bRetVal(false) ;
//   CNtfsFolderName fnFolder(CSpdCStringF512(wsNewPath.c_str())) ;
//   DWORD dwNtErr(0) ;
//
//   if (fnFolder.DoesFolderExist(dwNtErr))
//   {  //
//      // folder exists, exit.
//      //
//      bRetVal = true ;
//   }
//   else
//   {  //
//      // Folder does not exist..
//      //
//      if (!fnFolder.CreateFolder(dwNtErr, psaSecAttribs))
//      {  //
//         // failed to create the folder
//         //
//         bRetVal = true ;
//      }
//   }
//   return bRetVal ;   
//} // CreateFolder()


/* GetLastValidFolder() - Finds the last valid subdirectory in the supplied
 * string.  Found will be zero length if the drive letter is invalid.
--------------------------------------------------------------------------------*/
SPDUTILS_IMP_EXP void __stdcall SpdUtils::GetLastValidFolder(const std::wstring &wsSearch, std::wstring &wsFound)
{
   wsFound = L"" ;
} // SLastValidDirectory()


/* WaitForSingleObject() - Win32 wrapper to catch race conditions
--------------------------------------------------------------------------------*/
SPDUTILS_IMP_EXP DWORD __stdcall SpdUtils::WaitForSingleObject(HANDLE hMutex, DWORD )
{
DWORD ret(WAIT_FAILED) ;
   //ret = WaitForSingleObject( hMutex, 2000 ) ; // to after 2 secs.
   //switch( ret )
   //{
   //   case WAIT_ABANDONED:
   //      MessageBox(NULL,"Wait abandonded","WaitForSingleObject Failure",MB_OK|MB_TASKMODAL) ;
   //      break ;
   //   case WAIT_OBJECT_0:
   //      break ;
   //   case WAIT_TIMEOUT:
   //       MessageBox(NULL,"Timed out","WaitForSingleObject Failure",MB_OK |MB_TASKMODAL) ;
   //      break ;
   //   case WAIT_FAILED:
   //      MessageBox(NULL,"Wait Failed, use GetLastError()","WaitForSingleObject Failure",MB_OK|MB_TASKMODAL ) ;
   //      break ;
   //   default:
   //      MessageBox(NULL,"Unrecognized return value","WaitForSingleObject Failure",MB_OK|MB_TASKMODAL ) ;
   //      break ;
   //}
   return ret ;

} // SWaitForSingleObject()

#if 0
// SpdStringMatch() - Returns true if the two strings match. Wildcards '*' and '?'
// can be used in the first parameter. Case insensitive.
// DCM 18.feb.05 - This function is now obsolete. Please use WildcardCompare()
// instead.
//-----------------------------------------------------------------------------
SPDUTILS_IMP_EXP bool __stdcall SpdUtils::StringMatch(char *wildStr, char *matchStr)
{
    char *pWildStr = _strupr(wildStr);
   char *pMatchStr = _strupr(matchStr);
	if (!wildStr || !matchStr) return false;
	if (!strcmp("*.*", wildStr)) pWildStr = "*";
   while (*pWildStr && *pMatchStr) {
   	if (*pWildStr == '*') {
      	if (!*(pWildStr+1))
         	return true;
			// While loop will keep comparisons of the form StringMatch(*ax,axax)
         // from failing.
			while (!SpdUtils::StringMatch(pWildStr+1, pMatchStr++)) {
         	if (!*pMatchStr)
            	return false;
         }
         return true;
      }
      if (*pWildStr == *pMatchStr || *pWildStr == '?') {
      	pWildStr++;
         pMatchStr++;
      }
      else {
  /*
      	if (pWildStr == wildStr) // No *s yet
         	return false;
      	pWildStr = wildStr;
			pMatchStr++;
  */
	  		return false;
      }
	}
   if (!*pWildStr && !*pMatchStr)
   	return true;
   if (*pWildStr) {
		while (*pWildStr)
      	if (*pWildStr++ != '*')
         	return false;
      return true;
/*
   	if (*pWildStr == '*' && !*(pWildStr+1))
      	return true;
      else
      	return false;
*/
   }
   return false;
}
#endif


/* wsplitpath() - an stl version of the crt _wsplitpath() routine.
 * SLB 08.may.2004 - created. At this time this is just a wrapper arount the crt 
 * version of the function.
---------------------------------------------------------------------------*/
SPDUTILS_IMP_EXP void __stdcall SpdUtils::wsplitpath(   
   const std::wstring &wsPath, 
   std::wstring &wsDrive,
   std::wstring &wsDir,
   std::wstring &wsFName,
   std::wstring &wsExt
   )
{
wchar_t szDrive[MAX_PATH] ;
wchar_t szDir[MAX_PATH*2] ;
wchar_t szFName[MAX_PATH*2] ;
wchar_t szExt[MAX_PATH*2] ;
   szDrive[0] = 0 ;
   szDir[0] = 0 ;
   szFName[0] = 0 ;
   szExt[0] = 0 ;
   _wsplitpath_s(                         // CR.13870; modified to use safe version 
      wsPath.c_str(),
      szDrive,
      sizeof(szDrive)/sizeof(wchar_t),    // CR.13870; added.    
      szDir,
      sizeof(szDir)/sizeof(wchar_t),      // CR.13870; added.  
      szFName,
      sizeof(szFName)/sizeof(wchar_t),    // CR.13870; added.  
      szExt,
      sizeof(szExt)/sizeof(wchar_t)       // CR.13870; added.  
      ) ;
   wsDrive = szDrive ;
   wsDir = szDir ;
   wsFName = szFName ;
   wsExt = szExt ;
}

/* wmakepath() - an stl version of the crt _wmakepath() routine.
 * SLB 08.may.2004 - created. At this time this is just a wrapper arount the crt 
 * version of the function.
---------------------------------------------------------------------------*/
SPDUTILS_IMP_EXP void __stdcall SpdUtils::wmakepath( std::wstring &wsPath,
   const std::wstring &wsDrive,
   const std::wstring &wsDir,
   const std::wstring &wsFName,
   const std::wstring &wsExt
   )
{
   wchar_t szPath[MAX_PATH*2] ;
   _wmakepath_s( szPath,
               wsDrive.c_str(),
               wsDir.c_str(),
               wsFName.c_str(),
               wsExt.c_str()
             ) ;
   wsPath = szPath ;
}

// ExtractFileDrive()
// Implemented as a STL of the borland ExtractFileDrive() method.
// SLB 01.jul.04 - created.
//-----------------------------------------------------------------------------
SPDUTILS_IMP_EXP std::wstring __stdcall SpdUtils::ExtractFileDrive(const std::wstring &wsPathName)
{
std::wstring wsDrive ;
std::wstring wsDir ;
std::wstring wsFName ;
std::wstring wsExt ;

	SpdUtils::wsplitpath( wsPathName, wsDrive, wsDir, wsFName, wsExt ) ;
	return wsDrive ;
}


// MaxFileNameLength() - Returns the max length of a job name, or directory for
//		the given drive string.
//---------------------------------------------------------------------------
SPDUTILS_IMP_EXP DWORD __stdcall SpdUtils::MaxFileNameLength(const std::wstring &wsDriveString)
{
wchar_t wcBuf[MAX_PATH] ;
DWORD dwLength(0) ;
DWORD dwFlags(0) ;
std::wstring wsDrive ;
BOOL bGetVolInfo(false) ;                                       
   // JMF 02.mar.04 - This function will extract the root from both paths with mapped drives
   // and from paths which use UNC
   wsDrive = SpdUtils::ExtractFileDrive(wsDriveString) ;
   wsDrive += L"\\"; // Required for GetVolumeInformation()

	bGetVolInfo = GetVolumeInformationW(	wsDrive.c_str(), 
														wcBuf, 
														sizeof(wcBuf)/sizeof(wchar_t), 
														NULL, 
														&dwLength, 
														&dwFlags, 
														wcBuf, 
														sizeof(wcBuf)/sizeof(wchar_t)
														) ;
	if (bGetVolInfo)
    	return dwLength;
	else
    	return 0;
}



/* wstrupr()
 *    Convert the string s to upper case.
------------------------------------------------------------------------------*/
SPDUTILS_IMP_EXP std::wstring __stdcall SpdUtils::wstrupr(std::wstring& s)
{
   std::locale loc;
   for (std::wstring::iterator i = s.begin(), e = s.end(); i != e; i++)
      *i = std::toupper(*i, loc);
   return s;
}



// WildcardCompare()
//    Compare a string to a wildcard mask containing '*' and '?' characters.
//    Return true for a match, false otherwise.
//-----------------------------------------------------------------------------
SPDUTILS_IMP_EXP bool __stdcall wcsWildCmp(const wchar_t* str, const wchar_t* mask)
{
   // If end of string is reached, there is a match only if all that's left
   // in the mask are '*' chars. We also a single '.' char to appear within
   // the '*' chars, since masks such as "*.*" and "s*.*" are common and need
   // to match file names without extensions.
   if (*str == 0)
   {
      bool dot_found = false;
      while (*mask == L'*' || (*mask == L'.' && !dot_found))
      {
         if (*mask == L'.')
            dot_found = true;
         mask++;
      }
      return (*mask == 0);
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
SPDUTILS_IMP_EXP bool __stdcall SpdUtils::WildcardCompare(
   const std::wstring& wsStr, const std::wstring& wsMask, bool case_sensitive)
{
   // Convert strings to upper case for case insensitive search.
   std::wstring str = wsStr;
   std::wstring mask = wsMask;
   if (!case_sensitive)
   {
      SpdUtils::wstrupr(str);
      SpdUtils::wstrupr(mask);
   }

   return wcsWildCmp(str.c_str(), mask.c_str());
}




// XmlEscapeString()
//-----------------------------------------------------------------------------
SPDUTILS_IMP_EXP bool __stdcall SpdUtils::XmlEscapeString(const wchar_t *pwcsIn, 
                                                          wchar_t *pwcsOut, 
                                                          size_t stOutBufLen, 
                                                          size_t &stOutStringLen
                                                          )
{
   bool bRetVal(false) ;
   //
   // initialize output to invalid value.
   //
   stOutStringLen = (size_t)-1 ;
   if (!pwcsIn || !pwcsOut)
   {  //
      // problem: invalid buffer object(s) specified.
      //
      assert(pwcsIn && pwcsOut) ;
   }
   else
   {  
      size_t stInPos(0) ;
      size_t stOutPos(0) ;
      //
      // begin search and replace.
      //
      for (
         stInPos = stOutPos = 0 ; 
         pwcsIn[stInPos] && stOutPos<stOutBufLen-1 ;
         stInPos++
         )
      {
         wchar_t *pwcsNewOut(NULL) ;
         size_t stOutLen(0) ;
         //
         // check for special characters
         //
         wchar_t pwcsNewBuf[256] ;
         wchar_t wcInChar = pwcsIn[stInPos] ;
         if (wcInChar < 0x20 && wcInChar != 0x09 && wcInChar != 0x0A && wcInChar != 0x0D)
         {
            // We don't put a leading zero for 0x0 - 0xF since the .net XML classes
            // don't do this.
            stOutLen = _snwprintf_s(pwcsNewBuf, sizeof(pwcsNewBuf)/sizeof(wchar_t),
               L"&#x%X;", (int)wcInChar) ;
            pwcsNewOut = pwcsNewBuf ;
         }
         else
         {
            switch (wcInChar)
            {
            case L'&': 
               stOutLen = 5 ;
               pwcsNewOut =  L"&amp;" ;
               break ;
            case L'>':
               stOutLen = 4 ;
               pwcsNewOut =  L"&gt;" ;
               break ;
            case L'<':
               stOutLen = 4 ;
               pwcsNewOut =  L"&lt;" ;
               break ;
            case L'\"':
               stOutLen = 6 ;
               pwcsNewOut =  L"&quot;" ;
               break ;
            case L'\'':
               stOutLen = 6 ;
               pwcsNewOut =  L"&apos;" ;
               break ;
            default: 
               //
               // Not a special character.
               // Take care of copyin the data here.
               //
               stOutLen = 0 ;
               pwcsOut[stOutPos] = wcInChar ;
               stOutPos++ ;
               break ;
            }
         }
         if (pwcsNewOut && stOutBufLen>stOutPos+stOutLen)
         {  //
            // take care of copying special character's escaped value here.
            //
            wmemcpy(pwcsOut+stOutPos, pwcsNewOut, stOutLen) ;
         }
         //
         // update output position.
         // Note: this will only be non-zero for escaped characters.
         //
         stOutPos += stOutLen ;
      }
      if (!(stOutPos<stOutBufLen-1))
      {  //
         // problem: output buffer was not large enough to include terminating 0.
         //
         if (stOutBufLen)
            pwcsOut[0] = 0 ;
         bRetVal = false ;
      }
      else
      {
         pwcsOut[stOutPos] = 0 ;
         stOutStringLen = stOutPos ;
         bRetVal = true ;
      }
   }
   return bRetVal ;
}

// XmlUnescapeString()
//-----------------------------------------------------------------------------
SPDUTILS_IMP_EXP bool __stdcall SpdUtils::XmlUnescapeString(const wchar_t *pwcsIn, 
   wchar_t *pwcsOut, 
   size_t stOutBufLen, 
   size_t &stOutStringLen
   )
{
   bool bRetVal(false) ;
   //
   // initialize output to invalid value.
   //
   stOutStringLen = (size_t)-1 ;
   if (!pwcsIn || !pwcsOut)
   {  //
      // problem: invalid buffer object(s) specified.
      //
      assert(pwcsIn && pwcsOut) ;
   }
   else
   {  
      size_t stInPos(0) ;
      size_t stOutPos(0) ;
      //
      // begin search and replace.
      //
      for (
         stInPos = stOutPos = 0 ; 
         pwcsIn[stInPos] && stOutPos<stOutBufLen-1 ;
         //stInPos++
         )
      {
         //
         // Note all escaped strings begin with the '&' char and are terminated by the ';' char.
         // 
         switch (pwcsIn[stInPos])
         {
         case L'&':
            {
               size_t stSubStrLen(0) ;

               stSubStrLen = wcscspn(&pwcsIn[stInPos], L";") + 1 ; // find position of next ';' char
               if (!wcsncmp(&pwcsIn[stInPos], L"&gt;", stSubStrLen))
               {
                  pwcsOut[stOutPos++] = L'>' ;
                  stInPos+=4 ;
               }
               else if (!wcsncmp(&pwcsIn[stInPos], L"&lt;", stSubStrLen))
               {
                  pwcsOut[stOutPos++] = L'<' ;
                  stInPos += 4 ;
               }
               else if (!wcsncmp(&pwcsIn[stInPos], L"&amp;", stSubStrLen))
               {
                  pwcsOut[stOutPos++] = L'&' ;
                  stInPos += 5 ;
               }
               else if (!wcsncmp(&pwcsIn[stInPos], L"&quot;", stSubStrLen))
               {
                  pwcsOut[stOutPos++] = L'\"' ;
                  stInPos += 6 ;
               }
               else if (!wcsncmp(&pwcsIn[stInPos], L"&apos;", stSubStrLen))
               {
                  pwcsOut[stOutPos++] = L'\'' ;
                  stInPos += 6 ;
               }
               else if (!wcsncmp(&pwcsIn[stInPos], L"&#x", stSubStrLen)) // very rare
               {
                  int escChar ;
                  if (_snwscanf_s(&pwcsIn[stInPos], stSubStrLen, L"&#x%X;", &escChar) > 0)
                  {
                     pwcsOut[stOutPos++] = (wchar_t)escChar ;
                     stInPos += stSubStrLen ;
                  }
               }
               else
               {
                  pwcsOut[stOutPos++] = pwcsIn[stInPos++] ;
               }
            }
            break ;
         default:
            pwcsOut[stOutPos++] = pwcsIn[stInPos++] ;
            break ;
         }
      }
      if (!(stOutPos<stOutBufLen-1))
      {  //
         // problem: output buffer was not large enough to include terminating 0.
         //
         if (stOutBufLen)
            pwcsOut[0] = 0 ;
         bRetVal = false ;
      }
      else
      {
         pwcsOut[stOutPos] = 0 ;
         stOutStringLen = stOutPos ;
         bRetVal = true ;
      }
   }
   return bRetVal ;
}

//
// This 2-D array contains XML escape sequences and there corresponding un-escaped values.
//
const std::wstring wsEscapedStrings[][2] = 
{
   { std::wstring(L"&amp;"), std::wstring(L"&") },
   { std::wstring(L"&gt;"), std::wstring(L">") },
   { std::wstring(L"&lt;"), std::wstring(L"<") },
   { std::wstring(L"&quot;"), std::wstring(L"\"") },
   { std::wstring(L"&apos;"), std::wstring(L"'") },
   { std::wstring(L""),    std::wstring(L"")  } // array terminator.
} ;

// XmlEscapeString()
//-----------------------------------------------------------------------------
SPDUTILS_IMP_EXP std::wstring __stdcall SpdUtils::XmlEscapeString(const std::wstring &wsXmlUnescapedString)
{
   std::wstring wsRetVal ;
   size_t i(0) ;
   //
   // begin search and replace.
   //
   wsRetVal = wsXmlUnescapedString ;
   for (
      i=0 ; 
      wsEscapedStrings[i][1].length() ;
      i++
      )
   {
      size_t stEscapePos(0) ;
      //
      // special char which needs to be escaped has been determined...  the loop 
      // variable, i, does this.
      //
      // The following loop searches for all instances of this char...
      // (wsEscapedStrings[i][1]).  When this char is found, it is replaced
      // with the corresponding escaped string.
      //
      for (
         stEscapePos = wsRetVal.find(wsEscapedStrings[i][1]) ;
         stEscapePos != std::wstring::npos ;
         stEscapePos = wsRetVal.find(wsEscapedStrings[i][1], stEscapePos)
         )
      {
         //
         // At the position where the special char was found, replace the
         // char with the corresponding escape string (wsEscapedStrings[i][0])
         //
         wsRetVal.replace(
            stEscapePos, 
            wsEscapedStrings[i][1].length(),
            wsEscapedStrings[i][0]
            ) ;
         //
         // update the search position to be beyond the replacement string.
         //
         stEscapePos += wsEscapedStrings[i][0].length() ;
      }
      //
      // All instances of the current special char have been replaced.
      // continue to search and replace the next escape string.
      //
   }
   
   return wsRetVal ;
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
} ;
// ConvertNonPrintableToHex()
//-----------------------------------------------------------------------------
SPDUTILS_IMP_EXP std::wstring __stdcall SpdUtils::ConvertNonPrintableToHex(const std::wstring &wsRawString)
{
   std::wstring wsRetVal ;
   size_t i(0) ;
   //
   // begin search and replace.
   //
   wsRetVal = wsRawString ;
   for (
      i=0 ; 
      wsNonPrintableStrings[i][1].length() ;
      i++
      )
   {
      size_t stEscapePos(0) ;
      //
      // special char which needs to be escaped has been determined...  the loop 
      // variable, i, does this.
      //
      // The following loop searches for all instances of this char...
      // (wsEscapedStrings[i][1]).  When this char is found, it is replaced
      // with the corresponding escaped string.
      //
      for (
         stEscapePos = wsRetVal.find(wsNonPrintableStrings[i][1]) ;
         stEscapePos != std::wstring::npos ;
         stEscapePos = wsRetVal.find(wsNonPrintableStrings[i][1], stEscapePos)
         )
      {
         //
         // At the position where the special char was found, replace the
         // char with the corresponding escape string (wsEscapedStrings[i][0])
         //
         wsRetVal.replace(
            stEscapePos, 
            wsNonPrintableStrings[i][1].length(),
            wsNonPrintableStrings[i][0]
            ) ;
         //
         // update the search position to be beyond the replacement string.
         //
         stEscapePos += wsNonPrintableStrings[i][0].length() ;
      }
      //
      // All instances of the current special char have been replaced.
      // continue to search and replace the next escape string.
      //
   }
   
   return wsRetVal ;
}

// XmlUnescapeString()
//-----------------------------------------------------------------------------
SPDUTILS_IMP_EXP std::wstring __stdcall SpdUtils::XmlUnescapeString(const std::wstring &wsXmlEscapedString)
{
std::wstring wsRetVal ;
size_t i(0) ;

   //
   // begin search and replace.
   //

   wsRetVal = wsXmlEscapedString ;
   for (i=0 ; 
        wsEscapedStrings[i][0].length() ;
        i++
        )
   {
   size_t stEscapePos(0) ;
      //
      // Unescaped string has been determined...  the loop variable, i, does this.
      //
      // The following loop searches for all instances of the current escaped string...
      // (wsEscapedStrings[i][0]).  When the escaped string is found, it is replaced
      // with the corresponding un-escaped string.
      //
      for (stEscapePos = wsRetVal.find(wsEscapedStrings[i][0]) ;
           stEscapePos != std::wstring::npos ;
           stEscapePos = wsRetVal.find(wsEscapedStrings[i][0], stEscapePos)
           )
      {
         //
         // At the position where the escape sequence was found, replace the
         // escape string with the corresponding Un-escaped string (wsEscapedStrings[i][1])
         //
         wsRetVal.replace(stEscapePos, 
            wsEscapedStrings[i][0].length(),
            wsEscapedStrings[i][1]
            ) ;
         //
         // update the search position to be beyond the replacement string.
         //
         stEscapePos += wsEscapedStrings[i][1].length() ;
      }
      //
      // All instances of the current escape string have been replaced.
      // continue to search and replace the next escape string.
      //
   }
   
   return wsRetVal ;
}


SPDUTILS_IMP_EXP void __stdcall SpdUtils::GetWin32ErrorMessage(std::wstring &wsMsg, DWORD dwNtErr) 
{
   // clear message to begin with.
   wsMsg.clear() ; 
   // Get last error for the calling thread.
   if (!dwNtErr)
      dwNtErr = GetLastError() ;
   if (dwNtErr) 
   {
      // Create message header.
      wchar_t wcHeader[32];
      wchar_t *wcRetMsg(NULL) ;
      wchar_t *pwcErrMsg(NULL) ;
      void *ppErrMsg(&pwcErrMsg) ;
	   unsigned int nMsgSize(0) ;

      _snwprintf_s(wcHeader, sizeof(wcHeader)/sizeof(wchar_t), L"NT(%08X): ", dwNtErr) ;
      
      // Format message.
      nMsgSize = FormatMessageW(
         FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, 
         dwNtErr, 
         MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(wchar_t *)ppErrMsg, 
         0, 
         NULL
         ) ;

      if (nMsgSize && pwcErrMsg)
      {
         wchar_t pCrLf[] = { 0x000D, 0x000A, 0x0000 } ; //cr/lf
         size_t stSearchStartPos(0) ;

         // Construct message.
         wsMsg = std::wstring(wcHeader) + std::wstring(pwcErrMsg) ;

         // Free memory allocated by FormatMessage.
         LocalFree(pwcErrMsg) ;

         // Replace cr/lf with spaces.
         stSearchStartPos = wsMsg.find(pCrLf, stSearchStartPos, 2) ;
         if (stSearchStartPos != std::wstring::npos)
         {
            wsMsg.replace(stSearchStartPos, 2, L"") ;
         }
      }
   }
}

SPDUTILS_IMP_EXP DWORD __stdcall SpdUtils::DeleteFolderAndFiles(std::wstring &wsFolder) 
{
	DWORD lastError = 0;
	WCHAR debug_buf[1024];
	if(PathFileExists(wsFolder.c_str()) == true)
	{
		// Walk through all the files in the directory, remove Read-Only attribute and delete the files
		WIN32_FIND_DATAW ffd;
		WCHAR szPathFind[MAX_PATH];
		wmemcpy_s(&szPathFind[0], MAX_PATH, wsFolder.c_str(), MAX_PATH);
		PathAppend(szPathFind, L"*");

		HANDLE hFind = FindFirstFile(szPathFind, &ffd);
		do 
		{
			if (hFind != INVALID_HANDLE_VALUE)
			{
				if((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
				{
//_snwprintf_s(debug_buf, 1024, L"SoftwareServerDataMgr::LoadFromFile() - File: '%s', A: %d", ffd.cFileName, ffd.dwFileAttributes);
//OutputDebugStringW(debug_buf);

					WCHAR szDeleteFile[MAX_PATH];
					wmemcpy_s(&szDeleteFile[0], MAX_PATH, wsFolder.c_str(), MAX_PATH);
					PathAppend(szDeleteFile, ffd.cFileName);

					_wchmod(szDeleteFile, _S_IWRITE);	// Take off Read-Only Attr
					if (!DeleteFile(szDeleteFile))
					{
_snwprintf_s(debug_buf, 1024, L"DeleteFile Failed, File: '%s', Error Code: %d", szDeleteFile, GetLastError());
OutputDebugStringW(debug_buf);
					}
				}
			}
		} while (FindNextFile(hFind, &ffd) != 0);

		if (hFind != INVALID_HANDLE_VALUE)
			FindClose(hFind);

		if(!RemoveDirectory(wsFolder.c_str()))
		{
_snwprintf_s(debug_buf, 1024, L"RemoveDirectory Failed, Directory: '%s', Error Code: %d", wsFolder.c_str(), GetLastError());
OutputDebugStringW(debug_buf);
		}
	}
	return lastError;
}

