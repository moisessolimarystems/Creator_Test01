//---------------------------------------------------------------------------


#ifndef SolSearcherEnterpriseDetailsH
#define SolSearcherEnterpriseDetailsH
//---------------------------------------------------------------------------
#include "kyslevel.h"

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <DB.hpp>
//---------------------------------------------------------------------------
class TSolSearcherDetails : public TFrame
{
__published:	// IDE-managed Components
   TQuery *DetailSQL;
   TPopupMenu *PropertyPopup;
   TMenuItem *GeneratePasswordMenuItem;
   TListView *SolSearcherPropertyList;
   TMenuItem *SetValueMenuItem;

   void __fastcall GeneratePasswordMenuItemClick(TObject *Sender);
   void __fastcall SetValueMenuItemClick(TObject *Sender);
private:	// User declarations
   bool             m_bInitialized;
   int              m_mode;
   TDatabase       *m_pDatabase;
   SSProtectionKey *m_pKey;
   SKeyRecord      *m_pKeyRecord;

   void( __closure *OnDetailUpdate)(bool);
   void SetValue(TListItem *pItem);

public:		// User declarations
   __fastcall TSolSearcherDetails(TComponent* Owner);

   void Load(SKeyRecord* pKeyRecord);
   void Initialize(int mode, void( __closure *onModuleUpdate)(bool)=NULL);
   void GeneratePassword(int type);


};
//---------------------------------------------------------------------------
extern PACKAGE TSolSearcherDetails *SolSearcherDetails;
//---------------------------------------------------------------------------
#endif
