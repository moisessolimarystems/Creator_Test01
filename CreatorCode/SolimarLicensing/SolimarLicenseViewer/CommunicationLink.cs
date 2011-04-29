using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Diagnostics;

namespace SolimarLicenseViewer
{
    public class KeyInfo
    {
        public KeyInfo()
        {
            keyName = "";
            productName = "";
            version = "0.0";
            hoursLeft = 0;
            expirationDate = null;
            keyTypeName = "";
            licenseTypeName = "";
            keyTypeID = 0;
            bIsActive = false;
            bIsProgrammed = false;
            applicationInstance = 0;
        }
        public string keyName;
        public string productName;
        public string version;
        public long hoursLeft;
        public DateTime? expirationDate;
        public string keyTypeName;
        public long keyTypeID;
        public string licenseTypeName;
        public bool bIsActive;
        public bool bIsProgrammed;
        public long applicationInstance;
    }
    public class CommunicationLink : Shared.VisualComponents.IEventLog
    {
        #region Constructor
        public CommunicationLink()
        {
            m_licenseWrapper = new Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseWrapper();
            m_licServer = null;
            //create a new global spec
            Solimar.Licensing.GlobalSoftwareSpec globalSwSpec = new Solimar.Licensing.GlobalSoftwareSpec();
            m_softwareSpec = globalSwSpec.softwareSpec;
            m_ServerName = "Not Connected";
        }

        #endregion

        #region IEventLog
        public string GetMachineName() { return this.ServerName; }
        public System.Collections.Generic.IList<Solimar.Licensing.Attribs.Sys_EventLogInfoAttribs.Sys_EventLogEntriesInfoAttribs> GetAllEntries()
        {
            string streamedEventLog = this.GetEventLogList_ForLicenseServer();

            System.Collections.Generic.IList<Solimar.Licensing.Attribs.Sys_EventLogInfoAttribs.Sys_EventLogEntriesInfoAttribs> eventLogList = new System.Collections.Generic.List<Solimar.Licensing.Attribs.Sys_EventLogInfoAttribs.Sys_EventLogEntriesInfoAttribs>();
            Solimar.Licensing.Attribs.Sys_EventLogInfoAttribs sysEventLogInfo = new Solimar.Licensing.Attribs.Sys_EventLogInfoAttribs();
            sysEventLogInfo.AssignMembersFromStream(streamedEventLog);

            foreach (Solimar.Licensing.Attribs.Sys_EventLogInfoAttribs.Sys_EventLogEntriesInfoAttribs eventEntryInfo in sysEventLogInfo.entryList.TVal)
            {
                eventLogList.Add(eventEntryInfo);
            }
            return eventLogList;

        }
        #endregion

        #region Async Helpers
        public delegate void DelParamVoidReturnsVoid();
        public delegate void DelParamStringBoolReturnsVoid(string s1, bool b1);
        public delegate void DelParamByteArrayReturnsVoid(Byte[] byteArray);
        public delegate void DelParamByteArrayRefStringReturnsVoid(Byte[] byteArray, ref string refS1);
        public delegate void DelParamRefByteArrayReturnsVoid(ref Byte[] refByteArray);
        public delegate void DelParamStringRefByteArrayReturnsVoid(string s1, ref Byte[] refByteArray);
        #endregion
        #region Async Version of LicenseServerWrapper Methods
        public void Async_Connect(String _serverName)
        {
            Async_Connect(_serverName, true);
        }
        public void Async_Connect(String _serverName, bool _bDisconnectOnError)
        {
            DelParamStringBoolReturnsVoid delFunction = new DelParamStringBoolReturnsVoid(Connect);

            IAsyncResult result = delFunction.BeginInvoke(_serverName, _bDisconnectOnError, null, null);
            result.AsyncWaitHandle.WaitOne();

            // Call EndInvoke to retrieve the results.
            delFunction.EndInvoke(result);
        }
        public void Async_EnterSoftwareLicArchive(Byte[] _byteArrayLicense)
        {
            DelParamByteArrayReturnsVoid delFunction = new DelParamByteArrayReturnsVoid(EnterSoftwareLicArchive);
            IAsyncResult result = delFunction.BeginInvoke(_byteArrayLicense, null, null);

            result.AsyncWaitHandle.WaitOne();

            // Call EndInvoke to retrieve the results.
            delFunction.EndInvoke(result);
        }
        public void Async_EnterSoftwareLicPacket(Byte[] _byteLicPacket, ref String _refVerificationCode)
        {
            DelParamByteArrayRefStringReturnsVoid delFunction = new DelParamByteArrayRefStringReturnsVoid(EnterSoftwareLicPacket);
            IAsyncResult result = delFunction.BeginInvoke(_byteLicPacket, ref _refVerificationCode, null, null);

            result.AsyncWaitHandle.WaitOne();

            // Call EndInvoke to retrieve the results.
            delFunction.EndInvoke(ref _refVerificationCode, result);
        }
        public void Async_GenerateLicenseSystemData(ref Byte[] _newByteArrayLicense)
        {
            DelParamRefByteArrayReturnsVoid delFunction = new DelParamRefByteArrayReturnsVoid(GenerateLicenseSystemData);
            IAsyncResult result = delFunction.BeginInvoke(ref _newByteArrayLicense, null, null);

            result.AsyncWaitHandle.WaitOne();

            // Call EndInvoke to retrieve the results.
            delFunction.EndInvoke(ref _newByteArrayLicense, result);
        }
        public void Async_GenerateSoftwareLicArchive_ByLicense(string _softwareLicense, ref byte[] _refByteLicenseArchive)
        {
            DelParamStringRefByteArrayReturnsVoid delFunction = new DelParamStringRefByteArrayReturnsVoid(GenerateSoftwareLicArchive_ByLicense);
            IAsyncResult result = delFunction.BeginInvoke(_softwareLicense, ref _refByteLicenseArchive, null, null);

            result.AsyncWaitHandle.WaitOne();

            // Call EndInvoke to retrieve the results.
            delFunction.EndInvoke(ref _refByteLicenseArchive, result);
        }
        #endregion

        #region LicenseServerWrapper Methods
        public void Connect(String serverName)
        {
            Connect(serverName, true);
        }
        public void Connect(String serverName, bool bDisconnectOnError)
        {
            try
            {
                Disconnect();

                try { m_licenseWrapper.ConnectLicenseViewerEx(serverName); }
                catch (COMException innerEx) 
                {
                    throw innerEx; 
                }
                if(m_licServer == null)
                    m_licServer = new Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseServerWrapper();

                try { m_licServer.Connect(serverName); }
                catch (COMException innerEx)
                {
                    m_licServer = null;
                    throw innerEx;
                }
                m_ServerName = serverName;

                //CR.FIX.13048 - Try to retrieve software spec from license server, use it if
                //it is newer than source version.

                //Calling GetSoftwareSpec() will cause the license server to crash in pre 3.0.276 and below.
                //1 option, try to call GetSoftwareSpecByProduct(), if E_NOTIMPL is returned, then
                //calling GetSoftwareSpec() will crash license server
                int svrMajor = 0;
                int svrMinor = 0;
                int svrBuild = 0;
                GetVersionLicenseServer(serverName, ref svrMajor, ref svrMinor, ref svrBuild);
                InstanceVersionInfo verInfo = new InstanceVersionInfo(svrMajor, svrMinor, svrBuild);
                if (verInfo.CompareTo(new InstanceVersionInfo(3, 0, 276)) > 0)
                {
                    string str = m_licServer.GetSoftwareSpec();
                    Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_SoftwareSpecAttribs tmpSpec = new Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_SoftwareSpecAttribs();
                    tmpSpec.AssignMembersFromStream(str);

                    bool? bReplaceSoftwareSpec = null;
                    if(tmpSpec.softwareSpec_Major.TVal != m_softwareSpec.softwareSpec_Major.TVal)
                        bReplaceSoftwareSpec = (tmpSpec.softwareSpec_Major.TVal > m_softwareSpec.softwareSpec_Major.TVal);
                    if (!bReplaceSoftwareSpec.HasValue && (tmpSpec.softwareSpec_Minor.TVal != m_softwareSpec.softwareSpec_Minor.TVal))
                        bReplaceSoftwareSpec = (tmpSpec.softwareSpec_Minor.TVal > m_softwareSpec.softwareSpec_Minor.TVal);
                    if (!bReplaceSoftwareSpec.HasValue && (tmpSpec.softwareSpec_SubMajor.TVal != m_softwareSpec.softwareSpec_SubMajor.TVal))
                        bReplaceSoftwareSpec = (tmpSpec.softwareSpec_SubMajor.TVal > m_softwareSpec.softwareSpec_SubMajor.TVal);
                    if (!bReplaceSoftwareSpec.HasValue && (tmpSpec.softwareSpec_SubMinor.TVal != m_softwareSpec.softwareSpec_SubMinor.TVal))
                        bReplaceSoftwareSpec = (tmpSpec.softwareSpec_SubMinor.TVal > m_softwareSpec.softwareSpec_SubMinor.TVal);

                    if ((bReplaceSoftwareSpec.HasValue == true) && (bReplaceSoftwareSpec.Value == true))
                    {
                        m_softwareSpec = tmpSpec;
                        tmpSpec = null;
                    }
                }
            }
            catch (COMException ex)
            {
                m_exception = ex;
                if (bDisconnectOnError)
                    Disconnect();
                else
                    m_ServerName = serverName;
                throw;
            }
        }
        public void SetData(Byte[] diagnosticDateByteArray)
        {
            m_bDiagnosticDateView = true;
            m_DiagnosticDateByteArray = diagnosticDateByteArray;
        }
        public void Disconnect()
        {
            m_licenseWrapper.DisconnectEx();
            if (m_licServer != null)
            {
                m_licServer.Disconnect();
                m_licServer = null;
            }
            
            m_ServerName = "";
        }
        public bool IsConnected()   { return (m_licServer != null); }

        //Cache for Software Licenses -
        //  string m_allSoftwareLic_Cache
        //  Generic.Dictionary<string, string> m_softwareLicByLic_Cache
        private string m_allSoftwareLic_Cache = null;
        private System.Collections.Generic.Dictionary<string, string> m_softwareLicByLic_Cache = new Dictionary<string, string>();
        public void GetAllSoftwareLicenses(ref String slStream)
        {
            GetAllSoftwareLicenses(ref slStream, true);
        }
        public void GetAllSoftwareLicenses(ref String slStream, bool bForceRefresh)
        {
            if (m_licServer == null)
            {
                throw new COMException("GetAllSoftwareLicenses Failed");
            }
            else
            {
                try
                {
                    if (m_allSoftwareLic_Cache == null || (bForceRefresh == true && this.bDiagnosticDateView == false))
                    {
                        if (this.bDiagnosticDateView)
                        {
                            try
                            {
                                string licInfoListStreamed = "";
                                string modifiedDateStreamed = "";
                                string keyAttribsListStreamed = "";
                                string usageInfoStreamed = "";
                                string eventLogListStreamed = "";
                                string connectionInfoListStreamed = "";
                                licInfoListStreamed = m_licServer.GenerateStreamData_ByLicenseSystemData(m_DiagnosticDateByteArray, ref modifiedDateStreamed, ref keyAttribsListStreamed, ref usageInfoStreamed, ref eventLogListStreamed, ref connectionInfoListStreamed);

                                Solimar.Licensing.Attribs.AttribsMemberStringList streamedKeyInfoList = new Solimar.Licensing.Attribs.AttribsMemberStringList("stringList", new System.Collections.ArrayList());
                                streamedKeyInfoList.SVal = keyAttribsListStreamed;
                                //Populate m_protectionKeyCache
                                foreach (string streamedInfo in streamedKeyInfoList.TVal)
                                {
                                    Solimar.Licensing.Attribs.Lic_KeyAttribs keyAttrib = new Solimar.Licensing.Attribs.Lic_KeyAttribs();
                                    keyAttrib.AssignMembersFromStream(streamedInfo);
                                    Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseProtectionKeyInfo wrapperKeyInfo = new Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseProtectionKeyInfo();
                                    wrapperKeyInfo.keyName = keyAttrib.keyName;

                                    //The last row contains info about key
                                    System.Collections.ArrayList layoutList = keyAttrib.layout.TVal;
                                    Solimar.Licensing.Attribs.AttribsMemberBuffer tmpBuffer = (layoutList.Count >= 8) ? (Solimar.Licensing.Attribs.AttribsMemberBuffer)layoutList[7] : null;
                                    int[] intArray = new int[8];
                                    for (int idx = 0; idx < tmpBuffer.TVal.Length / 2; idx++)
                                        intArray[idx] = Convert.ToInt32(tmpBuffer.TVal[2 * idx] * 0x100 + tmpBuffer.TVal[2 * idx + 1]);

                                    wrapperKeyInfo.keyTypeID = intArray[0];
                                    wrapperKeyInfo.productID = intArray[1];
                                    wrapperKeyInfo.version = string.Format("{0:x}.{1:x}", ((intArray[2] & 0xf000) >> 12), ((intArray[2] & 0x0ff0) >> 4));
                                    wrapperKeyInfo.keyTypeName = wrapperKeyInfo.GetKeyTypeString();
                                    wrapperKeyInfo.productName = GetProductName(wrapperKeyInfo.productID);
                                    if (wrapperKeyInfo.productName == string.Empty && wrapperKeyInfo.productID == 0xff)
                                        wrapperKeyInfo.productName = "Software Verification Key"; //hardcode
                                    wrapperKeyInfo.applicationInstance = intArray[4];
                                    if (wrapperKeyInfo.productID != 0xff)
                                    {
                                        tmpBuffer = (Solimar.Licensing.Attribs.AttribsMemberBuffer)layoutList[3];
                                        intArray = new int[8];
                                        for (int idx = 0; idx < tmpBuffer.TVal.Length / 2; idx++)
                                            intArray[idx] = Convert.ToInt32(tmpBuffer.TVal[2 * idx] * 0x100 + tmpBuffer.TVal[2 * idx + 1]);
                                        wrapperKeyInfo.hoursLeft = intArray[2];

                                        tmpBuffer = (Solimar.Licensing.Attribs.AttribsMemberBuffer)layoutList[4];
                                        intArray = new int[8];
                                        for (int idx = 0; idx < tmpBuffer.TVal.Length / 2; idx++)
                                            intArray[idx] = Convert.ToInt32(tmpBuffer.TVal[2 * idx] * 0x100 + tmpBuffer.TVal[2 * idx + 1]);

                                        wrapperKeyInfo.expirationDate = new DateTime(1970, 1, 1); //time(NULL)
                                        wrapperKeyInfo.expirationDate = (wrapperKeyInfo.expirationDate.AddSeconds((intArray[1] * 0x10000) + intArray[0])).ToLocalTime();
                                    }
                                    if (m_protectionKeyCache == null)
                                        m_protectionKeyCache = new List<Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseProtectionKeyInfo>();
                                    m_protectionKeyCache.Add(wrapperKeyInfo);
                                }
                                Solimar.Licensing.Attribs.AttribsMemberStringList streamedInfoList = new Solimar.Licensing.Attribs.AttribsMemberStringList("stringList", new System.Collections.ArrayList());
                                streamedInfoList.SVal = licInfoListStreamed;
                                Solimar.Licensing.Attribs.AttribsMemberStringList softwareLicNameList = new Solimar.Licensing.Attribs.AttribsMemberStringList("stringList", new System.Collections.ArrayList());
                                if (usageInfoStreamed.Length != 0)
                                {
                                    try { m_usageAttribs.AssignMembersFromStream(Solimar.Licensing.Attribs.XMLDocumentHelper.GetDocumentElementFromString(usageInfoStreamed).InnerText); }
                                    catch (Exception) { }
                                }
                                if(eventLogListStreamed.Length != 0)
                                {
                                    m_DiagnosticEventLogStreamed = Solimar.Licensing.Attribs.XMLDocumentHelper.GetDocumentElementFromString(eventLogListStreamed).InnerText;
                                }
                                
                                m_DiagnosticDateCreatedDate = Solimar.Licensing.Attribs.AttribFormat.ConvertStringToDateTime(modifiedDateStreamed);
                                foreach (string streamedInfo in streamedInfoList.TVal)
                                {
                                    Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs licInfoAttribs = new Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs();
                                    licInfoAttribs.AssignMembersFromStream(streamedInfo);
                                    string licInfoName = Solimar.Licensing.Attribs.Lic_LicenseInfoAttribsHelper.GenerateLicenseServerName((int)licInfoAttribs.customerID.TVal, (int)licInfoAttribs.destinationID.TVal, (int)licInfoAttribs.softwareGroupLicenseID.TVal);
                                    softwareLicNameList.TVal.Add(licInfoName);
                                    m_softwareLicByLic_Cache[licInfoName] = streamedInfo;
                                }
                                //surround xml <stringList> around the softwareLicNameList
                                m_allSoftwareLic_Cache = string.Format("<stringList>{0}</stringList>", softwareLicNameList);
                            }
                            catch (COMException)
                            {
                                //On error set m_allSoftwareLic_Cache = null so next if statement is hit
                                m_allSoftwareLic_Cache = null;
                                throw;
                            }
                        }
                        else
                            m_allSoftwareLic_Cache = m_licServer.GetAllSoftwareLicenses();
                    }

                    //if (m_allSoftwareLic_Cache == null || bForceRefresh == true)
                    //    m_allSoftwareLic_Cache = m_licServer.GetAllSoftwareLicenses();
                    slStream = m_allSoftwareLic_Cache;
                }
                catch (COMException ex)
                {
                    throw new COMException("GetAllSoftwareLicenses Failed", ex);
                }
            }
        }

        public void GetSoftwareLicenseInfoForAll(ref String slStream)
        {
            try
            {
                if (this.bDiagnosticDateView == true)
                {
                    Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs licInfo = new Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs();
                    foreach (Solimar.Licensing.Attribs.Lic_UsageInfoAttribs.Lic_UsProductInfoAttribs usProdInfo in m_usageAttribs.productList.TVal)
                    {
                        Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs prodInfo = new Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs();
                        prodInfo.productID.TVal = usProdInfo.productID.TVal;
                        prodInfo.productAppInstance.TVal = (uint)usProdInfo.appInstanceList.TVal.Count;
                        licInfo.productList.TVal.Add(prodInfo);
                    }
                    slStream = licInfo.Stream;
                }
                else
                    slStream = m_licServer.GetSoftwareLicenseInfo_ForAll();
            }
            catch (COMException ex)
            {
                throw new COMException("GetSoftwareLicenseInfoForAll Failed", ex);
            }
        }

        public void GetSoftwareLicenseInfoByLicense(String softwareLicense, ref String sliStream)
        {
            GetSoftwareLicenseInfoByLicense(softwareLicense, ref sliStream, true);
        }
        public void GetSoftwareLicenseInfoByLicense(String softwareLicense, ref String sliStream, bool bForceRefresh)
        {
            try
            {
                if (this.bDiagnosticDateView)
                    bForceRefresh = false; //Using diagnostic data, Stop force refresh
                if (m_softwareLicByLic_Cache.ContainsKey(softwareLicense) == false || bForceRefresh == true)
                    m_softwareLicByLic_Cache[softwareLicense] = m_licServer.GetSoftwareLicenseInfo_ByLicense(softwareLicense);
                sliStream = m_softwareLicByLic_Cache[softwareLicense];
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
                if (this.bDiagnosticDateView)
                {
                    foreach (Solimar.Licensing.Attribs.Lic_UsageInfoAttribs.Lic_UsProductInfoAttribs usProdInfo in m_usageAttribs.productList.TVal)
                    {
                        if (usProdInfo.productID.TVal == prodID)
                        {
                            Solimar.Licensing.Attribs.AttribsMemberStringList strList = new Solimar.Licensing.Attribs.AttribsMemberStringList("stringList", new System.Collections.ArrayList());
                            foreach (Solimar.Licensing.Attribs.Lic_UsageInfoAttribs.Lic_UsAppInstanceInfoAttribs usAppInstInfo in usProdInfo.appInstanceList.TVal)
                                strList.TVal.Add(usAppInstInfo.applicationInstance.TVal);
                            generalStream = string.Format("<strList>{0}</strList>", strList.SVal);
                            break;
                        }
                    }
                }
                else
                {
                    generalStream = m_licServer.SoftwareGetApplicationInstanceListByProduct(prodID);
                }
            }
            catch (COMException ex)
            {
                throw new COMException("SoftwareGetApplicationInstanceListByProduct Failed", ex);
            }
        }

        public void VerifyTokenByLicense(String softwareLicense, int validationTokenType, String verificationValue)
        {
            try
            {
                m_licServer.ValidateToken_ByLicense(softwareLicense, validationTokenType, verificationValue);
            }
            catch (COMException ex)
            {
                throw new COMException("VerifyTokenByLicense Failed", ex);
            }
        }

        public string GetVerifyTokenByLicense(String softwareLicense, int validationTokenType, String verificationValue, ref bool tokenVerified)
        {
            string message = "Verified";
            try
            {
                tokenVerified = false;
                m_licServer.ValidateToken_ByLicense(softwareLicense, validationTokenType, verificationValue);
                tokenVerified = true;
            }
            catch (COMException ex)
            {
                message = ex.Message;
            }
            return message;
        }

        public string GetSoftwareLicenseStatus_ByLicense(String softwareLicense, ref bool softwareLicenseValid)
        {
            try
            {
                string retVal = "";
                if (m_bDiagnosticDateView)
                {
                    softwareLicenseValid = true;
                    retVal = "Diagnostic Data";
                }
                else
                    retVal = m_licServer.GetSoftwareLicenseStatus_ByLicense(softwareLicense, ref softwareLicenseValid);
                return retVal;
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
            catch (COMException)
            {
                throw;
            }
        }
        public void EnterProtectionKeyPasswordPacket(Byte[] byteLicPacket, ref String verificationCode)
        {
            try
            {
                verificationCode = m_licServer.EnterProtectionKeyPasswordPacket(byteLicPacket);
            }
            catch (COMException)
            {
                throw;
            }
        }
        public bool EnterProtectionKeyPassword(String password)
        {
            try
            {
                return m_licServer.EnterProtectionKeyPassword(password);
            }
            catch (COMException)
            {
                throw;
            }
        }

        public void GenerateSoftwareLicPacket(String licenseAttribs, DateTime expirationDate, ref String verificationCode, ref Byte[] newByteArrayLicensePacket)
        {
            try
            {
                m_licServer.GenerateSoftwareLicPacket(licenseAttribs, expirationDate, ref verificationCode, ref newByteArrayLicensePacket);
            }
            catch (COMException)
            {
                throw;
            }
        }

        public void GenerateVerifyDataWithVerCode_ByLicense(String softwareLicense, ref Byte[] newByteArrayLicense)
        {
            try
            {
                m_licServer.GenerateVerifyDataWithVerCode_ByLicense(softwareLicense, ref newByteArrayLicense);
            }
            catch (COMException)
            {
                throw;
            }
        }
        public void GenerateLicenseSystemData(ref Byte[] newByteArrayLicense)
        {
            try
            {
                m_licServer.GenerateLicenseSystemData(ref newByteArrayLicense);
            }
            catch (COMException)
            {
                throw;
            }
        }

        public void GenerateVerifyDataWithLicInfo_ByLicense(String softwareLicense, ref Byte[] newByteArrayLicense)
        {
            try
            {
                m_licServer.GenerateVerifyDataWithLicInfo_ByLicense(softwareLicense, ref newByteArrayLicense);
            }
            catch (COMException)
            {
                throw;
            }
        }

        public void GenerateSoftwareLicArchive_ByLicense(String softwareLicense, ref Byte[] newByteArrayLicense)
        {
            try
            {
                m_licServer.GenerateSoftwareLicArchive_ByLicense(softwareLicense, ref newByteArrayLicense);
            }
            catch (COMException)
            {
                throw;
            }
        }
        public void EnterSoftwareLicArchive(Byte[] newByteArrayLicense)
        {
            try
            {
                m_licServer.EnterSoftwareLicArchive(newByteArrayLicense);
            }
            catch (COMException)
            {
                throw;
            }
        }

        public void GenerateLicPackage_BySoftwareLicPacket(Byte[] byteArray, ref String slpStream)
        {
            try
            {
                slpStream = m_licServer.GenerateLicPackage_BySoftwareLicPacket(byteArray);
            }
            catch (COMException)
            {
                throw;
            }
        }

        public void GenerateLicPackage_BySoftwareLicArchive(Byte[] byteArray, ref String slaStream)
        {
            try
            {
                slaStream = m_licServer.GenerateLicPackage_BySoftwareLicArchive(byteArray);
            }
            catch (COMException)
            {
                throw;
            }
        }

        public void SoftwareLicenseUseActivationToExtendTime_ByLicenseAndContractNumber(String softwareLicense, String contractNumber)
        {
            try
            {
                m_licServer.SoftwareLicenseUseActivationToExtendTime_ByLicenseAndContractNumber(softwareLicense, contractNumber);
            }
            catch (COMException)
            {
                throw;
            }
        }

        public string GetEventLogList_ForLicenseServer()
        {
            string streamedEventLog = string.Empty;
            try
            {
                if (this.bDiagnosticDateView == false)
                    streamedEventLog = m_licServer.GetEventLogList_ForLicenseServer();
                else
                    streamedEventLog = m_DiagnosticEventLogStreamed;
            }
            catch (COMException)
            {
                throw;
            }
            return streamedEventLog;
        }

        //Cache for Protection Key Info - m_protectionKeyCache
        private System.Collections.Generic.List<Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseProtectionKeyInfo> m_protectionKeyCache = null;
        public System.Collections.Generic.List<Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseProtectionKeyInfo> KeyEnumerate()
        {
            return KeyEnumerate(true);
        }
        public System.Collections.Generic.List<Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseProtectionKeyInfo> KeyEnumerate(bool bForceRefresh)
        {
            try
            {
                //Don't check for keys when using diagnostic data
                if (m_bDiagnosticDateView)
                {
                }
                else if (!m_bDiagnosticDateView)
                {
                    if (m_protectionKeyCache == null || bForceRefresh == true)
                        m_protectionKeyCache = m_licServer.KeyEnumerate();
                }
                return m_protectionKeyCache;
            }
            catch (COMException)
            {
                throw;
            }
        }
        public Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseProtectionKeyInfo KeyFindByName(String license)
        {
            Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseProtectionKeyInfo matchingKeyInfo = null;
            foreach (Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseProtectionKeyInfo currentKeyInfo in KeyEnumerate(false))
            {
                if (String.Compare(license, currentKeyInfo.keyName, true) == 0)
                {
                    matchingKeyInfo = currentKeyInfo;
                    break;
                }
            }
            return matchingKeyInfo;
        }

        public System.Collections.Generic.List<Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseProtectionKeyModuleInfo> GetModuleInfoList_ByLicense(String license)
        {
            return m_licServer.KeyModuleEnumerate(license);
        }

        public String KeyFormat(String key)
        {
            try
            {
                return m_licServer.KeyFormat(key);
            }
            catch (COMException)
            {
                throw;
            }
        }
        public String KeyProgramVerification(String key, long customerNumber, long keyNumber)
        {
            try
            {
                return m_licServer.KeyProgramVerification(key, (int)customerNumber, (int)keyNumber);
            }
            catch (COMException)
            {
                throw;
            }
        }
        #endregion

        #region ManagerWrapper Methods
        public void InitializeWrapper(String appID, int productID, int prodMajor, int prodMinor)
        {
            try
            {
                m_licenseWrapper.InitializeViewOnly(appID, productID, prodMajor, prodMinor);
            }
            catch (COMException ex)
            {
                throw new COMException("InitializeWrapper Failed", ex);
            }
        }

        public void GetVersionLicenseManager(ref int refVerMajor, ref int refVerMinor, ref int refVerBuild)
        {
            try
            {
                m_licenseWrapper.GetVersionLicenseManagerEx(ref refVerMajor, ref refVerMinor, ref refVerBuild);
            }
            catch (COMException ex)
            {
                throw new COMException("GetVersionLicenseManager Failed", ex);
            }
        }
        public void GetVersionLicenseServer(String licServer, ref int refVerMajor, ref int refVerMinor, ref int refVerBuild)
        {
            try
            {
                m_licenseWrapper.GetVersionLicenseServerEx(licServer, ref refVerMajor, ref refVerMinor, ref refVerBuild);
            }
            catch (COMException ex)
            {
                throw new COMException("GetVersionLicenseServer Failed", ex);
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

        public void ModuleLicenseInUse_ByApp(int modID, ref int licenseCount)
        {
            try
            {
                licenseCount = m_licenseWrapper.ModuleLicenseInUse_ByAppEx(modID);
            }
            catch (COMException ex)
            {
                throw new COMException("ModuleLicenseInUse_ByApp Failed", ex);
            }
        }

        //public void ModuleLicenseObtain(int modID, int licenseCount)
        //{
        //    try
        //    {
        //        m_licenseWrapper.ModuleLicenseObtainEx(modID, licenseCount);
        //    }
        //    catch (COMException ex)
        //    {
        //        throw new COMException("ModuleLicenseObtain Failed", ex);
        //    }
        //}

        //public void ModuleLicenseRelease(int modID, int licenseCount)
        //{
        //    try
        //    {
        //        m_licenseWrapper.ModuleLicenseReleaseEx(modID, licenseCount);
        //    }
        //    catch (COMException ex)
        //    {
        //        throw new COMException("ModuleLicenseRelease Failed", ex);
        //    }
        //}

        #endregion

        #region Helper Methods
        public string GetProductName(int productID)
        {
            return Solimar.Licensing.Attribs.Lic_LicenseInfoAttribsHelper.GetProductName(m_softwareSpec, (uint)productID);
        }

        public string GetModuleName(int productID, int moduleID)
        {
            return Solimar.Licensing.Attribs.Lic_LicenseInfoAttribsHelper.GetModuleName(m_softwareSpec, (uint)productID, (uint)moduleID);
        }

        public int GetProductID(String productName)
        {
            foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in m_softwareSpec.productSpecMap.TVal.Values)
            {
                if (string.Compare(productSpec.productName, productName, true) == 0)
                    return (int)productSpec.productID.TVal;
            }
            return -1;
        }

        public int GetModuleID(int productID, String moduleName)
        {
            foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs moduleSpec in Solimar.Licensing.Attribs.Lic_LicenseInfoAttribsHelper.GetModuleList(m_softwareSpec, (uint)productID).TVal.Values)
            {
                if (string.Compare(moduleSpec.moduleName, moduleName, true) == 0)
                    return (int)moduleSpec.moduleID.TVal;
            }
            return -1;
        }

        public void WriteEventLog(String EventLogMsg, EventLogEntryType LogType)
        {
            // source should be   created by install.
            if (EventLog.SourceExists("Solimar License Viewer"))
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
        public Exception Exception
        {
            get { return m_exception; }
            set { m_exception = value; }
        }
        #endregion

        #region Private Variables
        private Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_SoftwareSpecAttribs m_softwareSpec;
        private Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseServerWrapper m_licServer;
        private Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseWrapper m_licenseWrapper;
        private String m_ServerName;
        private bool m_bDiagnosticDateView = false;
        private DateTime? m_DiagnosticDateCreatedDate = null;
        private Byte[] m_DiagnosticDateByteArray = null;
        private Exception m_exception = null;
        private Solimar.Licensing.Attribs.Lic_UsageInfoAttribs m_usageAttribs = new Solimar.Licensing.Attribs.Lic_UsageInfoAttribs();
        private string m_DiagnosticEventLogStreamed = string.Empty;
        #endregion
        public bool bDiagnosticDateView { get { return m_bDiagnosticDateView; } }
        public DateTime? diagnosticDateCreatedDate { get { return m_DiagnosticDateCreatedDate; } }
        public Solimar.Licensing.Attribs.Lic_UsageInfoAttribs diagnosticUsageAttribs { get { return m_usageAttribs; } }

    }
}
