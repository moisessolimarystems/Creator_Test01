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

/*
 * TODO : 
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
        private ListViewMgr _lvManager; //manages standard listview functions
        private ListView storageListView; //internal listview to store full contents 
        public static CommunicationLink s_CommLink; //link to license manager/server functions
        private PermissionsTable m_Permissions; //table to hold permissions
        private string m_selectedDirectory; //last selected directory for packet
        private List<String> m_ServerList;  //list of valid servers stored on user system
        private string m_searchString = ""; //search string for filtering view
        private string m_CurrentLicenseName = "";   //current active license server        
        //used by hardware key view
        private string _selectedHardwareKeyCustomer = "";   //current active hardware key customer
        private CustomerProperty _selectedLicenseCustomer;  //current active license customer
        private string _selectedTreeNodeKey = "";   // current active tree node
        private ReportManager _reportManager;   //manages report functions
        private TreeNode _copyNode; //node to copy for report tree view
        private Bitmap memoryImage; //used to hold screen capture for printing

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
            m_ServerList = new List<String>();
            s_CommLink = new CommunicationLink();
            storageListView = new ListView();
            _selectedLicenseCustomer = new CustomerProperty();
            _reportManager = new ReportManager(s_CommLink);
            _lvManager = new ListViewMgr();
            _lvManager.SetListViewColumnSorter(DetailListView);
            _lvManager.SetListViewColumnSorter(TransactionListView);
            _lvManager.SetListViewColumnSorter(CustomersListView);
            _lvManager.SetListViewColumnSorter(LicensePacketListView);
            _lvManager.SetListViewColumnSorter(HardwareKeyListView);
            _lvManager.SetListViewColumnSorter(ReportListView);
            ResetMainToolStripMenu();
            //set to make read-only items not greyed-out            
            DetailPropertyGrid.ViewForeColor = Color.FromArgb(254, 0, 0, 0);
        }

        #region Enable/Disable ContextMenu

        private void ResetContextMenu(ToolStripItemCollection items)
        {
            foreach (ToolStripItem tsm in items)
            {
                tsm.Visible = false;
            }
        }

        private void DisableContextMenu(ToolStripItemCollection items)
        {
            foreach (ToolStripItem tsm in items)
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
            DisableContextMenu(addLicMainToolStripBtn.DropDownItems);
            DisableContextMenu(addProductLicenseMainToolStripDropDownBtn.DropDownItems);
            printToolStripButton.Enabled = true; //print button always available
        }

         private void EnableLicenseMainToolStripDropDownMenuItems()
        {
            if (DetailTreeView.SelectedNode.Tag is LicenseServerProperty)                                           
                newStandardLicenseToolStripMenuItem.Enabled = m_Permissions.pt_create_modify_key.Value;            
        }

        private void EnableProductLicenseMainToolStripDropDownMenuItems()
        {
            LicenseServerProperty licData = DetailTreeView.SelectedNode.Tag as LicenseServerProperty;
            //disable new order if at least two validation tokens do not exist    
            if (m_Permissions.pt_create_modify_key.Value)            
                if (licData.IsEnabled)
                    newProductLicenseMainToolStripMenuItem.Enabled = true;
        }

        private void EnableLicenseContextMenu(bool value)
        {
            LicenseServerProperty licData = null;
            if (DetailTreeView.SelectedNode != null)
                licData = DetailTreeView.SelectedNode.Tag as LicenseServerProperty;
            else
                DetailTreeView.SelectedNode = DetailTreeView.Nodes[0];
            expandAllToolStripMenuItem.Visible = !DetailTreeView.SelectedNode.IsExpanded;
            expandAllToolStripMenuItem.Enabled = (DetailTreeView.SelectedNode.Nodes.Count == 0) ? false : true;
            collapseToolStripMenuItem.Visible = DetailTreeView.SelectedNode.IsExpanded;
            //add license items
            newLicenseToolStripMenuItem.Visible = true;
            newLicenseToolStripMenuItem.Enabled = (DestNameComboBox.Items.Count > 2) && m_Permissions.pt_create_modify_key.Value;
            newProductLicenseToolStripMenuItem.Visible = true; 
            newProductLicenseToolStripMenuItem.Enabled = false;
            lcmToolStripSeparator3.Visible = true;
            //find item
            findToolStripMenuItem.Visible = true;
            //separator item
            lcmToolStripSeparator1.Visible = true;
            //generate license item
            createPacketFileToolStripMenuItem.Visible = true;
            createPacketFileToolStripMenuItem.Enabled = false;
            validationTokensToolStripMenuItem.Visible = true;
            validationTokensToolStripMenuItem.Enabled = false;
            //separator item
            lcmToolStripSeparator4.Visible = true;
            //also need to account for admin allowed extra new orders 
            if (licData != null)
            {
                if (m_Permissions.pt_create_modify_key.Value)
                {
                    //license items
                    deleteToolStripMenuItem.Enabled = m_Permissions.pt_create_modify_key.Value;              
                    //allow refresh if license is not expired or has validation tokens          
                    lcmToolStripSeparator2.Visible = true;
                    if (!licData.IsActive)
                    {
                        deleteToolStripMenuItem.Visible = true;
                        if (licData.HasLostToken)
                            reactivateToolStripMenuItem.Visible = true;
                    }
                    else
                    {
                        deactivateToolStripMenuItem.Visible = true;                        
                    }            
                    if (licData.IsEnabled)
                        createPacketFileToolStripMenuItem.Enabled = true;
                    newProductLicenseToolStripMenuItem.Enabled = licData.IsActive && m_Permissions.pt_create_modify_key.Value;
                    validationTokensToolStripMenuItem.Enabled  = licData.IsActive && m_Permissions.pt_create_modify_key.Value;
                    newTokenToolStripMenuItem.Enabled          = !licData.IsEnabled && m_Permissions.pt_create_modify_key.Value;
                    clearToolStripMenuItem.Enabled             = licData.IsEnabled && !licData.HasActiveHardwareToken && m_Permissions.pt_create_modify_key.Value;
                    markLostToolStripMenuItem.Enabled          = licData.HasActiveHardwareToken && m_Permissions.pt_create_modify_key.Value;
                }                
            }
            deactivateToolStripMenuItem.Enabled = m_Permissions.pt_create_modify_key.Value;
            reactivateToolStripMenuItem.Enabled = m_Permissions.pt_create_modify_key.Value;         
        }

        private void EnableProductLicenseContextMenu()
        {
            expandAllToolStripMenuItem.Visible = !DetailTreeView.SelectedNode.IsExpanded;
            expandAllToolStripMenuItem.Enabled = (DetailTreeView.SelectedNode.Nodes.Count == 0) ? false : true;
            collapseToolStripMenuItem.Visible = DetailTreeView.SelectedNode.IsExpanded;
            //allow deactivate if pl is not deactivated
            deactivateToolStripMenuItem.Enabled = deleteToolStripMenuItem.Enabled = m_Permissions.pt_create_modify_key.Value;
            if (DetailTreeView.SelectedNode.Tag is ProductLicenseProperty)
            {
                lcmToolStripSeparator1.Visible = true;
                LicenseServerProperty lsProperty;
                if (DetailTreeView.SelectedNode.Parent.Parent.Tag is LicenseServerProperty)
                    lsProperty = DetailTreeView.SelectedNode.Parent.Parent.Tag as LicenseServerProperty;
                else
                    lsProperty = DetailTreeView.SelectedNode.Parent.Parent.Parent.Tag as LicenseServerProperty;
                ProductLicenseProperty plProperty = DetailTreeView.SelectedNode.Tag as ProductLicenseProperty;
                if (plProperty.IsActive && plProperty.Status != ProductLicenseState.AddOn)
                    deactivateToolStripMenuItem.Visible = true;
                else
                    deleteToolStripMenuItem.Visible = true;

            }
            lcmToolStripSeparator3.Visible = true;
            findToolStripMenuItem.Visible = true;
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
            printToolStripButton.Enabled = true;
            navigateBackToolStripButton.Enabled = true;
            if (!searchToolStripTextBox.Visible) searchToolStripTextBox.Visible = true;
            if (!SearchToolStripLabel.Visible) SearchToolStripLabel.Visible = true;
            searchToolStripTextBox.Enabled = true;
            SearchToolStripLabel.Enabled = true;
            if (node.Tag is LicenseServerProperty)
            {
                addLicMainToolStripBtn.Enabled = m_Permissions.pt_create_modify_key.Value;
                addProductLicenseMainToolStripDropDownBtn.Enabled = (node.Tag as LicenseServerProperty).IsActive && m_Permissions.pt_create_modify_key.Value;
                createPacketMainToolStripBtn.Enabled = (node.Tag as LicenseServerProperty).IsActive;
                EnableLicenseMainToolStripDropDownMenuItems();
                EnableProductLicenseMainToolStripDropDownMenuItems();
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
        private void CreatorForm_FormClosing(object sender, FormClosingEventArgs e)
        {
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
        private void newCustMainToolStripBtn_Click(object sender, EventArgs e)
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
        private void searchToolStripTextBox_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter || e.KeyCode == Keys.Return)
            {
                SearchCurrentView(searchToolStripTextBox.Text);
            }
        }
        private void newProductLicenseMainToolStripDropDownBtn_Click(object sender, EventArgs e)
        {            
            AddProductLicense();
        }
        private void newStandardMainToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CreateStandardLicense();
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
            if (newProductLicenseMainToolStripMenuItem.Enabled)
                AddProductLicense();
        }
        private void addProductLicenseMainToolStripDropDownBtn_DropDownOpening(object sender, EventArgs e)
        {
            EnableProductLicenseMainToolStripDropDownMenuItems();
        }
        private void navigateForwardToolStripButton_Click(object sender, EventArgs e)
        {
            if (MainTabControl.SelectedTab == LicensesTabPage)
            {
                CustomersListView.Visible = false;
                CustomerToolStrip.Visible = true;
                LoadDBLicenses("", "", _selectedLicenseCustomer.Name, false);
                navigateBackToolStripButton.Enabled = true;
                navigateForwardToolStripButton.Enabled = false;
            }
            else
            {
                SearchCurrentView(_selectedHardwareKeyCustomer);
            }
        }
        private void navigateBackToolStripButton_Click(object sender, EventArgs e)
        {
            ResetMainToolStripMenu();
            if (MainTabControl.SelectedTab == LicensesTabPage)
            {
                CustomersListView.Visible = true;
                navigateBackToolStripButton.Enabled = false;
                navigateForwardToolStripButton.Enabled = true;
                CustomerToolStrip.Visible = false;
            }
            else
            {   //load customer view
                _selectedHardwareKeyCustomer = "";
                EnableHardwareCustomerView();
            }
            SearchCurrentView("");
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
            if (searchToolStripTextBox.Text.Length == 0 && !searchToolStripTextBox.Focused)
            {
                if (CustomersListView.Visible || _selectedHardwareKeyCustomer.Length == 0)
                    searchToolStripTextBox.Text = "Customer Filter";
                else
                    searchToolStripTextBox.Text = "License Server Filter";
                searchToolStripTextBox.Font = new Font(this.Font, FontStyle.Italic);
                searchToolStripTextBox.ForeColor = SystemColors.InactiveCaptionText;
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
                    foreach (SolimarLicenseProtectionKeyInfo attachedKey in pkeyList)
                    {
                        if (attachedKey.IsKeyTypeVerification())
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
                keyName = string.Format("{0:x4}-{1:x4}", custRec.SCRnumber, nextKeyValue);
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
            string keyValue = "";
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
        private void softwareTokenPreferencesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (SoftwareTokenOptions dlg = new SoftwareTokenOptions())
            {
                if (dlg.ShowDialog(this) == DialogResult.OK)
                {
                }
            }
        }
        private void updateVersionsToolStripMenuItem_Click(object sender, EventArgs e)
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
                _selectedHardwareKeyCustomer = (_selectedLicenseCustomer.Name != null) ? _selectedLicenseCustomer.Name : "";
                ResetMainToolStripMenu();
                SearchToolStripLabel.Visible = !(_selectedHardwareKeyCustomer.Length > 0);
                searchToolStripTextBox.Visible = !(_selectedHardwareKeyCustomer.Length > 0);
                SearchCurrentView("");
                loadingCircle1.Parent = splitContainer2.Panel1;
            }
            else
            {
                if (MainTabControl.SelectedTab == ReportTabPage)
                {
                    ResetMainToolStripMenu();
                    SearchToolStripLabel.Visible = false;
                    searchToolStripTextBox.Visible = false;
                    LoadReportListView();
                    loadingCircle1.Parent = splitContainer2.Panel1;
                }
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
                    searchToolStripTextBox.Text = "";
                    CustomerToolStrip.Visible = true;
                    findToolStrip.Visible = false;
                    CustomersListView.Visible = false;
                    //Load Destination Names into combobox
                    LoadDestinationNameComboBox(_selectedLicenseCustomer.Id);
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
                CustomerDialogData custData = new CustomerDialogData();
                if (newCustDlg.ShowDialog(this, custData) == DialogResult.OK)
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
                            DestinationNameTable newDestName = new DestinationNameTable();
                            newDestName.CustID = _selectedLicenseCustomer.Id;
                            newDestName.DestID = (int)client.GetNextDestinationID((uint)_selectedLicenseCustomer.Id);
                            newDestName.DestName = dlg.Name;
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
                    LoadDBLicenses("", _selectedTreeNodeKey, DestNameComboBox.Text, false);
                    _selectedTreeNodeKey = "";
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
            findToolStripTextBox.Text = "";
        }

        private void findToolStripTextBox_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter || e.KeyCode == Keys.Return)
            {
                findSearchString(findToolStripTextBox.Text);
            }
        }

        private void DetailTreeView_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Control && e.KeyCode.ToString() == "F")
            {
                findToolStrip.Visible = true;
            }            
        }
        private void DetailTreeView_AfterSelect(object sender, TreeViewEventArgs e)
        {
            LoadDetailTreeViewSelectedNode(e.Node);
        }

        private void LoadDetailTreeViewSelectedNode(TreeNode node)
        {
            DetailPropertyGrid.Enabled = true;
            DetailSplitContainer.Panel1Collapsed = false;
            switch (node.Level)
            { 
                case 1:            //level 1 - Product
                    m_CurrentLicenseName = (node.Parent.Tag as LicenseServerProperty).Name;
                    DetailSplitContainer.Panel1Collapsed = true;                    
                    LoadProductNode(node);
                    break;
                case 2:            //level 2 - Product License
                    LoadProductNode(node.Parent);
                    ProductLicenseProperty plProperty = node.Tag as ProductLicenseProperty;
                    LicenseServerProperty licProperty = node.Parent.Parent.Tag as LicenseServerProperty;
                    m_CurrentLicenseName = licProperty.Name;
                    if (!licProperty.IsActive || !plProperty.IsActive)
                        DetailPropertyGrid.Enabled = false;
                    break;
                case 3:            //level 3 - AddOn Product License
                    LoadProductNode(node.Parent.Parent);
                    ProductLicenseProperty plAddOn = node.Tag as ProductLicenseProperty;
                    LicenseServerProperty lsAddOn = node.Parent.Parent.Parent.Tag as LicenseServerProperty;
                    m_CurrentLicenseName = lsAddOn.Name;
                    if (!lsAddOn.IsActive || !plAddOn.IsActive)
                        DetailPropertyGrid.Enabled = false;
                    break;
                default:    //level 0 - Customer
                    if (node.Tag is LicenseServerProperty)
                    {
                        LoadLicenseNode(node);
                        m_CurrentLicenseName = (node.Tag as LicenseServerProperty).Name;
                        if (!(node.Tag as LicenseServerProperty).IsActive)
                            DetailPropertyGrid.Enabled = false;
                        createPacketMainToolStripBtn.Enabled = true;
                    }
                    break;
            }
            if (node.Name != "Empty")
            {
                DetailPropertyGrid.SelectedObject = null;
                DetailPropertyGrid.SelectedObject = (!(node.Tag is ProductCollectionProperty)) ? node.Tag : null;
                EnableToolStripMenu(node);
                //major performance hit
                SetLicenseServerState(node, false);
                SetCurrentViewTabPages(node);
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
                TreeNode node = DetailTreeView.GetNodeAt(p);
                if (node != null)
                {
                    if (node.Bounds.Contains(p))
                    {
                        //DetailTreeView.find
                        //// Go to the node that the user clicked
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
                //SetLicenseServerState(e.Node, false);
            }
            DetailTreeView.EndUpdate();
        }

        #region LicenseContextMenu Events
        private void findToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //determine if find is a product license or license server
            findToolStrip.Visible = true;
        }
        private void deleteToolStripMenuItem_Click(object sender, EventArgs e)
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
                    if (parentNode.Nodes.Count > 1)
                    {
                        selectedIndex = (selectedIndex > 0) ? --selectedIndex : ++selectedIndex;
                        DetailTreeView.SelectedNode = parentNode.Nodes[selectedIndex];
                    }
                    else
                        DetailTreeView.SelectedNode = parentNode;
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
                        EnableToolStripMenu(emptyNode);
                    }
                }
            }
            if (DetailTreeView.SelectedNode != null)
                SetLicenseServerState(DetailTreeView.SelectedNode, true);
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
        #endregion

        #region DetailListView Events
        private void DetailListView_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            //product level            
            switch (DetailTreeView.SelectedNode.Level)
            {
                case 0: //License
                    break;
                case 1: //Product
                    //initialize tooltip information for DetailListView - Modules
                    //retrieve order data using information from selected item
                    string information = "", expirationDate = "";
                    ProductCollectionProperty productLicenseList = DetailTreeView.SelectedNode.Tag as ProductCollectionProperty;
                    int moduleID;
                    Int32.TryParse(e.Item.Name, out moduleID);
                    StringBuilder sb = new StringBuilder();
                    foreach (ProductLicenseProperty productLicense in productLicenseList.ProductCollection)
                    {
                        ModuleTable mt = productLicense.ModuleList.Find(c => c.ModID == moduleID);
                        if (mt != null)
                        {
                            expirationDate = (productLicense.ExpirationDate.HasValue) ? productLicense.ExpirationDate.Value.ToShortDateString() : "None";
                            sb.AppendFormat("Product License - {0}, Exp Date - {1}\n", productLicense.ID, expirationDate);
                        }
                    }
                    information = sb.ToString();
                    if (information.Length > 0)
                        information = information.Remove(information.Length - 1, 1);
                    e.Item.ToolTipText = information;
                    break;
                case 2: //Product License, Add On License
                case 3:
                    //initialize buttons for DetailListView - Modules
                    ProductLicenseProperty pl = DetailPropertyGrid.SelectedObject as ProductLicenseProperty;
                    dlvAddToolStripButton.Visible = false;
                    dlvRemoveToolStripButton.Enabled = e.IsSelected & pl.IsAllowedRemoveModule((e.Item.Tag as ModuleProperty).ID);
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
                DisableContextMenu(DetailListViewContextMenuStrip.Items);
                if (DetailPropertyGrid.SelectedObject is LicenseServerProperty)
                {
                    if ((DetailPropertyGrid.SelectedObject as LicenseServerProperty).IsActive)
                    {
                        bShow = true;
                        if (m_Permissions.pt_module_pwd.Value)
                        {
                            dlvNewToolStripMenuItem.Visible = true;
                            dlvNewToolStripMenuItem.Enabled = (DetailListView.Items.Count == 0);
                            dlvDeleteToolStripMenuItem.Visible = true;
                            bool bFoundHardware = false;
                            foreach (ListViewItem lvItem in DetailListView.Items)
                            {
                                ValidationProperty token = lvItem.Tag as ValidationProperty;
                                if(token != null)
                                {
                                    if (token.TokenType == Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID)
                                    {
                                        bFoundHardware = true;
                                        break;
                                    }
                                }
                            }
                            dlvDeleteToolStripMenuItem.Enabled = !bFoundHardware && DetailListView.Items.Count > 0;
                        }
                    }
                }
                if (DetailPropertyGrid.SelectedObject is ProductLicenseProperty)
                {
                    bShow = true;
                    dlvEditToolStripMenuItem.Visible = true;
                    toolStripSeparator2.Visible = true;
                    incrementToolStripMenuItem.Visible = true;
                    decrementToolStripMenuItem.Visible = true;
                    ProductLicenseProperty plProperty = DetailPropertyGrid.SelectedObject as ProductLicenseProperty;
                    if (m_Permissions.pt_module_pwd.Value && plProperty.IsActive)
                    {
                        if (item != null)
                        {
                            ModuleProperty module = item.Tag as ModuleProperty;
                            dlvEditToolStripMenuItem.Enabled = !s_CommLink.IsDefaultModule(plProperty.ProductID, module.ID);
                            incrementToolStripMenuItem.Enabled = (plProperty.GetAvailableModuleUnits(module) > 0 && module.Value < module.MaxValue) && !s_CommLink.IsDefaultModule(plProperty.ProductID, module.ID);
                            decrementToolStripMenuItem.Enabled = (module.Value > 0) && !s_CommLink.IsDefaultModule(plProperty.ProductID, module.ID);
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
            if(m_Permissions.pt_create_modify_key.Value)
            {
                if(DetailListView.SelectedItems.Count > 0 && e.KeyCode.Equals(Keys.Delete))
                {
                    if (DetailPropertyGrid.SelectedObject is LicenseServerProperty)
                        if(dlvRemoveToolStripButton.Enabled)
                            RemoveValidationToken(DetailPropertyGrid.SelectedObject as LicenseServerProperty);
                }
                if(e.KeyCode.Equals(Keys.Insert))
                {
                    if (DetailPropertyGrid.SelectedObject is LicenseServerProperty)
                        CreateValidationToken(DetailPropertyGrid.SelectedObject as LicenseServerProperty);
                }
                if(e.KeyCode.Equals(Keys.Subtract))
                {
                    //throw dialog with error if fail
                    DecrementSelectedModules();
                }
                if(e.KeyCode.Equals(Keys.Add))
                {
                    //throw dialog with error if fail
                    IncrementSelectedModules();
                }
            }
        }

        private bool IncrementSelectedModules()
        {
            bool bRetVal = false;
            if (DetailPropertyGrid.SelectedObject is ProductLicenseProperty)
            {
                //throw error that is at max if available is 0
                ProductLicenseProperty plp = DetailPropertyGrid.SelectedObject as ProductLicenseProperty;
                foreach (ListViewItem lvItem in DetailListView.SelectedItems)
                {
                    ModuleProperty module = lvItem.Tag as ModuleProperty;
                    if (plp.GetAvailableModuleUnits(module) > 0 && module.Value < module.MaxValue)
                    {
                        IncrementModules(DetailListView.SelectedItems);
                        bRetVal = true;
                    }
                    //else throw error??
                }
            }
            return bRetVal;
        }

        private bool DecrementSelectedModules()
        {
            bool bRetVal = false;
            if (DetailPropertyGrid.SelectedObject is ProductLicenseProperty)
            {
                ProductLicenseProperty plp = DetailPropertyGrid.SelectedObject as ProductLicenseProperty;
                foreach (ListViewItem lvItem in DetailListView.SelectedItems)
                {
                    ModuleProperty module = lvItem.Tag as ModuleProperty;
                    if (module.Value > 0)
                    {
                        DecrementModules(DetailListView.SelectedItems);
                        bRetVal = true;
                    }
                }
            }
            return bRetVal;
        }
        //available units is always positive.
        private void IncrementModules(ListView.SelectedListViewItemCollection lvItems)
        {
            string moduleName = "", available = "";
            bool bSuccess = true;
            ProductLicenseProperty productLicense = DetailPropertyGrid.SelectedObject as ProductLicenseProperty;
            if(productLicense != null)
            {
                foreach(ListViewItem lvItem in lvItems)
                {
                    ModuleProperty module = lvItem.Tag as ModuleProperty;
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
            string moduleName = "", available = "";
            bool bSuccess = true;
            ProductLicenseProperty productLicense = DetailPropertyGrid.SelectedObject as ProductLicenseProperty;
            if(productLicense != null)
            {
                foreach(ListViewItem lvItem in lvItems)
                {
                    ModuleProperty module = lvItem.Tag as ModuleProperty;
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
            if (DetailPropertyGrid.SelectedObject is LicenseServerProperty)
                CreateValidationToken(DetailPropertyGrid.SelectedObject as LicenseServerProperty);
        }
        private void dlvDeleteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (DetailPropertyGrid.SelectedObject is LicenseServerProperty)   //token remove
                RemoveValidationToken(DetailPropertyGrid.SelectedObject as LicenseServerProperty);
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
            if (DetailPropertyGrid.SelectedObject is LicenseServerProperty)
                CreateValidationToken(DetailPropertyGrid.SelectedObject as LicenseServerProperty);
        }
        private void dlvRemoveToolStripButton_Click(object sender, EventArgs e)
        {
            if (DetailPropertyGrid.SelectedObject is LicenseServerProperty)   //token remove
                RemoveValidationToken(DetailPropertyGrid.SelectedObject as LicenseServerProperty);
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
                    ProductLicenseState oldStatus = (ProductLicenseState)e.OldValue;
                    ProductLicenseState newStatus = (ProductLicenseState)e.ChangedItem.Value;
                    if (oldStatus == ProductLicenseState.AddOn &&
                        newStatus == ProductLicenseState.Licensed)
                    {
                        TreeNode parentNode = DetailTreeView.SelectedNode.Parent;
                        int selectedIndex = DetailTreeView.SelectedNode.Index;
                        DeleteProductLicenseNode(DetailTreeView.SelectedNode, false);
                        if (DetailTreeView.SelectedNode.Nodes.Count > 1)
                        {
                            selectedIndex = (selectedIndex > 0) ? --selectedIndex : ++selectedIndex;
                            DetailTreeView.SelectedNode = parentNode.Nodes[selectedIndex];
                        }
                        else
                            DetailTreeView.SelectedNode = parentNode;
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
            ShowTransactionListView(transactionToolStripComboBox.SelectedItem.ToString(), "");
        }
        #endregion

        #region HardwareKeyListView Events
        private void HardwareKeyListView_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            if (_selectedHardwareKeyCustomer.Length > 0)
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
                        LoadHardwareKeyListView("", _selectedHardwareKeyCustomer);
                        //ValidationKeysTabPage.Text = string.Format("Hardware Keys - [{0}]", _selectedHardwareKeyCustomer);
                    }
                }
                else
                {
                    if (item.SubItems[1].Text != "")
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
                PacketContextMenuStrip.Show(packetDescriptionRichTextBox, e.X, e.Y);
            }
        }
        #endregion

        #region packetToolStripComboBox Events
        private void packetToolStripComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (packetToolStripComboBox.SelectedIndex)
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
                        if (packetItem.StateImageIndex != 0)
                        {
                            verifyToolStripMenuItem.Enabled = true;
                            break;
                        }
                    }
                }
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
            int imageIndex = Enums.GetListViewIconIndex("VERIFIED");
            using (PacketVerification dlg = new PacketVerification())
            {   //selectedItem text will be packet name
                PacketVerificationDialogData data = new PacketVerificationDialogData();
                data.LicenseName = GetLicenseNameFromTreeView();
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                {
                    if (data.Verified)
                    {
                        //create transaction
                        TransactionManager.CreateTransaction(TransactionType.PacketVerification, 
                                          data.LicenseName, 
                                          "", 
                                          "Verified packet", 
                                          data.SelectedPacketName,
                                          "");
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
                ReportProperty selectedReport = e.Node.Tag as ReportProperty;
                _reportManager.DeleteReport(selectedReport);
                selectedReport.ID = (e.Label != null) ? e.Label : e.Node.Text;
                _reportManager.SaveReport(selectedReport);
            }
        }

        #region ReportTreeViewContextMenuStrip

        private void lsReportToolStripMenuItem_Click(object sender, EventArgs e)
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
                data.Report.Type = ReportProperty.ReportType.LicenseServer;
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                {
                    TreeNode newReportNode = new TreeNode();
                    newReportNode.Text = data.Report.ID;
                    newReportNode.Tag = data.Report;
                    reportsTreeView.Nodes[0].Nodes.Add(newReportNode);
                    reportsTreeView.SelectedNode = newReportNode;
                    reportsTreeView.SelectedNode.BeginEdit();
                }
            }
        }
        private void plReportToolStripMenuItem_Click(object sender, EventArgs e)
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
                data.Report.Type = ReportProperty.ReportType.ProductLicense;
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                {
                    TreeNode newReportNode = new TreeNode();
                    newReportNode.Text = data.Report.ID;
                    newReportNode.Tag = data.Report;
                    reportsTreeView.Nodes[0].Nodes.Add(newReportNode);
                    reportsTreeView.SelectedNode = newReportNode;
                    reportsTreeView.SelectedNode.BeginEdit();
                }
            }
        }
        private void deleteReportToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (reportsTreeView.SelectedNode != null)
            {
                if (reportsTreeView.SelectedNode.Level != 0)
                {

                    ReportProperty selectedReport = reportsTreeView.SelectedNode.Tag as ReportProperty;
                    DialogResult result = MessageBox.Show(string.Format("Please confirm deletion of report: {0}.", selectedReport.ID), "Delete Report Confirmation",
                                  MessageBoxButtons.OKCancel, MessageBoxIcon.Exclamation);
                    if (result == DialogResult.OK)
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
                data.Report = (reportsTreeView.SelectedNode.Tag as ReportProperty);
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                {
                    ReportProperty selectedReport = selectedNode.Tag as ReportProperty;
                    _reportManager.DeleteReport(selectedReport);
                    reportsTreeView.SelectedNode = null;
                    selectedNode.Tag = data.Report;
                    _reportManager.SaveReport(data.Report);
                    reportsTreeView.SelectedNode = selectedNode;
                }
            }
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
                if ((reportsTreeView.SelectedNode.Tag as ReportProperty).Type == ReportProperty.ReportType.LicenseServer)
                {
                    if (item.SubItems[1].Text != "")
                        LoadSelectedLicenseServer(item.SubItems[1].Text);
                }
                else if((reportsTreeView.SelectedNode.Tag as ReportProperty).Type == ReportProperty.ReportType.ProductLicense)
                {
                    if (item.SubItems[1].Text != "")
                        LoadSelectedProductLicense(item.SubItems[1].Text);
                }
            }
        }
        #endregion 
        #endregion

        #region Form Methods
        private int ConnectServer()
        {
            //using keyword forces scope onto object, so it will be disposed after it is done.
            using (ConnectDialog cd = new ConnectDialog())
            {
                /*customerToolStripLabel.Text*/ customerToolStripStatusLabel.Text = "";
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

        private void ShowPacketListView(string type)
        {
            LicensePacketListView.BeginUpdate();
            LicensePacketListView.Items.Clear();
            foreach (ListViewItem lvItem in storageListView.Items)
            {   //type length > 0 filter otherwise all
                if (lvItem.ImageIndex == Enums.GetListViewIconIndex(type) ||
                    type.Length == 0)
                {
                    ListViewItem clonedItem = lvItem.Clone() as ListViewItem;
                    clonedItem.Name = lvItem.Name;
                    LicensePacketListView.Items.Insert(0, clonedItem);
                }
            }
            _lvManager.AutoResizeColumns(LicensePacketListView);
            LicensePacketListView.EndUpdate();
        }

        private bool ValidTransactionType(string type, TransactionTable transaction)
        {       
            //knowing transaction type only helps for product 
            //need to know product license id for specific pl transactions
            bool bRetVal = false;
            if(type == "Product")
            {
                if (transaction.taOrderID != null)
                    bRetVal = true;
            }
            else if (type == "Product License")
            { 
                if(DetailTreeView.SelectedNode.Tag is ProductLicenseProperty)
                {
                    ProductLicenseProperty pl = DetailTreeView.SelectedNode.Tag as ProductLicenseProperty;                    
                    //specific to transaction product license id
                    if(transaction.taOrderID.HasValue && transaction.taOrderID.Value == pl.ProductLicenseDatabaseID)
                        bRetVal = true;
                }
            }
            else
                bRetVal = true; 
            return bRetVal;
        }

        //type -> All, Product, Product Licenses
        //All = everything
        //Product = all lvitems with description containing detailtreeview.node level or all
        //Product License = all lvitems with plid or product or all
        private void ShowTransactionListView(string type, string searchKey)
        {
            TransactionListView.BeginUpdate();
            TransactionListView.Items.Clear();           
            List<ListViewItem> items = new List<ListViewItem>();
            foreach (ListViewItem lvItem in storageListView.Items)
            {   
                if (ValidTransactionType(type, lvItem.Tag as TransactionTable))               
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
        //issue std selected license node is customer node
        //      all others are std license node 
        private void SearchCurrentView(string searchString)
        {
            if (searchToolStripTextBox.ForeColor == SystemColors.InactiveCaptionText)
                searchToolStripTextBox.Text = "";
            if (MainTabControl.SelectedTab == LicensesTabPage)
            {
                if (CustomersListView.Visible)
                    LoadDBCustomers(searchString, false);
                else
                    LoadDBLicenses(searchString, "", DestNameComboBox.Text, false);
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
                if (selectedTab == PropertyGridTabPage)
                    LoadDetailListView(DetailTreeView.SelectedNode.Tag);
                else if (selectedTab == TransactionsTabPage)
                {
                    transactionToolStripComboBox.Items.Clear();
                    transactionToolStripComboBox.Items.Add("All");
                    if (DetailTreeView.SelectedNode.Level >= 1)
                        transactionToolStripComboBox.Items.Add("Product");
                    if (DetailTreeView.SelectedNode.Level > 1)
                        transactionToolStripComboBox.Items.Add("Product License");
                    LoadTransactionItems(m_CurrentLicenseName);
                }
                else if (selectedTab == PacketTabPage)
                    LoadPacketItems(m_CurrentLicenseName);
            }
        }

        #region License Server Methods
        private void CreateStandardLicense()
        {
            LicenseServerProperty newLicense = new LicenseServerProperty();
            Service<ICreator>.Use((client) =>
            {
                CustomerTable custRec = client.GetCustomer(_selectedLicenseCustomer.Name, false);
                if (custRec == null)
                {
                    MessageBox.Show("Failed to initialize standard license", "Create Standard License Server");
                    return;
                }
                newLicense.CustID = (uint)custRec.SCRnumber;
                DestinationNameTable dnt = client.GetDestinationID(_selectedLicenseCustomer.Id, DestNameComboBox.Text);
                newLicense.DestName = dnt.DestName;
                newLicense.DestID = (uint)dnt.DestID;
                newLicense.GroupID = client.GetNextGroupID(newLicense.CustID, newLicense.DestID);
            });
            newLicense.Permissions = m_Permissions;
            //display settings form for product connections
            using (LicenseInfoForm dlg = new LicenseInfoForm("Create New License Server", ref s_CommLink))
            {
                LicenseDialogData data = new LicenseDialogData(newLicense);
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                    CreateLicenseNode(data);
            }
        }
        private void CreateLicenseEntity(LicenseServerProperty licProperties)
        {
            //Create new license file from CustomerProperty id and next available swID
            Service<ICreator>.Use((client) =>
            {            
                Lic_PackageAttribs licInfo = new Lic_PackageAttribs();
                //licproperties has custid, destid, groupid set
                licInfo.licLicenseInfoAttribs.TVal = licProperties.LicInfo;
                LicenseTable licRecord = new LicenseTable();
                licRecord.GroupID = (int)licProperties.GroupID;
                licRecord.DestinationID = (int)licProperties.DestID;
                licRecord.SCRnumber = (int)licProperties.CustID;                
                licRecord.LicenseName = licProperties.Name;
                licRecord.IsActive = true;
                licRecord.LicenseComments = licProperties.Comments;
                //need to add any validation tokens into licInfo.Stream
                licRecord.LicenseInfo = licInfo.Stream;
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
        private void CreateLicenseNode(LicenseDialogData data)
        {
            CreateLicenseEntity(data.LicInfo);
            //resort tree
            LoadDBLicenses("", data.LicInfo.Name, data.LicInfo.DestName, false);
        }
        private bool DeleteLicenseNode(TreeNode selectedLicenseNode, bool bVerify)
        {
            bool bDelete = false;
            LicenseServerProperty selectedLicense = selectedLicenseNode.Tag as LicenseServerProperty;
            if (bVerify)
            {
                DialogResult result = MessageBox.Show(string.Format("Please confirm deletion of license: {0}.", selectedLicense.Name), "Delete License Confirmation",
                                          MessageBoxButtons.OKCancel, MessageBoxIcon.Exclamation);
                if (result == DialogResult.OK)
                    bDelete = true;
            }
            if (bDelete)
            {
                DeleteAllProductLicenseEntities(selectedLicense.Name);
                selectedLicenseNode.Nodes.Clear();
                DeleteLicenseEntity((selectedLicenseNode.Tag as LicenseServerProperty).Name);
                DetailTreeView.Nodes.Remove(selectedLicenseNode);
                return true;
            }
            return false;
        }
        private void LoadLicenseNode(TreeNode licenseNode)
        {
            string productName;
            LicenseTable license = null;
            List<ProductLicenseTable> pltList = null;
            Service<ICreator>.Use((client) =>
            {
                license = client.GetLicenseByName(licenseNode.Name, false);
                licenseNode.Tag = new LicenseServerProperty(license, m_Permissions);
                if (license != null)
                    pltList = client.GetProductLicenses(license.LicenseName);
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
                        productNode.ImageIndex = Enums.GetDetailTreeViewIconIndex(s_CommLink.GetProductBaseName(productName));
                        productNode.SelectedImageIndex = productNode.ImageIndex;
                        productNode.Nodes.Add(new VirtualTreeNode());
                        //load products also
                        productNode.Tag = new ProductCollectionProperty(plt.ProductID);
                        (productNode.Tag as ProductCollectionProperty).ProductCollection.Add(new ProductLicenseProperty(plt, m_Permissions));
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
                        if (!bAdded) licenseNode.Nodes.Add(productNode);                           
                    }
                    else
                    {   //update
                        productNode = licenseNode.Nodes.Find(productName, false).First();
                        (productNode.Tag as ProductCollectionProperty).UpdateProductProperty(new ProductLicenseProperty(plt, m_Permissions));
                    }
                }

                //update product collection
                string prodName;
                List<TreeNode> removeNodes = new List<TreeNode>();
                //update product nodes
                foreach (TreeNode prodNode in licenseNode.Nodes)
                {   //remove any nodes that may have been removed.             
                    prodName = prodNode.Name;
                    if (prodName.Length > 0)
                    {
                        bool bFound = false;
                        foreach (ProductLicenseTable plt in pltList)
                        {
                            if (s_CommLink.GetProductName(plt.ProductID) == prodName)
                            {
                                bFound = true; //mark 
                                break;
                            }
                        }
                        if (!bFound)
                            removeNodes.Add(prodNode);
                        else
                        {
                            ProductCollectionProperty pcp = prodNode.Tag as ProductCollectionProperty;
                            List<ProductLicenseProperty> removeProductLicenses = new List<ProductLicenseProperty>();
                            foreach (ProductLicenseProperty pp in pcp.ProductCollection)
                            {
                                bFound = false;
                                foreach (ProductLicenseTable plt in pltList)
                                {
                                    if (plt.plID == pp.ID)
                                    {
                                        bFound = true; //mark 
                                        break;
                                    }
                                }
                                if (!bFound)
                                    removeProductLicenses.Add(pp);
                            }
                            foreach (ProductLicenseProperty product in removeProductLicenses)
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
        private void LoadLicenseView(IList<LicenseTable> licRecords, string selectedName)
        {
            DetailTreeView.BeginUpdate();
            DetailTreeView.Nodes.Clear();
            if (licRecords.Count > 0)
            {
                TreeNode[] nodes = new TreeNode[licRecords.Count()];
                int index = 0;
                //add all to stored list
                foreach (var license in licRecords.OrderBy(d => d.DestinationID).ThenBy(g => g.GroupID))
                {
                    TreeNode licNode = new TreeNode();
                    licNode.Text = licNode.Name = license.LicenseName;
                    licNode.ImageIndex = Enums.GetDetailTreeViewIconIndex("License");
                    licNode.SelectedImageIndex = licNode.ImageIndex;
                    if (!licNode.IsExpanded)
                        licNode.Nodes.Add(new VirtualTreeNode());
                    licNode.Tag = new LicenseServerProperty(license, m_Permissions);
                    nodes[index] = licNode;
                    index++;
                }
                DetailTreeView.Nodes.AddRange(nodes);
            }
            if (DetailTreeView.Nodes.Count > 0)
            {
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
            else
                DetailPropertyGrid.SelectedObject = null;
            DetailTreeView.EndUpdate();
        }
        private void LoadSelectedLicenseServer(string selectedLicense)
        {
            string[] lsString;
            int custID, destID;
            //load licenses, highlight particular license
            CustomersListView.Visible = false;
            MainTabControl.SelectedTab = LicensesTabPage;
            //get destination from string or.....store in tag?
            lsString = selectedLicense.Split("-".ToCharArray());
            //1)custid 2)destid 3) number 
            Service<ICreator>.Use((client) =>
            {
                custID = Int32.Parse(lsString[0], System.Globalization.NumberStyles.HexNumber);
                destID = Int32.Parse(lsString[1]);
                _selectedLicenseCustomer = new CustomerProperty(client.GetCustomer(custID.ToString(), false));
                _selectedTreeNodeKey = selectedLicense;
                LoadDestinationNameComboBox(custID, destID);
            });
        }
        private void LoadDestinationNameComboBox(int custID)
        {
            DestNameComboBox.Items.Clear();
            Service<ICreator>.Use((client) =>
            {
                List<DestinationNameTable> destNames = client.GetDestNamesByCustID(custID);
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
                else
                    LoadDBLicenses("", "", "", false);
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
                LoadDBLicenses("", "", "", false);
        }
        private void SetLicenseServerState(TreeNode currentNode, bool bAllLicenseServers)
        {
            TreeNode licenseNode = null;
            if (currentNode.Tag is LicenseServerProperty)
                licenseNode = currentNode;
            else if (currentNode.Tag is ProductCollectionProperty)
                licenseNode = currentNode.Parent;
            else if (currentNode.Tag is ProductLicenseProperty)
            {
                if ((currentNode.Tag as ProductLicenseProperty).ParentID != null)
                    licenseNode = currentNode.Parent.Parent.Parent;
                else
                    licenseNode = currentNode.Parent.Parent;
            }
            LicenseServerProperty selectedLicense;
            IList<string> modifiedLicenses = null;
            Service<ICreator>.Use((client) =>
            {
                modifiedLicenses = client.GetModifiedLicensesByCustomer(_selectedLicenseCustomer.Name);
            });
            foreach (TreeNode node in DetailTreeView.Nodes)
            {
                if (!bAllLicenseServers)
                    if (node.Name != licenseNode.Name)
                        continue;   //skip over all license nodes that don't match
                if (node.Tag != null)
                {
                    selectedLicense = node.Tag as LicenseServerProperty;
                    node.ToolTipText = "";
                    if (!selectedLicense.IsActive)
                    {
                        node.ImageIndex = node.SelectedImageIndex = Enums.GetDetailTreeViewIconIndex("LicenseDeactivated");
                        node.NodeFont = new Font(this.Font, FontStyle.Italic);
                        node.ForeColor = SystemColors.InactiveCaptionText;
                    }
                    else
                    {
                        node.NodeFont = this.Font;
                        node.ForeColor = this.ForeColor;
                        node.ImageIndex = node.SelectedImageIndex = Enums.GetDetailTreeViewIconIndex("License");
                        if (modifiedLicenses != null)
                        {
                            if (modifiedLicenses.Contains(selectedLicense.Name))
                            {
                                node.ImageIndex = node.SelectedImageIndex = Enums.GetDetailTreeViewIconIndex("LicenseUpdate");
                                node.ToolTipText = "Changes have been made to license. Generate license packet.";
                            }
                        }
                        if (!(selectedLicense).IsEnabled) //disable key, not enough tokens
                        {
                            node.ImageIndex = node.SelectedImageIndex = Enums.GetDetailTreeViewIconIndex("LicenseDisabled");
                            node.ToolTipText = "Not enough validation tokens are set. Add a new validation token to enable license.";
                        }
                    }
                }
                if (!bAllLicenseServers)
                    break;
            }
        }
        #endregion

        #region Validation Token Methods
        public void CreateValidationToken(LicenseServerProperty licInfo)
        {
            using (LicenseInfoForm dlg = new LicenseInfoForm("Add Validation Tokens", ref s_CommLink))
            {
                TokenDialogData data = new TokenDialogData(licInfo);
                string HardwareTokenValue = "";
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
                        foreach (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs token in data.TokenList)//licInfo.LicInfo.licVerificationAttribs.TVal.validationTokenList.TVal)
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
                                    dbToken = new TokenTable();
                                    dbToken.LicenseID = dbLicense.ID;
                                    dbToken.TokenType = (byte)token.tokenType.TVal;
                                    dbToken.TokenValue = token.tokenValue.TVal;
                                    dbToken.CustID = (int)data.LicInfo.CustID;
                                    client.CreateToken(dbToken);
                                }
                            }
                        }
                        string transactionMsg = (HardwareTokenValue != "") ? HardwareTokenValue : "Software";
                        TransactionManager.CreateTransaction(TransactionType.Token,
                                          licInfo.Name,
                                          "",
                                          "Add Validation Token(s)",
                                          transactionMsg,
                                          "");
                        client.MarkDirty(licInfo.Name);
                    });
                    PopulateDetailListView(data.LicInfo);
                    SetLicenseServerState(DetailTreeView.SelectedNode,false);
                }
            }
        }
        private void RemoveValidationToken(LicenseServerProperty selectedLicense)
        {
            string hardwareTokenValue = "";
            DialogResult dlgResult = MessageBox.Show("Clearing tokens will disable license. Click OK to continue with deletion of tokens.", "Delete Token Confirmation",
                        MessageBoxButtons.OKCancel, MessageBoxIcon.Warning);
            if (dlgResult == DialogResult.OK)
            {
                selectedLicense.LicInfo.licVerificationAttribs.TVal.validationTokenList.TVal.Clear();
                Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs licCodeToken = new Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs();
                licCodeToken.tokenType.TVal = Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttLicenseCode;
                selectedLicense.LicInfo.licVerificationAttribs.TVal.validationTokenList.TVal.Add(licCodeToken);
                Service<ICreator>.Use((client) =>
                {
                    IList<TokenTable> tokens = client.GetTokensByLicenseName(selectedLicense.Name);
                    foreach (TokenTable token in tokens)
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
                                client.DeleteToken(token);
                        }
                    }
                    string transactionMsg = (hardwareTokenValue != "") ? hardwareTokenValue : "Software";
                    TransactionManager.CreateTransaction(TransactionType.Token,
                                                         selectedLicense.Name,
                                                         "",
                                                         "Remove Token(s)",
                                                         transactionMsg,
                                                         "");
                    client.MarkDirty(selectedLicense.Name);
                });

            }
            PopulateDetailListView(selectedLicense);
            SetLicenseServerState(DetailTreeView.SelectedNode, false);
        }
        private bool ValidationTokenCompare(Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs token, Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs storedToken)
        {
            bool found = false;
            if (storedToken.tokenType.TVal.Equals(token.tokenType.TVal))
                if (storedToken.tokenValue.TVal.Equals(token.tokenValue.TVal))
                    found = true;
            return found;
        }
        #endregion

        #region Product Methods
        //create only done on new contract with new product        
        private void LoadProductNode(TreeNode node)
        {
            List<ProductLicenseTable> plRecords = null;           
            if (node.Parent != null)
            {
                Service<ICreator>.Use((client) =>
                {
                    plRecords = client.GetProductLicensesByProduct(node.Parent.Name, (byte)s_CommLink.GetProductID(node.Name));
                });
                TreeNode plParent;
                ProductLicenseProperty selectedProductLicense;
                if (plRecords != null)
                {
                    //System.Diagnostics.Trace.WriteLine("PopulateListView() - Start");
                    foreach (ProductLicenseTable plt in plRecords)
                    {
                        TreeNode plNode = null;
                        selectedProductLicense = (node.Tag as ProductCollectionProperty).GetProductLicenseProperty(plt.plID);
                        //if (!plt.IsActive)
                        //    selectedProductLicense.ProductConnection = 0;
                        if (node.Nodes.Find(plt.plID, true).Count() == 0)
                        {
                            plNode = new TreeNode(plt.plID);
                            plNode.Name = plt.plID;
                            plNode.Tag = new ProductLicenseProperty(plt, m_Permissions);
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
                                plNode.ImageIndex = Enums.GetDetailTreeViewIconIndex("ORDER");
                                plNode.SelectedImageIndex = plNode.ImageIndex;
                                node.Nodes.Add(plNode);
                            }
                        }
                        else
                        {
                            plNode = node.Nodes.Find(plt.plID, true).First();
                            plNode.Tag = new ProductLicenseProperty(plt, m_Permissions);
                        }
                        plNode.ForeColor = this.ForeColor;
                        plNode.NodeFont = this.Font;
                        if ((plNode.Tag as ProductLicenseProperty).IsExpired)
                        {
                            plNode.NodeFont = new Font(this.Font, FontStyle.Italic);
                            plNode.ForeColor = Color.Red;
                        }
                        if (!(plNode.Tag as ProductLicenseProperty).IsActive)
                        {
                            plNode.NodeFont = new Font(this.Font, FontStyle.Italic);
                            plNode.ForeColor = SystemColors.InactiveCaptionText;
                        }
                    }
                    //System.Diagnostics.Trace.WriteLine("PopulateListView() - End");
                    if (node.Nodes.Count > 0)
                    {
                        foreach (TreeNode plNode in node.Nodes)
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
                                node.Nodes.Remove(plNode);
                        }
                    }
                }
            }
        }
        private TreeNode CreateProductNode(ProductLicenseProperty plData)
        {
            TreeNode node = new TreeNode(plData.ProductName);
            node.Name = plData.ProductName;
            node.ImageIndex = Enums.GetDetailTreeViewIconIndex(s_CommLink.GetProductBaseName(node.Name));
            node.SelectedImageIndex = node.ImageIndex;
            node.Tag = new ProductCollectionProperty(plData.ProductID);
            (node.Tag as ProductCollectionProperty).ProductCollection.Add(plData);
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
            plNode.Tag = new ProductLicenseProperty(pltData, m_Permissions);
            (plNode.Tag as ProductLicenseProperty).ProductVersion = new LicenseVersion(pltData.ProductVersion);
            if (pltData.ParentProductLicenseID != null)
            {
                plNode.ImageIndex = Enums.GetDetailTreeViewIconIndex("ADDONORDER");
                plNode.SelectedImageIndex = plNode.ImageIndex;
            }
            else
            {
                plNode.ImageIndex = Enums.GetDetailTreeViewIconIndex("ORDER");
                plNode.SelectedImageIndex = plNode.ImageIndex;
            }
            return plNode;
        }
        private ProductLicenseTable CreateProductLicenseEntity(ProductLicenseTable pltData)
        {
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
                                                      "",
                                                      pltData.plID,
                                                      string.Format("Add {0} Product License", s_CommLink.GetProductName(pltData.ProductID)),
                                                      pltData.plID,
                                                      "");
                //update order index to next available index                
                licRec.OrderIndex = plRec.plIndex + 1;
                client.UpdateLicense(licRec, true);
                pltData = client.GetProductLicense(pltData.plID);
            });
            return pltData;
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
                    ProductLicenseTable pltDelete = null;
                    ProductLicenseProperty selectedProductLicense = (plNode.Tag as ProductLicenseProperty);
                    Service<ICreator>.Use((client) => { pltDelete = client.GetProductLicense(selectedProductLicense.ID); });
                    DeleteProductLicenseEntity(pltDelete);
                    ProductCollectionProperty pcp = productNode.Tag as ProductCollectionProperty;
                    pcp.RemoveProductProperty(selectedProductLicense);
                    if(pcp.ProductCollection.Count == 0)
                        productNode.Remove();
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
        private void DeleteProductLicenseEntity(ProductLicenseTable plt)
        {
            Lic_PackageAttribs.Lic_ProductInfoAttribs storedProduct = new Lic_PackageAttribs.Lic_ProductInfoAttribs();
            LicenseTable storedLicense = null;
            List<string> removeProductLicenses = new List<string>();
            Service<ICreator>.Use((client) =>
            {
                client.DeleteProductLicense(plt);
                removeProductLicenses.Add(plt.plID);
                if (plt.ParentProductLicenseID == null)
                {   //get addons, remove 
                    List<string> plAddons = client.GetAddOnProductLicenses(plt.plID);
                    ProductLicenseTable pltAddOn = null;
                    foreach (string addon in plAddons)
                    {
                        pltAddOn = client.GetProductLicense(addon);
                        if (pltAddOn != null)
                        {
                            client.DeleteProductLicense(pltAddOn);
                            removeProductLicenses.Add(pltAddOn.plID);
                        }
                    }
                }
                storedLicense = client.GetLicenseByID(plt.LicenseID, false);
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
                              "",
                              string.Format("Remove {0} Product License - {1}", s_CommLink.GetProductName(plt.ProductID), Enum.GetName(typeof(ProductLicenseState), plt.plState)),
                              plt.plID,
                              "");
        }
        private void LoadSelectedProductLicense(string selectedProductLicense)
        {
            string[] lsString;
            int custID = 0, destID = 0;
            //load licenses, highlight particular license
            CustomersListView.Visible = false;
            MainTabControl.SelectedTab = LicensesTabPage;
            //get destination from string or.....store in tag?
            lsString = selectedProductLicense.Split("-".ToCharArray());
            //1)custid 2)destid 3) number 4) type 5) index
            custID = Int32.Parse(lsString[0], System.Globalization.NumberStyles.HexNumber);
            destID = Int32.Parse(lsString[1]);
            Service<ICreator>.Use((client) =>
            {
                _selectedLicenseCustomer = new CustomerProperty(client.GetCustomer(custID.ToString(), false));
                _selectedTreeNodeKey = selectedProductLicense;
            });
            LoadDestinationNameComboBox(custID, destID);
            //expand selected license server then highlight product license
        }

        private string GetLicenseNameFromTreeView()
        {
            string licenseName = "";
            if (DetailTreeView.SelectedNode.Tag is LicenseServerProperty)
                licenseName = (DetailTreeView.SelectedNode.Tag as LicenseServerProperty).Name;
            else if (DetailTreeView.SelectedNode.Tag is ProductCollectionProperty)
                licenseName = (DetailTreeView.SelectedNode.Parent.Tag as LicenseServerProperty).Name;
            else
                licenseName = (DetailTreeView.SelectedNode.Tag as ProductLicenseProperty).LicenseServer;
            return licenseName;
        }

        //private void CloneProductLicense(ProductLicenseProperty plp)
        //{
        //    //show contract dialog
        //    int plIndex = 0;
        //    Lic_PackageAttribs licPackage = new Lic_PackageAttribs();
        //    ProductLicenseProperty plProperty = new ProductLicenseProperty(s_CommLink);
        //    string licenseName = plp.LicenseServer;
        //    Service<ICreator>.Use((client) =>
        //    {
        //        //IList<ProductLicenseTable> plList = client.GetProductLicenses(licenseName);
        //        ////****TODO**** Get correct max count by figuring out what the number of product licenses using activations are
        //        //if (plList.Where(c => !c.IsActive && c.Activations > 0).Count() > AppConstants.MaxProductLicenses)
        //        //{
        //        //    MessageBox.Show(licenseName + " has reached the maximum number of product licenses allowed.", "Product License Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        //        //    return;
        //        //}
        //        plIndex = client.GetNextProductLicenseIndex(licenseName);
        //    });
        //    plProperty.LicenseServer = licenseName;
        //    plProperty.Index = plIndex;
        //    plProperty.ID = Lic_LicenseInfoAttribsHelper.GenerateProductLicenseName(licenseName, plIndex);
        //    plProperty.Permissions = m_Permissions;
        //    //plProperty.
        //    //license file already created
        //    //display settings form for product connections
        //    using (LicenseInfoForm dlg = new LicenseInfoForm(string.Format("Clone Product License - {0}", plp.ID), ref s_CommLink))
        //    {
        //        ProductLicenseDialogData data = new ProductLicenseDialogData(plProperty);
        //        if (dlg.ShowDialog(this, data) == DialogResult.OK)
        //        {
        //            TreeNode productNode = DetailTreeView.SelectedNode.Nodes.Find(data.ProductLicense.Product.Name, false).FirstOrDefault();
        //            TreeNode plNode = CreateProductLicenseNode(data.ProductLicense);
        //            if (productNode == null)
        //            {
        //                productNode = CreateProductNode(data.ProductLicense);
        //                //add node in alpha order
        //                bool bAdded = false;
        //                if (DetailTreeView.SelectedNode.Nodes.Count > 0)
        //                {
        //                    foreach (TreeNode node in DetailTreeView.SelectedNode.Nodes)
        //                    {
        //                        if (string.Compare(node.Text, productNode.Text) > 0)
        //                        {
        //                            DetailTreeView.SelectedNode.Nodes.Insert(node.Index, productNode);
        //                            bAdded = true;
        //                            break;
        //                        }
        //                    }
        //                }
        //                if (!bAdded)
        //                    DetailTreeView.SelectedNode.Nodes.Add(productNode);
        //            }
        //            else //attach to product tree
        //            {
        //                CreateProductEntity(data.ProductLicense);
        //                (productNode.Tag as ProductCollectionProperty).ProductCollection.Add(data.ProductLicense.Product);
        //            }
        //            if (data.ProductLicense.ParentID == null)
        //                productNode.Nodes.Add(plNode);
        //            else
        //            {
        //                //add to parent order node
        //                TreeNode parentProductLicenseNode = DetailTreeView.SelectedNode.Nodes.Find(data.ProductLicense.ParentID, true).FirstOrDefault();
        //                parentProductLicenseNode.Nodes.Add(plNode);
        //            }
        //            DetailTreeView.SelectedNode = plNode;
        //            SetLicenseServerState(DetailTreeView.SelectedNode, true);
        //        }
        //    }
        //}

        private void AddProductLicense()
        {            
            //show contract dialog
            int plIndex = 0, licenseID = 0;
            ProductLicenseTable plt = new ProductLicenseTable();
            string licenseName = DetailTreeView.SelectedNode.Name;
            Service<ICreator>.Use((client) =>
            {
                plIndex = client.GetNextProductLicenseIndex(licenseName);    
                licenseID = client.GetLicenseByName(licenseName,false).ID;
            });
            plt.plIndex = plIndex;
            plt.plID = Lic_LicenseInfoAttribsHelper.GenerateProductLicenseName(licenseName, plIndex);
            plt.LicenseID = licenseID;
            //license file already created
            //display settings form for product connections
            using (LicenseInfoForm dlg = new LicenseInfoForm("Create New Product License", ref s_CommLink))
            {
                ProductLicenseDialogData data = new ProductLicenseDialogData(licenseName, plt);
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                {
                    string productName = s_CommLink.GetProductName(data.ProductLicense.ProductID);
                    TreeNode productNode = DetailTreeView.SelectedNode.Nodes.Find(productName, false).FirstOrDefault();                                                                   
                    TreeNode plNode = CreateProductLicenseNode(data.ProductLicense);                        
                    if (productNode == null)
                    {
                        productNode = CreateProductNode(plNode.Tag as ProductLicenseProperty);                        
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
                        (productNode.Tag as ProductCollectionProperty).ProductCollection.Add(plNode.Tag as ProductLicenseProperty);
                    }                    
                    if (data.ProductLicense.ParentProductLicenseID == null)
                        productNode.Nodes.Add(plNode);
                    else
                    {
                        //add to parent order node
                        TreeNode parentProductLicenseNode = DetailTreeView.SelectedNode.Nodes.Find(data.ProductLicense.ParentProductLicenseID, true).FirstOrDefault();
                        parentProductLicenseNode.Nodes.Add(plNode);
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
            if (!(m_selectedDirectory.Length > 0))
                m_selectedDirectory = Directory.GetCurrentDirectory();
            PacketProperty newPacket = new PacketProperty(string.Format("{0}-{1}-{2}-{3}-{4}-{5}-{6}", DetailTreeView.SelectedNode.Name, DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day, DateTime.Now.Hour, DateTime.Now.Minute, DateTime.Now.Second),
                                                  "",
                                                  DateTime.Today,
                                                  m_selectedDirectory, //Path.Combine(m_selectedDirectory, _selectedLicenseCustomer.Name),
                                                  "");

            using (LicenseInfoForm dlg = new LicenseInfoForm("Create New Packet", ref s_CommLink))
            {
                //update license server with latest tokens
                PacketDialogData data = new PacketDialogData(newPacket, _selectedLicenseCustomer.Name);
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                {
                    m_selectedDirectory = Path.GetFullPath(data.PacketInfo.OutputPath);// + "\\..");
                    GeneratePacket(data.PacketInfo.Name, DetailTreeView.SelectedNode.Name, data.ExpDate, data.PacketInfo.Description, data.PacketInfo.OutputPath);
                }
            }
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
                    packetItem.Name = packet.PacketName;
                    packetItem.Text = packet.DateCreated.ToString();
                    packetItem.Tag = packet;
                    packetItem.ImageIndex = Enums.GetListViewIconIndex((packet.IsVerified ? "VERIFIED" : "UNVERIFIED"));
                    packetItem.StateImageIndex = packetItem.ImageIndex;
                    packetItem.SubItems.Add(packet.PacketName);
                    packetItem.SubItems.Add(packet.ExpiredDate.ToShortDateString());
                    packetItem.SubItems.Add(packet.UserName);
                    packetItem.SubItems.Add(packet.VerifiedBy);
                    //add to stored packet list
                    storageListView.Items.Add(packetItem);//.Clone() as ListViewItem);
                }
                storageListView.Items[storageListView.Items.Count - 1].Font = new Font(this.Font, FontStyle.Bold);
            }
            _lvManager.ResetListViewColumnSorter(LicensePacketListView);
            _lvManager.SetSortIndexColumn(LicensePacketListView.Handle, 0);
            packetToolStripComboBox.SelectedIndex = 0;
            ShowPacketListView("");
            //needed outside of begin/end update to allow clearing of LicensePacketListView
            LicensePacketListView.Sort();
        }
        #endregion

        #region Module Methods

        private bool IsModuleFiltered(ModuleProperty module, string filterType)
        {
            switch (filterType)
            {
                case "NonDefault":
                    if (!s_CommLink.IsDefaultModule((DetailPropertyGrid.SelectedObject as ProductLicenseProperty).ProductID, module.Name)) return true;
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
                if (IsModuleFiltered(lvItem.Tag as ModuleProperty, type))
                    DetailListView.Items.Insert(0, lvItem.Clone() as ListViewItem);
            }
            _lvManager.AutoResizeColumns(DetailListView);      
            DetailListView.EndUpdate();
        }
        #endregion

        #region Customer Methods
        private bool DeleteCustomerItem(ListViewItem lvItem)
        {
            string custName = lvItem.SubItems[1].Text;
            int custID = Int32.Parse(lvItem.Text, System.Globalization.NumberStyles.HexNumber);
            DialogResult result = MessageBox.Show(string.Format("Please confirm deletion of customer: {0}.", custName), "Delete Customer Confirmation",
                          MessageBoxButtons.OKCancel, MessageBoxIcon.Exclamation);
            if (result == DialogResult.OK)
            {
                DeleteCustomerEntity(custID, custName);
                return true;
            }
            return false;
        }
        private void DeleteCustomerEntity(int custID, string custName)
        {
            Service<ICreator>.Use((client) =>
            {
                IList<LicenseTable> ltList = client.GetLicensesByCustomer(custName);
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
                        licenseName = "";
                        if (license != null)
                            licenseName = license.LicenseName;
                        item.SubItems.Add(licenseName);
                        item.SubItems.Add(((TokenStatus)tokens[index].TokenStatus).ToString());                        
                        string activatedDate = (tokens[index].ActivatedDate.HasValue) ? tokens[index].ActivatedDate.Value.ToString() : "";
                        string deactivatedDate = (tokens[index].DeactivatedDate.HasValue) ? tokens[index].DeactivatedDate.Value.ToString() : "";
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
            navigateBackToolStripButton.Enabled = true;
            seekKeyToolStripButton.Enabled = true;
            reserveHardwareKeyToolStripButton.Enabled = m_Permissions.pt_create_modify_key.Value;
            searchToolStripTextBox.Visible = false;
            SearchToolStripLabel.Visible = false;
            searchToolStripTextBox.Enabled = false;
            SearchToolStripLabel.Enabled = false;
            deactivateHardwareKeyToolStripButton.Enabled = false;
            activateHardwareKeyToolStripButton.Enabled = false;
            if (HardwareKeyListView.SelectedItems.Count > 0)
            {
                deactivateHardwareKeyToolStripButton.Enabled = m_Permissions.pt_create_modify_key.Value;
                if (HardwareKeyListView.SelectedItems[0].SubItems[2].Text != "Active" && 
                    HardwareKeyListView.SelectedItems[0].SubItems[2].Text != "Lost")
                    activateHardwareKeyToolStripButton.Enabled = m_Permissions.pt_create_modify_key.Value;
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
            navigateBackToolStripButton.Enabled = false;
            navigateForwardToolStripButton.Enabled = false;
            seekKeyToolStripButton.Enabled = true;
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
                if (selectedObject is LicenseServerProperty)
                {
                    if (!DetailListView.Columns.ContainsKey("Status"))
                    {
                        DetailListView.Columns.Clear();
                        DetailListView.Columns.Add("Type", "Type");
                        DetailListView.Columns.Add("Value", "Value");
                        DetailListView.Columns.Add("Status", "Status");
                    }
                }
                else if (selectedObject is ProductCollectionProperty)
                {
                    if (!DetailListView.Columns.ContainsKey("Product Connections"))
                    {
                        DetailListView.Columns.Clear();
                        DetailListView.Columns.Add("Name", "Name");
                        DetailListView.Columns.Add("Units", "Units");
                        DetailListView.Columns.Add("Product Connections", "Product Connections");
                    }
                }
                else
                {
                    if (!DetailListView.Columns.ContainsKey("Available"))
                    {
                        DetailListView.Columns.Clear();
                        DetailListView.Columns.Add("Name", "Name");
                        DetailListView.Columns.Add("Units", "Units");
                        DetailListView.Columns.Add("Available", "Available");
                    }
                }
                _lvManager.ResetListViewColumnSorter(DetailListView);
            }
            /// <summary>
            /// Populates the ListView column headers based upon the selected TreeNode of the License view.
            /// </summary>
            public void PopulateDetailListView(Object selectedObject)
            {
                if (selectedObject is LicenseServerProperty)
                    PopulateDetailListView(selectedObject as LicenseServerProperty);
                else if (selectedObject is ProductCollectionProperty)
                    PopulateDetailListView(selectedObject as ProductCollectionProperty);
                else if(selectedObject is ProductLicenseProperty)
                    PopulateDetailListView(selectedObject as ProductLicenseProperty);
            }
            public void PopulateDetailListView(LicenseServerProperty licenseData)
            {
                IList<TokenTable> tokenList = null;
                IList<SoftwareTokenTable> swTokenList = null;
                DetailListView.Items.Clear();
                Image tokenImage = ListViewImageList.Images[Enums.GetListViewIconIndex("VALIDATIONTOKENS")];
                ImageToolStripLabel.Size = new Size(tokenImage.Width, tokenImage.Height);
                ImageToolStripLabel.Image = tokenImage;
                DetailListViewToolStripLabel.Text = "Validation Tokens";
                DetailListViewToolStripLabel2.Visible = false;
                //enable/disable buttons
                dlvAddToolStripButton.Visible = true;
                moduleFilterToolStripComboBox.Visible = false;
                ListViewItem lvItem;        
                TokenTable hardwareToken = null;
               // retrieve tokens from database not license info object
                Service<ICreator>.Use((client) =>
                {
                    tokenList = client.GetTokensByLicenseName(licenseData.Name);
                    swTokenList = client.GetAllSoftwareTokens();
                });
                if (tokenList != null)
                {             
                    foreach(TokenTable token in tokenList)
                    {
                        if (tokenList.Count > 1)
                        {
                            string tokenName = Enum.GetName(typeof(Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType), token.TokenType);
                            SoftwareTokenTable st = swTokenList.First(t => t.TokenType == tokenName);
                            if (st.Status == 0)
                                continue;
                        }
                        lvItem = new ListViewItem();
                        ValidationProperty vToken = new ValidationProperty(token);
                        lvItem.Tag = vToken;
                        if (vToken.TokenType.Equals(Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID))
                        {
                            Service<ICreator>.Use((client) =>
                            {
                                hardwareToken = client.GetTokenByLicenseName(licenseData.Name, (byte)vToken.TokenType);
                                if (hardwareToken == null)
                                    MessageBox.Show(string.Format("Failed to get token status for {0}", vToken.TokenValue), "Load Validation Token List");
                                else
                                    (lvItem.Tag as ValidationProperty).Status = ((TokenStatus)hardwareToken.TokenStatus).ToString();
                            });
                        }
                        lvItem.Name = lvItem.Text = vToken.TokenType.ToString();
                        lvItem.ImageIndex = Enums.GetListViewIconIndex(lvItem.Text);
                        lvItem.SubItems.Add(vToken.TokenValue);
                        lvItem.SubItems.Add(vToken.Status);
                        DetailListView.Items.Add(lvItem);
                    }
                    dlvAddToolStripButton.Enabled = tokenList.Count <= 0;
                    dlvRemoveToolStripButton.Enabled = tokenList.Count > 0 && hardwareToken == null;              
                } 
                _lvManager.AutoResizeColumns(DetailListView);
            }
            public void PopulateDetailListView(ProductCollectionProperty productData)
            {
                DetailListView.Items.Clear();
                Image productImage = DetailTreeViewImageList.Images[Enums.GetDetailTreeViewIconIndex(s_CommLink.GetProductBaseName((byte)productData.ProductID))];
                ImageToolStripLabel.Size = new Size(productImage.Width, productImage.Height);
                ImageToolStripLabel.Image = productImage;
                DetailListViewToolStripLabel.Text = string.Format("{0} Modules", s_CommLink.GetProductName(productData.ProductID));
                DetailListViewToolStripLabel2.Visible = true;
                dlvAddToolStripButton.Visible = false;
                dlvRemoveToolStripButton.Visible = false;
                moduleFilterToolStripComboBox.Visible = false;
                //needs to skip any product,modules that are deactivated            
                LicenseServerProperty lsp = DetailTreeView.SelectedNode.Parent.Tag as LicenseServerProperty;
                Service<ICreator>.Use((client) =>
                {
                    ProductLicenseTable plt = client.GetProductLicense(productData.ProductCollection[0].ID);
                    LicenseVersion version = new LicenseVersion(plt.ProductVersion);
                    DetailListViewToolStripLabel2.Text = string.Format("Product Version : {0}", version.ToString());
                    List<ModuleTable> moduleList = client.GetAllActiveModulesByProduct(lsp.Name, productData.ProductID);
                    Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec = CreatorForm.s_CommLink.GetProductSpec(productData.ProductID);
                    List<ListViewItem> lvItems = new List<ListViewItem>();
                    foreach (Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs moduleSpec in productSpec.moduleSpecMap.TVal.Values)
                    {
                        bool bDeprecated = false;
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
                        if (!bDeprecated)
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
                            lvItem.ImageIndex = Enums.GetListViewIconIndex("MODULE");
                            if (s_CommLink.IsDefaultModule(productSpec.productID.TVal, (short)moduleSpec.moduleID.TVal))
                                lvItem.SubItems.Add("*");
                            else
                                lvItem.SubItems.Add(modValue.ToString());
                            lvItem.SubItems.Add(modAppInstance.ToString());
                            lvItems.Add(lvItem);
                        }
                    }
                    DetailListView.Items.AddRange(lvItems.ToArray());
                });
            }
            private void PopulateDetailListView(ProductLicenseProperty plData)
            {
                //clear previous items from storage and current view
                DetailListView.Items.Clear();
                storageListView.Items.Clear();
                //test-dev image missing
                Image productImage = DetailTreeViewImageList.Images[Enums.GetDetailTreeViewIconIndex(s_CommLink.GetProductBaseName(plData.ProductName))];
                ImageToolStripLabel.Size = new Size(productImage.Width, productImage.Height);
                ImageToolStripLabel.Image = productImage;
                DetailListViewToolStripLabel.Text = string.Format("{0} Modules", plData.ProductName);
                DetailListViewToolStripLabel2.Visible = false;
                dlvAddToolStripButton.Visible = false;
                dlvAddToolStripButton.Enabled = true;
                dlvRemoveToolStripButton.Visible = false;
                moduleFilterToolStripComboBox.Visible = true;
                //need to set plData read-only attributes
                Service<ICreator>.Use((client) =>
                {   //gets all modules for a given product license and add-on product licenses
                    string productLicense = (plData.ParentID == null) ? plData.ID : plData.ParentID;
                    List<ModuleTable> moduleList = client.GetAllModules(productLicense);
                    foreach (ModuleTable module in moduleList.Where(c => c.ProductLicenseID == plData.ProductLicenseDatabaseID))
                    {   //totalValue = licensed module value + add-on module value;
                        //           = trial module value;
                        short totalValue = 0;
                        if (plData.Status == ProductLicenseState.Trial)
                            totalValue = module.Value;
                        else
                        {
                            foreach (ModuleTable mod in moduleList)
                            {
                                if (mod.ModID == module.ModID)
                                    totalValue += mod.Value;
                            }
                        }
                        ListViewItem lvItem = CreateModuleListViewItem(plData, module, totalValue);
                        if (storageListView.Items.Find(lvItem.Name, false).Count() == 0)
                            storageListView.Items.Add(lvItem);
                    }
                });
                if (moduleFilterToolStripComboBox.SelectedItem != null)               
                    ShowModuleListView(moduleFilterToolStripComboBox.Text);
                else
                {
                    moduleFilterToolStripComboBox.SelectedIndex = 0;
                    ShowModuleListView("All");
                }
            }

            public ListViewItem CreateModuleListViewItem(ProductLicenseProperty productLicense, ModuleTable module, short totalValue)
            {
                string moduleName = s_CommLink.GetModuleName(productLicense.ProductID, module.ModID);
                ListViewItem lvItem = new ListViewItem();
                ModuleProperty newModule = new ModuleProperty(module);
                lvItem.Tag = newModule;
                newModule.ProductID = productLicense.ProductID;
                lvItem.Name = lvItem.Text = moduleName;
                lvItem.ImageIndex = Enums.GetListViewIconIndex("MODULE");
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
                    int maxValue = s_CommLink.GetMaxModuleValue(productLicense.ProductID, module.ModID);
                    int units = maxValue - totalValue;
                    lvItem.SubItems.Add(units.ToString());
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
            IList<TransactionTable> transactions = null;
            IList<ProductLicenseTable> productLicenses = null;
            ProductLicenseTable pl = null;
            string productLicenseName = "";
            storageListView.Items.Clear();
            ListViewItem[] lvItems;
            int index = 0;
            Service<ICreator>.Use((client) =>
            {
                //now two steps, given a license name, get all transactions for a license and sort by packet
                transactions = client.GetNewTransactionsByLicenseName(licenseName).OrderByDescending(c => c.taDateCreated).ToList();
                productLicenses = client.GetProductLicenses(licenseName);                
                if (transactions.Count > 0)
                {
                    lvItems = new ListViewItem[transactions.Count];
                    for (index = 0; index < transactions.Count; index++)
                    {
                        pl = productLicenses.Where(c => c.ID == transactions[index].taOrderID).FirstOrDefault();
                        productLicenseName = (pl != null) ? pl.plID : "";
                        ListViewItem item = new ListViewItem();
                        item.Tag = transactions[index];
                        item.ForeColor = System.Drawing.Color.SteelBlue;
                        item.Text = transactions[index].taDateCreated.ToString();
                        item.SubItems.Add(Enum.GetName(typeof(TransactionType), transactions[index].taType));
                        item.SubItems.Add(transactions[index].taDescription);
                        item.SubItems.Add(transactions[index].taUnits);
                        item.SubItems.Add(transactions[index].taPreviousValue);
                        item.SubItems.Add(productLicenseName);
                        item.SubItems.Add(transactions[index].taUser);
                        item.SubItems.Add("");
                        lvItems[index] = item;
                    }
                    storageListView.Items.AddRange(lvItems);
                }
                packets = client.GetPacketsByLicenseName(licenseName).OrderByDescending(c => c.DateCreated).ToList();
                foreach (var packet in packets)
                {
                    transactions = client.GetTransactionsByPacketID(packet.ID).OrderByDescending(c => c.taDateCreated).ToList(); ;
                    if (transactions.Count > 0)
                    {
                        lvItems = new ListViewItem[transactions.Count];
                        for (index = 0; index < transactions.Count; index++)
                        {
                            pl = productLicenses.Where(c => c.ID == transactions[index].taOrderID).FirstOrDefault();
                            productLicenseName = (pl != null) ? pl.plID : "";
                            ListViewItem item = new ListViewItem();
                            item.Tag = transactions[index];
                            item.Text = transactions[index].taDateCreated.ToString();
                            item.SubItems.Add(Enum.GetName(typeof(TransactionType), transactions[index].taType));
                            item.SubItems.Add(transactions[index].taDescription);
                            item.SubItems.Add(transactions[index].taUnits);
                            item.SubItems.Add(transactions[index].taPreviousValue);
                            item.SubItems.Add(productLicenseName);
                            item.SubItems.Add(transactions[index].taUser);
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

        private void LoadProductLicenseReport(ReportProperty rp)
        {
            IList<ProductLicenseTable> licenses = null;
            IList<CustomerTable> customers = null;
            CustomerTable customer = null;
            string[] plSplit;
            Service<ICreator>.Use((client) =>
            {                //function to make values database friendly?
                licenses = client.GetProductLicensesByConditions(rp.DatabaseConditions, rp.MatchAll);
                customers = client.GetAllCustomers("", false);
            });
            PopulateReportListViewColumns(ReportProperty.ReportType.ProductLicense);
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
                    newItem.SubItems.Add(((ProductLicenseState)(licenses[index].plState)).ToString());
                    if (licenses[index].ExpirationDate.HasValue)
                        newItem.SubItems.Add(licenses[index].ExpirationDate.Value.ToShortDateString());
                    else
                        newItem.SubItems.Add("");
                    newItem.SubItems.Add(licenses[index].Extensions.ToString());
                    newItem.SubItems.Add(licenses[index].Activations.ToString());
                    newItem.SubItems.Add(licenses[index].ActivationAmount.ToString());
                    newItem.SubItems.Add(licenses[index].IsActive.ToString());
                    newItem.SubItems.Add(licenses[index].Description);
                    lvItems[index] = newItem;
                }
                ReportListView.Items.AddRange(lvItems);
            }
            _lvManager.AutoResizeColumns(ReportListView);
            ReportListView.EndUpdate();
        }

        private void LoadLicenseServerReport(ReportProperty rp)
        {
            IList<LicenseTable> licenses = null;
            IList<CustomerTable> customers = null;
            CustomerTable customer = null;
            string[] plSplit;
            Service<ICreator>.Use((client) =>
            {
                licenses = client.GetLicensesByConditions(rp.DatabaseConditions, rp.MatchAll);
                customers = client.GetAllCustomers("", false);
            });
            PopulateReportListViewColumns(ReportProperty.ReportType.LicenseServer);
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
                    newItem.SubItems.Add(licenses[index].LicenseComments);
                    lvItems[index] = newItem;
                }
                ReportListView.Items.AddRange(lvItems);
            }
            _lvManager.AutoResizeColumns(ReportListView);
            ReportListView.EndUpdate();
        }

        private void LoadHardwareTokenReport(ReportProperty rp)
        {
            IList<TokenTable> tokens = null;
            IList<CustomerTable> customers = null;
            LicenseTable licRecord = null;            
            CustomerTable customer = null;
            PopulateReportListViewColumns(ReportProperty.ReportType.HardwareToken);
            ReportListView.BeginUpdate();
            ReportListView.Items.Clear();            
            Service<ICreator>.Use((client) =>
            {
                tokens = client.GetHardwareTokensByConditions(rp.DatabaseConditions, rp.MatchAll);
                customers = client.GetAllCustomers("", false);
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
                        string licID = (licRecord != null) ? licRecord.LicenseName : "";
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

        private void LoadReportNode(TreeNode node)
        {
            ReportProperty rp = node.Tag as ReportProperty;
            if (rp != null)
            {
                if (rp.Type == ReportProperty.ReportType.LicenseServer)
                    LoadLicenseServerReport(rp);
                else if (rp.Type == ReportProperty.ReportType.ProductLicense)
                    LoadProductLicenseReport(rp);
                else
                    LoadHardwareTokenReport(rp);
            }
            else
                ReportListView.Items.Clear();
        }
        /// <summary>
        /// Populates the ListView column headers based upon the selected TreeNode of the License view.
        /// </summary>
        public void PopulateReportListViewColumns(ReportProperty.ReportType _reportType)
        {
            ReportListView.Columns.Clear();
            if (_reportType == ReportProperty.ReportType.ProductLicense)
            {
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
                ReportListView.Columns.Add("Notes");
            }
            else if (_reportType == ReportProperty.ReportType.LicenseServer)
            {
                ReportListView.Columns.Add("Customer");
                ReportListView.Columns.Add("License Server");
                ReportListView.Columns.Add("Active");
                ReportListView.Columns.Add("Notes");
            }
            else
            {
                ReportListView.Columns.Add("Customer");
                ReportListView.Columns.Add("Hardware ID");
                ReportListView.Columns.Add("License Server");
                ReportListView.Columns.Add("Status");
                ReportListView.Columns.Add("Activated Date");
                ReportListView.Columns.Add("Deactivated Date");
            }
            _lvManager.ResetListViewColumnSorter(ReportListView);
        }
        //read xml for reports, load their conditions into report node tags
        private void LoadReportListView()
        {
            List<ReportProperty> reportList = _reportManager.GetReports();
            ////populate tree node
            TreeNode reportNode = null;
            foreach (TreeNode node in reportsTreeView.Nodes)
            {
                node.Nodes.Clear();
                if (node.Name == "RootNode")
                {
                    foreach (ReportProperty report in reportList)
                    {
                        reportNode = new TreeNode();
                        reportNode.Text = report.ID;
                        reportNode.Tag = report;
                        node.Nodes.Add(reportNode);
                    }
                }
            }
            reportsTreeView.SelectedNode = reportsTreeView.Nodes[0];
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
            /*customerToolStripLabel.Text =*/ customerToolStripStatusLabel.Text = "";
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
            using (BackgroundWorker worker = new BackgroundWorker())
            {
                worker.DoWork += ((sender, e) => data = GetDBLicenses(worker, destName, searchString, loadOption));
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
            return licenses;
        }
        private void OnDBLicensesRetrieved(IList<LicenseTable> data, string selectedName)
        {
            if (data != null)
            {
                LoadLicenseView(data, selectedName);
            }
            if (searchToolStripTextBox.ForeColor == SystemColors.InactiveCaptionText)
                searchToolStripTextBox.Text = "License Server Filter";
            loadingCircle1.Active = false;
            loadingCircle1.Visible = false;
            customerToolStripStatusLabel.Text = "Customer : " + _selectedLicenseCustomer.Name;
            //customerToolStripLabel.Text = _selectedLicenseCustomer.Name;
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
                LicensePacket packet = new LicensePacket(licenseName);
                packet.BuildLicensePackage();
                //strip license of deactivated product licenses
                client.GenerateLicensePacket(packetName,
                                             packet.DatabaseRecord,
                                             expDate,
                                             description,
                                             ref verificationCode,
                                             ref newByteArrayLicensePacket,
                                             WindowsIdentity.GetCurrent().Name);
                packet.SaveLicensePackage();               
                PacketTable storedPacket = client.GetPacketByVerificationCode(verificationCode);
                if (storedPacket == null)
                    return;
                
                //retrieve all transactions for a given license that doesnt have a packet id
                //determine if any transactions are exp date, amount in days, or activations
                //update extension count
                //create transaction with type extension string Expires x/x/x x Activations/x Days value = Extension x
                IList<TransactionTable> transactionList = client.GetNewTransactionsByLicenseName(licenseName);
                List<int> extensionList = new List<int>();
                //need to ignore deactivated pl transactions
                foreach (TransactionTable recordedTransaction in transactionList)
                {
                    if((recordedTransaction.taType == (byte)TransactionType.ExpirationDate) ||
                       (recordedTransaction.taType == (byte)TransactionType.ActivationTotal) ||
                       (recordedTransaction.taType == (byte)TransactionType.ActivationAmount)) 
                    {
                        if(!extensionList.Contains(recordedTransaction.taOrderID.Value))
                            extensionList.Add(recordedTransaction.taOrderID.Value);
                    }
                    recordedTransaction.taPacketID = storedPacket.ID;
                    client.UpdateTransaction(recordedTransaction);
                }
                //if yes pull product license, retrieve current values                 
                string txDescription = "";
                foreach (int plID in extensionList)
                {
                    ProductLicenseTable plt = client.GetProductLicenseByID(plID);
                    if (plt != null)
                    {
                        plt.Extensions += 1;
                        if (plt.Activations == 0)
                            txDescription = string.Format("Expires - {0}", plt.ExpirationDate.Value.ToShortDateString());
                        else
                            txDescription = string.Format("({0} Activations/{1} Days) Expires - {2}", plt.Activations, plt.ActivationAmount, plt.ExpirationDate.Value.ToShortDateString());
                        //add extension entry with summary of the extension
                        TransactionManager.CreateTransaction(TransactionType.Extension,
                                          licenseName,
                                          plt.plID,
                                          txDescription,
                                          string.Format("Extension {0}", plt.Extensions),
                                          "");
                        //bump up extension count
                        client.UpdateProductLicense(plt);
                    }
                }
                transactionList = client.GetNewTransactionsByLicenseName(licenseName);
                foreach (TransactionTable recordedTransaction in transactionList)
                {
                    if ((recordedTransaction.taType == (byte)TransactionType.Extension))
                    {
                        recordedTransaction.taPacketID = storedPacket.ID;
                        client.UpdateTransaction(recordedTransaction);
                    }
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
            SetLicenseServerState(DetailTreeView.SelectedNode, false);
        }
        #endregion

        
        private void cloneToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //only display on product licenses
            //get product license information from currently selected tag
            //launch pl dialog? disallow modification
        }

        private void findSearchString(string findString)
        {
            //determine if the string is a license server or product license
            //
            bool bFound = false;
            string[] findStringArray;
            findStringArray = findString.Split("-".ToCharArray());
            //valid search string
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
                    ProductLicenseTable plt = null;
                    //pull from database 
                    Service<ICreator>.Use((client) =>
                    {
                        plt = client.GetProductLicense(findToolStripTextBox.Text);
                    });
                    if (plt != null)
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
                }
            }
            if (!bFound)
            {
                MessageBox.Show("Failed to find " + findToolStripTextBox.Text, "Find", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void deactivateToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult result = MessageBox.Show(string.Format("Please confirm deactivation for {0}.", DetailTreeView.SelectedNode.Name), "Deactivate Confirmation",
                                      MessageBoxButtons.OKCancel, MessageBoxIcon.Exclamation);
            if (result == DialogResult.OK)
            {
                if (DetailTreeView.SelectedNode.Tag is LicenseServerProperty)
                {
                    LicenseServerProperty selectedLicense = DetailTreeView.SelectedNode.Tag as LicenseServerProperty;
                    //verify delete is allowed.            
                    Service<ICreator>.Use((client) =>
                    {
                        LicenseServerProperty lsProperty = DetailTreeView.Nodes.Find(selectedLicense.Name, true)[0].Tag as LicenseServerProperty;
                        TransactionManager.CreateTransaction(TransactionType.LicenseServer,
                                          lsProperty.Name,
                                          "",
                                          "Deactivate License Server",
                                          "",
                                          ""
                                          );
                        LicenseTable licRecord = client.GetLicenseByName(lsProperty.Name, false);
                        licRecord.IsActive = false;
                        lsProperty.IsActive = false;
                        client.UpdateLicense(licRecord, true);
                        IList<TokenTable> tokens = client.GetTokensByLicenseName(selectedLicense.Name);
                        if (tokens != null)
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
                    });
                    DetailPropertyGrid.SelectedObject = DetailTreeView.SelectedNode.Tag;
                    PopulateDetailListView(DetailPropertyGrid.SelectedObject);
                    SetLicenseServerState(DetailTreeView.SelectedNode, false);
                }
                else
                {   //propertygrid object needs to be update, treenode status needs to be updated
                    ProductLicenseProperty plProperty = DetailPropertyGrid.SelectedObject as ProductLicenseProperty;
                    Service<ICreator>.Use((client) =>
                    {   //decrement app instance and modules from product view
                        List<string> plAddOns = client.GetAddOnProductLicenses(plProperty.ID);
                        plAddOns.Add(plProperty.ID);
                        if (plAddOns.Count > 0)
                        {
                            ProductLicenseTable plt;
                            foreach (string pl in plAddOns)
                            {
                                plt = client.GetProductLicense(pl);
                                TransactionManager.CreateTransaction(TransactionType.LicenseServer,
                                                  "",
                                                  plt.plID,
                                                  "Deactivate Product License",
                                                  plt.plID,
                                                  "");
                                plt.IsActive = false;
                                client.UpdateProductLicense(plt);
                                //Deactivate product
                                LicenseTable storedLicense = client.GetLicenseByID(plt.LicenseID, false);
                                Lic_PackageAttribs licPackage = new Lic_PackageAttribs();
                                licPackage.Stream = storedLicense.LicenseInfo;
                                foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs product in licPackage.licLicenseInfoAttribs.TVal.productList.TVal)
                                {
                                    if (product.productID.TVal.Equals((uint)plt.ProductID))
                                    {
                                        //order product has same value as dbproduct
                                        //client type subtract total, client add-on subtract nothing
                                        //decrement appinstance for product when removing an order
                                        //1) non-client, non addon - decrement appinstance
                                        //2) non-client, add-on - skip
                                        //3) client, non addon - decrement all app instances
                                        //4) client, add-on - skip
                                        if (plt.plState != (byte)ProductLicenseState.AddOn)
                                        {
                                            if (s_CommLink.GetProductSpec(plt.ProductID).productLicType.TVal != Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.TProductLicenseType.pltClient)
                                                product.productAppInstance.TVal -= 1;
                                            else
                                                product.productAppInstance.TVal = 0;
                                        }
                                        break;
                                    }
                                }
                                storedLicense.LicenseInfo = licPackage.Stream;
                                client.UpdateLicense(storedLicense, true);
                            }
                        }
                    });
                    DetailTreeView.SelectedNode.Tag = DetailPropertyGrid.SelectedObject;
                    DetailPropertyGrid.Refresh();
                    LoadProductNode(DetailTreeView.SelectedNode.Parent);
                }
            }
        }

        private void reactivateToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult result = MessageBox.Show(string.Format("Please confirm reactivation for {0}.", DetailTreeView.SelectedNode.Name), "Reactivate Confirmation",
                                      MessageBoxButtons.OKCancel, MessageBoxIcon.Exclamation);
            if (result == DialogResult.OK)
            {
                if (DetailTreeView.SelectedNode.Tag is LicenseServerProperty)
                {
                    LicenseServerProperty selectedLicense = DetailTreeView.SelectedNode.Tag as LicenseServerProperty;
                    //verify delete is allowed.            
                    Service<ICreator>.Use((client) =>
                    {
                        LicenseServerProperty lsProperty = DetailTreeView.Nodes.Find(selectedLicense.Name, true)[0].Tag as LicenseServerProperty;
                        TransactionManager.CreateTransaction(TransactionType.LicenseServer,
                                          lsProperty.Name,
                                          "",
                                          "Reactivate License Server",
                                          "",
                                          ""
                                          );
                        LicenseTable licRecord = client.GetLicenseByName(lsProperty.Name, false);
                        licRecord.IsActive = true;
                        lsProperty.IsActive = true;
                        client.UpdateLicense(licRecord, true);
                        IList<TokenTable> tokens = client.GetTokensByLicenseName(selectedLicense.Name);
                        if (tokens != null)
                        {
                            foreach (TokenTable token in tokens)
                            {
                                if (token.TokenType == (byte)Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID)
                                {
                                    token.LicenseID = licRecord.ID;
                                    token.TokenStatus = (byte)TokenStatus.Active;
                                    client.UpdateToken(token);
                                }
                                //Discuss whether to keep tokens around
                            }
                        }
                    });
                    DetailPropertyGrid.SelectedObject = DetailTreeView.SelectedNode.Tag;
                    PopulateDetailListView(DetailPropertyGrid.SelectedObject);
                    SetLicenseServerState(DetailTreeView.SelectedNode, false);
                }
            }
        }

        private void markLostToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult result = MessageBox.Show(string.Format("Please confirm lost hardware token for {0}.", DetailTreeView.SelectedNode.Name), "Lost Confirmation",
                          MessageBoxButtons.OKCancel, MessageBoxIcon.Exclamation);
            if (result == DialogResult.OK)
            {
                if (DetailTreeView.SelectedNode.Tag is LicenseServerProperty)
                {
                    LicenseServerProperty selectedLicense = DetailTreeView.SelectedNode.Tag as LicenseServerProperty;
                    //verify delete is allowed.            
                    Service<ICreator>.Use((client) =>
                    {
                        LicenseServerProperty lsProperty = DetailTreeView.Nodes.Find(selectedLicense.Name, true)[0].Tag as LicenseServerProperty;
                        LicenseTable licRecord = client.GetLicenseByName(lsProperty.Name, false);
                        licRecord.IsActive = false;
                        lsProperty.IsActive = false;
                        client.UpdateLicense(licRecord, true);
                        IList<TokenTable> tokens = client.GetTokensByLicenseName(selectedLicense.Name);
                        string lostTokenName = "";
                        if (tokens != null)
                        {
                            foreach (TokenTable token in tokens)
                            {
                                if (token.TokenType == (byte)Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID)
                                {
                                    token.TokenStatus = (byte)TokenStatus.Lost;
                                    client.UpdateToken(token);
                                    lostTokenName = token.TokenValue;
                                }
                            }
                        }
                        TransactionManager.CreateTransaction(TransactionType.LicenseServer,
                                          lsProperty.Name,
                                          "",
                                          "Lost Hardware Token",
                                          lostTokenName,
                                          ""
                                          );
                    });
                    DetailPropertyGrid.SelectedObject = DetailTreeView.SelectedNode.Tag;
                    SetLicenseServerState(DetailTreeView.SelectedNode, false);
                    PopulateDetailListView(DetailPropertyGrid.SelectedObject);
                }
            }
        }

        private void newTokenToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (DetailPropertyGrid.SelectedObject is LicenseServerProperty)
                CreateValidationToken(DetailPropertyGrid.SelectedObject as LicenseServerProperty);
        }

        private void clearToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (DetailPropertyGrid.SelectedObject is LicenseServerProperty)   //token remove
                RemoveValidationToken(DetailPropertyGrid.SelectedObject as LicenseServerProperty);
        }

        private void ReportTreeViewContextMenuStrip_Opening(object sender, CancelEventArgs e)
        {
            bool isReportSelected = ((reportsTreeView.SelectedNode.Tag as ReportProperty)!= null);
            editReportToolStripMenuItem.Enabled = isReportSelected;
            deleteReportToolStripMenuItem.Enabled = isReportSelected;
            exportReportToolStripMenuItem.Enabled = isReportSelected;
            copyReportToolStripMenuItem.Enabled = isReportSelected;
            pasteReportToolStripMenuItem.Visible = false;   
            if (_copyNode != null)
                if (_copyNode.Tag is ReportProperty)
                    pasteReportToolStripMenuItem.Visible = true;                       
            renameReportToolStripMenuItem.Enabled = isReportSelected;
        }

        private void hardwareTokenReportToolStripMenuItem_Click(object sender, EventArgs e)
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
                data.Report.Type = ReportProperty.ReportType.HardwareToken;
                if (dlg.ShowDialog(this, data) == DialogResult.OK)
                {
                    TreeNode newReportNode = new TreeNode();
                    newReportNode.Text = data.Report.ID;
                    newReportNode.Tag = data.Report;
                    reportsTreeView.Nodes[0].Nodes.Add(newReportNode);
                    reportsTreeView.SelectedNode = newReportNode;
                    reportsTreeView.SelectedNode.BeginEdit();
                }
            }
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

        private void renameReportToolStripMenuItem_Click(object sender, EventArgs e)
        {
            reportsTreeView.SelectedNode.BeginEdit();
        }

        private void pasteReportToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //create new report based off saved report 
            //set new report to be edited
            //option will only be available if a reportnode has been set to copy
            ReportProperty copiedReport = _copyNode.Tag as ReportProperty;
            ReportProperty newReport = new ReportProperty(s_CommLink);
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

        private void incrementToolStripMenuItem_Click(object sender, EventArgs e)
        {
            IncrementSelectedModules();
        }

        private void decrementToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DecrementSelectedModules();
        }

        private void dlvEditToolStripMenuItem_DropDownOpening(object sender, EventArgs e)
        {
            if (DetailListView.SelectedItems.Count > 0)
            {
                setModuleToolStripTextBox.Text = (DetailListView.SelectedItems[0].Tag as ModuleProperty).Value.ToString();
            }
        }

        private void setModuleToolStripTextBox_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                if(DetailListView.SelectedItems.Count > 0)
                {
                    ushort oldValue, newValue;
                    ListViewItem selectedItem = DetailListView.SelectedItems[0];
                    ModuleProperty module = selectedItem.Tag as ModuleProperty;
                    oldValue = (ushort)module.Value;
                    ProductLicenseProperty plp = DetailPropertyGrid.SelectedObject as ProductLicenseProperty;
                    string errorMsg = "";
                    if (s_CommLink.IsDefaultModule(plp.ProductID, module.ID))
                        errorMsg = "Default modules can not be modified.";
                    else if (!ushort.TryParse(setModuleToolStripTextBox.Text, out newValue))
                        errorMsg = "Invalid format for {0} value";
                    else
                    {
                        if (oldValue != newValue)
                        {
                            if (newValue > module.MaxValue)
                                errorMsg = "Specified value exceeds maximum allowed for {0}";
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
                    if(errorMsg.Length > 0)
                    {
                        module.Value = (short)oldValue;
                        MessageBox.Show(string.Format(errorMsg, module.Name), "Set Module Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        setModuleToolStripTextBox.SelectAll();
                    }
                }
            }
        }

        private void SetModuleState(ProductLicenseProperty plp, ListViewItem lvItem)
        {
            ModuleProperty module = lvItem.Tag as ModuleProperty;
            lvItem.ForeColor = this.ForeColor;
            lvItem.Font = this.Font;
            if (plp.ExpirationDate.HasValue && plp.ExpirationDate.Value.CompareTo(new DateTime(1900, 1, 1)) != 0)
            {
                if (plp.ExpirationDate.Value.ToLocalTime().CompareTo(CurrentExpirationDate) < 0)
                    lvItem.ForeColor = System.Drawing.Color.Red;
            }
            else
            {                    //permanent - blue
                if (plp.Status == ProductLicenseState.Licensed)
                    lvItem.ForeColor = System.Drawing.Color.SteelBlue;
            }
            if (module.Value == 0 && module.AppInstance == 0)
            {
                lvItem.ForeColor = System.Drawing.Color.SlateGray;
                lvItem.Font = new System.Drawing.Font(lvItem.Font, System.Drawing.FontStyle.Italic);
            }
        }

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
    }
}
