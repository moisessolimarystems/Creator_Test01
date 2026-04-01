using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace ValidationTokenRetriever
{
	public partial class Form1 : Form
	{
		public Form1()
		{
			InitializeComponent();
			Initialize();
		}

		private void Initialize()
		{
			RefreshState();
		}
		private void exitToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Application.Exit();
		}

		private void RefreshState()
		{
			try
			{
				compNameTextBox.Text = FindComputerName();
			}
			catch (Exception exc)
			{
				globalErrorProvider.SetError(compNameTextBox, exc.Message);
			}
			
			try
			{
				macTextBox.Text = FindMACAddress();
			}
			catch (Exception exc)
			{
				globalErrorProvider.SetError(macTextBox, exc.Message);
			}

			try
			{
				uuidTextBox.Text = FindSystemUuid();
			}
			catch (Exception exc)
			{
				globalErrorProvider.SetError(uuidTextBox, exc.Message);
			}

			try
			{
				biosTextBox.Text = FindBiosSerialNumber();
			}
			catch (Exception exc)
			{
				globalErrorProvider.SetError(biosTextBox, exc.Message);
			}
		}

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
						address = obj["MacAddress"].ToString();
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
			System.Management.ManagementObjectSearcher query1 = new System.Management.ManagementObjectSearcher("Select * FROM Win32_ComputerSystemProduct");
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
			System.Management.ManagementObjectSearcher query1 = new System.Management.ManagementObjectSearcher("Select * FROM Win32_BIOS");
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

			//System.Management.ManagementObjectSearcher query1 = new System.Management.ManagementObjectSearcher("Select * FROM Win32_BIOS");
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
			System.Windows.Forms.Clipboard.SetText(generateExportText());
		}

		private void exportToFile()
		{
			globalSaveFileDialog.FileName = MakeFileNameSafe(companyTextBox.Text) + "-" + MakeFileNameSafe(compNameTextBox.Text) + "-" + MakeFileNameSafe("solimarValidationTokens") + ".csv";
			if (globalSaveFileDialog.ShowDialog() == DialogResult.OK)
			{
				using (System.IO.TextWriter txtWriter = new System.IO.StreamWriter(globalSaveFileDialog.OpenFile()))
				{
					txtWriter.Write(generateExportText());
					txtWriter.Close();
				}
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
	}
}