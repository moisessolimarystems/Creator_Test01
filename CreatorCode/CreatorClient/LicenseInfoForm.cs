using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Reflection;
using System.Runtime.InteropServices;
using System.IO;
using Solimar.Licensing;
using Solimar.Licensing.LicenseManagerWrapper;
using Solimar.Licensing.Attribs;
using Client.Creator.CreatorService;
using Client.Creator.ServiceProxy;
namespace Client.Creator
{
    public partial class LicenseInfoForm : Shared.VisualComponents.DialogBaseForm
    {
        private Object selectedObject;
        private CommunicationLink m_CommLink;
        private bool m_Validated;

        #region Properties
        public DateTime CurrentExpirationDate
        {
            get { return new DateTime(DateTime.Today.Year, DateTime.Today.Month, DateTime.Today.Day, 10, 0, 0); }
        }
        #endregion

        #region Form Events
        public LicenseInfoForm(string title, ref CommunicationLink commLink)
        {
            m_Validated = true;
            InitializeComponent();
            this.Text = title;
            licInfoTabControl.TabPages.Clear();
            m_CommLink = commLink;
        }

        private void LicenseInfoForm_InitDialog(object sender, Shared.VisualComponents.InitDialogEventArgs e)
        {
            if (e.Data is LicenseDialogData)
                LoadLicenseTabPage(e.Data as LicenseDialogData);
            else if (e.Data is TokenDialogData)
                LoadTokenTabPage(e.Data as TokenDialogData);
            else if (e.Data is PacketDialogData)
                LoadPacketTabPage(e.Data as PacketDialogData);
            else if (e.Data is ProductLicenseDialogData)
                LoadProductLicenseTabPage(e.Data as ProductLicenseDialogData);         
        }

        private void LicenseInfoForm_FinishDialog(object sender, Shared.VisualComponents.FinishDialogEventArgs e)
        { 
            if (e.Result != DialogResult.OK)
                return;

            if (e.Data is LicenseDialogData)
                SaveLicenseTabPage(e.Data as LicenseDialogData);
            else if (e.Data is TokenDialogData)
                SaveTokenTabPage(e.Data as TokenDialogData);
            else if (e.Data is PacketDialogData)
                SavePacketTabPage(e.Data as PacketDialogData);
            else if (e.Data is ProductLicenseDialogData)
                SaveProductLicenseTabPage(e.Data as ProductLicenseDialogData);
        }

        private void btnOk_Click(object sender, EventArgs e)
        {
            if (tokenGroupBox.Parent == topPanel)
                ValidateTokenForm();
        }

        private void LicenseInfoForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (DialogResult == DialogResult.OK)
                if (!m_Validated)
                    e.Cancel = true;
        }
        #endregion

        #region Product License Methods

        //needs next product license name, license server name
        void LoadProductLicenseTabPage(ProductLicenseDialogData plData)
        {
            selectedObject = plData as Object;
            topPanel.Controls.Clear();
            productLicenseGroupBox.Parent = topPanel;
            productLicenseNumberTextBox.Text = plData.ProductLicense.plID;
            List<ProductLicenseTable> productLicenses;
            List<string> productList = new List<string>();
            Service<ICreator>.Use((client) => 
            {
                foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in m_CommLink.m_softwareSpec.productSpecMap.TVal.Values)
                {
                    bool bSkip = false;
                    productLicenses = client.GetProductLicensesByProduct(plData.LicenseServerID, (byte)productSpec.productID.TVal);
                    foreach (ProductLicenseTable pl in productLicenses)
                    {   //skip if client and not perm
                        if (pl.plState.Equals((byte)ProductLicenseState.Trial) &&
                           productSpec.productLicType.TVal.Equals(Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.TProductLicenseType.pltClient))
                            bSkip = true;
                    }
                    if (!bSkip)
                        productList.Add(productSpec.productName);
                }
            });
            if (productList.Count == 0)
                productList.Add("No Products Available");
            ProductLicenseProductComboBox.Items.AddRange(productList.ToArray());
            ProductLicenseProductComboBox.SelectedIndex = 0;
            expDateTextBox.Text = "30";
        }

        void SaveProductLicenseTabPage(ProductLicenseDialogData plData)
        {
            string[] splitStatus = ProductLicenseTypeComboBox.SelectedItem.ToString().Split(":".ToCharArray());
            //can only be one or two
            if (splitStatus.Count() > 1)
            {
                ProductLicenseState state = (ProductLicenseState)Enum.Parse(typeof(ProductLicenseState), splitStatus[0].Trim());
                plData.ProductLicense.plState = (byte)state;
                plData.ProductLicense.ParentProductLicenseID = splitStatus[1].Trim();
            }
            else
            {
                ProductLicenseState state = (ProductLicenseState)Enum.Parse(typeof(ProductLicenseState), splitStatus[0].Trim());
                plData.ProductLicense.plState = (byte)state;
            }
            plData.ProductLicense.ProductID = (byte)m_CommLink.GetProductID(ProductLicenseProductComboBox.SelectedItem.ToString());
            plData.ProductLicense.ProductConnection = (byte)((plData.ProductLicense.plState == (byte)ProductLicenseState.AddOn) ? 0 : 1);
            plData.ProductLicense.ProductVersion = productLicenseVersionMaskedTextBox.Text;
            plData.ProductLicense.ExpirationDate = CurrentExpirationDate.AddDays(Int32.Parse(expDateTextBox.Text)).ToUniversalTime();
            plData.ProductLicense.Description = productLicenseDescriptionTextBox.Text;
        }
        #endregion

        #region License Methods

        private void LoadLicenseTabPage(LicenseDialogData licData)
        {
            topPanel.Controls.Clear();
            licenseGroupBox.Parent = topPanel;
            //need to get next available value from DB for destination ID and group ID.
            selectedObject = licData.LicInfo as Object;         
            destinationTextBox.Text = licData.LicInfo.DestName;
            //license name
            licNameTextBox.Text = licData.LicInfo.Name;
        }

        private void SaveLicenseTabPage(LicenseDialogData licData)
        {
            //TODO : validate that the license does not already exist
            licData.LicInfo.Comments = licDescriptTextBox.Text;
        }
        #endregion

        #region Token Methods
        //needs to support SOFTWARE/HARDWARE validation
        private void LoadTokenTabPage(TokenDialogData tokenData)
        {
            selectedObject = tokenData.LicInfo as Object;
            ////hardware token get value from database
            btnOk.Enabled = true;
            topPanel.Controls.Clear();
            tokenGroupBox.Parent = topPanel;
            hardwareRadioButton.Checked = true;
        }

        private Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType GetTokenEnum(string tokenName)        
        {
            Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType token = Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttNone;
            switch (tokenName)
            {
                case "Computer Name":
                    token = Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttComputerName;
                    break;
                case "Mac Address":
                    token = Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttMacAddress;
                    break;
                case "Bios Serial Number":
                    token = Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttBiosSerialNumber;
                    break;
                case "Domain Name":
                    token = Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttDomainName;
                    break;
                case "Operating System":
                    token = Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttOperatingSystem;
                    break;
                case "Part of Domain":
                    token = Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttPartOfDomain;
                    break;
                case "System Manufacturer":
                    token = Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttSystemManufacturer;
                    break;
                case "System Model":
                    token = Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttSystemModel;
                    break;
                case "System Type":
                    token = Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttSystemType;
                    break;
                case "System Product UUID":
                    token = Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttSystemUuid;
                    break;
                default: break;
            }
            return token;
        }

        private void SaveTokenTabPage(TokenDialogData tokenData)
        {
            Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs newToken;            
            if (hardwareRadioButton.Checked)
            {
                newToken = new Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs();
                newToken.tokenType.TVal =  Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID;
                newToken.tokenValue.TVal = tokenListView.SelectedItems[0].Text;
                tokenData.TokenList.Add(newToken);
            }
            else
            {
                foreach (ListViewItem lvItem in tokenListView.Items)
                {
                    newToken = new Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs();
                    newToken.tokenType.TVal = GetTokenEnum(lvItem.Text);  
                    newToken.tokenValue.TVal = lvItem.SubItems[1].Text;
                    tokenData.TokenList.Add(newToken);               
                }
            }
        }

        private void ValidateTokenForm()
        {
            Cursor.Current = Cursors.WaitCursor;
            m_Validated = true;
            Service<ICreator>.Use((client) =>
            {
                LicenseServerProperty selectedLicense = selectedObject as LicenseServerProperty;
                Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType selectedType;
                if (hardwareRadioButton.Checked)
                {
                    selectedType = Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID;
                    if (client.TokenExists(selectedLicense.CustID, (byte)selectedType, tokenListView.SelectedItems[0].Text))
                    {
                        m_Validated = false;
                        // Set the ErrorProvider error with the text to display.
                        //errorProvider1.SetError(this.tokenValueTextBox, "Validation token already exists for this customer!");
                        MessageBox.Show(tokenListView.SelectedItems[0].Text + " already exists for this customer!", "Validation Token Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }  
                }
                else
                {
                    IList<SoftwareTokenTable> tokenList = client.GetAllSoftwareTokens();
                    if (tokenList != null) //ensure all active tokens are available from customer
                    {
                        foreach (SoftwareTokenTable token in tokenList.Where(t => t.Status > 0))
                        {
                            if (token.TokenType != Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID.ToString())
                            {   
                                if (!(tokenListView.Items.Find(token.TokenType, false).Count() > 0))
                                {
                                    m_Validated = false;
                                    MessageBox.Show(token.TokenType + " is missing from customer provided validation tokens!", "Validation Token Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                    break;
                                }
                            }
                        }
                    }
                    if (m_Validated) //ensure all tokens have a value
                    {
                        foreach (ListViewItem lvItem in tokenListView.Items)
                        {
                            if (!(lvItem.SubItems[1].Text.Length > 0))
                            {
                                m_Validated = false;
                                MessageBox.Show(lvItem.Text + " has an empty value!", "Validation Token Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                break;
                            }
                        }
                    }
                    if (m_Validated) //ensure all token sets are unique
                    {
                        ListViewItem[] foundTokens;
                        string tokenName, tokenValue;
                        CustomerTable customerDB = client.GetCustomer(selectedLicense.CustID.ToString(), false);
                        if (customerDB != null)
                        {
                            IList<LicenseTable> licServerList = client.GetLicensesByCustomer(customerDB.SCRname);
                            IList<TokenTable> tokenDBList = client.GetTokensByCustomerID(customerDB.SCRnumber);
                            foreach (LicenseTable licServer in licServerList)
                            {
                                bool bMatch = true;
                                //all tokens must match in order to pass validation
                                foreach (TokenTable token in tokenDBList.Where(t => t.LicenseID == licServer.ID))
                                {
                                    tokenName = ((Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType)token.TokenType).ToString();
                                    foundTokens = tokenListView.Items.Find(tokenName, false);
                                    if(foundTokens.Count() > 0)
                                    {
                                        tokenValue = foundTokens[0].SubItems[1].Text;
                                        if(token.TokenValue != tokenValue)
                                        {    
                                            bMatch = false;
                                            break;
                                        }
                                    }
                                }
                                if (licServer.LicenseName != selectedLicense.Name)
                                {
                                    if (bMatch)
                                    {
                                        m_Validated = false;
                                        MessageBox.Show(string.Format("Validation tokens are not unique for {0}!\nLicense Server - {1} has the same validation tokens.", customerDB.SCRname, licServer.LicenseName), "Validation Token Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                        //token is already in LS - A
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            });
            Cursor.Current = Cursors.Default;
        }

        private bool IsValidTokenType(string tokenType)
        {
            if (GetTokenEnum(tokenType) != Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttNone)
                return true;
            return false;
        }
        #endregion

        #region Packet Methods
        private void LoadPacketTabPage(PacketDialogData packetData)
        {
            topPanel.Controls.Clear();
            packetGroupBox.Parent = topPanel;
            selectedObject = packetData as Object;
            packetNameTextBox.Text = packetData.PacketName;
            packetExpDateTextBox.Text = DateTime.Now.AddDays(14).ToLongDateString();
            packetOutputPathTextBox.Text = packetData.SelectedDirectory;           
        }
        private void SavePacketTabPage(PacketDialogData packetData)
        {
            packetData.ExpDate = DateTime.Parse(packetExpDateTextBox.Text); 
            packetData.SelectedDirectory = packetOutputPathTextBox.Text;
            if (!Directory.Exists(packetData.SelectedDirectory))            
                Directory.CreateDirectory(packetData.SelectedDirectory);            
            packetData.UserNotes = packetDescriptTextBox.Text;
        }
        #endregion

        #region PacketTabPage Events

        private void folderBrowseButton_Click(object sender, EventArgs e)
        {
            if (folderBrowserDialog.ShowDialog(this) == DialogResult.OK)
            {
                PacketDialogData selectedPacketData = selectedObject as PacketDialogData;
                packetOutputPathTextBox.Text = Path.Combine(folderBrowserDialog.SelectedPath, selectedPacketData.CustomerName);
            }
        }

        #endregion 

        #region TokenTabPage Events

        private void browseTokenFileButton_Click(object sender, EventArgs e)
        {
            LicenseServerProperty selectedLicense = selectedObject as LicenseServerProperty;
            //if software use browse csv dialog
            if (this.browseCSVOpenFileDialog.ShowDialog() == DialogResult.OK)
            {
                tokenListView.Items.Clear();
                try
                {
                    using (StreamReader sr = new StreamReader(browseCSVOpenFileDialog.FileName))
                    {                        
                        String line, tokenValue, tokenName;
                        int tokenNameStartPos, tokenNameEndPos, tokenValueStartPos, tokenValueEndPos;
                        while ((line = sr.ReadLine()) != null)
                        {
                            if (line.Length > 0)
                            {
                                tokenNameStartPos = line.IndexOf("\"");
                                tokenNameEndPos = line.IndexOf("\"", tokenNameStartPos + 1);
                                tokenName = line.Substring(tokenNameStartPos, tokenNameEndPos - tokenNameStartPos);
                                tokenName = tokenName.Trim(new Char[] { '"' });
                                tokenValueStartPos = line.IndexOf("\"", tokenNameEndPos + 1);
                                tokenValueEndPos = line.IndexOf("\"", tokenValueStartPos + 1);
                                tokenValue = line.Substring(tokenValueStartPos, tokenValueEndPos - tokenValueStartPos);
                                tokenValue = tokenValue.Trim(new Char[] { '"' });
                                if (IsValidTokenType(tokenName))
                                {
                                    ListViewItem lvItem = new ListViewItem(tokenName);
                                    lvItem.Name = GetTokenEnum(tokenName).ToString();
                                    lvItem.SubItems.Add(tokenValue);
                                    tokenListView.Items.Add(lvItem);
                                }
                            }
                        }
                        sr.Close();
                    }
                    if (tokenListView.Items.Count > 0)
                    {
                        tokenListView.Columns[0].Width = -2;
                        tokenListView.Columns[1].Width = -2;
                        btnOk.Enabled = true;
                        TokenDescriptionLabel.Text = "Customer Validation Information";
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Failed to import validation tokens from " + browseCSVOpenFileDialog.FileName, "Validation Token Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }
        //fill tokenlist with available hardware tokens
        //hide browse button
        private void hardwareRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            LicenseServerProperty selectedOrder = selectedObject as LicenseServerProperty;
            TokenDescriptionLabel.Text = "Available Hardware Tokens";
            browseTokenFileButton.Visible = false;
            tokenListView.Items.Clear();
            btnOk.Enabled = true;
            Service<ICreator>.Use((client) =>
            {
                IList<TokenTable> tokenList = client.GetAvailableHardwareTokensByCustID(selectedOrder.CustID);
                foreach (TokenTable token in tokenList)
                {
                    if (token.TokenStatus != (byte)TokenStatus.Deactivated)
                    {
                        ListViewItem lvItem = new ListViewItem(token.TokenValue);
                        lvItem.SubItems.Add(((TokenStatus)token.TokenStatus).ToString());
                        tokenListView.Items.Add(lvItem);
                    }
                }
            });
            if (tokenListView.Items.Count > 0)
            {
                tokenListView.Focus();
                tokenListView.Items[0].Selected = true;
                tokenListView.Columns[0].Width = -2;
                tokenListView.Columns[1].Width = -2;
            }

        }
        //enable browse button, ask for 
        private void softwareRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            TokenDescriptionLabel.Text = "Please browse for customer information.";
            browseTokenFileButton.Visible = true;
            tokenListView.Items.Clear();
            btnOk.Enabled = false;
        }

        #endregion

        #region ProductLicenseTabPage Events

        private void productLicenseProductComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            //filter out available types - for status
            ProductLicenseDialogData storedObject = selectedObject as ProductLicenseDialogData;            
            //get current product version for product;
            string selectedProduct = ProductLicenseProductComboBox.SelectedItem.ToString();
            ProductLicenseTypeComboBox.Items.Clear();
            productLicenseVersionMaskedTextBox.Text = "0.00";
            Service<ICreator>.Use((client) => 
            {
                //get all orders for a given product
                IList<ProductLicenseTable> dbProductLicenses = client.GetProductLicensesByProduct(storedObject.LicenseServerID, (byte)m_CommLink.GetProductID(selectedProduct));
                List<ProductLicenseTable> productLicenses;
                foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in m_CommLink.m_softwareSpec.productSpecMap.TVal.Values)
                {
                    if (productSpec.productName.TVal.Equals(selectedProduct))
                    {
                        productLicenses = client.GetProductLicensesByProduct(storedObject.LicenseServerID, (byte)m_CommLink.GetProductID(productSpec.productName.TVal));
                        if (!(productSpec.productLicType.TVal.Equals(Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.TProductLicenseType.pltClient) &&
                            productLicenses.Count > 0))
                            ProductLicenseTypeComboBox.Items.Add(ProductLicenseState.Trial);
                        break;
                    }
                }
                if (dbProductLicenses.Count > 0)
                {
                    ProductLicenseState state = (ProductLicenseState)dbProductLicenses[0].plState;
                    foreach (ProductLicenseTable plRec in dbProductLicenses)
                    {
                        if (plRec.plState.Equals((byte)ProductLicenseState.Licensed))
                        {
                            ProductLicenseTypeComboBox.Items.Add(string.Format("{0} : {1}", ProductLicenseState.AddOn, plRec.plID));  
                        }
                    }               
                }
                ProductLicenseTypeComboBox.SelectedIndex = 0;
                string baseProduct = (selectedProduct.Contains("Test")) ?  CreatorForm.s_CommLink.GetProductBaseName(selectedProduct) : selectedProduct;
                short productID = CreatorForm.s_CommLink.GetProductID(baseProduct);
                if (productID > 0)
                {
                    int productVersion = client.GetProductVersionFromTable((byte)productID);
                    productLicenseVersionMaskedTextBox.Text = string.Format("{0:x}", productVersion);
                }
            });

        }

        private void expDateTextBox_TextChanged(object sender, EventArgs e)
        {
            try
            {
                Int32.Parse(expDateTextBox.Text);
            }
            catch (Exception ex)
            {
                System.Windows.Forms.MessageBox.Show("Please input a valid number");
                expDateTextBox.SelectionStart = 0;
                expDateTextBox.SelectionLength = expDateTextBox.Text.Length;
            }
        }
        #endregion
    }

    #region LicenseDialogData class
    public class LicenseDialogData : Shared.VisualComponents.DialogData
    {
        private LicenseServerProperty m_LicInfo;

        #region Constructors

        public LicenseDialogData(LicenseServerProperty licInfo)
        {
            m_LicInfo = licInfo;            
        }
        #endregion

        #region Properties

        public LicenseServerProperty LicInfo
        {
            get { return this.m_LicInfo; }
            set { this.m_LicInfo = value; }
        }
        #endregion
    }
    #endregion

    #region PacketDialogData class
    public class PacketDialogData : Shared.VisualComponents.DialogData
    {
        string _packetName;
        DateTime _expDate;
        string _customerName;
        string _selectedDirectory;
        string _userNotes;

        #region Constructors

        public PacketDialogData(string customerName, string packetName, string selectedDirectory)
        {
            _customerName = customerName;
            _packetName = packetName;
            _selectedDirectory = selectedDirectory;
        }
        #endregion

        #region Properties

        public string PacketName
        {
            get { return _packetName; }
            set { _packetName = value; }            
        }

        public string SelectedDirectory
        {
            get { return _selectedDirectory; }
            set { _selectedDirectory = value; }
        }

        public DateTime ExpDate
        {
            get { return _expDate; }
            set { _expDate = value; }
        }

        public string CustomerName
        {
            get { return _customerName; }
            set { _customerName = value; }
        }

        public string UserNotes
        {
            get { return _userNotes; }
            set { _userNotes = value; }
        }
        #endregion
    }
    #endregion

    #region TokenDialogData class
    public class TokenDialogData : Shared.VisualComponents.DialogData
    {        
        private LicenseServerProperty m_LicInfo;
        private List<Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs> m_TokenList;

        #region Constructors

        public TokenDialogData(LicenseServerProperty licInfo)
        {
            m_LicInfo = licInfo;
            m_TokenList = new List<Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs>();
        }
        #endregion

        #region Properties

        public LicenseServerProperty LicInfo
        {
            get { return this.m_LicInfo; }
            set { this.m_LicInfo = value; }
        }

        public List<Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs> TokenList
        {
            get { return this.m_TokenList; }
            set { this.TokenList = value; }
        }

        #endregion
    }
    #endregion

    #region ProductLicenseDialogData class
    public class ProductLicenseDialogData : Shared.VisualComponents.DialogData
    {
        private ProductLicenseTable _pltInfo;
        private string _licenseServerID;

        #region Constructors

        public ProductLicenseDialogData(string licenseServerID, ProductLicenseTable pltInfo)
        {
            _pltInfo = pltInfo;
            _licenseServerID = licenseServerID;
        }
        #endregion

        #region Properties

        public ProductLicenseTable ProductLicense
        {
            get { return this._pltInfo; }
            set { this._pltInfo = value; }
        }
        public string LicenseServerID
        {
            get { return this._licenseServerID; }
        }
        #endregion
    }
    #endregion

    #region HardwareKeyDialogData class
    public class HardwareKeyDialogData : Shared.VisualComponents.DialogData
    {
        private string _customerName;
        private string _keyValue;

        #region Constructors

        public HardwareKeyDialogData()
        {
            _customerName = "";
            _keyValue = "";
        }

        public HardwareKeyDialogData(string custName, string keyValue)
        {
            _customerName = custName;
            _keyValue = keyValue;
        }
        #endregion

        #region Properties

        public string CustomerName
        {
            get { return _customerName; }
            set { _customerName = value; }
        }

        public string KeyValue
        {
            get { return _keyValue; }
            set { _keyValue = value; }
        }
        #endregion
    }
    #endregion
}
