using System.ServiceModel;
using System;

namespace Client.Creator.ServiceProxy
{
    /// <summary>
    /// Service client delegate
    /// </summary>
    public delegate void UseServiceDelegate<T>(T proxy); 

    /// <summary>
    /// A helper class to run code using a WCF client proxy
    /// 
    /// This weird class came from 
    /// http://www.iserviceoriented.com/blog/post/Indisposable+-+WCF+Gotcha+1.aspx
    /// </summary>
    public static class Service<T>
    {
        #region Static Fields
        public static EndpointAddress address; // = new EndpointAddress(new Uri(string.Format("net.tcp://{0}:{1}/Service1", "achou2", "9090")), EndpointIdentity.CreateSpnIdentity("Service1"));               
        public static ChannelFactory<T> _channelFactory = new ChannelFactory<T>("CreatorServiceTcp");        
        #endregion

        #region Methods
        /// <summary>
        /// Creates the WCF service proxy and runs the codeBlock delegate
        /// using the WCF service proxy that is created
        /// </summary>
        /// <param name="codeBlock">The code to run using the WCF Proxy</param>
        public static void Use(UseServiceDelegate<T> codeBlock)
        {
            //ChannelFactory<T> _channelFactory = new ChannelFactory<T>("NetTcpBinding_IService1", address);              
            //_channelFactory.Endpoint.Address = new EndpointAddress(new Uri(string.Format("net.tcp://{0}:{1}/Creator", "achou2", "9091")), EndpointIdentity.CreateSpnIdentity("Creator")); ;
            IClientChannel proxy = (IClientChannel)_channelFactory.CreateChannel();
            proxy.Open();
            bool success = false; 
            try 
            { 
                codeBlock((T)proxy); 
                proxy.Close(); 
                success = true; 
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
        #endregion
    }
}
