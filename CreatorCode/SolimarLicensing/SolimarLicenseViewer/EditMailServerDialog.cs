using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Solimar.Licensing.Attribs;
namespace SolimarLicenseViewer
{
    public partial class EditMailServerDialog : Shared.VisualComponents.DialogBaseForm
    {
        public EditMailServerDialog()
        {
            InitializeComponent();
            InitializeInternal();
        }
        private void InitializeInternal()
        {
            // CR.18713 - Add SSL & TSL
            this.encryptionComboBox.Items.Clear();
            foreach (var element in Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_AlertMailServerAttribs.AttribsMemberEnum_TEncryptionType.GetAliases())
                this.encryptionComboBox.Items.Add(element);
            this.encryptionComboBox.SelectedIndex = 0;
        }

        private const string TITLE = "Edit Mail Server";
        private const string READONLY = " (Read Only)";
        private Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_AlertMailServerAttribs mailServerAttribs = null;
        private bool bReadOnly = false;
        public void SetData(Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_AlertMailServerAttribs _mailServerAttribs, bool _bReadOnly)
        {
            this.mailServerAttribs = (Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_AlertMailServerAttribs)_mailServerAttribs.Clone();
            this.bReadOnly = _bReadOnly;

            this.Text = string.Format("{0}{1}",
                TITLE,
                this.bReadOnly ? READONLY : ""
                );

            if (this.mailServerAttribs != null)
            {
                this.mailServerTBox.Text = this.mailServerAttribs.mailServerName.TVal;
                this.portTBox.Text = this.mailServerAttribs.portNumber.TVal.ToString();
                this.emailTBox.Text = this.mailServerAttribs.fromEmail.TVal;
                this.displayNameTBox.Text = this.mailServerAttribs.fromDisplayName.TVal;

                // CR.18713 - Add SSL & TSL
                this.encryptionComboBox.SelectedIndex = Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_AlertMailServerAttribs.AttribsMemberEnum_TEncryptionType.GetIndexFromEnum(this.mailServerAttribs.encryption.TVal);

                if (this.mailServerAttribs.authenticationType.TVal == Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_AlertMailServerAttribs.TAuthenticationType.ttAnonymous)
                    this.authAnoyRButton.Checked = true;
                else
                    this.authBasicRButton.Checked = true;
                this.authBasicUserTBox.Text = this.mailServerAttribs.authBasicUserName.TVal;
                this.authBasicPasswordTBox.Text = this.mailServerAttribs.authBasicUserPassword.TVal;
                this.authBasicConfirmPasswordTBox.Text = this.mailServerAttribs.authBasicUserPassword.TVal;
            }

            this.mailServerTBox.ReadOnly = _bReadOnly;
            this.portTBox.ReadOnly = _bReadOnly;
            this.emailTBox.ReadOnly = _bReadOnly;
            this.displayNameTBox.ReadOnly = _bReadOnly;
            this.encryptionComboBox.Enabled = !_bReadOnly;

            this.authAnoyRButton.Enabled = !_bReadOnly;
            this.authBasicRButton.Enabled = !_bReadOnly;
            this.authBasicUserTBox.ReadOnly = _bReadOnly;
            this.authBasicPasswordTBox.ReadOnly = _bReadOnly;
            this.authBasicConfirmPasswordTBox.ReadOnly = _bReadOnly;

            RefreshUI();
        }
        public Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_AlertMailServerAttribs GetData()
        {
            if (ValidateInput() && this.mailServerAttribs != null)
            {
                this.mailServerAttribs.mailServerName.TVal = this.mailServerTBox.Text;
                UInt32 portNumber;
                if (UInt32.TryParse(this.portTBox.Text, out portNumber))
                    this.mailServerAttribs.portNumber.TVal = portNumber;
                this.mailServerAttribs.fromEmail.TVal = this.emailTBox.Text;
                this.mailServerAttribs.fromDisplayName.TVal = this.displayNameTBox.Text;

                // CR.18713 - Add SSL & TSL
                this.mailServerAttribs.encryption.EVal = Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_AlertMailServerAttribs.AttribsMemberEnum_TEncryptionType.GetEnumValueFromAlias(this.encryptionComboBox.Text);

                if (this.authAnoyRButton.Checked)
                {
                    this.mailServerAttribs.authenticationType.TVal = Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_AlertMailServerAttribs.TAuthenticationType.ttAnonymous;
                }
                else if (this.authBasicRButton.Checked)
                {
                    this.mailServerAttribs.authenticationType.TVal = Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_AlertMailServerAttribs.TAuthenticationType.ttBasic;
                    this.mailServerAttribs.authBasicUserName.TVal = this.authBasicUserTBox.Text;
                    this.mailServerAttribs.authBasicUserPassword.TVal = this.authBasicPasswordTBox.Text;
                }
            }
            return this.mailServerAttribs;
        }

        private void EditMailServerDialog_Load(object sender, EventArgs e)
        {
            this.ActiveControl = this.mailServerTBox;
        }

        private void authCheckedChanged(object sender, EventArgs e)
        {
            RefreshUI();
        }
        private void RefreshUI()
        {
            this.authBasicUserTBox.Enabled = this.authBasicRButton.Checked;
            this.authBasicPasswordTBox.Enabled = this.authBasicRButton.Checked;
            this.authBasicConfirmPasswordTBox.Enabled = this.authBasicRButton.Checked;

            this.authBasicUserLabel.Enabled = this.authBasicRButton.Checked;
            this.authBasicPasswordLabel.Enabled = this.authBasicRButton.Checked;
            this.authBasicConfirmPasswordLabel.Enabled = this.authBasicRButton.Checked;
        }

        private bool ValidateInput()
        {
            bool bValid = this.bReadOnly;
            Control errorControl = null;
            if (!this.bReadOnly)
            {
                try
                {
                    if (string.IsNullOrEmpty(this.mailServerTBox.Text))
                    {
                        errorControl = this.mailServerTBox;
                        throw new Exception("Mail Server cannot be empty.");
                    }

                    if (string.IsNullOrEmpty(this.portTBox.Text))
                    {
                        errorControl = this.portTBox;
                        throw new Exception("Port cannot be empty.");
                    }
                    int portNumber = 0;
                    if (Int32.TryParse(this.portTBox.Text, out portNumber) == false)
                    {
                        errorControl = this.portTBox;
                        throw new Exception("Port must be a number.");
                    }
                    if (string.IsNullOrEmpty(this.emailTBox.Text))
                    {
                        errorControl = this.emailTBox;
                        throw new Exception("From E-mail Address cannot be empty.");
                    }
                    if (this.authBasicRButton.Checked)
                    {
                        //if (string.IsNullOrEmpty(this.authBasicDomainTBox.Text))
                        //{
                        //    errorControl = this.authBasicDomainTBox;
                        //    throw new Exception("Authentication Domain cannot be empty.");
                        //}
                        if (string.IsNullOrEmpty(this.authBasicUserTBox.Text))
                        {
                            errorControl = this.authBasicUserTBox;
                            throw new Exception("Authentication Username cannot be empty.");
                        }
                        if (string.IsNullOrEmpty(this.authBasicPasswordTBox.Text))
                        {
                            errorControl = this.authBasicPasswordTBox;
                            throw new Exception("Authentication Password cannot be empty.");
                        }
                        if (string.IsNullOrEmpty(this.authBasicConfirmPasswordTBox.Text))
                        {
                            errorControl = this.authBasicConfirmPasswordTBox;
                            throw new Exception("Authentication Confirm Password cannot be empty.");
                        }
                        if (!string.Equals(this.authBasicPasswordTBox.Text, this.authBasicConfirmPasswordTBox.Text, StringComparison.OrdinalIgnoreCase))
                        {
                            errorControl = this.authBasicPasswordTBox;
                            throw new Exception("Authentication Password and Confirm Password do not match.");
                        }
                    }
                    bValid = true;
                }
                catch (Exception ex)
                {
                    try
                    {
                        if (errorControl != null)
                            this.ActiveControl = errorControl;
                    }
                    catch (Exception)
                    {
                    }

                    HandleExceptions.DisplayException(ex);
                }
            }
            return bValid;
        }

        private void btnOk_Click(object sender, EventArgs e)
        {
            if (!ValidateInput())
                DialogResult = System.Windows.Forms.DialogResult.None;
        }
    }
}
