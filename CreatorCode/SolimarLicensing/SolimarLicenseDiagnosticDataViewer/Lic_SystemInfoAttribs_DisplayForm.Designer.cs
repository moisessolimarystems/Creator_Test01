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
            this.outerSplitContainer = new System.Windows.Forms.SplitContainer();
            this.computerNameListView = new Shared.VisualComponents.NoFlickerListView();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.splitContainer2 = new System.Windows.Forms.SplitContainer();
            this.macAddressListView = new Shared.VisualComponents.NoFlickerListView();
            this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
            this.biosSerialNumberListView = new Shared.VisualComponents.NoFlickerListView();
            this.columnHeader3 = new System.Windows.Forms.ColumnHeader();
            this.headerPanel = new System.Windows.Forms.Panel();
            this.licenseServerVersionTitleLabel = new System.Windows.Forms.Label();
            this.licenseServerVersionLabel = new System.Windows.Forms.Label();
            this.outerSplitContainer.Panel1.SuspendLayout();
            this.outerSplitContainer.Panel2.SuspendLayout();
            this.outerSplitContainer.SuspendLayout();
            this.splitContainer2.Panel1.SuspendLayout();
            this.splitContainer2.Panel2.SuspendLayout();
            this.splitContainer2.SuspendLayout();
            this.headerPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // outerSplitContainer
            // 
            this.outerSplitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.outerSplitContainer.Location = new System.Drawing.Point(0, 31);
            this.outerSplitContainer.Name = "outerSplitContainer";
            // 
            // outerSplitContainer.Panel1
            // 
            this.outerSplitContainer.Panel1.Controls.Add(this.computerNameListView);
            // 
            // outerSplitContainer.Panel2
            // 
            this.outerSplitContainer.Panel2.Controls.Add(this.splitContainer2);
            this.outerSplitContainer.Size = new System.Drawing.Size(364, 122);
            this.outerSplitContainer.SplitterDistance = 113;
            this.outerSplitContainer.TabIndex = 0;
            // 
            // computerNameListView
            // 
            this.computerNameListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1});
            this.computerNameListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.computerNameListView.FullRowSelect = true;
            this.computerNameListView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.computerNameListView.HideSelection = false;
            this.computerNameListView.Location = new System.Drawing.Point(0, 0);
            this.computerNameListView.Name = "computerNameListView";
            this.computerNameListView.Size = new System.Drawing.Size(113, 122);
            this.computerNameListView.TabIndex = 1;
            this.computerNameListView.UseCompatibleStateImageBehavior = false;
            this.computerNameListView.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Computer Name";
            this.columnHeader1.Width = 103;
            // 
            // splitContainer2
            // 
            this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer2.Location = new System.Drawing.Point(0, 0);
            this.splitContainer2.Name = "splitContainer2";
            // 
            // splitContainer2.Panel1
            // 
            this.splitContainer2.Panel1.Controls.Add(this.macAddressListView);
            // 
            // splitContainer2.Panel2
            // 
            this.splitContainer2.Panel2.Controls.Add(this.biosSerialNumberListView);
            this.splitContainer2.Size = new System.Drawing.Size(247, 122);
            this.splitContainer2.SplitterDistance = 112;
            this.splitContainer2.TabIndex = 0;
            // 
            // macAddressListView
            // 
            this.macAddressListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader2});
            this.macAddressListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.macAddressListView.FullRowSelect = true;
            this.macAddressListView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.macAddressListView.HideSelection = false;
            this.macAddressListView.Location = new System.Drawing.Point(0, 0);
            this.macAddressListView.Name = "macAddressListView";
            this.macAddressListView.Size = new System.Drawing.Size(112, 122);
            this.macAddressListView.TabIndex = 2;
            this.macAddressListView.UseCompatibleStateImageBehavior = false;
            this.macAddressListView.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "MAC Address";
            this.columnHeader2.Width = 101;
            // 
            // biosSerialNumberListView
            // 
            this.biosSerialNumberListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader3});
            this.biosSerialNumberListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.biosSerialNumberListView.FullRowSelect = true;
            this.biosSerialNumberListView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.biosSerialNumberListView.HideSelection = false;
            this.biosSerialNumberListView.Location = new System.Drawing.Point(0, 0);
            this.biosSerialNumberListView.Name = "biosSerialNumberListView";
            this.biosSerialNumberListView.Size = new System.Drawing.Size(131, 122);
            this.biosSerialNumberListView.TabIndex = 3;
            this.biosSerialNumberListView.UseCompatibleStateImageBehavior = false;
            this.biosSerialNumberListView.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "BIOS Serial Number";
            this.columnHeader3.Width = 118;
            // 
            // headerPanel
            // 
            this.headerPanel.Controls.Add(this.licenseServerVersionLabel);
            this.headerPanel.Controls.Add(this.licenseServerVersionTitleLabel);
            this.headerPanel.Dock = System.Windows.Forms.DockStyle.Top;
            this.headerPanel.Location = new System.Drawing.Point(0, 0);
            this.headerPanel.Name = "headerPanel";
            this.headerPanel.Size = new System.Drawing.Size(364, 31);
            this.headerPanel.TabIndex = 1;
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
            // licenseServerVersionLabel
            // 
            this.licenseServerVersionLabel.AutoSize = true;
            this.licenseServerVersionLabel.Location = new System.Drawing.Point(128, 9);
            this.licenseServerVersionLabel.Name = "licenseServerVersionLabel";
            this.licenseServerVersionLabel.Size = new System.Drawing.Size(116, 13);
            this.licenseServerVersionLabel.TabIndex = 1;
            this.licenseServerVersionLabel.Text = "[LicenseServerVersion]";
            // 
            // Lic_SystemInfoAttribs_DisplayForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(364, 153);
            this.Controls.Add(this.outerSplitContainer);
            this.Controls.Add(this.headerPanel);
            this.Name = "Lic_SystemInfoAttribs_DisplayForm";
            this.ShowIcon = false;
            this.Text = "Lic_SystemInfoAttribs";
            this.outerSplitContainer.Panel1.ResumeLayout(false);
            this.outerSplitContainer.Panel2.ResumeLayout(false);
            this.outerSplitContainer.ResumeLayout(false);
            this.splitContainer2.Panel1.ResumeLayout(false);
            this.splitContainer2.Panel2.ResumeLayout(false);
            this.splitContainer2.ResumeLayout(false);
            this.headerPanel.ResumeLayout(false);
            this.headerPanel.PerformLayout();
            this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.SplitContainer outerSplitContainer;
		private System.Windows.Forms.SplitContainer splitContainer2;
		private Shared.VisualComponents.NoFlickerListView computerNameListView;
		private System.Windows.Forms.ColumnHeader columnHeader1;
		private Shared.VisualComponents.NoFlickerListView macAddressListView;
		private System.Windows.Forms.ColumnHeader columnHeader2;
		private Shared.VisualComponents.NoFlickerListView biosSerialNumberListView;
		private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.Panel headerPanel;
        private System.Windows.Forms.Label licenseServerVersionLabel;
        private System.Windows.Forms.Label licenseServerVersionTitleLabel;
	}
}