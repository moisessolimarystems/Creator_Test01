using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using Solimar.Licensing.Attribs;

namespace Client.Creator
{
    public class ValidationProperty : BaseGridItem
    {
        Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs _token;

        public ValidationProperty()
        {
            _token = new Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs();            
        }

        public ValidationProperty(Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs token)
        {
            _token = token;
        }

        public void SetValidationProperty(ValidationProperty token)
        {            
            //_tokenType = token.TokenType;
            //_tokenValue = token.TokenValue;                
        }

        public override string Name
        {
            //need to translate the id to name;
            get { return System.Enum.GetName(typeof(Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType), TokenType); }
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

        public override string ToString()
        {
            return Name;
        }
    }
}
