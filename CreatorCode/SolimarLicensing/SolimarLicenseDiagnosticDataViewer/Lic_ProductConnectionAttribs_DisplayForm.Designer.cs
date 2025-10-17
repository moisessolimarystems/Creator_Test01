namespace SolimarLicenseDiagnosticDataViewer
{
	partial class Lic_ProductConnectionAttribs_DisplayForm
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
         this.statusStrip1 = new System.Windows.Forms.StatusStrip();
         this.productsTSStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
         this.installedTSStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
         this.backpanel = new System.Windows.Forms.Panel();
         this.productListView = new Shared.VisualComponents.NoFlickerListView();
         this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
         this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
         this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
         this.columnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
         this.columnHeader5 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
         this.columnHeader6 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
         this.toolStrip1 = new System.Windows.Forms.ToolStrip();
         this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
         this.generalToolTip = new System.Windows.Forms.ToolTip(this.components);
         this.statusStrip1.SuspendLayout();
         this.backpanel.SuspendLayout();
         this.toolStrip1.SuspendLayout();
         this.SuspendLayout();
         // 
         // statusStrip1
         // 
         this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.productsTSStatusLabel,
            this.installedTSStatusLabel});
         this.statusStrip1.Location = new System.Drawing.Point(0, 312);
         this.statusStrip1.Name = "statusStrip1";
         this.statusStrip1.Size = new System.Drawing.Size(790, 24);
         this.statusStrip1.TabIndex = 3;
         this.statusStrip1.Text = "statusStrip1";
         // 
         // productsTSStatusLabel
         // 
         this.productsTSStatusLabel.Name = "productsTSStatusLabel";
         this.productsTSStatusLabel.Size = new System.Drawing.Size(62, 19);
         this.productsTSStatusLabel.Text = "[Products]";
         // 
         // installedTSStatusLabel
         // 
         this.installedTSStatusLabel.BorderSides = System.Windows.Forms.ToolStripStatusLabelBorderSides.Left;
         this.installedTSStatusLabel.Name = "installedTSStatusLabel";
         this.installedTSStatusLabel.Size = new System.Drawing.Size(63, 19);
         this.installedTSStatusLabel.Text = "[Installed]";
         // 
         // backpanel
         // 
         this.backpanel.Controls.Add(this.productListView);
         this.backpanel.Controls.Add(this.toolStrip1);
         this.backpanel.Dock = System.Windows.Forms.DockStyle.Fill;
         this.backpanel.Location = new System.Drawing.Point(0, 0);
         this.backpanel.Name = "backpanel";
         this.backpanel.Size = new System.Drawing.Size(790, 312);
         this.backpanel.TabIndex = 4;
         // 
         // productListView
         // 
         this.productListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3,
            this.columnHeader4,
            this.columnHeader5,
            this.columnHeader6});
         this.productListView.Dock = System.Windows.Forms.DockStyle.Fill;
         this.productListView.FullRowSelect = true;
         this.productListView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
         this.productListView.Location = new System.Drawing.Point(0, 25);
         this.productListView.Name = "productListView";
         this.productListView.ShowItemToolTips = true;
         this.productListView.Size = new System.Drawing.Size(790, 287);
         this.productListView.TabIndex = 0;
         this.productListView.UseCompatibleStateImageBehavior = false;
         this.productListView.View = System.Windows.Forms.View.Details;
         // 
         // columnHeader1
         // 
         this.columnHeader1.Text = "Product";
         this.columnHeader1.Width = 160;
         // 
         // columnHeader2
         // 
         this.columnHeader2.Text = "Primary License Server";
         this.columnHeader2.Width = 127;
         // 
         // columnHeader3
         // 
         this.columnHeader3.Text = "Fail Over Server";
         this.columnHeader3.Width = 96;
         // 
         // columnHeader4
         // 
         this.columnHeader4.Text = "Use Test/Dev/DR Licensing";
         this.columnHeader4.Width = 159;
         // 
         // columnHeader5
         // 
         this.columnHeader5.Text = "Status";
         this.columnHeader5.Width = 103;
         // 
         // columnHeader6
         // 
         this.columnHeader6.Text = "Installed";
         this.columnHeader6.Width = 112;
         // 
         // toolStrip1
         // 
         this.toolStrip1.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
         this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripLabel1});
         this.toolStrip1.Location = new System.Drawing.Point(0, 0);
         this.toolStrip1.Name = "toolStrip1";
         this.toolStrip1.Size = new System.Drawing.Size(790, 25);
         this.toolStrip1.TabIndex = 1;
         this.toolStrip1.Text = "toolStrip1";
         // 
         // toolStripLabel1
         // 
         this.toolStripLabel1.Name = "toolStripLabel1";
         this.toolStripLabel1.Size = new System.Drawing.Size(162, 22);
         this.toolStripLabel1.Text = "Product Connection Settings:";
         // 
         // Lic_ProductConnectionAttribs_DisplayForm
         // 
         this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
         this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
         this.ClientSize = new System.Drawing.Size(790, 336);
         this.Controls.Add(this.backpanel);
         this.Controls.Add(this.statusStrip1);
         this.Name = "Lic_ProductConnectionAttribs_DisplayForm";
         this.Text = "Lic_ProductConnectionAttribs_DisplayForm";
         this.statusStrip1.ResumeLayout(false);
         this.statusStrip1.PerformLayout();
         this.backpanel.ResumeLayout(false);
         this.backpanel.PerformLayout();
         this.toolStrip1.ResumeLayout(false);
         this.toolStrip1.PerformLayout();
         this.ResumeLayout(false);
         this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.StatusStrip statusStrip1;
		private System.Windows.Forms.ToolStripStatusLabel productsTSStatusLabel;
		private System.Windows.Forms.Panel backpanel;
		private Shared.VisualComponents.NoFlickerListView productListView;
		private System.Windows.Forms.ColumnHeader columnHeader1;
		private System.Windows.Forms.ColumnHeader columnHeader2;
		private System.Windows.Forms.ColumnHeader columnHeader3;
		private System.Windows.Forms.ColumnHeader columnHeader4;
		private System.Windows.Forms.ColumnHeader columnHeader5;
		private System.Windows.Forms.ToolStrip toolStrip1;
		private System.Windows.Forms.ToolStripLabel toolStripLabel1;
		private System.Windows.Forms.ColumnHeader columnHeader6;
      private System.Windows.Forms.ToolStripStatusLabel installedTSStatusLabel;
      private System.Windows.Forms.ToolTip generalToolTip;
	}
}