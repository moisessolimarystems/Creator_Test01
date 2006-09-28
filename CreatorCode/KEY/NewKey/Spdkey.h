//-------------------------------------------------------------------------*
//    filename - spdkey.h                                                  *
//                                                                         *
//    Class(es):                                                           *
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
#ifndef __SPDKEY_H
#define __SPDKEY_H

#define MAX_OUTPUT_UNITS         0x00001000 //4096

// Pages Per Minute specifications
#define MAX_PPM_EXTENSIONS       15
#define MAX_PPM_ELEMENTS         5
#define MAX_PAGES_PER_MINUTE     500 // unlimited
#define XCH_IPDS_ID              2   // Mod id numbers
#define XCH_PCL_ID               35
#define XCH_PS_ID                39
#define XCH_PS_DBCS_ID           44
#define AFPDS_PS_ID              47

// key cell specifications
#define OUTPUT_UNIT_CELL         0x20
#define TOTAL_MODULE_CELLS       17
#define MODS_PER_CELL            4
#define TOTAL_KEY_MODULES        TOTAL_MODULE_CELLS*MODS_PER_CELL
#define MAX_MOD_UNITS_LICENSED   0xF

// key cell numbers
#define MODULE_START_CELL        0



class ModuleCell;

/*-------------------------------------------------------------------------*
 *                                                                         *
 *    SpdProtectionKey Class:                                              *
 *                                                                         *
 *    Derived from ProtectionKey, this class expands upon ProtectionKey    *
 *    functionality by adding SP/D module licensing support to the         *
 *    existing ProtectionKey interface.                                    *
 *                                                                         *
 *-------------------------------------------------------------------------*/
class SpdProtectionKey : public ProtectionKey
{
public:
   SpdProtectionKey();
   virtual ~SpdProtectionKey() {}
   SpdProtectionKey(const SpdProtectionKey&);

   // protection key data (references into keyDataBlock)
   ushort& outputUnits;

   // functions that do not perform key I/O
   //uchar getLicense(ushort mod_id) const;
   ushort getLicense(ushort mod_offset, ushort mod_bits) const;
   void licenseMods();
   void licenseDevices();
   //void setLicense(ushort mod_id, ushort units_licensed);
   void setLicense(ushort mod_offset, ushort mod_bits, ushort units_licensed);
   void setLicensesToZero(); //setSpdKeyCellsToZero
   void setLicenses(ushort* buffer);
   ushort getOutputUnits();
   void   setOutputUnits(ushort units);

   // simple key I/O functions
   virtual void getModulePassword(uchar mod_id,
                           unsigned int units_licensed,
                           ProductId product_id,
                           ushort product_version,
                           ISolimarLicenseSvr* pServer,
                           ushort password_number,
                           char* Password_String
                           );

   //---Output Unit Functions
   virtual void getOutputPassword(ushort, ISolimarLicenseSvr*, ushort password_number, char*);

   //--- Pages Per Minute Functions
   void getPagesPerMinutePassword(ushort, ushort, ISolimarLicenseSvr*, char*, ushort password_number, long);

   ushort getPagesPerMinuteExtensions();
   ushort getPagesPerMinute(ushort/*mod_id*/);
   short setPagesPerMinuteExtensions(ushort/*extensions*/);
   short setPagesPerMinute(ulong/*mod_id*/, ulong/*pages*/);
   //short incrementPagesPerMinuteExtensions(ushort/*extensions*/);

protected:
   // protection key data (references into ProtectionKey::keyDataBlock)
   ushort* moduleCells;
   // above is equivalent to ModuleCell moduleCells[TOTAL_MODULE_CELLS]
};

#endif
