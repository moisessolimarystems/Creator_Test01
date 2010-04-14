using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using Client.Creator.CreatorService;
using Client.Creator.ServiceProxy;

namespace Client.Creator
{
    public class CustomerProperty
    {
        #region Fields
        CustomerTable _customer;
        #endregion

        #region Constructor
        public CustomerProperty() 
        {
            _customer = new CustomerTable();
        }

        public CustomerProperty(CustomerTable cust)
        {
            _customer = cust;
        }
        #endregion

        #region Properties
        [Browsable(false)]
        public int Id
        {
            get { return _customer.SCRnumber; }
            set { _customer.SCRnumber = value; }
        }

        [Category("Customer Information")]
        [Description("Name")]
        [DisplayName("Name")]          
        public string Name
        {
            get { return _customer.SCRname; }
            set { _customer.SCRname = value;}
        }


        [Category("Customer Information")]
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
