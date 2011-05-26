namespace Client.Creator
{
    partial class ProductConfigurationDialog
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
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ProductConfigurationDialog));
            this.optionsTabControl = new System.Windows.Forms.TabControl();
            this.versionTabPage = new System.Windows.Forms.TabPage();
            this.productVersionDataGridView = new System.Windows.Forms.DataGridView();
            this.tokenTabPage = new System.Windows.Forms.TabPage();
            this.removeAllButton = new System.Windows.Forms.Button();
            this.removeButton = new System.Windows.Forms.Button();
            this.addAllButton = new System.Windows.Forms.Button();
            this.addButton = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.selectedListView = new System.Windows.Forms.ListView();
            this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
            this.availableListView = new System.Windows.Forms.ListView();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.optionsTabControl.SuspendLayout();
            this.versionTabPage.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.productVersionDataGridView)).BeginInit();
            this.tokenTabPage.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnCancel
            // 
            this.btnCancel.Location = new System.Drawing.Point(98, 3);
            // 
            // btnOk
            // 
            this.btnOk.Location = new System.Drawing.Point(19, 3);
            this.btnOk.Click += new System.EventHandler(this.btnOk_Click);
            // 
            // optionsTabControl
            // 
            this.optionsTabControl.Controls.Add(this.versionTabPage);
            this.optionsTabControl.Controls.Add(this.tokenTabPage);
            this.optionsTabControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.optionsTabControl.Location = new System.Drawing.Point(8, 8);
            this.optionsTabControl.Name = "optionsTabControl";
            this.optionsTabControl.SelectedIndex = 0;
            this.optionsTabControl.Size = new System.Drawing.Size(404, 270);
            this.optionsTabControl.TabIndex = 6;
            this.optionsTabControl.SelectedIndexChanged += new System.EventHandler(this.optionsTabControl_SelectedIndexChanged);
            // 
            // versionTabPage
            // 
            this.versionTabPage.Controls.Add(this.productVersionDataGridView);
            this.versionTabPage.Location = new System.Drawing.Point(4, 22);
            this.versionTabPage.Name = "versionTabPage";
            this.versionTabPage.Size = new System.Drawing.Size(396, 244);
            this.versionTabPage.TabIndex = 2;
            this.versionTabPage.Text = "Product Versions";
            this.versionTabPage.UseVisualStyleBackColor = true;
            // 
            // productVersionDataGridView
            // 
            this.productVersionDataGridView.AllowUserToAddRows = false;
            this.productVersionDataGridView.AllowUserToDeleteRows = false;
            this.productVersionDataGridView.AllowUserToResizeColumns = false;
            this.productVersionDataGridView.AllowUserToResizeRows = false;
            dataGridViewCellStyle1.BackColor = System.Drawing.Color.AliceBlue;
            this.productVersionDataGridView.AlternatingRowsDefaultCellStyle = dataGridViewCellStyle1;
            this.productVersionDataGridView.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.Fill;
            this.productVersionDataGridView.BackgroundColor = System.Drawing.SystemColors.Window;
            this.productVersionDataGridView.CellBorderStyle = System.Windows.Forms.DataGridViewCellBorderStyle.None;
            this.productVersionDataGridView.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.productVersionDataGridView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.productVersionDataGridView.Location = new System.Drawing.Point(0, 0);
            this.productVersionDataGridView.MultiSelect = false;
            this.productVersionDataGridView.Name = "productVersionDataGridView";
            this.productVersionDataGridView.RowHeadersVisible = false;
            this.productVersionDataGridView.RowTemplate.Height = 16;
            this.productVersionDataGridView.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.productVersionDataGridView.Size = new System.Drawing.Size(396, 244);
            this.productVersionDataGridView.TabIndex = 0;
            this.productVersionDataGridView.DataError += new System.Windows.Forms.DataGridViewDataErrorEventHandler(this.productVersionDataGridView_DataError);
            // 
            // tokenTabPage
            // 
            this.tokenTabPage.Controls.Add(this.removeAllButton);
            this.tokenTabPage.Controls.Add(this.removeButton);
            this.tokenTabPage.Controls.Add(this.addAllButton);
            this.tokenTabPage.Controls.Add(this.addButton);
            this.tokenTabPage.Controls.Add(this.label3);
            this.tokenTabPage.Controls.Add(this.label1);
            this.tokenTabPage.Controls.Add(this.selectedListView);
            this.tokenTabPage.Controls.Add(this.availableListView);
            this.tokenTabPage.Location = new System.Drawing.Point(4, 23);
            this.tokenTabPage.Name = "tokenTabPage";
            this.tokenTabPage.Padding = new System.Windows.Forms.Padding(3);
            this.tokenTabPage.Size = new System.Drawing.Size(396, 243);
            this.tokenTabPage.TabIndex = 0;
            this.tokenTabPage.Text = "Software Tokens";
            this.tokenTabPage.UseVisualStyleBackColor = true;
            // 
            // removeAllButton
            // 
            this.removeAllButton.Image = ((System.Drawing.Image)(resources.GetObject("removeAllButton.Image")));
            this.removeAllButton.Location = new System.Drawing.Point(186, 124);
            this.removeAllButton.Name = "removeAllButton";
            this.removeAllButton.Size = new System.Drawing.Size(24, 24);
            this.removeAllButton.TabIndex = 16;
            this.removeAllButton.UseVisualStyleBackColor = true;
            this.removeAllButton.Click += new System.EventHandler(this.removeAllButton_Click);
            // 
            // removeButton
            // 
            this.removeButton.Image = ((System.Drawing.Image)(resources.GetObject("removeButton.Image")));
            this.removeButton.Location = new System.Drawing.Point(186, 93);
            this.removeButton.Name = "removeButton";
            this.removeButton.Size = new System.Drawing.Size(24, 24);
            this.removeButton.TabIndex = 15;
            this.removeButton.UseVisualStyleBackColor = true;
            this.removeButton.Click += new System.EventHandler(this.removeButton_Click);
            // 
            // addAllButton
            // 
            this.addAllButton.Image = ((System.Drawing.Image)(resources.GetObject("addAllButton.Image")));
            this.addAllButton.Location = new System.Drawing.Point(186, 62);
            this.addAllButton.Name = "addAllButton";
            this.addAllButton.Size = new System.Drawing.Size(24, 24);
            this.addAllButton.TabIndex = 14;
            this.addAllButton.UseVisualStyleBackColor = true;
            this.addAllButton.Click += new System.EventHandler(this.addAllButton_Click);
            // 
            // addButton
            // 
            this.addButton.Image = ((System.Drawing.Image)(resources.GetObject("addButton.Image")));
            this.addButton.Location = new System.Drawing.Point(186, 31);
            this.addButton.Name = "addButton";
            this.addButton.Size = new System.Drawing.Size(24, 24);
            this.addButton.TabIndex = 13;
            this.addButton.UseVisualStyleBackColor = true;
            this.addButton.Click += new System.EventHandler(this.addButton_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(219, 15);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(49, 13);
            this.label3.TabIndex = 12;
            this.label3.Text = "&Selected";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(18, 15);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(50, 13);
            this.label1.TabIndex = 11;
            this.label1.Text = "&Available";
            // 
            // selectedListView
            // 
            this.selectedListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader2});
            this.selectedListView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            this.selectedListView.Location = new System.Drawing.Point(222, 31);
            this.selectedListView.Name = "selectedListView";
            this.selectedListView.Size = new System.Drawing.Size(153, 197);
            this.selectedListView.TabIndex = 10;
            this.selectedListView.UseCompatibleStateImageBehavior = false;
            this.selectedListView.View = System.Windows.Forms.View.Details;
            this.selectedListView.SelectedIndexChanged += new System.EventHandler(this.selectedListView_SelectedIndexChanged);
            // 
            // columnHeader2
            // 
            this.columnHeader2.Width = 149;
            // 
            // availableListView
            // 
            this.availableListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1});
            this.availableListView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            this.availableListView.Location = new System.Drawing.Point(21, 31);
            this.availableListView.Name = "availableListView";
            this.availableListView.Size = new System.Drawing.Size(153, 197);
            this.availableListView.TabIndex = 9;
            this.availableListView.UseCompatibleStateImageBehavior = false;
            this.availableListView.View = System.Windows.Forms.View.Details;
            this.availableListView.SelectedIndexChanged += new System.EventHandler(this.availableListView_SelectedIndexChanged);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Width = 149;
            // 
            // ProductConfigurationDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.ClientSize = new System.Drawing.Size(420, 309);
            this.Controls.Add(this.optionsTabControl);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "ProductConfigurationDialog";
            this.Padding = new System.Windows.Forms.Padding(8, 8, 8, 0);
            this.Text = "Options";
            this.Controls.SetChildIndex(this.optionsTabControl, 0);
            this.optionsTabControl.ResumeLayout(false);
            this.versionTabPage.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.productVersionDataGridView)).EndInit();
            this.tokenTabPage.ResumeLayout(false);
            this.tokenTabPage.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl optionsTabControl;
        private System.Windows.Forms.TabPage tokenTabPage;
        private System.Windows.Forms.TabPage versionTabPage;
        private System.Windows.Forms.DataGridView productVersionDataGridView;
        private System.Windows.Forms.Button removeAllButton;
        private System.Windows.Forms.Button removeButton;
        private System.Windows.Forms.Button addAllButton;
        private System.Windows.Forms.Button addButton;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ListView selectedListView;
        private System.Windows.Forms.ListView availableListView;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.Label label3;


    }
}
