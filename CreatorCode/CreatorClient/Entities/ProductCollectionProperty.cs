using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Client.Creator
{
    public class ProductCollectionProperty
    {
        private List<ProductLicenseProperty> _productCollection;
        private byte _productID;

        public ProductCollectionProperty(byte productID)
        {
            _productCollection = new List<ProductLicenseProperty>();
            _productID = productID;
        }

        public byte ProductID
        {
            get { return _productID; }
            set { _productID = value; }
        }

        public List<ProductLicenseProperty> ProductCollection
        {
            get { return _productCollection; }
            set { _productCollection = value; }
        }

        public ProductLicenseProperty GetProductLicenseProperty(string productLicenseID)
        {
            ProductLicenseProperty selectedProductLicense = null;
            foreach (ProductLicenseProperty plp in _productCollection)
            {
                if (plp.ID == productLicenseID)
                {
                    selectedProductLicense = plp;
                    break;
                }
            }
            if (selectedProductLicense == null && _productCollection.Count > 0)
                selectedProductLicense = _productCollection[0];
            return selectedProductLicense;
        }

        public void UpdateProductProperty(ProductLicenseProperty productLicense)
        {
            RemoveProductProperty(productLicense);
            _productCollection.Add(productLicense);
        }

        public void RemoveProductProperty(ProductLicenseProperty productLicense)
        {
            ProductLicenseProperty selectedProductLicense = null;
            foreach (ProductLicenseProperty plp in _productCollection)
            {
                if (plp.ID == productLicense.ID)
                {
                    selectedProductLicense = plp;
                    break;
                }
            }
            if (selectedProductLicense != null)
                _productCollection.Remove(selectedProductLicense);
        }
    }
}
