using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace SolimarLicenseDiagnosticDataViewer
{
    public partial class SolimarValidationToken_DisplayForm : Form
    {
        public SolimarValidationToken_DisplayForm()
        {
            InitializeComponent();
        }

        public void SetData(string _data)
        {
            if (_data != null)
            {
                string[] csvList = _data.Split(',', '\r', '\n');
                string key = "";
                string value = "";
                for (int idx = 0; idx < csvList.Length; idx++)
                {
                    key = csvList[idx].Trim('\"');
                    if (key.Length != 0)
                    {
                        value = csvList[idx + 1].Trim('\"');
                        if (string.Compare("Company", key, true) == 0)
                            companyTextBox.Text = value;
                        else if (string.Compare("Computer Name", key, true) == 0)
                            compNameTextBox.Text = value;
                        else if (string.Compare("Mac Address", key, true) == 0)
                            macTextBox.Text = value;
                        else if (string.Compare("Bios Serial Number", key, true) == 0)
                            biosTextBox.Text = value;
                        else if (string.Compare("System Product UUID", key, true) == 0)
                            uuidTextBox.Text = value;
                        idx++;
                    }
                }
            }
        }
        private void GeneralMouseDown(object sender, MouseEventArgs e)
        {
            if (sender is TextBox)
                (sender as TextBox).SelectAll();
        } 

        private const int CP_NOCLOSE_BUTTON = 0x200;
        protected override CreateParams CreateParams
        {
            get
            {
                CreateParams cp = base.CreateParams;
                cp.ClassStyle |= CP_NOCLOSE_BUTTON;	// Disable Close button
                return cp;
            }
        }
    }
}
