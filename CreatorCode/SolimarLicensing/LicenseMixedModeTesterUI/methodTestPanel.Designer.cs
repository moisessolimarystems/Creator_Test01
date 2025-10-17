namespace LicenseMixedModeTesterUI
{
	partial class methodTestPanel
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
			this.paramPanel = new System.Windows.Forms.Panel();
			this.executePanel = new System.Windows.Forms.Panel();
			this.executeButton = new System.Windows.Forms.Button();
			this.executePanel.SuspendLayout();
			this.SuspendLayout();
			// 
			// paramPanel
			// 
			this.paramPanel.AutoScroll = true;
			this.paramPanel.Dock = System.Windows.Forms.DockStyle.Fill;
			this.paramPanel.Location = new System.Drawing.Point(0, 0);
			this.paramPanel.Name = "paramPanel";
			this.paramPanel.Size = new System.Drawing.Size(416, 177);
			this.paramPanel.TabIndex = 0;
			// 
			// executePanel
			// 
			this.executePanel.Controls.Add(this.executeButton);
			this.executePanel.Dock = System.Windows.Forms.DockStyle.Bottom;
			this.executePanel.Location = new System.Drawing.Point(0, 177);
			this.executePanel.Name = "executePanel";
			this.executePanel.Size = new System.Drawing.Size(416, 30);
			this.executePanel.TabIndex = 1;
			// 
			// executeButton
			// 
			this.executeButton.Location = new System.Drawing.Point(3, 3);
			this.executeButton.Name = "executeButton";
			this.executeButton.Size = new System.Drawing.Size(75, 23);
			this.executeButton.TabIndex = 0;
			this.executeButton.Text = "Execute";
			this.executeButton.UseVisualStyleBackColor = true;
			// 
			// methodTestPanel
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.paramPanel);
			this.Controls.Add(this.executePanel);
			this.Name = "methodTestPanel";
			this.Size = new System.Drawing.Size(416, 207);
			this.executePanel.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Panel paramPanel;
		private System.Windows.Forms.Panel executePanel;
		private System.Windows.Forms.Button executeButton;
	}
}
