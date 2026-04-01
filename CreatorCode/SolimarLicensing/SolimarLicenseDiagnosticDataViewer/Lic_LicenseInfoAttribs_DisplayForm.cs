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

	public partial class Lic_LicenseInfoAttribs_DisplayForm : Form
	{
		public Lic_LicenseInfoAttribs_DisplayForm()
		{
			InitializeComponent();
			
		}
		public void SetData(Lic_PackageAttribs.Lic_LicenseInfoAttribs _data)
		{
			if (_data != null)
			{
				string softwareLicense = Solimar.Licensing.Attribs.Lic_LicenseInfoAttribsHelper.GetDisplayLabel(_data);
				this.Text = string.Format("Lic_LicenseInfoAttribs [{0}]", softwareLicense);

				//licenseInfoTreeView
				//Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_SoftwareSpecAttribs g_softwareSpec;
				Solimar.Licensing.GlobalSoftwareSpec globalSwSpec = new Solimar.Licensing.GlobalSoftwareSpec();
				licenseInfoTreeView.SetData(globalSwSpec.softwareSpec, _data, true);

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
