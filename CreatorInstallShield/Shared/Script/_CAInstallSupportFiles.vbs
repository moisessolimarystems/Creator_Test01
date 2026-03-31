'===================================================================================================
' Function    : InstallSentinelDriver
' Parameters  : NONE
' Description : Installs the rainbow sentinel driver located in the source directory of the install.
'
'
'===================================================================================================
Function InstallSentinelDriver()

    Dim sentPath, sentOpts
    Dim sourceDir, nVersion
    Set mShell = CreateObject("WScript.Shell")
    Set mFSO = CreateObject("Scripting.FileSystemObject")
    sourceDir = Session.Property("SOURCEDIR")
    If mFSO.FileExists(sourceDir & "\SPNComboInstaller.exe") Then	
	 sentPath = mFSO.GetFile(sourceDir & "\SPNComboInstaller.exe")
    End If

    On Error Resume Next
    Err.Clear
    nResult = mShell.RegRead("HKEY_LOCAL_MACHINE\SOFTWARE\Rainbow Technologies\SuperPro Combo Installer\InstallDir")
    nVersion = InStr(nResult, "1.0.5")
    If (Err <> 0) Or (nVersion = 0) Then	
   	 If mFSO.FileExists(sentPath)  Then
            sentOpts = " /v" & Chr(34) & "/qb REBOOT=ReallySuppress REBOOTPROMPT=Suppress CONFIRMUPGRADE=TRUE" & Chr(34)
            mShell.Run Chr(34) & sentPath & Chr(34) & sentOpts, 1, True
	 End If
    End If
    Set mShell = Nothing
    Set mFSO = Nothing

End Function

'==================================================================================================
' Function    : InstallDotNet
' Parameters  : NONE
' Description : Searches for the .NET registry key to determine whether to install the .NET
'               package or not. The contents of the package are extracted into the source directory,
'               then run using the msiexec command. 
'
'
'==================================================================================================
Function InstallDotNet()

    Dim sentPath, sentOpts
    Dim sourceDir, tempDir
    Set mShell = CreateObject("WScript.Shell")
    Set mFSO = CreateObject("Scripting.FileSystemObject")
    On Error Resume Next
    Err.Clear

    Const vbOKOnly = 0
    nResult = mShell.RegRead("HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\.NETFramework\policy\v1.1\4322")
    If Err <> 0 Then
         sourceDir = Session.Property("SOURCEDIR")
	 tempDir = Session.Property("TempFolder")
         If mFSO.FolderExists(tempDir & "\tmpnet") Then
                mFSO.DeleteFolder (tempDir & "\tmpnet")
         End If
         sentPath = mFSO.GetFile(sourceDir & "\dotnetfx.exe")
         If mFSO.FileExists(sentPath) Then
                 sentOpts = " /t:" & tempDir & "tmpnet /c"	
                 mShell.Run Chr(34) & sentPath & Chr(34) & sentOpts, 1, True
                 sentPath = "msiexec"
                 sentOpts = " /i " & tempDir & "tmpnet\netfx.msi"
                 mShell.Run Chr(34) & sentPath & Chr(34) & sentOpts, 1, True
         End If
    End If
  
    nResult = mShell.RegRead("HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\.NETFramework\policy\v1.1\4322")	
    If Err <> 0 Then
	MsgBox "Microsoft .Net Framework was not installed. Components installed that are dependent on the framework " & _ 
	       "will not function without it. Please install the .NET Framework found in the installation setup directory " & _
	       "or from Microsoft.com", vbOKOnly, "Warning" 	
    End If
    Set mShell = Nothing
    Set mFSO = Nothing

End Function

'===================================================================================================
' Function    : _WriteToLogFile
' Parameters  : szMessage - string to write into the log file
' Description : Uses Session object to grab Installer object. Install object contains the function
'               to create a record to hold the log message. The record is passed to the msi created
'               Session object to write to the log file through the Message function.
'               Uses hex for msiMessageTypeInfo so it doesnt display to the user.
'===================================================================================================
Private Sub WriteToLogFile(szMessage)

  msiMessageTypeInfo = &H4000000   'if I ever want to add info messages
  msiMessageTypeError = &H1000000
  Set record = Session.Installer.CreateRecord(0)

  record.StringData(0) = STARS & szMessage
  Session.Message msiMessageTypeError, record
  
End Sub
