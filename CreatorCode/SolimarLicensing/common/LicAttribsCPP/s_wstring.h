// s_wstring.h : TODO: Add file description here.
//
#ifndef __s_wstring_h
#define __s_wstring_h
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <string>
#include <guiddef.h>

#ifdef _BORLAND_CKJD_ASLP_ASFE_
#include <stdlib.h>
#define  _wcstoui64( x,y,z )  ( _wtoi64(x) )
#define  _wcstoi64( x,y,z )  ( _wtoi64(x) )
#else
#pragma warning ( push ) // save warning settings to pre-compiler's "stack"
#pragma warning ( disable : 4244 )
#endif


/* class s_wstring
 * SLB 29.jan.04  Added support for time_t type(both constructor and casting operator).
 * SLB 17.aug.04  Changed default radix from 10 to 16.
---------------------------------------------------------------------------------------------------*/
class s_wstring : public std::wstring
{
public:
   s_wstring() : std::wstring(L"") 
   {
      m_nRadix = 16 ;
   }
   s_wstring(std::wstring wsVal, int nRadix=16 ) : std::wstring(wsVal)
   {

      m_nRadix = max(nRadix,2) ;
   }
   s_wstring(wchar_t *wcsVal, int nRadix=16 ) : std::wstring(wcsVal)
   {
      m_nRadix = max(nRadix,2) ;
   }

   // SLB - Need constructor for all int types so compiler does not have 
   // issues resolving operations where integer promotion can occur.
   s_wstring(long lVal, int nRadix=16) : std::wstring() 
   {
      m_nRadix = max(nRadix,2) ;
      *this = (__int64)lVal ;
   }
   s_wstring(unsigned long ulVal, int nRadix=16) : std::wstring() 
   {
      m_nRadix = max(nRadix,2) ;
      *this = (unsigned __int64)ulVal ;
   }

   s_wstring(unsigned __int8 nVal, int nRadix=16) : std::wstring() 
   {
      unsigned __int64 qwTmp(nVal) ;

      m_nRadix = max(nRadix,2) ;
      *this = qwTmp ;
   }
   s_wstring(__int8 nVal, int nRadix=16) : std::wstring() 
   {
      m_nRadix = max(nRadix,2) ;
      *this = __int64(nVal) ;
   }
   s_wstring(unsigned __int16 nVal, int nRadix=16) : std::wstring() 
   {
      unsigned __int64 qwTmp(nVal) ;

      m_nRadix = max(nRadix,2) ;
      *this = qwTmp ;
   }
   s_wstring(__int16 nVal, int nRadix=16) : std::wstring() 
   {
      m_nRadix = max(nRadix,2) ;
      *this = __int64(nVal) ;
   }

   s_wstring(unsigned __int32 nVal, int nRadix=16) : std::wstring() 
   {
      unsigned __int64 qwTmp(nVal) ;

      m_nRadix = max(nRadix,2) ;
      *this = qwTmp ;
   }
   s_wstring(__int32 nVal, int nRadix=16) : std::wstring() 
   {
      m_nRadix = max(nRadix,2) ;
      *this = __int64(nVal) ;
   }

   s_wstring(__int64 qiVal, int nRadix=16)
   {
      m_nRadix = max(nRadix,2) ;
      *this = qiVal ;
   }
   s_wstring(unsigned __int64 qwVal, int nRadix=16)
   {
      m_nRadix = max(nRadix,2) ;
      *this = qwVal ;
   }
   s_wstring(float flVal, int nRadix=16) : std::wstring() 
   {
      m_nRadix = max(nRadix,2) ;
      *this = (long double)flVal ;
   }
   s_wstring(double dbVal, int nRadix=16) : std::wstring() 
   {
      m_nRadix = max(nRadix,2) ;
      *this = (long double)dbVal ;
   }
   s_wstring(long double ldbVal, int nRadix=16) : std::wstring() 
   {
      m_nRadix = max(nRadix,2) ;
      *this = ldbVal ;
   }

	
	// assignment operators
   s_wstring & operator = ( const wchar_t *wcsVal )
   {
      s_wstring wsTemp( wcsVal, m_nRadix) ;
      *this = wsTemp ;
      return *this ;
   }
   s_wstring & operator = (__int64 qiVal)
   {  //
      // SLB 22.feb.2007 CR.7832; see note for __int64() operator.  Changed to
      // use the unsigned version of the int to string conversion.
      //
      wchar_t szVal[65+1+1] ; // max length of + 1 for sign + 1 for null term

#if _MSC_VER < 1400
      _ui64tow((unsigned __int64)qiVal, szVal, m_nRadix) ; // always converts to hex  
#else
      _ui64tow_s((unsigned __int64)qiVal, szVal, sizeof(szVal)/sizeof(wchar_t), m_nRadix) ; // always converts to hex  
#endif
      *this = s_wstring(szVal, m_nRadix) ;
      return *this ;
   }
   s_wstring & operator = (unsigned __int64 qwVal)
   {
      wchar_t szVal[65+1+1] ; // max length of + 1 for sign + 1 for null term

#if _MSC_VER < 1400
      _ui64tow(qwVal, szVal, m_nRadix) ; // always converts to hex 
#else
      _ui64tow_s(qwVal, szVal, sizeof(szVal)/sizeof(wchar_t), m_nRadix) ; // always converts to hex  
#endif
      *this = s_wstring(szVal, m_nRadix) ;
      return *this ;
   }


   s_wstring & operator = (long double ldbVal)
   {
      wchar_t szVal[_CVTBUFSIZE] ; // microsoft claims that this size is sufficient to store and value for double(note: using _gcvt())
		//
		// Using 12 significant digits
		//
#if _MSC_VER < 1400
		_snwprintf( szVal, sizeof(szVal)/sizeof(wchar_t), L"%.12f", ldbVal) ; 
#else
		_snwprintf_s( szVal, sizeof(szVal)/sizeof(wchar_t), _TRUNCATE, L"%.12f", ldbVal) ; 
#endif
		//
		// Note that radix is not used with floating point numbers.  It is always
		// 10.
		//
      *this = s_wstring(szVal, 10) ;
      return *this ;
   }
   // these all cause C4244 compiler warnings, possible loss of data
   operator bool () const
   {
      __int64 qiVal(0) ;

      qiVal = *this ;
      return qiVal != 0 ;
   }
   operator long() const
   {
      return (__int64)*this ;
   }

   operator unsigned long() const
   {
      return (unsigned __int64)*this ;
   }

   operator __int8() const
   {
      return (__int64)*this ;
   }

   operator unsigned __int8() const
   {
      return (unsigned __int64)*this ;
   }
   operator __int16() const 
   {
      return (__int64)*this ;
   }
   operator unsigned __int16() const
   {
      return (unsigned __int64)*this ;
   }
   operator __int32() const
   {
      return (__int64)*this ;
   }
   operator unsigned __int32() const
   {
      return (unsigned __int64)*this ;
   }
   operator __int64() const
   {  //
      // SLB 22.feb.2007 CR.7832; found that the string must be treated as unsigned data
      // otherwise, for negative numbers, the value is interpretted as an over-flow value
      // for the signed version of _wcstoui64().  Using the unsigned version works for signed  
      // values because the return value gets cast as signed (so if msb==1, the value is 
      // treated as negative.
      // 
      return (__int64)_wcstoui64(
         this->c_str(), // convert our string to wchar format
         NULL,            // don't care where scan stopped, so pass NULL here
         m_nRadix         // use radix specified(2-35 are valid values).
         ) ;
   }

   operator unsigned __int64() const
   {
      return _wcstoui64(  
         this->c_str(), 
         NULL, 
         m_nRadix
         ) ;
   }

	operator float() const 
	{ 
		return (long double)*this ; 
	}
	operator double() const 
	{ 
		return (long double)*this ; 
	}

   operator long double() const
   {
	   wchar_t *pwcStopped(NULL) ;

		return wcstod(	
         this->c_str(),				// convert our string to C string format(Null terminated)
			&pwcStopped					// scan stop position ptr gets put in here.
			) ;
   }

// Commented time_t/size_t out because this is same a type long.
//
   
   // extended type support - must add a constructor and casting operator for each new type added.
 //  s_wstring(time_t ttVal, int nRadix=10) : std::wstring() 
 //  {
 //     m_nRadix = nRadix ;
 //     *this = (__int64)ttVal ;
 //  }

	//operator time_t() const
 //  {
 //     return (unsigned __int64) *this ;
 //  }

 //  s_wstring(size_t ttVal, int nRadix=10) : std::wstring() 
 //  {
 //     m_nRadix = nRadix ;
 //     *this = (unsigned __int64)ttVal ;
 //  }

	//operator size_t() const
 //  {
 //     return (unsigned __int64) *this ;
 //  }
   void SetRadix(int nNewRadix) { m_nRadix = nNewRadix ; }
private:
   int m_nRadix ;
} ;
#ifdef _BORLAND_CKJD_ASLP_ASFE_
#else
#pragma warning ( pop )// restore warning settings from pre-compiler's "stack"
#endif

#endif
