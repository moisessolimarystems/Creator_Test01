#ifndef __STDAFX_H__
#define __STDAFX_H__

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
#pragma once
#define _WIN32_DCOM

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define NUM_MOD_COLS 3
#define NUM_KEY_COLS 7

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <windows.h>
#include <comutil.h>
#include <comdef.h>
#include <stdio.h>
#include <Commctrl.h>

// TODO: reference additional headers your program requires here
#include "..\SolimarLicenseServer\_SolimarLicenseServer.h"

//Used to Map the Product IDs as specified in KeySpec.xml
const int SPDProductID = 0;
const int XImageProductID = 1;
const int XImageNTProductID = 2;
const int PCLDriverProductID = 3;
const int IndexPlugInProductID = 4;
const int ConnectivityServerProductID = 6;
const int IConvertProductID = 7;
const int SolSearcherEnterpriseProductID = 8;
const int SolScriptProductID = 9;
const int SDXDesignerProductID = 10;
const int QuantumServerProductID = 11;
const int RubikaProductID = 12;
const int SPDEProductID = 14;
const int SPProductID = 15;

//Used to map the license as specified in KeySpec.xml
const int InitialTrialID = 0;
const int ExtendedTrialID = 1;
const int Base = 2;
const int UninitializedTrialID = 3;
const int ExtendedTrialID2 = 4;
const int ExtendedTrialID3 = 5;
const int ExtendedTrialID4 = 6;
const int ExtendedTrialID5 = 7;
const int Unused = 10;
const int Deactivated = 11;

//Used to map the key type as specified in KeySpec.xml
const int KEYNone			=0;
const int KEYAddon		    =1;
const int KEYBase			=2;
const int KEYReplacement	=3;
const int KEYRelicense	    =4;
const int KEYInventory	    =5;
const int KEYBackup		    =6;
const int KEYEmergency	    =7;
//const int KEYSolimar	    =8
const int KEYCustom		    =9;
const int KEYLoan			=10;
const int KEYDemo			=11;
const int KEYRental		    =12;
const int KEYDevelopment	=13;
const int KEYReserved		=14;

//structures
typedef struct 
{
	_variant_t		KeyNumber;
	_variant_t		ProductID;
	_variant_t		ProductVersion;
	_variant_t		ApplicationInstance;
	_variant_t		License;
	long			HoursLeft;
	_variant_t		Active;
	_variant_t		ExpirationDate;
	_variant_t		KeyType;
}KeyInfoStructure, *KeyInfoStructurePtr;

typedef struct
{
	_variant_t ModuleID;
	_variant_t ModuleName;
	long LicensesInUse;
	long TotalLicenses;
}ModuleLicensingStructure, *ModuleLicensingStructurePtr;

typedef struct
{
	int x_coord;
	int y_coord;
	int height;
	int width;
	int ColumnWidth[NUM_MOD_COLS];
	int ColOrder[NUM_MOD_COLS];
}ModuleGUIConfigurationStruct, *ModuleGUIConfigurationStructPtr;


typedef struct
{
	int x_coord;
	int y_coord;
	int height;
	int width;
	int ColWidths[NUM_KEY_COLS];
	int ColOrder[NUM_KEY_COLS];
}KeyGUIConfigurationStruct, *KeyGUIConfigurationStructPtr;

#endif //__STDAFX_H__
