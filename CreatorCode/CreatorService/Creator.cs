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

namespace Service.Creator
{
    // NOTE: If you change the class name "Service1" here, you must also update the reference to "Service1" in App.config.
    [ServiceBehavior(ConcurrencyMode = ConcurrencyMode.Reentrant
            , InstanceContextMode = InstanceContextMode.Single)]
    public class Creator : ICreator
    {
        private static readonly IDictionary<string, string>
            _filterNames = new Dictionary<string, string>
                {
                    {"Customer", "SCustomerRecord.SCRname"},
                    {"DestinationID", "DestinationID"},
                    {"GroupID", "GroupID"},
                    {"LicenseType", "LicenseType"}
                };

        private static readonly IDictionary<string, string>
            _filterOperators = new Dictionary<string, string>
                {
                    {"Equals", "="},
                    {"LessThan", "<"},
                    {"GreaterThan", ">"},
                    {"Contains", "Contains"}
                };

        public static SolimarLicenseServerWrapper m_licServer = new SolimarLicenseServerWrapper();

        #region ICreator Members

        #region Customer Implementation
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<CustomerTable> GetAllCustomers(string searchString, bool enableLoadOptions)
        {
            return CustomerTable.GetAllCustomers(searchString, enableLoadOptions);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<string> GetAllCustomerNames(string searchString)
        {
            return CustomerTable.GetAllCustomerNames(searchString);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public int GetCustomerCount(string searchString, bool enableLoadOptions)
        {
            return CustomerTable.GetCustomerCount(searchString, enableLoadOptions);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public CustomerTable GetCustomer(string searchString, bool enableLoadOptions)
        {
            return CustomerTable.GetCustomer(searchString, enableLoadOptions);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void CreateCustomer(CustomerTable custRecord)
        {
            CustomerTable.CreateCustomer(custRecord);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void UpdateCustomer(CustomerTable custRecord)
        {
            CustomerTable.UpdateCustomer(custRecord);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void DeleteCustomer(CustomerTable custRecord)
        {
            CustomerTable.DeleteCustomer(custRecord);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public int GetLastCustomerID()
        {
            return CustomerTable.GetLastCustomerID();
        }
        #endregion

        #region License Implementation
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<LicenseTable> GetAllLicenses(string searchString, bool enableLoadOptions)
        {
            return LicenseTable.GetAllLicenses(searchString, enableLoadOptions);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<LicenseTable> GetLicensesByID(int custID, int destID, int groupID, bool enableLoadOptions)
        {
            return LicenseTable.GetLicensesByID(custID, destID, groupID, enableLoadOptions);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public LicenseTable GetLicenseByName(string licenseName, bool enableLoadOptions)
        {
            return LicenseTable.GetLicenseByName(licenseName, enableLoadOptions);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public LicenseTable GetLicenseByID(int licID, bool enableLoadOptions)
        {
            return LicenseTable.GetLicenseByID(licID, enableLoadOptions);
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
        public bool IsLicenseModified(string licenseName) //int custID, int destID, int groupID, Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType licenseType)
        {
            return LicenseTable.IsLicenseModified(licenseName); //custID, destID, groupID, (Byte)licenseType);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public bool IsLicenseUpdated(string licenseName)
        {
            return LicenseTable.IsLicenseUpdated(licenseName); 
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<string> GetModifiedLicensesByCustomer(string custName)
        {
            return LicenseTable.GetModifiedLicenseNamesByCustomer(custName);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<string> GetUpdatedLicensesByCustomer(string custName)
        {
            return LicenseTable.GetUpdatedLicensesByCustomer(custName);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public int GetLicenseCountByType(uint custID, uint destID, uint groupID, Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType licType)
        {
            //TODO : try not to cast. Performance issues and might truncate and bad practice
            return LicenseTable.GetLicCountByType(custID, destID, groupID, (Byte)licType);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public int GetDerivedLicenseCount(uint custID, uint destID, uint groupID, Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType licType)
        {
            return LicenseTable.GetDerivedLicenseCount(custID, destID, groupID, (Byte)licType);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public int GetLicenseCountByDestName(uint custID, uint destID)
        {
            return LicenseTable.GetLicenseCountByDestName(custID, destID);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<LicenseTable> GetLicencesByConditions(IList<Condition> conditionList)
        {
            String value;
            int result;
            StringBuilder conditionString = new StringBuilder();
            foreach (Condition userCondition in conditionList)
            {
                //create a condition string from condition list using dictionary to translate condition to conditionstring
                conditionString.Append(_filterNames[userCondition.ConditionName.ToString()]);
                conditionString.Append(_filterOperators[userCondition.ConditionOperator.ToString()]);
                value = userCondition.ConditionValue;
                if (Int32.TryParse(value, out result))
                {
                    conditionString.Append(value);
                }
                else
                {
                    conditionString.Append("\"");
                    conditionString.Append(userCondition.ConditionValue.ToString());
                    conditionString.Append("\"");
                }
            }
            return LicenseTable.GetLicensesByConditions(conditionString.ToString());                        
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void GenerateLicensePacket(string packetName, string licenseName, DateTime expDate, string comments, ref string verificationCode, ref Byte[] newByteArrayLicensePacket, string user)
        {
            //convert licinfo into a string to store into DB and to pass into license server
            //save verification code
            //return file back to client
            //if (m_licServer == null)
            //{
            //    m_licServer = new SolimarLicenseServerWrapper();
                m_licServer.Connect("localhost");
            //}
            LicenseTable licRecord = LicenseTable.GetLicenseByName(licenseName, false);
            m_licServer.GenerateSoftwareLicPacket(licRecord.LicenseInfo, expDate, ref verificationCode, ref newByteArrayLicensePacket);
            PacketTable newPacket = new PacketTable()
            {
                PacketName = packetName,
                LicenseID = licRecord.ID,
                PacketVerificationCode = verificationCode,
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
        public void CreateLicense(LicenseTable license)
        {
            LicenseTable.CreateLicense(license);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void UpdateLicense(LicenseTable license)
        {
            LicenseTable.UpdateLicense(license);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void DeleteLicense(LicenseTable license)
        {
            LicenseTable.DeleteLicense(license);
        }
        #endregion

        #region Packet Implementation
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<PacketTable> GetPacketsByLicenseName(string licenseName)
        {
            return PacketTable.GetPacketsByLicenseName(licenseName);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public PacketTable GetPacketByVerificationCode(string verificationCode)
        {
            return PacketTable.GetPacketByVerificationCode(verificationCode);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public bool ValidateVerificationCode(string packetName, string verificationCode)
        {
            return PacketTable.ValidateVerificationCode(packetName, verificationCode);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public string GenerateLicInfoByVerifyData(ref Byte[] byteVerifyData)
        {
            m_licServer.Connect("localhost");
            return m_licServer.GenerateLicInfo_ByVerifyData(byteVerifyData);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void CreatePacket(PacketTable packet)
        {
            PacketTable.CreatePacket(packet);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void UpdatePacket(PacketTable packet)
        {
            PacketTable.UpdatePacket(packet);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void DeletePacket(PacketTable packet)
        {
            PacketTable.DeletePacket(packet);
        }
        #endregion

        #region Transaction Implementation 
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<TransactionTable> GetTransactionsByPacketID(int packetID)
        {
            return TransactionTable.GetTransactionsByPacketID(packetID);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<TransactionTable> GetNewTransactionsByLicenseName(string licenseName)
        {
            return TransactionTable.GetNewTransactionsByLicenseName(licenseName);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void UpdateSubLicenseTransactionsByOrder(int stdLicOrderID, int subLicOrderID)
        {
            TransactionTable.UpdateSubLicenseTransactionsByOrder(stdLicOrderID, subLicOrderID);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void CreateTransaction(TransactionTable ta)
        {
            TransactionTable.CreateTransaction(ta);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void UpdateTransaction(TransactionTable ta)
        {
            TransactionTable.UpdateTransaction(ta);
        }
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void DeleteTransaction(TransactionTable ta)
        {
            TransactionTable.DeleteTransaction(ta);
        }

        #endregion

        #region Order Implementation
        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<OrderTable> GetAllOrders()
        {
            return OrderTable.GetAllOrders();
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<OrderTable> GetOrdersByLicenseName(string licenseName)
        {
            return OrderTable.GetOrdersByLicenseName(licenseName);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<OrderTable> GetOrdersEqualsProduct(string licenseName, string productName)
        {
            return OrderTable.GetOrdersEqualsProduct(licenseName, productName);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public OrderTable GetOrderByOrderName(string orderName)
        {
            return OrderTable.GetOrderByOrderName(orderName);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public IList<string> GetAddOnOrdersByStandardOrder(string standardOrder)
        {
            return OrderTable.GetAddOnOrdersByStandardOrder(standardOrder);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public int GetNextOrderIndex(string licenseName)
        {
            return OrderTable.GetNextOrderIndex(licenseName);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public int GetProductVersionFromTable(int productID)
        {
            return OrderTable.GetProductVersionFromTable(productID);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void CreateOrder(OrderTable contract)
        {
            OrderTable.CreateOrder(contract);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void UpdateOrder(OrderTable contract)
        {
            OrderTable.UpdateOrder(contract);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void DeleteOrder(OrderTable contract)
        {
            OrderTable.DeleteOrder(contract);
        }
        #endregion

        #region Token Implementation
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
        public IList<TokenTable> GetTokensByLicenseName(string licenseName)
        {
            return TokenTable.GetTokensByLicenseName(licenseName);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public TokenTable GetTokenByLicenseName(string licenseName, byte tokenType)
        {
            return TokenTable.GetTokenByLicenseName(licenseName, tokenType);
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
        public void CreateToken(TokenTable ta)
        {
            TokenTable.CreateToken(ta);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void UpdateToken(TokenTable ta)
        {
            TokenTable.UpdateToken(ta);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void DeleteToken(TokenTable ta)
        {
            TokenTable.DeleteToken(ta);
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
        public DestinationNameTable GetDestinationID(int custID, string destName)
        {
            return DestinationNameTable.GetDestinationID(custID, destName);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public uint GetNextDestinationID(uint custID)
        {
            return DestinationNameTable.GetNextDestinationID(custID);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void CreateDestinationName(DestinationNameTable destName)
        {
            DestinationNameTable.CreateDestinationName(destName);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void UpdateDestinationName(DestinationNameTable destName)
        {
            DestinationNameTable.UpdateDestinationName(destName);
        }

        [OperationBehavior(Impersonation = ImpersonationOption.NotAllowed)]
        public void DeleteDestinationName(DestinationNameTable destName)
        {
            DestinationNameTable.DeleteDestinationName(destName);
        }
        #endregion

        #endregion
    }
}
