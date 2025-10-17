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
   public partial class Lic_ProductConnectionAttribs_DisplayForm : Base_DisplayForm
   {
      public Lic_ProductConnectionAttribs_DisplayForm()
      {
         InitializeComponent();
         Initialize();
      }
      public void Initialize()
      {
         Shared.VisualComponents.ControlHelper.SetWindowTheme(this.productListView.Handle, "Explorer", null);
      }

      public void SetData(Lic_ClientSettingsAttribs _data)
      {
         int installedProducts = 0;
         try
         {
            Solimar.Licensing.GlobalSoftwareSpec globalSoftwareSpec = new Solimar.Licensing.GlobalSoftwareSpec();
            System.Collections.Generic.SortedList<string, ListViewItem> newList = new System.Collections.Generic.SortedList<string, ListViewItem>();
            foreach (Lic_ClientSettingsAttribs.Lic_ProductConnectionAttribs prodConnAttribs in _data.prodConnList.TVal)
            {
               ListViewItem lvi = new ListViewItem();
               lvi.Text = string.Format("{0} ({1})",
                  Solimar.Licensing.Attribs.Lic_LicenseInfoAttribsHelper.GetProductName(globalSoftwareSpec.softwareSpec, prodConnAttribs.productID.TVal),
                  prodConnAttribs.productID.TVal
               );
               lvi.SubItems.Add(prodConnAttribs.serverName.TVal);
               lvi.SubItems.Add(prodConnAttribs.backupServerName.TVal);
               lvi.SubItems.Add(prodConnAttribs.bTestDev.TVal ? "True" : "False");
               lvi.SubItems.Add("Status Unknown");
               lvi.SubItems.Add(prodConnAttribs.bInstalled.TVal ? "True" : "False");
               if (prodConnAttribs.bInstalled.TVal)
                  installedProducts++;
               lvi.ForeColor = prodConnAttribs.bInstalled.TVal ? System.Drawing.SystemColors.WindowText : System.Drawing.Color.Red;
               lvi.ToolTipText = prodConnAttribs.bInstalled.TVal ? "Installed" : "Not Installed";
               newList.Add(lvi.Text, lvi);
            }
            this.productListView.Items.Clear();
            this.productListView.Items.AddRange(newList.Values.ToArray());
         }
         catch (Exception)
         {
         }
         finally
         {
            this.productsTSStatusLabel.Text = string.Format(
               "{0} Item{1}",
               this.productListView.Items.Count,
               this.productListView.Items.Count == 1 ? "" : "s"
               );

            this.installedTSStatusLabel.Text = string.Format(
               "{0} Installed Item{1}",
               installedProducts,
               installedProducts == 1 ? "" : "s"
               );
         }
      }
   }
}
