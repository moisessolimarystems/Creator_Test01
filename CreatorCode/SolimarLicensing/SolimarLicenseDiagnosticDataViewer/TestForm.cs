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
    public partial class TestForm : Form
    {
        public TestForm()
        {
            InitializeComponent();
        }

        
        public void SetData(string _newText)
        {
            SetData("", _newText);
        }
        public void SetData(string _newTitle, string _newText)
        {
            if (_newTitle.Length > 0)
                this.Text = _newText;
            textBox1.Text = _newText;
        }

        private void textBox1_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.A && e.Control)
            {
                textBox1.SelectAll();
            }
        }

        private void toolStripDropDownButton1_Click(object sender, EventArgs e)
        {
            this.FormBorderStyle = FormBorderStyle.FixedSingle;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            listView1.Visible = false;

        }

        private void button2_Click(object sender, EventArgs e)
        {
            listView1.Visible = true;
        }
    }
}
