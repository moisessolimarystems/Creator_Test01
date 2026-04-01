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
			this.headerPanel = new System.Windows.Forms.Panel();
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
			this.headerPanel.SuspendLayout();
			this.SuspendLayout();
			// 
			// headerPanel
			// 
			this.headerPanel.Controls.Add(this.lastTouchLabel);
			this.headerPanel.Controls.Add(this.versionLabel);
			this.headerPanel.Controls.Add(this.lastTouchTitleLabel);
			this.headerPanel.Controls.Add(this.versionTitleLabel);
			this.headerPanel.Dock = System.Windows.Forms.DockStyle.Top;
			this.headerPanel.Location = new System.Drawing.Point(0, 0);
			this.headerPanel.Name = "headerPanel";
			this.headerPanel.Size = new System.Drawing.Size(605, 50);
			this.headerPanel.TabIndex = 0;
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
			this.noFlickerListView1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.noFlickerListView1.FullRowSelect = true;
			this.noFlickerListView1.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
			this.noFlickerListView1.Location = new System.Drawing.Point(0, 50);
			this.noFlickerListView1.Name = "noFlickerListView1";
			this.noFlickerListView1.Size = new System.Drawing.Size(605, 119);
			this.noFlickerListView1.Sorting = System.Windows.Forms.SortOrder.Ascending;
			this.noFlickerListView1.TabIndex = 1;
			this.noFlickerListView1.UseCompatibleStateImageBehavior = false;
			this.noFlickerListView1.View = System.Windows.Forms.View.Details;
			this.noFlickerListView1.ColumnClick += new System.Windows.Forms.ColumnClickEventHandler(this.noFlickerListView1_ColumnClick);
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
			// Lic_ServerDataAttribs_DisplayForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(605, 169);
			this.Controls.Add(this.noFlickerListView1);
			this.Controls.Add(this.headerPanel);
			this.DoubleBuffered = true;
			this.Name = "Lic_ServerDataAttribs_DisplayForm";
			this.ShowIcon = false;
			this.Text = "Lic_ServerDataAttribs";
			this.headerPanel.ResumeLayout(false);
			this.headerPanel.PerformLayout();
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
	}
}