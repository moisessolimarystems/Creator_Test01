//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "CustomerForm.h"
#include "VersionDlg.h"
//---------------------------------------------------------------------
#ifdef __BORLAND_CPP__
#pragma resource "..\\creator\\*.dfm"
#else
#pragma resource "*.dfm"
#endif
TDlgVersion *DlgVersion;


const int TDlgVersion::convertProductToIndex[] = { 0/*SPD_NT*/, 2/*XImage 3.1/95*/, 1/*XImage NT*/, 0, 0, 3/*Indexer*/, 4/*ConnectivityServer*/, 5/*iConvert*/, 6/*SS Enterprise*/, 7/*SOL Script*/, 8/*SDX Designer*/, 9/*Quantum Server*/, 10/*Rubika*/, 0, 11/*SPDE*/};
const int TDlgVersion::convertIndexToProduct[] = { SPD_PRODUCT, XIMAGENT_PRODUCT, XIMAGE_PRODUCT, INDEX_PLUGIN, CONNECT_PRODUCT, ICONVERT_PRODUCT, SOLSEARCHER_ENTERPRISE_PRODUCT, SOLSCRIPT_PRODUCT, SDX_DESIGNER_PRODUCT, QUANTUM_PRODUCT, RUBIKA_PRODUCT, SPDE_PRODUCT};

//==============================================================================
// TDlgVersion() - Constructor
//
//==============================================================================
//__fastcall TDlgVersion::TDlgVersion(TComponent* AOwner, unsigned short product_id, unsigned short current_version, bool ALLPRODUCTS)
__fastcall TDlgVersion::TDlgVersion(TComponent* AOwner)
   : TForm(AOwner)
{
   DOS_to_NT = false;
}

//==============================================================================
// setDialog()
//==============================================================================
void TDlgVersion::setDialog(unsigned short product_id, unsigned short current_version, bool DOStoNT)
{
   //set initial_version
   init_version = current_version;
   //enable or disable users ability to convert a dos key to and nt key
   DOS_to_NT = DOStoNT;
   //initialize product and version display members
   ProductComboBox->ItemIndex = convertProductToIndex[product_id];

   //initialize version editbox with release version for current product
   version = lookup->getCurrentVersion(product_id, false);
   VersionEdit->Text = Format("%4X", OPENARRAY(TVarRec,(version)));

   //if no product or version information is passed in, dialog is being used to
   //update a product version
   if(product_id==0 &&current_version==0)
   {
      ALL_PRODUCTS=true;
      ApplyBtn->Enabled=true;
      ProductComboBox->Enabled=true;

      Caption = "Product Version Utility";
   }
   //update the version of a specific key
   else
   {
      ALL_PRODUCTS=false;
      ApplyBtn->Enabled=false;
      ProductComboBox->Enabled=false;

      Caption = "Update Version";
   }
}

//==============================================================================
//
//==============================================================================
void __fastcall TDlgVersion::OKBtnClick(TObject *Sender)
{
   //update version for product selected and exit dialog
   if(ALL_PRODUCTS)
   {
      ApplyBtnClick(this);
   }
   //validate version user has entered
   else
   {
      char *endptr;
      version = static_cast<unsigned short>(strtol(VersionEdit->Text.c_str(), &endptr, 16));

      //if user does not have permission to convert a dos key to an nt key, verify version
      if( DOS_to_NT == false )
      {
   	   if( init_version < 0x4000 && version >= 0x4000 )
         {
			   ModalResult = mrNone;
      	   Application->MessageBox("Do not have user right to convert a SPD/Dos key into a SPD/NT key.", "User Permissions", MB_OK );
            return;
         }
      }

      if( init_version >= version )
      {
   	   ModalResult = mrNone;
         Application->MessageBox("New version must be greater than the current version.", "Invalid Entry", MB_OK );
      }

   }
}
//==============================================================================
// ProductComboBoxChange()
//==============================================================================
void __fastcall TDlgVersion::ProductComboBoxChange(TObject *Sender)
{
   version = lookup->getCurrentVersion(convertIndexToProduct[ProductComboBox->ItemIndex], false);
   VersionEdit->Text = Format("%4X", OPENARRAY(TVarRec,(version)));
}

//==============================================================================
// ApplyBtnClick()
//
//==============================================================================
void __fastcall TDlgVersion::ApplyBtnClick(TObject *Sender)
{
   char *endptr;
   unsigned short tmp_version(static_cast<unsigned short>(strtol(VersionEdit->Text.c_str(), &endptr, 16)));
   lookup->updateVersion(convertIndexToProduct[ProductComboBox->ItemIndex], tmp_version);
}




