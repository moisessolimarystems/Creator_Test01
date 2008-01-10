using System;
namespace Solimar
{
	namespace Licensing
	{
		public class CSolLicHRESULT
		{
			#region Defines for Custom HRESULTS
			
			private const uint HRESULT_BASE = 0x80040000;
			private const uint ITF_OFFSET = 0x0000;
			private const uint ITF_MIN = HRESULT_BASE+ITF_OFFSET;
			private const uint ITF_CR_MIN = ITF_MIN+0x9000;
			private const uint ITF_LIC_SVR_OFFSET = ITF_CR_MIN+0x0000;
			private const uint ITF_RNBO_OFFSET = ITF_CR_MIN+0x0100;
			private const uint ITF_MAX = 0xFFFF;
		
			#region Rainbow Error Codes
			
			public const uint SP_INVALID_FUNCTION_CODE = 1;
			public const uint SP_INVALID_PACKET = 2;
			public const uint SP_UNIT_NOT_FOUND = 3;
			public const uint SP_ACCESS_DENIED = 4;
			public const uint SP_INVALID_MEMORY_ADDRESS = 5;
			public const uint SP_INVALID_ACCESS_CODE = 6;
			public const uint SP_PORT_IS_BUSY = 7;
			public const uint SP_WRITE_NOT_READY = 8;
			public const uint SP_NO_PORT_FOUND = 9;
			public const uint SP_ALREADY_ZERO = 10;
			public const uint SP_DRIVER_NOT_INSTALLED = 12;
			public const uint SP_COMMUNICATIONS_ERROR = 13;
			public const uint SP_VERSION_NOT_SUPPORTED = 18;
			public const uint SP_OS_ENVIRONMENT_NOT_SUPPORTED = 19;
			public const uint SP_QUERY_TOO_LONG = 20;
			public const uint SP_DRIVER_IS_BUSY = 30;
			public const uint SP_PORT_ALLOCATION_FAILURE = 31;
			public const uint SP_PORT_RELEASE_FAILURE = 32;
			public const uint SP_ACQUIRE_PORT_TIMEOUT = 39;
			public const uint SP_SIGNAL_NOT_SUPPORTED = 42;
			public const uint SP_INIT_NOT_CALLED = 57;
			public const uint SP_DRIVER_TYPE_NOT_SUPPORTED = 58;
			public const uint SP_FAIL_ON_DRIVER_COMM = 59;
			public const uint SP_SERVER_PROBABLY_NOT_UP = 60;
			public const uint SP_UNKNOWN_HOST = 61;
			public const uint SP_SENDTO_FAILED = 62;
			public const uint SP_SOCKET_CREATION_FAILED = 63;
			public const uint SP_NORESOURCES = 64;
			public const uint SP_BROADCAST_NOT_SUPPORTED = 65;
			public const uint SP_BAD_SERVER_MESSAGE = 66;
			public const uint SP_NO_SERVER_RUNNING = 67;
			public const uint SP_NO_NETWORK = 68;
			public const uint SP_NO_SERVER_RESPONSE = 69;
			public const uint SP_NO_LICENSE_AVAILABLE = 70;
			public const uint SP_INVALID_LICENSE = 71;
			public const uint SP_INVALID_OPERATION = 72;
			public const uint SP_BUFFER_TOO_SMALL = 73;
			public const uint SP_INTERNAL_ERROR = 74;
			public const uint SP_PACKET_ALREADY_INITIALIZED = 75;
			public const uint SP_INVALID_STATUS = 255;
		
			#endregion
		
			/* Error Codes used for custom HRESULTs. */
			private const uint EHR_UNKNOWN = ITF_LIC_SVR_OFFSET+1;
			private const uint EHR_DRIVER_INIT_FAILURE = ITF_LIC_SVR_OFFSET+2;
			private const uint EHR_PASSWORD_NO_MATCH = ITF_LIC_SVR_OFFSET+3;
			private const uint EHR_PASSWORD_INVALID_FORMAT = ITF_LIC_SVR_OFFSET+4;
			private const uint EHR_LICENSE_INSUFFICIENT = ITF_LIC_SVR_OFFSET+5;
			private const uint EHR_LICENSE_INVALID = ITF_LIC_SVR_OFFSET+6;
			private const uint EHR_LICENSE_EXPRIED = ITF_LIC_SVR_OFFSET+7;
			private const uint EHR_KEY_TRIAL_EXPIRED = ITF_LIC_SVR_OFFSET+8;
			private const uint EHR_KEY_NOT_PRESENT = ITF_LIC_SVR_OFFSET+9;
			private const uint EHR_CLOCK_ERROR = ITF_LIC_SVR_OFFSET+10;
			private const uint EHR_KEY_WRITE_FAILURE = ITF_LIC_SVR_OFFSET+11;
			private const uint EHR_KEY_WRITE_ACCESS_DENIED = ITF_LIC_SVR_OFFSET+12;
			private const uint EHR_KEY_NOT_ACTIVATED = ITF_LIC_SVR_OFFSET+13;
			private const uint EHR_AUTHENTICATION_FAILURE = ITF_LIC_SVR_OFFSET+14;
			private const uint EHR_MANAGER_NOT_INITIALIZED = ITF_LIC_SVR_OFFSET+15;
			private const uint EHR_KEY_NO_SUITABLE_KEY = ITF_LIC_SVR_OFFSET+16;
			private const uint EHR_KEY_LOCKED = ITF_LIC_SVR_OFFSET+17;
			private const uint EHR_CLIENT_TIMEOUT = ITF_LIC_SVR_OFFSET+18;
			private const uint EHR_PACKET_EXPIRED = ITF_LIC_SVR_OFFSET+19;
			private const uint EHR_KEY_BASE_NOT_MATCHING = ITF_LIC_SVR_OFFSET+20;
			private const uint EHR_KEY_NOT_MATCHING_INSTANCES = ITF_LIC_SVR_OFFSET+21;
			private const uint EHR_KEY_NO_FREE_APP_INSTANCE = ITF_LIC_SVR_OFFSET + 22;
			private const uint EHR_KEY_NO_BASE_KEY = ITF_LIC_SVR_OFFSET + 23;
			private const uint EHR_KEY_RESTORED_KEY_SERVER = ITF_LIC_SVR_OFFSET + 24;
			private const uint EHR_KEY_USE_BACKUP_KEY_SERVER = ITF_LIC_SVR_OFFSET + 25;
			private const uint EHR_KEY_NO_REMOTE_VERSION_KEY_SERVER = ITF_LIC_SVR_OFFSET + 26;
			
		
			// Rainbow driver errors
			private const uint EHR_SP_INVALID_FUNCTION_CODE = ITF_RNBO_OFFSET+SP_INVALID_FUNCTION_CODE;
			private const uint EHR_SP_INVALID_PACKET = ITF_RNBO_OFFSET+SP_INVALID_PACKET;
			private const uint EHR_SP_UNIT_NOT_FOUND = ITF_RNBO_OFFSET+SP_UNIT_NOT_FOUND;
			private const uint EHR_SP_ACCESS_DENIED = ITF_RNBO_OFFSET+SP_ACCESS_DENIED;
			private const uint EHR_SP_INVALID_MEMORY_ADDRESS = ITF_RNBO_OFFSET+SP_INVALID_MEMORY_ADDRESS;
			private const uint EHR_SP_INVALID_ACCESS_CODE = ITF_RNBO_OFFSET+SP_INVALID_ACCESS_CODE;
			private const uint EHR_SP_PORT_IS_BUSY = ITF_RNBO_OFFSET+SP_PORT_IS_BUSY;
			private const uint EHR_SP_WRITE_NOT_READY = ITF_RNBO_OFFSET+SP_WRITE_NOT_READY;
			private const uint EHR_SP_NO_PORT_FOUND = ITF_RNBO_OFFSET+SP_NO_PORT_FOUND;
			private const uint EHR_SP_ALREADY_ZERO = ITF_RNBO_OFFSET+SP_ALREADY_ZERO;
			private const uint EHR_SP_DRIVER_NOT_INSTALLED = ITF_RNBO_OFFSET+SP_DRIVER_NOT_INSTALLED;
			private const uint EHR_SP_VERSION_NOT_SUPPORTED = ITF_RNBO_OFFSET+SP_VERSION_NOT_SUPPORTED;
			private const uint EHR_SP_QUERY_TOO_LONG = ITF_RNBO_OFFSET+SP_QUERY_TOO_LONG;
			private const uint EHR_SP_DRIVER_IS_BUSY = ITF_RNBO_OFFSET+SP_DRIVER_IS_BUSY;
			private const uint EHR_SP_PORT_ALLOCATION_FAILURE = ITF_RNBO_OFFSET+SP_PORT_ALLOCATION_FAILURE;
			private const uint EHR_SP_PORT_RELEASE_FAILURE = ITF_RNBO_OFFSET+SP_PORT_RELEASE_FAILURE;
			private const uint EHR_SP_ACQUIRE_PORT_TIMEOUT = ITF_RNBO_OFFSET+SP_ACQUIRE_PORT_TIMEOUT;
			private const uint EHR_SP_SIGNAL_NOT_SUPPORTED = ITF_RNBO_OFFSET+SP_SIGNAL_NOT_SUPPORTED;
			private const uint EHR_SP_INIT_NOT_CALLED = ITF_RNBO_OFFSET+SP_INIT_NOT_CALLED;
			private const uint EHR_SP_FAIL_ON_DRIVER_COMM = ITF_RNBO_OFFSET+SP_FAIL_ON_DRIVER_COMM;
			private const uint EHR_SP_SERVER_PROBABLY_NOT_UP = ITF_RNBO_OFFSET+SP_SERVER_PROBABLY_NOT_UP;
			private const uint EHR_SP_UNKNOWN_HOST = ITF_RNBO_OFFSET+SP_UNKNOWN_HOST;
			private const uint EHR_SP_SENDTO_FAILED = ITF_RNBO_OFFSET+SP_SENDTO_FAILED;
			private const uint EHR_SP_SOCKET_CREATION_FAILED = ITF_RNBO_OFFSET+SP_SOCKET_CREATION_FAILED;
			private const uint EHR_SP_NORESOURCES = ITF_RNBO_OFFSET+SP_NORESOURCES;
			private const uint EHR_SP_BROADCAST_NOT_SUPPORTED = ITF_RNBO_OFFSET+SP_BROADCAST_NOT_SUPPORTED;
			private const uint EHR_SP_BAD_SERVER_MESSAGE = ITF_RNBO_OFFSET+SP_BAD_SERVER_MESSAGE;
			private const uint EHR_SP_NO_SERVER_RUNNING = ITF_RNBO_OFFSET+SP_NO_SERVER_RUNNING;
			private const uint EHR_SP_NO_NETWORK = ITF_RNBO_OFFSET+SP_NO_NETWORK;
			private const uint EHR_SP_NO_SERVER_RESPONSE = ITF_RNBO_OFFSET+SP_NO_SERVER_RESPONSE;
			private const uint EHR_SP_NO_LICENSE_AVAILABLE = ITF_RNBO_OFFSET+SP_NO_LICENSE_AVAILABLE;
			private const uint EHR_SP_INVALID_LICENSE = ITF_RNBO_OFFSET+SP_INVALID_LICENSE;
			private const uint EHR_SP_INVALID_OPERATION = ITF_RNBO_OFFSET+SP_INVALID_OPERATION;
			private const uint EHR_SP_BUFFER_TOO_SMALL = ITF_RNBO_OFFSET+SP_BUFFER_TOO_SMALL;
			private const uint EHR_SP_INTERNAL_ERROR = ITF_RNBO_OFFSET+SP_INTERNAL_ERROR;
			private const uint EHR_SP_PACKET_ALREADY_INITIALIZED = ITF_RNBO_OFFSET+SP_PACKET_ALREADY_INITIALIZED;
			private const uint EHR_SP_INVALID_STATUS = ITF_RNBO_OFFSET+SP_INVALID_STATUS;
			#endregion
			
			#region Defines for Custom HRESULTS Messages
		
			private static System.Collections.Hashtable SolLicErrors = null;
			static CSolLicHRESULT()
			{
				SolLicErrors = new System.Collections.Hashtable();
				SolLicErrors.Add(EHR_UNKNOWN,"Unknown error (Key server)");
				SolLicErrors.Add(EHR_DRIVER_INIT_FAILURE,"Driver initialization failure (Key server)");
				SolLicErrors.Add(EHR_PASSWORD_NO_MATCH,"Passwords do not match (Key server)");
				SolLicErrors.Add(EHR_PASSWORD_INVALID_FORMAT,"Invalid password format (Key server)");
				SolLicErrors.Add(EHR_LICENSE_INSUFFICIENT,"Insufficient license (Key server)");
				SolLicErrors.Add(EHR_LICENSE_INVALID,"Invalid license (Key server)");
				SolLicErrors.Add(EHR_LICENSE_EXPRIED,"License expired (Key server)");
				SolLicErrors.Add(EHR_KEY_TRIAL_EXPIRED,"Trial Key expired (Key server)");
				SolLicErrors.Add(EHR_KEY_NOT_PRESENT,"Key not present (Key server)");
				SolLicErrors.Add(EHR_CLOCK_ERROR,"Clock error (Key server)");
				SolLicErrors.Add(EHR_KEY_WRITE_FAILURE,"Failed writing to the key (Key server)");
				SolLicErrors.Add(EHR_KEY_WRITE_ACCESS_DENIED,"Failed writing to the key, access is denied (Key server)");
				SolLicErrors.Add(EHR_KEY_NOT_ACTIVATED,"Key not active (Key server)");
				SolLicErrors.Add(EHR_AUTHENTICATION_FAILURE,"Authentication failure (Key server)");
				SolLicErrors.Add(EHR_MANAGER_NOT_INITIALIZED,"Manager not initialized (Key server)");
				SolLicErrors.Add(EHR_KEY_NO_SUITABLE_KEY,"No suitable key found (Key server)");
				SolLicErrors.Add(EHR_KEY_LOCKED,"Key locked (Key server)");
				SolLicErrors.Add(EHR_CLIENT_TIMEOUT,"Client timeout (Key server)");
				SolLicErrors.Add(EHR_PACKET_EXPIRED,"Packet expired (Key server)");
				SolLicErrors.Add(EHR_KEY_BASE_NOT_MATCHING,"Base Keys do not match module for module (Key server)");
				SolLicErrors.Add(EHR_KEY_NOT_MATCHING_INSTANCES,"Keys do not match by Application Instances for all the Key on the server (Key server)");
				SolLicErrors.Add(EHR_KEY_NO_FREE_APP_INSTANCE, "Insufficient licensing, unable to obtain an Application Instances on the server (Key server)");
				SolLicErrors.Add(EHR_KEY_NO_BASE_KEY, "Insufficient licensing, unable to locate a Base Key on the server (Key server)");
				SolLicErrors.Add(EHR_KEY_RESTORED_KEY_SERVER, "Licensing on the Key Server has been restored (Key server)");
				SolLicErrors.Add(EHR_KEY_USE_BACKUP_KEY_SERVER, "Unable to validate Licensing on the Key Server, switching to Backup Key Server (Key server)");
				SolLicErrors.Add(EHR_KEY_NO_REMOTE_VERSION_KEY_SERVER, "License Server is at a version that does not support remote connectivity.  Please upgrade the License Server (Key server)");
				SolLicErrors.Add(EHR_SP_INVALID_FUNCTION_CODE,"Invalid function code (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_INVALID_PACKET,"Invalid packet (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_UNIT_NOT_FOUND,"Unit not found (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_ACCESS_DENIED,"Access is denied (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_INVALID_MEMORY_ADDRESS,"Invalid memory address (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_INVALID_ACCESS_CODE,"Invalid access code (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_PORT_IS_BUSY,"Port is busy (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_WRITE_NOT_READY,"Write not ready (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_NO_PORT_FOUND,"No parallel ports found (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_ALREADY_ZERO,"Counter already zero (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_DRIVER_NOT_INSTALLED,"System device driver not installed or detected (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_VERSION_NOT_SUPPORTED,"System device driver is outdated, update the driver (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_QUERY_TOO_LONG,"Query to long (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_DRIVER_IS_BUSY,"System driver is busy (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_PORT_ALLOCATION_FAILURE,"Failure to allocate parallel port (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_PORT_RELEASE_FAILURE,"Failure to release previously allocated parallel port (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_ACQUIRE_PORT_TIMEOUT,"Failure to aquire access to parallel port with time-out period (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_SIGNAL_NOT_SUPPORTED,"Machine does not support a signal line (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_INIT_NOT_CALLED,"Key is not initialized (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_FAIL_ON_DRIVER_COMM,"Client library failed on communication with the Rainbow system driver (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_SERVER_PROBABLY_NOT_UP,"The server is not responding, the client has timed out (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_UNKNOWN_HOST,"The server host is unknown; the server is not on the network or an invalid host name was specified (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_SENDTO_FAILED,"Unable to send a message to the server (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_SOCKET_CREATION_FAILED,"Unable able to open a network socket (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_NORESOURCES,"Insufficient resources (such as memory) to complete the request (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_BROADCAST_NOT_SUPPORTED,"Broadcast is not supported by the network interface on the system (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_BAD_SERVER_MESSAGE,"Could not understand a message received from the server (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_NO_SERVER_RUNNING,"Could not communicate with the server; verify the server is running (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_NO_NETWORK,"Unable to talk to the specified host; network communication problems encountered (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_NO_SERVER_RESPONSE,"No server responded to the client broadcast. There is no server running in the subnet (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_NO_LICENSE_AVAILABLE,"All licenses are currently in use (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_INVALID_LICENSE,"License is no longer valid (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_INVALID_OPERATION,"Specified operation cannot be performed (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_BUFFER_TOO_SMALL,"The size of the buffer is not sufficient to hold the expected data (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_INTERNAL_ERROR,"An internal error (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_PACKET_ALREADY_INITIALIZED,"The packet being initialized was already initialized (Rainbow driver)");
				SolLicErrors.Add(EHR_SP_INVALID_STATUS,"An invalid status code was returned (Rainbow driver)");
			}
			#endregion
			public static bool IsCustomHR(uint _hresult)
			{
				return (_hresult & (ITF_CR_MIN-ITF_MIN)) > 0;
			}
			public static string GetHRMessage(uint _hresult)
			{
				string ErrorMessage = "";
				if(SolLicErrors!=null && SolLicErrors.Contains(_hresult))
					ErrorMessage = (string)SolLicErrors[_hresult];
				return ErrorMessage;
			}
			public static bool IsEqual(int errorCode, uint hresult)
			{
				return ((uint)errorCode) == hresult;
			}
		}
	}
}
