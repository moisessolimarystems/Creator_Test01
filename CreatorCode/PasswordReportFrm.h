//----------------------------------------------------------------------------
#ifndef PasswordReportFrmH
#define PasswordReportFrmH
//----------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\QuickRpt.hpp>
#include <vcl\QRCtrls.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <ExtCtrls.hpp>
#include <Qrctrls.hpp>
#include <DB.hpp>

#define PASSWORD_START 0
#define PASSWORD_LENGTH 12
//----------------------------------------------------------------------------
class TPasswordReport : public TQuickRep
{
__published:
   TQRBand *ColumnHeaderBand1;
   TQRBand *DetailBand1;
   TQRBand *TitleBand1;
   TQRLabel *StaticLabel1;
   TQRLabel *StaticLabel2;
   TQRSysData *QRSysData1;
   TQRDBText *QRDBText1;
   TQuery *ReportQuery;
   TQRLabel *TitleLabel;
   TQRRichText *QRRichText1;
   TQRLabel *StaticReportLabel;
   TQRLabel *StaticKeyLabel;
        TQRExpr *QRExpr1;
        TQRDBText *QRDBText2;

private:
public:
   __fastcall TPasswordReport::TPasswordReport(TComponent* Owner, AnsiString);
};
//----------------------------------------------------------------------------
extern TPasswordReport *PasswordReport;
//----------------------------------------------------------------------------
#endif
