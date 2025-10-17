// CSpdJobProperties.h : TODO: Add file description here.
//
#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT  (0x0601)
#endif
#define _WIN32_DCOM
#include <atlcomcli.h>
#include <atlsafe.h>
#include "SpdBaseDll.h"
#include "s_wstring.h"
#include "quadword.h"
#include <set>
#include <map>
#include <vector>
#include <assert.h>


namespace SpdJobProperty
{
class CJobPropertyMemberBase ;
}

namespace SpdStreamable
{
#define T_CHAR_LEN(buflen) (buflen/sizeof(wchar_t))

SPDBASE_IMP_EXP wchar_t *ReallocateWcsBuffer(
   wchar_t *pwcBufferToCopy, 
   size_t stBuffSize, 
   size_t &stNewBufSize
   ) ;


//*************************************************************************************
//
// The idea is that all primitive types must derive from CStreamable and
// define their own "ToString()" method.  This will allow more complex objects like
// lists and maps to be streamable (presuming they are defined with objects 
// which derive from CStreamable).
//
//*************************************************************************************
class SPDBASE_IMP_EXP CStreamable
{
public:
	virtual bool ToString(wchar_t *pwcNewVal, size_t stNewBufLen, size_t &stNewStringLength) const = 0 ;
   virtual bool InitFromString(const wchar_t *pwcVal) = 0 ;
   virtual void ToVARIANT(VARIANT *pvt) const = 0 ;
   virtual void InitFromVARIANT(const VARIANT *pvt) = 0 ;
   virtual void Clear() = 0 ;
   virtual size_t Size() const = 0 ;
   virtual bool IsEmpty() const = 0 ;
   //
	// Helper methods for derived classes.
	//
   static size_t GetNextXmlTagAndData(
      wchar_t *pwcsTagName,
      wchar_t *pwcsData,
      size_t stDataBufferSize,
      const wchar_t *pwcsIn
      ) ;
} ;

SPDBASE_IMP_EXP wchar_t * AllocateAndStreamProperty(const CStreamable &Property) ;


//*************************************************************************************
// 
// This template class is supposed to help the Streamable primitive types appear 
// transparent to code not concerned with its streamable nature.
//
//*************************************************************************************
template <class T>
class CStreamablePrimitiveBase : public CStreamable
{
public:
   typedef T contained_t ;
   typedef CStreamablePrimitiveBase<T> primitive_base_t ;

public:
	T m_tVal ;

public:
   CStreamablePrimitiveBase() {}
   CStreamablePrimitiveBase(const CStreamablePrimitiveBase<T> &c ) : m_tVal(c.m_tVal) {}
   CStreamablePrimitiveBase(const T &tVal ) : m_tVal(tVal) {}

   CStreamablePrimitiveBase<T> & operator = (const CStreamablePrimitiveBase<T> &c)
   {
      m_tVal = c.m_tVal ;
      return *this ;
   }

   T& operator = (const T &c) 
   { 
      m_tVal = c ;
      return m_tVal ;
   }

   bool ToString(wchar_t *pwcsNewBuf, size_t stNewBufLen, size_t &stNewStringLength) const 
   {
      bool bRetVal(false) ;

      bRetVal = ValueToString(m_tVal, pwcsNewBuf, stNewBufLen, stNewStringLength) ;
      if (!bRetVal)
         stNewStringLength = -1 ;
      return bRetVal ;
   }

   bool InitFromString(const wchar_t *pwcVal) 
   {
      bool bRetVal(false) ;

      bRetVal = StringToValue(pwcVal, m_tVal) ;
      return bRetVal ;
   }

   virtual size_t Size() const { return 0 ; }
   // 
	// Casting operators
	//
	operator const T &() const
   {
      return m_tVal ;
   }
	operator T &() 
   {
      return m_tVal ;
   }

   T* operator -> () 
   {
      return &m_tVal ;
   }

   const T* operator -> () const
   {
     return &m_tVal ;
   }
   T GetVal() { return m_tVal ; }
	bool operator !=(const CStreamablePrimitiveBase<T> &c) const { return compare(m_tVal, c.m_tVal)!=0  ; }
	bool operator ==(const CStreamablePrimitiveBase<T> &c) const { return compare(m_tVal, c.m_tVal)==0 ; }
	bool operator >(const CStreamablePrimitiveBase<T> &c) const { return compare(m_tVal, c.m_tVal)>0 ; }
	bool operator <(const CStreamablePrimitiveBase<T> &c) const { return compare(m_tVal, c.m_tVal)<0 ; }

private:
	virtual bool ValueToString(const T &tVal, wchar_t *pwcsNewBuf, size_t stNewBufLen, size_t &stNewStringLength) const = 0 ;
	virtual bool StringToValue(const wchar_t *pwcBuf, T &tOutVal) const = 0 ;
   virtual int compare(const T &tVal1, const T &tVal2) const = 0 ;
} ;


//*************************************************************************************
//
// This Template class is meant to handle concrete types which can be 
// subject to integer promotion. Specifically, there is a casting operator defined
// in s_wstring() for this type.  If there is no casting operator defined for s_wstring()
// then a new class should be written for the object.
//
//Needs to define the following:
//
//*************************************************************************************
#pragma warning (push) 
#pragma warning (disable : 4800) // disable performance warning when T=bool
template<class T, int nRadix>
class CStreamableInts : public CStreamablePrimitiveBase<T>
{
public:
	//
	// Required definition for part 1) constructors.
	//
	CStreamableInts() : CStreamablePrimitiveBase<T>() {}
   CStreamableInts(const CStreamableInts<T, nRadix> &c ) : CStreamablePrimitiveBase<T>(c) {}   
   CStreamableInts(const T &tVal ) : CStreamablePrimitiveBase<T>(tVal) {}

	//
	// Required definitions for part 2) Assignment operators.
	//
	CStreamableInts<T, nRadix> & operator = (const CStreamableInts<T, nRadix> &c)
   {
      primitive_base_t::operator =(c) ;
      return *this ;
   }

   T & operator = (const T &c) // assignment operator when type T is used
   {
      return primitive_base_t::operator =(c) ;
   }

   void Clear()
   {
      m_tVal = 0 ;
   }

   bool IsEmpty() const 
   {  
      //
      // integer types are considered empty when they have a zero value.
      //
      return false ; 
   }

	bool ValueToString(const T &tVal, wchar_t *pwcsNewBuf, size_t stNewBufLen, size_t &stNewStringLength) const 
   {
      bool bRetVal(false) ;
      //
      // initialize output
      //
      stNewStringLength = 0 ;
      if (stNewBufLen>=(65+1+1)) // max length of + 1 for sign + 1 for null term
      {
#if _MSC_VER < 1400
         _i64tow((QUADWORD)tVal, pwcsNewBuf, nRadix) ; // always converts to hex 
#else
         _i64tow_s((QUADWORD)tVal, pwcsNewBuf, stNewBufLen, nRadix) ; // always converts to hex  
#endif
         stNewStringLength = wcslen(pwcsNewBuf) ;
         bRetVal = true ;
      }
      return bRetVal ;
   }

	bool StringToValue(const wchar_t *pwcBuf, T &tOutVal) const 
   {
      bool bRetVal(true) ;

      tOutVal = (T)_wcstoi64(pwcBuf, NULL, nRadix) ;   
      return bRetVal ;
   }

   void ToVARIANT(VARIANT *pvt) const 
   {
      CComVariant vt(m_tVal) ; 

      vt.Detach(pvt) ;
   }
   void InitFromVARIANT(const VARIANT *pvt) 
   {
      switch (pvt->vt)
      {
      case VT_I1: 
      case VT_I2: 
      case VT_I4: 
      case VT_I8: 
         m_tVal = pvt->iVal ; break ;
      case VT_UI1:
      case VT_UI2:
      case VT_UI4:
      case VT_UI8:
         m_tVal = pvt->uiVal ; break ;
      case VT_BOOL:
         m_tVal = pvt->boolVal ; break ;
      default:
         m_tVal = 0 ;
         
      }
   }
   int compare(const T &tVal1, const T &tVal2) const 
   {
      int nRetVal(1) ;
      if (tVal1<tVal2) 
         nRetVal = -1 ;
      else if (tVal1==tVal2)
         nRetVal = 0 ;
      return nRetVal ;
   }
} ;
#pragma warning (pop)

//
// have to over ride this template method for types which don't want their type cast to 
// __int64 as the default method does.
//
template<>
void CStreamableInts <time_t, 16> ::ToVARIANT(VARIANT *pvt) const
{
   CComVariant vt((__int64)m_tVal) ; 

   vt.Detach(pvt) ;
}

template<>
void CStreamableInts <time_t, 10> ::ToVARIANT(VARIANT *pvt) const
{
   CComVariant vt((__int64)m_tVal) ; 

   vt.Detach(pvt) ;
}


//
// types which are streamed in hex
//
typedef CStreamableInts<unsigned __int64, 16> QwordHexObj ;
typedef CStreamableInts<unsigned long, 16> DwordHexObj ;
typedef CStreamableInts<unsigned __int16, 16> WordHexObj ;
typedef CStreamableInts<unsigned __int8, 16>  ByteHexObj ;
typedef CStreamableInts<__int32, 16> IntHexObj ;
typedef CStreamableInts<__int64, 16> QintHexObj ;
typedef CStreamableInts<__int16, 16> SintHexObj ;
typedef CStreamableInts<__int8, 16> CharHexObj ;

// 
// types which are streamed in decimal
//
typedef CStreamableInts<unsigned __int64, 10> QwordDecObj ;
typedef CStreamableInts<unsigned long, 10> DwordDecObj ;
typedef CStreamableInts<unsigned __int16, 10> WordDecObj ;
typedef CStreamableInts<unsigned __int8, 10>  ByteDecObj ;
typedef CStreamableInts<__int32, 10> IntDecObj ;
typedef CStreamableInts<__int64, 10> QintDecObj ;
typedef CStreamableInts<__int16, 10> SintDecObj ;
typedef CStreamableInts<__int8, 10> CharDecObj ;

//
// compatibility types (all are streamed in hex)
//
typedef QwordHexObj QwordObj ;
typedef DwordHexObj DwordObj ;
typedef WordHexObj WordObj ;
typedef ByteHexObj ByteObj ;
typedef IntHexObj IntObj ;
typedef QintHexObj QintObj ;
typedef SintHexObj SintObj ;
typedef CharHexObj CharObj;


typedef CStreamableInts<bool, 16> BoolObj ;
typedef CStreamableInts<time_t, 16> Time_tObj ;
typedef CStreamableInts<size_t, 16> Size_tObj ;

//
// helper macros to operate on GUIDs
//
#define GUID_QWREF0(guid) (((QUADWORD *) &guid)[0])
#define GUID_QWREF1(guid) (((QUADWORD *) &guid)[0])
//
// helper inline GUID methods.
//
__inline int InlineCompareGUID(const GUID &rguid1, const GUID &rguid2)
{  //
   // Note: arbitrarily use first QUADWORD of GUID as most Significant.
   // 
   int nRetVal ;
   if (GUID_QWREF0(rguid1) < GUID_QWREF0(rguid2))
      nRetVal = -1 ;
   else if (GUID_QWREF0(rguid1) > GUID_QWREF0(rguid2))
      nRetVal = 1 ;
   else
   {
      if (GUID_QWREF1(rguid1) < GUID_QWREF1(rguid2))
         nRetVal = -1 ;
      else if (GUID_QWREF1(rguid1) > GUID_QWREF1(rguid2))
         nRetVal = 1 ;
      else
         nRetVal = 0 ;
   }
   return nRetVal ;
}


__inline bool InlineClearGUID(GUID &rguid)
{
   GUID_QWREF0(rguid) = 0 ;
   GUID_QWREF1(rguid) = 0 ;
   return true; //BERT - not return a value
}

__inline bool InlineIsClearGUID(const GUID &rguid)
{
   return (
      GUID_QWREF0(rguid) == 0 &&
      GUID_QWREF1(rguid) == 0 
      ) ;
}

__inline bool InlineCopyGUID(GUID &rguid1, const GUID &rguid2)
{
   GUID_QWREF0(rguid1) = GUID_QWREF0(rguid2) ;
   GUID_QWREF1(rguid1) = GUID_QWREF1(rguid2) ;
}

class CStreamableGuid : public CStreamablePrimitiveBase<GUID>
{
public:
	//
	// Required definition for part 1) constructors.
	//
	CStreamableGuid() : CStreamablePrimitiveBase<GUID>() {}
   CStreamableGuid(const CStreamableGuid &c ) : CStreamablePrimitiveBase<GUID>(c) {}   
   CStreamableGuid(const GUID &tVal ) : CStreamablePrimitiveBase<GUID>(tVal) {}

	//
	// Required definitions for part 2) Assignment operators.
	//
	CStreamableGuid & operator = (const CStreamableGuid &c)
   {
      primitive_base_t::operator =(c.m_tVal) ;
      return *this ;
   }

   GUID & operator = (const GUID &c) // assignment operator when type T is used
   {
      InlineCopyGUID(m_tVal, c) ;
      return m_tVal ;
   }

   void Clear()
   {
      InlineClearGUID(m_tVal) ;
   }

   bool IsEmpty() const 
   {  //
      // GUID is considered empty when it is zeroed out.
      //
      return InlineIsClearGUID(m_tVal) ;
   }

	bool ValueToString(const GUID &tVal, wchar_t *pwcsNewBuf, size_t stNewBufLen, size_t &stNewStringLength) const 
   {
      bool bRetVal(false) ;
      //
      // initialize output
      //
      if (stNewBufLen<40)
      {
         stNewStringLength = 0 ;
      }
      else
      {
#if _MSC_VER < 1400
         stNewStringLength = _snwprintf(
            pwcsNewBuf,
            stNewBufLen,
            L"%08I64X-%08I64X",
            GUID_QWREF0(tVal),
            GUID_QWREF1(tVal)
            ) ;
#else
         stNewStringLength = _snwprintf_s(
            pwcsNewBuf,
            stNewBufLen,
			_TRUNCATE,
            L"%08I64X-%08I64X",
            GUID_QWREF0(tVal),
            GUID_QWREF1(tVal)
            ) ;
#endif
         bRetVal = true ;
      }
      return bRetVal ;
   }

	bool StringToValue(const wchar_t *pwcBuf, GUID &tOutVal) const 
   {
      bool bRetVal(true) ;
      wchar_t *pwcNext(NULL) ;

      GUID_QWREF0(tOutVal) = (QUADWORD)_wcstoi64(pwcBuf, &pwcNext, 16) ; 
      GUID_QWREF1(tOutVal) = (QUADWORD)_wcstoi64(pwcNext, NULL, 16) ; 
      return bRetVal ;
   }

   void ToVARIANT(VARIANT *pvt) const 
   {
      wchar_t wcsBuf[128] ;
      size_t stLength ;
      CComBSTR bstrVal ;
      
      ToString(wcsBuf, 128, stLength) ;
      bstrVal = wcsBuf ;
      bstrVal.CopyTo(pvt) ;
   }
   void InitFromVARIANT(const VARIANT *pvt)
   {
      InitFromString((wchar_t *)pvt->bstrVal) ;
   }
   int compare(const GUID &tVal1, const GUID &tVal2) const 
   {
      return InlineCompareGUID(tVal1, tVal2) ;
   }
} ;


class SPDBASE_IMP_EXP CSpdCStringBase 
{
public:
   const static size_t npos = (size_t)(-1) ;
public:
   int compare(const CSpdCStringBase &c, bool bCaseSensitiveOn) const
   {
      int nRetVal(0) ;
      //
      // SLB 10.oct.2005 - CR.4858 queue jobs sorting is not case independent.
      // It seems that case independency is a desireable for all string properties when
      // being sorted.  So, I have changed from using wmemcmp() to wcsnicmp().
      // SLB 11.oct.2005 - This class is not always used for job properties.  Changed to
      // do either case sensitive or case in-sensitive compares.
      //
      if (bCaseSensitiveOn)
         nRetVal = wmemcmp(c_str(), c.c_str(), min(wcslen(), c.wcslen())) ;
      else
         nRetVal = _wcsnicmp(c_str(), c.c_str(), min(wcslen(), c.wcslen())) ;
      if (!nRetVal && wcslen()!=c.wcslen())
      {
         nRetVal = wcslen() > c.wcslen() ? 1 : -1 ;
      }
      return nRetVal ;
   }
   int compare(const wchar_t *pwcs, bool bCaseSensitiveOn) const
   {
      int nRetVal(0) ;
      if (!pwcs)
         nRetVal = 1 ;
      else
      {
         //
         // SLB 10.oct.2005 - CR.4858 queue jobs sorting is not case independent.
         //
         if (bCaseSensitiveOn)
            nRetVal = wmemcmp(c_str(), pwcs, wcslen()) ;
         else
            nRetVal = _wcsnicmp(c_str(), pwcs, wcslen()) ;
      }
      return nRetVal ;
   }
   virtual size_t GetMaxLength() const = 0 ;
   virtual const wchar_t *c_str() const = 0 ;
   virtual size_t wcslen() const = 0 ;
   virtual void clear() = 0 ;
   virtual bool empty() const { return wcslen()==0 ; }
   virtual size_t find(const wchar_t *pwcs) const 
   { 
      bool bFound(false) ;
      size_t stFindPos(0) ;

      for (
         stFindPos = 0 ; 
         stFindPos<wcslen() && c_str()[stFindPos] && !bFound ; 
         stFindPos++
         )
      {
         size_t stCompPos(0) ;
         for (
            stCompPos = 0 ;
            (stFindPos+stCompPos)<GetMaxLength() && c_str()[stFindPos+stCompPos]==pwcs[stCompPos] && pwcs[stCompPos]!=0 ;
            stCompPos++
            )
         {
         }
         //
         // SLB 22.jun.2006; fix.  moved the following check out of inner loop.
         //
         if (!pwcs[stCompPos])
         {  //
            // at end of the string being searched for.
            // success: substring has been found.
            //
            bFound = true ;
            break ;
         }
      }
      if (!bFound)
         stFindPos = npos ;
      return stFindPos ;
   }
   virtual size_t find(const wchar_t wc, size_t stStartPos=0) const 
   {
      size_t stFindPos(npos) ;
      const wchar_t *pwcFound(NULL) ;

      if (stStartPos>=wcslen())
      {  //
         // problem: specified start position is beyond strings length.
         //
         assert(stStartPos<wcslen()) ;
      }
      else
      {
         pwcFound = wcschr(c_str()+stStartPos, wc) ;
         if (pwcFound)
            stFindPos = (size_t)(pwcFound - c_str()) ;
         else
            stFindPos = npos ;
      }
      return stFindPos ;
   }
   size_t find_last_of(wchar_t wc) const
   {
      size_t stFindPos ;

      for(
         stFindPos = wcslen()-1 ;
         stFindPos != npos && c_str()[stFindPos]!=wc ;
         stFindPos--
         )
      {
      }
      return stFindPos ;
   }
   size_t find_first_not_of(wchar_t wc) const
   {
      size_t stPos(0) ;
      wchar_t *pwcFound(NULL) ;

      for (
         stPos = 0 ;
         c_str()[stPos] != wc && stPos<wcslen() ;
         stPos++
         )
      {}
      if (stPos>=wcslen())
         stPos = npos ;
      return stPos ;
   }
   virtual CSpdCStringBase & assign(const CSpdCStringBase &c) { return assign1(c) ; }
   virtual CSpdCStringBase & assign(const wchar_t *pwcs, size_t stLen) { return assign2(pwcs, stLen) ; }
   virtual const wchar_t * assign(const wchar_t *pwcs) 
   { 
      if (pwcs)
      {
         size_t stStrLen = min(GetMaxLength(), ::wcslen(pwcs)) ;

         assign(pwcs, stStrLen) ;
      }
      else
      {
         clear() ;
      }
      return c_str() ;
   }
   virtual CSpdCStringBase & append(const CSpdCStringBase &a) = 0 ; 
   virtual void erase(size_t stPos, size_t stCount=npos) = 0 ;
   const wchar_t & operator[] (size_t stPos) const
   {
      assert(stPos<wcslen()) ;
      if (stPos>=wcslen())
      {
         stPos = 0 ;
      }
      return c_str()[stPos] ;
   }
   
   virtual wchar_t & operator[] (size_t stPos) = 0 ;
   virtual void substr(CSpdCStringBase &scsOutput, size_t stPos, size_t stCount) const = 0 ;
   void Clear()
   {
      clear() ;
   }

   bool IsEmpty() const 
   {  
      return empty() ; 
   }
   CSpdCStringBase & operator = (const CSpdCStringBase &c)
   {
      return assign(c) ;
   }
   const wchar_t * operator = (const wchar_t *pwcs)
   {
      return assign(pwcs) ;
   }
   CSpdCStringBase & operator += (const CSpdCStringBase &scs)
   {
      append(scs) ;
      return *this ;
   }

   bool operator < (const CSpdCStringBase &scs) const
   {
      bool bRetVal(compare(scs, false)<0) ;

      return bRetVal ;
   }

   bool operator > (const CSpdCStringBase &scs) const
   {
      bool bRetVal(compare(scs, false)>0) ;

      return bRetVal ;
   }

   bool operator == (const CSpdCStringBase &scs) const
   {
      bool bRetVal(compare(scs, false)==0) ;

      return bRetVal ;
   }

   bool operator == (const wchar_t *pwcs) const
   {
      bool bRetVal(compare(pwcs, false)==0) ;

      return bRetVal ;
   }

   bool operator != (const CSpdCStringBase &scs) const 
   {
      bool bRetVal(compare(scs, false)!=0) ;

      return bRetVal ;
   }

   bool operator != (const wchar_t *pwcs) const 
   {
      bool bRetVal(compare(pwcs, false)!=0) ;

      return bRetVal ;
   }
private:
   virtual CSpdCStringBase & assign1(const CSpdCStringBase &c) = 0 ;
   virtual CSpdCStringBase & assign2(const wchar_t *pwc, size_t stLen) = 0 ;
} ;

template <size_t stBufferSize>
class CSpdCStringFixed : public CSpdCStringBase
{
private:
   wchar_t m_wcsVal[stBufferSize] ;
   size_t m_stStrLen ;
public:
   CSpdCStringFixed() : m_stStrLen(0)
   {
      m_wcsVal[0] = 0 ;
   }
   CSpdCStringFixed(const CSpdCStringBase &c) : m_stStrLen(0)
   { 
      assign(c) ;
   }
   CSpdCStringFixed(const wchar_t *pwcs) : m_stStrLen(0)
   {
      assign(pwcs) ;
   }
   CSpdCStringBase & operator = (const CSpdCStringBase &c)
   {
      assign(c) ;
      return *this ;
   }
   const wchar_t * operator = (const wchar_t *pwcs)
   {
      return assign(pwcs) ;
   }

   wchar_t & operator[] (size_t stPos) 
   {
      assert(stPos<m_stStrLen) ;
      if (stPos>=m_stStrLen)
      {
         stPos = 0 ;
      }
      return m_wcsVal[stPos] ;
   }

   size_t GetMaxLength() const
   { 
      return stBufferSize - 1 ;
   }
   size_t wcslen() const { return m_stStrLen ; }
   const wchar_t *c_str() const { return m_wcsVal ; }
   void clear() { m_wcsVal[0] = 0 ; m_stStrLen = 0 ; }
   void erase(size_t stPos, size_t stCount=npos) 
   {
      if (stPos>=m_stStrLen)
      {  //
         // problem: starting position is outside of string's range.
         //
         assert(false) ;
      }
      else
      {
         if (stCount==npos)
         {  //
            // special case: erase rest of string after position.
            //
            m_wcsVal[stPos] = 0 ;
            m_stStrLen = stPos ;
         }
         else
         {
            stCount = min(m_stStrLen-stPos, stCount) ;
            wmemcpy(
               m_wcsVal+stPos, 
               m_wcsVal+stPos+stCount,
               m_stStrLen - (stPos+stCount)
               ) ;
            m_stStrLen -= stCount ;
         }
      }
   }

   void substr(CSpdCStringBase &scsOutput, size_t stPos, size_t stCount) const
   {
      size_t stCopySize(0) ;
      if (stPos>=wcslen())
      {  //
         // problem: requestedstarting position is beyond string length.
         //
         assert(stPos<wcslen()) ;
      }
      else if (stCount==npos)
      {
         stCopySize = wcslen()-stPos ;
      }
      else
      {
         stCopySize = stCount ;
      }
      scsOutput.assign(m_wcsVal+stPos, stCopySize) ;
   }

   CSpdCStringBase &append(const CSpdCStringBase &a) 
   {
      wmemcpy(m_wcsVal + m_stStrLen, a.c_str(), a.wcslen()) ;
      m_stStrLen += a.wcslen() ;
      m_wcsVal[m_stStrLen] = 0 ;
      return *this ;
   }

private:
   CSpdCStringBase & assign1(const CSpdCStringBase &c) 
   {
      m_stStrLen = min(GetMaxLength(), c.wcslen()) ;
      wmemcpy(m_wcsVal, c.c_str(), m_stStrLen) ;
      m_wcsVal[m_stStrLen] = 0 ; // ensures valud is always terminated.
      return *this ;
   }
   CSpdCStringBase & assign2(const wchar_t *pwc, size_t stLen)
   {
      if (pwc)
      {
         m_stStrLen = min(GetMaxLength(), stLen) ;
         wmemcpy(m_wcsVal, pwc, m_stStrLen) ;
      }
      else
      {
         m_stStrLen = 0 ;
      }
      m_wcsVal[m_stStrLen] = 0 ; // ensures value is always terminated.
      return *this ;
   }

} ;

template <size_t t_stMinBufSize>
class CSpdCStringGrowable : public CSpdCStringBase
{
private:
   wchar_t *m_pwcsVal ;
   size_t m_stBufLen ;
   size_t m_stStrLen ;
   const wchar_t m_wcsEmpty ;
public:
   CSpdCStringGrowable() : m_stStrLen(0), m_stBufLen(0), m_pwcsVal(NULL), m_wcsEmpty(0)
   {
   }
   ~CSpdCStringGrowable()
   {
      if (m_pwcsVal)
      {
         delete[](m_pwcsVal) ;
         m_pwcsVal = NULL ;
      }
      m_stBufLen = 0 ;
      m_stStrLen = 0 ;
   }
   CSpdCStringGrowable(const CSpdCStringBase &c) : m_stStrLen(0), m_stBufLen(0), m_pwcsVal(NULL), m_wcsEmpty(0)
   { 
      assign(c) ;
   }
   CSpdCStringGrowable(const CSpdCStringGrowable<t_stMinBufSize> &c) : m_stStrLen(0), m_stBufLen(0), m_pwcsVal(NULL), m_wcsEmpty(0)
   { 
      assign(c) ;
   }
   CSpdCStringGrowable(const wchar_t *pwcs) : m_stStrLen(0), m_stBufLen(0), m_pwcsVal(NULL), m_wcsEmpty(0)
   {
      assign(pwcs) ;
   }
   CSpdCStringBase & operator = (const CSpdCStringBase &c)
   {
      assign(c) ;
      return *this ;
   }
   CSpdCStringGrowable<t_stMinBufSize> & operator = (const CSpdCStringGrowable<t_stMinBufSize> &c)
   {
      assign(c) ;
      return *this ;
   }
   const wchar_t * operator = (const wchar_t *pwcs)
   {
      return assign(pwcs) ;
   }
   wchar_t & operator[] (size_t stPos) 
   {
      assert(stPos<m_stStrLen) ;
      if (stPos>=m_stStrLen)
      {
         stPos = 0 ;
      }
      return m_pwcsVal[stPos] ;
   }

   size_t GetMaxLength() const
   { 
      return (size_t)0x7FFFFFFF ;
   }
   size_t wcslen() const { return m_stStrLen ; }
   const wchar_t *c_str() const 
   { 
      if (empty())
         return &m_wcsEmpty ;
      else
         return m_pwcsVal ; 
   }
   void clear() 
   {  
      if (m_pwcsVal)
         m_pwcsVal[0] = 0 ; 
      m_stStrLen = 0 ; 
   }
   void erase(size_t stPos, size_t stCount=npos) 
   {
      if (stPos>=m_stStrLen)
      {  //
         // problem: starting position is outside of string's range.
         //
         assert(false) ;
      }
      else
      {
         if (stCount==npos)
         {  //
            // special case: erase rest of string after position.
            //
            m_pwcsVal[stPos] = 0 ;
            m_stStrLen = stPos ;
         }
         else
         {
            stCount = min(m_stStrLen-stPos, stCount) ;
            wmemcpy(
               m_pwcsVal+stPos, 
               m_pwcsVal+stPos+stCount,
               m_stStrLen - (stPos+stCount)
               ) ;
            m_stStrLen -= stCount ;
         }
      }
   }

   void substr(CSpdCStringBase &scsOutput, size_t stPos, size_t stCount) const
   {
      size_t stCopySize(0) ;
      if (stPos>=wcslen())
      {  //
         // problem: requestedstarting position is beyond string length.
         //
         assert(stPos<wcslen()) ;
      }
      else if (stCount==npos)
      {
         stCopySize = wcslen()-stPos ;
      }
      else
      {
         stCopySize = stCount ;
      }
      scsOutput.assign(m_pwcsVal+stPos, stCopySize) ;
   }

   CSpdCStringBase &append(const CSpdCStringBase &a) 
   {
      if (wcslen()+a.wcslen()+1>m_stBufLen)
      {
         ReAllocBuf(wcslen()+a.wcslen()+1) ;
      }
      wmemcpy(m_pwcsVal + m_stStrLen, a.c_str(), a.wcslen()) ;
      m_stStrLen += a.wcslen() ;
      m_pwcsVal[m_stStrLen] = 0 ;
      return *this ;
   }

private:
   CSpdCStringBase & assign1(const CSpdCStringBase &c) 
   {
      if (c.wcslen()+1>m_stBufLen)
      {
         m_stStrLen = 0 ; 
         ReAllocBuf(c.wcslen()+1) ;
      }
      m_stStrLen = c.wcslen() ;
      wmemcpy(m_pwcsVal, c.c_str(), m_stStrLen) ;
      if (m_pwcsVal)
         m_pwcsVal[m_stStrLen] = 0 ; // ensures value is always terminated.
      return *this ;
   }
   CSpdCStringBase & assign2(const wchar_t *pwcs, size_t stLen)
   {
      if (pwcs)
      {
         if (stLen+1>m_stBufLen)
         {
            m_stStrLen = 0 ; 
            ReAllocBuf(stLen+1) ;
         }
         m_stStrLen = stLen ;
         wmemcpy(m_pwcsVal, pwcs, m_stStrLen) ;
      }
      else
      {
         m_stStrLen = 0 ;
      }
      if (m_pwcsVal)
         m_pwcsVal[m_stStrLen] = 0 ; // ensures value is always terminated.
      return *this ;
   }
   void ReAllocBuf(size_t stMinBufLen) 
   {
      if (stMinBufLen<0)
      {
         assert(stMinBufLen>=0) ;
      }
      else if (stMinBufLen>m_stBufLen)
      {
         size_t stNewBufLen(max(t_stMinBufSize, stMinBufLen)) ;
         wchar_t *pwcsNewBuf = new wchar_t[stNewBufLen] ;

         if (m_pwcsVal)
         {
            wmemcpy(pwcsNewBuf, m_pwcsVal, m_stStrLen) ;
            pwcsNewBuf[m_stStrLen] = 0 ;
            delete[](m_pwcsVal) ;
         }
         m_pwcsVal = pwcsNewBuf ;
         m_stBufLen = stNewBufLen ;
      }
   }
} ;




typedef CSpdCStringFixed<16> CSpdCStringF16 ;
typedef CSpdCStringFixed<32> CSpdCStringF32 ;
typedef CSpdCStringFixed<64> CSpdCStringF64 ;
typedef CSpdCStringFixed<128> CSpdCStringF128 ;
typedef CSpdCStringFixed<256> CSpdCStringF256 ;
typedef CSpdCStringFixed<512> CSpdCStringF512 ;
typedef CSpdCStringFixed<2048> CSpdCStringF2048 ;
typedef CSpdCStringFixed<4096> CSpdCStringF4096 ;
typedef CSpdCStringFixed<8192> CSpdCStringF8192 ;
typedef CSpdCStringFixed<16384> CSpdCStringF16384 ;

typedef CSpdCStringGrowable<16> CSpdCStringG16 ;
typedef CSpdCStringGrowable<32> CSpdCStringG32 ;
typedef CSpdCStringGrowable<64> CSpdCStringG64 ;
typedef CSpdCStringGrowable<128> CSpdCStringG128 ;
typedef CSpdCStringGrowable<256> CSpdCStringG256 ;
typedef CSpdCStringGrowable<512> CSpdCStringG512 ;
typedef CSpdCStringGrowable<2048> CSpdCStringG2048 ;
typedef CSpdCStringGrowable<4096> CSpdCStringG4096 ;
typedef CSpdCStringGrowable<8192> CSpdCStringG8192 ;
typedef CSpdCStringGrowable<16384> CSpdCStringG16384 ;


//*************************************************************************************
//
// This class is meant to wrap C-style strings.
//
//*************************************************************************************
template <size_t stStringSize>
class CStreamableCString : public CStreamablePrimitiveBase< CSpdCStringGrowable<stStringSize> >
{
public:
	//
	// Required definition for part 1) constructors.
	//
	CStreamableCString() : CStreamablePrimitiveBase< CSpdCStringGrowable<stStringSize> >() {}
   CStreamableCString(const CStreamableCString &c) : CStreamablePrimitiveBase< CSpdCStringGrowable<stStringSize> >(c) {}   
   CStreamableCString(const CSpdCStringBase &tVal) : CStreamablePrimitiveBase< CSpdCStringGrowable<stStringSize> >(tVal) {}

	//
	// Required definitions for part 2) Assignment operators.
	//
	CStreamableCString & operator = (const CStreamableCString &c)
   {
      primitive_base_t::operator =(c) ;
      return *this ;
   }
   CSpdCStringGrowable<stStringSize> & operator = (const CSpdCStringBase &c) // assignment operator when type T is used
   {
      return primitive_base_t::operator =(c) ;
   }

   void Clear()
   {
      m_tVal.Clear() ;
   }

   bool IsEmpty() const 
   {  
      return m_tVal.IsEmpty() ; 
   }

	bool ValueToString(
      const CSpdCStringGrowable<stStringSize> &tVal, 
      wchar_t *pwcsNewBuf, 
      size_t stNewBufLen, 
      size_t &stNewStringLength
      ) const 
   {
      bool bRetVal(true) ;
      //
      // initialize output
      //
      stNewStringLength = 0 ;
      if (!pwcsNewBuf)
      {  //
         // problem: destination ptr is null.
         //
         assert(false) ;
         bRetVal = false ;
      }
      else
      {
         if (tVal.wcslen()>=stNewBufLen)
         {  //
            // problem: output buffer is not large enough to hold string + 0 terminator.
            //
            bRetVal = false ;
            stNewStringLength = (size_t)-1 ;
         }
         else
         {  
            int nNewStringLength(0) ;
            BOOL bRes(FALSE) ;
            //
            // escape the string
            //
            bRes = SpdUtils::XmlEscapeString( 
               tVal.c_str(), 
               pwcsNewBuf,
               stNewBufLen,
               stNewStringLength
               ) ;
            if (!bRes)
            {  //
               // problem: XmlEscapeString() failed.  most likely output buffer was too small.
               //
               bRetVal = false ;
            }
            else
            {
               bRetVal = true ;
            }
         }
      }
      return bRetVal ;
   }

	bool StringToValue(const wchar_t *pwcsBuf, CSpdCStringGrowable<stStringSize> &tOutVal) const 
   {
      bool bRetVal(true) ;
      
      if (!pwcsBuf)
      {  //
         // problem: input ptr is null.
         //
         assert(false) ;
         bRetVal = false ;
      }
      else
      {
         BOOL bRes(FALSE) ;
         size_t stStringLen(0) ;
         size_t stMaxStringLen(wcslen(pwcsBuf)) ;
         wchar_t *pwcsUnescaped = new wchar_t[stMaxStringLen+1] ;

         bRes = SpdUtils::XmlUnescapeString(
            pwcsBuf,
            pwcsUnescaped,
            tOutVal.GetMaxLength(),
            stStringLen
            ) ;
         if (!bRes)
         {  //
            // problem: AtlUnescapeUrl() failed.  Not enough room in buffer.
            //
            bRetVal = false ;
            tOutVal.clear() ;
         }
         else
         {
            tOutVal = pwcsUnescaped ;
         }
         delete[](pwcsUnescaped) ;
      }
      return bRetVal ;
   }

   void ToVARIANT(VARIANT *pvt) const 
   {
      CComBSTR bstrVal(m_tVal.c_str()) ;
      bstrVal.CopyTo(pvt) ;
   }
   void InitFromVARIANT(const VARIANT *pvt)
   {
      m_tVal = (wchar_t *)pvt->bstrVal ;
   }
   int compare(const CSpdCStringGrowable<stStringSize> &tVal1, const CSpdCStringGrowable<stStringSize> &tVal2) const 
   {  //
      // SLB 21.oct.2005 CR.4858; Job sorting is no longer case sensitive.
      // SLB 18.may.2007 CR.8240; Because string class is growable, must use the length of the largest string for 
      // this compare vs the Template's stStringSize value.
      //
      return _wcsnicmp(tVal1.c_str(), tVal2.c_str(), max(tVal1.wcslen(), tVal2.wcslen())) ;
   }
} ;

typedef CStreamableCString<16> WCString16Obj ;
typedef CStreamableCString<64> WCString64Obj ;
typedef CStreamableCString<128> WCString128Obj ;
typedef CStreamableCString<256> WCString256Obj ;
typedef CStreamableCString<512> WCString512Obj ;
typedef CStreamableCString<2048> WCString2048Obj ;
typedef CStreamableCString<4096> WCString4096Obj ;
typedef CStreamableCString<8192> WCString8192Obj ;
typedef CStreamableCString<16384> WCString16384Obj ;

//
// SLB 30.jun.2006 CR.6641; CStreamableSystemTime class now works strictly with FILETIME vs. SYSTEMTIME.
// This simplifies code and makes it more efficient.  SYSTEMTIME is used only in serialization/de-serialization
// methods.  Also, a new member was added to indicate whether serialized data is in local time or UTC format.
//
class CStreamableSystemTime : public CStreamable 
{
public: // public members
   FILETIME m_tVal ; 
   bool m_bIsStreamInLocalTime ;

public: // public methods
	//
	// Required definition for part 1) constructors.
	//
   CStreamableSystemTime() : m_bIsStreamInLocalTime(false) { memset(&m_tVal, 0, sizeof(m_tVal)) ; }
   CStreamableSystemTime(const CStreamableSystemTime &c) :
      m_bIsStreamInLocalTime(c.m_bIsStreamInLocalTime)
   {
      m_tVal = c.m_tVal ;
   }
   CStreamableSystemTime(const FILETIME &tVal) : m_bIsStreamInLocalTime(false) { m_tVal = tVal ; }
	//
	// Required definitions for part 2) Assignment operators.
	//
	CStreamableSystemTime & operator = (const CStreamableSystemTime &c)
   {
      m_tVal = c.m_tVal ;
      return *this ;
   }

   FILETIME & operator = (const FILETIME &c) // assignment operator when type T is used
   {
      m_tVal = c ;
      return m_tVal ;
   }

   void Clear()
   {
      m_tVal.dwLowDateTime = 0 ;
      m_tVal.dwHighDateTime = 0 ;
   }

   bool IsEmpty() const
   {
      if (m_tVal.dwLowDateTime != 0 || m_tVal.dwHighDateTime != 0) 
         return false ;
      return true ;
   }

   size_t Size() const
   {
      return 0 ;
   }

   operator FILETIME() const
   {
      return m_tVal ;
   }

   bool InitFromString(const wchar_t *pwcVal) 
   {
      bool bRetVal(false) ;

      bRetVal = StringToValue(pwcVal, m_tVal) ;
      return bRetVal ;
   }

	size_t ValueToString(const FILETIME &ftVal, wchar_t *pwcOut, size_t stOutBufLen) const
   {
      size_t stStringLength(0) ;
      SYSTEMTIME stVal ;
      FILETIME ftSystem(ftVal) ;

      if (m_bIsStreamInLocalTime)
      {
         FileTimeToLocalFileTime(&ftVal, &ftSystem) ;
      }

      FileTimeToSystemTime(&ftSystem, &stVal) ;
#if _MSC_VER < 1400
      stStringLength = _snwprintf(
         pwcOut,
         stOutBufLen,
         L"%04d-%02d-%02d %02d:%02d:%02d.%03d", 
         stVal.wYear,
         stVal.wMonth,
         stVal.wDay,
         stVal.wHour,
         stVal.wMinute,
         stVal.wSecond,
         stVal.wMilliseconds%1000 // CR.5729
         ) ;
#else
      stStringLength = _snwprintf_s(
         pwcOut,
         stOutBufLen,
		 _TRUNCATE,
         L"%04d-%02d-%02d %02d:%02d:%02d.%03d", 
         stVal.wYear,
         stVal.wMonth,
         stVal.wDay,
         stVal.wHour,
         stVal.wMinute,
         stVal.wSecond,
         stVal.wMilliseconds%1000 // CR.5729
         ) ;
#endif
      return stStringLength ;
   }

	bool ToString(wchar_t *pwcNewVal, size_t stNewBufLen, size_t &stNewStringLength) const
   {
	   stNewStringLength = ValueToString(m_tVal, pwcNewVal, stNewBufLen) ;
      return true ;
   }
   //
   // StringToValue() - SLB 15.jul.2005 Re-wrote this to be higher speed version at the 
   // cost of error checking.
   //
	bool StringToValue(const wchar_t *pwcBuf, FILETIME &ftOutVal) const 
   {
      bool bRetVal(true) ;
      SYSTEMTIME stVal ;
      wchar_t wcCpy[25] ;
      wchar_t *pwcNull(NULL) ;
      //
      // Make a working copy of the const input buffer.
      //
      wmemcpy(wcCpy, pwcBuf, 24) ;
      //
      // Set intermediate delimiters to NULL.
      //
      wcCpy[4] = wcCpy[7] = wcCpy[10] = wcCpy[13] = wcCpy[16] = wcCpy[19] = wcCpy[24] = 0 ;
      //
      // Now convert known fixed offsets to their appropriate values.
      //
      stVal.wYear =     (WORD)_wcstoi64(wcCpy+0 , &pwcNull, 10) ;
      stVal.wMonth =    (WORD)_wcstoi64(wcCpy+5 , &pwcNull, 10) ;
      stVal.wDay =      (WORD)_wcstoi64(wcCpy+8 , &pwcNull, 10) ;
      stVal.wHour =     (WORD)_wcstoi64(wcCpy+11, &pwcNull, 10) ;
      stVal.wMinute =   (WORD)_wcstoi64(wcCpy+14, &pwcNull, 10) ;
      stVal.wSecond =   (WORD)_wcstoi64(wcCpy+17, &pwcNull, 10) ;
      stVal.wMilliseconds = (WORD)_wcstoi64(wcCpy+20, &pwcNull, 10) % 1000 ;
      stVal.wDayOfWeek = 0 ;

      bRetVal = (FALSE != SystemTimeToFileTime(&stVal, &ftOutVal)) ;
      if (bRetVal && m_bIsStreamInLocalTime)
      {
         FILETIME ftTemp(ftOutVal) ;
         bRetVal = (FALSE != LocalFileTimeToFileTime(&ftTemp, &ftOutVal)) ;
      }
      return bRetVal ;
   }

   void ToVARIANT(VARIANT *pvt) const 
   {
      wchar_t wcsBuf[128] ;
      size_t stLength ;
      CComBSTR bstrVal ;
      
      ToString(wcsBuf, 128, stLength) ;
      bstrVal = wcsBuf ;
      bstrVal.CopyTo(pvt) ;
   }
   void InitFromVARIANT(const VARIANT *pvt)
   {
      InitFromString((wchar_t *)pvt->bstrVal) ;
   }
   int compare(const FILETIME &tVal1, const FILETIME &tVal2) const 
   {
      return CompareFileTime(&tVal1, &tVal2) ;
   }
   void SetToCurrentTime()
   {
      GetSystemTimeAsFileTime(&m_tVal) ;
   }
private: // private methods
   int compare(const CStreamableSystemTime &src) const
   {
      return compare(m_tVal, src.m_tVal) ;
   }
} ;

typedef CStreamableSystemTime SystemTimeObj ;


//*************************************************************************************
//
// This Template class presumes T is something like std::map<X,Y>.  Where Y must be 
// an object derived from CStreamable.
//
//Needs to define the following:
// 1) Needed Constructors
// 2) Needed Assignment operators
// 3) ValueToString() and StringToValue()
//
//*************************************************************************************
template <class T>
class CStreamableMaps : public CStreamablePrimitiveBase<T>
{

public:  // public methods
	//
	// Required definition for part 1) constructors.
	//
	CStreamableMaps() : CStreamablePrimitiveBase<T>() {}
   CStreamableMaps(const CStreamableMaps<T> &c) : CStreamablePrimitiveBase<T>(c)
   {
   } 
   CStreamableMaps(const T &tVal) : CStreamablePrimitiveBase<T>(tVal)
   {}

	//
	// Required definitions for part 2) Assignment operators.
	//
	CStreamableMaps<T> & operator = (const CStreamableMaps<T> &c)
   {
      CStreamablePrimitiveBase<T>::operator =(c) ;
      return *this ;
   }

   T & operator = (const T &tNewVal) // assignment operator when type T is used
   {
      return CStreamablePrimitiveBase<T>::operator =(tNewVal) ;
   }
   bool ValueToString(
      const T &tVal, 
      wchar_t *pwcsOutBuf, 
      size_t stOutBufSize, 
      size_t &stNewStringLength
      ) const
   {
      //
		// ValueToString()/StringToValue() are where the meat of this template lies.  
		// Class T must be of the following form: std::map<X,Y> where X & Y are derived from 
		//	CStreamable. Hence they have a the ToString() method defined.  This will generate a 
		// XML tag like stream.
		// 
		// Generates a stream like this:
		// <Map>
		//		<N><K>key</K><V>value</V></N>
		//		<N><K>key</K><V>value</V></N>
		//		<N><K>key</K><V>value</V></N>
		//				.
		//				.
		//		<N><K>key</K><V>value</V></N>
		// </Map>
      bool bRetVal(true) ;
      size_t stValBufSize(0x2000) ;
      wchar_t *pwcsValBuf(ReallocateWcsBuffer(NULL, stValBufSize, stValBufSize)) ; // allocates a growable buffer
      const wchar_t *pwcMapKey = L"Map" ;
	   T::const_iterator it (tVal.begin()) ;
      int nCpyLen(0) ;
      size_t stWriteLen(0) ;

      nCpyLen = _snwprintf(
         &pwcsOutBuf[stWriteLen],
         stOutBufSize - stWriteLen,
         L"<%s>",
         pwcMapKey
         ) ;
      if (nCpyLen<0)
      {  //
         // problem: not enough room for beginning tag.
         //
         pwcsOutBuf[0] = 0 ;
         bRetVal = false ;
      }
      else
      {
         stWriteLen += nCpyLen ;

		   while (it != tVal.end())
		   {  //
			   // for each node, generate appropriate XML.  
			   // 
            wchar_t wcsKeyBuf[64] ;
            size_t stKeyStringLen(0) ;
            size_t stValStringLen(0) ;

            nCpyLen = -1 ;
            //
            // fill key buffer
            //
            if (!it->first.ToString(wcsKeyBuf, 64, stKeyStringLen))
            {  //
               // serious problem: key name is longer than 64 chars.
               //
               pwcsOutBuf[0] = 0 ;
               bRetVal = false ;
               assert(false) ;
               break ;
            }
            else if (!it->second.ToString(pwcsValBuf, stValBufSize, stValStringLen) )
            {  //
               // not a problem: failed to stream value into value buffer...  
               // The buffer is to small, increase its size (and try again).
               //
               pwcsValBuf = ReallocateWcsBuffer(
                  pwcsValBuf, 
                  stValBufSize,
                  stValBufSize
                  ) ; 
            }
            else
            {
               nCpyLen = _snwprintf(
                  &pwcsOutBuf[stWriteLen],
                  stOutBufSize - stWriteLen,
                  L"<N><K>%s</K><V>%s</V></N>",
                  wcsKeyBuf,
                  pwcsValBuf
                  ) ;
               if (nCpyLen<0)
               {  //
                  // problem: filled up the buffer. 
                  // break out of the loop.
                  //
                  pwcsOutBuf[0] = 0 ;
                  bRetVal = false ;
                  break ;
               }
               else
               {
                  stWriteLen += nCpyLen ;
			         it++ ;
               }
            }
		   }

         //
         // Add </Map>
         //
         if (bRetVal)
         {
            nCpyLen = _snwprintf(
               &pwcsOutBuf[stWriteLen],
               stOutBufSize - stWriteLen,
               L"</%s>",
               pwcMapKey
               ) ;
            if (nCpyLen<0)
            {  //
               // problem: not enough space for the trailing tag
               //
               pwcsOutBuf[0] = 0 ;
               bRetVal = false ;
            }
            else
            {
               stWriteLen += nCpyLen ;
               stNewStringLength = stWriteLen ;
               pwcsOutBuf[stOutBufSize - 1] = 0 ; // ensure null termination.
            }
         }
      }
      if (pwcsValBuf)
         delete[] (pwcsValBuf) ;
      return bRetVal ;
   }

   void ToVARIANT(VARIANT *pvt) const 
   {
      T::const_iterator it(m_tVal.begin()) ;
      HRESULT hr1(S_OK), hr2(S_OK) ;
      LONG i(0) ;
      //
      // ATL setup stuff.
      //
      CComSafeArray<VARIANT> csaArray ;   // Create a container for the Safe array of variants
      CComSafeArrayBound bound[2] ;       // two dimensional SAFEARRAY
      //
      // need to create and initialize the dimensions of the ATL SAFEARRAY
      //
      bound[0].SetLowerBound(0) ;
      bound[0].SetCount((ULONG)m_tVal.size()) ;
      bound[1].SetLowerBound(0) ;
      bound[1].SetCount(2) ;
      hr1 = csaArray.Create(bound, 2) ; //  trying to build a a two dimensional safe array here.

      for (
         it = m_tVal.begin(), i=0 ;
         it != m_tVal.end() && SUCCEEDED(hr1) && SUCCEEDED(hr2);
         it++, i++
         )
      {
         LONG pIndices[2] = { i, 0 } ;
         CComVariant cvKey ;
         CComVariant cvVal ;

         it->first.ToVARIANT(&cvKey) ;
         it->second.ToVARIANT(&cvVal) ;
         
         hr1 = csaArray.MultiDimSetAt(pIndices, cvKey) ;
         pIndices[1] = 1 ; // increment least significant (right most) index from 0 to 1
         hr2 = csaArray.MultiDimSetAt(pIndices, cvVal) ;
      }
      pvt->parray = csaArray.Detach() ;
      pvt->vt = VT_ARRAY | VT_VARIANT ;
   }

   void InitFromVARIANT(const VARIANT *pvt) 
   {
      m_tVal.clear() ;
      if (!pvt || pvt->vt == VT_EMPTY)
      {  //
         // Not a problem: No properties are in the SAFEARRAY
         //
      }
      else if (pvt->vt != (VT_ARRAY|VT_VARIANT))
      {  //
         // Variant does not contain a Safe array of BSTRs.
         //
         assert(pvt->vt==(VT_ARRAY|VT_VARIANT)) ;
      }
      else
      {
         CComSafeArray<VARIANT> sa ;
         //
         // attach the ATL wrapper to the safe array 
         //
         sa.Attach(pvt->parray) ;
         //
         // Validate the dimensions of the array, should be 2-Dim.
         //
         if (
            sa.GetDimensions() != 2 //&& // must be 2 dimensions
            //sa.GetUpperBound(1) != 2 // 2nd dimension should only be 2 deep.
            )
         {  //
            // This is not a 2Dim array... No good.
            //
            assert(sa.GetDimensions()==2) ;
         }
         else
         { 
            ULONG i(0) ;  // loop counter
            //
            // All looks good with safe array
            // Go through safe array and build a properties map from it.
            //
            for (
               i=sa.GetLowerBound(0) ; 
               i<sa.GetCount(0); 
               i++
               )
            {
               HRESULT hr1(S_OK), hr2(S_OK) ;
               CComVariant vtKey ;
               CComVariant vtVal ;
               LONG pIndices[2] = {i,0} ; 

               hr1 = sa.MultiDimGetAt(pIndices, vtKey) ;
               pIndices[1] = 1 ;
               hr2 = sa.MultiDimGetAt(pIndices, vtVal) ;

               if (!SUCCEEDED(hr1) || !SUCCEEDED(hr2))
               {  //
                  // failed to read data from safe array
                  //
                  assert(SUCCEEDED(hr1)&&SUCCEEDED(hr2)) ;
                  break ;
               }
               else
               {
                  T::key_type key ;
                  T::mapped_type val ;

                  key.InitFromVARIANT(&vtKey) ;
                  val.InitFromVARIANT(&vtVal) ;
                  m_tVal.insert(std::make_pair(key, val)) ;
               }
            }
         }
         sa.Detach() ;
      }
   }
   virtual void Clear() 
   {
      m_tVal.clear() ;
   }

   bool IsEmpty() const
   {
      return m_tVal.empty() ;
   }

   virtual size_t Size() const
   { 
      return m_tVal.size() ; 
   }

	/* StringToValue() - ingests Xml stream and initializes object from it.
	 *
	 * SLB 20.aug.04 Changed format of Xml.  No longer puts count in xml.
	--------------------------------------------------------------------------------------------*/
	bool StringToValue(const wchar_t *pwcsBuf, T &tOutVal) const 
	{
      bool bRetVal(true) ;
	   const wchar_t *pwcsTagMap(L"Map") ;
	   const wchar_t *pwcsTagNode(L"N") ;
	   const wchar_t *pwcsTagKey(L"K") ;
	   const wchar_t *pwcsTagVal(L"V") ;

      size_t stNodeBufSize(0x1000) ;
      wchar_t *pwcsNodeBuf(ReallocateWcsBuffer(NULL, stNodeBufSize, stNodeBufSize)) ;
      size_t stValBufSize(0x1000) ;
      wchar_t *pwcsValBuf(ReallocateWcsBuffer(NULL, stValBufSize, stValBufSize)) ;
      size_t stLoc(0) ;
      //
      // clear output data
      //
      tOutVal.clear() ;
      
      while (pwcsBuf[stLoc] && pwcsBuf[stLoc]!=L'<')
         stLoc++ ;
      if (wcsncmp(pwcsBuf+stLoc+1, pwcsTagMap, wcslen(pwcsTagMap)))
      {  //
         // problem: did not begin with '<' char
         //
      }
      else
      {
         size_t stNodeLen(-1) ;
         wchar_t wcsTagName[64] ;
         //
         // increment location beyond beginning tag.
         //
         stLoc += wcslen(pwcsTagMap)+2 ;
         
         while (stNodeLen==-1)
         {
            stNodeLen = GetNextXmlTagAndData(
               wcsTagName,
               pwcsNodeBuf,
               stNodeBufSize,
               pwcsBuf + stLoc
               ) ;
            if (stNodeLen==-1)
            {  //
               // Not a problem: node data buffer was not large enough.
               // increase size of both the Node data buffer and the Value buffer.
               //
               pwcsNodeBuf = ReallocateWcsBuffer(pwcsNodeBuf, stNodeBufSize, stNodeBufSize) ;
               pwcsValBuf = ReallocateWcsBuffer(pwcsValBuf, stValBufSize, stValBufSize) ;
            }
            else if (!stNodeLen)
            {  //
               // Not a problem: no more node data.
               //
               bRetVal = true ;
               break ;
            }
            else
            {
               //
               // Now have Node data (hopefully!)
               //
               if (wcscmp(wcsTagName, pwcsTagNode))
               {  //
                  // problem: this is not a Node tag.
                  //
                  bRetVal = false ;
                  break ;
               }
               else
               {
                  wchar_t wcsKeyBuf[64] ;
                  size_t stKeyLen(0) ;
                  
                  stKeyLen = GetNextXmlTagAndData(
                     wcsTagName,
                     wcsKeyBuf,
                     64,
                     pwcsNodeBuf + 0
                     ) ;

                  if (wcscmp(wcsTagName, pwcsTagKey))
                  {  //
                     // problem: this is not a key tag
                     //
                     bRetVal = false ;
                     break ;
                  }
                  else
                  {
                     GetNextXmlTagAndData(
                        wcsTagName,
                        pwcsValBuf,
                        stValBufSize,
                        pwcsNodeBuf + stKeyLen
                        ) ;
                     if (wcscmp(wcsTagName, pwcsTagVal))
                     {  //
                        // problem: this is nota value tag.
                        //
                        bRetVal = false ;
                        break ;
                     }
                     else
                     {  //
                        // Ok: now have Key value pair.  
                        // Add it to list.
                        //
					         T::key_type ktKey ;
					         T::mapped_type mtValue ;

					         ktKey.InitFromString(wcsKeyBuf) ;
					         mtValue.InitFromString(pwcsValBuf) ;
					         tOutVal.insert(std::make_pair(ktKey,mtValue)) ;
                        stLoc += stNodeLen ;
                        stNodeLen = -1 ;
                     }
                  }
               }
            }
         }
      }
      if (pwcsNodeBuf)
         delete[] (pwcsNodeBuf) ;
      if (pwcsValBuf)
         delete[] (pwcsValBuf) ;
      return bRetVal ;
   }
   int compare(const T &tVal1, const T&tVal2) const
   {
      return 0 ;
   }
} ;



typedef std::map< WCString64Obj, WCString256Obj > TMapStringString ;
typedef std::map< WCString64Obj, DwordObj > TMapStringDword ;
typedef std::map< DwordObj, WCString256Obj > TMapDwordString ;
typedef std::map< DwordObj, DwordObj > TMapDwordDword ;
//typedef std::map< WCString64Obj, BufferObj > TMapStringBuffer ;
//typedef std::map< BufferObj, BufferObj, std::less<CBuffer> > TMapBufferBuffer ;

typedef CStreamableMaps< TMapStringString > MapStringStringObj ;
typedef CStreamableMaps< TMapStringDword > MapStringDwordObj ;



//*************************************************************************************
//
// This Template class presumes T is something like std::vector<X,Y>.  Where Y must be 
// an object derived from CStreamable.
//
//Needs to define the following:
// 1) Needed Constructors
// 2) Needed Assignment operators
// 3) ValueToString() and StringToValue()
//
//*************************************************************************************
template <class T>
class CStreamableVectors : public CStreamablePrimitiveBase<T>
{

public:  // public methods
	//
	// Required definition for part 1) constructors.
	//
	CStreamableVectors() : CStreamablePrimitiveBase<T>() {}
   CStreamableVectors(const CStreamableVectors<T> &c) : CStreamablePrimitiveBase<T>(c)
   {
   } 
   CStreamableVectors(const T &tVal) : CStreamablePrimitiveBase<T>(tVal)
   {}

	//
	// Required definitions for part 2) Assignment operators.
	//
	CStreamableVectors<T> & operator = (const CStreamableVectors<T> &c)
   {
      CStreamablePrimitiveBase<T>::operator =(c) ;
      return *this ;
   }

   T & operator = (const T &tNewVal) // assignment operator when type T is used
   {
      return CStreamablePrimitiveBase<T>::operator =(tNewVal) ;
   }
   virtual void Clear() 
   {
      m_tVal.clear() ;
   }

   bool IsEmpty() const
   {
      return m_tVal.empty() ;
   }

   virtual size_t Size() const
   { 
      return m_tVal.size() ; 
   }

   bool ValueToString(
      const T &tVal, 
      wchar_t *pwcsOutBuf, 
      size_t stOutBufSize, 
      size_t &stNewStringLength
      ) const
	{	//
		// ValueToString()/StringToValue() are where the meat of this template lies.  
		// Class T must be of the following form: std::vector<X> where X is derived from 
		//	CStreamable. Hence they have a the ToString() method defined.  This will generate a 
		// XML tag like stream.
		// 
		// Generates a stream like this:
		// <Vector>
		//		<N>value</N>
		//		<N>value</N>
		//		<N>value</N>
		//				.
		//				.
		//		<N>value</N>
		// </Vector>
		//
      bool bRetVal(true) ;
      size_t stValStringLen(0) ;
      size_t stValBufSize(0x2000) ;
      wchar_t *pwcsValBuf(ReallocateWcsBuffer(NULL, stValBufSize, stValBufSize)) ; // allocates a growable buffer
      const wchar_t *pwcVectorKey = L"Vector" ;
	   T::const_iterator it (tVal.begin()) ;
      int nCpyLen(0) ;
      size_t stWriteLen(0) ;

      nCpyLen = _snwprintf(
         &pwcsOutBuf[stWriteLen],
         stOutBufSize - stWriteLen,
         L"<%s>",
         pwcVectorKey
         ) ;
      if (nCpyLen<0)
      {  //
         // problem: not enough space for the starting tag.
         //
         bRetVal = false ;
         pwcsOutBuf[0] = 0 ;
      }
      else
      {
         stWriteLen += nCpyLen ;

		   while (it != tVal.end())
		   {  //
			   // for each node, generate appropriate XML.  
			   // 
            if (!it->ToString(pwcsValBuf, stValBufSize, stValStringLen))
            {  //
               // not a problem: failed to stream value into value buffer...  
               // The buffer is to small, increase its size.
               //
               pwcsValBuf = ReallocateWcsBuffer(
                  pwcsValBuf, 
                  stValBufSize,
                  stValBufSize
                  ) ; 
            }
            else
            {
               nCpyLen = _snwprintf(
                  &pwcsOutBuf[stWriteLen],
                  stOutBufSize - stWriteLen,
                  L"<N>%s</N>",
                  pwcsValBuf
                  ) ;
               if (nCpyLen<0)
               {  //
                  // problem: filled up the buffer. 
                  //
                  bRetVal = false ;
                  pwcsOutBuf[0] = 0 ;
                  break ; 
               }
               else
               {
                  stWriteLen += nCpyLen ;
			         it++ ;
               }
            }
		   }

         //
         // Add </Vector>
         //
         if (bRetVal)
         {
            nCpyLen = _snwprintf(
               &pwcsOutBuf[stWriteLen],
               stOutBufSize - stWriteLen,
               L"</%s>",
               pwcVectorKey
               ) ;
            if (nCpyLen<0)
            {  //
               // problem: not enough room for the trailing tag
               //
               bRetVal = false ;
               pwcsOutBuf[0] = 0 ;
            }
            else
            {
               stWriteLen += nCpyLen ;
               stNewStringLength = stWriteLen ;
               pwcsOutBuf[stOutBufSize - 1] = 0 ; // ensure null termination.
            }
         }
      }
      if (pwcsValBuf)
         delete[] (pwcsValBuf) ;
      return bRetVal ;
   }

	/* StringToValue() - ingests Xml stream and initializes object from it.
	 *
	 * SLB 20.aug.04 Changed format of Xml.  No longer puts count in xml.
	--------------------------------------------------------------------------------------------*/
	bool StringToValue(const wchar_t *pwcsBuf, T &tOutVal) const 
	{
      bool bRetVal(true) ;
	   const wchar_t *pwcsTagVector(L"Vector") ;
	   const wchar_t *pwcsTagNode(L"N") ;

      size_t stNodeBufSize(0x1000) ;
      wchar_t *pwcsNodeBuf(ReallocateWcsBuffer(NULL, stNodeBufSize, stNodeBufSize)) ;
      size_t stLoc(0) ;
      //
      // clear output data
      //
      tOutVal.clear() ;
      
      while (pwcsBuf[stLoc] && pwcsBuf[stLoc]!=L'<')
         stLoc++ ;
      if (wcsncmp(pwcsBuf+stLoc+1, pwcsTagVector, wcslen(pwcsTagVector)))
      {  //
         // problem: did not begin with '<' char
         //
      }
      else
      {
         size_t stNodeLen(-1) ;
         wchar_t wcsTagName[64] ;
         //
         // increment location beyond beginning tag.
         //
         stLoc += wcslen(pwcsTagVector)+2 ;
         
         while (stNodeLen==-1)
         {
            stNodeLen = GetNextXmlTagAndData(
               wcsTagName,
               pwcsNodeBuf,
               stNodeBufSize,
               pwcsBuf + stLoc
               ) ;
            if (stNodeLen==-1)
            {  //
               // Not a problem: node data buffer was not large enough.
               // increase size of both the Node data buffer and the Value buffer.
               //
               pwcsNodeBuf = ReallocateWcsBuffer(pwcsNodeBuf, stNodeBufSize, stNodeBufSize) ;
            }
            else if (!stNodeLen)
            {  //
               // Not a problem: no more node data.
               //
               bRetVal = true ;
               break ;
            }
            else
            {
               //
               // Now have Node data (hopefully!)
               //
               if (wcscmp(wcsTagName, pwcsTagNode))
               {  //
                  // problem: this is not a Node tag.
                  //
                  bRetVal = false ;
                  break ;
               }
               else
               {  //
                  // Add it to list.
                  //
						T::value_type vtValue ;

						vtValue.InitFromString(pwcsNodeBuf) ;
						tOutVal.push_back(vtValue ) ;
                  stLoc += stNodeLen ;
                  stNodeLen = -1 ;
               }
            }
         }
      }
      if (pwcsNodeBuf)
         delete[] (pwcsNodeBuf) ;
      return bRetVal ;
   }
   void ToVARIANT(VARIANT *pvt) const 
   {
      T::const_iterator it(m_tVal.begin()) ;
      HRESULT hr(S_OK) ;
      LONG i(0) ;
      //
      // ATL setup stuff.
      //
      CComSafeArray<VARIANT> csaArray((DWORD)m_tVal.size()) ;   // Create a container for the Safe array of variants
      for (
         it = m_tVal.begin(), i=0 ;
         it != m_tVal.end() && SUCCEEDED(hr) ;
         it++, i++
         )
      {
         CComVariant cvVal ;

         it->ToVARIANT(&cvVal) ;
         hr = csaArray.SetAt(i, cvVal) ;
      }
      pvt->parray = csaArray.Detach() ;
      pvt->vt = VT_ARRAY|VT_VARIANT ;
   }

   void InitFromVARIANT(const VARIANT *pvt) 
   {
      m_tVal.clear() ;
      if (!pvt || pvt->vt == VT_EMPTY)
      {  //
         // Not a problem: No properties are in the SAFEARRAY
         //
      }
      else if (pvt->vt != (VT_ARRAY|VT_VARIANT))
      {  //
         // Variant does not contain a Safe array of BSTRs.
         //
         assert(pvt->vt==(VT_ARRAY|VT_VARIANT)) ;
      }
      else
      {  //
         // define and attach the ATL wrapper to the safe array 
         //
         CComSafeArray<VARIANT> sa(pvt->parray) ;
         //
         // Validate the dimensions of the array, should be 1-Dim.
         //
         if (
            sa.GetDimensions() != 1 //&& // must be 2 dimensions
            )
         {  //
            // This is not a 1Dim array... No good.
            //
            assert(sa.GetDimensions()==1) ;
         }
         else
         { 
            ULONG i(0) ;  // loop counter
            //
            // All looks good with safe array
            // Go through safe array and build a properties map from it.
            //
            for (
               i=sa.GetLowerBound(0) ; 
               i<sa.GetCount(0); 
               i++
               )
            {
               HRESULT hr(S_OK) ;
               CComVariant vtVal ;
               T::value_type val ;

               vtVal = sa.GetAt(i) ;
               val.InitFromVARIANT(&vtVal) ;
               m_tVal.push_back(val) ;
            }
         }
         sa.Detach() ;
      }
   }
   int compare(const T &tVal1, const T&tVal2) const
   {
      return 0 ;
   }
} ;

typedef std::vector< WCString64Obj > TVectorString ;
typedef std::vector< DwordObj > TVectorDword ;
//typedef std::vector< WCString64Obj, BufferObj > TVectorStringBuffer ;
//typedef std::vector< BufferObj, BufferObj, std::less<CBuffer> > TVectorBufferBuffer ;

typedef CStreamableVectors< TVectorString > VectorStringObj ;
typedef CStreamableVectors< TVectorDword > VectorDwordObj ;



/*--------------------------------------------------------------------*
 *                                                                    *
 *   CSpdPropertyContainer Class:                                   *
 *                                                                    *
 *                                                                    *
 *   TODO: Add description for class.
 *                                                                    *
 *--------------------------------------------------------------------*/
class SPDBASE_IMP_EXP CPropertyContainer : public CStreamable
{
public: 
   //
   // note: this member is controlled by containing object. 
   // It should not be overwritten by copy constructors or assignment operators.
   //
   bool m_bStreamUsesContainerName ; 

public:
   CPropertyContainer() : 
      m_bStreamUsesContainerName(true)
   {}
   CPropertyContainer(const CPropertyContainer &c) :
      m_bStreamUsesContainerName(c.m_bStreamUsesContainerName)
   {}
   CPropertyContainer & operator =(const CPropertyContainer &c)
   {
      m_bStreamUsesContainerName = c.m_bStreamUsesContainerName ;
      return *this ;
   }
   virtual const wchar_t *GetContainerName() const = 0 ;
   virtual const SpdJobProperty::CJobPropertyMemberBase * GetPropertyObjectFromIndex(DWORD n) const = 0 ;

	virtual bool ToString(wchar_t *pwcNewVal, size_t stNewBufLen, size_t &stNewStringLength) const ;
   virtual bool InitFromString(const wchar_t *pwcVal) ;
   void InitFromVARIANT(const VARIANT *pvt) ;
   void ToVARIANT(VARIANT *pvt) const ;
   virtual void Clear() ;
   virtual bool IsEmpty() const ;
   virtual size_t Size() const ;
   bool AreAnyPropertiesModified() const ;
   DWORD GetModifiedPropertiesCount() const ;
   bool AreAnyModifiedPropertiesReadOnly() const ;
   void AssignModifiedPropertiesOnly(const CPropertyContainer& src) ;
   SpdJobProperty::CJobPropertyMemberBase *GetPropertyObjectFromKey(const wchar_t *pwcsKey) const ;
   DWORD GetPropertyIndexFromName(const wchar_t *pwcsPropertyName) const ;
   void GetPropertyNameFromIndex(
      CSpdCStringBase &scsPropertyName, 
      DWORD dwPropertyObjectIndex
      ) ; 
   bool IsValidPropertyName(const wchar_t *pwcsPropertyName) const ;
   bool IsPropertyReadOnly(const wchar_t *pwcsPropertyName) const ;
   void SetPropertyFromVARIANT(
      const wchar_t *pwcsPropertyName,
      const VARIANT &vtPropertyVal
      ) ;
   void SetPropertyFromStringWithTags(
      const wchar_t *pwcsPropertyName, 
      const wchar_t *pwcsPropertyVal
      ) ; 
   void SetPropertyFromStringNoTags(
      const wchar_t *pwcsPropertyName, 
      const wchar_t *pwcsPropertyVal
      ) ; 
   void GetPropertyStringWithTags(
      const wchar_t *pwcsPropertyName, 
      wchar_t *pwcOutPropertyVal, 
      size_t stOutBufLen
      ) const ;
   void GetPropertyStringNoTags(
      const wchar_t *pwcsPropertyName, 
      wchar_t *pwcsOutPropertyVal, 
      size_t stOutBufLen
      ) const ;
   int compare(const CPropertyContainer &cmp) const ;
} ;

} // end of namespace SpdStreamable
