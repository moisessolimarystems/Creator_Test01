#include "stdafx.h"
#include "KeyInfoListViewManager.h"

using namespace KeyViewManager;

#undef MessageBox
KeyInfoListViewManager::KeyInfoListViewManager(CommunicationLink* CommLink, ListView* KeyInfoListView)
			:TheKeyListView(KeyInfoListView)
{
	//new up a commlink object
	OurCommLink = CommLink;	
	Connect();
}

KeyInfoListViewManager::~KeyInfoListViewManager()
{
	Disconnect();
	OurCommLink = NULL;

	if(TheModViewManager)
	{
		delete TheModViewManager;
		TheModViewManager = NULL;
	}
}

bool KeyInfoListViewManager::Connect()
{	
	if(OurCommLink)
	{
		//initializes the commlink object to be KeyInfo connection 
		if(SUCCEEDED(OurCommLink->InitializeKeyInfoConnection()))
			return true;
	}
	return false;
}

bool KeyInfoListViewManager::Disconnect()
{	
	if(OurCommLink)
	{		
		OurCommLink->UnInitializeKeyInfoConnection();	
		return true;			
	}
	return false;
}

bool KeyInfoListViewManager::PopulateView()
{
    bool FillRowSuccess = true;
	KeyInfoStructure TheKeyInfoStructure;
	if(!OurCommLink)
		return false;

	int	numKeys = OurCommLink->GetNumKeys();
	//creates a new row for each key number in the key
	for(int i = 0; i < numKeys; i++)
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
		int MajorVersion = TheKeyInfoStructure.ProductVersion.lVal >> 12;

		//extract the last three bytes but drop the last unused byte
		int MinorVersion = (TheKeyInfoStructure.ProductVersion.lVal & 0x0fff) >> 4;

		//%x hexadecimal integer format
		sprintf_s(ProductVersion, sizeof(ProductVersion)/sizeof(char), "%x.%02x", MajorVersion, MinorVersion);
		TheKeyInfoStructure.ProductVersion.SetString(ProductVersion);
		
		int LicenseID = ((int)(TheKeyInfoStructure.License));
		char val[25]; //pass in char[] to get return val	
		retval = MapLicenseID(&LicenseID, val);
		TheKeyInfoStructure.License.SetString(retval);

		int KeyTypeID = ((int)(TheKeyInfoStructure.KeyType));
		retval = MapKeyTypeID(&KeyTypeID);
		TheKeyInfoStructure.KeyType.SetString(retval);

		//fills the row with the info stored in the structure
		FillRowSuccess = FillRow(TheKeyInfoStructure);
	}
	return FillRowSuccess;
}

//maps the status id returned from the lower layer app
//to the corresponding license as described in status.txt
char* KeyInfoListViewManager::MapLicenseID(int* pLicenseID, char* retval)
{
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
			case ExtendedTrialID6 :
			case ExtendedTrialID7 :
			case ExtendedTrialID8 :
			case ExtendedTrialID9 :
			case ExtendedTrialID10 :
			case ExtendedTrialID11 :
			case ExtendedTrialID12 :
			case ExtendedTrialID13 :
			case ExtendedTrialID14 :
			case ExtendedTrialID15 :
			{	//Extensions 6-15 are offset by 6 from their real values (ex. 12 = extension 6)
				sprintf_s(retval, 25, "Extended Trial%d", (*pLicenseID) - ExtendedOffset);				
				return retval; 
			}
			default :
				return retval = "Unknown License";			
		}
}

//maps the status id returned from the lower layer app
//to the corresponding license as described in status.txt
char* KeyInfoListViewManager::MapKeyTypeID(int* pKeyTypeID)
{
		char* retval;

		//map the Product ID according to KeySpec.xml
		switch (*pKeyTypeID)
		{
			case KEYNone :
				return retval = "Uninitialized Trial";
			case KEYAddon :
				return retval = "Addon";				
			case KEYBase :
				return retval = "Base";
			case KEYReplacement :
				return retval = "Replacement";				
			case KEYRelicense :
				return retval = "Relicense";				
			case KEYInventory :
				return retval = "Inventory";				
			case KEYBackup :
				return retval = "Backup";								
			case KEYEmergency :
				return retval = "Emergency";				
			//case //KEYSolimar :
			//	return retval = "Unused";				
			case KEYCustom : 
				return retval = "Custom";
			case KEYLoan :
				return retval = "Loan";
			case KEYDemo :
				return retval = "Demo";
			case KEYRental :
				return retval = "Rental";
			case KEYDevelopment :
				return retval = "Development";
			case KEYVerification :
				return retval = "Verification";
			default :
				return retval = "Reserved";
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
				return retval = "iCONVERT";
				
			case SolSearcherEnterpriseProductID :
				return retval = "SOLsearcher Enterprise";
				
			case SolScriptProductID : 
				return retval = "SOLscript";
				
			case SDXDesignerProductID :
				return retval = "SDX Designer";

			case SOLfusionProductID :
				return retval = "SOLfusion";

			case SPProductID :
				return retval = "SSE SP";

			case RubikaProductID :
				return retval = "Rubika";

			case SPDEProductID : 
				return retval = "SPDE";

			case VerificationKeyID:
				return retval = "Software Verification Key";

			default :
				return retval = "Unknown License";
		}
}

/*
int* KeyInfoListViewManager::MapProductName(String* pProductName)
{
		int* retVal;
		//map the Product Name according to KeySpec.xml
		switch (pProductName)
		{
			case L"SP/D Platinum" :
				return retVal = SPDProductID;

			case L"Connectivity Server" :
				return retval = ConnectivityServerProductID;
				
			case L"Quantum Server" :
				return retval = QuantumServerProductID;

			case "XImage":
				return retval = XImageProductID;
				
			case "XImage NT" :
				return retval = XImageNTProductID;
				
			case "PCL Driver" :
				return retval = PCLDriverProductID;
				
			case "IndexPlugInProductID" :
				return retval = IndexPlugInProductID;
								
			case "IConvert"  :
				return retval = IConvertProductID;
				
			case "SolSearcher Enterprise"  :
				return retval = SolSearcherEnterpriseProductID;
				
			case "SolScript" : 
				return retval = SolScriptProductID;
				
			case "SDX Designer" :
				return retval = SDXDesignerProductID;

			case "SSE SP" :
				return retval = SPProductID;

			case "Rubika" :
				return retval = RubikaProductID;

			case "SPDE" : 
				return retval = SPDEProductID;

			default :
				return retval = 256;
		}
}
*/
//Fills a row with the info in the key info structure
bool KeyInfoListViewManager::FillRow(KeyInfoStructure TheKeyInfoStructure)
{
	ListViewItem*  listViewItem1 = new ListViewItem();
	listViewItem1->UseItemStyleForSubItems = true;

	//Connection Type - Remote(Blue) or Local(Black)
	if(TheKeyInfoStructure.ApplicationInstance.intVal > 0) 
	{
		listViewItem1->ForeColor = System::Drawing::Color::SteelBlue;
	}

	//Active Status - Red,Italic or Black
	if(TheKeyInfoStructure.Active.intVal == 0) 
	{
		listViewItem1->Font = new System::Drawing::Font(listViewItem1->Font,System::Drawing::FontStyle::Italic);
		listViewItem1->ForeColor = System::Drawing::Color::Red;
	}


	//Key Type Column
	listViewItem1->Text = TheKeyInfoStructure.KeyType.bstrVal;
	//KeyNumber Column
	listViewItem1->SubItems->Add(TheKeyInfoStructure.KeyNumber.bstrVal);	
	//Product ID Column
	listViewItem1->SubItems->Add(TheKeyInfoStructure.ProductID.bstrVal);
	//Product Version Column
	listViewItem1->SubItems->Add(TheKeyInfoStructure.ProductVersion.bstrVal);
	//License Status Column
	listViewItem1->SubItems->Add(TheKeyInfoStructure.License.bstrVal);
	//Days Left Column
    //Convert the hours left into Days Left as requested by Tech Support
    TheKeyInfoStructure.HoursLeft /= 24;
	//Permanent Key
	if(TheKeyInfoStructure.HoursLeft == 0 && TheKeyInfoStructure.Active.intVal != 0)
		listViewItem1->SubItems->Add(S"Unlimited");
	else if(!String::Equals(TheKeyInfoStructure.KeyType.bstrVal, S"Development"))	//initial trial key
	{
		listViewItem1->SubItems->Add(S"-");
	}
	else
	{	
		char retval[10];
		sprintf_s(retval, sizeof(retval)/sizeof(char), "%d", TheKeyInfoStructure.HoursLeft);
		listViewItem1->SubItems->Add(retval);
	}
	//Convert from UTC time to Local time.
	SYSTEMTIME st;
	FILETIME ft, dft;
    LARGE_INTEGER li;  
	VariantTimeToSystemTime(TheKeyInfoStructure.ExpirationDate,&st);
	SystemTimeToFileTime(&st, &ft);
	FileTimeToLocalFileTime(&ft, &dft);
    li.LowPart = dft.dwLowDateTime;
    li.HighPart = dft.dwHighDateTime;    
	System::DateTime utc_t = System::DateTime::FromFileTimeUtc(li.QuadPart);

	//Expiration Date Column
	if(TheKeyInfoStructure.ExpirationDate)
	{
		//Permanent Key
		if(TheKeyInfoStructure.HoursLeft == 0 && TheKeyInfoStructure.Active.intVal != 0)
			listViewItem1->SubItems->Add(S"Unlimited");
		else	
			listViewItem1->SubItems->Add(utc_t.ToString());
	}

	//HACK JWL - 08-06-17 : yy-mm-dd
	//For verification keys, don't show other info, not important
	//if(String::Equals(TheKeyInfoStructure.ProductID.bstrVal, S"Software Verification Key"))
	if(String::Equals(TheKeyInfoStructure.KeyType.bstrVal, S"Verification"))
	{
		//delete listViewItem1;
		//listViewItem1 = new ListViewItem();
		listViewItem1->SubItems->Clear();
		//Key Type Column
		listViewItem1->Text = TheKeyInfoStructure.KeyType.bstrVal;
		
		//KeyNumber Column
		listViewItem1->SubItems->Add(TheKeyInfoStructure.KeyNumber.bstrVal);	

		//Product ID Column
		listViewItem1->SubItems->Add(TheKeyInfoStructure.ProductID.bstrVal);
	}
	ListViewItem* __mcTemp__2[] = new ListViewItem*[1];
    __mcTemp__2[0] = listViewItem1;
	this->TheKeyListView->Items->AddRange(__mcTemp__2);
	return true;
}

String* KeyInfoListViewManager::UnicodeStrToString(String* uniStr)
{
	UnicodeEncoding* unicode = new UnicodeEncoding();
	Byte encodedBytes[] = unicode -> GetBytes(uniStr);
	return unicode->GetString(encodedBytes);
}
