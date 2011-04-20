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
    public partial class Lic_SoftwareSpecAttribs_DisplayForm : Base_DisplayForm
    {
        public Lic_SoftwareSpecAttribs_DisplayForm()
        {
            InitializeComponent();
        }
        public void SetData(Lic_PackageAttribs.Lic_SoftwareSpecAttribs _data)
        {
            softwareSpecViewerControl1.SetData(_data);
        }
    }
}
