using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Client.Creator
{
    class AppConstants
    {
        public static readonly String LicenseServerName = "Solimar License Server";
        //Form Constants
        public static readonly String ClientTitle = "Creator";
        //XML File/Folder Path
        public static readonly String LocalConnectionFilePath = "\\Solimar\\LicenseCreator\\";
        public static readonly String LocalConnectionFile = "Reports.xml";

        public static readonly int MaxActivations = 255;
        public static readonly int MaxActivationAmountInDays = 170;
        public static readonly int MaxProductLicenses = 20;

        //License Object Type Names
        public const String LicenseServerObjectName = "LicenseServer";
        public const String ProductCollectionObjectName = "ProductCollection";
        public const String ProductLicenseObjectName = "ProductLicense";

        //License Properties Tab Page Names
        public const String PropertyGridTabPage = "PropertyGridTabPage";
        public const String TransactionsTabPage = "TransactionsTabPage";
        public const String PacketTabPage = "PacketTabPage";

        //LicenseView Tab Page Names
        public const String CustomersTabPage = "CustomersTabPage";
        public const String LicensesTabPage = "LicensesTabPage";
        public const String ValidationKeysTabPage = "ValidationKeysTabPage";
        public const String ReportTabPage = "ReportTabPage";

        //View Items
        public static readonly int CustomersView = 0;
        public static readonly int LicensesView = 1;
        public static readonly int TokensView = 2;
        public static readonly int ReportsView = 3;

        public class ReportXMLTags
        {
            //ReportElement
            //NameAttrib
            //TypeAttrib
            //ConditionElement
            //NameAttrib
            //OperatorAttrib
            //ValueAttrib
            public static readonly String ReportStructureElement = "ReportStructure";
            public static readonly String ReportElement = "Report";
            public static readonly String IdAttrib = "id";
            public static readonly String TypeAttrib = "type";
            public static readonly String MatchAttrib = "matchAny";
            public static readonly String ConditionElement = "Condition";
            public static readonly String NameAttrib = "name";
            public static readonly String OperatorAttrib = "operator";
            public static readonly String ValueAttrib = "value";
        }
    }
}
