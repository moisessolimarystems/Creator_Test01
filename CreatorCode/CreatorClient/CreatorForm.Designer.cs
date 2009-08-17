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
            this.connectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.newReportToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.exitMainToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.optionsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.MainToolStrip = new System.Windows.Forms.ToolStrip();
            this.navigateBackToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.navigateForwardToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator5 = new System.Windows.Forms.ToolStripSeparator();
            this.newCustMainToolStripBtn = new System.Windows.Forms.ToolStripButton();
            this.addLicMainToolStripBtn = new System.Windows.Forms.ToolStripSplitButton();
            this.newStandardLicenseToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newFailoverLicenseToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newDisasterRecoveryLicenseToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newTestDevelopmentLicenseToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addProductLicenseMainToolStripDropDownBtn = new System.Windows.Forms.ToolStripSplitButton();
            this.newProductLicenseMainToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.existingProductLicenseMainToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator7 = new System.Windows.Forms.ToolStripSeparator();
            this.SearchToolStripLabel = new System.Windows.Forms.ToolStripLabel();
            this.searchToolStripTextBox = new System.Windows.Forms.ToolStripTextBox();
            this.createPacketMainToolStripBtn = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.toggleViewMainToolStripBtn = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator8 = new System.Windows.Forms.ToolStripSeparator();
            this.seekKeyToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.reserveHardwareKeyToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.deactivateHardwareKeyToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.activateHardwareKeyToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.LicenseViewPanel = new System.Windows.Forms.Panel();
            this.splitContainer2 = new System.Windows.Forms.SplitContainer();
            this.MainTabControl = new System.Windows.Forms.TabControl();
            this.LicensesTabPage = new System.Windows.Forms.TabPage();
            this.CustomersListView = new Shared.VisualComponents.NoFlickerListView();
            this.custNumColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.custNameColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.LicenseViewSplitContainer = new System.Windows.Forms.SplitContainer();
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
            this.dlvRemoveToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.dlvAddToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.dlvEditToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.moduleFilterToolStripComboBox = new System.Windows.Forms.ToolStripComboBox();
            this.loadingCircle1 = new Shared.VisualComponents.LoadingCircle();
            this.TransactionsTabPage = new System.Windows.Forms.TabPage();
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
            this.PacketNameColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.CreateDateColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.ExpDateColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.CreatedByColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.VerifiedByColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.packetDescriptionRichTextBox = new System.Windows.Forms.RichTextBox();
            this.PacketListToolStrip = new System.Windows.Forms.ToolStrip();
            this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
            this.toolStripComboBox1 = new System.Windows.Forms.ToolStripComboBox();
            this.ValidationKeysTabPage = new System.Windows.Forms.TabPage();
            this.panel1 = new System.Windows.Forms.Panel();
            this.HardwareKeyListView = new Shared.VisualComponents.NoFlickerListView();
            this.ReportTabPage = new System.Windows.Forms.TabPage();
            this.ReportListView = new Shared.VisualComponents.NoFlickerListView();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
            this.MainSplitContainer = new System.Windows.Forms.SplitContainer();
            this.MainTreeView = new System.Windows.Forms.TreeView();
            this.MainTreeViewImageList = new System.Windows.Forms.ImageList(this.components);
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.PacketContextMenuStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.verifyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.updateToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.DetailListViewContextMenuStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.dlvNewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.dlvEditToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.dlvDeleteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.MainTreeViewContextMenuStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.editReportToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.deleteToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.LicenseContextMenuStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.expandToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.collapseToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.lcmToolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.resyncToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.deleteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.lcmToolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.addToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newProductLicenseToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.existingProductLicenseToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addLicenseToolStripSeparator = new System.Windows.Forms.ToolStripSeparator();
            this.newLicenseToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newFailoverToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newDisRecToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newTestDevToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripMenuItem();
            this.verifiedToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.unVerifiedToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.lcmToolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.createPacketFileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.customerContextMenuStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.renameCustomerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.deleteCustomerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator6 = new System.Windows.Forms.ToolStripSeparator();
            this.newCustomerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.customerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ValidationKeyToolStrip = new System.Windows.Forms.ToolStrip();
            this.ValidationKeyCustomerLabel = new System.Windows.Forms.ToolStripLabel();
            this.CustomerToolStrip = new System.Windows.Forms.ToolStrip();
            this.CustomerToolStripLabel = new System.Windows.Forms.ToolStripLabel();
            this.updateVersionsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.MainMenuStrip.SuspendLayout();
            this.MainToolStrip.SuspendLayout();
            this.LicenseViewPanel.SuspendLayout();
            this.splitContainer2.Panel1.SuspendLayout();
            this.splitContainer2.Panel2.SuspendLayout();
            this.splitContainer2.SuspendLayout();
            this.MainTabControl.SuspendLayout();
            this.LicensesTabPage.SuspendLayout();
            this.LicenseViewSplitContainer.Panel1.SuspendLayout();
            this.LicenseViewSplitContainer.Panel2.SuspendLayout();
            this.LicenseViewSplitContainer.SuspendLayout();
            this.PropertyGridTabControl.SuspendLayout();
            this.PropertyGridTabPage.SuspendLayout();
            this.DetailSplitContainer.Panel1.SuspendLayout();
            this.DetailSplitContainer.Panel2.SuspendLayout();
            this.DetailSplitContainer.SuspendLayout();
            this.DetailListViewToolStrip.SuspendLayout();
            this.TransactionsTabPage.SuspendLayout();
            this.TransactionsToolStrip.SuspendLayout();
            this.PacketTabPage.SuspendLayout();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.PacketListToolStrip.SuspendLayout();
            this.ValidationKeysTabPage.SuspendLayout();
            this.panel1.SuspendLayout();
            this.ReportTabPage.SuspendLayout();
            this.MainSplitContainer.Panel1.SuspendLayout();
            this.MainSplitContainer.SuspendLayout();
            this.PacketContextMenuStrip.SuspendLayout();
            this.DetailListViewContextMenuStrip.SuspendLayout();
            this.MainTreeViewContextMenuStrip.SuspendLayout();
            this.LicenseContextMenuStrip.SuspendLayout();
            this.customerContextMenuStrip.SuspendLayout();
            this.ValidationKeyToolStrip.SuspendLayout();
            this.CustomerToolStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // MainMenuStrip
            // 
            this.MainMenuStrip.BackColor = System.Drawing.SystemColors.Control;
            this.MainMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.toolStripMenuItem1,
            this.viewToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.MainMenuStrip.Location = new System.Drawing.Point(0, 0);
            this.MainMenuStrip.Name = "MainMenuStrip";
            this.MainMenuStrip.Size = new System.Drawing.Size(989, 24);
            this.MainMenuStrip.TabIndex = 0;
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.connectToolStripMenuItem,
            this.toolStripSeparator1,
            this.newReportToolStripMenuItem,
            this.toolStripSeparator3,
            this.exitMainToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(35, 20);
            this.fileToolStripMenuItem.Text = "&File";
            // 
            // connectToolStripMenuItem
            // 
            this.connectToolStripMenuItem.Name = "connectToolStripMenuItem";
            this.connectToolStripMenuItem.Size = new System.Drawing.Size(142, 22);
            this.connectToolStripMenuItem.Text = "Connect";
            this.connectToolStripMenuItem.Click += new System.EventHandler(this.connectToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(139, 6);
            // 
            // newReportToolStripMenuItem
            // 
            this.newReportToolStripMenuItem.Name = "newReportToolStripMenuItem";
            this.newReportToolStripMenuItem.Size = new System.Drawing.Size(142, 22);
            this.newReportToolStripMenuItem.Text = "New Report";
            this.newReportToolStripMenuItem.Click += new System.EventHandler(this.newReportToolStripMenuItem_Click);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(139, 6);
            // 
            // exitMainToolStripMenuItem
            // 
            this.exitMainToolStripMenuItem.Name = "exitMainToolStripMenuItem";
            this.exitMainToolStripMenuItem.Size = new System.Drawing.Size(142, 22);
            this.exitMainToolStripMenuItem.Text = "E&xit";
            this.exitMainToolStripMenuItem.Click += new System.EventHandler(this.exitMainToolStripMenuItem_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.updateVersionsToolStripMenuItem,
            this.optionsToolStripMenuItem});
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(44, 20);
            this.toolStripMenuItem1.Text = "Tools";
            // 
            // optionsToolStripMenuItem
            // 
            this.optionsToolStripMenuItem.Name = "optionsToolStripMenuItem";
            this.optionsToolStripMenuItem.Size = new System.Drawing.Size(210, 22);
            this.optionsToolStripMenuItem.Text = "Options...";
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(40, 20);
            this.helpToolStripMenuItem.Text = "Help";
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.aboutToolStripMenuItem.Text = "About";
            this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
            // 
            // viewToolStripMenuItem
            // 
            this.viewToolStripMenuItem.Name = "viewToolStripMenuItem";
            this.viewToolStripMenuItem.Size = new System.Drawing.Size(41, 20);
            this.viewToolStripMenuItem.Text = "View";
            // 
            // MainToolStrip
            // 
            this.MainToolStrip.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.MainToolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.navigateBackToolStripButton,
            this.navigateForwardToolStripButton,
            this.toolStripSeparator5,
            this.newCustMainToolStripBtn,
            this.addLicMainToolStripBtn,
            this.addProductLicenseMainToolStripDropDownBtn,
            this.toolStripSeparator7,
            this.SearchToolStripLabel,
            this.searchToolStripTextBox,
            this.createPacketMainToolStripBtn,
            this.toolStripSeparator4,
            this.toggleViewMainToolStripBtn,
            this.toolStripSeparator8,
            this.seekKeyToolStripButton,
            this.reserveHardwareKeyToolStripButton,
            this.deactivateHardwareKeyToolStripButton,
            this.activateHardwareKeyToolStripButton});
            this.MainToolStrip.Location = new System.Drawing.Point(0, 24);
            this.MainToolStrip.Name = "MainToolStrip";
            this.MainToolStrip.Padding = new System.Windows.Forms.Padding(0);
            this.MainToolStrip.Size = new System.Drawing.Size(989, 27);
            this.MainToolStrip.TabIndex = 1;
            // 
            // navigateBackToolStripButton
            // 
            this.navigateBackToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.navigateBackToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("navigateBackToolStripButton.Image")));
            this.navigateBackToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.navigateBackToolStripButton.Name = "navigateBackToolStripButton";
            this.navigateBackToolStripButton.Size = new System.Drawing.Size(23, 24);
            this.navigateBackToolStripButton.Text = "Back";
            this.navigateBackToolStripButton.Click += new System.EventHandler(this.navigateBackToolStripButton_Click);
            // 
            // navigateForwardToolStripButton
            // 
            this.navigateForwardToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.navigateForwardToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("navigateForwardToolStripButton.Image")));
            this.navigateForwardToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.navigateForwardToolStripButton.Name = "navigateForwardToolStripButton";
            this.navigateForwardToolStripButton.Size = new System.Drawing.Size(23, 24);
            this.navigateForwardToolStripButton.Text = "Forward";
            this.navigateForwardToolStripButton.Click += new System.EventHandler(this.navigateForwardToolStripButton_Click);
            // 
            // toolStripSeparator5
            // 
            this.toolStripSeparator5.Name = "toolStripSeparator5";
            this.toolStripSeparator5.Size = new System.Drawing.Size(6, 27);
            // 
            // newCustMainToolStripBtn
            // 
            this.newCustMainToolStripBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.newCustMainToolStripBtn.Image = ((System.Drawing.Image)(resources.GetObject("newCustMainToolStripBtn.Image")));
            this.newCustMainToolStripBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.newCustMainToolStripBtn.Name = "newCustMainToolStripBtn";
            this.newCustMainToolStripBtn.Size = new System.Drawing.Size(23, 24);
            this.newCustMainToolStripBtn.Text = "Add New Customer";
            this.newCustMainToolStripBtn.Click += new System.EventHandler(this.newCustMainToolStripBtn_Click);
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
            this.addLicMainToolStripBtn.Size = new System.Drawing.Size(32, 24);
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
            // addProductLicenseMainToolStripDropDownBtn
            // 
            this.addProductLicenseMainToolStripDropDownBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.addProductLicenseMainToolStripDropDownBtn.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newProductLicenseMainToolStripMenuItem,
            this.existingProductLicenseMainToolStripMenuItem});
            this.addProductLicenseMainToolStripDropDownBtn.Image = ((System.Drawing.Image)(resources.GetObject("addProductLicenseMainToolStripDropDownBtn.Image")));
            this.addProductLicenseMainToolStripDropDownBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.addProductLicenseMainToolStripDropDownBtn.Name = "addProductLicenseMainToolStripDropDownBtn";
            this.addProductLicenseMainToolStripDropDownBtn.Size = new System.Drawing.Size(32, 24);
            this.addProductLicenseMainToolStripDropDownBtn.Text = "Add New Order";
            this.addProductLicenseMainToolStripDropDownBtn.ToolTipText = "Add new order to selected license";
            this.addProductLicenseMainToolStripDropDownBtn.ButtonClick += new System.EventHandler(this.addProductLicenseMainToolStripDropDownBtn_ButtonClick);
            this.addProductLicenseMainToolStripDropDownBtn.DropDownOpening += new System.EventHandler(this.addProductLicenseMainToolStripDropDownBtn_DropDownOpening);
            // 
            // newProductLicenseMainToolStripMenuItem
            // 
            this.newProductLicenseMainToolStripMenuItem.Name = "newProductLicenseMainToolStripMenuItem";
            this.newProductLicenseMainToolStripMenuItem.Size = new System.Drawing.Size(212, 22);
            this.newProductLicenseMainToolStripMenuItem.Text = "New Product License...";
            this.newProductLicenseMainToolStripMenuItem.Click += new System.EventHandler(this.newProductLicenseMainToolStripDropDownBtn_Click);
            // 
            // existingProductLicenseMainToolStripMenuItem
            // 
            this.existingProductLicenseMainToolStripMenuItem.Name = "existingProductLicenseMainToolStripMenuItem";
            this.existingProductLicenseMainToolStripMenuItem.Size = new System.Drawing.Size(212, 22);
            this.existingProductLicenseMainToolStripMenuItem.Text = "Existing Product License...";
            this.existingProductLicenseMainToolStripMenuItem.Click += new System.EventHandler(this.existingProductLicenseMainToolStripMenuItem_Click);
            // 
            // toolStripSeparator7
            // 
            this.toolStripSeparator7.Name = "toolStripSeparator7";
            this.toolStripSeparator7.Size = new System.Drawing.Size(6, 27);
            // 
            // SearchToolStripLabel
            // 
            this.SearchToolStripLabel.ActiveLinkColor = System.Drawing.Color.Red;
            this.SearchToolStripLabel.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
            this.SearchToolStripLabel.AutoToolTip = true;
            this.SearchToolStripLabel.BackColor = System.Drawing.SystemColors.Window;
            this.SearchToolStripLabel.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("SearchToolStripLabel.BackgroundImage")));
            this.SearchToolStripLabel.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.SearchToolStripLabel.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.SearchToolStripLabel.Image = ((System.Drawing.Image)(resources.GetObject("SearchToolStripLabel.Image")));
            this.SearchToolStripLabel.Margin = new System.Windows.Forms.Padding(0, 3, 5, 3);
            this.SearchToolStripLabel.Name = "SearchToolStripLabel";
            this.SearchToolStripLabel.Size = new System.Drawing.Size(16, 21);
            this.SearchToolStripLabel.Text = "toolStripLabel2";
            this.SearchToolStripLabel.ToolTipText = "Search";
            this.SearchToolStripLabel.Click += new System.EventHandler(this.SearchToolStripLabel_Click);
            // 
            // searchToolStripTextBox
            // 
            this.searchToolStripTextBox.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
            this.searchToolStripTextBox.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.searchToolStripTextBox.ForeColor = System.Drawing.SystemColors.InactiveCaptionText;
            this.searchToolStripTextBox.Margin = new System.Windows.Forms.Padding(0, 3, 0, 3);
            this.searchToolStripTextBox.Name = "searchToolStripTextBox";
            this.searchToolStripTextBox.Size = new System.Drawing.Size(120, 21);
            this.searchToolStripTextBox.Text = "Search";
            this.searchToolStripTextBox.Leave += new System.EventHandler(this.searchToolStripTextBox_Leave);
            this.searchToolStripTextBox.Enter += new System.EventHandler(this.searchToolStripTextBox_Enter);
            this.searchToolStripTextBox.KeyUp += new System.Windows.Forms.KeyEventHandler(this.searchToolStripTextBox_KeyUp);
            // 
            // createPacketMainToolStripBtn
            // 
            this.createPacketMainToolStripBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.createPacketMainToolStripBtn.Enabled = false;
            this.createPacketMainToolStripBtn.Image = ((System.Drawing.Image)(resources.GetObject("createPacketMainToolStripBtn.Image")));
            this.createPacketMainToolStripBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.createPacketMainToolStripBtn.Name = "createPacketMainToolStripBtn";
            this.createPacketMainToolStripBtn.Size = new System.Drawing.Size(23, 24);
            this.createPacketMainToolStripBtn.Text = "Generate License";
            this.createPacketMainToolStripBtn.ToolTipText = "Generate License Packet from selected license";
            this.createPacketMainToolStripBtn.Click += new System.EventHandler(this.createPacketMainToolStripBtn_Click);
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(6, 27);
            // 
            // toggleViewMainToolStripBtn
            // 
            this.toggleViewMainToolStripBtn.CheckOnClick = true;
            this.toggleViewMainToolStripBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toggleViewMainToolStripBtn.Image = ((System.Drawing.Image)(resources.GetObject("toggleViewMainToolStripBtn.Image")));
            this.toggleViewMainToolStripBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toggleViewMainToolStripBtn.Name = "toggleViewMainToolStripBtn";
            this.toggleViewMainToolStripBtn.Size = new System.Drawing.Size(23, 24);
            this.toggleViewMainToolStripBtn.Text = "ViewMenuTree";
            this.toggleViewMainToolStripBtn.ToolTipText = "Toggle View";
            this.toggleViewMainToolStripBtn.Visible = false;
            this.toggleViewMainToolStripBtn.CheckedChanged += new System.EventHandler(this.toggleViewMainToolStripBtn_CheckedChanged);
            // 
            // toolStripSeparator8
            // 
            this.toolStripSeparator8.Name = "toolStripSeparator8";
            this.toolStripSeparator8.Size = new System.Drawing.Size(6, 27);
            this.toolStripSeparator8.Visible = false;
            // 
            // seekKeyToolStripButton
            // 
            this.seekKeyToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.seekKeyToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("seekKeyToolStripButton.Image")));
            this.seekKeyToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.seekKeyToolStripButton.Name = "seekKeyToolStripButton";
            this.seekKeyToolStripButton.Size = new System.Drawing.Size(23, 24);
            this.seekKeyToolStripButton.Text = "Seek Hardware Key";
            this.seekKeyToolStripButton.Click += new System.EventHandler(this.seekKeyToolStripButton_Click);
            // 
            // reserveHardwareKeyToolStripButton
            // 
            this.reserveHardwareKeyToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.reserveHardwareKeyToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("reserveHardwareKeyToolStripButton.Image")));
            this.reserveHardwareKeyToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.reserveHardwareKeyToolStripButton.Name = "reserveHardwareKeyToolStripButton";
            this.reserveHardwareKeyToolStripButton.Size = new System.Drawing.Size(23, 24);
            this.reserveHardwareKeyToolStripButton.Text = "Reserve Hardware Key";
            this.reserveHardwareKeyToolStripButton.Click += new System.EventHandler(this.reserveHardwareKeyToolStripButton_Click);
            // 
            // deactivateHardwareKeyToolStripButton
            // 
            this.deactivateHardwareKeyToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.deactivateHardwareKeyToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("deactivateHardwareKeyToolStripButton.Image")));
            this.deactivateHardwareKeyToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.deactivateHardwareKeyToolStripButton.Name = "deactivateHardwareKeyToolStripButton";
            this.deactivateHardwareKeyToolStripButton.Size = new System.Drawing.Size(23, 24);
            this.deactivateHardwareKeyToolStripButton.Text = "Deactivate Hardware Key";
            this.deactivateHardwareKeyToolStripButton.Click += new System.EventHandler(this.deactivateHardwareKeyToolStripButton_Click);
            // 
            // activateHardwareKeyToolStripButton
            // 
            this.activateHardwareKeyToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.activateHardwareKeyToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("activateHardwareKeyToolStripButton.Image")));
            this.activateHardwareKeyToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.activateHardwareKeyToolStripButton.Name = "activateHardwareKeyToolStripButton";
            this.activateHardwareKeyToolStripButton.Size = new System.Drawing.Size(23, 24);
            this.activateHardwareKeyToolStripButton.Text = "Activate Hardware Key";
            this.activateHardwareKeyToolStripButton.Click += new System.EventHandler(this.activateHardwareKeyToolStripButton_Click);
            // 
            // LicenseViewPanel
            // 
            this.LicenseViewPanel.BackColor = System.Drawing.SystemColors.Window;
            this.LicenseViewPanel.Controls.Add(this.splitContainer2);
            this.LicenseViewPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.LicenseViewPanel.Location = new System.Drawing.Point(0, 51);
            this.LicenseViewPanel.Name = "LicenseViewPanel";
            this.LicenseViewPanel.Size = new System.Drawing.Size(989, 474);
            this.LicenseViewPanel.TabIndex = 8;
            // 
            // splitContainer2
            // 
            this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer2.Location = new System.Drawing.Point(0, 0);
            this.splitContainer2.Name = "splitContainer2";
            this.splitContainer2.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer2.Panel1
            // 
            this.splitContainer2.Panel1.BackColor = System.Drawing.SystemColors.Window;
            this.splitContainer2.Panel1.Controls.Add(this.MainTabControl);
            this.splitContainer2.Panel1.Controls.Add(this.MainSplitContainer);
            // 
            // splitContainer2.Panel2
            // 
            this.splitContainer2.Panel2.Controls.Add(this.statusStrip1);
            this.splitContainer2.Panel2MinSize = 10;
            this.splitContainer2.Size = new System.Drawing.Size(989, 474);
            this.splitContainer2.SplitterDistance = 452;
            this.splitContainer2.SplitterWidth = 2;
            this.splitContainer2.TabIndex = 2;
            // 
            // MainTabControl
            // 
            this.MainTabControl.Controls.Add(this.LicensesTabPage);
            this.MainTabControl.Controls.Add(this.ValidationKeysTabPage);
            this.MainTabControl.Controls.Add(this.ReportTabPage);
            this.MainTabControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.MainTabControl.Location = new System.Drawing.Point(0, 0);
            this.MainTabControl.Margin = new System.Windows.Forms.Padding(0);
            this.MainTabControl.Multiline = true;
            this.MainTabControl.Name = "MainTabControl";
            this.MainTabControl.Padding = new System.Drawing.Point(9, 3);
            this.MainTabControl.SelectedIndex = 0;
            this.MainTabControl.Size = new System.Drawing.Size(989, 452);
            this.MainTabControl.SizeMode = System.Windows.Forms.TabSizeMode.FillToRight;
            this.MainTabControl.TabIndex = 2;
            this.MainTabControl.SelectedIndexChanged += new System.EventHandler(this.MainTabControl_SelectedIndexChanged);
            // 
            // LicensesTabPage
            // 
            this.LicensesTabPage.Controls.Add(this.CustomersListView);
            this.LicensesTabPage.Controls.Add(this.LicenseViewSplitContainer);
            this.LicensesTabPage.Controls.Add(this.CustomerToolStrip);
            this.LicensesTabPage.Location = new System.Drawing.Point(4, 22);
            this.LicensesTabPage.Margin = new System.Windows.Forms.Padding(5);
            this.LicensesTabPage.Name = "LicensesTabPage";
            this.LicensesTabPage.Size = new System.Drawing.Size(981, 426);
            this.LicensesTabPage.TabIndex = 1;
            this.LicensesTabPage.Text = "Licenses";
            this.LicensesTabPage.UseVisualStyleBackColor = true;
            // 
            // CustomersListView
            // 
            this.CustomersListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.custNumColumnHeader,
            this.custNameColumnHeader});
            this.CustomersListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.CustomersListView.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.CustomersListView.FullRowSelect = true;
            this.CustomersListView.GridLines = true;
            this.CustomersListView.Location = new System.Drawing.Point(0, 25);
            this.CustomersListView.MultiSelect = false;
            this.CustomersListView.Name = "CustomersListView";
            this.CustomersListView.NoItemsMessage = "";
            this.CustomersListView.Size = new System.Drawing.Size(981, 401);
            this.CustomersListView.TabIndex = 2;
            this.CustomersListView.UseCompatibleStateImageBehavior = false;
            this.CustomersListView.View = System.Windows.Forms.View.Details;
            this.CustomersListView.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.CustomersListView_MouseDoubleClick);
            this.CustomersListView.MouseUp += new System.Windows.Forms.MouseEventHandler(this.CustomersListView_MouseUp);
            this.CustomersListView.ColumnClick += new System.Windows.Forms.ColumnClickEventHandler(this.CustomersListView_ColumnClick);
            // 
            // custNumColumnHeader
            // 
            this.custNumColumnHeader.Text = "Customer";
            this.custNumColumnHeader.Width = 76;
            // 
            // custNameColumnHeader
            // 
            this.custNameColumnHeader.Text = "Name";
            this.custNameColumnHeader.Width = 129;
            // 
            // LicenseViewSplitContainer
            // 
            this.LicenseViewSplitContainer.BackColor = System.Drawing.SystemColors.Control;
            this.LicenseViewSplitContainer.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.LicenseViewSplitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.LicenseViewSplitContainer.Location = new System.Drawing.Point(0, 25);
            this.LicenseViewSplitContainer.Margin = new System.Windows.Forms.Padding(0);
            this.LicenseViewSplitContainer.Name = "LicenseViewSplitContainer";
            // 
            // LicenseViewSplitContainer.Panel1
            // 
            this.LicenseViewSplitContainer.Panel1.Controls.Add(this.DetailTreeView);
            // 
            // LicenseViewSplitContainer.Panel2
            // 
            this.LicenseViewSplitContainer.Panel2.Controls.Add(this.PropertyGridTabControl);
            this.LicenseViewSplitContainer.Size = new System.Drawing.Size(981, 401);
            this.LicenseViewSplitContainer.SplitterDistance = 292;
            this.LicenseViewSplitContainer.SplitterWidth = 1;
            this.LicenseViewSplitContainer.TabIndex = 1;
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
            this.DetailTreeView.ShowNodeToolTips = true;
            this.DetailTreeView.Size = new System.Drawing.Size(288, 397);
            this.DetailTreeView.StateImageList = this.DetailTreeViewImageList;
            this.DetailTreeView.TabIndex = 2;
            this.DetailTreeView.BeforeExpand += new System.Windows.Forms.TreeViewCancelEventHandler(this.DetailTreeView_BeforeExpand);
            this.DetailTreeView.MouseUp += new System.Windows.Forms.MouseEventHandler(this.DetailTreeView_MouseUp);
            this.DetailTreeView.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.DetailTreeView_AfterSelect);
            this.DetailTreeView.KeyDown += new System.Windows.Forms.KeyEventHandler(this.DetailTreeView_KeyDown);
            // 
            // DetailTreeViewImageList
            // 
            this.DetailTreeViewImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("DetailTreeViewImageList.ImageStream")));
            this.DetailTreeViewImageList.TransparentColor = System.Drawing.Color.Transparent;
            this.DetailTreeViewImageList.Images.SetKeyName(0, "iCONVERT.ICO");
            this.DetailTreeViewImageList.Images.SetKeyName(1, "rubika32.ico");
            this.DetailTreeViewImageList.Images.SetKeyName(2, "RubikaProcessBuilder32.ico");
            this.DetailTreeViewImageList.Images.SetKeyName(3, "sdxDesigner32.ico");
            this.DetailTreeViewImageList.Images.SetKeyName(4, "SOLindexer32.ico");
            this.DetailTreeViewImageList.Images.SetKeyName(5, "SOLsearcherSinglePlatform32.ico");
            this.DetailTreeViewImageList.Images.SetKeyName(6, "SOLenterprise32.ico");
            this.DetailTreeViewImageList.Images.SetKeyName(7, "solfusion32.ico");
            this.DetailTreeViewImageList.Images.SetKeyName(8, "solScript32.ico");
            this.DetailTreeViewImageList.Images.SetKeyName(9, "Queue Manager32.ico");
            this.DetailTreeViewImageList.Images.SetKeyName(10, "System Manager32.ico");
            this.DetailTreeViewImageList.Images.SetKeyName(11, "spde32.ico");
            this.DetailTreeViewImageList.Images.SetKeyName(12, "SPDNT.ICO");
            this.DetailTreeViewImageList.Images.SetKeyName(13, "Customer.ico");
            this.DetailTreeViewImageList.Images.SetKeyName(14, "License.ico");
            this.DetailTreeViewImageList.Images.SetKeyName(15, "LicenseUpdate.ico");
            this.DetailTreeViewImageList.Images.SetKeyName(16, "LicenseDisabled.ico");
            this.DetailTreeViewImageList.Images.SetKeyName(17, "LicenseResync.ico");
            this.DetailTreeViewImageList.Images.SetKeyName(18, "Order.ico");
            this.DetailTreeViewImageList.Images.SetKeyName(19, "AppInstance.ico");
            this.DetailTreeViewImageList.Images.SetKeyName(20, "Empty.ico");
            // 
            // PropertyGridTabControl
            // 
            this.PropertyGridTabControl.Alignment = System.Windows.Forms.TabAlignment.Bottom;
            this.PropertyGridTabControl.Controls.Add(this.PropertyGridTabPage);
            this.PropertyGridTabControl.Controls.Add(this.TransactionsTabPage);
            this.PropertyGridTabControl.Controls.Add(this.PacketTabPage);
            this.PropertyGridTabControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.PropertyGridTabControl.ImageList = this.ListViewImageList;
            this.PropertyGridTabControl.ItemSize = new System.Drawing.Size(22, 20);
            this.PropertyGridTabControl.Location = new System.Drawing.Point(0, 0);
            this.PropertyGridTabControl.Margin = new System.Windows.Forms.Padding(0);
            this.PropertyGridTabControl.Multiline = true;
            this.PropertyGridTabControl.Name = "PropertyGridTabControl";
            this.PropertyGridTabControl.SelectedIndex = 0;
            this.PropertyGridTabControl.Size = new System.Drawing.Size(684, 397);
            this.PropertyGridTabControl.SizeMode = System.Windows.Forms.TabSizeMode.FillToRight;
            this.PropertyGridTabControl.TabIndex = 0;
            this.PropertyGridTabControl.Selecting += new System.Windows.Forms.TabControlCancelEventHandler(this.PropertyGridTabControl_Selecting);
            this.PropertyGridTabControl.TabIndexChanged += new System.EventHandler(this.PropertyGridTabControl_TabIndexChanged);
            // 
            // PropertyGridTabPage
            // 
            this.PropertyGridTabPage.Controls.Add(this.DetailSplitContainer);
            this.PropertyGridTabPage.ImageIndex = 2;
            this.PropertyGridTabPage.Location = new System.Drawing.Point(4, 4);
            this.PropertyGridTabPage.Margin = new System.Windows.Forms.Padding(0);
            this.PropertyGridTabPage.Name = "PropertyGridTabPage";
            this.PropertyGridTabPage.Size = new System.Drawing.Size(676, 369);
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
            this.DetailSplitContainer.Panel2.Controls.Add(this.loadingCircle1);
            this.DetailSplitContainer.Size = new System.Drawing.Size(676, 369);
            this.DetailSplitContainer.SplitterDistance = 165;
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
            this.DetailPropertyGrid.Size = new System.Drawing.Size(676, 165);
            this.DetailPropertyGrid.TabIndex = 2;
            this.DetailPropertyGrid.ToolbarVisible = false;
            this.DetailPropertyGrid.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(this.DetailPropertyGrid_PropertyValueChanged);
            // 
            // DetailListView
            // 
            this.DetailListView.Activation = System.Windows.Forms.ItemActivation.OneClick;
            this.DetailListView.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.DetailListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.DetailListView.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.DetailListView.FullRowSelect = true;
            this.DetailListView.Location = new System.Drawing.Point(0, 25);
            this.DetailListView.MultiSelect = false;
            this.DetailListView.Name = "DetailListView";
            this.DetailListView.ShowItemToolTips = true;
            this.DetailListView.Size = new System.Drawing.Size(676, 177);
            this.DetailListView.SmallImageList = this.ListViewImageList;
            this.DetailListView.TabIndex = 2;
            this.DetailListView.UseCompatibleStateImageBehavior = false;
            this.DetailListView.View = System.Windows.Forms.View.Details;
            this.DetailListView.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.DetailListView_MouseDoubleClick);
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
            this.DetailListViewToolStripLabel,
            this.dlvRemoveToolStripButton,
            this.dlvAddToolStripButton,
            this.dlvEditToolStripButton,
            this.moduleFilterToolStripComboBox});
            this.DetailListViewToolStrip.Location = new System.Drawing.Point(0, 0);
            this.DetailListViewToolStrip.Name = "DetailListViewToolStrip";
            this.DetailListViewToolStrip.Size = new System.Drawing.Size(676, 25);
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
            // dlvRemoveToolStripButton
            // 
            this.dlvRemoveToolStripButton.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
            this.dlvRemoveToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.dlvRemoveToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("dlvRemoveToolStripButton.Image")));
            this.dlvRemoveToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.dlvRemoveToolStripButton.Name = "dlvRemoveToolStripButton";
            this.dlvRemoveToolStripButton.Size = new System.Drawing.Size(23, 22);
            this.dlvRemoveToolStripButton.Click += new System.EventHandler(this.dlvRemoveToolStripButton_Click);
            // 
            // dlvAddToolStripButton
            // 
            this.dlvAddToolStripButton.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
            this.dlvAddToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.dlvAddToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("dlvAddToolStripButton.Image")));
            this.dlvAddToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.dlvAddToolStripButton.Name = "dlvAddToolStripButton";
            this.dlvAddToolStripButton.Size = new System.Drawing.Size(23, 22);
            this.dlvAddToolStripButton.Click += new System.EventHandler(this.dlvAddToolStripButton_Click);
            // 
            // dlvEditToolStripButton
            // 
            this.dlvEditToolStripButton.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
            this.dlvEditToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.dlvEditToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("dlvEditToolStripButton.Image")));
            this.dlvEditToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.dlvEditToolStripButton.Name = "dlvEditToolStripButton";
            this.dlvEditToolStripButton.Size = new System.Drawing.Size(23, 22);
            this.dlvEditToolStripButton.Click += new System.EventHandler(this.dlvEditToolStripButton_Click);
            // 
            // moduleFilterToolStripComboBox
            // 
            this.moduleFilterToolStripComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.moduleFilterToolStripComboBox.Items.AddRange(new object[] {
            "All",
            "Licensed",
            "UnLicensed",
            "NonDefault"});
            this.moduleFilterToolStripComboBox.Name = "moduleFilterToolStripComboBox";
            this.moduleFilterToolStripComboBox.Size = new System.Drawing.Size(121, 25);
            this.moduleFilterToolStripComboBox.SelectedIndexChanged += new System.EventHandler(this.moduleFilterToolStripComboBox_SelectedIndexChanged);
            // 
            // loadingCircle1
            // 
            this.loadingCircle1.Active = false;
            this.loadingCircle1.BackColor = System.Drawing.Color.Transparent;
            this.loadingCircle1.Color = System.Drawing.Color.DarkGray;
            this.loadingCircle1.DisplayText = "Please Wait...";
            this.loadingCircle1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.loadingCircle1.InnerCircleRadius = 6;
            this.loadingCircle1.Location = new System.Drawing.Point(0, 0);
            this.loadingCircle1.Name = "loadingCircle1";
            this.loadingCircle1.NumberSpoke = 9;
            this.loadingCircle1.OuterCircleRadius = 7;
            this.loadingCircle1.RotationSpeed = 100;
            this.loadingCircle1.Size = new System.Drawing.Size(676, 202);
            this.loadingCircle1.SpokeThickness = 4;
            this.loadingCircle1.StylePreset = Shared.VisualComponents.LoadingCircle.StylePresets.Firefox;
            this.loadingCircle1.TabIndex = 6;
            this.loadingCircle1.Visible = false;
            // 
            // TransactionsTabPage
            // 
            this.TransactionsTabPage.Controls.Add(this.TransactionListView);
            this.TransactionsTabPage.Controls.Add(this.TransactionsToolStrip);
            this.TransactionsTabPage.ImageIndex = 3;
            this.TransactionsTabPage.Location = new System.Drawing.Point(4, 4);
            this.TransactionsTabPage.Margin = new System.Windows.Forms.Padding(0);
            this.TransactionsTabPage.Name = "TransactionsTabPage";
            this.TransactionsTabPage.Size = new System.Drawing.Size(676, 394);
            this.TransactionsTabPage.TabIndex = 1;
            this.TransactionsTabPage.Text = "Transactions";
            this.TransactionsTabPage.UseVisualStyleBackColor = true;
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
            this.TransactionListView.Location = new System.Drawing.Point(0, 25);
            this.TransactionListView.Margin = new System.Windows.Forms.Padding(0);
            this.TransactionListView.Name = "TransactionListView";
            this.TransactionListView.NoItemsMessage = "";
            this.TransactionListView.Size = new System.Drawing.Size(676, 369);
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
            this.TransactionsToolStrip.Size = new System.Drawing.Size(676, 25);
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
            this.PacketTabPage.Size = new System.Drawing.Size(676, 394);
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
            this.splitContainer1.Panel2.Controls.Add(this.packetDescriptionRichTextBox);
            this.splitContainer1.Size = new System.Drawing.Size(676, 369);
            this.splitContainer1.SplitterDistance = 328;
            this.splitContainer1.SplitterWidth = 1;
            this.splitContainer1.TabIndex = 6;
            // 
            // LicensePacketListView
            // 
            this.LicensePacketListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.PacketNameColumnHeader,
            this.CreateDateColumnHeader,
            this.ExpDateColumnHeader,
            this.CreatedByColumnHeader,
            this.VerifiedByColumnHeader});
            this.LicensePacketListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.LicensePacketListView.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LicensePacketListView.FullRowSelect = true;
            this.LicensePacketListView.GridLines = true;
            this.LicensePacketListView.HideSelection = false;
            this.LicensePacketListView.Location = new System.Drawing.Point(0, 0);
            this.LicensePacketListView.Margin = new System.Windows.Forms.Padding(0);
            this.LicensePacketListView.MultiSelect = false;
            this.LicensePacketListView.Name = "LicensePacketListView";
            this.LicensePacketListView.NoItemsMessage = "";
            this.LicensePacketListView.Size = new System.Drawing.Size(674, 326);
            this.LicensePacketListView.StateImageList = this.ListViewImageList;
            this.LicensePacketListView.TabIndex = 4;
            this.LicensePacketListView.UseCompatibleStateImageBehavior = false;
            this.LicensePacketListView.View = System.Windows.Forms.View.Details;
            this.LicensePacketListView.SelectedIndexChanged += new System.EventHandler(this.LicensePacketListView_SelectedIndexChanged);
            this.LicensePacketListView.MouseUp += new System.Windows.Forms.MouseEventHandler(this.LicensePacketListView_MouseUp);
            this.LicensePacketListView.ColumnClick += new System.Windows.Forms.ColumnClickEventHandler(this.LicensePacketListView_ColumnClick);
            // 
            // PacketNameColumnHeader
            // 
            this.PacketNameColumnHeader.Text = "Name";
            this.PacketNameColumnHeader.Width = 56;
            // 
            // CreateDateColumnHeader
            // 
            this.CreateDateColumnHeader.Text = "Date Created";
            this.CreateDateColumnHeader.Width = 84;
            // 
            // ExpDateColumnHeader
            // 
            this.ExpDateColumnHeader.Text = "Expired Date";
            this.ExpDateColumnHeader.Width = 85;
            // 
            // CreatedByColumnHeader
            // 
            this.CreatedByColumnHeader.Text = "Created By";
            this.CreatedByColumnHeader.Width = 77;
            // 
            // VerifiedByColumnHeader
            // 
            this.VerifiedByColumnHeader.Text = "Verified By";
            this.VerifiedByColumnHeader.Width = 86;
            // 
            // packetDescriptionRichTextBox
            // 
            this.packetDescriptionRichTextBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.packetDescriptionRichTextBox.Location = new System.Drawing.Point(0, 0);
            this.packetDescriptionRichTextBox.Name = "packetDescriptionRichTextBox";
            this.packetDescriptionRichTextBox.Size = new System.Drawing.Size(674, 38);
            this.packetDescriptionRichTextBox.TabIndex = 0;
            this.packetDescriptionRichTextBox.Text = "";
            this.packetDescriptionRichTextBox.MouseUp += new System.Windows.Forms.MouseEventHandler(this.packetDescriptionRichTextBox_MouseUp);
            // 
            // PacketListToolStrip
            // 
            this.PacketListToolStrip.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.PacketListToolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripLabel1,
            this.toolStripComboBox1});
            this.PacketListToolStrip.Location = new System.Drawing.Point(0, 0);
            this.PacketListToolStrip.Name = "PacketListToolStrip";
            this.PacketListToolStrip.Size = new System.Drawing.Size(676, 25);
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
            // ValidationKeysTabPage
            // 
            this.ValidationKeysTabPage.Controls.Add(this.panel1);
            this.ValidationKeysTabPage.Location = new System.Drawing.Point(4, 22);
            this.ValidationKeysTabPage.Margin = new System.Windows.Forms.Padding(5);
            this.ValidationKeysTabPage.Name = "ValidationKeysTabPage";
            this.ValidationKeysTabPage.Size = new System.Drawing.Size(981, 426);
            this.ValidationKeysTabPage.TabIndex = 4;
            this.ValidationKeysTabPage.Text = "Validation Keys";
            this.ValidationKeysTabPage.UseVisualStyleBackColor = true;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.HardwareKeyListView);
            this.panel1.Controls.Add(this.ValidationKeyToolStrip);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(981, 426);
            this.panel1.TabIndex = 0;
            // 
            // HardwareKeyListView
            // 
            this.HardwareKeyListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.HardwareKeyListView.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.HardwareKeyListView.FullRowSelect = true;
            this.HardwareKeyListView.GridLines = true;
            this.HardwareKeyListView.Location = new System.Drawing.Point(0, 25);
            this.HardwareKeyListView.MultiSelect = false;
            this.HardwareKeyListView.Name = "HardwareKeyListView";
            this.HardwareKeyListView.NoItemsMessage = "There are no hardware keys to show in this view";
            this.HardwareKeyListView.ShowGroups = false;
            this.HardwareKeyListView.Size = new System.Drawing.Size(981, 401);
            this.HardwareKeyListView.TabIndex = 1;
            this.HardwareKeyListView.UseCompatibleStateImageBehavior = false;
            this.HardwareKeyListView.View = System.Windows.Forms.View.Details;
            this.HardwareKeyListView.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.HardwareKeyListView_MouseDoubleClick);
            this.HardwareKeyListView.ColumnClick += new System.Windows.Forms.ColumnClickEventHandler(this.HardwareKeyListView_ColumnClick);
            this.HardwareKeyListView.ItemSelectionChanged += new System.Windows.Forms.ListViewItemSelectionChangedEventHandler(this.HardwareKeyListView_ItemSelectionChanged);
            // 
            // ReportTabPage
            // 
            this.ReportTabPage.Controls.Add(this.ReportListView);
            this.ReportTabPage.Location = new System.Drawing.Point(4, 22);
            this.ReportTabPage.Margin = new System.Windows.Forms.Padding(5);
            this.ReportTabPage.Name = "ReportTabPage";
            this.ReportTabPage.Size = new System.Drawing.Size(981, 426);
            this.ReportTabPage.TabIndex = 3;
            this.ReportTabPage.Text = "Reports";
            this.ReportTabPage.UseVisualStyleBackColor = true;
            // 
            // ReportListView
            // 
            this.ReportListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
            this.ReportListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ReportListView.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ReportListView.Location = new System.Drawing.Point(0, 0);
            this.ReportListView.Name = "ReportListView";
            this.ReportListView.Size = new System.Drawing.Size(981, 426);
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
            // MainSplitContainer
            // 
            this.MainSplitContainer.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.MainSplitContainer.Location = new System.Drawing.Point(25, 264);
            this.MainSplitContainer.Margin = new System.Windows.Forms.Padding(0);
            this.MainSplitContainer.Name = "MainSplitContainer";
            // 
            // MainSplitContainer.Panel1
            // 
            this.MainSplitContainer.Panel1.BackColor = System.Drawing.SystemColors.Window;
            this.MainSplitContainer.Panel1.Controls.Add(this.MainTreeView);
            // 
            // MainSplitContainer.Panel2
            // 
            this.MainSplitContainer.Panel2.BackColor = System.Drawing.SystemColors.Window;
            this.MainSplitContainer.Size = new System.Drawing.Size(989, 111);
            this.MainSplitContainer.SplitterDistance = 144;
            this.MainSplitContainer.SplitterWidth = 1;
            this.MainSplitContainer.TabIndex = 1;
            this.MainSplitContainer.Visible = false;
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
            this.MainTreeView.Size = new System.Drawing.Size(140, 107);
            this.MainTreeView.TabIndex = 0;
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
            this.MainTreeViewImageList.Images.SetKeyName(1, "JobGroups.ico");
            this.MainTreeViewImageList.Images.SetKeyName(2, "History.ico");
            this.MainTreeViewImageList.Images.SetKeyName(3, "tree.ico");
            // 
            // statusStrip1
            // 
            this.statusStrip1.Location = new System.Drawing.Point(0, -2);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(989, 22);
            this.statusStrip1.TabIndex = 0;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // PacketContextMenuStrip
            // 
            this.PacketContextMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.verifyToolStripMenuItem,
            this.updateToolStripMenuItem});
            this.PacketContextMenuStrip.Name = "PacketContextMenuStrip";
            this.PacketContextMenuStrip.Size = new System.Drawing.Size(121, 48);
            // 
            // verifyToolStripMenuItem
            // 
            this.verifyToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("verifyToolStripMenuItem.Image")));
            this.verifyToolStripMenuItem.Name = "verifyToolStripMenuItem";
            this.verifyToolStripMenuItem.Size = new System.Drawing.Size(120, 22);
            this.verifyToolStripMenuItem.Text = "Verify";
            this.verifyToolStripMenuItem.Click += new System.EventHandler(this.verifyToolStripMenuItem_Click);
            // 
            // updateToolStripMenuItem
            // 
            this.updateToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("updateToolStripMenuItem.Image")));
            this.updateToolStripMenuItem.Name = "updateToolStripMenuItem";
            this.updateToolStripMenuItem.Size = new System.Drawing.Size(120, 22);
            this.updateToolStripMenuItem.Text = "Update";
            this.updateToolStripMenuItem.Click += new System.EventHandler(this.updateToolStripMenuItem_Click);
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
            this.expandToolStripMenuItem,
            this.collapseToolStripMenuItem,
            this.lcmToolStripSeparator1,
            this.resyncToolStripMenuItem,
            this.deleteToolStripMenuItem,
            this.lcmToolStripSeparator2,
            this.addToolStripMenuItem,
            this.toolStripMenuItem2,
            this.lcmToolStripSeparator3,
            this.createPacketFileToolStripMenuItem});
            this.LicenseContextMenuStrip.Name = "LicenseContextMenuStrip";
            this.LicenseContextMenuStrip.Size = new System.Drawing.Size(188, 176);
            // 
            // expandToolStripMenuItem
            // 
            this.expandToolStripMenuItem.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.expandToolStripMenuItem.Name = "expandToolStripMenuItem";
            this.expandToolStripMenuItem.Size = new System.Drawing.Size(187, 22);
            this.expandToolStripMenuItem.Text = "Expand";
            this.expandToolStripMenuItem.Click += new System.EventHandler(this.expandToolStripMenuItem_Click);
            // 
            // collapseToolStripMenuItem
            // 
            this.collapseToolStripMenuItem.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.collapseToolStripMenuItem.Name = "collapseToolStripMenuItem";
            this.collapseToolStripMenuItem.Size = new System.Drawing.Size(187, 22);
            this.collapseToolStripMenuItem.Text = "Collapse";
            this.collapseToolStripMenuItem.Click += new System.EventHandler(this.collapseToolStripMenuItem_Click);
            // 
            // lcmToolStripSeparator1
            // 
            this.lcmToolStripSeparator1.Name = "lcmToolStripSeparator1";
            this.lcmToolStripSeparator1.Size = new System.Drawing.Size(184, 6);
            // 
            // resyncToolStripMenuItem
            // 
            this.resyncToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("resyncToolStripMenuItem.Image")));
            this.resyncToolStripMenuItem.Name = "resyncToolStripMenuItem";
            this.resyncToolStripMenuItem.Size = new System.Drawing.Size(187, 22);
            this.resyncToolStripMenuItem.Text = "Resync";
            this.resyncToolStripMenuItem.Click += new System.EventHandler(this.resyncToolStripMenuItem_Click);
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
            // addToolStripMenuItem
            // 
            this.addToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newProductLicenseToolStripMenuItem,
            this.existingProductLicenseToolStripMenuItem,
            this.addLicenseToolStripSeparator,
            this.newLicenseToolStripMenuItem,
            this.newFailoverToolStripMenuItem,
            this.newDisRecToolStripMenuItem,
            this.newTestDevToolStripMenuItem});
            this.addToolStripMenuItem.Name = "addToolStripMenuItem";
            this.addToolStripMenuItem.Size = new System.Drawing.Size(187, 22);
            this.addToolStripMenuItem.Text = "Add";
            // 
            // newProductLicenseToolStripMenuItem
            // 
            this.newProductLicenseToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("newProductLicenseToolStripMenuItem.Image")));
            this.newProductLicenseToolStripMenuItem.Name = "newProductLicenseToolStripMenuItem";
            this.newProductLicenseToolStripMenuItem.Size = new System.Drawing.Size(258, 22);
            this.newProductLicenseToolStripMenuItem.Text = "New Product License...";
            this.newProductLicenseToolStripMenuItem.Click += new System.EventHandler(this.newProductLicenseToolStripMenuItem_Click);
            // 
            // existingProductLicenseToolStripMenuItem
            // 
            this.existingProductLicenseToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("existingProductLicenseToolStripMenuItem.Image")));
            this.existingProductLicenseToolStripMenuItem.Name = "existingProductLicenseToolStripMenuItem";
            this.existingProductLicenseToolStripMenuItem.Size = new System.Drawing.Size(258, 22);
            this.existingProductLicenseToolStripMenuItem.Text = "Existing Product License...";
            this.existingProductLicenseToolStripMenuItem.Click += new System.EventHandler(this.existingProductLicenseToolStripMenuItem_Click);
            // 
            // addLicenseToolStripSeparator
            // 
            this.addLicenseToolStripSeparator.Name = "addLicenseToolStripSeparator";
            this.addLicenseToolStripSeparator.Size = new System.Drawing.Size(255, 6);
            // 
            // newLicenseToolStripMenuItem
            // 
            this.newLicenseToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("newLicenseToolStripMenuItem.Image")));
            this.newLicenseToolStripMenuItem.Name = "newLicenseToolStripMenuItem";
            this.newLicenseToolStripMenuItem.Size = new System.Drawing.Size(258, 22);
            this.newLicenseToolStripMenuItem.Text = "New License Server...";
            this.newLicenseToolStripMenuItem.Click += new System.EventHandler(this.newLicenseToolStripMenuItem_Click);
            // 
            // newFailoverToolStripMenuItem
            // 
            this.newFailoverToolStripMenuItem.Name = "newFailoverToolStripMenuItem";
            this.newFailoverToolStripMenuItem.Size = new System.Drawing.Size(258, 22);
            this.newFailoverToolStripMenuItem.Text = "Failover License Server...";
            this.newFailoverToolStripMenuItem.Click += new System.EventHandler(this.newFailoverLicenseToolStripMenuItem_Click);
            // 
            // newDisRecToolStripMenuItem
            // 
            this.newDisRecToolStripMenuItem.Name = "newDisRecToolStripMenuItem";
            this.newDisRecToolStripMenuItem.Size = new System.Drawing.Size(258, 22);
            this.newDisRecToolStripMenuItem.Text = "Disaster Recovery License Server...";
            this.newDisRecToolStripMenuItem.Click += new System.EventHandler(this.newDRLicenseToolStripMenuItem_Click);
            // 
            // newTestDevToolStripMenuItem
            // 
            this.newTestDevToolStripMenuItem.Name = "newTestDevToolStripMenuItem";
            this.newTestDevToolStripMenuItem.Size = new System.Drawing.Size(258, 22);
            this.newTestDevToolStripMenuItem.Text = "Test Development License Server...";
            this.newTestDevToolStripMenuItem.Click += new System.EventHandler(this.newTestDevLicenseToolStripMenuItem_Click);
            // 
            // toolStripMenuItem2
            // 
            this.toolStripMenuItem2.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.verifiedToolStripMenuItem,
            this.unVerifiedToolStripMenuItem});
            this.toolStripMenuItem2.Name = "toolStripMenuItem2";
            this.toolStripMenuItem2.Size = new System.Drawing.Size(187, 22);
            this.toolStripMenuItem2.Text = "Transfer";
            // 
            // verifiedToolStripMenuItem
            // 
            this.verifiedToolStripMenuItem.Name = "verifiedToolStripMenuItem";
            this.verifiedToolStripMenuItem.Size = new System.Drawing.Size(134, 22);
            this.verifiedToolStripMenuItem.Text = "Verified ";
            // 
            // unVerifiedToolStripMenuItem
            // 
            this.unVerifiedToolStripMenuItem.Name = "unVerifiedToolStripMenuItem";
            this.unVerifiedToolStripMenuItem.Size = new System.Drawing.Size(134, 22);
            this.unVerifiedToolStripMenuItem.Text = "UnVerified";
            // 
            // lcmToolStripSeparator3
            // 
            this.lcmToolStripSeparator3.Name = "lcmToolStripSeparator3";
            this.lcmToolStripSeparator3.Size = new System.Drawing.Size(184, 6);
            // 
            // createPacketFileToolStripMenuItem
            // 
            this.createPacketFileToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("createPacketFileToolStripMenuItem.Image")));
            this.createPacketFileToolStripMenuItem.Name = "createPacketFileToolStripMenuItem";
            this.createPacketFileToolStripMenuItem.Size = new System.Drawing.Size(187, 22);
            this.createPacketFileToolStripMenuItem.Text = "Generate License File";
            this.createPacketFileToolStripMenuItem.Click += new System.EventHandler(this.createPacketFileToolStripMenuItem_Click);
            // 
            // customerContextMenuStrip
            // 
            this.customerContextMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.renameCustomerToolStripMenuItem,
            this.deleteCustomerToolStripMenuItem,
            this.toolStripSeparator6,
            this.newCustomerToolStripMenuItem});
            this.customerContextMenuStrip.Name = "customerContextMenuStrip";
            this.customerContextMenuStrip.Size = new System.Drawing.Size(125, 76);
            // 
            // renameCustomerToolStripMenuItem
            // 
            this.renameCustomerToolStripMenuItem.Name = "renameCustomerToolStripMenuItem";
            this.renameCustomerToolStripMenuItem.Size = new System.Drawing.Size(124, 22);
            this.renameCustomerToolStripMenuItem.Text = "Rename";
            this.renameCustomerToolStripMenuItem.Click += new System.EventHandler(this.renameCustomerToolStripMenuItem_Click);
            // 
            // deleteCustomerToolStripMenuItem
            // 
            this.deleteCustomerToolStripMenuItem.Name = "deleteCustomerToolStripMenuItem";
            this.deleteCustomerToolStripMenuItem.Size = new System.Drawing.Size(124, 22);
            this.deleteCustomerToolStripMenuItem.Text = "Delete";
            this.deleteCustomerToolStripMenuItem.Click += new System.EventHandler(this.deleteCustomerToolStripMenuItem_Click);
            // 
            // toolStripSeparator6
            // 
            this.toolStripSeparator6.Name = "toolStripSeparator6";
            this.toolStripSeparator6.Size = new System.Drawing.Size(121, 6);
            // 
            // newCustomerToolStripMenuItem
            // 
            this.newCustomerToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.customerToolStripMenuItem});
            this.newCustomerToolStripMenuItem.Name = "newCustomerToolStripMenuItem";
            this.newCustomerToolStripMenuItem.Size = new System.Drawing.Size(124, 22);
            this.newCustomerToolStripMenuItem.Text = "New";
            // 
            // customerToolStripMenuItem
            // 
            this.customerToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("customerToolStripMenuItem.Image")));
            this.customerToolStripMenuItem.Name = "customerToolStripMenuItem";
            this.customerToolStripMenuItem.Size = new System.Drawing.Size(131, 22);
            this.customerToolStripMenuItem.Text = "Customer";
            this.customerToolStripMenuItem.Click += new System.EventHandler(this.customerToolStripMenuItem_Click);
            // 
            // ValidationKeyToolStrip
            // 
            this.ValidationKeyToolStrip.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ValidationKeyToolStrip.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.ValidationKeyToolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ValidationKeyCustomerLabel});
            this.ValidationKeyToolStrip.Location = new System.Drawing.Point(0, 0);
            this.ValidationKeyToolStrip.Name = "ValidationKeyToolStrip";
            this.ValidationKeyToolStrip.Size = new System.Drawing.Size(981, 25);
            this.ValidationKeyToolStrip.TabIndex = 2;
            this.ValidationKeyToolStrip.Text = "toolStrip1";
            // 
            // ValidationKeyCustomerLabel
            // 
            this.ValidationKeyCustomerLabel.Name = "ValidationKeyCustomerLabel";
            this.ValidationKeyCustomerLabel.Size = new System.Drawing.Size(38, 22);
            this.ValidationKeyCustomerLabel.Text = "[Edit]";
            // 
            // CustomerToolStrip
            // 
            this.CustomerToolStrip.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.CustomerToolStrip.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.CustomerToolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.CustomerToolStripLabel});
            this.CustomerToolStrip.Location = new System.Drawing.Point(0, 0);
            this.CustomerToolStrip.Name = "CustomerToolStrip";
            this.CustomerToolStrip.Size = new System.Drawing.Size(981, 25);
            this.CustomerToolStrip.TabIndex = 3;
            this.CustomerToolStrip.Text = "toolStrip1";
            // 
            // CustomerToolStripLabel
            // 
            this.CustomerToolStripLabel.Name = "CustomerToolStripLabel";
            this.CustomerToolStripLabel.Size = new System.Drawing.Size(38, 22);
            this.CustomerToolStripLabel.Text = "[Edit]";
            // 
            // updateVersionsToolStripMenuItem
            // 
            this.updateVersionsToolStripMenuItem.Name = "updateVersionsToolStripMenuItem";
            this.updateVersionsToolStripMenuItem.Size = new System.Drawing.Size(210, 22);
            this.updateVersionsToolStripMenuItem.Text = "Update Product Version...";
            this.updateVersionsToolStripMenuItem.Click += new System.EventHandler(this.updateVersionsToolStripMenuItem_Click);
            // 
            // CreatorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.ClientSize = new System.Drawing.Size(989, 525);
            this.Controls.Add(this.LicenseViewPanel);
            this.Controls.Add(this.MainToolStrip);
            this.Controls.Add(this.MainMenuStrip);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Name = "CreatorForm";
            this.Text = "Creator";
            this.Load += new System.EventHandler(this.CreatorForm_Load);
            this.Shown += new System.EventHandler(this.connectToolStripMenuItem_Click);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.CreatorForm_FormClosing);
            this.MainMenuStrip.ResumeLayout(false);
            this.MainMenuStrip.PerformLayout();
            this.MainToolStrip.ResumeLayout(false);
            this.MainToolStrip.PerformLayout();
            this.LicenseViewPanel.ResumeLayout(false);
            this.splitContainer2.Panel1.ResumeLayout(false);
            this.splitContainer2.Panel2.ResumeLayout(false);
            this.splitContainer2.Panel2.PerformLayout();
            this.splitContainer2.ResumeLayout(false);
            this.MainTabControl.ResumeLayout(false);
            this.LicensesTabPage.ResumeLayout(false);
            this.LicensesTabPage.PerformLayout();
            this.LicenseViewSplitContainer.Panel1.ResumeLayout(false);
            this.LicenseViewSplitContainer.Panel2.ResumeLayout(false);
            this.LicenseViewSplitContainer.ResumeLayout(false);
            this.PropertyGridTabControl.ResumeLayout(false);
            this.PropertyGridTabPage.ResumeLayout(false);
            this.DetailSplitContainer.Panel1.ResumeLayout(false);
            this.DetailSplitContainer.Panel2.ResumeLayout(false);
            this.DetailSplitContainer.Panel2.PerformLayout();
            this.DetailSplitContainer.ResumeLayout(false);
            this.DetailListViewToolStrip.ResumeLayout(false);
            this.DetailListViewToolStrip.PerformLayout();
            this.TransactionsTabPage.ResumeLayout(false);
            this.TransactionsTabPage.PerformLayout();
            this.TransactionsToolStrip.ResumeLayout(false);
            this.TransactionsToolStrip.PerformLayout();
            this.PacketTabPage.ResumeLayout(false);
            this.PacketTabPage.PerformLayout();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.PacketListToolStrip.ResumeLayout(false);
            this.PacketListToolStrip.PerformLayout();
            this.ValidationKeysTabPage.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ReportTabPage.ResumeLayout(false);
            this.MainSplitContainer.Panel1.ResumeLayout(false);
            this.MainSplitContainer.ResumeLayout(false);
            this.PacketContextMenuStrip.ResumeLayout(false);
            this.DetailListViewContextMenuStrip.ResumeLayout(false);
            this.MainTreeViewContextMenuStrip.ResumeLayout(false);
            this.LicenseContextMenuStrip.ResumeLayout(false);
            this.customerContextMenuStrip.ResumeLayout(false);
            this.ValidationKeyToolStrip.ResumeLayout(false);
            this.ValidationKeyToolStrip.PerformLayout();
            this.CustomerToolStrip.ResumeLayout(false);
            this.CustomerToolStrip.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip MainMenuStrip;
        private System.Windows.Forms.ToolStrip MainToolStrip;
        private System.Windows.Forms.ToolStripTextBox searchToolStripTextBox;
        private System.Windows.Forms.Panel LicenseViewPanel;
        private System.Windows.Forms.ContextMenuStrip LicenseContextMenuStrip;
        private System.Windows.Forms.ToolStripMenuItem deleteToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator lcmToolStripSeparator2;
        private System.Windows.Forms.ToolStripSeparator lcmToolStripSeparator1;
        private System.Windows.Forms.SplitContainer MainSplitContainer;
        private System.Windows.Forms.TreeView MainTreeView;
        private System.Windows.Forms.SplitContainer LicenseViewSplitContainer;
        private System.Windows.Forms.TreeView DetailTreeView;
        private System.Windows.Forms.TabControl MainTabControl;
        private System.Windows.Forms.TabPage LicensesTabPage;
        private System.Windows.Forms.ToolStripButton createPacketMainToolStripBtn;
        private System.Windows.Forms.TabControl PropertyGridTabControl;
        private System.Windows.Forms.TabPage PropertyGridTabPage;
        private System.Windows.Forms.TabPage TransactionsTabPage;
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
        private System.Windows.Forms.ToolStrip PacketListToolStrip;
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
        private System.Windows.Forms.ColumnHeader PacketNameColumnHeader;
        private System.Windows.Forms.ColumnHeader CreateDateColumnHeader;
        private System.Windows.Forms.ColumnHeader CreatedByColumnHeader;
        private Shared.VisualComponents.LoadingCircle loadingCircle1;
        private System.Windows.Forms.ToolStripButton newCustMainToolStripBtn;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator7;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator8;
        private System.Windows.Forms.ToolStripButton toggleViewMainToolStripBtn;
        private System.Windows.Forms.ToolStripComboBox toolStripComboBox1;
        private System.Windows.Forms.ToolStripLabel toolStripLabel1;
        private System.Windows.Forms.ToolStripLabel ImageToolStripLabel;
        private System.Windows.Forms.TabPage ValidationKeysTabPage;
        private System.Windows.Forms.Panel panel1;
        private Shared.VisualComponents.NoFlickerListView HardwareKeyListView;
        private System.Windows.Forms.ToolStripMenuItem resyncToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem addToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newLicenseToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newProductLicenseToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem existingProductLicenseToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator addLicenseToolStripSeparator;
        private System.Windows.Forms.ToolStripMenuItem newFailoverToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newDisRecToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newTestDevToolStripMenuItem;
        private System.Windows.Forms.ToolStripSplitButton addLicMainToolStripBtn;
        private System.Windows.Forms.ToolStripMenuItem newStandardLicenseToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newFailoverLicenseToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newDisasterRecoveryLicenseToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newTestDevelopmentLicenseToolStripMenuItem;
        private System.Windows.Forms.ToolStripSplitButton addProductLicenseMainToolStripDropDownBtn;
        private System.Windows.Forms.ToolStripMenuItem existingProductLicenseMainToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newProductLicenseMainToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem connectToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.SplitContainer splitContainer2;
        private System.Windows.Forms.ToolStripButton dlvRemoveToolStripButton;
        private System.Windows.Forms.ToolStripButton dlvAddToolStripButton;
        private System.Windows.Forms.ToolStripButton dlvEditToolStripButton;
        private System.Windows.Forms.ToolStripMenuItem expandToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem collapseToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator lcmToolStripSeparator3;
        private System.Windows.Forms.ToolStripLabel SearchToolStripLabel;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem optionsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ColumnHeader ExpDateColumnHeader;
        private System.Windows.Forms.ColumnHeader VerifiedByColumnHeader;
        private System.Windows.Forms.RichTextBox packetDescriptionRichTextBox;
        private System.Windows.Forms.ToolStripMenuItem updateToolStripMenuItem;
        private System.Windows.Forms.ToolStripComboBox moduleFilterToolStripComboBox;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.ToolStripButton navigateBackToolStripButton;
        private System.Windows.Forms.ToolStripButton activateHardwareKeyToolStripButton;
        private System.Windows.Forms.ToolStripButton seekKeyToolStripButton;
        private System.Windows.Forms.ToolStripButton reserveHardwareKeyToolStripButton;
        private System.Windows.Forms.ToolStripButton deactivateHardwareKeyToolStripButton;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem2;
        private System.Windows.Forms.ToolStripMenuItem verifiedToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem unVerifiedToolStripMenuItem;
        private Shared.VisualComponents.NoFlickerListView CustomersListView;
        private System.Windows.Forms.ColumnHeader custNumColumnHeader;
        private System.Windows.Forms.ColumnHeader custNameColumnHeader;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator5;
        private System.Windows.Forms.ContextMenuStrip customerContextMenuStrip;
        private System.Windows.Forms.ToolStripMenuItem newCustomerToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem renameCustomerToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem deleteCustomerToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator6;
        private System.Windows.Forms.ToolStripMenuItem customerToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem viewToolStripMenuItem;
        private System.Windows.Forms.ToolStripButton navigateForwardToolStripButton;
        private System.Windows.Forms.ToolStrip ValidationKeyToolStrip;
        private System.Windows.Forms.ToolStripLabel ValidationKeyCustomerLabel;
        private System.Windows.Forms.ToolStrip CustomerToolStrip;
        private System.Windows.Forms.ToolStripLabel CustomerToolStripLabel;
        private System.Windows.Forms.ToolStripMenuItem updateVersionsToolStripMenuItem;

    }
}

