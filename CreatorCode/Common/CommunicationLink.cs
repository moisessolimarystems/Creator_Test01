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

        public void SoftwareLicenseDisasterRecoveryExtendTimeByLicense(String softwareLicense)
        {
            try
            {
                m_licServer.SoftwareLicenseUseActivationToExtendTime_ByLicense(softwareLicense);
            }
            catch(COMException ex)
            {
                throw new COMException("SoftwareLicenseDisasterRecoveryExtendTimeByLicense Failed", ex);
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
                                           "",
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
        public Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.Lic_ModuleSoftwareSpecAttribsMap GetModuleSpecList(uint productID)
        {
            return Lic_LicenseInfoAttribsHelper.GetModuleList(m_softwareSpec, productID);
        }

        public Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs GetTestDevProductSpec(uint productID)
        {
            foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in m_softwareSpec.productSpecMap.TVal.Values)
            {
                if (productSpec.sameModSpecProductID == productID)
                    return productSpec;
            }
            //need to return a valid bad value
            return null;
        }

        public Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs GetProductSpec(uint productID)
        {
            foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in m_softwareSpec.productSpecMap.TVal.Values)
            {
                if (productSpec.productID == productID)
                    return productSpec;
            }
            //need to return a valid bad value
            return null;
        }

        public string GetProductName(uint productID)
        {
            foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in m_softwareSpec.productSpecMap.TVal.Values)
            {
                if (productSpec.productID == productID)
                    return productSpec.productName;
            }
            return "Unknown";
        }

        public string GetModuleName(uint productID, uint moduleID)
        {
            Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.Lic_ModuleSoftwareSpecAttribsMap moduleList = GetModuleSpecList(productID);
            foreach (Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs moduleSpec in moduleList.TVal.Values)
            {
                if (moduleSpec.moduleID == moduleID)
                    return moduleSpec.moduleName;
            }
            return "Unknown";
        }

        public int GetProductID(String productName)
        {
            foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in m_softwareSpec.productSpecMap.TVal.Values)
            {
                if (productSpec.productName == productName)
                    return (int)productSpec.productID.TVal;
            }
            //need to return a valid bad value
            return -1;
        }

        public int GetModuleID(uint productID, String moduleName)
        {
            Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.Lic_ModuleSoftwareSpecAttribsMap moduleList = GetModuleSpecList(productID);
            foreach (Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs moduleSpec in moduleList.TVal.Values)
            {
                if (moduleSpec.moduleName == moduleName)                            
                    return (int)moduleSpec.moduleID.TVal;                        
            }                   
            //need to return a valid bad value
            return -1;
        }

        public bool IsDefaultModule(uint productID, uint moduleID)
        {
            foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in m_softwareSpec.productSpecMap.TVal.Values)
            {
                if (productSpec.productID == productID)
                {
                    foreach (Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs moduleSpec in productSpec.moduleSpecMap.TVal.Values)
                    {
                        if (moduleSpec.moduleID.TVal == moduleID)
                        {
                            if (moduleSpec.moduleDefaultLicense.TVal > 0)
                                return true;
                        }
                    }
                }
            }
            //need to return a valid bad value
            return false;
        }

        public bool IsDefaultModule(uint productID, string moduleName)
        {
            foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in m_softwareSpec.productSpecMap.TVal.Values)
            {
                if (productSpec.productID == productID)
                {
                    foreach (Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs moduleSpec in productSpec.moduleSpecMap.TVal.Values)
                    {
                        if (moduleSpec.moduleName.TVal == moduleName)
                        {
                            if (moduleSpec.moduleDefaultLicense.TVal > 0)
                                return true;
                        }
                    }
                }
            }
            //need to return a valid bad value
            return false;
        }

        public int GetDefaultModuleValue(uint productID, uint moduleID)
        {
            foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in m_softwareSpec.productSpecMap.TVal.Values)
            {
                if (productSpec.productID == productID)
                {
                    foreach (Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs moduleSpec in productSpec.moduleSpecMap.TVal.Values)
                    {
                        if (moduleSpec.moduleID.TVal == moduleID)
                        {
                                return (int)moduleSpec.moduleDefaultLicense.TVal;
                        }
                    }
                }
            }
            //need to return a valid bad value
            return -1;
        }

        public int GetUnlimitedModuleValue(uint productID, uint moduleID)
        {
            foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in m_softwareSpec.productSpecMap.TVal.Values)
            {
                if (productSpec.productID == productID)
                {
                    foreach (Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs moduleSpec in productSpec.moduleSpecMap.TVal.Values)
                    {
                        if (moduleSpec.moduleID.TVal == moduleID)
                        {
                            return (int)moduleSpec.modUnlimitedValue.TVal;
                        }
                    }
                }
            }
            //need to return a valid bad value
            return -1;
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
        #endregion

        #region Private Variables
        public Lic_PackageAttribs.Lic_SoftwareSpecAttribs m_softwareSpec;
        private SolimarLicenseServerWrapper m_licServer;
        private SolimarLicenseWrapper m_licenseWrapper;
        private String m_ServerName;
        #endregion
    }
}
