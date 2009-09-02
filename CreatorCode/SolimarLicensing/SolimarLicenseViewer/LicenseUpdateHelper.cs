using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace SolimarLicenseViewer
{
    public partial class LicenseUpdateHelper : Shared.VisualComponents.DialogBaseForm
    {
        #region class LicenseUpdateHelperData
        public enum LicenseUpdateActionEnum
        {
            luaeEmpty = -1,
            luaeImportNewLicPackage = 0,
            luaeUpdateCurrentLicPackage = 1,
            luaeImportNewLicArchive = 2,
            luaeImportNewKeyPasswordPackage = 3,
        }
        public class LicenseUpdateHelperData
        {
            public LicenseUpdateHelperData(LicenseUpdateActionEnum _action, string _directory)
            {
                action = _action;
                directory = _directory;
            }
            public LicenseUpdateActionEnum action;
            public string directory;
        }

        #endregion

        #region Private Variables
        private CommunicationLink m_CommLink;
        private LicenseUpdateHelperData m_LicUpdateHelperData;

        private System.Collections.Generic.Dictionary<string, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs> m_existingSwLicInfoHash = new Dictionary<string, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs>();
        private System.Collections.Generic.Dictionary<string, Solimar.Licensing.Attribs.Lic_PackageAttribs> m_newSwLicPackageHash = new Dictionary<string, Solimar.Licensing.Attribs.Lic_PackageAttribs>();
        private System.Collections.Generic.Dictionary<string, string> m_newSwLicFileNameHash = new Dictionary<string,string>();
        #endregion
        public LicenseUpdateHelper()
        {
            InitializeComponent();
            Initialize();
        }
        private void Initialize()
        {
        }
        public LicenseUpdateHelperData GetData()
        {
            m_LicUpdateHelperData.directory = directoryTextBox.Text;
            return m_LicUpdateHelperData; 
        }
        public void SetData(LicenseUpdateHelperData _data, CommunicationLink _commLink)
        {
            m_CommLink = _commLink;
            m_LicUpdateHelperData = _data;
            directoryTextBox.Text = m_LicUpdateHelperData.directory;
            string caption = "";
            switch (m_LicUpdateHelperData.action)
            {
                case LicenseUpdateActionEnum.luaeImportNewLicArchive:
                    caption = "Add new Software License from License Archive";
                    break;
                case LicenseUpdateActionEnum.luaeImportNewLicPackage:
                    caption = "Add new Software License from License Package";
                    break;
                case LicenseUpdateActionEnum.luaeUpdateCurrentLicPackage:
                    caption = "Update current Software License from License Package";
                    break;
                case LicenseUpdateActionEnum.luaeImportNewKeyPasswordPackage:
                    caption = "Apply Protection Key Password Package";
                    break;
                default:
                    caption = "Unknown";
                    break;
            }
            this.Text = caption;

            //Populate m_existingSwLicHash
            string generalStream = null;
            m_CommLink.GetAllSoftwareLicenses(ref generalStream, false);
            Solimar.Licensing.Attribs.AttribsMemberStringList strList = new Solimar.Licensing.Attribs.AttribsMemberStringList("stringList", new System.Collections.ArrayList());
            strList.SVal = generalStream;
            foreach (string softwareLicense in strList.TVal)
            {
                Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs licInfoAttrib = new Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs();
                try
                {
                    m_CommLink.GetSoftwareLicenseInfoByLicense(softwareLicense, ref generalStream, false);
                    licInfoAttrib.AssignMembersFromStream(generalStream);
                }
                catch (Exception)
                {
                    continue;
                }
                if (m_existingSwLicInfoHash.ContainsKey(softwareLicense) == false)
                    m_existingSwLicInfoHash.Add(softwareLicense, licInfoAttrib);
            }


            RefreshLicenseListView();
            licSelectionListView_SelectedIndexChanged(licSelectionListView, new EventArgs());
        }

        private void btnOk_Click(object sender, EventArgs e)
        {
            if (ImportLicense() == true)
                DialogResult = DialogResult.OK;
            else
                DialogResult = DialogResult.None;   //continue running dialog
        }

        //returns true if successfully imported new License.
        private bool ImportLicense()
        {
            
            bool bSuccess = false;
            string licenseType = "";
            try
            {
                this.Cursor = Cursors.WaitCursor;
                if (licSelectionListView.SelectedItems.Count == 0)
                    throw new ArgumentException();
                string softwareLicense = licSelectionListView.SelectedItems[0].Text;

                if (m_newSwLicFileNameHash.ContainsKey(softwareLicense) == false)
                    throw new ArgumentException();

                Byte[] licBytes = System.IO.File.ReadAllBytes(m_newSwLicFileNameHash[softwareLicense]);
                string verificationCode = "";
                switch (m_LicUpdateHelperData.action)
                {
                    case LicenseUpdateActionEnum.luaeImportNewKeyPasswordPackage:
                        licenseType = "Protection Key Password Packet";
                        m_CommLink.EnterProtectionKeyPasswordPacket(licBytes, ref verificationCode);
                        break;
                    case LicenseUpdateActionEnum.luaeImportNewLicArchive:
                        licenseType = "License Archive";
                        m_CommLink.EnterSoftwareLicArchive(licBytes);
                        break;
                    case LicenseUpdateActionEnum.luaeImportNewLicPackage:
                    case LicenseUpdateActionEnum.luaeUpdateCurrentLicPackage:
                        licenseType = "License Packet";
                        m_CommLink.EnterSoftwareLicPacket(licBytes, ref verificationCode);
                        break;
                    default:
                        throw new ArgumentException();
                    //break;
                }
                bSuccess = true;
                MessageBox.Show(this, "Applied " + licenseType + " successfully.", "Apply " + licenseType, MessageBoxButtons.OK, MessageBoxIcon.Information);

            }
            catch (System.Runtime.InteropServices.COMException cEx)
            {
                HandleExceptions.DisplayException(this, cEx, "Failed to apply " + licenseType + "!", "Apply " + licenseType);
            }
            catch (Exception ex)
            {
                HandleExceptions.DisplayException(this, ex, "Failed to apply " + licenseType + "!", "Apply " + licenseType);
            }
            finally
            {
                this.Cursor = Cursors.Default;
            }
            return bSuccess;
        }

        private void directoryFindButton_Click(object sender, EventArgs e)
        {
            if (directoryTextBox.Text.Length != 0)
                folderBrowserDialog.SelectedPath = directoryTextBox.Text;
            if (folderBrowserDialog.ShowDialog() == DialogResult.OK)
            {
                directoryTextBox.Text = folderBrowserDialog.SelectedPath;
                RefreshLicenseListView();
            }
        }
        private void directoryTextBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter || e.KeyCode == Keys.Return)
            {
                RefreshLicenseListView();
            }
        }
        private void RefreshLicenseListView()
        {
            //cycle through all the files in directoryTextBox.Text
            string ext = "";
            switch (m_LicUpdateHelperData.action)
            {
                case LicenseUpdateActionEnum.luaeImportNewLicArchive:
                    ext = "*.licArchive";
                    break;
                case LicenseUpdateActionEnum.luaeImportNewLicPackage:
                case LicenseUpdateActionEnum.luaeUpdateCurrentLicPackage:
                    ext = "*.packet";
                    break;
                case LicenseUpdateActionEnum.luaeImportNewKeyPasswordPackage:
                    ext = ".pkt";
                    break;
                default:
                    ext = "*";
                    break;
            }
            if (directoryTextBox.Text.Length != 0)
            {
                if (System.IO.Directory.Exists(directoryTextBox.Text))
                {
                    try
                    {
                        licSelectionListView.BeginUpdate();
                        licSelectionListView.Items.Clear();
                        m_newSwLicPackageHash.Clear();
                        m_newSwLicFileNameHash.Clear();

                        string[] fileEntries = System.IO.Directory.GetFiles(directoryTextBox.Text, ext);
                        foreach (string fileName in fileEntries)
                        {
                            try
                            {
                                Byte[] licPktBytes = System.IO.File.ReadAllBytes(fileName);
                                string licStream = "";
                                if (m_LicUpdateHelperData.action == LicenseUpdateActionEnum.luaeImportNewLicPackage ||
                                    m_LicUpdateHelperData.action == LicenseUpdateActionEnum.luaeUpdateCurrentLicPackage)
                                {
                                    m_CommLink.GenerateLicPackage_BySoftwareLicPacket(licPktBytes, ref licStream);
                                }
                                else if (m_LicUpdateHelperData.action == LicenseUpdateActionEnum.luaeImportNewLicArchive)
                                {
                                    m_CommLink.GenerateLicPackage_BySoftwareLicArchive(licPktBytes, ref licStream);
                                }


                                Solimar.Licensing.Attribs.Lic_PackageAttribs tmpLicPackageAttrib = new Solimar.Licensing.Attribs.Lic_PackageAttribs();
                                tmpLicPackageAttrib.AssignMembersFromStream(licStream);
                                string softwareLicense = Solimar.Licensing.Attribs.Lic_LicenseInfoAttribsHelper.GetDisplayLabel(tmpLicPackageAttrib.licLicenseInfoAttribs);

                                bool bAddItem = false;
                                if (m_LicUpdateHelperData.action == LicenseUpdateActionEnum.luaeImportNewLicPackage)
                                {
                                    bAddItem = (m_existingSwLicInfoHash.ContainsKey(softwareLicense) == false);
                                }
                                else if (m_LicUpdateHelperData.action == LicenseUpdateActionEnum.luaeImportNewLicArchive)
                                {
                                    bAddItem = true;
                                }
                                else if (m_LicUpdateHelperData.action == LicenseUpdateActionEnum.luaeUpdateCurrentLicPackage)
                                {
                                    bAddItem = (m_existingSwLicInfoHash.ContainsKey(softwareLicense) == true);
                                }
                                else if (m_LicUpdateHelperData.action == LicenseUpdateActionEnum.luaeImportNewKeyPasswordPackage)
                                {
                                    bAddItem = true;
                                }
                                //System.Diagnostics.Trace.WriteLine(softwareLicense + ", " + tmpLicPackageAttrib.licLicenseInfoAttribs.TVal.modifiedDate.TVal.ToLocalTime().ToString());

                                if (bAddItem)
                                {
                                    if (m_newSwLicPackageHash.ContainsKey(softwareLicense) == false)
                                    {
                                        m_newSwLicPackageHash.Add(softwareLicense, tmpLicPackageAttrib);
                                        m_newSwLicFileNameHash.Add(softwareLicense, fileName);
                                    }
                                    else
                                    {
                                        if (DateTime.Compare(tmpLicPackageAttrib.licLicenseInfoAttribs.TVal.modifiedDate.TVal, m_newSwLicPackageHash[softwareLicense].licLicenseInfoAttribs.TVal.modifiedDate.TVal) > 0)
                                        {
                                            m_newSwLicPackageHash[softwareLicense] = tmpLicPackageAttrib;
                                            m_newSwLicFileNameHash[softwareLicense] = fileName;
                                        }
                                    }
                                }
                            }
                            catch (Exception)
                            {
                                continue;
                            }
                        }


                        foreach (string softwareLicense in m_newSwLicPackageHash.Keys)
                        {
                            bool bAddToDisplay = true;
                            ListViewItem lvi = new ListViewItem(softwareLicense);

                            lvi.Tag = true;
                            lvi.SubItems.Add(m_newSwLicPackageHash[softwareLicense].licLicenseInfoAttribs.TVal.modifiedDate.TVal.ToLocalTime().ToString());
                            string validation = "Found";
                            foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs verToken in m_newSwLicPackageHash[softwareLicense].licLicenseInfoAttribs.TVal.licVerificationAttribs.TVal.validationTokenList.TVal)
                            {
                                //if validation token is computer, only add if computer name is correct.
                                //if validation token is hardware id, add, but show in red and don't allow selection if key is not on system.
                                if (verToken.tokenType == Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttComputerName)
                                {
                                    bAddToDisplay = string.Compare(m_CommLink.ServerName, verToken.tokenValue.TVal, true) == 0;
                                }
                                else if (verToken.tokenType == Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID)
                                {
                                    lvi.Tag = false;
                                    lvi.ForeColor = System.Drawing.Color.Red;
                                    string licenseType = "";
                                    if (m_LicUpdateHelperData.action == LicenseUpdateActionEnum.luaeImportNewLicArchive)
                                        licenseType = "License Archive";
                                    else
                                        licenseType = "License Package";

                                    validation = "Validation Failed: " + licenseType + " requires Validation Key " + verToken.tokenValue.TVal.ToString() + " on License Server, key was not found.";
                                    foreach (Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseProtectionKeyInfo keyInfo in m_CommLink.KeyEnumerate())
                                    {
                                        if (string.Compare(keyInfo.keyName, verToken.tokenValue.TVal.ToString(), true) == 0)
                                        {
                                            validation = "Found";
                                            lvi.ForeColor = System.Drawing.SystemColors.WindowText;
                                            lvi.Tag = true;
                                        }
                                    }
                                }
                            }

                            if (m_existingSwLicInfoHash.ContainsKey(softwareLicense))
                            {
                                if (DateTime.Compare(m_existingSwLicInfoHash[softwareLicense].modifiedDate.TVal, m_newSwLicPackageHash[softwareLicense].licLicenseInfoAttribs.TVal.modifiedDate.TVal) >= 0)
                                {
                                    string licenseType = "";
                                    if (m_LicUpdateHelperData.action == LicenseUpdateActionEnum.luaeImportNewLicArchive)
                                        licenseType = "License Archive";
                                    else
                                        licenseType = "License Package";
                                    validation = "Validation Failed: " + licenseType + " has already been applied to License Server.";
                                    lvi.Tag = false;
                                    lvi.ForeColor = System.Drawing.Color.Red;
                                }
                            }

                            lvi.SubItems.Add(validation);
                            if (bAddToDisplay)
                            {
                                licSelectionListView.Items.Add(lvi);
                            }
                        }
                    }
                    catch (Exception)
                    {
                    }
                    finally
                    {
                        licSelectionListView.EndUpdate();
                    }
                }
            }
        }

        private void refreshButton_Click(object sender, EventArgs e)
        {
            RefreshLicenseListView();
        }

        private void licSelectionListView_SelectedIndexChanged(object sender, EventArgs e)
        {
            bool bEnable = false;
            if (licSelectionListView.SelectedItems.Count > 0)
                bEnable = (licSelectionListView.SelectedItems[0].Tag is bool) && (((bool)licSelectionListView.SelectedItems[0].Tag) == true);

            btnOk.Enabled = bEnable;
        }

    }
}
