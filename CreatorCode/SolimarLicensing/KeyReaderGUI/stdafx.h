#ifndef __STDAFX_H__
#define __STDAFX_H__

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
#pragma once
#define _WIN32_DCOM
#define _WIN32_WINNT 0x0400

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

//structures
typedef struct 
{
	_variant_t		KeyNumber;
	_variant_t		ProductID;
	_variant_t		ProductVersion;
	_variant_t		License;
	long			HoursLeft;
	_variant_t		Active;
	_variant_t		ExpirationDate;
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
