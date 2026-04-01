// CAttribsBase.h : TODO: Add file description here.
//
#pragma once
#ifndef STRICT
#define STRICT
#endif
#ifndef NTDDI_VERSION
#define NTDDI_VERSION NTDDI_WINXP	//Version Constants for Kernel-Mode Drivers
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT  (0x0501)
#endif

// Windows Header Files:
#include <windows.h>
#include <map>
#include <list>
#include <vector>
#include <guiddef.h>
#include "quadword.h"
#include "s_wstring.h"
#include <assert.h>
#include "SpdUtils.h"
#include "SpdDefines.h"


//
// Version 3 of this functionality.
//

//
// header file 1 content.
//
class CWStringStream : public std::wstring
{
public:
	CWStringStream() : std::wstring() {} 
	CWStringStream(const std::wstring &ws) { *this = ws ; }
	CWStringStream(const wchar_t *wc) : std::wstring(wc) {}
	CWStringStream & operator =(const std::wstring &ws) 
   { 
      std::wstring::operator=(ws); 
      return *this  ; 
   }
	CWStringStream & operator =(const s_wstring &ws) 
   { 
      *this = (std::wstring)ws ; 
      return *this ; 
   }
	CWStringStream & operator =(const wchar_t *pwc) 
   { 
      std::wstring::operator=(pwc); 
      return *this  ; 
   }

} ;

namespace SpdAttribs
{

inline wchar_t *ReallocateWcsBuffer(wchar_t *pwcBufferToCopy, size_t stBuffSize, size_t &stNewBufSize)
{
   wchar_t *pwcNewBuffer(new wchar_t[2*stBuffSize]) ;

   stNewBufSize = 2*stBuffSize ;
   if (pwcBufferToCopy)
   {
      if (pwcNewBuffer)
         wmemcpy(pwcNewBuffer, pwcBufferToCopy, stBuffSize) ;
      delete [](pwcBufferToCopy) ;
   }
   assert(pwcNewBuffer!=NULL) ;
   return pwcNewBuffer ;
}
//*************************************************************************************
//
// The idea is that all primitive types must derive from CStreamable and
// define their own "ToString()" method.  This will allow more complex objects like
// lists and maps to be streamable (presuming they are defined with objects 
// which derive from CStreamable).
//
//*************************************************************************************
class CStreamable
{
public:
	operator CWStringStream() const { return ToString() ; }
   //operator std::wstring() const { return ToString() ; }
	CWStringStream ToString() const { CWStringStream ws ; ToString2(ws) ; return ws ; }
	//
	// Helper methods for derived classes.
	//
	size_t FindDataForTags(const std::wstring &wsTagId, 
								  const CWStringStream &wsData,
								  CWStringStream &wsOutVal,
								  bool bKeepTags = false,
								  size_t stSearchStartPos = 0 
								  ) const
   {
      size_t stRetVal(std::wstring::npos) ;
      std::wstring wsTokenTag(wsTagId + std::wstring(L">")) ;
      std::wstring wsOpenTag(std::wstring(L"<") + wsTokenTag ) ;
      size_t stStartPos(stSearchStartPos) ;
      size_t stEndPos(stSearchStartPos) ;
      DWORD dwTagCount(0) ;
      //
      // clear output
      //
      wsOutVal.clear() ;

      stStartPos = stRetVal = wsData.find(wsOpenTag, stSearchStartPos ) ;

      if (stStartPos != std::wstring::npos)
      {  //
         // found the open tag.
         // Adjust the search position and increment the tag count semaphore.
         //
         stEndPos = stStartPos + 1 ; // add one  because wsOpenTag.length() == wsTokenTag.length() + 1
         dwTagCount++ ;

         while (dwTagCount )
         {
            stEndPos = wsData.find(wsTokenTag, stEndPos + wsTokenTag.length()) ;
            if (stEndPos == std::wstring::npos)
            {  //
               // no more tags found.
               // break out of the loop.
               //
               break ;
            }
            else
            {
               if (wsData[stEndPos-1] == L'<' )
               {  //
                  // this is an opening tag, increment semaphore
                  //
                  dwTagCount ++ ;
               }
               else if (wsData[stEndPos-1] == L'/')
               {  //
                  // This is a closing tag, decrement semaphore
                  //
                  dwTagCount -- ;
               }
               else
               {  //
                  // This is just data that looks like a tag!
                  //
               }
            }
         } // end of tag search loop.

         if (dwTagCount)
         {  //
            // problem: xml is not clean.  could not match all opening tags with
            // closing tags.
            //
            stRetVal = std::wstring::npos ;
         }
         else
         {  //
            // semaphore is zero.
            // A matching close tag has been found for the first open tag.
            // Copy data between tags to output buffer.
            //
            if (bKeepTags)
            {  // 
               // keeping tags...
               // add the tag length to the end position.
               //
               stEndPos += wsTokenTag.length() ;
            }
            else
            {  //
               // not keeping tags...
               // Add the tag length to the start position.
               //
               stStartPos += wsOpenTag.length() ;
               stEndPos -= 2 ; // sub 2 because token tag length == close tag length - 2
            }
            //
            // populate output buffer.
            // 
            wsOutVal = wsData.substr(stStartPos, stEndPos - stStartPos  ) ;
         }
      }
      return stRetVal ;
   }


	//
	// The following methods must be defined by all derived classes.
	//
	virtual void ToString2(CWStringStream &wsOutVal ) const = 0 ;
} ;


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
   CStreamablePrimitiveBase() {}
   CStreamablePrimitiveBase(const CStreamablePrimitiveBase<T> &cpy ) : m_tVal(cpy.m_tVal) {}
   CStreamablePrimitiveBase(const T &tVal ) : m_tVal(tVal) {}

   virtual T& operator =(const T &tNewVal) = 0 ;
	virtual CStreamablePrimitiveBase<T> & operator =(const CWStringStream &ws) = 0 ;
	// 
	// Casting operators
	//
	operator T() const
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

	void ToString2(CWStringStream &wsOutVal) const
	{
		ValueToString(m_tVal, wsOutVal ) ;
	}

	virtual void ValueToString(const T &tVal, CWStringStream &wsOutVal) const = 0 ;
   virtual std::wstring ToStringNoTags() const 
   {
      CWStringStream wsRetVal ;

      ValueToString(m_tVal, wsRetVal) ;
      return (std::wstring) wsRetVal ;
   }
	virtual void StringToValue(const CWStringStream &ws, T &tOutVal ) const = 0 ;

	bool operator !=(const CStreamablePrimitiveBase<T> &cpy) const { return (m_tVal != cpy.m_tVal) ; }
	bool operator ==(const CStreamablePrimitiveBase<T> &cpy) const { return (m_tVal == cpy.m_tVal) ; }
	bool operator >(const CStreamablePrimitiveBase<T> &cpy) const { return (m_tVal > cpy.m_tVal) ; }
	bool operator <(const CStreamablePrimitiveBase<T> &cpy) const { return (m_tVal < cpy.m_tVal) ; }

	typedef T contained_t ;
public:
	T m_tVal ;
} ;


//*************************************************************************************
//
// This Template class is meant to handle concrete types which can be 
// subject to integer promotion. Specifically, there is a casting operator defined
// in s_wstring() for this type.  If there is no casting operator defined for s_wstring()
// then a new class should be written for the object.
//
//Needs to define the following:
// 1) Needed Constructors
// 2) Needed Assignment operators
// 3) ValueToString() and StringToValue()
//
//*************************************************************************************
template<class T, int nRadix>
class CStreamableInts : public CStreamablePrimitiveBase<T>
{
public:
	//
	// Required definition for part 1) constructors.
	//
	CStreamableInts() : CStreamablePrimitiveBase<T>() {}
   CStreamableInts(const CStreamableInts<T, nRadix> &cpy ) : CStreamablePrimitiveBase<T>(cpy) {}   
   CStreamableInts(const CWStringStream &wsInVal ) { StringToValue(wsInVal, m_tVal ) ; }
   CStreamableInts(const T &tVal ) : CStreamablePrimitiveBase<T>(tVal) {}

	//
	// Required definitions for part 2) Assignment operators.
	//
	CStreamableInts<T, nRadix> & operator = (const CStreamableInts<T, nRadix> &cpy)
   {
      m_tVal = cpy.m_tVal ;
      return *this ;
   }

   T & operator = (const T &tNewVal) // assignment operator when type T is used
   {
      m_tVal = tNewVal ;
      return m_tVal ;
   }

   CStreamableInts<T, nRadix> & operator = (const CWStringStream &wsNewVal) 
	{
		StringToValue(wsNewVal, m_tVal) ;
		return *this ;
	}

	//
	// Required definitions for part 3)
	//
	void ValueToString(const T &tVal, CWStringStream &wsOutVal) const
	{
		wsOutVal = s_wstring(tVal, nRadix) ;
	}

	void StringToValue(const CWStringStream &wsStream, T &tOutVal ) const
	{
	   s_wstring wsTemp(wsStream, nRadix) ;

		tOutVal = (T)wsTemp ; // s_wstring must have a casting operator of type T defined.
	}

} ;

//
// have to over ride this template method for types which don't want their type cast to 
// __int64 as the default method does.
//
template<>
void CStreamableInts<time_t, 16>::ValueToString(const time_t &tVal, CWStringStream &wsOutVal) const
{
   time_t tTmpVal(tVal) ;
	//
	// SLB 09.aug.04  Note: typecast to __int64 should not be necessary but seems to be required for type time_t
	//
	wsOutVal = s_wstring((__int64)tTmpVal, 16) ; 
}

template<>
void CStreamableInts<size_t, 16>::ValueToString(const size_t &tVal, CWStringStream &wsOutVal) const
{
   size_t tTmpVal(tVal) ;
	//
	// SLB 09.aug.04  Note: typecast to __int64 should not be necessary but seems to be required for type size_t
	//
	wsOutVal = s_wstring((__int64)tTmpVal, 16) ; 
}

//
// types which are streamed in hex
//
typedef CStreamableInts<unsigned __int64, 16> QwordHexObj ;
typedef CStreamableInts<unsigned __int32, 16> DwordHexObj ;
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
typedef CStreamableInts<unsigned __int32, 10> DwordDecObj ;
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
typedef CStreamableInts<double, 16> DoubleObj ;
typedef CStreamableInts<float, 16> FloatObj ;


//*************************************************************************************
//
// This Template class is meant to handle concrete types which can be 
// subject to integer promotion. Specifically, there is a casting operator defined
// in s_wstring() for this type.  If there is no casting operator defined for s_wstring()
// then a new class should be written for the object.
//
//Needs to define the following:
// 1) Needed Constructors
// 2) Needed Assignment operators
// 3) ValueToString() and StringToValue()
//
//*************************************************************************************
class CStreamableString : public CStreamablePrimitiveBase<std::wstring>
{
public:
	//
	// Required definition for part 1) constructors.
	//
	CStreamableString() : CStreamablePrimitiveBase<std::wstring>() {}
   CStreamableString(const CStreamableString &cpy ) : CStreamablePrimitiveBase<std::wstring>(cpy) {}
   CStreamableString(const CWStringStream &wsInVal ) { StringToValue(wsInVal, m_tVal ) ; }
   CStreamableString(const std::wstring &tVal ) : CStreamablePrimitiveBase<std::wstring>(tVal) {}
   CStreamableString(const wchar_t *pwcVal) 
   { 
      m_tVal = std::wstring(pwcVal) ; 
   }

	//
	// Required definitions for part 2) Assignment operators.
	//
	CStreamableString & operator = (const CStreamableString &cpy)
   {
      m_tVal = cpy.m_tVal ;
      return *this ;
   }

   std::wstring & operator = (const std::wstring &tNewVal) // assignment operator when type T is used
   {
      m_tVal = tNewVal ;
      return m_tVal ;
   }

   CStreamableString & operator = (const CWStringStream &wsNewVal) 
	{
		StringToValue(wsNewVal, m_tVal) ;
		return *this ;
	}

	//
	// Required definitions for part 3)
	//
	void ValueToString(const std::wstring &tVal, CWStringStream &wsOutVal) const
	{
      //wsOutVal = tVal ; 
      wsOutVal = SpdUtils::XmlEscapeString(tVal) ; 
	}
	void StringToValue(const CWStringStream &wsStream, std::wstring &tOutVal) const
	{
      
      //tOutVal = wsStream ; 
      tOutVal = SpdUtils::XmlUnescapeString(std::wstring(wsStream)) ; // s_wstring must have a casting operator of type T defined.
	}

	bool operator !=(const CStreamableString &cpy) const { return (m_tVal != cpy.m_tVal) ; }
	bool operator <(const CStreamableString &cpy) const { return (m_tVal < cpy.m_tVal) ; }

} ;

typedef CStreamableString WStringObj ;

//*************************************************************************************
//
//
//
//
//
//
//
//*************************************************************************************
class CBuffer
{
private: // private members
	size_t m_stBufLen ;
	PBYTE m_pbBuf ;

public: // public members
	CBuffer() : m_stBufLen(0), m_pbBuf(NULL) {}
	CBuffer(const PBYTE pb, size_t st ) : m_stBufLen(0), m_pbBuf(NULL)
	{	//
		// will create a copy of the buffer and not
		// touch the original
		//
		SetBuffer(pb,st) ;
	}
	CBuffer(const CBuffer &cpy ) : m_stBufLen(0), m_pbBuf(NULL)
	{
		operator =(cpy) ;
	}
	~CBuffer()
	{
		if (m_pbBuf)
      {
         delete[] (m_pbBuf) ;
      }
		m_pbBuf = NULL ;
		m_stBufLen = 0 ;
	}
	CBuffer &operator =(const CBuffer &cpy)
	{
		SetBuffer(cpy.m_pbBuf, cpy.m_stBufLen) ;
		return *this ;
	}
	bool operator==(const CBuffer &cpy) const 
	{
	   bool bRetVal(false) ;

		if (m_stBufLen == cpy.m_stBufLen)
		{
			if (!memcmp(m_pbBuf, cpy.m_pbBuf, m_stBufLen))
				bRetVal = true ;
		}
		return bRetVal ;
	}
	bool operator!=(const CBuffer &cpy) const
	{
		return !(operator ==(cpy)) ;
	}
	bool operator >(const CBuffer &cpy) const
   {
      bool bRetVal(false) ;

      if (m_stBufLen > cpy.m_stBufLen)
         bRetVal = true ;
      return bRetVal ;
   }
  	bool operator <(const CBuffer &cpy) const
   {
      bool bRetVal(false) ;

      if (m_stBufLen < cpy.m_stBufLen)
         bRetVal = true ;
      return bRetVal ;
   }
   PBYTE data() const { return m_pbBuf ; }
	size_t size() const { return m_stBufLen ; }

//private: // private methods
   // SLB 21.sep.2006 This does not need to be a private member:)  Made it public.
	void SetBuffer(const PBYTE pb, size_t st)
	{
		if (m_pbBuf)
		{
			delete[] (m_pbBuf) ;
		}
		m_pbBuf = new BYTE [st] ;
		m_stBufLen = st ;
		memcpy(m_pbBuf, pb, st ) ;
	}
} ;

class CStreamableBuffer : public CStreamablePrimitiveBase<CBuffer>
{
public:
	//
	// Required definition for part 1) constructors.
	//
	CStreamableBuffer() : CStreamablePrimitiveBase<CBuffer>() {}
   CStreamableBuffer(const CStreamableBuffer &cpy ) : CStreamablePrimitiveBase<CBuffer>(cpy) {}
   CStreamableBuffer(const CWStringStream &wsInVal ) { StringToValue(wsInVal, m_tVal ) ; }
   CStreamableBuffer(const CBuffer &tVal ) : CStreamablePrimitiveBase<CBuffer>(tVal) {}

	//
	// Required definitions for part 2) Assignment operators.
	//
	CStreamableBuffer & operator = (const CStreamableBuffer &cpy)
   {
      m_tVal = cpy.m_tVal ;
      return *this ;
   }

   CBuffer & operator = (const CBuffer &tNewVal) // assignment operator when type T is used
   {
      m_tVal = tNewVal ;
      return m_tVal ;
   }

   CStreamableBuffer & operator = (const CWStringStream &wsNewVal) 
	{
		StringToValue(wsNewVal, m_tVal) ;
		return *this ;
	}

	//
	// Required definitions for part 3)
	//
	void ValueToString(const CBuffer &tVal, CWStringStream &wsOutVal) const
	{	//
		// ValueToString()/StringToValue() are where the meat of this template lies.  
		//
		// Streamed out version of this object looks like:
		//
		//	<CBuffer>
		//	<Size>6</Size>
		// <Data>AB0934779523</Data>
		// </CBuffer>
		//
	   const CWStringStream wsBeginBuffer(L"<CBuffer>") ;
	   const CWStringStream wsEndBuffer(L"</CBuffer>") ;
	   const CWStringStream wsBeginSize(L"<Size>") ;
	   const CWStringStream wsEndSize(L"</Size>") ;
	   const CWStringStream wsBeginData(L"<Data>") ;
	   const CWStringStream wsEndData(L"</Data>") ;
	   //
	   // Note: typecast to __int64 should not be required here, but seems to be needed for non-standard
	   // types like time_t and size_t.
	   //
	   s_wstring wsSize((__int64)tVal.size(), 10) ;
	   s_wstring wsData(0,16) ;
	   DWORD i(0) ;

		wsOutVal = wsBeginBuffer ;
		wsOutVal += wsBeginSize ;
		wsOutVal +=		wsSize ;
		wsOutVal += wsEndSize ;
		wsOutVal += wsBeginData ;
		for (i=0; i<tVal.size(); i++)
		{
		   wchar_t wcFormatted[4] ;
			//
			// For each byte, format it in a hex string then append this to the 
			// Append operations should be fast as this is a std::wstring object
			// which is being appended to.
			//
			_snwprintf_s(wcFormatted, _countof(wcFormatted), L"%02X", tVal.data()[i] ) ;
			wsOutVal += wcFormatted ;
		}
		wsOutVal += wsEndData ;
		wsOutVal += wsEndBuffer ;
	}
	void StringToValue(const CWStringStream &ws, CBuffer &tOutVal ) const
	{
	   CWStringStream wsBufferData ;
	   const CWStringStream wsTagBuffer(L"CBuffer") ;
	   const CWStringStream wsTagSize(L"Size") ;
	   const CWStringStream wsTagData(L"Data") ;

		if (FindDataForTags(wsTagBuffer, ws, wsBufferData ) == std::wstring::npos)
		{	//
			//	Problem: could not retrieve Buffer object data.
         //
		}
		else
		{	//
			// Buffer object data is valid, look for number of bytes in data.
			//
			CWStringStream wsSizeData ;
			if (FindDataForTags(wsTagSize, wsBufferData, wsSizeData) == std::wstring::npos)
			{	//
				// Problem: could not retrieve number of bytes in data (size of converted buffer)
				//
			}
			else
			{	//
				// Found Size data, convert it to size_t
				//
			   size_t stBufferSize(0) ;
			   size_t i(0) ;

				stBufferSize = s_wstring(wsSizeData,10) ;
				//
				// Now retrieve Data from the Buffer object data
				//
				CWStringStream wsData ;

				if (FindDataForTags(wsTagData, wsBufferData, wsData) == std::wstring::npos)
				{	//
					// Problem: could not retrieve data
					//
				}
				else
				{	//
					// Buffer Data retrieved successfully.  
					// first, validate it is the size expected.
					//
					if (wsData.size() != stBufferSize*2)
					{	//
						// Problem: buffer fails size validation
						//
					}
					else
					{
                  size_t stFormattedPos(0) ;
					   size_t stBufPos(0) ;
					   PBYTE pbBuf(NULL) ;
	               //
						// Allocate a temporary buffer, then copy/convert the data to binary format.
						//
						pbBuf = new BYTE[stBufferSize] ;
						while (stBufPos < stBufferSize)
						{
						   s_wstring wsFormatted(wsData.substr(stFormattedPos, 2), 16)  ;

							pbBuf[stBufPos] = (BYTE)wsFormatted ;
							stBufPos ++ ;
							stFormattedPos += 2 ;
						}
						//
						// initialize tOutVal with buffer.
						//
						tOutVal = CBuffer(pbBuf, stBufferSize ) ;
						
						//
						// Now delete the temporary buffer.
						//
						delete[] (pbBuf) ;
					}
				}
			}
		}
	}
} ;



typedef CStreamableBuffer BufferObj ;

//
// FILETIME operators
//
//bool operator >(const FILETIME &t, const FILETIME &ut)
//{
//   return (CompareFileTime(&t, &ut)>0) ;
//}
//bool operator <(const FILETIME &t, const FILETIME &ut)
//{
//   return (CompareFileTime(&t, &ut)<0) ;
//}
//bool operator ==(const FILETIME &t, const FILETIME &ut)
//{
//   return (CompareFileTime(&t, &ut)==0) ;
//}


//*************************************************************************************
//
// This Template class is meant to handle concrete types which can be 
// subject to integer promotion. Specifically, there is a casting operator defined
// in s_wstring() for this type.  If there is no casting operator defined for s_wstring()
// then a new class should be written for the object.
//
//Needs to define the following:
// 1) Needed Constructors
// 2) Needed Assignment operators
// 3) ValueToString() and StringToValue()
//
//*************************************************************************************
class CStreamableSystemTime : public CStreamable
{
public: // public members
   FILETIME m_tVal ;
   bool m_bIsStreamInLocalTime ;

public: // public methods
	//
	// Required definition for part 1) constructors.
	//
	CStreamableSystemTime() : CStreamable(), m_bIsStreamInLocalTime(false) { memset(&m_tVal, 0, sizeof(m_tVal)) ; }
   CStreamableSystemTime(const CStreamableSystemTime &c) : m_bIsStreamInLocalTime(c.m_bIsStreamInLocalTime)
   { m_tVal = c.m_tVal ; }
   CStreamableSystemTime(const CWStringStream &wsInVal) : m_bIsStreamInLocalTime(false) { StringToValue(wsInVal, m_tVal) ; }
   CStreamableSystemTime(const FILETIME &tVal) : m_bIsStreamInLocalTime(false) { m_tVal = tVal ; }
   //CStreamableSystemTime(const SYSTEMTIME &stVal) : m_bStreamInLocalTime(false) { SystemTimeToFileTime(&stVal, &m_tVal) ; }
	//
	// Required definitions for part 2) Assignment operators.
	//
	CStreamableSystemTime & operator = (const CStreamableSystemTime &cpy)
   {
      m_tVal = cpy.m_tVal ;
      return *this ;
   }

   FILETIME operator = (const FILETIME &ftNewVal) // assignment operator when type T is used
   {
      //SYSTEMTIME stVal ;

      //m_tVal = ftNewVal ;
      //FileTimeToSystemTime(&ftNewVal, &stVal) ;
      //return stVal ;
      m_tVal = ftNewVal ;
      return m_tVal ;
   }

   //SYSTEMTIME operator = (const SYSTEMTIME &stNewVal) // assignment operator when type T is used
   //{
   //   SystemTimeToFileTime(&stNewVal, &m_tVal) ;
   //   return stNewVal ;
   //}

   CStreamableSystemTime & operator = (const CWStringStream &wsNewVal) 
	{
		StringToValue(wsNewVal, m_tVal) ;
		return *this ;
	}

   operator FILETIME () const
   {
      return m_tVal ;
   }
   //operator SYSTEMTIME() const
   //{
   //   SYSTEMTIME stVal ;

   //   FileTimeToSystemTime(&m_tVal, &stVal) ;
   //   return stVal ;
   //}

	//
	// Required definitions for part 3)
	//
	void ToString2(CWStringStream &wsOutVal) const
	{
      //SYSTEMTIME stVal ;

		ValueToString(m_tVal, wsOutVal) ;
	}

	void ValueToString(const FILETIME &ftVal, CWStringStream &wsOutVal) const
	{
      wchar_t wcTmp[50] ;
      SYSTEMTIME stVal ;
      FILETIME ftSystem(ftVal) ;

      if (m_bIsStreamInLocalTime)
      {
         FileTimeToLocalFileTime(&ftVal, &ftSystem) ;
      }

      FileTimeToSystemTime(&ftSystem, &stVal) ;

      _snwprintf_s(
         wcTmp,
         sizeof(wcTmp)/sizeof(wchar_t),
         L"%04d-%02d-%02d %02d:%02d:%02d.%04d",
         stVal.wYear,
         stVal.wMonth,
         stVal.wDay,
         stVal.wHour,
         stVal.wMinute,
         stVal.wSecond,
         stVal.wMilliseconds *10
         ) ;
      wsOutVal = wcTmp ; 
	}
	void StringToValue(const CWStringStream &wsStream, FILETIME &ftOutVal) const
	{
      SYSTEMTIME stVal ;
      size_t stSearchPos(0) ;
      size_t stFindPos(std::wstring::npos) ;
      s_wstring wsTemp(L"",10) ;
      BOOL bResult(false) ;

      stFindPos = wsStream.find(L"-", stSearchPos) ;
      if (stFindPos == std::wstring::npos)
      {  //
         // problem: could not find year.
         //
      }
      else
      {
         wsTemp = wsStream.substr(stSearchPos, stFindPos - stSearchPos).c_str() ;
         stVal.wYear = wsTemp ;
         stSearchPos = stFindPos+1 ;
         stFindPos = wsStream.find(L"-", stSearchPos) ;
         if (stFindPos == std::wstring::npos)
         {  //
            // problem: could not find month.
            //
         }
         else
         {
            wsTemp = wsStream.substr(stSearchPos, stFindPos - stSearchPos).c_str() ;
            stVal.wMonth = wsTemp ;
            stSearchPos = stFindPos+1 ;
            stFindPos = wsStream.find(L" ", stSearchPos) ;
            if (stFindPos == std::wstring::npos)
            {  //
               // problem: could not find day.
               //
            }
            else
            {
               wsTemp = wsStream.substr(stSearchPos, stFindPos - stSearchPos).c_str() ;
               stVal.wDay = wsTemp ;
               stSearchPos = stFindPos+1 ;
               stFindPos = wsStream.find(L":", stSearchPos) ;
               if (stFindPos == std::wstring::npos)
               {  //
                  // problem: could not find hour.
                  //
               }
               else
               {
                  wsTemp = wsStream.substr(stSearchPos, stFindPos - stSearchPos).c_str() ;
                  stVal.wHour = wsTemp ;
                  stSearchPos = stFindPos+1 ;
                  stFindPos = wsStream.find(L":", stSearchPos) ;
                  if (stFindPos == std::wstring::npos)
                  {  //
                     // problem: could not find minutes.
                     //
                  }
                  else
                  {
                     wsTemp = wsStream.substr(stSearchPos, stFindPos - stSearchPos).c_str() ;
                     stVal.wMinute = wsTemp ;
                     stSearchPos = stFindPos+1 ;
                     stFindPos = wsStream.find(L".", stSearchPos) ;
                     if (stFindPos == std::wstring::npos)
                     {  //
                        // problem: could not find seconds.
                        //
                     }
                     else
                     {
                        wsTemp = wsStream.substr(stSearchPos, stFindPos - stSearchPos).c_str() ;
                        stVal.wSecond = wsTemp ;
                        stSearchPos = stFindPos+1 ;
                        stFindPos = std::wstring::npos ;
                        wsTemp = wsStream.substr(stSearchPos, stFindPos - stSearchPos).c_str() ;
                        stVal.wMilliseconds = wsTemp ;
                        stVal.wMilliseconds /= 10 ;

                        bResult = SystemTimeToFileTime(&stVal, &ftOutVal) ;
                        if (bResult && m_bIsStreamInLocalTime)
                        {
                           FILETIME ftTemp(ftOutVal) ;
                           LocalFileTimeToFileTime(&ftTemp, &ftOutVal) ;
                        }
                     }
                  }
               }
            }
         }
      }

	}
   bool operator ==(const CStreamableSystemTime &src) const
   {
      bool bRetVal(false) ;

      bRetVal = compare(src) == 0 ? true : false ;
      return bRetVal ;
   }


   bool operator !=(const CStreamableSystemTime &src) const 
   {
      bool bRetVal(false) ;

      bRetVal = compare(src) == 0 ? false : true ;
      return bRetVal ;
   }

   bool operator <(const CStreamableSystemTime &src) const 
   {
      bool bRetVal(false) ;

      bRetVal = compare(src) < 0 ? true : false ;
      return bRetVal ;
   }

   bool operator >(const CStreamableSystemTime &src) const 
   {
      bool bRetVal(false) ;

      bRetVal = compare(src) > 0 ? true : false ;
      return bRetVal ;
   }
   void SetToCurrentTime()
   {
      GetSystemTimeAsFileTime(&m_tVal) ;
      //SYSTEMTIME stVal ;

      //GetSystemTime(&stVal ) ;
      //*this = stVal ;
   }
private: // private methods
   int compare(const CStreamableSystemTime &src) const
   {  // SLB 30.jun.2006 - modified to use the windows method for comparing FILETIME objects.
      return CompareFileTime(&m_tVal, &src.m_tVal) ;
      //int nRetVal(-1) ;
      //QUADWORD qwTime ;
      //QUADWORD qwSrcTime ;

      //qwTime = MAKEQUADWORD(m_tVal.dwLowDateTime, m_tVal.dwHighDateTime) ;
      //qwSrcTime = MAKEQUADWORD(src.m_tVal.dwLowDateTime, src.m_tVal.dwHighDateTime) ;
      //if (qwTime == qwSrcTime) 
      //   nRetVal = 0 ;
      //else if (qwTime > qwSrcTime)
      //   nRetVal = 1 ;
      //else
      //   nRetVal = -1 ;
      //return nRetVal ;
   }



} ;

typedef CStreamableSystemTime SystemTimeObj ;

////
////
////





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
   CStreamableMaps(const CStreamableMaps<T> &cpy ) : CStreamablePrimitiveBase<T>(cpy) {} 
   CStreamableMaps(const CWStringStream &wsInVal ) : CStreamablePrimitiveBase<T>(wsInVal) {}
	CStreamableMaps(const T &tVal ) : CStreamablePrimitiveBase<T>(tVal) {}

	//
	// Required definitions for part 2) Assignment operators.
	//
	CStreamableMaps<T> & operator = (const CStreamableMaps<T> &cpy)
   {
      m_tVal = cpy.m_tVal ;
      return *this ;
   }

   T & operator = (const T &tNewVal) // assignment operator when type T is used
   {
      m_tVal = tNewVal ;
      return m_tVal ;
   }

   CStreamableMaps<T> & operator = (const CWStringStream &wsNewVal) 
	{
		StringToValue(wsNewVal, m_tVal) ;
		return *this ;
	}

	//
	// Required definitions for part 3)
	//
	/* ValueToString() - builds Xml stream from the object data.
	 *
	 * SLB 20.aug.04 Changed format of Xml.  No longer puts count in xml.
    * SLB 05.jul.05 - Modified to use _snwprintf() to improve performance.
	--------------------------------------------------------------------------------------------*/
	void ValueToString(const T &tVal, CWStringStream &wsOutVal) const
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
		//
      size_t stOutBufSize(0x2000) ;
      wchar_t *pwcTempOut(ReallocateWcsBuffer(NULL, stOutBufSize, stOutBufSize)) ; // allocates a growable buffer
      const wchar_t *pwcMapKey = L"Map" ;
	   T::const_iterator it (tVal.begin()) ;
      int nCpyLen(0) ;
      size_t stWriteLen(0) ;

      nCpyLen = _snwprintf_s(
         &pwcTempOut[stWriteLen],
         stOutBufSize - stWriteLen,
         stOutBufSize - stWriteLen,
         L"<%s>",
         pwcMapKey
         ) ;
      stWriteLen += nCpyLen ;

		while (it != tVal.end() && stWriteLen<stOutBufSize)
		{  
			while (stOutBufSize - stWriteLen < wcslen(L"<N><K></K><V></V></N>") + wcslen(it->first.ToString().c_str()) + wcslen(it->second.ToString().c_str()) + 2/*NULL*/)
			{  //
				// Not enough room in buffer for ending tag.
				// make sure there is enough room
				//
				pwcTempOut = ReallocateWcsBuffer(pwcTempOut, stOutBufSize, stOutBufSize) ;
			}
			
			//
			// for each node, generate appropriate XML.  
			// 
         nCpyLen = _snwprintf_s(
            &pwcTempOut[stWriteLen],
            stOutBufSize - stWriteLen,
            stOutBufSize - stWriteLen,
            L"<N><K>%s</K><V>%s</V></N>",
            it->first.ToString().c_str(),
            it->second.ToString().c_str()
            ) ;

            stWriteLen += nCpyLen ;
            it++ ;
		}

      //
      // Add </Map>
      //
      while (stOutBufSize - stWriteLen < wcslen(pwcMapKey)+wcslen(L"</>")+2/*NULL*/)
      {  //
         // Not enough room in buffer for ending tag.
         // make sure there is enough room
         //
         pwcTempOut = ReallocateWcsBuffer(pwcTempOut, stOutBufSize, stOutBufSize) ;
      }
      nCpyLen = _snwprintf_s(
         &pwcTempOut[stWriteLen],
         stOutBufSize - stWriteLen,
         stOutBufSize - stWriteLen,
         L"</%s>",
         pwcMapKey
         ) ;
      stWriteLen += nCpyLen ;
      pwcTempOut[stOutBufSize - 1] = 0 ; // ensure null termination.
      wsOutVal = pwcTempOut ;

      assert(nCpyLen >= 0) ;
      delete(pwcTempOut) ;
	}
	/* StringToValue() - ingests Xml stream and initializes object from it.
	 *
	 * SLB 20.aug.04 Changed format of Xml.  No longer puts count in xml.
	--------------------------------------------------------------------------------------------*/
	void StringToValue(const CWStringStream &ws, T &tOutVal ) const
	{
	   CWStringStream wsMapData ;
	   const std::wstring wsTagMap(L"Map") ;
	   const std::wstring wsTagNode(L"N") ;
	   const std::wstring wsTagKey(L"K") ;
	   const std::wstring wsTagVal(L"V") ;
      //
      // clear output data
      //
      tOutVal.clear() ;

		if (FindDataForTags(wsTagMap, ws, wsMapData ) == std::wstring::npos)
		{	//
			//	Problem: could not retrieve Map data.
			//
		}
		else
		{	
		   size_t stNodeSearchPos(0) ;
			//
			// Map data is valid, Now retrieve key/value pairs from the map data
         // These are imbedded in "nodes".
			//
			while (stNodeSearchPos != std::wstring::npos)
			{	
			   CWStringStream wsNodeData ;
				//
				// Each iteration retrieves one Node which contains a key/value pair.  Retrieve 
				// Node data and set search position to position after the end tag.
				//
            stNodeSearchPos = FindDataForTags(wsTagNode, wsMapData, wsNodeData, true, stNodeSearchPos) ;
				if (stNodeSearchPos == std::wstring::npos)
				{	//
					// Not a Problem: No more nodes to retrieve.
					//
				}
				else
				{	
				   CWStringStream wsKeyData ;
				   CWStringStream wsValData ;
               //
					// key/value pair is within the node data
               // Update the search position index.   
               //
               stNodeSearchPos += wsNodeData.length() ;

               //
               // retrieve key and value data next.
					//
					if (FindDataForTags(wsTagKey, wsNodeData, wsKeyData) == std::wstring::npos)
					{	//
						// Problem: this is an empty node.  No Key data was specified.
						//
					}
					else if (FindDataForTags(wsTagVal, wsNodeData, wsValData)== std::wstring::npos)
					{	// 
						// Problem: No Value data was specified.
						//
					}
					else
					{	//
						// Key and value retrieved successfully, convert them and add them to the map.
						//
						T::key_type ktKey ;
						T::mapped_type mtValue ;

						ktKey = wsKeyData ;
						mtValue = wsValData ;
						tOutVal.insert(std::make_pair(ktKey,mtValue)) ;
					}
				}
			}
		}

	}
} ;

typedef std::map< WStringObj, WStringObj, std::less<std::wstring> > TMapStringString ;
typedef std::map< WStringObj, DwordObj, std::less<std::wstring> > TMapStringDword ;
typedef std::map< DwordObj, WStringObj > TMapDwordString ;
typedef std::map< DwordObj, DwordObj > TMapDwordDword ;
typedef std::map< WStringObj, BufferObj, std::less<std::wstring> > TMapStringBuffer ;
typedef std::map< BufferObj, BufferObj, std::less<CBuffer> > TMapBufferBuffer ;

class CAttribsBase ;
typedef std::map< WStringObj, CAttribsBase, std::less<std::wstring> > TMapStringAttributes ;

typedef CStreamableMaps< TMapStringString  > MapStringStringObj ;
typedef CStreamableMaps< TMapStringDword  > MapStringDwordObj ;
typedef CStreamableMaps< TMapDwordString  > MapDwordStringObj ;
typedef CStreamableMaps< TMapDwordDword  > MapDwordDwordObj ;
typedef CStreamableMaps< TMapStringBuffer  > MapStringBufferObj ;
typedef CStreamableMaps< TMapBufferBuffer  > MapBufferBufferObj ;
typedef CStreamableMaps< TMapStringAttributes  > MapStringAttributesObj ;






//*************************************************************************************
//
// This Template class presumes T is something like std::vector<X>.  Where X must be 
// an object derived from CStreamable.
//
// 1) Needed Constructors
// 2) Needed Assignment operators
// 3) ValueToString() and StringToValue()
//
//*************************************************************************************
template <class T>
class CStreamableVectors : public CStreamablePrimitiveBase<T>
{
public:
	//
	// Required definition for part 1) constructors.
	//
   CStreamableVectors() : CStreamablePrimitiveBase<T>() {}
	CStreamableVectors(const CStreamableVectors<T> &cpy ) : CStreamablePrimitiveBase<T>(cpy) {} 
	CStreamableVectors(const CWStringStream &wsInVal ) : CStreamablePrimitiveBase<T>(wsInVal) {}
	CStreamableVectors(const T &tVal ) : CStreamablePrimitiveBase<T>(tVal) {} 

	//
	// Required definitions for part 2) Assignment operators.
	//
	CStreamableVectors<T> & operator = (const CStreamableVectors<T> &cpy)
   {
      m_tVal = cpy.m_tVal ;
      return *this ;
   }

   T & operator = (const T &tNewVal) // assignment operator when type T is used
   {
      m_tVal = tNewVal ;
      return m_tVal ;
   }

   CStreamableVectors<T> & operator = (const CWStringStream &wsNewVal) 
	{
		StringToValue(wsNewVal, m_tVal) ;
		return *this ;
	}

	//
	// Required definitions for part 3)
	//
	/* ValueToString() - builds Xml stream from the object data.
	 *
	 * SLB 20.aug.04 Changed format of Xml.  No longer puts count in xml.
    * SLB 05.jul.05 - Modified to use _snwprintf() to improve performance.
	--------------------------------------------------------------------------------------------*/
	void ValueToString(const T &tVal, CWStringStream &wsOutVal) const
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
      size_t stOutBufSize(0x2000) ;
      wchar_t *pwcTempOut(ReallocateWcsBuffer(NULL, stOutBufSize, stOutBufSize)) ; 
      const wchar_t *pwcVectorKey = L"Vector" ;
	   T::const_iterator it (tVal.begin()) ;
      int nCpyLen(0) ;
      size_t stWriteLen(0) ;

      nCpyLen = _snwprintf_s(
         &pwcTempOut[stWriteLen],
         stOutBufSize - stWriteLen,
         stOutBufSize - stWriteLen,
         L"<%s>",
         pwcVectorKey
         ) ;
      stWriteLen += nCpyLen ;

		while (it != tVal.end() && stWriteLen<stOutBufSize)
		{  
			while (stOutBufSize - stWriteLen < wcslen(L"<N></N>") + wcslen(it->ToString().c_str()) + 2/*NULL*/)
			{  //
				// Not enough room in buffer for ending tag.
				// make sure there is enough room
				//
				pwcTempOut = ReallocateWcsBuffer(pwcTempOut, stOutBufSize, stOutBufSize) ;
			}

			//
			// for each node, generate appropriate XML.  
			// 
         nCpyLen = _snwprintf_s(
            &pwcTempOut[stWriteLen],
            stOutBufSize - stWriteLen,
            stOutBufSize - stWriteLen,
            L"<N>%s</N>",
            it->ToString().c_str()
            ) ;
            stWriteLen += nCpyLen ;
         	it++ ;
		}

      //
      // Add </Vector>
      //
      while (stOutBufSize - stWriteLen < wcslen(pwcVectorKey)+wcslen(L"</>")+2/*NULL*/)
      {  //
         // Not enough room in buffer for ending tag.
         // make sure there is enough room
         //
         pwcTempOut = ReallocateWcsBuffer(pwcTempOut, stOutBufSize, stOutBufSize) ;
      }

      nCpyLen = _snwprintf_s(
         &pwcTempOut[stWriteLen],
         stOutBufSize - stWriteLen,
         stOutBufSize - stWriteLen,
         L"</%s>",
         pwcVectorKey
         ) ;

      stWriteLen += nCpyLen ;
      pwcTempOut[stOutBufSize-1] = 0 ; // ensure this is NULL terminated.
      wsOutVal = pwcTempOut ;

      assert(nCpyLen >= 0) ;
      delete(pwcTempOut) ;
	}
	/* StringToValue() - ingests Xml stream and initializes object from it.
	 *
	 * SLB 20.aug.04 Changed format of Xml.  No longer puts count in xml.
	--------------------------------------------------------------------------------------------*/
	void StringToValue(const CWStringStream &ws, T &tOutVal ) const
	{
	   CWStringStream wsVectorData ;
	   const CWStringStream wsTagVector(L"Vector") ;
	   const std::wstring wsTagNode(L"N") ;
      //
      // clear output
      //
      tOutVal.clear() ;

		if (FindDataForTags(wsTagVector, ws, wsVectorData ) == std::wstring::npos)
		{	//
			//	Problem: could not retrieve Vector data.
         //
		}
		else
		{
         size_t stNodeSearchPos(0) ;
		   CWStringStream wsData ;
         //
			// Vector data is valid
			// Now retrieve items from the vector data. Retrieve until search for next node fails.
			//
			while (stNodeSearchPos!=std::wstring::npos) 
			{	
			   CWStringStream wsNodeData ;
				//
				// each iteration retrieves one node/item.
				//

				//
				// Find the next node data: e.g. wsNodeData will look like "<N><V>456</V></N>.
				// Also, sets the search position offset to position just after wsEndNode.
				//
				stNodeSearchPos = FindDataForTags(wsTagNode, wsVectorData, wsNodeData, true, stNodeSearchPos) ;
				if (stNodeSearchPos == std::wstring::npos)
				{	//
					// Not a Problem: No more nodes in vector data.
					//
				}
				else
				{	
				   CWStringStream wsValue ;
					//
					// retrieved item with its tags.  
               // update search position index.
               //
               stNodeSearchPos += wsNodeData.length() ;

               //
               // Now extract the item value.
					//
					if (FindDataForTags(wsTagNode, wsNodeData, wsValue) == std::wstring::npos)
					{	//
						// Problem: could not retrieve Node value.  Should have caught this 
                  // in prvious FindDataForTags().
						//
					}
					else
					{	//
						// item retrieved successfully, convert it and add it to the vector.
						//
						T::value_type vtValue ;

						vtValue = wsValue ;
						tOutVal.push_back(vtValue ) ;
					}
				}
			}
		}
	}
} ;


typedef std::vector< WStringObj > TVectorString ;
typedef std::vector< DwordObj > TVectorDword ;
typedef std::vector< BufferObj > TVectorBuffer ;
typedef std::vector< CAttribsBase > TVectorAttributes ;

typedef CStreamableVectors< TVectorString  > VectorStringObj ;
typedef CStreamableVectors< TVectorDword  > VectorDwordObj ;
typedef CStreamableVectors< TVectorBuffer  > VectorBufferObj ;
typedef CStreamableVectors< TVectorAttributes > VectorAttributesObj ;



// special types to be used
class CAttribMemberBase ;
typedef std::map < std::wstring, std::wstring, std::less<std::wstring> > SWStringMap ;
typedef SWStringMap TStringMap ;
typedef TStringMap::iterator TItStringMap ;
typedef TStringMap::const_iterator TItcStringMap ;

typedef SWStringMap TPropertiesMap ;
typedef TItStringMap TItPropertiesMap ;
typedef TItcStringMap TItcPropertiesMap ;

typedef std::map < s_wstring, CAttribMemberBase * > SAttribMemberMap ;

//*************************************************************************************
//
// class CAttribMemberBase:
//
//
//
//
//*************************************************************************************
class CAttribMemberBase : public CStreamable
{
public:
   //
   // Default constructor.  Warning.... Not only does this version of the constructor, not "self-link".
   // It leaves the keyname member empty so container class is responsible for adding a key for its
   // child object.  
   //
   // This is ideal when container is not an Attribs class but rather a Sreamable Vector or Map class.
   //
	CAttribMemberBase() : CStreamable(){}
   CAttribMemberBase(const CAttribMemberBase &cpy) : 
      m_wsKeyName(cpy.m_wsKeyName)
   {
   }
   CAttribMemberBase(SAttribMemberMap &mapAttribObjs,	// map object which this member will register itself in
                     const std::wstring &wsKeyName	// keyname of this object used in map and in stream
                     ) :
      m_wsKeyName(wsKeyName)
   {	//
      // auto register member into parent list.
      //
      mapAttribObjs.insert(std::make_pair(m_wsKeyName, this)) ;
   }
   CAttribMemberBase(const CWStringStream &wsKeyName						// keyname of this object used in map and in stream
                     ) :
      m_wsKeyName(wsKeyName)
   {	//
      // Non-registering version of constructor.  This is used by top level
      // attributes classes only.
      //
   }

	//
	// Some helper methods.
	//
	CWStringStream GetBeginKey() const 
	{	
	   std::wstring wsRetVal ;
		//
		// Build begin key string in return string.
		//
      if (m_wsKeyName.size())
		   wsRetVal = std::wstring(L"<") + m_wsKeyName + std::wstring(L">") ;
		return wsRetVal ;
	}
	CWStringStream GetEndKey() const
	{	
	   std::wstring wsRetVal ;
		//
		// Build end key string in return string.
		//
      if (m_wsKeyName.size())
   		wsRetVal = std::wstring(L"</") + m_wsKeyName + std::wstring(L">") ;
		return wsRetVal ;
	}

   const std::wstring & GetKeyName() const { return m_wsKeyName ; }
	//
	// pure virtual methods to be defined by derived classes.
	//
   // SLB 15.mar.2005 - removed default value support
   //
	//virtual bool IsDefaultValue() const = 0 ;
	//virtual void SetDefaultValue(const CWStringStream &ws) = 0 ;
	virtual CAttribMemberBase & operator =(const CWStringStream &ws) = 0 ;
   bool operator ==(const CAttribMemberBase &cmp) const { return (compare(cmp) == 0) ; }
   bool operator !=(const CAttribMemberBase &cmp) const { return (compare(cmp) != 0) ; }
   bool operator <(const CAttribMemberBase &cmp) const { return (compare(cmp) < 0) ; }
   bool operator >(const CAttribMemberBase &cmp) const { return (compare(cmp) > 0) ; }
   bool operator <=(const CAttribMemberBase &cmp) const { return (compare(cmp) <= 0) ; }
   bool operator >=(const CAttribMemberBase &cmp) const { return (compare(cmp) >= 0) ; }

protected:
   virtual int compare(const CAttribMemberBase &cmp) const = 0 ;
	const std::wstring m_wsKeyName ;
} ;





//*************************************************************************************
//
// template class CAttribMemberBaseU<U,T>:
//
//
//
// SLB 15.mar.2005 - removed default value support
//
//*************************************************************************************
template < class U, class T > 
class CAttribMemberBaseU : public CAttribMemberBase
{
public: // public types
	typedef U TypeU ;
	typedef T TypeT ;

protected: // protected members
   U m_uVal ;
	//U m_uDefaultVal ;

public: // public methods
	//
   // constructors
	//

   //
   // Default constructor -
   // 
   // Warning.... Not only does this version of the constructor, not "self-link",
   // it leaves the keyname member empty so container class is responsible for adding a key for its
   // child object.  
   //
   // This is ideal when container is not an Attribs class but rather a Sreamable Vector or Map class.
   //
   CAttribMemberBaseU() : CAttribMemberBase() {}
   //
   // Copy constructor
   //
	CAttribMemberBaseU(const CAttribMemberBaseU<U,T> &cpy ) : 
		CAttribMemberBase(cpy ),
		//m_uDefaultVal(cpy.m_uDefaultVal) 
      m_uVal(cpy.m_uVal)
   {
   } 

   CAttribMemberBaseU(	SAttribMemberMap &mapAttribObjs, // map object which this member will register itself in
                        const std::wstring &wsKeyName,	// keyname of this object used in map and in stream
                        const CAttribMemberBaseU &uCpy
	                  ) :
      CAttribMemberBase(mapAttribObjs, wsKeyName),
		//m_uDefaultVal(uCpy.m_uDefaultVal), 
		m_uVal(uCpy.m_uVal)
   {
   }


   CAttribMemberBaseU(	SAttribMemberMap &mapAttribObjs, // map object which this member will register itself in
                        const std::wstring &wsKeyName,	// keyname of this object used in map and in stream
                        const U &uDefaultVal
	                  ) :
      CAttribMemberBase(mapAttribObjs, wsKeyName),
		//m_uDefaultVal(uDefaultVal), 
		m_uVal(uDefaultVal)
   {
   }
	
	std::wstring ToStringNoTags() const 
   {
      std::wstring wsRetVal ;

      wsRetVal = m_uVal.ToStringNoTags() ;
      return wsRetVal ;
   }
   void ToString2(CWStringStream &wsOutVal) const
	{
		ValueToString(m_uVal, wsOutVal) ;
	}
   /*  ValueToString() -
    * SLB 05.jul.2005 - modified to use _snwprintf() for performance reasons.
   *--------------------------------------------------------------------------*/
	void ValueToString(const U &uVal, CWStringStream &wsRetVal) const
	{
      size_t stOutBufSize(0x2000) ;
      wchar_t *pwcTempOut(ReallocateWcsBuffer(NULL, stOutBufSize, stOutBufSize)) ; // allocates a growable buffer
      int nCpyLen(-1) ;

      while (stOutBufSize < wcslen(L"<></>\r\n") + (2*wcslen(GetKeyName().c_str())) + wcslen(uVal.ToString().c_str()) + 2/*NULL*/)
      {  
		//
		// Not enough room in buffer for ending tag.
		// make sure there is enough room
		//
		pwcTempOut = ReallocateWcsBuffer(pwcTempOut, stOutBufSize, stOutBufSize) ;
      }

      nCpyLen = _snwprintf_s(
            pwcTempOut,
            stOutBufSize,
            stOutBufSize,
            L"<%s>%s</%s>\r\n",
            GetKeyName().c_str(),
            uVal.ToString().c_str(),
            GetKeyName().c_str()
            ) ;
      pwcTempOut[stOutBufSize -1] = 0 ;
      wsRetVal = pwcTempOut ;

      assert(nCpyLen >= 0) ;
      delete(pwcTempOut) ;
	}
   /* StringToValue() -
    * 01.sep.04  Now supports empty keynames.  In this case, entire buffer is treated as 
    * intialization string.
   -------------------------------------------------------------------------------------*/
   void StringToValue(const CWStringStream &ws, U &uRetVal ) const
	{
	   size_t stBegin(0 ) ;
	   size_t stEnd(ws.size() - 1 ) ;
	   //
	   // Build strings for the HTML tags
	   //
	   std::wstring wsBegin(GetBeginKey()) ;
	   std::wstring wsEnd(GetEndKey()) ;
		//
		// Search for the begin html tag.
		//
      if (wsBegin.size())
		   stBegin = ws.find(wsBegin, 0 ) ; 
		if (stBegin == std::wstring::npos || ws.empty())
		{	//
			// no beginning tag found, no further processing required.  Do not
			// modify uRetVal
			//
		}
		else
		{	//
			// found the tag, update the beginning position offset to be the first byte after the tag.
			//
			stBegin += wsBegin.size() ;
			//
			// Now search for the end tag.
			//
         if (ws.size())
            stEnd = ws.find(wsEnd, stBegin ) ;
			if (stEnd == std::wstring::npos)
			{	// 
				// No end tag found.  no further processing required.  Do not
				// modify uRetVal.
				//
			}
			else
			{	//
				// Found the end tag...  retrieve value string.
				//
				std::wstring wsVal ;

				wsVal = ws.substr(stBegin, stEnd - stBegin) ; 
				//
				// Now convert value string to value. 
				// 
				uRetVal = (CWStringStream)wsVal ;
			}
		}

	}

	//
	// Assignment operators.
	//
   CAttribMemberBaseU<U,T> & operator = (const CWStringStream &wsNewVal) 
	{
		StringToValue(wsNewVal, m_uVal) ; 
		return *this ;
	}  

   CAttribMemberBaseU<U,T> & operator = (const CAttribMemberBaseU<U,T> &wsNewVal) 
	{
		m_uVal = wsNewVal.m_uVal ;
		return *this ;
	}  

	//
	// SLB 15.aug.04 - This operator's implementation is troublesome because when T= size_t or time_t
	// a reference to object m_uVal.m_tVal cannot be returned, due to compiler error regarding converting
	// size_t to DWORD.  This seems to be because size_t is treated as an __int64 for the most part, but
	// then treated as a DWORD else where within the compiler.  No strong conclusions though.
	// TODO: look into this some time because operator= typically returns by reference not by value.
	//
	T operator = (const T &tVal)
	{
		m_uVal.m_tVal = tVal ;
		return tVal ; //m_uVal.m_tVal ;
	}


	//
	// Methods required to define (because pure virtual in base class).
	//
	//bool IsDefaultValue() const { return m_uVal.m_tVal == m_uDefaultVal.m_tVal ; }
	//void SetDefaultValue(const CWStringStream &ws) 
	//{
	//	m_uDefaultVal = ws ;
	//}
	// 
	// Casting operators
	//
	operator T() const
	{
		return (T)m_uVal ;
	}

	operator T()
	{
		return (T)m_uVal ;
	}
   operator U() const
   {
      return m_uVal ;
   }
   operator U() 
   {
      return m_uVal ;
   }

	//
	// referencing operators
	//
   T* operator -> () 
   {
      return &m_uVal.m_tVal ;
   }

   const T* operator -> () const
   {
      return &m_uVal.m_tVal ;
   }
	
private: // private methods
   //
   // compare()
   //    return -1 if this < cmp
   //    return 0 if this == cmp
   //    return 1 if this > cmp
   virtual int compare(const CAttribMemberBase &cmp) const 
   {
   int nRetVal(0) ;
   const CAttribMemberBaseU<U,T> &cmpu((const CAttribMemberBaseU<U,T> &)cmp) ;
      if (m_uVal > cmpu.m_uVal)
         nRetVal = 1 ;
      else if (m_uVal < cmpu.m_uVal)
         nRetVal = -1 ;
      return 0 ;
   }
} ;



//
// Global operators for Attribute member objects
//
template<class U, class T>
bool operator !=(const CAttribMemberBaseU<U,T> &ut, const T &t)
{
	return t != (T)ut ; 
}

template<class U, class T>
bool operator !=(const T &t, const CAttribMemberBaseU<U,T> &ut)
{
	return operator !=(ut,t) ; 
}

template<class U, class T>
bool operator ==(const T &t, const CAttribMemberBaseU<U,T> &ut)
{
	return ! (operator !=(ut,t)) ; 
}

template<class U, class T>
bool operator ==(const CAttribMemberBaseU<U,T> &ut, const T &t)
{
	return operator ==(t,ut) ; 
}

template<class U, class T>
bool operator <(const CAttribMemberBaseU<U,T> &ut, const T &t)
{
	return (T)ut < t ; 
}

template<class U, class T>
bool operator <(const T &t, const CAttribMemberBaseU<U,T> &ut)
{
	return  t < (T)ut ; 
}

template<class U, class T>
bool operator >(const CAttribMemberBaseU<U,T> &ut, const T &t)
{
	return (T)ut > t ; 
}

template<class U, class T>
bool operator >(const T &t, const CAttribMemberBaseU<U,T> &ut)
{
	return  t > (T)ut ; 
}





//*************************************************************************************
//
//
//
//
// SLB 15.mar.2005 - removed default value support
//
//*************************************************************************************
typedef CAttribMemberBaseU<DwordObj, DWORD> DwordAttribMember ;
template<class T>
class AttribEnumTypeMember : public DwordAttribMember
{
public:
	AttribEnumTypeMember(SAttribMemberMap &mapAttribObjs, // map object which this member will register itself in
								const std::wstring &wsKeyName,	// keyname of this object used in map and in stream
								const T &tDefault
								) :
	   DwordAttribMember (mapAttribObjs, wsKeyName, (DWORD)tDefault) //(CAttribMemberBaseU<DwordObj, DWORD> ) tDefault
	{
	}

   // DCM 19.sep.04 - If operator T() is not const, then a C2440 compiler error occurs
   // when you attempt to cast a const AttribEnumTypeMember<T> to T. So operator T()
   // must be const. But this introduces strange compiler behavior, where switch
   // statements on an AttribEnumTypeMember<T> cause a C2450 compiler error to occur,
   // unless you explicitly cast the AttribEnumTypeMember<T> to either T or a DWORD in
   // the switch(). The operator DWORD() below makes the C2450 error go away, as long
   // as the operator DWORD() is NOT const. SLB and I don't currently understand this
   // strange behavior, and suspect it could be a Microsoft compiler bug. It might be
   // something worth investigating in the future.
	operator T() const { return (T)((DWORD)m_uVal ) ; }
	operator DWORD() { return (DWORD)m_uVal ; }
	T operator = (const T &tVal)
	{
		m_uVal.m_tVal = tVal ;
		return tVal ; //m_uVal.m_tVal ;
	}
} ;

//*************************************************************************************
//
//
//
//
// SLB 15.mar.2005 - removed default value support
//
//*************************************************************************************
class CAttribsBase : public CAttribMemberBase //CStreamable
{
   
public: // public data
   
protected: // protected data
   bool m_bIsAttribsMember;
   std::wstring m_wsAttribsClassName;
   TPropertiesMap m_mapAttribStream ; // contains the key/value pairs of the Attribs members in s_wstring form
   SAttribMemberMap m_mapAttribObjs ;   // a self built list of pointers to the attrib's members

public: // public methods
	//
	// Top level (non-linked)version of constructor.  No key name specified either.
	//
   CAttribsBase() : 
	   m_bIsAttribsMember(false),
      CAttribMemberBase()
   {
   }
   CAttribsBase(const CAttribsBase &cpy ) :
      m_bIsAttribsMember(cpy.m_bIsAttribsMember),
	   m_wsAttribsClassName(cpy.m_wsAttribsClassName),
      CAttribMemberBase(cpy)
   {
   }
	//
	// Top level (non-linked)version of constructor.
	//
	CAttribsBase(	const CWStringStream &wsAttribsClassName, 
						const CWStringStream &wsDefaults = CWStringStream()
					) :
	   m_bIsAttribsMember(false),
	   m_wsAttribsClassName(wsAttribsClassName),
      CAttribMemberBase(wsAttribsClassName)
   {
		//SetDefaultValue(wsDefaults ) ;
   }

	//
	// added for attrib nesting feature. Self linking version of constructor.
	//
   CAttribsBase(SAttribMemberMap &mapAttribObjs,				// map object which this member will register itself in
	             const std::wstring &wsAttribsClassName = L"hrmph", // name of the derived attribs class
                const std::wstring &wsAttribsKeyName = L"gargle",		// keyname of this object used in map and in stream
                const CWStringStream &wsDefaults = CWStringStream()
	             ) :
	   m_bIsAttribsMember(true),
	   m_wsAttribsClassName(wsAttribsClassName),
      CAttribMemberBase(mapAttribObjs, wsAttribsKeyName)
	{
	}
	

	CAttribsBase & operator = (const CWStringStream &wsAttribsStream)
   {
		InitFromString(wsAttribsStream) ; 
      return *this ;
   }

	void ToString2(CWStringStream &wsOutVal) const
	{
	   TPropertiesMap map ;

		InitMapFromMembers(map) ;
		InitStreamFromMap(wsOutVal, map) ;
	}

	CWStringStream ToString() const
	{
	   CWStringStream wsRetVal ;

		ToString2(wsRetVal) ;
      return wsRetVal ;
	}

	void InitFromString(const CWStringStream &ws)
	{
      InitMapFromStream(ws) ;
      InitMembersFromMap() ;
	}
	TPropertiesMap ToMap() const
	{
		return m_mapAttribStream ;
	}
	void InitFromMap(const TPropertiesMap &pm)
	{
		m_mapAttribStream = pm ;
		InitMembersFromMap() ;
	}

 	virtual TPropertiesMap ToMapNoTags() const
	{
      SpdAttribs::SAttribMemberMap::const_iterator itPropertyObj(m_mapAttribObjs.begin()) ;
      SpdAttribs::TPropertiesMap mapRetVal ;
      //
      // Loop through each property member and add it to the stream
      //
      for (
         itPropertyObj = m_mapAttribObjs.begin() ; 
         itPropertyObj != m_mapAttribObjs.end() ; 
         itPropertyObj++
         )
      {
         std::wstring wsKeyName(itPropertyObj->second->GetKeyName()) ;
         CWStringStream wsValue ;
         //
         // Initialize wsValue. To do this, retrieve its data in string format
         // Then strip the tags from this stream.
         //
         itPropertyObj->second->FindDataForTags(
            wsKeyName,
				itPropertyObj->second->ToString(), // this returns data+tags
				wsValue, // this will be data w/o tags
            false
            ) ;

         mapRetVal.insert(std::make_pair(wsKeyName, wsValue)) ;
      }
      return mapRetVal ;
	}

   const std::wstring GetClassName() const { return m_wsKeyName ; }
   virtual const wchar_t* GetDescriptiveName() const
   {
	   return L"";
   }
   // private member methods
private:
   int compare(const CAttribMemberBase &cmp) const
   {
      int nRetVal(0) ;
      const CAttribsBase &cmpb((const CAttribsBase &)cmp ) ;

      //
      // first compare keyname of attribs objects
      //
      if (GetKeyName() == cmpb.GetKeyName())
      {
         SAttribMemberMap::const_iterator itAttribObj(m_mapAttribObjs.begin()) ;
         SAttribMemberMap::const_iterator itCmpAttribObj(cmpb.m_mapAttribObjs.begin()) ;
         //
         // key names are the same, start comparing the individual attrib objects.  This algorithm presumes
         // that for attribs of same key name, all attrib members will have matching key names.
         //
         for (
            itAttribObj = m_mapAttribObjs.begin(), itCmpAttribObj = cmpb.m_mapAttribObjs.begin() ;
            nRetVal == 0 && itAttribObj != m_mapAttribObjs.end() && itCmpAttribObj != cmpb.m_mapAttribObjs.end() ;
            itAttribObj++
            )
         {
            if (*itAttribObj->second == *itCmpAttribObj->second)
               ;
            else if (*itAttribObj->second < *itCmpAttribObj->second)
               nRetVal = -1 ;
            else
               nRetVal = 1 ;
         }
      }
      else if (GetKeyName() < cmpb.GetKeyName())
      {
         nRetVal = -1 ;
      }
      else
      {
         nRetVal = 1 ;
      }
      return nRetVal ;
   }
	void GetHtmlKeys(const std::wstring &wsStream, TPropertiesMap &pmOutVal) const
	{
      size_t stStartKeyPos(0) ;
      size_t stEndKeyPos(0) ;
		//
		// Make sure output is clear before continuing.
		//
		pmOutVal.clear() ;

		while (stStartKeyPos != std::wstring::npos)
      {
		   std::wstring wsKeyName ;
         CWStringStream wsTagData ;
         size_t stDataLen(0) ;
      
         // 1) find '<'
         stStartKeyPos = wsStream.find(L"<", stStartKeyPos) ;
         if (stStartKeyPos == wsStream.npos)
            break ;
         // 2) find '>'...  everthing between is the key name string
         stEndKeyPos = wsStream.find(L">", stStartKeyPos) ;
         if (stEndKeyPos == wsStream.npos)
            break ;

         wsKeyName = wsStream.substr(stStartKeyPos+1, stEndKeyPos-(stStartKeyPos+1)) ;
			
         stStartKeyPos = FindDataForTags(wsKeyName, wsStream, wsTagData, true, stStartKeyPos) ;
         if (stStartKeyPos != std::wstring::npos)
         {
			   // 
			   // Key integrity verfied (found a terminating key).  
            // Add KeyName and its value to the map.  Note that the value added
            // contains the keyname tags within it.
			   //
            pmOutVal.insert(std::make_pair(wsKeyName,wsTagData)) ;
			
			   //
			   // Update search position before re-entering the loop
			   //
			   stStartKeyPos += wsTagData.length() ;
         }
		}
	}
	void InitMapFromStream(TPropertiesMap &mapOutVal, const std::wstring &wsAttribsStream) const
	{
	   CWStringStream wsTempStream = wsAttribsStream;
      CWStringStream wsStreamData;
		
		//
		// Get data associated with this object's attributes tag and put it into
		// wsAttribsStream.
		//
		// If there are two layers of tags (<key_name><class_name>...</class_name></key_name> vs. <key_name>...</key_name>)
		if (m_bIsAttribsMember)
		{
		   FindDataForTags(m_wsKeyName, wsAttribsStream, wsTempStream ) ;
         if (wsTempStream.size() == 0)
		   {	//
		      // no settings for this attributes class are contained in the stream.
            // Note this should not be treated as an error
            //
            return ;
		   }
		}
		
      FindDataForTags(m_wsAttribsClassName, wsTempStream, wsStreamData ) ;
      if (wsStreamData.size() == 0)
      {	//
         // no settings for this attributes class are contained in the stream.
         // Note this should not be treated as an error
         //
         return;
      }
		else
		{  
         TPropertiesMap pmTempKeyValMap ; 
         TItPropertiesMap itKeyVal ;
         TItPropertiesMap itOut(mapOutVal.begin()) ;
         // 
			// Success, attributes class tag was found. This stream 
			// contains valid attributes values.
         //
			// retrieve top level key/values
			//
			GetHtmlKeys(wsStreamData, pmTempKeyValMap ) ;  
         //
         // Now merge them into this objects attribute map member.
         //
         for (itKeyVal = pmTempKeyValMap.begin(); itKeyVal != pmTempKeyValMap.end(); itKeyVal++)
         {
            std::wstring wsKey(itKeyVal->first) ;  // helps with debugging.
            
            itOut = mapOutVal.find(wsKey) ;
            if (itOut != mapOutVal.end())
            {	//
					// the key for this object already exists, update it's value
					//
               itOut->second = itKeyVal->second ;
            }
            else
            {	//
					// the key for this object was not in the attributes map list, add the key/value pair
					//
					mapOutVal.insert(*itKeyVal) ; 
            }
         }
      }
	}

	void InitMapFromStream(const std::wstring &wsAttribsStream) 
   {
		InitMapFromStream(m_mapAttribStream, wsAttribsStream) ;
   }


   void InitStreamFromMap(std::wstring &wsOutVal, const TPropertiesMap &mapKeyVals) const
   {
      SWStringMap::const_iterator itAttribStream(mapKeyVals.begin()) ;

      wsOutVal.clear() ; // initialize to empty string

	   if (m_bIsAttribsMember)
	   {
         wsOutVal += L"<" ;
         wsOutVal += m_wsKeyName/*m_wsAttribsKeyName*/ ;
         wsOutVal += L">\n" ;
	   }
	  
      wsOutVal += L"<" ;
      wsOutVal += m_wsAttribsClassName;
      wsOutVal += L">\n" ;
      
      // loop through the entire map
      for (itAttribStream = mapKeyVals.begin(); itAttribStream != mapKeyVals.end(); itAttribStream++)
      {
			//
         // For each item in mapKeyVals, Append data to the stream.  Note:  
			// This data is self contained XML Key/value 
			//
			// e.g. <PageNo>6</PageNo>
			//
         wsOutVal += itAttribStream->second ;
      }

      wsOutVal += L"</" ;
      wsOutVal += m_wsAttribsClassName;
      wsOutVal += L">\n" ;
      
	   if (m_bIsAttribsMember)
	   {
         wsOutVal += L"</" ;
         wsOutVal += m_wsKeyName/*m_wsAttribsKeyName*/ ;
         wsOutVal += L">\n" ;
	   }
   }
	void InitStreamFromMap(std::wstring &wsOutVal) const
   {
		InitStreamFromMap(wsOutVal, m_mapAttribStream) ;
   }
   void InitMapFromMembers(TPropertiesMap &mapOutVal ) const
   {  
      SAttribMemberMap::const_iterator itAttribObj(m_mapAttribObjs.begin()) ;
      SWStringMap::iterator itAttribStream ;
      for (
         itAttribObj = m_mapAttribObjs.begin() ; 
         itAttribObj!=m_mapAttribObjs.end() ; 
         itAttribObj++
         )
      {
         itAttribStream = mapOutVal.find(itAttribObj->first) ;
         if (itAttribStream != mapOutVal.end())
         {	//
				// the key for this object already exists, update it's value.
				//
            itAttribStream->second = (CWStringStream) *itAttribObj->second ;
         }
         else
         {	//
				// the key for this object was not in the map list, add the object
				//
	
				//
				// First check to see if the value of the object has changed from the default value.  If
				// it has not, then do not add this object's key to the map.
				//
				//if (!itAttribObj->second->IsDefaultValue())
				{	//
					// Only add this object to the map if it has changed from the default.
					// SLB 15.mar.2005 - removed default value support.
               //
	            mapOutVal.insert(std::make_pair(itAttribObj->first, (CWStringStream) *itAttribObj->second)) ;
				}
         }
      }
   } // InitMapFromMembers()


   void InitMapFromMembers() 
   {  // For this routine "Map" represents the m_mapAttribStream object
		InitMapFromMembers(m_mapAttribStream ) ;
   } // InitMapFromMembers()


   void InitMembersFromMap() 
   {
      SAttribMemberMap::iterator itAttribObj ;
      SWStringMap::iterator itAttribStream(m_mapAttribStream.begin()) ;

      for (
         itAttribStream = m_mapAttribStream.begin() ; 
         itAttribStream != m_mapAttribStream.end() ; 
         itAttribStream++
         )
      {  // cycle through all key/value pairs in the stream map.
         itAttribObj = m_mapAttribObjs.find(itAttribStream->first) ;
         if (itAttribObj != m_mapAttribObjs.end())
         { // the key for this object exists, update it's value
            *itAttribObj->second = (CWStringStream)itAttribStream->second ;
         }
         else
         { // the key for this object was not in the map list, ignore it.
         }
      }
   } // InitMembersFromMap()
} ;





//*************************************************************************************
//
// Attribute member typedefs.
//
// These types are used in Attributes classes.
//
//
//*************************************************************************************

//
// basic types
//
typedef CAttribMemberBaseU<WStringObj, std::wstring>  WStringAttrib ;
typedef CAttribMemberBaseU<QwordObj, QUADWORD>        QwordAttrib ;
typedef CAttribMemberBaseU<DwordObj, DWORD>           DwordAttrib ;
typedef CAttribMemberBaseU<WordObj, WORD>             WordAttrib ;
typedef CAttribMemberBaseU<ByteObj, BYTE>             ByteAttrib ;
typedef CAttribMemberBaseU<IntObj, __int32>           IntAttrib ;
typedef CAttribMemberBaseU<QintObj, __int64>          QintAttrib ;
typedef CAttribMemberBaseU<SintObj, __int16>          SintAttrib ;
typedef CAttribMemberBaseU<CharObj, char>             CharAttrib ;
typedef CAttribMemberBaseU<BoolObj, bool>             BoolAttrib ;
typedef CAttribMemberBaseU<Time_tObj, time_t>         Time_tAttrib ;
typedef CAttribMemberBaseU<Size_tObj, size_t>         Size_tAttrib ;
typedef CAttribMemberBaseU<BufferObj, CBuffer>        BufferAttrib ;
typedef CAttribMemberBaseU<DoubleObj, double>         DoubleAttrib ;
typedef CAttribMemberBaseU<SystemTimeObj, SYSTEMTIME> SystemTimeAttrib ;

//
// Map type variations
//
typedef CAttribMemberBaseU<MapStringStringObj, TMapStringString>  MapStringStringAttrib ;
typedef CAttribMemberBaseU<MapStringDwordObj, TMapStringDword>    MapStringDwordAttrib ;
typedef CAttribMemberBaseU<MapDwordStringObj, TMapDwordString>    MapDwordStringAttrib ;
typedef CAttribMemberBaseU<MapDwordDwordObj, TMapDwordDword>      MapDwordDwordAttrib ;
typedef CAttribMemberBaseU<MapStringBufferObj, TMapStringBuffer>  MapStringBufferAttrib ;
typedef CAttribMemberBaseU<MapBufferBufferObj, TMapBufferBuffer>  MapBufferBufferAttrib ;
typedef CAttribMemberBaseU<MapStringAttributesObj, TMapStringAttributes>  MapStringAttributesAttrib ;

//
// Vector type variations
//
typedef CAttribMemberBaseU<VectorStringObj, TVectorString>     VectorStringAttrib ;
typedef CAttribMemberBaseU<VectorDwordObj, TVectorDword>       VectorDwordAttrib ;
typedef CAttribMemberBaseU<VectorBufferObj, TVectorBuffer>     VectorBufferAttrib ;
typedef CAttribMemberBaseU<VectorAttributesObj, TVectorAttributes>   VectorAttributesAttrib ;
} // end of namespace SpdAttribs  
