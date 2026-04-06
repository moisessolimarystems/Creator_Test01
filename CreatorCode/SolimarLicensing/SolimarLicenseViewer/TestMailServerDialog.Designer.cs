namespace SolimarLicenseViewer
{
    partial class TestMailServerDialog
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
            this.backPanel = new System.Windows.Forms.Panel();
            this.mainTabControl = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.headerPanel = new System.Windows.Forms.Panel();
            this.bodyTBox = new Shared.VisualComponents.TextBoxEx();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.subjectTBox = new Shared.VisualComponents.TextBoxEx();
            this.toTBox = new Shared.VisualComponents.TextBoxEx();
            this.label1 = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.configMailButton = new System.Windows.Forms.Button();
            this.testMailButton = new System.Windows.Forms.Button();
            this.backPanel.SuspendLayout();
            this.mainTabControl.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.headerPanel.SuspendLayout();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnCancel
            // 
            this.btnCancel.Location = new System.Drawing.Point(12, 5);
            this.btnCancel.Text = "Not Shown";
            this.btnCancel.Visible = false;
            // 
            // btnOk
            // 
            this.btnOk.Location = new System.Drawing.Point(93, 4);
            this.btnOk.Text = "Close";
            // 
            // backPanel
            // 
            this.backPanel.Controls.Add(this.mainTabControl);
            this.backPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.backPanel.Location = new System.Drawing.Point(0, 0);
            this.backPanel.Name = "backPanel";
            this.backPanel.Padding = new System.Windows.Forms.Padding(5);
            this.backPanel.Size = new System.Drawing.Size(438, 261);
            this.backPanel.TabIndex = 4;
            // 
            // mainTabControl
            // 
            this.mainTabControl.Controls.Add(this.tabPage1);
            this.mainTabControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mainTabControl.Location = new System.Drawing.Point(5, 5);
            this.mainTabControl.Name = "mainTabControl";
            this.mainTabControl.SelectedIndex = 0;
            this.mainTabControl.Size = new System.Drawing.Size(428, 251);
            this.mainTabControl.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.headerPanel);
            this.tabPage1.Controls.Add(this.panel1);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(420, 225);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Mail Server";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // headerPanel
            // 
            this.headerPanel.Controls.Add(this.bodyTBox);
            this.headerPanel.Controls.Add(this.label4);
            this.headerPanel.Controls.Add(this.label3);
            this.headerPanel.Controls.Add(this.subjectTBox);
            this.headerPanel.Controls.Add(this.toTBox);
            this.headerPanel.Controls.Add(this.label1);
            this.headerPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.headerPanel.Location = new System.Drawing.Point(3, 3);
            this.headerPanel.Name = "headerPanel";
            this.headerPanel.Size = new System.Drawing.Size(414, 189);
            this.headerPanel.TabIndex = 1;
            // 
            // bodyTBox
            // 
            this.bodyTBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.bodyTBox.Banner = "";
            this.bodyTBox.Location = new System.Drawing.Point(72, 61);
            this.bodyTBox.Multiline = true;
            this.bodyTBox.Name = "bodyTBox";
            this.bodyTBox.Size = new System.Drawing.Size(325, 122);
            this.bodyTBox.TabIndex = 8;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(14, 64);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(34, 13);
            this.label4.TabIndex = 7;
            this.label4.Text = "Body:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(14, 38);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(46, 13);
            this.label3.TabIndex = 6;
            this.label3.Text = "Subject:";
            // 
            // subjectTBox
            // 
            this.subjectTBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.subjectTBox.Banner = "";
            this.subjectTBox.Location = new System.Drawing.Point(72, 35);
            this.subjectTBox.Name = "subjectTBox";
            this.subjectTBox.Size = new System.Drawing.Size(325, 20);
            this.subjectTBox.TabIndex = 4;
            // 
            // toTBox
            // 
            this.toTBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.toTBox.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.SuggestAppend;
            this.toTBox.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.CustomSource;
            this.toTBox.Banner = "Required";
            this.toTBox.Location = new System.Drawing.Point(72, 9);
            this.toTBox.Name = "toTBox";
            this.toTBox.Size = new System.Drawing.Size(325, 20);
            this.toTBox.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(14, 12);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(23, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "To:";
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.configMailButton);
            this.panel1.Controls.Add(this.testMailButton);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panel1.Location = new System.Drawing.Point(3, 192);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(414, 30);
            this.panel1.TabIndex = 2;
            // 
            // configMailButton
            // 
            this.configMailButton.Location = new System.Drawing.Point(129, 3);
            this.configMailButton.Name = "configMailButton";
            this.configMailButton.Size = new System.Drawing.Size(123, 23);
            this.configMailButton.TabIndex = 1;
            this.configMailButton.Text = "Configure Mail Server";
            this.configMailButton.UseVisualStyleBackColor = true;
            this.configMailButton.Click += new System.EventHandler(this.configMailButton_Click);
            // 
            // testMailButton
            // 
            this.testMailButton.Location = new System.Drawing.Point(17, 3);
            this.testMailButton.Name = "testMailButton";
            this.testMailButton.Size = new System.Drawing.Size(106, 23);
            this.testMailButton.TabIndex = 0;
            this.testMailButton.Text = "Send Test E-mail";
            this.testMailButton.UseVisualStyleBackColor = true;
            this.testMailButton.Click += new System.EventHandler(this.testMailButton_Click);
            // 
            // TestMailServerDialog
            // 
            this.AcceptButton = null;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.btnOk;
            this.ClientSize = new System.Drawing.Size(438, 292);
            this.Controls.Add(this.backPanel);
            this.Name = "TestMailServerDialog";
            this.Text = "Test Mail Server";
            this.Load += new System.EventHandler(this.TestMailServerDialog_Load);
            this.Controls.SetChildIndex(this.backPanel, 0);
            this.backPanel.ResumeLayout(false);
            this.mainTabControl.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.headerPanel.ResumeLayout(false);
            this.headerPanel.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel backPanel;
        private System.Windows.Forms.TabControl mainTabControl;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.Panel headerPanel;
        private Shared.VisualComponents.TextBoxEx bodyTBox;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private Shared.VisualComponents.TextBoxEx subjectTBox;
        private Shared.VisualComponents.TextBoxEx toTBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button testMailButton;
        private System.Windows.Forms.Button configMailButton;
    }
}