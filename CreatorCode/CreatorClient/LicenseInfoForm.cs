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
            else if (e.Data is OrderDialogData)
                LoadOrderTabPage(e.Data as OrderDialogData);
            else
                LoadHardwareKeyTabPage();            
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
            else if (e.Data is OrderDialogData)
                SaveOrderTabPage(e.Data as OrderDialogData);
            else
                SaveHardwareKeyTabPage();
        }
        #endregion

        #region Order Methods

        void EnableOrderTabPage(bool bEnable)
        {
            orderProductComboBox.Enabled = bEnable;
            orderVersionMaskedTextBox.Enabled = bEnable;
            orderTypeComboBox.Enabled = bEnable;
            orderDescriptionTextBox.Enabled = bEnable;
            btnOk.Enabled = bEnable;
        }
        void LoadOrderTabPage(OrderDialogData orderData)
        {
            selectedObject = orderData as Object;
            //licInfoTabControl.TabPages.Add(orderTabPage);
            topPanel.Controls.Clear();
            orderGroupBox.Parent = topPanel;
            orderNumberTextBox.Text = orderData.OrderInfo.OrderNumber;
            //EnableOrderTabPage(true);
            List<OrderTable> orders;
            Service<ICreator>.Use((client) => 
            {
                //trial -> zero options : 1 order
                //perm -> add-on available : 1 order
                //perm, add-on -> zero available : 2 order                
                if (orderData.LicenseInfo.LicType != (Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltTestDev))
                {
                    //standard,failover,disaster             
                    foreach (Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in m_CommLink.m_softwareSpec.productSpecMap.TVal.Values)
                    {
                        if (!productSpec.productName.TVal.Contains("Test"))
                        {
                            //don't include modules that already exist?
                            orders = client.GetOrdersEqualsProduct(orderData.OrderInfo.LicenseName, productSpec.productName.TVal);
                            //0 order => product hasnt been added
                            if (productSpec.productLicType.TVal.Equals(Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.TProductLicenseType.pltClient) &&
                                orders.Count > 0)
                                continue;                                
                            orderProductComboBox.Items.Add(productSpec.productName);
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
                            string licenseBase = string.Format("{0:x4}-{1:x3}-{2:x4}-S01", orderData.LicenseInfo.CustID, orderData.LicenseInfo.DestID, orderData.LicenseInfo.GroupID);
                            int pos = productSpec.productName.TVal.IndexOf("Test");
                            //1) call twice, once with product, then product test
                            //2) call once with product and add only when 1 order(standard) found
                            orders = client.GetOrdersEqualsProduct(licenseBase, productSpec.productName.TVal.Remove(pos - 1));
                            if (orders.Count == 1)
                            {
                                //0 order => product hasnt been added                     
                                orders = client.GetOrdersEqualsProduct(orderData.OrderInfo.LicenseName, productSpec.productName);
                                if (orders.Count == 0)
                                    orderProductComboBox.Items.Add(productSpec.productName);
                            }
                        }
                    }
                }
            });
            if (orderProductComboBox.Items.Count == 0)
            {
                orderProductComboBox.Items.Add("No Products Available");
                //EnableOrderTabPage(false);
            }
            orderProductComboBox.SelectedIndex = 0;
            expDateTextBox.Text = "30";
            //load product combobox based on type of license
        }
        
        void SaveOrderTabPage(OrderDialogData orderData)
        {
            string[] splitOrderStatus = orderTypeComboBox.SelectedItem.ToString().Split(":".ToCharArray());
            //can only be one or two
            if (splitOrderStatus.Count() > 1)
            {
                orderData.OrderInfo.OrderStatus = (Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState)Enum.Parse(typeof(Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState), splitOrderStatus[0].Trim());
                orderData.OrderInfo.ParentOrderNumber = splitOrderStatus[1].Trim();
            }
            else
                orderData.OrderInfo.OrderStatus = (Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState)Enum.Parse(typeof(Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState), splitOrderStatus[0]);
            
            orderData.OrderInfo.Product.ID = (uint)m_CommLink.GetProductID(orderProductComboBox.SelectedItem.ToString());
            orderData.OrderInfo.ProductLicenseType = m_CommLink.GetProductSpec(orderData.OrderInfo.Product.ID).productLicType.TVal;
            orderData.OrderInfo.Product.AppInstance = (uint)((orderData.OrderInfo.OrderStatus == Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msAddOn) ? 0 : 1);
            string[] versionNum = orderVersionMaskedTextBox.Text.Split(".".ToCharArray());
            orderData.OrderInfo.Product.Version = new LicenseVersion(UInt32.Parse(versionNum[0]),UInt32.Parse(versionNum[1]));
            orderData.OrderInfo.ExpirationDate = DateTime.Today.AddDays(Int32.Parse(expDateTextBox.Text));
            orderData.OrderInfo.Description = orderDescriptionTextBox.Text;            
        }
        #endregion

        #region License Methods

        private void LoadLicenseTabPage(LicenseDialogData licData)
        {
            string selectedDestName;
            //licInfoTabControl.TabPages.Add(LicenseTabPage);
            topPanel.Controls.Clear();
            licenseGroupBox.Parent = topPanel;
            //need to get next available value from DB for destination ID and group ID.
            selectedObject = licData.LicInfo as Object;
            if (licData.LicInfo.IsStandardLicenseType)
            {               
                Service<ICreator>.Use((client) =>
                {                
                    IList<DestinationNameTable> destNames = client.GetDestNamesByCustID((int)licData.LicInfo.CustID).OrderBy(c => c.DestID).ToList();
                    foreach (DestinationNameTable dn in destNames)
                    {
                        destNameComboBox.Items.Add(dn.DestName);
                    }
                    if (destNameComboBox.Items.Count == 0) //add base customer
                    {
                        selectedDestName = client.GetCustomer(licData.LicInfo.CustID.ToString(), false).SCRname;
                        DestinationNameTable baseDestination = new DestinationNameTable();
                        baseDestination.CustID = (int)licData.LicInfo.CustID;
                        baseDestination.DestID = (int)licData.LicInfo.DestID;
                        baseDestination.DestName = selectedDestName;
                        client.CreateDestinationName(baseDestination);
                        destNameComboBox.Items.Add(selectedDestName);
                    }
                });
                licTypeComboBox.Items.Add(Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltStandard);
                licTypeComboBox.Items.Add(Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltStandardSubscription);                
                licTypeComboBox.SelectedItem = licData.LicInfo.LicType;
            }
            else
            {   //secondary keys only allow comment
                Service<ICreator>.Use((client) =>
                {   
                    selectedDestName = client.GetCustomer(licData.LicInfo.CustID.ToString(), false).SCRname;
                    destNameComboBox.Items.Add(selectedDestName);
                    destNameComboBox.DropDownStyle = ComboBoxStyle.Simple;
                    destNameComboBox.Enabled = false;
                    //increment index for test/dev and disaster recovery
                    licData.LicInfo.LicInfo.softwareLicTypeIndex.TVal = (uint)client.GetLicenseCountByType(licData.LicInfo.CustID, licData.LicInfo.DestID, licData.LicInfo.GroupID, licData.LicInfo.LicType) + 1;
                });
                licTypeComboBox.Items.Add(licData.LicInfo.LicType);                
                licTypeComboBox.SelectedItem = licData.LicInfo.LicType;
                licTypeComboBox.DropDownStyle = ComboBoxStyle.Simple;
                licTypeComboBox.Enabled = false;
            }
            //User Options for destination names
            destNameComboBox.Items.Add("<New...>");
            destNameComboBox.Items.Add("<Edit...>");
            //license name
            licNameTextBox.Text = licData.LicInfo.Name;
            //select destination name
            destNameComboBox.SelectedIndex = 0;
            //initialize comments
            licDescriptTextBox.Text = licData.LicInfo.Comments;
        }

        private void SaveLicenseTabPage(LicenseDialogData licData)
        {
            //TODO : validate that the license does not already exist
            licData.LicInfo.LicType = (Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType)licTypeComboBox.SelectedItem;
            licData.LicInfo.Comments = licDescriptTextBox.Text;
        }
        #endregion

        #region Token Methods
        private void LoadTokenTabPage(TokenDialogData tokenData)
        {
            selectedObject = tokenData.LicInfo as Object;
            //hardware token get value from database
            btnOk.Enabled = true;
            //licInfoTabControl.TabPages.Add(TokenTabPage);
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
                modID = (uint)m_CommLink.GetModuleID(moduleData.OrderData.Product.ID, moduleName);
                unlimitedValue = (uint)m_CommLink.GetUnlimitedModuleValue(moduleData.OrderData.Product.ID, modID);
                //need total product value 
                Service<ICreator>.Use((client) => 
                {
                    LicenseTable licRec = client.GetLicenseByName(moduleData.OrderData.LicenseName, false);
                    Lic_PackageAttribs licPackage = new Lic_PackageAttribs();
                    licPackage.Stream = licRec.LicenseInfo;                        
                    foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs product in licPackage.licLicenseInfoAttribs.TVal.productList.TVal)
                    {
                        if (product.productID.TVal.Equals(moduleData.OrderData.Product.ID))
                        {
                            foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs modRec in product.moduleList.TVal)
                            {
                                if (modRec.moduleID.TVal.Equals(modID) && (modRec.contractNumber.TVal.Equals(moduleData.OrderData.OrderNumber) || modRec.contractNumber.TVal.Equals(moduleData.OrderData.ParentOrderNumber)))
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
                foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs module in moduleData.OrderData.Product.ModuleList.TVal)
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

        //DecrementAddOnOrderAppInstance(module, contractnumber)
        
        //if module is from a std order bump up app inst when > 0
        //if module is from a std order w/app inst 0 and there exists an add-on order with app inst 1, decrement add-on
        //if module is from add-on and std order & any add-on has appinst 0 then +1 appinst for add-on
        private void SaveModuleTabPage(ModuleDialogData moduleData)
        {
            int modID;
            foreach (string modName in moduleData.ModuleNames)
            {
                foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs storedMod in moduleData.OrderData.Product.ModuleList.TVal)
                {
                    modID = m_CommLink.GetModuleID(moduleData.OrderData.Product.ID, modName);
                    if(modID >= 0)
                    {
                        if (storedMod.contractNumber.TVal.Equals(moduleData.OrderData.OrderNumber))
                        {
                            if (storedMod.moduleID.TVal.Equals((uint)modID))
                            {
                                storedMod.moduleValue.TVal = (uint)modValueNumericUpDown.Value;
                                if (storedMod.moduleValue.TVal > 0)
                                {
                                    storedMod.moduleAppInstance.TVal = (uint)((moduleData.OrderData.OrderStatus == Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msAddOn) ? 0 : 1);
                                    //if (GetTotalModuleAppInstance(moduleData, storedMod) == 0)
                                    //    storedMod.moduleAppInstance.TVal = 1;
                                    //else
                                    //{
                                    //    //std order
                                    //    if(moduleData.OrderData.ParentOrderNumber.Length == 0)
                                    //    {

                                    //    }
                                    //    //if add-on type
                                    //    //total is already > 0, don't do anything
                                    //    //if std type
                                    //    //total is already > 0, set to 1
                                    //    //decrement any modules with parent order number = to standard order
                                    //    //decrement any add-ons
                                    //    //increment std
                                    //    if(moduleData.OrderData.ParentOrderNumber
                                    //    storedMod.moduleAppInstance.TVal = (uint)((moduleData.OrderData.OrderStatus == Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msAddOn) ? 0 : 1);
                                    //}

                                    //get total value for a std and its add-ons
                                    //if current order is a std and appinst = 1, decrement the add-on  
                                    //if current order is add-on and app inst = 1, don't do anything
                                    //                               app inst = 0, then increment add-on                                                                     
                                }
                                else
                                {
                                    //if decrement std then increment add-on
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }

        //private uint GetTotalModuleAppInstance(ModuleDialogData moduleData,Lic_PackageAttribs.Lic_ModuleInfoAttribs storedMod)
        //{
        //    uint totalValue = 0;
        //    foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs modRec in  moduleData.OrderData.Product.ModuleList.TVal)
        //    {
        //        if (modRec.moduleID.TVal.Equals(storedMod) && (modRec.contractNumber.TVal.Equals(moduleData.OrderData.OrderNumber) || modRec.contractNumber.TVal.Equals(moduleData.OrderData.ParentOrderNumber)))
        //        {
        //            totalValue += modRec.moduleAppInstance.TVal;
        //            break;
        //        }
        //    }
        //    return totalValue;             
        //}

        //private uint GetTotalAddOnModuleAppInstance(ModuleDialogData moduleData, Lic_PackageAttribs.Lic_ModuleInfoAttribs storedMod)
        //{
        //    uint totalValue = 0;
        //    foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs modRec in moduleData.OrderData.Product.ModuleList.TVal)
        //    {
        //        if (modRec.moduleID.TVal.Equals(storedMod) && (modRec.contractNumber.TVal.Equals(moduleData.OrderData.OrderNumber) || modRec.contractNumber.TVal.Equals(moduleData.OrderData.ParentOrderNumber)))
        //        {
        //            totalValue += modRec.moduleAppInstance.TVal;
        //            break;
        //        }
        //    }
        //    return totalValue;  
        //}

        #endregion

        #region Hardware Key Methods
        private void LoadHardwareKeyTabPage()        
        {
            List<CustomerTable> custList = null;
            //licInfoTabControl.TabPages.Add(HardwareKeyTabPage);            
            topPanel.Controls.Clear();
            hardwareKeyGroupBox.Parent = topPanel;
            Service<ICreator>.Use((client) =>
            {
                custList = client.GetAllCustomers("", false);
            });
            foreach (CustomerTable cust in custList)
            {
                CustomerComboBox.Items.Add(cust.SCRname);
            }
            CustomerComboBox.SelectedIndex = 0;
            RefreshKeyList();
        }
        private void SaveHardwareKeyTabPage()
        {            
            string[] value = KeyValueTextBox.Text.Split(new Char[] { '-' });
            KeyProgramVerification(AvailableKeysListBox.SelectedItem.ToString(),
                                   Int32.Parse(value[0], System.Globalization.NumberStyles.HexNumber),
                                   Int32.Parse(value[1], System.Globalization.NumberStyles.HexNumber));
            TokenTable newHardwareKey = new TokenTable();
            newHardwareKey.TokenType = (byte)Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID;
            newHardwareKey.TokenValue = KeyValueTextBox.Text;
            newHardwareKey.LicenseID = -1;
            newHardwareKey.CustID = Int32.Parse(value[0], System.Globalization.NumberStyles.HexNumber);
            Service<ICreator>.Use((client) =>
            {    
                client.CreateToken(newHardwareKey);
            });
        }
        #endregion

        #region LicenseTabPage Events

        private void licTypeComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {            
            LicenseProperty licInfo = selectedObject as LicenseProperty;            
            licInfo.LicType = (Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType)licTypeComboBox.SelectedItem;
            Service<ICreator>.Use((client) => 
            {
                if (licInfo.IsStandardLicenseType)                   
                    licInfo.GroupID = client.GetNextGroupID(licInfo.CustID, licInfo.DestID);                               
                licInfo.LicInfo.softwareLicTypeIndex.TVal = (uint)(client.GetLicenseCountByType(licInfo.CustID, licInfo.DestID, licInfo.GroupID, licInfo.LicType) + 1);
            });
            licNameTextBox.Text = licInfo.Name;
        }

        //valid, existing dest name chosen from list in box
        private void destNameComboBox_SelectionChangeCommitted(object sender, EventArgs e)
        {
            LicenseProperty licInfo = selectedObject as LicenseProperty;
            Service<ICreator>.Use((client) => 
            {
                if (destNameComboBox.Text == "<New...>")
                {
                    using (NewDestinationName dlg = new NewDestinationName())
                    {
                        if (dlg.ShowDialog(this) == DialogResult.OK)
                        {   //create the new destination name for this customer                                                    
                            DestinationNameTable newDestName = new DestinationNameTable();
                            newDestName.CustID = (int)licInfo.CustID;
                            newDestName.DestID = (int)client.GetNextDestinationID(licInfo.CustID);
                            newDestName.DestName = dlg.DestinationName;
                            client.CreateDestinationName(newDestName);
                            licInfo.DestID = (uint)newDestName.DestID;
                            licInfo.DestName = newDestName.DestName;
                            destNameComboBox.Items.Insert(newDestName.DestID, dlg.DestinationName);
                            destNameComboBox.SelectedItem = dlg.DestinationName;
                        }
                        else
                        {
                            destNameComboBox.SelectedIndex = 0;
                        }
                    }
                }
                else if (destNameComboBox.Text == "<Edit...>")
                {
                    using (EditDestinationName dlg = new EditDestinationName(licInfo, destNameComboBox.Items))
                    {
                        dlg.ShowDialog(this);
                        if (dlg.Modified)
                        {
                            destNameComboBox.Items.Clear();
                            IList<DestinationNameTable> destTables = client.GetDestNamesByCustID((int)licInfo.CustID).OrderBy(c => c.DestID).ToList();
                            foreach(DestinationNameTable table in destTables)
                            {
                                destNameComboBox.Items.Add(table.DestName);                                
                            }
                            destNameComboBox.Items.Add("<New...>");
                            destNameComboBox.Items.Add("<Edit...>");
                        }
                        licInfo.DestID = (uint)dlg.SelectedDestinationID;
                        licInfo.DestName = dlg.SelectedDestinationName; 
                        destNameComboBox.SelectedItem = dlg.SelectedDestinationName;
                    }
                }
                else
                {
                    DestinationNameTable destName = client.GetDestinationID((int)licInfo.CustID, destNameComboBox.SelectedItem.ToString());
                    if (destName != null)
                    {
                        //set current license info to destid
                        licInfo.DestID = (uint)destName.DestID;
                        licInfo.DestName = destNameComboBox.SelectedItem.ToString();
                    }
                }
                licInfo.GroupID = client.GetNextGroupID(licInfo.CustID, licInfo.DestID);
                var license = client.GetLicenseByName(licInfo.Name, false);
                licTypeComboBox.Items.Clear();
                if (license != null || licInfo.GroupID > 0)
                {   //restore all license types if license with licInfo exists
                    foreach (var item in Enum.GetValues(typeof(Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType)))
                    {
                        licTypeComboBox.Items.Add(item);
                    }
                }
                else
                {   //allow only standard license if license with licInfo doesn't exist
                    licTypeComboBox.Items.Add(Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltStandard);
                }
            });
            licTypeComboBox.SelectedIndex = 0;   
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

        private void keyNameListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            tokenValueTextBox.Text = keyNameListBox.SelectedItem.ToString();
        }

        private void ValidateTokenForm()
        {
            Cursor.Current = Cursors.WaitCursor;
            m_Validated = true;
            Service<ICreator>.Use((client) =>
            {
                LicenseProperty selectedLicense = selectedObject as LicenseProperty;
                Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType selectedType = (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType)tokenTypeComboBox.SelectedItem;
                if (client.TokenExists(selectedLicense.CustID, (byte)selectedType, tokenValueTextBox.Text))
                {
                    m_Validated = false;
                    this.tokenValueTextBox.Select(0, this.tokenValueTextBox.Text.Length);

                    // Set the ErrorProvider error with the text to display.
                    errorProvider1.SetError(this.tokenValueTextBox, "Validation token already exists for this customer!");
                }
            });
            Cursor.Current = Cursors.Default; 
        }

        private void CustomerComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            Service<ICreator>.Use((client) => 
            {
                CustomerTable custRec = client.GetCustomer(CustomerComboBox.SelectedItem.ToString(), false);
                uint tokenValue = client.GetNextHardwareTokenValue((uint)custRec.SCRnumber);
                KeyValueTextBox.Text = string.Format("{0:x4}-{1:x4}", custRec.SCRnumber, tokenValue);
            });

        }

        private void tokenTypeComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {   
            LicenseProperty selectedOrder = selectedObject as LicenseProperty;      
            //keyNameGroupBox.Visible = false;
            keyNameListBox.Visible = false;
            availableHWKeyLabel.Visible = false;
            tokenValueTextBox.Enabled = false;
            browseTokenFileButton.Visible = false;
            btnOk.Enabled = false;
            if (tokenTypeComboBox.Enabled)
                tokenValueTextBox.Text = "";
            Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType selectedType = (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType)tokenTypeComboBox.SelectedItem;
            if (selectedType == Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID)
            {   //Edit ttHardwareID has value, skip 
                if (tokenValueTextBox.Text.Length == 0)
                {   //New ttHardwareID needs to retrieve next available value
                    keyNameListBox.Items.Clear();
                    Service<ICreator>.Use((client) => 
                    {
                        IList<TokenTable> tokenList = client.GetAvailableHardwareTokensByCustID(selectedOrder.CustID);
                        foreach (TokenTable token in tokenList)
                        {
                            keyNameListBox.Items.Add(token.TokenValue);
                        }
                    });
                }
                if(keyNameListBox.Items.Count > 0)
                    keyNameListBox.SelectedIndex = 0;                
                keyNameListBox.Visible = true;
                availableHWKeyLabel.Visible = true;
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
        
        private void programBtn_Click(object sender, EventArgs e)
        {
            string[] value = tokenValueTextBox.Text.Split(new Char[] { '-' });
             
            KeyProgramVerification(keyNameListBox.SelectedItem.ToString(), 
                                   Int32.Parse(value[0], System.Globalization.NumberStyles.HexNumber),
                                   Int32.Parse(value[1], System.Globalization.NumberStyles.HexNumber));
        }

        private void KeyProgramVerification(string keyInfo, int custID, int keyID)
        {
            using (BackgroundWorker worker = new BackgroundWorker())
            {
                worker.DoWork += ((sender, e) => ProgramVerification(keyInfo, custID, keyID));
                worker.RunWorkerCompleted += ((sender, e) => OnProgrammedKey());
                worker.RunWorkerAsync();
            }
            loadingCircle1.Visible = true;
            loadingCircle1.Active = true;
            btnOk.Enabled = false;
            //refreshBtn.Enabled = false;
        }

        private void ProgramVerification(string keyInfo, int custID, int keyID)
        {
            Service<ICreator>.Use((client) => 
            {
                client.KeyProgramVerification(keyInfo, custID, keyID);
            });
        }

        private void OnProgrammedKey()
        {
            loadingCircle1.Active = false;
            loadingCircle1.Visible = false;
            btnOk.Enabled = true;
            //refreshBtn.Enabled = true;
            //RefreshKeyList();
        }

        private void refreshBtn_Click(object sender, EventArgs e)
        {
            RefreshKeyList();
        }

        private void RefreshKeyList()
        {
            using (BackgroundWorker worker = new BackgroundWorker())
            {
                IList<SolimarLicenseProtectionKeyInfo> data = null;
                worker.DoWork += ((sender, e) => data = LoadKeyNameListBox());
                worker.RunWorkerCompleted += ((sender, e) => OnRefreshedKey(data));
                worker.RunWorkerAsync();
            }
            btnOk.Enabled = false;
            loadingCircle1.Visible = true;
            loadingCircle1.Active = true;
        }

        private void OnRefreshedKey(IList<SolimarLicenseProtectionKeyInfo> keys)
        {
            //keyNameListBox.Items.Clear();
            AvailableKeysListBox.Items.Clear();
            if (keys != null)
            {
                if (keys.Count > 0)
                {
                    foreach (SolimarLicenseProtectionKeyInfo key in keys)
                    {
                        //if (key.IsKeyTypeUninitialized())
                        //{
                        //keyNameListBox.Items.Add(key.keyName);
                        AvailableKeysListBox.Items.Add(key.keyName);
                        //}
                    }
                }
                if (AvailableKeysListBox.Items.Count < 1)
                {
                    AvailableKeysListBox.Items.Add("No Keys Attached");
                    AvailableKeysListBox.Enabled = false;
                    btnOk.Enabled = false;
                }
                else
                {
                    AvailableKeysListBox.Enabled = true;
                    AvailableKeysListBox.SelectedIndex = 0;
                    btnOk.Enabled = true;
                }
            }
            loadingCircle1.Active = false;
            loadingCircle1.Visible = false;
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

        #region OrderTabPage Events

        private void orderProductComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            //filter out available types - for status
            OrderDialogData storedObject = selectedObject as OrderDialogData;            
            //get current product version for product;
            string selectedProduct = orderProductComboBox.SelectedItem.ToString();
            //if (selectedProduct == "No Products Available")
            //    btnOk.Enabled = false;
            orderTypeComboBox.Items.Clear();
            orderVersionMaskedTextBox.Text = "0.00";
            Service<ICreator>.Use((client) => 
            {
                //get all orders for a given product
                IList<OrderTable> productOrders = client.GetOrdersEqualsProduct(storedObject.OrderInfo.LicenseName, selectedProduct);                
                orderTypeComboBox.Items.Add(Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msTrial);
                if (productOrders.Count > 0)
                {
                    Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState state = (Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState)productOrders[0].OrderState;
                    foreach(OrderTable orderRec in productOrders)
                    {
                        if (orderRec.OrderState.Equals((byte)Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msPerm))
                        {
                            orderTypeComboBox.Items.Add(string.Format("{0} : {1}", Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msAddOn, orderRec.OrderNumber));  
                        }
                    }               
                }
                orderTypeComboBox.SelectedIndex = 0;
                int productID = CreatorForm.s_CommLink.GetProductID(selectedProduct);
                if (productID > 0)
                {
                    int productVersion = client.GetProductVersionFromTable(productID);
                    orderVersionMaskedTextBox.Text = string.Format("{0:x}", productVersion);
                }
            });

        }

        private void orderTypeComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            //on trial don't need to do anything
            //on addon display list of possible orders to add onto 
            //if (orderTypeComboBox.SelectedItem.ToString() == Enum.GetName(typeof(Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState), Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msPerm))
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

        private void btnOk_Click(object sender, EventArgs e)
        {
            if (licInfoTabControl.SelectedTab == TokenTabPage)
                ValidateTokenForm();            
        }

        private void LicenseInfoForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (DialogResult == DialogResult.OK)
                if (!m_Validated)
                    e.Cancel = true;
        }

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
    }

    #region LicenseDialogData class
    public class LicenseDialogData : Shared.VisualComponents.DialogData
    {
        private LicenseProperty m_LicInfo;

        #region Constructors

        public LicenseDialogData(LicenseProperty licInfo)
        {
            m_LicInfo = licInfo;
        }
        #endregion

        #region Properties

        public LicenseProperty LicInfo
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
        private LicenseProperty m_LicInfo;
        private List<Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType> m_ValidTokenTypes;

        #region Constructors

        public TokenDialogData(ValidationProperty token, LicenseProperty licInfo)
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

        public LicenseProperty LicInfo
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
        private List<string> m_ModuleNames;
        private OrderProperty m_OrderData;
        #region Constructors

        public ModuleDialogData(List<string> moduleNames, OrderProperty orderData)
        {
            m_ModuleNames = moduleNames;
            m_OrderData = orderData;
        }
        #endregion

        #region Properties

        public List<string> ModuleNames
        {
            get { return this.m_ModuleNames; }
            set { this.m_ModuleNames = value; }
        }
        public OrderProperty OrderData
        {
            get { return this.m_OrderData; }
            set { this.m_OrderData = value; }
        }
        #endregion
    }
    #endregion

    #region OrderDialogData class
    public class OrderDialogData : Shared.VisualComponents.DialogData
    {
        private OrderProperty m_OrderInfo;
        private LicenseProperty m_LicenseInfo;

        #region Constructors

        public OrderDialogData(OrderProperty orderInfo, LicenseProperty licInfo)
        {
            m_OrderInfo = orderInfo;
            m_LicenseInfo = licInfo;
        }
        #endregion

        #region Properties

        public OrderProperty OrderInfo
        {
            get { return this.m_OrderInfo; }
            set { this.m_OrderInfo = value; }
        }

        public LicenseProperty LicenseInfo
        {
            get { return this.m_LicenseInfo; }
            set { this.m_LicenseInfo = value; }
        }

        //public Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType LicenseType
        //{
        //    get { return this.m_LicenseType; }
        //    set { this.m_LicenseType = value; }
        //}
        #endregion
    }
    #endregion
}
