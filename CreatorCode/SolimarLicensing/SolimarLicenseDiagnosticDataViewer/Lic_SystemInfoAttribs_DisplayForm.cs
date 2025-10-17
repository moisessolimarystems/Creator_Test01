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
    public partial class Lic_SystemInfoAttribs_DisplayForm : Base_DisplayForm
	{
		public Lic_SystemInfoAttribs_DisplayForm()
		{
			InitializeComponent();
			Shared.VisualComponents.ControlHelper.SetWindowTheme(this.valTokenListView.Handle, "Explorer", null);
		}

		public void SetData(Lic_SystemInfoAttribs _data)
		{
			if (_data != null)
			{
				licenseServerVersionLabel.Text = _data.licenseServerVersion.TVal;
				valTokenListView.Items.Clear();
				ListViewItem lvi = null;
				foreach (string computerName in _data.computerNameList.TVal)
				{
					lvi = new ListViewItem("Computer Name");
					lvi.SubItems.Add(computerName.ToLower());
					valTokenListView.Items.Add(lvi);
				}
				foreach (string macAddress in _data.macAddressList.TVal)
				{
					lvi = new ListViewItem("Mac Address");
					lvi.SubItems.Add(macAddress);
					valTokenListView.Items.Add(lvi);
				}
				foreach (string biosSerialNumber in _data.biosSerialNumberList.TVal)
				{
					lvi = new ListViewItem("Bios Serial Number");
					lvi.SubItems.Add(biosSerialNumber);
					valTokenListView.Items.Add(lvi);
				}

				foreach (string os in _data.operatingSystemList.TVal)
				{
					lvi = new ListViewItem("Operating System");
					lvi.SubItems.Add(os);
					valTokenListView.Items.Add(lvi);
				}
				foreach (string sysManufacturer in _data.systemManufacturerList.TVal)
				{
					lvi = new ListViewItem("System Manufacturer");
					lvi.SubItems.Add(sysManufacturer);
					valTokenListView.Items.Add(lvi);
				}
				foreach (string sysModel in _data.systemModelList.TVal)
				{
					lvi = new ListViewItem("System Model");
					lvi.SubItems.Add(sysModel);
					valTokenListView.Items.Add(lvi);
				}
				foreach (string sysType in _data.systemTypeList.TVal)
				{
					lvi = new ListViewItem("System Type");
					lvi.SubItems.Add(sysType);
					valTokenListView.Items.Add(lvi);
				}
				foreach (string sysUUID in _data.systemUuidList.TVal)
				{
					lvi = new ListViewItem("System UUID");
					lvi.SubItems.Add(sysUUID);
					valTokenListView.Items.Add(lvi);
				}
				foreach (string domain in _data.domainNameList.TVal)
				{
					lvi = new ListViewItem("Domain");
					lvi.SubItems.Add(domain);
					valTokenListView.Items.Add(lvi);
				}

				lvi = new ListViewItem("Part of Domain");
				lvi.SubItems.Add(_data.bPartOFDomain.TVal.ToString());
				valTokenListView.Items.Add(lvi);

				Shared.VisualComponents.ListViewHelper.ResizeListViewHeadersToMaxOfDataAndHeader(valTokenListView);
			}
		}

		private void contextMenuStrip1_Click(object sender, EventArgs e)
		{
			string copyText = "";
			ContextMenuStrip cMenuStrip = sender as System.Windows.Forms.ContextMenuStrip;
			if (cMenuStrip != null)
			{
				if (cMenuStrip.SourceControl is Panel)
				{
					StringBuilder copyStrBuilder = new StringBuilder();
					copyStrBuilder.Append(licenseServerVersionTitleLabel.Text);
					copyStrBuilder.Append(" ");
					copyStrBuilder.Append(licenseServerVersionLabel.Text);
					copyText = copyStrBuilder.ToString();
				}
				else if (cMenuStrip.SourceControl is ListView)
					copyText = getCopyTextForListView(cMenuStrip.SourceControl as ListView);
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
