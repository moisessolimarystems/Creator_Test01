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
    public class Transaction
    {
        public Transaction() { }       
        public TransactionType Type { get; set; }
        public string LicenseServerName { get; set; }
        public string ProductLicenseName { get; set; }
        public string Description { get; set; }
        public string Value { get; set; }
        public string PreviousValue { get; set; }
    }  
     
    public static class TransactionManager
    {
        public static ListView LogListView;
        public static ListViewMgr ListViewManager;

        public static void LogTransactions(Transaction transaction)
        {
            //LogListView.BeginUpdate();
            ListViewItem lvItem = new ListViewItem(DateTime.Now.ToString());
            lvItem.SubItems.Add(transaction.Type.ToString());
            lvItem.SubItems.Add(transaction.Description);
            lvItem.SubItems.Add(transaction.Value);
            lvItem.SubItems.Add(transaction.PreviousValue);
            lvItem.SubItems.Add(transaction.ProductLicenseName);
            lvItem.SubItems.Add(transaction.LicenseServerName);
            LogListView.Items.Add(lvItem);
            //LogListView.Sort();
            ListViewManager.AutoResizeColumns(LogListView);
            //LogListView.EndUpdate();
        }

        public static void CreateTransactions(List<Transaction> transactions)
        {
            List<TransactionTable> transactionList = new List<TransactionTable>();
            //LogListView.BeginUpdate();
            Service<ICreator>.Use((client) =>
            {
                foreach (Transaction transaction in transactions)
                {
                    LogTransactions(transaction);
                    int? lsID = null, plID = null;

                    if (transaction.ProductLicenseName.Length == 0) //license server transaction
                    {
                        LicenseTable selectedLicense = client.GetLicenseByName(transaction.LicenseServerName, false);
                        if (selectedLicense == null)
                        {
                            MessageBox.Show(string.Format("Failed to find license server : {0}", transaction.LicenseServerName), "Create Transaction");
                            continue;
                        }
                        lsID = selectedLicense.ID;
                    }
                    else //product license transaction
                    {
                        ProductLicenseTable selectedOrder = client.GetProductLicense(transaction.ProductLicenseName);
                        if (selectedOrder == null)
                        {
                            MessageBox.Show(string.Format("Failed to find product license : {0}", transaction.ProductLicenseName), "Create Transaction");
                            continue;
                        }
                        plID = selectedOrder.ID;
                        lsID = selectedOrder.LicenseID;
                    }
                    TransactionTable newTransaction = new TransactionTable()
                    {
                        taLicenseID = lsID.Value,
                        taType = (Byte)transaction.Type,
                        taDateCreated = DateTime.Now,
                        taPacketID = null,
                        taDescription = transaction.Description,
                        taUnits = transaction.Value,
                        taPreviousValue = transaction.PreviousValue,
                        taUser = WindowsIdentity.GetCurrent().Name,
                        taOrderID = plID
                    };
                    transactionList.Add(newTransaction);
                }
                client.CreateAllTransactions(transactionList);
            });
            //LogListView.EndUpdate();

        }
        public static void CreateTransaction(TransactionType type, string licenseServerName, string productLicenseName, string description, string value, string previousValue)
        {
            LogListView.BeginUpdate();
            LogTransactions(new Transaction() { Type = type, LicenseServerName = licenseServerName, ProductLicenseName = productLicenseName, Description = description, Value = value, PreviousValue = previousValue });
            LogListView.EndUpdate();
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
