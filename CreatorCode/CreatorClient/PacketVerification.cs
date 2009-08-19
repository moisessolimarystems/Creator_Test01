using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Client.Creator.CreatorService;
using System.Security.Principal;
using System.IO;
using Solimar.Licensing.Attribs;

namespace Client.Creator
{
    public partial class PacketVerification : Shared.VisualComponents.DialogBaseForm
    {
        private bool _validVerificationCode;
        private string _selectedPacketName;
        public PacketVerification()
        {
            _validVerificationCode = false;
            InitializeComponent();
        }

        private void PacketVerification_InitDialog(object sender, Shared.VisualComponents.InitDialogEventArgs e)
        {   
            //_selectedPacketName = (e.Data as PacketVerificationDialogData).SelectedPacketName;
        }

        private void PacketVerification_FinishDialog(object sender, Shared.VisualComponents.FinishDialogEventArgs e)
        {
            PacketVerificationDialogData data = e.Data as PacketVerificationDialogData;
            data.Verified = _validVerificationCode;
            data.SelectedPacketName = _selectedPacketName;
        }

        private void PacketVerification_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (DialogResult == DialogResult.OK)
                if (!_validVerificationCode)
                    e.Cancel = true;
        }

        private void btnOk_Click(object sender, EventArgs e)
        {
            ValidateVerificationCode();
        }

        private void ValidateVerificationCode()
        {
            Client.Creator.ServiceProxy.Service<ICreator>.Use((client) =>
            {
                //_validVerificationCode = client.ValidateVerificationCode(_selectedPacketName, verificationCodeTextBox.Text);
                //if (_validVerificationCode)
                //{
                    PacketTable storedPacket = client.GetPacketByVerificationCode(verificationCodeTextBox.Text);
                    if (storedPacket != null)
                    {
                        storedPacket.IsVerified = true;
                        storedPacket.VerifiedBy = WindowsIdentity.GetCurrent().Name;
                        client.UpdatePacket(storedPacket);
                        _selectedPacketName = storedPacket.PacketName;
                        MessageBox.Show("Verified packet " + _selectedPacketName, 
                                        "Successful Verification",
                                        MessageBoxButtons.OK,
                                        MessageBoxIcon.Information);
                        _validVerificationCode = true;
                    }
                    else
                    {
                        // Set the ErrorProvider error with the text to display.
                        MessageBox.Show("Verification code entered could not be validated for any packet.",
                                        "Verification Code Error",
                                        MessageBoxButtons.OK,
                                        MessageBoxIcon.Error);
                        verificationCodeTextBox.Select(0, this.verificationCodeTextBox.Text.Length);      
                    }
                //}
            });
            //if (!_validVerificationCode)
            //{                
            //    // Set the ErrorProvider error with the text to display.
            //    MessageBox.Show(string.Format("Verification code entered is invalid for packet : {0}", _selectedPacketName),
            //                    "Verification Code Error",
            //                    MessageBoxButtons.OK,
            //                    MessageBoxIcon.Error);
            //    verificationCodeTextBox.Select(0, this.verificationCodeTextBox.Text.Length);                    
            //}
         }

        private void VerificationFileBrowseButton_Click(object sender, EventArgs e)
        {
            if (this.openFileDialog.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    Byte[] licPktBytes = System.IO.File.ReadAllBytes(this.openFileDialog.FileName);
                    string licStream = "";
                    Client.Creator.ServiceProxy.Service<ICreator>.Use((client) =>
                    {
                        licStream = client.GenerateLicInfoByVerifyData(ref licPktBytes);
                    });
                    Lic_PackageAttribs licPackage = new Lic_PackageAttribs();
                    licPackage.Stream = licStream;
                    int index = 0;
                    if(licPackage.licLicenseInfoAttribs.TVal.licVerificationAttribs.TVal.verificationCodeHistoryList.TVal.Count > 0);
                        index = licPackage.licLicenseInfoAttribs.TVal.licVerificationAttribs.TVal.verificationCodeHistoryList.TVal.Count - 1;
                    Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_VerificationCodeAttribs attrib = (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_VerificationCodeAttribs)licPackage.licLicenseInfoAttribs.TVal.licVerificationAttribs.TVal.verificationCodeHistoryList.TVal[index];
                    verificationCodeTextBox.Text = attrib.verificationValue;
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Error");
                }
            }
        }
    }
    #region PacketVerificationDialogData class
    public class PacketVerificationDialogData : Shared.VisualComponents.DialogData
    {
        private string _packetName;
        private bool _verified;

        #region Constructors

        public PacketVerificationDialogData()
        {
            _packetName = "";
            _verified = false;
        }
        #endregion

        #region Properties

        public string SelectedPacketName
        {
            get { return this._packetName; }
            set { this._packetName = value; }
        }

        public bool Verified
        {
            get { return _verified; }
            set { _verified = true; }
        }
        #endregion
    }
    #endregion

}
