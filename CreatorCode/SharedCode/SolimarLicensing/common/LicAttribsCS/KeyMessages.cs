using System;
using System.Collections.Generic;
using System.Text;

namespace Solimar.Licensing
{
    /// <summary>
    /// This file is based on IDs found in KeyMessages.h
    /// This file is handed edited, and should be kept in sync with any id's 
    /// that are added to KeyMessages.h
    /// </summary>
    public class KeyMessages
    {
        public enum EventId : int
        {
            ClientTimeout = 20,
            // GracePeriodStarted = 25,
            // GracePeriodEnded = 26,
            // KeysRestoreKeyServer = 31,
            // KeysUseBackupKeyServer = 32,
            SoftwareModuleExpiration = 37,
            Generic = 38,
            // ViolationEnter = 39,
            // ViolationLeave = 40,
            // ValidInitialize = 41,
            GenericError = 42,
            ClientTimeoutProductAndAppInst = 43,
        }

        private static System.Collections.Generic.SortedDictionary<int, string> lookUpMessageList = new SortedDictionary<int, string>();

        static KeyMessages()
        {
            lookUpMessageList.Add((int)EventId.ClientTimeout, "Client timed out messages");
            // lookUpMessageList.Add((int)EventId.GracePeriodStarted, "Licensing Grace Period Messages (Entering)");
            // lookUpMessageList.Add((int)EventId.GracePeriodEnded, "Licensing Grace Period Messages (Leaving)");
            // lookUpMessageList.Add((int)EventId.KeysRestoreKeyServer, "Licensing Restored");
            // lookUpMessageList.Add((int)EventId.KeysUseBackupKeyServer, "Backup Server License Messages");
            lookUpMessageList.Add((int)EventId.SoftwareModuleExpiration, "Expiration Messages");
            lookUpMessageList.Add((int)EventId.Generic, "Generic Messages");
            // lookUpMessageList.Add((int)EventId.ViolationEnter, "Licensing Violation Messages (Entering)");
            // lookUpMessageList.Add((int)EventId.ViolationLeave, "Licensing Violation Messages (Leaving)");
            // lookUpMessageList.Add((int)EventId.ValidInitialize, "Licensing Initialization Messages");
           	lookUpMessageList.Add((int)EventId.GenericError, "Generic Error Messages");
            lookUpMessageList.Add((int)EventId.ClientTimeoutProductAndAppInst, "Client timed out messages");
        }


        //#region Defines for EventIds

        //// unknown/uninitialized error
        //public static readonly int MessageInvalid = 0;

        //// key system errors
        //public static readonly int MessageSystemDriver = 1;

        //// invalid/insufficient licensing
        //public static readonly int MessageViolationVersion = 2;
        //public static readonly int MessageViolation = 3;
        //public static readonly int MessageViolationInactive = 4;
        //public static readonly int MessageViolationInactiveSoon = 5;
        //public static readonly int MessageViolationClock = 6;

        //// key modification errors
        //public static readonly int MessageKeyWriteAccessDenied = 7;
        //public static readonly int MessageKeyWriteFailure = 8;

        //// temp key information
        //public static readonly int MessageTempKeyDaysRemaining = 9;
        //public static readonly int MessageTempKeyExpiresToday = 10;
        //public static readonly int MessageTempKeyExpired = 11;
        //public static readonly int MessageTempKeyNotActive = 12;
        //public static readonly int MessageTempKeyDecrementing = 13;

        //// entering passwords
        //public static readonly int MessagePasswordPermanent = 14;
        //public static readonly int MessagePasswordExtension = 15;
        //public static readonly int MessagePasswordVersion = 16;
        //public static readonly int MessagePasswordModule = 17;
        //public static readonly int MessagePasswordSPDOutput = 18;
        //public static readonly int MessagePasswordSPDPPM = 19;

        //// misc
        //public static readonly int MessageClientTimeout = 20;
        //public static readonly int MessageTooManyReleases = 21;

        //// license manager
        //public static readonly int MessageLMTempKeyExpired = 22;
        //public static readonly int MessageLMTempKeyActive = 23;
        //public static readonly int MessageLMTempKeyStatus = 24;

        //// grace period
        //public static readonly int MessageGracePeriodStarted = 25;
        //public static readonly int MessageGracePeriodEnded = 26;

        ////application instance key errors
        //public static readonly int MessageBaseKeysNotMatchingModuleByModule = 27;
        //public static readonly int MessageKeysNotMatchingApplicationInstance = 28;
        //public static readonly int MessageKeysNoApplicationInstance = 29;
        //public static readonly int MessageKeysNoBaseKey = 30;

        //// backup server
        //public static readonly int MessageKeysRestoreKeyServer = 31;
        //public static readonly int MessageKeysUseBackupKeyServer = 32;

        //// entering passwords2
        //public static readonly int MessagePasswordRemote = 33;
        //public static readonly int MessagePasswordApplicationInstance = 34;
        //public static readonly int MessagePasswordPacketVerificationCode = 35;
        //public static readonly int MessagePasswordSPDPPM2 = 36;

        //// softwareLicenseServer - module expiration
        //public static readonly int MessageSoftwareModuleExpiration = 37;

        //// generic message
        //public static readonly int MessageGeneric = 38;
        //public static readonly int MessageViolationEnter = 39;
        //public static readonly int MessageViolationLeave = 40;
        //public static readonly int MessageValidInitialize = 41;
        //public static readonly int MessageGenericError = 42;

        ////misc
        //public static readonly int MessageClientTimeoutProductAndAppInst = 43;

        ////last item
        //public static readonly int MessageLastId = 44;

        

        //private static List<string> lookUpMessageList = new List<string>()
        //{
        //    // unknown/uninitialized error
        //    /*MessageInvalid*/ "Invalid Message ID.",

        //    // key system errors
        //    /*MessageSystemDriver*/ "Protection key driver initialization error.",

        //    // invalid/insufficient licensing
        //    /*MessageViolationVersion*/ "Key is not licensed for product version %d.%02x. The key will be inactive until licensing is modified.",
        //    /*MessageViolation*/ "You have been in violation for %d day(s). %s",
        //    /*MessageViolationInactive*/ "The key will be inactive until licensing is modified.",
        //    /*MessageViolationInactiveSoon*/ "The key will soon be inactive until licensing is modified.",
        //    /*MessageViolationClock*/ "System clock differs from key's clock. Please set system clock forward to original value. The key will be inactive until clock difference is corrected.",

        //    // key modification errors
        //    /*MessageKeyWriteAccessDenied*/ "Unable to modify key due to key type restriction.  Please contact Solimar Technical Support.",
        //    /*MessageKeyWriteFailure*/ "Protection Key write failure.  Password is valid, but unable to modify key.  Please contact Solimar Technical Support.",

        //    // temp key information
        //    /*MessageTempKeyDaysRemaining*/ "Your temporary license has %d day(s) remaining.",
        //    /*MessageTempKeyExpiresToday*/ "Your temporary license expires today.",
        //    /*MessageTempKeyExpired*/ "Your temporary license has expired.",
        //    /*MessageTempKeyNotActive*/ "Key has not been activated.", //"Do you wish to activate the %hd day " "temporary license for this key?"
        //    /*MessageTempKeyDecrementing*/ "Key trial hours have been decremented.",

        //    // entering passwords
        //    /*MessagePasswordPermanent*/ "Password Correct. You now have a permanent license to use this key.",
        //    /*MessagePasswordExtension*/ "Password Correct. Temporary license to use this key extended %d day(s).",
        //    /*MessagePasswordVersion*/ "Password Correct. Key now licensed for product version %hx.%03hx.",
        //    /*MessagePasswordModule*/ "Password Correct. \"%s\" module now licensed for %d unit(s).",
        //    /*MessagePasswordSPDOutput*/ "Password Correct. Key now licensed for %d output device(s).",
        //    /*MessagePasswordSPDPPM*/ "Password Correct. Key now licensed for %d pages per minute.",

        //    // misc
        //    /*MessageClientTimeout*/ "A client has timed out in responding to the key server. That client's licenses are revoked.",
        //    /*MessageTooManyReleases*/ "A key was released more times than it was obtained.",

        //    // license manager
        //    /*MessageTempKeyExpired*/ "Trial key has expired and is no longer active. Hours Left: %d, Expires: %s.",
        //    /*MessageTempActive*/ "Trial key is now active. Hours Left: %d, Expires: %s.",
        //    /*MessageTempStatus*/ "Trial key status update. Hours Left: %d, Expires: %s.",

        //    // grace period
        //    /*MessageGracePeriodStarted*/ "Licensing has entered its grace period. The most common reasons for this are one of your keys has expired or communication to one of your key servers has been lost.",
        //    /*MessageGracePeriodEnded*/ "Licensing has been restored, the grace period has ended.",

        //    //application instance key errors
        //    /*MessageBaseKeysNotMatchingModuleByModule*/ "Base Keys do not match module for module, all keys on Server \"%s\" will be ignored.",
        //    /*MessageKeysNotMatchingApplicationInstance*/ "Keys do not match by Application Instances for all the Key on the server, all keys on Server \"%s\" will be ignored.",
        //    /*MessageKeysNoApplicationInstance*/ "Unable to find an available Application Instance, all keys on Server \"%s\" will be ignored.",
        //    /*MessageKeysNoBaseKey*/ "Unable to locate a Base Key on the Server, all keys on Server \"%s\" will be ignored.",
            	
        //    //backup key server messages
        //    /*MessageKeysRestoreKeyServer*/	"Licensing has been restored on License Server: %s.",
        //    /*MessageKeysUseBackupKeyServer*/	"Unable to validate licensing on License Server %s, switching to Backup License Server: %s.",

        //    // entering passwords2
        //    /*MessagePasswordRemote*/	"Password Correct. Key now licensed for remote access.",
        //    /*MessagePasswordApplicationInstance*/	"Password Correct. Key now licensed for %d application instance(s).",
        //    /*MessagePasswordPacketVerificationCode*/		"Password Packet Correct. Verification Code: %s.",
        //    /*MessagePasswordSPDPPM2*/ "Password Correct. \"%s\" module now licensed for %d pages per minute.",

        //    //softwareLicenseServer - module expiration
        //    /*MessageSoftwareModuleExpiration*/ "Warning, Product: %s (%d)\n%s\n",

        //    //MessageGeneric
        //    /*MessageGeneric*/ "%s",
        //    /*MessageViolationEnter*/ "%s",
        //    /*MessageViolationLeave*/ "%s",
        //    /*MessageValidInitialize*/ "%s",
        //    /*MessageGenericError*/ "%s",

        //    //Misc
        //    /*MessageClientTimeoutProductAndAppInst*/ "%s",

        //    // general errors
        //    /*MessageRpcError*/// "A client has timed out in responding to the key server. That client's licenses are revoked.",
        //};
        //#endregion

        public static string GetDescription(int _eventId)
        {
            //return (_eventId < lookUpMessageList.Count) ? lookUpMessageList[_eventId] : string.Empty;
            return (lookUpMessageList.ContainsKey(_eventId)) ? lookUpMessageList[_eventId] : string.Empty;
        }
    }
}
