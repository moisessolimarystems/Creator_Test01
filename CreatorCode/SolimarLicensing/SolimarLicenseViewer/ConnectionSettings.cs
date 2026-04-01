using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;

namespace SolimarLicenseViewer
{
    public class ConnectionSettings
    {
        public ConnectionSettings()
        {
            ResetSettings();
        }

		public void ResetSettings()
		{
			serverName = "localhost";
			backupName = "localhost"; 
			sharedServerName = "localhost";
			sharedBackupName = "localhost";
		}

        [Category("Primary Connection"), 
		 Description("Primary server name."),
		 DisplayName("Name")]
        public String ServerName
        {
            get
            {
                return serverName;
            }
            set
            {
                serverName = value;
            }
        }
		[Category("Secondary Connection"), 
		 Description("Primary shared server name."),
	     DisplayName("Name")]
        public String BackupName
        {
            get
            {
                return backupName;
            }
            set
            {
                backupName = value;
            }
        }
		[Category("Server Backup Connection"), 
		 Description("Backup server name."),
	     DisplayName("Name")]
        public String SharedServerName
        {
            get
            {
                return sharedServerName;
            }
            set
            {
                sharedServerName = value;
            }
        }
		[Category("Shared Backup Connection"), 
		 Description("Backup shared server name"),
		 DisplayName("Name")
         ] 
        public String SharedBackupName
        {
            get
            {
                return sharedBackupName;
            }
            set
            {
                sharedBackupName = value;
            }
        }

        private String serverName;
        private String backupName;
        private String sharedServerName;
        private String sharedBackupName;
    }

    public class ConnectionSettings2
    {
        public ConnectionSettings2()
        {
            ResetSettings();
        }

		public void ResetSettings()
		{
            productID = -1;
			serverName = "localhost";
			backupName = "";
            bUseTestDev = false;
		}

        [Category("Licensing"),
         Description("License Server that the product will contact for its licensing needs."),
         DisplayName("Primary License Server")]
        public String ServerName
        {
            get
            {
                return serverName;
            }
            set
            {
                serverName = value;
                IsDirty = true;
            }
        }

        [Category("Licensing"),
         Description("License Server that the product will contact for its licensing needs if an active connection to the Primary License Server is lost."),
         DisplayName("Failover License Server")]
        public String BackupName
        {
            get
            {
                return backupName;
            }
            set
            {
                backupName = value;
                IsDirty = true;
            }
        }

        [Category("Licensing"),
         Description("Set to true if you want the given product to use the Test/Dev/DR licensing for testing."),
         DisplayName("Use Test/Dev/DR Licensing")]
        public bool UseDevelopmentLic
        {
            get
            {
                return bUseTestDev;
            }
            set
            {
                bUseTestDev = value;
                IsDirty = true;
            }
        }
        [Browsable(false)]          //Don't show in propertyGrid
        public bool IsDirty
        {
            get { return bIsDirty; }
            set { bIsDirty = value; }
        }

        [Browsable(false),          //Don't show in propertyGrid
         ReadOnly(true),            //Don't allow to be edited in propertyGrid
         MergableProperty(false)]   //Don't display if multiple objects are selected in the propertyGrid
        public int ProductID
        {
            get { return productID; }
            set { productID = value; }
        }
        
        private String serverName;
        private String backupName;
        private int productID;
        private bool bUseTestDev;
        private bool bIsDirty;
    }
}
