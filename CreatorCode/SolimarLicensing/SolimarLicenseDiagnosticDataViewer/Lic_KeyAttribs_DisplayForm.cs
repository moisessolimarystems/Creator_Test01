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
					this.modifiedDateLabel.Text = _data.modifiedDate.TVal.ToLocalTime().ToString();
					this.currentActivationsLabel.Text = _data.currentActivations.TVal.ToString();
				}
				else  //Product
				{
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
					}
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
					copyStrBuilder.Append(verificationCodeTitleLabel.Text);
					copyStrBuilder.Append(" ");
					copyStrBuilder.Append(verificationCodeLabel.Text);

					copyStrBuilder.Append("\r\n");
					copyStrBuilder.Append(modifiedDateTitlelabel.Text);
					copyStrBuilder.Append(" ");
					copyStrBuilder.Append(modifiedDateLabel.Text);

					if (currentActivationsTitleLabel.Visible)
					{
						copyStrBuilder.Append("\r\n");
						copyStrBuilder.Append(currentActivationsTitleLabel.Text);
						copyStrBuilder.Append(" ");
						copyStrBuilder.Append(currentActivationsLabel.Text);
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
