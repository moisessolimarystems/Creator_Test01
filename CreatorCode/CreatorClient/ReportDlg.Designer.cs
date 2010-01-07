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
            // btnOk
            // 
            this.btnOk.Click += new System.EventHandler(this.btnOk_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.conditionPlusButton);
            this.groupBox1.Controls.Add(this.conditionMinusButton);
            this.groupBox1.Controls.Add(this.conditionValueTextBox);
            this.groupBox1.Controls.Add(this.conditionOperatorComboBox);
            this.groupBox1.Controls.Add(this.conditionNameComboBox);
            this.groupBox1.Location = new System.Drawing.Point(12, 14);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(501, 41);
            this.groupBox1.TabIndex = 3;
            this.groupBox1.TabStop = false;
            // 
            // conditionPlusButton
            // 
            this.conditionPlusButton.Location = new System.Drawing.Point(474, 15);
            this.conditionPlusButton.Name = "conditionPlusButton";
            this.conditionPlusButton.Size = new System.Drawing.Size(21, 20);
            this.conditionPlusButton.TabIndex = 4;
            this.conditionPlusButton.Text = "+";
            this.conditionPlusButton.UseVisualStyleBackColor = true;
            this.conditionPlusButton.Visible = false;
            this.conditionPlusButton.Click += new System.EventHandler(this.conditionPlusButton_Click);
            // 
            // conditionMinusButton
            // 
            this.conditionMinusButton.Enabled = false;
            this.conditionMinusButton.Location = new System.Drawing.Point(452, 15);
            this.conditionMinusButton.Name = "conditionMinusButton";
            this.conditionMinusButton.Size = new System.Drawing.Size(21, 20);
            this.conditionMinusButton.TabIndex = 3;
            this.conditionMinusButton.Text = "-";
            this.conditionMinusButton.UseVisualStyleBackColor = true;
            this.conditionMinusButton.Visible = false;
            this.conditionMinusButton.Click += new System.EventHandler(this.conditionMinusButton_Click);
            // 
            // conditionValueTextBox
            // 
            this.conditionValueTextBox.Location = new System.Drawing.Point(251, 15);
            this.conditionValueTextBox.Name = "conditionValueTextBox";
            this.conditionValueTextBox.Size = new System.Drawing.Size(195, 20);
            this.conditionValueTextBox.TabIndex = 2;
            this.conditionValueTextBox.Visible = false;
            // 
            // conditionOperatorComboBox
            // 
            this.conditionOperatorComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.conditionOperatorComboBox.FormattingEnabled = true;
            this.conditionOperatorComboBox.Location = new System.Drawing.Point(133, 14);
            this.conditionOperatorComboBox.Name = "conditionOperatorComboBox";
            this.conditionOperatorComboBox.Size = new System.Drawing.Size(112, 21);
            this.conditionOperatorComboBox.TabIndex = 1;
            this.conditionOperatorComboBox.Visible = false;
            // 
            // conditionNameComboBox
            // 
            this.conditionNameComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.conditionNameComboBox.FormattingEnabled = true;
            this.conditionNameComboBox.Location = new System.Drawing.Point(6, 14);
            this.conditionNameComboBox.Name = "conditionNameComboBox";
            this.conditionNameComboBox.Size = new System.Drawing.Size(121, 21);
            this.conditionNameComboBox.TabIndex = 0;
            this.conditionNameComboBox.Visible = false;
            this.conditionNameComboBox.SelectedIndexChanged += new System.EventHandler(this.conditionNameComboBox_SelectedIndexChanged);
            // 
            // ReportDlg
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.ClientSize = new System.Drawing.Size(525, 103);
            this.Controls.Add(this.groupBox1);
            this.Name = "ReportDlg";
            this.Text = "Report";
            this.FinishDialog += new Shared.VisualComponents.FinishDialogEventHandler(this.ReportDlg_FinishDialog);
            this.InitDialog += new Shared.VisualComponents.InitDialogEventHandler(this.ReportDlg_InitDialog);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.ReportDlg_FormClosing);
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
