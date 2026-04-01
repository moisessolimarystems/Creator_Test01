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


#endif //_WEBSERVICE_H_