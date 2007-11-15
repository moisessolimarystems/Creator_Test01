#include "stdafx.h"
#include "ControlSizing.h"

#define WIDTH_SCALAR .63
#define HEIGHT_SCALAR .55
#define FORM_LOC_X_COORD 55
#define FORM_LOC_Y_COORD 55

using namespace SizeControl;

//Constructor. 
//Initializes the member variables and creates a table of appropriate size.
ControlSizing::ControlSizing(Form* TheForm)
				: MainForm(TheForm)
{
	Screen* PrimaryScreen = Screen::PrimaryScreen;
	WorkingRectangle = PrimaryScreen->WorkingArea;

	FormWidth = (int)WorkingRectangle.Width;
	FormHeight = (int)WorkingRectangle.Height;
}

//Initializes the resources
ControlSizing::~ControlSizing()
{
}

void ControlSizing::InitializeFormSize()
{
	// Set the size of the form to be 3/4 the size of working rectangle.
	MainForm->Size =  *(__nogc new Size((int)(FormWidth * WIDTH_SCALAR), 
									   (int)(FormHeight * HEIGHT_SCALAR)));
}

void ControlSizing::InitializeFormLocation()
{
	
	// Set the location of the form
	MainForm->Location = *(__nogc new System::Drawing::Point(FORM_LOC_X_COORD, FORM_LOC_Y_COORD));
}
