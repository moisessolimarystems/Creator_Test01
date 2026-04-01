using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using Solimar.Licensing.Attribs;
using SolimarLicenseDiagnosticDataViewer.Properties;


namespace SolimarLicenseDiagnosticDataViewer
{
    public partial class LicenseDiagnosticDataViewerForm : Form
    {
        public LicenseDiagnosticDataViewerForm()
        {
            InitializeComponent();
            Initialize();
        }
        private void Initialize()
        {
            m_solLicServerWrapper = new Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseServerWrapper();
            m_connected = false;
            m_selectedDirectory = "";
            m_importPktDialog = new OpenFileDialog();
			m_loadedLicSysAttribs = null;
        }


        #region Tool Bar Menu Item Actions
        private void openLicenseSystemDataToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.m_importPktDialog.DefaultExt = "lsData";
            this.m_importPktDialog.Filter = "License System Data|*.lsData|License Verification Data|*.vData|License Packet|*.packet|License Archive|*.licArchive";
            this.m_importPktDialog.Title = "License System Data";
            this.m_importPktDialog.FileName = System.Environment.MachineName.ToLower();
            this.m_importPktDialog.InitialDirectory = m_selectedDirectory;
            if (this.m_importPktDialog.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    m_selectedDirectory = System.IO.Path.GetDirectoryName(this.m_importPktDialog.FileName);

                    #region if License Verification Data
                    if (string.Compare(System.IO.Path.GetExtension(this.m_importPktDialog.FileName), ".vData", true) == 0)
                    {
                        Byte[] licPktBytes = System.IO.File.ReadAllBytes(this.m_importPktDialog.FileName);
                        string licStream = "";
                        licStream = m_solLicServerWrapper.GenerateLicInfo_ByVerifyData(licPktBytes);
                        textBox1.Text = licStream.ToString();
                        SetPackage(this.m_importPktDialog.FileName, licStream);
                    }
                    #endregion
                    #region if License Verification Data
                    else if (string.Compare(System.IO.Path.GetExtension(this.m_importPktDialog.FileName), ".packet", true) == 0)
                    {
                        Byte[] licPktBytes = System.IO.File.ReadAllBytes(this.m_importPktDialog.FileName);
                        string licStream = "";
                        licStream = m_solLicServerWrapper.GenerateLicPackage_BySoftwareLicPacket(licPktBytes);
                        textBox1.Text = licStream.ToString();
                        SetPackage(this.m_importPktDialog.FileName, licStream);
                    }
                    #endregion
                    #region if License Archive
                    else if (string.Compare(System.IO.Path.GetExtension(this.m_importPktDialog.FileName), ".licArchive", true) == 0)
                    {
                        Byte[] licPktBytes = System.IO.File.ReadAllBytes(this.m_importPktDialog.FileName);
                        string licStream = "";
                        licStream = m_solLicServerWrapper.GenerateLicPackage_BySoftwareLicArchive(licPktBytes);
                        textBox1.Text = licStream.ToString();
                        SetPackage(this.m_importPktDialog.FileName, licStream);
                    }
                    #endregion
                    #region if License System Data
                    else //if (string.Compare(System.IO.Path.GetExtension(this.m_importPktDialog.FileName), ".lsData", true) == 0)
                    {
                        Byte[] licPktBytes = System.IO.File.ReadAllBytes(this.m_importPktDialog.FileName);
                        string licStream = "";
                        licStream = m_solLicServerWrapper.GenerateStream_ByLicenseSystemData(licPktBytes);
                        textBox1.Text = licStream.ToString();
                        SetSystemData(this.m_importPktDialog.FileName, licStream);
                    }
                    #endregion
                }
                catch (COMException ex)
                {
                    //alert user of failure
                    HandleExceptions.DisplayException(this, ex, "Failed to Import License System Data!", "License System Data");
                }
            }
        }

        private void closeLicenseSystemDataToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void cascadeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.LayoutMdi(System.Windows.Forms.MdiLayout.Cascade);
        }

        private void tileHorizontalToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.LayoutMdi(System.Windows.Forms.MdiLayout.TileHorizontal);
        }

        private void tileVerticalToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.LayoutMdi(System.Windows.Forms.MdiLayout.TileVertical);
        }

        private void arrangeIconsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.LayoutMdi(System.Windows.Forms.MdiLayout.ArrangeIcons);
        }




        private void connectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                m_solLicServerWrapper.Disconnect();
                m_connected = false;
                this.Text = string.Format("Solimar License Diagnostic Data Viewer - Disconnected");

                ConnectionDialog cDialog = new ConnectionDialog();
                cDialog.LoginCallBack += new ConnectionDialog.DelegateParamStringReturnBool(Connect);
                System.Collections.Generic.List<string> svrList = new System.Collections.Generic.List<string>();
                foreach (string server in Settings.Default.ServerList)
                    svrList.Add(server);
                cDialog.SetData(svrList);

                if (cDialog.ShowDialog() == DialogResult.OK)
                {
                    Settings.Default.ServerList.Clear();
                    Settings.Default.ServerList.AddRange(cDialog.GetData().ToArray());
                    Settings.Default.Save();
                    openLicenseSystemDataToolStripMenuItem_Click(sender, e);
                }
            }
            catch (COMException ex)
            {
                HandleExceptions.DisplayException(ex);
            }
        }
        //
        private bool Connect(string _server)
        {
            try
            {
                m_solLicServerWrapper.Connect(_server);
                this.Text = string.Format("Solimar License Diagnostic Data Viewer [{0}]", _server);
                m_connected = true;
            }
            catch (COMException ex)
            {
                if(this.InvokeRequired == false)
                    HandleExceptions.DisplayException(ex);
            }
            return m_connected;
        }

        private void fileToolStripMenuItem_DropDownOpening(object sender, EventArgs e)
        {
            openLicenseSystemDataToolStripMenuItem.Enabled = m_connected;
            closeLicenseSystemDataToolStripMenuItem.Enabled = m_connected;
        }

        #endregion

        #region Private Variables
        private Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseServerWrapper m_solLicServerWrapper;
        private bool m_connected;
        private string m_selectedDirectory;
        private System.Windows.Forms.OpenFileDialog m_importPktDialog;
		private Lic_LicenseSystemAttribs m_loadedLicSysAttribs;

        private TestForm m_textDialog = null;
		private System.Collections.Generic.Dictionary<string, Form> m_guidToFormMap = new Dictionary<string,Form>();
        #endregion

        private void LicenseDiagnosticDataViewerForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            m_solLicServerWrapper.Disconnect();
            m_solLicServerWrapper.Dispose();
        }


        #region Test Code
        private void makeObjectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Lic_ServerDataAttribs licSvrDataAttribs = new Lic_ServerDataAttribs();
            licSvrDataAttribs.fileInfoList.TVal.Add(new Lic_ServerDataAttribs.Lic_ServerDataFileInfoAttribs());
            textBox1.Text = licSvrDataAttribs.ToString();
        }

        private void testToolStripMenuItem_Click(object sender, EventArgs e)
        {
            TestForm dialog = new TestForm();
            dialog.MdiParent = this;
            dialog.Show();
        }

        private void makeLicPackageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            TestForm dialog = new TestForm();
            dialog.ShowDialog();
        }
        #endregion



        #region Left Nav Tree
        private void SetSystemData(string _fileName, string _licSysAttribsStreamed)
        {
            this.Cursor = Cursors.WaitCursor;
            leftTreeView.BeginUpdate();
            try
            {
                m_loadedLicSysAttribs = new Lic_LicenseSystemAttribs();
                m_loadedLicSysAttribs.AssignMembersFromStream(_licSysAttribsStreamed);

                leftTreeView.Nodes.Clear();
                foreach (Form mdiChild in this.MdiChildren)
                {
                    //if(!(mdiChild is TestForm))
                    mdiChild.Close();
                }

                TreeNode rootNode = new TreeNode(System.IO.Path.GetFileName(_fileName));
                leftTreeView.Nodes.Add(rootNode);

                if (m_loadedLicSysAttribs != null)
                {
                    TreeNode childNode = null;

                    StringBuilder toolTipBuilder = new StringBuilder();
                    string nodeText;

                    //nodeText = string.Format("Computer Name: {0}", m_loadedLicSysAttribs.computerName.ToString());
                    //toolTipBuilder.Append(nodeText);
                    //rootNode.Nodes.Add(new TreeNode(nodeText));

                    nodeText = string.Format("Created Date: {0}", m_loadedLicSysAttribs.createdDate.TVal.ToLocalTime().ToString());
                    toolTipBuilder.Append(nodeText);
                    rootNode.Nodes.Add(new TreeNode(nodeText));

                    //nodeText = string.Format("Bios Serial Number: {0}", m_loadedLicSysAttribs.biosSerialNumber.ToString());
                    //toolTipBuilder.Append("\r\n" + nodeText);
                    //rootNode.Nodes.Add(new TreeNode(nodeText));

                    //nodeText = string.Format("Mac Address: {0}", m_loadedLicSysAttribs.macAddress.ToString());
                    //toolTipBuilder.Append("\r\n" + nodeText);
                    //rootNode.Nodes.Add(new TreeNode(nodeText));

                    //nodeText = string.Format("System Product UUID: {0}", m_loadedLicSysAttribs.sysProdUuid.ToString());
                    //toolTipBuilder.Append("\r\n" + nodeText);
                    //rootNode.Nodes.Add(new TreeNode(nodeText));

                    childNode = new TreeNode("Lic_ServerDataAttribs");
                    childNode.Tag = Guid.NewGuid().ToString();
                    childNode.NodeFont = new Font(this.Font, FontStyle.Bold);

                    Lic_ServerDataAttribs tmpSvrDataAttrib = new Lic_ServerDataAttribs();
                    tmpSvrDataAttrib.AssignMembersFromStream(m_loadedLicSysAttribs.Streamed_ServerDataAttribs);
                    Lic_ServerDataAttribs_DisplayForm licSvrDataAttribsForm = new Lic_ServerDataAttribs_DisplayForm();
                    licSvrDataAttribsForm.SetData(tmpSvrDataAttrib);
                    licSvrDataAttribsForm.MdiParent = this;
                    licSvrDataAttribsForm.Show();
                    m_guidToFormMap.Add(childNode.Tag as string, licSvrDataAttribsForm);
                    rootNode.Nodes.Add(childNode);


                    //Lic_SystemInfoAttribs_DisplayForm
                    childNode = new TreeNode("Lic_SystemInfoAttribs");
                    childNode.Tag = Guid.NewGuid().ToString();
                    childNode.NodeFont = new Font(this.Font, FontStyle.Bold);
                    Lic_SystemInfoAttribs tmpSysInfoAttrib = new Lic_SystemInfoAttribs();
                    tmpSysInfoAttrib.AssignMembersFromStream(m_loadedLicSysAttribs.Streamed_SystemInfoAttribs);
                    Lic_SystemInfoAttribs_DisplayForm licSysInfoAttribsForm = new Lic_SystemInfoAttribs_DisplayForm();
                    licSysInfoAttribsForm.SetData(tmpSysInfoAttrib);
                    licSysInfoAttribsForm.MdiParent = this;
                    licSysInfoAttribsForm.Show();
                    m_guidToFormMap.Add(childNode.Tag as string, licSysInfoAttribsForm);
                    rootNode.Nodes.Add(childNode);

                    childNode = new TreeNode("Lic_SoftwareSpecAttribs");
                    childNode.Tag = Guid.NewGuid().ToString();
                    childNode.NodeFont = new Font(this.Font, FontStyle.Bold);
                    Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_SoftwareSpecAttribs tmpSoftwareSpecAttrib = new Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_SoftwareSpecAttribs();
                    tmpSoftwareSpecAttrib.AssignMembersFromStream(m_loadedLicSysAttribs.Streamed_SoftwareSpecAttribs);
                    Lic_SoftwareSpecAttribs_DisplayForm licSoftwareSpecAttribsForm = new Lic_SoftwareSpecAttribs_DisplayForm();
                    licSoftwareSpecAttribsForm.SetData(tmpSoftwareSpecAttrib);
                    licSoftwareSpecAttribsForm.MdiParent = this;
                    licSoftwareSpecAttribsForm.Show();
                    m_guidToFormMap.Add(childNode.Tag as string, licSoftwareSpecAttribsForm);
                    rootNode.Nodes.Add(childNode);


                    childNode = new TreeNode("List of Lic_KeyAttribs");
                    toolTipBuilder.Append(string.Format("\r\nList of Lic_KeyAttribs: {0}", m_loadedLicSysAttribs.ListOfStreamed_KeyAttribs.TVal.Count));
                    foreach (string streamedKeyAttrib in m_loadedLicSysAttribs.ListOfStreamed_KeyAttribs.TVal)
                    {
                        Lic_KeyAttribs tmpKeyAttribs = new Lic_KeyAttribs();
                        tmpKeyAttribs.AssignMembersFromStream(streamedKeyAttrib);
                        TreeNode tmpNode = new TreeNode(tmpKeyAttribs.keyName);
                        tmpNode.Tag = Guid.NewGuid().ToString();
                        tmpNode.NodeFont = new Font(this.Font, FontStyle.Bold);

                        Lic_KeyAttribs_DisplayForm keyDataAttribsForm = new Lic_KeyAttribs_DisplayForm();
                        keyDataAttribsForm.SetData(tmpKeyAttribs);
                        keyDataAttribsForm.MdiParent = this;
                        keyDataAttribsForm.Show();
                        m_guidToFormMap.Add(tmpNode.Tag as string, keyDataAttribsForm);


                        childNode.Nodes.Add(tmpNode);
                        //tmpKeyAttribs.layout.TVal.
                    }
                    rootNode.Nodes.Add(childNode);


                    childNode = new TreeNode("List of Lic_LicenseInfoAttribs");
                    childNode.Tag = Guid.NewGuid().ToString();
                    toolTipBuilder.Append(string.Format("\r\nList of Lic_LicenseInfoAttribso: {0}", m_loadedLicSysAttribs.ListOfStreamed_InfoAttribs.TVal.Count));
                    foreach (string streamedLicInfoAttrib in m_loadedLicSysAttribs.ListOfStreamed_InfoAttribs.TVal)
                    {
                        Lic_PackageAttribs.Lic_LicenseInfoAttribs tmpLicInfoAttribs = new Lic_PackageAttribs.Lic_LicenseInfoAttribs();
                        tmpLicInfoAttribs.AssignMembersFromStream(streamedLicInfoAttrib);
                        string softwareLicense = Solimar.Licensing.Attribs.Lic_LicenseInfoAttribsHelper.GetDisplayLabel(tmpLicInfoAttribs);
                        TreeNode tmpNode = new TreeNode(softwareLicense);
                        tmpNode.Tag = Guid.NewGuid().ToString();
                        tmpNode.NodeFont = new Font(this.Font, FontStyle.Bold);

                        Lic_LicenseInfoAttribs_DisplayForm licInfoAttribsForm = new Lic_LicenseInfoAttribs_DisplayForm();
                        licInfoAttribsForm.SetData(tmpLicInfoAttribs);
                        licInfoAttribsForm.MdiParent = this;
                        licInfoAttribsForm.Show();
                        m_guidToFormMap.Add(tmpNode.Tag as string, licInfoAttribsForm);

                        childNode.Nodes.Add(tmpNode);
                        //tmpLicInfoAttribs.
                        //tmpLicInfoAttribs.
                    }
                    rootNode.Nodes.Add(childNode);



                    childNode = new TreeNode("Raw XML");
                    childNode.Tag = Guid.NewGuid().ToString();
                    childNode.NodeFont = new Font(this.Font, FontStyle.Bold);
                    //Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_SoftwareSpecAttribs tmpSoftwareSpecAttrib = new Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_SoftwareSpecAttribs();
                    //tmpSoftwareSpecAttrib.AssignMembersFromStream(m_loadedLicSysAttribs.Streamed_SoftwareSpecAttribs);
                    XmlViewer_DisplayForm xmlForm = new XmlViewer_DisplayForm();
                    xmlForm.SetData(_licSysAttribsStreamed);
                    xmlForm.MdiParent = this;
                    xmlForm.Show();
                    m_guidToFormMap.Add(childNode.Tag as string, xmlForm);
                    rootNode.Nodes.Add(childNode);

                    rootNode.ToolTipText = toolTipBuilder.ToString();
                    rootNode.ExpandAll();
                }
            }
            catch (Exception ex)
            {
                textBox1.Text = ex.ToString();
            }
            finally
            {
                this.Cursor = Cursors.Default;
            }
            leftTreeView.EndUpdate();
        }

        private void SetPackage(string _fileName, string _licPackageAttribsStreamed)
        {
            this.Cursor = Cursors.WaitCursor;
            leftTreeView.BeginUpdate();
            try
            {
                StringBuilder toolTipBuilder = new StringBuilder();
                if (string.Compare(System.IO.Path.GetExtension(this.m_importPktDialog.FileName), ".vData", true) == 0)
                {
                    toolTipBuilder.Append("License Verification Data");
                }
                else if (string.Compare(System.IO.Path.GetExtension(this.m_importPktDialog.FileName), ".packet", true) == 0)
                {
                    toolTipBuilder.Append("License Packet");
                }
                else if (string.Compare(System.IO.Path.GetExtension(this.m_importPktDialog.FileName), ".licArchive", true) == 0)
                {
                    toolTipBuilder.Append("License Archive");
                }
                
                Solimar.Licensing.Attribs.Lic_PackageAttribs tmpLicPackage = new Solimar.Licensing.Attribs.Lic_PackageAttribs();
                tmpLicPackage.AssignMembersFromStream(_licPackageAttribsStreamed);

                leftTreeView.Nodes.Clear();
                foreach (Form mdiChild in this.MdiChildren)
                {
                    //if(!(mdiChild is TestForm))
                    mdiChild.Close();
                }

                TreeNode rootNode = new TreeNode(System.IO.Path.GetFileName(_fileName));
                leftTreeView.Nodes.Add(rootNode);

                if (tmpLicPackage != null)
                {
                    //Lic_PackageAttribs.Lic_LicenseInfoAttribs tmpLicInfoAttribs = new Lic_PackageAttribs.Lic_LicenseInfoAttribs();
                    //tmpLicInfoAttribs.AssignMembersFromStream(streamedLicInfoAttrib);
                    string softwareLicense = Solimar.Licensing.Attribs.Lic_LicenseInfoAttribsHelper.GetDisplayLabel(tmpLicPackage.licLicenseInfoAttribs);
                    TreeNode childNode = new TreeNode(softwareLicense);
                    childNode.Tag = Guid.NewGuid().ToString();
                    childNode.NodeFont = new Font(this.Font, FontStyle.Bold);

                    Lic_LicenseInfoAttribs_DisplayForm licInfoAttribsForm = new Lic_LicenseInfoAttribs_DisplayForm();
                    licInfoAttribsForm.SetData(tmpLicPackage.licLicenseInfoAttribs);
                    licInfoAttribsForm.MdiParent = this;
                    licInfoAttribsForm.Show();
                    m_guidToFormMap.Add(childNode.Tag as string, licInfoAttribsForm);
                    rootNode.Nodes.Add(childNode);

                    childNode = new TreeNode("Raw XML");
                    childNode.Tag = Guid.NewGuid().ToString();
                    childNode.NodeFont = new Font(this.Font, FontStyle.Bold);
                    XmlViewer_DisplayForm xmlForm = new XmlViewer_DisplayForm();
                    xmlForm.SetData(_licPackageAttribsStreamed);
                    xmlForm.MdiParent = this;
                    xmlForm.Show();
                    m_guidToFormMap.Add(childNode.Tag as string, xmlForm);
                    rootNode.Nodes.Add(childNode);

                    rootNode.ToolTipText = toolTipBuilder.ToString();
                    rootNode.ExpandAll();
                }
            }
            catch (Exception ex)
            {
                textBox1.Text = ex.ToString();
            }
            finally
            {
                this.Cursor = Cursors.Default;
            }
            leftTreeView.EndUpdate();
        }

        private void SetRawXml(string _xml)
        { 
        }


		private void leftTreeView_DoubleClick(object sender, EventArgs e)
		{
			if (leftTreeView.SelectedNode != null)
			{
				if (leftTreeView.SelectedNode.Tag is string)
				{
					Form tmpForm = m_guidToFormMap[leftTreeView.SelectedNode.Tag as string];
					if (tmpForm != null)
					{
						tmpForm.Focus();
						if(this.ActiveMdiChild.WindowState == FormWindowState.Minimized)
							tmpForm.WindowState = FormWindowState.Normal;
						else
							tmpForm.WindowState = this.ActiveMdiChild.WindowState;
						//tmpForm.s/
						//this.ActivateMdiChild(tmpForm);
					}
				}
			}
		}
        #endregion

        private bool m_firstTimeActivate = true;
        private void LicenseDiagnosticDataViewerForm_Activated(object sender, EventArgs e)
        {
            if (m_firstTimeActivate)
            {
                m_firstTimeActivate = false;
                connectToolStripMenuItem_Click(sender, e);
            }
        }



	}



    //Was lazy, this should be in it's own file - JWL... 
    public class HandleExceptions
    {
        public HandleExceptions()
        {
        }

        public static void DisplayException(System.Exception paramException)
        {
            DisplayException(null, paramException, "", "Error");
        }


        public static void DisplayException(System.Exception paramException, string paramMessageString)
        {
            DisplayException(null, paramException, paramMessageString, "Error");
        }

        public static void DisplayException(System.Exception paramException, string paramMessageString, string paramCaptionString)
        {
            DisplayException(null, paramException, paramMessageString, paramCaptionString);
        }

        public static void DisplayException(System.Windows.Forms.Form paramParentForm, System.Exception paramException, string paramMessageString, string paramCaptionString)
        {
            StringBuilder errBuilder = new StringBuilder();

            if (paramMessageString.Length > 0)
                errBuilder.Append(paramMessageString);
            if (errBuilder.Length > 0)
                errBuilder.Append("\n\r\nReason - ");

            errBuilder.Append(paramException.Message);

            MessageBox.Show(paramParentForm,
                            errBuilder.ToString(),
                            paramCaptionString,
                            MessageBoxButtons.OK,
                            MessageBoxIcon.Error);
        }
    }
}
