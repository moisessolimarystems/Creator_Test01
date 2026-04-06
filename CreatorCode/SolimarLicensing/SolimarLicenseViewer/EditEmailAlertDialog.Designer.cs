namespace SolimarLicenseViewer
{
    partial class EditEmailAlertDialog
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
            this.backPanel = new System.Windows.Forms.Panel();
            this.mainTabControl = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.headerPanel = new System.Windows.Forms.Panel();
            this.eventsConfigButton = new System.Windows.Forms.Button();
            this.productsConfigureButton = new System.Windows.Forms.Button();
            this.label5 = new System.Windows.Forms.Label();
            this.eventsTextBox = new Shared.VisualComponents.TextBoxEx();
            this.label4 = new System.Windows.Forms.Label();
            this.productsTextBox = new Shared.VisualComponents.TextBoxEx();
            this.statusComboBox = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.recipentsTBox = new Shared.VisualComponents.TextBoxEx();
            this.label2 = new System.Windows.Forms.Label();
            this.nameTBox = new Shared.VisualComponents.TextBoxEx();
            this.label1 = new System.Windows.Forms.Label();
            this.generalToolTip = new System.Windows.Forms.ToolTip(this.components);
            this.backPanel.SuspendLayout();
            this.mainTabControl.SuspendLayout();
            this.tabPage1.SuspendLayout();
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
            this.backPanel.Size = new System.Drawing.Size(447, 189);
            this.backPanel.TabIndex = 4;
            // 
            // mainTabControl
            // 
            this.mainTabControl.Controls.Add(this.tabPage1);
            this.mainTabControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mainTabControl.Location = new System.Drawing.Point(5, 5);
            this.mainTabControl.Name = "mainTabControl";
            this.mainTabControl.SelectedIndex = 0;
            this.mainTabControl.Size = new System.Drawing.Size(437, 179);
            this.mainTabControl.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.headerPanel);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(429, 153);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "E-mail Alert";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // headerPanel
            // 
            this.headerPanel.Controls.Add(this.eventsConfigButton);
            this.headerPanel.Controls.Add(this.productsConfigureButton);
            this.headerPanel.Controls.Add(this.label5);
            this.headerPanel.Controls.Add(this.eventsTextBox);
            this.headerPanel.Controls.Add(this.label4);
            this.headerPanel.Controls.Add(this.productsTextBox);
            this.headerPanel.Controls.Add(this.statusComboBox);
            this.headerPanel.Controls.Add(this.label3);
            this.headerPanel.Controls.Add(this.recipentsTBox);
            this.headerPanel.Controls.Add(this.label2);
            this.headerPanel.Controls.Add(this.nameTBox);
            this.headerPanel.Controls.Add(this.label1);
            this.headerPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.headerPanel.Location = new System.Drawing.Point(3, 3);
            this.headerPanel.Name = "headerPanel";
            this.headerPanel.Size = new System.Drawing.Size(423, 147);
            this.headerPanel.TabIndex = 0;
            // 
            // eventsConfigButton
            // 
            this.eventsConfigButton.Location = new System.Drawing.Point(334, 111);
            this.eventsConfigButton.Name = "eventsConfigButton";
            this.eventsConfigButton.Size = new System.Drawing.Size(75, 23);
            this.eventsConfigButton.TabIndex = 7;
            this.eventsConfigButton.Text = "Configure...";
            this.generalToolTip.SetToolTip(this.eventsConfigButton, "Configure Events...");
            this.eventsConfigButton.UseVisualStyleBackColor = true;
            this.eventsConfigButton.Click += new System.EventHandler(this.eventsConfigButton_Click);
            // 
            // productsConfigureButton
            // 
            this.productsConfigureButton.Location = new System.Drawing.Point(334, 85);
            this.productsConfigureButton.Name = "productsConfigureButton";
            this.productsConfigureButton.Size = new System.Drawing.Size(75, 23);
            this.productsConfigureButton.TabIndex = 5;
            this.productsConfigureButton.Text = "Configure...";
            this.generalToolTip.SetToolTip(this.productsConfigureButton, "Configure Solimar Products...");
            this.productsConfigureButton.UseVisualStyleBackColor = true;
            this.productsConfigureButton.Click += new System.EventHandler(this.productsConfigureButton_Click);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(14, 116);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(57, 13);
            this.label5.TabIndex = 10;
            this.label5.Text = "Event IDs:";
            // 
            // eventsTextBox
            // 
            this.eventsTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.eventsTextBox.Banner = "Required";
            this.eventsTextBox.Location = new System.Drawing.Point(109, 113);
            this.eventsTextBox.Name = "eventsTextBox";
            this.eventsTextBox.ReadOnly = true;
            this.eventsTextBox.Size = new System.Drawing.Size(219, 20);
            this.eventsTextBox.TabIndex = 6;
            this.eventsTextBox.TabStop = false;
            this.eventsTextBox.Enter += new System.EventHandler(this.eventsConfigButton_Click);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(14, 90);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(89, 13);
            this.label4.TabIndex = 8;
            this.label4.Text = "Solimar Products:";
            // 
            // productsTextBox
            // 
            this.productsTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.productsTextBox.Banner = "Required";
            this.productsTextBox.Location = new System.Drawing.Point(109, 87);
            this.productsTextBox.Name = "productsTextBox";
            this.productsTextBox.ReadOnly = true;
            this.productsTextBox.Size = new System.Drawing.Size(219, 20);
            this.productsTextBox.TabIndex = 4;
            this.productsTextBox.TabStop = false;
            this.generalToolTip.SetToolTip(this.productsTextBox, "Default");
            this.productsTextBox.Enter += new System.EventHandler(this.productsConfigureButton_Click);
            // 
            // statusComboBox
            // 
            this.statusComboBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.statusComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.statusComboBox.FormattingEnabled = true;
            this.statusComboBox.Location = new System.Drawing.Point(109, 35);
            this.statusComboBox.Name = "statusComboBox";
            this.statusComboBox.Size = new System.Drawing.Size(300, 21);
            this.statusComboBox.TabIndex = 2;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(14, 64);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(58, 13);
            this.label3.TabIndex = 6;
            this.label3.Text = "Recipents:";
            // 
            // recipentsTBox
            // 
            this.recipentsTBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.recipentsTBox.Banner = "Required";
            this.recipentsTBox.Location = new System.Drawing.Point(109, 61);
            this.recipentsTBox.Name = "recipentsTBox";
            this.recipentsTBox.Size = new System.Drawing.Size(300, 20);
            this.recipentsTBox.TabIndex = 3;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(14, 38);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(40, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "Status:";
            // 
            // nameTBox
            // 
            this.nameTBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.nameTBox.Banner = "Required";
            this.nameTBox.Location = new System.Drawing.Point(109, 9);
            this.nameTBox.Name = "nameTBox";
            this.nameTBox.Size = new System.Drawing.Size(300, 20);
            this.nameTBox.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(14, 12);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(62, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Alert Name:";
            // 
            // EditEmailAlertDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(447, 220);
            this.Controls.Add(this.backPanel);
            this.Name = "EditEmailAlertDialog";
            this.Text = "Edit Email Alert";
            this.Load += new System.EventHandler(this.EditEmailAlertDialog_Load);
            this.Controls.SetChildIndex(this.backPanel, 0);
            this.backPanel.ResumeLayout(false);
            this.mainTabControl.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.headerPanel.ResumeLayout(false);
            this.headerPanel.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel backPanel;
        private System.Windows.Forms.TabControl mainTabControl;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.Panel headerPanel;
        private System.Windows.Forms.Label label3;
        private Shared.VisualComponents.TextBoxEx recipentsTBox;
        private System.Windows.Forms.Label label2;
        private Shared.VisualComponents.TextBoxEx nameTBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox statusComboBox;
        private System.Windows.Forms.Label label5;
        private Shared.VisualComponents.TextBoxEx eventsTextBox;
        private System.Windows.Forms.Label label4;
        private Shared.VisualComponents.TextBoxEx productsTextBox;
        private System.Windows.Forms.Button eventsConfigButton;
        private System.Windows.Forms.Button productsConfigureButton;
        private System.Windows.Forms.ToolTip generalToolTip;
    }
}