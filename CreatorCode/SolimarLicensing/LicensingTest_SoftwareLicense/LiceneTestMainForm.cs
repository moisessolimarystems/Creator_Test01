using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace LicensingTest_SoftwareLicense
{
	public partial class Form1 : Form
	{
		public Form1()
		{
			InitializeComponent();
			Initialize();
		}



		//Solimar.Performance.PerformanceHelper globalPerfHelper = new Solimar.Performance.PerformanceHelper();
		//System.Diagnostics.PerformanceCounter globalPerAverageCall = new System.Diagnostics.PerformanceCounter(
		//        Solimar.Performance.PerfLicenseCategory.name,
		//        Solimar.Performance.AverageTimePerCallCounter.name,
		//        Application.ProductName,
		//        false);
		//System.Diagnostics.PerformanceCounter globalPerAverageCallBase = new System.Diagnostics.PerformanceCounter(
		//        Solimar.Performance.PerfLicenseCategory.name,
		//        Solimar.Performance.AverageTimePerCallBaseCounter.name,
		//        Application.ProductName,
		//        false);
		public void Initialize()
		{
			InitializeControl();
			//globalPerAverageCall.RawValue = 0;
			//globalPerAverageCallBase.RawValue = 0;
			//System.Diagnostics.PerformanceCounter licCounter = new System.Diagnostics.PerformanceCounter(
			//    Solimar.Performance.PerfLicenseCategory.name,
			//    Solimar.Performance.MessagesInQueueCounter.name,
			//    Application.ProductName,
			//    false);

//			Solimar.Performance.PerformanceCreator pC = new Solimar.Performance.PerformanceCreator();

			//System.Diagnostics.Stopwatch.GetTimestamp()
			long startTime, endTime;
			startTime = System.Diagnostics.Stopwatch.GetTimestamp();
			LogMessage("Solimar.Licensing.GlobalSoftwareSpec globalSwSpec = new Solimar.Licensing.GlobalSoftwareSpec() - Start");
			Solimar.Licensing.GlobalSoftwareSpec globalSwSpec = new Solimar.Licensing.GlobalSoftwareSpec();
			endTime = System.Diagnostics.Stopwatch.GetTimestamp();

			//globalPerAverageCall.IncrementBy(endTime-startTime);
			//globalPerAverageCallBase.Increment();

			LogMessage("Solimar.Licensing.GlobalSoftwareSpec globalSwSpec = new Solimar.Licensing.GlobalSoftwareSpec() - End");
			g_softwareSpec = globalSwSpec.softwareSpec;
			LogMessage("g_softwareSpec = globalSwSpec.softwareSpec; - End");
			//LogMessage(computerI);
			//Win32_ComputerSystemProduct.UUID
		}

		private void InitializeControl()
		{
			tsgen_createSplitButton_DropDownItemClicked(tsgen_createSplitButton, new ToolStripItemClickedEventArgs(createToolStripMenuItem));
		}



		public void LogMessage(COMException ex)
		{
			//string hrError = Solimar.Licensing.CSolLicHRESULT.GetHRMessage((uint)ex.ErrorCode);
			//LogMessage(message + hrError);
			LogMessage(ex.Message);
		}
		private void LogMessage_ThreadSafe(string message)
		{
			System.Diagnostics.Trace.WriteLine(message);
			messageListView.Items.Add(new ListViewItem(DateTime.Now.ToString() + ": " + message));
			messageListView.Items[messageListView.Items.Count - 1].EnsureVisible();
		}
		private delegate void DelegateStringParamReturnsVoid(string _param1);

		public void LogMessage(string message)
		{

			if (this.InvokeRequired)
			{
				DelegateStringParamReturnsVoid threadSafeCall = new DelegateStringParamReturnsVoid(LogMessage_ThreadSafe);
				this.Invoke(threadSafeCall, new object[] { message });
			}
			else
			{
				LogMessage_ThreadSafe(message);
			}

			//this.Invoke(LogMessage_ThreadSafe, new object[] { message });
		}
		public void LogMessage(string message, long hrValue)
		{
			//String.Format("0x{0:X8}", hrValue)
			//string hrError = ": hr = " + String.Format("0x{0:X8}", (int)hrValue) + " - " + Solimar.Licensing.CSolLicHRESULT.GetHRMessage((uint)hrValue);
			//LogMessage(message + hrError);

			LogMessage(message);
		}

		private void exitToolStripMenuItem_Click(object sender, EventArgs e)
		{
			this.Close();
		}

		//private SolimarLicenseManagerDotNet.SolimarLicenseServerWrapper.
		private Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_SoftwareSpecAttribs g_softwareSpec;
		private Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs g_licInfoAttrib;
		private Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseServerWrapper g_licServer = null;
		private Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseWrapper g_licenseWrapper = null;
		

		void SetRawKeySpec(string rawKeySpec)
		{
			rawKeySpecTextBox.Text = rawKeySpec;
		}
		void SetKeySpec(Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_SoftwareSpecAttribs keySpec)
		{
			TreeNode rootNode = new TreeNode("Root");
			Tree_Add_Lic_SoftwareSpecAttribs(ref rootNode, keySpec);
			keySpecTreeView.BeginUpdate();
			keySpecTreeView.Nodes.Clear();
			keySpecTreeView.Nodes.Add(rootNode);
			//keySpecTreeView.ExpandAll();
			rootNode.Expand();
			keySpecTreeView.EndUpdate();
		}

		void Tree_Add_Lic_SoftwareSpecAttribs(ref TreeNode param_refRootNode, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_SoftwareSpecAttribs param_prodSoftwareSpec)
		{
			TreeNode rootNode = new TreeNode("KeySpec");
			TreeNode productMapNode = new TreeNode("ProductMap");
			foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in param_prodSoftwareSpec.productSpecMap.TVal.Values)
				Tree_Add_Lic_ProductSoftwareSpecAttribs(ref productMapNode, productSpec);

			productMapNode.Expand();
			rootNode.Nodes.Add(productMapNode);
			rootNode.Expand();
			param_refRootNode.Nodes.Add(rootNode);
			
		}
		void Tree_Add_Lic_ProductSoftwareSpecAttribs(ref TreeNode param_refRootNode, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs param_prodSoftwareSpec)
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
			if(param_prodSoftwareSpec.sameModSpecProductID != 0)
				toolTipBuilder.Append("\r\n" + nodeText);

			nodeText = "PrevSharedProductID: " + System.Convert.ToInt32(param_prodSoftwareSpec.prevSharedProductID.ToString(), 16).ToString();
			productNode.Nodes.Add(new TreeNode(nodeText));
			if (param_prodSoftwareSpec.prevSharedProductID != 0)
				toolTipBuilder.Append("\r\n" + nodeText);

			TreeNode modNode = new TreeNode("ModuleMap");
			
			Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.Lic_ModuleSoftwareSpecAttribsMap moduleSpecMap = Solimar.Licensing.Attribs.Lic_LicenseInfoAttribsHelper.GetModuleList(g_softwareSpec, param_prodSoftwareSpec.productID);
			if (moduleSpecMap != null)
			{
				foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs moduleSpec in moduleSpecMap.TVal.Values)
					Tree_Add_Lic_ModuleSoftwareSpecAttribs(ref modNode, moduleSpec);
			}

			productNode.Nodes.Add(modNode);
			productNode.ToolTipText = toolTipBuilder.ToString();

			param_refRootNode.Nodes.Add(productNode);

			
		}

		void Tree_Add_Lic_VerificationAttribs(ref TreeNode param_refRootNode, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_VerificationAttribs param_verificationAttribs)
		{
			TreeNode verificationNode = new TreeNode("VerificationNode");
			TreeNode verListNode = new TreeNode("ValidationList");
			foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs verToken in param_verificationAttribs.validationTokenList.TVal)
			{
				Tree_Add_Lic_ValidationTokenAttribs(ref verListNode, verToken);
			}

			verListNode.Expand();
			verificationNode.Nodes.Add(verListNode);

			TreeNode verHistListNode = new TreeNode("VerificationHistoryList");
			foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_VerificationCodeAttribs verCodeToken in param_verificationAttribs.verificationCodeHistoryList.TVal)
			{
				Tree_Add_Lic_VerificationCodeAttribs(ref verHistListNode, verCodeToken);
			}
			verHistListNode.Expand();
			verificationNode.Nodes.Add(verHistListNode);

			verificationNode.Expand();
			param_refRootNode.Nodes.Add(verificationNode);
		}
		void Tree_Add_Lic_ValidationTokenAttribs(ref TreeNode param_refRootNode, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs param_verificationTokenAttribs)
		{
			StringBuilder toolTipBuilder = new StringBuilder();
			string nodeText;
			bool bSuccess = false;
			TreeNode verificationTokenNode = new TreeNode(ValidateToken(param_verificationTokenAttribs, ref bSuccess));
			TreeNode childNode = null;

			if(!bSuccess)
				verificationTokenNode.ForeColor = System.Drawing.Color.Red;

			nodeText = "TokenType: " + param_verificationTokenAttribs.tokenType.EVal + " (" + param_verificationTokenAttribs.tokenType + ")";
			toolTipBuilder.Append(nodeText + "\r\n");
			childNode = new TreeNode(nodeText);
			childNode.ForeColor = verificationTokenNode.ForeColor;
			verificationTokenNode.Nodes.Add(childNode);

			nodeText = "TokenValue: " + param_verificationTokenAttribs.tokenValue;
			toolTipBuilder.Append(nodeText);
			childNode = new TreeNode(nodeText);
			childNode.ForeColor = verificationTokenNode.ForeColor;
			verificationTokenNode.Nodes.Add(childNode);

			param_refRootNode.Nodes.Add(verificationTokenNode);
			verificationTokenNode.ToolTipText = toolTipBuilder.ToString();

		}
		void Tree_Add_Lic_VerificationCodeAttribs(ref TreeNode param_refRootNode, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_VerificationCodeAttribs param_verificationCodeAttribs)
		{
			StringBuilder toolTipBuilder = new StringBuilder();
			toolTipBuilder.Append("Date: ");
			toolTipBuilder.Append(param_verificationCodeAttribs.verificationDate.TVal.ToLocalTime());
			toolTipBuilder.Append(", VerificationCode: ");
			toolTipBuilder.Append(param_verificationCodeAttribs.verificationValue.TVal);

			TreeNode verificationCodeTokenNode = new TreeNode(toolTipBuilder.ToString());
			param_refRootNode.Nodes.Add(verificationCodeTokenNode);
			verificationCodeTokenNode.ToolTipText = toolTipBuilder.ToString();

			//TreeNode verificationCodeNode = new TreeNode(ValidateToken(param_verificationTokenAttribs, ref bSuccess));
			//TreeNode childNode = null;

		}

		void Tree_Add_Lic_ModuleSoftwareSpecAttribs(ref TreeNode param_refRootNode, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs param_modInfoAttribs)
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

		void Tree_Add_Lic_LicenseInfoAttribs(ref TreeNode param_refRootNode, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs param_licInfoAttribs)
		{
			if (param_licInfoAttribs != null)
			{
				//param_refRootNode.Nodes.Add(new TreeNode("CustomerID: " + param_licInfoAttribs.customerID));
				param_refRootNode.Nodes.Add(new TreeNode(String.Format("CustomerID: {0:X4}", param_licInfoAttribs.customerID.TVal)));
				param_refRootNode.LastNode.ForeColor = param_refRootNode.ForeColor;
				param_refRootNode.Nodes.Add(new TreeNode(String.Format("DestinationID: {0:X3}", param_licInfoAttribs.destinationID.TVal)));
				param_refRootNode.LastNode.ForeColor = param_refRootNode.ForeColor;
				//param_refRootNode.Nodes.Add(new TreeNode("SoftwareID: " + param_licInfoAttribs.softwareGroupLicenseID));
				param_refRootNode.Nodes.Add(new TreeNode(String.Format("SoftwareID: {0:X4}", param_licInfoAttribs.softwareGroupLicenseID.TVal)));
				param_refRootNode.LastNode.ForeColor = param_refRootNode.ForeColor;
				param_refRootNode.Nodes.Add(new TreeNode("SoftwareLicenseType: " + param_licInfoAttribs.softwareLicType.GetAlias() + " (" + param_licInfoAttribs.softwareLicType.ToString() + ")"));
				param_refRootNode.LastNode.ForeColor = param_refRootNode.ForeColor;
				//if ((param_licInfoAttribs.softwareLicType == Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltDisasterRecovery)
				//    || (param_licInfoAttribs.softwareLicType == Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltTestDev))
				if(param_licInfoAttribs.activationTotal.TVal != 0 )
				{
					param_refRootNode.Nodes.Add(new TreeNode("Activations Total: " + param_licInfoAttribs.activationTotal.TVal));
					param_refRootNode.LastNode.ForeColor = param_refRootNode.ForeColor;
					param_refRootNode.Nodes.Add(new TreeNode("Activations Current: " + param_licInfoAttribs.activationCurrent.TVal));
					param_refRootNode.LastNode.ForeColor = param_refRootNode.ForeColor;
					param_refRootNode.Nodes.Add(new TreeNode("Activation Expiration Date: " + param_licInfoAttribs.activationExpirationDate.TVal.ToLocalTime()));
					param_refRootNode.LastNode.ForeColor = param_refRootNode.ForeColor;
					param_refRootNode.Nodes.Add(new TreeNode("Activation Amount in Days: " + param_licInfoAttribs.activationAmountInDays.TVal));
					param_refRootNode.LastNode.ForeColor = param_refRootNode.ForeColor;
				}
				param_refRootNode.Nodes.Add(new TreeNode("Modified Date: " + param_licInfoAttribs.modifiedDate.TVal.ToLocalTime()));
				param_refRootNode.LastNode.ForeColor = param_refRootNode.ForeColor;

				TreeNode prodNode = new TreeNode("ProductList");
				foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs prodInfo in param_licInfoAttribs.productList.TVal)
				{
					Tree_Add_Lic_ProductInfoAttribs(ref prodNode, prodInfo);
				}

				param_refRootNode.Nodes.Add(prodNode);

				prodNode.Expand();
				Tree_Add_Lic_VerificationAttribs(ref param_refRootNode, param_licInfoAttribs.licVerificationAttribs);
			}
		}

		void Tree_Add_Lic_ProductInfoAttribs(ref TreeNode param_refRootNode, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs param_prodInfoAttribs)
		{
			StringBuilder toolTipBuilder = new StringBuilder();
			string nodeText;


			nodeText = "Product: " + Solimar.Licensing.Attribs.Lic_LicenseInfoAttribsHelper.GetProductName(g_softwareSpec, param_prodInfoAttribs.productID.TVal) + " (" + System.Convert.ToInt32(param_prodInfoAttribs.productID.ToString(), 16).ToString() + ")";
			toolTipBuilder.Append("ProductName: " + Solimar.Licensing.Attribs.Lic_LicenseInfoAttribsHelper.GetProductName(g_softwareSpec, param_prodInfoAttribs.productID.TVal) + "\r\n");
			
			//nodeText = "Product " + System.Convert.ToInt32(param_prodInfoAttribs.productID.ToString(), 16).ToString();
			TreeNode prodNode = new TreeNode(nodeText);

			nodeText = "ProductID: " + System.Convert.ToInt32(param_prodInfoAttribs.productID.ToString(), 16).ToString();
			prodNode.Nodes.Add(new TreeNode(nodeText));
			toolTipBuilder.Append(nodeText + "\r\n");

			nodeText = "ProductAppInstance: " + System.Convert.ToInt32(param_prodInfoAttribs.productAppInstance.ToString(), 16).ToString();
			prodNode.Nodes.Add(new TreeNode(nodeText));
			toolTipBuilder.Append(nodeText + "\r\n");

			System.Text.StringBuilder strBuilderIntro = new StringBuilder();
			strBuilderIntro.Append("ProductVersion: ");
			strBuilderIntro.Append(param_prodInfoAttribs.product_Major.ToString());
			strBuilderIntro.Append(".");
			strBuilderIntro.Append(String.Format("{0:00}", System.Convert.ToInt32(param_prodInfoAttribs.product_Minor.ToString(), 16)));
			//strBuilderIntro.Append(".");
			//strBuilderIntro.Append(param_prodInfoAttribs.product_SubMajor.ToString());
			//strBuilderIntro.Append(".");
			//strBuilderIntro.Append(param_prodInfoAttribs.product_SubMinor.ToString());
			prodNode.Nodes.Add(new TreeNode(strBuilderIntro.ToString()));
			toolTipBuilder.Append(strBuilderIntro.ToString());

			TreeNode modNode = new TreeNode("ModuleList");
			foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs modInfo in param_prodInfoAttribs.moduleList.TVal)
				Tree_Add_Lic_ModuleInfoAttribs(ref modNode, modInfo, param_prodInfoAttribs.productID);
			prodNode.Nodes.Add(modNode);
			prodNode.ToolTipText = toolTipBuilder.ToString();
			//param_prodInfoAttribs
			//prodNode.Expand();
			param_refRootNode.Nodes.Add(prodNode);
		}

		void Tree_Add_Lic_ModuleInfoAttribs(ref TreeNode param_refRootNode, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs param_modInfoAttribs, uint param_prodID)
		{

			StringBuilder toolTipBuilder = new StringBuilder();
			string nodeText;

			nodeText = "Module: " + System.Convert.ToInt32(param_modInfoAttribs.moduleID.ToString(), 16).ToString() + " {" + Solimar.Licensing.Attribs.Lic_LicenseInfoAttribsHelper.GetModuleName(g_softwareSpec, param_prodID, param_modInfoAttribs.moduleID.TVal) + "}";
			toolTipBuilder.Append(nodeText);

			TreeNode modNode = new TreeNode();
			TreeNode childNode = null;
			if (DateTime.Compare(new DateTime(1900, 1, 1), param_modInfoAttribs.moduleExpirationDate.TVal) != 0)
			{
				nodeText += " [Addon Module Expires: " + param_modInfoAttribs.moduleExpirationDate.TVal.ToLocalTime().ToString() + "]";
				if (DateTime.Now.ToUniversalTime().CompareTo(param_modInfoAttribs.moduleExpirationDate.TVal) > 0)
				{
					modNode.ForeColor = System.Drawing.Color.Red;
				}
			}

			modNode.Text = nodeText;

			toolTipBuilder.Append("\r\nModuleName: " + Solimar.Licensing.Attribs.Lic_LicenseInfoAttribsHelper.GetModuleName(g_softwareSpec, param_prodID, param_modInfoAttribs.moduleID.TVal));

			nodeText = "ModuleID: " + System.Convert.ToInt32(param_modInfoAttribs.moduleID.ToString(), 16).ToString();
			toolTipBuilder.Append("\r\n" + nodeText);
			childNode = new TreeNode(nodeText);
			childNode.ForeColor = modNode.ForeColor;
			modNode.Nodes.Add(childNode);

			nodeText = "ModuleValue: " + System.Convert.ToInt32(param_modInfoAttribs.moduleValue.ToString(), 16).ToString();
			toolTipBuilder.Append("\r\n" + nodeText);
			childNode = new TreeNode(nodeText);
			childNode.ForeColor = modNode.ForeColor;
			modNode.Nodes.Add(childNode);

			nodeText = "ModuleAppInstance: " + System.Convert.ToInt32(param_modInfoAttribs.moduleAppInstance.ToString(), 16).ToString();
			toolTipBuilder.Append("\r\n" + nodeText);
			childNode = new TreeNode(nodeText);
			childNode.ForeColor = modNode.ForeColor;
			modNode.Nodes.Add(childNode);

			//nodeText = "ModuleAppInstance: " + System.Convert.ToInt32(param_modInfoAttribs.moduleState.ToString(), 16).ToString();
			//nodeText = "ModuleState: " + param_modInfoAttribs.moduleState.EVal.ToString();
			nodeText = "ModuleState: " + param_modInfoAttribs.moduleState.GetAlias();
			toolTipBuilder.Append("\r\n" + nodeText);
			childNode = new TreeNode(nodeText);
			childNode.ForeColor = modNode.ForeColor;
			modNode.Nodes.Add(childNode);

			nodeText = "ContractNumber: " + param_modInfoAttribs.contractNumber.TVal.ToString();
			toolTipBuilder.Append("\r\n" + nodeText);
			childNode = new TreeNode(nodeText);
			childNode.ForeColor = modNode.ForeColor;
			modNode.Nodes.Add(childNode);

			if (DateTime.Compare(new DateTime(1900, 1, 1), param_modInfoAttribs.moduleExpirationDate.TVal) != 0)
			{
				nodeText = "ModuleExpirationDate: " + param_modInfoAttribs.moduleExpirationDate.TVal.ToLocalTime().ToString();
				toolTipBuilder.Append("\r\n" + nodeText);
				childNode = new TreeNode(nodeText);
				childNode.ForeColor = modNode.ForeColor;
				modNode.Nodes.Add(childNode);
			}
			modNode.ToolTipText = toolTipBuilder.ToString();
			param_refRootNode.Nodes.Add(modNode);

		}

		string ValidateToken(Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs param_verificationTokenAttribs, ref bool bRefSuccess)
		{
			System.Text.StringBuilder retVal = new StringBuilder("ValidationToken: ");
			//string softwareLicense = String.Format("{0}-{1}", System.Convert.ToInt32(g_licInfoAttrib.customerID, 16), g_licInfoAttrib.softwareLicenseID);
			//string softwareLicense = String.Format("{0}-{1}", g_licInfoAttrib.customerID, g_licInfoAttrib.softwareGroupLicenseID);
			string softwareLicense = Solimar.Licensing.Attribs.Lic_LicenseInfoAttribsHelper.GetDisplayLabel(g_licInfoAttrib);
			
			retVal.Append(param_verificationTokenAttribs.tokenType.EVal.ToString());
			try
			{
				g_licServer.ValidateToken_ByLicense(softwareLicense, (int)param_verificationTokenAttribs.tokenType.TVal, param_verificationTokenAttribs.tokenValue);
				retVal.Append(" - Validated");
				bRefSuccess = true;
			}
			catch (COMException comEX)
			{
				retVal.Append(" - ");
				retVal.Append(comEX.Message);
				bRefSuccess = false;
			}
			//g_licInfoAttrib.
			//int hrValue = g_licServer.ValidateToken_ByLicense(softwareLicense, (int)param_verificationTokenAttribs.tokenType.TVal, param_verificationTokenAttribs.tokenValue);

			//retVal.Append(" - ");
			//if (hrValue == 0)
			//{
			//    retVal.Append("Validated");
			//}
			//else
			//{
			//    retVal.Append(String.Format("0x{0:X8} - {1}", hrValue, Solimar.Licensing.CSolLicHRESULT.GetHRMessage((uint)hrValue)));
			//}
			return retVal.ToString();
		}


		private void ts_connectButton_Click(object sender, EventArgs e)
		{
			try
			{
				Cursor.Current = Cursors.WaitCursor;
				LogMessage("ts_connectButton_Click - Enter");

				LogMessage("Create g_licServer");


				g_licServer = new Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseServerWrapper();

				//long startTime, endTime;
				//startTime = System.Diagnostics.Stopwatch.GetTimestamp();
				g_licServer.Connect(ts_connectTextBox.Text);
				//endTime = System.Diagnostics.Stopwatch.GetTimestamp();
				//globalPerAverageCall.IncrementBy(endTime - startTime);
				//globalPerAverageCallBase.Increment();
				//LogMessage("globalPerAverageCall: " + globalPerAverageCall.RawValue.ToString());
				//LogMessage("globalPerAverageCallBase: " + globalPerAverageCallBase.RawValue.ToString());
				//LogMessage("Called g_licServer.Initialize() - Server: " + ts_connectTextBox.Text);

				SetKeySpec(g_softwareSpec);

				LogMessage("KeySpec Size: " + g_softwareSpec.ToString().Length);
				SetRawKeySpec(g_softwareSpec);
				//String sofwareSpec = "";
				////LogMessage("g_licServer.GetSoftwareSpec() - Start");
				////hrValue = g_licServer.GetSoftwareSpec(ref sofwareSpec);
				////LogMessage("g_licServer.GetSoftwareSpec() - End");
				//if (hrValue == 0)
				//{
				//    //g_softwareSpec = new Solimar.SPD.Attribs.Lic_PackageAttribs.Lic_SoftwareSpecAttribs();
				//    //g_softwareSpec.AssignMembersFromStream(sofwareSpec);
				//    //LogMessage("g_softwareSpec.AssignMembersFromStream(sofwareSpec) - End");



				//    SetKeySpec(g_softwareSpec);
				//    SetRawKeySpec(sofwareSpec);
				//    //LogMessage("Successfully called g_licServer.GetSoftwareSpec()");
				//    //LogMessage("Successfully called g_licServer.GetSoftwareSpec(): sofwareSpec = |" + sofwareSpec + "|");
				//}
				//else
				//    LogMessage("Called g_licServer.GetSoftwareSpec()", hrValue);

				//String licenseInfo = "";
				//hrValue = g_licServer.GetSoftwareLicenseInfo_ForAll(ref licenseInfo);
				//if (hrValue == 0)
				//{
				//    //LogMessage("Successfully called g_licServer.GetSoftwareLicenseInfo(): licenseInfo = |" + licenseInfo + "|");
				//    LogMessage("Successfully called g_licServer.GetSoftwareLicenseInfo()");
				//}
				//else
				//    LogMessage("Called g_licServer.GetSoftwareLicenseInfo()", hrValue);


				LogMessage("ts_connectButton_Click - Leave");
			}
			catch (COMException ex)
			{
				LogMessage(ex.Message);
			}
			finally
			{
				Cursor.Current = Cursors.Default;
			}

		}
		private void ts_LicenseInfoAllButton_Click(object sender, EventArgs e)
		{
			try
			{
				String generalStream = g_licServer.GetAllSoftwareLicenses();
				{
					Solimar.Licensing.Attribs.AttribsMemberStringList strList = new Solimar.Licensing.Attribs.AttribsMemberStringList("stringList", new System.Collections.ArrayList());
					strList.SVal = generalStream;

					bool bFirstTime = true;
					foreach (string softwareLicense in strList.TVal)
					{
						//long startTime, endTime;
						//startTime = System.Diagnostics.Stopwatch.GetTimestamp();
						
						generalStream = g_licServer.GetSoftwareLicenseInfo_ByLicense(softwareLicense);

						//endTime = System.Diagnostics.Stopwatch.GetTimestamp();
						//globalPerAverageCall.IncrementBy(endTime - startTime);
						//globalPerAverageCallBase.Increment();
						//LogMessage("globalPerAverageCall: " + globalPerAverageCall.RawValue.ToString());
						//LogMessage("globalPerAverageCallBase: " + globalPerAverageCallBase.RawValue.ToString());
						{
							LogMessage("LicenseInfo Size: " + generalStream.ToString().Length);

							SetRawKeySpec(generalStream);
							g_licInfoAttrib = new Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs();
							g_licInfoAttrib.AssignMembersFromStream(generalStream);

							populateLicenseInfo(softwareLicense, g_licInfoAttrib, bFirstTime);
							bFirstTime = false;
							//licenseInfoTreeView.ExpandAll();
							
						}
					}
					
				}
			}
			catch (COMException ex)
			{
				SetRawKeySpec(ex.Message);
				LogMessage(ex.Message);
			}
		}

		private void populateLicenseInfo(string _paramNodeName, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs _paramLicInfoAttribs, bool _paramClearAll)
		{
			try
			{
				licenseInfoViewerControl1.SetData(g_softwareSpec, _paramLicInfoAttribs, _paramClearAll);

				licenseInfoTreeView.BeginUpdate();
				if (_paramClearAll == true)
					licenseInfoTreeView.Nodes.Clear();
				bool bValid = false;
				string licStatus = g_licServer.GetSoftwareLicenseStatus_ByLicense(_paramNodeName, ref bValid);

				TreeNode rootNode = new TreeNode(_paramNodeName + " (" + licStatus + ")");
				if (bValid == false)
					rootNode.ForeColor = System.Drawing.Color.Red;

				//Tree_Add_Lic_LicenseInfoAttribs(ref rootNode, g_licInfoAttrib);
				Tree_Add_Lic_LicenseInfoAttribs(ref rootNode, _paramLicInfoAttribs);

				licenseInfoTreeView.Nodes.Add(rootNode);
				rootNode.Expand();


				licenseInfoTreeView.EndUpdate();
			}
			catch (COMException ex)
			{
				SetRawKeySpec(ex.Message);
				LogMessage(ex.Message);
			}
		}

		

		private void ts_lmgrInitButton_Click(object sender, EventArgs e)
		{
			int hrValue = 0;
			try
			{
				g_licenseWrapper = new Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseWrapper();
				g_licenseWrapper.ConnectEx("jlan5");
				LogMessage("Successfully called g_licenseWrapper.ConnectEx(jlan5)");


				bool bInitialized = g_licenseWrapper.InitializeEx(ts_lmgrAppInstanceTextBox.Text, 14, 1, 1, false, "", false, Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseWrapper.LICENSE_LEVEL.UI_LEVEL_ALL, 0, false, false);
				if (bInitialized)
					LogMessage("Successfully called g_licenseWrapper.InitializeEx(" + ts_lmgrAppInstanceTextBox.Text+ ")");
				if (hrValue != 0)
					LogMessage("Failed called g_licenseWrapper.InitializeEx(" + ts_lmgrAppInstanceTextBox.Text + ")");

			}
			catch (COMException ex)
			{
				LogMessage(ex.Message);
			}
		}

		private void ts_lmgrValidateButton_Click(object sender, EventArgs e)
		{
			try
			{
				bool bValid = g_licenseWrapper.ValidateLicenseEx();
				if (bValid)
					LogMessage("Successfully called g_licenseWrapper.ValidateLicenseEx()");
				if(bValid == false)
					LogMessage("Successfully called g_licenseWrapper.ValidateLicenseEx(), but licensing is invalid");
			}
			catch (COMException ex)
			{
				LogMessage(ex.Message);
			}
			
		}
		private void ts_lmgrObtainModButton_Click(object sender, EventArgs e)
		{
			try
			{
				g_licenseWrapper.ModuleLicenseObtainEx(28, 1);
				LogMessage("Successfully called g_licenseWrapper.ModuleLicenseObtainEx(28, 1)");
			}
			catch (COMException ex)
			{
				LogMessage(ex.Message);
			}
		}
		private void ts_lmgrTotalModButton_Click(object sender, EventArgs e)
		{
			try
			{
				int licenseCount = g_licenseWrapper.ModuleLicenseTotalEx(28);
				LogMessage("Successfully called g_licenseWrapper.ModuleLicenseTotalEx(28) licenseTotal = " + licenseCount.ToString());
			}
			catch (COMException ex)
			{
				LogMessage(ex.Message);
			}
		}

		private void ts_lmgrInUseModButton_Click(object sender, EventArgs e)
		{
			try
			{
				int licenseCount = g_licenseWrapper.ModuleLicenseInUseEx(28);
				LogMessage("Successfully called g_licenseWrapper.ModuleLicenseInUseEx(28) licenseInUse = " + licenseCount.ToString());
			}
			catch (COMException ex)
			{
				LogMessage(ex.Message);
			}
		}

		private void ts_lmgrReleaseModButton_Click(object sender, EventArgs e)
		{
			try
			{
				g_licenseWrapper.ModuleLicenseReleaseEx(28, 1);
				LogMessage("Successfully called g_licenseWrapper.ModuleLicenseReleaseEx(28)");
			}
			catch (COMException ex)
			{
				LogMessage(ex.Message);
			}
		}

		private void ts_lmgrDisconnectButton_Click(object sender, EventArgs e)
		{
			try
			{
				if(g_licenseWrapper != null)
					g_licenseWrapper.Dispose();
				LogMessage("Destroy g_licenseWrapper");
			}
			catch (COMException ex)
			{
				LogMessage(ex.Message);
			}
		}

		private void ts_AllModuleInuseButton_Click(object sender, EventArgs e)
		{
			try
			{
				int licenseCount = 0;
				int moduleID = System.Convert.ToInt32(ts_AllModuleIdTextBox.Text);
				int productID = System.Convert.ToInt32(tsgen_prodIdTextBox.Text);
				licenseCount = g_licServer.SoftwareModuleLicenseInUseForAll_ByProduct(productID, moduleID);
				LogMessage("Successfully called g_licServer.SoftwareModuleLicenseInUseForAll_ByProduct(Product(" + productID.ToString() + "), Module(" + moduleID.ToString() + ")) licenseInUse = " + licenseCount.ToString());


				//int hrValue = g_licServer.SoftwareModuleLicenseInUseForAll_ByProduct(productID, moduleID, ref licenseCount);
				//if (hrValue == 0)
				//    LogMessage("Successfully called g_licServer.SoftwareModuleLicenseInUseForAll_ByProduct(Product(" + productID.ToString() + "), Module(" + moduleID.ToString() + ")) licenseInUse = " + licenseCount.ToString());
				//if (hrValue != 0)
				//    LogMessage("Called g_licServer.SoftwareModuleLicenseInUseForAll_ByProduct(Product(" + productID.ToString() + "), Module(" + moduleID.ToString() + "))", hrValue);
			}
			catch (COMException ex)
			{
				LogMessage(ex.Message);
			}
		}

		private void ts_licStatusByProduct_Click(object sender, EventArgs e)
		{
			try
			{
				string licStream;
				int productID = System.Convert.ToInt32(tsgen_prodIdTextBox.Text);
				licStream = g_licServer.GetSoftwareLicenseStatus_ByProduct(productID);
				LogMessage("Successfully called g_licServer.GetSoftwareLicenseStatus_ByProduct(Product(" + productID.ToString() + ")");
				LogMessage(licStream);
			}
			catch (COMException ex)
			{
				LogMessage(ex.Message);
			}
		}

		private void ts_AllModuleTotalButton_Click(object sender, EventArgs e)
		{
			try
			{
				int licenseCount = 0;
				int moduleID = System.Convert.ToInt32(ts_AllModuleIdTextBox.Text);
				int productID = System.Convert.ToInt32(tsgen_prodIdTextBox.Text);


				licenseCount = g_licServer.SoftwareModuleLicenseTotalForAll_ByProduct(productID, moduleID);
				LogMessage("Successfully called g_licServer.SoftwareModuleLicenseTotalForAll_ByProduct(Product(" + productID.ToString() + "), Module(" + moduleID.ToString() + ")) licenseInUse = " + licenseCount.ToString());

				//int hrValue = g_licServer.SoftwareModuleLicenseTotalForAll_ByProduct(productID, moduleID, ref licenseCount);
				//if (hrValue == 0)
				//    LogMessage("Successfully called g_licServer.SoftwareModuleLicenseTotalForAll_ByProduct(Product(" + productID.ToString() + "), Module(" + moduleID.ToString() + ")) licenseInUse = " + licenseCount.ToString());
				//if (hrValue != 0)
				//    LogMessage("Called g_licServer.SoftwareModuleLicenseTotalForAll_ByProduct(Product(" + productID.ToString() + "), Module(" + moduleID.ToString() + ")): hr = " + String.Format("0x{0:X8}", hrValue));
			}
			catch (COMException ex)
			{
				LogMessage(ex.Message);
				//LogMessage("Called g_licServer.SoftwareModuleLicenseTotalForAll_ByProduct(Product(" + productID.ToString() + "), Module(" + moduleID.ToString() + ")): hr = " + String.Format("0x{0:X8}", hrValue));
			}
		}

		private void ts_LicenseAppInstAllButton_Click(object sender, EventArgs e)
		{
			try
			{
				String generalStream = "";
				int productID = System.Convert.ToInt32(tsgen_prodIdTextBox.Text);
				generalStream = g_licServer.SoftwareGetApplicationInstanceListByProduct(productID);
				LogMessage("Successfully called g_licServer.SoftwareGetApplicationInstanceListByProduct(Product(" + productID.ToString() + ")): stream = " + generalStream);

				SetRawKeySpec(generalStream);
				//Solimar.SPD.Attribs.Lic_LicenseInfoAttribs g_licInfoAttrib = new Solimar.SPD.Attribs.Lic_LicenseInfoAttribs();
				//g_licInfoAttrib.AssignMembersFromStream(generalStream);
			}
			catch (COMException ex)
			{
				LogMessage(ex.Message);
			}
		}



		private enum generateLicType
		{
			eGenerateUnknown = 1,
			eGenerateBackup = 2,
			eGenerateDisasterRecovery = 3,
			eGenerateTestDev = 4,
		};
		private void ts_genLicPacButton_Click(object sender, EventArgs e)
		{
			try
			{
				generateLicType eGenType = generateLicType.eGenerateTestDev;
				//generateLicType eGenType = generateLicType.eGenerateDisasterRecovery;
				//generateLicType eGenType = generateLicType.eGenerateBackup;
				///*
				Solimar.Licensing.Attribs.Lic_PackageAttribs licPackage = new Solimar.Licensing.Attribs.Lic_PackageAttribs();
				licPackage.licLicenseInfoAttribs.TVal.customerID.TVal = 0x100;
				licPackage.licLicenseInfoAttribs.TVal.softwareGroupLicenseID.TVal = 0x1cba;


				if (eGenType == generateLicType.eGenerateDisasterRecovery || eGenType == generateLicType.eGenerateTestDev)
				{
					licPackage.licLicenseInfoAttribs.TVal.licVerificationAttribs.TVal = g_licInfoAttrib.licVerificationAttribs;
				}
				else if (eGenType == generateLicType.eGenerateBackup)
				{
					licPackage.licLicenseInfoAttribs.TVal.licVerificationAttribs.TVal.validationTokenList.TVal.Clear();
					//licPackage.licLicenseInfoAttribs.TVal.licVerificationAttribs.TVal = new Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_VerificationAttribs();
					Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs verToken = new Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs();
					verToken.tokenType.TVal = Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID;
					verToken.tokenValue.TVal = "0100-1cba";
					licPackage.licLicenseInfoAttribs.TVal.licVerificationAttribs.TVal.validationTokenList.TVal.Add(verToken);

					verToken = new Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs();
					verToken.tokenType.TVal = Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttLicenseCode;
					verToken.tokenValue.TVal = "";
					licPackage.licLicenseInfoAttribs.TVal.licVerificationAttribs.TVal.validationTokenList.TVal.Add(verToken);
				}

				//licPackage.licSoftwareSpecAttribs.TVal = g_softwareSpec;
				//licPackage.licLicenseInfoAttribs.TVal.softwareLicType.TVal = Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltFailover;
				if (eGenType == generateLicType.eGenerateDisasterRecovery)
					licPackage.licLicenseInfoAttribs.TVal.softwareLicType.TVal = Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltDisasterRecovery;
				else if (eGenType == generateLicType.eGenerateBackup)
					licPackage.licLicenseInfoAttribs.TVal.softwareLicType.TVal = Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltFailover;
				else if (eGenType == generateLicType.eGenerateTestDev)
					licPackage.licLicenseInfoAttribs.TVal.softwareLicType.TVal = Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltTestDev;

				//licPackage.licLicenseInfoAttribs.TVal.softwareLicType.TVal = Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltStandard;

				//licPackage.licLicenseInfoAttribs.TVal.softwareLicType.TVal = Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltTestDev;
				licPackage.licLicenseInfoAttribs.TVal.productList.TVal = g_licInfoAttrib.productList;
				if (eGenType == generateLicType.eGenerateDisasterRecovery || eGenType == generateLicType.eGenerateTestDev)
				{
					licPackage.licLicenseInfoAttribs.TVal.activationTotal.TVal = 10;
					licPackage.licLicenseInfoAttribs.TVal.activationCurrent.TVal = 0;
					licPackage.licLicenseInfoAttribs.TVal.activationAmountInDays.TVal = 6;
					licPackage.licLicenseInfoAttribs.TVal.activationExpirationDate.TVal = (DateTime.Now.ToUniversalTime());
				}
				else if (eGenType == generateLicType.eGenerateBackup)
				{
				}

				//Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_VerificationCodeAttribs newVerCode = new Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_VerificationCodeAttribs();
				//newVerCode.verificationDate.TVal = DateTime.Now.ToUniversalTime();
				//newVerCode.verificationValue.TVal = "TestCode";
				//licPackage.licLicenseInfoAttribs.TVal.licVerificationAttribs.TVal.verificationCodeHistoryList.TVal.Add(newVerCode);

				//
				//Don't set Hardware key code, it is auto generated when license packet is successfully applied
				foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs verToken in licPackage.licLicenseInfoAttribs.TVal.licVerificationAttribs.TVal.validationTokenList.TVal)
				{
					if (verToken.tokenType.TVal == Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttLicenseCode)
						verToken.tokenValue.TVal = "";
					else if (verToken.tokenType.TVal == Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType.ttHardwareKeyID)
						verToken.tokenValue.TVal = "0100-1000";
				}

				//
				//Don't add any verification history to the attribs.
				licPackage.licLicenseInfoAttribs.TVal.licVerificationAttribs.TVal.verificationCodeHistoryList.TVal.Clear();

				//for (int idx = 0; idx < licPackage.licLicenseInfoAttribs.TVal.productList.TVal.Count; idx++)
				//{
				//    Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs prodInfo = licPackage.licLicenseInfoAttribs.TVal.productList.TVal[idx] as Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs;
				//    if (prodInfo.productID == 18)	//Remove All productID == 18
				//    {
				//        licPackage.licLicenseInfoAttribs.TVal.productList.TVal.RemoveAt(idx);
				//        idx--;
				//    }
				//}
				foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs prodInfo in licPackage.licLicenseInfoAttribs.TVal.productList.TVal)
				{
					#region Convert Product ID to Test/Dev Product ID
					if (eGenType == generateLicType.eGenerateTestDev)
					{
						switch ((Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID)prodInfo.productID.TVal)
						{
							case Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_Iconvert:
								prodInfo.productID.TVal = (int)Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_TestDevIconvert;
								break;
							case Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_Rubika:
								prodInfo.productID.TVal = (int)Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_TestDevRubika;
								break;
							case Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_RubikaProcessBuilder:
								prodInfo.productID.TVal = (int)Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_TestDevRubikaProcessBuilder;
								break;
							case Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_SdxDesigner:
								prodInfo.productID.TVal = (int)Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_TestDevSdxDesigner;
								break;
							case Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_SolFusion:
								prodInfo.productID.TVal = (int)Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_TestDevSolfusionSp;
								break;
							case Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_TestDevSolIndexer:
								prodInfo.productID.TVal = (int)Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_TestDevSolIndexer;
								break;
							case Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_SolScript:
								prodInfo.productID.TVal = (int)Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_TestDevSolScript;
								break;
							case Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_SolsearcherEnt:
								prodInfo.productID.TVal = (int)Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_TestDevSolsearcherEp;
								break;
							case Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_Spde:
								prodInfo.productID.TVal = (int)Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_TestDevSpde;
								break;
							case Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_SpdeQueueManager:
								prodInfo.productID.TVal = (int)Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_TestDevSpdeQueueManager;
								break;
							case Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_SolsearcherSp:
								prodInfo.productID.TVal = (int)Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_TestDevSseSp;
								break;
							default:
								break;
						}
					}
					#endregion
					#region Convert Test/Dev Product ID to Product ID
					else //Non-TestDev
					{
						switch ((Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID)prodInfo.productID.TVal)
						{
							case Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_TestDevIconvert:
								prodInfo.productID.TVal = (int)Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_Iconvert;
								break;
							case Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_TestDevRubika:
								prodInfo.productID.TVal = (int)Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_Rubika;
								break;
							case Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_TestDevRubikaProcessBuilder:
								prodInfo.productID.TVal = (int)Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_RubikaProcessBuilder;
								break;
							case Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_TestDevSdxDesigner:
								prodInfo.productID.TVal = (int)Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_SdxDesigner;
								break;
							case Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_TestDevSolfusionSp:
								prodInfo.productID.TVal = (int)Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_SolFusion;
								break;
							case Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_TestDevSolIndexer:
								prodInfo.productID.TVal = (int)Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_TestDevSolIndexer;
								break;
							case Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_TestDevSolScript:
								prodInfo.productID.TVal = (int)Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_SolScript;
								break;
							case Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_TestDevSolsearcherEp:
								prodInfo.productID.TVal = (int)Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_SolsearcherEnt;
								break;
							case Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_TestDevSpde:
								prodInfo.productID.TVal = (int)Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_Spde;
								break;
							case Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_TestDevSpdeQueueManager:
								prodInfo.productID.TVal = (int)Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_SpdeQueueManager;
								break;
							case Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_TestDevSseSp:
								prodInfo.productID.TVal = (int)Solimar.Licensing.Attribs.Lic_PackageAttribs.TLic_ProductID.pid_SolsearcherSp;
								break;
							default:
								break;
						}
					}
					#endregion

					//change prod app instance
					//if (prodInfo.productID.TVal == 27)
					//{
					//    prodInfo.productAppInstance.TVal = 7;
					//}

					//Look for SDX Designer product, and change ID to a SDX Designer Test/Dev.
					//if (prodInfo.productID.TVal == 10)
					//{
					//    prodInfo.productID.TVal = 18;
					//    break;
					//}
					//Look for SPDE product, and change ID to a SPDE Test/Dev.
					//if (prodInfo.productID.TVal == 14)
					//{
					//    prodInfo.productID.TVal = 20;
					//    break;
					//}

					//if (prodInfo.productID.TVal == 12)	//Rubika
					//{
					//    Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs tmpModAttribs = new Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs();
					//    tmpModAttribs.moduleID.TVal = 0;
					//    tmpModAttribs.moduleValue.TVal = 1;
					//    tmpModAttribs.moduleAppInstance.TVal = 1;
					//    tmpModAttribs.moduleExpirationDate.TVal = new DateTime(2008, 10, 1);
					//    prodInfo.moduleList.TVal.Add(tmpModAttribs);
					//    tmpModAttribs = new Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs();
					//    tmpModAttribs.moduleID.TVal = 0;
					//    tmpModAttribs.moduleValue.TVal = 1;
					//    tmpModAttribs.moduleAppInstance.TVal = 1;
					//    tmpModAttribs.moduleExpirationDate.TVal = new DateTime(2008, 8, 1);
					//    prodInfo.moduleList.TVal.Add(tmpModAttribs);
					//}

					//if (prodInfo.productID.TVal == 14)	//SPDE
					//{
					//    Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs tmpModAttribs1 = new Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs();
					//    tmpModAttribs1.moduleID.TVal = 0;
					//    tmpModAttribs1.moduleValue.TVal = 1;
					//    tmpModAttribs1.moduleAppInstance.TVal = 1;
					//    tmpModAttribs1.moduleExpirationDate.TVal = new DateTime(2008, 10, 1);
					//    prodInfo.moduleList.TVal.Add(tmpModAttribs1);

					//    Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs tmpModAttribs2 = new Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs();
					//    tmpModAttribs2.moduleID.TVal = 1;
					//    tmpModAttribs2.moduleValue.TVal = 2;
					//    tmpModAttribs2.moduleAppInstance.TVal = 1;
					//    tmpModAttribs2.moduleExpirationDate.TVal = new DateTime(2008, 12, 1);
					//    prodInfo.moduleList.TVal.Add(tmpModAttribs2);

					//    Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs tmpModAttribs3 = new Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs();
					//    tmpModAttribs3.moduleID.TVal = 0;
					//    tmpModAttribs3.moduleValue.TVal = 5;
					//    tmpModAttribs3.moduleAppInstance.TVal = 1;
					//    tmpModAttribs3.moduleExpirationDate.TVal = new DateTime(2008, 11, 1);
					//    prodInfo.moduleList.TVal.Add(tmpModAttribs3);
					//}
				}

				//Add Queue Manager
				//Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs tmpProdInfo = new Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs();
				//tmpProdInfo.productID.TVal = 18;	//QueueManager
				//tmpProdInfo.productAppInstance.TVal = 5;
				//tmpProdInfo.product_Major.TVal = 6;
				//tmpProdInfo.product_Minor.TVal = 1;
				//Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs tmpModAttribs3 = new Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs();
				//tmpModAttribs3.moduleID.TVal = 400;
				//tmpModAttribs3.moduleValue.TVal = 5;
				//tmpModAttribs3.moduleAppInstance.TVal = 5;
				//tmpModAttribs3.moduleExpirationDate.TVal = new DateTime(2008, 11, 1);
				//tmpProdInfo.moduleList.TVal.Add(tmpModAttribs3);
				//licPackage.licLicenseInfoAttribs.TVal.productList.TVal.Add(tmpProdInfo);


				//SetRawKeySpec(licPackage.ToString() + "\r\n\r\n" + licPackage.licLicenseInfoAttribs.TVal.licVerificationAttribs.TVal.ToString());
				SetRawKeySpec(licPackage.ToString());
				//return;
				//*/


				//string licenseAttribs = "Fake License Package Attribs";
				string licenseAttribs = licPackage.ToString();
				DateTime expirationDate = DateTime.Now.AddDays(7); //Good for 7 days...

				string verificationCode = "";
				//string newLicensePacket = "";
				byte[] newByteArrayLicensePacket = null;
				LogMessage("");

				LogMessage("Try to call g_licServer.GenerateSoftwareLicPacket()");
				g_licServer.GenerateSoftwareLicPacket(licenseAttribs, expirationDate, ref verificationCode, ref newByteArrayLicensePacket);
				//g_licServer.GenerateSoftwareLicPacket(licenseAttribs, expirationDate, ref verificationCode, ref newLicensePacket);
				LogMessage("   Successfully called g_licServer.GenerateSoftwareLicPacket()");
				//LogMessage("   licenseAttribs: " + licenseAttribs);
				LogMessage("   expirationDate: " + expirationDate.ToString());
				LogMessage("   verificationCode: " + verificationCode);
				StringBuilder sBuilder = new StringBuilder();
				StringBuilder asAsciiBuilder = new StringBuilder();
				if (newByteArrayLicensePacket != null)
				{
					g_byteLicPacket = newByteArrayLicensePacket;
					//int x = 0;
					//foreach (Byte by in newByteArrayLicensePacket)
					//{
					//    if(sBuilder.Length != 0)
					//        sBuilder.Append(" ");
					//    sBuilder.Append(String.Format("0x{0:X2}", by));
					//    if(x%2 == 0)
					//        asAsciiBuilder.Append((char)by);
					//    x++;
					//}
					//LogMessage("   newByteArrayLicensePacket: " + sBuilder.ToString());
				}
				//else
				//    LogMessage("   newByteArrayLicensePacket: NULL");
			}
			catch (COMException ex)
			{
				LogMessage(ex);
			}
		}
		byte[] g_byteLicPacket = null;
		private string g_licenseVerificationFile = "lic.ver";
		private string g_licenseArchiveFile = "lic.arc";
		
		private void ts_applyLicPacButton_Click(object sender, EventArgs e)
		{
			try
			{
				if (g_byteLicPacket == null)
				{
					LogMessage("Failed, no license packet to apply");
				}
				else
				{
					LogMessage("Try to call g_licServer.EnterSoftwareLicPacket()");
					string verificationCode = "";

					verificationCode = g_licServer.EnterSoftwareLicPacket(g_byteLicPacket);
					LogMessage("   Successfully called g_licServer.EnterSoftwareLicPacket()");
					LogMessage("   verificationCode: " + verificationCode);
				}
			}
			catch (COMException ex)
			{
				LogMessage(ex);
			}
		}
		private void ts_genLicVerButton_Click(object sender, EventArgs e)
		{
			try
			{
				byte[] newByteArrayLicensePacket = null;
				LogMessage("");

				LogMessage("Try to call g_licServer.GenerateVerifyDataWithVerCode_ByLicense()");
				g_licServer.GenerateVerifyDataWithVerCode_ByLicense(ts_genLicTextBox.Text, ref newByteArrayLicensePacket);
				LogMessage("   Successfully called g_licServer.GenerateVerifyDataWithVerCode_ByLicense()");
				if (newByteArrayLicensePacket != null)
				{
					StringBuilder sBuilder = new StringBuilder();
					StringBuilder asAsciiBuilder = new StringBuilder();
					g_byteLicPacket = newByteArrayLicensePacket;
					int x = 0;
					foreach (Byte by in newByteArrayLicensePacket)
					{
						if (sBuilder.Length != 0)
							sBuilder.Append(" ");
						sBuilder.Append(String.Format("0x{0:X2}", by));
						if (sBuilder.Length % 8 == 0)
							sBuilder.Append("\r\n");
						//if (x % 2 == 0)
							asAsciiBuilder.Append((char)by);

						x++;
					}
					//LogMessage("   newByteArrayLicensePacket: " + sBuilder.ToString());
					//SetRawKeySpec(asAsciiBuilder.ToString());
					SetRawKeySpec(sBuilder.ToString());

					using (System.IO.TextWriter tw = new System.IO.StreamWriter("licVer.sml"))
					{
						// write a line of text to the file
						tw.WriteLine(asAsciiBuilder.ToString());

						// close the stream
						tw.Close();
					}
					using (System.IO.FileStream fs = new System.IO.FileStream(g_licenseVerificationFile, System.IO.FileMode.OpenOrCreate))
					{
						System.IO.BinaryWriter bw = new System.IO.BinaryWriter(fs);

						// write a line of text to the file
						bw.Write(newByteArrayLicensePacket);

						// close the stream
						bw.Close();
						fs.Close();
					}
				}
			}
			catch (COMException ex)
			{
				LogMessage(ex);
			}
		}
		private void ts_genLicCopyButton_Click(object sender, EventArgs e)
		{
			try
			{
				byte[] newByteArrayLicensePacket = null;
				LogMessage("");

				LogMessage("Try to call g_licServer.GenerateVerifyDataWithLicInfo_ByLicense()");
				g_licServer.GenerateVerifyDataWithLicInfo_ByLicense(ts_genLicTextBox.Text, ref newByteArrayLicensePacket);
				LogMessage("   Successfully called g_licServer.GenerateVerifyDataWithLicInfo_ByLicense()");
				if (newByteArrayLicensePacket != null)
				{
					StringBuilder sBuilder = new StringBuilder();
					StringBuilder asAsciiBuilder = new StringBuilder();
					g_byteLicPacket = newByteArrayLicensePacket;
					int x = 0;
					foreach (Byte by in newByteArrayLicensePacket)
					{
						if (sBuilder.Length != 0)
							sBuilder.Append(" ");
						sBuilder.Append(String.Format("0x{0:X2}", by));
						if (sBuilder.Length % 8 == 0)
							sBuilder.Append("\r\n");
						//if (x % 2 == 0)
						asAsciiBuilder.Append((char)by);
						
						x++;
					}
					//LogMessage("   newByteArrayLicensePacket: " + sBuilder.ToString());
					//SetRawKeySpec(asAsciiBuilder.ToString());
					SetRawKeySpec(sBuilder.ToString());
					using (System.IO.TextWriter tw = new System.IO.StreamWriter("licCopy.sml"))
					{
						// write a line of text to the file
						tw.WriteLine(asAsciiBuilder.ToString());

						// close the stream
						tw.Close();
					}
				}
			}
			catch (COMException ex)
			{
				LogMessage(ex);
			}
		}
		private void ts_viewVerifyDataButton_Click(object sender, EventArgs e)
		{
			try
			{
				OpenFileDialog openDialog = new OpenFileDialog();
				openDialog.DefaultExt = "vData";
				openDialog.Filter = "License Verification Data|*.vData";
				openDialog.Title = "Import License Verification Data";
				if (openDialog.InitialDirectory.Length == 0)
					openDialog.InitialDirectory = Environment.SpecialFolder.Desktop.ToString();
				if (openDialog.ShowDialog() == DialogResult.OK)
				{
					Byte[] licPktBytes = System.IO.File.ReadAllBytes(openDialog.FileName);
					string licenseStream = g_licServer.GenerateLicInfo_ByVerifyData(licPktBytes);
					LogMessage("   Successfully called g_licServer.GenerateLicInfo_ByVerifyData()");
					LogMessage("   licenseStream: " + licenseStream);

					SetRawKeySpec(licenseStream);
					Solimar.Licensing.Attribs.Lic_PackageAttribs tmpLicPackage = new Solimar.Licensing.Attribs.Lic_PackageAttribs();
					tmpLicPackage.AssignMembersFromStream(licenseStream);

					string softwareLicense = Solimar.Licensing.Attribs.Lic_LicenseInfoAttribsHelper.GetDisplayLabel(tmpLicPackage.licLicenseInfoAttribs);
					populateLicenseInfo(softwareLicense + " (VerifyData)", tmpLicPackage.licLicenseInfoAttribs, true);
				}
			}
			catch (COMException ex)
			{
				LogMessage(ex);
			}
		}
		private void ts_DrActivationButton_Click(object sender, EventArgs e)
		{
			//ts_DrLicenseTextBox
			try
		 	{
				LogMessage("Try to call SoftwareLicenseUseActivationToExtendTime_ByLicense(License: " + ts_DrLicenseTextBox.Text + ")");
				g_licServer.SoftwareLicenseUseActivationToExtendTime_ByLicense(ts_DrLicenseTextBox.Text);
			}
			catch (COMException ex)
			{
				LogMessage(ex);
			}
		}
		private void ts_genLicArcButton_Click(object sender, EventArgs e)
		{
			try
			{
				//LogMessage("Try to call ts_genLicArcButton_Click(License: " + ts_DrLicenseTextBox.Text + ")");
				byte[] newByteArrayLicenseArchive = null;
				LogMessage("");

				LogMessage("Try to call g_licServer.GenerateVerifyDataWithVerCode_ByLicense()");
				g_licServer.GenerateSoftwareLicArchive_ByLicense(ts_genLicTextBox.Text, ref newByteArrayLicenseArchive);
				LogMessage("   Successfully called g_licServer.GenerateVerifyDataWithVerCode_ByLicense()");
				if (newByteArrayLicenseArchive != null)
				{
					StringBuilder sBuilder = new StringBuilder();
					StringBuilder asAsciiBuilder = new StringBuilder();
					g_byteLicPacket = newByteArrayLicenseArchive;
					int x = 1;
					foreach (Byte by in newByteArrayLicenseArchive)
					{
						//if (sBuilder.Length != 0)
						//    sBuilder.Append(" ");
						sBuilder.Append(String.Format("0x{0:x2}", by));
						//if (sBuilder.Length % 8 == 0)
						//if (sBuilder.Length % 16 == 0)
						if (x != newByteArrayLicenseArchive.Length)
							sBuilder.Append(", ");
						if (x % 16 == 0)
							sBuilder.Append("\r\n");
						//if (x % 2 == 0)
						asAsciiBuilder.Append((char)by);

						x++;
					}
					//LogMessage("   newByteArrayLicensePacket: " + sBuilder.ToString());
					//SetRawKeySpec(asAsciiBuilder.ToString());
					SetRawKeySpec(sBuilder.ToString());

					using (System.IO.TextWriter tw = new System.IO.StreamWriter("licArchive.txt"))
					{
						// write a line of text to the file
						tw.WriteLine(sBuilder.ToString());

						// close the stream
						tw.Close();
					}
					using (System.IO.FileStream fs = new System.IO.FileStream(g_licenseArchiveFile, System.IO.FileMode.OpenOrCreate))
					{
						System.IO.BinaryWriter bw = new System.IO.BinaryWriter(fs);

						// write a line of text to the file
						bw.Write(newByteArrayLicenseArchive);

						// close the stream
						bw.Close();
						fs.Close();
					}
				}
			}
			catch (COMException ex)
			{
				LogMessage(ex);
			}
		}
		private void ts_entLicArcButton_Click(object sender, EventArgs e)
		{
			try
			{
				using (System.IO.FileStream fs = new System.IO.FileStream(g_licenseArchiveFile, System.IO.FileMode.Open))
				{
					System.IO.BinaryReader bReader = new System.IO.BinaryReader(fs);

					byte[] byteBuffer = new byte[fs.Length];
					byteBuffer = bReader.ReadBytes((int)fs.Length);

					// close the stream
					bReader.Close();
					fs.Close();

					LogMessage("Try to call g_licServer.EnterSoftwareLicArchive()");
					g_licServer.EnterSoftwareLicArchive(byteBuffer);
					LogMessage("   Successfully called g_licServer.EnterSoftwareLicArchive()");
				}
			}
			catch (COMException ex)
			{
				LogMessage(ex);
			}

		}
		private void ts_viewLicPackageButton_Click(object sender, EventArgs e)
		{
			try
			{
				OpenFileDialog openDialog = new OpenFileDialog();
				openDialog.DefaultExt = "packet";
				openDialog.Filter = "License Packet|*.packet";
				openDialog.Title = "Import License Packet";
				if (openDialog.InitialDirectory.Length == 0)
					openDialog.InitialDirectory = Environment.SpecialFolder.Desktop.ToString();
				if (openDialog.ShowDialog() == DialogResult.OK)
				{
					Byte[] licPktBytes = System.IO.File.ReadAllBytes(openDialog.FileName);
					LogMessage("Try to call g_licServer.GenerateLicInfo_BySoftwareLicPacket()");
					string licPackageStream = g_licServer.GenerateLicPackage_BySoftwareLicPacket(licPktBytes);
					LogMessage("   Successfully called g_licServer.GenerateLicInfo_BySoftwareLicPacket()");

					Solimar.Licensing.Attribs.Lic_PackageAttribs tmpLicPackageAttrib = new Solimar.Licensing.Attribs.Lic_PackageAttribs();
					tmpLicPackageAttrib.AssignMembersFromStream(licPackageStream);
					string softwareLicense = Solimar.Licensing.Attribs.Lic_LicenseInfoAttribsHelper.GetDisplayLabel(tmpLicPackageAttrib.licLicenseInfoAttribs);
					populateLicenseInfo(softwareLicense + " (LicPackage)", tmpLicPackageAttrib.licLicenseInfoAttribs, true);
				}
			}
			catch (COMException ex)
			{
				LogMessage(ex);
			}
			//g_byteLicPacket

			//try
			//{
			//    using (System.IO.FileStream fs = new System.IO.FileStream(g_licenseVerificationFile, System.IO.FileMode.Open))
			//    {
			//        System.IO.BinaryReader bReader = new System.IO.BinaryReader(fs);

			//        byte[] byteBuffer = new byte[fs.Length];
			//        byteBuffer = bReader.ReadBytes((int)fs.Length);

			//        // close the stream
			//        bReader.Close();
			//        fs.Close();

			//        LogMessage("Try to call g_licServer.GenerateLicInfo_ByVerifyData()");
			//        string licInfo = g_licServer.GenerateLicInfo_ByVerifyData(byteBuffer);
			//        LogMessage("   Successfully called g_licServer.GenerateLicInfo_ByVerifyData()");

			//        Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs tmpLicInfoAttrib = new Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs();
			//        tmpLicInfoAttrib.AssignMembersFromStream(licInfo);
			//        string softwareLicense = Solimar.Licensing.Attribs.Lic_LicenseInfoAttribsHelper.GetDisplayLabel(tmpLicInfoAttrib);
			//        populateLicenseInfo(softwareLicense + " (LicVerify)", tmpLicInfoAttrib, true);
			//    }
			//}
			//catch (COMException ex)
			//{
			//    LogMessage(ex);
			//}
		}
		
		private void ts_viewLicArchiveButton_Click(object sender, EventArgs e)
		{
			try
			{
				OpenFileDialog openDialog = new OpenFileDialog();
				openDialog.DefaultExt = "licArchive";
				openDialog.Filter = "License Archive|*.licArchive";
				openDialog.Title = "Import License Archive";
				if (openDialog.InitialDirectory.Length == 0)
					openDialog.InitialDirectory = Environment.SpecialFolder.Desktop.ToString();
				if (openDialog.ShowDialog() == DialogResult.OK)
				{
					Byte[] licPktBytes = System.IO.File.ReadAllBytes(openDialog.FileName);
					LogMessage("Try to call g_licServer.GenerateLicInfo_BySoftwareLicArchive()");
					string licPackage = g_licServer.GenerateLicPackage_BySoftwareLicArchive(licPktBytes);
					LogMessage("   Successfully called g_licServer.GenerateLicPackage_BySoftwareLicArchive()");

					Solimar.Licensing.Attribs.Lic_PackageAttribs tmpLicPackageAttrib = new Solimar.Licensing.Attribs.Lic_PackageAttribs();
					tmpLicPackageAttrib.AssignMembersFromStream(licPackage);
					string softwareLicense = Solimar.Licensing.Attribs.Lic_LicenseInfoAttribsHelper.GetDisplayLabel(tmpLicPackageAttrib.licLicenseInfoAttribs);
					populateLicenseInfo(softwareLicense + " (LicArchive)", tmpLicPackageAttrib.licLicenseInfoAttribs, true);
				}
			}
			catch (COMException ex)
			{
				LogMessage(ex);
			}
		}
		private void ts_diffLicPackageButton_Click(object sender, EventArgs e)
		{
			try
			{
				string pacName1 = "";
				string pacName2 = "";
				OpenFileDialog openDialog = new OpenFileDialog();
				openDialog.DefaultExt = "packet";
				openDialog.Filter = "License Packet|*.packet";
				openDialog.Title = "Import License Packet";
				openDialog.FileName = "";
				if (openDialog.InitialDirectory.Length == 0)
					openDialog.InitialDirectory = Environment.SpecialFolder.Desktop.ToString();
				if (openDialog.ShowDialog() == DialogResult.OK)
				{
					pacName1 = openDialog.FileName;
				}

				openDialog.FileName = "";
				if (openDialog.InitialDirectory.Length == 0)
					openDialog.InitialDirectory = Environment.SpecialFolder.Desktop.ToString();
				if (openDialog.ShowDialog() == DialogResult.OK)
				{
					pacName2 = openDialog.FileName;
				}

				StringBuilder msgSb = new StringBuilder();
				msgSb.Append("pacName1: ");
				msgSb.Append(pacName1);
				LogMessage(msgSb.ToString());
				msgSb = new StringBuilder();
				msgSb.Append("pacName2: ");
				msgSb.Append(pacName2);
				LogMessage(msgSb.ToString());
				Byte[] licPktBytes = System.IO.File.ReadAllBytes(pacName1);
				LogMessage("Try to call g_licServer.GenerateLicInfo_BySoftwareLicPacket1()");
				string licPackageStream = g_licServer.GenerateLicPackage_BySoftwareLicPacket(licPktBytes);
				LogMessage("   Successfully called g_licServer.GenerateLicInfo_BySoftwareLicPacket1()");

				Solimar.Licensing.Attribs.Lic_PackageAttribs tmpLicPackageAttrib1 = new Solimar.Licensing.Attribs.Lic_PackageAttribs();
				tmpLicPackageAttrib1.AssignMembersFromStream(licPackageStream);

				licPktBytes = System.IO.File.ReadAllBytes(pacName2);
				LogMessage("Try to call g_licServer.GenerateLicInfo_BySoftwareLicPacket2()");
				licPackageStream = g_licServer.GenerateLicPackage_BySoftwareLicPacket(licPktBytes);
				LogMessage("   Successfully called g_licServer.GenerateLicInfo_BySoftwareLicPacket2()");

				Solimar.Licensing.Attribs.Lic_PackageAttribs tmpLicPackageAttrib2 = new Solimar.Licensing.Attribs.Lic_PackageAttribs();
				tmpLicPackageAttrib2.AssignMembersFromStream(licPackageStream);

				LogMessage(tmpLicPackageAttrib1.ToString());
				LogMessage(tmpLicPackageAttrib2.ToString());
				LogMessage("Compare(s1, s2) : " + string.Compare(tmpLicPackageAttrib1, tmpLicPackageAttrib2, true));

				differenceLicenseInfoControl1.SetData(g_softwareSpec, tmpLicPackageAttrib1, tmpLicPackageAttrib2, Shared.VisualComponents.DifferenceLicenseInfoControl.FilterType.ftShowDifference);

			}
			catch (COMException ex)
			{
				LogMessage(ex);
			}

		}
		private void showToolStripMenuItem_Click(object sender, EventArgs e)
		{
			this.Show();
		}

		private void tsgen_createSplitButton_DropDownItemClicked(object sender, ToolStripItemClickedEventArgs e)
		{
			if (sender is ToolStripSplitButton)
			{
				(sender as ToolStripSplitButton).Text = e.ClickedItem.Text;
				(sender as ToolStripSplitButton).ToolTipText = e.ClickedItem.ToolTipText;
				(sender as ToolStripSplitButton).Tag = e.ClickedItem.Tag;
			}
		}

		private void tsgen_createSplitButton_ButtonClick(object sender, EventArgs e)
		{
			if (sender is ToolStripSplitButton)
			{
				string licServer = string.Compare("<By Config>", connectToMachineToolStripMenuItem.Text, true) == 0 ? "" : connectToMachineToolStripMenuItem.Text;
				CreateConnection(licServer, Convert.ToBoolean(((sender as ToolStripSplitButton).Tag as string)));
			}
		}

		public void MessageCallback(IntPtr pContext, String keyIdent, uint messageType, int hrError, ValueType pvtTimestamp, String message)
		{
			//SolimarLicenseManagerDotNet.SolimarLicenseWrapper.DelegateMessageCallbackManaged delMessageFunc;
			//delMessageFunc = new SolimarLicenseManagerDotNet.SolimarLicenseWrapper.DelegateMessageCallbackManaged(UIMessageCallback);

			StringBuilder sBuilder = new StringBuilder();
			sBuilder.Append(pvtTimestamp != null ? pvtTimestamp.ToString() : DateTime.Now.ToString());
			sBuilder.Append(", ");
			sBuilder.Append(keyIdent);
			sBuilder.Append(", ");
			sBuilder.Append(message);
			sBuilder.Append(", ");
			sBuilder.Append(String.Format("0x{0:X8}", hrError));
			LogMessage(sBuilder.ToString());
			//System.Diagnostics.Trace.WriteLine(sBuilder.ToString());

			//Invoking this here causes a deadlock
			//this.Invoke(delMessageFunc, new object[] { pContext, keyIdent, messageType, hrError, pvtTimestamp, message });
		}
		public void InvalidCallback(IntPtr pContext, int hrError, String message)
		{
			StringBuilder sBuilder = new StringBuilder();
			sBuilder.Append(message);
			sBuilder.Append(", ");
			sBuilder.Append(String.Format("0x{0:X8}", hrError));
			LogMessage(sBuilder.ToString());
		}


		//private System.Collections.Generic.List<Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseWrapper> wrapperList = new List<Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseWrapper>();
		//private System.Collections.Generic.List<Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseWrapper.DelegateMessageCallbackManaged> wrapperDelList = new List<Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseWrapper.DelegateMessageCallbackManaged>();
		private void CreateConnection(string _licServer, bool _bUseSharedLicensing)
		{
			try
			{
				Cursor.Current = Cursors.WaitCursor;

				Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseWrapper tmpLicWrapper = new Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseWrapper();
				Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseWrapper.DelegateMessageCallbackManaged delMessage;
				delMessage = new Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseWrapper.DelegateMessageCallbackManaged(MessageCallback);
				if (tmpLicWrapper.RegisterMessageCallback(this.Handle, delMessage))
					LogMessage("Successfully called RegisterMessageCallback()");
				else
					LogMessage("Failed to call RegisterMessageCallback()");

				Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseWrapper.DelegateInvalidLicenseCallbackManaged delInvalid;
				delInvalid = new Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseWrapper.DelegateInvalidLicenseCallbackManaged(InvalidCallback);
				if (tmpLicWrapper.RegisterInvalidLicenseCallback(this.Handle, delInvalid))
					LogMessage("Successfully called RegisterInvalidLicenseCallback()");
				else
					LogMessage("Failed to call RegisterInvalidLicenseCallback()");

				//Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseWrapper.DelegateLicenseCallbackManaged delInvalid;
				//delInvalid = new Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseWrapper.DelegateInvalidLicenseCallbackManaged(InvalidCallback);
				//if (tmpLicWrapper.RegisterInvalidLicenseCallback(this.Handle, delInvalid))
				//    LogMessage("Successfully called RegisterInvalidLicenseCallback()");
				//else
				//    LogMessage("Failed to call RegisterInvalidLicenseCallback()");

				//VoidCallback

				//wrapperList.Add(tmpLicWrapper);
				//wrapperDelList.Add(delMessage);
				//tmpLicWrapper.ConnectEx(
				if (_licServer.CompareTo("") == 0)
				{
					tmpLicWrapper.ConnectByProductEx(System.Convert.ToInt32(tsgen_prodIdTextBox.Text), _bUseSharedLicensing);
					LogMessage("Successfully called tmpLicWrapper.ConnectByProductEx(" + tsgen_prodIdTextBox.Text + ", " + _bUseSharedLicensing.ToString() + ")");
				}
				else
				{
					tmpLicWrapper.ConnectEx(_licServer, _bUseSharedLicensing, false);
					LogMessage("Successfully called tmpLicWrapper.ConnectEx(" + _licServer + ", " + _bUseSharedLicensing.ToString() + ", false)");
				}

				bool bInitialized = tmpLicWrapper.InitializeEx(tsgen_appInstTextBox.Text, System.Convert.ToInt32(tsgen_prodIdTextBox.Text), 1, 1, false, "", false, Solimar.Licensing.LicenseManagerWrapper.SolimarLicenseWrapper.LICENSE_LEVEL.UI_LEVEL_ALL, 0, false, false);
				LogMessage("Successfully called tmpLicWrapper.InitializeEx(" + System.Convert.ToInt32(tsgen_prodIdTextBox.Text) + ", " + ts_lmgrAppInstanceTextBox.Text + "), bInitialized: " + bInitialized.ToString());


				LicensingTesterToolbar tmpTBar = new LicensingTesterToolbar();
				tmpTBar.SetData(System.Convert.ToInt32(tsgen_prodIdTextBox.Text), tsgen_appInstTextBox.Text, tmpLicWrapper);
				tmpTBar.delWriteMessage += new LicensingTesterToolbar.DelegateParamStringReturnsVoid(LogMessage);
				toolStripContainer1.LeftToolStripPanel.Controls.Add(tmpTBar);

			}
			catch (COMException ex)
			{
				LogMessage(ex.Message);
			}
			finally
			{
				Cursor.Current = Cursors.Default;
			}

		}

		private void messageListView_KeyDown(object sender, KeyEventArgs e)
		{
			if (e.KeyCode == Keys.A && e.Control)
			{
				messageListView.BeginUpdate();
				foreach (ListViewItem li in messageListView.Items)
					li.Selected = true;
				messageListView.EndUpdate();
			}
			else if (e.KeyCode == Keys.C && e.Control)
			{
				StringBuilder copyLines = new StringBuilder();
				foreach (ListViewItem li in messageListView.SelectedItems)
				{
					if (copyLines.Length != 0)
						copyLines.Append("\r\n");
					copyLines.Append(li.Text);
				}
				Clipboard.SetData(DataFormats.UnicodeText, copyLines.ToString());
			}
		}

		private void treeView_KeyDown(object sender, KeyEventArgs e)
		{
			if (sender is TreeView)
			{
				TreeView tmpTreeView = sender as TreeView;
				if (e.KeyCode == Keys.C && e.Control)
				{
					//Clipboard.SetData(DataFormats.UnicodeText, tmpTreeView.SelectedNode.Text);
					StringBuilder strBuilder = new StringBuilder();
					recursiveGenerateTreeNodeToString(tmpTreeView.SelectedNode, 0, ref strBuilder);
					Clipboard.SetData(DataFormats.UnicodeText, strBuilder);
				}
			}

		}
		private void recursiveGenerateTreeNodeToString(TreeNode _treeNode, int _depth, ref StringBuilder _strBuilder)
		{
			if (_strBuilder.Length != 0)
				_strBuilder.Append("\r\n");
			for (int spaceIdx = 0; spaceIdx < _depth; spaceIdx++)
				_strBuilder.Append("  ");
			_strBuilder.Append(_treeNode.Text);

			foreach (TreeNode childNode in _treeNode.Nodes)
			{
				recursiveGenerateTreeNodeToString(childNode, _depth + 1, ref _strBuilder);
			}

		}

	}
}