////////////////////////////////////////////////////////////////////////////////
//  File Name:  	NTService.h
//  Revision:   	1.05
//
//  Description:  	How to install, start, stop and remove services under NT
//
//  Authors:  		Mike Hegyi <mikeh@epicsys.com> - Original
//            		Stephan Hagedorn <shagedorn@dspace.de> - Revisions
//            		Alan F. Barksdale <afbarksd@ingr.com> - Extensions
//            		Giorgio Cognigni <giocogni@hotmail.com> - IS3 Support
//            		Troy Engel <tengel@abtcorp.com> - Rewrite, Extensions
//
//  Other:    		Ken Dyck <kdyck@go-dsp.com> - Reading his code helped ;-)
//            		Stefan Krueger <skrueger@installsite.org> - InstallSite guru
//  Modified: 		2000-05-30 by Stefan Krueger:
//            			- added #define for AddressString for IS6 compatibility  
//
//  Note:	    	_CreateNTService() parameters are rather complex, so it is
//	 				advised you read the MSDN/VC++ help for "CreateService".  
//					Read the function itself for the inline comments to help you.
////////////////////////////////////////////////////////////////////////////////  

#ifndef _NTSERVICE_H_
#define _NTSERVICE_H_

// Define this if you want to see debugging messages
// #define _NTS_DEBUG

// IS6 no longer supports AddressString
#if _ISCRIPT_VER >= 0x600
  #ifndef AddressString
    #define AddressString   &
  #endif
#endif

// constants copied from Winsvc.h
#define SC_MANAGER_ALL_ACCESS          0x000F003F
#define SERVICE_ALL_ACCESS             0x000F01FF

// Service Types (Bit Mask)
#define SERVICE_KERNEL_DRIVER          0x00000001
#define SERVICE_FILE_SYSTEM_DRIVER     0x00000002
#define SERVICE_ADAPTER                0x00000004
#define SERVICE_RECOGNIZER_DRIVER      0x00000008
#define SERVICE_DRIVER                 (SERVICE_KERNEL_DRIVER | SERVICE_FILE_SYSTEM_DRIVER | SERVICE_RECOGNIZER_DRIVER)
#define SERVICE_WIN32_OWN_PROCESS      0x00000010
#define SERVICE_WIN32_SHARE_PROCESS    0x00000020
#define SERVICE_WIN32                  (SERVICE_WIN32_OWN_PROCESS | SERVICE_WIN32_SHARE_PROCESS)
#define SERVICE_INTERACTIVE_PROCESS    0x00000100
#define SERVICE_TYPE_ALL               (SERVICE_WIN32  | SERVICE_ADAPTER | SERVICE_DRIVER  | SERVICE_INTERACTIVE_PROCESS)

// Start Type
#define SERVICE_BOOT_START             0x00000000
#define SERVICE_SYSTEM_START           0x00000001
#define SERVICE_AUTO_START             0x00000002
#define SERVICE_DEMAND_START           0x00000003
#define SERVICE_DISABLED               0x00000004

// Error control type
#define SERVICE_ERROR_IGNORE           0x00000000
#define SERVICE_ERROR_NORMAL           0x00000001
#define SERVICE_ERROR_SEVERE           0x00000002
#define SERVICE_ERROR_CRITICAL         0x00000003

// Control
#define SERVICE_CONTROL_STOP           0x00000001
#define SERVICE_CONTROL_PAUSE          0x00000002
#define SERVICE_CONTROL_CONTINUE       0x00000003
#define SERVICE_CONTROL_INTERROGATE    0x00000004
#define SERVICE_CONTROL_SHUTDOWN       0x00000005

// Status
#define SERVICE_STOPPED                0x00000001
#define SERVICE_START_PENDING          0x00000002
#define SERVICE_STOP_PENDING           0x00000003
#define SERVICE_RUNNING                0x00000004
#define SERVICE_CONTINUE_PENDING       0x00000005
#define SERVICE_PAUSE_PENDING          0x00000006
#define SERVICE_PAUSED                 0x00000007

typedef SERVICE_STATUS
begin
 NUMBER dwServiceType;
 NUMBER dwCurrentState;
 NUMBER dwControlsAccepted;
 NUMBER dwWin32ExitCode;
 NUMBER dwServiceSpecificExitCode;
 NUMBER dwCheckPoint;
 NUMBER dwWaitHint;
end;

// Prototype the service control APIs
prototype NUMBER AdvAPI32.OpenSCManagerA(POINTER, POINTER, NUMBER);
prototype NUMBER AdvAPI32.CreateServiceA(NUMBER, POINTER, POINTER, NUMBER, NUMBER, NUMBER, NUMBER, POINTER, POINTER, POINTER, POINTER, POINTER, POINTER);
prototype NUMBER Advapi32.OpenServiceA(NUMBER, POINTER, NUMBER);
prototype BOOL   Advapi32.StartServiceA(NUMBER, NUMBER, POINTER);
prototype BOOL   AdvAPI32.ControlService(NUMBER, NUMBER, POINTER);
prototype BOOL   AdvAPI32.CloseServiceHandle(NUMBER);
prototype BOOL   Advapi32.QueryServiceStatus(NUMBER, POINTER);
prototype BOOL   Advapi32.DeleteService(NUMBER);
prototype NUMBER KERNEL.GetLastError();

// global service handles, filled and released
POINTER ptrMcName, ptrDBName, pszSStartName;
NUMBER  schService, schSCManager;

// prototype the InstallScript functions.
prototype _PauseNTService(STRING);
prototype _ResumeNTService(STRING);
prototype _InitServiceConnections(STRING);
prototype _CloseServiceConnections();
prototype _IsServiceAvailable(STRING);
prototype _StartNTService(STRING);
prototype _StopNTService(STRING);
prototype _IsNTServiceStopped(STRING);
prototype _DeleteNTService(STRING);
prototype _CreateNTService(STRING, STRING,
                    NUMBER, NUMBER, NUMBER,
                    STRING, STRING, NUMBER,
                    STRING, STRING, STRING);

#endif // _NTSERVICE_H_


