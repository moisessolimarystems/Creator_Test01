////////////////////////////////////////////////////////////////////////////////
//                                                                            
//  File Name:		LicensingFunctions.h
//                                                                            
//  Description:	InstallShield script                                        
//                                                                            
//  Comments:		This file is to house the Licensing Functions
//
//  Copyright:		Solimar Systems, Inc. 
//					Copyright 2003  
//
// 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LICENSEFUNCTIONS_H_
#define _LICENSEFUNCTIONS_H_

#define LICENSE_VIEWER				"LicenseViewer"
#define LICENSE_MANAGER 			"SolimarLicenseManager"
#define LICENSE_SERVER 				"SolimarLicenseServer" 
#define INTERNAL_SERVER				"SolimarInternalRelease"
#define LICENSEVIEWER_PRODUCTCODE	"{A85C9E83-A135-4F5B-AA06-644210A259CA}"
#define LICENSEMANAGER_PRODUCTCODE	"{084943CE-CDB9-4344-AF97-8A9504585530}"
#define LICENSESERVER_PRODUCTCODE	"{EFF35B48-3A4A-4E93-A3F2-CF8B045A66C2}"
#define LICENSESERVER2_PRODUCTCODE	"{70D75965-DAB2-408F-A526-70800C189A15}"
#define LICENSEINTERNALSERVER_PRODUCTCODE	"{EFF35B48-3A4A-4E93-A3F2-CF8B045A66C2}"	
#define LICENSEINTERNALSERVER2_PRODUCTCODE	"{784289EB-1A53-4079-8B9A-9F7619928397}"
#define MSI_EXTENSION           	".msi"                
#define EXE_EXTENSION				".exe"
#define SOLIMAR_LICENSE_KEY         "SOFTWARE\\Solimar\\Solimar Licensing\\"

#define MAX_COMPONENTS				4

prototype InitLicensing();
prototype InstallLicensing(STRING);
prototype UninstallLicensing(STRING);
prototype InstallSentinelDriver();  
prototype RemoveOldLicensing();                 

prototype BOOL IsOldSolimarLicense(STRING);	  
prototype BOOL IsSharedLicenseRegKey(STRING);


#endif //_LICENSEFUNCTIONS_H_
