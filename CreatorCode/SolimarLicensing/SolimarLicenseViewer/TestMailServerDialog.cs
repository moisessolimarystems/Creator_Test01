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
    public partial class TestMailServerDialog : Shared.VisualComponents.DialogBaseForm
    {
        public TestMailServerDialog()
        {
            InitializeComponent();
            InitializeInternal();
            this.actionTestMailServer = null;
            this.actionConfigureMailServer = null;
        }
        public TestMailServerDialog(Action<string> _actionTestMailServer, Action<object> _actionConfigureMailServer)
        {
            InitializeComponent();
            InitializeInternal();
            this.actionTestMailServer = _actionTestMailServer;
            this.actionConfigureMailServer = _actionConfigureMailServer;
        }
        private void InitializeInternal()
        {
            this.subjectTBox.Text = this.testMailServerAttribs.subject.TVal;
            this.bodyTBox.Text = this.testMailServerAttribs.body.TVal;
        }
        private List<string> previousRecipientList = new List<string>();
        public void SetData(IList<string> _previousRecipientList)
        {
            if (_previousRecipientList != null)
            {
                this.previousRecipientList = (List<string>)_previousRecipientList;
                this.toTBox.AutoCompleteCustomSource.Clear();
                this.toTBox.AutoCompleteCustomSource.AddRange(this.previousRecipientList.ToArray());
                
            }
        }
        public IList<string> GetData()
        {
            return this.previousRecipientList;
        }

        private Action<string> actionTestMailServer = null;
        private Action<object> actionConfigureMailServer = null;
        private Lic_TestMailServer testMailServerAttribs = new Lic_TestMailServer();

        private void testMailButton_Click(object sender, EventArgs e)
        {
            try
            {
                Cursor.Current = Cursors.WaitCursor;
                if (!this.previousRecipientList.Contains(this.toTBox.Text))
                {
                    this.previousRecipientList.Add(this.toTBox.Text);
                    this.toTBox.AutoCompleteCustomSource.Clear();
                    this.toTBox.AutoCompleteCustomSource.AddRange(this.previousRecipientList.ToArray());
                }

                string[] recipientList = this.toTBox.Text.Split(';');
                this.testMailServerAttribs.recipentsList.TVal.Clear();
                foreach (string recipient in recipientList)
                    this.testMailServerAttribs.recipentsList.TVal.Add(recipient);
                this.testMailServerAttribs.subject.TVal = this.subjectTBox.Text;
                this.testMailServerAttribs.body.TVal = this.bodyTBox.Text;

                this.actionTestMailServer(this.testMailServerAttribs.ToString());

                MessageBox.Show(this, "Successfully send test e-mail.");
            }
            catch (Exception ex)
            {
                HandleExceptions.DisplayException(ex);
            }
            finally
            {
                Cursor.Current = Cursors.Default;
            }
        }

        private void TestMailServerDialog_Load(object sender, EventArgs e)
        {
            this.ActiveControl = this.toTBox;
        }

        private void configMailButton_Click(object sender, EventArgs e)
        {
            try
            {
                this.actionConfigureMailServer(null);
            }
            catch (Exception ex)
            {
                HandleExceptions.DisplayException(ex);
            }
        }
    }
}
