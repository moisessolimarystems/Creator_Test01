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
	public partial class Lic_ServerDataAttribs_DisplayForm : Form
	{
		public Lic_ServerDataAttribs_DisplayForm()
		{
			InitializeComponent();
			m_lastColumn = 0;
			m_sortOrder = SortOrder.Ascending;
			licenseInfoListView.SetSortColumn(m_lastColumn, m_sortOrder);
			//noFlickerListView1.ListViewItemSorter = new
		}

		public void SetData(Lic_ServerDataAttribs _data)
		{
			m_data = _data;
			if (m_data != null)
			{

				lastTouchLabel.Text = m_data.lastTouchDate.TVal.ToLocalTime().ToString();
				versionLabel.Text = string.Format("{0}.{1}.{2}.{3}", m_data.versionMajor, m_data.versionMinor, m_data.versionSubMajor, m_data.versionSubMinor);

				inClockViolLabel.Text = m_data.bInClockViol.TVal == true ? "True" : "False";
				clockViolCountLabel.Text = m_data.clockViolCount.TVal.ToString();
				lastClockViolationLabel.Text = ((DateTime.Compare(new DateTime(1900, 1, 1), m_data.clockViolLastDate.TVal)) != 0) ? m_data.clockViolLastDate.TVal.ToLocalTime().ToString() : "Never Had Violation.";

				licenseInfoListView.BeginUpdate();
				licenseInfoListView.Items.Clear();
				foreach (Lic_ServerDataAttribs.Lic_ServerDataFileInfoAttribs tmpSvrFileInfoAttribs in m_data.fileInfoList.TVal)
				{
					ListViewItem lvi = new ListViewItem();
					lvi.Text = tmpSvrFileInfoAttribs.LicName;
					lvi.SubItems.Add(tmpSvrFileInfoAttribs.LicFileName);
					lvi.SubItems.Add(tmpSvrFileInfoAttribs.LicFileLicenseCode);
					lvi.SubItems.Add(tmpSvrFileInfoAttribs.LicModifiedDate.TVal.ToLocalTime().ToString());
					//YYY - Change Later
					//lvi.SubItems.Add(tmpSvrFileInfoAttribs.LicCurrentActivations.TVal.ToString());
					licenseInfoListView.Items.Add(lvi);
				}
				//licenseInfoListView_SelectedIndexChanged(licenseInfoListView, null);
				if (licenseInfoListView.Items.Count > 0)    //Select first item if there is one.
					licenseInfoListView.Items[0].Selected = true;
				licenseInfoListView.EndUpdate();

				clockViolationsListView.BeginUpdate();
				clockViolationsListView.Items.Clear();
				foreach (Lic_ServerDataAttribs.Lic_ClockViolationInfoAttribs tmpClockViolInfoAttribs in m_data.clockViolHistoryList.TVal)
				{
					ListViewItem lvi = new ListViewItem();
					lvi.Text = tmpClockViolInfoAttribs.fileDate.TVal.ToLocalTime().ToString();
					lvi.SubItems.Add(tmpClockViolInfoAttribs.systemDate.TVal.ToLocalTime().ToString());
					lvi.SubItems.Add(tmpClockViolInfoAttribs.fileDate.TVal.Subtract(tmpClockViolInfoAttribs.systemDate.TVal).ToString());
					clockViolationsListView.Items.Add(lvi);
				}
				clockViolationsListView.EndUpdate();

			}
		}

		private int m_lastColumn;
		private SortOrder m_sortOrder;
		private Lic_ServerDataAttribs m_data;
		private void noFlickerListView1_ColumnClick(object sender, ColumnClickEventArgs e)
		{
			if (e.Column == m_lastColumn)
				m_sortOrder = (m_sortOrder == SortOrder.Ascending) ? SortOrder.Descending : SortOrder.Ascending;
			m_lastColumn = e.Column;
			licenseInfoListView.SetSortColumn(m_lastColumn, m_sortOrder);
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
		private void contextMenuStrip1_Click(object sender, EventArgs e)
		{
			string copyText = "";
			if (sender is System.Windows.Forms.ContextMenuStrip)
			{
				if (((sender as System.Windows.Forms.ContextMenuStrip).SourceControl) is Panel)
				{
					StringBuilder copyStrBuilder = new StringBuilder();
					copyStrBuilder.Append(versionTitleLabel.Text);
					copyStrBuilder.Append(" ");
					copyStrBuilder.Append(versionLabel.Text);

					copyStrBuilder.Append("\r\n");
					copyStrBuilder.Append(lastTouchTitleLabel.Text);
					copyStrBuilder.Append(" ");
					copyStrBuilder.Append(lastTouchLabel.Text);

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
				System.Windows.Forms.Clipboard.SetText(getCopyTextForListView(licenseInfoListView));
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

		private void licenseInfoListView_SelectedIndexChanged(object sender, EventArgs e)
		{
			if (sender is ListView)
			{
				ListView lView = sender as ListView;
				string selectedLicenseName = "";
				if (lView.SelectedItems.Count > 0)
					selectedLicenseName = lView.SelectedItems[0].Text;

				Lic_ServerDataAttribs.Lic_ServerDataFileInfoAttribs fileInfo = null;
				Lic_KeyAttribs keyAttribs = null;
				if (selectedLicenseName != string.Empty)
				{
					foreach (Lic_ServerDataAttribs.Lic_ServerDataFileInfoAttribs tmpSvrFileInfoAttribs in m_data.fileInfoList.TVal)
					{
						if (string.Compare(selectedLicenseName, tmpSvrFileInfoAttribs.LicName.TVal, true) == 0)
						{
							fileInfo = tmpSvrFileInfoAttribs;
							if (fileInfo.Streamed_ActivationAttribs.TVal != string.Empty)
							{
								keyAttribs = new Lic_KeyAttribs();
								keyAttribs.AssignMembersFromStream(fileInfo.Streamed_ActivationAttribs.TVal);
								lic_KeyAttribs_DisplayControl.SetData(keyAttribs);
							}
							break;
						}
					}
				}
				lic_KeyAttribs_DisplayControl.SetData(keyAttribs);
			}
		}

	}
}
