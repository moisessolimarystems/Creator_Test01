using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Client.Creator
{
    public partial class AboutDialog : Form
    {
        public AboutDialog()
        {
            InitializeComponent();
        }

        public void SetCreatorVersion(String version) { CreatorVersionLabel.Text = version; }
        //public void SetServerVersion(String version) { ServerVersionLabel.Text = version; }

        private void okButton_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
