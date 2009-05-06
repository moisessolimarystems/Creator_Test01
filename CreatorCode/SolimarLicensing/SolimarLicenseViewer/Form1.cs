using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using Microsoft.Win32;
using SolimarLicenseViewer.Properties;
using System.IO;

namespace SolimarLicenseViewer
{
    public enum ViewType { LICENSE, USAGE };

    public partial class Form1 : Form
    {
        #region Constructors
        public Form1()
        {
            InitializeComponent();
            //Disable form items
            EnableFormItems(false);
            //create connection to license wrapper
            m_CommLink = new CommunicationLink();
            //List to hold server names
            m_ServerList = new List<String>();
        }
        #endregion

        #region Form Methods
        private void PopulateAllViews()
        {
            Cursor.Current = Cursors.WaitCursor;
            PopulateTreeView();
            Cursor.Current = Cursors.Default;
        }

        private void PopulateTreeView()
        {
            Cursor.Current = Cursors.WaitCursor;
            this.treeView.BeginUpdate();
            if (myType == ViewType.LICENSE)
            {
                viewTypeToolStripMenuItem.Text = AppConstants.LicenseView;
                m_treeViewMgr.PopulateLicenseView();
            }
            else
            {
                viewTypeToolStripMenuItem.Text = AppConstants.UsageView;
                m_treeViewMgr.PopulateUsageView();
            }
            if (m_listViewMgr.SelectedNode != null)
            {
                TreeNode[] foundNodes = this.treeView.Nodes.Find(m_listViewMgr.SelectedNode.Name, true);
                if (foundNodes.Length > 0)
                    this.treeView.SelectedNode = foundNodes[0];
            }
            else if (this.treeView.Nodes.Count > 0)
                this.treeView.SelectedNode = this.treeView.Nodes[0];
            this.treeView.EndUpdate();
            Cursor.Current = Cursors.Default;
        }

        private void PopulateListView()
        {
            System.Diagnostics.Trace.WriteLine("PopulateListView() - Start");
            Cursor.Current = Cursors.WaitCursor;
            this.noFlickerListView.BeginUpdate();
            if (myType == ViewType.LICENSE)
            {
                m_listViewMgr.PopulateLicenseView();
            }
            else        //ViewType.Usage
            {
                m_listViewMgr.PopulateUsageView();
            }
            this.noFlickerListView.EndUpdate();
            Cursor.Current = Cursors.Default;
            System.Diagnostics.Trace.WriteLine("PopulateListView() - End");
        }

        private void InitializeView(ViewType type)
        {
            this.myType = type;
            EnableViewMenuItemCheckedState(this.myType);
            m_listViewMgr.SelectedNode = null;
            PopulateAllViews();
        }

        private void DisconnectServer()
        {
            Cursor.Current = Cursors.WaitCursor;
            this.treeView.BeginUpdate();
            this.treeView.Nodes.Clear();
            this.treeView.EndUpdate();

            this.noFlickerListView.BeginUpdate();
            this.noFlickerListView.Items.Clear();
            this.noFlickerListView.EndUpdate();

            m_CommLink.Disconnect();
            EnableFormItems(false);
            this.ConnectionStatusLabel.Text = "Disconnected";
            this.Text = AppConstants.FormTitle + " [Disconnected]";
            Cursor.Current = Cursors.Default;
        }
        private int ConnectServer()
        {
            //using keyword forces scope onto object, so it will be disposed after it is done.
            DisconnectServer();
            using (ConnectDialog cd = new ConnectDialog(m_CommLink))
            {
                ConnectDialogData data = new ConnectDialogData(m_ServerList.ToArray());
                if (cd.ShowDialog(this, data) == DialogResult.OK)
                {
                    try
                    {
                        //create treeview manager
                        m_listViewMgr = new ListViewMgr(this.infoSplitContainer, this.noFlickerListView, this.bottomNoFlickerListView, m_CommLink);
                        //create listview manager
                        m_treeViewMgr = new TreeViewMgr(this.treeView, m_CommLink);
                        //fill listview/treeview
                        PopulateAllViews();
                        //Enable Menu Items
                        EnableFormItems(true);
                        //Update Status
                        ConnectionStatusLabel.Text = String.Empty;
                        this.Text = AppConstants.FormTitle + " [" + m_CommLink.ServerName + "]";
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

        private void EnableFormItems(bool status)
        {
            EnableMenuItems(status);
            EnableToolItems(status);
        }

        private void EnableMenuItems(bool status)
        {
            fileLicenseToolStripMenuItem.Enabled = status;
            licenseToolStripMenuItem.Enabled = status;
            viewToolStripMenuItem.Enabled = status;
            usageToolStripMenuItem.Enabled = status;
        }

        private void EnableToolItems(bool status)
        {
            licenseToolStripButton.Enabled = status;
            refreshToolStripButton.Enabled = status;
            viewsToolStripSplitButton.Enabled = status;
        }

        private void EnterProtectionKeyPacket()
        {
            EnterPasswordPacketDialog dialog = new EnterPasswordPacketDialog();
            dialog.SetData(m_selectedDirectory);
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    Cursor.Current = Cursors.WaitCursor;
                    string pwPacket = dialog.GetData();
                    m_selectedDirectory = System.IO.Path.GetDirectoryName(dialog.GetData());
                    string verificationCode = "";
                    m_CommLink.EnterProtectionKeyPasswordPacket(System.IO.File.ReadAllBytes(pwPacket), ref verificationCode);
                    //alert user of success
                    MessageBox.Show(this, "Applied license password successfully.  Verification Code: " + verificationCode, "Apply License Password", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    PopulateAllViews();
                }
                catch (COMException ex)
                {
                    //alert user of failure
                    HandleExceptions.DisplayException(this, ex, "Failed to apply Key Password Packet!", "Apply Key Password Packet");
                }
                finally
                {
                    Cursor.Current = Cursors.Default;
                }

            }
        }
        private void EnterProtectionKeyPassword()
        {
            EnterPasswordDialog dialog = new EnterPasswordDialog();
            dialog.SetData();
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    Cursor.Current = Cursors.WaitCursor;
                    string password = dialog.GetData();
                    if (m_CommLink.EnterProtectionKeyPassword(password))
                    {
                        //alert user of success
                        MessageBox.Show(this, "Applied license password successfully.", "Apply License Password", MessageBoxButtons.OK, MessageBoxIcon.Information);

                        PopulateAllViews();
                    }
                    else
                    {
                        throw new COMException();
                    }
                }
                catch (COMException ex)
                {
                    //alert user of failure
                    HandleExceptions.DisplayException(this, ex, "Failed to apply license password!", "Apply License Password");
                }
                finally
                {
                    Cursor.Current = Cursors.Default;
                }
            }
        }
        private void ImportLicensePacket(bool bUpdateExisting)
        {
            LicenseUpdateHelper licHelper = new LicenseUpdateHelper();
            licHelper.SetData(new LicenseUpdateHelper.LicenseUpdateHelperData(bUpdateExisting ? LicenseUpdateHelper.LicenseUpdateActionEnum.luaeUpdateCurrentLicPackage : LicenseUpdateHelper.LicenseUpdateActionEnum.luaeImportNewLicPackage, m_selectedDirectory), m_CommLink);
            if (licHelper.ShowDialog() == DialogResult.OK)
            {
                LicenseUpdateHelper.LicenseUpdateHelperData data = licHelper.GetData();
                if (data != null)
                    m_selectedDirectory = data.directory;

                PopulateAllViews();
            }
        }

        private void ExportLicensePacket()
        {
            String verificationCode = "";
            String generalStream = "";
            this.exportPktDialog.DefaultExt = "packet";
            this.exportPktDialog.Filter = "License Packet|*.packet";
            this.exportPktDialog.Title = "Export License Packet";
            this.exportPktDialog.InitialDirectory = m_selectedDirectory;
            System.Windows.Forms.DialogResult result = exportPktDialog.ShowDialog();

            if (result == DialogResult.OK)
            {
                Byte[] newByteArrayLicensePacket = null;
                try
                {
                    m_CommLink.GetSoftwareLicenseInfoForAll(ref generalStream);
                    m_CommLink.GenerateSoftwareLicPacket(generalStream, DateTime.Now.AddDays(7), ref verificationCode, ref newByteArrayLicensePacket);
                    if (newByteArrayLicensePacket != null)
                        File.WriteAllBytes(exportPktDialog.FileName, newByteArrayLicensePacket);
                }
                catch (COMException ex)
                {
                    //alert user of failure
                    HandleExceptions.DisplayException(this, ex, "Failed to export license!", "Export License Packet");
                }
            }
        }

        private void EnableViewMenuItemCheckedState(ViewType myType)
        {
            if (myType == ViewType.LICENSE)
            {
                licenseToolStripMenuItem.CheckState = CheckState.Checked;
                usageToolStripMenuItem.CheckState = CheckState.Unchecked;
                licenseSplitBtnMenuItem.CheckState = CheckState.Checked;
                usageSplitBtnMenuItem.CheckState = CheckState.Unchecked;
            }
            else        //ViewType.Usage
            {
                licenseToolStripMenuItem.CheckState = CheckState.Unchecked;
                usageToolStripMenuItem.CheckState = CheckState.Checked;
                licenseSplitBtnMenuItem.CheckState = CheckState.Unchecked;
                usageSplitBtnMenuItem.CheckState = CheckState.Checked;
            }
        }

        private void General_KeyDown(object sender, KeyEventArgs e)
        {
            //Alan, you might want to move this into your ListViewMgr.cs, I just threw it in here because I needed some
            //copying behavior - Jeff 10-10-08
            if (sender is ListView)
            {
                ListView tmpListView = sender as ListView;
                if (e.KeyCode == Keys.C && e.Control)
                {
                    StringBuilder strBuilder = new StringBuilder();
                    foreach (ListViewItem selItem in tmpListView.SelectedItems)
                    {
                        if (strBuilder.Length > 0)
                            strBuilder.Append("\n");
                        //foreach (System.Windows.Forms.ListViewItem.ListViewSubItem subItem in selItem.SubItems)
                        for (int idx = 0; idx < selItem.SubItems.Count; idx++)
                        {
                            if (idx != 0)
                                strBuilder.Append(", ");
                            strBuilder.Append(selItem.SubItems[idx].Text);
                        }
                    }
                    Clipboard.SetData(DataFormats.UnicodeText, strBuilder.ToString());
                }
                else if (e.KeyCode == Keys.A)
                {
                    foreach (ListViewItem item in tmpListView.Items)
                        item.Selected = true;
                }
            }
        }
        #endregion

        #region ToolStrip Events
        private void importPacketToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ImportLicensePacket(true);
        }

        private void exportToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            ExportLicensePacket();
        }

        private void refreshToolStripButton_Click(object sender, EventArgs e)
        {
            Cursor storedCursor = this.Cursor;
            this.Cursor = Cursors.WaitCursor;
            if (treeView.SelectedNode != null)
                m_listViewMgr.SelectedNode = treeView.SelectedNode;
            PopulateAllViews();
            this.Cursor = storedCursor;
        }

        private void viewsToolStripSplitButton_ButtonClick(object sender, EventArgs e)
        {
            if (myType != ViewType.LICENSE)
            {
                InitializeView(ViewType.LICENSE);
            }
            else
            {
                InitializeView(ViewType.USAGE);
            }
        }

        private void licenseSplitBtnMenuItem_Click(object sender, EventArgs e)
        {
            if (myType != ViewType.LICENSE)
            {
                InitializeView(ViewType.LICENSE);
            }
        }

        private void usageSplitBtnMenuItem_Click(object sender, EventArgs e)
        {
            if (myType != ViewType.USAGE)
            {
                InitializeView(ViewType.USAGE);
            }
        }
        #endregion

        #region ContextMenu Events

        private void lvContextMenuStrip_Opening(object sender, CancelEventArgs e)
        {
            ListViewItem item = null;
            if (m_listViewMgr.SelectedNode.Text == AppConstants.LicenseRootNode)
            {
                // find any selected 
                if (this.noFlickerListView.SelectedItems.Count > 0)
                    item = this.noFlickerListView.SelectedItems[0];
                // user clicked in the background 
                if (item == null || item.SubItems[1].Text != AppConstants.DisasterRecoveryLicType)
                    e.Cancel = true; /* Close Context Menu */
                else
                    extendToolStripMenuItem.Enabled = m_listViewMgr.EnableDisasterRecoverExt;
            }
            else
                e.Cancel = true;    /* Close Context Menu */
        }

        private void extendToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                m_CommLink.SoftwareLicenseDisasterRecoveryExtendTimeByLicense(this.noFlickerListView.SelectedItems[0].Text);
                PopulateAllViews();
            }
            catch (COMException ex)
            {
            }
        }

        #endregion

        #region Menu Events

        #region File Menu Items
        private void ConnectMenuItem_Click(object sender, EventArgs e)
        {
            ConnectServer();
        }
        private void enterPasswordToolStripMenuItem_Click(object sender, EventArgs e)
        {
            EnterProtectionKeyPassword();
        }
        private void enterPasswordPacketToolStripMenuItem_Click(object sender, EventArgs e)
        {
            EnterProtectionKeyPacket();
        }

        private void importToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ImportLicensePacket(sender == updateLicToolStripMenuItem);
        }

        private void exportToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExportLicensePacket();
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }
        #endregion

        #region View Menu Items
        private void licenseToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (myType != ViewType.LICENSE)
            {
                InitializeView(ViewType.LICENSE);
            }
        }

        private void usageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (myType != ViewType.USAGE)
            {
                InitializeView(ViewType.USAGE);
            }
        }
        #endregion

        #region Tools Menu Items
        private void toolsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (RegistryKey rkey = Registry.LocalMachine.OpenSubKey(AppConstants.SolimarRegKey))
            {
                if (rkey.GetSubKeyNames().Length >= 1)
                    optionsToolStripMenuItem.Enabled = true;
                else
                    optionsToolStripMenuItem.Enabled = false;
                rkey.Close();
            }
        }
        private void optionsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //display settings form for product connections
            using (ProductSettingsDialog dlg = new ProductSettingsDialog(m_CommLink))
            {
                dlg.ShowDialog(this);
            }
        }
        #endregion

        #region Help Menu Items
        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (AboutBox aboutBox = new AboutBox())
            {
                int majorVer = 0;
                int minorVer = 0;
                int buildVer = 0;
                m_CommLink.GetVersionLicenseManager(ref majorVer, ref minorVer, ref buildVer);
                string licMgrVer = string.Format("{0}.{1}.{2}", majorVer, minorVer, buildVer);
                aboutBox.SetManagerVersion(licMgrVer);
                aboutBox.SetManagerName(System.Environment.MachineName.ToLower());

                try
                {
                    m_CommLink.GetVersionLicenseServer(m_CommLink.ServerName, ref majorVer, ref minorVer, ref buildVer);
                }
                catch (COMException)
                {
                    majorVer = 0;
                    minorVer = 0;
                    buildVer = 0;
                }
                string licSvrVer = string.Format("{0}.{1}.{2}", majorVer, minorVer, buildVer);
                aboutBox.SetServerVersion(licSvrVer);
                aboutBox.SetServerName(m_CommLink.ServerName);
                aboutBox.ShowDialog(this);
            }
        }
        #endregion

        #endregion

        #region Form Events

        //TODO : Need to be able to figure out how to create tree from contents
        private void treeView_AfterSelect(object sender, TreeViewEventArgs e)
        {
            m_listViewMgr.SelectedNode = this.treeView.SelectedNode;
            PopulateListView();
        }

        private void listView_ColumnClick(object sender, ColumnClickEventArgs e)
        {
            // Perform the sort with these new sort options.
            if (sender is ListView)
            {
                m_listViewMgr.SetListViewColumnSorter((sender as ListView), e.Column);
                (sender as ListView).Sort();
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // Set window location
            if (Settings.Default.WindowLocation != null)
            {
                this.Location = Settings.Default.WindowLocation;
            }

            // Set window size
            if (Settings.Default.WindowSize != null)
            {
                this.Size = Settings.Default.WindowSize;
            }

            // Set Server Connection List
            if (Settings.Default.ServerList != null)
            {
                String[] serverList = new String[Settings.Default.ServerList.Count];
                Settings.Default.ServerList.CopyTo(serverList, 0);
                this.m_ServerList.AddRange(serverList);
            }

            //Set Current ViewType
            if (Settings.Default.ViewType != null)
            {
                this.myType = (ViewType)Settings.Default.ViewType;
                EnableViewMenuItemCheckedState(this.myType);
            }

            if(Settings.Default.SelectedDirectory != null)
            {
                m_selectedDirectory = Settings.Default.SelectedDirectory;
            }
            if (m_selectedDirectory.Length == 0)
            {
                m_selectedDirectory = Environment.GetFolderPath(Environment.SpecialFolder.Desktop);
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
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
            //Copy ServerConnection List to app settings
            if (this.m_ServerList.Count > 0)
            {
                Settings.Default.ServerList.Clear();
                Settings.Default.ServerList.AddRange(this.m_ServerList.ToArray());
            }
            //Copy ViewType to app settings
            Settings.Default.ViewType = (int)this.myType;

            Settings.Default.SelectedDirectory = m_selectedDirectory;

            // Save settings
            Settings.Default.Save();
        }

        private void licenseToolStripButton_DropDownOpening(object sender, EventArgs e)
        {
            useActivationOnLicenseDropDownMenuItem.DropDownItems.Clear();
            genVerificationOnLicenseDropDownMenuItem.DropDownItems.Clear();
            genLicArchiveOnLicenseDropDownMenuItem.DropDownItems.Clear();

            //Expose enterPasswordToolStripMenuItem for any protection key that is not Uninitialized/Verification
            enterPasswordToolStripMenuItem.Visible = false;
            enterPasswordPacketToolStripMenuItem.Visible = false;
            sep5ToolStripMenuItem.Visible = false;
            foreach (Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseProtectionKeyInfo keyInfo in m_CommLink.KeyEnumerate(false))
            {
                if (keyInfo.IsKeyTypeUninitialized()==false && keyInfo.IsKeyTypeVerification()==false)
                {
                    enterPasswordToolStripMenuItem.Visible = true;
                    enterPasswordPacketToolStripMenuItem.Visible = true;
                    sep5ToolStripMenuItem.Visible = true;
                    break;
                }
            }

            string generalStream = null;
            m_CommLink.GetAllSoftwareLicenses(ref generalStream, false);
            Solimar.Licensing.Attribs.AttribsMemberStringList strList = new Solimar.Licensing.Attribs.AttribsMemberStringList("stringList", new System.Collections.ArrayList());
            strList.SVal = generalStream;
            bool bAtleastOneSoftwareServer = false;
            //display each package name and type
            System.Collections.Generic.SortedList<string, ToolStripMenuItem> genVerifyDataSortedList = new SortedList<string, ToolStripMenuItem>();
            System.Collections.Generic.SortedList<string, ToolStripMenuItem> genLicArchiveSortedList = new SortedList<string, ToolStripMenuItem>();
            System.Collections.Generic.SortedList<string, ToolStripMenuItem> useLicArchiveSortedList = new SortedList<string, ToolStripMenuItem>();
            foreach (string softwareLicense in strList.TVal)
            {
                bAtleastOneSoftwareServer = true;
                ToolStripMenuItem tsMenuItem = null;
                Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs licInfoAttrib = new Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs();
                m_CommLink.GetSoftwareLicenseInfoByLicense(softwareLicense, ref generalStream, false);
                licInfoAttrib.AssignMembersFromStream(generalStream);

                if (!(licInfoAttrib.activationAmountInDays.TVal == 0 && licInfoAttrib.activationCurrent.TVal == 0 && licInfoAttrib.activationTotal.TVal == 0) && (licInfoAttrib.activationCurrent.TVal != licInfoAttrib.activationTotal.TVal))
                {
                    tsMenuItem = new ToolStripMenuItem(softwareLicense);
                    tsMenuItem.Click += new EventHandler(tsUseActivationMenuItem_Click);
                    tsMenuItem.Tag = new int[] { (int)licInfoAttrib.activationCurrent.TVal, (int)licInfoAttrib.activationTotal.TVal, (int)licInfoAttrib.activationAmountInDays.TVal };
                    useLicArchiveSortedList.Add(softwareLicense, tsMenuItem);
                }

                tsMenuItem = new ToolStripMenuItem(softwareLicense);
                tsMenuItem.ToolTipText = "Generate Verification Data of License: " + softwareLicense + " for Solimar";
                tsMenuItem.Click += new EventHandler(tsGenCopyForSolimarMenuItem_Click);
                if (genVerifyDataSortedList.ContainsKey(softwareLicense) == false)
                {
                    genVerifyDataSortedList.Add(softwareLicense, tsMenuItem);

                    //Only display software licenses in this list that have a ProtectionKeyID in its list of verification tokens...
                    string protectionKey = Solimar.Licensing.Attribs.Lic_LicenseInfoAttribsHelper.GetValidationValue(licInfoAttrib.licVerificationAttribs.TVal.validationTokenList, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID);
                    if (protectionKey.Length > 0)
                    {
                        tsMenuItem = new ToolStripMenuItem(softwareLicense);
                        tsMenuItem.ToolTipText = "Generate License Archive to transfer to another computer for License: " + softwareLicense;
                        tsMenuItem.Click += new EventHandler(tsGenLicArchiveMenuItem_Click);
                        genLicArchiveSortedList.Add(softwareLicense, tsMenuItem);
                    }
                }
            }
            foreach (string swLic in useLicArchiveSortedList.Keys)
                useActivationOnLicenseDropDownMenuItem.DropDownItems.Add(useLicArchiveSortedList[swLic]);
            foreach (string swLic in genVerifyDataSortedList.Keys)
                genVerificationOnLicenseDropDownMenuItem.DropDownItems.Add(genVerifyDataSortedList[swLic]);
            foreach (string swLic in genLicArchiveSortedList.Keys)
                genLicArchiveOnLicenseDropDownMenuItem.DropDownItems.Add(genLicArchiveSortedList[swLic]);

            updateLicToolStripMenuItem.Visible = bAtleastOneSoftwareServer;
            genVerificationOnLicenseDropDownMenuItem.Visible = genVerificationOnLicenseDropDownMenuItem.DropDownItems.Count != 0;
            sep2ToolStripMenuItem.Visible = genVerificationOnLicenseDropDownMenuItem.DropDownItems.Count != 0;

            genLicArchiveOnLicenseDropDownMenuItem.Visible = genLicArchiveOnLicenseDropDownMenuItem.DropDownItems.Count != 0;
            
            useActivationOnLicenseDropDownMenuItem.Visible = useActivationOnLicenseDropDownMenuItem.DropDownItems.Count != 0;
            sep3ToolStripMenuItem.Visible = useActivationOnLicenseDropDownMenuItem.DropDownItems.Count != 0;
        }

        void tsGenLicArchiveMenuItem_Click(object sender, EventArgs e)
        {
            if (sender is ToolStripMenuItem)
            {
                ToolStripMenuItem tmpToolStripItem = sender as ToolStripMenuItem;

                this.exportPktDialog.DefaultExt = "licArchive";
                this.exportPktDialog.Filter = "License Archive|*.licArchive";
                this.exportPktDialog.Title = "Generate License Archive";
                this.exportPktDialog.FileName = tmpToolStripItem.Text;
                this.exportPktDialog.InitialDirectory = m_selectedDirectory;
                if (this.exportPktDialog.ShowDialog() == DialogResult.OK)
                {
                    try
                    {
                        m_selectedDirectory = System.IO.Path.GetDirectoryName(this.exportPktDialog.FileName);
                        Byte[] newByteArrayLicense = null;
                        m_CommLink.GenerateSoftwareLicArchive_ByLicense(tmpToolStripItem.Text, ref newByteArrayLicense);
                        if (newByteArrayLicense != null)
                        {
                            File.WriteAllBytes(exportPktDialog.FileName, newByteArrayLicense);
                            PopulateAllViews();
                        }
                    }
                    catch (COMException ex)
                    {
                        //alert user of failure
                        HandleExceptions.DisplayException(this, ex, "Failed to generate License Archive!", "License Archive");
                    }
                }
            }
        }

        private void tsGenCopyForSolimarMenuItem_Click(object sender, EventArgs e)
        {
            if (sender is ToolStripMenuItem)
            {
                ToolStripMenuItem tmpToolStripItem = sender as ToolStripMenuItem;

                this.exportPktDialog.DefaultExt = "vData";
                this.exportPktDialog.Filter = "License Verification Data|*.vData";
                this.exportPktDialog.Title = "Copy of License";
                this.exportPktDialog.FileName = tmpToolStripItem.Text;
                this.exportPktDialog.InitialDirectory = m_selectedDirectory;

                if (this.exportPktDialog.ShowDialog() == DialogResult.OK)
                {
                    try
                    {
                        m_selectedDirectory = System.IO.Path.GetDirectoryName(this.exportPktDialog.FileName);
                        Byte[] newByteArrayLicense = null;
                        m_CommLink.GenerateVerifyDataWithLicInfo_ByLicense(tmpToolStripItem.Text, ref newByteArrayLicense);
                        if (newByteArrayLicense != null)
                            File.WriteAllBytes(exportPktDialog.FileName, newByteArrayLicense);
                    }
                    catch (COMException ex)
                    {
                        //alert user of failure
                        HandleExceptions.DisplayException(this, ex, "Failed to generate copy of License!", "Copy of License");
                    }
                }
            }
        }

        private void tsGenVerificationDataMenuItem_Click(object sender, EventArgs e)
        {
            if (sender is ToolStripMenuItem)
            {
                ToolStripMenuItem tmpToolStripItem = sender as ToolStripMenuItem;

                this.exportPktDialog.DefaultExt = "vData";
                this.exportPktDialog.Filter = "License Verification Data|*.vData";
                this.exportPktDialog.Title = "License Verification Data";
                this.exportPktDialog.FileName = tmpToolStripItem.Text;
                this.exportPktDialog.InitialDirectory = m_selectedDirectory;

                if (this.exportPktDialog.ShowDialog() == DialogResult.OK)
                {
                    try
                    {
                        m_selectedDirectory = System.IO.Path.GetDirectoryName(this.exportPktDialog.FileName);
                        Byte[] newByteArrayLicense = null;
                        m_CommLink.GenerateVerifyDataWithVerCode_ByLicense(tmpToolStripItem.Text, ref newByteArrayLicense);
                        if (newByteArrayLicense != null)
                            File.WriteAllBytes(exportPktDialog.FileName, newByteArrayLicense);
                    }
                    catch (COMException ex)
                    {
                        //alert user of failure
                        HandleExceptions.DisplayException(this, ex, "Failed to generate License Verification Data!", "License Verification Data");
                    }
                }
            }
        }

        private void genDiagnosticDataDropDownMenuItem_Click(object sender, EventArgs e)
        {
            if (sender is ToolStripMenuItem)
            {
                ToolStripMenuItem tmpToolStripItem = sender as ToolStripMenuItem;
                this.exportPktDialog.DefaultExt = "lsData";
                this.exportPktDialog.Filter = "License System Data|*.lsData";
                this.exportPktDialog.Title = "License System Data";
                this.exportPktDialog.FileName = System.Environment.MachineName.ToLower();
                this.exportPktDialog.InitialDirectory = m_selectedDirectory;
                if (this.exportPktDialog.ShowDialog() == DialogResult.OK)
                {
                    try
                    {
                        this.Cursor = Cursors.WaitCursor;
                        m_selectedDirectory = System.IO.Path.GetDirectoryName(this.exportPktDialog.FileName);
                        Byte[] newByteArrayLicense = null;
                        m_CommLink.GenerateLicenseSystemData(ref newByteArrayLicense);
                        if (newByteArrayLicense != null)
                            File.WriteAllBytes(exportPktDialog.FileName, newByteArrayLicense);
                    }
                    catch (COMException ex)
                    {
                        //alert user of failure
                        HandleExceptions.DisplayException(this, ex, "Failed to generate License System Data!", "License System Data");
                    }
                    finally
                    {
                        this.Cursor = Cursors.Default;
                    }
                }
            }
        }

        private void tsUseActivationMenuItem_Click(object sender, EventArgs e)
        {
            if (sender is ToolStripMenuItem)
            {
                ToolStripMenuItem tmpToolStripItem = sender as ToolStripMenuItem;
                int[] intArray = (int[])tmpToolStripItem.Tag;  //should be in the format: activationCurrent, activationTotal, activationAmountInDays
                string message = string.Format("You are on activation {0} of {1} for License {2}. An activation will give you {3} more days. Are you sure you want to use an Activation?", intArray[0], intArray[1], tmpToolStripItem.Text, intArray[2]);
                if (MessageBox.Show(this, message, "Activate License: " + tmpToolStripItem.Text, MessageBoxButtons.YesNo, MessageBoxIcon.Question, MessageBoxDefaultButton.Button2) == DialogResult.Yes)
                {
                    try
                    {
                        m_CommLink.SoftwareLicenseDisasterRecoveryExtendTimeByLicense(tmpToolStripItem.Text);
                        refreshToolStripButton_Click(null, new EventArgs());
                    }
                    catch (Exception ex)
                    {
                        HandleExceptions.DisplayException(this, ex, "Failed to Activate License!", "Activate License");
                    }
                }
            }
        }

        private void importLicArchiveOnLicenseDropDownMenuItem_Click(object sender, EventArgs e)
        {
            LicenseUpdateHelper licHelper = new LicenseUpdateHelper();
            licHelper.SetData(new LicenseUpdateHelper.LicenseUpdateHelperData(LicenseUpdateHelper.LicenseUpdateActionEnum.luaeImportNewLicArchive, m_selectedDirectory), m_CommLink);
            if (licHelper.ShowDialog() == DialogResult.OK)
            {
                LicenseUpdateHelper.LicenseUpdateHelperData data = licHelper.GetData();
                if (data != null)
                    m_selectedDirectory = data.directory;

                PopulateAllViews();
            }
        }

        #endregion

        #region Protected Variables

        protected ViewType myType;

        #endregion

        #region Private Variables

        private ListViewMgr m_listViewMgr;
        private TreeViewMgr m_treeViewMgr;
        private CommunicationLink m_CommLink;
        private List<String> m_ServerList;
        private ImageList il;
        private string m_selectedDirectory;

        #endregion





    }

    //Was lazy, this should be in it's own file - JWL... 
    public class HandleExceptions
    {
        public HandleExceptions()
        {
        }

        public static void DisplayException(System.Exception paramException)
        {
            DisplayException(null, paramException, "", "Error");
        }


        public static void DisplayException(System.Exception paramException, string paramMessageString)
        {
            DisplayException(null, paramException, paramMessageString, "Error");
        }

        public static void DisplayException(System.Exception paramException, string paramMessageString, string paramCaptionString)
        {
            DisplayException(null, paramException, paramMessageString, paramCaptionString);
        }

        public static void DisplayException(System.Windows.Forms.Form paramParentForm, System.Exception paramException, string paramMessageString, string paramCaptionString)
        {
            StringBuilder errBuilder = new StringBuilder();

            if (paramMessageString.Length > 0)
                errBuilder.Append(paramMessageString);
            if (errBuilder.Length > 0)
                errBuilder.Append("\n\r\nReason - ");

            errBuilder.Append(paramException.Message);

            MessageBox.Show(paramParentForm,
                            errBuilder.ToString(),
                            paramCaptionString,
                            MessageBoxButtons.OK,
                            MessageBoxIcon.Error);
        }
    }
}