//---------------------------------------------------------------------------
// Filename:    clookups.h
// Note:        Copyright (c) 1993-2001 by Solimar Systems, Inc.
//              All Rights Reserved.
//------------------------------------------------------------------------------

#ifndef clookupsH
#define clookupsH
//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include <Db.hpp>
#include <DBTables.hpp>

//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------
const int MAX_SOLIMAR_PRODUCTS = 15;    // 9 Products Total, should be largest product ID + 1
                                        // current largest product ID = 14
struct SolimarProduct
{
   SolimarProduct(){ version = 0x0000;}

   AnsiString name;
   unsigned short version;

};

//------------------------------------------------------------------------------
//      class ModuleDetail
//------------------------------------------------------------------------------
class ModuleDetail
{
public:
   AnsiString name;                     //module name
   AnsiString engineer;                 //Alex, Bill, Bert, Drew, Dylan, Jeff, Steve
   AnsiString description;              //Module Description
   AnsiString partnumber;               //part number for each module
   unsigned short version_added;        //0000-FFFF
   unsigned short version_removed;      //0000-FFFF
   int max;                             //0-15
   //int max_solscript;                   //0-15 max solscript value
   int function;                        //input, emulation, output
   int pid;                             //product id
   unsigned short id;                   //module id
   bool spd_default;                    //spd default
   bool spde_default;                    //spde default
   bool con_module;                     //connectivity module
   bool con_default;                    //connectivity default
   bool iConvert_module;                //iConvert module
   bool iConvert_default;               //iConvert default module
   bool bExistingMember;                //existing database member
   bool pages_per_minute;               //pages per minute module
   bool m_bQuantumModule;
   bool m_bQuantumDefault;

   ModuleDetail();
   char* getActionText();
   bool isAvailableForVersion(unsigned short version);
   bool isAvailableForProduct(unsigned short product);
   bool isDefaultForProduct(unsigned short product);
   AnsiString getTextForUnits(unsigned short product, unsigned char units);
};
//------------------------------------------------------------------------------
//      class CLookup
//------------------------------------------------------------------------------
class CLookup
{
  private:
   TDatabase* cDatabase;
   TQuery*    cQuery;

   //product lookup members
   SolimarProduct product_list[MAX_SOLIMAR_PRODUCTS];  //array of products
   bool resetProductList();

   //module lookup members
   ModuleDetail unassigned_module;
   ModuleDetail* module_detail[64];
   ModuleDetail* iConvert_module_detail[64];
   ModuleDetail* m_solScriptModuleDetail[64];
   ModuleDetail* m_PDFUtilityModuleDetail[64];
   ModuleDetail* m_SpdeModuleDetail[64];

  public:
   CLookup();
   ~CLookup();

   //product lookup members
   void reloadProducts();
   unsigned short getCurrentVersion(unsigned short productId, bool spd_dos);
   AnsiString getProductText(unsigned short productId, unsigned short version);
   void updateVersion(unsigned short product_id, unsigned short version);

   //Added these functions for ModuleEditor....updates sModuleDetail
   int  getAction(AnsiString action_type);
   void updateMDEngineer(unsigned short module_id, AnsiString a_engineer);
   void updateMDModuleName(unsigned short module_id, AnsiString a_description);
   void updateMDProductId(unsigned short module_id, int iProductId);
   void updateMDPartNumber(unsigned short module_id, AnsiString a_partnumber);
   void updateMDVersionAdded(unsigned short module_id, unsigned short usVersion);
   void updateMDVersionRemoved(unsigned short module_id, unsigned short usVersion);
   void updateMDMax(unsigned short module_id, unsigned short usMax);
   void updateMDAction(unsigned short module_id, AnsiString a_function);
   void updateMDiConvertDefault(unsigned short module_id, bool biConvert_default);
   void updateMDiConvertModule(unsigned short module_id, bool biConvert_module);
   void updateMDSpdDefault(unsigned short module_id, bool bspd_default);
   void updateMDSpdeDefault(unsigned short module_id, bool bspde_default);
   void updateMDConnectivityDefault(unsigned short module_id, bool bcon_default);
   void updateMDConnectivityModule(unsigned short module_id, bool bcon_module);

   void addItemToSModuleDetail(int product_id,
                        unsigned short module_id, AnsiString module_name,
                        AnsiString module_engineer,
                        AnsiString module_partnumber, unsigned short module_version_added,
                        unsigned short module_version_removed, AnsiString module_type,
                        unsigned short module_max, bool module_spd_def, bool module_spde_def,
                        bool module_connect_def, bool module_connect_mod,
                        bool module_iConvert_def, bool module_iConvert_mod);

   //These functions are used to update SKeyRecord
   void updateKeyDescription(unsigned short keyrecord_id, AnsiString a_description);
   void updateKeyPKNumber(unsigned short keyrecord_id, int iPKNumber);
   void updateKeyeBONumber(unsigned short keyrecord_id, AnsiString a_eBONumber);


   //module lookup members
   ModuleDetail** getModuleList(int productID);
   bool resetModuleList();//setModuleDetail();
   bool isAssignedModule(int idx);
};

//------------------------------------------------------------------------------
extern CLookup* lookup;
//------------------------------------------------------------------------------
#endif
