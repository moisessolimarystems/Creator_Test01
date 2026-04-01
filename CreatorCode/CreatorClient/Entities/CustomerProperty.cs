using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using Client.Creator.CreatorService;

namespace Client.Creator
{
    //can I make an abstract base class?
    public class CustomerProperty
    {
        //string _name;
        //uint _id;
        //string _operatorName;
        CustomerTable _customer;

        public CustomerProperty() 
        {
            //_name = "";
            //_id = 0;
            //_operatorName = "";
            _customer = new CustomerTable();
        }

        public CustomerProperty(CustomerTable cust)
        {
            _customer = cust;
            //_name = cust.SCRname;
            //_id = (uint)cust.SCRnumber;
            //_operatorName = cust.SCRoperator;
        }

        [Browsable(false)]
        public int Id
        {
            get { return _customer.SCRnumber; }
            set { _customer.SCRnumber = value; }
        }

        [Category("CustomerProperty")]
        [Description("Name")]
        [DisplayName("Name")]          
        public string Name
        {
            get { return _customer.SCRname; }
            set { _customer.SCRname = value; }
        }


        [Category("CustomerProperty")]
        [Description("Operator")]
        [DisplayName("Operator")]        
        public string Operator
        {
            get { return _customer.SCRoperator; }
            set { _customer.SCRoperator = value; }
        }
    }
}
