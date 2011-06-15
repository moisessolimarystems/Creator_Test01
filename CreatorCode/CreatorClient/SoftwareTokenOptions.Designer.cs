namespace Client.Creator
{
    partial class SoftwareTokenOptions
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
            this.panel1 = new System.Windows.Forms.Panel();
            this.SoftwareTokenListView = new System.Windows.Forms.ListView();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.SoftwareTokenListView);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(343, 197);
            this.panel1.TabIndex = 4;
            // 
            // SoftwareTokenListView
            // 
            this.SoftwareTokenListView.CheckBoxes = true;
            this.SoftwareTokenListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1});
            this.SoftwareTokenListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.SoftwareTokenListView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            this.SoftwareTokenListView.Location = new System.Drawing.Point(0, 0);
            this.SoftwareTokenListView.Name = "SoftwareTokenListView";
            this.SoftwareTokenListView.Size = new System.Drawing.Size(343, 197);
            this.SoftwareTokenListView.TabIndex = 0;
            this.SoftwareTokenListView.UseCompatibleStateImageBehavior = false;
            this.SoftwareTokenListView.View = System.Windows.Forms.View.Details;
            this.SoftwareTokenListView.ItemChecked += new System.Windows.Forms.ItemCheckedEventHandler(this.SoftwareTokenListView_ItemChecked);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Software Token Type";
            this.columnHeader1.Width = 339;
            // 
            // SoftwareTokenOptions
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.ClientSize = new System.Drawing.Size(343, 228);
            this.Controls.Add(this.panel1);
            this.Name = "SoftwareTokenOptions";
            this.Text = "Software Token Preferences";
            this.Controls.SetChildIndex(this.panel1, 0);
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.ListView SoftwareTokenListView;
        private System.Windows.Forms.ColumnHeader columnHeader1;
    }
}
