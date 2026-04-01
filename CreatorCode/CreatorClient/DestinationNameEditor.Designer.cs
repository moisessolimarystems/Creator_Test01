namespace Client.Creator
{
    partial class DestinationNameEditor
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
            this.label1 = new System.Windows.Forms.Label();
            this.destNameTextBox = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // btnCancel
            // 
            this.btnCancel.Location = new System.Drawing.Point(87, 4);
            // 
            // btnOk
            // 
            this.btnOk.Location = new System.Drawing.Point(6, 4);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(97, 13);
            this.label1.TabIndex = 5;
            this.label1.Text = "Destination Name :";
            // 
            // destNameTextBox
            // 
            this.destNameTextBox.Location = new System.Drawing.Point(16, 29);
            this.destNameTextBox.Name = "destNameTextBox";
            this.destNameTextBox.Size = new System.Drawing.Size(246, 20);
            this.destNameTextBox.TabIndex = 6;
            // 
            // DestinationNameEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.ClientSize = new System.Drawing.Size(273, 83);
            this.Controls.Add(this.destNameTextBox);
            this.Controls.Add(this.label1);
            this.Name = "DestinationNameEditor";
            this.Text = "New Destination Name";
            this.FinishDialog += new Shared.VisualComponents.FinishDialogEventHandler(this.DestinationNameEditor_FinishDialog);
            this.InitDialog += new Shared.VisualComponents.InitDialogEventHandler(this.DestinationNameEditor_InitDialog);
            this.Controls.SetChildIndex(this.label1, 0);
            this.Controls.SetChildIndex(this.destNameTextBox, 0);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox destNameTextBox;

    }
}
