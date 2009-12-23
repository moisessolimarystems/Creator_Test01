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
        private DateTime _currentExpirationDate;

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
            else if (e.Data is ModuleDialogData)
                LoadModuleTabPage(e.Data as ModuleDialogData);
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
            else if (e.Data is ModuleDialogData)
                SaveModuleTabPage(e.Data as ModuleDialogData);
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

        bool IsClientType(string productName)
        {
            bool bRetVal = false;
            foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in m_CommLink.m_softwareSpec.productSpecMap.TVal.Values)
            {
                if (productSpec.productName.TVal.Equals(productName))
                {
                    if (productSpec.productLicType.TVal.Equals(Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.TProductLicenseType.pltClient))
                    {
                        bRetVal = true;
                        break;
                    }
                }
            }
            return bRetVal;
        }

        void LoadProductLicenseTabPage(ProductLicenseDialogData plData)
        {
            selectedObject = plData as Object;
            topPanel.Controls.Clear();
            productLicenseGroupBox.Parent = topPanel;
            productLicenseNumberTextBox.Text = plData.ProductLicense.ID;
            List<ProductLicenseTable> productLicenses;
            List<string> productList = new List<string>();
            Service<ICreator>.Use((client) => 
            {
                foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in m_CommLink.m_softwareSpec.productSpecMap.TVal.Values)
                {
                    bool bSkip = false;
                    productLicenses = client.GetProductLicensesByProduct(plData.ProductLicense.LicenseServer, (int)productSpec.productID.TVal);
                    foreach (ProductLicenseTable pl in productLicenses)
                    {
                        //skip if client and not perm
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

        //fill product entity with default modules
        //only trial/addon order types
        private void SetProductEntity(ProductLicenseProperty plData)
        {
            Lic_PackageAttribs.Lic_ModuleInfoAttribs module;
            Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.Lic_ModuleSoftwareSpecAttribsMap moduleList;
            foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in m_CommLink.m_softwareSpec.productSpecMap.TVal.Values)
            {
                if (productSpec.productID.TVal == plData.Product.ID)
                {
                    moduleList = m_CommLink.GetModuleSpecList(productSpec.productID.TVal);
                    foreach (Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs moduleSpec in moduleList.TVal.Values)
                    {
                        module = new Lic_PackageAttribs.Lic_ModuleInfoAttribs();
                        module.moduleID.TVal = moduleSpec.moduleID.TVal;
                        if (plData.Status.Equals(ProductLicenseState.Trial))
                        {
                            module.moduleValue.TVal = moduleSpec.moduleTrialLicense.TVal;
                            module.moduleAppInstance.TVal = 1;
                        }
                        if(module.moduleValue.TVal > 0)
                            plData.Product.ModuleList.TVal.Add(module);
                    }
                }
            }
        }

        void SaveProductLicenseTabPage(ProductLicenseDialogData plData)
        {
            string[] splitStatus = ProductLicenseTypeComboBox.SelectedItem.ToString().Split(":".ToCharArray());
            //can only be one or two
            if (splitStatus.Count() > 1)
            {
                plData.ProductLicense.Status = (ProductLicenseState)Enum.Parse(typeof(ProductLicenseState), splitStatus[0].Trim());
                plData.ProductLicense.ParentID = splitStatus[1].Trim();
            }
            else
                plData.ProductLicense.Status = (ProductLicenseState)Enum.Parse(typeof(ProductLicenseState), splitStatus[0]);
            plData.ProductLicense.Product.Product.bUseExpirationDate.TVal = true;
            plData.ProductLicense.Product.ID = (uint)m_CommLink.GetProductID(ProductLicenseProductComboBox.SelectedItem.ToString());
            plData.ProductLicense.ProductLicenseType = m_CommLink.GetProductSpec(plData.ProductLicense.Product.ID).productLicType.TVal;
            plData.ProductLicense.Product.AppInstance = (uint)((plData.ProductLicense.Status == ProductLicenseState.AddOn) ? 0 : 1);
            string[] versionNum = productLicenseVersionMaskedTextBox.Text.Split(".".ToCharArray());
            plData.ProductLicense.Product.Version = new LicenseVersion(UInt32.Parse(versionNum[0]), UInt32.Parse(versionNum[1]));
            plData.ProductLicense.ExpirationDate = CurrentExpirationDate.AddDays(Int32.Parse(expDateTextBox.Text));
            plData.ProductLicense.Description = productLicenseDescriptionTextBox.Text;
            SetProductEntity(plData.ProductLicense);
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
        #endregion

        #region Packet Methods
        private void LoadPacketTabPage(PacketDialogData packetData)
        {
            topPanel.Controls.Clear();
            packetGroupBox.Parent = topPanel;
            selectedObject = packetData as Object;
            packetNameTextBox.Text = packetData.PacketInfo.Name;
            packetExpDateTextBox.Text = DateTime.Now.AddDays(14).ToLongDateString();
            packetOutputPathTextBox.Text = packetData.PacketInfo.OutputPath;           
            packetDescriptTextBox.Text = packetData.PacketInfo.Description;
        }
        private void SavePacketTabPage(PacketDialogData packetData)
        {
            packetData.ExpDate = DateTime.Parse(packetExpDateTextBox.Text); 
            packetData.PacketInfo.OutputPath = packetOutputPathTextBox.Text;
            if (!Directory.Exists(packetData.PacketInfo.OutputPath))            
                Directory.CreateDirectory(packetData.PacketInfo.OutputPath);            
            packetData.PacketInfo.Description = packetDescriptTextBox.Text;
        }
        #endregion

        #region Module Methods
        private void LoadModuleTabPage(ModuleDialogData moduleData)
        {
            //Test/Dev modules should not exceed standard license modules
            //Need to grab license from database to find last 
            topPanel.Controls.Clear();
            moduleGroupBox.Parent = topPanel;
            //edit module
            if (moduleData.Module != null)
            {
                string moduleName = moduleData.Module.Name;
                modNameTextBox.Text = moduleName;               
                uint modID, unlimitedValue, totalValue = 0;
                modID = (uint)m_CommLink.GetModuleID(moduleData.ProductLicense.Product.ID, moduleName);
                unlimitedValue = (uint)m_CommLink.GetUnlimitedModuleValue(moduleData.ProductLicense.Product.ID, modID);
                //need total product value 
                Service<ICreator>.Use((client) => 
                {
                    LicenseTable licRec = client.GetLicenseByName(moduleData.ProductLicense.LicenseServer, false);
                    Lic_PackageAttribs licPackage = new Lic_PackageAttribs();
                    licPackage.Stream = licRec.LicenseInfo;                        
                    foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs product in licPackage.licLicenseInfoAttribs.TVal.productList.TVal)
                    {
                        if (product.productID.TVal.Equals(moduleData.ProductLicense.Product.ID))
                        {
                            foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs modRec in product.moduleList.TVal)
                            {
                                if (modRec.moduleID.TVal.Equals(modID) && (modRec.contractNumber.TVal.Equals(moduleData.ProductLicense.ID) || modRec.contractNumber.TVal.Equals(moduleData.ProductLicense.ParentID)))
                                {
                                    totalValue += modRec.moduleValue.TVal;
                                    break;
                                }
                            }
                        }
                    }
                });
                int availableUnits = (int)(unlimitedValue - totalValue);
                //need to set max to 0 if availability is 0
                foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs module in moduleData.ProductLicense.Product.ModuleList.TVal)
                {                        
                    if (module.moduleID.TVal.Equals(modID))
                    {
                        //unlimited = 0 or 1 or > 1
                        // -> unlimited 
                        //available units = 0, -1, or > 0
                        modValueNumericUpDown.Value = 0;                            
                        if (unlimitedValue == 0 || availableUnits < 0)
                        {
                            modValueNumericUpDown.Value = module.moduleValue.TVal;
                            modValueNumericUpDown.Maximum = 65536;
                            break;
                        }
                        else
                        {
                            if (availableUnits > 0)
                            {
                                modValueNumericUpDown.Maximum = availableUnits;
                                modValueNumericUpDown.Value = module.moduleValue.TVal;
                            }
                            else
                            {
                                modValueNumericUpDown.Value = 0;
                                modValueNumericUpDown.Maximum = 0;
                            }
                            break;
                        }
                    }                        
                }                
            }
        }
       
        //if module is from a std order bump up app inst when > 0
        //if module is from a std order w/app inst 0 and there exists an add-on order with app inst 1, decrement add-on
        //if module is from add-on and std order & any add-on has appinst 0 then +1 appinst for add-on
        private void SaveModuleTabPage(ModuleDialogData moduleData)
        {
            int totalModAppInstance;
            //change has been made.
            if (modValueNumericUpDown.Value != moduleData.Module.Units)
            {
                if (modValueNumericUpDown.Value > 0)
                {
                    Lic_PackageAttribs.Lic_ModuleInfoAttribs editModule = null;
                    if (!moduleData.ProductLicense.Product.Contains(moduleData.Module.ID))
                    {
                        editModule = new Lic_PackageAttribs.Lic_ModuleInfoAttribs();
                        editModule.moduleID.TVal = moduleData.Module.ID;                        
                        moduleData.ProductLicense.Product.ModuleList.TVal.Add(editModule);
                    }
                    else
                    {
                        editModule = moduleData.ProductLicense.Product.GetModule(moduleData.Module.ID);
                    }
                    editModule.moduleValue.TVal = (uint)modValueNumericUpDown.Value;
                    //if client type set product connections to total
                    //call function to get product spec then check to see if it is a client
                    totalModAppInstance = moduleData.ProductLicense.GetTotalModuleAppInstance(editModule.moduleID.TVal);
                    if (editModule.moduleValue.TVal > 0)
                    {
                        if (totalModAppInstance == 0)
                            editModule.moduleAppInstance.TVal = 1;
                        else //>1
                        {
                            if (totalModAppInstance > 1)
                            {
                                if (IsClientType(moduleData.ProductLicense.ProductName) && editModule.moduleState.TVal != Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msAddOn)
                                    editModule.moduleAppInstance.TVal = moduleData.ProductLicense.Product.AppInstance;
                                else
                                    editModule.moduleAppInstance.TVal = (uint)((moduleData.ProductLicense.Status == ProductLicenseState.AddOn) ? 0 : 1);
                            }
                            else
                            {
                                if (moduleData.ProductLicense.Status == ProductLicenseState.AddOn)
                                    editModule.moduleAppInstance.TVal = 0;
                                else
                                {
                                    editModule.moduleAppInstance.TVal = 1;
                                    moduleData.ProductLicense.DecrementAddOnModuleAppInstance(editModule.moduleID.TVal);
                                }
                            }
                        }
                    }
                    else
                    {
                        //if decrement std then increment add-on
                        //if decrement add-on don't decrement appinstance unless it is 1
                        editModule.moduleAppInstance.TVal = 0;
                        if (moduleData.ProductLicense.Status != ProductLicenseState.AddOn)
                        {
                            if (moduleData.ProductLicense.GetTotalAddOnModuleAppInstance(editModule.moduleID.TVal) == 0)
                                moduleData.ProductLicense.IncrementAddOnModuleAppInstance(editModule.moduleID.TVal);
                        }
                    }
                    //need to set again because it may have been updated based on units change
                    moduleData.Module.AppInstance = editModule.moduleAppInstance.TVal;
                    moduleData.Module.Units = editModule.moduleValue.TVal;
                    //if exist update
                    //else add
                }
                else
                {   //remove from product list if exist
                    moduleData.Module.AppInstance = 0;
                    moduleData.Module.Units = 0;
                    moduleData.ProductLicense.Product.RemoveModule(moduleData.Module.ID);
                }
            }
        }

        #endregion

        #region ModuleTabPage Events

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

        private void ValidateTokenForm()
        {
            Cursor.Current = Cursors.WaitCursor;
            m_Validated = true;
            //Service<ICreator>.Use((client) =>
            //{
            //    LicenseServerProperty selectedLicense = selectedObject as LicenseServerProperty;
            //    if (hardwareRadioButton.Checked)
            //    {

            //        Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType selectedType = Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID;
            //        if (client.TokenExists(selectedLicense.CustID, (byte)selectedType, tokenListView.SelectedItems[0].Text))
            //        {
            //            m_Validated = false;
            //            this.tokenValueTextBox.Select(0, this.tokenValueTextBox.Text.Length);

            //            // Set the ErrorProvider error with the text to display.
            //            //errorProvider1.SetError(this.tokenValueTextBox, "Validation token already exists for this customer!");
            //            MessageBox.Show("Validation token already exists for this customer!", "Validation Token Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            //        }
            //    }
            //    else
            //    {
            //    }
            //});
            Cursor.Current = Cursors.Default; 
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
                IList<ProductLicenseTable> dbProductLicenses = client.GetProductLicensesByProduct(storedObject.ProductLicense.LicenseServer, (int)m_CommLink.GetProductID(selectedProduct));
                List<ProductLicenseTable> productLicenses;
                foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in m_CommLink.m_softwareSpec.productSpecMap.TVal.Values)
                {
                    if (productSpec.productName.TVal.Equals(selectedProduct)) //&& !productSpec.productName.TVal.Contains("Test"))
                    {
                        productLicenses = client.GetProductLicensesByProduct(storedObject.ProductLicense.LicenseServer, (int)m_CommLink.GetProductID(productSpec.productName.TVal));
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
                int productID = CreatorForm.s_CommLink.GetProductID(baseProduct);
                if (productID > 0)
                {
                    int productVersion = client.GetProductVersionFromTable(productID);
                    productLicenseVersionMaskedTextBox.Text = string.Format("{0:x}", productVersion);
                }
            });

        }

        private void productLicenseTypeComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            //on trial don't need to do anything
            //on addon display list of possible orders to add onto 
            //if (orderTypeComboBox.SelectedItem.ToString() == Enum.GetName(typeof(Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState), Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msLicensed))
            //    //disable expiration date
            //    orderDateTimePicker.Enabled = false;
            //else
            //    orderDateTimePicker.Enabled = true;            
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

        private bool IsValidTokenType(string tokenType)
        {
            if(GetTokenEnum(tokenType) != Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttNone)
                return true;
            return false;
        }
            
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
                        CustomerTable ct = null;
                        String line, tokenValue, tokenName;                        
                        while ((line = sr.ReadLine()) != null)
                        {
                            tokenName = line.Substring(0, line.IndexOf(",") -1);
                            tokenName = tokenName.Trim(new Char[] { '"' });
                            tokenValue = line.Remove(0, line.IndexOf(",") + 1);
                            tokenValue = tokenValue.Trim(new Char[] { '"' });
                            //file validation
                            if (tokenName == "Company")
                            {
                                Service<ICreator>.Use((client) =>
                                {
                                    ct = client.GetCustomer(selectedLicense.CustID.ToString(), false);
                                });
                                if (ct != null)
                                {
                                    if (ct.SCRname != tokenValue)
                                    {
                                        MessageBox.Show("Invalid Validation Token file for " + ct.SCRname, "Software Token Error");
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                if (IsValidTokenType(tokenName))
                                {
                                    ListViewItem lvItem = new ListViewItem(tokenName);
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
                catch (Exception)
                {
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
        private PacketProperty m_PacketInfo;
        private DateTime m_ExpDate;
        private string m_Customer;

        #region Constructors

        public PacketDialogData(PacketProperty packetInfo, string customerName)
        {
            m_PacketInfo = packetInfo;
            m_Customer = customerName;
        }
        #endregion

        #region Properties

        public PacketProperty PacketInfo
        {
            get { return this.m_PacketInfo; }
            set { this.m_PacketInfo = value; }
        }

        public DateTime ExpDate
        {
            get { return m_ExpDate; }
            set { m_ExpDate = value; }
        }

        public string CustomerName
        {
            get { return m_Customer; }
            set { m_Customer = value; }
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

    #region ModuleDialogData class
    public class ModuleDialogData : Shared.VisualComponents.DialogData
    {
        private ModuleProperty _module;
        private ProductLicenseProperty _plData;
        #region Constructors

        public ModuleDialogData(ModuleProperty module, ProductLicenseProperty plData)
        {
            _module = module;
            _plData = plData;
        }
        #endregion

        #region Properties

        public ModuleProperty Module
        {
            get { return this._module; }
            set { this._module = value; }
        }
        public ProductLicenseProperty ProductLicense
        {
            get { return this._plData; }
            set { this._plData = value; }
        }
        #endregion
    }
    #endregion

    #region ProductLicenseDialogData class
    public class ProductLicenseDialogData : Shared.VisualComponents.DialogData
    {
        private ProductLicenseProperty _plInfo;

        #region Constructors

        public ProductLicenseDialogData(ProductLicenseProperty plInfo)
        {
            _plInfo = plInfo;
        }
        #endregion

        #region Properties

        public ProductLicenseProperty ProductLicense
        {
            get { return this._plInfo; }
            set { this._plInfo = value; }
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
