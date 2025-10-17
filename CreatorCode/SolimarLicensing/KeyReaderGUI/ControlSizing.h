#ifndef __CONTROLSIZING_H__
#define __CONTROLSIZING_H__

namespace SizeControl
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Drawing;
	using namespace System::Windows::Forms;

	//this class handles the sizing of controls upon the form load. 
	__gc class ControlSizing
	{
		public:

		//constructor
		ControlSizing(Form* TheForm);
		~ControlSizing();

		//initializes the size of the form
		void ControlSizing::InitializeFormSize();
		void ControlSizing::InitializeFormLocation();

	private:
		//Members
		System::Drawing::Rectangle WorkingRectangle;
		Form* MainForm;
		int FormWidth;
		int FormHeight;
	};
}
#endif //__CONTACTINFO_H__