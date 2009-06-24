using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Solimar.Licensing.Attribs;

namespace SolimarLicenseDiagnosticDataViewer
{
	public partial class Lic_SystemInfoAttribs_DisplayForm : Form
	{
		public Lic_SystemInfoAttribs_DisplayForm()
		{
			InitializeComponent();
		}

		public void SetData(Lic_SystemInfoAttribs _data)
		{
			if (_data != null)
			{
                licenseServerVersionLabel.Text = _data.licenseServerVersion.TVal;

				computerNameListView.Items.Clear();
				foreach (string computerName in _data.computerNameList.TVal)
					computerNameListView.Items.Add(computerName.ToLower());
				
				macAddressListView.Items.Clear();
				foreach (string macAddress in _data.macAddressList.TVal)
					macAddressListView.Items.Add(macAddress);

				biosSerialNumberListView.Items.Clear();
				foreach (string biosSerialNumber in _data.biosSerialNumberList.TVal)
					biosSerialNumberListView.Items.Add(biosSerialNumber);
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

		private void contextMenuStrip1_Click(object sender, EventArgs e)
		{
			string copyText = "";
			if (sender is System.Windows.Forms.ContextMenuStrip)
			{
				if (((sender as System.Windows.Forms.ContextMenuStrip).SourceControl) is Panel)
				{
					StringBuilder copyStrBuilder = new StringBuilder();
					copyStrBuilder.Append(licenseServerVersionTitleLabel.Text);
					copyStrBuilder.Append(" ");
					copyStrBuilder.Append(licenseServerVersionLabel.Text);
					copyText = copyStrBuilder.ToString();
				}
				else if (((sender as System.Windows.Forms.ContextMenuStrip).SourceControl) is ListView)
					copyText = getCopyTextForListView(((sender as System.Windows.Forms.ContextMenuStrip).SourceControl) as ListView);
			}
			if (copyText.Length != 0)
				System.Windows.Forms.Clipboard.SetText(copyText);
			else
				System.Windows.Forms.Clipboard.Clear();
		}
		private void general_KeyDown(object sender, KeyEventArgs e)
		{
			if (e.KeyCode == Keys.C && e.Control)
			{
				System.Windows.Forms.Clipboard.SetText(getCopyTextForListView(sender as ListView));
			}
			else if (e.KeyCode == Keys.A && e.Control)
			{
				foreach (ListViewItem li in (sender as ListView).Items)
					li.Selected = true;
			}
		}
		private string getCopyTextForListView(ListView _listView)
		{
			StringBuilder copyStrBuilder = new StringBuilder();
			foreach (ListViewItem lvi in _listView.SelectedItems)
			{
				StringBuilder lineStrBuilder = new StringBuilder();
				foreach (ListViewItem.ListViewSubItem subItem in lvi.SubItems)
				{
					if (lineStrBuilder.Length != 0)
						lineStrBuilder.Append(" ");
					lineStrBuilder.Append(subItem.Text);
				}
				if (copyStrBuilder.Length != 0)
					copyStrBuilder.Append("\r\n");
				copyStrBuilder.Append(lineStrBuilder.ToString());
			}
			return copyStrBuilder.ToString();
		}
	}
}
