               
////////////////////////////////////////////////////////////////////////////////
//                                                                            
//  File Name:		CustomDialogs.h
//                                                                            
//  Description:	InstallShield script                                        
//                                                                            
//  Comments:		This script file is for custom dialogs that I have created.
//
//  Copyright:		Solimar Systems, Inc. 
//					Copyright 2003  
//
//  Author:			Jeff Lan
//
////////////////////////////////////////////////////////////////////////////////     

#ifndef _CUSTOMDIALOGS_H_
#define _CUSTOMDIALOGS_H_ 

//
// Control IDs for all Dialog boxes
#ifndef RES_PBUT_NEXT
#define RES_PBUT_NEXT         					1   // ID of 'Next' push button
#endif
#ifndef RES_PBUT_CANCEL	
#define RES_PBUT_CANCEL       					9   // ID of 'Cancel' push button
#endif
#ifndef RES_PBUT_BACK
#define RES_PBUT_BACK        					12  // ID of 'Back' push button
#endif
#ifndef RES_DIALOG_TITLE
#define RES_DIALOG_TITLE						50  // ID of Header Label
#endif
#ifndef RES_DIALOG_SUBTITLE
#define RES_DIALOG_SUBTITLE						51  // ID of Header Message
#endif
#ifndef RES_DIALOG_SUBTITLE_MSG
#define RES_DIALOG_SUBTITLE_MSG					711 // ID of Sub-Header Message
#endif    
#ifndef RES_DIALOG_MSG
#define RES_DIALOG_MSG							1325 // ID of Sub-Header Message
#endif     
#ifndef RES_SKINNABLE_DIALOG_MSG
#define RES_SKINNABLE_DIALOG_MSG				710 // ID of Sub-Header Message for Skinned SdWelcomeMaint
#endif

// Dialog and control IDs for the SdWelcomeMaintCustomDlg() - 102        
#define RES_SDWELCOMEMAINT_REMOVEALL_RADIO		103

// Dialog and control IDs for SdUserInfoCustomDlg() - 0
#define RES_SDUSERLOGIN_USERNAME				302
#define RES_SDUSERLOGIN_PASSWORD         		303
#define RES_SDUSERLOGIN_CONFIRMPASS            	304  
#define RES_SDUSERLOGIN_DOMAINNAME				301

// Dialog ID Values - Direct Editor->Dialog to edit directly
#define RES_SDFINISHUPGRADEDLG_DIALOG_ID		12032// SdFinishUpgradeDlg
#define RES_SDWELCOMEMAINT_DIALOG_ID			102  // SdWelcomeMaintCustomDlg 
#define RES_SDUSERLOGIN_DIALOG_ID				12019 //"SdUserLoginCustomDlg" //SdUserInfoCustomDlg
#define RES_SDSKINNABLE_MAINT_DIALOG_ID			100 //SdMaintCustomDlg

#define DLL_WINDOWSUTILS					"WindowsUtil.dll" 
//
// Function prototype definitions 
prototype SdAskDestPathDlg();   
prototype SdFeatureTreeDlg();
prototype SdFinishUpgradeDlg(); 
prototype SdLicenseDlg();
prototype SdOptionsButtonsDlg();  
prototype SdWelcomeDlg();
prototype SdChooseSCSIDriverDlg(BOOL, BOOL);

prototype INT SdWelcomeMaintCustomDlg (BYVAL STRING/*Title*/, BYVAL STRING/*SubTitle*/);
prototype INT SdMaintCustomDlg (BYVAL STRING/*Title*/, BYVAL STRING/*SubTitle*/);
prototype INT SdUserLoginCustomDlg(BYVAL STRING/*Title*/, BYVAL STRING/*SubTitle*/);
prototype UnUseDialogDLL();
//function prototype from DLL_WINDOWSUTILS      
prototype cdecl WindowsUtil.SSPLogonUser(BYREF STRING ,BYREF STRING, BYREF STRING);

#endif //#ifndef _CUSTOMDIALOGS_H_
