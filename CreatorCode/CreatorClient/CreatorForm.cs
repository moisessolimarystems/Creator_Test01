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
using Solimar.Licensing.LicenseManagerWrapper;

/*
 * TODO : - Apply rules to license types - std,bkup,dr,testdev
 *        - Create custom filters to retrieve lists.
 *        - Add license comment attribute to license table
 *        - Improve look of interface
 *        - setup concurrency
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
        private ListView storageListView;
        public static CommunicationLink s_CommLink;
        private PermissionsTable m_Permissions;
        private string m_selectedDirectory;
        private List<String> m_ServerList;
        private string m_searchString = "";
        private string m_CurrentLicenseName = "";
        //used by hardware key view
        private string _selectedHardwareKeyCustomer = "";
        private string _selectedLicenseCustomer = "";
        private DateTime _currentExpirationDate;
        private ViewState _currentView;

        #region Properties

        public string CurrentServer
        {
            get { return m_ServerList.First(); }           
        }

        public DateTime CurrentExpirationDate
        {
            get { return new DateTime(DateTime.Today.Year, DateTime.Today.Month, DateTime.Today.Day, 10, 0, 0); }
        }

        #endregion

        public CreatorForm()
        {
            InitializeComponent();
            _currentView = ViewState.Customer;
            m_ServerList = new List<String>();
            s_CommLink = new CommunicationLink();
            storageListView = new ListView();
            _lvManager = new ListViewMgr();
            _lvManager.SetListViewColumnSorter(DetailListView);
            _lvManager.SetListViewColumnSorter(TransactionListView);
            _lvManager.SetListViewColumnSorter(CustomersListView);
            _lvManager.SetListViewColumnSorter(LicensePacketListView);
            _lvManager.SetListViewColumnSorter(HardwareKeyListView);            
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
            foreach (ToolStripItem tsm in addProductLicenseMainToolStripDropDownBtn.DropDownItems)
            {
                tsm.Enabled = false;
            }          
        }

        private void EnableCustomerContextMenu(bool value)
        {
            expandToolStripMenuItem.Visible = !DetailTreeView.SelectedNode.IsExpanded;
            expandToolStripMenuItem.Enabled = (DetailTreeView.SelectedNode.Nodes.Count == 0) ? false : true;
            collapseToolStripMenuItem.Visible = DetailTreeView.SelectedNode.IsExpanded;
            addToolStripMenuItem.Visible = value;
            newCustomerToolStripMenuItem.Visible = value;
            newLicenseToolStripMenuItem.Visible = value;
            newProductLicenseToolStripMenuItem.Visible = !value;
            lcmToolStripSeparator1.Visible = value; 
            existingProductLicenseToolStripMenuItem.Visible = !value;
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
            if (DetailTreeView.SelectedNode.Tag is LicenseServerProperty)
            {                
                LicenseServerProperty licData = DetailTreeView.SelectedNode.Tag as LicenseServerProperty;
                newStandardLicenseToolStripMenuItem.Enabled = true;
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

        private void EnableProductLicenseMainToolStripDropDownMenuItems()
        {
            LicenseServerProperty licData = DetailTreeView.SelectedNode.Tag as LicenseServerProperty;

            //also need to account for admin allowed extra new orders 
            if (m_Permissions.pt_create_modify_key.Value)
            {   //disable new order if at least two validation tokens do not exist                
                if (licData.IsEnabled)
                {
                    newProductLicenseMainToolStripMenuItem.Enabled = false;
                    newProductLicenseMainToolStripMenuItem.Enabled = false;
                    if (licData.IsStandardLicenseType)
                        newProductLicenseMainToolStripMenuItem.Enabled = true;
                    else
                    {
                        if(!licData.IsFailoverLicenseType)
                            existingProductLicenseMainToolStripMenuItem.Enabled = true;
                    }
                }
            }
        }

        private void EnableLicenseContextMenu(bool value)
        {
            LicenseServerProperty licData = null;
            if (DetailTreeView.SelectedNode != null)
                licData = DetailTreeView.SelectedNode.Tag as LicenseServerProperty;
            else
                DetailTreeView.SelectedNode = DetailTreeView.Nodes[0];
            expandToolStripMenuItem.Visible = !DetailTreeView.SelectedNode.IsExpanded;
            expandToolStripMenuItem.Enabled = (DetailTreeView.SelectedNode.Nodes.Count == 0) ? false : true;
            collapseToolStripMenuItem.Visible = DetailTreeView.SelectedNode.IsExpanded;
            //add license items
            addToolStripMenuItem.Visible = true;
            newLicenseToolStripMenuItem.Enabled = m_Permissions.pt_create_modify_key.Value;
            newProductLicenseToolStripMenuItem.Visible = true;
            existingProductLicenseToolStripMenuItem.Visible = true;
            addLicenseToolStripSeparator.Visible = true;
            newFailoverToolStripMenuItem.Visible = true;
            newDisRecToolStripMenuItem.Visible = true;
            newTestDevToolStripMenuItem.Visible = true;
            //separator item
            lcmToolStripSeparator1.Visible = true;
            //generate license item
            createPacketFileToolStripMenuItem.Visible = true;
            createPacketFileToolStripMenuItem.Enabled = false;

            //separator item
            lcmToolStripSeparator3.Visible = true;
            //also need to account for admin allowed extra new orders 
            if (licData != null)
            {
                if (m_Permissions.pt_create_modify_key.Value)
                {
                    addToolStripMenuItem.Enabled = true;

                    if (licData.IsStandardLicenseType)
                    {
                        addLicenseToolStripSeparator.Visible = true;
                        //enable failover if one does already exist
                        if (licData.FailoverLicenseCount == 0)
                            newFailoverToolStripMenuItem.Enabled = !licData.IsDeactivated;
                        newDisRecToolStripMenuItem.Enabled = !licData.IsDeactivated;
                        newTestDevToolStripMenuItem.Enabled = !licData.IsDeactivated;
                    }
                    //license items
                    deleteToolStripMenuItem.Enabled = m_Permissions.pt_create_modify_key.Value;
                    //disable new order if at least two validation tokens do not exist                
                    //allow refresh if license is not expired or has validation tokens          
                    lcmToolStripSeparator2.Visible = true;
                    resyncToolStripMenuItem.Visible = true;
                    resyncToolStripMenuItem.Enabled = false;
                    deleteToolStripMenuItem.Visible = true;
                    deleteToolStripMenuItem.Text = (licData.IsDeactivated) ? "Delete" : "Deactivate";
                    //test/dev, failover, disaster recovery can be updated
                    if (licData.IsStandardLicenseType)
                        newProductLicenseToolStripMenuItem.Enabled = true;
                    else
                    {
                        if (licData.HasUpdates)
                            resyncToolStripMenuItem.Enabled = true;
                        if (!licData.IsFailoverLicenseType)
                            existingProductLicenseToolStripMenuItem.Enabled = !licData.IsDeactivated;
                    }
                    if (licData.IsEnabled)
                        createPacketFileToolStripMenuItem.Enabled = true;
                }
                
            }
        }

        private void EnableProductLicenseContextMenu(bool value)
        {
            expandToolStripMenuItem.Visible = !DetailTreeView.SelectedNode.IsExpanded;
            expandToolStripMenuItem.Enabled = (DetailTreeView.SelectedNode.Nodes.Count == 0) ? false : true;
            collapseToolStripMenuItem.Visible = DetailTreeView.SelectedNode.IsExpanded;
            lcmToolStripSeparator1.Visible = value;
            resyncToolStripMenuItem.Visible = value;
            resyncToolStripMenuItem.Enabled = false;
            deleteToolStripMenuItem.Visible = value;
            deleteToolStripMenuItem.Enabled = m_Permissions.pt_create_modify_key.Value;
            deleteToolStripMenuItem.Text = "Delete";
            //allow deactivate if pl is not deactivated
            if (DetailTreeView.SelectedNode.Tag is ProductLicenseProperty)
            {
                LicenseServerProperty lsProperty;
                if (DetailTreeView.SelectedNode.Parent.Parent.Tag is LicenseServerProperty)
                    lsProperty = DetailTreeView.SelectedNode.Parent.Parent.Tag as LicenseServerProperty;
                else
                    lsProperty = DetailTreeView.SelectedNode.Parent.Parent.Parent.Tag as LicenseServerProperty;
                ProductLicenseProperty plProperty = DetailTreeView.SelectedNode.Tag as ProductLicenseProperty;               
                deleteToolStripMenuItem.Text = (plProperty.IsActive && plProperty.Status != ProductLicenseState.AddOn) ? "Deactivate" : "Delete";
                if (plProperty.LicenseName.Contains("F"))
                    deleteToolStripMenuItem.Enabled = false;
                else
                    deleteToolStripMenuItem.Enabled = !lsProperty.IsDeactivated;
            }
            else  //ProductProperty
                deleteToolStripMenuItem.Enabled = false;
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
            navigateBackToolStripButton.Enabled = true;
            //newCustMainToolStripBtn.Enabled = true;
            searchToolStripTextBox.Enabled = true;
            SearchToolStripLabel.Enabled = true;
            switch (node.Level)
            {
                case 0: //customer
                    if (node.Tag is LicenseServerProperty)
                    {
                        addLicMainToolStripBtn.Enabled = true;
                        addProductLicenseMainToolStripDropDownBtn.Enabled = !(node.Tag as LicenseServerProperty).IsDeactivated;
                        createPacketMainToolStripBtn.Enabled = !(node.Tag as LicenseServerProperty).IsDeactivated;
                        EnableLicenseMainToolStripDropDownMenuItems();
                        EnableProductLicenseMainToolStripDropDownMenuItems();
                    }
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
            MainTabControl.Visible = false;
            //MainTreeView.ExpandAll();
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
                    SearchCurrentView("");
                    EnableDetailListView(true);
                    MainSplitContainer.Panel2.Controls.Clear();
                    //RestoreTreeState(DetailTreeView, m_TreeState);                    
                    loadingCircle1.Parent = MainSplitContainer.Panel2;
                    LicenseViewSplitContainer.Parent = MainSplitContainer.Panel2;
                }
                else if (MainTreeView.SelectedNode.Text == "Packets")
                {
                    ResetMainToolStripMenu();
                    searchToolStripTextBox.Enabled = true;
                    SearchToolStripLabel.Enabled = true;
                    SearchCurrentView("");
                    MainSplitContainer.Panel2.Controls.Clear();
                    //panel1.Parent = MainSplitContainer.Panel2;
                    loadingCircle1.Parent = MainSplitContainer.Panel2;
                    CustomersListView.Parent = MainSplitContainer.Panel2;

                }
                else if (MainTreeView.SelectedNode.Text == "Hardware Keys")
                {
                    ResetMainToolStripMenu();
                    EnableHardwareCustomerView();
                    SearchCurrentView("");                    
                    MainSplitContainer.Panel2.Controls.Clear();
                    loadingCircle1.Parent = MainSplitContainer.Panel2;
                    panel1.Parent = MainSplitContainer.Panel2;
                }
                else
                {
                    if (MainTreeView.SelectedNode.Text != "Reports")
                    {
                        //LoadReportListView();
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
            switch (e.Node.Level)
            { 
                case 1:            //level 1 - Product
                    LoadProductNode(e.Node);
                    m_CurrentLicenseName = (e.Node.Parent.Tag as LicenseServerProperty).Name;
                    DetailPropertyGrid.Enabled = false;
                    break;
                case 2:            //level 2 - Order
                    LoadProductNode(e.Node.Parent);
                    ProductLicenseProperty plProperty = e.Node.Tag as ProductLicenseProperty;
                    LicenseServerProperty licProperty = e.Node.Parent.Parent.Tag as LicenseServerProperty;
                    m_CurrentLicenseName = licProperty.Name;
                    if (licProperty.IsDeactivated || !plProperty.IsActive)
                        DetailPropertyGrid.Enabled = false;
                    break;
                case 3:            //level 3 - AddOn Order
                    LoadProductNode(e.Node.Parent.Parent);
                    ProductLicenseProperty plAddOn = e.Node.Tag as ProductLicenseProperty;
                    LicenseServerProperty lsAddOn = e.Node.Parent.Parent.Parent.Tag as LicenseServerProperty;
                    m_CurrentLicenseName = lsAddOn.Name;
                    if (lsAddOn.IsDeactivated || !plAddOn.IsActive)
                        DetailPropertyGrid.Enabled = false;
                    break;
                default:    //level 0 - Customer
                    if (e.Node.Tag is LicenseServerProperty)
                    {
                        LoadLicenseNode(e.Node);
                        m_CurrentLicenseName = (e.Node.Tag as LicenseServerProperty).Name;
                        if ((e.Node.Tag as LicenseServerProperty).IsDeactivated)
                            DetailPropertyGrid.Enabled = false;
                        createPacketMainToolStripBtn.Enabled = true;
                    }
                    break;
            }
            if (e.Node.Name != "Empty")
            {
                DetailPropertyGrid.SelectedObject = e.Node.Tag;
                EnableToolStripMenu(e.Node);
                //major performance hit
                SetLicenseServerState(e.Node);
                SetCurrentViewTabPages(e.Node);
            }
        }

        private void SetCurrentViewTabPages(TreeNode node)
        {
            if (!PropertyGridTabControl.TabPages.Contains(PropertyGridTabPage))
                PropertyGridTabControl.TabPages.Add(PropertyGridTabPage);
            if (!(node.Tag is CustomerProperty))
            {
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
                ResetContextMenu();
                if (node != null)
                {
                    // Highlight the node that the user clicked.
                    // The node is highlighted until the Menu is displayed on the screen
                    DetailTreeView.SelectedNode = node;
                    // Find the appropriate ContextMenu based on the highlighted node
                    if (node.Level == 0)
                        EnableLicenseContextMenu(true);
                    else
                        EnableProductLicenseContextMenu(true);                    
                }
                else
                {                    //create standard license
                    EnableLicenseContextMenu(true);
                }
                LicenseContextMenuStrip.Show(DetailTreeView, p);
            }
        }

        private void DetailTreeView_BeforeExpand(object sender, TreeViewCancelEventArgs e)
        {
            DetailTreeView.BeginUpdate();
            if (e.Node.Nodes.ContainsKey("virtual"))
            {
                e.Node.Nodes.RemoveByKey("virtual");
                switch (e.Node.Level)
                {
                    case 0:
                        LoadLicenseNode(e.Node);
                        break;
                    case 1:
                        LoadProductNode(e.Node);
                        break;
                    default:
                        break;
                }
                SetLicenseServerState(e.Node);
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
                if (DetailPropertyGrid.SelectedObject is LicenseServerProperty)
                {
                    if (!(DetailPropertyGrid.SelectedObject as LicenseServerProperty).IsDeactivated)
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
                }
                if (DetailPropertyGrid.SelectedObject is ProductLicenseProperty)
                {
                    bShow = true;
                    ProductLicenseProperty plProperty = DetailPropertyGrid.SelectedObject as ProductLicenseProperty;
                    if (m_Permissions.pt_module_pwd.Value && plProperty.IsActive)
                    {
                        LicenseServerProperty license;
                        if(DetailTreeView.SelectedNode.Parent.Parent.Tag is LicenseServerProperty)
                            license = DetailTreeView.SelectedNode.Parent.Parent.Tag as LicenseServerProperty;
                        else
                            license = DetailTreeView.SelectedNode.Parent.Parent.Parent.Tag as LicenseServerProperty;
                        if (license.IsStandardLicenseType)
                        {                            
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
                            if (item != null)
                            {
                                //only if order is an admin order
                                //-admin order does not exist in standard order
                                //-difference between admin vs regular test dev order is
                                dlvEditToolStripMenuItem.Enabled = true;
                                dlvDeleteToolStripMenuItem.Enabled = (item.Tag as ModuleProperty).IsAllowedRemoveModule;
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
                    if (DetailPropertyGrid.SelectedObject is LicenseServerProperty)
                        if(dlvRemoveToolStripButton.Enabled)
                            dlvRemoveToken(DetailPropertyGrid.SelectedObject as LicenseServerProperty);
                    else                
                            dlvRemoveModule(DetailPropertyGrid.SelectedObject as ProductLicenseProperty);
                }
            }
            if(e.KeyCode.Equals(Keys.Insert))
            {
                if (DetailPropertyGrid.SelectedObject is LicenseServerProperty)
                    CreateNewValidationToken(DetailPropertyGrid.SelectedObject as LicenseServerProperty);
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
                        MainTabControl.Visible = true;
                        ResetMainToolStripMenu();
                        SearchCurrentView("");
                        splitContainer2.Panel1.Controls.Clear();
                        //RestoreTreeState(DetailTreeView, m_TreeState);                    
                        loadingCircle1.Parent = splitContainer2.Panel1;
                        MainTabControl.Parent = splitContainer2.Panel1;
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
            ProductLicenseProperty plSelected = null;
            ProductLicenseTable plStored = null;
            Service<ICreator>.Use((client) =>
            {
                switch (DetailTreeView.SelectedNode.Level)
                {
                    case 0:
                        //license level - only changes activations, validation tokens
                        LicenseServerProperty licItem = DetailPropertyGrid.SelectedObject as LicenseServerProperty;
                        licRec = client.GetLicenseByName(licItem.Name, false);
                        if (licRec == null)
                        {
                            MessageBox.Show("Failed to find license : " + licItem.Name, "Save License Information");
                            return;
                        }
                        licPackage = new Lic_PackageAttribs();
                        licPackage.Stream = licRec.LicenseInfo;
                        LicenseItemTransactions(ref licPackage.licLicenseInfoAttribs, ref licItem);
                        licRec.LicenseInfo = licPackage.Stream;
                        licRec.LicenseComments = licItem.Comments;
                        client.UpdateLicense(licRec);
                        LoadTransactionItems(licItem.Name);
                        SetLicenseServerState(DetailTreeView.SelectedNode);
                        break;
                    case 1:
                        break;
                    case 2:
                        //plData level - no add-on orders
                        //alert failover, test dev, disaster recovery 
                        plSelected = DetailPropertyGrid.SelectedObject as ProductLicenseProperty;
                        licRec = client.GetLicenseByName(plSelected.LicenseName, false);
                        if (licRec == null)
                        {
                            MessageBox.Show("Failed to find license server : " + plSelected.LicenseName, "Save Product License Information");
                            return;
                        }
                        plStored = client.GetProductLicense(plSelected.ID);
                        if (plStored == null)
                        {
                            MessageBox.Show("Failed to find product license : " + plSelected.ID, "Save Product License Information");
                            return;
                        }
                        bOrderChanged = OrderTransactions(ref plStored, plSelected);
                        client.UpdateProductLicense(plStored);
                        licPackage = new Lic_PackageAttribs();
                        licPackage.Stream = licRec.LicenseInfo;
                        ProductItemTransactions(ref licPackage.licLicenseInfoAttribs, plSelected, bOrderChanged);
                        licRec.LicenseInfo = licPackage.Stream;
                        client.UpdateLicense(licRec);
                        //issue need to repopulate ProductNode with updated module values from ProductItemTransactions
                        orderProduct = DetailTreeView.SelectedNode.Parent.Tag as ProductProperty;
                        foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs product in licPackage.licLicenseInfoAttribs.TVal.productList.TVal)
                        {
                            if (product.productID.TVal.Equals(plSelected.Product.ID))
                            {
                                orderProduct.ModuleList.TVal = product.moduleList.TVal;
                                orderProduct.Version = plSelected.ProductVersion;
                                break;
                            }
                        }
                        //editing module resets product connections to 1?
                        //other orders may need to be updated with the latest license version
                        foreach (TreeNode node in DetailTreeView.SelectedNode.Parent.Nodes)
                        {
                            ProductLicenseProperty orderNode = node.Tag as ProductLicenseProperty;
                            orderNode.ProductVersion = plSelected.ProductVersion;
                        }
                        foreach (TreeNode node in DetailTreeView.SelectedNode.Nodes)
                        {
                            ProductLicenseProperty orderNode = node.Tag as ProductLicenseProperty;
                            orderNode.ProductVersion = plSelected.ProductVersion;
                            orderNode.AppInstance = plSelected.AppInstance;
                        }
                        //need to reload customer node to set modified and pending states for any licenses
                        //load license node to pass changes to secondary licenses if necessary.
                        //need to disable propertygrid if product license is deactivated
                        SetLicenseServerState(DetailTreeView.SelectedNode);
                        LoadLicenseNode(DetailTreeView.SelectedNode.Parent.Parent);
                        LoadProductNode(DetailTreeView.SelectedNode.Parent);
                        LoadDetailListView(DetailPropertyGrid.SelectedObject as ProductLicenseProperty);
                        if (plSelected.Status == ProductLicenseState.Deactivated)
                            DetailPropertyGrid.Enabled = false;
                        LoadTransactionItems(plSelected.LicenseName);
                        break;
                    case 3:
                        //Add-on plData level         
                        TreeNode productNode = DetailTreeView.SelectedNode.Parent.Parent;
                        orderProduct = DetailTreeView.SelectedNode.Parent.Parent.Tag as ProductProperty;
                        plSelected = DetailPropertyGrid.SelectedObject as ProductLicenseProperty;
                        plStored = client.GetProductLicense(plSelected.ID);
                        //need to delete order if converting from add-on to perm
                        //issue, add-on order merged into perm, how to show transaction      
                        //delete order
                        licPackage = new Lic_PackageAttribs();
                        licRec = client.GetLicenseByName(plSelected.LicenseName, false);
                        licPackage.Stream = licRec.LicenseInfo;
                        ProductLicenseState currentState = (ProductLicenseState)plStored.plState;
                        bOrderChanged = OrderTransactions(ref plStored, plSelected);
                        if (currentState == ProductLicenseState.AddOn &&
                           plSelected.Status == ProductLicenseState.Licensed)
                        {
                            client.DeleteProductLicense(plStored);
                            DetailTreeView.SelectedNode = DetailTreeView.SelectedNode.Parent;
                            //return updated perm order contract
                            (DetailTreeView.SelectedNode.Tag as ProductLicenseProperty).Product.Product = AddOnProductTransactions(ref licPackage.licLicenseInfoAttribs, plSelected);
                            //need to update parent order with values from add-on order
                            DetailTreeView.SelectedNode.Nodes.RemoveByKey(plSelected.ID);
                            productNode = DetailTreeView.SelectedNode.Parent;
                        }
                        else
                        {
                            client.UpdateProductLicense(plStored);
                            ProductItemTransactions(ref licPackage.licLicenseInfoAttribs, plSelected, bOrderChanged);
                        }

                        //need to update productitemtransactions to record addon transactions
                        //issue selectedOrder add-on order has perm modules to add to already existing perm modules
                        //                    regular orders replace values.
                        licRec.LicenseInfo = licPackage.Stream;
                        client.UpdateLicense(licRec);
                        //issue need to repopulate ProductNode with updated module values from ProductItemTransactions
                        foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs product in licPackage.licLicenseInfoAttribs.TVal.productList.TVal)
                        {
                            if (product.productID.TVal.Equals(plSelected.Product.ID))
                            {
                                orderProduct.ModuleList.TVal = product.moduleList.TVal;
                                orderProduct.Version = plSelected.ProductVersion;
                                break;
                            }
                        }
                        orderProduct.AppInstance = plSelected.AppInstance;
                        SetLicenseServerState(DetailTreeView.SelectedNode);
                        LoadProductNode(productNode);
                        LoadDetailListView(DetailTreeView.SelectedNode.Tag as ProductLicenseProperty);
                        LoadTransactionItems(plSelected.LicenseName);
                        break;
                    default:
                        break;
                }
            });
        }

        private void deleteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (deleteToolStripMenuItem.Text == "Deactivate")
            {
                bool bDelete = false;
                DialogResult result = MessageBox.Show(string.Format("Please confirm deactivation for {0}.", DetailTreeView.SelectedNode.Name), "Deactivate Confirmation",
                                          MessageBoxButtons.OKCancel, MessageBoxIcon.Exclamation);
                if (result == DialogResult.OK)
                    bDelete = true;
                if (bDelete)
                {
                    //set license server state to deactivated
                    //set pl state to deactivated
                    if (DetailTreeView.SelectedNode.Tag is LicenseServerProperty)
                    {
                        Service<ICreator>.Use((client) =>
                        {
                            LicenseServerProperty lsProperty = DetailPropertyGrid.SelectedObject as LicenseServerProperty;
                            LicenseTable licRecord = client.GetLicenseByName(lsProperty.Name, false);
                            licRecord.LicenseType = (byte)LicenseServerType.Deactivated;
                            lsProperty.LicType = LicenseServerType.Deactivated;
                            DetailTreeView.SelectedNode.Tag = DetailPropertyGrid.SelectedObject;
                            client.UpdateLicense(licRecord);
                        });
                        SetLicenseServerState(DetailTreeView.SelectedNode);
                    }
                    else
                    {   //propertygrid object needs to be update, treenode status needs to be updated
                        ProductLicenseProperty plProperty = DetailPropertyGrid.SelectedObject as ProductLicenseProperty;
                        Service<ICreator>.Use((client) =>
                        {
                            List<string> plAddOns = client.GetAddOnProductLicenses(plProperty.ID);
                            plAddOns.Add(plProperty.ID);
                            if (plAddOns.Count > 0)
                            {
                                ProductLicenseTable plt;
                                foreach (string pl in plAddOns)
                                {
                                    plt = client.GetProductLicense(pl);
                                    plt.plState = (byte)ProductLicenseState.Deactivated;
                                    client.UpdateProductLicense(plt);
                                }
                            }
                        });
                        plProperty.Status = ProductLicenseState.Deactivated;
                        DetailTreeView.SelectedNode.Tag = DetailPropertyGrid.SelectedObject;
                        DetailPropertyGrid.Refresh();
                        LoadProductNode(DetailTreeView.SelectedNode.Parent);
                    }
                }
            }
            else
            {
                bool bDeleted = false;
                TreeNode parentNode = DetailTreeView.SelectedNode.Parent;
                int selectedIndex = DetailTreeView.SelectedNode.Index;
                if (DetailTreeView.SelectedNode.Tag is LicenseServerProperty)
                    bDeleted = DeleteLicenseNode(DetailTreeView.SelectedNode, true);
                else if (DetailTreeView.SelectedNode.Tag is ProductLicenseProperty)
                    bDeleted = DeleteProductLicenseNode(DetailTreeView.SelectedNode, true);
                if (bDeleted)
                {
                    if (parentNode != null)
                    {
                        if (parentNode.Nodes.Count > 0)
                        {
                            if (selectedIndex == 0)
                                DetailTreeView.SelectedNode = parentNode.Nodes[selectedIndex];
                            else
                                DetailTreeView.SelectedNode = parentNode.Nodes[selectedIndex - 1];
                        }
                    }
                    else
                    {
                        if (selectedIndex > 0)
                            selectedIndex = selectedIndex - 1;
                        if (DetailTreeView.Nodes.Count > 0)
                            DetailTreeView.SelectedNode = DetailTreeView.Nodes[selectedIndex];
                        else
                        {
                            TreeNode emptyNode = new TreeNode("No License Servers Available");
                            emptyNode.Name = "Empty";
                            emptyNode.ImageIndex = emptyNode.SelectedImageIndex = Enums.GetDetailTreeViewIconIndex("Empty");
                            DetailTreeView.Nodes.Add(emptyNode);
                            LicenseViewSplitContainer.Panel2Collapsed = true;
                        }
                    }
                }
            }
        }

        //issue std selected license node is customer node
        //      all others are std license node 
        private void AddLicenseNodeToDetailTreeView(LicenseDialogData data)
        {
            //TreeNode custNode = DetailTreeView.SelectedNode;
            //licInfo has correct license information
            //failover, DR still need to populate modules,products, orders
            TreeNode newNode = CreateLicenseNode(data.LicInfo);
            DetailTreeView.BeginUpdate();
            //resort tree
            LoadDBLicenses("",newNode.Name,false);
            //SetLicenseServerState(newNode);
            //DetailTreeView.Nodes.Find(newNode.Name, false).FirstOrDefault();
            DetailTreeView.EndUpdate();
            DetailPropertyGrid.SelectedObject = newNode.Tag;
        }
        
        private void CreateStandardLicense()
        {  
            LicenseServerProperty newLicense = new LicenseServerProperty();
            Service<ICreator>.Use((client) =>
            {
                CustomerTable custRec = client.GetCustomer(_selectedLicenseCustomer, false);
                if (custRec == null)
                {
                    MessageBox.Show("Failed to initialize standard license","Create Standard License Server");
                    return;
                }
                newLicense.CustID = (uint)custRec.SCRnumber;
                newLicense.GroupID = client.GetNextGroupID(newLicense.CustID, 0);
            });
            newLicense.Permissions = m_Permissions;
            //display settings form for product connections
            using (LicenseInfoForm dlg = new LicenseInfoForm("Create New License Server", ref s_CommLink))
            {
                LicenseDialogData data = new LicenseDialogData(newLicense);
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                    AddLicenseNodeToDetailTreeView(data);
                else
                {
                    if (DetailTreeView.SelectedNode != null)
                    {
                        //refresh current license information                       
                        LoadLicenseNode(DetailTreeView.SelectedNode);
                        DetailPropertyGrid.SelectedObject = DetailTreeView.SelectedNode.Tag;
                    }
                }
            }
        }

        private void CreateFailoverLicense()
        {
            LicenseServerProperty selectedLicense = new LicenseServerProperty(DetailTreeView.SelectedNode.Tag as LicenseServerProperty);
            //pass to function to convert std to failover
            selectedLicense.LicType = LicenseServerType.Failover; 
            //display settings form for product connections
            using (LicenseInfoForm dlg = new LicenseInfoForm("Create New License Server", ref s_CommLink))
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
            selectedLicense.LicType = LicenseServerType.DisasterRecovery;
            using (LicenseInfoForm dlg = new LicenseInfoForm("Create New License Server", ref s_CommLink))
            {
                LicenseDialogData data = new LicenseDialogData(selectedLicense);
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                    AddLicenseNodeToDetailTreeView(data);
            }
        }

        private void CreateTestDevLicense()
        {
            LicenseServerProperty selectedLicense = new LicenseServerProperty(DetailTreeView.SelectedNode.Tag as LicenseServerProperty);
            selectedLicense.LicType = LicenseServerType.TestDevelopment;
            //pass to function to convert std to failover
            using (LicenseInfoForm dlg = new LicenseInfoForm("Create New License Server", ref s_CommLink))
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
                                                  Path.Combine(m_selectedDirectory, _selectedLicenseCustomer),
                                                  "");
            
            using (LicenseInfoForm dlg = new LicenseInfoForm("Create New Packet", ref s_CommLink))
            {
                PacketDialogData data = new PacketDialogData(newPacket, _selectedLicenseCustomer);
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
            if (DetailPropertyGrid.SelectedObject is LicenseServerProperty)
                CreateNewValidationToken(DetailPropertyGrid.SelectedObject as LicenseServerProperty);
        }

        private void dlvEditToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ShowEditDialog(DetailPropertyGrid.SelectedObject, DetailListView.SelectedIndices);
        }

        private void ShowEditDialog(Object selectedObj, ListView.SelectedIndexCollection selectedIndices)
        {
            if (!(selectedObj is ProductProperty))
            {
                if (selectedObj is LicenseServerProperty)
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
                LicenseServerProperty selectedLicense =  DetailPropertyGrid.SelectedObject as LicenseServerProperty;
                TokenDialogData data = new TokenDialogData(tokenInfo,selectedLicense);
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                {
                    Service<ICreator>.Use((client) =>
                    {
                        TokenTable dbToken = client.GetTokenByLicenseName(selectedLicense.Name, (byte)data.Token.TokenType);
                        if (dbToken == null)
                        {
                            MessageBox.Show("Failed to find token to edit", "Edit Validation Token");
                            return;
                        }
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
                ModuleDialogData data = new ModuleDialogData(moduleList, DetailPropertyGrid.SelectedObject as ProductLicenseProperty);
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                {                   
                    SaveLicense();                    
                    PopulateDetailListView(data.ProductLicense);
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
                SetLicenseServerState(DetailTreeView.SelectedNode);
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

        private uint GetTotalAddOnModuleAppInstance(ProductLicenseProperty plData, ModuleProperty storedMod)
        {
            uint totalValue = 0;
            string stdProductLicense = "";
            IList<string> addOnOrders = null;
            Service<ICreator>.Use((client) =>
            {
                stdProductLicense = (storedMod.ModuleState != Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msAddOn) ? plData.ID : plData.ParentID;
                addOnOrders = client.GetAddOnProductLicenses(stdProductLicense);
            });
            foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs modRec in plData.Product.ModuleList.TVal)
            {
                if (modRec.moduleID.TVal.Equals(storedMod.ID) && (addOnOrders.Contains(modRec.contractNumber.TVal)))
                    totalValue += modRec.moduleAppInstance.TVal;
            }
            return totalValue;
        }

        private void IncrementAddOnModuleAppInstance(ProductLicenseProperty plData, ModuleProperty module)
        {
            //increment first add-on found
            IList<string> addOnOrders = null;
            Service<ICreator>.Use((client) =>
            {
                addOnOrders = client.GetAddOnProductLicenses(module.OrderNumber);
            });
            if (addOnOrders != null)
            {
                if (addOnOrders.Count > 0)
                {
                    foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs storedMod in plData.Product.ModuleList.TVal)
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
            if(DetailPropertyGrid.SelectedObject is LicenseServerProperty)   //token remove
                dlvRemoveToken(DetailPropertyGrid.SelectedObject as LicenseServerProperty);
            else                //module remove
                dlvRemoveModule(DetailPropertyGrid.SelectedObject as ProductLicenseProperty);
        }

        private void TransactionListView_ColumnClick(object sender, ColumnClickEventArgs e)
        {
            _lvManager.SetSortIndexColumn(TransactionListView.Handle, e.Column);
            // Perform the sort with these new sort options.
            TransactionListView.Sort();
            SetSingleTransactionListViewItemBackColor();
        }

        private void searchToolStripTextBox_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter || e.KeyCode == Keys.Return)
            {
                SearchCurrentView(searchToolStripTextBox.Text);
            }
        }

        private void verifyToolStripMenuItem_Click(object sender, EventArgs e)
        {
            int imageIndex = Enums.GetListViewIconIndex("VERIFIED");           
            using (PacketVerification dlg = new PacketVerification())
            {   //selectedItem text will be packet name
                PacketVerificationDialogData data = new PacketVerificationDialogData();
                int selectedIndex = 0;
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                {
                    if (data.Verified)
                    {
                        //only valid verification code can get to this point
                        foreach (ListViewItem item in storageListView.Items)
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
            foreach (ListViewItem lvItem in storageListView.Items)
            {   //type length > 0 filter otherwise all
                if (lvItem.ImageIndex == Enums.GetListViewIconIndex(type) ||
                    type.Length == 0)
                    LicensePacketListView.Items.Insert(0,lvItem.Clone() as ListViewItem);
            }
            _lvManager.AutoResizeColumns(LicensePacketListView);
            LicensePacketListView.EndUpdate();          
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
                        if (dbLicense == null)
                        {
                            MessageBox.Show("Failed to access license for new token", "Create New Validation Token");
                            return;
                        }
                        if (data.Token.TokenType == Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID)
                        {
                            dbToken = client.GetHardwareTokenByKeyValue(data.Token.TokenValue);
                            if (dbToken == null)
                            {
                                MessageBox.Show("Failed to fine hardware token :" + data.Token.TokenValue + " for creation.");
                                return;
                            }
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
        private TreeNode CreateProductLicenseNode(ProductLicenseProperty plData)
        {
            CreateProductLicenseEntity(plData);
            TreeNode orderNode = new TreeNode(plData.ID);
            orderNode.Name = plData.ID;
            orderNode.Tag = plData;
            if (plData.ParentID != null)
            {
                orderNode.ImageIndex = Enums.GetDetailTreeViewIconIndex("ADDONORDER");
                orderNode.SelectedImageIndex = orderNode.ImageIndex;
            }
            else
            {
                orderNode.ImageIndex = Enums.GetDetailTreeViewIconIndex("ORDER");
                orderNode.SelectedImageIndex = orderNode.ImageIndex;
            }
            return orderNode;
        }

        private void CreateProductLicenseEntity(ProductLicenseProperty plData)
        {
            ProductLicenseTable orderRec = new ProductLicenseTable();
            Service<ICreator>.Use((client) =>
            {
                LicenseTable licRec = client.GetLicenseByName(plData.LicenseName, false);
                if (licRec == null)
                {
                    MessageBox.Show("Failed to find license " + plData.LicenseName, "Create New Product License");
                    return;
                }
                orderRec.plID = plData.ID;
                orderRec.plIndex = (int)plData.Index;
                orderRec.plState = (byte)plData.Status;
                orderRec.LicenseID = licRec.ID;
                if (plData.ExpirationDate.HasValue)
                    orderRec.ExpirationDate = plData.ExpirationDate.Value.ToUniversalTime();
                else
                    orderRec.ExpirationDate = plData.ExpirationDate;
                orderRec.Description = plData.Description;
                orderRec.ProductID = (short)plData.Product.ID;
                orderRec.ProductVersion = plData.Product.Version.ToString();
                orderRec.ParentProductLicenseID = plData.ParentID;
                client.CreateProductLicense(orderRec);
                CreateTransaction(TransactionType.Order,
                                  plData,
                                  string.Format("Added {0} Product License - {1}", plData.ProductName, plData.Status.ToString()),
                                  plData.ID);
                //update order index to next available index
                licRec.OrderIndex = orderRec.plIndex + 1;
                client.UpdateLicense(licRec);
            });
            //create product order also 
        }

        //create only done on new contract with new product
        private TreeNode CreateProductNode(ProductLicenseProperty plData)
        {
            Lic_PackageAttribs.Lic_ProductInfoAttribs product = CreateProductEntity(plData);
            TreeNode node = new TreeNode(plData.Product.Name);
            node.Name = plData.Product.Name;
            node.ImageIndex = Enums.GetDetailTreeViewIconIndex(s_CommLink.GetProductBaseName(node.Name));
            node.SelectedImageIndex = node.ImageIndex;
            node.Tag = new ProductProperty(product);
            return node;
        }

        //returns the created product module list, issue comes when creating product node that already exists
        //need to add new product entity into existing product node not overwrite
        private Lic_PackageAttribs.Lic_ProductInfoAttribs CreateProductEntity(ProductLicenseProperty plData)
        {
            Lic_PackageAttribs.Lic_ProductInfoAttribs product = null;
            SetProductEntity(plData);
            Service<ICreator>.Use((client) =>
            {
                LicenseTable licRec = client.GetLicenseByName(plData.LicenseName, false);
                if (licRec == null)
                {
                    MessageBox.Show("Failed to find license server : " + plData.LicenseName,"Create New Product");
                    return;
                }
                Lic_PackageAttribs licPackage = new Lic_PackageAttribs();
                licPackage.Stream = licRec.LicenseInfo;
                bool bFound = false;
                foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs dbProduct in licPackage.licLicenseInfoAttribs.TVal.productList.TVal)
                {
                    if (dbProduct.productID.TVal.Equals(plData.Product.ID))
                    {
                        dbProduct.moduleList.TVal.AddRange(plData.Product.ModuleList.TVal);
                        dbProduct.product_Major.TVal = plData.Product.Product.product_Major.TVal;
                        dbProduct.product_Minor.TVal = plData.Product.Product.product_Minor.TVal;
                        if(plData.ProductLicenseType != Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.TProductLicenseType.pltClient)
                            if (plData.Status != ProductLicenseState.AddOn)
                                dbProduct.productAppInstance.TVal += 1;
                        product = dbProduct;
                        bFound = true;
                        break;
                    }
                }
                if (!bFound)
                {
                    licPackage.licLicenseInfoAttribs.TVal.productList.TVal.Add(plData.Product.Product);
                    product = plData.Product.Product;
                }
                licRec.LicenseInfo = licPackage.Stream;
                client.UpdateLicense(licRec);
            });
            return product;
        }

        //fill product entity with default modules
        //only trial/addon order types
        private void SetProductEntity(ProductLicenseProperty plData)
        {
            Lic_PackageAttribs.Lic_ModuleInfoAttribs module;
            Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.Lic_ModuleSoftwareSpecAttribsMap moduleList;
            foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in s_CommLink.m_softwareSpec.productSpecMap.TVal.Values)
            {
                if (productSpec.productID.TVal == plData.Product.ID)
                {
                    moduleList =  s_CommLink.GetModuleSpecList(productSpec.productID.TVal);
                    foreach (Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs moduleSpec in moduleList.TVal.Values)
                    {                        
                        module = new Lic_PackageAttribs.Lic_ModuleInfoAttribs();
                        module.moduleID.TVal = moduleSpec.moduleID.TVal;
                        if (plData.Status.Equals(ProductLicenseState.Trial))
                        {
                            module.moduleValue.TVal = moduleSpec.moduleTrialLicense.TVal;
                            module.moduleAppInstance.TVal = 1;
                        }
                        module.moduleExpirationDate.TVal = plData.ExpirationDate.Value.ToUniversalTime();
                        module.contractNumber.TVal = plData.ID;
                        module.moduleState.TVal = (Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState)Enums.GetProductLicenseState(plData.Status);
                        plData.Product.ModuleList.TVal.Add(module);
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
            node.ImageIndex = Enums.GetDetailTreeViewIconIndex("License");
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

        void AddExistingProductLicense(LicenseServerProperty licData, List<string> productLicenses)
        {
            //get std license
            //get test dev license
            //for each order selected in orders
            Service<ICreator>.Use((client) => 
            {
                string licenseBase = "P";
                LicenseTable baseLicRec = client.GetLicenseByName(string.Format("{0:x4}-{1:x3}-{2:x4}-{3}01", licData.CustID, licData.DestID, licData.GroupID, licenseBase), false);
                if (baseLicRec == null) //subscription type
                {
                    licenseBase = "S";
                    baseLicRec = client.GetLicenseByName(string.Format("{0:x4}-{1:x3}-{2:x4}-{3}01", licData.CustID, licData.DestID, licData.GroupID, licenseBase), false);
                }
                if (baseLicRec == null)
                {
                    MessageBox.Show("Failed to find base product license for : " + licData.Name, "Add Existing Product License Error");
                    return;
                }
                Lic_PackageAttribs baseLicPackage = new Lic_PackageAttribs();
                baseLicPackage.Stream = baseLicRec.LicenseInfo;
                string[] orderNumbers;
                LicenseTable licRec = client.GetLicenseByName(licData.Name, false);
                Lic_PackageAttribs licPackage = new Lic_PackageAttribs();
                licPackage.Stream = licRec.LicenseInfo;
                ProductLicenseTable orderRec, storedRec;
                string replaceString = Enums.GetLicenseServerPrefix(licData.LicType);
                foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs product in baseLicPackage.licLicenseInfoAttribs.TVal.productList.TVal)
                {
                    //std order
                    orderNumbers = Lic_LicenseInfoAttribsHelper.GetContractNumbers(product);
                    foreach (string orderNum in orderNumbers)
                    {                        //sub-orders                         
                        if (productLicenses.Contains(orderNum.Replace(replaceString, licenseBase)))
                        {                            //get standard order
                            orderRec = client.GetProductLicense(orderNum);
                            orderRec.LicenseID = licRec.ID;
                            orderRec.plID = string.Format("{0}-{1}", licData.Name, orderRec.plIndex);
                            orderRec.Description = "";
                            if (replaceString.Equals("T"))
                                orderRec.plState = (byte)ProductLicenseState.Trial;
                            if (licData.LicType == LicenseServerType.DisasterRecovery) 
                                orderRec.ExpirationDate = null;
                            else
                                orderRec.ExpirationDate = DateTime.Now.AddDays(30);
                            Lic_PackageAttribs.Lic_ProductInfoAttribs newProduct = new Lic_PackageAttribs.Lic_ProductInfoAttribs();
                            newProduct.Stream = product.Stream;
                            if (licData.LicType == LicenseServerType.TestDevelopment)
                            {   //modifying baselic effects enumeration
                                newProduct.productID.TVal = s_CommLink.GetTestDevProductSpec(product.productID.TVal).productID;
                                orderRec.ProductID = (short)newProduct.productID.TVal;
                            }
                            client.CreateProductLicense(orderRec);
                            storedRec = client.GetProductLicense(orderRec.plID);
                            CreateTransaction(TransactionType.Order,
                                              licData.Name,
                                              string.Format("Added {0} Product License - {1}", s_CommLink.GetProductName((uint)orderRec.ProductID), Enum.GetName(typeof(Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState), orderRec.plState)),
                                              orderRec.plID);
                            //convert product into test/dev product for test/dev key
                            //Don't include add-on orders
                            client.UpdateSubLicenseTransactionsByOrder(orderRec.ID, storedRec.ID);
                            foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs module in newProduct.moduleList.TVal)
                            {
                                if (module.contractNumber.TVal.Equals(orderNum))
                                {
                                    module.contractNumber.TVal = orderRec.plID;
                                    //Disaster Recovery has no expiration dates.                                  
                                    module.moduleExpirationDate.TVal = (licData.LicType == LicenseServerType.DisasterRecovery) ? new DateTime(1900, 1, 1) : orderRec.ExpirationDate.Value.ToLocalTime() ;
                                }
                            }
                            licPackage.licLicenseInfoAttribs.TVal.productList.TVal.Add(newProduct);
                        }
                        else
                        {
                            orderRec = client.GetProductLicense(orderNum);
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
                licRec.LicenseInfo = licPackage.Stream;
                client.UpdateLicense(licRec);
            });
        }

        //if existing replace old order
        void UpdateExistingProductLicense(LicenseServerProperty licData)
        {
            //only needed for disaster recovery and test dev            
            Service<ICreator>.Use((client) => 
            {
                //need to get current active P
                string licenseBase = "P";
                LicenseTable baseLicRec = client.GetLicenseByName(string.Format("{0:x4}-{1:x3}-{2:x4}-{3}01", licData.CustID, licData.DestID, licData.GroupID, licenseBase), false);
                if (baseLicRec == null) //subscription type
                {
                    licenseBase = "S";
                    baseLicRec = client.GetLicenseByName(string.Format("{0:x4}-{1:x3}-{2:x4}-{3}01", licData.CustID, licData.DestID, licData.GroupID, licenseBase), false);
                }
                if (baseLicRec == null)
                {
                    MessageBox.Show("Failed to find base product license : " + licData.Name, "Update Existing Produce License");
                    return;
                }
                Lic_PackageAttribs baseLicPackage = new Lic_PackageAttribs();
                baseLicPackage.Stream = baseLicRec.LicenseInfo;                
                LicenseTable subLicRec = client.GetLicenseByName(licData.Name, false);
                Lic_PackageAttribs subLicPackage = new Lic_PackageAttribs();
                subLicPackage.Stream = subLicRec.LicenseInfo;
                ProductLicenseTable plRec, dbplRec;
                uint productID;
                string replaceString = (licData.LicType == LicenseServerType.DisasterRecovery) ? "D" : "T";
                string[] plNumbers, splNumbers;
                string currProductLicense = "";
                //remove any dr orders that don't exist in std.
                foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs subLicProduct in subLicPackage.licLicenseInfoAttribs.TVal.productList.TVal)
                {
                    splNumbers = Lic_LicenseInfoAttribsHelper.GetContractNumbers(subLicProduct);
                    foreach (string splNumber in splNumbers)
                    {   //find standard order
                        bool bFound = false;
                        foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs baseProduct in baseLicPackage.licLicenseInfoAttribs.TVal.productList.TVal)
                        {
                            productID = (replaceString.Equals("T")) ? s_CommLink.GetTestDevProductSpec(baseProduct.productID.TVal).productID.TVal : baseProduct.productID.TVal;
                            if (productID.Equals(subLicProduct.productID.TVal))
                            {
                                splNumbers = Lic_LicenseInfoAttribsHelper.GetContractNumbers(baseProduct);
                                currProductLicense = splNumber.Replace(replaceString, licenseBase);
                                if (splNumbers.Contains(currProductLicense))
                                    bFound = true;
                                break;
                            }
                        }
                        if (!bFound)
                        {  //remove order from database
                            DeleteProductLicenseNode(DetailTreeView.Nodes.Find(splNumber, true).FirstOrDefault(), false);
                        }
                    }
                }
                bool bUpdated = false;
                foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs stdProduct in baseLicPackage.licLicenseInfoAttribs.TVal.productList.TVal)
                {
                    plNumbers = Lic_LicenseInfoAttribsHelper.GetContractNumbers(stdProduct);
                    foreach (string plNumber in plNumbers)
                    {   //stdLicense Order
                        plRec = client.GetProductLicense(plNumber);
                        //avoid orders that are add-ons
                        if (plRec != null)
                        {
                            if (plRec.plState != (byte)ProductLicenseState.AddOn)
                            {   //subLicense Order
                                dbplRec = client.GetProductLicense(string.Format("{0}-{1}", licData.Name, plRec.plIndex));
                                //add any transactions from std order to derived order
                                if (dbplRec != null)
                                {   //stored order exists, update - only update state
                                    bUpdated = true;
                                    dbplRec.plState = (replaceString.Equals("T")) ? (byte)ProductLicenseState.Trial : plRec.plState;
                                    client.UpdateSubLicenseTransactionsByOrder(plRec.ID, dbplRec.ID);
                                    foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs stdModule in stdProduct.moduleList.TVal)
                                    {
                                        //need to update module in licPackage with values from stdModule
                                        if (stdModule.contractNumber.TVal.Equals(plNumber))
                                        {
                                            foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs product in subLicPackage.licLicenseInfoAttribs.TVal.productList.TVal)
                                            {
                                                productID = (replaceString.Equals("T")) ? s_CommLink.GetTestDevProductSpec(stdProduct.productID.TVal).productID.TVal : stdProduct.productID.TVal;
                                                if (product.productID.TVal.Equals(productID))
                                                {
                                                    foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs module in product.moduleList.TVal)
                                                    {
                                                        if (module.contractNumber.TVal.Equals(plNumber.Replace(licenseBase, replaceString)) &&
                                                            module.moduleID.TVal.Equals(stdModule.moduleID.TVal))
                                                        {
                                                            module.moduleState.TVal = stdModule.moduleState.TVal;
                                                            module.moduleValue.TVal = stdModule.moduleValue.TVal;
                                                            module.moduleAppInstance.TVal = stdModule.moduleAppInstance.TVal;
                                                            //Disaster Recovery has no expiration dates.
                                                            module.moduleExpirationDate.TVal = (licData.LicType == LicenseServerType.DisasterRecovery) ? new DateTime(1900, 1, 1) : dbplRec.ExpirationDate.Value;
                                                            break;
                                                        }
                                                    }
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    client.UpdateProductLicense(dbplRec);
                                }
                            }
                        }
                    }
                }
                if (bUpdated)
                {
                    subLicRec.LicenseInfo = subLicPackage.Stream;
                    client.UpdateLicense(subLicRec);
                }
            });
        }

        //pl = productlicense
        //spl = standard product license
        //dbpl = database product license
        private void FailoverUpdateProductLicense(LicenseServerProperty licData)
        {
            Service<ICreator>.Use((client) => 
            {
                string licenseBase = "P"; 
                LicenseTable baseLicRec = client.GetLicenseByName(string.Format("{0:x4}-{1:x3}-{2:x4}-{3}01", licData.CustID, licData.DestID, licData.GroupID, licenseBase), false);
                if (baseLicRec == null) //subscription type
                {
                    licenseBase = "S";
                    baseLicRec = client.GetLicenseByName(string.Format("{0:x4}-{1:x3}-{2:x4}-{3}01", licData.CustID, licData.DestID, licData.GroupID, licenseBase), false);
                }
                if (baseLicRec == null)
                {
                    MessageBox.Show("Failed to find base license server", "Update Failover Product Licenses");
                    return;
                }
                Lic_PackageAttribs licPackage = new Lic_PackageAttribs();
                licPackage.Stream = baseLicRec.LicenseInfo;
                string[] splNumbers, fplNumbers;
                LicenseTable failOverLicenseRec = client.GetLicenseByName(licData.Name, false);
                Lic_PackageAttribs failoverLicPackage = new Lic_PackageAttribs();
                failoverLicPackage.Stream = failOverLicenseRec.LicenseInfo;
                ProductLicenseTable plRec, dbplRec;
                //Delete any orders that don't exist in std license
                string currProductLicense = "";
                foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs failoverProduct in failoverLicPackage.licLicenseInfoAttribs.TVal.productList.TVal)
                {
                    fplNumbers = Lic_LicenseInfoAttribsHelper.GetContractNumbers(failoverProduct);
                    foreach (string fplNumber in fplNumbers)
                    {   //find standard order
                        bool bFound = false;
                        foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs baseProduct in licPackage.licLicenseInfoAttribs.TVal.productList.TVal)
                        {
                            if (baseProduct.productID.TVal.Equals(failoverProduct.productID.TVal))
                            {
                                splNumbers = Lic_LicenseInfoAttribsHelper.GetContractNumbers(baseProduct);
                                currProductLicense = fplNumber.Replace("F", licenseBase);
                                if (splNumbers.Contains(currProductLicense))
                                    bFound = true;
                                break;
                            }
                        }
                        if (!bFound)
                        {  //remove order from database
                            DeleteProductLicenseNode(DetailTreeView.Nodes.Find(fplNumber, true).FirstOrDefault(), false);
                        }
                    }
                }
                foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs product in licPackage.licLicenseInfoAttribs.TVal.productList.TVal)
                {
                    splNumbers = Lic_LicenseInfoAttribsHelper.GetContractNumbers(product);
                    foreach (string splNumber in splNumbers)
                    {
                        plRec = client.GetProductLicense(splNumber);
                        dbplRec = client.GetProductLicense(string.Format("{0}-{1}", licData.Name, plRec.plIndex));
                        if (dbplRec != null)
                        {   //stored order exists, update
                            dbplRec.plState = plRec.plState;
                            dbplRec.ProductVersion = plRec.ProductVersion;
                            dbplRec.ExpirationDate = plRec.ExpirationDate;
                            dbplRec.Description = plRec.Description;
                            client.UpdateSubLicenseTransactionsByOrder(plRec.ID, dbplRec.ID);
                            foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs module in product.moduleList.TVal)
                            {
                                if (module.contractNumber.TVal.Equals(splNumber))
                                    module.contractNumber.TVal = dbplRec.plID;
                            }
                            client.UpdateProductLicense(dbplRec);
                        }
                        else
                        {   //storedRec order doesn't exist, create                            
                            plRec.LicenseID = failOverLicenseRec.ID;
                            plRec.plID = string.Format("{0}-{1}", licData.Name, plRec.plIndex);
                            if (plRec.ParentProductLicenseID != null)
                                plRec.ParentProductLicenseID = plRec.ParentProductLicenseID.Replace(licenseBase, "F");
                            foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs module in product.moduleList.TVal)
                            {
                                if (module.contractNumber.TVal.Equals(splNumber))
                                    module.contractNumber.TVal = plRec.plID;
                            }
                            client.CreateProductLicense(plRec);
                            dbplRec = client.GetProductLicense(string.Format("{0}-{1}", licData.Name, plRec.plIndex));
                            client.UpdateSubLicenseTransactionsByOrder(plRec.ID, dbplRec.ID);
                        }
                    }
                }
                failoverLicPackage.licLicenseInfoAttribs.TVal.productList.TVal = licPackage.licLicenseInfoAttribs.TVal.productList.TVal;
                failOverLicenseRec.LicenseInfo = failoverLicPackage.Stream;
                client.UpdateLicense(failOverLicenseRec);
            });
        }        
        //node is current selected product license node
        private bool DeleteProductLicenseNode(TreeNode plNode, bool bVerify)
        {
            bool bDeleteProductLicense = true;
            TreeNode productNode;
            if (plNode != null)
            {
                ProductLicenseProperty plData = plNode.Tag as ProductLicenseProperty;
                if (bVerify)
                {
                    bDeleteProductLicense = false;
                    DialogResult result = MessageBox.Show(string.Format("Please confirm deletion of product license: {0}.", plData.ID), "Delete Product License Confirmation",
                                          MessageBoxButtons.OKCancel, MessageBoxIcon.Exclamation);
                    if (result == DialogResult.OK)
                        bDeleteProductLicense = true;
                }
                if (bDeleteProductLicense)
                {
                    if (plData.ParentID != null)
                        productNode = plNode.Parent.Parent;
                    else
                        productNode = plNode.Parent;
                    Lic_PackageAttribs.Lic_ProductInfoAttribs storedProduct = DeleteOrderEntity(plNode.Tag as ProductLicenseProperty);
                    productNode.Nodes.Remove(plNode);                    
                    if (productNode.Nodes.Count == 0)
                    {
                        DeleteProductEntity(productNode.Parent.Name, productNode.Name);
                        productNode.Remove();                       
                    }
                    else
                    {
                        productNode.Tag = new ProductProperty(storedProduct);
                        //now set to either product node or perm order node
                        if (plData.ParentID!= null)
                            LoadDetailListView(productNode.Nodes.Find(plData.ParentID, true).FirstOrDefault().Tag as ProductLicenseProperty);
                        else
                            LoadDetailListView(productNode.Tag as ProductProperty);
                    }
                    return true;
                }
            }
            return false;
        }

        private void DeleteAllProductLicenseEntities(string licenseServer)
        {
            Service<ICreator>.Use((client) =>
            {
                client.DeleteAllProductLicenses(licenseServer);
            });
        }

        //remove order entity from DB, remove any modules with order number and update license
        private Lic_PackageAttribs.Lic_ProductInfoAttribs DeleteOrderEntity(ProductLicenseProperty plData)
        {
            Lic_PackageAttribs.Lic_ProductInfoAttribs storedProduct = new Lic_PackageAttribs.Lic_ProductInfoAttribs();
            CreateTransaction(TransactionType.Order,
                              plData,
                              string.Format("Removed {0} Product License - {1}", plData.ProductName, plData.Status.ToString()),
                              plData.ID);
            Service<ICreator>.Use((client) =>
            {
                ProductLicenseTable plStored = client.GetProductLicense(plData.ID);
                if (plStored == null)
                {
                    MessageBox.Show("Failed to find produce license : " + plData.ID, "Delete Produce License");
                    return;
                }
                client.DeleteProductLicense(plStored);
                if (plData.ParentID == null)
                {   //get addons, remove 
                    List<string> plAddons = client.GetAddOnProductLicenses(plData.ID);
                    foreach (string addon in plAddons)
                    {
                        plStored = client.GetProductLicense(addon);
                        client.DeleteProductLicense(plStored);
                    }
                }
            });

            Service<ICreator>.Use((client) =>
            {
                LicenseTable storedLicense = client.GetLicenseByName(plData.LicenseName, false);
                if (storedLicense == null)
                {
                    MessageBox.Show("Failed to find license server : " + plData.LicenseName, "Delete Produce License");
                    return;
                }
                Lic_PackageAttribs licPackage = new Lic_PackageAttribs();
                licPackage.Stream = storedLicense.LicenseInfo;
                foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs product in licPackage.licLicenseInfoAttribs.TVal.productList.TVal)
                {
                    if (product.productID.TVal.Equals(plData.Product.ID))
                    {
                        //order product has same value as dbproduct
                        //client type subtract total, client add-on subtract nothing
                        //decrement appinstance for product when removing an order
                        if (plData.ProductLicenseType != Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.TProductLicenseType.pltClient)
                            if (plData.Status != ProductLicenseState.AddOn)
                                product.productAppInstance.TVal -= 1;
                        Lic_PackageAttribs.Lic_ProductInfoAttribs.Lic_ModuleInfoAttribsList moduleList = Lic_LicenseInfoAttribsHelper.GetModuleList_ByContractNumber(product, plData.ID);
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
            node.Remove();
        }

        private void DeleteProductEntity(string licenseName, string productName)
        {
            uint selectedProductID = (uint)s_CommLink.GetProductID(productName);
            Service<ICreator>.Use((client) => 
            {
                LicenseTable licRec = client.GetLicenseByName(licenseName, false);
                if (licRec == null)
                {
                    MessageBox.Show("Failed to find license server : " + licenseName, "Delete Product Information");
                    return;
                }
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
        private bool DeleteLicenseNode(TreeNode selectedLicenseNode, bool bVerify)
        {
            bool bDelete = false;
            LicenseServerProperty selectedLicense = selectedLicenseNode.Tag as LicenseServerProperty;
            if(bVerify)
            {
                DialogResult result = MessageBox.Show(string.Format("Please confirm deletion of license: {0}.", selectedLicense.Name), "Delete License Confirmation",
                                          MessageBoxButtons.OKCancel, MessageBoxIcon.Exclamation);
                if (result == DialogResult.OK)
                    bDelete = true;
            }
            if(bDelete)
            {
                //verify delete is allowed.            
                if (selectedLicense.IsStandardLicenseType)
                {
                    int derivedCount = 0;
                    Service<ICreator>.Use((client) =>
                    {
                        derivedCount = client.GetDerivedLicenseCount(selectedLicense.CustID, selectedLicense.DestID, selectedLicense.GroupID, selectedLicense.SoftwareLicenseType);
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
                //foreach (TreeNode productNode in selectedLicenseNode.Nodes)
                //{
                //    if (productNode.Name != "virtual")
                //    {
                //        //foreach (TreeNode plNode in productNode.Nodes)
                //        //{
                //        //    if (productNode.Nodes.Count > 0)
                //        //    {
                //        //        foreach (TreeNode addOnNode in plNode.Nodes)
                //        //            DeleteOrderEntity(addOnNode.Tag as ProductLicenseProperty);                                 
                //        //    }
                //        //    DeleteOrderEntity(plNode.Tag as ProductLicenseProperty);                        
                //        //}                       
                //        DeleteProductEntity(selectedLicense.Name, productNode.Name);
                //    }
                //}
                DeleteAllProductLicenseEntities(selectedLicense.Name);
                selectedLicenseNode.Nodes.Clear();
                DeleteLicenseEntity((selectedLicenseNode.Tag as LicenseServerProperty).Name);
                DetailTreeView.Nodes.Remove(selectedLicenseNode);
                return true;
            }
            return false;
        }

        private void DeleteLicenseEntity(string licenseName)
        {
            Service<ICreator>.Use((client) => 
            {
                LicenseTable licRecord = client.GetLicenseByName(licenseName, false);
                if (licRecord == null)
                {
                    MessageBox.Show("Failed to find license server : " + licenseName, "Delete License Server");
                    return;
                }
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

        //delete destination names
        //delete licenses
        //delete tokens
        //delete orders

        private bool DeleteCustomerItem(ListViewItem lvItem)
        {
            string custName = lvItem.SubItems[1].Text;
            int custID = Int32.Parse(lvItem.Text);
            DialogResult result = MessageBox.Show(string.Format("Please confirm deletion of customer: {0}.", custName), "Delete Customer Confirmation",
                          MessageBoxButtons.OKCancel, MessageBoxIcon.Exclamation);
            if (result == DialogResult.OK)
            {
                //Service<ICreator>.Use((client) =>
                //{
                //    client.DeleteDestinationNameByCustomer(custID);
                //    //delete destination names
                //    //delete licenses
                //});
                //foreach (TreeNode node in selectedCustNode.Nodes)
                //{
                //    DeleteLicenseNode(node, false);
                //}
                //DeleteCustomerEntity(custID);
                //DetailTreeView.Nodes.Remove(selectedCustNode);
                //return true;
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

        private void PopulateHardwareKeyView(string searchString, string custName)
        {
            IList<TokenTable> tokens = null;
            string licenseName;
            LicenseTable license = null;
            ListViewItem[] lvItems = null;
            HardwareKeyListView.Items.Clear();
            Service<ICreator>.Use((client) =>
            {
                tokens = client.GetAllTokensByCustomer(searchString, custName, Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID);
                if (tokens.Count > 0)
                {
                    tokens = tokens.OrderBy(c => c.TokenValue).ToList();
                    lvItems = new ListViewItem[tokens.Count];
                    for (int index = 0; index < tokens.Count; index++)
                    {
                        ListViewItem item = new ListViewItem();
                        item.Name = item.Text = tokens[index].TokenValue;
                        license = client.GetLicenseByID(tokens[index].LicenseID, false);
                        licenseName = "";
                        if (license != null)
                            licenseName = license.LicenseName;
                        item.SubItems.Add(licenseName);
                        item.SubItems.Add(((TokenStatus)tokens[index].TokenStatus).ToString());
                        lvItems[index] = item;
                    }
                    this.HardwareKeyListView.Items.AddRange(lvItems);
                }
            });            

        }

        //initial view should be customers, key counts
        //double click on customer should be list of keys license server status
        private void PopulateHardwareKeyCustomerView(string searchString)
        {
            IList<TokenTable> tokens = null;
            IList<CustomerTable> customers = null;
            int reservedKeys, activeKeys, deactivatedKeys;
            Service<ICreator>.Use((client) =>
            {
                customers = client.GetAllCustomers(searchString, false);
                tokens = client.GetAllTokens(searchString, Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID);
            });
            if (customers.Count > 0)
            {
                ListViewItem[] lvItems = new ListViewItem[customers.Count];
                for (int index = 0; index < customers.Count; index++)
                {
                    reservedKeys = tokens.Where(c => c.CustID.Equals(customers[index].SCRnumber) && c.TokenStatus.Equals(0)).Count();
                    activeKeys = tokens.Where(c => c.CustID.Equals(customers[index].SCRnumber) && c.TokenStatus.Equals(1)).Count();
                    deactivatedKeys = tokens.Where(c => c.CustID.Equals(customers[index].SCRnumber) && c.TokenStatus.Equals(2)).Count();
                    ListViewItem item = new ListViewItem(new string[] { customers[index].SCRname, reservedKeys.ToString(), activeKeys.ToString(), deactivatedKeys.ToString() });
                    lvItems[index] = item;
                }
                HardwareKeyListView.Items.AddRange(lvItems);
            }
        }

        private void LoadDetailListView(Object item)
        {
            DetailListView.BeginUpdate();
            PopulateDetailListViewColumns(item);
            PopulateDetailListView(item);
            _lvManager.AutoResizeColumns(DetailListView);
            DetailListView.EndUpdate();
        }

        private void LoadHardwareKeyListView(string searchString, string custName)
        {
            HardwareKeyListView.BeginUpdate();
            HardwareKeyListView.Items.Clear();
            PopulateHardwareKeyListViewColumns(custName);
            PopulateHardwareKeyListView(searchString, custName);
            _lvManager.AutoResizeColumns(HardwareKeyListView);
            HardwareKeyListView.EndUpdate();
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
                DetailListView.Columns.Add("Status");
            }
            else if (selectedObject is ProductProperty)
            {
                DetailListView.Columns.Add("Name");
                DetailListView.Columns.Add("Value");
                DetailListView.Columns.Add("Product Connections");
            }
            else
            {
                DetailListView.Columns.Add("Name");
                DetailListView.Columns.Add("Value");
                DetailListView.Columns.Add("Available");
            }
            _lvManager.ResetListViewColumnSorter(DetailListView);
        }

        /// <summary>
        /// Populates the ListView column headers based upon the selected TreeNode of the License view.
        /// </summary>
        public void PopulateHardwareKeyListViewColumns(string custName)
        {
            HardwareKeyListView.Columns.Clear();
            if (custName.Length > 0)
            {
                //if (!(HardwareKeyListView.Columns.Count == 3))
                //{
                    HardwareKeyListView.Columns.Add("Key Value");                    
                    HardwareKeyListView.Columns.Add("License Server");
                    HardwareKeyListView.Columns.Add("Status");
                //}
            }
            else
            {
                //if (!(HardwareKeyListView.Columns.Count == 4))
                //{
                    HardwareKeyListView.Columns.Add("Customer");
                    HardwareKeyListView.Columns.Add("Reserved");
                    HardwareKeyListView.Columns.Add("Active");
                    HardwareKeyListView.Columns.Add("Deactivated");
                //}
            }
            _lvManager.ResetListViewColumnSorter(HardwareKeyListView);
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

        public void PopulateHardwareKeyListView(string searchString, string custName)
        {
            if (custName.Length > 0)
                PopulateHardwareKeyView(searchString, custName);
            else
                PopulateHardwareKeyCustomerView(searchString);
        }

        public void PopulateDetailListView(LicenseServerProperty licenseData)
        {
            DetailListView.Items.Clear();
            Image tokenImage = ListViewImageList.Images[Enums.GetListViewIconIndex("VALIDATIONTOKENS")];
            ImageToolStripLabel.Size = new Size(tokenImage.Width, tokenImage.Height);
            ImageToolStripLabel.Image = tokenImage;
            DetailListViewToolStripLabel.Text = "Validation Tokens";
            //enable/disable buttons
            dlvAddToolStripButton.Visible = true;
            dlvRemoveToolStripButton.Enabled = false;
            dlvEditToolStripButton.Enabled = false;
            moduleFilterToolStripComboBox.Visible = false;
            ListViewItem lvItem;        
            TokenTable hardwareToken = null;
            foreach (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs properties in licenseData.LicInfo.licVerificationAttribs.TVal.validationTokenList.TVal)
            {
                lvItem = new ListViewItem();
                lvItem.Tag = new ValidationProperty(properties);
                if(properties.tokenType.EVal.Equals(Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID))
                {
                    Service<ICreator>.Use((client) =>
                    {
                        hardwareToken = client.GetTokenByLicenseName(licenseData.Name, (byte)properties.tokenType);
                        if (hardwareToken == null)                       
                            MessageBox.Show(string.Format("Failed to get token status for {0}", properties.tokenValue.TVal), "Load Validation Token List");                        
                        else
                            (lvItem.Tag as ValidationProperty).Status = ((TokenStatus)hardwareToken.TokenStatus).ToString();
                    });
                }
                lvItem.Name = lvItem.Text = properties.tokenType.TVal.ToString();
                lvItem.ImageIndex = Enums.GetListViewIconIndex(lvItem.Text);
                lvItem.SubItems.Add(properties.tokenValue.TVal);
                lvItem.SubItems.Add((lvItem.Tag as ValidationProperty).Status);
                DetailListView.Items.Add(lvItem);
            }            
        }

        public void PopulateDetailListView(ProductProperty productData)
        {
            DetailListView.Items.Clear();
            Image productImage = DetailTreeViewImageList.Images[Enums.GetDetailTreeViewIconIndex(s_CommLink.GetProductBaseName(productData.Name))];
            ImageToolStripLabel.Size = new Size(productImage.Width, productImage.Height);
            ImageToolStripLabel.Image = productImage;
            DetailListViewToolStripLabel.Text = string.Format("{0} Modules", productData.Name);
            dlvAddToolStripButton.Visible = false;
            dlvRemoveToolStripButton.Enabled = false;
            dlvEditToolStripButton.Enabled = false;
            moduleFilterToolStripComboBox.Visible = false;
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

        private void PopulateDetailListView(ProductLicenseProperty plData)
        {
            //clear previous items from storage and current view
            DetailListView.Items.Clear();
            storageListView.Items.Clear();
            //test-dev image missing
            Image productImage = DetailTreeViewImageList.Images[Enums.GetDetailTreeViewIconIndex(s_CommLink.GetProductBaseName(plData.Product.Name))];
            ImageToolStripLabel.Size = new Size(productImage.Width, productImage.Height);
            ImageToolStripLabel.Image = productImage;
            DetailListViewToolStripLabel.Text = string.Format("{0} Modules", plData.Product.Name);
            dlvAddToolStripButton.Visible = false;
            dlvRemoveToolStripButton.Enabled = false;
            dlvEditToolStripButton.Enabled = false;
            moduleFilterToolStripComboBox.Visible = true;
            //need to set plData read-only attributes
            foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs properties in plData.Product.ModuleList.TVal)
            {
                if (properties.contractNumber.TVal.Equals(plData.ID))
                {
                    ListViewItem lvItem = CreateModuleListViewItem(properties, plData.Product.ID, false);
                    if (storageListView.Items.Find(lvItem.Name, false).Count() == 0)
                        storageListView.Items.Add(lvItem);                       
                }
            }
            if (moduleFilterToolStripComboBox.SelectedItem != null)
            {                
                ShowModuleListView(moduleFilterToolStripComboBox.Text);
            }
            else
            {
                moduleFilterToolStripComboBox.SelectedIndex = 0;
                ShowModuleListView("All");
            }
        }

        public ListViewItem CreateModuleListViewItem(Lic_PackageAttribs.Lic_ModuleInfoAttribs module, uint productID, bool bTotal)
        {
            ListViewItem lvItem = DetailListView.Items.Find(s_CommLink.GetModuleName(productID, module.moduleID),false).FirstOrDefault();            
            if (lvItem == null)
            {
                lvItem = new ListViewItem();
                lvItem.Tag = new ModuleProperty(module);
                (lvItem.Tag as ModuleProperty).ProductID = productID;
                lvItem.Name = lvItem.Text = s_CommLink.GetModuleName(productID, module.moduleID);
                lvItem.ImageIndex = Enums.GetListViewIconIndex("MODULE");
                lvItem.SubItems.Add(module.moduleValue.TVal.ToString());
                if (DetailPropertyGrid.SelectedObject is ProductLicenseProperty)
                {
                    TreeNode node;
                    ProductLicenseProperty plData = DetailPropertyGrid.SelectedObject as ProductLicenseProperty;
                    //trial - available is always 0
                    //perm, addon - total value is shared between these
                    string availableUnits = "Unlimited";
                    int unlimitedValue = s_CommLink.GetUnlimitedModuleValue(productID, module.moduleID.TVal);
                    if (plData.Status == ProductLicenseState.Trial)
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
                        if (plData.ParentID != null)
                            node = DetailTreeView.SelectedNode.Parent;
                        plData = node.Tag as ProductLicenseProperty;
                        foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs storedModule in plData.Product.Product.moduleList.TVal)
                        {
                            if (storedModule.moduleID.TVal.Equals(module.moduleID.TVal) &&
                                storedModule.contractNumber.TVal.Equals(plData.ID))
                            {
                                totalValue = storedModule.moduleValue.TVal;
                                break;
                            }
                        }
                        if (node.Nodes.Count > 0)
                        {
                            foreach (TreeNode addonNode in node.Nodes)
                            {
                                plData = addonNode.Tag as ProductLicenseProperty;
                                foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs storedModule in plData.Product.Product.moduleList.TVal)
                                {
                                    if (storedModule.moduleID.TVal.Equals(module.moduleID.TVal) &&
                                        storedModule.contractNumber.TVal.Equals(plData.ID))
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
                        if (module.moduleExpirationDate.TVal.ToLocalTime().CompareTo(CurrentExpirationDate) < 0)
                            lvItem.ForeColor = System.Drawing.Color.Red;
                    }
                    else
                    {
                        if(module.moduleState.TVal == Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msLicensed)
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
        private void SetLicenseServerState(TreeNode currentNode)
        {
            TreeNode licenseNode = null;         
            //if (node.Tag is CustomerProperty)            
            //    customerNode = node;
            if (currentNode.Tag is LicenseServerProperty)
                licenseNode = currentNode;
            else if (currentNode.Tag is ProductProperty)
                licenseNode = currentNode.Parent;
            else if (currentNode.Tag is ProductLicenseProperty)
            {
                if ((currentNode.Tag as ProductLicenseProperty).ParentID != null)
                    licenseNode = currentNode.Parent.Parent.Parent;
                else
                    licenseNode = currentNode.Parent.Parent;                        
            }
            LicenseServerProperty selectedLicense;

            IList<string> updatedLicenses = null;
            IList<string> modifiedLicenses = null;
            Service<ICreator>.Use((client) =>
            {
                updatedLicenses = client.GetUpdatedLicensesByCustomer(_selectedLicenseCustomer);
                modifiedLicenses = client.GetModifiedLicensesByCustomer(_selectedLicenseCustomer);
            });
            foreach (TreeNode node in DetailTreeView.Nodes)
            {               
                if (node.Text != "")
                {
                    selectedLicense = node.Tag as LicenseServerProperty;
                    if (selectedLicense.IsDeactivated)
                    {
                        node.ImageIndex = node.SelectedImageIndex = Enums.GetDetailTreeViewIconIndex("LicenseDeactivated");
                        node.NodeFont = new Font(this.Font, FontStyle.Italic);
                        node.ForeColor = SystemColors.InactiveCaptionText;
                        node.ToolTipText = "";
                    }
                    else
                    {
                        node.ImageIndex = node.SelectedImageIndex = Enums.GetDetailTreeViewIconIndex("License");
                        node.ToolTipText = "";
                        if (modifiedLicenses != null)
                        {
                            if (modifiedLicenses.Contains(selectedLicense.Name))
                            {
                                node.ImageIndex = node.SelectedImageIndex = Enums.GetDetailTreeViewIconIndex("LicenseUpdate");
                                node.ToolTipText = "Changes have been made to license. Generate license packet.";
                            }
                        }
                        if (!selectedLicense.IsStandardLicenseType)
                        {                            //check to see if there are pending changes
                            if (updatedLicenses != null)
                            {
                                if (!updatedLicenses.Contains(selectedLicense.Name))
                                {
                                    node.ImageIndex = node.SelectedImageIndex = Enums.GetDetailTreeViewIconIndex("LicenseResync");
                                    node.ToolTipText = "Standard license updates are available. Resync to apply changes.";
                                }
                            }
                        }
                        if (!(selectedLicense).IsEnabled) //disable key, not enough tokens
                        {
                            node.ImageIndex = node.SelectedImageIndex = Enums.GetDetailTreeViewIconIndex("LicenseDisabled");
                            node.ToolTipText = "Not enough validation tokens are set. Add a new validation token to enable license.";
                        }
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
            List<ProductLicenseTable> plRecords = null;
            ProductProperty selectedProduct = node.Tag as ProductProperty;
            Service<ICreator>.Use((client) => 
            {
                plRecords = client.GetProductLicensesByProduct(node.Parent.Name, s_CommLink.GetProductID(selectedProduct.Name));                
            });
            TreeNode plParent;
            foreach (ProductLicenseTable plt in plRecords)
            {       
                TreeNode plNode = null;
                if (!node.Nodes.ContainsKey(plt.plID))
                {
                    plNode = new TreeNode(plt.plID);
                    plNode.Name = plt.plID;
                    plNode.Tag = new ProductLicenseProperty(plt, selectedProduct, m_Permissions);
                    if (plt.ParentProductLicenseID != null)
                    {
                        plNode.ImageIndex = Enums.GetDetailTreeViewIconIndex("ADDONORDER");
                        plNode.SelectedImageIndex = plNode.ImageIndex;
                        plParent = node.Nodes.Find(plt.ParentProductLicenseID, false).FirstOrDefault();
                        if (plParent != null)
                        {
                            if (!plParent.Nodes.ContainsKey(plNode.Name))
                                plParent.Nodes.Add(plNode);
                        }
                    }
                    else
                    {
                        //if deactivated grey and italic text
                        plNode.ImageIndex = Enums.GetDetailTreeViewIconIndex("ORDER");
                        plNode.SelectedImageIndex = plNode.ImageIndex;
                        node.Nodes.Add(plNode);
                    }
                }
                else
                {
                    plNode = node.Nodes.Find(plt.plID, true).First();
                    plNode.Tag = new ProductLicenseProperty(plt, selectedProduct, m_Permissions);
                }
                if (!(plNode.Tag as ProductLicenseProperty).IsActive)
                {
                    plNode.NodeFont = new Font(this.Font, FontStyle.Italic);
                    plNode.ForeColor = SystemColors.InactiveCaptionText;
                }
            }
            if (node.Nodes.Count > 0)
            {
                foreach (TreeNode orderNode in node.Nodes)
                {
                    if (plRecords.Find(c => c.plID.Equals((orderNode.Name))) == null)
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
                licenseNode.Tag = new LicenseServerProperty(license, m_Permissions);
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
                    productNode.ImageIndex = Enums.GetDetailTreeViewIconIndex(s_CommLink.GetProductBaseName(productName));
                    productNode.SelectedImageIndex = productNode.ImageIndex;
                    productNode.Nodes.Add(new VirtualTreeNode());
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
            }
        }

        private void LoadLicenseView(IList<LicenseTable> licRecords, string selectedName)
        {
            Cursor storedCursor = this.Cursor;
            this.Cursor = Cursors.WaitCursor;
            DetailTreeView.BeginUpdate();
            DetailTreeView.Nodes.Clear();
            if(licRecords.Count > 0)
            {
                TreeNode[] nodes = new TreeNode[licRecords.Count()];
                int index = 0;
                foreach (var license in licRecords.OrderBy(d => d.DestinationID).ThenBy(g => g.GroupID).ThenBy(l => l.LicenseType))
                {
                    TreeNode licNode = new TreeNode(license.LicenseName);
                    licNode.Name = license.LicenseName;
                    licNode.ImageIndex = Enums.GetDetailTreeViewIconIndex("License");
                    licNode.SelectedImageIndex = licNode.ImageIndex;
                    if (!licNode.IsExpanded)
                        licNode.Nodes.Add(new VirtualTreeNode());
                    licNode.Tag = new LicenseServerProperty(license, m_Permissions);
                    nodes[index] = licNode;
                    index++;                
                }
                DetailTreeView.Nodes.AddRange(nodes);
                LicenseViewSplitContainer.Panel2Collapsed = false;
                DetailTreeView.Enabled = true;
            }
            else
            {
                TreeNode emptyNode = new TreeNode("No License Servers Available");
                emptyNode.Name = "Empty";
                emptyNode.ImageIndex = emptyNode.SelectedImageIndex = Enums.GetDetailTreeViewIconIndex("Empty");
                DetailTreeView.Nodes.Add(emptyNode);
                LicenseViewSplitContainer.Panel2Collapsed = true;
            }
            navigateBackToolStripButton.Enabled = true;
            navigateForwardToolStripButton.Enabled = false;
            //selected node should be first node.
            if (DetailTreeView.Nodes.Count > 0 && licRecords.Count() > 0)
            {
                if (selectedName.Length > 0)
                    DetailTreeView.SelectedNode = DetailTreeView.Nodes.Find(selectedName, false).FirstOrDefault();
                else
                    DetailTreeView.SelectedNode = DetailTreeView.Nodes[0];
            }
            else
                DetailPropertyGrid.SelectedObject = null;
            DetailTreeView.EndUpdate();
            this.Cursor = storedCursor;
        }

        private void PopulateCustomersListViewColumns()
        {
            DetailListView.Columns.Clear();
            DetailListView.Columns.Add("Number");
            DetailListView.Columns.Add("Name");
            _lvManager.ResetListViewColumnSorter(CustomersListView);
        }

        private void LoadCustomersListView(IList<CustomerTable> customerRecords)
        {            
            ListViewItem[] lvItems = null;
            //Need list view manager
            CustomersListView.BeginUpdate();
            PopulateCustomersListViewColumns();
            CustomersListView.Items.Clear();
              //group by customer
            lvItems = new ListViewItem[customerRecords.Count()];
            for (int index = 0; index < customerRecords.Count(); index++)
            {
                ListViewItem item = new ListViewItem();
                item.Text = string.Format("{0:X}", customerRecords[index].SCRnumber);
                item.Name = customerRecords[index].SCRname;
                item.SubItems.Add(customerRecords[index].SCRname);
                lvItems[index] = item;
            }
            if (lvItems.Count() > 0)
                this.CustomersListView.Items.AddRange(lvItems);            
            _lvManager.AutoResizeColumns(CustomersListView);
            CustomersListView.EndUpdate();
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

        private void LoadTransactionItems(string licenseName)
        {
            IList<PacketTable> packets = null;
            IList<TransactionTable> transactions = null;
            TransactionListView.BeginUpdate();
            TransactionListView.Items.Clear();
            TransactionListView.Groups.Clear();
            ListViewItem[] lvItems; 
            int index = 0;
            if (GroupToolStripButton.Checked)
                TransactionListView.Sorting = SortOrder.Descending;
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
                    lvItems = new ListViewItem[transactions.Count];
                    for(index = 0; index < transactions.Count; index++)
                    {
                        ListViewItem item = new ListViewItem();
                        item.Text = transactions[index].taDateCreated.ToString();
                        item.SubItems.Add(transactions[index].taDescription);
                        item.SubItems.Add(transactions[index].taUnits);
                        item.SubItems.Add(transactions[index].taUser);
                        item.Group = group;
                        lvItems[index] = item;
                    }
                    TransactionListView.Items.AddRange(lvItems);
                }
                foreach (var packet in packets)
                {
                    transactions = client.GetTransactionsByPacketID(packet.ID).OrderByDescending(c => c.taDateCreated).ToList();;
                    if (transactions.Count > 0)
                    {
                        ListViewGroup group = new ListViewGroup();
                        group.Header = packet.PacketName;
                        TransactionListView.Groups.Add(group);
                        lvItems = new ListViewItem[transactions.Count];
                        for (index = 0; index < transactions.Count; index++)
                        {
                            ListViewItem item = new ListViewItem();
                            item.Text = transactions[index].taDateCreated.ToString();
                            item.SubItems.Add(transactions[index].taDescription);
                            item.SubItems.Add(transactions[index].taUnits);
                            item.SubItems.Add(transactions[index].taUser);
                            item.Group = group;
                            lvItems[index] = item;
                        }
                        TransactionListView.Items.AddRange(lvItems);
                    }
                }
            });
            if (TransactionListView.ShowGroups)
                SetGroupTransactionListViewItemBackColor();
            else
                SetSingleTransactionListViewItemBackColor();
            _lvManager.AutoResizeColumns(TransactionListView);
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
            storageListView.Items.Clear();
            if (packets != null && packets.Count > 0)
            {
                bool b = false;
                foreach (var packet in packets)
                {
                    ListViewItem packetItem = new ListViewItem();
                    packetItem.Text = packet.PacketName;
                    packetItem.Tag = packet;
                    packetItem.ImageIndex = Enums.GetListViewIconIndex((packet.IsVerified ? "VERIFIED" : "UNVERIFIED"));
                    packetItem.StateImageIndex = packetItem.ImageIndex;
                    packetItem.SubItems.Add(packet.DateCreated.ToString());
                    packetItem.SubItems.Add(packet.ExpiredDate.ToShortDateString());
                    packetItem.SubItems.Add(packet.UserName);
                    packetItem.SubItems.Add(packet.VerifiedBy);
                    //add to stored packet list
                    storageListView.Items.Add(packetItem);//.Clone() as ListViewItem);
                }
                storageListView.Items[storageListView.Items.Count - 1].Font = new Font(this.Font, FontStyle.Bold); 
            }
            toolStripComboBox1.SelectedIndex = 0;            
            //needed outside of begin/end update to allow clearing of LicensePacketListView
            ShowPacketListView("");
        }
        //Transaction for License File 
        private void CreateTransaction(TransactionType type, string licenseName, string description, string value)
        {
            Service<ICreator>.Use((client) =>
            {
                LicenseTable selectedLicense = client.GetLicenseByName(licenseName, false);
                if (selectedLicense == null)
                {
                    MessageBox.Show(string.Format("Failed to find license server : {0}", licenseName), "Create Transaction");
                    return;
                }
                TransactionTable newTransaction = new TransactionTable();
                newTransaction.taLicenseID = selectedLicense.ID;
                newTransaction.taType = (Byte)type;
                newTransaction.taDateCreated = DateTime.Now;
                newTransaction.taPacketID = null;
                newTransaction.taDescription = description;
                newTransaction.taUnits = value;
                newTransaction.taUser = WindowsIdentity.GetCurrent().Name;
                client.CreateTransaction(newTransaction);
            });
            SetLicenseServerState(DetailTreeView.SelectedNode);
        }

        //Transaction for Order
        private void CreateTransaction(TransactionType type, ProductLicenseProperty plData, string description, string value)
        {
            Service<ICreator>.Use((client) =>
            {
                ProductLicenseTable selectedOrder = client.GetProductLicense(plData.ID);
                if (selectedOrder == null)
                {
                    MessageBox.Show(string.Format("Failed to find product license : {0}", plData.ID), "Create Transaction");
                    return;
                }
                TransactionTable newTransaction = new TransactionTable();
                newTransaction.taLicenseID = selectedOrder.LicenseID;
                newTransaction.taType = (Byte)type;
                newTransaction.taDateCreated = DateTime.Now;
                newTransaction.taPacketID = null;
                newTransaction.taDescription = description;
                newTransaction.taUnits = value;
                newTransaction.taUser = WindowsIdentity.GetCurrent().Name;
                newTransaction.taOrderID = selectedOrder.ID;
                client.CreateTransaction(newTransaction);
            });
            SetLicenseServerState(DetailTreeView.SelectedNode);
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
            licInfo.TVal.softwareLicType.EVal = (Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType)Enums.GetLicenseServerType(licItem.LicType);
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

        private bool OrderTransactions(ref ProductLicenseTable plData, ProductLicenseProperty productLicense)
        {
            //1) expdate, status, description
            bool bChanged = false;
            if (!plData.plState.Equals((byte)productLicense.Status))
            {
                CreateTransaction(TransactionType.Order,
                                  productLicense,
                                  string.Format("{0}: Product License Status from {1} to {2}", productLicense.ID, ((Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState)plData.plState).ToString(), productLicense.Status.ToString()),
                                  productLicense.Status.ToString());
                plData.plState = (byte)productLicense.Status;
                bChanged = true;
            }
            //product version extends to all orders of the same product 
            if (!plData.ProductVersion.Equals(productLicense.ProductVersion.ToString()))
            {
                CreateTransaction(TransactionType.Order,
                                  productLicense,
                                  string.Format("{0}: Product Version from {1} to {2}", productLicense.ID, productLicense.ProductVersion, productLicense.ProductVersion),
                                  productLicense.ProductVersion.ToString());
                plData.ProductVersion = productLicense.ProductVersion.ToString();
                Service<ICreator>.Use((client) => 
                {
                    IList<ProductLicenseTable> productOrders = client.GetProductLicensesByProduct(productLicense.LicenseName, (int)productLicense.Product.ID);
                    foreach (ProductLicenseTable order in productOrders)
                    {
                        order.ProductVersion = productLicense.ProductVersion.ToString();
                        client.UpdateProductLicense(order);
                    }
                });
            }
            DateTime? storedDT;
            if (plData.ExpirationDate.HasValue)
                storedDT = plData.ExpirationDate.Value.ToLocalTime();
            else
                storedDT = null;
            if (!storedDT.ToString().Equals(productLicense.ExpirationDate.ToString()))
            {                
                string storedValue = "None", value = "None";
                if (productLicense.ExpirationDate.HasValue)
                    value = productLicense.ExpirationDate.Value.ToShortDateString();
                if (plData.ExpirationDate.HasValue)
                    storedValue = plData.ExpirationDate.Value.ToShortDateString();
                CreateTransaction(TransactionType.Order,
                                  productLicense,
                                  string.Format("{0}: Product License Expiration Date {1} to {2}", productLicense.ID, storedValue, value), 
                                  value);
                bChanged = true;
                if (productLicense.ExpirationDate.HasValue)
                    plData.ExpirationDate = productLicense.ExpirationDate.Value.ToUniversalTime();
                else
                    plData.ExpirationDate = productLicense.ExpirationDate;
            }
            if (!plData.Description.Equals(productLicense.Description))
            {
                plData.Description = productLicense.Description;
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
                        if(module.contractNumber.TVal.Equals(selectedOrder.ID))
                            removeList.Add(module);
                    }
                    foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs removeModule in removeList)
                    {
                        product.moduleList.TVal.Remove(removeModule);
                    }
                    //add remove module values to perm modules
                    foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs module in product.moduleList.TVal)
                    {
                        if (module.contractNumber.TVal.Equals(selectedOrder.ParentID))
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
        private void ProductItemTransactions(ref Lic_PackageAttribs.AttribsMemberAttribsClass_Lic_LicenseInfoAttribs licInfo, ProductLicenseProperty plData, bool bOrderChanged)
        {
            //iterate over all products
            string moduleName;
            string licenseName = Lic_LicenseInfoAttribsHelper.GetDisplayLabel(licInfo);
            ProductProperty selectedProduct = plData.Product;
            //product nodes under licenses                          
            foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs prodInfo in licInfo.TVal.productList.TVal)
            {
                //find selectedProduct in stored db productList
                if (prodInfo.productID.TVal.Equals(selectedProduct.ID))
                {
                    if (!prodInfo.productAppInstance.TVal.Equals(selectedProduct.AppInstance))
                    {
                        if (!bOrderChanged)
                            CreateTransaction(TransactionType.Product, plData, string.Format("{0}:{1} - Application Instance from {1} to {2}", plData.ID, selectedProduct.Name, prodInfo.productAppInstance.SVal, selectedProduct.AppInstance.ToString()), selectedProduct.AppInstance.ToString());
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
                                                              plData,
                                                              string.Format("{0}:({1}-{2}) - Value from {3} to {4}", plData.ID, selectedProduct.Name, moduleName, storedModule.moduleValue.TVal.ToString(), mod.moduleValue.TVal.ToString()), 
                                                              mod.moduleValue.TVal.ToString());
                                        storedModule.moduleValue.TVal = mod.moduleValue.TVal;
                                    }
                                    //app instance
                                    if (!storedModule.moduleAppInstance.TVal.Equals(mod.moduleAppInstance.TVal))
                                    {
                                        if (!bOrderChanged)
                                            CreateTransaction(TransactionType.Product,
                                                              plData,
                                                              string.Format("{0}:({1}-{2}) - Application Instance from {3} to {4}", plData.ID, selectedProduct.Name, moduleName, storedModule.moduleAppInstance.ToString(), mod.moduleAppInstance.ToString()), 
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

        private void SearchCurrentView(string searchString)
        {
            if (searchToolStripTextBox.ForeColor == SystemColors.InactiveCaptionText)
                searchToolStripTextBox.Text = "";
            if (MainTabControl.SelectedTab == LicensesTabPage)
            {
                if (CustomersListView.Visible)
                    LoadDBCustomers(searchString, false);
                else
                    LoadDBLicenses(searchString, "", false);
            }
            else if (MainTabControl.SelectedTab == ValidationKeysTabPage)
                LoadHardwareKeyListView(searchString, _selectedHardwareKeyCustomer);                              
        }
        #endregion

        #region Database Methods
        private void LoadDBCustomers(string searchString, bool loadOption)
        {
            IList<CustomerTable> data = null;
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
            loadingCircle1.Visible = true;
            loadingCircle1.SpokeThickness = 6;
            loadingCircle1.InnerCircleRadius = 12;
            loadingCircle1.OuterCircleRadius = 13;
            loadingCircle1.NumberSpoke = 10;
            loadingCircle1.Active = true;
            CustomersListView.Items.Clear();
        }
        //Only called on a backgroundworker 
        private IList<CustomerTable> GetDBCustomers(BackgroundWorker worker, string searchString, bool loadOption)
        {
            IList<CustomerTable> cust = null;
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

        private void OnDBCustomersRetrieved(IList<CustomerTable> data)
        {
            if (data != null)
            {
                LoadCustomersListView(data);
            }
            loadingCircle1.Active = false;
            loadingCircle1.Visible = false;
            if (searchToolStripTextBox.ForeColor == SystemColors.InactiveCaptionText)
                searchToolStripTextBox.Text = "Search";
            newCustMainToolStripBtn.Enabled = true;
            SearchToolStripLabel.Enabled = true;
            searchToolStripTextBox.Enabled = true;
            CustomerToolStrip.Visible = false;
            //restore searchbox text
        }

        private void LoadDBLicenses(string searchString, string selectedName, bool loadOption)
        {
            IList<LicenseTable> data = null;
            using (BackgroundWorker worker = new BackgroundWorker())
            {
                worker.DoWork += ((sender, e) => data = GetDBLicenses(worker, searchString, loadOption));
                //returned from work so this can access ui main thread
                worker.RunWorkerCompleted += ((sender, e) => OnDBLicensesRetrieved(data, selectedName));
                worker.RunWorkerAsync();
            }
            loadingCircle1.Visible = true;
            loadingCircle1.SpokeThickness = 6;
            loadingCircle1.InnerCircleRadius = 12;
            loadingCircle1.OuterCircleRadius = 13;
            loadingCircle1.NumberSpoke = 10;
            loadingCircle1.Active = true;
        }
        //Only called on a backgroundworker 
        private IList<LicenseTable> GetDBLicenses(BackgroundWorker worker, string searchString, bool loadOption)
        {
            IList<LicenseTable> licenses = null;
            Service<ICreator>.Use((client) =>
            {
                try
                {
                    licenses = client.GetLicensesByCustomer(_selectedLicenseCustomer, searchString, loadOption);
                }
                catch (Exception e) { }
            });
            return licenses;
        }

        private void OnDBLicensesRetrieved(IList<LicenseTable> data, string selectedName)
        {
            if (data != null)
            {
                LoadLicenseView(data, selectedName);
            }
            loadingCircle1.Active = false;
            loadingCircle1.Visible = false;
            if (searchToolStripTextBox.ForeColor == SystemColors.InactiveCaptionText)
                searchToolStripTextBox.Text = "Search";
            SearchToolStripLabel.Enabled = true;
            searchToolStripTextBox.Enabled = true;
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
                if (storedPacket == null)
                {
                    return;
                }
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
            SetLicenseServerState(DetailTreeView.SelectedNode);
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

        private void CreateNewCustomer(string custName)
        {
            CustomerTable newCustRec = new CustomerTable();
            newCustRec.SCRname = custName;
            Service<ICreator>.Use((client) =>
            {
                client.CreateCustomer(newCustRec);
                //need to retrieve custRec again for updated scrnumber
                newCustRec = client.GetCustomer(custName, false);
                if (newCustRec == null)
                {
                    MessageBox.Show(string.Format("Failed to create new customer : {0}", custName), "Create New Customer");
                    return;
                }
                DestinationNameTable destRec = new DestinationNameTable();
                destRec.CustID = newCustRec.SCRnumber;
                destRec.DestID = 0;
                destRec.DestName = newCustRec.SCRname;
                client.CreateDestinationName(destRec);
            });
            ListViewItem lvItem = new ListViewItem(string.Format("{0:x}", newCustRec.SCRnumber));
            lvItem.Name = newCustRec.SCRname; 
            lvItem.SubItems.Add(newCustRec.SCRname);
            CustomersListView.Items.Add(lvItem);
            lvItem.Selected = true;
            CustomersListView.EnsureVisible(CustomersListView.Items.IndexOf(lvItem));
        }

        //    //currently on license file node
        private void newProductLicenseToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AddProductLicense();
        }

        private void newCustMainToolStripBtn_Click(object sender, EventArgs e)
        {
            //createcustomerentity
            //createcustomernode
            using (ItemNameDialog newCustDlg = new ItemNameDialog("Create New Customer"))
            {
                CustomerDialogData custData = new CustomerDialogData();
                if (newCustDlg.ShowDialog(this, custData) == DialogResult.OK)
                {
                    CreateNewCustomer(newCustDlg.Name);
                }
            }            
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
                HardwareKeyDialogData data = new HardwareKeyDialogData();
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                {
                    //LoadHardwareKeyListView(null);
                }
            }
        }

        private void DetailListView_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            //product level            
            switch (DetailTreeView.SelectedNode.Level)
            {
                case 0: //License
                    dlvAddToolStripButton.Visible = true;
                    dlvEditToolStripButton.Enabled = e.IsSelected;
                    //need to disable remove if ttlicensecode is selected.
                    dlvRemoveToolStripButton.Enabled = e.IsSelected & (e.Item.Tag as ValidationProperty).IsAllowedRemoveToken;
                    break;
                case 1: //Product
                    //initialize tooltip information for DetailListView - Modules
                    ModuleProperty selectedModule = e.Item.Tag as ModuleProperty;
                    //retrieve order data using information from selected item
                    string information = "", expirationDate = "";
                    ProductProperty product = DetailPropertyGrid.SelectedObject as ProductProperty;
                    foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs module in product.ModuleList.TVal)
                    {
                        if (module.moduleID.TVal.Equals(selectedModule.ID))
                        {
                            if (module.moduleExpirationDate.TVal.Equals(new DateTime(1900, 1, 1)))
                                expirationDate = "None";
                            else
                                expirationDate = module.moduleExpirationDate.TVal.ToLocalTime().ToShortDateString();
                            information += string.Format("Order Number - {0}, Expiration Date - {1}\n", module.contractNumber.TVal, expirationDate);
                        }
                    }
                    e.Item.ToolTipText = information;
                    break;
                case 3: //Product License, Add On License
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
            selectedLicense = DetailPropertyGrid.SelectedObject as LicenseServerProperty;
            ResyncSelectedLicense(selectedLicense);
            //LoadCustomerNode(DetailTreeView.SelectedNode.Parent);
            LoadLicenseNode(DetailTreeView.SelectedNode);
            SetLicenseServerState(DetailTreeView.SelectedNode);
            this.Cursor = storedCursor;
        }

        //DetailTreeView selected node contains a LicenseServerProperty object
        private void existingProductLicenseToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (ProductLicenseSelectionForm dlg = new ProductLicenseSelectionForm(s_CommLink))
            {
                ProductLicenseSelectionData data = new ProductLicenseSelectionData(DetailPropertyGrid.SelectedObject as LicenseServerProperty);                               
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                {
                    AddExistingProductLicense(data.SelectedLicense, data.ProductLicenseSelectionList);
                    DetailTreeView.BeginUpdate();
                    DetailTreeView.SelectedNode.Nodes.Clear();   
                    LoadLicenseNode(DetailTreeView.SelectedNode);
                    DetailTreeView.SelectedNode.Expand();
                    DetailTreeView.EndUpdate();      
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
                case LicenseServerType.Failover:
                    FailoverUpdateProductLicense(selectedLicense);
                    break;
                case LicenseServerType.DisasterRecovery:
                case LicenseServerType.TestDevelopment:
                    UpdateExistingProductLicense(selectedLicense);
                    break;
                default:
                    break;
            }
        }

        private void newProductLicenseMainToolStripDropDownBtn_Click(object sender, EventArgs e)
        {
            AddProductLicense();
        }

        private void existingProductLicenseMainToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (ProductLicenseSelectionForm dlg = new ProductLicenseSelectionForm(s_CommLink))
            {
                ProductLicenseSelectionData data = new ProductLicenseSelectionData(DetailPropertyGrid.SelectedObject as LicenseServerProperty);
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                {
                    AddExistingProductLicense(data.SelectedLicense, data.ProductLicenseSelectionList);
                    DetailTreeView.BeginUpdate();
                    DetailTreeView.SelectedNode.Nodes.Clear();
                    LoadLicenseNode(DetailTreeView.SelectedNode);
                    DetailTreeView.SelectedNode.Expand();
                    DetailTreeView.EndUpdate();
                }
            }
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

        private void AddProductLicense()
        {
            //show contract dialog
            int plIndex = 0;
            Lic_PackageAttribs licPackage = new Lic_PackageAttribs();
            ProductLicenseProperty plProperty = new ProductLicenseProperty();
            string licenseName = DetailTreeView.SelectedNode.Name;
            Service<ICreator>.Use((client) =>
            {
                plIndex = client.GetNextProductLicenseIndex(licenseName);
            });
            plProperty.LicenseName = licenseName;
            plProperty.Index = plIndex;
            plProperty.Permissions = m_Permissions;
            //license file already created
            //display settings form for product connections
            using (LicenseInfoForm dlg = new LicenseInfoForm("Create New Product License", ref s_CommLink))
            {
                ProductLicenseDialogData data = new ProductLicenseDialogData(plProperty, DetailPropertyGrid.SelectedObject as LicenseServerProperty);
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                {
                    //create orderNode
                    TreeNode newProductNode = CreateProductNode(data.ProductLicense);
                    //use overall product appinstance from database                    
                    data.ProductLicense.Product = (newProductNode.Tag as ProductProperty);
                    TreeNode plNode = CreateProductLicenseNode(data.ProductLicense);

                    TreeNode productNode = DetailTreeView.SelectedNode.Nodes.Find(data.ProductLicense.Product.Name, false).FirstOrDefault();
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
                    if (data.ProductLicense.ParentID == null)
                        productNode.Nodes.Add(plNode);

                    else
                    {
                        //add to parent order node
                        TreeNode parentProductLicenseNode = DetailTreeView.SelectedNode.Nodes.Find(data.ProductLicense.ParentID, true).FirstOrDefault();
                        parentProductLicenseNode.Nodes.Add(plNode);
                    }
                    DetailTreeView.SelectedNode = plNode;
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

        private void addProductLicenseMainToolStripDropDownBtn_ButtonClick(object sender, EventArgs e)
        {            
            if(newProductLicenseMainToolStripMenuItem.Enabled)
                AddProductLicense();
        }

        private void addProductLicenseMainToolStripDropDownBtn_DropDownOpening(object sender, EventArgs e)
        {
            EnableProductLicenseMainToolStripDropDownMenuItems();
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
                    ShowEditDialog(DetailPropertyGrid.SelectedObject, DetailListView.SelectedIndices);                
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
            if (DetailPropertyGrid.SelectedObject is LicenseServerProperty)
                CreateNewValidationToken(DetailPropertyGrid.SelectedObject as LicenseServerProperty);
        }

        private void dlvRemoveToolStripButton_Click(object sender, EventArgs e)
        {
            if (DetailPropertyGrid.SelectedObject is LicenseServerProperty)   //token remove
                dlvRemoveToken(DetailPropertyGrid.SelectedObject as LicenseServerProperty);
            else                //module remove
                dlvRemoveModule(DetailPropertyGrid.SelectedObject as ProductLicenseProperty);
        }

        private void dlvEditToolStripButton_Click(object sender, EventArgs e)
        {
            ShowEditDialog(DetailPropertyGrid.SelectedObject, DetailListView.SelectedIndices);
        }

        private void expandToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DetailTreeView.SelectedNode.Expand();
        }

        private void collapseToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DetailTreeView.SelectedNode.Collapse();
        }

        private void SearchToolStripLabel_Click(object sender, EventArgs e)
        {
            string searchString = "";
            if (searchToolStripTextBox.ForeColor != SystemColors.InactiveCaptionText)
                searchString = searchToolStripTextBox.Text;            
            SearchCurrentView(searchString);
        }

        private void searchToolStripTextBox_Enter(object sender, EventArgs e)
        {
            if (searchToolStripTextBox.ForeColor == SystemColors.InactiveCaptionText)
            {
                searchToolStripTextBox.Text = "";
                searchToolStripTextBox.Font = new Font(this.Font, FontStyle.Regular);
                searchToolStripTextBox.ForeColor = SystemColors.WindowText;
            }
        }

        private void searchToolStripTextBox_Leave(object sender, EventArgs e)
        {
            if (searchToolStripTextBox.Text.Length == 0)
            {
                searchToolStripTextBox.Text = "Search";
                searchToolStripTextBox.Font = new Font(this.Font, FontStyle.Italic);
                searchToolStripTextBox.ForeColor = SystemColors.InactiveCaptionText;
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
                    LoadDetailListView(DetailPropertyGrid.SelectedObject);
                else if (selectedTab == TransactionsTabPage)
                    LoadTransactionItems(m_CurrentLicenseName);
                else if (selectedTab == PacketTabPage)
                    LoadPacketItems(m_CurrentLicenseName);
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

        private bool IsModuleFiltered(ModuleProperty module, string filterType)
        {
            switch(filterType)
            {
                case "NonDefault":
                    if (!s_CommLink.IsDefaultModule((DetailPropertyGrid.SelectedObject as ProductLicenseProperty).Product.ID, module.Name)) return true;
                    break;
                case "Licensed":
                    if (module.Units > 0) return true;
                    break;
                case "UnLicensed":
                    if (module.Units == 0) return true;
                    break;
                case "All":
                    return true;
                default: break;
            }
            return false;
        }

        private void ShowModuleListView(string type)
        {
            DetailListView.BeginUpdate();
            DetailListView.Items.Clear();
            foreach (ListViewItem lvItem in storageListView.Items)
            {
                if (IsModuleFiltered(lvItem.Tag as ModuleProperty, type))
                    DetailListView.Items.Insert(0, lvItem.Clone() as ListViewItem);
            }
            _lvManager.AutoResizeColumns(DetailListView);
            DetailListView.EndUpdate();
        }

        private void moduleFilterToolStripComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (moduleFilterToolStripComboBox.SelectedIndex)
            {
                case 0:
                    ShowModuleListView("All");
                    break;
                case 1:
                    ShowModuleListView("Licensed");
                    break;
                case 2:
                    ShowModuleListView("UnLicensed");
                    break;
                case 3:
                    ShowModuleListView("NonDefault");
                    break;
                default: break;
            }
        }

        private void HardwareKeyListView_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            if(_selectedHardwareKeyCustomer.Length > 0)            
                EnableHardwareKeyView();
        }

        private void HardwareKeyListView_ColumnClick(object sender, ColumnClickEventArgs e)
        {
            _lvManager.SetSortIndexColumn(HardwareKeyListView.Handle, e.Column);
            // Perform the sort with these new sort options.
            HardwareKeyListView.Sort();
        }

        private void navigateBackToolStripButton_Click(object sender, EventArgs e)
        {
            if (MainTabControl.SelectedTab == LicensesTabPage)
            {
                CustomersListView.Visible = true;
                navigateBackToolStripButton.Enabled = false;
                navigateForwardToolStripButton.Enabled = true;
                LicensesTabPage.Text = "Licenses";
                CustomerToolStrip.Visible = false;
            }
            else
            {
                //load customer view
                _selectedHardwareKeyCustomer = "";
                SearchCurrentView("");
                EnableHardwareCustomerView();
                ValidationKeysTabPage.Text = "Validation Keys";
            }
        }

        private void HardwareKeyListView_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            if (!(_selectedHardwareKeyCustomer.Length > 0))
            {
                // Do a hit test for the current mouse position
                ListViewHitTestInfo hitInfo = HardwareKeyListView.HitTest(e.Location);
                // Test to see if the selected item and the hit test item are the same.
                if (HardwareKeyListView.SelectedItems.Count > 0)
                {
                    ListViewItem item = HardwareKeyListView.SelectedItems[0];
                    if (item.Equals(hitInfo.Item))
                    {
                        _selectedHardwareKeyCustomer = item.Text;                        
                        //launch edit dialog
                        LoadHardwareKeyListView("", _selectedHardwareKeyCustomer);
                        EnableHardwareKeyView();
                        //ValidationKeysTabPage.Text = string.Format("Hardware Keys - [{0}]", _selectedHardwareKeyCustomer);
                    }
                }
            }
        }

        private void seekKeyToolStripButton_Click(object sender, EventArgs e)
        {
            bool bKeyFound = false;
            string[] keyName;
            CustomerTable custRec = null;
            IList<SolimarLicenseProtectionKeyInfo> pkeyList = null;
            int index = 0;
            Service<ICreator>.Use((client) => 
            {
                pkeyList = client.KeyEnumerate();
                if (pkeyList.Count > 0)
                {
                    foreach(SolimarLicenseProtectionKeyInfo attachedKey in pkeyList)
                    {
                        if(attachedKey.IsKeyTypeVerification())
                        {
                            bKeyFound = true;
                            keyName = attachedKey.keyName.Split("-".ToCharArray());
                            //hex to decimal value
                            custRec = client.GetCustomer(Int32.Parse(keyName[0], System.Globalization.NumberStyles.HexNumber).ToString(), false);
                            if (custRec == null)
                            {
                                MessageBox.Show("Failed to seek attached key", "Seek Hardware Key");
                                break;
                            }
                            LoadHardwareKeyListView("", custRec.SCRname);
                            index = HardwareKeyListView.Items.IndexOfKey(attachedKey.keyName);
                            _selectedHardwareKeyCustomer = custRec.SCRname;
                            break;
                        }
                    }
                }             
            });
            if (!bKeyFound)
                MessageBox.Show("No hardware keys attached", "Seek Error", MessageBoxButtons.OK);
            else
            {
                HardwareKeyListView.Focus();
                HardwareKeyListView.Items[index].Selected = true;
            }
        }

        private void EnableHardwareKeyView()
        {
            navigateBackToolStripButton.Enabled = true;
            reserveHardwareKeyToolStripButton.Enabled = true;
            searchToolStripTextBox.Enabled = false;
            SearchToolStripLabel.Enabled = false;
            deactivateHardwareKeyToolStripButton.Enabled = false;
            activateHardwareKeyToolStripButton.Enabled = false;
            if (HardwareKeyListView.SelectedItems.Count > 0)
            { 
                deactivateHardwareKeyToolStripButton.Enabled = true;
                if (HardwareKeyListView.SelectedItems[0].SubItems[2].Text != "Active")
                    activateHardwareKeyToolStripButton.Enabled = true;
            }
            ValidationKeyToolStrip.Visible = true;
            ValidationKeyCustomerLabel.Text = _selectedHardwareKeyCustomer;
        }

        private void EnableHardwareCustomerView()
        {
            searchToolStripTextBox.Enabled = true;
            SearchToolStripLabel.Enabled = true;
            navigateBackToolStripButton.Enabled = false;
            navigateForwardToolStripButton.Enabled = false;
            seekKeyToolStripButton.Enabled = true;
            reserveHardwareKeyToolStripButton.Enabled = false;
            deactivateHardwareKeyToolStripButton.Enabled = false;
            activateHardwareKeyToolStripButton.Enabled = false;
            ValidationKeyToolStrip.Visible = false;  
        }

        private void reserveHardwareKeyToolStripButton_Click(object sender, EventArgs e)
        {
            string keyName = "";
            uint nextKeyValue;
            CustomerTable custRec = null;
            Service<ICreator>.Use((client) =>
            {
                custRec = client.GetCustomer(_selectedHardwareKeyCustomer, false);
                if (custRec == null)
                {
                    MessageBox.Show(string.Format("Failed to find customer : {0}", _selectedHardwareKeyCustomer), "Reserve Hardware Key");
                    return;
                }
                nextKeyValue = client.GetNextHardwareTokenValue((uint)custRec.SCRnumber);
                keyName = string.Format("{0:x4}-{1:x4}",custRec.SCRnumber,nextKeyValue);
                TokenTable newToken = new TokenTable();
                newToken.CustID = custRec.SCRnumber;
                newToken.TokenType = (byte)Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID;                
                newToken.TokenValue = keyName;
                newToken.TokenStatus = (byte)TokenStatus.Reserved;
                newToken.LicenseID = -1;
                client.CreateToken(newToken);
            });
            PopulateHardwareKeyListView("", _selectedHardwareKeyCustomer);
            HardwareKeyListView.Focus();
            HardwareKeyListView.Items[HardwareKeyListView.Items.IndexOfKey(keyName)].Selected = true;
        }

        private void deactivateHardwareKeyToolStripButton_Click(object sender, EventArgs e)
        {
            if (HardwareKeyListView.SelectedItems.Count > 0)
            {   //deactivated or reserved
                int selectedIndex = HardwareKeyListView.SelectedItems[0].Index;
                string keyValue = HardwareKeyListView.SelectedItems[0].Text;
                string keyStatus = HardwareKeyListView.SelectedItems[0].SubItems[2].Text;
                Cursor storedCursor = this.Cursor;
                this.Cursor = Cursors.WaitCursor;
                Service<ICreator>.Use((client) =>
                {
                    TokenTable selectedToken = client.GetHardwareTokenByKeyValue(keyValue);
                    if (selectedToken == null)
                    {
                        MessageBox.Show(string.Format("Failed to find hardware key : {0} in database", keyValue), "Deactivate Hardware Key");
                        return;
                    }
                    if (keyStatus == TokenStatus.Deactivated.ToString() ||
                        keyStatus == TokenStatus.Reserved.ToString())
                    {
                        if (selectedToken.LicenseID > 0)
                        {
                            string licenseName = HardwareKeyListView.SelectedItems[0].SubItems[1].Text;
                            MessageBox.Show(string.Format("Please remove from license server : {0} before deleting hardware key - {1}.", licenseName, keyValue), "Remove Hardware Key");
                            return;
                        }
                        client.DeleteToken(selectedToken);
                        HardwareKeyListView.Items.RemoveByKey(keyValue);
                        if (HardwareKeyListView.Items.Count > 0)
                        {
                            if (selectedIndex == 0)
                                HardwareKeyListView.Items[selectedIndex].Selected = true;
                            else
                                HardwareKeyListView.Items[selectedIndex - 1].Selected = true;
                        }
                    }
                    else //active
                    {                        
                        IList<SolimarLicenseProtectionKeyInfo> pKeyList = client.KeyEnumerate();
                        bool bFound = false;
                        foreach(SolimarLicenseProtectionKeyInfo attachedKey in pKeyList)
                        {
                            if (attachedKey.keyName.Equals(keyValue))
                            {
                                bFound = true;
                                break;
                            }
                        }
                        bFound = true;
                        if (bFound)
                        {
                            client.KeyFormat(keyValue);
                            selectedToken.TokenStatus = (byte)TokenStatus.Deactivated;
                            client.UpdateToken(selectedToken);
                            HardwareKeyListView.SelectedItems[0].SubItems[2].Text = TokenStatus.Deactivated.ToString();
                        }
                        else
                        {
                            MessageBox.Show(string.Format("Hardware Key : {0} is not attached to {1}", keyValue, m_ServerList[0]),
                                            "Deactivation Error",
                                            MessageBoxButtons.OK, 
                                            MessageBoxIcon.Error);
                        }
                    }
                });                
                this.Cursor = storedCursor;
            }
        }

        private void activateHardwareKeyToolStripButton_Click(object sender, EventArgs e)
        {
            int selectedIndex = HardwareKeyListView.SelectedItems[0].Index;
            string keyValue = HardwareKeyListView.SelectedItems[0].Text;
            string unintializedKeyValue = "";
            string[] keyName = keyValue.Split("-".ToCharArray());
            Cursor storedCursor = this.Cursor;
            this.Cursor = Cursors.WaitCursor;
            Service<ICreator>.Use((client) =>
            {
                IList<SolimarLicenseProtectionKeyInfo> pKeyList = client.KeyEnumerate();
                bool bFound = false;
                foreach (SolimarLicenseProtectionKeyInfo attachedKey in pKeyList)
                {
                    if (attachedKey.IsKeyTypeUninitialized())
                    {
                        bFound = true;
                        unintializedKeyValue = attachedKey.keyName;
                        break;
                    }
                }
                if (bFound)
                {
                    TokenTable selectedToken = null;
                    selectedToken = client.GetHardwareTokenByKeyValue(keyValue);
                    selectedToken.TokenStatus = (byte)TokenStatus.Active;
                    client.UpdateToken(selectedToken);
                    client.KeyProgramVerification(unintializedKeyValue,
                                                  Int32.Parse(keyName[0], System.Globalization.NumberStyles.HexNumber),
                                                  Int32.Parse(keyName[1], System.Globalization.NumberStyles.HexNumber));
                    HardwareKeyListView.SelectedItems[0].SubItems[2].Text = TokenStatus.Active.ToString();
                }
                else
                {
                    MessageBox.Show(string.Format("Failed to activate hardware key : {0}. No uninitialized keys are attached to {1}",keyValue, m_ServerList[0]),
                                    "Activation Error",
                                    MessageBoxButtons.OK,
                                    MessageBoxIcon.Error);
                }               
            });
            this.Cursor = storedCursor;
        }

        private void MainTabControl_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (MainTabControl.SelectedTab == LicensesTabPage)
            {
                ResetMainToolStripMenu();
                SearchCurrentView("");
                splitContainer2.Panel1.Controls.Clear();                   
                loadingCircle1.Parent = splitContainer2.Panel1;
                MainTabControl.Parent = splitContainer2.Panel1;
            }
            else if (MainTabControl.SelectedTab == ValidationKeysTabPage)
            {
                _selectedHardwareKeyCustomer = _selectedLicenseCustomer;
                ResetMainToolStripMenu();
                SearchCurrentView("");
                if (_selectedHardwareKeyCustomer.Length > 0)
                    EnableHardwareKeyView();
                else
                    EnableHardwareCustomerView();
                loadingCircle1.Parent = splitContainer2.Panel1;
            }
            else
            {
                if (MainTabControl.SelectedTab == ReportTabPage)
                {
                    //LoadReportListView();
                    //MainSplitContainer.Panel2.Controls.Clear();
                    //panel1.Parent = MainSplitContainer.Panel2;
                    loadingCircle1.Parent = splitContainer2.Panel1;
                    //ReportListView.Parent = splitContainer2.Panel1;
                }
            }
        }

        private void CustomersListView_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            // Do a hit test for the current mouse position
            ListViewHitTestInfo hitInfo = CustomersListView.HitTest(e.Location);
            // Test to see if the selected item and the hit test item are the same.
            if (CustomersListView.SelectedItems.Count > 0)
            {
                ListViewItem item = CustomersListView.SelectedItems[0];
                if (item.Equals(hitInfo.Item))
                {
                    _selectedLicenseCustomer = item.SubItems[1].Text;
                    CustomerToolStripLabel.Text = _selectedLicenseCustomer;
                    CustomerToolStrip.Visible = true;                    
                    CustomersListView.Visible = false;      
                    LoadDBLicenses("", "", false);
                }
            }
        }

        private void CustomersListView_MouseUp(object sender, MouseEventArgs e)
        {
            renameCustomerToolStripMenuItem.Enabled = false;
            deleteCustomerToolStripMenuItem.Enabled = false;
            //needs to handle validation properties also
            if (e.Button == MouseButtons.Right)
            {
                ListViewItem item = CustomersListView.GetItemAt(e.X, e.Y);
                if (item != null)
                    renameCustomerToolStripMenuItem.Enabled = true;
                customerContextMenuStrip.Show(CustomersListView, e.X, e.Y);
            }            
        }

        private void customerToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (ItemNameDialog newCustDlg = new ItemNameDialog("Create New Customer"))
            {
                CustomerDialogData custData = new CustomerDialogData();
                if (newCustDlg.ShowDialog(this, custData) == DialogResult.OK)
                {
                    CreateNewCustomer(newCustDlg.Name);
                }
            }  
        }

        private void deleteCustomerToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //confirm and delete customer and associated licenses
        }

        private void renameCustomerToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (CustomersListView.SelectedItems.Count > 0)
            {
                ListViewItem item = CustomersListView.SelectedItems[0];
                //show edit box
                using (ItemNameDialog renameCustDlg = new ItemNameDialog("Rename Customer"))
                {
                    CustomerDialogData custData = new CustomerDialogData(item.Name);
                    if (renameCustDlg.ShowDialog(this, custData) == DialogResult.OK)
                    {   //change made, update
                        if (item.Name != renameCustDlg.Name)
                        {
                            Service<ICreator>.Use((client) =>
                            {
                                CustomerTable custRec = client.GetCustomer(item.Name, true);
                                custRec.SCRname = renameCustDlg.Name;
                                client.UpdateCustomer(custRec);
                                DestinationNameTable destRec = client.GetDestinationName(custRec.SCRnumber, 0);
                                if (destRec != null)
                                {
                                    destRec.DestName = custRec.SCRname;
                                    client.UpdateDestinationName(destRec);
                                }
                            });
                            item.SubItems[1].Text = item.Name = renameCustDlg.Name;
                        }
                        item.Selected = true;
                        CustomersListView.Focus();
                    }
                }
            }
        }

        private void navigateForwardToolStripButton_Click(object sender, EventArgs e)
        {
            if (MainTabControl.SelectedTab == LicensesTabPage)
            {
                CustomersListView.Visible = false;
                CustomerToolStrip.Visible = true;
                LoadDBLicenses("", "", false);
                navigateBackToolStripButton.Enabled = true;
                navigateForwardToolStripButton.Enabled = false;               
                CustomerToolStripLabel.Text = _selectedLicenseCustomer;
            }
            else
            {
                SearchCurrentView(_selectedHardwareKeyCustomer);
                if (_selectedHardwareKeyCustomer.Length > 0)
                    EnableHardwareKeyView();
                else
                    EnableHardwareCustomerView();                
            }
        }

        private void CustomersListView_ColumnClick(object sender, ColumnClickEventArgs e)
        {
            _lvManager.SetSortIndexColumn(CustomersListView.Handle, e.Column);
            // Perform the sort with these new sort options.
            CustomersListView.Sort();
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (AboutDialog dlg = new AboutDialog())
            {
                int majorVer = 0;
                int minorVer = 0;
                int buildVer = 0;
                dlg.SetCreatorVersion(string.Format("{0}.{1}.{2}", VersionInfo.MAJOR_REVISION_NUMBER, VersionInfo.MINOR_REVISION_NUMBER, VersionInfo.BUILD_NUMBER));
                //try
                //{
                //    s_CommLink.GetVersionLicenseServer(s_CommLink.ServerName, ref majorVer, ref minorVer, ref buildVer);
                //}
                //catch (COMException)
                //{
                //    majorVer = 0;
                //    minorVer = 0;
                //    buildVer = 0;
                //}
                //dlg.SetServerVersion(string.Format("{0}.{1}.{2}",majorVer,minorVer,buildVer));
                if (dlg.ShowDialog() == DialogResult.OK)
                {
                }
            }
        }

        private void updateVersionsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (ProductConfigurationDialog dlg = new ProductConfigurationDialog())
            {
                if (dlg.ShowDialog() == DialogResult.OK)
                {
                }
            }
        }

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
