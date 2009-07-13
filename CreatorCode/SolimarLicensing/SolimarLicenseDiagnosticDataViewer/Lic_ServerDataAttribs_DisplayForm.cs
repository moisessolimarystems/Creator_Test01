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
	public partial class Lic_ServerDataAttribs_DisplayForm : Base_DisplayForm
	{
		public Lic_ServerDataAttribs_DisplayForm()
		{
			InitializeComponent();
			m_lastColumn = 0;
			m_sortOrder = SortOrder.Ascending;
			licenseInfoListView.SetSortColumn(m_lastColumn, m_sortOrder);
			Shared.VisualComponents.ControlHelper.SetWindowTheme(this.alertTreeView.Handle, "Explorer", null);
		}
		public Lic_ServerDataAttribs_DisplayForm(Func<int, string> _funcGetProductName)
		{
			InitializeComponent();
			m_funcGetProductName = _funcGetProductName;
			m_lastColumn = 0;
			m_sortOrder = SortOrder.Ascending;
			licenseInfoListView.SetSortColumn(m_lastColumn, m_sortOrder);
			Shared.VisualComponents.ControlHelper.SetWindowTheme(this.alertTreeView.Handle, "Explorer", null);
		}

		public void SetData(Lic_ServerDataAttribs _data)
		{
			m_data = _data;
			if (m_data != null)
			{

				lastTouchLabel.Text = m_data.lastTouchDate.TVal.ToLocalTime().ToString();
				versionLabel.Text = string.Format("{0}.{1}.{2}.{3}", m_data.versionMajor, m_data.versionMinor, m_data.versionSubMajor, m_data.versionSubMinor);

				clockTabPage.Text = String.Format(
					"Clock Violations{0}",
					(m_data.bInClockViol.TVal == true) ? " [In Violation]" : ""
					);
				inClockViolLabel.Text = m_data.bInClockViol.TVal == true ? "True" : "False";
				inClockViolLabel.ForeColor = m_data.bInClockViol.TVal == true ? Color.Red : SystemColors.WindowText;
				clockViolCountLabel.Text = m_data.clockViolCount.TVal.ToString();
				lastClockViolationLabel.Text = ((DateTime.Compare(new DateTime(1900, 1, 1), m_data.clockViolLastDate.TVal)) != 0) ? m_data.clockViolLastDate.TVal.ToLocalTime().ToString() : "Never Had Violation.";

				#region General
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
				#endregion

				#region Alert E-mails
				// The alert email root is always at index 1.
				this.alertTreeView.Nodes[1].Nodes.Clear();
				Dictionary<string, AlertTreeNode> orderedAlertTreeNodeMap = new Dictionary<string, AlertTreeNode>();
				foreach (Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_EmailAlertMailAttribs emailAlertAttrib in this.m_data.alertInfo.TVal.emailAlertsList.TVal)
					orderedAlertTreeNodeMap.Add(string.Format("{0}-{1}", emailAlertAttrib.name.TVal, emailAlertAttrib.id.TVal), new AlertTreeNode(emailAlertAttrib.name.TVal, emailAlertAttrib.id.TVal) { Tag = "1" });
				this.alertTreeView.Nodes[1].Nodes.AddRange(orderedAlertTreeNodeMap.Values.ToArray());
				this.alertTreeView.Nodes[1].Expand();

				this.alertTreeView.SelectedNode = this.alertTreeView.Nodes[0];
				#endregion

				#region Clock Violations
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
				#endregion

				#region Emulation Violations
				Lic_EmulationInfoAttribs tmpEmulationAttribs = new Lic_EmulationInfoAttribs();
				tmpEmulationAttribs.AssignMembersFromStream(m_data.streamed_EmulationInfoAttribs.TVal);
				this.emulationTabPage.Text = String.Format(
					"Emulation Violations{0}",
					(tmpEmulationAttribs.bDongleEmulatorDetected.TVal == true) ? " [In Violation]" : ""
					);
				this.emulatorInViolationLabel.Text = tmpEmulationAttribs.bDongleEmulatorDetected.TVal.ToString();
				this.emulatorInViolationLabel.ForeColor = tmpEmulationAttribs.bDongleEmulatorDetected.TVal == true ? Color.Red : SystemColors.WindowText;
				this.emulatorBypassLabel.Text = tmpEmulationAttribs.bBypassDongleEmulatorCheck.TVal.ToString();

				this.emulatorApiStatusCodeLabel.Text = tmpEmulationAttribs.checkEmulatorCall_StatusCode.TVal.ToString();
				this.emulatorApiFoundEmulatorLabel.Text = tmpEmulationAttribs.bCheckEmulatorCall_FoundEmulator.TVal.ToString();

				StringBuilder sBuilder = new StringBuilder();
				foreach (object serviceName in tmpEmulationAttribs.knownEmulatorServicesList.TVal)
				{
					if (sBuilder.Length > 0)
						sBuilder.Append("\r\n");
					sBuilder.Append(serviceName.ToString());
				}
				this.emulatorServiceTBox.Text = sBuilder.ToString();
				this.emulatorExServiceTBox.Text = tmpEmulationAttribs.excludeService.TVal.ToString();
				#endregion
			}
			this.Invalidate();
		}

		private class AlertTreeNode : TreeNode
		{
			public AlertTreeNode(string _name, string _alertId)
				: base(_name)
			{
				this.alertId = _alertId;
			}
			public string alertId { get; private set; }
		}

		private Func<int, string> m_funcGetProductName = null;
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

		private Panel lastPanel = null;
		private int lastPageIdx = -1;
		private void alertTreeView_AfterSelect(object sender, TreeViewEventArgs e)
		{
			TreeNode tNode = e.Node;
			this.SuspendLayout();
			int newPageIdx = 0;  // Defaultly use tabpage 0 if tag isn't set
			Int32.TryParse(tNode.Tag as string, out newPageIdx);
			if ((newPageIdx != lastPageIdx) &&
				(newPageIdx >= 0) &&
				(newPageIdx < this.alertTabControl.TabPages.Count) &&
				(this.alertTabControl.TabPages[newPageIdx].Controls.Count > 0) &&
				(this.alertTabControl.TabPages[newPageIdx].Controls[0] is Panel))
			{
				if (lastPageIdx != -1 && lastPanel != null)
					lastPanel.Parent = this.alertTabControl.TabPages[lastPageIdx];
				lastPanel = this.alertTabControl.TabPages[newPageIdx].Controls[0] as Panel;
				lastPanel.Parent = this.alertDisplayPanel;
				lastPageIdx = newPageIdx;
			}
			this.ResumeLayout();
			RefreshAlertUI();
		}

		private void RefreshAlertUI()
		{
			bool bDisplayMailServerSettings = this.lastPageIdx == 0;
			if (bDisplayMailServerSettings)
			{
				//this.m_data.alertInfo.TVal.mailServer.TVal.
				this.mailServerTBox.Text = this.m_data.alertInfo.TVal.mailServer.TVal.mailServerName.TVal;
				this.portTBox.Text = this.m_data.alertInfo.TVal.mailServer.TVal.portNumber.TVal.ToString();

				this.emailTBox.Text = this.m_data.alertInfo.TVal.mailServer.TVal.fromEmail.TVal;
				this.displayNameTBox.Text = this.m_data.alertInfo.TVal.mailServer.TVal.fromDisplayName.TVal;
				if (this.m_data.alertInfo.TVal.mailServer.TVal.authenticationType.TVal == Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_AlertMailServerAttribs.TAuthenticationType.ttAnonymous)
					this.authAnoyRButton.Checked = true;
				else
					this.authBasicRButton.Checked = true;
				this.authBasicUserTBox.Text = this.m_data.alertInfo.TVal.mailServer.TVal.authBasicUserName.TVal;
			}
			else
			{
				this.nameTBox.Text = string.Empty;
				this.alertIdTBox.Text = string.Empty;
				this.statusTBox.Text = string.Empty;
				this.recipentsTBox.Text = string.Empty;
				this.productsTextBox.Text = string.Empty;
				this.eventsTextBox.Text = string.Empty;

				AlertTreeNode alertNode = this.alertTreeView.SelectedNode as AlertTreeNode;
				if (alertNode != null)
				{
					Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_EmailAlertMailAttribs emailAlertAttrib = null;
					foreach (Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_EmailAlertMailAttribs tmpEmailAlertAttrib in this.m_data.alertInfo.TVal.emailAlertsList.TVal)
					{
						if (string.Equals(alertNode.alertId, tmpEmailAlertAttrib.id.TVal, StringComparison.OrdinalIgnoreCase))
						{
							emailAlertAttrib = tmpEmailAlertAttrib;
							break;
						}
					}
					if (emailAlertAttrib != null)
					{
						this.nameTBox.Text = emailAlertAttrib.name.TVal;
						this.alertIdTBox.Text = emailAlertAttrib.id.TVal;

						this.statusTBox.Text = emailAlertAttrib.bActive.TVal ? "Active" : "Deactive";

						StringBuilder recipentsSBuilder = new StringBuilder();
						foreach (string recipent in emailAlertAttrib.recipentsList.TVal)
						{
							recipentsSBuilder.AppendFormat("{0}{1}",
								((recipentsSBuilder.Length == 0) ? "" : ";"),
								recipent);
						}
						this.recipentsTBox.Text = recipentsSBuilder.ToString();

						System.Collections.Generic.SortedList<int, bool> checkedProductIdList = new System.Collections.Generic.SortedList<int, bool>();
						System.Collections.Generic.SortedList<int, bool> checkedEventIdList = new System.Collections.Generic.SortedList<int, bool>();
						System.Collections.Generic.SortedList<uint, bool> fullEventIdList = new System.Collections.Generic.SortedList<uint, bool>();
						StringBuilder displayBuilder = new StringBuilder();
						foreach (UInt32 eventId in emailAlertAttrib.eventIdList.TVal)
						{
							int partProductId = (int)eventId / 1000;
							if (partProductId == 0)
								partProductId = -1;
							int partEventId = (int)eventId % 1000;
							if (!checkedProductIdList.ContainsKey(partProductId))
								checkedProductIdList.Add(partProductId, true);
							if (!checkedEventIdList.ContainsKey(partEventId))
								checkedEventIdList.Add(partEventId, true);
							if (!fullEventIdList.ContainsKey(eventId))
								fullEventIdList.Add(eventId, true);
						}

						displayBuilder = new StringBuilder();
						foreach (uint fullEventId in fullEventIdList.Keys)
						{
							displayBuilder.AppendFormat(
								"{0}{1}",
								displayBuilder.Length == 0 ? "" : ", ",
								fullEventId);
						}
						this.eventIdTBox.Text = displayBuilder.ToString();

						List<string> productNamesList = new List<string>();
						foreach (int productId in checkedProductIdList.Keys)
							productNamesList.Add(m_funcGetProductName != null ? string.Format("{0} ({1})", m_funcGetProductName(productId), productId) : productId.ToString());
						productNamesList.Sort();

						displayBuilder = new StringBuilder();
						foreach (string productName in productNamesList)
						{
							displayBuilder.AppendFormat(
								"{0}{1}",
								displayBuilder.Length == 0 ? "" : ", ",
								productName);
						}
						this.productsTextBox.Text = displayBuilder.ToString();

						List<int> eventIdList = new List<int>();
						foreach (int eventId in checkedEventIdList.Keys)
							eventIdList.Add(eventId);
						eventIdList.Sort();

						displayBuilder = new StringBuilder();
						foreach (int eventId in eventIdList)
						{
							displayBuilder.AppendFormat(
								"{0}{1} ({2})",
								displayBuilder.Length == 0 ? "" : ", ",
								eventId,
								Solimar.Licensing.KeyMessages.GetDescription(eventId));
						}
						this.eventsTextBox.Text = displayBuilder.ToString();
					}
				}
			}
		}
	}
}

