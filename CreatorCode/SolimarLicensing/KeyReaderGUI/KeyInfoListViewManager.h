#ifndef __KEYINFOLISTVIEWMANAGER_H__
#define __KEYINFOLISTVIEWMANAGER_H__

#include "CommunicationLink.h"
#include "ModuleLicenseListViewManager.h"

namespace KeyViewManager
{
	using namespace System;
	using namespace System::IO;
	using namespace System::Data;
	using namespace System::Text;
	using namespace System::Windows::Forms;
	using namespace ModuleViewManager;
	using namespace System::ComponentModel;

	__gc class KeyInfoListViewManager
	{
	public:
		KeyInfoListViewManager(CommunicationLink* CommLink, ListView* KeyInfoListView);
		~KeyInfoListViewManager();
		bool PopulateView();
		bool Connect();
		bool Disconnect();

	private:
		bool FillRow(KeyInfoStructure TheKeyInfoStructure);
	
		//maps the product id returned from the lower layer app
		//to the corresponding product as described in keyspec.xml
		char* MapProductID(int* pProductID);

		//maps the product id returned from the lower layer app
		//to the corresponding product as described in keyspec.xml
		//static int* MapProductName(String* pProductName);
		//maps the status id returned from the lower layer app
		//to the corresponding license as described in status.txt
		char* MapLicenseID(int* pLicenseID, char* retval);

		//maps the key type id returned from the lower layer app
		//to the corresponding license as described in status.txt
		char*MapKeyTypeID(int* pKeyTypeID);

		String* UnicodeStrToString(String* unicode);

		//This is our communication Link
		CommunicationLink* OurCommLink;
		ListView* TheKeyListView;
		ListView* TheModListView;
		ModuleLicenseListViewManager* TheModViewManager;
		System::ComponentModel::Container* components;
	};
}

#endif //__KEYINFOLISTVIEWMANAGER_H__
