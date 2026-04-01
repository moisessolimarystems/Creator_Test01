using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Solimar.Licensing.Attribs;

namespace SolimarLicenseDiagnosticDataViewer
{
    public partial class Lic_KeyAttribs_DisplayControl : UserControl
    {
        public Lic_KeyAttribs_DisplayControl()
        {
            InitializeComponent();
        }
        public void SetData(Solimar.Licensing.Attribs.Lic_KeyAttribs _data)
        {
            this.keyNameLabel.Text = "";
            this.keyVersionLabel.Text = "";
            this.historyLabel.Text = "";
            this.verificationCodeLabel.Text = "";
            this.modifiedDateLabel.Text = "";
            this.lastTouchDateLabel.Text = "";
            activitySlotLView.BeginUpdate();
            activitySlotLView.Items.Clear();
            activitySlotLView.EndUpdate();

            if (_data != null)
            {
                this.keyNameLabel.Text = _data.keyName.TVal;
                this.keyVersionLabel.Text = _data.keyVersion.TVal.ToString();
                this.historyLabel.Text = _data.historyNumber.TVal.ToString();
                int productIdx = 0xff;
                int rowIdx = 0;
                string version = "";

                foreach (AttribsMemberBuffer bufferRow in _data.layout.TVal)
                {
                    ListViewItem lvi = new ListViewItem();
                    for (int bufIdx = 0; bufIdx < bufferRow.TVal.Length; bufIdx += 2)
                    {
                        if (rowIdx == 7 && bufIdx == 2)
                        {
                            productIdx = bufferRow.TVal[bufIdx + 1];
                        }
                        if (rowIdx == 7 && bufIdx == 4)
                        {
                            version = string.Format("{0:x}.{1:x}{2:x}", bufferRow.TVal[bufIdx] / 16, bufferRow.TVal[bufIdx] % 16, bufferRow.TVal[bufIdx + 1] / 16);
                        }
                    }
                    rowIdx++;
                }

                if (productIdx == 0xff)	//Verification Key
                {
                    mainBackSplitContainer.Panel2Collapsed = false;
                    this.verificationCodeTitleLabel.Text = "Verification Code:";
                    this.modifiedDateTitlelabel.Text = "Packet Creation Date:";
                    this.lastTouchDateTitleLabel.Text = "Last Touch Date:";
                    this.modifiedDateTitlelabel.Visible = true;
                    this.modifiedDateLabel.Visible = true;
                    this.lastTouchDateTitleLabel.Visible = true;
                    this.lastTouchDateLabel.Visible = true;

                    this.verificationCodeLabel.Text = _data.licenseCode.TVal;
                    this.modifiedDateLabel.Text = _data.packetCreationDate.TVal.ToLocalTime().ToString();
                    this.lastTouchDateLabel.Text = _data.currentDate.TVal.ToLocalTime().ToString();
                    //this.currentActivationsLabel.Text = _data.currentActivations.TVal.ToString();
                    if (_data.keyVersion.TVal == 1) // Populate Activation ListView
                    {
                        activitySlotLView.BeginUpdate();
                        for (int idx = 0; idx < _data.activationInfoList.TVal.Count; idx++)
                        {
                            ListViewItem lvi = new ListViewItem();
                            Lic_KeyAttribs.Lic_ActivationInfoAttribs actInfo = (Lic_KeyAttribs.Lic_ActivationInfoAttribs)_data.activationInfoList.TVal[idx];
                            lvi.Text = idx.ToString();
                            //lvi.SubItems.Add(actInfo.activationSlotCurrentActivation.TVal.ToString());
                            lvi.SubItems.Add(string.Format("{0} (0x{0:X2})", actInfo.activationSlotCurrentActivation.TVal));
                            //lvi.SubItems.Add(actInfo.activationSlotHoursToExpire.TVal.ToString());
                            lvi.SubItems.Add(string.Format("{0} (0x{0:X3})", actInfo.activationSlotHoursToExpire.TVal));
                            if (actInfo.activationSlotHoursToExpire.TVal == 0)
                                lvi.SubItems.Add("Expired");
                            else
                                lvi.SubItems.Add(_data.currentDate.TVal.ToLocalTime().AddHours(actInfo.activationSlotHoursToExpire.TVal).ToString());
                            activitySlotLView.Items.Add(lvi);
                        }
                        Shared.VisualComponents.ListViewHelper.ResizeListViewHeadersToMaxOfDataAndHeader(activitySlotLView);
                        activitySlotLView.EndUpdate();

                    }
                }
                else  //Product
                {
                    mainBackSplitContainer.Panel2Collapsed = true;
                    if (_data.keyName.TVal.Contains('{') == true) // Uninitialized Key has a '{' in its name
                    {
                        this.verificationCodeTitleLabel.Text = "Type:";
                        this.verificationCodeLabel.Text = "Uninitialized Key";
                        this.modifiedDateTitlelabel.Visible = false;
                        this.modifiedDateLabel.Visible = false;
                    }
                    else
                    {
                        this.verificationCodeTitleLabel.Text = "Product:";
                        this.verificationCodeLabel.Text = Lic_PackageAttribs.AttribsMemberEnum_TLic_ProductID.GetAlias((Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID)productIdx);
                        this.modifiedDateTitlelabel.Text = "Version:";
                        this.modifiedDateLabel.Text = version;
                        this.modifiedDateTitlelabel.Visible = true;
                        this.modifiedDateLabel.Visible = true;
                    }
                    this.lastTouchDateTitleLabel.Visible = false;
                    this.lastTouchDateLabel.Visible = false;
                }
            }
        }

        private void contextMenuStrip1_Click(object sender, EventArgs e)
        {
            string copyText = "";
            if (sender is System.Windows.Forms.ContextMenuStrip)
            {
                if (((sender as System.Windows.Forms.ContextMenuStrip).SourceControl) is Panel)
                {
                    StringBuilder copyStrBuilder = new StringBuilder();
                    copyStrBuilder.Append(keyNameTitleLabel.Text);
                    copyStrBuilder.Append(" ");
                    copyStrBuilder.Append(keyNameLabel.Text);

                    copyStrBuilder.Append("\r\n");
                    copyStrBuilder.Append(keyVersionTitleLabel.Text);
                    copyStrBuilder.Append(" ");
                    copyStrBuilder.Append(keyVersionLabel.Text);

                    copyStrBuilder.Append("\r\n");
                    copyStrBuilder.Append(historyTitleLabel.Text);
                    copyStrBuilder.Append(" ");
                    copyStrBuilder.Append(historyLabel.Text);

                    copyStrBuilder.Append("\r\n");
                    copyStrBuilder.Append(verificationCodeTitleLabel.Text);
                    copyStrBuilder.Append(" ");
                    copyStrBuilder.Append(verificationCodeLabel.Text);

                    copyStrBuilder.Append("\r\n");
                    copyStrBuilder.Append(modifiedDateTitlelabel.Text);
                    copyStrBuilder.Append(" ");
                    copyStrBuilder.Append(modifiedDateLabel.Text);

                    if (lastTouchDateTitleLabel.Visible)
                    {
                        copyStrBuilder.Append("\r\n");
                        copyStrBuilder.Append(lastTouchDateTitleLabel.Text);
                        copyStrBuilder.Append(" ");
                        copyStrBuilder.Append(lastTouchDateLabel.Text);
                    }

                    copyText = copyStrBuilder.ToString();
                }
                else if (((sender as System.Windows.Forms.ContextMenuStrip).SourceControl) is ListView)
                    copyText = getCopyTextForListView(((sender as System.Windows.Forms.ContextMenuStrip).SourceControl) as ListView);
            }
            if (copyText.Length != 0)
                System.Windows.Forms.Clipboard.SetText(copyText);
            else
                System.Windows.Forms.Clipboard.Clear();
        }
        private void general_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.C && e.Control)
            {
                System.Windows.Forms.Clipboard.SetText(getCopyTextForListView(activitySlotLView));
            }
            else if (e.KeyCode == Keys.A && e.Control)
            {
                activitySlotLView.BeginUpdate();
                foreach (ListViewItem li in activitySlotLView.Items)
                    li.Selected = true;
                activitySlotLView.EndUpdate();
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
