using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using System.Runtime.InteropServices;
using System.ServiceModel;
using Client.Creator.CreatorService;
using System.Configuration;
using System.ServiceModel.Configuration;

namespace Client.Creator
{
    public partial class ConnectDialog : Shared.VisualComponents.DialogBaseForm
    {
        public ConnectDialog()
        {
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
                this.btnOk.Focus();
            }            
        }

        private void ConnectDialog_FinishDialog(object sender, Shared.VisualComponents.FinishDialogEventArgs e)
        {
            if (e.Result != DialogResult.OK)
                return;

            ConnectDialogData d = e.Data as ConnectDialogData;
            if (d == null)
                e.Data = d = new ConnectDialogData();

            if (!d.ServerList.Contains(ServerNameComboBox.Text))
                d.ServerList.Insert(0, ServerNameComboBox.Text);
            else
            {
                d.ServerList.Remove(ServerNameComboBox.Text);
                d.ServerList.Insert(0, ServerNameComboBox.Text);
            }
        }

        private void ServerNameComboBox_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter || e.KeyCode == Keys.Return)
            {   
                ValidateServerNameComboBox();
                if (m_ValidServer)
                    DialogResult = DialogResult.OK;
            }
            else if (e.KeyCode == Keys.Escape)
            {
                DialogResult = DialogResult.Cancel;
            }
        }

        public static string GetEndPointName()
        {
            string endpointName = "Unknown";

            Configuration appConfig = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
            ServiceModelSectionGroup serviceModel = ServiceModelSectionGroup.GetSectionGroup(appConfig);
            BindingsSection bindings = serviceModel.Bindings;

            ChannelEndpointElementCollection endpoints = serviceModel.Client.Endpoints;

            for(int i=0; i<endpoints.Count; i++)
            {
                ChannelEndpointElement endpointElement = endpoints[i];
                if (endpointElement.Binding == "netTcpBinding")
                {
                    endpointName = endpointElement.Name;
                }
            }

            return endpointName;
        }

        private void ValidateServerNameComboBox()
        {
            Cursor.Current = Cursors.WaitCursor;
            try
            {
                Client.Creator.ServiceProxy.Service<ICreator>.IsValidHost(this.ServerNameComboBox.Text);
                int major = 0, minor = 0, buildversion = 0;
                Client.Creator.ServiceProxy.Service<ICreator>.Use((client) =>
                {
                    client.GetCreatorServiceVersion(ref major, ref minor, ref buildversion);
                });
                if (major == Common.VersionInfo.MAJOR_REVISION_NUMBER &&
                   minor == Common.VersionInfo.MINOR_REVISION_NUMBER &&
                   buildversion == Common.VersionInfo.BUILD_NUMBER)
                    m_ValidServer = true;
                else
                    throw new Exception(string.Format("Creator Client({0}.{1}.{2}) and Creator Service({3}.{4}.{5}) versions do not match.",
                                        Common.VersionInfo.MAJOR_REVISION_NUMBER, Common.VersionInfo.MINOR_REVISION_NUMBER, Common.VersionInfo.BUILD_NUMBER,
                                        major, minor, buildversion));
            }
            catch (Exception ex)
            {
                ServerNameComboBox.Select(0, this.ServerNameComboBox.Text.Length);

                // Set the ErrorProvider error with the text to display.
                errorProvider1.SetError(this.ServerNameComboBox, ex.Message);
            }
            Cursor.Current = Cursors.Default;    
        }

        private void btnOk_Click(object sender, EventArgs e)
        {
            ValidateServerNameComboBox();
        }

        private void ConnectDialog_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (DialogResult == DialogResult.OK)
                if (!m_ValidServer)
                    e.Cancel = true;
        }

        #region private members

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

