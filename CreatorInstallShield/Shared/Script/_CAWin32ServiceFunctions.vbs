'===================================================================================================
' Function    : StopServices
' Parameters  : NONE
' Description : Requires win2k and for a property named "SERVICES" to be defined with the names 
'		of the services to modify.
'===================================================================================================
Function StopServices()

    Dim strComputer, objService, errReturn
    Dim serviceList, services, serviceName
    services = Session.Property("SERVICES")       
    serviceList = Split(services, ",", -1, 1)

    Const ERROR_EXIT = 3
    StopServices = 0
    strComputer = "."
    For each serviceName in serviceList
    	Set objWMIService = GetObject("winmgmts:" _
  	  & "{impersonationLevel=impersonate}!\\" _
  	  & strComputer & "\root\cimv2")
	strQuery = "Select * from Win32_Service Where Name =" & Chr(34) & serviceName & Chr(34) & " And AcceptStop = True"
	Set colListOfServices = objWMIService.ExecQuery(strQuery)
	For Each objService in colListOfServices
            errReturn = objService.StopService()
	    If errReturn <> 0 Then
		StopServices = ERROR_EXIT
		Exit Function
	    End If
        Next
    Next
End Function

Function StartServices()

    Dim strComputer, objService
    Dim serviceList, services, serviceName
    services = Session.Property("SERVICES")       
    serviceList = Split(services, ",", -1, 1)

    strComputer = "."
    For each serviceName in serviceList
    	Set objWMIService = GetObject("winmgmts:" _
  	  & "{impersonationLevel=impersonate}!\\" _
  	  & strComputer & "\root\cimv2")
	strQuery = "Select * from Win32_Service Where Name =" & Chr(34) & serviceName & Chr(34)
	Set colListOfServices = objWMIService.ExecQuery(strQuery)
        For Each objService in colListOfServices
            objService.StartService()
        Next
    Next
End Function

Function ServiceStatus()
    Dim strComputer, objService
    Dim serviceList, services, serviceName    
    services = Session.Property("SERVICES")  
    serviceList = Split(services, ",", -1, 1)

    Const vbYesNo = 4
    Const vbYes = 6
    Const vbNo = 7 
    Const ERROR_INSTALL_USEREXIT = 2
    Const ERROR_SUCCESS = 0

    ServiceStatus = ERROR_SUCCESS
    strComputer = "."
    For each serviceName in serviceList
    	Set objWMIService = GetObject("winmgmts:" _
  	  & "{impersonationLevel=impersonate}!\\" _
  	  & strComputer & "\root\cimv2")
	strQuery = "Select * from Win32_Service Where Name =" & Chr(34) & serviceName & Chr(34)
	Set colListOfServices = objWMIService.ExecQuery(strQuery)
        For Each objService in colListOfServices
	    If objService.State <> "Stopped" Then
		var = MsgBox("The " & serviceName & " service is currently running. " & vbCr & _
			     "In order to uninstall the " & ServiceName & " service, it must be shut down. To do this, open the System Manager and select File>Shutdown Server. " & vbCr & _
			     "Setup will now exit.", vbOkOnly, "Warning")		  	     
		ServiceStatus = ERROR_INSTALL_USEREXIT 
    	        Exit Function
	    End If
        Next
    Next	
End Function



