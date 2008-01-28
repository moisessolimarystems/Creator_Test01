using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using Solimar.Licensing.LicenseManagerWrapper;

namespace LicenseMixedModeTesterUI
{
	public partial class Form1 : Form
	{
		public delegate void Delegate1(IntPtr pContext, string keyIdent, uint messageType, int hrError, ValueType pvtTimestamp, string message);

		public Form1()
		{
			InitializeComponent();
			//SolimarLicenseManagerDotNet.Class1
			//SolimarLicenseManagerDotNet.SolimarLicenseWrapper a = new SolimarLicenseManagerDotNet.SolimarLicenseWrapper();
			//a.Connect();
			//SolimarLicenseWrapper b = new SolimarLicenseWrapper();
			//b.Connect();
			//SolimarLicenseManagerDotNet.
			//SolimarLicenseManagerDotNet.

			int productID = 12;
			productID = 14;
			productID = 10;
			//productID = 55;
			bool bExists = false, bSuccess;
			slw = new Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseWrapper();
			String licenseServer = "jlan5";
			licenseServer = "fakeServer";
			licenseServer = "jzak2";

			SolimarLicenseWrapper.DelegateMessageCallbackManaged delMessage;
			delMessage = new SolimarLicenseWrapper.DelegateMessageCallbackManaged(MessageCallback);

			bSuccess = slw.RegisterMessageCallback(this.Handle, delMessage);
			if (bSuccess)
				WriteMessage("Successfully called RegisterMessageCallback()");
			else
				WriteMessage("Failed to call RegisterMessageCallback()");

			SolimarLicenseWrapper.DelegateInvalidLicenseCallbackManaged delInvalidLicense;
			delInvalidLicense = new SolimarLicenseWrapper.DelegateInvalidLicenseCallbackManaged(InvalidLicenseCallback);
			bSuccess = slw.RegisterInvalidLicenseCallback(this.Handle, delInvalidLicense);
			if (bSuccess)
				WriteMessage("Successfully called RegisterInvalidLicenseCallback()");
			else
				WriteMessage("Failed to call RegisterInvalidLicenseCallback()");


			/*
			bSuccess = TestFunctions(slw, licenseServer, productID);
			if (bSuccess)
				WriteMessage("Successfully called TestFunctions()");
			else
				WriteMessage("Failed to call TestFunctions()");
			//*/


			///*
			TestHrFunctions(slw, licenseServer, productID);
			//if (hrResult == 0)
			//    WriteMessage("Successfully called TestHrFunctions()");
			//else
			//    WriteMessage("Failed to call TestHrFunctions() " + hrResult.ToString());
			//*/
			
			// get a list of the members
			//System.Reflection.MemberInfo[] members = this.GetType().GetMembers(System.Reflection.BindingFlags.Instance | System.Reflection.BindingFlags.Public);
			InitializeFunctionTree(slw);

			//bSuccess = slw.UnregisterMessageCallback();
			//if (bSuccess)
			//    WriteMessage("UnregisterMessageCallback() " + bSuccess.ToString());


		}

		bool TestFunctions(Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseWrapper sLicWrapper, string licenseServer, int productID)
		{
			bool bSuccess = false;
			for (; ; )
			{
				//bSuccess = slw.Connect(licenseServer);
				////bSuccess = slw.ConnectByProduct(productID, false);
				//if (bSuccess)
				//    WriteMessage("Successfully connected to " + licenseServer);
				//else
				//    WriteMessage("Failed to connect to " + licenseServer);

				bSuccess = sLicWrapper.Connect(licenseServer);
				if (bSuccess)
					WriteMessage("Successfully connected to " + licenseServer);
				else
					WriteMessage("Failed to connect to " + licenseServer);


				bSuccess = sLicWrapper.Initialize(productID, 1, 0, false, "", true, SolimarLicenseWrapper.LICENSE_LEVEL.UI_LEVEL_ALL | SolimarLicenseWrapper.LICENSE_LEVEL.UI_STYLE_EVENT_LOG | SolimarLicenseWrapper.LICENSE_LEVEL.UI_STYLE_DIALOG, 0);
				
				if (bSuccess)
					WriteMessage("Successfully Initialized");
				else
					WriteMessage("Failed to Initialize");


				//SolimarLicenseWrapper.LicenseMessageDelegateProc.CreateDelegate(
				//sLicWrapper.RegisterMessageCallback(
				bool bExists = false;
				bSuccess = sLicWrapper.KeyProductExists(productID, 1, 0, ref bExists);

				if (bSuccess)
					WriteMessage("KeyProductExists(): " + bExists.ToString());
				else
					WriteMessage("Failed to call KeyProductExists()");

				int licCount = 0;
				bSuccess = sLicWrapper.ModuleLicenseInUse(1, ref licCount);

				int verMajor = 0, verMinor = 0, verBuild = 0;
				bSuccess = sLicWrapper.GetVersionLicenseManager(ref verMajor, ref verMinor, ref verBuild);
				if (bSuccess)
					WriteMessage("GetVersionLicenseManager(): " + verMajor.ToString() + "." + verMinor.ToString() + "." + verBuild.ToString());
				else
					WriteMessage("Failed to call GetVersionLicenseManager()");

				bSuccess = sLicWrapper.GetVersionLicenseServer(licenseServer, ref verMajor, ref verMinor, ref verBuild);
				if (bSuccess)
					WriteMessage("GetVersionLicenseServer(): " + verMajor.ToString() + "." + verMinor.ToString() + "." + verBuild.ToString());
				else
					WriteMessage("Failed to call GetVersionLicenseManager()");

				int modID = 202;
				int obtainAmount = 1;
				bSuccess = sLicWrapper.ModuleLicenseObtain(modID, obtainAmount);
				if (bSuccess)
					WriteMessage("Successfully called ModuleLicenseObtain(" + modID.ToString() + ", " + obtainAmount.ToString() + ")");
				else
					WriteMessage("Failed to call ModuleLicenseObtain()");

				bSuccess = sLicWrapper.ModuleLicenseTotal(modID, ref obtainAmount);
				if (bSuccess)
					WriteMessage("Successfully called ModuleLicenseTotal(" + modID.ToString() + ") " + obtainAmount.ToString());
				else
					WriteMessage("Failed to call ModuleLicenseTotal()");


				bSuccess = sLicWrapper.ModuleLicenseInUse(modID, ref obtainAmount);
				if (bSuccess)
					WriteMessage("Successfully called ModuleLicenseInUse(" + modID.ToString() + ") " + obtainAmount.ToString());
				else
					WriteMessage("Failed to call ModuleLicenseInUse()");

				bool bValid = false;
				bSuccess = sLicWrapper.ValidateLicense();
				if (!bSuccess)
				{
					WriteMessage("Failed to call ValidateLicense()");
					break;
				}
				WriteMessage("Successfully called ValidateLicense() " + bValid.ToString());

				bSuccess = sLicWrapper.ModuleLicenseRelease(modID, obtainAmount);
				if (!bSuccess)
				{
					WriteMessage("Failed to call ModuleLicenseRelease()");
					break;
				}
				WriteMessage("Successfully called ModuleLicenseRelease(" + modID.ToString() + ", " + obtainAmount.ToString() + ")");

				break;
			}
			return bSuccess;
		}

		void TestHrFunctions(Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseWrapper sLicWrapper, string licenseServer, int productID)
		{
			try
			{
				sLicWrapper.ConnectEx(licenseServer);
				WriteMessage("Successfully called ConnectEx(" + licenseServer + ")");

				//hrResult = sLicWrapper.InitializeEx(productID, 1, 0, false, "", true, SolimarLicenseWrapper.LICENSE_LEVEL.UI_LEVEL_ALL | SolimarLicenseWrapper.LICENSE_LEVEL.UI_STYLE_EVENT_LOG | SolimarLicenseWrapper.LICENSE_LEVEL.UI_STYLE_DIALOG, 0);
				sLicWrapper.InitializeEx("AppInstance", productID, 1, 0, false, "", false,
					SolimarLicenseWrapper.LICENSE_LEVEL.UI_LEVEL_ALL, 0, true, false);
				WriteMessage("Successfully called InitializeEx(" + productID.ToString() + ")");

				bool bExists = false;
				bExists = sLicWrapper.KeyProductExistsEx(productID, 1, 0);
				WriteMessage("Successfully called KeyProductExistsEx(" + productID.ToString() + ") " + bExists.ToString());


				int verMajor = 0, verMinor = 0, verBuild = 0;
				sLicWrapper.GetVersionLicenseManagerEx(ref verMajor, ref verMinor, ref verBuild);
				WriteMessage("Successfully called GetVersionLicenseManagerEx() " + verMajor.ToString() + "." + verMinor.ToString() + "." + verBuild.ToString());

				sLicWrapper.GetVersionLicenseServerEx(licenseServer, ref verMajor, ref verMinor, ref verBuild);
				WriteMessage("Successfully called GetVersionLicenseServerEx(" + licenseServer + ") " + verMajor.ToString() + "." + verMinor.ToString() + "." + verBuild.ToString());

				int modID = 1;
				int obtainAmount = 1;
				int sessionID = 0;

				sessionID = sLicWrapper.StartLicensingSessionEx();
				WriteMessage("Successfully called StartLicensingSessionEx() - sessionID = " + sessionID.ToString());

				sessionID = sLicWrapper.StartLicensingSessionEx();
				WriteMessage("Successfully called StartLicensingSessionEx() - sessionID = " + sessionID.ToString());

				sLicWrapper.ModuleLicenseObtainLicensingSessionEx(sessionID, modID, obtainAmount);
				WriteMessage("Successfully called ModuleLicenseObtainLicensingSessionEx()");


				obtainAmount = sLicWrapper.ModuleLicenseInUseLicensingSessionEx(sessionID, modID);
				WriteMessage("Successfully called ModuleLicenseInUseLicensingSessionEx() - obtainAmount = " + obtainAmount.ToString());

				sLicWrapper.EndLicensingSessionEx(sessionID);
				WriteMessage("Successfully called EndLicensingSessionEx()");

				sessionID = sLicWrapper.StartLicensingSessionEx();
				WriteMessage("Successfully called StartLicensingSessionEx() - sessionID = " + sessionID.ToString());

				obtainAmount = sLicWrapper.ModuleLicenseInUseLicensingSessionEx(sessionID, modID);
				WriteMessage("Successfully called ModuleLicenseInUseLicensingSessionEx() - obtainAmount = " + obtainAmount.ToString());

				//hrResult = sLicWrapper.ModuleLicenseObtainEx(modID, obtainAmount);
				//if (hrResult != 0)
				//{
				//    WriteMessage("Failed to called ModuleLicenseObtainEx() hrResult = " + hrResult);
				//    break;
				//}
				//WriteMessage("Successfully called ModuleLicenseObtainEx(" + modID.ToString() + ", " + obtainAmount.ToString() + ")");

				//hrResult = sLicWrapper.ModuleLicenseTotalEx(modID, ref obtainAmount);
				//if (hrResult != 0)
				//{
				//    WriteMessage("Failed to called ModuleLicenseTotalEx() hrResult = " + hrResult);
				//    break;
				//}
				//WriteMessage("Successfully called ModuleLicenseTotalEx(" + modID.ToString() + ") " + obtainAmount.ToString());

				//hrResult = sLicWrapper.ModuleLicenseInUseEx(modID, ref obtainAmount);
				//if (hrResult != 0)
				//{
				//    WriteMessage("Failed to called ModuleLicenseInUseEx() hrResult = " + hrResult);
				//    break;
				//}
				//WriteMessage("Successfully called ModuleLicenseInUseEx(" + modID.ToString() + ") " + obtainAmount.ToString());

				bool bValid = false;
				bValid = sLicWrapper.ValidateLicenseEx();
				WriteMessage("Successfully called ValidateLicenseEx() " + bValid.ToString());


				//hrResult = sLicWrapper.ModuleLicenseReleaseEx(modID, obtainAmount);
				//if (hrResult != 0)
				//{
				//    WriteMessage("Failed to called ModuleLicenseReleaseEx() hrResult = " + hrResult);
				//    break;
				//}
				//WriteMessage("Successfully called ModuleLicenseReleaseEx(" + modID.ToString() + ") " + bValid.ToString());
			}
			catch (System.Runtime.InteropServices.COMException comEX)
			{
				WriteMessage(comEX.Message);
			}
			catch (Exception ex)
			{
				WriteMessage(ex.Message);
			}

			return ;
		}

		void WriteMessage(String message)
		{
			lock (messageListView)
			{
				messageListView.Items.Add(message);
			}
		}


		public void MessageCallback(IntPtr pContext, String keyIdent, uint messageType, int hrError, ValueType pvtTimestamp, String message)
		{
			SolimarLicenseWrapper.DelegateMessageCallbackManaged delMessageFunc;
			delMessageFunc = new SolimarLicenseWrapper.DelegateMessageCallbackManaged(UIMessageCallback);

			StringBuilder sBuilder = new StringBuilder();
			sBuilder.Append(pvtTimestamp != null ? pvtTimestamp.ToString() : DateTime.Now.ToString());
			sBuilder.Append(", ");
			sBuilder.Append(keyIdent);
			sBuilder.Append(", ");
			sBuilder.Append(message);
			sBuilder.Append(", ");
			sBuilder.Append(String.Format("0x{0:X8}", hrError));
			System.Diagnostics.Trace.WriteLine(sBuilder.ToString());

			//Invoking this here causes a deadlock
			//this.Invoke(delMessageFunc, new object[] { pContext, keyIdent, messageType, hrError, pvtTimestamp, message });
		}

		public void UIMessageCallback(IntPtr pContext, String keyIdent, uint messageType, int hrError, ValueType pvtTimestamp, String message)
		{
			lock (callbackListView)
			{
				callbackListView.BeginUpdate();

				ListViewItem lvi = new ListViewItem();
				lvi.Text = pvtTimestamp != null ? pvtTimestamp.ToString() : DateTime.Now.ToString();
				lvi.SubItems.Add(messageType.ToString());
				lvi.SubItems.Add(keyIdent);
				lvi.SubItems.Add(message);
				lvi.SubItems.Add(String.Format("0x{0:X8}", hrError));
				callbackListView.Items.Add(lvi);

				callbackListView.EndUpdate();

				
			}

		}

		public void InvalidLicenseCallback(IntPtr pContext, int hr, String message)
		{
			SolimarLicenseWrapper.DelegateInvalidLicenseCallbackManaged delFunc;
			delFunc = new SolimarLicenseWrapper.DelegateInvalidLicenseCallbackManaged(UIInvalidLicenseCallback);
			this.Invoke(delFunc, new object[] { pContext, hr, message });
		}
		public void UIInvalidLicenseCallback(IntPtr pContext, int hr, String message)
		{
			lock (callbackListView)
			{
				callbackListView.BeginUpdate();

				ListViewItem lvi = new ListViewItem();
				lvi.Text = DateTime.Now.ToString();
				lvi.SubItems.Add("");
				lvi.SubItems.Add("");
				lvi.SubItems.Add(message);
				lvi.SubItems.Add(String.Format("0x{0:X8}", hr));
				callbackListView.Items.Add(lvi);

				callbackListView.EndUpdate();
			}
		}



		void InitializeFunctionTree(Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseWrapper paramLicWrapper)
		{
			System.Reflection.MethodInfo[] methodList = paramLicWrapper.GetType().GetMethods(System.Reflection.BindingFlags.Instance | System.Reflection.BindingFlags.Public);
			TreeNode rootNode = new TreeNode(paramLicWrapper.GetType().ToString());
			functionTreeView.Nodes.Add(rootNode);
			TreeNode funcNode = null;
			
			foreach (System.Reflection.MethodInfo meInfo in methodList)
			{
				funcNode = new TreeNode(meInfo.ToString());
				rootNode.Nodes.Add(funcNode);
			}
		}
		private void functionTreeView_Click(object sender, EventArgs e)
		{
			TreeNode selNode = functionTreeView.SelectedNode;
			if (selNode.Index != 0)
			{
				System.Reflection.MethodInfo[] methodList = slw.GetType().GetMethods(System.Reflection.BindingFlags.Instance | System.Reflection.BindingFlags.Public);
				methodPropertyGrid.SelectedObject = methodList[selNode.Index-1].GetParameters();
				methodTestPanel1.TestMethod(this.Handle, methodList[selNode.Index - 1], null); 

				/*
				System.Reflection.MethodInfo[] methodList = slw.GetType().GetMethods(System.Reflection.BindingFlags.Instance | System.Reflection.BindingFlags.Public);
				//selNode.Index
				methodPropertyGrid.SelectedObject = methodList[selNode.Index-1].GetParameters();
				*/
			}
			//
			//
		}


		private Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseWrapper slw;

		private void Form1_FormClosing(object sender, FormClosingEventArgs e)
		{
			slw.Dispose();
			slw = null;
		}


		
	}
}