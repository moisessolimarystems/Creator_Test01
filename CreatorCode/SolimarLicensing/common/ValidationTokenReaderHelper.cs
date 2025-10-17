using System;
using System.ComponentModel;
using System.Text;
using System.Reflection;
using System.IO;


namespace Solimar.Licensing.Common
{

	//The Validation Token Retriever.exe will generate a CSV file in the format:
	// "Field", "Value
	// Version 1.0.0.0 is in this format, this Class will take a csv file stream that can be
	//easily accessable through a map where the key is the different fields.  The keys "Field"
	//and "Version" aren't entered as keys.
	public class ValidationTokenReaderHelper
	{
		public ValidationTokenReaderHelper(System.IO.Stream _inputStream)
		{
			tokenMap = new System.Collections.Generic.SortedDictionary<string, string>();
			Solimar.Licensing.Common.CSVReader csvReader = new Solimar.Licensing.Common.CSVReader(_inputStream);
			foreach (System.Collections.Generic.List<string> csvItemList in csvReader.csvList)
			{
				if (csvItemList.Count == 0)
					continue;   //Ignore Empty Lines
				//should be only two items long in the format "field", "value"
				//ignore "field" and "version"
				//if (string.Compare(csvItemList[0], "field", true) == 0 || string.Compare(csvItemList[0], "version", true) == 0)
				if (string.Compare(csvItemList[0], "field", true) == 0)
					continue;	//Goto next item
				tokenMap.Add(csvItemList[0], csvItemList.Count > 1 ? csvItemList[1] : "");
			}
		}
		public System.Collections.Generic.SortedDictionary<string, string> tokenMap;
	}

	public class SolimarValidationData
	{
		public SolimarValidationData()
		{

		}

		[Category("Validation Token"),
		 Browsable(false),
		 ReadOnly(true),
		 DisplayName("Company Name")]
		public string CompanyName
		{
			get { return m_companyName; }
			set { m_companyName = value; }
		}
		[Category("Validation Token"),
		 ReadOnly(true),
		 DisplayName("Domain Name")]
		public string DomainName
		{
			get { return m_domainName; }
			set { m_domainName = value; }
		}
		[Category("Validation Token"),
		 ReadOnly(true),
		 DisplayName("Computer Name")]
		public string ComputerName
		{
			get { return m_computerName; }
			set { m_computerName = value; }
		}
		[Category("Validation Token"),
		 ReadOnly(true),
		 DisplayName("Bios Serial Number")]
		public string BiosSerialNumber
		{
			get { return m_biosSerialNumber; }
			set { m_biosSerialNumber = value; }
		}
		[Category("Validation Token"),
		 ReadOnly(true),
		 DisplayName("MAC Address")]
		public string macAddress
		{
			get { return m_macAddress; }
			set { m_macAddress = value; }
		}
		[Category("Validation Token"),
		 ReadOnly(true),
		 DisplayName("Operating System")]
		public string OperatingSystem
		{
			get { return m_operatingSystem; }
			set { m_operatingSystem = value; }
		}
		[Category("Validation Token"),
		 ReadOnly(true),
		 DisplayName("Part of Domain")]
		public string PartOfDomain
		{
			get { return m_bPartOfDomain; }
			set { m_bPartOfDomain = value; }
		}
		[Category("Validation Token"),
		 ReadOnly(true),
		 DisplayName("System Manufacturer")]
		public string SystemManufacturer
		{
			get { return m_systemManufacturer; }
			set { m_systemManufacturer = value; }
		}
		[Category("Validation Token"),
		 ReadOnly(true),
		 DisplayName("System Model")]
		public string SystemModel
		{
			get { return m_systemModel; }
			set { m_systemModel = value; }
		}
		[Category("Validation Token"),
		 ReadOnly(true),
		 DisplayName("System Type")]
		public string SystemType
		{
			get { return m_systemType; }
			set { m_systemType = value; }
		}
		[Category("Validation Token"),
		 ReadOnly(true),
		 DisplayName("System UUID")]
		public string SystemUUID
		{
			get { return m_systemUUID; }
			set { m_systemUUID = value; }
		}

		[Category("Validation Token"),
		 Browsable(false),
		 ReadOnly(true),
		 DisplayName("Version")]
		public string Version
		{
			get { return m_version; }
			set { m_version = value; }
		}

		private void SetBrowsableAttribStatus(string property, bool value)
		{
			PropertyDescriptor descriptor = TypeDescriptor.GetProperties(this.GetType())[property];
			BrowsableAttribute attrib = (BrowsableAttribute)descriptor.Attributes[typeof(BrowsableAttribute)];
			FieldInfo isBrowsable = attrib.GetType().GetField("browsable", BindingFlags.Instance | BindingFlags.NonPublic);
			isBrowsable.SetValue(attrib, value);
		}
		public bool FromCSV(string _input)
		{
			bool bSuccessImport = false;
			if (string.Compare(_input, string.Empty) != 0)
			{
				SetBrowsableAttribStatus("CompanyName", true);

				Solimar.Licensing.Common.ValidationTokenReaderHelper valTokenReader = new Solimar.Licensing.Common.ValidationTokenReaderHelper(new MemoryStream(System.Text.Encoding.UTF8.GetBytes(_input)));
				foreach (System.Collections.Generic.KeyValuePair<string, string> kvPair in valTokenReader.tokenMap)
				{
					if (string.Compare(kvPair.Key, "Company", true) == 0)
						m_companyName = kvPair.Value;
					else if (string.Compare(kvPair.Key, "Computer Name", true) == 0)
						m_computerName = kvPair.Value;
					else if (string.Compare(kvPair.Key, "Domain Name", true) == 0)
						m_domainName = kvPair.Value;
					else if (string.Compare(kvPair.Key, "Bios Serial Number", true) == 0)
						m_biosSerialNumber = kvPair.Value;
					else if (string.Compare(kvPair.Key, "Mac Address", true) == 0)
						m_macAddress = kvPair.Value;
					else if (string.Compare(kvPair.Key, "Operating System", true) == 0)
						m_operatingSystem = kvPair.Value;
					else if (string.Compare(kvPair.Key, "Part of Domain", true) == 0)
						m_bPartOfDomain = kvPair.Value;
					else if (string.Compare(kvPair.Key, "System Manufacturer", true) == 0)
						m_systemManufacturer = kvPair.Value;
					else if (string.Compare(kvPair.Key, "System Model", true) == 0)
						m_systemModel = kvPair.Value;
					else if (string.Compare(kvPair.Key, "System Type", true) == 0)
						m_systemType = kvPair.Value;
					else if (string.Compare(kvPair.Key, "System Product UUID", true) == 0)
						m_systemUUID = kvPair.Value;
					else if (string.Compare(kvPair.Key, "Version", true) == 0)
						m_version = kvPair.Value;

				}
			}
			return bSuccessImport;
		}

		public string ToCSV()
		{
			StringBuilder csvStrBuilder = new StringBuilder();
			csvStrBuilder.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe("Field"));
			csvStrBuilder.Append(",");
			csvStrBuilder.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe("Value"));
			csvStrBuilder.Append("\r\n");

			csvStrBuilder.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe("Version"));
			csvStrBuilder.Append(",");
			csvStrBuilder.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe(m_version));
			csvStrBuilder.Append("\r\n");

			csvStrBuilder.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe("Company"));
			csvStrBuilder.Append(",");
			csvStrBuilder.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe(m_companyName));
			csvStrBuilder.Append("\r\n");

			csvStrBuilder.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe("Computer Name"));
			csvStrBuilder.Append(",");
			csvStrBuilder.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe(m_computerName));
			csvStrBuilder.Append("\r\n");

			csvStrBuilder.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe("Domain Name"));
			csvStrBuilder.Append(",");
			csvStrBuilder.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe(m_domainName));
			csvStrBuilder.Append("\r\n");

			csvStrBuilder.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe("Bios Serial Number"));
			csvStrBuilder.Append(",");
			csvStrBuilder.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe(m_biosSerialNumber));
			csvStrBuilder.Append("\r\n");

			csvStrBuilder.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe("Mac Address"));
			csvStrBuilder.Append(",");
			csvStrBuilder.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe(m_macAddress));
			csvStrBuilder.Append("\r\n");

			csvStrBuilder.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe("Operating System"));
			csvStrBuilder.Append(",");
			csvStrBuilder.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe(m_operatingSystem));
			csvStrBuilder.Append("\r\n");

			csvStrBuilder.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe("Part of Domain"));
			csvStrBuilder.Append(",");
			csvStrBuilder.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe(m_bPartOfDomain));
			csvStrBuilder.Append("\r\n");

			csvStrBuilder.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe("System Manufacturer"));
			csvStrBuilder.Append(",");
			csvStrBuilder.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe(m_systemManufacturer));
			csvStrBuilder.Append("\r\n");

			csvStrBuilder.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe("System Model"));
			csvStrBuilder.Append(",");
			csvStrBuilder.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe(m_systemModel));
			csvStrBuilder.Append("\r\n");

			csvStrBuilder.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe("System Type"));
			csvStrBuilder.Append(",");
			csvStrBuilder.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe(m_systemType));
			csvStrBuilder.Append("\r\n");

			csvStrBuilder.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe("System Product UUID"));
			csvStrBuilder.Append(",");
			csvStrBuilder.Append(Solimar.Licensing.Common.CSVHelper.MakeCsvSafe(m_systemUUID));
			csvStrBuilder.Append("\r\n");

			return csvStrBuilder.ToString();
		}

		private string m_companyName = "";
		private string m_domainName = "";
		private string m_bPartOfDomain = "";
		private string m_computerName = "";
		private string m_macAddress = "";
		private string m_biosSerialNumber = "";
		private string m_systemUUID = "";
		private string m_systemManufacturer = "";
		private string m_systemModel = "";
		private string m_systemType = "";
		private string m_operatingSystem = "";
		private string m_version = "1.0.0.0";


	}
}