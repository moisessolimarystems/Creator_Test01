using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Shared.VisualComponents;
namespace SolimarLicenseViewer
{
    public partial class EditEmailAlert_ProductsDialog : Shared.VisualComponents.DialogBaseForm
    {
        public class DialogData
        {
            public DialogData(
                IList<int> _checkedProductIdList,
                IList<int> _allProductIdList,
                IList<int> _licensedProductIdList
                )
            {
                this.checkedProductIdList = _checkedProductIdList;
                this.allProductIdList = _allProductIdList;
                this.licensedProductIdList = _licensedProductIdList;
            }
            public IList<int> allProductIdList { get; private set; }
            public IList<int> checkedProductIdList { get; private set; }
            public IList<int> licensedProductIdList { get; private set; }
        }
        public EditEmailAlert_ProductsDialog()
        {
            InitializeComponent();
            InitializeInternal();
            this.funcGetProductName = null;
        }
        public EditEmailAlert_ProductsDialog(Func<int, string> _funcGetProductName, ImageList _imageList)
        {
            InitializeComponent();
            InitializeInternal();
            this.funcGetProductName = _funcGetProductName;
            this.productsListView.SmallImageList = _imageList;
        }
        private void InitializeInternal()
        {
            this.productsFilterTSComboBox.Items.Clear();
            this.productsFilterTSComboBox.Items.Add("All Products");
            this.productsFilterTSComboBox.Items.Add("Licensed Products");
        }

        private const string TITLE = "Configure Solimar Products";
        private const string READONLY = " (Read Only)";
        private bool bReadOnly = false;
        private Func<int, string> funcGetProductName = null;
        private DialogData data = null;
        public void SetData(DialogData _data, bool _bReadOnly)
        {
            this.data = _data;
            this.bReadOnly = _bReadOnly;

            this.Text = string.Format("{0}{1}",
                TITLE,
                this.bReadOnly ? READONLY : ""
                );

            // this will populate this.productsListView
            productsFilterTSComboBox.SelectedIndex = 1;

            List<int> checkedList = new List<int>(this.data.checkedProductIdList);
            foreach (ListViewItem lvi in this.productsListView.Items)
                lvi.Checked = checkedList.Contains((int)lvi.Tag);

            this.productsCheckAllTSButton.Enabled = !this.bReadOnly;
            this.productsUncheckAllTSButton.Enabled = !this.bReadOnly;
            this.productsListView.Enabled = !this.bReadOnly;
        }
        public DialogData GetData()
        {
            List<int> checkedList = new List<int>();
            foreach (ListViewItem lvi in this.productsListView.Items)
            {
                if (lvi.Checked)
                    checkedList.Add((int)lvi.Tag);
            }
            return new DialogData(checkedList.ToArray(), null, null);
        }
        private void productsCheckAllTSButton_Click(object sender, EventArgs e)
        {
            foreach (ListViewItem lvi in this.productsListView.Items)
                lvi.Checked = true;
        }

        private void productsUncheckAllTSButton_Click(object sender, EventArgs e)
        {
            foreach (ListViewItem lvi in this.productsListView.Items)
                lvi.Checked = false;
        }

        private void productsFilterTSComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            try
            {
                this.productsListView.BeginUpdate();

                // Track ID of all previous checked items
                List<int> checkedLviList = new List<int>();
                foreach (ListViewItem lvi in this.productsListView.CheckedItems)
                    checkedLviList.Add((int)lvi.Tag);

                // Track ID of all previous selected items
                List<int> selectedLviList = new List<int>();
                foreach (ListViewItem lvi in this.productsListView.SelectedItems)
                    selectedLviList.Add((int)lvi.Tag);

                List<ListViewItem> lviList = new List<ListViewItem>();
                IList<int> productIdList = (this.productsFilterTSComboBox.SelectedIndex == 0) ? this.data.allProductIdList : this.data.licensedProductIdList;
                for (int idx = 0; idx < productIdList.Count; idx++)
                {
                    ListViewItem lvi = new ListViewItem();
                    lvi.Text = (this.funcGetProductName != null) ? this.funcGetProductName(productIdList[idx]) : productIdList[idx].ToString();
                    lvi.Tag = productIdList[idx];
                    lvi.Checked = checkedLviList.Contains(productIdList[idx]);
                    lvi.Selected = selectedLviList.Contains(productIdList[idx]);
                    lvi.ImageIndex = TreeViewMgr.GetIconIndex(lvi.Text);
                    
                    lviList.Add(lvi);
                }

                this.productsListView.Items.Clear();
                this.productsListView.Items.AddRange(lviList.ToArray());
                this.productsListView.Columns[0].AutoResize(ColumnHeaderAutoResizeStyle.ColumnContent);
            }
            catch (Exception ex)
            {
                HandleExceptions.DisplayException(ex);
            }
            finally
            {
                this.productsListView.EndUpdate();
            }
        }

        private void productsListView_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.A && e.Control)
            {
                foreach (ListViewItem lvi in this.productsListView.Items)
                    lvi.Selected = true;
            }
        }

        private void EditEmailAlert_ProductsDialog_Load(object sender, EventArgs e)
        {
            this.ActiveControl = this.productsListView;
        }
    }
}
