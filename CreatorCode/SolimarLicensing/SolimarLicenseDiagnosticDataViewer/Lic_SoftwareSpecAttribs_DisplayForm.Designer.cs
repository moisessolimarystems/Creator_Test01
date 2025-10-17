namespace SolimarLicenseDiagnosticDataViewer
{
    partial class Lic_SoftwareSpecAttribs_DisplayForm
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
            this.softwareSpecViewerControl1 = new Shared.VisualComponents.SoftwareSpecViewerControl();
            this.SuspendLayout();
            // 
            // softwareSpecViewerControl1
            // 
            this.softwareSpecViewerControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.softwareSpecViewerControl1.FullRowSelect = true;
            this.softwareSpecViewerControl1.Location = new System.Drawing.Point(0, 0);
            this.softwareSpecViewerControl1.Name = "softwareSpecViewerControl1";
            this.softwareSpecViewerControl1.ShowLines = false;
            this.softwareSpecViewerControl1.ShowNodeToolTips = true;
            this.softwareSpecViewerControl1.Size = new System.Drawing.Size(507, 344);
            this.softwareSpecViewerControl1.TabIndex = 0;
            this.softwareSpecViewerControl1.Enter += new System.EventHandler(this.softwareSpecViewerControl1_Enter);
            // 
            // Lic_SoftwareSpecAttribs_DisplayForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(507, 344);
            this.Controls.Add(this.softwareSpecViewerControl1);
            this.Name = "Lic_SoftwareSpecAttribs_DisplayForm";
            this.Text = "Lic_SoftwareSpecAttribs";
            this.ResumeLayout(false);

        }

        #endregion

        private Shared.VisualComponents.SoftwareSpecViewerControl softwareSpecViewerControl1;
    }
}