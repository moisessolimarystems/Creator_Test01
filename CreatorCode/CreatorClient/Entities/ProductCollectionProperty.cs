using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Client.Creator
{
    public class ProductCollectionProperty
    {
        private List<ProductProperty> _productCollection;
        private uint _productID;

        public ProductCollectionProperty(uint productID)
        {
            _productCollection = new List<ProductProperty>();
            _productID = productID;
        }

        public uint ProductID
        {
            get { return _productID; }
            set { _productID = value; }
        }

        public List<ProductProperty> ProductCollection
        {
            get { return _productCollection; }
            set { _productCollection = value; }
        }

        public ProductProperty GetProductProperty(string productLicense)
        {
            ProductProperty selectedProduct = null;
            foreach (ProductProperty pp in _productCollection)
            {
                if (pp.Product.contractNumber.TVal == productLicense)
                {
                    selectedProduct = pp;
                    break;
                }
            }
            if (selectedProduct == null && _productCollection.Count > 0)
                selectedProduct = _productCollection[0];
            return selectedProduct;
        }

        public void UpdateProductProperty(ProductProperty product)
        {
            RemoveProductProperty(product);
            _productCollection.Add(product);
        }

        public void RemoveProductProperty(ProductProperty product)
        {
            ProductProperty selectedProduct = null;
            foreach (ProductProperty pp in _productCollection)
            {
                if (pp.Product.contractNumber.TVal == product.Product.contractNumber.TVal)
                {
                    selectedProduct = pp;
                    break;
                }
            }
            if(selectedProduct != null)
                _productCollection.Remove(selectedProduct);
        }
    }
}
