//----------------------------------------------------------------------------------------*
// filename - SpdException.cpp                                                            *
//                                                                                        *
//----------------------------------------------------------------------------------------*
#include "L_SpdException.h"
#include <list>
// CEnterExitMsg

HANDLE CEnterExitMsg::s_hMutex = CreateMutex(NULL, FALSE, NULL); 
std::map<DWORD, DWORD> CEnterExitMsg::s_ThreadEnterIndices; 
const std::wstring wsArray[] =                                                            // CR.24219; added.
{
   L"", 
   L"   ", 
   L"      ", 
   L"         ", 
   L"            ",
   L"               ",
   L"                  ",
   L"                     ",
   L"                        ",
   L"                           ",
   L"                              ",
   L"                                 ",
   L"                                    ",
   L"                                       ",
   L"                                          ",
   L"                                             ",
}; 
std::vector<std::wstring> CEnterExitMsg::s_SpaceStringTable(wsArray, wsArray + sizeof(wsArray)/sizeof(wsArray[0]));  // CR.24219; modified.

void CEnterExitMsg::GetFormattedTraceMsg(
   const wchar_t *pwcsInMsg, 
   wchar_t *pwcsFormatted, 
   size_t stFormattedBufLen)
{
   // ensure that output string is always valid.
   if (pwcsFormatted) pwcsFormatted[0] = 0;

   // return immediately if there is no text in message string.
   if (!pwcsInMsg || pwcsInMsg[0]==0) return;

   DWORD dwThreadId = GetCurrentThreadId();
   size_t stSpaceOffsetIndex = GetThreadSpaceIndex(dwThreadId);

   // Don't allow the space offsetindex to exceed the length of the string table.
   if (stSpaceOffsetIndex >= s_SpaceStringTable.size()) stSpaceOffsetIndex = s_SpaceStringTable.size()-1;
#if 1
   // break string up into a list of substrings so that message segments on new lines get formatted well.
   typedef std::list<std::wstring> TStringList;
   typedef TStringList::iterator TItStringList;

   TStringList sub_strings;
   std::wstring wsIn(pwcsInMsg);
   bool bEnd(false);
   size_t stStart(0); 
   size_t stLen(0);
   for (size_t stPos(0); ;)
   {
      if (stPos < wsIn.size())
      {
         if (wsIn[stPos] >= 0x20 && !bEnd) { stPos++; continue; }
         if (wsIn[stPos] < 0x20) { bEnd = true; stPos++;continue; }
         stLen = stPos - stStart;
      }
      else
         stLen = std::wstring::npos;
      bEnd = false;

      // First non-control char after control chars were encountered. ie. beginning of next record found.
      if (stLen) sub_strings.push_back(wsIn.substr(stStart, stLen));
      if (stPos >= wsIn.size()) break;

      // Look for next substring.
      stStart = stPos++;
   }

   if (sub_strings.empty()) { pwcsFormatted[0] = 0; return; }

   if (sub_strings.size() == 1)
   {
      _snwprintf_s(pwcsFormatted, stFormattedBufLen, _TRUNCATE, L"[%06u]   %s%s",
         dwThreadId,
         s_SpaceStringTable[stSpaceOffsetIndex].c_str(),
         sub_strings.begin()->c_str());
      return;
   }

   // More than 1 line in list. add custom formatting for this mode.
   TItStringList it(sub_strings.begin());
   size_t stOutPos(0);
   size_t stLine(0);
   for (; it != sub_strings.end(); it++, stLine++)
   {
      stLen = _snwprintf_s(pwcsFormatted+stOutPos, stFormattedBufLen-stOutPos, _TRUNCATE, L"[%06u.%zu] %s%s",
         dwThreadId,
         stLine,
         s_SpaceStringTable[stSpaceOffsetIndex].c_str(),
         it->c_str());
      if (stLen < 0) return;
      stOutPos += stLen;
   }

#else
   // Format message and output it.
   _snwprintf_s(
      pwcsFormatted, stFormattedBufLen, _TRUNCATE,
      L"[%06u] %s%s.",
      dwThreadId,
      s_SpaceStringTable[stSpaceOffsetIndex].c_str(),
      pwcsInMsg);
#endif
}


#if 0
void CEnterExitMsg::TraceMsg(const wchar_t *pwcsMsg)
{
   // Format message and output it.
   wchar_t wcsMsg[2048];
   GetFormattedTraceMsg(pwcsMsg, wcsMsg, 2048);
   OutputDebugStringW(wcsMsg);
}
#else
#pragma warning (push)
#pragma warning (disable: 4793)

/* TraceMsg()
 * 
 * SLB 03.feb.2020 CR.28872.V02; Dawned on me to add a LineFeed so that debug msgs in VisualStudio are not appended to each other.
 * SLB 25.jun.2020 CR.29503; Make sure that if invalid parm list is passed which causes exception in _vsnwprintf_s(), that the 
 * exception gets handled here.
-----------------------------------------------------------------------------------------------------*/
void CEnterExitMsg::TraceMsg(
   const wchar_t* pwcsFormatString, 
   ...)
{
   if (!pwcsFormatString || pwcsFormatString[0]==0) return;

   wchar_t wcsMsg[2048];
   int nSize(-1);
   try                                                                              // CR.29503; added try {} around existing code.
   {

      // Format the message.
      va_list pArg;
      va_start(pArg, pwcsFormatString);
      nSize = _vsnwprintf_s(wcsMsg, _TRUNCATE, pwcsFormatString, pArg);
      va_end(pArg);
   }
   catch (...)                                                                      // CR.29503; added catch() around existing code.
   {
      // eat the exception, but make sure that this is treated as an error.
      nSize = -1;
   }
   if (nSize < 0)
   {
      _snwprintf_s(wcsMsg, _TRUNCATE, L"[%06u] %s() error occurred formatting string.", GetCurrentThreadId(), __WFUNCTION__);
      OutputDebugStringW(wcsMsg);
      return;
   }

   wchar_t wcsOutMsg[2048];
   GetFormattedTraceMsg(wcsMsg, wcsOutMsg, 2048);

   OutputDebugStringW(L"\n");                                                       // CR.28872.V02; added.
   OutputDebugStringW(wcsOutMsg);
}
#pragma warning (pop)
#endif

void CEnterExitMsg::IncrementThreadSpaceIndex(
   DWORD dwThreadId)
{
   // Increment the space offset index, for this thread, in the static table.
   // Make sure to do this in a thread safe manner.
   CSpdBlock block(s_hMutex);

   std::map<DWORD, DWORD>::iterator itThreadEntry(s_ThreadEnterIndices.find(dwThreadId));
   if (itThreadEntry == s_ThreadEnterIndices.end())
      s_ThreadEnterIndices[dwThreadId] = 1;
   else
      itThreadEntry->second++;
}


void CEnterExitMsg::DecrementThreadSpaceIndex(
   DWORD dwThreadId)
{
   // Decrement the space offset index, for this thread, in the static table.
   // Make sure to do this in a thread safe manner.
   CSpdBlock block(s_hMutex);

   // If there is no entry for this thread, exit.
   std::map<DWORD, DWORD>::iterator itThreadEntry(s_ThreadEnterIndices.find(dwThreadId));
   if (itThreadEntry == s_ThreadEnterIndices.end()) return;

   // only decrement the entry if the current count is greater than 0
   if (itThreadEntry->second > 0) itThreadEntry->second--;
}


DWORD CEnterExitMsg::GetThreadSpaceIndex(
   DWORD dwThreadId)
{
   CSpdBlock block(s_hMutex);

   std::map<DWORD, DWORD>::iterator itThreadEntry(s_ThreadEnterIndices.find(dwThreadId));
   if (itThreadEntry == s_ThreadEnterIndices.end()) return 0;

   return itThreadEntry->second;
}

//
// win32_exception class member definitions.
//
// SLB - Added this class to help catch win32 exceptions.
//
_se_translator_function SpdException::win32_exception::s_old_handler = NULL; 	

void SpdException::win32_exception::install_handler()
{  
   // known bug: exception handlers are managed on a per thread basis.
   // however, this class only manages one static version (vs. one for each
   // thread).  This is not a real issue, at this time, because there is no
   // handler by default (it is NULL).
   s_old_handler = _set_se_translator(win32_exception::translate);
}

void SpdException::win32_exception::uninstall_handler()
{  
    _set_se_translator(s_old_handler);
}


void SpdException::win32_exception::translate(
   unsigned code, 
   EXCEPTION_POINTERS* info)
{
   // Windows guarantees that *(info->ExceptionRecord) is valid
   switch (code) 
   {
   case EXCEPTION_ACCESS_VIOLATION:
      throw access_violation(*(info->ExceptionRecord));
      break;
   default:
      throw win32_exception(*(info->ExceptionRecord));
   }
}
	
SpdException::win32_exception::win32_exception(const EXCEPTION_RECORD& info) : 
   mWhat("Win32 exception"), 
   mWhere(info.ExceptionAddress), 
   mCode(info.ExceptionCode)
{
   switch (info.ExceptionCode) 
   {
   case EXCEPTION_ACCESS_VIOLATION:
      mWhat = "Access violation"; 
      break; 
   case EXCEPTION_FLT_DIVIDE_BY_ZERO:
   case EXCEPTION_INT_DIVIDE_BY_ZERO:
      mWhat = "Divide by zero"; 
      break; 
   }
}
	

//
// access_violation class member definitions.
//
// SLB - Added this class to help catch win32 exceptions.
//

SpdException::access_violation::access_violation(const EXCEPTION_RECORD& info) : 
   win32_exception(info), 
   mIsWrite(false), 
   mBadAddress(0)
{
    mIsWrite = info.ExceptionInformation[0] == 1;
    mBadAddress = reinterpret_cast<win32_exception ::Address>(info.ExceptionInformation[1]);
}


/********************************************************************
 *
 * class hr_exception memebers
 *
 ********************************************************************/ 
/* constructor()
 *
 * SLB 05.feb.2014 CR.17498.V01; Added.
 * SLB 06.mar.2014 CR.17478; Updated class constructor. 
 * SLB 27.mar.2014 CR.17829; Added initialization of the m_bMessageWasDisplayed member.
 -----------------------------------------------------------------------------------------------------------*/
SpdException::hr_exception::hr_exception(
   HRESULT hr,
   const std::wstring &wsMessage,                                                   // CR.17478; modified.
   const std::wstring &wsMethod                                                     // CR.17478; modified.
   ) : 
   m_hr(hr),
   m_wsMessage(wsMessage),
   m_wsMethod(wsMethod),
   m_bMessageWasDisplayed(false)                                                    // CR.17829; added.
{}

/* copy constructor()
 *
 * SLB 05.feb.2014 CR.17498.V01; Added.
 * SLB 27.mar.2014 CR.17829; Added initialization of the m_bMessageWasDisplayed member.
 -----------------------------------------------------------------------------------------------------------*/
SpdException::hr_exception::hr_exception(const hr_exception &cpy) :
   m_hr(cpy.m_hr),
   m_wsMessage(cpy.m_wsMessage),
   m_wsMethod(cpy.m_wsMethod),
   m_bMessageWasDisplayed(cpy.m_bMessageWasDisplayed)                               // CR.17829; added.
{}

/* assignment operator
 *
 * SLB 05.feb.2014 CR.17498.V01; Added.
 * SLB 27.mar.2014 CR.17829; Added initialization of the m_bMessageWasDisplayed member.
 -----------------------------------------------------------------------------------------------------------*/
SpdException::hr_exception & SpdException::hr_exception::operator = (const hr_exception &cpy) 
{
   m_hr = cpy.m_hr; 
   m_wsMessage = cpy.m_wsMessage; 
   m_wsMethod = cpy.m_wsMethod; 
   m_bMessageWasDisplayed = cpy.m_bMessageWasDisplayed;                             // CR.17829; added.
   return *this; 
}

/* GetHr()
 *
 * SLB 05.feb.2014 CR.17498.V01; Added.
 -----------------------------------------------------------------------------------------------------------*/
HRESULT SpdException::hr_exception::GetHr() const 
{
   return m_hr; 
}

/* GetMessage()
 *
 * SLB 05.feb.2014 CR.17498.V01; Added.
 * SLB 03.feb.2019 CR.27366; Appended W to method name so that the Windows GetMessage() macro will work 
 * seamlessly when UNICODE is defined.
 -----------------------------------------------------------------------------------------------------------*/
std::wstring SpdException::hr_exception::GetMessageW() const                        // CR.27366; modified.
{
   return m_wsMessage; 
}

/* GetMethod()
 *
 * SLB 05.feb.2014 CR.17498.V01; Added.
 -----------------------------------------------------------------------------------------------------------*/
std::wstring SpdException::hr_exception::GetMethod() const 
{
   return m_wsMethod; 
}

/* WasMessageDisplayed()
 *
 * SLB 27.mar.2014 CR.17829; Added.
 -----------------------------------------------------------------------------------------------------------*/
bool SpdException::hr_exception::WasMessageDisplayed() const 
{
   return m_bMessageWasDisplayed; 
}

/* SetMessageWasDisplayed()
 *
 * SLB 27.mar.2014 CR.17829; Added.
 -----------------------------------------------------------------------------------------------------------*/
void SpdException::hr_exception::SetMessageWasDisplayed(
   bool bMessageWasDisplayed)  
{
   m_bMessageWasDisplayed = bMessageWasDisplayed; 
}

   
/********************************************************************
 *
 * class win_exception memebers
 *
 ********************************************************************/ 
/* constructor()
 *
 * SLB 20.feb.2014 CR.17478; Added.
 -----------------------------------------------------------------------------------------------------------*/
SpdException::win_exception::win_exception(
   HRESULT hr,
   const std::wstring &wsMessage,
   const std::wstring &wsMethod
   ) : 
   m_dwNtErr(hr),
   m_wsMessage(wsMessage),
   m_wsMethod(wsMethod)
{}

/* copy constructor()
 *
 * SLB 20.feb.2014 CR.17478; Added.
 -----------------------------------------------------------------------------------------------------------*/
SpdException::win_exception::win_exception(const win_exception &cpy) :
   m_dwNtErr(cpy.m_dwNtErr),
   m_wsMessage(cpy.m_wsMessage),
   m_wsMethod(cpy.m_wsMethod)
{}

/* assignment operator
 *
 * SLB 20.feb.2014 CR.17478; Added.
 -----------------------------------------------------------------------------------------------------------*/
SpdException::win_exception & SpdException::win_exception::operator = (const win_exception &cpy) 
{
   m_dwNtErr = cpy.m_dwNtErr; 
   m_wsMessage = cpy.m_wsMessage; 
   m_wsMethod = cpy.m_wsMethod; 
   return *this; 
}

/* GetNtErr()
 *
 * SLB 20.feb.2014 CR.17478; Added.
 -----------------------------------------------------------------------------------------------------------*/
DWORD SpdException::win_exception::GetNtErr() const 
{
   return m_dwNtErr; 
}

/* GetMessage()
 *
 * SLB 20.feb.2014 CR.17478; Added.
 -----------------------------------------------------------------------------------------------------------*/
std::wstring SpdException::win_exception::GetMessage() const 
{
   return m_wsMessage; 
}

/* GetMethod()
 *
 * SLB 20.feb.2014 CR.17478; Added.
 -----------------------------------------------------------------------------------------------------------*/
std::wstring SpdException::win_exception::GetMethod() const 
{
   return m_wsMethod; 
}
