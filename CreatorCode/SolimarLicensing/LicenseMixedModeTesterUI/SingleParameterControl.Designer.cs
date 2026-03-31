namespace LicenseMixedModeTesterUI
{
	partial class SingleParameterControl
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
			this.paramLabel = new System.Windows.Forms.Label();
			this.paramTextBox = new System.Windows.Forms.TextBox();
			this.SuspendLayout();
			// 
			// paramLabel
			// 
			this.paramLabel.AutoSize = true;
			this.paramLabel.Dock = System.Windows.Forms.DockStyle.Fill;
			this.paramLabel.Location = new System.Drawing.Point(2, 2);
			this.paramLabel.Margin = new System.Windows.Forms.Padding(0);
			this.paramLabel.Name = "paramLabel";
			this.paramLabel.Size = new System.Drawing.Size(62, 13);
			this.paramLabel.TabIndex = 0;
			this.paramLabel.Text = "paramLabel";
			// 
			// paramTextBox
			// 
			this.paramTextBox.Dock = System.Windows.Forms.DockStyle.Bottom;
			this.paramTextBox.Location = new System.Drawing.Point(2, 13);
			this.paramTextBox.Name = "paramTextBox";
			this.paramTextBox.Size = new System.Drawing.Size(543, 20);
			this.paramTextBox.TabIndex = 1;
			// 
			// SingleParameterControl
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.paramTextBox);
			this.Controls.Add(this.paramLabel);
			this.Name = "SingleParameterControl";
			this.Padding = new System.Windows.Forms.Padding(2);
			this.Size = new System.Drawing.Size(547, 35);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label paramLabel;
		private System.Windows.Forms.TextBox paramTextBox;
	}
}
