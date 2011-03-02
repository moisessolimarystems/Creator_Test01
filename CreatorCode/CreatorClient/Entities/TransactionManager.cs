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
                int? lsID = null, plID = null;
                if (productLicenseName.Length == 0) //license server transaction
                {
                    LicenseTable selectedLicense = client.GetLicenseByName(licenseServerName, false);
                    if (selectedLicense == null)
                    {
                        MessageBox.Show(string.Format("Failed to find license server : {0}", licenseServerName), "Create Transaction");
                        return;
                    }
                    lsID = selectedLicense.ID;
                }
                else //product license transaction
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
                TransactionTable newTransaction = new TransactionTable() 
                {
                    taLicenseID = lsID.Value,
                    taType = (Byte)type,
                    taDateCreated = DateTime.Now,
                    taPacketID = null,
                    taDescription = description,
                    taUnits = value,
                    taPreviousValue = previousValue,
                    taUser = WindowsIdentity.GetCurrent().Name,
                    taOrderID = plID
                };
                client.CreateTransaction(newTransaction);
            });
        }     
    }
}
