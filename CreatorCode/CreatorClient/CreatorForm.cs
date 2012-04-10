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
using System.Globalization;
using System.Reflection;
using System.Drawing.Printing;
using System.Drawing.Imaging;
using System.ServiceProcess;

/*
 * TODO : 
 *        - Create custom filters to retrieve lists.
 *        - Improve look of interface
 *        - setup concurrency
 *        - figure out way to display tree with children without taking hit of load time
 */

//given a List
//MyList.ForEach(name => listView1.Columns.Add(name));
//var myList = new List<string>() { "A", "B", "C" };  
namespace Client.Creator
{
    public partial class CreatorForm : Form
    {
        #region Fields
        //private Dictionary<string, bool> m_TreeState;
        private ListViewMgr _lvManager; //manages standard listview functions
        private ListView storageListView; //internal listview to store full contents 
        public static CommunicationLink s_CommLink; //link to license manager/server functions
        private PermissionsTable m_Permissions; //table to hold permissions
        private string m_selectedDirectory; //last selected directory for packet
        private List<String> m_ServerList;  //list of valid servers stored on user system
        private string m_searchString = string.Empty; //search string for filtering view
        private string m_CurrentLicenseName = string.Empty;   //current active license server        
        //used by hardware key view
        private string _selectedHardwareKeyCustomer = string.Empty;   //current active hardware key customer
        private Customer _selectedLicenseCustomer;  //current active license customer
        private string _selectedTreeNodeKey = string.Empty;   // current active tree node
        private ReportManager _reportManager;   //manages report functions
        private TreeNode _copyNode; //node to copy for report tree view
        private Bitmap memoryImage; //used to hold screen capture for printing
        #endregion

        #region Properties

        public DateTime CurrentExpirationDate
        {
            get { return new DateTime(DateTime.Today.Year, DateTime.Today.Month, DateTime.Today.Day, 10, 0, 0); }
        }

        public bool LicenseServerAvailable
        {
            get
            {
                ServiceController sc = new ServiceController(AppConstants.LicenseServerName);
                bool bLicenseServer = false;
                try
                {
                    bLicenseServer = (sc.Status != null);
                }
                catch (Exception ex)
                {
                }
                return bLicenseServer;
            }
        }

        #endregion

        #region Constructor
        public CreatorForm()
        {
            this.Font = SystemFonts.MessageBoxFont;
            InitializeComponent();
            m_ServerList = new List<String>();
            s_CommLink = new CommunicationLink();
            storageListView = new ListView();
            _selectedLicenseCustomer = new Customer();
            _reportManager = new ReportManager(s_CommLink);
            StaticImageList.Instance.GlobalImageList = CreatorImageList;    

            _lvManager = new ListViewMgr();
            _lvManager.SetListViewColumnSorter(DetailListView);
            _lvManager.SetListViewColumnSorter(TransactionListView);
            _lvManager.SetListViewColumnSorter(CustomersListView);
            _lvManager.SetListViewColumnSorter(LicensePacketListView);
            _lvManager.SetListViewColumnSorter(HardwareKeyListView);
            _lvManager.SetListViewColumnSorter(ReportListView);
            ResetMainToolStripMenu();
             
            //set to make read-only items not greyed-out            
        }
        #endregion

        #region Enable/Disable ContextMenu

        private void ResetContextMenu(ToolStripItemCollection items)
        {
            foreach (ToolStripItem tsm in items)
            {
                tsm.Visible = false;
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
            printToolStripButton.Enabled = true; //print button always available
        }

        //CONTACT DB : 4
        private void EnableLicenseContextMenu(bool value)
        {
            bool bLicenseServerEnabled, bHasActiveHardwareToken;
            LicenseServer licData = null;
            if (DetailTreeView.SelectedNode != null)
                licData = DetailTreeView.SelectedNode.Tag as LicenseServer;
            else
                DetailTreeView.SelectedNode = DetailTreeView.Nodes[0];
            expandAllToolStripMenuItem.Visible = !DetailTreeView.SelectedNode.IsExpanded;
            expandAllToolStripMenuItem.Enabled = (DetailTreeView.SelectedNode.Nodes.Count == 0) ? false : true;
            collapseToolStripMenuItem.Visible = DetailTreeView.SelectedNode.IsExpanded;
            collapseToolStripMenuItem.Enabled = DetailTreeView.SelectedNode.IsExpanded;
            //add license items
            newLicenseToolStripMenuItem.Visible = true;
            newLicenseToolStripMenuItem.Enabled = (DestNameComboBox.Items.Count > 2) && m_Permissions.pt_create_modify_key.Value;
            newProductLicenseToolStripMenuItem.Visible = true; 
            lcmToolStripSeparator3.Visible = true;
            //find item
            findToolStripMenuItem.Visible = findToolStripMenuItem.Enabled = true;
            //separator item
            lcmToolStripSeparator1.Visible = true;
            //generate license item
            createPacketFileToolStripMenuItem.Visible = true;
            validationTokensToolStripMenuItem.Visible = true;
            //separator item
            lcmToolStripSeparator4.Visible = true;
            //also need to account for admin allowed extra new orders 
            
            if (licData != null)
            {   
                bool bLockStatus = licData.LockStatus; // DB - 1
                bLicenseServerEnabled = licData.IsEnabled; // DB - 2
                if (m_Permissions.pt_extension_pwd.Value && m_Permissions.pt_version_pwd.Value)
                {                    //license items
                    checkOutToolStripMenuItem.Visible = true;
                    checkOutToolStripMenuItem.Enabled = !licData.LockedByAnyUser();
                    checkInToolStripMenuItem.Visible = true;                    
                    //allow checkin for current user or admin user
                    checkInToolStripMenuItem.Enabled = licData.LockedByCurrentUser() || licData.LockedByAdmin();    
                    lcmToolStripSeparator2.Visible = true;    
                }
                if (m_Permissions.pt_create_modify_key.Value)
                {
                    cloneToolStripMenuItem.Visible = cloneToolStripMenuItem.Enabled = true;
                    if (!licData.IsActive)
                    {
                        deleteToolStripMenuItem.Visible = true;
                        //if (licData.HasLostToken) //DB 3
                        if(!licData.IsActive)
                            reactivateToolStripMenuItem.Visible = true;
                    }
                    else
                        deactivateToolStripMenuItem.Visible = true;

                    if (licData.LockedByAnyUser())
                    {
                        bHasActiveHardwareToken = licData.HasActiveHardwareToken; // DB - 4
                        deleteToolStripMenuItem.Enabled = true;
                        newProductLicenseToolStripMenuItem.Enabled = licData.IsActive;
                        validationTokensToolStripMenuItem.Enabled = licData.IsActive;
                        newTokenToolStripMenuItem.Enabled = !bLicenseServerEnabled;
                        clearToolStripMenuItem.Enabled = bLicenseServerEnabled && !bHasActiveHardwareToken;
                        markLostToolStripMenuItem.Enabled = bHasActiveHardwareToken;
                        deactivateToolStripMenuItem.Enabled = true;
                        reactivateToolStripMenuItem.Enabled = true;
                    }
                }
                //disallow if active LS and has active PL without tokens 
                if (m_Permissions.pt_version_pwd.Value && !(licData.IsActive && licData.HasActiveProductLicense() && !bLicenseServerEnabled)) 
                    createPacketFileToolStripMenuItem.Enabled = true;
            }   
        }

        //DB - 1
        private void EnableProductLicenseContextMenu()
        {
            expandAllToolStripMenuItem.Visible = !DetailTreeView.SelectedNode.IsExpanded;
            expandAllToolStripMenuItem.Enabled = (DetailTreeView.SelectedNode.Nodes.Count == 0) ? false : true;
            collapseToolStripMenuItem.Enabled = collapseToolStripMenuItem.Visible = DetailTreeView.SelectedNode.IsExpanded;
            //allow deactivate if pl is not deactivated
            if (DetailTreeView.SelectedNode.Tag is ProductLicense)
            {
                lcmToolStripSeparator1.Visible = true;
                LicenseServer lsProperty;
                if (DetailTreeView.SelectedNode.Parent.Parent.Tag is LicenseServer)
                    lsProperty = DetailTreeView.SelectedNode.Parent.Parent.Tag as LicenseServer;
                else
                    lsProperty = DetailTreeView.SelectedNode.Parent.Parent.Parent.Tag as LicenseServer;
                reactivateToolStripMenuItem.Enabled = deactivateToolStripMenuItem.Enabled = deleteToolStripMenuItem.Enabled = lsProperty.IsActive && m_Permissions.pt_create_modify_key.Value && lsProperty.LockStatus; //DB - 1
                ProductLicense plProperty = DetailTreeView.SelectedNode.Tag as ProductLicense;
                if (plProperty.Status != ProductLicenseState.AddOn)
                {
                    deactivateToolStripMenuItem.Visible = plProperty.IsActive;
                    reactivateToolStripMenuItem.Visible = !plProperty.IsActive;
                }
                deleteToolStripMenuItem.Visible = !deactivateToolStripMenuItem.Visible;                    
            }
            lcmToolStripSeparator3.Visible = true;
            findToolStripMenuItem.Visible = true;
            findToolStripMenuItem.Enabled = true;
        }

        private void EnableDetailListView(bool value)
        {
            DetailSplitContainer.Panel2Collapsed = value;
        }
        //DB - 1
        //Need Global State Object to determine on/off
        //1) handle permissions
        //2) handle rights depending on current selected object
        private void EnableToolStripMenu(TreeNode node)
        {            
            ResetMainToolStripMenu();
            printToolStripButton.Enabled = true;
            navigateHomeToolStripButton.Enabled = true;
            if (!searchToolStripTextBox.Visible) searchToolStripTextBox.Visible = true;
            if (!SearchToolStripLabel.Visible) SearchToolStripLabel.Visible = true;
            searchToolStripTextBox.Enabled = true;
            SearchToolStripLabel.Enabled = true;
            if (node.Tag is LicenseServer)
            {
                LicenseServer lsp = node.Tag as LicenseServer;
                addLicMainToolStripBtn.Enabled = m_Permissions.pt_create_modify_key.Value;
                if (!lsp.LockStatus) //DB - 1             
                    addProductLicenseMainToolStripDropDownBtn.Enabled = lsp.LockedByCurrentUser() && lsp.IsActive && m_Permissions.pt_create_modify_key.Value;                
                //disallow packet generation for active LS, active PL, 0 tokens, no permissions
                createPacketMainToolStripBtn.Enabled = !(!lsp.IsEnabled && lsp.IsActive && lsp.HasActiveProductLicense()) && m_Permissions.pt_version_pwd.Value;
            }          
        }

        #endregion

        #region Form Events

        private void CreatorForm_Load(object sender, EventArgs e)
        {
            MainTabControl.Visible = false;
            // Set Server Connection List
            if (Settings.Default.ServerList != null)
            {
                String[] serverList = new String[Settings.Default.ServerList.Count];
                Settings.Default.ServerList.CopyTo(serverList, 0);
                this.m_ServerList.AddRange(serverList);
            }
            // Set window location
            if (Settings.Default.WindowLocation != null)
            {
                this.Location = new Point(Settings.Default.WindowLocation.X , Settings.Default.WindowLocation.Y);
            }

            // Set window size
            if (Settings.Default.WindowSize != null)
            {
                this.Size = Settings.Default.WindowSize;
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

        private void CheckInLocksByUser(string user)
        {
            //requires check since it may be called before connection to server.
            if (Service<ICreator>._channelFactory.State == CommunicationState.Opened)
            {
                Service<ICreator>.Use((client) =>
                    {
                        client.CheckInUser(user);
                    });
            }
        }

        private void CreatorForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            //unlock LS's belonging to current user.
            CheckInLocksByUser(WindowsIdentity.GetCurrent().Name);

            //Copy ServerConnection List to app settings
            if (this.m_ServerList.Count > 0)
            {
                Settings.Default.ServerList.Clear();
                Settings.Default.ServerList.AddRange(this.m_ServerList.ToArray());
            }

            // Copy window location to app settings
            Settings.Default.WindowLocation = this.Location;

            // Copy window size to app settings
            if (this.WindowState == FormWindowState.Normal)
            {
                Settings.Default.WindowSize = this.Size;
            }
            else
            {
                Settings.Default.WindowSize = this.RestoreBounds.Size;
            }

            Settings.Default.SelectedDirectory = m_selectedDirectory;

            // Save settings
            Settings.Default.Save();
        }

        #region MainToolStrip Events

        private void printToolStripButton_Click(object sender, EventArgs e)
        {
            CaptureScreen();
            if (printDialog1.ShowDialog() == DialogResult.OK)
                printDocument1.Print();
        }

        private void printToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CaptureScreen();
            if (printDialog1.ShowDialog() == DialogResult.OK)
                printDocument1.Print();
        }

        private void toolsToolStripMenuItem_DropDownOpening(object sender, EventArgs e)
        {
            optionsToolStripMenuItem.Enabled = false;
            if (m_Permissions != null)
                if (m_Permissions.pt_permanent_pwd.HasValue)
                    optionsToolStripMenuItem.Enabled = m_Permissions.pt_permanent_pwd.Value;                                            
        }

        private void addLicMainToolStripBtn_Click(object sender, EventArgs e)
        {
            CreateStandardLicense();
        }

        private void addProductLicenseMainToolStripDropDownBtn_Click(object sender, EventArgs e)
        {
            AddProductLicense();
        }

        private void newCustMainToolStripBtn_Click(object sender, EventArgs e)
        {
            using (ItemNameDialog newCustDlg = new ItemNameDialog("Create New Customer"))
            {                
                CustomerDialogData custData = new CustomerDialogData();
                if (newCustDlg.ShowDialog(this, custData) == DialogResult.OK)
                    CreateNewCustomer(newCustDlg.Name);
            }
        }

        private void searchToolStripTextBox_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter || e.KeyCode == Keys.Return)
                SearchCurrentView(searchToolStripTextBox.Text);
        }

        private void createPacketMainToolStripBtn_Click(object sender, EventArgs e)
        {
            GenerateLicensePacket();
        }

        private void addLicMainToolStripBtn_ButtonClick(object sender, EventArgs e)
        {
            CreateStandardLicense();              
        }

        private void navigateHomeToolStripButton_Click(object sender, EventArgs e)
        {
            ResetMainToolStripMenu();
            navigateHomeToolStripButton.Enabled = true;
            if (MainTabControl.SelectedTab == LicensesTabPage)
            {
                CustomersListView.Visible = true;
                CustomerToolStrip.Visible = false;
            }
            else
            {   //load customer view
                _selectedHardwareKeyCustomer = string.Empty;
                EnableHardwareCustomerView();
            }
            SearchCurrentView(string.Empty);
        }

        private void SearchToolStripLabel_Click(object sender, EventArgs e)
        {
            string searchString = string.Empty;
            if (searchToolStripTextBox.ForeColor != SystemColors.InactiveCaptionText)
                searchString = searchToolStripTextBox.Text;
            SearchCurrentView(searchString);
        }

        private void searchToolStripTextBox_Enter(object sender, EventArgs e)
        {
            if (searchToolStripTextBox.ForeColor == SystemColors.InactiveCaptionText)
            {
                searchToolStripTextBox.Text = string.Empty;
                searchToolStripTextBox.Font = this.Font; //new Font(this.Font, FontStyle.Regular);
                searchToolStripTextBox.ForeColor = SystemColors.WindowText;
            }
        }

        private void searchToolStripTextBox_Leave(object sender, EventArgs e)
        {
            if (searchToolStripTextBox.Text.Length == 0 && !searchToolStripTextBox.Focused)
            {
                searchToolStripTextBox.Text = (CustomersListView.Visible || _selectedHardwareKeyCustomer.Length == 0) ? "Customer Filter" : "License Server Filter";
                searchToolStripTextBox.Font = new Font(this.Font, FontStyle.Italic);
                searchToolStripTextBox.ForeColor = SystemColors.InactiveCaptionText;
            }
        }

        private void seekKeyToolStripButton_Click(object sender, EventArgs e)
        {
            bool bKeyFound = false;
            string[] keyName;
            int index = 0;
            Service<ICreator>.Use((client) =>
            {
                IList<SolimarLicenseProtectionKeyInfo> pkeyList = client.KeyEnumerate();
                if (pkeyList.Count > 0)
                {
                    foreach (SolimarLicenseProtectionKeyInfo attachedKey in pkeyList)
                    {
                        if (attachedKey.IsKeyTypeVerification())
                        {
                            bKeyFound = true;
                            keyName = attachedKey.keyName.Split("-".ToCharArray());
                            //hex to decimal value
                            CustomerTable custRec = client.GetCustomer(Int32.Parse(keyName[0], System.Globalization.NumberStyles.HexNumber).ToString(), false);
                            if (custRec == null)
                            {
                                MessageBox.Show("Failed to seek attached key", "Seek Hardware Key");
                                break;
                            }
                            LoadHardwareKeyListView(string.Empty, custRec.SCRname);
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

        private void reserveHardwareKeyToolStripButton_Click(object sender, EventArgs e)
        {
            string keyName = string.Empty;
            Service<ICreator>.Use((client) =>
            {
                CustomerTable custRec = client.GetCustomer(_selectedHardwareKeyCustomer, false);
                if (custRec == null)
                {
                    MessageBox.Show(string.Format("Failed to find customer : {0}", _selectedHardwareKeyCustomer), "Reserve Hardware Key");
                    return;
                }
                keyName = string.Format("{0:x4}-{1:x4}", custRec.SCRnumber, client.GetNextHardwareTokenValue((uint)custRec.SCRnumber));
                TokenTable newToken = new TokenTable()
                {
                    CustID = custRec.SCRnumber,
                    TokenType = (byte)Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID,
                    TokenValue = keyName,
                    TokenStatus = (byte)TokenStatus.Reserved,
                    LicenseID = -1
                };
                client.CreateToken(newToken);
            });
            PopulateHardwareKeyListView(string.Empty, _selectedHardwareKeyCustomer);
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
                    if (keyStatus == TokenStatus.Deactivated.ToString() || keyStatus == TokenStatus.Reserved.ToString())
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
                        foreach (SolimarLicenseProtectionKeyInfo attachedKey in pKeyList)
                        {
                            if (attachedKey.keyName.Equals(keyValue))
                            {
                                bFound = true;
                                break;
                            }
                        }
                        if (bFound)
                        {
                            client.KeyFormat(keyValue);
                            selectedToken.TokenStatus = (byte)TokenStatus.Deactivated;
                            selectedToken.DeactivatedDate = DateTime.Now;
                            client.UpdateToken(selectedToken);
                            HardwareKeyListView.SelectedItems[0].SubItems[2].Text = TokenStatus.Deactivated.ToString();
                            HardwareKeyListView.SelectedItems[0].SubItems[3].Text = DateTime.Now.ToString(); 
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
            string unintializedKeyValue = string.Empty;
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
                    selectedToken.ActivatedDate = DateTime.Now;
                    client.UpdateToken(selectedToken);
                    client.KeyProgramVerification(unintializedKeyValue,
                                                  Int32.Parse(keyName[0], System.Globalization.NumberStyles.HexNumber),
                                                  Int32.Parse(keyName[1], System.Globalization.NumberStyles.HexNumber));
                    HardwareKeyListView.SelectedItems[0].SubItems[2].Text = TokenStatus.Active.ToString();
                    HardwareKeyListView.SelectedItems[0].SubItems[3].Text = DateTime.Now.ToString();
                }
                else
                {
                    MessageBox.Show(string.Format("Failed to activate hardware key : {0}. No uninitialized keys are attached to {1}", keyValue, m_ServerList[0]),
                                    "Activation Error",
                                    MessageBoxButtons.OK,
                                    MessageBoxIcon.Error);
                }
            });
            this.Cursor = storedCursor;
        }

        private void clearKeyToolStripButton_Click(object sender, EventArgs e)
        {
            string keyValue = string.Empty;
            string[] keyName = keyValue.Split("-".ToCharArray());
            bool bFound = false;
            Cursor storedCursor = this.Cursor;
            this.Cursor = Cursors.WaitCursor;            
            Service<ICreator>.Use((client) =>
            {
                IList<SolimarLicenseProtectionKeyInfo> pKeyList = client.KeyEnumerate();

                foreach (SolimarLicenseProtectionKeyInfo attachedKey in pKeyList)
                {
                    if (!attachedKey.IsKeyTypeUninitialized())
                    {
                        bFound = true;
                        client.KeyFormat(attachedKey.keyName);
                        MessageBox.Show(string.Format("Hardware Token : {0} has been cleared", attachedKey.keyName), "Confirmation", MessageBoxButtons.OK, MessageBoxIcon.Information);
                        break;
                    }
                }
            });
            if(!bFound)
                MessageBox.Show(string.Format("Failed to clear attached hardware key : {0}. No initialized keys are attached to {1}", keyValue, m_ServerList[0]),
                                "Clear Error",
                                MessageBoxButtons.OK,
                                MessageBoxIcon.Error);
            this.Cursor = storedCursor;
        }
        #endregion

        #region MainMenuStrip Events
        private void exitMainToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }
        private void connectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ConnectServer();
        }
        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (AboutDialog dlg = new AboutDialog())
            {
                //int majorVer = 0;
                //int minorVer = 0;
                //int buildVer = 0;
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

        private void optionsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (ProductConfigurationDialog dlg = new ProductConfigurationDialog(s_CommLink))
            {
                if (dlg.ShowDialog() == DialogResult.OK)
                {
                }
            }
        }
        #endregion

        #region MainTabControl
        private void MainTabControl_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (MainTabControl.SelectedTab.Name)
            {
                case AppConstants.LicensesTabPage:
                    ResetMainToolStripMenu();
                    SearchCurrentView(string.Empty);
                    splitContainer2.Panel1.Controls.Clear();
                    loadingCircle1.Parent = splitContainer2.Panel1;
                    MainTabControl.Parent = splitContainer2.Panel1;                    
                    break;                    
                case AppConstants.ValidationKeysTabPage:
                    _selectedHardwareKeyCustomer = (_selectedLicenseCustomer.Name != null) ? _selectedLicenseCustomer.Name : string.Empty;
                    ResetMainToolStripMenu();
                    SearchToolStripLabel.Visible = !(_selectedHardwareKeyCustomer.Length > 0);
                    searchToolStripTextBox.Visible = !(_selectedHardwareKeyCustomer.Length > 0);
                    SearchCurrentView(string.Empty);
                    loadingCircle1.Parent = splitContainer2.Panel1;
                    break;
                case AppConstants.ReportTabPage:
                    ResetMainToolStripMenu();
                    SearchToolStripLabel.Visible = false;
                    searchToolStripTextBox.Visible = false;
                    LoadReportListView();
                    loadingCircle1.Parent = splitContainer2.Panel1;
                    break;
                default: break;
            }
        }
        #endregion      

        #region CustomersListView Events
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
                    _selectedLicenseCustomer.Name = item.SubItems[1].Text;
                    _selectedLicenseCustomer.Id = Int32.Parse(item.SubItems[0].Text, System.Globalization.NumberStyles.HexNumber);
                    searchToolStripTextBox.Text = string.Empty;
                    CustomerToolStrip.Visible = true;
                    findToolStrip.Visible = false;
                    CustomersListView.Visible = false;
                    CustomersListView.Items.Clear(); //necessary?
                    //Load Destination Names into combobox
                    LoadDestinationNameComboBox(_selectedLicenseCustomer);
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
                if (CustomersListView.GetItemAt(e.X, e.Y) != null)
                {
                    renameCustomerToolStripMenuItem.Enabled = true;
                    deleteCustomerToolStripMenuItem.Enabled = true;
                }
                customerContextMenuStrip.Show(CustomersListView, e.X, e.Y);
            }
        }
        private void CustomersListView_ColumnClick(object sender, ColumnClickEventArgs e)
        {
            _lvManager.SetSortIndexColumn(CustomersListView.Handle, e.Column);
            // Perform the sort with these new sort options.
            CustomersListView.Sort();
        }
        #endregion

        #region CustomerContextMenuStrip
        private void customerToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (ItemNameDialog newCustDlg = new ItemNameDialog("Create New Customer"))
            {
                if (newCustDlg.ShowDialog(this, new CustomerDialogData()) == DialogResult.OK)
                {
                    CreateNewCustomer(newCustDlg.Name);
                }
            }
        }
        private void deleteCustomerToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (CustomersListView.SelectedItems.Count > 0)
            {
                ListViewItem item = CustomersListView.SelectedItems[0];
                DeleteCustomerItem(item);
                CustomersListView.Items.Remove(item);
            }
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
        #endregion

        #region LicenseContextMenu Events
        private void findToolStripMenuItem_Click(object sender, EventArgs e)
        {   //determine if find is a product license or license server
            findToolStrip.Visible = true;
        }
        private void deleteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (DetailTreeView.SelectedNode.Tag is LicenseServer)
                DeleteLicenseNode(DetailTreeView.SelectedNode);
            else if (DetailTreeView.SelectedNode.Tag is ProductLicense)
                DeleteProductLicenseNode(DetailTreeView.SelectedNode);
        }
        private void newLicenseToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CreateStandardLicense();
        }
        private void newProductLicenseToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AddProductLicense();
        }

        private void generateLicenseFileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            GenerateLicensePacket();
        }
        private void expandAllToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DetailTreeView.SelectedNode.ExpandAll();
        }
        private void collapseToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DetailTreeView.SelectedNode.Collapse();
        }
        #endregion

        #region LicenseContextMenuStrip Events
        private void LicenseContextMenuStrip_Opening(object sender, CancelEventArgs e)
        {
            ResetContextMenu(LicenseContextMenuStrip.Items);
            if (DetailTreeView.SelectedNode != null)
            {
                // Highlight the node that the user clicked.
                // The node is highlighted until the Menu is displayed on the screen
                // Find the appropriate ContextMenu based on the highlighted node
                if (DetailTreeView.SelectedNode.Level == 0)
                    EnableLicenseContextMenu(true);
                else
                    EnableProductLicenseContextMenu();
            }
            else
            {                    //create standard license
                EnableLicenseContextMenu(true);
            }
        }

        private void createPacketFileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            GenerateLicensePacket();
        }

        private void cloneToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //only display on product licenses
            //get product license information from currently selected tag
            //launch pl dialog? disallow modification
            //copy Tokens from TokenTable, Product License from ProductLicenseTable, Modules from ModuleTable, no transactions, no packets
            // 1) Create LS - same dest
            // 2) Copy Tokens or no tokens?
            // 3) Create Product License
            //      4) Create Modules for Product License
            if (DetailTreeView.SelectedNode.Tag is LicenseServer)
            {
                bool bCopy = false;
                LicenseServer clonedLS = null;
                Service<ICreator>.Use((client) =>
                {
                    LicenseServer selectedLS = DetailTreeView.SelectedNode.Tag as LicenseServer;
                    clonedLS = new LicenseServer() 
                    { 
                        CustID = selectedLS.CustID, 
                        DestID = selectedLS.DestID,
                        GroupID = client.GetNextGroupID(selectedLS.CustID, selectedLS.DestID),
                        Permissions = m_Permissions,
                        ProductLicenseIndex = selectedLS.ProductLicenseIndex,
                        Description = "Copied from " + selectedLS.Name 
                    };
                    if (MessageBox.Show(string.Format("Please confirm copying of {0} to {1}.", selectedLS.Name, clonedLS.Name), 
                                        "Copy License Server",
                                        MessageBoxButtons.OKCancel, 
                                        MessageBoxIcon.Exclamation) == DialogResult.OK)
                    {
                        bCopy = true;
                        CreateLicenseEntity(clonedLS); //need to retain pl index
                        LicenseTable ls = client.GetLicenseByName(clonedLS.Name, false);
                        IList<ProductLicenseTable> plList = client.GetProductLicenses(selectedLS.Name, true);
                        foreach (ProductLicenseTable pl in plList)
                        {
                            ProductLicenseTable clonedPLT = new ProductLicenseTable()
                            {
                                plID = Lic_LicenseInfoAttribsHelper.GenerateProductLicenseName(clonedLS.Name, pl.plIndex),
                                plIndex = pl.plIndex,
                                plState = pl.plState,
                                LicenseID = ls.ID,
                                IsActive = true,
                                ExpirationDate = pl.ExpirationDate,
                                Description = string.Empty,
                                ProductID = pl.ProductID,
                                ProductVersion = pl.ProductVersion,
                                ParentProductLicenseID = (pl.ParentProductLicenseID != null) ? Lic_LicenseInfoAttribsHelper.GenerateProductLicenseName(clonedLS.Name, Int32.Parse(pl.ParentProductLicenseID.Substring(pl.ParentProductLicenseID.LastIndexOf("-")))) : null,
                                ProductConnection = pl.ProductConnection,
                                Activations = pl.Activations,
                                ActivationAmount = pl.ActivationAmount,
                                Extensions = pl.Extensions                                
                            };
                            client.CreateProductLicense(clonedPLT);
                            //create modules for product license
                            //retrieve product license to get dbID
                            ProductLicenseTable dbPLT = client.GetProductLicense(clonedPLT.plID);
                            IList<ModuleTable> dbModules = client.GetModulesByProductLicense(pl.plID, true);                           
                            ModuleTable[] newModules = new ModuleTable[dbModules.Count()];
                            for(int index = 0; index < dbModules.Count; index++)
                            {
                                ModuleTable newModule = new ModuleTable()
                                {
                                    ModID = dbModules[index].ModID,
                                    AppInstance = dbModules[index].AppInstance,
                                    ProductLicenseID = dbPLT.ID,
                                    Value = dbModules[index].Value
                                };
                                //quesiton why doesn't it keep the latest module value
                                newModules[index] = newModule;
                            }                            
                            client.CreateAllModules(newModules.ToList());
                        }
                    }
                });
                if(clonedLS != null && bCopy)
                    LoadSelectedLicenseServer(clonedLS.Name);
            }
        }

        private void checkInToolStripMenuItem_Click(object sender, EventArgs e)
        {
            checkInLicenseServer(DetailTreeView.SelectedNode);
        }

        private void checkOutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            checkOutLicenseServer(DetailTreeView.SelectedNode);
        }

        private void checkInLicenseServer(TreeNode LicenseServerNode)
        {
            if (LicenseServerNode.Tag != null)
            {
                (LicenseServerNode.Tag as LicenseServer).LockStatus = false;
                LoadDetailTreeViewSelectedNode(LicenseServerNode);
            }
        }
        private void checkOutLicenseServer(TreeNode LicenseServerNode)
        {
            if (LicenseServerNode.Tag != null)
            {
                (LicenseServerNode.Tag as LicenseServer).LockStatus = true;
                LoadDetailTreeViewSelectedNode(LicenseServerNode);
            }
        }

        private void DeactivateLicenseServer(LicenseServer selectedLicense)
        {         
            if (MessageBox.Show(string.Format("Please confirm deactivation for {0}.", selectedLicense.Name), 
                                "Deactivate",
                                MessageBoxButtons.OKCancel, 
                                MessageBoxIcon.Exclamation) == DialogResult.OK)
            {
                Service<ICreator>.Use((client) =>
                {
                    LicenseServer lsProperty = DetailTreeView.Nodes.Find(selectedLicense.Name, true)[0].Tag as LicenseServer;
                    TransactionManager.CreateTransaction(TransactionType.LicenseServer,
                                                         lsProperty.Name,
                                                         string.Empty,
                                                         "Deactivate License Server",
                                                         string.Empty,
                                                         string.Empty);
                    LicenseTable licRecord = client.GetLicenseByName(lsProperty.Name, false);
                    licRecord.IsActive = false;
                    lsProperty.IsActive = false;
                    client.UpdateLicense(licRecord, true); //DB-1
                    IList<TokenTable> tokens = client.GetTokensByLicenseName(selectedLicense.Name);
                    if (tokens != null) //DB - # of tokens
                    {
                        foreach (TokenTable token in tokens)
                        {
                            if (token.TokenType == (byte)Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID)
                            {
                                token.LicenseID = -1;
                                client.UpdateToken(token);
                            }
                            else
                                client.DeleteToken(token);
                        }
                    }
                    List<ProductLicenseTable> plList = client.GetProductLicenses(lsProperty.Name, true); // db - 2
                    if (plList != null)
                    {
                        foreach (ProductLicenseTable pl in plList)
                        {
                            pl.IsActive = false;
                        }
                        client.UpdateAllProductLicenses(plList); //db - 3
                    }
                });
                //DetailPropertyGrid.SelectedObject = DetailTreeView.SelectedNode.Tag;
                //PopulateDetailListView(DetailPropertyGrid.SelectedObject);
                ////SetLicenseServerState(DetailTreeView.SelectedNode, false);
                ////DetailPropertyGrid.Refresh();
                LoadDetailTreeViewSelectedNode(DetailTreeView.SelectedNode);
                foreach (TreeNode node in DetailTreeView.SelectedNode.Nodes)
                {
                    LoadProductNode(node);
                }
            }
            //LoadLicenseNode(DetailTreeView.SelectedNode);
        }

        //doesn't fix module product connection count for addon PLs
        private void DeactivateProductLicense(ProductLicense selectedProductLicense)
        {
            if (MessageBox.Show(string.Format("Please confirm deactivation for {0}.", selectedProductLicense.ID), 
                                "Deactivate",
                                MessageBoxButtons.OKCancel, 
                                MessageBoxIcon.Exclamation) == DialogResult.OK)
            {
                //propertygrid object needs to be update, treenode status needs to be updated
                Service<ICreator>.Use((client) =>
                {   //decrement app instance and modules from product view
                    List<string> plAddOns = client.GetAddOnProductLicenses(selectedProductLicense.ID);
                    plAddOns.Add(selectedProductLicense.ID);
                    if (plAddOns.Count > 0)
                    {   //need way to place product license + add-on product license in an array instead of grabbing one by one.
                        foreach (string pl in plAddOns)
                        {
                            ProductLicenseTable plt = client.GetProductLicense(pl);
                            TransactionManager.CreateTransaction(TransactionType.LicenseServer,
                                                                 string.Empty,
                                                                 plt.plID,
                                                                 "Deactivate Product License",
                                                                 plt.plID,
                                                                 string.Empty);
                            plt.IsActive = false;
                            client.UpdateProductLicense(plt);
                            //readjust product connections in the parent product license if add-on
                            if (plt.ParentProductLicenseID != "") //add-on
                            {
                                //adjust 
                            }
                        }
                    }
                });
                DetailTreeView.SelectedNode.Tag = DetailPropertyGrid.SelectedObject;
                DetailPropertyGrid.Refresh();
                LoadDetailTreeViewSelectedNode(DetailTreeView.SelectedNode);
            }
        }

        private void deactivateToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (DetailTreeView.SelectedNode.Tag is LicenseServer)
                DeactivateLicenseServer(DetailTreeView.SelectedNode.Tag as LicenseServer);            
            else if(DetailTreeView.SelectedNode.Tag is ProductLicense)         
                DeactivateProductLicense(DetailPropertyGrid.SelectedObject as ProductLicense);            
        }

        private void ReactivateLicenseServer(LicenseServer ls)
        {
            if (MessageBox.Show(string.Format("Please confirm reactivation of License Server : {0}.",
                                ls.Name), 
                                "Reactivate License Server",
                                MessageBoxButtons.OKCancel, 
                                MessageBoxIcon.Exclamation) == DialogResult.OK)
            {
                //verify delete is allowed.            
                Service<ICreator>.Use((client) =>
                {
                    TransactionManager.CreateTransaction(TransactionType.LicenseServer,
                                                         ls.Name,
                                                         string.Empty,
                                                         "Reactivate License Server",
                                                         string.Empty,
                                                         string.Empty);
                    LicenseTable licRecord = client.GetLicenseByName(ls.Name, false);
                    licRecord.IsActive = true;
                    ls.IsActive = true;
                    client.UpdateLicense(licRecord, true);
                    IList<TokenTable> tokens = client.GetTokensByLicenseName(ls.Name);
                    if (tokens != null)
                    {
                        foreach (TokenTable token in tokens)
                        {
                            if (token.TokenType == (byte)Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID)
                            {
                                token.LicenseID = licRecord.ID;
                                token.TokenStatus = (byte)TokenStatus.Active;
                                client.UpdateToken(token);
                                break;
                            }
                        }
                    }
                });
                DetailPropertyGrid.SelectedObject = DetailTreeView.SelectedNode.Tag;
                PopulateDetailListView(DetailPropertyGrid.SelectedObject);
                SetLicenseServerState(DetailTreeView.SelectedNode, false);
            }          
        }

        private void ReactivateProductLicense(ProductLicense productLicense)
        {
            if (MessageBox.Show(string.Format("Please confirm reactivation of Product License : {0}.",
                                productLicense.ID),
                                "Reactivate Product License",
                                MessageBoxButtons.OKCancel,
                                MessageBoxIcon.Exclamation) == DialogResult.OK)
            {
                //verify delete is allowed.            
                Service<ICreator>.Use((client) =>
                {
                //propertygrid object needs to be update, treenode status needs to be updated
                //ProductLicense selectedProductLicense = DetailPropertyGrid.SelectedObject as ProductLicense;
                //decrement app instance and modules from product view
                    List<string> plAddOns = client.GetAddOnProductLicenses(productLicense.ID);
                    plAddOns.Add(productLicense.ID);
                    if (plAddOns.Count > 0)
                    {   //need way to place product license + add-on product license in an array instead of grabbing one by one.
                        foreach (string pl in plAddOns)
                        {
                            ProductLicenseTable plt = client.GetProductLicense(pl);
                            TransactionManager.CreateTransaction(TransactionType.LicenseServer,
                                                                 string.Empty,
                                                                 plt.plID,
                                                                 "Reactivate Product License",
                                                                 plt.plID,
                                                                 string.Empty);
                            plt.IsActive = true;
                            client.UpdateProductLicense(plt);
                        }
                    }
                });
                DetailTreeView.SelectedNode.Tag = DetailPropertyGrid.SelectedObject;
                DetailPropertyGrid.Refresh();
                LoadDetailTreeViewSelectedNode(DetailTreeView.SelectedNode);
            }
        }
        private void reactivateToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (DetailTreeView.SelectedNode.Tag is LicenseServer)
                ReactivateLicenseServer(DetailTreeView.SelectedNode.Tag as LicenseServer);
            else if (DetailTreeView.SelectedNode.Tag is ProductLicense)
                ReactivateProductLicense(DetailPropertyGrid.SelectedObject as ProductLicense);    
        }

        private void markLostToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show(string.Format("Please confirm lost hardware token for {0}.", 
                                DetailTreeView.SelectedNode.Name), 
                                "Lost Hardware Token",
                                MessageBoxButtons.OKCancel, 
                                MessageBoxIcon.Exclamation) == DialogResult.OK)
            {
                if (DetailTreeView.SelectedNode.Tag is LicenseServer)
                {
                    LicenseServer selectedLicense = DetailTreeView.SelectedNode.Tag as LicenseServer;
                    //verify delete is allowed.            
                    Service<ICreator>.Use((client) =>
                    {
                        LicenseServer lsProperty = DetailTreeView.Nodes.Find(selectedLicense.Name, true)[0].Tag as LicenseServer;
                        LicenseTable licRecord = client.GetLicenseByName(lsProperty.Name, false);
                        licRecord.IsActive = false;
                        lsProperty.IsActive = false;
                        client.UpdateLicense(licRecord, true);
                        IList<TokenTable> tokens = client.GetTokensByLicenseName(selectedLicense.Name);
                        string lostTokenName = string.Empty;
                        if (tokens != null)
                        {
                            foreach (TokenTable token in tokens)
                            {
                                if (token.TokenType == (byte)Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID)
                                {
                                    token.TokenStatus = (byte)TokenStatus.Lost;
                                    client.UpdateToken(token);
                                    lostTokenName = token.TokenValue;
                                    break;
                                }
                            }
                        }
                        TransactionManager.CreateTransaction(TransactionType.LicenseServer,
                                                             lsProperty.Name,
                                                             string.Empty,
                                                             "Lost Hardware Token",
                                                             lostTokenName,
                                                             string.Empty);
                    });
                    DetailPropertyGrid.SelectedObject = DetailTreeView.SelectedNode.Tag;
                    SetLicenseServerState(DetailTreeView.SelectedNode, false);
                    PopulateDetailListView(DetailPropertyGrid.SelectedObject);
                }
            }
        }

        private void newTokenToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (DetailPropertyGrid.SelectedObject is LicenseServer)
                CreateValidationToken(DetailPropertyGrid.SelectedObject as LicenseServer);
        }

        private void clearToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (DetailPropertyGrid.SelectedObject is LicenseServer)   //token remove
                RemoveValidationToken(DetailPropertyGrid.SelectedObject as LicenseServer);
        }
        #endregion

        #region DetailListViewContextMenuStrip Events

        private void incrementToolStripMenuItem_Click(object sender, EventArgs e)
        {
            IncrementSelectedModules();
        }

        private void decrementToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DecrementSelectedModules();
        }

        private void SetModuleState(ProductLicense plp, ListViewItem lvItem)
        {
            lvItem.ForeColor = this.ForeColor;
            lvItem.Font = this.Font;
            if (plp.ExpirationDate.HasValue && plp.ExpirationDate.Value.CompareTo(new DateTime(1900, 1, 1)) != 0)
            {
                lvItem.ForeColor = (plp.ExpirationDate.Value.ToLocalTime().CompareTo(CurrentExpirationDate) < 0) ? Color.Red : this.ForeColor;
            }
            else //perm - blue
            {
                lvItem.ForeColor = (plp.Status == ProductLicenseState.Licensed) ? Color.SteelBlue : this.ForeColor;
            }
            if ((lvItem.Tag as Module).Value == 0 && (lvItem.Tag as Module).AppInstance == 0)
            {
                lvItem.ForeColor = Color.SlateGray;
                lvItem.Font = new Font(lvItem.Font, FontStyle.Italic);
            }
        }

        private void dlvEditToolStripMenuItem_DropDownOpening(object sender, EventArgs e)
        {
            if (DetailListView.SelectedItems.Count > 0)
            {
                setModuleToolStripTextBox.Text = (DetailListView.SelectedItems[0].Tag as Module).Value.ToString();
            }
        }

        private void setModuleToolStripTextBox_KeyUp(object sender, KeyEventArgs e)
        {           
            if (e.KeyCode == Keys.Enter)
            {
                if (DetailListView.SelectedItems.Count > 0)
                {
                    ushort oldValue, newValue;
                    ProductLicense plp = DetailPropertyGrid.SelectedObject as ProductLicense;
                    foreach (ListViewItem selectedItem in DetailListView.SelectedItems)
                    {
                        Module module = selectedItem.Tag as Module;
                        oldValue = (ushort)module.Value;
                        string errorMsg = string.Empty;
                        if (!ushort.TryParse(setModuleToolStripTextBox.Text, out newValue))
                            errorMsg = "Invalid format for {0} value";
                        else
                        {
                            if (oldValue != newValue)
                            {
                                if (newValue > (plp.GetAvailableModuleUnits(module) + oldValue))
                                    errorMsg = "Specified value exceeds the available units allowed for {0}";
                                else
                                {
                                    module.Value = (short)newValue;
                                    if (plp.SetModule(module))
                                    {
                                        selectedItem.SubItems[1].Text = module.Value.ToString();
                                        if (selectedItem.SubItems[2].Text != "Unlimited")
                                            selectedItem.SubItems[2].Text = plp.GetAvailableModuleUnits(module).ToString();
                                        SetModuleState(plp, selectedItem);
                                        DetailListViewContextMenuStrip.Close();
                                    }
                                }
                            }
                            else
                                return;
                        }
                        if (errorMsg.Length > 0)
                        {
                            module.Value = (short)oldValue;
                            MessageBox.Show(string.Format(errorMsg, module.Name), "Set Module Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            setModuleToolStripTextBox.SelectAll();
                        }
                    }
                }
            }
        }

        #endregion

        #region DestNameComboBox Events
        private void DestNameComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            //filter out all license servers that do not have the dest id selected.
            Service<ICreator>.Use((client) =>
            {
                if (DestNameComboBox.Text == "<Create...>")
                {
                    using (ItemNameDialog dlg = new ItemNameDialog("Create New Destination Name"))
                    {
                        DestNameDialogData destNameData = new DestNameDialogData((uint)_selectedLicenseCustomer.Id);
                        if (dlg.ShowDialog(this, destNameData) == DialogResult.OK)
                        {   //create the new destination name for this customer                                                    
                            DestinationNameTable newDestName = new DestinationNameTable()
                            {
                                CustID = _selectedLicenseCustomer.Id,
                                DestID = (int)client.GetNextDestinationID((uint)_selectedLicenseCustomer.Id),
                                DestName = dlg.Name,
                            };
                            client.CreateDestinationName(newDestName);
                            DestNameComboBox.Items.Insert(newDestName.DestID, dlg.Name);
                            DestNameComboBox.SelectedItem = dlg.Name;
                        }
                        else
                        {   //set back to last selected dest name.
                            int selectedIndex = DestNameComboBox.Items.IndexOf(_selectedLicenseCustomer.Name);
                            if (selectedIndex > 0)
                                DestNameComboBox.SelectedIndex = (selectedIndex > 0) ? selectedIndex : 2;
                        }
                    }
                }
                else if (DestNameComboBox.Text == "<Edit...>")
                {
                    using (EditDestinationName dlg = new EditDestinationName(_selectedLicenseCustomer.Id, DestNameComboBox.Items))
                    {
                        dlg.ShowDialog(this);
                        if (dlg.Modified)
                        {
                            DestNameComboBox.Items.Clear();
                            IList<DestinationNameTable> destTables = client.GetDestNamesByCustID(_selectedLicenseCustomer.Id).OrderBy(c => c.DestID).ToList();
                            foreach (DestinationNameTable table in destTables)
                            {
                                DestNameComboBox.Items.Add(table.DestName);
                            }
                            DestNameComboBox.Items.Add("<Create...>");
                            DestNameComboBox.Items.Add("<Edit...>");
                        }
                        if(dlg.SelectedDestinationName != null)
                            DestNameComboBox.SelectedItem = dlg.SelectedDestinationName;
                    }
                }
                else
                {
                    //reload license server nodes with only selected dest names
                    //try pulling from database instead of from stored list
                    //LoadLicenseServers(DestNameComboBox.Text);
                    //how to get choose the selected license server to highlight
                    //what resets _selectedtreenodekey
                    LoadDBLicenses(string.Empty, _selectedTreeNodeKey, DestNameComboBox.Text, true);
                    _selectedTreeNodeKey = string.Empty;
                }
            });
        }
        private void DestNameComboBox_DropDown(object sender, EventArgs e)
        {
            //reload list?
        }
        #endregion

        #region DetailTreeView Events

        private void nextFindToolStripButton_Click(object sender, EventArgs e)
        {
            findSearchString(findToolStripTextBox.Text);
        }

        private void closeFindToolStripButton_Click(object sender, EventArgs e)
        {
            //hide find toolstrip
            findToolStrip.Visible = false;
            findToolStripTextBox.Text = string.Empty;
        }

        private void findToolStripTextBox_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter || e.KeyCode == Keys.Return)
            {
                findSearchString(findToolStripTextBox.Text);
            }
        }

        //allow only administrators to unlock other people's locks
        private void DetailTreeView_KeyDown(object sender, KeyEventArgs e)
        {
            switch(e.KeyData)
            {
                case Keys.F:
                    if(e.Control) findToolStrip.Visible = true;
                    break;
                case Keys.F2:
                    if (DetailTreeView.SelectedNode.Tag is LicenseServer)
                        if (m_Permissions.pt_extension_pwd.Value && m_Permissions.pt_version_pwd.Value)                        
                            if (!(DetailTreeView.SelectedNode.Tag as LicenseServer).LockStatus)
                                checkOutLicenseServer(DetailTreeView.SelectedNode);                                            
                    break;
                case Keys.F3:
                    if (DetailTreeView.SelectedNode.Tag is LicenseServer)
                    {
                        if (m_Permissions.pt_extension_pwd.Value && m_Permissions.pt_version_pwd.Value)
                        {
                            LicenseServer lsp = DetailTreeView.SelectedNode.Tag as LicenseServer;
                            if (lsp.LockStatus)
                            {                                //check user, if admin always allow, if regular user only allow if same
                                if(lsp.LockedByCurrentUser() || lsp.LockedByAdmin() )
                                    checkInLicenseServer(DetailTreeView.SelectedNode);
                            }
                        }
                    } 
                    break;
                case Keys.Control | Keys.L:
                    if(m_Permissions.pt_create_modify_key.Value)                    
                        CreateStandardLicense();                    
                    break;
                case Keys.Insert:
                    if(DetailTreeView.SelectedNode.Tag is LicenseServer)
                    {
                        LicenseServer lsp = DetailTreeView.SelectedNode.Tag as LicenseServer;
                        if(lsp.LockStatus)                        
                            if(lsp.LockedByCurrentUser())    
                                AddProductLicense();
                    }                   
                    break;
                case Keys.F7:
                    if (DetailTreeView.SelectedNode.Tag is LicenseServer)
                    {
                        LicenseServer lsp = DetailTreeView.SelectedNode.Tag as LicenseServer;
                        if (lsp.IsEnabled && m_Permissions.pt_version_pwd.Value && lsp.IsActive)
                            GenerateLicensePacket();
                    }
                    break;
                case Keys.Delete:
                    if (DetailTreeView.SelectedNode.Tag is LicenseServer)
                    {
                        LicenseServer licenseServer = DetailTreeView.SelectedNode.Tag as LicenseServer;
                        if (licenseServer.LockStatus)
                        {
                            if (licenseServer.LockedByCurrentUser())
                            {
                                if (licenseServer.IsActive)
                                    DeactivateLicenseServer(licenseServer);
                                else
                                    DeleteLicenseNode(DetailTreeView.SelectedNode);
                            }
                        }
                    }
                    if (DetailTreeView.SelectedNode.Tag is ProductLicense)
                    {
                        ProductLicense productLicense = DetailTreeView.SelectedNode.Tag as ProductLicense;
                        //need to know whether or not user has rights to delete
                        TreeNode lsNode = DetailTreeView.Nodes.Find(productLicense.LicenseServer, true).First();
                        if (lsNode != null)
                        {
                            LicenseServer licenseServer = lsNode.Tag as LicenseServer;
                            if (licenseServer.LockStatus)
                            {
                                if (licenseServer.LockedByCurrentUser())
                                {
                                    if (productLicense.IsActive)
                                        DeactivateProductLicense(productLicense);
                                    else
                                        DeleteProductLicenseNode(DetailTreeView.SelectedNode);
                                }
                            }
                        }
                    }                    
                    break;

                default: break;
            }
        }

        private void DetailTreeView_AfterSelect(object sender, TreeViewEventArgs e)
        {
            //System.Diagnostics.Stopwatch watch = new System.Diagnostics.Stopwatch();
            //watch.Start();
            LoadDetailTreeViewSelectedNode(e.Node);
            //watch.Stop();
            //MessageBox.Show(string.Format("DetailTreeView_AfterSelect Elapsed: {0}\nIn milliseconds: {1}\nIn timer ticks: {2}", watch.Elapsed, watch.ElapsedMilliseconds, watch.ElapsedTicks), "INFORMATION", MessageBoxButtons.OK);

        }

        //need to account for deleted node
        private void LoadDetailTreeViewSelectedNode(TreeNode node)
        {                        
            LicenseServer lsp = null;
            TreeNode lspNode = null, productNode = null;
            DetailPropertyGrid.Enabled = true;
            DetailSplitContainer.Panel1Collapsed = false;
            DetailTreeView.BeginUpdate();
            //verify node exists otherwise find next available
            switch (node.Level)
            { 
                case 1:            //level 1 - Product
                    productNode = node;
                    LoadProductNode(productNode);
                    if (productNode.Tag != null)                    //remove self if holds no product licenses
                    {
                        lspNode = productNode.Parent;
                        lsp = lspNode.Tag as LicenseServer;
                        //bool bLocked = lsp.LockStatus;
                        m_CurrentLicenseName = lsp.Name;
                        DetailSplitContainer.Panel1Collapsed = true;
                    }
                    else
                        DetailTreeView.Nodes.Remove(productNode);
                    break;
                case 2:            //level 2 - Product License
                    productNode = node.Parent;
                    LoadProductNode(productNode);
                    if (productNode.Tag != null)                    //remove self if holds no product licenses
                    {
                        if (productNode.Nodes.Find(node.Name, true).Count() > 0)
                        {
                            lspNode = productNode.Parent;
                            lsp = lspNode.Tag as LicenseServer;
                            m_CurrentLicenseName = lsp.Name;
                        }
                    }
                    else                    
                        DetailTreeView.Nodes.Remove(productNode);                    
                    break;
                case 3:            //level 3 - AddOn Product License
                    productNode = node.Parent.Parent;
                    LoadProductNode(productNode);
                    if (productNode.Tag != null)                    //remove self if holds no product licenses
                    {
                        if (productNode.Nodes.Find(node.Name, true).Count() > 0)
                        {
                            lspNode = productNode.Parent;
                            lsp = lspNode.Tag as LicenseServer; //license server add-on
                            m_CurrentLicenseName = lsp.Name;
                        }
                    }
                    else
                        DetailTreeView.Nodes.Remove(productNode);
                    break;
                default:    //level 0 - License Server
                    LoadLicenseNode(node); //contact DB
                    if (node.Tag != null)
                    {
                        lspNode = node;
                        lsp = lspNode.Tag as LicenseServer;
                        if (lsp != null) m_CurrentLicenseName = lsp.Name;
                    }
                    else
                        if(node.Name != "Empty") DetailTreeView.Nodes.Remove(node);                    
                    break;
            }
            if (node.Name != "Empty" && lsp != null)
            {              
                if (!lsp.LockStatus || (lsp.UserLock != WindowsIdentity.GetCurrent().Name.ToLower()))
                    DetailPropertyGrid.Enabled = false;                              
                DetailPropertyGrid.SelectedObject = node.Tag; 
                EnableToolStripMenu(node);
                //major performance hit
                SetLicenseServerState(node, false); //contact DB
                SetCurrentViewTabPages(node);
                //Update the currently selected license server for lock status
                lspNode.Text = (lsp.UserLock != string.Empty) ? lsp.Name + " - " + lsp.UserLock : lsp.Name;
            }
            DetailTreeView.EndUpdate();
        }

        private void SetCurrentViewTabPages(TreeNode node)
        {
            if (!PropertyGridTabControl.TabPages.Contains(PropertyGridTabPage))
                PropertyGridTabControl.TabPages.Add(PropertyGridTabPage);
            if (!(node.Tag is Customer))
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

        private void DetailTreeView_MouseDown(object sender, MouseEventArgs e)
        {
            TreeNode node = DetailTreeView.GetNodeAt(new Point(e.X, e.Y));
            if (node != null)
            {
                if (node == DetailTreeView.SelectedNode)
                {
                    //System.Diagnostics.Stopwatch watch = new System.Diagnostics.Stopwatch();
                    //watch.Start();
                    LoadDetailTreeViewSelectedNode(node);
                    //watch.Stop();
                    //MessageBox.Show(string.Format("DetailTreeView_Mouse Up Elapsed: {0}\nIn milliseconds: {1}\nIn timer ticks: {2}", watch.Elapsed, watch.ElapsedMilliseconds, watch.ElapsedTicks), "INFORMATION", MessageBoxButtons.OK);
                }
            }
        }

        private void DetailTreeView_MouseUp(object sender, MouseEventArgs e)
        {
            // Point where mouse is clicked
            Point p = new Point(e.X, e.Y);
            TreeNode node = DetailTreeView.GetNodeAt(new Point(e.X, e.Y));
            if (e.Button == MouseButtons.Right)
            {
                if (node != null)
                {
                    if (node.Bounds.Contains(p))
                    {                        //// Go to the node that the user clicked
                        DetailTreeView.SelectedNode = node;
                    }
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
            }
            DetailTreeView.EndUpdate();
        }

        #endregion

        #region DetailListView Events

        private void DetailListView_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {        
            switch (DetailTreeView.SelectedNode.Level)
            {
                case 1: //Product
                    //initialize tooltip information for DetailListView - Modules
                    //retrieve order data using information from selected item
                    string info = string.Empty, expDate = string.Empty;
                    int moduleID;
                    Int32.TryParse(e.Item.Name, out moduleID);
                    StringBuilder sb = new StringBuilder();
                    foreach (ProductLicense productLicense in (DetailTreeView.SelectedNode.Tag as ProductCollection).Collection)
                    {
                        if (productLicense.ModuleList.Find(c => c.ModID == moduleID) != null)
                        {
                            expDate = (productLicense.ExpirationDate.HasValue) ? productLicense.ExpirationDate.Value.ToShortDateString() : "None";
                            sb.AppendFormat("Product License - {0}, Exp Date - {1}\n", productLicense.ID, expDate);
                        }
                    }
                    info = sb.ToString();
                    if (info.Length > 0) info = info.Remove(info.Length - 1, 1);
                    e.Item.ToolTipText = info;
                    break;                  
                default: break;
            }
        }
        private void DetailListView_MouseUp(object sender, MouseEventArgs e)
        {
            bool bShow = false;
            //needs to handle validation properties also
            if (e.Button == MouseButtons.Right)
            {
                ListViewItem item = DetailListView.GetItemAt(e.X, e.Y);
                ResetContextMenu(DetailListViewContextMenuStrip.Items);
                switch (DetailPropertyGrid.SelectedObject.GetType().Name)
                {
                    case AppConstants.LicenseServerObjectName:
                        if ((DetailPropertyGrid.SelectedObject as LicenseServer).IsActive)
                        {
                            bShow = true;
                            if (m_Permissions.pt_module_pwd.Value)
                            {
                                dlvNewToolStripMenuItem.Visible = true;
                                dlvNewToolStripMenuItem.Enabled = dlvAddToolStripButton.Enabled;
                                dlvDeleteToolStripMenuItem.Visible = true;
                                dlvDeleteToolStripMenuItem.Enabled = dlvRemoveToolStripButton.Enabled;
                            }
                        }
                        break;
                    case AppConstants.ProductLicenseObjectName:
                        bShow = true;
                        dlvEditToolStripMenuItem.Visible = true;
                        toolStripSeparator2.Visible = true;
                        incrementToolStripMenuItem.Visible = true;
                        decrementToolStripMenuItem.Visible = true;
                        ProductLicense plProperty = DetailPropertyGrid.SelectedObject as ProductLicense;
                        if (m_Permissions.pt_module_pwd.Value && plProperty.IsActive)
                        {
                            if (item != null)
                            {
                                Module module = item.Tag as Module;
                                dlvEditToolStripMenuItem.Enabled = !s_CommLink.IsDefaultModule(plProperty.ProductID, module.ID);
                                incrementToolStripMenuItem.Enabled = plProperty.CanIncrementModule(module);
                                decrementToolStripMenuItem.Enabled = plProperty.CanDecrementModule(module);
                            }
                        }
                        break;
                    default: break;
                }
                if(DetailPropertyGrid.Enabled && bShow) //if propertygrid isn't enabled, don't allow changes.
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
            //need to know if values can be changed. 
            //1) check if propertygrid is enabled -> LicenseServer has been loaded, which determines status
            //      -> side effect may not refresh to reflect real time status
            if(m_Permissions.pt_create_modify_key.Value && DetailPropertyGrid.Enabled)
            {
                switch (e.KeyCode)
                {
                    case Keys.Delete: 
                        if(DetailListView.SelectedItems.Count > 0)
                            if (DetailPropertyGrid.SelectedObject is LicenseServer)
                                if (dlvRemoveToolStripButton.Enabled)
                                    RemoveValidationToken(DetailPropertyGrid.SelectedObject as LicenseServer);                        
                        break;
                    case Keys.Insert:
                        if (DetailPropertyGrid.SelectedObject is LicenseServer)
                            CreateValidationToken(DetailPropertyGrid.SelectedObject as LicenseServer);
                        break;
                    case Keys.Subtract:
                        DecrementSelectedModules();
                        break;
                    case Keys.Add:
                        IncrementSelectedModules(); 
                        break;
                    default: break;
                }
            }
        }

        private bool IncrementSelectedModules()
        {
            bool bRetVal = false;
            if (DetailPropertyGrid.SelectedObject is ProductLicense)
            {
                //throw error that is at max if available is 0
                ProductLicense plp = DetailPropertyGrid.SelectedObject as ProductLicense;
                if (plp.IsActive)
                {
                    foreach (ListViewItem lvItem in DetailListView.SelectedItems)
                    {
                        if (plp.GetAvailableModuleUnits(lvItem.Tag as Module) > 0)
                        {
                            IncrementModules(DetailListView.SelectedItems);
                            bRetVal = true;
                        }
                        //else throw error??
                    }
                }
            }
            return bRetVal;
        }

        private bool DecrementSelectedModules()
        {
            bool bRetVal = false;
            if (DetailPropertyGrid.SelectedObject is ProductLicense)
            {
                ProductLicense plp = DetailPropertyGrid.SelectedObject as ProductLicense;
                if (plp.IsActive)
                {
                    foreach (ListViewItem lvItem in DetailListView.SelectedItems)
                    {
                        if ((lvItem.Tag as Module).Value > 0)
                        {
                            DecrementModules(DetailListView.SelectedItems);
                            bRetVal = true;
                        }
                    }
                }
            }
            return bRetVal;
        }
        //available units is always positive.
        private void IncrementModules(ListView.SelectedListViewItemCollection lvItems)
        {
            string moduleName = string.Empty, available = string.Empty;
            bool bSuccess = true;
            ProductLicense productLicense = DetailPropertyGrid.SelectedObject as ProductLicense;           
            if(productLicense != null)
            {
                foreach(ListViewItem lvItem in lvItems)
                {
                    Module module = lvItem.Tag as Module;
                    module.Value++;
                    if(productLicense.SetModule(module))
                    {
                        //repopulate list view item 
                        lvItem.Tag = module;
                        lvItem.SubItems[1].Text = module.Value.ToString();
                        available = lvItem.SubItems[2].Text;
                        if (available != "Unlimited")
                            lvItem.SubItems[2].Text = (Int32.Parse(available) - 1).ToString();
                        SetModuleState(productLicense, lvItem);
                    }
                    else
                    {
                        moduleName = module.Name;
                        bSuccess = false;
                        break;
                    }
                }
                if (!bSuccess)
                    MessageBox.Show(string.Format("Failed to increment module : {0}", moduleName), "Module Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
        //units is always greater then 0
        private void DecrementModules(ListView.SelectedListViewItemCollection lvItems)
        {
            string moduleName = string.Empty, available = string.Empty;
            bool bSuccess = true;
            ProductLicense productLicense = DetailPropertyGrid.SelectedObject as ProductLicense;
            if(productLicense != null)
            {
                foreach(ListViewItem lvItem in lvItems)
                {
                    Module module = lvItem.Tag as Module;
                    module.Value--;
                    if(productLicense.SetModule(module))
                    {
                        //repopulate list view item 
                        lvItem.Tag = module;
                        //value 
                        lvItem.SubItems[1].Text = module.Value.ToString();
                        //available
                        available = lvItem.SubItems[2].Text;
                        if (available != "Unlimited")
                            lvItem.SubItems[2].Text = (Int32.Parse(available) + 1).ToString();
                        SetModuleState(productLicense, lvItem);
                    }
                    else
                    {         
                        moduleName = module.Name;
                        bSuccess = false;
                        break;
                    }
                }
                if(!bSuccess) 
                    MessageBox.Show(string.Format("Failed to increment module : {0}", moduleName), "Module Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        #region DetailListViewContextMenuStrip Events
        private void dlvNewToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (DetailPropertyGrid.SelectedObject is LicenseServer)
                CreateValidationToken(DetailPropertyGrid.SelectedObject as LicenseServer);
        }
        private void dlvDeleteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (DetailPropertyGrid.SelectedObject is LicenseServer)   //token remove
                RemoveValidationToken(DetailPropertyGrid.SelectedObject as LicenseServer);
        }
        #endregion
        #endregion

        #region DetailListViewToolStrip Events

        #region moduleFilterToolStripComboBox
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
        #endregion

        private void dlvAddToolStripButton_Click(object sender, EventArgs e)
        {
            if (DetailPropertyGrid.SelectedObject is LicenseServer)
                CreateValidationToken(DetailPropertyGrid.SelectedObject as LicenseServer);
        }
        private void dlvRemoveToolStripButton_Click(object sender, EventArgs e)
        {
            if (DetailPropertyGrid.SelectedObject is LicenseServer)   //token remove
                RemoveValidationToken(DetailPropertyGrid.SelectedObject as LicenseServer);
        }

        #endregion

        #region PropertyGridTabControl Events
        private void PropertyGridTabControl_Selecting(object sender, TabControlCancelEventArgs e)
        {
            if (PropertyGridTabControl.TabPages.Count > 0)
            {
                LoadSelectedTabPage(PropertyGridTabControl.SelectedTab);
            }
        }

        #region DetailPropertyGrid Events
        private void DetailPropertyGrid_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {
            if (!(e.OldValue == e.ChangedItem.Value))
            {
                //Special case for product license status change from add-on -> licensed
                //must remove add-on product license and set selectednode to parent
                if (DetailTreeView.SelectedNode.Level == 3)
                {
                    if (e.OldValue is ProductLicenseState)
                    {
                        ProductLicenseState oldStatus = (ProductLicenseState)e.OldValue;
                        ProductLicenseState newStatus = (ProductLicenseState)e.ChangedItem.Value;
                        if (oldStatus == ProductLicenseState.AddOn && newStatus == ProductLicenseState.LicensedAddOn)
                        {
                            TreeNode productNode = DetailTreeView.SelectedNode.Parent.Parent;
                            ProductCollection pcp = productNode.Tag as ProductCollection;
                            pcp.RemoveProductProperty(DetailTreeView.SelectedNode.Tag as ProductLicense);
                            //remove from product collection
                            int selectedIndex = DetailTreeView.SelectedNode.Index;
                            //set status of add-on to inactive.
                            //inactive add-on gets removed from tree, collection, but not deleted.
                            if (DetailTreeView.SelectedNode.Nodes.Count > 1)
                            {
                                selectedIndex = (selectedIndex > 0) ? --selectedIndex : ++selectedIndex;
                                DetailTreeView.SelectedNode = DetailTreeView.SelectedNode.Parent.Nodes[selectedIndex];
                            }
                            else
                                DetailTreeView.SelectedNode = DetailTreeView.SelectedNode.Parent;
                        }
                    }
                }
            }
            LoadDetailTreeViewSelectedNode(DetailTreeView.SelectedNode);
        }

        private void DetailPropertyGrid_SelectedObjectsChanged(object sender, EventArgs e)
        {
            if (DetailPropertyGrid.SelectedObject != null)
            {
                foreach (Control c in DetailPropertyGrid.Controls)
                {
                    if (string.Compare(c.GetType().FullName, "System.Windows.Forms.PropertyGridInternal.PropertyGridView", true, CultureInfo.InvariantCulture) == 0)
                    {
                        // add a custom service provider to give us control over the property value error dialog shown to the user
                        FieldInfo errorDialogField = c.GetType().GetField("serviceProvider", BindingFlags.Instance | BindingFlags.NonPublic);
                        try
                        {
                            errorDialogField.SetValue(c, new PropertyGridServiceProvider(DetailPropertyGrid));
                        }
                        catch (Exception ex) { string error = ex.Message; }
                    }
                }
            }
        }
        #endregion

        #endregion

        #region TransactionListView Events
        private void TransactionListView_ColumnClick(object sender, ColumnClickEventArgs e)
        {
            _lvManager.SetSortIndexColumn(TransactionListView.Handle, e.Column);
            // Perform the sort with these new sort options.
            TransactionListView.Sort();
            //SetSingleTransactionListViewItemBackColor();
        }
        #endregion

        #region transactionToolStripComboBox Events
        private void transactionToolStripComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            ShowTransactionListView(transactionToolStripComboBox.SelectedItem.ToString(), string.Empty);
        }
        #endregion

        #region HardwareKeyListView Events
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
        private void HardwareKeyListView_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            ListViewHitTestInfo hitInfo = HardwareKeyListView.HitTest(e.Location);
            if (HardwareKeyListView.SelectedItems.Count > 0)
            {
                ListViewItem item = HardwareKeyListView.SelectedItems[0];
                if (!(_selectedHardwareKeyCustomer.Length > 0))
                {
                    // Do a hit test for the current mouse position
                    // Test to see if the selected item and the hit test item are the same.
                    if (item.Equals(hitInfo.Item))
                    {
                        _selectedHardwareKeyCustomer = item.SubItems[1].Text;
                        //launch edit dialog
                        LoadHardwareKeyListView(string.Empty, _selectedHardwareKeyCustomer);
                    }
                }
                else
                {
                    if (item.SubItems[1].Text != string.Empty)
                        LoadSelectedLicenseServer(item.SubItems[1].Text);
                }
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
        
        #region ValidationTokenContextMenu Events
        private void viewLSToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (HardwareKeyListView.SelectedItems.Count > 0)
                LoadSelectedLicenseServer(HardwareKeyListView.SelectedItems[0].SubItems[1].Text);
        }
        private void validationTokenContextMenuStrip_Opening(object sender, CancelEventArgs e)
        {
            viewLSToolStripMenuItem.Enabled = false;
            if (HardwareKeyListView.SelectedItems.Count > 0)
                if (HardwareKeyListView.SelectedItems[0].SubItems[1].Text.Length > 0)
                    viewLSToolStripMenuItem.Enabled = true;
        }
        #endregion

        #endregion

        #region packetDescriptionRichTextBox Events
        private void packetDescriptionRichTextBox_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                verifyToolStripMenuItem.Visible = false;
                updateToolStripMenuItem.Visible = true;
                if (DetailPropertyGrid.Enabled) //don't allow changes, if propertygrid is disabled                
                    PacketContextMenuStrip.Show(packetDescriptionRichTextBox, e.X, e.Y);
            }
        }
        #endregion

        #region packetToolStripComboBox Events
        private void packetToolStripComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {            
            if ((sender as ToolStripComboBox).SelectedText.Length > 0)
            {
                switch (packetToolStripComboBox.SelectedIndex)
                {
                    case 0:
                        ShowPacketListView(string.Empty);
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
        }
        #endregion

        #region LicensePacketListView Events
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
                    foreach (ListViewItem packetItem in LicensePacketListView.Items)
                    {
                        if (packetItem.ImageIndex != 0)
                        {
                            verifyToolStripMenuItem.Enabled = true && DetailPropertyGrid.Enabled;
                            break;
                        }
                    }
                }
                //if (DetailPropertyGrid.Enabled) //don't allow changes, if propertygrid is disabled                
                if (bShow)
                    PacketContextMenuStrip.Show(LicensePacketListView, e.X, e.Y);
            }
        }
        private void LicensePacketListView_ColumnClick(object sender, ColumnClickEventArgs e)
        {
            _lvManager.SetSortIndexColumn(LicensePacketListView.Handle, e.Column);
            // Perform the sort with these new sort options.
            LicensePacketListView.Sort();
        }
        private void LicensePacketListView_SelectedIndexChanged(object sender, EventArgs e)
        {
            verifyToolStripMenuItem.Enabled = (LicensePacketListView.SelectedItems.Count > 0) ? true : false;
            splitContainer1.Panel2Collapsed = (LicensePacketListView.SelectedItems.Count > 0) ? false : true;
            if (LicensePacketListView.SelectedItems.Count > 0)
            {
                ListViewItem selectedItem = LicensePacketListView.SelectedItems[0];
                packetDescriptionRichTextBox.Text = (selectedItem.Tag as PacketTable).PacketComments;
            }
        }

        #region PacketContextMenuStrip

        private void verifyToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (PacketVerification dlg = new PacketVerification())
            {   //selectedItem text will be packet name
                PacketVerificationDialogData data = new PacketVerificationDialogData() { LicenseName = GetLicenseNameFromTreeView() };
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                {
                    if (data.Verified)
                    {                        //create transaction
                        TransactionManager.CreateTransaction(TransactionType.PacketVerification, 
                                                             data.LicenseName,
                                                             string.Empty, 
                                                             "Verified packet", 
                                                             data.SelectedPacketName,
                                                             string.Empty);
                        //needs to refill storagelistview
                        LoadPacketItems(data.LicenseName);
                        ListViewItem selectedItem = LicensePacketListView.Items[LicensePacketListView.Items.IndexOfKey(data.SelectedPacketName)];
                        selectedItem.Selected = true;
                    }
                }
            }
        }
        private void updateToolStripMenuItem_Click(object sender, EventArgs e)
        {
            PacketTable selectedPacket = (LicensePacketListView.SelectedItems[0]).Tag as PacketTable;
            selectedPacket.PacketComments = packetDescriptionRichTextBox.Text;
            //get selected packettable object
            Service<ICreator>.Use((client) =>
            {
                client.UpdatePacket(selectedPacket);
            });
        }
        #endregion
        #endregion

        #region reportsTreeView Events

        private void reportsTreeView_AfterSelect(object sender, TreeViewEventArgs e)
        {
            LoadReportNode(e.Node);
        }
        private void reportsTreeView_BeforeLabelEdit(object sender, NodeLabelEditEventArgs e)
        {
            if (reportsTreeView.SelectedNode.Level == 0)
                e.CancelEdit = true;
        }
        private void reportsTreeView_AfterLabelEdit(object sender, NodeLabelEditEventArgs e)
        {
            foreach (TreeNode node in reportsTreeView.Nodes[0].Nodes)
            {
                if (node.Text == e.Label)
                {
                    MessageBox.Show(string.Format("Invalid report name. {0} already exists!", e.Label), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    e.CancelEdit = true;
                    break;
                }
            }
            if (!e.CancelEdit)
            {
                Report selectedReport = e.Node.Tag as Report;
                _reportManager.DeleteReport(selectedReport);
                selectedReport.ID = (e.Label != null) ? e.Label : e.Node.Text;
                _reportManager.SaveReport(selectedReport);
            }
        }

        private void reportsTreeView_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                // Point where mouse is clicked
                Point p = new Point(e.X, e.Y);
                TreeNode node = reportsTreeView.GetNodeAt(p);
                if (node != null)
                    if (node.Bounds.Contains(p))
                        reportsTreeView.SelectedNode = node;
                ReportTreeViewContextMenuStrip.Show(reportsTreeView, p);
            }
        }

        #region ReportTreeViewContextMenuStrip
        private void CreateNewReport(Report.ReportType reportType)
        {
            using (ReportDlg dlg = new ReportDlg())
            {
                ReportDlgData data = new ReportDlgData(s_CommLink);
                int untitledCount = 0;
                foreach (TreeNode node in reportsTreeView.Nodes[0].Nodes)
                {
                    if (node.Text.Contains("Untitled Report"))
                        untitledCount++;
                }
                data.Report.ID = string.Format("Untitled Report {0}", untitledCount);
                data.Report.Type = reportType;
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                {
                    TreeNode newReportNode = new TreeNode();
                    newReportNode.Text = data.Report.ID;
                    newReportNode.Tag = data.Report;
                    newReportNode.ImageIndex = newReportNode.SelectedImageIndex = GetReportImageIndex(data.Report.Type);
                    reportsTreeView.Nodes[0].Nodes.Add(newReportNode);
                    reportsTreeView.SelectedNode = newReportNode;
                    reportsTreeView.SelectedNode.BeginEdit();
                }
            }
        }

        private void licensePacketReportToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CreateNewReport(Report.ReportType.LicensePacket);
        }

        private void hardwareTokenReportToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CreateNewReport(Report.ReportType.HardwareToken);
        }

        private void lsReportToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CreateNewReport(Report.ReportType.LicenseServer);
        }
        private void plReportToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CreateNewReport(Report.ReportType.ProductLicense);
        }
        private void deleteReportToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (reportsTreeView.SelectedNode != null)
            {
                if (reportsTreeView.SelectedNode.Level != 0)
                {
                    Report selectedReport = reportsTreeView.SelectedNode.Tag as Report;
                    if (MessageBox.Show(string.Format("Please confirm deletion of report: {0}.", 
                                        selectedReport.ID), 
                                        "Delete Report",
                                        MessageBoxButtons.OKCancel, 
                                        MessageBoxIcon.Exclamation) == DialogResult.OK)
                    {
                        TreeNode nextNode = null;
                        int index = reportsTreeView.SelectedNode.Index;
                        if(index > 0 && reportsTreeView.SelectedNode.Parent.Nodes.Count > 0)
                            nextNode = reportsTreeView.SelectedNode.Parent.Nodes[index - 1];
                        _reportManager.DeleteReport(selectedReport);
                        reportsTreeView.Nodes.Remove(reportsTreeView.SelectedNode);
                        if(nextNode != null) reportsTreeView.SelectedNode = nextNode;
                    }
                }
            }
        }
        private void editReportToolStripMenuItem_Click(object sender, EventArgs e)
        {
            TreeNode selectedNode = null;
            using (ReportDlg dlg = new ReportDlg())
            {
                ReportDlgData data = new ReportDlgData(s_CommLink);
                selectedNode = reportsTreeView.SelectedNode;
                data.Report = (reportsTreeView.SelectedNode.Tag as Report);
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                {
                    Report selectedReport = selectedNode.Tag as Report;
                    _reportManager.DeleteReport(selectedReport);
                    reportsTreeView.SelectedNode = null;
                    selectedNode.Tag = data.Report;
                    _reportManager.SaveReport(data.Report);
                    reportsTreeView.SelectedNode = selectedNode;
                }
            }
        }

        private void ReportTreeViewContextMenuStrip_Opening(object sender, CancelEventArgs e)
        {
            bool isReportSelected = ((reportsTreeView.SelectedNode.Tag as Report) != null);
            editReportToolStripMenuItem.Enabled = isReportSelected;
            deleteReportToolStripMenuItem.Enabled = isReportSelected;
            exportReportToolStripMenuItem.Enabled = isReportSelected;
            copyReportToolStripMenuItem.Enabled = isReportSelected;
            pasteReportToolStripMenuItem.Visible = false;
            if (_copyNode != null)
                if (_copyNode.Tag is Report)
                    pasteReportToolStripMenuItem.Visible = true;
            renameReportToolStripMenuItem.Enabled = isReportSelected;
        }

        private void csvToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (SaveFileDialog dlg = new SaveFileDialog())
            {
                dlg.Filter = "csv files (*.csv)|*.csv";
                dlg.RestoreDirectory = true;
                dlg.FileName = reportsTreeView.SelectedNode.Text;
                dlg.Title = "Export Report to CSV File";
                ExportListViewManager exportMgr = new ExportListViewManager();
                if (dlg.ShowDialog(this) == DialogResult.OK)
                {
                    exportMgr.ListViewToCSV(ReportListView, dlg.FileName, false);
                    //PacketDialogData selectedPacketData = selectedObject as PacketDialogData;
                    //packetOutputPathTextBox.Text = Path.Combine(folderBrowserDialog.SelectedPath, selectedPacketData.CustomerName);
                }
            }
            ////show file dialog box, ask file name, 
            //ExportListViewManager.ListViewToCSV(reportsTreeView, , false);
        }

        private void xmlToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void renameReportToolStripMenuItem_Click(object sender, EventArgs e)
        {
            reportsTreeView.SelectedNode.BeginEdit();
        }

        private void pasteReportToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //create new report based off saved report 
            //set new report to be edited
            //option will only be available if a reportnode has been set to copy
            Report copiedReport = _copyNode.Tag as Report;
            Report newReport = new Report(s_CommLink);
            int untitledCount = 0;
            foreach (TreeNode node in reportsTreeView.Nodes[0].Nodes)
            {
                if (node.Text.Contains("Untitled Report"))
                    untitledCount++;
            }
            newReport.Conditions.AddRange(copiedReport.Conditions);
            newReport.ID = string.Format("Untitled Report {0}", untitledCount);
            newReport.Type = copiedReport.Type;
            TreeNode newReportNode = new TreeNode();
            newReportNode.Text = newReport.ID;
            newReportNode.Tag = newReport;
            reportsTreeView.Nodes[0].Nodes.Add(newReportNode);
            reportsTreeView.SelectedNode = newReportNode;
            reportsTreeView.SelectedNode.BeginEdit();
            _copyNode = null;
        }

        private void copyReportToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //save report name to copy
            _copyNode = reportsTreeView.SelectedNode;
        }
        #endregion

        #endregion

        #region ReportListView Events
        private void ReportListView_ColumnClick(object sender, ColumnClickEventArgs e)
        {
            _lvManager.SetSortIndexColumn(ReportListView.Handle, e.Column);
            // Perform the sort with these new sort options.
            ReportListView.Sort();
        }
        private void ReportListView_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            ListViewHitTestInfo hitInfo = HardwareKeyListView.HitTest(e.Location);
            if (ReportListView.SelectedItems.Count > 0)
            {
                ListViewItem item = ReportListView.SelectedItems[0];
                switch ((reportsTreeView.SelectedNode.Tag as Report).Type)
                {
                    case Report.ReportType.LicenseServer:
                        if (item.SubItems[1].Text != string.Empty)
                            LoadSelectedLicenseServer(item.SubItems[1].Text);
                        break;
                    case Report.ReportType.ProductLicense:
                        if (item.SubItems[1].Text != string.Empty)
                        {
                            ProductLicenseTable plt = null;
                            Service<ICreator>.Use((client) =>
                            {//ignore add-on PL that has been licensed.
                                plt = client.GetProductLicense(item.SubItems[1].Text);
                            });
                            if (plt != null)
                            {
                                if (!(plt.plState == (byte)ProductLicenseState.Licensed && plt.ParentProductLicenseID != null))
                                    LoadSelectedProductLicense(item.SubItems[1].Text);
                                else
                                    MessageBox.Show(string.Format("Product License - {0} is no longer available to be viewed.", item.SubItems[1].Text), "Warning");
                            } //verify existence of product license - add-ons are not deleted when merged into parent
                        }
                        break;
                    default: break;
                }
            }
        }
        #endregion 
        #endregion

        #region Form Methods
        private int ConnectServer()
        {
            using (ConnectDialog cd = new ConnectDialog())
            {
                customerToolStripStatusLabel.Text = string.Empty;
                ConnectDialogData data = new ConnectDialogData(m_ServerList.ToArray());
                if (cd.ShowDialog(this, data) == DialogResult.OK)
                {
                    try
                    {
                        this.Text = AppConstants.ClientTitle + " [" + data.ServerList.First() +"]"; //1st is last used server
                        Service<ICreator>._channelFactory.Endpoint.Address = new EndpointAddress(new Uri("net.tcp://" + data.ServerList.First() + ":9091/Creator/tcp"));
                        Service<ICreator>.Use((client) =>
                        {
                            m_Permissions = client.GetPermissionsByUser(WindowsIdentity.GetCurrent().Name);
                        });
                        MainTabControl.Visible = true;
                        ResetMainToolStripMenu();
                        SearchCurrentView(string.Empty);
                        splitContainer2.Panel1.Controls.Clear();
                        //RestoreTreeState(DetailTreeView, m_TreeState);                    
                        loadingCircle1.Parent = splitContainer2.Panel1;
                        MainTabControl.Parent = splitContainer2.Panel1;
                    }
                    catch (COMException ex)
                    {
                    }
                }                //save server list into settings again 
                m_ServerList.Clear();
                m_ServerList.AddRange(data.ServerList);
            }
            return 0;
        }

        private void ShowPacketListView(string type)
        {
            LicensePacketListView.BeginUpdate();
            LicensePacketListView.Items.Clear();
            ListViewItem[] lvItems = new ListViewItem[storageListView.Items.Count];
            int index = 0;
            foreach (ListViewItem lvItem in storageListView.Items)
            {   //type length > 0 filter otherwise all
                if (lvItem.ImageIndex == Enums.GetIconIndex(type) || type.Length == 0)
                {
                    ListViewItem clonedItem = lvItem.Clone() as ListViewItem;
                    clonedItem.Name = lvItem.Name;
                    lvItems[index++] = clonedItem;
                }
            }
            LicensePacketListView.Items.AddRange(lvItems);
            _lvManager.AutoResizeColumns(LicensePacketListView);
            LicensePacketListView.EndUpdate();
            splitContainer1.Panel2Collapsed = (LicensePacketListView.SelectedItems.Count > 0) ? false : true;
        }

        private bool ValidTransactionType(string type, TransactionTable transaction)
        {       
            //knowing transaction type only helps for product 
            //need to know product license id for specific pl transactions
            bool bRetVal = false;
            //given PL id, LS id, 
            //figure out product transactions by examining productcollection 
            //each product license in product collection should have their PL id
            if(type == "Product")
            {
                ProductCollection currentProductCollection = null;
                switch (DetailTreeView.SelectedNode.Level)
                {
                    case 1:
                        currentProductCollection = DetailTreeView.SelectedNode.Tag as ProductCollection;
                        break;
                    case 2:
                        currentProductCollection = DetailTreeView.SelectedNode.Parent.Tag as ProductCollection;
                        break;
                    case 3:
                        currentProductCollection = DetailTreeView.SelectedNode.Parent.Parent.Tag as ProductCollection;
                        break;
                    default: break;
                }
                int[] idArray = new int[currentProductCollection.Collection.Count];
                if (currentProductCollection != null)
                {
                    int index = 0;
                    foreach (ProductLicense plp in currentProductCollection.Collection)
                    {
                        idArray[index++] = plp.ProductLicenseDatabaseID;
                    }
                    if (transaction.taOrderID.HasValue)
                        if(idArray.Where(id => id == transaction.taOrderID.Value).Count() > 0)
                            bRetVal = true;
                }
            }
            else if (type == "Product License")
            { 
                if(DetailTreeView.SelectedNode.Tag is ProductLicense)
                {
                    ProductLicense pl = DetailTreeView.SelectedNode.Tag as ProductLicense;                    
                    //specific to transaction product license id
                    if(transaction.taOrderID.HasValue && transaction.taOrderID.Value == pl.ProductLicenseDatabaseID)
                        bRetVal = true;
                }
            }
            else
                bRetVal = true; 
            return bRetVal;
        }

        //product transactions = all transactions belonging to PLs of a particular product
        //type -> All, Product, Product Licenses
        //All = everything
        //Product = all lvitems with description containing detailtreeview.node level or all
        //Product License = all lvitems with plid or product or all
        private void ShowTransactionListView(string type, string searchKey)
        {
            TransactionListView.BeginUpdate();
            TransactionListView.Items.Clear();           
            List<ListViewItem> items = new List<ListViewItem>();
            /*ListViewItem[] items = new ListViewItem[storageListView.Items.Count];
            int index = 0;
             */
            foreach (ListViewItem lvItem in storageListView.Items)
            {
                if (ValidTransactionType(type, lvItem.Tag as TransactionTable))                    
               //     items[index++] = lvItem.Clone() as ListViewItem;
                    items.Add(lvItem.Clone() as ListViewItem);                
            }
            TransactionListView.Items.AddRange(items.ToArray());
            _lvManager.AutoResizeColumns(TransactionListView);
            TransactionListView.EndUpdate();
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

        private void findSearchString(string findString)
        {
            //determine if the string is a license server or product license
            //
            bool bFound = false;
            string[] findStringArray = findString.Split("-".ToCharArray());
            //valid search string LS = 3, PL = 4
            if (findStringArray.Count() == 3 || findStringArray.Count() == 4)
            {
                TreeNode findNode = null;
                if (findStringArray.Count() == 3)
                {
                    findNode = DetailTreeView.Nodes.Find(findString, false).FirstOrDefault();
                    if (findNode != null)
                    {
                        DetailTreeView.SelectedNode = findNode;
                        bFound = true;
                    }
                }
                else
                {
                    //pull from database 
                    Service<ICreator>.Use((client) =>
                    {
                        if (client.GetProductLicense(findToolStripTextBox.Text) != null)
                        {
                            findNode = DetailTreeView.Nodes.Find(findString.Substring(0, findString.LastIndexOf("-")), false).FirstOrDefault();
                            findNode.ExpandAll();
                            findNode = findNode.Nodes.Find(findString, true).FirstOrDefault();
                            if (findNode != null)
                            {
                                DetailTreeView.SelectedNode = findNode;
                                bFound = true;
                            }
                        }
                    });
                }
            }
            if (!bFound)
                MessageBox.Show("Failed to find " + findToolStripTextBox.Text, "Find", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        //issue std selected license node is customer node
        //      all others are std license node 
        private void SearchCurrentView(string searchString)
        {
            if (searchToolStripTextBox.ForeColor == SystemColors.InactiveCaptionText)
                searchToolStripTextBox.Text = string.Empty;
            if (MainTabControl.SelectedTab == LicensesTabPage)
            {
                if (CustomersListView.Visible)
                    LoadDBCustomers(searchString, false);
                else
                    LoadDBLicenses(searchString, string.Empty, DestNameComboBox.Text, true);
                searchToolStripTextBox.Focus();
            }
            else if (MainTabControl.SelectedTab == ValidationKeysTabPage)
                LoadHardwareKeyListView(searchString, _selectedHardwareKeyCustomer);
        }
        //Problem : remove/add conflicts with edit because module compare thinks edits are new/remove modules}
        private void LoadSelectedTabPage(TabPage selectedTab)
        {
            if (!DetailSplitContainer.Panel2Collapsed)
            {
                switch (selectedTab.Name)
                {
                    case AppConstants.PropertyGridTabPage:
                        LoadDetailListView(DetailPropertyGrid.SelectedObject); 
                        break;
                    case AppConstants.TransactionsTabPage:
                        transactionToolStripComboBox.Items.Clear();
                        transactionToolStripComboBox.Items.Add("All");
                        if (DetailTreeView.SelectedNode.Level >= 1)
                            transactionToolStripComboBox.Items.Add("Product");
                        if (DetailTreeView.SelectedNode.Level > 1)
                            transactionToolStripComboBox.Items.Add("Product License");
                        LoadTransactionItems(m_CurrentLicenseName);
                        break;
                    case AppConstants.PacketTabPage:
                        LoadPacketItems(m_CurrentLicenseName);
                        break;
                    default: break;

                }
            }
        }

        #region License Server Methods
        private void CreateStandardLicense()
        {
            LicenseServer newLicense = new LicenseServer();
            Service<ICreator>.Use((client) =>
            {
                CustomerTable custRec = client.GetCustomer(_selectedLicenseCustomer.Name, false);
                if (custRec == null)
                {
                    MessageBox.Show("Failed to initialize standard license", "Create License Server");
                    return;
                }
                newLicense.CustID = (uint)custRec.SCRnumber;
                DestinationNameTable dnt = client.GetDestinationID(_selectedLicenseCustomer.Id, DestNameComboBox.Text);
                newLicense.DestID = (uint)dnt.DestID;
                newLicense.GroupID = client.GetNextGroupID(newLicense.CustID, newLicense.DestID);
            });
            newLicense.Permissions = m_Permissions;
            if (MessageBox.Show(string.Format("Please confirm creation of License Server : {0}.", newLicense.Name), 
                                "Create License Server",
                                MessageBoxButtons.OKCancel, 
                                MessageBoxIcon.Question) == DialogResult.OK)            
                CreateLicenseNode(newLicense);
        }

        private void CreateLicenseEntity(LicenseServer licProperties)
        {
            //Create new license file from Customer id and next available swID
            Service<ICreator>.Use((client) =>
            {
                Lic_PackageAttribs licInfo = new Lic_PackageAttribs();
                licInfo.licLicenseInfoAttribs.TVal = licProperties.LicInfo; //licproperties has custid, destid, groupid set
                LicenseTable licRecord = new LicenseTable()
                {
                    GroupID = (int)licProperties.GroupID,
                    DestinationID = (int)licProperties.DestID,
                    SCRnumber = (int)licProperties.CustID,
                    LicenseName = licProperties.Name,
                    IsActive = true,
                    LicenseComments = licProperties.Comments,
                    OrderIndex = licProperties.ProductLicenseIndex,
                    LicenseInfo = licInfo.Stream       

                };
                client.CreateLicense(licRecord);
            });
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
                List<TokenTable> tokens = client.GetTokensByLicenseName(licRecord.LicenseName);//possibly need to check for null
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
        private void CreateLicenseNode(LicenseServer lsp)
        {
            CreateLicenseEntity(lsp);
            //resort tree
            LoadDBLicenses(string.Empty, lsp.Name, lsp.DestName, true);
        }
        private void DeleteLicenseNode(TreeNode selectedLicenseNode)//, bool bVerify)
        {
            LicenseServer selectedLicense = selectedLicenseNode.Tag as LicenseServer;
            if (MessageBox.Show(string.Format("Please confirm deletion of License Server : {0}.", selectedLicense.Name), 
                                "Delete License Server",
                                MessageBoxButtons.OKCancel, 
                                MessageBoxIcon.Exclamation) == DialogResult.OK)
            {
                int selectedIndex = selectedLicenseNode.Index;
                DeleteAllProductLicenseEntities(selectedLicense.Name);
                selectedLicenseNode.Nodes.Clear();
                DeleteLicenseEntity((selectedLicenseNode.Tag as LicenseServer).Name);
                DetailTreeView.Nodes.Remove(selectedLicenseNode);
                if (selectedIndex > 0)
                    selectedIndex = selectedIndex - 1;
                if (DetailTreeView.Nodes.Count > 0)
                    DetailTreeView.SelectedNode = DetailTreeView.Nodes[selectedIndex];
                else
                {
                    TreeNode emptyNode = new TreeNode("No License Servers Available");
                    emptyNode.Name = "Empty";
                    emptyNode.ImageIndex = emptyNode.SelectedImageIndex = Enums.GetIconIndex("Empty");
                    DetailTreeView.Nodes.Add(emptyNode);
                    LicenseViewSplitContainer.Panel2Collapsed = true;
                    EnableToolStripMenu(emptyNode);
                }
                if (DetailTreeView.SelectedNode != null)
                    SetLicenseServerState(DetailTreeView.SelectedNode, true);
            }
        }
        //CONTACT DB : 2
        private void LoadLicenseNode(TreeNode licenseNode) //loads LS data and product collection data
        {
            string productName;
            LicenseTable license = null;
            List<ProductLicenseTable> pltList = null;
            Service<ICreator>.Use((client) =>
            {
                license = client.GetLicenseByName(licenseNode.Name, true); // db - 1             
                if (license != null)
                {
                    licenseNode.Text = (license.UserLock != null) ? license.LicenseName + " - " + license.UserLock : license.LicenseName;
                    licenseNode.Tag = new LicenseServer(license, m_Permissions);
                    pltList = client.GetProductLicenses(license.LicenseName, false); // db - 2
                }
                else
                    licenseNode.Tag = null;
            });
            //need to clear out existing product collection
            TreeNode productNode = null;
            if (pltList != null)
            {
                foreach (ProductLicenseTable plt in pltList)
                {   //selectednode = licenseName
                    productName = s_CommLink.GetProductName(plt.ProductID);
                    if (!licenseNode.Nodes.ContainsKey(productName))
                    { 
                        productNode = new TreeNode(productName);
                        productNode.Name = productName;
                        productNode.ImageIndex = productNode.SelectedImageIndex = Enums.GetIconIndex(s_CommLink.GetProductBaseName(productName));
                        productNode.Nodes.Add(new VirtualTreeNode());
                        //load products also
                        productNode.Tag = new ProductCollection(plt.ProductID);
                        (productNode.Tag as ProductCollection).Collection.Add(new ProductLicense(plt, m_Permissions));
                        bool bAdded = false;
                        if (licenseNode.Nodes.Count > 0)
                        {
                            foreach (TreeNode node in licenseNode.Nodes) //collection of product nodes
                            {                                
                                if (string.Compare(node.Text, productNode.Text) > 0) //insert in alpha order
                                {
                                    licenseNode.Nodes.Insert(node.Index, productNode);
                                    bAdded = true;
                                    break;
                                }
                            }
                        }
                        if (!bAdded) licenseNode.Nodes.Add(productNode);                           
                    }
                    else
                    {   //update
                        productNode = licenseNode.Nodes.Find(productName, false).First();
                        (productNode.Tag as ProductCollection).UpdateProductProperty(new ProductLicense(plt, m_Permissions));
                    }
                }      
                //update product collection
                List<TreeNode> removeNodes = new List<TreeNode>();
                //update product nodes
                foreach (TreeNode prodNode in licenseNode.Nodes)
                {   //remove any nodes that may have been removed.             
                    if (prodNode.Name.Length > 0)
                    {
                        if (pltList.Where(p => p.ProductID == s_CommLink.GetProductID(prodNode.Name)).Count() == 0)
                            removeNodes.Add(prodNode);
                        else
                        {
                            ProductCollection pcp = prodNode.Tag as ProductCollection;
                            List<ProductLicense> removeProductLicenses = new List<ProductLicense>();
                            foreach (ProductLicense pp in pcp.Collection)
                            {
                                if(pltList.Where(p => p.plID == pp.ID).Count() == 0)
                                    removeProductLicenses.Add(pp);
                            }
                            foreach (ProductLicense product in removeProductLicenses)
                            {
                                pcp.RemoveProductProperty(product);
                            }
                        }
                    }
                }
                foreach (TreeNode removeNode in removeNodes)
                {
                    licenseNode.Nodes.Remove(removeNode);
                }
            }
        }

        private void SetDetailTreeViewLayout()
        {   //configure layout depending on number of tree nodes
            if (DetailTreeView.Nodes.Count > 0) //
            {
                LicenseViewSplitContainer.Panel2Collapsed = false;
                DetailTreeView.Enabled = true;
            }
            else
            {
                TreeNode emptyNode = new TreeNode("No License Servers Available");
                emptyNode.Name = "Empty";
                emptyNode.ImageIndex = emptyNode.SelectedImageIndex = Enums.GetIconIndex("Empty");
                DetailTreeView.Nodes.Add(emptyNode);
                LicenseViewSplitContainer.Panel2Collapsed = true;
            }
            navigateHomeToolStripButton.Enabled = true;
        }

        private void SelectDetailTreeViewNode(string selectedName)
        {   //selected node should be first node.
            if (DetailTreeView.Nodes.Count > 0)
            {
                if (selectedName.Length > 0)
                {
                    if (selectedName.Split("-".ToCharArray()).Count() == 3)
                    {
                        DetailTreeView.SelectedNode = DetailTreeView.Nodes.Find(selectedName, false).FirstOrDefault();
                    }
                    else
                    {
                        string licenseServer = selectedName.Remove(selectedName.LastIndexOf("-"), selectedName.Length - selectedName.LastIndexOf("-"));
                        DetailTreeView.SelectedNode = DetailTreeView.Nodes.Find(licenseServer, false).FirstOrDefault();
                        DetailTreeView.SelectedNode.ExpandAll();
                        DetailTreeView.SelectedNode = DetailTreeView.SelectedNode.Nodes.Find(selectedName, true).FirstOrDefault();
                    }
                }
                else
                    DetailTreeView.SelectedNode = DetailTreeView.Nodes[0];
                SetLicenseServerState(DetailTreeView.SelectedNode, true);
            }
            else //empty treeview, disable propertygrid
                DetailPropertyGrid.SelectedObject = null;
        }

        private void LoadLicenseView(IList<LicenseTable> licRecords, string selectedName)
        {
            DetailTreeView.BeginUpdate();
            DetailTreeView.Nodes.Clear();
            //add each license server to tree view
            if (licRecords.Count > 0)
            {
                TreeNode[] nodes = new TreeNode[licRecords.Count()];
                int index = 0;
                //add all to stored list
                foreach (var license in licRecords.OrderBy(d => d.DestinationID).ThenBy(g => g.GroupID))
                {
                    TreeNode licNode = new TreeNode();
                    licNode.Text = license.LicenseName;
                    if (license.UserLock != null) licNode.Text += " - " + license.UserLock;
                    licNode.Name = license.LicenseName;
                    licNode.ImageIndex = Enums.GetIconIndex("License");
                    licNode.SelectedImageIndex = licNode.ImageIndex;
                    if (!licNode.IsExpanded)
                        licNode.Nodes.Add(new VirtualTreeNode());
                    licNode.Tag = new LicenseServer(license, m_Permissions);
                    nodes[index++] = licNode;
                }
                DetailTreeView.Nodes.AddRange(nodes);
            }
            SetDetailTreeViewLayout();
            SelectDetailTreeViewNode(selectedName);
            DetailTreeView.EndUpdate();
        }

        private void LoadSelectedLicenseServer(string selectedLicense)
        {            
            //load licenses, highlight particular license
            CustomersListView.Visible = false;
            findToolStrip.Visible = false;
            MainTabControl.SelectedTab = LicensesTabPage;
            //get destination from string or.....store in tag?
            string[] lsString = selectedLicense.Split("-".ToCharArray());
            int custID = Int32.Parse(lsString[0], System.Globalization.NumberStyles.HexNumber);
            int destID = Int32.Parse(lsString[1], System.Globalization.NumberStyles.HexNumber);
            //1)custid 2)destid 3) number 
            Service<ICreator>.Use((client) =>
            {
                _selectedLicenseCustomer = new Customer(client.GetCustomer(custID.ToString(), false));
            });
            _selectedTreeNodeKey = selectedLicense;
            LoadDestinationNameComboBox(custID, destID);
        }
        private void LoadDestinationNameComboBox(Customer custProperty)
        {
            DestNameComboBox.Items.Clear();
            Service<ICreator>.Use((client) =>
            {
                List<DestinationNameTable> destNames = client.GetDestNamesByCustID(custProperty.Id);
                if (destNames.Count < 1) //create default destname if not found
                {
                    DestinationNameTable destRec = new DestinationNameTable()
                    {
                        CustID = custProperty.Id,
                        DestID = 0,
                        DestName = custProperty.Name
                    };
                    client.CreateDestinationName(destRec);
                    destNames = client.GetDestNamesByCustID(custProperty.Id);
                }
                foreach (DestinationNameTable dnt in destNames)
                {
                    DestNameComboBox.Items.Add(dnt.DestName);
                }
                if (m_Permissions.pt_create_modify_key.Value)
                {
                    DestNameComboBox.Items.Add("<Create...>");
                    DestNameComboBox.Items.Add("<Edit...>");
                }
                if (destNames.Count > 0)
                    DestNameComboBox.SelectedIndex = DestNameComboBox.Items.IndexOf(destNames.Where(c => c.DestID.Equals(0)).FirstOrDefault().DestName);
            });
        }
        private void LoadDestinationNameComboBox(int custID, int destID)
        {
            List<DestinationNameTable> destNames = null;
            DestNameComboBox.Items.Clear();
            Service<ICreator>.Use((client) =>
            {
                destNames = client.GetDestNamesByCustID(custID);
            });
            foreach (DestinationNameTable dnt in destNames)
            {
                DestNameComboBox.Items.Add(dnt.DestName);
            }
            if (m_Permissions.pt_create_modify_key.Value)
            {
                DestNameComboBox.Items.Add("<Create...>");
                DestNameComboBox.Items.Add("<Edit...>");
            }
            if (destNames.Count() > 0)
            {
                DestinationNameTable dnt = destNames.Where(c => c.DestID == destID).FirstOrDefault();
                if (dnt != null)
                    DestNameComboBox.SelectedIndex = DestNameComboBox.Items.IndexOf(dnt.DestName);
            }
            else
                LoadDBLicenses(string.Empty, string.Empty, string.Empty, true);
        }
        private void SetLicenseServerState(TreeNode currentNode, bool bAllLicenseServers)
        {
            TreeNode licenseNode = null;
            switch (currentNode.Level)
            {
                case 1: //product
                    licenseNode = currentNode.Parent;
                    break; 
                case 2: //product license
                    licenseNode = currentNode.Parent.Parent;
                    break;
                case 3: //add-on product license
                    licenseNode = currentNode.Parent.Parent.Parent;
                    break;
                default:
                    licenseNode = currentNode;
                    break; //license server
            }

            LicenseServer selectedLicense;
            IList<string> modifiedLicenses = null;
            IList<string> enabledLicenses = null;
            Service<ICreator>.Use((client) =>
            {
                modifiedLicenses = client.GetModifiedLicensesByCustomer(_selectedLicenseCustomer.Name);
                enabledLicenses = client.GetEnabledLicensesByCustomer(_selectedLicenseCustomer.Name);
            });
            foreach (TreeNode node in DetailTreeView.Nodes)
            {
                if (!bAllLicenseServers)
                    if (node.Name != licenseNode.Name)
                        continue;   //skip over all license nodes that don't match
                if (node.Tag != null)
                {
                    selectedLicense = node.Tag as LicenseServer;
                    node.ToolTipText = string.Empty;
                    if (!selectedLicense.IsActive)
                    {
                        node.ImageIndex = node.SelectedImageIndex = Enums.GetIconIndex("LicenseDeactivated");
                        node.NodeFont = new Font(this.Font, FontStyle.Italic);
                        node.ForeColor = SystemColors.InactiveCaptionText;
                    }
                    else
                    {
                        node.NodeFont = this.Font;
                        node.ForeColor = this.ForeColor;
                        node.ImageIndex = node.SelectedImageIndex = Enums.GetIconIndex("License");
                        if (modifiedLicenses != null)
                        {
                            if (modifiedLicenses.Contains(selectedLicense.Name))
                            {
                                node.ImageIndex = node.SelectedImageIndex = Enums.GetIconIndex("LicenseUpdate");
                                node.ToolTipText = "Changes have been made to license. Generate license packet.";
                            }
                        }
                        if (enabledLicenses != null)
                        {
                            if (!enabledLicenses.Contains(selectedLicense.Name))
                            {
                                node.ImageIndex = node.SelectedImageIndex = Enums.GetIconIndex("LicenseDisabled");
                                node.ToolTipText = "Not enough validation tokens are set. Add a new validation token to enable license.";
                            }
                        }
                    }
                }
                if (!bAllLicenseServers)
                    break;
            }
        }
        #endregion

        #region Validation Token Methods
        public void CreateValidationToken(LicenseServer licInfo)
        {
            using (LicenseInfoForm dlg = new LicenseInfoForm("New Validation Token(s) for " + licInfo.Name, ref s_CommLink))
            {
                TokenDialogData data = new TokenDialogData(licInfo);
                string HardwareTokenValue = string.Empty;
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                {
                    //take all tokens from data and add tokens to db then add list to license db
                    TokenTable dbToken = null;
                    Service<ICreator>.Use((client) =>
                    {
                        LicenseTable dbLicense = client.GetLicenseByName(licInfo.Name, false);
                        if (dbLicense == null)
                        {
                            MessageBox.Show("Failed to access license for new token", "Create New Validation Token");
                            return;
                        }
                        foreach (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs token in data.TokenList)
                        {
                            if (token.tokenType.TVal != Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttLicenseCode)
                            {
                                if (token.tokenType.TVal == Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID)
                                {
                                    dbToken = client.GetHardwareTokenByKeyValue(token.tokenValue.TVal);
                                    if (dbToken == null)
                                    {
                                        MessageBox.Show("Failed to fine hardware token :" + token.tokenValue.TVal + " for creation.");
                                        return;
                                    }
                                    HardwareTokenValue = dbToken.TokenValue;
                                    dbToken.LicenseID = dbLicense.ID;
                                    client.UpdateToken(dbToken);
                                }
                                else
                                {
                                    dbToken = new TokenTable()
                                    {
                                        LicenseID = dbLicense.ID,
                                        TokenType = (byte)token.tokenType.TVal,
                                        TokenValue = token.tokenValue.TVal,
                                        CustID = (int)data.LicInfo.CustID
                                    };
                                    client.CreateToken(dbToken);
                                }
                            }
                        }
                        string transactionMsg = (HardwareTokenValue != string.Empty) ? HardwareTokenValue : "Software";
                        TransactionManager.CreateTransaction(TransactionType.Token,
                                          licInfo.Name,
                                          string.Empty,
                                          "Add Validation Token(s)",
                                          transactionMsg,
                                          string.Empty);
                        client.MarkDirty(licInfo.Name);
                    });
                    PopulateDetailListView(data.LicInfo);
                    SetLicenseServerState(DetailTreeView.SelectedNode,false);
                }
            }
        }
        private void RemoveValidationToken(LicenseServer selectedLicense)
        {
            string hardwareTokenValue = string.Empty;
            if (MessageBox.Show("Deactivating tokens will disable license. Click OK to continue with deactivation of tokens.", 
                                "Deactivate Token(s)",
                                MessageBoxButtons.OKCancel, 
                                MessageBoxIcon.Warning)== DialogResult.OK)
            {
                selectedLicense.LicInfo.licVerificationAttribs.TVal.validationTokenList.TVal.Clear();
                Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs licCodeToken = new Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs();
                licCodeToken.tokenType.TVal = Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttLicenseCode;
                selectedLicense.LicInfo.licVerificationAttribs.TVal.validationTokenList.TVal.Add(licCodeToken);
                Service<ICreator>.Use((client) =>
                {
                    foreach (TokenTable token in client.GetTokensByLicenseName(selectedLicense.Name))
                    {                              
                        if (token != null)
                        {
                            if (token.TokenType == (byte)Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID)
                            {
                                hardwareTokenValue = token.TokenValue;
                                token.LicenseID = -1;
                                client.UpdateToken(token);
                            }
                            else
                            {   //deactivate sw tokens
                                token.TokenStatus = (byte)TokenStatus.Deactivated;
                                client.UpdateToken(token);
                            }   //client.DeleteToken(token);
                        }
                    }
                    string transactionMsg = (hardwareTokenValue != string.Empty) ? hardwareTokenValue : "Software";
                    TransactionManager.CreateTransaction(TransactionType.Token,
                                                         selectedLicense.Name,
                                                         string.Empty,
                                                         "Deactivate Token(s)",
                                                         transactionMsg,
                                                         string.Empty);
                    client.MarkDirty(selectedLicense.Name);
                });
            }
            PopulateDetailListView(selectedLicense);
            SetLicenseServerState(DetailTreeView.SelectedNode, false);
        }

        #endregion

        #region Product Methods
        //create only done on new contract with new product        
        private void LoadProductNode(TreeNode productNode)
        {
            List<ProductLicenseTable> plRecords = null;
            if (productNode.Parent != null)
            {
                Service<ICreator>.Use((client) =>
                {
                    plRecords = client.GetProductLicensesByProduct(productNode.Parent.Name, (byte)s_CommLink.GetProductID(productNode.Name));
                });
                TreeNode plParent;
                ProductLicense selectedProductLicense;
                if (plRecords != null)
                {
                    //System.Diagnostics.Trace.WriteLine("PopulateListView() - Start");
                    /*System.Diagnostics.Stopwatch watch = new System.Diagnostics.Stopwatch();
                    watch.Start();
                    */
                    foreach (ProductLicenseTable plt in plRecords)
                    {
                        TreeNode plNode = null;
                        selectedProductLicense = (productNode.Tag as ProductCollection).GetProductLicenseProperty(plt.plID);
                        if (productNode.Nodes.Find(plt.plID, true).Count() == 0) //create new product node
                        {
                            plNode = new TreeNode(plt.plID);
                            plNode.Name = plt.plID;
                            plNode.Tag = new ProductLicense(plt, m_Permissions);
                            if (plt.ParentProductLicenseID != null)
                            {
                                plNode.ImageIndex = plNode.SelectedImageIndex = Enums.GetIconIndex("ADDONORDER");                                
                                plParent = productNode.Nodes.Find(plt.ParentProductLicenseID, false).FirstOrDefault();
                                if (plParent != null)                                
                                    if (!plParent.Nodes.ContainsKey(plNode.Name))
                                        plParent.Nodes.Add(plNode);                                
                            }
                            else
                            {
                                plNode.ImageIndex = plNode.SelectedImageIndex = Enums.GetIconIndex("ORDER");
                                productNode.Nodes.Add(plNode);
                            }
                        }
                        else //update found product node
                        {
                            plNode = productNode.Nodes.Find(plt.plID, true).First();
                            plNode.Tag = new ProductLicense(plt, m_Permissions);
                        }
                        //setup node style
                        plNode.ForeColor = this.ForeColor;
                        plNode.NodeFont = this.Font;
                        if ((plNode.Tag as ProductLicense).IsExpired)
                        {
                            plNode.NodeFont = new Font(this.Font, FontStyle.Italic);
                            plNode.ForeColor = Color.Red;
                        }
                        if (!(plNode.Tag as ProductLicense).IsActive)
                        {
                            plNode.NodeFont = new Font(this.Font, FontStyle.Italic);
                            plNode.ForeColor = SystemColors.InactiveCaptionText;
                        }
                    }

                    /*watch.Stop();
                    MessageBox.Show(string.Format("DetailTreeView_AfterSelect Elapsed: {0}\nIn milliseconds: {1}\nIn timer ticks: {2}", watch.Elapsed, watch.ElapsedMilliseconds, watch.ElapsedTicks), "INFORMATION", MessageBoxButtons.OK);
                    */
                    //System.Diagnostics.Trace.WriteLine("PopulateListView() - End");
                    if (productNode.Nodes.Count > 0) //remove extra nodes, how to load without needing this?
                    {   //Remove nodes that do not exist anymore
                        foreach (TreeNode plNode in productNode.Nodes)
                        {
                            if (plNode.Nodes.Count > 0)
                            {
                                foreach (TreeNode addonNode in plNode.Nodes)
                                {
                                    if (plRecords.Find(c => c.plID.Equals((addonNode.Name))) == null)
                                        plNode.Nodes.Remove(addonNode);
                                }
                            }
                            if (plRecords.Find(c => c.plID.Equals((plNode.Name))) == null)
                                productNode.Nodes.Remove(plNode);
                        }
                    }
                    else
                        productNode.Tag = null;
                }
            }
        }

        private TreeNode CreateProductNode(ProductLicense plData)
        {
            TreeNode node = new TreeNode(plData.ProductName)
            {           
                Name = plData.ProductName,
                ImageIndex = Enums.GetIconIndex(s_CommLink.GetProductBaseName(plData.ProductName)),
                SelectedImageIndex = Enums.GetIconIndex(s_CommLink.GetProductBaseName(plData.ProductName)),
                Tag = new ProductCollection(plData.ProductID)
            };
            (node.Tag as ProductCollection).Collection.Add(plData);
            return node;
        }
        #endregion

        #region Product License Methods
        private TreeNode CreateProductLicenseNode(ProductLicenseTable pltData)
        {            
            pltData = CreateProductLicenseEntity(pltData);
            //add modules to db
            TreeNode plNode = new TreeNode(pltData.plID);
            plNode.Name = pltData.plID;
            plNode.Tag = new ProductLicense(pltData, m_Permissions);
            (plNode.Tag as ProductLicense).ProductVersion = new LicenseVersion(pltData.ProductVersion);
            if (pltData.ParentProductLicenseID != null)
            {
                plNode.ImageIndex = Enums.GetIconIndex("ADDONORDER");
                plNode.SelectedImageIndex = plNode.ImageIndex;
            }
            else
            {
                plNode.ImageIndex = Enums.GetIconIndex("ORDER");
                plNode.SelectedImageIndex = plNode.ImageIndex;
            }
            return plNode;
        }
        private ProductLicenseTable CreateProductLicenseEntity(ProductLicenseTable pltData)
        {
            //pltRec already created and reserved. Retrieve and populate attributes
            ProductLicenseTable plRec = new ProductLicenseTable();
            Service<ICreator>.Use((client) =>
            {
                LicenseTable licRec = client.GetLicenseByID(pltData.LicenseID, false);
                if (licRec == null)
                {
                    MessageBox.Show("Failed to find license server for " + pltData.plID, "Create New Product License");
                    return;
                }
                plRec.plID = pltData.plID;
                plRec.plIndex = pltData.plIndex;
                plRec.plState = pltData.plState;
                plRec.LicenseID = pltData.LicenseID;
                plRec.IsActive = true;
                plRec.ExpirationDate = pltData.ExpirationDate;
                plRec.Description = pltData.Description;
                plRec.ProductID = pltData.ProductID;
                plRec.ProductVersion = pltData.ProductVersion;
                plRec.ParentProductLicenseID = pltData.ParentProductLicenseID;
                plRec.ProductConnection = pltData.ProductConnection;
                client.CreateProductLicense(plRec);
                //initialize products
                TransactionManager.CreateTransaction(TransactionType.LicenseServer,
                                                      string.Empty,
                                                      pltData.plID,
                                                      string.Format("Add {0} Product License", s_CommLink.GetProductName(pltData.ProductID)),
                                                      pltData.plID,
                                                      string.Empty);
                //update order index to next available index                
                licRec.OrderIndex = plRec.plIndex + 1;
                client.UpdateLicense(licRec, true);
                pltData = client.GetProductLicense(pltData.plID);
            });
            return pltData;
        }

        //node is current selected product license node
        private void DeleteProductLicenseNode(TreeNode plNode)
        {
            TreeNode productNode;
            if (plNode != null)
            {
                ProductLicense plData = plNode.Tag as ProductLicense;
                if (MessageBox.Show(string.Format("Please confirm deletion of product license: {0}.", 
                                    plData.ID), 
                                    "Delete Product License",
                                    MessageBoxButtons.OKCancel, 
                                    MessageBoxIcon.Exclamation) == DialogResult.OK)
                {
                    productNode = (plData.ParentID != null) ? productNode = plNode.Parent.Parent : plNode.Parent;
                    int selectedIndex = plNode.Index;
                    ProductLicenseTable pltDelete = null;
                    ProductLicense selectedProductLicense = (plNode.Tag as ProductLicense);
                    Service<ICreator>.Use((client) => { pltDelete = client.GetProductLicense(selectedProductLicense.ID); });
                    DeleteProductLicenseEntity(pltDelete);
                    ProductCollection pcp = productNode.Tag as ProductCollection;
                    pcp.RemoveProductProperty(selectedProductLicense);
                    if (pcp.Collection.Count == 0)
                    {
                        productNode.Remove();
                        DetailTreeView.SelectedNode = productNode.Parent;
                    }
                    else
                    {
                        if (plNode.Parent.Nodes.Count > 1)
                        {   
                            selectedIndex = (selectedIndex > 0) ? --selectedIndex : ++selectedIndex;
                            DetailTreeView.SelectedNode = productNode.Nodes[selectedIndex];
                        }
                        else //delete last addon -> go to parent PL
                            DetailTreeView.SelectedNode = plNode.Parent;
                    }
                    if (DetailTreeView.SelectedNode != null)
                        SetLicenseServerState(DetailTreeView.SelectedNode, true);                    
                }
            }
        }
        private void DeleteAllProductLicenseEntities(string licenseServer)
        {
            Service<ICreator>.Use((client) =>
            {
                client.DeleteAllProductLicenses(licenseServer);
            });
        }
        //remove order entity from DB, remove any modules with order number and update license
        private void DeleteProductLicenseEntity(ProductLicenseTable plt)
        {
            LicenseTable storedLicense = null;
            List<string> removeProductLicenses = new List<string>();
            Service<ICreator>.Use((client) =>
            {
                client.DeleteProductLicense(plt); //Remove current PL
                removeProductLicenses.Add(plt.plID);
                if (plt.ParentProductLicenseID == null)
                {   //get addons, remove 
                    List<string> plAddons = client.GetAddOnProductLicenses(plt.plID);
                    ProductLicenseTable pltAddOn = null;
                    foreach (string addon in plAddons) //Remove current addon PLs
                    {
                        pltAddOn = client.GetProductLicense(addon);
                        if (pltAddOn != null)
                        {
                            client.DeleteProductLicense(pltAddOn);
                            removeProductLicenses.Add(pltAddOn.plID);
                        }
                    }
                }
                storedLicense = client.GetLicenseByID(plt.LicenseID, true);
                if (storedLicense == null)
                {
                    MessageBox.Show("Failed to find license server for " + plt.plID, "Delete Produce License");
                    return;
                }
                Lic_PackageAttribs licPackage = new Lic_PackageAttribs();
                licPackage.Stream = storedLicense.LicenseInfo;
                List<Lic_PackageAttribs.Lic_ProductInfoAttribs> removeProductList = new List<Lic_PackageAttribs.Lic_ProductInfoAttribs>();
                foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs product in licPackage.licLicenseInfoAttribs.TVal.productList.TVal)
                {
                    if(removeProductLicenses.Contains(product.contractNumber.TVal))
                    {
                        removeProductList.Add(product);                        
                    }
                }
                foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs prod in removeProductList)
                {
                    licPackage.licLicenseInfoAttribs.TVal.productList.TVal.Remove(prod);
                }
                storedLicense.LicenseInfo = licPackage.Stream;
                client.UpdateLicense(storedLicense, true);
            });
            TransactionManager.CreateTransaction(TransactionType.LicenseServer,
                                                 storedLicense.LicenseName,
                                                 string.Empty,
                                                 string.Format("Remove {0} Product License - {1}", s_CommLink.GetProductName(plt.ProductID), Enum.GetName(typeof(ProductLicenseState), plt.plState)),
                                                 plt.plID,
                                                 string.Empty);
        }
        private void LoadSelectedProductLicense(string selectedProductLicense)
        {
            //load licenses, highlight particular license
            CustomersListView.Visible = false;
            findToolStrip.Visible = false;
            MainTabControl.SelectedTab = LicensesTabPage;
            //get destination from string or.....store in tag?
            string[] lsString = selectedProductLicense.Split("-".ToCharArray());
            //1)custid 2)destid 3) number 4) type 5) index
            int custID = Int32.Parse(lsString[0], System.Globalization.NumberStyles.HexNumber);
            int destID = Int32.Parse(lsString[1]);
            Service<ICreator>.Use((client) =>
            {
                _selectedLicenseCustomer = new Customer(client.GetCustomer(custID.ToString(), false));
            });
            _selectedTreeNodeKey = selectedProductLicense;
            LoadDestinationNameComboBox(custID, destID);
            //expand selected license server then highlight product license
        }

        private string GetLicenseNameFromTreeView()
        {
            string licenseName = string.Empty;
            if (DetailTreeView.SelectedNode.Tag is LicenseServer)
                licenseName = (DetailTreeView.SelectedNode.Tag as LicenseServer).Name;
            else if (DetailTreeView.SelectedNode.Tag is ProductCollection)
                licenseName = (DetailTreeView.SelectedNode.Parent.Tag as LicenseServer).Name;
            else
                licenseName = (DetailTreeView.SelectedNode.Tag as ProductLicense).LicenseServer;
            return licenseName;
        }

        private void AddProductLicense()
        {
            string licenseName = DetailTreeView.SelectedNode.Name;          
            //license file already created
            //display settings form for product connections
            using (LicenseInfoForm dlg = new LicenseInfoForm("New Product License(s) for " + licenseName, ref s_CommLink))
            {
                ProductLicenseDialogData data = new ProductLicenseDialogData(licenseName);
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                {
                    TreeNode plNode = null, productNode = null, productLicenseNode = null, parentProductLicenseNode = null;
                    foreach (ProductLicenseTable plt in data.ProductLicense)
                    {
                        ProductLicenseTable plTable = plt;
                        string productName = s_CommLink.GetProductName(plTable.ProductID); 
                        if (!DetailTreeView.SelectedNode.IsExpanded) DetailTreeView.SelectedNode.Expand();
                        productNode = DetailTreeView.SelectedNode.Nodes.Find(productName, false).FirstOrDefault();
                        plNode = CreateProductLicenseNode(plTable);                      
                        if (productNode == null)
                        {
                            productNode = CreateProductNode(plNode.Tag as ProductLicense);
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
                            (productNode.Tag as ProductCollection).Collection.Add(plNode.Tag as ProductLicense);
                        }
                        if (!productNode.IsExpanded) productNode.Expand();
                        productLicenseNode = productNode.Nodes.Find(plTable.plID, true).FirstOrDefault();
                        if (productLicenseNode == null)
                        {
                            if ( plTable.ParentProductLicenseID == null)
                                productNode.Nodes.Add(plNode);                            
                            else
                            {   //add to parent order node
                                parentProductLicenseNode = DetailTreeView.SelectedNode.Nodes.Find(/*data.ProductLicense.ParentProductLicenseID*/ plTable.ParentProductLicenseID, true).FirstOrDefault();
                                parentProductLicenseNode.Nodes.Add(plNode);
                            }
                        }
                        else
                        {
                            if (plTable.ParentProductLicenseID != null && !productLicenseNode.Parent.IsExpanded)
                                productLicenseNode.Parent.Expand();
                            plNode = productLicenseNode;
                        }
                    }
                    DetailTreeView.SelectedNode = plNode;
                    SetLicenseServerState(DetailTreeView.SelectedNode, true);
                }
            }
        }
        #endregion

        #region Packet Methods
        private void GenerateLicensePacket()
        {
            m_selectedDirectory = string.Format("{0}\\{1}\\{2}.{3}.{4}", Directory.GetCurrentDirectory(), _selectedLicenseCustomer.Name, DateTime.Now.Month, DateTime.Now.Day, DateTime.Now.Year);
            //  Directory\\Company\\Date
            using (LicenseInfoForm dlg = new LicenseInfoForm("Generate License Packet for " + DetailTreeView.SelectedNode.Name, ref s_CommLink))
            {                
                PacketDialogData data = new PacketDialogData()
                {
                    CustomerName = _selectedLicenseCustomer.Name,
                    PacketName = string.Format("{0}-{1}-{2}-{3}-{4}-{5}-{6}", DetailTreeView.SelectedNode.Name, DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day, DateTime.Now.Hour, DateTime.Now.Minute, DateTime.Now.Second),
                    SelectedDirectory = m_selectedDirectory,
                    LicenseName = DetailTreeView.SelectedNode.Name
                };
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                {                    
                    GeneratePacket(data);
                }
            }
        }

        private void LoadPacketItems(string licenseName)
        {
            //given packet id display transactions or
            //given license display transactions
            IList<PacketTable> packets = null;  
            Service<ICreator>.Use((client) =>
            {   //now two steps, given a license name, get all transactions for a license and sort by packet
                packets = client.GetPacketsByLicenseName(licenseName);
            });
            //Need list view manager
            storageListView.Items.Clear();
            if (packets != null && packets.Count > 0)
            {
                foreach (var packet in packets)
                {
                    ListViewItem packetItem = new ListViewItem();
                    packetItem.Name = packet.PacketName;
                    packetItem.Text = packet.DateCreated.ToString();
                    packetItem.Tag = packet;
                    packetItem.ImageIndex = (packet.IsVerified) ? -1 : Enums.GetIconIndex("UNVERIFIED");
                    packetItem.SubItems.Add(packet.PacketName);
                    packetItem.SubItems.Add(packet.ExpiredDate.ToShortDateString());
                    packetItem.SubItems.Add(packet.UserName);
                    packetItem.SubItems.Add(packet.VerifiedBy);
                    //add to stored packet list
                    storageListView.Items.Add(packetItem);
                }
                storageListView.Items[storageListView.Items.Count - 1].Font = new Font(this.Font, FontStyle.Bold);
                storageListView.Items[storageListView.Items.Count - 1].ForeColor = Color.SteelBlue;
            }
            _lvManager.ResetListViewColumnSorter(LicensePacketListView);
            _lvManager.SetSortIndexColumn(LicensePacketListView.Handle, 0);
            packetToolStripComboBox.SelectedIndex = 0;
            ShowPacketListView(string.Empty);
            //needed outside of begin/end update to allow clearing of LicensePacketListView
            LicensePacketListView.Sort();
        }
        #endregion

        #region Module Methods

        private bool IsModuleFiltered(Module module, string filterType)
        {
            switch (filterType)
            {
                case "NonDefault":
                    if (!s_CommLink.IsDefaultModule((DetailPropertyGrid.SelectedObject as ProductLicense).ProductID, module.Name)) return true;
                    break;
                case "Licensed":
                    if (module.Value > 0) return true;
                    break;
                case "UnLicensed":
                    if (module.Value == 0) return true;
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
                if (IsModuleFiltered(lvItem.Tag as Module, type))
                    DetailListView.Items.Insert(0, lvItem.Clone() as ListViewItem);
            }
            _lvManager.AutoResizeColumns(DetailListView);      
            DetailListView.EndUpdate();
        }
        #endregion

        #region Print Methods
        private void CaptureScreen()
        {
            Graphics myGraphics = this.CreateGraphics();
            Size s = this.Size;
            memoryImage = new Bitmap(s.Width, s.Height, myGraphics);
            Graphics memoryGraphics = Graphics.FromImage(memoryImage);
            memoryGraphics.CopyFromScreen(this.Location.X, this.Location.Y, 0, 0, s);
        }

        private void printDocument1_PrintPage(System.Object sender,
               System.Drawing.Printing.PrintPageEventArgs e)
        {
            int x = e.MarginBounds.X;
            int y = e.MarginBounds.Y;
            int width = memoryImage.Width;
            int height = memoryImage.Height;

            if ((width / e.MarginBounds.Width) > (height / e.MarginBounds.Height))
            {
                width = e.MarginBounds.Width;
                height = memoryImage.Height * e.MarginBounds.Width / memoryImage.Width;
            }
            else
            {
                height = e.MarginBounds.Height;
                width = memoryImage.Width * e.MarginBounds.Height / memoryImage.Height;
            }
            System.Drawing.Rectangle destRect = new System.Drawing.Rectangle(x, y, width, height);
            e.Graphics.DrawImage(memoryImage, destRect, 0, 0, memoryImage.Width, memoryImage.Height, System.Drawing.GraphicsUnit.Pixel);
        }
        #endregion

        #region Customer Methods
        private bool DeleteCustomerItem(ListViewItem lvItem)
        {
            string custName = lvItem.SubItems[1].Text;
            if (MessageBox.Show(string.Format("Please confirm deletion of customer : {0}.", custName), 
                                "Delete Customer",
                                MessageBoxButtons.OKCancel, 
                                MessageBoxIcon.Exclamation) == DialogResult.OK)
            {
                DeleteCustomerEntity(Int32.Parse(lvItem.Text, System.Globalization.NumberStyles.HexNumber), custName);
                return true;
            }
            return false;
        }
        private void DeleteCustomerEntity(int custID, string custName)
        {
            Service<ICreator>.Use((client) =>
            {
                IList<LicenseTable> ltList = client.GetLicensesByCustomer(custName, false);
                client.DeleteAllDestinationNames(custID);
                foreach (LicenseTable lt in ltList)
                {
                    DeleteAllProductLicenseEntities(lt.LicenseName);
                    DeleteLicenseEntity(lt.LicenseName);
                }
                CustomerTable custRec = client.GetCustomer(custID.ToString(), false);
                client.DeleteCustomer(custRec);
            });
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
                foreach (var license in customer.LicenseTables.OrderBy(d => d.DestinationID).ThenBy(g => g.GroupID))
                {
                    if (license.LicenseName.ToLower().Contains(m_searchString) || (customer.SCRname.ToLower().Contains(m_searchString)))
                    {
                        if (!node.Nodes.ContainsKey(license.LicenseName))
                        {
                            licenseNode = new TreeNode(license.LicenseName);
                            licenseNode.Name = licenseNode.Text;
                            licenseNode.Tag = new LicenseServer(license, m_Permissions);
                            licenseNode.Nodes.Add(new VirtualTreeNode());
                            node.Nodes.Add(licenseNode);
                            if (m_searchString.Length > 0)
                                licenseNode.Parent.Expand();
                        }
                        else
                        {
                            licenseNode = node.Nodes[node.Nodes.IndexOfKey(license.LicenseName)];
                            licenseNode.Tag = new LicenseServer(license, m_Permissions);
                        }
                    }
                }
            }
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

        private void CreateNewCustomer(string custName)
        {
            CustomerTable newCustRec = new CustomerTable();            
            newCustRec.SCRname = custName;
            Service<ICreator>.Use((client) =>
            {
                client.CreateCustomer(newCustRec);
                newCustRec = client.GetCustomer(custName, false);
                if (newCustRec == null)
                {
                    MessageBox.Show(string.Format("Failed to create new customer : {0}", custName), "Create New Customer");
                    return;
                }
                DestinationNameTable destRec = new DestinationNameTable()
                {
                    CustID = newCustRec.SCRnumber,
                    DestID = 0, //Initial Destination ID is 0
                    DestName = newCustRec.SCRname
                };
                client.CreateDestinationName(destRec);
            });
            ListViewItem lvItem = new ListViewItem(string.Format("{0:x}", newCustRec.SCRnumber));
            lvItem.Name = newCustRec.SCRname;
            lvItem.SubItems.Add(newCustRec.SCRname);            
            CustomersListView.Items.Add(lvItem);
            lvItem.Selected = true;
            CustomersListView.EnsureVisible(CustomersListView.Items.IndexOf(lvItem));
        }
        #endregion

        #region  Hardware Key Methods
        private void PopulateHardwareKeyView(string searchString, string custName)
        {
            IList<TokenTable> tokens = null;
            string licenseName;
            LicenseTable license = null;
            ListViewItem[] lvItems = null;
            EnableHardwareKeyView();
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
                        licenseName = (license != null) ? license.LicenseName : string.Empty;
                        item.SubItems.Add(licenseName);
                        item.SubItems.Add(((TokenStatus)tokens[index].TokenStatus).ToString());
                        string activatedDate = (tokens[index].ActivatedDate.HasValue) ? tokens[index].ActivatedDate.Value.ToString() : string.Empty;
                        string deactivatedDate = (tokens[index].DeactivatedDate.HasValue) ? tokens[index].DeactivatedDate.Value.ToString() : string.Empty;
                        item.SubItems.Add(activatedDate);
                        item.SubItems.Add(deactivatedDate);
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
            EnableHardwareCustomerView();
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
                    ListViewItem item = new ListViewItem(new string[] { string.Format("{0:X}", customers[index].SCRnumber), customers[index].SCRname, reservedKeys.ToString(), activeKeys.ToString(), deactivatedKeys.ToString() });
                    lvItems[index] = item;
                }
                HardwareKeyListView.Items.AddRange(lvItems);
            }
            if (searchToolStripTextBox.ForeColor == SystemColors.InactiveCaptionText)
                searchToolStripTextBox.Text = "Customer Filter";
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
        public void PopulateHardwareKeyListViewColumns(string custName)
        {
            HardwareKeyListView.Columns.Clear();
            if (custName.Length > 0)
            {
                if (HardwareKeyListView.Columns.Count == 0)
                {
                    HardwareKeyListView.Columns.Add("Key Value");
                    HardwareKeyListView.Columns.Add("License Server");
                    HardwareKeyListView.Columns.Add("Status");
                    HardwareKeyListView.Columns.Add("Activated Date");
                    HardwareKeyListView.Columns.Add("Deactivated Date");
                }
            }
            else
            {
                if (HardwareKeyListView.Columns.Count == 0)
                {
                    HardwareKeyListView.Columns.Add("Number");
                    HardwareKeyListView.Columns.Add("Customer");
                    HardwareKeyListView.Columns.Add("Reserved");
                    HardwareKeyListView.Columns.Add("Activated");
                    HardwareKeyListView.Columns.Add("Deactivated");
                }
            }
            _lvManager.ResetListViewColumnSorter(HardwareKeyListView);
        }
        public void PopulateHardwareKeyListView(string searchString, string custName)
        {
            if (custName.Length > 0)
                PopulateHardwareKeyView(searchString, custName);
            else
                PopulateHardwareKeyCustomerView(searchString);
        }
        private void EnableHardwareKeyView()
        {
            navigateHomeToolStripButton.Enabled = true;
            seekKeyToolStripButton.Enabled = LicenseServerAvailable;
            reserveHardwareKeyToolStripButton.Enabled = m_Permissions.pt_create_modify_key.Value;
            searchToolStripTextBox.Visible = false;
            SearchToolStripLabel.Visible = false;
            searchToolStripTextBox.Enabled = false;
            SearchToolStripLabel.Enabled = false;
            deactivateHardwareKeyToolStripButton.Enabled = false;
            activateHardwareKeyToolStripButton.Enabled = false;
            if (HardwareKeyListView.SelectedItems.Count > 0)
            {
                deactivateHardwareKeyToolStripButton.Enabled = m_Permissions.pt_create_modify_key.Value && LicenseServerAvailable;
                if (HardwareKeyListView.SelectedItems[0].SubItems[2].Text != "Active" && 
                    HardwareKeyListView.SelectedItems[0].SubItems[2].Text != "Lost")
                    activateHardwareKeyToolStripButton.Enabled = m_Permissions.pt_create_modify_key.Value && LicenseServerAvailable;
            }
            clearKeyToolStripButton.Enabled = true;
            ValidationKeyToolStrip.Visible = true;
            ValidationKeyCustomerLabel.Text = _selectedHardwareKeyCustomer;
        }

        private void EnableHardwareCustomerView()
        {
            searchToolStripTextBox.Visible = true;
            SearchToolStripLabel.Visible = true;
            searchToolStripTextBox.Enabled = true;
            SearchToolStripLabel.Enabled = true;
            navigateHomeToolStripButton.Enabled = false;
            seekKeyToolStripButton.Enabled = LicenseServerAvailable;
            reserveHardwareKeyToolStripButton.Enabled = false;
            deactivateHardwareKeyToolStripButton.Enabled = false;
            activateHardwareKeyToolStripButton.Enabled = false;
            ValidationKeyToolStrip.Visible = false;
        }
        #endregion

        #region DetailListView Methods
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
                switch (selectedObject.GetType().Name)
                {
                    case AppConstants.LicenseServerObjectName:
                        if (!DetailListView.Columns.ContainsKey("Status"))
                        {
                            DetailListView.Columns.Clear();
                            DetailListView.Columns.Add("Type", "Type");
                            DetailListView.Columns.Add("Value", "Value");
                            DetailListView.Columns.Add("Status", "Status");
                        }
                        break;
                    case AppConstants.ProductCollectionObjectName:
                        if (!DetailListView.Columns.ContainsKey("Product Connections"))
                        {
                            DetailListView.Columns.Clear();
                            DetailListView.Columns.Add("Name", "Name");
                            DetailListView.Columns.Add("Units", "Units");
                            DetailListView.Columns.Add("Product Connections", "Product Connections");
                        }                        
                        break;                    
                    default: //ProductLicense
                        if (!DetailListView.Columns.ContainsKey("Available"))
                        {
                            DetailListView.Columns.Clear();
                            DetailListView.Columns.Add("Name", "Name");
                            DetailListView.Columns.Add("Units", "Units");
                            DetailListView.Columns.Add("Available", "Available");
                        }
                        break; 
                };
                _lvManager.ResetListViewColumnSorter(DetailListView);
            }
            /// <summary>
            /// Populates the ListView column headers based upon the selected TreeNode of the License view.
            /// </summary>
            public void PopulateDetailListView(Object selectedObject)
            {
                switch (selectedObject.GetType().Name)
                {
                    case AppConstants.LicenseServerObjectName:
                        PopulateDetailListView(selectedObject as LicenseServer);
                        break;
                    case AppConstants.ProductCollectionObjectName:
                        PopulateDetailListView(selectedObject as ProductCollection);
                        break;
                    default: //product license
                        PopulateDetailListView(selectedObject as ProductLicense);
                        break;
                }
            }
            
            //CONTACT DB : 2
            public void PopulateDetailListView(LicenseServer licenseData)
            {
                IList<TokenTable> tokenList = null;
                IList<SoftwareTokenTable> swTokenList = null;
                DetailListView.Items.Clear();
                int imageIndex = Enums.GetIconIndex("VALIDATIONTOKENS");
                if (imageIndex < 0) imageIndex = 0;
                Image tokenImage = CreatorImageList.Images[imageIndex];
                ImageToolStripLabel.Size = new Size(tokenImage.Width, tokenImage.Height);
                ImageToolStripLabel.Image = tokenImage;
                DetailListViewToolStripLabel.Text = "Validation Tokens";
                DetailListViewToolStripLabel2.Visible = false;
                //enable/disable buttons
                dlvAddToolStripButton.Visible = true;
                dlvRemoveToolStripButton.Visible = true;
                moduleFilterToolStripComboBox.Visible = false;
                totalModuleListToolStripButton.Visible = false;
                defaultModuleListToolStripButton.Visible = false;
                ListViewItem lvItem;        
                TokenTable hardwareToken = null;
               // retrieve tokens from database not license info object
                Service<ICreator>.Use((client) => 
                {                    
                    tokenList = client.GetTokensByLicenseName(licenseData.Name); //db - 1
                    swTokenList = client.GetAllSoftwareTokens(); // db - 2
                });
                if (tokenList != null)
                {             
                    foreach(TokenTable token in tokenList)
                    {
                        if (tokenList.Count > 1)
                        {
                            string tokenName = Enum.GetName(typeof(Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType), token.TokenType);
                            SoftwareTokenTable st = swTokenList.FirstOrDefault(t => t.TokenType == tokenName);
                            //skip unused sw tokens or licensecode
                            if (st == null || st.Status == 0) //st will be null for ttLicenseCode
                                continue;
                        }
                        lvItem = new ListViewItem();
                        Validation vToken = new Validation(token);
                        lvItem.Tag = vToken;
                        if (vToken.TokenType.Equals(Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID))
                        {
                            hardwareToken = tokenList.First(t => t.TokenType == (byte)vToken.TokenType);
                            if (hardwareToken == null)
                                MessageBox.Show(string.Format("Failed to get token status for {0}", vToken.TokenValue), "Load Validation Token List");
                            else
                                (lvItem.Tag as Validation).Status = ((TokenStatus)hardwareToken.TokenStatus).ToString();
                        }
                        lvItem.Name = lvItem.Text = vToken.TokenType.ToString();
                        lvItem.ImageIndex = Enums.GetIconIndex(lvItem.Text);
                        lvItem.SubItems.Add(vToken.TokenValue);
                        lvItem.SubItems.Add(vToken.Status);
                        if (vToken.Status == TokenStatus.Deactivated.ToString())
                        {
                            lvItem.ForeColor = System.Drawing.Color.SlateGray;
                            lvItem.Font = new System.Drawing.Font(lvItem.Font, System.Drawing.FontStyle.Italic);
                        }
                        DetailListView.Items.Add(lvItem);
                    }
                    bool bLockStatus = licenseData.LockStatus; //db - 2
                    dlvAddToolStripButton.Enabled = (tokenList.Where(t => t.TokenStatus == (byte)TokenStatus.Reserved || t.TokenStatus == (byte)TokenStatus.Active).Count() == 0 || tokenList.Count == 0) && licenseData.IsActive && licenseData.LockedByCurrentUser();
                    dlvRemoveToolStripButton.Enabled = tokenList.Where(t => t.TokenStatus != (byte)TokenStatus.Deactivated).Count() > 0 && hardwareToken == null && licenseData.IsActive && licenseData.LockedByCurrentUser();              
                }               
                _lvManager.SetSortIndexColumn(DetailListView.Handle, DetailListView.Columns.Count - 1);
                DetailListView.Sort();
                _lvManager.AutoResizeColumns(DetailListView);
            }
            //CONTACT DB : 2
            public void PopulateDetailListView(ProductCollection productData)
            {
                ProductLicenseTable plt = null;
                List<ModuleTable> moduleList = null;
                DetailListView.Items.Clear();
                int imageIndex = Enums.GetIconIndex(s_CommLink.GetProductBaseName((byte)productData.ProductID));
                if (imageIndex < 0) imageIndex = 0;
                Image productImage = CreatorImageList.Images[imageIndex];
                ImageToolStripLabel.Size = new Size(productImage.Width, productImage.Height);
                ImageToolStripLabel.Image = productImage;
                DetailListViewToolStripLabel.Text = string.Format("{0} Module Summary", s_CommLink.GetProductName(productData.ProductID));
                DetailListViewToolStripLabel2.Visible = true;
                dlvAddToolStripButton.Visible = false;
                dlvRemoveToolStripButton.Visible = false;
                moduleFilterToolStripComboBox.Visible = false;
                totalModuleListToolStripButton.Visible = false;
                defaultModuleListToolStripButton.Visible = false;
                //needs to skip any product,modules that are deactivated            
                LicenseServer lsp = DetailTreeView.SelectedNode.Parent.Tag as LicenseServer;
                Service<ICreator>.Use((client) =>
                {
                    plt = client.GetProductLicense(productData.Collection[0].ID);
                    moduleList = client.GetAllActiveModulesByProduct(lsp.Name, productData.ProductID);
                });
                if (plt != null)
                {
                    LicenseVersion version = new LicenseVersion(plt.ProductVersion);
                    DetailListViewToolStripLabel2.Text = string.Format("Product Version : {0}", version.ToString());
                    Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec = CreatorForm.s_CommLink.GetProductSpec(productData.ProductID);
                    List<ListViewItem> lvItems = new List<ListViewItem>();
                    foreach (Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs moduleSpec in productSpec.moduleSpecMap.TVal.Values)
                    {
                        bool bDeprecated = false, bIntroduced = false;
                        if (moduleSpec.moduleVersionDeprecated_Major.TVal > 0)
                        {
                            if (version.Major > moduleSpec.moduleVersionDeprecated_Major)
                                bDeprecated = true;
                            else if (version.Major == moduleSpec.moduleVersionDeprecated_Major)
                            {
                                if (version.Minor >= moduleSpec.moduleVersionDeprecated_Minor)
                                    bDeprecated = true;
                            }
                        }
                        if (version.Major > moduleSpec.moduleVersionIntroduced_Major)
                            bIntroduced = true;
                        else if (version.Major == moduleSpec.moduleVersionIntroduced_Major)
                        {
                            if (version.Minor >= moduleSpec.moduleVersionIntroduced_Minor)
                                bIntroduced = true;
                        }
                        if (bIntroduced && !bDeprecated)
                        {
                            int modValue = 0, modAppInstance = 0;
                            foreach (ModuleTable mt in moduleList.Where(c => c.ModID == moduleSpec.moduleID.TVal))
                            {   //sum up module value, module app instance
                                modValue += mt.Value;
                                modAppInstance += mt.AppInstance;
                            }
                            ListViewItem lvItem = new ListViewItem();
                            lvItem.Text = moduleSpec.moduleName.TVal;
                            lvItem.Name = moduleSpec.moduleID.SVal;
                            lvItem.ImageIndex = Enums.GetIconIndex("MODULE");
                            if (s_CommLink.IsDefaultModule(productSpec.productID.TVal, (short)moduleSpec.moduleID.TVal))
                                lvItem.SubItems.Add("*");
                            else
                                lvItem.SubItems.Add(modValue.ToString());
                            lvItem.SubItems.Add(modAppInstance.ToString());
                            lvItems.Add(lvItem);
                        }
                    }
                    DetailListView.Items.AddRange(lvItems.ToArray());
                }
            }
            //CONTACT DB : 1
            private void PopulateDetailListView(ProductLicense plData)
            {
                //clear previous items from storage and current view
                DetailListView.Items.Clear();
                storageListView.Items.Clear();
                //test-dev image missing
                int imageIndex = Enums.GetIconIndex(s_CommLink.GetProductBaseName(plData.ProductName));
                if (imageIndex < 0) imageIndex = 0;
                Image productImage = CreatorImageList.Images[imageIndex];
                ImageToolStripLabel.Size = new Size(productImage.Width, productImage.Height);
                ImageToolStripLabel.Image = productImage;
                DetailListViewToolStripLabel.Text = string.Format("{0} Modules", plData.ProductName);
                DetailListViewToolStripLabel2.Visible = false;
                dlvAddToolStripButton.Visible = false;
                dlvAddToolStripButton.Enabled = true;
                dlvRemoveToolStripButton.Visible = false;
                moduleFilterToolStripComboBox.Visible = true;
                totalModuleListToolStripButton.Visible = true;
                defaultModuleListToolStripButton.Visible = true;
                totalModuleListToolStripButton.Enabled = m_Permissions.pt_create_modify_key.Value && DetailPropertyGrid.Enabled;
                defaultModuleListToolStripButton.Enabled = m_Permissions.pt_create_modify_key.Value && DetailPropertyGrid.Enabled;
                //need to set plData read-only attributes
                //gets all modules for a given product license and add-on product licenses
                string productLicense = (plData.ParentID == null) ? plData.ID : plData.ParentID;
                List<ModuleTable> moduleList = plData.ModuleList;
                foreach (ModuleTable module in moduleList)
                {   //totalValue = licensed module value + add-on module value;
                    //           = trial module value;
                    uint totalValue = 0;
                    if (plData.Status == ProductLicenseState.Trial)
                        totalValue = (uint)module.Value;
                    else
                    {
                        foreach (ModuleTable mod in moduleList)
                        {
                            if (mod.ModID == module.ModID)
                                totalValue += (uint)mod.Value;
                        }
                    }
                    ListViewItem lvItem = CreateModuleListViewItem(plData, module, totalValue);
                    if (storageListView.Items.Find(lvItem.Name, false).Count() == 0)
                        storageListView.Items.Add(lvItem);
                }
                if (moduleFilterToolStripComboBox.SelectedItem != null)               
                    ShowModuleListView(moduleFilterToolStripComboBox.Text);
                else
                {
                    moduleFilterToolStripComboBox.SelectedIndex = 0;
                    ShowModuleListView("All");
                }
            }

            public ListViewItem CreateModuleListViewItem(ProductLicense productLicense, ModuleTable module, uint totalValue)
            {
                Module newModule = new Module(module);
                newModule.ProductID = productLicense.ProductID;
                ListViewItem lvItem = new ListViewItem();
                lvItem.Tag = newModule;
                lvItem.Name = lvItem.Text = s_CommLink.GetModuleName(productLicense.ProductID, module.ModID);
                lvItem.ImageIndex = Enums.GetIconIndex("MODULE");
                if (s_CommLink.IsDefaultModule(productLicense.ProductID, module.ModID))
                {
                    lvItem.SubItems.Add("*");
                    lvItem.SubItems.Add("*");
                }
                else
                {
                    lvItem.SubItems.Add(module.Value.ToString());
                    //perm, addon - total value is shared between these
                    //available units = maximum value - total value(perm/trial + addon)
                    //total value should never exceed max value
                    //bool bVal = false;
                    //if (module.ModID == 101)
                    //    bVal = true;
                    if (newModule.UnlimitedValue == 0)
                        lvItem.SubItems.Add("Unlimited");
                    else
                    {
                        //int maxValue = s_CommLink.GetMaxModuleValue(productLicense.ProductID, module.ModID);
                        uint units = productLicense.GetAvailableModuleUnits(newModule, totalValue);
                        lvItem.SubItems.Add(units.ToString());
                    }
                }
                SetModuleState(productLicense,lvItem);
                return lvItem;
            }
    #endregion

        #region Transactions
        //retrieve transactions by license server/product license
        private void LoadTransactionItems(string licenseName)
        {
            IList<PacketTable> packets = null;
            IList<TransactionTable> transactions = null, selectedTransactions = null;
            IList<ProductLicenseTable> productLicenses = null;
            ProductLicenseTable pl = null;
            string productLicenseName = string.Empty;
            storageListView.Items.Clear();
            ListViewItem[] lvItems;
            int index = 0;
            Service<ICreator>.Use((client) =>
            {   //now two steps, given a license name, get all transactions for a license and sort by packet
                transactions = client.GetNewTransactionsByLicenseName(licenseName).OrderByDescending(c => c.taDateCreated).ToList();
                //filtered out add-ons, want to include add on information....
                productLicenses = client.GetProductLicenses(licenseName, true);                
                if (transactions.Count > 0)
                {
                    lvItems = new ListViewItem[transactions.Count];
                    for (index = 0; index < transactions.Count; index++)
                    {
                        pl = productLicenses.Where(c => c.ID == transactions[index].taOrderID).FirstOrDefault();
                        productLicenseName = (pl != null) ? pl.plID : string.Empty;
                        ListViewItem item = new ListViewItem();                        
                        item.Tag = transactions[index];
                        if(transactions[index].taType != (byte)TransactionType.PacketVerification)
                            item.ForeColor = System.Drawing.Color.SteelBlue;
                        item.Text = transactions[index].taDateCreated.ToString();
                        item.SubItems.Add(Enum.GetName(typeof(TransactionType), transactions[index].taType));
                        item.SubItems.Add(transactions[index].taDescription);
                        item.SubItems.Add(transactions[index].taUnits);
                        item.SubItems.Add(transactions[index].taPreviousValue);
                        item.SubItems.Add(productLicenseName);
                        item.SubItems.Add(transactions[index].taUser);
                        item.SubItems.Add(string.Empty);
                        lvItems[index] = item;
                    }
                    storageListView.Items.AddRange(lvItems);
                }
                packets = client.GetPacketsByLicenseName(licenseName).OrderByDescending(c => c.DateCreated).ToList();
                transactions = client.GetTransactionsByLicenseName(licenseName);
                foreach (var packet in packets)
                {
                    selectedTransactions = transactions.Where(t => t.taPacketID == packet.ID).OrderByDescending(c => c.taDateCreated).ToList();
                    if (selectedTransactions.Count > 0)
                    {
                        lvItems = new ListViewItem[selectedTransactions.Count];
                        for (index = 0; index < selectedTransactions.Count; index++)
                        {
                            pl = productLicenses.Where(c => c.ID == selectedTransactions[index].taOrderID).FirstOrDefault();
                            productLicenseName = (pl != null) ? pl.plID : string.Empty;
                            ListViewItem item = new ListViewItem();
                            item.Tag = selectedTransactions[index];
                            item.Text = selectedTransactions[index].taDateCreated.ToString();
                            item.SubItems.Add(Enum.GetName(typeof(TransactionType), selectedTransactions[index].taType));
                            item.SubItems.Add(selectedTransactions[index].taDescription);
                            item.SubItems.Add(selectedTransactions[index].taUnits);
                            item.SubItems.Add(selectedTransactions[index].taPreviousValue);
                            item.SubItems.Add(productLicenseName);
                            item.SubItems.Add(selectedTransactions[index].taUser);
                            item.SubItems.Add(packet.PacketName);
                            lvItems[index] = item;
                        }
                        storageListView.Items.AddRange(lvItems);
                    }
                }
            });
            _lvManager.ResetListViewColumnSorter(TransactionListView);
            _lvManager.SetSortIndexColumn(TransactionListView.Handle, 0);
            transactionToolStripComboBox.SelectedIndex = (DetailTreeView.SelectedNode.Level < 3) ? DetailTreeView.SelectedNode.Level : 2;
            TransactionListView.Sort();
        }
        #endregion

        #region Reporting

        private void LoadProductLicenseReport(Report rp)
        {
            IList<ProductLicenseTable> licenses = null;
            IList<CustomerTable> customers = null;
            IList<PacketTable> packets = null;
            IList<TokenTable> tokens = null;

            CustomerTable customer = null;

            string[] plSplit;
            string verifiedStatus, validationType;
            Service<ICreator>.Use((client) =>
            {                
                licenses = client.GetProductLicensesByConditions(rp.DatabaseConditions, rp.MatchAll);
                packets = client.GetPacketsByLicenseName(string.Empty);
                customers = client.GetAllCustomers(string.Empty, false);
                tokens = client.GetAllTokens(string.Empty, Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttNone);
            });
            PopulateReportListViewColumns(Report.ReportType.ProductLicense);
            ReportListView.BeginUpdate();
            ReportListView.Items.Clear();
            if (licenses != null)
            {
                ListViewItem[] lvItems = new ListViewItem[licenses.Count];
                for (int index = 0; index < licenses.Count; index++)
                {
                    ListViewItem newItem = new ListViewItem();
                    plSplit = licenses[index].plID.Split("-".ToCharArray());
                    customer = customers.Where(c => c.SCRnumber.Equals(Int32.Parse(plSplit[0], System.Globalization.NumberStyles.HexNumber))).FirstOrDefault();
                    if (customer != null)
                        newItem.Text = customer.SCRname;
                    else
                        newItem.Text = "Unknown";
                    //1. id 2) product 3) version 4) state 5) expdate - extension 6)activation 7) activation amount 8) active 9) description
                    newItem.SubItems.Add(licenses[index].plID);
                    newItem.SubItems.Add(s_CommLink.GetProductName(licenses[index].ProductID));
                    newItem.SubItems.Add(licenses[index].ProductVersion);
                    newItem.SubItems.Add(Enums.GetEnumDescription((ProductLicenseState)licenses[index].plState));
                    if (licenses[index].ExpirationDate.HasValue)
                        newItem.SubItems.Add(licenses[index].ExpirationDate.Value.ToShortDateString());
                    else
                        newItem.SubItems.Add(string.Empty);
                    newItem.SubItems.Add(licenses[index].Extensions.ToString());
                    newItem.SubItems.Add(licenses[index].Activations.ToString());
                    newItem.SubItems.Add(licenses[index].ActivationAmount.ToString());
                    newItem.SubItems.Add(licenses[index].IsActive.ToString());
                    
                    //need link to licenses to product licenses
                    //have product license list with access to modules
                    
                    validationType = (tokens.Where(t => t.LicenseID == licenses[index].LicenseID && t.TokenType == (byte)Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID).Count() > 0) ? "Hardware" : "Software";
                    if (validationType == "Hardware")
                        validationType = tokens.Where(t => t.LicenseID == licenses[index].LicenseID && t.TokenType == (byte)Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID).First().TokenValue;
                    newItem.SubItems.Add(validationType);
                    
                    verifiedStatus = (packets.Where(p => p.LicenseID == licenses[index].LicenseID && p.IsVerified == false).Count() == 0 && (packets.Where(p => p.LicenseID == licenses[index].LicenseID).Count() > 0)) ? bool.TrueString : bool.FalseString;
                    newItem.SubItems.Add(verifiedStatus);
                    newItem.SubItems.Add(licenses[index].Description);
                    lvItems[index] = newItem;                    
                }
                ReportListView.Items.AddRange(lvItems);
            }
            _lvManager.AutoResizeColumns(ReportListView);
            ReportListView.EndUpdate();
        }

        private void LoadLicenseServerReport(Report rp)
        {
            IList<LicenseTable> licenses = null;
            IList<CustomerTable> customers = null;
            IList<TokenTable> tokens = null;
            IList<PacketTable> packets = null;
            CustomerTable customer = null;
            string[] plSplit;
            string validationType, verifiedStatus;
            Service<ICreator>.Use((client) =>
            {
                licenses = client.GetLicensesByConditions(rp.DatabaseConditions, rp.MatchAll, false);              
                customers = client.GetAllCustomers(string.Empty, false);
                packets = client.GetPacketsByLicenseName(string.Empty);
                tokens = client.GetAllTokens(string.Empty, Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttNone);
            });
            PopulateReportListViewColumns(Report.ReportType.LicenseServer);
            ReportListView.BeginUpdate();
            ReportListView.Items.Clear();
            if (licenses != null)
            {
                ListViewItem[] lvItems = new ListViewItem[licenses.Count];
                for (int index = 0; index < licenses.Count; index++)
                {
                    ListViewItem newItem = new ListViewItem();
                    plSplit = licenses[index].LicenseName.Split("-".ToCharArray());
                    customer = customers.Where(c => c.SCRnumber.Equals(licenses[index].SCRnumber)).FirstOrDefault();
                    if (customer != null)
                        newItem.Text = customer.SCRname;
                    else
                        newItem.Text = "Unknown";
                    newItem.SubItems.Add(licenses[index].LicenseName);
                    newItem.SubItems.Add(licenses[index].IsActive.ToString());
                    validationType = (tokens.Where(t => t.LicenseID == licenses[index].ID && t.TokenType == (byte)Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID).Count() > 0) ? "Hardware": "Software";
                    if (validationType == "Hardware")
                        validationType = tokens.Where(t => t.LicenseID == licenses[index].ID && t.TokenType == (byte)Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID).First().TokenValue;
                    newItem.SubItems.Add(validationType);
                    verifiedStatus = (packets.Where(t => t.LicenseID == licenses[index].ID && t.IsVerified == false).Count() == 0 && packets.Where(t => t.LicenseID == licenses[index].ID).Count() > 0) ? bool.TrueString : bool.FalseString;
                    newItem.SubItems.Add(verifiedStatus);
                    newItem.SubItems.Add(licenses[index].LicenseComments);
                    lvItems[index] = newItem;
                }
                ReportListView.Items.AddRange(lvItems);
            }
            _lvManager.AutoResizeColumns(ReportListView);
            ReportListView.EndUpdate();
        }

        private void LoadHardwareTokenReport(Report rp)
        {
            IList<TokenTable> tokens = null;
            IList<CustomerTable> customers = null;
            LicenseTable licRecord = null;            
            CustomerTable customer = null;
            PopulateReportListViewColumns(Report.ReportType.HardwareToken);
            ReportListView.BeginUpdate();
            ReportListView.Items.Clear();            
            Service<ICreator>.Use((client) =>
            {
                tokens = client.GetHardwareTokensByConditions(rp.DatabaseConditions, rp.MatchAll);
                customers = client.GetAllCustomers(string.Empty, false);
                if (tokens != null)
                {
                    ListViewItem[] lvItems = new ListViewItem[tokens.Count];
                    for (int index = 0; index < tokens.Count; index++)
                    {
                        ListViewItem newItem = new ListViewItem();
                        customer = customers.Where(c => c.SCRnumber.Equals(tokens[index].CustID)).FirstOrDefault();
                        if (customer != null)
                            newItem.Text = customer.SCRname;
                        else
                            newItem.Text = "Unknown";
                        //token id
                        newItem.SubItems.Add(tokens[index].TokenValue);
                        //license server
                        licRecord = client.GetLicenseByID(tokens[index].LicenseID, false);
                        string licID = (licRecord != null) ? licRecord.LicenseName : string.Empty;
                        newItem.SubItems.Add(licID);
                        //status
                        newItem.SubItems.Add(((TokenStatus)tokens[index].TokenStatus).ToString());
                        //activated date
                        newItem.SubItems.Add(tokens[index].ActivatedDate.ToString());
                        //deactivated date
                        newItem.SubItems.Add(tokens[index].DeactivatedDate.ToString());
                        lvItems[index] = newItem;
                    }
                    ReportListView.Items.AddRange(lvItems);
                }
            });
            _lvManager.AutoResizeColumns(ReportListView);
            ReportListView.EndUpdate();
        }

        private void LoadLicensePacketReport(Report rp)
        {
            IList<PacketTable> packets = null;
            IList<CustomerTable> customers = null;
            IList<LicenseTable> licenses = null;
            LicenseTable license = null;
            CustomerTable customer = null;
            PopulateReportListViewColumns(Report.ReportType.LicensePacket);
            ReportListView.BeginUpdate();
            ReportListView.Items.Clear();
            Service<ICreator>.Use((client) =>
            {
                packets = client.GetPacketsByConditions(rp.DatabaseConditions, rp.MatchAll);
                licenses = client.GetLicensesByCustomer(string.Empty, false);
                customers = client.GetAllCustomers(string.Empty, false);
                if (packets != null)
                {
                    ListViewItem[] lvItems = new ListViewItem[packets.Count];
                    for (int index = 0; index < packets.Count; index++)
                    {
                        ListViewItem newItem = new ListViewItem();
                        license = licenses.Where(l => l.ID == packets[index].LicenseID).FirstOrDefault();
                        if(license != null)
                            customer = customers.Where(c => c.SCRnumber == license.SCRnumber).FirstOrDefault();
                        if (customer != null)
                            newItem.Text = customer.SCRname;
                        else
                            newItem.Text = "Unknown";                        
                        newItem.SubItems.Add(packets[index].PacketName);
                        //dateCreated
                        newItem.SubItems.Add(packets[index].DateCreated.ToShortDateString());
                        //expiredDate
                        newItem.SubItems.Add(packets[index].ExpiredDate.ToShortDateString());
                        //license server
                        newItem.SubItems.Add(packets[index].IsVerified.ToString());
                        newItem.SubItems.Add(packets[index].VerifiedBy);
                        newItem.SubItems.Add(packets[index].PacketComments);
                        lvItems[index] = newItem;
                    }
                    ReportListView.Items.AddRange(lvItems);
                }
            });
            _lvManager.AutoResizeColumns(ReportListView);
            ReportListView.EndUpdate();
        }

        private void LoadReportNode(TreeNode node)
        {
            ReportListView.Items.Clear();
            ReportListView.Columns.Clear();
            Report rp = node.Tag as Report;
            if (rp != null)
            {
                if (rp.Type == Report.ReportType.LicenseServer)
                    LoadLicenseServerReport(rp);
                else if (rp.Type == Report.ReportType.ProductLicense)
                    LoadProductLicenseReport(rp);
                else if(rp.Type == Report.ReportType.HardwareToken)
                    LoadHardwareTokenReport(rp);
                else
                    LoadLicensePacketReport(rp);
            }
        }
        /// <summary>
        /// Populates the ListView column headers based upon the selected TreeNode of the License view.
        /// </summary>
        public void PopulateReportListViewColumns(Report.ReportType _reportType)
        {
            ReportListView.Columns.Clear();
            switch (_reportType)
            {   //values loaded in column order
                case Report.ReportType.LicenseServer:
                    ReportListView.Columns.Add("Customer");
                    ReportListView.Columns.Add("License Server");
                    ReportListView.Columns.Add("Active");
                    ReportListView.Columns.Add("Validation");
                    ReportListView.Columns.Add("Verified");
                    ReportListView.Columns.Add("Notes");
                    break;
                case Report.ReportType.ProductLicense:
                    //1. id 2) product 3) version 4) state 5) extension 6)activation 7) activation amount 8) active 9) description
                    ReportListView.Columns.Add("Customer");
                    ReportListView.Columns.Add("ID");
                    ReportListView.Columns.Add("Product");
                    ReportListView.Columns.Add("Version");
                    ReportListView.Columns.Add("State");
                    ReportListView.Columns.Add("Expiration Date");
                    ReportListView.Columns.Add("Extension");
                    ReportListView.Columns.Add("Activations");
                    ReportListView.Columns.Add("Activation Amount");
                    ReportListView.Columns.Add("Active");
                    ReportListView.Columns.Add("Validation");
                    ReportListView.Columns.Add("Verified");
                    ReportListView.Columns.Add("Notes");
                    break;
                case Report.ReportType.HardwareToken:
                    ReportListView.Columns.Add("Customer");
                    ReportListView.Columns.Add("Hardware ID");
                    ReportListView.Columns.Add("License Server");
                    ReportListView.Columns.Add("Status");
                    ReportListView.Columns.Add("Activated Date");
                    ReportListView.Columns.Add("Deactivated Date");
                    break;
                case Report.ReportType.LicensePacket:
                    ReportListView.Columns.Add("Customer");
                    ReportListView.Columns.Add("License Packet");
                    ReportListView.Columns.Add("Date Created");
                    ReportListView.Columns.Add("Expiration Date");
                    ReportListView.Columns.Add("Verified");
                    ReportListView.Columns.Add("Verified By");
                    ReportListView.Columns.Add("Notes");
                    break;
                default:
                    break;
            }
            _lvManager.ResetListViewColumnSorter(ReportListView);
        }

        private int GetReportImageIndex(Report.ReportType reportType)
        {
            int imageIndex = -1;
            switch (reportType)
            {
                case Report.ReportType.HardwareToken:
                    imageIndex = Enums.GetIconIndex("TTHARDWAREKEYID");
                    break;
                case Report.ReportType.LicenseServer:
                    imageIndex = Enums.GetIconIndex("LICENSE");
                    break;
                case Report.ReportType.ProductLicense:
                    imageIndex = Enums.GetIconIndex("ORDER");
                    break;
                case Report.ReportType.LicensePacket:
                    imageIndex = Enums.GetIconIndex("ZIPPEDFILE");
                    break;
                default: break;
            }
            return imageIndex;
        }

        //read xml for reports, load their conditions into report node tags
        private void LoadReportListView()
        {
            List<Report> reportList = _reportManager.GetReports();
            ////populate tree node
            TreeNode reportNode = null;
            //RootNode not selectable, 
            foreach (TreeNode node in reportsTreeView.Nodes)
            {
                node.Nodes.Clear();
                if (node.Name == "RootNode")
                {
                    foreach (Report report in reportList)
                    {
                        reportNode = new TreeNode();
                        //add icon depending on type
                        reportNode.ImageIndex = reportNode.SelectedImageIndex = GetReportImageIndex(report.Type);
                        reportNode.Text = report.ID;
                        reportNode.Tag = report;
                        node.Nodes.Add(reportNode);
                    }
                }
            }
            reportsTreeView.SelectedNode = reportsTreeView.Nodes[0];
            LoadReportNode(reportsTreeView.SelectedNode);
            reportsTreeView.Nodes[0].ExpandAll();
        }
        #endregion

        #endregion

        #region Database Methods
        /// <summary>
        /// Retrieves customers from database.
        /// </summary>
        /// <param name="searchString">The string used to filter customers. Valid searches are all/part customer name and customer ID</param>        
        /// <param name="loadOption">Determines whether or not to load retrieved licenses with packet information.</param>
        private void LoadDBCustomers(string searchString, bool loadOption)
        {
            IList<CustomerTable> data = null;

            loadingCircle1.Visible = true;
            loadingCircle1.SpokeThickness = 6;
            loadingCircle1.InnerCircleRadius = 12;
            loadingCircle1.OuterCircleRadius = 13;
            loadingCircle1.NumberSpoke = 10;
            loadingCircle1.Active = true;
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
            CustomersListView.Items.Clear();
        }
        /// <summary>
        /// Retrieves customers list from database on background thread.
        /// </summary>
        /// <param name="worker">BackgroundWorker thread</param>        
        /// <param name="searchString">The string used to filter customers. Valid searches are all/part customer name and customer ID</param>        
        /// <param name="loadOption">Determines whether or not to load retrieved licenses with packet information.</param>
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
                searchToolStripTextBox.Text = "Customer Filter";
            newCustMainToolStripBtn.Enabled = m_Permissions.pt_create_modify_key.Value;
            SearchToolStripLabel.Visible = SearchToolStripLabel.Enabled = true;
            searchToolStripTextBox.Visible = searchToolStripTextBox.Enabled = true;
            customerToolStripStatusLabel.Text = string.Empty;
            //restore searchbox text
        }
        /// <summary>
        /// Retrieves license servers from database.
        /// </summary>
        /// <param name="searchString">The string used to filter license servers. Valid searches are all/part license server name and product license name</param>
        /// <param name="selectedName">The license server to be selected on detailtreeview</param>
        /// <param name="loadOption">Determines whether or not to load retrieved licenses with packet information.</param>
        private void LoadDBLicenses(string searchString, string selectedName, string destName, bool loadOption)
        {
            IList<LicenseTable> data = null;

            loadingCircle1.Visible = true;
            loadingCircle1.SpokeThickness = 6;
            loadingCircle1.InnerCircleRadius = 12;
            loadingCircle1.OuterCircleRadius = 13;
            loadingCircle1.NumberSpoke = 10;
            loadingCircle1.Active = true;
            
            using (BackgroundWorker worker = new BackgroundWorker())
            {
                worker.DoWork += ((sender, e) => data = GetDBLicenses(worker, destName, searchString, loadOption));
                //returned from work so this can access ui main thread
                worker.RunWorkerCompleted += ((sender, e) => OnDBLicensesRetrieved(data, selectedName));
                worker.RunWorkerAsync();
            }
        }
        //Only called on a backgroundworker 
        private IList<LicenseTable> GetDBLicenses(BackgroundWorker worker, string destName, string searchString, bool loadOption)
        {
            IList<LicenseTable> licenses = null;
            Service<ICreator>.Use((client) =>
            {
                try
                {                    //needs to retrieve licenses by customer/destid or destname
                    licenses = client.GetLicensesByDestination(_selectedLicenseCustomer.Name, destName, searchString, loadOption);
                }
                catch (Exception e) { }
            });
            return licenses.ToList();
        }
        private void OnDBLicensesRetrieved(IList<LicenseTable> data, string selectedName)
        {
            //System.Diagnostics.Stopwatch watch = new System.Diagnostics.Stopwatch();
            //watch.Start();
            if (data != null)
                LoadLicenseView(data, selectedName);
            //watch.Stop();
            //MessageBox.Show(string.Format("Elapsed: {0}\nIn milliseconds: {1}\nIn timer ticks: {2}", watch.Elapsed, watch.ElapsedMilliseconds, watch.ElapsedTicks), "INFORMATION", MessageBoxButtons.OK);
            if (searchToolStripTextBox.ForeColor == SystemColors.InactiveCaptionText)
                searchToolStripTextBox.Text = "License Server Filter";
            loadingCircle1.Active = false;
            loadingCircle1.Visible = false;
            customerToolStripStatusLabel.Text = "Customer : " + _selectedLicenseCustomer.Name;
            //customerToolStripLabel.Text = _selectedLicenseCustomer.Name;
        }
        
        //private void GeneratePacket(string packetName, string licenseName, DateTime expDate, string description, string outputPath)
        private void GeneratePacket(PacketDialogData data)
        {
            bool bSuccess = false;
            using (BackgroundWorker worker = new BackgroundWorker())
            {
                worker.DoWork += ((sender, e) => bSuccess = GenerateDBPacket(data));//packetName, licenseName, expDate, description, outputPath));
                worker.RunWorkerCompleted += ((sender, e) => OnGeneratedPacket(data.LicenseName/*licenseName*/, bSuccess));
                worker.RunWorkerAsync();
            }
            loadingCircle1.Visible = true;
            loadingCircle1.SpokeThickness = 6;
            loadingCircle1.InnerCircleRadius = 12;
            loadingCircle1.OuterCircleRadius = 13;
            loadingCircle1.NumberSpoke = 10;
            loadingCircle1.Active = true;
        }
        //9 DB calls + number of extension transactions
        private bool GenerateDBPacket(PacketDialogData data)//string packetName, string licenseName, DateTime expDate, string description, string outputPath)
        {
            bool bSuccess = false;
            Service<ICreator>.Use((client) => 
            {
                Byte[] newByteArrayLicensePacket = null;
                string verificationCode = string.Empty;
                LicensePacket packet = new LicensePacket(s_CommLink, data.LicenseName);//licenseName);    
                packet.BuildLicensePackage(data); //(1 DB)    
                //strip license of deactivated product licenses (1 DB)              
                client.GenerateLicensePacket(data.PacketName,//packetName,
                                             packet.DatabaseRecord,
                                             data.ExpDate,//expDate,
                                             data.UserNotes,//description,
                                             ref verificationCode,
                                             ref newByteArrayLicensePacket,
                                             WindowsIdentity.GetCurrent().Name); 
                packet.SaveLicensePackage(); //(1 DB)
                if (newByteArrayLicensePacket != null)
                {
                    File.WriteAllBytes(string.Format("{0}\\{1}.{2}", data.SelectedDirectory/*outputPath*/, data.PacketName/*packetName*/, "packet"), newByteArrayLicensePacket);
                    if (File.Exists(string.Format("{0}\\{1}.{2}", data.SelectedDirectory/*outputPath*/, data.PacketName/*packetName*/, "packet")))
                    {
                        PacketTable storedPacket = client.GetPacketByVerificationCode(verificationCode); //(1 DB)
                        if (storedPacket != null)
                        {   //retrieve all transactions for a given license that doesnt have a packet id
                            List<TransactionTable> transactionList = client.GetNewTransactionsByLicenseName(data.LicenseName/*licenseName*/); //(1 DB)
                            List<int> extensionList = new List<int>();
                            //need to ignore deactivated pl transactions
                            foreach (TransactionTable recordedTransaction in transactionList)
                            {   //determine if any transactions are exp date, amount in days, or activations
                                if ((recordedTransaction.taType == (byte)TransactionType.ExpirationDate) ||
                                    (recordedTransaction.taType == (byte)TransactionType.ActivationTotal) ||
                                    (recordedTransaction.taType == (byte)TransactionType.ActivationAmount))
                                {
                                    if (!extensionList.Contains(recordedTransaction.taOrderID.Value))
                                        extensionList.Add(recordedTransaction.taOrderID.Value);
                                }
                                recordedTransaction.taPacketID = storedPacket.ID;
                            }
                            client.UpdateAllTransactions(transactionList); //(1 DB)
           
                            string txDescription;
                            IList<ProductLicenseTable> pltList = client.GetProductLicenses(data.LicenseName/*licenseName*/, true); //(1 DB)
                            List<ProductLicenseTable> updateList = new List<ProductLicenseTable>();
                            foreach (int plID in extensionList)
                            {
                                ProductLicenseTable plt = pltList.Where(p => p.ID == plID).FirstOrDefault();
                                if (plt != null)
                                {   //bump extension count if exp date has been updated.
                                    plt.Extensions += 1;
                                    if (plt.Activations == 0)
                                        txDescription = string.Format("Expires - {0}", plt.ExpirationDate.Value.ToShortDateString());
                                    else
                                        txDescription = string.Format("({0} Activations/{1} Days) Expires - {2}", plt.Activations, plt.ActivationAmount, plt.ExpirationDate.Value.ToShortDateString());
                                    //create transaction with type extension string Expires x/x/x x Activations/x Days value = Extension x
                                    TransactionManager.CreateTransaction(TransactionType.Extension,
                                                                         data.LicenseName/*licenseName*/,
                                                                         plt.plID,
                                                                         txDescription,
                                                                         string.Format("Extension {0}", plt.Extensions),
                                                                         string.Empty);
                                    updateList.Add(plt);
                                }                               
                            }
                            if(updateList.Count > 0) client.UpdateAllProductLicenses(updateList); //(1 DB)
                            transactionList = client.GetNewTransactionsByLicenseName(data.LicenseName/*licenseName*/); // (1 DB)
                            foreach (TransactionTable recordedTransaction in transactionList)
                            {   //update new extension transactions with packet id
                                if ((recordedTransaction.taType == (byte)TransactionType.Extension))
                                    recordedTransaction.taPacketID = storedPacket.ID;
                            }
                            client.UpdateAllTransactions(transactionList); //(1 DB)
                            bSuccess = true;
                        }
                    }
                }
            });
            return bSuccess;
        }

        private void OnGeneratedPacket(string licenseName, bool bSuccess)
        {
            loadingCircle1.Active = false;
            loadingCircle1.Visible = false;
            if (!bSuccess)
                MessageBox.Show(string.Format("Failed to generate license packet for {0}", licenseName), "Generate License Packet");
            LoadTransactionItems(licenseName);
            LoadPacketItems(licenseName);
            SetLicenseServerState(DetailTreeView.SelectedNode, false);                        
        }
        #endregion

        private void DetailTreeView_ItemDrag(object sender, ItemDragEventArgs e)
        {
            StringBuilder dragString = new StringBuilder();
            TreeNode node = e.Item as TreeNode;
            dragString.Append("\n");
            if (node.Tag is LicenseServer)
            {
                LicenseServer ls = node.Tag as LicenseServer;
                Service<ICreator>.Use(client =>
                {
                    List<ProductLicenseTable> plList = client.GetProductLicenses(ls.Name, true);
                    foreach (ProductLicenseTable plt in plList.OrderBy(p => p.ProductID))
                    {
                        dragString.Append(s_CommLink.GetProductName(plt.ProductID));
                        dragString.Append(" ");
                        dragString.Append(plt.plID);
                        dragString.Append("\n");
                    }
                });
                //need to copy all product licenses also
            }
            else if (node.Tag is ProductCollection)
            { 
                //loop through and append Product - ID
                ProductCollection pc = node.Tag as ProductCollection;
                if (pc.Collection.Count > 0)
                {
                    foreach (ProductLicense pl in pc.Collection)
                    {
                        dragString.Append(pl.ProductName);
                        dragString.Append(" ");
                        dragString.Append(pl.ID);
                        dragString.Append("\n");
                    }
                }
            }
            else if (node.Tag is ProductLicense)
            { 
                ProductLicense pl = node.Tag as ProductLicense;
                dragString.Append(pl.ProductName);
                dragString.Append(" ");
                dragString.Append(pl.ID);                
            }
            DoDragDrop(dragString.ToString(), DragDropEffects.Copy | DragDropEffects.Move);            
        }

        private void totalModuleListToolStripButton_Click(object sender, EventArgs e)
        {
            if(DetailTreeView.SelectedNode.Tag is ProductLicense)
            {
                ProductLicense productLicense = DetailTreeView.SelectedNode.Tag as ProductLicense;
                if (MessageBox.Show(string.Format("Please confirm setting maximum units to {0} module list.",productLicense.ProductName),
                                    "Set Maximum Module List Units",
                                    MessageBoxButtons.OKCancel,
                                    MessageBoxIcon.Exclamation) == DialogResult.OK)
                {
                    productLicense.SetLicensedToTrial();
                    Service<ICreator>.Use((client) =>
                    {
                        ProductLicenseTable plt = client.GetProductLicense(productLicense.ID);
                        if (plt != null)
                        {
                            TransactionManager.CreateTransaction(TransactionType.Module,
                                                                  string.Empty,
                                                                  productLicense.ID,
                                                                  string.Format("Set Module List to Maximum Units", ProductName),
                                                                  "",
                                                                  "");
                            client.MarkDirty(productLicense.LicenseServer);
                        }
                    });
                    SetLicenseServerState(DetailTreeView.SelectedNode, false);
                    PopulateDetailListView(DetailPropertyGrid.SelectedObject);
                }
            }
        }

        private void defaultModuleListToolStripButton_Click(object sender, EventArgs e)
        {
            if(DetailTreeView.SelectedNode.Tag is ProductLicense)
            {
                ProductLicense productLicense = DetailTreeView.SelectedNode.Tag as ProductLicense;
                if (MessageBox.Show(string.Format("Please confirm setting default units to {0} module list.", productLicense.ProductName),
                                    "Set Default Module List Units",
                                    MessageBoxButtons.OKCancel,
                                    MessageBoxIcon.Exclamation) == DialogResult.OK)
                {
                    productLicense.SetTrialToLicensed(true);
                    Service<ICreator>.Use((client) =>
                    {
                        ProductLicenseTable plt = client.GetProductLicense(productLicense.ID);
                        if (plt != null)
                        {
                            TransactionManager.CreateTransaction(TransactionType.Module,
                                                                  string.Empty,
                                                                  productLicense.ID,
                                                                  string.Format("Set Module List to Default Units", ProductName),
                                                                  "",
                                                                  "");
                            client.MarkDirty(productLicense.LicenseServer);
                        }
                    });
                    SetLicenseServerState(DetailTreeView.SelectedNode, false);
                    PopulateDetailListView(DetailPropertyGrid.SelectedObject);
                }
            }
        }

    }
}
