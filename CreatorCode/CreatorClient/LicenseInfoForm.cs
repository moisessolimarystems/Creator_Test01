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
using System.Text.RegularExpressions;

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
            if (e.Data is TokenDialogData)
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
            if (e.Data is TokenDialogData)
                SaveTokenTabPage(e.Data as TokenDialogData);
            else if (e.Data is PacketDialogData)
                SavePacketTabPage(e.Data as PacketDialogData);
            else if (e.Data is ProductLicenseDialogData)
                SaveProductLicenseTabPage(e.Data as ProductLicenseDialogData);
        }

        private void btnOk_Click(object sender, EventArgs e)
        {
            if (tokenAttribsSplitContainer.Parent == topPanel)
                ValidateTokenForm();
            if (selectedObject is ProductLicenseDialogData)
                ValidateProductLicenses();
            if (selectedObject is PacketDialogData)
                ValidatePacketPath();
        }

        private void LicenseInfoForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (DialogResult == DialogResult.OK)
                if (!m_Validated)
                    e.Cancel = true;
        }
        #endregion

        #region Product License Methods
        void ValidateProductLicenses()
        {
            ProductLicenseDialogData pltData = selectedObject as ProductLicenseDialogData;
            m_Validated = true;
            List<ProductLicenseTable> pltList = null;
            Service<ICreator>.Use((client) =>
            {
                pltList = client.GetProductLicenses(pltData.LicenseServerString, true);               
            });
            SaveCurrentItem();
            foreach (ProductLicenseTable plt in bindingNavigator1.BindingSource.List)
            {
                if (pltList.Where(p => p.plID == plt.plID).Count() != 0)
                {
                    string previousID = plt.plID;
                    //retrieve the next available number 
                    plt.plIndex = pltData.NextLicenseServerIndex;
                    plt.plID = Lic_LicenseInfoAttribsHelper.GenerateProductLicenseName(pltData.LicenseServerString, pltData.NextLicenseServerIndex);
                    pltData.NextLicenseServerIndex = pltData.NextLicenseServerIndex + 1;                    
                    //can loop through and 
                    MessageBox.Show(string.Format("{0} already exists! {0} will be updated to {1}. Please try again.", previousID, plt.plID), "Error", MessageBoxButtons.OK);
                    m_Validated = false;
                    InitializeProductLicenseTabPage(plt);
                    break;
                }
            } 
        }

        void InitializeProductLicenseTabPage(ProductLicenseTable plt)
        {
            ProductLicenseDialogData pltData = selectedObject as ProductLicenseDialogData;
            productLicenseAttribsGroupBox.Text = plt.plID;
            List<ProductLicenseTable> productLicenses;
            List<string> productList = new List<string>();
            Service<ICreator>.Use((client) => 
            {
                IList<ProductLicenseTable> dbProductLicenses = client.GetProductLicenses(pltData.LicenseServerString, true);
                foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in m_CommLink.m_softwareSpec.productSpecMap.TVal.Values)
                {
                    bool bSkip = false;
                    productLicenses = dbProductLicenses.Where(p => p.ProductID == (byte)productSpec.productID.TVal).ToList();//client.GetProductLicensesByProduct(pltData.LicenseServerString, (byte)productSpec.productID.TVal);
                    /*foreach (ProductLicenseTable pl in productLicenses)
                    {   //skip if client and not perm
                        if (pl.plState.Equals((byte)ProductLicenseState.Trial) &&
                           productSpec.productLicType.TVal.Equals(Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.TProductLicenseType.pltClient))
                            bSkip = true;
                    }
                    if (!bSkip)
                     */
                        productList.Add(productSpec.productName);
                }
            });
            if (productList.Count == 0)
                productList.Add("No Products Available");
            ProductLicenseProductComboBox.Items.Clear();
            ProductLicenseProductComboBox.Items.AddRange(productList.ToArray());
            
            //if plt.productID 
            //set to plt.productid 
            //convert id to product string
            ProductLicenseProductComboBox.SelectedIndex = (plt.ProductID > 0) ? ProductLicenseProductComboBox.FindString(m_CommLink.GetProductName(plt.ProductID)) : 0;
            //set product type
            typeComboBox.DataSource = ProductLicense.ProductLicenseTypeList;
            //set product version
            if (plt.ProductVersion != null)
                productLicenseVersionMaskedTextBox.Text = plt.ProductVersion;
            //set to plt.expiration
            expDateTextBox.Text = (plt.ExpirationDate.HasValue) ? plt.ExpirationDate.Value.Date.Subtract(DateTime.Today).Days.ToString() : "30";
            //set longDate string
            longDateLabel.Text = "(" + CurrentExpirationDate.AddDays(Int32.Parse(expDateTextBox.Text)).ToUniversalTime().ToLongDateString() + ")";
            //set to description if set
            productLicenseDescriptionTextBox.Text = plt.Description;
            EnableBindingNavigatorItems();
        }

        //needs next product license name, license server name
        void LoadProductLicenseTabPage(ProductLicenseDialogData plData)
        {
            selectedObject = plData as Object;
            topPanel.Controls.Clear();
            productLicenseSplitContainer.Parent = topPanel;
            //create bindinglist from productlicense
            BindingList<ProductLicenseTable> bindingList = new BindingList<ProductLicenseTable>();
            bindingNavigator1.BindingSource = new BindingSource();
            bindingNavigator1.BindingSource.DataSource = bindingList;            
            InitializeProductLicenseTabPage(AddCurrentItem());
        }

        void SaveProductLicenseTabPage(ProductLicenseDialogData plData)
        {
            //SaveCurrentItem();
            List<ProductLicenseTable> pltList = new List<ProductLicenseTable>();
            foreach (ProductLicenseTable plt in bindingNavigator1.BindingSource.List)
            {
                pltList.Add(plt);
            }
            plData.ProductLicense = pltList;
        }
        #endregion

        #region Token Methods
        //needs to support SOFTWARE/HARDWARE validation
        private void LoadTokenTabPage(TokenDialogData tokenData)
        {
            selectedObject = tokenData.LicInfo as Object;
            ////hardware token get value from database
            btnOk.Enabled = false;
            topPanel.Controls.Clear();
            tokenAttribsSplitContainer.Parent = topPanel;
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
                LicenseServer selectedLicense = selectedObject as LicenseServer;
                Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType selectedType;
                if (hardwareRadioButton.Checked)
                {
                    selectedType = Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID;
                    if (client.TokenExists(selectedLicense.CustID, (byte)selectedType, tokenListView.SelectedItems[0].Text))
                    {
                        m_Validated = false;
                        // Set the ErrorProvider error with the text to display.
                        MessageBox.Show(tokenListView.SelectedItems[0].Text + " already exists for this customer!", "Validation Token Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
                else
                {
                    IList<SoftwareTokenTable> tokenList = CreatorForm.s_AllSoftwareTokens;// client.GetAllSoftwareTokens();
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
                            if (lvItem.Name == Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttPartOfDomain.ToString())
                            {
                                if (lvItem.SubItems[1].Text != Boolean.TrueString)
                                {
                                    if (MessageBox.Show("The software token 'Part of Domain' has the value 'False'. Do you want to continue to use this set of software tokens?", "Validation Tokens", MessageBoxButtons.YesNo, MessageBoxIcon.Warning) == DialogResult.No)
                                    {
                                        m_Validated = false;
                                        break;
                                    }
                                }
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
                            IList<LicenseTable> licServerList = client.GetLicensesByCustomer(customerDB.SCRname, false);
                            IList<TokenTable> tokenDBList = client.GetTokensByCustomerID(customerDB.SCRnumber);
                            foreach (LicenseTable licServer in licServerList)
                            {
                                bool bMatch = false; 
                                //loop thru each active token for 
                                foreach (TokenTable tokenDB in tokenDBList.Where(t => t.LicenseID == licServer.ID && t.TokenStatus != (byte)TokenStatus.Deactivated))
                                {
                                    tokenName = ((Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType)tokenDB.TokenType).ToString();
                                    //tokenlistview = user input tokens
                                    foundTokens = tokenListView.Items.Find(tokenName, false);                                        
                                    if (foundTokens.Count() > 0)
                                    {
                                        tokenValue = foundTokens[0].SubItems[1].Text;
                                        if (tokenDB.TokenValue != tokenValue)
                                        {
                                            bMatch = false;
                                            break;
                                        }
                                        else
                                        {
                                            bMatch = true;
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
            licPktSplitContainer.Parent = topPanel;
            selectedObject = packetData as Object;
            licPacketAttribsGroupBox.Text = packetData.PacketName + ".packet";
            licPacketDescriptionLabel.Text = string.Format("This packet will expire on {0}. Please choose a location for the license packet to be generated.", DateTime.Now.AddDays(14).ToLongDateString());
            packetOutputPathTextBox.Text = packetData.SelectedDirectory;                       
            Service<ICreator>.Use((client) => 
            {
                TokenTable token = client.GetTokenByLicenseName(packetData.LicenseName, (byte)Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttLicenseCode);
                clearLicenseCodeCheckBox.Enabled = (token != null);
            });
        }
        private void SavePacketTabPage(PacketDialogData packetData)
        {           
            packetData.ExpDate = DateTime.Now.AddDays(14); 
            packetData.SelectedDirectory = packetOutputPathTextBox.Text;       
            packetData.UserNotes = packetDescriptTextBox.Text;
            packetData.ClearLicenseCode = clearLicenseCodeCheckBox.Checked;
        }
        private void ValidatePacketPath()
        {
            try
            {
                if (!Directory.Exists(packetOutputPathTextBox.Text))
                    Directory.CreateDirectory(packetOutputPathTextBox.Text);
                m_Validated = true;
            }
            catch(Exception e)
            {
                m_Validated = false;
                MessageBox.Show(packetOutputPathTextBox.Text + " is an invalid path.", "Packet Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
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

        private void packetDescriptTextBox_TextChanged(object sender, EventArgs e)
        {
            licPacketNotesLabel.Text = string.Format("{0} Characters Left(Limit is 255 characters)", 255 - packetDescriptTextBox.Text.Length);
        }

        private void packetOutputPathTextBox_MouseHover(object sender, EventArgs e)
        {            
            toolTip.Show(packetOutputPathTextBox.Text, packetOutputPathTextBox);
        }

        #endregion 

        #region TokenTabPage Events

        private void browseTokenFileButton_Click(object sender, EventArgs e)
        {
            LicenseServer selectedLicense = selectedObject as LicenseServer;
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
                        TokenDescriptionLabel.Text = selectedLicense.DestName + " Validation Information";
                    }
                    else
                        btnOk.Enabled = false;
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
            LicenseServer selectedOrder = selectedObject as LicenseServer;
            TokenDescriptionLabel.Text = "Please select from the available hardware tokens.";
            browseTokenFileButton.Visible = false;
            tokenListView.Items.Clear();
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
                btnOk.Enabled = true;
                tokenListView.Columns[0].Width = -2;
                tokenListView.Columns[1].Width = -2;
                tokenListView.Focus();
                tokenListView.Items[0].Selected = true;  
            }
            else
                btnOk.Enabled = false;

        }
        //enable browse button, ask for 
        private void softwareRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            LicenseServer selectedOrder = selectedObject as LicenseServer;
            TokenDescriptionLabel.Text = string.Format("Please browse to the csv file containing {0} validation tokens.", selectedOrder.DestName);
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
            Service<ICreator>.Use((client) => 
            {
                //get all orders for a given product
                IList<ProductLicenseTable> dbProductLicenses = client.GetProductLicensesByProduct(storedObject.LicenseServerString, (byte)m_CommLink.GetProductID(selectedProduct));
                //List<ProductLicenseTable> productLicenses;
                foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in m_CommLink.m_softwareSpec.productSpecMap.TVal.Values)
                {
                    if (productSpec.productName.TVal.Equals(selectedProduct))
                    {
                        /*productLicenses = dbProductLicenses.Where(p => p.ProductID == (byte)m_CommLink.GetProductID(productSpec.productName.TVal)).ToList();
                        if (!(productSpec.productLicType.TVal.Equals(Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.TProductLicenseType.pltClient) &&
                            productLicenses.Count > 0))
                            ProductLicenseTypeComboBox.Items.Add(ProductLicenseState.Trial);
                        */
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
                    LicenseVersion version = new LicenseVersion(productVersion);
                    productLicenseVersionMaskedTextBox.Text = version.ToString();
                }
            });

        }

        private void expDateTextBox_TextChanged(object sender, EventArgs e)
        {
            try
            {
                Int32.Parse(expDateTextBox.Text);
                longDateLabel.Text = "(" + CurrentExpirationDate.AddDays(Int32.Parse(expDateTextBox.Text)).ToUniversalTime().ToLongDateString() + ")";
            }
            catch (Exception ex)
            {
                System.Windows.Forms.MessageBox.Show("Please input a valid number");
                expDateTextBox.SelectionStart = 0;
                expDateTextBox.SelectionLength = expDateTextBox.Text.Length;
            }
        }

        private void bindingNavigatorAddNewItem_Click(object sender, EventArgs e)
        {
            //save current view
            SaveCurrentItem();
            //initialize new view
            InitializeProductLicenseTabPage(AddCurrentItem());
        }

        private void bindingNavigatorDeleteItem_Click(object sender, EventArgs e)
        {
            InitializeProductLicenseTabPage(RemoveCurrentItem());
            //delete current item
            //re-initialize previous view
        }

        private void SaveCurrentItem()
        {
            ProductLicenseTable plt = bindingNavigator1.BindingSource.Current as ProductLicenseTable;
            string[] splitStatus = ProductLicenseTypeComboBox.SelectedItem.ToString().Split(":".ToCharArray());
            //can only be one or two
            if (splitStatus.Count() > 1)
            {
                ProductLicenseState state = (ProductLicenseState)Enum.Parse(typeof(ProductLicenseState), splitStatus[0].Trim());
                plt.plState = (byte)state;
                plt.ParentProductLicenseID = splitStatus[1].Trim();
            }
            else
            {
                ProductLicenseState state = (ProductLicenseState)Enum.Parse(typeof(ProductLicenseState), splitStatus[0].Trim());
                plt.plState = (byte)state;
            }
            plt.ProductID = (byte)m_CommLink.GetProductID(ProductLicenseProductComboBox.SelectedItem.ToString());
            plt.ProductConnection = (byte)((plt.plState == (byte)ProductLicenseState.AddOn) ? 0 : 1);
            plt.ProductVersion = productLicenseVersionMaskedTextBox.Text;
            plt.ExpirationDate = CurrentExpirationDate.AddDays(Int32.Parse(expDateTextBox.Text)).ToUniversalTime();
            plt.Description = productLicenseDescriptionTextBox.Text;
        }

        private ProductLicenseTable AddCurrentItem()
        {
            ProductLicenseDialogData pltData = selectedObject as ProductLicenseDialogData;
            ProductLicenseTable plt = new ProductLicenseTable();
            plt.plIndex = pltData.NextLicenseServerIndex;
            plt.plID = Lic_LicenseInfoAttribsHelper.GenerateProductLicenseName(pltData.LicenseServerString, pltData.NextLicenseServerIndex);
            plt.LicenseID = pltData.LicenseServerID;            
            bindingNavigator1.BindingSource.Add(plt);
            //set current position to the end
            bindingNavigator1.BindingSource.Position = bindingNavigator1.BindingSource.List.Count-1;
            //bump next available LS index
            pltData.NextLicenseServerIndex = pltData.NextLicenseServerIndex + 1;
            return bindingNavigator1.BindingSource.Current as ProductLicenseTable;
        }

        //remove current plt
        //reindex from current to end
        //set current to item before removed item
        //update next available index
        private ProductLicenseTable RemoveCurrentItem()
        {
            ProductLicenseDialogData pltData = selectedObject as ProductLicenseDialogData;
            int currentPosition = bindingNavigator1.BindingSource.Position;
            int removeIndex = 0;
            if (bindingNavigator1.BindingSource.Count > 1)
            {
                removeIndex = (bindingNavigator1.BindingSource.Current as ProductLicenseTable).plIndex;
                bindingNavigator1.BindingSource.RemoveCurrent();            
                bindingNavigator1.BindingSource.Position = currentPosition - 1;
                foreach (ProductLicenseTable plt in bindingNavigator1.BindingSource.List)
                {   //reindex list
                    if (plt.plIndex > removeIndex)
                        plt.plID = Lic_LicenseInfoAttribsHelper.GenerateProductLicenseName(pltData.LicenseServerString, --plt.plIndex); 
                }
                pltData.NextLicenseServerIndex = pltData.NextLicenseServerIndex - 1;
            }
            return bindingNavigator1.BindingSource.Current as ProductLicenseTable;
        }

        private void bindingNavigatorMovePreviousItem_Click(object sender, EventArgs e)
        {
            SaveCurrentItem();
            bindingNavigator1.BindingSource.MovePrevious();
            InitializeProductLicenseTabPage(bindingNavigator1.BindingSource.Current as ProductLicenseTable);
        }

        private void bindingNavigatorMoveNextItem_Click(object sender, EventArgs e)
        {
            SaveCurrentItem();
            bindingNavigator1.BindingSource.MoveNext();
            InitializeProductLicenseTabPage(bindingNavigator1.BindingSource.Current as ProductLicenseTable);
        }

        private void EnableBindingNavigatorItems()
        {
            //enable move next
            bindingNavigatorMoveNextItem.Enabled = (bindingNavigator1.BindingSource.Position != bindingNavigator1.BindingSource.Count - 1);
            //enablt move back
            bindingNavigatorMovePreviousItem.Enabled = (bindingNavigator1.BindingSource.Position != 0);
            //enable move last
            bindingNavigatorMoveFirstItem.Enabled = (bindingNavigator1.BindingSource.Position != 0);
            //enable move first
            bindingNavigatorMoveLastItem.Enabled = (bindingNavigator1.BindingSource.Position != bindingNavigator1.BindingSource.Count - 1);
            //enable delete
            bindingNavigatorDeleteItem.Enabled = (bindingNavigator1.BindingSource.List.Count > 1);
        }

        private void bindingNavigatorMoveLastItem_Click(object sender, EventArgs e)
        {
            SaveCurrentItem();
            bindingNavigator1.BindingSource.MoveLast();
            InitializeProductLicenseTabPage(bindingNavigator1.BindingSource.Current as ProductLicenseTable);
        }

        private void bindingNavigatorMoveFirstItem_Click(object sender, EventArgs e)
        {
            SaveCurrentItem();
            bindingNavigator1.BindingSource.MoveFirst();
            InitializeProductLicenseTabPage(bindingNavigator1.BindingSource.Current as ProductLicenseTable);
        }

        private void productLicenseDescriptionTextBox_TextChanged(object sender, EventArgs e)
        {
            notesCharactersLeftLabel.Text = string.Format("{0} Characters Left(Limit is 255 characters)", 255 - productLicenseDescriptionTextBox.Text.Length);
        }
        #endregion

        private void productLicenseVersionMaskedTextBox_Validating(object sender, CancelEventArgs e)
        {
            Regex reg = new Regex(@"\d+\.\d+$");
            string version = productLicenseVersionMaskedTextBox.Text;
            if (!reg.IsMatch(version))
            {
                System.Windows.Forms.MessageBox.Show("Please enter a valid version", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                e.Cancel = true;
            }     
        }
    }

    #region PacketDialogData class
    public class PacketDialogData : Shared.VisualComponents.DialogData
    {
        #region Constructors

        public PacketDialogData() {}

        #endregion

        #region Properties

        public string PacketName
        {
            get;
            set;
        }

        public string LicenseName
        {
            get;
            set;
        }

        public string SelectedDirectory
        {
            get;
            set;
        }

        public DateTime ExpDate
        {
            get;
            set;
        }

        public string CustomerName
        {
            get;
            set;
        }

        public string UserNotes
        {
            get;
            set;
        }
        public bool ClearLicenseCode
        {
            get;
            set;
        }
        #endregion
    }
    #endregion

    #region TokenDialogData class
    public class TokenDialogData : Shared.VisualComponents.DialogData
    {        
        private LicenseServer m_LicInfo;
        private List<Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs> m_TokenList;

        #region Constructors

        public TokenDialogData(LicenseServer licInfo)
        {
            m_LicInfo = licInfo;
            m_TokenList = new List<Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs>();
        }
        #endregion

        #region Properties

        public LicenseServer LicInfo
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
        private List<ProductLicenseTable> _pltInfoList;
        private string _lsString;
        private int _lsID;
        private int _nextProductLicenseIndex;

        #region Constructors

        public ProductLicenseDialogData(string licenseServerString)
        {
            _pltInfoList = new List<ProductLicenseTable>();
            _lsString = licenseServerString;
            Service<ICreator>.Use((client) =>
            {
                _nextProductLicenseIndex = client.GetNextProductLicenseIndex(licenseServerString);
                _lsID = client.GetLicenseByName(licenseServerString, false).ID;
            });
        }
        #endregion

        #region Properties

        public List<ProductLicenseTable> ProductLicense
        {
            get { return this._pltInfoList.ToList(); }
            set 
            {
                this._pltInfoList.Clear();
                this._pltInfoList.AddRange(value);
            }
        }
        public string LicenseServerString
        {
            get { return this._lsString; }
        }

        public int NextLicenseServerIndex
        {
            get { return this._nextProductLicenseIndex; }
            set { this._nextProductLicenseIndex = value; }
        }

        public int LicenseServerID
        {
            get { return _lsID; }
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
            _customerName = string.Empty;
            _keyValue = string.Empty;
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
