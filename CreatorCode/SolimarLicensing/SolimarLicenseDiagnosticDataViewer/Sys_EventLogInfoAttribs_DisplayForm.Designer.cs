namespace SolimarLicenseDiagnosticDataViewer
{
    partial class Sys_EventLogInfoAttribs_DisplayForm
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
            this.mainBackPanel = new System.Windows.Forms.Panel();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.msgListView = new Shared.VisualComponents.NoFlickerListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader5 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.eventLogEntryControl1 = new Shared.VisualComponents.EventLogEntryControl();
            this.msgToolStrip = new System.Windows.Forms.ToolStrip();
            this.tsErrorButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.tsInfoButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.tsWarningButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.tsSourceLabel = new System.Windows.Forms.ToolStripLabel();
            this.tsSourceComboBox = new System.Windows.Forms.ToolStripComboBox();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.tssItemLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.mainBackPanel.SuspendLayout();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.msgToolStrip.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // mainBackPanel
            // 
            this.mainBackPanel.Controls.Add(this.splitContainer1);
            this.mainBackPanel.Controls.Add(this.msgToolStrip);
            this.mainBackPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mainBackPanel.Location = new System.Drawing.Point(0, 0);
            this.mainBackPanel.Name = "mainBackPanel";
            this.mainBackPanel.Size = new System.Drawing.Size(712, 392);
            this.mainBackPanel.TabIndex = 0;
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 25);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.msgListView);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.eventLogEntryControl1);
            this.splitContainer1.Size = new System.Drawing.Size(712, 367);
            this.splitContainer1.SplitterDistance = 172;
            this.splitContainer1.TabIndex = 0;
            // 
            // msgListView
            // 
            this.msgListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3,
            this.columnHeader4,
            this.columnHeader5});
            this.msgListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.msgListView.FullRowSelect = true;
            this.msgListView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.msgListView.HideSelection = false;
            this.msgListView.Location = new System.Drawing.Point(0, 0);
            this.msgListView.MultiSelect = false;
            this.msgListView.Name = "msgListView";
            this.msgListView.Size = new System.Drawing.Size(712, 172);
            this.msgListView.TabIndex = 0;
            this.msgListView.UseCompatibleStateImageBehavior = false;
            this.msgListView.View = System.Windows.Forms.View.Details;
            this.msgListView.SelectedIndexChanged += new System.EventHandler(this.msgListView_SelectedIndexChanged);
            this.msgListView.KeyDown += new System.Windows.Forms.KeyEventHandler(this.general_KeyDown);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Level";
            this.columnHeader1.Width = 90;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Date and Time";
            this.columnHeader2.Width = 190;
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "Source";
            this.columnHeader3.Width = 170;
            // 
            // columnHeader4
            // 
            this.columnHeader4.Text = "Event ID";
            this.columnHeader4.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.columnHeader4.Width = 90;
            // 
            // columnHeader5
            // 
            this.columnHeader5.Text = "Task Category";
            this.columnHeader5.Width = 125;
            // 
            // eventLogEntryControl1
            // 
            this.eventLogEntryControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.eventLogEntryControl1.Location = new System.Drawing.Point(0, 0);
            this.eventLogEntryControl1.Name = "eventLogEntryControl1";
            this.eventLogEntryControl1.Size = new System.Drawing.Size(712, 191);
            this.eventLogEntryControl1.TabIndex = 0;
            // 
            // msgToolStrip
            // 
            this.msgToolStrip.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.msgToolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tsErrorButton,
            this.toolStripSeparator1,
            this.tsInfoButton,
            this.toolStripSeparator2,
            this.tsWarningButton,
            this.toolStripSeparator3,
            this.tsSourceLabel,
            this.tsSourceComboBox});
            this.msgToolStrip.Location = new System.Drawing.Point(0, 0);
            this.msgToolStrip.Name = "msgToolStrip";
            this.msgToolStrip.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
            this.msgToolStrip.Size = new System.Drawing.Size(712, 25);
            this.msgToolStrip.TabIndex = 0;
            this.msgToolStrip.Text = "toolStrip1";
            // 
            // tsErrorButton
            // 
            this.tsErrorButton.Checked = true;
            this.tsErrorButton.CheckOnClick = true;
            this.tsErrorButton.CheckState = System.Windows.Forms.CheckState.Checked;
            this.tsErrorButton.Image = global::SolimarLicenseDiagnosticDataViewer.Properties.Resources.eventLogError;
            this.tsErrorButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tsErrorButton.Name = "tsErrorButton";
            this.tsErrorButton.Size = new System.Drawing.Size(57, 22);
            this.tsErrorButton.Text = "Errors";
            this.tsErrorButton.ToolTipText = "Filter Error Messages";
            this.tsErrorButton.Click += new System.EventHandler(this.RefreshUI);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // tsInfoButton
            // 
            this.tsInfoButton.Checked = true;
            this.tsInfoButton.CheckOnClick = true;
            this.tsInfoButton.CheckState = System.Windows.Forms.CheckState.Checked;
            this.tsInfoButton.Image = global::SolimarLicenseDiagnosticDataViewer.Properties.Resources.eventLogInfo;
            this.tsInfoButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tsInfoButton.Name = "tsInfoButton";
            this.tsInfoButton.Size = new System.Drawing.Size(78, 22);
            this.tsInfoButton.Text = "Messages";
            this.tsInfoButton.ToolTipText = "Filter Information Messages";
            this.tsInfoButton.Click += new System.EventHandler(this.RefreshUI);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 25);
            // 
            // tsWarningButton
            // 
            this.tsWarningButton.Checked = true;
            this.tsWarningButton.CheckOnClick = true;
            this.tsWarningButton.CheckState = System.Windows.Forms.CheckState.Checked;
            this.tsWarningButton.Image = global::SolimarLicenseDiagnosticDataViewer.Properties.Resources.eventLogWarning;
            this.tsWarningButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tsWarningButton.Name = "tsWarningButton";
            this.tsWarningButton.Size = new System.Drawing.Size(77, 22);
            this.tsWarningButton.Text = "Warnings";
            this.tsWarningButton.ToolTipText = "Filter Warning Messages";
            this.tsWarningButton.Click += new System.EventHandler(this.RefreshUI);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(6, 25);
            // 
            // tsSourceLabel
            // 
            this.tsSourceLabel.Name = "tsSourceLabel";
            this.tsSourceLabel.Size = new System.Drawing.Size(46, 22);
            this.tsSourceLabel.Text = "Source:";
            // 
            // tsSourceComboBox
            // 
            this.tsSourceComboBox.AutoToolTip = true;
            this.tsSourceComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.tsSourceComboBox.Name = "tsSourceComboBox";
            this.tsSourceComboBox.Size = new System.Drawing.Size(200, 25);
            this.tsSourceComboBox.DropDownStyleChanged += new System.EventHandler(this.RefreshUI);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tssItemLabel});
            this.statusStrip1.Location = new System.Drawing.Point(0, 392);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(712, 22);
            this.statusStrip1.TabIndex = 1;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // tssItemLabel
            // 
            this.tssItemLabel.Name = "tssItemLabel";
            this.tssItemLabel.Size = new System.Drawing.Size(36, 17);
            this.tssItemLabel.Text = "Items";
            // 
            // Sys_EventLogInfoAttribs_DisplayForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(712, 414);
            this.Controls.Add(this.mainBackPanel);
            this.Controls.Add(this.statusStrip1);
            this.Name = "Sys_EventLogInfoAttribs_DisplayForm";
            this.Text = "Sys_EventLogInfoAttribs";
            this.mainBackPanel.ResumeLayout(false);
            this.mainBackPanel.PerformLayout();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.msgToolStrip.ResumeLayout(false);
            this.msgToolStrip.PerformLayout();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel mainBackPanel;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private Shared.VisualComponents.NoFlickerListView msgListView;
        private Shared.VisualComponents.EventLogEntryControl eventLogEntryControl1;
        private System.Windows.Forms.ToolStrip msgToolStrip;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripButton tsErrorButton;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripButton tsInfoButton;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripButton tsWarningButton;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripLabel tsSourceLabel;
        private System.Windows.Forms.ToolStripComboBox tsSourceComboBox;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.ColumnHeader columnHeader4;
        private System.Windows.Forms.ColumnHeader columnHeader5;
        private System.Windows.Forms.ToolStripStatusLabel tssItemLabel;
    }
}