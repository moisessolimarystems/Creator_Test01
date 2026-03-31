//---------------------------------------------------------------------
#include <vcl.h>
#include <Registry.hpp>
#pragma hdrstop

#include "ConnectionEditor.h"
//#include "SolimarLicensing\SolimarLicenseServer\_SolimarLicenseServer_i.c"
#include "SolimarLicensing\SolimarLicenseServer\_SolimarLicenseServer.h"
//#include "SolimarLicensing\common\ChallengeResponseHelper.h"

//---------------------------------------------------------------------
#pragma resource "*.dfm"
TConnectionEditorDlg *ConnectionEditorDlg;
//---------------------------------------------------------------------
__fastcall TConnectionEditorDlg::TConnectionEditorDlg(TComponent* AOwner)
	: TForm(AOwner)
{
     TRegistry *Reg = new TRegistry(KEY_READ);
     try
     {
         Reg->RootKey = HKEY_LOCAL_MACHINE;
         if (Reg->OpenKey("\\Software\\Solimar Systems\\Creator", true))
         {
             KeyServerNameEB->Text = Reg->ReadString("KeyServer");
             Reg->CloseKey();
         }
     }
     __finally
     {
         delete Reg;
     }
}
//---------------------------------------------------------------------
void __fastcall TConnectionEditorDlg::OKBtnClick(TObject *Sender)
{
     //validate server name
     if(IsValidServer(KeyServerNameEB->Text))
     {
         TRegistry *Reg = new TRegistry;
         try
         {
             Reg->RootKey = HKEY_LOCAL_MACHINE;
             if (Reg->OpenKey("\\Software\\Solimar Systems\\Creator", true))
             {
                Reg->WriteString("KeyServer", KeyServerNameEB->Text);
                Reg->CloseKey();
             }
         }
         __finally
         {
             delete Reg;
         }
     }
     else
     {
         Application->MessageBox("Invalid Key Server!", "Key Message", MB_OK|MB_ICONERROR );
         ModalResult = mrNone;
         KeyServerNameEB->SetFocus();
     }
}

//---------------------------------------------------------------------------

bool TConnectionEditorDlg::IsValidServer(AnsiString serverName)
{
     HRESULT hr;
     wchar_t wcharServer[128];
     COSERVERINFO	serverInfo	= {0, serverName.WideChar(wcharServer ,serverName.WideCharBufSize()), NULL, 0};
     MULTI_QI        multiQI         = {&IID_ISolimarLicenseSvr, NULL, NOERROR};

     hr = CoCreateInstanceEx(
          __uuidof(CSolimarLicenseSvr),
          NULL,
          CLSCTX_REMOTE_SERVER | CLSCTX_LOCAL_SERVER,
          &serverInfo,
          1,
          &multiQI);

     if (!SUCCEEDED(hr))
     {
          return false;
     }
     return true;
}

