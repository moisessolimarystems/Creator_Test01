using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data.Linq;

namespace CreatorData
{
    public partial class TokenTable
    {
        public static IList<TokenTable> GetAllTokens(string searchString, Byte token)
        {
            IList<TokenTable> list;
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                
                if (searchString.Length == 0)
                {
                    if (token != 0)
                        list = db.TokenTables.Where(c => c.TokenType.Equals(token)).OrderBy(t => t.LicenseID).ToList();
                    else
                        list = db.TokenTables.ToList();
                }
                else
                {
                    if (token != 0)
                    {
                        list = db.TokenTables.Where(c => c.TokenType.Equals(token) &&
                                                        (c.TokenValue.Contains(searchString) ||
                                                         c.LicenseTable.CustomerTable.SCRname.Contains(searchString) ||
                                                         c.LicenseTable.LicenseName.Contains(searchString))).ToList(); 
                    }
                    else
                        list = db.TokenTables.Where(c => c.TokenValue.Contains(searchString) ||
                                                         c.LicenseTable.SCRnumber.ToString().Contains(searchString)).ToList();
                }
                return list;
            }
        }

        //can duplicate token exist for a given customer?
        //each license file represents a unique system
        //a customer should have a unique mac, computer, bioserial, hardware key
        public static bool TokenExists(uint custID, byte tokenType, string tokenValue)
        {
            bool bExists = false;
            using(CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;

                IList<TokenTable> tokenList = db.TokenTables.Where(c => c.LicenseTable.SCRnumber.Equals(custID) &&
                                                              c.TokenType.Equals(tokenType) &&
                                                              c.TokenValue.Equals(tokenValue)).ToList();
                if (tokenList.Count > 0)
                    bExists = true;                
            }
            return bExists;
        }

        public static IList<TokenTable> GetTokensByLicenseName(string licenseName)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                return db.TokenTables.Where(c => c.LicenseTable.LicenseName.Equals(licenseName)).ToList();
            }
        }

        //each license can only have at most one of each token type
        public static TokenTable GetTokenByLicenseName(string licenseName, byte tokenType)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                return db.TokenTables.Where(c => c.LicenseTable.LicenseName.Equals(licenseName) &&
                                                       c.TokenType.Equals(tokenType)).SingleOrDefault();
            }
        }

        public static bool IsHardwareTokenActive(uint custID, string keyValue)
        {
            bool bActive = false;
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                IList<TokenTable> activeHardwareKey = db.TokenTables.Where(c => c.CustID.Equals(custID) &&
                                                                                c.TokenType.Equals(1) &&
                                                                                c.TokenStatus.Equals(1) &&
                                                                                c.TokenValue.Equals(keyValue)).ToList();
                if (activeHardwareKey.Count > 0)
                    bActive = true;
            }
            return bActive;
        }

        public static TokenTable GetHardwareTokenByKeyValue(uint custID, string keyValue)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                return db.TokenTables.Where(c => c.CustID.Equals(custID) &&
                                            c.TokenType.Equals(1) &&
                                            c.TokenValue.Equals(keyValue)).SingleOrDefault();
            }
        }

        public static IList<TokenTable> GetAvailableHardwareTokensByCustID(uint custID)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.ObjectTrackingEnabled = false;
                return db.TokenTables.Where(c => c.CustID.Equals(custID) &&
                                            c.LicenseID.Equals(-1)).ToList();   
            }
        }

        //license, 
        public static uint GetNextHardwareTokenValue(uint custID)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                uint value = 4096;
                uint currentValue = 0;
                db.ObjectTrackingEnabled = false;
                //hardware token has type value 1
                IList<TokenTable> tokenList = db.TokenTables.Where(c => c.CustID.Equals(custID) &&
                                                              c.TokenType.Equals(1)).OrderBy(p => p.TokenValue).ToList();
                string[] splitToken;               
                if (tokenList.Count() > 0)
                {
                    splitToken = tokenList.FirstOrDefault().TokenValue.Split("-".ToCharArray());
                    if (UInt32.Parse(splitToken[1], System.Globalization.NumberStyles.HexNumber) == 4096)    //initial value for hwkeys -> 1000
                    {
                        foreach (TokenTable token in tokenList)
                        {
                            splitToken = token.TokenValue.Split("-".ToCharArray());
                            currentValue = UInt32.Parse(splitToken[1], System.Globalization.NumberStyles.HexNumber);
                            if ((currentValue - value) > 1)
                            {   //increment to next available group ID
                                value = value + 1;
                                break;
                            }
                            value = currentValue;
                        }
                        splitToken = tokenList.Last().TokenValue.Split("-".ToCharArray());
                        if (value == UInt32.Parse(splitToken[1], System.Globalization.NumberStyles.HexNumber))
                            value = value + 1;
                    }
                }
                return value;
            }
        }

        public static void CreateToken(TokenTable validationToken)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.TokenTables.InsertOnSubmit(validationToken);
                db.SubmitChanges();
            }
        }

        public static void UpdateToken(TokenTable validationToken)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.TokenTables.Attach(validationToken, true);
                db.SubmitChanges();
            }
        }

        public static void DeleteToken(TokenTable validationToken)
        {
            using (CreatorDataContext db = new CreatorDataContext())
            {
                db.TokenTables.Attach(validationToken);
                db.TokenTables.DeleteOnSubmit(validationToken);
                db.SubmitChanges();
            }
        }
    }
}
