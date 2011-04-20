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
    public partial class Lic_KeyAttribs_DisplayForm : Base_DisplayForm/*<Lic_KeyAttribs>*/
	{
		public Lic_KeyAttribs_DisplayForm()
		{
			InitializeComponent();
			Initialize();
		}
		public void Initialize()
		{
			m_heightPanel = keyListView.Height;
			Shared.VisualComponents.ControlHelper.SetWindowTheme(this.keyListView.Handle, "Explorer", null);
			ToogleControl(false);
			//this.Height -= m_heightPanel;
		}
		private int m_heightPanel;
		//public override void SetData(Lic_KeyAttribs _data)
        public void SetData(Lic_KeyAttribs _data)
		{
			if (_data != null)
			{
				this.Text = string.Format("Lic_KeyAttribs: [{0}]", _data.keyName);

				lic_KeyAttribs_DisplayControl.SetData(_data);

				//this.keyNameLabel.Text = _data.keyName.TVal;
				this.keyListView.BeginUpdate();
				this.keyListView.Items.Clear();
				int rowIdx = 0;
				//int productIdx = -1;
				//string version = "";
				foreach (AttribsMemberBuffer bufferRow in _data.layout.TVal)
				{
					ListViewItem lvi = new ListViewItem();
					for (int bufIdx = 0; bufIdx < bufferRow.TVal.Length; bufIdx += 2)
					{
						if (bufIdx == 0)
							lvi.Text = string.Format("{0:X2}{1:X2}", bufferRow.TVal[bufIdx], bufferRow.TVal[bufIdx + 1]);
						else
							lvi.SubItems.Add(string.Format("{0:X2}{1:X2}", bufferRow.TVal[bufIdx], bufferRow.TVal[bufIdx + 1]));

						//if (rowIdx == 7 && bufIdx == 2)
						//{
						//    productIdx = bufferRow.TVal[bufIdx + 1];
						//}
						//if (rowIdx == 7 && bufIdx == 4)
						//{
						//    version = string.Format("{0:x}.{1:x}{2:x}", bufferRow.TVal[bufIdx]/16, bufferRow.TVal[bufIdx]%16, bufferRow.TVal[bufIdx+1]/16);
						//}
					}

					this.keyListView.Items.Add(lvi);
					rowIdx++;
				}


				//if (productIdx == 0xff)	//Verification Key
				//{
				//    this.verificationCodeLabel.Text = _data.licenseCode.TVal;
				//    this.modifiedDateLabel.Text = _data.packetCreationDate.TVal.ToLocalTime().ToString();
				//    //this.currentActivationsLabel.Text = _data.currentActivations.TVal.ToString();
				//    if (_data.keyVersion.TVal == 1) // Populate Activation ListView
				//    {
				//        activitySlotLView.BeginUpdate();
				//        activitySlotLView.Items.Clear();
				//        for (int idx = 0; idx < _data.activationInfoList.TVal.Count; idx++)
				//        {
				//            ListViewItem lvi = new ListViewItem();
				//            Lic_KeyAttribs.Lic_ActivationInfoAttribs actInfo = (Lic_KeyAttribs.Lic_ActivationInfoAttribs)_data.activationInfoList.TVal[idx];
				//            lvi.Text = idx.ToString();
				//            //lvi.SubItems.Add(actInfo.activationSlotCurrentActivation.TVal.ToString());
				//            lvi.SubItems.Add(string.Format("{0} (0x{0:X2})", actInfo.activationSlotCurrentActivation.TVal));
				//            //lvi.SubItems.Add(actInfo.activationSlotHoursToExpire.TVal.ToString());
				//            lvi.SubItems.Add(string.Format("{0} (0x{0:X3})", actInfo.activationSlotHoursToExpire.TVal));
				//            activitySlotLView.Items.Add(lvi);
				//        }
				//        Shared.VisualComponents.ListViewHelper.ResizeListViewHeadersToMaxOfDataAndHeader(activitySlotLView);
				//        activitySlotLView.EndUpdate();

				//    }
				//}
				//else  //Product
				//{
				//    if (_data.keyName.TVal.Contains('{') == true) // Uninitialized Key has a '{' in its name
				//    {
				//        this.verificationCodeTitleLabel.Text = "Type:";
				//        this.verificationCodeLabel.Text = "Uninitialized Key";
				//        this.modifiedDateTitlelabel.Visible = false;
				//        this.modifiedDateLabel.Visible = false;
				//    }
				//    else
				//    {
				//        this.verificationCodeTitleLabel.Text = "Product:";
				//        this.verificationCodeLabel.Text = Lic_PackageAttribs.AttribsMemberEnum_TLic_ProductID.GetAlias((Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID)productIdx);
				//        this.modifiedDateTitlelabel.Text = "Version:";
				//        this.modifiedDateLabel.Text = version;
				//    }
				//    this.currentActivationsTitleLabel.Visible = false;
				//    this.currentActivationsLabel.Visible = false;
				//}

				this.keyListView.EndUpdate();
			}
		}

		private void ToogleControl(bool _bShow)
		{
			if (_bShow) // show
			{
				displayButton.Tag = "1";
				displayButton.Text = "Hide Key Cells";
				keyListView.Visible = true;
				this.Height += m_heightPanel;
				this.FormBorderStyle = FormBorderStyle.Sizable;
			}
			else // hide
			{
				displayButton.Tag = "0";
				displayButton.Text = "Show Key Cells";
				m_heightPanel = keyListView.Height;
				keyListView.Visible = false;
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

		private void contextMenuStrip1_Click(object sender, EventArgs e)
		{
		}

		private void general_KeyDown(object sender, KeyEventArgs e)
		{
			if (e.KeyCode == Keys.C && e.Control)
			{
				System.Windows.Forms.Clipboard.SetText(getCopyTextForListView(keyListView));
			}
			else if (e.KeyCode == Keys.A && e.Control)
			{
				keyListView.BeginUpdate();
				foreach (ListViewItem li in keyListView.Items)
					li.Selected = true;
				keyListView.EndUpdate();
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

