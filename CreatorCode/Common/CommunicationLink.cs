// CommunicationLink.cs
//
// SLB 15.sep.2025 CR.34456; Changes for new attribs code (Licensing 3.4+) to work.

using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Diagnostics;
using Solimar.Licensing.LicenseManagerWrapper;
using Solimar.Licensing;
using Solimar.Licensing.Attribs;

namespace Client.Creator
{
    public class CommunicationLink
    {
        #region Constructor
        public CommunicationLink()
        {
            m_licServer = new Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseServerWrapper();
            m_licenseWrapper = new Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseWrapper();
            //create a new global spec
            Solimar.Licensing.GlobalSoftwareSpec globalSwSpec = new Solimar.Licensing.GlobalSoftwareSpec();
            m_softwareSpec = globalSwSpec.softwareSpec;            
            m_ServerName = "Not Connected";
        }
        #endregion

        #region LicenseServerWrapper Methods
        public void Connect(String serverName)
        {
            try
            {
                m_licServer.Disconnect();
                m_licServer.Connect(serverName);
                m_licenseWrapper.ConnectEx(serverName);
                
                m_ServerName = serverName;
            }
            catch (COMException ex)
            {
                throw new COMException("Connect Failed", ex);
            }
        }

        public void GetAllSoftwareLicenses(ref String slStream)
        {
            try
            {
                slStream = m_licServer.GetAllSoftwareLicenses();
            }
            catch (COMException ex)
            {
                throw new COMException("GetAllSoftwareLicenses Failed", ex);
            }
        }

        public void GetSoftwareLicenseInfoForAll(ref String slStream)
        {
            try
            {
                slStream = m_licServer.GetSoftwareLicenseInfo_ForAll();
            }
            catch (COMException ex)
            {
                throw new COMException("GetSoftwareLicenseInfoForAll Failed", ex);
            }
        }

        public void GetSoftwareLicenseInfoByLicense(String softwareLicense, ref String sliStream)
        {
            try
            {
                sliStream = m_licServer.GetSoftwareLicenseInfo_ByLicense(softwareLicense);
            }
            catch (COMException ex)
            {
                throw new COMException("GetSoftwareLicenseInfoByLicense Failed", ex);
            }
        }

        public void SoftwareModuleLicenseInUseForAllByProduct(int prodID, int modID, ref int licCount)
        {
            try
            {
                licCount = m_licServer.SoftwareModuleLicenseInUseForAll_ByProduct(prodID, modID);
            }
            catch (COMException ex)
            {
                throw new COMException("SoftwareModuleLicenseInUseForAllByProduct Failed", ex);
            }
        }

        public void SoftwareModuleLicenseTotalForAllByProduct(int prodID, int modID, ref int licenseCount)
        {
            try
            {
                licenseCount = m_licServer.SoftwareModuleLicenseTotalForAll_ByProduct(prodID, modID);
            }
            catch (COMException ex)
            {
                throw new COMException("SoftwareModuleLicenseTotalForAllByProduct Failed", ex);
            }
        }

        public void SoftwareGetApplicationInstanceListByProduct(int prodID, ref String generalStream)
        {
            try
            {
                generalStream = m_licServer.SoftwareGetApplicationInstanceListByProduct(prodID);
            }
            catch (COMException ex)
            {
                throw new COMException("SoftwareGetApplicationInstanceListByProduct Failed", ex);
            }
        }

        public void VerifyTokenByLicense(String softwareLicense, int verificationTokenType, String verificationValue)
        {
            try
            {
                m_licServer.ValidateToken_ByLicense(softwareLicense, verificationTokenType, verificationValue);
            }
            catch (COMException ex)
            {
                throw new COMException("VerifyTokenByLicense Failed", ex);
            }
        }

        public void EnterSoftwareLicPacket(Byte[] byteLicPacket, ref String verificationCode)
        {
            try
            {
                verificationCode = m_licServer.EnterSoftwareLicPacket(byteLicPacket);
            }
            catch (COMException ex)
            {
                throw new COMException("EnterSoftwareLicPacket Failed", ex);
            }
        }

        public void GenerateSoftwareLicPacket(String licenseAttribs, DateTime expirationDate, ref String verificationCode, ref Byte[] newByteArrayLicensePacket)
        {
            try
            {
                m_licServer.GenerateSoftwareLicPacket(licenseAttribs, expirationDate, ref verificationCode, ref newByteArrayLicensePacket);
            }
            catch (COMException ex)
            {
                throw new COMException("EnterSoftwareLicPacket Failed", ex);
            }
        }

        #endregion 

        #region ManagerWrapper Methods
        public void InitializeWrapper(String appID, int productID, int prodMajor, int prodMinor)
        {
            try
            {
                m_licenseWrapper.InitializeEx(appID,
                                           productID,
                                           prodMajor,
                                           prodMinor,
                                           false,
                                           string.Empty,
                                           false,
                                           Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseWrapper.LICENSE_LEVEL.UI_LEVEL_ALL,
                                           0,
                                           false,
                                           false);
            }
            catch (COMException ex)
            {
                throw new COMException("InitializeWrapper Failed", ex);
            }
        }

        public void ModuleLicenseTotal(int modID, ref int licenseCount)
        {
            try
            {
                licenseCount = m_licenseWrapper.ModuleLicenseTotalEx(modID);
            }
            catch (COMException ex)
            {
                throw new COMException("InitializeWrapper Failed", ex);
            }
        }

        public void ModuleLicenseInUse(int modID, ref int licenseCount)
        {
            try
            {
                licenseCount = m_licenseWrapper.ModuleLicenseInUseEx(modID);
            }
            catch (COMException ex)
            {
                throw new COMException("InitializeWrapper Failed", ex);
            }
        }

        public void ModuleLicenseObtain(int modID, int licenseCount)
        {
            try
            {
                m_licenseWrapper.ModuleLicenseObtainEx(modID, licenseCount);
            }
            catch (COMException ex)
            {
                throw new COMException("ModuleLicenseObtain Failed", ex);
            }
        }

        public void ModuleLicenseRelease(int modID, int licenseCount)
        {
            try
            {
                m_licenseWrapper.ModuleLicenseReleaseEx(modID, licenseCount);
            }
            catch (COMException ex)
            {
                throw new COMException("ModuleLicenseRelease Failed", ex);
            }
        }

        #endregion

        #region Helper Methods

        public Int64 ConvertFromBase36(string _base36Number)
        {
            string base36Chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
            char[] arrInput = _base36Number.ToUpper().ToCharArray();
            Array.Reverse(arrInput);
            Int64 returnValue = 0;
            for (int i = 0; i < arrInput.Length; i++)
            {
                int valueindex = base36Chars.IndexOf(arrInput[i]);
                returnValue += Convert.ToInt64(valueindex * Math.Pow(36, i));
            }
            return returnValue;
        }

        public List<Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs> GetModuleSpecForProductVersion(uint productID, uint versionMajor, uint versionMinor)
        {
            List<Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs> moduleSpecList = new List<Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs>();
            Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.Lic_ModuleSoftwareSpecAttribsMap moduleSpecMap = GetModuleSpecList(productID);
            foreach (Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs moduleSpec in moduleSpecMap.Values) // CR.34456; modified.
			{
                bool bDeprecated = false;
                bool bValid = false;
                if (moduleSpec.moduleVersionDeprecated_Major.TVal > 0)
                {
                    if (versionMajor > moduleSpec.moduleVersionDeprecated_Major)
                        bDeprecated = true;
                    else if (versionMajor == moduleSpec.moduleVersionDeprecated_Major)
                    {
                        if (versionMinor >= moduleSpec.moduleVersionDeprecated_Minor)
                            bDeprecated = true;
                    }
                }
                if (versionMajor > moduleSpec.moduleVersionIntroduced_Major)
                    bValid = true;
                else if (versionMajor == moduleSpec.moduleVersionIntroduced_Major)
                {
                    if (versionMinor >= moduleSpec.moduleVersionIntroduced_Minor)
                        bValid = true;
                }
                if (bValid && !bDeprecated)
                    moduleSpecList.Add(moduleSpec);
            }
            return moduleSpecList;
        }

        public Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.Lic_ModuleSoftwareSpecAttribsMap GetModuleSpecList(uint productID)
        {
            return Lic_LicenseInfoAttribsHelper.GetModuleList(m_softwareSpec, productID);
        }

        public List<String> GetModuleNameList(uint productID)
        {
            List<String> moduleList = new List<string>();
            Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.Lic_ModuleSoftwareSpecAttribsMap moduleSpecList = GetModuleSpecList(productID);
            foreach (Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs moduleSpec in moduleSpecList.Values) // CR.34456; modified.
			{
                moduleList.Add(moduleSpec.moduleName.TVal);
            }
            return moduleList;
        }

        public List<String> GetAllModuleNamesList()
        {
            List<String> moduleList = new List<string>();
            List<String> productList = new List<string>();
            foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in m_softwareSpec.productSpecMap.Values) // CR.34456; modified.
			{
                moduleList.AddRange(GetModuleNameList(productSpec.productID.TVal));
            }
            return moduleList;
        }

        public Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs GetTestDevProductSpec(byte productID)
        {
            string productName = "Test/Dev/DR " + GetProductName(productID);
            foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in m_softwareSpec.productSpecMap.Values) // CR.34456; modified.
			{
                //want to find test dev product spec given base product id                
                if (productSpec.productName.Equals(productName) || productSpec.productLabel.Equals(productName)) // CR.34456; modified.
				{
                    return productSpec;                        
                }
            }
            //need to return a valid bad value
            return null;
        }

        public bool IsClientType(ushort productID)
        {
            Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec = GetProductSpec(productID);
            if (productSpec != null)
                return (productSpec.productLicType.TVal == Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.TProductLicenseType.pltClient);
            return false;
        }

        public Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs GetProductSpec(ushort productID)
        {
            foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in m_softwareSpec.productSpecMap.Values) // CR.34456; modified.
			{
                if (productSpec.productID.TVal == productID)
                {
                    if (productSpec.sameModSpecProductID.TVal > 0)
                        return GetProductSpec((ushort)productSpec.sameModSpecProductID.TVal);
                    return productSpec;
                }
            }
            return null;
        }

        public uint GetModuleSpecRevision(ushort productID, ushort modID)
        {
            foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in m_softwareSpec.productSpecMap.Values) // CR.34456; modified.
			{
                if (productSpec.productID.TVal == productID)
                {
                    if (productSpec.sameModSpecProductID.TVal > 0)
                        return GetModuleSpecRevision((ushort)productSpec.sameModSpecProductID.TVal, modID);
                    foreach (Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs moduleSpec in productSpec.moduleSpecMap.Values) // CR.34456; modified.
					{
                        if (moduleSpec.moduleID.TVal == modID)
                            return moduleSpec.moduleVersionIntroduced_SubMinor.TVal;                            
                    }         
                }
            }
            return 0;
        }

        public uint GetProductIDByModuleName(string moduleName)
        {
            uint productID = 0;
            foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in m_softwareSpec.productSpecMap.Values) // CR.34456; modified.
			{
                foreach (Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs module in productSpec.moduleSpecMap.Values) // CR.34456; modified.
				{
                    //if (module.moduleName.ToLower() == moduleName.ToLower())
                    if (string.Compare(module.moduleName.TVal, moduleName, true) == 0) 
					{
                        productID = productSpec.productID.TVal;
                        break;
                    }
                }
                if (productID != 0) break;
            }
            return productID;
        }

        public List<String> GetProductNameList()
        {
            List<String> productList = new List<string>();
            string productName;
            foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in m_softwareSpec.productSpecMap.Values) // CR.34456; modified.
			{
                productName = (productSpec.productLabel.TVal.Length > 0) ? productSpec.productLabel.TVal : productSpec.productName.TVal; 
                productList.Add(productName);
            }
            return productList;
        }

        public string GetProductName(byte productID)
        {
            foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in m_softwareSpec.productSpecMap.Values) // CR.34456; modified.
			{
                if (productSpec.productID.TVal == productID)
                    return (productSpec.productLabel.TVal.Length > 0) ? productSpec.productLabel : productSpec.productName;
            }
            return "Unknown";
        }

        public string GetProductBaseName(string productName)
        {           
            if (productName.Contains("Test"))
                productName = productName.Replace("Test/Dev/DR", string.Empty).Trim();
            if (productName.Contains("PLP"))
                productName = productName.Replace("PLP", string.Empty).Trim();
            return productName;        
        }

        public string GetProductBaseName(byte productID)
        {
            string productName = GetProductName(productID);
            if (productName.Contains("Test"))
                productName = productName.Replace("Test/Dev/DR", string.Empty).Trim();
            if (productName.Contains("PLP"))
                productName = productName.Replace("PLP", string.Empty).Trim();
            return productName;
        }

        public string GetModuleName(uint productID, short moduleID)
        {
            Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.Lic_ModuleSoftwareSpecAttribsMap moduleList = GetModuleSpecList(productID);
            if (moduleList != null)
            { 
                foreach (Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs moduleSpec in moduleList.Values) // CR.34456; modified.
				{
                    if (moduleSpec.moduleID.TVal == moduleID)
                        return moduleSpec.moduleName;
                }
            }
            return "Unknown";
        }

        public Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs GetModuleSpec(uint productID, short moduleID)
        {
            Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.Lic_ModuleSoftwareSpecAttribsMap moduleList = GetModuleSpecList(productID);
            if (moduleList != null)
            {
                foreach (Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs moduleSpec in moduleList.Values) // CR.34456; modified.
				{
                    if (moduleSpec.moduleID.TVal == moduleID)
                        return moduleSpec;
                }
            }
            return null;
        }

        public short GetProductID(String productName)
        {
            string compareProductName = string.Empty;     
            foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in m_softwareSpec.productSpecMap.Values) // CR.34456; modified.
			{
                compareProductName = (productSpec.productLabel.TVal.Length > 0) ? productSpec.productLabel.TVal : productSpec.productName.TVal;
                //if (productSpec.productName.ToLower() == productName.ToLower())
                if (string.Compare(compareProductName, productName.Trim(), true) == 0)
                    return (short)productSpec.productID.TVal;
            }
            //need to return a valid bad value
            return -1;
        }

        public short GetModuleID(uint productID, String moduleName)
        {
            Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.Lic_ModuleSoftwareSpecAttribsMap moduleList = GetModuleSpecList(productID);
            if (moduleList != null)
            {
                foreach (Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs moduleSpec in moduleList.Values) // CR.34456; modified.
				{
                    //if (moduleSpec.moduleName.TVal.ToLower() == moduleName.ToLower())
                    if (string.Compare(moduleSpec.moduleName.TVal, moduleName, true) == 0)
                        return (short)moduleSpec.moduleID.TVal;
                }
            }   
            //need to return a valid bad value
            return -1;
        }

        public bool IsPLPModule(uint productID, short moduleID)
        {
            foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in m_softwareSpec.productSpecMap.Values) // CR.34456; modified.
			{
                if (productSpec.productID.TVal == productID)
                {
                    if (productSpec.sameModSpecProductID.TVal > 0)
                        return IsPLPModule(productSpec.sameModSpecProductID.TVal, moduleID);
                    else
                    {
                        foreach (Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs moduleSpec in productSpec.moduleSpecMap.Values) // CR.34456; modified.
						{
                            //if (moduleSpec.moduleName.TVal.ToLower() == moduleName.ToLower())
                            if (moduleSpec.moduleID.TVal == moduleID)
                            {
                                if (moduleSpec.moduleName.TVal.ToUpper().EndsWith("PLP")) 
                                    return true;
                            }
                        }
                    }
                }
            }
            //need to return a valid bad value
            return false;
        }

        public bool IsDefaultModule(uint productID, short moduleID)
        {
            foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in m_softwareSpec.productSpecMap.Values) // CR.34456; modified.
			{
                if (productSpec.productID.TVal == productID)
                {
                    if (productSpec.sameModSpecProductID.TVal > 0)
                        return IsDefaultModule(productSpec.sameModSpecProductID.TVal, moduleID);
                    else
                    {
                        foreach (Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs moduleSpec in productSpec.moduleSpecMap.Values) // CR.34456; modified.
						{
                            if (moduleSpec.moduleID.TVal == moduleID)
                            {
                                if (moduleSpec.moduleDefaultLicense.TVal > 0 && moduleSpec.moduleTrialLicense.TVal <= 1)
                                    return true;
                            }
                        }
                    }
                }
            }
            //need to return a valid bad value
            return false;
        }

        public bool IsDefaultModule(uint productID, string moduleName)
        { 
            foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in m_softwareSpec.productSpecMap.Values) // CR.34456; modified.
			{
                if (productSpec.productID.TVal == productID)
                {
                    if (productSpec.sameModSpecProductID.TVal > 0)
                        return IsDefaultModule(productSpec.sameModSpecProductID.TVal, moduleName);
                    else
                    {
                        foreach (Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs moduleSpec in productSpec.moduleSpecMap.Values) // CR.34456; modified.
						{
                            //if (moduleSpec.moduleName.TVal.ToLower() == moduleName.ToLower())
                            if(string.Compare(moduleSpec.moduleName.TVal,  moduleName, true) == 0)
                            {
                                if (moduleSpec.moduleDefaultLicense.TVal > 0)
                                    return true;
                            }
                        }
                    }
                }
            }
            //need to return a valid bad value
            return false;
        }

        public uint GetDefaultModuleValue(uint productID, short moduleID)
        {
            foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in m_softwareSpec.productSpecMap.Values) // CR.34456; modified.
			{
                if (productSpec.productID.TVal == productID)
                {
                    if (productSpec.sameModSpecProductID.TVal > 0)
                        return GetDefaultModuleValue(productSpec.sameModSpecProductID.TVal, moduleID);
                    else
                    {
                        foreach (Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs moduleSpec in productSpec.moduleSpecMap.Values) // CR.34456; modified.
						{
                            if (moduleSpec.moduleID.TVal == moduleID)
                            {
                                return moduleSpec.moduleDefaultLicense.TVal;
                            }
                        }
                    }
                }
            }
            //need to return a valid bad value
            return 0;
        }

        public uint GetUnlimitedModuleValue(uint productID, uint moduleID)
        {
            foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in m_softwareSpec.productSpecMap.Values) // CR.34456; modified.
			{
                if (productSpec.productID.TVal == productID)
                {
                    if(productSpec.sameModSpecProductID.TVal > 0)
                        return GetUnlimitedModuleValue(productSpec.sameModSpecProductID.TVal, moduleID);
                    else 
                    {
                        foreach (Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs moduleSpec in productSpec.moduleSpecMap.Values) // CR.34456; modified.
						{
                            if (moduleSpec.moduleID.TVal == moduleID)
                            {
                                return moduleSpec.modUnlimitedValue.TVal;
                            }
                        }
                    }
                }
            }
            //need to return a valid bad value
            return 0;
        }

        public uint GetModuleTrialValue(uint productID, short moduleID)
        {
            foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in m_softwareSpec.productSpecMap.Values) // CR.34456; modified.
			{
                if (productSpec.productID.TVal == productID)
                {
                    if (productSpec.sameModSpecProductID.TVal > 0)
                        return GetModuleTrialValue(productSpec.sameModSpecProductID.TVal, moduleID);
                    else
                    {
                        foreach (Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs moduleSpec in productSpec.moduleSpecMap.Values) // CR.34456; modified.
						{
                            if (moduleSpec.moduleID.TVal == moduleID)
                            {
                                return moduleSpec.moduleTrialLicense.TVal;
                            }
                        }
                    }
                }
            }
            //need to return a valid bad value
            return 0;
        }

        public void WriteEventLog(String EventLogMsg, EventLogEntryType LogType)
        {
            // source should be   created by install.
            if (EventLog.SourceExists( "Solimar License Viewer" ) )
            {
		        // Create an EventLog instance and assign its source.
		        EventLog myLog = new EventLog();
		        myLog.Source = "Solimar License Viewer";		   
		        // Write an informational entry to the event log.    
		        myLog.WriteEntry(EventLogMsg, LogType);
            }		   
        }
        #endregion

        #region Properties

        public string ServerName
        {
            get { return m_ServerName; }
            set { m_ServerName = value; }
        }

        public Lic_PackageAttribs.Lic_SoftwareSpecAttribs SoftwareSpec
        {
            get { return m_softwareSpec; }
        }
        #endregion

        #region Private Variables
        public Lic_PackageAttribs.Lic_SoftwareSpecAttribs m_softwareSpec;
        private SolimarLicenseServerWrapper m_licServer;
        private SolimarLicenseWrapper m_licenseWrapper;
        static private String m_ServerName;
        #endregion
    }
}
