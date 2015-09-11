using System;
using System.Collections.Generic;
using System.Text;

namespace Solimar.Licensing.Attribs
{
	public class Lic_LicenseInfoAttribsHelper
	{
		public static string GenerateLicenseServerName(int _customerNumber, int _destinationNumber, int _groupNumber/*, Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType _eSWLType, int _typeIndex*/)
		{
			string displayLabel = "";
			//string licType = "U";
			//switch (_eSWLType)
			//{
			//    case Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltFailover:
			//        licType = "F";
			//        break;
			//    case Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltDisasterRecovery:
			//        licType = "D";
			//        break;
			//    case Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltPerpetual:
			//        licType = "P";
			//        break;
			//    case Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltSubscription:
			//        licType = "S";
			//        break;
			//    case Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltTestDev:
			//        licType = "T";
			//        break;
			//    default:
			//        //All other types 
			//        licType = "S";
			//        break;
			//};
			//displayLabel = string.Format("{0:x4}-{1:x4}-{2}{3:d2}", _licInfoAttribs.customerID.TVal, _licInfoAttribs.softwareGroupLicenseID.TVal, licType, _licInfoAttribs.softwareLicTypeIndex.TVal);
			/*
			displayLabel = string.Format("{0:x3}-{1}-{2}-{3}{4}",
				_customerNumber,
				(destId_Base36.Length == 1) ? "0" + destId_Base36 : destId_Base36,
				(swGrpId_Base36.Length == 1) ? "0" + swGrpId_Base36 : swGrpId_Base36,
				(licType==string.Empty) ? "" : licType + "-",
				_typeIndex);
			*/
			//displayLabel = string.Format("{0:x3}-{1}-{2}-{3}",
			//    _customerNumber,
			//    (destId_Base36.Length == 1) ? "0" + destId_Base36 : destId_Base36,
			//    (swGrpId_Base36.Length == 1) ? "0" + swGrpId_Base36 : swGrpId_Base36,
			//    licType);
			string destId_Base36 = ConvertToBase36(_destinationNumber);
			string swGrpId_Base36 = ConvertToBase36(_groupNumber);
			if (swGrpId_Base36.Length == 1)
				swGrpId_Base36 = "00" + swGrpId_Base36;
			else if (swGrpId_Base36.Length == 2)
				swGrpId_Base36 = "0" + swGrpId_Base36;

			displayLabel = string.Format("{0:x3}-{1}-{2}",
				_customerNumber,
				(destId_Base36.Length == 1) ? "0" + destId_Base36 : destId_Base36,
				swGrpId_Base36);
			//(swGrpId_Base36.Length == 1) ? "0" + swGrpId_Base36 : swGrpId_Base36,
			//licType);

			return displayLabel;
		}
		public static string GetDisplayLabel(Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs _licInfoAttribs)
		{
			return (_licInfoAttribs == null) ? "" : GenerateLicenseServerName((int)_licInfoAttribs.customerID.TVal, (int)_licInfoAttribs.destinationID.TVal, (int)_licInfoAttribs.softwareGroupLicenseID.TVal/*, _licInfoAttribs.softwareLicType.TVal, (int)_licInfoAttribs.softwareLicTypeIndex.TVal*/);
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
					productName = (productSpec.productLabel.TVal.Length > 0) ? productSpec.productLabel : productSpec.productName;
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


        //// C# only function
        //public static string[] GetContractNumbers(Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs _prodInfoAttribs)
        //{
        //    System.Collections.Generic.List<string> contractNumberList = new List<string>();
        //    foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs moduleInfo in _prodInfoAttribs.moduleList.TVal)
        //    {
        //        if (moduleInfo.contractNumber.TVal.Length!=0 && contractNumberList.Contains(moduleInfo.contractNumber.TVal) == false)
        //            contractNumberList.Add(moduleInfo.contractNumber.TVal);
        //    }
        //    return contractNumberList.ToArray();
        //}

        //// C# only function
        //public static Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs.Lic_ModuleInfoAttribsList GetModuleList_ByContractNumber(Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs _prodInfoAttribs, string _contractNumber)
        //{
        //    Lic_PackageAttribs.Lic_ProductInfoAttribs.Lic_ModuleInfoAttribsList moduleList = new Lic_PackageAttribs.Lic_ProductInfoAttribs.Lic_ModuleInfoAttribsList("modList", new System.Collections.ArrayList());
        //    foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ModuleInfoAttribs moduleInfo in _prodInfoAttribs.moduleList.TVal)
        //    {
        //        if (string.Compare(moduleInfo.contractNumber.TVal, _contractNumber, true) == 0)
        //            moduleList.TVal.Add(moduleInfo);
        //    }
        //    return moduleList;
        //}


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
        
        // C# only function
        public static bool GenerateActivitySlotHistoryInfo(ref Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs _licInfoAttribs1)
        {
            bool bSuccess = false;
            Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ActivitySlotHistoryInfoAttribs activitySlotHistInfo = new Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ActivitySlotHistoryInfoAttribs();
            Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ActivitySlotChangeInfoAttribs slotChangesInfo = null;
            
            //Cycle through last list of ActivitySlotIds to ContractNumbers
            SortedDictionary<string/*ContractNumber*/, ushort/*ActivitySlotId*/> previousContractNumberToActivitySlotId = new SortedDictionary<string, ushort>();
            foreach (Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ActivitySlotInfoAttribs activitySlotInfo in _licInfoAttribs1.activitySlotList.TVal)
                previousContractNumberToActivitySlotId[activitySlotInfo.contractNumber.TVal] = activitySlotInfo.activitySlotID.TVal;

            //Cycle through all the ProductLicenses of current LicenseInfo object, remember all that have activations, and if they 
            //are overriding current activations or hours to expire
            SortedDictionary<string/*contractNumber*/, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs> contractNumberToProdInfo = new SortedDictionary<string, Lic_PackageAttribs.Lic_ProductInfoAttribs>();
            foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs prodInfo in _licInfoAttribs1.productList.TVal)
            {
                if (prodInfo.bUseActivations.TVal == true)
                    contractNumberToProdInfo[prodInfo.contractNumber] = prodInfo;
            }

            #region Generate add, remove and contract number lists
            List<string> removeContractNumberList = new List<string>();
            List<string> addContractNumberList = new List<string>();
            foreach (string contractNumber in previousContractNumberToActivitySlotId.Keys)
            {
                if (contractNumberToProdInfo.ContainsKey(contractNumber) == false)
                    removeContractNumberList.Add(contractNumber);
            }
            foreach (string contractNumber in contractNumberToProdInfo.Keys)
            {
                if(previousContractNumberToActivitySlotId.ContainsKey(contractNumber) == false)
                    addContractNumberList.Add(contractNumber);
            }
            #endregion

            #region Cycle through remove list, and do remove changes
            List<ushort/*ActivitySlotId*/> openActivitySlots = new List<ushort>();
            foreach (string removeContractNumber in removeContractNumberList)
            {
                if (previousContractNumberToActivitySlotId.ContainsKey(removeContractNumber))
                {
                    slotChangesInfo = new Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ActivitySlotChangeInfoAttribs();
                    slotChangesInfo.actionType.TVal = Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ActivitySlotChangeInfoAttribs.TActivitySlotChangeActionType.ascaDelete;
                    slotChangesInfo.contractNumber.TVal = removeContractNumber;
                    slotChangesInfo.param1.TVal = previousContractNumberToActivitySlotId[removeContractNumber];
                    activitySlotHistInfo.activitySlotChangeInfoList.TVal.Add(slotChangesInfo);

                    openActivitySlots.Add(previousContractNumberToActivitySlotId[removeContractNumber]);
                    previousContractNumberToActivitySlotId.Remove(removeContractNumber);
                    
                }
            }
            openActivitySlots.Sort();
            #endregion

            SortedDictionary<ushort/*ActivitySlotId*/, string/*ContractNumber*/> newActivitySlotIdToContractNumber = new SortedDictionary<ushort, string>();
            foreach (KeyValuePair<string, ushort> kvPair in previousContractNumberToActivitySlotId)
            {
                newActivitySlotIdToContractNumber.Add(kvPair.Value, kvPair.Key);
            }
            foreach (string addContractNumber in addContractNumberList)
            {
                if (openActivitySlots.Count > 0)    // There is a existing slot to insert into
                {
                    slotChangesInfo = new Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ActivitySlotChangeInfoAttribs();
                    slotChangesInfo.actionType.TVal = Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ActivitySlotChangeInfoAttribs.TActivitySlotChangeActionType.ascaAdd;
                    slotChangesInfo.contractNumber.TVal = addContractNumber;
                    slotChangesInfo.param1.TVal = openActivitySlots[0];
                    activitySlotHistInfo.activitySlotChangeInfoList.TVal.Add(slotChangesInfo);

                    newActivitySlotIdToContractNumber.Add(openActivitySlots[0], addContractNumber);
                    openActivitySlots.RemoveAt(0);
                }
                else // Append to the end of the List
                {
                    slotChangesInfo = new Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ActivitySlotChangeInfoAttribs();
                    slotChangesInfo.actionType.TVal = Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ActivitySlotChangeInfoAttribs.TActivitySlotChangeActionType.ascaAdd;
                    slotChangesInfo.contractNumber.TVal = addContractNumber;
                    slotChangesInfo.param1.TVal = (ushort)newActivitySlotIdToContractNumber.Count;
                    activitySlotHistInfo.activitySlotChangeInfoList.TVal.Add(slotChangesInfo);

                    newActivitySlotIdToContractNumber.Add((ushort)newActivitySlotIdToContractNumber.Count, addContractNumber);
                }
            }

            #region if there are any open activity slots, push all other items up, start with items at the end of the list to minimize changes
            ushort[] newSlotIdList = new ushort[newActivitySlotIdToContractNumber.Keys.Count];  //ordered list with biggest indexed items at end
            newActivitySlotIdToContractNumber.Keys.CopyTo(newSlotIdList, 0);
            for (   int idx = newSlotIdList.Length - 1; 
                    idx >= 0 && openActivitySlots.Count > 0; 
                    idx--)
            {
                if (newSlotIdList[idx] > openActivitySlots[0])
                {
                    slotChangesInfo = new Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ActivitySlotChangeInfoAttribs();
                    slotChangesInfo.actionType.TVal = Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ActivitySlotChangeInfoAttribs.TActivitySlotChangeActionType.ascaMove;
                    slotChangesInfo.contractNumber.TVal = newActivitySlotIdToContractNumber[newSlotIdList[idx]];
                    slotChangesInfo.param1.TVal = newSlotIdList[idx];   // Old activity slot id
                    slotChangesInfo.param2.TVal = openActivitySlots[0]; // New activity slot id
                    activitySlotHistInfo.activitySlotChangeInfoList.TVal.Add(slotChangesInfo);
                    newActivitySlotIdToContractNumber.Remove(newSlotIdList[idx]);
                    newActivitySlotIdToContractNumber.Add(openActivitySlots[0], slotChangesInfo.contractNumber.TVal);
                    openActivitySlots.RemoveAt(0);
                }
                else
                {
                    break;
                }
            }
            #endregion
            #region add all override changes
            foreach (KeyValuePair<string/*contractNumber*/, Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_ProductInfoAttribs> kvPair in contractNumberToProdInfo)
            {
                if (kvPair.Value.bActivationOverrideCurrent.TVal == true || kvPair.Value.bActivationOverrideCurrentHoursToExpire.TVal == true)
                {
                    #region Find Activation Slot for contract number
                    int activationSlot = -1;
                    foreach (KeyValuePair<ushort, string> newActivitySlotIdToContractNumberPair in newActivitySlotIdToContractNumber)
                    {
                        if (string.Compare(kvPair.Key, newActivitySlotIdToContractNumberPair.Value, true) == 0)
                        {
                            activationSlot = (int)newActivitySlotIdToContractNumberPair.Key;
                            break;
                        }
                    }
                    #endregion
                    if (activationSlot != -1)
                    {
                        if (kvPair.Value.bActivationOverrideCurrent.TVal == true)
                        {
                            //For new ContactNumbers just added, don't add command to set to 0, it is automatically 0
                            if (!((ushort)kvPair.Value.activationOverrideCurrent.TVal == 0 && previousContractNumberToActivitySlotId.ContainsKey(kvPair.Key) == false))
                            {
                                slotChangesInfo = new Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ActivitySlotChangeInfoAttribs();
                                slotChangesInfo.actionType.TVal = Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ActivitySlotChangeInfoAttribs.TActivitySlotChangeActionType.ascaSetActivations;
                                slotChangesInfo.contractNumber.TVal = kvPair.Key;
                                slotChangesInfo.param1.TVal = (ushort)activationSlot;
                                slotChangesInfo.param2.TVal = (ushort)kvPair.Value.activationOverrideCurrent.TVal;
                                activitySlotHistInfo.activitySlotChangeInfoList.TVal.Add(slotChangesInfo);
                            }

                            kvPair.Value.activationOverrideCurrent.TVal = 0;
                            kvPair.Value.bActivationOverrideCurrent.TVal = false;
                        }
                        if (kvPair.Value.bActivationOverrideCurrentHoursToExpire.TVal == true)
                        {
                            //For new ContactNumbers just added, don't add command to set to 0, it is automatically 0
                            if (!((ushort)kvPair.Value.activationOverrideCurrentHoursToExpire.TVal == 0 && previousContractNumberToActivitySlotId.ContainsKey(kvPair.Key) == false))
                            {
                                slotChangesInfo = new Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ActivitySlotChangeInfoAttribs();
                                slotChangesInfo.actionType.TVal = Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ActivitySlotChangeInfoAttribs.TActivitySlotChangeActionType.ascaSetHoursToExpire;
                                slotChangesInfo.contractNumber.TVal = kvPair.Key;
                                slotChangesInfo.param1.TVal = (ushort)activationSlot;
                                slotChangesInfo.param2.TVal = (ushort)kvPair.Value.activationOverrideCurrentHoursToExpire.TVal;
                                activitySlotHistInfo.activitySlotChangeInfoList.TVal.Add(slotChangesInfo);
                            }

                            kvPair.Value.activationOverrideCurrentHoursToExpire.TVal = 0;
                            kvPair.Value.bActivationOverrideCurrentHoursToExpire.TVal = false;
                        }
                    }
                }
            }
            #endregion

            //Update History Node - Only add if there are changes
            if (activitySlotHistInfo.activitySlotChangeInfoList.TVal.Count > 0)
            {
                activitySlotHistInfo.historyNumber.TVal = (ushort)(_licInfoAttribs1.activitySlotHistoryList.TVal.Count + 1);
                _licInfoAttribs1.activitySlotHistoryList.TVal.Add(activitySlotHistInfo);
            }
            //Update Activity Slot List
            _licInfoAttribs1.activitySlotList.TVal.Clear();
            foreach (KeyValuePair<ushort, string> kvPair in newActivitySlotIdToContractNumber)
            {
                Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ActivitySlotInfoAttribs tmpSlotAttribs = new Lic_PackageAttribs.Lic_LicenseInfoAttribs.Lic_ActivitySlotInfoAttribs();
                tmpSlotAttribs.activitySlotID.TVal = (ushort)kvPair.Key;
                tmpSlotAttribs.contractNumber.TVal = kvPair.Value;
                _licInfoAttribs1.activitySlotList.TVal.Add(tmpSlotAttribs);
//System.Diagnostics.Trace.WriteLine(string.Format("Slot: {0}, ContractNumber {1}", tmpSlotAttribs.activitySlotID.TVal, tmpSlotAttribs.contractNumber.TVal));
            }
            bSuccess = true;
            return bSuccess;
        }
        public static bool RemoveUnusedExcess(ref Solimar.Licensing.Attribs.Lic_PackageAttribs.Lic_LicenseInfoAttribs _licInfoAttribs1)
        {
            bool bSuccess = false;
            try
            {
                foreach (Lic_PackageAttribs.Lic_ProductInfoAttribs prodInfo in _licInfoAttribs1.productList.TVal)
                {
                    //Lic_PackageAttribs.Lic_ModuleInfoAttribs modInfo in prodInfo.moduleList.TVal
                    int modIdx = 0;
                    while (modIdx < prodInfo.moduleList.TVal.Count)
                    {
                        Lic_PackageAttribs.Lic_ModuleInfoAttribs modInfo = (Lic_PackageAttribs.Lic_ModuleInfoAttribs)prodInfo.moduleList.TVal[modIdx];
                        if (modInfo.moduleValue.TVal == 0)
                        {
                            prodInfo.moduleList.TVal.RemoveAt(modIdx);
                            modIdx = 0;
                        }
                        else
                            modIdx++;
                    }
                }
                bSuccess = true;
            }
            catch (Exception)
            {
            }
            return bSuccess;
        }
        private static bool InitializeDate(DateTime _dateTime)
        {
            return (DateTime.Compare(new DateTime(1900, 1, 1), _dateTime) != 0);
        }
        
	}
}
