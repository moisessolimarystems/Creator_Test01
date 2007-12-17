#ifndef __PASSWORDFORM_H__
#define __PASSWORDFORM_H__

#include "CommunicationLink.h"
#include "PasswordValidation.h"

namespace PWDForm
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Windows::Forms;
	using namespace PWDValidation;

	__gc class PasswordForm : public System::Windows::Forms::Form
	{
	public :
		//constructor
		PasswordForm(PasswordValidation* ThePasswordValidater);
		~PasswordForm();

		//Releases all of the resources that this object owns.
		void Dispose(Boolean disposing);

		

	private:
		System::Void PWDForm::PasswordForm::button1_Click(System::Object* sender, System::EventArgs* e);
		
		//initializes the resources on that this object owns.
		void InitializeComponent();
		
		//Members
		System::ComponentModel::Container* components;
		PasswordValidation* PasswordValidater;
	};

}
#endif