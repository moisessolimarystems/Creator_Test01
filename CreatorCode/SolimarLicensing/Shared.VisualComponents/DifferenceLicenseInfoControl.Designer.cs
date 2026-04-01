namespace Shared.VisualComponents
{
    partial class DifferenceLicenseInfoControl
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
            this.MainBackPanel = new System.Windows.Forms.Panel();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.mainToolStrip = new System.Windows.Forms.ToolStrip();
            this.filterLabelTS = new System.Windows.Forms.ToolStripLabel();
            this.filterComboBoxTS = new System.Windows.Forms.ToolStripComboBox();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.leftListView = new Shared.VisualComponents.ListView_Scroll();
            this.TextCH1 = new System.Windows.Forms.ColumnHeader();
            this.rightListView = new Shared.VisualComponents.ListView_Scroll();
            this.TextCH2 = new System.Windows.Forms.ColumnHeader();
            this.MainBackPanel.SuspendLayout();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.mainToolStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // MainBackPanel
            // 
            this.MainBackPanel.Controls.Add(this.splitContainer1);
            this.MainBackPanel.Controls.Add(this.mainToolStrip);
            this.MainBackPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.MainBackPanel.Location = new System.Drawing.Point(0, 0);
            this.MainBackPanel.Name = "MainBackPanel";
            this.MainBackPanel.Size = new System.Drawing.Size(641, 311);
            this.MainBackPanel.TabIndex = 0;
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 25);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.leftListView);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.rightListView);
            this.splitContainer1.Size = new System.Drawing.Size(641, 286);
            this.splitContainer1.SplitterDistance = 330;
            this.splitContainer1.TabIndex = 0;
            // 
            // mainToolStrip
            // 
            this.mainToolStrip.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.mainToolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.filterLabelTS,
            this.filterComboBoxTS,
            this.toolStripSeparator1});
            this.mainToolStrip.Location = new System.Drawing.Point(0, 0);
            this.mainToolStrip.Name = "mainToolStrip";
            this.mainToolStrip.Size = new System.Drawing.Size(641, 25);
            this.mainToolStrip.TabIndex = 1;
            this.mainToolStrip.Text = "toolStrip1";
            // 
            // filterLabelTS
            // 
            this.filterLabelTS.Name = "filterLabelTS";
            this.filterLabelTS.Size = new System.Drawing.Size(38, 22);
            this.filterLabelTS.Text = "Filter: ";
            // 
            // filterComboBoxTS
            // 
            this.filterComboBoxTS.AutoSize = false;
            this.filterComboBoxTS.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.filterComboBoxTS.Items.AddRange(new object[] {
            "Show All",
            "Show Differences"});
            this.filterComboBoxTS.Name = "filterComboBoxTS";
            this.filterComboBoxTS.Size = new System.Drawing.Size(121, 21);
            this.filterComboBoxTS.ToolTipText = "Filter";
            this.filterComboBoxTS.SelectedIndexChanged += new System.EventHandler(this.filterComboBoxTS_SelectedIndexChanged);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // leftListView
            // 
            this.leftListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.TextCH1});
            this.leftListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.leftListView.Font = new System.Drawing.Font("Courier New", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.leftListView.FullRowSelect = true;
            this.leftListView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            this.leftListView.HideSelection = false;
            this.leftListView.Location = new System.Drawing.Point(0, 0);
            this.leftListView.Name = "leftListView";
            this.leftListView.ShowItemToolTips = true;
            this.leftListView.Size = new System.Drawing.Size(330, 286);
            this.leftListView.TabIndex = 1;
            this.toolTip1.SetToolTip(this.leftListView, "QWERTY");
            this.leftListView.UseCompatibleStateImageBehavior = false;
            this.leftListView.View = System.Windows.Forms.View.Details;
            this.leftListView.Scroll += new Shared.VisualComponents.ListView_Scroll.ScrollHandler(this.leftListView_Scroll);
            this.leftListView.SelectedIndexChanged += new System.EventHandler(this.leftListView_SelectedIndexChanged);
            // 
            // TextCH1
            // 
            this.TextCH1.Width = 500;
            // 
            // rightListView
            // 
            this.rightListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.TextCH2});
            this.rightListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.rightListView.Font = new System.Drawing.Font("Courier New", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rightListView.FullRowSelect = true;
            this.rightListView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            this.rightListView.HideSelection = false;
            this.rightListView.Location = new System.Drawing.Point(0, 0);
            this.rightListView.Name = "rightListView";
            this.rightListView.ShowItemToolTips = true;
            this.rightListView.Size = new System.Drawing.Size(307, 286);
            this.rightListView.TabIndex = 2;
            this.rightListView.UseCompatibleStateImageBehavior = false;
            this.rightListView.View = System.Windows.Forms.View.Details;
            this.rightListView.Scroll += new Shared.VisualComponents.ListView_Scroll.ScrollHandler(this.rightListView_Scroll);
            this.rightListView.SelectedIndexChanged += new System.EventHandler(this.rightListView_SelectedIndexChanged);
            // 
            // TextCH2
            // 
            this.TextCH2.Width = 500;
            // 
            // DifferenceLicenseInfoControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.MainBackPanel);
            this.Name = "DifferenceLicenseInfoControl";
            this.Size = new System.Drawing.Size(641, 311);
            this.MainBackPanel.ResumeLayout(false);
            this.MainBackPanel.PerformLayout();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.mainToolStrip.ResumeLayout(false);
            this.mainToolStrip.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel MainBackPanel;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private ListView_Scroll leftListView;
        private System.Windows.Forms.ColumnHeader TextCH1;
        private ListView_Scroll rightListView;
        private System.Windows.Forms.ColumnHeader TextCH2;
        private System.Windows.Forms.ToolStrip mainToolStrip;
        private System.Windows.Forms.ToolStripComboBox filterComboBoxTS;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripLabel filterLabelTS;
        private System.Windows.Forms.ToolTip toolTip1;
    }
}
