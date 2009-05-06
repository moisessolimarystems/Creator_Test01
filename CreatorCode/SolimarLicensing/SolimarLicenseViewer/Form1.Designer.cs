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
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.licenseToolStripButton = new System.Windows.Forms.ToolStripDropDownButton();
            this.importDropDownMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exportDropDownMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.viewsToolStripSplitButton = new System.Windows.Forms.ToolStripSplitButton();
            this.licenseSplitBtnMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.usageSplitBtnMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.refreshToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.useActivationOnLicenseDropDownMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.formSplitContainer = new System.Windows.Forms.SplitContainer();
            this.treeView = new System.Windows.Forms.TreeView();
            this.productIconList = new System.Windows.Forms.ImageList(this.components);
            this.TreeViewMenuStrip = new System.Windows.Forms.MenuStrip();
            this.viewTypeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.infoSplitContainer = new System.Windows.Forms.SplitContainer();
            this.noFlickerListView = new Shared.VisualComponents.NoFlickerListView();
            this.lvContextMenuStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.extendToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.bottomNoFlickerListView = new Shared.VisualComponents.NoFlickerListView();
            this.importPktDialog = new System.Windows.Forms.OpenFileDialog();
            this.exportPktDialog = new System.Windows.Forms.SaveFileDialog();
            this.folderBrowserDialog1 = new System.Windows.Forms.FolderBrowserDialog();
            this.FileMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.ConnectMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.refreshToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.sep4ToolStripMenuItem = new System.Windows.Forms.ToolStripSeparator();
            this.licenseToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.usageToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.optionsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
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
            this.generalToolTip = new System.Windows.Forms.ToolTip(this.components);
            this.statusStrip1.SuspendLayout();
            this.toolStrip1.SuspendLayout();
            this.formSplitContainer.Panel1.SuspendLayout();
            this.formSplitContainer.Panel2.SuspendLayout();
            this.formSplitContainer.SuspendLayout();
            this.TreeViewMenuStrip.SuspendLayout();
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
            // toolStrip1
            // 
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.licenseToolStripButton,
            this.toolStripSeparator2,
            this.viewsToolStripSplitButton,
            this.refreshToolStripButton});
            this.toolStrip1.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.HorizontalStackWithOverflow;
            this.toolStrip1.Location = new System.Drawing.Point(0, 24);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(655, 25);
            this.toolStrip1.TabIndex = 2;
            this.toolStrip1.Text = "toolStrip1";
            this.toolStrip1.Visible = false;
            // 
            // licenseToolStripButton
            // 
            this.licenseToolStripButton.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.importDropDownMenuItem,
            this.exportDropDownMenuItem});
            this.licenseToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("licenseToolStripButton.Image")));
            this.licenseToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.licenseToolStripButton.Name = "licenseToolStripButton";
            this.licenseToolStripButton.Size = new System.Drawing.Size(75, 22);
            this.licenseToolStripButton.Text = "License";
            this.licenseToolStripButton.ToolTipText = "Apply License Packet";
            this.licenseToolStripButton.DropDownOpening += new System.EventHandler(this.licenseToolStripButton_DropDownOpening);
            // 
            // importDropDownMenuItem
            // 
            this.importDropDownMenuItem.Name = "importDropDownMenuItem";
            this.importDropDownMenuItem.Size = new System.Drawing.Size(119, 22);
            this.importDropDownMenuItem.Text = "Import...";
            this.importDropDownMenuItem.Click += new System.EventHandler(this.importPacketToolStripMenuItem_Click);
            // 
            // exportDropDownMenuItem
            // 
            this.exportDropDownMenuItem.Name = "exportDropDownMenuItem";
            this.exportDropDownMenuItem.Size = new System.Drawing.Size(119, 22);
            this.exportDropDownMenuItem.Text = "Export...";
            this.exportDropDownMenuItem.Click += new System.EventHandler(this.exportToolStripMenuItem1_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 25);
            // 
            // viewsToolStripSplitButton
            // 
            this.viewsToolStripSplitButton.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.licenseSplitBtnMenuItem,
            this.usageSplitBtnMenuItem});
            this.viewsToolStripSplitButton.Image = ((System.Drawing.Image)(resources.GetObject("viewsToolStripSplitButton.Image")));
            this.viewsToolStripSplitButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.viewsToolStripSplitButton.Name = "viewsToolStripSplitButton";
            this.viewsToolStripSplitButton.Size = new System.Drawing.Size(69, 22);
            this.viewsToolStripSplitButton.Text = "Views";
            this.viewsToolStripSplitButton.ButtonClick += new System.EventHandler(this.viewsToolStripSplitButton_ButtonClick);
            // 
            // licenseSplitBtnMenuItem
            // 
            this.licenseSplitBtnMenuItem.Name = "licenseSplitBtnMenuItem";
            this.licenseSplitBtnMenuItem.Size = new System.Drawing.Size(113, 22);
            this.licenseSplitBtnMenuItem.Text = "License";
            this.licenseSplitBtnMenuItem.Click += new System.EventHandler(this.licenseSplitBtnMenuItem_Click);
            // 
            // usageSplitBtnMenuItem
            // 
            this.usageSplitBtnMenuItem.Name = "usageSplitBtnMenuItem";
            this.usageSplitBtnMenuItem.Size = new System.Drawing.Size(113, 22);
            this.usageSplitBtnMenuItem.Text = "Usage";
            this.usageSplitBtnMenuItem.Click += new System.EventHandler(this.usageSplitBtnMenuItem_Click);
            // 
            // refreshToolStripButton
            // 
            this.refreshToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("refreshToolStripButton.Image")));
            this.refreshToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.refreshToolStripButton.Name = "refreshToolStripButton";
            this.refreshToolStripButton.Size = new System.Drawing.Size(66, 22);
            this.refreshToolStripButton.Text = "Refresh";
            this.refreshToolStripButton.ToolTipText = "Refresh View";
            this.refreshToolStripButton.Click += new System.EventHandler(this.refreshToolStripButton_Click);
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
            this.formSplitContainer.Panel1.Controls.Add(this.TreeViewMenuStrip);
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
            this.treeView.Location = new System.Drawing.Point(0, 24);
            this.treeView.Name = "treeView";
            this.treeView.SelectedImageIndex = 0;
            this.treeView.Size = new System.Drawing.Size(188, 325);
            this.treeView.TabIndex = 0;
            this.treeView.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.treeView_AfterSelect);
            // 
            // productIconList
            // 
            this.productIconList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("productIconList.ImageStream")));
            this.productIconList.TransparentColor = System.Drawing.Color.Transparent;
            this.productIconList.Images.SetKeyName(0, "SPDNT.ICO");
            this.productIconList.Images.SetKeyName(1, "iCONVERT.ICO");
            this.productIconList.Images.SetKeyName(2, "rubika32.ico");
            this.productIconList.Images.SetKeyName(3, "sdxDesigner32.ico");
            this.productIconList.Images.SetKeyName(4, "SOLenterprise32.ico");
            this.productIconList.Images.SetKeyName(5, "solfusion32.ico");
            this.productIconList.Images.SetKeyName(6, "solScript32.ico");
            this.productIconList.Images.SetKeyName(7, "spde32.ico");
            this.productIconList.Images.SetKeyName(8, "System Manager32.ico");
            this.productIconList.Images.SetKeyName(9, "Queue Manager32.ico");
            this.productIconList.Images.SetKeyName(10, "Licenses.ico");
            this.productIconList.Images.SetKeyName(11, "LicenseID.ico");
            this.productIconList.Images.SetKeyName(12, "AppInstance.ico");
            this.productIconList.Images.SetKeyName(13, "History.ico");
            this.productIconList.Images.SetKeyName(14, "Empty.ico");
            this.productIconList.Images.SetKeyName(15, "protectionKeys.ico");
            this.productIconList.Images.SetKeyName(16, "protectionKey.ico");
            this.productIconList.Images.SetKeyName(17, "Rubika Process Builder32.ico");
            // 
            // TreeViewMenuStrip
            // 
            this.TreeViewMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.viewTypeToolStripMenuItem});
            this.TreeViewMenuStrip.Location = new System.Drawing.Point(0, 0);
            this.TreeViewMenuStrip.Name = "TreeViewMenuStrip";
            this.TreeViewMenuStrip.Size = new System.Drawing.Size(188, 24);
            this.TreeViewMenuStrip.TabIndex = 1;
            // 
            // viewTypeToolStripMenuItem
            // 
            this.viewTypeToolStripMenuItem.Name = "viewTypeToolStripMenuItem";
            this.viewTypeToolStripMenuItem.Size = new System.Drawing.Size(12, 20);
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
            // 
            // infoSplitContainer.Panel2
            // 
            this.infoSplitContainer.Panel2.Controls.Add(this.bottomNoFlickerListView);
            this.infoSplitContainer.Panel2Collapsed = true;
            this.infoSplitContainer.Size = new System.Drawing.Size(463, 349);
            this.infoSplitContainer.SplitterDistance = 165;
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
            this.noFlickerListView.TabIndex = 2;
            this.noFlickerListView.UseCompatibleStateImageBehavior = false;
            this.noFlickerListView.View = System.Windows.Forms.View.Details;
            this.noFlickerListView.ColumnClick += new System.Windows.Forms.ColumnClickEventHandler(this.listView_ColumnClick);
            this.noFlickerListView.KeyDown += new System.Windows.Forms.KeyEventHandler(this.General_KeyDown);
            // 
            // lvContextMenuStrip
            // 
            this.lvContextMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.extendToolStripMenuItem});
            this.lvContextMenuStrip.Name = "lvContextMenuStrip";
            this.lvContextMenuStrip.Size = new System.Drawing.Size(110, 26);
            this.lvContextMenuStrip.Opening += new System.ComponentModel.CancelEventHandler(this.lvContextMenuStrip_Opening);
            // 
            // extendToolStripMenuItem
            // 
            this.extendToolStripMenuItem.Name = "extendToolStripMenuItem";
            this.extendToolStripMenuItem.Size = new System.Drawing.Size(109, 22);
            this.extendToolStripMenuItem.Text = "Extend";
            this.extendToolStripMenuItem.Click += new System.EventHandler(this.extendToolStripMenuItem_Click);
            // 
            // bottomNoFlickerListView
            // 
            this.bottomNoFlickerListView.ContextMenuStrip = this.lvContextMenuStrip;
            this.bottomNoFlickerListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.bottomNoFlickerListView.FullRowSelect = true;
            this.bottomNoFlickerListView.Location = new System.Drawing.Point(0, 0);
            this.bottomNoFlickerListView.Name = "bottomNoFlickerListView";
            this.bottomNoFlickerListView.ShowItemToolTips = true;
            this.bottomNoFlickerListView.Size = new System.Drawing.Size(150, 46);
            this.bottomNoFlickerListView.Sorting = System.Windows.Forms.SortOrder.Ascending;
            this.bottomNoFlickerListView.TabIndex = 3;
            this.bottomNoFlickerListView.UseCompatibleStateImageBehavior = false;
            this.bottomNoFlickerListView.View = System.Windows.Forms.View.Details;
            this.bottomNoFlickerListView.ColumnClick += new System.Windows.Forms.ColumnClickEventHandler(this.listView_ColumnClick);
            this.bottomNoFlickerListView.KeyDown += new System.Windows.Forms.KeyEventHandler(this.General_KeyDown);
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
            this.ConnectMenuItem,
            this.toolStripSeparator4,
            this.exitToolStripMenuItem});
            this.FileMenu.Name = "FileMenu";
            this.FileMenu.Size = new System.Drawing.Size(37, 20);
            this.FileMenu.Text = "File";
            // 
            // ConnectMenuItem
            // 
            this.ConnectMenuItem.Name = "ConnectMenuItem";
            this.ConnectMenuItem.Size = new System.Drawing.Size(119, 22);
            this.ConnectMenuItem.Text = "Connect";
            this.ConnectMenuItem.Click += new System.EventHandler(this.ConnectMenuItem_Click);
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(116, 6);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(119, 22);
            this.exitToolStripMenuItem.Text = "E&xit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // viewToolStripMenuItem
            // 
            this.viewToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.refreshToolStripMenuItem,
            this.sep4ToolStripMenuItem,
            this.licenseToolStripMenuItem,
            this.usageToolStripMenuItem});
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
            // sep4ToolStripMenuItem
            // 
            this.sep4ToolStripMenuItem.Name = "sep4ToolStripMenuItem";
            this.sep4ToolStripMenuItem.Size = new System.Drawing.Size(110, 6);
            // 
            // licenseToolStripMenuItem
            // 
            this.licenseToolStripMenuItem.Name = "licenseToolStripMenuItem";
            this.licenseToolStripMenuItem.Size = new System.Drawing.Size(113, 22);
            this.licenseToolStripMenuItem.Text = "License";
            this.licenseToolStripMenuItem.Click += new System.EventHandler(this.licenseToolStripMenuItem_Click);
            // 
            // usageToolStripMenuItem
            // 
            this.usageToolStripMenuItem.Name = "usageToolStripMenuItem";
            this.usageToolStripMenuItem.Size = new System.Drawing.Size(113, 22);
            this.usageToolStripMenuItem.Text = "Usage";
            this.usageToolStripMenuItem.Click += new System.EventHandler(this.usageToolStripMenuItem_Click);
            // 
            // toolsToolStripMenuItem
            // 
            this.toolsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.optionsToolStripMenuItem});
            this.toolsToolStripMenuItem.Name = "toolsToolStripMenuItem";
            this.toolsToolStripMenuItem.Size = new System.Drawing.Size(48, 20);
            this.toolsToolStripMenuItem.Text = "Tools";
            this.toolsToolStripMenuItem.Click += new System.EventHandler(this.toolsToolStripMenuItem_Click);
            // 
            // optionsToolStripMenuItem
            // 
            this.optionsToolStripMenuItem.Name = "optionsToolStripMenuItem";
            this.optionsToolStripMenuItem.Size = new System.Drawing.Size(125, 22);
            this.optionsToolStripMenuItem.Text = "Options...";
            this.optionsToolStripMenuItem.Click += new System.EventHandler(this.optionsToolStripMenuItem_Click);
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
            this.toolsToolStripMenuItem,
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
            // 
            // genDiagnosticDataDropDownMenuItem
            // 
            this.genDiagnosticDataDropDownMenuItem.Name = "genDiagnosticDataDropDownMenuItem";
            this.genDiagnosticDataDropDownMenuItem.Size = new System.Drawing.Size(319, 22);
            this.genDiagnosticDataDropDownMenuItem.Text = "Generate License Diagnostic Data for Solimar...";
            this.genDiagnosticDataDropDownMenuItem.Click += new System.EventHandler(this.genDiagnosticDataDropDownMenuItem_Click);
            // 
            // sep2ToolStripMenuItem
            // 
            this.sep2ToolStripMenuItem.Name = "sep2ToolStripMenuItem";
            this.sep2ToolStripMenuItem.Size = new System.Drawing.Size(316, 6);
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
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.ClientSize = new System.Drawing.Size(655, 395);
            this.Controls.Add(this.formSplitContainer);
            this.Controls.Add(this.toolStrip1);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.menuStrip1);
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "License Viewer [Disconnected]";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.Shown += new System.EventHandler(this.ConnectMenuItem_Click);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.formSplitContainer.Panel1.ResumeLayout(false);
            this.formSplitContainer.Panel1.PerformLayout();
            this.formSplitContainer.Panel2.ResumeLayout(false);
            this.formSplitContainer.ResumeLayout(false);
            this.TreeViewMenuStrip.ResumeLayout(false);
            this.TreeViewMenuStrip.PerformLayout();
            this.infoSplitContainer.Panel1.ResumeLayout(false);
            this.infoSplitContainer.Panel2.ResumeLayout(false);
            this.infoSplitContainer.ResumeLayout(false);
            this.lvContextMenuStrip.ResumeLayout(false);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.SplitContainer formSplitContainer;
        private System.Windows.Forms.TreeView treeView;
        private System.Windows.Forms.MenuStrip TreeViewMenuStrip;
        private System.Windows.Forms.ToolStripStatusLabel ConnectionStatusLabel;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripButton refreshToolStripButton;
        private System.Windows.Forms.ToolStripMenuItem viewTypeToolStripMenuItem;
        private System.Windows.Forms.OpenFileDialog importPktDialog;
        private System.Windows.Forms.ContextMenuStrip lvContextMenuStrip;
        private System.Windows.Forms.ToolStripMenuItem extendToolStripMenuItem;
        private System.Windows.Forms.SaveFileDialog exportPktDialog;
        private System.Windows.Forms.FolderBrowserDialog folderBrowserDialog1;
        private System.Windows.Forms.ToolStripSplitButton viewsToolStripSplitButton;
        private System.Windows.Forms.ToolStripMenuItem licenseSplitBtnMenuItem;
        private System.Windows.Forms.ToolStripMenuItem usageSplitBtnMenuItem;
        private System.Windows.Forms.ToolStripMenuItem FileMenu;
        private System.Windows.Forms.ToolStripMenuItem ConnectMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem viewToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem licenseToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem usageToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem toolsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem optionsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripDropDownButton licenseToolStripButton;
        private System.Windows.Forms.ToolStripMenuItem importDropDownMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exportDropDownMenuItem;
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
        private System.Windows.Forms.ToolStripSeparator sep4ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem enterPasswordToolStripMenuItem;
        private System.Windows.Forms.SplitContainer infoSplitContainer;
        private Shared.VisualComponents.NoFlickerListView noFlickerListView;
        private Shared.VisualComponents.NoFlickerListView bottomNoFlickerListView;
        private System.Windows.Forms.ToolStripMenuItem enterPasswordPacketToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator sep5ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem updateLicToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem genDiagnosticDataDropDownMenuItem;

    }
}

