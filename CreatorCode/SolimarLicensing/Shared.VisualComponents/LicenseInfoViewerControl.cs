using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace Shared.VisualComponents
{
    public partial class LicenseInfoViewerControl : System.Windows.Forms.TreeView
    {
        public LicenseInfoViewerControl()
        {
            InitializeComponent();
            this.KeyDown += new KeyEventHandler(LicenseInfoViewerControl_KeyDown);
        }

        void LicenseInfoViewerControl_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.C && e.Control)
            {
                if(this.SelectedNode != null)
                    Clipboard.SetData(DataFormats.UnicodeText, this.SelectedNode.Text);
            }
            //throw new NotImplementedException();
        }

        private Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_SoftwareSpecAttribs g_softwareSpec;
        public void SetData(Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_SoftwareSpecAttribs param_softwareSpec, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs param_licenseInfoAttribs, bool param_bClearAll)
        {
            //this
            //param_licenseInfoAttribs

            g_softwareSpec = param_softwareSpec;
            try
            {
                this.BeginUpdate();
                if(param_bClearAll)
                    this.Nodes.Clear();

                TreeNode rootNode = new TreeNode(Solimar.Licensing.Attribs.Lic_LicenseInfoAttribsHelper.GetDisplayLabel(param_licenseInfoAttribs));
                Tree_Add_Lic_LicenseInfoAttribs(ref rootNode, param_licenseInfoAttribs);
                this.Nodes.Add(rootNode);
                rootNode.Expand();
            }
            catch (Exception ex)
            {
            }
            finally
            {
                this.EndUpdate();
            }
        }

        private void Tree_Add_Lic_LicenseInfoAttribs(ref TreeNode param_refRootNode, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs param_licInfoAttribs)
        {
            if (param_licInfoAttribs != null)
            {
                //param_refRootNode.Nodes.Add(new TreeNode("CustomerID: " + param_licInfoAttribs.customerID));
                param_refRootNode.Nodes.Add(new TreeNode(String.Format("CustomerID: {0:X4}", param_licInfoAttribs.customerID.TVal)));
                param_refRootNode.LastNode.ForeColor = param_refRootNode.ForeColor;
                param_refRootNode.Nodes.Add(new TreeNode(String.Format("DestinationID: {0:X3}", param_licInfoAttribs.destinationID.TVal)));
                param_refRootNode.LastNode.ForeColor = param_refRootNode.ForeColor;
                //param_refRootNode.Nodes.Add(new TreeNode("SoftwareID: " + param_licInfoAttribs.softwareGroupLicenseID));
                param_refRootNode.Nodes.Add(new TreeNode(String.Format("SoftwareID: {0:X4}", param_licInfoAttribs.softwareGroupLicenseID.TVal)));
                param_refRootNode.LastNode.ForeColor = param_refRootNode.ForeColor;
                param_refRootNode.Nodes.Add(new TreeNode("SoftwareLicenseType: " + param_licInfoAttribs.softwareLicType.GetAlias() + " (" + param_licInfoAttribs.softwareLicType.ToString() + ")"));
                param_refRootNode.LastNode.ForeColor = param_refRootNode.ForeColor;
                //if ((param_licInfoAttribs.softwareLicType == Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltDisasterRecovery)
                //    || (param_licInfoAttribs.softwareLicType == Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltTestDev))
                if (param_licInfoAttribs.activationTotal.TVal != 0)
                {
                    param_refRootNode.Nodes.Add(new TreeNode("Activations Total: " + param_licInfoAttribs.activationTotal.TVal));
                    param_refRootNode.LastNode.ForeColor = param_refRootNode.ForeColor;
                    param_refRootNode.Nodes.Add(new TreeNode("Activations Current: " + param_licInfoAttribs.activationCurrent.TVal));
                    param_refRootNode.LastNode.ForeColor = param_refRootNode.ForeColor;
                    param_refRootNode.Nodes.Add(new TreeNode("Activation Expiration Date: " + param_licInfoAttribs.activationExpirationDate.TVal.ToLocalTime()));
                    param_refRootNode.LastNode.ForeColor = param_refRootNode.ForeColor;
                    param_refRootNode.Nodes.Add(new TreeNode("Activation Amount in Days: " + param_licInfoAttribs.activationAmountInDays.TVal));
                    param_refRootNode.LastNode.ForeColor = param_refRootNode.ForeColor;
                }
                param_refRootNode.Nodes.Add(new TreeNode("Modified Date: " + param_licInfoAttribs.modifiedDate.TVal.ToLocalTime()));
                param_refRootNode.LastNode.ForeColor = param_refRootNode.ForeColor;

                TreeNode prodNode = new TreeNode("ProductList");
                foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs prodInfo in param_licInfoAttribs.productList.TVal)
                {
                    Tree_Add_Lic_ProductInfoAttribs(ref prodNode, prodInfo);
                }

                param_refRootNode.Nodes.Add(prodNode);

                prodNode.Expand();
                Tree_Add_Lic_VerificationAttribs(ref param_refRootNode, param_licInfoAttribs.licVerificationAttribs, param_licInfoAttribs);
            }
        }

        private void Tree_Add_Lic_ProductInfoAttribs(ref TreeNode param_refRootNode, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs param_prodInfoAttribs)
        {
            StringBuilder toolTipBuilder = new StringBuilder();
            string nodeText;


            nodeText = "Product: " + Solimar.Licensing.Attribs.Lic_LicenseInfoAttribsHelper.GetProductName(g_softwareSpec, param_prodInfoAttribs.productID.TVal) + " (" + System.Convert.ToInt32(param_prodInfoAttribs.productID.ToString(), 16).ToString() + ")";
            toolTipBuilder.Append("ProductName: " + Solimar.Licensing.Attribs.Lic_LicenseInfoAttribsHelper.GetProductName(g_softwareSpec, param_prodInfoAttribs.productID.TVal) + "\r\n");

            //nodeText = "Product " + System.Convert.ToInt32(param_prodInfoAttribs.productID.ToString(), 16).ToString();
            TreeNode prodNode = new TreeNode(nodeText);

            nodeText = "ProductID: " + System.Convert.ToInt32(param_prodInfoAttribs.productID.ToString(), 16).ToString();
            prodNode.Nodes.Add(new TreeNode(nodeText));
            toolTipBuilder.Append(nodeText + "\r\n");

            nodeText = "ProductAppInstance: " + System.Convert.ToInt32(param_prodInfoAttribs.productAppInstance.ToString(), 16).ToString();
            prodNode.Nodes.Add(new TreeNode(nodeText));
            toolTipBuilder.Append(nodeText + "\r\n");

            System.Text.StringBuilder strBuilderIntro = new StringBuilder();
            strBuilderIntro.Append("ProductVersion: ");
            strBuilderIntro.Append(param_prodInfoAttribs.product_Major.ToString());
            strBuilderIntro.Append(".");
            strBuilderIntro.Append(String.Format("{0:00}", System.Convert.ToInt32(param_prodInfoAttribs.product_Minor.ToString(), 16)));
            //strBuilderIntro.Append(".");
            //strBuilderIntro.Append(param_prodInfoAttribs.product_SubMajor.ToString());
            //strBuilderIntro.Append(".");
            //strBuilderIntro.Append(param_prodInfoAttribs.product_SubMinor.ToString());
            prodNode.Nodes.Add(new TreeNode(strBuilderIntro.ToString()));
            toolTipBuilder.Append(strBuilderIntro.ToString());

            TreeNode modNode = new TreeNode("ModuleList");
            foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs modInfo in param_prodInfoAttribs.moduleList.TVal)
                Tree_Add_Lic_ModuleInfoAttribs(ref modNode, modInfo, param_prodInfoAttribs.productID);
            prodNode.Nodes.Add(modNode);
            prodNode.ToolTipText = toolTipBuilder.ToString();
            //param_prodInfoAttribs
            //prodNode.Expand();
            param_refRootNode.Nodes.Add(prodNode);
        }

        private void Tree_Add_Lic_ModuleInfoAttribs(ref TreeNode param_refRootNode, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs param_modInfoAttribs, uint param_prodID)
        {

            StringBuilder toolTipBuilder = new StringBuilder();
            string nodeText;

            nodeText = "Module: " + System.Convert.ToInt32(param_modInfoAttribs.moduleID.ToString(), 16).ToString() + " {" + Solimar.Licensing.Attribs.Lic_LicenseInfoAttribsHelper.GetModuleName(g_softwareSpec, param_prodID, param_modInfoAttribs.moduleID.TVal) + "}";
            toolTipBuilder.Append(nodeText);
            
            TreeNode modNode = new TreeNode();
            TreeNode childNode = null;
            if (DateTime.Compare(new DateTime(1900, 1, 1), param_modInfoAttribs.moduleExpirationDate.TVal) != 0)
            {
                nodeText += " [Addon Module Expires: " + param_modInfoAttribs.moduleExpirationDate.TVal.ToLocalTime().ToString() + "]";
                if (DateTime.Now.ToUniversalTime().CompareTo(param_modInfoAttribs.moduleExpirationDate.TVal) > 0)
                {
                    modNode.ForeColor = System.Drawing.Color.Red;
                }
            }

            modNode.Text = nodeText;
            modNode.Tag = new System.Collections.Generic.KeyValuePair<uint, DateTime>(param_modInfoAttribs.moduleID.TVal, param_modInfoAttribs.moduleExpirationDate.TVal);

            toolTipBuilder.Append("\r\nModuleName: " + Solimar.Licensing.Attribs.Lic_LicenseInfoAttribsHelper.GetModuleName(g_softwareSpec, param_prodID, param_modInfoAttribs.moduleID.TVal));

            nodeText = "ModuleID: " + System.Convert.ToInt32(param_modInfoAttribs.moduleID.ToString(), 16).ToString();
            toolTipBuilder.Append("\r\n" + nodeText);
            childNode = new TreeNode(nodeText);
            childNode.ForeColor = modNode.ForeColor;
            modNode.Nodes.Add(childNode);

            nodeText = "ModuleValue: " + System.Convert.ToInt32(param_modInfoAttribs.moduleValue.ToString(), 16).ToString();
            toolTipBuilder.Append("\r\n" + nodeText);
            childNode = new TreeNode(nodeText);
            childNode.ForeColor = modNode.ForeColor;
            modNode.Nodes.Add(childNode);

            nodeText = "ModuleAppInstance: " + System.Convert.ToInt32(param_modInfoAttribs.moduleAppInstance.ToString(), 16).ToString();
            toolTipBuilder.Append("\r\n" + nodeText);
            childNode = new TreeNode(nodeText);
            childNode.ForeColor = modNode.ForeColor;
            modNode.Nodes.Add(childNode);

            if (DateTime.Compare(new DateTime(1900, 1, 1), param_modInfoAttribs.moduleExpirationDate.TVal) != 0)
            {
                nodeText = "ModuleExpirationDate: " + param_modInfoAttribs.moduleExpirationDate.TVal.ToLocalTime().ToString();
                toolTipBuilder.Append("\r\n" + nodeText);
                childNode = new TreeNode(nodeText);
                childNode.ForeColor = modNode.ForeColor;
                modNode.Nodes.Add(childNode);
            }
            modNode.ToolTipText = toolTipBuilder.ToString();
            AlphaAdd_ModuleInfoTreeNode(ref param_refRootNode, modNode);

        }
        private void AlphaAdd_ModuleInfoTreeNode(ref TreeNode param_refRootNode, TreeNode param_childNode)
        {
            int insertLocationIdx = param_refRootNode.Nodes.Count;  //Defaultly add to the end
            if (param_childNode.Tag is System.Collections.Generic.KeyValuePair<uint, DateTime>)
            {
                System.Collections.Generic.KeyValuePair<uint, DateTime> childKvPair = (System.Collections.Generic.KeyValuePair<uint, DateTime>)param_childNode.Tag;
                for (int idx = 0; idx < param_refRootNode.Nodes.Count; idx++)
                {
                    if (param_refRootNode.Nodes[idx].Tag is System.Collections.Generic.KeyValuePair<uint, DateTime>)
                    {
                        System.Collections.Generic.KeyValuePair<uint, DateTime> tmpKvPair = (System.Collections.Generic.KeyValuePair<uint, DateTime>)param_refRootNode.Nodes[idx].Tag;
                        if (childKvPair.Key < tmpKvPair.Key)    // passed the module
                        {
                            insertLocationIdx = idx;
                            break;
                        }
                        else if (childKvPair.Key == tmpKvPair.Key)  // same module, see if expiration date is after
                        {
                            if (childKvPair.Value.CompareTo(tmpKvPair.Value) < 1) // found expiration date to place after
                            {
                                insertLocationIdx = idx;
                                break;
                            }
                        }
                    }
                }
            }
            param_refRootNode.Nodes.Insert(insertLocationIdx, param_childNode);
        }

        private void Tree_Add_Lic_VerificationAttribs(ref TreeNode param_refRootNode, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_VerificationAttribs param_verificationAttribs, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs param_licInfoAttribs)
        {
            TreeNode verificationNode = new TreeNode("VerificationNode");
            TreeNode verListNode = new TreeNode("ValidationList");
            foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs verToken in param_verificationAttribs.validationTokenList.TVal)
            {
                Tree_Add_Lic_ValidationTokenAttribs(ref verListNode, verToken, param_licInfoAttribs);
            }

            verListNode.Expand();
            verificationNode.Nodes.Add(verListNode);

            TreeNode verHistListNode = new TreeNode("VerificationHistoryList");
            foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_VerificationCodeAttribs verCodeToken in param_verificationAttribs.verificationCodeHistoryList.TVal)
            {
                Tree_Add_Lic_VerificationCodeAttribs(ref verHistListNode, verCodeToken);
            }
            verHistListNode.Expand();
            verificationNode.Nodes.Add(verHistListNode);

            verificationNode.Expand();
            param_refRootNode.Nodes.Add(verificationNode);
        }
        private void Tree_Add_Lic_ValidationTokenAttribs(ref TreeNode param_refRootNode, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs param_verificationTokenAttribs, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs param_licInfoAttribs)
        {
            StringBuilder toolTipBuilder = new StringBuilder();
            string nodeText;
            bool bSuccess = false;
            TreeNode verificationTokenNode = new TreeNode(ValidateToken(param_verificationTokenAttribs, param_licInfoAttribs, ref bSuccess));
            TreeNode childNode = null;

            bSuccess = true;
            if (!bSuccess)
                verificationTokenNode.ForeColor = System.Drawing.Color.Red;

            nodeText = "TokenType: " + param_verificationTokenAttribs.tokenType.EVal + " (" + param_verificationTokenAttribs.tokenType + ")";
            toolTipBuilder.Append(nodeText + "\r\n");
            childNode = new TreeNode(nodeText);
            childNode.ForeColor = verificationTokenNode.ForeColor;
            verificationTokenNode.Nodes.Add(childNode);

            nodeText = "TokenValue: " + param_verificationTokenAttribs.tokenValue;
            toolTipBuilder.Append(nodeText);
            childNode = new TreeNode(nodeText);
            childNode.ForeColor = verificationTokenNode.ForeColor;
            verificationTokenNode.Nodes.Add(childNode);

            param_refRootNode.Nodes.Add(verificationTokenNode);
            verificationTokenNode.ToolTipText = toolTipBuilder.ToString();

            //param_refRootNode = null;
            return;
        }
        private void Tree_Add_Lic_VerificationCodeAttribs(ref TreeNode param_refRootNode, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_VerificationCodeAttribs param_verificationCodeAttribs)
        {
            StringBuilder toolTipBuilder = new StringBuilder();
            toolTipBuilder.Append("Date: ");
            toolTipBuilder.Append(param_verificationCodeAttribs.verificationDate.TVal.ToLocalTime());
            toolTipBuilder.Append(", VerificationCode: ");
            toolTipBuilder.Append(param_verificationCodeAttribs.verificationValue.TVal);

            TreeNode verificationCodeTokenNode = new TreeNode(toolTipBuilder.ToString());
            param_refRootNode.Nodes.Add(verificationCodeTokenNode);
            verificationCodeTokenNode.ToolTipText = toolTipBuilder.ToString();

            //TreeNode verificationCodeNode = new TreeNode(ValidateToken(param_verificationTokenAttribs, ref bSuccess));
            //TreeNode childNode = null;

        }

        private string ValidateToken(Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs param_verificationTokenAttribs, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs param_licInfoAttribs, ref bool bRefSuccess)
        {
            System.Text.StringBuilder retVal = new StringBuilder("ValidationToken: ");
            //string softwareLicense = String.Format("{0}-{1}", System.Convert.ToInt32(g_licInfoAttrib.customerID, 16), g_licInfoAttrib.softwareLicenseID);
            //string softwareLicense = String.Format("{0}-{1}", g_licInfoAttrib.customerID, g_licInfoAttrib.softwareGroupLicenseID);
            string softwareLicense = Solimar.Licensing.Attribs.Lic_LicenseInfoAttribsHelper.GetDisplayLabel(param_licInfoAttribs);

            retVal.Append(param_verificationTokenAttribs.tokenType.EVal.ToString());
            //try
            //{
            //    g_licServer.ValidateToken_ByLicense(softwareLicense, (int)param_verificationTokenAttribs.tokenType.TVal, param_verificationTokenAttribs.tokenValue);
            //    retVal.Append(" - Validated");
            //    bRefSuccess = true;
            //}
            //catch (COMException comEX)
            //{
            //    retVal.Append(" - ");
            //    retVal.Append(comEX.Message);
            //    bRefSuccess = false;
            //}
            return retVal.ToString();
        }
    }
}
