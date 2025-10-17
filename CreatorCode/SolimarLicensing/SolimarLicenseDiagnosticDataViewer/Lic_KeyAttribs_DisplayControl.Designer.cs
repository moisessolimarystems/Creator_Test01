namespace SolimarLicenseDiagnosticDataViewer
{
    partial class Lic_KeyAttribs_DisplayControl
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.mainBackPanel = new System.Windows.Forms.Panel();
            this.mainBackSplitContainer = new System.Windows.Forms.SplitContainer();
            this.headerPanel = new System.Windows.Forms.Panel();
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.copyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.historyLabel = new System.Windows.Forms.Label();
            this.historyTitleLabel = new System.Windows.Forms.Label();
            this.keyVersionLabel = new System.Windows.Forms.Label();
            this.keyVersionTitleLabel = new System.Windows.Forms.Label();
            this.lastTouchDateLabel = new System.Windows.Forms.Label();
            this.modifiedDateLabel = new System.Windows.Forms.Label();
            this.verificationCodeLabel = new System.Windows.Forms.Label();
            this.keyNameLabel = new System.Windows.Forms.Label();
            this.lastTouchDateTitleLabel = new System.Windows.Forms.Label();
            this.modifiedDateTitlelabel = new System.Windows.Forms.Label();
            this.verificationCodeTitleLabel = new System.Windows.Forms.Label();
            this.keyNameTitleLabel = new System.Windows.Forms.Label();
            this.activitySlotLView = new Shared.VisualComponents.NoFlickerListView();
            this.columnHeader9 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader10 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader11 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader12 = new System.Windows.Forms.ColumnHeader();
            this.mainBackPanel.SuspendLayout();
            this.mainBackSplitContainer.Panel1.SuspendLayout();
            this.mainBackSplitContainer.Panel2.SuspendLayout();
            this.mainBackSplitContainer.SuspendLayout();
            this.headerPanel.SuspendLayout();
            this.contextMenuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // mainBackPanel
            // 
            this.mainBackPanel.Controls.Add(this.mainBackSplitContainer);
            this.mainBackPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mainBackPanel.Location = new System.Drawing.Point(0, 0);
            this.mainBackPanel.Name = "mainBackPanel";
            this.mainBackPanel.Size = new System.Drawing.Size(538, 196);
            this.mainBackPanel.TabIndex = 0;
            // 
            // mainBackSplitContainer
            // 
            this.mainBackSplitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mainBackSplitContainer.Location = new System.Drawing.Point(0, 0);
            this.mainBackSplitContainer.Name = "mainBackSplitContainer";
            // 
            // mainBackSplitContainer.Panel1
            // 
            this.mainBackSplitContainer.Panel1.Controls.Add(this.headerPanel);
            // 
            // mainBackSplitContainer.Panel2
            // 
            this.mainBackSplitContainer.Panel2.Controls.Add(this.activitySlotLView);
            this.mainBackSplitContainer.Size = new System.Drawing.Size(538, 196);
            this.mainBackSplitContainer.SplitterDistance = 244;
            this.mainBackSplitContainer.TabIndex = 0;
            // 
            // headerPanel
            // 
            this.headerPanel.ContextMenuStrip = this.contextMenuStrip1;
            this.headerPanel.Controls.Add(this.historyLabel);
            this.headerPanel.Controls.Add(this.historyTitleLabel);
            this.headerPanel.Controls.Add(this.keyVersionLabel);
            this.headerPanel.Controls.Add(this.keyVersionTitleLabel);
            this.headerPanel.Controls.Add(this.lastTouchDateLabel);
            this.headerPanel.Controls.Add(this.modifiedDateLabel);
            this.headerPanel.Controls.Add(this.verificationCodeLabel);
            this.headerPanel.Controls.Add(this.keyNameLabel);
            this.headerPanel.Controls.Add(this.lastTouchDateTitleLabel);
            this.headerPanel.Controls.Add(this.modifiedDateTitlelabel);
            this.headerPanel.Controls.Add(this.verificationCodeTitleLabel);
            this.headerPanel.Controls.Add(this.keyNameTitleLabel);
            this.headerPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.headerPanel.Location = new System.Drawing.Point(0, 0);
            this.headerPanel.Name = "headerPanel";
            this.headerPanel.Size = new System.Drawing.Size(244, 196);
            this.headerPanel.TabIndex = 1;
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
            // historyLabel
            // 
            this.historyLabel.AutoSize = true;
            this.historyLabel.Location = new System.Drawing.Point(68, 26);
            this.historyLabel.Name = "historyLabel";
            this.historyLabel.Size = new System.Drawing.Size(82, 13);
            this.historyLabel.TabIndex = 11;
            this.historyLabel.Text = "[HistoryNumber]";
            // 
            // historyTitleLabel
            // 
            this.historyTitleLabel.AutoSize = true;
            this.historyTitleLabel.Location = new System.Drawing.Point(3, 26);
            this.historyTitleLabel.Name = "historyTitleLabel";
            this.historyTitleLabel.Size = new System.Drawing.Size(49, 13);
            this.historyTitleLabel.TabIndex = 10;
            this.historyTitleLabel.Text = "History#:";
            // 
            // keyVersionLabel
            // 
            this.keyVersionLabel.AutoSize = true;
            this.keyVersionLabel.Location = new System.Drawing.Point(68, 13);
            this.keyVersionLabel.Name = "keyVersionLabel";
            this.keyVersionLabel.Size = new System.Drawing.Size(66, 13);
            this.keyVersionLabel.TabIndex = 9;
            this.keyVersionLabel.Text = "[KeyVersion]";
            // 
            // keyVersionTitleLabel
            // 
            this.keyVersionTitleLabel.AutoSize = true;
            this.keyVersionTitleLabel.Location = new System.Drawing.Point(3, 13);
            this.keyVersionTitleLabel.Name = "keyVersionTitleLabel";
            this.keyVersionTitleLabel.Size = new System.Drawing.Size(66, 13);
            this.keyVersionTitleLabel.TabIndex = 8;
            this.keyVersionTitleLabel.Text = "Key Version:";
            // 
            // lastTouchDateLabel
            // 
            this.lastTouchDateLabel.AutoSize = true;
            this.lastTouchDateLabel.Location = new System.Drawing.Point(3, 123);
            this.lastTouchDateLabel.Name = "lastTouchDateLabel";
            this.lastTouchDateLabel.Size = new System.Drawing.Size(87, 13);
            this.lastTouchDateLabel.TabIndex = 7;
            this.lastTouchDateLabel.Text = "[LastTouchDate]";
            // 
            // modifiedDateLabel
            // 
            this.modifiedDateLabel.AutoSize = true;
            this.modifiedDateLabel.Location = new System.Drawing.Point(3, 88);
            this.modifiedDateLabel.Name = "modifiedDateLabel";
            this.modifiedDateLabel.Size = new System.Drawing.Size(109, 13);
            this.modifiedDateLabel.TabIndex = 6;
            this.modifiedDateLabel.Text = "[PacketCreationDate]";
            // 
            // verificationCodeLabel
            // 
            this.verificationCodeLabel.AutoSize = true;
            this.verificationCodeLabel.Location = new System.Drawing.Point(3, 52);
            this.verificationCodeLabel.Name = "verificationCodeLabel";
            this.verificationCodeLabel.Size = new System.Drawing.Size(90, 13);
            this.verificationCodeLabel.TabIndex = 5;
            this.verificationCodeLabel.Text = "[VerificationCode]";
            // 
            // keyNameLabel
            // 
            this.keyNameLabel.AutoSize = true;
            this.keyNameLabel.Location = new System.Drawing.Point(68, 0);
            this.keyNameLabel.Name = "keyNameLabel";
            this.keyNameLabel.Size = new System.Drawing.Size(59, 13);
            this.keyNameLabel.TabIndex = 4;
            this.keyNameLabel.Text = "[KeyName]";
            // 
            // lastTouchDateTitleLabel
            // 
            this.lastTouchDateTitleLabel.AutoSize = true;
            this.lastTouchDateTitleLabel.Location = new System.Drawing.Point(3, 110);
            this.lastTouchDateTitleLabel.Name = "lastTouchDateTitleLabel";
            this.lastTouchDateTitleLabel.Size = new System.Drawing.Size(90, 13);
            this.lastTouchDateTitleLabel.TabIndex = 3;
            this.lastTouchDateTitleLabel.Text = "Last Touch Date:";
            // 
            // modifiedDateTitlelabel
            // 
            this.modifiedDateTitlelabel.AutoSize = true;
            this.modifiedDateTitlelabel.Location = new System.Drawing.Point(3, 75);
            this.modifiedDateTitlelabel.Name = "modifiedDateTitlelabel";
            this.modifiedDateTitlelabel.Size = new System.Drawing.Size(112, 13);
            this.modifiedDateTitlelabel.TabIndex = 2;
            this.modifiedDateTitlelabel.Text = "Packet Creation Date:";
            // 
            // verificationCodeTitleLabel
            // 
            this.verificationCodeTitleLabel.AutoSize = true;
            this.verificationCodeTitleLabel.Location = new System.Drawing.Point(3, 39);
            this.verificationCodeTitleLabel.Name = "verificationCodeTitleLabel";
            this.verificationCodeTitleLabel.Size = new System.Drawing.Size(90, 13);
            this.verificationCodeTitleLabel.TabIndex = 1;
            this.verificationCodeTitleLabel.Text = "Verification Code:";
            // 
            // keyNameTitleLabel
            // 
            this.keyNameTitleLabel.AutoSize = true;
            this.keyNameTitleLabel.Location = new System.Drawing.Point(3, 0);
            this.keyNameTitleLabel.Name = "keyNameTitleLabel";
            this.keyNameTitleLabel.Size = new System.Drawing.Size(59, 13);
            this.keyNameTitleLabel.TabIndex = 0;
            this.keyNameTitleLabel.Text = "Key Name:";
            // 
            // activitySlotLView
            // 
            this.activitySlotLView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader9,
            this.columnHeader10,
            this.columnHeader11,
            this.columnHeader1,
            this.columnHeader12});
            this.activitySlotLView.ContextMenuStrip = this.contextMenuStrip1;
            this.activitySlotLView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.activitySlotLView.FullRowSelect = true;
            this.activitySlotLView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.activitySlotLView.HideSelection = false;
            this.activitySlotLView.Location = new System.Drawing.Point(0, 0);
            this.activitySlotLView.Name = "activitySlotLView";
            this.activitySlotLView.Size = new System.Drawing.Size(290, 196);
            this.activitySlotLView.TabIndex = 1;
            this.activitySlotLView.UseCompatibleStateImageBehavior = false;
            this.activitySlotLView.View = System.Windows.Forms.View.Details;
            this.activitySlotLView.KeyDown += new System.Windows.Forms.KeyEventHandler(this.general_KeyDown);
            // 
            // columnHeader9
            // 
            this.columnHeader9.Text = "Slot";
            this.columnHeader9.Width = 88;
            // 
            // columnHeader10
            // 
            this.columnHeader10.Text = "Activation#";
            this.columnHeader10.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.columnHeader10.Width = 98;
            // 
            // columnHeader11
            // 
            this.columnHeader11.Text = "Hours Left";
            this.columnHeader11.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.columnHeader11.Width = 92;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Expiration Date";
            // 
            // columnHeader12
            // 
            this.columnHeader12.Text = "";
            this.columnHeader12.Width = 0;
            // 
            // Lic_KeyAttribs_DisplayControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.mainBackPanel);
            this.Name = "Lic_KeyAttribs_DisplayControl";
            this.Size = new System.Drawing.Size(538, 196);
            this.mainBackPanel.ResumeLayout(false);
            this.mainBackSplitContainer.Panel1.ResumeLayout(false);
            this.mainBackSplitContainer.Panel2.ResumeLayout(false);
            this.mainBackSplitContainer.ResumeLayout(false);
            this.headerPanel.ResumeLayout(false);
            this.headerPanel.PerformLayout();
            this.contextMenuStrip1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel mainBackPanel;
        private System.Windows.Forms.SplitContainer mainBackSplitContainer;
        private System.Windows.Forms.Panel headerPanel;
        private System.Windows.Forms.Label lastTouchDateLabel;
        private System.Windows.Forms.Label modifiedDateLabel;
        private System.Windows.Forms.Label verificationCodeLabel;
        private System.Windows.Forms.Label keyNameLabel;
        private System.Windows.Forms.Label lastTouchDateTitleLabel;
        private System.Windows.Forms.Label modifiedDateTitlelabel;
        private System.Windows.Forms.Label verificationCodeTitleLabel;
        private System.Windows.Forms.Label keyNameTitleLabel;
        private Shared.VisualComponents.NoFlickerListView activitySlotLView;
        private System.Windows.Forms.ColumnHeader columnHeader9;
        private System.Windows.Forms.ColumnHeader columnHeader10;
        private System.Windows.Forms.ColumnHeader columnHeader11;
        private System.Windows.Forms.ColumnHeader columnHeader12;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
        private System.Windows.Forms.ToolStripMenuItem copyToolStripMenuItem;
        private System.Windows.Forms.Label keyVersionLabel;
        private System.Windows.Forms.Label keyVersionTitleLabel;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.Label historyLabel;
        private System.Windows.Forms.Label historyTitleLabel;
    }
}
