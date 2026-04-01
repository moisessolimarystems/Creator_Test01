namespace Client.Creator
{
    partial class OrderSelectionForm
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
            this.listView1 = new System.Windows.Forms.ListView();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader3 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader4 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader5 = new System.Windows.Forms.ColumnHeader();
            this.SuspendLayout();
            // 
            // btnCancel
            // 
            this.btnCancel.Location = new System.Drawing.Point(86, 5);
            // 
            // btnOk
            // 
            this.btnOk.Location = new System.Drawing.Point(5, 5);
            // 
            // listView1
            // 
            this.listView1.CheckBoxes = true;
            this.listView1.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3,
            this.columnHeader4,
            this.columnHeader5});
            this.listView1.FullRowSelect = true;
            this.listView1.Location = new System.Drawing.Point(12, 12);
            this.listView1.Name = "listView1";
            this.listView1.Size = new System.Drawing.Size(462, 238);
            this.listView1.TabIndex = 3;
            this.listView1.UseCompatibleStateImageBehavior = false;
            this.listView1.View = System.Windows.Forms.View.Details;
            this.listView1.ColumnClick += new System.Windows.Forms.ColumnClickEventHandler(this.listView1_ColumnClick);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Order Number";
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Type";
            this.columnHeader2.Width = 61;
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "Product";
            this.columnHeader3.Width = 75;
            // 
            // columnHeader4
            // 
            this.columnHeader4.Text = "Exp Date";
            this.columnHeader4.Width = 83;
            // 
            // columnHeader5
            // 
            this.columnHeader5.Text = "Description";
            this.columnHeader5.Width = 103;
            // 
            // OrderSelectionForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.ClientSize = new System.Drawing.Size(486, 287);
            this.Controls.Add(this.listView1);
            this.Name = "OrderSelectionForm";
            this.Text = "Order Selection";
            this.FinishDialog += new Shared.VisualComponents.FinishDialogEventHandler(this.OrderSelectionForm_FinishDialog);
            this.InitDialog += new Shared.VisualComponents.InitDialogEventHandler(this.OrderSelectionForm_InitDialog);
            this.Controls.SetChildIndex(this.listView1, 0);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListView listView1;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.ColumnHeader columnHeader4;
        private System.Windows.Forms.ColumnHeader columnHeader5;

    }
}
