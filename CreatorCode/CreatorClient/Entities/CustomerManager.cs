using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Client.Creator.ServiceProxy;
using Client.Creator.Properties;
using System.ServiceModel;
using Client.Creator.CreatorService;

namespace Client.Creator
{
    class CustomerManager
    {
        //takes a customer property and does actions on it
        //create new customerproperty
        //edit customerproperty
        //CustomerProperty CreateNewCustomer(string customerName)
        //{
        //    CustomerTable newCustRec = new CustomerTable();
        //    newCustRec.SCRname = customerName;
        //    Service<ICreator>.Use((client) =>
        //    {
        //        client.CreateCustomer(newCustRec);
        //        //need to retrieve custRec again for updated scrnumber
        //        newCustRec = client.GetCustomer(customerName, false);
        //        if (newCustRec == null)
        //        {
        //            MessageBox.Show(string.Format("Failed to create new customer : {0}", custName), "Create New Customer");
        //            return;
        //        }
        //        DestinationNameTable destRec = new DestinationNameTable();
        //        destRec.CustID = newCustRec.SCRnumber;
        //        destRec.DestID = 0;
        //        destRec.DestName = newCustRec.SCRname;
        //        client.CreateDestinationName(destRec);
        //    });
        //    ListViewItem lvItem = new ListViewItem(string.Format("{0:x}", newCustRec.SCRnumber));
        //    lvItem.Name = newCustRec.SCRname;
        //    lvItem.SubItems.Add(newCustRec.SCRname);
        //    CustomersListView.Items.Add(lvItem);
        //    lvItem.Selected = true;
        //    CustomersListView.EnsureVisible(CustomersListView.Items.IndexOf(lvItem));
        //}     
    }
}
