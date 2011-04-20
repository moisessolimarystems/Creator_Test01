namespace SolimarLicenseDiagnosticDataViewer
{
    partial class XmlViewer_DisplayForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(XmlViewer_DisplayForm));
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.findTSButton = new System.Windows.Forms.ToolStripButton();
            this.findTSTextBox = new System.Windows.Forms.ToolStripTextBox();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.TSSTitleLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.TSSLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStrip1.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // textBox1
            // 
            this.textBox1.AcceptsReturn = true;
            this.textBox1.AcceptsTab = true;
            this.textBox1.BackColor = System.Drawing.Color.White;
            this.textBox1.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBox1.Location = new System.Drawing.Point(0, 25);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.ReadOnly = true;
            this.textBox1.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBox1.Size = new System.Drawing.Size(284, 217);
            this.textBox1.TabIndex = 0;
            this.textBox1.KeyDown += new System.Windows.Forms.KeyEventHandler(this.textBox1_KeyDown);
            // 
            // toolStrip1
            // 
            this.toolStrip1.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.findTSButton,
            this.findTSTextBox});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(284, 25);
            this.toolStrip1.TabIndex = 2;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // findTSButton
            // 
            this.findTSButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.findTSButton.Image = ((System.Drawing.Image)(resources.GetObject("findTSButton.Image")));
            this.findTSButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.findTSButton.Name = "findTSButton";
            this.findTSButton.Size = new System.Drawing.Size(34, 22);
            this.findTSButton.Text = "Find";
            this.findTSButton.Click += new System.EventHandler(this.findTSButton_Click);
            // 
            // findTSTextBox
            // 
            this.findTSTextBox.Name = "findTSTextBox";
            this.findTSTextBox.Size = new System.Drawing.Size(200, 25);
            this.findTSTextBox.KeyDown += new System.Windows.Forms.KeyEventHandler(this.findTSTextBox_KeyDown);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.TSSTitleLabel1,
            this.TSSLabel1});
            this.statusStrip1.Location = new System.Drawing.Point(0, 242);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(284, 22);
            this.statusStrip1.TabIndex = 3;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // TSSTitleLabel1
            // 
            this.TSSTitleLabel1.Name = "TSSTitleLabel1";
            this.TSSTitleLabel1.Size = new System.Drawing.Size(50, 17);
            this.TSSTitleLabel1.Text = "nb char:";
            // 
            // TSSLabel1
            // 
            this.TSSLabel1.Name = "TSSLabel1";
            this.TSSLabel1.Size = new System.Drawing.Size(15, 17);
            this.TSSLabel1.Text = "[]";
            // 
            // XmlViewer_DisplayForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 264);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.toolStrip1);
            this.DoubleBuffered = true;
            this.Name = "XmlViewer_DisplayForm";
            this.ShowInTaskbar = false;
            this.Text = "Raw XML";
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton findTSButton;
        private System.Windows.Forms.ToolStripTextBox findTSTextBox;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel TSSTitleLabel1;
        private System.Windows.Forms.ToolStripStatusLabel TSSLabel1;
    }
}