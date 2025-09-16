// LicensePacket.cs
//
// SLB 15.sep.2025 CR.34456; Changes for new attribs code (Licensing 3.4+) to work.
// Renamed member variables to have m_ prefix vs. _. 

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Client.Creator.CreatorService;
using Solimar.Licensing.Attribs;
using Client.Creator.ServiceProxy;

namespace Client.Creator
{
    class LicensePacket
    {
        #region Fields
        private Lic_PackageAttribs m_licPackage;                                    // CR.34456; modified.
		private LicenseTable m_licenseTable;										// CR.34456; modified.
		private string m_licenseServer;                                             // CR.34456; modified.

		//set by determining from the included products in a packet.
		private uint m_highestModuleRevision;										// CR.34456; modified.   
		CommunicationLink m_commLink;                                               // CR.34456; modified.
		#endregion

		#region Constructor
		/// <summary>
		/// Initializes a new instance of the LicensePacket class.
		/// </summary>
		/// <param name="licenseServer">The license server name that belongs to the license packet.</param>
		public LicensePacket(CommunicationLink commLink, string licenseServer)        
        {            
            m_licPackage = new Lic_PackageAttribs();
            m_licenseTable = null;
            m_licenseServer = licenseServer;
            m_highestModuleRevision = 0;
            m_commLink = commLink;
        }
        #endregion 

        #region Properties
        /// <summary>
        /// Gets the License Package object containing information to store in a license packet.
        /// </summary>
        public Lic_PackageAttribs LicPackage
        {
            get { return m_licPackage; }
        }
        /// <summary>
        /// Gets the license table containing database information on a license server.
        /// </summary>
        public LicenseTable DatabaseRecord
        {
            get { return m_licenseTable; }
        }
        #endregion

        #region Methods
        /// <summary>
        /// Builds a license package object by retrieving information from the database. Stores resulting object into _licenseTable[FIELD].
        /// </summary>
        public bool BuildLicensePackage(PacketDialogData data)
        {
            bool bRetVal = false;
            int major = 0, minor = 0, buildVersion = 0;
            Service<ICreator>.Use((client) =>
            {                
                m_licenseTable = client.GetLicenseByName(m_licenseServer, true);
                client.GetLicenseServerVersion(ref major, ref minor, ref buildVersion);
            });
            if (m_licenseTable != null)
            {
                m_licPackage.Stream = m_licenseTable.LicenseInfo;
                if (PopulateProductInfo())  //add products, productlicenses, modules to _licPackage                
                {                    
                    if (PopulateValidationTokens(data)) //add tokens to _licPackage 
                    {   //set software spec version based on the spec built with Creator
                        Lic_PackageAttribs.Lic_LicenseInfoAttribs licInfo = m_licPackage.licLicenseInfoAttribs;
                        m_licPackage.licReplaceSoftwareSpecAttribs = m_commLink.SoftwareSpec; // CR.34456; modified.
                        //add current software spec
                        Lic_LicenseInfoAttribsHelper.GenerateActivitySlotHistoryInfo(ref licInfo);
                        m_licPackage.licLicenseInfoAttribs = licInfo;                // CR.34456; modified.
						m_licPackage.licSoftwareSpecAttribs.softwareSpec_Major.TVal = (uint)major; // CR.34456; modified.
						m_licPackage.licSoftwareSpecAttribs.softwareSpec_Minor.TVal = (uint)minor; // CR.34456; modified.
						m_licPackage.licSoftwareSpecAttribs.softwareSpec_SubMajor.TVal = (uint)buildVersion; // CR.34456; modified.
						m_licPackage.licSoftwareSpecAttribs.softwareSpec_SubMinor.TVal = m_highestModuleRevision;   //value determined from included products highest revision
                        m_licenseTable.LicenseInfo = m_licPackage.Stream;
                        bRetVal = true;
                    }
                }
            }
            return bRetVal;
        }
        /// <summary>
        /// Saves the resulting _licPackage object from BuildLicensePackage into the database.
        /// </summary>
        public void SaveLicensePackage()
        {
            Lic_PackageAttribs.Lic_LicenseInfoAttribs licInfo = LicPackage.licLicenseInfoAttribs;
            licInfo.licVerificationAttribs.validationTokenList.Clear();				// CR.34456; modified.
			licInfo.productList.Clear();											// CR.34456; modified.
			LicPackage.licSoftwareSpecAttribs = new Lic_PackageAttribs.Lic_SoftwareSpecAttribs();  // CR.34456; modified.
			Service<ICreator>.Use((client) =>
            {
                m_licenseTable.LicenseInfo = LicPackage.Stream;
                m_licenseTable.IsDirty = false;
                client.UpdateLicense(m_licenseTable);
            });            
        }

        /// <summary>
        /// Retrieves all validation tokens in the database for a license server and stores them the _licPackage object with the tokens.
        /// 2 cases - active, deactive ls
        /// active ls - include all active tokens, can clear licenseCode
        /// deactive ls - include computer name with sw tokens, include nothing for hardware token     
        /// </summary>
        private bool PopulateValidationTokens(PacketDialogData data)
        {
            bool bRetVal = true;
            IList<SoftwareTokenTable> softwareTokens = null;
            IList<TokenTable> tokenList = null;            
            m_licPackage.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList.Clear(); // CR.34456; modified.

			Service<ICreator>.Use((client) =>
            {
                softwareTokens = CreatorForm.s_AllSoftwareTokens;
                tokenList = client.GetTokensByLicenseName(m_licenseServer).Where(t => t.TokenStatus != (byte)TokenStatus.Deactivated).ToList();
            });       
            if (m_licenseTable.IsActive)
            {
                bool bExistLicenseCode = false;
                foreach (TokenTable token in tokenList)  //add validation tokens for licinfo object, tokentable may contain licensecode
                {
                    if (tokenList.Count > 1)
                    { 
                        string tokenName = Enum.GetName(typeof(Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType), token.TokenType);
                        SoftwareTokenTable st = softwareTokens.FirstOrDefault(t => t.TokenType == tokenName);
                        if (st != null)
                        {
                            if (st.Status == 0)
                                continue;
                        }
                        else  //not found in software token table - ttlicensecode
                        {
                            if(token.TokenType == (byte)Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttLicenseCode)
                            {
                                if (!data.ClearLicenseCode)
                                    continue; //skip licenseCode token ClearLicenseCode selected
                                bExistLicenseCode = true;
                            }
                        }
                    }
                    Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs newToken = new Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs();
                    newToken.tokenType.TVal = (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType)token.TokenType;
                    newToken.tokenValue.TVal = token.TokenValue;
                    m_licPackage.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList.Add(newToken); // CR.34456; modified.
				}
                if (!bExistLicenseCode)  //add empty licensecode token if it hasn't been added
                {
                    Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs licCodeToken = new Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs();
                    licCodeToken.tokenType.TVal = Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttLicenseCode;
                    m_licPackage.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList.Add(licCodeToken); // CR.34456; modified.
				}
            }
            else //deactive ls
            {   //SW will have history of tokens, HW will be empty                 
                TokenTable computerNameToken = tokenList.Where(t => t.TokenType == (byte)Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttComputerName &&
                                                                    t.TokenStatus != (byte)TokenStatus.Deactivated).FirstOrDefault();
                if (computerNameToken != null) //no need to add any tokens for HW deactivated LS
                {
                    Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs newToken = new Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs();
                    newToken.tokenType.TVal = (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType)computerNameToken.TokenType;
                    newToken.tokenValue.TVal = computerNameToken.TokenValue;
                    m_licPackage.licLicenseInfoAttribs.licVerificationAttribs.validationTokenList.Add(newToken); // CR.34456; modified.
				}
            }
            return bRetVal;
        }

        /// <summary>
        /// Retrieves all product licenses from the database along with their modules and stores them into the _licPackage object.
        /// </summary>
        private bool PopulateProductInfo()
        {
            bool bRetVal = false;
            uint currentModuleRevision = 0;
            Service<ICreator>.Use((client) =>
            {
                List<ProductLicenseTable> pltList = client.GetProductLicenses(m_licenseServer,false);
                List<TransactionTable> transactionList = client.GetNewTransactionsByLicenseName(m_licenseServer);
                List<ModuleTable> moduleList = client.GetAllModules(m_licenseServer);
                foreach (ProductLicenseTable plt in pltList.Where(pl => pl.IsActive == true))
                {
                    Lic_PackageAttribs.Lic_ProductInfoAttribs product = new Lic_PackageAttribs.Lic_ProductInfoAttribs();
                    product.contractNumber.TVal = plt.plID;
                    product.productID.TVal = plt.ProductID;
                    product.productState.TVal = (Lic_PackageAttribs.Lic_ProductInfoAttribs.TProductState)Enum.Parse(typeof(Lic_PackageAttribs.Lic_ProductInfoAttribs.TProductState), plt.plState.ToString());
                    product.productAppInstance.TVal = plt.ProductConnection;
                    string[] versionString = plt.ProductVersion.Split(".".ToCharArray());
                    if(versionString.Count() > 0)
                    {
                        uint value;
                        UInt32.TryParse(versionString[0], out value);
                        product.product_Major.TVal = value;
                        UInt32.TryParse(versionString[1], out value);
                        product.product_Minor.TVal = value;
                        product.product_SubMajor.TVal = 0;
                        product.product_SubMinor.TVal = 0;
                    }
                    product.expirationDate.TVal = (plt.ExpirationDate.HasValue) ? plt.ExpirationDate.Value : new DateTime(1900, 1, 1);
                    product.bUseExpirationDate.TVal = plt.ExpirationDate.HasValue;
                    product.activationTotal.TVal = plt.Activations;
                    product.activationAmountInDays.TVal = plt.ActivationAmount;
                    product.bUseActivations.TVal = (plt.Activations > 0);
                    product.bActivationOverrideCurrent.TVal = (transactionList.Where(t => t.taOrderID == plt.ID &&
                                                                                         (t.taType == (byte)TransactionType.ActivationAmount ||
                                                                                          t.taType == (byte)TransactionType.ActivationTotal ||
                                                                                          t.taType == (byte)TransactionType.ExpirationDate)).Count() > 0);
                    List<ModuleTable> currentModules = moduleList.Where(m => m.ProductLicenseID == plt.ID).ToList();                  
                    if (currentModules.Count > 0)
                    {
                        foreach (ModuleTable module in currentModules)
                        {                            
                            Lic_PackageAttribs.Lic_ModuleInfoAttribs mod = new Lic_PackageAttribs.Lic_ModuleInfoAttribs();
                            mod.moduleID.TVal = (uint)module.ModID;
                            mod.moduleValue.TVal = (uint)module.Value;
                            mod.moduleAppInstance.TVal = module.AppInstance;
                            if (module.Value > 0) //only modules with value count towards spec revision
                            {
								// get revision by setting to highest product revision while adding
								currentModuleRevision = CreatorForm.s_CommLink.GetModuleSpecRevision(plt.ProductID, (ushort)module.ModID);  
								m_highestModuleRevision = (currentModuleRevision > m_highestModuleRevision) ? currentModuleRevision : m_highestModuleRevision;
                            }
                            product.moduleList.Add(mod);							// CR.34456; modified.
						}
                    }
                    m_licPackage.licLicenseInfoAttribs.productList.Add(product);		// CR.34456; modified.
				}
                bRetVal = true;
            });
            return bRetVal;
        }
        #endregion 
    }
}
