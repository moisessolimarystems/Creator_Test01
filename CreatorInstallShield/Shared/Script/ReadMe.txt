HOW TO USE THIS SAMPLE CODE

 1. Extract the files from this .zip archive
 2. Start InstallShield and open your project
 3. Navigate to the Scripts tab in the InstallShield IDE
 4. Right click the top node "Your project name Script Files"
 5. Select "Insert Files" from the context menu
 6. Select File type "Script Files (*.rul)", browse to the 
    LaunchAppEx.rul file you extracted in step 1 and click Open.
 7. Repeat steps 4 and 5
 8. Select File type "Header Files (*.h)", browse to the 
    LaunchAppEx.h file you extracted in step 1 and click Open.
 9. At the top of your setup.rul file, below the other #include
    statements, add this line:
    #include "LaunchAppEx.h"
10. At the top of your setup.rul file, below the other #include
    statements (if any), add this line:
    #include "LaunchAppEx.rul"
11. At the point in your script where you want to launch a program,
    call the function _LaunchAppEx just like any built in function.
    

SYNTAX

_LaunchAppEx(szProgram, szCmdLine, nWait, nShow, nTimeOut, nvExitCode);


DESCRIPTION

The _LaunchAppEx function launches the application specified by 
szProgram with the command line parameters specified by szCmdLine. The 
third parameter, nWait, indicates whether or not the setup should wait 
until the launched application terminates before continuing. You can 
specify whether the launched program should run in a normal window or 
invisible, which is useful for running DOS commands or batch files 
without flashing up a DOS box. It also waits for programs that don't 
create a window. You can specify a time-out for the launched process 
to finish. The _LaunchAppEx function returns the result code of the 
launched program.


PARAMETERS

The _LaunchAppEx function takes six parameters:

szProgram
  Specifies the filename of the application to be launched. Specify 
  the complete path and filename of the application. Do not include 
  any command line parameters. If the fully qualified name of the 
  application includes long folder names and/or a long filename, pass 
  szProgram to LongPathToQuote before passing it to _LaunchAppEx.
  This parameter must point to an executable program. If you want to 
  launch a .bat file, please see the comments section below.
 
szCmdLine
  Specifies the command line parameters to pass to the launched 
  application. To launch the application with no command line 
  parameters, pass a null string ("").

nWait
  Specifies whether or not the setup should wait for the launched 
  application to terminate before continuing. Pass one of the 
  following predefined constants in this parameter:
  
  NOWAIT
    Specifies that the setup should continue immediately after 
    launching the application, which will run simultaneously with the 
    setup script. The function will not capture the exit code from the 
    launched application in this case.
  
  WAIT
    Specifies that the setup should wait until the application 
    launched by this function has terminated or the specified timeout 
    has elapsed.

nShow
  Specifies how the launched application should run. Pass one of the
  following constants in this parameter:
  
  SW_SHOWNORMAL
  Specifies that the application should run in a normal window
  
  SW_HIDE
  Specifies that the application should run without window. Use this 
  constant to launch a DOS program without the command window popping 
  up.
  
  Any other SW_* constant
  as defined in the Microsoft Windows Platform SDK
       
nTimeOut
  Specifies the time-out interval, in milliseconds. The function 
  returns if the interval elapses, even if the launched application 
  has not terminated. If nTimeOut is zero, the function returns 
  immediately. If nTimeOut is -1, the function's time-out interval 
  never elapses.

nvExitCode
  Returns the result code of the launched program.
  If NOWAIT was specified in the nWait parameter nvExitCode will 
  always return 0.
  If the function returns before the launched application is 
  finished (because the time-out interval has elapsed) the constant 
  STILL_ACTIVE is returned in  this parameter. STILL_ACTIVE is 
  defined as 0x103 in LaunchAppEx.h.
    

RETURN VALUES
  
  0
  Indicates that the application was launched successfully.
  
  -1
  Indicates that the application was not launched successfully.  
  

COMMENTS

  If you want to run a batch (.bat) file you must specify the 
  command processor (cmd.exe for Windows NT/2000 or command.com 
  for Windows 9x/Me) in the szProgram parameter. In the szCmdLine 
  parameter pass "/c " + the path to your .bat file. Sample for IS6:
    if ( SYSINFO.WIN9X.bWin9X ) then
        szProgram = "command.com";
    else
        szProgram = "cmd.exe";
    szParam = TARGETDIR ^ "YourBatch.bat"; 
    LongPathToQuote(szParam, TRUE);
    szParam = "/c " + szParam;
    ChangeDirectory(TARGETDIR);
    _LaunchAppEx(szProgram, szParam, WAIT, SW_HIDE, -1, nvExitCode);

  _LaunchAppEx uses the CreateProcess Windows API function to launch 
  the application. Then it calls the WaitForSingleObject Windows API 
  function to monitor the process handle of the launched application.
  This has several advantages:
  * Waiting doesn't consume too much CPU time.
  * The function can wait for programs that don't create a window.
  * You can specify a time-out in case the launched application hangs.

  Some applications attempt to load DLLs and may not run correctly if 
  those DLLs cannot be located. To ensure that an application can find 
  the DLLs it requires, it may be necessary to change to the directory 
  containing the application executable before calling _LaunchAppEx. 
  To change the current directory, call the ChangeDirectory function.

  The _LaunchAppEx function can monitor only the application specified 
  in szProgram; if that application goes on to launch other 
  applications or processes, the setup will be unable to monitor them. 
  For that reason, the setup will continue after the first application 
  ends even if additional applications launched by that first 
  application are still running.
      
  This code is designed to work with InstallShield Professional 
  versions 5 and 6, however 16 bit setups are not supported.


COPYRIGHT, LICENSE, CREDITS, DISCLAIMER

Copyright (c) 2000 by Dipl.-Ing. Stefan Krueger <skrueger@installsite.org>
You have the non-exclusive royalty-free right to use this code in your 
setup program. You are not allowed to sell, publish or redistribute this 
code in any form except as compiled part of your setup program.
This function is based on the HideAppAndWait script by Elihu Rozen 
and the LaunchAppGetExitCode script by Ed Smiley.
This sample code is provided "as is", without warrenty of any kind. It 
is completely up to you to verify that it works for your purpose. It 
is for informational purposes only.
