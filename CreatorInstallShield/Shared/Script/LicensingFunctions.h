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
#define MSI_EXTENSION           	".msi"
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
