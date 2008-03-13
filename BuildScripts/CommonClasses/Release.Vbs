
Class CRelease
    Public mBuild, mFormat, mReleaseCandidatePath
 
    Public Sub Class_Initialize() 'set default values
        Set mFormat = New CFormatting
        If g_DEBUG Then
			mReleaseCandidatePath = "I:\Alan\release_candidates\"
        Else
			mReleaseCandidatePath = "Z:\Release_Candidates\"
        End If
    End Sub
    
    Private Sub Class_Terminate()  'this gets called when the CProductBuild object gets deleted
		Set mFormat = Nothing
    End Sub    
   
    Public Property Set Build(ByRef aBuild)
        Set mBuild = aBuild
    End Property
    

    'Checks to make sure all the necessary release params were passed in
    Public Function CheckParams()
        Dim bWasError
        bWasError = False

        If mBuild.PrevLabel = "" Then
            bWasError = True
            mBuild.WriteToDlgAndFile "ERROR: You are trying to do a release but did not pass in a value for -PL (Previous Label)"
        End If
      
        CheckParams = bWasError
 
    End Function
        
    '=========================================================================
    ' Function    : AddReleaseToMKS
    ' Parameter   : aMKS - Handle to MKS class
    '               aMoveFiles - Handle to MoveFile class,
    '               aVersionNum - release version number
    ' Description : Check in release ftp executable into MKS.
    '               Deleting all other ftp executables and folders of the same
    '               version, except the Release. This is done to maintain the
    '               release sandbox on the S:\ drive
    '=========================================================================
    Function AddReleaseToMKS(aMKS, aMoveFiles, aVersionNum)
        Dim ReleaseFilePath, versionNum
        Dim Folder1, SubFolders1, Files1, f1
        ReleaseFilePath = mReleaseCandidatePath & mBuild.ProductName & "\"
        versionNum = Replace(aVersionNum, ".", "_", 1, 2, 1) 'substitute decimals for underscores
        mBuild.WriteToDlgAndFile "Adding FTP executable " & mBuild.ProductName & versionNum & ".exe to MKS"
        'Need to archive and label???
        Set Folder1 = mBuild.FSO.GetFolder(ReleaseFilePath)
        Set SubFolders1 = Folder1.SubFolders
        For Each f1 In SubFolders1
                If InStr(f1.Name, aVersionNum) = 0 And InStr(f1.Name, mFormat.VersionToMajorMinor(aVersionNum)) > 0 Then
                    aMoveFiles.DeleteFileOrFolder "Folder",  f1, True
                End If
        Next
        Set Files1 = Folder1.Files
        For Each f1 In Files1
            If InStr(f1.Name, versionNum) > 0 Then   'possible to have multiple ftp exe
				aMKS.AddFiles ReleaseFilePath & mBuild.ProductName & ".pj", Chr(34) & f1 & Chr(34)
            Else
                If InStr(f1.Name, ".pj") = 0 And InStr(f1.Name, mFormat.VersionToMajorMinor(aVersionNum)) > 0 Then
                        aMoveFiles.DeleteFileOrFolder "File",  Chr(34) & f1 & Chr(34) , True
                End If
            End If
        Next
    End Function
        
    Function UpdateReleaseFile(aMKS, aReleaseFile)
        mBuild.WriteToDlgAndFile "Updating Release File: " & aReleaseFile
      
                Dim output, pjFile, releaseFilePath
                output = "temp"
                releaseFilePath = mBuild.Shell.CurrentDirectory & "\Product_Scripts\" & mBuild.ProductName & "\"
                pjFile = releaseFilePath & "Configuration\Configuration.pj" 'mBuild.ProductName & ".pj"
				aMKS.CheckOutFiles pjFile, releaseFilePath & aReleaseFile
				
                mBuild.RunCmd "attrib -r " & Chr(34) + releaseFilePath & aReleaseFile + Chr(34)

                Dim readFile, writeFile, Line
                Set readFile = mBuild.FSO.OpenTextFile(releaseFilePath & aReleaseFile)
                Set writeFile = mBuild.FSO.CreateTextFile(releaseFilePath & output)
                Do While Not readFile.AtEndOfStream
                        Line = readFile.ReadLine
                        writeFile.writeLine Line
                Loop
                writeFile.writeLine mBuild.PrevLabel
                writeFile.Close
                readFile.Close
                mBuild.FSO.DeleteFile releaseFilePath & aReleaseFile, 1
                mBuild.FSO.MoveFile releaseFilePath & output, releaseFilePath & aReleaseFile
				aMKS.CheckInFiles pjFile, releaseFilePath & aReleaseFile
    End Function

    Sub CopySelfExtractingExeToNetwork(aFileName)
                Dim MoveFiles, OutputPath
                Set MoveFiles = New CMoveFiles
                Set MoveFiles.Build = mBuild

                OutputPath = MoveFiles.NetworkBasePath + "CurrentVersion\release\FTP\"
				MoveFiles.CopyFileOrFolder "File", mBuild.MainProjPath + "\FTP\" + aFileName, OutputPath, True
                Set MoveFiles = Nothing
    End Sub
        
    Sub CreateBuildVersionTextFileInImageDir(versionNum)
        Dim VersionNumberFormatted, TextFileName
        VersionNumberFormatted = Replace(versionNum, ".", "_", 1, -1)
        TextFileName = "Version - " & mBuild.ProductName & "_" & VersionNumberFormatted & ".txt"
        mBuild.FSO.CreateTextFile mBuild.ImageDirPath & "\" & TextFileName
    End Sub

End Class


