// Validation.cs
//
// SLB 15.sep.2025 CR.34456; Changes for new attribs code (Licensing 3.4+) to work.
// Renamed member variables to have m_ prefix vs. _. 

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
    public class Validation : BaseGridItem
    {

        #region Fields
        string m_status;															// CR.34456; modified.
        Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs m_token; // CR.34456; modified.
		#endregion

		#region Constructor
		/// <summary>
		/// Initializes a new instance of the Validation class. 
		/// Represents a validation token for a license server.
		/// </summary>
		/// <param name="token">TokenTable object from database.</param>
		public Validation(TokenTable token)
        {
            m_token = new Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs();
            m_token.tokenType.TVal = (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType)token.TokenType ;
            m_token.tokenValue.TVal = token.TokenValue;
            m_status = ((TokenStatus)token.TokenStatus).ToString(); //"Active";
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
                //return !_token.tokenType.TVal.Equals(Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttLicenseCode);
                if (!m_token.tokenType.Equals(Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttLicenseCode)) // CR.34456; modified.
					return true;
                return false;
            }
        }

        /// <summary>
        /// Gets or sets the collection of product licenses.
        /// </summary>
        public Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs ValidationToken
        {
            get { return m_token; }
            set { m_token = value; }
        }

        /// <summary>
        /// Gets or sets the collection of product licenses.
        /// </summary>
        public Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType TokenType
        {
            get { return m_token.tokenType.TVal; }
            set { m_token.tokenType.TVal = value; }
        }

        /// <summary>
        /// Gets or sets the collection of product licenses.
        /// </summary>
        public string TokenValue
        {
            get { return m_token.tokenValue.TVal; }
            set { m_token.tokenValue.TVal = value; }
        }

        /// <summary>
        /// Gets or sets the collection of product licenses.
        /// </summary>
        public string Status
        {
            get{ return m_status;  }
            set { m_status = value; }
        }
        #endregion
    }
}
