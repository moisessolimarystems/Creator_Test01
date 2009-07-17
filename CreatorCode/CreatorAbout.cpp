//------------------------------------------------------------------------------
// Filename:    CreatorAbout.cpp
// Note:        Copyright (c) 1993-2001 by Solimar Systems, Inc.
//              All Rights Reserved.
//------------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "creatorabout.h"

#pragma resource "*.dfm"
TCreatorAboutBox *CreatorAboutBox;

// PsUtilAboutBox() - Constructor
//---------------------------------------------------------------------
__fastcall TCreatorAboutBox::TCreatorAboutBox(/*int Ver,*/ TComponent* AOwner )
	: TForm(AOwner)
{
    AnsiString dateString;
    char verString[256];
    wsprintf(verString, "Version %d.%d.%d", VERSION_MAJOR, VERSION_MINOR, BUILD_NUMBER);
    Version->Caption = verString;
    dateString = DateToStr(Date());
    wsprintf(verString, "Copyright 1995-%s Solimar Systems, Inc.", dateString.SubString(dateString.Length() - 3, 4));
    CopyRightLabel->Caption = verString;

}

