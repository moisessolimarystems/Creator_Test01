namespace SolimarLicenseViewer
{
    partial class EditEmailAlert_EventsDialog
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
            this.eventsListView = new Shared.VisualComponents.NoFlickerListView();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.eventsCheckAllTSButton = new System.Windows.Forms.ToolStripButton();
            this.eventsUncheckAllTSButton = new System.Windows.Forms.ToolStripButton();
            this.backPanel.SuspendLayout();
            this.toolStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // backPanel
            // 
            this.backPanel.Controls.Add(this.eventsListView);
            this.backPanel.Controls.Add(this.toolStrip1);
            this.backPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.backPanel.Location = new System.Drawing.Point(0, 0);
            this.backPanel.Name = "backPanel";
            this.backPanel.Padding = new System.Windows.Forms.Padding(5, 0, 5, 0);
            this.backPanel.Size = new System.Drawing.Size(396, 321);
            this.backPanel.TabIndex = 4;
            // 
            // eventsListView
            // 
            this.eventsListView.CheckBoxes = true;
            this.eventsListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1});
            this.eventsListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.eventsListView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            this.eventsListView.Location = new System.Drawing.Point(5, 25);
            this.eventsListView.Name = "eventsListView";
            this.eventsListView.Size = new System.Drawing.Size(386, 296);
            this.eventsListView.Sorting = System.Windows.Forms.SortOrder.Ascending;
            this.eventsListView.TabIndex = 1;
            this.eventsListView.UseCompatibleStateImageBehavior = false;
            this.eventsListView.View = System.Windows.Forms.View.Details;
            this.eventsListView.KeyDown += new System.Windows.Forms.KeyEventHandler(this.eventsListView_KeyDown);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Messages";
            this.columnHeader1.Width = 71;
            // 
            // toolStrip1
            // 
            this.toolStrip1.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.eventsCheckAllTSButton,
            this.eventsUncheckAllTSButton});
            this.toolStrip1.Location = new System.Drawing.Point(5, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(386, 25);
            this.toolStrip1.TabIndex = 0;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // eventsCheckAllTSButton
            // 
            this.eventsCheckAllTSButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.eventsCheckAllTSButton.Image = global::SolimarLicenseViewer.Properties.Resources.checked_all;
            this.eventsCheckAllTSButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.eventsCheckAllTSButton.Name = "eventsCheckAllTSButton";
            this.eventsCheckAllTSButton.Size = new System.Drawing.Size(23, 22);
            this.eventsCheckAllTSButton.Text = "Check All";
            this.eventsCheckAllTSButton.Click += new System.EventHandler(this.eventsCheckAllTSButton_Click);
            // 
            // eventsUncheckAllTSButton
            // 
            this.eventsUncheckAllTSButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.eventsUncheckAllTSButton.Image = global::SolimarLicenseViewer.Properties.Resources.check_all;
            this.eventsUncheckAllTSButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.eventsUncheckAllTSButton.Name = "eventsUncheckAllTSButton";
            this.eventsUncheckAllTSButton.Size = new System.Drawing.Size(23, 22);
            this.eventsUncheckAllTSButton.Text = "Uncheck All";
            this.eventsUncheckAllTSButton.Click += new System.EventHandler(this.eventsUncheckAllTSButton_Click);
            // 
            // EditEmailAlert_EventsDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(396, 352);
            this.Controls.Add(this.backPanel);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow;
            this.Name = "EditEmailAlert_EventsDialog";
            this.Text = "Configure Events";
            this.Load += new System.EventHandler(this.EditEmailAlert_EventsDialog_Load);
            this.Controls.SetChildIndex(this.backPanel, 0);
            this.backPanel.ResumeLayout(false);
            this.backPanel.PerformLayout();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel backPanel;
        private Shared.VisualComponents.NoFlickerListView eventsListView;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton eventsCheckAllTSButton;
        private System.Windows.Forms.ToolStripButton eventsUncheckAllTSButton;
    }
}