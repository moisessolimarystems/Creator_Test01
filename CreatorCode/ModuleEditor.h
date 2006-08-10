//---------------------------------------------------------------------------


#ifndef ModueEditorH
#define ModueEditorH
//---------------------------------------------------------------------------
/*
#include "clookups.h"
#include <Grids.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <OleCtrls.hpp>
#include <ComCtrls.hpp>
*/
#include "ModuleDetail.h"
#include <Classes.hpp>
#include <Controls.hpp>
#include <Forms.hpp>
#include <StdCtrls.hpp>

#include <Buttons.hpp>
#include <ExtCtrls.hpp>

//
// MODE_4 = Module Editor

//---------------------------------------------------------------------------
class TModuleEditor : public TFrame
{
__published:	// IDE-managed Components
        TModuleFrame *ModuleFrame1;
private:	// User declarations
        int productID;
public:		// User declarations
        __fastcall TModuleEditor(TComponent* Owner);

};
//---------------------------------------------------------------------------
extern PACKAGE TModuleEditor *ModuleEditor;
//---------------------------------------------------------------------------
#endif
