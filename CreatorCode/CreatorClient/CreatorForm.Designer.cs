namespace Client.Creator
{
    partial class CreatorForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(CreatorForm));
            System.Windows.Forms.TreeNode treeNode1 = new System.Windows.Forms.TreeNode("Licenses", 1, 1);
            System.Windows.Forms.TreeNode treeNode2 = new System.Windows.Forms.TreeNode("Packets", 2, 2);
            System.Windows.Forms.TreeNode treeNode3 = new System.Windows.Forms.TreeNode("Hardware Keys");
            System.Windows.Forms.TreeNode treeNode4 = new System.Windows.Forms.TreeNode("Creator", 3, 3, new System.Windows.Forms.TreeNode[] {
            treeNode1,
            treeNode2,
            treeNode3});
            System.Windows.Forms.TreeNode treeNode5 = new System.Windows.Forms.TreeNode("Reports", 0, 0);
            this.MainMenuStrip = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newReportToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.exitMainToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.MainToolStrip = new System.Windows.Forms.ToolStrip();
            this.newCustMainToolStripBtn = new System.Windows.Forms.ToolStripButton();
            this.removeCustMainToolStripBtn = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator7 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripSearchBox = new System.Windows.Forms.ToolStripTextBox();
            this.toolStripSearchButton = new System.Windows.Forms.ToolStripDropDownButton();
            this.addLicMainToolStripBtn = new System.Windows.Forms.ToolStripSplitButton();
            this.newStandardLicenseToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newFailoverLicenseToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newDisasterRecoveryLicenseToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newTestDevelopmentLicenseToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.removeLicMainToolStripBtn = new System.Windows.Forms.ToolStripButton();
            this.createPacketMainToolStripBtn = new System.Windows.Forms.ToolStripButton();
            this.refreshMainToolStripBtn = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator8 = new System.Windows.Forms.ToolStripSeparator();
            this.addOrderMainToolStripDropDownBtn = new System.Windows.Forms.ToolStripSplitButton();
            this.newOrderMainToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.existingOrderMainToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.removeOrderMainToolStripBtn = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator9 = new System.Windows.Forms.ToolStripSeparator();
            this.toggleViewMainToolStripBtn = new System.Windows.Forms.ToolStripButton();
            this.MainStatusStrip = new System.Windows.Forms.StatusStrip();
            this.progressBarStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.countToolStripStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.LicenseViewPanel = new System.Windows.Forms.Panel();
            this.MainSplitContainer = new System.Windows.Forms.SplitContainer();
            this.MainTreeView = new System.Windows.Forms.TreeView();
            this.MainTreeViewImageList = new System.Windows.Forms.ImageList(this.components);
            this.MainTabControl = new System.Windows.Forms.TabControl();
            this.LicensesTabPage = new System.Windows.Forms.TabPage();
            this.SubSplitContainer = new System.Windows.Forms.SplitContainer();
            this.DetailTreeView = new System.Windows.Forms.TreeView();
            this.DetailTreeViewImageList = new System.Windows.Forms.ImageList(this.components);
            this.PropertyGridTabControl = new System.Windows.Forms.TabControl();
            this.PropertyGridTabPage = new System.Windows.Forms.TabPage();
            this.DetailSplitContainer = new System.Windows.Forms.SplitContainer();
            this.DetailPropertyGrid = new System.Windows.Forms.PropertyGrid();
            this.DetailListView = new System.Windows.Forms.ListView();
            this.ListViewImageList = new System.Windows.Forms.ImageList(this.components);
            this.DetailListViewToolStrip = new System.Windows.Forms.ToolStrip();
            this.ImageToolStripLabel = new System.Windows.Forms.ToolStripLabel();
            this.DetailListViewToolStripLabel = new System.Windows.Forms.ToolStripLabel();
            this.ListViewTabPage = new System.Windows.Forms.TabPage();
            this.TransactionListView = new Shared.VisualComponents.NoFlickerListView();
            this.TransactionDateColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.DescriptionColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.ValueColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.UserColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.TransactionsToolStrip = new System.Windows.Forms.ToolStrip();
            this.GroupToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.SingleToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.PacketTabPage = new System.Windows.Forms.TabPage();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.LicensePacketListView = new Shared.VisualComponents.NoFlickerListView();
            this.PktNameColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.PktCodeColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.PktDateColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.PktDescriptionColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.PktUserColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.PacketContextMenuStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.verifyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.PacketListToolStrip = new System.Windows.Forms.ToolStrip();
            this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
            this.toolStripComboBox1 = new System.Windows.Forms.ToolStripComboBox();
            this.showAllToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.showVerifiedToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.showUnVerifiedToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.PacketsTabPage = new System.Windows.Forms.TabPage();
            this.PacketListView = new Shared.VisualComponents.NoFlickerListView();
            this.statusColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.nameColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.dateColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.vCodeColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.commentsColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.userNameColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.ReportTabPage = new System.Windows.Forms.TabPage();
            this.ReportListView = new Shared.VisualComponents.NoFlickerListView();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
            this.HardwareKeyTabPage = new System.Windows.Forms.TabPage();
            this.panel1 = new System.Windows.Forms.Panel();
            this.HardwareKeyListView = new Shared.VisualComponents.NoFlickerListView();
            this.columnHeader3 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader4 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader5 = new System.Windows.Forms.ColumnHeader();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.addHardwareKeyToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.loadingCircle1 = new Shared.VisualComponents.LoadingCircle();
            this.DetailListViewContextMenuStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.dlvNewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.dlvEditToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.dlvDeleteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.MainTreeViewContextMenuStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.editReportToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.deleteToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.LicenseContextMenuStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.addToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newCustomerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newLicenseToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newOrderToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.existingOrderToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addLicenseToolStripSeparator = new System.Windows.Forms.ToolStripSeparator();
            this.newFailoverToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newDisRecToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newTestDevToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.lcmToolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.refreshToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.deleteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.lcmToolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.createPacketFileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.MainMenuStrip.SuspendLayout();
            this.MainToolStrip.SuspendLayout();
            this.MainStatusStrip.SuspendLayout();
            this.LicenseViewPanel.SuspendLayout();
            this.MainSplitContainer.Panel1.SuspendLayout();
            this.MainSplitContainer.Panel2.SuspendLayout();
            this.MainSplitContainer.SuspendLayout();
            this.MainTabControl.SuspendLayout();
            this.LicensesTabPage.SuspendLayout();
            this.SubSplitContainer.Panel1.SuspendLayout();
            this.SubSplitContainer.Panel2.SuspendLayout();
            this.SubSplitContainer.SuspendLayout();
            this.PropertyGridTabControl.SuspendLayout();
            this.PropertyGridTabPage.SuspendLayout();
            this.DetailSplitContainer.Panel1.SuspendLayout();
            this.DetailSplitContainer.Panel2.SuspendLayout();
            this.DetailSplitContainer.SuspendLayout();
            this.DetailListViewToolStrip.SuspendLayout();
            this.ListViewTabPage.SuspendLayout();
            this.TransactionsToolStrip.SuspendLayout();
            this.PacketTabPage.SuspendLayout();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.PacketContextMenuStrip.SuspendLayout();
            this.PacketListToolStrip.SuspendLayout();
            this.PacketsTabPage.SuspendLayout();
            this.ReportTabPage.SuspendLayout();
            this.HardwareKeyTabPage.SuspendLayout();
            this.panel1.SuspendLayout();
            this.toolStrip1.SuspendLayout();
            this.DetailListViewContextMenuStrip.SuspendLayout();
            this.MainTreeViewContextMenuStrip.SuspendLayout();
            this.LicenseContextMenuStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // MainMenuStrip
            // 
            this.MainMenuStrip.BackColor = System.Drawing.SystemColors.Control;
            this.MainMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
            this.MainMenuStrip.Location = new System.Drawing.Point(0, 0);
            this.MainMenuStrip.Name = "MainMenuStrip";
            this.MainMenuStrip.Size = new System.Drawing.Size(993, 24);
            this.MainMenuStrip.TabIndex = 0;
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newReportToolStripMenuItem,
            this.toolStripSeparator3,
            this.exitMainToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(35, 20);
            this.fileToolStripMenuItem.Text = "&File";
            // 
            // newReportToolStripMenuItem
            // 
            this.newReportToolStripMenuItem.Name = "newReportToolStripMenuItem";
            this.newReportToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.newReportToolStripMenuItem.Text = "New Report";
            this.newReportToolStripMenuItem.Click += new System.EventHandler(this.newReportToolStripMenuItem_Click);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(149, 6);
            // 
            // exitMainToolStripMenuItem
            // 
            this.exitMainToolStripMenuItem.Name = "exitMainToolStripMenuItem";
            this.exitMainToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.exitMainToolStripMenuItem.Text = "E&xit";
            this.exitMainToolStripMenuItem.Click += new System.EventHandler(this.exitMainToolStripMenuItem_Click);
            // 
            // MainToolStrip
            // 
            this.MainToolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newCustMainToolStripBtn,
            this.removeCustMainToolStripBtn,
            this.toolStripSeparator7,
            this.toolStripSearchBox,
            this.toolStripSearchButton,
            this.addLicMainToolStripBtn,
            this.removeLicMainToolStripBtn,
            this.createPacketMainToolStripBtn,
            this.refreshMainToolStripBtn,
            this.toolStripSeparator8,
            this.addOrderMainToolStripDropDownBtn,
            this.removeOrderMainToolStripBtn,
            this.toolStripSeparator9,
            this.toggleViewMainToolStripBtn});
            this.MainToolStrip.Location = new System.Drawing.Point(0, 24);
            this.MainToolStrip.Name = "MainToolStrip";
            this.MainToolStrip.Size = new System.Drawing.Size(993, 25);
            this.MainToolStrip.TabIndex = 1;
            // 
            // newCustMainToolStripBtn
            // 
            this.newCustMainToolStripBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.newCustMainToolStripBtn.Image = ((System.Drawing.Image)(resources.GetObject("newCustMainToolStripBtn.Image")));
            this.newCustMainToolStripBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.newCustMainToolStripBtn.Name = "newCustMainToolStripBtn";
            this.newCustMainToolStripBtn.Size = new System.Drawing.Size(23, 22);
            this.newCustMainToolStripBtn.Text = "Add New Customer";
            this.newCustMainToolStripBtn.Click += new System.EventHandler(this.newCustMainToolStripBtn_Click);
            // 
            // removeCustMainToolStripBtn
            // 
            this.removeCustMainToolStripBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.removeCustMainToolStripBtn.Image = ((System.Drawing.Image)(resources.GetObject("removeCustMainToolStripBtn.Image")));
            this.removeCustMainToolStripBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.removeCustMainToolStripBtn.Name = "removeCustMainToolStripBtn";
            this.removeCustMainToolStripBtn.Size = new System.Drawing.Size(23, 22);
            this.removeCustMainToolStripBtn.Text = "Remove Selected Customer";
            this.removeCustMainToolStripBtn.Click += new System.EventHandler(this.removeCustMainToolStripBtn_Click);
            // 
            // toolStripSeparator7
            // 
            this.toolStripSeparator7.Name = "toolStripSeparator7";
            this.toolStripSeparator7.Size = new System.Drawing.Size(6, 25);
            // 
            // toolStripSearchBox
            // 
            this.toolStripSearchBox.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
            this.toolStripSearchBox.Margin = new System.Windows.Forms.Padding(0, 0, 5, 0);
            this.toolStripSearchBox.Name = "toolStripSearchBox";
            this.toolStripSearchBox.Size = new System.Drawing.Size(100, 25);
            this.toolStripSearchBox.KeyUp += new System.Windows.Forms.KeyEventHandler(this.toolStripSearchBox_KeyUp);
            // 
            // toolStripSearchButton
            // 
            this.toolStripSearchButton.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
            this.toolStripSearchButton.BackColor = System.Drawing.SystemColors.Control;
            this.toolStripSearchButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripSearchButton.Image = ((System.Drawing.Image)(resources.GetObject("toolStripSearchButton.Image")));
            this.toolStripSearchButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripSearchButton.Name = "toolStripSearchButton";
            this.toolStripSearchButton.Size = new System.Drawing.Size(29, 22);
            this.toolStripSearchButton.Text = "toolStripButton1";
            this.toolStripSearchButton.ToolTipText = "Search";
            this.toolStripSearchButton.Click += new System.EventHandler(this.toolStripSearchButton_Click);
            // 
            // addLicMainToolStripBtn
            // 
            this.addLicMainToolStripBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.addLicMainToolStripBtn.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newStandardLicenseToolStripMenuItem,
            this.newFailoverLicenseToolStripMenuItem,
            this.newDisasterRecoveryLicenseToolStripMenuItem,
            this.newTestDevelopmentLicenseToolStripMenuItem});
            this.addLicMainToolStripBtn.Image = ((System.Drawing.Image)(resources.GetObject("addLicMainToolStripBtn.Image")));
            this.addLicMainToolStripBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.addLicMainToolStripBtn.Name = "addLicMainToolStripBtn";
            this.addLicMainToolStripBtn.Size = new System.Drawing.Size(32, 22);
            this.addLicMainToolStripBtn.Text = "Add New License";
            this.addLicMainToolStripBtn.ButtonClick += new System.EventHandler(this.addLicMainToolStripBtn_ButtonClick);
            this.addLicMainToolStripBtn.DropDownOpening += new System.EventHandler(this.addLicMainToolStripBtn_DropDownOpening);
            // 
            // newStandardLicenseToolStripMenuItem
            // 
            this.newStandardLicenseToolStripMenuItem.Name = "newStandardLicenseToolStripMenuItem";
            this.newStandardLicenseToolStripMenuItem.Size = new System.Drawing.Size(247, 22);
            this.newStandardLicenseToolStripMenuItem.Text = "New Standard License...";
            this.newStandardLicenseToolStripMenuItem.Click += new System.EventHandler(this.newStandardMainToolStripMenuItem_Click);
            // 
            // newFailoverLicenseToolStripMenuItem
            // 
            this.newFailoverLicenseToolStripMenuItem.Name = "newFailoverLicenseToolStripMenuItem";
            this.newFailoverLicenseToolStripMenuItem.Size = new System.Drawing.Size(247, 22);
            this.newFailoverLicenseToolStripMenuItem.Text = "New Failover License...";
            this.newFailoverLicenseToolStripMenuItem.Click += new System.EventHandler(this.newFailoverMainToolStripMenuItem_Click);
            // 
            // newDisasterRecoveryLicenseToolStripMenuItem
            // 
            this.newDisasterRecoveryLicenseToolStripMenuItem.Name = "newDisasterRecoveryLicenseToolStripMenuItem";
            this.newDisasterRecoveryLicenseToolStripMenuItem.Size = new System.Drawing.Size(247, 22);
            this.newDisasterRecoveryLicenseToolStripMenuItem.Text = "New Disaster Recovery License...";
            this.newDisasterRecoveryLicenseToolStripMenuItem.Click += new System.EventHandler(this.newDisasterRecoveryMainToolStripMenuItem_Click);
            // 
            // newTestDevelopmentLicenseToolStripMenuItem
            // 
            this.newTestDevelopmentLicenseToolStripMenuItem.Name = "newTestDevelopmentLicenseToolStripMenuItem";
            this.newTestDevelopmentLicenseToolStripMenuItem.Size = new System.Drawing.Size(247, 22);
            this.newTestDevelopmentLicenseToolStripMenuItem.Text = "New Test Development License...";
            this.newTestDevelopmentLicenseToolStripMenuItem.Click += new System.EventHandler(this.newTestDevelopmentMainToolStripMenuItem_Click);
            // 
            // removeLicMainToolStripBtn
            // 
            this.removeLicMainToolStripBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.removeLicMainToolStripBtn.Image = ((System.Drawing.Image)(resources.GetObject("removeLicMainToolStripBtn.Image")));
            this.removeLicMainToolStripBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.removeLicMainToolStripBtn.Name = "removeLicMainToolStripBtn";
            this.removeLicMainToolStripBtn.Size = new System.Drawing.Size(23, 22);
            this.removeLicMainToolStripBtn.Text = "Remove Selected License";
            this.removeLicMainToolStripBtn.Click += new System.EventHandler(this.removeLicMainToolStripBtn_Click);
            // 
            // createPacketMainToolStripBtn
            // 
            this.createPacketMainToolStripBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.createPacketMainToolStripBtn.Enabled = false;
            this.createPacketMainToolStripBtn.Image = ((System.Drawing.Image)(resources.GetObject("createPacketMainToolStripBtn.Image")));
            this.createPacketMainToolStripBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.createPacketMainToolStripBtn.Name = "createPacketMainToolStripBtn";
            this.createPacketMainToolStripBtn.Size = new System.Drawing.Size(23, 22);
            this.createPacketMainToolStripBtn.Text = "Generate License";
            this.createPacketMainToolStripBtn.ToolTipText = "Generate License Packet from selected license";
            this.createPacketMainToolStripBtn.Click += new System.EventHandler(this.createPacketMainToolStripBtn_Click);
            // 
            // refreshMainToolStripBtn
            // 
            this.refreshMainToolStripBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.refreshMainToolStripBtn.Image = ((System.Drawing.Image)(resources.GetObject("refreshMainToolStripBtn.Image")));
            this.refreshMainToolStripBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.refreshMainToolStripBtn.Name = "refreshMainToolStripBtn";
            this.refreshMainToolStripBtn.Size = new System.Drawing.Size(23, 22);
            this.refreshMainToolStripBtn.Text = "Refresh Selected Item";
            this.refreshMainToolStripBtn.Click += new System.EventHandler(this.refreshMainToolStripBtn_Click);
            // 
            // toolStripSeparator8
            // 
            this.toolStripSeparator8.Name = "toolStripSeparator8";
            this.toolStripSeparator8.Size = new System.Drawing.Size(6, 25);
            // 
            // addOrderMainToolStripDropDownBtn
            // 
            this.addOrderMainToolStripDropDownBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.addOrderMainToolStripDropDownBtn.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newOrderMainToolStripMenuItem,
            this.existingOrderMainToolStripMenuItem});
            this.addOrderMainToolStripDropDownBtn.Image = ((System.Drawing.Image)(resources.GetObject("addOrderMainToolStripDropDownBtn.Image")));
            this.addOrderMainToolStripDropDownBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.addOrderMainToolStripDropDownBtn.Name = "addOrderMainToolStripDropDownBtn";
            this.addOrderMainToolStripDropDownBtn.Size = new System.Drawing.Size(32, 22);
            this.addOrderMainToolStripDropDownBtn.Text = "Add New Order";
            this.addOrderMainToolStripDropDownBtn.ToolTipText = "Add new order to selected license";
            this.addOrderMainToolStripDropDownBtn.ButtonClick += new System.EventHandler(this.addOrderMainToolStripDropDownBtn_ButtonClick);
            this.addOrderMainToolStripDropDownBtn.DropDownOpening += new System.EventHandler(this.addOrderMainToolStripDropDownBtn_DropDownOpening);
            // 
            // newOrderMainToolStripMenuItem
            // 
            this.newOrderMainToolStripMenuItem.Name = "newOrderMainToolStripMenuItem";
            this.newOrderMainToolStripMenuItem.Size = new System.Drawing.Size(165, 22);
            this.newOrderMainToolStripMenuItem.Text = "New Order...";
            this.newOrderMainToolStripMenuItem.Click += new System.EventHandler(this.addOrderMainToolStripDropDownBtn_Click);
            // 
            // existingOrderMainToolStripMenuItem
            // 
            this.existingOrderMainToolStripMenuItem.Name = "existingOrderMainToolStripMenuItem";
            this.existingOrderMainToolStripMenuItem.Size = new System.Drawing.Size(165, 22);
            this.existingOrderMainToolStripMenuItem.Text = "Existing Order...";
            this.existingOrderMainToolStripMenuItem.Click += new System.EventHandler(this.existingOrderMainToolStripMenuItem_Click);
            // 
            // removeOrderMainToolStripBtn
            // 
            this.removeOrderMainToolStripBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.removeOrderMainToolStripBtn.Image = ((System.Drawing.Image)(resources.GetObject("removeOrderMainToolStripBtn.Image")));
            this.removeOrderMainToolStripBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.removeOrderMainToolStripBtn.Name = "removeOrderMainToolStripBtn";
            this.removeOrderMainToolStripBtn.Size = new System.Drawing.Size(23, 22);
            this.removeOrderMainToolStripBtn.Text = "Remove Selected Order";
            this.removeOrderMainToolStripBtn.ToolTipText = "Remove order from selected license";
            this.removeOrderMainToolStripBtn.Click += new System.EventHandler(this.removeOrderMainToolStripBtn_Click);
            // 
            // toolStripSeparator9
            // 
            this.toolStripSeparator9.Name = "toolStripSeparator9";
            this.toolStripSeparator9.Size = new System.Drawing.Size(6, 25);
            // 
            // toggleViewMainToolStripBtn
            // 
            this.toggleViewMainToolStripBtn.CheckOnClick = true;
            this.toggleViewMainToolStripBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toggleViewMainToolStripBtn.Image = ((System.Drawing.Image)(resources.GetObject("toggleViewMainToolStripBtn.Image")));
            this.toggleViewMainToolStripBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toggleViewMainToolStripBtn.Name = "toggleViewMainToolStripBtn";
            this.toggleViewMainToolStripBtn.Size = new System.Drawing.Size(23, 22);
            this.toggleViewMainToolStripBtn.Text = "ViewMenuTree";
            this.toggleViewMainToolStripBtn.ToolTipText = "Toggle View";
            this.toggleViewMainToolStripBtn.CheckedChanged += new System.EventHandler(this.toggleViewMainToolStripBtn_CheckedChanged);
            // 
            // MainStatusStrip
            // 
            this.MainStatusStrip.BackColor = System.Drawing.SystemColors.Control;
            this.MainStatusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.progressBarStatusLabel,
            this.countToolStripStatusLabel});
            this.MainStatusStrip.Location = new System.Drawing.Point(0, 507);
            this.MainStatusStrip.Name = "MainStatusStrip";
            this.MainStatusStrip.Size = new System.Drawing.Size(993, 22);
            this.MainStatusStrip.TabIndex = 2;
            // 
            // progressBarStatusLabel
            // 
            this.progressBarStatusLabel.Name = "progressBarStatusLabel";
            this.progressBarStatusLabel.Size = new System.Drawing.Size(0, 17);
            // 
            // countToolStripStatusLabel
            // 
            this.countToolStripStatusLabel.Name = "countToolStripStatusLabel";
            this.countToolStripStatusLabel.Size = new System.Drawing.Size(58, 17);
            this.countToolStripStatusLabel.Text = "ItemCount";
            // 
            // LicenseViewPanel
            // 
            this.LicenseViewPanel.Controls.Add(this.MainSplitContainer);
            this.LicenseViewPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.LicenseViewPanel.Location = new System.Drawing.Point(0, 49);
            this.LicenseViewPanel.Name = "LicenseViewPanel";
            this.LicenseViewPanel.Size = new System.Drawing.Size(993, 458);
            this.LicenseViewPanel.TabIndex = 8;
            // 
            // MainSplitContainer
            // 
            this.MainSplitContainer.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.MainSplitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.MainSplitContainer.Location = new System.Drawing.Point(0, 0);
            this.MainSplitContainer.Margin = new System.Windows.Forms.Padding(0);
            this.MainSplitContainer.Name = "MainSplitContainer";
            // 
            // MainSplitContainer.Panel1
            // 
            this.MainSplitContainer.Panel1.Controls.Add(this.MainTreeView);
            // 
            // MainSplitContainer.Panel2
            // 
            this.MainSplitContainer.Panel2.BackColor = System.Drawing.SystemColors.Control;
            this.MainSplitContainer.Panel2.Controls.Add(this.MainTabControl);
            this.MainSplitContainer.Panel2.Controls.Add(this.loadingCircle1);
            this.MainSplitContainer.Size = new System.Drawing.Size(993, 458);
            this.MainSplitContainer.SplitterDistance = 172;
            this.MainSplitContainer.SplitterWidth = 1;
            this.MainSplitContainer.TabIndex = 1;
            // 
            // MainTreeView
            // 
            this.MainTreeView.BackColor = System.Drawing.SystemColors.Window;
            this.MainTreeView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.MainTreeView.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.MainTreeView.FullRowSelect = true;
            this.MainTreeView.HideSelection = false;
            this.MainTreeView.ImageIndex = 0;
            this.MainTreeView.ImageList = this.MainTreeViewImageList;
            this.MainTreeView.LabelEdit = true;
            this.MainTreeView.Location = new System.Drawing.Point(0, 0);
            this.MainTreeView.Name = "MainTreeView";
            treeNode1.ImageIndex = 1;
            treeNode1.Name = "LicenseNode";
            treeNode1.SelectedImageIndex = 1;
            treeNode1.Text = "Licenses";
            treeNode2.ImageIndex = 2;
            treeNode2.Name = "PacketNode";
            treeNode2.SelectedImageIndex = 2;
            treeNode2.Text = "Packets";
            treeNode3.Name = "KeyNode";
            treeNode3.Text = "Hardware Keys";
            treeNode4.ImageIndex = 3;
            treeNode4.Name = "CreatorRootNode";
            treeNode4.NodeFont = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            treeNode4.SelectedImageIndex = 3;
            treeNode4.Text = "Creator";
            treeNode5.ImageIndex = 0;
            treeNode5.Name = "ReportRootNode";
            treeNode5.NodeFont = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            treeNode5.SelectedImageIndex = 0;
            treeNode5.Text = "Reports";
            this.MainTreeView.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            treeNode4,
            treeNode5});
            this.MainTreeView.SelectedImageIndex = 0;
            this.MainTreeView.ShowLines = false;
            this.MainTreeView.ShowRootLines = false;
            this.MainTreeView.Size = new System.Drawing.Size(168, 454);
            this.MainTreeView.TabIndex = 0;
            this.MainTreeView.AfterLabelEdit += new System.Windows.Forms.NodeLabelEditEventHandler(this.MainTreeView_AfterLabelEdit);
            this.MainTreeView.MouseUp += new System.Windows.Forms.MouseEventHandler(this.MainTreeView_MouseUp);
            this.MainTreeView.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.MainTreeView_AfterSelect);
            this.MainTreeView.KeyUp += new System.Windows.Forms.KeyEventHandler(this.MainTreeView_KeyUp);
            this.MainTreeView.BeforeSelect += new System.Windows.Forms.TreeViewCancelEventHandler(this.MainTreeView_BeforeSelect);
            // 
            // MainTreeViewImageList
            // 
            this.MainTreeViewImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("MainTreeViewImageList.ImageStream")));
            this.MainTreeViewImageList.TransparentColor = System.Drawing.Color.Transparent;
            this.MainTreeViewImageList.Images.SetKeyName(0, "sdx_report32.ico");
            this.MainTreeViewImageList.Images.SetKeyName(1, "flow_new.ico");
            this.MainTreeViewImageList.Images.SetKeyName(2, "History.ico");
            this.MainTreeViewImageList.Images.SetKeyName(3, "tree.ico");
            // 
            // MainTabControl
            // 
            this.MainTabControl.Controls.Add(this.LicensesTabPage);
            this.MainTabControl.Controls.Add(this.PacketsTabPage);
            this.MainTabControl.Controls.Add(this.ReportTabPage);
            this.MainTabControl.Controls.Add(this.HardwareKeyTabPage);
            this.MainTabControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.MainTabControl.Location = new System.Drawing.Point(0, 0);
            this.MainTabControl.Margin = new System.Windows.Forms.Padding(0);
            this.MainTabControl.Multiline = true;
            this.MainTabControl.Name = "MainTabControl";
            this.MainTabControl.SelectedIndex = 0;
            this.MainTabControl.Size = new System.Drawing.Size(816, 454);
            this.MainTabControl.TabIndex = 2;
            this.MainTabControl.Visible = false;
            // 
            // LicensesTabPage
            // 
            this.LicensesTabPage.Controls.Add(this.SubSplitContainer);
            this.LicensesTabPage.Location = new System.Drawing.Point(4, 22);
            this.LicensesTabPage.Margin = new System.Windows.Forms.Padding(0);
            this.LicensesTabPage.Name = "LicensesTabPage";
            this.LicensesTabPage.Size = new System.Drawing.Size(808, 428);
            this.LicensesTabPage.TabIndex = 1;
            this.LicensesTabPage.Text = "Licenses";
            this.LicensesTabPage.UseVisualStyleBackColor = true;
            // 
            // SubSplitContainer
            // 
            this.SubSplitContainer.BackColor = System.Drawing.SystemColors.Control;
            this.SubSplitContainer.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.SubSplitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.SubSplitContainer.Location = new System.Drawing.Point(0, 0);
            this.SubSplitContainer.Margin = new System.Windows.Forms.Padding(0);
            this.SubSplitContainer.Name = "SubSplitContainer";
            // 
            // SubSplitContainer.Panel1
            // 
            this.SubSplitContainer.Panel1.Controls.Add(this.DetailTreeView);
            // 
            // SubSplitContainer.Panel2
            // 
            this.SubSplitContainer.Panel2.Controls.Add(this.PropertyGridTabControl);
            this.SubSplitContainer.Size = new System.Drawing.Size(808, 428);
            this.SubSplitContainer.SplitterDistance = 314;
            this.SubSplitContainer.SplitterWidth = 1;
            this.SubSplitContainer.TabIndex = 1;
            // 
            // DetailTreeView
            // 
            this.DetailTreeView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.DetailTreeView.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.DetailTreeView.HideSelection = false;
            this.DetailTreeView.ImageIndex = 0;
            this.DetailTreeView.ImageList = this.DetailTreeViewImageList;
            this.DetailTreeView.Location = new System.Drawing.Point(0, 0);
            this.DetailTreeView.Name = "DetailTreeView";
            this.DetailTreeView.SelectedImageIndex = 0;
            this.DetailTreeView.Size = new System.Drawing.Size(310, 424);
            this.DetailTreeView.StateImageList = this.DetailTreeViewImageList;
            this.DetailTreeView.TabIndex = 2;
            this.DetailTreeView.BeforeExpand += new System.Windows.Forms.TreeViewCancelEventHandler(this.DetailTreeView_BeforeExpand);
            this.DetailTreeView.MouseUp += new System.Windows.Forms.MouseEventHandler(this.DetailTreeView_MouseUp);
            this.DetailTreeView.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.DetailTreeView_AfterSelect);
            // 
            // DetailTreeViewImageList
            // 
            this.DetailTreeViewImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("DetailTreeViewImageList.ImageStream")));
            this.DetailTreeViewImageList.TransparentColor = System.Drawing.Color.Transparent;
            this.DetailTreeViewImageList.Images.SetKeyName(0, "flow_externalProcess.ico");
            this.DetailTreeViewImageList.Images.SetKeyName(1, "AppInstance.ico");
            this.DetailTreeViewImageList.Images.SetKeyName(2, "Empty.ico");
            this.DetailTreeViewImageList.Images.SetKeyName(3, "History.ico");
            this.DetailTreeViewImageList.Images.SetKeyName(4, "Licenses.ico");
            this.DetailTreeViewImageList.Images.SetKeyName(5, "LicenseID.ico");
            this.DetailTreeViewImageList.Images.SetKeyName(6, "iCONVERT.ICO");
            this.DetailTreeViewImageList.Images.SetKeyName(7, "rubika32.ico");
            this.DetailTreeViewImageList.Images.SetKeyName(8, "RubikaProcessBuilder32.ico");
            this.DetailTreeViewImageList.Images.SetKeyName(9, "sdxDesigner32.ico");
            this.DetailTreeViewImageList.Images.SetKeyName(10, "SOLindexer32.ico");
            this.DetailTreeViewImageList.Images.SetKeyName(11, "SOLsearcherSinglePlatform32.ico");
            this.DetailTreeViewImageList.Images.SetKeyName(12, "SOLenterprise32.ico");
            this.DetailTreeViewImageList.Images.SetKeyName(13, "solfusion32.ico");
            this.DetailTreeViewImageList.Images.SetKeyName(14, "solScript32.ico");
            this.DetailTreeViewImageList.Images.SetKeyName(15, "Queue Manager32.ico");
            this.DetailTreeViewImageList.Images.SetKeyName(16, "System Manager32.ico");
            this.DetailTreeViewImageList.Images.SetKeyName(17, "spde32.ico");
            this.DetailTreeViewImageList.Images.SetKeyName(18, "SPDNT.ICO");
            // 
            // PropertyGridTabControl
            // 
            this.PropertyGridTabControl.Alignment = System.Windows.Forms.TabAlignment.Bottom;
            this.PropertyGridTabControl.Controls.Add(this.PropertyGridTabPage);
            this.PropertyGridTabControl.Controls.Add(this.ListViewTabPage);
            this.PropertyGridTabControl.Controls.Add(this.PacketTabPage);
            this.PropertyGridTabControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.PropertyGridTabControl.ImageList = this.ListViewImageList;
            this.PropertyGridTabControl.Location = new System.Drawing.Point(0, 0);
            this.PropertyGridTabControl.Margin = new System.Windows.Forms.Padding(0);
            this.PropertyGridTabControl.Multiline = true;
            this.PropertyGridTabControl.Name = "PropertyGridTabControl";
            this.PropertyGridTabControl.SelectedIndex = 0;
            this.PropertyGridTabControl.Size = new System.Drawing.Size(489, 424);
            this.PropertyGridTabControl.TabIndex = 0;
            this.PropertyGridTabControl.TabIndexChanged += new System.EventHandler(this.PropertyGridTabControl_TabIndexChanged);
            // 
            // PropertyGridTabPage
            // 
            this.PropertyGridTabPage.Controls.Add(this.DetailSplitContainer);
            this.PropertyGridTabPage.ImageIndex = 2;
            this.PropertyGridTabPage.Location = new System.Drawing.Point(4, 4);
            this.PropertyGridTabPage.Margin = new System.Windows.Forms.Padding(0);
            this.PropertyGridTabPage.Name = "PropertyGridTabPage";
            this.PropertyGridTabPage.Size = new System.Drawing.Size(481, 397);
            this.PropertyGridTabPage.TabIndex = 0;
            this.PropertyGridTabPage.Text = "Properties";
            this.PropertyGridTabPage.UseVisualStyleBackColor = true;
            // 
            // DetailSplitContainer
            // 
            this.DetailSplitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.DetailSplitContainer.Location = new System.Drawing.Point(0, 0);
            this.DetailSplitContainer.Margin = new System.Windows.Forms.Padding(0);
            this.DetailSplitContainer.Name = "DetailSplitContainer";
            this.DetailSplitContainer.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // DetailSplitContainer.Panel1
            // 
            this.DetailSplitContainer.Panel1.Controls.Add(this.DetailPropertyGrid);
            // 
            // DetailSplitContainer.Panel2
            // 
            this.DetailSplitContainer.Panel2.Controls.Add(this.DetailListView);
            this.DetailSplitContainer.Panel2.Controls.Add(this.DetailListViewToolStrip);
            this.DetailSplitContainer.Size = new System.Drawing.Size(481, 397);
            this.DetailSplitContainer.SplitterDistance = 185;
            this.DetailSplitContainer.SplitterWidth = 2;
            this.DetailSplitContainer.TabIndex = 0;
            // 
            // DetailPropertyGrid
            // 
            this.DetailPropertyGrid.CategoryForeColor = System.Drawing.Color.Green;
            this.DetailPropertyGrid.Dock = System.Windows.Forms.DockStyle.Fill;
            this.DetailPropertyGrid.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.DetailPropertyGrid.HelpVisible = false;
            this.DetailPropertyGrid.Location = new System.Drawing.Point(0, 0);
            this.DetailPropertyGrid.Margin = new System.Windows.Forms.Padding(0);
            this.DetailPropertyGrid.Name = "DetailPropertyGrid";
            this.DetailPropertyGrid.Size = new System.Drawing.Size(481, 185);
            this.DetailPropertyGrid.TabIndex = 2;
            this.DetailPropertyGrid.ToolbarVisible = false;
            this.DetailPropertyGrid.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(this.DetailPropertyGrid_PropertyValueChanged);
            // 
            // DetailListView
            // 
            this.DetailListView.Activation = System.Windows.Forms.ItemActivation.OneClick;
            this.DetailListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.DetailListView.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.DetailListView.FullRowSelect = true;
            this.DetailListView.Location = new System.Drawing.Point(0, 25);
            this.DetailListView.MultiSelect = false;
            this.DetailListView.Name = "DetailListView";
            this.DetailListView.ShowItemToolTips = true;
            this.DetailListView.Size = new System.Drawing.Size(481, 185);
            this.DetailListView.SmallImageList = this.ListViewImageList;
            this.DetailListView.TabIndex = 2;
            this.DetailListView.UseCompatibleStateImageBehavior = false;
            this.DetailListView.View = System.Windows.Forms.View.Details;
            this.DetailListView.MouseUp += new System.Windows.Forms.MouseEventHandler(this.DetailListView_MouseUp);
            this.DetailListView.ColumnClick += new System.Windows.Forms.ColumnClickEventHandler(this.DetailListView_ColumnClick);
            this.DetailListView.ItemSelectionChanged += new System.Windows.Forms.ListViewItemSelectionChangedEventHandler(this.DetailListView_ItemSelectionChanged);
            this.DetailListView.KeyDown += new System.Windows.Forms.KeyEventHandler(this.DetailListView_KeyDown);
            // 
            // ListViewImageList
            // 
            this.ListViewImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ListViewImageList.ImageStream")));
            this.ListViewImageList.TransparentColor = System.Drawing.Color.Transparent;
            this.ListViewImageList.Images.SetKeyName(0, "pass64.ico");
            this.ListViewImageList.Images.SetKeyName(1, "fail64.ico");
            this.ListViewImageList.Images.SetKeyName(2, "listbullets.png");
            this.ListViewImageList.Images.SetKeyName(3, "transaction_logfile.ico");
            this.ListViewImageList.Images.SetKeyName(4, "zippedFile.png");
            this.ListViewImageList.Images.SetKeyName(5, "ttCompuer.png");
            this.ListViewImageList.Images.SetKeyName(6, "ttHardwareKeyID.ico");
            this.ListViewImageList.Images.SetKeyName(7, "ttMacAddress.png");
            this.ListViewImageList.Images.SetKeyName(8, "ttBiosSerialNumber.png");
            this.ListViewImageList.Images.SetKeyName(9, "ttLicenseCode.png");
            this.ListViewImageList.Images.SetKeyName(10, "Module.ico");
            this.ListViewImageList.Images.SetKeyName(11, "ValidationTokens.png");
            // 
            // DetailListViewToolStrip
            // 
            this.DetailListViewToolStrip.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.DetailListViewToolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ImageToolStripLabel,
            this.DetailListViewToolStripLabel});
            this.DetailListViewToolStrip.Location = new System.Drawing.Point(0, 0);
            this.DetailListViewToolStrip.Name = "DetailListViewToolStrip";
            this.DetailListViewToolStrip.Size = new System.Drawing.Size(481, 25);
            this.DetailListViewToolStrip.TabIndex = 5;
            // 
            // ImageToolStripLabel
            // 
            this.ImageToolStripLabel.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.ImageToolStripLabel.Name = "ImageToolStripLabel";
            this.ImageToolStripLabel.Size = new System.Drawing.Size(0, 22);
            // 
            // DetailListViewToolStripLabel
            // 
            this.DetailListViewToolStripLabel.Font = new System.Drawing.Font("Tahoma", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.DetailListViewToolStripLabel.ForeColor = System.Drawing.SystemColors.ControlText;
            this.DetailListViewToolStripLabel.Name = "DetailListViewToolStripLabel";
            this.DetailListViewToolStripLabel.Size = new System.Drawing.Size(38, 22);
            this.DetailListViewToolStripLabel.Text = "Label";
            // 
            // ListViewTabPage
            // 
            this.ListViewTabPage.Controls.Add(this.TransactionListView);
            this.ListViewTabPage.Controls.Add(this.TransactionsToolStrip);
            this.ListViewTabPage.ImageIndex = 3;
            this.ListViewTabPage.Location = new System.Drawing.Point(4, 4);
            this.ListViewTabPage.Margin = new System.Windows.Forms.Padding(0);
            this.ListViewTabPage.Name = "ListViewTabPage";
            this.ListViewTabPage.Size = new System.Drawing.Size(481, 397);
            this.ListViewTabPage.TabIndex = 1;
            this.ListViewTabPage.Text = "Transactions";
            this.ListViewTabPage.UseVisualStyleBackColor = true;
            // 
            // TransactionListView
            // 
            this.TransactionListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.TransactionDateColumnHeader,
            this.DescriptionColumnHeader,
            this.ValueColumnHeader,
            this.UserColumnHeader});
            this.TransactionListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.TransactionListView.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.TransactionListView.FullRowSelect = true;
            this.TransactionListView.GridLines = true;
            this.TransactionListView.Location = new System.Drawing.Point(0, 25);
            this.TransactionListView.Margin = new System.Windows.Forms.Padding(0);
            this.TransactionListView.Name = "TransactionListView";
            this.TransactionListView.NoItemsMessage = "";
            this.TransactionListView.Size = new System.Drawing.Size(481, 372);
            this.TransactionListView.Sorting = System.Windows.Forms.SortOrder.Descending;
            this.TransactionListView.TabIndex = 0;
            this.TransactionListView.UseCompatibleStateImageBehavior = false;
            this.TransactionListView.View = System.Windows.Forms.View.Details;
            this.TransactionListView.ColumnClick += new System.Windows.Forms.ColumnClickEventHandler(this.TransactionListView_ColumnClick);
            // 
            // TransactionDateColumnHeader
            // 
            this.TransactionDateColumnHeader.Text = "Date";
            this.TransactionDateColumnHeader.Width = 88;
            // 
            // DescriptionColumnHeader
            // 
            this.DescriptionColumnHeader.Text = "Description";
            this.DescriptionColumnHeader.Width = 81;
            // 
            // ValueColumnHeader
            // 
            this.ValueColumnHeader.Text = "Value";
            // 
            // UserColumnHeader
            // 
            this.UserColumnHeader.Text = "User";
            this.UserColumnHeader.Width = 79;
            // 
            // TransactionsToolStrip
            // 
            this.TransactionsToolStrip.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.TransactionsToolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.GroupToolStripButton,
            this.SingleToolStripButton});
            this.TransactionsToolStrip.Location = new System.Drawing.Point(0, 0);
            this.TransactionsToolStrip.Name = "TransactionsToolStrip";
            this.TransactionsToolStrip.Size = new System.Drawing.Size(481, 25);
            this.TransactionsToolStrip.TabIndex = 1;
            this.TransactionsToolStrip.Text = "toolStrip1";
            // 
            // GroupToolStripButton
            // 
            this.GroupToolStripButton.Checked = true;
            this.GroupToolStripButton.CheckState = System.Windows.Forms.CheckState.Checked;
            this.GroupToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.GroupToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("GroupToolStripButton.Image")));
            this.GroupToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.GroupToolStripButton.Margin = new System.Windows.Forms.Padding(5, 1, 0, 2);
            this.GroupToolStripButton.Name = "GroupToolStripButton";
            this.GroupToolStripButton.Size = new System.Drawing.Size(23, 22);
            this.GroupToolStripButton.Text = "GroupView";
            this.GroupToolStripButton.Click += new System.EventHandler(this.GroupToolStripButton_Click);
            // 
            // SingleToolStripButton
            // 
            this.SingleToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.SingleToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("SingleToolStripButton.Image")));
            this.SingleToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.SingleToolStripButton.Name = "SingleToolStripButton";
            this.SingleToolStripButton.Size = new System.Drawing.Size(23, 22);
            this.SingleToolStripButton.Click += new System.EventHandler(this.SingleToolStripButton_Click);
            // 
            // PacketTabPage
            // 
            this.PacketTabPage.Controls.Add(this.splitContainer1);
            this.PacketTabPage.Controls.Add(this.PacketListToolStrip);
            this.PacketTabPage.ImageIndex = 4;
            this.PacketTabPage.Location = new System.Drawing.Point(4, 4);
            this.PacketTabPage.Name = "PacketTabPage";
            this.PacketTabPage.Size = new System.Drawing.Size(481, 397);
            this.PacketTabPage.TabIndex = 2;
            this.PacketTabPage.Text = "Packets";
            this.PacketTabPage.UseVisualStyleBackColor = true;
            // 
            // splitContainer1
            // 
            this.splitContainer1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
            this.splitContainer1.Location = new System.Drawing.Point(0, 25);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.LicensePacketListView);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.BackColor = System.Drawing.SystemColors.Control;
            this.splitContainer1.Panel2.Paint += new System.Windows.Forms.PaintEventHandler(this.splitContainer1_Panel2_Paint);
            this.splitContainer1.Panel2Collapsed = true;
            this.splitContainer1.Size = new System.Drawing.Size(481, 372);
            this.splitContainer1.SplitterDistance = 311;
            this.splitContainer1.SplitterWidth = 1;
            this.splitContainer1.TabIndex = 6;
            // 
            // LicensePacketListView
            // 
            this.LicensePacketListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.PktNameColumnHeader,
            this.PktCodeColumnHeader,
            this.PktDateColumnHeader,
            this.PktDescriptionColumnHeader,
            this.PktUserColumnHeader});
            this.LicensePacketListView.ContextMenuStrip = this.PacketContextMenuStrip;
            this.LicensePacketListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.LicensePacketListView.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LicensePacketListView.FullRowSelect = true;
            this.LicensePacketListView.Location = new System.Drawing.Point(0, 0);
            this.LicensePacketListView.MultiSelect = false;
            this.LicensePacketListView.Name = "LicensePacketListView";
            this.LicensePacketListView.NoItemsMessage = "";
            this.LicensePacketListView.Size = new System.Drawing.Size(479, 370);
            this.LicensePacketListView.StateImageList = this.ListViewImageList;
            this.LicensePacketListView.TabIndex = 4;
            this.LicensePacketListView.UseCompatibleStateImageBehavior = false;
            this.LicensePacketListView.View = System.Windows.Forms.View.Details;
            this.LicensePacketListView.SelectedIndexChanged += new System.EventHandler(this.LicensePacketListView_SelectedIndexChanged);
            // 
            // PktNameColumnHeader
            // 
            this.PktNameColumnHeader.Text = "Name";
            this.PktNameColumnHeader.Width = 56;
            // 
            // PktCodeColumnHeader
            // 
            this.PktCodeColumnHeader.Text = "Verification Code";
            this.PktCodeColumnHeader.Width = 102;
            // 
            // PktDateColumnHeader
            // 
            this.PktDateColumnHeader.Text = "Date Created";
            this.PktDateColumnHeader.Width = 84;
            // 
            // PktDescriptionColumnHeader
            // 
            this.PktDescriptionColumnHeader.Text = "Description";
            this.PktDescriptionColumnHeader.Width = 77;
            // 
            // PktUserColumnHeader
            // 
            this.PktUserColumnHeader.Text = "User";
            // 
            // PacketContextMenuStrip
            // 
            this.PacketContextMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.verifyToolStripMenuItem});
            this.PacketContextMenuStrip.Name = "PacketContextMenuStrip";
            this.PacketContextMenuStrip.Size = new System.Drawing.Size(114, 26);
            // 
            // verifyToolStripMenuItem
            // 
            this.verifyToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("verifyToolStripMenuItem.Image")));
            this.verifyToolStripMenuItem.Name = "verifyToolStripMenuItem";
            this.verifyToolStripMenuItem.Size = new System.Drawing.Size(113, 22);
            this.verifyToolStripMenuItem.Text = "Verify";
            this.verifyToolStripMenuItem.Click += new System.EventHandler(this.verifyToolStripMenuItem_Click);
            // 
            // PacketListToolStrip
            // 
            this.PacketListToolStrip.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.PacketListToolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripLabel1,
            this.toolStripComboBox1,
            this.showAllToolStripButton,
            this.showVerifiedToolStripButton,
            this.showUnVerifiedToolStripButton});
            this.PacketListToolStrip.Location = new System.Drawing.Point(0, 0);
            this.PacketListToolStrip.Name = "PacketListToolStrip";
            this.PacketListToolStrip.Size = new System.Drawing.Size(481, 25);
            this.PacketListToolStrip.TabIndex = 5;
            // 
            // toolStripLabel1
            // 
            this.toolStripLabel1.Name = "toolStripLabel1";
            this.toolStripLabel1.Size = new System.Drawing.Size(56, 22);
            this.toolStripLabel1.Text = "View Filter";
            // 
            // toolStripComboBox1
            // 
            this.toolStripComboBox1.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.toolStripComboBox1.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.toolStripComboBox1.Items.AddRange(new object[] {
            "All",
            "Verified",
            "Unverified"});
            this.toolStripComboBox1.Margin = new System.Windows.Forms.Padding(5, 1, 1, 1);
            this.toolStripComboBox1.Name = "toolStripComboBox1";
            this.toolStripComboBox1.Size = new System.Drawing.Size(121, 23);
            this.toolStripComboBox1.SelectedIndexChanged += new System.EventHandler(this.toolStripComboBox1_SelectedIndexChanged);
            // 
            // showAllToolStripButton
            // 
            this.showAllToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.showAllToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("showAllToolStripButton.Image")));
            this.showAllToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.showAllToolStripButton.Name = "showAllToolStripButton";
            this.showAllToolStripButton.Size = new System.Drawing.Size(23, 22);
            this.showAllToolStripButton.Text = "All";
            this.showAllToolStripButton.Visible = false;
            this.showAllToolStripButton.Click += new System.EventHandler(this.showAllToolStripButton_Click);
            // 
            // showVerifiedToolStripButton
            // 
            this.showVerifiedToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.showVerifiedToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("showVerifiedToolStripButton.Image")));
            this.showVerifiedToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.showVerifiedToolStripButton.Name = "showVerifiedToolStripButton";
            this.showVerifiedToolStripButton.Size = new System.Drawing.Size(23, 22);
            this.showVerifiedToolStripButton.Text = "Verified";
            this.showVerifiedToolStripButton.Visible = false;
            this.showVerifiedToolStripButton.Click += new System.EventHandler(this.showVerifiedToolStripButton_Click);
            // 
            // showUnVerifiedToolStripButton
            // 
            this.showUnVerifiedToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.showUnVerifiedToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("showUnVerifiedToolStripButton.Image")));
            this.showUnVerifiedToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.showUnVerifiedToolStripButton.Name = "showUnVerifiedToolStripButton";
            this.showUnVerifiedToolStripButton.Size = new System.Drawing.Size(23, 22);
            this.showUnVerifiedToolStripButton.Text = "UnVerified";
            this.showUnVerifiedToolStripButton.Visible = false;
            this.showUnVerifiedToolStripButton.Click += new System.EventHandler(this.showUnVerifiedToolStripButton_Click);
            // 
            // PacketsTabPage
            // 
            this.PacketsTabPage.Controls.Add(this.PacketListView);
            this.PacketsTabPage.Location = new System.Drawing.Point(4, 22);
            this.PacketsTabPage.Name = "PacketsTabPage";
            this.PacketsTabPage.Padding = new System.Windows.Forms.Padding(3);
            this.PacketsTabPage.Size = new System.Drawing.Size(808, 428);
            this.PacketsTabPage.TabIndex = 2;
            this.PacketsTabPage.Text = "Packets";
            this.PacketsTabPage.UseVisualStyleBackColor = true;
            // 
            // PacketListView
            // 
            this.PacketListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.statusColumnHeader,
            this.nameColumnHeader,
            this.dateColumnHeader,
            this.vCodeColumnHeader,
            this.commentsColumnHeader,
            this.userNameColumnHeader});
            this.PacketListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.PacketListView.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.PacketListView.FullRowSelect = true;
            this.PacketListView.Location = new System.Drawing.Point(3, 3);
            this.PacketListView.Name = "PacketListView";
            this.PacketListView.Size = new System.Drawing.Size(802, 422);
            this.PacketListView.TabIndex = 0;
            this.PacketListView.UseCompatibleStateImageBehavior = false;
            this.PacketListView.View = System.Windows.Forms.View.Details;
            // 
            // statusColumnHeader
            // 
            this.statusColumnHeader.Text = "Status";
            this.statusColumnHeader.Width = 66;
            // 
            // nameColumnHeader
            // 
            this.nameColumnHeader.Text = "Name";
            // 
            // dateColumnHeader
            // 
            this.dateColumnHeader.Text = "Date Created";
            this.dateColumnHeader.Width = 100;
            // 
            // vCodeColumnHeader
            // 
            this.vCodeColumnHeader.Text = "Verification Code";
            this.vCodeColumnHeader.Width = 116;
            // 
            // commentsColumnHeader
            // 
            this.commentsColumnHeader.Text = "Comments";
            this.commentsColumnHeader.Width = 82;
            // 
            // userNameColumnHeader
            // 
            this.userNameColumnHeader.Text = "User";
            this.userNameColumnHeader.Width = 92;
            // 
            // ReportTabPage
            // 
            this.ReportTabPage.Controls.Add(this.ReportListView);
            this.ReportTabPage.Location = new System.Drawing.Point(4, 22);
            this.ReportTabPage.Name = "ReportTabPage";
            this.ReportTabPage.Padding = new System.Windows.Forms.Padding(3);
            this.ReportTabPage.Size = new System.Drawing.Size(808, 428);
            this.ReportTabPage.TabIndex = 3;
            this.ReportTabPage.Text = "Report";
            this.ReportTabPage.UseVisualStyleBackColor = true;
            // 
            // ReportListView
            // 
            this.ReportListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
            this.ReportListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ReportListView.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ReportListView.Location = new System.Drawing.Point(3, 3);
            this.ReportListView.Name = "ReportListView";
            this.ReportListView.Size = new System.Drawing.Size(802, 422);
            this.ReportListView.TabIndex = 0;
            this.ReportListView.UseCompatibleStateImageBehavior = false;
            this.ReportListView.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "License Name";
            this.columnHeader1.Width = 99;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Description";
            this.columnHeader2.Width = 104;
            // 
            // HardwareKeyTabPage
            // 
            this.HardwareKeyTabPage.Controls.Add(this.panel1);
            this.HardwareKeyTabPage.Location = new System.Drawing.Point(4, 22);
            this.HardwareKeyTabPage.Name = "HardwareKeyTabPage";
            this.HardwareKeyTabPage.Size = new System.Drawing.Size(808, 428);
            this.HardwareKeyTabPage.TabIndex = 4;
            this.HardwareKeyTabPage.Text = "Hardware Keys";
            this.HardwareKeyTabPage.UseVisualStyleBackColor = true;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.HardwareKeyListView);
            this.panel1.Controls.Add(this.toolStrip1);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(808, 428);
            this.panel1.TabIndex = 0;
            // 
            // HardwareKeyListView
            // 
            this.HardwareKeyListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader3,
            this.columnHeader4,
            this.columnHeader5});
            this.HardwareKeyListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.HardwareKeyListView.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.HardwareKeyListView.FullRowSelect = true;
            this.HardwareKeyListView.GridLines = true;
            this.HardwareKeyListView.Location = new System.Drawing.Point(0, 25);
            this.HardwareKeyListView.Name = "HardwareKeyListView";
            this.HardwareKeyListView.Size = new System.Drawing.Size(808, 403);
            this.HardwareKeyListView.TabIndex = 1;
            this.HardwareKeyListView.UseCompatibleStateImageBehavior = false;
            this.HardwareKeyListView.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "Customer";
            this.columnHeader3.Width = 98;
            // 
            // columnHeader4
            // 
            this.columnHeader4.Text = "Key Value";
            this.columnHeader4.Width = 105;
            // 
            // columnHeader5
            // 
            this.columnHeader5.Text = "License Name";
            this.columnHeader5.Width = 137;
            // 
            // toolStrip1
            // 
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addHardwareKeyToolStripButton});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(808, 25);
            this.toolStrip1.TabIndex = 0;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // addHardwareKeyToolStripButton
            // 
            this.addHardwareKeyToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.addHardwareKeyToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("addHardwareKeyToolStripButton.Image")));
            this.addHardwareKeyToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.addHardwareKeyToolStripButton.Name = "addHardwareKeyToolStripButton";
            this.addHardwareKeyToolStripButton.Size = new System.Drawing.Size(23, 22);
            this.addHardwareKeyToolStripButton.Text = "Add Hardware Key";
            this.addHardwareKeyToolStripButton.Click += new System.EventHandler(this.addHardwareKeyToolStripButton_Click);
            // 
            // loadingCircle1
            // 
            this.loadingCircle1.Active = false;
            this.loadingCircle1.BackColor = System.Drawing.Color.Transparent;
            this.loadingCircle1.Color = System.Drawing.Color.DarkGray;
            this.loadingCircle1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.loadingCircle1.InnerCircleRadius = 5;
            this.loadingCircle1.Location = new System.Drawing.Point(0, 0);
            this.loadingCircle1.Name = "loadingCircle1";
            this.loadingCircle1.NumberSpoke = 12;
            this.loadingCircle1.OuterCircleRadius = 11;
            this.loadingCircle1.RotationSpeed = 100;
            this.loadingCircle1.Size = new System.Drawing.Size(816, 454);
            this.loadingCircle1.SpokeThickness = 2;
            this.loadingCircle1.StylePreset = Shared.VisualComponents.LoadingCircle.StylePresets.MacOSX;
            this.loadingCircle1.TabIndex = 6;
            this.loadingCircle1.Text = "loadingCircle1";
            // 
            // DetailListViewContextMenuStrip
            // 
            this.DetailListViewContextMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.dlvNewToolStripMenuItem,
            this.dlvEditToolStripMenuItem,
            this.dlvDeleteToolStripMenuItem});
            this.DetailListViewContextMenuStrip.Name = "DetailListViewContextMenuStrip";
            this.DetailListViewContextMenuStrip.Size = new System.Drawing.Size(117, 70);
            // 
            // dlvNewToolStripMenuItem
            // 
            this.dlvNewToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("dlvNewToolStripMenuItem.Image")));
            this.dlvNewToolStripMenuItem.Name = "dlvNewToolStripMenuItem";
            this.dlvNewToolStripMenuItem.Size = new System.Drawing.Size(116, 22);
            this.dlvNewToolStripMenuItem.Text = "New";
            this.dlvNewToolStripMenuItem.Click += new System.EventHandler(this.dlvNewToolStripMenuItem_Click);
            // 
            // dlvEditToolStripMenuItem
            // 
            this.dlvEditToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("dlvEditToolStripMenuItem.Image")));
            this.dlvEditToolStripMenuItem.Name = "dlvEditToolStripMenuItem";
            this.dlvEditToolStripMenuItem.Size = new System.Drawing.Size(116, 22);
            this.dlvEditToolStripMenuItem.Text = "Edit";
            this.dlvEditToolStripMenuItem.Click += new System.EventHandler(this.dlvEditToolStripMenuItem_Click);
            // 
            // dlvDeleteToolStripMenuItem
            // 
            this.dlvDeleteToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("dlvDeleteToolStripMenuItem.Image")));
            this.dlvDeleteToolStripMenuItem.Name = "dlvDeleteToolStripMenuItem";
            this.dlvDeleteToolStripMenuItem.Size = new System.Drawing.Size(116, 22);
            this.dlvDeleteToolStripMenuItem.Text = "Delete";
            this.dlvDeleteToolStripMenuItem.Click += new System.EventHandler(this.dlvDeleteToolStripMenuItem_Click);
            // 
            // MainTreeViewContextMenuStrip
            // 
            this.MainTreeViewContextMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.editReportToolStripMenuItem,
            this.toolStripSeparator2,
            this.deleteToolStripMenuItem1});
            this.MainTreeViewContextMenuStrip.Name = "MainTreeViewContextMenuStrip";
            this.MainTreeViewContextMenuStrip.Size = new System.Drawing.Size(140, 54);
            // 
            // editReportToolStripMenuItem
            // 
            this.editReportToolStripMenuItem.Name = "editReportToolStripMenuItem";
            this.editReportToolStripMenuItem.Size = new System.Drawing.Size(139, 22);
            this.editReportToolStripMenuItem.Text = "Edit Report";
            this.editReportToolStripMenuItem.Click += new System.EventHandler(this.editReportToolStripMenuItem_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(136, 6);
            // 
            // deleteToolStripMenuItem1
            // 
            this.deleteToolStripMenuItem1.Name = "deleteToolStripMenuItem1";
            this.deleteToolStripMenuItem1.Size = new System.Drawing.Size(139, 22);
            this.deleteToolStripMenuItem1.Text = "Delete";
            // 
            // LicenseContextMenuStrip
            // 
            this.LicenseContextMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addToolStripMenuItem,
            this.lcmToolStripSeparator1,
            this.refreshToolStripMenuItem,
            this.deleteToolStripMenuItem,
            this.lcmToolStripSeparator2,
            this.createPacketFileToolStripMenuItem});
            this.LicenseContextMenuStrip.Name = "LicenseContextMenuStrip";
            this.LicenseContextMenuStrip.Size = new System.Drawing.Size(188, 104);
            // 
            // addToolStripMenuItem
            // 
            this.addToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newCustomerToolStripMenuItem,
            this.newLicenseToolStripMenuItem,
            this.newOrderToolStripMenuItem,
            this.existingOrderToolStripMenuItem,
            this.addLicenseToolStripSeparator,
            this.newFailoverToolStripMenuItem,
            this.newDisRecToolStripMenuItem,
            this.newTestDevToolStripMenuItem});
            this.addToolStripMenuItem.Name = "addToolStripMenuItem";
            this.addToolStripMenuItem.Size = new System.Drawing.Size(187, 22);
            this.addToolStripMenuItem.Text = "Add";
            // 
            // newCustomerToolStripMenuItem
            // 
            this.newCustomerToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("newCustomerToolStripMenuItem.Image")));
            this.newCustomerToolStripMenuItem.Name = "newCustomerToolStripMenuItem";
            this.newCustomerToolStripMenuItem.Size = new System.Drawing.Size(223, 22);
            this.newCustomerToolStripMenuItem.Text = "New Customer...";
            this.newCustomerToolStripMenuItem.Click += new System.EventHandler(this.newCustomerToolStripMenuItem_Click);
            // 
            // newLicenseToolStripMenuItem
            // 
            this.newLicenseToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("newLicenseToolStripMenuItem.Image")));
            this.newLicenseToolStripMenuItem.Name = "newLicenseToolStripMenuItem";
            this.newLicenseToolStripMenuItem.Size = new System.Drawing.Size(223, 22);
            this.newLicenseToolStripMenuItem.Text = "New Standard License...";
            this.newLicenseToolStripMenuItem.Click += new System.EventHandler(this.newLicenseToolStripMenuItem_Click);
            // 
            // newOrderToolStripMenuItem
            // 
            this.newOrderToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("newOrderToolStripMenuItem.Image")));
            this.newOrderToolStripMenuItem.Name = "newOrderToolStripMenuItem";
            this.newOrderToolStripMenuItem.Size = new System.Drawing.Size(223, 22);
            this.newOrderToolStripMenuItem.Text = "New Order...";
            this.newOrderToolStripMenuItem.Click += new System.EventHandler(this.newOrderToolStripMenuItem_Click);
            // 
            // existingOrderToolStripMenuItem
            // 
            this.existingOrderToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("existingOrderToolStripMenuItem.Image")));
            this.existingOrderToolStripMenuItem.Name = "existingOrderToolStripMenuItem";
            this.existingOrderToolStripMenuItem.Size = new System.Drawing.Size(223, 22);
            this.existingOrderToolStripMenuItem.Text = "Existing Order...";
            this.existingOrderToolStripMenuItem.Click += new System.EventHandler(this.existingOrderToolStripMenuItem_Click);
            // 
            // addLicenseToolStripSeparator
            // 
            this.addLicenseToolStripSeparator.Name = "addLicenseToolStripSeparator";
            this.addLicenseToolStripSeparator.Size = new System.Drawing.Size(220, 6);
            // 
            // newFailoverToolStripMenuItem
            // 
            this.newFailoverToolStripMenuItem.Name = "newFailoverToolStripMenuItem";
            this.newFailoverToolStripMenuItem.Size = new System.Drawing.Size(223, 22);
            this.newFailoverToolStripMenuItem.Text = "Failover License...";
            this.newFailoverToolStripMenuItem.Click += new System.EventHandler(this.newFailoverLicenseToolStripMenuItem_Click);
            // 
            // newDisRecToolStripMenuItem
            // 
            this.newDisRecToolStripMenuItem.Name = "newDisRecToolStripMenuItem";
            this.newDisRecToolStripMenuItem.Size = new System.Drawing.Size(223, 22);
            this.newDisRecToolStripMenuItem.Text = "Disaster Recovery License...";
            this.newDisRecToolStripMenuItem.Click += new System.EventHandler(this.newDRLicenseToolStripMenuItem_Click);
            // 
            // newTestDevToolStripMenuItem
            // 
            this.newTestDevToolStripMenuItem.Name = "newTestDevToolStripMenuItem";
            this.newTestDevToolStripMenuItem.Size = new System.Drawing.Size(223, 22);
            this.newTestDevToolStripMenuItem.Text = "Test Development License...";
            this.newTestDevToolStripMenuItem.Click += new System.EventHandler(this.newTestDevLicenseToolStripMenuItem_Click);
            // 
            // lcmToolStripSeparator1
            // 
            this.lcmToolStripSeparator1.Name = "lcmToolStripSeparator1";
            this.lcmToolStripSeparator1.Size = new System.Drawing.Size(184, 6);
            // 
            // refreshToolStripMenuItem
            // 
            this.refreshToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("refreshToolStripMenuItem.Image")));
            this.refreshToolStripMenuItem.Name = "refreshToolStripMenuItem";
            this.refreshToolStripMenuItem.Size = new System.Drawing.Size(187, 22);
            this.refreshToolStripMenuItem.Text = "Refresh";
            this.refreshToolStripMenuItem.Click += new System.EventHandler(this.refreshToolStripMenuItem_Click);
            // 
            // deleteToolStripMenuItem
            // 
            this.deleteToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("deleteToolStripMenuItem.Image")));
            this.deleteToolStripMenuItem.Name = "deleteToolStripMenuItem";
            this.deleteToolStripMenuItem.Size = new System.Drawing.Size(187, 22);
            this.deleteToolStripMenuItem.Text = "Delete";
            this.deleteToolStripMenuItem.Click += new System.EventHandler(this.deleteToolStripMenuItem_Click);
            // 
            // lcmToolStripSeparator2
            // 
            this.lcmToolStripSeparator2.Name = "lcmToolStripSeparator2";
            this.lcmToolStripSeparator2.Size = new System.Drawing.Size(184, 6);
            // 
            // createPacketFileToolStripMenuItem
            // 
            this.createPacketFileToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("createPacketFileToolStripMenuItem.Image")));
            this.createPacketFileToolStripMenuItem.Name = "createPacketFileToolStripMenuItem";
            this.createPacketFileToolStripMenuItem.Size = new System.Drawing.Size(187, 22);
            this.createPacketFileToolStripMenuItem.Text = "Generate License File";
            this.createPacketFileToolStripMenuItem.Click += new System.EventHandler(this.createPacketFileToolStripMenuItem_Click);
            // 
            // CreatorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.ClientSize = new System.Drawing.Size(993, 529);
            this.Controls.Add(this.LicenseViewPanel);
            this.Controls.Add(this.MainStatusStrip);
            this.Controls.Add(this.MainToolStrip);
            this.Controls.Add(this.MainMenuStrip);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Name = "CreatorForm";
            this.Text = "Creator";
            this.Load += new System.EventHandler(this.CreatorForm_Load);
            this.MainMenuStrip.ResumeLayout(false);
            this.MainMenuStrip.PerformLayout();
            this.MainToolStrip.ResumeLayout(false);
            this.MainToolStrip.PerformLayout();
            this.MainStatusStrip.ResumeLayout(false);
            this.MainStatusStrip.PerformLayout();
            this.LicenseViewPanel.ResumeLayout(false);
            this.MainSplitContainer.Panel1.ResumeLayout(false);
            this.MainSplitContainer.Panel2.ResumeLayout(false);
            this.MainSplitContainer.ResumeLayout(false);
            this.MainTabControl.ResumeLayout(false);
            this.LicensesTabPage.ResumeLayout(false);
            this.SubSplitContainer.Panel1.ResumeLayout(false);
            this.SubSplitContainer.Panel2.ResumeLayout(false);
            this.SubSplitContainer.ResumeLayout(false);
            this.PropertyGridTabControl.ResumeLayout(false);
            this.PropertyGridTabPage.ResumeLayout(false);
            this.DetailSplitContainer.Panel1.ResumeLayout(false);
            this.DetailSplitContainer.Panel2.ResumeLayout(false);
            this.DetailSplitContainer.Panel2.PerformLayout();
            this.DetailSplitContainer.ResumeLayout(false);
            this.DetailListViewToolStrip.ResumeLayout(false);
            this.DetailListViewToolStrip.PerformLayout();
            this.ListViewTabPage.ResumeLayout(false);
            this.ListViewTabPage.PerformLayout();
            this.TransactionsToolStrip.ResumeLayout(false);
            this.TransactionsToolStrip.PerformLayout();
            this.PacketTabPage.ResumeLayout(false);
            this.PacketTabPage.PerformLayout();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.PacketContextMenuStrip.ResumeLayout(false);
            this.PacketListToolStrip.ResumeLayout(false);
            this.PacketListToolStrip.PerformLayout();
            this.PacketsTabPage.ResumeLayout(false);
            this.ReportTabPage.ResumeLayout(false);
            this.HardwareKeyTabPage.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.DetailListViewContextMenuStrip.ResumeLayout(false);
            this.MainTreeViewContextMenuStrip.ResumeLayout(false);
            this.LicenseContextMenuStrip.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip MainMenuStrip;
        private System.Windows.Forms.ToolStrip MainToolStrip;
        private System.Windows.Forms.ToolStripTextBox toolStripSearchBox;
        private System.Windows.Forms.StatusStrip MainStatusStrip;
        private System.Windows.Forms.Panel LicenseViewPanel;
        private System.Windows.Forms.ContextMenuStrip LicenseContextMenuStrip;
        private System.Windows.Forms.ToolStripMenuItem deleteToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator lcmToolStripSeparator2;
        private System.Windows.Forms.ToolStripSeparator lcmToolStripSeparator1;
        private System.Windows.Forms.SplitContainer MainSplitContainer;
        private System.Windows.Forms.TreeView MainTreeView;
        private System.Windows.Forms.SplitContainer SubSplitContainer;
        private System.Windows.Forms.TreeView DetailTreeView;
        private System.Windows.Forms.TabControl MainTabControl;
        private System.Windows.Forms.TabPage LicensesTabPage;
        private System.Windows.Forms.TabPage PacketsTabPage;
        private Shared.VisualComponents.NoFlickerListView PacketListView;
        private System.Windows.Forms.ColumnHeader nameColumnHeader;
        private System.Windows.Forms.ColumnHeader vCodeColumnHeader;
        private System.Windows.Forms.ColumnHeader dateColumnHeader;
        private System.Windows.Forms.ColumnHeader statusColumnHeader;
        private System.Windows.Forms.ToolStripButton createPacketMainToolStripBtn;
        private System.Windows.Forms.ColumnHeader commentsColumnHeader;
        private System.Windows.Forms.ToolStripDropDownButton toolStripSearchButton;
        private System.Windows.Forms.TabControl PropertyGridTabControl;
        private System.Windows.Forms.TabPage PropertyGridTabPage;
        private System.Windows.Forms.TabPage ListViewTabPage;
        private Shared.VisualComponents.NoFlickerListView TransactionListView;
        private System.Windows.Forms.ColumnHeader TransactionDateColumnHeader;
        private System.Windows.Forms.ColumnHeader DescriptionColumnHeader;
        private System.Windows.Forms.ColumnHeader ValueColumnHeader;
        private System.Windows.Forms.ColumnHeader UserColumnHeader;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripMenuItem exitMainToolStripMenuItem;
        private System.Windows.Forms.ImageList DetailTreeViewImageList;
        private System.Windows.Forms.ImageList MainTreeViewImageList;
        private System.Windows.Forms.SplitContainer DetailSplitContainer;
        private System.Windows.Forms.PropertyGrid DetailPropertyGrid;
        private System.Windows.Forms.ContextMenuStrip DetailListViewContextMenuStrip;
        private System.Windows.Forms.ToolStripMenuItem dlvNewToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem dlvEditToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem dlvDeleteToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem createPacketFileToolStripMenuItem;
        private System.Windows.Forms.ListView DetailListView;
        private System.Windows.Forms.ContextMenuStrip PacketContextMenuStrip;
        private System.Windows.Forms.TabPage PacketTabPage;
        private System.Windows.Forms.ToolStripMenuItem verifyToolStripMenuItem;
        private System.Windows.Forms.ImageList ListViewImageList;
        private System.Windows.Forms.ToolStripStatusLabel progressBarStatusLabel;
        private System.Windows.Forms.ToolStrip PacketListToolStrip;
        private System.Windows.Forms.ToolStripButton showAllToolStripButton;
        private System.Windows.Forms.ToolStripButton showVerifiedToolStripButton;
        private System.Windows.Forms.ToolStripButton showUnVerifiedToolStripButton;
        private System.Windows.Forms.TabPage ReportTabPage;
        private Shared.VisualComponents.NoFlickerListView ReportListView;
        private System.Windows.Forms.ContextMenuStrip MainTreeViewContextMenuStrip;
        private System.Windows.Forms.ToolStripMenuItem editReportToolStripMenuItem;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ToolStripMenuItem newReportToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripMenuItem deleteToolStripMenuItem1;
        private System.Windows.Forms.ToolStrip DetailListViewToolStrip;
        private System.Windows.Forms.ToolStripLabel DetailListViewToolStripLabel;
        private System.Windows.Forms.ToolStrip TransactionsToolStrip;
        private System.Windows.Forms.ToolStripButton GroupToolStripButton;
        private System.Windows.Forms.ToolStripButton SingleToolStripButton;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private Shared.VisualComponents.NoFlickerListView LicensePacketListView;
        private System.Windows.Forms.ColumnHeader PktNameColumnHeader;
        private System.Windows.Forms.ColumnHeader PktCodeColumnHeader;
        private System.Windows.Forms.ColumnHeader PktDateColumnHeader;
        private System.Windows.Forms.ColumnHeader PktDescriptionColumnHeader;
        private System.Windows.Forms.ColumnHeader PktUserColumnHeader;
        private System.Windows.Forms.ColumnHeader userNameColumnHeader;
        private Shared.VisualComponents.LoadingCircle loadingCircle1;
        private System.Windows.Forms.ToolStripButton newCustMainToolStripBtn;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator7;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator8;
        private System.Windows.Forms.ToolStripButton toggleViewMainToolStripBtn;
        private System.Windows.Forms.ToolStripComboBox toolStripComboBox1;
        private System.Windows.Forms.ToolStripStatusLabel countToolStripStatusLabel;
        private System.Windows.Forms.ToolStripLabel toolStripLabel1;
        private System.Windows.Forms.ToolStripLabel ImageToolStripLabel;
        private System.Windows.Forms.TabPage HardwareKeyTabPage;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private Shared.VisualComponents.NoFlickerListView HardwareKeyListView;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.ColumnHeader columnHeader4;
        private System.Windows.Forms.ColumnHeader columnHeader5;
        private System.Windows.Forms.ToolStripButton addHardwareKeyToolStripButton;
        private System.Windows.Forms.ToolStripMenuItem refreshToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem addToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newCustomerToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newLicenseToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newOrderToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem existingOrderToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator addLicenseToolStripSeparator;
        private System.Windows.Forms.ToolStripMenuItem newFailoverToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newDisRecToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newTestDevToolStripMenuItem;
        private System.Windows.Forms.ToolStripButton removeCustMainToolStripBtn;
        private System.Windows.Forms.ToolStripButton removeLicMainToolStripBtn;
        private System.Windows.Forms.ToolStripButton removeOrderMainToolStripBtn;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator9;
        private System.Windows.Forms.ToolStripButton refreshMainToolStripBtn;
        private System.Windows.Forms.ToolStripSplitButton addLicMainToolStripBtn;
        private System.Windows.Forms.ToolStripMenuItem newStandardLicenseToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newFailoverLicenseToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newDisasterRecoveryLicenseToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newTestDevelopmentLicenseToolStripMenuItem;
        private System.Windows.Forms.ToolStripSplitButton addOrderMainToolStripDropDownBtn;
        private System.Windows.Forms.ToolStripMenuItem existingOrderMainToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newOrderMainToolStripMenuItem;

    }
}

