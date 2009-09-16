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
        private CommunicationLink _commLink;
        public ProductLicenseSelectionForm(CommunicationLink commLink)
        {
            InitializeComponent();
            _lvManager = new ListViewMgr();
            _lvManager.SetListViewColumnSorter(listView1);
            _commLink = commLink;
        }

        private void ProductLicenseSelectionForm_InitDialog(object sender, Shared.VisualComponents.InitDialogEventArgs e)
        {
            LicenseServerProperty licData = (e.Data as ProductLicenseSelectionData).SelectedLicense;
            Service<ICreator>.Use((client) =>
            {
                string licenseBase = "P";
                List<ProductLicenseTable> productLicenses = client.GetProductLicenses(string.Format("{0:x4}-{1:x3}-{2:x4}-{3}01", licData.CustID, licData.DestID, licData.GroupID, licenseBase));
                if (productLicenses.Count == 0) //subscription type
                {
                    licenseBase = "S";
                    productLicenses = client.GetProductLicenses(string.Format("{0:x4}-{1:x3}-{2:x4}-{3}01", licData.CustID, licData.DestID, licData.GroupID, licenseBase));
                }
                List<ProductLicenseTable> subProductLicenses = client.GetProductLicenses(licData.Name);
                foreach (ProductLicenseTable plRecord in productLicenses)
                {
                    if (plRecord.plState != (byte)ProductLicenseState.AddOn)
                    {
                        //if order already exists apply check to checkbox and make greyed out? unselectable?
                        ListViewItem lvItem = new ListViewItem();
                        if (licData.LicType == LicenseServerType.DisasterRecovery) //Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltDisasterRecovery)
                        {
                            if (subProductLicenses.Exists(c => c.plID.Equals(plRecord.plID.Replace(licenseBase, "D"))))
                                continue;
                        }
                        else if (licData.LicType == LicenseServerType.TestDevelopment) //Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltTestDev)
                        {
                            if (subProductLicenses.Exists(c => c.plID.Equals(plRecord.plID.Replace(licenseBase, "T"))))
                                continue;
                        }
                        lvItem.Text = plRecord.plID;
                        lvItem.SubItems.Add(Enum.GetName(typeof(ProductLicenseState), plRecord.plState));
                        lvItem.SubItems.Add(_commLink.GetProductName((uint)plRecord.ProductID));
                        if (plRecord.ExpirationDate.HasValue)
                            lvItem.SubItems.Add(plRecord.ExpirationDate.Value.ToLocalTime().ToShortDateString());
                        else
                            lvItem.SubItems.Add("");
                        lvItem.SubItems.Add(plRecord.Description);
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
