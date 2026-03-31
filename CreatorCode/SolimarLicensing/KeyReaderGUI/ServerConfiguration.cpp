#include "stdafx.h"
#include "ServerConfiguration.h"

using namespace KeyReaderGUI;

System::Void ServerConfiguration::ServerConfiguration_Load(System::Object*  sender, System::EventArgs*  e)
{
 	TheConnectionSetting = new ConnectionSettings();
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
				ProductComboBox->Items->Add(s);
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
	RegistryKey* prodKey = Registry::LocalMachine->OpenSubKey(LICENSE_PARENT_KEY);
	prodKey = prodKey->OpenSubKey(ProductID.ToString());
	//Product SubKey
	if(prodKey != NULL)
	{	//Connection SubKeys
		String* connectKeys[] = prodKey->GetSubKeyNames();
		for(int i = 0; i < connectKeys->Length; i++)
		{	//Connection Key
			RegistryKey* connectKey = prodKey->OpenSubKey(connectKeys[i]);
			if(connectKey)
			{	//Connection Values
				String* valueNames[] = connectKey->GetValueNames();
				for(int j = 0; j < valueNames->Length; j++)
				{
					if(valueNames[j]->Equals(SERVER_NAME_KEY_VALUE))
					{
						//set values for TheConnectionSettings
						if(connectKeys[i]->Equals(PRIMARY_CONNECTION))
							TheConnectionSetting->set_ServerName(connectKey->GetValue(valueNames[j])->ToString());
						else if(connectKeys[i]->Equals(SECONDARY_CONNECTION))
							TheConnectionSetting->set_BackupName(connectKey->GetValue(valueNames[j])->ToString());
						else if(connectKeys[i]->Equals(SHARED_PRIMARY_CONNECTION))					
							TheConnectionSetting->set_SharedServerName(connectKey->GetValue(valueNames[j])->ToString());
						else					
							TheConnectionSetting->set_SharedBackupName(connectKey->GetValue(valueNames[j])->ToString());					
						bRetVal = true;
					}
				}
			}
			connectKey->Close();
		}
		prodKey->Close();
	}
	//force propertygrid to show any new values
	ServerPropertyGrid->Refresh();
	return bRetVal;
}

bool ServerConfiguration::SaveSettings(int ProductID)
{
	bool bRetVal = false;
	if(ProductID > 0)
	{
		RegistryKey* prodKey = Registry::LocalMachine->CreateSubKey(LICENSE_PARENT_KEY);
		prodKey = prodKey->CreateSubKey(ProductID.ToString());
		//Product SubKey
		if(prodKey)
		{	//Connection SubKeys
			RegistryKey* tempKey = prodKey->CreateSubKey(PRIMARY_CONNECTION);
			//set values for Primary Connection
			tempKey->SetValue(SERVER_NAME_KEY_VALUE, TheConnectionSetting->get_ServerName());
			tempKey->SetValue(SHARED_KEY_VALUE, __box(REGISTRY_FALSE));
			tempKey->SetValue(BACKUP_KEY_VALUE, __box(REGISTRY_FALSE));
			tempKey = prodKey->CreateSubKey(SECONDARY_CONNECTION);
			//set values for secondary connection
			tempKey->SetValue(SERVER_NAME_KEY_VALUE, TheConnectionSetting->get_BackupName());
			tempKey->SetValue(SHARED_KEY_VALUE, __box(REGISTRY_FALSE));
			tempKey->SetValue(BACKUP_KEY_VALUE, __box(REGISTRY_TRUE));
			tempKey = prodKey->CreateSubKey(SHARED_PRIMARY_CONNECTION);
			//set values for shared primary connection
			tempKey->SetValue(SERVER_NAME_KEY_VALUE, TheConnectionSetting->get_SharedServerName());
			tempKey->SetValue(SHARED_KEY_VALUE, __box(REGISTRY_TRUE));
			tempKey->SetValue(BACKUP_KEY_VALUE, __box(REGISTRY_FALSE));
			tempKey = prodKey->CreateSubKey(SHARED_SECONDARY_CONNECTION);
			//set values for shared secondary connection
			tempKey->SetValue(SERVER_NAME_KEY_VALUE, TheConnectionSetting->get_SharedBackupName());
			tempKey->SetValue(SHARED_KEY_VALUE, __box(REGISTRY_TRUE));
			tempKey->SetValue(BACKUP_KEY_VALUE, __box(REGISTRY_TRUE));
			tempKey->Close();
			bRetVal = true;
		}
		prodKey->Close();
		m_ValueChanged = false;
	}
	return bRetVal;
}

int ServerConfiguration::MapProductName(String* ProductName)
{
	if(ProductName != NULL)
	{
		ProductName = ProductName->ToUpper();
		if(ProductName->Equals(L"SPD"))
			return SPDProductID;
		else if(ProductName->Equals(L"ICONVERT"))
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
	}
	return -1;
}