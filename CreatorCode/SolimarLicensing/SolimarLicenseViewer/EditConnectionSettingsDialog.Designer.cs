namespace SolimarLicenseViewer
{
    partial class EditConnectionSettingsDialog
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
            this.singleProdPropertyGrid = new System.Windows.Forms.PropertyGrid();
            this.mainPanel = new System.Windows.Forms.Panel();
            this.headerPanel = new System.Windows.Forms.Panel();
            this.warningLabel = new System.Windows.Forms.Label();
            this.mainPanel.SuspendLayout();
            this.headerPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // singleProdPropertyGrid
            // 
            this.singleProdPropertyGrid.Dock = System.Windows.Forms.DockStyle.Fill;
            this.singleProdPropertyGrid.Location = new System.Drawing.Point(0, 0);
            this.singleProdPropertyGrid.Name = "singleProdPropertyGrid";
            this.singleProdPropertyGrid.PropertySort = System.Windows.Forms.PropertySort.NoSort;
            this.singleProdPropertyGrid.Size = new System.Drawing.Size(396, 210);
            this.singleProdPropertyGrid.TabIndex = 3;
            this.singleProdPropertyGrid.PreviewKeyDown += new System.Windows.Forms.PreviewKeyDownEventHandler(this.singleProdPropertyGrid_PreviewKeyDown);
            // 
            // mainPanel
            // 
            this.mainPanel.Controls.Add(this.singleProdPropertyGrid);
            this.mainPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mainPanel.Location = new System.Drawing.Point(0, 33);
            this.mainPanel.Name = "mainPanel";
            this.mainPanel.Size = new System.Drawing.Size(396, 210);
            this.mainPanel.TabIndex = 4;
            // 
            // headerPanel
            // 
            this.headerPanel.Controls.Add(this.warningLabel);
            this.headerPanel.Dock = System.Windows.Forms.DockStyle.Top;
            this.headerPanel.Location = new System.Drawing.Point(0, 0);
            this.headerPanel.Name = "headerPanel";
            this.headerPanel.Padding = new System.Windows.Forms.Padding(3);
            this.headerPanel.Size = new System.Drawing.Size(396, 33);
            this.headerPanel.TabIndex = 5;
            // 
            // warningLabel
            // 
            this.warningLabel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.warningLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.warningLabel.Location = new System.Drawing.Point(3, 3);
            this.warningLabel.Name = "warningLabel";
            this.warningLabel.Size = new System.Drawing.Size(390, 27);
            this.warningLabel.TabIndex = 0;
            this.warningLabel.Text = "Warning: Any License Setting change will not take effect until the product is res" +
                "tarted.";
            // 
            // EditConnectionSettingsDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(396, 274);
            this.Controls.Add(this.mainPanel);
            this.Controls.Add(this.headerPanel);
            this.Name = "EditConnectionSettingsDialog";
            this.Text = "Edit Connection Settings";
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.General_KeyDown);
            this.Controls.SetChildIndex(this.headerPanel, 0);
            this.Controls.SetChildIndex(this.mainPanel, 0);
            this.mainPanel.ResumeLayout(false);
            this.headerPanel.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PropertyGrid singleProdPropertyGrid;
        private System.Windows.Forms.Panel mainPanel;
        private System.Windows.Forms.Panel headerPanel;
        private System.Windows.Forms.Label warningLabel;
    }
}