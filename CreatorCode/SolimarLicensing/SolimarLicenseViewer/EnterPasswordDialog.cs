using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace SolimarLicenseViewer
{
    public partial class EnterPasswordDialog : Shared.VisualComponents.DialogBaseForm
    {
        public EnterPasswordDialog()
        {
            InitializeComponent();
        }
        public void SetData()
        {
        }
        public string GetData()
        {
            return password;
        }
        private string password;

        private void passwordTextBox_TextChanged(object sender, EventArgs e)
        {
            btnOk.Enabled = passwordTextBox.Text.Length > 0;
        }

        private void EnterPasswordDialog_Load(object sender, EventArgs e)
        {
            passwordTextBox_TextChanged(passwordTextBox, new EventArgs());
            passwordTextBox.Focus();
        }

        private void btnOk_Click(object sender, EventArgs e)
        {
            password = passwordTextBox.Text;
        }
    }
}
