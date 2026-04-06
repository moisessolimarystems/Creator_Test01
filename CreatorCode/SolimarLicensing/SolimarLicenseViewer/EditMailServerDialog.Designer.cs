namespace SolimarLicenseViewer
{
    partial class EditMailServerDialog
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
            this.panel1 = new System.Windows.Forms.Panel();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.authBasicConfirmPasswordLabel = new System.Windows.Forms.Label();
            this.authBasicPasswordLabel = new System.Windows.Forms.Label();
            this.authBasicUserLabel = new System.Windows.Forms.Label();
            this.authBasicConfirmPasswordTBox = new Shared.VisualComponents.TextBoxEx();
            this.authBasicPasswordTBox = new Shared.VisualComponents.TextBoxEx();
            this.authBasicUserTBox = new Shared.VisualComponents.TextBoxEx();
            this.authBasicRButton = new System.Windows.Forms.RadioButton();
            this.authAnoyRButton = new System.Windows.Forms.RadioButton();
            this.headerPanel = new System.Windows.Forms.Panel();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.displayNameTBox = new Shared.VisualComponents.TextBoxEx();
            this.emailTBox = new Shared.VisualComponents.TextBoxEx();
            this.portTBox = new Shared.VisualComponents.TextBoxEx();
            this.label2 = new System.Windows.Forms.Label();
            this.mailServerTBox = new Shared.VisualComponents.TextBoxEx();
            this.label1 = new System.Windows.Forms.Label();
            this.backPanel.SuspendLayout();
            this.mainTabControl.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.panel1.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.headerPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnOk
            // 
            this.btnOk.Click += new System.EventHandler(this.btnOk_Click);
            // 
            // backPanel
            // 
            this.backPanel.Controls.Add(this.mainTabControl);
            this.backPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.backPanel.Location = new System.Drawing.Point(0, 0);
            this.backPanel.Name = "backPanel";
            this.backPanel.Padding = new System.Windows.Forms.Padding(5);
            this.backPanel.Size = new System.Drawing.Size(483, 311);
            this.backPanel.TabIndex = 3;
            // 
            // mainTabControl
            // 
            this.mainTabControl.Controls.Add(this.tabPage1);
            this.mainTabControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mainTabControl.Location = new System.Drawing.Point(5, 5);
            this.mainTabControl.Name = "mainTabControl";
            this.mainTabControl.SelectedIndex = 0;
            this.mainTabControl.Size = new System.Drawing.Size(473, 301);
            this.mainTabControl.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.panel1);
            this.tabPage1.Controls.Add(this.headerPanel);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(465, 275);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Mail Server";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.groupBox1);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(3, 118);
            this.panel1.Name = "panel1";
            this.panel1.Padding = new System.Windows.Forms.Padding(14, 3, 14, 3);
            this.panel1.Size = new System.Drawing.Size(459, 154);
            this.panel1.TabIndex = 1;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.authBasicConfirmPasswordLabel);
            this.groupBox1.Controls.Add(this.authBasicPasswordLabel);
            this.groupBox1.Controls.Add(this.authBasicUserLabel);
            this.groupBox1.Controls.Add(this.authBasicConfirmPasswordTBox);
            this.groupBox1.Controls.Add(this.authBasicPasswordTBox);
            this.groupBox1.Controls.Add(this.authBasicUserTBox);
            this.groupBox1.Controls.Add(this.authBasicRButton);
            this.groupBox1.Controls.Add(this.authAnoyRButton);
            this.groupBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox1.Location = new System.Drawing.Point(14, 3);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(431, 148);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "SMTP Authentication";
            // 
            // authBasicConfirmPasswordLabel
            // 
            this.authBasicConfirmPasswordLabel.AutoSize = true;
            this.authBasicConfirmPasswordLabel.Location = new System.Drawing.Point(40, 123);
            this.authBasicConfirmPasswordLabel.Name = "authBasicConfirmPasswordLabel";
            this.authBasicConfirmPasswordLabel.Size = new System.Drawing.Size(94, 13);
            this.authBasicConfirmPasswordLabel.TabIndex = 9;
            this.authBasicConfirmPasswordLabel.Text = "Confirm Password:";
            // 
            // authBasicPasswordLabel
            // 
            this.authBasicPasswordLabel.AutoSize = true;
            this.authBasicPasswordLabel.Location = new System.Drawing.Point(40, 97);
            this.authBasicPasswordLabel.Name = "authBasicPasswordLabel";
            this.authBasicPasswordLabel.Size = new System.Drawing.Size(56, 13);
            this.authBasicPasswordLabel.TabIndex = 8;
            this.authBasicPasswordLabel.Text = "Password:";
            // 
            // authBasicUserLabel
            // 
            this.authBasicUserLabel.AutoSize = true;
            this.authBasicUserLabel.Location = new System.Drawing.Point(40, 71);
            this.authBasicUserLabel.Name = "authBasicUserLabel";
            this.authBasicUserLabel.Size = new System.Drawing.Size(58, 13);
            this.authBasicUserLabel.TabIndex = 7;
            this.authBasicUserLabel.Text = "Username:";
            // 
            // authBasicConfirmPasswordTBox
            // 
            this.authBasicConfirmPasswordTBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.authBasicConfirmPasswordTBox.Banner = "Required";
            this.authBasicConfirmPasswordTBox.Location = new System.Drawing.Point(154, 120);
            this.authBasicConfirmPasswordTBox.Name = "authBasicConfirmPasswordTBox";
            this.authBasicConfirmPasswordTBox.PasswordChar = '*';
            this.authBasicConfirmPasswordTBox.Size = new System.Drawing.Size(253, 20);
            this.authBasicConfirmPasswordTBox.TabIndex = 6;
            // 
            // authBasicPasswordTBox
            // 
            this.authBasicPasswordTBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.authBasicPasswordTBox.Banner = "Required";
            this.authBasicPasswordTBox.Location = new System.Drawing.Point(154, 94);
            this.authBasicPasswordTBox.Name = "authBasicPasswordTBox";
            this.authBasicPasswordTBox.PasswordChar = '*';
            this.authBasicPasswordTBox.Size = new System.Drawing.Size(253, 20);
            this.authBasicPasswordTBox.TabIndex = 5;
            // 
            // authBasicUserTBox
            // 
            this.authBasicUserTBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.authBasicUserTBox.Banner = "Required";
            this.authBasicUserTBox.Location = new System.Drawing.Point(154, 68);
            this.authBasicUserTBox.Name = "authBasicUserTBox";
            this.authBasicUserTBox.Size = new System.Drawing.Size(253, 20);
            this.authBasicUserTBox.TabIndex = 4;
            // 
            // authBasicRButton
            // 
            this.authBasicRButton.AutoSize = true;
            this.authBasicRButton.Location = new System.Drawing.Point(20, 42);
            this.authBasicRButton.Name = "authBasicRButton";
            this.authBasicRButton.Size = new System.Drawing.Size(122, 17);
            this.authBasicRButton.TabIndex = 1;
            this.authBasicRButton.TabStop = true;
            this.authBasicRButton.Text = "Basic Authentication";
            this.authBasicRButton.UseVisualStyleBackColor = true;
            this.authBasicRButton.CheckedChanged += new System.EventHandler(this.authCheckedChanged);
            // 
            // authAnoyRButton
            // 
            this.authAnoyRButton.AutoSize = true;
            this.authAnoyRButton.Location = new System.Drawing.Point(20, 19);
            this.authAnoyRButton.Name = "authAnoyRButton";
            this.authAnoyRButton.Size = new System.Drawing.Size(151, 17);
            this.authAnoyRButton.TabIndex = 0;
            this.authAnoyRButton.TabStop = true;
            this.authAnoyRButton.Text = "Anonymous Authentication";
            this.authAnoyRButton.UseVisualStyleBackColor = true;
            this.authAnoyRButton.CheckedChanged += new System.EventHandler(this.authCheckedChanged);
            // 
            // headerPanel
            // 
            this.headerPanel.Controls.Add(this.label4);
            this.headerPanel.Controls.Add(this.label3);
            this.headerPanel.Controls.Add(this.displayNameTBox);
            this.headerPanel.Controls.Add(this.emailTBox);
            this.headerPanel.Controls.Add(this.portTBox);
            this.headerPanel.Controls.Add(this.label2);
            this.headerPanel.Controls.Add(this.mailServerTBox);
            this.headerPanel.Controls.Add(this.label1);
            this.headerPanel.Dock = System.Windows.Forms.DockStyle.Top;
            this.headerPanel.Location = new System.Drawing.Point(3, 3);
            this.headerPanel.Name = "headerPanel";
            this.headerPanel.Size = new System.Drawing.Size(459, 115);
            this.headerPanel.TabIndex = 0;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(14, 90);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(75, 13);
            this.label4.TabIndex = 7;
            this.label4.Text = "Display Name:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(14, 64);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(105, 13);
            this.label3.TabIndex = 6;
            this.label3.Text = "From E-mail Address:";
            // 
            // displayNameTBox
            // 
            this.displayNameTBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.displayNameTBox.Banner = "";
            this.displayNameTBox.Location = new System.Drawing.Point(168, 87);
            this.displayNameTBox.Name = "displayNameTBox";
            this.displayNameTBox.Size = new System.Drawing.Size(253, 20);
            this.displayNameTBox.TabIndex = 5;
            // 
            // emailTBox
            // 
            this.emailTBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.emailTBox.Banner = "Required";
            this.emailTBox.Location = new System.Drawing.Point(168, 61);
            this.emailTBox.Name = "emailTBox";
            this.emailTBox.Size = new System.Drawing.Size(253, 20);
            this.emailTBox.TabIndex = 4;
            // 
            // portTBox
            // 
            this.portTBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.portTBox.Banner = "Required";
            this.portTBox.Location = new System.Drawing.Point(168, 35);
            this.portTBox.Name = "portTBox";
            this.portTBox.Size = new System.Drawing.Size(69, 20);
            this.portTBox.TabIndex = 3;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(14, 38);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(69, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "Port Number:";
            // 
            // mailServerTBox
            // 
            this.mailServerTBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.mailServerTBox.Banner = "Required";
            this.mailServerTBox.Location = new System.Drawing.Point(168, 9);
            this.mailServerTBox.Name = "mailServerTBox";
            this.mailServerTBox.Size = new System.Drawing.Size(253, 20);
            this.mailServerTBox.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(14, 12);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(148, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Outgoing Mail Server (SMTP):";
            // 
            // EditMailServerDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(483, 342);
            this.Controls.Add(this.backPanel);
            this.Name = "EditMailServerDialog";
            this.Text = "Configure Mail Server";
            this.Load += new System.EventHandler(this.EditMailServerDialog_Load);
            this.Controls.SetChildIndex(this.backPanel, 0);
            this.backPanel.ResumeLayout(false);
            this.mainTabControl.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.headerPanel.ResumeLayout(false);
            this.headerPanel.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel backPanel;
        private System.Windows.Forms.TabControl mainTabControl;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.Panel headerPanel;
        private Shared.VisualComponents.TextBoxEx portTBox;
        private System.Windows.Forms.Label label2;
        private Shared.VisualComponents.TextBoxEx mailServerTBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private Shared.VisualComponents.TextBoxEx displayNameTBox;
        private Shared.VisualComponents.TextBoxEx emailTBox;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.RadioButton authBasicRButton;
        private System.Windows.Forms.RadioButton authAnoyRButton;
        private System.Windows.Forms.Label authBasicConfirmPasswordLabel;
        private System.Windows.Forms.Label authBasicPasswordLabel;
        private System.Windows.Forms.Label authBasicUserLabel;
        private Shared.VisualComponents.TextBoxEx authBasicConfirmPasswordTBox;
        private Shared.VisualComponents.TextBoxEx authBasicPasswordTBox;
        private Shared.VisualComponents.TextBoxEx authBasicUserTBox;
    }
}