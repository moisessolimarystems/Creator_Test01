//
// CSpdJobTransaction.cpp
// 
//
#include "SpdAttribsCPP\CSpdJobTransactionModel.h"
#include "SpdAttribsCPP\CDJobInfoAttribs.h"                                         // CR.26999.Item60; added.

#include "SpdUtils.h"

using namespace SpdException; 

namespace SpdJobTransaction
{
/*
 * statics
 * SLB 18.dec.2018 CR.7698; fixes so that description of a transaction property can be retrieved from its Id.
 */
CSpdJobTransaction::TTransactionInfoMap CSpdJobTransaction::s_TInfoMap = CSpdJobTransaction::BuildInfoMap(); 

/* EnumToString() - static method. TActivityState overload.
 * 
 * SLB 19.may.2019 CR.26999.Item60; created.
 ---------------------------------------------------------------------------------------------*/
CSpdCString CSpdJobTransaction::EnumToString(TActivityState as)
{
   switch (as)
   {
   case asActive: return L"Active";
   case asCompleted: return L"Completed";
   }
   return L"Invalid";
}


/* EnumToString() - static method. TCacheState overload.
 * 
 * SLB 19.may.2019 CR.26999.Item60; created.
 ---------------------------------------------------------------------------------------------*/
CSpdCString CSpdJobTransaction::EnumToString(TCacheState cs)
{
   switch (cs)
   {
   case csAdd: return L"Add";
   case csRemove: return L"Remove";
   }
   return L"Invalid";
}

/* EnumToString() - static method. TDJobState overload.
 * 
 * SLB 19.may.2019 CR.26999.Item60; created.
 ---------------------------------------------------------------------------------------------*/
CSpdCString CSpdJobTransaction::EnumToString(TDJobState djs)
{
   return CDJobStateAttribs::EnumToString((CDJobStateAttribs::TDJobState)djs);
}

/* EnumToString() - static method. TCompletionCode overload.
 * 
 * SLB 19.may.2019 CR.26999.Item60; created.
 ---------------------------------------------------------------------------------------------*/
CSpdCString CSpdJobTransaction::EnumToString(TCompletionCode cc)
{
   return CDJobCompleteAttribs::CCompletion::EnumToString((CDJobCompleteAttribs::CCompletion::TCompletionCode)cc);
}

/* SetMemberRaw() - 
 * 
 * SLB 23.oct.2015 CR.18552; created.
 * SLB 11.dec.2015 CR.18552.V01; Change so that anytime a member gets set the Modified flag gets 
 * set to true.
 * SLB 28.apr.2016 CR.18552.V05; Modified string type from std::wstring to CSpdCString.
---------------------------------------------------------------------------------------------*/
void CSpdJobTransaction::SetMemberRaw(
   TTransactionPropertyId tpi, 
   const CSpdCString &wsVal)                                                        // CR.18552.V05; modified.
{
   m_Properties[tpi] = wsVal; 
   m_bModified = true;                                                              // CR.18552.V01; added.
}

/* GetMember() - 
 * 
 * SLB 23.oct.2015 CR.18552; created.
 * SLB 18.dec.2015 CR.18552.V02; Numeric values streamed in hex vs. dec.
---------------------------------------------------------------------------------------------*/
bool CSpdJobTransaction::GetMember(
   TTransactionPropertyId tpi, 
   QUADWORD &val) const
{
   TItcPropertyList itVal(m_Properties.find(tpi)); 
   if (itVal==m_Properties.end()) return false; 

   val = _wcstoui64(itVal->second.c_str(), NULL, 16);                               // CR.18552.V02; modified.
   return true; 
}

/* SetMember() - 
 * 
 * SLB 23.oct.2015 CR.18552; created.
 * SLB 18.dec.2015 CR.18552.V02; Numeric values streamed in hex vs. dec.
---------------------------------------------------------------------------------------------*/
void CSpdJobTransaction::SetMember(
   TTransactionPropertyId tpi, 
   QUADWORD val) 
{
   wchar_t wcsVal[64]; 
   _i64tow_s(val, wcsVal, 64, 16);                                                  // CR.18552.V02; modified.

   SetMemberRaw(tpi, wcsVal); 
}

/* GetMember() - 
 * 
 * SLB 23.oct.2015 CR.18552; created.
 * SLB 28.apr.2016 CR.18552.V05; Modified string type from std::wstring to CSpdCString.
---------------------------------------------------------------------------------------------*/
bool CSpdJobTransaction::GetMember(
   TTransactionPropertyId tpi, 
   CSpdCString &val) const                                                          // CR.18552.V05; modified.
{
   TItcPropertyList itVal(m_Properties.find(tpi)); 
   if (itVal==m_Properties.end()) return false; 

   val = SpdUtils::XmlUnescapeString(itVal->second); 
   return true; 
}

/* SetMember() - 
 * 
 * SLB 23.oct.2015 CR.18552; created.
 * SLB 28.apr.2016 CR.18552.V05; Modified string type from std::wstring to CSpdCString.
---------------------------------------------------------------------------------------------*/
void CSpdJobTransaction::SetMember(
   TTransactionPropertyId tpi, 
   const CSpdCString &val)                                                          // CR.18552.V05; modified.
{
   CSpdCString scsEscapedVal(SpdUtils::XmlEscapeString(val));                       // CR.18552.V05; modified.

   SetMemberRaw(tpi, scsEscapedVal); 
}



/* ReallocateWcsBuffer() - 
*
* SLB 06.feb.2008 Modified to catch memory allocation exception.
---------------------------------------------------------------------------*/
inline wchar_t *ReallocateWcsBuffer(
   wchar_t *pwcBufferToCopy, 
   size_t stBuffSize, 
   size_t &stNewBufSize,
   bool bFreeCopyBufferOnError,
   bool bDontCopy = false)
{
   wchar_t *pwcNewBuffer(NULL); 
   stNewBufSize = pwcBufferToCopy ? 2*stBuffSize : stBuffSize; 
   try
   {
      pwcNewBuffer = new wchar_t[stNewBufSize]; 
   }
   catch(...) {}
   if (!pwcNewBuffer) 
   {  //
      // problem: failed to allocate a new buffer.
      //
      if (bFreeCopyBufferOnError && pwcBufferToCopy!=NULL)
         delete [](pwcBufferToCopy); 
      throw (hr_exception(E_POINTER, L"Failed to allocate buffer. Out of memory.", __WFUNCTION__));      
   }
   pwcNewBuffer[0] = 0; 
   if (pwcBufferToCopy!=NULL)
   {
      if (pwcNewBuffer && !bDontCopy)
      {
         wmemcpy(pwcNewBuffer, pwcBufferToCopy, stBuffSize); 
      }
      delete [](pwcBufferToCopy); 
   }
   return pwcNewBuffer; 
}

/* Serialize()
*
* SLB 28.mar.2016 CR.18552.V04; Added this comment header.
---------------------------------------------------------------------------------------------------------*/
CSpdCString CSpdJobTransaction::Serialize() const
{
   size_t stBufLen(16384); 
   wchar_t *pwcs(NULL); 
   while (true)
   {
      pwcs = ReallocateWcsBuffer(pwcs, stBufLen, stBufLen, true); 
      size_t stCharWriteLen;
      try { Serialize(pwcs, stBufLen, stCharWriteLen);  }
      catch (...) { continue;  }
      break; 
   }
   CSpdCString ws(pwcs); 
   delete[] (pwcs); 
   return ws; 
}


/* Serialize() - serializes the class into psuedo xml. Note: all values should be escaped already.
   *
   * Xml is in following format:
   * 
   *  <ActivityEntry>
   *     <aid>SpdeActivityId</aid>
   *     <amn>SpdeActivityMachineName</amn>
   *     <as>Activity State</as>
   *     <cs>Cache State</cs>
   *     <pl>
   *        Serialized Activity Properties
   *     </pl>
   *  </ActivityEntry>
   * Serialized Activity Properties looks like:
   *
   *    <N><K>tpi</K><V>val</V></N>               // N=Node, K=Key, V=Value
   *    .
   *    .
   *    .
   * 
---------------------------------------------------------------------------------------------*/
void CSpdJobTransaction::Serialize(
   wchar_t *pwcsBuf, 
   size_t stBufCharLen, 
   size_t &stCharWriteLen) const
{
   DWORD dwWriteOff(0); 
   int nWriteLen(0); 
   nWriteLen = _snwprintf_s(
      pwcsBuf+dwWriteOff, stBufCharLen-dwWriteOff, _TRUNCATE,
      L"<ActivityEntry><aid>%I64u</aid><as>%u</as><cs>%u</cs><djs>%u</djs><pl>\n",
      (QUADWORD)GetSpdeActivityId(),
      (DWORD)GetActivityState(),
      (DWORD)GetCacheState(),
      (DWORD)GetDJobState()); 
   if (nWriteLen<0) throw (hr_exception(E_FAIL, L"Insufficient buffer size for <ActivityEntry> key", __WFUNCTION__)); 
   dwWriteOff+=nWriteLen; 
   //
   // Now add in property entries.
   //
   for (TItcPropertyList it(m_Properties.begin()); it != m_Properties.end(); it++)
   {
      nWriteLen = _snwprintf_s(
         pwcsBuf+dwWriteOff, stBufCharLen-dwWriteOff, _TRUNCATE,
         L"<N><K>%u</K><V>%s</V></N>",
         it->first,
         it->second.c_str()); 
      if (nWriteLen<0) throw (hr_exception(E_FAIL, L"Insufficient buffer size for Activity's Property Node.", __WFUNCTION__)); 
      dwWriteOff+=nWriteLen; 
   }
   //
   // Add closing tags
   //
   nWriteLen = _snwprintf_s(
      pwcsBuf+dwWriteOff, stBufCharLen-dwWriteOff, _TRUNCATE,
      L"\n</pl></ActivityEntry>"); 
   if (nWriteLen<0) throw (hr_exception(E_FAIL, L"Insufficient buffer size for terminating </ActivityEntry> key", __WFUNCTION__)); 
   dwWriteOff+=nWriteLen; 
   stCharWriteLen = dwWriteOff; 
}

/* GetMaxSerializeCharLen() - Generates approx buffer size needed to serialize this object.
   * 
   * SLB 22.oct.2015 CR.18552; created.
---------------------------------------------------------------------------------------------*/
size_t CSpdJobTransaction::GetMaxSerializeCharLen() const
{
   //
   // initialize chars needed with count for <JT> + </JT>
   //
   size_t stCharLenNeeded(9);              
   //
   // Now add in space needed for each property's node tags AND its Key. Presuming keys will never
   // exceed 5 digits.
   //
   stCharLenNeeded += (21+5)*m_Properties.size(); 
   //
   // Now add in length of value strings.
   //
   for (TItcPropertyList it(m_Properties.begin()); it != m_Properties.end(); it++)
   {
      stCharLenNeeded+=it->second.size(); 
   }
   return stCharLenNeeded; 
}

/* Deserialize() - Initializes this object from an xml stream. Note: escaped values should 
 * remain so.
 * 
 * SLB 22.oct.2015 CR.18552; created.
 * SLB 28.apr.2016 CR.18552.V05; Modified string type from std::wstring to CSpdCString.
---------------------------------------------------------------------------------------------*/
void CSpdJobTransaction::Deserialize(
   const CSpdCString &wsXmlBuf)                                                     // CR.18552.V05; modified.
{
   CSpdCString wsMapData;                                                           // CR.18552.V05; modified.
   size_t stLen; 
   stLen = FindDataForTags(L"ActivityEntry", wsXmlBuf, wsMapData); 
   if (stLen == std::wstring::npos) throw (hr_exception(E_FAIL, L"Corrupt XML buffer. Tag: <JT>", __WFUNCTION__)); 

   //
   // retrieve ActivityID, ActivityState and CacheState.
   //
   CSpdCString wsItemData;                                                          // CR.18552.V05; modified.
   stLen = FindDataForTags(L"aid", wsMapData, wsItemData); 
   if (stLen == std::wstring::npos) throw (hr_exception(E_FAIL, L"Corrupt XML buffer. Tag: <aid>", __WFUNCTION__)); 
   QUADWORD qwItem(_wtoi64(wsItemData.c_str())); 
   SetSpdeActivityId(qwItem); 

   stLen = FindDataForTags(L"as", wsMapData, wsItemData); 
   if (stLen == std::wstring::npos) throw (hr_exception(E_FAIL, L"Corrupt XML buffer. Tag: <as>", __WFUNCTION__)); 
   int nItem(_wtoi(wsItemData.c_str())); 
   SetActivityState((TActivityState)nItem); 

   stLen = FindDataForTags(L"cs", wsMapData, wsItemData); 
   if (stLen == std::wstring::npos) throw (hr_exception(E_FAIL, L"Corrupt XML buffer. Tag: <cs>", __WFUNCTION__)); 
   nItem = _wtoi(wsItemData.c_str()); 
   SetCacheState((TCacheState)nItem); 

   stLen = FindDataForTags(L"djs", wsMapData, wsItemData); 
   if (stLen == std::wstring::npos) throw (hr_exception(E_FAIL, L"Corrupt XML buffer. Tag: <djs>", __WFUNCTION__)); 
   nItem = _wtoi(wsItemData.c_str()); 
   SetDJobState((TDJobState)nItem); 

   size_t stNodeSearchPos(0); 
   while (stNodeSearchPos != std::wstring::npos)
   {
      //
      // Each iteration retrieves one Node which contains a key/value pair.  Retrieve 
      // Node data and set search position to position after the end tag.
      //
      CSpdCString wsNodeData;                                                       // CR.18552.V05; modified.
      stNodeSearchPos = FindDataForTags(L"N", wsMapData, wsNodeData, false, stNodeSearchPos); 
      if (stNodeSearchPos == std::wstring::npos) break; 


      CSpdCString wsKeyData;                                                        // CR.18552.V05; modified.
      stLen = FindDataForTags(L"K", wsNodeData, wsKeyData); 
      if (stLen == std::wstring::npos) throw (hr_exception(E_FAIL, L"Corrupt XML buffer. Tag: <K>", __WFUNCTION__)); 

      CSpdCString wsValueData;                                                      // CR.18552.V05; modified.
      stLen = FindDataForTags(L"V", wsNodeData, wsValueData, false, stLen); 
      if (stLen == std::wstring::npos) throw (hr_exception(E_FAIL, L"Corrupt XML buffer. Tag: <V>", __WFUNCTION__)); 
      //
      // data has been retrieved from xml... Now store it.
      //
      TTransactionPropertyId tpi((TTransactionPropertyId)_wtoi(wsKeyData.c_str())); 
      SetMemberRaw(tpi, wsValueData); 
   }
}

/* FindDataForTags() - returns xml data for given XML tag id.
 * returns offset in buffer where search completed.
 * 
 * SLB 23.oct.2015 CR.18552; created.
 * SLB 28.apr.2016 CR.18552.V05; Modified string type from std::wstring to CSpdCString.
---------------------------------------------------------------------------------------------*/
size_t CSpdJobTransaction::FindDataForTags(
   const CSpdCString &wsTagId,                                                      // CR.18552.V05; modified.
   const CSpdCString &wsData,                                                       // CR.18552.V05; modified.
   CSpdCString &wsOutVal,                                                           // CR.18552.V05; modified.
   bool bKeepTags,
   size_t stSearchStartPos) const
{
   size_t stRetVal(CSpdCString::npos); 
   CSpdCString wsTokenTag(wsTagId + CSpdCString(L">"));                             // CR.18552.V05; modified.
   CSpdCString wsOpenTag(CSpdCString(L"<") + wsTokenTag);                           // CR.18552.V05; modified.
   size_t stStartPos(stSearchStartPos); 
   size_t stEndPos(stSearchStartPos); 
   //
   // clear output
   //
   wsOutVal.clear(); 

   stStartPos = stRetVal = wsData.find(wsOpenTag, stSearchStartPos); 
   if (stStartPos == CSpdCString::npos) return CSpdCString::npos; 
   //
   // found the open tag.
   // Adjust the search position and increment the tag count semaphore.
   //
   DWORD dwTagCount(0); 
   stEndPos = stStartPos + 1;  // add one  because wsOpenTag.length() == wsTokenTag.length() + 1
   dwTagCount++; 
   while (dwTagCount)
   {
      stEndPos = wsData.find(wsTokenTag, stEndPos + wsTokenTag.size()); 
      //
      // if eos encountered, no more tags found. break out of the loop.
      //
      if (stEndPos == CSpdCString::npos) break; 
      //
      // Tag token found...
      // If opening tag, increment tag count.
      //
      if (wsData[stEndPos-1] == L'<') { dwTagCount++;  continue;  }
      //
      // If closing tag, decrement tag count.
      //
      if (wsData[stEndPos-1] == L'/') { dwTagCount--;  continue;  }
   } 
   //
   // if Tagcount is not zero, xml is not clean. Could not match all opening tags with
   // closing tags.
   //
   if (dwTagCount) return CSpdCString::npos; 
   //
   // Success! Semaphore is zero. A matching close tag has been found for the first open tag.
   // Calculate/save return value.
   //
   size_t stSearchEndPos(stEndPos + wsTokenTag.size()); 
   //
   // Copy data between tags to output buffer. If keeping tags, add token tag length.
   // If not Keeping tags, adjust startpos and sub 2 because token tag length == close tag length-2
   //
   if (bKeepTags) stEndPos += wsTokenTag.size(); 
   else { stStartPos += wsOpenTag.size();  stEndPos -= 2;  }
   //
   // populate output buffer.
   // 
   wsOutVal = wsData.substr(stStartPos, stEndPos - stStartPos); 
   return stSearchEndPos; 
}


/* _GetTpiDescription() - static method which returns the text description for the specified Transaction Propertry Id
 * 
 * SLB 26.oct.2015 CR.18552; created.
 * SLB 28.apr.2016 CR.18552.V05; Modified string type from std::wstring to CSpdCString.
---------------------------------------------------------------------------------------------*/
CSpdCString CSpdJobTransaction::GetDescription(
   TTransactionPropertyId tpi)          // CR.18552.V05; modified.
{
   TItTransactionInfoMap it(s_TInfoMap.find(tpi)); 
   if (it == s_TInfoMap.end()) return L"Invalid tpId specified."; 

   return it->second.m_wsDescription; 
}


}
