//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//#include "KeyForm.h"
#include <stdio.h>     //sprintf
#include "kyslevel.h"  //SKeyRecord
#include "ExtensionDlg.h"
//--------------------------------------------------------------------- 

#pragma resource "*.dfm"

TDlgExtension *DlgExtension;
//==============================================================================
//
//==============================================================================
__fastcall TDlgExtension::TDlgExtension(TComponent* AOwner, SKeyRecord* keyrec)
	: TForm(AOwner)
{
   //
   key_record = keyrec;

   switch (key_record->pkey->status)
   {
      case INITIAL_TRIAL:
         //extNum = 0;
         Caption = "1st Extension";
         //new_status = EXTENDED_TRIAL;
         break;
      case EXTENDED_TRIAL:
         //extNum = 1;
         Caption = "2nd Extension";
         //new_status = EXTENDED_TRIAL2;
         break;
      case EXTENDED_TRIAL2:
         //extNum = 2;
         Caption = "3rd Extension";
         //new_status = EXTENDED_TRIAL3;
         break;
      case EXTENDED_TRIAL3:
         //extNum = 3;
         Caption = "4th Extension";
         //new_status = EXTENDED_TRIAL4;
         break;
      case EXTENDED_TRIAL4:
      	 Caption = "Final Extension";
         //extNum = 4;
         //new_status = EXTENDED_TRIAL5;
         break;
   }

}

//==============================================================================
// OkBtnClick(TObject *Sender)
//==============================================================================
void __fastcall TDlgExtension::OkBtnClick(TObject *Sender)
{
   ///char message[80];
   unsigned char ext_days(static_cast<unsigned char>(Days->Text.ToInt()));
   //int days = Days->Text.ToInt();

   // make sure data is in range
   if(((key_record->pkey->productId == 0 && key_record->pkey->productVersion > 0x400F ) ||
       (key_record->pkey->productId == 1 && key_record->pkey->productVersion > 0x2190 ) ||
       (key_record->pkey->productId == 2 && key_record->pkey->productVersion > 0x1020 ))&&
       (ext_days > 90 ) ){
   	Application->MessageBox("Invalid entry, number must \n\rrange from 1 to 90.", "Warning", MB_OK|MB_ICONWARNING );
      Days->Text = 90;
   	Days->SetFocus();
      return;
   }
   else if(((key_record->pkey->productId == 0 && key_record->pkey->productVersion <= 0x400F ) ||
       (key_record->pkey->productId == 1 && key_record->pkey->productVersion <= 0x2190 ) ||
       (key_record->pkey->productId == 2 && key_record->pkey->productVersion <= 0x1020 ))&&
       (ext_days > 90 ) ){
   	Application->MessageBox("Invalid entry, number must \n\rrange from 1 to 90.", "Warning", MB_OK|MB_ICONWARNING );
       Days->Text = 90;
   	Days->SetFocus();
      return;
   }
   /*password = keyPtr->pkey->getExtensionPassword( (uchar)(Days->Text.ToInt()),
         								keyPtr->pkey->productId,
                                 keyPtr->pkey->productVersion, extNum);
   if(password){
   	sprintf(pswdString, "%lX", password  );
   	if( password ){// if a password was generated
   		sprintf( message, "Are you sure you would like to generate\nan extension password?  It will extend the\nlicense for %d days.", Days->Text.ToInt() );
   		if( IDYES == Application->MessageBox(message, "Key Message", MB_YESNO|MB_ICONINFORMATION) ){
         	ModalResult = IDYES;
            return;
   		}
   	}
   } */
   ModalResult = IDYES;
}

//---------------------------------------------------------------------------
