namespace SolimarLicenseViewer
{
    partial class AboutBox
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
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
            this.versionServer = new System.Windows.Forms.Label();
            this.labelServer = new System.Windows.Forms.Label();
            this.linkLabel1 = new System.Windows.Forms.LinkLabel();
            this.linkWebsite = new System.Windows.Forms.LinkLabel();
            this.labelCopyright = new System.Windows.Forms.Label();
            this.labelPhoneNumbers = new System.Windows.Forms.Label();
            this.labelAddress = new System.Windows.Forms.Label();
            this.labelWebsite = new System.Windows.Forms.Label();
            this.linkTechSupport = new System.Windows.Forms.LinkLabel();
            this.linkSales = new System.Windows.Forms.LinkLabel();
            this.labelFtp = new System.Windows.Forms.Label();
            this.labelEmail = new System.Windows.Forms.Label();
            this.labelLicensing = new System.Windows.Forms.Label();
            this.versionLicensing = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // versionServer
            // 
            this.versionServer.BackColor = System.Drawing.Color.Transparent;
            this.versionServer.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.versionServer.Location = new System.Drawing.Point(431, 59);
            this.versionServer.Name = "versionServer";
            this.versionServer.Size = new System.Drawing.Size(108, 16);
            this.versionServer.TabIndex = 42;
            this.versionServer.Text = "XX.XX.XXXXXX";
            this.versionServer.Click += new System.EventHandler(this.AboutBox_Click);
            // 
            // labelServer
            // 
            this.labelServer.BackColor = System.Drawing.Color.Transparent;
            this.labelServer.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelServer.Location = new System.Drawing.Point(431, 44);
            this.labelServer.Name = "labelServer";
            this.labelServer.Size = new System.Drawing.Size(160, 15);
            this.labelServer.TabIndex = 41;
            this.labelServer.Text = "License Server:";
            this.labelServer.Click += new System.EventHandler(this.AboutBox_Click);
            // 
            // linkLabel1
            // 
            this.linkLabel1.BackColor = System.Drawing.Color.Transparent;
            this.linkLabel1.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.linkLabel1.LinkColor = System.Drawing.Color.FromArgb(((int)(((byte)(51)))), ((int)(((byte)(102)))), ((int)(((byte)(153)))));
            this.linkLabel1.Location = new System.Drawing.Point(47, 112);
            this.linkLabel1.Name = "linkLabel1";
            this.linkLabel1.Size = new System.Drawing.Size(235, 16);
            this.linkLabel1.TabIndex = 1;
            this.linkLabel1.TabStop = true;
            this.linkLabel1.Text = "ftp://downloads.solimarsystems.com";
            this.linkLabel1.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.HTTPLabel_LinkClicked);
            this.linkLabel1.PreviewKeyDown += new System.Windows.Forms.PreviewKeyDownEventHandler(this.AboutBox_PreviewKeyDown);
            // 
            // linkWebsite
            // 
            this.linkWebsite.BackColor = System.Drawing.Color.Transparent;
            this.linkWebsite.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.linkWebsite.LinkColor = System.Drawing.Color.FromArgb(((int)(((byte)(51)))), ((int)(((byte)(102)))), ((int)(((byte)(153)))));
            this.linkWebsite.Location = new System.Drawing.Point(47, 96);
            this.linkWebsite.Name = "linkWebsite";
            this.linkWebsite.Size = new System.Drawing.Size(235, 16);
            this.linkWebsite.TabIndex = 0;
            this.linkWebsite.TabStop = true;
            this.linkWebsite.Text = "http://www.solimarsystems.com";
            this.linkWebsite.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.HTTPLabel_LinkClicked);
            this.linkWebsite.PreviewKeyDown += new System.Windows.Forms.PreviewKeyDownEventHandler(this.AboutBox_PreviewKeyDown);
            // 
            // labelCopyright
            // 
            this.labelCopyright.BackColor = System.Drawing.Color.Transparent;
            this.labelCopyright.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelCopyright.Location = new System.Drawing.Point(3, 179);
            this.labelCopyright.Name = "labelCopyright";
            this.labelCopyright.Size = new System.Drawing.Size(588, 17);
            this.labelCopyright.TabIndex = 31;
            this.labelCopyright.Text = "© 1997-XXXX. Solimar Systems, Inc. All rights reserved. All trademarks are the pr" +
       "operty of their respective owners.";
            this.labelCopyright.Click += new System.EventHandler(this.AboutBox_Click);
            // 
            // labelPhoneNumbers
            // 
            this.labelPhoneNumbers.BackColor = System.Drawing.Color.Transparent;
            this.labelPhoneNumbers.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelPhoneNumbers.Location = new System.Drawing.Point(3, 48);
            this.labelPhoneNumbers.Name = "labelPhoneNumbers";
            this.labelPhoneNumbers.Size = new System.Drawing.Size(148, 16);
            this.labelPhoneNumbers.TabIndex = 36;
            this.labelPhoneNumbers.Text = "Tel:   +1.619.849.2800";
            this.labelPhoneNumbers.Click += new System.EventHandler(this.AboutBox_Click);
            // 
            // labelAddress
            // 
            this.labelAddress.BackColor = System.Drawing.Color.Transparent;
            this.labelAddress.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelAddress.Location = new System.Drawing.Point(3, 0);
            this.labelAddress.Name = "labelAddress";
            this.labelAddress.Size = new System.Drawing.Size(148, 16);
            this.labelAddress.TabIndex = 33;
            this.labelAddress.Text = "Solimar Systems, Inc.";
            this.labelAddress.Click += new System.EventHandler(this.AboutBox_Click);
            // 
            // labelWebsite
            // 
            this.labelWebsite.BackColor = System.Drawing.Color.Transparent;
            this.labelWebsite.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelWebsite.Location = new System.Drawing.Point(3, 96);
            this.labelWebsite.Name = "labelWebsite";
            this.labelWebsite.Size = new System.Drawing.Size(42, 16);
            this.labelWebsite.TabIndex = 34;
            this.labelWebsite.Text = "Web:";
            this.labelWebsite.Click += new System.EventHandler(this.AboutBox_Click);
            // 
            // linkTechSupport
            // 
            this.linkTechSupport.BackColor = System.Drawing.Color.Transparent;
            this.linkTechSupport.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.linkTechSupport.LinkColor = System.Drawing.Color.FromArgb(((int)(((byte)(51)))), ((int)(((byte)(102)))), ((int)(((byte)(153)))));
            this.linkTechSupport.Location = new System.Drawing.Point(47, 128);
            this.linkTechSupport.Name = "linkTechSupport";
            this.linkTechSupport.Size = new System.Drawing.Size(235, 16);
            this.linkTechSupport.TabIndex = 2;
            this.linkTechSupport.TabStop = true;
            this.linkTechSupport.Text = "tech.support@solimarsystems.com";
            this.linkTechSupport.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.EmailLabel_LinkClicked);
            this.linkTechSupport.PreviewKeyDown += new System.Windows.Forms.PreviewKeyDownEventHandler(this.AboutBox_PreviewKeyDown);
            // 
            // linkSales
            // 
            this.linkSales.BackColor = System.Drawing.Color.Transparent;
            this.linkSales.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.linkSales.LinkColor = System.Drawing.Color.FromArgb(((int)(((byte)(51)))), ((int)(((byte)(102)))), ((int)(((byte)(153)))));
            this.linkSales.Location = new System.Drawing.Point(47, 144);
            this.linkSales.Name = "linkSales";
            this.linkSales.Size = new System.Drawing.Size(235, 16);
            this.linkSales.TabIndex = 3;
            this.linkSales.TabStop = true;
            this.linkSales.Text = "sales@solimarsystems.com";
            this.linkSales.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.EmailLabel_LinkClicked);
            this.linkSales.PreviewKeyDown += new System.Windows.Forms.PreviewKeyDownEventHandler(this.AboutBox_PreviewKeyDown);
            // 
            // labelFtp
            // 
            this.labelFtp.BackColor = System.Drawing.Color.Transparent;
            this.labelFtp.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelFtp.Location = new System.Drawing.Point(3, 112);
            this.labelFtp.Name = "labelFtp";
            this.labelFtp.Size = new System.Drawing.Size(42, 16);
            this.labelFtp.TabIndex = 25;
            this.labelFtp.Text = "FTP:";
            this.labelFtp.Click += new System.EventHandler(this.AboutBox_Click);
            // 
            // labelEmail
            // 
            this.labelEmail.BackColor = System.Drawing.Color.Transparent;
            this.labelEmail.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelEmail.Location = new System.Drawing.Point(3, 128);
            this.labelEmail.Name = "labelEmail";
            this.labelEmail.Size = new System.Drawing.Size(42, 16);
            this.labelEmail.TabIndex = 26;
            this.labelEmail.Text = "EMail:";
            this.labelEmail.Click += new System.EventHandler(this.AboutBox_Click);
            // 
            // labelLicensing
            // 
            this.labelLicensing.BackColor = System.Drawing.Color.Transparent;
            this.labelLicensing.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelLicensing.Location = new System.Drawing.Point(431, 0);
            this.labelLicensing.Name = "labelLicensing";
            this.labelLicensing.Size = new System.Drawing.Size(160, 16);
            this.labelLicensing.TabIndex = 28;
            this.labelLicensing.Text = "License Manager :";
            this.labelLicensing.Click += new System.EventHandler(this.AboutBox_Click);
            // 
            // versionLicensing
            // 
            this.versionLicensing.BackColor = System.Drawing.Color.Transparent;
            this.versionLicensing.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.versionLicensing.Location = new System.Drawing.Point(431, 16);
            this.versionLicensing.Name = "versionLicensing";
            this.versionLicensing.Size = new System.Drawing.Size(108, 16);
            this.versionLicensing.TabIndex = 29;
            this.versionLicensing.Text = "XX.XX.XXXXXX";
            this.versionLicensing.Click += new System.EventHandler(this.AboutBox_Click);
            // 
            // label1
            // 
            this.label1.BackColor = System.Drawing.Color.Transparent;
            this.label1.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(3, 16);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(148, 16);
            this.label1.TabIndex = 27;
            this.label1.Text = "1515 Second Ave.";
            this.label1.Click += new System.EventHandler(this.AboutBox_Click);
            // 
            // label2
            // 
            this.label2.BackColor = System.Drawing.Color.Transparent;
            this.label2.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(3, 32);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(148, 16);
            this.label2.TabIndex = 30;
            this.label2.Text = "San Diego, CA 92101";
            this.label2.Click += new System.EventHandler(this.AboutBox_Click);
            // 
            // label3
            // 
            this.label3.BackColor = System.Drawing.Color.Transparent;
            this.label3.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(3, 64);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(148, 16);
            this.label3.TabIndex = 32;
            this.label3.Text = "Fax:  +1.619.849.2801";
            this.label3.Click += new System.EventHandler(this.AboutBox_Click);
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.Transparent;
            this.panel1.Controls.Add(this.labelAddress);
            this.panel1.Controls.Add(this.versionServer);
            this.panel1.Controls.Add(this.label3);
            this.panel1.Controls.Add(this.labelServer);
            this.panel1.Controls.Add(this.label2);
            this.panel1.Controls.Add(this.linkLabel1);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Controls.Add(this.linkWebsite);
            this.panel1.Controls.Add(this.versionLicensing);
            this.panel1.Controls.Add(this.labelCopyright);
            this.panel1.Controls.Add(this.labelLicensing);
            this.panel1.Controls.Add(this.labelEmail);
            this.panel1.Controls.Add(this.labelPhoneNumbers);
            this.panel1.Controls.Add(this.labelFtp);
            this.panel1.Controls.Add(this.linkSales);
            this.panel1.Controls.Add(this.labelWebsite);
            this.panel1.Controls.Add(this.linkTechSupport);
            this.panel1.Location = new System.Drawing.Point(72, 295);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(594, 196);
            this.panel1.TabIndex = 43;
            this.panel1.Click += new System.EventHandler(this.AboutBox_Click);
            // 
            // AboutBox
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImage = global::SolimarLicenseViewer.Properties.Resources.AboutBox;
            this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.ClientSize = new System.Drawing.Size(740, 580);
            this.Controls.Add(this.panel1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "AboutBox";
            this.Padding = new System.Windows.Forms.Padding(9);
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "AboutBox";
            this.Load += new System.EventHandler(this.AboutBox_Load);
            this.Click += new System.EventHandler(this.AboutBox_Click);
            this.PreviewKeyDown += new System.Windows.Forms.PreviewKeyDownEventHandler(this.AboutBox_PreviewKeyDown);
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        protected System.Windows.Forms.Label versionServer;
        private System.Windows.Forms.Label labelServer;
        protected System.Windows.Forms.LinkLabel linkLabel1;
        protected System.Windows.Forms.LinkLabel linkWebsite;
        protected System.Windows.Forms.Label labelCopyright;
        protected System.Windows.Forms.Label labelPhoneNumbers;
        protected System.Windows.Forms.Label labelAddress;
        protected System.Windows.Forms.Label labelWebsite;
        protected System.Windows.Forms.LinkLabel linkTechSupport;
        protected System.Windows.Forms.LinkLabel linkSales;
        protected System.Windows.Forms.Label labelFtp;
        protected System.Windows.Forms.Label labelEmail;
        protected System.Windows.Forms.Label labelLicensing;
        protected System.Windows.Forms.Label versionLicensing;
        protected System.Windows.Forms.Label label1;
        protected System.Windows.Forms.Label label2;
        protected System.Windows.Forms.Label label3;
        private System.Windows.Forms.Panel panel1;

    }
}
