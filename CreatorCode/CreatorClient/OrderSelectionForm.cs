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
    public partial class OrderSelectionForm : Shared.VisualComponents.DialogBaseForm
    {
        private ListViewMgr _lvManager;

        public OrderSelectionForm()
        {
            InitializeComponent();
            _lvManager = new ListViewMgr();
            _lvManager.SetListViewColumnSorter(listView1);
        }

        private void OrderSelectionForm_InitDialog(object sender, Shared.VisualComponents.InitDialogEventArgs e)
        {            
            LicenseProperty licData = (e.Data as OrderSelectionData).SelectedLicense;
            Service<ICreator>.Use((client) =>
            {
                List<OrderTable> standardOrders = client.GetOrdersByLicenseName(string.Format("{0:x4}-{1:x3}-{2:x4}-S01", licData.CustID, licData.DestID, licData.GroupID));
                List<OrderTable> subOrders = client.GetOrdersByLicenseName(licData.Name);
                foreach (OrderTable order in standardOrders)
                {
                    if (order.OrderState != (byte)Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msAddOn)
                    {
                        //if order already exists apply check to checkbox and make greyed out? unselectable?
                        ListViewItem lvItem = new ListViewItem();
                        if (licData.LicType == Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltDisasterRecovery)
                        {
                            if (subOrders.Exists(c => c.OrderNumber.Equals(order.OrderNumber.Replace("S", "D"))))
                                continue;
                            //lvItem.Checked = true;
                        }
                        else if (licData.LicType == Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltTestDev)
                        {
                            if (subOrders.Exists(c => c.OrderNumber.Equals(order.OrderNumber.Replace("S", "T"))))
                            {
                                continue;
                                //lvItem.Checked = true;
                            }
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

        private void OrderSelectionForm_FinishDialog(object sender, Shared.VisualComponents.FinishDialogEventArgs e)
        {
            foreach (ListViewItem lvItem in listView1.Items)
            {
                if (lvItem.Checked)
                    (e.Data as OrderSelectionData).OrderSelectionList.Add(lvItem.Text);
            }
        }

        private void listView1_ColumnClick(object sender, ColumnClickEventArgs e)
        {
            _lvManager.SetSortIndexColumn(listView1.Handle, e.Column);
            // Perform the sort with these new sort options.
            listView1.Sort();
        }

    }
    #region OrderSelectionData class
    public class OrderSelectionData : Shared.VisualComponents.DialogData
    {
        private LicenseProperty _selectedLicense;
        private List<string> _orderSelectionList;
        #region Constructors

        public OrderSelectionData(LicenseProperty selectedLicense)
        {
            _selectedLicense = selectedLicense;
            _orderSelectionList = new List<string>();
        }
        #endregion

        #region Properties

        public LicenseProperty SelectedLicense
        {
            get { return _selectedLicense; }
            set { _selectedLicense = value; }
        }

        public List<string> OrderSelectionList
        {
            get { return _orderSelectionList; }
            set { _orderSelectionList = value; }
        }        
        #endregion
    }
    #endregion
}
