namespace SolimarLicenseDiagnosticDataViewer
{
    partial class LicenseDiagnosticDataViewerForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(LicenseDiagnosticDataViewerForm));
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.connectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.openLicenseSystemDataToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.closeLicenseSystemDataToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.windowToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.cascadeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.tileHorizontalToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.tileVerticalToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.arrangeIconsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.testActionToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.testToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.makeObjectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.makeLicPackageToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.panel1 = new System.Windows.Forms.Panel();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.bottumSplitter = new System.Windows.Forms.Splitter();
            this.leftPanel = new System.Windows.Forms.Panel();
            this.leftTreeView = new System.Windows.Forms.TreeView();
            this.leftSplitter = new System.Windows.Forms.Splitter();
            this.generalToolTip = new System.Windows.Forms.ToolTip(this.components);
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabel2 = new System.Windows.Forms.ToolStripStatusLabel();
            this.imageList = new System.Windows.Forms.ImageList(this.components);
            this.menuStrip1.SuspendLayout();
            this.panel1.SuspendLayout();
            this.leftPanel.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.windowToolStripMenuItem,
            this.testActionToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.MdiWindowListItem = this.windowToolStripMenuItem;
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1149, 24);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.connectToolStripMenuItem,
            this.toolStripSeparator2,
            this.openLicenseSystemDataToolStripMenuItem,
            this.closeLicenseSystemDataToolStripMenuItem,
            this.toolStripSeparator1,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "&File";
            this.fileToolStripMenuItem.DropDownOpening += new System.EventHandler(this.fileToolStripMenuItem_DropDownOpening);
            // 
            // connectToolStripMenuItem
            // 
            this.connectToolStripMenuItem.Name = "connectToolStripMenuItem";
            this.connectToolStripMenuItem.Size = new System.Drawing.Size(213, 22);
            this.connectToolStripMenuItem.Text = "&Connect";
            this.connectToolStripMenuItem.Click += new System.EventHandler(this.connectToolStripMenuItem_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(210, 6);
            // 
            // openLicenseSystemDataToolStripMenuItem
            // 
            this.openLicenseSystemDataToolStripMenuItem.Name = "openLicenseSystemDataToolStripMenuItem";
            this.openLicenseSystemDataToolStripMenuItem.Size = new System.Drawing.Size(213, 22);
            this.openLicenseSystemDataToolStripMenuItem.Text = "&Open License System Data";
            this.openLicenseSystemDataToolStripMenuItem.Click += new System.EventHandler(this.openLicenseSystemDataToolStripMenuItem_Click);
            // 
            // closeLicenseSystemDataToolStripMenuItem
            // 
            this.closeLicenseSystemDataToolStripMenuItem.Name = "closeLicenseSystemDataToolStripMenuItem";
            this.closeLicenseSystemDataToolStripMenuItem.Size = new System.Drawing.Size(213, 22);
            this.closeLicenseSystemDataToolStripMenuItem.Text = "&Close License System Data";
            this.closeLicenseSystemDataToolStripMenuItem.Visible = false;
            this.closeLicenseSystemDataToolStripMenuItem.Click += new System.EventHandler(this.closeLicenseSystemDataToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(210, 6);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(213, 22);
            this.exitToolStripMenuItem.Text = "E&xit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // windowToolStripMenuItem
            // 
            this.windowToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.cascadeToolStripMenuItem,
            this.tileHorizontalToolStripMenuItem,
            this.tileVerticalToolStripMenuItem,
            this.arrangeIconsToolStripMenuItem});
            this.windowToolStripMenuItem.Name = "windowToolStripMenuItem";
            this.windowToolStripMenuItem.Size = new System.Drawing.Size(63, 20);
            this.windowToolStripMenuItem.Text = "&Window";
            // 
            // cascadeToolStripMenuItem
            // 
            this.cascadeToolStripMenuItem.Name = "cascadeToolStripMenuItem";
            this.cascadeToolStripMenuItem.Size = new System.Drawing.Size(151, 22);
            this.cascadeToolStripMenuItem.Text = "&Cascade";
            this.cascadeToolStripMenuItem.Click += new System.EventHandler(this.cascadeToolStripMenuItem_Click);
            // 
            // tileHorizontalToolStripMenuItem
            // 
            this.tileHorizontalToolStripMenuItem.Name = "tileHorizontalToolStripMenuItem";
            this.tileHorizontalToolStripMenuItem.Size = new System.Drawing.Size(151, 22);
            this.tileHorizontalToolStripMenuItem.Text = "Tile &Horizontal";
            this.tileHorizontalToolStripMenuItem.Click += new System.EventHandler(this.tileHorizontalToolStripMenuItem_Click);
            // 
            // tileVerticalToolStripMenuItem
            // 
            this.tileVerticalToolStripMenuItem.Name = "tileVerticalToolStripMenuItem";
            this.tileVerticalToolStripMenuItem.Size = new System.Drawing.Size(151, 22);
            this.tileVerticalToolStripMenuItem.Text = "Tile &Vertical";
            this.tileVerticalToolStripMenuItem.Click += new System.EventHandler(this.tileVerticalToolStripMenuItem_Click);
            // 
            // arrangeIconsToolStripMenuItem
            // 
            this.arrangeIconsToolStripMenuItem.Name = "arrangeIconsToolStripMenuItem";
            this.arrangeIconsToolStripMenuItem.Size = new System.Drawing.Size(151, 22);
            this.arrangeIconsToolStripMenuItem.Text = "&Arrange Icons";
            this.arrangeIconsToolStripMenuItem.Click += new System.EventHandler(this.arrangeIconsToolStripMenuItem_Click);
            // 
            // testActionToolStripMenuItem
            // 
            this.testActionToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.testToolStripMenuItem,
            this.makeObjectToolStripMenuItem,
            this.makeLicPackageToolStripMenuItem});
            this.testActionToolStripMenuItem.Name = "testActionToolStripMenuItem";
            this.testActionToolStripMenuItem.Size = new System.Drawing.Size(79, 20);
            this.testActionToolStripMenuItem.Text = "Test Action";
            this.testActionToolStripMenuItem.Visible = false;
            // 
            // testToolStripMenuItem
            // 
            this.testToolStripMenuItem.Name = "testToolStripMenuItem";
            this.testToolStripMenuItem.Size = new System.Drawing.Size(165, 22);
            this.testToolStripMenuItem.Text = "Test";
            this.testToolStripMenuItem.Click += new System.EventHandler(this.testToolStripMenuItem_Click);
            // 
            // makeObjectToolStripMenuItem
            // 
            this.makeObjectToolStripMenuItem.Name = "makeObjectToolStripMenuItem";
            this.makeObjectToolStripMenuItem.Size = new System.Drawing.Size(165, 22);
            this.makeObjectToolStripMenuItem.Text = "Make Object";
            this.makeObjectToolStripMenuItem.Click += new System.EventHandler(this.makeObjectToolStripMenuItem_Click);
            // 
            // makeLicPackageToolStripMenuItem
            // 
            this.makeLicPackageToolStripMenuItem.Name = "makeLicPackageToolStripMenuItem";
            this.makeLicPackageToolStripMenuItem.Size = new System.Drawing.Size(165, 22);
            this.makeLicPackageToolStripMenuItem.Text = "Make LicPackage";
            this.makeLicPackageToolStripMenuItem.Click += new System.EventHandler(this.makeLicPackageToolStripMenuItem_Click);
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.textBox1);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panel1.Location = new System.Drawing.Point(0, 501);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(1149, 87);
            this.panel1.TabIndex = 3;
            this.panel1.Visible = false;
            // 
            // textBox1
            // 
            this.textBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBox1.Location = new System.Drawing.Point(0, 0);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBox1.Size = new System.Drawing.Size(1149, 87);
            this.textBox1.TabIndex = 0;
            // 
            // bottumSplitter
            // 
            this.bottumSplitter.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.bottumSplitter.Location = new System.Drawing.Point(0, 496);
            this.bottumSplitter.Name = "bottumSplitter";
            this.bottumSplitter.Size = new System.Drawing.Size(1149, 5);
            this.bottumSplitter.TabIndex = 5;
            this.bottumSplitter.TabStop = false;
            this.bottumSplitter.Visible = false;
            // 
            // leftPanel
            // 
            this.leftPanel.Controls.Add(this.leftTreeView);
            this.leftPanel.Dock = System.Windows.Forms.DockStyle.Left;
            this.leftPanel.Location = new System.Drawing.Point(0, 24);
            this.leftPanel.Name = "leftPanel";
            this.leftPanel.Size = new System.Drawing.Size(188, 472);
            this.leftPanel.TabIndex = 6;
            // 
            // leftTreeView
            // 
            this.leftTreeView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.leftTreeView.FullRowSelect = true;
            this.leftTreeView.HideSelection = false;
            this.leftTreeView.Location = new System.Drawing.Point(0, 0);
            this.leftTreeView.Name = "leftTreeView";
            this.leftTreeView.ShowLines = false;
            this.leftTreeView.ShowNodeToolTips = true;
            this.leftTreeView.Size = new System.Drawing.Size(188, 472);
            this.leftTreeView.TabIndex = 10;
            this.leftTreeView.DoubleClick += new System.EventHandler(this.leftTreeView_DoubleClick);
            // 
            // leftSplitter
            // 
            this.leftSplitter.Location = new System.Drawing.Point(188, 24);
            this.leftSplitter.Name = "leftSplitter";
            this.leftSplitter.Size = new System.Drawing.Size(5, 472);
            this.leftSplitter.TabIndex = 7;
            this.leftSplitter.TabStop = false;
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1,
            this.toolStripStatusLabel2});
            this.statusStrip1.Location = new System.Drawing.Point(0, 588);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(1149, 24);
            this.statusStrip1.TabIndex = 9;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(33, 19);
            this.toolStripStatusLabel1.Text = "[File]";
            this.toolStripStatusLabel1.Visible = false;
            // 
            // toolStripStatusLabel2
            // 
            this.toolStripStatusLabel2.BorderSides = System.Windows.Forms.ToolStripStatusLabelBorderSides.Left;
            this.toolStripStatusLabel2.Name = "toolStripStatusLabel2";
            this.toolStripStatusLabel2.Size = new System.Drawing.Size(66, 19);
            this.toolStripStatusLabel2.Text = "[File Type]";
            this.toolStripStatusLabel2.Visible = false;
            // 
            // imageList
            // 
            this.imageList.ColorDepth = System.Windows.Forms.ColorDepth.Depth32Bit;
            this.imageList.ImageSize = new System.Drawing.Size(16, 16);
            this.imageList.TransparentColor = System.Drawing.Color.Transparent;
            // 
            // LicenseDiagnosticDataViewerForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1149, 612);
            this.Controls.Add(this.leftSplitter);
            this.Controls.Add(this.leftPanel);
            this.Controls.Add(this.bottumSplitter);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.menuStrip1);
            this.Controls.Add(this.statusStrip1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.IsMdiContainer = true;
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "LicenseDiagnosticDataViewerForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Solimar License Diagnostic Data Viewer - Disconnected";
            this.Activated += new System.EventHandler(this.LicenseDiagnosticDataViewerForm_Activated);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.LicenseDiagnosticDataViewerForm_FormClosing);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.leftPanel.ResumeLayout(false);
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openLicenseSystemDataToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem closeLicenseSystemDataToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem testActionToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem testToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem windowToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem cascadeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem tileHorizontalToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem tileVerticalToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem arrangeIconsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem connectToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.ToolStripMenuItem makeObjectToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem makeLicPackageToolStripMenuItem;
        private System.Windows.Forms.Splitter bottumSplitter;
        private System.Windows.Forms.Panel leftPanel;
        private System.Windows.Forms.Splitter leftSplitter;
        private System.Windows.Forms.TreeView leftTreeView;
        private System.Windows.Forms.ToolTip generalToolTip;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ImageList imageList;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel2;
    }
}

