using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Solimar.Licensing.Common;
using System.Reflection;

namespace ValidationTokenRetriever
{
	public partial class Form1 : Form
	{
		public Form1()
		{
			InitializeComponent();

		}

		private void Initialize()
		{
			//CR.FIX.14343 - Add Customer Licensing Warning document to the beginning of the program.
			warningRichTextBox.Rtf = @"{\rtf1\ansi\ansicpg1252\uc1 \deff0\deflang1033\deflangfe1033{\fonttbl{\f0\froman\fcharset0\fprq2{\*\panose 02020603050405020304}Times New Roman;}{\f1\fswiss\fcharset0\fprq2{\*\panose 020b0604020202020204}Arial;}
{\f28\fswiss\fcharset128\fprq2{\*\panose 020b0604020202020204}Arial Unicode MS;}{\f29\fswiss\fcharset128\fprq2{\*\panose 020b0604020202020204}@Arial Unicode MS;}{\f30\froman\fcharset238\fprq2 Times New Roman CE;}
{\f31\froman\fcharset204\fprq2 Times New Roman Cyr;}{\f33\froman\fcharset161\fprq2 Times New Roman Greek;}{\f34\froman\fcharset162\fprq2 Times New Roman Tur;}{\f35\froman\fcharset177\fprq2 Times New Roman (Hebrew);}
{\f36\froman\fcharset178\fprq2 Times New Roman (Arabic);}{\f37\froman\fcharset186\fprq2 Times New Roman Baltic;}{\f38\fswiss\fcharset238\fprq2 Arial CE;}{\f39\fswiss\fcharset204\fprq2 Arial Cyr;}{\f41\fswiss\fcharset161\fprq2 Arial Greek;}
{\f42\fswiss\fcharset162\fprq2 Arial Tur;}{\f43\fswiss\fcharset177\fprq2 Arial (Hebrew);}{\f44\fswiss\fcharset178\fprq2 Arial (Arabic);}{\f45\fswiss\fcharset186\fprq2 Arial Baltic;}{\f256\fswiss\fcharset0\fprq2 Arial Unicode MS Western;}
{\f254\fswiss\fcharset238\fprq2 Arial Unicode MS CE;}{\f255\fswiss\fcharset204\fprq2 Arial Unicode MS Cyr;}{\f257\fswiss\fcharset161\fprq2 Arial Unicode MS Greek;}{\f258\fswiss\fcharset162\fprq2 Arial Unicode MS Tur;}
{\f259\fswiss\fcharset177\fprq2 Arial Unicode MS (Hebrew);}{\f260\fswiss\fcharset178\fprq2 Arial Unicode MS (Arabic);}{\f261\fswiss\fcharset186\fprq2 Arial Unicode MS Baltic;}{\f264\fswiss\fcharset0\fprq2 @Arial Unicode MS Western;}
{\f262\fswiss\fcharset238\fprq2 @Arial Unicode MS CE;}{\f263\fswiss\fcharset204\fprq2 @Arial Unicode MS Cyr;}{\f265\fswiss\fcharset161\fprq2 @Arial Unicode MS Greek;}{\f266\fswiss\fcharset162\fprq2 @Arial Unicode MS Tur;}
{\f267\fswiss\fcharset177\fprq2 @Arial Unicode MS (Hebrew);}{\f268\fswiss\fcharset178\fprq2 @Arial Unicode MS (Arabic);}{\f269\fswiss\fcharset186\fprq2 @Arial Unicode MS Baltic;}}{\colortbl;\red0\green0\blue0;\red0\green0\blue255;\red0\green255\blue255;
\red0\green255\blue0;\red255\green0\blue255;\red255\green0\blue0;\red255\green255\blue0;\red255\green255\blue255;\red0\green0\blue128;\red0\green128\blue128;\red0\green128\blue0;\red128\green0\blue128;\red128\green0\blue0;\red128\green128\blue0;
\red128\green128\blue128;\red192\green192\blue192;}{\stylesheet{\ql \li0\ri0\widctlpar\aspalpha\aspnum\faauto\adjustright\rin0\lin0\itap0 \f1\fs22\lang1033\langfe1033\cgrid\langnp1033\langfenp1033 \snext0 Normal;}{\*\cs10 \additive 
Default Paragraph Font;}{\*\cs15 \additive \b \sbasedon10 Strong;}{\s16\ql \li0\ri0\sb100\sa100\sbauto1\saauto1\widctlpar\aspalpha\aspnum\faauto\adjustright\rin0\lin0\itap0 
\fs24\lang1033\langfe1033\loch\f28\hich\af28\dbch\af28\cgrid\langnp1033\langfenp1033 \sbasedon0 \snext16 Normal (Web);}{\*\cs17 \additive \ul\cf2 \sbasedon10 Hyperlink;}}{\*\listtable{\list\listtemplateid1698360456\listhybrid{\listlevel\levelnfc0
\levelnfcn0\leveljc0\leveljcn0\levelfollow0\levelstartat1\levelspace0\levelindent0{\leveltext\leveltemplateid-1524838112\'02\'00.;}{\levelnumbers\'01;}\b\i0\f1\fs24\chbrdr\brdrnone\brdrcf1 \chshdng0\chcfpat1\chcbpat1\fbias0 \fi-360\li720\jclisttab\tx720 }
{\listlevel\levelnfc0\levelnfcn0\leveljc0\leveljcn0\levelfollow0\levelstartat1\levelspace0\levelindent0{\leveltext\leveltemplateid477280836\'02\'01.;}{\levelnumbers\'01;}\chbrdr\brdrnone\brdrcf1 \chshdng0\chcfpat1\chcbpat1 \fi-360\li1440\jclisttab\tx1440 
}{\listlevel\levelnfc0\levelnfcn0\leveljc0\leveljcn0\levelfollow0\levelstartat1\levelspace0\levelindent0{\leveltext\leveltemplateid1150816644\'02\'02.;}{\levelnumbers\'01;}\chbrdr\brdrnone\brdrcf1 \chshdng0\chcfpat1\chcbpat1 \fi-360\li2160
\jclisttab\tx2160 }{\listlevel\levelnfc0\levelnfcn0\leveljc0\leveljcn0\levelfollow0\levelstartat1\levelspace0\levelindent0{\leveltext\leveltemplateid1292945438\'02\'03.;}{\levelnumbers\'01;}\chbrdr\brdrnone\brdrcf1 \chshdng0\chcfpat1\chcbpat1 
\fi-360\li2880\jclisttab\tx2880 }{\listlevel\levelnfc0\levelnfcn0\leveljc0\leveljcn0\levelfollow0\levelstartat1\levelspace0\levelindent0{\leveltext\leveltemplateid-1668540604\'02\'04.;}{\levelnumbers\'01;}\chbrdr\brdrnone\brdrcf1 
\chshdng0\chcfpat1\chcbpat1 \fi-360\li3600\jclisttab\tx3600 }{\listlevel\levelnfc0\levelnfcn0\leveljc0\leveljcn0\levelfollow0\levelstartat1\levelspace0\levelindent0{\leveltext\leveltemplateid-489628448\'02\'05.;}{\levelnumbers\'01;}\chbrdr
\brdrnone\brdrcf1 \chshdng0\chcfpat1\chcbpat1 \fi-360\li4320\jclisttab\tx4320 }{\listlevel\levelnfc0\levelnfcn0\leveljc0\leveljcn0\levelfollow0\levelstartat1\levelspace0\levelindent0{\leveltext\leveltemplateid287476360\'02\'06.;}{\levelnumbers\'01;}
\chbrdr\brdrnone\brdrcf1 \chshdng0\chcfpat1\chcbpat1 \fi-360\li5040\jclisttab\tx5040 }{\listlevel\levelnfc0\levelnfcn0\leveljc0\leveljcn0\levelfollow0\levelstartat1\levelspace0\levelindent0{\leveltext\leveltemplateid-163304844\'02\'07.;}{\levelnumbers
\'01;}\chbrdr\brdrnone\brdrcf1 \chshdng0\chcfpat1\chcbpat1 \fi-360\li5760\jclisttab\tx5760 }{\listlevel\levelnfc0\levelnfcn0\leveljc0\leveljcn0\levelfollow0\levelstartat1\levelspace0\levelindent0{\leveltext\leveltemplateid828114022
\'02\'08.;}{\levelnumbers\'01;}\chbrdr\brdrnone\brdrcf1 \chshdng0\chcfpat1\chcbpat1 \fi-360\li6480\jclisttab\tx6480 }{\listname ;}\listid745226411}}{\*\listoverridetable{\listoverride\listid745226411\listoverridecount0\ls1}}{\info
{\title PLEASE READ THIS NOTICE}{\author jflynn}{\operator jlan}{\creatim\yr2011\mo2\dy2\hr10\min8}{\revtim\yr2011\mo2\dy2\hr10\min8}{\version2}{\edmins1}{\nofpages2}{\nofwords484}{\nofchars2761}{\*\company Solimar Systems, Inc.}{\nofcharsws3390}
{\vern8243}}\margl1296\margr1296\margt1296\margb1296 \widowctrl\ftnbj\aenddoc\noxlattoyen\expshrtn\noultrlspc\dntblnsbdb\nospaceforul\formshade\horzdoc\dgmargin\dghspace180\dgvspace180\dghorigin1296\dgvorigin1296\dghshow1\dgvshow1
\jexpand\viewkind1\viewscale100\pgbrdrhead\pgbrdrfoot\splytwnine\ftnlytwnine\htmautsp\nolnhtadjtbl\useltbaln\alntblind\lytcalctblwd\lyttblrtgr\lnbrkrule \fet0\sectd \linex0\endnhere\sectlinegrid360\sectdefaultcl {\*\pnseclvl1
\pnucrm\pnstart1\pnindent720\pnhang{\pntxta .}}{\*\pnseclvl2\pnucltr\pnstart1\pnindent720\pnhang{\pntxta .}}{\*\pnseclvl3\pndec\pnstart1\pnindent720\pnhang{\pntxta .}}{\*\pnseclvl4\pnlcltr\pnstart1\pnindent720\pnhang{\pntxta )}}{\*\pnseclvl5
\pndec\pnstart1\pnindent720\pnhang{\pntxtb (}{\pntxta )}}{\*\pnseclvl6\pnlcltr\pnstart1\pnindent720\pnhang{\pntxtb (}{\pntxta )}}{\*\pnseclvl7\pnlcrm\pnstart1\pnindent720\pnhang{\pntxtb (}{\pntxta )}}{\*\pnseclvl8\pnlcltr\pnstart1\pnindent720\pnhang
{\pntxtb (}{\pntxta )}}{\*\pnseclvl9\pnlcrm\pnstart1\pnindent720\pnhang{\pntxtb (}{\pntxta )}}\pard\plain \qc \li0\ri0\widctlpar\aspalpha\aspnum\faauto\adjustright\rin0\lin0\itap0 \f1\fs22\lang1033\langfe1033\cgrid\langnp1033\langfenp1033 {\cs15\b\fs48 
PLEASE READ THIS NOTICE}{\fs24\loch\af28\hich\af28\dbch\af28 
\par }\pard \ql \li0\ri0\widctlpar\aspalpha\aspnum\faauto\adjustright\rin0\lin0\itap0 {\~
\par }\pard \qc \li0\ri0\widctlpar\aspalpha\aspnum\faauto\adjustright\rin0\lin0\itap0 {\cs15\b\fs36 IMPORTANT NOTICE CONCERNING\~SOLIMAR\~PRODUCT LICENSES\~GENERATED USING COMPUTER VALIDATION}{
\par }\pard\plain \s16\ql \li0\ri0\sb100\sa100\sbauto1\saauto1\widctlpar\aspalpha\aspnum\faauto\adjustright\rin0\lin0\itap0 \fs24\lang1033\langfe1033\loch\af28\hich\af28\dbch\af28\cgrid\langnp1033\langfenp1033 {\f1 \hich\af1\dbch\af28\loch\f1 
Solimar Systems, Inc. (""Solimar"") uses sophisticated\~systems to control the licensing of its software.}{\fs20 
\par }{\f1 \hich\af1\dbch\af28\loch\f1 Your organization has requested\~to use\hich\af1\dbch\af28\loch\f1  the ""Computer Validation""\~licensing system\~in lieu of a hardware USB device.\~The Computer Validation licensing system\~relies on\~
identifying information specific to the computer hosting the licensing system.}{\fs20 
\par }{\cs15\b\f1 \hich\af1\dbch\af28\loch\f1 Making changes to the identifying information us\hich\af1\dbch\af28\loch\f1 ed by the\~Computer Validation licensing system will invalidate\~the software\~license.}{\fs20 
\par }{\f1 \hich\af1\dbch\af28\loch\f1 It is }{\cs15\b\f1 \hich\af1\dbch\af28\loch\f1 VERY IMPORTANT}{\f1 \hich\af1\dbch\af28\loch\f1  for your organization to implement safeguards to avoid changing this\~
identifying information. Invalidating the Solimar product license can disable the\hich\af1\dbch\af28\loch\f1  Solimar\~software, resulting in\~production disruption. As well, your\~organization\~may incur significant fees\~for Solimar to\~generate a\~
replacement license.}{\fs20 
\par }{\f1 \hich\af1\dbch\af28\loch\f1 Solimar's Computer Validation licensing system uses\~identifying information collected by the Solimar\hich\af1\dbch\af28\loch\f1  ""Validation Token Retriever"" application.\~To obtain a licensing\~
key, you must run the Validation Token Retriever\~utility on the specific computer that will host the Solimar License Server software and\~you must send the resulting file to Solimar.\~Solimar \hich\af1\dbch\af28\loch\f1 w\hich\af1\dbch\af28\loch\f1 ill\~
return\~to you a\~""License Packet"" file containing a\~licensing\~key\~for the computer on which you ran the Validation Token Retriever.\~You\~will\~use the Solimar License Server software to apply\~the License Packet\~that enables the Solimar\~software.}{
\fs20 
\par }{\f1 \hich\af1\dbch\af28\loch\f1 The Compu\hich\af1\dbch\af28\loch\f1 ter Validation licensing system }{\f1\cf1 \hich\af1\dbch\af28\loch\f1 collects}{\f1 \hich\af1\dbch\af28\loch\f1  the following parameters to build\~the licensing\~key:}{\fs20 
\par }\pard\plain \ql \li0\ri0\widctlpar\aspalpha\aspnum\faauto\adjustright\rin0\lin0\itap0 \f1\fs22\lang1033\langfe1033\cgrid\langnp1033\langfenp1033 {\~\~\~\~\~\~\~ Company Name}{\fs20 
\par }{\~\~\~\~\~\~\~ Computer Name}{\fs20 
\par }{\~\~\~\~\~\~\~ Domain Name}{\fs20 
\par }{\~\~\~\~\~\~\~ BIOS Serial Number}{\fs20 
\par }{\~\~\~\~\~\~\~ MAC Address}{\fs20 
\par }{\~\~\~\~\~\~\~ Operating System}{\fs20 
\par }{\~\~\~\~\~\~\~ Part of Domain}{\fs20 
\par }{\~\~\~\~\~\~\~ System Manufacturer}{\fs20 
\par }{\~\~\~\~\~\~\~ System Model}{\fs20 
\par }{\~\~\~\~\~\~\~ System Type}{\fs20 
\par }{\~\~\~\~\~\~\~ System Product UUID}{\fs20 
\par }{\fs20 
\par }{\cs15\b\fs24 Changing the value of any of the parameters used\~by the Computer Validation\~licensing system, even on a virtualized computer, can invalidate the licensing key provided by Solimar. In addition, the following may occur:}{\fs24  
\par {\listtext\pard\plain\cs15\b\f1 \hich\af1\dbch\af0\loch\f1 1.\tab}}\pard \ql \fi-360\li720\ri0\sb100\sa100\sbauto1\saauto1\widctlpar\jclisttab\tx720\aspalpha\aspnum\faauto\ls1\adjustright\rin0\lin720\itap0 {\cs15\b\fs24 Your organization\~
may have to pay for a new license;}{\b\fs24 \~ }{\b\fs24 
\par {\listtext\pard\plain\cs15\b\f1 \hich\af1\dbch\af0\loch\f1 2.\tab}}{\cs15\b\fs24 Your organization\~may incur significant\~services fees\~for a\~replacement license;}{\b\fs24  }{\b\fs24 
\par {\listtext\pard\plain\cs15\b\f1 \hich\af1\dbch\af0\loch\f1 3.\tab}}{\cs15\b\fs24 Your organization may lose its eligibility\~to use\~the Computer\~Validation\~licensing system;}{\b\fs24  }{\fs20 
\par {\listtext\pard\plain\cs15\b\f1 \hich\af1\dbch\af0\loch\f1 4.\tab}}{\cs15\b\fs24 Your organization may experience significant software downtime\~and production disruptions because of the invalid license.}{\fs20 
\par }\pard\plain \s16\ql \li0\ri0\widctlpar\aspalpha\aspnum\faauto\adjustright\rin0\lin0\itap0 \fs24\lang1033\langfe1033\loch\af28\hich\af28\dbch\af28\cgrid\langnp1033\langfenp1033 {\f1 \hich\af1\dbch\af28\loch\f1 Solimar strongly recommends\~
that you advise anyone who might alter the configuration of a physical or vi\hich\af1\dbch\af28\loch\f1 rtualized\~computer running the Solimar License\~Server software\~}{\cs15\b\f1 \hich\af1\dbch\af28\loch\f1 NOT}{\f1 \~\hich\af1\dbch\af28\loch\f1 
to make any changes to any parameter used\~by the\~Computer Validation\~licensing system.}{
\par }\pard\plain \ql \li0\ri0\widctlpar\aspalpha\aspnum\faauto\adjustright\rin0\lin0\itap0 \f1\fs22\lang1033\langfe1033\cgrid\langnp1033\langfenp1033 {\fs24 If your organization wants to move\~the Solimar\~License Server\~
software to a different computer, even a virtualized one,\~from that for which the ""Validation Token Retriever"" was run,\~you\~must contact Solimar\~}{\cs15\b\fs24 BEFORE}{\fs24 \~taking any steps\~
to do so and you should follow the instructions Solimar then provides you.}{
\par }{\fs20 
\par }\pard\plain \s16\ql \li0\ri0\widctlpar\aspalpha\aspnum\faauto\adjustright\rin0\lin0\itap0 \fs24\lang1033\langfe1033\loch\af28\hich\af28\dbch\af28\cgrid\langnp1033\langfenp1033 {\f1 \hich\af1\dbch\af28\loch\f1 
You can contact the Solimar Technical Support Team \hich\af1\dbch\af28\loch\f1 by emailing: ""mailto:support@solimarsystems.com"".  Solimar also recommends that you\~generate and maintain a backup system image of the License Server computer\~each time you app
\hich\af1\dbch\af28\loch\f1 ly\~a License Packet file.}{\fs20 
\par }\pard\plain \ql \li0\ri0\widctlpar\aspalpha\aspnum\faauto\adjustright\rin0\lin0\itap0 \f1\fs22\lang1033\langfe1033\cgrid\langnp1033\langfenp1033 {\~}{\fs20 
\par }}";

			RefreshState();
		}
		private void exitToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Application.Exit();
		}


		private void RefreshState()
		{
			m_vData = new SolimarValidationData();
			StringBuilder errorStrBuilder = new StringBuilder();

			try
			{
				m_vData.DomainName = FindDomain();
			}
			catch (Exception exc)
			{
				if (errorStrBuilder.Length > 0)
					errorStrBuilder.Append("\r\n");
				errorStrBuilder.Append(exc.Message);
			}

			try
			{
				m_vData.PartOfDomain = FindPartOfDomain();
			}
			catch (Exception exc)
			{
				if (errorStrBuilder.Length > 0)
					errorStrBuilder.Append("\r\n");
				errorStrBuilder.Append(exc.Message);
			}

			try
			{
				m_vData.ComputerName = FindComputerName();
			}
			catch (Exception exc)
			{
				if (errorStrBuilder.Length > 0)
					errorStrBuilder.Append("\r\n");
				errorStrBuilder.Append(exc.Message);
			}

			try
			{
				m_vData.macAddress = FindMACAddress();
				if (m_vData.macAddress == string.Empty)
					throw new Exception("No MAC Address is detected, must have a MAC Address.");
			}
			catch (Exception exc)
			{
				if (errorStrBuilder.Length > 0)
					errorStrBuilder.Append("\r\n");
				errorStrBuilder.Append(exc.Message);
			}

			try
			{
				m_vData.SystemUUID = FindSystemUuid();
			}
			catch (Exception exc)
			{
				if (errorStrBuilder.Length > 0)
					errorStrBuilder.Append("\r\n");
				errorStrBuilder.Append(exc.Message);
			}

			try
			{
				m_vData.BiosSerialNumber = FindBiosSerialNumber();
			}
			catch (Exception exc)
			{
				if (errorStrBuilder.Length > 0)
					errorStrBuilder.Append("\r\n");
				errorStrBuilder.Append(exc.Message);
			}

			try
			{
				m_vData.SystemManufacturer = FindSystemManufacturer();
			}
			catch (Exception exc)
			{
				if (errorStrBuilder.Length > 0)
					errorStrBuilder.Append("\r\n");
				errorStrBuilder.Append(exc.Message);
			}

			try
			{
				m_vData.SystemModel = FindSystemModel();
			}
			catch (Exception exc)
			{
				if (errorStrBuilder.Length > 0)
					errorStrBuilder.Append("\r\n");
				errorStrBuilder.Append(exc.Message);
			}

			try
			{
				m_vData.SystemType = FindSystemType();
			}
			catch (Exception exc)
			{
				if (errorStrBuilder.Length > 0)
					errorStrBuilder.Append("\r\n");
				errorStrBuilder.Append(exc.Message);
			}
			try
			{
				m_vData.OperatingSystem = FindOperatingSystem();
			}
			catch (Exception exc)
			{
				if (errorStrBuilder.Length > 0)
					errorStrBuilder.Append("\r\n");
				errorStrBuilder.Append(exc.Message);
			}

			propertyGrid1.SelectedObject = m_vData;
			if (errorStrBuilder.Length > 0)
			{
				string errMsg = string.Format("Will be unable to generate Solimar Validation Token Data because: \r\n{0}", errorStrBuilder.ToString());
				this.m_errorMsg = string.Format("Error trying to generate Solimar Validation Token Data because: \r\n{0}", errorStrBuilder.ToString());
				//globalErrorProvider.SetError(genButton, errorStrBuilder.ToString());
				MessageBox.Show(this, errMsg, "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
				globalErrorProvider.SetError(genButton, errMsg);
			}
			this.ActiveControl = companyTextBox;
		}

		private string m_errorMsg = string.Empty;
		private SolimarValidationData m_vData = null;
		private string FindComputerName()
		{
			return System.Environment.MachineName.ToLower();
		}
		private string FindMACAddress()
		{
			//create out management class object using the
			//Win32_NetworkAdapterConfiguration class to get the attributes
			//af the network adapter
			System.Management.ManagementClass mgmt = new System.Management.ManagementClass("Win32_NetworkAdapterConfiguration");

			//create our ManagementObjectCollection to get the attributes with
			System.Management.ManagementObjectCollection objCol = mgmt.GetInstances();
			string address = String.Empty;
			//loop through all the objects we find
			foreach (System.Management.ManagementObject obj in objCol)
			{
				if (address == String.Empty)  // only return MAC Address from first card
				{
					//grab the value from the first network adapter we find
					//you can change the string to an array and get all
					//network adapters found as well
					if ((bool)obj["IPEnabled"] == true)
					{
						string serviceName = (string)obj["ServiceName"];
						if (serviceName != null && string.Compare(serviceName, "msloop") != 0)
						{
							address = obj["MacAddress"].ToString();
						}
					}
				}
				//dispose of our object
				obj.Dispose();
			}
			//replace the ":" with an empty space, this could also
			//be removed if you wish
			//address = address.Replace(":", "");
			//return the mac address
			return address.ToLower();
		}
		private string FindSystemUuid()
		{
			System.Management.ManagementObjectSearcher query1 = new System.Management.ManagementObjectSearcher("SELECT * FROM Win32_ComputerSystemProduct");
			System.Management.ManagementObjectCollection objList = query1.Get();
			StringBuilder strBuilderSerialNumber = new StringBuilder();
			string uuid = String.Empty;
			foreach (System.Management.ManagementObject obj in objList)
			{
				if (uuid == String.Empty && obj["UUID"] != null)  // only return first UUID hit
					uuid = obj["UUID"] != null ? obj["UUID"].ToString() : "NULL";
				obj.Dispose();
			}
			return uuid == String.Empty ? "NULL" : uuid;
		}
		private string FindBiosSerialNumber()
		{
			System.Management.ManagementObjectSearcher query1 = new System.Management.ManagementObjectSearcher("SELECT * FROM Win32_BIOS");
			System.Management.ManagementObjectCollection objList = query1.Get();
			StringBuilder strBuilderSerialNumber = new StringBuilder();
			string serialNumber = String.Empty;
			foreach (System.Management.ManagementObject obj in objList)
			{
				if (serialNumber == String.Empty && obj["SerialNumber"] != null)  // only return first UUID hit
					serialNumber = obj["SerialNumber"] != null ? obj["SerialNumber"].ToString() : "NULL";
				obj.Dispose();
			}
			return serialNumber == String.Empty ? "NULL" : serialNumber;

			//System.Management.ManagementObjectSearcher query1 = new System.Management.ManagementObjectSearcher("SELECT * FROM Win32_BIOS");
			//System.Management.ManagementObjectCollection sers = query1.Get();
			//StringBuilder strBuilderSerialNumber = new StringBuilder();
			////string dSerialNumber = String.Empty;
			//foreach (System.Management.ManagementObject serial in sers)
			//{
			//    //if (dSerialNumber == String.Empty)  // only return first BIOS serial number found
			//    //{
			//    //    dSerialNumber = serial["SerialNumber"] != null ? serial["SerialNumber"].ToString() : "NULL";
			//    //}
			//    if (strBuilderSerialNumber.Length != 0)
			//        strBuilderSerialNumber.Append(", ");
			//    strBuilderSerialNumber.Append("SerialNumber=\"");
			//    strBuilderSerialNumber.Append(serial["SerialNumber"] != null ? serial["SerialNumber"].ToString() : "NULL");
			//    strBuilderSerialNumber.Append("\"");

			//    //dispose of our object
			//    serial.Dispose();
			//}

			//query1 = new System.Management.ManagementObjectSearcher("Select * FROM Win32_BaseBoard");
			//System.Management.ManagementObjectCollection objList = query1.Get();
			//foreach (System.Management.ManagementObject obj in objList)
			//{
			//    strBuilderSerialNumber.Append(", Tag=\"");
			//    strBuilderSerialNumber.Append(obj["Tag"] != null ? obj["Tag"].ToString() : "NULL");
			//    strBuilderSerialNumber.Append("\", Manufacturer=\"");
			//    strBuilderSerialNumber.Append(obj["Manufacturer"] != null ? obj["Manufacturer"].ToString() : "NULL");
			//    strBuilderSerialNumber.Append("\", SerialNumber =\"");
			//    strBuilderSerialNumber.Append(obj["SerialNumber"] != null ? obj["SerialNumber"].ToString() : "NULL");
			//    strBuilderSerialNumber.Append("\", Version =\"");
			//    strBuilderSerialNumber.Append(obj["Version"] != null ? obj["Version"].ToString() : "NULL");
			//    strBuilderSerialNumber.Append("\", Product =\"");
			//    strBuilderSerialNumber.Append(obj["Product"] != null ? obj["Product"].ToString() : "NULL");
			//    strBuilderSerialNumber.Append("\"");
			//    obj.Dispose();
			//}

			////return dSerialNumber;
			//return strBuilderSerialNumber.ToString();
		}
		private string FindDomain()
		{
			System.Management.ManagementObjectSearcher query1 = new System.Management.ManagementObjectSearcher("SELECT * FROM Win32_ComputerSystem");
			System.Management.ManagementObjectCollection objList = query1.Get();
			StringBuilder strBuilderSerialNumber = new StringBuilder();
			string domain = String.Empty;
			foreach (System.Management.ManagementObject obj in objList)
			{
				if (domain == String.Empty && obj["Domain"] != null)  // only return first UUID hit
					domain = obj["Domain"] != null ? obj["Domain"].ToString() : "NULL";
				obj.Dispose();
			}
			return domain == String.Empty ? "NULL" : domain;
		}
		private string FindPartOfDomain()
		{
			System.Management.ManagementObjectSearcher query1 = new System.Management.ManagementObjectSearcher("SELECT * FROM Win32_ComputerSystem");
			System.Management.ManagementObjectCollection objList = query1.Get();
			StringBuilder strBuilderSerialNumber = new StringBuilder();
			string partOfDomain = String.Empty;
			foreach (System.Management.ManagementObject obj in objList)
			{
				if (partOfDomain == String.Empty && obj["PartOfDomain"] != null)  // only return first UUID hit
					partOfDomain = obj["PartOfDomain"] != null ? obj["PartOfDomain"].ToString() : "NULL";
				obj.Dispose();
			}
			return partOfDomain == String.Empty ? "NULL" : partOfDomain;
		}
		private string FindSystemManufacturer()
		{
			System.Management.ManagementObjectSearcher query1 = new System.Management.ManagementObjectSearcher("SELECT * FROM Win32_ComputerSystem");
			System.Management.ManagementObjectCollection objList = query1.Get();
			StringBuilder strBuilderSerialNumber = new StringBuilder();
			string manufacturer = String.Empty;
			foreach (System.Management.ManagementObject obj in objList)
			{
				if (manufacturer == String.Empty && obj["Manufacturer"] != null)  // only return first manufacturer hit
					manufacturer = obj["Manufacturer"] != null ? obj["Manufacturer"].ToString() : "NULL";
				obj.Dispose();
			}
			return manufacturer == String.Empty ? "NULL" : manufacturer;
		}
		private string FindSystemModel()
		{
			System.Management.ManagementObjectSearcher query1 = new System.Management.ManagementObjectSearcher("SELECT * FROM Win32_ComputerSystem");
			System.Management.ManagementObjectCollection objList = query1.Get();
			StringBuilder strBuilderSerialNumber = new StringBuilder();
			string model = String.Empty;
			foreach (System.Management.ManagementObject obj in objList)
			{
				if (model == String.Empty && obj["Model"] != null)  // only return first model hit
					model = obj["Model"] != null ? obj["Model"].ToString() : "NULL";
				obj.Dispose();
			}
			return model == String.Empty ? "NULL" : model;
		}
		private string FindSystemType()
		{
			System.Management.ManagementObjectSearcher query1 = new System.Management.ManagementObjectSearcher("SELECT * FROM Win32_ComputerSystem");
			System.Management.ManagementObjectCollection objList = query1.Get();
			StringBuilder strBuilderSerialNumber = new StringBuilder();
			string systemType = String.Empty;
			foreach (System.Management.ManagementObject obj in objList)
			{
				if (systemType == String.Empty && obj["SystemType"] != null)  // only return first SystemType hit
					systemType = obj["SystemType"] != null ? obj["SystemType"].ToString() : "NULL";
				obj.Dispose();
			}
			return systemType == String.Empty ? "NULL" : systemType;
		}
		private string FindOperatingSystem()
		{
			System.Management.ManagementObjectSearcher query1 = new System.Management.ManagementObjectSearcher("SELECT * FROM Win32_OperatingSystem");
			System.Management.ManagementObjectCollection objList = query1.Get();
			StringBuilder strBuilderSerialNumber = new StringBuilder();
			string operatingSystem = String.Empty;
			foreach (System.Management.ManagementObject obj in objList)
			{
				if (operatingSystem == String.Empty && obj["Caption"] != null)  // only return first Caption hit
				{
					operatingSystem = obj["Caption"] != null ? obj["Caption"].ToString() : "NULL";

					// The '™' and '®' character causes the encrypting of the xml to fail, remove.
					operatingSystem = operatingSystem.Replace('™', ' ').Replace('®', ' ');
				}

				obj.Dispose();
			}
			return operatingSystem == String.Empty ? "NULL" : operatingSystem;
		}

		private void genButton_Click(object sender, EventArgs e)
		{
			exportToFile();

		}
		private void generateFileForSolimarSystemsToolStripMenuItem_Click(object sender, EventArgs e)
		{
			exportToFile();
		}

		private void copyAllContentsToClipboardToolStripMenuItem_Click(object sender, EventArgs e)
		{
			//System.Windows.Forms.Clipboard.SetText(generateExportText());
			System.Windows.Forms.Clipboard.SetText(m_vData.ToCSV());
		}

		private void exportToFile()
		{
			if (this.m_errorMsg == string.Empty)
			{
				globalSaveFileDialog.FileName = MakeFileNameSafe(m_vData.CompanyName) + "-" + MakeFileNameSafe(m_vData.ComputerName) + ".svt.csv";
				if (globalSaveFileDialog.ShowDialog() == DialogResult.OK)
				{
					using (System.IO.TextWriter txtWriter = new System.IO.StreamWriter(globalSaveFileDialog.OpenFile()))
					{
						//txtWriter.Write(generateExportText());
						txtWriter.Write(m_vData.ToCSV());
						txtWriter.Close();
					}
				}
			}
			else
			{
				MessageBox.Show(this, this.m_errorMsg, "Error generating Validation Token Data", MessageBoxButtons.OK, MessageBoxIcon.Error);
				globalErrorProvider.SetError(genButton, this.m_errorMsg);
			}
		}
		private string generateExportText()
		{
			StringBuilder strLine = new StringBuilder();
			strLine.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe("Field"));
			strLine.Append(",");
			strLine.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe("Value"));
			strLine.Append("\r\n");

			strLine.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe("Version"));
			strLine.Append(",");
			strLine.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe("1.0.0.0"));
			strLine.Append("\r\n");

			strLine.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe("Company"));
			strLine.Append(",");
			strLine.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe(companyTextBox.Text));
			strLine.Append("\r\n");

			strLine.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe("Computer Name"));
			strLine.Append(",");
			strLine.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe(compNameTextBox.Text));
			strLine.Append("\r\n");

			strLine.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe("Mac Address"));
			strLine.Append(",");
			strLine.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe(macTextBox.Text));
			strLine.Append("\r\n");

			strLine.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe("System Product UUID"));
			strLine.Append(",");
			strLine.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe(uuidTextBox.Text));
			strLine.Append("\r\n");

			strLine.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe("Bios Serial Number"));
			strLine.Append(",");
			strLine.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe(biosTextBox.Text));
			strLine.Append("\r\n");

			return strLine.ToString();
		}

		private string MakeFileNameSafe(string _input)
		{
			return _input != null ? _input.Replace('\\', '-').Replace('/', '-').Replace(':', '_').Replace('*', '_').Replace('?', '_').Replace('\"', '_').Replace('<', '_').Replace('>', '_').Replace('|', '_') : "";
		}

		//Sample Code to read in csv file.
		private void importButton_Click(object sender, EventArgs e)
		{
			System.Diagnostics.Trace.WriteLine("Imported Text");
			OpenFileDialog fDialog = new OpenFileDialog();
			fDialog.Filter = "CSV files|*.csv";
			if (fDialog.ShowDialog() == DialogResult.OK)
			{
				Solimar.Licensing.Common.ValidationTokenReaderHelper valTokenReader = new Solimar.Licensing.Common.ValidationTokenReaderHelper(fDialog.OpenFile());
				StringBuilder tmpTextSBuilder = new StringBuilder();
				foreach (string key in valTokenReader.tokenMap.Keys)
				{
					tmpTextSBuilder.Append(key);
					tmpTextSBuilder.Append(" - ");
					tmpTextSBuilder.Append(valTokenReader.tokenMap[key]);
					System.Diagnostics.Trace.WriteLine(tmpTextSBuilder.ToString());
				}
			}
		}

		private void companyTextBox_TextChanged(object sender, EventArgs e)
		{
			m_vData.CompanyName = companyTextBox.Text;
		}

		private void Form1_Load(object sender, EventArgs e)
		{
			Initialize();
		}
		private void Form1_Shown(object sender, EventArgs e)
		{
			ChangeTabPage(0);
		}

		private void idLicenseSvrButton_Click(object sender, EventArgs e)
		{
			ChangeTabPage((string.Compare(idLicenseSvrTextBox.Text, FindComputerName(), true) != 0) ? 2 : 3);
		}
		private void warningButton_Click(object sender, EventArgs e)
		{
			ChangeTabPage(1);
		}

		private Control m_activeControl = null;
		private int m_activeControlsTabPageIdx = -1;
		private void ChangeTabPage(int _newTabPageIdx)
		{
			if (_newTabPageIdx != m_activeControlsTabPageIdx && _newTabPageIdx >= 0 && _newTabPageIdx < mainTabControl.TabPages.Count)
			{
				//Reattach old page
				if (m_activeControl != null)
					mainTabControl.TabPages[m_activeControlsTabPageIdx].Controls.Add(m_activeControl);

				m_activeControl = mainTabControl.TabPages[_newTabPageIdx].Controls[0];
				m_activeControlsTabPageIdx = _newTabPageIdx;
				mainBackPanel.Controls.Add(m_activeControl);
			}
			if (_newTabPageIdx == 0)
			{
				ActiveControl = warningRichTextBox;
				this.AcceptButton = warningButton;
			}
			else if (_newTabPageIdx == 1)
			{
				ActiveControl = idLicenseSvrTextBox;
				idLicenseSvrTextBox.Focus();
				this.AcceptButton = idLicenseSvrButton;
			}
			else if (_newTabPageIdx == 2)
			{
				wrongLicenseSvrLabel.Text = string.Format("The computer name you entered for the Solimar License Server (\"{0}\") is not this computer (\"{1}\").\r\n\r\nYou must run the Solimar Validation Token Retriever on the computer that will host the Solimar License Server.", idLicenseSvrTextBox.Text, FindComputerName());
				this.AcceptButton = closeButton;
			}
			else if (_newTabPageIdx == 3)
			{
				exportToolStripMenuItem.Visible = true;
				this.AcceptButton = genButton;
			}
		}

		private void idLicenseSvrTextBox_TextChanged(object sender, EventArgs e)
		{
			idLicenseSvrButton.Enabled = idLicenseSvrTextBox.TextLength > 0;
		}

		private void idLicenseSvrTextBox_KeyDown(object sender, KeyEventArgs e)
		{
			if ((e.KeyCode == Keys.Enter || e.KeyCode == Keys.Return) && idLicenseSvrButton.Enabled)
				idLicenseSvrButton_Click(idLicenseSvrButton, null);
		}

		private void warningRichTextBox_LinkClicked(object sender, LinkClickedEventArgs e)
		{
			System.Diagnostics.Process.Start(e.LinkText);
		}
	}


}