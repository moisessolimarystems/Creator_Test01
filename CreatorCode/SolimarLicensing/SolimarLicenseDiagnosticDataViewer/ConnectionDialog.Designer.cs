namespace SolimarLicenseDiagnosticDataViewer
{
	partial class ConnectionDialog
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
            this.buttonPanel = new System.Windows.Forms.Panel();
            this.connectButton = new System.Windows.Forms.Button();
            this.cancelButton = new System.Windows.Forms.Button();
            this.licenseServerTitleLabel = new System.Windows.Forms.Label();
            this.licenseServerComboBox = new System.Windows.Forms.ComboBox();
            this.buttonPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // buttonPanel
            // 
            this.buttonPanel.Controls.Add(this.connectButton);
            this.buttonPanel.Controls.Add(this.cancelButton);
            this.buttonPanel.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.buttonPanel.Location = new System.Drawing.Point(0, 68);
            this.buttonPanel.Name = "buttonPanel";
            this.buttonPanel.Size = new System.Drawing.Size(318, 39);
            this.buttonPanel.TabIndex = 0;
            // 
            // connectButton
            // 
            this.connectButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.connectButton.Location = new System.Drawing.Point(159, 8);
            this.connectButton.Name = "connectButton";
            this.connectButton.Size = new System.Drawing.Size(75, 23);
            this.connectButton.TabIndex = 1;
            this.connectButton.Text = "Connect";
            this.connectButton.UseVisualStyleBackColor = true;
            this.connectButton.Click += new System.EventHandler(this.connectButton_Click);
            this.connectButton.KeyDown += new System.Windows.Forms.KeyEventHandler(this.general_KeyDown);
            // 
            // cancelButton
            // 
            this.cancelButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.cancelButton.Location = new System.Drawing.Point(240, 8);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(75, 23);
            this.cancelButton.TabIndex = 2;
            this.cancelButton.Text = "Cancel";
            this.cancelButton.UseVisualStyleBackColor = true;
            this.cancelButton.Click += new System.EventHandler(this.cancelButton_Click);
            this.cancelButton.KeyDown += new System.Windows.Forms.KeyEventHandler(this.general_KeyDown);
            // 
            // licenseServerTitleLabel
            // 
            this.licenseServerTitleLabel.AutoSize = true;
            this.licenseServerTitleLabel.Location = new System.Drawing.Point(12, 28);
            this.licenseServerTitleLabel.Name = "licenseServerTitleLabel";
            this.licenseServerTitleLabel.Size = new System.Drawing.Size(81, 13);
            this.licenseServerTitleLabel.TabIndex = 1;
            this.licenseServerTitleLabel.Text = "License Server:";
            // 
            // licenseServerComboBox
            // 
            this.licenseServerComboBox.FormattingEnabled = true;
            this.licenseServerComboBox.Location = new System.Drawing.Point(99, 25);
            this.licenseServerComboBox.Name = "licenseServerComboBox";
            this.licenseServerComboBox.Size = new System.Drawing.Size(207, 21);
            this.licenseServerComboBox.TabIndex = 0;
            this.licenseServerComboBox.KeyDown += new System.Windows.Forms.KeyEventHandler(this.licenseServerComboBox_KeyDown);
            // 
            // ConnectionDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(318, 107);
            this.Controls.Add(this.licenseServerComboBox);
            this.Controls.Add(this.licenseServerTitleLabel);
            this.Controls.Add(this.buttonPanel);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Name = "ConnectionDialog";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Connect to License Server";
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.general_KeyDown);
            this.buttonPanel.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

        private System.Windows.Forms.Panel buttonPanel;
        private System.Windows.Forms.Button connectButton;
        private System.Windows.Forms.Button cancelButton;
        private System.Windows.Forms.Label licenseServerTitleLabel;
        private System.Windows.Forms.ComboBox licenseServerComboBox;
	}
}