namespace SolimarLicenseDiagnosticDataViewer
{
    partial class SolimarValidationToken_DisplayForm
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
            this.mainBackPanel = new System.Windows.Forms.Panel();
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
            this.mainBackPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // mainBackPanel
            // 
            this.mainBackPanel.Controls.Add(this.uuidLabel);
            this.mainBackPanel.Controls.Add(this.uuidTextBox);
            this.mainBackPanel.Controls.Add(this.companyLabel);
            this.mainBackPanel.Controls.Add(this.companyTextBox);
            this.mainBackPanel.Controls.Add(this.biosLabel);
            this.mainBackPanel.Controls.Add(this.macLabel);
            this.mainBackPanel.Controls.Add(this.compNameLabel);
            this.mainBackPanel.Controls.Add(this.biosTextBox);
            this.mainBackPanel.Controls.Add(this.macTextBox);
            this.mainBackPanel.Controls.Add(this.compNameTextBox);
            this.mainBackPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mainBackPanel.Location = new System.Drawing.Point(0, 0);
            this.mainBackPanel.Name = "mainBackPanel";
            this.mainBackPanel.Size = new System.Drawing.Size(502, 160);
            this.mainBackPanel.TabIndex = 2;
            // 
            // uuidLabel
            // 
            this.uuidLabel.AutoSize = true;
            this.uuidLabel.Location = new System.Drawing.Point(12, 122);
            this.uuidLabel.Name = "uuidLabel";
            this.uuidLabel.Size = new System.Drawing.Size(114, 13);
            this.uuidLabel.TabIndex = 12;
            this.uuidLabel.Text = "System Product UUID:";
            // 
            // uuidTextBox
            // 
            this.uuidTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.uuidTextBox.Location = new System.Drawing.Point(146, 119);
            this.uuidTextBox.Name = "uuidTextBox";
            this.uuidTextBox.ReadOnly = true;
            this.uuidTextBox.Size = new System.Drawing.Size(344, 20);
            this.uuidTextBox.TabIndex = 11;
            this.uuidTextBox.MouseDown += new System.Windows.Forms.MouseEventHandler(this.GeneralMouseDown);
            // 
            // companyLabel
            // 
            this.companyLabel.AutoSize = true;
            this.companyLabel.Location = new System.Drawing.Point(12, 19);
            this.companyLabel.Name = "companyLabel";
            this.companyLabel.Size = new System.Drawing.Size(85, 13);
            this.companyLabel.TabIndex = 10;
            this.companyLabel.Text = "Company Name:";
            // 
            // companyTextBox
            // 
            this.companyTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.companyTextBox.Location = new System.Drawing.Point(146, 16);
            this.companyTextBox.Name = "companyTextBox";
            this.companyTextBox.ReadOnly = true;
            this.companyTextBox.Size = new System.Drawing.Size(344, 20);
            this.companyTextBox.TabIndex = 0;
            this.companyTextBox.MouseDown += new System.Windows.Forms.MouseEventHandler(this.GeneralMouseDown);
            // 
            // biosLabel
            // 
            this.biosLabel.AutoSize = true;
            this.biosLabel.Location = new System.Drawing.Point(12, 96);
            this.biosLabel.Name = "biosLabel";
            this.biosLabel.Size = new System.Drawing.Size(99, 13);
            this.biosLabel.TabIndex = 6;
            this.biosLabel.Text = "Bios Serial Number:";
            // 
            // macLabel
            // 
            this.macLabel.AutoSize = true;
            this.macLabel.Location = new System.Drawing.Point(12, 70);
            this.macLabel.Name = "macLabel";
            this.macLabel.Size = new System.Drawing.Size(74, 13);
            this.macLabel.TabIndex = 5;
            this.macLabel.Text = "MAC Address:";
            // 
            // compNameLabel
            // 
            this.compNameLabel.AutoSize = true;
            this.compNameLabel.Location = new System.Drawing.Point(12, 44);
            this.compNameLabel.Name = "compNameLabel";
            this.compNameLabel.Size = new System.Drawing.Size(86, 13);
            this.compNameLabel.TabIndex = 4;
            this.compNameLabel.Text = "Computer Name:";
            // 
            // biosTextBox
            // 
            this.biosTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.biosTextBox.Location = new System.Drawing.Point(146, 93);
            this.biosTextBox.Name = "biosTextBox";
            this.biosTextBox.ReadOnly = true;
            this.biosTextBox.Size = new System.Drawing.Size(344, 20);
            this.biosTextBox.TabIndex = 3;
            this.biosTextBox.MouseDown += new System.Windows.Forms.MouseEventHandler(this.GeneralMouseDown);
            // 
            // macTextBox
            // 
            this.macTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.macTextBox.Location = new System.Drawing.Point(146, 67);
            this.macTextBox.Name = "macTextBox";
            this.macTextBox.ReadOnly = true;
            this.macTextBox.Size = new System.Drawing.Size(344, 20);
            this.macTextBox.TabIndex = 2;
            this.macTextBox.MouseDown += new System.Windows.Forms.MouseEventHandler(this.GeneralMouseDown);
            // 
            // compNameTextBox
            // 
            this.compNameTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.compNameTextBox.Location = new System.Drawing.Point(146, 41);
            this.compNameTextBox.Name = "compNameTextBox";
            this.compNameTextBox.ReadOnly = true;
            this.compNameTextBox.Size = new System.Drawing.Size(344, 20);
            this.compNameTextBox.TabIndex = 1;
            this.compNameTextBox.MouseDown += new System.Windows.Forms.MouseEventHandler(this.GeneralMouseDown);
            // 
            // SolimarValidationToken_DisplayForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(502, 160);
            this.Controls.Add(this.mainBackPanel);
            this.Name = "SolimarValidationToken_DisplayForm";
            this.ShowIcon = false;
            this.Text = "Solimar Validation Token";
            this.mainBackPanel.ResumeLayout(false);
            this.mainBackPanel.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel mainBackPanel;
        private System.Windows.Forms.Label uuidLabel;
        private System.Windows.Forms.TextBox uuidTextBox;
        private System.Windows.Forms.Label companyLabel;
        private System.Windows.Forms.TextBox companyTextBox;
        private System.Windows.Forms.Label biosLabel;
        private System.Windows.Forms.Label macLabel;
        private System.Windows.Forms.Label compNameLabel;
        private System.Windows.Forms.TextBox biosTextBox;
        private System.Windows.Forms.TextBox macTextBox;
        private System.Windows.Forms.TextBox compNameTextBox;
    }
}