#include "stdafx.h"
#include "ServerConfiguration.h"
//#include <shfolder.h> 

using namespace KeyReaderGUI;

System::Void ServerConfiguration::ServerConfiguration_Load(System::Object*  sender, System::EventArgs*  e)
{
 	TheConnectionSetting = new ConnectionSettings();
	//Read Registry for installed products
	RegistryKey* rkey = Registry::LocalMachine->OpenSubKey(SOLIMAR_KEY);
	if(rkey)
	{
		// Retrieve all the subkeys for the specified key.
		String* productNames[] = rkey->GetSubKeyNames();
		// Print the contents of the array to the console.
		System::Collections::IEnumerator* enum0 = productNames->GetEnumerator();
		while (enum0->MoveNext())
		{
			String* s = __try_cast<String*>(enum0->Current);
			if(!s->Equals(LICENSE_PRODUCT)) 
			{
				if(MapProductName(s) >= 0)
					ProductComboBox->Items->Add(s);
			}
		}
		rkey->Close();
		if(productNames->Count > 1)	//Solimar Licensing is one key
			ProductComboBox->SelectedIndex = 0;
	}	
	ServerPropertyGrid->SelectedObject = TheConnectionSetting;
}

System::Void ServerConfiguration::OKBtn_Click(System::Object*  sender, System::EventArgs*  e)
{
	SaveSettings(MapProductName(static_cast<String*>(ProductComboBox->SelectedItem)));
	DialogResult = DialogResult::OK;
}

System::Void ServerConfiguration::CancelBtn_Click(System::Object*  sender, System::EventArgs*  e)
{
	DialogResult = DialogResult::Cancel;
}

System::Void ServerConfiguration::ProductComboBox_SelectedIndexChanged(System::Object*  sender, System::EventArgs*  e)
{
	if(m_ValueChanged)
		SaveSettings(MapProductName(m_LastSelectedProduct));	
	m_LastSelectedProduct = static_cast<String*>(ProductComboBox->SelectedItem);
	LoadSettings(MapProductName(static_cast<String*>(ProductComboBox->SelectedItem)));
}

System::Void ServerConfiguration::ServerPropertyGrid_PropertyValueChanged(System::Object*  s, System::Windows::Forms::PropertyValueChangedEventArgs*  e)
{
	m_ValueChanged = true;
}
bool ServerConfiguration::LoadSettings(int ProductID)
{
	bool bRetVal = false;
	TheConnectionSetting->ResetSettings();
	String* filePath = String::Concat(Environment::GetFolderPath(Environment::SpecialFolder::CommonApplicationData),
									  LOCAL_CONNECTION_FILE_PATH,
									  LOCAL_CONNECTION_FILE);
	if(File::Exists(filePath))
	{
		FileStream* fs = new FileStream(filePath, FileMode::OpenOrCreate, FileAccess::Read, FileShare::Read);
		// Create the XmlReader object.
		XmlReader* reader = XmlReader::Create(fs);
		//find product section for specified id in xml
		reader->ReadToFollowing(PRODUCT_ELEMENT);
		do
		{
			if(String::Equals(reader->GetAttribute(ID_ATTRIB),ProductID.ToString()))
			{
				reader->ReadToDescendant(SERVER_ELEMENT);
				do
				{
					//set values for TheConnectionSettings
					if(reader->GetAttribute(BACKUP_ATTRIB)->Equals("0") && reader->GetAttribute(SHARED_ATTRIB)->Equals("0")) 
					{
						TheConnectionSetting->set_ServerName(reader->GetAttribute(NAME_ATTRIB));
						continue;
					}
					if(reader->GetAttribute(BACKUP_ATTRIB)->Equals("1") && reader->GetAttribute(SHARED_ATTRIB)->Equals("0"))
					{			
						TheConnectionSetting->set_BackupName(reader->GetAttribute(NAME_ATTRIB));
						continue;
					}	
					if(reader->GetAttribute(BACKUP_ATTRIB)->Equals("0") && reader->GetAttribute(SHARED_ATTRIB)->Equals("1"))				
					{
						TheConnectionSetting->set_SharedServerName(reader->GetAttribute(NAME_ATTRIB));
						continue;
					}
					if(reader->GetAttribute(BACKUP_ATTRIB)->Equals("1") && reader->GetAttribute(SHARED_ATTRIB)->Equals("1"))				
					{			
						TheConnectionSetting->set_SharedBackupName(reader->GetAttribute(NAME_ATTRIB));					
						continue;
					}
					bRetVal = true;
				}
				while(reader->ReadToNextSibling(SERVER_ELEMENT));
				break;
			}
		}
		while(reader->ReadToNextSibling(PRODUCT_ELEMENT));
		reader->Close();
		fs->Close();
	}

	if(!bRetVal)
		SaveSettings(ProductID);

	//force propertygrid to show any new values
	ServerPropertyGrid->Refresh();
	return bRetVal;
}

bool ServerConfiguration::SaveSettings(int ProductID)
{
	bool bRetVal = false;
	String* filePath = String::Concat(Environment::GetFolderPath(Environment::SpecialFolder::CommonApplicationData),
									  LOCAL_CONNECTION_FILE_PATH,
									  LOCAL_CONNECTION_FILE);
	if(!File::Exists(filePath))
	{
		if(!CreateDefaultXML())
			return false;
	}

	String* tempFilePath = String::Concat(Environment::GetFolderPath(Environment::SpecialFolder::CommonApplicationData),
										  LOCAL_CONNECTION_FILE_PATH,
										  "temp.xml");
	FileStream* fsRead = new FileStream(filePath, FileMode::OpenOrCreate, FileAccess::ReadWrite, FileShare::ReadWrite);	
	FileStream* fsWrite = new FileStream(tempFilePath, FileMode::OpenOrCreate, FileSystemRights::Modify, FileShare::ReadWrite, 8, FileOptions::None);	
	XmlReader* reader = XmlReader::Create(fsRead);
	XmlWriterSettings* settings = new XmlWriterSettings;
	settings->Indent = true;
	XmlWriter* writer = XmlWriter::Create(fsWrite, settings);
	while (reader->Read())
	{
		if ( reader->NodeType == XmlNodeType::Element )
		{
			//ignore existing product settings, will overwrite.
			if(String::Equals(reader->Name, PRODUCT_ELEMENT) && String::Equals(reader->GetAttribute(ID_ATTRIB), ProductID.ToString()))
			{	
				reader->Skip();
				continue;
			}
			//copy from read stream
			writer->WriteStartElement(reader->Name);
			writer->WriteAttributes( reader, false);
			if (reader->IsEmptyElement )
				writer->WriteEndElement();	
			//insert product settings
			if(String::Equals(reader->Name, SERVER_STRUCTURE_ELEMENT))
			{
				writer->WriteStartElement(PRODUCT_ELEMENT);
				writer->WriteAttributeString(ID_ATTRIB, ProductID.ToString());
				//write primary
				writer->WriteStartElement(SERVER_ELEMENT);
				writer->WriteAttributeString(NAME_ATTRIB, TheConnectionSetting->get_ServerName());
				writer->WriteAttributeString(BACKUP_ATTRIB, "0");
				writer->WriteAttributeString(SHARED_ATTRIB, "0");
				writer->WriteEndElement();
				//write shared primary
				writer->WriteStartElement(SERVER_ELEMENT);
				writer->WriteAttributeString(NAME_ATTRIB, TheConnectionSetting->get_SharedServerName());
				writer->WriteAttributeString(BACKUP_ATTRIB, "0");
				writer->WriteAttributeString(SHARED_ATTRIB, "1");
				writer->WriteEndElement();				
				//write secondary
				writer->WriteStartElement(SERVER_ELEMENT);
				writer->WriteAttributeString(NAME_ATTRIB, TheConnectionSetting->get_BackupName());
				writer->WriteAttributeString(BACKUP_ATTRIB, "1");
				writer->WriteAttributeString(SHARED_ATTRIB, "0");
				writer->WriteEndElement();
				//write shared secondary
				writer->WriteStartElement(SERVER_ELEMENT);
				writer->WriteAttributeString(NAME_ATTRIB, TheConnectionSetting->get_SharedBackupName());
				writer->WriteAttributeString(BACKUP_ATTRIB, "1");
				writer->WriteAttributeString(SHARED_ATTRIB, "1");
				writer->WriteEndElement();
				//Close Product Element
				writer->WriteEndElement();
				bRetVal = true;
			}
		}
		if ( reader->NodeType == XmlNodeType::EndElement )
		{
			writer->WriteEndElement();
		}
	}
	writer->Flush();
	writer->Close();
	reader->Close();
	fsRead->Close();
	fsWrite->Close();
	File::Delete(filePath);
	File::Move(tempFilePath, filePath);
	//give everyone rights to new file. Should surround with try/catch incase of failure.
    AddFileSecurity(filePath, 
				    "Everyone", 
	             	FileSystemRights::FullControl, 
					AccessControlType::Allow);
	m_ValueChanged = false;
	return bRetVal;
}

int ServerConfiguration::MapProductName(String* ProductName)
{
	if(ProductName != NULL)
	{
		ProductName = ProductName->ToUpper();
		if(ProductName->Equals(L"ICONVERT"))
			return IConvertProductID;
		else if(ProductName->Equals(L"SOLSEARCHER"))
			return SolSearcherEnterpriseProductID;
		else if(ProductName->Equals(L"SOLSCRIPT"))
			return SolScriptProductID;
		else if(ProductName->Equals(L"SDX DESIGNER"))
			return SDXDesignerProductID;
		else if(ProductName->Equals(L"RUBIKA"))
			return RubikaProductID;
		else if(ProductName->Equals(L"SPDE"))
			return SPDEProductID;
		else if(ProductName->Equals(L"SOLFUSION"))
			return SOLfusionProductID;
	}
	return -1;
}
//<LicensingStructure>
//	<Version value=""/>
//		<ServerStructure>
//		</ServerStructure>
//</LicensingStructure>
bool ServerConfiguration::CreateDefaultXML()
{
	String* filePath = String::Concat(Environment::GetFolderPath(Environment::SpecialFolder::CommonApplicationData),
								      LOCAL_CONNECTION_FILE_PATH);	
	DirectoryInfo* di = new DirectoryInfo(filePath);
    try
    {
		if(di->Exists == false)
			di->Create();
    }
    catch (Exception* /*e*/ ) 
    {
		return false;
    }     
	FileStream* fsWrite = new FileStream(String::Concat(filePath,LOCAL_CONNECTION_FILE), FileMode::OpenOrCreate, FileSystemRights::Modify, FileShare::ReadWrite, 8, FileOptions::None);	
	XmlWriterSettings* settings = new XmlWriterSettings;
	settings->Indent = true;
	XmlWriter* writer = XmlWriter::Create(fsWrite, settings);
	//LicensingStructure Element
	writer->WriteStartElement(LICENSING_STRUCTURE_ELEMENT);
	//Version Element
	writer->WriteStartElement(VERSION_ELEMENT);
	//Value Attribute
	writer->WriteAttributeString(VALUE_ATTRIB, "1.0");
	writer->WriteEndElement();
	//ServerStructure Element
	writer->WriteStartElement(SERVER_STRUCTURE_ELEMENT);
	writer->WriteFullEndElement();	
	//Close Licensing Structure Element
	writer->WriteFullEndElement();
	writer->Flush();
	writer->Close();
	fsWrite->Close();
	return true;
}

/*
 * Function    : AddFileSecurity
 * Description : Gives account the supplied rights to filename
 * Parameters  : fileName - 
 *				 account  - 
 *				 rights	  -
 *				 controlType -
 */
void ServerConfiguration::AddFileSecurity(String* fileName, String* account, 
                        FileSystemRights rights, AccessControlType controlType)
{
    // Get a FileSecurity object that represents the 
    // current security settings.
    FileSecurity* fSecurity = File::GetAccessControl(fileName);

    // Add the FileSystemAccessRule to the security settings. 
    fSecurity->AddAccessRule(new FileSystemAccessRule
                                   (account,rights, controlType));

    // Set the new access settings.
    File::SetAccessControl(fileName, fSecurity);
}
