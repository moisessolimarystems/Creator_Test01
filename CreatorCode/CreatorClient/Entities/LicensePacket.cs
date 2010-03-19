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
        private Lic_PackageAttribs _licPackage;
        private LicenseTable _licenseTable;
        private string _licenseServer;
        private uint _highestProductRevision;   //set by determining from the included products in a packet.

        public LicensePacket(string licenseServer)        
        {            
            _licPackage = new Lic_PackageAttribs();
            _licenseTable = null;
            _licenseServer = licenseServer;
            _highestProductRevision = 0;
        }

        public Lic_PackageAttribs LicPackage
        {
            get { return _licPackage; }
        }

        public LicenseTable DatabaseRecord
        {
            get { return _licenseTable; }
        }
        
        /// <summary>
        /// add tokens
        /// create products
        /// add modules to products
        /// </summary>
        public bool BuildLicensePackage()
        {
            bool bRetVal = false;
            int major = 0, minor = 0, buildVersion = 0;
            Service<ICreator>.Use((client) =>
            {
                _licenseTable = client.GetLicenseByName(_licenseServer, false);
                client.GetLicenseServerVersion(ref major, ref minor, ref buildVersion);
            });
            if (_licenseTable != null)
            {
                _licPackage.Stream = _licenseTable.LicenseInfo;
                if (PopulateValidationTokens())
                {
                    if (PopulateProductInfo())
                    {
                        Lic_PackageAttribs.Lic_LicenseInfoAttribs licInfo = _licPackage.licLicenseInfoAttribs;
                        Lic_LicenseInfoAttribsHelper.GenerateActivitySlotHistoryInfo(ref licInfo);
                        _licPackage.licLicenseInfoAttribs.TVal = licInfo;
                        _licPackage.licSoftwareSpecAttribs.TVal.softwareSpec_Major.TVal = (uint)major;
                        _licPackage.licSoftwareSpecAttribs.TVal.softwareSpec_Minor.TVal = (uint)minor;
                        _licPackage.licSoftwareSpecAttribs.TVal.softwareSpec_SubMajor.TVal = (uint)buildVersion;
                        _licPackage.licSoftwareSpecAttribs.TVal.softwareSpec_SubMinor.TVal = _highestProductRevision;//value determined from included products highest revision
                        _licenseTable.LicenseInfo = _licPackage.Stream;
                        bRetVal = true;
                    }
                }
            }
            return bRetVal;
        }

        public void SaveLicensePackage()
        {
            Lic_PackageAttribs.Lic_LicenseInfoAttribs licInfo = LicPackage.licLicenseInfoAttribs;
            licInfo.licVerificationAttribs.TVal.validationTokenList.TVal.Clear();
            licInfo.productList.TVal.Clear();
            Service<ICreator>.Use((client) =>
            {
                _licenseTable.LicenseInfo = LicPackage.Stream;
                client.UpdateLicense(_licenseTable, false);
            });            
        }

        private bool PopulateValidationTokens()
        {
            bool bRetVal = true;
            IList<SoftwareTokenTable> softwareTokens = null;
            IList<TokenTable> tokenList = null;
            Service<ICreator>.Use((client) =>
            {               
                softwareTokens = client.GetAllSoftwareTokens();
                tokenList = client.GetTokensByLicenseName(_licenseServer);
            });
            _licPackage.licLicenseInfoAttribs.TVal.licVerificationAttribs.TVal.validationTokenList.TVal.Clear();
            Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs licCodeToken = new Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs();
            licCodeToken.tokenType.TVal = Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttLicenseCode;
            _licPackage.licLicenseInfoAttribs.TVal.licVerificationAttribs.TVal.validationTokenList.TVal.Add(licCodeToken);            
            //add validation tokens for licinfo object
            foreach (TokenTable token in tokenList)
            {
                if (tokenList.Count > 1)
                {
                    string tokenName = Enum.GetName(typeof(Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType), token.TokenType);
                    SoftwareTokenTable st = softwareTokens.First(t => t.TokenType == tokenName);
                    if (st.Status == 0)
                        continue;
                }
                Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs newToken = new Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs();
                newToken.tokenType.TVal = (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType)token.TokenType;
                newToken.tokenValue.TVal = token.TokenValue;
                _licPackage.licLicenseInfoAttribs.TVal.licVerificationAttribs.TVal.validationTokenList.TVal.Add(newToken);
            }
            return bRetVal;
        }

        private bool PopulateProductInfo()
        {
            bool bRetVal = false;
            uint currentProductRevision;
            Service<ICreator>.Use((client) =>
            {
                List<ProductLicenseTable> pltList = client.GetProductLicenses(_licenseServer);
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
                    List<TransactionTable> transactionList = client.GetNewTransactionsByLicenseName(_licenseServer);
                    product.bActivationOverrideCurrent.TVal = (transactionList.Where(t => t.taOrderID == plt.ID &&
                                                                                         (t.taType == (byte)TransactionType.ActivationAmount ||
                                                                                          t.taType == (byte)TransactionType.ActivationTotal ||
                                                                                          t.taType == (byte)TransactionType.ExpirationDate)).Count() > 0);
                    List<ModuleTable> moduleList = client.GetModulesByProductLicense(plt.plID);
                    if (moduleList.Count > 0)
                    {
                        foreach (ModuleTable module in moduleList)
                        {
                            Lic_PackageAttribs.Lic_ModuleInfoAttribs mod = new Lic_PackageAttribs.Lic_ModuleInfoAttribs();
                            mod.moduleID.TVal = (uint)module.ModID;
                            mod.moduleValue.TVal = (uint)module.Value;
                            mod.moduleAppInstance.TVal = module.AppInstance;
                            product.moduleList.TVal.Add(mod);
                        }
                    }
                    _licPackage.licLicenseInfoAttribs.TVal.productList.TVal.Add(product);
                    //get revision by setting to highest product revision while adding
                    currentProductRevision = CreatorForm.s_CommLink.GetProductSpecRevision(plt.ProductID);
                    _highestProductRevision = (currentProductRevision > _highestProductRevision) ? currentProductRevision : _highestProductRevision;
                }
                bRetVal = true;
            });
            return bRetVal;
        }
    }
}
