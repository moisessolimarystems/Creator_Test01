using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
namespace SolimarLicenseDiagnosticDataViewer
{
    public partial class SolimarValidationToken_DisplayForm : Base_DisplayForm<string>
    {
        public SolimarValidationToken_DisplayForm()
        {
            InitializeComponent();
        }

        public override void SetData(string _data)
        {
            if (_data != null)
            {
                Solimar.Licensing.Common.SolimarValidationData tmpData = new Solimar.Licensing.Common.SolimarValidationData();
                tmpData.FromCSV(_data);
                tokenRetrieverLabel.Text = tmpData.Version;
                propertyGrid1.SelectedObject = tmpData;
            }
        }
        private void GeneralMouseDown(object sender, MouseEventArgs e)
        {
            if (sender is TextBox)
                (sender as TextBox).SelectAll();
        } 
    }
}
