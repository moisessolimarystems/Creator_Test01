namespace SolimarLicenseViewer
{
    partial class ProductSettingsDialog
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
            this.MiddlePanel = new System.Windows.Forms.Panel();
            this.TestPanel = new System.Windows.Forms.Panel();
            this.multiProductListView = new System.Windows.Forms.ListView();
            this.ProductCH = new System.Windows.Forms.ColumnHeader();
            this.LicServerCH = new System.Windows.Forms.ColumnHeader();
            this.BackupLicServerCH = new System.Windows.Forms.ColumnHeader();
            this.TestDevCH = new System.Windows.Forms.ColumnHeader();
            this.singleProdPropertyGrid = new System.Windows.Forms.PropertyGrid();
            this.MiddlePanel.SuspendLayout();
            this.TestPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnOk
            // 
            this.btnOk.Text = "Save";
            this.btnOk.Click += new System.EventHandler(this.btnOk_Click);
            // 
            // MiddlePanel
            // 
            this.MiddlePanel.Controls.Add(this.TestPanel);
            this.MiddlePanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.MiddlePanel.Location = new System.Drawing.Point(0, 0);
            this.MiddlePanel.Name = "MiddlePanel";
            this.MiddlePanel.Size = new System.Drawing.Size(1018, 258);
            this.MiddlePanel.TabIndex = 3;
            // 
            // TestPanel
            // 
            this.TestPanel.AutoScroll = true;
            this.TestPanel.Controls.Add(this.multiProductListView);
            this.TestPanel.Controls.Add(this.singleProdPropertyGrid);
            this.TestPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.TestPanel.Location = new System.Drawing.Point(0, 0);
            this.TestPanel.Name = "TestPanel";
            this.TestPanel.Padding = new System.Windows.Forms.Padding(3);
            this.TestPanel.Size = new System.Drawing.Size(1018, 258);
            this.TestPanel.TabIndex = 2;
            // 
            // multiProductListView
            // 
            this.multiProductListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.ProductCH,
            this.LicServerCH,
            this.BackupLicServerCH,
            this.TestDevCH});
            this.multiProductListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.multiProductListView.FullRowSelect = true;
            this.multiProductListView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.multiProductListView.HideSelection = false;
            this.multiProductListView.Location = new System.Drawing.Point(3, 3);
            this.multiProductListView.Name = "multiProductListView";
            this.multiProductListView.Size = new System.Drawing.Size(737, 252);
            this.multiProductListView.Sorting = System.Windows.Forms.SortOrder.Ascending;
            this.multiProductListView.TabIndex = 0;
            this.multiProductListView.UseCompatibleStateImageBehavior = false;
            this.multiProductListView.View = System.Windows.Forms.View.Details;
            this.multiProductListView.SelectedIndexChanged += new System.EventHandler(this.multiProductListView_SelectedIndexChanged);
            // 
            // ProductCH
            // 
            this.ProductCH.Text = "Product";
            this.ProductCH.Width = 250;
            // 
            // LicServerCH
            // 
            this.LicServerCH.Text = "Primary License Server";
            this.LicServerCH.Width = 158;
            // 
            // BackupLicServerCH
            // 
            this.BackupLicServerCH.Text = "Failover License Server";
            this.BackupLicServerCH.Width = 132;
            // 
            // TestDevCH
            // 
            this.TestDevCH.Text = "Use Development Licensing";
            this.TestDevCH.Width = 149;
            // 
            // singleProdPropertyGrid
            // 
            this.singleProdPropertyGrid.Dock = System.Windows.Forms.DockStyle.Right;
            this.singleProdPropertyGrid.Location = new System.Drawing.Point(740, 3);
            this.singleProdPropertyGrid.Name = "singleProdPropertyGrid";
            this.singleProdPropertyGrid.PropertySort = System.Windows.Forms.PropertySort.NoSort;
            this.singleProdPropertyGrid.Size = new System.Drawing.Size(275, 252);
            this.singleProdPropertyGrid.TabIndex = 1;
            this.singleProdPropertyGrid.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(this.singleProdPropertyGrid_PropertyValueChanged);
            // 
            // ProductSettingsDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.ClientSize = new System.Drawing.Size(1018, 289);
            this.Controls.Add(this.MiddlePanel);
            this.Name = "ProductSettingsDialog";
            this.Text = "Product Connection Settings";
            this.Load += new System.EventHandler(this.ProductSettingsDialog_Load);
            this.Controls.SetChildIndex(this.MiddlePanel, 0);
            this.MiddlePanel.ResumeLayout(false);
            this.TestPanel.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel MiddlePanel;
        private System.Windows.Forms.Panel TestPanel;
        private System.Windows.Forms.ListView multiProductListView;
        private System.Windows.Forms.ColumnHeader ProductCH;
        private System.Windows.Forms.ColumnHeader LicServerCH;
        private System.Windows.Forms.ColumnHeader BackupLicServerCH;
        private System.Windows.Forms.ColumnHeader TestDevCH;
        private System.Windows.Forms.PropertyGrid singleProdPropertyGrid;
    }
}
