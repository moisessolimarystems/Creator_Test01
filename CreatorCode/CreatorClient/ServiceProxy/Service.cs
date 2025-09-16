// Service.cs
//
// SLB 15.sep.2025 CR.34456; Changes for new attribs code (Licensing 3.4+) to work.
// Removed some variable declarations which weren't being used (e.g catch() {} ex vars).
// Renamed member variables to have m_ prefix vs. _. 

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
        public static ChannelFactory<T> s_channelFactory = new ChannelFactory<T>("CreatorServiceTcp"); // CR.34456; modified.
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
            IClientChannel proxy = (IClientChannel)s_channelFactory.CreateChannel();            
            bool success = false;
            try
            {
                codeBlock((T)proxy);
                proxy.Close();
                success = true;
            }
            catch (FaultException<EndpointNotFoundException>)						// CR.34456; modified.
			{
                Console.WriteLine("Client FaulException<EndpointNotFoundException>");
            }
            catch (FaultException<ApplicationException>)							// CR.34456; modified.
			{
                Console.WriteLine("Client FaultException<ApplicationException>");
            }
            catch (FaultException<ArgumentException>)								// CR.34456; modified.
			{
                Console.WriteLine("Client FaultException<ArgumentException>");
            }
            catch (FaultException<Exception>)										 // CR.34456; modified.
			{
                Console.WriteLine("Client FaultException<Exception>");
            }
            catch (FaultException)													// CR.34456; modified.
			{
                Console.WriteLine("Client FaultException");
            }
            catch (Exception)														// CR.34456; modified.
			{
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
            s_channelFactory = new ChannelFactory<T>("CreatorServiceTcp",
                                                                       new EndpointAddress(new Uri("net.tcp://" + hostName + ":9091/Creator/tcp")));
            IClientChannel proxy = (IClientChannel)s_channelFactory.CreateChannel();
            try
            {
                proxy.Open();
                proxy.Close();
                bValidHost = true;
            }
            catch (CommunicationException)											// CR.34456; modified.
			{
                throw new Exception(string.Format("Failed to connect to {0}. Check if the creator service is started.", hostName));
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
