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
			this.mainBackPanel = new System.Windows.Forms.Panel();
			this.uuidLabel = new System.Windows.Forms.Label();
			this.uuidTextBox = new System.Windows.Forms.TextBox();
			this.companyLabel = new System.Windows.Forms.Label();
			this.companyTextBox = new System.Windows.Forms.TextBox();
			this.genButton = new System.Windows.Forms.Button();
			this.biosLabel = new System.Windows.Forms.Label();
			this.macLabel = new System.Windows.Forms.Label();
			this.compNameLabel = new System.Windows.Forms.Label();
			this.biosTextBox = new System.Windows.Forms.TextBox();
			this.macTextBox = new System.Windows.Forms.TextBox();
			this.compNameTextBox = new System.Windows.Forms.TextBox();
			this.globalErrorProvider = new System.Windows.Forms.ErrorProvider(this.components);
			this.globalSaveFileDialog = new System.Windows.Forms.SaveFileDialog();
			this.menuStrip1.SuspendLayout();
			this.mainBackPanel.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.globalErrorProvider)).BeginInit();
			this.SuspendLayout();
			// 
			// menuStrip1
			// 
			this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.exportToolStripMenuItem});
			this.menuStrip1.Location = new System.Drawing.Point(0, 0);
			this.menuStrip1.Name = "menuStrip1";
			this.menuStrip1.Size = new System.Drawing.Size(570, 24);
			this.menuStrip1.TabIndex = 0;
			this.menuStrip1.Text = "menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.exitToolStripMenuItem});
			this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
			this.fileToolStripMenuItem.Size = new System.Drawing.Size(35, 20);
			this.fileToolStripMenuItem.Text = "&File";
			// 
			// exitToolStripMenuItem
			// 
			this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
			this.exitToolStripMenuItem.Size = new System.Drawing.Size(103, 22);
			this.exitToolStripMenuItem.Text = "E&xit";
			this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
			// 
			// exportToolStripMenuItem
			// 
			this.exportToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.generateFileForSolimarSystemsToolStripMenuItem,
            this.copyAllContentsToClipboardToolStripMenuItem});
			this.exportToolStripMenuItem.Name = "exportToolStripMenuItem";
			this.exportToolStripMenuItem.Size = new System.Drawing.Size(51, 20);
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
			// mainBackPanel
			// 
			this.mainBackPanel.Controls.Add(this.uuidLabel);
			this.mainBackPanel.Controls.Add(this.uuidTextBox);
			this.mainBackPanel.Controls.Add(this.companyLabel);
			this.mainBackPanel.Controls.Add(this.companyTextBox);
			this.mainBackPanel.Controls.Add(this.genButton);
			this.mainBackPanel.Controls.Add(this.biosLabel);
			this.mainBackPanel.Controls.Add(this.macLabel);
			this.mainBackPanel.Controls.Add(this.compNameLabel);
			this.mainBackPanel.Controls.Add(this.biosTextBox);
			this.mainBackPanel.Controls.Add(this.macTextBox);
			this.mainBackPanel.Controls.Add(this.compNameTextBox);
			this.mainBackPanel.Dock = System.Windows.Forms.DockStyle.Fill;
			this.mainBackPanel.Location = new System.Drawing.Point(0, 24);
			this.mainBackPanel.Name = "mainBackPanel";
			this.mainBackPanel.Size = new System.Drawing.Size(570, 188);
			this.mainBackPanel.TabIndex = 1;
			// 
			// uuidLabel
			// 
			this.uuidLabel.AutoSize = true;
			this.uuidLabel.Location = new System.Drawing.Point(12, 100);
			this.uuidLabel.Name = "uuidLabel";
			this.uuidLabel.Size = new System.Drawing.Size(114, 13);
			this.uuidLabel.TabIndex = 12;
			this.uuidLabel.Text = "System Product UUID:";
			// 
			// uuidTextBox
			// 
			this.uuidTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.uuidTextBox.Location = new System.Drawing.Point(146, 97);
			this.uuidTextBox.Name = "uuidTextBox";
			this.uuidTextBox.ReadOnly = true;
			this.uuidTextBox.Size = new System.Drawing.Size(412, 20);
			this.uuidTextBox.TabIndex = 11;
			this.uuidTextBox.TabStop = false;
			// 
			// companyLabel
			// 
			this.companyLabel.AutoSize = true;
			this.companyLabel.Location = new System.Drawing.Point(12, 23);
			this.companyLabel.Name = "companyLabel";
			this.companyLabel.Size = new System.Drawing.Size(85, 13);
			this.companyLabel.TabIndex = 10;
			this.companyLabel.Text = "Company Name:";
			// 
			// companyTextBox
			// 
			this.companyTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.companyTextBox.Location = new System.Drawing.Point(146, 20);
			this.companyTextBox.Name = "companyTextBox";
			this.companyTextBox.Size = new System.Drawing.Size(412, 20);
			this.companyTextBox.TabIndex = 0;
			// 
			// genButton
			// 
			this.genButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.genButton.Location = new System.Drawing.Point(15, 157);
			this.genButton.Name = "genButton";
			this.genButton.Size = new System.Drawing.Size(256, 23);
			this.genButton.TabIndex = 8;
			this.genButton.Text = "Generate File for Solimar Systems...";
			this.genButton.UseVisualStyleBackColor = true;
			this.genButton.Click += new System.EventHandler(this.genButton_Click);
			// 
			// biosLabel
			// 
			this.biosLabel.AutoSize = true;
			this.biosLabel.Location = new System.Drawing.Point(12, 127);
			this.biosLabel.Name = "biosLabel";
			this.biosLabel.Size = new System.Drawing.Size(99, 13);
			this.biosLabel.TabIndex = 6;
			this.biosLabel.Text = "Bios Serial Number:";
			// 
			// macLabel
			// 
			this.macLabel.AutoSize = true;
			this.macLabel.Location = new System.Drawing.Point(12, 74);
			this.macLabel.Name = "macLabel";
			this.macLabel.Size = new System.Drawing.Size(74, 13);
			this.macLabel.TabIndex = 5;
			this.macLabel.Text = "MAC Address:";
			// 
			// compNameLabel
			// 
			this.compNameLabel.AutoSize = true;
			this.compNameLabel.Location = new System.Drawing.Point(12, 48);
			this.compNameLabel.Name = "compNameLabel";
			this.compNameLabel.Size = new System.Drawing.Size(86, 13);
			this.compNameLabel.TabIndex = 4;
			this.compNameLabel.Text = "Computer Name:";
			// 
			// biosTextBox
			// 
			this.biosTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.biosTextBox.Location = new System.Drawing.Point(146, 124);
			this.biosTextBox.Name = "biosTextBox";
			this.biosTextBox.ReadOnly = true;
			this.biosTextBox.Size = new System.Drawing.Size(412, 20);
			this.biosTextBox.TabIndex = 3;
			this.biosTextBox.TabStop = false;
			// 
			// macTextBox
			// 
			this.macTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.macTextBox.Location = new System.Drawing.Point(146, 71);
			this.macTextBox.Name = "macTextBox";
			this.macTextBox.ReadOnly = true;
			this.macTextBox.Size = new System.Drawing.Size(412, 20);
			this.macTextBox.TabIndex = 2;
			this.macTextBox.TabStop = false;
			// 
			// compNameTextBox
			// 
			this.compNameTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.compNameTextBox.Location = new System.Drawing.Point(146, 45);
			this.compNameTextBox.Name = "compNameTextBox";
			this.compNameTextBox.ReadOnly = true;
			this.compNameTextBox.Size = new System.Drawing.Size(412, 20);
			this.compNameTextBox.TabIndex = 1;
			this.compNameTextBox.TabStop = false;
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
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(570, 212);
			this.Controls.Add(this.mainBackPanel);
			this.Controls.Add(this.menuStrip1);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MainMenuStrip = this.menuStrip1;
			this.Name = "Form1";
			this.Text = "Solimar Validation Token Retriever";
			this.menuStrip1.ResumeLayout(false);
			this.menuStrip1.PerformLayout();
			this.mainBackPanel.ResumeLayout(false);
			this.mainBackPanel.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.globalErrorProvider)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.MenuStrip menuStrip1;
		private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
		private System.Windows.Forms.Panel mainBackPanel;
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
	}
}

