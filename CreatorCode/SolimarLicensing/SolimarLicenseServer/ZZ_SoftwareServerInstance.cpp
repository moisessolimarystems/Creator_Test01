#include "SoftwareServerInstance.h"
// Rename file from SoftwareServerInstance.cpp to ZZ_SoftwareServerInstance.cpp to ensure that all other instance have a chance to get put
// on the stack before g_licenseController.   There was an issue during developlment where LicenseController g_licenseController was being executed 
// before an array to unescape xml was initialize thus causing an error that was hard to debug.

// Commented out code to help detect memory leak
//#include "vld.h"
//class LastObject
//{
//public:
//   LastObject()
//   {
//      OutputDebugStringW(L"LastObject::LastObject()");
//      VLDEnable();
//   }
//   ~LastObject()
//   {
//      OutputDebugStringW(L"LastObject::~LastObject()");
//      VLDReportLeaks();
//   }
//};
//
//LastObject g_lastObject;

LicenseController g_licenseController;

//SoftwareServer g_softwareServer;

