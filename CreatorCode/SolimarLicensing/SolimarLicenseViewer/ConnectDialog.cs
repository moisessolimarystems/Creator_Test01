using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using System.Runtime.InteropServices;

namespace SolimarLicenseViewer
{
    public partial class ConnectDialog : Shared.VisualComponents.DialogBaseForm
    {
        public ConnectDialog(CommunicationLink commLink)
        {
            m_CommLink = commLink;
            InitializeComponent();
        }

        private void ConnectDialog_InitDialog(object sender, Shared.VisualComponents.InitDialogEventArgs e)
        {
            ConnectDialogData d = e.Data as ConnectDialogData;
            if (d != null)
            {
                ServerNameComboBox.Items.AddRange(d.ServerList.ToArray());
                if (ServerNameComboBox.Items.Count > 0)
                    ServerNameComboBox.SelectedIndex = 0;
            }            
        }

        private void ConnectDialog_FinishDialog(object sender, Shared.VisualComponents.FinishDialogEventArgs e)
        {
            if (e.Result != DialogResult.OK)
                return;

            ConnectDialogData d = e.Data as ConnectDialogData;
            if (d == null)
                e.Data = d = new ConnectDialogData();

            if (d.ServerList.Contains(ServerNameComboBox.Text))
                d.ServerList.Remove(ServerNameComboBox.Text);
            d.ServerList.Insert(0, ServerNameComboBox.Text);
        }

        private void ServerNameComboBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter || e.KeyCode == Keys.Return)
            {
                btnOk_Click(sender, new EventArgs());
            }
            else if (e.KeyCode == Keys.Escape)
            {
                btnCancel_Click(sender, new EventArgs());
            }
        }

        private void btnOk_Click(object sender, EventArgs e)
        {
            Cursor.Current = Cursors.WaitCursor;
            try
            {
                using (Shared.VisualComponents.BaseMessageDialog messageDialog = new Shared.VisualComponents.BaseMessageDialog())
                {
                    messageDialog.SetData(new Shared.VisualComponents.MessageBoxData());
                    messageDialog.Show();

                    messageDialog.SetMessage(string.Format("Connecting to {0}...", ServerNameComboBox.Text));
                    messageDialog.Update();

                    m_CommLink.Async_Connect(ServerNameComboBox.Text);
                    errorProvider1.SetError(this.ServerNameComboBox, "");

                    messageDialog.Hide();
                }

                this.DialogResult = DialogResult.OK;
            }
            catch (COMException ex)
            {
                this.DialogResult = DialogResult.None;
                ServerNameComboBox.Select(0, this.ServerNameComboBox.Text.Length);
                errorProvider1.SetError(this.ServerNameComboBox, "Failed to Connect to Specified Server - " + ex.Message);
                HandleExceptions.DisplayException(this, ex, "Failed to Connect to Specified Server", "Connection Failed");
            }
            Cursor.Current = Cursors.Default;
        }
        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
        }

        #region private members

        private CommunicationLink m_CommLink;

        #endregion
    }

    #region ConnectDialogData class
    public class ConnectDialogData : Shared.VisualComponents.DialogData
    {
        private List<String> serverList;

        #region Constructors

        public ConnectDialogData()
        {
            this.serverList = new List<String>();
            this.serverList.Clear();
        }

        public ConnectDialogData(String[] serverList)
        {
            this.serverList = new List<String>();
            this.serverList.AddRange(serverList);        
        }
        #endregion

        #region Properties

        public List<String> ServerList
        {
            get { return this.serverList; }
            set { this.serverList.AddRange(value); }
        }
        #endregion
    }
    #endregion
}

