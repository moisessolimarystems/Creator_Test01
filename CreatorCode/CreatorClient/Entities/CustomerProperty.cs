using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using Client.Creator.CreatorService;

namespace Client.Creator
{
    public class CustomerProperty
    {
        CustomerTable _customer;

        public CustomerProperty() 
        {
            _customer = new CustomerTable();
        }

        public CustomerProperty(CustomerTable cust)
        {
            _customer = cust;
        }

        #region Properties
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
        #endregion
    }
}
