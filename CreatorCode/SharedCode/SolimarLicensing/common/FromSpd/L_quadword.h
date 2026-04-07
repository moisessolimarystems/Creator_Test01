#pragma once

#ifndef _quadword_h																						// CR.32662.V04; added.
#define _quadword_h																						// CR.32662.V04; added.

#define SDEVICE_IMP_EXP

#ifndef WIDEN
#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)
#define __WFUNCTION__ WIDEN(__FUNCTION__)
#endif

typedef unsigned __int64 QUADWORD;
typedef unsigned __int64 QWORD;
#define LODWORD(qw)  ((DWORD) (((QUADWORD) qw)& 0xFFFFFFFF))
#define HIDWORD(qw)  ((DWORD)(((QUADWORD)qw) >> 32))
#define MAKEQUADWORD(dwLo, dwHi)   ( (((QUADWORD)dwHi)<<32) | ((DWORD)dwLo) )
#define MAKE_QUADWORD_FROM_FILETIME(ft)  MAKEQUADWORD(ft.dwLowDateTime,ft.dwHighDateTime)
#define MAKE_FILETIME_FROM_QUADWORD(qw) (CFILETIME(qw))

/*
 * class CFILETIME
 *
 * This class is meant to be a "clear" wrapper of the Win32 FILETIME struct. So there are no 
 * virtual methods and no additional members.
 *
 * It has helpful methods for converting FILETIMEs between QUADWORD, time_t and .NETs DateTime objects.
 * 
 * SLB 29.jan.2019 CR.26999.Item43; Fleshed this class out some more. Added operators for casting to commonly used types:
 * time_t, QUADWORD. 
 * Added comparison operators.
 * Added methods to clear and check if its clear.
 * Also, added/implemented InitFromDotnetTime() and GetDotnetTime(). These methods convert to/from a QUADWORD. But make 
 * calculations compatible with the .NET DateTime object.
 * SLB 08.feb.2019 CR.26999.Item26; Added constructor for time_t.
 * SLB 02.mar.2019 CR.26999.Item101; Updated time_t operations to work similar to DOTNET time conversion. There are explicit
 * methods which are then used by defined time_t constructors and operators.
 * Added more comparison operators.
 * SLB 11.jun.2021 CR.30240; Fix for comparison operators... Now all paths return a value.
 * SLB 30.jul.2021 CR.30710.V01; Fix for comparison operators... They were returning inverted results.
 * SLB 22.apr.2024 CR.32662.V03; added assignment operator for SYSTEMTIME.
 * SLB 17.jul.2025 CR.34222; FILETIME has multiple definitions in managed C++. Use
 * the default C++ definition.
 */
class CFILETIME : public ::FILETIME																	// CR.34222; modified.
{
public:
   CFILETIME()
   {
      GetSystemTimeAsFileTime(this);
   }
   CFILETIME(const ::FILETIME &ft)																	// CR.34222; modified.
   {
      this->operator = (ft);
   }
   CFILETIME(QUADWORD qw)
   {
      this->operator = (qw);
   }
   CFILETIME(time_t tt)                                                             // CR.26999.Item26; added.
   {
      this->operator = (tt);
   }

	CFILETIME(const SYSTEMTIME &st)																	// CR.32662.V03; added.
	{
		this->operator = (st);
	}

   CFILETIME & operator = (const ::FILETIME &ft)                                     // CR.34222; modified. CR.26999.Item43; added.
   {
      dwLowDateTime = ft.dwLowDateTime;
      dwHighDateTime = ft.dwHighDateTime;
      return *this;
   }
   CFILETIME & operator = (QUADWORD qw)                                             // CR.26999.Item43; added.
   {
      dwLowDateTime = LODWORD(qw);
      dwHighDateTime = HIDWORD(qw);
      return *this;
   }
   CFILETIME & operator = (time_t tt)                                               // CR.26999.Item43; added.
   {
      return InitFromTIme_tTime(tt);                                                // CR.26999.Item101; modified.
   }

	CFILETIME& operator = (const SYSTEMTIME& st)													// CR.32662.V03; added.
	{
		BOOL bRES;
		bRES = SystemTimeToFileTime(&st, this);
		return *this;
	}

   //
   // CR.26999.Item43; comparison operators added.
   //
   bool operator == (const CFILETIME &ft) const { if (CompareFileTime(this, &ft) == 0) return true; return false; } // CR.30240; modified.
   bool operator != (const CFILETIME &ft) const { if (CompareFileTime(this, &ft) != 0) return true; return false; } // CR.30240; modified.
   bool operator > (const CFILETIME &ft) const { if (CompareFileTime(this, &ft) > 0) return true; return false; } // CR.30710.V01; modified. CR.30240; modified.
   bool operator < (const CFILETIME &ft) const { if (CompareFileTime(this, &ft) < 0) return true; return false; } // CR.30710.V01; modified. CR.30240; modified.
   bool operator >= (const CFILETIME &ft) const { if (CompareFileTime(this, &ft) >= 0) return true; return false; } // CR.30710.V01; modified. CR.26999.Item101; added.
   bool operator <= (const CFILETIME &ft) const { if (CompareFileTime(this, &ft) <= 0) return true; return false; } // CR.30710.V01; modified. CR.26999.Item101; added.

   operator ::FILETIME & () { return *this; }													// CR.34222; modified. CR.26999.Item43; modified.
   operator QUADWORD() const { return MAKE_QUADWORD_FROM_FILETIME((*this)); }       // CR.26999.Item43; added.
   operator time_t() const { return GetTime_tTime(); }                              // CR.26999.Item43; added.
   void Clear() { dwLowDateTime = dwHighDateTime = 0; }                             // CR.26999.Item43; added.
   bool IsClear() const { return dwLowDateTime == dwHighDateTime == 0; }            // CR.26999.Item43; added.
	CFILETIME & UTCtoLocal()																			// CR.34222; added.
	{ 
		CFILETIME ft(*this);
		FileTimeToLocalFileTime(&ft, this);

		return *this;
	}

   // CR.26999.Item43 - convert between .NET DateTime Ticks and FileTime.
   //
   // .NET DateTime and FILETIME use a tick value that is equal, it is 100ns or 10,000,000 ticks/sec. 
   // However, .NETs start date for a time stamp is Jan 1,0001 while FILETIMEs is Jan 1, 16001. 
   // Calculate and subtract difference, in Ticks, from the .NET time to get the FILETIME.
   //
   // TicksInAYear - 10,000,000 Ticks/sec *60 sec/min *60min/hr *24hr/day *365day/yr
   #define FT_TICKS_PER_DAY ((QUADWORD)10000000L * 60L * 60L * 24L)
   #define FT_TICKS_PER_YEAR (FT_TICKS_PER_DAY * 365L)

	// Number of leap years. 
   // There is a leap year every year whose number is perfectly divisible by four - except for years which are both 
   // divisible by 100 and not divisible by 400. 
   #define LEAP_YEARS(yr) (((yr)/4) - ((yr)/100 -(yr)/400))
   #define LEAP_YEARS_IN_RANGE(y0, y1) (LEAP_YEARS(y1) - LEAP_YEARS(y0))
   
   // Number of ticks between 0001 and 1601:  Ticks for number std years + Ticks for each additonal day due to a leap year.
   #define FT_TICKS_TO_1601 (FT_TICKS_PER_YEAR*(1601L-1L) + FT_TICKS_PER_DAY * LEAP_YEARS_IN_RANGE(1, 1601))
   CFILETIME InitFromDotnetTime(QUADWORD qwDotnet)                                  // CR.26999.Item43; added.
   {
		if (qwDotnet < FT_TICKS_TO_1601) { throw 1; }

      return this->operator = (qwDotnet - FT_TICKS_TO_1601);
   }
   QUADWORD GetDotnetTime() const                                                   // CR.26999.Item43; added.
   {
      return MAKE_QUADWORD_FROM_FILETIME((*this)) + FT_TICKS_TO_1601;
   }
   //
   // CR.26999.Item101; time_t converting.
   // For time_t conversions, number of FT ticks between 1601 and 1970 is used.
   // Also, uses differnt mechanisms for initializing if time_t is 32bit or 64bit value.
   //
   #define FT_TICKS_1601_TO_1970 (FT_TICKS_PER_YEAR*(1970-1601) +FT_TICKS_PER_DAY* LEAP_YEARS_IN_RANGE(1601, 1970))
   CFILETIME & InitFromTIme_tTime(time_t tt)                                          // CR.26999.Item101; added.
   {
      #ifdef _USE_32BIT_TIME_T
      LONGLONG ll = Int32x32To64(tt, 10000000) + 116444736000000000;
      #else
      LONGLONG ll = tt*10000000 + 116444736000000000;
      #endif
      return this->operator = ((QUADWORD)ll);
   }
   time_t GetTime_tTime() const                                                     // CR.26999.Item101; added.
   {
      if (MAKE_QUADWORD_FROM_FILETIME((*this)) < FT_TICKS_1601_TO_1970) { throw 1; }

      return (time_t)(MAKE_QUADWORD_FROM_FILETIME((*this)) - FT_TICKS_1601_TO_1970) / 10000000;
   }
};

#if 0
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
inline QUADWORD SetFilePointerQuadWord(	
   HANDLE hFile, 
	QUADWORD qwDistanceToMove,
	DWORD dwMoveMethod)
{
   LONG lDistanceHi(HIDWORD(qwDistanceToMove)) ;
   DWORD dwDistanceLo(LODWORD(qwDistanceToMove)) ;
   dwDistanceLo = SetFilePointer(	
      hFile, 
	   dwDistanceLo, 
	   &lDistanceHi,
	   dwMoveMethod) ;

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
#endif

#endif