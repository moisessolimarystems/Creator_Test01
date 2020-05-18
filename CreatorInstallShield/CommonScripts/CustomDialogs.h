               
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
#ifndef RES_CHECKBOX1_MSG
#define RES_CHECKBOX1_MSG				601 // ID of Sub-Header Message for Skinned SdWelcomeMaint
#endif
#ifndef RES_CHECKBOX2_MSG
#define RES_CHECKBOX2_MSG				602 // ID of Sub-Header Message for Skinned SdWelcomeMaint
#endif

// Dialog and control IDs for the SdWelcomeMaintCustomDlg() - 102        
#define RES_SDWELCOMEMAINT_REMOVEALL_RADIO		103

// Dialog and control IDs for SdUserLoginCustomDlg() - 12019
#define RES_SDUSERLOGIN_USERNAME				302
#define RES_SDUSERLOGIN_PASSWORD         		303     
 
// Dialog and control IDs for SdUserInfoCustomDlg() - 12017
#define RES_SDUSERINFO_USERNAME					301 

// Dialog ID Values - Direct Editor->Dialog to edit directly
#define RES_SDFINISHUPGRADEDLG_DIALOG_ID		12032// SdFinishUpgradeDlg
#define RES_SDWELCOMEMAINT_DIALOG_ID			102  // SdWelcomeMaintCustomDlg 
#define RES_SDUSERLOGIN_DIALOG_ID				12019 //"SdUserLoginCustomDlg"
#define RES_SDSKINNABLE_MAINT_DIALOG_ID			100 //SdMaintCustomDlg
#define RES_SDUSERINFO_DIALOG_ID				12017 //SdUserInfoCustomDlg       
#define RES_ASKOPTIONS2_DIALOG_ID               10105 //AskOptions2
       
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

prototype INT SdUserLoginCustomDlg(BYVAL STRING/*Title*/, BYVAL STRING/*SubTitle*/, BYVAL STRING);
prototype INT SdUserInfoCustomDlg(BYVAL STRING/*Title*/, BYVAL STRING/*SubTitle*/, BYVAL STRING);
prototype INT SdWelcomeMaintCustomDlg (BYVAL STRING/*Title*/, BYVAL STRING/*SubTitle*/);
prototype INT SdMaintCustomDlg (BYVAL STRING/*Title*/, BYVAL STRING/*SubTitle*/);
prototype AskOptions2( number, BYVAL STRING/*Title*/, BYVAL STRING/*SubTitle*/, byval string/*Msg*/, ... );
                   
//function prototype from DLL_WINDOWSUTILS      
prototype cdecl WindowsUtil.SSPLogonUser(BYREF STRING ,BYREF STRING, BYREF STRING);                   
prototype cdecl LogonUserValidation.IsValidLogonUser(BYREF STRING ,BYREF STRING, BYREF STRING);     
#endif //#ifndef _CUSTOMDIALOGS_H_
