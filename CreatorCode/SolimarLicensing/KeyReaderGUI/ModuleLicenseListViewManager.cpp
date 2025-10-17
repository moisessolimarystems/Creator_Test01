#include "stdafx.h"
#include "ModuleLicenseListViewManager.h"

using namespace ModuleViewManager;

#undef MessageBox
//constructor...initialize the member variables
ModuleLicenseListViewManager::ModuleLicenseListViewManager(CommunicationLink* CommLink, ListView* TheModViewManager)
							: TheModView(TheModViewManager)
{
	//initialize the member variables
	KeyNum = NULL;
	OurCommLink = CommLink;
	if(!OurCommLink)
		return;
}

ModuleLicenseListViewManager::~ModuleLicenseListViewManager()
{
	//disconnect from the solimar license server
	OurCommLink->UnInitializeModuleLicenseConnection();
	OurCommLink = NULL;
}

//returns whether the object is initialized
bool ModuleLicenseListViewManager::IsInitialized()
{
	if(OurCommLink)
		return OurCommLink->IsModInitialized();

	return false;
}

//fills in the module license list view
void ModuleLicenseListViewManager::PopulateView()
{
	ModuleLicensingStructure TheModuleLicStructure;
	_variant_t retval;
	if(!OurCommLink)
		return;

	//converts the string* into a variant
	System::Runtime::InteropServices::Marshal::
			GetNativeVariantForObject(KeyNum, &retval);

	//make sure the key is programmed
	if(!(OurCommLink->KeyIsProgrammed(&(retval.bstrVal))))
		return;

	int numModules = OurCommLink->GetNumModules();
	for(int i = 0; i < numModules; i++)
    {
		memset(&TheModuleLicStructure, 0, sizeof(TheModuleLicStructure));
	    OurCommLink->GetModuleLicensingStructureArray(TheModuleLicStructure, i);
		//don't show modules that are not licensed
		if(TheModuleLicStructure.TotalLicenses > 0)			
			FillRow(TheModuleLicStructure);
	}
}

//populate a row in the module license list view
void ModuleLicenseListViewManager::FillRow(ModuleLicensingStructure TheModuleLicStructure)
{
	//add the module name to the list view
	ListViewItem*  listViewItem1 = new ListViewItem();
	listViewItem1->Text = TheModuleLicStructure.ModuleName.bstrVal;

	char retval[10];

	if((long)TheModuleLicStructure.TotalLicenses >= (long)TheModuleLicStructure.ModuleUnlimited && 
	   ((long)TheModuleLicStructure.ModuleUnlimited > 0))
	{
		listViewItem1->SubItems->Add(S"Unlimited");
	}
	else
	{
		sprintf_s(retval, sizeof(retval), "%d", TheModuleLicStructure.TotalLicenses);
		listViewItem1->SubItems->Add(retval);		
	}

	//convert the licenses in use to a string and insert it as a subitem
	sprintf_s(retval, sizeof(retval), "%d", TheModuleLicStructure.LicensesInUse);
	listViewItem1->SubItems->Add(retval);

	ListViewItem* __mcTemp__2[] = new ListViewItem*[1];
	__mcTemp__2[0] = listViewItem1;
	
	//add the data row to the list view
	this->TheModView->Items->AddRange(__mcTemp__2);
}

void ModuleLicenseListViewManager::SetKeyID(Object* NewKeyNumber)
{
	//sets the key id and initializes the module license connection based on the 
	//new key number
	if(!OurCommLink)
		return;

	KeyNum = NewKeyNumber;
	OurCommLink->InitializeModuleLicenseConnection(NewKeyNumber);
}
