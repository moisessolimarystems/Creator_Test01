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

        #region Fields
        string _status;
        Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs _token;
        #endregion

        #region Constructor
        /// <summary>
        /// Initializes a new instance of the ValidationProperty class. 
        /// Represents a validation token for a license server.
        /// </summary>
        /// <param name="token">TokenTable object from database.</param>
        public ValidationProperty(TokenTable token)
        {
            _token = new Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs();
            _token.tokenType.TVal = (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType)token.TokenType ;
            _token.tokenValue.TVal = token.TokenValue;
            _status = ((TokenStatus)token.TokenStatus).ToString(); //"Active";
        }
        #endregion 

        #region Properties
        /// <summary>
        /// Gets or sets the collection of product licenses.
        /// </summary>
        public bool IsAllowedRemoveToken
        {
            get
            {
                if (!_token.tokenType.TVal.Equals(Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttLicenseCode))
                    return true;
                return false;
            }
        }

        /// <summary>
        /// Gets or sets the collection of product licenses.
        /// </summary>
        public Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs ValidationToken
        {
            get { return _token; }
            set { _token = value; }
        }

        /// <summary>
        /// Gets or sets the collection of product licenses.
        /// </summary>
        public Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType TokenType
        {
            get { return _token.tokenType.TVal; }
            set { _token.tokenType.TVal = value; }
        }

        /// <summary>
        /// Gets or sets the collection of product licenses.
        /// </summary>
        public string TokenValue
        {
            get { return _token.tokenValue.TVal; }
            set { _token.tokenValue.TVal = value; }
        }

        /// <summary>
        /// Gets or sets the collection of product licenses.
        /// </summary>
        public string Status
        {
            get{ return _status;  }
            set { _status = value; }
        }
        #endregion
    }
}
