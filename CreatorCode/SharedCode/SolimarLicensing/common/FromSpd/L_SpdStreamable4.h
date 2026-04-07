//
// SpdStreamable.h 
//
// SLB 26.sep.2011 CR.14962; converted all assert() calls to sol_assert() calls.
//
#pragma once

#ifndef _SpdStreamable4_h																				// CR.32662.V04; added.
#define _SpdStreamable4_h																				// CR.32662.V04; added.

#if 0
#ifndef NTDDI_VERSION
#define NTDDI_VERSION NTDDI_WINXP	//Version Constants for Kernel-Mode Drivers
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT  0x0501
#endif
#endif

#define _WIN32_DCOM
#include "L_SSILib1Dll.h"                                                             // CR.13870; modified.
#include <atlcomcli.h>
#include <atlsafe.h>
//#include "s_wstring.h"
#include "L_quadword.h"
#include <set>
#include <map>
#include <vector>
#include <assert.h>
#include "L_SpdException.h"

#ifndef WIDEN
#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)
#define __WFUNCTION__ WIDEN(__FUNCTION__)
#endif

#pragma warning(disable: 4091)  /* 'typedef ': ignored when no variable is declared */ // CR.24131; added.

namespace SpdJobProperty
{
class CJobPropertyMemberBase; 
}


namespace SpdStreamable
{

/*
 * class CSpdCStringBase
 *
 * SLB 13.Sep.2018 CR.18557; Fix for methods find_last_of() and find_first_not_of(). One was not working,
 * changed the other so code logic matched.
 */
class SSILIB1_EXPORT CSpdCStringBase                                                // CR.13870; Now uses SSILIB1_EXPORT to define linkage.
{
public:
   const static size_t npos = (size_t)(-1); 
public:
   int compare(const CSpdCStringBase &c, bool bCaseSensitiveOn) const
   {
      // SLB 10.oct.2005 - CR.4858 queue jobs sorting is not case independent.
      // It seems that case independency is a desireable for all string properties when
      // being sorted.  So, I have changed from using wmemcmp() to wcsnicmp().
      // SLB 11.oct.2005 - This class is not always used for job properties.  Changed to
      // do either case sensitive or case in-sensitive compares.
      // SLB 20.jul.2016 CR.21572; Tweaked to optimize. This method gets called a lot.
      int nRes; 
      if (bCaseSensitiveOn) nRes = wmemcmp(c_str(), c.c_str(), min(wcslen(), c.wcslen())); 
      else nRes = _wcsicmp(c_str(), c.c_str());                                    // CR.21448; modified.
      if (nRes) return nRes; 

      // strings appear equal via wmemcmp(). Attempt tie breaker using string length.
      if (wcslen() != c.wcslen()) return (wcslen() > c.wcslen() ? 1 : -1); 

      return 0; 
   }
   int compare(const wchar_t *pwcs, bool bCaseSensitiveOn) const
   {
      if (!pwcs) return 1; 

      int nPwcsLen((int)::wcslen(pwcs));

      // SLB 18.jul.2018 CR.17862.V08; Fix for always returning 0 (equal) when contained value is an emptry string.
      if (empty())                                                                  // CR.17862.V08; added.
      {
         if (pwcs[0] == 0) return 0;
         return -1;
      }

      // SLB 10.oct.2005 - CR.4858 queue jobs sorting is not case independent.
      // SLB 15.aug.2019 CR.28242; Identified a potential Access Violation when using wmemcmp(). Decided to 
      // go with wcsncmp() because it returns the correct value based upon string lengths as well. 
      // Note: This could be applied to other version of compare(), but it does not have the same bug and 
      // it has gone through performance optimization and revalidating performance is out of the scope of this
      // project/CR.
      if (bCaseSensitiveOn)
         return wcsncmp(c_str(), pwcs, max((int)wcslen(), nPwcsLen));

      return _wcsnicmp(c_str(), pwcs, max((int)wcslen(), nPwcsLen)); 
   }
   virtual size_t GetMaxLength() const = 0; 
   virtual const wchar_t *c_str() const = 0; 
   virtual size_t wcslen() const = 0; 
   size_t size() const { return wcslen(); }
   virtual void clear() = 0; 
   virtual bool empty() const { return wcslen() == 0; }
   virtual size_t find(const wchar_t *pwcs) const 
   { 
      bool bFound(false); 
      size_t stFindPos(0); 

      for (stFindPos = 0; stFindPos < wcslen() && c_str()[stFindPos] && !bFound; stFindPos++)
      {
         size_t stCompPos(0); 
         for (stCompPos = 0; (stFindPos+stCompPos)<GetMaxLength() && c_str()[stFindPos+stCompPos]==pwcs[stCompPos] && pwcs[stCompPos] != 0; stCompPos++)
         {
         }

         // SLB 22.jun.2006; fix.  moved the following check out of inner loop.
         if (!pwcs[stCompPos])
         {  
            // at end of the string being searched for.
            // success: substring has been found.
            bFound = true; 
            break; 
         }
      }
      if (!bFound)
         stFindPos = npos; 
      return stFindPos; 
   }
   virtual size_t find(const wchar_t wc, size_t stStartPos = 0) const 
   {
      size_t stFindPos(npos); 
      const wchar_t *pwcFound(NULL); 

      if (stStartPos>=wcslen())
      {  //
         // problem: specified start position is beyond strings length.
         //
         //assert(stStartPos<wcslen()); 
         sol_assert(false);                                                         // CR.14962; modified.
      }
      else
      {
         pwcFound = wcschr(c_str()+stStartPos, wc); 
         if (pwcFound)
            stFindPos = (size_t)(pwcFound - c_str()); 
         else
            stFindPos = npos; 
      }
      return stFindPos; 
   }
   size_t find(const CSpdCStringBase &wsFind, size_t stStartPos = 0) const
   {
      if (size() < wsFind.size()) return npos; 
      for (size_t i(stStartPos);  i <= size()-wsFind.size(); i++)
      {
         if (!wcsncmp(c_str()+i, wsFind.c_str(), wsFind.size())) return i; 
      }
      return npos; 
   }
   size_t find_first_of(wchar_t wc, size_t stOffset = 0) const
   {
      if (wcslen() == 0) return npos;
      for (size_t stPos = stOffset; stPos < wcslen(); stPos++)
      {
         if (c_str()[stPos] == wc) return stPos; 
      }
      return npos;
   }
   size_t find_last_of(wchar_t wc, size_t stOffset=npos) const
   {
      if (wcslen() == 0) return npos;

      if (stOffset == npos) stOffset = wcslen() - 1;
      for (size_t stPos = stOffset; true; stPos--)                                 // CR.18557; modified.
      {
         if (c_str()[stPos] == wc) return stPos;                                    // CR.18557; added.
         if (stPos == 0) return npos;
      }
      return npos;
   }
   size_t find_first_not_of(wchar_t wc, size_t stOffset = 0) const
   {
      if (wcslen() == 0) return npos;
      for (size_t stPos = stOffset;  stPos < wcslen();  stPos++)                    // CR.18557; modified.
      {
         if (c_str()[stPos] != wc) return stPos;                                    // CR.18557; added.
      }
      return npos;
   }
   virtual CSpdCStringBase & assign(const CSpdCStringBase &c) { return assign1(c); }
   virtual CSpdCStringBase & assign(const wchar_t *pwcs, size_t stLen) { return assign2(pwcs, stLen); }
   virtual const wchar_t * assign(const wchar_t *pwcs, const wchar_t *pwcs2=NULL) 
   { 
      if (!pwcs) 
      { 
         clear(); 
         return c_str(); 
      }

      size_t stStrLen(min(GetMaxLength(), (size_t)(pwcs2 - pwcs)));
      if (!pwcs2)
         stStrLen = min(GetMaxLength(), ::wcslen(pwcs)); 
      assign2(pwcs, stStrLen);
      return c_str(); 
   }

   virtual const wchar_t * assign(wchar_t wc)
   {
      wchar_t pwcs[2];
      pwcs[0] = wc; pwcs[1] = 0;

      assign2(pwcs, 1);
      return c_str();
   }

   virtual CSpdCStringBase & append(const CSpdCStringBase &a) = 0;  
   virtual CSpdCStringBase & append(const std::wstring &a) = 0;  
   virtual CSpdCStringBase & append(wchar_t wc) = 0;  
   virtual void erase(size_t stPos, size_t stCount=npos) = 0; 
   const wchar_t & operator[] (size_t stPos) const
   {
      //assert(stPos<wcslen()); 
      if (!(stPos < wcslen()))
         sol_assert(false);                                                         // CR.14962; modified.
      if (stPos >= wcslen())
      {
         stPos = 0; 
      }
      return c_str()[stPos]; 
   }

   wchar_t & operator[] (size_t stPos) 
   {
      if (stPos >= wcslen()) throw (SpdException::hr_exception(E_FAIL, L"Position is out of range.", __WFUNCTION__)); 
      return get_datap()[stPos]; 
   }
   
   //virtual wchar_t & operator[] (size_t stPos) = 0; 
   virtual void substr(CSpdCStringBase &wsOutput, size_t stPos, size_t stCount) const
   {
      if (stPos >= wcslen()) { return; }
      //
      // requested starting position is not beyond string length. calculate copy len.;
      // Adjust count so it reflects max ammount that may be copied.
      //
      size_t stCopySize(min(stCount, wcslen() - stPos)); 
      //
      // copy substring to output.
      //
      wsOutput.assign(c_str() + stPos, stCopySize); 
   }
   void Clear()
   {
      clear(); 
   }

   bool IsEmpty() const {  return empty(); }
   CSpdCStringBase & operator = (const CSpdCStringBase &c)
   {
      assign(c); 
      return *this; 
   }
   const wchar_t * operator = (const wchar_t *pwcs) { return assign(pwcs); }
   
   CSpdCStringBase & operator = (const std::wstring &ws)
   {
      assign(ws.c_str()); 
      return *this; 
   }

   CSpdCStringBase & operator += (const CSpdCStringBase &ws)
   {
      append(ws); 
      return *this; 
   }

   CSpdCStringBase & operator += (const std::wstring &ws)
   {
      append(ws); 
      return *this; 
   }

   CSpdCStringBase & operator += (wchar_t wc)
   {
      append(wc); 
      return *this; 
   }

   bool operator < (const CSpdCStringBase &ws) const { return compare(ws, true) < 0; }
   bool operator > (const CSpdCStringBase &ws) const { return compare(ws, true) > 0; }
   bool operator == (const CSpdCStringBase &ws) const { return compare(ws, true) == 0; }
   bool operator == (const wchar_t *pwcs) const { return compare(pwcs, true) == 0; }
   bool operator != (const CSpdCStringBase &ws) const { return compare(ws, true) != 0; }
   bool operator != (const wchar_t *pwcs) const { return compare(pwcs, true) != 0; }

private:
   wchar_t *get_datap() { return (wchar_t *)c_str(); }
   virtual CSpdCStringBase & assign1(const CSpdCStringBase &c) = 0; 
   virtual CSpdCStringBase & assign2(const wchar_t *pwc, size_t stLen) = 0; 
}; 

// SLB CR.34040.V01; cleanup. removed removed CSpdCStringFixed as it will never be 
// used.

template <size_t t_stMinBufSize>
class CSpdCStringGrowable : public CSpdCStringBase
{
private:
   wchar_t m_wcsFixed[t_stMinBufSize]; 
   wchar_t *m_pwcsVal; 
   size_t m_stBufLen; 
   size_t m_stStrLen; 
private:
   void _init() 
   { 
      m_stStrLen = 0; 
      m_stBufLen = t_stMinBufSize; 
      m_pwcsVal = m_wcsFixed; 
      m_pwcsVal[0] = 0; 
   }

public:
   CSpdCStringGrowable() 
   {
      _init(); 
   }
   ~CSpdCStringGrowable()
   {
      if (m_pwcsVal!=m_wcsFixed)
      {
         delete[](m_pwcsVal); 
         m_pwcsVal = NULL; 
      }
      m_stBufLen = 0; 
      m_stStrLen = 0; 
   }
   CSpdCStringGrowable(const CSpdCStringBase &c) 
   { 
      _init(); 
      assign(c); 
   }
   CSpdCStringGrowable(const CSpdCStringGrowable<t_stMinBufSize> &c) 
   { 
      _init(); 
      assign(c); 
   }
   CSpdCStringGrowable(const wchar_t *pwcs, const wchar_t *pwcs2=NULL) 
   {
      _init();
      assign(pwcs, pwcs2); 
   }

   #if 1
   CSpdCStringGrowable(const std::wstring &ws) 
   {
      _init(); 
      assign2(ws.c_str(), ws.size()); 
   }
#endif

   CSpdCStringBase & operator = (const CSpdCStringBase &c)
   {
      assign(c); 
      return *this; 
   }
   CSpdCStringGrowable<t_stMinBufSize> & operator = (const CSpdCStringGrowable<t_stMinBufSize> &c)
   {
      assign(c); 
      return *this; 
   }
   const wchar_t * operator = (const wchar_t *pwcs)
   {
      return assign(pwcs); 
   }
   const wchar_t * operator = (const wchar_t wc)
   {
      return assign(wc); 
   }
	CSpdCStringGrowable<t_stMinBufSize>& operator = (const BSTR bstr)
	{
		assign((const wchar_t*)bstr);
		return *this;
	}

   CSpdCStringGrowable<t_stMinBufSize> & operator = (const std::wstring &ws)
   {
      assign2(ws.c_str(), ws.size()); 
      return *this; 
   }
   operator std::wstring () const { return c_str(); }
   operator CComBSTR() const { return c_str(); }
   //operator const wchar_t *() const { return c_str(); }

   CSpdCStringGrowable<t_stMinBufSize> operator + (const CSpdCStringBase &a) const
   {
      CSpdCStringGrowable<t_stMinBufSize> tmp(*this); 
      tmp.append(a); 
      return tmp; 
   }

   size_t GetMaxLength() const
   { 
      return (size_t)0x7FFFFFFF; 
   }
   size_t wcslen() const { return m_stStrLen; }
   const wchar_t *c_str() const { return m_pwcsVal; }
   void clear() 
   {  
      m_pwcsVal[0] = 0;  
      m_stStrLen = 0;  
   }
	// SLB 20.jun.2025 CR.34040.V01; Fixed bug where stCount is exactly the amount 
	// left in the string length, string would not get NULL terminated at the 
	// correct location.
   void erase(size_t stPos, size_t stCount=npos) 
   {
      // if position is after end of string, just return.
      if (stPos >= m_stStrLen) return;
      if (stCount == npos || m_stStrLen == stPos + stCount)									// CR.34040.V01; modified.
      {  
         // special case: erase rest of string after position.
         m_pwcsVal[stPos] = 0; 
         m_stStrLen = stPos; 
         return;
      }

      stCount = min(m_stStrLen - stPos, stCount); 
      wmemcpy(m_pwcsVal + stPos, m_pwcsVal + stPos + stCount, m_stStrLen - (stPos + stCount)); 
      m_stStrLen -= stCount; 
   }

   void substr(CSpdCStringGrowable &ws, size_t stPos, size_t stCount=npos) const
   {
      CSpdCStringBase::substr(ws, stPos, stCount); 
   }

   CSpdCStringGrowable substr(size_t stPos, size_t stCount=npos) const
   {
      CSpdCStringGrowable wsRetVal; 
      CSpdCStringBase::substr(wsRetVal, stPos, stCount); 
      return wsRetVal; 
   }

   CSpdCStringGrowable &append(const CSpdCStringBase &a) 
   {
      if (m_stStrLen + a.wcslen() +1 > m_stBufLen)
      {
         ReAllocBuf(m_stStrLen+a.wcslen() + 1); 
      }
      wmemcpy(m_pwcsVal + m_stStrLen, a.c_str(), a.wcslen()); 
      m_stStrLen += a.wcslen(); 
      m_pwcsVal[m_stStrLen] = 0; 
      return *this; 
   }

   CSpdCStringBase &append(const std::wstring &a)
   {
      CSpdCStringGrowable ws; 
      ws.assign2(a.c_str(), a.size()); 
      return append(ws); 
   }

   CSpdCStringBase &append(wchar_t wc)
   {
      wchar_t wcs[2] = { wc, 0 };
      return append(wcs); 
   }

   /* replace() - this form of replace looks in current stream, for all instances of wsOld and replaces
    * them with wsNew. The end result is returned.
    *
    * SLB 07.jun.2019 CR.27900; Created. Note: this code is not attempting to be efficient. If that is required, 
    * then the code should be revisited.
    */
   CSpdCStringGrowable replace(const CSpdCStringBase &wsOld, const CSpdCStringBase &wsNew) const
   {
      size_t stStartPos(0);
      CSpdCStringGrowable wsVal(*this);
      for (size_t stFound = wsVal.find(wsOld, stStartPos); stFound != npos; stFound = wsVal.find(wsOld, stStartPos))
      {
         //
         // found. build new string.
         //
         CSpdCStringGrowable wsPre(wsVal.substr(0, stFound));
         CSpdCStringGrowable wsEnd(wsVal.substr(stFound + wsOld.wcslen()));
         wsVal = wsPre + wsNew + wsEnd;

         // Update startpos for next time through the loop.
         stStartPos = stFound + wsNew.wcslen();
      }
      return wsVal;
   }

   QUADWORD toQuadWord(int nRadix=10) const
   {
      QUADWORD val;
      val = (QUADWORD)_wcstoui64(c_str(), NULL, nRadix);
      return val;
   }

   CSpdCStringGrowable toUpper() const
   {
      CSpdCStringGrowable ws(*this); 
      for (size_t stPos(0); stPos < ws.size(); stPos++)
      {
         ws[stPos] = towupper(ws[stPos]);
      }
      return ws;
   }
   CSpdCStringGrowable toLower() const                                              // CR.26999.Item55; added.
   {
      CSpdCStringGrowable ws(*this); 
      for (size_t stPos(0); stPos < ws.size(); stPos++)
      {
         ws[stPos] = towlower(ws[stPos]);
      }
      return ws;
   }

private:
   CSpdCStringBase & assign1(const CSpdCStringBase &c) 
   {
      assign2(c.c_str(), c.size()); 
      return *this; 
   }
   CSpdCStringBase & assign2(const wchar_t *pwcs, size_t stLen)
   {
      m_pwcsVal[0] = 0; 
      m_stStrLen = 0; 
      if (!pwcs) return *this; 

      if (stLen + 1 > m_stBufLen) ReAllocBuf(stLen + 1); 
      m_stStrLen = stLen; 
      wmemcpy(m_pwcsVal, pwcs, m_stStrLen); 
      m_pwcsVal[m_stStrLen] = 0;  // ensures value is always terminated.
      return *this; 
   }
   void ReAllocBuf(size_t stMinBufLen) 
   {
      if (stMinBufLen < 0)
      {
         sol_assert(false);                                                         // CR.14962; modified.
      }
      else if (stMinBufLen > m_stBufLen)
      {
         size_t stNewBufLen(max(t_stMinBufSize, stMinBufLen)); 
         wchar_t *pwcsNewBuf = new wchar_t[stNewBufLen]; 

         if (m_pwcsVal)
         {
            wmemcpy(pwcsNewBuf, m_pwcsVal, m_stStrLen); 
            pwcsNewBuf[m_stStrLen] = 0; 
            m_pwcsVal[0] = 0; 
            if (m_pwcsVal!=m_wcsFixed) delete[](m_pwcsVal); 
         }
         m_pwcsVal = pwcsNewBuf; 
         m_stBufLen = stNewBufLen; 
      }
   }
}; 

// CR.21572; modified starting length of CSpdCString so that it will at least contain a GUID.
typedef CSpdCStringGrowable<40> CSpdCString;                                        // CR.21572; modified.

// CR.29294.V01; Comment out this constant. It is not used anywhere and it causes SolimarCompiler project
// to break (linker errors).
//const CSpdCString c_wsEmpty = L"";                                                // CR.29294; added.

#define T_CHAR_LEN(buflen) (buflen/sizeof(wchar_t))

//
// Define global comparison operators so compiler knows to use the CSpdCString when operating with wchar_t ptr lvalues.
//
__inline CSpdCString operator + (const wchar_t *pwcs, const CSpdCString &ws)         // CR.7698; added.
{
   return CSpdCString(pwcs) + ws;
}

__inline CSpdCString operator + (const CSpdCString &ws, const wchar_t *pwcs)         // CR.7698; added.
{
   return ws + CSpdCString(pwcs);
}

__inline bool operator == (const wchar_t *wcs, const CSpdCString &ws)               // CR.7698; added.
{
   return CSpdCString(wcs) == ws;
}

} // end of namespace SpdStreamable

#endif