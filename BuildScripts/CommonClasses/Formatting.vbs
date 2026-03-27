Class CFormatting

	'Changes spaces in the string to stars
	'ex: solimar print director  ==> solimar*print*director
      '%%% iconvert doesn't use
	'Function spaceToStar(str)
 
	'   If str = " " Then
	'		spaceToStar = "BAD NAME"
'		Else
'			spaceToStar = Replace(str, " ", "*", 1, -1)
'		End If
'
'	End Function
   
      '%%%iconvert doesn't use
	Function VersionToDevPath(version)

		VersionToDevPath = "_" & Replace(version, ".", "_", 1, -1)

	End Function


    '%%%iconvert doesn't use
	'Changes 5.00A ==>  0x500A
	Function VersionToHex(version)
		VersionToHex = "0x" & Replace(version, ".", "", 1, -1)
	End Function





	'ex. Changes 5.00C ==> SPD500C
	'Used in the .pfw file for the FTP site self exe
	'%%%iconvert doesn't use
    Function VersionToOldTitle(version, product)
		VersionToOldTitle = product & Replace(version, ".", "", 1, -1)
	End Function


    '%%%iConvert doesn't use
	Function VersionToSixDigit(version)
		VersionToSixDigit = version
		Do While (Len(VersionToSixDigit) < 6)
			VersionToSixDigit = "0" & VersionToSixDigit
		Loop
	End Function



	'Make a string be a length of X by appendings 0s or cutting the string
	'%%%iconvert doesn't use
    Function VersionToXChar(version, x)
		VersionToXChar = version
		If Len(VersionToXChar) > x Then 'Takes the first x chars
			VersionToXChar = Mid(VersionToXChar, 1, x)
		Else
			Do While Len(VersionToXChar) < x 'Appends 0s until version is has a length of x
				VersionToXChar = VersionToXChar & "0"
			Loop
		End If
	End Function


End Class
