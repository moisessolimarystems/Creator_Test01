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
//#define SPDE_OUTPUT_UNIT_CELL           0x20
#define SPDE_SESSION_CELL               0x28
#define SPDE_TOTAL_MODULE_CELLS         8
#define SPDE_MODS_PER_CELL              16
#define SPDE_TOTAL_KEY_MODULES           SPDE_TOTAL_MODULE_CELLS*SPDE_MODS_PER_CELL

#define SPDE_SESSION_START_CELL                 0x28
#define SPDE_HARDWARE_MODULE_START_CELL         0x08
#define SPDE_RESOURCE_MODULE_START_CELL         0x0F
#define SPDE_MODULE_START_CELL                  0x00

/*-------------------------------------------------------------------------*
 *                                                                         *
 *    SpdProtectionKey Class:                                              *
 *                                                                         *
 *    Derived from ProtectionKey, this class expands upon ProtectionKey    *
 *    functionality by adding SP/D module licensing support to the         *
 *    existing ProtectionKey interface.                                    *
 *                                                                         *
 *-------------------------------------------------------------------------*/
class SpdeProtectionKey : public SpdProtectionKey
{
public:
   SpdeProtectionKey();
   ~SpdeProtectionKey() {}
   SpdeProtectionKey(const SpdeProtectionKey&);


   ushort getOperatorSessionUnits();
   ushort getUserSessionUnits();

   //void setOutputUnits(ushort OutputUnits);
   void setOperatorSessionUnits(ushort SessionUnits);
   void setUserSessionUnits(ushort SessionUnits);

   //---Get Unit Functions
  void getOutputPassword(ushort, ISolimarLicenseSvr3*, ushort, char*);
  void getOperatorSessionPassword(ushort, ISolimarLicenseSvr3*, ushort, char*);
  void getUserSessionPassword(ushort, ISolimarLicenseSvr3*, ushort, char*);

protected:
    // top 8 bits -> userSession units
   //               bottom 8 bits -> operatorSession units
   ushort& sessionUnits;

   // above is equivalent to ModuleCell moduleCells[TOTAL_MODULE_CELLS]
};

#endif
