using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.IO;
using System.Windows.Forms;
using Client.Creator.CreatorService;
using Solimar.Licensing.Attribs;
using System.Runtime.InteropServices;
using System.Security.Principal;
using System.Threading;
using System.Windows.Forms.VisualStyles;
using System.ServiceModel;
using Client.Creator.ServiceProxy;
using Client.Creator.Properties;

/*
 * TODO : - Apply rules to license types - std,bkup,dr,testdev
 *        - disallow adding of duplicate products -> DONE
 *        - Create packet from license info and store ValidationProperty, date into packet table -> DONE
 *        - Create custom filters to retrieve lists.
 *        - Add license comment attribute to license table
 *        - Improve look of interface
 *        - setup service as a windows service
 *        - setup concurrency
 *        - setup remote access to service for client
 *        - figure out way to display tree with children without taking hit of load time
 */
namespace Client.Creator
{
    public partial class CreatorForm : Form
    {
        //private Dictionary<string, bool> m_TreeState;
        /// <summary>
        /// ListViewColumn sort object
        /// </summary>
        /// 
        private ListViewMgr _lvManager;
        private ListView storedPacketListView;
        public static CommunicationLink s_CommLink;
        private PermissionsTable m_Permissions;
        private string m_selectedDirectory;
        private List<String> m_ServerList;
        private string m_searchString = "";
        private string m_CurrentLicenseName = "";

        #region Enums

        enum PacketListViewState
        {
            ALL,
            VERIFIED,
            UNVERIFIED
        }

        enum TransactionType
        {
            License,
            Order,
            Product
        }

        enum DetailTreeViewIconList
        {
            ICONVERT,
            RUBIKA,
            RUBIKAPROCESSBUILDER,
            SDXDESIGNER,
            SOLINDEXER,
            SOLSEARCHERENTERPRISESINGLEPLATEFORM,
            SOLSEARCHERENTERPRISE,
            SOLFUSION,
            SOLSCRIPT,
            SPDEQUEUEMANAGER,
            SPDESYSTEMMANAGER,
            SPDE,
            SPD,
            CUSTOMER,
            LICENSE,
            LICENSEUPDATE,
            LICENSEDISABLED,
            LICENSERESYNC,
            ORDER,
            ADDONORDER,
            EMPTY
        }

        enum ListViewIconList
        {
            VERIFIED,
            UNVERIFIED,
            LISTBULLETS,
            TRANSACTION_LOGFILE,
            ZIPPEDFILE,
            TTCOMPUTERNAME,
            TTHARDWAREKEYID,
            TTMACADDRESS,
            TTBIOSSERIALNUMBER,
            TTLICENSECODE,
            MODULE,
            VALIDATIONTOKENS
        }

        private int GetDetailTreeViewIconIndex(String key)
        {
            DetailTreeViewIconList iconIndex;
            try
            {
                iconIndex = (DetailTreeViewIconList)Enum.Parse(typeof(DetailTreeViewIconList), key.Replace(" ", ""), true);
            }
            catch (Exception ex)
            {
                return -1;
            }
            return (int)iconIndex;
        }

        private int GetListViewIconIndex(String key)
        {
            ListViewIconList iconIndex;
            try
            {
                iconIndex = (ListViewIconList)Enum.Parse(typeof(ListViewIconList), key.Replace(" ", ""), true);
            }
            catch (Exception ex)
            {
                return -1;
            }
            return (int)iconIndex;
        }

        #endregion

        #region Properties
        public string CurrentServer
        {
            get { return m_ServerList.First(); }           
        }

        #endregion

        public CreatorForm()
        {
            InitializeComponent();
            m_ServerList = new List<String>();
            s_CommLink = new CommunicationLink();
            storedPacketListView = new ListView();
            _lvManager = new ListViewMgr();
            _lvManager.SetListViewColumnSorter(DetailListView);
            _lvManager.SetListViewColumnSorter(TransactionListView);
            _lvManager.SetListViewColumnSorter(PacketListView);
            _lvManager.SetListViewColumnSorter(LicensePacketListView);
            ResetMainToolStripMenu();
            //set to make read-only items not greyed-out            
            DetailPropertyGrid.ViewForeColor = Color.FromArgb(254, 0, 0, 0);
        }

        #region Enable/Disable ContextMenu

        private void ResetContextMenu()
        {
            foreach (ToolStripItem tsm in LicenseContextMenuStrip.Items)
            {
                tsm.Visible = false;
            }
            foreach (ToolStripItem tsm in addToolStripMenuItem.DropDownItems)
            {
                tsm.Enabled = false;
            }
        }

        private void ResetLicenseTypeContextMenu()
        {
            foreach (ToolStripItem tsm in newLicenseToolStripMenuItem.DropDownItems)
            {
                tsm.Enabled = false;                
            }
        }

        private void ResetMainToolStripMenu()
        {
            foreach (ToolStripItem tsm in MainToolStrip.Items)
            {
                if(!(tsm.Tag is ToolStripSeparator))
                    tsm.Enabled = false;
            }
            foreach (ToolStripItem tsm in addLicMainToolStripBtn.DropDownItems)
            {
                tsm.Enabled = false;
            }
            foreach (ToolStripItem tsm in addOrderMainToolStripDropDownBtn.DropDownItems)
            {
                tsm.Enabled = false;
            }
            toggleViewMainToolStripBtn.Enabled = true;
        }

        private void EnablePacketContextMenu(bool value)
        {

        }

        private void EnableCustomerContextMenu(bool value)
        {
            expandToolStripMenuItem.Visible = !DetailTreeView.SelectedNode.IsExpanded;
            expandToolStripMenuItem.Enabled = (DetailTreeView.SelectedNode.Nodes.Count == 0) ? false : true;
            collapseToolStripMenuItem.Visible = DetailTreeView.SelectedNode.IsExpanded;
            addToolStripMenuItem.Visible = value;
            newCustomerToolStripMenuItem.Visible = value;
            newLicenseToolStripMenuItem.Visible = value;
            newOrderToolStripMenuItem.Visible = !value;
            lcmToolStripSeparator1.Visible = value; 
            existingOrderToolStripMenuItem.Visible = !value;
            resyncToolStripMenuItem.Visible = value;
            resyncToolStripMenuItem.Enabled = false;
            deleteToolStripMenuItem.Visible = value;
            addLicenseToolStripSeparator.Visible = !value;
            newFailoverToolStripMenuItem.Visible = !value;
            newDisRecToolStripMenuItem.Visible = !value;
            newTestDevToolStripMenuItem.Visible = !value;
            lcmToolStripSeparator2.Visible = value;
            newCustomerToolStripMenuItem.Enabled = m_Permissions.pt_create_modify_key.Value;
            newLicenseToolStripMenuItem.Enabled = m_Permissions.pt_create_modify_key.Value;
            deleteToolStripMenuItem.Enabled = m_Permissions.pt_create_modify_key.Value;
        }

        private void EnableLicenseMainToolStripDropDownMenuItems()
        {
            //standard is enabled customer node selected
            if (DetailTreeView.SelectedNode.Tag is CustomerProperty)
                newStandardLicenseToolStripMenuItem.Enabled = true;
            else
            {
                LicenseServerProperty licData = DetailTreeView.SelectedNode.Tag as LicenseServerProperty;
                //add license items
                if (licData.IsStandardLicenseType)
                {
                    if (licData.FailoverLicenseCount == 0)
                        newFailoverLicenseToolStripMenuItem.Enabled = true;
                    newDisasterRecoveryLicenseToolStripMenuItem.Enabled = true;
                    newTestDevelopmentLicenseToolStripMenuItem.Enabled = true;
                }
            }
        }

        private void EnableOrderMainToolStripDropDownMenuItems()
        {
            LicenseServerProperty licData = DetailTreeView.SelectedNode.Tag as LicenseServerProperty;

            //also need to account for admin allowed extra new orders 
            if (m_Permissions.pt_create_modify_key.Value)
            {   //disable new order if at least two validation tokens do not exist                
                if (licData.IsEnabled)
                {
                    newOrderMainToolStripMenuItem.Enabled = false;
                    newOrderMainToolStripMenuItem.Enabled = false;
                    if (licData.IsStandardLicenseType)
                        newOrderMainToolStripMenuItem.Enabled = true;
                    else
                    {
                        if(!licData.IsFailoverLicenseType)
                            existingOrderMainToolStripMenuItem.Enabled = true;
                    }
                }
            }
        }

        private void EnableLicenseContextMenu(bool value)
        {
            LicenseServerProperty licData = DetailTreeView.SelectedNode.Tag as LicenseServerProperty;
            expandToolStripMenuItem.Visible = !DetailTreeView.SelectedNode.IsExpanded;
            expandToolStripMenuItem.Enabled = (DetailTreeView.SelectedNode.Nodes.Count == 0) ? false : true;
            collapseToolStripMenuItem.Visible = DetailTreeView.SelectedNode.IsExpanded;
            //add license items
            addToolStripMenuItem.Visible = true;
            newCustomerToolStripMenuItem.Visible = false;
            newLicenseToolStripMenuItem.Visible = false;
            newOrderToolStripMenuItem.Visible = true;
            existingOrderToolStripMenuItem.Visible = true;
            addLicenseToolStripSeparator.Visible = true;
            newFailoverToolStripMenuItem.Visible = true;
            newDisRecToolStripMenuItem.Visible = true;
            newTestDevToolStripMenuItem.Visible = true;
            //separator item
            lcmToolStripSeparator1.Visible = true;
            //generate license item
            createPacketFileToolStripMenuItem.Visible = true;
            //separator item
            lcmToolStripSeparator2.Visible = true;        
            //separator item
            lcmToolStripSeparator3.Visible = true;
            //also need to account for admin allowed extra new orders 
            if (m_Permissions.pt_create_modify_key.Value)
            {
                if (licData.IsStandardLicenseType)
                {
                    addLicenseToolStripSeparator.Visible = true;
                    //enable failover if one does already exist
                    if (licData.FailoverLicenseCount == 0)
                        newFailoverToolStripMenuItem.Enabled = true;
                    newDisRecToolStripMenuItem.Enabled = true;
                    newTestDevToolStripMenuItem.Enabled = true;
                }
                //license items
                deleteToolStripMenuItem.Enabled = m_Permissions.pt_create_modify_key.Value;
                //disable new order if at least two validation tokens do not exist                
                //allow refresh if license is not expired or has validation tokens          
                resyncToolStripMenuItem.Visible = true;
                resyncToolStripMenuItem.Enabled = false;
                deleteToolStripMenuItem.Visible = true;
                createPacketFileToolStripMenuItem.Enabled = false;
                if (licData.IsEnabled)
                {
                    //test/dev, failover, disaster recovery can be updated
                    if (licData.IsStandardLicenseType)
                        newOrderToolStripMenuItem.Enabled = true;
                    else
                    {
                        if (licData.HasUpdates)
                            resyncToolStripMenuItem.Enabled = true;
                        if(!licData.IsFailoverLicenseType)
                            existingOrderToolStripMenuItem.Enabled = true;
                    }
                    createPacketFileToolStripMenuItem.Enabled = true;
                }
            }
        }

        private void EnableOrderContextMenu(bool value)
        {
            expandToolStripMenuItem.Visible = !DetailTreeView.SelectedNode.IsExpanded;
            expandToolStripMenuItem.Enabled = (DetailTreeView.SelectedNode.Nodes.Count == 0) ? false : true;
            collapseToolStripMenuItem.Visible = DetailTreeView.SelectedNode.IsExpanded;
            lcmToolStripSeparator1.Visible = value;
            resyncToolStripMenuItem.Visible = value;
            resyncToolStripMenuItem.Enabled = false;
            deleteToolStripMenuItem.Visible = value;
            deleteToolStripMenuItem.Enabled = m_Permissions.pt_create_modify_key.Value;
            if (DetailTreeView.SelectedNode.Tag is ProductLicenseProperty)
            {
                if ((DetailTreeView.SelectedNode.Tag as ProductLicenseProperty).LicenseName.Contains("F"))
                    deleteToolStripMenuItem.Enabled = false;
            }
        }

        private void EnableDetailListView(bool value)
        {
            DetailSplitContainer.Panel2Collapsed = value;
        }

        //Need Global State Object to determine on/off
        //1) handle permissions
        //2) handle rights depending on current selected object
        private void EnableToolStripMenu(TreeNode node)
        {
            ResetMainToolStripMenu();
            newCustMainToolStripBtn.Enabled = true;
            toolStripSearchBox.Enabled = true;
            SearchToolStripLabel.Enabled = true;
            switch (node.Level)
            {
                case 0: //customer
                    addLicMainToolStripBtn.Enabled = true;
                    EnableLicenseMainToolStripDropDownMenuItems();                    
                    break;
                case 1: //license
                    addLicMainToolStripBtn.Enabled = true;
                    addOrderMainToolStripDropDownBtn.Enabled = true;
                    createPacketMainToolStripBtn.Enabled = true;                    
                    EnableLicenseMainToolStripDropDownMenuItems();
                    EnableOrderMainToolStripDropDownMenuItems();
                    break;
                case 2: //product
                case 3: //order
                case 4: //add-on order
                    break;
            }            
        }

        #endregion

        #region Form Events

        private void CreatorForm_Load(object sender, EventArgs e)
        {
            MainTreeView.Visible = false;
            MainTreeView.ExpandAll();
            // Set Server Connection List
            if (Settings.Default.ServerList != null)
            {
                String[] serverList = new String[Settings.Default.ServerList.Count];
                Settings.Default.ServerList.CopyTo(serverList, 0);
                this.m_ServerList.AddRange(serverList);
            }
            if (Settings.Default.SelectedDirectory != null)
            {
                m_selectedDirectory = Settings.Default.SelectedDirectory;
            }
            if (m_selectedDirectory.Length == 0)
            {
                m_selectedDirectory = Environment.GetFolderPath(Environment.SpecialFolder.Desktop);
            }
        }

        #region MainTreeView Events
        private void MainTreeView_BeforeSelect(object sender, TreeViewCancelEventArgs e)
        {
            //if (e.Node.Level == 0)
            //{

            //    e.Cancel = true;
            //}
            /* if (MainTreeView.SelectedNode != null)
             {
                 if (MainTreeView.SelectedNode.Text.Equals("Licenses") && !e.Node.Text.Equals("Licenses"))
                     m_TreeState = SaveTreeState(DetailTreeView);
             }
             */
        }

        private void MainTreeView_AfterSelect(object sender, TreeViewEventArgs e)
        {        
          //ignore top level root nodes
            if (e.Node.Level > 0)
            {
                //countToolStripStatusLabel.Text = "";
                if (MainTreeView.SelectedNode.Text == "Licenses")
                {
                    ResetMainToolStripMenu();
                    SearchCurrentView();
                    EnableDetailListView(true);
                    MainSplitContainer.Panel2.Controls.Clear();
                    //RestoreTreeState(DetailTreeView, m_TreeState);                    
                    loadingCircle1.Parent = MainSplitContainer.Panel2;
                    SubSplitContainer.Parent = MainSplitContainer.Panel2;
                }
                else if (MainTreeView.SelectedNode.Text == "Packets")
                {
                    ResetMainToolStripMenu();
                    SearchCurrentView();
                    MainSplitContainer.Panel2.Controls.Clear();
                    //panel1.Parent = MainSplitContainer.Panel2;
                    loadingCircle1.Parent = MainSplitContainer.Panel2;
                    PacketListView.Parent = MainSplitContainer.Panel2;

                }
                else if (MainTreeView.SelectedNode.Text == "Hardware Keys")
                {
                    ResetMainToolStripMenu();
                    SearchCurrentView();
                    MainSplitContainer.Panel2.Controls.Clear();
                    loadingCircle1.Parent = MainSplitContainer.Panel2;
                    panel1.Parent = MainSplitContainer.Panel2;
                }
                else
                {
                    if (MainTreeView.SelectedNode.Text != "Reports")
                    {
                        LoadReportListView();
                        MainSplitContainer.Panel2.Controls.Clear();
                        //panel1.Parent = MainSplitContainer.Panel2;
                        loadingCircle1.Parent = MainSplitContainer.Panel2;
                        ReportListView.Parent = MainSplitContainer.Panel2;
                    }
                }
            }
        }

        private void MainTreeView_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                // Point where mouse is clicked
                Point p = new Point(e.X, e.Y);
                // Go to the node that the user clicked
                TreeNode node = MainTreeView.GetNodeAt(p);
                if (node != null)
                {
                    // Highlight the node that the user clicked.
                    // The node is highlighted until the Menu is displayed on the screen                                        
                    if (MainTreeView.SelectedNode.Parent != null)
                    {
                        if (MainTreeView.SelectedNode.Parent.Text == "Reports")
                            MainTreeViewContextMenuStrip.Show(MainTreeView, p);
                    }
                }
            }
        }
        #endregion

        #region DetailTreeView Events

        private void DetailTreeView_AfterSelect(object sender, TreeViewEventArgs e)
        {
            DetailPropertyGrid.Enabled = true;
            if(e.Node.Tag is CustomerProperty)
                EnableDetailListView(true);
            else
                EnableDetailListView(false);
            switch (e.Node.Level)
            {
                case 1:            //level 1 - License
                    LoadLicenseNode(e.Node);
                    m_CurrentLicenseName = (e.Node.Tag as LicenseServerProperty).Name;
                    createPacketMainToolStripBtn.Enabled = true;
                    break;
                case 2:            //level 2 - Product
                    LoadProductNode(e.Node);
                    m_CurrentLicenseName = (e.Node.Parent.Tag as LicenseServerProperty).Name;
                    DetailPropertyGrid.Enabled = false;
                    break;
                case 3:            //level 3 - Order
                    LoadProductNode(e.Node.Parent);
                    m_CurrentLicenseName = (e.Node.Parent.Parent.Tag as LicenseServerProperty).Name;
                    LicenseServerProperty licProperty = e.Node.Parent.Parent.Tag as LicenseServerProperty;
                    if (licProperty.IsBackupLicenseType)
                        DetailPropertyGrid.Enabled = false;
                    break;
                case 4:            //level 4 - AddOn Order
                    LoadProductNode(e.Node.Parent.Parent);
                    m_CurrentLicenseName = (e.Node.Parent.Parent.Parent.Tag as LicenseServerProperty).Name;
                    LicenseServerProperty addOnLicense = e.Node.Parent.Parent.Parent.Tag as LicenseServerProperty;
                    if (addOnLicense.IsBackupLicenseType)
                        DetailPropertyGrid.Enabled = false;
                    break;
                default:             //level 0 - Customer
                    LoadCustomerNode(e.Node);
                    m_CurrentLicenseName = "";
                    break;
            }            
            EnableToolStripMenu(e.Node);
            //major performance hit
            SetCustomerLicenseState(e.Node);
            SetCurrentViewTabPages(e.Node);
            DetailPropertyGrid.SelectedObject = e.Node.Tag;
        }

        private void SetCurrentViewTabPages(TreeNode node)
        {
            if (!PropertyGridTabControl.TabPages.Contains(PropertyGridTabPage))
                PropertyGridTabControl.TabPages.Add(PropertyGridTabPage);
            if (!(node.Tag is CustomerProperty))
            {
                LoadDetailListView(node.Tag);
                if (!PropertyGridTabControl.TabPages.Contains(TransactionsTabPage))
                    PropertyGridTabControl.TabPages.Add(TransactionsTabPage);
                if (!PropertyGridTabControl.TabPages.Contains(PacketTabPage))
                    PropertyGridTabControl.TabPages.Add(PacketTabPage);
            }
            else
            {
                PropertyGridTabControl.TabPages.Remove(TransactionsTabPage);
                PropertyGridTabControl.TabPages.Remove(PacketTabPage);
            }
            LoadSelectedTabPage(PropertyGridTabControl.SelectedTab);
        }

        private void DetailTreeView_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                // Point where mouse is clicked
                Point p = new Point(e.X, e.Y);
                // Go to the node that the user clicked
                TreeNode node = DetailTreeView.GetNodeAt(p);
                if (node != null)
                {
                    // Highlight the node that the user clicked.
                    // The node is highlighted until the Menu is displayed on the screen
                    DetailTreeView.SelectedNode = node;
                    // Find the appropriate ContextMenu based on the highlighted node
                    ResetContextMenu();
                    switch (node.Level)
                    {
                        case 0:
                            EnableCustomerContextMenu(true);
                            break;
                        case 1:
                            EnableLicenseContextMenu(true);
                            break;
                        case 2:
                            EnableOrderContextMenu(true);
                            break;
                        case 3:
                            EnableOrderContextMenu(true);
                            break;
                        case 4:
                            EnableOrderContextMenu(true);
                            break;
                        default:
                            return;
                    }
                    LicenseContextMenuStrip.Show(DetailTreeView, p);
                }
            }
        }

        private void DetailTreeView_BeforeExpand(object sender, TreeViewCancelEventArgs e)
        {
            if (e.Node.Tag is LicenseServerProperty)
            {   //Disable expand if key is not enabled to stop license functions.
                if (!(e.Node.Tag as LicenseServerProperty).IsEnabled)
                {
                    e.Cancel = true;
                    return;
                }
            }
            DetailTreeView.BeginUpdate();
            if (e.Node.Nodes.ContainsKey("virtual"))
            {
                e.Node.Nodes.RemoveByKey("virtual");
                switch (e.Node.Level)
                {
                    case 0:
                        LoadCustomerNode(e.Node);
                        break;
                    case 1:
                        LoadLicenseNode(e.Node);
                        break;
                    case 2:
                        LoadProductNode(e.Node);
                        break;
                    default:
                        break;
                }
                SetCustomerLicenseState(e.Node);
            }
            DetailTreeView.EndUpdate();
        }

        #endregion

        #region DetailListView Events

        private void DetailListView_MouseUp(object sender, MouseEventArgs e)
        {
            bool bShow = false;
            //needs to handle validation properties also
            if (e.Button == MouseButtons.Right)
            {
                ListViewItem item = DetailListView.GetItemAt(e.X, e.Y);
                dlvNewToolStripMenuItem.Visible = false;
                dlvEditToolStripMenuItem.Enabled = false;
                dlvDeleteToolStripMenuItem.Enabled = false;
                if (DetailTreeView.SelectedNode.Tag is LicenseServerProperty)
                {
                    bShow = true;
                    if (m_Permissions.pt_module_pwd.Value)
                    {
                        dlvNewToolStripMenuItem.Visible = true;
                        if (item != null)
                        {   //enable edit/delete menu items for item selected
                            dlvEditToolStripMenuItem.Enabled = true;
                            dlvDeleteToolStripMenuItem.Enabled = (item.Tag as ValidationProperty).IsAllowedRemoveToken;
                        }
                    }
                }
                if (DetailTreeView.SelectedNode.Tag is ProductLicenseProperty)
                {
                    bShow = true;
                    if (m_Permissions.pt_module_pwd.Value)
                    {
                        LicenseServerProperty license;
                        if(DetailTreeView.SelectedNode.Parent.Parent.Tag is LicenseServerProperty)
                            license = DetailTreeView.SelectedNode.Parent.Parent.Tag as LicenseServerProperty;
                        else
                            license = DetailTreeView.SelectedNode.Parent.Parent.Parent.Tag as LicenseServerProperty;
                        if (license.IsStandardLicenseType)
                        {
                            ProductLicenseProperty selectedOrder = DetailTreeView.SelectedNode.Tag as ProductLicenseProperty;
                            //case 1: no item selected - no edit, delete
                            //case 2: trial - no new, delete, yes edit
                            //case 3: permanent - no new, delete default mod yes edit, non default ok new, edit, delete
                            //case 4; add on - yes new, edit, delete
                            //no item selected, don't allow edit or delete
                            //***need new rules for test/dev
                            if (item != null)
                            {
                                dlvEditToolStripMenuItem.Enabled = true;
                                dlvDeleteToolStripMenuItem.Enabled = (item.Tag as ModuleProperty).IsAllowedRemoveModule;
                            }
                        }
                        else if (license.IsTestDevelopmentLicenseType)
                        {
                            ProductLicenseProperty selectedOrder = DetailTreeView.SelectedNode.Tag as ProductLicenseProperty;
                            if (item != null)
                            {
                                //only if order is an admin order
                                //-admin order does not exist in standard order
                                //-difference between admin vs regular test dev order is
                                //if(selectedOrder.)
                                //{
                                    dlvEditToolStripMenuItem.Enabled = true;
                                    dlvDeleteToolStripMenuItem.Enabled = (item.Tag as ModuleProperty).IsAllowedRemoveModule; ;
                                //}
                            }
                        }
                    }
                }
                if (bShow)
                    DetailListViewContextMenuStrip.Show(DetailListView, e.X, e.Y);
            }
        }

        private void DetailListView_ColumnClick(object sender, ColumnClickEventArgs e)
        {
            _lvManager.SetSortIndexColumn(DetailListView.Handle, e.Column);
            // Perform the sort with these new sort options.
            DetailListView.Sort();
        }
        private void DetailListView_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Control && e.KeyCode.Equals(Keys.A))
            {
                foreach (ListViewItem lvItem in DetailListView.Items)
                {
                    lvItem.Selected = true;
                }
            }

            if(m_Permissions.pt_create_modify_key.Value)
            {
                if(DetailListView.SelectedItems.Count > 0 && e.KeyCode.Equals(Keys.Delete))
                {
                    if (DetailTreeView.SelectedNode.Tag is LicenseServerProperty)   //token remove
                        if(dlvRemoveToolStripButton.Enabled)
                            dlvRemoveToken(DetailTreeView.SelectedNode.Tag as LicenseServerProperty);
                    else                //module remove
                        dlvRemoveModule(DetailTreeView.SelectedNode.Tag as ProductLicenseProperty);
                }
            }
            if(e.KeyCode.Equals(Keys.Insert))
            {
                if (DetailTreeView.SelectedNode.Tag is LicenseServerProperty)
                    CreateNewValidationToken(DetailTreeView.SelectedNode.Tag as LicenseServerProperty);
            }
        }
        #endregion

        #region DetailPropertyGrid Events
        private void DetailPropertyGrid_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {
            //changes for license property and order property
            //if property value changed want to mark license as modified
            //save object back to database            
            if (!e.OldValue.Equals(e.ChangedItem.Value))
                SaveLicense();
        }
        #endregion


        private int ConnectServer()
        {
            //using keyword forces scope onto object, so it will be disposed after it is done.
            using (ConnectDialog cd = new ConnectDialog())
            {
                ConnectDialogData data = new ConnectDialogData(m_ServerList.ToArray());
                if (cd.ShowDialog(this, data) == DialogResult.OK)
                {
                    try
                    {
                        string hostName = data.ServerList.First();
                        this.Text = AppConstants.ClientTitle + " [" + hostName + "]";
                        Service<ICreator>._channelFactory.Endpoint.Address = new EndpointAddress(new Uri("net.tcp://" + hostName + ":9091/Creator/tcp"));
                        Service<ICreator>.Use((client) =>
                        {
                            m_Permissions = client.GetPermissionsByUser(WindowsIdentity.GetCurrent().Name);
                        });
                        MainTreeView.Visible = true;
                        MainTreeView.SelectedNode = null;
                        MainTreeView.SelectedNode = MainTreeView.Nodes.Find("LicenseNode", true).FirstOrDefault();
                        //shortCutToolStripComboBox.SelectedIndex = 0;
                    }
                    catch (COMException ex)
                    {
                    }
                }
                //save server list into settings again 
                String[] serverList = new string[data.ServerList.Count];
                data.ServerList.CopyTo(serverList, 0);
                m_ServerList.Clear();
                m_ServerList.AddRange(serverList);
            }
            return 0;
        }
        //any change made by standard needs to be applied to backup/disaster
        private void SaveLicense()
        {          
            Lic_PackageAttribs licPackage;
            LicenseTable licRec;
            bool bOrderChanged = false;
            ProductProperty orderProduct = null;
            ProductLicenseProperty selectedOrder = null;
            OrderTable storedOrder = null;
            Service<ICreator>.Use((client) =>
            {
                switch (DetailTreeView.SelectedNode.Level)
                {
                    case 0:
                        //customer level
                        CustomerProperty custItem = DetailTreeView.SelectedNode.Tag as CustomerProperty;
                        string oldValue = DetailTreeView.SelectedNode.Text.Remove(DetailTreeView.SelectedNode.Text.IndexOf("(")).Trim();
                        CustomerTable custRec = client.GetCustomer(oldValue, true);
                        custRec.SCRname = custItem.Name;
                        client.UpdateCustomer(custRec);
                        DetailTreeView.SelectedNode.Text = string.Format("{0} ({1})", custRec.SCRname, custRec.LicenseTables.Count());
                        SetCustomerLicenseState(DetailTreeView.SelectedNode);
                        break;
                    case 1:
                        //license level - only changes activations, validation tokens
                        licPackage = new Lic_PackageAttribs();
                        LicenseServerProperty licItem = DetailTreeView.SelectedNode.Tag as LicenseServerProperty;
                        licRec = client.GetLicenseByName(DetailTreeView.SelectedNode.Text, false);
                        licPackage.Stream = licRec.LicenseInfo;
                        LicenseItemTransactions(ref licPackage.licLicenseInfoAttribs, ref licItem);
                        licRec.LicenseInfo = licPackage.Stream;
                        licRec.LicenseComments = licItem.Comments;
                        client.UpdateLicense(licRec);
                        LoadTransactionItems(licItem.Name);
                        SetCustomerLicenseState(DetailTreeView.SelectedNode);
                        break;
                    case 2:
                        break;
                    case 3:
                        //orderdata level - no add-on orders
                        //alert failover, test dev, disaster recovery 
                        selectedOrder = DetailTreeView.SelectedNode.Tag as ProductLicenseProperty;
                        storedOrder = client.GetOrderByOrderName(selectedOrder.OrderNumber);
                        bOrderChanged = OrderTransactions(ref storedOrder, selectedOrder);
                        client.UpdateOrder(storedOrder);
                        licPackage = new Lic_PackageAttribs();
                        licRec = client.GetLicenseByName(selectedOrder.LicenseName, false);
                        licPackage.Stream = licRec.LicenseInfo;                   
                        ProductItemTransactions(ref licPackage.licLicenseInfoAttribs, selectedOrder, bOrderChanged);
                        licRec.LicenseInfo = licPackage.Stream;
                        client.UpdateLicense(licRec);
                        //issue need to repopulate ProductNode with updated module values from ProductItemTransactions
                        orderProduct = DetailTreeView.SelectedNode.Parent.Tag as ProductProperty;
                        foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs product in licPackage.licLicenseInfoAttribs.TVal.productList.TVal)
                        {
                            if (product.productID.TVal.Equals(selectedOrder.Product.ID))
                            {
                                orderProduct.ModuleList.TVal = product.moduleList.TVal;
                                orderProduct.Version = selectedOrder.ProductVersion;
                                break;
                            }
                        }
                        //other orders may need to be updated with the latest license version
                        foreach (TreeNode node in DetailTreeView.SelectedNode.Parent.Nodes)
                        {
                            ProductLicenseProperty orderNode = node.Tag as ProductLicenseProperty;
                            orderNode.ProductVersion = selectedOrder.ProductVersion;
                        }
                        //need to reload customer node to set modified and pending states for any licenses
                        //load license node to pass changes to secondary licenses if necessary.
                        //SetLicenseState
                        SetCustomerLicenseState(DetailTreeView.SelectedNode);
                        LoadLicenseNode(DetailTreeView.SelectedNode.Parent.Parent);
                        LoadProductNode(DetailTreeView.SelectedNode.Parent);
                        LoadDetailListView(DetailTreeView.SelectedNode.Tag as ProductLicenseProperty);
                        LoadTransactionItems(selectedOrder.LicenseName);
                        break;
                    case 4:
                        //Add-on orderdata level         
                        TreeNode productNode = DetailTreeView.SelectedNode.Parent.Parent;
                        orderProduct = DetailTreeView.SelectedNode.Parent.Parent.Tag as ProductProperty;
                        selectedOrder = DetailTreeView.SelectedNode.Tag as ProductLicenseProperty;
                        storedOrder = client.GetOrderByOrderName(selectedOrder.OrderNumber);
                        //need to delete order if converting from add-on to perm
                        //issue, add-on order merged into perm, how to show transaction      
                        //delete order
                        licPackage = new Lic_PackageAttribs();
                        licRec = client.GetLicenseByName(selectedOrder.LicenseName, false);
                        licPackage.Stream = licRec.LicenseInfo;
                        Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState currentState = (Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState)storedOrder.OrderState;
                        bOrderChanged = OrderTransactions(ref storedOrder, selectedOrder);
                        if (currentState == Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msAddOn &&
                           selectedOrder.OrderStatus == Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msPerm)
                        {
                            client.DeleteOrder(storedOrder);
                            DetailTreeView.SelectedNode = DetailTreeView.SelectedNode.Parent;
                            //return updated perm order contract
                            (DetailTreeView.SelectedNode.Tag as ProductLicenseProperty).Product.Product = AddOnProductTransactions(ref licPackage.licLicenseInfoAttribs, selectedOrder);
                            //need to update parent order with values from add-on order
                            DetailTreeView.SelectedNode.Nodes.RemoveByKey(selectedOrder.OrderNumber);
                            productNode = DetailTreeView.SelectedNode.Parent;
                        }
                        else
                        {
                            client.UpdateOrder(storedOrder);
                            ProductItemTransactions(ref licPackage.licLicenseInfoAttribs, selectedOrder, bOrderChanged);
                        }

                        //need to update productitemtransactions to record addon transactions
                        //issue selectedOrder add-on order has perm modules to add to already existing perm modules
                        //                    regular orders replace values.
                        licRec.LicenseInfo = licPackage.Stream;
                        client.UpdateLicense(licRec);
                        //issue need to repopulate ProductNode with updated module values from ProductItemTransactions
                        foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs product in licPackage.licLicenseInfoAttribs.TVal.productList.TVal)
                        {
                            if (product.productID.TVal.Equals(selectedOrder.Product.ID))
                            {
                                orderProduct.ModuleList.TVal = product.moduleList.TVal;
                                orderProduct.Version = selectedOrder.ProductVersion;
                                break;
                            }
                        }
                        SetCustomerLicenseState(DetailTreeView.SelectedNode);
                        LoadProductNode(productNode);
                        LoadDetailListView(DetailTreeView.SelectedNode.Tag as ProductLicenseProperty);
                        LoadTransactionItems(selectedOrder.LicenseName);
                        break;
                    default:
                        break;
                }
            });
        }

        private void deleteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            bool bDeleted = false;
            TreeNode parentNode = DetailTreeView.SelectedNode.Parent;
            int selectedIndex = DetailTreeView.SelectedNode.Index;
            if (DetailTreeView.SelectedNode.Tag is CustomerProperty)
                bDeleted = DeleteCustomerNode(DetailTreeView.SelectedNode);
            if (DetailTreeView.SelectedNode.Tag is LicenseServerProperty)
                bDeleted = DeleteLicenseNode(DetailTreeView.SelectedNode);
            if (DetailTreeView.SelectedNode.Tag is ProductLicenseProperty)
                bDeleted = DeleteOrderNode(DetailTreeView.SelectedNode, true);
            if (bDeleted)
            {
                if (parentNode.Nodes.Count > 0)
                {
                    if (selectedIndex == 0)
                        DetailTreeView.SelectedNode = parentNode.Nodes[selectedIndex];
                    else
                        DetailTreeView.SelectedNode = parentNode.Nodes[selectedIndex - 1];
                }
            }
        }

        //issue std selected license node is customer node
        //      all others are std license node 
        private void AddLicenseNodeToDetailTreeView(LicenseDialogData data)
        {
            TreeNode custNode = DetailTreeView.SelectedNode;
            //licInfo has correct license information
            //failover, DR still need to populate modules,products, orders
            TreeNode newNode = CreateLicenseNode(data.LicInfo);
            DetailTreeView.BeginUpdate();
            if (DetailTreeView.SelectedNode.Tag is LicenseServerProperty)
                custNode = DetailTreeView.SelectedNode.Parent;
            //resort tree
            custNode.Nodes.Clear();
            LoadCustomerNode(custNode);
            SetCustomerLicenseState(custNode);
            custNode.Text = string.Format("{0} ({1})", custNode.Name, custNode.Nodes.Count);            
            DetailTreeView.SelectedNode = custNode.Nodes.Find(newNode.Name, false).FirstOrDefault();
            DetailTreeView.EndUpdate();
            DetailPropertyGrid.SelectedObject = newNode.Tag;
        }
        
        private void CreateStandardLicense()
        {
            LicenseServerProperty newLicense = new LicenseServerProperty();
            TreeNode selectedNode = DetailTreeView.SelectedNode;
            //set to customer node if selected is license node
            if (DetailTreeView.SelectedNode.Tag is LicenseServerProperty)
                selectedNode = DetailTreeView.SelectedNode.Parent;
            Service<ICreator>.Use((client) =>
            {
                newLicense.CustID = (uint)client.GetCustomer(selectedNode.Name, false).SCRnumber;
                newLicense.GroupID = client.GetNextGroupID(newLicense.CustID, 0);
            });
            newLicense.Permissions = m_Permissions;
            //display settings form for product connections
            using (LicenseInfoForm dlg = new LicenseInfoForm("Create New License", ref s_CommLink))
            {
                LicenseDialogData data = new LicenseDialogData(newLicense);
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                    AddLicenseNodeToDetailTreeView(data);
            }
        }

        private void CreateFailoverLicense()
        {
            LicenseServerProperty selectedLicense = new LicenseServerProperty(DetailTreeView.SelectedNode.Tag as LicenseServerProperty);
            //pass to function to convert std to failover
            selectedLicense.LicType = Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltFailover;
            //display settings form for product connections
            using (LicenseInfoForm dlg = new LicenseInfoForm("Create New License", ref s_CommLink))
            {
                LicenseDialogData data = new LicenseDialogData(selectedLicense);
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                    AddLicenseNodeToDetailTreeView(data);
            }
        }

        private void CreateDRLicense()
        {
            LicenseServerProperty selectedLicense = new LicenseServerProperty(DetailTreeView.SelectedNode.Tag as LicenseServerProperty);
            //pass to function to convert std to failover
            selectedLicense.LicType = Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltDisasterRecovery;
            using (LicenseInfoForm dlg = new LicenseInfoForm("Create New License", ref s_CommLink))
            {
                LicenseDialogData data = new LicenseDialogData(selectedLicense);
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                    AddLicenseNodeToDetailTreeView(data);
            }
        }

        private void CreateTestDevLicense()
        {
            LicenseServerProperty selectedLicense = new LicenseServerProperty(DetailTreeView.SelectedNode.Tag as LicenseServerProperty);
            selectedLicense.LicType = Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltTestDev;
            //pass to function to convert std to failover
            using (LicenseInfoForm dlg = new LicenseInfoForm("Create New License", ref s_CommLink))
            {
                LicenseDialogData data = new LicenseDialogData(selectedLicense);
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                    AddLicenseNodeToDetailTreeView(data);
            }
        }
        private void newLicenseToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CreateStandardLicense();
        }

        //copies 
        private void newFailoverLicenseToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CreateFailoverLicense();
        }

        //copies license attribs, clears tokens, doesn't copy orders
        private void newDRLicenseToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CreateDRLicense();
        }

        //copies license attribs, clears tokens, doesn't copy orders
        private void newTestDevLicenseToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CreateTestDevLicense();
        }

        private void GenerateLicensePacket()
        {
            if (!(m_selectedDirectory.Length > 0))
                m_selectedDirectory = Directory.GetCurrentDirectory();
            PacketProperty newPacket = new PacketProperty(string.Format("{0}-{1}-{2}-{3}-{4}-{5}-{6}", DetailTreeView.SelectedNode.Name, DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day, DateTime.Now.Hour, DateTime.Now.Minute, DateTime.Now.Second),
                                                  "",
                                                  DateTime.Today,
                                                  Path.Combine(m_selectedDirectory, DetailTreeView.SelectedNode.Parent.Name),
                                                  "");
            
            using (LicenseInfoForm dlg = new LicenseInfoForm("Create New Packet", ref s_CommLink))
            {
                PacketDialogData data = new PacketDialogData(newPacket, DetailTreeView.SelectedNode.Parent.Name);
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                {
                    m_selectedDirectory = Path.GetFullPath(data.PacketInfo.OutputPath + "\\..");
                    GeneratePacket(data.PacketInfo.Name, DetailTreeView.SelectedNode.Name, data.ExpDate, data.PacketInfo.Description, data.PacketInfo.OutputPath);
                    //restore license text to normal from modified
                    
                }
            }
        }
        private void generateLicenseFileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            GenerateLicensePacket();
        }

        private void dlvNewToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (DetailTreeView.SelectedNode.Tag is LicenseServerProperty)
                CreateNewValidationToken(DetailTreeView.SelectedNode.Tag as LicenseServerProperty);
        }

        private void dlvEditToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ShowEditDialog(DetailTreeView.SelectedNode.Tag, DetailListView.SelectedIndices);
        }

        private void ShowEditDialog(Object nodeObj, ListView.SelectedIndexCollection selectedIndices)
        {
            if (!(nodeObj is ProductProperty))
            {
                if (nodeObj is LicenseServerProperty)
                    EditValidationToken(selectedIndices);
                else
                    EditModules(selectedIndices);
            }
        }

        private void EditValidationToken(ListView.SelectedIndexCollection selectedIndexes)
        {
            using (LicenseInfoForm dlg = new LicenseInfoForm("Modify Validation Token", ref s_CommLink))
            {
                //disable modifying type                
                ValidationProperty tokenInfo = DetailListView.Items[selectedIndexes[0]].Tag as ValidationProperty;
                LicenseServerProperty selectedLicense =  DetailTreeView.SelectedNode.Tag as LicenseServerProperty;
                TokenDialogData data = new TokenDialogData(tokenInfo,selectedLicense);
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                {
                    Service<ICreator>.Use((client) =>
                    {
                        TokenTable dbToken = client.GetTokenByLicenseName(selectedLicense.Name, (byte)data.Token.TokenType);
                        dbToken.TokenType = (byte)data.Token.TokenType;
                        dbToken.TokenValue = data.Token.TokenValue;
                        client.UpdateToken(dbToken);
                    });
                    //save token to db and licinfo
                    SaveLicense();
                    PopulateDetailListView(data.LicInfo);
                }
            }
        }
        private void EditModules(ListView.SelectedIndexCollection selectedIndexes)
        {
            List<string> moduleList = new List<string>();
            foreach (int index in selectedIndexes)
            {
                moduleList.Add(DetailListView.Items[index].Text);
            }
            using (LicenseInfoForm dlg = new LicenseInfoForm("Modify Module", ref s_CommLink))
            {
                ModuleDialogData data = new ModuleDialogData(moduleList, DetailTreeView.SelectedNode.Tag as ProductLicenseProperty);
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                {                   
                    SaveLicense();                    
                    PopulateDetailListView(data.OrderData);
                }
            }
        }

        private void dlvRemoveToken(LicenseServerProperty selectedLicense)
        {
            bool bModified = false;
            DialogResult dlgResult = DialogResult.None;
            TokenTable token = null;
            ValidationProperty selectedToken;
            //Get Confirmation of Delete from User
            foreach (ListViewItem selectedItem in DetailListView.SelectedItems)
            {
                selectedToken = selectedItem.Tag as ValidationProperty;
                if (DetailListView.Items.Count > 2)
                    dlgResult = MessageBox.Show(string.Format("Please confirm delete token : {0}.", selectedToken.Name), "Delete Token Confirmation",
                                MessageBoxButtons.OKCancel, MessageBoxIcon.Exclamation);
                else
                    dlgResult = MessageBox.Show(string.Format("Deleting token : {0} will disable license : {1}.\nClick OK to continue with deletion of token.", selectedToken.Name, selectedLicense.Name), "Delete Token Confirmation",
                                MessageBoxButtons.OKCancel, MessageBoxIcon.Warning);
                if (dlgResult == DialogResult.OK)
                {
                    selectedLicense.LicInfo.licVerificationAttribs.TVal.validationTokenList.TVal.Remove(selectedToken.ValidationToken);
                    DetailListView.Items.Remove(selectedItem);
                    PopulateDetailListView(selectedLicense);
                    Service<ICreator>.Use((client) =>
                    {
                        token = client.GetTokenByLicenseName(selectedLicense.Name, (byte)selectedToken.TokenType);
                        if (token != null)
                        {
                            if (token.TokenType == (byte)Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID)
                            {
                                token.LicenseID = -1;
                                client.UpdateToken(token);
                            }
                            else
                                client.DeleteToken(token);
                        }
                    });
                    CreateTransaction(TransactionType.License, selectedLicense.Name, string.Format("Removed Token - {0}", selectedToken.Name), selectedToken.TokenValue);
                    bModified = true;
                }
            }
            if (bModified)
            {
                SaveLicense();
                SetCustomerLicenseState(DetailTreeView.SelectedNode);
            }
        }

        private void dlvRemoveModule(ProductLicenseProperty selectedOrder)
        {
            //module remove
            ModuleProperty module;
            foreach (ListViewItem selectedItem in DetailListView.SelectedItems)
            {
                module = selectedItem.Tag as ModuleProperty;
                if (!s_CommLink.IsDefaultModule(selectedOrder.Product.ID, module.ID))
                {
                    //set module to nothing 
                    module.Units = 0;
                    module.AppInstance = 0;
                    if (module.ModuleState != Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msAddOn)
                    {
                        if (GetTotalAddOnModuleAppInstance(selectedOrder, module) == 0)
                            IncrementAddOnModuleAppInstance(selectedOrder, module);
                    }                     
                    //0 = name
                    //1 = value                        
                    selectedItem.SubItems[1].Text = module.Units.ToString();
                }
                CreateTransaction(TransactionType.Product, selectedOrder, string.Format("Removed Module - {0}", s_CommLink.GetModuleName(selectedOrder.Product.ID, module.ID)), module.Units.ToString());
            }
            SaveLicense();
        }

        private uint GetTotalAddOnModuleAppInstance(ProductLicenseProperty moduleData, ModuleProperty storedMod)
        {
            uint totalValue = 0;
            string stdProductLicense = "";
            IList<string> addOnOrders = null;
            Service<ICreator>.Use((client) =>
            {
                stdProductLicense = (storedMod.ModuleState != Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msAddOn) ? moduleData.OrderNumber : moduleData.ParentOrderNumber;
                addOnOrders = client.GetAddOnOrdersByStandardOrder(stdProductLicense);
            });
            foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs modRec in moduleData.Product.ModuleList.TVal)
            {
                if (modRec.moduleID.TVal.Equals(storedMod.ID) && (addOnOrders.Contains(modRec.contractNumber.TVal)))
                    totalValue += modRec.moduleAppInstance.TVal;
            }
            return totalValue;
        }

        private void IncrementAddOnModuleAppInstance(ProductLicenseProperty moduleData, ModuleProperty module)
        {
            //increment first add-on found
            IList<string> addOnOrders = null;
            Service<ICreator>.Use((client) =>
            {
                addOnOrders = client.GetAddOnOrdersByStandardOrder(module.OrderNumber);
            });
            if (addOnOrders != null)
            {
                if (addOnOrders.Count > 0)
                {
                    foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs storedMod in moduleData.Product.ModuleList.TVal)
                    {
                        if (storedMod.moduleID.TVal == module.ID &&
                            addOnOrders.Contains(storedMod.contractNumber.TVal))
                        {
                            if (storedMod.moduleValue.TVal > 0)                            
                                storedMod.moduleAppInstance.TVal = 1;                            
                            break;
                        }
                    }
                }
            }
        }

        private void dlvDeleteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if(DetailTreeView.SelectedNode.Tag is LicenseServerProperty)   //token remove
                dlvRemoveToken(DetailTreeView.SelectedNode.Tag as LicenseServerProperty);
            else                //module remove
                dlvRemoveModule(DetailTreeView.SelectedNode.Tag as ProductLicenseProperty);
        }

        private void TransactionListView_ColumnClick(object sender, ColumnClickEventArgs e)
        {
            _lvManager.SetSortIndexColumn(TransactionListView.Handle, e.Column);
            // Perform the sort with these new sort options.
            TransactionListView.Sort();
            SetSingleTransactionListViewItemBackColor();
        }

        private void toolStripSearchBox_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter || e.KeyCode == Keys.Return)
            {
                SearchCurrentView();
            }
        }

        private void verifyToolStripMenuItem_Click(object sender, EventArgs e)
        {
            int imageIndex = GetListViewIconIndex("VERIFIED");
            //ListViewItem selectedItem = LicensePacketListView.SelectedItems[0];
            using (PacketVerification dlg = new PacketVerification())
            {   //selectedItem text will be packet name
                PacketVerificationDialogData data = new PacketVerificationDialogData();
                int selectedIndex = 0;
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                {
                    if (data.Verified)
                    {
                        //only valid verification code can get to this point
                        foreach (ListViewItem item in storedPacketListView.Items)
                        {
                            if (item.Text.Equals(data.SelectedPacketName))
                            {
                                item.ImageIndex = imageIndex;
                                item.StateImageIndex = imageIndex;
                                selectedIndex = item.Index;
                                break;
                            }
                        }
                        if (toolStripComboBox1.SelectedItem.ToString().Equals("Unverified"))
                            ShowPacketListView("UNVERIFIED");
                        else if (toolStripComboBox1.SelectedItem.ToString().Equals("All"))
                            ShowPacketListView("");
                        ListViewItem selectedItem = LicensePacketListView.Items[selectedIndex];
                        selectedItem.Selected = true;
                    }
                }
            }
        }

        private void ShowPacketListView(string type)
        {
            LicensePacketListView.BeginUpdate();
            LicensePacketListView.Items.Clear();

            foreach (ListViewItem lvItem in storedPacketListView.Items)
            {
                if (lvItem.ImageIndex == GetListViewIconIndex(type) ||
                    type.Length == 0)
                    LicensePacketListView.Items.Insert(0,lvItem.Clone() as ListViewItem);
            }
            _lvManager.AutoResizeColumns(LicensePacketListView);
            LicensePacketListView.EndUpdate();
            //splitContainer1.Panel2Collapsed = true;            
        }

        private void newReportToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (ReportDlg dlg = new ReportDlg())
            {
                ReportDlgData data = new ReportDlgData();
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                {
                    //create new node under report root
                    //name it to be untitled plus one of the root children
                    //populate the listview with the licenses from service
                    TreeNode newChildNode = new TreeNode("Untitled Report " + MainTreeView.Nodes[1].Nodes.Count.ToString());
                    newChildNode.Tag = data.Report;
                    MainTreeView.Nodes[1].Nodes.Add(newChildNode);
                    MainTreeView.SelectedNode = newChildNode;
                }
            }
        }

        private void editReportToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (ReportDlg dlg = new ReportDlg())
            {
                ReportDlgData data = new ReportDlgData();
                data.Report = (MainTreeView.SelectedNode.Tag as ReportProperty);
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                {
                    MainTreeView.SelectedNode.Tag = data.Report;
                    MainTreeView.SelectedNode = MainTreeView.SelectedNode;
                }
            }
        }

        private void MainTreeView_KeyUp(object sender, KeyEventArgs e)
        {
            //capture F2 click
        }

        private void SetSingleTransactionListViewItemBackColor()
        {
            for (int i = 0; i < TransactionListView.Items.Count; i++)
            {
                if (i > 0)
                {
                    if (TransactionListView.Items[i - 1].BackColor == Color.AliceBlue)
                        TransactionListView.Items[i].BackColor = Color.White;
                    else
                        TransactionListView.Items[i].BackColor = Color.AliceBlue;
                }
                else
                    TransactionListView.Items[i].BackColor = Color.AliceBlue;
            }
        }

        private void SetGroupTransactionListViewItemBackColor()
        {
            foreach (ListViewGroup group in TransactionListView.Groups)
            {
                for (int i = 0; i < group.Items.Count; i++)
                {
                    if (i > 0)
                    {
                        if (group.Items[i - 1].BackColor == Color.AliceBlue)
                            group.Items[i].BackColor = Color.White;
                        else
                            group.Items[i].BackColor = Color.AliceBlue;
                    }
                    else
                        group.Items[i].BackColor = Color.AliceBlue;
                }
            }
        }
        private void GroupToolStripButton_Click(object sender, EventArgs e)
        {
            //group items by their group
            if (!GroupToolStripButton.Checked)
            {
                GroupToolStripButton.Checked = true;
                SingleToolStripButton.Checked = false;
                TransactionListView.ShowGroups = true;
                SetGroupTransactionListViewItemBackColor();
                //resetcolor for transactionitems
            }
        }

        private void SingleToolStripButton_Click(object sender, EventArgs e)
        {
            //don't group items
            if (!SingleToolStripButton.Checked)
            {
                GroupToolStripButton.Checked = false;
                SingleToolStripButton.Checked = true;
                TransactionListView.ShowGroups = false;
                SetSingleTransactionListViewItemBackColor();
                //resetcolor for transactionitems
            }
        }

        private void LicensePacketListView_SelectedIndexChanged(object sender, EventArgs e)
        {
            verifyToolStripMenuItem.Enabled = (LicensePacketListView.SelectedItems.Count > 0) ? true : false;
            if (LicensePacketListView.SelectedItems.Count > 0)
            {
                ListViewItem selectedItem = LicensePacketListView.SelectedItems[0];
                packetDescriptionRichTextBox.Text = (selectedItem.Tag as PacketTable).PacketComments;
                //splitContainer1.Panel2Collapsed = false;
            }
            else
            {
                //splitContainer1.Panel2Collapsed = true;
            }
        }
        #endregion

        #region Form Methods

        public void CreateNewValidationToken(LicenseServerProperty licInfo)
        {
            using (LicenseInfoForm dlg = new LicenseInfoForm("Create New Validation Token", ref s_CommLink))
            {
                Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs newToken = new Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs();
                newToken.tokenType.TVal = Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttNone;
                TokenDialogData data = new TokenDialogData(new ValidationProperty(newToken), licInfo);
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                {
                    TokenTable dbToken = null;
                    Service<ICreator>.Use((client) =>
                    {
                        LicenseTable dbLicense = client.GetLicenseByName(licInfo.Name, false);
                        if (data.Token.TokenType == Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID)
                        {
                            dbToken = client.GetHardwareTokenByKeyValue(data.LicInfo.CustID, data.Token.TokenValue);
                            dbToken.LicenseID = dbLicense.ID;
                            client.UpdateToken(dbToken);
                        }
                        else
                        {
                            dbToken = new TokenTable();
                            dbToken.LicenseID = dbLicense.ID;
                            dbToken.TokenType = (byte)data.Token.TokenType;
                            dbToken.TokenValue = data.Token.TokenValue;
                            dbToken.CustID = (int)data.LicInfo.CustID;
                            client.CreateToken(dbToken);
                        }
                    });
                    CreateTransaction(TransactionType.License, licInfo.Name, string.Format("Added Token - {0}", Enum.GetName(typeof(Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType), dbToken.TokenType)), dbToken.TokenValue);
                    SaveLicense();
                    PopulateDetailListView(data.LicInfo);
                }
            }
        }

        /*private Dictionary<string, bool> SaveTreeState(TreeView tree)
        {
            Dictionary<string, bool> nodeStates = new Dictionary<string, bool>();            
            SaveTreeNodeState(nodeStates, tree.Nodes);

            foreach(TreeNode node in tree.Nodes)
            {
                if (tree.Nodes[i].Nodes.Count > 0)
                {
                    if(!nodeStates.ContainsKey(tree.Nodes[i].Name))
                        nodeStates.Add(tree.Nodes[i].Name, tree.Nodes[i].IsExpanded);
                        foreach(TreeNode node in tree.Nodes)    
                }
            }
            return nodeStates;
        }

        private void SaveTreeNodeState(TreeNodeCollection nodes)
        {
            
            foreach (TreeNode node in nodes)
            {   
                node.Nodes.find
                if (!nodeStates.ContainsKey(node.Name))
                {
                    nodeStates.Add(node.Name, node.IsExpanded);
                    SaveTreeNodeState(node);
                }
            }
        }

        private void RestoreTreeState(TreeView tree, Dictionary<string, bool> treeState)
        {
            for (int i = 0; i < tree.Nodes.Count; i++)
            {
                if (treeState.ContainsKey(tree.Nodes[i].Name))
                {
                    if (treeState[tree.Nodes[i].Name])
                        tree.Nodes[i].Expand();
                    else
                        tree.Nodes[i].Collapse();
                }
            }
        }
         */
        private TreeNode CreateOrderNode(ProductLicenseProperty orderData)
        {
            CreateOrderEntity(orderData);
            TreeNode orderNode = new TreeNode(orderData.OrderNumber);
            orderNode.Name = orderData.OrderNumber;
            orderNode.Tag = orderData;
            if (orderData.ParentOrderNumber != null)
            {
                orderNode.ImageIndex = GetDetailTreeViewIconIndex("ADDONORDER");
                orderNode.SelectedImageIndex = orderNode.ImageIndex;
            }
            else
            {
                orderNode.ImageIndex = GetDetailTreeViewIconIndex("ORDER");
                orderNode.SelectedImageIndex = orderNode.ImageIndex;
            }
            return orderNode;
        }

        private void CreateOrderEntity(ProductLicenseProperty orderData)
        {
            OrderTable orderRec = new OrderTable();
            Service<ICreator>.Use((client) =>
            {
                LicenseTable licRec = client.GetLicenseByName(orderData.LicenseName, false);
                orderRec.OrderNumber = orderData.OrderNumber;
                orderRec.OrderIndex = (int)orderData.OrderIndex;
                orderRec.OrderState = (byte)orderData.OrderStatus;
                orderRec.LicenseID = licRec.ID;
                orderRec.ExpirationDate = orderData.ExpirationDate;
                orderRec.Description = orderData.Description;
                orderRec.ProductName = orderData.Product.Name;
                orderRec.ProductVersion = orderData.Product.Version.ToString();
                orderRec.ParentOrderNumber = orderData.ParentOrderNumber;
                client.CreateOrder(orderRec);
                CreateTransaction(TransactionType.Order, 
                                  orderData, 
                                  string.Format("Added {0} Product License - {1}", orderData.ProductName, orderData.OrderStatus.ToString()), 
                                  orderData.OrderNumber);
                //update order index to next available index
                licRec.OrderIndex = orderRec.OrderIndex + 1;
                client.UpdateLicense(licRec);
            });
            //create product order also 
        }

        string GetProductBaseName(string tdProductName)
        {
            int pos = tdProductName.IndexOf("Test");
            if (pos > 0)
                tdProductName = tdProductName.Remove(pos - 1);
            return tdProductName;
        }

        //create only done on new contract with new product
        private TreeNode CreateProductNode(ProductLicenseProperty orderData)
        {
            Lic_PackageAttribs.Lic_ProductInfoAttribs product = CreateProductEntity(orderData);
            TreeNode node = new TreeNode(orderData.Product.Name);
            node.Name = orderData.Product.Name;
            node.ImageIndex = GetDetailTreeViewIconIndex(GetProductBaseName(node.Name));
            node.SelectedImageIndex = node.ImageIndex;
            node.Tag = new ProductProperty(product);
            return node;
        }

        //returns the created product module list, issue comes when creating product node that already exists
        //need to add new product entity into existing product node not overwrite
        private Lic_PackageAttribs.Lic_ProductInfoAttribs CreateProductEntity(ProductLicenseProperty orderData)
        {
            Lic_PackageAttribs.Lic_ProductInfoAttribs product = null;
            SetProductEntity(orderData);
            Service<ICreator>.Use((client) =>
            {
                LicenseTable licRec = client.GetLicenseByName(orderData.LicenseName, false);
                Lic_PackageAttribs licPackage = new Lic_PackageAttribs();
                licPackage.Stream = licRec.LicenseInfo;
                bool bFound = false;
                foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs dbProduct in licPackage.licLicenseInfoAttribs.TVal.productList.TVal)
                {
                    if (dbProduct.productID.TVal.Equals(orderData.Product.ID))
                    {
                        dbProduct.moduleList.TVal.AddRange(orderData.Product.ModuleList.TVal);
                        dbProduct.product_Major.TVal = orderData.Product.Product.product_Major.TVal;
                        dbProduct.product_Minor.TVal = orderData.Product.Product.product_Minor.TVal;
                        if(orderData.OrderStatus != Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msAddOn)
                            dbProduct.productAppInstance.TVal++; //= orderData.Product.AppInstance;
                        product = dbProduct;
                        bFound = true;
                        break;
                    }
                }
                if (!bFound)
                {
                    licPackage.licLicenseInfoAttribs.TVal.productList.TVal.Add(orderData.Product.Product);
                    product = orderData.Product.Product;
                }
                licRec.LicenseInfo = licPackage.Stream;
                client.UpdateLicense(licRec);
                LicenseTable test = client.GetLicenseByName(licRec.LicenseName, false);
            });
            return product;
        }

        //fill product entity with default modules
        //only trial/addon order types
        private void SetProductEntity(ProductLicenseProperty orderData)
        {
            Lic_PackageAttribs.Lic_ModuleInfoAttribs module;
            Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.Lic_ModuleSoftwareSpecAttribsMap moduleList;
            foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in s_CommLink.m_softwareSpec.productSpecMap.TVal.Values)
            {
                if (productSpec.productID.TVal == orderData.Product.ID)
                {
                    moduleList =  s_CommLink.GetModuleSpecList(productSpec.productID.TVal);
                    foreach (Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs moduleSpec in moduleList.TVal.Values)
                    {                        
                        module = new Lic_PackageAttribs.Lic_ModuleInfoAttribs();
                        module.moduleID.TVal = moduleSpec.moduleID.TVal;
                        if(orderData.OrderStatus.Equals(Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msTrial))
                        {
                            module.moduleValue.TVal = moduleSpec.moduleTrialLicense.TVal;
                            module.moduleAppInstance.TVal = 1;
                        }       
                        module.moduleExpirationDate.TVal = orderData.ExpirationDate.Value;                
                        module.contractNumber.TVal = orderData.OrderNumber;
                        module.moduleState.TVal = orderData.OrderStatus;
                        orderData.Product.ModuleList.TVal.Add(module);
                    }
                }
            }
        }

        //transaction type
        //need to separate into two functions
        //1) create license node
        //2) update group db entity with new product
        private TreeNode CreateLicenseNode(LicenseServerProperty licProperties)
        {
            //any type that is not standard needs to be based off of standard
            CreateLicenseEntity(licProperties);
            TreeNode node = new TreeNode(licProperties.Name);
            node.Name = licProperties.Name;            
            node.ImageIndex = GetDetailTreeViewIconIndex("License");
            node.SelectedImageIndex = node.ImageIndex;
            node.StateImageIndex = 1;
            node.Tag = licProperties;
            node.Nodes.Add(new VirtualTreeNode());
            return node;
        }

        private void CreateLicenseEntity(LicenseServerProperty licProperties)
        {
            //Create new license file from CustomerProperty id and next available swID
            Service<ICreator>.Use((client) =>
            {
                //need function to copy standard for backup and disaster recovery                
                Lic_PackageAttribs licInfo = new Lic_PackageAttribs();
                //update licinfo with information from standard license
                //backup - copy standard and create new orders
                //disaster recovery - copy standard and create new orders
                //test dev - don't need to do anything, enforce rules on creating new test dev products
                //test/dev rules :
                //1) module value <= standard module value
                //2) can't add test/dev product that does not exist in standard license
                licInfo.licLicenseInfoAttribs.TVal = licProperties.LicInfo;
                //clear tokens if any for derived license types - F,DR,TD
                if(licInfo.licLicenseInfoAttribs.TVal.licVerificationAttribs.TVal.validationTokenList.TVal.Count > 0)
                    licInfo.licLicenseInfoAttribs.TVal.licVerificationAttribs.TVal.validationTokenList.TVal.Clear();
                //clear products except for failover                
                licInfo.licLicenseInfoAttribs.TVal.productList.TVal.Clear();
                Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs token = new Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs();
                token.tokenType.EVal = Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttLicenseCode;
                licInfo.licLicenseInfoAttribs.TVal.licVerificationAttribs.TVal.validationTokenList.TVal.Add(token);
                LicenseTable licRecord = new LicenseTable();
                licRecord.GroupID = (int)licProperties.GroupID;
                licRecord.DestinationID = (int)licProperties.DestID;
                licRecord.SCRnumber = (int)licProperties.CustID;
                licRecord.LicenseType = (Byte)licProperties.LicType;
                licRecord.LicenseName = licProperties.Name;
                licRecord.LicenseComments = licProperties.Comments;
                //need to add any validation tokens into licInfo.Stream
                licRecord.LicenseInfo = licInfo.Stream;
                client.CreateLicense(licRecord);
            });
        }
        //PopulateFailoverLicense - updates db license
        //FailoverAddOrder - updates order license and populates license product object
        //FailoverUpdateOrder - ""
        //FailoverDeleteOrder - ""
        //PopulateDisasterRecoveryLicense - 
        //DisasterRecoverAddOrder-
        //DisasterRecoveryUpdateOrder-
        //DisasterRecoveryDeleteOrder-
        //PopulateTestDevLicense
        //TestDevAddOrder-
        //TestDevUpdateOrder-
        //TestDevDeleteOrder-
        void AddExistingOrder(LicenseServerProperty licData, List<string> orders)
        {
            //get std license
            //get test dev license
            //for each order selected in orders
            Service<ICreator>.Use((client) => 
            {
                string licenseBase = string.Format("{0:x4}-{1:x3}-{2:x4}-P01", licData.CustID, licData.DestID, licData.GroupID);
                LicenseTable stdLicRec = client.GetLicenseByName(licenseBase, false);
                Lic_PackageAttribs stdLicPackage = new Lic_PackageAttribs();
                stdLicPackage.Stream = stdLicRec.LicenseInfo;
                string[] orderNumbers;
                LicenseTable licRec = client.GetLicenseByName(licData.Name, false);
                Lic_PackageAttribs licPackage = new Lic_PackageAttribs();
                licPackage.Stream = licRec.LicenseInfo;
                OrderTable orderRec, storedRec;
                string replaceString = "";
                if (licData.LicType == Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltDisasterRecovery)
                    replaceString = "D";
                else
                    replaceString = "T";
                foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs product in stdLicPackage.licLicenseInfoAttribs.TVal.productList.TVal)
                {
                    //std order
                    orderNumbers = Lic_LicenseInfoAttribsHelper.GetContractNumbers(product);
                    foreach (string orderNum in orderNumbers)
                    {
                        //sub-orders                         
                        if (orders.Contains(orderNum.Replace(replaceString, "P")))
                        {
                            //get standard order
                            orderRec = client.GetOrderByOrderName(orderNum);
                            //update std order with td order
                            orderRec.LicenseID = licRec.ID;
                            orderRec.OrderNumber = string.Format("{0}-{1}", licData.Name, orderRec.OrderIndex);
                            orderRec.Description = "";                                            
                            if(licData.LicType == Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltDisasterRecovery) 
                                orderRec.ExpirationDate = null;
                            else
                                orderRec.ExpirationDate = DateTime.Now.AddDays(30);
                            if (licData.LicType == Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltTestDev)
                            {
                                product.productID.TVal = s_CommLink.GetTestDevProductSpec(product.productID.TVal).productID;
                                orderRec.ProductName = s_CommLink.GetProductName(product.productID.TVal);
                            }
                            client.CreateOrder(orderRec);
                            storedRec = client.GetOrderByOrderName(string.Format("{0}-{1}", licData.Name, orderRec.OrderIndex));
                            CreateTransaction(TransactionType.Order,
                                              licData.Name,
                                              string.Format("Added {0} Product License - {1}", orderRec.ProductName, Enum.GetName(typeof(Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState), orderRec.OrderState)),
                                              orderRec.OrderNumber);
                            //convert product into test/dev product for test/dev key
                            //Don't include add-on orders
                            client.UpdateSubLicenseTransactionsByOrder(orderRec.ID, storedRec.ID);
                            foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs module in product.moduleList.TVal)
                            {
                                if (module.contractNumber.TVal.Equals(orderNum))
                                {
                                    module.contractNumber.TVal = orderRec.OrderNumber;
                                    //Disaster Recovery has no expiration dates.                                  
                                    module.moduleExpirationDate.TVal = (licData.LicType == Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltDisasterRecovery) ? new DateTime(1900, 1, 1) : orderRec.ExpirationDate.Value;
                                }
                            }
                            licPackage.licLicenseInfoAttribs.TVal.productList.TVal.Add(product);
                        }
                        else
                        {
                            orderRec = client.GetOrderByOrderName(orderNum);
                            if (orderRec.OrderState == (byte)Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msAddOn)
                            {
                                List<Lic_PackageAttribs.Lic_ModuleInfoAttribs> removeList = new List<Lic_PackageAttribs.Lic_ModuleInfoAttribs>();
                                foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs module in product.moduleList.TVal)
                                {
                                    if (module.contractNumber.TVal.Equals(orderNum))
                                    {
                                        removeList.Add(module);
                                    }
                                }
                                foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs removeModule in removeList)
                                {
                                    product.moduleList.TVal.Remove(removeModule);
                                }
                            }
                        }
                    }
                }
                licRec.LicenseInfo = licPackage.Stream;
                client.UpdateLicense(licRec);
            });
        }

        //if existing replace old order
        void UpdateExistingOrder(LicenseServerProperty licData)
        {
            //only needed for disaster recovery and test dev
            string licenseBase = string.Format("{0:x4}-{1:x3}-{2:x4}-P01", licData.CustID, licData.DestID, licData.GroupID);
            Service<ICreator>.Use((client) => 
            {                
                LicenseTable stdLicRec = client.GetLicenseByName(licenseBase, false);
                Lic_PackageAttribs stdLicPackage = new Lic_PackageAttribs();
                stdLicPackage.Stream = stdLicRec.LicenseInfo;                
                LicenseTable licRec = client.GetLicenseByName(licData.Name, false);
                Lic_PackageAttribs licPackage = new Lic_PackageAttribs();
                licPackage.Stream = licRec.LicenseInfo;
                OrderTable orderRec, storedRec;
                uint productID;
                string replaceString = "";
                if (licData.LicType == Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltDisasterRecovery)
                    replaceString = "D";
                else
                    replaceString = "T";
                string[] orderNumbers;
                foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs stdProduct in stdLicPackage.licLicenseInfoAttribs.TVal.productList.TVal)
                {
                    orderNumbers = Lic_LicenseInfoAttribsHelper.GetContractNumbers(stdProduct);
                    foreach (string orderNum in orderNumbers)
                    {
                        //stdLicense Order
                        orderRec = client.GetOrderByOrderName(orderNum);
                        //avoid orders that are add-ons
                        if (orderRec.OrderState != (byte)Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msAddOn)
                        {
                            //subLicense Order
                            storedRec = client.GetOrderByOrderName(string.Format("{0}-{1}", licData.Name, orderRec.OrderIndex));
                            //add any transactions from std order to derived order
                            if (storedRec != null)
                            {   //stored order exists, update - only update state
                                storedRec.OrderState = orderRec.OrderState;
                                client.UpdateSubLicenseTransactionsByOrder(orderRec.ID, storedRec.ID);
                                foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs stdModule in stdProduct.moduleList.TVal)
                                {
                                    //need to update module in licPackage with values from stdModule
                                    if (stdModule.contractNumber.TVal.Equals(orderNum))
                                    {
                                        foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs product in licPackage.licLicenseInfoAttribs.TVal.productList.TVal)
                                        {
                                            productID = (replaceString.Equals("T")) ? s_CommLink.GetTestDevProductSpec(stdProduct.productID.TVal).productID.TVal : stdProduct.productID.TVal;
                                            if (product.productID.TVal.Equals(productID))
                                            {
                                                foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs module in product.moduleList.TVal)
                                                {
                                                    if (module.contractNumber.TVal.Equals(orderNum.Replace("P", replaceString)) &&
                                                        module.moduleID.TVal.Equals(stdModule.moduleID.TVal))
                                                    {
                                                        module.moduleState.TVal = stdModule.moduleState.TVal;
                                                        module.moduleValue.TVal = stdModule.moduleValue.TVal;
                                                        module.moduleAppInstance.TVal = stdModule.moduleAppInstance.TVal;
                                                        //Disaster Recovery has no expiration dates.
                                                        module.moduleExpirationDate.TVal = (licData.LicType == Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltDisasterRecovery) ? new DateTime(1900, 1, 1) : storedRec.ExpirationDate.Value;                                                   
                                                        break;
                                                    }
                                                }
                                                break;
                                            }
                                        }
                                    }
                                }
                                client.UpdateOrder(storedRec);
                            }
                        }
                    }
                }
                licRec.LicenseInfo = licPackage.Stream;
                client.UpdateLicense(licRec);
            });
        }

        void PopulateFailoverLicense(LicenseServerProperty licData)
        {
            string licenseBase = string.Format("{0:x4}-{1:x3}-{2:x4}-P01", licData.CustID, licData.DestID, licData.GroupID);
            Service<ICreator>.Use((client) =>
            {
                LicenseTable licRec = client.GetLicenseByName(licenseBase, false);
                Lic_PackageAttribs licPackage = new Lic_PackageAttribs();
                licPackage.Stream = licRec.LicenseInfo;
                licPackage.licLicenseInfoAttribs.TVal.softwareLicType.TVal = licData.LicType;
                licPackage.licLicenseInfoAttribs.TVal.softwareLicTypeIndex.TVal = licData.LicInfo.softwareLicTypeIndex.TVal;
                licPackage.licLicenseInfoAttribs.TVal.licVerificationAttribs.TVal.validationTokenList.TVal.Clear();
                string[] orderNumbers;
                int licenseID = client.GetLicenseByName(licData.Name, false).ID;
                foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs product in licPackage.licLicenseInfoAttribs.TVal.productList.TVal)
                {
                    orderNumbers = Lic_LicenseInfoAttribsHelper.GetContractNumbers(product);
                    foreach (string orderNum in orderNumbers)
                    {
                        OrderTable orderRec = client.GetOrderByOrderName(orderNum);
                        //if order with same license name and index
                        orderRec.LicenseID = licenseID;
                        orderRec.OrderIndex = client.GetNextOrderIndex(licData.Name);
                        orderRec.OrderNumber = string.Format("{0}-{1}", licData.Name, orderRec.OrderIndex);
                        if (orderRec.ParentOrderNumber != null)
                            orderRec.ParentOrderNumber = orderRec.ParentOrderNumber.Replace("P", "F");
                        foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs module in product.moduleList.TVal)
                        {
                            if (module.contractNumber.TVal.Equals(orderNum))
                                module.contractNumber.TVal = orderRec.OrderNumber;
                        }
                        client.CreateOrder(orderRec);
                    }
                }
                licRec = client.GetLicenseByName(licData.Name, false);
                licRec.LicenseInfo = licPackage.Stream;
                client.UpdateLicense(licRec);
            });
        }

        //called on a save license for standard
        private void FailoverUpdateOrder(LicenseServerProperty licData)
        {
            Service<ICreator>.Use((client) => 
            {
                string licenseBase = string.Format("{0:x4}-{1:x3}-{2:x4}-P01", licData.CustID, licData.DestID, licData.GroupID);
                LicenseTable stdLicRec = client.GetLicenseByName(licenseBase, false);
                Lic_PackageAttribs licPackage = new Lic_PackageAttribs();
                licPackage.Stream = stdLicRec.LicenseInfo;
                string[] orderNumbers;
                LicenseTable failOverLicenseRec = client.GetLicenseByName(licData.Name, false);
                OrderTable orderRec, storedRec;
                //add or update any orders in std
                foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs product in licPackage.licLicenseInfoAttribs.TVal.productList.TVal)
                {
                    orderNumbers = Lic_LicenseInfoAttribsHelper.GetContractNumbers(product);
                    foreach (string orderNum in orderNumbers)
                    {
                        orderRec = client.GetOrderByOrderName(orderNum);
                        storedRec = client.GetOrderByOrderName(string.Format("{0}-{1}", licData.Name, orderRec.OrderIndex));
                        if (storedRec != null)
                        {   //stored order exists, update
                            storedRec.OrderState = orderRec.OrderState;
                            storedRec.ProductVersion = orderRec.ProductVersion;
                            storedRec.ExpirationDate = orderRec.ExpirationDate;
                            storedRec.Description = orderRec.Description;
                            client.UpdateSubLicenseTransactionsByOrder(orderRec.ID, storedRec.ID);
                            foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs module in product.moduleList.TVal)
                            {
                                if (module.contractNumber.TVal.Equals(orderNum))
                                    module.contractNumber.TVal = storedRec.OrderNumber;
                            }
                            client.UpdateOrder(storedRec);
                        }
                        else
                        {   //storedRec order doesn't exist, create                            
                            orderRec.LicenseID = failOverLicenseRec.ID;
                            orderRec.OrderNumber = string.Format("{0}-{1}", licData.Name, orderRec.OrderIndex);
                            if (orderRec.ParentOrderNumber != null)
                                orderRec.ParentOrderNumber = orderRec.ParentOrderNumber.Replace("P", "F");
                            foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs module in product.moduleList.TVal)
                            {
                                if (module.contractNumber.TVal.Equals(orderNum))
                                    module.contractNumber.TVal = orderRec.OrderNumber;
                            }
                            client.CreateOrder(orderRec);
                            storedRec = client.GetOrderByOrderName(string.Format("{0}-{1}", licData.Name, orderRec.OrderIndex));
                            client.UpdateSubLicenseTransactionsByOrder(orderRec.ID, storedRec.ID);
                        }
                    }
                }
                //Delete any orders that don't exist in std license
                //foreach()
                //{
                //    DeleteOrderNode();
                //}
                //Delete any orders that don't exist in std license

                //don't clear validation tokens from failover
                Lic_PackageAttribs failoverLicPackage = new Lic_PackageAttribs();
                failoverLicPackage.Stream = failOverLicenseRec.LicenseInfo;
                failoverLicPackage.licLicenseInfoAttribs.TVal.productList.TVal = licPackage.licLicenseInfoAttribs.TVal.productList.TVal;
                failOverLicenseRec.LicenseInfo = failoverLicPackage.Stream;
                client.UpdateLicense(failOverLicenseRec);
            });
        }

        
        private bool DeleteOrderNode(TreeNode node, bool bVerify)
        {
            bool bDeleteOrder = true;
            TreeNode productNode;
            ProductLicenseProperty orderData = node.Tag as ProductLicenseProperty;
            if (bVerify)
            {
                bDeleteOrder = false;
                DialogResult result = MessageBox.Show(string.Format("Please confirm deletion of order: {0}.", orderData.OrderNumber), "Delete Order Confirmation",
                                      MessageBoxButtons.OKCancel, MessageBoxIcon.Exclamation);
                if (result == DialogResult.OK)
                    bDeleteOrder = true;       
            }
            if (bDeleteOrder)
            {
                if (orderData.ParentOrderNumber != null)
                    productNode = node.Parent.Parent;
                else
                    productNode = node.Parent;

                Lic_PackageAttribs.Lic_ProductInfoAttribs storedProduct = DeleteOrderEntity(node.Tag as ProductLicenseProperty);
                productNode.Nodes.Remove(node);
                if (productNode.Nodes.Count == 0)
                    DeleteProductNode(productNode);
                else
                {
                    productNode.Tag = new ProductProperty(storedProduct);
                    //now set to either product node or perm order node
                    if (orderData.ParentOrderNumber != null)
                        LoadDetailListView(productNode.Nodes.Find(orderData.ParentOrderNumber, true).FirstOrDefault().Tag as ProductLicenseProperty);
                    else
                        LoadDetailListView(productNode.Tag as ProductProperty);
                    //reload detaillistview
                }
                return true;
            }
            return false;
        }

        //remove order entity from DB, remove any modules with order number and update license
        private Lic_PackageAttribs.Lic_ProductInfoAttribs DeleteOrderEntity(ProductLicenseProperty orderData)
        {
            Lic_PackageAttribs.Lic_ProductInfoAttribs storedProduct = new Lic_PackageAttribs.Lic_ProductInfoAttribs();
            CreateTransaction(TransactionType.Order,
                              orderData,
                              string.Format("Removed {0} Product License - {1}", orderData.ProductName, orderData.OrderStatus.ToString()),
                              orderData.OrderNumber);
            Service<ICreator>.Use((client) =>
            {
                OrderTable storedOrder = client.GetOrderByOrderName(orderData.OrderNumber);
                //if order is standard, reuse order
                if (storedOrder.OrderNumber.Contains("P"))
                {
                    //deactivate order instead of deleting to continue order # sequence.
                    storedOrder.LicenseID = -1;
                    client.UpdateOrder(storedOrder);
                }
                else
                    client.DeleteOrder(storedOrder);
              
            });

            Service<ICreator>.Use((client) =>
            {
                LicenseTable storedLicense = client.GetLicenseByName(orderData.LicenseName, false);
                Lic_PackageAttribs licPackage = new Lic_PackageAttribs();
                licPackage.Stream = storedLicense.LicenseInfo;
                foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs product in licPackage.licLicenseInfoAttribs.TVal.productList.TVal)
                {
                    if (product.productID.TVal.Equals(orderData.Product.ID))
                    {
                        //decrement appinstance for product when removing an order
                        product.productAppInstance.TVal--;
                        Lic_PackageAttribs.Lic_ProductInfoAttribs.Lic_ModuleInfoAttribsList moduleList = Lic_LicenseInfoAttribsHelper.GetModuleList_ByContractNumber(product, orderData.OrderNumber);
                        foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs mod in moduleList.TVal)
                        {
                            product.moduleList.TVal.Remove(mod);
                        }
                        storedProduct = product;
                        break;
                    }
                }
                storedLicense.LicenseInfo = licPackage.Stream;
                client.UpdateLicense(storedLicense);
            });
            return storedProduct;
        }

        //transaction type
        //means updating licenseinfo and removing product node
        private void DeleteProductNode(TreeNode node)
        {
            DeleteProductEntity(node.Parent.Name, node.Name);
            node.Parent.Nodes.Remove(node);
        }

        private void DeleteProductEntity(string licenseName, string productName)
        {
            uint selectedProductID = (uint)s_CommLink.GetProductID(productName);
            Service<ICreator>.Use((client) => 
            {
                LicenseTable licRec = client.GetLicenseByName(licenseName, false);
                Lic_PackageAttribs licPackage = new Lic_PackageAttribs();
                licPackage.Stream = licRec.LicenseInfo;
                foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs product in licPackage.licLicenseInfoAttribs.TVal.productList.TVal)
                {
                    if (product.productID.TVal.Equals(selectedProductID))
                    {
                        licPackage.licLicenseInfoAttribs.TVal.productList.TVal.Remove(product);
                        break;
                    }
                }
                licRec.LicenseInfo = licPackage.Stream;
                client.UpdateLicense(licRec);
            });
        }
        //done by right clicking node 
        private bool DeleteLicenseNode(TreeNode selectedLicenseNode)
        {
            LicenseServerProperty selectedLicense = selectedLicenseNode.Tag as LicenseServerProperty;
            DialogResult result = MessageBox.Show(string.Format("Please confirm deletion of license: {0}.", selectedLicense.Name), "Delete License Confirmation",
                                      MessageBoxButtons.OKCancel, MessageBoxIcon.Exclamation);
            if (result == DialogResult.OK)
            {
                //verify delete is allowed.            
                if (selectedLicense.IsStandardLicenseType)
                {
                    int derivedCount = 0;
                    Service<ICreator>.Use((client) =>
                    {
                        derivedCount = client.GetDerivedLicenseCount(selectedLicense.CustID, selectedLicense.DestID, selectedLicense.GroupID, selectedLicense.LicType);
                    });
                    if (derivedCount > 0)
                    {
                        MessageBox.Show("Please delete any derived licenses before removing standard licenses.", "Warning",
                             MessageBoxButtons.OK, MessageBoxIcon.Warning);
                        return false;
                    }
                }
                //if delete standard make sure there any derived licenses - failover, DR, test dev
                //delete orders also
                foreach (TreeNode node in selectedLicenseNode.Nodes)
                {
                    if (node.Name != "virtual")
                    {
                        foreach (TreeNode orderNode in node.Nodes)
                        {
                            if (node.Nodes.Count > 0)
                            {
                                foreach (TreeNode addOnNode in orderNode.Nodes)
                                {
                                    DeleteOrderNode(addOnNode, false);
                                }
                            }
                            DeleteOrderNode(orderNode, false);
                        }
                    }
                }
                DeleteLicenseEntity((selectedLicenseNode.Tag as LicenseServerProperty).Name);
                selectedLicenseNode.Parent.Text = string.Format("{0} ({1})", selectedLicenseNode.Parent.Name, selectedLicenseNode.Parent.Nodes.Count - 1);
                selectedLicenseNode.Parent.Nodes.Remove(selectedLicenseNode);
                return true;
            }
            return false;
        }

        private void DeleteLicenseEntity(string licenseName)
        {
            Service<ICreator>.Use((client) => 
            {
                LicenseTable licRecord = client.GetLicenseByName(licenseName, false);
                List<TokenTable> tokens = client.GetTokensByLicenseName(licRecord.LicenseName);
                foreach (TokenTable token in tokens)
                {
                    if (token.TokenType == (byte)Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID)
                    {
                        token.LicenseID = -1;
                        client.UpdateToken(token);
                    }
                    else
                    {
                        client.DeleteToken(token);
                    }
                }                
                client.DeleteLicense(licRecord);
            });
        }

        private bool DeleteCustomerNode(TreeNode selectedCustNode)
        {
            CustomerProperty selectedCustomer = selectedCustNode.Tag as CustomerProperty;
            DialogResult result = MessageBox.Show(string.Format("Please confirm deletion of customer: {0}.", selectedCustomer.Name), "Delete License Confirmation",
                          MessageBoxButtons.OKCancel, MessageBoxIcon.Exclamation);
            if (result == DialogResult.OK)
            {
                DeleteCustomerEntity(selectedCustomer.Id);
                DetailTreeView.Nodes.Remove(selectedCustNode);
                return true;
            }
            return false;
        }

        private void DeleteCustomerEntity(int CustID)
        {
            Service<ICreator>.Use((client) => 
            {
                CustomerTable custRec = client.GetCustomer(CustID.ToString(), false);
                client.DeleteCustomer(custRec);
            });
        }

        private void LoadDetailListView(Object item)
        {
            DetailListView.BeginUpdate();
            PopulateDetailListViewColumns(item);
            PopulateDetailListView(item);
            _lvManager.AutoResizeColumns(DetailListView);
            DetailListView.EndUpdate();
        }
        /// <summary>
        /// Populates the ListView column headers based upon the selected TreeNode of the License view.
        /// </summary>
        public void PopulateDetailListViewColumns(Object selectedObject)
        {
            DetailListView.Columns.Clear();
            if (selectedObject is LicenseServerProperty)
            {
                DetailListView.Columns.Add("Type");
                DetailListView.Columns.Add("Value");
            }
            else if (selectedObject is ProductProperty)
            {
                DetailListView.Columns.Add("Name");
                DetailListView.Columns.Add("Value");
                DetailListView.Columns.Add("AppInstance");
            }
            else
            {
                DetailListView.Columns.Add("Name");
                DetailListView.Columns.Add("Value");
                DetailListView.Columns.Add("Available");
            }
            _lvManager.ResetListViewColumnSorter(DetailListView);
        }

        public void PopulateDetailListView(Object selectedObject)
        {
            if (selectedObject is LicenseServerProperty)
                PopulateDetailListView(selectedObject as LicenseServerProperty);
            else if (selectedObject is ProductProperty)
                PopulateDetailListView(selectedObject as ProductProperty);
            else if(selectedObject is ProductLicenseProperty)
                PopulateDetailListView(selectedObject as ProductLicenseProperty);
        }

        public void PopulateDetailListView(LicenseServerProperty licenseData)
        {
            DetailListView.Items.Clear();
            Image tokenImage = ListViewImageList.Images[GetListViewIconIndex("VALIDATIONTOKENS")];
            ImageToolStripLabel.Size = new Size(tokenImage.Width, tokenImage.Height);
            ImageToolStripLabel.Image = tokenImage;
            DetailListViewToolStripLabel.Text = "Validation Tokens";
            //enable/disable buttons
            dlvAddToolStripButton.Visible = true;
            dlvRemoveToolStripButton.Enabled = false;
            dlvEditToolStripButton.Enabled = false;

            ListViewItem lvItem;        
            foreach (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs properties in licenseData.LicInfo.licVerificationAttribs.TVal.validationTokenList.TVal)
            {
                lvItem = new ListViewItem();
                lvItem.Tag = new ValidationProperty(properties);
                lvItem.Name = lvItem.Text = properties.tokenType.TVal.ToString();          
                lvItem.ImageIndex = GetListViewIconIndex(lvItem.Text);
                lvItem.SubItems.Add(properties.tokenValue.TVal);
                DetailListView.Items.Add(lvItem);
            }            
        }

        public void PopulateDetailListView(ProductProperty productData)
        {
            DetailListView.Items.Clear();
            Image productImage = DetailTreeViewImageList.Images[GetDetailTreeViewIconIndex(GetProductBaseName(productData.Name))];
            ImageToolStripLabel.Size = new Size(productImage.Width, productImage.Height);
            ImageToolStripLabel.Image = productImage;
            DetailListViewToolStripLabel.Text = string.Format("{0} Modules", productData.Name);
            dlvAddToolStripButton.Visible = false;
            dlvRemoveToolStripButton.Enabled = false;
            dlvEditToolStripButton.Enabled = false;
            foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs properties in productData.ModuleList.TVal)
            {
                if (!(properties.moduleAppInstance.TVal.Equals(0) && properties.moduleValue.TVal.Equals(0)))
                {
                    ListViewItem lvItem = CreateModuleListViewItem(properties, productData.ID, true);
                    if (DetailListView.Items.Find(lvItem.Name, false).Count() == 0)
                        DetailListView.Items.Add(lvItem);
                }
            }
        }

        private void PopulateDetailListView(ProductLicenseProperty orderData)
        {
            DetailListView.Items.Clear();
            //test-dev image missing
            Image productImage = DetailTreeViewImageList.Images[GetDetailTreeViewIconIndex(GetProductBaseName(orderData.Product.Name))];
            ImageToolStripLabel.Size = new Size(productImage.Width, productImage.Height);
            ImageToolStripLabel.Image = productImage;
            DetailListViewToolStripLabel.Text = string.Format("{0} Modules", orderData.Product.Name);
            dlvAddToolStripButton.Visible = false;
            dlvRemoveToolStripButton.Enabled = false;
            dlvEditToolStripButton.Enabled = false;
            //need to set orderdata read-only attributes
            foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs properties in orderData.Product.ModuleList.TVal)
            {
                if (properties.contractNumber.TVal.Equals(orderData.OrderNumber))
                {
                    ListViewItem lvItem = CreateModuleListViewItem(properties, orderData.Product.ID, false);
                    if(DetailListView.Items.Find(lvItem.Name, false).Count() == 0)
                        DetailListView.Items.Add(lvItem);
                }
            }
        }

        public ListViewItem CreateModuleListViewItem(Lic_PackageAttribs.Lic_ModuleInfoAttribs module, uint productID, bool bTotal)
        {
            ListViewItem lvItem = DetailListView.Items.Find(s_CommLink.GetModuleName(productID, module.moduleID),false).FirstOrDefault();            
            if (lvItem == null)
            {
                lvItem = new ListViewItem();
                lvItem.Tag = new ModuleProperty(module);
                lvItem.Name = lvItem.Text = s_CommLink.GetModuleName(productID, module.moduleID);
                lvItem.ImageIndex = GetListViewIconIndex("MODULE");
                lvItem.SubItems.Add(module.moduleValue.TVal.ToString());
                if (DetailTreeView.SelectedNode.Tag is ProductLicenseProperty)
                {
                    TreeNode node;
                    ProductLicenseProperty orderData = DetailTreeView.SelectedNode.Tag as ProductLicenseProperty;
                    //trial - available is always 0
                    //perm, addon - total value is shared between these
                    string availableUnits = "Unlimited";
                    int unlimitedValue = s_CommLink.GetUnlimitedModuleValue(productID, module.moduleID.TVal);
                    if (orderData.OrderStatus == Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msTrial)
                    {
                        if(unlimitedValue > 0)
                            availableUnits = "0";
                    }
                    else
                    {
                        //totalValue = perm + any addons
                        uint totalValue = 0;
                        //perm order
                        node = DetailTreeView.SelectedNode;
                        if (orderData.ParentOrderNumber != null)
                            node = DetailTreeView.SelectedNode.Parent;
                        orderData = node.Tag as ProductLicenseProperty;
                        foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs storedModule in orderData.Product.Product.moduleList.TVal)
                        {
                            if (storedModule.moduleID.TVal.Equals(module.moduleID.TVal))
                            {
                                totalValue = storedModule.moduleValue.TVal;
                                break;
                            }
                        }
                        if (node.Nodes.Count > 0)
                        {
                            foreach (TreeNode addonNode in node.Nodes)
                            {
                                orderData = addonNode.Tag as ProductLicenseProperty;
                                foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs storedModule in orderData.Product.Product.moduleList.TVal)
                                {
                                    if (storedModule.moduleID.TVal.Equals(module.moduleID.TVal))
                                    {
                                        totalValue += storedModule.moduleValue.TVal;
                                        break;
                                    }
                                }
                            }
                        }
                        if (unlimitedValue > 0)
                        {
                            int units = (s_CommLink.GetUnlimitedModuleValue(productID, module.moduleID.TVal) - (int)totalValue);
                            if (units < 0)
                                units = 0;
                            availableUnits = units.ToString();
                        }
                    }
                    lvItem.SubItems.Add(availableUnits);
                }
                else
                    lvItem.SubItems.Add(module.moduleAppInstance.TVal.ToString());
                if (!bTotal)
                {                
                    if (module.moduleExpirationDate.TVal.CompareTo(new DateTime(1900, 1, 1)) != 0)
                    {
                        if (module.moduleExpirationDate.TVal.CompareTo(DateTime.Now) < 0)
                            lvItem.ForeColor = System.Drawing.Color.Red;
                    }
                    else
                    {
                        if(module.moduleState.TVal == Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msPerm)
                            lvItem.ForeColor = System.Drawing.Color.SteelBlue;
                        //permanent - blue
                    }
                }
                if (module.moduleValue.TVal.Equals(0) && module.moduleAppInstance.TVal.Equals(0))
                {
                    lvItem.ForeColor = System.Drawing.Color.Gray;
                    lvItem.Font = new System.Drawing.Font(lvItem.Font, System.Drawing.FontStyle.Italic);
                }
            }
            else
            {
                if (bTotal)
                {
                    //this value is still referenced to module
                    int modValue = Int32.Parse(lvItem.SubItems[1].Text);
                    int modAppInstance = Int32.Parse(lvItem.SubItems[2].Text);
                    //remove expiration date column
                    lvItem.SubItems.RemoveAt(1);
                    //remove contract column
                    lvItem.SubItems.RemoveAt(1);
                    lvItem.SubItems.Add((modValue + module.moduleValue.TVal).ToString());
                    lvItem.SubItems.Add((modAppInstance + module.moduleAppInstance.TVal).ToString());
                }
            }
            return lvItem;
        }

        /// <summary>
        /// Requires node to be the customer node.
        /// </summary>
        private void SetCustomerLicenseState(TreeNode node)
        {
            TreeNode customerNode = null;         
            if (node.Tag is CustomerProperty)            
                customerNode = node;
            else if(node.Tag is LicenseServerProperty)
                customerNode = node.Parent;
            else if(node.Tag is ProductProperty)
                customerNode = node.Parent.Parent;
            else if(node.Tag is ProductLicenseProperty)
            {
                if((node.Tag as ProductLicenseProperty).ParentOrderNumber != null)
                    customerNode = node.Parent.Parent.Parent.Parent;
                else
                    customerNode = node.Parent.Parent.Parent;                        
            }
            LicenseServerProperty selectedLicense;

            IList<string> updatedLicenses = null;
            IList<string> modifiedLicenses = null;
            Service<ICreator>.Use((client) =>
            {
                updatedLicenses = client.GetUpdatedLicensesByCustomer(customerNode.Name);
                modifiedLicenses = client.GetModifiedLicensesByCustomer(customerNode.Name);
            });
            foreach (TreeNode licNode in customerNode.Nodes)
            {
                if (licNode.Text != "")
                {
                    selectedLicense = licNode.Tag as LicenseServerProperty;
                    licNode.ImageIndex = licNode.SelectedImageIndex = GetDetailTreeViewIconIndex("License");
                    licNode.ToolTipText = "";
                    if (modifiedLicenses != null)
                    {
                        if (modifiedLicenses.Contains(selectedLicense.Name))
                        {
                            licNode.ImageIndex = licNode.SelectedImageIndex = GetDetailTreeViewIconIndex("LicenseUpdate");
                            licNode.ToolTipText = "Changes have been made to license. Generate license packet.";
                        }
                    }
                    if (!selectedLicense.IsStandardLicenseType)
                    {
                        //check to see if there are pending changes
                        if (updatedLicenses != null)
                        {
                            if (!updatedLicenses.Contains(selectedLicense.Name))
                            {
                                licNode.ImageIndex = licNode.SelectedImageIndex = GetDetailTreeViewIconIndex("LicenseResync");
                                licNode.ToolTipText = "Standard license updates are available. Resync to apply changes.";
                            }
                        }
                    }
                    if (!(selectedLicense).IsEnabled) //disable key, not enough tokens
                    {
                        licNode.ImageIndex = licNode.SelectedImageIndex = GetDetailTreeViewIconIndex("LicenseDisabled");
                        licNode.ToolTipText = "Not enough validation tokens are set. Add a new validation token to enable license.";
                    }
                }
            }
        }
        private void LoadCustomerNode(TreeNode node)
        {
            CustomerTable customer = null;
            TreeNode licenseNode;
            Service<ICreator>.Use((client) => 
            {
                customer = client.GetCustomer(node.Name, true);
            });
                //need to filter licenses if there is any text in the searchbox
            if (customer != null)
            {
                foreach (var license in customer.LicenseTables.OrderBy(d => d.DestinationID).ThenBy(g => g.GroupID).ThenBy(l => l.LicenseType))
                {
                    if (license.LicenseName.ToLower().Contains(m_searchString) || (customer.SCRname.ToLower().Contains(m_searchString)))
                    {
                        if (!node.Nodes.ContainsKey(license.LicenseName))
                        {
                            //set text to modified mode if license has transactions with no packet id
                            licenseNode = new TreeNode(license.LicenseName);
                            licenseNode.Name = licenseNode.Text;
                            licenseNode.Tag = new LicenseServerProperty(license, m_Permissions);                                  
                            licenseNode.Nodes.Add(new VirtualTreeNode());
                            node.Nodes.Add(licenseNode);
                            if (m_searchString.Length > 0)
                                licenseNode.Parent.Expand();
                        }
                        else
                        {
                            licenseNode = node.Nodes[node.Nodes.IndexOfKey(license.LicenseName)];
                            licenseNode.Tag = new LicenseServerProperty(license, m_Permissions);
                        }
                    }
                }
            }
        }
        //current node selected is a product node
        private void LoadProductNode(TreeNode node)
        {
            List<OrderTable> orders = null;
            ProductProperty selectedProduct = node.Tag as ProductProperty;
            Service<ICreator>.Use((client) => 
            {
                orders = client.GetOrdersEqualsProduct(node.Parent.Name, selectedProduct.Name);
            });
            TreeNode permOrderNode;
            foreach (OrderTable order in orders)
            {       
                TreeNode orderNode = null;
                if (!node.Nodes.ContainsKey(order.OrderNumber))
                {
                    orderNode = new TreeNode(order.OrderNumber);
                    orderNode.Name = order.OrderNumber;
                    orderNode.Tag = new ProductLicenseProperty(order, selectedProduct, m_Permissions);
                    if (order.ParentOrderNumber != null)
                    {
                        orderNode.ImageIndex = GetDetailTreeViewIconIndex("ADDONORDER");
                        orderNode.SelectedImageIndex = orderNode.ImageIndex;
                        permOrderNode = node.Nodes.Find(order.ParentOrderNumber, false).FirstOrDefault();
                        if (permOrderNode != null)
                        {
                            if (!permOrderNode.Nodes.ContainsKey(orderNode.Name))
                                permOrderNode.Nodes.Add(orderNode);
                        }
                    }
                    else
                    {
                        orderNode.ImageIndex = GetDetailTreeViewIconIndex("ORDER");
                        orderNode.SelectedImageIndex = orderNode.ImageIndex;
                        node.Nodes.Add(orderNode);
                    }
                }
                else
                {
                    orderNode = node.Nodes.Find(order.OrderNumber, true).First();
                    orderNode.Tag = new ProductLicenseProperty(order, selectedProduct, m_Permissions);
                }
            }
            if (node.Nodes.Count > 0)
            {
                foreach (TreeNode orderNode in node.Nodes)
                {
                    if (orders.Find(c=> c.OrderNumber.Equals((orderNode.Name))) == null)
                        node.Nodes.Remove(orderNode);
                }
            }        
        }

        private void LoadLicenseNode(TreeNode licenseNode)
        {
            string productName;
            LicenseTable license = null;
            Service<ICreator>.Use((client) => 
            {
                license = client.GetLicenseByName(licenseNode.Name, false);
            });
            Lic_PackageAttribs licPackage = new Lic_PackageAttribs();
            if (license != null)
                licPackage.Stream = license.LicenseInfo;
            foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs product in licPackage.licLicenseInfoAttribs.TVal.productList.TVal)
            {   //selectednode = licenseName
                productName = s_CommLink.GetProductName(product.productID);
                TreeNode productNode = null;
                if (!licenseNode.Nodes.ContainsKey(productName))
                {
                    productNode = new TreeNode(productName);
                    productNode.Name = productName;
                    productNode.ImageIndex = GetDetailTreeViewIconIndex(GetProductBaseName(productName));
                    productNode.SelectedImageIndex = productNode.ImageIndex;
                    //load products also
                    productNode.Tag = new ProductProperty(product);
                    bool bAdded = false;
                    if (licenseNode.Nodes.Count > 0)
                    {
                        foreach (TreeNode node in licenseNode.Nodes)
                        {
                            if (string.Compare(node.Text, productNode.Text) > 0)
                            {
                                licenseNode.Nodes.Insert(node.Index, productNode);
                                bAdded = true;
                                break;
                            }
                        }
                    }
                    if (!bAdded)
                        licenseNode.Nodes.Add(productNode);
                }
                else
                {   //update
                    productNode = licenseNode.Nodes.Find(productName, false).First();
                    productNode.Tag = new ProductProperty(product);
                }
                LoadProductNode(productNode);
            }
        }

        private void LoadLicenseView(IEnumerable<CustomerTable> customers)
        {
            Cursor storedCursor = this.Cursor;
            this.Cursor = Cursors.WaitCursor;
            DetailTreeView.BeginUpdate();
            DetailTreeView.Nodes.Clear();
            //return a list of licenses, create parent nodes first then add license nodes
            if (customers != null)
            {
                if (customers.Count() > 0)
                {
                    foreach (var cust in customers)
                    {
                        TreeNode custNode = new TreeNode(string.Format("{0} ({1})", cust.SCRname, cust.LicenseTables.Count()));
                        custNode.Name = cust.SCRname;
                        custNode.ImageIndex = GetDetailTreeViewIconIndex("Customer");
                        custNode.SelectedImageIndex = custNode.ImageIndex;
                        if (!custNode.IsExpanded)
                            custNode.Nodes.Add(new VirtualTreeNode());
                        custNode.Tag = new CustomerProperty(cust);
                        DetailTreeView.Nodes.Add(custNode);
                    }
                    SubSplitContainer.Panel2Collapsed = false;
                    DetailTreeView.Enabled = true;
                }
                else
                {
                    TreeNode emptyNode = new TreeNode("Search Results Empty");
                    emptyNode.ImageIndex = emptyNode.SelectedImageIndex = GetDetailTreeViewIconIndex("Empty");
                    DetailTreeView.Nodes.Add(emptyNode);
                    SubSplitContainer.Panel2Collapsed = true;
                    DetailTreeView.Enabled = false;
                }
            }
            //selected node should be first node.
            if(DetailTreeView.Nodes.Count > 0 && customers.Count() > 0)
                DetailTreeView.SelectedNode = DetailTreeView.Nodes[0];
            else
                DetailPropertyGrid.SelectedObject = null;
            DetailTreeView.EndUpdate();
            EnableDetailListView(true);
            this.Cursor = storedCursor;
        }

        private void LoadPacketListView()
        {
            IList<LicenseTable> licenses = null;
            Service<ICreator>.Use((client) => 
            {
                licenses = client.GetAllLicenses(m_searchString, true);
            });
            //Need list view manager
            PacketListView.BeginUpdate();
            PacketListView.Items.Clear();
            foreach (var license in licenses)
            {
                if (license.PacketTables != null)
                {
                    //group by customer
                    bool b = false;
                    ListViewGroup licenseGroup = null;
                    string custName;
                    foreach (var packet in license.PacketTables)
                    {
                        //custName = packet.PacketName.Remove(packet.PacketName.IndexOf("_"));
                        
                        //only create new group if it doesn't already exist
                        //foreach (ListViewGroup group in PacketListView.Groups)
                        //{
                        //    if (group.Header.Equals(custName))
                        //    {
                        //        licenseGroup = group;
                        //        break;
                        //    }
                        //}
                        //if (licenseGroup == null)
                        //{
                        //    licenseGroup = new ListViewGroup();
                        //    PacketListView.Groups.Add(licenseGroup);
                        //    licenseGroup.Header = custName;
                        //}
                        ListViewItem item = new ListViewItem();
                        //item.Group = licenseGroup;
                        item.Text = (packet.IsVerified ? "verified" : "unverified");
                        item.Checked = packet.IsVerified;
                        item.SubItems.Add(packet.PacketName);
                        item.SubItems.Add(packet.DateCreated.ToLocalTime().ToShortDateString());
                        item.SubItems.Add(packet.PacketVerificationCode);
                        item.SubItems.Add(packet.PacketComments);
                        item.SubItems.Add(packet.UserName);
                        this.PacketListView.Items.Add(item);
                        if (!b)
                        {
                            item.BackColor = Color.AliceBlue;
                            b = true;
                        }
                        else
                            b = false;
                    }
                }
            }
            _lvManager.AutoResizeColumns(PacketListView);
            PacketListView.EndUpdate();
        }

        private void LoadReportListView()
        {
            IList<LicenseTable> licenses = null;
            Service<ICreator>.Use((client) => 
            {
                licenses = client.GetLicencesByConditions((MainTreeView.SelectedNode.Tag as ReportProperty).Conditions);
            });
            ReportListView.BeginUpdate();
            ReportListView.Items.Clear();
            foreach (LicenseTable filteredLicense in licenses)
            {
                ListViewItem newItem = new ListViewItem();
                newItem.Text = filteredLicense.LicenseName;
                newItem.SubItems.Add(filteredLicense.LicenseComments);
                ReportListView.Items.Add(newItem);
            }
            _lvManager.AutoResizeColumns(ReportListView);
            ReportListView.EndUpdate();
        }

        private void LoadHardwareKeyListView()
        {
            IList<TokenTable> tokens = null;
            Service<ICreator>.Use((client) => 
            {
                tokens = client.GetAllTokens(m_searchString, Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID);

                //Need list view manager

                bool b = false;
                HardwareKeyListView.BeginUpdate();
                HardwareKeyListView.Items.Clear();
                LicenseTable license;
                CustomerTable cust;
                string licenseName;
                foreach (var token in tokens)
                {                
                    ListViewItem item = new ListViewItem();
                    cust = client.GetCustomer(token.CustID.ToString(),false);                                        
                    //customer name
                    item.Text = cust.SCRname;
                    //key value
                    item.SubItems.Add(token.TokenValue);
                    //license name
                    license = client.GetLicenseByID(token.LicenseID, false);
                    licenseName = "";
                    if (license != null)
                        licenseName = license.LicenseName;
                    item.SubItems.Add(licenseName);
                    this.HardwareKeyListView.Items.Add(item);
                    if (!b)
                    {
                        item.BackColor = Color.AliceBlue;
                        b = true;
                    }
                    else
                        b = false;
                }
            });

            _lvManager.AutoResizeColumns(HardwareKeyListView);
            HardwareKeyListView.EndUpdate();
        }

        private void LoadTransactionItems(string licenseName)
        {
            IList<PacketTable> packets = null;
            IList<TransactionTable> transactions = null;
            TransactionListView.BeginUpdate();
            TransactionListView.Items.Clear();
            TransactionListView.Groups.Clear();
            Service<ICreator>.Use((client) => 
            {
                //now two steps, given a license name, get all transactions for a license and sort by packet
                packets = client.GetPacketsByLicenseName(licenseName).OrderByDescending(c => c.DateCreated).ToList();
                //Need list view manager
                transactions = client.GetNewTransactionsByLicenseName(licenseName).OrderByDescending(c => c.taDateCreated).ToList();
                if (transactions.Count > 0)
                {
                    ListViewGroup group = new ListViewGroup();
                    group.Header = "Current Transactions";
                    TransactionListView.Groups.Add(group); 
                    foreach (var transaction in transactions)
                    {
                        ListViewItem item = new ListViewItem();
                        item.Text = transaction.taDateCreated.ToLocalTime().ToShortDateString();
                        item.SubItems.Add(transaction.taDescription);
                        item.SubItems.Add(transaction.taUnits);
                        item.SubItems.Add(transaction.taUser);
                        item.Group = group;
                        TransactionListView.Items.Add(item);
                    }                 
                }
                foreach (var packet in packets)
                {
                    transactions = client.GetTransactionsByPacketID(packet.ID).OrderByDescending(c => c.taDateCreated).ToList();;
                    if (transactions.Count > 0)
                    {
                        ListViewGroup group = new ListViewGroup();
                        group.Header = packet.PacketName;
                        TransactionListView.Groups.Add(group);
                        foreach (var transaction in transactions)
                        {
                            ListViewItem item = new ListViewItem();
                            item.Text = transaction.taDateCreated.ToLocalTime().ToShortDateString();
                            item.SubItems.Add(transaction.taDescription);
                            item.SubItems.Add(transaction.taUnits);
                            item.SubItems.Add(transaction.taUser);
                            item.Group = group;
                            TransactionListView.Items.Add(item);
                        }
                    }
                }
            });
            if (TransactionListView.ShowGroups)
                SetGroupTransactionListViewItemBackColor();
            else
                SetSingleTransactionListViewItemBackColor();
            _lvManager.AutoResizeColumns(TransactionListView);
            toolStripComboBox1.SelectedIndex = 0;
            TransactionListView.EndUpdate();
        }

        private void LoadPacketItems(string licenseName)
        {
            //given packet id display transactions or
            //given license display transactions
            IList<PacketTable> packets = null;
            Service<ICreator>.Use((client) => 
            {
                //now two steps, given a license name, get all transactions for a license and sort by packet
                packets = client.GetPacketsByLicenseName(licenseName);
            });
            //Need list view manager
            //LicensePacketListView.BeginUpdate();
            //LicensePacketListView.Items.Clear();
            storedPacketListView.Items.Clear();
            if (packets != null && packets.Count > 0)
            {
                bool b = false;
                foreach (var packet in packets)
                {
                    ListViewItem packetItem = new ListViewItem();
                    packetItem.Text = packet.PacketName;
                    packetItem.Tag = packet;
                    packetItem.ImageIndex = GetListViewIconIndex((packet.IsVerified ? "VERIFIED" : "UNVERIFIED"));
                    packetItem.StateImageIndex = packetItem.ImageIndex;
                    packetItem.SubItems.Add(packet.DateCreated.ToShortDateString());
                    packetItem.SubItems.Add(packet.ExpiredDate.ToShortDateString());
                    packetItem.SubItems.Add(packet.UserName);
                    packetItem.SubItems.Add(packet.VerifiedBy);
                    //LicensePacketListView.Items.Add(packetItem);                    
                    //if (!b)
                    //{
                    //    packetItem.BackColor = Color.AliceBlue;
                    //    b = true;
                    //}
                    //else
                    //    b = false;
                    //add to stored packet list
                    storedPacketListView.Items.Add(packetItem);//.Clone() as ListViewItem);
                }                
                storedPacketListView.Items[storedPacketListView.Items.Count - 1].Font = new Font(this.Font, FontStyle.Bold); 
            }
            toolStripComboBox1.SelectedIndex = 0;            
            //LicensePacketListView.EndUpdate();
            //needed outside of begin/end update to allow clearing of LicensePacketListView
            ShowPacketListView("");
        }
        //Transaction for License File 
        private void CreateTransaction(TransactionType type, string licenseName, string description, string value)
        {
            Service<ICreator>.Use((client) =>
            {
                LicenseTable selectedLicense = client.GetLicenseByName(licenseName, false);
                TransactionTable newTransaction = new TransactionTable();
                newTransaction.taLicenseID = selectedLicense.ID;
                newTransaction.taType = (Byte)type;
                newTransaction.taDateCreated = DateTime.UtcNow;
                newTransaction.taPacketID = null;
                newTransaction.taDescription = description;
                newTransaction.taUnits = value;
                newTransaction.taUser = WindowsIdentity.GetCurrent().Name;
                client.CreateTransaction(newTransaction);
            });
            SetCustomerLicenseState(DetailTreeView.SelectedNode);
        }

        //Transaction for Order
        private void CreateTransaction(TransactionType type, ProductLicenseProperty orderData, string description, string value)
        {
            Service<ICreator>.Use((client) =>
            {
                OrderTable selectedOrder = client.GetOrderByOrderName(orderData.OrderNumber);
                TransactionTable newTransaction = new TransactionTable();
                newTransaction.taLicenseID = selectedOrder.LicenseID;
                newTransaction.taType = (Byte)type;
                newTransaction.taDateCreated = DateTime.UtcNow;
                newTransaction.taPacketID = null;
                newTransaction.taDescription = description;
                newTransaction.taUnits = value;
                newTransaction.taUser = WindowsIdentity.GetCurrent().Name;
                newTransaction.taOrderID = selectedOrder.ID;
                client.CreateTransaction(newTransaction);
            });
            SetCustomerLicenseState(DetailTreeView.SelectedNode);
        }

        //Compare between current licenseinfo vs stored db licenseinfo
        //Difference creates a transaction and value is updated for stored db licenseinfo
        private void LicenseItemTransactions(ref Lic_PackageAttribs.AttribsMemberAttribsClass_Lic_LicenseInfoAttribs licInfo, ref LicenseServerProperty licItem)
        {
            //License Attribs 
            //1) LicType 2) GroupID 3) DestID 4) CustID 5) ValidationToken list
            string licenseName = Lic_LicenseInfoAttribsHelper.GetDisplayLabel(licInfo);
            //Compare GroupID - READONLY
            licInfo.TVal.softwareGroupLicenseID.TVal = licItem.GroupID;
            //Compare CustID - READONLY
            licInfo.TVal.customerID.TVal = licItem.CustID;
            //Compare LicenseType - READONLY
            licInfo.TVal.softwareLicType.EVal = licItem.LicType;
            //Compare DestID - READONLY
            licInfo.TVal.destinationID.TVal = licItem.DestID;

            if (!licInfo.TVal.activationCurrent.TVal.Equals(licItem.ActivationCurrent))
            {
                CreateTransaction(TransactionType.License, licenseName, string.Format("Changed Activation Current from {0} to {1}", licInfo.TVal.activationCurrent.ToString(), licItem.ActivationCurrent.ToString()), licItem.ActivationCurrent.ToString());
                licInfo.TVal.activationCurrent.TVal = licItem.ActivationCurrent;
            }
            if (!licInfo.TVal.activationTotal.TVal.Equals(licItem.ActivationTotal))
            {
                CreateTransaction(TransactionType.License, licenseName, string.Format("Changed Activation Total from {0} to {1}", licInfo.TVal.activationTotal.ToString(), licItem.ActivationTotal.ToString()), licItem.ActivationTotal.ToString());
                licInfo.TVal.activationTotal.TVal = licItem.ActivationTotal;
            }
            if (!licInfo.TVal.activationAmountInDays.TVal.Equals(licItem.ActivationAmtInDays))
            {
                CreateTransaction(TransactionType.License, licenseName, string.Format("Changed Activation Amount in Days from {0} to {1}", licInfo.TVal.activationAmountInDays.ToString(), licItem.ActivationAmtInDays.ToString()), licItem.ActivationAmtInDays.ToString());
                licInfo.TVal.activationAmountInDays.TVal = licItem.ActivationAmtInDays;
            }
            bool found;
            if (licInfo.TVal.licVerificationAttribs.TVal.validationTokenList.TVal.Count > licItem.LicInfo.licVerificationAttribs.TVal.validationTokenList.TVal.Count)
            {
                //validatation token was removed
                List<Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs> removeList = new List<Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs>();
                foreach (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs storedToken in licInfo.TVal.licVerificationAttribs.TVal.validationTokenList.TVal)
                {   //search for any tokens in stored tokenlist that are not in new token list
                    found = false;
                    foreach (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs validationToken in licItem.LicInfo.licVerificationAttribs.TVal.validationTokenList.TVal)
                    {
                        if (ValidationTokenCompare(validationToken, storedToken))
                        {
                            found = true;
                            break;
                        }
                    }
                    if (!found)
                        removeList.Add(storedToken);
                }
                foreach (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs removeToken in removeList)
                {
                    licInfo.TVal.licVerificationAttribs.TVal.validationTokenList.TVal.Remove(removeToken);
                }
            }
            else if (licInfo.TVal.licVerificationAttribs.TVal.validationTokenList.TVal.Count < licItem.LicInfo.licVerificationAttribs.TVal.validationTokenList.TVal.Count)
            {   //validation token was added
                foreach (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs vToken in licItem.LicInfo.licVerificationAttribs.TVal.validationTokenList.TVal)
                {
                    found = false;
                    foreach (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs tokenInfo in licInfo.TVal.licVerificationAttribs.TVal.validationTokenList.TVal)
                    {
                        if (ValidationTokenCompare(vToken, tokenInfo))
                        {
                            found = true;
                            break;
                        }
                    }
                    //new token type was created
                    if (!found)
                    {
                        //add token since not found
                        Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs token = new Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs();
                        token = vToken;
                        licInfo.TVal.licVerificationAttribs.TVal.validationTokenList.TVal.Add(token);                        
                    }
                }
            }
            else
            {   //modified
                foreach (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs vToken in licItem.LicInfo.licVerificationAttribs.TVal.validationTokenList.TVal)
                {
                    foreach (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs storedToken in licInfo.TVal.licVerificationAttribs.TVal.validationTokenList.TVal)
                    {
                        //if tokentype is the same, then the value must be different
                        if (storedToken.tokenType.TVal.Equals(vToken.tokenType.TVal))
                        {   //only modify if value has changed
                            if (!storedToken.tokenValue.TVal.Equals(vToken.tokenValue.TVal))
                            {
                                CreateTransaction(TransactionType.License, licenseName, string.Format("Changed Token Value from {0} to {1}", storedToken.tokenValue.TVal, vToken.tokenValue.TVal), vToken.tokenValue.TVal);
                                storedToken.tokenValue.TVal = vToken.tokenValue.TVal;
                                break;
                            }
                        }
                    }
                }
            }
        }

        private bool ValidationTokenCompare(Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs token, Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs storedToken)
        {
            bool found = false;
            if (storedToken.tokenType.TVal.Equals(token.tokenType.TVal))
                if (storedToken.tokenValue.TVal.Equals(token.tokenValue.TVal))
                    found = true;                            
            return found;
        }

        private bool OrderTransactions(ref OrderTable storedOrder, ProductLicenseProperty selectedOrder)
        {
            //1) expdate, status, description
            bool bChanged = false;
            if (!storedOrder.OrderState.Equals((byte)selectedOrder.OrderStatus))
            {
                CreateTransaction(TransactionType.Order, 
                                  selectedOrder,
                                  string.Format("{0}: Product License Status from {1} to {2}", selectedOrder.OrderNumber, ((Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState)storedOrder.OrderState).ToString(), selectedOrder.OrderStatus.ToString()), 
                                  selectedOrder.OrderStatus.ToString());
                storedOrder.OrderState = (byte)selectedOrder.OrderStatus;
                bChanged = true;
            }
            //product version extends to all orders of the same product 
            if (!storedOrder.ProductVersion.Equals(selectedOrder.ProductVersion.ToString()))
            {
                CreateTransaction(TransactionType.Order,
                                  selectedOrder,
                                  string.Format("{0}: Product Version from {1} to {2}", selectedOrder.OrderNumber, storedOrder.ProductVersion, selectedOrder.ProductVersion),
                                  selectedOrder.ProductVersion.ToString());
                storedOrder.ProductVersion = selectedOrder.ProductVersion.ToString();
                Service<ICreator>.Use((client) => 
                {
                    IList<OrderTable> productOrders = client.GetOrdersEqualsProduct(selectedOrder.LicenseName, selectedOrder.ProductName);
                    foreach (OrderTable order in productOrders)
                    {
                        order.ProductVersion = selectedOrder.ProductVersion.ToString();
                        client.UpdateOrder(order);
                    }
                });
            }
            if(!storedOrder.ExpirationDate.ToString().Equals(selectedOrder.ExpirationDate.ToString()))
            {                
                string storedValue = "None", value = "None";
                if (selectedOrder.ExpirationDate.HasValue)
                    value = selectedOrder.ExpirationDate.Value.ToShortDateString();
                if(storedOrder.ExpirationDate.HasValue)
                    storedValue = storedOrder.ExpirationDate.Value.ToShortDateString();
                CreateTransaction(TransactionType.Order, 
                                  selectedOrder,
                                  string.Format("{0}: Product License Expiration Date {1} to {2}", selectedOrder.OrderNumber,storedValue, value), 
                                  value);
                bChanged = true;
                storedOrder.ExpirationDate = selectedOrder.ExpirationDate;
            }
            if (!storedOrder.Description.Equals(selectedOrder.Description))
            {
                storedOrder.Description = selectedOrder.Description;
            }
            return bChanged;
        }

        private Lic_PackageAttribs.Lic_ProductInfoAttribs AddOnProductTransactions(ref Lic_PackageAttribs.AttribsMemberAttribsClass_Lic_LicenseInfoAttribs licInfo, ProductLicenseProperty selectedOrder)
        {
            List<Lic_PackageAttribs.Lic_ModuleInfoAttribs> removeList = new List<Lic_PackageAttribs.Lic_ModuleInfoAttribs>();
            Lic_PackageAttribs.Lic_ProductInfoAttribs updatedProduct = null;
            foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs product in licInfo.TVal.productList.TVal)
            {
                if (product.productID.TVal.Equals(selectedOrder.Product.ID))
                {
                    foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs module in product.moduleList.TVal)
                    {
                        if(module.contractNumber.TVal.Equals(selectedOrder.OrderNumber))
                            removeList.Add(module);
                    }
                    foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs removeModule in removeList)
                    {
                        product.moduleList.TVal.Remove(removeModule);
                    }
                    //add remove module values to perm modules
                    foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs module in product.moduleList.TVal)
                    {
                        if (module.contractNumber.TVal.Equals(selectedOrder.ParentOrderNumber))
                        {
                            foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs removeModule in removeList)
                            {
                                if (removeModule.moduleID.TVal.Equals(module.moduleID.TVal))
                                {
                                    if (removeModule.moduleValue.TVal > 0)
                                    {
                                        module.moduleValue.TVal += removeModule.moduleValue.TVal;
                                        if (module.moduleAppInstance.TVal == 0) module.moduleAppInstance.TVal = 1;
                                    }                                    
                                    break;
                                }
                            }
                        }
                    }
                    updatedProduct = product;
                    break; 
                }
            }
            return updatedProduct;
        }

        //Problem : remove/add conflicts with edit because module compare thinks edits are new/remove modules}
        private void ProductItemTransactions(ref Lic_PackageAttribs.AttribsMemberAttribsClass_Lic_LicenseInfoAttribs licInfo, ProductLicenseProperty orderData, bool bOrderChanged)
        {
            //iterate over all products
            string moduleName;
            string licenseName = Lic_LicenseInfoAttribsHelper.GetDisplayLabel(licInfo);
            ProductProperty selectedProduct = orderData.Product;
            //product nodes under licenses                          
            foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs prodInfo in licInfo.TVal.productList.TVal)
            {
                //find selectedProduct in stored db productList
                if (prodInfo.productID.TVal.Equals(selectedProduct.ID))
                {
                    if (!prodInfo.productAppInstance.TVal.Equals(selectedProduct.AppInstance))
                    {
                        if (!bOrderChanged)
                            CreateTransaction(TransactionType.Product, orderData, string.Format("{0}:{1} - Application Instance from {1} to {2}", orderData.OrderNumber, selectedProduct.Name, prodInfo.productAppInstance.SVal, selectedProduct.AppInstance.ToString()), selectedProduct.AppInstance.ToString());
                        prodInfo.productAppInstance.TVal = selectedProduct.AppInstance;
                    }
                    prodInfo.product_Major.TVal = selectedProduct.Product.product_Major.TVal;
                    prodInfo.product_Minor.TVal = selectedProduct.Product.product_Minor.TVal;
                    prodInfo.product_SubMajor.TVal = selectedProduct.Product.product_SubMajor.TVal;
                    prodInfo.product_SubMinor.TVal = selectedProduct.Product.product_SubMinor.TVal;
                    ////Add, Edit, Remove Module list
                    //add new module for perm or add-on, trial->perm, perm->trial
                    foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs mod in selectedProduct.ModuleList.TVal)
                    {
                        moduleName = s_CommLink.GetModuleName(selectedProduct.ID, mod.moduleID.TVal);
                        foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs storedModule in prodInfo.moduleList.TVal)
                        {
                            if (storedModule.moduleID.TVal.Equals(mod.moduleID.TVal))
                            {   //matched new module to stored module
                                //value must be different since it was not found
                                //units
                                //contractNumbers do not change 
                                if (storedModule.contractNumber.TVal.Equals(mod.contractNumber.TVal))
                                {
                                    //if add on to perm -> add modules not equal
                                    //trial,perm
                                    if (!storedModule.moduleValue.TVal.Equals(mod.moduleValue.TVal))
                                    {
                                        if (!bOrderChanged)
                                            CreateTransaction(TransactionType.Product,
                                                              orderData, 
                                                              string.Format("{0}:({1}-{2}) - Value from {3} to {4}",orderData.OrderNumber,selectedProduct.Name, moduleName, storedModule.moduleValue.TVal.ToString(), mod.moduleValue.TVal.ToString()), 
                                                              mod.moduleValue.TVal.ToString());
                                        storedModule.moduleValue.TVal = mod.moduleValue.TVal;
                                    }
                                    //app instance
                                    if (!storedModule.moduleAppInstance.TVal.Equals(mod.moduleAppInstance.TVal))
                                    {
                                        if (!bOrderChanged)
                                            CreateTransaction(TransactionType.Product,
                                                              orderData, 
                                                              string.Format("{0}:({1}-{2}) - Application Instance from {3} to {4}",orderData.OrderNumber, selectedProduct.Name, moduleName, storedModule.moduleAppInstance.ToString(), mod.moduleAppInstance.ToString()), 
                                                              mod.moduleAppInstance.ToString());
                                        storedModule.moduleAppInstance.TVal = mod.moduleAppInstance.TVal;
                                    }
                                    //expiration date
                                    if (!storedModule.moduleExpirationDate.TVal.Equals(mod.moduleExpirationDate.TVal))
                                    {
                                        storedModule.moduleExpirationDate.TVal = mod.moduleExpirationDate.TVal;
                                    }     
                                    //expiration date
                                    if (!storedModule.moduleState.TVal.Equals(mod.moduleState.TVal))
                                    {
                                        storedModule.moduleState.TVal = mod.moduleState.TVal;
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }

        private void SearchCurrentView()
        {
            if (toolStripSearchBox.ForeColor == SystemColors.InactiveCaptionText)
                toolStripSearchBox.Text = "";
            m_searchString = toolStripSearchBox.Text;
            switch (MainTreeView.SelectedNode.Text)//shortCutToolStripComboBox.Text)
            {
                case "Licenses":
                    LoadDBCustomers(m_searchString, true);
                    break;
                case "Packets":
                    LoadPacketListView();
                    break;
                case "Hardware Keys":
                    LoadHardwareKeyListView();
                    break;
                default:
                    break;
            }                
        }
        #endregion

        #region Database Methods
        private void LoadDBCustomers(string searchString, bool loadOption)
        {
            IEnumerable<CustomerTable> data = null;
            using (BackgroundWorker worker = new BackgroundWorker())
            {
                //lambda expression for anonymous delegate 
                //data.Where(x => x.Beta < 100);
                //delegate(DataClass x)
                //{
                //    return x.Beta < 100;
                //});
                //worker.WorkerReportsProgress = true;
                worker.DoWork += ((sender, e) => data = GetDBCustomers(worker, searchString, loadOption));
                //worker.ProgressChanged += ((sender, e) => ReportChangedProgress(e.ProgressPercentage));
                //returned from work so this can access ui main thread
                worker.RunWorkerCompleted += ((sender, e) => OnDBCustomersRetrieved(data));
                worker.RunWorkerAsync();
            }
            //panel1.BackColor = Color.FromArgb(0, System.Drawing.Color.Transparent);
            //panel1.Visible = true;
            loadingCircle1.Visible = true;
            loadingCircle1.SpokeThickness = 6;
            loadingCircle1.InnerCircleRadius = 12;
            loadingCircle1.OuterCircleRadius = 13;
            loadingCircle1.NumberSpoke = 10;
            loadingCircle1.Active = true;
            DetailTreeView.Nodes.Clear();
        }
        //Only called on a backgroundworker 
        private IEnumerable<CustomerTable> GetDBCustomers(BackgroundWorker worker, string searchString, bool loadOption)
        {
            IEnumerable<CustomerTable> cust = null;
            Service<ICreator>.Use((client) => 
            {
                try
                {
                    cust = client.GetAllCustomers(searchString, loadOption);
                }
                catch (Exception e) { }
            });
            return cust;
        }

        private void OnDBCustomersRetrieved(IEnumerable<CustomerTable> data)
        {
            if (data != null)
            {
                LoadLicenseView(data);
            }
            loadingCircle1.Active = false;
            loadingCircle1.Visible = false;
            if (toolStripSearchBox.ForeColor == SystemColors.InactiveCaptionText)
                toolStripSearchBox.Text = "Search";
            //restore searchbox text
        }

        private void GeneratePacket(string packetName, string licenseName, DateTime expDate, string description, string outputPath)
        {
            using (BackgroundWorker worker = new BackgroundWorker())
            {
                worker.DoWork += ((sender, e) => GenerateDBPacket(packetName, licenseName, expDate, description, outputPath));
                worker.RunWorkerCompleted += ((sender, e) => OnGeneratedPacket(licenseName));
                worker.RunWorkerAsync();
            }
            loadingCircle1.Visible = true;
            loadingCircle1.SpokeThickness = 6;
            loadingCircle1.InnerCircleRadius = 12;
            loadingCircle1.OuterCircleRadius = 13;
            loadingCircle1.NumberSpoke = 10;
            loadingCircle1.Active = true;
        }

        private void GenerateDBPacket(string packetName, string licenseName, DateTime expDate, string description, string outputPath)
        {
            Service<ICreator>.Use((client) => 
            {
                Byte[] newByteArrayLicensePacket = null;
                string verificationCode = "";

                client.GenerateLicensePacket(packetName,
                                             licenseName,
                                             expDate,
                                             description,
                                             ref verificationCode,
                                             ref newByteArrayLicensePacket,
                                             WindowsIdentity.GetCurrent().Name);
                PacketTable storedPacket = client.GetPacketByVerificationCode(verificationCode);
                //retrieve all transactions for a given license that doesnt have a packet id
                IList<TransactionTable> transactionList = client.GetNewTransactionsByLicenseName(licenseName);
                foreach (TransactionTable recordedTransaction in transactionList)
                {
                    recordedTransaction.taPacketID = storedPacket.ID;
                    client.UpdateTransaction(recordedTransaction);
                }
                if (newByteArrayLicensePacket != null)
                    File.WriteAllBytes(string.Format("{0}\\{1}.{2}", outputPath, packetName, "packet"), newByteArrayLicensePacket);
            });
        }

        private void OnGeneratedPacket(string licenseName)
        {
            loadingCircle1.Active = false;
            loadingCircle1.Visible = false;
            LoadTransactionItems(licenseName);
            LoadPacketItems(licenseName);
            SetCustomerLicenseState(DetailTreeView.SelectedNode);
        }
        #endregion

        private void PropertyGridTabControl_TabIndexChanged(object sender, EventArgs e)
        {
            if (PropertyGridTabControl.SelectedTab == PacketTabPage)
            {
                //if (LicensePacketListView.SelectedItems.Count == 0)
                //    splitContainer1.Panel2Collapsed = true;
            }
        }

        private void splitContainer1_Panel2_Paint(object sender, PaintEventArgs e)
        {
            if (VisualStyleRenderer.IsSupported)
            {
                VisualStyleRenderer render = new VisualStyleRenderer(VisualStyleElement.ExplorerBar.NormalGroupHead.Normal);
                render.DrawBackground(e.Graphics, (sender as Panel).Bounds, (sender as Panel).ClientRectangle);
            }
        }

        private void CreateNewCustomerNode()
        {
            string custName;
            CustomerTable newCustRec = new CustomerTable();
            for (int count = 1; ; count++)
            {
                custName = string.Format("{0}{1}", "NewCustomer", count);
                if (!DetailTreeView.Nodes.ContainsKey(custName))
                {
                    newCustRec.SCRname = custName;
                    break;
                }
            }
            Service<ICreator>.Use((client) =>
            {
                client.CreateCustomer(newCustRec);
                //need to retrieve custRec again for updated scrnumber
                newCustRec = client.GetCustomer(custName, false);
            });
            TreeNode newNode = new TreeNode(string.Format("{0} ({1})", newCustRec.SCRname, 0));
            newNode.Name = newCustRec.SCRname;
            newNode.ImageIndex = GetDetailTreeViewIconIndex("Customer");
            newNode.SelectedImageIndex = newNode.ImageIndex;
            newNode.Tag = new CustomerProperty(newCustRec);
            DetailTreeView.Nodes.Add(newNode);
            DetailTreeView.SelectedNode = newNode;
        }

        private void newCustomerToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CreateNewCustomerNode();            
        }

        //    //currently on license file node
        private void newOrderToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AddOrder();
        }

        private void newCustMainToolStripBtn_Click(object sender, EventArgs e)
        {
            //createcustomerentity
            //createcustomernode
            CreateNewCustomerNode();
        }

        private void toggleViewMainToolStripBtn_CheckedChanged(object sender, EventArgs e)
        {
            if (toggleViewMainToolStripBtn.Checked)
                MainSplitContainer.Panel1Collapsed = true;
            else
                MainSplitContainer.Panel1Collapsed = false;
        }

        private void toolStripComboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (toolStripComboBox1.SelectedIndex)
            {
                case 0:
                    ShowPacketListView("");
                    break;
                case 1:
                    ShowPacketListView("VERIFIED");
                    break;
                case 2:
                    ShowPacketListView("UNVERIFIED");
                    break;
                default: break;                    
            }
        }

        private void addHardwareKeyToolStripButton_Click(object sender, EventArgs e)
        {
            using (LicenseInfoForm dlg = new LicenseInfoForm("Create New Hardware Key", ref s_CommLink))
            {
                if (dlg.ShowDialog(this, null) == DialogResult.OK)
                {
                    LoadHardwareKeyListView();
                }
            }
        }

        private void DetailListView_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            //product level            
            switch (DetailTreeView.SelectedNode.Level)
            {
                case 1:
                    dlvAddToolStripButton.Visible = true;
                    dlvEditToolStripButton.Enabled = e.IsSelected;
                    //need to disable remove if ttlicensecode is selected.
                    dlvRemoveToolStripButton.Enabled = e.IsSelected & (e.Item.Tag as ValidationProperty).IsAllowedRemoveToken;
                    break;
                case 2:
                    //initialize tooltip information for DetailListView - Modules
                    ModuleProperty selectedModule = e.Item.Tag as ModuleProperty;
                    //retrieve order data using information from selected item
                    string information = "", expirationDate = "";
                    ProductProperty product = DetailTreeView.SelectedNode.Tag as ProductProperty;
                    foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs module in product.ModuleList.TVal)
                    {
                        if (module.moduleID.TVal.Equals(selectedModule.ID))
                        {
                            if (module.moduleExpirationDate.TVal.Equals(new DateTime(1900, 1, 1)))
                                expirationDate = "None";
                            else
                                expirationDate = module.moduleExpirationDate.TVal.ToShortDateString();
                            information += string.Format("Order Number - {0}, Expiration Date - {1}\n", module.contractNumber.TVal, expirationDate);
                        }
                    }
                    e.Item.ToolTipText = information;
                    break;
                case 3:
                case 4:
                    //initialize buttons for DetailListView - Modules
                    dlvAddToolStripButton.Visible = false;
                    dlvEditToolStripButton.Enabled = e.IsSelected;
                    dlvRemoveToolStripButton.Enabled = e.IsSelected & (e.Item.Tag as ModuleProperty).IsAllowedRemoveModule;                                                                                            
                    break;
                default: break;
            }
        }

        private void currentTransactionToolStripButton_Click(object sender, EventArgs e)
        {

        }

        private void resyncToolStripMenuItem_Click(object sender, EventArgs e)
        {
            LicenseServerProperty selectedLicense;
            //only enable refresh for licenses that have pending changes available 
            Cursor storedCursor = this.Cursor;
            this.Cursor = Cursors.WaitCursor;
            selectedLicense = DetailTreeView.SelectedNode.Tag as LicenseServerProperty;
            ResyncSelectedLicense(selectedLicense);
            LoadCustomerNode(DetailTreeView.SelectedNode.Parent);
            LoadLicenseNode(DetailTreeView.SelectedNode);
            SetCustomerLicenseState(DetailTreeView.SelectedNode);
            this.Cursor = storedCursor;
        }

        //DetailTreeView selected node contains a LicenseServerProperty object
        private void existingOrderToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (ProductLicenseSelectionForm dlg = new ProductLicenseSelectionForm())
            {
                OrderSelectionData data = new OrderSelectionData(DetailTreeView.SelectedNode.Tag as LicenseServerProperty);                               
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                {
                    AddExistingOrder(data.SelectedLicense, data.OrderSelectionList);
                    DetailTreeView.BeginUpdate();
                    DetailTreeView.SelectedNode.Nodes.Clear();   
                    LoadLicenseNode(DetailTreeView.SelectedNode);
                    DetailTreeView.SelectedNode.Expand();
                    DetailTreeView.EndUpdate();
                    DetailPropertyGrid.SelectedObject = DetailTreeView.SelectedNode.Tag;
                }
            }
        }

        private void exitMainToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void ResyncSelectedLicense(LicenseServerProperty selectedLicense)
        {
            switch (selectedLicense.LicType)
            {
                case Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltFailover:
                    FailoverUpdateOrder(selectedLicense);
                    break;
                case Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltDisasterRecovery:
                case Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltTestDev:
                    UpdateExistingOrder(selectedLicense);
                    break;
                default:
                    break;
            }
        }

        private void addOrderMainToolStripDropDownBtn_Click(object sender, EventArgs e)
        {
            AddOrder();
        }

        private void newOrderMainToolStripMenuItem_Click(object sender, EventArgs e)
        {
            
        }

        private void existingOrderMainToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (ProductLicenseSelectionForm dlg = new ProductLicenseSelectionForm())
            {
                OrderSelectionData data = new OrderSelectionData(DetailTreeView.SelectedNode.Tag as LicenseServerProperty);
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                {
                    AddExistingOrder(data.SelectedLicense, data.OrderSelectionList);
                    DetailTreeView.BeginUpdate();
                    DetailTreeView.SelectedNode.Nodes.Clear();
                    LoadLicenseNode(DetailTreeView.SelectedNode);
                    DetailTreeView.SelectedNode.Expand();
                    DetailTreeView.EndUpdate();
                    DetailPropertyGrid.SelectedObject = DetailTreeView.SelectedNode.Tag;
                }
            }
        }

        private void addLicMainToolStripBtn_Click(object sender, EventArgs e)
        {
            
        }

        private void newFailoverMainToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CreateFailoverLicense();
        }

        private void newDisasterRecoveryMainToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CreateDRLicense();
        }

        private void newTestDevelopmentMainToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CreateTestDevLicense();
        }

        private void newStandardMainToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CreateStandardLicense();
        }

        private void AddOrder()
        {
            //show contract dialog
            int orderIndex = 0;
            Lic_PackageAttribs licPackage = new Lic_PackageAttribs();
            ProductLicenseProperty newOrder = new ProductLicenseProperty();
            string licenseName = DetailTreeView.SelectedNode.Name;
            Service<ICreator>.Use((client) =>
            {
                orderIndex = client.GetNextOrderIndex(licenseName);
            });
            newOrder.LicenseName = licenseName;
            newOrder.OrderIndex = orderIndex;
            newOrder.Permissions = m_Permissions;
            //license file already created
            //display settings form for product connections
            using (LicenseInfoForm dlg = new LicenseInfoForm("Create New Order", ref s_CommLink))
            {
                OrderDialogData data = new OrderDialogData(newOrder, DetailTreeView.SelectedNode.Tag as LicenseServerProperty);
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                {
                    //create orderNode
                    TreeNode orderNode = CreateOrderNode(data.OrderInfo);
                    TreeNode newProductNode = CreateProductNode(data.OrderInfo);
                    TreeNode productNode = DetailTreeView.SelectedNode.Nodes.Find(data.OrderInfo.Product.Name, false).FirstOrDefault();
                    if (productNode == null)
                    {
                        productNode = newProductNode;
                        //add node in alpha order
                        bool bAdded = false;
                        if (DetailTreeView.SelectedNode.Nodes.Count > 0)
                        {
                            foreach (TreeNode node in DetailTreeView.SelectedNode.Nodes)
                            {
                                if (string.Compare(node.Text, productNode.Text) > 0)
                                {
                                    DetailTreeView.SelectedNode.Nodes.Insert(node.Index, productNode);
                                    bAdded = true;
                                    break;
                                }
                            }
                        }
                        if (!bAdded)
                            DetailTreeView.SelectedNode.Nodes.Add(productNode);
                    }
                    else //attach to product tree
                    {
                        productNode.Tag = newProductNode.Tag;
                    }
                    if (data.OrderInfo.ParentOrderNumber == null)
                        productNode.Nodes.Add(orderNode);

                    else
                    {
                        //add to parent order node
                        TreeNode parentOrderNode = DetailTreeView.SelectedNode.Nodes.Find(data.OrderInfo.ParentOrderNumber, true).FirstOrDefault();
                        parentOrderNode.Nodes.Add(orderNode);
                    }
                    DetailTreeView.SelectedNode = orderNode;
                    DetailPropertyGrid.SelectedObject = orderNode.Tag;
                }
            }
        }

        private void createPacketMainToolStripBtn_Click(object sender, EventArgs e)
        {
            GenerateLicensePacket();
        }

        private void createPacketFileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            GenerateLicensePacket();
        }

        private void addLicMainToolStripBtn_ButtonClick(object sender, EventArgs e)
        {
            if (newStandardLicenseToolStripMenuItem.Enabled)
                CreateStandardLicense();
        }

        private void addLicMainToolStripBtn_DropDownOpening(object sender, EventArgs e)
        {
            //enable new licenses depending on selection
            EnableLicenseMainToolStripDropDownMenuItems();
        }

        private void addOrderMainToolStripDropDownBtn_ButtonClick(object sender, EventArgs e)
        {            
            if(newOrderMainToolStripMenuItem.Enabled)
                AddOrder();
        }

        private void addOrderMainToolStripDropDownBtn_DropDownOpening(object sender, EventArgs e)
        {
            EnableOrderMainToolStripDropDownMenuItems();
        }

        private void connectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ConnectServer();
        }

        private void CreatorForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            //Copy ServerConnection List to app settings
            if (this.m_ServerList.Count > 0)
            {
                Settings.Default.ServerList.Clear();
                Settings.Default.ServerList.AddRange(this.m_ServerList.ToArray());
            }

            Settings.Default.SelectedDirectory = m_selectedDirectory;

            // Save settings
            Settings.Default.Save();
        }

        private void DetailListView_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            // Do a hit test for the current mouse position
            ListViewHitTestInfo hitInfo = DetailListView.HitTest(e.Location);
                                
            // Test to see if the selected item and the hit test item are the same.
            if (DetailListView.SelectedItems.Count > 0)
            {
                ListViewItem item = DetailListView.SelectedItems[0];
                if (item.Equals(hitInfo.Item))
                {
                    //launch edit dialog
                    ShowEditDialog(DetailTreeView.SelectedNode.Tag, DetailListView.SelectedIndices);
                }
            }
        }

        private void DetailTreeView_KeyDown(object sender, KeyEventArgs e)
        {
            //if (DetailTreeView.SelectedNode != null)
            //{
            //    if (e.KeyCode == Keys.F5)
            //    {
            //        Cursor storedCursor = this.Cursor;
            //        this.Cursor = Cursors.WaitCursor;
            //        LicenseServerProperty selectedLicense = DetailTreeView.SelectedNode.Tag as LicenseServerProperty;
            //        ResyncSelectedLicense(selectedLicense);
            //        LoadCustomerNode(DetailTreeView.SelectedNode.Parent);
            //        LoadLicenseNode(DetailTreeView.SelectedNode);
            //        this.Cursor = storedCursor;
            //    }
            //}
        }

        private void dlvAddToolStripButton_Click(object sender, EventArgs e)
        {
            if (DetailTreeView.SelectedNode.Tag is LicenseServerProperty)
                CreateNewValidationToken(DetailTreeView.SelectedNode.Tag as LicenseServerProperty);
        }

        private void dlvRemoveToolStripButton_Click(object sender, EventArgs e)
        {
            if (DetailTreeView.SelectedNode.Tag is LicenseServerProperty)   //token remove
                dlvRemoveToken(DetailTreeView.SelectedNode.Tag as LicenseServerProperty);
            else                //module remove
                dlvRemoveModule(DetailTreeView.SelectedNode.Tag as ProductLicenseProperty);
        }

        private void dlvEditToolStripButton_Click(object sender, EventArgs e)
        {
            ShowEditDialog(DetailTreeView.SelectedNode.Tag, DetailListView.SelectedIndices);
        }

        private void expandToolStripMenuItem_Click(object sender, EventArgs e)
        {
            bool bAllow = true;
            if(DetailTreeView.SelectedNode.Tag is LicenseServerProperty)  //disallow expand
                bAllow = (DetailTreeView.SelectedNode.Tag as LicenseServerProperty).IsEnabled;
            if(bAllow)
                DetailTreeView.SelectedNode.Expand();
        }

        private void collapseToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DetailTreeView.SelectedNode.Collapse();
        }

        private void SearchToolStripLabel_Click(object sender, EventArgs e)
        {
            SearchCurrentView();
        }

        private void toolStripSearchBox_Enter(object sender, EventArgs e)
        {
            if (toolStripSearchBox.ForeColor == SystemColors.InactiveCaptionText)
            {
                toolStripSearchBox.Text = "";
                toolStripSearchBox.Font = new Font(this.Font, FontStyle.Regular);
                toolStripSearchBox.ForeColor = SystemColors.WindowText;
            }
        }

        private void toolStripSearchBox_Leave(object sender, EventArgs e)
        {
            if (toolStripSearchBox.Text.Length == 0)
            {
                toolStripSearchBox.Text = "Search";
                toolStripSearchBox.Font = new Font(this.Font, FontStyle.Italic);
                toolStripSearchBox.ForeColor = SystemColors.InactiveCaptionText;
            }
        }

        private void PropertyGridTabControl_Selecting(object sender, TabControlCancelEventArgs e)
        {
            if (PropertyGridTabControl.TabPages.Count > 0)
            {
                LoadSelectedTabPage(PropertyGridTabControl.SelectedTab);
            }
        }

        private void LoadSelectedTabPage(TabPage selectedTab)
        {
            if (!DetailSplitContainer.Panel2Collapsed)
            {
                if (selectedTab == PropertyGridTabPage)
                    LoadDetailListView(DetailTreeView.SelectedNode.Tag);
                else if (selectedTab == TransactionsTabPage)
                    LoadTransactionItems(m_CurrentLicenseName);
                else if (selectedTab == PacketTabPage)
                    LoadPacketItems(m_CurrentLicenseName);
            }
        }

        private void shortCutToolStripComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            //ignore top level root nodes
                //countToolStripStatusLabel.Text = "";
            if (shortCutToolStripComboBox.Text == "Licenses")
            {
                ResetMainToolStripMenu();
                MainToolStrip.Visible = true;
                SearchCurrentView();
                EnableDetailListView(true);
                MainSplitContainer.Panel2.Controls.Clear();
                //RestoreTreeState(DetailTreeView, m_TreeState);                    
                loadingCircle1.Parent = MainSplitContainer.Panel2;
                SubSplitContainer.Parent = MainSplitContainer.Panel2;
            }
            else if (shortCutToolStripComboBox.Text == "Packets")
            {
                //ResetMainToolStripMenu();
                MainToolStrip.Visible = false;
                SearchCurrentView();
                MainSplitContainer.Panel2.Controls.Clear();
                //panel1.Parent = MainSplitContainer.Panel2;
                loadingCircle1.Parent = MainSplitContainer.Panel2;
                PacketListView.Parent = MainSplitContainer.Panel2;

            }
            else if (shortCutToolStripComboBox.Text == "Hardware Keys")
            {
                //ResetMainToolStripMenu();
                MainToolStrip.Visible = false;
                SearchCurrentView();
                MainSplitContainer.Panel2.Controls.Clear();
                loadingCircle1.Parent = MainSplitContainer.Panel2;
                panel1.Parent = MainSplitContainer.Panel2;
            }
            else
            {
                if (shortCutToolStripComboBox.Text != "Reports")
                {
                    LoadReportListView();
                    MainSplitContainer.Panel2.Controls.Clear();
                    //panel1.Parent = MainSplitContainer.Panel2;
                    loadingCircle1.Parent = MainSplitContainer.Panel2;
                    ReportListView.Parent = MainSplitContainer.Panel2;
                }
            }
        }

        private void LicensePacketListView_MouseUp(object sender, MouseEventArgs e)
        {
            bool bShow = false;
            //needs to handle validation properties also
            if (e.Button == MouseButtons.Right)
            {
                ListViewItem item = LicensePacketListView.GetItemAt(e.X, e.Y);
                verifyToolStripMenuItem.Visible = true;
                updateToolStripMenuItem.Visible = false;
                verifyToolStripMenuItem.Enabled = false;
                bShow = true;
                if (m_Permissions.pt_version_pwd.Value)
                {
                    //if (item != null)
                    //{   //enable edit/delete menu items for item selected
                    //    if((item.Tag as PacketTable).VerifiedBy == null)
                    foreach (ListViewItem packetItem in LicensePacketListView.Items)
                    {
                        if (packetItem.StateImageIndex != 0)
                        {
                            verifyToolStripMenuItem.Enabled = true;
                            break;
                        }
                    }
                    //}
                }
                if (bShow)
                    PacketContextMenuStrip.Show(LicensePacketListView, e.X, e.Y);
            }
        }

        private void packetDescriptionRichTextBox_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {      
                verifyToolStripMenuItem.Visible = false;
                updateToolStripMenuItem.Visible = true;                
                PacketContextMenuStrip.Show(packetDescriptionRichTextBox, e.X, e.Y);                   
            }
        }

        private void updateToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //get selected packet item
            ListViewItem item = LicensePacketListView.SelectedItems[0];
            PacketTable selectedPacket = item.Tag as PacketTable;
            selectedPacket.PacketComments = packetDescriptionRichTextBox.Text;
            //get selected packettable object
            Service<ICreator>.Use((client) =>
            {
                client.UpdatePacket(selectedPacket);
            });
        }

        private void LicensePacketListView_ColumnClick(object sender, ColumnClickEventArgs e)
        {
            _lvManager.SetSortIndexColumn(LicensePacketListView.Handle, e.Column);
            // Perform the sort with these new sort options.
            LicensePacketListView.Sort();
        }


        //// Create a Font object for the node tags.
        //Font textFont = new Font("Helvetica", 8, FontStyle.Bold);


        //// Returns the bounds of the specified node, including the region 
        //// occupied by the node label and any node tag displayed.
        //private Rectangle NodeBounds(TreeNode node)
        //{
        //    // Set the return value to the normal node bounds.
        //    Rectangle bounds = node.Bounds;
        //    if (node.Text != null)
        //    {
        //        // Retrieve a Graphics object from the TreeView handle
        //        // and use it to calculate the display width of the tag.
        //        Graphics g = DetailTreeView.CreateGraphics();
        //        int textWidth = (int)g.MeasureString
        //            (node.Text.ToString(), textFont).Width + 6;

        //        // Adjust the node bounds using the calculated value.
        //        bounds.Offset(textWidth / 2, 0);
        //        bounds = Rectangle.Inflate(bounds, textWidth / 2, 0);
        //        g.Dispose();
        //    }
        //    return bounds;
        //}

        //private void DetailTreeView_DrawNode(object sender, DrawTreeNodeEventArgs e)
        //{
        //    // Draw the background and node text for a selected node.
        //    if ((e.State & TreeNodeStates.Selected) != 0)
        //    {
        //        // Draw the background of the selected node. The NodeBounds
        //        // method makes the highlight rectangle large enough to
        //        // include the text of a node tag, if one is present.
        //        e.Graphics.FillRectangle(Brushes.LightSkyBlue, NodeBounds(e.Node));

        //        // Retrieve the node font. If the node font has not been set,
        //        // use the TreeView font.
        //        Font nodeFont = e.Node.NodeFont;
        //        if (nodeFont == null) nodeFont = ((TreeView)sender).Font;

        //        // Draw the node text.
        //        e.Graphics.DrawString(e.Node.Text, nodeFont, Brushes.White, //e.Bounds);
        //            Rectangle.Inflate(e.Bounds, 3, 0));
        //    }

        //    // Use the default background and node text.
        //    else
        //    {
        //        e.DrawDefault = true;
        //    }

            // If a node tag is present, draw its string representation 
            // to the right of the label text.
            //if (e.Node.Tag != null)
            //{
            //    e.Graphics.DrawString(e.Node.Tag.ToString(), tagFont,
            //        Brushes.Brown, e.Bounds.Right + 2, e.Bounds.Top);
            //}

            // If the node has focus, draw the focus rectangle large, making
            // it large enough to include the text of the node tag, if present.
            //if ((e.State & TreeNodeStates.Focused) != 0)
            //{
            //    using (Pen focusPen = new Pen(Color.Black))
            //    {
            //        focusPen.DashStyle = System.Drawing.Drawing2D.DashStyle.Dot;
            //        Rectangle focusBounds = NodeBounds(e.Node);
            //        focusBounds.Size = new Size(focusBounds.Width - 1,
            //        focusBounds.Height - 1);
            //        e.Graphics.DrawRectangle(focusPen, focusBounds);
            //    }
            //}
     // }

        //private void tabControl1_DrawItem(object sender, DrawItemEventArgs e)
        //{
        //    Graphics g = e.Graphics;
        //    Brush _TextBrush;

        //    //' Get the item from the collection.
        //    TabPage _TabPage = tabControl1.TabPages[e.Index];

        //    //' Get the real bounds for the tab rectangle.
        //    Rectangle _TabBounds = tabControl1.GetTabRect(e.Index);

        //    if (e.State == DrawItemState.Selected)
        //    {
        //        //' Draw a different background color, and don't paint a focus rectangle.
        //        _TextBrush = new System.Drawing.SolidBrush(e.ForeColor);  //new SolidBrush(Color.Red);
        //        //g.FillRectangle(Brushes.Gray, e.Bounds);
        //    }
        //    else
        //    {
        //        _TextBrush = new System.Drawing.SolidBrush(e.ForeColor);
        //        e.DrawBackground();
        //    }


        //    //' Use our own font.
        //    Font _TabFont = new Font("Arial", 10, FontStyle.Bold, GraphicsUnit.Pixel);

        //    //' Draw string. Center the text.
        //    StringFormat _StringFlags = new StringFormat(StringFormatFlags.DirectionVertical);
        //    _StringFlags.Alignment = StringAlignment.Center;
        //    _StringFlags.LineAlignment = StringAlignment.Center;
        //    g.DrawString(_TabPage.Text, _TabFont, _TextBrush, _TabBounds, new StringFormat(_StringFlags));
        //}
    }
    // Implements the manual sorting of items by columns.
}
