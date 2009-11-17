using System;
using System.Collections.Generic;
using System.Text;

namespace Solimar.Licensing.Attribs
{
	public class Lic_LicenseInfoAttribsHelper
	{
		public static string GenerateLicenseServerName(int _customerNumber, int _destinationNumber, int _groupNumber, Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType _eSWLType, int _typeIndex)
		{
			string displayLabel = "";
			string licType = "U";
			switch (_eSWLType)
			{
				case Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltFailover:
					licType = "F";
					break;
				case Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltDisasterRecovery:
					licType = "D";
					break;
				case Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltPerpetual:
					licType = "P";
					break;
				case Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltSubscription:
					licType = "S";
					break;
				case Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltTestDev:
					licType = "T";
					break;
				default:
					break;
			};
			//displayLabel = string.Format("{0:x4}-{1:x4}-{2}{3:d2}", _licInfoAttribs.customerID.TVal, _licInfoAttribs.softwareGroupLicenseID.TVal, licType, _licInfoAttribs.softwareLicTypeIndex.TVal);
			string destId_Base36 = ConvertToBase36(_destinationNumber);
			string swGrpId_Base36 = ConvertToBase36(_groupNumber);
			displayLabel = string.Format("{0:x3}-{1}-{2}-{3}{4}",
				_customerNumber,
				(destId_Base36.Length == 1) ? "0" + destId_Base36 : destId_Base36,
				(swGrpId_Base36.Length == 1) ? "0" + swGrpId_Base36 : swGrpId_Base36,
				licType,
				_typeIndex);

			return displayLabel;
		}
		public static string GetDisplayLabel(Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs _licInfoAttribs)
		{
			return (_licInfoAttribs == null) ? "" : GenerateLicenseServerName((int)_licInfoAttribs.customerID.TVal, (int)_licInfoAttribs.destinationID.TVal, (int)_licInfoAttribs.softwareGroupLicenseID.TVal, _licInfoAttribs.softwareLicType.TVal, (int)_licInfoAttribs.softwareLicTypeIndex.TVal);
		}
		public static string GenerateProductLicenseName(string _name, int _productLicenseIndex)
		{
			return string.Format("{0}-{1}", _name, ConvertToBase36(_productLicenseIndex));
		}
		private static string ConvertToBase36(int _base10Number)
		{
			String newNumber = "";
			String chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
			//String chars = "0123456789abcdefghijklmnopqrstuvwxyz";
			while (_base10Number >= 36)
			{
				int r = _base10Number % 36;
				newNumber = chars[r] + newNumber;
				_base10Number = _base10Number / 36;
			}
			// the last number to convert
			newNumber = chars[_base10Number] + newNumber;

			return newNumber;
		}
		private static Int64 ConvertFromBase36(string _base36Number)
		{
			string base36Chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
			char[] arrInput = _base36Number.ToCharArray();
			Array.Reverse(arrInput);
			Int64 returnValue = 0;
			for (int i = 0; i < arrInput.Length; i++)
			{
				int valueindex = base36Chars.IndexOf(arrInput[i]);
				returnValue += Convert.ToInt64(valueindex * Math.Pow(36, i));
			}
			return returnValue;
		}
		private static bool SplitProductLicenseName(string _productLicenseName, out string _outName, out long _outProductLicenseIndex)
		{
			bool bSuccess = false;
			_outName = "";
			_outProductLicenseIndex = -1;
			if (_productLicenseName != string.Empty)
			{
				int lastIdx = _productLicenseName.LastIndexOf('-'); //returns -1 if not found
				if (lastIdx != -1)
				{
					_outName = _productLicenseName.Substring(0, lastIdx);
					_outProductLicenseIndex = ConvertFromBase36(_productLicenseName.Substring(lastIdx + 1, _productLicenseName.Length - (lastIdx + 1)));
					bSuccess = true;
				}
			}
			return bSuccess;
		}

		public static int CompareProductLicenseName(string _prodLicNumber1, string _prodLicNumber2)
		{
			int compareValue = -1000;
			string prodLic1 = "", prodLic2 = "";
			long num1 = -1, num2 = -1;
			if (SplitProductLicenseName(_prodLicNumber1, out prodLic1, out num1) && SplitProductLicenseName(_prodLicNumber2, out prodLic2, out num2))
			{
				compareValue = string.Compare(prodLic1, prodLic2, true);
				if (compareValue == 0)
					compareValue = (int)(num1 - num2);
			}

			return compareValue;
		}

		public static string GetValidationValue(Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_VerificationAttribs.Lic_ValidationTokenAttribsList _tokenList, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs.TTokenType _tokenType)
		{
			string tokenValue = "";
			foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs valToken in _tokenList.TVal)
			{
				if (valToken.tokenType.TVal == _tokenType)
				{
					tokenValue = valToken.tokenValue.TVal;
					break;
				}
			}
			return tokenValue;
		}

		public static string GetProductName(Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_SoftwareSpecAttribs paramSSoftwareSpec, uint paramProductID)
		{
			string productName = "";
			foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in paramSSoftwareSpec.productSpecMap.TVal.Values)
			{
				if (productSpec.productID.TVal == paramProductID)
					productName = productSpec.productName;
			}
			return productName;
		}
		public static string GetModuleName(Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_SoftwareSpecAttribs paramSSoftwareSpec, uint paramProductID, uint paramModuleID)
		{
			string moduleName = "";
			foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec in paramSSoftwareSpec.productSpecMap.TVal.Values)
			{
				if (productSpec.productID.TVal == paramProductID)
				{
					Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.Lic_ModuleSoftwareSpecAttribsMap moduleSpecMap = Solimar.Licensing.Attribs.Lic_LicenseInfoAttribsHelper.GetModuleList(paramSSoftwareSpec, paramProductID);
					if (moduleSpecMap != null)
					{
						foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleSoftwareSpecAttribs moduleSpec in moduleSpecMap.TVal.Values)
						{
							if (moduleSpec.moduleID == paramModuleID)
							{
								moduleName = moduleSpec.moduleName;
								break;
							}
						}
						if (moduleName.Length != 0)
							break;
					}
					
				}
			}
			return moduleName;
		}

		public static Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.Lic_ModuleSoftwareSpecAttribsMap GetModuleList(Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_SoftwareSpecAttribs paramSSoftwareSpec, uint paramProductID)
		{
			Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs.Lic_ModuleSoftwareSpecAttribsMap moduleMap = null;
			Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs productSpec = null;
			foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs currentProductSpec in paramSSoftwareSpec.productSpecMap.TVal.Values)
			{
				if (currentProductSpec.productID.TVal == paramProductID)
				{
					productSpec = currentProductSpec;
					break;
				}
			}
			if (productSpec != null)
			{
				if (productSpec.sameModSpecProductID.TVal != 0)
				{
					foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductSoftwareSpecAttribs currentProductSpec in paramSSoftwareSpec.productSpecMap.TVal.Values)
					{
						if (currentProductSpec.productID.TVal == productSpec.sameModSpecProductID.TVal)
						{
							moduleMap = currentProductSpec.moduleSpecMap;
							break;
						}
					}
				}
				else
				{
					moduleMap = productSpec.moduleSpecMap;
				}
			}
			return moduleMap;
		}


        // C# only function
        public static string[] GetContractNumbers(Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs _prodInfoAttribs)
        {
            System.Collections.Generic.List<string> contractNumberList = new List<string>();
            foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs moduleInfo in _prodInfoAttribs.moduleList.TVal)
            {
                if (moduleInfo.contractNumber.TVal.Length!=0 && contractNumberList.Contains(moduleInfo.contractNumber.TVal) == false)
                    contractNumberList.Add(moduleInfo.contractNumber.TVal);
            }
            return contractNumberList.ToArray();
        }

        // C# only function
        public static Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs.Lic_ModuleInfoAttribsList GetModuleList_ByContractNumber(Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs _prodInfoAttribs, string _contractNumber)
        {
            Lic_PackageAttribs.Lic_ProductInfoAttribs.Lic_ModuleInfoAttribsList moduleList = new Lic_PackageAttribs.Lic_ProductInfoAttribs.Lic_ModuleInfoAttribsList("modList", new System.Collections.ArrayList());
            foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs moduleInfo in _prodInfoAttribs.moduleList.TVal)
            {
                if (string.Compare(moduleInfo.contractNumber.TVal, _contractNumber, true) == 0)
                    moduleList.TVal.Add(moduleInfo);
            }
            return moduleList;
        }


        public static bool IsEqual(Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs _licInfoAttribs1, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs _licInfoAttribs2)
        {
            return string.Compare(_licInfoAttribs1.ToString(), _licInfoAttribs2.ToString(), true) == 0;
        }
        public static bool IsEqual(Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs _prodInfoAttribs1, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs _prodInfoAttribs2)
        {
            //_prodInfoAttribs1.moduleList
            return false;
        }
        public static bool IsEqual(Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs _modInfoAttribs1, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs _modInfoAttribs2)
        {
            return string.Compare(_modInfoAttribs1.ToString(), _modInfoAttribs2.ToString(), true) == 0;
        }
        public static bool IsEqual(Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs _valTokenAttribs1, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ValidationTokenAttribs _valTokenAttribs2)
        {
            //_valTokenAttribs2
            return false;
        }
        
	}
}
