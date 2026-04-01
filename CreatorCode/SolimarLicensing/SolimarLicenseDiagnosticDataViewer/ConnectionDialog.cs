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
	public partial class ConnectionDialog : Form
	{
		public ConnectionDialog()
		{
			InitializeComponent();
		}

        public void SetData(System.Collections.Generic.List<string> _stringList)
        {
            licenseServerComboBox.BeginUpdate();
            licenseServerComboBox.Items.Clear();
            if (_stringList != null)
            {
                licenseServerComboBox.Items.AddRange(_stringList.ToArray());
                if(licenseServerComboBox.Items.Count > 0)
                    licenseServerComboBox.SelectedIndex = 0;
            }
            licenseServerComboBox.EndUpdate();
        }

        public System.Collections.Generic.List<string> GetData()
        {
            System.Collections.Generic.List<string> svrList = new System.Collections.Generic.List<string>();
            string selectedSvr = licenseServerComboBox.Text;
            svrList.Add(selectedSvr);
            foreach (string svr in licenseServerComboBox.Items)
            {
                if(string.Compare(selectedSvr, svr, true) != 0)
                    svrList.Add(svr);
            }
            return svrList;
        }


        

        public delegate bool DelegateParamStringReturnBool(string _str1Param);

        public DelegateParamStringReturnBool LoginCallBack = null;

        private void connectButton_Click(object sender, EventArgs e)
        {
            try
            {
                bool bSuccess = false;
                if (this.LoginCallBack != null)
                {
                    this.Cursor = Cursors.WaitCursor;
                    //this.Enabled = false;
                    bSuccess = this.LoginCallBack(licenseServerComboBox.Text);
                }

                this.DialogResult = bSuccess == true ? DialogResult.OK : DialogResult.None;
            }
            catch (Exception)
            {
                this.DialogResult = DialogResult.None;
            }
            finally
            {
                //this.Enabled = true;
                this.Cursor = Cursors.Default;
            }
            
        }

        private void cancelButton_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
        }

        private void licenseServerComboBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter || e.KeyCode == Keys.Return)
                connectButton_Click(sender, e);
            else
                general_KeyDown(sender, e);
        }

        private void general_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Escape)
                cancelButton_Click(sender, new EventArgs());
        }
	}
}
