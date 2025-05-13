using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Solimar.Licensing.Attribs;
using System.ComponentModel;
using System.Reflection;

namespace Client.Creator
{
        #region Enums

        enum ProductLicenseStatus
        {
            Default,
            InActive,
            Expired
        }

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

        public enum TransactionType
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
            ProductConnection        = 17,
            Type                     = 18
        }

        enum IconList
        {
            ICONVERT = 0, 
            RUBIKA,
            RUBIKAPROCESSBUILDER,
            SDXDESIGNER,
            SOLIMARINDEXINGTOOLS,
            SOLSEARCHERENTERPRISESINGLEPLATFORM,
            SOLSEARCHERENTERPRISE,
            SOLFUSION,
            SOLSCRIPT,
            SPDEQUEUEMANAGER,
            SPDESYSTEMMANAGER,
            SPDE,
            SOLITRACK,
            SPD,
            LIBRARYSERVICES,
            XIMAGEPDF,
            SOLIMARUTILITIES,
            SPDEESSENTIALS,
            READYPDFCLIENT,
            SOLSEARCHERCLOUD,
            CUSTOMER,
            LICENSE,
            LICENSEUPDATE,
            LICENSEDISABLED,
            LICENSEDEACTIVATED,
            ORDER,
            ADDONORDER,
            EMPTY,
            VERIFIED,
            UNVERIFIED,
            LISTBULLETS,
            TRANSACTION_LOGFILE,
            ZIPPEDFILE,
            TTCOMPUTERNAME,
            TTHARDWAREKEYID,
            TTMACADDRESS,
            TTBIOSSERIALNUMBER,
            TTDOMAINNAME,
            TTOPERATINGSYSTEM,
            TTPARTOFDOMAIN,
            TTSYSTEMMANUFACTURER,
            TTSYSTEMMODEL,
            TTSYSTEMTYPE,
            TTSYSTEMUUID,
            TTLICENSECODE,
            MODULE,
            VALIDATIONTOKENS,
            REPORTS,
            SPDEREMOTECLIENTMANAGER,
            UNKNOWN
        }

        public enum LicenseServerType
        {
            Standard,
            Deactivated
        }

        public enum ProductLicenseState
        {
            [Description("Licensed")]
            Licensed,
            [Description("Trial")]
            Trial,
            [Description("Add-On")]
            AddOn,
            [Description("Licensed Add-On")]
            LicensedAddOn
        }

        public enum LicenseLimitation
        {
            ProductLicense=20,
            Activations=254,
            ActivationDays=170
        }

        public class Enums
        {
            public static string GetEnumDescription(Enum value)
            {
                FieldInfo fi = value.GetType().GetField(value.ToString());

                DescriptionAttribute[] attributes =
                    (DescriptionAttribute[])fi.GetCustomAttributes(
                    typeof(DescriptionAttribute),
                    false);

                if (attributes != null &&
                    attributes.Length > 0)
                    return attributes[0].Description;
                else
                    return value.ToString();
            }

        public static int GetIconIndex(String key)
        {
            IconList iconIndex;
            var cleanKey = key.Replace(" ", string.Empty);
            if (!Enum.TryParse(cleanKey, true, out iconIndex))
            {
                cleanKey = key.Replace("Essentials", string.Empty);
                if (!Enum.TryParse(cleanKey, true, out iconIndex))
                {
                    iconIndex = IconList.UNKNOWN;
                }
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
                String prefix = string.Empty;
                if (licType != LicenseServerType.Deactivated)
                    prefix = licType.ToString().Substring(0, 1);
                return prefix;
            }
        }
        #endregion
}
