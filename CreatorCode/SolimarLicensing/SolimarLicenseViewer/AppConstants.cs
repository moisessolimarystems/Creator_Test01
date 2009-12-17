using System;
using System.Collections.Generic;
using System.Text;

namespace SolimarLicenseViewer
{
    public class AppConstants
    {
        //initial key created when installed
        public static readonly String SolimarRegKey     = "SOFTWARE\\Solimar";
        public static readonly String SolimarRegKey32On64 = "SOFTWARE\\Wow6432Node\\Solimar";
        public static readonly String LicenseProduct    = "Solimar Licensing";        
        
        //XML File/Folder Path
        public static readonly String LocalConnectionFilePath = "\\Solimar\\SolimarLicenseServer\\";
        public static readonly String LocalConnectionFile     = "LocalConnectionSettings.xml";
        
        //Form Constants
        public static readonly String LicenseView = "License";
        public static readonly String UsageView = "Usage";
        public static readonly String FormTitle = "Solimar License Viewer";
        public static readonly String DiagnosticDataTitle = "Solimar License Diagnostic Data Viewer";
        public static readonly String UnlimitedValue = "Unlimited";


        //TreeView Constants
        public static readonly String LicenseRootNode = "Licenses";
        public static readonly String UsageRootNode = "Usage";
        public static readonly String NoConnectedProductsRootNode = "No Connected Products";
        public static readonly String ProtectionKeyRootNode = "Protection Keys";
        public static readonly String ProductConnectionSettingsRootNode = "Product Connection Settings";

        //ListViewItem Constants
        public static readonly String DisasterRecoveryLicType = "sltDisasterRecovery";
        public static readonly String ProductConnectionSettingsUnknownStatus = "Status Unknown";

        #region ToolStripToolBar Constant
        public static readonly String ProdSettingsEditConnTSB = "Edit Connection Settings";
        public static readonly String ProdSettingsEditConnToolTipTSB = "Edit Connection Settings for the given Product";
        public static readonly String ProdSettingsTestConnSelectedTSB = "Test Connection Settings";
        public static readonly String ProdSettingsTestConnSelectedToolTipTSB = "Test Connection to License Server for the given Product";
        public static readonly String ProdSettingsTestConnAllTSB = "Test All Connection Settings";
        public static readonly String ProdSettingsTestConnAllToolTipTSB = "Test Connection to All License Server";
        #endregion

        #region ListView Constants

        public static readonly String VerifiedStatus = "Verified";
        public static readonly String UnVerifiedStatus = "UnVerified";
        public static readonly String NotUsedStatus = "Not Used";
        public static readonly String ExpiredStatus = "Activation Expired";
        public static readonly String HistoryNode = "History";
        

        //License Node Column Headers
        public static readonly String NameHeader = "Name";
        public static readonly String LicenceTypeHeader = "License Type";
        public static readonly String VerificationStatusHeader = "Verification Status";
        public static readonly String ExpirationHeader = "Expiration Date";
        //public static readonly String CurrentActivationHeader = "Current Activation";
        public static readonly String CurrentActivationHeader = "Activation Usage";
        public static readonly String TotalActivationHeader = "Total Activations";
        public static readonly String DaysPerActivationHeader = "Days Per Activation";
        public static readonly String ActivationExpirationHeader = "Current Activation Valid Until";
        public static readonly String KeyTypeHeader = "Key Type";
        public static readonly String KeyNameHeader = "Key Name";
        public static readonly String KeyProductIdHeader = "Product ID";
        public static readonly String KeyProductVersionHeader = "Version";
        public static readonly String KeyLicenseTypeHeader = "License Type";
        public static readonly String KeyDaysLeftHeader = "Days Left";
        public static readonly String KeyExpirationDateHeader = "Expiration Date";
        public static readonly String ConnProductHeader = "Product";
        public static readonly String ConnPrimaryServerHeader = "Primary License Server";
        public static readonly String ConnBackupServerHeader = "Fail Over Server";
        public static readonly String ConnTestDevHeader = "Use Development Licensing";
        public static readonly String ConnStatusHeader = "Status";

        public static readonly String FilterHeader = "View";
        public static readonly String GroupByModuleItem = "Group By Module";
        public static readonly String DetailsItem = "Details";
        public static readonly String ProductLicNumberItem = "Group By Product License";


        //Product Node Column Headers
        public static readonly String VersionHeader = "Version";
        public static readonly String AppInstanceHeader = "Application Instance";
        //History Node Column Headers
        public static readonly String VerificationCodeHeader = "Verification Code";
        public static readonly String DateAppliedHeader = "Date Applied";
        //Module Node Column Headers
        public static readonly String SummaryAllProdLicNum = " Summary of all Product Licenses";
        public static readonly String ProdLicNumHeader = "Product License";
        public static readonly String ProdLicNumStateHeader = "Status";
        public static readonly String LicensesHeader = "Licenses";
        public static readonly String UnitsHeader = "Module";
        public static readonly String AppInstanceExpiredHeader = "Expired Application Instance";
        public static readonly String BottomLicenseRootNode = "BottomLicenses";
        public static readonly String BottomUnitsHeader = "BottomModule";


        //Usage Node Column Headers
        public static readonly String UsageProductHeader = "Product";
        public static readonly String UsageAppInstanceHeader = "Application Instance";
        public static readonly String UsageModuleHeader = "Module";
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
