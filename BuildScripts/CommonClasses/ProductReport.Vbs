Class CProductReport
    Private mShell, mFSO, mStartTime, mReportLogFile, mDebug, mCmdTemp, mCancelDiag
    Private mBuildType, mPrevLabel, mProjPaths, mLeftOverSecs, mReleaseFile, mExecInfo, SolSearcherSpecialFS, OutputBasePath
    Private mSrcLabel, mLastBuild, mLastRelease, mBegDevPath, mLastLabel, mPopDataBase, mProject, mLastBuildLabel, mLicenseSummary 
    Private mGenTextFile, mRootProjectDir, mGenAllRelease, mProductChoice, mEmailChoice, mOneStepBldChoice, mStartLabel, mEndLabel
    Private mLogFileName, mInputFile, mksProgram, mCurrentDir, mNumPJFiles, mPJFileArray(2), mFilePathArray(2), mFileNameArray(10), fileNameCount
    Private eReportFilesError, eEmailSendError, eCancel, mMKS , mFileNameCount			
    Public mReportPath
          
    Public Sub Class_Initialize() 'set default values
        Set mShell = wscript.CreateObject("Wscript.Shell")
        Set mFSO = wscript.CreateObject("Scripting.FileSystemObject")
        Set mCancelDiag = wscript.CreateObject("BuildDialog.BuildDialogB6", "BuildDiag_")
        Set mProjPaths = New CProjectPaths
        
        eReportFilesError = 0
        eEmailSendError = 1
        eCancel = 2
        OutputBasePath = "L:\cd_images\"  
        mSrcLabel = ""
        mGenTextFile = "N"
        mLastBuild = "N"
        mLastRelease = "N"
        mBegDevPath = "N"
        mLastLabel = "N"
        mGenAllRelease = "N"
        mNumPJFiles = 1
        mOneStepBldChoice = "N"
        mEmailChoice = "B"
        mProject = 1
        mDebug = "N"
        mStartTime = Time
        mPopDataBase = "N"
	mFileNameCount = 0
		
 	Dim CurrentDir
        CurrentDir = mShell.CurrentDirectory
        mReportPath = mFSO.GetFolder(CurrentDir & "\\")
        If mFSO.FileExists("..\Cancelled.txt") = True Then
                EndProgram (eCancel)
        End If
        If mFSO.FileExists("..\ReportDone.txt") = True Then
                mFSO.DeleteFile "..\ReportDone.txt", 1
        End If
        mLeftOverTime = 0
        mInputFile = "BuildRegistry.txt"
        mFSO.CreateTextFile (mReportPath & "\logFolder\StandardOutput.txt")
        mFSO.CreateTextFile (mReportPath & "\logFolder\StandardError.txt")
    End Sub

    Public Sub SetMembers()
        If Not mFSO.FolderExists(mReportPath + "\logFolder") Then
            mFSO.CreateFolder mReportPath + "\logFolder"
        End If
		If Not mFSO.FolderExists(mReportPath + "\Reports") Then
            mFSO.CreateFolder mReportPath + "\Reports"
        End If
        mCancelDiag.DisplayCancelDialog "Time Started: " & Time, CInt(45), CStr("Report Script")
        mLogFileName = mReportPath + "\logFolder\ReportLog.txt"
        Set mReportLogFile = mFSO.CreateTextFile(mLogFileName)
    End Sub
   
      '***********************************************************************************************************
        '       -PAR            source label
        '       -PJT            Project Type C, V, BC or BV
        '       -LB             Report based on Last Build
        '       -LR             Report based on Last Release
        '       -BDP            Report based on Beginnning of Dev Path
        '       -LBL            Report based on Specify the label
        '       -LBLB           Report based on Last Build Label
        '       -POP            Populate the database
        '       -CTF            Create Text File (Report)
        '       -ARD            All Release Differences for the project
        '       -PTC            Product to use S=spd, I=iCONVERT, D=DeRoix(SOLsearcher), V=Solscript
        '       -EC             Email to produce B=build R=release
        '       -BLD            B=OneStepBuild, R=Release, N=neither
        '       -SLB            Starting label
        '       -ELB            Ending label
        '       -DB             Y or N
	'       -LS		Y or N
      '************************************************************************************************************
    Public Sub GetParams(aMKS)
        Dim iParam, CodeBuildType, ExistingDevPath, VersionNum
        WriteToDlgAndFile "Params passed into the script: "
            For iParam = 0 To wscript.Arguments.Count - 1 Step 2
            WriteToDlgAndFile "    " + wscript.Arguments(iParam) + " " + wscript.Arguments(iParam + 1)
                    Select Case wscript.Arguments(iParam)
                        Case "-PAR"
                                mSrcLabel = wscript.Arguments(iParam + 1)
                        Case "-LB"              'Last Build
                                mLastBuild = wscript.Arguments(iParam + 1)
                        Case "-LR"              'Last Build
                                mLastRelease = wscript.Arguments(iParam + 1)
                        Case "-BDP"             'Beginnning of Dev Path
                                mBegDevPath = wscript.Arguments(iParam + 1)
                        Case "-LBL"             'Specify the label
                                mLastLabel = wscript.Arguments(iParam + 1)
                        Case "-LBLB"            'Last Build Label
                                mLastBuildLabel = wscript.Arguments(iParam + 1)
                        Case "-POP"             'Populate the database
                                mPopDataBase = wscript.Arguments(iParam + 1)
                        Case "-CTF"             'Create Text File
                                mGenTextFile = wscript.Arguments(iParam + 1)
                        Case "-PTH"             'Path to the pj files
                                mRootProjectDir = wscript.Arguments(iParam + 1)
                        Case "-ARD"             'All Release Differences
                                mGenAllRelease = wscript.Arguments(iParam + 1)
                        Case "-PTC"             'Product
                                mProductChoice = wscript.Arguments(iParam + 1)
                        Case "-EC"              'Email choice
                                mEmailChoice = wscript.Arguments(iParam + 1)
                        Case "-BLD"             '1-Step Bld choice
                                mOneStepBldChoice = wscript.Arguments(iParam + 1)
                        Case "-SLB"               'start label
                                mStartLabel = wscript.Arguments(iParam + 1)
                        Case "-ELB"             'end label
                                mEndLabel = wscript.Arguments(iParam + 1)
                        Case "-DB"              ' debug option
                                mDebug = wscript.Arguments(iParam + 1)
			Case "-LS"
				mLicenseSummary = wscript.Arguments(iParam + 1)
                 End Select
            Next
	    Set mMKS = aMKS	
    End Sub

    Function SetProjPathsVar(aProductName, aProjectName, aProjPath, aInstallSrcArchive, aArchive, aPJFile, bPJFile, cPJFile, _
                              aFilePath, bFilePath, cFilePath, aReleaseFile, aFileNamePrefix, aVersionPrefix)
       'Pull this information from a file instead?
        mProjPaths.SetMainProductName aProductName
        mProjPaths.SetMainProjectName aProjectName
        mProjPaths.SetMainSrcProjPath aProjPath
        mProjPaths.SetInstallSrcArchive aInstallSrcArchive
        mProjPaths.SetArchive aArchive
        mProjPaths.SetReleaseFile mReportPath & "\Product_Scripts\" & aReleaseFile
        mProjPaths.SetFileNamePrefix aFileNamePrefix
        mProjPaths.SetVersionPrefix aVersionPrefix
        mPJFileArray(0) = aPJFile
        mPJFileArray(1) = bPJFile
        mPJFileArray(2) = cPJFile
        mFilePathArray(0) = aFilePath
        mFilePathArray(1) = bFilePath
        mFilePathArray(2) = cFilePath
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
        mCancelDiag.AddProgress CInt(minsPassed)
        
        WriteCmdOutputToFile mReportPath & "\logFolder\StandardOutput.txt", CmdLine, CmdStr
        WriteCmdOutputToFile mReportPath & "\logFolder\StandardError.txt", CmdLine, ErrorStr
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
    
    Public Function WriteToDlgAndFile(Display)
        If Display <> "" Then
                Dim diagDisplay
                    diagDisplay = CStr(Time & " : " & Display)
                If Not IsEmpty(mCancelDiag) Then
                        mCancelDiag.UpdateDisplay CStr(diagDisplay)
                    End If
                    wscript.echo diagDisplay
            mReportLogFile.writeLine diagDisplay
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

        'fix checkPath later so all projects can use it. -emester
    'Public Function CheckPath(path)
            
    '    If (mFSO.FolderExists(path) = False) Then
        '       CheckPath GetUpperDirPath(path)
        '       mFSO.CreateFolder path
        ' End If
            
    'End Function

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

    '***************************************************************************
    ' Function    : StripAllPrefix
    ' Parameters  : Version - the full label/version name 
    ' Description : Removes the prefix from the label/version name using the 
    '               VersionPrefix property set during initialization
    '***************************************************************************
    Function StripAllPrefix(Version)
		Dim pos, prefix
		If InStr(Version, VersionPrefix) <> 0 Then
			StripAllPrefix = Right(Version, Len(Version) - Len(VersionPrefix))
		Else
			For pos=1 To Len(Version)
				If IsNumeric(Mid(Version, pos, 1)) Then
					Exit For
				End If
			Next
			StripAllPrefix = Mid(Version, pos)
		End If
    End Function 
	
    Function GetBuildVersion(aPattern)
                Dim output, readFile, writeFile
                Set readFile = mFSO.OpenTextFile(mInputFile)
                Dim check, line, pos
                check = 0
                Do While Not readFile.AtEndOfStream And check = 0
                        line = readFile.ReadLine
                        If InStr(line, aPattern) <> 0 And check = 0 Then
                                'If InStr(Version, Right(Line, Len(Line) - Len(aPattern))) = 0 Then
                                '        UpdateFileVersion = 1
                                'End If
                                check = 1
                                pos = InStr(line, " ")
                                GetBuildVersion = Right(line, Len(line) - pos)
                        End If
                    
                Loop
                readFile.Close
    End Function
    
    Function GetVersionFromFile(aFile, aPattern, aKey)
        Dim output, readFile, writeFile, pos
        Set readFile = mFSO.OpenTextFile(aFile)
        Do While Not readFile.AtEndOfStream
                line = readFile.ReadLine
                If InStr(line, aPattern) <> 0 Then
                        pos = InStr(line, aKey)
                        GetVersionFromFile = Trim(Left(Right(line, Len(line) - pos), Len(Right(line, Len(line) - pos)) - 1))
                End If
        Loop
        readFile.Close
    End Function
                     
    Function BuildError(aErrMsg)
        WriteToDlgAndFile "ERROR: " + aErrMsg
        NetSendMsg "BUILD ERROR: Refer to " + mLogFileName + " for details"
        Dim Result
        Result = MsgBox("An error occured in the current build." + vbNewLine + "Refer to " + mLogFileName + _
                " for details.  Do you want to abort this build?", vbYesNo, "BUILD ERROR")
        If Result = vbYes Then  ' Or mIgnoreMsgBox <> "N" Then
            TerminateBuild
            wscript.Quit 0
        End If
   End Function

   Sub TerminateBuild()
        Dim cancelFile
        WriteToDlgAndFile "Creating Cancelled.txt"
        Set cancelFile = mFSO.CreateTextFile(mReportPath & "\Cancelled.txt")
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
                        WriteCmdOutputToFile mReportPath & "\logFolder\StandardOutput.txt", "Build cancelled by user - output is from the last command that was run.", CmdStr
                        WriteCmdOutputToFile mReportPath & "\logFolder\StandardError.txt", "Build cancelled by user - output is from the last command that was run.", ErrorStr
 
            If ExitCode <> 0 Then
                WriteToDlgAndFile "There was an error executing the last command.  Exit code was: " & ExitCode
                NetSendMsg "BUILD ERROR when running last command. Refer to " + mLogFileName + " to see what command failed."
            End If
        End If

        TerminateBuild
    End Sub

    Private Sub Class_Terminate()  'this gets called when the CProductBuild object gets deleted
        Dim endTime
        endTime = Time
        WriteToDlgAndFile "Time Ended: " & endTime
        WriteToDlgAndFile "Time for Script to Run: " & getHrMinSec(mStartTime, endTime)
        mCancelDiag.ScriptDone False
        wscript.DisconnectObject mCancelDiag
        Set mCancelDiag = Nothing
        Dim writeFile
        Set writeFile = mFSO.CreateTextFile(mReportPath & "\ReportDone.txt")
        writeFile.Close
        NetSendMsg "The " + mProductChoice + " report has completed."
    End Sub
    
   Public Function PopulateDataBase(aRepAccess)

            If mPopDataBase = "Y" Then
                WriteToDlgAndFile "Deleting Tables"
                aRepAccess.DeleteTable ("txm")
                aRepAccess.DeleteTable ("txs")
                aRepAccess.DeleteTable ("txr")
                'Loop through the number of project files
                Dim nResult
                For loopIndex = 0 To (mNumPJFiles - 1)
                        WriteToDlgAndFile "Generating files from " & mPJFileArray(loopIndex)
                        wscript.echo "si report --basename=" & Chr(34) & MainProjectName & Chr(34) & " --project=" & _	
                                MainSrcProjPath + "\" & mPJFileArray(0) & ".pj"    
                        nResult = mShell.Run("si report --basename=" & Chr(34) & MainProjectName & Chr(34) & " --project=" & _
                                MainSrcProjPath + "\" & mPJFileArray(0) & ".pj",1, True)    
                        wscript.echo mFilePathArray(loopIndex) & "\" & mPJFileArray(loopIndex) & ".tx*"
                        CopyFileOrFolder "File", mShell.CurrentDirectory & "\" & mPJFileArray(loopIndex) & ".tx*", _
                                          mFilePathArray(loopIndex) & "\", True, eReportFilesError
                        'Set new archID offset to that of the highest archID from previous project file
                        mArchOffset = mLargestArchSoFar
                        WriteToDlgAndFile "Populating txm from " & mPJFileArray(loopIndex)
                        aRepAccess.PopulateTable "txm",  mFilePathArray(loopIndex), mPJFileArray(loopIndex), mArchOffset
                        WriteToDlgAndFile "Populating txs from " & mPJFileArray(loopIndex)
                        aRepAccess.PopulateTable "txs",  mFilePathArray(loopIndex), mPJFileArray(loopIndex), mArchOffset
                        WriteToDlgAndFile "Populating txr from " & mPJFileArray(loopIndex)
                        aRepAccess.PopulateTable "txr",  mFilePathArray(loopIndex), mPJFileArray(loopIndex), mArchOffset
                Next
        End If
    End Function
          
       'Changes "Hello" -> Hello
        Function ChopDoubleQuote(str)
                ChopDoubleQuote = Replace(str, Chr(34), "", 1, -1)
        End Function

        'Changes "7.31.000001" -> 7_31_000001
        Function ChangeDotToLine(str)
                ChangeDotToLine = Replace(str, ".", "_", 1, -1)
        End Function

        'Changes 'Hello' -> Hello
        Function ChopSingleQuote(str)
                ChopSingleQuote = Replace(str, "'", "", 1, -1)
        End Function

        'Change "1.2" -> "01.0002"
        'Change "1.2.3.4" -> "01.0002.0003.0004"
        Function InsertZeros(str)
                InsertZeros = ""
                startPos = 2 'skip first spot because it is a "
                endPos = 1
                Do While endPos <> 0
                        endPos = InStr(startPos, str, ".")
                        If endPos = 0 Then
                                Length = Len(str) - startPos
                        Else
                                Length = endPos - startPos
                        End If

                        digit = Mid(str, startPos, Length)
                        If startPos = 2 Then 'Looking at the initial case
                                Length = 2
                        Else
                                Length = 4
                        End If
                        Do While Len(digit) < Length
                                digit = "0" + digit
                        Loop
                        If Length = 4 Then
                                InsertZeros = InsertZeros + "." + digit
                        Else
                                InsertZeros = InsertZeros + digit
                        End If

                        startPos = endPos + 1
                Loop
                InsertZeros = Chr(34) + InsertZeros + Chr(34)
        End Function
     
        'Based on cmdChoice gives returns the right label depending on that choice
        Function FindLabel(cmdChoice)
                Dim RevNum, archID
                mCmdTemp.CommandText = "Select * From txs"
                Set recordSetTXS = mCmdTemp.Execute()
                FindLabel = ""
                Dim LastLabel 
                If cmdChoice = "LB" Or cmdChoice = "LBLB" Then    'LastBuild option
                        FindLabel = GetLatestBuild(mProject, cmdChoice)
                        'Function GetLatestBuild returns the appropriate label
                End If
                If cmdChoice = "BDP" Then  'Find the beginning of the devPath
                'it will be the first release of current build for both
                        Dim lineVersion, wholeLine, greatestVersionSoFar
                        greatestVersionSoFar = 0
                         
                        Set ReleaseFile = mFSO.OpenTextFile(ReleaseFile)     'mRelease is a global var
                        Do Until ReleaseFile.AtEndOfStream
                                wholeLine = ReleaseFile.ReadLine
                                lineVersion = StripAllPrefix(wholeLine)', VersionPrefix)
                                If greatestVersionSoFar < lineVersion And IsNumeric(lineVersion) Then
                                        FindLabel = wholeLine
                                        greatestVersionSoFar = lineVersion
                                End If
                        Loop
                        ReleaseFile.Close
                End If
                If cmdChoice = "LR" Then
                        Set readFile = mFSO.OpenTextFile(ReleaseFile)        'mRelease is a global var
                        Do While Not readFile.AtEndOfStream    
                                FindLabel = readFile.ReadLine 'need to add prefix to release version nums
                        Loop
                        readFile.Close
                End If
		If cmdChoice = "PLR" Then
			Set readFile = mFSO.OpenTextFile(ReleaseFile)        'mRelease is a global var
                        Do While Not readFile.AtEndOfStream 
				LastLabel = FindLabel   
                                FindLabel = readFile.ReadLine
                        Loop
                        readFile.Close 			       'need to add prefix to release version nums	
                        FindLabel =  LastLabel  ' last release before current release
		End If	
                If cmdChoice = "LBL" Then
                        FindLabel = mSrcLabel            'Global Var Read in from the parameters
                End If
                FindLabel = ChopDoubleQuote(FindLabel)
        End Function

        'Checks the pj file in the directory to see if the project is a variant project, or not
        'returns 0 part of the main trunk, 1 if in a var devpath
        Function CheckProject(filePath, pjName)
                Dim line
                line = mMKS.SandboxInfo(RootProjectDir + "\" & MainProjectName & ".pj")		
                If (InStr(line, "Variant") <> 0)  Or (InStr(line, "Build") <> 0)Then
                    CheckProject = 1        'Variant DevPath
                Else
                    CheckProject = 0        'Main DevPath
                End If
        End Function

        'Checks the pj file in the directory to see if the project is a variant project, or not
        'returns 0 part of the main trunk, 1 if in a var devpath
        Function CheckProjectDetailed(filePath, pjName)
                Dim line
		line = mMKS.SandBoxInfo(filePath & "\" & pjName & ".pj")
                WriteToDlgAndFile filePath & "\" & pjName & ".pj"
                If (InStr(line, "Variant") = 0) Or (InStr(line, "Build") = 0) Then
                    CheckProjectDetailed = 0        'Main DevPath
                Else
                    CheckProjectDetailed = 1        'Variant DevPath
                End If
        End Function

        'returns the most current build based on projType
    Function GetLatestBuild(projType, cmdChoice)  
        Dim bHaveSegment, segment, segmentVersion, line, curVersion, startVal, endVal, prevVersion, LastBuild
        prevVersion = 0
        curVersion = 0
        line = ""
        endVal = 0
        bHaveSegment = True
        Dim Done
        Done = False
        Set projFile = mFSO.OpenTextFile(Archive)    'Project Var
        Do Until "symbols" = Mid(line, 1, 7)
            line = projFile.ReadLine
        Loop
        'Extract segment from line
        startVal = endVal + 1
        endVal = InStr(startVal, line, ":")
        If endVal = 0 Then
        'If there were no entries
            Done = True
            'bHaveSegment = False
        Else
            startVal = InStr(startVal, line, " ") + 1
            segment = Mid(line, startVal, endVal - startVal)
        End If
        
        Dim Version, VersionNum, BuildNum
        If mSrcLabel <> "" Then
                Version = Left(StripAllPrefix(mSrcLabel), 4) ', VersionPrefix), 4)
        Else
                Version = GetLastVersionNum
        End If
        'this loop determines puts version numbers and build numbers into VersionNumArray and BuildNumArray
        Do While Done = False And Not projFile.AtEndOfStream
            dotPosition = InStr(segment, "%2E")
            Do Until dotPosition = 0
                segment = Mid(segment, 1, dotPosition - 1) + "." + Mid(segment, dotPosition + 3, Len(segment))
                dotPosition = InStr(segment, "%2E")
            Loop
            FullLabel = segment
            BuildNum = StripAllPrefix(segment) ', VersionPrefix)
            
            If Mid(BuildNum, 1, 1) = "0" Then           'have to account for leading 0's
                VersionNum = Mid(BuildNum, 2, 4)
            Else
                VersionNum = Mid(BuildNum, 1, 4)
            End If
            
            If VersionNum = Version Then
                If BuildNum > curVersion Then
                    curVersion = FullLabel
                ElseIf BuildNum > prevVersion Then
                    prevVersion = FullLabel
                End If
            End If
            segment = GetNextSegment(endVal, line, projFile)
            If segment = "" Then
                Done = True
            End If
            Count = Count + 1
        Loop
        'Exit file
        projFile.Close
        
        WriteToDlgAndFile "/###################################"
        WriteToDlgAndFile "Prev Version: " & prevVersion
        WriteToDlgAndFile "Version: " & curVersion
        WriteToDlgAndFile "####################################/"
  
        If cmdChoice = "LBLB" Then
             GetLatestBuild = prevVersion
        Else
             GetLatestBuild = curVersion
        End If 
        WriteToDlgAndFile "Leaving - GetLatestBuild = [" & GetLatestBuild & "]"
    End Function

Function GetCurrentVersionNum()
        GetCurrentVersionNum = GetBuildVersion(ProductChoice & "CurrentVersion")
End Function

Function GetNextVersionNum()
		GetNextVersionNum = GetBuildVersion(ProductChoice & "NextVersion")     
End Function

Function GetLastVersionNum()
		GetLastVersionNum = GetBuildVersion("last" & ProductChoice & "Build")
End Function

Function GetNextSegment(endVal, line, projFile)

    Dim startVal
    'Extract the next segment from line
    startVal = endVal + 1
    endVal = InStr(startVal, line, ":")
    If endVal = 0 Then
    'If there were no more colons in line
        If InStr(line, ";") Then
            GetNextSegment = ""  'return the empty string
        Else
            line = projFile.ReadLine
            startVal = 9
            endVal = InStr(line, ":")
            GetNextSegment = Mid(line, startVal, endVal - startVal)
        End If
    Else
        startVal = InStr(startVal, line, " ") + 1
        GetNextSegment = Mid(line, startVal, endVal - startVal)
    End If

End Function

'=============================================================================
' Name        : GetDevPath
' Parameters  : filepath - path of the sandbox to get SIE info
'               pjname   - project name in the sandbox
' Description : Retrieve the development path name of the input sandbox. 
'               Development Path names have format "_[Major]_[Minor]" where
'               [Major] is one digit and [Minor] is 2 digits. Information is
'               retrieved through the mks function "SandBoxInfo". This function
'               returns a string of sandbox information which is stripped of the 
'               development path field for GetDevPath to use.
'==============================================================================
Function GetDevPath(filePath, pjName)
                Dim line, field, pos
				If Project = 1 Then
					line = mMKS.SandBoxInfo(filePath & "\" & pjName & ".pj") 'returns relevant information about sandbox
					field = "Development Path: "  
					pos = InStr(line, field) 	              	
                    GetDevPath = Mid(line, pos + Len(field), 5) 'format of all dev path : _5_14
                Else
                    GetDevPath = "_"
                End If
End Function
  
 '***** STILL NEED TO CHECK TO MAKE SURE THE REPORTS WERE GENERATED CORRECTLY ******
Function CopyReports(aInputPath, aOutputPath, arrayFN, counterFN)
    'aOutputPath = aOutputPath & GetProductFolderName & "\"
    
    For Index = 0 To counterFN - 1 Step 1
        CopyFileOrFolder "File", aInputPath & arrayFN(Index), aOutputPath & arrayFN(Index), True, eReportFilesError
    Next
        
End Function
                    
Function SetDestinationPath()
    Dim PathToCopyFiles
    PathToCopyFiles = GetOutputPath
    PathToCopyFiles = PathToCopyFiles & ProductChoice
    SetDestinationPath = PathToCopyFiles
End Function

Function GetOutputPath()
    Dim Path
    If (CheckProject(mFilePathArray(0), mPJFileArray(0))) Then 'If Variant project
        Path = OutputBasePath & "CurrentVersion\"
        If OneStepBldChoice = "B" Then
            Path = Path & "test\"
        ElseIf OneStepBldChoice = "R" Then
            Path = Path & "release\"
        End If
    Else
        Path = OutputBasePath & "NextVersion\test\"
    End If

    GetOutputPath = Path
End Function

'Had to put the copying of files and folders into a seperate function so On Error Resume Next would be isolated
'only to this function
Function CopyFileOrFolder(aCopyType, aSourcePath, aDestinationPath, aOverRide, aErrorCode)
    Err.Clear
    On Error Resume Next
    If aCopyType = "File" Then
        WriteToDlgAndFile "Copying File from: " & aSourcePath & " to: " & aDestinationPath
        mFSO.CopyFile aSourcePath, aDestinationPath, aOverRide
    Else 'copy folder instead
        WriteToDlgAndFile "Copying Folder from: " & aSourcePath & " to: " & aDestinationPath
        mFSO.CopyFolder aSourcePath, aDestinationPath
    End If
    
    If Err Then
        EndProgram aErrorCode
    End If
End Function

Function GetEmailBody()
    Dim location, lastBldLabel
    
    lastBldLabel = FindLabel("LB")
    If mOneStepBldChoice = "R" Then
        lastBldLabel = FindLabel("LR")
    End If

    location = GetOutputPath & ProductChoice
    Index = 1
    charTest = Mid(lastBldLabel, Index, 1)
    Do Until IsNumeric(charTest)
        Index = Index + 1
        charTest = Mid(lastBldLabel, Index, 1)
    Loop

    Dim buildNumber
    'buildNumber = Mid(lastBldLabel, Index, Len(lastBldLabel))
    buildNumber = Mid(lastBldLabel, InStr(lastBldLabel, ".") - 1, Len(lastBldLabel))
  
    Dim FileString
    For Index = 0 To fileNameCount - 1 Step 1
        FileString = FileString & "        " & mFileNameArray(Index) & vbNewLine
    Next
    
    Dim String1
    String1 = "Please review the change notes.  If you see any of your Change Requests in the notes, please verify that the Change Request is fixed." _
              & "  The change will be in the format of CR.FIX.###### where ###### is the Change Request number."
    If Project = "0" Then
        String1 = String1 & vbNewLine & vbNewLine & "Please upgrade to this version for daily use."
    End If
           
    If mOneStepBldChoice = "B" Then 'build
        GetEmailBody = mProductChoice & " " & buildNumber & " is at " & location & "." & vbNewLine & vbNewLine _
                 & "These are the changes since the last " & Left(buildNumber, InStrRev(buildNumber, ".") - 1) & " build." & vbNewLine _
                 & vbNewLine & "Read the attached files to see what has changed." & vbNewLine _
                 & "These notes are also located at:" & vbNewLine _
                 & "    " & GetOutputPath & "VersionDifferences\" & ProductChoice & vbNewLine & FileString _
                 & vbNewLine & vbNewLine & String1 & vbNewLine & vbNewLine _
                 & "If you have any questions, please contact me" _
                 & vbNewLine & vbNewLine & "Alan Chou"
    ElseIf mOneStepBldChoice = "R" Then  'release
        ftpLocation = GetOutputPath & "ftp\" & ProductName & Replace(buildNumber, ".", "_", 1, -1) & ".exe"
        GetEmailBody = "The version " & mProductChoice & " " & buildNumber & "is released" & vbNewLine _
                 & vbNewLine & "Please burn new cd's and send this version, located at:" & vbNewLine _
                 & location & vbNewLine & vbNewLine & "The self-extracting exe for the ftp site is at:" _
                 & vbNewLine & ftpLocation & vbNewLine & vbNewLine _
                 & "These are the changes since the last " & Left(buildNumber, InStrRev(buildNumber, ".") - 1) & " release." _
                 & vbNewLine & vbNewLine & "Read the attached files to see what has changed." & vbNewLine _
                 & "These notes are also located at:" & vbNewLine _
                 & "    " & GetOutputPath & "VersionDifferences\" & ProductChoice & vbNewLine _
                 & FileString & vbNewLine & vbNewLine & String1 & vbNewLine & vbNewLine _
                 & "If you have any questions, please contact me" _
                 & vbNewLine & vbNewLine & "Alan Chou"	
    End If

End Function

Function GetEmailSubject()
    Dim lastBldLabel, lastReleaseLabel
    
    If OneStepBldChoice = "B" Then 'build
         lastBldLabel = FindLabel("LB")
         Version = StripAllPrefix(lastBldLabel)
         GetEmailSubject = mProductChoice & " version " & Left(Version, InStrRev(Version, ".") - 1) & " build " & Right(Version, InStrRev(Version, ".")) & " is out for test"
    ElseIf OneStepBldChoice = "R" Then  'release
         lastReleaseLabel = FindLabel("LR")
         Version = StripAllPrefix(lastReleaseLabel)
         GetEmailSubject = mProductChoice & " version " & Left(Version, InStrRev(Version, ".") - 1) & " build " & Right(Version, InStrRev(Version, ".")) & " has been released"
    End If
End Function
        
Function WriteEmailToFile(Subject, Body)
    Dim TextStream1
    Set TextStream1 = mFSO.OpenTextFile(OutputBasePath & "EmailsToSend\email.txt", 2, True, 0)
    TextStream1.writeLine Subject
    TextStream1.writeLine vbNewLine & vbNewLine
    TextStream1.writeLine Body
    TextStream1.Close
End Function

Function SendEmail(Subject, Body, ReportPath, DescReportName, LabelReportName)
    WriteToDlgAndFile "Sending the email"
    Dim oSMTP
    Set oSMTP = wscript.CreateObject("SoftArtisans.SMTPMail") 'SMTPControl.SMTP")
    
    Error = False
    Err.Clear
    
    If mFSO.FileExists(ReportPath & DescReportName) And mFSO.FileExists(ReportPath & LabelReportName) Then
        With oSMTP
            .RemoteHost = "mail.corp.solimarsystems.com"
            .MailFromAddress = "achou@solimarsystems.com"
            .FromName = "achou@solimarsystems.com"
            .AddRecipient "all@solimarsystems.com"  '"emester@solimarsystems.com, jlan@solimarsystems.com" 'all@solimarsystems.com
            .Charset = 1
            .Subject = Subject
            .BodyText = Body
            .AddAttachment (ReportPath & DescReportName)
            .AddAttachment (ReportPath & LabelReportName)
            .SendMail
        End With
    Else
        Error = True
    End If
    If Err.Number <> 0 Then 'There was an error sending out the email
        Error = True
    End If
    If Error = True Then
        EndProgram eEmailSendError '"Error " & Err.Number & ": " & Err.Description
    End If

End Function

Function OutputErrorReport(aErrorCode)
    'create the error file
    Dim TextStream1
    Set TextStream1 = mFSO.OpenTextFile(mReportPath & "\logfolder\ReportError.txt", 2, True, 0)
    TextStream1.writeLine "An error has occured while running the report script: "
    
    Select Case (aErrorCode)
	  Case eReportFilesError
			TextStream1.writeLine "* There was an error with the email's attachment files: "
			TextStream1.writeLine "   " & mReportPath & DescReportName
			TextStream1.writeLine "   " & mReportPath & LabelReportName & vbNewLine
	  Case eEmailSendError
	        TextStream1.writeLine "* There was an error sending the email" & vbNewLine
	  Case eCancel
			TextStream1.writeLine "* The report script will not be started due to user cancelling the build process" & vbNewLine
    End Select
    OutputRemainingSteps aErrorCode, TextStream1
  
    If mDebug <> "Y" Then
            NetSendArry = Array("achou", "DKBZC11")
            For i = 0 To UBound(NetSendArry)
                mShell.Run "net send " & NetSendArry(i) & " There was an error when generating the report scripts." & vbNewLine & _
                         "Refer to " & mReportPath & "ReportError.txt" & " for an error log"
            Next
            TextStream1.Close
     End If
End Function

Function GetVersionNumFormatted()
'get the current build number, then format it and return
    Dim VersionNum
    VersionNum = GetLatestBuild(Project, "LB")
    VersionNum = StripAllPrefix(VersionNum) ' ,VersionPrefix)
    GetVersionNumFormatted = ChangeDotToLine(VersionNum)
End Function

 'Sequence of events:
 'delete built files from correct folder(next or current) if one step build script or release
 'copy files from Image Dir into the folder that was just emptied
 'check to make sure report script has the correct #'s
 'send out the email attaching the 2 files generated
 'if any of the steps were unable to complete put the results into error file & net send
  'message saying error occured
                
 'These variables indicate whether there's a "special" structure for copying the files from
 'the build into.  Normally the files would just be copied into a folder corresponding to
 'the product name but some of the products have a different file structure so if this is
 'the case, the variable will be set so the script knows to handle this case separately.
 'Const IConvertSpecialFS = True
    Sub CopyToNetworkAndSendEmail(g_Access)
       Dim ReportPath, InputPath, ReportOutputPath
       InputPath = mRootProjectDir & "\..\" 
       ReportPath = mReportPath + "\Reports\"               
       ReportOutputPath = GetOutputPath & "VersionDifferences\" & ProductChoice & "\"
       CopyReports ReportPath, ReportOutputPath, mFileNameArray, 2    'earlier the program copied the files to the report directory - double check this
                   
        If EmailChoice = "Y" And (OneStepBldChoice = "B" Or OneStepBldChoice = "R") Then 'copy files to correct folder                                           
                  SendEmail GetEmailSubject, GetEmailBody, ReportPath, mFileNameArray(0), mFileNameArray(1) 'fileNameArray hold report descript, label files
        End If

        'Generate a text file based on all the differences in the project from 4.04a to current
        If mGenAllRelease = "Y" Then
                g_Access.CreateAllReleaseDesc "reportAllReleaseDesc.txt"
        End If
 End Sub

Sub SetStartLabel()
                If mLastBuild = "Y" Then         'Last Build
                        mStartLabel = FindLabel("LBLB")
                End If
                If mLastRelease = "Y" Then       'Last Release
                        mStartLabel = FindLabel("PLR")
                End If
                If mBegDevPath = "Y" Then        'Beginning of Dev Path
                        mStartLabel = FindLabel("BDP")
                End If
                If mLastLabel = "Y" Then         'From Label
                        mStartLabel = FindLabel("LBL")                  
                End If
                If mLastBuildLabel = "Y" Then    'Last Build Label, only used by 1-Step-Build
                        mStartLabel = FindLabel("LBLB")            
                End If
End Sub

Sub GenerateTextFile(g_Access)
		Dim fileName
                WriteToDlgAndFile "Generating Reports"
                mProject = CheckProject(mFilePathArray(0), mPJFileArray(0))  'check to see if is a variant project or not
                mCancelDiag.UpdateDisplay "     Project=" & Project & "   0 = main trunk, 1 = var devpath"
                If EndLabel = "" Then
			mEndLabel = FindLabel("LB")
		End If
				
        mCancelDiag.UpdateDisplay "     LastBuildLabel: " & lastBldLabel

        If mLastBuild = "Y" Then      			   'Last Build
			CreateDescAndLabelReports mStartLabel, mEndLabel
        ElseIf mLastRelease = "Y" Then      		   'Last Release
			mEndLabel = FindLabel("LR")
			CreateDescAndLabelReports mStartLabel, mEndLabel
        ElseIf mBegDevPath = "Y" Then      			   'Beginning of Dev Path
            CreateDescAndLabelReports mStartLabel, mEndLabel
        ElseIf mLastLabel = "Y" And mLicenseSummary <> "Y" Then        		   'From Label
			CreateDescAndLabelReports mStartLabel, mEndLabel                    
        ElseIf mLastBuildLabel = "Y"  Then   		   'Last Build Label, only used by 1-Step-Build
                        CreateDescAndLabelReports mStartLabel, mEndLabel                   
		ElseIf mLicenseSummary = "Y" Then 'only for solimar licensing report that lists all labels using license version mStartLabel
			fileName = "LicenseSummary_" + ChangeDotToLine(g_Report.StartLabel) + ".txt"	
			g_Access.CreateTextFileLicenseLabels mStartLabel,  fileName 
			FormatLicenseSummary mReportPath  & "\Reports\" & fileName, mStartLabel
		Else						   'startlabel <> "" case
			CreateDescAndLabelReports StartLabel, EndLabel 	
        End If
               
    End Sub

Function IsLicenseAvailable(Prefix)
	Dim Products
	Dim i

	Products = Array("Creator", "SOLscript", "SPDE", "PdfEngine")
	IsLicenseAvailable = 0
	For i = 0 To UBound(Products)
		If Products(i) = Prefix Then
			IsLicenseAvailable = 1
			Exit For
		End If
	Next
End Function

'*****************************************************************
' Function Name : IsReleaseLabel
' Parameter     : Label - text with name and version
' Description   : Extracts version and name from Label and then 
'                 searches in the release file to see whether or
'                 not the label has been released.
'*****************************************************************
Function IsReleaseLabel(Label)
	Dim Prefix, Version, FileName
	Dim Line

 	Version = StripAllPrefix(Label)
	Prefix = Left(Label, Len(Label) - Len(Version))	'grab prefix for element name
	FileName = mReportPath & "\Product_Scripts\" & Prefix & "\Configuration\" & Prefix & "Release.txt"
	Set readFile = mFSO.OpenTextFile(FileName)

	IsReleaseLabel = 0	
	Do While Not readFile.AtEndOfStream
		Line = Trim(readFile.ReadLine)
		If Line = Label Then
		    IsReleaseLabel = 1
		    Exit Do
		End If		
	Loop
	readFile.Close
End Function

Sub FormatLicenseSummary(FileName, Label)
	Dim Header, Root
	Dim Output, Line, Prefix
	Output = Left(FileName, Len(FileName) - 4) & ".xml"
	Set readFile = mFSO.OpenTextFile(FileName)
	Set writeFile = mFSO.CreateTextFile(Output)
	
	Prefix =  ""
	Header =  "<?xml version=""1.0"" encoding=""ISO-8859-1""?>" & vbNewLine & _
			  "<?xml-stylesheet type=""text/xsl"" href=""LicenseSummary.xsl""?>"
	Root   =  "<Licensing Version=" & Chr(34) & StripAllPrefix(Label) & Chr(34) & ">"
	Body   =  ""
	'initial conditions
	Line = Trim(Replace(readFile.ReadLine, Chr(34), ""))
	Do While Not readFile.AtEndOfStream		
		Version = StripAllPrefix(Line)
		Prefix = Left(Line, Len(Line) - Len(Version))	'grab prefix for element name
		If (IsLicenseAvailable(Prefix) = 1) Then
			Body = Body & "<Product Name=" & Chr(34) &  Prefix & Chr(34) & ">" + vbNewLine 	
			Do While InStr(Line, Prefix) > 0 And Not readFile.AtEndOfStream			
				If IsReleaseLabel(Line) = 1 Then
					Body = Body + "<Label Released='true'>" & Version & "</Label>" & vbNewLine
				Else
					Body = Body + "<Label>" & Version & "</Label>" & vbNewLine
				End If
				Line = Trim(Replace(readFile.ReadLine, Chr(34), ""))	
				Version = StripAllPrefix(Line)
			Loop
			If readFile.AtEndOfStream Then		
				If IsReleaseLabel(Line) = 1 Then
					Body = Body + "<Label Released='true'>" & Version & "</Label>" & vbNewLine
				Else
					Body = Body + "<Label>" & Version & "</Label>" & vbNewLine
				End If
			End If
			Body = Body & "</Product>" + vbNewLine	 'close product element		
		Else
			Line = Trim(Replace(readFile.ReadLine, Chr(34), ""))
		End If	
	Loop	
	'close body
	EndRoot = "</Licensing>"

	readFile.Close
	writeFile.writeLine Header
	writeFile.writeLine Root
	writeFile.writeLine Body
	writeFile.WRiteLine EndRoot
	writeFile.Close
	
End Sub
    
Sub CreateDescAndLabelReports(StartLabel, EndLabel)
			mFileNameArray(mFileNameCount) = FileNamePrefix & "ReportDesc" & ChangeDotToLine(g_Report.StartLabel) & "To" & ChangeDotToLine(g_Report.EndLabel) & ".txt"
            If mFSO.FileExists(mFileNameArray(mFileNameCount)) Then
                        WriteToDlgAndFile "Report Description from " & StartLabel & " to " & EndLabel & " Already Exists"
            Else
						WriteToDlgAndFile "Creating Description Report file from: " + StartLabel + " to " + EndLabel
                        g_Access.CreateDescReportLabelToLabel StartLabel, EndLabel, mFileNameArray(mFileNameCount)
            End If
            mFileNameCount = mFileNameCount + 1
            mFileNameArray(mFileNameCount) = FileNamePrefix & "ReportLabel" & ChangeDotToLine(StartLabel) & "To" & ChangeDotToLine(EndLabel) & ".txt"
            If mFSO.FileExists(mFileNameArray(mFileNameCount)) Then
                        WriteToDlgAndFile "Report Label from " & StartLabel & " to " & EndLabel & " Already Exists"
            Else
                        WriteToDlgAndFile "Creating Label Report file from: " + StartLabel + " to " + EndLabel
                        g_Access.CreateLabelReportLabelToLabel StartLabel, EndLabel, mFileNameArray(mFileNameCount)
            End If
            mFileNameCount = mFileNameCount + 1
            WriteToDlgAndFile "Finished creating reports from " + StartLabel + " to " + EndLabel
End Sub

Function EndProgram(aErrorCode)
    If aErrorCode <> eCancel Then
        connection.Close
    End If
    endTime = Time
    mCancelDiag.UpdateDisplay "Time Ended: " & endTime
    mCancelDiag.UpdateDisplay "Time for Script to Run: " & getHrMinSec(startTime, endTime)
    mCancelDiag.ScriptDone False
    wscript.DisconnectObject mCancelDiag
    Set cancelDiag = Nothing
    Set writeFile = mFSO.CreateTextFile(mReportPath & "\ReportDone.txt")
    writeFile.writeLine CStr(errorcode)
    writeFile.Close

    If aErrorCode < 0 Then
        If mDebug <> "Y" Then
                NetSendArry = Array("achou", "DKBZC11")
                For i = 0 To UBound(NetSendArry)
                    Shell.Run "net send " & NetSendArry(i) & " Report Script has finished with no errors"
                Next
        End If
    Else
        WriteEmailToFile GetEmailSubject, GetEmailBody
        OutputErrorReport (aErrorCode)
    End If
    wscript.Quit 0
End Function

'Accessor Methods
    Function SetConnection(connection)
        Set mCmdTemp = connection.mCmdTemp
    End Function

    Function ProductDebug()
         ProductDebug = mDebug
    End Function
        
    Function Project()
        Project = mProject
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

    Public Property Get CancelDialog()
            Set CancelDialog = mCancelDiag
        End Property

    Public Property Get ExecInfo()
            Set ExecInfo = mExecInfo
        End Property

    Public Property Get SrcLabel()
            ProductName = mSrcLabel
        End Property

    Public Property Get LastRelease()
            LastRelease = mLastRelease
        End Property

    Public Property Get PrevLabel()
            PrevLabel = mPrevLabel
        End Property

    Public Property Get BegDevPath()
        BegDevPath = mBegDevPath
        End Property
    Public Property Get LastLabel()
        LastLabel = mLastLabel
    End Property

    Public Property Get PopDataBase()
        PopDataBase = mPopDataBase
    End Property
    
    Public Property Get GenTextFile()
        GenTextFile = mGenTextFile
    End Property
    
   Public Property Get startTime()
            startTime = mStartTime
   End Property

   Public Property Get RootProjectDir()
        RootProjectDir = mRootProjectDir
   End Property
   
   Public Property Get GenAllRelease()
        GenAllRelease = mGenAllRelease
   End Property

    Public Property Get ProductChoice()
        ProductChoice = mProductChoice
    End Property

    Public Property Get EmailChoice()
        EmailChoice = mEmailChoice
    End Property

    Public Property Get OneStepBldChoice()
        OneStepBldChoice = mOneStepBldChoice
    End Property
    
    Public Property Get StartLabel()
        StartLabel = mStartLabel
    End Property

    Public Property Get EndLabel()
        EndLabel = mEndLabel
    End Property
    
    Public Property Get ProductName()
        ProductName = mFilePathArray(2)
    End Property
    
    Public Property Get MainSrcProjPath()
		MainSrcProjPath = mProjPaths.mMainSrcProjPath
	End Property   
       
    Public Property Get MainProductName()
        MainProductName = mProjPaths.mMainProductName
    End Property

    Public Property Get MainProjectName()
        MainProjectName = mProjPaths.mMainProjectName
    End Property

    Public Property Get InstallSrcArchive()
       InstallSrcArchive = mProjPaths.mInstallSrcArchive
    End Property

    Public Property Get Archive()
       Archive = mProjPaths.mArchive
    End Property

    Public Property Get ReleaseFile()
       ReleaseFile = mProjPaths.mReleaseFile
    End Property

    Public Property Get FileNamePrefix()
       FileNamePrefix = mProjPaths.mFileNamePrefix
    End Property

    Public Property Get VersionPrefix()
        VersionPrefix = mProjPaths.mVersionPrefix
    End Property

End Class
Class CProjectPaths
    Public mMainProductName, mMainProjectName, mInstallSrcArchive, mArchive, mMainSrcProjPath
    Public mReleaseFile, mFileNamePrefix, mVersionPrefix
 
    Public Sub SetMainProductName(aMainProductName)
        mMainProductName = aMainProductName
    End Sub

    Public Sub SetMainProjectName(aProjName)
        mMainProjectName = aProjName
    End Sub

    Public Sub SetInstallSrcArchive(aInstallSrcArchive)
        mInstallSrcArchive = aInstallSrcArchive
    End Sub

    Public Sub SetArchive(aArchive)
        mArchive = aArchive
    End Sub

    Public Sub SetReleaseFile(aRelease)
        mReleaseFile = aRelease
    End Sub
 
    Public Sub SetFileNamePrefix(aFileNamePrefix)
        mFileNamePrefix = mVersionPrefix
    End Sub

    Public Sub SetVersionPrefix(aVersionPrefix)
        mVersionPrefix = aVersionPrefix
    End Sub

	Public Sub SetMainSrcProjPath(aProjPath)
		mMainSrcProjPath = aProjPath
	End Sub
End Class



