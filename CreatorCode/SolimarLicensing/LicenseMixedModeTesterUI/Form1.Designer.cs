namespace LicenseMixedModeTesterUI
{
	partial class Form1
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
			this.messagesSplitContainer = new System.Windows.Forms.SplitContainer();
			this.messageListView = new System.Windows.Forms.ListView();
			this.infoCH = new System.Windows.Forms.ColumnHeader();
			this.callbackListView = new System.Windows.Forms.ListView();
			this.DateTimeCH = new System.Windows.Forms.ColumnHeader();
			this.MessageTypeCH = new System.Windows.Forms.ColumnHeader();
			this.keyCH = new System.Windows.Forms.ColumnHeader();
			this.MessageCH = new System.Windows.Forms.ColumnHeader();
			this.errorCodeCH = new System.Windows.Forms.ColumnHeader();
			this.mainSplitContainer = new System.Windows.Forms.SplitContainer();
			this.functionSplitContainer = new System.Windows.Forms.SplitContainer();
			this.functionTreeView = new System.Windows.Forms.TreeView();
			this.methodPropertyGrid = new System.Windows.Forms.PropertyGrid();
			this.methodTestPanel1 = new LicenseMixedModeTesterUI.methodTestPanel();
			this.messagesSplitContainer.Panel1.SuspendLayout();
			this.messagesSplitContainer.Panel2.SuspendLayout();
			this.messagesSplitContainer.SuspendLayout();
			this.mainSplitContainer.Panel1.SuspendLayout();
			this.mainSplitContainer.Panel2.SuspendLayout();
			this.mainSplitContainer.SuspendLayout();
			this.functionSplitContainer.Panel1.SuspendLayout();
			this.functionSplitContainer.Panel2.SuspendLayout();
			this.functionSplitContainer.SuspendLayout();
			this.SuspendLayout();
			// 
			// messagesSplitContainer
			// 
			this.messagesSplitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
			this.messagesSplitContainer.Location = new System.Drawing.Point(0, 0);
			this.messagesSplitContainer.Name = "messagesSplitContainer";
			// 
			// messagesSplitContainer.Panel1
			// 
			this.messagesSplitContainer.Panel1.Controls.Add(this.messageListView);
			// 
			// messagesSplitContainer.Panel2
			// 
			this.messagesSplitContainer.Panel2.Controls.Add(this.callbackListView);
			this.messagesSplitContainer.Size = new System.Drawing.Size(702, 316);
			this.messagesSplitContainer.SplitterDistance = 335;
			this.messagesSplitContainer.TabIndex = 0;
			// 
			// messageListView
			// 
			this.messageListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.infoCH});
			this.messageListView.Dock = System.Windows.Forms.DockStyle.Fill;
			this.messageListView.FullRowSelect = true;
			this.messageListView.Location = new System.Drawing.Point(0, 0);
			this.messageListView.Name = "messageListView";
			this.messageListView.Size = new System.Drawing.Size(335, 316);
			this.messageListView.TabIndex = 0;
			this.messageListView.UseCompatibleStateImageBehavior = false;
			this.messageListView.View = System.Windows.Forms.View.Details;
			// 
			// infoCH
			// 
			this.infoCH.Text = "Information";
			this.infoCH.Width = 690;
			// 
			// callbackListView
			// 
			this.callbackListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.DateTimeCH,
            this.MessageTypeCH,
            this.keyCH,
            this.MessageCH,
            this.errorCodeCH});
			this.callbackListView.Dock = System.Windows.Forms.DockStyle.Fill;
			this.callbackListView.FullRowSelect = true;
			this.callbackListView.Location = new System.Drawing.Point(0, 0);
			this.callbackListView.Name = "callbackListView";
			this.callbackListView.Size = new System.Drawing.Size(363, 316);
			this.callbackListView.TabIndex = 0;
			this.callbackListView.UseCompatibleStateImageBehavior = false;
			this.callbackListView.View = System.Windows.Forms.View.Details;
			// 
			// DateTimeCH
			// 
			this.DateTimeCH.Text = "Time";
			this.DateTimeCH.Width = 132;
			// 
			// MessageTypeCH
			// 
			this.MessageTypeCH.Text = "Type";
			this.MessageTypeCH.Width = 58;
			// 
			// keyCH
			// 
			this.keyCH.Text = "Key";
			this.keyCH.Width = 78;
			// 
			// MessageCH
			// 
			this.MessageCH.Text = "Message";
			this.MessageCH.Width = 356;
			// 
			// errorCodeCH
			// 
			this.errorCodeCH.Text = "Error Code";
			this.errorCodeCH.Width = 71;
			// 
			// mainSplitContainer
			// 
			this.mainSplitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
			this.mainSplitContainer.Location = new System.Drawing.Point(0, 0);
			this.mainSplitContainer.Name = "mainSplitContainer";
			this.mainSplitContainer.Orientation = System.Windows.Forms.Orientation.Horizontal;
			// 
			// mainSplitContainer.Panel1
			// 
			this.mainSplitContainer.Panel1.Controls.Add(this.functionSplitContainer);
			// 
			// mainSplitContainer.Panel2
			// 
			this.mainSplitContainer.Panel2.Controls.Add(this.messagesSplitContainer);
			this.mainSplitContainer.Size = new System.Drawing.Size(702, 345);
			this.mainSplitContainer.SplitterDistance = 25;
			this.mainSplitContainer.TabIndex = 1;
			// 
			// functionSplitContainer
			// 
			this.functionSplitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
			this.functionSplitContainer.Location = new System.Drawing.Point(0, 0);
			this.functionSplitContainer.Name = "functionSplitContainer";
			// 
			// functionSplitContainer.Panel1
			// 
			this.functionSplitContainer.Panel1.Controls.Add(this.functionTreeView);
			// 
			// functionSplitContainer.Panel2
			// 
			this.functionSplitContainer.Panel2.Controls.Add(this.methodTestPanel1);
			this.functionSplitContainer.Panel2.Controls.Add(this.methodPropertyGrid);
			this.functionSplitContainer.Size = new System.Drawing.Size(702, 25);
			this.functionSplitContainer.SplitterDistance = 234;
			this.functionSplitContainer.TabIndex = 0;
			this.functionSplitContainer.Visible = false;
			// 
			// functionTreeView
			// 
			this.functionTreeView.Dock = System.Windows.Forms.DockStyle.Fill;
			this.functionTreeView.Location = new System.Drawing.Point(0, 0);
			this.functionTreeView.Name = "functionTreeView";
			this.functionTreeView.Size = new System.Drawing.Size(234, 25);
			this.functionTreeView.TabIndex = 0;
			this.functionTreeView.Click += new System.EventHandler(this.functionTreeView_Click);
			// 
			// methodPropertyGrid
			// 
			this.methodPropertyGrid.Dock = System.Windows.Forms.DockStyle.Left;
			this.methodPropertyGrid.HelpVisible = false;
			this.methodPropertyGrid.Location = new System.Drawing.Point(0, 0);
			this.methodPropertyGrid.Name = "methodPropertyGrid";
			this.methodPropertyGrid.PropertySort = System.Windows.Forms.PropertySort.NoSort;
			this.methodPropertyGrid.Size = new System.Drawing.Size(126, 25);
			this.methodPropertyGrid.TabIndex = 0;
			this.methodPropertyGrid.ToolbarVisible = false;
			// 
			// methodTestPanel1
			// 
			this.methodTestPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.methodTestPanel1.Location = new System.Drawing.Point(126, 0);
			this.methodTestPanel1.Name = "methodTestPanel1";
			this.methodTestPanel1.Size = new System.Drawing.Size(338, 25);
			this.methodTestPanel1.TabIndex = 1;
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(702, 345);
			this.Controls.Add(this.mainSplitContainer);
			this.Name = "Form1";
			this.Text = "Form1";
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
			this.messagesSplitContainer.Panel1.ResumeLayout(false);
			this.messagesSplitContainer.Panel2.ResumeLayout(false);
			this.messagesSplitContainer.ResumeLayout(false);
			this.mainSplitContainer.Panel1.ResumeLayout(false);
			this.mainSplitContainer.Panel2.ResumeLayout(false);
			this.mainSplitContainer.ResumeLayout(false);
			this.functionSplitContainer.Panel1.ResumeLayout(false);
			this.functionSplitContainer.Panel2.ResumeLayout(false);
			this.functionSplitContainer.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.SplitContainer messagesSplitContainer;
		private System.Windows.Forms.ListView messageListView;
		private System.Windows.Forms.ColumnHeader infoCH;
		private System.Windows.Forms.ListView callbackListView;
		private System.Windows.Forms.ColumnHeader MessageCH;
		private System.Windows.Forms.ColumnHeader keyCH;
		private System.Windows.Forms.ColumnHeader MessageTypeCH;
		private System.Windows.Forms.ColumnHeader DateTimeCH;
		private System.Windows.Forms.ColumnHeader errorCodeCH;
		private System.Windows.Forms.SplitContainer mainSplitContainer;
		private System.Windows.Forms.SplitContainer functionSplitContainer;
		private System.Windows.Forms.TreeView functionTreeView;
		private System.Windows.Forms.PropertyGrid methodPropertyGrid;
		private methodTestPanel methodTestPanel1;
	}
}

