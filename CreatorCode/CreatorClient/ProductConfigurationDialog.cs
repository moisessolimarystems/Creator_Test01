using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Text.RegularExpressions;
using Client.Creator.CreatorService;
using System.Globalization;
using System.Reflection;

namespace Client.Creator
{
    public partial class ProductConfigurationDialog : Shared.VisualComponents.DialogBaseForm
    {
        #region Fields
        CommunicationLink commLink;
        #endregion

        #region Constructor
        public ProductConfigurationDialog(CommunicationLink commLink)
        {
            InitializeComponent();
            this.commLink = commLink;
            optionsTabControl.ImageList = StaticImageList.Instance.GlobalImageList;
            versionTabPage.ImageIndex = Enums.GetIconIndex("VERSIONTAB");
            tokenTabPage.ImageIndex = Enums.GetIconIndex("TOKENTAB");
            //First tab loaded is the product version tab.
            LoadProductVersionTab();

        }
        #endregion

        #region Methods

        #region ProductVersionTab Methods

        /// <summary>
        /// Remove the column header border in the Aero theme in Vista,
        /// but keep it for other themes such as standard and classic.
        /// </summary>
        static DataGridViewHeaderBorderStyle ProperColumnHeadersBorderStyle
        {
            get
            {
                return (SystemFonts.MessageBoxFont.Name == "Segoe UI") ?
                DataGridViewHeaderBorderStyle.None :
                DataGridViewHeaderBorderStyle.Raised;
            }
        }

        private void LoadProductVersionTab()
        {
            BindingList<ProductContainer> productList = new BindingList<ProductContainer>();
            InitializeComponent();
            //load property grid with product items
            Client.Creator.ServiceProxy.Service<ICreator>.Use((client) =>
            {
                LicenseVersion version;
                string hexVersion, minorVersion, majorVersion;
                IList<ProductTable> products = client.GetProducts();
                foreach (ProductTable product in products)
                {
                    if (!product.pName.Contains("Test") && commLink.GetProductName((byte)product.pId) != "Unknown")
                    {
                        hexVersion = string.Format("{0:x}", product.pVersion);
                        minorVersion = hexVersion.Substring(hexVersion.Length - 3, 2);
                        majorVersion = hexVersion.Substring(0, hexVersion.Length - (minorVersion.Length + 1));
                        version = new LicenseVersion(UInt32.Parse(majorVersion), UInt32.Parse(minorVersion));
                        productList.Add(new ProductContainer(product.pName, version, StaticImageList.Instance.GlobalImageList.Images[Enums.GetIconIndex(commLink.GetProductName((byte)product.pId))]));
                    }
                }
            });            
            productVersionDataGridView.DataSource = productList;
            productVersionDataGridView.Columns[0].AutoSizeMode = DataGridViewAutoSizeColumnMode.AllCellsExceptHeader;
            productVersionDataGridView.Columns[0].HeaderText = string.Empty;
            productVersionDataGridView.ColumnHeadersBorderStyle = ProperColumnHeadersBorderStyle;
        }

        #endregion

        #region SoftwareTokenTab Methods

        private void LoadSoftwareTokensTab()
        {
            availableListView.SmallImageList = StaticImageList.Instance.GlobalImageList;
            selectedListView.SmallImageList = StaticImageList.Instance.GlobalImageList;
            IList<SoftwareTokenTable> swTokens = null;
            //load property grid with product items
            Client.Creator.ServiceProxy.Service<ICreator>.Use((client) =>
            {
                swTokens = client.GetAllSoftwareTokens();
            });
            foreach (SoftwareTokenTable swt in swTokens)
            {                
                ListViewItem lvItem = new ListViewItem();                
                lvItem.Name = lvItem.Text = swt.TokenType;
                int imageIndex = Enums.GetIconIndex(swt.TokenType);
                if(imageIndex > 0) lvItem.ImageIndex = imageIndex;
                if (swt.Status > 0)
                    selectedListView.Items.Add(lvItem);
                else
                    availableListView.Items.Add(lvItem);
                lvItem.Checked = swt.Status > 0;
            }
            EnableButtons();
        }

        private void EnableButtons()
        {
            addButton.Enabled = availableListView.SelectedItems.Count > 0;
            addAllButton.Enabled = availableListView.Items.Count > 0;
            removeButton.Enabled = selectedListView.SelectedItems.Count > 0;
            removeAllButton.Enabled = selectedListView.Items.Count > 0;
        }

        private void SaveSoftwareTokens()
        {
            byte status;
            ServiceProxy.Service<ICreator>.Use((client) =>
            {
                List<SoftwareTokenTable> dbTokens = client.GetAllSoftwareTokens();
                foreach (SoftwareTokenTable token in dbTokens)
                {
                    status = (byte)(selectedListView.Items.ContainsKey(token.TokenType) ? 1 : 0);
                    if (token.Status != status)
                    {
                        token.Status = status;
                        client.UpdateSoftwareTokenTable(token);
                    }
                }
            });
        }
        #endregion

        #endregion

        #region Events

        #region Form Events
        private void btnOk_Click(object sender, EventArgs e)
        {
            if (optionsTabControl.SelectedTab == versionTabPage)
            { }
            else
            {
                SaveSoftwareTokens();
            }
        }
        #endregion

        #region OptionsTabControl Events
        private void optionsTabControl_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (optionsTabControl.SelectedTab == versionTabPage)
            {
                LoadProductVersionTab();
            }
            else
            {
                LoadSoftwareTokensTab();
            }
        }
        #endregion

        #region ProductVersionDataGridView Events
        private void productVersionDataGridView_DataError(object sender, DataGridViewDataErrorEventArgs e)
        {
            if (e.Exception != null)
            {                
                MessageBox.Show(e.Exception.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                e.ThrowException = false;
                e.Cancel = true;
                DataGridView dgv = (DataGridView)sender;
                dgv.EditingControl.Text = dgv.CurrentCell.Value.ToString();  
             }
        }
        #endregion

        #region SoftwareTokenTab View Events
        private void addButton_Click(object sender, EventArgs e)
        {
            foreach (ListViewItem lvItem in availableListView.SelectedItems)
            {
                ListViewItem newItem = lvItem.Clone() as ListViewItem;
                newItem.Name = newItem.Text;
                selectedListView.Items.Add(newItem);
                availableListView.Items.Remove(lvItem);
            }
            EnableButtons();
        }

        private void addAllButton_Click(object sender, EventArgs e)
        {
            foreach (ListViewItem lvItem in availableListView.Items)
            {
                ListViewItem newItem = lvItem.Clone() as ListViewItem;
                newItem.Name = newItem.Text;
                selectedListView.Items.Add(newItem);
            }
            availableListView.Items.Clear();
            EnableButtons();
        }

        private void removeButton_Click(object sender, EventArgs e)
        {
            foreach (ListViewItem lvItem in selectedListView.SelectedItems)
            {
                ListViewItem newItem = lvItem.Clone() as ListViewItem;
                newItem.Name = newItem.Text;
                availableListView.Items.Add(newItem);
                selectedListView.Items.Remove(lvItem);
            }
            EnableButtons();
        }

        private void removeAllButton_Click(object sender, EventArgs e)
        {
            foreach (ListViewItem lvItem in selectedListView.Items)
            {
                ListViewItem newItem = lvItem.Clone() as ListViewItem;
                newItem.Name = newItem.Text;
                availableListView.Items.Add(newItem);
            }
            selectedListView.Items.Clear();
            EnableButtons();
        }

        private void availableListView_SelectedIndexChanged(object sender, EventArgs e)
        {
            EnableButtons();
        }

        private void selectedListView_SelectedIndexChanged(object sender, EventArgs e)
        {
            EnableButtons();
        }
        #endregion

        #endregion
    }

    class ProductContainer : INotifyPropertyChanged
    {
        string product;
        LicenseVersion version;
        Image image;

        public event PropertyChangedEventHandler PropertyChanged;

        public ProductContainer(string product, LicenseVersion version, Image image)
        {
            this.product = product;
            this.version = version;
            this.image = image;
        }

        public Image Image
        {
            get { return image; }
        }
        public string Product
        {
            get { return product; }             
        }

        public LicenseVersion Version
        {
            get { return version; }
            set 
            {
                string strVersion = (value.Minor > 9) ? string.Format("{0}{1}0", value.Major, value.Minor) : string.Format("{0}0{1}0", value.Major, value.Minor);
                int newVersion = Int32.Parse(strVersion, System.Globalization.NumberStyles.HexNumber);
                ServiceProxy.Service<ICreator>.Use((client) =>
                {
                    ProductTable productRec = client.GetProduct(product);
                    if (!productRec.pVersion.Equals(newVersion))
                    {
                        productRec.pVersion = newVersion;
                        client.UpdateProductTable(productRec);
                    }
                });
                //set database
                version = value;
                this.NotifyPropertyChanged("Version");
            }
        }

        private void NotifyPropertyChanged(string name)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(name));
        }
    }
}
