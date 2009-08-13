namespace Client.Creator
{
    partial class LicenseInfoForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(LicenseInfoForm));
            this.topPanel = new System.Windows.Forms.Panel();
            this.licInfoTabControl = new System.Windows.Forms.TabControl();
            this.LicenseTabPage = new System.Windows.Forms.TabPage();
            this.licenseGroupBox = new System.Windows.Forms.GroupBox();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.licDescriptLabel = new System.Windows.Forms.Label();
            this.licTypeLabel = new System.Windows.Forms.Label();
            this.licDestIDLabel = new System.Windows.Forms.Label();
            this.licDescriptTextBox = new System.Windows.Forms.TextBox();
            this.licTypeComboBox = new System.Windows.Forms.ComboBox();
            this.destNameComboBox = new System.Windows.Forms.ComboBox();
            this.licNameTextBox = new System.Windows.Forms.TextBox();
            this.licNameLabel = new System.Windows.Forms.Label();
            this.TokenTabPage = new System.Windows.Forms.TabPage();
            this.tokenGroupBox = new System.Windows.Forms.GroupBox();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.tokenTypeLabel = new System.Windows.Forms.Label();
            this.tokenValueTextBox = new System.Windows.Forms.TextBox();
            this.tokenTypeComboBox = new System.Windows.Forms.ComboBox();
            this.tokenValueLabel = new System.Windows.Forms.Label();
            this.availableHWKeyLabel = new System.Windows.Forms.Label();
            this.browseTokenFileButton = new System.Windows.Forms.Button();
            this.keyNameListView = new System.Windows.Forms.ListView();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
            this.ModuleTabPage = new System.Windows.Forms.TabPage();
            this.moduleGroupBox = new System.Windows.Forms.GroupBox();
            this.tableLayoutPanel3 = new System.Windows.Forms.TableLayoutPanel();
            this.modNameLabel = new System.Windows.Forms.Label();
            this.modValueLabel = new System.Windows.Forms.Label();
            this.modNameComboBox = new System.Windows.Forms.ComboBox();
            this.modValueNumericUpDown = new System.Windows.Forms.NumericUpDown();
            this.PacketTabPage = new System.Windows.Forms.TabPage();
            this.packetGroupBox = new System.Windows.Forms.GroupBox();
            this.tableLayoutPanel4 = new System.Windows.Forms.TableLayoutPanel();
            this.packetNameLabel = new System.Windows.Forms.Label();
            this.folderBrowseButton = new System.Windows.Forms.Button();
            this.packetNameTextBox = new System.Windows.Forms.TextBox();
            this.packetDescriptTextBox = new System.Windows.Forms.TextBox();
            this.packetDateLabel = new System.Windows.Forms.Label();
            this.descriptionLabel = new System.Windows.Forms.Label();
            this.packetOutputPathTextBox = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.packetExpDateTextBox = new System.Windows.Forms.TextBox();
            this.orderTabPage = new System.Windows.Forms.TabPage();
            this.orderGroupBox = new System.Windows.Forms.GroupBox();
            this.tableLayoutPanel5 = new System.Windows.Forms.TableLayoutPanel();
            this.label4 = new System.Windows.Forms.Label();
            this.productLicenseDescriptionTextBox = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.expDateTextBox = new System.Windows.Forms.TextBox();
            this.productLicenseNumberTextBox = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.productLicenseVersionMaskedTextBox = new System.Windows.Forms.MaskedTextBox();
            this.ProductLicenseTypeComboBox = new System.Windows.Forms.ComboBox();
            this.ProductLicenseProductComboBox = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.folderBrowserDialog = new System.Windows.Forms.FolderBrowserDialog();
            this.errorProvider1 = new System.Windows.Forms.ErrorProvider(this.components);
            this.browseCSVOpenFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.topPanel.SuspendLayout();
            this.licInfoTabControl.SuspendLayout();
            this.LicenseTabPage.SuspendLayout();
            this.licenseGroupBox.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.TokenTabPage.SuspendLayout();
            this.tokenGroupBox.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            this.ModuleTabPage.SuspendLayout();
            this.moduleGroupBox.SuspendLayout();
            this.tableLayoutPanel3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.modValueNumericUpDown)).BeginInit();
            this.PacketTabPage.SuspendLayout();
            this.packetGroupBox.SuspendLayout();
            this.tableLayoutPanel4.SuspendLayout();
            this.orderTabPage.SuspendLayout();
            this.orderGroupBox.SuspendLayout();
            this.tableLayoutPanel5.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).BeginInit();
            this.SuspendLayout();
            // 
            // btnCancel
            // 
            this.btnCancel.Location = new System.Drawing.Point(78, 5);
            this.btnCancel.Size = new System.Drawing.Size(72, 23);
            // 
            // btnOk
            // 
            this.btnOk.Location = new System.Drawing.Point(3, 5);
            this.btnOk.Size = new System.Drawing.Size(72, 23);
            this.btnOk.Click += new System.EventHandler(this.btnOk_Click);
            // 
            // topPanel
            // 
            this.topPanel.BackColor = System.Drawing.SystemColors.Control;
            this.topPanel.Controls.Add(this.licInfoTabControl);
            this.topPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.topPanel.Location = new System.Drawing.Point(0, 0);
            this.topPanel.Name = "topPanel";
            this.topPanel.Padding = new System.Windows.Forms.Padding(20, 10, 20, 0);
            this.topPanel.Size = new System.Drawing.Size(475, 293);
            this.topPanel.TabIndex = 3;
            // 
            // licInfoTabControl
            // 
            this.licInfoTabControl.Controls.Add(this.LicenseTabPage);
            this.licInfoTabControl.Controls.Add(this.TokenTabPage);
            this.licInfoTabControl.Controls.Add(this.ModuleTabPage);
            this.licInfoTabControl.Controls.Add(this.PacketTabPage);
            this.licInfoTabControl.Controls.Add(this.orderTabPage);
            this.licInfoTabControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.licInfoTabControl.Location = new System.Drawing.Point(20, 10);
            this.licInfoTabControl.Margin = new System.Windows.Forms.Padding(1);
            this.licInfoTabControl.Name = "licInfoTabControl";
            this.licInfoTabControl.SelectedIndex = 0;
            this.licInfoTabControl.Size = new System.Drawing.Size(435, 283);
            this.licInfoTabControl.TabIndex = 9;
            // 
            // LicenseTabPage
            // 
            this.LicenseTabPage.Controls.Add(this.licenseGroupBox);
            this.LicenseTabPage.Location = new System.Drawing.Point(4, 22);
            this.LicenseTabPage.Name = "LicenseTabPage";
            this.LicenseTabPage.Padding = new System.Windows.Forms.Padding(3);
            this.LicenseTabPage.Size = new System.Drawing.Size(427, 257);
            this.LicenseTabPage.TabIndex = 0;
            this.LicenseTabPage.Text = "License";
            this.LicenseTabPage.UseVisualStyleBackColor = true;
            // 
            // licenseGroupBox
            // 
            this.licenseGroupBox.BackColor = System.Drawing.Color.Transparent;
            this.licenseGroupBox.Controls.Add(this.tableLayoutPanel1);
            this.licenseGroupBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.licenseGroupBox.Location = new System.Drawing.Point(3, 3);
            this.licenseGroupBox.Name = "licenseGroupBox";
            this.licenseGroupBox.Padding = new System.Windows.Forms.Padding(10);
            this.licenseGroupBox.Size = new System.Drawing.Size(421, 251);
            this.licenseGroupBox.TabIndex = 14;
            this.licenseGroupBox.TabStop = false;
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.BackColor = System.Drawing.Color.Transparent;
            this.tableLayoutPanel1.ColumnCount = 2;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 271F));
            this.tableLayoutPanel1.Controls.Add(this.licDescriptLabel, 0, 4);
            this.tableLayoutPanel1.Controls.Add(this.licTypeLabel, 0, 3);
            this.tableLayoutPanel1.Controls.Add(this.licDestIDLabel, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.licDescriptTextBox, 1, 4);
            this.tableLayoutPanel1.Controls.Add(this.licTypeComboBox, 1, 3);
            this.tableLayoutPanel1.Controls.Add(this.destNameComboBox, 1, 2);
            this.tableLayoutPanel1.Controls.Add(this.licNameTextBox, 1, 0);
            this.tableLayoutPanel1.Controls.Add(this.licNameLabel, 0, 0);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(10, 23);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 6;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 89F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(401, 218);
            this.tableLayoutPanel1.TabIndex = 13;
            // 
            // licDescriptLabel
            // 
            this.licDescriptLabel.AutoSize = true;
            this.licDescriptLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.licDescriptLabel.Location = new System.Drawing.Point(3, 102);
            this.licDescriptLabel.Margin = new System.Windows.Forms.Padding(3, 10, 3, 0);
            this.licDescriptLabel.Name = "licDescriptLabel";
            this.licDescriptLabel.Size = new System.Drawing.Size(75, 13);
            this.licDescriptLabel.TabIndex = 8;
            this.licDescriptLabel.Text = "Description:";
            // 
            // licTypeLabel
            // 
            this.licTypeLabel.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.licTypeLabel.AutoSize = true;
            this.licTypeLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.licTypeLabel.Location = new System.Drawing.Point(3, 71);
            this.licTypeLabel.Margin = new System.Windows.Forms.Padding(3, 3, 3, 0);
            this.licTypeLabel.Name = "licTypeLabel";
            this.licTypeLabel.Size = new System.Drawing.Size(87, 13);
            this.licTypeLabel.TabIndex = 7;
            this.licTypeLabel.Text = "License Type:";
            // 
            // licDestIDLabel
            // 
            this.licDestIDLabel.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.licDestIDLabel.AutoSize = true;
            this.licDestIDLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.licDestIDLabel.Location = new System.Drawing.Point(3, 40);
            this.licDestIDLabel.Margin = new System.Windows.Forms.Padding(3, 3, 3, 0);
            this.licDestIDLabel.Name = "licDestIDLabel";
            this.licDestIDLabel.Size = new System.Drawing.Size(111, 13);
            this.licDestIDLabel.TabIndex = 3;
            this.licDestIDLabel.Text = "Destination Name:";
            // 
            // licDescriptTextBox
            // 
            this.licDescriptTextBox.BackColor = System.Drawing.SystemColors.Window;
            this.licDescriptTextBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.licDescriptTextBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.licDescriptTextBox.Location = new System.Drawing.Point(135, 97);
            this.licDescriptTextBox.Margin = new System.Windows.Forms.Padding(5);
            this.licDescriptTextBox.Multiline = true;
            this.licDescriptTextBox.Name = "licDescriptTextBox";
            this.licDescriptTextBox.Size = new System.Drawing.Size(261, 79);
            this.licDescriptTextBox.TabIndex = 9;
            // 
            // licTypeComboBox
            // 
            this.licTypeComboBox.BackColor = System.Drawing.SystemColors.Window;
            this.licTypeComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.licTypeComboBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.licTypeComboBox.FormattingEnabled = true;
            this.licTypeComboBox.Location = new System.Drawing.Point(135, 66);
            this.licTypeComboBox.Margin = new System.Windows.Forms.Padding(5);
            this.licTypeComboBox.Name = "licTypeComboBox";
            this.licTypeComboBox.Size = new System.Drawing.Size(188, 21);
            this.licTypeComboBox.TabIndex = 6;
            this.licTypeComboBox.SelectedIndexChanged += new System.EventHandler(this.licTypeComboBox_SelectedIndexChanged);
            // 
            // destNameComboBox
            // 
            this.destNameComboBox.BackColor = System.Drawing.SystemColors.Window;
            this.destNameComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.destNameComboBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.destNameComboBox.FormattingEnabled = true;
            this.destNameComboBox.Location = new System.Drawing.Point(135, 35);
            this.destNameComboBox.Margin = new System.Windows.Forms.Padding(5);
            this.destNameComboBox.Name = "destNameComboBox";
            this.destNameComboBox.Size = new System.Drawing.Size(188, 21);
            this.destNameComboBox.TabIndex = 12;
            this.destNameComboBox.SelectionChangeCommitted += new System.EventHandler(this.destNameComboBox_SelectionChangeCommitted);
            // 
            // licNameTextBox
            // 
            this.licNameTextBox.BackColor = System.Drawing.SystemColors.Window;
            this.licNameTextBox.Enabled = false;
            this.licNameTextBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.licNameTextBox.Location = new System.Drawing.Point(135, 5);
            this.licNameTextBox.Margin = new System.Windows.Forms.Padding(5);
            this.licNameTextBox.Name = "licNameTextBox";
            this.licNameTextBox.ReadOnly = true;
            this.licNameTextBox.Size = new System.Drawing.Size(188, 20);
            this.licNameTextBox.TabIndex = 1;
            // 
            // licNameLabel
            // 
            this.licNameLabel.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.licNameLabel.AutoSize = true;
            this.licNameLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.licNameLabel.Location = new System.Drawing.Point(3, 10);
            this.licNameLabel.Margin = new System.Windows.Forms.Padding(3, 3, 3, 0);
            this.licNameLabel.Name = "licNameLabel";
            this.licNameLabel.Size = new System.Drawing.Size(55, 13);
            this.licNameLabel.TabIndex = 0;
            this.licNameLabel.Text = "License:";
            // 
            // TokenTabPage
            // 
            this.TokenTabPage.Controls.Add(this.tokenGroupBox);
            this.TokenTabPage.Location = new System.Drawing.Point(4, 22);
            this.TokenTabPage.Name = "TokenTabPage";
            this.TokenTabPage.Padding = new System.Windows.Forms.Padding(3);
            this.TokenTabPage.Size = new System.Drawing.Size(427, 257);
            this.TokenTabPage.TabIndex = 1;
            this.TokenTabPage.Text = "Token";
            this.TokenTabPage.UseVisualStyleBackColor = true;
            // 
            // tokenGroupBox
            // 
            this.tokenGroupBox.Controls.Add(this.tableLayoutPanel2);
            this.tokenGroupBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tokenGroupBox.Location = new System.Drawing.Point(3, 3);
            this.tokenGroupBox.Name = "tokenGroupBox";
            this.tokenGroupBox.Padding = new System.Windows.Forms.Padding(10);
            this.tokenGroupBox.Size = new System.Drawing.Size(421, 251);
            this.tokenGroupBox.TabIndex = 4;
            this.tokenGroupBox.TabStop = false;
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.ColumnCount = 3;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 46.08696F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 53.91304F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 79F));
            this.tableLayoutPanel2.Controls.Add(this.tokenTypeLabel, 0, 0);
            this.tableLayoutPanel2.Controls.Add(this.tokenValueTextBox, 1, 1);
            this.tableLayoutPanel2.Controls.Add(this.tokenTypeComboBox, 1, 0);
            this.tableLayoutPanel2.Controls.Add(this.tokenValueLabel, 0, 1);
            this.tableLayoutPanel2.Controls.Add(this.availableHWKeyLabel, 0, 2);
            this.tableLayoutPanel2.Controls.Add(this.browseTokenFileButton, 2, 1);
            this.tableLayoutPanel2.Controls.Add(this.keyNameListView, 1, 2);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(10, 23);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 4;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 54F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(401, 218);
            this.tableLayoutPanel2.TabIndex = 8;
            // 
            // tokenTypeLabel
            // 
            this.tokenTypeLabel.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.tokenTypeLabel.AutoSize = true;
            this.tokenTypeLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tokenTypeLabel.Location = new System.Drawing.Point(3, 2);
            this.tokenTypeLabel.Name = "tokenTypeLabel";
            this.tokenTypeLabel.Size = new System.Drawing.Size(113, 26);
            this.tokenTypeLabel.TabIndex = 3;
            this.tokenTypeLabel.Text = "Choose Validation Type:";
            // 
            // tokenValueTextBox
            // 
            this.tokenValueTextBox.CausesValidation = false;
            this.tokenValueTextBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tokenValueTextBox.Location = new System.Drawing.Point(153, 36);
            this.tokenValueTextBox.Margin = new System.Windows.Forms.Padding(5, 5, 0, 5);
            this.tokenValueTextBox.Name = "tokenValueTextBox";
            this.tokenValueTextBox.Size = new System.Drawing.Size(168, 20);
            this.tokenValueTextBox.TabIndex = 0;
            this.tokenValueTextBox.TextChanged += new System.EventHandler(this.tokenValueTextBox_TextChanged);
            this.tokenValueTextBox.KeyUp += new System.Windows.Forms.KeyEventHandler(this.tokenValueTextBox_KeyUp);
            // 
            // tokenTypeComboBox
            // 
            this.tokenTypeComboBox.BackColor = System.Drawing.SystemColors.Window;
            this.tokenTypeComboBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tokenTypeComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.tokenTypeComboBox.FormattingEnabled = true;
            this.tokenTypeComboBox.Location = new System.Drawing.Point(153, 5);
            this.tokenTypeComboBox.Margin = new System.Windows.Forms.Padding(5, 5, 0, 5);
            this.tokenTypeComboBox.Name = "tokenTypeComboBox";
            this.tokenTypeComboBox.Size = new System.Drawing.Size(168, 21);
            this.tokenTypeComboBox.TabIndex = 1;
            this.tokenTypeComboBox.SelectedIndexChanged += new System.EventHandler(this.tokenTypeComboBox_SelectedIndexChanged);
            this.tokenTypeComboBox.KeyUp += new System.Windows.Forms.KeyEventHandler(this.tokenTypeComboBox_KeyUp);
            // 
            // tokenValueLabel
            // 
            this.tokenValueLabel.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.tokenValueLabel.AutoSize = true;
            this.tokenValueLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tokenValueLabel.Location = new System.Drawing.Point(3, 41);
            this.tokenValueLabel.Margin = new System.Windows.Forms.Padding(3, 3, 3, 0);
            this.tokenValueLabel.Name = "tokenValueLabel";
            this.tokenValueLabel.Size = new System.Drawing.Size(83, 13);
            this.tokenValueLabel.TabIndex = 4;
            this.tokenValueLabel.Text = "Token Value:";
            // 
            // availableHWKeyLabel
            // 
            this.availableHWKeyLabel.AutoSize = true;
            this.availableHWKeyLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.availableHWKeyLabel.Location = new System.Drawing.Point(3, 71);
            this.availableHWKeyLabel.Margin = new System.Windows.Forms.Padding(3, 10, 3, 0);
            this.availableHWKeyLabel.Name = "availableHWKeyLabel";
            this.availableHWKeyLabel.Size = new System.Drawing.Size(121, 26);
            this.availableHWKeyLabel.TabIndex = 8;
            this.availableHWKeyLabel.Text = "Available Hardware Keys:";
            // 
            // browseTokenFileButton
            // 
            this.browseTokenFileButton.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.browseTokenFileButton.Location = new System.Drawing.Point(321, 35);
            this.browseTokenFileButton.Margin = new System.Windows.Forms.Padding(0, 3, 3, 3);
            this.browseTokenFileButton.Name = "browseTokenFileButton";
            this.browseTokenFileButton.Size = new System.Drawing.Size(25, 22);
            this.browseTokenFileButton.TabIndex = 9;
            this.browseTokenFileButton.Text = "...";
            this.browseTokenFileButton.UseVisualStyleBackColor = true;
            this.browseTokenFileButton.Click += new System.EventHandler(this.browseTokenFileButton_Click);
            // 
            // keyNameListView
            // 
            this.keyNameListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
            this.tableLayoutPanel2.SetColumnSpan(this.keyNameListView, 2);
            this.keyNameListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.keyNameListView.FullRowSelect = true;
            this.keyNameListView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            this.keyNameListView.Location = new System.Drawing.Point(153, 66);
            this.keyNameListView.Margin = new System.Windows.Forms.Padding(5);
            this.keyNameListView.MultiSelect = false;
            this.keyNameListView.Name = "keyNameListView";
            this.keyNameListView.Size = new System.Drawing.Size(243, 97);
            this.keyNameListView.TabIndex = 10;
            this.keyNameListView.UseCompatibleStateImageBehavior = false;
            this.keyNameListView.View = System.Windows.Forms.View.Details;
            this.keyNameListView.SelectedIndexChanged += new System.EventHandler(this.keyNameListView_SelectedIndexChanged);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Name";
            this.columnHeader1.Width = 162;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Status";
            this.columnHeader2.Width = 75;
            // 
            // ModuleTabPage
            // 
            this.ModuleTabPage.Controls.Add(this.moduleGroupBox);
            this.ModuleTabPage.Location = new System.Drawing.Point(4, 22);
            this.ModuleTabPage.Name = "ModuleTabPage";
            this.ModuleTabPage.Size = new System.Drawing.Size(427, 257);
            this.ModuleTabPage.TabIndex = 2;
            this.ModuleTabPage.Text = "Module";
            this.ModuleTabPage.UseVisualStyleBackColor = true;
            // 
            // moduleGroupBox
            // 
            this.moduleGroupBox.Controls.Add(this.tableLayoutPanel3);
            this.moduleGroupBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.moduleGroupBox.Location = new System.Drawing.Point(0, 0);
            this.moduleGroupBox.Name = "moduleGroupBox";
            this.moduleGroupBox.Padding = new System.Windows.Forms.Padding(10);
            this.moduleGroupBox.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.moduleGroupBox.Size = new System.Drawing.Size(427, 257);
            this.moduleGroupBox.TabIndex = 8;
            this.moduleGroupBox.TabStop = false;
            // 
            // tableLayoutPanel3
            // 
            this.tableLayoutPanel3.BackColor = System.Drawing.Color.Transparent;
            this.tableLayoutPanel3.ColumnCount = 3;
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 128F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 79F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 200F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel3.Controls.Add(this.modNameLabel, 0, 0);
            this.tableLayoutPanel3.Controls.Add(this.modValueLabel, 0, 1);
            this.tableLayoutPanel3.Controls.Add(this.modNameComboBox, 1, 0);
            this.tableLayoutPanel3.Controls.Add(this.modValueNumericUpDown, 1, 1);
            this.tableLayoutPanel3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel3.Location = new System.Drawing.Point(10, 23);
            this.tableLayoutPanel3.Name = "tableLayoutPanel3";
            this.tableLayoutPanel3.RowCount = 3;
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 30F));
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 30F));
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel3.Size = new System.Drawing.Size(407, 224);
            this.tableLayoutPanel3.TabIndex = 9;
            // 
            // modNameLabel
            // 
            this.modNameLabel.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.modNameLabel.AutoSize = true;
            this.modNameLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.modNameLabel.Location = new System.Drawing.Point(3, 8);
            this.modNameLabel.Name = "modNameLabel";
            this.modNameLabel.Size = new System.Drawing.Size(88, 13);
            this.modNameLabel.TabIndex = 0;
            this.modNameLabel.Text = "Module Name:";
            // 
            // modValueLabel
            // 
            this.modValueLabel.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.modValueLabel.AutoSize = true;
            this.modValueLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.modValueLabel.Location = new System.Drawing.Point(3, 38);
            this.modValueLabel.Name = "modValueLabel";
            this.modValueLabel.Size = new System.Drawing.Size(102, 13);
            this.modValueLabel.TabIndex = 1;
            this.modValueLabel.Text = "Number of Units:";
            // 
            // modNameComboBox
            // 
            this.modNameComboBox.BackColor = System.Drawing.SystemColors.Window;
            this.tableLayoutPanel3.SetColumnSpan(this.modNameComboBox, 2);
            this.modNameComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.modNameComboBox.FormattingEnabled = true;
            this.modNameComboBox.Location = new System.Drawing.Point(133, 5);
            this.modNameComboBox.Margin = new System.Windows.Forms.Padding(5);
            this.modNameComboBox.Name = "modNameComboBox";
            this.modNameComboBox.Size = new System.Drawing.Size(151, 21);
            this.modNameComboBox.TabIndex = 5;
            // 
            // modValueNumericUpDown
            // 
            this.tableLayoutPanel3.SetColumnSpan(this.modValueNumericUpDown, 2);
            this.modValueNumericUpDown.Location = new System.Drawing.Point(133, 35);
            this.modValueNumericUpDown.Margin = new System.Windows.Forms.Padding(5);
            this.modValueNumericUpDown.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.modValueNumericUpDown.Name = "modValueNumericUpDown";
            this.modValueNumericUpDown.Size = new System.Drawing.Size(69, 20);
            this.modValueNumericUpDown.TabIndex = 6;
            // 
            // PacketTabPage
            // 
            this.PacketTabPage.Controls.Add(this.packetGroupBox);
            this.PacketTabPage.Location = new System.Drawing.Point(4, 22);
            this.PacketTabPage.Name = "PacketTabPage";
            this.PacketTabPage.Size = new System.Drawing.Size(427, 257);
            this.PacketTabPage.TabIndex = 3;
            this.PacketTabPage.Text = "Packet";
            this.PacketTabPage.UseVisualStyleBackColor = true;
            // 
            // packetGroupBox
            // 
            this.packetGroupBox.Controls.Add(this.tableLayoutPanel4);
            this.packetGroupBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.packetGroupBox.Location = new System.Drawing.Point(0, 0);
            this.packetGroupBox.Name = "packetGroupBox";
            this.packetGroupBox.Padding = new System.Windows.Forms.Padding(10);
            this.packetGroupBox.Size = new System.Drawing.Size(427, 257);
            this.packetGroupBox.TabIndex = 9;
            this.packetGroupBox.TabStop = false;
            // 
            // tableLayoutPanel4
            // 
            this.tableLayoutPanel4.ColumnCount = 3;
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 29.6496F));
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 70.3504F));
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 52F));
            this.tableLayoutPanel4.Controls.Add(this.packetNameLabel, 0, 0);
            this.tableLayoutPanel4.Controls.Add(this.folderBrowseButton, 2, 2);
            this.tableLayoutPanel4.Controls.Add(this.packetNameTextBox, 1, 0);
            this.tableLayoutPanel4.Controls.Add(this.packetDescriptTextBox, 1, 3);
            this.tableLayoutPanel4.Controls.Add(this.packetDateLabel, 0, 1);
            this.tableLayoutPanel4.Controls.Add(this.descriptionLabel, 0, 3);
            this.tableLayoutPanel4.Controls.Add(this.packetOutputPathTextBox, 1, 2);
            this.tableLayoutPanel4.Controls.Add(this.label3, 0, 2);
            this.tableLayoutPanel4.Controls.Add(this.packetExpDateTextBox, 1, 1);
            this.tableLayoutPanel4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel4.Location = new System.Drawing.Point(10, 23);
            this.tableLayoutPanel4.Name = "tableLayoutPanel4";
            this.tableLayoutPanel4.RowCount = 5;
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 30F));
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 30F));
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 42F));
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 78F));
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 8F));
            this.tableLayoutPanel4.Size = new System.Drawing.Size(407, 224);
            this.tableLayoutPanel4.TabIndex = 0;
            // 
            // packetNameLabel
            // 
            this.packetNameLabel.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.packetNameLabel.AutoSize = true;
            this.packetNameLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.packetNameLabel.Location = new System.Drawing.Point(3, 8);
            this.packetNameLabel.Name = "packetNameLabel";
            this.packetNameLabel.Size = new System.Drawing.Size(87, 13);
            this.packetNameLabel.TabIndex = 0;
            this.packetNameLabel.Text = "Packet Name:";
            // 
            // folderBrowseButton
            // 
            this.folderBrowseButton.Image = ((System.Drawing.Image)(resources.GetObject("folderBrowseButton.Image")));
            this.folderBrowseButton.Location = new System.Drawing.Point(354, 65);
            this.folderBrowseButton.Margin = new System.Windows.Forms.Padding(0, 5, 0, 0);
            this.folderBrowseButton.Name = "folderBrowseButton";
            this.folderBrowseButton.Size = new System.Drawing.Size(25, 25);
            this.folderBrowseButton.TabIndex = 8;
            this.folderBrowseButton.UseVisualStyleBackColor = true;
            this.folderBrowseButton.Click += new System.EventHandler(this.folderBrowseButton_Click);
            // 
            // packetNameTextBox
            // 
            this.packetNameTextBox.BackColor = System.Drawing.SystemColors.Window;
            this.packetNameTextBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.packetNameTextBox.Location = new System.Drawing.Point(110, 5);
            this.packetNameTextBox.Margin = new System.Windows.Forms.Padding(5);
            this.packetNameTextBox.Name = "packetNameTextBox";
            this.packetNameTextBox.ReadOnly = true;
            this.packetNameTextBox.Size = new System.Drawing.Size(239, 20);
            this.packetNameTextBox.TabIndex = 4;
            // 
            // packetDescriptTextBox
            // 
            this.packetDescriptTextBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.packetDescriptTextBox.Location = new System.Drawing.Point(110, 107);
            this.packetDescriptTextBox.Margin = new System.Windows.Forms.Padding(5);
            this.packetDescriptTextBox.Multiline = true;
            this.packetDescriptTextBox.Name = "packetDescriptTextBox";
            this.packetDescriptTextBox.Size = new System.Drawing.Size(239, 68);
            this.packetDescriptTextBox.TabIndex = 7;
            // 
            // packetDateLabel
            // 
            this.packetDateLabel.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.packetDateLabel.AutoSize = true;
            this.packetDateLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.packetDateLabel.Location = new System.Drawing.Point(3, 38);
            this.packetDateLabel.Name = "packetDateLabel";
            this.packetDateLabel.Size = new System.Drawing.Size(98, 13);
            this.packetDateLabel.TabIndex = 1;
            this.packetDateLabel.Text = "Expiration Date:";
            // 
            // descriptionLabel
            // 
            this.descriptionLabel.AutoSize = true;
            this.descriptionLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.descriptionLabel.Location = new System.Drawing.Point(3, 112);
            this.descriptionLabel.Margin = new System.Windows.Forms.Padding(3, 10, 3, 0);
            this.descriptionLabel.Name = "descriptionLabel";
            this.descriptionLabel.Size = new System.Drawing.Size(75, 13);
            this.descriptionLabel.TabIndex = 3;
            this.descriptionLabel.Text = "Description:";
            // 
            // packetOutputPathTextBox
            // 
            this.packetOutputPathTextBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.packetOutputPathTextBox.Location = new System.Drawing.Point(110, 65);
            this.packetOutputPathTextBox.Margin = new System.Windows.Forms.Padding(5);
            this.packetOutputPathTextBox.Multiline = true;
            this.packetOutputPathTextBox.Name = "packetOutputPathTextBox";
            this.packetOutputPathTextBox.Size = new System.Drawing.Size(239, 32);
            this.packetOutputPathTextBox.TabIndex = 6;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(3, 70);
            this.label3.Margin = new System.Windows.Forms.Padding(3, 10, 3, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(60, 26);
            this.label3.TabIndex = 2;
            this.label3.Text = "Packet Location:";
            // 
            // packetExpDateTextBox
            // 
            this.packetExpDateTextBox.BackColor = System.Drawing.SystemColors.Window;
            this.packetExpDateTextBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.packetExpDateTextBox.Location = new System.Drawing.Point(110, 35);
            this.packetExpDateTextBox.Margin = new System.Windows.Forms.Padding(5);
            this.packetExpDateTextBox.Name = "packetExpDateTextBox";
            this.packetExpDateTextBox.ReadOnly = true;
            this.packetExpDateTextBox.Size = new System.Drawing.Size(239, 20);
            this.packetExpDateTextBox.TabIndex = 9;
            // 
            // orderTabPage
            // 
            this.orderTabPage.Controls.Add(this.orderGroupBox);
            this.orderTabPage.Location = new System.Drawing.Point(4, 22);
            this.orderTabPage.Name = "orderTabPage";
            this.orderTabPage.Padding = new System.Windows.Forms.Padding(3);
            this.orderTabPage.Size = new System.Drawing.Size(427, 257);
            this.orderTabPage.TabIndex = 4;
            this.orderTabPage.Text = "Order";
            this.orderTabPage.UseVisualStyleBackColor = true;
            // 
            // orderGroupBox
            // 
            this.orderGroupBox.Controls.Add(this.tableLayoutPanel5);
            this.orderGroupBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.orderGroupBox.Location = new System.Drawing.Point(3, 3);
            this.orderGroupBox.Name = "orderGroupBox";
            this.orderGroupBox.Padding = new System.Windows.Forms.Padding(10);
            this.orderGroupBox.Size = new System.Drawing.Size(421, 251);
            this.orderGroupBox.TabIndex = 14;
            this.orderGroupBox.TabStop = false;
            // 
            // tableLayoutPanel5
            // 
            this.tableLayoutPanel5.ColumnCount = 2;
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 38.90274F));
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 61.09726F));
            this.tableLayoutPanel5.Controls.Add(this.label4, 0, 0);
            this.tableLayoutPanel5.Controls.Add(this.productLicenseDescriptionTextBox, 1, 5);
            this.tableLayoutPanel5.Controls.Add(this.label7, 0, 5);
            this.tableLayoutPanel5.Controls.Add(this.expDateTextBox, 1, 4);
            this.tableLayoutPanel5.Controls.Add(this.productLicenseNumberTextBox, 1, 0);
            this.tableLayoutPanel5.Controls.Add(this.label6, 0, 4);
            this.tableLayoutPanel5.Controls.Add(this.label5, 0, 3);
            this.tableLayoutPanel5.Controls.Add(this.productLicenseVersionMaskedTextBox, 1, 3);
            this.tableLayoutPanel5.Controls.Add(this.ProductLicenseTypeComboBox, 1, 2);
            this.tableLayoutPanel5.Controls.Add(this.ProductLicenseProductComboBox, 1, 1);
            this.tableLayoutPanel5.Controls.Add(this.label1, 0, 2);
            this.tableLayoutPanel5.Controls.Add(this.label2, 0, 1);
            this.tableLayoutPanel5.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel5.Location = new System.Drawing.Point(10, 23);
            this.tableLayoutPanel5.Name = "tableLayoutPanel5";
            this.tableLayoutPanel5.RowCount = 7;
            this.tableLayoutPanel5.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 30F));
            this.tableLayoutPanel5.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 30F));
            this.tableLayoutPanel5.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 30F));
            this.tableLayoutPanel5.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 30F));
            this.tableLayoutPanel5.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 30F));
            this.tableLayoutPanel5.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 61F));
            this.tableLayoutPanel5.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 8F));
            this.tableLayoutPanel5.Size = new System.Drawing.Size(401, 218);
            this.tableLayoutPanel5.TabIndex = 0;
            // 
            // label4
            // 
            this.label4.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(3, 2);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(103, 26);
            this.label4.TabIndex = 5;
            this.label4.Text = "Product License Number:";
            // 
            // productLicenseDescriptionTextBox
            // 
            this.productLicenseDescriptionTextBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.productLicenseDescriptionTextBox.Location = new System.Drawing.Point(160, 155);
            this.productLicenseDescriptionTextBox.Margin = new System.Windows.Forms.Padding(5);
            this.productLicenseDescriptionTextBox.Multiline = true;
            this.productLicenseDescriptionTextBox.Name = "productLicenseDescriptionTextBox";
            this.productLicenseDescriptionTextBox.Size = new System.Drawing.Size(236, 51);
            this.productLicenseDescriptionTextBox.TabIndex = 10;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label7.Location = new System.Drawing.Point(3, 160);
            this.label7.Margin = new System.Windows.Forms.Padding(3, 10, 3, 0);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(75, 13);
            this.label7.TabIndex = 11;
            this.label7.Text = "Description:";
            // 
            // expDateTextBox
            // 
            this.expDateTextBox.Location = new System.Drawing.Point(160, 125);
            this.expDateTextBox.Margin = new System.Windows.Forms.Padding(5);
            this.expDateTextBox.Name = "expDateTextBox";
            this.expDateTextBox.Size = new System.Drawing.Size(52, 20);
            this.expDateTextBox.TabIndex = 13;
            this.expDateTextBox.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.expDateTextBox.TextChanged += new System.EventHandler(this.expDateTextBox_TextChanged);
            // 
            // productLicenseNumberTextBox
            // 
            this.productLicenseNumberTextBox.BackColor = System.Drawing.SystemColors.Window;
            this.productLicenseNumberTextBox.Enabled = false;
            this.productLicenseNumberTextBox.Location = new System.Drawing.Point(160, 5);
            this.productLicenseNumberTextBox.Margin = new System.Windows.Forms.Padding(5);
            this.productLicenseNumberTextBox.Name = "productLicenseNumberTextBox";
            this.productLicenseNumberTextBox.ReadOnly = true;
            this.productLicenseNumberTextBox.Size = new System.Drawing.Size(198, 20);
            this.productLicenseNumberTextBox.TabIndex = 4;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.Location = new System.Drawing.Point(3, 130);
            this.label6.Margin = new System.Windows.Forms.Padding(3, 10, 3, 0);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(94, 13);
            this.label6.TabIndex = 9;
            this.label6.Text = "Days Licensed:";
            // 
            // label5
            // 
            this.label5.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.Location = new System.Drawing.Point(3, 98);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(53, 13);
            this.label5.TabIndex = 7;
            this.label5.Text = "Version:";
            // 
            // productLicenseVersionMaskedTextBox
            // 
            this.productLicenseVersionMaskedTextBox.Location = new System.Drawing.Point(160, 95);
            this.productLicenseVersionMaskedTextBox.Margin = new System.Windows.Forms.Padding(5);
            this.productLicenseVersionMaskedTextBox.Mask = "0.00";
            this.productLicenseVersionMaskedTextBox.Name = "productLicenseVersionMaskedTextBox";
            this.productLicenseVersionMaskedTextBox.Size = new System.Drawing.Size(52, 20);
            this.productLicenseVersionMaskedTextBox.TabIndex = 12;
            this.productLicenseVersionMaskedTextBox.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // ProductLicenseTypeComboBox
            // 
            this.ProductLicenseTypeComboBox.FormattingEnabled = true;
            this.ProductLicenseTypeComboBox.Location = new System.Drawing.Point(160, 65);
            this.ProductLicenseTypeComboBox.Margin = new System.Windows.Forms.Padding(5);
            this.ProductLicenseTypeComboBox.Name = "ProductLicenseTypeComboBox";
            this.ProductLicenseTypeComboBox.Size = new System.Drawing.Size(198, 21);
            this.ProductLicenseTypeComboBox.TabIndex = 1;
            this.ProductLicenseTypeComboBox.SelectedIndexChanged += new System.EventHandler(this.productLicenseTypeComboBox_SelectedIndexChanged);
            // 
            // ProductLicenseProductComboBox
            // 
            this.ProductLicenseProductComboBox.FormattingEnabled = true;
            this.ProductLicenseProductComboBox.Location = new System.Drawing.Point(160, 35);
            this.ProductLicenseProductComboBox.Margin = new System.Windows.Forms.Padding(5);
            this.ProductLicenseProductComboBox.Name = "ProductLicenseProductComboBox";
            this.ProductLicenseProductComboBox.Size = new System.Drawing.Size(198, 21);
            this.ProductLicenseProductComboBox.Sorted = true;
            this.ProductLicenseProductComboBox.TabIndex = 0;
            this.ProductLicenseProductComboBox.SelectedIndexChanged += new System.EventHandler(this.productLicenseProductComboBox_SelectedIndexChanged);
            // 
            // label1
            // 
            this.label1.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(3, 68);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(39, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Type:";
            // 
            // label2
            // 
            this.label2.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(3, 38);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(55, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Product:";
            // 
            // errorProvider1
            // 
            this.errorProvider1.ContainerControl = this;
            // 
            // browseCSVOpenFileDialog
            // 
            this.browseCSVOpenFileDialog.DefaultExt = "csv";
            this.browseCSVOpenFileDialog.Filter = "CSV Files|*.csv";
            // 
            // LicenseInfoForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.ClientSize = new System.Drawing.Size(475, 324);
            this.Controls.Add(this.topPanel);
            this.Name = "LicenseInfoForm";
            this.Text = "";
            this.FinishDialog += new Shared.VisualComponents.FinishDialogEventHandler(this.LicenseInfoForm_FinishDialog);
            this.InitDialog += new Shared.VisualComponents.InitDialogEventHandler(this.LicenseInfoForm_InitDialog);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.LicenseInfoForm_FormClosing);
            this.Controls.SetChildIndex(this.topPanel, 0);
            this.topPanel.ResumeLayout(false);
            this.licInfoTabControl.ResumeLayout(false);
            this.LicenseTabPage.ResumeLayout(false);
            this.licenseGroupBox.ResumeLayout(false);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.TokenTabPage.ResumeLayout(false);
            this.tokenGroupBox.ResumeLayout(false);
            this.tableLayoutPanel2.ResumeLayout(false);
            this.tableLayoutPanel2.PerformLayout();
            this.ModuleTabPage.ResumeLayout(false);
            this.moduleGroupBox.ResumeLayout(false);
            this.tableLayoutPanel3.ResumeLayout(false);
            this.tableLayoutPanel3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.modValueNumericUpDown)).EndInit();
            this.PacketTabPage.ResumeLayout(false);
            this.packetGroupBox.ResumeLayout(false);
            this.tableLayoutPanel4.ResumeLayout(false);
            this.tableLayoutPanel4.PerformLayout();
            this.orderTabPage.ResumeLayout(false);
            this.orderGroupBox.ResumeLayout(false);
            this.tableLayoutPanel5.ResumeLayout(false);
            this.tableLayoutPanel5.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel topPanel;
        private System.Windows.Forms.TabControl licInfoTabControl;
        private System.Windows.Forms.TabPage LicenseTabPage;
        private System.Windows.Forms.TabPage TokenTabPage;
        private System.Windows.Forms.TabPage ModuleTabPage;
        private System.Windows.Forms.TabPage PacketTabPage;
        private System.Windows.Forms.ComboBox tokenTypeComboBox;
        private System.Windows.Forms.TextBox tokenValueTextBox;
        private System.Windows.Forms.Label tokenValueLabel;
        private System.Windows.Forms.Label tokenTypeLabel;
        private System.Windows.Forms.NumericUpDown modValueNumericUpDown;
        private System.Windows.Forms.ComboBox modNameComboBox;
        private System.Windows.Forms.Label modValueLabel;
        private System.Windows.Forms.Label modNameLabel;
        private System.Windows.Forms.Label packetNameLabel;
        private System.Windows.Forms.TextBox packetDescriptTextBox;
        private System.Windows.Forms.TextBox packetOutputPathTextBox;
        private System.Windows.Forms.TextBox packetNameTextBox;
        private System.Windows.Forms.Label descriptionLabel;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label packetDateLabel;
        private System.Windows.Forms.FolderBrowserDialog folderBrowserDialog;
        private System.Windows.Forms.Button folderBrowseButton;
        private System.Windows.Forms.GroupBox tokenGroupBox;
        private System.Windows.Forms.GroupBox moduleGroupBox;
        private System.Windows.Forms.TabPage orderTabPage;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox ProductLicenseTypeComboBox;
        private System.Windows.Forms.ComboBox ProductLicenseProductComboBox;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox productLicenseNumberTextBox;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox productLicenseDescriptionTextBox;
        private System.Windows.Forms.MaskedTextBox productLicenseVersionMaskedTextBox;
        private System.Windows.Forms.ErrorProvider errorProvider1;
        private System.Windows.Forms.TextBox expDateTextBox;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Label licDescriptLabel;
        private System.Windows.Forms.Label licTypeLabel;
        private System.Windows.Forms.Label licDestIDLabel;
        private System.Windows.Forms.TextBox licDescriptTextBox;
        private System.Windows.Forms.ComboBox licTypeComboBox;
        private System.Windows.Forms.ComboBox destNameComboBox;
        private System.Windows.Forms.TextBox licNameTextBox;
        private System.Windows.Forms.Label licNameLabel;
        private System.Windows.Forms.GroupBox licenseGroupBox;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.Label availableHWKeyLabel;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel3;
        private System.Windows.Forms.GroupBox packetGroupBox;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel4;
        private System.Windows.Forms.GroupBox orderGroupBox;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel5;
        private System.Windows.Forms.TextBox packetExpDateTextBox;
        private System.Windows.Forms.Button browseTokenFileButton;
        private System.Windows.Forms.OpenFileDialog browseCSVOpenFileDialog;
        private System.Windows.Forms.ListView keyNameListView;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;


    }
}
