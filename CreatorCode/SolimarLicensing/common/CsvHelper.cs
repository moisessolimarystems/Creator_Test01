using System;
using System.Text;

namespace Solimar.Licensing.Common
{
	public class CSVHelper
	{
		public static string MakeCsvSafe(string _inputString)
		{
			return _inputString != null ? "\"" + _inputString.Replace("\"", "\"\"") + "\"" : "";
		}
	}
	public class CSVReader
	{
		public CSVReader(System.IO.Stream _inputStream)
		{
			csvList = null;
			string line;
			using (System.IO.TextReader txtReader = new System.IO.StreamReader(_inputStream))
			{
				System.Collections.Generic.List<System.Collections.Generic.List<string>> tmpList = new System.Collections.Generic.List<System.Collections.Generic.List<string>>();
				int idx = 0;
				line = txtReader.ReadLine();
				while (line != null)
				{
					System.Collections.Generic.List<string> tmpStrList = new System.Collections.Generic.List<string>();
					tmpList.Add(LineToCsv(line));
					line = txtReader.ReadLine();
					idx++;
				}
				csvList = tmpList;
			}
		}
		private System.Collections.Generic.List<string> LineToCsv(string _line)
		{
			bool bStartQuote = false;
			bool bAddToItem = false;
			bool bNextCsvItem = false;

			System.Collections.Generic.List<string> csvList = new System.Collections.Generic.List<string>();
			StringBuilder csvItem = new StringBuilder();
			char? currentCh = null;
			char? lastCh = null;
			for (int idx = 0; idx < _line.Length; idx++)
			{
				currentCh = _line[idx];
				if (currentCh == '"')
				{
					if (bStartQuote == false)
					{
						if (lastCh == '"')
						{
							bStartQuote = true;
							bAddToItem = true;
						}
						else
						{
							bStartQuote = true;
							bAddToItem = false;
						}
					}
					else //if(bStartQuote == true)
					{
						bStartQuote = false;
						bAddToItem = false;
					}
				}
				else if (currentCh == ',')
				{
					if (bStartQuote == true)
					{
						bAddToItem = true;
					}
					else //bStartQuote == false
					{
						bNextCsvItem = true;
					}
				}
				else if (currentCh == '\n')
				{
					bAddToItem = true;
				}
				else if (currentCh == ' ')
				{
					if (lastCh == null)
						bAddToItem = false;
					else
						bAddToItem = true;
				}
				else
				{
					bAddToItem = true;
				}

				if (bAddToItem == true)
				{
					csvItem.Append(currentCh);
				}
				if (bNextCsvItem == true)
				{
					csvList.Add(csvItem.ToString());
					csvItem = new StringBuilder();
					bNextCsvItem = false;
				}

				lastCh = currentCh;
			}

			if (csvItem.Length != 0)
				csvList.Add(csvItem.ToString());

			return csvList;
		}
		public System.Collections.Generic.List<System.Collections.Generic.List<string>> csvList;
	}
}
