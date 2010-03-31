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
