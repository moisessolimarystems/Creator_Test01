#ifndef __PASSWORDVALIDATION_H__
#define __PASSWORDVALIDATION_H__

#include "CommunicationLink.h"

namespace PWDValidation
{
	using namespace System::IO;
	using namespace System::Windows::Forms;
	using namespace System::Diagnostics;

	__gc class PasswordValidation
	{
	public:
		PasswordValidation(CommunicationLink*);
		~PasswordValidation();
		HRESULT ProcessPasswordFile(BSTR TheFileName);
		HRESULT CheckPassword(String* InputString);

	private:
		CommunicationLink* TheCommLink;
	};
}

#endif
