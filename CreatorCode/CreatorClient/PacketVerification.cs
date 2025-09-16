// ListViewMgr.cs
//
// SLB 15.sep.2025 CR.34456; Changes for new attribs code (Licensing 3.4+) to work.
// Removed some variable declarations which weren't being used (e.g catch() {} ex vars).
// Renamed member variables to have m_ prefix vs. _. 

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

        bool m_validVerificationCode;												// CR.34456; modified.
		string m_selectedPacketName;                                                // CR.34456; modified.
		string m_licenseName;														// CR.34456; modified.
		string m_licenseCode;														// CR.34456; modified.
		PacketVerificationDialogData m_pktData;                                     // CR.34456; modified.

		#endregion

		public PacketVerification()
        {
            m_validVerificationCode = false;
            m_pktData = null;
            InitializeComponent();
        }

        private void PacketVerification_InitDialog(object sender, Shared.VisualComponents.InitDialogEventArgs e)
        {
            m_pktData = e.Data as PacketVerificationDialogData;
        }

        private void PacketVerification_FinishDialog(object sender, Shared.VisualComponents.FinishDialogEventArgs e)
        {
            PacketVerificationDialogData data = e.Data as PacketVerificationDialogData;
            data.Verified = m_validVerificationCode;
            data.SelectedPacketName = m_selectedPacketName;
            data.LicenseName = m_licenseName;
        }

        private void PacketVerification_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (DialogResult == DialogResult.OK)
                if (!m_validVerificationCode)
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
                        m_selectedPacketName = storedPacket.PacketName;
                        LicenseTable lt = client.GetLicenseByID(storedPacket.LicenseID, false);
                        m_licenseName = lt.LicenseName;
                        IList<PacketTable> packetList = client.GetPacketsByLicenseName(m_licenseName);
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
                            if (m_licenseCode != null)
                            {
                                if (m_licenseCode.Length > 0) //store licenseCode to allow reset when generating packets.
                                {
                                    TokenTable licCodeToken = client.GetTokenByLicenseName(m_licenseName, (byte)Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttLicenseCode);
                                    if (licCodeToken != null)
                                    {
                                        licCodeToken.TokenValue = m_licenseCode;
                                        client.UpdateToken(licCodeToken);
                                    }
                                    else
                                    {
                                        licCodeToken = new TokenTable()
                                        {
                                            CustID = lt.SCRnumber,
                                            LicenseID = lt.ID,
                                            TokenType = (byte)Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttLicenseCode,
                                            TokenValue = m_licenseCode
                                        };
                                        client.CreateToken(licCodeToken);
                                    }
                                }
                            }
                        }
                        MessageBox.Show("Verified packet " + m_selectedPacketName, 
                                        "Successful Verification",
                                        MessageBoxButtons.OK,
                                        MessageBoxIcon.Information);
                        m_validVerificationCode = true;
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
                        foreach (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs token in tmpLicInfoAttribs.licVerificationAttribs.validationTokenList.TVal) // CR.34456; modified.
						{
                            if (token.tokenType == Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttLicenseCode)
                            {
                                m_licenseCode = token.tokenValue;
                                break;
                            }
                        }
                        string licenseName = Lic_LicenseInfoAttribsHelper.GetDisplayLabel(tmpLicInfoAttribs);
                        if (licenseName == m_pktData.LicenseName)
                        {
                            int index = 0;
                            if (tmpLicInfoAttribs.licVerificationAttribs.verificationCodeHistoryList.Count > 0) 
                                index = tmpLicInfoAttribs.licVerificationAttribs.verificationCodeHistoryList.Count - 1;
                            Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_VerificationCodeAttribs attrib = (Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_VerificationCodeAttribs)tmpLicInfoAttribs.licVerificationAttribs.verificationCodeHistoryList.TVal[index];
                            verificationCodeTextBox.Text = attrib.verificationValue;
                            break;
                        }
                    }
                }
                catch (Exception)													// CR.34456; modified.
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
