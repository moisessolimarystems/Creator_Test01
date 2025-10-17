namespace SolimarLicenseDiagnosticDataViewer
{
   partial class Lic_RunningProcessesInfoAttribs_DisplayForm
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Lic_RunningProcessesInfoAttribs_DisplayForm));
			this.serviceBackPanel = new System.Windows.Forms.Panel();
			this.splitContainer1 = new System.Windows.Forms.SplitContainer();
			this.servicesListView = new Shared.VisualComponents.NoFlickerListView();
			this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader5 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader6 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.tabControl1 = new System.Windows.Forms.TabControl();
			this.infoTabPage = new System.Windows.Forms.TabPage();
			this.serverTypeTBox = new System.Windows.Forms.TextBox();
			this.label2 = new System.Windows.Forms.Label();
			this.pathTBox = new System.Windows.Forms.TextBox();
			this.displayTBox = new System.Windows.Forms.TextBox();
			this.runningTBox = new System.Windows.Forms.TextBox();
			this.nameTBox = new System.Windows.Forms.TextBox();
			this.panel1 = new System.Windows.Forms.Panel();
			this.descriptionTBox = new System.Windows.Forms.TextBox();
			this.label9 = new System.Windows.Forms.Label();
			this.label7 = new System.Windows.Forms.Label();
			this.label5 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.toolStrip1 = new System.Windows.Forms.ToolStrip();
			this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
			this.runningTSComboBox = new System.Windows.Forms.ToolStripComboBox();
			this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			this.toolStripLabel2 = new System.Windows.Forms.ToolStripLabel();
			this.serviceTypeTSComboBox = new System.Windows.Forms.ToolStripComboBox();
			this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
			this.highlightKnownEnumationTSButton = new System.Windows.Forms.ToolStripButton();
			this.statusStrip1 = new System.Windows.Forms.StatusStrip();
			this.itemsTSStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
			this.emulatorTlSStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
			this.backPanel = new System.Windows.Forms.Panel();
			this.mainTabControl = new System.Windows.Forms.TabControl();
			this.servicesTabPage = new System.Windows.Forms.TabPage();
			this.serviceBackPanel.SuspendLayout();
			this.splitContainer1.Panel1.SuspendLayout();
			this.splitContainer1.Panel2.SuspendLayout();
			this.splitContainer1.SuspendLayout();
			this.tabControl1.SuspendLayout();
			this.infoTabPage.SuspendLayout();
			this.panel1.SuspendLayout();
			this.toolStrip1.SuspendLayout();
			this.statusStrip1.SuspendLayout();
			this.backPanel.SuspendLayout();
			this.mainTabControl.SuspendLayout();
			this.servicesTabPage.SuspendLayout();
			this.SuspendLayout();
			// 
			// serviceBackPanel
			// 
			this.serviceBackPanel.Controls.Add(this.splitContainer1);
			this.serviceBackPanel.Controls.Add(this.toolStrip1);
			this.serviceBackPanel.Dock = System.Windows.Forms.DockStyle.Fill;
			this.serviceBackPanel.Location = new System.Drawing.Point(3, 3);
			this.serviceBackPanel.Name = "serviceBackPanel";
			this.serviceBackPanel.Size = new System.Drawing.Size(624, 327);
			this.serviceBackPanel.TabIndex = 0;
			// 
			// splitContainer1
			// 
			this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.splitContainer1.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
			this.splitContainer1.Location = new System.Drawing.Point(0, 25);
			this.splitContainer1.Name = "splitContainer1";
			this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
			// 
			// splitContainer1.Panel1
			// 
			this.splitContainer1.Panel1.Controls.Add(this.servicesListView);
			// 
			// splitContainer1.Panel2
			// 
			this.splitContainer1.Panel2.Controls.Add(this.tabControl1);
			this.splitContainer1.Panel2.Padding = new System.Windows.Forms.Padding(3);
			this.splitContainer1.Size = new System.Drawing.Size(624, 302);
			this.splitContainer1.SplitterDistance = 139;
			this.splitContainer1.TabIndex = 3;
			// 
			// servicesListView
			// 
			this.servicesListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3,
            this.columnHeader4,
            this.columnHeader5,
            this.columnHeader6});
			this.servicesListView.Dock = System.Windows.Forms.DockStyle.Fill;
			this.servicesListView.FullRowSelect = true;
			this.servicesListView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
			this.servicesListView.Location = new System.Drawing.Point(0, 0);
			this.servicesListView.MultiSelect = false;
			this.servicesListView.Name = "servicesListView";
			this.servicesListView.Size = new System.Drawing.Size(624, 139);
			this.servicesListView.Sorting = System.Windows.Forms.SortOrder.Ascending;
			this.servicesListView.TabIndex = 1;
			this.servicesListView.UseCompatibleStateImageBehavior = false;
			this.servicesListView.View = System.Windows.Forms.View.Details;
			this.servicesListView.SelectedIndexChanged += new System.EventHandler(this.servicesListView_SelectedIndexChanged);
			this.servicesListView.KeyDown += new System.Windows.Forms.KeyEventHandler(this.general_KeyDown);
			// 
			// columnHeader1
			// 
			this.columnHeader1.Text = "Name";
			this.columnHeader1.Width = 89;
			// 
			// columnHeader2
			// 
			this.columnHeader2.Text = "Running";
			// 
			// columnHeader3
			// 
			this.columnHeader3.Text = "Type";
			this.columnHeader3.Width = 91;
			// 
			// columnHeader4
			// 
			this.columnHeader4.Text = "Path";
			this.columnHeader4.Width = 108;
			// 
			// columnHeader5
			// 
			this.columnHeader5.Text = "Display Name";
			this.columnHeader5.Width = 138;
			// 
			// columnHeader6
			// 
			this.columnHeader6.Text = "Description";
			this.columnHeader6.Width = 117;
			// 
			// tabControl1
			// 
			this.tabControl1.Controls.Add(this.infoTabPage);
			this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tabControl1.Location = new System.Drawing.Point(3, 3);
			this.tabControl1.Name = "tabControl1";
			this.tabControl1.SelectedIndex = 0;
			this.tabControl1.Size = new System.Drawing.Size(618, 153);
			this.tabControl1.TabIndex = 0;
			// 
			// infoTabPage
			// 
			this.infoTabPage.Controls.Add(this.serverTypeTBox);
			this.infoTabPage.Controls.Add(this.label2);
			this.infoTabPage.Controls.Add(this.pathTBox);
			this.infoTabPage.Controls.Add(this.displayTBox);
			this.infoTabPage.Controls.Add(this.runningTBox);
			this.infoTabPage.Controls.Add(this.nameTBox);
			this.infoTabPage.Controls.Add(this.panel1);
			this.infoTabPage.Controls.Add(this.label9);
			this.infoTabPage.Controls.Add(this.label7);
			this.infoTabPage.Controls.Add(this.label5);
			this.infoTabPage.Controls.Add(this.label3);
			this.infoTabPage.Controls.Add(this.label1);
			this.infoTabPage.Location = new System.Drawing.Point(4, 22);
			this.infoTabPage.Name = "infoTabPage";
			this.infoTabPage.Padding = new System.Windows.Forms.Padding(3);
			this.infoTabPage.Size = new System.Drawing.Size(610, 127);
			this.infoTabPage.TabIndex = 0;
			this.infoTabPage.Text = "General";
			this.infoTabPage.UseVisualStyleBackColor = true;
			// 
			// serverTypeTBox
			// 
			this.serverTypeTBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.serverTypeTBox.Location = new System.Drawing.Point(465, 30);
			this.serverTypeTBox.Name = "serverTypeTBox";
			this.serverTypeTBox.ReadOnly = true;
			this.serverTypeTBox.Size = new System.Drawing.Size(139, 20);
			this.serverTypeTBox.TabIndex = 3;
			// 
			// label2
			// 
			this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(409, 33);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(34, 13);
			this.label2.TabIndex = 11;
			this.label2.Text = "Type:";
			// 
			// pathTBox
			// 
			this.pathTBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.pathTBox.Location = new System.Drawing.Point(87, 56);
			this.pathTBox.Name = "pathTBox";
			this.pathTBox.ReadOnly = true;
			this.pathTBox.Size = new System.Drawing.Size(517, 20);
			this.pathTBox.TabIndex = 4;
			// 
			// displayTBox
			// 
			this.displayTBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.displayTBox.Location = new System.Drawing.Point(87, 30);
			this.displayTBox.Name = "displayTBox";
			this.displayTBox.ReadOnly = true;
			this.displayTBox.Size = new System.Drawing.Size(305, 20);
			this.displayTBox.TabIndex = 2;
			// 
			// runningTBox
			// 
			this.runningTBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.runningTBox.Location = new System.Drawing.Point(465, 4);
			this.runningTBox.Name = "runningTBox";
			this.runningTBox.ReadOnly = true;
			this.runningTBox.Size = new System.Drawing.Size(139, 20);
			this.runningTBox.TabIndex = 1;
			// 
			// nameTBox
			// 
			this.nameTBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.nameTBox.Location = new System.Drawing.Point(87, 4);
			this.nameTBox.Name = "nameTBox";
			this.nameTBox.ReadOnly = true;
			this.nameTBox.Size = new System.Drawing.Size(305, 20);
			this.nameTBox.TabIndex = 0;
			// 
			// panel1
			// 
			this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.panel1.AutoScroll = true;
			this.panel1.Controls.Add(this.descriptionTBox);
			this.panel1.Location = new System.Drawing.Point(87, 82);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(517, 42);
			this.panel1.TabIndex = 10;
			// 
			// descriptionTBox
			// 
			this.descriptionTBox.Dock = System.Windows.Forms.DockStyle.Fill;
			this.descriptionTBox.Location = new System.Drawing.Point(0, 0);
			this.descriptionTBox.Multiline = true;
			this.descriptionTBox.Name = "descriptionTBox";
			this.descriptionTBox.ReadOnly = true;
			this.descriptionTBox.ScrollBars = System.Windows.Forms.ScrollBars.Both;
			this.descriptionTBox.Size = new System.Drawing.Size(517, 42);
			this.descriptionTBox.TabIndex = 5;
			// 
			// label9
			// 
			this.label9.AutoSize = true;
			this.label9.Location = new System.Drawing.Point(6, 85);
			this.label9.Name = "label9";
			this.label9.Size = new System.Drawing.Size(63, 13);
			this.label9.TabIndex = 8;
			this.label9.Text = "Description:";
			// 
			// label7
			// 
			this.label7.AutoSize = true;
			this.label7.Location = new System.Drawing.Point(6, 33);
			this.label7.Name = "label7";
			this.label7.Size = new System.Drawing.Size(75, 13);
			this.label7.TabIndex = 6;
			this.label7.Text = "Display Name:";
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.Location = new System.Drawing.Point(6, 59);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(32, 13);
			this.label5.TabIndex = 4;
			this.label5.Text = "Path:";
			// 
			// label3
			// 
			this.label3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(409, 7);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(50, 13);
			this.label3.TabIndex = 2;
			this.label3.Text = "Running:";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(6, 7);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(38, 13);
			this.label1.TabIndex = 0;
			this.label1.Text = "Name:";
			// 
			// toolStrip1
			// 
			this.toolStrip1.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripLabel1,
            this.runningTSComboBox,
            this.toolStripSeparator1,
            this.toolStripLabel2,
            this.serviceTypeTSComboBox,
            this.toolStripSeparator2,
            this.highlightKnownEnumationTSButton});
			this.toolStrip1.Location = new System.Drawing.Point(0, 0);
			this.toolStrip1.Name = "toolStrip1";
			this.toolStrip1.Size = new System.Drawing.Size(624, 25);
			this.toolStrip1.TabIndex = 0;
			this.toolStrip1.Text = "mainToolStrip";
			// 
			// toolStripLabel1
			// 
			this.toolStripLabel1.Name = "toolStripLabel1";
			this.toolStripLabel1.Size = new System.Drawing.Size(55, 22);
			this.toolStripLabel1.Text = "Running:";
			// 
			// runningTSComboBox
			// 
			this.runningTSComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.runningTSComboBox.Items.AddRange(new object[] {
            "<All>",
            "True"});
			this.runningTSComboBox.Name = "runningTSComboBox";
			this.runningTSComboBox.Size = new System.Drawing.Size(121, 25);
			this.runningTSComboBox.SelectedIndexChanged += new System.EventHandler(this.filterTSComboBox_SelectedIndexChanged);
			// 
			// toolStripSeparator1
			// 
			this.toolStripSeparator1.Name = "toolStripSeparator1";
			this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
			// 
			// toolStripLabel2
			// 
			this.toolStripLabel2.Name = "toolStripLabel2";
			this.toolStripLabel2.Size = new System.Drawing.Size(76, 22);
			this.toolStripLabel2.Text = "Service Type:";
			// 
			// serviceTypeTSComboBox
			// 
			this.serviceTypeTSComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.serviceTypeTSComboBox.Items.AddRange(new object[] {
            "<All>",
            "Service",
            "System Driver"});
			this.serviceTypeTSComboBox.Name = "serviceTypeTSComboBox";
			this.serviceTypeTSComboBox.Size = new System.Drawing.Size(121, 25);
			this.serviceTypeTSComboBox.SelectedIndexChanged += new System.EventHandler(this.serviceTypeTSComboBox_SelectedIndexChanged);
			// 
			// toolStripSeparator2
			// 
			this.toolStripSeparator2.Name = "toolStripSeparator2";
			this.toolStripSeparator2.Size = new System.Drawing.Size(6, 25);
			// 
			// highlightKnownEnumationTSButton
			// 
			this.highlightKnownEnumationTSButton.CheckOnClick = true;
			this.highlightKnownEnumationTSButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
			this.highlightKnownEnumationTSButton.Image = ((System.Drawing.Image)(resources.GetObject("highlightKnownEnumationTSButton.Image")));
			this.highlightKnownEnumationTSButton.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.highlightKnownEnumationTSButton.Name = "highlightKnownEnumationTSButton";
			this.highlightKnownEnumationTSButton.Size = new System.Drawing.Size(163, 22);
			this.highlightKnownEnumationTSButton.Text = "Highlight Emulation Services";
			this.highlightKnownEnumationTSButton.ToolTipText = "Highlight All Known Emulation Services";
			this.highlightKnownEnumationTSButton.Click += new System.EventHandler(this.highlightKnownEnumationTSButton_Click);
			// 
			// statusStrip1
			// 
			this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.itemsTSStatusLabel,
            this.emulatorTlSStatusLabel});
			this.statusStrip1.Location = new System.Drawing.Point(0, 365);
			this.statusStrip1.Name = "statusStrip1";
			this.statusStrip1.Size = new System.Drawing.Size(644, 24);
			this.statusStrip1.TabIndex = 2;
			this.statusStrip1.Text = "statusStrip1";
			// 
			// itemsTSStatusLabel
			// 
			this.itemsTSStatusLabel.Name = "itemsTSStatusLabel";
			this.itemsTSStatusLabel.Size = new System.Drawing.Size(44, 19);
			this.itemsTSStatusLabel.Text = "[Items]";
			// 
			// emulatorTlSStatusLabel
			// 
			this.emulatorTlSStatusLabel.BorderSides = System.Windows.Forms.ToolStripStatusLabelBorderSides.Left;
			this.emulatorTlSStatusLabel.Name = "emulatorTlSStatusLabel";
			this.emulatorTlSStatusLabel.Size = new System.Drawing.Size(72, 19);
			this.emulatorTlSStatusLabel.Text = "[Emulators]";
			// 
			// backPanel
			// 
			this.backPanel.Controls.Add(this.mainTabControl);
			this.backPanel.Dock = System.Windows.Forms.DockStyle.Fill;
			this.backPanel.Location = new System.Drawing.Point(0, 0);
			this.backPanel.Name = "backPanel";
			this.backPanel.Padding = new System.Windows.Forms.Padding(3);
			this.backPanel.Size = new System.Drawing.Size(644, 365);
			this.backPanel.TabIndex = 1;
			// 
			// mainTabControl
			// 
			this.mainTabControl.Controls.Add(this.servicesTabPage);
			this.mainTabControl.Dock = System.Windows.Forms.DockStyle.Fill;
			this.mainTabControl.Location = new System.Drawing.Point(3, 3);
			this.mainTabControl.Name = "mainTabControl";
			this.mainTabControl.SelectedIndex = 0;
			this.mainTabControl.Size = new System.Drawing.Size(638, 359);
			this.mainTabControl.TabIndex = 1;
			// 
			// servicesTabPage
			// 
			this.servicesTabPage.Controls.Add(this.serviceBackPanel);
			this.servicesTabPage.Location = new System.Drawing.Point(4, 22);
			this.servicesTabPage.Name = "servicesTabPage";
			this.servicesTabPage.Padding = new System.Windows.Forms.Padding(3);
			this.servicesTabPage.Size = new System.Drawing.Size(630, 333);
			this.servicesTabPage.TabIndex = 1;
			this.servicesTabPage.Text = "Services & System Drivers";
			this.servicesTabPage.UseVisualStyleBackColor = true;
			// 
			// Lic_RunningProcessesInfoAttribs_DisplayForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(644, 389);
			this.Controls.Add(this.backPanel);
			this.Controls.Add(this.statusStrip1);
			this.Name = "Lic_RunningProcessesInfoAttribs_DisplayForm";
			this.Text = "Lic_RunningProcessesInfoAttribs";
			this.serviceBackPanel.ResumeLayout(false);
			this.serviceBackPanel.PerformLayout();
			this.splitContainer1.Panel1.ResumeLayout(false);
			this.splitContainer1.Panel2.ResumeLayout(false);
			this.splitContainer1.ResumeLayout(false);
			this.tabControl1.ResumeLayout(false);
			this.infoTabPage.ResumeLayout(false);
			this.infoTabPage.PerformLayout();
			this.panel1.ResumeLayout(false);
			this.panel1.PerformLayout();
			this.toolStrip1.ResumeLayout(false);
			this.toolStrip1.PerformLayout();
			this.statusStrip1.ResumeLayout(false);
			this.statusStrip1.PerformLayout();
			this.backPanel.ResumeLayout(false);
			this.mainTabControl.ResumeLayout(false);
			this.servicesTabPage.ResumeLayout(false);
			this.ResumeLayout(false);
			this.PerformLayout();

      }

      #endregion

      private System.Windows.Forms.Panel serviceBackPanel;
      private Shared.VisualComponents.NoFlickerListView servicesListView;
      private System.Windows.Forms.ToolStrip toolStrip1;
      private System.Windows.Forms.ColumnHeader columnHeader1;
      private System.Windows.Forms.ColumnHeader columnHeader2;
      private System.Windows.Forms.ColumnHeader columnHeader3;
      private System.Windows.Forms.ColumnHeader columnHeader4;
      private System.Windows.Forms.ColumnHeader columnHeader5;
      private System.Windows.Forms.ToolStripLabel toolStripLabel1;
      private System.Windows.Forms.ToolStripComboBox runningTSComboBox;
      private System.Windows.Forms.StatusStrip statusStrip1;
      private System.Windows.Forms.ToolStripStatusLabel itemsTSStatusLabel;
      private System.Windows.Forms.SplitContainer splitContainer1;
      private System.Windows.Forms.TabControl tabControl1;
      private System.Windows.Forms.TabPage infoTabPage;
      private System.Windows.Forms.Label label9;
      private System.Windows.Forms.Label label7;
      private System.Windows.Forms.Label label5;
      private System.Windows.Forms.Label label3;
      private System.Windows.Forms.Label label1;
      private System.Windows.Forms.Panel panel1;
      private System.Windows.Forms.TextBox descriptionTBox;
      private System.Windows.Forms.TextBox pathTBox;
      private System.Windows.Forms.TextBox displayTBox;
      private System.Windows.Forms.TextBox runningTBox;
      private System.Windows.Forms.TextBox nameTBox;
		private System.Windows.Forms.Panel backPanel;
		private System.Windows.Forms.TabControl mainTabControl;
		private System.Windows.Forms.TabPage servicesTabPage;
		private System.Windows.Forms.ColumnHeader columnHeader6;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
		private System.Windows.Forms.ToolStripLabel toolStripLabel2;
		private System.Windows.Forms.ToolStripComboBox serviceTypeTSComboBox;
		private System.Windows.Forms.TextBox serverTypeTBox;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
		private System.Windows.Forms.ToolStripButton highlightKnownEnumationTSButton;
		private System.Windows.Forms.ToolStripStatusLabel emulatorTlSStatusLabel;
   }
}