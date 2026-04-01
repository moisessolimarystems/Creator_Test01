using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace LicensingTest_SoftwareLicense
{
	public partial class LicensingTesterToolbar : ToolStrip
	{
		public LicensingTesterToolbar()
		{
			InitializeComponent();
			Initialize();
		}

		private ToolStripLabel tProdLabel;
		private ToolStripLabel tAppInstLabel;
		private ToolStripSeparator tSpace1Sep;
		private ToolStripSeparator tSpace2Sep;
		private ToolStripTextBox tModIdTextbox;
		private ToolStripTextBox tModAmountTextbox;
		private ToolStripDropDownButton tActionDropdown;
		private ToolStripMenuItem tActObtainMenuItem;
		private ToolStripMenuItem tActReleaseMenuItem;
		private ToolStripMenuItem tActInuseMenuItem;
		private ToolStripMenuItem tActTotalMenuItem;
		private ToolStripButton tValidateButton;
		private ToolStripButton tDisconnectButton;
		private void Initialize()
		{
			tProdLabel = new ToolStripLabel("Product");
			//tProdLabel.Font.Style |= FontStyle.Bold;
			this.Items.Add(tProdLabel);
			tAppInstLabel = new ToolStripLabel("AppInstance");
			//tAppInstLabel.Font.Bold = true;
			this.Items.Add(tAppInstLabel);
			tSpace1Sep = new ToolStripSeparator();
			this.Items.Add(tSpace1Sep);

			tModIdTextbox = new ToolStripTextBox();
			tModIdTextbox.TextBox.Text = "28";
			tModIdTextbox.ToolTipText = "Module ID for Action";
			this.Items.Add(tModIdTextbox);
			tModAmountTextbox = new ToolStripTextBox();
			tModAmountTextbox.TextBox.Text = "1";
			tModAmountTextbox.ToolTipText = "Amount for Action";
			this.Items.Add(tModAmountTextbox);
			tActionDropdown = new ToolStripDropDownButton();
			tActionDropdown.Text = "Actions";
			tActObtainMenuItem = new ToolStripMenuItem();
			tActObtainMenuItem.Text = "Obtain";
			tActObtainMenuItem.Click += new EventHandler(tActObtainMenuItem_Click);
			tActionDropdown.DropDownItems.Add(tActObtainMenuItem);
			tActReleaseMenuItem = new ToolStripMenuItem();
			tActReleaseMenuItem.Text = "Release";
			tActReleaseMenuItem.Click += new EventHandler(tActReleaseMenuItem_Click);
			tActionDropdown.DropDownItems.Add(tActReleaseMenuItem);
			tActInuseMenuItem = new ToolStripMenuItem();
			tActInuseMenuItem.Text = "In Use";
			tActInuseMenuItem.Click += new EventHandler(tActInuseMenuItem_Click);
			tActionDropdown.DropDownItems.Add(tActInuseMenuItem);
			tActTotalMenuItem = new ToolStripMenuItem();
			tActTotalMenuItem.Text = "Total";
			tActTotalMenuItem.Click += new EventHandler(tActTotalMenuItem_Click);
			tActionDropdown.DropDownItems.Add(tActTotalMenuItem);
			this.Items.Add(tActionDropdown);
			//private ToolStripMenuItem tActObtainMenuItem;
			//private ToolStripMenuItem tActReleaseMenuItem;
			//private ToolStripMenuItem tActInuseMenuItem;
			//private ToolStripMenuItem tActTotalMenuItem;
			//tActionDropdown.

			tSpace2Sep = new ToolStripSeparator();
			this.Items.Add(tSpace2Sep);
			tValidateButton = new ToolStripButton("Validate");
			tValidateButton.Click += new EventHandler(tValidateButton_Click);
			this.Items.Add(tValidateButton);
			tDisconnectButton = new ToolStripButton("Disconnect");
			tDisconnectButton.Click += new EventHandler(tDisconnectButton_Click);
			this.Items.Add(tDisconnectButton);

		}

		public delegate void DelegateParamStringReturnsVoid(string param1);
		public DelegateParamStringReturnsVoid delWriteMessage = null;


		void tActTotalMenuItem_Click(object sender, EventArgs e)
		{
			try
			{
				int modID = System.Convert.ToInt32(tModIdTextbox.TextBox.Text);
				int licenseCount = 0;
				licenseCount = m_licWrapper.ModuleLicenseTotalEx(modID);
				LogMessage("Success m_licWrapper.ModuleLicenseTotalEx(" + modID.ToString() + ") licenseInUse = " + licenseCount.ToString());
			}
			catch (COMException ex)
			{
				LogMessage(ex.Message);
			}
			//throw new Exception("The method or operation is not implemented.");
		}

		void tActInuseMenuItem_Click(object sender, EventArgs e)
		{
			try
			{
				int modID = System.Convert.ToInt32(tModIdTextbox.TextBox.Text);
				int licenseCount = 0;
				licenseCount = m_licWrapper.ModuleLicenseInUseEx(modID);
				LogMessage("Success m_licWrapper.ModuleLicenseInUseEx(" + modID.ToString() + ") licenseInUse = " + licenseCount.ToString());
			}
			catch (COMException ex)
			{
				LogMessage(ex.Message);
			}
		}

		void tActReleaseMenuItem_Click(object sender, EventArgs e)
		{
			try
			{
				int modID = System.Convert.ToInt32(tModIdTextbox.TextBox.Text);
				int amount = System.Convert.ToInt32(tModAmountTextbox.TextBox.Text);
				m_licWrapper.ModuleLicenseReleaseEx(modID, amount);
				
				LogMessage("Success m_licWrapper.ModuleLicenseReleaseEx(" + modID.ToString() + ", " + amount.ToString() + ")");
			}
			catch (COMException ex)
			{
				LogMessage(ex.Message);
			}
			//throw new Exception("The method or operation is not implemented.");
		}

		void tActObtainMenuItem_Click(object sender, EventArgs e)
		{
			try
			{
				int modID = System.Convert.ToInt32(tModIdTextbox.TextBox.Text);
				int amount = System.Convert.ToInt32(tModAmountTextbox.TextBox.Text);
				m_licWrapper.ModuleLicenseObtainEx(modID, amount);
				LogMessage("Success m_licWrapper.ModuleLicenseObtainEx(" + modID.ToString() + ", " + amount.ToString() + ")");
			}
			catch (COMException ex)
			{
				LogMessage(ex.Message);
			}
		}

		void tDisconnectButton_Click(object sender, EventArgs e)
		{
			LogMessage("Destroy");
			//m_licWrapper.Dispose();
			m_licWrapper.DisconnectEx();
			m_licWrapper.Dispose();
			this.Dispose();
			//throw new Exception("The method or operation is not implemented.");
		}



		void tValidateButton_Click(object sender, EventArgs e)
		{
			try
			{
				bool bValid = false;
				bValid = m_licWrapper.ValidateLicenseEx();
				if (bValid)
					LogMessage("Success m_licWrapper.ValidateLicenseEx()");
				if (bValid == false)
					LogMessage("Success m_licWrapper.ValidateLicenseEx(), but licensing is invalid");
			}
			catch (COMException ex)
			{
				LogMessage(ex.Message);
			}
			//throw new Exception("The method or operation is not implemented.");
		}

		private long m_productID;
		private string m_appInstance;
		private Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseWrapper m_licWrapper;
		public void SetData(long _productID, string _appInstance, Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseWrapper _licWrapper)
		{
			m_productID = _productID;
			m_appInstance = _appInstance;
			m_licWrapper = _licWrapper;

			tProdLabel.Text = "ProductID: " + m_productID.ToString();
			tAppInstLabel.Text = "AppInstance: " + m_appInstance.ToString(); ;
		}

		public void LogMessage(string message)
		{
			if (delWriteMessage != null)
				delWriteMessage(m_appInstance + " - " + m_productID.ToString() + " - " + message);
				
		}
	}
}
