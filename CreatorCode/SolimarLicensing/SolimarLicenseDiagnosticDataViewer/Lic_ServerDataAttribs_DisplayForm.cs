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
				lastTouchLabel.Text = _data.lastTouchDate.TVal.ToString();
				versionLabel.Text = string.Format("{0}.{1}.{2}.{3}", _data.versionMajor, _data.versionMinor, _data.versionSubMajor, _data.versionSubMinor);

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
	}
}
