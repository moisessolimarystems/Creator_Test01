//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("LicenseWrapperTestPrj.res");
USEFORM("LicenseWrapperTest.cpp", Form1);
USEUNIT("..\manager_wrapper\LicensingWrapper.cpp");
USEUNIT("..\SolimarLicenseServer\KeySpec.cpp");
USEUNIT("..\common\CryptoHelper.cpp");
USEUNIT("..\common\ChallengeResponseHelper.cpp");
USEUNIT("..\common\ChallengeResponse.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        CoInitializeEx(0,COINIT_MULTITHREADED);
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TForm1), &Form1);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
