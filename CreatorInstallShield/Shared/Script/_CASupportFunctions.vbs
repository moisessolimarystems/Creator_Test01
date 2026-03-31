'===================================================================================================
' Function    : DetectProductVersion
' Parameters  : NONE
' Description : Detects current version. If installed version is newer then the source version
'               prompts user that proceeding may break other dependent applications. If user
'               chooses not to proceed then return custom action failure to abort setup.
'		Place _CA in user interface sequence so it doesnt execute in silent installs which
'		products do for licensing.
'		Session object cant be used for silent installs since it maybe in a different context
'		since it is launched from another installer.
'===================================================================================================
Function DetectProductVersion()

    Dim regPath, var
    Dim productName, productVersion
    Set mShell = CreateObject("WScript.Shell")
    Set mFSO = CreateObject("Scripting.FileSystemObject")
    productName = Session.Property("ProductName")       
    productVersion = Session.Property("ProductVersion")
    
    Const vbYesNo = 4
    Const vbYes = 6
    Const vbNo = 7 
    Const IDABORT = 3
    
    On Error Resume Next 
    Err.Clear
    regPath = "HKEY_LOCAL_MACHINE\SOFTWARE\Solimar\Solimar Licensing\" & productName & "\CurrentVersion"
    nResult = mShell.RegRead(regPath)  
    'if no regread err then product is installed. Check for current version
    If Err.number = 0 Then
		If nResult > productVersion Then
			var = MsgBox("The version of " & productName & " currently installed is newer then the one found in this setup. " & _
				   "Uninstalling the current version and installing this setup version may break any applications dependent on it." & _
				   "Do you want to proceed?", vbYesno, "Warning")
			If var <> vbYes Then
				DetectProductVersion = IDABORT
			End If
		End If
    End If
	
    Set mShell = Nothing
    Set mFSO = Nothing

End Function


