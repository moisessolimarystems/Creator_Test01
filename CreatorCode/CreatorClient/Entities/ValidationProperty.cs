using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using Solimar.Licensing.Attribs;
using Client.Creator.CreatorService;
using Client.Creator.ServiceProxy;

namespace Client.Creator
{
    public class ValidationProperty : BaseGridItem
    {


        string _status;
        Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs _token;

        public ValidationProperty()
        {
            _token = new Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs();
            _status = "Active";
        }

        public ValidationProperty(Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs token)
        {

            _token = token;
            _status = "Active";
        }

        public ValidationProperty(TokenTable token)
        {

            _token = new Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs();
            _token.tokenType.TVal = (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType)token.TokenType ;
            _token.tokenValue.TVal = token.TokenValue;
            _status = "Active";
        }

        public override string Name
        {
            //need to translate the id to name;
            get { return System.Enum.GetName(typeof(Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType), TokenType); }
        }

        [Browsable(false)]
        public bool IsAllowedRemoveToken
        {
            get
            {
                if (!_token.tokenType.TVal.Equals(Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttLicenseCode))
                    return true;
                return false;
            }
        }

        [Browsable(false)]
        public Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs ValidationToken
        {
            get { return _token; }
            set { _token = value; }
        }

        [DisplayName("Token Type")]
        [Description("Token Type")]
        [Category("ValidationProperty")]
        public Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType TokenType
        {
            get { return _token.tokenType.TVal; }
            set { _token.tokenType.TVal = value; }
        }

        [DisplayName("Token Value")]
        [Description("Token Value")]
        [Category("ValidationProperty")]
        public string TokenValue
        {
            get { return _token.tokenValue.TVal; }
            set { _token.tokenValue.TVal = value; }
        }

        public string Status
        {
            get{ return _status;  }
            set { _status = value; }         
        }

        public override string ToString()
        {
            return Name;
        }
    }
}
