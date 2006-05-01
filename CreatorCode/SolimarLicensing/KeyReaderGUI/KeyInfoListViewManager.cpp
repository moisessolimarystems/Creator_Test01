#include "stdafx.h"
#include "KeyInfoListViewManager.h"


//Used to Map the Product IDs as specified in KeySpec.xml
const int SPDProductID = 0;
const int XImageProductID = 1;
const int XImageNTProductID = 2;
const int PCLDriverProductID = 3;
const int IndexPlugInProductID = 4;
const int ConnectivityServerProductID = 6;
const int IConvertProductID = 7;
const int SolSearcherEnterpriseProductID = 8;
const int SolScriptProductID = 9;
const int SDXDesignerProductID = 10;
const int QuantumServerProductID = 11;
const int RubikaProductID = 12;
const int SPDEProductID = 14;

//Used to map the license as specified in KeySpec.xml
const int InitialTrialID = 0;
const int ExtendedTrialID = 1;
const int Base = 2;
const int UninitializedTrialID = 3;
const int ExtendedTrialID2 = 4;
const int ExtendedTrialID3 = 5;
const int ExtendedTrialID4 = 6;
const int ExtendedTrialID5 = 7;
const int Unused = 10;
const int Deactivated = 11;

using namespace KeyViewManager;

KeyInfoListViewManager::KeyInfoListViewManager(ListView* KeyInfoListView)
			:TheKeyListView(KeyInfoListView)
{
	//new up a commlink object
	OurCommLink = new CommunicationLink();

	//connect to the solimar license server
	OurCommLink->Connect();

	//initializes the commlink object to be KeyInfo connection 
	OurCommLink->InitializeKeyInfoConnection();
}

KeyInfoListViewManager::~KeyInfoListViewManager()
{
	OurCommLink->UnInitializeKeyInfoConnection();
	OurCommLink->Disconnect();
	delete OurCommLink;
	OurCommLink = 0;

	if(TheModViewManager)
	{
		delete TheModViewManager;
		TheModViewManager = NULL;
	}

}

bool KeyInfoListViewManager::PopulateView()
{
   bool FillRowSuccess = true;
	KeyInfoStructure TheKeyInfoStructure;
	int numkeys = OurCommLink->GetNumKeys();

	//creates a new row for each key number in the key
	for(int i = 0; i < numkeys; i++)
	{
		memset(&TheKeyInfoStructure, 0, sizeof(KeyInfoStructure));
		OurCommLink->GetKeyInfoStructure(TheKeyInfoStructure, i);

		char* retval;
		int ProductID = ((int)(TheKeyInfoStructure.ProductID));
		retval = MapProductID(&ProductID);
		TheKeyInfoStructure.ProductID.SetString(retval);

		char ProductVersion[10];

		//extract the first byte
		//12 = 3 Bytes * sizeof(BYTE) = 3*4
		int MajorVersion = ((int)TheKeyInfoStructure.ProductVersion) >> 12;

		//extract the last three bytes
		int MinorVersion = ((int)TheKeyInfoStructure.ProductVersion) & 0x0fff;
		//int MinorVersion = (int)((((((int)TheKeyInfoStructure.ProductVersion) & 0x0F00) >> 8) * 10) + (((((int)TheKeyInfoStructure.ProductVersion) & 0x00F0) >> 4) * 1));

		sprintf(ProductVersion, "%x.%x", MajorVersion, MinorVersion);
		TheKeyInfoStructure.ProductVersion.SetString(ProductVersion);
		
		int LicenseID = ((int)(TheKeyInfoStructure.License));
		retval = MapLicenseID(&LicenseID);
		TheKeyInfoStructure.License.SetString(retval);

		//fills the row with the info stored in the structure
		FillRowSuccess = FillRow(TheKeyInfoStructure);
	}
	return FillRowSuccess;
}

//maps the status id returned from the lower layer app
//to the corresponding license as described in status.txt
char* KeyInfoListViewManager::MapLicenseID(int* pLicenseID)
{
		char* retval;

		//map the Product ID according to KeySpec.xml
		switch (*pLicenseID)
		{
			case UninitializedTrialID :
				return retval = "Uninitialized Trial";

			case InitialTrialID :
				return retval = "Initial Trial";
				
			case ExtendedTrialID :
				return retval = "Extended Trial";

			case ExtendedTrialID2 :
				return retval = "Extended Trial2";
				
			case ExtendedTrialID3 :
				return retval = "Extended Trial3";
				
			case ExtendedTrialID4 :
				return retval = "Extended Trial4";
				
			case ExtendedTrialID5 :
				return retval = "Extended Trial5";
								
			case Base :
				return retval = "Base";
				
			case Unused :
				return retval = "Unused";
				
			case Deactivated : 
				return retval = "Deactivated";
			default :
				return retval = "Unknown License";
		}
}

//maps the product id returned from the lower layer app
//to the corresponding product as described in keyspec.xml
char* KeyInfoListViewManager::MapProductID(int* pProductID)
{
		char* retval;

		//map the Product ID according to KeySpec.xml
		switch (*pProductID)
		{
			case SPDProductID :
				return retval = "SP/D Platinum";

			case ConnectivityServerProductID :
				return retval = "Connectivity Server";
				
			case QuantumServerProductID :
				return retval = "Quantum Server";

			case XImageProductID:
				return retval = "XImage";
				
			case XImageNTProductID :
				return retval = "XImage NT";
				
			case PCLDriverProductID :
				return retval = "PCL Driver";
				
			case IndexPlugInProductID :
				return retval = "IndexPlugInProductID";
								
			case IConvertProductID :
				return retval = "IConvert";
				
			case SolSearcherEnterpriseProductID :
				return retval = "SolSearcher Enterprise";
				
			case SolScriptProductID : 
				return retval = "SolScript";
				
			case SDXDesignerProductID :
				return retval = "SDX Designer";

			case RubikaProductID :
				return retval = "Rubika";

			case SPDEProductID : 
				return retval = "SP/D Enterprise";

			default :
				return retval = "Unknown License";
		}
}

//Fills a row with the info in the key info structure
bool KeyInfoListViewManager::FillRow(KeyInfoStructure TheKeyInfoStructure)
{
	ListViewItem*  listViewItem1 = new ListViewItem();
	listViewItem1->Text = TheKeyInfoStructure.KeyNumber.bstrVal;
	
	listViewItem1->SubItems->Add(TheKeyInfoStructure.ProductID.bstrVal);
	listViewItem1->SubItems->Add(TheKeyInfoStructure.ProductVersion.bstrVal);
	listViewItem1->SubItems->Add(TheKeyInfoStructure.License.bstrVal);

	if(TheKeyInfoStructure.Active.intVal == 0)
		listViewItem1->SubItems->Add(S"0");

	else
		listViewItem1->SubItems->Add(S"1");

	char retval[10];

   //Convert the hours left into Days Left as requested by Tech Support
   TheKeyInfoStructure.HoursLeft /= 24;
	sprintf(retval, "%d", TheKeyInfoStructure.HoursLeft);
	listViewItem1->SubItems->Add(retval);

	//convert the COM Variant into an Object
	Object* ConvertedStr = System::Runtime::InteropServices::Marshal
					::GetObjectForNativeVariant(&TheKeyInfoStructure.ExpirationDate);	

	if(TheKeyInfoStructure.ExpirationDate)
	{
		listViewItem1->SubItems->Add(ConvertedStr->ToString());
		ListViewItem* __mcTemp__2[] = new ListViewItem*[1];
    	__mcTemp__2[0] = listViewItem1;
		this->TheKeyListView->Items->AddRange(__mcTemp__2);
		return true;
	}
	else
	{
		return false;
	}
}
