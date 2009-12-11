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
        IList<CustomerTable> GetAllCustomers(string searchStr, bool enableLoadOptions);

        [OperationContract]
        IList<string> GetAllCustomerNames(string searchStr);

        [OperationContract]
        int GetCustomerCount(string searchStr, bool enableLoadOptions);

        [OperationContract]
        CustomerTable GetCustomer(string searchStr, bool enableLoadOptions);

        [OperationContract]
        void CreateCustomer(CustomerTable ct);

        [OperationContract]
        void UpdateCustomer(CustomerTable ct);

        [OperationContract]
        void DeleteCustomer(CustomerTable ct);

        [OperationContract]
        int GetLastCustomerID();

        #endregion

        #region License OperationContracts
        [OperationContract]
        IList<LicenseTable> GetLicensesByCustomer(string custName);

        [OperationContract]
        IList<LicenseTable> GetLicensesByDestination(string custName, string destName, string findStr, bool bLoadOptions);        

        [OperationContract]
        LicenseTable GetLicenseByName(string licName, bool bLoadOptions);

        [OperationContract]
        LicenseTable GetLicenseByID(int licID, bool bLoadOptions);

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
        int GetLicenseType(string licName);

        [OperationContract]
        IList<LicenseTable> GetLicensesByConditions(IList<Condition> cl);

        [OperationContract]
        bool IsLicenseModified(string licName);

        [OperationContract]
        IList<string> GetModifiedLicensesByCustomer(string custName);

        [OperationContract]
        int GetLicenseCountByType(uint custID, uint destID, uint groupID, Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType licType);

        [OperationContract]
        int GetDerivedLicenseCount(uint custID, uint destID, uint groupID, Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType licType);

        [OperationContract]
        IList<string> GetDerivedLicenseNames(uint custID, uint destID, uint groupID, Byte licType);

        [OperationContract]
        void GenerateLicensePacket(string pktName, LicenseTable licName, DateTime expDate, string comments, ref string vCode, ref Byte[] newByteArrayLicensePacket, string user);

        [OperationContract]
        IList<SolimarLicenseProtectionKeyInfo> KeyEnumerate();

        [OperationContract]
        string KeyFormat(string key);

        [OperationContract]
        string KeyProgramVerification(string key, int custNum, int keyNum);

        [OperationContract()]
        void CreateLicense(LicenseTable lt);

        [OperationContract()]
        void UpdateLicense(LicenseTable lt, bool isDirty);

        [OperationContract()]
        void DeleteLicense(LicenseTable lt);

        #endregion

        #region Packet OperationContracts

        [OperationContract]
        IList<PacketTable> GetPacketsByLicenseName(string ln);

        [OperationContract]
        PacketTable GetPacketByVerificationCode(string vCode);

        [OperationContract]
        bool ValidateVerificationCode(string pktName, string vCode);

        [OperationContract]
        string GenerateLicInfoByVerifyData(ref Byte[] byteVerifyData);

        [OperationContract()]
        void CreatePacket(PacketTable pt);

        [OperationContract()]
        void UpdatePacket(PacketTable pt);

        [OperationContract()]
        void DeletePacket(PacketTable pt);

        #endregion

        #region Transaction OperationContracts

        //[OperationContract]
        //IList<TransactionTable> GetTransactionsByLicenseName(string licenseName);

        [OperationContract]
        IList<TransactionTable> GetTransactionsByPacketID(int pktID);

        [OperationContract]
        IList<TransactionTable> GetNewTransactionsByLicenseName(string licName);

        [OperationContract]
        void UpdateSubLicenseTransactionsByOrder(int stdLicOrderID, int subLicOrderID);

        [OperationContract()]
        void CreateTransaction(TransactionTable tt);

        [OperationContract()]
        void UpdateTransaction(TransactionTable tt);

        [OperationContract()]
        void DeleteTransaction(TransactionTable tt);
        #endregion

        #region Product License OperationContracts
        [OperationContract]
        IList<ProductLicenseTable> GetProductLicensesByConditions(IList<Condition> cl);

        [OperationContract]
        IList<ProductLicenseTable> GetAllProductLicenses();

        [OperationContract]
        IList<ProductLicenseTable> GetProductLicenses(string lsID);

        [OperationContract]
        IList<ProductLicenseTable> GetProductLicensesByProduct(string lsID, int prodID);

        [OperationContract]
        ProductLicenseTable GetProductLicense(string plID);

        [OperationContract]
        IList<string> GetAddOnProductLicenses(string plID);

        [OperationContract]
        int GetNextProductLicenseIndex(string lsID);

        [OperationContract]
        int GetProductVersionFromTable(int prodID);

        [OperationContract]
        IList<string> GetDeactivatedProductLicenses(string lsName);

        [OperationContract]
        void DeactivateProductLicense(string plID);

        [OperationContract]
        void DeleteAllProductLicenses(string lsName);

        [OperationContract()]
        void CreateProductLicense(ProductLicenseTable ta);

        [OperationContract()]
        void UpdateProductLicense(ProductLicenseTable ta);

        [OperationContract()]
        void DeleteProductLicense(ProductLicenseTable ta);

        #endregion

        #region Token TokenContracts
        [OperationContract]
        IList<TokenTable> GetAllTokens(string searchString, Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType token);
        
        [OperationContract]
        IList<TokenTable> GetAllTokensByCustomer(string searchString, string custName, Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType token);

        [OperationContract]
        IList<TokenTable> GetTokensByLicenseName(string licName);

        [OperationContract]
        TokenTable GetHardwareTokenByKeyValue(string keyValue);

        [OperationContract]
        IList<TokenTable> GetAvailableHardwareTokensByCustID(uint custID);

        [OperationContract]
        TokenTable GetTokenByLicenseName(string licName, byte tokenType);

        [OperationContract]
        bool TokenExists(uint custID, byte tokenType, string tokenValue);

        [OperationContract]
        bool IsHardwareTokenActive(uint custID, string tokenValue);

        [OperationContract]
        uint GetNextHardwareTokenValue(uint custID);

        [OperationContract]
        void CreateToken(TokenTable tt);

        [OperationContract]
        void UpdateToken(TokenTable tt);

        [OperationContract]
        void DeleteToken(TokenTable tt);

        #endregion

        #region DestinationName DestinationNameContracts
        [OperationContract]
        IList<DestinationNameTable> GetDestNamesByCustID(int custID);

        [OperationContract]
        DestinationNameTable GetDestinationName(int custID, int destID);

        [OperationContract]
        DestinationNameTable GetDestinationID(int custID, string dn);

        [OperationContract]
        void DeleteAllDestinationNames(int custID);

        [OperationContract]
        uint GetNextDestinationID(uint custID);

        [OperationContract()]
        void CreateDestinationName(DestinationNameTable dnt);

        [OperationContract()]
        void UpdateDestinationName(DestinationNameTable dnt);

        [OperationContract()]
        void DeleteDestinationName(DestinationNameTable dnt);

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
        void UpdateProductTable(ProductTable pt);

        #endregion

        #region SoftwareToken OperationContracts
        [OperationContract]
        void UpdateSoftwareTokenTable(SoftwareTokenTable stt);
        [OperationContract]
        SoftwareTokenTable GetSoftwareToken(string tokenType);
        [OperationContract]
        IList<SoftwareTokenTable> GetAllSoftwareTokens();
        #endregion
    }

    [DataContract]
    public enum ConditionName
    {
        [EnumMember]
        Customer,
        [EnumMember]
        LicenseServer,
        [EnumMember]
        LicenseType,
        [EnumMember]
        ProductLicense,
        [EnumMember]
        ExpirationDate,       
        [EnumMember]
        Product,
        [EnumMember]
        ProductVersion,
        [EnumMember]
        State
    }

    [DataContract]
    public enum ConditionOperator
    {
        [EnumMember]
        Equal,
        [EnumMember]
        NotEqual,
        [EnumMember]
        LessThan,
        [EnumMember]
        GreaterThan,
        [EnumMember]
        Contains
    }

    [DataContract]
    public class Condition
    {
        private ConditionName _name;
        private ConditionOperator _operator;
        private string _value;

        [DataMember]
        public ConditionName Name
        {
            get { return _name; }
            set { _name = value; }
        }

        //numeric operators - =, >, <
        //string operators - is, contains, is not, does not contain
        [DataMember]
        public ConditionOperator Operator
        {
            get { return _operator; }
            set { _operator = value; }
        }

        //can be numeric or string       
        [DataMember]
        public string Value
        {
            get { return _value; }
            set { _value = value; }
        }
    }
}
