namespace SolimarLicenseDiagnosticDataViewer
{
	partial class Lic_SystemInfoAttribs_DisplayForm
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
			this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
			this.copyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.headerPanel = new System.Windows.Forms.Panel();
			this.licenseServerVersionLabel = new System.Windows.Forms.Label();
			this.licenseServerVersionTitleLabel = new System.Windows.Forms.Label();
			this.valTokenListView = new Shared.VisualComponents.NoFlickerListView();
			this.columnHeader4 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader5 = new System.Windows.Forms.ColumnHeader();
			this.contextMenuStrip1.SuspendLayout();
			this.headerPanel.SuspendLayout();
			this.SuspendLayout();
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
			// headerPanel
			// 
			this.headerPanel.ContextMenuStrip = this.contextMenuStrip1;
			this.headerPanel.Controls.Add(this.licenseServerVersionLabel);
			this.headerPanel.Controls.Add(this.licenseServerVersionTitleLabel);
			this.headerPanel.Dock = System.Windows.Forms.DockStyle.Top;
			this.headerPanel.Location = new System.Drawing.Point(0, 0);
			this.headerPanel.Name = "headerPanel";
			this.headerPanel.Size = new System.Drawing.Size(364, 31);
			this.headerPanel.TabIndex = 1;
			// 
			// licenseServerVersionLabel
			// 
			this.licenseServerVersionLabel.AutoSize = true;
			this.licenseServerVersionLabel.Location = new System.Drawing.Point(128, 9);
			this.licenseServerVersionLabel.Name = "licenseServerVersionLabel";
			this.licenseServerVersionLabel.Size = new System.Drawing.Size(116, 13);
			this.licenseServerVersionLabel.TabIndex = 1;
			this.licenseServerVersionLabel.Text = "[LicenseServerVersion]";
			// 
			// licenseServerVersionTitleLabel
			// 
			this.licenseServerVersionTitleLabel.AutoSize = true;
			this.licenseServerVersionTitleLabel.Location = new System.Drawing.Point(3, 9);
			this.licenseServerVersionTitleLabel.Name = "licenseServerVersionTitleLabel";
			this.licenseServerVersionTitleLabel.Size = new System.Drawing.Size(119, 13);
			this.licenseServerVersionTitleLabel.TabIndex = 0;
			this.licenseServerVersionTitleLabel.Text = "License Server Version:";
			// 
			// valTokenListView
			// 
			this.valTokenListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader4,
            this.columnHeader5});
			this.valTokenListView.ContextMenuStrip = this.contextMenuStrip1;
			this.valTokenListView.Dock = System.Windows.Forms.DockStyle.Fill;
			this.valTokenListView.FullRowSelect = true;
			this.valTokenListView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
			this.valTokenListView.HideSelection = false;
			this.valTokenListView.Location = new System.Drawing.Point(0, 31);
			this.valTokenListView.Name = "valTokenListView";
			this.valTokenListView.Size = new System.Drawing.Size(364, 243);
			this.valTokenListView.Sorting = System.Windows.Forms.SortOrder.Ascending;
			this.valTokenListView.TabIndex = 2;
			this.valTokenListView.UseCompatibleStateImageBehavior = false;
			this.valTokenListView.View = System.Windows.Forms.View.Details;
			this.valTokenListView.KeyDown += new System.Windows.Forms.KeyEventHandler(this.general_KeyDown);
			// 
			// columnHeader4
			// 
			this.columnHeader4.Text = "Validation Token";
			this.columnHeader4.Width = 157;
			// 
			// columnHeader5
			// 
			this.columnHeader5.Text = "Validation Value";
			this.columnHeader5.Width = 194;
			// 
			// Lic_SystemInfoAttribs_DisplayForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(364, 274);
			this.Controls.Add(this.valTokenListView);
			this.Controls.Add(this.headerPanel);
			this.Name = "Lic_SystemInfoAttribs_DisplayForm";
			this.ShowIcon = false;
			this.Text = "Lic_SystemInfoAttribs";
			this.contextMenuStrip1.ResumeLayout(false);
			this.headerPanel.ResumeLayout(false);
			this.headerPanel.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Panel headerPanel;
		private System.Windows.Forms.Label licenseServerVersionLabel;
		private System.Windows.Forms.Label licenseServerVersionTitleLabel;
		private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
		private System.Windows.Forms.ToolStripMenuItem copyToolStripMenuItem;
		private Shared.VisualComponents.NoFlickerListView valTokenListView;
		private System.Windows.Forms.ColumnHeader columnHeader4;
		private System.Windows.Forms.ColumnHeader columnHeader5;
	}
}