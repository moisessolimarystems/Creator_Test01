namespace SolimarLicenseDiagnosticDataViewer
{
	partial class Lic_KeyAttribs_DisplayForm
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
            this.displayButton = new System.Windows.Forms.Button();
            this.keyListView = new System.Windows.Forms.ListView();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader3 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader4 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader5 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader6 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader7 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader8 = new System.Windows.Forms.ColumnHeader();
            this.activitySlotPanel = new System.Windows.Forms.Panel();
            this.lic_KeyAttribs_DisplayControl = new SolimarLicenseDiagnosticDataViewer.Lic_KeyAttribs_DisplayControl();
            this.contextMenuStrip1.SuspendLayout();
            this.activitySlotPanel.SuspendLayout();
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
            // displayButton
            // 
            this.displayButton.Location = new System.Drawing.Point(3, 143);
            this.displayButton.Name = "displayButton";
            this.displayButton.Size = new System.Drawing.Size(120, 23);
            this.displayButton.TabIndex = 9;
            this.displayButton.Tag = "0";
            this.displayButton.Text = "Show Key Cells";
            this.displayButton.UseVisualStyleBackColor = true;
            this.displayButton.Click += new System.EventHandler(this.displayButton_Click);
            // 
            // keyListView
            // 
            this.keyListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3,
            this.columnHeader4,
            this.columnHeader5,
            this.columnHeader6,
            this.columnHeader7,
            this.columnHeader8});
            this.keyListView.ContextMenuStrip = this.contextMenuStrip1;
            this.keyListView.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.keyListView.FullRowSelect = true;
            this.keyListView.GridLines = true;
            this.keyListView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            this.keyListView.HideSelection = false;
            this.keyListView.Location = new System.Drawing.Point(0, 169);
            this.keyListView.Name = "keyListView";
            this.keyListView.Size = new System.Drawing.Size(537, 150);
            this.keyListView.TabIndex = 1;
            this.keyListView.UseCompatibleStateImageBehavior = false;
            this.keyListView.View = System.Windows.Forms.View.Details;
            this.keyListView.KeyDown += new System.Windows.Forms.KeyEventHandler(this.general_KeyDown);
            // 
            // activitySlotPanel
            // 
            this.activitySlotPanel.Controls.Add(this.displayButton);
            this.activitySlotPanel.Controls.Add(this.lic_KeyAttribs_DisplayControl);
            this.activitySlotPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.activitySlotPanel.Location = new System.Drawing.Point(0, 0);
            this.activitySlotPanel.Name = "activitySlotPanel";
            this.activitySlotPanel.Size = new System.Drawing.Size(537, 169);
            this.activitySlotPanel.TabIndex = 2;
            // 
            // lic_KeyAttribs_DisplayControl
            // 
            this.lic_KeyAttribs_DisplayControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lic_KeyAttribs_DisplayControl.Location = new System.Drawing.Point(0, 0);
            this.lic_KeyAttribs_DisplayControl.Name = "lic_KeyAttribs_DisplayControl";
            this.lic_KeyAttribs_DisplayControl.Size = new System.Drawing.Size(537, 169);
            this.lic_KeyAttribs_DisplayControl.TabIndex = 1;
            // 
            // Lic_KeyAttribs_DisplayForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(537, 319);
            this.Controls.Add(this.activitySlotPanel);
            this.Controls.Add(this.keyListView);
            this.DoubleBuffered = true;
            this.Name = "Lic_KeyAttribs_DisplayForm";
            this.ShowIcon = false;
            this.Text = "Lic_KeyAttribs";
            this.contextMenuStrip1.ResumeLayout(false);
            this.activitySlotPanel.ResumeLayout(false);
            this.ResumeLayout(false);

		}

		#endregion

        private System.Windows.Forms.ListView keyListView;
		private System.Windows.Forms.ColumnHeader columnHeader1;
		private System.Windows.Forms.ColumnHeader columnHeader2;
		private System.Windows.Forms.ColumnHeader columnHeader3;
		private System.Windows.Forms.ColumnHeader columnHeader4;
		private System.Windows.Forms.ColumnHeader columnHeader5;
		private System.Windows.Forms.ColumnHeader columnHeader6;
		private System.Windows.Forms.ColumnHeader columnHeader7;
        private System.Windows.Forms.ColumnHeader columnHeader8;
        private System.Windows.Forms.Button displayButton;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
        private System.Windows.Forms.ToolStripMenuItem copyToolStripMenuItem;
        private System.Windows.Forms.Panel activitySlotPanel;
        private Lic_KeyAttribs_DisplayControl lic_KeyAttribs_DisplayControl;
	}
}