//----------------------------------------------------------------------------------------*
// filename -       SpdUtils.h                                                            *
//                                                                                        *
//             This module contains some helper functions dealing with DOS file           *
//             naming conventions and other misc. routines                                *
//                                                                                        *
// Classes:                                                                               *
//    MutexLock                                                                           *
//                                                                                        *
// Functions:                                                                             *
//    mkLineStr()                                                                         *
//    verifyDir()                                                                         *
//    makeFName()                                                                         *
//    copyFile()                                                                          *
//    percentOf()                                                                         *
//    fixDosName()                                                                        *
//    xatoi()                                                                             *
//    SCreateDirectory()                                                                  *
//                                                                                        *
//                                                                                        *
//    Copyright (c) 1993-1998 by Solimar Systems, Inc.  All Rights Reserved.              *
//                                                                                        *
// Release Info:                                                                          *
// Nm     Date         Discription                                                        *
// SLB   07.jan.98   Added SCreateDirectory()                                             *
// SLB   28.oct.97   SPD Nt port.                                                         *
//----------------------------------------------------------------------------------------*
#pragma once
#include <string>
#include "quadword.h"
//#include "SpdStreamable.h"
#include <assert.h>
#include <exception>

#ifndef BIT
#define BIT(x) (((QUADWORD)1) << x)
#endif

#define fnsplit(a,b,c,d,e) SpdUtils::wsplitpath(a,b,c,d,e)
#define fnmerge(a,b,c,d,e) SpdUtils::wmakepath(a,b,c,d,e)

#ifndef SPDUTILS_EXPORTS
   #define SPDUTILS_IMP_EXP
#else
   #define SPDUTILS_IMP_EXP  __declspec(dllexport)
#endif

#define TERMINATE_WCS(wcs)  wcs[sizeof(wcs)/sizeof(wchar_t)-1] = 0


namespace SpdUtils
{

	enum OsTypeFlags
	{
		ostNoRestricts=0x1,
		ostDOS=0x2,
		ostNTFS=0x4,
		ostUNIX=0x8,
		ostNovQueue=0x10,
		ostXeroxDJDE=0x20
	}  ;

	SPDUTILS_IMP_EXP bool __stdcall VerifyFolder(
      const std::wstring &wsDir, 
      bool bWriteRights=false
      ) ; 
	SPDUTILS_IMP_EXP bool __stdcall VerifyFolderA(
      const char *strDir, 
      bool bWriteRights=false
      ) ;
	SPDUTILS_IMP_EXP void __stdcall MakeFileName(
      std::wstring &wsDest, 
      const std::wstring &wsDir, 
      const std::wstring &wsFileName
      ) ; 
	SPDUTILS_IMP_EXP bool __stdcall CopyFile(
      const std::wstring &wsDest, 
      const std::wstring &wsSource
      ) ; 
	SPDUTILS_IMP_EXP QUADWORD __stdcall PercentOf(
      QUADWORD x, 
      QUADWORD y
      ) ;
	SPDUTILS_IMP_EXP bool __stdcall Eof(
      int hfile
      ) ;
	SPDUTILS_IMP_EXP void __stdcall FixFileChar(
      wchar_t &c, 
      DWORD os_type_flags
      );
	SPDUTILS_IMP_EXP std::wstring __stdcall FixDosName( 
      const std::wstring &wsFileName, 
      bool bLongFileNamesOk=false
      ) ; //SDEVICE_IMP_EXP char * fixDosName( char *fname, bool longfilenames=false ) ;
	SPDUTILS_IMP_EXP std::wstring __stdcall FixFileName( 
      const std::wstring &wsFileName, 
      DWORD dwOsTypes
      ) ; //SDEVICE_IMP_EXP char * fixFileName(char *fname, OsTypeFlags os_type_flags);
   //SPDUTILS_IMP_EXP void __stdcall FixFileNameNtfs(
   //   const SpdStreamable::CSpdCStringBase &scsInputFileName, 
   //   SpdStreamable::CSpdCStringBase &scsOutputFileName
   //   ) ;
   SPDUTILS_IMP_EXP char * __stdcall newStr(
      const char *szString
      ) ;
	//SPDUTILS_IMP_EXP int  xatoi(const char *s) ;
	SPDUTILS_IMP_EXP bool __stdcall CreateFolder( 
      const std::wstring &wsNewDir, 
      LPSECURITY_ATTRIBUTES lpsecattribs = NULL 
      ) ; //SDEVICE_IMP_EXP bool  SCreateDirectory(const char *newdir, LPSECURITY_ATTRIBUTES lpsecattribs = NULL ) ;
	SPDUTILS_IMP_EXP void __stdcall GetFullPath(
      std::wstring &wsNwePath
      ) ; //SDEVICE_IMP_EXP void SGetFullPath(char*);
	SPDUTILS_IMP_EXP void __stdcall GetLastValidFolder( 
      const std::wstring &wsSearch, 
      std::wstring &wsFound 
      ) ; //SDEVICE_IMP_EXP void  SLastValidDirectory( const char *search, char *found) ;
	SPDUTILS_IMP_EXP DWORD __stdcall WaitForSingleObject(
      HANDLE hMutex, 
      DWORD 
      ) ;
	SPDUTILS_IMP_EXP bool __stdcall StringMatch(
      char *wildStr, 
      char *matchStr
      );
	SPDUTILS_IMP_EXP void __stdcall wsplitpath(
      const std::wstring &wsPath, 
      std::wstring &wsDrive,
      std::wstring &wsDir,
		std::wstring &wsFName,
		std::wstring &wsExt
		) ;
	SPDUTILS_IMP_EXP void __stdcall wmakepath( 
      std::wstring &wsPath,
		const std::wstring &wsDrive,
		const std::wstring &wsDir,
		const std::wstring &wsFName,
		const std::wstring &wsExt
		) ;
	SPDUTILS_IMP_EXP std::wstring __stdcall ExtractFileDrive(
      const std::wstring &wsPathName
      ) ;
	SPDUTILS_IMP_EXP DWORD __stdcall MaxFileNameLength(
      const std::wstring &wsDriveString
      ) ;
   SPDUTILS_IMP_EXP std::wstring __stdcall wstrupr(
      std::wstring& ws
      );
   SPDUTILS_IMP_EXP bool __stdcall WildcardCompare(
      const std::wstring& wsStr, 
      const std::wstring& wsMask, 
      bool case_sensitive=false
      ) ;
   SPDUTILS_IMP_EXP bool __stdcall XmlEscapeString(
      const wchar_t *pwcsIn, 
      wchar_t *pwcsOut, 
      size_t stOutBufLen, 
      size_t &stOutStringLen
      ) ;
   SPDUTILS_IMP_EXP bool __stdcall XmlUnescapeString(
      const wchar_t *pwcsIn, 
      wchar_t *pwcsOut, 
      size_t stOutBufLen, 
      size_t &stOutStringLen
      ) ;
   SPDUTILS_IMP_EXP std::wstring __stdcall XmlEscapeString(
      const std::wstring &wsXmlUnescapedString
      ) ;
   SPDUTILS_IMP_EXP std::wstring __stdcall XmlUnescapeString(
      const std::wstring &wsXmlEscapedString
      ) ;
   SPDUTILS_IMP_EXP std::wstring __stdcall ConvertNonPrintableToHex(
      const std::wstring &wsRawString
      ) ;
   SPDUTILS_IMP_EXP void __stdcall GetWin32ErrorMessage(
      std::wstring &wsMsg, 
      DWORD dwNtErr
      ) ;

} 

