using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace LicenseMixedModeTesterUI
{
	public partial class SingleParameterControl : UserControl
	{
		public SingleParameterControl()
		{
			InitializeComponent();
		}

		public void SetItem(System.Reflection.ParameterInfo paramInfo)
		{
			paramLabel.Text = paramInfo.Name;
			//paramInfo.
			//paramInfo
		}
		public string GetItem()
		{
			return paramTextBox.Text;
		}
	}
}
