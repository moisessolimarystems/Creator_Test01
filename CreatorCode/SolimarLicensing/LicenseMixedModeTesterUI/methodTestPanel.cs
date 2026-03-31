using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace LicenseMixedModeTesterUI
{
	public partial class methodTestPanel : UserControl
	{
		public methodTestPanel()
		{
			InitializeComponent();
		}

		public delegate void DelegateParamStringReturnsVoid(string message);

		public void TestMethod(IntPtr objectContext, System.Reflection.MethodInfo testMethodInfo, DelegateParamStringReturnsVoid messageFunction)
		{
			paramPanel.Controls.Clear();
			foreach (System.Reflection.ParameterInfo paramInfo in testMethodInfo.GetParameters())
			{
				SingleParameterControl singleParamCntl = new SingleParameterControl();
				singleParamCntl.Dock = DockStyle.Top;
				//paramInfo.Dock
				singleParamCntl.SetItem(paramInfo);
				paramPanel.Controls.Add(singleParamCntl);

			}
			//paramPanel

			//SingleParameterControl
		}
	}
}
