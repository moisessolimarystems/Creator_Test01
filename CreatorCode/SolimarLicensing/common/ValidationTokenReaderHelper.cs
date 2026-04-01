using System;


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
				//should be only two items long in the format "field", "value"
				//ignore "field" and "version"
				if (string.Compare(csvItemList[0], "field", true) == 0 || string.Compare(csvItemList[0], "version", true) == 0)
					continue;	//Goto next item
				tokenMap.Add(csvItemList[0], csvItemList[1]);
			}
		}
		public System.Collections.Generic.SortedDictionary<string, string> tokenMap;
	}
}