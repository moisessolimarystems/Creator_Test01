namespace Client.Creator
{
    partial class PacketVerification
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(PacketVerification));
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.label1 = new System.Windows.Forms.Label();
            this.verificationCodeTextBox = new System.Windows.Forms.TextBox();
            this.VerificationFileBrowseButton = new System.Windows.Forms.Button();
            this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.tableLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnOk
            // 
            this.btnOk.Click += new System.EventHandler(this.btnOk_Click);
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 3;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 40.57508F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 59.42492F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 34F));
            this.tableLayoutPanel1.Controls.Add(this.label1, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.verificationCodeTextBox, 1, 0);
            this.tableLayoutPanel1.Controls.Add(this.VerificationFileBrowseButton, 2, 0);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 1;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 53.42466F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(343, 48);
            this.tableLayoutPanel1.TabIndex = 3;
            // 
            // label1
            // 
            this.label1.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(5, 21);
            this.label1.Margin = new System.Windows.Forms.Padding(5, 7, 3, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(108, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Verification Code:";
            // 
            // verificationCodeTextBox
            // 
            this.verificationCodeTextBox.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.verificationCodeTextBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.verificationCodeTextBox.Location = new System.Drawing.Point(128, 16);
            this.verificationCodeTextBox.Margin = new System.Windows.Forms.Padding(3, 10, 0, 3);
            this.verificationCodeTextBox.Name = "verificationCodeTextBox";
            this.verificationCodeTextBox.Size = new System.Drawing.Size(180, 22);
            this.verificationCodeTextBox.TabIndex = 1;
            // 
            // VerificationFileBrowseButton
            // 
            this.VerificationFileBrowseButton.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.VerificationFileBrowseButton.BackColor = System.Drawing.SystemColors.Control;
            this.VerificationFileBrowseButton.Image = ((System.Drawing.Image)(resources.GetObject("VerificationFileBrowseButton.Image")));
            this.VerificationFileBrowseButton.Location = new System.Drawing.Point(311, 16);
            this.VerificationFileBrowseButton.Margin = new System.Windows.Forms.Padding(3, 10, 3, 3);
            this.VerificationFileBrowseButton.Name = "VerificationFileBrowseButton";
            this.VerificationFileBrowseButton.Size = new System.Drawing.Size(25, 23);
            this.VerificationFileBrowseButton.TabIndex = 2;
            this.VerificationFileBrowseButton.UseVisualStyleBackColor = false;
            this.VerificationFileBrowseButton.Click += new System.EventHandler(this.VerificationFileBrowseButton_Click);
            // 
            // openFileDialog
            // 
            this.openFileDialog.DefaultExt = "lsData";
            this.openFileDialog.Filter = "License Verification Data|*.lsData";
            // 
            // PacketVerification
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.ClientSize = new System.Drawing.Size(343, 79);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Name = "PacketVerification";
            this.Text = "Packet Verification";
            this.FinishDialog += new Shared.VisualComponents.FinishDialogEventHandler(this.PacketVerification_FinishDialog);
            this.InitDialog += new Shared.VisualComponents.InitDialogEventHandler(this.PacketVerification_InitDialog);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.PacketVerification_FormClosing);
            this.Controls.SetChildIndex(this.tableLayoutPanel1, 0);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox verificationCodeTextBox;
        private System.Windows.Forms.Button VerificationFileBrowseButton;
        private System.Windows.Forms.OpenFileDialog openFileDialog;
    }
}
