//-------------------------------------------------------------------------*
//    filename - spdkey.cpp                                                *
//                                                                         *
//    Class(es):                                                           *
//       ModuleCell                                                        *
//       SpdProtectionKey                                                  *
//                                                                         *
//-------------------------------------------------------------------------*
//-------------------------------------------------------------------------*
//                                                                         *
//    Solimar Print/Director                                               *
//                                                                         *
//    Copyright (c) 1992-1996 by Solimar Systems, Inc.                     *
//    All Rights Reserved.                                                 *
//                                                                         *
//-------------------------------------------------------------------------*
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <classes.hpp>

//
// Windows NT or 32-Bit Windows 95 Application
//

#if defined(__WIN32__)

#include <windows.h>
//#include "3prty\rainbow\winnt\spromeps.h"

//
// Windows 3.1 or 16-Bit Windows 95 Application
//

#else

//#include "3prty\rainbow\windows\superpro.h"

#endif // Windows 3.1 or 16-Bit Windows 95 Application

#include "keyprod.h"
#include "pkey.h"
#include "spdkey.h"

#ifdef __DocServer__
#include "lm_sol.h"
#endif

#ifdef __AfpCvt__
#include "lm_sol.h"
#endif

#ifdef __SOLscript__
#include "SOLscriptKeyMgr.h"
#endif

#ifdef __SPD_MODULE_LICENSING__
#ifndef __DocServer__
#ifndef __AfpCvt__
#ifndef __SOLscript__
#include "sbase.h"
#include "sdevice.h"
#include "spath.h"
#include "smodule.h"
#endif
#endif
#endif
#endif

// These symbols were not defined in Builder 6     - Chris
#if defined(__BORLANDC__)
#ifndef min
#define min(x,y) ((x) < (y) ? (x) : (y))
#endif
#ifndef max
#define max(x,y) ((x) < (y) ? (y) : (x))
#endif
#endif


/*-------------------------------------------------------------------------*
 *                                                                         *
 *    ModuleCell Class:                                                    *
 *                                                                         *
 *    This class corresponds to one cell of the protection key.  Using     *
 *    ModuleCell, you can access a ushort by nibble (4 bits).  ModuleCells *
 *    are used primarily to store the module licensing information from    *
 *    the protection key.                                                  *
 *                                                                         *
 *-------------------------------------------------------------------------*/
enum Nibbles {NIBBLE_A, NIBBLE_B, NIBBLE_C, NIBBLE_D};
class ModuleCell
{
public:
   ModuleCell() : a(0), b(0), c(0), d(0) {};
   operator ushort() const {return *((ushort*)this);}

   enum {NIBBLE=4};
   ushort d:NIBBLE;
   ushort c:NIBBLE;
   ushort b:NIBBLE;
   ushort a:NIBBLE;
};



/*-------------------------------------------------------------------------*
 *                                                                         *
 *    SpdProtectionKey Class:                                              *
 *                                                                         *
 *    Derived from ProtectionKey, this class expands upon ProtectionKey    *
 *    functionality by adding SP/D module licensing support to the         *
 *    existing ProtectionKey interface.                                    *
 *                                                                         *
 *-------------------------------------------------------------------------*/

/* constructor
---------------------------------------------------------------------------*/
#ifdef __WIN32__ // the Win32 rainbow library
SpdProtectionKey::SpdProtectionKey() :
   ProtectionKey(),
   moduleCells((ModuleCell*)&keyDataBlock.data[MODULE_START_CELL]),
   outputUnits(keyDataBlock.data[OUTPUT_UNIT_CELL])
{
}
#else // not the Win32 rainbow library
SpdProtectionKey::SpdProtectionKey() :
   ProtectionKey(),
   moduleCells((ModuleCell*)&keyDataBlock.data[MODULE_START_CELL])
   outputUnits(keyDataBlock.data[OUTPUT_UNIT_CELL])
{
}
#endif

/* copy constructor
---------------------------------------------------------------------------*/
SpdProtectionKey::SpdProtectionKey(const SpdProtectionKey& pkey) :
   ProtectionKey(pkey),
   moduleCells((ModuleCell*)&keyDataBlock.data[MODULE_START_CELL]),
   outputUnits(keyDataBlock.data[OUTPUT_UNIT_CELL])
{
}

/* getLicense()
 *    Get and return the license of module mod_id in the moduleCells[] array.
---------------------------------------------------------------------------*/
uchar SpdProtectionKey::getLicense(ushort mod_id) const
{
   ushort units_licensed = 0;
   ushort mcell = ushort(mod_id>>2); // >>2 same as /4
   Nibbles offset = (Nibbles)(mod_id&3); // same as %4
   switch(offset) {
      case NIBBLE_A:
         units_licensed = moduleCells[mcell].a;
         break;
      case NIBBLE_B:
         units_licensed = moduleCells[mcell].b;
         break;
      case NIBBLE_C:
         units_licensed = moduleCells[mcell].c;
         break;
      case NIBBLE_D:
         units_licensed = moduleCells[mcell].d;
   }
   return (uchar)units_licensed;
}
/* getOutputPassword()
 *
 *
 *
 *
 *
 *    This query will produce a password that is therefore unique
 *    for this customer, with this key, and with this many output units
 *    licensed.  AMAZING!
 -----------------------------------------------------------------------------*/
 const uchar OUTPUT_POOL_MODULE_ID = 128;
 void SpdProtectionKey::getOutputPassword(ushort output_units,
                                          ISolimarLicenseSvr* pServer,
                                          char* Password_String)
 {
   if(pServer)
   {
      BSTR password;
      if(SUCCEEDED(pServer->GenerateModulePassword(customerNumber,
                                                   keyNumber,
                                                   productId,
                                                   OUTPUT_POOL_MODULE_ID,
                                                   output_units,
                                                   &password
                                                  )))
      {
         AnsiString* pwd = new AnsiString(password);
         strcpy(Password_String, pwd->c_str());
         delete pwd;
      }
      SysFreeString(password);
   }
 }



/* getModulePassword()
 *    Calculate and return the password for module mod_id by querying the
 *    secondary algorithm of the protection key.  If successful, returns
 *    the password for module mod_id, otherwise returns 0. Note that this
 *    function uses the units_licensed parameter to generate a password
 *    for the "next" incremental license. For example, if units_licensed
 *    is 2, the password this function returns should license the user for
 *    3 units.
 *
 *    Query = 01234567, where
 *       012 = Customer Number
 *       34  = Key Number
 *       5   = current units_licensed for module #mod_id, 0 for trial key
 *       67  = mod_id
 *
 *    This query will produce a password that is therefore unique
 *    for this customer, with this key, for this module, and with
 *    this many units licensed.
---------------------------------------------------------------------------*/
void SpdProtectionKey::getModulePassword(uchar mod_id,
                                          uchar units_licensed,
                                          ProductId product_id,
                                          ushort product_version,
                                          ISolimarLicenseSvr* pServer,
                                          char* Password_String
                                          )
{
   if(pServer)
   {
      BSTR password;
      if(SUCCEEDED(pServer->GenerateModulePassword(customerNumber,
                                                   keyNumber,
                                                   product_id,
                                                   mod_id,
                                                   units_licensed,
                                                   &password
                                                  )))
      {
         AnsiString* pwd = new AnsiString(password);
         strcpy(Password_String, pwd->c_str());
         delete pwd;
      }
      SysFreeString(password);
   }
}



//==============================================================================
// Function:    getPagesPerMinutePassword()
// Purpose:     This function will create and return a password for
//
//              This query will produce a password that is therefore unique
//              for this customer, with this key, and with this many pages
//              licensed.  AMAZING!
// Parameters:  ushort - # of extensions
// Returns:     ulong - password
//==============================================================================
void SpdProtectionKey::getPagesPerMinutePassword(ushort ext,
                                                 ushort pages,
                                                 ISolimarLicenseSvr* pServer,
                                                 char* Password_String,
                                                 long ModID)
{
   if(pServer)
   {
      BSTR password;
      DWORD ppm_struct = (((pages & 0x0000FFFF) << 16) | (ext & 0x0000FFFF));
      if(SUCCEEDED(pServer->GenerateModulePassword(customerNumber,
                                                   keyNumber,
                                                   productId,
                                                   ModID,
                                                   ppm_struct,
                                                   &password
                                                  )))
      {
         AnsiString* pwd = new AnsiString(password);
         strcpy(Password_String, pwd->c_str());
         delete pwd;
      }
      SysFreeString(password);
   }
}


//==============================================================================
// Function:    getPagesPerMinute()
// Purpose:     This function will return the number of pages per minute.
//              It will check the registry for certain settings.
//		Under the Solimar\\Solimar Print/Director key there will be an
//		entry based on the customer number, key number, mod id, and
//		pages per minute. The DWORD entry is found based on the key number
//		that it is looking for, and then the pages are extracted from the
//		DWORD value.
//		n = (0x00000FFF & pages) |
//		  ((0x000000FF & mod_id)<<12) |
//		  ((0x000000FF & keyNumber)<<20);
// Parameters:  ushort - mod_id
// Returns:     ushort - the pages per minute
//==============================================================================
ushort SpdProtectionKey::getPagesPerMinute(ushort mod_id)
{
   DWORD pages(0);
   DWORD regKeyInfo(0);
   HKEY hKey;

   if ( isOnTrial() )
	   return MAX_PAGES_PER_MINUTE;

   sprintf( scratchBuf, "SOFTWARE\\Solimar\\Solimar Print/Director");
   if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, scratchBuf, 0, KEY_ALL_ACCESS, &hKey)==ERROR_SUCCESS)
   {
        DWORD dwValue =(DWORD)-1;
		  DWORD dwType,dwLen=sizeof(DWORD);

        LPCSTR lpValueName;
        sprintf( scratchBuf, "%03hX%02hd%03hd", customerNumber, keyNumber, mod_id );
 		  lpValueName = scratchBuf;

   	switch(mod_id)
		{
			case XCH_IPDS_ID:      //Xchange::IPDS
         case XCH_PCL_ID:       //Xchange::PCL
         case XCH_PS_ID:        //Xchange::PS
         case XCH_PS_DBCS_ID:   //Xchange::PS (Double byte character support)
         case AFPDS_PS_ID:      //AFPDS::PS
         	regKeyInfo = (RegQueryValueEx(hKey, lpValueName, NULL, &dwType,(BYTE*)&dwValue,&dwLen) != ERROR_SUCCESS) ? 0 : dwValue;
            pages = (0x00000FFF & regKeyInfo);
            break;
         default:
            pages = 0;
            break;
      }
   }

   RegCloseKey(hKey);
   return (ushort)pages;
}

//==============================================================================
// Function:    setPagesPerMinute()
// Purpose:     This function will set the number of pages per minute in the
//		registry. We will code it so that it is not recognizeable to
//		the user.
// Parameters:  ulong mod_id -
//		ulong pages -
// Returns:     None
//==============================================================================
short SpdProtectionKey::setPagesPerMinute(ulong mod_id, ulong pages)
{
   HKEY hKey;
   DWORD disp ;

   sprintf( scratchBuf, "SOFTWARE\\Solimar\\Solimar Print/Director");
   if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, scratchBuf, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &disp)==ERROR_SUCCESS)
   {
		LPCTSTR lpValueName;
      switch(mod_id)
      {
      	case XCH_IPDS_ID:      //Xchange::IPDS
      	case XCH_PCL_ID:       //Xchange::PCL
      	case XCH_PS_ID:        //Xchange::PS
      	case XCH_PS_DBCS_ID:   //Xchange::PS (Double byte character support)
      	case AFPDS_PS_ID:      //AFPDS::PS
      		sprintf( scratchBuf, "%03hX%02hd%03hd", customerNumber, keyNumber, mod_id );
      		lpValueName = scratchBuf;
      		break;
      	default:
      		lpValueName = "";
      		break;
      }

      DWORD n = (0x00000FFF & pages) | ((0x000000FF & mod_id)<<12) | ((0x000000FF & keyNumber)<<20);
      if (RegSetValueEx(hKey, lpValueName, NULL, REG_DWORD, (BYTE *) &n, sizeof(DWORD)) != ERROR_SUCCESS)
      {
      	MessageBox(NULL, "Error Updating Registry.", "", MB_OK | MB_ICONWARNING);
         return FAILURE;
      }
      // end of registry
	}
	RegCloseKey(hKey);
   return SUCCESS;

}


//==============================================================================
// Function:    getPagesPerMinuteExtensions()
// Purpose:     This function will return the number of extensions from the key
//              The extensions are stored in the output units cell, and are the
//              4 most significant bits of 0xAFFF where A houses the number of
//              extensions.  Since 4095 is the max number of outputs, we will use
//              This value for the pages per minute extensions.  That way, the
//              key is responsible for the number of extensions.
// Parameters:  None
// Returns:     ulong - password
//==============================================================================
ushort SpdProtectionKey::getPagesPerMinuteExtensions()
{
   DWORD ext(0);
   DWORD regKeyInfo(0);
   HKEY hKey;

   if ( isOnTrial() )
        return 0;

   sprintf( scratchBuf, "SOFTWARE\\Solimar\\Solimar Print/Director");
   if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, scratchBuf, 0, KEY_ALL_ACCESS, &hKey)==ERROR_SUCCESS)
   {

        DWORD dwValue =(DWORD)-1;
        DWORD dwType,dwLen=sizeof(DWORD);

        LPCSTR lpValueName;
        sprintf( scratchBuf, "%03hX%02hX", customerNumber, keyNumber);
        lpValueName = scratchBuf;

   	  regKeyInfo = (RegQueryValueEx(hKey, lpValueName, NULL, &dwType,(BYTE*)&dwValue,&dwLen) != ERROR_SUCCESS) ? 0 : dwValue;
        ext = (0x000000FF & regKeyInfo);
   }

   RegCloseKey(hKey);
   return (ushort)ext;
}

//==============================================================================
// Function:    setPagesPerMinuteExtensions()
// Purpose:     This function will set the number of extensions for the key
//              The extensions are stored in the output units cell, and are the
//              4 most significant bits of 0xAFFF where A houses the number of
//              extensions.  Since 4095 is the max number of outputs, we will use
//              This value for the pages per minute extensions.  That way, the
//              key is responsible for the number of extensions.
// Parameters:  ushort - extensions
// Returns:     ulong - password
//==============================================================================
short SpdProtectionKey::setPagesPerMinuteExtensions(ushort ext)
{
   HKEY hKey;
   DWORD disp ;

   sprintf( scratchBuf, "SOFTWARE\\Solimar\\Solimar Print/Director");
   if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, scratchBuf, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &disp)==ERROR_SUCCESS)
   {
      LPCTSTR lpValueName;
      sprintf( scratchBuf, "%03hX%02hX", customerNumber, keyNumber);
      lpValueName = scratchBuf;

      DWORD n = (0x000000FF & ext) | ((0x000000FF & keyNumber)<<12) | ((0x00000FFF & customerNumber)<<20);
      if (RegSetValueEx(hKey, lpValueName, NULL, REG_DWORD, (BYTE *) &n, sizeof(DWORD)) != ERROR_SUCCESS)
      {
      	MessageBox(NULL, "Error Updating Registry.", "", MB_OK | MB_ICONWARNING);
         return FAILURE;
      }
      // end of registry
   }
   RegCloseKey(hKey);
   return SUCCESS;
}

/* licenseMods()
 *    Set up unitslicensed of the module list according to the module
 *    licensing information of all active keys.  Does not read in licensing
 *    info from the key, but uses data read in by readKeyData().
---------------------------------------------------------------------------*/
void SpdProtectionKey::licenseMods()
{
#ifdef __SPD_MODULE_LICENSING__
   uchar digit;
   SModuleInfo *info;
   for (info = SModuleInfo::top; info; info = (SModuleInfo *)info->next)
   {
      digit = getLicense(ushort(info->id));
      info->unitslicensed = uchar(min(info->unitslicensed+digit, int(info->maxlicensed)));
   }
#endif
}

/* licenseDevices()
 *
 *
------------------------------------------------------------------------------*/
void SpdProtectionKey::licenseDevices()
{
#ifdef __SPD_MODULE_LICENSING__
	unsigned short digit(outputUnits);
   SModuleInfo::outputDeviceLicenses =
      (unsigned short)( min(SModuleInfo::outputDeviceLicenses+digit, MAX_OUTPUT_UNITS) );
#endif
}


/* setLicense()
 *    Set the license of module mod_id in the moduleCells[] array to
 *    units_licensed.
---------------------------------------------------------------------------*/
void SpdProtectionKey::setLicense(ushort mod_id, ushort units_licensed)
{
   ushort mcell = ushort(mod_id>>2); // >>2 same as /4
   Nibbles offset = (Nibbles)(mod_id&3); // same as %4
   switch(offset) {
      case NIBBLE_A:
         moduleCells[mcell].a = units_licensed;
         break;
      case NIBBLE_B:
         moduleCells[mcell].b = units_licensed;
         break;
      case NIBBLE_C:
         moduleCells[mcell].c = units_licensed;
         break;
      case NIBBLE_D:
         moduleCells[mcell].d = units_licensed;
   }
}

/* setLicenses(ushort* buffer)
 *    Should only be used by creator.
---------------------------------------------------------------------------*/
void SpdProtectionKey::setLicenses(ushort* buffer)
{
   memcpy( moduleCells, buffer, 32 );
}


/* setLicensesToZero()
 *    Initializes all licenses in the moduleCells[] array to 0 and sets all
 *    other cells that need to be zeroed out.   This is performed when the
 *    status of the key changes from trial to base.
---------------------------------------------------------------------------*/
void SpdProtectionKey::setLicensesToZero()
{
   //zero out total output units
   outputUnits = 0x0000;

   //zero out modules
   ushort total_modules = TOTAL_MODULE_CELLS*MODS_PER_CELL;
   for (ushort i = 0; i < total_modules; i++)
      setLicense(i, 0);
}

// getOutputUnits - returns the number of outputs licensed for the key
ushort SpdProtectionKey::getOutputUnits()
{
	return outputUnits;
}




