namespace SolimarLicenseViewer
{
    partial class EnterPasswordDialog
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(EnterPasswordDialog));
            this.passwordLabel = new System.Windows.Forms.Label();
            this.passwordTextBox = new System.Windows.Forms.TextBox();
            this.textPanel = new System.Windows.Forms.Panel();
            this.descLabel = new System.Windows.Forms.Label();
            this.textPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnOk
            // 
            this.btnOk.Click += new System.EventHandler(this.btnOk_Click);
            // 
            // passwordLabel
            // 
            this.passwordLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.passwordLabel.AutoSize = true;
            this.passwordLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.passwordLabel.Location = new System.Drawing.Point(12, 94);
            this.passwordLabel.Name = "passwordLabel";
            this.passwordLabel.Size = new System.Drawing.Size(65, 13);
            this.passwordLabel.TabIndex = 3;
            this.passwordLabel.Text = "Password:";
            // 
            // passwordTextBox
            // 
            this.passwordTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.passwordTextBox.Location = new System.Drawing.Point(83, 91);
            this.passwordTextBox.Name = "passwordTextBox";
            this.passwordTextBox.Size = new System.Drawing.Size(228, 20);
            this.passwordTextBox.TabIndex = 0;
            this.passwordTextBox.TextChanged += new System.EventHandler(this.passwordTextBox_TextChanged);
            // 
            // textPanel
            // 
            this.textPanel.Controls.Add(this.descLabel);
            this.textPanel.Dock = System.Windows.Forms.DockStyle.Top;
            this.textPanel.Location = new System.Drawing.Point(0, 0);
            this.textPanel.Name = "textPanel";
            this.textPanel.Padding = new System.Windows.Forms.Padding(10);
            this.textPanel.Size = new System.Drawing.Size(314, 84);
            this.textPanel.TabIndex = 6;
            // 
            // descLabel
            // 
            this.descLabel.Dock = System.Windows.Forms.DockStyle.Top;
            this.descLabel.Location = new System.Drawing.Point(10, 10);
            this.descLabel.Name = "descLabel";
            this.descLabel.Size = new System.Drawing.Size(294, 67);
            this.descLabel.TabIndex = 6;
            this.descLabel.Text = resources.GetString("descLabel.Text");
            // 
            // EnterPasswordDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(314, 148);
            this.Controls.Add(this.textPanel);
            this.Controls.Add(this.passwordTextBox);
            this.Controls.Add(this.passwordLabel);
            this.Name = "EnterPasswordDialog";
            this.Text = "Please Enter a Password";
            this.Load += new System.EventHandler(this.EnterPasswordDialog_Load);
            this.Controls.SetChildIndex(this.passwordLabel, 0);
            this.Controls.SetChildIndex(this.passwordTextBox, 0);
            this.Controls.SetChildIndex(this.textPanel, 0);
            this.textPanel.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label passwordLabel;
        private System.Windows.Forms.TextBox passwordTextBox;
        private System.Windows.Forms.Panel textPanel;
        private System.Windows.Forms.Label descLabel;
    }
}