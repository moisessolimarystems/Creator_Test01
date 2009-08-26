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


const int TDlgVersion::convertProductToIndex[] = { 0/*SPD_NT*/, 2/*XImage 3.1/95*/, 1/*XImage NT*/, 0, 0, 3/*Indexer*/, 4/*ConnectivityServer*/, 5/*iConvert*/, 6/*SS Enterprise*/, 7/*SOL Script*/, 8/*SDX Designer*/, 9/*Quantum Server*/, 10/*Rubika*/, 11 /*SOLFUSION*/, 12/*SPDE*/, 13/*SOLITRACK*/};
const int TDlgVersion::convertIndexToProduct[] = { SPD_PRODUCT, XIMAGENT_PRODUCT, XIMAGE_PRODUCT, INDEX_PLUGIN, CONNECT_PRODUCT, ICONVERT_PRODUCT, SOLSEARCHER_ENTERPRISE_PRODUCT, SOLSCRIPT_PRODUCT, SDX_DESIGNER_PRODUCT, QUANTUM_PRODUCT, RUBIKA_PRODUCT, SOLFUSION_PRODUCT, SPDE_PRODUCT, SOLITRACK_PRODUCT};

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
   int majorVersion, minorVersion;

   //set initial_version
   init_version = current_version;
   //enable or disable users ability to convert a dos key to and nt key
   DOS_to_NT = DOStoNT;
   //initialize product and version display members
   ProductComboBox->ItemIndex = convertProductToIndex[product_id];
   //initialize version editbox with release version for current product
   version = lookup->getCurrentVersion(product_id, false);
   majorVersion = ((int)version) >> 12;
   //extract the last three bytes
   minorVersion = (int)((((version & 0x0F00) >> 8) * 10) + (((version & 0x00F0) >> 4) * 1));
   VersionEdit->Text = AnsiString::Format("%d.%d", OPENARRAY(TVarRec,(majorVersion,minorVersion)));
   //VersionEdit->Text = Format("%4X", OPENARRAY(TVarRec,(version)));
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
   AnsiString dbVersion;
   //update version for product selected and exit dialog
   if(ALL_PRODUCTS)
   {
      ApplyBtnClick(this);
   }
   //validate version user has entered
   else
   {
      char *endptr;
      if(!IsValidVersionFormat(VersionEdit->Text))
      {
          ModalResult = mrNone;
          VersionEdit->SelectAll();
          VersionEdit->SetFocus();
          Application->MessageBox("Please enter a valid version.", "Invalid Entry", MB_OK );
          return;
      }
      //convert input into hex format
      dbVersion = DBFormatVersion(VersionEdit->Text);
      version = static_cast<unsigned short>(strtol(dbVersion.c_str(), &endptr, 16));

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
         VersionEdit->SelectAll();
         VersionEdit->SetFocus();
         Application->MessageBox("New version must be greater than the current version.", "Invalid Entry", MB_OK );
      }

   }
}
//==============================================================================
// ProductComboBoxChange()
//==============================================================================
void __fastcall TDlgVersion::ProductComboBoxChange(TObject *Sender)
{
   int majorVersion, minorVersion;
   version = lookup->getCurrentVersion(convertIndexToProduct[ProductComboBox->ItemIndex], false);
   majorVersion = ((int)version) >> 12;
   //extract the last three bytes
   minorVersion = (int)((((version & 0x0F00) >> 8) * 10) + (((version & 0x00F0) >> 4) * 1));
   VersionEdit->Text = AnsiString::Format("%d.%d", OPENARRAY(TVarRec,(majorVersion,minorVersion)));
}

//==============================================================================
// ApplyBtnClick()
//
//==============================================================================
void __fastcall TDlgVersion::ApplyBtnClick(TObject *Sender)
{
   char *endptr;
   AnsiString dbVersion;
   if(!IsValidVersionFormat(VersionEdit->Text))
   {
       ModalResult = mrNone;
       VersionEdit->SelectAll();
       VersionEdit->SetFocus();
       Application->MessageBox("Please enter a valid version.", "Invalid Entry", MB_OK );
       return;
   }
   dbVersion = DBFormatVersion(VersionEdit->Text);
   //need to format user input to DB format x.x to x.xxx
   unsigned short tmp_version(static_cast<unsigned short>(strtol(dbVersion.c_str(), &endptr, 16)));
   lookup->updateVersion(convertIndexToProduct[ProductComboBox->ItemIndex], tmp_version);
}

//Expect strVersion to be valid X.X+ , minorVersion max of 2
AnsiString TDlgVersion::DBFormatVersion(AnsiString strVersion)
{
    AnsiString majorVersion, minorVersion;
    int pos;
    pos = strVersion.AnsiPos(".");
    majorVersion = AnsiString(strVersion.SubString(0,pos-1).ToInt());
    minorVersion = AnsiString(strVersion.SubString(pos+1, strVersion.Length()-(majorVersion.Length())).ToInt());

    if(minorVersion.Length() == 1)
        minorVersion = "0" + minorVersion;
    minorVersion= minorVersion + "0";
    return majorVersion + minorVersion;
}

bool TDlgVersion::IsValidVersionFormat(AnsiString versionString)
{
    AnsiString majorVersion, minorVersion, tempString;
    int count = 0, pos = 0, number = 0;
    //find position of period
    tempString = versionString;
    do
    {
        pos = tempString.AnsiPos(".");
        tempString = tempString.SubString(pos+1, tempString.Length()-pos);
        if(pos != 0)
            count++;
    }
    while(pos != 0);
    if(count != 1)
        return false;
    else
    {   //only one period ...split into major, minor
        pos = versionString.AnsiPos(".");
        majorVersion = versionString.SubString(0,pos-1);
        if((StrToIntDef(majorVersion, -1) < 0) || (majorVersion.ToInt() < 1))
            return false;
        minorVersion = versionString.SubString(pos+1, versionString.Length()-(majorVersion.Length()));
        if((minorVersion.Length() > 2) || (StrToIntDef(minorVersion, -1) < 0))
            return false;
    }
    return true;
}




