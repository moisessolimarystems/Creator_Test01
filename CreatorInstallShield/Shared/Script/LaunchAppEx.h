////////////////////////////////////////////////////////////////////////////////
//
//          File: LaunchAppEx.h                                          	
//                                                                    	
//	 Description: This file contains prototypes and definitions for the 
//                _LaunchAppEx function
//
// Last revision: November 29, 2000
//
//     Copyright: (c) 2000 by Dipl.-Ing. Stefan Krueger <skrueger@installsite.org>
//                You have the non-exclusive royalty-free right to use this code
//                in your setup prgram. You are not allowed to sell, publish or
//                redistribute this code in any form except as compiled part of
//                your setup program.
//
//         Notes: This function is based on the HideAppAndWait script by Elihu 
//                Rozen <Elihu@PSUAlum.com> and the LaunchAppGetExitCode script
//                by Ed Smiley <esmiley@cruzio.com>.
//                
////////////////////////////////////////////////////////////////////////////////

#ifndef _LAUNCHAPPEX_H
#define _LAUNCHAPPEX_H

#ifndef STILL_ACTIVE
	#define STILL_ACTIVE    0x00000103
#endif

#if _ISCRIPT_VER < 0x600
	#ifndef BYVAL
		#define BYVAL
	#endif
#endif

     prototype _LaunchAppEx(STRING, STRING, NUMBER, NUMBER, NUMBER, BYREF NUMBER);
     prototype SetProgressBarText(BYVAL STRING, BYVAL STRING, BYVAL STRING);
     prototype BOOL kernel32.CreateProcessA(POINTER, BYVAL STRING, POINTER, 
               POINTER, BOOL, NUMBER, POINTER, POINTER, POINTER, 
               POINTER);
     prototype BOOL kernel32.GetExitCodeProcess(NUMBER, POINTER);
     prototype NUMBER kernel32.WaitForSingleObject(NUMBER, NUMBER);
     prototype BOOL kernel32.CloseHandle(HWND);
     prototype BOOL User32.SetDlgItemText(HWND, INT, BYVAL STRING);
  
#endif
