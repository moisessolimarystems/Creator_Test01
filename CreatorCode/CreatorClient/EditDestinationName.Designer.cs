namespace Client.Creator
{
    partial class EditDestinationName
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
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.label1 = new System.Windows.Forms.Label();
            this.flowLayoutPanel1 = new System.Windows.Forms.FlowLayoutPanel();
            this.removeDestNameButton = new System.Windows.Forms.Button();
            this.renameDestNameButton = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.destNameListView = new System.Windows.Forms.ListView();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.flowLayoutPanel1.SuspendLayout();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnCancel
            // 
            this.btnCancel.Location = new System.Drawing.Point(89, 0);
            this.btnCancel.Visible = false;
            // 
            // btnOk
            // 
            this.btnOk.Location = new System.Drawing.Point(89, 0);
            this.btnOk.Text = "Close";
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.tableLayoutPanel1);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.flowLayoutPanel1);
            this.splitContainer1.Size = new System.Drawing.Size(336, 176);
            this.splitContainer1.SplitterDistance = 241;
            this.splitContainer1.TabIndex = 3;
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.label1, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.destNameListView, 0, 1);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.Padding = new System.Windows.Forms.Padding(10, 0, 0, 0);
            this.tableLayoutPanel1.RowCount = 2;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.Size = new System.Drawing.Size(241, 176);
            this.tableLayoutPanel1.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 0);
            this.label1.Name = "label1";
            this.label1.Padding = new System.Windows.Forms.Padding(0, 10, 0, 0);
            this.label1.Size = new System.Drawing.Size(99, 23);
            this.label1.TabIndex = 0;
            this.label1.Text = "Destination Names:";
            // 
            // flowLayoutPanel1
            // 
            this.flowLayoutPanel1.Controls.Add(this.removeDestNameButton);
            this.flowLayoutPanel1.Controls.Add(this.renameDestNameButton);
            this.flowLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.flowLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.flowLayoutPanel1.Name = "flowLayoutPanel1";
            this.flowLayoutPanel1.Padding = new System.Windows.Forms.Padding(4, 21, 5, 0);
            this.flowLayoutPanel1.Size = new System.Drawing.Size(91, 176);
            this.flowLayoutPanel1.TabIndex = 0;
            // 
            // removeDestNameButton
            // 
            this.removeDestNameButton.Location = new System.Drawing.Point(7, 24);
            this.removeDestNameButton.Name = "removeDestNameButton";
            this.removeDestNameButton.Size = new System.Drawing.Size(75, 23);
            this.removeDestNameButton.TabIndex = 0;
            this.removeDestNameButton.Text = "Remove";
            this.removeDestNameButton.UseVisualStyleBackColor = true;
            this.removeDestNameButton.Click += new System.EventHandler(this.removeDestNameButton_Click);
            // 
            // renameDestNameButton
            // 
            this.renameDestNameButton.Location = new System.Drawing.Point(7, 53);
            this.renameDestNameButton.Name = "renameDestNameButton";
            this.renameDestNameButton.Size = new System.Drawing.Size(75, 23);
            this.renameDestNameButton.TabIndex = 1;
            this.renameDestNameButton.Text = "Rename";
            this.renameDestNameButton.UseVisualStyleBackColor = true;
            this.renameDestNameButton.Click += new System.EventHandler(this.renameDestNameButton_Click);
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.splitContainer1);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(336, 176);
            this.panel1.TabIndex = 4;
            // 
            // destNameListView
            // 
            this.destNameListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.destNameListView.HideSelection = false;
            this.destNameListView.LabelEdit = true;
            this.destNameListView.Location = new System.Drawing.Point(13, 26);
            this.destNameListView.MultiSelect = false;
            this.destNameListView.Name = "destNameListView";
            this.destNameListView.Size = new System.Drawing.Size(225, 147);
            this.destNameListView.TabIndex = 1;
            this.destNameListView.UseCompatibleStateImageBehavior = false;
            this.destNameListView.View = System.Windows.Forms.View.List;
            this.destNameListView.AfterLabelEdit += new System.Windows.Forms.LabelEditEventHandler(this.destNameListView_AfterLabelEdit);
            this.destNameListView.ItemSelectionChanged += new System.Windows.Forms.ListViewItemSelectionChangedEventHandler(this.destNameListView_ItemSelectionChanged);
            // 
            // EditDestinationName
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.ClientSize = new System.Drawing.Size(336, 207);
            this.Controls.Add(this.panel1);
            this.Name = "EditDestinationName";
            this.Text = "Edit Destination Names";
            this.Controls.SetChildIndex(this.panel1, 0);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.flowLayoutPanel1.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel1;
        private System.Windows.Forms.Button removeDestNameButton;
        private System.Windows.Forms.Button renameDestNameButton;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ListView destNameListView;
    }
}
