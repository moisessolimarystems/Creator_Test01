using System;
using System.Collections.Generic;
using System.Text;

namespace SolimarLicenseViewer
{
    public class AppConstants
    {
        //initial key created when installed
        public static readonly String SolimarRegKey     = "SOFTWARE\\Solimar";
        public static readonly String LicenseProduct    = "Solimar Licensing";        
        
        //XML File/Folder Path
        public static readonly String LocalConnectionFilePath = "\\Solimar\\SolimarLicenseServer\\";
        public static readonly String LocalConnectionFile     = "LocalConnectionSettings.xml";
        
        //Form Constants
        public static readonly String LicenseView = "License";
        public static readonly String UsageView = "Usage";
        public static readonly String FormTitle = "License Viewer";
        public static readonly String UnlimitedValue = "Unlimited";


        //TreeView Constants
        public static readonly String LicenseRootNode = "Licenses";
        public static readonly String UsageRootNode = "No Connected Products";
        public static readonly String ProtectionKeyRootNode = "Protection Keys";

        //ListViewItem Constants
        public static readonly String DisasterRecoveryLicType = "sltDisasterRecovery";

        #region ListView Constants

        public static readonly String VerifiedStatus = "Verified";
        public static readonly String UnVerifiedStatus = "UnVerified";
        public static readonly String NotUsedStatus = "Not Used";
        public static readonly String HistoryNode = "History";
        

        //License Node Column Headers
        public static readonly String NameHeader = "Name";
        public static readonly String LicenceTypeHeader = "License Type";
        public static readonly String VerificationStatusHeader = "Verification Status";
        public static readonly String ExpirationHeader = "Valid Until";
        public static readonly String CurrentActivationHeader = "Current Activation";
        public static readonly String TotalActivationHeader = "Total Activations";
        public static readonly String DaysPerActivationHeader = "Days Per Activation";
        public static readonly String KeyTypeHeader = "Key Type";
        public static readonly String KeyNameHeader = "Key Name";
        public static readonly String KeyProductIdHeader = "Product ID";
        public static readonly String KeyProductVersionHeader = "Version";
        public static readonly String KeyLicenseTypeHeader = "License Type";
        public static readonly String KeyDaysLeftHeader = "Days Left";
        public static readonly String KeyExpirationDateHeader = "Expiration Date";


        //Product Node Column Headers
        public static readonly String VersionHeader = "Version";
        public static readonly String AppInstanceHeader = "AppInstance";
        //History Node Column Headers
        public static readonly String VerificationCodeHeader = "Verification Code";
        public static readonly String DateAppliedHeader = "Date Applied";
        //Module Node Column Headers
        public static readonly String LicensesHeader = "Licenses";
        public static readonly String UnitsHeader = "Units";
        //Usage Node Column Headers
        public static readonly String ObtainedHeader = "Obtained";
        public static readonly String TotalHeader = "Total";
        //Protection Key Module Column Hearers
        public static readonly String PkModuleHeader = "Module Name";
        public static readonly String PkModuleInUseHeader = "In Use";
        public static readonly String PkModuleTotalHeader = "Total Licenses";
        
        #endregion

        public class ProductSettingsXML
        {
            //XML Elements & Attributes
            public static readonly String LicensingStructureElement  = "LicensingStructure";
            public static readonly String ServerStructureElement     = "ServerStructure";
            public static readonly String VersionElement			 = "Version";
            public static readonly String ValueAttrib				 = "value";
            public static readonly String ServerElement			     = "Server";
            public static readonly String ProductElement		 	 = "Product";
            public static readonly String IdAttrib					 = "id";
            public static readonly String NameAttrib				 = "name";
            public static readonly String BackupAttrib				 = "backup";
            public static readonly String SharedAttrib               = "shared";

            //new to verion 2.0
            public static readonly String SoftwareLicAttrib          = "softwareLic";   //Not currently used
            public static readonly String TestDevAttrib              = "testDev";
            public static readonly String DisasterRecoveryAttrib     = "disasterRecovery";  //Not currently used
        }
    }
}
