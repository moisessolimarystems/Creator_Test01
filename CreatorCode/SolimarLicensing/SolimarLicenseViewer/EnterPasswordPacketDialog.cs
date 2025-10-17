using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace SolimarLicenseViewer
{
    public partial class EnterPasswordPacketDialog : Shared.VisualComponents.DialogBaseForm
    {
        public EnterPasswordPacketDialog()
        {
            InitializeComponent();
        }
        public void SetData(string passwordPacketLocation)
        {
            passwordTextBox.Text = passwordPacketLocation;
        }
        public string GetData()
        {
            return passwordTextBox.Text;
        }

        private void directoryFindButton_Click(object sender, EventArgs e)
        {
            if (passwordTextBox.Text.Length != 0)
                this.openFileDialog1.InitialDirectory = passwordTextBox.Text;

            this.openFileDialog1.DefaultExt = "pkt";
            this.openFileDialog1.FileName = "";
            this.openFileDialog1.Filter = "Key Password Packet|*.pkt";
            this.openFileDialog1.Title = "Key Password Packet";
            if (this.openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                passwordTextBox.Text = this.openFileDialog1.FileName;
            }
        }

        private void btnOk_Click(object sender, EventArgs e)
        {
            if (System.IO.File.Exists(passwordTextBox.Text) == false)
            {
                HandleExceptions.DisplayException(this, new Exception("Unable to locate file"), "", "Error importing Key Password Packet");
                DialogResult = DialogResult.None;
            }
            else
            {
                DialogResult = DialogResult.OK;
            }
        }
    }
}
