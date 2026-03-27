#pragma once

#include <tchar.h>
#include <comdef.h>
#include <time.h>

// list of static key code strings used to generate key messages

typedef enum
{
	MT_ERROR = 0,
	MT_WARNING = 1, 
	MT_INFO = 2
} EMessageType;

// key system errors
static const wchar_t* MessageSystemDriver = L"Protection key driver initialization error.";

// invalid/insufficient licensing
static const wchar_t* MessageViolationVersion = L"Key is not licensed for product version %hx.%03hx. The key will be inactive until licensing is modified.";
static const wchar_t* MessageViolation = L"You have been in violation for %d day(s). %s";
static const wchar_t* MessageViolationInactive = L"The key will be inactive until licensing is modified.";
static const wchar_t* MessageViolationInactiveSoon = L"The key will soon be inactive until licensing is modified.";
static const wchar_t* MessageViolationClock = L"System clock differs from key's clock. Please set system clock forward to original value. The key will be inactive until clock difference is corrected.";

// key modification errors
static const wchar_t* MessageKeyWriteAccessDenied = L"Unable to modify key due to key type restriction.  Please contact Solimar Technical Support.";
static const wchar_t* MessageKeyWriteFailure = L"Protection Key write failure.  Password is valid, but unable to modify key.  Please contact Solimar Technical Support.";

// temp key information
static const wchar_t* MessageTempKeyDaysRemaining = L"Your temporary license has %d day(s) remaining.";
static const wchar_t* MessageTempKeyExpiresToday = L"Your temporary license expires today.";
static const wchar_t* MessageTempKeyExpired = L"Your temporary license has expired.";
static const wchar_t* MessageTempKeyNotActive = L"Key has not been activated."; //"Do you wish to activate the %hd day " "temporary license for this key?"
static const wchar_t* MessageTempKeyDecrementing = L"Key trial hours has been decremented.";

// entering passwords
static const wchar_t* MessagePasswordPermanent = L"Password Correct. You now have a permanent license to use this key.";
static const wchar_t* MessagePasswordExtension = L"Password Correct. Temporary license to use this key extended %d day(s).";
static const wchar_t* MessagePasswordVersion = L"Password Correct. Key now licensed for product version %hx.%03hx.";
static const wchar_t* MessagePasswordModule = L"Password Correct. \"%s\" module now licensed for %d unit(s).";
static const wchar_t* MessagePasswordSPDOutput = L"Password Correct. Key now licensed for %d output device(s).";
static const wchar_t* MessagePasswordSPDPPM = L"Password Correct. Key now licensed for %d pages per minute.";

// misc
static const wchar_t* MessageClientTimeout = L"A client has timed out in responding to the key server. That client's licenses are revoked.";
static const wchar_t* MessageTooManyReleases = L"A key was released more times than it was obtained.";
