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
    public partial class Lic_LicenseInfoAttribs_DisplayForm : Base_DisplayForm
    {
        public Lic_LicenseInfoAttribs_DisplayForm() : base()
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
    }

    
    
	
}
