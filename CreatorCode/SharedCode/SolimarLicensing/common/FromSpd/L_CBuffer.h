#pragma once

#ifndef _CBuffer_h																						// CR.33897; added
#define _CBuffer_h																						// CR.33897; added.

#include "L_SpdStreamable4.h"                                                       // CR.21547; added.
using namespace SpdStreamable;                                                      // CR.21547; added.

//*************************************************************************************
//
// class CBuffer.
//
// SLB 14.apr.2025 CR.32662.V03; Changes required to work with attribs code imported 
// from Licensing. Namely the at() const; vs at(); methods.
//*************************************************************************************
class CBuffer
{
private: // private members
   size_t m_stBufLen; 
   PBYTE m_pbBuf; 

public: // public members
   CBuffer() : m_stBufLen(0), m_pbBuf(NULL) {}
	CBuffer(
		const BYTE* pb, 																					// CR.32662.V03; modified. 
		size_t st) :
		m_stBufLen(0),
		m_pbBuf(NULL)
	{
      // will create a copy of the buffer and not
      // touch the original
      SetBuffer(pb,st); 
   }
   CBuffer(const CBuffer &cpy) : m_stBufLen(0), m_pbBuf(NULL)
   {
      operator =(cpy); 
   }
   //
   // SLB 06.may.2016 CR.21547; Added constructor which takes a string. CAttributeComplexPrimitive wrapper now requires 
   // that the ComplexPrimitive type be given a default value from a string.
   //
   CBuffer(const CSpdCString &wsDefault) : 
		m_stBufLen(0), 
		m_pbBuf(NULL)
   {
      if (wsDefault.empty()) return; 

      size_t stSize(wsDefault.size() / 2); 
      BYTE *pbBuf = new BYTE[stSize]; 
      size_t stOffset(0); 
      CSpdCString wsOneByte; 
      for ( 
         wsOneByte = wsDefault.substr(2 * stOffset, 2); 
         wsOneByte.size() == 2; 
         stOffset++, wsOneByte = wsDefault.substr(2 * stOffset, 2))
      {
         pbBuf[stOffset] = (BYTE)_wcstoui64(wsOneByte.c_str(), NULL, 16); 
      }
      SetBuffer(pbBuf, stSize); 
      delete[] (pbBuf); 
   }
   ~CBuffer()
   {
      if (m_pbBuf) delete[] (m_pbBuf); 
      m_pbBuf = NULL; 
      m_stBufLen = 0; 
   }
   CBuffer &operator =(const CBuffer &cpy)
   {
      SetBuffer(cpy.m_pbBuf, cpy.m_stBufLen); 
      return *this; 
   }
   bool operator ==(const CBuffer &cpy) const 
   {
      if (m_stBufLen != cpy.m_stBufLen) return false; 
      if (memcmp(m_pbBuf, cpy.m_pbBuf, m_stBufLen)) return false; 
      return true; 
   }
   bool operator !=(const CBuffer &cpy) const { return !(operator ==(cpy)); }
   bool operator >(const CBuffer &cpy) const
   {
      if (m_stBufLen > cpy.m_stBufLen) return true; 
      return false; 
   }
   bool operator <(const CBuffer &cpy) const
   {
      if (m_stBufLen < cpy.m_stBufLen) return true; 
      return false; 
   }
   PBYTE data() const { return m_pbBuf; }
   size_t size() const { return m_stBufLen; }

//private: 
   // SLB 21.sep.2006 This does not need to be a private member:)  Made it public.
	void SetBuffer(
		const BYTE* pb, 																					// CR.32662.V03; modified.
		size_t st)
	{
      if (m_pbBuf) delete[] (m_pbBuf);  

      if (pb && st)
      {
         m_pbBuf = new BYTE [st]; 
         m_stBufLen = st; 
         memcpy(m_pbBuf, pb, st); 
      }
      else
      {
         // CR.Fix.29288.v01: Added special case for NULL buffer.
         m_pbBuf = NULL;
         m_stBufLen = 0;
      }
   }
   void Clear() { SetBuffer(NULL, 0); }
}; 

#endif