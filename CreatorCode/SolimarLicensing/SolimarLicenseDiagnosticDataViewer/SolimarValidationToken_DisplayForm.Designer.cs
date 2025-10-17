namespace SolimarLicenseDiagnosticDataViewer
{
    partial class SolimarValidationToken_DisplayForm
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
            this.mainBackPanel = new System.Windows.Forms.Panel();
            this.propertyGrid1 = new System.Windows.Forms.PropertyGrid();
            this.tokenRetrieverTitleLabel = new System.Windows.Forms.Label();
            this.tokenRetrieverLabel = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.mainBackPanel.SuspendLayout();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // mainBackPanel
            // 
            this.mainBackPanel.Controls.Add(this.propertyGrid1);
            this.mainBackPanel.Controls.Add(this.panel1);
            this.mainBackPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mainBackPanel.Location = new System.Drawing.Point(0, 0);
            this.mainBackPanel.Name = "mainBackPanel";
            this.mainBackPanel.Size = new System.Drawing.Size(583, 252);
            this.mainBackPanel.TabIndex = 2;
            // 
            // propertyGrid1
            // 
            this.propertyGrid1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.propertyGrid1.HelpVisible = false;
            this.propertyGrid1.Location = new System.Drawing.Point(0, 32);
            this.propertyGrid1.Name = "propertyGrid1";
            this.propertyGrid1.PropertySort = System.Windows.Forms.PropertySort.NoSort;
            this.propertyGrid1.Size = new System.Drawing.Size(583, 220);
            this.propertyGrid1.TabIndex = 13;
            this.propertyGrid1.ToolbarVisible = false;
            this.propertyGrid1.ViewForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(1)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            // 
            // tokenRetrieverTitleLabel
            // 
            this.tokenRetrieverTitleLabel.AutoSize = true;
            this.tokenRetrieverTitleLabel.Location = new System.Drawing.Point(3, 9);
            this.tokenRetrieverTitleLabel.Name = "tokenRetrieverTitleLabel";
            this.tokenRetrieverTitleLabel.Size = new System.Drawing.Size(125, 13);
            this.tokenRetrieverTitleLabel.TabIndex = 14;
            this.tokenRetrieverTitleLabel.Text = "Token Retreiver Version:";
            // 
            // tokenRetrieverLabel
            // 
            this.tokenRetrieverLabel.AutoSize = true;
            this.tokenRetrieverLabel.Location = new System.Drawing.Point(134, 9);
            this.tokenRetrieverLabel.Name = "tokenRetrieverLabel";
            this.tokenRetrieverLabel.Size = new System.Drawing.Size(94, 13);
            this.tokenRetrieverLabel.TabIndex = 15;
            this.tokenRetrieverLabel.Text = "[Retriever Version]";
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.tokenRetrieverTitleLabel);
            this.panel1.Controls.Add(this.tokenRetrieverLabel);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(583, 32);
            this.panel1.TabIndex = 16;
            // 
            // SolimarValidationToken_DisplayForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(583, 252);
            this.Controls.Add(this.mainBackPanel);
            this.Name = "SolimarValidationToken_DisplayForm";
            this.ShowIcon = false;
            this.Text = "Solimar Validation Token";
            this.mainBackPanel.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel mainBackPanel;
        private System.Windows.Forms.PropertyGrid propertyGrid1;
        private System.Windows.Forms.Label tokenRetrieverLabel;
        private System.Windows.Forms.Label tokenRetrieverTitleLabel;
        private System.Windows.Forms.Panel panel1;
    }
}