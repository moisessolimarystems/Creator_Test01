namespace SolimarLicenseViewer
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.ConnectionStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.useActivationOnLicenseDropDownMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.formSplitContainer = new System.Windows.Forms.SplitContainer();
            this.treeView = new System.Windows.Forms.TreeView();
            this.productIconList = new System.Windows.Forms.ImageList(this.components);
            this.infoSplitContainer = new System.Windows.Forms.SplitContainer();
            this.noFlickerListView = new Shared.VisualComponents.NoFlickerListView();
            this.lvContextMenuStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.copyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.extendToolStripSeparator = new System.Windows.Forms.ToolStripSeparator();
            this.extendToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editConnToolStripSeparator = new System.Windows.Forms.ToolStripSeparator();
            this.editConnToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.testConnToolStripSeparator = new System.Windows.Forms.ToolStripSeparator();
            this.testConnSelToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.testConnToAllToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.lvToolStrip = new System.Windows.Forms.ToolStrip();
            this.bottomNoFlickerListView = new Shared.VisualComponents.NoFlickerListView();
            this.bottomLvToolStrip = new System.Windows.Forms.ToolStrip();
            this.importPktDialog = new System.Windows.Forms.OpenFileDialog();
            this.exportPktDialog = new System.Windows.Forms.SaveFileDialog();
            this.folderBrowserDialog1 = new System.Windows.Forms.FolderBrowserDialog();
            this.FileMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.refreshToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileLicenseToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.enterPasswordToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.enterPasswordPacketToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.sep5ToolStripMenuItem = new System.Windows.Forms.ToolStripSeparator();
            this.importToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.updateLicToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exportToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.sep1ToolStripMenuItem = new System.Windows.Forms.ToolStripSeparator();
            this.genVerificationOnLicenseDropDownMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.genDiagnosticDataDropDownMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.sep2ToolStripMenuItem = new System.Windows.Forms.ToolStripSeparator();
            this.sep3ToolStripMenuItem = new System.Windows.Forms.ToolStripSeparator();
            this.genLicArchiveOnLicenseDropDownMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.importLicArchiveOnLicenseDropDownMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.diagnosticDataToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.generateLicenseDiagnosticDataForSolimarToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadDiagnosticDataToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.diagnosticDataToolStripMenuSeparator = new System.Windows.Forms.ToolStripSeparator();
            this.remoteServerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.remoteConnectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.remoteConnectToolStripMenuSeparator = new System.Windows.Forms.ToolStripSeparator();
            this.generalToolTip = new System.Windows.Forms.ToolTip(this.components);
            this.statusStrip1.SuspendLayout();
            this.formSplitContainer.Panel1.SuspendLayout();
            this.formSplitContainer.Panel2.SuspendLayout();
            this.formSplitContainer.SuspendLayout();
            this.infoSplitContainer.Panel1.SuspendLayout();
            this.infoSplitContainer.Panel2.SuspendLayout();
            this.infoSplitContainer.SuspendLayout();
            this.lvContextMenuStrip.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ConnectionStatusLabel});
            this.statusStrip1.Location = new System.Drawing.Point(0, 373);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(655, 22);
            this.statusStrip1.TabIndex = 1;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // ConnectionStatusLabel
            // 
            this.ConnectionStatusLabel.Name = "ConnectionStatusLabel";
            this.ConnectionStatusLabel.Size = new System.Drawing.Size(79, 17);
            this.ConnectionStatusLabel.Text = "Disconnected";
            // 
            // useActivationOnLicenseDropDownMenuItem
            // 
            this.useActivationOnLicenseDropDownMenuItem.Name = "useActivationOnLicenseDropDownMenuItem";
            this.useActivationOnLicenseDropDownMenuItem.Size = new System.Drawing.Size(319, 22);
            this.useActivationOnLicenseDropDownMenuItem.Text = "Use Activation on License...";
            // 
            // formSplitContainer
            // 
            this.formSplitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.formSplitContainer.Location = new System.Drawing.Point(0, 24);
            this.formSplitContainer.Name = "formSplitContainer";
            // 
            // formSplitContainer.Panel1
            // 
            this.formSplitContainer.Panel1.Controls.Add(this.treeView);
            // 
            // formSplitContainer.Panel2
            // 
            this.formSplitContainer.Panel2.Controls.Add(this.infoSplitContainer);
            this.formSplitContainer.Size = new System.Drawing.Size(655, 349);
            this.formSplitContainer.SplitterDistance = 188;
            this.formSplitContainer.TabIndex = 3;
            // 
            // treeView
            // 
            this.treeView.BackColor = System.Drawing.SystemColors.Window;
            this.treeView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.treeView.HideSelection = false;
            this.treeView.ImageIndex = 0;
            this.treeView.ImageList = this.productIconList;
            this.treeView.Location = new System.Drawing.Point(0, 0);
            this.treeView.Name = "treeView";
            this.treeView.SelectedImageIndex = 0;
            this.treeView.ShowNodeToolTips = true;
            this.treeView.Size = new System.Drawing.Size(188, 349);
            this.treeView.TabIndex = 0;
            this.treeView.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.treeView_AfterSelect);
            // 
            // productIconList
            // 
            this.productIconList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("productIconList.ImageStream")));
            this.productIconList.TransparentColor = System.Drawing.Color.Transparent;
            this.productIconList.Images.SetKeyName(0, "SPDNT.ICO");
            this.productIconList.Images.SetKeyName(1, "iconvert16.png");
            this.productIconList.Images.SetKeyName(2, "rubika16.png");
            this.productIconList.Images.SetKeyName(3, "sdx16.png");
            this.productIconList.Images.SetKeyName(4, "SOLenterprise32.ico");
            this.productIconList.Images.SetKeyName(5, "solfusion32.ico");
            this.productIconList.Images.SetKeyName(6, "solscript16.png");
            this.productIconList.Images.SetKeyName(7, "spde16.png");
            this.productIconList.Images.SetKeyName(8, "spde16.png");
            this.productIconList.Images.SetKeyName(9, "Queue Manager16.png");
            this.productIconList.Images.SetKeyName(10, "Licenses.ico");
            this.productIconList.Images.SetKeyName(11, "LicenseID.ico");
            this.productIconList.Images.SetKeyName(12, "AppInstance.ico");
            this.productIconList.Images.SetKeyName(13, "History.ico");
            this.productIconList.Images.SetKeyName(14, "Empty.ico");
            this.productIconList.Images.SetKeyName(15, "protectionKeys.ico");
            this.productIconList.Images.SetKeyName(16, "protectionKey.ico");
            this.productIconList.Images.SetKeyName(17, "Rubika Process Builder16.png");
            this.productIconList.Images.SetKeyName(18, "solindexer16.png");
            this.productIconList.Images.SetKeyName(19, "Usage.ico");
            this.productIconList.Images.SetKeyName(20, "Product Connection Settings.ico");
            this.productIconList.Images.SetKeyName(21, "solitrack16.png");
            this.productIconList.Images.SetKeyName(22, "SOLenterpriseSP32.ico");
            // 
            // infoSplitContainer
            // 
            this.infoSplitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.infoSplitContainer.Location = new System.Drawing.Point(0, 0);
            this.infoSplitContainer.Name = "infoSplitContainer";
            this.infoSplitContainer.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // infoSplitContainer.Panel1
            // 
            this.infoSplitContainer.Panel1.Controls.Add(this.noFlickerListView);
            this.infoSplitContainer.Panel1.Controls.Add(this.lvToolStrip);
            // 
            // infoSplitContainer.Panel2
            // 
            this.infoSplitContainer.Panel2.Controls.Add(this.bottomNoFlickerListView);
            this.infoSplitContainer.Panel2.Controls.Add(this.bottomLvToolStrip);
            this.infoSplitContainer.Panel2Collapsed = true;
            this.infoSplitContainer.Size = new System.Drawing.Size(463, 349);
            this.infoSplitContainer.SplitterDistance = 104;
            this.infoSplitContainer.TabIndex = 2;
            // 
            // noFlickerListView
            // 
            this.noFlickerListView.ContextMenuStrip = this.lvContextMenuStrip;
            this.noFlickerListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.noFlickerListView.FullRowSelect = true;
            this.noFlickerListView.HideSelection = false;
            this.noFlickerListView.Location = new System.Drawing.Point(0, 0);
            this.noFlickerListView.Name = "noFlickerListView";
            this.noFlickerListView.ShowItemToolTips = true;
            this.noFlickerListView.Size = new System.Drawing.Size(463, 349);
            this.noFlickerListView.Sorting = System.Windows.Forms.SortOrder.Ascending;
            this.noFlickerListView.TabIndex = 3;
            this.noFlickerListView.UseCompatibleStateImageBehavior = false;
            this.noFlickerListView.View = System.Windows.Forms.View.Details;
            this.noFlickerListView.ColumnClick += new System.Windows.Forms.ColumnClickEventHandler(this.listView_ColumnClick);
            this.noFlickerListView.KeyDown += new System.Windows.Forms.KeyEventHandler(this.General_KeyDown);
            // 
            // lvContextMenuStrip
            // 
            this.lvContextMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.copyToolStripMenuItem,
            this.extendToolStripSeparator,
            this.extendToolStripMenuItem,
            this.editConnToolStripSeparator,
            this.editConnToolStripMenuItem,
            this.testConnToolStripSeparator,
            this.testConnSelToolStripMenuItem,
            this.testConnToAllToolStripMenuItem});
            this.lvContextMenuStrip.Name = "lvContextMenuStrip";
            this.lvContextMenuStrip.Size = new System.Drawing.Size(253, 132);
            this.lvContextMenuStrip.Opening += new System.ComponentModel.CancelEventHandler(this.lvContextMenuStrip_Opening);
            // 
            // copyToolStripMenuItem
            // 
            this.copyToolStripMenuItem.Name = "copyToolStripMenuItem";
            this.copyToolStripMenuItem.Size = new System.Drawing.Size(252, 22);
            this.copyToolStripMenuItem.Text = "&Copy";
            this.copyToolStripMenuItem.Click += new System.EventHandler(this.copyToolStripMenuItem_Click);
            // 
            // extendToolStripSeparator
            // 
            this.extendToolStripSeparator.Name = "extendToolStripSeparator";
            this.extendToolStripSeparator.Size = new System.Drawing.Size(249, 6);
            // 
            // extendToolStripMenuItem
            // 
            this.extendToolStripMenuItem.Name = "extendToolStripMenuItem";
            this.extendToolStripMenuItem.Size = new System.Drawing.Size(252, 22);
            this.extendToolStripMenuItem.Text = "Use Activation on License...";
            this.extendToolStripMenuItem.Click += new System.EventHandler(this.extendToolStripMenuItem_Click);
            // 
            // editConnToolStripSeparator
            // 
            this.editConnToolStripSeparator.Name = "editConnToolStripSeparator";
            this.editConnToolStripSeparator.Size = new System.Drawing.Size(249, 6);
            // 
            // editConnToolStripMenuItem
            // 
            this.editConnToolStripMenuItem.Image = global::SolimarLicenseViewer.Properties.Resources.editLicense;
            this.editConnToolStripMenuItem.Name = "editConnToolStripMenuItem";
            this.editConnToolStripMenuItem.Size = new System.Drawing.Size(252, 22);
            this.editConnToolStripMenuItem.Text = "Edit Connection Settings";
            this.editConnToolStripMenuItem.ToolTipText = "Edit Connection Settings for the given Product";
            this.editConnToolStripMenuItem.Click += new System.EventHandler(this.editConnToolStripMenuItem_Click);
            // 
            // testConnToolStripSeparator
            // 
            this.testConnToolStripSeparator.Name = "testConnToolStripSeparator";
            this.testConnToolStripSeparator.Size = new System.Drawing.Size(249, 6);
            // 
            // testConnSelToolStripMenuItem
            // 
            this.testConnSelToolStripMenuItem.Image = global::SolimarLicenseViewer.Properties.Resources.licenseTest;
            this.testConnSelToolStripMenuItem.Name = "testConnSelToolStripMenuItem";
            this.testConnSelToolStripMenuItem.Size = new System.Drawing.Size(252, 22);
            this.testConnSelToolStripMenuItem.Text = "Test Connection to License Server";
            this.testConnSelToolStripMenuItem.ToolTipText = "Test Connection to License Server";
            this.testConnSelToolStripMenuItem.Click += new System.EventHandler(this.testConnSelToolStripMenuItem_Click);
            // 
            // testConnToAllToolStripMenuItem
            // 
            this.testConnToAllToolStripMenuItem.Image = global::SolimarLicenseViewer.Properties.Resources.licenseTestAll;
            this.testConnToAllToolStripMenuItem.Name = "testConnToAllToolStripMenuItem";
            this.testConnToAllToolStripMenuItem.Size = new System.Drawing.Size(252, 22);
            this.testConnToAllToolStripMenuItem.Text = "Test All Connection Settings";
            this.testConnToAllToolStripMenuItem.ToolTipText = "Test Connection to All License Server";
            this.testConnToAllToolStripMenuItem.Click += new System.EventHandler(this.testConnToAllToolStripMenuItem_Click);
            // 
            // lvToolStrip
            // 
            this.lvToolStrip.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.lvToolStrip.Location = new System.Drawing.Point(0, 0);
            this.lvToolStrip.Name = "lvToolStrip";
            this.lvToolStrip.Size = new System.Drawing.Size(463, 25);
            this.lvToolStrip.TabIndex = 2;
            this.lvToolStrip.Text = "toolStrip2";
            this.lvToolStrip.Visible = false;
            // 
            // bottomNoFlickerListView
            // 
            this.bottomNoFlickerListView.ContextMenuStrip = this.lvContextMenuStrip;
            this.bottomNoFlickerListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.bottomNoFlickerListView.FullRowSelect = true;
            this.bottomNoFlickerListView.HideSelection = false;
            this.bottomNoFlickerListView.Location = new System.Drawing.Point(0, 0);
            this.bottomNoFlickerListView.Name = "bottomNoFlickerListView";
            this.bottomNoFlickerListView.ShowItemToolTips = true;
            this.bottomNoFlickerListView.Size = new System.Drawing.Size(150, 46);
            this.bottomNoFlickerListView.Sorting = System.Windows.Forms.SortOrder.Ascending;
            this.bottomNoFlickerListView.TabIndex = 4;
            this.bottomNoFlickerListView.UseCompatibleStateImageBehavior = false;
            this.bottomNoFlickerListView.View = System.Windows.Forms.View.Details;
            this.bottomNoFlickerListView.ColumnClick += new System.Windows.Forms.ColumnClickEventHandler(this.listView_ColumnClick);
            this.bottomNoFlickerListView.KeyDown += new System.Windows.Forms.KeyEventHandler(this.General_KeyDown);
            // 
            // bottomLvToolStrip
            // 
            this.bottomLvToolStrip.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.bottomLvToolStrip.Location = new System.Drawing.Point(0, 0);
            this.bottomLvToolStrip.Name = "bottomLvToolStrip";
            this.bottomLvToolStrip.Size = new System.Drawing.Size(150, 25);
            this.bottomLvToolStrip.TabIndex = 5;
            this.bottomLvToolStrip.Text = "toolStrip1";
            this.bottomLvToolStrip.Visible = false;
            // 
            // importPktDialog
            // 
            this.importPktDialog.DefaultExt = "packet";
            this.importPktDialog.Filter = "License Packet|*.packet";
            this.importPktDialog.Title = "Import License Packet";
            // 
            // exportPktDialog
            // 
            this.exportPktDialog.DefaultExt = "packet";
            this.exportPktDialog.Filter = "License Packet|*.packet";
            this.exportPktDialog.Title = "Export License Packet";
            // 
            // FileMenu
            // 
            this.FileMenu.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.exitToolStripMenuItem});
            this.FileMenu.Name = "FileMenu";
            this.FileMenu.Size = new System.Drawing.Size(37, 20);
            this.FileMenu.Text = "File";
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(92, 22);
            this.exitToolStripMenuItem.Text = "E&xit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // viewToolStripMenuItem
            // 
            this.viewToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.refreshToolStripMenuItem});
            this.viewToolStripMenuItem.Name = "viewToolStripMenuItem";
            this.viewToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.viewToolStripMenuItem.Text = "View";
            // 
            // refreshToolStripMenuItem
            // 
            this.refreshToolStripMenuItem.Name = "refreshToolStripMenuItem";
            this.refreshToolStripMenuItem.Size = new System.Drawing.Size(113, 22);
            this.refreshToolStripMenuItem.Text = "Refresh";
            this.refreshToolStripMenuItem.Click += new System.EventHandler(this.refreshToolStripButton_Click);
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.helpToolStripMenuItem.Text = "Help";
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(107, 22);
            this.aboutToolStripMenuItem.Text = "About";
            this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.FileMenu,
            this.fileLicenseToolStripMenuItem,
            this.viewToolStripMenuItem,
            this.diagnosticDataToolStripMenuItem,
            this.remoteServerToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(655, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileLicenseToolStripMenuItem
            // 
            this.fileLicenseToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.enterPasswordToolStripMenuItem,
            this.enterPasswordPacketToolStripMenuItem,
            this.sep5ToolStripMenuItem,
            this.importToolStripMenuItem,
            this.updateLicToolStripMenuItem,
            this.exportToolStripMenuItem,
            this.sep1ToolStripMenuItem,
            this.genVerificationOnLicenseDropDownMenuItem,
            this.genDiagnosticDataDropDownMenuItem,
            this.sep2ToolStripMenuItem,
            this.useActivationOnLicenseDropDownMenuItem,
            this.sep3ToolStripMenuItem,
            this.genLicArchiveOnLicenseDropDownMenuItem,
            this.importLicArchiveOnLicenseDropDownMenuItem});
            this.fileLicenseToolStripMenuItem.Name = "fileLicenseToolStripMenuItem";
            this.fileLicenseToolStripMenuItem.Size = new System.Drawing.Size(58, 20);
            this.fileLicenseToolStripMenuItem.Text = "License";
            this.fileLicenseToolStripMenuItem.DropDownOpening += new System.EventHandler(this.licenseToolStripButton_DropDownOpening);
            // 
            // enterPasswordToolStripMenuItem
            // 
            this.enterPasswordToolStripMenuItem.Name = "enterPasswordToolStripMenuItem";
            this.enterPasswordToolStripMenuItem.Size = new System.Drawing.Size(319, 22);
            this.enterPasswordToolStripMenuItem.Text = "Enter Password for Protection Key...";
            this.enterPasswordToolStripMenuItem.Click += new System.EventHandler(this.enterPasswordToolStripMenuItem_Click);
            // 
            // enterPasswordPacketToolStripMenuItem
            // 
            this.enterPasswordPacketToolStripMenuItem.Name = "enterPasswordPacketToolStripMenuItem";
            this.enterPasswordPacketToolStripMenuItem.Size = new System.Drawing.Size(319, 22);
            this.enterPasswordPacketToolStripMenuItem.Text = "Enter Password Packet for Protection Key...";
            this.enterPasswordPacketToolStripMenuItem.Click += new System.EventHandler(this.enterPasswordPacketToolStripMenuItem_Click);
            // 
            // sep5ToolStripMenuItem
            // 
            this.sep5ToolStripMenuItem.Name = "sep5ToolStripMenuItem";
            this.sep5ToolStripMenuItem.Size = new System.Drawing.Size(316, 6);
            // 
            // importToolStripMenuItem
            // 
            this.importToolStripMenuItem.Name = "importToolStripMenuItem";
            this.importToolStripMenuItem.Size = new System.Drawing.Size(319, 22);
            this.importToolStripMenuItem.Text = "Import new License from License Packet...";
            this.importToolStripMenuItem.Click += new System.EventHandler(this.importToolStripMenuItem_Click);
            // 
            // updateLicToolStripMenuItem
            // 
            this.updateLicToolStripMenuItem.Name = "updateLicToolStripMenuItem";
            this.updateLicToolStripMenuItem.Size = new System.Drawing.Size(319, 22);
            this.updateLicToolStripMenuItem.Text = "Update existing License from License Packet...";
            this.updateLicToolStripMenuItem.Click += new System.EventHandler(this.importToolStripMenuItem_Click);
            // 
            // exportToolStripMenuItem
            // 
            this.exportToolStripMenuItem.Name = "exportToolStripMenuItem";
            this.exportToolStripMenuItem.Size = new System.Drawing.Size(319, 22);
            this.exportToolStripMenuItem.Text = "Export...";
            this.exportToolStripMenuItem.Visible = false;
            this.exportToolStripMenuItem.Click += new System.EventHandler(this.exportToolStripMenuItem_Click);
            // 
            // sep1ToolStripMenuItem
            // 
            this.sep1ToolStripMenuItem.Name = "sep1ToolStripMenuItem";
            this.sep1ToolStripMenuItem.Size = new System.Drawing.Size(316, 6);
            // 
            // genVerificationOnLicenseDropDownMenuItem
            // 
            this.genVerificationOnLicenseDropDownMenuItem.Name = "genVerificationOnLicenseDropDownMenuItem";
            this.genVerificationOnLicenseDropDownMenuItem.Size = new System.Drawing.Size(319, 22);
            this.genVerificationOnLicenseDropDownMenuItem.Text = "Generate Verification Data for Solimar...";
            this.genVerificationOnLicenseDropDownMenuItem.Visible = false;
            // 
            // genDiagnosticDataDropDownMenuItem
            // 
            this.genDiagnosticDataDropDownMenuItem.Name = "genDiagnosticDataDropDownMenuItem";
            this.genDiagnosticDataDropDownMenuItem.Size = new System.Drawing.Size(319, 22);
            this.genDiagnosticDataDropDownMenuItem.Text = "Generate License Diagnostic Data for Solimar...";
            this.genDiagnosticDataDropDownMenuItem.Visible = false;
            this.genDiagnosticDataDropDownMenuItem.Click += new System.EventHandler(this.genDiagnosticDataDropDownMenuItem_Click);
            // 
            // sep2ToolStripMenuItem
            // 
            this.sep2ToolStripMenuItem.Name = "sep2ToolStripMenuItem";
            this.sep2ToolStripMenuItem.Size = new System.Drawing.Size(316, 6);
            this.sep2ToolStripMenuItem.Visible = false;
            // 
            // sep3ToolStripMenuItem
            // 
            this.sep3ToolStripMenuItem.Name = "sep3ToolStripMenuItem";
            this.sep3ToolStripMenuItem.Size = new System.Drawing.Size(316, 6);
            // 
            // genLicArchiveOnLicenseDropDownMenuItem
            // 
            this.genLicArchiveOnLicenseDropDownMenuItem.Name = "genLicArchiveOnLicenseDropDownMenuItem";
            this.genLicArchiveOnLicenseDropDownMenuItem.Size = new System.Drawing.Size(319, 22);
            this.genLicArchiveOnLicenseDropDownMenuItem.Text = "Transfer existing License to License Archive...";
            this.genLicArchiveOnLicenseDropDownMenuItem.ToolTipText = "Generate License Archive to transfer to another computer";
            // 
            // importLicArchiveOnLicenseDropDownMenuItem
            // 
            this.importLicArchiveOnLicenseDropDownMenuItem.Name = "importLicArchiveOnLicenseDropDownMenuItem";
            this.importLicArchiveOnLicenseDropDownMenuItem.Size = new System.Drawing.Size(319, 22);
            this.importLicArchiveOnLicenseDropDownMenuItem.Text = "Import new License from License Archive...";
            this.importLicArchiveOnLicenseDropDownMenuItem.ToolTipText = "Import License Archive from another computer";
            this.importLicArchiveOnLicenseDropDownMenuItem.Click += new System.EventHandler(this.importLicArchiveOnLicenseDropDownMenuItem_Click);
            // 
            // diagnosticDataToolStripMenuItem
            // 
            this.diagnosticDataToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.generateLicenseDiagnosticDataForSolimarToolStripMenuItem,
            this.loadDiagnosticDataToolStripMenuItem,
            this.diagnosticDataToolStripMenuSeparator});
            this.diagnosticDataToolStripMenuItem.Name = "diagnosticDataToolStripMenuItem";
            this.diagnosticDataToolStripMenuItem.Size = new System.Drawing.Size(102, 20);
            this.diagnosticDataToolStripMenuItem.Text = "Diagnostic Data";
            // 
            // generateLicenseDiagnosticDataForSolimarToolStripMenuItem
            // 
            this.generateLicenseDiagnosticDataForSolimarToolStripMenuItem.Name = "generateLicenseDiagnosticDataForSolimarToolStripMenuItem";
            this.generateLicenseDiagnosticDataForSolimarToolStripMenuItem.Size = new System.Drawing.Size(319, 22);
            this.generateLicenseDiagnosticDataForSolimarToolStripMenuItem.Text = "Generate License Diagnostic Data for Solimar...";
            this.generateLicenseDiagnosticDataForSolimarToolStripMenuItem.Click += new System.EventHandler(this.genDiagnosticDataDropDownMenuItem_Click);
            // 
            // loadDiagnosticDataToolStripMenuItem
            // 
            this.loadDiagnosticDataToolStripMenuItem.Name = "loadDiagnosticDataToolStripMenuItem";
            this.loadDiagnosticDataToolStripMenuItem.Size = new System.Drawing.Size(319, 22);
            this.loadDiagnosticDataToolStripMenuItem.Text = "Load Diagnostic Data...";
            this.loadDiagnosticDataToolStripMenuItem.Click += new System.EventHandler(this.loadDiagnosticDataToolStripMenuItem_Click);
            // 
            // diagnosticDataToolStripMenuSeparator
            // 
            this.diagnosticDataToolStripMenuSeparator.Name = "diagnosticDataToolStripMenuSeparator";
            this.diagnosticDataToolStripMenuSeparator.Size = new System.Drawing.Size(316, 6);
            this.diagnosticDataToolStripMenuSeparator.Visible = false;
            // 
            // remoteServerToolStripMenuItem
            // 
            this.remoteServerToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.remoteConnectToolStripMenuItem,
            this.remoteConnectToolStripMenuSeparator});
            this.remoteServerToolStripMenuItem.Name = "remoteServerToolStripMenuItem";
            this.remoteServerToolStripMenuItem.Size = new System.Drawing.Size(95, 20);
            this.remoteServerToolStripMenuItem.Text = "Remote Server";
            // 
            // remoteConnectToolStripMenuItem
            // 
            this.remoteConnectToolStripMenuItem.Name = "remoteConnectToolStripMenuItem";
            this.remoteConnectToolStripMenuItem.Size = new System.Drawing.Size(128, 22);
            this.remoteConnectToolStripMenuItem.Text = "Connect...";
            this.remoteConnectToolStripMenuItem.ToolTipText = "Connect to a Remote License Server";
            this.remoteConnectToolStripMenuItem.Click += new System.EventHandler(this.remoteConnectToolStripMenuItem_Click);
            // 
            // remoteConnectToolStripMenuSeparator
            // 
            this.remoteConnectToolStripMenuSeparator.Name = "remoteConnectToolStripMenuSeparator";
            this.remoteConnectToolStripMenuSeparator.Size = new System.Drawing.Size(125, 6);
            this.remoteConnectToolStripMenuSeparator.Visible = false;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.ClientSize = new System.Drawing.Size(655, 395);
            this.Controls.Add(this.formSplitContainer);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.menuStrip1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Solimar License Viewer [Disconnected]";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.Shown += new System.EventHandler(this.ConnectMenuItem_Click);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.formSplitContainer.Panel1.ResumeLayout(false);
            this.formSplitContainer.Panel2.ResumeLayout(false);
            this.formSplitContainer.ResumeLayout(false);
            this.infoSplitContainer.Panel1.ResumeLayout(false);
            this.infoSplitContainer.Panel1.PerformLayout();
            this.infoSplitContainer.Panel2.ResumeLayout(false);
            this.infoSplitContainer.Panel2.PerformLayout();
            this.infoSplitContainer.ResumeLayout(false);
            this.lvContextMenuStrip.ResumeLayout(false);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.SplitContainer formSplitContainer;
        private System.Windows.Forms.TreeView treeView;
        private System.Windows.Forms.ToolStripStatusLabel ConnectionStatusLabel;
        private System.Windows.Forms.OpenFileDialog importPktDialog;
        private System.Windows.Forms.ContextMenuStrip lvContextMenuStrip;
        private System.Windows.Forms.ToolStripMenuItem extendToolStripMenuItem;
        private System.Windows.Forms.SaveFileDialog exportPktDialog;
        private System.Windows.Forms.FolderBrowserDialog folderBrowserDialog1;
        private System.Windows.Forms.ToolStripMenuItem FileMenu;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem viewToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolTip generalToolTip;
        private System.Windows.Forms.ImageList productIconList;
        private System.Windows.Forms.ToolStripMenuItem useActivationOnLicenseDropDownMenuItem;
        private System.Windows.Forms.ToolStripMenuItem fileLicenseToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem importToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exportToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator sep1ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem genVerificationOnLicenseDropDownMenuItem;
        private System.Windows.Forms.ToolStripSeparator sep2ToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator sep3ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem genLicArchiveOnLicenseDropDownMenuItem;
        private System.Windows.Forms.ToolStripMenuItem importLicArchiveOnLicenseDropDownMenuItem;
        private System.Windows.Forms.ToolStripMenuItem refreshToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem enterPasswordToolStripMenuItem;
        private System.Windows.Forms.SplitContainer infoSplitContainer;
        private Shared.VisualComponents.NoFlickerListView noFlickerListView;
        private Shared.VisualComponents.NoFlickerListView bottomNoFlickerListView;
        private System.Windows.Forms.ToolStripMenuItem enterPasswordPacketToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator sep5ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem updateLicToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem genDiagnosticDataDropDownMenuItem;
        private System.Windows.Forms.ToolStripMenuItem copyToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator extendToolStripSeparator;
        private System.Windows.Forms.ToolStrip lvToolStrip;
        private System.Windows.Forms.ToolStripMenuItem remoteServerToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem remoteConnectToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator editConnToolStripSeparator;
        private System.Windows.Forms.ToolStripMenuItem editConnToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator testConnToolStripSeparator;
        private System.Windows.Forms.ToolStripMenuItem testConnSelToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem testConnToAllToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator remoteConnectToolStripMenuSeparator;
        private System.Windows.Forms.ToolStrip bottomLvToolStrip;
        private System.Windows.Forms.ToolStripMenuItem diagnosticDataToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem generateLicenseDiagnosticDataForSolimarToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadDiagnosticDataToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator diagnosticDataToolStripMenuSeparator;

    }
}

