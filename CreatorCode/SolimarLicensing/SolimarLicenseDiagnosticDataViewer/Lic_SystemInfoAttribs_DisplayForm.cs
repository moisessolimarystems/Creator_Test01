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
	}
}
