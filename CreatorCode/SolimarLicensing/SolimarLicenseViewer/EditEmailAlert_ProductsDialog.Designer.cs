namespace SolimarLicenseViewer
{
    partial class EditEmailAlert_ProductsDialog
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
         this.productsListView = new Shared.VisualComponents.NoFlickerListView();
         this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
         this.productsToolStrip = new System.Windows.Forms.ToolStrip();
         this.productsCheckAllTSButton = new System.Windows.Forms.ToolStripButton();
         this.productsUncheckAllTSButton = new System.Windows.Forms.ToolStripButton();
         this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
         this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
         this.productsFilterTSComboBox = new System.Windows.Forms.ToolStripComboBox();
         this.backPanel.SuspendLayout();
         this.productsToolStrip.SuspendLayout();
         this.SuspendLayout();
         // 
         // backPanel
         // 
         this.backPanel.Controls.Add(this.productsListView);
         this.backPanel.Controls.Add(this.productsToolStrip);
         this.backPanel.Dock = System.Windows.Forms.DockStyle.Fill;
         this.backPanel.Location = new System.Drawing.Point(0, 0);
         this.backPanel.Name = "backPanel";
         this.backPanel.Padding = new System.Windows.Forms.Padding(5, 0, 5, 0);
         this.backPanel.Size = new System.Drawing.Size(406, 326);
         this.backPanel.TabIndex = 5;
         // 
         // productsListView
         // 
         this.productsListView.CheckBoxes = true;
         this.productsListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1});
         this.productsListView.Dock = System.Windows.Forms.DockStyle.Fill;
         this.productsListView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
         this.productsListView.Location = new System.Drawing.Point(5, 25);
         this.productsListView.Name = "productsListView";
         this.productsListView.Size = new System.Drawing.Size(396, 301);
         this.productsListView.Sorting = System.Windows.Forms.SortOrder.Ascending;
         this.productsListView.TabIndex = 1;
         this.productsListView.UseCompatibleStateImageBehavior = false;
         this.productsListView.View = System.Windows.Forms.View.Details;
         this.productsListView.KeyDown += new System.Windows.Forms.KeyEventHandler(this.productsListView_KeyDown);
         // 
         // columnHeader1
         // 
         this.columnHeader1.Width = 353;
         // 
         // productsToolStrip
         // 
         this.productsToolStrip.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
         this.productsToolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.productsCheckAllTSButton,
            this.productsUncheckAllTSButton,
            this.toolStripSeparator1,
            this.toolStripLabel1,
            this.productsFilterTSComboBox});
         this.productsToolStrip.Location = new System.Drawing.Point(5, 0);
         this.productsToolStrip.Name = "productsToolStrip";
         this.productsToolStrip.Size = new System.Drawing.Size(396, 25);
         this.productsToolStrip.TabIndex = 5;
         this.productsToolStrip.Text = "toolStrip1";
         // 
         // productsCheckAllTSButton
         // 
         this.productsCheckAllTSButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
         this.productsCheckAllTSButton.Image = global::SolimarLicenseViewer.Properties.Resources.checked_all;
         this.productsCheckAllTSButton.ImageTransparentColor = System.Drawing.Color.Magenta;
         this.productsCheckAllTSButton.Name = "productsCheckAllTSButton";
         this.productsCheckAllTSButton.Size = new System.Drawing.Size(23, 22);
         this.productsCheckAllTSButton.Text = "Check All";
         this.productsCheckAllTSButton.Click += new System.EventHandler(this.productsCheckAllTSButton_Click);
         // 
         // productsUncheckAllTSButton
         // 
         this.productsUncheckAllTSButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
         this.productsUncheckAllTSButton.Image = global::SolimarLicenseViewer.Properties.Resources.check_all;
         this.productsUncheckAllTSButton.ImageTransparentColor = System.Drawing.Color.Magenta;
         this.productsUncheckAllTSButton.Name = "productsUncheckAllTSButton";
         this.productsUncheckAllTSButton.Size = new System.Drawing.Size(23, 22);
         this.productsUncheckAllTSButton.Text = "Uncheck All";
         this.productsUncheckAllTSButton.Click += new System.EventHandler(this.productsUncheckAllTSButton_Click);
         // 
         // toolStripSeparator1
         // 
         this.toolStripSeparator1.Name = "toolStripSeparator1";
         this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
         this.toolStripSeparator1.Visible = false;
         // 
         // toolStripLabel1
         // 
         this.toolStripLabel1.Name = "toolStripLabel1";
         this.toolStripLabel1.Size = new System.Drawing.Size(36, 22);
         this.toolStripLabel1.Text = "Filter:";
         this.toolStripLabel1.Visible = false;
         // 
         // productsFilterTSComboBox
         // 
         this.productsFilterTSComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
         this.productsFilterTSComboBox.Name = "productsFilterTSComboBox";
         this.productsFilterTSComboBox.Size = new System.Drawing.Size(121, 25);
         this.productsFilterTSComboBox.Visible = false;
         this.productsFilterTSComboBox.SelectedIndexChanged += new System.EventHandler(this.productsFilterTSComboBox_SelectedIndexChanged);
         // 
         // EditEmailAlert_ProductsDialog
         // 
         this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
         this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
         this.ClientSize = new System.Drawing.Size(406, 357);
         this.Controls.Add(this.backPanel);
         this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow;
         this.Name = "EditEmailAlert_ProductsDialog";
         this.Text = "Configure Solimar Products";
         this.Load += new System.EventHandler(this.EditEmailAlert_ProductsDialog_Load);
         this.Controls.SetChildIndex(this.backPanel, 0);
         this.backPanel.ResumeLayout(false);
         this.backPanel.PerformLayout();
         this.productsToolStrip.ResumeLayout(false);
         this.productsToolStrip.PerformLayout();
         this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel backPanel;
        private Shared.VisualComponents.NoFlickerListView productsListView;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ToolStrip productsToolStrip;
        private System.Windows.Forms.ToolStripButton productsCheckAllTSButton;
        private System.Windows.Forms.ToolStripButton productsUncheckAllTSButton;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripLabel toolStripLabel1;
        private System.Windows.Forms.ToolStripComboBox productsFilterTSComboBox;
    }
}