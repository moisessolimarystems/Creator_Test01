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
	public partial class Lic_EmulationInfoAttribs_DisplayForm : Base_DisplayForm
	{
		public Lic_EmulationInfoAttribs_DisplayForm()
		{
			InitializeComponent();
		}
		public void SetData(Lic_EmulationInfoAttribs _data)
		{
			this.emulatorInViolationLabel.Text = string.Empty;
			this.emulatorBypassLabel.Text = string.Empty;
			this.emulatorApiStatusCodeLabel.Text = string.Empty;
			this.emulatorApiFoundEmulatorLabel.Text = string.Empty;
			this.emulatorServiceTBox.Text = string.Empty;
			this.emulatorExServiceTBox.Text = string.Empty;
			if (_data != null)
			{
				this.emulatorInViolationLabel.Text = _data.bDongleEmulatorDetected.TVal.ToString();
				this.emulatorBypassLabel.Text = _data.bBypassDongleEmulatorCheck.TVal.ToString();
				this.emulatorApiStatusCodeLabel.Text = _data.checkEmulatorCall_StatusCode.TVal.ToString();
				this.emulatorApiFoundEmulatorLabel.Text = _data.bCheckEmulatorCall_FoundEmulator.TVal.ToString();

				StringBuilder sBuilder = new StringBuilder();
				foreach (object serviceName in _data.knownEmulatorServicesList.TVal)
				{
					if (sBuilder.Length > 0)
						sBuilder.Append("\r\n");
					sBuilder.Append(serviceName.ToString());
				}
				this.emulatorServiceTBox.Text = sBuilder.ToString();
				this.emulatorExServiceTBox.Text = _data.excludeService.TVal.ToString();
			}
		}
	}
}
