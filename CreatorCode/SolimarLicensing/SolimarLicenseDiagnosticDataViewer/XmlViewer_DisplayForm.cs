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
    public partial class XmlViewer_DisplayForm : Form
    {
        public XmlViewer_DisplayForm()
        {
            InitializeComponent();
        }

        public void SetData(string _data)
        {
            if (_data != null)
            {
                textBox1.Text = _data;
                textBox1.SelectionStart = 0;
                textBox1.SelectionLength = 0;
            }
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

        private void textBox1_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.A && e.Control)
            {
                textBox1.SelectAll();
            }
            else if (e.KeyCode == Keys.F3)
            {
                findTSButton_Click(findTSButton, new EventArgs());
            }
        }
        private void findTSTextBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.A && e.Control)
            {
                findTSTextBox.SelectAll();
            }
            else if (e.KeyCode == Keys.Return)
            {
                textBox1.Focus();
                findTSButton_Click(findTSButton, new EventArgs());
            }
        }

        private void findTSButton_Click(object sender, EventArgs e)
        {
            int location = textBox1.SelectionStart;
            bool bRetry = true;
            int startIdx = textBox1.SelectionStart;
            while (bRetry)
            {
                location = textBox1.Text.IndexOf(findTSTextBox.TextBox.Text, (startIdx + 1 < textBox1.Text.Length) ? (startIdx + 1) : 0);
                if (location == -1)
                {
                    if (startIdx == -1)
                    {
                        textBox1.SelectionStart = 0;
                        textBox1.SelectionLength = 0;
                        bRetry = false;
                    }
                    else
                    {
                        // loop again
                        startIdx = -1;
                    }
                }
                else
                {
                    bRetry = false;
                    textBox1.SelectionStart = location;
                    startIdx = textBox1.SelectionStart;
                    textBox1.SelectionLength = findTSTextBox.TextBox.Text.Length;
                    textBox1.ScrollToCaret();
                }
            }
        }


    }
}
