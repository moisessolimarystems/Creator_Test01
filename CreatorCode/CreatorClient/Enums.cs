using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Solimar.Licensing.Attribs;

namespace Client.Creator
{
        #region Enums

        enum ViewState
        {
            Customer,
            LicenseServer
        }
             
        enum TokenStatus
        {
            Reserved,
            Active,
            Deactivated
        }

        enum PacketListViewState
        {
            ALL,
            VERIFIED,
            UNVERIFIED
        }

        enum TransactionType
        {
            License,
            Order,
            Product
        }

        enum DetailTreeViewIconList
        {
            ICONVERT,
            RUBIKA,
            RUBIKAPROCESSBUILDER,
            SDXDESIGNER,
            SOLINDEXER,
            SOLSEARCHERENTERPRISESINGLEPLATFORM,
            SOLSEARCHERENTERPRISE,
            SOLFUSION,
            SOLSCRIPT,
            SPDEQUEUEMANAGER,
            SPDESYSTEMMANAGER,
            SPDE,
            SOLITRACK,
            SPD,
            CUSTOMER,
            LICENSE,
            LICENSEUPDATE,
            LICENSEDISABLED,
            LICENSERESYNC,
            LICENSEDEACTIVATED,
            ORDER,
            ADDONORDER,
            EMPTY
        }

        enum ListViewIconList
        {
            VERIFIED,
            UNVERIFIED,
            LISTBULLETS,
            TRANSACTION_LOGFILE,
            ZIPPEDFILE,
            TTCOMPUTERNAME,
            TTHARDWAREKEYID,
            TTMACADDRESS,
            TTBIOSSERIALNUMBER,
            TTLICENSECODE,
            MODULE,
            VALIDATIONTOKENS
        }

        public enum LicenseServerType
        {
            Perpetual,
            Failover,
            DisasterRecovery,
            TestDevelopment,
            Subscription,
            Deactivated
        }

        public enum ProductLicenseState
        {
            Trial,
            Licensed,
            AddOn,
            Deactivated
        }

        public class Enums
        {
            public static int GetDetailTreeViewIconIndex(String key)
            {
                DetailTreeViewIconList iconIndex;
                try
                {
                    iconIndex = (DetailTreeViewIconList)Enum.Parse(typeof(DetailTreeViewIconList), key.Replace(" ", ""), true);
                }
                catch (Exception ex)
                {
                    return -1;
                }
                return (int)iconIndex;
            }

            public static int GetListViewIconIndex(String key)
            {
                ListViewIconList iconIndex;
                try
                {
                    iconIndex = (ListViewIconList)Enum.Parse(typeof(ListViewIconList), key.Replace(" ", ""), true);
                }
                catch (Exception ex)
                {
                    return -1;
                }
                return (int)iconIndex;
            }

            public static int GetLicenseServerType(LicenseServerType lsType)
            {
                int licenseType;
                switch (lsType)
                {
                    case LicenseServerType.DisasterRecovery:
                        licenseType = (int)Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltDisasterRecovery;
                        break;
                    case LicenseServerType.Failover:
                        licenseType = (int)Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltFailover;
                        break;
                    case LicenseServerType.Perpetual:
                        licenseType = (int)Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltPerpetual;
                        break;
                    case LicenseServerType.Subscription:
                        licenseType = (int)Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltSubscription;
                        break;
                    case LicenseServerType.TestDevelopment:
                        licenseType = (int)Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltTestDev;
                        break;
                    default: licenseType = -1; break;
                }
                return licenseType;
            }

            public static int GetProductLicenseState(ProductLicenseState plState)
            {
                int productLicenseState;
                switch (plState)
                {
                    case ProductLicenseState.Trial:
                        productLicenseState = (int)Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msTrial;
                        break;
                    case ProductLicenseState.AddOn:
                        productLicenseState = (int)Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msAddOn;
                        break;
                    case ProductLicenseState.Licensed:
                        productLicenseState = (int)Lic_PackageAttribs.Lic_ModuleInfoAttribs.TModuleState.msLicensed;
                        break;
                    default: productLicenseState = -1; break;
                }
                return productLicenseState;
            }

            public static String GetLicenseServerPrefix(LicenseServerType licType)
            {
                String prefix = "";
                if (licType != LicenseServerType.Deactivated)
                    prefix = licType.ToString().Substring(0, 1);
                return prefix;
            }
        }
        #endregion
}
