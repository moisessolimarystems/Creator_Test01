//------------------------------------------------------------------------------
// Filename:    clookups.cpp
// Note:        Copyright (c) 1993-2001 by Solimar Systems, Inc.
//              All Rights Reserved.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "key\newkey\keyprod.h"
#include "kyslevel.h"
#include "clookups.h"

#pragma package(smart_init)
//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------

//==============================================================================
// Function:    CLookup()
// Purpose:     Constructor
//==============================================================================
CLookup::CLookup()
{
   //get access to current database connection
   cDatabase = Session->FindDatabase("CustDB");
   cQuery = new TQuery(NULL);


   // initialize all module lists
   for( int mod_id=0; mod_id<64; mod_id++)
   {
      //initialize modules in module list
      //Create new structures,and assign new module id's to each one.
      module_detail[mod_id] = new ModuleDetail();
      module_detail[mod_id]->id = mod_id;

      ////initialize modules for iConvert module list
      //Create new structures,and assign new module id's to each one.
      iConvert_module_detail[mod_id] = new ModuleDetail();
      iConvert_module_detail[mod_id]->id = mod_id;

      ////initialize modules for iConvert module list
      //Create new structures,and assign new module id's to each one.
      m_solScriptModuleDetail[mod_id] = new ModuleDetail();
      m_solScriptModuleDetail[mod_id]->id = mod_id;

      ////initialize modules for SDX Designer module list
      //Create new structures,and assign new module id's to each one.
      m_SDXDesignerModuleDetail[mod_id] = new ModuleDetail();
      m_SDXDesignerModuleDetail[mod_id]->id = mod_id;
   }



   if(cDatabase)
   {
      cQuery->DatabaseName = cDatabase->DatabaseName;

      //query product information and setup product_list
      resetProductList();
      resetModuleList();
   }
}

//==============================================================================
// Function:    resetProductList
// Purpose:     resets the list in the database defined by the sProducts table
//              in the database.  Returns true if refreshed or false if it fails.
// Parameters:  None
// Returns:     bool
//==============================================================================
bool CLookup::resetProductList()
{
   bool result(true);

   try
   {
      cQuery->Close();
      cQuery->SQL->Add("select * from sProducts");
      cQuery->Open();

      cQuery->First();

      while(!cQuery->Eof)
      {
         int product_id(cQuery->FieldByName("Pid")->AsInteger);
         //check range
         if( product_id >= 0 && product_id <= MAX_SOLIMAR_PRODUCTS )
         {
            product_list[product_id].name = cQuery->FieldByName("Pname")->AsString;
            product_list[product_id].version = static_cast<unsigned short>(cQuery->FieldByName("Pversion")->AsInteger);
         }
         cQuery->Next();
      }
      cQuery->Close();
   }
   catch(...)
   {
      cQuery->Close();
      result=false;
   }
   return result;
}

//==============================================================================
// Function:    ~CLookup
// Purpose:     Destructor
//==============================================================================
CLookup::~CLookup()
{
   cQuery->Close();
   delete cQuery;

   //free memory associated with all module lists
   for( int mod_id=0; mod_id<64; mod_id++)
   {
      //free memory associated with module_list. (SPD or CONNECTIVITY)
      delete module_detail[mod_id];

      //free memory associated with iConvert_module_list. (ICONVERT)
      delete iConvert_module_detail[mod_id];

      delete m_solScriptModuleDetail[mod_id];

      delete m_SDXDesignerModuleDetail[mod_id];
   }
}

//==============================================================================
// Function:    reloadProducts
// Purpose:
// Parameters:  None
// Returns:     None
// Note:        Currently Does Nothing
//==============================================================================
void CLookup::reloadProducts()
{
}

//==============================================================================
// Function:    getProductVersion()
// Purpose:     returns the version associated with each product given in the
//              table sProducts.  The information is based upon the productId
//              number passed in.  If the productId = 4 (which is not used), then
//              we know it's SPD_DOS.
// Parameters:  unsigned short - product_id
//              bool - SPD_DOS
// Returns:     unsigned short
//==============================================================================
unsigned short CLookup::getCurrentVersion(unsigned short productId, bool SPD_DOS)
{
   //special case for spd legacy products
   if(SPD_DOS)
      return product_list[4].version;

   if(productId <= MAX_SOLIMAR_PRODUCTS)
   {
      return product_list[productId].version;
   }
   else
      return 0x0000;
}

//==============================================================================
// Function:    getProductText
// Purpose:     Returns the name of the product. (4 is not used.)  This info
//              is based upon the productId, and is drawn from the list that
//              was generated from sProducts and copied into a list.
// Parameters:  unsigned short - productId
//              unsigned short - version
// Returns:     AnsiString
//==============================================================================
AnsiString CLookup::getProductText(unsigned short productId, unsigned short version)
{
   if( productId <= MAX_SOLIMAR_PRODUCTS && productId != 4 )
   {
      //if spd product - check if legacy
      if(productId==0 && version < 0x400A)
         productId=4; //set to spd legacy

      return product_list[productId].name;
   }
   else
      return "Invalid product.";
}



//==============================================================================
// Function:    isAssignedModule
// Purpose:     This function returns true if the module corresponding to the
//              index passed in is an assigned module, and false if it is not.
// Parameters:  int idx - index
// Returns:     bool
// Note:        should be part of module list class
//==============================================================================
bool CLookup::isAssignedModule(int idx)
{
   return( &unassigned_module != module_detail[idx] );
}


//==============================================================================
// Function:    getModuleList
// Purpose:     returns the appropriate module list depending on the product id
//              given
// Parameters:  int productID - specifies SPD, Connectivity, or iConvert(7)
// Returns:     ModuleDetail**
//============================================================================== 
ModuleDetail** CLookup::getModuleList(int productID)
{
   if (productID == ICONVERT_PRODUCT)
       return iConvert_module_detail;
   else if (productID == SOLSCRIPT_PRODUCT)
      return m_solScriptModuleDetail;
   else if (productID == SDX_DESIGNER_PRODUCT)
      return m_SDXDesignerModuleDetail;
   else
       return module_detail;
}

//==============================================================================
// Function:    resetModuleList()
// Purpose:     refreshes the module lists.  This information is taken from the
//              sModuleDetail table in the database.  Returns true if it
//              refreshes, and false if it fails.
// Parameters:  None
// Returns:     bool
//==============================================================================
bool CLookup::resetModuleList()
{
   bool result(false);
   int mod_id; //module id is index into module_detail list
   int product_id; //product id specifies whether or not to use SPD or ICONVERT
   //int max_solscript;
   ModuleDetail** pModuleList(NULL);

   //initialize module contents in list to point to unassigned module defaults
   //unassigned_module = new ModuleDetail();
   for( mod_id=0; mod_id<64; mod_id++)
   {
      // refresh the contents, and assign the module id
      *module_detail[mod_id] = unassigned_module;
      module_detail[mod_id]->id = mod_id;

      // refresh the contents, and assign the module id
      *iConvert_module_detail[mod_id] = unassigned_module;
      iConvert_module_detail[mod_id]->id = mod_id;

      *m_solScriptModuleDetail[mod_id] = unassigned_module;
      m_solScriptModuleDetail[mod_id]->id = mod_id;

      *m_SDXDesignerModuleDetail[mod_id] = unassigned_module;
      m_SDXDesignerModuleDetail[mod_id]->id = mod_id;
   }

   //query module information
   try
   {
      cQuery->Close();
      cQuery->SQL->Clear();
      cQuery->SQL->Add("SELECT * FROM sModuleDetail");
      cQuery->Prepare();
      cQuery->Open();

      cQuery->First();

      while(!cQuery->Eof)
      {
         //----
         mod_id = cQuery->FieldByName("MDid")->AsInteger;
         product_id = cQuery->FieldByName("pId")->AsInteger;
         ///max_solscript = cQuery->FieldByName("MDmaxsolscript")->AsInteger;
         //
         // Set the module list for iConvert
         //
         if (product_id==ICONVERT_PRODUCT) {
            pModuleList = iConvert_module_detail;
         }
         else if (product_id==SOLSCRIPT_PRODUCT) {
            pModuleList = m_solScriptModuleDetail;
         }
         else if (product_id==SDX_DESIGNER_PRODUCT)
            pModuleList = m_SDXDesignerModuleDetail;
         else
            pModuleList = module_detail;

         if (mod_id >= 0 && mod_id < 64)
         {

            // Subtract 1000 because there are 64 values in the array, so each
            // module will correspond to each of these values.  There are labeled
            // 10XX or some other value in the sModuleDetail table.
            pModuleList[mod_id]->id = mod_id;
            pModuleList[mod_id]->pid = cQuery->FieldByName("pId")->AsInteger;
            pModuleList[mod_id]->name = cQuery->FieldByName("MDdescription")->AsString;
            pModuleList[mod_id]->version_added = cQuery->FieldByName("MDversionAdded")->AsInteger;
            pModuleList[mod_id]->version_removed = cQuery->FieldByName("MDversionRemoved")->AsInteger;
            pModuleList[mod_id]->max = cQuery->FieldByName("MDmax")->AsInteger;
            pModuleList[mod_id]->function = cQuery->FieldByName("MDaction")->AsInteger;
            pModuleList[mod_id]->spd_default = cQuery->FieldByName("MDspdDefault")->AsBoolean;
            pModuleList[mod_id]->con_default = cQuery->FieldByName("MDconnectivityDefault")->AsBoolean;
            pModuleList[mod_id]->con_module = cQuery->FieldByName("MDconnectivityModule")->AsBoolean;
            pModuleList[mod_id]->iConvert_default = cQuery->FieldByName("MDiConvertDefault")->AsBoolean;
            pModuleList[mod_id]->iConvert_module = cQuery->FieldByName("MDiConvertModule")->AsBoolean;
            pModuleList[mod_id]->engineer = cQuery->FieldByName("MDengineer")->AsString;
            pModuleList[mod_id]->description = cQuery->FieldByName("MDdescription")->AsString;
            pModuleList[mod_id]->partnumber = cQuery->FieldByName("MDpartnumber")->AsString;
            pModuleList[mod_id]->pages_per_minute = cQuery->FieldByName("MDpagesPerMinute")->AsBoolean;
            pModuleList[mod_id]->m_bQuantumDefault = cQuery->FieldByName("MDquantumDefault")->AsBoolean;
            pModuleList[mod_id]->m_bQuantumModule = cQuery->FieldByName("MDquantumModule")->AsBoolean;
            //pModuleList[mod_id]->max_solscript = cQuery->FieldByName("MDmaxsolscript")->AsInteger;
            //
            // Keep track of the modules to see if they are existing members or not.
            if (pModuleList[mod_id]->name == "{ Not Used }") {
                pModuleList[mod_id]->bExistingMember = false;
            }
            else
                pModuleList[mod_id]->bExistingMember = true;
         }

         //----
         cQuery->Next();

      }
      cQuery->Close();
      result = true;
   }
   catch(...)
   {
      cQuery->Close();
   }

   return result;
}

//==============================================================================
// Function:    updateVersion()
// Purpose:     Saves new version for specifed product to the database.  All
//              version checking should already be done.
//              updates sProducts table in the database
// Parameters:  unsigned short - product_id
//              unsigned short - version
// Returns:     None
//==============================================================================
void CLookup::updateVersion(unsigned short product_id, unsigned short version)
{
   //all products can have their version updated except SPD Legacy
   if(product_id <= MAX_SOLIMAR_PRODUCTS && product_id != 4)
   {
      //update product information
      try
      {
        cQuery->Close();
        cQuery->SQL->Clear();
        cQuery->SQL->Add("update sProducts SET Pversion = :version WHERE Pid = :id");
        cQuery->ParamByName("version")->AsInteger = version;
        cQuery->ParamByName("id")->AsInteger = product_id;
        cQuery->ExecSQL();
      }
      catch(EDBEngineError &e)
      {
        Application->MessageBox(e.Message.c_str(), "Database Error", MB_OK|MB_ICONERROR);
      }

      //refresh product list
      resetProductList();
   } // end if

}

//==============================================================================
// Function:    updateMDVersionRemoved
// Purpose:     will update the version removed corresponding to the module id.
//              Updates sModuleDetail table in the database.
// Parameters:  unsigned short module_id - module id
//              unsigned short usVersion - version removed
// Returns:     None
//==============================================================================
void CLookup::updateMDVersionRemoved(unsigned short module_id, unsigned short usVersion)
{
     //update product information
     try
     {
        cQuery->Close();
        cQuery->SQL->Clear();
        cQuery->SQL->Add("update sModuleDetail SET mdVersionRemoved = :version WHERE mdId = :id");
        cQuery->ParamByName("version")->AsSmallInt = usVersion;
        cQuery->ParamByName("id")->AsInteger = module_id;
        cQuery->ExecSQL();
     }
     catch(EDBEngineError &e)
     {
        Application->MessageBox(e.Message.c_str(), "Database Error", MB_OK|MB_ICONERROR);
     }
     //refresh Module list
     resetModuleList();
}

//==============================================================================
// Function:    updateMDVersionAdded
// Purpose:     will update the version added corresponding to the module id.
//              Updates sModuleDetail table in the database.
// Parameters:  unsigned short module_id - module id
//              unsigned short usVersion - version added
// Returns:     None
//==============================================================================
void CLookup::updateMDVersionAdded(unsigned short module_id, unsigned short usVersion)
{
     //update module information
     try
     {
        cQuery->Close();
        cQuery->SQL->Clear();
        cQuery->SQL->Add("update sModuleDetail SET mdVersionAdded = :version WHERE mdId = :id");
        cQuery->ParamByName("version")->AsSmallInt= usVersion;
        cQuery->ParamByName("id")->AsInteger = module_id;
        cQuery->ExecSQL();
     }
     catch(EDBEngineError &e)
     {
        Application->MessageBox(e.Message.c_str(), "Database Error", MB_OK|MB_ICONERROR);
     }
     //refresh module list
     resetModuleList();
}

//==============================================================================
// Function:    updateMDEngineer
// Purpose:     will update the field in the database that has the name of the
//              engineer that worked on a particular module.  Associated by the
//              module id.
//              Updates sModuleDetail table in the database.
// Parameters:  unsigned short module_id - module id
//              AnsiString  a_engineer - name of the engineer to be put into database
// Returns:     None
//==============================================================================
void CLookup::updateMDEngineer(unsigned short module_id, AnsiString a_engineer)
{
     //update product information
     try
     {
        cQuery->Close();
        cQuery->SQL->Clear();
        cQuery->SQL->Add("update sModuleDetail SET mdEngineer = :engineer WHERE mdId = :id");
        cQuery->ParamByName("engineer")->AsString = a_engineer;
        cQuery->ParamByName("id")->AsInteger = module_id;
        cQuery->ExecSQL();
     }
     catch(EDBEngineError &e)
     {
        Application->MessageBox(e.Message.c_str(), "Database Error", MB_OK|MB_ICONERROR);
     }

     //refresh module list
     resetModuleList();
}

//==============================================================================
// Function:    updateMDProductId
// Purpose:     will update the field in the database that has the product Id.
//              Updates sModuleDetail table in the database.
// Parameters:  unsigned short module_id - module id
//              AnsiString  a_engineer - name of the engineer to be put into database
// Returns:     None
//==============================================================================
void CLookup::updateMDProductId(unsigned short module_id, int iProductId)
{
     //update product information
     try
     {
        cQuery->Close();
        cQuery->SQL->Clear();
        cQuery->SQL->Add("update sModuleDetail SET pId = :prodcutid WHERE mdId = :id");
        cQuery->ParamByName("productid")->AsInteger = iProductId;
        cQuery->ParamByName("id")->AsInteger = module_id;
        cQuery->ExecSQL();
     }
     catch(EDBEngineError &e)
     {
        Application->MessageBox(e.Message.c_str(), "Database Error", MB_OK|MB_ICONERROR);
     }

     //refresh module list
     resetModuleList();
}

//==============================================================================
// Function:    updateMDModuleName
// Purpose:     will update the field in the database that has a description of
//              the module (module name). Associated by the module id.
//              Updates sModuleDetail table in the database.
// Parameters:  unsigned short module_id - module id
//              AnsiString  a_description - name of the module
// Returns:     None
//==============================================================================
void CLookup::updateMDModuleName(unsigned short module_id, AnsiString a_description)
{
     //update product information
     try
     {
        cQuery->Close();
        cQuery->SQL->Clear();
        cQuery->SQL->Add("update sModuleDetail SET mdDescription = :description WHERE mdId = :id");
        cQuery->ParamByName("description")->AsString = a_description;
        cQuery->ParamByName("id")->AsInteger = module_id;
        cQuery->ExecSQL();
     }
     catch(EDBEngineError &e)
     {
        Application->MessageBox(e.Message.c_str(), "Database Error", MB_OK|MB_ICONERROR);
     }
     //refresh product list
     resetModuleList();
}

//==============================================================================
// Function:    getAction
// Purpose:     returns a value 0, 1, 2, or 3 corresponding to what string is
//              passed in...i.e. Input=1, Emulation=2, Output=3,
//              LU Type=4, Job Event=5, Emailer=6 or Undefined=0.
//              Updates sModuleDetail table in the database.
// Parameters:  AnsiString action_type
// Returns:     int
//==============================================================================
int  CLookup::getAction(AnsiString action_type)
{
     int iAction;
     if (action_type == "Input")
        iAction = 1; //mtInitiator;
     else if (action_type == "Emulation")
        iAction = 2; //mtGateway;
     else if (action_type == "Output")
        iAction = 3; //mtTerminator;
     else if (action_type == "LU")
        iAction = 4;
     else if (action_type == "Job Event")
        iAction = 5;
     else if (action_type == "Emailer")
        iAction = 6;
     else
        iAction = NULL; //Undefined

     return iAction;
}
//==============================================================================
// Function:    updateMDAction
// Purpose:     will update the field in the database that contains the type of
//              module that it is..input = 1, emulation = 2, output = 3, undefined.
//              Updates sModuleDetail table in the database.
// Parameters:  unsigned short module_id - module id
//              AnsiString  a_function - type of module..described in the purpose.
// Returns:     None
//==============================================================================
void CLookup::updateMDAction(unsigned short module_id, AnsiString a_function)
{
     int iAction;
     iAction = getAction(a_function);

     //update product information
     try
     {
        cQuery->Close();
        cQuery->SQL->Clear();
        cQuery->SQL->Add("update sModuleDetail SET mdAction = :function WHERE mdId = :id");
        cQuery->ParamByName("function")->AsInteger = iAction;
        cQuery->ParamByName("id")->AsInteger = module_id;
        cQuery->ExecSQL();
     }

     catch(EDBEngineError &e)
     {
        Application->MessageBox(e.Message.c_str(), "Database Error", MB_OK|MB_ICONERROR);
     }
     //refresh module list
     resetModuleList();
}

//==============================================================================
// Function:    updateMDPartNumber
// Purpose:     will update the part number associated with each module.
//              Updates sModuleDetail table in the database.
// Parameters:  unsigned short module_id - module id
//              AnsiString  a_partnumber - name of the engineer to be put into
//                                         database
// Returns:     None
//==============================================================================
void CLookup::updateMDPartNumber(unsigned short module_id, AnsiString a_partnumber)
{
     //update module information
     try
     {
        cQuery->Close();
        cQuery->SQL->Clear();
        cQuery->SQL->Add("update sModuleDetail SET mdPartNumber = :partnumber WHERE mdId = :id");
        cQuery->ParamByName("partnumber")->AsString = a_partnumber;
        cQuery->ParamByName("id")->AsInteger = module_id;
        cQuery->ExecSQL();
     }
     catch(EDBEngineError &e)
     {
        Application->MessageBox(e.Message.c_str(), "Database Error", MB_OK|MB_ICONERROR);
     }

     //refresh module list
     resetModuleList();
}

//==============================================================================
// Function:    updateMDMax
// Purpose:     will update the mdMax field associated with each module.
//              Updates sModuleDetail table in the database.
// Parameters:  unsigned short module_id - module id
//              unsigned short usMax - max value to be put into database
// Returns:     None
//==============================================================================
void CLookup::updateMDMax(unsigned short module_id, unsigned short usMax)
{
     //update module information
     try
     {
        cQuery->Close();
        cQuery->SQL->Clear();
        cQuery->SQL->Add("update sModuleDetail SET mdMax = :max WHERE mdId = :id");
        cQuery->ParamByName("max")->AsInteger = usMax;
        cQuery->ParamByName("id")->AsInteger = module_id;
        cQuery->ExecSQL();
     }
     catch(EDBEngineError &e)
     {
        Application->MessageBox(e.Message.c_str(), "Database Error", MB_OK|MB_ICONERROR);
     }

     //refresh Module list
     resetModuleList();
}

//==============================================================================
// Function:    updateMDSpdDefault
// Purpose:     will update the mdSpdDefault field associated with each module.
//              Updates sModuleDetail table in the database.
// Parameters:  unsigned short module_id - module id
//              bool bspd_default - true if spd_default
// Returns:     None
//==============================================================================
void CLookup::updateMDSpdDefault(unsigned short module_id, bool bspd_default)
{
     //update module information
     try
     {
        cQuery->Close();
        cQuery->SQL->Clear();
        cQuery->SQL->Add("update sModuleDetail SET mdSpdDefault = :spddef WHERE mdId = :id");
        cQuery->ParamByName("spddef")->AsBoolean = bspd_default;
        cQuery->ParamByName("id")->AsInteger = module_id;
        cQuery->ExecSQL();
     }
     catch(EDBEngineError &e)
     {
        Application->MessageBox(e.Message.c_str(), "Database Error", MB_OK|MB_ICONERROR);
     }

     //refresh module list
     resetModuleList();
}

//==============================================================================
// Function:    updateMDConnectivityDefault
// Purpose:     will update the mdConnectivityDefault field associated with each
//              module.
//              Updates sModuleDetail table in the database.
// Parameters:  unsigned short module_id - module id
//              bool bcon_default - true if con_default
// Returns:     None
//==============================================================================
void CLookup::updateMDConnectivityDefault(unsigned short module_id, bool bcon_default)
{
     //update module information
     try
     {
        cQuery->Close();
        cQuery->SQL->Clear();
        cQuery->SQL->Add("update sModuleDetail SET mdConnectivityDefault = :condef WHERE mdId = :id");
        cQuery->ParamByName("condef")->AsBoolean = bcon_default;
        cQuery->ParamByName("id")->AsInteger = module_id;
        cQuery->ExecSQL();
     }
     catch(EDBEngineError &e)
     {
        Application->MessageBox(e.Message.c_str(), "Database Error", MB_OK|MB_ICONERROR);
     }

     //refresh module list
     resetModuleList();
}

//==============================================================================
// Function:    updateMDiConvertDefault
// Purpose:     will update the mdiConvertDefault field associated with each
//              module.
//              Updates sModuleDetail table in the database.
// Parameters:  unsigned short module_id - module id
//              bool biConvert_default - true if iConvert_default
// Returns:     None
//==============================================================================
void CLookup::updateMDiConvertDefault(unsigned short module_id, bool biConvert_default)
{
     //update module information
     try
     {
        cQuery->Close();
        cQuery->SQL->Clear();
        cQuery->SQL->Add("update sModuleDetail SET mdiConvertDefault = :iConvertdef WHERE mdId = :id");
        cQuery->ParamByName("iConvertdef")->AsBoolean = biConvert_default;
        cQuery->ParamByName("id")->AsInteger = module_id;
        cQuery->ExecSQL();
     }
     catch(EDBEngineError &e)
     {
        Application->MessageBox(e.Message.c_str(), "Database Error", MB_OK|MB_ICONERROR);
     }

     //refresh module list
     resetModuleList();
}

//==============================================================================
// Function:    updateMDConnectivityModule
// Purpose:     will update the mdConnectivityModule field associated with each
//              module.
//              Updates sModuleDetail table in the database.
// Parameters:  unsigned short module_id - module id
//              bool bcon_module - true if connectivity module
// Returns:     None
//==============================================================================
void CLookup::updateMDConnectivityModule(unsigned short module_id, bool bcon_module)
{
     //update module information
     try
     {
        cQuery->Close();
        cQuery->SQL->Clear();
        cQuery->SQL->Add("update sModuleDetail SET mdConnectivityModule = :conmod WHERE mdId = :id");
        cQuery->ParamByName("conmod")->AsBoolean = bcon_module;
        cQuery->ParamByName("id")->AsInteger = module_id;
        cQuery->ExecSQL();
     }
     catch(EDBEngineError &e)
     {
        Application->MessageBox(e.Message.c_str(), "Database Error", MB_OK|MB_ICONERROR);
     }

     //refresh module list
     resetModuleList();
}
//==============================================================================
// Function:    updateMDiConvertModule
// Purpose:     will update the mdiConvertModule field associated with each
//              module.
//              Updates sModuleDetail table in the database.
// Parameters:  unsigned short module_id - module id
//              bool biConvert_module - true if iConvert module
// Returns:     None
//==============================================================================
void CLookup::updateMDiConvertModule(unsigned short module_id, bool biConvert_module)
{
     //update module information
     try
     {
        cQuery->Close();
        cQuery->SQL->Clear();
        cQuery->SQL->Add("update sModuleDetail SET mdiConvertModule = :iConvertmod WHERE mdId = :id"); // AND pId = :productID");
        cQuery->ParamByName("iConvertmod")->AsBoolean = biConvert_module;
        cQuery->ParamByName("id")->AsInteger = module_id;
        // Alex - Add this to all fields
        //cQuery->ParamByName("productID")->AsInteger = pID;
        cQuery->ExecSQL();
     }
     catch(EDBEngineError &e)
     {
        Application->MessageBox(e.Message.c_str(), "Database Error", MB_OK|MB_ICONERROR);
     }

     //refresh module list
     resetModuleList();
}

//==============================================================================
// Function:    addItemToSModuleDetail
// Purpose:     this function will add new field to the database.
// Parameters:  unsigned short  module_id
//              AnsiString      module_engineer
//              unsigned short  module_version_added
//              unsigned short  module_version_removed
//              int             module_type
//              unsigned short  module_max
//              bool            module_spd_def
//              bool            module_connect_def
//              bool            module_connect_mod
// Returns:     None
//==============================================================================
void CLookup::addItemToSModuleDetail(int product_id,
                                unsigned short module_id, AnsiString module_name,
                                AnsiString module_engineer, AnsiString module_partnumber,
                                unsigned short module_version_added,
                                unsigned short module_version_removed, AnsiString module_type,
                                unsigned short module_max, bool module_spd_def,
                                bool module_connect_def, bool module_connect_mod,
                                bool module_iConvert_def, bool module_iConvert_mod )
{
     //
     // Change the action from a string to the integer corresponding to it.
     int iAction;
     iAction = getAction(module_type);
     
     //update module information
     try
     {

        cQuery->Close();
        cQuery->SQL->Clear();
        cQuery->SQL->Add("INSERT INTO sModuleDetail (pId, mdId, mdDescription, mdEngineer, mdPartNumber, mdVersionAdded, mdVersionRemoved, mdAction, mdMax, mdSpdDefault, mdConnectivityDefault, mdConnectivityModule, mdiConvertDefault, mdiConvertModule) values (:productID, :id, :name, :engineer, :partnumber, :version_added, :version_removed, :type, :max, :spddef, :connectdef, :connectmod, :iConvertdef, :iConvertmod)");
        cQuery->ParamByName("productID")->AsInteger = product_id;
        cQuery->ParamByName("id")->AsInteger = module_id;
        cQuery->ParamByName("name")->AsString = module_name;
        cQuery->ParamByName("engineer")->AsString = module_engineer;
        cQuery->ParamByName("partnumber")->AsString = module_partnumber;
        cQuery->ParamByName("version_added")->AsSmallInt = module_version_added;
        cQuery->ParamByName("version_removed")->AsSmallInt = module_version_removed;
        cQuery->ParamByName("type")->AsInteger = iAction;
        cQuery->ParamByName("max")->AsInteger = module_max;
        cQuery->ParamByName("spddef")->AsBoolean = module_spd_def;
        cQuery->ParamByName("connectdef")->AsBoolean = module_connect_def;
        cQuery->ParamByName("connectmod")->AsBoolean = module_connect_mod;
        cQuery->ParamByName("iConvertdef")->AsBoolean = module_iConvert_def;
        cQuery->ParamByName("iConvertmod")->AsBoolean = module_iConvert_mod;
        cQuery->Prepare();
        cQuery->ExecSQL();

     }
     catch(EDBEngineError &e)
     {
        Application->MessageBox(e.Message.c_str(), "Database Error", MB_OK|MB_ICONERROR);
     }

     //refresh module list
     resetModuleList();
}

//==============================================================================
// Function:    updateKeyDescription
// Purpose:     this function will update the description field in SKeyRecord in
//              the database
// Parameters:  unsigned short  keyrecord_id
//              AnsiString      a_description
// Returns:     None
//==============================================================================
void CLookup::updateKeyDescription(unsigned short keyrecord_id, AnsiString a_description)
{
     //update SKeyRecord information
     try
     {
        cQuery->Close();
        cQuery->SQL->Clear();
        cQuery->SQL->Add("update SKeyRecord SET skrDescription = :desc WHERE skrId = :id");
        cQuery->ParamByName("desc")->AsString = a_description;
        cQuery->ParamByName("id")->AsInteger = keyrecord_id;
        cQuery->ExecSQL();
     }
     catch(EDBEngineError &e)
     {
        Application->MessageBox(e.Message.c_str(), "Database Error", MB_OK|MB_ICONERROR);
     }
     // Update the Product List and the ModuleList.
     resetProductList();
     resetModuleList();
}

//==============================================================================
// Function:    updateKeyPKNumber
// Purpose:     this function will update the description field in SKeyRecord in
//              the database
// Parameters:  unsigned short  keyrecord_id
//              int      iPKNumber
// Returns:     None
//==============================================================================
void CLookup::updateKeyPKNumber(unsigned short keyrecord_id, int iPKNumber)
{
     //update SKeyRecord information
     try
     {
        cQuery->Close();
        cQuery->SQL->Clear();
        cQuery->SQL->Add("update SKeyRecord SET skrPo = :po WHERE skrId = :id");
        cQuery->ParamByName("po")->AsInteger = iPKNumber;
        cQuery->ParamByName("id")->AsInteger = keyrecord_id;
        cQuery->ExecSQL();
     }
     catch(EDBEngineError &e)
     {
        Application->MessageBox(e.Message.c_str(), "Database Error", MB_OK|MB_ICONERROR);
     }

     // Update the Product List and the ModuleList.
     resetProductList();
     resetModuleList();
}

//==============================================================================
// Function:    updateKeyeBONumber
// Purpose:     this function will update the epicore BackOffice order number
//              field in SKeyRecord in the database
// Parameters:  unsigned short  keyrecord_id
//              AnsiString      a_eBONumber
// Returns:     None
//==============================================================================
void CLookup::updateKeyeBONumber(unsigned short keyrecord_id, AnsiString a_eBONumber)
{
     //update SKeyRecord information
     try
     {
        cQuery->Close();
        cQuery->SQL->Clear();
        cQuery->SQL->Add("update SKeyRecord SET skreBOnumber = :eBO WHERE skrId = :id");
        cQuery->ParamByName("eBO")->AsString = a_eBONumber;
        cQuery->ParamByName("id")->AsInteger = keyrecord_id;
        cQuery->ExecSQL();
     }
     catch(EDBEngineError &e)
     {
        Application->MessageBox(e.Message.c_str(), "Database Error", MB_OK|MB_ICONERROR);
     }
     // Update the Product List and the ModuleList.
     resetProductList();
     resetModuleList();
}


//==============================================================================
// ModuleDetail() - if max == 0 module is unused
//==============================================================================
ModuleDetail::ModuleDetail(): name("{ Not Used }"),
                              version_added(0),
                              version_removed(0xFFFF),
                              max(0xF),
                              function(mtIlleagal),
                              con_module(false),
                              con_default(false),
                              spd_default(false),
                              iConvert_default(false),
                              iConvert_module(false),
                              engineer("N/A"),
                              partnumber("N/A"),
                              pages_per_minute(false),
                              bExistingMember(false),
                              m_bQuantumModule(false),
                              m_bQuantumDefault(false)
                              {}

//==============================================================================
// Function:    getActionText()
// Purpose:     Returns the type or name of a particular module.  There are three
//              valid choices...Input, Emulation, or Output.  Unused modules will
//              have Undefined returned.
// Parameters:  None
// Returns:     char *
//==============================================================================
char* ModuleDetail::getActionText()
{
   if( function == mtInitiator )
      return "Input";
   else if( function == mtGateway )
      return "Emulation";
   else if( function == mtTerminator )
      return "Output";
   else if ( function == mtLUType )
      return "LU";
   else if ( function == mtJobEvent )
      return "Job Event";
   else if ( function == mtEmailer )
      return "Emailer";
   else
      return "Undefined";
}

//==============================================================================
// Function:    getTextForUnits
// Purpose:     should only be called on a module that is available for the
//              current product.
// Parameters:  unsigned short
//              unsigned char
// Returns:     AnsiString
//==============================================================================
AnsiString ModuleDetail::getTextForUnits(unsigned short product_id, unsigned char units)
{
   AnsiString result_string(units);

   if( units != 0 )
   {
      if(isDefaultForProduct(product_id))
         result_string = "*";
      else if(max == 1)
         result_string = "X";
      //else # of units
   }
   return result_string;
}

//==============================================================================
// Function:    isAvailableForVersion
// Purpose:
// Parameters:  unsigned short
// Returns:     bool
//==============================================================================
bool ModuleDetail::isAvailableForVersion(unsigned short version)
{
   if(version_added <= version && version_removed >= version )
      return true;
   else
      return false;
}

//==============================================================================
// Function:    isAvailableForProduct
// Purpose:
// Parameters:  unsigned short
// Returns:     bool
//==============================================================================
bool ModuleDetail::isAvailableForProduct(unsigned short product)
{
   bool result;
   switch(product)
   {
      case SPD_PRODUCT:
         result = true;         //spd platinum supports all modules
         break;
      case CONNECT_PRODUCT:
         result = con_module;   // supports connectivity server modules
         break;
      case QUANTUM_PRODUCT:
         result = m_bQuantumModule;
         break;
      case ICONVERT_PRODUCT:
         result = iConvert_module;   // supports iConvert modules.
         break;
      case SOLSCRIPT_PRODUCT:
         result = static_cast<bool>(max>0);
         break;
      case SDX_DESIGNER_PRODUCT:
         result = static_cast<bool>(max>0);
         break;
      default:
         result = false;
   }
   return result;
}

//==============================================================================
// Function:    isDefaultForProduct
// Purpose:     Generates module password.  Generating the module unit password
//              by selecting it is part of the old process.  This should only be
//              used for keys of type SOLIMAR once the clientel system is
//              intergrated with creator.
// Parameters:  unsigned short
// Returns:     bool
//==============================================================================
bool ModuleDetail::isDefaultForProduct(unsigned short product)
{
   if(product == SPD_PRODUCT)
      return spd_default;
   else if(product == CONNECT_PRODUCT)
      return con_default;
   else if(product == QUANTUM_PRODUCT)
      return m_bQuantumDefault;
   else if(product == ICONVERT_PRODUCT)
      return iConvert_default;
   else
      return false;
}

