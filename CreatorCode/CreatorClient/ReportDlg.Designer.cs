namespace Client.Creator
{
    partial class ReportDlg
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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.conditionPlusButton = new System.Windows.Forms.Button();
            this.conditionMinusButton = new System.Windows.Forms.Button();
            this.conditionValueTextBox = new System.Windows.Forms.TextBox();
            this.conditionOperatorComboBox = new System.Windows.Forms.ComboBox();
            this.conditionNameComboBox = new System.Windows.Forms.ComboBox();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.conditionPlusButton);
            this.groupBox1.Controls.Add(this.conditionMinusButton);
            this.groupBox1.Controls.Add(this.conditionValueTextBox);
            this.groupBox1.Controls.Add(this.conditionOperatorComboBox);
            this.groupBox1.Controls.Add(this.conditionNameComboBox);
            this.groupBox1.Location = new System.Drawing.Point(12, 26);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(425, 57);
            this.groupBox1.TabIndex = 3;
            this.groupBox1.TabStop = false;
            // 
            // conditionPlusButton
            // 
            this.conditionPlusButton.Location = new System.Drawing.Point(398, 21);
            this.conditionPlusButton.Name = "conditionPlusButton";
            this.conditionPlusButton.Size = new System.Drawing.Size(21, 20);
            this.conditionPlusButton.TabIndex = 4;
            this.conditionPlusButton.Text = "+";
            this.conditionPlusButton.UseVisualStyleBackColor = true;
            // 
            // conditionMinusButton
            // 
            this.conditionMinusButton.Enabled = false;
            this.conditionMinusButton.Location = new System.Drawing.Point(376, 21);
            this.conditionMinusButton.Name = "conditionMinusButton";
            this.conditionMinusButton.Size = new System.Drawing.Size(21, 20);
            this.conditionMinusButton.TabIndex = 3;
            this.conditionMinusButton.Text = "-";
            this.conditionMinusButton.UseVisualStyleBackColor = true;
            // 
            // conditionValueTextBox
            // 
            this.conditionValueTextBox.Location = new System.Drawing.Point(242, 20);
            this.conditionValueTextBox.Name = "conditionValueTextBox";
            this.conditionValueTextBox.Size = new System.Drawing.Size(128, 20);
            this.conditionValueTextBox.TabIndex = 2;
            // 
            // conditionOperatorComboBox
            // 
            this.conditionOperatorComboBox.FormattingEnabled = true;
            this.conditionOperatorComboBox.Location = new System.Drawing.Point(133, 19);
            this.conditionOperatorComboBox.Name = "conditionOperatorComboBox";
            this.conditionOperatorComboBox.Size = new System.Drawing.Size(103, 21);
            this.conditionOperatorComboBox.TabIndex = 1;
            // 
            // conditionNameComboBox
            // 
            this.conditionNameComboBox.FormattingEnabled = true;
            this.conditionNameComboBox.Location = new System.Drawing.Point(6, 19);
            this.conditionNameComboBox.Name = "conditionNameComboBox";
            this.conditionNameComboBox.Size = new System.Drawing.Size(121, 21);
            this.conditionNameComboBox.TabIndex = 0;
            this.conditionNameComboBox.SelectedIndexChanged += new System.EventHandler(this.conditionNameComboBox_SelectedIndexChanged);
            // 
            // ReportDlg
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.ClientSize = new System.Drawing.Size(449, 138);
            this.Controls.Add(this.groupBox1);
            this.Name = "ReportDlg";
            this.Text = "Report";
            this.FinishDialog += new Shared.VisualComponents.FinishDialogEventHandler(this.ReportDlg_FinishDialog);
            this.InitDialog += new Shared.VisualComponents.InitDialogEventHandler(this.ReportDlg_InitDialog);
            this.Controls.SetChildIndex(this.groupBox1, 0);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button conditionPlusButton;
        private System.Windows.Forms.Button conditionMinusButton;
        private System.Windows.Forms.TextBox conditionValueTextBox;
        private System.Windows.Forms.ComboBox conditionOperatorComboBox;
        private System.Windows.Forms.ComboBox conditionNameComboBox;
    }
}
