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
        #region Fields

        bool _validVerificationCode;
        string _selectedPacketName, _licenseName, _licenseCode;      
        PacketVerificationDialogData _pktData;

        #endregion

        public PacketVerification()
        {
            _validVerificationCode = false;
            _pktData = null;
            InitializeComponent();
        }

        private void PacketVerification_InitDialog(object sender, Shared.VisualComponents.InitDialogEventArgs e)
        {
            _pktData = e.Data as PacketVerificationDialogData;
        }

        private void PacketVerification_FinishDialog(object sender, Shared.VisualComponents.FinishDialogEventArgs e)
        {
            PacketVerificationDialogData data = e.Data as PacketVerificationDialogData;
            data.Verified = _validVerificationCode;
            data.SelectedPacketName = _selectedPacketName;
            data.LicenseName = _licenseName;
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
                PacketTable storedPacket = client.GetPacketByVerificationCode(verificationCodeTextBox.Text);                
                if (storedPacket != null)
                {
                    if (storedPacket.IsVerified)
                    {
                        MessageBox.Show("Packet : " + storedPacket.PacketName + " has already been verified!", "Duplicate Verification - " + storedPacket.PacketName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    }
                    else
                    {
                        _selectedPacketName = storedPacket.PacketName;
                        LicenseTable lt = client.GetLicenseByID(storedPacket.LicenseID, false);
                        _licenseName = lt.LicenseName;
                        IList<PacketTable> packetList = client.GetPacketsByLicenseName(_licenseName);
                        List<PacketTable> modifiedPacketList = new List<PacketTable>();
                        if (packetList != null)
                        {
                            foreach (PacketTable packet in packetList)
                            {
                                if (packet.DateCreated.CompareTo(storedPacket.DateCreated) <= 0 && packet.IsVerified != true)
                                {
                                    packet.IsVerified = true;
                                    packet.VerifiedBy = WindowsIdentity.GetCurrent().Name;
                                    modifiedPacketList.Add(packet);
                                }
                            }                            
                            client.UpdatePackets(modifiedPacketList);
                            if (_licenseCode.Length > 0) //store licenseCode to allow reset when generating packets.
                            {
                                TokenTable licCodeToken = client.GetTokenByLicenseName(_licenseName, (byte)Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttLicenseCode);
                                if (licCodeToken != null)
                                {
                                    licCodeToken.TokenValue = _licenseCode;
                                    client.UpdateToken(licCodeToken);
                                }
                                else
                                {
                                    licCodeToken = new TokenTable()
                                    {
                                        CustID = lt.SCRnumber,
                                        LicenseID = lt.ID,
                                        TokenType = (byte)Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttLicenseCode,
                                        TokenValue = _licenseCode
                                    };
                                    client.CreateToken(licCodeToken);
                                }
                            }
                        }
                        MessageBox.Show("Verified packet " + _selectedPacketName, 
                                        "Successful Verification",
                                        MessageBoxButtons.OK,
                                        MessageBoxIcon.Information);
                        _validVerificationCode = true;
                    }
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
            });
         }

        private void VerificationFileBrowseButton_Click(object sender, EventArgs e)
        {
            if (this.openFileDialog.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    Byte[] licPktBytes = System.IO.File.ReadAllBytes(this.openFileDialog.FileName);
                    string licStream = string.Empty;
                    Client.Creator.ServiceProxy.Service<ICreator>.Use((client) =>
                    {
                        licStream = client.GenerateStreamByLicenseSystemData(ref licPktBytes);
                    });                    
                    Lic_LicenseSystemAttribs loadedLicSysAttribs = new Lic_LicenseSystemAttribs();
                    loadedLicSysAttribs.Stream = licStream;
                    foreach (string keyInfo in loadedLicSysAttribs.ListOfStreamed_InfoAttribs.TVal)
                    {                        
                        Lic_PackageAttribs.Lic_LicenseInfoAttribs tmpLicInfoAttribs = new Lic_PackageAttribs.Lic_LicenseInfoAttribs();
                        tmpLicInfoAttribs.Stream = keyInfo;
                        foreach (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs token in tmpLicInfoAttribs.licVerificationAttribs.TVal.validationTokenList.TVal)
                        {
                            if (token.tokenType == Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttLicenseCode)
                            {
                                _licenseCode = token.tokenValue;
                                break;
                            }
                        }
                        string licenseName = Lic_LicenseInfoAttribsHelper.GetDisplayLabel(tmpLicInfoAttribs);
                        if (licenseName == _pktData.LicenseName)
                        {
                            int index = 0;
                            if (tmpLicInfoAttribs.licVerificationAttribs.TVal.verificationCodeHistoryList.TVal.Count > 0) ;
                            index = tmpLicInfoAttribs.licVerificationAttribs.TVal.verificationCodeHistoryList.TVal.Count - 1;
                            Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_VerificationCodeAttribs attrib = (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_VerificationCodeAttribs)tmpLicInfoAttribs.licVerificationAttribs.TVal.verificationCodeHistoryList.TVal[index];
                            verificationCodeTextBox.Text = attrib.verificationValue;
                            break;
                        }
                    }
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
        private string _licenseName;
        private bool _verified;

        #region Constructors

        public PacketVerificationDialogData()
        {
            _packetName = string.Empty;
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

        public string LicenseName
        {
            get { return _licenseName; }
            set { _licenseName = value; }
        }
        #endregion
    }
    #endregion

}
