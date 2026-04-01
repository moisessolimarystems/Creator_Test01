namespace ValidationTokenRetriever
{
	partial class Form1
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
			this.menuStrip1 = new System.Windows.Forms.MenuStrip();
			this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.exportToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.generateFileForSolimarSystemsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.copyAllContentsToClipboardToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.mainButtonPanel = new System.Windows.Forms.Panel();
			this.uuidLabel = new System.Windows.Forms.Label();
			this.uuidTextBox = new System.Windows.Forms.TextBox();
			this.companyLabel = new System.Windows.Forms.Label();
			this.companyTextBox = new System.Windows.Forms.TextBox();
			this.biosLabel = new System.Windows.Forms.Label();
			this.macLabel = new System.Windows.Forms.Label();
			this.compNameLabel = new System.Windows.Forms.Label();
			this.biosTextBox = new System.Windows.Forms.TextBox();
			this.macTextBox = new System.Windows.Forms.TextBox();
			this.compNameTextBox = new System.Windows.Forms.TextBox();
			this.genButton = new System.Windows.Forms.Button();
			this.globalErrorProvider = new System.Windows.Forms.ErrorProvider(this.components);
			this.globalSaveFileDialog = new System.Windows.Forms.SaveFileDialog();
			this.bottomPanel = new System.Windows.Forms.Panel();
			this.mainBackPanel = new System.Windows.Forms.Panel();
			this.propertiesPanel = new System.Windows.Forms.Panel();
			this.propertyGrid1 = new System.Windows.Forms.PropertyGrid();
			this.menuStrip1.SuspendLayout();
			this.mainButtonPanel.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.globalErrorProvider)).BeginInit();
			this.bottomPanel.SuspendLayout();
			this.mainBackPanel.SuspendLayout();
			this.propertiesPanel.SuspendLayout();
			this.SuspendLayout();
			// 
			// menuStrip1
			// 
			this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.exportToolStripMenuItem});
			this.menuStrip1.Location = new System.Drawing.Point(0, 0);
			this.menuStrip1.Name = "menuStrip1";
			this.menuStrip1.Size = new System.Drawing.Size(519, 24);
			this.menuStrip1.TabIndex = 0;
			this.menuStrip1.Text = "menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.exitToolStripMenuItem});
			this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
			this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
			this.fileToolStripMenuItem.Text = "&File";
			// 
			// exitToolStripMenuItem
			// 
			this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
			this.exitToolStripMenuItem.Size = new System.Drawing.Size(92, 22);
			this.exitToolStripMenuItem.Text = "E&xit";
			this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
			// 
			// exportToolStripMenuItem
			// 
			this.exportToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.generateFileForSolimarSystemsToolStripMenuItem,
            this.copyAllContentsToClipboardToolStripMenuItem});
			this.exportToolStripMenuItem.Name = "exportToolStripMenuItem";
			this.exportToolStripMenuItem.Size = new System.Drawing.Size(52, 20);
			this.exportToolStripMenuItem.Text = "&Export";
			// 
			// generateFileForSolimarSystemsToolStripMenuItem
			// 
			this.generateFileForSolimarSystemsToolStripMenuItem.Name = "generateFileForSolimarSystemsToolStripMenuItem";
			this.generateFileForSolimarSystemsToolStripMenuItem.Size = new System.Drawing.Size(256, 22);
			this.generateFileForSolimarSystemsToolStripMenuItem.Text = "&Generate file for Solimar Systems...";
			this.generateFileForSolimarSystemsToolStripMenuItem.Click += new System.EventHandler(this.generateFileForSolimarSystemsToolStripMenuItem_Click);
			// 
			// copyAllContentsToClipboardToolStripMenuItem
			// 
			this.copyAllContentsToClipboardToolStripMenuItem.Name = "copyAllContentsToClipboardToolStripMenuItem";
			this.copyAllContentsToClipboardToolStripMenuItem.Size = new System.Drawing.Size(256, 22);
			this.copyAllContentsToClipboardToolStripMenuItem.Text = "&Copy all contents to clipboard";
			this.copyAllContentsToClipboardToolStripMenuItem.Click += new System.EventHandler(this.copyAllContentsToClipboardToolStripMenuItem_Click);
			// 
			// mainButtonPanel
			// 
			this.mainButtonPanel.Controls.Add(this.uuidLabel);
			this.mainButtonPanel.Controls.Add(this.uuidTextBox);
			this.mainButtonPanel.Controls.Add(this.companyLabel);
			this.mainButtonPanel.Controls.Add(this.companyTextBox);
			this.mainButtonPanel.Controls.Add(this.biosLabel);
			this.mainButtonPanel.Controls.Add(this.macLabel);
			this.mainButtonPanel.Controls.Add(this.compNameLabel);
			this.mainButtonPanel.Controls.Add(this.biosTextBox);
			this.mainButtonPanel.Controls.Add(this.macTextBox);
			this.mainButtonPanel.Controls.Add(this.compNameTextBox);
			this.mainButtonPanel.Dock = System.Windows.Forms.DockStyle.Top;
			this.mainButtonPanel.Location = new System.Drawing.Point(0, 0);
			this.mainButtonPanel.Name = "mainButtonPanel";
			this.mainButtonPanel.Size = new System.Drawing.Size(519, 33);
			this.mainButtonPanel.TabIndex = 1;
			// 
			// uuidLabel
			// 
			this.uuidLabel.AutoSize = true;
			this.uuidLabel.Location = new System.Drawing.Point(12, 100);
			this.uuidLabel.Name = "uuidLabel";
			this.uuidLabel.Size = new System.Drawing.Size(114, 13);
			this.uuidLabel.TabIndex = 12;
			this.uuidLabel.Text = "System Product UUID:";
			this.uuidLabel.Visible = false;
			// 
			// uuidTextBox
			// 
			this.uuidTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.uuidTextBox.Location = new System.Drawing.Point(146, 97);
			this.uuidTextBox.Name = "uuidTextBox";
			this.uuidTextBox.ReadOnly = true;
			this.uuidTextBox.Size = new System.Drawing.Size(361, 20);
			this.uuidTextBox.TabIndex = 11;
			this.uuidTextBox.TabStop = false;
			this.uuidTextBox.Visible = false;
			// 
			// companyLabel
			// 
			this.companyLabel.AutoSize = true;
			this.companyLabel.Location = new System.Drawing.Point(8, 10);
			this.companyLabel.Name = "companyLabel";
			this.companyLabel.Size = new System.Drawing.Size(85, 13);
			this.companyLabel.TabIndex = 10;
			this.companyLabel.Text = "Company Name:";
			// 
			// companyTextBox
			// 
			this.companyTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.companyTextBox.Location = new System.Drawing.Point(99, 7);
			this.companyTextBox.Name = "companyTextBox";
			this.companyTextBox.Size = new System.Drawing.Size(408, 20);
			this.companyTextBox.TabIndex = 0;
			this.companyTextBox.TextChanged += new System.EventHandler(this.companyTextBox_TextChanged);
			// 
			// biosLabel
			// 
			this.biosLabel.AutoSize = true;
			this.biosLabel.Location = new System.Drawing.Point(12, 127);
			this.biosLabel.Name = "biosLabel";
			this.biosLabel.Size = new System.Drawing.Size(99, 13);
			this.biosLabel.TabIndex = 6;
			this.biosLabel.Text = "Bios Serial Number:";
			this.biosLabel.Visible = false;
			// 
			// macLabel
			// 
			this.macLabel.AutoSize = true;
			this.macLabel.Location = new System.Drawing.Point(12, 74);
			this.macLabel.Name = "macLabel";
			this.macLabel.Size = new System.Drawing.Size(74, 13);
			this.macLabel.TabIndex = 5;
			this.macLabel.Text = "MAC Address:";
			this.macLabel.Visible = false;
			// 
			// compNameLabel
			// 
			this.compNameLabel.AutoSize = true;
			this.compNameLabel.Location = new System.Drawing.Point(12, 48);
			this.compNameLabel.Name = "compNameLabel";
			this.compNameLabel.Size = new System.Drawing.Size(86, 13);
			this.compNameLabel.TabIndex = 4;
			this.compNameLabel.Text = "Computer Name:";
			this.compNameLabel.Visible = false;
			// 
			// biosTextBox
			// 
			this.biosTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.biosTextBox.Location = new System.Drawing.Point(146, 124);
			this.biosTextBox.Name = "biosTextBox";
			this.biosTextBox.ReadOnly = true;
			this.biosTextBox.Size = new System.Drawing.Size(361, 20);
			this.biosTextBox.TabIndex = 3;
			this.biosTextBox.TabStop = false;
			this.biosTextBox.Visible = false;
			// 
			// macTextBox
			// 
			this.macTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.macTextBox.Location = new System.Drawing.Point(146, 71);
			this.macTextBox.Name = "macTextBox";
			this.macTextBox.ReadOnly = true;
			this.macTextBox.Size = new System.Drawing.Size(361, 20);
			this.macTextBox.TabIndex = 2;
			this.macTextBox.TabStop = false;
			this.macTextBox.Visible = false;
			// 
			// compNameTextBox
			// 
			this.compNameTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.compNameTextBox.Location = new System.Drawing.Point(146, 45);
			this.compNameTextBox.Name = "compNameTextBox";
			this.compNameTextBox.ReadOnly = true;
			this.compNameTextBox.Size = new System.Drawing.Size(361, 20);
			this.compNameTextBox.TabIndex = 1;
			this.compNameTextBox.TabStop = false;
			this.compNameTextBox.Visible = false;
			// 
			// genButton
			// 
			this.genButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.genButton.Location = new System.Drawing.Point(3, 6);
			this.genButton.Name = "genButton";
			this.genButton.Size = new System.Drawing.Size(256, 23);
			this.genButton.TabIndex = 8;
			this.genButton.Text = "Generate File for Solimar Systems...";
			this.genButton.UseVisualStyleBackColor = true;
			this.genButton.Click += new System.EventHandler(this.genButton_Click);
			// 
			// globalErrorProvider
			// 
			this.globalErrorProvider.ContainerControl = this;
			// 
			// globalSaveFileDialog
			// 
			this.globalSaveFileDialog.FileName = "solimarValidationTokens";
			this.globalSaveFileDialog.Filter = "CSV files|*.csv";
			// 
			// bottomPanel
			// 
			this.bottomPanel.Controls.Add(this.genButton);
			this.bottomPanel.Dock = System.Windows.Forms.DockStyle.Bottom;
			this.bottomPanel.Location = new System.Drawing.Point(0, 207);
			this.bottomPanel.Name = "bottomPanel";
			this.bottomPanel.Size = new System.Drawing.Size(519, 38);
			this.bottomPanel.TabIndex = 13;
			// 
			// mainBackPanel
			// 
			this.mainBackPanel.Controls.Add(this.propertiesPanel);
			this.mainBackPanel.Controls.Add(this.bottomPanel);
			this.mainBackPanel.Dock = System.Windows.Forms.DockStyle.Fill;
			this.mainBackPanel.Location = new System.Drawing.Point(0, 24);
			this.mainBackPanel.Name = "mainBackPanel";
			this.mainBackPanel.Size = new System.Drawing.Size(519, 245);
			this.mainBackPanel.TabIndex = 13;
			// 
			// propertiesPanel
			// 
			this.propertiesPanel.Controls.Add(this.propertyGrid1);
			this.propertiesPanel.Controls.Add(this.mainButtonPanel);
			this.propertiesPanel.Dock = System.Windows.Forms.DockStyle.Fill;
			this.propertiesPanel.Location = new System.Drawing.Point(0, 0);
			this.propertiesPanel.Name = "propertiesPanel";
			this.propertiesPanel.Size = new System.Drawing.Size(519, 207);
			this.propertiesPanel.TabIndex = 14;
			// 
			// propertyGrid1
			// 
			this.propertyGrid1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.propertyGrid1.HelpVisible = false;
			this.propertyGrid1.Location = new System.Drawing.Point(0, 33);
			this.propertyGrid1.Name = "propertyGrid1";
			this.propertyGrid1.PropertySort = System.Windows.Forms.PropertySort.NoSort;
			this.propertyGrid1.Size = new System.Drawing.Size(519, 174);
			this.propertyGrid1.TabIndex = 2;
			this.propertyGrid1.ToolbarVisible = false;
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(519, 269);
			this.Controls.Add(this.mainBackPanel);
			this.Controls.Add(this.menuStrip1);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MainMenuStrip = this.menuStrip1;
			this.Name = "Form1";
			this.Text = "Solimar Validation Token Retriever";
			this.menuStrip1.ResumeLayout(false);
			this.menuStrip1.PerformLayout();
			this.mainButtonPanel.ResumeLayout(false);
			this.mainButtonPanel.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.globalErrorProvider)).EndInit();
			this.bottomPanel.ResumeLayout(false);
			this.mainBackPanel.ResumeLayout(false);
			this.propertiesPanel.ResumeLayout(false);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.MenuStrip menuStrip1;
		private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
		private System.Windows.Forms.Panel mainButtonPanel;
		private System.Windows.Forms.Label compNameLabel;
		private System.Windows.Forms.TextBox biosTextBox;
		private System.Windows.Forms.TextBox macTextBox;
		private System.Windows.Forms.TextBox compNameTextBox;
		private System.Windows.Forms.Label biosLabel;
		private System.Windows.Forms.Label macLabel;
		private System.Windows.Forms.Button genButton;
		private System.Windows.Forms.ErrorProvider globalErrorProvider;
		private System.Windows.Forms.SaveFileDialog globalSaveFileDialog;
		private System.Windows.Forms.Label companyLabel;
		private System.Windows.Forms.TextBox companyTextBox;
		private System.Windows.Forms.Label uuidLabel;
		private System.Windows.Forms.TextBox uuidTextBox;
		private System.Windows.Forms.ToolStripMenuItem exportToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem generateFileForSolimarSystemsToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem copyAllContentsToClipboardToolStripMenuItem;
		private System.Windows.Forms.Panel bottomPanel;
		private System.Windows.Forms.Panel mainBackPanel;
		private System.Windows.Forms.Panel propertiesPanel;
		private System.Windows.Forms.PropertyGrid propertyGrid1;
	}
}

