using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Globalization;

namespace SolimarLicenseDiagnosticDataViewer
{
    public partial class XmlViewer_DisplayForm : Base_DisplayForm
    {
        public XmlViewer_DisplayForm() : base()
        {
            InitializeComponent();
        }
        //private readonly string RTF_HEADER = @"{\rtf1\ansi{\colortbl;\red0\green0\blue0;\red255\green0\blue0;\red0\green255\blue0;\red0\green0\blue255;}";
        private readonly string RTF_HEADER = @"{\rtf1\ansi ";
        private readonly string RTF_FOOTER = @"}";
        private string rawXml = string.Empty;
        private string selectedWord = string.Empty;
        public void SetData(string _data)
        {
            if (_data != null)
            {
                textBox1.Text = _data;
                textBox1.SelectionStart = 0;
                textBox1.SelectionLength = 0;
                TSSLabel1.Text = textBox1.Text.Length.ToString("0,0");

                this.rawXml = _data;
                richTextBox1.SelectionStart = 0;
                richTextBox1.SelectionLength = 0;
                RefreshSelection(string.Empty);
                //RefreshSelection("Vector");
 //@"this is in \b bold\b0, " +
 //@"this is in \i italics\i0, " +
 //@"and this is \ul underlined\ul0.}";
            }
        }

        private void textBox1_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.A && e.Control)
            {
                if (sender is TextBoxBase)
                    (sender as TextBoxBase).SelectAll();
                //textBox1.SelectAll();
            }
            else if (e.KeyCode == Keys.F && e.Control)
            {
                this.findTSTextBox.Text = this.richTextBox1.SelectedText;
                findTSButton_Click(findTSButton, EventArgs.Empty);
            }
            else if (e.KeyCode == Keys.F3)
            {
                findTSButton_Click(findTSButton, EventArgs.Empty);
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
                findTSButton_Click(findTSButton, EventArgs.Empty);
            }
        }

        private void RefreshSelection(string _selectionText)
        {
            if (_selectionText != this.selectedWord || String.IsNullOrEmpty(richTextBox1.Text))
            {
                this.selectedWord = _selectionText;
                string newXml = this.rawXml.Replace("\r\n", @"\line");
                if (!string.IsNullOrEmpty(_selectionText))
                    newXml = newXml.Replace(_selectionText, @"\b " + _selectionText + @"\b0 ");
                richTextBox1.Rtf = string.Format("{0}{1}{2}", RTF_HEADER, newXml, RTF_FOOTER);

                if (!String.IsNullOrEmpty(_selectionText))
                {
                    this.foundTitleSSLabel.Visible = true;
                    this.foundTitleSSLabel.Text = string.Format("Found \"{0}\" {1} times",
                        _selectionText,
                        //(this.rawXml.Split(new string[] { _selectionText }, StringSplitOptions.None).Length - 1).ToString("0,0")
                        (this.rawXml.Split(new string[] { _selectionText }, StringSplitOptions.None).Length - 1).ToString("0,0").TrimStart(new char[] {'0'})
                        );
                }
                else
                {
                    this.foundTitleSSLabel.Visible = false;
                }
            }
        }
        private void findTSButton_Click(object sender, EventArgs e)
        {
            findText(false);
        }
        private void findText(bool _bIncludeSelectedText)
        {
            //_bIncludeSelectedText
            //int location = richTextBox1.SelectionStart;
            //int startIdx = richTextBox1.SelectionStart;

            int location = richTextBox1.SelectionStart;
            int startIdx = richTextBox1.SelectionStart;

            bool bRetry = true;
            
            this.SuspendLayout();
            this.richTextBox1.SuspendLayout();
            //this.richTextBox1.ScrollBars = RichTextBoxScrollBars.None;

            RefreshSelection(findTSTextBox.TextBox.Text);

            while (bRetry)
            {
                location = richTextBox1.Text.IndexOf(findTSTextBox.TextBox.Text, (startIdx + 1 < richTextBox1.Text.Length) ? (startIdx + 1) : 0);
                if (location == -1)
                {
                    if (startIdx == -1)
                    {
                        richTextBox1.SelectionStart = 0;
                        richTextBox1.SelectionLength = 0;
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
                    richTextBox1.SelectionStart = location;
                    startIdx = textBox1.SelectionStart;
                    richTextBox1.SelectionLength = findTSTextBox.TextBox.Text.Length;
                    richTextBox1.ScrollToCaret();
                }
            }

            this.richTextBox1.ResumeLayout(false);
            this.ResumeLayout(false);
        }

        private void findToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.findTSTextBox.Text = this.richTextBox1.SelectedText;
            findTSButton_Click(findTSButton, EventArgs.Empty);
        }

        private void copyToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Clipboard.SetData(DataFormats.UnicodeText, this.richTextBox1.SelectedText);
        }

        private void selectAllToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.richTextBox1.SelectAll();
        }

        private void contextMenuStrip1_Opening(object sender, CancelEventArgs e)
        {
            bool bSelected = this.richTextBox1.SelectedText.Length > 0;
            this.findToolStripMenuItem.Visible = bSelected;
            this.copyToolStripMenuItem.Visible = bSelected;
            this.toolStripSeparator1.Visible = bSelected;
        }
    }
}
