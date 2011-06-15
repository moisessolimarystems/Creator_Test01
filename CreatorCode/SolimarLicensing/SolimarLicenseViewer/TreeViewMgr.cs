using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Drawing;

namespace SolimarLicenseViewer
{
    /// <summary>
    // Implements the manager handling the TreeView of the main form
    /// </summary>
    class TreeViewMgr
    {
        enum IconList
        {
            SPD,
            ICONVERT,
            RUBIKA,
            SDXDESIGNER,
            SOLSEARCHERENTERPRISE,
            SOLFUSION,
            SOLSCRIPT,
            SPDE,
            SPDEQUEUEMANAGER,
            SPDESYSTEMMANAGER,
            LICENSES,
            LICENSEID,
            APPINSTANCE,
            HISTORY,
            EMPTY,
            PROTECTIONKEYS,
            PROTECTIONKEY,
            RUBIKAPROCESSBUILDER,
            SOLINDEXER,
            USAGE,
            PRODUCTCONNECTIONSETTINGS,
            SOLITRACK,
            SOLSEARCHERENTERPRISESINGLEPLATFORM,
            LIBRARYSERVICES,
            EVENTLOG,
        }


        #region Constructor
        /// <summary>
        /// Class constructor.  Initializes the TreeView to manage and the link to the License Server/Wrapper
        /// </summary>
        public TreeViewMgr(TreeView lv, CommunicationLink commlink)
        {
            TheTreeView = lv;
            m_CommLink = commlink;
        }
        #endregion

        #region Form Methods

        private int GetIconIndex(String key)
        {
            IconList iconIndex;
            try
            {
                //remove " Text/Dev/DR" from key names...
                string cleanKey = key.Replace("Test/Dev/DR ", "");
                iconIndex = (IconList)Enum.Parse(typeof(IconList), cleanKey.Replace(" ", ""), true);
                //iconIndex = (IconList)Enum.Parse(typeof(IconList), key.Replace(" ", ""), true);
            }
            catch (Exception)
            {
                return -1;
            }
            return (int)iconIndex;
        }

        #endregion

        #region License Node
        private TreeNode GenerateLicenseNode()
        {
            TreeNode rootNode = null;
            try
            {
                rootNode = new TreeNode(AppConstants.LicenseRootNode);
                rootNode.ImageIndex = GetIconIndex("Licenses");
                rootNode.SelectedImageIndex = rootNode.ImageIndex;
                rootNode.Name = rootNode.Text;
                String productName;
                String generalStream = "";
                m_CommLink.GetAllSoftwareLicenses(ref generalStream);
                Solimar.Licensing.Attribs.AttribsMemberStringList strList = new Solimar.Licensing.Attribs.AttribsMemberStringList("stringList", new System.Collections.ArrayList());
                strList.SVal = generalStream;

                //No software Licensing, hide node...
                if (strList.TVal.Count == 0 && !m_CommLink.bDiagnosticDateView)
                {
                    rootNode = null;
                }
                else
                {
                    StringBuilder toolTipBuilder = null;
                    #region foreach (string softwareLicense in strList.TVal)
                    foreach (string softwareLicense in strList.TVal)
                    {
                        m_CommLink.GetSoftwareLicenseInfoByLicense(softwareLicense, ref generalStream);
                        Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs licInfoAttrib = new Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs();
                        licInfoAttrib.AssignMembersFromStream(generalStream);
                        TreeNode swLicNode = new TreeNode(softwareLicense);
                        swLicNode.ImageIndex = GetIconIndex("LicenseID");
                        swLicNode.SelectedImageIndex = swLicNode.ImageIndex;
                        swLicNode.Name = swLicNode.Text;

                        bool bLicValid = false;
                        if (m_CommLink.bDiagnosticDateView)
                        {
                            if (licInfoAttrib.diagDataErr.TVal == 0xffffff) //Uninitialized License Status
                            {
                                swLicNode.Text = string.Format("{0} Diagnostic Data (Status Unknown)", softwareLicense);
                                swLicNode.ForeColor = System.Drawing.Color.Black;
                            }
                            else if (licInfoAttrib.diagDataErr.TVal == 0) //No Error
                            {
                                swLicNode.Text = string.Format("{0} Diagnostic Data ({1})", softwareLicense, SolimarLicenseViewer.AppConstants.VerifiedStatus);
                                swLicNode.ForeColor = System.Drawing.Color.Black;
                            }
                            else //License Error
                            {
                                swLicNode.Text = string.Format("{0} Diagnostic Data - ({1})", softwareLicense, licInfoAttrib.diagDataErrMsg.TVal);
                                swLicNode.ForeColor = System.Drawing.Color.Red;
                            }
                        }
                        else
                        {
                            string errorMessage = m_CommLink.GetSoftwareLicenseStatus_ByLicense(softwareLicense, ref bLicValid);
                            if (bLicValid == true)
                            {
                                swLicNode.Text = softwareLicense + " (" + SolimarLicenseViewer.AppConstants.VerifiedStatus + ")";
                                swLicNode.ForeColor = System.Drawing.Color.Black;
                            }
                            else
                            {
                                swLicNode.Text = softwareLicense + " (" + errorMessage + ")";
                                swLicNode.ForeColor = System.Drawing.Color.Red;
                            }
                        }
                        #region foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs prodInfo in licInfoAttrib.productList.TVal)

                        foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs prodInfo in licInfoAttrib.productList.TVal)
                        {
                            productName = m_CommLink.GetProductName((int)prodInfo.productID.TVal);

                            TreeNode prodNode = FindChildTreeNode(swLicNode, productName);
                            bool bNewNode = (prodNode == null);

                            if (bNewNode == true)
                            {
                                if (string.IsNullOrEmpty(productName))
                                    productName = string.Format("Unknown Product {0}", (int)prodInfo.productID.TVal);
                                prodNode = new TreeNode(productName);
                                prodNode.ImageIndex = GetIconIndex(productName);
                                prodNode.SelectedImageIndex = prodNode.ImageIndex;
                                prodNode.Name = swLicNode.Name + prodNode.Text;
                                prodNode.ForeColor = swLicNode.ForeColor;
                                prodNode.Tag = new object[] 
                                { 
                                    prodInfo.productAppInstance.TVal, 
                                    prodInfo.product_Major.TVal,
                                    prodInfo.product_Minor.TVal,
                                    prodInfo.product_SubMajor.TVal,
                                    prodInfo.product_SubMinor.TVal,
                                    productName
                                };
                                InsertIntoTree_Alpha(swLicNode, prodNode);
                            }
                            else
                            {
                                object[] tmpObjList = (object[])prodNode.Tag;
                                tmpObjList[0] = (uint)tmpObjList[0] + prodInfo.productAppInstance.TVal;
                            }
                        }

                        //Cycle through all the tree nodes at this level, and generate tool tip.  must generate at the end because tag stores the 
                        //number of application instances seen the whole time.
                        foreach (TreeNode tNode in swLicNode.Nodes)
                        {
                            object[] tmpObjList = (object[])tNode.Tag;

                            toolTipBuilder = new StringBuilder();
                            toolTipBuilder.Append("Product: ");
                            toolTipBuilder.Append(tmpObjList[5]);

                            toolTipBuilder.Append("\n");
                            toolTipBuilder.Append(SolimarLicenseViewer.AppConstants.VersionHeader);
                            toolTipBuilder.Append(": ");
                            toolTipBuilder.Append(string.Format("{0}.{1}", tmpObjList[1], tmpObjList[2]));
                            if ((uint)tmpObjList[3] != 0 && (uint)tmpObjList[4] != 0)
                                toolTipBuilder.Append(string.Format(".{0}.{1}", tmpObjList[3], tmpObjList[4]));

                            toolTipBuilder.Append("\n");
                            toolTipBuilder.Append(SolimarLicenseViewer.AppConstants.AppInstanceHeader);
                            toolTipBuilder.Append(": ");
                            toolTipBuilder.Append(tmpObjList[0].ToString());

                            tNode.ToolTipText = toolTipBuilder.ToString();

                        }

                        #endregion

                        toolTipBuilder = new StringBuilder();
                        toolTipBuilder.Append("License: ");
                        toolTipBuilder.Append(swLicNode.Text);

                        //toolTipBuilder.Append("\n");
                        //toolTipBuilder.Append(SolimarLicenseViewer.AppConstants.VerificationStatusHeader);
                        //toolTipBuilder.Append(": ");
                        //toolTipBuilder.Append(bLicValid ? SolimarLicenseViewer.AppConstants.VerifiedStatus : errorMessage);

                        swLicNode.ToolTipText = toolTipBuilder.ToString();
                        InsertIntoTree_Alpha(rootNode, swLicNode);
                    }
                    #endregion
                    if (rootNode.Nodes.Count > 0)
                    {
                        TreeNode historyNode = new TreeNode(AppConstants.HistoryNode);
                        historyNode.ImageIndex = GetIconIndex("History");
                        historyNode.SelectedImageIndex = historyNode.ImageIndex;
                        historyNode.Name = historyNode.Text;
                        rootNode.Nodes.Add(historyNode);
                    }
                }
            }
            catch (COMException /*ex*/)
            {
                rootNode = null;
            }
            return rootNode;
        }
        #endregion

        #region Protection Key Node
        private TreeNode GenerateProtectionKeyNode()
        {
            TreeNode protectionKeyNode = null;
            if (m_CommLink.Exception == null)
            {
                protectionKeyNode = new TreeNode(SolimarLicenseViewer.AppConstants.ProtectionKeyRootNode);
                protectionKeyNode.ImageIndex = GetIconIndex("protectionKeys");
                protectionKeyNode.SelectedImageIndex = protectionKeyNode.ImageIndex;
                protectionKeyNode.Name = protectionKeyNode.Text;
            }
            return protectionKeyNode;
        }
        #endregion

        #region Usage Node
        /// <summary>
        /// This method populates the TreeView with Usage information.
        /// Information is displayed on 2 tree levels. Application Instances, and Modules.
        /// </summary>
        private TreeNode GenerateUsageNode()
        {
            TreeNode rootNode = null;
            if (m_CommLink.Exception == null)
            {
                rootNode = new TreeNode(string.Format("{0}{1}", SolimarLicenseViewer.AppConstants.UsageRootNode, m_CommLink.bDiagnosticDateView == true ? string.Format(" at {0}", m_CommLink.diagnosticDateCreatedDate.Value.ToLocalTime().ToString()) : ""));
                rootNode.ImageIndex = GetIconIndex("Usage");
                rootNode.SelectedImageIndex = rootNode.ImageIndex;
                rootNode.Name = SolimarLicenseViewer.AppConstants.UsageRootNode;
                rootNode.ToolTipText = rootNode.Text;

                try
                {
                    String productName;
                    String generalStream = "";
                    m_CommLink.GetSoftwareLicenseInfoForAll(ref generalStream);
                    Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs licInfoAttrib = new Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs();
                    licInfoAttrib.AssignMembersFromStream(generalStream);

                    bool bFoundAtleastOneProduct = false;
                    StringBuilder toolTipBuilder = null;
                    #region foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs prodInfo in licInfoAttrib.productList.TVal)
                    foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs prodInfo in licInfoAttrib.productList.TVal)
                    {
                        //add a product node if there are app instances
                        if (prodInfo.productID.ToString() != "0")
                        {
                            System.Collections.Generic.Dictionary<string, bool?> usageMap = m_CommLink.GetAppInstToUsageMap_ByProduct((int)prodInfo.productID.TVal);

                            if (usageMap.Count > 0)
                            {
                                productName = m_CommLink.GetProductName((int)prodInfo.productID.TVal);
                                TreeNode prodNode = new TreeNode(productName);
                                prodNode.ImageIndex = GetIconIndex(productName);
                                prodNode.SelectedImageIndex = prodNode.ImageIndex;
                                prodNode.Name = prodNode.Text;
                                toolTipBuilder = new StringBuilder();
                                toolTipBuilder.Append(AppConstants.UsageProductHeader);
                                toolTipBuilder.Append(": ");
                                toolTipBuilder.Append(productName);
                                prodNode.ToolTipText = toolTipBuilder.ToString();

                                #region foreach (System.Collections.Generic.KeyValuePair<string, bool?> usagePair in usageMap)
                                foreach (System.Collections.Generic.KeyValuePair<string, bool?> usagePair in usageMap)
                                {
                                    //string connectionType = "Unknown: ";
                                    //if (usagePair.Value.HasValue)
                                    //    connectionType = usagePair.Value.Value ? "Primary: " : "Failover: ";
                                    string connectionType = (usagePair.Value.HasValue && (usagePair.Value.Value == false)) ? "Failover: " : "";
                                    TreeNode appInstNode = new TreeNode(string.Format("{0}{1}", connectionType, usagePair.Key));
                                    appInstNode.ImageIndex = GetIconIndex("AppInstance");
                                    appInstNode.SelectedImageIndex = appInstNode.ImageIndex;
                                    appInstNode.Name = usagePair.Key;   //Name is the Application Instance Name
                                    toolTipBuilder = new StringBuilder();
                                    toolTipBuilder.Append(AppConstants.UsageProductHeader);
                                    toolTipBuilder.Append(": ");
                                    toolTipBuilder.Append(productName);
                                    toolTipBuilder.Append("\n");
                                    toolTipBuilder.Append(AppConstants.UsageAppInstanceHeader);
                                    toolTipBuilder.Append(": ");
                                    toolTipBuilder.Append(usagePair.Key);
                                    if((usagePair.Value.HasValue && (usagePair.Value.Value == false)))
                                        toolTipBuilder.Append("\nFailover");
                                    appInstNode.ToolTipText = toolTipBuilder.ToString();
                                    prodNode.Nodes.Add(appInstNode);
                                }
                                #endregion
                                rootNode.Nodes.Add(prodNode);
                                bFoundAtleastOneProduct = true;
                            }
                        }
                    }
                    #endregion
                    if (!bFoundAtleastOneProduct)   //Remove Root node if no products...
                        rootNode = null;
                }
                catch (COMException)
                {
                }
            }
            return rootNode;
        }
        #endregion

        #region Event Log Node
        private TreeNode GenerateEventLogNode()
        {
            TreeNode rootNode = null;
            rootNode = new TreeNode(SolimarLicenseViewer.AppConstants.EventLogRootNode);
            rootNode.ImageIndex = GetIconIndex("eventLog");
            rootNode.SelectedImageIndex = rootNode.ImageIndex;
            rootNode.Name = SolimarLicenseViewer.AppConstants.EventLogRootNode;
            //rootNode.ToolTipText = rootNode.Text;
            return rootNode;
        }
        #endregion

        #region Product Connection Settings Node
        private TreeNode GenerateProductConnectionSettingsNode()
        {
            TreeNode rootNode = new TreeNode(SolimarLicenseViewer.AppConstants.ProductConnectionSettingsRootNode);
            rootNode.ImageIndex = GetIconIndex("Product Connection Settings");
            rootNode.SelectedImageIndex = rootNode.ImageIndex;
            rootNode.Name = rootNode.Text;
            return rootNode;
        }
        #endregion

        /// <summary>
        /// This method populates the TreeView with License, Protection Key, Usage and Product Connection information.
        /// Information is displayed on 2 tree levels. Application Instances, and Modules.
        /// </summary>
        public void PopulateView()
        {
            string selectedNodeText = "";
            if (this.TheTreeView.SelectedNode != null)
                selectedNodeText = this.TheTreeView.SelectedNode.Text;
            this.TheTreeView.Nodes.Clear();

            TreeNode tmpNode = null;
            tmpNode = GenerateLicenseNode();
            if (tmpNode != null)
                this.TheTreeView.Nodes.Add(tmpNode);

            tmpNode = GenerateProtectionKeyNode();
            if (tmpNode != null)
                this.TheTreeView.Nodes.Add(tmpNode);

            tmpNode = GenerateUsageNode();
            if (tmpNode != null)
                this.TheTreeView.Nodes.Add(tmpNode);

            if (!m_CommLink.bDiagnosticDateView)
            {
                tmpNode = GenerateProductConnectionSettingsNode();
                if (tmpNode != null)
                    this.TheTreeView.Nodes.Add(tmpNode);
            }

            tmpNode = GenerateEventLogNode();
            if (tmpNode != null)
                this.TheTreeView.Nodes.Add(tmpNode);

            this.TheTreeView.ShowRootLines = this.TheTreeView.Nodes.Count > 1;
            this.TheTreeView.ExpandAll();
        }

        /// <summary>
        /// This method inserts the newChildTreeNode into the parentTreeNode in Alpha Order
        /// </summary>
        void InsertIntoTree_Alpha(TreeNode parentTreeNode, TreeNode newChildTreeNode)
        {
            int insertIdx = 0;
            while (insertIdx < parentTreeNode.Nodes.Count)
            {
                if (parentTreeNode.Nodes[insertIdx].Text.CompareTo(newChildTreeNode.Text) > 0)
                    break;
                insertIdx++;
            }
            parentTreeNode.Nodes.Insert(insertIdx, newChildTreeNode);
        }
        TreeNode FindChildTreeNode(TreeNode _parentTreeNode, string _nodeText)
        {
            TreeNode foundNode = null;
            if (_parentTreeNode != null && _nodeText != string.Empty)
            {
                foreach (TreeNode childNode in _parentTreeNode.Nodes)
                {
                    if (string.Compare(childNode.Text, _nodeText, true) == 0)
                    {
                        foundNode = childNode;
                        break;
                    }
                }
            }
            return foundNode;
        }

        #region Private Variables
        /// <summary>
        /// Specifies the TreeView object from the main form
        /// </summary>
        private TreeView TheTreeView;
        /// <summary>
        /// Specifies the communication link to the license server/wrapper
        /// </summary>
        private CommunicationLink m_CommLink;
        #endregion
    }
}
