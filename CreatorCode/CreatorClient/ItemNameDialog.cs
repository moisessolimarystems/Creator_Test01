using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Client.Creator.CreatorService;
using Solimar.Licensing.Attribs;

namespace Client.Creator
{
    public partial class ItemNameDialog : Shared.VisualComponents.DialogBaseForm
    {
        private string _name = "";
        private bool _bValidItem = true;
        private Object _selectedObject;        

        public string Name
        {
            get { return _name; }
            set { _name = value; }
        }

        #region ItemNameDialog Methods
        public ItemNameDialog()
        {
            InitializeComponent();
        }

        public ItemNameDialog(string title)
        {
            InitializeComponent();
            this.Text = title;
            itemNameTextBox.Focus();
        }

        private void ValidateDialogInput(string inputString)
        {
            if (inputString != Name || inputString.Length == 0)
            {
                if (_selectedObject is CustomerDialogData)
                    _bValidItem = (_selectedObject as CustomerDialogData).IsCustomerNameValid(inputString);
                else
                    _bValidItem = (_selectedObject as DestNameDialogData).IsDestNameValid(inputString);
                if (!_bValidItem)
                {
                    MessageBox.Show(string.Format("Please enter a valid {0}.", _selectedObject.ToString()),
                                    "Input Error",
                                    MessageBoxButtons.OK,
                                    MessageBoxIcon.Error);
                    itemNameTextBox.Select(0, inputString.Length);
                    itemNameTextBox.Focus();
                }
            }
        }
        #endregion

        #region ItemNameDialog Events
        private void ItemNameDialog_FinishDialog(object sender, Shared.VisualComponents.FinishDialogEventArgs e)
        {
            _name = itemNameTextBox.Text;
        }

        private void ItemNameDialog_InitDialog(object sender, Shared.VisualComponents.InitDialogEventArgs e)
        {
            if (e.Data is CustomerDialogData)
            {
                CustomerDialogData custData = e.Data as CustomerDialogData;
                Name = itemNameTextBox.Text = custData.Customer;
                ItemTypeLabel.Text = custData.ToString() + ":";
            }
            else
            {
                DestNameDialogData destNameData = e.Data as DestNameDialogData;
                Name = itemNameTextBox.Text = destNameData.DestinationName;
                ItemTypeLabel.Text = destNameData.ToString() + ":";
            }
            _selectedObject = e.Data;
        }

        private void ItemNameDialog_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (DialogResult == DialogResult.OK)
                if (!_bValidItem)
                    e.Cancel = true;
        }

        private void btnOk_Click(object sender, EventArgs e)
        {
            ValidateDialogInput(itemNameTextBox.Text);
        }

        private void ItemNameDialog_Shown(object sender, EventArgs e)
        {
            itemNameTextBox.Select(0, itemNameTextBox.Text.Length);
            itemNameTextBox.Focus();
        }
        #endregion
    }
    #region CustomerDialogData class
    public class CustomerDialogData : Shared.VisualComponents.DialogData
    {
        private string _customerName;

        #region Constructors

        public CustomerDialogData()
        {
            _customerName = "";
        }
        public CustomerDialogData(string custName)
        {
            _customerName = custName;
        }
        public bool IsCustomerNameValid(string custName)
        {
            bool bRetVal = true;
            if(custName.Length > 0)
            {   //Check Duplicates
                CustomerTable custRec = null;
                Client.Creator.ServiceProxy.Service<ICreator>.Use((client) =>
                {
                    custRec = client.GetCustomer(custName, false);
                });
                if (custRec != null)
                    bRetVal = false;
                //Check Valid Characters
                if (custName.IndexOfAny(System.IO.Path.GetInvalidFileNameChars()) > -1)
                    bRetVal = false;                
            }

            return bRetVal;
        }        
        public override string ToString()
        {
            return "Customer Name";
        }
        #endregion

        #region Properties

        public string Customer
        {
            get { return this._customerName; }
            set { this._customerName = value; }
        }
        #endregion
    }
    #endregion

    #region DestNameDialogData class
    public class DestNameDialogData : Shared.VisualComponents.DialogData
    {
        private string _destName;
        private uint _custID;

        #region Constructors

        public DestNameDialogData(uint custID)
        {
            _destName = "";
            _custID = custID;
        }
        public DestNameDialogData(string destName, uint custID)
        {
            _destName = destName;
            _custID = custID;
        }
        public bool IsDestNameValid(string destName)
        {
            bool bRetVal = false;
            if (destName.Length > 0)
            {
                DestinationNameTable destNameRec = null;
                Client.Creator.ServiceProxy.Service<ICreator>.Use((client) =>
                {
                    destNameRec = client.GetDestinationID((int)_custID, destName);
                });
                if (destNameRec == null)
                    bRetVal = true;
            }
            return bRetVal;
        }
        public override string ToString()
        {
            return "Destination Name";
        }
        #endregion

        #region Properties

        public string DestinationName
        {
            get { return this._destName; }
            set { this._destName = value; }
        }
        #endregion
    }
    #endregion
}
