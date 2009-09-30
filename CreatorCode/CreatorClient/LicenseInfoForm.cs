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
                //trial -> zero options : 1 order
                //perm -> add-on available : 1 order
                //perm, add-on -> zero available : 2 order    
                if (plData.LicenseInfo.LicType != LicenseServerType.TestDevelopment)
                {   //standard,failover,disaster             
                    foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in m_CommLink.m_softwareSpec.productSpecMap.TVal.Values)
                    {
                        bool bSkip = false;
                        if (!productSpec.productName.TVal.Contains("Test"))
                        {
                            productLicenses = client.GetProductLicensesByProduct(plData.ProductLicense.LicenseServer, (int)productSpec.productID.TVal);
                            //0 order => product hasnt been added
                            //if (productSpec.productLicType.TVal.Equals(Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.TProductLicenseType.pltClient) &&
                            //    productLicenses.Count > 0)
                            foreach (ProductLicenseTable pl in productLicenses)
                            {          
                                //skip if client and not perm
                                if(pl.plState.Equals((byte)ProductLicenseState.Trial) &&
                                   productSpec.productLicType.TVal.Equals(Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.TProductLicenseType.pltClient))
                                    bSkip = true;
                            }
                            if(!bSkip)
                                productList.Add(productSpec.productName);
                        }
                    }
                }
                else
                {
                    //only add products that can be added....
                    //testdev
                    foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in m_CommLink.m_softwareSpec.productSpecMap.TVal.Values)
                    {
                        if (productSpec.productName.TVal.Contains("Test"))
                        {
                            //should only add products that exist in the standard license
                            //and haven't already been added.
                            string licenseBase = "P";
                            LicenseTable baseLicRec = client.GetLicenseByName(string.Format("{0:x4}-{1:x3}-{2:x4}-{3}01", plData.LicenseInfo.CustID, plData.LicenseInfo.DestID, plData.LicenseInfo.GroupID, licenseBase), false);
                            if (baseLicRec == null) //subscription type                            
                                licenseBase = "S";                                                           
                            int pos = productSpec.productName.TVal.IndexOf("Test");
                            //1) call twice, once with product, then product test
                            //2) call once with product and add only when 1 order(standard) found
                            productLicenses = client.GetProductLicensesByProduct(licenseBase, (int)productSpec.productID.TVal);
                            if (productLicenses.Count == 1)
                            {   //0 order => product hasnt been added                     
                                productLicenses = client.GetProductLicensesByProduct(plData.ProductLicense.LicenseServer, (int)productSpec.productID.TVal);
                                if (productLicenses.Count == 0)
                                    productList.Add(productSpec.productName);
                            }
                        }
                    }
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
                plData.ProductLicense.Status = (ProductLicenseState)Enum.Parse(typeof(ProductLicenseState), splitStatus[0].Trim());
                plData.ProductLicense.ParentID = splitStatus[1].Trim();
            }
            else
                plData.ProductLicense.Status = (ProductLicenseState)Enum.Parse(typeof(ProductLicenseState), splitStatus[0]);

            plData.ProductLicense.Product.ID = (uint)m_CommLink.GetProductID(ProductLicenseProductComboBox.SelectedItem.ToString());
            plData.ProductLicense.ProductLicenseType = m_CommLink.GetProductSpec(plData.ProductLicense.Product.ID).productLicType.TVal;
            plData.ProductLicense.Product.AppInstance = (uint)((plData.ProductLicense.Status == ProductLicenseState.AddOn) ? 0 : 1);
            string[] versionNum = productLicenseVersionMaskedTextBox.Text.Split(".".ToCharArray());
            plData.ProductLicense.Product.Version = new LicenseVersion(UInt32.Parse(versionNum[0]), UInt32.Parse(versionNum[1]));
            plData.ProductLicense.ExpirationDate = CurrentExpirationDate.AddDays(Int32.Parse(expDateTextBox.Text));
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
            if (licData.LicInfo.IsStandardLicenseType)
            {               
                destinationTextBox.Text = licData.LicInfo.DestName;
                licTypeComboBox.Items.Add(LicenseServerType.Perpetual);
                licTypeComboBox.Items.Add(LicenseServerType.Subscription);                
                licTypeComboBox.SelectedItem = licData.LicInfo.LicType;
            }
            else
            {   //secondary keys only allow comment
                Service<ICreator>.Use((client) =>
                {   
                    //increment index for test/dev and disaster recovery
                    Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType licType = (Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType)Enums.GetLicenseServerType(licData.LicInfo.LicType);
                    licData.LicInfo.LicInfo.softwareLicTypeIndex.TVal = (uint)client.GetLicenseCountByType(licData.LicInfo.CustID, licData.LicInfo.DestID, licData.LicInfo.GroupID, licType) + 1;
                });
                destinationTextBox.Text = licData.LicInfo.DestName;
                licTypeComboBox.Items.Add(licData.LicInfo.LicType);                
                licTypeComboBox.SelectedItem = licData.LicInfo.LicType;
                //licTypeComboBox.DropDownStyle = ComboBoxStyle.Simple;
                //licTypeComboBox.Enabled = false;
            }
            //license name
            licNameTextBox.Text = licData.LicInfo.Name;
        }

        private void SaveLicenseTabPage(LicenseDialogData licData)
        {
            //TODO : validate that the license does not already exist
            licData.LicInfo.LicType = (LicenseServerType)Enum.Parse(typeof(LicenseServerType), licTypeComboBox.SelectedItem.ToString());
            licData.LicInfo.Comments = licDescriptTextBox.Text;
        }
        #endregion

        #region Token Methods
        private void LoadTokenTabPage(TokenDialogData tokenData)
        {
            selectedObject = tokenData.LicInfo as Object;
            //hardware token get value from database
            btnOk.Enabled = true;
            topPanel.Controls.Clear();
            tokenGroupBox.Parent = topPanel;
            //enabled when new token, ttNone passed in
            tokenTypeComboBox.Enabled = (tokenData.Token.TokenType != Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttNone) ? false : true;
            //load token
            if (tokenTypeComboBox.Enabled)
            {   //new token
                foreach (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs token in tokenData.LicInfo.LicInfo.licVerificationAttribs.TVal.validationTokenList.TVal)
                {
                    tokenData.ValidTokenList.Remove(token.tokenType);
                }
                if (tokenData.ValidTokenList.Count == 0)
                    tokenData.ValidTokenList.Add(Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttNone);

            }
            else
            {   //old token
                foreach (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs token in tokenData.LicInfo.LicInfo.licVerificationAttribs.TVal.validationTokenList.TVal)
                {
                    if (token.tokenType.TVal.Equals(tokenData.Token.TokenType))
                    {
                        tokenData.ValidTokenList.Clear();
                        tokenData.ValidTokenList.Add(token.tokenType.TVal);
                        tokenValueTextBox.Text = token.tokenValue.TVal;
                        break;
                    }
                }
            }
            tokenTypeComboBox.DataSource = tokenData.ValidTokenList;
            tokenTypeComboBox.SelectedItem = tokenTypeComboBox.Items[0];
        }

        private void SaveTokenTabPage(TokenDialogData tokenData)
        {
            //new case -> create new token and then add to tokendata            
            if (tokenData.Token.TokenType == Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttNone)
            {
                Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs newToken = new Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs();
                newToken.tokenType.TVal = (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType)tokenTypeComboBox.SelectedItem;
                newToken.tokenValue.TVal = tokenValueTextBox.Text;
                tokenData.LicInfo.LicInfo.licVerificationAttribs.TVal.validationTokenList.TVal.Add(newToken);
                tokenData.Token.ValidationToken = newToken;
            }
            else
            { //edit case -> find token then modify tokendata
                foreach (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs token in tokenData.LicInfo.LicInfo.licVerificationAttribs.TVal.validationTokenList.TVal)
                {
                    if (token.tokenType.TVal.Equals((Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType)tokenTypeComboBox.SelectedItem))
                    {
                        token.tokenValue.TVal = tokenValueTextBox.Text;
                        tokenData.Token.ValidationToken = token;
                        break;
                    }
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
            modNameComboBox.Enabled = false;
            //edit module
            if (moduleData.ModuleNames.Count != 0)
            {               
                string moduleName = moduleData.ModuleNames[0];
                modNameComboBox.Items.Add(moduleName);
                modNameComboBox.SelectedIndex = 0;
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
            int modID, totalModAppInstance;
            foreach (string modName in moduleData.ModuleNames)
            {
                foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs storedMod in moduleData.ProductLicense.Product.ModuleList.TVal)
                {
                    modID = m_CommLink.GetModuleID(moduleData.ProductLicense.Product.ID, modName);
                    if(modID >= 0)
                    {
                        if (storedMod.contractNumber.TVal.Equals(moduleData.ProductLicense.ID))
                        {
                            if (storedMod.moduleID.TVal.Equals((uint)modID))
                            {
                                storedMod.moduleValue.TVal = (uint)modValueNumericUpDown.Value;
                                //if client type set product connections to total
                                //call function to get product spec then check to see if it is a client
                                totalModAppInstance = GetTotalModuleAppInstance(moduleData, storedMod);
                                if (storedMod.moduleValue.TVal > 0)
                                {                                       
                                    if (totalModAppInstance == 0)
                                        storedMod.moduleAppInstance.TVal = 1;
                                    else //>1
                                    {
                                        if (totalModAppInstance > 1)
                                        {
                                            if (IsClientType(moduleData.ProductLicense.ProductName) && storedMod.moduleState.TVal != Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msAddOn)
                                                storedMod.moduleAppInstance.TVal = moduleData.ProductLicense.Product.AppInstance;                                            
                                            else
                                                storedMod.moduleAppInstance.TVal = (uint)((moduleData.ProductLicense.Status == ProductLicenseState.AddOn) ? 0 : 1);
                                        }
                                        else
                                        {
                                            if (storedMod.moduleState.TVal == Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msAddOn)
                                                storedMod.moduleAppInstance.TVal = 0;
                                            else
                                            {
                                                storedMod.moduleAppInstance.TVal = 1;
                                                DecrementAddOnModuleAppInstance(moduleData, storedMod);
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    //if decrement std then increment add-on
                                    //if decrement add-on don't decrement appinstance unless it is 1
                                    storedMod.moduleAppInstance.TVal = 0;
                                    if (storedMod.moduleState.TVal != Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msAddOn)
                                    {
                                        if (GetTotalAddOnModuleAppInstance(moduleData, storedMod) == 0)
                                            IncrementAddOnModuleAppInstance(moduleData, storedMod);
                                    }
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }

        private uint GetTotalAddOnModuleAppInstance(ModuleDialogData moduleData, Lic_PackageAttribs.Lic_ModuleInfoAttribs storedMod)
        {
            uint totalValue = 0;
            string stdProductLicense = "";
            IList<string> addOnOrders = null;
            Service<ICreator>.Use((client) =>
            {
                stdProductLicense = (storedMod.moduleState.TVal != Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msAddOn) ? moduleData.ProductLicense.ID : moduleData.ProductLicense.ParentID;
                    addOnOrders = client.GetAddOnProductLicenses(stdProductLicense);
            });
            foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs modRec in moduleData.ProductLicense.Product.ModuleList.TVal)
            {
                if (modRec.moduleID.TVal.Equals(storedMod.moduleID.TVal) && (addOnOrders.Contains(modRec.contractNumber.TVal)))
                    totalValue += modRec.moduleAppInstance.TVal;
            }
            return totalValue;
        }

        private int GetTotalModuleAppInstance(ModuleDialogData moduleData, Lic_PackageAttribs.Lic_ModuleInfoAttribs storedMod)
        {
            uint totalValue = 0;
            string stdProductLicense = "";
            IList<string> addOnOrders = null;
            Service<ICreator>.Use((client) =>
            {
                stdProductLicense = (storedMod.moduleState.TVal != Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msAddOn) ? moduleData.ProductLicense.ID : moduleData.ProductLicense.ParentID;
                addOnOrders = client.GetAddOnProductLicenses(stdProductLicense);
            });
            foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs modRec in moduleData.ProductLicense.Product.ModuleList.TVal)
            {
                if (modRec.moduleID.TVal.Equals(storedMod.moduleID.TVal) && (addOnOrders.Contains(modRec.contractNumber.TVal) || modRec.contractNumber.TVal.Equals(stdProductLicense)))
                    totalValue += modRec.moduleAppInstance.TVal;
            }
            return (int)totalValue;
        }

        //standard order moduledata does not know add-on orders need to retrieve from database?
        private void DecrementAddOnModuleAppInstance(ModuleDialogData moduleData, Lic_PackageAttribs.Lic_ModuleInfoAttribs module)
        {
            IList<string> addOnOrders = null;
            Service<ICreator>.Use((client) =>
            {
                addOnOrders = client.GetAddOnProductLicenses(module.contractNumber);
            });
            if(addOnOrders != null)
            {
                if(addOnOrders.Count > 0)
                {
                    foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs storedMod in moduleData.ProductLicense.Product.ModuleList.TVal)
                    {
                        if (storedMod.moduleID.TVal == module.moduleID.TVal &&
                            addOnOrders.Contains(storedMod.contractNumber.TVal))
                            storedMod.moduleAppInstance.TVal = 0;                                                                          
                    }
                }
            }
        }

        private void IncrementAddOnModuleAppInstance(ModuleDialogData moduleData, Lic_PackageAttribs.Lic_ModuleInfoAttribs module)
        {
            //increment first add-on found
            IList<string> addOnOrders = null;
            Service<ICreator>.Use((client) =>
            {
                addOnOrders = client.GetAddOnProductLicenses(module.contractNumber);
            });
            if (addOnOrders != null)
            {
                if (addOnOrders.Count > 0)
                {
                    foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs storedMod in moduleData.ProductLicense.Product.ModuleList.TVal)
                    {
                        if (storedMod.moduleID.TVal == module.moduleID.TVal &&
                            addOnOrders.Contains(storedMod.contractNumber.TVal))
                        {
                            if(storedMod.moduleValue.TVal > 0)
                                storedMod.moduleAppInstance.TVal = 1;
                            break;
                        }
                    }
                }
            }
        }
        #endregion

        #region LicenseTabPage Events

        private void licTypeComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {            
            LicenseServerProperty licInfo = selectedObject as LicenseServerProperty;
            licInfo.LicType = (LicenseServerType)Enum.Parse(typeof(LicenseServerType), licTypeComboBox.SelectedItem.ToString());
            Service<ICreator>.Use((client) => 
            {
                if (licInfo.IsStandardLicenseType)                   
                    licInfo.GroupID = client.GetNextGroupID(licInfo.CustID, licInfo.DestID);
                Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType licType = (Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType)Enums.GetLicenseServerType(licInfo.LicType);                
                licInfo.LicInfo.softwareLicTypeIndex.TVal = (uint)(client.GetLicenseCountByType(licInfo.CustID, licInfo.DestID, licInfo.GroupID, licType) + 1);
            });
            licNameTextBox.Text = licInfo.Name;            
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
            //    Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType selectedType = (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType)tokenTypeComboBox.SelectedItem;
            //    if (client.TokenExists(selectedLicense.CustID, (byte)selectedType, tokenValueTextBox.Text))
            //    {
            //        m_Validated = false;
            //        this.tokenValueTextBox.Select(0, this.tokenValueTextBox.Text.Length);

            //        // Set the ErrorProvider error with the text to display.
            //        //errorProvider1.SetError(this.tokenValueTextBox, "Validation token already exists for this customer!");
            //        MessageBox.Show("Validation token already exists for this customer!", "Validation Token Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            //    }
            //});
            Cursor.Current = Cursors.Default; 
        }

        private string GetNextAvailableHardwareKey(string customerName)
        {
            CustomerTable custRec = null;
            uint tokenValue = 0;
            Service<ICreator>.Use((client) =>
            {
                custRec = client.GetCustomer(customerName, false);
                tokenValue = client.GetNextHardwareTokenValue((uint)custRec.SCRnumber);
            });
            return string.Format("{0:x4}-{1:x4}", custRec.SCRnumber, tokenValue);
        }

        private void tokenTypeComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {   
            LicenseServerProperty selectedOrder = selectedObject as LicenseServerProperty;      
            keyNameListView.Visible = false;
            availableHWKeyLabel.Visible = false;
            tokenValueTextBox.Enabled = false;
            browseTokenFileButton.Visible = false;
            btnOk.Enabled = false;
            if (tokenTypeComboBox.Enabled)
                tokenValueTextBox.Text = "";
            Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType selectedType = (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType)tokenTypeComboBox.SelectedItem;
            if (selectedType == Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID)
            {   //Edit ttHardwareID has value, skip 
                keyNameListView.Visible = true;
                availableHWKeyLabel.Visible = true;
                if (tokenValueTextBox.Text.Length == 0)
                {   //New ttHardwareID needs to retrieve next available value
                    keyNameListView.Items.Clear();
                    Service<ICreator>.Use((client) => 
                    {
                        IList<TokenTable> tokenList = client.GetAvailableHardwareTokensByCustID(selectedOrder.CustID);
                        foreach (TokenTable token in tokenList)
                        {                       
                            keyNameListView.Items.Add(token.TokenValue);
                        }
                    });
                }
                if (keyNameListView.Items.Count > 0)
                {
                    keyNameListView.Focus();
                    keyNameListView.Items[0].Selected = true;
                }
            }
            else
            {                
                if (selectedType != Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttNone)
                {
                    tokenValueTextBox.Enabled = true;
                    browseTokenFileButton.Visible = true;
                }
            }                
        }

        private void tokenValueTextBox_TextChanged(object sender, EventArgs e)
        {
            btnOk.Enabled = tokenValueTextBox.Text.Length > 0;
        }
        
        private IList<SolimarLicenseProtectionKeyInfo> LoadKeyNameListBox()
        {
            IList<SolimarLicenseProtectionKeyInfo> pkeyList = null;
            Service<ICreator>.Use((client) => 
            {
                pkeyList = client.KeyEnumerate();
            });
            return pkeyList;
        }

        private void tokenValueTextBox_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter || e.KeyCode == Keys.Return)
            {
                ValidateTokenForm();
                if (m_Validated)                
                    DialogResult = DialogResult.OK;
            }
            else if (e.KeyCode == Keys.Escape)
            {
                DialogResult = DialogResult.Cancel;
            }
        }

        private void tokenTypeComboBox_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Escape)
            {
                DialogResult = DialogResult.Cancel;
            }
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
                    if (productSpec.productName.TVal.Equals(selectedProduct) && !productSpec.productName.TVal.Contains("Test"))
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
                int productID = CreatorForm.s_CommLink.GetProductID(selectedProduct);
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

        private string GetCSVTokenAlias(string tokenType)
        {
            string retVal = "";
            switch (tokenType)
            {
                case "ttComputerName"://computername
                    retVal = "Computer Name";
                    break;
                case "ttBiosNumber"://bios
                    retVal = "Bios Serial Number";
                    break;
                case "ttMacAddress"://macaddress
                    retVal = "Mac Address";
                    break;
                default:
                    break;
            }
            return retVal;
        }

        private void browseTokenFileButton_Click(object sender, EventArgs e)
        {
            if (this.browseCSVOpenFileDialog.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    using (StreamReader sr = new StreamReader(browseCSVOpenFileDialog.FileName))
                    {
                        String line, importedValue, selectedToken;
                        selectedToken = GetCSVTokenAlias(tokenTypeComboBox.SelectedItem.ToString());
                        while ((line = sr.ReadLine()) != null)
                        {
                            if (line.Contains(selectedToken))
                            {
                                //strip the line for value and load it into textbox
                                importedValue = line.Remove(0, line.IndexOf(",")+1);
                                importedValue = importedValue.Trim(new Char[] { '"' });
                                tokenValueTextBox.Text = importedValue;
                                break;
                            }
                        }
                        sr.Close();
                    }
                }
                catch (Exception)
                {
                }
            }
        }

        private void keyNameListView_SelectedIndexChanged(object sender, EventArgs e)
        {
            if(keyNameListView.SelectedItems.Count > 0)
                tokenValueTextBox.Text = keyNameListView.SelectedItems[0].Text;
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
        private ValidationProperty m_Token;
        private LicenseServerProperty m_LicInfo;
        private List<Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType> m_ValidTokenTypes;

        #region Constructors

        public TokenDialogData(ValidationProperty token, LicenseServerProperty licInfo)
        {
            m_Token = token;
            m_LicInfo = licInfo;
            m_ValidTokenTypes = new List<Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType>();
            m_ValidTokenTypes.Add(Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID);
            m_ValidTokenTypes.Add(Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttComputerName);
            m_ValidTokenTypes.Add(Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttMacAddress);
            m_ValidTokenTypes.Add(Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttBiosSerialNumber);
            m_ValidTokenTypes.Add(Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttLicenseCode);
        }
        #endregion

        #region Properties

        public ValidationProperty Token
        {
            get { return this.m_Token; }
            set { this.m_Token = value; }
        }

        public LicenseServerProperty LicInfo
        {
            get { return this.m_LicInfo; }
            set { this.m_LicInfo = value; }
        }

        public List<Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType> ValidTokenList
        {
            get { return this.m_ValidTokenTypes; }
            set { this.m_ValidTokenTypes = value; }
        }

        #endregion
    }
    #endregion

    #region ModuleDialogData class
    public class ModuleDialogData : Shared.VisualComponents.DialogData
    {
        private List<string> _moduleNames;
        private ProductLicenseProperty _plData;
        #region Constructors

        public ModuleDialogData(List<string> moduleNames, ProductLicenseProperty plData)
        {
            _moduleNames = moduleNames;
            _plData = plData;
        }
        #endregion

        #region Properties

        public List<string> ModuleNames
        {
            get { return this._moduleNames; }
            set { this._moduleNames = value; }
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
        private LicenseServerProperty _LicenseInfo;

        #region Constructors

        public ProductLicenseDialogData(ProductLicenseProperty plInfo, LicenseServerProperty licInfo)
        {
            _plInfo = plInfo;
            _LicenseInfo = licInfo;
        }
        #endregion

        #region Properties

        public ProductLicenseProperty ProductLicense
        {
            get { return this._plInfo; }
            set { this._plInfo = value; }
        }

        public LicenseServerProperty LicenseInfo
        {
            get { return this._LicenseInfo; }
            set { this._LicenseInfo = value; }
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
