//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Kyslevel.h"
#include "ModuleEditor.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ModuleDetail"
#pragma resource "*.dfm"
TModuleEditor *ModuleEditor;
//---------------------------------------------------------------------------
__fastcall TModuleEditor::TModuleEditor(TComponent* Owner)
        : TFrame(Owner)
{
        //
        // Module Editor is derived from Module Detail, so I can use those
        // functions.
        // Need a module detail for the Module Editor that lists all modules
        // regardless of the product ID number passed in....second parameter.
        ModuleFrame1->initialize(MODE_4, 2);
        ModuleFrame1->load(NULL, true);
}
//---------------------------------------------------------------------------

