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
            this.conditionValueTextBox = new System.Windows.Forms.TextBox();
            this.conditionValueTypeComboBox = new System.Windows.Forms.ComboBox();
            this.conditionPlusButton = new System.Windows.Forms.Button();
            this.conditionMinusButton = new System.Windows.Forms.Button();
            this.conditionOperatorComboBox = new System.Windows.Forms.ComboBox();
            this.conditionNameComboBox = new System.Windows.Forms.ComboBox();
            this.conditionValueComboBox = new System.Windows.Forms.ComboBox();
            this.matchCheckBox = new System.Windows.Forms.CheckBox();
            this.matchSingleLabel = new System.Windows.Forms.Label();
            this.matchComboBox = new System.Windows.Forms.ComboBox();
            this.matchMultipleLabel = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnOk
            // 
            this.btnOk.Click += new System.EventHandler(this.btnOk_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.conditionValueTextBox);
            this.groupBox1.Controls.Add(this.conditionValueTypeComboBox);
            this.groupBox1.Controls.Add(this.conditionPlusButton);
            this.groupBox1.Controls.Add(this.conditionMinusButton);
            this.groupBox1.Controls.Add(this.conditionOperatorComboBox);
            this.groupBox1.Controls.Add(this.conditionNameComboBox);
            this.groupBox1.Controls.Add(this.conditionValueComboBox);
            this.groupBox1.Location = new System.Drawing.Point(12, 33);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(501, 41);
            this.groupBox1.TabIndex = 3;
            this.groupBox1.TabStop = false;
            // 
            // conditionValueTextBox
            // 
            this.conditionValueTextBox.Location = new System.Drawing.Point(251, 14);
            this.conditionValueTextBox.Name = "conditionValueTextBox";
            this.conditionValueTextBox.Size = new System.Drawing.Size(95, 20);
            this.conditionValueTextBox.TabIndex = 2;
            this.conditionValueTextBox.Visible = false;
            this.conditionValueTextBox.KeyDown += new System.Windows.Forms.KeyEventHandler(this.conditionValueTextBox_KeyDown);
            // 
            // conditionValueTypeComboBox
            // 
            this.conditionValueTypeComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.conditionValueTypeComboBox.FormattingEnabled = true;
            this.conditionValueTypeComboBox.Location = new System.Drawing.Point(352, 14);
            this.conditionValueTypeComboBox.Name = "conditionValueTypeComboBox";
            this.conditionValueTypeComboBox.Size = new System.Drawing.Size(95, 21);
            this.conditionValueTypeComboBox.TabIndex = 5;
            this.conditionValueTypeComboBox.Visible = false;
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
            // conditionOperatorComboBox
            // 
            this.conditionOperatorComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.conditionOperatorComboBox.FormattingEnabled = true;
            this.conditionOperatorComboBox.Location = new System.Drawing.Point(133, 14);
            this.conditionOperatorComboBox.Name = "conditionOperatorComboBox";
            this.conditionOperatorComboBox.Size = new System.Drawing.Size(112, 21);
            this.conditionOperatorComboBox.TabIndex = 1;
            this.conditionOperatorComboBox.Visible = false;
            this.conditionOperatorComboBox.SelectedIndexChanged += new System.EventHandler(this.conditionOperatorComboBox_SelectedIndexChanged);
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
            // conditionValueComboBox
            // 
            this.conditionValueComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.conditionValueComboBox.FormattingEnabled = true;
            this.conditionValueComboBox.Location = new System.Drawing.Point(251, 14);
            this.conditionValueComboBox.Name = "conditionValueComboBox";
            this.conditionValueComboBox.Size = new System.Drawing.Size(196, 21);
            this.conditionValueComboBox.TabIndex = 6;
            this.conditionValueComboBox.Visible = false;
            // 
            // matchCheckBox
            // 
            this.matchCheckBox.AutoSize = true;
            this.matchCheckBox.Checked = true;
            this.matchCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.matchCheckBox.Location = new System.Drawing.Point(12, 13);
            this.matchCheckBox.Margin = new System.Windows.Forms.Padding(0);
            this.matchCheckBox.Name = "matchCheckBox";
            this.matchCheckBox.Size = new System.Drawing.Size(56, 17);
            this.matchCheckBox.TabIndex = 4;
            this.matchCheckBox.Text = "Match";
            this.matchCheckBox.UseVisualStyleBackColor = true;
            this.matchCheckBox.CheckedChanged += new System.EventHandler(this.matchCheckBox_CheckedChanged);
            // 
            // matchSingleLabel
            // 
            this.matchSingleLabel.AutoSize = true;
            this.matchSingleLabel.Location = new System.Drawing.Point(61, 14);
            this.matchSingleLabel.Name = "matchSingleLabel";
            this.matchSingleLabel.Size = new System.Drawing.Size(89, 13);
            this.matchSingleLabel.TabIndex = 5;
            this.matchSingleLabel.Text = "the following rule:";
            // 
            // matchComboBox
            // 
            this.matchComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.matchComboBox.FormattingEnabled = true;
            this.matchComboBox.Items.AddRange(new object[] {
            "all",
            "any"});
            this.matchComboBox.Location = new System.Drawing.Point(65, 11);
            this.matchComboBox.Name = "matchComboBox";
            this.matchComboBox.Size = new System.Drawing.Size(51, 21);
            this.matchComboBox.TabIndex = 6;
            // 
            // matchMultipleLabel
            // 
            this.matchMultipleLabel.AutoSize = true;
            this.matchMultipleLabel.Location = new System.Drawing.Point(117, 14);
            this.matchMultipleLabel.Name = "matchMultipleLabel";
            this.matchMultipleLabel.Size = new System.Drawing.Size(106, 13);
            this.matchMultipleLabel.TabIndex = 7;
            this.matchMultipleLabel.Text = "of the following rules:";
            // 
            // ReportDlg
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.ClientSize = new System.Drawing.Size(525, 114);
            this.Controls.Add(this.matchComboBox);
            this.Controls.Add(this.matchSingleLabel);
            this.Controls.Add(this.matchMultipleLabel);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.matchCheckBox);
            this.Name = "ReportDlg";
            this.Text = "Report";
            this.FinishDialog += new Shared.VisualComponents.FinishDialogEventHandler(this.ReportDlg_FinishDialog);
            this.InitDialog += new Shared.VisualComponents.InitDialogEventHandler(this.ReportDlg_InitDialog);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.ReportDlg_FormClosing);
            this.Controls.SetChildIndex(this.matchCheckBox, 0);
            this.Controls.SetChildIndex(this.groupBox1, 0);
            this.Controls.SetChildIndex(this.matchMultipleLabel, 0);
            this.Controls.SetChildIndex(this.matchSingleLabel, 0);
            this.Controls.SetChildIndex(this.matchComboBox, 0);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button conditionPlusButton;
        private System.Windows.Forms.Button conditionMinusButton;
        private System.Windows.Forms.TextBox conditionValueTextBox;
        private System.Windows.Forms.ComboBox conditionOperatorComboBox;
        private System.Windows.Forms.ComboBox conditionNameComboBox;
        private System.Windows.Forms.CheckBox matchCheckBox;
        private System.Windows.Forms.Label matchSingleLabel;
        private System.Windows.Forms.ComboBox matchComboBox;
        private System.Windows.Forms.Label matchMultipleLabel;
        private System.Windows.Forms.ComboBox conditionValueTypeComboBox;
        private System.Windows.Forms.ComboBox conditionValueComboBox;
    }
}
