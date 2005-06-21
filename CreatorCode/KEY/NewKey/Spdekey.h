//-------------------------------------------------------------------------*
//    filename - spdekey.h                                                  *
//                                                                         *
//    Class(es):                                                           *
//       SpdeProtectionKey                                                  *
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
#ifndef __SPDEKEY_H
#define __SPDEKEY_H

// Pages Per Minute specifications
#define SPDE_MAX_PPM_EXTENSIONS       15
#define SPDE_MAX_PPM_ELEMENTS         5
#define SPDE_MAX_PAGES_PER_MINUTE     500 // unlimited
#define SPDE_XCH_IPDS_ID              34   // Mod id numbers
#define SPDE_XCH_PCL_ID               36
#define SPDE_XCH_PS_ID                38
#define SPDE_XCH_PS_DBCS_ID           39
#define SPDE_AFPDS_PS_ID              2

// key cell specifications
#define SPDE_OUTPUT_UNIT_CELL           0x28
#define SPDE_OPERATOR_SESSION_CELL      0xB0
#define SPDE_USER_SESSION_CELL          0xB8
#define SPDE_TOTAL_MODULE_CELLS         3
#define SPDE_MODS_PER_CELL              16
#define SPDE_TOTAL_KEY_MODULES           SPDE_TOTAL_MODULE_CELLS*SPDE_MODS_PER_CELL
#define SPDE_MAX_MOD_UNITS_LICENSED      0xF

#define SPDE_HARDWARE_MODULE_START_CELL         16
#define SPDE_RESOURCE_MODULE_START_CELL         15
#define SPDE_MODULE_START_CELL                  0

// Key cell numbers
const short PSINTERP_CELL = 0x178;
const short BUSTAGIN_CELL = 0x19c;
const short BUSTAGOUT_CELL = 0x198;
const short PARALLELIN_CELL = 0x194;
const short PARALLELOUT_CELL = 0x190;
const short SCSIOUT_CELL = 0x18c;
const short RESOURCES_CELL = 0x15;
const short HARDWARE_CELL_I = 0x16;
const short HARDWARE_CELL_II = 0x17;

class ModuleCell;
class LargeModuleCell;

/*-------------------------------------------------------------------------*
 *                                                                         *
 *    SpdProtectionKey Class:                                              *
 *                                                                         *
 *    Derived from ProtectionKey, this class expands upon ProtectionKey    *
 *    functionality by adding SP/D module licensing support to the         *
 *    existing ProtectionKey interface.                                    *
 *                                                                         *
 *-------------------------------------------------------------------------*/
class SpdeProtectionKey : public ProtectionKey
{
public:
   SpdeProtectionKey();
   SpdeProtectionKey(const SpdeProtectionKey&);

   // protection key data (references into keyDataBlock)
   ushort& outputUnits;
   ushort& operatorSessionUnits;
   ushort& userSessionUnits;

   // functions that do not perform key I/O
   uchar getLicense(ushort mod_id) const;
   void licenseMods();
   void licenseDevices();
   void setLicense(/*ushort index,*/ ushort mod_id, ushort units_licensed);
   void setLicensesToZero(); //setSpdKeyCellsToZero
   void setLicenses(ushort* buffer, bool hardware);

   ushort getOutputUnits();
   ushort getOperatorSessionUnits();
   ushort getUserSessionUnits();

   // simple key I/O functions
   void getModulePassword(uchar mod_id,
                           uchar units_licensed,
                           ProductId product_id,
                           ushort product_version,
                           ISolimarLicenseSvr* pServer,
                           char* Password_String
                           );


   //---Get Unit Functions
  void getOutputPassword(ushort, ISolimarLicenseSvr*, char*);
  void getOperatorSessionPassword(ushort, ISolimarLicenseSvr*, char*);
  void getUserSessionPassword(ushort, ISolimarLicenseSvr*, char*);

  //---Set Unit Functions

   //--- Pages Per Minute Functions
   void getPagesPerMinutePassword(ushort, ushort, ISolimarLicenseSvr*, char*, long);

   ushort getPagesPerMinuteExtensions();
   short setPagesPerMinuteExtensions(ushort/*extensions*/);


protected:
   // protection key data (references into ProtectionKey::keyDataBlock)
   ModuleCell* moduleCells;   //regular module area
   LargeModuleCell* hardwareModuleCells;  //hardware module area
   LargeModuleCell* resourceModuleCells;  //resource module area
   // above is equivalent to ModuleCell moduleCells[TOTAL_MODULE_CELLS]
};

#endif
