using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;
using System.ComponentModel;
using CreatorData;
using Solimar.Licensing;
using Solimar.Licensing.Attribs;
using Solimar.Licensing.LicenseManagerWrapper;

namespace Service.Creator
{
    // NOTE: If you change the interface name "ICreator" here, you must also update the reference to "ICreator" in App.config.
    [ServiceContract]
    public interface ICreator
    {
        //CRUD operations - Create, Read, Update, Delete
       
        #region Customer OperationContracts
        // TODO: Add your service operations here
        [OperationContract]
        IList<CustomerTable> GetAllCustomers(string searchString, bool enableLoadOptions);

        [OperationContract]
        IList<string> GetAllCustomerNames(string searchString);

        [OperationContract]
        int GetCustomerCount(string searchString, bool enableLoadOptions);

        [OperationContract]
        CustomerTable GetCustomer(string searchString, bool enableLoadOptions);

        [OperationContract]
        void CreateCustomer(CustomerTable custRecord);

        [OperationContract]
        void UpdateCustomer(CustomerTable custRecord);

        [OperationContract]
        void DeleteCustomer(CustomerTable custRecord);

        [OperationContract]
        int GetLastCustomerID();

        #endregion

        #region License OperationContracts
        [OperationContract]
        IList<LicenseTable> GetAllLicenses(string searchString, bool enableLoadOptions);

        [OperationContract]
        IList<LicenseTable> GetLicensesByID(int custID, int destID, int groupID, bool enableLoadOptions);

        [OperationContract]
        LicenseTable GetLicenseByName(string licenseName, bool enableLoadOptions);

        [OperationContract]
        LicenseTable GetLicenseByID(int licID, bool enableLoadOptions);

        [OperationContract]
        int GetLicenseCountByID(int custID, int destID, int groupID);

        [OperationContract]
        uint GetNextGroupID(uint custID, uint destID);

        [OperationContract]
        uint GetLastGroupID(uint custID, uint destID);

        [OperationContract]
        int GetLicenseCountByDestName(uint custID, uint destID);

        [OperationContract]
        uint GetLastDestinationID(uint custID);

        [OperationContract]
        IList<LicenseTable> GetLicencesByConditions(IList<Condition> conditionList);

        [OperationContract]
        bool IsLicenseModified(string licenseName);

        [OperationContract]
        bool IsLicenseUpdated(string licenseName);

        [OperationContract]
        IList<string> GetModifiedLicensesByCustomer(string custName);

        [OperationContract]
        IList<string> GetUpdatedLicensesByCustomer(string custName); 

        [OperationContract]
        int GetLicenseCountByType(uint custID, uint destID, uint groupID, Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType licType);

        [OperationContract]
        int GetDerivedLicenseCount(uint custID, uint destID, uint groupID, Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType licType);

        [OperationContract]
        void GenerateLicensePacket(string packetName, string licenseName, DateTime expDate, string comments, ref string verificationCode, ref Byte[] newByteArrayLicensePacket, string user);

        [OperationContract]
        IList<SolimarLicenseProtectionKeyInfo> KeyEnumerate();

        [OperationContract]
        string KeyFormat(string key);

        [OperationContract]
        string KeyProgramVerification(string key, int customerNumber, int keyNumber);

        [OperationContract()]
        void CreateLicense(LicenseTable license);

        [OperationContract()]
        void UpdateLicense(LicenseTable license);

        [OperationContract()]
        void DeleteLicense(LicenseTable license);

        #endregion

        #region Packet OperationContracts

        [OperationContract]
        IList<PacketTable> GetPacketsByLicenseName(string licenseName);

        [OperationContract]
        PacketTable GetPacketByVerificationCode(string verificationCode);

        [OperationContract]
        bool ValidateVerificationCode(string packetName, string verificationCode);

        [OperationContract]
        string GenerateLicInfoByVerifyData(ref Byte[] byteVerifyData);

        [OperationContract()]
        void CreatePacket(PacketTable packet);

        [OperationContract()]
        void UpdatePacket(PacketTable packet);

        [OperationContract()]
        void DeletePacket(PacketTable packet);

        #endregion

        #region Transaction OperationContracts

        [OperationContract]
        IList<TransactionTable> GetTransactionsByPacketID(int packetID);

        [OperationContract]
        IList<TransactionTable> GetNewTransactionsByLicenseName(string licenseName);

        [OperationContract]
        void UpdateSubLicenseTransactionsByOrder(int stdLicOrderID, int subLicOrderID);

        [OperationContract()]
        void CreateTransaction(TransactionTable ta);

        [OperationContract()]
        void UpdateTransaction(TransactionTable ta);

        [OperationContract()]
        void DeleteTransaction(TransactionTable ta);
        #endregion

        #region Order OperationContracts
        [OperationContract]
        IList<OrderTable> GetAllOrders();

        [OperationContract]
        IList<OrderTable> GetOrdersByLicenseName(string licenseName);

        [OperationContract]
        IList<OrderTable> GetOrdersEqualsProduct(string licenseName, string productName);

        [OperationContract]
        IList<string> GetAddOnOrdersByStandardOrder(string standardOrder);

        [OperationContract]
        OrderTable GetOrderByOrderName(string orderName);

        [OperationContract]
        int GetNextOrderIndex(string licenseName);

        [OperationContract]
        int GetProductVersionFromTable(int productID);

        [OperationContract()]
        void CreateOrder(OrderTable ta);

        [OperationContract()]
        void UpdateOrder(OrderTable ta);

        [OperationContract()]
        void DeleteOrder(OrderTable ta);

        #endregion

        #region Token TokenContracts
        [OperationContract]
        IList<TokenTable> GetAllTokens(string searchString, Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType token);
        
        [OperationContract]
        IList<TokenTable> GetAllTokensByCustomer(string searchString, string custName, Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType token);

        [OperationContract]
        IList<TokenTable> GetTokensByLicenseName(string licenseName);

        [OperationContract]
        TokenTable GetHardwareTokenByKeyValue(string keyValue);

        [OperationContract]
        IList<TokenTable> GetAvailableHardwareTokensByCustID(uint custID);

        [OperationContract]
        TokenTable GetTokenByLicenseName(string licenseName, byte tokenType);

        [OperationContract]
        bool TokenExists(uint custID, byte tokenType, string tokenValue);

        [OperationContract]
        bool IsHardwareTokenActive(uint custID, string tokenValue);

        [OperationContract]
        uint GetNextHardwareTokenValue(uint custID);

        [OperationContract]
        void CreateToken(TokenTable ta);

        [OperationContract]
        void UpdateToken(TokenTable ta);

        [OperationContract]
        void DeleteToken(TokenTable ta);

        #endregion

        #region DestinationName DestinationNameContracts
        [OperationContract]
        IList<DestinationNameTable> GetDestNamesByCustID(int custID);

        [OperationContract]
        DestinationNameTable GetDestinationName(int custID, int destID);

        [OperationContract]
        DestinationNameTable GetDestinationID(int custID, string destName);

        [OperationContract]
        uint GetNextDestinationID(uint custID);

        [OperationContract()]
        void CreateDestinationName(DestinationNameTable destName);

        [OperationContract()]
        void UpdateDestinationName(DestinationNameTable destName);

        [OperationContract()]
        void DeleteDestinationName(DestinationNameTable destName);

        #endregion

        #region Permission PermissionContracts
        [OperationContract]
        PermissionsTable GetPermissionsByUser(string user);
        #endregion

        #region Product OperationContracts
        [OperationContract]
        IList<ProductTable> GetProducts();

        [OperationContract]
        ProductTable GetProduct(string product);

        [OperationContract]
        void UpdateProductTable(ProductTable product);
        #endregion
    }

    [DataContract]
    public enum Name
    {
        [EnumMember]
        Customer,
        [EnumMember]
        DestinationID,
        [EnumMember]
        GroupID,
        [EnumMember]
        LicenseType
    }
    
    [DataContract]
    public enum Operator
    { 
        [EnumMember]
        Equals,
        [EnumMember]
        LessThan,
        [EnumMember]
        GreaterThan
    }     

    [DataContract]
    public class Condition
    {
        private Name _name;
        private Operator _operator;
        private string _value;

        [DataMember]
        public Name ConditionName
        {
            get { return _name; }
            set { _name = value; }
        }

        //numeric operators - =, >, <
        //string operators - is, contains, is not, does not contain
        [DataMember]
        public Operator ConditionOperator
        {
            get { return _operator;  }
            set { _operator = value; }
        }

        //can be numeric or string       
        [DataMember]
        public string ConditionValue
        {
            get { return _value; }
            set { _value = value; }
        }
    }
}
