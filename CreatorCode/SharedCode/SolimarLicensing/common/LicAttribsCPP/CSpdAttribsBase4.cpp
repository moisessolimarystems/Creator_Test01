// CSpdAttribsBase4.cpp - Implements Attributes v4 base functionality.
// 
// SLB 11.jun.2024 CR.32662.V03; Imported from SPDE and modified to work in the 
// Licensing environment.

#include "L_CSpdAttribsBase4.h"
#include "..\FromSpd\L_CSpdJobTransactionTypes.h"

#include "time.h"
using namespace SpdAttribs4; 


/*
 *
 * class CStreamableIf methods.
 *
 */


 /* SerializeBegin() - static method.
 ---------------------------------------------------------------------------------------*/
size_t CStreamableIf::SerializeBegin(wchar_t* pwcsBuf, size_t stBufCharLen, bool bUseTag, bool bUseContainerTag) const
{
	if (!bUseContainerTag) return 0;

	int nWriteLen(0);
	size_t stWriteOff(0);
	if (bUseTag && GetTagName().size())
	{
		nWriteLen = _snwprintf_s(
			pwcsBuf + stWriteOff, stBufCharLen - stWriteOff, _TRUNCATE,
			L"<%s>",
			GetTagName().c_str());
		if (nWriteLen < 0) throw (hr_exception(E_FAIL, L"Insufficient buffer size for begin key", __WFUNCTION__));
		stWriteOff += nWriteLen;
	}
	return stWriteOff;
}

/* SerializeEnd() - static method.
---------------------------------------------------------------------------------------*/
size_t CStreamableIf::SerializeEnd(wchar_t* pwcsBuf, size_t stBufCharLen, bool bUseTag, bool bUseContainerTag) const
{
	if (!bUseContainerTag) return 0;

	int nWriteLen(0);
	size_t stWriteOff(0);
	if (bUseTag && GetTagName().size())
	{
		nWriteLen = _snwprintf_s(
			pwcsBuf + stWriteOff, stBufCharLen - stWriteOff, _TRUNCATE,
			L"</%s>",
			GetTagName().c_str());
		if (nWriteLen < 0) throw (hr_exception(E_FAIL, L"Insufficient buffer size for end key", __WFUNCTION__));
		stWriteOff += nWriteLen;
	}
	return stWriteOff;
}

/* FindDataForTags() - static method.
---------------------------------------------------------------------------------------*/
size_t CStreamableIf::FindDataForTags(
	const CSpdCString& scsTag,
	const CSpdCString& wsData,
	CSpdCString& wsOutVal,
	bool bKeepTags,
	size_t stSearchStartPos)
{
	if (wsData.empty()) return CSpdCString::npos;

	size_t stRetVal(CSpdCString::npos);
	CSpdCString wsTokenTag(scsTag);  wsTokenTag.append(CSpdCString(L">"));
	CSpdCString wsOpenTag(CSpdCString(L"<") + wsTokenTag);
	if (scsTag.empty())
	{
		wsTokenTag.clear();
		wsOpenTag.clear();
	}
	size_t stStartPos(stSearchStartPos);
	size_t stEndPos(stSearchStartPos);
	DWORD dwTagCount(0);

	// clear output
	wsOutVal.clear();

	stStartPos = stRetVal = 0;
	if (wsOpenTag.size()) stStartPos = wsData.find(wsOpenTag, stSearchStartPos);

	if (stStartPos == CSpdCString::npos) return CSpdCString::npos;

	// found the open tag.
	// Adjust the search position and increment the tag count semaphore.
	stEndPos = stStartPos + 1;  // add one  because wsOpenTag.length() == wsTokenTag.length() + 1
	dwTagCount++;

	while (dwTagCount && !wsTokenTag.empty())
	{
		if (scsTag.empty()) { dwTagCount = 0; break; }
		stEndPos = wsData.find(wsTokenTag, stEndPos + wsTokenTag.size());

		// make sure a tag was found.
		if (stEndPos == CSpdCString::npos) break;

		// opening tag?
		if (wsData[stEndPos - 1] == L'<') { dwTagCount++; continue; }

		// closing tag?
		if (wsData[stEndPos - 1] == L'/') { dwTagCount--; continue; }

		// This is just data that looks like a tag! continue.
	} // end of tag search loop.

	// if Tagcount is not zero, could not match all opening tags with closing tags. xml is not clean.
	if (dwTagCount) return CSpdCString::npos;
	stRetVal = stEndPos + wsTokenTag.size();

	// semaphore is zero.
	// A matching close tag has been found for the first open tag.
	// Copy data between tags to output buffer.
	if (bKeepTags)
	{
		// keeping tags...
		// add the tag length to the end position.
		stEndPos += wsTokenTag.size();
	}
	else
	{
		// not keeping tags...
		// Add the tag length to the start position.
		stStartPos += wsOpenTag.size();
		stEndPos -= 2;  // sub 2 because token tag length == close tag length - 2
	}

	// populate output buffer.
	//wsOutVal = wsData.substr(stStartPos, stEndPos - stStartPos); 
	wsData.substr(wsOutVal, stStartPos, stEndPos - stStartPos);
	return stRetVal;
}



#if 0

//
// SLB 06.jul.2006 Added to reduce memory footprint of objects using property members,
// created this type to represent a unique member.  This is then used to store
// and retrieve that members serializing keys.
// SLB 19.mar.2019 CR.27575; added support for shortcut versions of string variables... referred to as 
// "Alternate" string variables.
// SLB 17.sep.2019 CR.28299; Added support for yet another alternate string variable key. Needed for Input Job name.
// Also, improved support of legacy string variable strings... In legacy code, string variable key could be as long as
// use wanted, as long as the first three chars matched the required key. Included support for this condition with 
// these changes.
// SLB 09.oct.2020 CR.29830; Added support for yet another alternate string variable key. Needed for Input Job name (again) to
// support "I" as the key value.
//
class CAttribPropertyIds
{
public:
   const CSpdCString m_wsKeyName; 
   const CSpdCString m_wsLegacyKeyName; 
   const CSpdCString m_wsStringVarKey;                                                 // CR.7698; added.
   const CSpdCString m_wsStringVarKey_Alt;                                             // CR.27575; added.
   const CSpdCString m_wsStringVarKey_Alt1;                                            // CR.28299; added.
   const CSpdCString m_wsStringVarKey_Alt2;                                            // CR.29830; added.
   const DBSpoolJobAttributeID m_jaDbsJaId;
   const TTransactionPropertyId m_tpiPropertyId;                                       // CR.18552; added.
   const TEnginePropertyIds m_epiPropertyId; 
   bool m_bIsReadOnlyForClient; 

private:
   CAttribPropertyIds() :
      m_jaDbsJaId(),
      m_tpiPropertyId(),
      m_epiPropertyId()
      {} // prevent use of default constructor.

public:
   CAttribPropertyIds(const wchar_t *pwcsKeyName, const wchar_t *pwcsLegacyKeyName, const wchar_t *pwcsStringVarKey, const wchar_t *pwcsStringVarKey_Alt, const wchar_t *pwcsStringVarKey_Alt1, const wchar_t *pwcsStringVarKey_Alt2, DBSpoolJobAttributeID jaDbsJaId, TTransactionPropertyId tpId, TEnginePropertyIds epiId, bool bCReadOnly=false) :
      m_wsKeyName(pwcsKeyName),
      m_wsLegacyKeyName(pwcsLegacyKeyName),
      m_wsStringVarKey(pwcsStringVarKey),
      m_wsStringVarKey_Alt(pwcsStringVarKey_Alt),                                   // CR.27575; added.
      m_wsStringVarKey_Alt1(pwcsStringVarKey_Alt1),                                 // CR.28299; added.
      m_wsStringVarKey_Alt2(pwcsStringVarKey_Alt2),                                 // CR.29830; added.
      m_jaDbsJaId(jaDbsJaId),
      m_tpiPropertyId(tpId),
      m_epiPropertyId(epiId),
      m_bIsReadOnlyForClient(bCReadOnly)
   {
      // CR.28299; ensure that all string variable keys have max len of 3 chars.
      if (m_wsStringVarKey.wcslen() > 3) (CSpdCString &)m_wsStringVarKey = m_wsStringVarKey.substr(0, 3);
      if (m_wsStringVarKey_Alt.wcslen() > 3) (CSpdCString &)m_wsStringVarKey_Alt = m_wsStringVarKey_Alt.substr(0, 3);
      if (m_wsStringVarKey_Alt1.wcslen() > 3)(CSpdCString &) m_wsStringVarKey_Alt1 = m_wsStringVarKey_Alt1.substr(0, 3);
      if (m_wsStringVarKey_Alt2.wcslen() > 3)(CSpdCString &) m_wsStringVarKey_Alt2 = m_wsStringVarKey_Alt2.substr(0, 3); // CR.29830; added.
   }
   CAttribPropertyIds(const CAttribPropertyIds &c) :
      m_wsKeyName(c.m_wsKeyName),
      m_wsLegacyKeyName(c.m_wsLegacyKeyName),
      m_wsStringVarKey(c.m_wsStringVarKey),
      m_wsStringVarKey_Alt(c.m_wsStringVarKey_Alt),                                 // CR.27575; added.
      m_wsStringVarKey_Alt1(c.m_wsStringVarKey_Alt1),                               // CR.28299; added.
      m_wsStringVarKey_Alt2(c.m_wsStringVarKey_Alt2),                               // CR.29830; added.
      m_jaDbsJaId(c.m_jaDbsJaId),
      m_tpiPropertyId(c.m_tpiPropertyId),
      m_epiPropertyId(c.m_epiPropertyId),
      m_bIsReadOnlyForClient(c.m_bIsReadOnlyForClient)
   {}
}; 
   
//----------------------------------------------------------------------------------
//  CPropertyIdMap
//
// SLB 06.jul.2006 Added to reduce memory footprint of objects using  
// property members, created this class to initialize the  Keyname lookup table.
//
// SLB 28.apr.2010 CR.12758; relevant AddProperty() calls now specify a DbsJobPropertyId. 
// SLB 26.jul.2011 CR.14962; Added entries for CMediumInfoJobProperty class in CInitJobPropertyIdMap()
// method. 
// SLB 08.nov.2011 CR.14349; Added entries for CDBSpoolJobParams, CDBSpoolResourceControl and CMediumInfoContainer. All
// tags are for updated DBSpool Messages.
// SLB 04.dec.2015 CR.18552; Added support for mapping job properties to transaction properties.
// SLB 03.dec.2018 CR.7698; Add support for associationg string variable keys with property.
// SLB 19.mar.2019 CR.27575; added support for shortcut versions of string variables... referred to as 
// "Alternate" string variables.
//----------------------------------------------------------------------------------
class CPropertyIdMap
{
#define jaInvalid ((DBSpoolJobAttributeID)-1) 
public:
   typedef std::map <TEnginePropertyIds, CAttribPropertyIds> TIdMap; 
   typedef TIdMap::iterator TItIdMap; 
   typedef TIdMap::const_iterator TItcIdMap; 
   typedef std::pair<TItIdMap, bool> TPairResIdMap;                                 // CR.30115; modified.

   typedef std::map <CSpdCString, CAttribPropertyIds *> TPtrIdMap; 
   typedef TPtrIdMap::const_iterator TItcPtrIdMap; 

   typedef std::map <TTransactionPropertyId, CAttribPropertyIds *> TPtrPropertyIdMap; 
   typedef TPtrPropertyIdMap::const_iterator TItcPtrPropertyIdMap; 

public:
   static TIdMap s_AttribIdMap; 
   static TPtrIdMap s_TagIdMap; 
   static TPtrIdMap s_LegacyTagIdMap; 
   static TPtrIdMap s_StringVarKeyMap;                                              // CR.7968; added.
   static TPtrPropertyIdMap s_PropertyIdMap; 

public:
   void InitPropertyIdMap(); 
   void AddPropertyEntry(
      TEnginePropertyIds epiId,
      const wchar_t *pwcsXmlTag,
      const wchar_t *pwcsLegacyKey,
      TTransactionPropertyId tpi = (TTransactionPropertyId)-1,                      // CR.18552; added.
      DBSpoolJobAttributeID dbsJaId = jaInvalid,                                    // CR.12758;
      bool bIsReadOnlyForClient = false,
      const wchar_t *pwcsStringVarKey = NULL,                                       // CR.7698; added.
      const wchar_t *pwcsStringVarKey_Alt = NULL,                                   // CR.27575; added.
      const wchar_t *pwcsStringVarKey_Alt1 = NULL,                                  // CR.28299; added.
      const wchar_t *pwcsStringVarKey_Alt2 = NULL);                                 // CR.29830; added.
}; 

CPropertyIdMap::TIdMap CPropertyIdMap::s_AttribIdMap; 
CPropertyIdMap::TPtrIdMap CPropertyIdMap::s_TagIdMap; 
CPropertyIdMap::TPtrIdMap CPropertyIdMap::s_LegacyTagIdMap; 
CPropertyIdMap::TPtrIdMap CPropertyIdMap::s_StringVarKeyMap; 
CPropertyIdMap::TPtrPropertyIdMap CPropertyIdMap::s_PropertyIdMap; 

//
// Note: try to keep order properties are added the same as they are listed
// in the enumerated type.
// 
// SLB 11.jan.2011 CR.14108; changed dbspool property id for jobsize from jaFileSize to jaJobSize. DBSpool does not 
// allow file size to be modified by its clients.
// Also, changed dbspool property id from jaPageCount to jaTotalImpressions. jaPageCount is not displayed by dbspool.
// SLB 03.aug.2012 CR.16338; Updated so SPDE properties Misc1-4 have corresponding mappings into DBSpool properties 
// Category1-4. Note: no mapping exists for SPDE's Misc0 property.
// SLB 16.jul.2012 CR.15891.V02; Added entries epiMessageControlAttribsActiveReferences and 
// epiMessageControlAttribsHaltedReferences for new members which indicated references to an object, in 
// control messages. 
// SLB 14.mar.2014 CR.17478; Add entries for three new job property values.
// SLB 01.apr.2015 CR.18757; Added entries for new job property values. JobDataType and JobTicketType .
// SLB 21.aug.2015 CR.19149; Added entries for new job property members. IpdsPagesBuffered and IpdsPagesPrinted.
// DCM 11.nov.2015 CR.Fix.20594.v01: Added entries for new job property member m_dwCopyNum.
// DCM 11.nov.2015 CR.Fix.20338.v01: Added entries for new job property members m_dwStartPageRange and m_dwEndPageRange.
// SLB 04.dec.2015 CR.18552; Added tpi (Transaction Property Index) values for many properties to help with 
// translating job properties into transaction properties.
// SLB 11.dec.2015 CR.18552.V01; Fixed tpi mapping for JobId.
// SLB 13.may.2016 CR.21581; Added tpi mapping for multiple properties.
// SLB 29.jun.2016 CR.21678; Found that XchangeMsgString tag did not match what GUI was using. Fixed 
// to match GUI (XMS vs xms).
// Added new entries for various DBSpool properties.
// SLB 11.jul.2016 CR.21678.V02; Fixed up tag for epiDBSpoolIsEnabled. Needed to be compatible with older versions 
// of SPDE, SOLfusion, etc.
// SLB 17.feb.2017 CR.23073; Fix for not initializing Queue Entry Time with a TransactionPropertyId. This was causing havoc
// when sort order for a queue was set to Queue Entry Time.
// SLB 28.apr.2017 CR.22874; Gave QueueBiPassFilterId a legacy attribute so that it can be used to give a file
// a unique name instead of the jobid.
// SLB 03.dec.2018 CR.7698; Updated Job property entries with String Variable tag names, if they have one.
// SLB 19.mar.2019 CR.27575; added support for shortcut versions of string variables to all properties which
// had them in spde 8.3.
// SLB 28.may.2019 CR.27816; Fix for 27575 changes. Fixed Output Job Name string variable tag.
// SLB 17.sep.2019 CR.28299; Added support for yet another alternate string var keyname, needed for input job name.
// SLB 24.feb.2020 CR.28392; Found that QueueJobState tag ("qjs") did not align with what GUI was using... which was same, except for
// all caps ("QJS"). Changed to match GUI usage.
// SLB 15.oct.2020 CR.29680; Updated IpdsPagesPrinted job property to map to SOLitrack's jaCompletedImpressions property.
// SLB 05.nov.2020 CR.28730; Solitrack does not expose TotalMailpieces in the unmanaged code at this time... Use a #define in the meantime.
// Minimum Solitrack version required to support this property is 2.0.310.
// SLB 17.nov.2020 CR.28847; Added support for new ReadyPDF property: PdfoDocProperties.
// SLB 23.nov.2020 CR.28370.V01; Corrected the xml tag specified for sheetcount (from SC -> SHC).
// SLB 24.nov.2021 CR.22237; Added mapping for DBSpoolJobAttributes for the PageRangeTemplate property.
// SLB 22.jun.2022 CR.31582; Added new job property: DbsSourceActivityBy. Fixed up 
// tagnames for some DBSpool properties.
#define jaTotalMailpieces (DBSpoolJobAttributeID)144

void CPropertyIdMap::InitPropertyIdMap()
{  
   // CSpdJobProperties properties.
   AddPropertyEntry(epiJobDescription,       L"JD",   L"de",   tpiJobDescription, jaJobDescription, false, L"DES"); 
   AddPropertyEntry(epiOJobName,             L"OJN",  L"",     tpiOJobName, jaOJobName, true, L"OJO", L"O");                     // CR.27816; modified. CR.27575; modified.
   AddPropertyEntry(epiIJobName,             L"IJN",  L"jo",   tpiIJobName, jaIJobName, false, L"JOB", L"J", L"IJO", L"I");      // CR.29830; modified. CR.28299; modified. CR.27575; modified.
   AddPropertyEntry(epiIExtName,             L"EJN",  L"",     tpiIJobExt, jaIExtName, false, L"EXT", L"E");                     // CR.27575; modified.
   AddPropertyEntry(epiHostName,             L"HN",   L"ho",   tpiHostName, jaHostName, false, L"HOS", L"H");                    // CR.27575; modified.
   AddPropertyEntry(epiLocalHostName,        L"LHN",  L"lh",   tpiLHostName, jaLocalHostName, false, L"LHO", L"L");              // CR.27575; modified.

   AddPropertyEntry(epiDestinationName,      L"DN",   L"dn",   tpiDestinationName, jaSPDEDestinationName, false, L"DST", L"D");  // CR.27575; modified.
   AddPropertyEntry(epiJobAccountName,       L"AN",   L"ac",   tpiAccountName, jaJobAccountName, false, L"ACC", L"A");           // CR.27575; modified.
   AddPropertyEntry(epiUserName,             L"UN",   L"us",   tpiUserName, jaSPDEUserName, false, L"USE", L"U");                // CR.27575; modified.
   AddPropertyEntry(epiHostType,             L"HT",   L"ht",   tpiHostType, jaHostType, false, L"HTY"); 
   AddPropertyEntry(epiDestinationType,      L"DT",   L"dt",   tpiDestinationType, jaSPDEDestinationType, false, L"DTY"); 
   AddPropertyEntry(epiDestinationAddress,   L"DA",   L"da",   tpiDestinationAddress, jaSPDEDestinationAddress, false, L"DAD"); 
   AddPropertyEntry(epiDestinationPort,      L"DP",   L"dp",   tpiDestinationPort, jaDestinationPort, false, L"DPO"); 
   AddPropertyEntry(epiDestinationQueue,     L"DQ",   L"dq",   tpiDestinationQueue, jaSPDEDestinationQueue, false, L"DQU"); 
   AddPropertyEntry(epiJobType,              L"JT",   L"jt",   tpiJobType, jaJobType, false, L"JTY"); 
   AddPropertyEntry(epiFormName,             L"FN",   L"fo",   tpiFormName, jaFormName, false, L"FOR", L"F");                    // CR.27575; modified.
   AddPropertyEntry(epiXeroxJdlName,         L"JDL",  L"jd",   tpiXeroxJDL, jaXeroxJdlName, false, L"JDL"); 
   AddPropertyEntry(epiXeroxJdeName,         L"JDE",  L"je",   tpiXeroxJDE, jaXeroxJdeName, false, L"JDE"); 
   AddPropertyEntry(epiXchangeString,        L"XMS",  L"xs",   tpiXchangeMiscString, jaXchangeString, false, L"XCH", L"X");      // CR.27575; modified. CR.21678; modified.
   AddPropertyEntry(epiXchangeOpMsgString,   L"XOM",  L"om",   tpiXchangeOpMsgString, jaXchangeOpMsgString, false, L"XOP");
   AddPropertyEntry(epiJobSize,              L"JS",   L"si",   tpiJobSize, jaJobSize, false, L"JSZ");                            // CR.14108; modified.
   AddPropertyEntry(epiBytesProcessed,       L"BP",   L"",     tpiPInputBytes); 
   AddPropertyEntry(epiPageCount,            L"PC",   L"pa",   tpiPageCount, jaTotalImpressions, false, L"PAG", L"P");           // CR.27575; modified. CR.14108; modified.
   AddPropertyEntry(epiJobCopies,            L"CC",   L"co",   tpiCopyCount, jaJobCopies, false, L"COP"); 
   AddPropertyEntry(epiVolumeIO,             L"VO",   L"vo",   tpiVolio); 
   AddPropertyEntry(epiJobId,                L"JID",  L"ji",   tpiJobId, jaJobIDSPDE, true, L"JID");                             // CR.18552.V01; modified.
   AddPropertyEntry(epiFcbName,              L"FCB",  L"fc",   tpiFcbName, jaFcbName, false, L"FCB"); 
   AddPropertyEntry(epiJobClassName,         L"CLN",  L"cl",   tpiClassName, jaJobClassName, false, L"CLA", L"C");               // CR.27575; modified.
   AddPropertyEntry(epiInkColor,             L"IC",   L"ic",   tpiInkColor, jaInkColor, false, L"INK"); 
   AddPropertyEntry(epiAfpFormdef,           L"AF",   L"af",   tpiAfpFormdef, jaAfpFormDef, false, L"AFD"); 
   AddPropertyEntry(epiAfpPagedef,           L"AP",   L"ap",   tpiAfpPagedef, jaAfpPageDef, false, L"APD"); 
   AddPropertyEntry(epiReportStartPageNum,   L"RSP",  L"",     tpiReportStartPageNum, jaInvalid, false, L"RSP", L"R");           // CR.27575; modified.
   AddPropertyEntry(epiLpdQueueName,         L"LQU",  L"",     tpiLpdQueueName, jaSPDELpdQueueName, false, L"LQU"); 
   AddPropertyEntry(epiSpdeActivityUserId,   L"AI",   L"",     tpiSpdeActivityUserId, jaJobReleasedBy, true);                    // CR.9195.V01
   AddPropertyEntry(epiFolder1,              L"F1",   L"",     tpiFolder1, jaInvalid, false, L"FD1");                            // CR.11127
   AddPropertyEntry(epiFolder2,              L"F2",   L"",     tpiFolder2, jaInvalid, false, L"FD2");                            // CR.11127
   AddPropertyEntry(epiFolder3,              L"F3",   L"",     tpiFolder3, jaInvalid, false, L"FD3");                            // CR.11127
   AddPropertyEntry(epiMisc0,                L"M0",   L"");                                                                      // CR.9419
   AddPropertyEntry(epiMisc1,                L"M1",   L"",  tpiMisc1, jaCategory1, false, L"MS1", L"1");                         // CR.27575; modified. CR.16338; modified. CR.9419; added.
   AddPropertyEntry(epiMisc2,                L"M2",   L"",  tpiMisc2, jaCategory2, false, L"MS2");                               // CR.16338; modified. CR.9419; added.
   AddPropertyEntry(epiMisc3,                L"M3",   L"",  tpiMisc3, jaCategory3, false, L"MS3");                               // CR.16338; modified. CR.9419; added.
   AddPropertyEntry(epiMisc4,                L"M4",   L"",  tpiMisc4, jaCategory4, false, L"MS4");                               // CR.16338; modified. CR.9419; added.
   AddPropertyEntry(epiEncryptMode,          L"EM",   L"",  tpiEncryptMode, jaInvalid, true);                                    // CR.17478; added.
   AddPropertyEntry(epiMiscInt1,             L"MI1",  L"",  tpiMiscInt1, jaInvalid, false, L"MI1");                              // CR.17478; added.
   AddPropertyEntry(epiMiscInt2,             L"MI2",  L"",  tpiMiscInt2, jaInvalid, false, L"MI2");                              // CR.17478; added.
   AddPropertyEntry(epiJobDataType,          L"JDT",  L"",  tpiJobDataType, jaInvalid, true);                                    // CR.18757; added.
   AddPropertyEntry(epiJobTicketType,        L"JTT",  L"",  tpiJobTicketType, jaInvalid, true);                                  // CR.18757; added.
   AddPropertyEntry(epiIpdsPagesBuffered,    L"IPB",  L"",  tpiIpdsPagesBuffered, jaInvalid, true);                              // CR.19149; added.
   AddPropertyEntry(epiIpdsPagesPrinted,     L"IPP",  L"",  tpiIpdsPagesPrinted, jaCompletedImpressions, true);                  // CR.29680; modified. CR.19149; added.
   AddPropertyEntry(epiPdfoDocProperties,    L"PODP", L"",  tpiPdfoDocProperties, jaInvalid, false, L"PDP");                     // CR.28847; added.
   AddPropertyEntry(epiCopyNum,              L"CNU",  L"",  tpiPCurrentCopyNum, jaInvalid, true, L"CNU");                        // CR.Fix.20594.v01
   AddPropertyEntry(epiStartPageRange,       L"SPR",  L"",  tpiStartPageRange, jaInvalid, false, L"SPR");                        // CR.Fix.20338.v01
   AddPropertyEntry(epiEndPageRange,         L"EPR",  L"",  tpiEndPageRange, jaInvalid, false, L"EPR");                          // CR.Fix.20338.v01
   AddPropertyEntry(epiPageRangeTemplate,    L"PRT",  L"",  tpiPageRangeTemplate, jaPageRangeTemplate);                          // CR.22237; modified. CR.16892; added.
   AddPropertyEntry(epiDBSpoolIsEnabled,     L"DBSE", L"",  tpiDbsIsEnabled, jaInvalid, true);                                   // CR.21678.V02; modified. CR.13557;
   AddPropertyEntry(epiDBSpoolIsRequired,    L"DBSR", L"");                                                                      // CR.13557;
   AddPropertyEntry(epiSheetCount,           L"SHC",  L"",  tpiSheetCount, jaTotalSheets, false, L"SHC");                        // CR.28730.V01; modified. CR.28730; added.
   AddPropertyEntry(epiMailPieceCount,       L"MPC",  L"",  tpiMailpieceCount, jaTotalMailpieces, false, L"MPC");                // CR.28730; added.

   // DBSpool specific properties.
   AddPropertyEntry(epiDBSpoolJobId,         L"dbsjid",  L"",  tpiDbsJobId, jaJobID, true, L"SID", L"S");                        // CR.27575; modified. CR.9419
   AddPropertyEntry(epiDBSpoolLastTag,       L"dbstv",   L"",  tpiDbsLastTag, jaJobTags);                                        // CR.13557;
   AddPropertyEntry(epiDbsStatusMsg,         L"dbssm",   L"",  tpiDbsStatusMsg, jaJobStatusMessage); 
   AddPropertyEntry(epiDbsDisplayJobName,    L"dbsdjn",  L"",  tpiDbsDisplayJobName, jaDisplayJobName); 
   AddPropertyEntry(epiDbsComputerName,      L"dbscn",   L"",  tpiDbsComputerName, jaApplicationHost); 
   AddPropertyEntry(epiDbsApplicationName,   L"dbsan",   L"",  tpiDbsApplicationName, jaApplicationName); 
   AddPropertyEntry(epiDbsHasSourceFile,     L"dbshsf",  L"",  tpiDbsHasSourceFile, jaHasFile); 
   AddPropertyEntry(epiDbsDeletedTime,       L"dbssdt",  L"",  tpiDbsSourceDeletedTime, jaDeleted);                              // CR.31582; modified.
   AddPropertyEntry(epiDbsMovedTime,         L"dbssmt",  L"",  tpiDbsSourceMovedTime, jaMoved);                                  // CR.31582; modified.
   AddPropertyEntry(epiDbsStatus,            L"dbss",    L"",  tpiDbsState, jaJobStateID); 
   AddPropertyEntry(epiDbsJobStartTime,      L"dbsjst",  L"",  tpiDbsJobStartTime, jaProcessingStarted); 
   AddPropertyEntry(epiDbsJobEndTime,        L"dbsjet",  L"",  tpiDbsJobEndTime, jaProcessingEnded); 
   AddPropertyEntry(epiDbsCheckpointId,      L"dbscpid", L"",  tpiDbsCheckpointId, jaCheckpointID); 
   AddPropertyEntry(epiDbsSourceActivityBy,  L"dbssab",  L"",  tpiDbsSourceActivityBy, jaJobReleasedBy);                         // CR.31582; added.

   AddPropertyEntry(epiMediumInfoListId,                 L"LID",  L"",  tpiJobMediumInfoList);                                   // CR.14962; added.
   AddPropertyEntry(epiMediumInfoObjectId,               L"OID",  L"");                                                          // CR.14962; added.
   AddPropertyEntry(epiMediumInfoId,                     L"ID",   L"",  tpiMIId);                                                // CR.14962; added.
   AddPropertyEntry(epiMediumInfoSheetsPrinted,          L"SP",   L"",  tpiMISheetsPrintedTotal);                                // CR.14962; added.
   AddPropertyEntry(epiMediumInfoSheetsPrintedSimplex,   L"SPS",  L"",  tpiMISheetsPrintedSimplex);                              // CR.14962; added.
   AddPropertyEntry(epiMediumInfoSheetsPrintedDuplex,    L"SPD",  L"",  tpiMISheetsPrintedDuplex);                               // CR.14962; added.
   AddPropertyEntry(epiMediumInfoName,                   L"N",    L"",  tpiMIMediumName);                                        // CR.14962; added.
   AddPropertyEntry(epiMediumInfoType,                   L"T",    L"",  tpiMIMediumType);                                        // CR.14962; added.
   AddPropertyEntry(epiMediumInfoSize,                   L"S",    L"",  tpiMIMediumSize);                                        // CR.14962; added.
   AddPropertyEntry(epiMediumInfoColor,                  L"C",    L"",  tpiMIMediumColor);                                       // CR.14962; added.
   AddPropertyEntry(epiMediumInfoWeight,                 L"W",    L"",  tpiMIMediumWeight);                                      // CR.14962; added.
   
   // CJobProperties property Ids
   AddPropertyEntry(epiJobPropertiesJobId, L"jid", L""); 
   AddPropertyEntry(epiJobPropertiesQueueJobPosition, L"pos", L""); 
   AddPropertyEntry(epiJobPropertiesJobState, L"QJS", L"", tpiQueueJobState);                                                    // CR.28392; modified. CR.21581; modified.
   AddPropertyEntry(epiJobPropertiesJobActivityState, L"jas", L""); 
   AddPropertyEntry(epiJobPropertiesJobPropertyList, L"jpl", L""); 

   // CSpdMediumJob property Ids
   AddPropertyEntry(epiMediumJobFileCreationTime, L"FCT", L"cr", tpiFileCreateTime, jaInvalid, true);                            // CR.21581; modified.
   AddPropertyEntry(epiMediumJobFileLastAccessTime, L"FLAT", L"", tpiFileLastAccessTime, jaInvalid, true);                       // CR.21581; modified.
   AddPropertyEntry(epiMediumJobFileLastWriteTime, L"FLWT", L"", tpiFileLastWriteTime, jaInvalid, true);                         // CR.21581; modified.
   AddPropertyEntry(epiMediumJobBiPassFilterId, L"BPFQ", L"bp", tpiBipassFilterId);                                              // CR.22874; modified. CR.21581; modified.

   // CSpdQueueJob property Ids
   AddPropertyEntry(epiQueueJobQueueJobState, L"QJS", L"en", (TTransactionPropertyId)-1, jaInvalid, true);                       // CR.Fix.20594.v01); 
   AddPropertyEntry(epiQueueJobQueueEntryTime, L"QET", L"", tpiQueueEntryTime, jaInvalid, true);                                 // CR.23073; modified.
   AddPropertyEntry(epiQueueJobStartPoint, L"QSP", L"", tpiStartPoint);                                                          // CR.21581; modified.
   AddPropertyEntry(epiQueueJobEndPoint, L"QEP", L"", tpiEndPoint);                                                              // CR.21581; modified.
   AddPropertyEntry(epiQueueJobForcePastFiltering, L"QFPF", L"", (TTransactionPropertyId)-1, jaInvalid, true); 
   AddPropertyEntry(epiQueueJobQueuePositionIsLocked, L"QLOC", L""); 

   AddPropertyEntry(epiSpdeActivityId, L"tid", L"", tpiSpdeActivityId, jaInvalid, false, L"AID");                                // CR.21572; added.

   // CSpdDispatchJobPropertyIF access members.
   AddPropertyEntry(epiDJobPropertyIfDJobId,                L"djid",    L"",   tpiDJobId, jaInvalid, true);       
   AddPropertyEntry(epiDJobPropertyIfDJobSubmitId,          L"djsid",   L"",   tpiDJobSubmitId, jaInvalid, true);                // CR.26999.Item102; added.
   AddPropertyEntry(epiDJobPropertyIfDJobDisplayName,       L"djdn",    L"",   tpiDJobDisplayName, jaInvalid, true);       
   AddPropertyEntry(epiDJobPropertyIfDTPathName,            L"djtpn",   L"",   tpiDTPathName, jaInvalid, true);       
   AddPropertyEntry(epiDJobPropertyIfDTPathId,              L"djtpid",  L"",   tpiDTPathId, jaInvalid, true);       
   AddPropertyEntry(epiDJobPropertyIfDJobInputFileName,     L"djifn",   L"",   tpiDJobInputFileName, jaInvalid, true);       
   AddPropertyEntry(epiDJobPropertyIfDJobInputFileSize,     L"djifs",   L"",   tpiDJobInputFileSize, jaInvalid, true);       
   AddPropertyEntry(epiDJobPropertyIfDJobOutputFileName,    L"djofn",   L"",   tpiDJobOutputFileName, jaInvalid, true);       
   AddPropertyEntry(epiDJobPropertyIfDPathId,               L"djpid",   L"",   tpiDPathId, jaInvalid, true);       
   AddPropertyEntry(epiDJobPropertyIfDJobState,             L"djs",     L"",   tpiDJobState, jaInvalid, true);       
   AddPropertyEntry(epiDJobPropertyIfDSubmitByMachine,      L"djsbm",   L"",   tpiDSubmitByMachine, jaInvalid, true);       
   AddPropertyEntry(epiDJobPropertyIfDSubmitByUser,         L"djsbu",   L"",   tpiDSubmitByUser, jaInvalid, true);       
   AddPropertyEntry(epiDJobPropertyIfDProcessByMachine,     L"djpbm",   L"",   tpiDProcessByMachine, jaInvalid, true);       
   AddPropertyEntry(epiDJobPropertyIfDCancelByMachine,      L"djcbm",   L"",   tpiDCancelByMachine, jaInvalid, true);       
   AddPropertyEntry(epiDJobPropertyIfDCancelByUser,         L"djcbu",   L"",   tpiDCancelByUser, jaInvalid, true);       
   AddPropertyEntry(epiDJobPropertyIfDApaCreateTime,        L"djct",    L"",   tpiDApaCreateTime, jaInvalid, true);       
   AddPropertyEntry(epiDJobPropertyIfDApaLockTime,          L"djlt",    L"",   tpiDApaLockTime, jaInvalid, true);       
   AddPropertyEntry(epiDJobPropertyIfDApaSubmitTime,        L"djst",    L"",   tpiDApaSubmitTime, jaInvalid, true);       
   AddPropertyEntry(epiDJobPropertyIfDApaPostTime,          L"djpt",    L"",   tpiDApaPostTime, jaInvalid, true);       
   AddPropertyEntry(epiDJobPropertyIfDApaActiveTime,        L"djat",    L"",   tpiDApaActiveTime, jaInvalid, true);       
   AddPropertyEntry(epiDJobPropertyIfDApaPostprocessTime,   L"djppt",   L"",   tpiDApaPostprocessTime, jaInvalid, true);       
   AddPropertyEntry(epiDJobPropertyIfDApaCompleteTime,      L"djct",    L"",   tpiDApaCompleteTime, jaInvalid, true);       
   AddPropertyEntry(epiDJobPropertyIfDApaCompletionCode,    L"djcc",    L"",   tpiDApaCompletionCode, jaInvalid, true);       
   AddPropertyEntry(epiDJobPropertyIfDApaInterruptCode,     L"djic",    L"",   tpiDApaInterruptCode, jaInvalid, true);       
   AddPropertyEntry(epiDJobPropertyIfDApaInterruptState,    L"djis",    L"",   tpiDApaInterruptState, jaInvalid, true);       
}

/* AddPropertyEntry()
 *
 * SLB 24.apr.2017 CR.22874; Minor optimization to use emplace() vs. insert() in std collections.
 * SLB 19.mar.2019 CR.27575; Added support for an alternative string variable key (several 
 * string variables can key off of a single char).
 ---------------------------------------------------------------------------------------*/
void CPropertyIdMap::AddPropertyEntry(
   TEnginePropertyIds epiId, 
   const wchar_t *pwcsXmlTag, 
   const wchar_t *pwcsLegacyKey, 
   TTransactionPropertyId tpi,                                                      // CR.18552; added.
   DBSpoolJobAttributeID dbsJaId,                                                   // CR.12758;
   bool bIsReadOnlyForClient,
   const wchar_t *pwcsStringVarKey,                                                 // CR.7698; added.
   const wchar_t *pwcsStringVarKey_Alt,                                             // CR.27575; added.
   const wchar_t *pwcsStringVarKey_Alt1,                                            // CR.28299; added.
   const wchar_t *pwcsStringVarKey_Alt2)                                            // CR.29830; added
{
   // If insert fails, throw assert. Duplicate id needs to be resolved by developer.
   TPairResIdMap res; 
   res = s_AttribIdMap.emplace(
      epiId, 
      CAttribPropertyIds(
         pwcsXmlTag, 
         pwcsLegacyKey, 
         pwcsStringVarKey,                                                          // CR.7968; added.
         pwcsStringVarKey_Alt,                                                      // CR.27575; added.
         pwcsStringVarKey_Alt1,                                                     // CR.28299; added.
         pwcsStringVarKey_Alt2,                                                     // CR.29830; added.
         dbsJaId,                                                                   // CR.12758;
         tpi,                                                                       // CR.18552; added.
         epiId,
         bIsReadOnlyForClient)); 
   if (!res.second) sol_assert(false); 

   // Add an entry to the Legacy Tag Id map and
   // the PropertyId map as well.
   s_TagIdMap.emplace(CSpdCString(pwcsXmlTag), &(res.first->second));               // CR.22874; modified.
   s_LegacyTagIdMap.emplace(CSpdCString(pwcsLegacyKey), &(res.first->second));      // CR.22874; modified.
   s_PropertyIdMap.emplace(tpi, &(res.first->second));                              // CR.22874; modified.
   if (pwcsStringVarKey)                                                            // CR.27575; added if().
   {
      // Add string variable map entries. Note: use string variable from the CAttribPropertyIds object, 
      // not the value passed in, because the CAttribPropertyIds constructor may fix-up the string as needed.
      s_StringVarKeyMap.emplace(res.first->second.m_wsStringVarKey, &(res.first->second)); // CR.28299; modified. CR.7968; added.

      // If there is an alternate string variable key, add that entry as well.
      if (pwcsStringVarKey_Alt)                                                     // CR.27575; added.
         s_StringVarKeyMap.emplace(res.first->second.m_wsStringVarKey_Alt, &(res.first->second)); // CR.28299; modified. CR.27575; added.
      if (pwcsStringVarKey_Alt1)                                                    // CR.28299; added.
         s_StringVarKeyMap.emplace(res.first->second.m_wsStringVarKey_Alt1, &(res.first->second)); // CR.28299; added.
      if (pwcsStringVarKey_Alt2)                                                    // CR.29830; added.
         s_StringVarKeyMap.emplace(res.first->second.m_wsStringVarKey_Alt2, &(res.first->second)); // CR.29830; added.
   }
}


class InitAfterStatics
{
public:
   InitAfterStatics()
   {
      CPropertyIdMap pim; 
      pim.InitPropertyIdMap(); 
   }
} Start; 

/*
 *
 * CAttributes static methods.
 *
 * Needs:
 * 1) Methods for retrieving all properties via TEnginePropertyId
 * 2) 
 * 1) Methods for all job property Ids from Transaction Id
 * method for 
 */

/* GetTransactionPropertyId() - static
---------------------------------------------------------------------------------------*/
TTransactionPropertyId CAttributes::GetTransactionPropertyId(
   const CSpdCString &wsAttribTag)
{
   CPropertyIdMap::TItcPtrIdMap it(CPropertyIdMap::s_TagIdMap.find(wsAttribTag)); 
   if (it == CPropertyIdMap::s_TagIdMap.end()) return ((TTransactionPropertyId)-1); 
   
   return it->second->m_tpiPropertyId; 
}
/* GetTransactionPropertyId() - static
---------------------------------------------------------------------------------------*/
TTransactionPropertyId CAttributes::GetTransactionPropertyId(
   TEnginePropertyIds epId) 
{
   CPropertyIdMap::TItcIdMap it(CPropertyIdMap::s_AttribIdMap.find(epId)); 
   if (it == CPropertyIdMap::s_AttribIdMap.end()) return ((TTransactionPropertyId)-1); 
   
   return it->second.m_tpiPropertyId; 
}

/* GetTransactionPropertyId() - static
 * SLB 04.dec.2018 CR.7968; created.
---------------------------------------------------------------------------------------*/
TTransactionPropertyId SpdAttribs4::CAttributes::GetTransactionPropertyId(
   const CSpdCString & wsStringVarTag, 
   bool) 
{
   CPropertyIdMap::TItcPtrIdMap it(CPropertyIdMap::s_StringVarKeyMap.find(wsStringVarTag)); 
   if (it == CPropertyIdMap::s_StringVarKeyMap.end()) return ((TTransactionPropertyId)-1); 
   
   return it->second->m_tpiPropertyId; 
}

/* GetTransactionPropertyId() - static over-load
 * SLB 11.nov.2020 CR.28730; created.
---------------------------------------------------------------------------------------*/
TTransactionPropertyId SpdAttribs4::CAttributes::GetTransactionPropertyId(
   DBSpoolJobAttributeID dbsjaId)
{
   for (CPropertyIdMap::TItcIdMap it(CPropertyIdMap::s_AttribIdMap.begin()); it != CPropertyIdMap::s_AttribIdMap.end(); it++)
   { 
      if (it->second.m_jaDbsJaId == dbsjaId) return it->second.m_tpiPropertyId;
   }
   return tpiInvalid;
}

/* GetLegacyKey() - static
---------------------------------------------------------------------------------------*/
CSpdCString CAttributes::GetLegacyKey(
   const CSpdCString &wsAttribTag) 
{
   CPropertyIdMap::TItcPtrIdMap it(CPropertyIdMap::s_TagIdMap.find(wsAttribTag)); 
   if (it == CPropertyIdMap::s_TagIdMap.end()) return L""; 
   
   return it->second->m_wsLegacyKeyName; 
}
/* GetDbsId() - static
---------------------------------------------------------------------------------------*/
int CAttributes::GetDbsId(
   const CSpdCString &wsAttribTag) 
{
   CPropertyIdMap::TItcPtrIdMap it(CPropertyIdMap::s_TagIdMap.find(wsAttribTag)); 
   if (it == CPropertyIdMap::s_TagIdMap.end()) return -1; 
   
   return it->second->m_jaDbsJaId; 
}
/* GetTagKeyFromLegacyKey() - static
---------------------------------------------------------------------------------------*/
CSpdCString CAttributes::GetTagKeyFromLegacyKey(
   const CSpdCString &wsLegacyTag) 
{
   CPropertyIdMap::TItcPtrIdMap it(CPropertyIdMap::s_LegacyTagIdMap.find(wsLegacyTag)); 
   if (it == CPropertyIdMap::s_LegacyTagIdMap.end()) return L"NoKey"; 
   
   return it->second->m_wsKeyName; 
}

/* GetTagKeyFromPropertyId() - static
---------------------------------------------------------------------------------------*/
CSpdCString CAttributes::GetTagKeyFromPropertyId(
   TTransactionPropertyId tpi) 
{
   CPropertyIdMap::TItcPtrPropertyIdMap it(CPropertyIdMap::s_PropertyIdMap.find(tpi)); 
   if (it == CPropertyIdMap::s_PropertyIdMap.end()) return L"NoKey"; 
   
   return it->second->m_wsKeyName; 
}

/* GetTagKeyFromPropertyId() - static
---------------------------------------------------------------------------------------*/
CSpdCString CAttributes::GetTagKeyFromPropertyId(
   TEnginePropertyIds epi) 
{
   CPropertyIdMap::TItcIdMap it(CPropertyIdMap::s_AttribIdMap.find(epi)); 
   if (it == CPropertyIdMap::s_AttribIdMap.end()) return L"NoKey"; 
   
   return it->second.m_wsKeyName; 
}

/* IsPropertyReadOnlyForClients() - static
---------------------------------------------------------------------------------------*/
bool CAttributes::IsPropertyReadOnlyForClients(
   TTransactionPropertyId tpi) 
{
   CPropertyIdMap::TItcPtrPropertyIdMap it(CPropertyIdMap::s_PropertyIdMap.find(tpi)); 
   if (it == CPropertyIdMap::s_PropertyIdMap.end()) return true; 
   
   return it->second->m_bIsReadOnlyForClient; 
}

/* IsPropertyReadOnlyForClients() - static
---------------------------------------------------------------------------------------*/
bool CAttributes::IsPropertyReadOnlyForClients(
   const CSpdCString &wsAttribTag) 
{
   CPropertyIdMap::TItcPtrIdMap it(CPropertyIdMap::s_TagIdMap.find(wsAttribTag)); 
   if (it == CPropertyIdMap::s_TagIdMap.end()) return true; 

   return it->second->m_bIsReadOnlyForClient; 
}

/* CalculateRedundancyCode() - static
 * 
 * SLB 12.jun.2019 CR.27900.V02; Created. Current algorithm is: 
 *   Hi 32 bit = string length.
 *   Lo 32 bit = running count of wchar_t values.
 * This presumes that the running count will not exceed the max for an unsigned 32 bit value.
---------------------------------------------------------------------------------------*/
QUADWORD CAttributes::CalculateRedundancyCode(
   const CSpdCString & wsStream)
{
   // Put string length in Hi 32 bit portion.
   QUADWORD qwCount((QUADWORD)wsStream.wcslen() << 32);

   // Calculate running value count in lower 32 bits.
   for (DWORD i = 0; i < wsStream.wcslen(); i++) qwCount += wsStream[i];

   return qwCount;
}

#endif

namespace SpdAttribs4
{

/* AllocateAndStreamProperty() - global method.
 *
 * SLB 06.feb.2008 - Modified to handle memory allocation error.
 * SLB 30.apr.2009 CR.11217; Performance enhancement. Will now set a character count
 * variable if it is not NULL.
 * SLB 23.apr.2015 CR.18893.V03; Tracking down Job Property serializing issue. Re-structured 
 * and fixed up to be more robust.
---------------------------------------------------------------------------*/
wchar_t * AllocateAndStreamProperty(
   const CStreamableIf &Property, 
   size_t *pstCharCount,
   size_t stSuggestedBufSize,
   bool bUseTag,
   bool bUseContainerTag)
{
   if (!stSuggestedBufSize)  { sol_assert(false); return NULL; }
   
   size_t stBufSize(stSuggestedBufSize); 
   wchar_t *pwcsPropertyBuf(NULL); 
   size_t stOutLen(0); 
   CClockTime ctBegin(CClockTime::Clock()); 

   while (true)
   {
      if (stBufSize > 0x10000000) throw (hr_exception(E_OUTOFMEMORY, L"Serialized object memory requirements exceeds max allowed.", __WFUNCTION__)); 
      pwcsPropertyBuf = ReallocateWcsBuffer(pwcsPropertyBuf, stBufSize, stBufSize, true); 
      if (pwcsPropertyBuf == NULL) { sol_assert(false); return NULL; }
      
      try { stOutLen = Property.Serialize(pwcsPropertyBuf, stBufSize, bUseTag, bUseContainerTag); }
      catch (hr_exception &e) 
      { 
         wchar_t wcsMsg[256]; 
         _snwprintf_s(wcsMsg, _TRUNCATE, L"%s() Buffersize(0x%08zX) Exception caught: hr: %08X msg: %s fnc: %s()", // CR.24131; modified.
            __WFUNCTION__,
            stBufSize,
            e.GetHr(),
            e.GetMessage().c_str(),
            e.GetMethod().c_str()); 
         CEnterExitMsg::TraceMsg(wcsMsg); 
         continue;  
      }
      break; 
   }

   if (!stOutLen && pwcsPropertyBuf)
   {
      delete [](pwcsPropertyBuf); 
      pwcsPropertyBuf = NULL; 
      throw (hr_exception(E_OUTOFMEMORY, L"Attribute class is too large to serialize.", __WFUNCTION__)); 
   } 

   if (pstCharCount != NULL)
   {
      *pstCharCount = stOutLen;                                                     // CR.11217
   }
   return pwcsPropertyBuf; 
}

/*
 * class CAttributes
 *
 *
 */

//
// XML Serializing/Deserializing methods.
//

/* Serialize()
 *
 * SLB 28.mar.2016 CR.18552.V04; Added this comment header.
---------------------------------------------------------------------------------------------------------*/
size_t CAttributes::Serialize(wchar_t *pwcsBuf, size_t stBufCharLen, bool bUseTag, bool bUseContainerTag, bool bTransparentOn) const 
{
   CSpdLock block(m_cs);                                                       // CR.21630; added.
   if (!block.Locked()) throw (hr_exception(E_FAIL, L"Another thread is accessing CAttributes object.", __WFUNCTION__)); 

   size_t stWriteOff(0); 
   stWriteOff += SerializeBegin(pwcsBuf+stWriteOff, stBufCharLen-stWriteOff, bUseTag, bUseContainerTag); 

   // Now add in property entries.
   for (TItcPropertyList it(m_Properties.begin()); it != m_Properties.end(); it++)
   {
      int nWriteLen(0); 
      nWriteLen = _snwprintf_s(
         pwcsBuf+stWriteOff, stBufCharLen-stWriteOff, _TRUNCATE,
         L"<%s>%s</%s>",
         it->first.c_str(),
         it->second.c_str(),
         it->first.c_str()); 
      if (nWriteLen<0) throw (hr_exception(E_FAIL, L"Insufficient buffer size for simple primitive property Node.", __WFUNCTION__)); 
      stWriteOff += nWriteLen; 
   }
   stWriteOff += SerializeEnd(pwcsBuf+stWriteOff, stBufCharLen-stWriteOff, bUseTag, bUseContainerTag); 
   return stWriteOff; 
}

/* SerializeBegin()
 *
 * SLB 28.mar.2016 CR.18552.V04; Fix for Serializing classes which use the same tag for both the tagname and the 
 * container name.
---------------------------------------------------------------------------------------------------------*/
size_t CAttributes::SerializeBegin(wchar_t *pwcsBuf, size_t stBufCharLen, bool bUseTag, bool bUseContainerTag) const 
{
   if (!bUseTag && !bUseContainerTag) return 0; 

   if (m_bInSlimMode) bUseContainerTag = false; 
   int nWriteLen(0); 
   size_t stWriteOff(0); 
   if (bUseTag && GetTagName().size())
   {
      nWriteLen = _snwprintf_s(
         pwcsBuf + stWriteOff, stBufCharLen - stWriteOff, _TRUNCATE,
         L"<%s>",
         GetTagName().c_str()); 
      if (nWriteLen < 0) throw (hr_exception(E_FAIL, L"Insufficient buffer size for begin key", __WFUNCTION__)); 
      stWriteOff += nWriteLen; 
   }
   if (bUseContainerTag && GetContainerTag().size())
   {
      nWriteLen = _snwprintf_s(pwcsBuf+stWriteOff, stBufCharLen-stWriteOff, _TRUNCATE,
         L"<%s>",
         GetContainerTag().c_str()); 
      if (nWriteLen < 0) throw (hr_exception(E_FAIL, L"Insufficient buffer size for begin container key", __WFUNCTION__)); 
      stWriteOff += nWriteLen; 
   }
   return stWriteOff; 
}

/* SerializeEnd()
 *
 * SLB 28.mar.2016 CR.18552.V04; Fix for Serializing classes which use the same tag for both the tagname and the 
 * container name.
---------------------------------------------------------------------------------------------------------*/
size_t CAttributes::SerializeEnd(wchar_t *pwcsBuf, size_t stBufCharLen, bool bUseTag, bool bUseContainerTag) const 
{
   if (!bUseTag && !bUseContainerTag) return 0; 

   if (m_bInSlimMode) bUseContainerTag = false; 
   int nWriteLen(0); 
   size_t stWriteOff(0); 

   // Add closing tags
   if (bUseContainerTag && GetContainerTag().size()) 
   {
      nWriteLen = _snwprintf_s(pwcsBuf+stWriteOff, stBufCharLen-stWriteOff, _TRUNCATE,
         L"</%s>",
         GetContainerTag().c_str()); 
      if (nWriteLen < 0) throw (hr_exception(E_FAIL, L"Insufficient buffer size for terminating container key", __WFUNCTION__)); 
      stWriteOff += nWriteLen; 
   }
   if (bUseTag && GetTagName().size())
   {
      nWriteLen = _snwprintf_s(pwcsBuf+stWriteOff, stBufCharLen-stWriteOff, _TRUNCATE,
         L"</%s>",
         GetTagName().c_str()); 
      if (nWriteLen < 0) throw (hr_exception(E_FAIL, L"Insufficient buffer size for terminating key", __WFUNCTION__)); 
      stWriteOff += nWriteLen; 
   }
   return stWriteOff; 
}


/* Serialize()
 *
 * SLB 28.mar.2016 CR.18552.V04; Added this comment header.
---------------------------------------------------------------------------------------------------------*/
CSpdCString CAttributes::Serialize(bool bUseTag, bool bUseContainerTag) const
{
   size_t stBufLen(16384); 
   wchar_t *pwcs(NULL); 
   while (true)
   {
      pwcs = ReallocateWcsBuffer(pwcs, stBufLen, stBufLen, true); 
      try { Serialize(pwcs, stBufLen, bUseTag, bUseContainerTag);  }
      catch (...) { continue;  }
      break; 
   }
   CSpdCString scs(pwcs); 
   delete[] (pwcs); 
   return scs; 
}


/* DeserializeFromFile() - This version reads the data from a file.
 *    Note: Cannot be used if filesize is 1MB or greater.
 *
 * SLB 06.jul.2017 CR.17862.V05; created/added.
---------------------------------------------------------------------------------------------------------*/
void CAttributes::DeserializeFromFile(const CNtfsFolderName &nfnXmlFile) 
{
   QUADWORD qwFileSize(nfnXmlFile.GetFileSize()); 
   if (qwFileSize > 1024*1024) throw (hr_exception(E_FAIL, L"File size is too large to stream directly into buffer", __WFUNCTION__)); 
   if (!qwFileSize) throw (hr_exception(E_FAIL, L"Empty file cannot contain valid xml", __WFUNCTION__)); 
   
   if (qwFileSize%1) qwFileSize -=1; 
   wchar_t *pwcsXmlBuf = new wchar_t[(size_t)qwFileSize/2 + 1]; 

   // Open the file. Uses a smart handle so that CloseHandle() gets called automatically when 
   // this method is exited (either normal exit or via exception).
   CSmartHandle shFile;  
   BOOL bRes(FALSE); 
   shFile = CreateFile(
      nfnXmlFile.GetFullFileName().c_str(),
      GENERIC_READ,
      FILE_SHARE_READ,
      NULL,
      OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL,
      NULL); 
   DWORD dwNtErr(GetLastError()); 
   if (shFile == INVALID_HANDLE_VALUE) throw (hr_exception(HRESULT_FROM_WIN32(dwNtErr), L"Failed to open file.", __WFUNCTION__)); 

   // successfully opened the file.  Read the xml from it.
   DWORD dwReadLength(0); 
   BOOL bOk(FALSE); 
   bOk = ReadFile(shFile, pwcsXmlBuf, (DWORD)qwFileSize, &dwReadLength, NULL); 
   dwNtErr = GetLastError(); 
   if (!bOk) throw (hr_exception(HRESULT_FROM_WIN32(dwNtErr), L"Failed to read file into buffer.", __WFUNCTION__)); 

   // Success!!!  Read the file into the xmlstream buffer.  
   // Terminate the string.
   pwcsXmlBuf[dwReadLength/2] = 0; 

   // Now call method which will Deserialize from a buffer.
   Deserialize(pwcsXmlBuf); 
}

/* GetMaxSerializeCharLen()
 *
 * SLB 28.mar.2016 CR.18552.V04; Added this comment header.
---------------------------------------------------------------------------------------------------------*/
size_t CAttributes::GetMaxSerializeCharLen() const 
{
   CSpdLock block(m_cs);                                                       // CR.21630; added.
   if (!block.Locked()) throw (hr_exception(E_FAIL, L"Another thread is accessing CAttributes object.", __WFUNCTION__)); 

   // Initialize value to include pl tags.
   // <pl>\n\n</pl>
   size_t stCharLenNeeded(11);              

   // Now add in length of key/value strings.
   for (TItcPropertyList it(m_Properties.begin()); it != m_Properties.end(); it++)
   {
      // Added in count for static part of tags <N><K></K><V></V></N>
      stCharLenNeeded+=21; 

      // Add in count for variable data (key name and value stream).
      stCharLenNeeded+=it->first.size(); 
      stCharLenNeeded+=it->second.size(); 
   }
   return stCharLenNeeded; 
}


/* FindNextXmlTagAndData()
 *
 * SLB 28.mar.2016 CR.18552.V04; Added this comment header.
---------------------------------------------------------------------------------------------------------*/
size_t CAttributes::FindNextXmlTagAndData(
   const CSpdCString &wsXmlBuf, 
   size_t stSearchPos, 
   CSpdCString &scsTag, 
   CSpdCString &scsTagData) const
{
   // presumes buffer starts with "<" char.
   size_t stTagOpenPos(wsXmlBuf.find(CSpdCString(L"<"), stSearchPos)); 
   if (stTagOpenPos==CSpdCString::npos) return CSpdCString::npos; 

   // Found open tag bracket, now look for close tag bracket.
   size_t stTagClosePos(wsXmlBuf.find(CSpdCString(L">"), stTagOpenPos)); 
   if (stTagClosePos == CSpdCString::npos) return CSpdCString::npos; 

   // We have a tag.
   wsXmlBuf.substr(scsTag, stTagOpenPos+1, stTagClosePos-stTagOpenPos-1 ); 

   return FindDataForTags(scsTag, wsXmlBuf, scsTagData, false, stTagOpenPos); 
}

/* Deserialize()
 *
 * SLB 28.mar.2016 CR.18552.V04; Fix for not calculating where to search for the container tag if the tagname is found.
 * SLB 30.aug.2016 CR.22134; Fix for not setting up the m_EscapedMembers list correctly.
---------------------------------------------------------------------------------------------------------*/
void CAttributes::Deserialize(const CSpdCString &wsXmlBuf, bool bTransparentOn, bool bClearFirst) 
{
   if (bClearFirst) Clear(); 
   if (wsXmlBuf.empty()) return; 

   CSpdCString scsUnescaped; 
   if (wsXmlBuf[0] == L'&') scsUnescaped = SpdUtils::XmlUnescapeString(wsXmlBuf); 
   const CSpdCString &scsXml(scsUnescaped.size() ? scsUnescaped : wsXmlBuf); 

   CSpdCString scsTag; 
   CSpdCString scsTagData; 
   bool bTestForContainerTag(true); 
   bool bTestForWrapperTag(true); 
   size_t stStartNextTagSrch(0);                                                    // CR.18552.V04; added.
   for (size_t stNodeSearchPos = FindNextXmlTagAndData(scsXml, 0, scsTag, scsTagData); 
      stNodeSearchPos != CSpdCString::npos; 
      stNodeSearchPos = FindNextXmlTagAndData(scsXml, stStartNextTagSrch, scsTag, scsTagData))
   {
      if (bTestForWrapperTag && scsTag==GetTagName()) { stStartNextTagSrch += scsTag.size() + 2; bTestForWrapperTag = false; continue; }
      if (bTestForContainerTag && scsTag==GetContainerTag()) { stStartNextTagSrch += scsTag.size() + 2; bTestForContainerTag = false; continue; }

      // Tag is not the wrapper tag. Add this tag and its data to the Member list.
      bTestForWrapperTag = false; 
      bTestForContainerTag = false; 
      stStartNextTagSrch = stNodeSearchPos;                                         // CR.18552.V04; added.
      SetMemberRaw(scsTag, scsTagData); 
      if (scsTagData.find(L"&") != CSpdCString::npos) m_EscapedMembers.emplace(scsTag); // CR.22134; added.
   }
}

/* SetMemberRaw() - 
 * 
 * SLB 23.oct.2015 CR.18552; created.
 * SLB 11.dec.2015 CR.18552.V01; Change so that anytime a member gets set the Modified flag gets 
 * set to true.
 * SLB 19.jul.2016 CR.21572; Optimization param added. based upon value for bAddToEscapedList parm,
 * method will add or erase the tag to/from the Escaped Members list.
---------------------------------------------------------------------------------------------*/
void CAttributes::SetMemberRaw(
   const CSpdCString &scsTag, 
   const CSpdCString &scsVal,
   bool bAddToEscapedList)                                                          // CR.21572; added.
{
   CSpdLock block(m_cs);                                                       // CR.21630; added.
   if (!block.Locked()) throw (hr_exception(E_FAIL, L"Another thread is accessing CAttributes object.", __WFUNCTION__)); 

   m_Properties[scsTag] = scsVal; 

   // Take care of escaped members list.
   if (bAddToEscapedList) m_EscapedMembers.emplace(scsTag);                         // CR.21572; added.
   else m_EscapedMembers.erase(scsTag);                                             // CR.21572; added.

   m_bIsModified = true; 
}

/* GetMemberRaw() - returns value for given tag. Note: this value is not unescaped, it is the raw Xml data
 * stored in the m_Properties map.
 * 
 * SLB 03.feb.2016 CR.18552.V03; created.
---------------------------------------------------------------------------------------------*/
bool CAttributes::GetMemberRaw(
   const CSpdCString &scsTag, 
   CSpdCString &scsVal) const 
{
   CSpdLock block(m_cs);                                                       // CR.21630; added.
   if (!block.Locked()) throw (hr_exception(E_FAIL, L"Another thread is accessing CAttributes object.", __WFUNCTION__)); 

   TItcPropertyList itVal(m_Properties.find(scsTag)); 
   if (itVal == m_Properties.end()) return false; 

   scsVal = itVal->second; 
   return true; 
}

bool CAttributes::GetMemberRaw(
   const CSpdCString &scsTag, 
   TPtrCString *ppscsVal) const 
{
   CSpdLock block(m_cs);                                                       // CR.21630; added.
   if (!block.Locked()) throw (hr_exception(E_FAIL, L"Another thread is accessing CAttributes object.", __WFUNCTION__)); 

   TItcPropertyList itVal(m_Properties.find(scsTag)); 
   if (itVal==m_Properties.end()) return false; 

   *ppscsVal = &itVal->second; 
   return true; 
}



/* GetMember() - for type double
 * 
 * SLB 06.may.2016 CR.21547; created.
---------------------------------------------------------------------------------------------*/
bool CAttributes::GetMember(
   const CSpdCString &scsTag, 
   double &val) const
{
   CSpdLock block(m_cs);                                                       // CR.21630; added.
   if (!block.Locked()) throw (hr_exception(E_FAIL, L"Another thread is accessing CAttributes object.", __WFUNCTION__)); 

   TItcPropertyList itVal(m_Properties.find(scsTag)); 
   if (itVal == m_Properties.end()) return false; 

   Primitive<double> p; 
   p.Deserialize(itVal->second, val); 
   return true; 
}

/* SetMember() - for type double
 * 
 * SLB 06.may.2016 CR.21547; created.
---------------------------------------------------------------------------------------------*/
void CAttributes::SetMember(
   const CSpdCString &scsTag, 
   double val) 
{
   wchar_t wcsVal[100]; 
   Primitive<double> p; 
   p.Serialize(val, wcsVal, 100); 

   SetMemberRaw(scsTag, wcsVal); 
}

/* GetMember() - 
 * 
 * SLB 23.oct.2015 CR.18552; created.
 * SLB 18.dec.2015 CR.18552.V02; Numeric values streamed in hex vs. dec.
---------------------------------------------------------------------------------------------*/
bool CAttributes::GetMember(
   const CSpdCString &scsTag, 
   QUADWORD &val,
   bool bInDec) const
{
   CSpdLock block(m_cs);                                                       // CR.21630; added.
   if (!block.Locked()) throw (hr_exception(E_FAIL, L"Another thread is accessing CAttributes object.", __WFUNCTION__)); 

   TItcPropertyList itVal(m_Properties.find(scsTag)); 
   if (itVal == m_Properties.end()) return false; 

   Primitive<QUADWORD> p(bInDec); 
   p.Deserialize(itVal->second, val); 
   return true; 
}

/* SetMember() - 
 * 
 * SLB 23.oct.2015 CR.18552; created.
 * SLB 18.dec.2015 CR.18552.V02; Numeric values streamed in hex vs. dec.
---------------------------------------------------------------------------------------------*/
void CAttributes::SetMember(
   const CSpdCString &scsTag, 
   QUADWORD val,
   bool bInDec) 
{
   wchar_t wcsVal[100]; 
   Primitive<QUADWORD> p(bInDec); 
   p.Serialize(val, wcsVal, 100); 

   SetMemberRaw(scsTag, wcsVal); 
}

/* GetMember() - 
 * 
 * SLB 23.oct.2015 CR.18552; created.
 * SLB 19.jul.2016 CR.21572; Attempting to optimize Serialization/de-serialization of strings. Now 
 * behaves like GetMemberRaw() if the tag is not in the m_EscapedMembers list. 
---------------------------------------------------------------------------------------------*/
bool CAttributes::GetMember(
   const CSpdCString &scsTag, 
   CSpdCString &val,
   bool bTransparencyOn) const
{
   CSpdLock block(m_cs);                                                       // CR.21630; added.
   if (!block.Locked()) throw (hr_exception(E_FAIL, L"Another thread is accessing CAttributes object.", __WFUNCTION__)); 

   if (bTransparencyOn) return GetMemberRaw(scsTag, val);  

   TItcPropertyList itVal(m_Properties.find(scsTag)); 
   if (itVal == m_Properties.end()) return false; 

   // CR.21572; Optimization, only user primitive SDeserialize() if the tag IS in the escaped members list. This 
   // indicates that it is escaped and needs to be un-escaped.
   // Otherwise, just pass along raw value.
   if (m_EscapedMembers.find(scsTag) != m_EscapedMembers.end())                     // CR.21572; modified.
      Primitive<CSpdCString>::SDeserialize(itVal->second, val); 
   else
      val = itVal->second;                                                          // CR.21572; added.
   return true; 
}

/* SetMember() - Note: code for this method does not use the Primitive<> object because it is more
 * efficient (using existing methods) to escape the string directly.
 * 
 * SLB 23.oct.2015 CR.18552; created.
 * SLB 20.jul.2016 CR.21572; Optimized so if string value does not need to be escaped, it is not 
 * copied an extra time.
---------------------------------------------------------------------------------------------*/
void CAttributes::SetMember(
   const CSpdCString &scsTag, 
   const CSpdCString &val,
   bool bTransparencyOn) 
{
   if (bTransparencyOn) { SetMemberRaw(scsTag, val); return; }

   // When escaping, tell XmlEscapeString() to ignore setting wsEscapedVal if the 
   // string did not need to be escaped.
   CSpdCString wsEscapedVal; 
   bool bWasValEscaped; 
   bWasValEscaped = SpdUtils::XmlEscapeString(val, wsEscapedVal, true);             // CR.21572; modified.

   // Call SetMemberRaw() with appropriate variable... Depending upon whether the 
   // Escaped string needs to be used or not.
   if (!bWasValEscaped) SetMemberRaw(scsTag, val, false);  
   else SetMemberRaw(scsTag, wsEscapedVal, true);                                   // CR.21572; modified.
}

/* GetMember() - wstring_crypto version
 * 
 * SLB 22.apr.2016 CR.18552.V05; created.
---------------------------------------------------------------------------------------------*/
bool CAttributes::GetMember(
   const CSpdCString &scsTag, 
   wstring_crypto &val) const
{
   CSpdLock block(m_cs);                                                       // CR.21630; added.
   if (!block.Locked()) throw (hr_exception(E_FAIL, L"Another thread is accessing CAttributes object.", __WFUNCTION__)); 

   TItcPropertyList itVal(m_Properties.find(scsTag)); 
   if (itVal == m_Properties.end()) return false; 

   Primitive<wstring_crypto> p; 
   p.Deserialize(itVal->second, val); 
   return true; 
}

/* SetMember() - wstring_crypto version
 * 
 * SLB 22.apr.2016 CR.18552.V05; created.
---------------------------------------------------------------------------------------------*/
void CAttributes::SetMember(
   const CSpdCString &scsTag, 
   const wstring_crypto &val) 
{
   Primitive<wstring_crypto> p; 
   wchar_t wcsSerializedBuf[512]; 
   p.Serialize(val, wcsSerializedBuf, 512); 

   SetMemberRaw(scsTag, wcsSerializedBuf); 
}

#if 0
/* GetMember() - Calls and returns the corresponding GetXXX() val for a job property (wstring overload)
 *
 * SLB 27.jan.2015 CR.18552.V03; created.
---------------------------------------------------------------------------*/
bool CAttributes::GetMember(
   TTransactionPropertyId tpi, 
   CSpdCString &val) const
{
   TTransactionPropertyType tpt(GetTPropertyType(tpi)); 
   if (tpt != ptString) throw (hr_exception(E_INVALIDARG, L"Internal error. Specified property can't use output type CSpdCString.", __WFUNCTION__)); 

   CSpdCString scsTag(GetTagKeyFromPropertyId(tpi)); 
   return GetMember(scsTag, val); 
}

/* GetMember() - Calls and returns the corresponding GetXXX() val for a job property (QUADWORD overload)
 *
 * SLB 27.jan.2015 CR.18552.V03; created.
 * SLB 07.mar.2019 CR.27492; Fix for retrieving QUADWORD data presuming Hex format. Attributes/Jobproperties always
 * store QUADWORDs as Decimal.
---------------------------------------------------------------------------*/
bool CAttributes::GetMember(
   TTransactionPropertyId tpi, 
   QUADWORD &val) const
{
   TTransactionPropertyType tpt(GetTPropertyType(tpi)); 
   if (tpt != ptQuadWord && tpt != ptBool && tpt != ptEnum) throw (hr_exception(E_INVALIDARG, L"Internal error. Specified property can't use output type QUADWORD.", __WFUNCTION__)); 

   CSpdCString scsTag(GetTagKeyFromPropertyId(tpi)); 
   return GetMember(scsTag, val, true);                                             // CR.27492; modified.
}

/* GetPropertyVal() - Calls and returns the corresponding GetXXX() val for a job property (FILETIME overload)
 *
 * SLB 27.jan.2015 CR.18552.V03; created.
---------------------------------------------------------------------------*/
bool CAttributes::GetMember(TTransactionPropertyId tpi, FILETIME &val) const
{
   TTransactionPropertyType tpt(GetTPropertyType(tpi)); 
   if (tpt != ptFileTime) throw (hr_exception(E_INVALIDARG, L"Internal error. Specified property can't use output type FILETIME.", __WFUNCTION__)); 

   CSpdCString scsTag(GetTagKeyFromPropertyId(tpi)); 
   return GetMember(scsTag, val); 
}
#endif

} // end of namespace
