using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System;
using System.Collections.Specialized;
using System.Net;
using System.Security;
using Newtonsoft.Json;

namespace Client.Creator
{
    public static class Sugar
    {
        public enum Action
        {
            NEW,
            RETURN,
            ACTIVE
        };

        public enum EndType
        {
            SERVER,
            PRODUCT
        }

        public static string Post(Action action, EndType endType, string license)
        {
            string response;
            var postData = new Dictionary<string, string>()
            {
                {"keyID", "1f6ed653-8c6e-29f7-82fa-501ae068fd92"},
                {"action", GetActionString(action)},
                {"endType", GetEndTypeString(endType)},
                {"license", license},
                {"userID", System.Security.Principal.WindowsIdentity.GetCurrent().Name}
            }; 
            using (WebClient client = new WebClient())
            {
                var dataString = JsonConvert.SerializeObject(postData);
                client.Headers.Add(HttpRequestHeader.ContentType, "application/json");
                response = client.UploadString(new Uri("https://sugar.solimarsystems.com/index.php?entryPoint=creator_push"), "POST", dataString);                
            }
            return response;
        }

        private static string GetActionString(Action action)
        {
            string value = "";
            switch (action)
            {
                case Action.NEW :
                    value = "New";
                    break;
                case Action.RETURN:
                    value = "Return";
                    break;
                case Action.ACTIVE:
                    value = "Active";
                    break;
                default:break;
            }
            return value;
        }

        private static string GetEndTypeString(EndType endType)
        {
            string value = "";
            switch (endType)
            {
                case EndType.PRODUCT:
                    value = "Product";
                    break;
                case EndType.SERVER:
                    value = "Server";
                    break;
                default: break;
            }
            return value;
        }
    }
}
