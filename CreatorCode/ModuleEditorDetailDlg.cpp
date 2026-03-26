//------------------------------------------------------------------------------
// Filename:    ModuleEditorDetailDlg.cpp
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "clookups.h"
#include "ModuleEditorDetailDlg.h"
#include "ModuleEditorDlg.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------

TModuleEditorDetailsDlg *ModuleEditorDetailsDlg;

//==============================================================================
// Function:    TModuleEditorDetailsDlg
// Purpose:     Constructor
// Parameters:  TComponent *Owner
// Returns:     None
// Note:
//==============================================================================
__fastcall TModuleEditorDetailsDlg::TModuleEditorDetailsDlg(TComponent* Owner)
        : TForm(Owner)
{

}
//==============================================================================
// Function:    UpdateButtonClick
// Purpose:
// Parameters:  TObject *Sender
// Returns:     None
// Note:
//==============================================================================
void __fastcall TModuleEditorDetailsDlg::UpdateButtonClick(TObject *Sender)
{
     // Convert the Caption text value to an integer value
     char *endptr;
     char cBuffer[256];

     // Initialize mod_id
     AnsiString aTempValue;
     aTempValue = ModIDNumberLabel->Caption;
     wsprintf(cBuffer, "%s", aTempValue);
     mod_id = static_cast<int>(strtol(cBuffer, &endptr, 10));

     aTempValue = ProductIDLabel->Caption;
     wsprintf(cBuffer, "%s", aTempValue);
     product_id = static_cast<int>(strtol(cBuffer, &endptr, 10));

     // Initialize the other member variables
     mod_name = ModNameEdit->Text;
     mod_engineer = ModEngineerCombo->Text;
     mod_partnumber = ModPartNumberEdit->Text;
     mod_version_added = static_cast<unsigned short>(strtol(ModVersionAddedEdit->Text.c_str(), &endptr, 16));
     mod_version_removed = static_cast<unsigned short>(strtol(ModVersionRemovedEdit->Text.c_str(), &endptr, 16));
     mod_type = ModTypeCombo->Text;
     mod_max = static_cast<unsigned short>(strtol(ModMaxEdit->Text.c_str(), &endptr, 10));
     mod_spd_def = ModSpdDefaultCheckBox->Checked;
     mod_connect_def = ModConnectivityDefaultCheckBox->Checked;
     mod_connect_mod = ModConnectivityModuleCheckBox->Checked;


     //
     // If the item to update is not in the database, then we need to insert the
     // new item instead of updating it.
     if ( !bExistingMember )
     {        
        //
        // If the updated item is not already in the database, then we need to
        // Add these items to the database.
        lookup->addItemToSModuleDetail(product_id,
                                  mod_id,
                                  mod_name, 
                                  mod_engineer,
                                  mod_partnumber,
                                  mod_version_added,
                                  mod_version_removed,
                                  mod_type,
                                  mod_max,
                                  mod_spd_def,
                                  mod_connect_def,
                                  mod_connect_mod,
                                  mod_iConvert_def,
                                  mod_iConvert_mod );
        
     }
     else
     {
        //
        // Otherwise...
        // If the module exists in the database already, then we need to update
        // the module and refresh the module list.
        lookup->updateMDEngineer(mod_id, mod_engineer);
        lookup->updateMDModuleName(mod_id, mod_name);
        lookup->updateMDPartNumber(mod_id, mod_partnumber);
        lookup->updateMDVersionAdded(mod_id, mod_version_added);
        lookup->updateMDVersionRemoved(mod_id, mod_version_removed);
        lookup->updateMDMax(mod_id, mod_max);
        lookup->updateMDSpdDefault(mod_id, mod_spd_def);
        lookup->updateMDConnectivityDefault(mod_id, mod_connect_def);
        lookup->updateMDConnectivityModule(mod_id, mod_connect_mod);
        lookup->updateMDAction(mod_id, mod_type);

        // Alex - Add update for iConvert default, and have to add the product id
        //      as a parameter.
     }


}
//==============================================================================
// Function:    ModConnectivityDefaultCheckBoxClick
// Purpose:
// Parameters:  TObject *Sender
// Returns:     None
// Note:
//==============================================================================
void __fastcall TModuleEditorDetailsDlg::ModConnectivityDefaultCheckBoxClick(
      TObject *Sender)
{
        if (ModConnectivityDefaultCheckBox->Checked == true)
                ModConnectivityModuleCheckBox->Checked = true;
}

//==============================================================================
// Function:    ModiConvertDefaultCheckBoxClick
// Purpose:     This function is necessary so that if the user checks that a
//              module is a iConvert default module....the function will
//              automatically check that it is a iConvert module.
// Parameters:  TObject *Sender
// Returns:     None
// Note:
//==============================================================================
void __fastcall TModuleEditorDetailsDlg::ModiConvertDefaultCheckBoxClick(
      TObject *Sender)
{
        if (ModiConvertDefaultCheckBox->Checked == true)
                ModiConvertModuleCheckBox->Checked = true;
}

