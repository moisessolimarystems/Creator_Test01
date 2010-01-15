using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Solimar.Licensing.Attribs;

namespace Client.Creator
{
        #region Enums
            
        enum TokenStatus
        {
            Reserved,
            Active,
            Deactivated,
            Lost
        }

        enum PacketListViewState
        {
            ALL,
            VERIFIED,
            UNVERIFIED
        }

        enum TransactionType
        {
            Token                    = 0,           //License Server Transaction Types
            LicenseServer            = 1,          
            PacketVerification       = 2,
            Version                  = 10,          //Product License Transaction Types
            Module                   = 11,
            Status                   = 12,
            ExpirationDate           = 13,
            ActivationTotal          = 14,
            ActivationAmount         = 15,
            Extension                = 16,
            ProductConnection        = 17
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
            Deactivated
        }

        public enum ProductLicenseState
        {
            Trial,
            Licensed,
            AddOn,
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
