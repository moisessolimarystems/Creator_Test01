namespace SolimarLicenseDiagnosticDataViewer
{
	partial class Lic_ServerDataAttribs_DisplayForm
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
			this.headerPanel = new System.Windows.Forms.Panel();
			this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
			this.copyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.lastClockViolationLabel = new System.Windows.Forms.Label();
			this.lastClockViolationTitleLabel = new System.Windows.Forms.Label();
			this.clockViolCountLabel = new System.Windows.Forms.Label();
			this.clockViolCountTitleLabel = new System.Windows.Forms.Label();
			this.inClockViolLabel = new System.Windows.Forms.Label();
			this.inClockViolTitleLabel = new System.Windows.Forms.Label();
			this.lastTouchLabel = new System.Windows.Forms.Label();
			this.versionLabel = new System.Windows.Forms.Label();
			this.lastTouchTitleLabel = new System.Windows.Forms.Label();
			this.versionTitleLabel = new System.Windows.Forms.Label();
			this.noFlickerListView1 = new Shared.VisualComponents.NoFlickerListView();
			this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader3 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader4 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader5 = new System.Windows.Forms.ColumnHeader();
			this.splitContainer1 = new System.Windows.Forms.SplitContainer();
			this.clockViolationsListView = new Shared.VisualComponents.NoFlickerListView();
			this.columnHeader6 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader7 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader8 = new System.Windows.Forms.ColumnHeader();
			this.headerPanel.SuspendLayout();
			this.contextMenuStrip1.SuspendLayout();
			this.splitContainer1.Panel1.SuspendLayout();
			this.splitContainer1.Panel2.SuspendLayout();
			this.splitContainer1.SuspendLayout();
			this.SuspendLayout();
			// 
			// headerPanel
			// 
			this.headerPanel.ContextMenuStrip = this.contextMenuStrip1;
			this.headerPanel.Controls.Add(this.lastClockViolationLabel);
			this.headerPanel.Controls.Add(this.lastClockViolationTitleLabel);
			this.headerPanel.Controls.Add(this.clockViolCountLabel);
			this.headerPanel.Controls.Add(this.clockViolCountTitleLabel);
			this.headerPanel.Controls.Add(this.inClockViolLabel);
			this.headerPanel.Controls.Add(this.inClockViolTitleLabel);
			this.headerPanel.Controls.Add(this.lastTouchLabel);
			this.headerPanel.Controls.Add(this.versionLabel);
			this.headerPanel.Controls.Add(this.lastTouchTitleLabel);
			this.headerPanel.Controls.Add(this.versionTitleLabel);
			this.headerPanel.Dock = System.Windows.Forms.DockStyle.Top;
			this.headerPanel.Location = new System.Drawing.Point(0, 0);
			this.headerPanel.Name = "headerPanel";
			this.headerPanel.Size = new System.Drawing.Size(605, 57);
			this.headerPanel.TabIndex = 0;
			// 
			// contextMenuStrip1
			// 
			this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.copyToolStripMenuItem});
			this.contextMenuStrip1.Name = "contextMenuStrip1";
			this.contextMenuStrip1.Size = new System.Drawing.Size(103, 26);
			this.contextMenuStrip1.Click += new System.EventHandler(this.contextMenuStrip1_Click);
			// 
			// copyToolStripMenuItem
			// 
			this.copyToolStripMenuItem.Name = "copyToolStripMenuItem";
			this.copyToolStripMenuItem.Size = new System.Drawing.Size(102, 22);
			this.copyToolStripMenuItem.Text = "Copy";
			// 
			// lastClockViolationLabel
			// 
			this.lastClockViolationLabel.AutoSize = true;
			this.lastClockViolationLabel.Location = new System.Drawing.Point(439, 33);
			this.lastClockViolationLabel.Name = "lastClockViolationLabel";
			this.lastClockViolationLabel.Size = new System.Drawing.Size(132, 13);
			this.lastClockViolationLabel.TabIndex = 9;
			this.lastClockViolationLabel.Text = "[Last Clock Violation Date]";
			// 
			// lastClockViolationTitleLabel
			// 
			this.lastClockViolationTitleLabel.AutoSize = true;
			this.lastClockViolationTitleLabel.Location = new System.Drawing.Point(302, 33);
			this.lastClockViolationTitleLabel.Name = "lastClockViolationTitleLabel";
			this.lastClockViolationTitleLabel.Size = new System.Drawing.Size(129, 13);
			this.lastClockViolationTitleLabel.TabIndex = 8;
			this.lastClockViolationTitleLabel.Text = "Last Clock Violation Date:";
			// 
			// clockViolCountLabel
			// 
			this.clockViolCountLabel.AutoSize = true;
			this.clockViolCountLabel.Location = new System.Drawing.Point(439, 20);
			this.clockViolCountLabel.Name = "clockViolCountLabel";
			this.clockViolCountLabel.Size = new System.Drawing.Size(114, 13);
			this.clockViolCountLabel.TabIndex = 7;
			this.clockViolCountLabel.Text = "[Clock Violation Count]";
			// 
			// clockViolCountTitleLabel
			// 
			this.clockViolCountTitleLabel.AutoSize = true;
			this.clockViolCountTitleLabel.Location = new System.Drawing.Point(302, 22);
			this.clockViolCountTitleLabel.Name = "clockViolCountTitleLabel";
			this.clockViolCountTitleLabel.Size = new System.Drawing.Size(120, 13);
			this.clockViolCountTitleLabel.TabIndex = 6;
			this.clockViolCountTitleLabel.Text = "Clock Violation Counter:";
			// 
			// inClockViolLabel
			// 
			this.inClockViolLabel.AutoSize = true;
			this.inClockViolLabel.Location = new System.Drawing.Point(439, 7);
			this.inClockViolLabel.Name = "inClockViolLabel";
			this.inClockViolLabel.Size = new System.Drawing.Size(95, 13);
			this.inClockViolLabel.TabIndex = 5;
			this.inClockViolLabel.Text = "[In Clock Violation]";
			// 
			// inClockViolTitleLabel
			// 
			this.inClockViolTitleLabel.AutoSize = true;
			this.inClockViolTitleLabel.Location = new System.Drawing.Point(302, 9);
			this.inClockViolTitleLabel.Name = "inClockViolTitleLabel";
			this.inClockViolTitleLabel.Size = new System.Drawing.Size(92, 13);
			this.inClockViolTitleLabel.TabIndex = 4;
			this.inClockViolTitleLabel.Text = "In Clock Violation:";
			// 
			// lastTouchLabel
			// 
			this.lastTouchLabel.AutoSize = true;
			this.lastTouchLabel.Location = new System.Drawing.Point(92, 22);
			this.lastTouchLabel.Name = "lastTouchLabel";
			this.lastTouchLabel.Size = new System.Drawing.Size(87, 13);
			this.lastTouchLabel.TabIndex = 3;
			this.lastTouchLabel.Text = "[LastTouchDate]";
			// 
			// versionLabel
			// 
			this.versionLabel.AutoSize = true;
			this.versionLabel.Location = new System.Drawing.Point(92, 9);
			this.versionLabel.Name = "versionLabel";
			this.versionLabel.Size = new System.Drawing.Size(48, 13);
			this.versionLabel.TabIndex = 2;
			this.versionLabel.Text = "[Version]";
			// 
			// lastTouchTitleLabel
			// 
			this.lastTouchTitleLabel.AutoSize = true;
			this.lastTouchTitleLabel.Location = new System.Drawing.Point(3, 22);
			this.lastTouchTitleLabel.Name = "lastTouchTitleLabel";
			this.lastTouchTitleLabel.Size = new System.Drawing.Size(90, 13);
			this.lastTouchTitleLabel.TabIndex = 1;
			this.lastTouchTitleLabel.Text = "Last Touch Date:";
			// 
			// versionTitleLabel
			// 
			this.versionTitleLabel.AutoSize = true;
			this.versionTitleLabel.Location = new System.Drawing.Point(3, 9);
			this.versionTitleLabel.Name = "versionTitleLabel";
			this.versionTitleLabel.Size = new System.Drawing.Size(45, 13);
			this.versionTitleLabel.TabIndex = 0;
			this.versionTitleLabel.Text = "Version:";
			// 
			// noFlickerListView1
			// 
			this.noFlickerListView1.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3,
            this.columnHeader4,
            this.columnHeader5});
			this.noFlickerListView1.ContextMenuStrip = this.contextMenuStrip1;
			this.noFlickerListView1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.noFlickerListView1.FullRowSelect = true;
			this.noFlickerListView1.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
			this.noFlickerListView1.Location = new System.Drawing.Point(0, 57);
			this.noFlickerListView1.Name = "noFlickerListView1";
			this.noFlickerListView1.Size = new System.Drawing.Size(605, 114);
			this.noFlickerListView1.Sorting = System.Windows.Forms.SortOrder.Ascending;
			this.noFlickerListView1.TabIndex = 1;
			this.noFlickerListView1.UseCompatibleStateImageBehavior = false;
			this.noFlickerListView1.View = System.Windows.Forms.View.Details;
			this.noFlickerListView1.ColumnClick += new System.Windows.Forms.ColumnClickEventHandler(this.noFlickerListView1_ColumnClick);
			this.noFlickerListView1.KeyDown += new System.Windows.Forms.KeyEventHandler(this.general_KeyDown);
			// 
			// columnHeader1
			// 
			this.columnHeader1.Text = "License Name";
			this.columnHeader1.Width = 100;
			// 
			// columnHeader2
			// 
			this.columnHeader2.Text = "Verification Code";
			this.columnHeader2.Width = 100;
			// 
			// columnHeader3
			// 
			this.columnHeader3.Text = "File Name";
			this.columnHeader3.Width = 114;
			// 
			// columnHeader4
			// 
			this.columnHeader4.Text = "Modified Date";
			this.columnHeader4.Width = 155;
			// 
			// columnHeader5
			// 
			this.columnHeader5.Text = "Current Activations";
			this.columnHeader5.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			this.columnHeader5.Width = 120;
			// 
			// splitContainer1
			// 
			this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.splitContainer1.Location = new System.Drawing.Point(0, 0);
			this.splitContainer1.Name = "splitContainer1";
			this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
			// 
			// splitContainer1.Panel1
			// 
			this.splitContainer1.Panel1.Controls.Add(this.noFlickerListView1);
			this.splitContainer1.Panel1.Controls.Add(this.headerPanel);
			// 
			// splitContainer1.Panel2
			// 
			this.splitContainer1.Panel2.Controls.Add(this.clockViolationsListView);
			this.splitContainer1.Size = new System.Drawing.Size(605, 225);
			this.splitContainer1.SplitterDistance = 171;
			this.splitContainer1.TabIndex = 2;
			// 
			// clockViolationsListView
			// 
			this.clockViolationsListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader6,
            this.columnHeader7,
            this.columnHeader8});
			this.clockViolationsListView.ContextMenuStrip = this.contextMenuStrip1;
			this.clockViolationsListView.Dock = System.Windows.Forms.DockStyle.Fill;
			this.clockViolationsListView.FullRowSelect = true;
			this.clockViolationsListView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
			this.clockViolationsListView.Location = new System.Drawing.Point(0, 0);
			this.clockViolationsListView.Name = "clockViolationsListView";
			this.clockViolationsListView.Size = new System.Drawing.Size(605, 50);
			this.clockViolationsListView.TabIndex = 0;
			this.clockViolationsListView.UseCompatibleStateImageBehavior = false;
			this.clockViolationsListView.View = System.Windows.Forms.View.Details;
			this.clockViolationsListView.KeyDown += new System.Windows.Forms.KeyEventHandler(this.general_KeyDown);
			// 
			// columnHeader6
			// 
			this.columnHeader6.Text = "Clock Violations - Last Valid Touch Date";
			this.columnHeader6.Width = 240;
			// 
			// columnHeader7
			// 
			this.columnHeader7.Text = "System Date";
			this.columnHeader7.Width = 234;
			// 
			// columnHeader8
			// 
			this.columnHeader8.Text = "Time Off";
			this.columnHeader8.Width = 111;
			// 
			// Lic_ServerDataAttribs_DisplayForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(605, 225);
			this.Controls.Add(this.splitContainer1);
			this.DoubleBuffered = true;
			this.Name = "Lic_ServerDataAttribs_DisplayForm";
			this.ShowIcon = false;
			this.Text = "Lic_ServerDataAttribs";
			this.headerPanel.ResumeLayout(false);
			this.headerPanel.PerformLayout();
			this.contextMenuStrip1.ResumeLayout(false);
			this.splitContainer1.Panel1.ResumeLayout(false);
			this.splitContainer1.Panel2.ResumeLayout(false);
			this.splitContainer1.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Panel headerPanel;
		private System.Windows.Forms.Label lastTouchLabel;
		private System.Windows.Forms.Label versionLabel;
		private System.Windows.Forms.Label lastTouchTitleLabel;
		private System.Windows.Forms.Label versionTitleLabel;
		private Shared.VisualComponents.NoFlickerListView noFlickerListView1;
		private System.Windows.Forms.ColumnHeader columnHeader1;
		private System.Windows.Forms.ColumnHeader columnHeader2;
		private System.Windows.Forms.ColumnHeader columnHeader3;
		private System.Windows.Forms.ColumnHeader columnHeader4;
		private System.Windows.Forms.ColumnHeader columnHeader5;
		private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
		private System.Windows.Forms.ToolStripMenuItem copyToolStripMenuItem;
		private System.Windows.Forms.Label lastClockViolationLabel;
		private System.Windows.Forms.Label lastClockViolationTitleLabel;
		private System.Windows.Forms.Label clockViolCountLabel;
		private System.Windows.Forms.Label clockViolCountTitleLabel;
		private System.Windows.Forms.Label inClockViolLabel;
		private System.Windows.Forms.Label inClockViolTitleLabel;
		private System.Windows.Forms.SplitContainer splitContainer1;
		private Shared.VisualComponents.NoFlickerListView clockViolationsListView;
		private System.Windows.Forms.ColumnHeader columnHeader6;
		private System.Windows.Forms.ColumnHeader columnHeader7;
		private System.Windows.Forms.ColumnHeader columnHeader8;
	}
}