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
    public partial class Form1 : Form
    {
        #region Constructors
        public Form1()
        {
            InitializeComponent();

            Shared.VisualComponents.ControlHelper.ReduceFlicker(this);

            Shared.VisualComponents.ControlHelper.SetWindowTheme(this.treeView.Handle, "Explorer", null);

            // CR.17742 - License Viewer flickers - When setting TVS_EX_FADEINOUTEXPANDOS, when you shrink, then repop up the License Viewer, the 
            // License Viewer majorly flickers like crazy.  Do not use this.
            //Treeviews in Windows Explorer also have the fade effects. This can be achieved via the TVS_EX_FADEINOUTEXPANDOS [0x0040] extended style.
            //Shared.VisualComponents.ControlHelper.SendMessage(this.treeView.Handle, 0x1100 + 44, (IntPtr)0x0040, (IntPtr)0x0040);

            //The treeviews also have the "auto-scroll" feature. You can enable this via the TVS_EX_AUTOHSCROLL [0x0020] extended style.
            //Shared.VisualComponents.ControlHelper.SendMessage(this.treeView.Handle, 0x1100 + 44, (IntPtr)0x0020, (IntPtr)0x0020);

            //Disable form items
            EnableFormItems(false);
            //create connection to license wrapper
            m_CommLink = new CommunicationLink();
        }
        #endregion

        #region Form Methods
        private void PopulateAllViews()
        {
            Cursor.Current = Cursors.WaitCursor;
            //this.noFlickerListView.BeginUpdate();
            //this.noFlickerListView.Items.Clear();
            //this.noFlickerListView.EndUpdate();
            PopulateTreeView();
            Cursor.Current = Cursors.Default;
        }

        private void PopulateTreeView()
        {
            Cursor.Current = Cursors.WaitCursor;
            m_treeViewMgr.PopulateView(hideEmptyLicServersToolStripMenuItem.Checked);
            if (m_listViewMgr.SelectedNode != null)
            {
                TreeNode[] foundNodes = this.treeView.Nodes.Find(m_listViewMgr.SelectedNode.Name, true);
                if (foundNodes.Length > 0)
                    this.treeView.SelectedNode = foundNodes[0];
                else
                    this.treeView.SelectedNode = null;
            }
            else if (this.treeView.Nodes.Count > 0)
            {
                if (string.Compare(this.treeView.Nodes[0].Text, "Licenses") == 0)
                    this.treeView.SelectedNode = this.treeView.Nodes[0].Nodes.Count > 0 ? this.treeView.Nodes[0].Nodes[0] : this.treeView.Nodes[0];
                else
                    //this.treeView.SelectedNode = this.treeView.Nodes[this.treeView.Nodes.Count - 1];
                    this.treeView.SelectedNode = this.treeView.Nodes[this.treeView.Nodes.Count - 2];
            }
            Cursor.Current = Cursors.Default;
        }

        private void PopulateListView()
        {
            //System.Diagnostics.Trace.WriteLine("PopulateListView() - Start");
            try
            {
                //Application.UseWaitCursor = true;
                SetWaitCursor(true);
                //this.Enabled = false;
                //Cursor.Current = Cursors.WaitCursor;
                //this.noFlickerListView.BeginUpdate();
                //Application.DoEvents();
                m_listViewMgr.PopulateView();
            }
            catch (Exception)
            {
            }
            finally
            {
                //this.noFlickerListView.EndUpdate();
                this.Enabled = true;
                SetWaitCursor(false);
                //Application.UseWaitCursor = false;
                //Cursor.Current = Cursors.Default;
            }
            
            //System.Diagnostics.Trace.WriteLine("PopulateListView() - End");
        }
        private void SetWaitCursor(bool _bUseWaitCursor)
        {
            //System.Diagnostics.Trace.WriteLine(string.Format("UseWaitCursor({1}) - Pre - this.Cursor: {0}", this.Cursor.ToString(), _bUseWaitCursor));
            if (this.InvokeRequired == true)
            {
                this.Invoke(new ListViewMgr.ParamBoolReturnsVoid(this.SetWaitCursor), new object[] { _bUseWaitCursor });
            }
            else
            {
                this.Enabled = !_bUseWaitCursor;
                Application.UseWaitCursor = _bUseWaitCursor;
                //this.UseWaitCursor = _bUseWaitCursor;
                Application.DoEvents();
            }
            //System.Diagnostics.Trace.WriteLine(string.Format("UseWaitCursor({1}) - Post - this.Cursor: {0}", this.Cursor.ToString(), _bUseWaitCursor));
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
            // If is a child form, then prompt for server to connect to
            // If not a child form, connect to local host by default.  Handle case when no license
            // server is on local machine
            #region if (!bViewDiagnosticData && m_bIsChildForm)
            if (!m_bViewDiagnosticData && m_bIsChildForm)
            {
                //using keyword forces scope onto object, so it will be disposed after it is done.
                DisconnectServer();
                using (ConnectDialog cd = new ConnectDialog(m_CommLink))
                {
                    // Set Server Connection List
                    if (Settings.Default.ServerList == null)
                        Settings.Default.ServerList = new System.Collections.Specialized.StringCollection();

                    List<string> serverList = new List<string>();
                    foreach (string str in Settings.Default.ServerList)
                        serverList.Add(str);

                    ConnectDialogData data = new ConnectDialogData(serverList.ToArray());
                    if (cd.ShowDialog(this, data) == DialogResult.OK)
                    {
                        //save server list into settings again 
                        //Copy ServerConnection List to app settings
                        if (data.ServerList.Count > 0)
                        {
                            Settings.Default.ServerList.Clear();
                            Settings.Default.ServerList.AddRange(data.ServerList.ToArray());
                            Settings.Default.Save();
                        }

                        if (this.parentForm is SolimarLicenseViewer.Form1)
                            (this.parentForm as SolimarLicenseViewer.Form1).setChildInformation(this, m_CommLink.ServerName, true);
                    }
                    else
                    {
                        this.Close();
                        return 0;
                    }
                }
            }
            #endregion
            #region else //connect machine
            else
            {
                string machineName = "";
                if (m_bViewDiagnosticData)
                {
                    machineName = m_CommLink.ServerName;
                    ConnectionStatusLabel.Text = "Loading Diagnostic Data";
                    this.Text = string.Format("Loading Diagnostic Data[{0}]...", m_diagnosticDataFileName);
                    if (this.parentForm is SolimarLicenseViewer.Form1)
                        (this.parentForm as SolimarLicenseViewer.Form1).setChildInformation(this, string.Format("Diagnostic Data - {0}", this.m_diagnosticDataFileName), false);
                }
                else
                {
                    machineName = System.Environment.MachineName.ToLower();
                    ConnectionStatusLabel.Text = "Connecting";
                    this.Text = string.Format("Connecting to [{0}]...", machineName);
                }
                try
                {
                    m_CommLink.Connect(machineName, false);
                }
                catch (Exception)
                {
                    m_CommLink.ServerName = machineName;
                }
            }
            #endregion

            using (Shared.VisualComponents.BaseMessageDialog messageDialog = new Shared.VisualComponents.BaseMessageDialog())
            {
                messageDialog.SetData(new Shared.VisualComponents.MessageBoxData());
                messageDialog.Show();

                try
                {
                    if (m_bViewDiagnosticData)
                        this.Text = string.Format("Loading Diagnostic Data[{0}]...", m_diagnosticDataFileName);
                    else
                        this.Text = string.Format("Connecting to [{0}]...", m_CommLink.ServerName);

                    messageDialog.SetMessage(this.Text);
                    messageDialog.Update();

                    //create treeview manager
                    m_listViewMgr = new ListViewMgr(
                        this.infoSplitContainer, 
                        this.noFlickerListView, 
                        this.lvToolStrip, 
                        this.bottomNoFlickerListView, 
                        this.bottomLvToolStrip, 
                        this.bottomEventLogEntryControl1, 
                        m_CommLink,
                        productIconList
                        );
                    m_listViewMgr.DelUseWaitCursor += new ListViewMgr.ParamBoolReturnsVoid(this.SetWaitCursor);
                    m_listViewMgr.DelTestConnection += new ListViewMgr.ParamListViewItemListReturnsVoid(this.TestConnection);
                    //create listview manager
                    m_treeViewMgr = new TreeViewMgr(this.treeView, m_CommLink);
                    //fill listview/treeview
                    PopulateAllViews();
                    //Enable Menu Items
                    EnableFormItems(true);
                    //Update Status
                    ConnectionStatusLabel.Text = String.Empty;

                    int majorVer = 0;
                    int minorVer = 0;
                    int buildVer = 0;
                    string licSvrVer = string.Format("{0}.{1}.{2}", majorVer, minorVer, buildVer);
                    try
                    {
                        if (!m_CommLink.IsConnected())
                        {
                            licSvrVer = string.Format(" - Licenese Server NOT installed{0}", m_CommLink.Exception != null ? (" - " + m_CommLink.Exception.Message) : "");
                        }
                        else
                        {
                            m_CommLink.GetVersionLicenseServer(m_CommLink.ServerName, ref majorVer, ref minorVer, ref buildVer);
                            licSvrVer = string.Format(" - version: {0}.{1}.{2}", majorVer, minorVer, buildVer);
                        }
                    }
                    catch (COMException)
                    {
                        majorVer = 0;
                        minorVer = 0;
                        buildVer = 0;
                        licSvrVer = "";
                    }

                    if (!m_bViewDiagnosticData)
                    {
                        this.Text = string.Format("{0} [{1}]{2}",
                            AppConstants.FormTitle,
                            m_CommLink.ServerName,
                            licSvrVer.Length == 0 ? "" : licSvrVer);
                    }
                    else
                    {
                        this.Text = string.Format("{0} [{1}] - created date: {2}",
                            AppConstants.DiagnosticDataTitle,
                            m_diagnosticDataFileName,
                            m_CommLink.diagnosticDateCreatedDate.Value.ToLocalTime().ToString()
                            );

                    }

                    if (m_CommLink.Exception != null)
                    {
                        //Failed to connect to a license server, disable all license settings.
                        fileLicenseToolStripMenuItem.Visible = false;
                        diagnosticDataToolStripMenuItem.Visible = false;
                    }
                }
                catch (COMException)
                {
                }
                messageDialog.Hide();
            }
            return 0;
        }

        private void EnableFormItems(bool status)
        {
            EnableMenuItems(status);
        }

        private void EnableMenuItems(bool status)
        {
            fileLicenseToolStripMenuItem.Enabled = status;
            viewToolStripMenuItem.Enabled = status;
            diagnosticDataToolStripMenuItem.Enabled = status;
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
                    HandleExceptions.DisplayException(this, ex, "Invalid Password - Failed to apply license password", "Apply License Password");
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

        private string ConvertToCsv(string line)
        {
            return line.Replace("\"", "\"\"");
        }
        private void General_KeyDown(object sender, KeyEventArgs e)
        {
            //Alan, you might want to move this into your ListViewMgr.cs, I just threw it in here because I needed some
            //copying behavior - Jeff 10-10-08
            ListView tmpListView = sender as ListView;
            if (tmpListView != null)
            {
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
                            strBuilder.Append(string.Format("\"{0}\"", ConvertToCsv(selItem.SubItems[idx].Text)));
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
        private void hideEmptyLicServersToolStripMenuItem_Click(object sender, EventArgs e)
        {
            hideEmptyLicServersToolStripMenuItem.Checked = !hideEmptyLicServersToolStripMenuItem.Checked;
            refreshToolStripButton_Click(null, null);
        }

        #endregion

        #region ContextMenu Events

        private void lvContextMenuStrip_Opening(object sender, CancelEventArgs e)
        {
            bool bTopListViewSelected = false;
            if (sender is System.Windows.Forms.ContextMenuStrip)
            {
                object tmpObj = ((System.Windows.Forms.ContextMenuStrip)(sender)).SourceControl;
                if (tmpObj is Shared.VisualComponents.NoFlickerListView)
                    copyToolStripMenuItem.Enabled = (tmpObj as Shared.VisualComponents.NoFlickerListView).SelectedItems.Count > 0;
                bTopListViewSelected = (tmpObj == this.noFlickerListView);
            }

            bool bDisplayExtendOption = false;
            bool bDisplayConnectionSettings = false;
            bool bSelectedItem = false;
            ListViewItem item = null;
            //if(string.Compare(m_listViewMgr.SelectedNode.Text, AppConstants.LicenseRootNode, true) == 0)
            //Look for Product License Level
            if (m_listViewMgr.SelectedNode.Level == 2 && m_listViewMgr.SelectedNode.Parent != null && m_listViewMgr.SelectedNode.Parent.Parent != null
                && string.Compare(m_listViewMgr.SelectedNode.Parent.Parent.Text, AppConstants.LicenseRootNode, true) == 0
                && bTopListViewSelected)
            {
                // find any selected 
                if (this.noFlickerListView.SelectedItems.Count > 0)
                    item = this.noFlickerListView.SelectedItems[0];

                //YYY - Stopped here on friday 12-4-09
                // user clicked in the background 
                //bDisplayExtendOption = (item == null || item.SubItems[1].Text != AppConstants.DisasterRecoveryLicType);
                bDisplayExtendOption = true;
            }
            else if (string.Compare(m_listViewMgr.SelectedNode.Text, AppConstants.ProductConnectionSettingsRootNode, true) == 0)
            {
                bDisplayConnectionSettings = true;
                bSelectedItem = (this.noFlickerListView.SelectedItems.Count > 0);
            }

            if (m_CommLink.bDiagnosticDateView)//for diagnostic view, hide ability to extend
            {
                bDisplayExtendOption = false;
                extendToolStripMenuItem.Visible = false;
            }
            extendToolStripMenuItem.Visible = bDisplayExtendOption;
            extendToolStripSeparator.Visible = bDisplayExtendOption;
            if (bDisplayExtendOption)
            {
                // Cycle through all the selected items
                extendToolStripMenuItem.Enabled = this.noFlickerListView.SelectedItems.Count > 0;
                foreach (ListViewItem lvi in this.noFlickerListView.SelectedItems)
                {
                    //if (m_listViewMgr.EnableDisasterRecoverExt_ByLicense(lvi.Text) == false)
                    //{
                    //    extendToolStripMenuItem.Enabled = false;
                    //    break;
                    //}
                    if (m_listViewMgr.EnableActivationExt_ByProductLicense(lvi.Text) == false)
                    {
                        extendToolStripMenuItem.Enabled = false;
                        break;
                    }
                }
            }

            editConnToolStripSeparator.Visible = bDisplayConnectionSettings;
            editConnToolStripMenuItem.Visible = bDisplayConnectionSettings;
            editConnToolStripMenuItem.Enabled = bSelectedItem;
            testConnToolStripSeparator.Visible = bDisplayConnectionSettings;
            testConnSelToolStripMenuItem.Visible = bDisplayConnectionSettings;
            testConnSelToolStripMenuItem.Enabled = bSelectedItem;
            testConnToAllToolStripMenuItem.Visible = bDisplayConnectionSettings;
        }

        private void copyToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ListView parentListView = null;
            if (sender is ToolStripItem)
                parentListView = (ListView)((System.Windows.Forms.ContextMenuStrip)((sender as ToolStripItem).Owner)).SourceControl;

            if (parentListView != null)
                General_KeyDown(parentListView, new KeyEventArgs(Keys.C | Keys.Control));
        }

        private void extendToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                //useActivationInternal(
                //    this.noFlickerListView.SelectedItems[0].Text,   //license name
                //    Convert.ToInt32(this.noFlickerListView.SelectedItems[0].SubItems[4].Text),   //activationCurrent
                //    Convert.ToInt32(this.noFlickerListView.SelectedItems[0].SubItems[5].Text),   //activationTotal
                //    Convert.ToInt32(this.noFlickerListView.SelectedItems[0].SubItems[6].Text)    //activationAmountInDays
                //);
                //int[] intArray = (int[])this.noFlickerListView.SelectedItems[0].Tag;  //should be in the format: activationCurrent, activationTotal, activationAmountInDays
                //useActivationInternal(this.noFlickerListView.SelectedItems[0].Text,
                //    intArray[0], //activationCurrent
                //    intArray[1], //activationTotal
                //    intArray[2]);//activationAmountInDays

                object[] objArray = (object[])this.noFlickerListView.SelectedItems[0].Tag;  //should be in the format: (string)softwareLicense, (int)activationCurrent, (int)activationTotal, (int)activationAmountInDays
                useActivationInternal((string)objArray[0], //software license
                    this.noFlickerListView.SelectedItems[0].Text, //product license number
                    (int)objArray[1], //activationCurrent
                    (int)objArray[2], //activationTotal
                    (int)objArray[3], //activationAmountInDays
                    (DateTime)objArray[4],
                    (DateTime)objArray[5]
                    );


                //useActivationInternal(tmpToolStripItem.Text, objArray[0], objArray[1], objArray[2], objArray[3]);

            }
            catch (COMException)
            {
            }
        }

        private void editConnToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_listViewMgr.prodConn_EditConnSettings(sender, e);
        }

        private void testConnSelToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_listViewMgr.prodConnTestConnSelTSButton_Click(sender, e);
        }

        private void testConnToAllToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_listViewMgr.prodConnTestConnAllTSButton_Click(sender, e);
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


        #region Help Menu Items
        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (AboutBox aboutBox = new AboutBox())
            {
                int majorVer = 0;
                int minorVer = 0;
                int buildVer = 0;
                string licMgrVer = "";
                try
                {
                    m_CommLink.GetVersionLicenseManager(ref majorVer, ref minorVer, ref buildVer);
                    licMgrVer = string.Format("{0}.{1}.{2}", majorVer, minorVer, buildVer);
                }
                catch (COMException)
                {
                    majorVer = 0;
                    minorVer = 0;
                    buildVer = 0;
                    licMgrVer = string.Format("Not Installed");
                }
                aboutBox.SetManagerVersion(licMgrVer);
                aboutBox.SetManagerName(System.Environment.MachineName.ToLower());

                string licSvrVer = "";
                try
                {
                    m_CommLink.GetVersionLicenseServer(m_CommLink.ServerName, ref majorVer, ref minorVer, ref buildVer);
                    licSvrVer = string.Format("{0}.{1}.{2}", majorVer, minorVer, buildVer);
                }
                catch (COMException)
                {
                    majorVer = 0;
                    minorVer = 0;
                    buildVer = 0;
                    licSvrVer = string.Format("Not Installed");
                }
                aboutBox.SetServerVersion(licSvrVer);
                aboutBox.SetServerName(m_CommLink.ServerName);
                aboutBox.ShowDialog(this);
            }
        }
        private void solimarLicenseManualToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string manualFullPath = System.IO.Path.Combine(
                System.IO.Path.GetDirectoryName(System.Windows.Forms.Application.ExecutablePath),
                "Manuals\\Solimar Licensing Manual.pdf");
            System.Diagnostics.Process.Start(manualFullPath);
        }
        #endregion

        #endregion

        #region Form Events

        //TODO : Need to be able to figure out how to create tree from contents
        private void treeView_AfterSelect(object sender, TreeViewEventArgs e)
        {
            m_listViewMgr.SelectedNode = this.treeView.SelectedNode;
            m_listViewMgr.SelectedNode.Expand();
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
                int moveAmount = m_bIsChildForm ? 50 : 0;
                this.Location = new Point(Settings.Default.WindowLocation.X + moveAmount, Settings.Default.WindowLocation.Y + moveAmount);
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

            hideEmptyLicServersToolStripMenuItem.Checked = Settings.Default.HideEmptyLicServers;
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (!m_bIsChildForm)
            {
                // Copy window location to app settings
                if (this.Location.X > 0 && this.Location.Y > 0)
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
                Settings.Default.HideEmptyLicServers = hideEmptyLicServersToolStripMenuItem.Checked;

                // Save settings
                Settings.Default.Save();

            }
            else
            {
                if (this.parentForm is SolimarLicenseViewer.Form1)
                    (this.parentForm as SolimarLicenseViewer.Form1).childFormClose(this, "");

            }
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
                if (keyInfo.IsKeyTypeUninitialized() == false && keyInfo.IsKeyTypeVerification() == false)
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
            //genVerificationOnLicenseDropDownMenuItem.Visible = genVerificationOnLicenseDropDownMenuItem.DropDownItems.Count != 0;
            //sep2ToolStripMenuItem.Visible = genVerificationOnLicenseDropDownMenuItem.DropDownItems.Count != 0;

            genLicArchiveOnLicenseDropDownMenuItem.Visible = genLicArchiveOnLicenseDropDownMenuItem.DropDownItems.Count != 0;

            useActivationOnLicenseDropDownMenuItem.Visible = useActivationOnLicenseDropDownMenuItem.DropDownItems.Count != 0;
            sep3ToolStripMenuItem.Visible = useActivationOnLicenseDropDownMenuItem.DropDownItems.Count != 0;
        }


        void tsGenLicArchiveMenuItem_Click(object sender, EventArgs e)
        {
            ToolStripMenuItem tmpToolStripItem = sender as ToolStripMenuItem;
            if (tmpToolStripItem != null)
            {
                this.exportPktDialog.DefaultExt = "licArchive";
                this.exportPktDialog.Filter = "License Archive|*.licArchive";
                this.exportPktDialog.Title = "Generate License Archive";
                this.exportPktDialog.FileName = tmpToolStripItem.Text;
                this.exportPktDialog.InitialDirectory = m_selectedDirectory;
                if (this.exportPktDialog.ShowDialog() == DialogResult.OK)
                {
                    try
                    {
                        this.Cursor = Cursors.WaitCursor;
                        m_selectedDirectory = System.IO.Path.GetDirectoryName(this.exportPktDialog.FileName);
                        Byte[] newByteArrayLicense = null;
                        using (Shared.VisualComponents.BaseMessageDialog messageDialog = new Shared.VisualComponents.BaseMessageDialog())
                        {
                            messageDialog.SetData(new Shared.VisualComponents.MessageBoxData());
                            messageDialog.Show();

                            messageDialog.SetMessage(string.Format("Generating License Archive..."));
                            messageDialog.Update();
                            m_CommLink.Async_GenerateSoftwareLicArchive_ByLicense(tmpToolStripItem.Text, ref newByteArrayLicense);

                            messageDialog.Hide();
                        }
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
                    finally
                    {
                        this.Cursor = Cursors.Default;
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
                        this.Cursor = Cursors.WaitCursor;
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
                    finally
                    {
                        this.Cursor = Cursors.Default;
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
                        this.Cursor = Cursors.WaitCursor;
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
                    finally
                    {
                        this.Cursor = Cursors.Default;
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
                this.exportPktDialog.FileName = string.Compare(m_CommLink.ServerName, "localhost", true) == 0 ? System.Environment.MachineName.ToLower() : m_CommLink.ServerName;
                this.exportPktDialog.InitialDirectory = m_selectedDirectory;
                if (this.exportPktDialog.ShowDialog() == DialogResult.OK)
                {
                    try
                    {
                        this.Cursor = Cursors.WaitCursor;
                        m_selectedDirectory = System.IO.Path.GetDirectoryName(this.exportPktDialog.FileName);
                        Byte[] newByteArrayLicense = null;
                        using (Shared.VisualComponents.BaseMessageDialog messageDialog = new Shared.VisualComponents.BaseMessageDialog())
                        {
                            messageDialog.SetData(new Shared.VisualComponents.MessageBoxData());
                            messageDialog.Show();

                            messageDialog.SetMessage(string.Format("Generating Diagnostic Data..."));
                            messageDialog.Update();
                            m_CommLink.Async_GenerateLicenseSystemData(ref newByteArrayLicense);

                            messageDialog.Hide();
                        }
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

        private void eMailToSolimarSystemsIncToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                int major = 0;
                int minor = 0;
                int build = 0;
                this.m_CommLink.GetVersionLicenseServer(this.m_CommLink.ServerName, ref major, ref minor, ref build);
                if (major < 3 || (major == 3 && minor < 2))
                {
                    throw new Exception(string.Format("License Server '{0}' must be at least version 3.2 for this feature.  The server is currenly version {1}.{2}", this.m_CommLink.ServerName, major, minor));
                }

                string confirmQuestion = "Are you sure you want to e-mail the Diagnostic Data to Solimar Systems, Inc.?";
                if (MessageBox.Show(confirmQuestion, "Confirm e-mailing to Solimar Systems, Inc.", MessageBoxButtons.YesNo, MessageBoxIcon.Question, MessageBoxDefaultButton.Button2) == DialogResult.Yes)
                {
                    this.Cursor = Cursors.WaitCursor;
                    if (m_listViewMgr.ensureMailServerIsConfigured())
                    {
                        m_CommLink.Async_GenerateLicenseSystemDataForSolimar();
                        MessageBox.Show(this, "Successfully send Diagnostic Data to Solimar Systems, Inc.");
                    }
                }
            }
            catch (Exception ex)
            {
                HandleExceptions.DisplayException(this, ex, "Failed to generate License Diagnostic Data and e-mail to Solimar Systems, Inc.", "License System Data");
            }
            finally
            {
                this.Cursor = Cursors.Default;
            }
        }

        private void tsUseActivationMenuItem_Click(object sender, EventArgs e)
        {
            if (sender is ToolStripMenuItem)
            {
                ToolStripMenuItem tmpToolStripItem = sender as ToolStripMenuItem;
                //int[] intArray = (int[])tmpToolStripItem.Tag;  //should be in the format: activationCurrent, activationTotal, activationAmountInDays
                //useActivationInternal(tmpToolStripItem.Text, intArray[0], intArray[1], intArray[2]);
                object[] objArray = (object[])tmpToolStripItem.Tag;  //should be in the format: (string)softwareLicense, (int)activationCurrent, (int)activationTotal, (int)activationAmountInDays
                //useActivationInternal(tmpToolStripItem.Text, objArray[0], objArray[1], objArray[2], objArray[3]);
                useActivationInternal((string)objArray[0], //product license number
                    tmpToolStripItem.Text, //software license
                    (int)objArray[1], //activationCurrent
                    (int)objArray[2], //activationTotal
                    (int)objArray[3], //activationAmountInDays
                    (DateTime)objArray[4],
                    (DateTime)objArray[5]
                    );
            }
        }
        private void useActivationInternal(string _softwareLicense, string _productLicenseNumber, int _activationCurrent, int _activationTotal, int _activationAmountInDays, DateTime _activationCurrentExpirationDate, DateTime _activationOverallExpirationDate)
        {
            //string message = string.Format("You are on activation {0} of {1} for License {2}. An activation will give you {3} more days. Are you sure you want to use an Activation?", _activationCurrent, _activationTotal, _productLicenseNumber, _activationAmountInDays);
            DateTime origDateTime = DateTime.Now.ToUniversalTime();
            DateTime currDateTime = new DateTime(origDateTime.Year, origDateTime.Month, origDateTime.Day, origDateTime.Hour, 0, 0).AddHours(1);
            //string message = string.Format(
            //    "[{0}] - [{1}] - [{2}] - [{3}] - [{4}] - [{5}] - [{6}] - [{7}]",
            //    _softwareLicense,
            //    _productLicenseNumber, 
            //    _activationCurrent, 
            //    _activationTotal, 
            //    _activationAmountInDays, 
            //    _activationCurrentExpirationDate.ToLocalTime().ToString(),
            //    _activationOverallExpirationDate.ToLocalTime().ToString(),
            //    currDateTime.ToLocalTime().ToString()
            //    );

            if (currDateTime.CompareTo(_activationCurrentExpirationDate) < 0)
                currDateTime = _activationCurrentExpirationDate;
            DateTime newExpDate = currDateTime.AddDays(_activationAmountInDays);
            if (_activationOverallExpirationDate.CompareTo(newExpDate) < 0)
                newExpDate = _activationOverallExpirationDate;

            string message = "";
            if (_activationCurrentExpirationDate.CompareTo(newExpDate) >= 0)
            {
                message = "The Current Activation Expiration Date cannot exceed the Product License Expiration Date.";
                MessageBox.Show(this, message, "Activate Product License: " + _productLicenseNumber, MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            else
            {
                message = string.Format(
                        "You are currently on activation {0} of {1} for Product License: {2}. If you choose to apply another activation it will be valid until: {3}. Do you want to apply another activation now?",
                        _activationCurrent,
                        _activationTotal,
                        _productLicenseNumber,
                        newExpDate.ToLocalTime().ToString()
                    );
                if (MessageBox.Show(this, message, "Activate Product License: " + _productLicenseNumber, MessageBoxButtons.YesNo, MessageBoxIcon.Question, MessageBoxDefaultButton.Button2) == DialogResult.Yes)
                {
                    try
                    {
                        //throw new COMException(message);
                        this.Cursor = Cursors.WaitCursor;
                        m_CommLink.SoftwareLicenseUseActivationToExtendTime_ByLicenseAndContractNumber(_softwareLicense, _productLicenseNumber);
                        refreshToolStripButton_Click(null, new EventArgs());
                    }
                    catch (Exception ex)
                    {
                        HandleExceptions.DisplayException(this, ex, "Failed to Activate License!", "Activate License");
                    }
                    finally
                    {
                        this.Cursor = Cursors.Default;
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

        #endregion

        #region Private Variables

        private ListViewMgr m_listViewMgr;
        private TreeViewMgr m_treeViewMgr;
        private CommunicationLink m_CommLink;
        private string m_selectedDirectory;
        private bool m_bIsChildForm = false;
        public Shared.VisualComponents.BaseMessageDialog messageDialog = null;

        #endregion

        private void remoteConnectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SolimarLicenseViewer.Form1 newForm = new Form1();
            newForm.setAsChildForm(true, this);
            newForm.Show();
        }
        private void loadDiagnosticDataToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //OpenFileDialog
            this.importPktDialog.DefaultExt = "lsData";
            this.importPktDialog.Filter = "License System Data|*.lsData";
            this.importPktDialog.Title = "License System Data";
            this.importPktDialog.InitialDirectory = m_selectedDirectory;
            if (importPktDialog.ShowDialog() == DialogResult.OK)
            {
                m_selectedDirectory = System.IO.Path.GetDirectoryName(this.importPktDialog.FileName);
                SolimarLicenseViewer.Form1 newForm = new Form1();
                //Extract all the bytes out of this.importPktDialog.FileName
                newForm.setDiagnosticData(m_CommLink.ServerName, System.IO.File.ReadAllBytes(this.importPktDialog.FileName), this.importPktDialog.FileName);
                newForm.setAsChildForm(true, this);
                newForm.Show();
            }
        }
        public void setAsChildForm(bool _bTreatAsChildForm, object _sender)
        {
            m_bIsChildForm = _bTreatAsChildForm;
            if (m_CommLink.bDiagnosticDateView)
            {
                fileLicenseToolStripMenuItem.Visible = false;
                viewToolStripMenuItem.Visible = false;
                diagnosticDataToolStripMenuItem.Visible = false;

            }
            loadDiagnosticDataToolStripMenuItem.Visible = !m_bIsChildForm;
            remoteServerToolStripMenuItem.Visible = !m_bIsChildForm;
            solimarLicenseManualToolStripMenuItem.Visible = !m_bIsChildForm;
            helpToolStripSepMenuItem1.Visible = !m_bIsChildForm;
            this.ShowInTaskbar = !m_bIsChildForm;
            this.parentForm = _sender as Form;
            this.MinimizeBox = false;
            this.MaximizeBox = false;
            //this.Shown += new System.EventHandler(this.ConnectMenuItem_Click);
        }
        public void setChildInformation(object _sender, string _serverName, bool bRemoteServer)
        {
            ToolStripMenuItem tsItem = new ToolStripMenuItem(_serverName);
            tsItem.Tag = _sender;
            tsItem.Name = tsItem.Text;
            tsItem.TextAlign = ContentAlignment.MiddleLeft;
            tsItem.Click += new EventHandler(FindRemoteLicenseViewUI_Click);

            string[] strList = _serverName.Split('\\');
            ToolStripMenuItem tsItem2 = new ToolStripMenuItem(strList[strList.Length - 1])
            {
                Name = _serverName,
                Tag = _sender,
                TextAlign = ContentAlignment.MiddleLeft
            };
            tsItem2.Click += new EventHandler(FindRemoteLicenseViewUI_Click);

            if (bRemoteServer)
            {
                remoteServerToolStripMenuItem.DropDownItems.Add(tsItem);
                remoteConnectToolStripMenuSeparator.Visible = remoteServerToolStripMenuItem.DropDownItems.Count > 2;

                tsItem2.ToolTipText = string.Format("Goto Solimar License Viewer on \'{0}\'", _serverName);
                remoteSvrToolStrip.Items.Add(tsItem2);
                remoteSvrToolStrip.Visible = true;
            }
            else
            {
                diagnosticDataToolStripMenuItem.DropDownItems.Add(tsItem);
                diagnosticDataToolStripMenuSeparator.Visible = diagnosticDataToolStripMenuItem.DropDownItems.Count > 3;

                tsItem2.ToolTipText = string.Format("Goto \'{0}\'", _serverName);
                diagnosticDataToolStrip.Items.Add(tsItem2);
                diagnosticDataToolStrip.Visible = true;
            }
        }
        public void childFormClose(object _sender, string _serverName)
        {
            RemoveItemFromToolStripCollection(_sender, remoteServerToolStripMenuItem.DropDownItems);
            RemoveItemFromToolStripCollection(_sender, diagnosticDataToolStripMenuItem.DropDownItems);
            RemoveItemFromToolStripCollection(_sender, remoteSvrToolStrip.Items);
            RemoveItemFromToolStripCollection(_sender, diagnosticDataToolStrip.Items);

            remoteConnectToolStripMenuSeparator.Visible = remoteServerToolStripMenuItem.DropDownItems.Count > 2;
            diagnosticDataToolStripMenuSeparator.Visible = diagnosticDataToolStripMenuItem.DropDownItems.Count > 3;
            remoteSvrToolStrip.Visible = remoteSvrToolStrip.Items.Count > 2;
            diagnosticDataToolStrip.Visible = diagnosticDataToolStrip.Items.Count > 2;
        }
        public void RemoveItemFromToolStripCollection(object _tag, ToolStripItemCollection _tsItemCollection)
        {
            ToolStripMenuItem removeTsItem = null;
            for (int idx = 0; idx < _tsItemCollection.Count; idx++)
            {
                if (_tsItemCollection[idx] is ToolStripMenuItem &&
                    _tsItemCollection[idx].Tag == _tag)
                {
                    removeTsItem = _tsItemCollection[idx] as ToolStripMenuItem;
                    break;
                }
            }
            if (removeTsItem != null)
                _tsItemCollection.Remove(removeTsItem);
        }

        //Use this to view the license info diagnostic data
        public void setDiagnosticData(string _serverName, Byte[] _diagnosticDataByteArray, string _diagnosticDataFileName)
        {
            m_CommLink.ServerName = _serverName;
            m_CommLink.SetData(_diagnosticDataByteArray);
            m_diagnosticDataFileName = _diagnosticDataFileName;
            m_bViewDiagnosticData = true;
        }
        private bool m_bViewDiagnosticData = false;
        private string m_diagnosticDataFileName = "";

        void FindRemoteLicenseViewUI_Click(object sender, EventArgs e)
        {
            if (sender is ToolStripMenuItem)
            {
                if ((sender as ToolStripMenuItem).Tag != null)
                    (((sender as ToolStripMenuItem).Tag) as Form1).Focus();

            }
        }
        private Form parentForm = null;

        #region Background Worker - testConnBackgroundWorker
        private void TestConnection(IList<ListViewItem> _lviList)
        {
            this.UseWaitCursor = true;
            try
            {
                if (messageDialog == null)
                    messageDialog = new Shared.VisualComponents.BaseMessageDialog();

                messageDialog.Show();
                messageDialog.SetData(new Shared.VisualComponents.MessageBoxData());
                messageDialog.SetMessage("Testing Product Connection");
                messageDialog.Update();
                testConnBackgroundWorker.RunWorkerAsync(_lviList);
            }
            catch (Exception)
            {
                this.UseWaitCursor = false;
            }
        }

        private void testConnBackgroundWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            IList<ListViewItem> lviList = e.Argument as IList<ListViewItem>;
            if (lviList != null && lviList.Count > 0)
            {
                foreach (ListViewItem lvItem in lviList)
                {
                    testConnBackgroundWorker.ReportProgress(0, lvItem);
                    Exception lastEx = null;
                    try
                    {
                        testConnectionToLicenseServer(new ConnectionSettings2()
                            {
                                ProductID = (int)lvItem.Tag,
                                ServerName = lvItem.SubItems[1].Text,
                                BackupName = lvItem.SubItems[2].Text,
                                UseDevelopmentLic = (System.String.Compare(lvItem.SubItems[3].Text, "true", true) == 0),
                            });
                    }
                    catch (Exception ex)
                    {
                        lastEx = ex;
                    }
                    testConnBackgroundWorker.ReportProgress(0, new Solimar.Licensing.Common.Collections.Tuple<ListViewItem, Exception>(lvItem, lastEx));
                }
            }
            e.Result = lviList;
        }
        // throws an exception if fails to connect...
        private void testConnectionToLicenseServer(ConnectionSettings2 _connectionSettings)
        {
            if (_connectionSettings != null)
            {
                using (Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseWrapper licWrapper = new Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseWrapper())
                {
                    Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID productID = (Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID)_connectionSettings.ProductID;
                    licWrapper.ConnectByProductEx((int)productID, false);
                    try
                    {
                        licWrapper.InitializeEx(System.Environment.MachineName.ToLower(),   //application_instance
                                                (int)productID, //product
                                                0,              //prod_ver_major
                                                1,              //prod_ver_minor - CR.FIX.12672 - Use version 0.1 instead of 0.0, 0.0 is not valid
                                                false,          //single_key
                                                "",             //specific_single_key_ident
                                                false,          //lock_keys
                                                0,              //ui_level
                                                0,              //grace_period_minutes
                                                false,          //application_instance_lock_keys
                                                false);         //bypass_remote_key_restriction
                    }
                    catch (Exception)
                    {
                        throw;
                    }
                    finally
                    {
                        licWrapper.DisconnectEx();
                    }
                }
            }
        }

        private void testConnBackgroundWorker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            if (this.InvokeRequired)
            {
                this.Invoke(new System.ComponentModel.ProgressChangedEventHandler(testConnBackgroundWorker_ProgressChanged), new object[] { sender, e });
            }
            else
            {
                //Update UI
                #region if (e.UserState is Solimar.Licensing.Common.Collections.Tuple<ListViewItem, Exception>)
                if (e.UserState is Solimar.Licensing.Common.Collections.Tuple<ListViewItem, Exception>)
                {
                    Solimar.Licensing.Common.Collections.Tuple<ListViewItem, Exception> updateItem = e.UserState as Solimar.Licensing.Common.Collections.Tuple<ListViewItem, Exception>;
                    try
                    {
                        updateItem.Item1.ListView.BeginUpdate();
                        if (updateItem.Item2 == null)
                        {
                            updateItem.Item1.ForeColor = System.Drawing.Color.Green;
                            updateItem.Item1.SubItems[4].Text = "Successfully connected to the License Server";
                            updateItem.Item1.ToolTipText = string.Format("Connection Status: {0}", updateItem.Item1.SubItems[4].Text);
                            messageDialog.SetMessage(string.Format("Product: {0}, License Server: {1}, {2}", updateItem.Item1.SubItems[0].Text, updateItem.Item1.SubItems[1].Text, updateItem.Item1.ToolTipText));
                        }
                        else
                        {
                            updateItem.Item1.ForeColor = System.Drawing.Color.Red;
                            updateItem.Item1.SubItems[4].Text = updateItem.Item2.Message.Replace('\r', ' ').Replace('\n', ' ');
                            updateItem.Item1.ToolTipText = string.Format("Connection Error: {0}", updateItem.Item1.SubItems[4].Text);
                            messageDialog.SetMessage(updateItem.Item1.ToolTipText);
                        }

                        messageDialog.Update();
                        Shared.VisualComponents.ListViewHelper.ResizeListViewHeadersToMaxOfDataAndHeader(updateItem.Item1.ListView);
                    }
                    catch (Exception)
                    {
                    }
                    finally
                    {
                        updateItem.Item1.ListView.EndUpdate();
                    }
                }
                #endregion
                #region else if (e.UserState is ListViewItem)
                else if (e.UserState is ListViewItem)
                {
                    ListViewItem lvi = e.UserState as ListViewItem;
                    try
                    {
                        messageDialog.SetMessage(string.Format("Product: {0}, trying to connect to License Server: {1}", lvi.SubItems[0].Text, lvi.SubItems[1].Text));
                    }
                    catch (Exception)
                    {
                    }
                }
                #endregion
            }
        }

        private void testConnBackgroundWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (this.InvokeRequired)
            {
                this.Invoke(new System.ComponentModel.RunWorkerCompletedEventHandler(testConnBackgroundWorker_RunWorkerCompleted), new object[] { sender, e });
            }
            else
            {
                if(messageDialog != null)
                    messageDialog.Hide();
                this.UseWaitCursor = false;
            }
        }
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

            string innerExMsg = GetMessageFromInnerException(paramException);
            if (!string.IsNullOrEmpty(innerExMsg))
                errBuilder.AppendFormat(" - {0}", innerExMsg);

            MessageBox.Show(paramParentForm,
                            errBuilder.ToString(),
                            paramCaptionString,
                            MessageBoxButtons.OK,
                            MessageBoxIcon.Error);
        }
        protected static string GetMessageFromInnerException(System.Exception paramException)
        {
            StringBuilder sBuilder = new StringBuilder();
            if (paramException != null)
            {
                sBuilder.Append((paramException.InnerException != null) ? paramException.InnerException.Message : string.Empty);
                string innerExMsg = GetMessageFromInnerException(paramException.InnerException);
                if (!string.IsNullOrEmpty(innerExMsg))
                    sBuilder.AppendFormat(" - {0}", innerExMsg);
            }
            return sBuilder.ToString();
        }
    }
}