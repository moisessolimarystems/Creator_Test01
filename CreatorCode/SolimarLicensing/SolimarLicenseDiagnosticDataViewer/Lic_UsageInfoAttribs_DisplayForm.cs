using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Solimar.Licensing.Attribs;

namespace SolimarLicenseDiagnosticDataViewer
{
    public partial class Lic_UsageInfoAttribs_DisplayForm : Form
    {
        public Lic_UsageInfoAttribs_DisplayForm()
        {
            InitializeComponent();
        }
        private Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_SoftwareSpecAttribs g_softwareSpec = null;
        public void SetData(Lic_UsageInfoAttribs _data, DateTime _createdDate)
        {
            if (g_softwareSpec == null)
            {
                Solimar.Licensing.GlobalSoftwareSpec globalSoftwareSpec = new Solimar.Licensing.GlobalSoftwareSpec();
                g_softwareSpec = globalSoftwareSpec.softwareSpec;
            }
            try
            {
                usageTreeView.BeginUpdate();
                usageTreeView.Nodes.Clear();

                TreeNode rootNode = new TreeNode(string.Format("Usage at: {0}", _createdDate.ToLocalTime().ToString()));
                foreach(Solimar.Licensing.Attribs.Lic_UsageInfoAttribs.Lic_UsProductInfoAttribs usageInfo in _data.productList.TVal)
                    Tree_Add_Lic_UsProductInfoAttribs(ref rootNode, usageInfo);
                usageTreeView.Nodes.Add(rootNode);

                StringBuilder sBuilder = new StringBuilder();
                foreach (TreeNode childNode in rootNode.Nodes)
                {
                    if (sBuilder.Length != 0)
                        sBuilder.Append("\r\n");
                    sBuilder.Append(childNode.Text);
                }
                rootNode.ToolTipText = sBuilder.ToString();

                rootNode.Expand();
            }
            catch (Exception)
            {
            }
            finally
            {
                usageTreeView.EndUpdate();
            }
        }
        

        private void Tree_Add_Lic_UsProductInfoAttribs(ref TreeNode param_refRootNode, Solimar.Licensing.Attribs.Lic_UsageInfoAttribs.Lic_UsProductInfoAttribs param_dataAttribs)
        {
            string nodeText;
            nodeText = string.Format("Product: {0} (0x{1:x})", Solimar.Licensing.Attribs.Lic_LicenseInfoAttribsHelper.GetProductName(g_softwareSpec, param_dataAttribs.productID.TVal), param_dataAttribs.productID.TVal);
            TreeNode tNode = new TreeNode(nodeText);
            foreach (Solimar.Licensing.Attribs.Lic_UsageInfoAttribs.Lic_UsAppInstanceInfoAttribs usageInfo in param_dataAttribs.appInstanceList.TVal)
                Tree_Add_Lic_UsAppInstanceInfoAttribs(ref tNode, usageInfo, param_dataAttribs.productID.TVal);
            InsertIntoTree_Alpha(param_refRootNode, tNode);
            StringBuilder sBuilder = new StringBuilder();
            foreach (TreeNode childNode in tNode.Nodes)
            {
                if (sBuilder.Length != 0)
                    sBuilder.Append("\r\n");
                sBuilder.Append(childNode.Text);
            }
            tNode.ToolTipText = sBuilder.ToString();
        }

        private void Tree_Add_Lic_UsAppInstanceInfoAttribs(ref TreeNode param_refRootNode, Solimar.Licensing.Attribs.Lic_UsageInfoAttribs.Lic_UsAppInstanceInfoAttribs param_dataAttribs, uint param_productID)
        {
            string nodeText = string.Format("Application: {0}", param_dataAttribs.applicationInstance.TVal);
            TreeNode tNode = new TreeNode(nodeText);
            foreach (Solimar.Licensing.Attribs.Lic_UsageInfoAttribs.Lic_UsModuleInfoAttribs usageInfo in param_dataAttribs.moduleList.TVal)
                Tree_Add_Lic_UsModuleInfoAttribs(ref tNode, usageInfo, param_productID);
            StringBuilder sBuilder = new StringBuilder();
            foreach (TreeNode childNode in tNode.Nodes)
            {
                if (sBuilder.Length != 0)
                    sBuilder.Append("\r\n");
                sBuilder.Append(childNode.Text);
            }
            tNode.ToolTipText = sBuilder.ToString();
            InsertIntoTree_Alpha(param_refRootNode, tNode);
        }

        private void Tree_Add_Lic_UsModuleInfoAttribs(ref TreeNode param_refRootNode, Solimar.Licensing.Attribs.Lic_UsageInfoAttribs.Lic_UsModuleInfoAttribs param_dataAttribs, uint param_productID)
        {
            string nodeText = string.Format("ModuleID: {0} ({1}), InUse: {2}", 
                param_dataAttribs.moduleID.TVal, 
                Solimar.Licensing.Attribs.Lic_LicenseInfoAttribsHelper.GetModuleName(g_softwareSpec, param_productID, param_dataAttribs.moduleID), 
                param_dataAttribs.moduleUsage.TVal);
            TreeNode tNode = new TreeNode(nodeText);
            InsertIntoTree_Alpha(param_refRootNode, tNode);
        }

        private const int CP_NOCLOSE_BUTTON = 0x200;
        protected override CreateParams CreateParams
        {
            get
            {
                CreateParams cp = base.CreateParams;
                cp.ClassStyle |= CP_NOCLOSE_BUTTON;	// Disable Close button
                return cp;
            }
        }

        private void usageTreeView_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.C && e.Control)
            {
                if (usageTreeView.SelectedNode != null)
                    Clipboard.SetData(DataFormats.UnicodeText, usageTreeView.SelectedNode.Text);
                    //Clipboard.SetData(DataFormats.UnicodeText, string.Format("{0}\r\n{1}", usageTreeView.SelectedNode.Text, usageTreeView.SelectedNode.ToolTipText));
            }
        }

        private void copyToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (usageTreeView.SelectedNode != null)
                Clipboard.SetData(DataFormats.UnicodeText, usageTreeView.SelectedNode.Text);
        }
        private void General_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                TreeNode tNode = usageTreeView.GetNodeAt(e.Location);
                if (tNode != null)
                    tNode.TreeView.SelectedNode = tNode;
            }
        }
        /// <summary>
        /// This method inserts the newChildTreeNode into the parentTreeNode in Alpha Order
        /// </summary>
        void InsertIntoTree_Alpha(TreeNode parentTreeNode, TreeNode newChildTreeNode)
        {
            int insertIdx = 0;
            while (insertIdx < parentTreeNode.Nodes.Count)
            {
                if (parentTreeNode.Nodes[insertIdx].Text.CompareTo(newChildTreeNode.Text) > 0)
                    break;
                insertIdx++;
            }
            parentTreeNode.Nodes.Insert(insertIdx, newChildTreeNode);
        }

    }
}
