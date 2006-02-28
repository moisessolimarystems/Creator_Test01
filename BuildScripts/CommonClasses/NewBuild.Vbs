Class CNewBuild
    Public mbCleanedIS, mbLabeledMain, mCodeBuildType, mExistingDevPath, mVersionNum, mBuildLabel, mLicenseVersion  
    Public mBuild, mInputFile

    Public Sub Class_Initialize() 'set default values
	  mExistingDevPath = Null       
    End Sub

    Public Property Set Build(ByRef aBuild)
        Set mBuild = aBuild
    End Property

    'Checks to make sure all the necessary release params were passed in
    Public Function CheckParams()
        Dim bWasError
        bWasError = False
        If mVersionNum = "" Then
            bWasError = True
            mBuild.WriteToDlgAndFile "ERROR: No value was given for parameter -VN (Version Number).  Please pass in a valid version number for this build"
        End If
        If mBuild.ProductName = "Solscript" Or mBuild.ProductName = "SpdNT" And mCodeBuildType <> "FB" And mCodeBuildType <> "MK" Then '%%%i think only solscript uses the make or full build options, but make sure this is correct***
            bWasError = True
            mBuild.WriteToDlgAndFile "ERROR: -CBT (Code build type) was invalid." + _
                                     "Please pass in a valid option: FB for a full build, or MK for make."
        End If
        CheckParams = bWasError
    End Function


    'Accessor Methods
    Public Property Get CodeBuildType()
            CodeBuildType = mCodeBuildType
        End Property

    Public Property Get ExistingDevPath()
            ExistingDevPath = mExistingDevPath
        End Property

    Public Property Get VersionNum()
            VersionNum = mVersionNum
        End Property

    Public Property Get BuildLabel()
            BuildLabel = mBuildLabel
        End Property
	
	Public Property Get LicenseVersion()
			LicenseVersion = mLicenseVersion
	End Property

	Public Property Let LicenseVersion(aLicenseVersion)
			mLicenseVersion = aLicenseVersion
	End Property
	
    Public Property Let BuildLabel(aBuildLabel)
            mBuildLabel = aBuildLabel
        End Property


        Public Property Let CleanedIS(abCleanedIS)
            mbCleanedIS = abCleanedIS
        End Property

    Public Property Let LabeledMain(abLabeledMain)
            mbLabeledMain = abLabeledMain
        End Property

    Public Property Let CheckPointedMain(abCheckPointedMain)
            mbCheckPointedMain = abCheckPointedMain
        End Property



    Function GetProjType()
        
          If mExistingDevPath = "" Then
              GetProjType = "Trunk"
          Else
              GetProjType = "Branch"
        End If

    End Function


    Function UndoBuildChanges()
            mBuild.WriteToDlgAndFile "Backing out of Script"
                If mbCleanedIS Then  'And mbCheckPointedMain Then    'only need to rebuild if we already checkpointed the project
                        mBuild.WriteToDlgAndFile "Rebuilding Install Source"
                        RecreateInstallSourceSandbox
                End If
                If mbLabeledMain Then
                        Dim MKS
                        Set MKS = New CMKS
                        Set MKS.Build = mBuild
                        mBuild.WriteToDlgAndFile "Deleting labels from Main Project"
                        MKS.UndoLabel mBuildLabel
                        Set MKS = Nothing
                End If
'               If mbCheckPointedMain Then
'                       mBuild.WriteToDlgAndFile "Deleting revision from Main Project"
'                       mBuild.RunCmd ("rcs -y -o" & mBuildLabel & " " & mBuild.MainSrcRCSPJFile)
'               End If

    End Function


    Private Function RecreateInstallSourceSandbox()
                mBuild.WriteToDlgAndFile "recreating installsource"

                If (mBuild.FSO.FolderExists(mBuild.InstallSrcPath) = True) Then
                        mBuild.FSO.DeleteFile mBuild.InstallSrcPath, 1
                        mBuild.FSO.DeleteFolder mBuild.InstallSrcPath, 1
                End If
                mBuild.FSO.CreateFolder (mBuild.InstallSrcPath)

                If GetProjType = "Trunk" Then
                        mBuild.RunCmd "pj sandbox -S " + Chr(34) + mBuild.InstallSrcPJFile + Chr(34) + " -g -y -i -P " + Chr(34) + mBuild.SrcInstallSrcPJ + Chr(34)
                Else
                        mBuild.RunCmd "pj sandbox -S " + Chr(34) + mBuild.InstallSrcPJFile + Chr(34) + " -g -y -i -N " + mExistingDevPath + " -P " + Chr(34) + mBuild.SrcInstallSrcPJ + Chr(34)
                End If
    End Function


    Sub CreateBuildVersionTextFileInImageDir()
        Dim VersionNumberFormatted, TextFileName
        VersionNumberFormatted = Replace(mVersionNum, ".", "_", 1, -1)
        TextFileName = "Version - " & mBuild.ProductName & "_" & VersionNumberFormatted & ".txt"
        mBuild.FSO.CreateTextFile mBuild.ImageDirPath & "\" & TextFileName
        End Sub

    Sub CreateLicenseVersionTextFileInImageDir()
        Dim VersionNumberFormatted, TextFileName
        VersionNumberFormatted = Replace(mLicenseVersion, ".", "_", 1, -1)
        TextFileName = "Version - LicenseBuild_" & VersionNumberFormatted & ".txt"
        mBuild.FSO.CreateTextFile mBuild.ImageDirPath & "\Common\SolimarLicensing\" & TextFileName
    End Sub	

    Sub IncrementBuildNum(aPattern, aMKS)
    
           Dim output, readFile, writeFile, BuildScriptPJ
           output = mBuild.mBuildPath & "\temp.txt"
           mInputFile = mBuild.mBuildPath & "\BuildRegistry.txt"
           'aMKS.RefreshFile mBuild.mBuildPath & "\buildScript.pj", mInputFile
	   aMKS.LockFiles mBuild.mBuildPath & "\buildScript.pj", mInputFile
           mBuild.RunCmd "attrib -r " & Chr(34) + mInputFile + Chr(34)
           
           Set readFile = mBuild.FSO.OpenTextFile(mInputFile)
           Set writeFile = mBuild.FSO.CreateTextFile(output)
           Dim check, Line, buildNum
           check = 0
          
           Do While Not readFile.AtEndOfStream
                    Line = readFile.ReadLine
                    If InStr(Line, aPattern) <> 0 And check = 0 Then
                            check = 1
                            pos = InStr(Line, " ")
                            buildNum = Right(Line, Len(Line) - pos) + 1
                            aPattern = aPattern & " " & buildNum
                            writeFile.writeLine aPattern
                    Else
                            writeFile.writeLine Line
                    End If
           Loop
           mBuild.WriteToDlgAndFile "Incremented build number to be " & buildNum & " in " & mInputFile
           writeFile.Close
           readFile.Close
           mBuild.FSO.DeleteFile mInputFile, 1
           mBuild.FSO.MoveFile output, mInputFile
           aMKS.CheckInFiles mBuild.mBuildPath & "\buildScript.pj", mInputFile

      End Sub

End Class
