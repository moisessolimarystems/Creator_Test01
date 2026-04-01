#ifndef __QUADWORD_H
#define __QUADWORD_H
#ifndef STRICT
#define STRICT
#endif
#include <windows.h>
//#include "headers\sbase.h"

typedef unsigned __int64 QUADWORD;
#define SDEVICE_IMP_EXP


#define LODWORD(qw)  ((DWORD) (((QUADWORD) qw)& 0xFFFFFFFF))
#define HIDWORD(qw)  ((DWORD)(((QUADWORD)qw) >> 32))
#define MAKEQUADWORD(dwLo, dwHi)   ( (((QUADWORD)dwHi)<<32) | ((DWORD)dwLo) )
#define MAKE_QUADWORD_FROM_FILETIME(ft)  MAKEQUADWORD(ft.dwLowDateTime,ft.dwHighDateTime)

// MakeQuadWord()
inline QUADWORD MakeQuadWord(DWORD lo, DWORD hi)
{
	return ((QUADWORD)hi << 32) + lo;
}

// SplitQuadWord()
//		Returns the lo word, sets the hi word.
inline DWORD SplitQuadWord(QUADWORD quad, LONG & hi)
{
	hi = (LONG) (quad>>32);
	return (DWORD)quad;
}

// SetFilePointerQuadWord()
//		Wrapper for the Win32 function SetFilePointer(), using the
//		QUADWORD type.
//---------------------------------------------------------------------------
inline QUADWORD SetFilePointerQuadWord(	HANDLE hFile, 
														QUADWORD qwDistanceToMove,
														DWORD dwMoveMethod
														)
{
LONG lDistanceHi = HIDWORD(qwDistanceToMove) ;
DWORD dwDistanceLo = LODWORD(qwDistanceToMove) ;

   dwDistanceLo = SetFilePointer(	hFile, 
												dwDistanceLo, 
												&lDistanceHi,
												dwMoveMethod
												) ;
   return MakeQuadWord(dwDistanceLo, lDistanceHi) ;
}

// Tell() - Returns the current file position.
//---------------------------------------------------------------------------
inline QUADWORD TellQuadWord(HANDLE h_file)
{
	return SetFilePointerQuadWord(h_file, 0, FILE_CURRENT);
}

// GetFileSizeQuadWord()
//		Wrapper for the Win32 function GetFileSize(), using the
//		QUADWORD type.
//---------------------------------------------------------------------------
inline QUADWORD GetFileSizeQuadWord(HANDLE hFile)
{
DWORD dwLo, dwHi;
    dwLo = GetFileSize(hFile, &dwHi);
    return MakeQuadWord(dwLo, dwHi);
}

// GetFileSizeQuadWord()
//		Return the file size of the specified lpFileName. Returns 0 for file
//    not found.
//---------------------------------------------------------------------------
inline QUADWORD GetFileSizeQuadWord(const char* lpFileName)
{
   WIN32_FILE_ATTRIBUTE_DATA file_attribute_data;
   if (GetFileAttributesExA(lpFileName, GetFileExInfoStandard, &file_attribute_data))
      return MakeQuadWord(file_attribute_data.nFileSizeLow, file_attribute_data.nFileSizeHigh);
   else
      return 0;
}


// GetFileSizeQuadWordW()
//		Return the file size of the specified fileName. Returns 0 for file
//    not found.
//---------------------------------------------------------------------------
inline QUADWORD GetFileSizeQuadWordW(const wchar_t* fileName)
{
   WIN32_FILE_ATTRIBUTE_DATA file_attribute_data;
   if (GetFileAttributesExW(fileName, GetFileExInfoStandard, &file_attribute_data))
      return MakeQuadWord(file_attribute_data.nFileSizeLow, file_attribute_data.nFileSizeHigh);
   else
      return 0;
}


//
//SDEVICE_IMP_EXP QUADWORD SetFilePointerQuadWord(HANDLE h_file, QUADWORD q_distance_to_move,
//                                                DWORD dw_move_method) ;
//SDEVICE_IMP_EXP QUADWORD TellQuadWord(HANDLE h_file) ;
//SDEVICE_IMP_EXP QUADWORD GetFileSizeQuadWord(HANDLE hFile) ;
//SDEVICE_IMP_EXP QUADWORD GetFileSizeQuadWord(const char* lpFileName) ;
 
#endif
