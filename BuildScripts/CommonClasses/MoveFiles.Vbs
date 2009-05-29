Class CMoveFiles
 
    Public mNetworkBasePath, mBuild, mFormat, mReleaseCandidatePath

    Public Sub Class_Initialize() 'set default values
       		Set mFormat = New CFormatting
       		If g_DEBUG Then
				mNetworkBasePath = "I:\Alan\cd_images\"
                mReleaseCandidatePath = "I:\Alan\release_candidates\"
			Else
				mNetworkBasePath = "L:\cd_images\"
				mReleaseCandidatePath = "Z:\Release_Candidates\"	
			End If
    End Sub
    
    Private Sub Class_Terminate()  'this gets called when the CProductBuild object gets deleted
		Set mFormat = Nothing
    End Sub
    
    Public Property Set Build(ByRef aBuild) 
        Set mBuild = aBuild
		If aBuild.EngOption <> "N" Then
			mNetworkBasePath = "L:\cd_images\Engineering Builds\"
		End If
    End Property        

    Public Property Get NetworkBasePath()
        NetworkBasePath = mNetworkBasePath
    End Property 

    Public Property Get ReleaseCandidatePath()
		ReleaseCandidatePath = mReleaseCandidatePath
    End Property

    'Move ImageDir into a temporary folder on network	
    'Then move everything to correct folder name if successful
    Sub MoveCDImageToNetwork(aDestFolder, aDestFolderName)
		Dim DestPath, tempDestPath
		tempDestPath =  GetOutputPath + aDestFolder + "\Temp"
        DestPath = GetOutputPath + aDestFolder + "\" + aDestFolderName
		mBuild.WriteToDlgAndFile "Copying CD image files to the network" 
        mBuild.WriteToDlgAndFile "Creating: " & tempDestPath
		CreateFileOrFolder "Folder", tempDestPath, True 
		Set Folder1 = mBuild.FSO.GetFolder(mBuild.ImageDirPath)
        Set SubFolders1 = Folder1.SubFolders
        If SubFolders1.Count <> 0 Then
            CopyFileOrFolder "Folder", mBuild.ImageDirPath & "\*", tempDestPath & "\", True
        End If
        Set Files1 = Folder1.Files
        If Files1.Count <> 0 Then
            CopyFileOrFolder "File", mBuild.ImageDirPath & "\*", tempDestPath & "\", True
        End If
        If mBuild.FSO.FolderExists(DestPath) Then 
			DeleteFileOrFolder "Folder", DestPath, True
		End If
		CreateFileOrFolder "Folder", DestPath, True
		Set Folder1 = mBuild.FSO.GetFolder(tempDestPath)
        Set SubFolders1 = Folder1.SubFolders
        If SubFolders1.Count <> 0 Then
			MoveFileOrFolder "Folder", tempDestPath & "\*", DestPath & "\"
		End If
		Set Folder1 = mBuild.FSO.GetFolder(tempDestPath)
		Set Files1 = Folder1.Files
		If Files1.Count <> 0 Then
			MoveFileOrFolder "File", tempDestPath & "\*", DestPath & "\"
		End If
		DeleteFileOrFolder "Folder", tempDestPath, True
    End Sub

    Sub MoveCDImageToImageDir(aImageSourcePath)
        mBuild.WriteToDlgAndFile "Moving Files to Image Directory"
        If (mBuild.FSO.FolderExists(mBuild.ImageDirPath) = False) Then
       		 'mBuild.FSO.CreateFolder (mBuild.ImageDirPath)
		 CreateFileOrFolder "Folder", mBuild.ImageDirPath, True
        End If

        Dim Folder1, SubFolders1, Files1
	    Set Folder1 = mBuild.FSO.GetFolder(aImageSourcePath)
        Set SubFolders1 = Folder1.SubFolders
        If SubFolders1.Count <> 0 Then
            CopyFileOrFolder "Folder", aImageSourcePath & "\*", mBuild.ImageDirPath & "\", True
        End If
        Set Files1 = Folder1.Files
        If Files1.Count <> 0 Then
            CopyFileOrFolder "File", aImageSourcePath & "\*", mBuild.ImageDirPath & "\", True
        End If
    End Sub

	Function MakeDir (strPath)
		Dim strParentPath
		Err.Clear
		On Error Resume Next		
		strParentPath = mBuild.FSO.GetParentFolderName(strPath)
		If Not mBuild.FSO.FolderExists(strParentPath) Then MakeDir strParentPath
		If Not mBuild.FSO.FolderExists(strPath) Then mBuild.FSO.CreateFolder strPath
		If Err Then
		    mBuild.BuildError "Error attempting to create " + strPath
		    mBuild.WriteToDlgAndfile Err.Description
		End If
		MakeDir = mBuild.FSO.FolderExists(strPath)
	End Function
    
    Sub DeleteFolderContents(aFolderPath)
        Dim Folder, SubFolders, Files
		Set Folder = mBuild.FSO.GetFolder(aFolderPath)
        Set SubFolders = Folder.SubFolders
		For Each f1 in SubFolders	 
			DeleteFileOrFolder "Folder", f1, True 
		Next

        Set Files = Folder.Files
		For Each f1 in Files	 
            DeleteFileOrFolder "File", f1, True 
		Next
    End Sub

    Public Function GetOutputPath()
		If mBuild.BuildType = "R" Then    'Release
		    GetOutputPath = mNetworkBasePath & "CurrentVersion\release\"  
		Elseif mBuild.BuildType = "NB" Then   'New Build
			If mBuild.NewBuild.GetProjType = "Branch" Then 
				GetOutputPath = mNetworkBasePath & "CurrentVersion\test\"
			Else  'Trunk
				GetOutputPath = mNetworkBasePath & "NextVersion\test\"
 			End If
		Else
		    mBuild.BuildError "Internal error in CMoveFiles.GetOutputPath.  Invalid value for mBuild.BuildType.  The value is: " + mBuild.BuildType 
		End If
    End Function

    Public Function DeleteFileOrFolder(aDeleteType, aPath, aOverride)
        mBuild.WriteToDlgAndFile "Deleting: " & aPath
		Err.Clear
		On Error Resume Next
        If aDeleteType = "File" Then
            mBuild.FSO.DeleteFile aPath, aOverride
		Else 
			mBuild.FSO.DeleteFolder aPath, aOverride  'remove contents of the folder
		End If
		If Err Then
			mBuild.BuildError "Error attempting to delete " + aDeleteType + ": " + aPath
			mBuild.WriteToDlgAndfile Err.Description
		End If
    End Function

    Public Function CreateFileOrFolder(aCreateType, aPath, aOverride)
        Dim retVal
        retVal = True
        mBuild.WriteToDlgAndFile "Creating: " & aPath
	    Err.Clear
	    On Error Resume Next
        If aCreateType = "File" Then
	        mBuild.FSO.CreateTextFile aPath, aOverride
		Else 
	        retVal = MakeDir(aPath)  'remove contents of the folder
		End If
		If Err Or retVal = False Then
		    mBuild.BuildError "Error attempting to create " + aCreateType + ": " + aPath
		    mBuild.WriteToDlgAndfile Err.Description
		End If
    End Function	

    'Had to put the copying of files and folders into a seperate function so On Error Resume Next would be isolated
    'only to this Function
    Public Function CopyFileOrFolder(aCopyType, aSourcePath, aDestinationPath, aOverRide)
       mBuild.WriteToDlgAndFile "Copying " + aCopyType + " from: " + aSourcePath + " to: " & aDestinationPath

       Err.Clear
       On Error Resume Next
       If aCopyType = "File" Then
	       mBuild.FSO.CopyFile aSourcePath, aDestinationPath, aOverRide
       Elseif aCopyType = "Folder" Then 'copy folder instead
	       mBuild.FSO.CopyFolder aSourcePath, aDestinationPath, aOverRide
	   Else
	       BuildError "An invalid parameter: " + aCopyType + " was passed in to CMoveFiles.CopyFileOrFolder"
       End If
   
       If Err Then
	       mBuild.BuildError "Error Attempting to copy " + aCopyType + " from " + aSourcePath + " to " + aDestinationPath 
	       mBuild.WriteToDlgAndfile Err.Description
       End If
    End Function

    'Had to put the moving of files and folders into a seperate function so On Error Resume Next would be isolated
    'only to this Function
    Public Function MoveFileOrFolder(aMoveType, aSourcePath, aDestinationPath)
       mBuild.WriteToDlgAndFile "Moving " + aMoveType + " from: " + aSourcePath + " to: " & aDestinationPath

       Err.Clear
       On Error Resume Next
       If aMoveType = "File" Then
	       mBuild.FSO.MoveFile aSourcePath, aDestinationPath
       Elseif aMoveType = "Folder" Then 'copy folder instead
	       mBuild.FSO.MoveFolder aSourcePath, aDestinationPath
	   Else
	       BuildError "An invalid parameter: " + aMoveType + " was passed in to CMoveFiles.MoveFileOrFolder"
       End If
   
       If Err Then
	       mBuild.BuildError "Error Attempting to Move " + aMoveType + " from " + aSourcePath + " to " + aDestinationPath 
	       mBuild.WriteToDlgAndfile Err.Description
       End If
    End Function

    '=================================================================================
    ' Function    : MoveCDImageFTPToRCPath
    ' Parameters  : aDestFolder - product name
    '               aDestFolderName - product version number 
    '               aFileName - ftp executable name
    ' Description : Copies built cd image from build directory to products release
    '               candidate folder on R:\Build_Machine\Release_Candidate.
    '      	    Also copies FTP executable to product folder on Z:\
    '=================================================================================	
    Sub MoveCDImageFTPToRCPath(aDestFolder, aDestFolderName)
		Dim DestPath, tempDestPath, f1
        DestPath = mReleaseCandidatePath + aDestFolder + "\" + aDestFolderName
		FTPDestPath = mReleaseCandidatePath + aDestFolder + "\" 
		mBuild.WriteToDlgAndFile "Copying CD image files to the Release_Candidate Folder" 
        If mBuild.FSO.FolderExists(DestPath) Then 
			DeleteFileOrFolder "Folder", DestPath, True
		End If
        mBuild.WriteToDlgAndFile "Creating: " & DestPath  
		CreateFileOrFolder "Folder", DestPath, True 
		Set Folder1 = mBuild.FSO.GetFolder(mBuild.ImageDirPath)
        Set SubFolders1 = Folder1.SubFolders
        If SubFolders1.Count <> 0 Then
            CopyFileOrFolder "Folder", mBuild.ImageDirPath & "\*", DestPath & "\", True
        End If
        Set Files1 = Folder1.Files
        If Files1.Count <> 0 Then
            CopyFileOrFolder "File", mBuild.ImageDirPath & "\*", DestPath & "\", True
        End If
		Set Folder1 = mBuild.FSO.GetFolder(mBuild.MainProjPath + "\FTP")
		Set Files1 = Folder1.Files
		For Each f1 in Files1	 
		    If InStr(f1.Name, ".exe") > 0 Then	'possibly to have multiple ftp exe
			    CopyFileOrFolder "File", f1, FTPDestPath, True  
		    End If
		Next
    End Sub

    '=================================================================================
    ' Function    : MoveCDImageFTPToNetwork
    ' Parameters  : aDestFolder - product name
    '               aDestFolderName - product version number 
    '               aFileName - ftp executable name
    ' Description : Copies built ftp from build directory to network          
    '      	    
    '=================================================================================	
    Sub MoveCDImageFTPToNetwork()
	Dim f1
	Set Folder1 = mBuild.FSO.GetFolder(mBuild.MainProjPath + "\FTP")
	Set Files1 = Folder1.Files
	For Each f1 in Files1	 
	    If InStr(f1.Name, ".exe") > 0 Then	'possibly to have multiple ftp exe
		    CopyFileOrFolder "File", f1, GetOutPutPath & "FTP\", True  
	    End If
	Next
    End Sub

    '===============================================================================
    ' Function    : MoveRCPathToNetwork
    ' Parameters  : aVersionNum - version number of product to look for in RC folder
    '  		    aProductName - product name to determine what cd image to look for	
    ' Description : Uses version number to determine what RC folder to look for.
    '               FTP exe has unique file name ProductName + VersionNumber + .exe 
    '		    Copies build to release folder and ftp exe to FTP folder.	
    '===============================================================================	
    Sub MoveRCPathToNetwork(aProductName, aVersionNum)
		Dim DestPath, SrcPath, FTPSrcPath, VersionNum, f1
		DestPath = GetOutputPath + aProductName + "\" + mFormat.VersionToMajorMinor(aVersionNum)	
		SrcPath = mReleaseCandidatePath + aProductName + "\" + aVersionNum
		FTPSrcPath = mReleaseCandidatePath & aProductName  
		If mBuild.FSO.FolderExists(DestPath) Then 
			DeleteFileOrFolder "Folder", DestPath, True
		End If
		mBuild.WriteToDlgAndFile "Creating: " & DestPath
		CreateFileOrFolder "Folder", DestPath, True
		Set Folder1 = mBuild.FSO.GetFolder(SrcPath)
        Set SubFolders1 = Folder1.SubFolders
        If SubFolders1.Count <> 0 Then
            CopyFileOrFolder "Folder", SrcPath & "\*", DestPath & "\", True
        End If
        Set Files1 = Folder1.Files
        If Files1.Count <> 0 Then
            CopyFileOrFolder "File", SrcPath & "\*", DestPath & "\", True
        End If
		VersionNum = Replace(aVersionNum, ".", "_", 1, 2, 1) 'substitute decimals for underscores
		Set Folder1 = mBuild.FSO.GetFolder(FTPSrcPath)
		Set Files1 = Folder1.Files
		For Each f1 in Files1
			If InStr(f1.Name, VersionNum) > 0 Then  'possible to have multiple ftp exe	
				CopyFileOrFolder "File", f1, GetOutPutPath & "FTP\", True
			End If 
		Next
    End Sub    
End Class

