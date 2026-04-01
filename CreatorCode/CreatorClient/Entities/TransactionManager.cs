using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Client.Creator.Entities
{
    class TransactionManager
    {
        public TransactionManager()
        {
        }
        /*
        private void CreateTransaction(TransactionType type, string licenseName, string description, string value, string previous value)
        {
            Service<ICreator>.Use((client) =>
            {
                LicenseTable selectedLicense = client.GetLicenseByName(licenseName, false);
                if (selectedLicense == null)
                {
                    MessageBox.Show(string.Format("Failed to find license server : {0}", licenseName), "Create Transaction");
                    return;
                }
                TransactionTable newTransaction = new TransactionTable();
                newTransaction.taLicenseID = selectedLicense.ID;
                newTransaction.taType = (Byte)type;
                newTransaction.taDateCreated = DateTime.Now;
                newTransaction.taPacketID = null;
                newTransaction.taDescription = description;
                newTransaction.taUnits = value;
                newTransaction.taUser = WindowsIdentity.GetCurrent().Name;
                client.CreateTransaction(newTransaction);
            });
            //SetLicenseServerState(DetailTreeView.SelectedNode, false);
        }
        //Transaction for Order
        private void CreateProductLicenseTransaction(TransactionType type, string plID, string description, string value)
        {
            Service<ICreator>.Use((client) =>
            {
                ProductLicenseTable selectedOrder = client.GetProductLicense(plID);
                if (selectedOrder == null)
                {
                    MessageBox.Show(string.Format("Failed to find product license : {0}", plID), "Create Transaction");
                    return;
                }
                TransactionTable newTransaction = new TransactionTable();
                newTransaction.taLicenseID = selectedOrder.LicenseID;
                newTransaction.taType = (Byte)type;
                newTransaction.taDateCreated = DateTime.Now;
                newTransaction.taPacketID = null;
                newTransaction.taDescription = description;
                newTransaction.taUnits = value;
                newTransaction.taUser = WindowsIdentity.GetCurrent().Name;
                newTransaction.taOrderID = selectedOrder.ID;
                client.CreateTransaction(newTransaction);
            });
            //SetLicenseServerState(DetailTreeView.SelectedNode, false);
        }
        //Compare between current licenseinfo vs stored db licenseinfo
        //Difference creates a transaction and value is updated for stored db licenseinfo
        private void LicenseItemTransactions(ref Lic_PackageAttribs.AttribsMemberAttribsClass_Lic_LicenseInfoAttribs licInfo, ref LicenseServerProperty licItem)
        {
            //License Attribs 
            //1) LicType 2) GroupID 3) DestID 4) CustID 5) ValidationToken list
            string licenseName = Lic_LicenseInfoAttribsHelper.GetDisplayLabel(licInfo);
            //Compare GroupID - READONLY
            licInfo.TVal.softwareGroupLicenseID.TVal = licItem.GroupID;
            //Compare CustID - READONLY
            licInfo.TVal.customerID.TVal = licItem.CustID;
            //Compare LicenseType - READONLY
            licInfo.TVal.softwareLicType.EVal = (Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType)Enums.GetLicenseServerType(licItem.LicType);
            //Compare DestID - READONLY
            licInfo.TVal.destinationID.TVal = licItem.DestID;

            //if (!licInfo.TVal.activationCurrent.TVal.Equals(licItem.ActivationCurrent))
            //{
            //    CreateTransaction(TransactionType.LicenseServer, licenseName, string.Format("Changed Activation Current from {0} to {1}", licInfo.TVal.activationCurrent.ToString(), licItem.ActivationCurrent.ToString()), licItem.ActivationCurrent.ToString());
            //    licInfo.TVal.activationCurrent.TVal = licItem.ActivationCurrent;
            //}
            if (!licInfo.TVal.activationTotal.TVal.Equals(licItem.ActivationTotal))
            {
                CreateTransaction(TransactionType.LicenseServer, licenseName, string.Format("Changed Activation Total from {0} to {1}", licInfo.TVal.activationTotal.ToString(), licItem.ActivationTotal.ToString()), licItem.ActivationTotal.ToString());
                licInfo.TVal.activationTotal.TVal = licItem.ActivationTotal;
            }
            if (!licInfo.TVal.activationAmountInDays.TVal.Equals(licItem.ActivationAmtInDays))
            {
                CreateTransaction(TransactionType.LicenseServer, licenseName, string.Format("Changed Activation Amount in Days from {0} to {1}", licInfo.TVal.activationAmountInDays.ToString(), licItem.ActivationAmtInDays.ToString()), licItem.ActivationAmtInDays.ToString());
                licInfo.TVal.activationAmountInDays.TVal = licItem.ActivationAmtInDays;
            }
            bool found;
            if (licInfo.TVal.licVerificationAttribs.TVal.validationTokenList.TVal.Count > licItem.LicInfo.licVerificationAttribs.TVal.validationTokenList.TVal.Count)
            {
                //validatation token was removed
                List<Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs> removeList = new List<Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs>();
                foreach (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs storedToken in licInfo.TVal.licVerificationAttribs.TVal.validationTokenList.TVal)
                {   //search for any tokens in stored tokenlist that are not in new token list
                    found = false;
                    foreach (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs validationToken in licItem.LicInfo.licVerificationAttribs.TVal.validationTokenList.TVal)
                    {
                        if (ValidationTokenCompare(validationToken, storedToken))
                        {
                            found = true;
                            break;
                        }
                    }
                    if (!found)
                        removeList.Add(storedToken);
                }
                foreach (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs removeToken in removeList)
                {
                    licInfo.TVal.licVerificationAttribs.TVal.validationTokenList.TVal.Remove(removeToken);
                }
            }
            else if (licInfo.TVal.licVerificationAttribs.TVal.validationTokenList.TVal.Count < licItem.LicInfo.licVerificationAttribs.TVal.validationTokenList.TVal.Count)
            {   //validation token was added
                foreach (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs vToken in licItem.LicInfo.licVerificationAttribs.TVal.validationTokenList.TVal)
                {
                    found = false;
                    foreach (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs tokenInfo in licInfo.TVal.licVerificationAttribs.TVal.validationTokenList.TVal)
                    {
                        if (ValidationTokenCompare(vToken, tokenInfo))
                        {
                            found = true;
                            break;
                        }
                    }
                    //new token type was created
                    if (!found)
                    {
                        //add token since not found
                        Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs token = new Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs();
                        token = vToken;
                        licInfo.TVal.licVerificationAttribs.TVal.validationTokenList.TVal.Add(token);
                    }
                }
            }
            else
            {   //modified
                foreach (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs vToken in licItem.LicInfo.licVerificationAttribs.TVal.validationTokenList.TVal)
                {
                    foreach (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs storedToken in licInfo.TVal.licVerificationAttribs.TVal.validationTokenList.TVal)
                    {
                        //if tokentype is the same, then the value must be different
                        if (storedToken.tokenType.TVal.Equals(vToken.tokenType.TVal))
                        {   //only modify if value has changed
                            if (!storedToken.tokenValue.TVal.Equals(vToken.tokenValue.TVal))
                            {
                                CreateTransaction(TransactionType.LicenseServer, licenseName, string.Format("Changed Token Value from {0} to {1}", storedToken.tokenValue.TVal, vToken.tokenValue.TVal), vToken.tokenValue.TVal);
                                storedToken.tokenValue.TVal = vToken.tokenValue.TVal;
                                break;
                            }
                        }
                    }
                }
            }
        }
        private bool OrderTransactions(ref ProductLicenseTable plData, ProductLicenseProperty productLicense)
        {
            //1) expdate, status, description
            bool bChanged = false;
            if (!plData.plState.Equals((byte)productLicense.Status))
            {
                CreateProductLicenseTransaction(TransactionType.Product,
                                  productLicense.ID,
                                  string.Format("{0}: Product License Status from {1} to {2}", productLicense.ID, ((ProductLicenseState)plData.plState).ToString(), productLicense.Status.ToString()),
                                  productLicense.Status.ToString());
                plData.plState = (byte)productLicense.Status;
                bChanged = true;
            }
            //product version extends to all orders of the same product 
            if (!plData.ProductVersion.Equals(productLicense.ProductVersion.ToString()))
            {
                CreateProductLicenseTransaction(TransactionType.Product,
                                  productLicense.ID,
                                  string.Format("{0}: Product Version from {1} to {2}", productLicense.ID, plData.ProductVersion, productLicense.ProductVersion),
                                  productLicense.ProductVersion.ToString());
                plData.ProductVersion = productLicense.ProductVersion.ToString();
                Service<ICreator>.Use((client) =>
                {
                    IList<ProductLicenseTable> productOrders = client.GetProductLicensesByProduct(productLicense.LicenseServer, (int)productLicense.Product.ID);
                    foreach (ProductLicenseTable order in productOrders)
                    {
                        order.ProductVersion = productLicense.ProductVersion.ToString();
                        client.UpdateProductLicense(order);
                    }
                });
            }
            DateTime? storedDT;
            if (plData.ExpirationDate.HasValue)
                storedDT = plData.ExpirationDate.Value.ToLocalTime();
            else
                storedDT = null;
            if (!storedDT.ToString().Equals(productLicense.ExpirationDate.ToString()))
            {
                string storedValue = "None", value = "None";
                if (productLicense.ExpirationDate.HasValue)
                    value = productLicense.ExpirationDate.Value.ToShortDateString();
                if (plData.ExpirationDate.HasValue)
                    storedValue = plData.ExpirationDate.Value.ToShortDateString();
                CreateProductLicenseTransaction(TransactionType.Product,
                                  productLicense.ID,
                                  string.Format("{0}: Product License Expiration Date {1} to {2}", productLicense.ID, storedValue, value),
                                  value);
                bChanged = true;
                if (productLicense.ExpirationDate.HasValue)
                    plData.ExpirationDate = productLicense.ExpirationDate.Value.ToUniversalTime();
                else
                    plData.ExpirationDate = productLicense.ExpirationDate;
            }
            if (!plData.Description.Equals(productLicense.Description))
            {
                plData.Description = productLicense.Description;
            }
            return bChanged;
        }
        private void ProductItemTransactions(ref Lic_PackageAttribs.AttribsMemberAttribsClass_Lic_LicenseInfoAttribs licInfo, ProductLicenseProperty plData, bool bOrderChanged)
        {
            //iterate over all products
            string moduleName;
            string licenseName = Lic_LicenseInfoAttribsHelper.GetDisplayLabel(licInfo);
            ProductProperty selectedProduct = plData.Product;
            //product nodes under licenses                          
            foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs prodInfo in licInfo.TVal.productList.TVal)
            {
                //find selectedProduct in stored db productList
                if (prodInfo.productID.TVal.Equals(selectedProduct.ID))
                {
                    if (!prodInfo.productAppInstance.TVal.Equals(selectedProduct.AppInstance))
                    {
                        if (!bOrderChanged)
                            CreateProductLicenseTransaction(TransactionType.Product, plData.ID, string.Format("{0}:{1} - Application Instance from {2} to {3}", plData.ID, selectedProduct.Name, prodInfo.productAppInstance.SVal, selectedProduct.AppInstance.ToString()), selectedProduct.AppInstance.ToString());
                        prodInfo.productAppInstance.TVal = selectedProduct.AppInstance;
                    }
                    prodInfo.product_Major.TVal = selectedProduct.Product.product_Major.TVal;
                    prodInfo.product_Minor.TVal = selectedProduct.Product.product_Minor.TVal;
                    prodInfo.product_SubMajor.TVal = selectedProduct.Product.product_SubMajor.TVal;
                    prodInfo.product_SubMinor.TVal = selectedProduct.Product.product_SubMinor.TVal;
                    ////Add, Edit, Remove Module list
                    //add new module for perm or add-on, trial->perm, perm->trial
                    foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs mod in selectedProduct.ModuleList.TVal)
                    {
                        moduleName = s_CommLink.GetModuleName(selectedProduct.ID, mod.moduleID.TVal);
                        foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs storedModule in prodInfo.moduleList.TVal)
                        {
                            if (storedModule.moduleID.TVal.Equals(mod.moduleID.TVal))
                            {   //matched new module to stored module
                                //value must be different since it was not found
                                //units
                                //contractNumbers do not change 
                                if (storedModule.contractNumber.TVal.Equals(mod.contractNumber.TVal))
                                {
                                    //if add on to perm -> add modules not equal
                                    //trial,perm
                                    if (!storedModule.moduleValue.TVal.Equals(mod.moduleValue.TVal))
                                    {
                                        if (!bOrderChanged)
                                            CreateProductLicenseTransaction(TransactionType.Product,
                                                              plData.ID,
                                                              string.Format("{0}:({1}-{2}) - Value from {3} to {4}", plData.ID, selectedProduct.Name, moduleName, storedModule.moduleValue.TVal.ToString(), mod.moduleValue.TVal.ToString()),
                                                              mod.moduleValue.TVal.ToString());
                                        storedModule.moduleValue.TVal = mod.moduleValue.TVal;
                                    }
                                    //app instance
                                    if (!storedModule.moduleAppInstance.TVal.Equals(mod.moduleAppInstance.TVal))
                                    {
                                        if (!bOrderChanged)
                                            CreateProductLicenseTransaction(TransactionType.Product,
                                                              plData.ID,
                                                              string.Format("{0}:({1}-{2}) - Application Instance from {3} to {4}", plData.ID, selectedProduct.Name, moduleName, storedModule.moduleAppInstance.ToString(), mod.moduleAppInstance.ToString()),
                                                              mod.moduleAppInstance.ToString());
                                        storedModule.moduleAppInstance.TVal = mod.moduleAppInstance.TVal;
                                    }
                                    //expiration date
                                    if (!storedModule.moduleExpirationDate.TVal.Equals(mod.moduleExpirationDate.TVal))
                                    {
                                        storedModule.moduleExpirationDate.TVal = mod.moduleExpirationDate.TVal;
                                    }
                                    //expiration date
                                    if (!storedModule.moduleState.TVal.Equals(mod.moduleState.TVal))
                                    {
                                        storedModule.moduleState.TVal = mod.moduleState.TVal;
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
         */
    }
}
