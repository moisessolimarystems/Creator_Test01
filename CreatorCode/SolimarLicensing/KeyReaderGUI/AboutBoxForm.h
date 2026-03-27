#ifndef __ABOUTBOXFORM_H__
#define __ABOUTBOXFORM_H__

namespace AboutBox
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Drawing;
	using namespace System::Windows::Forms;

	__gc class AboutBoxForm : public System::Windows::Forms::Form
	{
		public :
		//constructor
		AboutBoxForm();
		~AboutBoxForm();

		//Releases all of the resources that this object owns.
		void Dispose(Boolean disposing);

		

	private:
		//initializes the resources on that this object owns.
		void InitializeComponent();
		
		//Members
		System::ComponentModel::Container* components;
	};
}
#endif //__ABOUTBOXFORM_H__