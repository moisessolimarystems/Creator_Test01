namespace SolimarLicenseDiagnosticDataViewer
{
    partial class Lic_UsageInfoAttribs_DisplayForm
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
            this.usageTreeView = new System.Windows.Forms.TreeView();
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.copyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.contextMenuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // usageTreeView
            // 
            this.usageTreeView.ContextMenuStrip = this.contextMenuStrip1;
            this.usageTreeView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.usageTreeView.FullRowSelect = true;
            this.usageTreeView.Location = new System.Drawing.Point(0, 0);
            this.usageTreeView.Name = "usageTreeView";
            this.usageTreeView.ShowNodeToolTips = true;
            this.usageTreeView.Size = new System.Drawing.Size(360, 298);
            this.usageTreeView.TabIndex = 0;
            this.usageTreeView.MouseClick += new System.Windows.Forms.MouseEventHandler(this.General_MouseClick);
            this.usageTreeView.KeyDown += new System.Windows.Forms.KeyEventHandler(this.usageTreeView_KeyDown);
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.copyToolStripMenuItem});
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.Size = new System.Drawing.Size(103, 26);
            // 
            // copyToolStripMenuItem
            // 
            this.copyToolStripMenuItem.Name = "copyToolStripMenuItem";
            this.copyToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.copyToolStripMenuItem.Text = "Copy";
            this.copyToolStripMenuItem.Click += new System.EventHandler(this.copyToolStripMenuItem_Click);
            // 
            // Lic_UsageInfoAttribs_DisplayForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(360, 298);
            this.Controls.Add(this.usageTreeView);
            this.Name = "Lic_UsageInfoAttribs_DisplayForm";
            this.ShowIcon = false;
            this.Text = "Lic_UsageInfoAttribs";
            this.contextMenuStrip1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TreeView usageTreeView;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
        private System.Windows.Forms.ToolStripMenuItem copyToolStripMenuItem;
    }
}