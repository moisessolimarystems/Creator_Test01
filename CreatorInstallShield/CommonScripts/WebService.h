#ifndef _WEBSERVICE_H_
#define _WEBSERVICE_H_  

//////////////////////////////////////////////////////////////////////////
//various webservice functions
//////////////////////////////////////////////////////////////////////////
prototype NUMBER CreateWebSite(number, string, string, string, string);
prototype NUMBER GetNextWebSiteID(string, BYREF number, BYREF BOOL);
prototype NUMBER CreateVirtualDirectory(number, string, string, BOOL);
prototype NUMBER StartWebService(number);
prototype NUMBER GetWebSiteRoot(NUMBER, BYREF STRING);    
prototype NUMBER DeleteVirtualDirectory(number, string, BOOL);
prototype BOOL IsValidWebsite(string, BYREF number);
prototype NUMBER CheckIISExists();
prototype NUMBER CheckIIS6Compatibility();

prototype IsGetObj.ForEachStart(byref object, byref VARIANT);
prototype IsGetObj.ForEachGetNextItem(byref VARIANT, byref object);

#endif //_WEBSERVICE_H_