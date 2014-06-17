using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Configuration;
using System.Drawing;
using System.Net;
using Shared.VisualComponents;

namespace SolimarLicenseViewer
{
    using DisplayNameAndEventType = Solimar.Licensing.Common.Collections.Tuple<string, System.Diagnostics.EventLogEntryType>;
    using EventTypeToEventCounterList = SortedList<string, Solimar.Licensing.Common.Collections.Tuple<ToolStripItem, int, string>>;
    using EventTypeToEventCounterPair = KeyValuePair<string, Solimar.Licensing.Common.Collections.Tuple<ToolStripItem, int, string>>;

    /// <summary>Tracks Number of Events seen of a given Event Type</summary>
    using EventCounter = Solimar.Licensing.Common.Collections.Tuple<ToolStripItem, int/*Count Seen*/, string>;
    using Solimar.Licensing.Attribs;

    public class ListViewMgr
    {
        /// <summary>Function Signature: void Function(bool)</summary>
        public delegate void ParamBoolReturnsVoid(bool b1);
        public ParamBoolReturnsVoid DelUseWaitCursor = null;

        public delegate void ParamListViewItemListReturnsVoid(IList<ListViewItem> lviList);
        public ParamListViewItemListReturnsVoid DelTestConnection = null;
        public void TestConnection(IList<ListViewItem> _lviList)
        {
            if (DelTestConnection != null)
                DelTestConnection(_lviList);
        }

        public void UseWaitCursor(bool _bUseWaitCursor)
        {
            if (DelUseWaitCursor != null)
                DelUseWaitCursor(_bUseWaitCursor);
        }


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
            ToolStrip bottomLvTs,
            Shared.VisualComponents.EventLogEntryControl eventLogEntryControl,
            CommunicationLink commlink,
            ImageList imageList)
        {
            TheSplitControl = splitCntl;
            TheListView = lv;
            TheListViewToolStrip = lvTs;
            TheBottomListView = bottomLv;
            TheBottomListViewToolStrip = bottomLvTs;
            TheEventLogEntryControl = eventLogEntryControl;
            m_CommLink = commlink;
            m_TreeNode = null;
            m_connSettingsHelper = new ConnectionSettingsHelper(m_CommLink);
            m_toolStripList = new Dictionary<string, List<ToolStripItem>>();
            m_imageList = imageList;

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
            ToolStripComboBox tmpTCB = null;

            #region Items for AppConstants.LicenseRootNode
            tmpTSItemList = new List<ToolStripItem>();
            tmpTSItemList.Add(new ToolStripLabel());
            m_toolStripList.Add(AppConstants.LicenseRootNode, tmpTSItemList);
            #endregion

            #region Items for AppConstants.BottomUnitsHeader
            tmpTSItemList = new List<ToolStripItem>();
            tmpTSItemList.Add(new ToolStripLabel());

            m_moduleFilterComboBox = new ToolStripComboBox();
            m_moduleFilterComboBox.Items.Add(AppConstants.DetailsItem);
            m_moduleFilterComboBox.Items.Add(AppConstants.GroupByModuleItem);
            //m_moduleFilterComboBox.Items.Add(AppConstants.ProductLicNumberItem);
            //Remove ability to group by product license number

            m_moduleFilterComboBox.Alignment = ToolStripItemAlignment.Right;
            m_moduleFilterComboBox.DropDownStyle = ComboBoxStyle.DropDownList;
            m_moduleFilterComboBox.SelectedIndex = 1;
            m_moduleFilterComboBox.SelectedIndexChanged += new EventHandler(m_moduleFilterComboBox_SelectedIndexChanged);
            using (Graphics g = TheListView.CreateGraphics())
            {
                m_moduleFilterComboBox.AutoSize = false;
                SizeF size = g.MeasureString(AppConstants.ProductLicNumberItem, new Font(m_moduleFilterComboBox.Font.FontFamily.Name, m_moduleFilterComboBox.Font.Size));
                size.Width += 20;
                m_moduleFilterComboBox.Size = size.ToSize();
            }
            tmpTSItemList.Add(m_moduleFilterComboBox);

            tmpTSL = new ToolStripLabel(AppConstants.FilterHeader + ": ");
            tmpTSL.Alignment = ToolStripItemAlignment.Right;
            tmpTSItemList.Add(tmpTSL);

            m_toolStripList.Add(AppConstants.BottomUnitsHeader, tmpTSItemList);
            #endregion

            #region Items for AppConstants.BottomLicenseRootNode
            tmpTSItemList = new List<ToolStripItem>();
            tmpTSItemList.Add(new ToolStripLabel());
            m_toolStripList.Add(AppConstants.BottomLicenseRootNode, tmpTSItemList);
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
            tmpTSB.DisplayStyle = ToolStripItemDisplayStyle.Image;
            tmpTSB.Image = global::SolimarLicenseViewer.Properties.Resources.licenseTestAll;
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
            tmpTSB.DisplayStyle = ToolStripItemDisplayStyle.Image;
            tmpTSB.Image = global::SolimarLicenseViewer.Properties.Resources.licenseTest;
            tmpTSB.Click += new EventHandler(prodConnTestConnSelTSButton_Click);
            tmpTSItemList.Add(tmpTSB);

            tmpTSS = new ToolStripSeparator();
            tmpTSS.Alignment = ToolStripItemAlignment.Right;
            tmpTSItemList.Add(tmpTSS);

            tmpTSB = new ToolStripButton();
            tmpTSB.Alignment = ToolStripItemAlignment.Right;
            tmpTSB.Text = AppConstants.ProdSettingsEditConnTSB;
            tmpTSB.ToolTipText = AppConstants.ProdSettingsEditConnToolTipTSB;
            tmpTSB.DisplayStyle = ToolStripItemDisplayStyle.Image;
            tmpTSB.Image = global::SolimarLicenseViewer.Properties.Resources.editLicense;
            tmpTSB.Click += new EventHandler(prodConn_EditConnSettings);
            tmpTSItemList.Add(tmpTSB);

            #endregion

            #region Items for AppConstants.EventLogRootNode
            tmpTSItemList = new List<ToolStripItem>();

            tmpTSB = new ToolStripButton();
            tmpTSB.Text = AppConstants.EventLogFilterErrorTSB;
            tmpTSB.ToolTipText = AppConstants.EventLogFilterErrorToolTipTSB;
            tmpTSB.DisplayStyle = ToolStripItemDisplayStyle.ImageAndText;
            tmpTSB.Image = global::SolimarLicenseViewer.Properties.Resources.eventLogError;
            tmpTSB.Tag = new DisplayNameAndEventType(AppConstants.EventLogFilterErrorTSB, System.Diagnostics.EventLogEntryType.Error);
            tmpTSB.CheckOnClick = true;
            tmpTSB.CheckState = CheckState.Checked;
            tmpTSB.Click += new EventHandler(RefreshEventLogData);
            tmpTSItemList.Add(tmpTSB);

            tmpTSItemList.Add(new ToolStripSeparator());

            tmpTSB = new ToolStripButton();
            tmpTSB.Text = AppConstants.EventLogFilterInfoTSB;
            tmpTSB.ToolTipText = AppConstants.EventLogFilterInfoToolTipTSB;
            tmpTSB.DisplayStyle = ToolStripItemDisplayStyle.ImageAndText;
            tmpTSB.Image = global::SolimarLicenseViewer.Properties.Resources.eventLogInfo;
            tmpTSB.Tag = new DisplayNameAndEventType(AppConstants.EventLogFilterInfoTSB, System.Diagnostics.EventLogEntryType.Information);
            tmpTSB.CheckOnClick = true;
            tmpTSB.CheckState = CheckState.Checked;
            tmpTSB.Click += new EventHandler(RefreshEventLogData);
            tmpTSItemList.Add(tmpTSB);

            tmpTSItemList.Add(new ToolStripSeparator());

            tmpTSB = new ToolStripButton();
            tmpTSB.Text = AppConstants.EventLogFilterWarningTSB;
            tmpTSB.ToolTipText = AppConstants.EventLogFilterWarningToolTipTSB;
            tmpTSB.DisplayStyle = ToolStripItemDisplayStyle.ImageAndText;
            tmpTSB.Image = global::SolimarLicenseViewer.Properties.Resources.eventLogWarning;
            tmpTSB.Tag = new DisplayNameAndEventType(AppConstants.EventLogFilterWarningTSB, System.Diagnostics.EventLogEntryType.Warning);
            tmpTSB.CheckOnClick = true;
            tmpTSB.CheckState = CheckState.Checked;
            tmpTSB.Click += new EventHandler(RefreshEventLogData);
            tmpTSItemList.Add(tmpTSB);

            tmpTSItemList.Add(new ToolStripSeparator());

            tmpTSItemList.Add(new ToolStripLabel(AppConstants.EventLogSourceLabel + ": "));

            tmpTCB = new ToolStripComboBox();
            tmpTCB.Sorted = true;
            tmpTCB.DropDownStyle = ComboBoxStyle.DropDownList;
            //tmpTCB.FlatStyle = FlatStyle.Standard;
            tmpTCB.Items.Add(new EventLog_ComboSourceItem());
            tmpTCB.SelectedIndex = 0;
            //tmpTCB.Tag = ;
            //tmpTCB.Dock = DockStyle.Fill;
            tmpTCB.AutoToolTip = true;
            tmpTCB.SelectedIndexChanged += new EventHandler(RefreshEventLogData);
            //tmpTCB.
            //tmpTCB.AutoSize = true;
            tmpTCB.Width = 200;
            tmpTCB.ToolTipText = AppConstants.EventLogSourceToolTip;
            tmpTSItemList.Add(tmpTCB);

            //The Custom Solimar Product Filter...
            tmpTSItemList.Add(new ToolStripSeparator(){Tag = typeof(bool), Visible = false});
            tmpTSItemList.Add(new ToolStripLabel("Solimar Product: ") { Tag = typeof(bool), Visible = false });
            tmpTCB = new ToolStripComboBox();
            tmpTCB.Sorted = true;
            tmpTCB.DropDownStyle = ComboBoxStyle.DropDownList;
            //tmpTCB.FlatStyle = FlatStyle.Standard;
            tmpTCB.Items.Add(new EventLog_ComboSolLicSvrProductItem());
            tmpTCB.Visible = false;
            tmpTCB.SelectedIndex = 0;
            tmpTCB.Tag = typeof(bool);
            //tmpTCB.Dock = DockStyle.Fill;
            tmpTCB.AutoToolTip = true;
            tmpTCB.SelectedIndexChanged += new EventHandler(RefreshEventLogData);
            //tmpTCB.
            //tmpTCB.AutoSize = true;
            tmpTCB.Width = 200;
            tmpTCB.ToolTipText = AppConstants.EventLogSourceToolTip;
            tmpTSItemList.Add(tmpTCB);

            m_toolStripList.Add(AppConstants.EventLogRootNode, tmpTSItemList);

            #endregion

            #region Items for AppConstants.EmailAlertsNode
            tmpTSItemList = new List<ToolStripItem>();

            tmpTSL = new ToolStripLabel();
            tmpTSL.Text = string.Format("{0}:", AppConstants.EmailAlertNode);
            tmpTSItemList.Add(tmpTSL);

            tmpTSB = new ToolStripButton()
            {
                Text = AppConstants.EmailAlertTestMailTSB,
                ToolTipText = AppConstants.EmailAlertTestMailToolTipTSB,
                Alignment = ToolStripItemAlignment.Right,
            };
            tmpTSB.Click += new EventHandler(emailAlert_TestMailServer);
            tmpTSItemList.Add(tmpTSB);

            tmpTSItemList.Add(new ToolStripSeparator() { Alignment = ToolStripItemAlignment.Right });

            tmpTSB = new ToolStripButton()
            {
                Text = AppConstants.EmailAlertEditMailTSB,
                ToolTipText = AppConstants.EmailAlertEditMailToolTipTSB,
                Alignment = ToolStripItemAlignment.Right,
            };
            tmpTSB.Click += new EventHandler(emailAlert_EditMailServer);
            tmpTSItemList.Add(tmpTSB);

            tmpTSItemList.Add(new ToolStripSeparator() { Alignment = ToolStripItemAlignment.Right });

            tmpTSB = new ToolStripButton()
            {
                Text = AppConstants.EmailAlertDeleteTSB,
                ToolTipText = AppConstants.EmailAlertDeleteToolTipTSB,
                Alignment = ToolStripItemAlignment.Right,
            };
            tmpTSB.Click += new EventHandler(emailAlert_DeleteAlert);
            tmpTSItemList.Add(tmpTSB);

            tmpTSItemList.Add(new ToolStripSeparator() { Alignment = ToolStripItemAlignment.Right });

            tmpTSB = new ToolStripButton()
            {
                Text = AppConstants.EmailAlertEditSB,
                ToolTipText = AppConstants.EmailAlertEditToolTipTSB,
                Alignment = ToolStripItemAlignment.Right,
            };
            tmpTSB.Click += new EventHandler(emailAlert_EditAlert);
            tmpTSItemList.Add(tmpTSB);

            tmpTSItemList.Add(new ToolStripSeparator() { Alignment = ToolStripItemAlignment.Right });

            tmpTSB = new ToolStripButton()
            {
                Text = AppConstants.EmailAlertNewTSB,
                ToolTipText = AppConstants.EmailAlertNewToolTipTSB,
                Alignment = ToolStripItemAlignment.Right,
            };
            tmpTSB.Click += new EventHandler(emailAlert_NewAlert);
            tmpTSItemList.Add(tmpTSB);


            m_toolStripList.Add(AppConstants.EmailAlertNode, tmpTSItemList);
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
//System.Diagnostics.Trace.WriteLine("SolimarLicenseViewer.PopulateListView() - Start");
            //Clear items in listview
            this.TheListView.BeginUpdate();
            this.TheListView.MultiSelect = true;
            this.TheListView.Items.Clear();
            this.TheListView.Groups.Clear();
            this.TheListView.Reset_NoItemsMessage();
            this.TheBottomListView.BeginUpdate();
            this.TheBottomListView.Items.Clear();
            this.TheBottomListView.Groups.Clear();
            this.TheBottomListView.Reset_NoItemsMessage();

            this.TheEventLogEntryControl.Visible = false;
            this.TheBottomListView.Visible = true;
            try
            {
                Shared.VisualComponents.ControlHelper.RemoveEvents(this.TheListView, "DoubleClick");
                Shared.VisualComponents.ControlHelper.RemoveEvents(this.TheListView, "KeyDown");
                Shared.VisualComponents.ControlHelper.RemoveEvents(this.TheListView, "SelectedIndexChanged");

                TheSplitControl.Panel2Collapsed = true;
                switch (SelectedNode.Level)
                {
                    case 0:
                        if (SelectedNode.Name == AppConstants.LicenseRootNode)
                            LoadPackageData();
                        else if (SelectedNode.Name == AppConstants.ProtectionKeyRootNode)
                            LoadProtectionKeysData();
                        else if (SelectedNode.Name == AppConstants.UsageRootNode)
                            LoadUsageData();
                        else if (SelectedNode.Name == AppConstants.ProductConnectionSettingsRootNode)
                            LoadProductConnectionData();
                        else if (SelectedNode.Name == AppConstants.EventLogRootNode)
                            LoadEventLogData();
                        else if (SelectedNode.Name == AppConstants.EmailAlertNode)
                            LoadEmailAlertData();
                        break;
                    case 1:
                        if (SelectedNode.Name == AppConstants.HistoryNode)
                            LoadHistoryData();
                        else if (SelectedNode.Parent.Name == AppConstants.LicenseRootNode)
                            LoadProductData();
                        else if (SelectedNode.Parent.Name == AppConstants.UsageRootNode)
                            LoadAppInstData();
                        break;
                    case 2:
                        if (SelectedNode.Parent.Parent.Name == AppConstants.LicenseRootNode)  //load modules
                            LoadProductLicenseInfo();
                        //LoadModuleData();
                        else if (SelectedNode.Parent.Parent.Name == AppConstants.UsageRootNode)    //used app instance
                            LoadUsedModuleData(SelectedNode.Parent.Name);
                        break;
                    default:
                        break;
                }
                if (TheListView.Items.Count > 0)
                    TheListView.GridLines = true;
                else
                    TheListView.GridLines = false;
                //System.Diagnostics.Trace.WriteLine("SolimarLicenseViewer.PopulateListView() - PopulateViewColumns()");
                PopulateViewColumns();
                //System.Diagnostics.Trace.WriteLine("SolimarLicenseViewer.PopulateListView() - TheListView.Sort()");
                TheListView.Sort();
                //System.Diagnostics.Trace.WriteLine("SolimarLicenseViewer.PopulateListView() - TheBottomListView.Sort()");
                TheBottomListView.Sort();
                //System.Diagnostics.Trace.WriteLine("SolimarLicenseViewer.PopulateListView() - TheBottomListView.Sort() - Ebd");
            }
            catch (Exception)
            {
            }
            finally
            {
                Shared.VisualComponents.ListViewHelper.ResizeListViewHeadersToMaxOfDataAndHeader(TheListView);
                Shared.VisualComponents.ListViewHelper.ResizeListViewHeadersToMaxOfDataAndHeader(TheBottomListView);

                this.TheBottomListView.EndUpdate();
                this.TheListView.EndUpdate();
            }
//System.Diagnostics.Trace.WriteLine("SolimarLicenseViewer.PopulateListView() - End");
        }

        /// <summary>
        /// Populates the ListView column headers based upon the selected TreeNode of the License view.
        /// </summary>
        private void PopulateViewColumns()
        {
            //System.Diagnostics.Trace.WriteLine("PopulateViewColumns() - Start");
            TheListView.BeginUpdate();
            TheListView.SuspendLayout();
            TheListView.Columns.Clear();
            TheBottomListView.Columns.Clear();
            ColumnHeader colHeader = null;
            ListView defaultListView = null;
            int defaultColIdx = -1;
            SortOrder defaultSortOrder = SortOrder.None;
            switch (SelectedNode.Level)
            {
                case 0:
                    #region if (SelectedNode.Text == AppConstants.LicenseRootNode)
                    if (SelectedNode.Name == AppConstants.LicenseRootNode)
                    {
                        TheListView.Columns.Add(AppConstants.NameHeader);
                        TheListView.Columns.Add(AppConstants.VerificationStatusHeader);
                        //{
                        //    colHeader = new ColumnHeader();
                        //    colHeader.Text = AppConstants.ExpirationHeader;
                        //    colHeader.Tag = typeof(DateTime);
                        //    TheListView.Columns.Add(colHeader);

                        //    colHeader = new ColumnHeader();
                        //    colHeader.Text = AppConstants.CurrentActivationHeader;
                        //    colHeader.Tag = typeof(int);
                        //    colHeader.TextAlign = HorizontalAlignment.Right;
                        //    TheListView.Columns.Add(colHeader);

                        //    colHeader = new ColumnHeader();
                        //    colHeader.Text = AppConstants.TotalActivationHeader;
                        //    colHeader.Tag = typeof(int);
                        //    colHeader.TextAlign = HorizontalAlignment.Right;
                        //    TheListView.Columns.Add(colHeader);

                        //    colHeader = new ColumnHeader();
                        //    colHeader.Text = AppConstants.DaysPerActivationHeader;
                        //    colHeader.Tag = typeof(int);
                        //    colHeader.TextAlign = HorizontalAlignment.Right;
                        //    TheListView.Columns.Add(colHeader);
                        //}

                    }
                    #endregion
                    #region else if(SelectedNode.Name == AppConstants.ProtectionKeyRootNode)
                    else if (SelectedNode.Name == AppConstants.ProtectionKeyRootNode)
                    {
                        TheListView.Columns.Add(AppConstants.KeyTypeHeader);
                        TheListView.Columns.Add(AppConstants.KeyNameHeader);
                        TheListView.Columns.Add(AppConstants.KeyProductIdHeader);
                        TheListView.Columns.Add(AppConstants.KeyProductVersionHeader);
                        TheListView.Columns.Add(AppConstants.KeyLicenseTypeHeader);
                        colHeader = new ColumnHeader();
                        colHeader.Text = AppConstants.KeyDaysLeftHeader;
                        colHeader.Tag = typeof(int);
                        colHeader.TextAlign = HorizontalAlignment.Right;
                        TheListView.Columns.Add(colHeader);

                        colHeader = new ColumnHeader();
                        colHeader.Text = AppConstants.KeyExpirationDateHeader;
                        colHeader.Tag = typeof(DateTime);
                        TheListView.Columns.Add(colHeader);

                        TheBottomListView.Columns.Add(AppConstants.PkModuleHeader);
                        TheBottomListView.Columns.Add(AppConstants.PkModuleTotalHeader);
                        TheBottomListView.Columns.Add(AppConstants.PkModuleInUseHeader);

                    }
                    #endregion
                    #region else if(SelectedNode.Name == AppConstants.UsageRootNode)
                    else if (SelectedNode.Name == AppConstants.UsageRootNode)
                    {
                        TheListView.Columns.Add(AppConstants.UsageProductHeader);
                    }
                    #endregion
                    #region else if(SelectedNode.Name == AppConstants.ProductConnectionSettingsRootNode)
                    else if (SelectedNode.Name == AppConstants.ProductConnectionSettingsRootNode)
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
                    else if (SelectedNode.Name == AppConstants.EventLogRootNode)
                    {
                        List<ColumnHeader> colHeaderList = new List<ColumnHeader>();
                        colHeaderList.Add(new ColumnHeader() { Text = AppConstants.EventLogLevelHeader });
                        colHeaderList.Add(new ColumnHeader()
                        {
                            Text = AppConstants.EventLogDateTimeHeader,
                            Tag = typeof(DateTime)
                        });
                        colHeaderList.Add(new ColumnHeader() { Text = AppConstants.EventLogSourceHeader });
                        colHeaderList.Add(new ColumnHeader()
                        {
                            Text = AppConstants.EventLogEventIdHeader,
                            Tag = typeof(int),
                            TextAlign = HorizontalAlignment.Right
                        });
                        colHeaderList.Add(new ColumnHeader() { Text = AppConstants.EventLogTaskCategoryHeader });

                        TheListView.Columns.AddRange(colHeaderList.ToArray());

                        //Sort by DateTime Default
                        defaultListView = TheListView;
                        defaultColIdx = 1;
                        defaultSortOrder = SortOrder.Descending;
                    }
                    else if (SelectedNode.Name == AppConstants.EmailAlertNode)
                    {
                        List<ColumnHeader> colHeaderList = new List<ColumnHeader>();
                        colHeaderList.Add(new ColumnHeader() { Text = AppConstants.EmailAlertNameHeader });
                        colHeaderList.Add(new ColumnHeader() { Text = AppConstants.EmailAlertActiveHeader });
                        colHeaderList.Add(new ColumnHeader() { Text = AppConstants.EmailAlertRecipentListHeader });
                        colHeaderList.Add(new ColumnHeader() { Text = AppConstants.EmailAlertIdListHeader });

                        TheListView.Columns.AddRange(colHeaderList.ToArray());
                    }
                    break;
                case 1:
                    #region if (SelectedNode.Name == AppConstants.HistoryNode)
                    if (SelectedNode.Name == AppConstants.HistoryNode)
                    {
                        TheListView.Columns.Add(AppConstants.NameHeader);
                        //TheListView.Columns.Add(AppConstants.VerificationCodeHeader);
                        colHeader = new ColumnHeader();
                        colHeader.Text = AppConstants.DateAppliedHeader;
                        colHeader.Tag = typeof(DateTime);
                        TheListView.Columns.Add(colHeader);
                    }
                    #endregion
                    #region else if (SelectedNode.Parent.Name == AppConstants.LicenseRootNode)
                    else if (SelectedNode.Parent.Name == AppConstants.LicenseRootNode)
                    {
                        TheListView.Columns.Add(AppConstants.NameHeader);
                        TheListView.Columns.Add(AppConstants.VersionHeader);
                        colHeader = new ColumnHeader();
                        colHeader.Text = AppConstants.AppInstanceHeader;
                        colHeader.Tag = typeof(DateTime);
                        TheListView.Columns.Add(colHeader);
                    }
                    #endregion
                    #region else if (SelectedNode.Parent.Name == AppConstants.UsageRootNode)
                    else if (SelectedNode.Parent.Name == AppConstants.UsageRootNode)
                    {
                        TheListView.Columns.Add(AppConstants.UsageAppInstanceHeader);
                    }
                    #endregion
                    break;
                case 2:
                    #region if (SelectedNode.Parent.Parent.Name == AppConstants.LicenseRootNode)

                    if (SelectedNode.Parent.Parent.Name == AppConstants.LicenseRootNode)
                    {
                        //ProdLicNumHeader

                        ListView prodLicListView = TheListView;
                        ListView moduleDataListView = TheBottomListView;
                        #region Populate Columns for TheListView
                        colHeader = new ColumnHeader();
                        colHeader.Text = AppConstants.ProdLicNumHeader;
                        colHeader.Tag = typeof(string);
                        prodLicListView.Columns.Add(colHeader);

                        colHeader = new ColumnHeader();
                        colHeader.Text = AppConstants.ProdLicNumStateHeader;
                        colHeader.Tag = typeof(string);
                        prodLicListView.Columns.Add(colHeader);

                        colHeader = new ColumnHeader();
                        colHeader.Text = AppConstants.ExpirationHeader;
                        colHeader.Tag = typeof(DateTime);
                        prodLicListView.Columns.Add(colHeader);

                        colHeader = new ColumnHeader();
                        colHeader.Text = AppConstants.CurrentActivationHeader;
                        colHeader.Tag = typeof(string);
                        colHeader.TextAlign = HorizontalAlignment.Right;
                        prodLicListView.Columns.Add(colHeader);

                        //colHeader = new ColumnHeader();
                        //colHeader.Text = AppConstants.CurrentActivationHeader;
                        //colHeader.Tag = typeof(int);
                        //colHeader.TextAlign = HorizontalAlignment.Right;
                        //prodLicListView.Columns.Add(colHeader);

                        //colHeader = new ColumnHeader();
                        //colHeader.Text = AppConstants.TotalActivationHeader;
                        //colHeader.Tag = typeof(int);
                        //colHeader.TextAlign = HorizontalAlignment.Right;
                        //prodLicListView.Columns.Add(colHeader);

                        colHeader = new ColumnHeader();
                        colHeader.Text = AppConstants.DaysPerActivationHeader;
                        colHeader.Tag = typeof(int);
                        colHeader.TextAlign = HorizontalAlignment.Right;
                        prodLicListView.Columns.Add(colHeader);

                        colHeader = new ColumnHeader();
                        colHeader.Text = AppConstants.ActivationExpirationHeader;
                        colHeader.Tag = typeof(DateTime);
                        colHeader.TextAlign = HorizontalAlignment.Left;
                        prodLicListView.Columns.Add(colHeader);

                        #endregion

                        #region Populate Columns for TheBottomListView
                        colHeader = new ColumnHeader();
                        colHeader.Text = AppConstants.UnitsHeader;
                        colHeader.Tag = typeof(string);
                        moduleDataListView.Columns.Add(colHeader);

                        colHeader = new ColumnHeader();
                        colHeader.Text = AppConstants.LicensesHeader;
                        colHeader.Tag = typeof(int);
                        colHeader.TextAlign = HorizontalAlignment.Right;
                        moduleDataListView.Columns.Add(colHeader);

                        colHeader = new ColumnHeader();
                        colHeader.Text = AppConstants.AppInstanceHeader;
                        colHeader.Tag = typeof(int);
                        colHeader.TextAlign = HorizontalAlignment.Right;
                        moduleDataListView.Columns.Add(colHeader);

                        if (m_moduleFilterComboBox.SelectedIndex == 1/*GroupByModule*/)
                        {
                            colHeader = new ColumnHeader();
                            colHeader.Text = AppConstants.AppInstanceExpiredHeader;
                            colHeader.Tag = typeof(int);
                            colHeader.TextAlign = HorizontalAlignment.Right;
                            moduleDataListView.Columns.Add(colHeader);
                        }
                        else//if((m_moduleFilterComboBox.SelectedIndex == 0/*Details*/) || (m_moduleFilterComboBox.SelectedIndex == 2/*GroupByProductLicNumber*/))
                        {
                            colHeader = new ColumnHeader();
                            colHeader.Text = AppConstants.ExpirationHeader;
                            colHeader.Tag = typeof(DateTime);
                            moduleDataListView.Columns.Add(colHeader);

                            colHeader = new ColumnHeader();
                            colHeader.Text = AppConstants.ProdLicNumHeader;
                            colHeader.Tag = typeof(string);
                            moduleDataListView.Columns.Add(colHeader);
                        }
                        #endregion
                    }
                    #endregion
                    #region else if (SelectedNode.Parent.Parent.Name == AppConstants.UsageRootNode)
                    else if (SelectedNode.Parent.Parent.Name == AppConstants.UsageRootNode)
                    {
                        TheListView.Columns.Add(AppConstants.UsageModuleHeader);
                        colHeader = new ColumnHeader();
                        colHeader.Text = AppConstants.ObtainedHeader;
                        colHeader.Tag = typeof(int);
                        colHeader.TextAlign = HorizontalAlignment.Right;
                        TheListView.Columns.Add(colHeader);

                        colHeader = new ColumnHeader();
                        colHeader.Text = AppConstants.TotalHeader;
                        colHeader.Tag = typeof(int);
                        colHeader.TextAlign = HorizontalAlignment.Right;
                        TheListView.Columns.Add(colHeader);
                    }
                    #endregion
                    break;
                default:
                    break;
            }
            TheListView.Columns.Add("");    //Extra Cell at the end
            TheBottomListView.Columns.Add("");    //Extra Cell at the end
            ResetListViewColumnSorter(TheListView);
            ResetListViewColumnSorter(TheBottomListView);
            if (defaultListView != null)
                SetListViewColumnSorter(defaultListView, defaultColIdx, defaultSortOrder);
            TheListView.ResumeLayout();
            TheListView.EndUpdate();
            //System.Diagnostics.Trace.WriteLine("PopulateViewColumns() - End");
        }

		private void PopulateBottomViewColumns_ModuleInfo(bool _bDisplayDetails)
		{
			TheBottomListView.BeginUpdate();
			TheBottomListView.Columns.Clear();
			ColumnHeader colHeader = null;
			if (SelectedNode.Parent.Parent.Text == AppConstants.LicenseRootNode)
			{
				ListView moduleDataListView = TheBottomListView;
				#region Populate Columns for TheBottomListView
				colHeader = new ColumnHeader();
				colHeader.Text = AppConstants.UnitsHeader;
				colHeader.Tag = typeof(string);
				moduleDataListView.Columns.Add(colHeader);

				colHeader = new ColumnHeader();
				colHeader.Text = AppConstants.LicensesHeader;
				colHeader.Tag = typeof(int);
				colHeader.TextAlign = HorizontalAlignment.Right;
				moduleDataListView.Columns.Add(colHeader);

				colHeader = new ColumnHeader();
				colHeader.Text = AppConstants.AppInstanceHeader;
				colHeader.Tag = typeof(int);
				colHeader.TextAlign = HorizontalAlignment.Right;
				moduleDataListView.Columns.Add(colHeader);

				if (_bDisplayDetails == false/*GroupByModule*/)
				{
					colHeader = new ColumnHeader();
					colHeader.Text = AppConstants.AppInstanceExpiredHeader;
					colHeader.Tag = typeof(int);
					colHeader.TextAlign = HorizontalAlignment.Right;
					moduleDataListView.Columns.Add(colHeader);
				}
				else //if(_bDisplayDetails == false/*Details*/)
				{
					colHeader = new ColumnHeader();
					colHeader.Text = AppConstants.ExpirationHeader;
					colHeader.Tag = typeof(DateTime);
					moduleDataListView.Columns.Add(colHeader);

					colHeader = new ColumnHeader();
					colHeader.Text = AppConstants.ProdLicNumHeader;
					colHeader.Tag = typeof(string);
					moduleDataListView.Columns.Add(colHeader);
				}
				#endregion
			}

			TheBottomListView.Columns.Add("");    //Extra Cell at the end
			ResetListViewColumnSorter(TheBottomListView);
			TheBottomListView.EndUpdate();
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

                    //if (licInfoAttrib.activationCurrent.TVal == 0 &&
                    //    licInfoAttrib.activationTotal.TVal == 0 &&
                    //    licInfoAttrib.activationAmountInDays.TVal == 0)
                    //{
                    //    //m_DisasterRecoveryMode = false;
                    //    lvItem.SubItems.Add(SolimarLicenseViewer.AppConstants.NotUsedStatus);
                    //    lvItem.SubItems.Add(SolimarLicenseViewer.AppConstants.NotUsedStatus);
                    //    lvItem.SubItems.Add(SolimarLicenseViewer.AppConstants.NotUsedStatus);
                    //    lvItem.SubItems.Add(SolimarLicenseViewer.AppConstants.NotUsedStatus);
                    //}
                    //else
                    //{
                    //    //if (toolTipBuilder.Length != 0)
                    //    //    toolTipBuilder.Append("");
                    //    toolTipBuilder.Append("\n");
                    //    toolTipBuilder.Append(AppConstants.ExpirationHeader);
                    //    toolTipBuilder.Append(": ");
                    //    toolTipBuilder.Append(licInfoAttrib.activationExpirationDate.TVal.ToLocalTime().ToString());

                    //    toolTipBuilder.Append("\n");
                    //    toolTipBuilder.Append(AppConstants.CurrentActivationHeader);
                    //    toolTipBuilder.Append(string.Format(": {0} (of {1})\n", licInfoAttrib.activationCurrent.TVal, licInfoAttrib.activationTotal.TVal));
                    //    //toolTipBuilder.Append(AppConstants.CurrentActivationHeader);
                    //    //toolTipBuilder.Append(": ");
                    //    //toolTipBuilder.Append(licInfoAttrib.activationCurrent.TVal.ToString());
                    //    //toolTipBuilder.Append("\n");
                    //    //toolTipBuilder.Append(AppConstants.TotalActivationHeader);
                    //    //toolTipBuilder.Append(": ");
                    //    //toolTipBuilder.Append(licInfoAttrib.activationTotal.TVal.ToString());
                    //    //toolTipBuilder.Append("\n");
                    //    toolTipBuilder.Append(AppConstants.DaysPerActivationHeader);
                    //    toolTipBuilder.Append(": ");
                    //    toolTipBuilder.Append(licInfoAttrib.activationAmountInDays.TVal.ToString());

                    //    if (licInfoAttrib.activationCurrent.TVal < licInfoAttrib.activationTotal.TVal)
                    //        enableActivationByLicense[softwareLicense] = true;  //m_EnableDisasterRecoverExt = true;
                    //    else
                    //        enableActivationByLicense[softwareLicense] = false;  //m_EnableDisasterRecoverExt = false;
                    //    lvItem.SubItems.Add(licInfoAttrib.activationExpirationDate.TVal.ToLocalTime().ToString());
                    //    lvItem.SubItems.Add(licInfoAttrib.activationCurrent.TVal.ToString());
                    //    lvItem.SubItems.Add(licInfoAttrib.activationTotal.TVal.ToString());
                    //    lvItem.SubItems.Add(licInfoAttrib.activationAmountInDays.TVal.ToString());
                    //    //m_DisasterRecoveryMode = true;
                    //}
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
                    string productName = m_CommLink.GetProductName((int)prodInfo.productID.TVal);

                    ListViewItem lvItem = FindListViewItem(this.TheListView.Items, productName) ?? FindListViewItem(lviList, productName);
                    // Line above is same as commented out code
                    //ListViewItem lvItem = FindListViewItem(this.TheListView.Items, productName);
                    //if (lvItem == null)
                    //    lvItem = FindListViewItem(lviList, productName);
                    
                    bool bNewItem = (lvItem == null);
                    if (bNewItem == true)
                    {
                        lvItem = new ListViewItem();
                        lvItem.Text = productName;

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

                        bool bExpired = false;
                        if (prodInfo.bUseExpirationDate.TVal == true)
                            bExpired = DateTime.Compare(DateTime.Now, prodInfo.expirationDate.TVal.ToLocalTime()) > 0;

                        if (!bExpired && prodInfo.bUseActivations.TVal == true)
                            bExpired = DateTime.Compare(DateTime.Now, prodInfo.activationCurrentExpirationDate.TVal.ToLocalTime()) > 0;

                        lvItem.SubItems.Add(bExpired ? "0" : prodInfo.productAppInstance.TVal.ToString());
                        lvItem.ForeColor = m_TreeNode.ForeColor;
                        //lvItem.SubItems.Add(prodInfo.productExpirationDate.TVal.ToLocalTime().ToString());

                        lviList.Add(lvItem);
                    }
                    else
                    {
                        //Update existing item - subitem 2 has the prodAppInstance, update.
                        try
                        {
                            bool bExpired = false;
                            if (prodInfo.bUseExpirationDate.TVal == true)
                                bExpired = DateTime.Compare(DateTime.Now, prodInfo.expirationDate.TVal.ToLocalTime()) > 0;

                            if (!bExpired && prodInfo.bUseActivations.TVal == true)
                                bExpired = DateTime.Compare(DateTime.Now, prodInfo.activationCurrentExpirationDate.TVal.ToLocalTime()) > 0;

                            Int64 appInstance = Convert.ToInt64(lvItem.SubItems[2].Text);
                            if (!bExpired)
                                appInstance += prodInfo.productAppInstance.TVal;
                            lvItem.SubItems[2].Text = appInstance.ToString();
                            //lvItem.SubItems[2].Text = string.Format("0x{0:x}", appInstance + prodInfo.productAppInstance.TVal);

                        }
                        catch (Exception)
                        { }
                    }
                }

                this.TheListView.Items.AddRange(lviList.ToArray());
            }
            catch (COMException ex)
            {
                this.TheListView.NoItemsMessage = ex.ToString();
            }
        }

        /// <summary>
        /// Loads module information into the ListView. Populates the name, value, in use, application instances, expiration date columns.
        /// </summary>
        public void LoadProductLicenseInfo()
        {
            int productID = 0;
            String generalStream = "";
            Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs licInfoAttrib = new Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs();
            try
            {
                if (m_toolStripList.ContainsKey(AppConstants.LicenseRootNode))
                {

                    this.TheListViewToolStrip.Visible = true;
                    this.TheListViewToolStrip.Items.Clear();
                    this.TheListViewToolStrip.Items.AddRange(m_toolStripList[AppConstants.LicenseRootNode].ToArray());
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

                this.TheListView.MultiSelect = false;
                Shared.VisualComponents.ControlHelper.RemoveEvents(this.TheListView, "SelectedIndexChanged");
                this.TheListView.SelectedIndexChanged += new EventHandler(TheListView_SelectedIndexChanged);

                m_CommLink.GetSoftwareLicenseInfoByLicense(m_TreeNode.Parent.Name, ref generalStream);
                licInfoAttrib.AssignMembersFromStream(generalStream);

                foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs prodInfo in licInfoAttrib.productList.TVal)
                {
                    productID = (int)prodInfo.productID.TVal;
                    if (string.Equals(m_CommLink.GetProductName(productID), m_TreeNode.Text))
                    {
                        bool bExpired = false;
                        ListViewItem lvi = new ListViewItem();
                        lvi.Text = prodInfo.contractNumber;
                        lvi.SubItems.Add(prodInfo.productState.GetAlias());
                        string expirationDate = SolimarLicenseViewer.AppConstants.NotUsedStatus;
                        string activationCurrent = SolimarLicenseViewer.AppConstants.NotUsedStatus;
                        string activationTotal = SolimarLicenseViewer.AppConstants.NotUsedStatus;
                        string activationAmountInDays = SolimarLicenseViewer.AppConstants.NotUsedStatus;
                        string activationCurrentExpirationDate = SolimarLicenseViewer.AppConstants.NotUsedStatus;
                        enableActivationExtByProductLicense[prodInfo.contractNumber] = prodInfo.bUseActivations;

                        lvi.Tag = new object[] 
                        { 
                            m_TreeNode.Parent.Name, 
                            (int)prodInfo.activationCurrent.TVal, 
                            (int)prodInfo.activationTotal.TVal, 
                            (int)prodInfo.activationAmountInDays.TVal,
                            prodInfo.activationCurrentExpirationDate.TVal,
                            prodInfo.expirationDate.TVal
                        };

                        if (prodInfo.bUseExpirationDate.TVal == true || prodInfo.bUseActivations.TVal == true)
                        {
                            if (prodInfo.bUseExpirationDate.TVal == true)
                                expirationDate = prodInfo.expirationDate.TVal.ToLocalTime().ToString();

                            if (prodInfo.bUseActivations.TVal == true)
                            {
                                activationCurrent = string.Format("{0} (of {1})", prodInfo.activationCurrent.TVal, prodInfo.activationTotal.TVal);
                                activationTotal = prodInfo.activationTotal.TVal.ToString();
                                activationAmountInDays = prodInfo.activationAmountInDays.TVal.ToString();
                                bExpired = DateTime.Compare(DateTime.Now, prodInfo.activationCurrentExpirationDate.TVal.ToLocalTime()) > 0;
                                activationCurrentExpirationDate = !bExpired ? prodInfo.activationCurrentExpirationDate.TVal.ToLocalTime().ToString() : SolimarLicenseViewer.AppConstants.ExpiredStatus;
                            }

                            if (!bExpired && prodInfo.bUseExpirationDate.TVal == true)
                                bExpired = DateTime.Compare(DateTime.Now, prodInfo.expirationDate.TVal.ToLocalTime()) > 0;
                        }

                        lvi.SubItems.Add(expirationDate);
                        lvi.SubItems.Add(activationCurrent);
                        //lvi.SubItems.Add(activationTotal);
                        lvi.SubItems.Add(activationAmountInDays);
                        lvi.SubItems.Add(activationCurrentExpirationDate);
                        if (bExpired == true)
                            lvi.ForeColor = System.Drawing.Color.Red;
                        this.TheListView.Items.Add(lvi);
                    }
                }

                //Add Summary of all All Product License Numbers only if there are multiple Product License Numbers
                if (this.TheListView.Items.Count >= 2)
                {
                    ListViewItem lvi = new ListViewItem();
                    lvi.Text = AppConstants.SummaryAllProdLicNum;
                    lvi.SubItems.Add("-");
                    lvi.SubItems.Add("-");
                    lvi.SubItems.Add("-");
                    //lvi.SubItems.Add("-");
                    lvi.SubItems.Add("-");
                    lvi.SubItems.Add("-");
                    //lvi.SubItems.Add(SolimarLicenseViewer.AppConstants.NotUsedStatus);
                    //lvi.SubItems.Add(SolimarLicenseViewer.AppConstants.NotUsedStatus);
                    //lvi.SubItems.Add(SolimarLicenseViewer.AppConstants.NotUsedStatus);
                    //lvi.SubItems.Add(SolimarLicenseViewer.AppConstants.NotUsedStatus);
                    //lvi.SubItems.Add(SolimarLicenseViewer.AppConstants.NotUsedStatus);
                    //lvi.SubItems.Add(SolimarLicenseViewer.AppConstants.NotUsedStatus);

                    this.TheListView.Items.Add(lvi);
                }



                if (this.TheListView.Items.Count > 0)
                    this.TheListView.Items[0].Selected = true;
            }
            catch (COMException)
            {
            }


        }
        public void LoadModuleData(string _prodLicNumber)
        {
            int productID = 0, moduleID = 0;
            String generalStream = "";

            Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs licInfoAttrib = new Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs();
            try
            {
                this.TheBottomListView.BeginUpdate();
                this.TheBottomListView.Items.Clear();

                bool bDisplayAllModuleInfo = string.Compare(_prodLicNumber, AppConstants.SummaryAllProdLicNum, true) == 0;

                //LicenseRootNode
                if (bDisplayAllModuleInfo == true)
                {
                    if (m_toolStripList.ContainsKey(AppConstants.BottomUnitsHeader))
                    {
                        this.TheBottomListViewToolStrip.Visible = true;
                        this.TheBottomListViewToolStrip.Items.Clear();
                        this.TheBottomListViewToolStrip.Items.AddRange(m_toolStripList[AppConstants.BottomUnitsHeader].ToArray());
                        //this.TheBottomListViewToolStrip.Items[0].Text = string.Format("{0}: {1}", AppConstants.ProdLicNumHeader, _prodLicNumber);
                        this.TheBottomListViewToolStrip.Items[0].Text = AppConstants.SummaryAllProdLicNum;
                    }
                    else
                    {
                        this.TheBottomListViewToolStrip.Visible = false;
                    }
                }
                else if (bDisplayAllModuleInfo == false)
                {
                    if (m_toolStripList.ContainsKey(AppConstants.BottomLicenseRootNode))
                    {

                        this.TheBottomListViewToolStrip.Visible = true;
                        this.TheBottomListViewToolStrip.Items.Clear();
                        this.TheBottomListViewToolStrip.Items.AddRange(m_toolStripList[AppConstants.BottomLicenseRootNode].ToArray());
                        this.TheBottomListViewToolStrip.Items[0].Text = string.Format("{0}: {1}", AppConstants.ProdLicNumHeader, _prodLicNumber);
                    }
                    else
                    {
                        this.TheBottomListViewToolStrip.Visible = false;
                    }
                }


                //non-summary view will be forced to be details view.
                bool bDisplayGroupByModule = false;
                bool bDisplayDetails = false;
                if (bDisplayAllModuleInfo)
                {
                    bDisplayGroupByModule = (m_moduleFilterComboBox.SelectedIndex == 1/*GroupByModule*/);
                    bDisplayDetails = !bDisplayGroupByModule && (m_moduleFilterComboBox.SelectedIndex == 0/*Details*/);
                }
                else
                {
                    bDisplayGroupByModule = false;
                    bDisplayDetails = true;
                }
                PopulateBottomViewColumns_ModuleInfo(bDisplayDetails);

                m_CommLink.GetSoftwareLicenseInfoByLicense(m_TreeNode.Parent.Name, ref generalStream);
                licInfoAttrib.AssignMembersFromStream(generalStream);

                bool bClockViolation_AllModuleWithExpireDate = licInfoAttrib.bLicClockViolation.TVal;

                System.Collections.Generic.List<ListViewItem> lviList = new List<ListViewItem>();
                System.Collections.Generic.SortedDictionary<string/*ProductLicenseNumber*/, ListViewGroup> prodLicNumberToLvGroupMap = null;

                //For - bDisplayGroupByModule - Entry - 1st: ValidModValueSum, 2nd: ValidModAppInstanceSum, 3rd: ExpiredModAppInstanceSum
                System.Collections.Generic.SortedDictionary<int, int[]> sumModInfoList = new SortedDictionary<int, int[]>();
                System.Collections.Generic.SortedDictionary<int, System.Collections.Generic.SortedList<DateTime, int>> modExpirationDateList = new SortedDictionary<int, System.Collections.Generic.SortedList<DateTime, int>>();

                //if(!bDisplayGroupByModule && !bDisplayDetails) then filter GroupByProductNumber
                if (!bDisplayGroupByModule && !bDisplayDetails)
                    prodLicNumberToLvGroupMap = new SortedDictionary<string, ListViewGroup>();
                foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs prodInfo in licInfoAttrib.productList.TVal)
                {
                    int tmpProductID = (int)prodInfo.productID.TVal;
                    if (string.Equals(m_CommLink.GetProductName(tmpProductID), m_TreeNode.Text) &&
                        (bDisplayAllModuleInfo || string.Equals(_prodLicNumber, prodInfo.contractNumber)))
                    //string.Equals(_prodLicNumber, prodInfo.contractNumber))
                    {
                        productID = tmpProductID;

                        //Calculate expiration date on product license, apply it to it's child modules.
                        DateTime? expirationDate = null;
                        string expirationDateText = null;
                        bool bExpired = false;
                        if (prodInfo.bUseActivations.TVal == true || prodInfo.bUseExpirationDate.TVal == true)
                        {
                            DateTime? activationExpirationDate = null;
                            DateTime? prodLicNumberExpirationDate = null;
                            if (prodInfo.bUseActivations.TVal == true)
                                activationExpirationDate = prodInfo.activationCurrentExpirationDate.TVal;
                            if (prodInfo.bUseExpirationDate.TVal == true)
                                prodLicNumberExpirationDate = prodInfo.expirationDate.TVal;

                            bool? bUseActivationExpiration = null;
                            if (activationExpirationDate != null && prodLicNumberExpirationDate != null)
                                bUseActivationExpiration = (DateTime.Compare(activationExpirationDate.Value, prodLicNumberExpirationDate.Value) < 0);
                            else if (activationExpirationDate == null && prodLicNumberExpirationDate != null)
                                bUseActivationExpiration = false;
                            else if (activationExpirationDate != null && prodLicNumberExpirationDate == null)
                                bUseActivationExpiration = true;

                            if (bUseActivationExpiration != null)
                            {
                                //Use to display Activation Expired
                                expirationDate = bUseActivationExpiration.Value ? activationExpirationDate.Value : prodLicNumberExpirationDate.Value;
                                bExpired = (expirationDate != null) && (DateTime.Compare(DateTime.Now, expirationDate.Value.ToLocalTime()) > 0);
                                expirationDateText = (bExpired && bUseActivationExpiration.Value) ? SolimarLicenseViewer.AppConstants.ExpiredStatus : expirationDate.Value.ToLocalTime().ToString();
                            }
                        }
                        if (bClockViolation_AllModuleWithExpireDate)
                        {
                            bExpired = bClockViolation_AllModuleWithExpireDate;
                            expirationDateText = "System Clock Violation";
                        }

                        #region if (bDisplayGroupByModule)    //Grouping
                        if (bDisplayGroupByModule)    //Grouping
                        {
                            //Entry - 1st: ValidModValueSum, 2nd: ValidModAppInstanceSum, 3rd: ExpiredModAppInstanceSum
                            //System.Collections.Generic.SortedDictionary<int, int[]> sumModInfoList = new SortedDictionary<int, int[]>();
                            //System.Collections.Generic.SortedDictionary<int, System.Collections.Generic.SortedList<DateTime, int>> modExpirationDateList = new SortedDictionary<int, System.Collections.Generic.SortedList<DateTime, int>>();
                            foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs modInfo in prodInfo.moduleList.TVal)
                            {
                                if (modInfo.moduleAppInstance != 0 || modInfo.moduleValue != 0)
                                {
                                    moduleID = (int)modInfo.moduleID.TVal;
                                    if (!sumModInfoList.ContainsKey(moduleID))
                                        sumModInfoList.Add(moduleID, new int[3]);

                                    bool bHasExpirationDate = expirationDate != null;

                                    if (bHasExpirationDate)
                                    {
                                        if (!modExpirationDateList.ContainsKey(moduleID))
                                            modExpirationDateList.Add(moduleID, new SortedList<DateTime, int>());

                                        if (!modExpirationDateList[moduleID].ContainsKey(expirationDate.Value))
                                            modExpirationDateList[moduleID].Add(expirationDate.Value, 0);
                                        modExpirationDateList[moduleID][expirationDate.Value] += (int)modInfo.moduleAppInstance.TVal;
                                    }

                                    if (bExpired)
                                    {
                                        sumModInfoList[moduleID][2] += (int)modInfo.moduleAppInstance.TVal;
                                    }
                                    else //Not Expired
                                    {
                                        sumModInfoList[moduleID][0] += (int)modInfo.moduleValue.TVal;
                                        sumModInfoList[moduleID][1] += (int)modInfo.moduleAppInstance.TVal;
                                    }
                                }
                            }
                        }
                        #endregion
                        #region else //Details or ViewByProductLicenseNumber
                        else //Details or ViewByProductLicenseNumber
                        {
                            foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs modInfo in prodInfo.moduleList.TVal)
                            {
                                if (modInfo.moduleAppInstance != 0 || modInfo.moduleValue != 0)
                                {
                                    //Get Module Info using product id and module id
                                    ListViewItem lvItem = new ListViewItem();
                                    moduleID = (int)modInfo.moduleID.TVal;
                                    lvItem.Text = m_CommLink.GetModuleName(productID, moduleID);
                                    if (bExpired)
                                    {
                                        lvItem.ForeColor = System.Drawing.Color.Red;
                                        lvItem.ToolTipText = string.Format("Module Expired on: {0}", expirationDateText);
                                    }
                                    else
                                        lvItem.ForeColor = m_TreeNode.ForeColor;

                                    lvItem.SubItems.Add(modInfo.moduleValue.TVal.ToString());
                                    lvItem.SubItems.Add(modInfo.moduleAppInstance.TVal.ToString());
                                    if (expirationDateText != null)
                                        lvItem.SubItems.Add(expirationDateText);
                                    else
                                        lvItem.SubItems.Add("");
                                    lvItem.SubItems.Add(prodInfo.contractNumber);
                                    if (prodLicNumberToLvGroupMap != null)
                                    {
                                        if (!prodLicNumberToLvGroupMap.ContainsKey(prodInfo.contractNumber))
                                        {
                                            prodLicNumberToLvGroupMap[prodInfo.contractNumber] = new ListViewGroup("Product License: " + prodInfo.contractNumber);
                                        }
                                        lvItem.Group = prodLicNumberToLvGroupMap[prodInfo.contractNumber];
                                    }
                                    lviList.Add(lvItem);
                                }
                            }
                        }
                        #endregion
                    }
                }

                #region if (bDisplayGroupByModule)    //Grouping
                if (bDisplayGroupByModule)
                {
                    foreach (System.Collections.Generic.KeyValuePair<int/*ModuleID*/, int[]> keyPair in sumModInfoList)
                    {
                        ListViewItem lvItem = new ListViewItem();
                        lvItem.Text = m_CommLink.GetModuleName(productID, keyPair.Key);
                        //lvItem.Text = m_CommLink.GetModuleName(productID, keyPair.Key) + ", " + prodInfo.contractNumber;

                        lvItem.SubItems.Add(keyPair.Value[0].ToString());
                        lvItem.SubItems.Add(keyPair.Value[1].ToString());
                        lvItem.SubItems.Add(keyPair.Value[2].ToString());

                        if (keyPair.Value[0] == 0)
                            lvItem.ForeColor = System.Drawing.Color.Red;
                        else
                            lvItem.ForeColor = m_TreeNode.ForeColor;

                        StringBuilder toolTipSB = new StringBuilder();
                        if (modExpirationDateList.ContainsKey(keyPair.Key) == true)
                        {
                            if (bClockViolation_AllModuleWithExpireDate)
                            {
                                toolTipSB.Append(keyPair.Value[2].ToString());
                                if (keyPair.Value[2] > 1)
                                    toolTipSB.Append(" Modules'");
                                else
                                    toolTipSB.Append(" Module's");
                                toolTipSB.Append(" Product Connections expired because of System Clock Violation");
                            }
                            else
                            {
                                foreach (System.Collections.Generic.KeyValuePair<DateTime, int> dTimeToIntKVPair in modExpirationDateList[keyPair.Key])
                                {
                                    if (toolTipSB.Length != 0)
                                        toolTipSB.Append("\n");
                                    toolTipSB.Append(dTimeToIntKVPair.Value.ToString());
                                    if (dTimeToIntKVPair.Value > 1)
                                        toolTipSB.Append(" Modules'");
                                    else
                                        toolTipSB.Append(" Module's");
                                    toolTipSB.Append(" Product Connections");

                                    if (DateTime.Now.ToUniversalTime().CompareTo(dTimeToIntKVPair.Key) > 0)
                                        toolTipSB.Append(" expired on: ");
                                    else
                                        toolTipSB.Append(" will expire on: ");

                                    if (!bClockViolation_AllModuleWithExpireDate)
                                        toolTipSB.Append(dTimeToIntKVPair.Key.ToLocalTime().ToString());
                                }
                            }
                        }
                        lvItem.ToolTipText = toolTipSB.ToString();
                        lviList.Add(lvItem);
                    }
                }
                #endregion

                this.TheBottomListView.Items.AddRange(lviList.ToArray());
                if (prodLicNumberToLvGroupMap != null)
                {
                    System.Collections.Generic.SortedDictionary<ProductLicenseNumber, ListViewGroup> sortedProdLicNumberToLvGroupMap = new System.Collections.Generic.SortedDictionary<ProductLicenseNumber, ListViewGroup>();
                    foreach (KeyValuePair<string/*ProductLicenseNumber*/, ListViewGroup> kvp in prodLicNumberToLvGroupMap)
                        sortedProdLicNumberToLvGroupMap.Add(new ProductLicenseNumber(kvp.Key), kvp.Value);
                    foreach (KeyValuePair<ProductLicenseNumber, ListViewGroup> kvp in sortedProdLicNumberToLvGroupMap)
                        this.TheBottomListView.Groups.Add(kvp.Value);
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

                Shared.VisualComponents.ListViewHelper.ResizeListViewHeadersToMaxOfDataAndHeader(TheBottomListView);
                this.TheBottomListView.EndUpdate();
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
                        //lvItem.SubItems.Add(verToken.verificationValue);
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
                this.TheListView.MultiSelect = false;
                Shared.VisualComponents.ControlHelper.RemoveEvents(this.TheListView, "SelectedIndexChanged");
                this.TheListView.SelectedIndexChanged += new EventHandler(TheListView_SelectedIndexChanged);

                foreach (Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseProtectionKeyInfo keyInfo in m_CommLink.KeyEnumerate())
                {
                    ListViewItem lvItem = new ListViewItem();
                    lvItem.Text = "Unknown Key Type";

                    if (keyInfo != null)
                    {
                        //System.Diagnostics.Trace.WriteLine("LoadProtectionKeysData() keyName: " + keyInfo.keyName + ", productName: " + keyInfo.productName);
                        lvItem.Tag = keyInfo.keyName;
                        lvItem.SubItems.Add(keyInfo.keyName);

                        if (keyInfo.applicationInstance > 0)
                            lvItem.ForeColor = System.Drawing.Color.SteelBlue;
                        if (!keyInfo.bIsActive && !keyInfo.IsKeyTypeVerification() && !keyInfo.IsKeyTypeUninitialized() && !m_CommLink.bDiagnosticDateView)
                        {
                            lvItem.Font = new System.Drawing.Font(lvItem.Font, System.Drawing.FontStyle.Italic);
                            lvItem.ForeColor = System.Drawing.Color.Red;
                            if (keyInfo.IsKeyTypeDevelopment() && keyInfo.hoursLeft == 0)
                                lvItem.ToolTipText = string.Format("Test\\Dev Protection Key {0} expired: 0 hours left", keyInfo.keyName);
                            else
                                lvItem.ToolTipText = string.Format("Protection Key {0} expired on: {1}", keyInfo.keyName, keyInfo.expirationDate.ToString());
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

                            // CR.FIX.16562 - expiration for creator 2 trial keys does not display, fixed issue for trial keys
                            // looking like perm keys when they are in the last day.  Use keyInfo.hoursLeft instead of days.
                            if (keyInfo.hoursLeft == 0 && keyInfo.bIsActive)
                                lvItem.SubItems.Add("Unlimited");
                            else if (keyInfo.IsKeyTypeDevelopment())
                                lvItem.SubItems.Add(daysLeft.ToString());
                            else
                                lvItem.SubItems.Add("-");

                            if (keyInfo.hoursLeft == 0 && keyInfo.bIsActive)
                                lvItem.SubItems.Add("Unlimited");
                            else
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
            //System.Diagnostics.Trace.WriteLine("TheListView_SelectedIndexChanged()");
            if (sender is ListView)
            {
                ListView lView = sender as ListView;
                switch (SelectedNode.Level)
                {
                    case 0:
                        #region if (SelectedNode.Text == AppConstants.ProtectionKeyRootNode)
                        if (SelectedNode.Text == AppConstants.ProtectionKeyRootNode)
                        {
                            if (lView.SelectedItems.Count > 0 && lView.SelectedItems[0].Tag is string)
                            {
                                Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseProtectionKeyInfo keyInfo = m_CommLink.KeyFindByName(lView.SelectedItems[0].Tag as string);
                                if (keyInfo != null && !keyInfo.IsKeyTypeUninitialized() && !keyInfo.IsKeyTypeVerification() && !m_CommLink.bDiagnosticDateView)
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
                        #endregion
                        #region if (SelectedNode.Text == AppConstants.EventLogRootNode)
                        else if (SelectedNode.Text == AppConstants.EventLogRootNode)
                        {
                            TheEventLogEntryControl.Visible = true;
                            TheBottomListView.Visible = false;
                            TheBottomListViewToolStrip.Visible = false;

                            TheSplitControl.Panel2Collapsed = false;
                            if (lView.SelectedItems.Count > 0)
                                LoadEventLogEntryData(lView.SelectedItems[0].Tag as Solimar.Licensing.Attribs.Sys_EventLogInfoAttribs.Sys_EventLogEntriesInfoAttribs);
                            else
                                LoadEventLogEntryData(null);
                        }
                        #endregion
                        else
                        {
                        }
                        break;
                    case 2:
                        #region if (SelectedNode.Parent.Parent.Text == AppConstants.LicenseRootNode)
                        if (SelectedNode.Parent.Parent.Text == AppConstants.LicenseRootNode)
                        {
                            //_prodLicNumber
                            if (lView.SelectedItems.Count > 0)
                            {
                                //Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseProtectionKeyInfo keyInfo = m_CommLink.KeyFindByName(lView.SelectedItems[0].Tag as string);
                                //if (keyInfo != null && !keyInfo.IsKeyTypeUninitialized() && !keyInfo.IsKeyTypeVerification())
                                if (true)
                                {
                                    TheSplitControl.Panel2Collapsed = false;
                                    LoadModuleData(lView.SelectedItems[0].Text);
                                }
                                //else
                                //{
                                //    TheSplitControl.Panel2Collapsed = true;
                                //}
                            }
                            else
                            {
                                LoadModuleData("");
                                //TheSplitControl.Panel2Collapsed = true;
                            }
                        }
                        #endregion
                        else
                        {
                        }
                        break;
                    default:
                        break;
                }
            }
        }

        public void LoadProtectionKeyModulesData(string keyName)
        {
            //System.Diagnostics.Trace.WriteLine("LoadProtectionKeyModulesData() keyName: " + keyName);
            try
            {
                if (m_toolStripList.ContainsKey(AppConstants.BottomLicenseRootNode))
                {
                    this.TheBottomListViewToolStrip.Visible = true;
                    this.TheBottomListViewToolStrip.Items.Clear();
                    this.TheBottomListViewToolStrip.Items.AddRange(m_toolStripList[AppConstants.BottomLicenseRootNode].ToArray());
                    this.TheBottomListViewToolStrip.Items[0].Text = string.Format("{0}: {1}", AppConstants.KeyNameHeader, keyName);
                }
                else
                {
                    this.TheBottomListViewToolStrip.Visible = false;
                }

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

                Shared.VisualComponents.ListViewHelper.ResizeListViewHeadersToMaxOfDataAndHeader(TheBottomListView);
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
                        System.Collections.Generic.Dictionary<string, bool?> usageMap = m_CommLink.GetAppInstToUsageMap_ByProduct((int)prodInfo.productID.TVal);
                        if (usageMap.Count > 0)
                        {
                            productName = m_CommLink.GetProductName((int)prodInfo.productID.TVal);
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

                System.Collections.Generic.Dictionary<string, bool?> usageMap = m_CommLink.GetAppInstToUsageMap_ByProduct(m_CommLink.GetProductID(m_TreeNode.Text));
                if (usageMap.Count > 0)
                {
                    foreach (System.Collections.Generic.KeyValuePair<string, bool?> usagePair in usageMap)
                    {
                        //string connectionType = "Unknown: ";
                        //if (usagePair.Value.HasValue)
                        //    connectionType = usagePair.Value.Value ? "Primary: " : "Failover: ";
                        string connectionType = (usagePair.Value.HasValue && (usagePair.Value.Value == false)) ? "Failover: " : "";

                        ListViewItem lvItem = new ListViewItem();
                        lvItem.Text = string.Format("{0}{1}", connectionType, usagePair.Key);
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
                #region if (m_CommLink.bDiagnosticDateView == true)
                if (m_CommLink.bDiagnosticDateView == true)
                {
                    foreach (Solimar.Licensing.Attribs.Lic_UsageInfoAttribs.Lic_UsProductInfoAttribs prodInfo in m_CommLink.diagnosticUsageAttribs.productList.TVal)
                    {
                        productID = (int)prodInfo.productID.TVal;
                        if (string.Equals(m_CommLink.GetProductName(productID), _productName))
                        {
                            foreach (Solimar.Licensing.Attribs.Lic_UsageInfoAttribs.Lic_UsAppInstanceInfoAttribs appInstInfo in prodInfo.appInstanceList.TVal)
                            {
                                if (string.Compare(appInstInfo.applicationInstance.TVal, m_TreeNode.Name, true) == 0)
                                {
                                    foreach (Solimar.Licensing.Attribs.Lic_UsageInfoAttribs.Lic_UsModuleInfoAttribs modInfo in appInstInfo.moduleList.TVal)
                                    {
                                        ListViewItem lvItem = new ListViewItem();
                                        lvItem.Text = m_CommLink.GetModuleName(productID, (int)modInfo.moduleID.TVal);
                                        lvItem.SubItems.Add(modInfo.moduleUsage.TVal.ToString());
                                        lvItem.SubItems.Add("-");
                                        //m_CommLink.ModuleLicenseTotal(moduleID, ref licenseCount);
                                        //lvItem.SubItems.Add(licenseCount.ToString());
                                        this.TheListView.Items.Add(lvItem);
                                    }
                                }
                            }
                        }
                    }
                }
                #endregion
                #region else //if (m_CommLink.bDiagnosticDateView == false)
                else //if (m_CommLink.bDiagnosticDateView == false)
                {
                    foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs prodInfo in licInfoAttrib.productList.TVal)
                    {   //found matching prodinfo attriblist
                        productID = (int)prodInfo.productID.TVal;
                        if (string.Equals(m_CommLink.GetProductName(productID), _productName))
                        {
                            m_CommLink.InitializeWrapper(m_TreeNode.Name, productID, (int)prodInfo.product_Major.TVal, (int)prodInfo.product_Minor.TVal);
                            foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs modInfo in prodInfo.moduleList.TVal)
                            {
                                moduleID = (int)modInfo.moduleID.TVal;

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
                #endregion
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
            TestConnection(lvItemList);
        }
        public void prodConnTestConnAllTSButton_Click(object sender, EventArgs e)
        {
            System.Collections.Generic.List<ListViewItem> lvItemList = new List<ListViewItem>();
            foreach (ListViewItem lvi in this.TheListView.Items)
                lvItemList.Add(lvi);
            TestConnection(lvItemList);
        }
        #endregion

        #region Event Log Section
        public void LoadEventLogData()
        {
            //EventLogMgr eventLogMgr = new EventLogMgr();
            //PopulateEventLogView(eventLogMgr);
            
            PopulateEventLogView();

            RefreshEventLogData(null, null);
        }
        //public void PopulateEventLogView(Shared.VisualComponents.IEventLog iEventLogObj)
        public void PopulateEventLogView()
        {
            //System.Diagnostics.Trace.WriteLine(string.Format("PopulateEventLogView() - Enter - MachineName: {0}", iEventLogObj.GetMachineName()));
            //System.Diagnostics.Trace.WriteLine(string.Format("{0} - PopulateEventLogView() - Enter", (new System.Diagnostics.StackTrace()).FrameCount));
            try
            {
                this.TheListViewToolStrip.Visible = true;
                this.TheListViewToolStrip.Items.Clear();
                this.TheListViewToolStrip.Items.AddRange(m_toolStripList[AppConstants.EventLogRootNode].ToArray());
                
                //this.TheListView.FindForm().Cursor = Cursors.WaitCursor;
                this.TheListView.BeginUpdate();
                this.TheListView.Items.Clear();
                this.TheListView.MultiSelect = false;

                //this.TheListView.SelectedIndexChanged -= new EventHandler(TheListView_SelectedIndexChanged);
                Shared.VisualComponents.ControlHelper.RemoveEvents(this.TheListView, "SelectedIndexChanged");
                this.TheListView.SelectedIndexChanged += new EventHandler(TheListView_SelectedIndexChanged);
                TheListView_SelectedIndexChanged(this.TheListView, null);
                //this.bottomInfoPanel.visible = true;
            }
            catch (Exception ex)
            {
                this.TheListView.NoItemsMessage = ex.Message;
            }
            finally
            {
                this.TheListView.EndUpdate();
                //this.TheListView.FindForm().Cursor = Cursors.Default;
            }
            //System.Diagnostics.Trace.WriteLine("PopulateEventLogView() - Leave");
        }

        public void RefreshEventLogData(object sender, EventArgs e)
        {
            
            ////if (!IsLocalMachine(m_CommLink.ServerName))
            //if (IsLocalMachine(m_CommLink.ServerName))
            //    RefreshEventLogData(new EventLogMgr());
            //else if (m_CommLink.bDiagnosticDateView == true)
            //{
            //    RefreshEventLogData(new EventLogMgr_LicenseSvr(m_CommLink));
            //}
            //else
            //{
            //    // For remote Event Log usage, use license server in the future...
            //    //RefreshEventLogData(new RemoteEventLogMgr(m_CommLink.ServerName));
            //    RefreshEventLogData(new EventLogMgr_LicenseSvr(m_CommLink));
            //}

            bool bResetProductComboBox = false;
            if (sender is ToolStripComboBox)
                bResetProductComboBox = (sender as ToolStripComboBox).SelectedItem is EventLog_ComboSourceItem;

            if (m_CommLink.bDiagnosticDateView == true)
            {
                //RefreshEventLogData(new EventLogMgr_LicenseSvr(m_CommLink), bResetProductComboBox);
                RefreshEventLogData(m_CommLink, bResetProductComboBox);
            }
            //else if (!IsLocalMachine(m_CommLink.ServerName))
            else if (IsLocalMachine(m_CommLink.ServerName))
            {
                RefreshEventLogData(new EventLogMgr(), bResetProductComboBox);
            }
            else
            {
                // For remote Event Log usage, use license server in the future...
                //RefreshEventLogData(new RemoteEventLogMgr(m_CommLink.ServerName));
                //RefreshEventLogData(new EventLogMgr_LicenseSvr(m_CommLink), bResetProductComboBox);
                RefreshEventLogData(m_CommLink, bResetProductComboBox);
            }
        }

        /// <summary>Refreshes the List of eventlog messages on a given PC.</summary>
        public void RefreshEventLogData(Shared.VisualComponents.IEventLog _iEventLogObj, bool _bResetProductComboBox)
        {
            //System.Diagnostics.Trace.WriteLine(string.Format("RefreshEventLogData() - Enter - MachineName: {0}, Type: {1}", _iEventLogObj.GetMachineName(), _iEventLogObj.GetType().ToString()));
            try
            {
                UseWaitCursor(true);
                //System.IO.FileSystemWatcher
                //Track Previously selected item
                int? selEventIdx = null;
                if (this.TheListView.SelectedItems.Count > 0 && this.TheListView.SelectedItems[0].Tag is Solimar.Licensing.Attribs.Sys_EventLogInfoAttribs.Sys_EventLogEntriesInfoAttribs)
                    selEventIdx = (int)(this.TheListView.SelectedItems[0].Tag as Solimar.Licensing.Attribs.Sys_EventLogInfoAttribs.Sys_EventLogEntriesInfoAttribs).index.TVal;
                ListViewItem newSelLvi = null;

                this.TheListView.BeginUpdate();
                this.TheListView.Items.Clear();
                System.Collections.Generic.List<ListViewItem> lviList = new List<ListViewItem>();
                //System.Diagnostics.Trace.WriteLine("RefreshEventLogData() - foreach() - Start");
                //SortedList<string, Solimar.Licensing.Common.Collections.Tuple<ToolStripItem, int, string>> eventTypeFilterHash = // Dictionary of EventLogType Filters
                //    new SortedList<
                //        string,  // EventLog Type
                //        Solimar.Licensing.Common.Collections.Tuple<
                //            ToolStripItem,  // ToolStrip
                //            int,            // Count Seen
                //            string>>();     // Text
                EventTypeToEventCounterList eventTypeFilterHash = new EventTypeToEventCounterList();

                ToolStripComboBox toolStripSourceComboBox = null;
                ToolStripComboBox toolStripSolEventProductComboBox = null;
                List<ToolStripItem> optionalyShownControl = new List<ToolStripItem>();
                
                EventLog_ComboSourceItem filterItem = null;
                EventLog_ComboSolLicSvrProductItem solEventFilterItem = null;
                string filterSource = string.Empty;
                #region foreach (ToolStripItem tsItem in this.TheListViewToolStrip.Items)
                foreach (ToolStripItem tsItem in this.TheListViewToolStrip.Items)
                {
                    if (tsItem is ToolStripButton)
                    {
                        tsItem.Text = ((DisplayNameAndEventType)tsItem.Tag).Item1;
                        if((tsItem as ToolStripButton).Checked == true)
                            eventTypeFilterHash.Add(((DisplayNameAndEventType)tsItem.Tag).Item2.ToString(), new EventCounter(tsItem, 0, ((Solimar.Licensing.Common.Collections.Tuple<string, System.Diagnostics.EventLogEntryType>)tsItem.Tag).Item1));
                    }
                    else if (tsItem is ToolStripComboBox && filterItem == null)
                    {
                        toolStripSourceComboBox = tsItem as ToolStripComboBox;
                        filterItem = toolStripSourceComboBox.SelectedItem as EventLog_ComboSourceItem;
                        filterSource = filterItem != null ? filterItem.source : string.Empty;
                        toolStripSourceComboBox.Items.Clear();
                    }
                    else if (tsItem.Tag == typeof(bool))
                    {
                        tsItem.Visible = false;
                        optionalyShownControl.Add(tsItem);
                    }
                }

                //See if Solimar License Server is selected...
                if (string.Compare(filterSource, AppConstants.LicenseServer, true) == 0)
                {
                    foreach (ToolStripItem tsItem in optionalyShownControl)
                    {
                        tsItem.Visible = true;
                        if (tsItem is ToolStripComboBox && solEventFilterItem == null)
                        {
                            toolStripSolEventProductComboBox = (tsItem as ToolStripComboBox);
                            if (_bResetProductComboBox && toolStripSolEventProductComboBox.Items.Count > 0)
                                solEventFilterItem = toolStripSolEventProductComboBox.Items[0] as EventLog_ComboSolLicSvrProductItem;
                            else
                                solEventFilterItem = toolStripSolEventProductComboBox.SelectedItem as EventLog_ComboSolLicSvrProductItem;
                        }
                        
                    }
                }
                #endregion

                //int idx = 0;
                System.Collections.Generic.SortedList<string, int> sourceList = new SortedList<string, int>();
                sourceList.Add(string.Empty, 0);

                System.Collections.Generic.SortedList<long/*ProdID*/, int/*CountSeen*/> productList = new SortedList<long, int>();
                productList.Add(-1, 0);
                //

                #region foreach (System.Diagnostics.EventLogEntry eventLogEntry in iEventLogObj.GetAllEntries())
                IList<Solimar.Licensing.Attribs.Sys_EventLogInfoAttribs.Sys_EventLogEntriesInfoAttribs> eventLogEntryList = _iEventLogObj.GetAllEntries();
                if(eventLogEntryList!=null)
                {
                    foreach (Solimar.Licensing.Attribs.Sys_EventLogInfoAttribs.Sys_EventLogEntriesInfoAttribs eventLogEntry in eventLogEntryList)
                    {
                        string key = ((System.Diagnostics.EventLogEntryType)eventLogEntry.entryType.TVal).ToString();
                        bool pastAllFilter = false;
                        bool pastSourceFilter = true;
                        if (filterSource != string.Empty)
                            pastSourceFilter = (string.Compare(filterSource, eventLogEntry.source.TVal, true) == 0);

                        pastAllFilter = pastSourceFilter;
                        if (pastAllFilter && solEventFilterItem != null)
                            pastAllFilter = solEventFilterItem.Match(eventLogEntry.eventID);

                        if (!sourceList.ContainsKey(eventLogEntry.source.TVal))
                            sourceList.Add(eventLogEntry.source.TVal, 0);
                        if (eventTypeFilterHash.ContainsKey(key) == true)
                            sourceList[eventLogEntry.source.TVal]++;

                        long productId = EventLog_ComboSolLicSvrProductItem.GetProductID((long)eventLogEntry.eventID.TVal);
                        if (pastSourceFilter && !productList.ContainsKey(productId))
                            productList.Add(productId, 0);
                        if (pastSourceFilter && eventTypeFilterHash.ContainsKey(key) == true)
                            productList[productId]++;

                        if (eventTypeFilterHash.ContainsKey(key) == true && pastAllFilter == true)
                        {
                            ListViewItem lvi = new ListViewItem();
                            lvi.Text = key;
                            lvi.Tag = eventLogEntry;
                            lvi.SubItems.Add(eventLogEntry.timeGenerated.TVal.ToString());
                            lvi.SubItems.Add(eventLogEntry.source.TVal);
                            lvi.SubItems.Add(eventLogEntry.instanceId.TVal.ToString());
                            lvi.SubItems.Add(eventLogEntry.categoryNumber.TVal == 0 ? "None" : eventLogEntry.category.TVal);
                            lviList.Add(lvi);
                            //if(!sourceList.ContainsKey(eventLogEntry.Source))
                            //    sourceList.Add(eventLogEntry.Source, 0);
                            //sourceList[eventLogEntry.Source]++;

                            if (newSelLvi == null && selEventIdx.HasValue && selEventIdx == eventLogEntry.index.TVal)
                                newSelLvi = lvi;

                            //if (++idx > 100)    //For testing use top 500
                            //    break;
                            eventTypeFilterHash[key].Item2++;
                        }
                    }
                }
                #endregion
                //System.Diagnostics.Trace.WriteLine("RefreshEventLogData() - foreach() - End");

                foreach (EventTypeToEventCounterPair kvPair in eventTypeFilterHash)
                {
                    if (kvPair.Value.Item1 is ToolStripButton)
                        (kvPair.Value.Item1 as ToolStripButton).Text = string.Format("{0} {1}", kvPair.Value.Item2, kvPair.Value.Item3);
                }

                #region if (toolStripSourceComboBox != null)
                if (toolStripSourceComboBox != null)
                {
                    EventLog_ComboSourceItem selectedItem = null;
                    List<EventLog_ComboSourceItem> tmpList = new List<EventLog_ComboSourceItem>();
                    foreach (KeyValuePair<string, int> kvPair in sourceList)
                    {
                        EventLog_ComboSourceItem entry = new EventLog_ComboSourceItem() { source = kvPair.Key, itemCount = kvPair.Value };
                        tmpList.Add(entry);
                        if (selectedItem == null && filterItem.CompareTo(entry) == 0)
                            selectedItem = entry;
                    }
                    toolStripSourceComboBox.Items.AddRange(tmpList.ToArray());
                    
                    toolStripSourceComboBox.SelectedIndexChanged -= new EventHandler(RefreshEventLogData);
                    toolStripSourceComboBox.SelectedItem = selectedItem;
                    toolStripSourceComboBox.SelectedIndexChanged += new EventHandler(RefreshEventLogData);
                }
                #endregion

                #region if (toolStripSolEventProductComboBox != null)
                if (toolStripSolEventProductComboBox != null)
                {
                    EventLog_ComboSolLicSvrProductItem selectedItem = null;
                    List<EventLog_ComboSolLicSvrProductItem> tmpList = new List<EventLog_ComboSolLicSvrProductItem>();
                    foreach (KeyValuePair<long, int> kvPair in productList)
                    {
                        EventLog_ComboSolLicSvrProductItem entry = new EventLog_ComboSolLicSvrProductItem(kvPair.Key) { itemCount = kvPair.Value};
                        entry.source = (kvPair.Key == 0) ? AppConstants.LicenseServerNoProduct : m_CommLink.GetProductName((int)kvPair.Key);

                        tmpList.Add(entry);
                        if (selectedItem == null && solEventFilterItem.CompareTo(entry) == 0)
                            selectedItem = entry;
                    }
                    toolStripSolEventProductComboBox.Items.Clear();
                    toolStripSolEventProductComboBox.Items.AddRange(tmpList.ToArray());

                    toolStripSolEventProductComboBox.SelectedIndexChanged -= new EventHandler(RefreshEventLogData);
                    toolStripSolEventProductComboBox.SelectedItem = selectedItem;
                    toolStripSolEventProductComboBox.SelectedIndexChanged += new EventHandler(RefreshEventLogData);
                }
                #endregion

                //System.Diagnostics.Trace.WriteLine("RefreshEventLogData() - AddRange() - Start");
                this.TheListView.Items.AddRange(lviList.ToArray());
                if (newSelLvi == null && this.TheListView.Items.Count > 0)
                    newSelLvi = this.TheListView.Items[0];

                if (newSelLvi != null)
                {
                    newSelLvi.Selected = true;
                    newSelLvi.EnsureVisible();
                }
                else
                    LoadEventLogEntryData(null);

                //System.Diagnostics.Trace.WriteLine("RefreshEventLogData() - AddRange() - End");
            }
            catch (Exception ex)
            {
                this.TheListView.NoItemsMessage = ex.Message;

                COMException comEx = ex as COMException;
                if (comEx != null)
                {
                    if (((uint)comEx.ErrorCode) == 0x80004001)
                    {
                        this.TheListView.NoItemsMessage = string.Format("Upgrade License Server for this functionality - {0}", comEx.Message);
                        this.TheListView.Columns.Clear();
                        TheSplitControl.Panel2Collapsed = true;
                        this.TheListViewToolStrip.Items.Clear();
                        this.TheListViewToolStrip.Items.Add(AppConstants.EventLogRootNode);
                    }
                }
            }
            finally
            {
                this.TheListView.EndUpdate();
                UseWaitCursor(false);
            }
            //System.Diagnostics.Trace.WriteLine("RefreshEventLogData() - Leave");
        }

        public void LoadEventLogEntryData(Solimar.Licensing.Attribs.Sys_EventLogInfoAttribs.Sys_EventLogEntriesInfoAttribs _eventLogEntry)
        {
            //System.Diagnostics.Trace.WriteLine("LoadEventLogEntryData() - Enter");
            this.TheEventLogEntryControl.SetData(_eventLogEntry);
            //System.Diagnostics.Trace.WriteLine("LoadEventLogEntryData() - Leave");
        }

        #endregion

        #region Email Alert Section
        private const string DIAGNOSTIC_DATA_ERROR = "Unable to perform this action with diagnostic data.";
        public void LoadEmailAlertData()
        {
            try
            {
                PopulateEmailAlertView();

                int major = 0;
                int minor = 0;
                int build = 0;
                this.m_CommLink.GetVersionLicenseServer(this.m_CommLink.ServerName, ref major, ref minor, ref build);
                if (major < 3 || (major == 3 && minor < 2))
                {
                    Exception ex = new Exception(string.Format("License Server '{0}' must be at least version 3.2 for this feature.  The server is currenly version {1}.{2}", this.m_CommLink.ServerName, major, minor));
                    HandleExceptions.DisplayException(ex);
                    this.TheListView.NoItemsMessage = ex.Message;
                    throw ex;

                }
                else
                {
                    RefreshEmailAlertData(null, null);
                    emailAlert_SelectedIndexChanged(this.TheListView, new EventArgs());
                }
            }
            catch (Exception)
            {
                foreach (ToolStripItem tsItem in this.TheListViewToolStrip.Items)
                {
                    if (tsItem is ToolStripButton)
                        (tsItem as ToolStripButton).Enabled = false;
                }
            }
        }
        public void PopulateEmailAlertView()
        {
            try
            {
                this.TheListViewToolStrip.Visible = true;
                this.TheListViewToolStrip.Items.Clear();
                this.TheListViewToolStrip.Items.AddRange(m_toolStripList[AppConstants.EmailAlertNode].ToArray());

                //this.TheListView.FindForm().Cursor = Cursors.WaitCursor;
                this.TheListView.BeginUpdate();
                this.TheListView.Items.Clear();
                this.TheListView.MultiSelect = false;

                this.TheListView.SelectedIndexChanged += new EventHandler(emailAlert_SelectedIndexChanged);
                this.TheListView.KeyDown += new KeyEventHandler(emailAlert_KeyDown);
                this.TheListView.DoubleClick += new EventHandler(emailAlert_EditAlert);

                TheListView_SelectedIndexChanged(this.TheListView, null);
                //this.bottomInfoPanel.visible = true;
            }
            catch (Exception ex)
            {
                this.TheListView.NoItemsMessage = ex.Message;
            }
            finally
            {
                this.TheListView.EndUpdate();
                //this.TheListView.FindForm().Cursor = Cursors.Default;
            }
        }
        public void RefreshEmailAlertData(object sender, EventArgs e)
        {
            //if (m_CommLink.bDiagnosticDateView == true)
            //{
            //}
            String generalStream = "";
            Solimar.Licensing.Attribs.Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_EmailAlertMailAttribsList emailAlertAttribsList = new Solimar.Licensing.Attribs.Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_EmailAlertMailAttribsList("emLt", new ArrayList());
            try
            {
                this.TheListView.BeginUpdate();
                System.Collections.Generic.List<ListViewItem> lviList = new List<ListViewItem>();

                // Track previous selected id
                SortedDictionary<string, bool> selIdTable = new SortedDictionary<string, bool>();
                foreach (ListViewItem selLvi in this.TheListView.SelectedItems)
                    selIdTable.Add(selLvi.Tag as string, true);

                // This code might occur at the CommunicationLink level instead, think about

                m_CommLink.GetAllEmailAlerts(ref generalStream);
                emailAlertAttribsList.SVal = generalStream;

                // For initial testing, just add item, don't update yet
                foreach (Solimar.Licensing.Attribs.Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_EmailAlertMailAttribs emailAlertAttribs in emailAlertAttribsList.TVal)
                {
                    ListViewItem lvItem = new ListViewItem();
                    lvItem.Name = emailAlertAttribs.id.TVal;
                    lvItem.Text = emailAlertAttribs.name.TVal;
                    lvItem.Tag = emailAlertAttribs.id.TVal;
                    lvItem.Selected = selIdTable.ContainsKey(emailAlertAttribs.id.TVal);
                    lvItem.SubItems.Add(string.Format("{0}", emailAlertAttribs.bActive.TVal));

                    StringBuilder sBuilder = new StringBuilder();
                    foreach (string recipentId in emailAlertAttribs.recipentsList.TVal)
                        sBuilder.AppendFormat("{0}{1}", (sBuilder.Length > 0) ? ", " : "", recipentId);
                    lvItem.SubItems.Add(sBuilder.ToString());

                    sBuilder = new StringBuilder();
                    foreach (UInt32 eventId in emailAlertAttribs.eventIdList.TVal)
                        sBuilder.AppendFormat("{0}{1}", (sBuilder.Length > 0) ? ", " : "", eventId);
                    lvItem.SubItems.Add(sBuilder.ToString());

                    lviList.Add(lvItem);
                }

                this.TheListView.Items.Clear();
                this.TheListView.Items.AddRange(lviList.ToArray());
                Shared.VisualComponents.ListViewHelper.ResizeListViewHeadersToMaxOfDataAndHeader(TheListView);
            }
            catch (Exception ex)
            {
                this.TheListView.NoItemsMessage = ex.Message;
            }
            finally
            {
                this.TheListView.EndUpdate();
            }
        }

        private void ValidateEmailAlertName(string _streamedEmailAlertMailAttribs)
        {
            Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_EmailAlertMailAttribs tmpEmailAlertMailAttribs = new Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_EmailAlertMailAttribs();
            tmpEmailAlertMailAttribs.AssignMembersFromStream(_streamedEmailAlertMailAttribs);
            foreach (ListViewItem lvi in this.TheListView.Items)
            {
                if (string.Equals(lvi.Text, tmpEmailAlertMailAttribs.name.TVal, StringComparison.OrdinalIgnoreCase) &&
                    !string.Equals(lvi.Tag as string, tmpEmailAlertMailAttribs.id, StringComparison.OrdinalIgnoreCase))
                {
                    throw new Exception(string.Format("Name: '{0}' is already used, choose a different name", tmpEmailAlertMailAttribs.name.TVal));
                }
            }
        }

        private List<int> GetLicensedProductIdList()
        {
            string generalStream = string.Empty;
            m_CommLink.GetSoftwareLicenseInfoForAll(ref generalStream);
            Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs licInfoAttrib = new Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs();
            licInfoAttrib.AssignMembersFromStream(generalStream);

            List<int> licenseIdList = new List<int>();
            licenseIdList.Add(-1);
            foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs prodInfo in licInfoAttrib.productList.TVal)
                licenseIdList.Add((int)prodInfo.productID.TVal);

            return licenseIdList;
        }
        private List<int> GetAllProductIdList()
        {
            List<int> allIdList = new List<int>();  // Look at software spec to find ALL the products...
            allIdList.Add(-1);
            foreach (UInt32 productID in m_CommLink.SoftwareSpec.productSpecMap.TVal.Keys)
                allIdList.Add((int)productID);
            return allIdList;
        }
        private string GetProductName(int _productID)
        {
            return (_productID == -1) ? "Solimar License Server" : m_CommLink.GetProductName(_productID);
        }

        private void emailAlert_NewAlert(object sender, EventArgs e)
        {
            using (EditEmailAlertDialog dialog = new EditEmailAlertDialog(
                GetProductName,
                ValidateEmailAlertName,
                this.m_imageList))
            {
                try
                {
                    if (m_CommLink.bDiagnosticDateView)
                        throw new Exception(DIAGNOSTIC_DATA_ERROR);

                    if (ensureMailServerIsConfigured())
                    {
                        Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_EmailAlertMailAttribs emailAlertAttribs = new Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_EmailAlertMailAttribs();

                        SolimarLicenseViewer.EditEmailAlertDialog.EditEmailAlertDialogData data = new EditEmailAlertDialog.EditEmailAlertDialogData(
                            emailAlertAttribs,
                            //GetAllProductIdList(),
                            GetLicensedProductIdList().ToArray()
                            );

                        dialog.SetData(data, this.m_CommLink.bDiagnosticDateView);

                        if (dialog.ShowDialog() == DialogResult.OK)
                        {
                            Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_EmailAlertMailAttribs emailData = dialog.GetData();
                            string emailAlertId = string.Empty;
                            m_CommLink.AddEmailAlert(emailData.ToString(), ref emailAlertId);

                            RefreshEmailAlertData(null, null);
                        }
                    }
                }
                catch (Exception ex)
                {
                    HandleExceptions.DisplayException(ex);
                }
            }

        }
        private void emailAlert_EditAlert(object sender, EventArgs e)
        {
            if (this.TheListView.SelectedItems.Count > 0)
            {
                try
                {
                    if (ensureMailServerIsConfigured())
                    {
                        ListViewItem selLvi = this.TheListView.SelectedItems[0];
                        string emailAlertId = selLvi.Tag as string;

                        string streamedAttribs = string.Empty;
                        m_CommLink.GetEmailAlert(emailAlertId, ref streamedAttribs);

                        Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_EmailAlertMailAttribs emailAlertAttribs = new Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_EmailAlertMailAttribs();
                        emailAlertAttribs.AssignMembersFromStream(streamedAttribs);

                        using (EditEmailAlertDialog dialog = new EditEmailAlertDialog(
                            GetProductName,
                            ValidateEmailAlertName,
                            this.m_imageList))
                        {
                            SolimarLicenseViewer.EditEmailAlertDialog.EditEmailAlertDialogData data = new EditEmailAlertDialog.EditEmailAlertDialogData(
                                emailAlertAttribs,
                                //GetAllProductIdList(),
                                GetLicensedProductIdList().ToArray()
                                );

                            dialog.SetData(data, this.m_CommLink.bDiagnosticDateView);

                            if (dialog.ShowDialog() == DialogResult.OK)
                            {
                                if (m_CommLink.bDiagnosticDateView == false)
                                {
                                    Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_EmailAlertMailAttribs emailData = dialog.GetData();
                                    m_CommLink.SetEmailAlert(emailData.id.TVal, emailData.ToString());

                                    RefreshEmailAlertData(null, null);
                                }
                            }
                        }
                    }
                }
                catch (Exception ex)
                {
                    HandleExceptions.DisplayException(ex);
                }
            }
        }
        private void emailAlert_DeleteAlert(object sender, EventArgs e)
        {
            if (this.TheListView.SelectedItems.Count > 0)
            {
                try
                {
                    if (m_CommLink.bDiagnosticDateView)
                        throw new Exception(DIAGNOSTIC_DATA_ERROR);

                    ListViewItem selLvi = this.TheListView.SelectedItems[0];
                    string deleteQuestion = string.Format("Are you sure you want to delete the E-mail Alert \'{0}\'?", selLvi.Text);
                    if (MessageBox.Show(deleteQuestion, "Confirm Delete", MessageBoxButtons.YesNo, MessageBoxIcon.Question, MessageBoxDefaultButton.Button2) == DialogResult.Yes)
                    {
                        m_CommLink.DeleteEmailAlert(selLvi.Tag as string);
                        RefreshEmailAlertData(null, null);
                    }
                }
                catch (Exception ex)
                {
                    HandleExceptions.DisplayException(ex);
                }
            }
        }
        private void ConfigureMailServer(object obj)
        {
            emailAlert_EditMailServer(null, null);
        }
        private void TestMailServer(string _testMailAttribsStreamed)
        {
            m_CommLink.TestMailServerInfo(_testMailAttribsStreamed);
        }
        private List<string> previousRecipientList = new List<string>();
        private void emailAlert_TestMailServer(object sender, EventArgs e)
        {
            using (TestMailServerDialog dialog = new TestMailServerDialog(TestMailServer, ConfigureMailServer))
            {
                try
                {
                    if (m_CommLink.bDiagnosticDateView)
                        throw new Exception(DIAGNOSTIC_DATA_ERROR);

                    if (ensureMailServerIsConfigured())
                    {
                        string mailServerInfoStream = string.Empty;
                        m_CommLink.GetMailServerInfo(ref mailServerInfoStream);

                        Solimar.Licensing.Attribs.Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_AlertMailServerAttribs mailServerAttribs = new Solimar.Licensing.Attribs.Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_AlertMailServerAttribs();
                        mailServerAttribs.AssignMembersFromStream(mailServerInfoStream);
                        dialog.SetData(this.previousRecipientList);
                        if (dialog.ShowDialog() == DialogResult.OK)
                        {
                            this.previousRecipientList = (List<string>)dialog.GetData();
                        }
                    }
                }
                catch (Exception ex)
                {
                    HandleExceptions.DisplayException(ex);
                }
            }
        }

        public bool ensureMailServerIsConfigured()
        {
            return ensureMailServerIsConfigured(false);
        }
        private bool ensureMailServerIsConfigured(bool bBypassEmptyMailServerCheck)
        {
            DialogResult dialogResult = DialogResult.OK;

            using (EditMailServerDialog dialog = new EditMailServerDialog())
            {
                try
                {
                    string mailServerInfoStream = string.Empty;
                    m_CommLink.GetMailServerInfo(ref mailServerInfoStream);

                    Solimar.Licensing.Attribs.Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_AlertMailServerAttribs mailServerAttribs = new Solimar.Licensing.Attribs.Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_AlertMailServerAttribs();
                    mailServerAttribs.AssignMembersFromStream(mailServerInfoStream);

                    bool bEditMailServer = (mailServerAttribs.mailServerName.TVal.Length == 0) || bBypassEmptyMailServerCheck;

                    if (mailServerAttribs.mailServerName.TVal.Length == 0)
                        MessageBox.Show("The Mail Server must be configured before continuing.");

                    if (bEditMailServer)
                    {
                        dialog.SetData(mailServerAttribs, m_CommLink.bDiagnosticDateView);
                        dialogResult = dialog.ShowDialog();
                        if (dialogResult == DialogResult.OK)
                        {
                            if (m_CommLink.bDiagnosticDateView == false)
                            {
                                mailServerAttribs = dialog.GetData();
                                m_CommLink.SetMailServerInfo(mailServerAttribs.ToString());
                            }
                        }
                    }
                }
                catch (Exception ex)
                {
                    HandleExceptions.DisplayException(ex);
                }
            }
            return (dialogResult == DialogResult.OK);
        }

        private void emailAlert_EditMailServer(object sender, EventArgs e)
        {
            ensureMailServerIsConfigured(true);
        }
        private void emailAlert_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Insert)
                emailAlert_NewAlert(this.TheListView, new EventArgs());
            else if (e.KeyCode == Keys.Enter)
                emailAlert_EditAlert(this.TheListView, new EventArgs());
            else if (e.KeyCode == Keys.Delete)
                emailAlert_DeleteAlert(this.TheListView, new EventArgs());
            else
                e.Handled = false;
        }
        private void emailAlert_SelectedIndexChanged(object sender, EventArgs e)
        {
            bool bSelected = this.TheListView.SelectedItems.Count > 0;
            foreach (ToolStripItem tsItem in this.TheListViewToolStrip.Items)
            {
                if (tsItem is ToolStripButton)
                {
                    if (
                        string.Equals(tsItem.Text, AppConstants.EmailAlertEditSB, StringComparison.InvariantCultureIgnoreCase) ||
                        string.Equals(tsItem.Text, AppConstants.EmailAlertDeleteTSB, StringComparison.InvariantCultureIgnoreCase))
                    {
                        tsItem.Enabled = bSelected;
                    }
                }
            }
        }

        #endregion

        #region ListViewColumn Methods
        /// <summary>
        /// Force the ListView to be sorted by a given column and sortOrder
        /// </summary>
        public void SetListViewColumnSorter(ListView listView, int columnIndex, SortOrder sortOrder)
        {
            ListViewColumnSorter lvSorter = null;
            if (listView.ListViewItemSorter is ListViewColumnSorter)
            {
                lvSorter = (listView.ListViewItemSorter as ListViewColumnSorter);
                SetSortIcons(listView, lvSorter.SortColumn, columnIndex);
                lvSorter.SortColumn = columnIndex;
                lvSorter.Order = sortOrder;
            }
            if (columnIndex <= listView.Columns.Count - 1)
                lvSorter.SortType = (System.Type)listView.Columns[columnIndex].Tag;
            SetSortIcons(listView, lvSorter.SortColumn, columnIndex);
            SetSelectedColumnColor(listView, columnIndex);
        }
        /// <summary>
        /// Initializes the ListViewColumnSorter with options based upon currently selected column. 
        /// <param name="columnIndex"> index of the column to sort </param>
        /// </summary>
        public void SetListViewColumnSorter(ListView listView, int columnIndex)
        {
            // Determine if clicked column is already the column that is being sorted.
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

        //public ListViewItem FindListViewItem(ListView _lView, string _text)
        public ListViewItem FindListViewItem(ICollection _lviList, string _text)
        {
            ListViewItem foundItem = null;
            if (_lviList != null && _text != string.Empty)
            {
                foreach (ListViewItem lvi in _lviList)
                {
                    if (string.Compare(lvi.Text, _text, true) == 0)
                    {
                        foundItem = lvi;
                        break;
                    }
                }
            }
            return foundItem;
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

        public Boolean EnableDisasterRecoverExt_ByLicense(String _licenseName)
        {
            Boolean bEnableDisaster = false;
            if (enableActivationByLicense.ContainsKey(_licenseName))
                bEnableDisaster = enableActivationByLicense[_licenseName];
            return bEnableDisaster;
        }
        private System.Collections.Generic.Dictionary<String, bool> enableActivationByLicense = new System.Collections.Generic.Dictionary<String, bool>();

        public Boolean EnableActivationExt_ByProductLicense(String _productLicenseName)
        {
            Boolean bEnableDisaster = false;
            if (enableActivationExtByProductLicense.ContainsKey(_productLicenseName))
                bEnableDisaster = enableActivationExtByProductLicense[_productLicenseName];
            return bEnableDisaster;
        }
        private System.Collections.Generic.Dictionary<String, bool> enableActivationExtByProductLicense = new System.Collections.Generic.Dictionary<String, bool>();

        #endregion

        #region Private Variables
        /// <summary>
        /// Specifies the ListView from the MainForm
        /// </summary>
        private Shared.VisualComponents.NoFlickerListView TheListView;
        private Shared.VisualComponents.EventLogEntryControl TheEventLogEntryControl;
        private SplitContainer TheSplitControl;
        private ToolStrip TheListViewToolStrip;
        private ToolStrip TheBottomListViewToolStrip;
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
        //private Boolean m_DisasterRecoveryMode;
        /// <summary>
        /// Boolean object to determine disaster recovery extensions enabled
        /// </summary>
        //private Boolean m_EnableDisasterRecoverExt;

        private ImageList m_imageList;

        private Dictionary<string, List<ToolStripItem>> m_toolStripList;
        private ToolStripComboBox m_moduleFilterComboBox;
        //private ToolStripButton m_moduleGroupByTSButton;
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
                            DateTime d1;
                            DateTime d2;

                            bool bD1IsDate = DateTime.TryParse(itemX, out d1);
                            bool bD2IsDate = DateTime.TryParse(itemY, out d2);

                            if (bD1IsDate && bD2IsDate)
                                compareResult = DateTime.Compare(d1, d2);
                            else if (!bD1IsDate && !bD2IsDate)
                                compareResult = String.Compare(itemX, itemY);
                            else if (!bD1IsDate && bD2IsDate)
                                compareResult = -1;
                            else if (bD1IsDate && !bD2IsDate)
                                compareResult = 1;

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

    public class ProductLicenseNumber : System.IComparable
    {
        public ProductLicenseNumber(string _prodLicNumber)
        {
            m_prodLicNumber = _prodLicNumber;
        }

        public int CompareTo(Object _obj)
        {
            int resultValue = -1;
            if (_obj is ProductLicenseNumber)
                resultValue = Solimar.Licensing.Attribs.Lic_LicenseInfoAttribsHelper.CompareProductLicenseName(m_prodLicNumber, (_obj as ProductLicenseNumber).ProdLicNum);
            return resultValue;
        }
        private string m_prodLicNumber;
        public string ProdLicNum { get { return m_prodLicNumber; } }
    }

    public class EventLog_ComboSourceItem : System.IComparable
    {
        public string source = string.Empty;
        public int itemCount = 0;

        public int CompareTo(Object _obj) { return CompareTo(_obj as EventLog_ComboSourceItem); }
        public int CompareTo(EventLog_ComboSourceItem _srcItem) { return string.Compare(this.source, _srcItem.source, true); }
        public override string ToString() 
        {
            return (this.source == string.Empty)
                ? " <All>"
                : string.Format("{0} ({1})", this.source, this.itemCount);
        }
    }

    public class EventLog_ComboSolLicSvrProductItem : System.IComparable
    {
        public EventLog_ComboSolLicSvrProductItem() {}
        public EventLog_ComboSolLicSvrProductItem(long _productID)
        {
            productID = _productID;
            //Software
            //Look in Software Spec for Product Name
            if (productID != -1)
            {
                //source = ;
            }
        }
        static public long GetProductID(long _eventID)
        {
            return (_eventID / 1000);
        }
        public bool Match(long _eventID)
        {
            bool bMatch = false;
            if (productID == -1)
                bMatch = true;
            else
                bMatch = GetProductID(_eventID) == productID;
            return bMatch;
        }
        public long productID = -1;
        public string source = string.Empty;
        public int itemCount = 0;

        public int CompareTo(Object _obj) { return CompareTo(_obj as EventLog_ComboSolLicSvrProductItem); }
        public int CompareTo(EventLog_ComboSolLicSvrProductItem _srcItem) { return (int)(this.productID - _srcItem.productID); }
        public override string ToString()
        {
            return (string.IsNullOrEmpty(this.source))
                ? " <All>"
                : string.Format("{0} ({1})", this.source, this.itemCount);
        }
    }
    //public class ProductLicenseNumberComparer : IComparer, IComparer<string>
    //{
    //    public ProductLicenseNumberComparer() { }
    //    public int Compare(object _x, object _y)
    //    {
    //        return Compare((string)_x, (string)_y);
    //    }
    //    public int StringCompare(string _x, string _y)
    //    {
    //        int result = 0;
    //        return result;
    //    }
    //}
}
