namespace Client.Creator
{
    partial class ConnectDialog
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
            this.ServerNameComboBox = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.errorProvider1 = new System.Windows.Forms.ErrorProvider(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).BeginInit();
            this.SuspendLayout();
            // 
            // btnCancel
            // 
            this.btnCancel.Location = new System.Drawing.Point(200, 5);
            // 
            // btnOk
            // 
            this.btnOk.CausesValidation = true;
            this.btnOk.Location = new System.Drawing.Point(120, 5);
            // 
            // ServerNameComboBox
            // 
            this.ServerNameComboBox.FormattingEnabled = true;
            this.ServerNameComboBox.Location = new System.Drawing.Point(73, 32);
            this.ServerNameComboBox.Name = "ServerNameComboBox";
            this.ServerNameComboBox.Size = new System.Drawing.Size(202, 21);
            this.ServerNameComboBox.TabIndex = 2;
            this.ServerNameComboBox.Validating += new System.ComponentModel.CancelEventHandler(this.ServerNameComboBox_Validating);
            this.ServerNameComboBox.KeyUp += new System.Windows.Forms.KeyEventHandler(this.ServerNameComboBox_KeyUp);
            this.ServerNameComboBox.Validated += new System.EventHandler(this.ServerNameComboBox_Validated);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.CausesValidation = false;
            this.label1.Location = new System.Drawing.Point(12, 35);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(41, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "Server:";
            // 
            // errorProvider1
            // 
            this.errorProvider1.ContainerControl = this;
            // 
            // ConnectDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.ClientSize = new System.Drawing.Size(290, 103);
            this.Controls.Add(this.ServerNameComboBox);
            this.Controls.Add(this.label1);
            this.Name = "ConnectDialog";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "License Server Connection";
            this.FinishDialog += new Shared.VisualComponents.FinishDialogEventHandler(this.ConnectDialog_FinishDialog);
            this.InitDialog += new Shared.VisualComponents.InitDialogEventHandler(this.ConnectDialog_InitDialog);
            this.Controls.SetChildIndex(this.label1, 0);
            this.Controls.SetChildIndex(this.ServerNameComboBox, 0);
            ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox ServerNameComboBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ErrorProvider errorProvider1;
    }
}
