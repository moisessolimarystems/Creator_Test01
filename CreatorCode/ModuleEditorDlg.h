//------------------------------------------------------------------------------
// Filename:    ModuleEditorDlg.h
//------------------------------------------------------------------------------

#ifndef ModuleEditorDlgH
#define ModuleEditorDlgH
//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
#include "ModuleEditor.h"

//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//              Class: TModuleEditorForm
//------------------------------------------------------------------------------
class TModuleEditorForm : public TForm
{
__published:	// IDE-managed Components
        TBitBtn *Cancel;
        TBitBtn *OK;
        TModuleEditor *ModuleEditor1;
        void __fastcall ModuleFrame1ModuleListDblClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TModuleEditorForm(TComponent* Owner);
        unsigned short getModuleID(); // returns the module id number
        unsigned short getModuleProductID(); // returns the product id associated with the module
        unsigned short getModuleVersionAdded(); // returns the version that the module was introduced
        unsigned short getModuleVersionRemoved(); // returns the version that a module was removed
        unsigned short getModuleMax(); // returns the max value for mdMax in the database
        AnsiString getModuleName();   // returns module name
        AnsiString getModuleType();   // returns input, emulation, or output
        AnsiString getModuleEngineer();// returns engineer that worked on module
        AnsiString getModulePartNumber(); // returns part number associate w/ module
        bool getModuleSpdDefault(); // returns true if Spd Default
        bool getModuleSpdeDefault(); // returns true if Spd Default
        bool getModuleiConvertDefault(); // returns true if iConvert Default
        bool getModuleiConvertModule(); // returns true if iConvert Module
        bool getModuleConnectivityDefault(); // returns true if Connectivity Default
        bool getModuleConnectivityModule(); // returns true if Connectivity Module
        bool getExistingMemberStatus(); // returns true if it is an Existing member in the database


        // Member variables
        unsigned short mod_pid;
        unsigned short mod_id;
        unsigned short mod_max;
        unsigned short mod_version_added;
        unsigned short mod_version_removed;
        AnsiString mod_name;
        AnsiString mod_type;
        AnsiString mod_engineer;
        AnsiString mod_partnumber;
        bool mod_spd_def;
        bool mode_spde_def;
        bool mod_connect_def;
        bool mod_connect_mod;
        bool mod_iConvert_def;
        bool mod_iConvert_mod;
        bool bExistingMember;
};
//---------------------------------------------------------------------------
extern PACKAGE TModuleEditorForm *ModuleEditorForm;
//---------------------------------------------------------------------------
#endif
