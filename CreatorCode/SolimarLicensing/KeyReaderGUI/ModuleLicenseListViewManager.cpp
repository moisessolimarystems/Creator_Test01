#include "stdafx.h"
#include "ModuleLicenseListViewManager.h"

using namespace ModuleViewManager;

//constructor...initialize the member variables
ModuleLicenseListViewManager::ModuleLicenseListViewManager(ListView* TheModViewManager)
							: TheModView(TheModViewManager)
{
	//initialize the member variables
	KeyNum = NULL;

	//new up a commlink object
	OurCommLink = new CommunicationLink();

	//connect to the solimar license server
	OurCommLink->Connect();
}

ModuleLicenseListViewManager::~ModuleLicenseListViewManager()
{
	//disconnect from the solimar license server
   //new
   OurCommLink->UnInitializeModuleLicenseConnection();
	OurCommLink->Disconnect();
	delete OurCommLink;
	OurCommLink = 0;
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

	//converts the string* into a variant
	System::Runtime::InteropServices::Marshal::
			GetNativeVariantForObject(KeyNum, &retval);

	//make sure the key is programmed
	if(!(OurCommLink->KeyIsProgrammed(&(retval.bstrVal))))
		return;

	for(int i = 0; i < OurCommLink->GetNumModules(); i++)
   {
		memset(&TheModuleLicStructure, 0, sizeof(TheModuleLicStructure));
	   OurCommLink->GetModuleLicensingStructureArray(TheModuleLicStructure, i);
	
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
	int product_id = OurCommLink->GetProductID();
   bool unlimited = false;
   int module_id = TheModuleLicStructure.ModuleID.intVal;

   switch(product_id)
   {
      case SPD_PRODUCT:
       	if((long)TheModuleLicStructure.TotalLicenses >= SPD_MAX_NUMBER_MODULES[module_id])
         {
            sprintf(retval, "%d", SPD_MAX_NUMBER_MODULES[module_id]);
            listViewItem1->SubItems->Add(retval);
            //listViewItem1->SubItems->Add(S"Unlimited");
            unlimited = true;
         } 
	      break;

      case ICONVERT_PRODUCT:
         if((long)TheModuleLicStructure.TotalLicenses >= ICONVERT_MAX_NUMBER_MODULES[module_id])
         {
            sprintf(retval, "%d", ICONVERT_MAX_NUMBER_MODULES[module_id]);
            listViewItem1->SubItems->Add(retval);
            //listViewItem1->SubItems->Add(S"Unlimited");
            unlimited = true;
         }
         break;

      case SOLSCRIPT_PRODUCT:
         if((long)TheModuleLicStructure.TotalLicenses >= SOLSCRIPT_MAX_NUMBER_MODULES[module_id])
         {
            sprintf(retval, "%d", SOLSCRIPT_MAX_NUMBER_MODULES[module_id]);
            listViewItem1->SubItems->Add(retval);
            //listViewItem1->SubItems->Add(S"Unlimited");
            unlimited = true;
         }
         break;

      case SDX_DESIGNER_PRODUCT:
         if((long)TheModuleLicStructure.TotalLicenses >= SDX_DESIGNER_MAX_NUMBER_MODULES[module_id])
         {
            sprintf(retval, "%d", SDX_DESIGNER_MAX_NUMBER_MODULES[module_id]);
            listViewItem1->SubItems->Add(retval);
            //listViewItem1->SubItems->Add(S"Unlimited");
            unlimited = true;
         }
         break;
   }

   if(!unlimited)
	{
		sprintf(retval, "%d", TheModuleLicStructure.TotalLicenses);
		listViewItem1->SubItems->Add(retval);
	}

	//convert the licenses in use to a string and insert it as a subitem
	sprintf(retval, "%d", TheModuleLicStructure.LicensesInUse);
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
	KeyNum = NewKeyNumber;
	OurCommLink->InitializeModuleLicenseConnection(NewKeyNumber);
}
