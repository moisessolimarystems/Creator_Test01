
Class CProductBuild
    Private mShell, mFSO, mStartTime, mProductName, mBuildLogFile, mNewDevPath, mNewNextVersion, mIgnoreMsgBox, mDebug, mLastBuild
    Private mBuildType, mPrevLabel, mProjPaths, mLeftOverSecs, mRelease, mBuildDialog, mExecInfo, mVersionNum
    Private mLockedPJFile, mLockedFile  'had to put these here instead of in MKS class b/c there should be only 1 instance of these
    Private mLogFileName, mInputFile
    Public mNewBuild, mErrCount, mBuildPath, mEngBuild

    Public Sub Class_Initialize() 'set default values
        Set mShell = wscript.CreateObject("Wscript.Shell")
        Set mFSO = wscript.CreateObject("Scripting.FileSystemObject")
        Set mBuildDialog = wscript.CreateObject("BuildDialog.BuildDialogB6", "BuildDiag_")
        Set mProjPaths = New CProjectPaths
        mDebug = "N"
        mEngBuild = "N"
        mNewBuild = Null
        mRelease = Null
        mStartTime = Time
 	mErrcount = 0
        
	Err.Clear
	If mFSO.FileExists("..\Done.txt") = True Then
            mFSO.DeleteFile "..\Done.txt", 1
        End If
        If mFSO.FileExists("..\Cancelled.txt") = True Then
            mFSO.DeleteFile "..\Cancelled.txt", 1
        End If
        mLeftOverTime = 0

	mBuildPath = mFSO.GetFolder(mShell.CurrentDirectory & "\\")
        mInputFile = "BuildRegistry.txt"
        
        If mFSO.FileExists("..\logFolder\StandardOutput.txt") Then
              mFSO.DeleteFile "..\logFolder\StandardOutput.txt", 1
        End If
        If mFSO.FileExists("..\logFolder\StandardError.txt") Then
              mFSO.DeleteFile "..\logFolder\StandardError.txt", 1
        End If

        mFSO.CreateTextFile (mBuildPath & "\logFolder\StandardOutput.txt")
        mFSO.CreateTextFile (mBuildPath & "\logFolder\StandardError.txt")
        If Err.Number <> 0 Then
               	BuildError "Unable to create standard output/error files : " + Err.Description
        End If 

    End Sub


    Public Sub SetMembers(aAveBuildTime, aProductName)
        mProductName = aProductName
        mBuildDialog.DisplayCancelDialog "Time Started: " & Time, CInt(aAveBuildTime), CStr(mProductName)
        Err.Clear
	If Not mFSO.FolderExists(mBuildPath + "\logFolder") Then
            mFSO.CreateFolder mBuildPath + "\logFolder"
        End If
        Dim CurDate, CurTime
        CurDate = Replace(Date, "/", "-")
        CurTime = Replace(Time, ":", "")
        mLogFileName = mBuildPath & "\logFolder\buildLog.txt"
        
        Set mBuildLogFile = mFSO.CreateTextFile(mLogFileName)
        If Err.Number <> 0 Then
               	BuildError "Unable to create build log file : " + Err.Description
        End If 
    End Sub


    '***************************************************************************************************
    '* -BT    mBuildType          NB (New build), PB (Previous build), R (Release), CB (Create Branch)
    '* -MPP   mMainProjectPath    Path of main sandbox
    '* -CBT   mCodeBuildType      FB (Full Build), MK (Make)
    '* -VN    mVersionNum         Version Number To build
    '* -NNV   mNewNextVersion     What the new next version will become
    '* -PL    mPrevLabel          Label to generate build from
    '* -NDP   mNewDevPath         New development path to create
    '* -EDP   mExistingDevPath    Existing Development Path to use
    '* -SE    mIgnoreMsgBox       Y or N for suppressing all messageboxes
    '* -DB    mDebug              Y or N switch for debug in product scripts
    '* -LB    mLastBuild          
    '* -LIC   licenseVersion      version of solimar licensing to use 
    '* -EB    mEngBuild           Y or N Engineering Build
    '***************************************************************************************************
    Public Sub GetParams()

        Dim iParam, CodeBuildType, ExistingDevPath, licenseVersion
         WriteToDlgAndFile "Params passed into the script: "
            For iParam = 0 To wscript.Arguments.Count - 1 Step 2
            WriteToDlgAndFile "    " + wscript.Arguments(iParam) + " " + wscript.Arguments(iParam + 1)

               Select Case wscript.Arguments(iParam)
                    Case "-BT"
				        mBuildType = wscript.Arguments(iParam + 1)
                    Case "-MPP"
				        mProjPaths.SetMainProjPath wscript.Arguments(iParam + 1)
                    Case "-PL"
				        mPrevLabel = wscript.Arguments(iParam + 1)
                    Case "-NNV"
					mNewNextVersion = wscript.Arguments(iParam + 1)
                    Case "-NDP"
					mNewDevPath = wscript.Arguments(iParam + 1)
                    Case "-CBT"
					CodeBuildType = wscript.Arguments(iParam + 1)
                    Case "-EDP"
				        ExistingDevPath = wscript.Arguments(iParam + 1)
                    Case "-VN"
					mVersionNum = wscript.Arguments(iParam + 1)
                    Case "-SE"
					mIgnoreMsgBox = wscript.Arguments(iParam + 1)
                    Case "-DB"
				        mDebug = wscript.Arguments(iParam + 1)
                    Case "-LB"
					mLastBuild = wscript.Arguments(iParam + 1)
		    Case "-LIC"
					licenseVersion = wscript.Arguments(iParam + 1)	
		    Case "-EB"	
					mEngBuild = wscript.Arguments(iParam + 1)
               End Select
            Next

        If mBuildType = "NB" Then 'doing a new build
            Set mNewBuild = New CNewBuild
            Set mNewBuild.Build = Me
            mNewBuild.mCodeBuildType = CodeBuildType
            mNewBuild.mExistingDevPath = ExistingDevPath   'This may not have been set
            mNewBuild.mVersionNum = mVersionNum
            mNewBuild.mLicenseVersion = StripPrefix(licenseVersion, "SolimarLicensing")
            
        ElseIf mBuildType = "R" Then  'doing a release
            Set mRelease = New CRelease
            Set mRelease.Build = Me             'Me is vbscript's this equivalent
        End If

        CheckParams     'check to make sure all passed params were valid
    End Sub
 
    Private Sub CheckParams()
        Dim bWasError
        bWasError = False
        If mBuildType <> "NB" And mBuildType <> "PB" And mBuildType <> "R" And mBuildType <> "CB" Then
            bWasError = True
            WriteToDlgAndFile "ERROR: A valid value for parameter -BT (Build Type) was not passed in. " + _
                              "Valid values are: NB for a New Build, PB for a Previous Build, R for a Release, and CB for Create Branch"
        End If
        If mBuildType = "PB" And mPrevLabel = "" Then
            bWasError = True
            WriteToDlgAndFile "ERROR: You are trying to do a build of a previous version but did not pass in -PL (Previous Label)"
        End If
        If mProjPaths.mMainProjPath = "" Then
            bWasError = True
            WriteToDlgAndFile "ERROR: No value was given for parameter -MPP (MainProjectPath).  Please pass in the sandbox path"
        End If
        If mBuildType = "CB" And (mNewDevPath = "" Or mNewNextVersion = "" Or mPrevLabel = "") Then  'must have both parameters
            bWasError = True
            WriteToDlgAndFile "ERROR: You are creating a branch so you must pass in values for -PL (Previous Label), " + _
                              "-NNV (New Next Version), and -NDP (New Development Path)"
        End If

        Dim bBadNewBuildParams, bBadReleaseParams
        bBadNewBuildParams = False
        bBadReleaseParams = False
        If Not IsNull(mNewBuild) Then
            bBadNewBuildParams = mNewBuild.CheckParams
        End If
        If Not IsNull(mRelease) Then
            bBadReleaseParams = mRelease.CheckParams
        End If

        If bWasError Or bBadNewBuildParams Or bBadReleaseParams Then
            BuildError "One or more of the parameters passed into the script was missing or invalid."
        End If
    End Sub


    Function SetProjPathsVars(aCode, aISourcePath, aIShieldPath,aMainPJ, aMainSrcPath, aCodePJ, aMainSourcePJ, aImageDir, aMainSrcRCSPJ, aISourcePJ, aSourceISourcePJ) ', aISourceBuildPJ
        'Pull this information from a file instead?
        mProjPaths.SetCodePath aCode 
        mProjPaths.SetInstallSourcePath aISourcePath
        mProjPaths.SetInstallShieldPath aIShieldPath
        mProjPaths.SetMainPJFile aMainPJ
        mProjPaths.SetMainSrcPath aMainSrcPath
        mProjPaths.SetMainCodePJFile aCodePJ
        mProjPaths.SetMainSourcePJFile aMainSourcePJ
        mProjPaths.SetImageDirPath aImageDir
        mProjPaths.SetMainSourceRCSPJFile aMainSrcRCSPJ
        mProjPaths.SetInstallSourcePJFile aISourcePJ
        mProjPaths.SetSourceInstallSourcePJFile aSourceISourcePJ '%%%I think this is right, iconvert used the rcs dir instead though
    End Function

    'Accessor Methods

    
  
    Function ProductDebug()
         ProductDebug = mDebug
    End Function
        
    Function LastBuild()
        LastBuild = mLastBuild
    End Function

    Public Property Get FSO()
            Set FSO = mFSO
    End Property

    Public Property Get Shell()
            Set Shell = mShell
        End Property

    Public Property Get BuildDialog()
            Set BuildDialog = mBuildDialog
        End Property

    Public Property Get ExecInfo()
            Set ExecInfo = mExecInfo
        End Property

    Public Property Get ProductName()
            ProductName = mProductName
        End Property

    Public Property Get BuildType()
            BuildType = mBuildType
    End Property

    Public Property Get EngOption()
	EngOption = mEngBuild
    End Property

    Public Property Get PrevLabel()
            PrevLabel = mPrevLabel
        End Property

    Public Property Get NewDevPath()
        NewDevPath = mNewDevPath
     End Property

    Public Property Get NewNextVersion()
        NewNextVersion = mNewNextVersion
    End Property

    Public Property Get startTime()
            startTime = mStartTime
    End Property

    Public Property Get LockedPJFile()
        LockedPJFile = mLockedPJFile
    End Property

    Public Property Get LockedFile()
        LockedFile = mLockedFile
    End Property

    Public Property Get MainSrcPath()
		MainSrcPath = mProjPaths.mMainSrcPath
	End Property
	
    Public Property Get MainProjPath()
        MainProjPath = mProjPaths.mMainProjPath
    End Property

    Public Property Get MainCodePath()
        MainCodePath = mProjPaths.mMainProjPath + "\" + mProjPaths.mMainCodePath
    End Property
    
    'temporary hack until move into xml 
    Public Property Get MainCodePJFile()
		MainCodePJFile = mProjPaths.mMainProjPath + "\" + mProjPaths.mMainCodePath + "\" + mProjPaths.mMainCodePJFile
    End Property 

    Public Property Get InstallSrcPath()
        InstallSrcPath = mProjPaths.mMainProjPath + "\" + mProjPaths.mInstallSourcePath
    End Property

    Public Property Get InstallShieldProjPath()
       InstallShieldProjPath = mProjPaths.mMainProjPath + "\" + mProjPaths.mInstallShieldProjPath
    End Property

    Public Property Get MainPJFile()
       MainPJFile = mProjPaths.mMainProjPath + "\" + mProjPaths.mMainPJFile
    End Property

    Public Property Get MainSrcPJFile()
       MainSrcPJFile = mProjPaths.mMainSourcePJFile
    End Property

    Public Property Get InstallSrcBuildPJ
       InstallSrcBuildPJ = mProjPaths.mInstallSourceBuildPJ
    End Property

    Public Property Get ImageDirPath()
       ImageDirPath = mProjPaths.mMainProjPath + "\" + mProjPaths.mImageDirPath
    End Property

    Public Property Get MainSrcRCSPJFile()
       MainSrcRCSPJFile = mProjPaths.mMainSourceRCSPJFile
    End Property

    Public Property Get InstallSrcPJFile()
       InstallSrcPJFile = mProjPaths.mMainProjPath + "\" + mProjPaths.mInstallSourcePJFile
    End Property

    Public Property Get SrcInstallSrcPJ()
        SrcInstallSrcPJ = mProjPaths.mSourceInstallSourcePJ
    End Property

    Public Property Get NewBuild()
            If IsNull(mNewBuild) Then
                    BuildError "Internal Error in CProductBuild::Get NewBuild.  mNewBuild is empty when trying to access it"
                End If
                Set NewBuild = mNewBuild
    End Property

    Public Property Get Release()
        If IsNull(mRelease) Then
                BuildError "Internal Error in CProductBuild::Get Release.  mRelease is empty when trying to access it"
        End If
        Set Release = mRelease
    End Property

    Public Property Let ImageDirPath(aPath)
            mProjPaths.SetImageDirPath aPath
        End Property

    Public Property Let LockedPJFile(aFile)
            mLockedPJFile = aFile
        End Property

    Public Property Let LockedFile(aFile)
            mLockedFile = aFile
        End Property

    Public Function WriteToDlgAndFile(Display)
        If Display <> "" Then
                Dim diagDisplay
                    diagDisplay = CStr(Time & " : " & Display)
                If Not IsEmpty(mBuildDialog) Then
                        mBuildDialog.UpdateDisplay CStr(diagDisplay)
                    End If
                    wscript.echo diagDisplay
            mBuildLogFile.writeLine diagDisplay
            End If
    End Function


    'When running a command using Exec, the object returned can give misleading results for executables.
        'Some exes (InstallShield for example)
        'return information to StdErr when the command was actually successful and some return no information to
        'StdErr when the command was unsuccessful so looking a the StdErr by itself to determine if an error
        'occurred doesn't work.  The exitcode seems to return a non-zero result when an error occurs and 0 otherwise
        'so that is now used to determine the error status.  This hasn't been tested for all cases however and
        'may not work correctly for some.  For bat files, the bat file must exit with an error value (if an error occurs)
        'in order for the exitcode to be set correctly.
    Function RunCmd(CmdLine)
        WriteToDlgAndFile "     Running Command: " & CmdLine

        Dim ErrorStr, CmdStr, TimeStarted, TimeEnded
        TimeStarted = Time
        Dim ExitCode

        Set mExecInfo = mShell.Exec(CmdLine)
        Do: wscript.Sleep 10
                        CmdStr = CmdStr & mExecInfo.StdOut.ReadAll
                        wscript.echo CmdStr
                        ErrorStr = ErrorStr & mExecInfo.StdErr.ReadAll
        Loop While mExecInfo.Status = 0 And Not mExecInfo.StdOut.AtEndOfStream And Not mExecInfo.StdErr.AtEndOfStream
        ExitCode = mExecInfo.ExitCode
        If InStr(CmdLine, "bcw -b") <> 0 Or InStr(CmdLine, "bcw -m") <> 0 Then
                        Dim RevisedCmd
                        RevisedCmd = Replace(CmdLine, "-b", "-m") 'we're going to make it since it just built so its fast (we just want the exit code)
                        ExitCode = mShell.Run(RevisedCmd, 1, True) 'have to run it with the shell to get the correct exit code for this cmd
        End If
    
            WriteToDlgAndFile "    ExitCode was: " + CStr(ExitCode)
        TimeEnded = Time
        Dim minsPassed
        minsPassed = GetMinsPassed(TimeStarted, TimeEnded)
        mBuildDialog.AddProgress CInt(minsPassed)
        
        WriteCmdOutputToFile "logFolder\StandardOutput.txt", CmdLine, CmdStr
        WriteCmdOutputToFile "logFolder\StandardError.txt", CmdLine, ErrorStr
        CheckForCmdErrors CmdLine, ExitCode
        RunCmd = CmdStr
    End Function


    Private Function WriteCmdOutputToFile(aFile, aCmdLine, aOutput)
        If aOutput <> "" Then
                        Dim temp
                        Set temp = mFSO.OpenTextFile(aFile, 8, True)      'Append to the file
                        temp.writeLine "----------------------------------------------------------------------------------"
                        temp.writeLine "COMMAND: " + aCmdLine
                        temp.writeLine "OUTPUT: "
                        temp.Write aOutput
                        temp.Close
                End If
        End Function


    Private Function CheckForCmdErrors(aCmdLine, aExitCode)
            Dim HasError
                HasError = False
                If aExitCode <> 0 Then
                    HasError = True
                End If
            If aExitCode = 226 And InStr(aCmdLine, "pj sandbox -S") <> 0 Then 'special condition(falsely errors out - double check this though)
                    HasError = False
            End If
            If aExitCode = 1 And InStr(aCmdLine, "dirsizes.exe") <> 0 Then
                    HasError = False
            End If
	    If aExitCode = 1 And InStr(aCmdLine, "devenv") <> 0 Then
		HasError = False
	    End If
        If HasError Then
            BuildError "Error running command: " + aCmdLine & vbNewLine + "Refer to logFolder\StandardOutput.txt " + _
                       "and logFolder\StandardError.txt to see the output from the command that failed. Exit code: " + CStr(aExitCode)
        End If
        End Function


    ' The progress bars that keeps track of time passed only takes integers so have to keep track of missed secs so its more accurate
    Function GetMinsPassed(aTimeStarted, aTimeEnded)
        Dim totalSecsPassed, totalMinsPassed

        totalSecsPassed = (aTimeEnded - aTimeStarted) * 24 * 60 * 60
        totalSecsPassed = totalSecsPassed + mLeftOverSecs
        mLeftOverSecs = totalSecsPassed Mod 60
        totalMinsPassed = (totalSecsPassed - mLeftOverSecs) / 60
        GetMinsPassed = totalMinsPassed Mod 60
    End Function


    Sub NetSendMsg(aMsg)
        If mIgnoreMsgBox <> "Y" Or mDebug <> "Y" Then
            NetSendArry = Array("achou", "DKBZC11")
            For i = 0 To UBound(NetSendArry)
                   mShell.Run "net send " & NetSendArry(i) + " " + aMsg
            Next
        End If
    End Sub

        Function ReadRegistry(aPath)
		   Err.Clear
            On Error Resume Next
        Dim Value
                Value = mShell.RegRead(aPath)
                If Err.Number <> 0 Then
                    BuildError "Error reading registry path: " + aPath + ". " + Err.Description
                End If
		   ReadRegistry = Value
        End Function

        'fix checkPath later so all projects can use it. -emester
    'Public Function CheckPath(path)
            
    '    If (mFSO.FolderExists(path) = False) Then
        '       CheckPath GetUpperDirPath(path)
        '       mFSO.CreateFolder path
        ' End If
            
    'End Function


        Function CreateNewFolder(Folder)
		Err.Clear

                'CheckPath (Folder)  '%%%% are we supposed to do this or not???
                If Not mFSO.FolderExists(Folder) Then
                        WriteToDlgAndFile "Creating folder: " + Folder
                        Err.Clear
                        mFSO.CreateFolder Folder
                        On Error Resume Next
                        If Err.Number <> 0 Then
                            BuildError "Unable to create the folder: " + Folder + ". " + Err.Description
                        End If
                End If
	        If Err.Number <> 0 Then
               		BuildError "Unable to create new folder : " + Folder + " : " + Err.Description
        	End If 

        End Function

        Function getHrMinSec(startTime, endTime)

                'Function is done in this longer manner so that rounding only occurs at seconds
                Dim totalSecsPassed, secsPassed, totalMinsPassed, minsPassed, hrsPassed
                totalSecsPassed = (endTime - startTime) * 24 * 60 * 60
                secsPassed = totalSecsPassed Mod 60
                totalMinsPassed = (totalSecsPassed - secsPassed) / 60
                minsPassed = totalMinsPassed Mod 60
                hrsPassed = CInt((totalMinsPassed - minsPassed) / 60)
                getHrMinSec = CStr(hrsPassed & " Hour(s), " & minsPassed & " Mins, " & secsPassed & " Secs")

        End Function

        'returns 1 if the version is changed
        'returns 0 if the version is the same version that is already there
        Function UpdateFileVersion(fileInput, pattern, Version)
                WriteToDlgAndFile "Updating File Version: " & fileInput
                Dim output, readFile, writeFile
                output = "temp"
                Err.Clear
                Set readFile = mFSO.OpenTextFile(fileInput)
                Set writeFile = mFSO.CreateTextFile(output)
                UpdateFileVersion = 0
                Dim check, Line
                check = 0
                Do While Not readFile.AtEndOfStream
                        Line = readFile.ReadLine
                        If InStr(Line, pattern) <> 0 And check = 0 Then
                                If InStr(Version, Right(Line, Len(Line) - Len(pattern))) = 0 Then
                                        UpdateFileVersion = 1
                                End If
                                check = 1
                                writeFile.writeLine pattern & Version
                        Else
                                writeFile.writeLine Line
                        End If
                Loop
                writeFile.Close
                readFile.Close
                mFSO.DeleteFile fileInput, 1
                mFSO.MoveFile output, fileInput
		If Err.Number <> 0 Then
                    BuildError "Unable to update file version for: " + fileInput + " : " + Err.Description
                End If

        End Function

     '%%%iconvert doesn't use.  get rid of if other projects don't use either.
        Function UpdateIniFile(inputFile, section, patternKey, Version)

                WriteToDlgAndFile "Updating File: " & inputFile
                Dim output, bRightSection, readFile, writeFile
                output = "temp"
                Err.Clear
                bRightSection = False
                Set readFile = mFSO.OpenTextFile(inputFile)
                Set writeFile = mFSO.CreateTextFile(output)
                UpdateIniFile = 0
                Dim Line
                Do While Not readFile.AtEndOfStream
                        Line = readFile.ReadLine
                        If InStr(Line, "[") = 1 Then
                                If InStr(Line, section) <> 0 Then
                                        bRightSection = True
                                Else
                                        bRightSection = False
                                End If
                        End If
                        If InStr(Line, patternKey) <> 0 And bRightSection = True Then
                                If InStr(Version, Right(Line, Len(Line) - Len(patternKey))) = 0 Then
                                        UpdateIniFile = 1
                                End If
                                writeFile.writeLine patternKey & Version
                        Else
                                writeFile.writeLine Line
                        End If
                Loop
                writeFile.Close
                readFile.Close
                mFSO.DeleteFile inputFile, 1
                mFSO.MoveFile output, inputFile
		If Err.Number <> 0 Then
                    BuildError "Unable to update file for: " + fileInput + " with " + inputFile + " : " + Err.Description
                End If

        End Function

    Function UpdateFile(inputFile, patternKey, newPattern)

                RunCmd "attrib -r " & inputFile
                WriteToDlgAndFile "Updating File: " & inputFile
                Dim output, readFile, writeFile
                output = "temp"
                Err.Clear
                Set readFile = mFSO.OpenTextFile(inputFile)
                Set writeFile = mFSO.CreateTextFile(output)
                Dim Line
                Do While Not readFile.AtEndOfStream
                        Line = RTrim(readFile.ReadLine)
                        If InStr(Line, patternKey) <> 0 Then
                            writeFile.writeLine Left(Line, (InStr(Line,patternKey) + Len(patternKey))-1) & newPattern
                        Else
                            writeFile.writeLine Line
                        End If
                Loop
                writeFile.Close
                readFile.Close
                mFSO.DeleteFile inputFile, 1
                mFSO.MoveFile output, inputFile
		If Err.Number <> 0 Then
                    BuildError "Unable to update file for: " + inputFile + " with " + newPattern + " : " + Err.Description
                End If                

        End Function
 
     'currently only used for Spd builds
     Function GetAndFormatRCVersionNum(inputFile, patternKey)

                Dim output, readFile, versionNum
                Set readFile = mFSO.OpenTextFile(inputFile)
                Dim Line
                Do While Not readFile.AtEndOfStream
                        Line = RTrim(readFile.ReadLine)
                        If InStr(Line, patternKey) <> 0 Then
                            pos = InStr(Line, ",")
                            versionNum = Mid(Line, (pos - 2), 8)
                            Exit Do
                        End If
                Loop
                readFile.Close
                GetAndFormatRCVersionNum = Replace(versionNum, ",", "")

    End Function

    Function UpdateFileVersion2(aFilePath, aPattern, aNewVersion)

                WriteToDlgAndFile "Updating File Version: " & fileInput
                Dim output, readFile, writeFile, FileText, ReplacedText
                output = "temp"
                Err.Clear
                Set readFile = mFSO.OpenTextFile(aFilePath)
                Set writeFile = mFSO.CreateTextFile(output)
				FileText = readFile.ReadAll
				ReplacedText = Replace(FileText, aPattern, aPattern + " " + aNewVersion)

				writeFile.Write ReplacedText
                writeFile.Close
                readFile.Close
                mFSO.DeleteFile aFilePath, 1
                mFSO.MoveFile output, aFilePath
		If Err.Number <> 0 Then
                    BuildError "Unable to update file for: " + aFilePath + " with " + aPattern + " : " + Err.Description
                End If   

        End Function


      '%%% iConvert doesn't use this function.  see if other projects use.  if not take out. if so need to modify it so its correct.
        Function UpdatePathInFile(inputFile, section, patternKey, oldPathSection, newPathSection)

                Dim output, bRightSection, readFile, writeFile
                output = "temp"
                Err.Clear
                bRightSection = False
                Set readFile = mFSO.OpenTextFile(inputFile)
                Set writeFile = mFSO.CreateTextFile(output)
                Dim Line
                Do While Not readFile.AtEndOfStream
                        Line = readFile.ReadLine
                        If InStr(Line, "[") = 1 Then
                                If InStr(Line, section) <> 0 Then
                                        bRightSection = True
                                Else
                                        bRightSection = False
                                End If
                        End If
                        If InStr(Line, patternKey) <> 0 And InStr(Line, oldPathSection) And bRightSection = True Then
                                Dim occursAt
                                occursAt = InStr(1, Line, oldPathSection)
                                writeFile.writeLine Mid(Line, 1, occursAt - 1) & newPathSection & Mid(Line, occursAt + Len(oldPathSection))
                        Else
                                writeFile.writeLine Line
                        End If
                Loop
                writeFile.Close
                readFile.Close
                mFSO.DeleteFile inputFile, 1
                mFSO.MoveFile output, inputFile
		If Err.Number <> 0 Then
                    BuildError "Unable to update file for: " + inputFile + " with " + patternKey + " : " + Err.Description
                End If   

        End Function


        Function RemoveExcessFiles()
                RunCmd ("attrib -r " & InstallSrcPath & "\*.* /S")
                WriteToDlgAndFile "Removing PJ Files from: " & InstallSrcPath
                RunCmd "rm -R -f " & Chr(34) & InstallSrcPath & Chr(34) & "\*.pj"
                RunCmd "rm -R -f " & Chr(34) & InstallSrcPath & Chr(34) & "\*\*.pj"
                RunCmd "rm -R -f " & Chr(34) & InstallSrcPath & Chr(34) & "\*\*\*.pj"
                RunCmd "rm -R -f " & Chr(34) & InstallSrcPath & Chr(34) & "\*\*\*\*.pj"
                RunCmd "rm -R -f " & Chr(34) & InstallSrcPath & Chr(34) & "\*\*\*\*\*.pj"
                RunCmd "rm -R -f " & Chr(34) & InstallSrcPath & Chr(34) & "\*\*\*\*\*\*.pj"
                RunCmd "rm -R -f " & Chr(34) & InstallSrcPath & Chr(34) & "\*\*\*\*\*\*\*.pj"
                RunCmd "rm -R -f " & Chr(34) & InstallSrcPath & Chr(34) & "\*\*\*\*\*\*\*\*.pj"
                RunCmd "rm -R -f " & Chr(34) & InstallSrcPath & Chr(34) & "\*\*\*\*\*\*\*\*\*.pj"
                WriteToDlgAndFile "Removing USE Files from: " & InstallSrcPath
                RunCmd "rm -R -f " & Chr(34) & InstallSrcPath & Chr(34) & "\*.use"
                RunCmd "rm -R -f " & Chr(34) & InstallSrcPath & Chr(34) & "\*\*.use"
                RunCmd "rm -R -f " & Chr(34) & InstallSrcPath & Chr(34) & "\*\*\*.use"
                RunCmd "rm -R -f " & Chr(34) & InstallSrcPath & Chr(34) & "\*\*\*\*.use"
                RunCmd "rm -R -f " & Chr(34) & InstallSrcPath & Chr(34) & "\*\*\*\*\*.use"
                RunCmd "rm -R -f " & Chr(34) & InstallSrcPath & Chr(34) & "\*\*\*\*\*\*.use"
                RunCmd "rm -R -f " & Chr(34) & InstallSrcPath & Chr(34) & "\*\*\*\*\*\*\*.use"
                RunCmd "rm -R -f " & Chr(34) & InstallSrcPath & Chr(34) & "\*\*\*\*\*\*\*\*.use"
                RunCmd "rm -R -f " & Chr(34) & InstallSrcPath & Chr(34) & "\*\*\*\*\*\*\*\*\*.use"
                'WriteToDlgAndFile "Removing Lib Files from: " & InstallSrcPath
                'RunCmd "rm -R -f " & Chr(34) & InstallSrcPath & Chr(34) & "\*.lib"
                'RunCmd "rm -R -f " & Chr(34) & InstallSrcPath & Chr(34) & "\*\*.lib"
                'RunCmd "rm -R -f " & Chr(34) & InstallSrcPath & Chr(34) & "\*\*\*.lib"
                'RunCmd "rm -R -f " & Chr(34) & InstallSrcPath & Chr(34) & "\*\*\*\*.lib"
                'RunCmd "rm -R -f " & Chr(34) & InstallSrcPath & Chr(34) & "\*\*\*\*\*.lib"
                'RunCmd "rm -R -f " & Chr(34) & InstallSrcPath & Chr(34) & "\*\*\*\*\*\*.lib"
                'RunCmd "rm -R -f " & Chr(34) & InstallSrcPath & Chr(34) & "\*\*\*\*\*\*\*.lib"
                'RunCmd "rm -R -f " & Chr(34) & InstallSrcPath & Chr(34) & "\*\*\*\*\*\*\*\*.lib"
                'RunCmd "rm -R -f " & Chr(34) & InstallSrcPath & Chr(34) & "\*\*\*\*\*\*\*\*\*.lib"
		WriteToDlgAndFile "Removing PJ Files from: " & ImageDirPath
		RunCmd "rm -R -f " & Chr(34) & ImageDirPath & Chr(34) & "\*.pj"
                RunCmd "rm -R -f " & Chr(34) & ImageDirPath & Chr(34) & "\*\*.pj"
                RunCmd "rm -R -f " & Chr(34) & ImageDirPath & Chr(34) & "\*\*\*.pj"
                RunCmd "rm -R -f " & Chr(34) & ImageDirPath & Chr(34) & "\*\*\*\*.pj"
                RunCmd "rm -R -f " & Chr(34) & ImageDirPath & Chr(34) & "\*\*\*\*\*.pj"
                RunCmd "rm -R -f " & Chr(34) & ImageDirPath & Chr(34) & "\*\*\*\*\*\*.pj"
                RunCmd "rm -R -f " & Chr(34) & ImageDirPath & Chr(34) & "\*\*\*\*\*\*\*.pj"
                RunCmd "rm -R -f " & Chr(34) & ImageDirPath & Chr(34) & "\*\*\*\*\*\*\*\*.pj"
                RunCmd "rm -R -f " & Chr(34) & ImageDirPath & Chr(34) & "\*\*\*\*\*\*\*\*\*.pj"
		
        End Function


    '*************************************************************************************************
    '* Function:    UpdateInstallShieldVersion
    '* Purpose:     Change the "product version" of an installshield developer 7 & 8 project
    '*                  Note, this seems to fails for MergeModule projects, seems to work
    '*                  for both standard and basic msi projects.
    '* Parameters   - param_pathToIsm       : Location of installshield ism
    '*              - param_versionNumber   : New version number
    '*************************************************************************************************
    'This is the same function that is used in the SOLsearcher build
    Function UpdateInstallShieldVersion(ByRef aProject, param_pathToIsm, param_versionNumber)
        On Error Resume Next    'trying to set version number on MergeModule projects causes exception.
        WriteToDlgAndFile "Updating InstallShieldVersion: " & param_pathToIsm
        aProject.OpenProject (param_pathToIsm)
        aProject.ProductVersion = param_versionNumber
        aProject.SaveProject
	aProject.CloseProject        
        
    End Function

    '*************************************************************************************************
    '* Function:    UpdateInstallShieldProperty
    '* Purpose:     Change a property in the 'Property Table' in installshield by opening the msi
    '*              database.
    '*             
    '* Parameters : param_pathToIsm - path to installshield project
    '*              aPropName       - name of property in property table
    '*              aPropVal        - value to set property
    '*************************************************************************************************
    'This is the same function that is used in the SOLsearcher build
    Function UpdateInstallShieldProperty(param_pathToIsm, aPropName, aPropVal)
		Set oMSI = CreateObject("WindowsInstaller.Installer")

		' open ISM file in transacted mode
		Set oDB = oMSI.OpenDatabase(param_pathToIsm, 1)
		' fetch the one and only samplesource record
		Set oView = oDB.OpenView("SELECT * FROM `Property` WHERE `Property`=" & "'" & aPropName & "'")
		oView.Execute
		Set oRec = oView.Fetch
		' change field 2, the Value field
		oRec.StringData(2) = aPropVal
		' update the changed record
		oView.Modify 2, oRec
		' close the view, commit changes, clean up
		oView.Close: oDB.Commit: Set oMSI = Nothing       
    End Function

	'******************************************************************************
	' Function    : CleanFolder
	' Purpose     : remove an files from a folder except for exclusion files.
	' Parameters  : aFlder - the folder to clean
	'******************************************************************************
	Function CleanFolders(aFldr, ByRef ExclusionList())
		   Dim f, f1, sf, nResult
           Dim fldr
	   Err.Clear
           Set f = mFSO.GetFolder(aFldr)
           Set sf = f.SubFolders
           nResult = CleanFiles(aFldr, ExclusionList)
	   For Each f1 In sf
		nResult = CleanFolders(f1.Path, ExclusionList)  'need to pass in paths
           Next	
	   If Err.Number <> 0 Then
                BuildError "Unable to clean folder: " + aFldr + " : " + Err.Description
           End If   
	End Function
	
	'****************************************************************************
	' Function    : CleanFiles
	' Purpose     : Delete any files in a given folder except those in the exclusionlist 
	' Parameters  : aFolder - the path to the folder with files to remove
	'
	'*****************************************************************************   
	Function CleanFiles(aFolder, ByRef ExclusionList())
         Dim f, f1,fc
         Dim Path, nResult
	 Err.Clear
         Set f = mFSO.GetFolder(aFolder)
         Set fc = f.Files 'file list	
         For Each f1 In fc
              nResult = false	
              For Each ext In ExclusionList
			If InStr(f1.Name, ext) Then
				nResult = true
			End If		
	      Next
              If nResult <> true Then
		      mFSO.DeleteFile f1, true	
	      End If
         Next
	 If Err.Number <> 0 Then
              BuildError "Unable to clean files in : " + aFolder + " : " + Err.Description
         End If   	 
   End Function


    '***************************************************************************************
    '* Function    :         CompareFolders
    '* Purpose     :         Verify contents of two folders are the same
    '* Parameters  :         aFldr = path to first folder to compare
    '*                       bFldr = name of folder to compare, located within aFolder
    '* Description :         Compare 2 folders to see if they have the same contents
    '*
    '**************************************************************************************
    Function CompareFolders(aFldr, bFldr)
           Dim f, f1, sf, nResult
           Dim fldr
	   Err.Clear
           Set f = mFSO.GetFolder(aFldr)
           Set sf = f.SubFolders
           nResult = CompareFiles(aFldr, bFldr)
		   For Each f1 In sf
				nResult = CompareFolders(f1.Path, bFldr & "\" & f1.Name)  'need to pass in paths
           Next	
	   If Err.Number <> 0 Then
              BuildError "Unable to compare folders : " + aFldr + " and " + bFldr + " : " + Err.Description
           End If  
    End Function

    '***************************************************************************************
    '* Function    :         CompareFiles
    '* Purpose     :         Verify contents of two folders are the same
    '* Parameters  :         aFolder = path to first folder to compare
    '*                       bFolder = name of folder to compare, located within aFolder
    '* Description :         First check if all files in "a" are in "b"
    '*                       Second check if all files in "a" have same date as "b"
    '**************************************************************************************	
   Function CompareFiles(aFolder, bFolder)
         Dim f, f1, f2, fc
         Dim Path, nResult, pos
         Dim date1, date2 ', time1, time2   add time later
	 Err.Clear
         Set f = mFSO.GetFolder(aFolder)
         Set fc = f.Files 'file list
		 CompareFiles = True	
         For Each f1 In fc
              nResult = mFSO.FileExists(bFolder & "\" & f1.Name)
              If nResult <> True Then   'check if exists in MKS
				WriteToDlgAndFile  "Error : Unable to find : " + f1 + " in the installsource "	
				mErrCount = mErrCount + 1	
			  Else
				Set f2 = mFSO.GetFile(bFolder & "\" & f1.Name)
				pos = InStr(f1.DateLastModified, " ")	
				date1 = Left(f1.DateLastModified, pos) 
				date2 = Left(f2.DateLastModified, pos)
				If date1 <> date2 Then   'check if timestamp in MKS matches
        			WriteToDlgAndFile "Error : Timestamp of file : " & f1 & " " & date1 & _
               	            	  " Timestamp of file : " & f2 & " " & date2	   
					mErrCount = mErrCount + 1
	    		Elseif f1.Size <> f2.Size Then
					WriteToDlgAndFile  " Error : Size of file : " & f1 & " " & f1.Size &  _
                        	    	      "Size of file : "
					mErrCount = mErrCount + 1		
				End If		
              End If
         Next
         If Err.Number <> 0 Then
              BuildError "Unable to compare files in " + aFolder + " and " + bFolder + " : " + Err.Description
         End If  
	 
   End Function
   '=================================================================
   ' Function    : CheckErrorCount
   ' Parameter   : aStr - string to pass into BuildError
   ' Description : checks if the error count used by CompareFiles
   '               is greater then 0. If it is give a Build Error.
   ' Usage       : Used to keep count of the number of errors CompareFiles
   '               accumulates. Otherwise user will not receive any error
   '               messages, they will only be found in the logs.              
   '=================================================================
   Function CheckErrorCount(aStr)
   	  If mErrCount > 0 Then
		BuildError aStr 
	  End If
   End Function	
 
   '==================================================================
   ' Function    : ValidateInstallSource
   ' Parameter   : aMKS - MKS handle to perform mks functions
   ' Description : Makes a parallel copy of InstallSrc folder into 
   '		       "BuildInstallSource". Deletes InstallSrc folder after
   '               and revert restores timestamp of files.
   ' Usage       : Used to perform file/folder validation of contents
   '               in installsource. InstallSource and BuildInstallSource
   '               folders are used by CompareFolders function to validate.
   '==================================================================
   Function ValidateInstallSource(aMKS, aMoveFiles)
	
		Dim ExclusionList 
		ExclusionList = Array(".pj")
		aMoveFiles.CopyFileOrFolder "Folder", InstallSrcPath, MainProjPath & "\BuildInstallSource", True
		RunCmd ("attrib -r " & InstallSrcPath & "\*.* /S")
		'aMoveFiles.DeleteFileOrFolder "Folder", InstallSrcPath, True	
		CleanFolders InstallSrcPath, ExclusionList
		aMKS.Refresh MainPJFile, InstallSrcPJFile
		aMoveFiles.CopyFileOrFolder "Folder", InstallSrcPath, MainProjPath & "\ArchivedInstallSource", True
		RunCmd ("attrib -r " & InstallSrcPath & "\*.* /S")
		aMoveFiles.DeleteFileOrFolder "Folder", InstallSrcPath, True
		aMoveFiles.CopyFileOrFolder "Folder", MainProjPath & "\BuildInstallSource", InstallSrcPath, True
	
   End Function

    'had to create this function for SOLsearcher since that project creates 2 exes with different names than
        'what is used by CreateSelfExtractingEXE.  Probably should change the other projects to use this fctn instead
        'and just pass in the name so we don't have 2 functions with nearly identical code to maintain.
    Function CreateSelfExtractingEXE(aVersionNum, aName, aSetupName)
            Dim Path, Name, destinationFile, SourceWebPackageExe
            Path = "C:\Program Files\InstallShield\PackageForTheWeb 3"
            Name = VersionToNewTitle(aVersionNum, aName)
            WriteToDlgAndFile "creating self extracting exe: " + aName + ".exe"
            destinationFile = Path + "\Projects\" + Name + ".pfw"
	    Err.Clear
            If mFSO.FileExists(destinationFile) Then
                    mFSO.DeleteFile destinationFile, 1
            End If
	    If aSetupName = "" Then
		aSetupName = "Setup.exe"
	    End If
            CreateWebProjectFile destinationFile, Name, aVersionNum, mProductName, Name + ".exe", aSetupName

            'Generate Web Self-Extracting EXE     '-a autobuild, -s silent mode
            SourceWebPackageExe = Path + "\pftwwiz.exe"
            RunCmd Chr(34) + SourceWebPackageExe + Chr(34) + " " + Chr(34) + destinationFile + Chr(34) + " -a -s"
			CreateSelfExtractingEXE = Name + ".exe"
	    If Err.Number <> 0 Then
                BuildError "Unable to create self extracting exe : " + aSetupName + " : " + Err.Description
            End If  
    End Function

    'ex. Changes 5.01.000001 ==> SPD5_01_000001
    'Used in the .pfw file for the FTP site self exe
    Private Function VersionToNewTitle(Version, product)
            VersionToNewTitle = product & Replace(Version, ".", "_", 1, -1)
    End Function

    Private Function CreateWebProjectFile(aFile, aTitle, aVersionNum, aProjName, aExeName, aSetupName)
        WriteToDlgAndFile "creating project file for self extracting exe"
        Set writeFile = mFSO.CreateTextFile(aFile)
        writeFile.writeLine "[PackageForTheWeb]"
        writeFile.writeLine "version=2#"
        writeFile.writeLine ""
        writeFile.writeLine "[Options]"
        writeFile.writeLine "Title=" + aTitle
        writeFile.writeLine "Company=Solimar Systems, Inc."
        writeFile.writeLine "CompanyEMail=tech.support@solimarsystems.com"
        writeFile.writeLine "BasePath=" + ImageDirPath
        writeFile.writeLine "ImportPath="
        writeFile.writeLine "UseRTF=0"
        writeFile.writeLine "SaveFiles=0"
        writeFile.writeLine "SubFolders=1"
        writeFile.writeLine "ApplicationName=" + aProjName
        writeFile.writeLine "Description="
        writeFile.writeLine "Comments="
        writeFile.writeLine "Notice=2004 Copyright Solimar Systems, Inc."
        writeFile.writeLine "Version=" + aVersionNum
        writeFile.writeLine "OutputSpec=" + CStr(mProjPaths.mMainProjPath + "\FTP\" + aExeName)
        writeFile.writeLine "GUIDs=0"
        writeFile.writeLine "Type=2"
        writeFile.writeLine "Compress=1"
        writeFile.writeLine "Sign=0"
        writeFile.writeLine "Transfer=0"
        writeFile.writeLine "Files="
        writeFile.writeLine ""
        writeFile.writeLine "[Engine]"
        writeFile.writeLine "Setup=" & aSetupName
        writeFile.writeLine ""
        writeFile.writeLine "[Web Page]"
        writeFile.writeLine "Generate=1"
        writeFile.writeLine "Template="
        writeFile.writeLine "MSClient="
        writeFile.writeLine "NClient="
        writeFile.writeLine ""
        writeFile.writeLine "[Runtime]"
        writeFile.writeLine "Welcome="
        writeFile.writeLine "License="
        writeFile.writeLine "Prompt="
        writeFile.writeLine "Password="
        writeFile.writeLine "DefaultPath=%TEMP%\" + mProductName + " CD Image "
        writeFile.writeLine "Language=English"
        writeFile.writeLine "WindowStyle=0"
        writeFile.writeLine "Options=35"
        writeFile.writeLine "Execute=" & aSetupName
        writeFile.writeLine "CmdLine=/SMS"
        writeFile.writeLine "[Extension]"
        writeFile.writeLine "Server="
        writeFile.writeLine "Calls=0"
        writeFile.writeLine "[Code Signing]"
        writeFile.writeLine "CompanyURL="
        writeFile.writeLine "SpecFile="
        writeFile.writeLine "KeyFile="
        writeFile.writeLine "Method=0"
        writeFile.writeLine ""
        writeFile.Close
    End Function

    'returns only the numerical version number without the prefix.
    Function StripPrefix(Version, prefix)
        If InStr(1, Version, prefix, vbTextCompare) > 0 Then
            StripPrefix = Mid(Version, Len(prefix) + 1)
        Else
            StripPrefix = Version
        End If
    End Function

    Sub UpdateRegVersionVals(aNextVersion, aCurrentVersion)
        Dim current, output
        current = GetBuildVersion(aNextVersion)
        WriteToDlgAndFile "Updating next version " + aNextVersion + " with value " + Right(mNewNextVersion, 4)
        RunCmd "attrib -r " + mInputFile

	Dim MKS
	Set MKS = New CMKS
	Set MKS.Build = Me
	If Not g_DEBUG Then
	    MKS.RefreshFile mBuildPath & "\buildScript.pj", mInputFile
  	    MKS.LockFiles mBuildPath & "\buildScript.pj", mInputFile
            UpdateFileVersion mInputFile, aCurrentVersion, " " + current
            UpdateFileVersion mInputFile, aNextVersion, " " + Right(mNewNextVersion, 4)
            MKS.CheckInFiles mBuildPath & "\buildScript.pj", mInputFile
        End If
    End Sub

    Function GetBuildVersion(aPattern)
            Dim output, readFile, writeFile
	        Dim MKS
	        Set MKS = New CMKS
	        Set MKS.Build = Me
		MKS.RefreshFile mBuildPath & "\buildScript.pj", mInputFile
		Err.Clear
                Set readFile = mFSO.OpenTextFile(mInputFile)
                Dim check, Line, pos
                check = 0
                Do While Not readFile.AtEndOfStream And check = 0
                        Line = readFile.ReadLine
                        If InStr(Line, aPattern) <> 0 And check = 0 Then
                                'If InStr(Version, Right(Line, Len(Line) - Len(aPattern))) = 0 Then
                                '        UpdateFileVersion = 1
                                'End If
                                check = 1
                                pos = InStr(Line, " ")
                                GetBuildVersion = Right(Line, Len(Line) - pos)
                        End If 
                Loop
                readFile.Close
		Set MKS = Nothing
	   	If Err.Number <> 0 Then
                	BuildError "Unable to get build version for : " + aPattern + " : " + Err.Description
          	End If 
    End Function
    
    Function GetVersionFromFile(aFile, aPattern, aKey)
        Dim output, readFile, writeFile, pos
		Dim check 
		check = 0
        Set readFile = mFSO.OpenTextFile(aFile)
        Do While Not readFile.AtEndOfStream And check = 0
                Line = readFile.ReadLine
                If InStr(Line, aPattern) <> 0 Then
                        pos = InStr(Line, aKey)
                        GetVersionFromFile = Trim(Left(Right(Line, Len(Line) - pos), Len(Right(Line, Len(Line) - pos)) - 1))
						check = 1	
                End If
        Loop
        readFile.Close
  	If Err.Number <> 0 Then
               	BuildError "Unable to get version from file : " + aFile + " : " + Err.Description
       	End If 
    End Function
        
    Function IsFileEmpty(aFile)
           Dim output, readFile, writeFile
           Set readFile = mFSO.OpenTextFile(aFile)
           Dim check, Line, pos
           If readFile.AtEndOfStream <> True Then
                IsFileEmpty = False
           Else
                IsFileEmpty = True
           End If
           readFile.Close
     	   If Err.Number <> 0 Then
               	BuildError "Unable to determine if file is empty : " + aFile + " : " + Err.Description
       	   End If 
    End Function
                 
    Private Function FormatNextVersionNum()
        Dim temp
        temp = Replace(mNewNextVersion, "_", ".")  '%%%% fix this
    End Function
    
	Function GetEmailBody()
		Dim oldDevPath, newDevPath
		oldDevPath = GetBuildVersion(ProductName & "CurrentVersion")
		newDevPath = GetBuildVersion(ProductName & "NextVersion")
	           
		GetEmailBody = "Hello," & vbNewLine _
					& "    " & "Just wanted to remind everyone that since we just released " & oldDevPath & ". Everyone "    & vbNewLine _
					& "should make their current development sandboxes for " & oldDevPath & ". The new Next Version is "     & vbNewLine _
				  	& newDevPath & " which is now the main trunk. " & vbNewLine _
					& "	   " & "I have an additional comment regarding the " & oldDevPath & " release. When we made "    & vbNewLine _
					& "the " & oldDevPath & " development path we branched off the " & PrevLabel & " label. This means that " & vbNewLine _
					& "if you checked any files into " & oldDevPath & " after build " & PrevLabel & " your changes did not " & vbNewLine _
					& "actually make it into the " & oldDevPath & " branch and need to be merged into " & newDevPath & "."   & vbNewLine _
					& "Please double check your changes for any files may need to be merged." _
	End Function

	Function GetEmailSubject()
		Dim oldDevPath, newDevPath 
		oldDevPath = GetBuildVersion(ProductName & "CurrentVersion")
		GetEmailSubject = ProductName & " " & oldDevPath & " release and your sandboxes." 
	End Function
	
    Function SendEmail(aSubject,aMsg)
           WriteToDlgAndFile "Sending the email"
            Dim oSMTP
            Set oSMTP = wscript.CreateObject("SoftArtisans.SMTPMail") 'SMTPControl.SMTP")
    
            Error = False
            Err.Clear    
             With oSMTP
             .RemoteHost = "mail.corp.solimarsystems.com"
             .MailFromAddress = "alan.chou@solimarsystems.com"
             .FromName = "alan.chou@solimarsystems.com"
	     If mBuildType <> "CB" Then
             	.AddRecipient "Tech.Support.Internal@SolimarSystems.com" 
	     End If
  	     .AddRecipient "Engineering@SolimarSystems.com"
	     .AddCC "alan.chou@solimarsystems.com" 
	     .Charset = 1
             .Subject = aSubject
             .BodyText = aMsg
             .SendMail
             End With
            
            If Err.Number <> 0 Then 'There was an error sending out the email
                Error = True
            End If
            If Error = True Then
                EndProgram eEmailSendError '"Error " & Err.Number & ": " & Err.Description
            End If
	    Set oSMTP = Nothing
    End Function

    Function BuildError(aErrMsg)
        WriteToDlgAndFile "ERROR: " + aErrMsg
        NetSendMsg "BUILD ERROR: Refer to " + mLogFileName + " for details"
        Dim Result
       ' If mIgnoreMsgBox <> "Y" Then
            Result = MsgBox("An error occured in the current build." + vbNewLine + "Refer to " + mLogFileName + _
                " for details.  Do you want to abort this build?", vbYesNo, "BUILD ERROR")
       ' End If
        
        
        If Result = vbYes Then  ' Or mIgnoreMsgBox <> "N" Then
            If Not IsNull(mNewBuild) Then
                   ' If mIgnoreMsgBox <> "Y" Then
                            Result = MsgBox("Do you want to undo the changes that have been made so far?", vbYesNo, "Undo Changes")
                  '  End If
                    If Result = vbYes Then ' Or mIgnoreMsgBox <> "N" Then
                            mNewBuild.UndoBuildChanges
                    End If
            End If
            TerminateBuild
            wscript.Quit 0
        End If
                    
        End Function

    Sub TerminateBuild()
        Dim MKS
        Set MKS = New CMKS
        Set MKS.Build = Me
        WriteToDlgAndFile "Unlocking files if any are locked."
        MKS.UnlockFiles
        Set MKS = Nothing

        Dim cancelFile
        WriteToDlgAndFile "Creating Cancelled.txt"
        Set cancelFile = mFSO.CreateTextFile("Cancelled.txt")
        cancelFile.writeLine ""
        cancelFile.Close
    End Sub

    Sub Cancel()
        WriteToDlgAndFile "Build was cancelled by the user"

        If Not IsEmpty(mExecInfo) Then
            Dim ErrorStr, CmdStr
            Do: wscript.Sleep 10
                CmdStr = CmdStr & mExecInfo.StdOut.ReadAll
                wscript.echo CmdStr
                ErrorStr = ErrorStr & mExecInfo.StdErr.ReadAll
            Loop While mExecInfo.Status = 0 And Not mExecInfo.StdOut.AtEndOfStream _
                               And Not mExecInfo.StdErr.AtEndOfStream
 
            Dim ExitCode
                        ExitCode = mExecInfo.ExitCode
                        WriteCmdOutputToFile "logFolder\StandardOutput.txt", "Build cancelled by user - output is from the last command that was run.", CmdStr
                        WriteCmdOutputToFile "logFolder\StandardError.txt", "Build cancelled by user - output is from the last command that was run.", ErrorStr
 
            If ExitCode <> 0 Then
                WriteToDlgAndFile "There was an error executing the last command.  Exit code was: " & ExitCode
                NetSendMsg "BUILD ERROR when running last command. Refer to " + mLogFileName + " to see what command failed."
            End If
        End If

        TerminateBuild
        If Not IsNull(mNewBuild) Then
            mNewBuild.UndoBuildChanges
        End If

    End Sub

    Private Sub Class_Terminate()  'this gets called when the CProductBuild object gets deleted
        Dim endTime
        endTime = Time
        WriteToDlgAndFile "Time Ended: " & endTime
        WriteToDlgAndFile "Time for Script to Run: " & getHrMinSec(mStartTime, endTime)
	Dim MoveFiles
	Set MoveFiles = New CMoveFiles
	Set MoveFiles.Build = Me
	DestPath = mBuildPath & "\\logFolder\\" & mProductName
	If mFSO.FolderExists(DestPath) <> True Then
		MoveFiles.CreateFileOrFolder "Folder", DestPath, True
	End If
	Dim logFileName
	Select Case mBuildType 
               Case "NB"	'New Build 	
			logFileName = "\\BuildLog - " & mVersionNum& ".txt"
	       Case "R"		'Release
			logFileName = "\\ReleaseLog - " & mPrevLabel & ".txt"
	       Case "CB"	'Branch
			logFileName = "\\BranchLog - " & mPrevLabel & ".txt"
	End Select	
	MoveFiles.CopyFileOrFolder "File", mLogFileName , DestPath & logfileName, True 
        mBuildDialog.ScriptDone False
        wscript.DisconnectObject mBuildDialog
        Set mBuildDialog = Nothing
        Dim writeFile
        Set writeFile = mFSO.CreateTextFile("Done.txt")
        writeFile.Close
        NetSendMsg "The " + mProductName + " build has completed."
    End Sub

End Class

Class CProjectPaths
    Public mMainProjPath, mMainCodePath, mInstallSourcePath, mInstallShieldProjPath, mMainPJFile
        Public mMainSourcePJFile, mImageDirPath, mMainSourceRCSPJFile  , mInstallSourceBuildPJ
        Public mInstallSourcePJFile, mSourceInstallSourcePJ, mMainCodePJFile, mMainSrcPath

    Public Sub Class_Initialize() 'set default values
        mMainProjPath = ""
        mMainCodePath = ""
        mInstallSourcePath = ""
        mInstallShieldProjPath = ""
    End Sub
 
    Public Sub SetMainProjPath(aMainProjDir)
        mMainProjPath = aMainProjDir
    End Sub

    Public Sub SetCodePath(aCodeDir)
        mMainCodePath = aCodeDir
    End Sub

    Public Sub SetInstallSourcePath(aInstallSourceDir)
        mInstallSourcePath = aInstallSourceDir
    End Sub

    Public Sub SetInstallShieldPath(aInstallShieldDir)
        mInstallShieldProjPath = aInstallShieldDir
    End Sub

    Public Sub SetMainPJFile(aPJFile)
        mMainPJFile = aPJFile
    End Sub

	Public Sub SetMainCodePJFile(aPJFile)
		mMainCodePJFile = aPJFile
	End Sub
	
    Public Sub SetMainSourcePJFile(aPJFile)
        mMainSourcePJFile = aPJFile
    End Sub

    Public Sub SetMainSrcPath(aPath)
        mMainSrcPath = aPath
    End Sub

    Public Sub SetImageDirPath(aImageDir)
        mImageDirPath = aImageDir
    End Sub

    Public Sub SetMainSourceRCSPJFile(aRCSPJFile)
        mMainSourceRCSPJFile = aRCSPJFile
    End Sub
 
    Public Sub SetInstallSourcePJFile(aPJFile)
        mInstallSourcePJFile = mInstallSourcePath + "\" + aPJFile
	    mInstallSourceBuildPJ = aPJFile
    End Sub

    Public Sub SetSourceInstallSourcePJFile(aPJFile)
        mSourceInstallSourcePJ = aPJFile
    End Sub

    Private Sub Class_Terminate()
    End Sub

End Class



