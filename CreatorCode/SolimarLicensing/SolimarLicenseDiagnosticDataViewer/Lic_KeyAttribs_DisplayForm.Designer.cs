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
            this.headerPanel = new System.Windows.Forms.Panel();
            this.displayButton = new System.Windows.Forms.Button();
            this.currentActivationsLabel = new System.Windows.Forms.Label();
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.copyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.modifiedDateLabel = new System.Windows.Forms.Label();
            this.verificationCodeLabel = new System.Windows.Forms.Label();
            this.keyNameLabel = new System.Windows.Forms.Label();
            this.currentActivationsTitleLabel = new System.Windows.Forms.Label();
            this.modifiedDateTitlelabel = new System.Windows.Forms.Label();
            this.verificationCodeTitleLabel = new System.Windows.Forms.Label();
            this.keyNameTitleLabel = new System.Windows.Forms.Label();
            this.keyListView = new System.Windows.Forms.ListView();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader3 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader4 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader5 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader6 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader7 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader8 = new System.Windows.Forms.ColumnHeader();
            this.headerPanel.SuspendLayout();
            this.contextMenuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // headerPanel
            // 
            this.headerPanel.ContextMenuStrip = this.contextMenuStrip1;
            this.headerPanel.Controls.Add(this.displayButton);
            this.headerPanel.Controls.Add(this.currentActivationsLabel);
            this.headerPanel.Controls.Add(this.modifiedDateLabel);
            this.headerPanel.Controls.Add(this.verificationCodeLabel);
            this.headerPanel.Controls.Add(this.keyNameLabel);
            this.headerPanel.Controls.Add(this.currentActivationsTitleLabel);
            this.headerPanel.Controls.Add(this.modifiedDateTitlelabel);
            this.headerPanel.Controls.Add(this.verificationCodeTitleLabel);
            this.headerPanel.Controls.Add(this.keyNameTitleLabel);
            this.headerPanel.Dock = System.Windows.Forms.DockStyle.Top;
            this.headerPanel.Location = new System.Drawing.Point(0, 0);
            this.headerPanel.Name = "headerPanel";
            this.headerPanel.Size = new System.Drawing.Size(487, 73);
            this.headerPanel.TabIndex = 0;
            // 
            // displayButton
            // 
            this.displayButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.displayButton.Location = new System.Drawing.Point(364, 43);
            this.displayButton.Name = "displayButton";
            this.displayButton.Size = new System.Drawing.Size(120, 23);
            this.displayButton.TabIndex = 9;
            this.displayButton.Tag = "0";
            this.displayButton.Text = "Show Key Cells";
            this.displayButton.UseVisualStyleBackColor = true;
            this.displayButton.Click += new System.EventHandler(this.displayButton_Click);
            // 
            // currentActivationsLabel
            // 
            this.currentActivationsLabel.AutoSize = true;
            this.currentActivationsLabel.Location = new System.Drawing.Point(105, 48);
            this.currentActivationsLabel.Name = "currentActivationsLabel";
            this.currentActivationsLabel.Size = new System.Drawing.Size(99, 13);
            this.currentActivationsLabel.TabIndex = 7;
            this.currentActivationsLabel.Text = "[CurrentActivations]";
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
            // modifiedDateLabel
            // 
            this.modifiedDateLabel.AutoSize = true;
            this.modifiedDateLabel.Location = new System.Drawing.Point(105, 35);
            this.modifiedDateLabel.Name = "modifiedDateLabel";
            this.modifiedDateLabel.Size = new System.Drawing.Size(76, 13);
            this.modifiedDateLabel.TabIndex = 6;
            this.modifiedDateLabel.Text = "[ModifiedDate]";
            // 
            // verificationCodeLabel
            // 
            this.verificationCodeLabel.AutoSize = true;
            this.verificationCodeLabel.Location = new System.Drawing.Point(105, 22);
            this.verificationCodeLabel.Name = "verificationCodeLabel";
            this.verificationCodeLabel.Size = new System.Drawing.Size(90, 13);
            this.verificationCodeLabel.TabIndex = 5;
            this.verificationCodeLabel.Text = "[VerificationCode]";
            // 
            // keyNameLabel
            // 
            this.keyNameLabel.AutoSize = true;
            this.keyNameLabel.Location = new System.Drawing.Point(105, 9);
            this.keyNameLabel.Name = "keyNameLabel";
            this.keyNameLabel.Size = new System.Drawing.Size(59, 13);
            this.keyNameLabel.TabIndex = 4;
            this.keyNameLabel.Text = "[KeyName]";
            // 
            // currentActivationsTitleLabel
            // 
            this.currentActivationsTitleLabel.AutoSize = true;
            this.currentActivationsTitleLabel.Location = new System.Drawing.Point(3, 48);
            this.currentActivationsTitleLabel.Name = "currentActivationsTitleLabel";
            this.currentActivationsTitleLabel.Size = new System.Drawing.Size(99, 13);
            this.currentActivationsTitleLabel.TabIndex = 3;
            this.currentActivationsTitleLabel.Text = "Current Activations:";
            // 
            // modifiedDateTitlelabel
            // 
            this.modifiedDateTitlelabel.AutoSize = true;
            this.modifiedDateTitlelabel.Location = new System.Drawing.Point(3, 35);
            this.modifiedDateTitlelabel.Name = "modifiedDateTitlelabel";
            this.modifiedDateTitlelabel.Size = new System.Drawing.Size(76, 13);
            this.modifiedDateTitlelabel.TabIndex = 2;
            this.modifiedDateTitlelabel.Text = "Modified Date:";
            // 
            // verificationCodeTitleLabel
            // 
            this.verificationCodeTitleLabel.AutoSize = true;
            this.verificationCodeTitleLabel.Location = new System.Drawing.Point(3, 22);
            this.verificationCodeTitleLabel.Name = "verificationCodeTitleLabel";
            this.verificationCodeTitleLabel.Size = new System.Drawing.Size(90, 13);
            this.verificationCodeTitleLabel.TabIndex = 1;
            this.verificationCodeTitleLabel.Text = "Verification Code:";
            // 
            // keyNameTitleLabel
            // 
            this.keyNameTitleLabel.AutoSize = true;
            this.keyNameTitleLabel.Location = new System.Drawing.Point(3, 9);
            this.keyNameTitleLabel.Name = "keyNameTitleLabel";
            this.keyNameTitleLabel.Size = new System.Drawing.Size(59, 13);
            this.keyNameTitleLabel.TabIndex = 0;
            this.keyNameTitleLabel.Text = "Key Name:";
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
            this.keyListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.keyListView.FullRowSelect = true;
            this.keyListView.GridLines = true;
            this.keyListView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            this.keyListView.Location = new System.Drawing.Point(0, 73);
            this.keyListView.Name = "keyListView";
            this.keyListView.Size = new System.Drawing.Size(487, 182);
            this.keyListView.TabIndex = 1;
            this.keyListView.UseCompatibleStateImageBehavior = false;
            this.keyListView.View = System.Windows.Forms.View.Details;
            this.keyListView.KeyDown += new System.Windows.Forms.KeyEventHandler(this.general_KeyDown);
            // 
            // Lic_KeyAttribs_DisplayForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(487, 255);
            this.Controls.Add(this.keyListView);
            this.Controls.Add(this.headerPanel);
            this.DoubleBuffered = true;
            this.Name = "Lic_KeyAttribs_DisplayForm";
            this.ShowIcon = false;
            this.Text = "Lic_KeyAttribs";
            this.headerPanel.ResumeLayout(false);
            this.headerPanel.PerformLayout();
            this.contextMenuStrip1.ResumeLayout(false);
            this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Panel headerPanel;
		private System.Windows.Forms.Label verificationCodeTitleLabel;
		private System.Windows.Forms.Label keyNameTitleLabel;
		private System.Windows.Forms.Label modifiedDateTitlelabel;
		private System.Windows.Forms.Label currentActivationsLabel;
		private System.Windows.Forms.Label modifiedDateLabel;
		private System.Windows.Forms.Label verificationCodeLabel;
		private System.Windows.Forms.Label keyNameLabel;
		private System.Windows.Forms.Label currentActivationsTitleLabel;
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
	}
}