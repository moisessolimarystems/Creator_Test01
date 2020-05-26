////////////////////////////////////////////////////////////////////////////////
//                                                                            
//  File Name:		Utility.h
//                                                                            
//  Description:	InstallShield script                                        
//                                                                            
//  Comments:		This file is to house support functionality.
//
//  Copyright:		Solimar Systems, Inc. 
//					Copyright 2003  
//
//  Author:			Jeff Lan
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _UTILITY_H_
#define WIN_NET_COMMAND "NET"
#define WIN_DISM_COMMAND "DISM"

prototype SelfRegister(STRING, STRING);
prototype SelfUnRegister(STRING, STRING);
prototype RegisterService(STRING, STRING);     
prototype UnRegisterService(STRING);  
prototype DisableService( STRING ); 
prototype RegAsmRegister(STRING, STRING, BOOL);
prototype RegAsmUnRegister(STRING, STRING, BOOL);   
prototype InstallPerformanceCounter(STRING, STRING);
prototype RemovePerformanceCounter(STRING, STRING);
prototype FindNReplaceInFile(STRING,STRING,STRING,STRING);
prototype FindReplaceAllFiles(STRING,STRING,STRING,STRING);
prototype RegSvcsRegister(STRING,STRING,STRING);
prototype RegSvcsUnRegister(STRING,STRING,STRING);
prototype BOOL IsMSMQEnabled();
prototype EnableMSMQ(STRING,STRING);
prototype DisableMSMQWorkgroup();
prototype NUMBER GetSharedFileCount(STRING,BOOL);
prototype EnableRemoteAccessCOMPlus();
prototype SetNetTCPPortSharing();
prototype EnableSystemRegistrySetting(STRING, STRING);
prototype BOOL AddStringToRegMultiSZ(NUMBER, STRING, STRING, STRING, BOOL);
prototype BOOL RemoveStringFromRegMultiSZ(NUMBER, STRING, STRING, STRING);
prototype AddUserToLocalGroupEx(STRING, STRING);
prototype BOOL IsGroupServiceAccount(STRING);

//Enable Windows Components
prototype EnableWebServerRole();

//Set DCOM Protocols
prototype AddDCOMProtocolTCPIP();
prototype BOOL AddTCPIPPortRange();

//Enable MSDTC settings
prototype EnableNetworkDtcAccess();
prototype EnableNetworkDtcAccessClients();
prototype EnableNetworkDtcAccessInbound();
prototype EnableNetworkDtcAccessOutbound();
prototype EnableNetworkDtcAccessTransactions();

//Setup Group Policy
prototype DisableForceUnloadPolicy();
#endif //_UTILITY_H_