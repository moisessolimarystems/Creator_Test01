
Class CMKS
    Public mBuild
    Public Property Set Build(ByRef aBuild)
        Set mBuild = aBuild
        'mBuild.RunCmd "si connect --hostname=mksapp --port=7001 --user=mksuser --password=W3lc0m3 --quiet"
    End Property
    
    Function CkPtAndLabel(aLabel)
       Dim output 
       mBuild.WriteToDlgAndFile "Checkpointing and labeling project with " & aLabel
        If Not g_DEBUG Then
            'mBuild.NewBuild.CheckPointedMain = True
            output = mBuild.RunCmd("si checkpoint --label=" & aLabel & " --labelMembers --recurse --quiet --sandbox=" & _
                          Chr(34) & mBuild.MainPJFile & Chr(34) & " --description=build_" & aLabel)
	    If (InStr(output, "Failed") <> 0) Then
		mBuild.BuildError "CheckPoint failed! Check in project RCS folders for any locked files( filenames starting with VC_ or _)" & output
	    End If		  
            mBuild.NewBuild.LabeledMain = True
        End If
    End Function

    Function LabelFile(aProject, aLabel, aFile)
        mBuild.WriteToDlgAndFile "Checkpointing and labeling project with " & aLabel
        If Not g_DEBUG Then
            
            mBuild.RunCmd "si addlabel --yes --quiet --recurse --sandbox=" & Chr(34) & aProject & Chr(34) & _
                          " --label=" & aLabel & " " & aFile
        End If
    End Function

    Sub UndoLabel(aLabel)
        mBuild.WriteToDlgAndFile "Undoing labeling of project with label " + aLabel
                If Not g_DEBUG Then
                    mBuild.RunCmd ("si deletelabel --yes --quiet --recurse --sandbox=" + Chr(34) + mBuild.MainPJFile _
                                   + Chr(34) + " --label=" + aLabel)
        End If
    End Sub

'===================================================================================================================
' Function     : GenerateSandBox
' Parameters   : aType - the type of sandbox that should be generated("New" - variant or trunk, "Build")
' Description  : "Resync" - resynchronize and restore the timestamp to a sandbox
'                "New" - Drops registered sandbox if exists, creates a nonpopulated sandbox, then resynchronizes to
'                        restore sandbox members with correct timestamp
'                "Build" - Drops registered sandbox if exists, creates a nonpopulated build sandbox, then resynchronizes
'                          to restore sandbox members with correct timestamp
'==================================================================================================================
    Function GenerateSandBox(aType)
                Dim Result
                If g_DEBUG Then
                    Result = MsgBox("Would you like to generate a new sandbox?", vbYesNo, "Generate Sandbox dialog")
        End If

                If Not g_DEBUG Or Result = vbYes Then
                        Select Case aType
                        Case "Resync"   'ReSynch current sandbox
                                mBuild.WriteToDlgAndFile "Resynching main project sandbox"
                                mBuild.RunCmd ("si resync --overwriteChanged --restoreTimestamp --quiet --recurse --sandbox=" _
                                                + Chr(34) + mBuild.MainPJFile + Chr(34))
                        Case "New"   'Create a new sandbox from the trunk
                                CleanFolder mBuild.MainProjPath
                                DropSandbox ("New")
                                If mBuild.NewBuild.GetProjType = "Trunk" Then
                                        mBuild.WriteToDlgAndFile "Creating new sandbox from trunk"
                                        mBuild.RunCmd ("si createsandbox --quiet --recurse --nopopulate --project=" + Chr(34) + mBuild.MainSrcPJFile + _
                                        Chr(34) + " " + Chr(34) + mBuild.MainProjPath + Chr(34))
                                        mBuild.RunCmd ("si resync --yes --overwriteChanged --restoreTimestamp --quiet --recurse --sandbox=" _
                                                + Chr(34) + mBuild.MainPJFile + Chr(34))
                                Else
                                        mBuild.WriteToDlgAndFile "Creating new sandbox from existing development path"
                                        mBuild.RunCmd ("si createsandbox --quiet --recurse --nopopulate --devpath=" & mBuild.NewBuild.ExistingDevPath & _
                                                        " --project=" + Chr(34) + mBuild.MainSrcPJFile + Chr(34) + " " + Chr(34) + _
                                                        mBuild.MainProjPath + Chr(34))
                                        mBuild.RunCmd ("si resync --yes --overwriteChanged --restoreTimestamp --quiet --recurse --sandbox=" _
                                                + Chr(34) + mBuild.MainPJFile + Chr(34))
                                End If
                        Case "Build"
                                DropSandbox ("Build")
                                mBuild.WriteToDlgAndFile "Creating a build sandbox from label: " + mBuild.PrevLabel
                                CleanFolder mBuild.MainProjPath
                                mBuild.RunCmd ("si createsandbox --quiet --recurse --nopopulate --projectRevision=" + mBuild.PrevLabel + " --project=" + _
                                              Chr(34) + mBuild.MainSrcPJFile + Chr(34) + " " + Chr(34) + mBuild.MainProjPath + Chr(34))
                                mBuild.RunCmd ("si resync --yes --overwriteChanged --restoreTimestamp --quiet --recurse --sandbox=" _
                                                + Chr(34) + mBuild.MainPJFile + Chr(34))
			Case "Branch"
			        mBuild.WriteToDlgAndFile "Creating new sandbox from existing development path"
                                mBuild.RunCmd ("si createsandbox --quiet --recurse --nopopulate --devpath=" & mBuild.NewDevPath & _
                                               " --project=" + Chr(34) + mBuild.MainSrcPJFile + Chr(34) + " " + Chr(34) + _
                                                mBuild.MainProjPath + Chr(34))
                                mBuild.RunCmd ("si resync --yes --overwriteChanged --restoreTimestamp --quiet --recurse --sandbox=" _
                                                + Chr(34) + mBuild.MainPJFile + Chr(34))
                        End Select
                End If
    End Function
    
    Function GenerateSubProjectSandbox(aMainPJFile, aMainSrcPJFile, aSandboxPath)
		 mBuild.WriteToDlgAndFile "Creating a build sandbox from label: " + mBuild.PrevLabel
         CleanFolder mBuild.MainProjPath
         mBuild.RunCmd ("si createsandbox --recurse --nopopulate --projectRevision=" + mBuild.PrevLabel + " --project=" + _
                         Chr(34) + aMainSrcPJFile + Chr(34) + " " + Chr(34) + aSandboxPath + Chr(34))
         mBuild.RunCmd ("si resync --yes --overwriteChanged --restoreTimestamp --quiet --recurse --sandbox=" _
                         + Chr(34) + aSandboxPath & "\" & aMainPJFile + Chr(34))
    End Function 
    

    Sub CreateNewDevPath(aProjPath, aPJFile)
        mBuild.WriteToDlgAndFile "Creating new development path " + mBuild.NewDevPath + " from label " + mBuild.PrevLabel + _
                                         " in sandbox " + aProjPath
        If Not g_DEBUG Then
                    CleanFolder aProjPath
                mBuild.RunCmd ("si createdevpath --quiet --projectRevision=" + mBuild.PrevLabel + " --devpath=" & mBuild.NewDevPath + _
                               " --project=" + Chr(34) + mBuild.MainSrcPJFile + Chr(34))
        End If
    End Sub

    Private Sub CleanFolder(aPath)
        If mBuild.FSO.FolderExists(aPath) Then
                mBuild.FSO.DeleteFile aPath, 1
                    mBuild.FSO.DeleteFolder aPath, 1
            End If
            mBuild.FSO.CreateFolder aPath
    End Sub

    Sub LockFiles(aPJFile, aLockFile)
        mBuild.WriteToDlgAndFile "Locking pj " + aPJFile + ", File: " + aLockFile
        If Not g_DEBUG Then
                mBuild.LockedPJFile = aPJFile
                mBuild.LockedFile = aLockFile
                mBuild.RunCmd "si lock --quiet --recurse --yes --changePackageId=:none --sandbox=" + Chr(34) + aPJFile + Chr(34) + _
                              " " + aLockFile
        End If
    End Sub

    Sub UnlockFiles()
            If Not g_DEBUG Then
                        If mBuild.LockedPJFile <> "" Then  'something is still locked so unlock it
                            mBuild.WriteToDlgAndFile "Unlocking locked pj/file.  PJ: " + mBuild.LockedPJFile + " File: " + mBuild.LockedFile
                            mBuild.RunCmd "si unlock --yes --recurse --quiet --sandbox=" + Chr(34) + mBuild.LockedPJFile + Chr(34) + " " + mBuild.LockedFile
                            mBuild.LockedPJFile = ""
                            mBuild.LockedFile = ""
                        Else
                            mBuild.WriteToDlgAndFile "There were no files found to unlock."
                        End If
                End If
        End Sub

    Sub UnlockFileByLabel(aProject, aFile, aLabel)
            If Not g_DEBUG Then
                    
                        mBuild.WriteToDlgAndFile "Unlocking locked pj/file.  PJ: " + aProject + " File: " + aFile
                        mBuild.RunCmd "si unlock --yes --recurse --quiet --revision=" & aLabel & " --sandbox=" + Chr(34) + aProject + Chr(34) + " " + aFile
                       
                End If
    End Sub

    Sub UnlockFile(aPJFile, aLockFile)
            If Not g_DEBUG Then
                            mBuild.WriteToDlgAndFile "Unlocking locked pj/file.  PJ: " + mBuild.LockedPJFile + " File: " + mBuild.LockedFile
                            mBuild.RunCmd "si unlock --recurse --yes --quiet --sandbox=" + Chr(34) + aPJFile + Chr(34) + " " + aLockFile
            End If
    End Sub

    'made this function so you don't have to remember which params to use for checking in a file everytime
    'also can add debugging code in here b/c don't want to really checkin files to mks if testing
    Sub CheckInFiles(aProjPJ, aFile) 'aProjDir, I don't think I need to pass in the project dir if I also pass in the project pj file %%%check this though
        mBuild.WriteToDlgAndFile "Checking in file(s) " + aFile + " to " + aProjPJ
        If Not g_DEBUG Then
            mBuild.RunCmd "si ci --nolock --saveTimestamp --checkinUnchanged --description build --changePackageId=:none --recurse --quiet --sandbox=" + _
                          Chr(34) + aProjPJ + Chr(34) + " " + aFile
            If Not (aProjPJ = mBuild.LockedPJFile And aFile = mBuild.LockedFile) Then
                    mBuild.BuildError "Error checking in files.  The locked files don't match up with the files being checked in.                                               " + _
                                      "Locked PJ File: " + mBuild.LockedPJFile + " Checking in PJ File: " + aProjPJ + _
                                      "Locked File: " + mBuild.LockedFile + " Checking in File: " + aFile
            Else
                    mBuild.LockedPJFile = ""
                    mBuild.LockedFile = ""
            End If
        End If
    End Sub

    Sub CheckOutFiles(aProjPJ, aFile)
            mBuild.WriteToDlgAndFile "Checking Out Files"
        If Not g_DEBUG Then
                    mBuild.LockedPJFile = aProjPJ
                        mBuild.LockedFile = aFile
                mBuild.RunCmd "si co --recurse --lock --overwriteChanged --quiet --yes --changePackageId=:none --sandbox=" + _
                              aProjPJ + " " + aFile
        End If
    End Sub

    Sub CheckOutFilesNoLock(aProjPJ, aFile)
        mBuild.WriteToDlgAndFile "Checking Out Files"
        If Not g_DEBUG Then
                    mBuild.LockedPJFile = aProjPJ
                    mBuild.LockedFile = aFile
                    mBuild.RunCmd "si co --recurse --nolock --restoreTimestamp --overwriteChanged --yes --quiet --changePackageId=:none --sandbox=" + _
                                   aProjPJ + " " + aFile
        End If
    End Sub

    Sub CheckOutFilesByLabel(aProjPJ, aFile, aLabel)
            mBuild.WriteToDlgAndFile "Checking Out Files"
        If Not g_DEBUG Then
                    mBuild.LockedPJFile = aProjPJ
                        mBuild.LockedFile = aFile
                mBuild.RunCmd "si co --recurse --lock --changePackageId=:none --overwriteChanged --quiet --revision=" + _
                               aLabel + " --sandbox=" + aProjPJ + " " + aFile
        End If
    End Sub
        
    Sub AddFiles(aProjPJ, aFile)
            mBuild.WriteToDlgAndFile "Adding file(s) " + aFile + " to " + aProjPJ
            If Not g_DEBUG Then
                    mBuild.RunCmd "si add --quiet --yes --changePackageId=:none --sandbox " & aProjPJ & " " & aFile
        End If
    End Sub

    Sub AddFileToArchive(aProjPJ, aFile)
         mBuild.WriteToDlgAndFile "Adding file(s) " + aFile + " to " + aProjPJ + " archive"
        
         If Not g_DEBUG Then
                 mBuild.RunCmd "si add --quiet --yes --changePackageId=:none --sandbox=" & aProjPJ & " " & aFile
                 'CheckInFiles aProjPJ, aFile
         End If
    End Sub
	
	Sub Revert(aSandbox, aProjPJ)
				mBuild.WriteToDlgAndFile "Reverting " + aProjPJ
                mBuild.RunCmd "si revert --yes --overwriteUnchanged --restoreTimestamp --quiet --recurse --sandbox=" _
                              + Chr(34) + aSandbox + Chr(34) & " " & Chr(34) & aProjPJ & Chr(34)
    End Sub

    Sub Refresh(aSandbox, aProjPJ)
		Dim Sandbox, ProjPJ, FSO
                Set FSO = CreateObject("Scripting.FileSystemObject")

		Sandbox = FSO.GetFile(aSandbox)
		ProjPJ = FSO.GetFile(aProjPJ)
                mBuild.WriteToDlgAndFile "Refreshing " + aProjPJ
                mBuild.RunCmd "si resync --yes --overwriteChanged --populate --overwriteUnchanged --restoreTimestamp --quiet --recurse --sandbox=" _
                              + Chr(34) + Sandbox + Chr(34) & " " & Chr(34) & ProjPJ & Chr(34)
    End Sub

    Sub RefreshFile(aProjPJ, aFile)
                mBuild.WriteToDlgAndFile "Refreshing " + aProjPJ
                mBuild.RunCmd "si resync --yes --overwriteChanged --restoreTimestamp --quiet --recurse --sandbox=" _
                              + Chr(34) + aProjPJ + Chr(34) + " " + aFile
    End Sub
        
    Sub RemoveFiles(aProjPJ, aFile)
          mBuild.WriteToDlgAndFile "Removing file(s) " + aFile + " from " + aProjPJ
            If Not g_DEBUG Then
                    mBuild.RunCmd "si drop --quiet --changePackageId=:none --sandbox=" & aProjPJ & " " & aFile
        End If
    End Sub
  
    Sub DropSandbox(aType)  
             If Not g_DEBUG Or Result = vbYes Then
                 Select Case aType
                        Case "New"   'Create a new sandbox from the trunk
                            If mBuild.NewBuild.GetProjType = "Trunk" Then
                                   mBuild.WriteToDlgAndFile "Dropping new sandbox "
                                   mBuild.RunCmd ("si dropsandbox --noconfirm --delete=none --yes " + Chr(34) + mBuild.MainPJFile + Chr(34))
                            Else
                                   mBuild.WriteToDlgAndFile "Dropping new sandbox from existing development path"
                                   mBuild.RunCmd ("si dropsandbox --noconfirm --delete=none --yes " + Chr(34) + _
                                                   mBuild.MainPJFile + Chr(34))
                            End If
                         Case "Build"
                                   mBuild.WriteToDlgAndFile "Creating a build sandbox from label: " + mBuild.PrevLabel
                                   mBuild.RunCmd ("si dropsandbox --noconfirm --delete=none --yes " + Chr(34) + mBuild.MainPJFile + Chr(34))
				End Select
             End If
        
    End Sub

    Function ModsProject(aProjPJ, aLabel)
        Dim output
        mBuild.WriteToDlgAndFile "Checking if " + aProjPJ + " has been modified since " + aLabel
        If Not g_DEBUG Then
                'RunCmd returns output from stdout
               ModsProject = mBuild.RunCmd("si mods --projectRevision=" & aLabel & " --project=" & Chr(34) & aProjPJ & Chr(34))
                      
        End If
    End Function

'=====================================================================
' Function    : SandBoxInfo
' Parameters  : none
' Description : returns information about the current project sandbox.
'               Otherwise returns an error message.
' Usage       : Used to determine whether or not to drop a sandbox by
'               looking for error message.
'=====================================================================
    Function SandBoxInfo(aSandBox)
        
        SandBoxInfo = mBuild.RunCmd("si sandboxinfo --sandbox=" & aSandBox)

    End Function

'=====================================================================
' Function    : ProjectInfo
' Parameters  : none
' Description : returns information about the current project .
'               Otherwise returns an error message.
' Usage       : used to determine whether or not a project is a variant
'=====================================================================
    Function ProjectInfo(aProjPJ)
        
        ProjectInfo = mBuild.RunCmd("si projectinfo --sandbox=" & aSandBox)

    End Function
    
'================================================================================
' Function    : ConfigureSubProject
' Parameters  : aProj    - sandbox path of parent project pj 
'               aSubProj - sandbox path of subproject pj
'               aLabel   - version of the subproject 	       
' Description : Configures a subproject to a given revision for a project.
'               aProject must be the parent of the subproject, not a grandparent.
'				Resyncs after configuring, to retrieve the correct files.
'================================================================================
    Function ConfigureSubProject(aProj, aSubProj, aLabel)
		
		mBuild.WriteToDlgAndFile "Configuring " + aSubProj + " to build " + aLabel
		mBuild.RunCmd "si configuresubproject --sandbox=" & aProj & " --subprojectRevision=" & aLabel & _
					  " --type=build " & aSubProj
					 
    End Function

	Sub Disconnect()
		'mBuild.RunCmd "si disconnect --hostname=mksapp --port=7001 --user=mksuser --password=W3lc0m3 --quiet --yes"
	End Sub
	
End Class


