using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace SolimarLicenseViewer
{
    public partial class EditConnectionSettingsDialog : Shared.VisualComponents.DialogBaseForm
    {
        public EditConnectionSettingsDialog()
        {
            InitializeComponent();
        }


        public void SetData(ConnectionSettings2[] _connSettingsArray, string _caption)
        {
            this.Text = _caption;
            foreach(var setting in _connSettingsArray)
            {
                setting.IsDirty = false;
            }
            singleProdPropertyGrid.SelectedObjects = _connSettingsArray;
            
        }
        public ConnectionSettings2[] GetData( )
        {
            //ConnectionSettings2[] retVal = null;


            return singleProdPropertyGrid.SelectedObjects as ConnectionSettings2[];
        }

        private void General_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Escape)
            {
                CancelButton.PerformClick();
            }
            else if (e.KeyCode == Keys.Escape)
            {
                //this.ok.PerformClick();
            }
        }

        private void singleProdPropertyGrid_PreviewKeyDown(object sender, PreviewKeyDownEventArgs e)
        {
            General_KeyDown(sender, new KeyEventArgs(e.KeyData));
        }

        private void EditConnectionSettingsDialog_Load(object sender, EventArgs e)
        {
            ActiveControl = singleProdPropertyGrid;
        }
    }
}
