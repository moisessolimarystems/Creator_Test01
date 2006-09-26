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


// unknown/uninitialized error
static const unsigned int MessageInvalid = 0;

// key system errors
static const unsigned int MessageSystemDriver = 1;

// invalid/insufficient licensing
static const unsigned int MessageViolationVersion = 2;
static const unsigned int MessageViolation = 3;
static const unsigned int MessageViolationInactive = 4;
static const unsigned int MessageViolationInactiveSoon = 5;
static const unsigned int MessageViolationClock = 6;

// key modification errors
static const unsigned int MessageKeyWriteAccessDenied = 7;
static const unsigned int MessageKeyWriteFailure = 8;

// temp key information
static const unsigned int MessageTempKeyDaysRemaining = 9;
static const unsigned int MessageTempKeyExpiresToday = 10;
static const unsigned int MessageTempKeyExpired = 11;
static const unsigned int MessageTempKeyNotActive = 12;
static const unsigned int MessageTempKeyDecrementing = 13;

// entering passwords
static const unsigned int MessagePasswordPermanent = 14;
static const unsigned int MessagePasswordExtension = 15;
static const unsigned int MessagePasswordVersion = 16;
static const unsigned int MessagePasswordModule = 17;
static const unsigned int MessagePasswordSPDOutput = 18;
static const unsigned int MessagePasswordSPDPPM = 19;

// misc
static const unsigned int MessageClientTimeout = 20;
static const unsigned int MessageTooManyReleases = 21;

// license manager
static const unsigned int MessageLMTempKeyExpired = 22;
static const unsigned int MessageLMTempKeyActive = 23;
static const unsigned int MessageLMTempKeyStatus = 24;

// grace period
static const unsigned int MessageGracePeriodStarted = 25;
static const unsigned int MessageGracePeriodEnded = 26;


// Do not change the order of these items!!!!
// Only append items to the end of the list, do not delete or insert items anywhere else!!!!
// Doing so will cause the lookup numbers above to return the wrong strings.
// Modifying the above strings will cause a mismatch in clients that rely on those numbers.
static const wchar_t* LicensingMessageStringTable[256] = 
{
// unknown/uninitialized error
	/*MessageInvalid*/ L"Invalid Message ID.",

// key system errors
	/*MessageSystemDriver*/ L"Protection key driver initialization error.",

// invalid/insufficient licensing
	/*MessageViolationVersion*/ L"Key is not licensed for product version %hx.%03hx. The key will be inactive until licensing is modified.",
	/*MessageViolation*/ L"You have been in violation for %d day(s). %s",
	/*MessageViolationInactive*/ L"The key will be inactive until licensing is modified.",
	/*MessageViolationInactiveSoon*/ L"The key will soon be inactive until licensing is modified.",
	/*MessageViolationClock*/ L"System clock differs from key's clock. Please set system clock forward to original value. The key will be inactive until clock difference is corrected.",

// key modification errors
	/*MessageKeyWriteAccessDenied*/ L"Unable to modify key due to key type restriction.  Please contact Solimar Technical Support.",
	/*MessageKeyWriteFailure*/ L"Protection Key write failure.  Password is valid, but unable to modify key.  Please contact Solimar Technical Support.",

// temp key information
	/*MessageTempKeyDaysRemaining*/ L"Your temporary license has %d day(s) remaining.",
	/*MessageTempKeyExpiresToday*/ L"Your temporary license expires today.",
	/*MessageTempKeyExpired*/ L"Your temporary license has expired.",
	/*MessageTempKeyNotActive*/ L"Key has not been activated.", //"Do you wish to activate the %hd day " "temporary license for this key?"
	/*MessageTempKeyDecrementing*/ L"Key trial hours has been decremented.",

// entering passwords
	/*MessagePasswordPermanent*/ L"Password Correct. You now have a permanent license to use this key.",
	/*MessagePasswordExtension*/ L"Password Correct. Temporary license to use this key extended %d day(s).",
	/*MessagePasswordVersion*/ L"Password Correct. Key now licensed for product version %hx.%03hx.",
	/*MessagePasswordModule*/ L"Password Correct. \"%s\" module now licensed for %d unit(s).",
	/*MessagePasswordSPDOutput*/ L"Password Correct. Key now licensed for %d output device(s).",
	/*MessagePasswordSPDPPM*/ L"Password Correct. Key now licensed for %d pages per minute.",

// misc
	/*MessageClientTimeout*/ L"A client has timed out in responding to the key server. That client's licenses are revoked.",
	/*MessageTooManyReleases*/ L"A key was released more times than it was obtained.",

// license manager
	/*MessageTempKeyExpired*/ L"Trial key has expired and is no longer active. Hours Left: %d, Expires: %s.",
	/*MessageTempActive*/ L"Trial key is now active. Hours Left: %d, Expires: %s.",
	/*MessageTempStatus*/ L"Trial key status update. Hours Left: %d, Expires: %s.",

// grace period
	/*MessageGracePeriodStarted*/ L"Licensing has entered its grace period. The most common reasons for this are one of your keys has expired or communication to one of your key servers has been lost.",
	/*MessageGracePeriodEnded*/ L"Licensing has been restored, the grace period has ended.",
};


