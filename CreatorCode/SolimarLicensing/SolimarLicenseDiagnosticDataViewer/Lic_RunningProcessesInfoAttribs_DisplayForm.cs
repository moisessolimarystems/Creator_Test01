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
   public partial class Lic_RunningProcessesInfoAttribs_DisplayForm : Base_DisplayForm
   {
      public Lic_RunningProcessesInfoAttribs_DisplayForm()
      {
         InitializeComponent();
         Initialize();
      }
      public void Initialize()
      {
         Shared.VisualComponents.ControlHelper.SetWindowTheme(this.servicesListView.Handle, "Explorer", null);
			//this.highlightKnownEnumationTSButton.Checked = true;
         this.runningTSComboBox.SelectedIndex = 0;
			this.serviceTypeTSComboBox.SelectedIndex = 0;
			this.knownEmulatorHash.Add("xlrnbo", true);			
      }

      public void SetData(Lic_RunningProcessesInfoAttribs _data)
      {
         m_data = _data;
         RefreshUI();
         servicesListView_SelectedIndexChanged(null, null);
      }

		[FlagsAttribute]
		private enum EServiceInforType:int
		{
			None = 0,
			Service = 1,
			SystemDriver = 2,
		};
		private const string ServerTypeFileSystemDriver = "File System Driver";
		private const string ServerTypeKernelDriver = "Kernel Driver";
		private const string ServerTypeOwnProcess = "Own Process";
		private const string ServerTypeShareProcess = "Share Process";
		private SortedDictionary<string, bool> knownEmulatorHash = new SortedDictionary<string, bool>();
					
      private void RefreshUI()
      {
         if (m_data != null)
         {
				int emulatorCount = 0;
            try
            {
               bool bRunningStateShowAll = (this.runningTSComboBox.SelectedIndex == 0);
					EServiceInforType sType = EServiceInforType.None;
					if (this.serviceTypeTSComboBox.SelectedIndex == 1)
						sType = EServiceInforType.Service;
					else if (this.serviceTypeTSComboBox.SelectedIndex == 2)
						sType = EServiceInforType.SystemDriver;
					else
						sType = EServiceInforType.Service | EServiceInforType.SystemDriver;

               this.servicesListView.BeginUpdate();
               this.servicesListView.Items.Clear();
					
               List<ListViewItem> lviList = new List<ListViewItem>();
               foreach (Lic_RunningProcessesInfoAttribs.Lic_ServicesInfoAttribs serviceInfo in m_data.servicesList.TVal)
               {
						bool bPassRunningStateFilter = bRunningStateShowAll || (!bRunningStateShowAll && (serviceInfo.bStarted.TVal == true));
						bool bPassServiceTypeService = string.Equals(serviceInfo.serviceType.TVal, ServerTypeOwnProcess, StringComparison.OrdinalIgnoreCase) || string.Equals(serviceInfo.serviceType.TVal, ServerTypeShareProcess, StringComparison.OrdinalIgnoreCase);
						bool bPassServiceTypeSystemDriver = string.Equals(serviceInfo.serviceType.TVal, ServerTypeFileSystemDriver, StringComparison.OrdinalIgnoreCase) || string.Equals(serviceInfo.serviceType.TVal, ServerTypeKernelDriver, StringComparison.OrdinalIgnoreCase);
						bool bPassServiceTypeFiler = (((sType & EServiceInforType.Service) != 0) && (bPassServiceTypeService == true)) || (((sType & EServiceInforType.SystemDriver) != 0) && (bPassServiceTypeSystemDriver == true));
						
						if (bPassRunningStateFilter && bPassServiceTypeFiler)
                  {
                     ListViewItem lvi = new ListViewItem();
                     lvi.Text = serviceInfo.name.TVal;
                     lvi.Tag = serviceInfo;
                     lvi.SubItems.Add(serviceInfo.bStarted.TVal == true ? "TRUE" : "FALSE");
							lvi.SubItems.Add(serviceInfo.serviceType.TVal);
                     lvi.SubItems.Add(serviceInfo.path.TVal);
                     lvi.SubItems.Add(serviceInfo.display.TVal);
                     lvi.SubItems.Add(serviceInfo.description.TVal);
                     lviList.Add(lvi);

							if (this.knownEmulatorHash.ContainsKey(serviceInfo.name.TVal.ToLower()))
								emulatorCount++;
                  }
               }
               this.servicesListView.Items.AddRange(lviList.ToArray());
            }
            catch (Exception)
            {
            }
            finally
            {
               this.servicesListView.EndUpdate();
               this.itemsTSStatusLabel.Text = string.Format("{0} Items", this.servicesListView.Items.Count);
					this.emulatorTlSStatusLabel.Text = string.Format("{0} Emulators", emulatorCount);
            }
         }
      }

      private Lic_RunningProcessesInfoAttribs m_data;

      private void filterTSComboBox_SelectedIndexChanged(object sender, EventArgs e)
      {
         RefreshUI();
      }
		private void serviceTypeTSComboBox_SelectedIndexChanged(object sender, EventArgs e)
		{
			RefreshUI();
		}
		private void highlightKnownEnumationTSButton_Click(object sender, EventArgs e)
		{
			foreach (ListViewItem lvi in this.servicesListView.Items)
			{
				bool bItemIsAnEmulator = this.knownEmulatorHash.ContainsKey(lvi.Text.ToLower());
				lvi.ForeColor = (bItemIsAnEmulator && this.highlightKnownEnumationTSButton.Checked) ? Color.Red : SystemColors.WindowText;
				lvi.BackColor = (bItemIsAnEmulator && this.highlightKnownEnumationTSButton.Checked) ? SystemColors.Info : SystemColors.Window;
				if (bItemIsAnEmulator && this.highlightKnownEnumationTSButton.Checked)
					lvi.EnsureVisible();
			}
		}

      private void servicesListView_SelectedIndexChanged(object sender, EventArgs e)
      {
         this.nameTBox.Text = "";
         this.runningTBox.Text = "";
         this.displayTBox.Text = "";
         this.pathTBox.Text = "";
         this.descriptionTBox.Text = "";
			this.serverTypeTBox.Text = "";
			this.infoTabPage.Text = "Service Type:";

         if (this.servicesListView.SelectedItems.Count > 0)
         {
            Lic_RunningProcessesInfoAttribs.Lic_ServicesInfoAttribs serviceInfo = this.servicesListView.SelectedItems[0].Tag as Lic_RunningProcessesInfoAttribs.Lic_ServicesInfoAttribs;
            if (serviceInfo != null)
            {
               this.nameTBox.Text = serviceInfo.name.TVal;
               this.runningTBox.Text = serviceInfo.bStarted.TVal ? "TRUE" : "FALSE";
               this.displayTBox.Text = serviceInfo.display.TVal;
               this.pathTBox.Text = serviceInfo.path.TVal;
               this.descriptionTBox.Text = serviceInfo.description.TVal;
					this.serverTypeTBox.Text = serviceInfo.serviceType.TVal;

					bool bPassServiceTypeService = string.Equals(serviceInfo.serviceType.TVal, ServerTypeOwnProcess, StringComparison.OrdinalIgnoreCase) || string.Equals(serviceInfo.serviceType.TVal, ServerTypeShareProcess, StringComparison.OrdinalIgnoreCase);
					bool bPassServiceTypeSystemDriver = string.Equals(serviceInfo.serviceType.TVal, ServerTypeFileSystemDriver, StringComparison.OrdinalIgnoreCase) || string.Equals(serviceInfo.serviceType.TVal, ServerTypeKernelDriver, StringComparison.OrdinalIgnoreCase);
					if (bPassServiceTypeService)
						this.infoTabPage.Text = "Service Type: 'Service'";
					else if (bPassServiceTypeSystemDriver)
						this.infoTabPage.Text = "Service Type: 'System Driver'";
            }
         }
			//highlightKnownEnumationTSButton_Click(null, null);
      }

      private void general_KeyDown(object sender, KeyEventArgs e)
      {
         if (e.KeyCode == Keys.C && e.Control)
         {
            System.Windows.Forms.Clipboard.SetText(Shared.VisualComponents.ListViewHelper.GetCopyTextForListView(this.servicesListView));
            e.Handled = true;
         }
      }
   }
}
