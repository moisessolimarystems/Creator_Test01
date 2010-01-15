using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Dynamic;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;
using Solimar.Licensing.Attribs;
using System.Configuration;
using System.ComponentModel;
using CreatorData;
using Solimar.Licensing.LicenseManagerWrapper;
using Client.Creator;

namespace Service.Creator
{
    // NOTE: If you change the class name "Service1" here, you must also update the reference to "Service1" in App.config.
    [ServiceBehavior(ConcurrencyMode = ConcurrencyMode.Reentrant
            , InstanceContextMode = InstanceContextMode.Single)]
    public class Creator : ICreator
    {
        private static readonly IDictionary<string, string>
            _filterLSNames = new Dictionary<string, string>
                {            
                    {"Customer", "CustomerTable.SCRname"},
                    {"LicenseServer", "LicenseName"},
                    {"Active", "IsActive"}
                };

        private static readonly IDictionary<string, string>
            _filterHTNames = new Dictionary<string, string>
                {            
                    {"Customer", "CustID"},
                    {"HardwareID", "ID"},
                    {"LicenseServer", "LicenseTable.LicenseName"},
                    {"State", "TokenStatus"},
                    {"ActivatedDate", "ActivatedDate"},
                    {"DeactivatedDate", "DeactivatedDate"}
                };

        private static readonly IDictionary<string, string>
            _filterPLNames = new Dictionary<string, string>
                {            
                    {"Customer", "LicenseTable.CustomerTable.SCRname"},
                    {"ProductLicense", "plID"},
                    {"ExpirationDate", "ExpirationDate"},
                    {"Product", "ProductID"},
                    {"ProductVersion", "ProductVersion"},
                    {"State", "plState"},
                    {"Active", "IsActive"},
                    {"Extension","Extensions"},
                    {"Activation","Activations"},
                    {"ActivationAmount","ActivationAmount"}
                };

        private static readonly IDictionary<string, string>
            _filterOperators = new Dictionary<string, string>
                {
                    {"Equal", "="},
                    {"NotEqual", "!="},
                    {"LessThan", "<"},
                    {"GreaterThan", ">"},
                    {"Contains", "Contains"},
                };
        public static SolimarLicenseServerWrapper m_licServer = new SolimarLicenseServerWrapper();
        public static CommunicationLink m_CommLink = new CommunicationLink();

        #region ICreator Members

        #region Customer Implementation
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<CustomerTable> GetAllCustomers(string searchStr, bool enableLoadOptions)
        {
            return CustomerTable.GetAllCustomers(searchStr, enableLoadOptions);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<string> GetAllCustomerNames(string searchStr)
        {
            return CustomerTable.GetAllCustomerNames(searchStr);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public int GetCustomerCount(string searchStr, bool enableLoadOptions)
        {
            return CustomerTable.GetCustomerCount(searchStr, enableLoadOptions);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public CustomerTable GetCustomer(string searchStr, bool enableLoadOptions)
        {
            return CustomerTable.GetCustomer(searchStr, enableLoadOptions);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void CreateCustomer(CustomerTable ct)
        {
            CustomerTable.CreateCustomer(ct);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void UpdateCustomer(CustomerTable ct)
        {
            CustomerTable.UpdateCustomer(ct);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void DeleteCustomer(CustomerTable ct)
        {
            CustomerTable.DeleteCustomer(ct);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public int GetLastCustomerID()
        {
            return CustomerTable.GetLastCustomerID();
        }
        #endregion

        #region License Implementation
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<LicenseTable> GetLicensesByDestination(string custName, string destName, string findStr, bool bLoadOptions)
        {
            return LicenseTable.GetLicensesByDestination(custName, destName, findStr, bLoadOptions);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<LicenseTable> GetLicensesByCustomer(string custName)
        {
            return LicenseTable.GetLicensesByCustomer(custName);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public LicenseTable GetLicenseByName(string licName, bool bLoadOptions)
        {
            return LicenseTable.GetLicenseByName(licName, bLoadOptions);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public LicenseTable GetLicenseByID(int licID, bool bLoadOptions)
        {
            return LicenseTable.GetLicenseByID(licID, bLoadOptions);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public int GetLicenseCountByID(int custID, int destID, int groupID)
        {
            return LicenseTable.GetLicenseCountByID(custID, destID, groupID);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public uint GetNextGroupID(uint custID, uint destID)
        {
            return LicenseTable.GetNextGroupID(custID, destID);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public uint GetLastGroupID(uint custID, uint destID)
        {
            return LicenseTable.GetLastGroupID(custID, destID);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public uint GetLastDestinationID(uint custID)
        {
            return LicenseTable.GetLastDestinationID(custID);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public bool IsLicenseModified(string licName) 
        {
            return LicenseTable.IsLicenseModified(licName);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<string> GetModifiedLicensesByCustomer(string custName)
        {
            return LicenseTable.GetModifiedLicenseNamesByCustomer(custName);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public int GetLicenseCountByDestName(uint custID, uint destID)
        {
            return LicenseTable.GetLicenseCountByDestName(custID, destID);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<LicenseTable> GetLicensesByConditions(IList<Condition> cl)
        {
            String value;
            int result;
            DateTime date;
            StringBuilder conditionString = new StringBuilder();
            foreach (Condition userCondition in cl)
            {
                if (conditionString.Length != 0)
                    conditionString.Append(" and ");
                //create a condition string from condition list using dictionary to translate condition to conditionstring
                conditionString.Append(_filterLSNames[userCondition.Name.ToString()]);
                value = userCondition.Value;
                if (DateTime.TryParse(value, out date))
                {
                    value = string.Format("DateTime({0},{1},{2})", date.Year, date.Month, date.Day);
                }
                if (_filterOperators[userCondition.Operator.ToString()] == "Contains")
                {
                    conditionString.Append(".").Append(_filterOperators[userCondition.Operator.ToString()]).Append("(\"").Append(value).Append("\")");
                }
                else
                {
                    if (!Int32.TryParse(value, out result))
                    {
                        value = "\"" + value + "\"";
                    }
                    conditionString.Append(_filterOperators[userCondition.Operator.ToString()]);
                    conditionString.Append(value);
                }                
            }
            return LicenseTable.GetLicensesByConditions(conditionString.ToString());
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void GenerateLicensePacket(string pktName, LicenseTable lt, DateTime expDate, string comments, ref string vCode, ref Byte[] newByteArrayLicensePacket, string user)
        {
            //convert licinfo into a string to store into DB and to pass into license server
            //save verification code
            //return file back to client
            //if (m_licServer == null)
            //{
            //    m_licServer = new SolimarLicenseServerWrapper();
            m_licServer.Connect("localhost");
            //}
            //LicenseTable licRecord = LicenseTable.GetLicenseByName(licenseName, false);
            m_licServer.GenerateSoftwareLicPacket(lt.LicenseInfo, expDate, ref vCode, ref newByteArrayLicensePacket);
            PacketTable newPacket = new PacketTable()
            {
                PacketName = pktName,
                LicenseID = lt.ID,
                PacketVerificationCode = vCode,
                DateCreated = DateTime.Now,
                ExpiredDate = expDate,
                IsVerified = false,
                PacketComments = comments,
                UserName = user
                //want to attach all changes to transaction.
            };   
            CreatePacket(newPacket);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<SolimarLicenseProtectionKeyInfo> KeyEnumerate()
        {
            m_licServer.Connect("localhost");
            return m_licServer.KeyEnumerate();
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public string KeyFormat(string key)
        {
            m_licServer.Connect("localhost");
            return m_licServer.KeyFormat(key);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public string KeyProgramVerification(string key, int customerNumber, int keyNumber)
        {
            m_licServer.Connect("localhost");
            return m_licServer.KeyProgramVerification(key, customerNumber, keyNumber);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void CreateLicense(LicenseTable lt)
        {
            LicenseTable.CreateLicense(lt);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void UpdateLicense(LicenseTable lt, bool isDirty)
        {
            lt.IsDirty = isDirty;
            LicenseTable.UpdateLicense(lt);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void DeleteLicense(LicenseTable lt)
        {
            LicenseTable.DeleteLicense(lt);
        }
        #endregion

        #region Packet Implementation
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<PacketTable> GetPacketsByLicenseName(string licName)
        {
            return PacketTable.GetPacketsByLicenseName(licName);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public PacketTable GetPacketByVerificationCode(string vCode)
        {
            return PacketTable.GetPacketByVerificationCode(vCode);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public bool ValidateVerificationCode(string pktName, string vCode)
        {
            return PacketTable.ValidateVerificationCode(pktName, vCode);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public string GenerateStreamByLicenseSystemData(ref Byte[] byteVerifyData)
        {
            m_licServer.Connect("localhost");
            return m_licServer.GenerateStream_ByLicenseSystemData(byteVerifyData);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void CreatePacket(PacketTable pt)
        {
            PacketTable.CreatePacket(pt);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void UpdatePacket(PacketTable pt)
        {
            PacketTable.UpdatePacket(pt);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void DeletePacket(PacketTable pt)
        {
            PacketTable.DeletePacket(pt);
        }
        #endregion

        #region Transaction Implementation 
        //[OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        //public IList<TransactionTable> GetTransactionsByLicenseName(string licenseName)
        //{
        //    return TransactionTable.GetTransactionsByLicense(licenseName);
        //}
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<TransactionTable> GetTransactionsByPacketID(int pktID)
        {
            return TransactionTable.GetTransactionsByPacketID(pktID);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<TransactionTable> GetNewTransactionsByLicenseName(string licName)
        {
            return TransactionTable.GetNewTransactionsByLicenseName(licName);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void UpdateSubLicenseTransactionsByOrder(int stdLicOrderID, int subLicOrderID)
        {
            TransactionTable.UpdateSubLicenseTransactionsByOrder(stdLicOrderID, subLicOrderID);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void CreateTransaction(TransactionTable tt)
        {
            TransactionTable.CreateTransaction(tt);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void UpdateTransaction(TransactionTable tt)
        {
            TransactionTable.UpdateTransaction(tt);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void DeleteTransaction(TransactionTable tt)
        {
            TransactionTable.DeleteTransaction(tt);
        }

        #endregion

        #region Product License Implementation

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<ProductLicenseTable> GetProductLicensesByConditions(IList<Condition> cl)
        {
            String value;
            int result;
            DateTime date;
            StringBuilder conditionString = new StringBuilder();
            
            foreach (Condition userCondition in cl)
            {
                if (conditionString.Length != 0)
                    conditionString.Append(" and ");
                //create a condition string from condition list using dictionary to translate condition to conditionstring
                conditionString.Append(_filterPLNames[userCondition.Name.ToString()]);
                value = userCondition.Value;
                if (DateTime.TryParse(value, out date))
                {
                    value = string.Format("DateTime({0},{1},{2},{3},{4},{5})", date.Year, date.Month, date.Day, 18,0,0);
                }
                if (_filterOperators[userCondition.Operator.ToString()] == "Contains")
                {
                    conditionString.Append(".").Append(_filterOperators[userCondition.Operator.ToString()]).Append("(\"").Append(value).Append("\")");
                }
                else
                {
                    if(!Int32.TryParse(value, out result))
                    {
                        if(!value.Contains("DateTime"))
                            value = "\"" + value + "\"";
                    }
                    conditionString.Append(_filterOperators[userCondition.Operator.ToString()]);
                    conditionString.Append(value);
                }
            }
            return ProductLicenseTable.GetProductLicensesByConditions(conditionString.ToString());
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<ProductLicenseTable> GetAllProductLicenses()
        {
            return ProductLicenseTable.GetAllProductLicenses();
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<ProductLicenseTable> GetProductLicenses(string lsID)
        {
            return ProductLicenseTable.GetProductLicenses(lsID);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<ProductLicenseTable> GetProductLicensesByProduct(string lsID, int prodID)
        {
            return ProductLicenseTable.GetProductLicenses(lsID, prodID);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public ProductLicenseTable GetProductLicense(string plID)
        {
            return ProductLicenseTable.GetProductLicense(plID);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public ProductLicenseTable GetProductLicenseByID(int productLicenseID)
        {
            return ProductLicenseTable.GetProductLicenseByID(productLicenseID);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<string> GetAddOnProductLicenses(string plID)
        {
            return ProductLicenseTable.GetAddOnProductLicenses(plID);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<string> GetDeactivatedProductLicenses(string lsID)
        {
            return ProductLicenseTable.GetDeactivateProductLicenses(lsID);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public int GetNextProductLicenseIndex(string lsID)
        {
            return ProductLicenseTable.GetNextProductLicenseIndex(lsID);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public int GetProductVersionFromTable(int prodID)
        {
            return ProductLicenseTable.GetProductVersionFromTable(prodID);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void DeactivateProductLicense(string plID)
        {
            ProductLicenseTable.DeactivateProductLicense(plID);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void DeleteAllProductLicenses(string ls)
        {
            ProductLicenseTable.DeleteAllProductLicenses(ls);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void CreateProductLicense(ProductLicenseTable plt)
        {
            ProductLicenseTable.CreateProductLicense(plt);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void UpdateProductLicense(ProductLicenseTable plt)
        {
            ProductLicenseTable.UpdateProductLicense(plt);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void DeleteProductLicense(ProductLicenseTable plt)
        {
            ProductLicenseTable.DeleteProductLicense(plt);
        }
        #endregion

        #region Token Implementation
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<TokenTable> GetHardwareTokensByConditions(IList<Condition> cl)
        {
            String value;
            int result;
            DateTime date;
            StringBuilder conditionString = new StringBuilder();
            foreach (Condition userCondition in cl)
            {
                if (conditionString.Length != 0)
                    conditionString.Append(" and ");
                //create a condition string from condition list using dictionary to translate condition to conditionstring
                conditionString.Append(_filterHTNames[userCondition.Name.ToString()]);
                value = userCondition.Value;
                if (DateTime.TryParse(value, out date))
                {
                    value = string.Format("DateTime({0},{1},{2},{3},{4},{5})", date.Year, date.Month, date.Day, 18, 0, 0);
                }
                if (_filterOperators[userCondition.Operator.ToString()] == "Contains")
                {
                    conditionString.Append(".").Append(_filterOperators[userCondition.Operator.ToString()]).Append("(\"").Append(value).Append("\")");
                }
                else
                {                  
                    if (!Int32.TryParse(value, out result))
                    {
                        if (!value.Contains("DateTime"))
                            value = "\"" + value + "\"";
                    }
                    conditionString.Append(_filterOperators[userCondition.Operator.ToString()]);
                    conditionString.Append(value);
                }
            }
            return TokenTable.GetHardwareTokensByConditions(conditionString.ToString());
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<TokenTable> GetAllTokens(string searchString, Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType token)
        {
            return TokenTable.GetAllTokens(searchString, (byte)token);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<TokenTable> GetAllTokensByCustomer(string searchString, string custName, Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType token)
        {
            return TokenTable.GetAllTokensByCustomer(searchString, custName,(byte)token);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<TokenTable> GetAvailableHardwareTokensByCustID(uint custID)
        {
            return TokenTable.GetAvailableHardwareTokensByCustID(custID);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public TokenTable GetHardwareTokenByKeyValue(string keyValue)
        {
            return TokenTable.GetHardwareTokenByKeyValue(keyValue);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<TokenTable> GetTokensByLicenseName(string licName)
        {
            return TokenTable.GetTokensByLicenseName(licName);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public TokenTable GetTokenByLicenseName(string licName, byte tokenType)
        {
            return TokenTable.GetTokenByLicenseName(licName, tokenType);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public bool TokenExists(uint custID, byte tokenType, string tokenValue)
        {
            return TokenTable.TokenExists(custID, tokenType, tokenValue);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public bool IsHardwareTokenActive(uint custID, string tokenValue)
        {
            return TokenTable.IsHardwareTokenActive(custID, tokenValue);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public uint GetNextHardwareTokenValue(uint custID)
        {
            return TokenTable.GetNextHardwareTokenValue(custID);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void CreateToken(TokenTable tt)
        {
            TokenTable.CreateToken(tt);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void UpdateToken(TokenTable tt)
        {
            TokenTable.UpdateToken(tt);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void DeleteToken(TokenTable tt)
        {
            TokenTable.DeleteToken(tt);
        }
        #endregion

        #region Permission Implementation
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public PermissionsTable GetPermissionsByUser(string user)
        {
            return PermissionsTable.GetPermissionByUser(user);
        }

        #endregion

        #region DestinationName Implementation
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<DestinationNameTable> GetDestNamesByCustID(int custID)
        {
            return DestinationNameTable.GetDestNamesByCustID(custID);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public DestinationNameTable GetDestinationName(int custID, int destID)
        {
            return DestinationNameTable.GetDestinationName(custID, destID);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public DestinationNameTable GetDestinationID(int custID, string dn)
        {
            return DestinationNameTable.GetDestinationID(custID, dn);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public uint GetNextDestinationID(uint custID)
        {
            return DestinationNameTable.GetNextDestinationID(custID);
        }
        
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void DeleteAllDestinationNames(int custID)
        {
            DestinationNameTable.DeleteAllDestinationNamesByCustomer(custID);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void CreateDestinationName(DestinationNameTable dnt)
        {
            DestinationNameTable.CreateDestinationName(dnt);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void UpdateDestinationName(DestinationNameTable dnt)
        {
            DestinationNameTable.UpdateDestinationName(dnt);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void DeleteDestinationName(DestinationNameTable dnt)
        {
            DestinationNameTable.DeleteDestinationName(dnt);
        }
        #endregion

        #region ProductTable Implementation
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<ProductTable> GetProducts()
        {
            return ProductTable.GetAllProducts();
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public ProductTable GetProduct(string product)
        {
            return ProductTable.GetProduct(product);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void UpdateProductTable(ProductTable pt)
        {
            ProductTable.UpdateProductTable(pt);
        }
        #endregion

        #region SoftwareTokenTable Implementation
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void UpdateSoftwareTokenTable(SoftwareTokenTable stt)
        {
            SoftwareTokenTable.UpdateProductTable(stt);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public SoftwareTokenTable GetSoftwareToken(string tokenType)
        {
            return SoftwareTokenTable.GetSoftwareToken(tokenType);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<SoftwareTokenTable> GetAllSoftwareTokens()
        {
            return SoftwareTokenTable.GetAllSoftwareTokens();
        }
        #endregion

        #region Module Implementation
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void CreateModule(ModuleTable module)
        { ModuleTable.CreateModule(module); }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void UpdateModule(ModuleTable module)
        { ModuleTable.UpdateModule(module); }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void DeleteModule(ModuleTable module)
        { ModuleTable.DeleteModule(module); }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void CreateAllModules(IList<ModuleTable> modules)
        { ModuleTable.CreateAllModules(modules); }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void UpdateAllModules(IList<ModuleTable> modules)
        { ModuleTable.UpdateAllModules(modules); }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void DeleteAllModules(int productlicenseID)
        { ModuleTable.DeleteAllModules(productlicenseID); }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public ModuleTable GetModule(string productLicenseName, int modID)
        { return ModuleTable.GetModule(productLicenseName, modID); }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<ModuleTable> GetAllModules(string productLicenseName)
        { return ModuleTable.GetAllModules(productLicenseName); }
        #endregion
        #endregion
    }
}
