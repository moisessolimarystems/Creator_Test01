namespace Shared.VisualComponents
{
    partial class EventLogEntryControl
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.mainBackPanel = new System.Windows.Forms.Panel();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.label1 = new System.Windows.Forms.Label();
            this.evtLogName = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.evtLogLogged = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.evtLogLevel = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.evtLogSource = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.evtLogEventId = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.evtLogTask = new System.Windows.Forms.Label();
            this.label17 = new System.Windows.Forms.Label();
            this.evtLogUser = new System.Windows.Forms.Label();
            this.label19 = new System.Windows.Forms.Label();
            this.evtLogComputer = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.evtLogKeywords = new System.Windows.Forms.Label();
            this.mainBackPanel.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // mainBackPanel
            // 
            this.mainBackPanel.Controls.Add(this.tabControl1);
            this.mainBackPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mainBackPanel.Location = new System.Drawing.Point(0, 0);
            this.mainBackPanel.Name = "mainBackPanel";
            this.mainBackPanel.Padding = new System.Windows.Forms.Padding(3);
            this.mainBackPanel.Size = new System.Drawing.Size(423, 374);
            this.mainBackPanel.TabIndex = 0;
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Location = new System.Drawing.Point(3, 3);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(417, 368);
            this.tabControl1.TabIndex = 2;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.richTextBox1);
            this.tabPage1.Controls.Add(this.tableLayoutPanel1);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Margin = new System.Windows.Forms.Padding(5);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(5);
            this.tabPage1.Size = new System.Drawing.Size(409, 342);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Event";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // richTextBox1
            // 
            this.richTextBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.richTextBox1.Location = new System.Drawing.Point(5, 70);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.ReadOnly = true;
            this.richTextBox1.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.ForcedBoth;
            this.richTextBox1.Size = new System.Drawing.Size(399, 267);
            this.richTextBox1.TabIndex = 0;
            this.richTextBox1.Text = "";
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 4;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 80F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 80F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.Controls.Add(this.label1, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.evtLogName, 1, 0);
            this.tableLayoutPanel1.Controls.Add(this.label7, 2, 1);
            this.tableLayoutPanel1.Controls.Add(this.evtLogLogged, 3, 1);
            this.tableLayoutPanel1.Controls.Add(this.label13, 2, 0);
            this.tableLayoutPanel1.Controls.Add(this.evtLogLevel, 3, 0);
            this.tableLayoutPanel1.Controls.Add(this.label5, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.evtLogSource, 1, 1);
            this.tableLayoutPanel1.Controls.Add(this.label9, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.evtLogEventId, 1, 2);
            this.tableLayoutPanel1.Controls.Add(this.label11, 2, 2);
            this.tableLayoutPanel1.Controls.Add(this.evtLogTask, 3, 2);
            this.tableLayoutPanel1.Controls.Add(this.label17, 2, 3);
            this.tableLayoutPanel1.Controls.Add(this.evtLogUser, 3, 3);
            this.tableLayoutPanel1.Controls.Add(this.label19, 0, 3);
            this.tableLayoutPanel1.Controls.Add(this.evtLogComputer, 1, 3);
            this.tableLayoutPanel1.Controls.Add(this.label15, 0, 4);
            this.tableLayoutPanel1.Controls.Add(this.evtLogKeywords, 1, 4);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(5, 5);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 13;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 15F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 15F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 15F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 15F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 15F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 15F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 15F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 15F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 15F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 15F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 15F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 15F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 15F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(399, 65);
            this.tableLayoutPanel1.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(3, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(59, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Log Name:";
            // 
            // evtLogName
            // 
            this.evtLogName.AutoSize = true;
            this.evtLogName.Location = new System.Drawing.Point(83, 0);
            this.evtLogName.Name = "evtLogName";
            this.evtLogName.Size = new System.Drawing.Size(68, 13);
            this.evtLogName.TabIndex = 1;
            this.evtLogName.Text = "evtLogName";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(202, 15);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(46, 13);
            this.label7.TabIndex = 6;
            this.label7.Text = "Logged:";
            // 
            // evtLogLogged
            // 
            this.evtLogLogged.AutoSize = true;
            this.evtLogLogged.Location = new System.Drawing.Point(282, 15);
            this.evtLogLogged.Name = "evtLogLogged";
            this.evtLogLogged.Size = new System.Drawing.Size(76, 13);
            this.evtLogLogged.TabIndex = 7;
            this.evtLogLogged.Text = "evtLogLogged";
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(202, 0);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(36, 13);
            this.label13.TabIndex = 12;
            this.label13.Text = "Level:";
            // 
            // evtLogLevel
            // 
            this.evtLogLevel.AutoSize = true;
            this.evtLogLevel.Location = new System.Drawing.Point(282, 0);
            this.evtLogLevel.Name = "evtLogLevel";
            this.evtLogLevel.Size = new System.Drawing.Size(66, 13);
            this.evtLogLevel.TabIndex = 13;
            this.evtLogLevel.Text = "evtLogLevel";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(3, 15);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(44, 13);
            this.label5.TabIndex = 4;
            this.label5.Text = "Source:";
            // 
            // evtLogSource
            // 
            this.evtLogSource.AutoSize = true;
            this.evtLogSource.Location = new System.Drawing.Point(83, 15);
            this.evtLogSource.Name = "evtLogSource";
            this.evtLogSource.Size = new System.Drawing.Size(74, 13);
            this.evtLogSource.TabIndex = 5;
            this.evtLogSource.Text = "evtLogSource";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(3, 30);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(52, 13);
            this.label9.TabIndex = 8;
            this.label9.Text = "Event ID:";
            // 
            // evtLogEventId
            // 
            this.evtLogEventId.AutoSize = true;
            this.evtLogEventId.Location = new System.Drawing.Point(83, 30);
            this.evtLogEventId.Name = "evtLogEventId";
            this.evtLogEventId.Size = new System.Drawing.Size(77, 13);
            this.evtLogEventId.TabIndex = 9;
            this.evtLogEventId.Text = "evtLogEventId";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(202, 30);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(52, 15);
            this.label11.TabIndex = 10;
            this.label11.Text = "Task Category:";
            // 
            // evtLogTask
            // 
            this.evtLogTask.AutoSize = true;
            this.evtLogTask.Location = new System.Drawing.Point(282, 30);
            this.evtLogTask.Name = "evtLogTask";
            this.evtLogTask.Size = new System.Drawing.Size(64, 13);
            this.evtLogTask.TabIndex = 11;
            this.evtLogTask.Text = "evtLogTask";
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(202, 45);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(32, 13);
            this.label17.TabIndex = 16;
            this.label17.Text = "User:";
            // 
            // evtLogUser
            // 
            this.evtLogUser.AutoSize = true;
            this.evtLogUser.Location = new System.Drawing.Point(282, 45);
            this.evtLogUser.Name = "evtLogUser";
            this.evtLogUser.Size = new System.Drawing.Size(62, 13);
            this.evtLogUser.TabIndex = 17;
            this.evtLogUser.Text = "evtLogUser";
            // 
            // label19
            // 
            this.label19.AutoSize = true;
            this.label19.Location = new System.Drawing.Point(3, 45);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(55, 13);
            this.label19.TabIndex = 18;
            this.label19.Text = "Computer:";
            // 
            // evtLogComputer
            // 
            this.evtLogComputer.AutoSize = true;
            this.evtLogComputer.Location = new System.Drawing.Point(83, 45);
            this.evtLogComputer.Name = "evtLogComputer";
            this.evtLogComputer.Size = new System.Drawing.Size(85, 13);
            this.evtLogComputer.TabIndex = 19;
            this.evtLogComputer.Text = "evtLogComputer";
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(3, 60);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(62, 13);
            this.label15.TabIndex = 14;
            this.label15.Text = "Key Words:";
            this.label15.Visible = false;
            // 
            // evtLogKeywords
            // 
            this.evtLogKeywords.AutoSize = true;
            this.evtLogKeywords.Location = new System.Drawing.Point(83, 60);
            this.evtLogKeywords.Name = "evtLogKeywords";
            this.evtLogKeywords.Size = new System.Drawing.Size(86, 13);
            this.evtLogKeywords.TabIndex = 15;
            this.evtLogKeywords.Text = "evtLogKeywords";
            this.evtLogKeywords.Visible = false;
            // 
            // EventLogEntryControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.mainBackPanel);
            this.Name = "EventLogEntryControl";
            this.Size = new System.Drawing.Size(423, 374);
            this.mainBackPanel.ResumeLayout(false);
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel mainBackPanel;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.RichTextBox richTextBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label evtLogName;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label evtLogSource;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label evtLogLogged;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label evtLogEventId;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label evtLogTask;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label evtLogLevel;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Label evtLogKeywords;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.Label evtLogUser;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.Label evtLogComputer;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
    }
}
