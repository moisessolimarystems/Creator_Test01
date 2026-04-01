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
            this.LicenseInfoGroupBox = new System.Windows.Forms.GroupBox();
            this.destNameComboBox = new System.Windows.Forms.ComboBox();
            this.licDescriptLabel = new System.Windows.Forms.Label();
            this.licDescriptTextBox = new System.Windows.Forms.TextBox();
            this.licTypeLabel = new System.Windows.Forms.Label();
            this.licTypeComboBox = new System.Windows.Forms.ComboBox();
            this.licDestIDLabel = new System.Windows.Forms.Label();
            this.licNameLabel = new System.Windows.Forms.Label();
            this.licNameTextBox = new System.Windows.Forms.TextBox();
            this.TokenTabPage = new System.Windows.Forms.TabPage();
            this.keyNameGroupBox = new System.Windows.Forms.GroupBox();
            this.keyNameListBox = new System.Windows.Forms.ListBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.tokenTypeComboBox = new System.Windows.Forms.ComboBox();
            this.tokenValueTextBox = new System.Windows.Forms.TextBox();
            this.tokenValueLabel = new System.Windows.Forms.Label();
            this.tokenTypeLabel = new System.Windows.Forms.Label();
            this.ModuleTabPage = new System.Windows.Forms.TabPage();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.modNameLabel = new System.Windows.Forms.Label();
            this.modValueLabel = new System.Windows.Forms.Label();
            this.modValueNumericUpDown = new System.Windows.Forms.NumericUpDown();
            this.modNameComboBox = new System.Windows.Forms.ComboBox();
            this.PacketTabPage = new System.Windows.Forms.TabPage();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.packetInfoLabel = new System.Windows.Forms.Label();
            this.folderBrowseButton = new System.Windows.Forms.Button();
            this.packetDescriptTextBox = new System.Windows.Forms.TextBox();
            this.packetOutputPathTextBox = new System.Windows.Forms.TextBox();
            this.packetDateTextBox = new System.Windows.Forms.TextBox();
            this.packetNameTextBox = new System.Windows.Forms.TextBox();
            this.descriptionLabel = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.packetDateLabel = new System.Windows.Forms.Label();
            this.packetNameLabel = new System.Windows.Forms.Label();
            this.orderTabPage = new System.Windows.Forms.TabPage();
            this.expDateTextBox = new System.Windows.Forms.TextBox();
            this.orderVersionMaskedTextBox = new System.Windows.Forms.MaskedTextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.orderDescriptionTextBox = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.orderNumberTextBox = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.orderTypeComboBox = new System.Windows.Forms.ComboBox();
            this.orderProductComboBox = new System.Windows.Forms.ComboBox();
            this.HardwareKeyTabPage = new System.Windows.Forms.TabPage();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.loadingCircle1 = new Shared.VisualComponents.LoadingCircle();
            this.label10 = new System.Windows.Forms.Label();
            this.AvailableKeysListBox = new System.Windows.Forms.ListBox();
            this.label9 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.KeyValueTextBox = new System.Windows.Forms.TextBox();
            this.CustomerComboBox = new System.Windows.Forms.ComboBox();
            this.folderBrowserDialog = new System.Windows.Forms.FolderBrowserDialog();
            this.errorProvider1 = new System.Windows.Forms.ErrorProvider(this.components);
            this.topPanel.SuspendLayout();
            this.licInfoTabControl.SuspendLayout();
            this.LicenseTabPage.SuspendLayout();
            this.LicenseInfoGroupBox.SuspendLayout();
            this.TokenTabPage.SuspendLayout();
            this.keyNameGroupBox.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.ModuleTabPage.SuspendLayout();
            this.groupBox4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.modValueNumericUpDown)).BeginInit();
            this.PacketTabPage.SuspendLayout();
            this.orderTabPage.SuspendLayout();
            this.HardwareKeyTabPage.SuspendLayout();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).BeginInit();
            this.SuspendLayout();
            // 
            // topPanel
            // 
            this.topPanel.Controls.Add(this.licInfoTabControl);
            this.topPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.topPanel.Location = new System.Drawing.Point(0, 0);
            this.topPanel.Name = "topPanel";
            this.topPanel.Size = new System.Drawing.Size(471, 360);
            this.topPanel.TabIndex = 3;
            // 
            // licInfoTabControl
            // 
            this.licInfoTabControl.Controls.Add(this.LicenseTabPage);
            this.licInfoTabControl.Controls.Add(this.TokenTabPage);
            this.licInfoTabControl.Controls.Add(this.ModuleTabPage);
            this.licInfoTabControl.Controls.Add(this.PacketTabPage);
            this.licInfoTabControl.Controls.Add(this.orderTabPage);
            this.licInfoTabControl.Controls.Add(this.HardwareKeyTabPage);
            this.licInfoTabControl.Location = new System.Drawing.Point(5, 10);
            this.licInfoTabControl.Margin = new System.Windows.Forms.Padding(1);
            this.licInfoTabControl.Name = "licInfoTabControl";
            this.licInfoTabControl.SelectedIndex = 0;
            this.licInfoTabControl.Size = new System.Drawing.Size(462, 347);
            this.licInfoTabControl.TabIndex = 9;
            // 
            // LicenseTabPage
            // 
            this.LicenseTabPage.Controls.Add(this.LicenseInfoGroupBox);
            this.LicenseTabPage.Location = new System.Drawing.Point(4, 22);
            this.LicenseTabPage.Name = "LicenseTabPage";
            this.LicenseTabPage.Padding = new System.Windows.Forms.Padding(3);
            this.LicenseTabPage.Size = new System.Drawing.Size(454, 321);
            this.LicenseTabPage.TabIndex = 0;
            this.LicenseTabPage.Text = "License";
            this.LicenseTabPage.UseVisualStyleBackColor = true;
            // 
            // LicenseInfoGroupBox
            // 
            this.LicenseInfoGroupBox.Controls.Add(this.destNameComboBox);
            this.LicenseInfoGroupBox.Controls.Add(this.licDescriptLabel);
            this.LicenseInfoGroupBox.Controls.Add(this.licDescriptTextBox);
            this.LicenseInfoGroupBox.Controls.Add(this.licTypeLabel);
            this.LicenseInfoGroupBox.Controls.Add(this.licTypeComboBox);
            this.LicenseInfoGroupBox.Controls.Add(this.licDestIDLabel);
            this.LicenseInfoGroupBox.Controls.Add(this.licNameLabel);
            this.LicenseInfoGroupBox.Controls.Add(this.licNameTextBox);
            this.LicenseInfoGroupBox.Location = new System.Drawing.Point(6, 6);
            this.LicenseInfoGroupBox.Name = "LicenseInfoGroupBox";
            this.LicenseInfoGroupBox.Size = new System.Drawing.Size(442, 319);
            this.LicenseInfoGroupBox.TabIndex = 10;
            this.LicenseInfoGroupBox.TabStop = false;
            this.LicenseInfoGroupBox.Text = "License Information";
            // 
            // destNameComboBox
            // 
            this.destNameComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.destNameComboBox.FormattingEnabled = true;
            this.destNameComboBox.Items.AddRange(new object[] {
            "Add Destination Name..."});
            this.destNameComboBox.Location = new System.Drawing.Point(43, 100);
            this.destNameComboBox.Name = "destNameComboBox";
            this.destNameComboBox.Size = new System.Drawing.Size(188, 21);
            this.destNameComboBox.TabIndex = 12;
            this.destNameComboBox.SelectionChangeCommitted += new System.EventHandler(this.destNameComboBox_SelectionChangeCommitted);
            // 
            // licDescriptLabel
            // 
            this.licDescriptLabel.AutoSize = true;
            this.licDescriptLabel.Location = new System.Drawing.Point(43, 189);
            this.licDescriptLabel.Name = "licDescriptLabel";
            this.licDescriptLabel.Size = new System.Drawing.Size(60, 13);
            this.licDescriptLabel.TabIndex = 8;
            this.licDescriptLabel.Text = "Description";
            // 
            // licDescriptTextBox
            // 
            this.licDescriptTextBox.Location = new System.Drawing.Point(43, 205);
            this.licDescriptTextBox.Multiline = true;
            this.licDescriptTextBox.Name = "licDescriptTextBox";
            this.licDescriptTextBox.Size = new System.Drawing.Size(329, 73);
            this.licDescriptTextBox.TabIndex = 9;
            // 
            // licTypeLabel
            // 
            this.licTypeLabel.AutoSize = true;
            this.licTypeLabel.Location = new System.Drawing.Point(43, 137);
            this.licTypeLabel.Name = "licTypeLabel";
            this.licTypeLabel.Size = new System.Drawing.Size(71, 13);
            this.licTypeLabel.TabIndex = 7;
            this.licTypeLabel.Text = "License Type";
            // 
            // licTypeComboBox
            // 
            this.licTypeComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.licTypeComboBox.FormattingEnabled = true;
            this.licTypeComboBox.Location = new System.Drawing.Point(43, 153);
            this.licTypeComboBox.Name = "licTypeComboBox";
            this.licTypeComboBox.Size = new System.Drawing.Size(188, 21);
            this.licTypeComboBox.TabIndex = 6;
            this.licTypeComboBox.SelectedIndexChanged += new System.EventHandler(this.licTypeComboBox_SelectedIndexChanged);
            // 
            // licDestIDLabel
            // 
            this.licDestIDLabel.AutoSize = true;
            this.licDestIDLabel.Location = new System.Drawing.Point(43, 85);
            this.licDestIDLabel.Name = "licDestIDLabel";
            this.licDestIDLabel.Size = new System.Drawing.Size(91, 13);
            this.licDestIDLabel.TabIndex = 3;
            this.licDestIDLabel.Text = "Destination Name";
            // 
            // licNameLabel
            // 
            this.licNameLabel.AutoSize = true;
            this.licNameLabel.Location = new System.Drawing.Point(43, 34);
            this.licNameLabel.Name = "licNameLabel";
            this.licNameLabel.Size = new System.Drawing.Size(35, 13);
            this.licNameLabel.TabIndex = 0;
            this.licNameLabel.Text = "Name";
            // 
            // licNameTextBox
            // 
            this.licNameTextBox.Location = new System.Drawing.Point(43, 50);
            this.licNameTextBox.Name = "licNameTextBox";
            this.licNameTextBox.ReadOnly = true;
            this.licNameTextBox.Size = new System.Drawing.Size(329, 20);
            this.licNameTextBox.TabIndex = 1;
            // 
            // TokenTabPage
            // 
            this.TokenTabPage.Controls.Add(this.keyNameGroupBox);
            this.TokenTabPage.Controls.Add(this.groupBox3);
            this.TokenTabPage.Location = new System.Drawing.Point(4, 22);
            this.TokenTabPage.Name = "TokenTabPage";
            this.TokenTabPage.Padding = new System.Windows.Forms.Padding(3);
            this.TokenTabPage.Size = new System.Drawing.Size(454, 321);
            this.TokenTabPage.TabIndex = 1;
            this.TokenTabPage.Text = "Token";
            this.TokenTabPage.UseVisualStyleBackColor = true;
            // 
            // keyNameGroupBox
            // 
            this.keyNameGroupBox.Controls.Add(this.keyNameListBox);
            this.keyNameGroupBox.Location = new System.Drawing.Point(22, 133);
            this.keyNameGroupBox.Name = "keyNameGroupBox";
            this.keyNameGroupBox.Size = new System.Drawing.Size(411, 154);
            this.keyNameGroupBox.TabIndex = 8;
            this.keyNameGroupBox.TabStop = false;
            this.keyNameGroupBox.Text = "Available Hardware Keys";
            // 
            // keyNameListBox
            // 
            this.keyNameListBox.FormattingEnabled = true;
            this.keyNameListBox.Location = new System.Drawing.Point(23, 32);
            this.keyNameListBox.Name = "keyNameListBox";
            this.keyNameListBox.Size = new System.Drawing.Size(273, 95);
            this.keyNameListBox.TabIndex = 7;
            this.keyNameListBox.SelectedIndexChanged += new System.EventHandler(this.keyNameListBox_SelectedIndexChanged);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.tokenTypeComboBox);
            this.groupBox3.Controls.Add(this.tokenValueTextBox);
            this.groupBox3.Controls.Add(this.tokenValueLabel);
            this.groupBox3.Controls.Add(this.tokenTypeLabel);
            this.groupBox3.Location = new System.Drawing.Point(22, 16);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(411, 111);
            this.groupBox3.TabIndex = 4;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Validation Token Information";
            // 
            // tokenTypeComboBox
            // 
            this.tokenTypeComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.tokenTypeComboBox.FormattingEnabled = true;
            this.tokenTypeComboBox.Location = new System.Drawing.Point(126, 28);
            this.tokenTypeComboBox.Name = "tokenTypeComboBox";
            this.tokenTypeComboBox.Size = new System.Drawing.Size(170, 21);
            this.tokenTypeComboBox.TabIndex = 3;
            this.tokenTypeComboBox.SelectedIndexChanged += new System.EventHandler(this.tokenTypeComboBox_SelectedIndexChanged);
            // 
            // tokenValueTextBox
            // 
            this.tokenValueTextBox.Location = new System.Drawing.Point(126, 65);
            this.tokenValueTextBox.Name = "tokenValueTextBox";
            this.tokenValueTextBox.Size = new System.Drawing.Size(170, 20);
            this.tokenValueTextBox.TabIndex = 2;
            this.tokenValueTextBox.TextChanged += new System.EventHandler(this.tokenValueTextBox_TextChanged);
            this.tokenValueTextBox.Validated += new System.EventHandler(this.tokenValueTextBox_Validated);
            this.tokenValueTextBox.Validating += new System.ComponentModel.CancelEventHandler(this.tokenValueTextBox_Validating);
            // 
            // tokenValueLabel
            // 
            this.tokenValueLabel.AutoSize = true;
            this.tokenValueLabel.Location = new System.Drawing.Point(20, 68);
            this.tokenValueLabel.Name = "tokenValueLabel";
            this.tokenValueLabel.Size = new System.Drawing.Size(68, 13);
            this.tokenValueLabel.TabIndex = 1;
            this.tokenValueLabel.Text = "Token Value";
            // 
            // tokenTypeLabel
            // 
            this.tokenTypeLabel.AutoSize = true;
            this.tokenTypeLabel.Location = new System.Drawing.Point(20, 31);
            this.tokenTypeLabel.Name = "tokenTypeLabel";
            this.tokenTypeLabel.Size = new System.Drawing.Size(65, 13);
            this.tokenTypeLabel.TabIndex = 0;
            this.tokenTypeLabel.Text = "Token Type";
            // 
            // ModuleTabPage
            // 
            this.ModuleTabPage.Controls.Add(this.groupBox4);
            this.ModuleTabPage.Location = new System.Drawing.Point(4, 22);
            this.ModuleTabPage.Name = "ModuleTabPage";
            this.ModuleTabPage.Size = new System.Drawing.Size(454, 321);
            this.ModuleTabPage.TabIndex = 2;
            this.ModuleTabPage.Text = "Module";
            this.ModuleTabPage.UseVisualStyleBackColor = true;
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.modNameLabel);
            this.groupBox4.Controls.Add(this.modValueLabel);
            this.groupBox4.Controls.Add(this.modValueNumericUpDown);
            this.groupBox4.Controls.Add(this.modNameComboBox);
            this.groupBox4.Location = new System.Drawing.Point(17, 16);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(422, 155);
            this.groupBox4.TabIndex = 8;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Module Information";
            // 
            // modNameLabel
            // 
            this.modNameLabel.AutoSize = true;
            this.modNameLabel.Location = new System.Drawing.Point(19, 29);
            this.modNameLabel.Name = "modNameLabel";
            this.modNameLabel.Size = new System.Drawing.Size(35, 13);
            this.modNameLabel.TabIndex = 0;
            this.modNameLabel.Text = "Name";
            // 
            // modValueLabel
            // 
            this.modValueLabel.AutoSize = true;
            this.modValueLabel.Location = new System.Drawing.Point(20, 80);
            this.modValueLabel.Name = "modValueLabel";
            this.modValueLabel.Size = new System.Drawing.Size(34, 13);
            this.modValueLabel.TabIndex = 1;
            this.modValueLabel.Text = "Value";
            // 
            // modValueNumericUpDown
            // 
            this.modValueNumericUpDown.Location = new System.Drawing.Point(23, 96);
            this.modValueNumericUpDown.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.modValueNumericUpDown.Name = "modValueNumericUpDown";
            this.modValueNumericUpDown.Size = new System.Drawing.Size(95, 20);
            this.modValueNumericUpDown.TabIndex = 6;
            // 
            // modNameComboBox
            // 
            this.modNameComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.modNameComboBox.FormattingEnabled = true;
            this.modNameComboBox.Location = new System.Drawing.Point(22, 45);
            this.modNameComboBox.Name = "modNameComboBox";
            this.modNameComboBox.Size = new System.Drawing.Size(200, 21);
            this.modNameComboBox.TabIndex = 5;
            // 
            // PacketTabPage
            // 
            this.PacketTabPage.Controls.Add(this.groupBox1);
            this.PacketTabPage.Controls.Add(this.packetInfoLabel);
            this.PacketTabPage.Controls.Add(this.folderBrowseButton);
            this.PacketTabPage.Controls.Add(this.packetDescriptTextBox);
            this.PacketTabPage.Controls.Add(this.packetOutputPathTextBox);
            this.PacketTabPage.Controls.Add(this.packetDateTextBox);
            this.PacketTabPage.Controls.Add(this.packetNameTextBox);
            this.PacketTabPage.Controls.Add(this.descriptionLabel);
            this.PacketTabPage.Controls.Add(this.label3);
            this.PacketTabPage.Controls.Add(this.packetDateLabel);
            this.PacketTabPage.Controls.Add(this.packetNameLabel);
            this.PacketTabPage.Location = new System.Drawing.Point(4, 22);
            this.PacketTabPage.Name = "PacketTabPage";
            this.PacketTabPage.Size = new System.Drawing.Size(454, 321);
            this.PacketTabPage.TabIndex = 3;
            this.PacketTabPage.Text = "Packet";
            this.PacketTabPage.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Location = new System.Drawing.Point(127, 24);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(283, 5);
            this.groupBox1.TabIndex = 10;
            this.groupBox1.TabStop = false;
            // 
            // packetInfoLabel
            // 
            this.packetInfoLabel.AutoSize = true;
            this.packetInfoLabel.Location = new System.Drawing.Point(25, 20);
            this.packetInfoLabel.Name = "packetInfoLabel";
            this.packetInfoLabel.Size = new System.Drawing.Size(96, 13);
            this.packetInfoLabel.TabIndex = 9;
            this.packetInfoLabel.Text = "Packet Information";
            // 
            // folderBrowseButton
            // 
            this.folderBrowseButton.Image = ((System.Drawing.Image)(resources.GetObject("folderBrowseButton.Image")));
            this.folderBrowseButton.Location = new System.Drawing.Point(385, 158);
            this.folderBrowseButton.Margin = new System.Windows.Forms.Padding(0);
            this.folderBrowseButton.Name = "folderBrowseButton";
            this.folderBrowseButton.Size = new System.Drawing.Size(25, 25);
            this.folderBrowseButton.TabIndex = 8;
            this.folderBrowseButton.UseVisualStyleBackColor = true;
            this.folderBrowseButton.Click += new System.EventHandler(this.folderBrowseButton_Click);
            // 
            // packetDescriptTextBox
            // 
            this.packetDescriptTextBox.Location = new System.Drawing.Point(74, 229);
            this.packetDescriptTextBox.Multiline = true;
            this.packetDescriptTextBox.Name = "packetDescriptTextBox";
            this.packetDescriptTextBox.Size = new System.Drawing.Size(336, 63);
            this.packetDescriptTextBox.TabIndex = 7;
            // 
            // packetOutputPathTextBox
            // 
            this.packetOutputPathTextBox.Location = new System.Drawing.Point(74, 158);
            this.packetOutputPathTextBox.Multiline = true;
            this.packetOutputPathTextBox.Name = "packetOutputPathTextBox";
            this.packetOutputPathTextBox.Size = new System.Drawing.Size(295, 42);
            this.packetOutputPathTextBox.TabIndex = 6;
            // 
            // packetDateTextBox
            // 
            this.packetDateTextBox.Location = new System.Drawing.Point(74, 110);
            this.packetDateTextBox.Name = "packetDateTextBox";
            this.packetDateTextBox.ReadOnly = true;
            this.packetDateTextBox.Size = new System.Drawing.Size(123, 20);
            this.packetDateTextBox.TabIndex = 5;
            // 
            // packetNameTextBox
            // 
            this.packetNameTextBox.Location = new System.Drawing.Point(74, 64);
            this.packetNameTextBox.Name = "packetNameTextBox";
            this.packetNameTextBox.ReadOnly = true;
            this.packetNameTextBox.Size = new System.Drawing.Size(336, 20);
            this.packetNameTextBox.TabIndex = 4;
            // 
            // descriptionLabel
            // 
            this.descriptionLabel.AutoSize = true;
            this.descriptionLabel.Location = new System.Drawing.Point(71, 213);
            this.descriptionLabel.Name = "descriptionLabel";
            this.descriptionLabel.Size = new System.Drawing.Size(60, 13);
            this.descriptionLabel.TabIndex = 3;
            this.descriptionLabel.Text = "Description";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(71, 142);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(85, 13);
            this.label3.TabIndex = 2;
            this.label3.Text = "Packet Location";
            // 
            // packetDateLabel
            // 
            this.packetDateLabel.AutoSize = true;
            this.packetDateLabel.Location = new System.Drawing.Point(71, 94);
            this.packetDateLabel.Name = "packetDateLabel";
            this.packetDateLabel.Size = new System.Drawing.Size(30, 13);
            this.packetDateLabel.TabIndex = 1;
            this.packetDateLabel.Text = "Date";
            // 
            // packetNameLabel
            // 
            this.packetNameLabel.AutoSize = true;
            this.packetNameLabel.Location = new System.Drawing.Point(71, 48);
            this.packetNameLabel.Name = "packetNameLabel";
            this.packetNameLabel.Size = new System.Drawing.Size(35, 13);
            this.packetNameLabel.TabIndex = 0;
            this.packetNameLabel.Text = "Name";
            // 
            // orderTabPage
            // 
            this.orderTabPage.Controls.Add(this.expDateTextBox);
            this.orderTabPage.Controls.Add(this.orderVersionMaskedTextBox);
            this.orderTabPage.Controls.Add(this.label7);
            this.orderTabPage.Controls.Add(this.orderDescriptionTextBox);
            this.orderTabPage.Controls.Add(this.label6);
            this.orderTabPage.Controls.Add(this.label5);
            this.orderTabPage.Controls.Add(this.label4);
            this.orderTabPage.Controls.Add(this.orderNumberTextBox);
            this.orderTabPage.Controls.Add(this.label2);
            this.orderTabPage.Controls.Add(this.label1);
            this.orderTabPage.Controls.Add(this.orderTypeComboBox);
            this.orderTabPage.Controls.Add(this.orderProductComboBox);
            this.orderTabPage.Location = new System.Drawing.Point(4, 22);
            this.orderTabPage.Name = "orderTabPage";
            this.orderTabPage.Padding = new System.Windows.Forms.Padding(3);
            this.orderTabPage.Size = new System.Drawing.Size(454, 321);
            this.orderTabPage.TabIndex = 4;
            this.orderTabPage.Text = "Order";
            this.orderTabPage.UseVisualStyleBackColor = true;
            // 
            // expDateTextBox
            // 
            this.expDateTextBox.Location = new System.Drawing.Point(47, 189);
            this.expDateTextBox.Name = "expDateTextBox";
            this.expDateTextBox.Size = new System.Drawing.Size(74, 20);
            this.expDateTextBox.TabIndex = 13;
            this.expDateTextBox.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.expDateTextBox.TextChanged += new System.EventHandler(this.expDateTextBox_TextChanged);
            // 
            // orderVersionMaskedTextBox
            // 
            this.orderVersionMaskedTextBox.Location = new System.Drawing.Point(301, 90);
            this.orderVersionMaskedTextBox.Mask = "0.00";
            this.orderVersionMaskedTextBox.Name = "orderVersionMaskedTextBox";
            this.orderVersionMaskedTextBox.Size = new System.Drawing.Size(53, 20);
            this.orderVersionMaskedTextBox.TabIndex = 12;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(44, 222);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(60, 13);
            this.label7.TabIndex = 11;
            this.label7.Text = "Description";
            // 
            // orderDescriptionTextBox
            // 
            this.orderDescriptionTextBox.Location = new System.Drawing.Point(47, 238);
            this.orderDescriptionTextBox.Multiline = true;
            this.orderDescriptionTextBox.Name = "orderDescriptionTextBox";
            this.orderDescriptionTextBox.Size = new System.Drawing.Size(354, 65);
            this.orderDescriptionTextBox.TabIndex = 10;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(44, 173);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(77, 13);
            this.label6.TabIndex = 9;
            this.label6.Text = "Days Licensed";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(298, 73);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(42, 13);
            this.label5.TabIndex = 7;
            this.label5.Text = "Version";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(44, 21);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(73, 13);
            this.label4.TabIndex = 5;
            this.label4.Text = "Order Number";
            // 
            // orderNumberTextBox
            // 
            this.orderNumberTextBox.Location = new System.Drawing.Point(47, 37);
            this.orderNumberTextBox.Name = "orderNumberTextBox";
            this.orderNumberTextBox.ReadOnly = true;
            this.orderNumberTextBox.Size = new System.Drawing.Size(215, 20);
            this.orderNumberTextBox.TabIndex = 4;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(44, 73);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(44, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Product";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(44, 122);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(31, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Type";
            // 
            // orderTypeComboBox
            // 
            this.orderTypeComboBox.FormattingEnabled = true;
            this.orderTypeComboBox.Location = new System.Drawing.Point(47, 138);
            this.orderTypeComboBox.Name = "orderTypeComboBox";
            this.orderTypeComboBox.Size = new System.Drawing.Size(215, 21);
            this.orderTypeComboBox.TabIndex = 1;
            this.orderTypeComboBox.SelectedIndexChanged += new System.EventHandler(this.orderTypeComboBox_SelectedIndexChanged);
            // 
            // orderProductComboBox
            // 
            this.orderProductComboBox.FormattingEnabled = true;
            this.orderProductComboBox.Location = new System.Drawing.Point(47, 89);
            this.orderProductComboBox.Name = "orderProductComboBox";
            this.orderProductComboBox.Size = new System.Drawing.Size(215, 21);
            this.orderProductComboBox.TabIndex = 0;
            this.orderProductComboBox.SelectedIndexChanged += new System.EventHandler(this.orderProductComboBox_SelectedIndexChanged);
            // 
            // HardwareKeyTabPage
            // 
            this.HardwareKeyTabPage.Controls.Add(this.groupBox2);
            this.HardwareKeyTabPage.Location = new System.Drawing.Point(4, 22);
            this.HardwareKeyTabPage.Name = "HardwareKeyTabPage";
            this.HardwareKeyTabPage.Size = new System.Drawing.Size(454, 321);
            this.HardwareKeyTabPage.TabIndex = 5;
            this.HardwareKeyTabPage.Text = "Hardware Key";
            this.HardwareKeyTabPage.UseVisualStyleBackColor = true;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.loadingCircle1);
            this.groupBox2.Controls.Add(this.label10);
            this.groupBox2.Controls.Add(this.AvailableKeysListBox);
            this.groupBox2.Controls.Add(this.label9);
            this.groupBox2.Controls.Add(this.label8);
            this.groupBox2.Controls.Add(this.KeyValueTextBox);
            this.groupBox2.Controls.Add(this.CustomerComboBox);
            this.groupBox2.Location = new System.Drawing.Point(29, 14);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(395, 277);
            this.groupBox2.TabIndex = 0;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Program Hardware Key";
            // 
            // loadingCircle1
            // 
            this.loadingCircle1.Active = false;
            this.loadingCircle1.Color = System.Drawing.Color.DarkGray;
            this.loadingCircle1.InnerCircleRadius = 5;
            this.loadingCircle1.Location = new System.Drawing.Point(120, 141);
            this.loadingCircle1.Name = "loadingCircle1";
            this.loadingCircle1.NumberSpoke = 12;
            this.loadingCircle1.OuterCircleRadius = 11;
            this.loadingCircle1.RotationSpeed = 100;
            this.loadingCircle1.Size = new System.Drawing.Size(207, 108);
            this.loadingCircle1.SpokeThickness = 2;
            this.loadingCircle1.StylePreset = Shared.VisualComponents.LoadingCircle.StylePresets.MacOSX;
            this.loadingCircle1.TabIndex = 5;
            this.loadingCircle1.Text = "loadingCircle1";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(24, 141);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(76, 13);
            this.label10.TabIndex = 4;
            this.label10.Text = "Available Keys";
            // 
            // AvailableKeysListBox
            // 
            this.AvailableKeysListBox.FormattingEnabled = true;
            this.AvailableKeysListBox.Location = new System.Drawing.Point(120, 141);
            this.AvailableKeysListBox.Name = "AvailableKeysListBox";
            this.AvailableKeysListBox.Size = new System.Drawing.Size(207, 108);
            this.AvailableKeysListBox.TabIndex = 1;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(24, 90);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(55, 13);
            this.label9.TabIndex = 3;
            this.label9.Text = "Key Value";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(24, 41);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(51, 13);
            this.label8.TabIndex = 2;
            this.label8.Text = "Customer";
            // 
            // KeyValueTextBox
            // 
            this.KeyValueTextBox.Location = new System.Drawing.Point(120, 87);
            this.KeyValueTextBox.Name = "KeyValueTextBox";
            this.KeyValueTextBox.ReadOnly = true;
            this.KeyValueTextBox.Size = new System.Drawing.Size(207, 20);
            this.KeyValueTextBox.TabIndex = 1;
            // 
            // CustomerComboBox
            // 
            this.CustomerComboBox.FormattingEnabled = true;
            this.CustomerComboBox.Location = new System.Drawing.Point(120, 38);
            this.CustomerComboBox.Name = "CustomerComboBox";
            this.CustomerComboBox.Size = new System.Drawing.Size(207, 21);
            this.CustomerComboBox.TabIndex = 0;
            this.CustomerComboBox.SelectedIndexChanged += new System.EventHandler(this.CustomerComboBox_SelectedIndexChanged);
            // 
            // errorProvider1
            // 
            this.errorProvider1.ContainerControl = this;
            // 
            // LicenseInfoForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.ClientSize = new System.Drawing.Size(471, 391);
            this.Controls.Add(this.topPanel);
            this.Name = "LicenseInfoForm";
            this.Text = "";
            this.FinishDialog += new Shared.VisualComponents.FinishDialogEventHandler(this.LicenseInfoForm_FinishDialog);
            this.InitDialog += new Shared.VisualComponents.InitDialogEventHandler(this.LicenseInfoForm_InitDialog);
            this.Controls.SetChildIndex(this.topPanel, 0);
            this.topPanel.ResumeLayout(false);
            this.licInfoTabControl.ResumeLayout(false);
            this.LicenseTabPage.ResumeLayout(false);
            this.LicenseInfoGroupBox.ResumeLayout(false);
            this.LicenseInfoGroupBox.PerformLayout();
            this.TokenTabPage.ResumeLayout(false);
            this.keyNameGroupBox.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.ModuleTabPage.ResumeLayout(false);
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.modValueNumericUpDown)).EndInit();
            this.PacketTabPage.ResumeLayout(false);
            this.PacketTabPage.PerformLayout();
            this.orderTabPage.ResumeLayout(false);
            this.orderTabPage.PerformLayout();
            this.HardwareKeyTabPage.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
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
        private System.Windows.Forms.TextBox licNameTextBox;
        private System.Windows.Forms.Label licNameLabel;
        private System.Windows.Forms.TextBox licDescriptTextBox;
        private System.Windows.Forms.Label licDescriptLabel;
        private System.Windows.Forms.Label licTypeLabel;
        private System.Windows.Forms.ComboBox licTypeComboBox;
        private System.Windows.Forms.Label licDestIDLabel;
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
        private System.Windows.Forms.TextBox packetDateTextBox;
        private System.Windows.Forms.TextBox packetNameTextBox;
        private System.Windows.Forms.Label descriptionLabel;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label packetDateLabel;
        private System.Windows.Forms.FolderBrowserDialog folderBrowserDialog;
        private System.Windows.Forms.Button folderBrowseButton;
        private System.Windows.Forms.Label packetInfoLabel;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.GroupBox LicenseInfoGroupBox;
        private System.Windows.Forms.ListBox keyNameListBox;
        private System.Windows.Forms.GroupBox keyNameGroupBox;
        private System.Windows.Forms.TabPage orderTabPage;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox orderTypeComboBox;
        private System.Windows.Forms.ComboBox orderProductComboBox;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox orderNumberTextBox;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox orderDescriptionTextBox;
        private System.Windows.Forms.MaskedTextBox orderVersionMaskedTextBox;
        private System.Windows.Forms.ErrorProvider errorProvider1;
        private System.Windows.Forms.TabPage HardwareKeyTabPage;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.TextBox KeyValueTextBox;
        private System.Windows.Forms.ComboBox CustomerComboBox;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.ListBox AvailableKeysListBox;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.ComboBox destNameComboBox;
        private System.Windows.Forms.TextBox expDateTextBox;
        private Shared.VisualComponents.LoadingCircle loadingCircle1;


    }
}
