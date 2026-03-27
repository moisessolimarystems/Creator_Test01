//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "LicenseWrapperTest.h"
#include "..\manager_wrapper\LicensingWrapper.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
                bool fun = true;
		LicensingWrapper license;
		fun = license.Connect(L"localhost");
		fun = license.Initialize(8, 2, 11, false, std::wstring(L""), false, LicensingWrapper::UI_ALL_DIALOGS);
		//fun = license.RegisterMessageCallback(0, LicenseCallback);
 		fun = license.ModuleLicenseObtain(0,1);
}
//---------------------------------------------------------------------------
