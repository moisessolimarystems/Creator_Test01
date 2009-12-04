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
                //remove " Text/Dev" from key names...
                string cleanKey = key.Replace("Test/Dev ", "");
                iconIndex = (IconList)Enum.Parse(typeof(IconList), cleanKey.Replace(" ", ""), true);
                //iconIndex = (IconList)Enum.Parse(typeof(IconList), key.Replace(" ", ""), true);
            }
            catch(Exception)
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
                if (strList.TVal.Count == 0)
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
                        #region foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs prodInfo in licInfoAttrib.productList.TVal)

                        foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs prodInfo in licInfoAttrib.productList.TVal)
                        {
                            productName = m_CommLink.GetProductName(System.Convert.ToInt32(prodInfo.productID.ToString(), 16));

                            TreeNode prodNode = FindChildTreeNode(swLicNode, productName);
                            bool bNewNode = (prodNode == null);

                            if (bNewNode == true)
                            {
                                prodNode = new TreeNode(productName);
                                prodNode.ImageIndex = GetIconIndex(productName);
                                prodNode.SelectedImageIndex = prodNode.ImageIndex;
                                prodNode.Name = swLicNode.Name + prodNode.Text;
                                prodNode.ForeColor = swLicNode.ForeColor;
                            }

                            toolTipBuilder = new StringBuilder();
                            toolTipBuilder.Append("Product: ");
                            toolTipBuilder.Append(productName);

                            toolTipBuilder.Append("\n");
                            toolTipBuilder.Append(SolimarLicenseViewer.AppConstants.VersionHeader);
                            toolTipBuilder.Append(": ");
                            toolTipBuilder.Append(prodInfo.product_Major.TVal.ToString());
                            toolTipBuilder.Append(".");
                            toolTipBuilder.Append(prodInfo.product_Minor.TVal.ToString());
                            if (prodInfo.product_SubMajor != 0 && prodInfo.product_SubMinor != 0)
                            {
                                toolTipBuilder.Append(".");
                                toolTipBuilder.Append(prodInfo.product_SubMajor.TVal.ToString());
                                toolTipBuilder.Append(".");
                                toolTipBuilder.Append(prodInfo.product_SubMinor.TVal.ToString());
                            }

                            toolTipBuilder.Append("\n");
                            toolTipBuilder.Append(SolimarLicenseViewer.AppConstants.AppInstanceHeader);
                            toolTipBuilder.Append(": ");
                            toolTipBuilder.Append(prodInfo.productAppInstance.TVal.ToString());

                            prodNode.ToolTipText = toolTipBuilder.ToString();
                            if (bNewNode == true)
                                InsertIntoTree_Alpha(swLicNode, prodNode);
                        }
                        #endregion

                        toolTipBuilder = new StringBuilder();
                        toolTipBuilder.Append("License: ");
                        toolTipBuilder.Append(softwareLicense);

                        toolTipBuilder.Append("\n");
                        toolTipBuilder.Append(SolimarLicenseViewer.AppConstants.LicenceTypeHeader);
                        toolTipBuilder.Append(": ");
                        toolTipBuilder.Append(licInfoAttrib.softwareLicType.GetAlias());

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
                rootNode = new TreeNode(SolimarLicenseViewer.AppConstants.UsageRootNode);
                rootNode.ImageIndex = GetIconIndex("Usage");
                rootNode.SelectedImageIndex = rootNode.ImageIndex;
                rootNode.Name = rootNode.Text;
                try
                {
                    String productName;
                    String generalStream = "";
                    m_CommLink.GetSoftwareLicenseInfoForAll(ref generalStream);
                    Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs licInfoAttrib = new Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs();
                    licInfoAttrib.AssignMembersFromStream(generalStream);

                    bool bFoundAtleastOneProduct = false;
                    StringBuilder toolTipBuilder = null;
                    foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs prodInfo in licInfoAttrib.productList.TVal)
                    {
                        //add a product node if there are app instances
                        if (prodInfo.productID.ToString() != "0")
                        {
                            m_CommLink.SoftwareGetApplicationInstanceListByProduct(System.Convert.ToInt32(prodInfo.productID.ToString(), 16), ref generalStream);
                            Solimar.Licensing.Attribs.AttribsMemberStringList appList = new Solimar.Licensing.Attribs.AttribsMemberStringList("stringList", new System.Collections.ArrayList());
                            appList.SVal = generalStream;
                            if (appList.TVal.Count > 0)
                            {
                                productName = m_CommLink.GetProductName(System.Convert.ToInt32(prodInfo.productID.ToString(), 16));
                                TreeNode prodNode = new TreeNode(productName);
                                prodNode.ImageIndex = GetIconIndex(productName);
                                prodNode.SelectedImageIndex = prodNode.ImageIndex;
                                prodNode.Name = prodNode.Text;
                                toolTipBuilder = new StringBuilder();
                                toolTipBuilder.Append(AppConstants.UsageProductHeader);
                                toolTipBuilder.Append(": ");
                                toolTipBuilder.Append(productName);
                                prodNode.ToolTipText = toolTipBuilder.ToString();
                                foreach (string appInstance in appList.TVal)
                                {
                                    TreeNode appInstNode = new TreeNode(appInstance);
                                    appInstNode.ImageIndex = GetIconIndex("AppInstance");
                                    appInstNode.SelectedImageIndex = appInstNode.ImageIndex;
                                    appInstNode.Name = appInstNode.Text;
                                    toolTipBuilder = new StringBuilder();
                                    toolTipBuilder.Append(AppConstants.UsageProductHeader);
                                    toolTipBuilder.Append(": ");
                                    toolTipBuilder.Append(productName);
                                    toolTipBuilder.Append("\n");
                                    toolTipBuilder.Append(AppConstants.UsageAppInstanceHeader);
                                    toolTipBuilder.Append(": ");
                                    toolTipBuilder.Append(appInstance);
                                    appInstNode.ToolTipText = toolTipBuilder.ToString();
                                    prodNode.Nodes.Add(appInstNode);
                                }
                                rootNode.Nodes.Add(prodNode);
                                bFoundAtleastOneProduct = true;
                            }
                        }
                    }
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
            if(this.TheTreeView.SelectedNode != null)
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

            tmpNode = GenerateProductConnectionSettingsNode();
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
