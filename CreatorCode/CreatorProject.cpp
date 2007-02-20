//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USEFORM("CustomerForm.cpp", CustForm);
USEFORM("CreatorAbout.cpp", CreatorAboutBox);
USEFORM("CustQueryDlg.cpp", QueryDialog);
USEFORM("CustUpdateDlg.cpp", UpdateDlg);
USEFORM("Keyform.cpp", FCustomerKeys);
USEFORM("VersionDlg.cpp", DlgVersion);
USEFORM("ExtensionDlg.cpp", DlgExtension);
USEFORM("setunitsdlg.cpp", UnitsDlg);
USEFORM("KeyWizard.cpp", KeyWizardFrm);
USEFORM("ModuleDetail.cpp", ModuleFrame); /* TFrame: File Type */
USEFORM("PasswordReportFrm.cpp", PasswordReport); /* TQuickRep: File Type */
USEFORM("ModuleEditor.cpp", ModuleEditor); /* TFrame: File Type */
USEFORM("ModuleEditorDlg.cpp", ModuleEditorForm);
USEFORM("ModuleEditorDetailDlg.cpp", ModuleEditorDetailsDlg);
USEFORM("PPMDlg.cpp", DlgPPM);
USEFORM("DevelopmentPasswordDlg.cpp", DlgDevelopmentPassword);
USEFORM("keySearchDlg.cpp", ksdSearchDlg);
USEFORM("keyDateSearchDlg.cpp", dsSearchDlg);
USEFORM("SolSearcherEnterpriseDetails.cpp", SolSearcherDetails); /* TFrame: File Type */
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
      try
      {
         Application->Initialize();
         Application->Title = "Creator";
         Application->CreateForm(__classid(TCustForm), &CustForm);
         Application->CreateForm(__classid(TModuleEditorForm), &ModuleEditorForm);
         Application->CreateForm(__classid(TModuleEditorDetailsDlg), &ModuleEditorDetailsDlg);
         Application->CreateForm(__classid(TDlgPPM), &DlgPPM);
         Application->CreateForm(__classid(TDlgDevelopmentPassword), &DlgDevelopmentPassword);
         Application->CreateForm(__classid(TksdSearchDlg), &ksdSearchDlg);
         Application->CreateForm(__classid(TdsSearchDlg), &dsSearchDlg);
         Application->Run();
      }
      catch (Exception &exception)
      {
         Application->ShowException(&exception);
      }
      return 0;
}
//---------------------------------------------------------------------------
