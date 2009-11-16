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
			noFlickerListView1.SetSortColumn(m_lastColumn, m_sortOrder);
			//noFlickerListView1.ListViewItemSorter = new
		}

		public void SetData(Lic_ServerDataAttribs _data)
		{
			if (_data != null)
			{
				lastTouchLabel.Text = _data.lastTouchDate.TVal.ToLocalTime().ToString();
				versionLabel.Text = string.Format("{0}.{1}.{2}.{3}", _data.versionMajor, _data.versionMinor, _data.versionSubMajor, _data.versionSubMinor);

				inClockViolLabel.Text = _data.bInClockViol.TVal == true ? "True" : "False";
				clockViolCountLabel.Text = _data.clockViolCount.TVal.ToString();
				lastClockViolationLabel.Text = ((DateTime.Compare(new DateTime(1900, 1, 1), _data.clockViolLastDate.TVal)) != 0) ? _data.clockViolLastDate.TVal.ToLocalTime().ToString() : "Never Had Violation.";

				noFlickerListView1.BeginUpdate();
				noFlickerListView1.Items.Clear();
				foreach (Lic_ServerDataAttribs.Lic_ServerDataFileInfoAttribs tmpSvrFileInfoAttribs in _data.fileInfoList.TVal)
				{
					ListViewItem lvi = new ListViewItem();
					lvi.Text = tmpSvrFileInfoAttribs.LicName;
					lvi.SubItems.Add(tmpSvrFileInfoAttribs.LicFileVerificationCode);
					lvi.SubItems.Add(tmpSvrFileInfoAttribs.LicFileName);
					lvi.SubItems.Add(tmpSvrFileInfoAttribs.LicModifiedDate.TVal.ToLocalTime().ToString());
					lvi.SubItems.Add(tmpSvrFileInfoAttribs.LicCurrentActivations.TVal.ToString());
					noFlickerListView1.Items.Add(lvi);
				}
				noFlickerListView1.EndUpdate();

				clockViolationsListView.BeginUpdate();
				clockViolationsListView.Items.Clear();
				foreach (Lic_ServerDataAttribs.Lic_ClockViolationInfoAttribs tmpClockViolInfoAttribs in _data.clockViolHistoryList.TVal)
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
		private void noFlickerListView1_ColumnClick(object sender, ColumnClickEventArgs e)
		{
			if (e.Column == m_lastColumn)
				m_sortOrder = (m_sortOrder == SortOrder.Ascending) ? SortOrder.Descending : SortOrder.Ascending;
			m_lastColumn = e.Column;
			noFlickerListView1.SetSortColumn(m_lastColumn, m_sortOrder);
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
				System.Windows.Forms.Clipboard.SetText(getCopyTextForListView(noFlickerListView1));
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
