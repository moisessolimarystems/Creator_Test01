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

const int VERSION_MAJOR  = 2;
const int VERSION_MINOR = 26;
const int BUILD_NUMBER = 46;

//------------------------------------------------------------------------------
// Class TCreatorAboutBox
//------------------------------------------------------------------------------
class TCreatorAboutBox : public TForm
{
__published:
        TLabel *Version;
        TBitBtn *BitBtn1;
	TImage *Image1;
        TLabel *CopyRightLabel;

public:
	__fastcall TCreatorAboutBox(/*int,*/ TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern TCreatorAboutBox *CreatorAboutBox;
//----------------------------------------------------------------------------
#endif    
