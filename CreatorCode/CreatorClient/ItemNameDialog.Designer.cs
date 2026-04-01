namespace Client.Creator
{
    partial class ItemNameDialog
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
            this.ItemTypeLabel = new System.Windows.Forms.Label();
            this.itemNameTextBox = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // btnCancel
            // 
            this.btnCancel.Location = new System.Drawing.Point(87, 4);
            // 
            // btnOk
            // 
            this.btnOk.Location = new System.Drawing.Point(6, 4);
            this.btnOk.Click += new System.EventHandler(this.btnOk_Click);
            // 
            // ItemTypeLabel
            // 
            this.ItemTypeLabel.AutoSize = true;
            this.ItemTypeLabel.Location = new System.Drawing.Point(13, 9);
            this.ItemTypeLabel.Name = "ItemTypeLabel";
            this.ItemTypeLabel.Size = new System.Drawing.Size(31, 13);
            this.ItemTypeLabel.TabIndex = 5;
            this.ItemTypeLabel.Text = "[Edit]";
            // 
            // itemNameTextBox
            // 
            this.itemNameTextBox.Location = new System.Drawing.Point(16, 29);
            this.itemNameTextBox.Name = "itemNameTextBox";
            this.itemNameTextBox.Size = new System.Drawing.Size(246, 20);
            this.itemNameTextBox.TabIndex = 6;
            // 
            // ItemNameDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.ClientSize = new System.Drawing.Size(273, 83);
            this.Controls.Add(this.itemNameTextBox);
            this.Controls.Add(this.ItemTypeLabel);
            this.Name = "ItemNameDialog";
            this.Text = "[Edit]";
            this.FinishDialog += new Shared.VisualComponents.FinishDialogEventHandler(this.ItemNameDialog_FinishDialog);
            this.Shown += new System.EventHandler(this.ItemNameDialog_Shown);
            this.InitDialog += new Shared.VisualComponents.InitDialogEventHandler(this.ItemNameDialog_InitDialog);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.ItemNameDialog_FormClosing);
            this.Controls.SetChildIndex(this.ItemTypeLabel, 0);
            this.Controls.SetChildIndex(this.itemNameTextBox, 0);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label ItemTypeLabel;
        private System.Windows.Forms.TextBox itemNameTextBox;

    }
}
