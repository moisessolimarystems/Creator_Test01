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

prototype SelfRegister(STRING, STRING);
prototype SelfUnRegister(STRING, STRING);
prototype RegisterService(STRING, STRING);     
prototype UnRegisterService(STRING);  
prototype DisableService( STRING ); 
prototype RegAsmRegister(STRING, STRING, BOOL);
prototype RegAsmUnRegister(STRING, STRING, BOOL);   
prototype InstallPerformanceCounter(STRING, STRING);
prototype RemovePerformanceCounter(STRING, STRING);

#endif //_UTILITY_H_