using System.ServiceModel;
using System;

namespace Client.Creator.ServiceProxy
{
    /// <summary>
    /// Service client delegate
    /// </summary>
    public delegate void UseServiceDelegate<T>(T proxy); 

    public static class Service<T>
    {
        #region Static Fields             
        public static ChannelFactory<T> _channelFactory = new ChannelFactory<T>("CreatorServiceTcp");        
        //public static 
        #endregion

        #region Methods
        /// <summary>
        /// Creates the WCF service proxy and runs the codeBlock delegate
        /// using the WCF service proxy that is created
        /// </summary>
        /// <param name="codeBlock">The code to run using the WCF Proxy</param>
        public static void Use(UseServiceDelegate<T> codeBlock)
        {
            IClientChannel proxy = (IClientChannel)_channelFactory.CreateChannel();            
            bool success = false;
            try
            {
                codeBlock((T)proxy);
                proxy.Close();
                success = true;
            }
            catch (FaultException<EndpointNotFoundException> eex)
            {
                Console.WriteLine("Client FaulException<EndpointNotFoundException>");
            }
            catch (FaultException<ApplicationException> dex)
            {
                Console.WriteLine("Client FaultException<ApplicationException>");
            }
            catch (FaultException<ArgumentException> aex)
            {
                Console.WriteLine("Client FaultException<ArgumentException>");
            }
            catch (FaultException<Exception> ex)
            {
                Console.WriteLine("Client FaultException<Exception>");
            }
            catch (FaultException fEx)
            {
                Console.WriteLine("Client FaultException");
            }
            finally
            {
                if (!success)
                {
                    proxy.Abort();
                }
            }
        }

        public static bool IsValidHost(string hostName)
        {
            bool bValidHost = false;
            ChannelFactory<T> _channelFactory = new ChannelFactory<T>("CreatorServiceTcp",
                                                                       new EndpointAddress(new Uri("net.tcp://" + hostName + ":9091/Creator/tcp")));
            IClientChannel proxy = (IClientChannel)_channelFactory.CreateChannel();
            try
            {
                proxy.Open();
                proxy.Close();
                bValidHost = true;
            }
            catch (Exception ex)
            {
                throw ex;
            }
            finally
            {
                if (!bValidHost)
                {
                    proxy.Abort();
                }
            }
            return bValidHost;
        }
        #endregion
    }
}
