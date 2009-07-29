using System;
using System.Collections.Generic;
using System.Text;

namespace Solimar.Licensing.Attribs
{
	public class Lic_LicenseInfoAttribsHelper
	{
		public static string GetDisplayLabel(Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs _licInfoAttribs)
		{
			string displayLabel = "";
			if (_licInfoAttribs != null)
			{
				string licType = "U";
				switch (_licInfoAttribs.softwareLicType.TVal)
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
				displayLabel = string.Format("{0:x4}-{1:x3}-{2:x4}-{3}{4:d2}", _licInfoAttribs.customerID.TVal, _licInfoAttribs.destinationID.TVal, _licInfoAttribs.softwareGroupLicenseID.TVal, licType, _licInfoAttribs.softwareLicTypeIndex.TVal);
			}
			return displayLabel;
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
