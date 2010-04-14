using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Text.RegularExpressions;
using Client.Creator.CreatorService;

namespace Client.Creator
{
    public partial class ProductConfigurationDialog : Shared.VisualComponents.DialogBaseForm
    {
        private bool _validVersion;
        public ProductConfigurationDialog(CommunicationLink commLink)
        {
            InitializeComponent();
            _validVersion = false;
            //load property grid with product items
            Client.Creator.ServiceProxy.Service<ICreator>.Use((client) =>
            {
                IList<ProductTable> products = client.GetProducts();
                foreach (ProductTable product in products)
                {
                    if(!product.pName.Contains("Test") && commLink.GetProductName((byte)product.pId) != "Unknown")
                        comboBox1.Items.Add(product.pName);                    
                }
            });
            comboBox1.SelectedIndex = 0;
        }

        private void btnOk_Click(object sender, EventArgs e)
        {
            ValidateProductVersion();
        }

        private void ProductConfigurationDialog_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (DialogResult == DialogResult.OK)
            {
                if (!_validVersion)
                {
                    e.Cancel = true;
                }
                else
                {                    //update product version
                    LicenseVersion version = new LicenseVersion(textBox1.Text);
                    string strVersion;
                    if(version.Minor > 9)
                        strVersion = string.Format("{0}{1}0", version.Major, version.Minor);
                    else
                        strVersion = string.Format("{0}0{1}0", version.Major, version.Minor);
                    int newVersion = Int32.Parse(strVersion, System.Globalization.NumberStyles.HexNumber);
                    ServiceProxy.Service<ICreator>.Use((client) =>
                    {
                        ProductTable productRec = client.GetProduct(comboBox1.SelectedItem.ToString());
                        if (!productRec.pVersion.Equals(newVersion))
                        {
                            productRec.pVersion = newVersion;
                            client.UpdateProductTable(productRec);
                        }
                    });
                    
                }
            }
            
        }

        private void ValidateProductVersion()
        {            
            Regex reg = new Regex(@"\d+\.\d+");
            if (!reg.IsMatch(textBox1.Text))
            {
                MessageBox.Show("Please enter a valid version.",
                    "Input Error",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Error);
                textBox1.Select(0, textBox1.Text.Length);
                textBox1.Focus();
            }
            else
                _validVersion = true;
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            ServiceProxy.Service<ICreator>.Use((client) =>
            {
                ProductTable productRec = client.GetProduct(comboBox1.SelectedItem.ToString());
                string hexVersion = string.Format("{0:x}", productRec.pVersion);
                string minorVersion = hexVersion.Substring(hexVersion.Length - 3, 2);
                string majorVersion = hexVersion.Substring(0, hexVersion.Length - (minorVersion.Length + 1));
                textBox1.Text = string.Format("{0}.{1}", Int32.Parse(majorVersion), Int32.Parse(minorVersion)); 
            });
        }
    }
}
