
/**************************************************************************
* (C) Copyright 2009 SafeNet, Inc. All rights reserved.
*
* This software contains proprietary information, which shall not
* be reproduced or transferred to other documents and shall not
* be disclosed to others or used for manufacturing or any other
* purpose without prior written permission of SafeNet, Inc.
*
* !\AntiEmurate.h
*  \brief
*  Implementate the interface of  antiemulator.lib
*  \author	Simeon Zheng
*  \verison 1.0.0.1
*  \date    2009-09-02
*  Description: Header File of antiemurate.lib .
*  Purpose     - This module provides a way to detecte SENTEMUL2007.exe  
*                or other emulator software
**************************************************************************/
#ifndef _ANTIEMURATE_H_
#define _ANTIEMURATE_H_


#include <windows.h>

#define SP_IN
#define SP_OUT
#define SP_IO

/* Anti Emulator Library API Error Codes */

#define API_SUCCESS                      0
#define API_LOADLIBRARY_ERROR            1
#define API_CHECKSERVICE_ERROR           2
#define API_INTERNAL_ERROR               3


#ifdef __cplusplus
extern "C" {
#endif

/*++

Routine Description:

   This routine detect if there is emulator software installed. 

Arguments:

   bEmulator          -  Flags to show if there is emulator software characteristic checked in target machine.

   strEmulatorName    -  Emulator software Name to be checked, which can be viewed by Windows Device Management or WinObject tool. And it can be the below value:
      1)	Device Name   - such as xrnbo; it will check if the emulator software, which device name is "xrnbo" and can be viewed by WinObject tool, is running.
      2)	Service Name - such as "xrnbo"; it will check if the emulator software, which service name is "xrnbo" and can be viewed by Device Management or Registry,  is running
      3)	Default Value -- NULL; it checks the emulator software by the default device name or service name stored in library internal and the default emulator software characteristic.

   strExceptionServiceName - Customer's Service name which also depends on Sentinel Service.  And it can be below value:
      1) Default value - NULL; All the service which depends on "sentinel" service will be detected as emulator software.
      2) Exception Service Name - Such as "xrnbo"; If there is a service which is named as "xrnbo" and depends on "sentinel", it will be detected as emulator software. However because the "xrnbo" is passed by strExceptionSErviceName parameter, it will be removed from the emulator software list. 


Return Value:
   One of the error code will be returned:
      API_SUCCESS        -- Success
      API_LOADLIBRARY_ERROR -- Failed to load library
      API_CHECKSERVICE_ERROR -- Failed to check speical service name
      API_INTERNAL_ERROR -- There is an internal error happened

--*/
   extern  int __stdcall RNBOsproCheckEmulator(
      SP_OUT BOOL* bEmulator, 
      SP_IN char*  strEmulatorName, 
      SP_IN char*  strExceptionServiceName);

#ifdef __cplusplus
}
#endif

//Function ponter definition for dynamic library
typedef int (__stdcall* fnRNBOsproCheckEmulator)(
   SP_OUT BOOL* bEmulator, 
   SP_IN char*  strEmulatorName, 
   SP_IN char*  strExceptionServiceName);
#endif

