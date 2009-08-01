using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Client.Creator.CreatorService;
using Solimar.Licensing.Attribs;
using Client.Creator.ServiceProxy;

namespace Client.Creator
{
    public partial class ProductLicenseSelectionForm : Shared.VisualComponents.DialogBaseForm
    {
        private ListViewMgr _lvManager;

        public ProductLicenseSelectionForm()
        {
            InitializeComponent();
            _lvManager = new ListViewMgr();
            _lvManager.SetListViewColumnSorter(listView1);
        }

        private void ProductLicenseSelectionForm_InitDialog(object sender, Shared.VisualComponents.InitDialogEventArgs e)
        {
            LicenseServerProperty licData = (e.Data as ProductLicenseSelectionData).SelectedLicense;
            Service<ICreator>.Use((client) =>
            {
                string licenseBase = "P";
                List<OrderTable> standardOrders = client.GetOrdersByLicenseName(string.Format("{0:x4}-{1:x3}-{2:x4}-{3}01", licData.CustID, licData.DestID, licData.GroupID, licenseBase));
                if (standardOrders.Count == 0) //subscription type
                {
                    licenseBase = "S";
                    standardOrders = client.GetOrdersByLicenseName(string.Format("{0:x4}-{1:x3}-{2:x4}-{3}01", licData.CustID, licData.DestID, licData.GroupID, licenseBase));
                }
                List<OrderTable> subOrders = client.GetOrdersByLicenseName(licData.Name);
                foreach (OrderTable order in standardOrders)
                {
                    if (order.OrderState != (byte)Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msAddOn)
                    {
                        //if order already exists apply check to checkbox and make greyed out? unselectable?
                        ListViewItem lvItem = new ListViewItem();
                        if (licData.LicType == Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltDisasterRecovery)
                        {
                            if (subOrders.Exists(c => c.OrderNumber.Equals(order.OrderNumber.Replace(licenseBase, "D"))))
                                continue;
                        }
                        else if (licData.LicType == Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltTestDev)
                        {
                            if (subOrders.Exists(c => c.OrderNumber.Equals(order.OrderNumber.Replace(licenseBase, "T"))))
                                continue;                            
                        }
                        lvItem.Text = order.OrderNumber;
                        lvItem.SubItems.Add(Enum.GetName(typeof(Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState), order.OrderState));
                        lvItem.SubItems.Add(order.ProductName);
                        if (order.ExpirationDate.HasValue)
                            lvItem.SubItems.Add(order.ExpirationDate.Value.ToShortDateString());
                        else
                            lvItem.SubItems.Add("");
                        lvItem.SubItems.Add(order.Description);
                        listView1.Items.Add(lvItem);
                    }
                }
                _lvManager.AutoResizeColumns(listView1);
            });
        }

        private void ProductLicenseSelectionForm_FinishDialog(object sender, Shared.VisualComponents.FinishDialogEventArgs e)
        {
            foreach (ListViewItem lvItem in listView1.Items)
            {
                if (lvItem.Checked)
                    (e.Data as ProductLicenseSelectionData).ProductLicenseSelectionList.Add(lvItem.Text);
            }
        }

        private void listView1_ColumnClick(object sender, ColumnClickEventArgs e)
        {
            _lvManager.SetSortIndexColumn(listView1.Handle, e.Column);
            // Perform the sort with these new sort options.
            listView1.Sort();
        }

    }
    #region ProductLicenseSelectionData class
    public class ProductLicenseSelectionData : Shared.VisualComponents.DialogData
    {
        private LicenseServerProperty _selectedLicense;
        private List<string> _productLicenseSelectionList;
        #region Constructors

        public ProductLicenseSelectionData(LicenseServerProperty selectedLicense)
        {
            _selectedLicense = selectedLicense;
            _productLicenseSelectionList = new List<string>();
        }
        #endregion

        #region Properties

        public LicenseServerProperty SelectedLicense
        {
            get { return _selectedLicense; }
            set { _selectedLicense = value; }
        }

        public List<string> ProductLicenseSelectionList
        {
            get { return _productLicenseSelectionList; }
            set { _productLicenseSelectionList = value; }
        }        
        #endregion
    }
    #endregion
}
