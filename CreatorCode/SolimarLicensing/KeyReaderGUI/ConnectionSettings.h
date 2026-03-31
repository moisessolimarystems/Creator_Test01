#pragma once
namespace KeyReaderGUI
{
	using namespace System;
	using namespace System::ComponentModel;
	
	public __gc class ConnectionSettings : public System::Object 
	{
		public:
			ConnectionSettings(void) : 
				serverName(L"localhost"), 
				backupName(L"localhost"), 
				sharedServerName(L"localhost"), 
				sharedBackupName(L"localhost") {};

		public:
			[Category("Primary Connection"), 
			 Description("Primary server name."),
			 DisplayName("Name")] 
			__property String* get_ServerName() { return serverName; }
			__property void set_ServerName(String* value) { serverName = value; } 
			
			[Category("Secondary Connection"), 
			 Description("Backup server name."),
			 DisplayName("Name")] 
			__property String* get_BackupName() { return backupName; }
			__property void set_BackupName(String* value) { backupName = value; } 

			[Category("Shared Primary Connection"), 
			 Description("Primary shared server name."),
			 DisplayName("Name")] 
			__property String* get_SharedServerName() { return sharedServerName; }
			__property void set_SharedServerName(String* value) { sharedServerName = value; } 

			[Category("Shared Secondary Connection"), 
			 Description("Backup shared server name"),
			 DisplayName("Name")] 
			__property String* get_SharedBackupName() { return sharedBackupName; }
			__property void set_SharedBackupName(String* value) { sharedBackupName = value; } 

		public:
			~ConnectionSettings(void){};

		private:
			String* serverName;
			String* backupName;
			String* sharedServerName;
			String* sharedBackupName;
	};
}
