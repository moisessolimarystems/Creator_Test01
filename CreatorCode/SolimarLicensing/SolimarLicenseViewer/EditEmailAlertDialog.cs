using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Solimar.Licensing.Attribs;
using Shared.VisualComponents;
namespace SolimarLicenseViewer
{
    public partial class EditEmailAlertDialog : Shared.VisualComponents.DialogBaseForm
    {
        public EditEmailAlertDialog()
        {
            InitializeComponent();
            InitializeInternal();
            this.funcGetProductName = null;
            this.actionValidateName = null;
            this.imageList = null;
        }
        public EditEmailAlertDialog(
            Func<int, string> _funcGetProductName,
            Action<string> _funcValidateName, 
            ImageList _imageList)
        {
            InitializeComponent();
            InitializeInternal();
            this.funcGetProductName = _funcGetProductName;
            this.actionValidateName = _funcValidateName;
            this.imageList = _imageList;
        }
        
        private Func<int, string> funcGetProductName = null;
        private Action<string> actionValidateName = null;
        //private Action
        private void InitializeInternal()
        {
            this.statusComboBox.Items.Clear();
            this.statusComboBox.Items.Add("Active");
            this.statusComboBox.Items.Add("Deactive");
            // this.statusComboBox.SelectedIndex = 0;
        }

        private const string TITLE = "Edit E-mail Alert";
        private const string READONLY = " (Read Only)";
        private bool bReadOnly = false;
        private Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_EmailAlertMailAttribs emailAlertAttribs;
        //private Lic_PackageAttribs.Lic_LicenseInfoAttribs licenseInfoAttribs;
        private int[] allProductIdList = null;
        private int[] licensedProductIdList;

        private ImageList imageList = null;
        System.Collections.Generic.SortedList<int, bool> checkedProductIdList = new System.Collections.Generic.SortedList<int, bool>();
        System.Collections.Generic.SortedList<int, bool> checkedEventIdList = new System.Collections.Generic.SortedList<int, bool>();
        
        public class EditEmailAlertDialogData
        {
            public EditEmailAlertDialogData(
                Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_EmailAlertMailAttribs _emailAlertAttribs,
                //int[] _allProductIdList,
                int[] _licensedProductIdList
                )
            {
                this.emailAlertAttribs = _emailAlertAttribs.Clone() as Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_EmailAlertMailAttribs;
                //this.allProductIdList = _allProductIdList;
                this.licensedProductIdList = _licensedProductIdList;
            }
            public Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_EmailAlertMailAttribs emailAlertAttribs { get; private set; }
            //public int[] allProductIdList { get; private set; }
            public int[] licensedProductIdList { get; private set; }
            //Lic_PackageAttribs.Lic_LicenseInfoAttribs licenseInfoAttribs { get; private set; }
        }

        public void SetData(
            EditEmailAlertDialogData _data,
            bool _bReadOnly
            )
        {
            this.emailAlertAttribs = (Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_EmailAlertMailAttribs)_data.emailAlertAttribs.Clone();
            //this.allProductIdList = _data.allProductIdList;
            this.licensedProductIdList = _data.licensedProductIdList;
            this.bReadOnly = _bReadOnly;

            this.Text = string.Format("{0}{1}",
                TITLE,
                this.bReadOnly ? READONLY : ""
                );

            if (this.emailAlertAttribs != null)
            {
                this.checkedProductIdList.Clear();
                this.checkedEventIdList.Clear();
                foreach (UInt32 eventId in this.emailAlertAttribs.eventIdList.TVal)
                {
                    int partProductId = (int)eventId / 1000;
                    if (partProductId == 0)
                        partProductId = -1;
                    int partEventId = (int)eventId % 1000;
                    if (!this.checkedProductIdList.ContainsKey(partProductId))
                        this.checkedProductIdList.Add(partProductId, true);
                    if (!this.checkedEventIdList.ContainsKey(partEventId))
                        this.checkedEventIdList.Add(partEventId, true);
                }

                this.nameTBox.Text = this.emailAlertAttribs.name;

                this.statusComboBox.SelectedIndex = this.emailAlertAttribs.bActive.TVal ? 0 : 1;

                StringBuilder recipentsSBuilder = new StringBuilder();
                foreach (string recipent in this.emailAlertAttribs.recipentsList.TVal)
                {
                    recipentsSBuilder.AppendFormat("{0}{1}",
                        ((recipentsSBuilder.Length == 0) ? "" : ";"),
                        recipent);
                }
                this.recipentsTBox.Text = recipentsSBuilder.ToString();
            }

            this.nameTBox.ReadOnly = this.bReadOnly;
            this.statusComboBox.Enabled = !this.bReadOnly;
            this.recipentsTBox.ReadOnly = this.bReadOnly;

            RefreshUI();
        }
        public Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_EmailAlertMailAttribs GetData()
        {
            Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_EmailAlertMailAttribs tmpEmailAlertAttribs = new Lic_ServerDataAttribs.Lic_AlertInfoAttribs.Lic_EmailAlertMailAttribs();
            tmpEmailAlertAttribs.name.TVal = this.nameTBox.Text;
            tmpEmailAlertAttribs.bActive.TVal = (this.statusComboBox.SelectedIndex == 0);

            foreach (int productId in this.checkedProductIdList.Keys)
                foreach (UInt32 eventId in this.checkedEventIdList.Keys)
                    tmpEmailAlertAttribs.eventIdList.TVal.Add((productId == -1) ? (UInt32)eventId : (UInt32)((productId * 1000) + eventId));

            tmpEmailAlertAttribs.id = this.emailAlertAttribs.id;
            foreach (string recipient in this.recipentsTBox.Text.Split(';'))
                tmpEmailAlertAttribs.recipentsList.TVal.Add(recipient);
            return tmpEmailAlertAttribs;
        }

        private void btnOk_Click(object sender, EventArgs e)
        {
            if (!ValidateInput())
                DialogResult = System.Windows.Forms.DialogResult.None;
        }

        private bool ValidateInput()
        {
            bool bValid = false;
            Control errorControl = null;
            Control postErrorControl = null;
            try
            {
                if (string.IsNullOrEmpty(this.nameTBox.Text))
                {
                    errorControl = this.nameTBox;
                    throw new Exception("Alert name cannot be empty.");
                }
                try
                {
                    this.emailAlertAttribs.name.TVal = this.nameTBox.Text;
                    this.actionValidateName(this.emailAlertAttribs.ToString());
                }
                catch (Exception)
                {
                    errorControl = this.nameTBox;
                    throw;
                }

                if (string.IsNullOrEmpty(this.recipentsTBox.Text))
                {
                    errorControl = this.recipentsTBox;
                    throw new Exception("Recipents cannot be empty.");
                }

                if (string.IsNullOrEmpty(this.productsTextBox.Text))
                {
                    postErrorControl = this.productsTextBox;
                    throw new Exception("Must specify at least one Solimar Product.");
                }

                if (string.IsNullOrEmpty(this.eventsTextBox.Text))
                {
                    postErrorControl = this.eventsTextBox;
                    throw new Exception("Must specify at least one Event ID.");
                }
                bValid = true;
            }
            catch (Exception ex)
            {
                try
                {
                    if (errorControl != null)
                        this.ActiveControl = errorControl;
                }
                catch (Exception)
                {
                }

                HandleExceptions.DisplayException(ex);

                try
                {
                    if (postErrorControl != null)
                        this.ActiveControl = postErrorControl;
                }
                catch (Exception)
                {
                }
            }
            return bValid;
        }

        private void EditEmailAlertDialog_Load(object sender, EventArgs e)
        {
            this.ActiveControl = this.nameTBox;
        }

        private void RefreshUI()
        {
            // Configure productsTextBox & eventsTextBox based on this.checkedProductIdList and this.checkedEventIdList
            List<string> productNamesList = new List<string>();
            foreach (int productId in this.checkedProductIdList.Keys)
                productNamesList.Add(this.funcGetProductName(productId));
            productNamesList.Sort();

            StringBuilder toolTipBuilder = new StringBuilder();
            StringBuilder displayBuilder = new StringBuilder();
            foreach (string productName in productNamesList)
            {
                toolTipBuilder.AppendFormat(
                    "{0}{1}",
                    toolTipBuilder.Length == 0 ? "" : "\r\n",
                    productName);
                displayBuilder.AppendFormat(
                    "{0}{1}",
                    displayBuilder.Length == 0 ? "" : ", ",
                    productName);
            }
            this.generalToolTip.SetToolTip(this.productsTextBox, toolTipBuilder.ToString());
            this.productsTextBox.Text = displayBuilder.ToString();

            List<int> eventIdList = new List<int>();
            foreach (int eventId in this.checkedEventIdList.Keys)
                eventIdList.Add(eventId);
            eventIdList.Sort();

            toolTipBuilder = new StringBuilder();
            displayBuilder = new StringBuilder();
            foreach (int eventId in eventIdList)
            {
                //if (eventId == (int)Solimar.Licensing.KeyMessages.EventId.ClientTimeoutProductAndAppInst)
                //{
                //    toolTipBuilder.AppendFormat(
                //        "{0}{1}",
                //        toolTipBuilder.Length == 0 ? "" : "\r\n",
                //        Solimar.Licensing.KeyMessages.GetDescription(eventId));
                //}
                displayBuilder.AppendFormat(
                    "{0}{1}",
                    displayBuilder.Length == 0 ? "" : ", ",
                    eventId);
            }
            this.generalToolTip.SetToolTip(this.eventsTextBox, toolTipBuilder.ToString());
            this.eventsTextBox.Text = displayBuilder.ToString();


            //this.checkedProductIdList.Add(productId, true);
        }
        private void productsConfigureButton_Click(object sender, EventArgs e)
        {
            try
            {
                using (EditEmailAlert_ProductsDialog dialog = new EditEmailAlert_ProductsDialog(this.funcGetProductName, this.imageList))
                {
                    EditEmailAlert_ProductsDialog.DialogData data = new EditEmailAlert_ProductsDialog.DialogData(
                        this.checkedProductIdList.Keys,
                        this.allProductIdList,
                        this.licensedProductIdList
                        );
                    dialog.SetData(data, this.bReadOnly);
                    if (dialog.ShowDialog(this) == DialogResult.OK)
                    {
                        data = dialog.GetData();
                        this.checkedProductIdList.Clear();
                        foreach (int productId in data.checkedProductIdList)
                            this.checkedProductIdList.Add(productId, true);
                        RefreshUI();
                    }
                }
            }
            catch (Exception ex)
            {
                HandleExceptions.DisplayException(ex);
            }
        }

        private void eventsConfigButton_Click(object sender, EventArgs e)
        {
            try
            {
                using (EditEmailAlert_EventsDialog dialog = new EditEmailAlert_EventsDialog())
                {
                    EditEmailAlert_EventsDialog.DialogData data = new EditEmailAlert_EventsDialog.DialogData(
                        this.checkedEventIdList.Keys
                        );
                    dialog.SetData(data, this.bReadOnly);
                    if (dialog.ShowDialog(this) == DialogResult.OK)
                    {
                        data = dialog.GetData();
                        this.checkedEventIdList.Clear();
                        foreach (int eventId in data.checkedEventIdList)
                            this.checkedEventIdList.Add(eventId, true);
                        RefreshUI();
                    }
                }
            }
            catch (Exception ex)
            {
                HandleExceptions.DisplayException(ex);
            }
        }
    }
}
