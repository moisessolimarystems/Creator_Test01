using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using System.Runtime.InteropServices;

namespace Client.Creator
{
    public partial class ConnectDialog : Shared.VisualComponents.DialogBaseForm
    {
        public ConnectDialog(CommunicationLink commLink)
        {
            m_CommLink = commLink;
            m_ValidServer = false;
            InitializeComponent();
        }

        private void ConnectDialog_InitDialog(object sender, Shared.VisualComponents.InitDialogEventArgs e)
        {
            ConnectDialogData d = e.Data as ConnectDialogData;
            if (d != null)
            {
                String[] serverList = new String[d.ServerList.Count];
                d.ServerList.CopyTo(serverList,0);
                ServerNameComboBox.Items.AddRange(serverList);
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

            if(!d.ServerList.Contains(ServerNameComboBox.Text))
                d.ServerList.Add(ServerNameComboBox.Text);
        }

        private void ServerNameComboBox_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter || e.KeyCode == Keys.Return)
            {   
                this.btnOk.Focus(); //set focus to force validation
                if(m_ValidServer)
                    DialogResult = DialogResult.OK;
            }
            else if (e.KeyCode == Keys.Escape)
            {
                this.ServerNameComboBox.CausesValidation = false;
                DialogResult = DialogResult.Cancel;
            }
        }

        private void ServerNameComboBox_Validated(object sender, EventArgs e)
        {
            errorProvider1.SetError(this.ServerNameComboBox, ""); 
        }

        private void ServerNameComboBox_Validating(object sender, CancelEventArgs e)
        {
            Cursor.Current = Cursors.WaitCursor;
            try
            {
                m_CommLink.Connect(ServerNameComboBox.Text);
                m_ValidServer = true;
            }
            catch (COMException ex)
            {
                e.Cancel = true;
                ServerNameComboBox.Select(0, this.ServerNameComboBox.Text.Length);

                // Set the ErrorProvider error with the text to display.
                errorProvider1.SetError(this.ServerNameComboBox, "Failed to Connect to Specified Server");
            }
            Cursor.Current = Cursors.Default;
        }

        #region private members

        private CommunicationLink m_CommLink;
        private Boolean m_ValidServer;

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

