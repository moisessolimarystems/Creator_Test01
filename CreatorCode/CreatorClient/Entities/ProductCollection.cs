using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Client.Creator
{
    public class ProductCollection
    {
        #region Fields
        private List<ProductLicense> _productCollection;
        private byte _productID;
        #endregion

        #region Constructor
        /// <summary>
        /// Initializes a new instance of the ProductCollection class. 
        /// Represents a collection of Product Licenses
        /// </summary>
        /// <param name="module">Product ID of the collection of product licenses.</param>
        public ProductCollection(byte productID)
        {
            _productCollection = new List<ProductLicense>();
            _productID = productID;
        }
        #endregion

        #region Properties
        /// <summary>
        /// Gets or sets the product ID used to determine the type of product held in the collection of product licenses.
        /// </summary>
        public byte ProductID
        {
            get { return _productID; }
            set { _productID = value; }
        }

        /// <summary>
        /// Gets or sets the collection of product licenses.
        /// </summary>
        public List<ProductLicense> Collection
        {
            get { return _productCollection; }
            set { _productCollection = value; }
        }
        #endregion

        #region Methods
        /// <summary>
        /// Retrieves the product license object from the collection by product license ID.
        /// </summary>
        /// <param name="productLicenseID">ID of the product license to retrieve.</param>
        public ProductLicense GetProductLicenseProperty(string productLicenseID)
        {
            ProductLicense selectedProductLicense = null;
            foreach (ProductLicense plp in _productCollection)
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

        /// <summary>
        /// Updates the specific product license in the collection of product licenses.
        /// </summary>
        /// <param name="productLicense">ProductLicense object to update</param>
        public void UpdateProductProperty(ProductLicense productLicense)
        {
            RemoveProductProperty(productLicense);
            _productCollection.Add(productLicense);
        }

        /// <summary>
        /// Removes the specified product license from the product license collection.
        /// </summary>
        /// <param name="productLicense">ProductLicense object to remove</param>
        public void RemoveProductProperty(ProductLicense productLicense)
        {
            ProductLicense selectedProductLicense = null;
            foreach (ProductLicense plp in _productCollection)
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
        #endregion
    }
}
