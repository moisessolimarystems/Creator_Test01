using Shared.VisualComponents;
namespace SolimarLicenseDiagnosticDataViewer
{
	partial class Lic_LicenseInfoAttribs_DisplayForm
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
			this.generalToolTip = new System.Windows.Forms.ToolTip(this.components);
			this.licenseInfoTreeView = new Shared.VisualComponents.LicenseInfoViewerControl();
			this.SuspendLayout();
			// 
			// licenseInfoTreeView
			// 
			this.licenseInfoTreeView.Dock = System.Windows.Forms.DockStyle.Fill;
			this.licenseInfoTreeView.FullRowSelect = true;
			this.licenseInfoTreeView.Location = new System.Drawing.Point(0, 0);
			this.licenseInfoTreeView.Name = "licenseInfoTreeView";
			this.licenseInfoTreeView.ShowLines = false;
			this.licenseInfoTreeView.ShowNodeToolTips = true;
			this.licenseInfoTreeView.Size = new System.Drawing.Size(452, 302);
			this.licenseInfoTreeView.TabIndex = 0;
			// 
			// Lic_LicenseInfoAttribs_DisplayForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(452, 302);
			this.Controls.Add(this.licenseInfoTreeView);
			this.Name = "Lic_LicenseInfoAttribs_DisplayForm";
			this.ShowIcon = false;
			this.Text = "Lic_LicenseInfoAttribs";
			this.ResumeLayout(false);

		}

		#endregion

		private LicenseInfoViewerControl licenseInfoTreeView;
		private System.Windows.Forms.ToolTip generalToolTip;
	}
}