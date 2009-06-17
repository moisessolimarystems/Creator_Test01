namespace SolimarLicenseViewer
{
    partial class LicenseUpdateHelper
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
            this.MainPanel = new System.Windows.Forms.Panel();
            this.licSelectionListView = new System.Windows.Forms.ListView();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader3 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
            this.topHeaderPanel = new System.Windows.Forms.Panel();
            this.refreshButton = new System.Windows.Forms.Button();
            this.directoryLabelTitle = new System.Windows.Forms.Label();
            this.directoryFindButton = new System.Windows.Forms.Button();
            this.directoryTextBox = new System.Windows.Forms.TextBox();
            this.folderBrowserDialog = new System.Windows.Forms.FolderBrowserDialog();
            this.MainPanel.SuspendLayout();
            this.topHeaderPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnCancel
            // 
            this.btnCancel.TabIndex = 1;
            // 
            // btnOk
            // 
            this.btnOk.TabIndex = 0;
            this.btnOk.Text = "Apply";
            this.btnOk.Click += new System.EventHandler(this.btnOk_Click);
            // 
            // MainPanel
            // 
            this.MainPanel.Controls.Add(this.licSelectionListView);
            this.MainPanel.Controls.Add(this.topHeaderPanel);
            this.MainPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.MainPanel.Location = new System.Drawing.Point(0, 0);
            this.MainPanel.Name = "MainPanel";
            this.MainPanel.Size = new System.Drawing.Size(754, 401);
            this.MainPanel.TabIndex = 0;
            // 
            // licSelectionListView
            // 
            this.licSelectionListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader3,
            this.columnHeader2});
            this.licSelectionListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.licSelectionListView.FullRowSelect = true;
            this.licSelectionListView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.licSelectionListView.HideSelection = false;
            this.licSelectionListView.Location = new System.Drawing.Point(0, 59);
            this.licSelectionListView.MultiSelect = false;
            this.licSelectionListView.Name = "licSelectionListView";
            this.licSelectionListView.Size = new System.Drawing.Size(754, 342);
            this.licSelectionListView.Sorting = System.Windows.Forms.SortOrder.Ascending;
            this.licSelectionListView.TabIndex = 1;
            this.licSelectionListView.UseCompatibleStateImageBehavior = false;
            this.licSelectionListView.View = System.Windows.Forms.View.Details;
            this.licSelectionListView.SelectedIndexChanged += new System.EventHandler(this.licSelectionListView_SelectedIndexChanged);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Software License";
            this.columnHeader1.Width = 152;
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "Modified Date";
            this.columnHeader3.Width = 165;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Status";
            this.columnHeader2.Width = 422;
            // 
            // topHeaderPanel
            // 
            this.topHeaderPanel.Controls.Add(this.refreshButton);
            this.topHeaderPanel.Controls.Add(this.directoryLabelTitle);
            this.topHeaderPanel.Controls.Add(this.directoryFindButton);
            this.topHeaderPanel.Controls.Add(this.directoryTextBox);
            this.topHeaderPanel.Dock = System.Windows.Forms.DockStyle.Top;
            this.topHeaderPanel.Location = new System.Drawing.Point(0, 0);
            this.topHeaderPanel.Name = "topHeaderPanel";
            this.topHeaderPanel.Size = new System.Drawing.Size(754, 59);
            this.topHeaderPanel.TabIndex = 0;
            // 
            // refreshButton
            // 
            this.refreshButton.Location = new System.Drawing.Point(6, 30);
            this.refreshButton.Name = "refreshButton";
            this.refreshButton.Size = new System.Drawing.Size(75, 23);
            this.refreshButton.TabIndex = 2;
            this.refreshButton.Text = "Refresh";
            this.refreshButton.UseVisualStyleBackColor = true;
            this.refreshButton.Click += new System.EventHandler(this.refreshButton_Click);
            // 
            // directoryLabelTitle
            // 
            this.directoryLabelTitle.AutoSize = true;
            this.directoryLabelTitle.Location = new System.Drawing.Point(3, 9);
            this.directoryLabelTitle.Name = "directoryLabelTitle";
            this.directoryLabelTitle.Size = new System.Drawing.Size(147, 13);
            this.directoryLabelTitle.TabIndex = 6;
            this.directoryLabelTitle.Text = "Choose a directory to Search:";
            // 
            // directoryFindButton
            // 
            this.directoryFindButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.directoryFindButton.Location = new System.Drawing.Point(727, 4);
            this.directoryFindButton.Name = "directoryFindButton";
            this.directoryFindButton.Size = new System.Drawing.Size(24, 23);
            this.directoryFindButton.TabIndex = 1;
            this.directoryFindButton.Text = "...";
            this.directoryFindButton.UseVisualStyleBackColor = true;
            this.directoryFindButton.Click += new System.EventHandler(this.directoryFindButton_Click);
            // 
            // directoryTextBox
            // 
            this.directoryTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.directoryTextBox.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.Suggest;
            this.directoryTextBox.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.FileSystemDirectories;
            this.directoryTextBox.Location = new System.Drawing.Point(156, 6);
            this.directoryTextBox.Name = "directoryTextBox";
            this.directoryTextBox.Size = new System.Drawing.Size(565, 20);
            this.directoryTextBox.TabIndex = 0;
            this.directoryTextBox.KeyDown += new System.Windows.Forms.KeyEventHandler(this.directoryTextBox_KeyDown);
            // 
            // LicenseUpdateHelper
            // 
            this.AcceptButton = null;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(754, 432);
            this.Controls.Add(this.MainPanel);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Sizable;
            this.Name = "LicenseUpdateHelper";
            this.ShowIcon = false;
            this.Text = "License Update Helper";
            this.Controls.SetChildIndex(this.MainPanel, 0);
            this.MainPanel.ResumeLayout(false);
            this.topHeaderPanel.ResumeLayout(false);
            this.topHeaderPanel.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel MainPanel;
        private System.Windows.Forms.ListView licSelectionListView;
        private System.Windows.Forms.Panel topHeaderPanel;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.Button directoryFindButton;
        private System.Windows.Forms.TextBox directoryTextBox;
        private System.Windows.Forms.Label directoryLabelTitle;
        private System.Windows.Forms.FolderBrowserDialog folderBrowserDialog;
        private System.Windows.Forms.Button refreshButton;
        private System.Windows.Forms.ColumnHeader columnHeader3;


    }
}