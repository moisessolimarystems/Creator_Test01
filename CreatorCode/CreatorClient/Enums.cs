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
            Token                = 0,           //License Server Transaction Types
            LicenseServerStatus  = 1,
            ProductLicense       = 2,             
            PacketVerification   = 3,
            Version              = 10, //Product License Transaction Types
            ExpirationDate       = 11, 
            Module               = 12,
            ProductLicenseStatus = 13,
            ApplicationInstance  = 14,
            Activation           = 15
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
            Standard,
            Failover,
            Deactivated
        }

        public enum ProductLicenseState
        {
            Trial,
            Licensed,
            AddOn,
            Deactivated
        }

        public enum LicenseLimitation
        {
            ProductLicense=20,
            Activations=254,
            ActivationDays=170
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
                    //case LicenseServerType.DisasterRecovery:
                    //    licenseType = (int)Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltDisasterRecovery;
                    //    break;
                    case LicenseServerType.Failover:
                        licenseType = (int)Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltFailover;
                        break;
                    case LicenseServerType.Standard:
                        licenseType = (int)Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltPerpetual;
                        break;
                    //case LicenseServerType.Subscription:
                    //    licenseType = (int)Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltSubscription;
                    //    break;
                    //case LicenseServerType.TestDevelopment:
                    //    licenseType = (int)Lic_PackageAttribs.Lic_LicenseInfoAttribs.TSoftwareLicenseType.sltTestDev;
                    //    break;
                    default: licenseType = -1; break;
                }
                return licenseType;
            }

            public static Lic_PackageAttribs.Lic_ProductInfoAttribs.TProductState GetProductState(ProductLicenseState plState)
            {
                Lic_PackageAttribs.Lic_ProductInfoAttribs.TProductState productLicenseState = Lic_PackageAttribs.Lic_ProductInfoAttribs.TProductState.psTrial;
                switch (plState)
                {
                    case ProductLicenseState.Trial:
                        productLicenseState = Lic_PackageAttribs.Lic_ProductInfoAttribs.TProductState.psTrial;
                        break;
                    case ProductLicenseState.AddOn:
                        productLicenseState = Lic_PackageAttribs.Lic_ProductInfoAttribs.TProductState.psAddOn;
                        break;
                    case ProductLicenseState.Licensed:
                        productLicenseState = Lic_PackageAttribs.Lic_ProductInfoAttribs.TProductState.psLicensed;
                        break;
                }
                return productLicenseState;
            }

            public static ProductLicenseState GetProductLicenseState(Lic_PackageAttribs.Lic_ProductInfoAttribs.TProductState plState)
            {
                ProductLicenseState productLicenseState = ProductLicenseState.Deactivated;
                switch (plState)
                {
                    case Lic_PackageAttribs.Lic_ProductInfoAttribs.TProductState.psTrial:
                        productLicenseState = ProductLicenseState.Licensed;
                        break;
                    case Lic_PackageAttribs.Lic_ProductInfoAttribs.TProductState.psAddOn:
                        productLicenseState = ProductLicenseState.AddOn;
                        break;
                    case Lic_PackageAttribs.Lic_ProductInfoAttribs.TProductState.psLicensed:
                        productLicenseState = ProductLicenseState.Licensed;
                        break;
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
