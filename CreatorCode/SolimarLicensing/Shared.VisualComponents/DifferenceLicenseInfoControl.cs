using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace Shared.VisualComponents
{
    public partial class DifferenceLicenseInfoControl : UserControl
    {
        public DifferenceLicenseInfoControl()
        {
            InitializeComponent();
            Initialize();
        }
        private void Initialize()
        {
            if (filterComboBoxTS.SelectedIndex == -1)
                filterComboBoxTS.SelectedIndex = 0;
        }
        private Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_SoftwareSpecAttribs g_softwareSpec = null;
        private Solimar.Licensing.Attribs.Lic_PackageAttribs g_packageAttribs1 = null;
        private Solimar.Licensing.Attribs.Lic_PackageAttribs g_packageAttribs2 = null;
        public enum FilterType { ftShowAll, ftShowDifference };
        private FilterType g_eFilter = FilterType.ftShowAll;
        public void SetData(Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_SoftwareSpecAttribs _softwareSpec, Solimar.Licensing.Attribs.Lic_PackageAttribs _packageAttribs1, Solimar.Licensing.Attribs.Lic_PackageAttribs _packageAttribs2, FilterType _eFilter)
        {
            g_softwareSpec = _softwareSpec;
            g_packageAttribs1 = _packageAttribs1;
            g_packageAttribs2 = _packageAttribs2;
            if (_eFilter == FilterType.ftShowDifference)
                filterComboBoxTS.SelectedIndex = 1;
            leftListView.Items.Clear();

            rightListView.Items.Clear();

            System.Collections.Generic.List<ItemDifference> diffList1 = new List<ItemDifference>();
            System.Collections.Generic.List<ItemDifference> diffList2 = new List<ItemDifference>();

            if (CalculateDifferences(_packageAttribs1.licLicenseInfoAttribs.TVal, _packageAttribs2.licLicenseInfoAttribs.TVal, ref diffList1, ref diffList2) == true)
            {
                SetData(leftListView, diffList1);
                SetData(rightListView, diffList2);
            }
        }

        //For - Lic_LicenseInfoAttribs
        private bool CalculateDifferences(Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs _licInfoAttribs1, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs _licInfoAttribs2, ref System.Collections.Generic.List<ItemDifference> _refDiffList1, ref System.Collections.Generic.List<ItemDifference> _refDiffList2)
        {
            bool bDifference = false;

            if (_refDiffList1 != null && _refDiffList2 != null && _licInfoAttribs1 != null && _licInfoAttribs2 != null)
            {
                string softwareLicense1 = Solimar.Licensing.Attribs.Lic_LicenseInfoAttribsHelper.GetDisplayLabel(_licInfoAttribs1);
                string softwareLicense2 = Solimar.Licensing.Attribs.Lic_LicenseInfoAttribsHelper.GetDisplayLabel(_licInfoAttribs2);
                StringBuilder sBuilder1 = null;
                StringBuilder sBuilder2 = null;
                string toolTip1 = softwareLicense1;
                string toolTip2 = softwareLicense2;
                string title = "License ID: ";
                int tabs = 0;
                int lineNumber = _refDiffList1.Count == 0 ? 1 : _refDiffList1.Count;
                ItemDifference.DiffType diffType1 = ItemDifference.DiffType.dtSame;
                ItemDifference.DiffType diffType2 = ItemDifference.DiffType.dtSame;
                if (string.Compare(softwareLicense1, softwareLicense2, true) == 0)
                {
                    diffType1 = ItemDifference.DiffType.dtSame | ItemDifference.DiffType.dtAlwaysShow;
                    diffType2 = ItemDifference.DiffType.dtSame | ItemDifference.DiffType.dtAlwaysShow;
                }
                else
                {
                    diffType1 = ItemDifference.DiffType.dtModified | ItemDifference.DiffType.dtAlwaysShow;
                    diffType2 = ItemDifference.DiffType.dtModified | ItemDifference.DiffType.dtAlwaysShow;
                }

                sBuilder1 = new StringBuilder();
                sBuilder2 = new StringBuilder();
                sBuilder1.Append(title);
                sBuilder2.Append(title);
                sBuilder1.Append(softwareLicense1);
                sBuilder2.Append(softwareLicense2);
                _refDiffList1.Add(new ItemDifference(lineNumber, sBuilder1.ToString(), tabs, toolTip1, diffType1));
                _refDiffList2.Add(new ItemDifference(lineNumber, sBuilder2.ToString(), tabs, toolTip2, diffType2));
                lineNumber++;

                #region Software Lic Type:
                if (_licInfoAttribs1.softwareLicType.EVal.CompareTo(_licInfoAttribs2.softwareLicType.EVal) == 0)
                {
                    diffType1 = ItemDifference.DiffType.dtSame;
                    diffType2 = ItemDifference.DiffType.dtSame;
                }
                else
                {
                    diffType1 = ItemDifference.DiffType.dtModified;
                    diffType2 = ItemDifference.DiffType.dtModified;
                }
                sBuilder1 = new StringBuilder();
                sBuilder2 = new StringBuilder();
                title = "Software Lic Type: ";
                sBuilder1.Append(title);
                sBuilder1.Append(_licInfoAttribs1.softwareLicType.EVal.ToString());
                sBuilder2.Append(title);
                sBuilder2.Append(_licInfoAttribs2.softwareLicType.EVal.ToString());
                #endregion
                _refDiffList1.Add(new ItemDifference(lineNumber, sBuilder1.ToString(), tabs + 1, toolTip1, diffType1));
                _refDiffList2.Add(new ItemDifference(lineNumber, sBuilder2.ToString(), tabs + 1, toolTip2, diffType2));
                lineNumber++;

                #region Modified Date:
                if (_licInfoAttribs1.modifiedDate.TVal.CompareTo(_licInfoAttribs2.modifiedDate.TVal) == 0)
                {
                    diffType1 = ItemDifference.DiffType.dtSame;
                    diffType2 = ItemDifference.DiffType.dtSame;
                }
                else
                {
                    diffType1 = ItemDifference.DiffType.dtModified;
                    diffType2 = ItemDifference.DiffType.dtModified;
                }
                sBuilder1 = new StringBuilder();
                sBuilder2 = new StringBuilder();
                title = "Modified Date: ";
                sBuilder1.Append(title);
                sBuilder1.Append(_licInfoAttribs1.modifiedDate.TVal.ToLocalTime());
                sBuilder2.Append(title);
                sBuilder2.Append(_licInfoAttribs2.modifiedDate.TVal.ToLocalTime());
                #endregion
                _refDiffList1.Add(new ItemDifference(lineNumber, sBuilder1.ToString(), tabs + 1, toolTip1, diffType1));
                _refDiffList2.Add(new ItemDifference(lineNumber, sBuilder2.ToString(), tabs + 1, toolTip2, diffType2));
                lineNumber++;

                #region Activations:
                bool pktHasAct1 = _licInfoAttribs1.activationTotal.TVal > 0;
                bool pktHasAct2 = _licInfoAttribs2.activationTotal.TVal > 0;
                if (pktHasAct1 == true || pktHasAct2 == true)
                {
                    #region Calculate Activation difference
                    if (pktHasAct1 == false)
                    {
                        diffType1 = ItemDifference.DiffType.dtSpace;
                        diffType2 = ItemDifference.DiffType.dtAdded;
                    }
                    else if (pktHasAct2 == false)
                    {
                        diffType1 = ItemDifference.DiffType.dtRemoved;
                        diffType2 = ItemDifference.DiffType.dtSpace;
                    }
                    else if (_licInfoAttribs1.activationTotal.TVal != _licInfoAttribs2.activationTotal.TVal)
                    {
                        diffType1 = ItemDifference.DiffType.dtModified;
                        diffType2 = ItemDifference.DiffType.dtModified;
                    }
                    else
                    {
                        diffType1 = ItemDifference.DiffType.dtSame;
                        diffType2 = ItemDifference.DiffType.dtSame;
                    }
                    #endregion

                    #region Activations Total:
                    sBuilder1 = new StringBuilder();
                    sBuilder2 = new StringBuilder();
                    title = "Activations Total: ";
                    if (diffType1 != ItemDifference.DiffType.dtSpace)
                    {
                        sBuilder1.Append(title);
                        sBuilder1.Append(_licInfoAttribs1.activationTotal.TVal.ToString());
                    }
                    if (diffType2 != ItemDifference.DiffType.dtSpace)
                    {
                        sBuilder2.Append(title);
                        sBuilder2.Append(_licInfoAttribs2.activationTotal.TVal.ToString());
                    }
                    #endregion
                    _refDiffList1.Add(new ItemDifference(lineNumber, sBuilder1.ToString(), tabs + 1, toolTip1, diffType1));
                    _refDiffList2.Add(new ItemDifference(lineNumber, sBuilder2.ToString(), tabs + 1, toolTip2, diffType2));
                    lineNumber++;

                    #region Activation Amount in Days:
                    if (diffType1 == ItemDifference.DiffType.dtSame || diffType1 == ItemDifference.DiffType.dtModified)
                    {
                        if (_licInfoAttribs1.activationAmountInDays.TVal != _licInfoAttribs2.activationAmountInDays.TVal)
                        {
                            diffType1 = ItemDifference.DiffType.dtModified;
                            diffType2 = ItemDifference.DiffType.dtModified;
                        }
                        else
                        {
                            diffType1 = ItemDifference.DiffType.dtSame;
                            diffType2 = ItemDifference.DiffType.dtSame;
                        }
                    }
                    sBuilder1 = new StringBuilder();
                    sBuilder2 = new StringBuilder();
                    title = "Activation Amount in Days: ";
                    if (diffType1 != ItemDifference.DiffType.dtSpace)
                    {
                        sBuilder1.Append(title);
                        sBuilder1.Append(_licInfoAttribs1.activationAmountInDays.TVal.ToString());
                    }
                    if (diffType2 != ItemDifference.DiffType.dtSpace)
                    {
                        sBuilder2.Append(title);
                        sBuilder2.Append(_licInfoAttribs2.activationAmountInDays.TVal.ToString());
                    }
                    #endregion
                    _refDiffList1.Add(new ItemDifference(lineNumber, sBuilder1.ToString(), tabs + 1, toolTip1, diffType1));
                    _refDiffList2.Add(new ItemDifference(lineNumber, sBuilder2.ToString(), tabs + 1, toolTip2, diffType2));
                    lineNumber++;

                    #region Activations Current:
                    if (diffType1 == ItemDifference.DiffType.dtSame || diffType1 == ItemDifference.DiffType.dtModified)
                    {
                        if (_licInfoAttribs1.activationCurrent.TVal != _licInfoAttribs2.activationCurrent.TVal)
                        {
                            diffType1 = ItemDifference.DiffType.dtModified;
                            diffType2 = ItemDifference.DiffType.dtModified;
                        }
                        else
                        {
                            diffType1 = ItemDifference.DiffType.dtSame;
                            diffType2 = ItemDifference.DiffType.dtSame;
                        }
                    }
                    sBuilder1 = new StringBuilder();
                    sBuilder2 = new StringBuilder();
                    title = "Activations Current: ";
                    if (diffType1 != ItemDifference.DiffType.dtSpace)
                    {
                        sBuilder1.Append(title);
                        sBuilder1.Append(_licInfoAttribs1.activationCurrent.TVal.ToString());
                    }
                    if (diffType2 != ItemDifference.DiffType.dtSpace)
                    {
                        sBuilder2.Append(title);
                        sBuilder2.Append(_licInfoAttribs2.activationCurrent.TVal.ToString());
                    }
                    #endregion
                    _refDiffList1.Add(new ItemDifference(lineNumber, sBuilder1.ToString(), tabs + 1, toolTip1, diffType1));
                    _refDiffList2.Add(new ItemDifference(lineNumber, sBuilder2.ToString(), tabs + 1, toolTip2, diffType2));
                    lineNumber++;

                    #region Activation Expiration Date:
                    if (diffType1 == ItemDifference.DiffType.dtSame || diffType1 == ItemDifference.DiffType.dtModified)
                    {
                        if (_licInfoAttribs1.activationExpirationDate.TVal.CompareTo(_licInfoAttribs2.activationExpirationDate.TVal) != 0)
                        {
                            diffType1 = ItemDifference.DiffType.dtModified;
                            diffType2 = ItemDifference.DiffType.dtModified;
                        }
                        else
                        {
                            diffType1 = ItemDifference.DiffType.dtSame;
                            diffType2 = ItemDifference.DiffType.dtSame;
                        }
                    }
                    sBuilder1 = new StringBuilder();
                    sBuilder2 = new StringBuilder();
                    title = "Activation Expiration Date:";
                    if (diffType1 != ItemDifference.DiffType.dtSpace)
                    {
                        sBuilder1.Append(title);
                        sBuilder1.Append(_licInfoAttribs1.activationExpirationDate.TVal.ToLocalTime());
                    }
                    if (diffType2 != ItemDifference.DiffType.dtSpace)
                    {
                        sBuilder2.Append(title);
                        sBuilder2.Append(_licInfoAttribs2.activationExpirationDate.TVal.ToLocalTime());
                    }
                    #endregion
                    _refDiffList1.Add(new ItemDifference(lineNumber, sBuilder1.ToString(), tabs + 1, toolTip1, diffType1));
                    _refDiffList2.Add(new ItemDifference(lineNumber, sBuilder2.ToString(), tabs + 1, toolTip2, diffType2));
                    lineNumber++;
                }
                #endregion

                #region Product List:
                title = "Product List:";
                diffType1 = ItemDifference.DiffType.dtSame | ItemDifference.DiffType.dtAlwaysShow;
                diffType2 = ItemDifference.DiffType.dtSame | ItemDifference.DiffType.dtAlwaysShow;
                _refDiffList1.Add(new ItemDifference(lineNumber, title, tabs + 1, toolTip1, diffType1));
                _refDiffList2.Add(new ItemDifference(lineNumber, title, tabs + 1, toolTip2, diffType2));
                lineNumber++;
                //_licInfoAttribs1.productList
                CalculateDifferences(ref lineNumber,
                        tabs + 2,
                        toolTip1,
                        toolTip2,
                        _licInfoAttribs1.productList,
                        _licInfoAttribs2.productList,
                        ref _refDiffList1,
                        ref _refDiffList2);
                #endregion

                #region Validation List:
                //title = "Validation List:";
                //diffType1 = ItemDifference.DiffType.dtSame;
                //diffType2 = ItemDifference.DiffType.dtSame;
                //_refDiffList1.Add(new ItemDifference(lineNumber, title, tabs + 1, diffType1));
                //_refDiffList2.Add(new ItemDifference(lineNumber, title, tabs + 1, diffType2));
                //lineNumber++;
                CalculateDifferences(   ref lineNumber, 
                                        tabs + 1,
                                        toolTip1,
                                        toolTip2,
                                        _licInfoAttribs1.licVerificationAttribs, 
                                        _licInfoAttribs2.licVerificationAttribs, 
                                        ref _refDiffList1, 
                                        ref _refDiffList2);
                #endregion
                bDifference = true;
            }
            return bDifference;
        }

        private class ProdInfoDiff
        {
            public ProdInfoDiff(uint _prodID,
                                Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs _prodInfo1,
                                Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs _prodInfo2)
            {
                this.prodID = _prodID;
                this.prodInfo1 = _prodInfo1;
                this.prodInfo2 = _prodInfo2;
            }
            public uint prodID;
            public Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs prodInfo1;
            public Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs prodInfo2;
        }
        //For - Lic_ProductInfoAttribsList
        private bool CalculateDifferences(  ref int _refLineNumber, 
                                            int _tabs,
                                            string _toolTip1,
                                            string _toolTip2,
                                            Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ProductInfoAttribsList _licProdInfoListAttribs1,
                                            Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ProductInfoAttribsList _licProdInfoListAttribs2, 
                                            ref System.Collections.Generic.List<ItemDifference> _refDiffList1, 
                                            ref System.Collections.Generic.List<ItemDifference> _refDiffList2)
        {
            bool bDifference = false;
            if (_refDiffList1 != null && _refDiffList2 != null && _licProdInfoListAttribs1 != null && _licProdInfoListAttribs2 != null)
            {
                System.Collections.Generic.Dictionary<uint, ProdInfoDiff> diffHash = new Dictionary<uint, ProdInfoDiff>();
                foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs prodInfo in _licProdInfoListAttribs1.TVal)
                {
                    if (diffHash.ContainsKey(prodInfo.productID.TVal) == false)
                        diffHash.Add(prodInfo.productID.TVal, new ProdInfoDiff(prodInfo.productID.TVal, prodInfo, null));
                }
                foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs prodInfo in _licProdInfoListAttribs2.TVal)
                {
                    if (diffHash.ContainsKey(prodInfo.productID.TVal) == false)
                        diffHash.Add(prodInfo.productID.TVal, new ProdInfoDiff(prodInfo.productID.TVal, null, prodInfo));
                    else
                        diffHash[prodInfo.productID.TVal].prodInfo2 = prodInfo;
                }

                string title = "";
                StringBuilder sBuilder1 = null;
                StringBuilder sBuilder2 = null;

                ItemDifference.DiffType diffType1 = ItemDifference.DiffType.dtSame;
                ItemDifference.DiffType diffType2 = ItemDifference.DiffType.dtSame;

                ItemDifference.DiffType diffInternalType1 = ItemDifference.DiffType.dtSame;
                ItemDifference.DiffType diffInternalType2 = ItemDifference.DiffType.dtSame;
                string toolTip1 = "";
                string toolTip2 = "";
                foreach (uint prodID in diffHash.Keys)
                {
                    if (!(diffHash[prodID].prodInfo1==null && diffHash[prodID].prodInfo2==null))
                    {
                        if (diffHash[prodID].prodInfo1 == null)
                        {
                            diffType1 = ItemDifference.DiffType.dtSpace;
                            diffType2 = ItemDifference.DiffType.dtAdded;
                        }
                        else if (diffHash[prodID].prodInfo2 == null)
                        {
                            diffType1 = ItemDifference.DiffType.dtRemoved;
                            diffType2 = ItemDifference.DiffType.dtSpace;
                        }
                        else
                        {
                            diffType1 = ItemDifference.DiffType.dtSame;
                            diffType2 = ItemDifference.DiffType.dtSame;
                        }

                        title = Solimar.Licensing.Attribs.Lic_LicenseInfoAttribsHelper.GetProductName(g_softwareSpec, prodID);
                        sBuilder1 = new StringBuilder();
                        sBuilder2 = new StringBuilder();
                        toolTip1 = _toolTip1 + " -> " + title;
                        toolTip2 = _toolTip2 + " -> " + title;
                        if (diffType1 != ItemDifference.DiffType.dtSpace)
                            sBuilder1.Append(title);
                        if (diffType2 != ItemDifference.DiffType.dtSpace)
                            sBuilder2.Append(title);

                        _refDiffList1.Add(new ItemDifference(_refLineNumber, sBuilder1.ToString(), _tabs, toolTip1, diffType1));
                        _refDiffList2.Add(new ItemDifference(_refLineNumber, sBuilder2.ToString(), _tabs, toolTip2, diffType2));
                        _refLineNumber++;

                        #region Product Version:
                        diffInternalType1 = diffType1;
                        diffInternalType2 = diffType2;
                        if (diffType1 == ItemDifference.DiffType.dtSame && diffType2 == ItemDifference.DiffType.dtSame)
                        {
                            if (diffHash[prodID].prodInfo1.product_Major.TVal == diffHash[prodID].prodInfo2.product_Major.TVal &&
                                diffHash[prodID].prodInfo1.product_Minor.TVal == diffHash[prodID].prodInfo2.product_Minor.TVal &&
                                diffHash[prodID].prodInfo1.product_SubMajor.TVal == diffHash[prodID].prodInfo2.product_SubMajor.TVal &&
                                diffHash[prodID].prodInfo1.product_SubMinor.TVal == diffHash[prodID].prodInfo2.product_SubMinor.TVal)
                            {
                                diffInternalType1 = ItemDifference.DiffType.dtSame;
                                diffInternalType2 = ItemDifference.DiffType.dtSame;
                            }
                            else
                            {
                                diffInternalType1 = ItemDifference.DiffType.dtModified;
                                diffInternalType2 = ItemDifference.DiffType.dtModified;
                            }

                        }
                        title = "Product Version: ";
                        sBuilder1 = new StringBuilder();
                        sBuilder2 = new StringBuilder();
                        if (diffType1 != ItemDifference.DiffType.dtSpace)
                        {
                            sBuilder1.Append(title);
                            sBuilder1.Append(diffHash[prodID].prodInfo1.product_Major.TVal.ToString());
                            sBuilder1.Append(".");
                            sBuilder1.Append(diffHash[prodID].prodInfo1.product_Minor.TVal.ToString());
                            sBuilder1.Append(".");
                            sBuilder1.Append(diffHash[prodID].prodInfo1.product_SubMajor.TVal.ToString());
                            sBuilder1.Append(".");
                            sBuilder1.Append(diffHash[prodID].prodInfo1.product_SubMinor.TVal.ToString());
                        }
                        if (diffType2 != ItemDifference.DiffType.dtSpace)
                        {
                            sBuilder2.Append(title);
                            sBuilder2.Append(diffHash[prodID].prodInfo2.product_Major.TVal.ToString());
                            sBuilder2.Append(".");
                            sBuilder2.Append(diffHash[prodID].prodInfo2.product_Minor.TVal.ToString());
                            sBuilder2.Append(".");
                            sBuilder2.Append(diffHash[prodID].prodInfo2.product_SubMajor.TVal.ToString());
                            sBuilder2.Append(".");
                            sBuilder2.Append(diffHash[prodID].prodInfo2.product_SubMinor.TVal.ToString());
                        }

                        #endregion
                        _refDiffList1.Add(new ItemDifference(_refLineNumber, sBuilder1.ToString(), _tabs + 1, toolTip1, diffInternalType1));
                        _refDiffList2.Add(new ItemDifference(_refLineNumber, sBuilder2.ToString(), _tabs + 1, toolTip2, diffInternalType2));
                        _refLineNumber++;

                        #region Product App Instance:
                        diffInternalType1 = diffType1;
                        diffInternalType2 = diffType2;
                        if (diffType1 == ItemDifference.DiffType.dtSame && diffType2 == ItemDifference.DiffType.dtSame)
                        {
                            if (diffHash[prodID].prodInfo1.productAppInstance.TVal == diffHash[prodID].prodInfo2.productAppInstance.TVal)
                            {
                                diffInternalType1 = ItemDifference.DiffType.dtSame;
                                diffInternalType2 = ItemDifference.DiffType.dtSame;
                            }
                            else
                            {
                                diffInternalType1 = ItemDifference.DiffType.dtModified;
                                diffInternalType2 = ItemDifference.DiffType.dtModified;
                            }
                        }
                        title = "Product App Instance: ";
                        sBuilder1 = new StringBuilder();
                        sBuilder2 = new StringBuilder();
                        if (diffType1 != ItemDifference.DiffType.dtSpace)
                        {
                            sBuilder1.Append(title);
                            sBuilder1.Append(diffHash[prodID].prodInfo1.productAppInstance.TVal.ToString());
                        }
                        if (diffType2 != ItemDifference.DiffType.dtSpace)
                        {
                            sBuilder2.Append(title);
                            sBuilder2.Append(diffHash[prodID].prodInfo2.productAppInstance.TVal.ToString());
                        }

                        #endregion
                        _refDiffList1.Add(new ItemDifference(_refLineNumber, sBuilder1.ToString(), _tabs + 1, toolTip1, diffInternalType1));
                        _refDiffList2.Add(new ItemDifference(_refLineNumber, sBuilder2.ToString(), _tabs + 1, toolTip2, diffInternalType2));
                        _refLineNumber++;

                        #region Module List:
                        title = "Module List";
                        sBuilder1 = new StringBuilder();
                        sBuilder2 = new StringBuilder();
                        if (diffType1 != ItemDifference.DiffType.dtSpace)
                            sBuilder1.Append(title);
                        if (diffType2 != ItemDifference.DiffType.dtSpace)
                            sBuilder2.Append(title);

                        _refDiffList1.Add(new ItemDifference(_refLineNumber, sBuilder1.ToString(), _tabs + 1, toolTip1, diffType1));
                        _refDiffList2.Add(new ItemDifference(_refLineNumber, sBuilder2.ToString(), _tabs + 1, toolTip2, diffType2));
                        _refLineNumber++;

                        CalculateDifferences(   ref _refLineNumber,
                                                _tabs + 2,
                                                toolTip1,
                                                toolTip2,
                                                prodID,
                                                (diffHash[prodID].prodInfo1 != null) ? diffHash[prodID].prodInfo1.moduleList : null,
                                                (diffHash[prodID].prodInfo2 != null) ? diffHash[prodID].prodInfo2.moduleList : null,
                                                ref _refDiffList1,
                                                ref _refDiffList2);
                        #endregion
                    }
                }
            }
            return bDifference;
        }

        private class ModInfoListDiff
        {
            public ModInfoListDiff(uint _modID)
            {
                this.modID = _modID;
                this.mod1List = new List<Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs>();
                this.mod2List = new List<Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs>();
            }
            public uint modID;
            public System.Collections.Generic.List<Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs> mod1List;
            public System.Collections.Generic.List<Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs> mod2List;
        }
        private class ModInfoDiff
        {
            public ModInfoDiff(Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs _modInfo1, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs _modInfo2)
            {
                this.modInfo1 = _modInfo1;
                this.modInfo2 = _modInfo2;
            }
            public Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs modInfo1;
            public Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs modInfo2;
        }
        //For - Lic_ModuleInfoAttribsList
        private bool CalculateDifferences(  ref int _refLineNumber,
                                            int _tabs,
                                            string _toolTip1,
                                            string _toolTip2,
                                            uint _prodID,
                                            Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs.Lic_ModuleInfoAttribsList _licModInfoListAttribs1,
                                            Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs.Lic_ModuleInfoAttribsList _licModInfoListAttribs2,
                                            ref System.Collections.Generic.List<ItemDifference> _refDiffList1,
                                            ref System.Collections.Generic.List<ItemDifference> _refDiffList2)
        {
            bool bDifference = false;
            if (_refDiffList1 != null && _refDiffList2 != null)
            {
                System.Collections.Generic.Dictionary<uint, ModInfoListDiff> diffHash = new Dictionary<uint, ModInfoListDiff>();
                if (_licModInfoListAttribs1 != null)
                {
                    foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs modInfo in _licModInfoListAttribs1.TVal)
                    {
                        if (diffHash.ContainsKey(modInfo.moduleID) == false)
                            diffHash.Add(modInfo.moduleID, new ModInfoListDiff(modInfo.moduleID));
                        diffHash[modInfo.moduleID].mod1List.Add(modInfo);
                    }
                }

                if (_licModInfoListAttribs2 != null)
                {
                    foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs modInfo in _licModInfoListAttribs2.TVal)
                    {
                        if (diffHash.ContainsKey(modInfo.moduleID) == false)
                            diffHash.Add(modInfo.moduleID, new ModInfoListDiff(modInfo.moduleID));
                        diffHash[modInfo.moduleID].mod2List.Add(modInfo);
                    }
                }

                ItemDifference.DiffType diffType1 = ItemDifference.DiffType.dtSame;
                ItemDifference.DiffType diffType2 = ItemDifference.DiffType.dtSame;
                ItemDifference.DiffType diffInternalType1 = ItemDifference.DiffType.dtSame;
                ItemDifference.DiffType diffInternalType2 = ItemDifference.DiffType.dtSame;
                string title = "";
                StringBuilder sBuilder1 = null;
                StringBuilder sBuilder2 = null;
                string toolTip1 = "";
                string toolTip2 = "";
                System.Collections.Generic.List<ModInfoDiff> modDiffList = new List<ModInfoDiff>();
                
                foreach (uint modID in diffHash.Keys)
                {
                    modDiffList = MatchModuleDifferences(diffHash[modID]);

                    foreach (ModInfoDiff keyPair in modDiffList)
                    {
                        //if (!(keyPair.modInfo1 != null || keyPair.modInfo2 != null))
                        {
                            if (keyPair.modInfo1 == null)
                            {
                                diffType1 = ItemDifference.DiffType.dtSpace;
                                diffType2 = ItemDifference.DiffType.dtAdded;
                            }
                            else if (keyPair.modInfo2 == null)
                            {
                                diffType1 = ItemDifference.DiffType.dtRemoved;
                                diffType2 = ItemDifference.DiffType.dtSpace;
                            }
                            else
                            {
                                diffType1 = ItemDifference.DiffType.dtSame;
                                diffType2 = ItemDifference.DiffType.dtSame;
                            }

                            title = Solimar.Licensing.Attribs.Lic_LicenseInfoAttribsHelper.GetModuleName(g_softwareSpec, _prodID, modID);
                            toolTip1 = _toolTip1 + " -> " + title;
                            toolTip2 = _toolTip2 + " -> " + title;
                            sBuilder1 = new StringBuilder();
                            sBuilder2 = new StringBuilder();

                            if (diffType1 != ItemDifference.DiffType.dtSpace)
                            {
                                sBuilder1.Append(title);
                                sBuilder1.Append(": ");
                            }
                            if (diffType2 != ItemDifference.DiffType.dtSpace)
                            {
                                sBuilder2.Append(title);
                                sBuilder2.Append(": ");
                            }

                            _refDiffList1.Add(new ItemDifference(_refLineNumber, sBuilder1.ToString(), _tabs, toolTip1, diffType1));
                            _refDiffList2.Add(new ItemDifference(_refLineNumber, sBuilder2.ToString(), _tabs, toolTip2, diffType2));
                            _refLineNumber++;

                            #region Module Value:
                            diffInternalType1 = diffType1;
                            diffInternalType2 = diffType2;
                            if (diffType1 == ItemDifference.DiffType.dtSame && diffType2 == ItemDifference.DiffType.dtSame)
                            {
                                if (keyPair.modInfo1.moduleValue.TVal != keyPair.modInfo2.moduleValue.TVal)
                                {
                                    diffInternalType1 = ItemDifference.DiffType.dtModified;
                                    diffInternalType2 = ItemDifference.DiffType.dtModified;
                                }
                                else
                                {
                                    diffInternalType1 = ItemDifference.DiffType.dtSame;
                                    diffInternalType2 = ItemDifference.DiffType.dtSame;
                                }
                            }

                            title = "Module Value: ";
                            sBuilder1 = new StringBuilder();
                            if (diffInternalType1 != ItemDifference.DiffType.dtSpace)
                            {
                                sBuilder1.Append(title);
                                sBuilder1.Append(keyPair.modInfo1.moduleValue.TVal.ToString());
                            }
                            sBuilder2 = new StringBuilder();
                            if (diffInternalType2 != ItemDifference.DiffType.dtSpace)
                            {
                                sBuilder2.Append(title);
                                sBuilder2.Append(keyPair.modInfo2.moduleValue.TVal.ToString());
                            }
                            #endregion
                            _refDiffList1.Add(new ItemDifference(_refLineNumber, sBuilder1.ToString(), _tabs + 1, toolTip1, diffInternalType1));
                            _refDiffList2.Add(new ItemDifference(_refLineNumber, sBuilder2.ToString(), _tabs + 1, toolTip2, diffInternalType2));
                            _refLineNumber++;

                            #region Module AppInstance:
                            diffInternalType1 = diffType1;
                            diffInternalType2 = diffType2;
                            if (diffType1 == ItemDifference.DiffType.dtSame && diffType2 == ItemDifference.DiffType.dtSame)
                            {
                                if (keyPair.modInfo1.moduleAppInstance.TVal != keyPair.modInfo2.moduleAppInstance.TVal)
                                {
                                    diffInternalType1 = ItemDifference.DiffType.dtModified;
                                    diffInternalType2 = ItemDifference.DiffType.dtModified;
                                }
                                else
                                {
                                    diffInternalType1 = ItemDifference.DiffType.dtSame;
                                    diffInternalType2 = ItemDifference.DiffType.dtSame;
                                }
                            }

                            title = "Module Application Instance: ";
                            sBuilder1 = new StringBuilder();
                            if (diffInternalType1 != ItemDifference.DiffType.dtSpace)
                            {
                                sBuilder1.Append(title);
                                sBuilder1.Append(keyPair.modInfo1.moduleAppInstance.TVal.ToString());
                            }
                            sBuilder2 = new StringBuilder();
                            if (diffInternalType2 != ItemDifference.DiffType.dtSpace)
                            {
                                sBuilder2.Append(title);
                                sBuilder2.Append(keyPair.modInfo2.moduleAppInstance.TVal.ToString());
                            }
                            #endregion
                            _refDiffList1.Add(new ItemDifference(_refLineNumber, sBuilder1.ToString(), _tabs + 1, toolTip1, diffInternalType1));
                            _refDiffList2.Add(new ItemDifference(_refLineNumber, sBuilder2.ToString(), _tabs + 1, toolTip2, diffInternalType2));
                            _refLineNumber++;

                            #region Module ModState:
                            diffInternalType1 = diffType1;
                            diffInternalType2 = diffType2;
                            if (diffType1 == ItemDifference.DiffType.dtSame && diffType2 == ItemDifference.DiffType.dtSame)
                            {
                                if (keyPair.modInfo1.moduleState.EVal.CompareTo(keyPair.modInfo2.moduleState.EVal) != 0)
                                {
                                    diffInternalType1 = ItemDifference.DiffType.dtModified;
                                    diffInternalType2 = ItemDifference.DiffType.dtModified;
                                }
                                else
                                {
                                    diffInternalType1 = ItemDifference.DiffType.dtSame;
                                    diffInternalType2 = ItemDifference.DiffType.dtSame;
                                }
                            }

                            title = "Module State: ";
                            sBuilder1 = new StringBuilder();
                            if (diffInternalType1 != ItemDifference.DiffType.dtSpace)
                            {
                                sBuilder1.Append(title);
                                sBuilder1.Append(keyPair.modInfo1.moduleState.GetAlias());
                            }
                            sBuilder2 = new StringBuilder();
                            if (diffInternalType2 != ItemDifference.DiffType.dtSpace)
                            {
                                sBuilder2.Append(title);
                                sBuilder2.Append(keyPair.modInfo2.moduleState.GetAlias());
                            }
                            #endregion
                            _refDiffList1.Add(new ItemDifference(_refLineNumber, sBuilder1.ToString(), _tabs + 1, toolTip1, diffInternalType1));
                            _refDiffList2.Add(new ItemDifference(_refLineNumber, sBuilder2.ToString(), _tabs + 1, toolTip2, diffInternalType2));
                            _refLineNumber++;

                            #region Module ModExpirationDate:
                            diffInternalType1 = diffType1;
                            diffInternalType2 = diffType2;
                            if (diffType1 == ItemDifference.DiffType.dtSame && diffType2 == ItemDifference.DiffType.dtSame)
                            {
                                if (keyPair.modInfo1.moduleExpirationDate.TVal.CompareTo(keyPair.modInfo2.moduleExpirationDate.TVal) != 0)
                                {
                                    diffInternalType1 = ItemDifference.DiffType.dtModified;
                                    diffInternalType2 = ItemDifference.DiffType.dtModified;
                                }
                                else
                                {
                                    diffInternalType1 = ItemDifference.DiffType.dtSame;
                                    diffInternalType2 = ItemDifference.DiffType.dtSame;
                                }
                            }

                            if ((keyPair.modInfo1 != null && (keyPair.modInfo1.moduleExpirationDate.TVal.CompareTo(new DateTime(1900, 1, 1)) != 0)) ||
                                (keyPair.modInfo2 != null && (keyPair.modInfo2.moduleExpirationDate.TVal.CompareTo(new DateTime(1900, 1, 1)) != 0)))
                            {
                                title = "Module Expiration Date: ";
                                sBuilder1 = new StringBuilder();
                                if (diffInternalType1 != ItemDifference.DiffType.dtSpace)
                                {
                                    sBuilder1.Append(title);
                                    sBuilder1.Append(keyPair.modInfo1.moduleExpirationDate.TVal.ToLocalTime());
                                }
                                sBuilder2 = new StringBuilder();
                                if (diffInternalType2 != ItemDifference.DiffType.dtSpace)
                                {
                                    sBuilder2.Append(title);
                                    sBuilder2.Append(keyPair.modInfo2.moduleExpirationDate.TVal.ToLocalTime());
                                }
                                _refDiffList1.Add(new ItemDifference(_refLineNumber, sBuilder1.ToString(), _tabs + 1, toolTip1, diffInternalType1));
                                _refDiffList2.Add(new ItemDifference(_refLineNumber, sBuilder2.ToString(), _tabs + 1, toolTip2, diffInternalType2));
                                _refLineNumber++;
                            }
                            #endregion

                            #region Order Number:
                            diffInternalType1 = diffType1;
                            diffInternalType2 = diffType2;
                            if (diffType1 == ItemDifference.DiffType.dtSame && diffType2 == ItemDifference.DiffType.dtSame)
                            {
                                if(string.Compare(keyPair.modInfo1.contractNumber.TVal, keyPair.modInfo2.contractNumber.TVal) != 0)
                                {
                                    diffInternalType1 = ItemDifference.DiffType.dtModified;
                                    diffInternalType2 = ItemDifference.DiffType.dtModified;
                                }
                                else
                                {
                                    diffInternalType1 = ItemDifference.DiffType.dtSame;
                                    diffInternalType2 = ItemDifference.DiffType.dtSame;
                                }
                            }

                            title = "Order Number: ";
                            sBuilder1 = new StringBuilder();
                            if (diffInternalType1 != ItemDifference.DiffType.dtSpace)
                            {
                                sBuilder1.Append(title);
                                sBuilder1.Append(keyPair.modInfo1.contractNumber.TVal);
                            }
                            sBuilder2 = new StringBuilder();
                            if (diffInternalType2 != ItemDifference.DiffType.dtSpace)
                            {
                                sBuilder2.Append(title);
                                sBuilder2.Append(keyPair.modInfo2.contractNumber.TVal);
                            }
                            #endregion
                            _refDiffList1.Add(new ItemDifference(_refLineNumber, sBuilder1.ToString(), _tabs + 1, toolTip1, diffInternalType1));
                            _refDiffList2.Add(new ItemDifference(_refLineNumber, sBuilder2.ToString(), _tabs + 1, toolTip2, diffInternalType2));
                            _refLineNumber++;
                        }
                    }
                }

                bDifference = true;
            }
            
            return bDifference;
        }
        private System.Collections.Generic.List<ModInfoDiff> MatchModuleDifferences(ModInfoListDiff _modInfoList)
        {
            System.Collections.Generic.List<ModInfoDiff> modDiffList = new List<ModInfoDiff>();
            if (_modInfoList != null)
            {
                foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs modInfo in _modInfoList.mod1List)
                {
                    modDiffList.Add(new ModInfoDiff(modInfo, null));
                }

                //for testing
                foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs modInfo in _modInfoList.mod2List)
                {
                    //modDiffList.Add(new ModInfoDiff(modInfo, null));
                    ModInfoDiff matchingPair = null;
                    foreach (ModInfoDiff keyPair in modDiffList)
                    {
                        //calculate best match
                        //For testing just use last one seen
                        if (keyPair.modInfo2 == null)
                            matchingPair = keyPair;

                    }
                    if (matchingPair == null)
                    {
                        matchingPair = new ModInfoDiff(null, null);
                        modDiffList.Add(matchingPair);
                    }

                    matchingPair.modInfo2 = modInfo;
                }
            }


            return modDiffList;
        }

        //For - AttribsMemberAttribsClass_Lic_VerificationAttribs
        private bool CalculateDifferences(ref int _refLineNumber, int _tabs, string _toolTip1, string _toolTip2, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.AttribsMemberAttribsClass_Lic_VerificationAttribs _licVerficationAttribs1, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.AttribsMemberAttribsClass_Lic_VerificationAttribs _licVerficationAttribs2, ref System.Collections.Generic.List<ItemDifference> _refDiffList1, ref System.Collections.Generic.List<ItemDifference> _refDiffList2)
        {
            bool bDifference = false;
            if (_refDiffList1 != null && _refDiffList2 != null && _licVerficationAttribs1 != null && _licVerficationAttribs2 != null)
            {
                string title = "";
                
                #region Validation List:
                title = "Validation Token List:";
                _refDiffList1.Add(new ItemDifference(_refLineNumber, title, _tabs, _toolTip1, ItemDifference.DiffType.dtSame | ItemDifference.DiffType.dtAlwaysShow));
                _refDiffList2.Add(new ItemDifference(_refLineNumber, title, _tabs, _toolTip2, ItemDifference.DiffType.dtSame | ItemDifference.DiffType.dtAlwaysShow));
                _refLineNumber++;

                CalculateDifferences(   ref _refLineNumber, 
                                        _tabs + 1,
                                        _toolTip1,
                                        _toolTip2,
                                        _licVerficationAttribs1.TVal.validationTokenList, 
                                        _licVerficationAttribs2.TVal.validationTokenList, 
                                        ref _refDiffList1, 
                                        ref _refDiffList2);

                #endregion

                #region Verification History List:
                title = "Verification History List:";
                _refDiffList1.Add(new ItemDifference(_refLineNumber, title, _tabs, _toolTip1, ItemDifference.DiffType.dtSame | ItemDifference.DiffType.dtAlwaysShow));
                _refDiffList2.Add(new ItemDifference(_refLineNumber, title, _tabs, _toolTip2, ItemDifference.DiffType.dtSame | ItemDifference.DiffType.dtAlwaysShow));
                _refLineNumber++;

                #endregion



                bDifference = true;
            }
            return bDifference;
        }

        private class ValTokenDiff
        {
            public ValTokenDiff(Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType _tokenType, string _value1, string _value2)
            {
                this.tokenType = _tokenType;
                this.value1 = _value1;
                this.value2 = _value2;
            }
            public Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType tokenType;
            public string value1;
            public string value2;
        }
        //For - Lic_ValidationTokenAttribsList
        private bool CalculateDifferences(ref int _refLineNumber, int _tabs, string _toolTip1, string _toolTip2, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_VerificationAttribs.Lic_ValidationTokenAttribsList _licValidationTokenListAttribs1, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_VerificationAttribs.Lic_ValidationTokenAttribsList _licValidationTokenListAttribs2, ref System.Collections.Generic.List<ItemDifference> _refDiffList1, ref System.Collections.Generic.List<ItemDifference> _refDiffList2)
        {
            
            bool bDifference = false;
            if (_refDiffList1 != null && _refDiffList2 != null && _licValidationTokenListAttribs1 != null && _licValidationTokenListAttribs2 != null)
            {
                System.Collections.Generic.Dictionary<Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType, ValTokenDiff> diffHash = new Dictionary<Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType, ValTokenDiff>();
                foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs valToken in _licValidationTokenListAttribs1.TVal)
                {
                    if (diffHash.ContainsKey(valToken.tokenType) == false)
                        diffHash.Add(valToken.tokenType, new ValTokenDiff(valToken.tokenType, valToken.tokenValue.TVal, ""));
                }

                foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs valToken in _licValidationTokenListAttribs2.TVal)
                {
                    if (diffHash.ContainsKey(valToken.tokenType) == false)
                        diffHash.Add(valToken.tokenType, new ValTokenDiff(valToken.tokenType, "", valToken.tokenValue.TVal));
                    else
                        diffHash[valToken.tokenType].value2 = valToken.tokenValue.TVal;
                }

                string title = "";
                StringBuilder sBuilder1 = null;
                StringBuilder sBuilder2 = null;

                ItemDifference.DiffType diffType1 = ItemDifference.DiffType.dtSame;
                ItemDifference.DiffType diffType2 = ItemDifference.DiffType.dtSame;
                foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType tokenType in diffHash.Keys)
                {
                    if (!(diffHash[tokenType].value1.Length == 0 && diffHash[tokenType].value2.Length == 0))
                    {
                        if (diffHash[tokenType].value1.Length == 0)
                        {
                            diffType1 = ItemDifference.DiffType.dtSpace;
                            diffType2 = ItemDifference.DiffType.dtAdded;
                        }
                        else if (diffHash[tokenType].value2.Length == 0)
                        {
                            diffType1 = ItemDifference.DiffType.dtRemoved;
                            diffType2 = ItemDifference.DiffType.dtSpace;
                        }
                        else if (string.Compare(diffHash[tokenType].value1, diffHash[tokenType].value2, true) != 0)
                        {
                            diffType1 = ItemDifference.DiffType.dtModified;
                            diffType2 = ItemDifference.DiffType.dtModified;
                        }
                        else
                        {
                            diffType1 = ItemDifference.DiffType.dtSame;
                            diffType2 = ItemDifference.DiffType.dtSame;
                        }
                        title = Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.AttribsMemberEnum_TTokenType.GetAlias(tokenType);
                        sBuilder1 = new StringBuilder();
                        sBuilder2 = new StringBuilder();
                        if (diffType1 != ItemDifference.DiffType.dtSpace)
                        {
                            sBuilder1.Append(title);
                            sBuilder1.Append(": ");
                            sBuilder1.Append(diffHash[tokenType].value1);
                        }
                        if (diffType2 != ItemDifference.DiffType.dtSpace)
                        {
                            sBuilder2.Append(title);
                            sBuilder2.Append(": ");
                            sBuilder2.Append(diffHash[tokenType].value2);
                        }
                        _refDiffList1.Add(new ItemDifference(_refLineNumber, sBuilder1.ToString(), _tabs, _toolTip1, diffType1));
                        _refDiffList2.Add(new ItemDifference(_refLineNumber, sBuilder2.ToString(), _tabs, _toolTip2, diffType2));
                        _refLineNumber++;
                    }
                }
            }
            return bDifference;
        }

        

        private void SetData(ListView _listView, System.Collections.Generic.List<ItemDifference> _diffList)
        {
            if (_listView != null && _diffList != null)
            {
                foreach (ItemDifference itemDiff in _diffList)
                {
                    if (DisplayItem(itemDiff.viewType))
                        _listView.Items.Add(NewListViewItem(itemDiff.lineNumber, itemDiff.line, itemDiff.ident, itemDiff.toolTip, CalculateBackColor(itemDiff.viewType), CalculateFontStyle(itemDiff.viewType)));
                }
            }
        }
        private bool DisplayItem(ItemDifference.DiffType _viewType)
        {
            return (    ((_viewType & ItemDifference.DiffType.dtAlwaysShow)!=0) ||
                        g_eFilter == FilterType.ftShowAll ||
                        (g_eFilter == FilterType.ftShowDifference && ((_viewType & ItemDifference.DiffType.dtAdded) != 0)) ||
                        (g_eFilter == FilterType.ftShowDifference && ((_viewType & ItemDifference.DiffType.dtModified) != 0)) ||
                        (g_eFilter == FilterType.ftShowDifference && ((_viewType & ItemDifference.DiffType.dtRemoved) != 0)) ||
                        (g_eFilter == FilterType.ftShowDifference && ((_viewType & ItemDifference.DiffType.dtSpace) != 0)));
        }
        private System.Drawing.Color CalculateBackColor(ItemDifference.DiffType _viewType)
        {
            System.Drawing.Color backColor = System.Drawing.Color.White;
            if ((_viewType & ItemDifference.DiffType.dtAdded) != 0)
                backColor = System.Drawing.Color.LightGreen;
            else if ((_viewType & ItemDifference.DiffType.dtModified) != 0)
                backColor = System.Drawing.Color.LightYellow;
            else if ((_viewType & ItemDifference.DiffType.dtRemoved) != 0)
                backColor = System.Drawing.Color.LightPink;
            else //if ((_viewType & ItemDifference.DiffType.dtSame) != 0 ||(_viewType & ItemDifference.DiffType.dtSpace) != 0)
            {
                backColor = System.Drawing.Color.White;
            }
            //switch (_viewType)
            //{
            //    case ItemDifference.DiffType.dtAdded:
            //        backColor = System.Drawing.Color.LightGreen;
            //        break;
            //    case ItemDifference.DiffType.dtModified:
            //        backColor = System.Drawing.Color.LightYellow;
            //        break;
            //    case ItemDifference.DiffType.dtRemoved:
            //        backColor = System.Drawing.Color.LightPink;
            //        break;
            //    case ItemDifference.DiffType.dtSame:
            //    case ItemDifference.DiffType.dtSpace:
            //    default:
            //        backColor = System.Drawing.Color.White;
            //        break;
            //}
            return backColor;
        }
        private System.Drawing.FontStyle CalculateFontStyle(ItemDifference.DiffType _viewType)
        {
            FontStyle fStyle = 0;
            if ((_viewType & ItemDifference.DiffType.dtAlwaysShow) != 0)
                fStyle = FontStyle.Regular;
            return fStyle;
        }
        private ListViewItem NewListViewItem(int _lineNumber, string _text, int _tabs, string _toolTip, System.Drawing.Color _backColor, System.Drawing.FontStyle _fontStyle)
        {
            StringBuilder sb = new StringBuilder();
            sb.Append(string.Format("{0:d5}  ", _lineNumber));
            for (int idx = 0; idx < _tabs; idx++)
                sb.Append("   ");
            sb.Append(_text);
            ListViewItem lvi = new ListViewItem(sb.ToString());
            //ListViewItem lvi = new ListViewItem(string.Format("{0:d5}", _lineNumber));
            //lvi.SubItems.Add(sb.ToString());
            lvi.BackColor = _backColor;
            lvi.Font = new Font(lvi.Font, _fontStyle);
            lvi.ToolTipText = _toolTip;
            return lvi;
        }

        private class ItemDifference
        {
            public ItemDifference()
            {
                Initialize(0, "", 0, "", DiffType.dtSame);
            }
            public ItemDifference(int _lineNumber, string _line, int _ident, string _toolTip, DiffType _viewType)
            {
                Initialize(_lineNumber, _line, _ident, _toolTip, _viewType);
            }
            private void Initialize(int _lineNumber, string _line, int _ident, string _toolTip, DiffType _viewType)
            {
                this.lineNumber = _lineNumber;
                this.ident = _ident;
                this.line = _line;
                this.toolTip = _toolTip;
                this.viewType = _viewType;
            }
            [FlagsAttribute]
            public enum DiffType : int
            { 
                dtAdded = 1,
                dtRemoved = 2,
                dtModified = 4,
                dtSame = 8,
                dtSpace = 16,
                dtAlwaysShow = 32,
            };
            public int ident;
            public int lineNumber;
            public string toolTip;
            public string line;
            public DiffType viewType;
        }
        private void leftListView_Scroll(object sender, ListViewScrollEventArgs e)
        {
            this.leftListView.Scroll -= new Shared.VisualComponents.ListView_Scroll.ScrollHandler(this.leftListView_Scroll);
            this.rightListView.Scroll -= new Shared.VisualComponents.ListView_Scroll.ScrollHandler(this.rightListView_Scroll);
            rightListView.BeginUpdate();
            //rightListView.ScrollTo(e);
            //rightListView.EnsureVisible(leftListView.TopItem.Index);
            //rightListView.TopItem = rightListView.Items[leftListView.TopItem.Index];
            //if (rightListView.TopItem.Index != leftListView.TopItem.Index)
            //    leftListView.TopItem = leftListView.Items[rightListView.TopItem.Index];
            rightListView.ScrollTo(e);
System.Diagnostics.Trace.WriteLine("leftListView_Scroll1() - Left: " + leftListView.TopItem.Index.ToString() + ", Right: " + rightListView.TopItem.Index.ToString());
            if (rightListView.TopItem.Index != leftListView.TopItem.Index)
                rightListView.TopItem = rightListView.Items[leftListView.TopItem.Index];    

            rightListView.EndUpdate();
            this.leftListView.Scroll += new Shared.VisualComponents.ListView_Scroll.ScrollHandler(this.leftListView_Scroll);
            this.rightListView.Scroll += new Shared.VisualComponents.ListView_Scroll.ScrollHandler(this.rightListView_Scroll);
System.Diagnostics.Trace.WriteLine("leftListView_Scroll2() - Left: " + leftListView.TopItem.Index.ToString() + ", Right: " + rightListView.TopItem.Index.ToString());
        }
        private void rightListView_Scroll(object sender, ListViewScrollEventArgs e)
        {
            this.leftListView.Scroll -= new Shared.VisualComponents.ListView_Scroll.ScrollHandler(this.leftListView_Scroll);
            this.rightListView.Scroll -= new Shared.VisualComponents.ListView_Scroll.ScrollHandler(this.rightListView_Scroll);
            leftListView.BeginUpdate();
            //leftListView.ScrollTo(e);
            //leftListView.EnsureVisible(rightListView.TopItem.Index);

            //leftListView.TopItem = leftListView.Items[rightListView.TopItem.Index];
            //if (rightListView.TopItem.Index != leftListView.TopItem.Index)
            //    rightListView.TopItem = rightListView.Items[leftListView.TopItem.Index];

            leftListView.ScrollTo(e);
System.Diagnostics.Trace.WriteLine("rightListView_Scroll1() - Left: " + leftListView.TopItem.Index.ToString() + ", Right: " + rightListView.TopItem.Index.ToString());
            if (rightListView.TopItem.Index != leftListView.TopItem.Index)
                leftListView.TopItem = leftListView.Items[rightListView.TopItem.Index];
            leftListView.EndUpdate();
            this.leftListView.Scroll += new Shared.VisualComponents.ListView_Scroll.ScrollHandler(this.leftListView_Scroll);
            this.rightListView.Scroll += new Shared.VisualComponents.ListView_Scroll.ScrollHandler(this.rightListView_Scroll);
System.Diagnostics.Trace.WriteLine("rightListView_Scroll2() - Left: " + leftListView.TopItem.Index.ToString() + ", Right: " + rightListView.TopItem.Index.ToString());
        }

        private bool bChangingSelection = false;
        private void leftListView_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (bChangingSelection == false)
            {
                bChangingSelection = true;
                rightListView.SelectedItems.Clear();
                foreach (ListViewItem selLvi in leftListView.SelectedItems)
                {
                    rightListView.Items[selLvi.Index].Selected = true;
                }
                bChangingSelection = false;
            }
        }

        private void rightListView_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (bChangingSelection == false)
            {
                bChangingSelection = true;

                leftListView.SelectedItems.Clear();
                foreach (ListViewItem selLvi in rightListView.SelectedItems)
                {
                    leftListView.Items[selLvi.Index].Selected = true;
                }

                bChangingSelection = false;
            }
        }

        private void filterComboBoxTS_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (filterComboBoxTS.SelectedIndex == 0)
                g_eFilter = FilterType.ftShowAll;
            else
                g_eFilter = FilterType.ftShowDifference;
            if (g_softwareSpec != null && g_packageAttribs1 != null && g_packageAttribs2 != null)
                SetData(g_softwareSpec, g_packageAttribs1, g_packageAttribs2, g_eFilter);
        }
    }
}
