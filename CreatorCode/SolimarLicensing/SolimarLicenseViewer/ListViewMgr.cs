using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Configuration;
using System.Drawing;
using System.Net;

namespace SolimarLicenseViewer
{
    public class ListViewMgr
    {
        #region ListView Win32 Defines
        [StructLayout(LayoutKind.Sequential)]
        public struct HDITEM
        {
            public Int32 mask;
            public Int32 cxy;
            [MarshalAs(UnmanagedType.LPTStr)]
            public String pszText;
            public IntPtr hbm;
            public Int32 cchTextMax;
            public Int32 fmt;
            public Int32 lParam;
            public Int32 iImage;
            public Int32 iOrder;
        };

        // Parameters for ListView-Headers
        public const Int32 HDI_FORMAT = 0x0004;
        public const Int32 HDF_LEFT = 0x0000;
        public const Int32 HDF_STRING = 0x4000;
        public const Int32 HDF_SORTUP = 0x0400;
        public const Int32 HDF_SORTDOWN = 0x0200;
        public const Int32 HDM_GETITEM = 0x1200 + 11;  // HDM_FIRST + 11
        public const Int32 HDM_SETITEM = 0x1200 + 12;  // HDM_FIRST + 12
        public const Int32 LVM_GETHEADER = 0x1000 + 31;  // LVM_FIRST + 31
        public const Int32 LVM_FIRST = 0x1000;
        public const Int32 LVM_SETSELECTEDCOLUMN = (LVM_FIRST + 140);
        public const Int32 LVM_SETCOLUMNWIDTH = LVM_FIRST + 30;

        [DllImport("user32.dll")]
        private static extern IntPtr SendMessage(IntPtr hWnd, uint Msg, IntPtr wParam, IntPtr lParam);

        [DllImport("user32.dll", EntryPoint = "SendMessage")]
        private static extern IntPtr SendMessageITEM(IntPtr Handle, Int32 msg, IntPtr wParam, ref HDITEM lParam);
        #endregion

        #region Constructor
        /// <summary>
        /// Class constructor.  Initializes the ListView to manage and the link to the License Server/Wrapper.
        /// Also creates an instance to a ListView column sorter.
        /// <param name="lv">ListView from the MainForm</param>
        /// <param name="commlink">Link to Solimar License Server/Wrapper</param>        
        /// </summary>
        public ListViewMgr(SplitContainer splitCntl, 
            Shared.VisualComponents.NoFlickerListView lv, 
            ToolStrip lvTs,
            Shared.VisualComponents.NoFlickerListView bottomLv, 
            CommunicationLink commlink)
        {
            TheSplitControl = splitCntl;
            TheListView = lv;
            TheListViewToolStrip = lvTs;
            TheBottomListView = bottomLv;
            m_CommLink = commlink;
            m_TreeNode = null;
            m_connSettingsHelper = new ConnectionSettingsHelper(m_CommLink);
            m_toolStripList = new Dictionary<string, List<ToolStripItem>>();

            // Create an instance of a ListView column sorter and assign it 
            // to the ListView control.
            ListViewColumnSorter lvSorter = new ListViewColumnSorter();
            lvSorter.Order = SortOrder.Ascending;
            lvSorter.SortColumn = 0;
            lv.ListViewItemSorter = lvSorter;

            lvSorter = new ListViewColumnSorter();
            lvSorter.Order = SortOrder.Ascending;
            lvSorter.SortColumn = 0;
            TheBottomListView.ListViewItemSorter = lvSorter;

            InitializeToolStripItems();
        }
        public void InitializeToolStripItems()
        {
            List<ToolStripItem> tmpTSItemList = null;
            ToolStripButton tmpTSB = null;
            ToolStripSeparator tmpTSS = null;
            ToolStripLabel tmpTSL = null;

            #region Items for AppConstants.LicenseRootNode
            tmpTSItemList = new List<ToolStripItem>();
            tmpTSItemList.Add(new ToolStripLabel());
            m_toolStripList.Add(AppConstants.LicenseRootNode, tmpTSItemList);
            #endregion

            #region Items for AppConstants.UnitsHeader
            tmpTSItemList = new List<ToolStripItem>();
            tmpTSItemList.Add(new ToolStripLabel());

            m_moduleFilterComboBox = new ToolStripComboBox();
            m_moduleFilterComboBox.Items.Add(AppConstants.AllItem);
            m_moduleFilterComboBox.Items.Add(AppConstants.ExpiredItem);
            m_moduleFilterComboBox.Items.Add(AppConstants.NonExpiredItem);
            m_moduleFilterComboBox.Alignment = ToolStripItemAlignment.Right;
            m_moduleFilterComboBox.DropDownStyle = ComboBoxStyle.DropDownList;
            m_moduleFilterComboBox.SelectedIndex = 0;
            m_moduleFilterComboBox.SelectedIndexChanged += new EventHandler(m_moduleFilterComboBox_SelectedIndexChanged);
            tmpTSItemList.Add(m_moduleFilterComboBox);

            tmpTSL = new ToolStripLabel(AppConstants.FilterHeader + ": ");
            tmpTSL.Alignment = ToolStripItemAlignment.Right;
            tmpTSItemList.Add(tmpTSL);

            m_toolStripList.Add(AppConstants.UnitsHeader, tmpTSItemList);


            #endregion

            #region Items for AppConstants.ProtectionKeyRootNode
            tmpTSItemList = new List<ToolStripItem>();
            tmpTSItemList.Add(new ToolStripLabel());
            m_toolStripList.Add(AppConstants.ProtectionKeyRootNode, tmpTSItemList);
            #endregion

            #region Items for AppConstants.UsageRootNode
            tmpTSItemList = new List<ToolStripItem>();
            tmpTSItemList.Add(new ToolStripLabel());
            m_toolStripList.Add(AppConstants.UsageRootNode, tmpTSItemList);
            #endregion

            #region Items for AppConstants.ProductConnectionSettingsRootNode
            tmpTSItemList = new List<ToolStripItem>();
            tmpTSItemList.Add(new ToolStripLabel());

            tmpTSB = new ToolStripButton();
            tmpTSB.Alignment = ToolStripItemAlignment.Right;
            tmpTSB.Text = AppConstants.ProdSettingsTestConnAllTSB;
            tmpTSB.ToolTipText = AppConstants.ProdSettingsTestConnAllToolTipTSB;
            tmpTSB.DisplayStyle = ToolStripItemDisplayStyle.Text;
            tmpTSB.Click += new EventHandler(prodConnTestConnAllTSButton_Click);
            tmpTSItemList.Add(tmpTSB);
            m_toolStripList.Add(AppConstants.ProductConnectionSettingsRootNode, tmpTSItemList);

            tmpTSS = new ToolStripSeparator();
            tmpTSS.Alignment = ToolStripItemAlignment.Right;
            tmpTSItemList.Add(tmpTSS);

            tmpTSB = new ToolStripButton();
            tmpTSB.Alignment = ToolStripItemAlignment.Right;
            tmpTSB.Text = AppConstants.ProdSettingsTestConnSelectedTSB;
            tmpTSB.ToolTipText = AppConstants.ProdSettingsTestConnSelectedToolTipTSB;
            tmpTSB.DisplayStyle = ToolStripItemDisplayStyle.Text;
            tmpTSB.Click += new EventHandler(prodConnTestConnSelTSButton_Click);
            tmpTSItemList.Add(tmpTSB);

            tmpTSS = new ToolStripSeparator();
            tmpTSS.Alignment = ToolStripItemAlignment.Right;
            tmpTSItemList.Add(tmpTSS);

            tmpTSB = new ToolStripButton();
            tmpTSB.Alignment = ToolStripItemAlignment.Right;
            tmpTSB.Text = AppConstants.ProdSettingsEditConnTSB;
            tmpTSB.ToolTipText = AppConstants.ProdSettingsEditConnToolTipTSB;
            tmpTSB.DisplayStyle = ToolStripItemDisplayStyle.Text;
            tmpTSB.Click += new EventHandler(prodConn_EditConnSettings);
            tmpTSItemList.Add(tmpTSB);

            #endregion
        }

        void m_moduleFilterComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            PopulateView();
        }
        #endregion

        /// <summary>
        /// Populates the ListView with license information based upon the selected TreeNode.
        /// </summary>
        public void PopulateView()
        {
            //Clear items in listview
            this.TheListView.BeginUpdate();
            this.TheListView.Items.Clear();
            this.TheListView.Reset_NoItemsMessage();
            this.TheListView.SelectedIndexChanged -= new EventHandler(prodConn_SelectedIndexChanged);
            this.TheListView.KeyDown -= new KeyEventHandler(prodConn_KeyDown);
            this.TheListView.DoubleClick -= new EventHandler(prodConn_EditConnSettings);

            this.TheListView.SelectedIndexChanged -= new EventHandler(TheListView_SelectedIndexChanged);
            TheSplitControl.Panel2Collapsed = true;
            switch (SelectedNode.Level)
            {
                case 0:
                    if (SelectedNode.Text == AppConstants.LicenseRootNode)
                        LoadPackageData();
                    else if (SelectedNode.Text == AppConstants.ProtectionKeyRootNode)
                        LoadProtectionKeysData();
                    else if (SelectedNode.Text == AppConstants.UsageRootNode)
                        LoadUsageData();
                    else if (SelectedNode.Text == AppConstants.ProductConnectionSettingsRootNode)
                        LoadProductConnectionData();
                    break;
                case 1:
                    if (SelectedNode.Text == AppConstants.HistoryNode)
                        LoadHistoryData();
                    else if (SelectedNode.Parent.Text == AppConstants.LicenseRootNode)
                        LoadProductData();
                    else if (SelectedNode.Parent.Text == AppConstants.UsageRootNode)
                        LoadAppInstData();
                    break;
                case 2:
                    if (SelectedNode.Parent.Parent.Text == AppConstants.LicenseRootNode)  //load modules
                        LoadModuleData();
                    else if (SelectedNode.Parent.Parent.Text == AppConstants.UsageRootNode)    //used app instance
                        LoadUsedModuleData(SelectedNode.Parent.Name);
                    break;
                default:
                    break;
            }
            if (TheListView.Items.Count > 0)
                TheListView.GridLines = true;
            else
                TheListView.GridLines = false;
            AutoResizeColumns(TheListView, ColumnHeaderAutoResizeStyle.ColumnContent);
            PopulateViewColumns();
            this.TheListView.EndUpdate();
        }

        /// <summary>
        /// Populates the ListView column headers based upon the selected TreeNode of the License view.
        /// </summary>
        private void PopulateViewColumns()
        {
            TheListView.BeginUpdate();
            TheListView.Columns.Clear();
            ColumnHeader colHeader = null;
            switch (SelectedNode.Level)
            {
                case 0:
                    #region if (SelectedNode.Text == AppConstants.LicenseRootNode)
                    if (SelectedNode.Text == AppConstants.LicenseRootNode)
                    {
                        TheListView.Columns.Add(AppConstants.NameHeader);
                        TheListView.Columns.Add(AppConstants.LicenceTypeHeader);
                        TheListView.Columns.Add(AppConstants.VerificationStatusHeader);
                        {
                            colHeader = new ColumnHeader();
                            colHeader.Text = AppConstants.ExpirationHeader;
                            colHeader.Tag = typeof(DateTime);
                            TheListView.Columns.Add(colHeader);

                            colHeader = new ColumnHeader();
                            colHeader.Text = AppConstants.CurrentActivationHeader;
                            colHeader.Tag = typeof(int);
                            TheListView.Columns.Add(colHeader);

                            colHeader = new ColumnHeader();
                            colHeader.Text = AppConstants.TotalActivationHeader;
                            colHeader.Tag = typeof(int);
                            TheListView.Columns.Add(colHeader);

                            colHeader = new ColumnHeader();
                            colHeader.Text = AppConstants.DaysPerActivationHeader;
                            colHeader.Tag = typeof(int);
                            TheListView.Columns.Add(colHeader);
                        }
                    }
                    #endregion
                    #region else if(SelectedNode.Text == AppConstants.ProtectionKeyRootNode)
                    else if (SelectedNode.Text == AppConstants.ProtectionKeyRootNode)
                    {
                        TheListView.Columns.Add(AppConstants.KeyTypeHeader);
                        TheListView.Columns.Add(AppConstants.KeyNameHeader);
                        TheListView.Columns.Add(AppConstants.KeyProductIdHeader);
                        TheListView.Columns.Add(AppConstants.KeyProductVersionHeader);
                        TheListView.Columns.Add(AppConstants.KeyLicenseTypeHeader);
                        colHeader = new ColumnHeader();
                        colHeader.Text = AppConstants.KeyDaysLeftHeader;
                        colHeader.Tag = typeof(int);
                        TheListView.Columns.Add(colHeader);

                        colHeader = new ColumnHeader();
                        colHeader.Text = AppConstants.KeyExpirationDateHeader;
                        colHeader.Tag = typeof(DateTime);
                        TheListView.Columns.Add(colHeader);

                        TheBottomListView.Columns.Clear();
                        TheBottomListView.Columns.Add(AppConstants.PkModuleHeader);
                        TheBottomListView.Columns.Add(AppConstants.PkModuleTotalHeader);
                        TheBottomListView.Columns.Add(AppConstants.PkModuleInUseHeader);
                        
                    }
                    #endregion
                    #region else if(SelectedNode.Text == AppConstants.UsageRootNode)
                    else if (SelectedNode.Text == AppConstants.UsageRootNode)
                    {
                        TheListView.Columns.Add(AppConstants.UsageProductHeader);
                    }
                    #endregion
                    #region else if(SelectedNode.Text == AppConstants.ProductConnectionSettingsRootNode)
                    else if (SelectedNode.Text == AppConstants.ProductConnectionSettingsRootNode)
                    {
                        if (IsLocalMachine(m_CommLink.ServerName))
                        {
                            TheListView.Columns.Add(AppConstants.ConnProductHeader);
                            TheListView.Columns.Add(AppConstants.ConnPrimaryServerHeader);
                            TheListView.Columns.Add(AppConstants.ConnBackupServerHeader);
                            TheListView.Columns.Add(AppConstants.ConnTestDevHeader);
                            TheListView.Columns.Add(AppConstants.ConnStatusHeader);
                        }
                        else
                        {
                            TheListView.Columns.Add("");
                        }
                    }
                    #endregion
                    break;
                case 1:
                    #region if (SelectedNode.Text == AppConstants.HistoryNode)
                    if (SelectedNode.Text == AppConstants.HistoryNode)
                    {
                        TheListView.Columns.Add(AppConstants.NameHeader);
                        TheListView.Columns.Add(AppConstants.VerificationCodeHeader);
                        colHeader = new ColumnHeader();
                        colHeader.Text = AppConstants.DateAppliedHeader;
                        colHeader.Tag = typeof(DateTime);
                        TheListView.Columns.Add(colHeader);
                    }
                    #endregion
                    #region else if (SelectedNode.Parent.Text == AppConstants.LicenseRootNode)
                    else if (SelectedNode.Parent.Text == AppConstants.LicenseRootNode)
                    {
                        TheListView.Columns.Add(AppConstants.NameHeader);
                        TheListView.Columns.Add(AppConstants.VersionHeader);
                        colHeader = new ColumnHeader();
                        colHeader.Text = AppConstants.AppInstanceHeader;
                        colHeader.Tag = typeof(DateTime);
                        TheListView.Columns.Add(colHeader);
                    }
                    #endregion
                    #region else if (SelectedNode.Parent.Text == AppConstants.UsageRootNode)
                    else if (SelectedNode.Parent.Text == AppConstants.UsageRootNode)
                    {
                        TheListView.Columns.Add(AppConstants.UsageAppInstanceHeader);
                    }
                    #endregion
                    break;
                case 2:
                    #region if (SelectedNode.Parent.Parent.Text == AppConstants.LicenseRootNode)
                    if (SelectedNode.Parent.Parent.Text == AppConstants.LicenseRootNode)
                    {
                        colHeader = new ColumnHeader();
                        colHeader.Text = AppConstants.UnitsHeader;
                        colHeader.Tag = typeof(string);
                        TheListView.Columns.Add(colHeader);

                        colHeader = new ColumnHeader();
                        colHeader.Text = AppConstants.LicensesHeader;
                        colHeader.Tag = typeof(int);
                        TheListView.Columns.Add(colHeader);

                        colHeader = new ColumnHeader();
                        colHeader.Text = AppConstants.AppInstanceHeader;
                        colHeader.Tag = typeof(int);
                        TheListView.Columns.Add(colHeader);

                        colHeader = new ColumnHeader();
                        colHeader.Text = AppConstants.ExpirationHeader;
                        colHeader.Tag = typeof(DateTime);
                        TheListView.Columns.Add(colHeader);
                    }
                    #endregion
                    #region else if (SelectedNode.Parent.Parent.Text == AppConstants.UsageRootNode)
                    else if (SelectedNode.Parent.Parent.Text == AppConstants.UsageRootNode)
                    {
                        TheListView.Columns.Add(AppConstants.UsageModuleHeader);
                        colHeader = new ColumnHeader();
                        colHeader.Text = AppConstants.ObtainedHeader;
                        colHeader.Tag = typeof(int);
                        TheListView.Columns.Add(colHeader);

                        colHeader = new ColumnHeader();
                        colHeader.Text = AppConstants.TotalHeader;
                        colHeader.Tag = typeof(int);
                        TheListView.Columns.Add(colHeader);
                    }
                    #endregion
                    break;
                default:
                    break;
            }
            ResetListViewColumnSorter(TheListView);
            ResetListViewColumnSorter(TheBottomListView);
            AutoResizeColumns(TheListView, ColumnHeaderAutoResizeStyle.HeaderSize);
            AutoResizeColumns(TheBottomListView, ColumnHeaderAutoResizeStyle.HeaderSize);
            TheListView.EndUpdate();
        }

        #region License Section
        /// <summary>
        /// Loads package information into the ListView. Populates name, license type, and verification status columns. 
        /// </summary>
        public void LoadPackageData()
        {
            if (m_toolStripList.ContainsKey(AppConstants.LicenseRootNode))
            {
                this.TheListViewToolStrip.Visible = true;
                this.TheListViewToolStrip.Items.Clear();
                this.TheListViewToolStrip.Items.AddRange(m_toolStripList[AppConstants.LicenseRootNode].ToArray());
                this.TheListViewToolStrip.Items[0].Text = m_TreeNode.Text + ": ";
            }
            else
            {
                this.TheListViewToolStrip.Visible = false;
            }
            String generalStream = "";
            String verificationStatus = AppConstants.UnVerifiedStatus;
            Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs licInfoAttrib = new Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs();
            try
            {

                m_CommLink.GetAllSoftwareLicenses(ref generalStream);
                Solimar.Licensing.Attribs.AttribsMemberStringList strList = new Solimar.Licensing.Attribs.AttribsMemberStringList("stringList", new System.Collections.ArrayList());
                strList.SVal = generalStream;
                System.Collections.Generic.List<ListViewItem> lviList = new List<ListViewItem>();
                //display each package name and type
                foreach (string softwareLicense in strList.TVal)
                {
                    m_CommLink.GetSoftwareLicenseInfoByLicense(softwareLicense, ref generalStream);
                    licInfoAttrib.AssignMembersFromStream(generalStream);
                    ListViewItem lvItem = new ListViewItem();
                    lvItem.Text = softwareLicense;
                    lvItem.SubItems.Add(licInfoAttrib.softwareLicType.GetAlias());
                    StringBuilder toolTipBuilder = new StringBuilder();
                    try
                    {
                        bool bValid = false;
                        System.Collections.Generic.SortedList<string, bool> uniqueMessagesMap = new SortedList<string, bool>();
                        verificationStatus = m_CommLink.GetSoftwareLicenseStatus_ByLicense(softwareLicense, ref bValid);
                        toolTipBuilder.Append(verificationStatus);
                        uniqueMessagesMap.Add(verificationStatus, true);
                        if (!bValid)	//Cycle through all tokens to find all tokens that have an error
                        {
                            bool tokenIsValid = false;
                            string tokenVerfiedMessage;
                            foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs verToken in licInfoAttrib.licVerificationAttribs.TVal.validationTokenList.TVal)
                            {
                                tokenVerfiedMessage = m_CommLink.GetVerifyTokenByLicense(softwareLicense, (int)verToken.tokenType.TVal, verToken.tokenValue, ref tokenIsValid);
                                if (!tokenIsValid)	//Only add messages for failed cases
                                {
                                    if (uniqueMessagesMap.ContainsKey(tokenVerfiedMessage) == false)
                                    {
                                        if (toolTipBuilder.Length != 0)
                                            toolTipBuilder.Append("\n");
                                        toolTipBuilder.Append(tokenVerfiedMessage);
                                        uniqueMessagesMap.Add(tokenVerfiedMessage, true);
                                    }
                                }
                            }
                            lvItem.ForeColor = System.Drawing.Color.Red;
                        }
                        else
                        {
                            lvItem.ForeColor = System.Drawing.Color.Black;
                        }
                    }
                    catch (COMException)
                    {
                    }
                    lvItem.SubItems.Add(verificationStatus);

                    if (licInfoAttrib.activationCurrent.TVal == 0 &&
                        licInfoAttrib.activationTotal.TVal == 0 &&
                        licInfoAttrib.activationAmountInDays.TVal == 0)
                    {
                        m_DisasterRecoveryMode = false;
                        lvItem.SubItems.Add(SolimarLicenseViewer.AppConstants.NotUsedStatus);
                        lvItem.SubItems.Add(SolimarLicenseViewer.AppConstants.NotUsedStatus);
                        lvItem.SubItems.Add(SolimarLicenseViewer.AppConstants.NotUsedStatus);
                        lvItem.SubItems.Add(SolimarLicenseViewer.AppConstants.NotUsedStatus);
                    }
                    else
                    {
                        //if (toolTipBuilder.Length != 0)
                        //    toolTipBuilder.Append("");
                        toolTipBuilder.Append("\n");
                        toolTipBuilder.Append(AppConstants.ExpirationHeader);
                        toolTipBuilder.Append(": ");
                        toolTipBuilder.Append(licInfoAttrib.activationExpirationDate.TVal.ToLocalTime().ToString());

                        toolTipBuilder.Append("\n");
                        toolTipBuilder.Append(AppConstants.CurrentActivationHeader);
                        toolTipBuilder.Append(": ");
                        toolTipBuilder.Append(licInfoAttrib.activationCurrent.ToString());
                        toolTipBuilder.Append("\n");
                        toolTipBuilder.Append(AppConstants.TotalActivationHeader);
                        toolTipBuilder.Append(": ");
                        toolTipBuilder.Append(licInfoAttrib.activationTotal.TVal.ToString());
                        toolTipBuilder.Append("\n");
                        toolTipBuilder.Append(AppConstants.DaysPerActivationHeader);
                        toolTipBuilder.Append(": ");
                        toolTipBuilder.Append(licInfoAttrib.activationAmountInDays.TVal.ToString());

                        if (licInfoAttrib.activationCurrent.TVal < licInfoAttrib.activationTotal.TVal)
                            m_EnableDisasterRecoverExt = true;
                        else
                            m_EnableDisasterRecoverExt = false;
                        lvItem.SubItems.Add(licInfoAttrib.activationExpirationDate.TVal.ToLocalTime().ToString());
                        lvItem.SubItems.Add(licInfoAttrib.activationCurrent.TVal.ToString());
                        lvItem.SubItems.Add(licInfoAttrib.activationTotal.TVal.ToString());
                        lvItem.SubItems.Add(licInfoAttrib.activationAmountInDays.TVal.ToString());
                        m_DisasterRecoveryMode = true;
                    }
                    lvItem.ToolTipText = toolTipBuilder.ToString();
                    lviList.Add(lvItem);
               }
                this.TheListView.Items.AddRange(lviList.ToArray());
            }
            catch (COMException)
            {
                this.TheListView.NoItemsMessage = m_CommLink.Exception.Message;
            }
        }

        /// <summary>
        /// Loads product information into the ListView. Populates the name, version, application instances, and expiration date columns.
        /// </summary>
        public void LoadProductData()
        {
            if (m_toolStripList.ContainsKey(AppConstants.LicenseRootNode))
            {
                this.TheListViewToolStrip.Visible = true;
                this.TheListViewToolStrip.Items.Clear();
                this.TheListViewToolStrip.Items.AddRange(m_toolStripList[AppConstants.LicenseRootNode].ToArray());
                //this.TheListViewToolStrip.Items[0].Text = m_TreeNode.Name + ": ";
                StringBuilder sBuilder = new StringBuilder();
                foreach (string tmpStr in m_TreeNode.ToolTipText.Split('\n'))
                {
                    if (sBuilder.Length != 0)
                        sBuilder.Append(", ");
                    sBuilder.Append(tmpStr);
                }
                this.TheListViewToolStrip.Items[0].Text = sBuilder.ToString();
            }
            else
            {
                this.TheListViewToolStrip.Visible = false;
            }
            String generalStream = "";
            Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs licInfoAttrib = new Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs();
            try
            {
                System.Collections.Generic.List<ListViewItem> lviList = new List<ListViewItem>();
                m_CommLink.GetSoftwareLicenseInfoByLicense(m_TreeNode.Name, ref generalStream);
                licInfoAttrib.AssignMembersFromStream(generalStream);
                foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs prodInfo in licInfoAttrib.productList.TVal)
                {
                    ListViewItem lvItem = new ListViewItem();
                    lvItem.Text = m_CommLink.GetProductName(System.Convert.ToInt32(prodInfo.productID.ToString(), 16));
                    System.Text.StringBuilder strBuilder = new StringBuilder();
                    strBuilder.Append(prodInfo.product_Major.TVal.ToString());
                    strBuilder.Append(".");
                    strBuilder.Append(prodInfo.product_Minor.TVal.ToString());
                    if (prodInfo.product_SubMajor != 0 && prodInfo.product_SubMinor != 0)
                    {
                        strBuilder.Append(".");
                        strBuilder.Append(prodInfo.product_SubMajor.TVal.ToString());
                        strBuilder.Append(".");
                        strBuilder.Append(prodInfo.product_SubMinor.TVal.ToString());
                    }
                    lvItem.SubItems.Add(strBuilder.ToString());
                    lvItem.SubItems.Add(prodInfo.productAppInstance.TVal.ToString());
                    lvItem.ForeColor = m_TreeNode.ForeColor;
                    //lvItem.SubItems.Add(prodInfo.productExpirationDate.TVal.ToLocalTime().ToString());
                    lviList.Add(lvItem);
                }
                this.TheListView.Items.AddRange(lviList.ToArray());
            }
            catch (COMException)
            {
            }
        }

        /// <summary>
        /// Loads module information into the ListView. Populates the name, value, in use, application instances, expiration date columns.
        /// </summary>
        public void LoadModuleData()
        {
            int productID, moduleID;
            int useCount = -1;
            String generalStream = "";
            Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs licInfoAttrib = new Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs();
            try
            {
                //LicenseRootNode
                if (m_toolStripList.ContainsKey(AppConstants.UnitsHeader))
                {
                    this.TheListViewToolStrip.Visible = true;
                    this.TheListViewToolStrip.Items.Clear();
                    this.TheListViewToolStrip.Items.AddRange(m_toolStripList[AppConstants.UnitsHeader].ToArray());
                    StringBuilder sBuilder = new StringBuilder();
                    foreach (string tmpStr in m_TreeNode.ToolTipText.Split('\n'))
                    {
                        if(sBuilder.Length != 0)
                            sBuilder.Append(", ");
                        sBuilder.Append(tmpStr);
                    }
                    this.TheListViewToolStrip.Items[0].Text = sBuilder.ToString();
                }
                else
                {
                    this.TheListViewToolStrip.Visible = false;
                }


                m_CommLink.GetSoftwareLicenseInfoByLicense(m_TreeNode.Parent.Name, ref generalStream);
                licInfoAttrib.AssignMembersFromStream(generalStream);
                foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs prodInfo in licInfoAttrib.productList.TVal)
                {
                    productID = System.Convert.ToInt32(prodInfo.productID.ToString(), 16);
                    if (string.Equals(m_CommLink.GetProductName(productID), m_TreeNode.Text))
                    {
                        System.Collections.Generic.List<ListViewItem> lviList = new List<ListViewItem>();
                        bool bDisplayExpired = m_moduleFilterComboBox.SelectedIndex == 0/*All*/ || m_moduleFilterComboBox.SelectedIndex == 1/*Expired*/;
                        bool bDisplayNonExpired = m_moduleFilterComboBox.SelectedIndex == 0/*All*/ || m_moduleFilterComboBox.SelectedIndex == 2/*NonExpired*/;
                        foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs modInfo in prodInfo.moduleList.TVal)
                        {
                            //Get Module Info using product id and module id
                            ListViewItem lvItem = new ListViewItem();
                            moduleID = System.Convert.ToInt32(modInfo.moduleID.ToString(), 16);
                            lvItem.Text = m_CommLink.GetModuleName(productID, moduleID);
                            if ((DateTime.Compare(new DateTime(1900, 1, 1), modInfo.moduleExpirationDate.TVal) != 0) && (DateTime.Now.ToUniversalTime().CompareTo(modInfo.moduleExpirationDate.TVal) > 0))
                            {
                                lvItem.ForeColor = System.Drawing.Color.Red;
                                if (!bDisplayExpired)
                                    continue;
                            }
                            else
                            {
                                lvItem.ForeColor = m_TreeNode.ForeColor;
                                if (!bDisplayNonExpired)
                                    continue;
                            }

                            lvItem.SubItems.Add(System.Convert.ToInt32(modInfo.moduleValue.ToString(), 16).ToString());
                            m_CommLink.SoftwareModuleLicenseInUseForAllByProduct(productID, moduleID, ref useCount);
                            lvItem.SubItems.Add(System.Convert.ToInt32(modInfo.moduleAppInstance.ToString(), 16).ToString());
                            if (DateTime.Compare(new DateTime(1900, 1, 1), modInfo.moduleExpirationDate.TVal) != 0)
                                lvItem.SubItems.Add(modInfo.moduleExpirationDate.TVal.ToLocalTime().ToString());
                            else
                                lvItem.SubItems.Add("");
                            lviList.Add(lvItem);
                        }
                        this.TheListView.Items.AddRange(lviList.ToArray());
                    }
                }
            }
            catch (COMException)
            {
            }
        }

        /// <summary>
        /// Loads packet history information into the ListView. Populates the name, verification code, and date applied columns.
        /// </summary>
        public void LoadHistoryData()
        {
            String generalStream = "";
            Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs licInfoAttrib = new Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs();
            try
            {
                if (m_toolStripList.ContainsKey(AppConstants.LicenseRootNode))
                {
                    this.TheListViewToolStrip.Visible = true;
                    this.TheListViewToolStrip.Items.Clear();
                    this.TheListViewToolStrip.Items.AddRange(m_toolStripList[AppConstants.LicenseRootNode].ToArray());
                    this.TheListViewToolStrip.Items[0].Text = m_TreeNode.Text + ": ";
                }
                else
                {
                    this.TheListViewToolStrip.Visible = false;
                }

                m_CommLink.GetAllSoftwareLicenses(ref generalStream);
                Solimar.Licensing.Attribs.AttribsMemberStringList strList = new Solimar.Licensing.Attribs.AttribsMemberStringList("stringList", new System.Collections.ArrayList());
                strList.SVal = generalStream;
                //display each package name and type
                foreach (string softwareLicense in strList.TVal)
                {
                    m_CommLink.GetSoftwareLicenseInfoByLicense(softwareLicense, ref generalStream);
                    licInfoAttrib.AssignMembersFromStream(generalStream);
                    // Only display the last item.
                    if (licInfoAttrib.licVerificationAttribs.TVal.verificationCodeHistoryList.TVal.Count > 0)
                    {
                        ListViewItem lvItem = new ListViewItem();
                        lvItem.Text = softwareLicense;
                        Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_VerificationCodeAttribs verToken = (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_VerificationCodeAttribs)licInfoAttrib.licVerificationAttribs.TVal.verificationCodeHistoryList.TVal[licInfoAttrib.licVerificationAttribs.TVal.verificationCodeHistoryList.TVal.Count - 1];
                        lvItem.SubItems.Add(verToken.verificationValue);
                        lvItem.SubItems.Add(verToken.verificationDate.TVal.ToLocalTime().ToString());
                        this.TheListView.Items.Add(lvItem);
                    }
                }
            }
            catch (COMException)
            {
            }
        }

        #endregion

        #region Protection Key Section
        /// <summary>
        /// Loads protection key information into the ListView. Populates the name, ...
        /// </summary>
        public void LoadProtectionKeysData()
        {
            try
            {
                if (m_toolStripList.ContainsKey(AppConstants.ProtectionKeyRootNode))
                {
                    this.TheListViewToolStrip.Visible = true;
                    this.TheListViewToolStrip.Items.Clear();
                    this.TheListViewToolStrip.Items.AddRange(m_toolStripList[AppConstants.ProtectionKeyRootNode].ToArray());
                    this.TheListViewToolStrip.Items[0].Text = m_TreeNode.Text + ": ";
                }
                else
                {
                    this.TheListViewToolStrip.Visible = false;
                }
                //this.TheListView.BeginUpdate();
                //this.TheListView.Items.Clear();
                this.TheListView.SelectedIndexChanged -= new EventHandler(TheListView_SelectedIndexChanged);
                this.TheListView.SelectedIndexChanged += new EventHandler(TheListView_SelectedIndexChanged);

                foreach (Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseProtectionKeyInfo keyInfo in m_CommLink.KeyEnumerate())
                {
                    ListViewItem lvItem = new ListViewItem();
                    lvItem.Text = "Unknown Key Type";

                    if (keyInfo != null)
                    {
                        System.Diagnostics.Trace.WriteLine("LoadProtectionKeysData() keyName: " + keyInfo.keyName + ", productName: " + keyInfo.productName);
                        lvItem.Tag = keyInfo.keyName;
                        lvItem.SubItems.Add(keyInfo.keyName);

                        if (keyInfo.applicationInstance > 0)
                            lvItem.ForeColor = System.Drawing.Color.SteelBlue;
                        if (!keyInfo.bIsActive && !keyInfo.IsKeyTypeVerification() && !keyInfo.IsKeyTypeUninitialized())
                        {
                            lvItem.Font = new System.Drawing.Font(lvItem.Font, System.Drawing.FontStyle.Italic);
                            lvItem.ForeColor = System.Drawing.Color.Red;
                        }

                        lvItem.Text = keyInfo.keyTypeName;
                        if (keyInfo.IsKeyTypeUninitialized())
                            lvItem.SubItems.Add("-");
                        else
                            lvItem.SubItems.Add(keyInfo.productName);
                        if (!keyInfo.IsKeyTypeVerification() && !keyInfo.IsKeyTypeUninitialized())
                        {
                            lvItem.SubItems.Add(keyInfo.version);
                            lvItem.SubItems.Add(keyInfo.licenseTypeName);
                            int daysLeft = (int)(keyInfo.hoursLeft / 24);
                            if (daysLeft == 0 && keyInfo.bIsActive)
                                lvItem.SubItems.Add("Unlimited");
                            else if (keyInfo.IsKeyTypeDevelopment())
                                lvItem.SubItems.Add(daysLeft.ToString());
                            else
                                lvItem.SubItems.Add("-");
                            lvItem.SubItems.Add(keyInfo.expirationDate.ToString());
                        }
                        else
                        {
                            lvItem.SubItems.Add("-");
                            lvItem.SubItems.Add("-");
                            lvItem.SubItems.Add("-");
                            lvItem.SubItems.Add("-");
                        }
                    }
                    this.TheListView.Items.Add(lvItem);

                }
            }
            catch (COMException)
            {
            }
            finally
            {
                if (this.TheListView.Items.Count > 0)
                    this.TheListView.Items[0].Selected = true;
                TheListView_SelectedIndexChanged(this.TheListView, new EventArgs());
                //this.TheListView.EndUpdate();
            }
        }

        void TheListView_SelectedIndexChanged(object sender, EventArgs e)
        {
            System.Diagnostics.Trace.WriteLine("TheListView_SelectedIndexChanged()");
            if (sender is ListView)
            {
                ListView lView = sender as ListView;
                if (lView.SelectedItems.Count > 0 && lView.SelectedItems[0].Tag is string)
                {
                    Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseProtectionKeyInfo keyInfo = m_CommLink.KeyFindByName(lView.SelectedItems[0].Tag as string);
                    if (keyInfo != null && !keyInfo.IsKeyTypeUninitialized() && !keyInfo.IsKeyTypeVerification())
                    {
                        TheSplitControl.Panel2Collapsed = false;
                        LoadProtectionKeyModulesData(lView.SelectedItems[0].Tag as string);
                    }
                    else
                    {
                        TheSplitControl.Panel2Collapsed = true;
                    }
                }
                else
                {
                    LoadProtectionKeyModulesData("");
                    TheSplitControl.Panel2Collapsed = true;
                }
            }
        }

        public void LoadProtectionKeyModulesData(string keyName)
        {
            System.Diagnostics.Trace.WriteLine("LoadProtectionKeyModulesData() keyName: " + keyName);
            try
            {
                this.TheBottomListView.BeginUpdate();
                this.TheBottomListView.Items.Clear();
                if (keyName.Length > 0)
                {
                    Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseProtectionKeyInfo keyInfo = m_CommLink.KeyFindByName(keyName);
                    if (keyInfo != null && !keyInfo.IsKeyTypeUninitialized() && !keyInfo.IsKeyTypeVerification())
                    {
                        System.Collections.Generic.List<Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseProtectionKeyModuleInfo> modList = m_CommLink.GetModuleInfoList_ByLicense(keyName);
                        if (modList != null)
                        {
                            foreach (Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseProtectionKeyModuleInfo modInfo in modList)
                            {
                                if (modInfo.moduleTotal > 0)
                                {
                                    ListViewItem lvItem = new ListViewItem();
                                    lvItem.Text = modInfo.moduleName;
                                    if (modInfo.moduleTotal >= modInfo.moduleUnlimited && modInfo.moduleUnlimited > 0)
                                        lvItem.SubItems.Add(SolimarLicenseViewer.AppConstants.UnlimitedValue);
                                    else
                                        lvItem.SubItems.Add(modInfo.moduleTotal.ToString());
                                    lvItem.SubItems.Add(modInfo.moduleInUse.ToString());

                                    this.TheBottomListView.Items.Add(lvItem);
                                }
                            }
                        }
                    }
                }
            }
            catch (COMException)
            {

            }
            finally
            {
                if (TheBottomListView.Items.Count > 0)
                    TheBottomListView.GridLines = true;
                else
                    TheBottomListView.GridLines = false;

                AutoResizeColumns(TheBottomListView, ColumnHeaderAutoResizeStyle.ColumnContent);
                AutoResizeColumns(TheBottomListView, ColumnHeaderAutoResizeStyle.HeaderSize);
                this.TheBottomListView.EndUpdate();
            }
        }
        #endregion

        #region Usage Section

        public void LoadUsageData()
        {
            try
            {
                if (m_toolStripList.ContainsKey(AppConstants.UsageRootNode))
                {
                    this.TheListViewToolStrip.Visible = true;
                    this.TheListViewToolStrip.Items.Clear();
                    this.TheListViewToolStrip.Items.AddRange(m_toolStripList[AppConstants.UsageRootNode].ToArray());
                    this.TheListViewToolStrip.Items[0].Text = m_TreeNode.Text + ": ";
                }
                else
                {
                    this.TheListViewToolStrip.Visible = false;
                }

                String productName;
                String generalStream = "";
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
                            this.TheListView.Items.Add(productName);
                        }
                    }
                }
            }
            catch (COMException)
            {
            }
        }
        /// <summary>
        /// Loads application instance information into the ListView. Populates the name column.
        /// Displays only application instances connected to the license server.
        /// </summary>
        public void LoadAppInstData()
        {
            String generalStream = "";
            Solimar.Licensing.Attribs.AttribsMemberStringList appList = new Solimar.Licensing.Attribs.AttribsMemberStringList("stringList", new System.Collections.ArrayList());
            try
            {
                if (m_toolStripList.ContainsKey(AppConstants.UsageRootNode))
                {
                    this.TheListViewToolStrip.Visible = true;
                    this.TheListViewToolStrip.Items.Clear();
                    this.TheListViewToolStrip.Items.AddRange(m_toolStripList[AppConstants.UsageRootNode].ToArray());
                    StringBuilder sBuilder = new StringBuilder();
                    foreach (string tmpStr in m_TreeNode.ToolTipText.Split('\n'))
                    {
                        if (sBuilder.Length != 0)
                            sBuilder.Append(", ");
                        sBuilder.Append(tmpStr);
                    }
                    this.TheListViewToolStrip.Items[0].Text = AppConstants.UsageRootNode + " - " + sBuilder.ToString();
                }
                else
                {
                    this.TheListViewToolStrip.Visible = false;
                }
                m_CommLink.SoftwareGetApplicationInstanceListByProduct(m_CommLink.GetProductID(m_TreeNode.Text), ref generalStream);
                appList.SVal = generalStream;
                if (appList.TVal.Count > 0)
                {
                    foreach (string appInstance in appList.TVal)
                    {
                        ListViewItem lvItem = new ListViewItem();
                        lvItem.Text = appInstance;
                        //add another informative item
                        this.TheListView.Items.Add(lvItem);
                    }
                }
            }
            catch (COMException ex)
            {
                HandleExceptions.DisplayException(ex);
            }
        }
        /// <summary>
        /// Loads Used Module information into the ListView. Populates the name column, obtained, total columns.
        /// Displays only modules that have been obtained by the application instance.
        /// </summary>
        public void LoadUsedModuleData(string _productName)
        {
            int productID, moduleID;
            int licenseCount = 0;
            String generalStream = "";
            Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs licInfoAttrib = new Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs();
            try
            {
                if (m_toolStripList.ContainsKey(AppConstants.UsageRootNode))
                {
                    this.TheListViewToolStrip.Visible = true;
                    this.TheListViewToolStrip.Items.Clear();
                    this.TheListViewToolStrip.Items.AddRange(m_toolStripList[AppConstants.UsageRootNode].ToArray());
                    //this.TheListViewToolStrip.Items[0].Text = m_TreeNode.ToolTipText + ": ";
                    StringBuilder sBuilder = new StringBuilder();
                    foreach (string tmpStr in m_TreeNode.ToolTipText.Split('\n'))
                    {
                        if (sBuilder.Length != 0)
                            sBuilder.Append(", ");
                        sBuilder.Append(tmpStr);
                    }
                    this.TheListViewToolStrip.Items[0].Text = AppConstants.UsageRootNode + " - " + sBuilder.ToString();
                }
                else
                {
                    this.TheListViewToolStrip.Visible = false;
                }
                m_CommLink.GetSoftwareLicenseInfoForAll(ref generalStream);
                licInfoAttrib.AssignMembersFromStream(generalStream);
                foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs prodInfo in licInfoAttrib.productList.TVal)
                {   //found matching prodinfo attriblist
                    productID = System.Convert.ToInt32(prodInfo.productID.ToString(), 16);
                    if (string.Equals(m_CommLink.GetProductName(productID), _productName))
                    {
                        m_CommLink.InitializeWrapper(m_TreeNode.Text, productID, System.Convert.ToInt32(prodInfo.product_Major.ToString(), 16), System.Convert.ToInt32(prodInfo.product_Minor.ToString(), 16));
                        foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs modInfo in prodInfo.moduleList.TVal)
                        {
                            moduleID = System.Convert.ToInt32(modInfo.moduleID.ToString(), 16);

                            m_CommLink.ModuleLicenseInUse_ByApp(moduleID, ref licenseCount);
                            if (licenseCount > 0)
                            {
                                ListViewItem lvItem = new ListViewItem();
                                lvItem.Text = m_CommLink.GetModuleName(productID, moduleID);
                                lvItem.SubItems.Add(licenseCount.ToString());
                                m_CommLink.ModuleLicenseTotal(moduleID, ref licenseCount);
                                lvItem.SubItems.Add(licenseCount.ToString());
                                this.TheListView.Items.Add(lvItem);
                            }
                        }
                        //found product, no need to continue searching productlist
                        break;
                    }
                }
            }
            catch (COMException) { }
        }
        #endregion

        #region ProductionConnectionData Section
        public void LoadProductConnectionData()
        {
            if (IsLocalMachine(m_CommLink.ServerName))
            {
                if (m_toolStripList.ContainsKey(AppConstants.ProductConnectionSettingsRootNode))
                {
                    this.TheListViewToolStrip.Visible = true;
                    this.TheListViewToolStrip.Items.Clear();
                    this.TheListViewToolStrip.Items.AddRange(m_toolStripList[AppConstants.ProductConnectionSettingsRootNode].ToArray());
                    this.TheListViewToolStrip.Items[0].Text = m_TreeNode.Text + ": ";
                }
                else
                {
                    this.TheListViewToolStrip.Visible = false;
                }

                this.TheListView.SelectedIndexChanged += new EventHandler(prodConn_SelectedIndexChanged);
                this.TheListView.KeyDown += new KeyEventHandler(prodConn_KeyDown);
                this.TheListView.DoubleClick += new EventHandler(prodConn_EditConnSettings);

                foreach (ConnectionSettings2 connSettings in m_connSettingsHelper.GetConnectionSettings().Values)
                    UpdateProdConnListView(connSettings);
            }
            else
            {
                if (m_toolStripList.ContainsKey(AppConstants.LicenseRootNode))
                {
                    this.TheListViewToolStrip.Visible = true;
                    this.TheListViewToolStrip.Items.Clear();
                    this.TheListViewToolStrip.Items.AddRange(m_toolStripList[AppConstants.LicenseRootNode].ToArray());
                    this.TheListViewToolStrip.Items[0].Text = m_TreeNode.Text + ": ";
                }
                else
                {
                    this.TheListViewToolStrip.Visible = false;
                }
                this.TheListView.NoItemsMessage = "This can only be set when connected to the localhost";
            }
        }



        ///key into ListView where _productID is unique, display_connectionSettings.  If _productID is new then add
        ///_connectionSettings.  If _productID is already there, update existing item.
        private void UpdateProdConnListView(ConnectionSettings2 _connectionSettings)
        {
            this.TheListView.BeginUpdate();

            ListViewItem matchingLvi = null;
            foreach (ListViewItem lvi in this.TheListView.Items)
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
                matchingLvi.ForeColor = System.Drawing.Color.Black;
                matchingLvi.Text = m_CommLink.GetProductName(_connectionSettings.ProductID);
                matchingLvi.Tag = _connectionSettings.ProductID;
                matchingLvi.SubItems.Add(_connectionSettings.ServerName);
                matchingLvi.SubItems.Add(_connectionSettings.BackupName);
                matchingLvi.SubItems.Add(_connectionSettings.UseDevelopmentLic.ToString());
                matchingLvi.SubItems.Add(AppConstants.ProductConnectionSettingsUnknownStatus);
                this.TheListView.Items.Add(matchingLvi);
            }
            else //Update Existing item
            {
                matchingLvi.ForeColor = System.Drawing.Color.Black;
                matchingLvi.Text = m_CommLink.GetProductName(_connectionSettings.ProductID);
                matchingLvi.SubItems[1].Text = _connectionSettings.ServerName;
                matchingLvi.SubItems[2].Text = _connectionSettings.BackupName;
                matchingLvi.SubItems[3].Text = _connectionSettings.UseDevelopmentLic.ToString();
                matchingLvi.SubItems[4].Text = AppConstants.ProductConnectionSettingsUnknownStatus;
            }
            prodConn_SelectedIndexChanged(TheListView, new EventArgs());
            this.TheListView.EndUpdate();
        }

        public void prodConn_EditConnSettings(object sender, EventArgs e)
        {
            if (this.TheListView.SelectedItems.Count > 0)
            {
                Dictionary<int, ConnectionSettings2> prodConnSettingsMap = m_connSettingsHelper.GetConnectionSettings();
                EditConnectionSettingsDialog dialog = new EditConnectionSettingsDialog();
                System.Collections.Generic.List<ConnectionSettings2> objList = new List<ConnectionSettings2>();
                foreach (ListViewItem lvi in this.TheListView.SelectedItems)
                {
                    if ((int)lvi.Tag != -1)
                        objList.Add(prodConnSettingsMap[(int)lvi.Tag]);
                }

                string caption = (objList.Count > 1) ?
                    "Edit Connection Settings for multiple products" :
                    "Edit Connection Settings Product: " + m_CommLink.GetProductName(objList[0].ProductID);

                dialog.SetData(objList.ToArray(), caption);

                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    foreach (ConnectionSettings2 connSettings in dialog.GetData())
                    {
                        if (connSettings.IsDirty)
                        {
                            UpdateProdConnListView(connSettings);
                            m_connSettingsHelper.SetConnectionSetting(connSettings.ProductID, connSettings);
                        }
                    }
                }

            }
        }
        private void prodConn_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                prodConn_EditConnSettings(this.TheListView, new EventArgs());
            }
            else
            {
                e.Handled = false;
            }
        }
        private void prodConn_SelectedIndexChanged(object sender, EventArgs e)
        {
            bool bSelected = this.TheListView.SelectedItems.Count > 0;
            foreach (ToolStripItem tsItem in this.TheListViewToolStrip.Items)
            {
                if (tsItem is ToolStripButton)
                {
                    if (((string.Compare(tsItem.Text, AppConstants.ProdSettingsEditConnTSB, true) == 0) ||
                    (string.Compare(tsItem.Text, AppConstants.ProdSettingsTestConnSelectedTSB, true) == 0))
                    )
                    {
                        tsItem.Enabled = bSelected;
                    }
                }
            }
        }
        public void prodConnTestConnSelTSButton_Click(object sender, EventArgs e)
        {
            System.Collections.Generic.List<ListViewItem> lvItemList = new List<ListViewItem>();
            foreach (ListViewItem lvi in this.TheListView.SelectedItems)
                lvItemList.Add(lvi);
            testConnectionToLicenseServer(lvItemList);
        }
        public void prodConnTestConnAllTSButton_Click(object sender, EventArgs e)
        {
            System.Collections.Generic.List<ListViewItem> lvItemList = new List<ListViewItem>();
            foreach (ListViewItem lvi in this.TheListView.Items)
                lvItemList.Add(lvi);
            testConnectionToLicenseServer(lvItemList);
        }

        private void testConnectionToLicenseServer(System.Collections.Generic.List<ListViewItem> _listViewItemList)
        {
            ConnectionSettings2 connSettings = new ConnectionSettings2();
            foreach (ListViewItem lvItem in _listViewItemList)
            {
                connSettings.ProductID = (int)lvItem.Tag;
                connSettings.ServerName = lvItem.SubItems[1].Text;
                connSettings.BackupName = lvItem.SubItems[2].Text;
                connSettings.UseDevelopmentLic = System.String.Compare(lvItem.SubItems[3].Text, "true", true) == 0;
                try
                {
                    testConnectionToLicenseServer(connSettings);
                    lvItem.ForeColor = System.Drawing.Color.Green;
                    lvItem.SubItems[4].Text = "Successfully connected to the License Server";
                }
                catch (Exception ex)
                {
                    lvItem.ForeColor = System.Drawing.Color.Red;
                    lvItem.SubItems[4].Text = ex.Message.Replace('\r', ' ').Replace('\n', ' ');
                }
                //setToolTip(lvItem);
            }
        }
        // throws an exception if fails to connect...
        private void testConnectionToLicenseServer(ConnectionSettings2 _connectionSettings)
        {
            if (_connectionSettings != null)
            {
                try
                {

                    this.TheListView.FindForm().Cursor = Cursors.WaitCursor;
                    //this.TheListView.FindForm().Enabled = false;

                    using (Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseWrapper licWrapper = new Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseWrapper())
                    {
                        licWrapper.ConnectEx(_connectionSettings.ServerName);
                        if (_connectionSettings.BackupName.Length != 0)
                            licWrapper.ConnectEx(_connectionSettings.BackupName, false, true);
                        Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID productID = (Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID)_connectionSettings.ProductID;
                        if (_connectionSettings.UseDevelopmentLic == true)
                        {
                            //Successfully connected, use a different product ID for the Test/Dev licensing
                            if (productID == Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_Iconvert)
                                productID = Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_TestDevIconvert;
                            else if (productID == Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_Rubika)
                                productID = Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_TestDevRubika;
                            else if (productID == Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_RubikaProcessBuilder)
                                productID = Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_TestDevRubikaProcessBuilder;
                            else if (productID == Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_SdxDesigner)
                                productID = Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_TestDevSdxDesigner;
                            else if (productID == Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_SolIndexer)
                                productID = Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_TestDevSolIndexer;
                            else if (productID == Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_SolFusion)
                                productID = Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_TestDevSolfusionSp;
                            else if (productID == Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_SolScript)
                                productID = Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_TestDevSolScript;
                            else if (productID == Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_SolsearcherEnt)
                                productID = Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_TestDevSolsearcherEp;
                            else if (productID == Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_Spde)
                                productID = Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_TestDevSpde;
                            else if (productID == Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_SpdeQueueManager)
                                productID = Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_TestDevSpdeQueueManager;
                            else if (productID == Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_SolsearcherSp)
                                productID = Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_TestDevSseSp;
                        }

                        licWrapper.InitializeEx(System.Environment.MachineName.ToLower(),   //application_instance
                                                (int)productID, //product
                                                0,              //prod_ver_major
                                                0,              //prod_ver_minor
                                                false,          //single_key
                                                "",             //specific_single_key_ident
                                                false,          //lock_keys
                                                0,              //ui_level
                                                0,              //grace_period_minutes
                                                false,          //application_instance_lock_keys
                                                false);         //bypass_remote_key_restriction
                        licWrapper.DisconnectEx();
                        //licWrapper = null;
                    }
                }
                catch (Exception ex)
                {
                    HandleExceptions.DisplayException(this.TheListView.FindForm(), ex, "Failed to connect to License Server", "Test Connection to License Server");
                    throw;
                }
                finally
                {

                    //this.TheListView.FindForm().Enabled = true;
                    this.TheListView.FindForm().Cursor = Cursors.Default;
                }
            }
        }
        #endregion

        #region ListViewColumn Methods
        /// <summary>
        /// Initializes the ListViewColumnSorter with options based upon currently selected column. 
        /// <param name="columnIndex"> index of the column to sort </param>
        /// </summary>
        public void SetListViewColumnSorter(ListView listView, int columnIndex)
        {
            // Determine if clicked column is already the column that is being sorted.
            //listView.ListViewItemSorter
            ListViewColumnSorter lvSorter = null;
            if (listView.ListViewItemSorter is ListViewColumnSorter)
                lvSorter = (listView.ListViewItemSorter as ListViewColumnSorter);
            if (columnIndex == lvSorter.SortColumn)
            {
                // Reverse the current sort direction for this column.
                if (lvSorter.Order == SortOrder.Ascending)
                {
                    lvSorter.Order = SortOrder.Descending;
                }
                else
                {
                    lvSorter.Order = SortOrder.Ascending;
                }
            }
            else
            {
                SetSortIcons(listView, lvSorter.SortColumn, columnIndex);
                // Set the column number that is to be sorted; default to ascending.
                lvSorter.SortColumn = columnIndex;
                lvSorter.Order = SortOrder.Ascending;
            }

            if (columnIndex <= listView.Columns.Count - 1)
                lvSorter.SortType = (System.Type)listView.Columns[columnIndex].Tag;
            SetSortIcons(listView, lvSorter.SortColumn, columnIndex);
            SetSelectedColumnColor(listView, columnIndex);
        }

        //sizes depending on what the current comparer's order
        public void SetSortIcons(ListView lView, int previouslySortedColumn, int newSortColumn)
        {
            IntPtr hHeader = SendMessage(lView.Handle, LVM_GETHEADER, IntPtr.Zero, IntPtr.Zero);
            IntPtr newColumn = new IntPtr(newSortColumn);
            IntPtr prevColumn = new IntPtr(previouslySortedColumn);
            HDITEM hdItem;
            IntPtr rtn;

            // Only update the previous item if it existed and if it was a different one.
            if (previouslySortedColumn != -1 && previouslySortedColumn != newSortColumn)
            {
                // Clear icon from the previous column.
                hdItem = new HDITEM();
                hdItem.mask = HDI_FORMAT;
                rtn = SendMessageITEM(hHeader, HDM_GETITEM, prevColumn, ref hdItem);
                hdItem.fmt &= ~HDF_SORTDOWN & ~HDF_SORTUP;
                rtn = SendMessageITEM(hHeader, HDM_SETITEM, prevColumn, ref hdItem);
            }

            // Set icon on the new column.
            hdItem = new HDITEM();
            hdItem.mask = HDI_FORMAT;
            rtn = SendMessageITEM(hHeader, HDM_GETITEM, newColumn, ref hdItem);
            ListViewColumnSorter lvSorter = null;
            if (lView.ListViewItemSorter is ListViewColumnSorter)
                lvSorter = (lView.ListViewItemSorter as ListViewColumnSorter);
            if (lvSorter == null || lvSorter.Order == SortOrder.Ascending || lvSorter.Order == SortOrder.None)
            {
                hdItem.fmt &= ~HDF_SORTDOWN;
                hdItem.fmt |= HDF_SORTUP;
                hdItem.fmt |= HDF_LEFT;
            }
            else
            {
                hdItem.fmt &= ~HDF_SORTUP;
                hdItem.fmt |= HDF_SORTDOWN;
                hdItem.fmt |= HDF_LEFT;
            }
            rtn = SendMessageITEM(hHeader, HDM_SETITEM, newColumn, ref hdItem);
        }

        public void SetSelectedColumnColor(ListView lView, int newSortColumn)
        {
            IntPtr newColumn = new IntPtr(newSortColumn);
            SendMessage(lView.Handle, LVM_SETSELECTEDCOLUMN, newColumn, IntPtr.Zero);
        }

        /// <summary>
        /// AutoResize each ListView column based upon the header size. 
        /// Need to call after columns are added for proper resizing.
        /// </summary>
        /// 
        public void AutoResizeColumns(ListView listView, ColumnHeaderAutoResizeStyle headerStyle)
        {
            foreach (ColumnHeader column in listView.Columns)
            {
                column.AutoResize(headerStyle);
                if (listView.Columns.IndexOf(column) != listView.Columns.Count - 1)
                {
                    //Pad 3 spaces to account for sort up/down arrow
                    column.Width += TextRenderer.MeasureText(" ", listView.Font).Width * 3;
                }
            }
        }

        public void ResetListViewColumnSorter(ListView listView)
        {
            if (listView.Items.Count > 0)
            {
                if (listView.ListViewItemSorter is ListViewColumnSorter)
                    (listView.ListViewItemSorter as ListViewColumnSorter).ResetProperties();
                SetListViewColumnSorter(listView, 0);
            }
        }
        #endregion

        #region Helper Methods
        public bool IsLocalMachine(string _serverName)
        {
            bool bIsLocalMachine = false;
            bIsLocalMachine = (string.Compare(_serverName, "localhost", true) == 0);

            if (!bIsLocalMachine)
                bIsLocalMachine = (string.Compare(_serverName, System.Environment.MachineName, true) == 0);
            if (!bIsLocalMachine)
            {
                //see if IP address was past in
                try
                {
                    //find host by name
                    IPHostEntry iphostentry = Dns.GetHostEntry(Dns.GetHostName());

                    //enumerate IP addresses
                    foreach (IPAddress ipaddress in iphostentry.AddressList)
                    {
                        if (string.Compare(_serverName, ipaddress.ToString(), true) == 0)
                        {
                            bIsLocalMachine = true;
                            break;
                        }
                    }
                }
                catch (Exception ex)
                {
                    HandleExceptions.DisplayException(ex);
                }
            }
            return bIsLocalMachine;
        }
        #endregion

        #region Properties
        /// <summary>
        /// Gets the currently selected TreeNode.
        /// </summary>
        public TreeNode SelectedNode
        {
            get { return m_TreeNode; }
            set { m_TreeNode = value; }
        }

        public Boolean EnableDisasterRecoverExt
        {
            get { return m_EnableDisasterRecoverExt; }
            set { m_EnableDisasterRecoverExt = value; }
        }
        #endregion

        #region Private Variables
        /// <summary>
        /// Specifies the ListView from the MainForm
        /// </summary>
        private Shared.VisualComponents.NoFlickerListView TheListView;
        private SplitContainer TheSplitControl;
        private ToolStrip TheListViewToolStrip;
        private Shared.VisualComponents.NoFlickerListView TheBottomListView;
        /// <summary>
        /// Stores user selected TreeNode
        /// </summary>
        private TreeNode m_TreeNode;
        /// <summary>
        /// object to the license server/wrapper functions
        /// </summary>
        private CommunicationLink m_CommLink;
        private ConnectionSettingsHelper m_connSettingsHelper;
        /// <summary>
        /// Boolean object to determine disaster recovery mode
        /// </summary>
        private Boolean m_DisasterRecoveryMode;
        /// <summary>
        /// Boolean object to determine disaster recovery extensions enabled
        /// </summary>
        private Boolean m_EnableDisasterRecoverExt;

        private Dictionary<string, List<ToolStripItem>> m_toolStripList;
        private ToolStripComboBox m_moduleFilterComboBox;
        #endregion
    }
    // Implements the manual sorting of items by columns.
    class ListViewColumnSorter : IComparer
    {
        #region Private Variables
        private System.Type InternalSortType;
        /// <summary>
        /// Specifies the column to be sorted
        /// </summary>
        private int ColumnToSort;
        /// <summary>
        /// Specifies the order in which to sort (i.e. 'Ascending').
        /// </summary>
        private SortOrder OrderOfSort;
        /// <summary>
        /// Case insensitive comparer object
        /// </summary>
        private CaseInsensitiveComparer ObjectCompare;
        #endregion

        #region Constructor
        /// <summary>
        /// Class constructor.  Initializes various elements
        /// </summary>
        public ListViewColumnSorter()
        {
            // Initialize the column to '0'
            ColumnToSort = 0;

            // Initialize the sort order to 'none'
            OrderOfSort = SortOrder.None;

            // Initialize the CaseInsensitiveComparer object
            ObjectCompare = new CaseInsensitiveComparer();
        }
        #endregion

        #region Comparison Methods
        /// <summary>
        /// This method is inherited from the IComparer interface.  It compares the two objects passed using a case insensitive comparison.
        /// </summary>
        /// <param name="x">First object to be compared</param>
        /// <param name="y">Second object to be compared</param>
        /// <returns>The result of the comparison. "0" if equal, negative if 'x' is less than 'y' and positive if 'x' is greater than 'y'</returns>
        public int Compare(object x, object y)
        {
            int compareResult = 0;

            ListViewItem listviewX, listviewY;

            // Cast the objects to be compared to ListViewItem objects
            listviewX = (ListViewItem)x;
            listviewY = (ListViewItem)y;

            if (ColumnToSort < listviewX.SubItems.Count && ColumnToSort < listviewY.SubItems.Count)
            {
                string itemX = listviewX.SubItems[ColumnToSort].Text;
                string itemY = listviewY.SubItems[ColumnToSort].Text;

                object obj1 = InternalSortType;
                System.Type sysType = null;
                if (InternalSortType is System.Type)
                    sysType = (System.Type)InternalSortType;

                bool bCalculatedCompareResult = false;
                try
                {
                    if (itemX.Length != 0 && itemY.Length != 0)
                    {
                        if (InternalSortType == typeof(System.Int32))
                        {
                            compareResult = Convert.ToInt32(itemX) - Convert.ToInt32(itemY);
                            bCalculatedCompareResult = true;
                        }
                        else if (InternalSortType == typeof(System.DateTime))
                        {
                            compareResult = DateTime.Compare(Convert.ToDateTime(itemX), Convert.ToDateTime(itemY));
                            bCalculatedCompareResult = true;
                        }
                    }
                }
                catch (Exception)
                {
                }

                if (bCalculatedCompareResult == false)
                {
                    compareResult = ObjectCompare.Compare(itemX, itemY);
                }
            }

            // Calculate correct return value based on object comparison
            if (OrderOfSort == SortOrder.Ascending)
            {
                // Ascending sort is selected, return normal result of compare operation
                return compareResult;
            }
            else if (OrderOfSort == SortOrder.Descending)
            {
                // Descending sort is selected, return negative result of compare operation
                return (-compareResult);
            }
            else
            {
                // Return '0' to indicate they are equal
                return 0;
            }
        }

        public void ResetProperties()
        {
            SortColumn = 0;
            Order = SortOrder.None;
            SortType = typeof(string);
        }

        #endregion

        #region Properties
        /// <summary>
        /// Gets or sets the number of the column to which to apply the sorting operation (Defaults to '0').
        /// </summary>
        public int SortColumn
        {
            get { return ColumnToSort; }
            set { ColumnToSort = value; }
        }

        /// <summary>
        /// Gets or sets the order of sorting to apply (for example, 'Ascending' or 'Descending').
        /// </summary>
        public SortOrder Order
        {
            get { return OrderOfSort; }
            set { OrderOfSort = value; }
        }

        public System.Type SortType
        {
            get { return InternalSortType; }
            set { InternalSortType = value; }
        }
        #endregion
    }
}
