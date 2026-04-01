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
            PROTECTIONKEY
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
                string cleanKey = key.Replace(" Test/Dev", "");
                iconIndex = (IconList)Enum.Parse(typeof(IconList), cleanKey.Replace(" ", ""), true);
                //iconIndex = (IconList)Enum.Parse(typeof(IconList), key.Replace(" ", ""), true);
            }
            catch(Exception ex)
            {
                return -1;
            }
            return (int)iconIndex;
        }

        #endregion

        #region License View
        /// <summary>
        /// This method populates the TreeView with License information including the packet history.
        /// Contains Packet, Product, and Module License attributes.
        /// </summary>
        public void PopulateLicenseView()
        {
            try
            {
                String productName;
                String generalStream = ""; 
                m_CommLink.GetAllSoftwareLicenses(ref generalStream);
                this.TheTreeView.Nodes.Clear();
                Solimar.Licensing.Attribs.AttribsMemberStringList strList = new Solimar.Licensing.Attribs.AttribsMemberStringList("stringList", new System.Collections.ArrayList());
                strList.SVal = generalStream;
                TreeNode rootNode = new TreeNode(AppConstants.LicenseRootNode);
                rootNode.ImageIndex = GetIconIndex("Licenses");
                rootNode.SelectedImageIndex = rootNode.ImageIndex;
                rootNode.Name = rootNode.Text;
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
                    foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs prodInfo in licInfoAttrib.productList.TVal)
                    {
                        productName = m_CommLink.GetProductName(System.Convert.ToInt32(prodInfo.productID.ToString(), 16));
                        TreeNode prodNode = new TreeNode(productName);
                        prodNode.ImageIndex = GetIconIndex(productName);
                        prodNode.SelectedImageIndex = prodNode.ImageIndex;
                        prodNode.Name = swLicNode.Name + prodNode.Text;
                        prodNode.ForeColor = swLicNode.ForeColor;
                        InsertIntoTree_Alpha(swLicNode, prodNode);
                    }
                    InsertIntoTree_Alpha(rootNode, swLicNode);
                }
                if (rootNode.Nodes.Count > 0)
                {
                    this.TheTreeView.Nodes.Add(rootNode);
                    TreeNode historyNode = new TreeNode(AppConstants.HistoryNode);
                    historyNode.ImageIndex = GetIconIndex("History");
                    historyNode.SelectedImageIndex = historyNode.ImageIndex;
                    historyNode.Name = historyNode.Text;
                    rootNode.Nodes.Add(historyNode);
                }
                

                TreeNode protectionKeyNode = new TreeNode(SolimarLicenseViewer.AppConstants.ProtectionKeyRootNode);
                protectionKeyNode.ImageIndex = GetIconIndex("protectionKeys");
                protectionKeyNode.SelectedImageIndex = protectionKeyNode.ImageIndex;

                this.TheTreeView.Nodes.Add(protectionKeyNode);

                this.TheTreeView.ExpandAll();
            }
            catch (COMException ex)
            {

            }
        }

        /// <summary>
        /// This method inserts the newChildTreeNode into the parentTreeNode in Alpha Order
        /// </summary>
        void InsertIntoTree_Alpha(TreeNode parentTreeNode, TreeNode newChildTreeNode)
        {
            int insertIdx = 0;
            while(insertIdx < parentTreeNode.Nodes.Count)
            {
                if (parentTreeNode.Nodes[insertIdx].Text.CompareTo(newChildTreeNode.Text) > 0)
                    break;
                insertIdx++;
            }
            parentTreeNode.Nodes.Insert(insertIdx, newChildTreeNode);
        }
        #endregion

        #region Usage View
        /// <summary>
        /// This method populates the TreeView with Usage information.
        /// Information is displayed on 2 tree levels. Application Instances, and Modules.
        /// </summary>
        public void PopulateUsageView()
        {
            try
            {
                String productName;
                String generalStream = "";
                this.TheTreeView.Nodes.Clear();
                m_CommLink.GetSoftwareLicenseInfoForAll(ref generalStream);
                Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs licInfoAttrib = new Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs();
                licInfoAttrib.AssignMembersFromStream(generalStream);                            
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
                            foreach (string appInstance in appList.TVal)
                            {
                                TreeNode appInstNode = new TreeNode(appInstance);
                                appInstNode.ImageIndex = GetIconIndex("AppInstance");
                                appInstNode.SelectedImageIndex = appInstNode.ImageIndex;
                                appInstNode.Name = appInstNode.Text;
                                prodNode.Nodes.Add(appInstNode);
                            }
                            this.TheTreeView.Nodes.Add(prodNode);
                        }
                    }
                }
                if (this.TheTreeView.Nodes.Count <= 0)
                {
                    //add information node indicating no node found
                    TreeNode infoNode = new TreeNode(AppConstants.UsageRootNode);
                    infoNode.ImageIndex = GetIconIndex("Empty");
                    infoNode.SelectedImageIndex = infoNode.ImageIndex;
                    this.TheTreeView.Nodes.Add(infoNode);
                }
                this.TheTreeView.ExpandAll();
            }
            catch (COMException ex)
            {
            }
        }
        #endregion

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
