//------------------------------------------------------------------------------
// Filename:    CreatorAbout.h
// Note:        Copyright (c) 1993-2001 by Solimar Systems, Inc.
//              All Rights Reserved.
//------------------------------------------------------------------------------
#ifndef creatoraboutH
#define creatoraboutH
//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <vcl\ExtCtrls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\Classes.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Windows.hpp>
#include <vcl\System.hpp>

//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Class TCreatorAboutBox
//------------------------------------------------------------------------------
class TCreatorAboutBox : public TForm
{
__published:
    TLabel *Version;
    TLabel *Label;
    TLabel *Label1;
    TBitBtn *BitBtn1;
	TImage *Image1;
	
private:
   __fastcall TCreatorAboutBox(TComponent* Owner):TForm(Owner){}
public:
	__fastcall TCreatorAboutBox(int, TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern TCreatorAboutBox *CreatorAboutBox;
//----------------------------------------------------------------------------
#endif    
