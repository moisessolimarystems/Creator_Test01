//------------------------------------------------------------------------------
// Filename:    ModuleEditorDlg.cpp
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include "ModuleEditorDetailDlg.h"

#include <vcl.h>
#pragma hdrstop

#include "ModuleEditorDlg.h"
#include "ModuleEditor.h"
#include "clookups.h"


#pragma package(smart_init)
#pragma link "ModuleEditor"
#pragma link "ModuleEditor"
#pragma resource "*.dfm"
//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------
TModuleEditorForm *ModuleEditorForm;


//==============================================================================
// Function:    TModuleEditorForm
// Purpose:     Constructor
//==============================================================================
__fastcall TModuleEditorForm::TModuleEditorForm(TComponent* Owner)
        : TForm(Owner)
{
      ModuleEditorForm->ClientWidth = 800;
      ModuleEditorForm->ClientHeight = 375;
      ModuleEditor1->Width = 800;
      ModuleEditor1->ModuleFrame1->Width = 775;
      ModuleEditor1->ModuleFrame1->ModuleList->Width = 750;
      
}

//==============================================================================
// Function:    ModuleFrame1ModuleListDblClick
// Purpose:     Handler function when you double click on an item in the module
//              list.
// Parameters:  TObject
// Returns:     None
//==============================================================================
void __fastcall TModuleEditorForm::ModuleFrame1ModuleListDblClick(
      TObject *Sender)
{
      //
      // NOTE: This function was originally going to be used so that users could
      // Update the database with part numbers. Since this never worked out,
      // this event is no longer needed. It is available if we do ever need it.
      return;

/*
      // Commented out to suppress warning messages, but keep the code.

      char addver[256];
      char remver[256];
      //
      // Initialize the member variables with the appropriate values
      mod_id = getModuleID();
      mod_pid = getModuleProductID();
      mod_name = getModuleName();
      mod_type = getModuleType();
      mod_engineer = getModuleEngineer();
      mod_version_added = getModuleVersionAdded();
      mod_version_removed = getModuleVersionRemoved();
      mod_partnumber = getModulePartNumber();
      mod_max = getModuleMax();
      mod_spd_def = getModuleSpdDefault();
      mod_connect_def = getModuleConnectivityDefault();
      mod_connect_mod = getModuleConnectivityModule();
      mod_iConvert_def = getModuleiConvertDefault();
      mod_iConvert_mod = getModuleiConvertModule();
      bExistingMember = getExistingMemberStatus();

      // change the values that we got for the versions from decimal to hex
      ltoa(mod_version_added, addver, 16);
      ltoa(mod_version_removed, remver, 16);

      // create a dialog, and set the values to display the selected item in the list
      TModuleEditorDetailsDlg *modEditorDetailsDlg = new TModuleEditorDetailsDlg(this);

      modEditorDetailsDlg->ProductIDLabel->Caption = mod_pid;
      modEditorDetailsDlg->ModIDNumberLabel->Caption = mod_id;
      modEditorDetailsDlg->ModNameEdit->Text = mod_name;
      modEditorDetailsDlg->ModTypeCombo->Text = mod_type;
      modEditorDetailsDlg->ModEngineerCombo->Text = mod_engineer;
      modEditorDetailsDlg->ModVersionAddedEdit->Text = addver;
      modEditorDetailsDlg->ModVersionRemovedEdit->Text = remver;
      modEditorDetailsDlg->ModPartNumberEdit->Text = mod_partnumber;
      modEditorDetailsDlg->ModMaxEdit->Text = mod_max;
      modEditorDetailsDlg->ModSpdDefaultCheckBox->Checked = mod_spd_def;
      modEditorDetailsDlg->ModConnectivityDefaultCheckBox->Checked = mod_connect_def;
      modEditorDetailsDlg->ModConnectivityModuleCheckBox->Checked = mod_connect_mod;
      modEditorDetailsDlg->ModiConvertModuleCheckBox->Checked = mod_iConvert_mod;
      modEditorDetailsDlg->ModiConvertDefaultCheckBox->Checked = mod_iConvert_def;



      modEditorDetailsDlg->bExistingMember = bExistingMember;
      if ( modEditorDetailsDlg->ShowModal() == mrOk )
      {
         // Need to update the list and refresh the dialog
         ModuleEditor1->ModuleFrame1->load(NULL, true);
         ModuleEditor1->ModuleFrame1->Repaint();
         bExistingMember = modEditorDetailsDlg->bExistingMember;
      }

      // delete the instance of the dialog before exiting the function.
      if( modEditorDetailsDlg )
      {
   	delete modEditorDetailsDlg;
        modEditorDetailsDlg = NULL;
      }
*/
}

//==============================================================================
// Function:    getExistingMemberStatus();
// Purpose:     returns true or false....if exists in database or not.
// Parameters:  None
// Returns:     bool
//==============================================================================
bool TModuleEditorForm::getExistingMemberStatus()
{
     TListItem* selected(ModuleEditor1->ModuleFrame1->ModuleList->Selected);
     if(selected==NULL)
        return false;

     ModuleDetail* detail = static_cast<ModuleDetail*>(selected->Data);
     return (detail->bExistingMember);
}

//==============================================================================
// Function:    getModuleID();
// Purpose:     returns the module ID of the selected item
// Parameters:  None
// Returns:     None
//==============================================================================
unsigned short TModuleEditorForm::getModuleID()
{
     TListItem* selected(ModuleEditor1->ModuleFrame1->ModuleList->Selected);
     if(selected==NULL)
        return NULL;

     ModuleDetail* detail = static_cast<ModuleDetail*>(selected->Data);
     return (detail->id);
}

//==============================================================================
// Function:    getModuleProductID();
// Purpose:     returns the product ID for the module of the selected item
// Parameters:  None
// Returns:     None
//==============================================================================
unsigned short TModuleEditorForm::getModuleProductID()
{
     TListItem* selected(ModuleEditor1->ModuleFrame1->ModuleList->Selected);
     if(selected==NULL)
        return NULL;

     ModuleDetail* detail = static_cast<ModuleDetail*>(selected->Data);
     return (detail->pid);
}

//==============================================================================
// Function:    getModuleName();
// Purpose:     returns the module name of the selected item
// Parameters:  None
// Returns:     AnsiString
//==============================================================================
AnsiString TModuleEditorForm::getModuleName()
{
     TListItem* selected(ModuleEditor1->ModuleFrame1->ModuleList->Selected);
     if(selected==NULL)
        return "Invalid Module Name";

     ModuleDetail* detail = static_cast<ModuleDetail*>(selected->Data);
     return (detail->name );
}

//==============================================================================
// Function:    getModuleType();
// Purpose:     returns the module type....input, emulation, or output
// Parameters:  None
// Returns:     AnsiString
//==============================================================================
AnsiString TModuleEditorForm::getModuleType()
{
     TListItem* selected(ModuleEditor1->ModuleFrame1->ModuleList->Selected);
     if(selected==NULL)
        return "Invalid Module Type";

     ModuleDetail* detail = static_cast<ModuleDetail*>(selected->Data);
     return (detail->getActionText());
}

//==============================================================================
// Function:    getModuleEngineer();
// Purpose:     returns the name of the engineer that worked on the module
// Parameters:  None
// Returns:     AnsiString
//==============================================================================
AnsiString TModuleEditorForm::getModuleEngineer()
{
     TListItem* selected(ModuleEditor1->ModuleFrame1->ModuleList->Selected);
     if(selected==NULL)
        return "N/A";

     ModuleDetail* detail = static_cast<ModuleDetail*>(selected->Data);
     return ( detail->engineer );
}

//==============================================================================
// Function:    getModuleVersionAdded();
// Purpose:     returns the version that this particular module was introduced into
//              the software.
// Parameters:  None
// Returns:     unsigned short
//==============================================================================
unsigned short TModuleEditorForm::getModuleVersionAdded()
{
     TListItem* selected(ModuleEditor1->ModuleFrame1->ModuleList->Selected);
     if(selected==NULL)
        return -1;

     ModuleDetail* detail = static_cast<ModuleDetail*>(selected->Data);
     return ( detail->version_added );
}

//==============================================================================
// Function:    getModuleVersionRemoved();
// Purpose:     returns the version that this particular module was introduced into
//              the software.
// Parameters:  None
// Returns:     unsigned short
//==============================================================================
unsigned short TModuleEditorForm::getModuleVersionRemoved()
{
     TListItem* selected(ModuleEditor1->ModuleFrame1->ModuleList->Selected);
     if(selected==NULL)
        return -1;

     ModuleDetail* detail = static_cast<ModuleDetail*>(selected->Data);
     return ( detail->version_removed);
}


//==============================================================================
// Function:    getModulePartNumber();
// Purpose:     returns the part number associated with each module
// Parameters:  None
// Returns:     AnsiString
//==============================================================================
AnsiString TModuleEditorForm::getModulePartNumber()
{
     TListItem* selected(ModuleEditor1->ModuleFrame1->ModuleList->Selected);
     if(selected==NULL)
        return NULL;

     ModuleDetail* detail = static_cast<ModuleDetail*>(selected->Data);
     return (detail->partnumber);
}

//==============================================================================
// Function:    getModuleMax();
// Purpose:     returns the module max of the selected item
// Parameters:  None
// Returns:     None
//==============================================================================
unsigned short TModuleEditorForm::getModuleMax()
{
     TListItem* selected(ModuleEditor1->ModuleFrame1->ModuleList->Selected);
     if(selected==NULL)
        return NULL;

     ModuleDetail* detail = static_cast<ModuleDetail*>(selected->Data);
     return (detail->max);
}

//==============================================================================
// Function:    getModuleSpdDefault();
// Purpose:     returns true or false depending whether SpdDefault is checked or not
// Parameters:  None
// Returns:     bool
//==============================================================================
bool TModuleEditorForm::getModuleSpdDefault()
{
     TListItem* selected(ModuleEditor1->ModuleFrame1->ModuleList->Selected);
     if(selected==NULL)
        return false;

     ModuleDetail* detail = static_cast<ModuleDetail*>(selected->Data);
     if ( (detail->spd_default) == 1)
        return true;
     else
        return false;
}

//==============================================================================
// Function:    getModuleSpdeDefault();
// Purpose:     returns true or false depending whether SpdDefault is checked or not
// Parameters:  None
// Returns:     bool
//==============================================================================
bool TModuleEditorForm::getModuleSpdeDefault()
{
     TListItem* selected(ModuleEditor1->ModuleFrame1->ModuleList->Selected);
     if(selected==NULL)
        return false;

     ModuleDetail* detail = static_cast<ModuleDetail*>(selected->Data);
     if ( (detail->spde_default) == 1)
        return true;
     else
        return false;
}

//==============================================================================
// Function:    getModuleConnectivityDefault();
// Purpose:     returns true or false depending whether mdConnectivityDefault is
//              checked or not
// Parameters:  None
// Returns:     bool
//==============================================================================
bool TModuleEditorForm::getModuleConnectivityDefault()
{
     TListItem* selected(ModuleEditor1->ModuleFrame1->ModuleList->Selected);
     if(selected==NULL)
        return false;

     ModuleDetail* detail = static_cast<ModuleDetail*>(selected->Data);
     if ( (detail->con_default) == 1)
        return true;
     else
        return false;
}

//==============================================================================
// Function:    getModuleConnectivityModule();
// Purpose:     returns true or false depending whether mdConnectivityModule is
//              checked or not
// Parameters:  None
// Returns:     bool
//==============================================================================
bool TModuleEditorForm::getModuleConnectivityModule()
{
     TListItem* selected(ModuleEditor1->ModuleFrame1->ModuleList->Selected);
     if(selected==NULL)
        return false;

     ModuleDetail* detail = static_cast<ModuleDetail*>(selected->Data);
     if ( (detail->con_module) == 1)
        return true;
     else
        return false;
}

//==============================================================================
// Function:    getModuleiConvertDefault();
// Purpose:     returns true or false depending whether mdiConvertDefault is
//              checked or not
// Parameters:  None
// Returns:     bool
//==============================================================================
bool TModuleEditorForm::getModuleiConvertDefault()
{
     TListItem* selected(ModuleEditor1->ModuleFrame1->ModuleList->Selected);
     if(selected==NULL)
        return false;

     ModuleDetail* detail = static_cast<ModuleDetail*>(selected->Data);
     if ( (detail->iConvert_default) == 1)
        return true;
     else
        return false;
}

//==============================================================================
// Function:    getModuleiConvertModule();
// Purpose:     returns true or false depending whether mdiConvertModule is
//              checked or not
// Parameters:  None
// Returns:     bool
//==============================================================================
bool TModuleEditorForm::getModuleiConvertModule()
{
     TListItem* selected(ModuleEditor1->ModuleFrame1->ModuleList->Selected);
     if(selected==NULL)
        return false;

     ModuleDetail* detail = static_cast<ModuleDetail*>(selected->Data);
     if ( (detail->iConvert_module) == 1)
        return true;
     else
        return false;
}

//==============================================================================
// Function:
// Purpose:
// Parameters:
// Returns:     
//==============================================================================

