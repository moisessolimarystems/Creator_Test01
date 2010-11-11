using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace Shared.VisualComponents
{
    public partial class SoftwareSpecViewerControl : System.Windows.Forms.TreeView
    {
        public SoftwareSpecViewerControl()
        {
            InitializeComponent();
            this.KeyDown += new KeyEventHandler(General_KeyDown);
            this.MouseClick += new MouseEventHandler(General_MouseClick);

            this.ContextMenuStrip = new System.Windows.Forms.ContextMenuStrip();
            this.ContextMenuStrip.Items.Add("Copy");
            this.ContextMenuStrip.Click += new EventHandler(ContextMenuStrip_Click);
        }
        protected override void InitLayout()
        {
            Shared.VisualComponents.ControlHelper.SetWindowTheme(this.Handle, "Explorer", null);
            //Treeviews in Windows Explorer also have the fade effects. This can be achieved via the TVS_EX_FADEINOUTEXPANDOS [0x0040] extended style.
            Shared.VisualComponents.ControlHelper.SendMessage(this.Handle, 0x1100 + 44, (IntPtr)0x0040, (IntPtr)0x0040);
            //The treeviews also have the "auto-scroll" feature. You can enable this via the TVS_EX_AUTOHSCROLL [0x0020] extended style.
            //Shared.VisualComponents.ControlHelper.SendMessage(this.treeView.Handle, 0x1100 + 44, (IntPtr)0x0020, (IntPtr)0x0020);
            base.InitLayout();
        }

        private void ContextMenuStrip_Click(object sender, EventArgs e)
        {
            if (sender != null)
            {
                if (this.SelectedNode != null)
                    Clipboard.SetData(DataFormats.UnicodeText, this.SelectedNode.Text);
            }
        }

        private void General_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                TreeNode tNode = this.GetNodeAt(e.Location);
                if (tNode != null)
                    tNode.TreeView.SelectedNode = tNode;
            }
        }

        private void General_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.C && e.Control)
            {
                if (this.SelectedNode != null)
                    Clipboard.SetData(DataFormats.UnicodeText, this.SelectedNode.Text);
            }
        }

        public void SetData(Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_SoftwareSpecAttribs param_SoftwareSpec)
        {
            this.BeginUpdate();
            try
            {
                this.Nodes.Clear();

                TreeNode rootNode = new TreeNode("Software Spec");
                Tree_Add_Lic_SoftwareSpecAttribs(ref rootNode, param_SoftwareSpec);
                this.Nodes.Add(rootNode);
                rootNode.Expand();
            }
            finally
            {
                this.EndUpdate();
            }
        }

        void Tree_Add_Lic_SoftwareSpecAttribs(ref TreeNode param_refRootNode, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_SoftwareSpecAttribs param_SoftwareSpec)
        {
            StringBuilder toolTipBuilder = new StringBuilder();
            TreeNode tmpNode = new TreeNode();
            System.Text.StringBuilder strBuilderIntro = new StringBuilder();
            toolTipBuilder.Append("Version: ");
            toolTipBuilder.Append(System.Convert.ToInt32(param_SoftwareSpec.softwareSpec_Major.ToString(), 16).ToString());
            toolTipBuilder.Append(".");
            toolTipBuilder.Append(System.Convert.ToInt32(param_SoftwareSpec.softwareSpec_Minor.ToString(), 16).ToString());
            toolTipBuilder.Append(".");
            toolTipBuilder.Append(System.Convert.ToInt32(param_SoftwareSpec.softwareSpec_SubMajor.ToString(), 16).ToString());
            toolTipBuilder.Append(".");
            toolTipBuilder.Append(System.Convert.ToInt32(param_SoftwareSpec.softwareSpec_SubMinor.ToString(), 16).ToString());
            tmpNode.Text = toolTipBuilder.ToString();
            param_refRootNode.Nodes.Add(tmpNode);

            TreeNode productMapNode = new TreeNode("ProductMap");
            foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in param_SoftwareSpec.productSpecMap.TVal.Values)
                Tree_Add_Lic_ProductSoftwareSpecAttribs(ref productMapNode, productSpec, param_SoftwareSpec);

            productMapNode.Expand();
            param_refRootNode.Nodes.Add(productMapNode);
            param_refRootNode.ToolTipText = toolTipBuilder.ToString();

        }
        void Tree_Add_Lic_ProductSoftwareSpecAttribs(ref TreeNode param_refRootNode, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs param_prodSoftwareSpec, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_SoftwareSpecAttribs param_SoftwareSpec)
        {
            StringBuilder toolTipBuilder = new StringBuilder();
            string nodeText;

            nodeText = "ProductNode: " + param_prodSoftwareSpec.productName + " (" + System.Convert.ToInt32(param_prodSoftwareSpec.productID.ToString(), 16).ToString() + ")";
            TreeNode productNode = new TreeNode(nodeText);
            nodeText = "ProductName: " + param_prodSoftwareSpec.productName;
            productNode.Nodes.Add(new TreeNode(nodeText));
            toolTipBuilder.Append(nodeText + "\r\n");

            nodeText = "ProductID: " + System.Convert.ToInt32(param_prodSoftwareSpec.productID.ToString(), 16).ToString();
            productNode.Nodes.Add(new TreeNode(nodeText));
            toolTipBuilder.Append(nodeText);

            nodeText = "SameModSpecProductID: " + System.Convert.ToInt32(param_prodSoftwareSpec.sameModSpecProductID.ToString(), 16).ToString();
            productNode.Nodes.Add(new TreeNode(nodeText));
            if (param_prodSoftwareSpec.sameModSpecProductID != 0)
                toolTipBuilder.Append("\r\n" + nodeText);

            nodeText = "PrevSharedProductID: " + System.Convert.ToInt32(param_prodSoftwareSpec.prevSharedProductID.ToString(), 16).ToString();
            productNode.Nodes.Add(new TreeNode(nodeText));
            if (param_prodSoftwareSpec.prevSharedProductID != 0)
                toolTipBuilder.Append("\r\n" + nodeText);

            TreeNode modNode = new TreeNode("ModuleMap");

            Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.Lic_ModuleSoftwareSpecAttribsMap moduleSpecMap = Solimar.Licensing.Attribs.Lic_LicenseInfoAttribsHelper.GetModuleList(param_SoftwareSpec, param_prodSoftwareSpec.productID);
            if (moduleSpecMap != null)
            {
                foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs moduleSpec in moduleSpecMap.TVal.Values)
                    Tree_Add_Lic_ModuleSoftwareSpecAttribs(ref modNode, moduleSpec, param_SoftwareSpec);
            }

            productNode.Nodes.Add(modNode);
            productNode.ToolTipText = toolTipBuilder.ToString();

            param_refRootNode.Nodes.Add(productNode);


        }
        void Tree_Add_Lic_ModuleSoftwareSpecAttribs(ref TreeNode param_refRootNode, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs param_modInfoAttribs, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_SoftwareSpecAttribs param_SoftwareSpec)
        {
            StringBuilder toolTipBuilder = new StringBuilder();
            string nodeText;

            nodeText = "Module: " + System.Convert.ToInt32(param_modInfoAttribs.moduleID.ToString(), 16).ToString() + " {" + param_modInfoAttribs.moduleName + "}";
            //toolTipBuilder.Append(nodeText);

            TreeNode moduleNode = new TreeNode(nodeText);
            nodeText = "ModuleName: " + param_modInfoAttribs.moduleName;
            toolTipBuilder.Append(nodeText);
            moduleNode.Nodes.Add(new TreeNode(nodeText));

            nodeText = "ModuleID: " + System.Convert.ToInt32(param_modInfoAttribs.moduleID.ToString(), 16).ToString();
            toolTipBuilder.Append("\r\n" + nodeText);
            moduleNode.Nodes.Add(new TreeNode(nodeText));

            nodeText = "ModuleDefault: " + System.Convert.ToInt32(param_modInfoAttribs.moduleDefaultLicense.ToString(), 16).ToString();
            toolTipBuilder.Append("\r\n" + nodeText);
            moduleNode.Nodes.Add(new TreeNode(nodeText));

            nodeText = "TrialLicense: " + System.Convert.ToInt32(param_modInfoAttribs.moduleTrialLicense.ToString(), 16).ToString();
            toolTipBuilder.Append("\r\n" + nodeText);
            moduleNode.Nodes.Add(new TreeNode(nodeText));

            if (string.Compare(param_modInfoAttribs.modUnlimitedValue, "0") != 0)
            {
                nodeText = "ModuleUnlimitedValue: " + System.Convert.ToInt32(param_modInfoAttribs.modUnlimitedValue.ToString(), 16).ToString();
                toolTipBuilder.Append("\r\n" + nodeText);
                moduleNode.Nodes.Add(new TreeNode(nodeText));
            }

            if (string.Compare(param_modInfoAttribs.modulePoolModuleID, "0") != 0)
            {
                nodeText = "ModulePoolID: " + System.Convert.ToInt32(param_modInfoAttribs.modulePoolModuleID.ToString(), 16).ToString();
                toolTipBuilder.Append("\r\n" + nodeText);
                moduleNode.Nodes.Add(new TreeNode(nodeText));
            }
            System.Text.StringBuilder strBuilderIntro = new StringBuilder();
            strBuilderIntro.Append("VersionIntroduced: ");
            strBuilderIntro.Append(System.Convert.ToInt32(param_modInfoAttribs.moduleVersionIntroduced_Major.ToString(), 16).ToString());
            strBuilderIntro.Append(".");
            strBuilderIntro.Append(System.Convert.ToInt32(param_modInfoAttribs.moduleVersionIntroduced_Minor.ToString(), 16).ToString());
            strBuilderIntro.Append(".");
            strBuilderIntro.Append(System.Convert.ToInt32(param_modInfoAttribs.moduleVersionIntroduced_SubMajor.ToString(), 16).ToString());
            strBuilderIntro.Append(".");
            strBuilderIntro.Append(System.Convert.ToInt32(param_modInfoAttribs.moduleVersionIntroduced_SubMinor.ToString(), 16).ToString());
            toolTipBuilder.Append("\r\n" + strBuilderIntro.ToString());
            moduleNode.Nodes.Add(new TreeNode(strBuilderIntro.ToString()));

            System.Text.StringBuilder strBuilderDepre = new StringBuilder();
            strBuilderDepre.Append("VersionDeprecated: ");
            strBuilderDepre.Append(System.Convert.ToInt32(param_modInfoAttribs.moduleVersionDeprecated_Major.ToString(), 16).ToString());
            strBuilderDepre.Append(".");
            strBuilderDepre.Append(System.Convert.ToInt32(param_modInfoAttribs.moduleVersionDeprecated_Minor.ToString(), 16).ToString());
            strBuilderDepre.Append(".");
            strBuilderDepre.Append(System.Convert.ToInt32(param_modInfoAttribs.moduleVersionDeprecated_SubMajor.ToString(), 16).ToString());
            strBuilderDepre.Append(".");
            strBuilderDepre.Append(System.Convert.ToInt32(param_modInfoAttribs.moduleVersionDeprecated_SubMinor.ToString(), 16).ToString());
            moduleNode.Nodes.Add(new TreeNode(strBuilderDepre.ToString()));

            moduleNode.ToolTipText = toolTipBuilder.ToString();
            param_refRootNode.Nodes.Add(moduleNode);
        }

    }
}
