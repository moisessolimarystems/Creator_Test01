using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Client.Creator.CreatorService;

namespace Client.Creator
{
    public partial class DestinationNameEditor : Shared.VisualComponents.DialogBaseForm
    {
        private string _destinationName = "";

        public string DestinationName
        {
            get{ return _destinationName;}
            set { _destinationName = value; }
        }

        public DestinationNameEditor()
        {
            InitializeComponent();
        }

        private void DestinationNameEditor_FinishDialog(object sender, Shared.VisualComponents.FinishDialogEventArgs e)
        {
            _destinationName = destNameTextBox.Text;
        }

        private void DestinationNameEditor_InitDialog(object sender, Shared.VisualComponents.InitDialogEventArgs e)
        {

        }
    }
}
