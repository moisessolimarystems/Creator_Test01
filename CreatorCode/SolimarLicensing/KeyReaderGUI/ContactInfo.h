#ifndef __CONTACTINFO_H__
#define __CONTACTINFO_H__

namespace ContactInfoForm
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Drawing;
	using namespace System::Windows::Forms;

	__gc class ContactInfo : public System::Windows::Forms::Form
	{
		public :
		//constructor
		ContactInfo();
		~ContactInfo();

		//Releases all of the resources that this object owns.
		void Dispose(Boolean disposing);

		

	private:
		//initializes the resources on that this object owns.
		void InitializeComponent();
		
		//Members
		System::ComponentModel::Container* components;
	};
}
#endif //__CONTACTINFO_H__