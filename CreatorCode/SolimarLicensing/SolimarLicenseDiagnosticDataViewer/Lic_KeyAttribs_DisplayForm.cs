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
	public partial class Lic_KeyAttribs_DisplayForm : Form
	{
		public Lic_KeyAttribs_DisplayForm()
		{
			InitializeComponent();
            Initialize();
		}
        public void Initialize()
        {
            m_heightPanel = keyListView.Height;
            ToogleControl(false);
            //this.Height -= m_heightPanel;
        }
        private int m_heightPanel;
		public void SetData(Lic_KeyAttribs _data)
		{
			if (_data != null)
			{
				this.Text = string.Format("Lic_KeyAttribs: [{0}]", _data.keyName);
				this.keyNameLabel.Text = _data.keyName.TVal;
				this.keyListView.BeginUpdate();
				this.keyListView.Items.Clear();
				int rowIdx = 0;
				int productIdx = -1;
				string version = "";
				foreach (AttribsMemberBuffer bufferRow in _data.layout.TVal)
				{
					ListViewItem lvi = new ListViewItem();
					for (int bufIdx = 0; bufIdx < bufferRow.TVal.Length; bufIdx += 2)
					{
						if (bufIdx == 0)
							lvi.Text = string.Format("{0:X2}{1:X2}", bufferRow.TVal[bufIdx], bufferRow.TVal[bufIdx+1]);
						else
							lvi.SubItems.Add(string.Format("{0:X2}{1:X2}", bufferRow.TVal[bufIdx], bufferRow.TVal[bufIdx+1]));

						if (rowIdx == 7 && bufIdx == 2)
						{
							productIdx = bufferRow.TVal[bufIdx + 1];
						}
						if (rowIdx == 7 && bufIdx == 4)
						{
							version = string.Format("{0:x}.{1:x}{2:x}", bufferRow.TVal[bufIdx]/16, bufferRow.TVal[bufIdx]%16, bufferRow.TVal[bufIdx+1]/16);
						}
					}
					
					this.keyListView.Items.Add(lvi);
					rowIdx++;
				}


				if (productIdx == 0xff)	//Verification Key
				{
					this.verificationCodeLabel.Text = _data.verificationCode.TVal;
					this.modifiedDateLabel.Text = _data.modifiedDate.TVal.ToString();
					this.currentActivationsLabel.Text = _data.currentActivations.TVal.ToString();
				}
				else  //Product
				{
					this.verificationCodeTitleLabel.Text = "Product:";
					this.verificationCodeLabel.Text = Lic_PackageAttribs.AttribsMemberEnum_TLic_ProductID.GetAlias((Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID)productIdx);
					this.modifiedDateTitlelabel.Text = "Version:";
					this.modifiedDateLabel.Text = version;
					this.currentActivationsTitleLabel.Visible = false;
					this.currentActivationsLabel.Visible = false;
				}

				this.keyListView.EndUpdate();
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

        private void ToogleControl(bool _bShow)
        {
            if (_bShow) // show
            {
                displayButton.Tag = "1";
                displayButton.Text = "Hide Key Cells";
                this.Height += m_heightPanel;
                this.FormBorderStyle = FormBorderStyle.Sizable;
            }
            else // hide
            {
                displayButton.Tag = "0";
                displayButton.Text = "Show Key Cells";
                m_heightPanel = keyListView.Height;
                this.Height -= m_heightPanel;
                this.FormBorderStyle = FormBorderStyle.FixedSingle;
            }
        }
        private void displayButton_Click(object sender, EventArgs e)
        {
            if (sender is Button)
            {
                Button tmpButton = sender as Button;
                ToogleControl(string.Compare(tmpButton.Tag as string, "0", true) == 0);
                //if (string.Compare(tmpButton.Tag as string, "0", true) == 0)
                //{
                //    tmpButton.Tag = "1";
                //    tmpButton.Text = "Hide Key Cells";
                //    this.Height += m_heightPanel;
                //}
                //else
                //{
                //    tmpButton.Tag = "0";
                //    tmpButton.Text = "Show Key Cells";
                //    this.Height -= m_heightPanel;
                //}
            }
        } 
	}
}
