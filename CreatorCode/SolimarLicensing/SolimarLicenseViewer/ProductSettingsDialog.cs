using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Security.AccessControl;
using System.Xml;
using System.IO;
using Microsoft.Win32;

namespace SolimarLicenseViewer
{
    public partial class ProductSettingsDialog : Shared.VisualComponents.DialogBaseForm
    {
        public ProductSettingsDialog(CommunicationLink commlink)
        {
            InitializeComponent();

            m_CommLink = commlink;
        }

        private void ProductSettingsDialog_Load(object sender, EventArgs e)
        {
            InitializeConnSetting2UI();
            this.Text = this.Text + " For Computer: " + System.Environment.MachineName.ToLower();
        }


        /*
         * Function    : AddFileSecurity
         * Description : Gives account the supplied rights to filename
         * Parameters  : fileName - 
         *				 account  - 
         *				 rights	  -
         *				 controlType -
         */
        private void AddFileSecurity(String fileName, String account, 
                                FileSystemRights rights, AccessControlType controlType)
        {
            // Get a FileSecurity object that represents the 
            // current security settings.
            FileSecurity fSecurity = File.GetAccessControl(fileName);

            // Add the FileSystemAccessRule to the security settings. 
            fSecurity.AddAccessRule(new FileSystemAccessRule
                                           (account,rights, controlType));

            // Set the new access settings.
            File.SetAccessControl(fileName, fSecurity);
        }


        #region Form Events

        private void btnOk_Click(object sender, EventArgs e)
        {
            //save settings
            SaveSettingsToXML(m_allConnectionSettings);
        }

        #endregion


        #region Save/Load from XML
        private enum xmlVersion
        {
            eVersionUnknown = 0,
            eVersion1 = 1,
            eVersion2 = 2,
        };
        private System.Collections.Generic.Dictionary<int, ConnectionSettings2> LoadSettingsFromXML()
        {
            System.Collections.Generic.Dictionary<int, ConnectionSettings2> tmpDictionary = new Dictionary<int, ConnectionSettings2>();
            String filePath = String.Concat(Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData),
                                            AppConstants.LocalConnectionFilePath,
                                            AppConstants.LocalConnectionFile);
            if (File.Exists(filePath))
            {
                FileStream fs = new FileStream(filePath, FileMode.OpenOrCreate, FileAccess.Read, FileShare.Read);
                XmlReader reader = XmlReader.Create(fs);    // Create the XmlReader object.

                //find product section for specified id in xml
                try
                {
                    xmlVersion eXmlVer = xmlVersion.eVersionUnknown;
                    if (reader.ReadToFollowing(AppConstants.ProductSettingsXML.VersionElement))
                    {
                        string tmpValue = reader.GetAttribute(AppConstants.ProductSettingsXML.ValueAttrib);
                        if (string.Compare(tmpValue, "1.0") == 0)
                            eXmlVer = xmlVersion.eVersion1;
                        else if(string.Compare(tmpValue, "2.0") == 0)
                            eXmlVer = xmlVersion.eVersion2;
                    }

                    bool bXmlNodeFound = false;
                    #region Licensing config - Convert XML Version 1.0 to ConnectionSettings2
                    if (eXmlVer == xmlVersion.eVersion1)
                    {
                        do
                        {
                            bXmlNodeFound = reader.ReadToFollowing(AppConstants.ProductSettingsXML.ProductElement);
                            int productID = Convert.ToInt32(reader.GetAttribute(AppConstants.ProductSettingsXML.IdAttrib));

                            int idx = 0;
                            do
                            {
                                idx++;
                                bXmlNodeFound = reader.ReadToFollowing(AppConstants.ProductSettingsXML.ServerElement);
                                if (!bXmlNodeFound)
                                    continue;
                                
                                string tmpString = reader.GetAttribute(AppConstants.ProductSettingsXML.NameAttrib);
                                if (tmpString == null)
                                    continue;
                                string serverName = tmpString;//( ? "" : tmpString;   //use blank

                                tmpString = reader.GetAttribute(AppConstants.ProductSettingsXML.BackupAttrib);
                                if (tmpString == null)
                                    continue;
                                bool bBackup = (string.Compare(tmpString, "1") == 0);

                                tmpString = reader.GetAttribute(AppConstants.ProductSettingsXML.SharedAttrib);
                                if (tmpString == null)
                                    continue;
                                bool bShared = (string.Compare(tmpString, "1") == 0);

                                int tmpProductID = productID;
                                #region recalculate productID if shared==true
                                if (bShared)
                                {
                                    tmpProductID = -1;
                                    switch ((Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID)productID)
                                    {
                                        case Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_Rubika:
                                            tmpProductID = (int)Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_RubikaProcessBuilder;
                                            break;
                                        case Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_Spde:
                                            tmpProductID = (int)Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_SpdeQueueManager;
                                            break;
                                        case Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_SolIndexer:
                                            tmpProductID = (int)Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_SdxDesigner;
                                            break;
                                        default:
                                            break;
                                    }
                                }
                                #endregion
                                if (tmpProductID == -1)
                                    continue;

                                ConnectionSettings2 tmpConnSettings = null;
                                if (tmpDictionary.ContainsKey(tmpProductID))
                                    tmpConnSettings = tmpDictionary[tmpProductID];
                                else
                                    tmpConnSettings = new ConnectionSettings2();

                                if (bBackup)
                                {
                                    if (string.Compare(serverName, "localhost", true) != 0)
                                        tmpConnSettings.BackupName = serverName;
                                }
                                else
                                {
                                    tmpConnSettings.ServerName = serverName;
                                }
                                tmpConnSettings.ProductID = tmpProductID;

                                if (tmpConnSettings.IsDirty)
                                {
                                    tmpConnSettings.IsDirty = false;
                                    tmpDictionary[tmpProductID] = tmpConnSettings;
                                }
                            } while (bXmlNodeFound && idx<4);

                        } while (bXmlNodeFound);
                    }
                    #endregion
                    #region Convert Licensing config - XML Version 2.0 to ConnectionSettings2
                    else if (eXmlVer == xmlVersion.eVersion2)
                    {
                        string tmpString = null;
                        do
                        {
                            bXmlNodeFound = reader.ReadToFollowing(AppConstants.ProductSettingsXML.ProductElement);
                            if (!bXmlNodeFound)
                                continue;

                            int productID = Convert.ToInt32(reader.GetAttribute(AppConstants.ProductSettingsXML.IdAttrib));
                            bXmlNodeFound = reader.ReadToFollowing(AppConstants.ProductSettingsXML.ServerElement);
                            if (!bXmlNodeFound)
                                continue;

                            ConnectionSettings2 tmpConnSettings = new ConnectionSettings2();
                            tmpString = reader.GetAttribute(AppConstants.ProductSettingsXML.NameAttrib);
                            if (tmpString == null)
                                continue;
                            tmpConnSettings.ServerName = tmpString;

                            tmpString = reader.GetAttribute(AppConstants.ProductSettingsXML.BackupAttrib);
                            if (tmpString == null)
                                continue;
                            tmpConnSettings.BackupName = tmpString;

                            tmpString = reader.GetAttribute(AppConstants.ProductSettingsXML.TestDevAttrib);
                            if (tmpString == null)
                                continue;
                            tmpConnSettings.UseDevelopmentLic = String.Compare(tmpString, "1") == 0;
                            tmpConnSettings.ProductID = productID;

                            tmpConnSettings.IsDirty = false;
                            tmpDictionary.Add(productID, tmpConnSettings);

                        } while (bXmlNodeFound);
                    }
                    #endregion
                }
                catch (Exception)
                {
                }
                reader.Close();
                fs.Close();
            }
            return tmpDictionary;
        }

        //returns false if there is a problem saving to XML
        private bool SaveSettingsToXML(System.Collections.Generic.Dictionary<int, ConnectionSettings2> _dictionaryOfConnectionSettings)
        {
            bool bRetVal = false;
            String path = String.Concat(Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData), AppConstants.LocalConnectionFilePath, "");
            DirectoryInfo di = new DirectoryInfo(path);
            try
            {
		        if(di.Exists == false)
			        di.Create();
            }
            catch (Exception)
            {
                throw;
            }
            String filePath = String.Concat(path, AppConstants.LocalConnectionFile);
            String tempFilePath = String.Concat(Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData),
                                                AppConstants.LocalConnectionFilePath,
                                                "temp.xml");
            FileStream fsWrite = new FileStream(tempFilePath, FileMode.OpenOrCreate, FileSystemRights.Modify, FileShare.ReadWrite, 8, FileOptions.None);
            XmlWriterSettings settings = new XmlWriterSettings();
            settings.Indent = true;
            XmlWriter writer = XmlWriter.Create(fsWrite, settings);


            writer.WriteStartElement(AppConstants.ProductSettingsXML.LicensingStructureElement);  //LicensingStructure Element

            writer.WriteStartElement(AppConstants.ProductSettingsXML.VersionElement);   //Version Element
            writer.WriteAttributeString(AppConstants.ProductSettingsXML.ValueAttrib, "2.0");
            writer.WriteEndElement();   //Close Version Element

            writer.WriteStartElement(AppConstants.ProductSettingsXML.ServerStructureElement);   //ServerStructure Element

            if (_dictionaryOfConnectionSettings != null)
            {
                foreach (int dictKey in _dictionaryOfConnectionSettings.Keys)
                {
                    writer.WriteStartElement(AppConstants.ProductSettingsXML.ProductElement);   //Product Element
                    writer.WriteAttributeString(AppConstants.ProductSettingsXML.IdAttrib, dictKey.ToString());

                    writer.WriteStartElement(AppConstants.ProductSettingsXML.ServerElement);    //Server Element
                    writer.WriteAttributeString(AppConstants.ProductSettingsXML.NameAttrib, _dictionaryOfConnectionSettings[dictKey].ServerName);
                    writer.WriteAttributeString(AppConstants.ProductSettingsXML.BackupAttrib, _dictionaryOfConnectionSettings[dictKey].BackupName);
                    writer.WriteAttributeString(AppConstants.ProductSettingsXML.TestDevAttrib, _dictionaryOfConnectionSettings[dictKey].UseDevelopmentLic == true ? "1" : "0");
                    writer.WriteEndElement();       //Close Server Element
                    
                    writer.WriteFullEndElement();   //Close Product Element
                }
            }
            writer.WriteFullEndElement();   //Close ServerStructure Element

            writer.WriteFullEndElement();   //Close Licensing Structure Element

            
            writer.Flush();
            writer.Close();
            fsWrite.Close();

            File.Delete(filePath);
            File.Move(tempFilePath, filePath);
            //give everyone rights to new file. Should surround with try/catch incase of failure.
            AddFileSecurity(filePath,
                            "Everyone",
                            FileSystemRights.FullControl,
                            AccessControlType.Allow);
            bRetVal = true;
            return bRetVal;
        }
        #endregion

        #region Section for use of UI

        private void InitializeConnSetting2UI()
        {
            m_allConnectionSettings = LoadSettingsFromXML();

            System.Collections.Generic.List<int> installedProductsList = new List<int>();
            #region Read Registry for installed products
            using (RegistryKey rkey = Registry.LocalMachine.OpenSubKey(AppConstants.SolimarRegKey))
            {
                foreach (String productName in rkey.GetSubKeyNames())
                {
                    if (!productName.Equals(AppConstants.LicenseProduct))
                    {
                        int productID = m_CommLink.GetProductID(productName);
                        if (productID >= 0)
                        {
                            installedProductsList.Add(productID);

                            //Also add secondary product too...
                            if (productID == (int)Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_Rubika)
                                installedProductsList.Add((int)Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_RubikaProcessBuilder);
                            else if (productID == (int)Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_Spde)
                                installedProductsList.Add((int)Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_SpdeQueueManager);
                            else if (productID == (int)Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_SolIndexer)
                                installedProductsList.Add((int)Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_SdxDesigner);
                        }
                    }
                }
                rkey.Close();
            }
            #endregion

            //Ensure that all installed products are in m_allConnectionSettings
            foreach (int productID in installedProductsList)
            {
                if (m_allConnectionSettings.ContainsKey(productID) == false)
                {
                    ConnectionSettings2 newConnSettings = new ConnectionSettings2();
                    newConnSettings.ServerName = "localhost";
                    newConnSettings.ProductID = productID;
                    m_allConnectionSettings[productID] = newConnSettings;
                }
            }

            foreach (int productID in m_allConnectionSettings.Keys)
            {
                if (installedProductsList.Contains(productID))  //Only display installed products
                {
                    UpdateListView(m_allConnectionSettings[productID]);
                }
            }
            if (multiProductListView.Items.Count > 0)
                multiProductListView.Items[0].Selected = true;
            btnOk.Enabled = false;
        }
        ///key into ListView where _productID is unique, display_connectionSettings.  If _productID is new then add
        ///_connectionSettings.  If _productID is already there, update existing item.
        private void UpdateListView(ConnectionSettings2 _connectionSettings)
        {
            multiProductListView.BeginUpdate();

            ListViewItem matchingLvi = null;
            foreach (ListViewItem lvi in multiProductListView.Items)
            {
                if ((lvi.Tag != null) && ((int)lvi.Tag) == _connectionSettings.ProductID)
                {
                    matchingLvi = lvi;
                    break;
                }

            }
            if (matchingLvi == null)    //New item
            {
                matchingLvi = new ListViewItem();
                matchingLvi.Text = m_CommLink.GetProductName(_connectionSettings.ProductID);
                matchingLvi.Tag = _connectionSettings.ProductID;
                matchingLvi.SubItems.Add(_connectionSettings.ServerName);
                matchingLvi.SubItems.Add(_connectionSettings.BackupName);
                matchingLvi.SubItems.Add(_connectionSettings.UseDevelopmentLic.ToString());
                multiProductListView.Items.Add(matchingLvi);
            }
            else //Update Existing item
            {
                matchingLvi.Text = m_CommLink.GetProductName(_connectionSettings.ProductID);
                matchingLvi.SubItems[1].Text = _connectionSettings.ServerName;
                matchingLvi.SubItems[2].Text = _connectionSettings.BackupName;
                matchingLvi.SubItems[3].Text = _connectionSettings.UseDevelopmentLic.ToString();
            }
            multiProductListView.EndUpdate();
        }
        private void multiProductListView_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (multiProductListView.SelectedItems.Count > 0)
            {
                System.Collections.Generic.List<object> objList = new List<object>();
                foreach (ListViewItem lvi in multiProductListView.SelectedItems)
                {
                    if ((int)lvi.Tag != -1)
                        objList.Add(m_allConnectionSettings[(int)lvi.Tag]);
                }
                singleProdPropertyGrid.SelectedObjects = objList.ToArray();
            }
            else
            {
                singleProdPropertyGrid.SelectedObjects = null;
            }
        }

        private void singleProdPropertyGrid_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {
            btnOk.Enabled = true;
            foreach (ConnectionSettings2 connSettings in singleProdPropertyGrid.SelectedObjects)
            {
                if(connSettings.IsDirty)
                    UpdateListView(connSettings);
            }
        }

        #endregion

        private CommunicationLink m_CommLink;
        System.Collections.Generic.Dictionary<int/*productID*/, ConnectionSettings2> m_allConnectionSettings;
    }
}

