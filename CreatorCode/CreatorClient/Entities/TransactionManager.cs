using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Solimar.Licensing.Attribs;
using System.Security.Principal;
using Client.Creator.ServiceProxy;
using Client.Creator.CreatorService;

namespace Client.Creator
{
    public static class TransactionManager
    {
        public static void CreateTransaction(TransactionType type, string licenseServerName, string productLicenseName, string description, string value, string previousValue)
        {
            Service<ICreator>.Use((client) =>
            {
                int lsID = 0, plID = 0;
                if (productLicenseName == "")
                {
                    LicenseTable selectedLicense = client.GetLicenseByName(licenseServerName, false);
                    if (selectedLicense == null)
                    {
                        MessageBox.Show(string.Format("Failed to find license server : {0}", licenseServerName), "Create Transaction");
                        return;
                    }
                    lsID = selectedLicense.ID;
                }
                else
                {
                    ProductLicenseTable selectedOrder = client.GetProductLicense(productLicenseName);
                    if (selectedOrder == null)
                    {
                        MessageBox.Show(string.Format("Failed to find product license : {0}", productLicenseName), "Create Transaction");
                        return;
                    }
                    plID = selectedOrder.ID;
                    lsID = selectedOrder.LicenseID;
                }
                TransactionTable newTransaction = new TransactionTable();
                newTransaction.taLicenseID = lsID;
                newTransaction.taType = (Byte)type;
                newTransaction.taDateCreated = DateTime.Now;
                newTransaction.taPacketID = null;
                newTransaction.taDescription = description;
                newTransaction.taUnits = value;
                newTransaction.taPreviousValue = previousValue;
                newTransaction.taUser = WindowsIdentity.GetCurrent().Name;
                if(plID != 0)
                    newTransaction.taOrderID = plID;
                client.CreateTransaction(newTransaction);
            });
        }
        //private bool ProductLicenseTransactions(ref ProductLicenseTable plData, ProductLicenseProperty productLicense)
        //{
        //    //1) expdate, status, description, activation amount, activations
        //    bool bChanged = false;
        //    if (!plData.plState.Equals((byte)productLicense.Status))
        //    {
        //        CreateTransaction(TransactionType.Status,
        //                          "",
        //                          productLicense.ID,
        //                          string.Format("Edit {0} Status", productLicense.ProductName),
        //                          productLicense.Status.ToString(),
        //                          ((ProductLicenseState)plData.plState).ToString());
        //        plData.plState = (byte)productLicense.Status;
        //        bChanged = true;
        //    }
        //    if (!plData.Activations.Equals((byte)productLicense.ActivationTotal))
        //    {
        //        CreateTransaction(TransactionType.ActivationTotal,
        //                      "",
        //                      productLicense.ID,
        //                      string.Format("Modify {0} Activation Total", productLicense.ProductName),
        //                      plData.Activations.ToString(),
        //                      productLicense.ActivationTotal.ToString());  
        //        plData.Activations = (byte)productLicense.ActivationTotal;
        //        bChanged = true;
        //    }
        //    if (!plData.ActivationAmount.Equals((byte)productLicense.ActivationAmountInDays))
        //    {
        //        CreateTransaction(TransactionType.ActivationAmount,
        //                      "",
        //                      productLicense.ID,
        //                      string.Format("Modify {0} Activation Amount in Days", productLicense.ProductName),
        //                      plData.ActivationAmount.ToString(),
        //                      productLicense.ActivationAmountInDays.ToString());  
        //        plData.ActivationAmount = (byte)productLicense.ActivationAmountInDays;
        //        bChanged = true;
        //    }
        //    //product version extends to all orders of the same product 
        //    if (!plData.ProductVersion.Equals(productLicense.ProductVersion.ToString()))
        //    {
        //        CreateTransaction(TransactionType.Version,
        //                          "",
        //                          productLicense.ID,
        //                          string.Format("Modify {0} Version", productLicense.ProductName),
        //                          productLicense.ProductVersion.ToString(),
        //                          plData.ProductVersion);
        //        plData.ProductVersion = productLicense.ProductVersion.ToString();
        //        Service<ICreator>.Use((client) =>
        //        {
        //            IList<ProductLicenseTable> productLicenses = client.GetProductLicensesByProduct(productLicense.LicenseServer, (int)productLicense.Product.ID);
        //            foreach (ProductLicenseTable pl in productLicenses)
        //            {
        //                pl.ProductVersion = productLicense.ProductVersion.ToString();
        //                client.UpdateProductLicense(pl);
        //            }
        //        });
        //    }

        //    DateTime? storedDT;
        //    if (plData.ExpirationDate.HasValue)
        //        storedDT = plData.ExpirationDate.Value.ToLocalTime();
        //    else
        //        storedDT = null;
        //    if (!storedDT.ToString().Equals(productLicense.ExpirationDate.ToString()))
        //    {
        //        string storedValue = "None", value = "None";
        //        if (productLicense.ExpirationDate.HasValue)
        //            value = productLicense.ExpirationDate.Value.ToShortDateString();
        //        if (plData.ExpirationDate.HasValue)
        //            storedValue = plData.ExpirationDate.Value.ToShortDateString();
        //        CreateTransaction(TransactionType.ExpirationDate,
        //                          "",
        //                          productLicense.ID,
        //                          string.Format("Modify {0} Expiration Date", productLicense.ProductName),
        //                          value,
        //                          storedValue);
        //        bChanged = true;
        //        if (productLicense.ExpirationDate.HasValue)
        //            plData.ExpirationDate = productLicense.ExpirationDate.Value.ToUniversalTime();
        //        else
        //            plData.ExpirationDate = productLicense.ExpirationDate;
        //    }
        //    if (!plData.Description.Equals(productLicense.FullDescription))
        //    {
        //        plData.Description = productLicense.FullDescription;
        //    }
        //    return bChanged;
        //}
        //private void ProductItemTransactions(ref Lic_PackageAttribs.AttribsMemberAttribsClass_Lic_LicenseInfoAttribs licInfo, ProductLicenseProperty plData, bool bOrderChanged)
        //{
        //    //iterate over all products
        //    string moduleName;
        //    string licenseName = Lic_LicenseInfoAttribsHelper.GetDisplayLabel(licInfo);
        //    ProductProperty selectedProduct = plData.Product;
        //    //product nodes under licenses                          
        //    foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs prodInfo in licInfo.TVal.productList.TVal)
        //    {
        //        if(prodInfo.productID.TVal == plData.Product.ID)
        //        {
        //            prodInfo.product_Major.TVal = plData.Product.Product.product_Major.TVal;
        //            prodInfo.product_Minor.TVal = plData.Product.Product.product_Minor.TVal;
        //            prodInfo.product_SubMajor.TVal = plData.Product.Product.product_SubMajor.TVal;
        //            prodInfo.product_SubMinor.TVal = plData.Product.Product.product_SubMinor.TVal;
        //            //find selectedProduct in stored db productList
        //            if (prodInfo.contractNumber.TVal.Equals(plData.ID))
        //            {
        //                if (!prodInfo.productAppInstance.TVal.Equals(selectedProduct.AppInstance))
        //                {
        //                    if (!bOrderChanged)
        //                        CreateTransaction(TransactionType.ProductConnection,
        //                                          "",
        //                                          plData.ID,
        //                                          string.Format("Modify {0} Product Connection", selectedProduct.Name),
        //                                          selectedProduct.AppInstance.ToString(),
        //                                          prodInfo.productAppInstance.TVal.ToString());
        //                }
        //                ////Add, Edit, Remove Module list
        //                foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs mod in selectedProduct.ModuleList.TVal)
        //                {
        //                    moduleName = s_CommLink.GetModuleName(selectedProduct.ID, (short)mod.moduleID.TVal);
        //                    Lic_PackageAttribs.Lic_ModuleInfoAttribs module = null;
        //                    foreach (Lic_PackageAttribs.Lic_ModuleInfoAttribs storedModule in prodInfo.moduleList.TVal)
        //                    {
        //                        if (storedModule.moduleID.TVal == mod.moduleID.TVal)
        //                        {
        //                            module = storedModule;
        //                            break;
        //                        }
        //                    }
        //                    if (!bOrderChanged)
        //                    {
        //                        string previousValue = (module != null) ? module.moduleValue.TVal.ToString() : "0";
        //                        if (previousValue != mod.moduleValue.TVal.ToString())
        //                        {
        //                            CreateTransaction(TransactionType.Module,
        //                                              "",
        //                                              plData.ID,
        //                                              string.Format("Modify {0} Module Value - {1}", selectedProduct.Name, moduleName),
        //                                              mod.moduleValue.TVal.ToString(),
        //                                              previousValue);
        //                        }
        //                    }
        //                }
        //                prodInfo.Stream = selectedProduct.Product.Stream;
        //            }
        //        }                 
        //    }
        //}      
    }
}
