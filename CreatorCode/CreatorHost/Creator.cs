using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using System.ServiceModel;
using System.Reflection;
using Service.Creator;
using System.ServiceModel.Description;
using System.Configuration;
using System.Configuration.Install;

namespace CreatorHost
{
    [RunInstaller(true)]
    public class ProjectInstaller : Installer
    {
        private ServiceProcessInstaller process;
        private ServiceInstaller service;

        public ProjectInstaller()
        {
            process = new ServiceProcessInstaller();
            process.Account = ServiceAccount.LocalSystem;
            service = new ServiceInstaller();
            service.ServiceName = "Creator Service Host";
            service.StartType = ServiceStartMode.Automatic;
            service.ServiceName = "Creator";
            Installers.Add(process);
            Installers.Add(service);
        }
    }

    public partial class CreatorService : ServiceBase
    {
        private static ServiceHost creatorHost = null;

        public CreatorService()
        {
        }
        
        protected override void OnStart(string[] args)
        {
            CreatorService.StartService();
        }

        public static void StartService()
        {
            try
            {
                //WCF service hosting
                creatorHost = new ServiceHost(typeof(Creator));
                StartServiceHost(creatorHost);
            }
            catch (TargetInvocationException tiEx)
            {
                Console.WriteLine("Error occurred " + tiEx.Message);
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error occurred " + ex.Message);
            }
        }

        protected override void OnStop()
        {
            try
            {
                CreatorService.StopServiceHost(creatorHost);
            }
            catch (Exception e)
            {

                Console.WriteLine("Error occurred " + e.Message);
            }
        }

        private static void StartServiceHost(ServiceHost serviceHost)
        {
            try
            {
                // We will recreate the service host here to be sure we don't have a 
                //service in a faulted state
                serviceHost = new ServiceHost(serviceHost.Description.ServiceType);

                Console.WriteLine("Attempting to open Service.Creator service.");

                serviceHost.Open();
                serviceHost.Faulted += new EventHandler(ServiceHost_Faulted);
            }
            catch (ObjectDisposedException objDisposedEx)
            {
                Console.WriteLine(objDisposedEx.ToString());

            }
            catch (InvalidOperationException invalidOpEx)
            {
                Console.WriteLine(invalidOpEx.ToString());

            }
            catch (CommunicationObjectFaultedException commObjFaultedEx)
            {
                Console.WriteLine(commObjFaultedEx.ToString());

            }
            catch (System.ServiceProcess.TimeoutException tmOutEx)
            {
                Console.WriteLine(tmOutEx.ToString());

            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.ToString());
            }
        }

        private static void StopServiceHost(ServiceHost serviceHost)
        {
            if (serviceHost.State != CommunicationState.Closed)
            {
                if (serviceHost.State != CommunicationState.Faulted)
                {
                    serviceHost.Close();
                    Console.WriteLine(String.Format("{0} Closed.",
                        serviceHost.Description.Name));
                }
                else
                {
                    serviceHost.Abort();
                    Console.WriteLine(String.Format("{0} Aborted.",
                        serviceHost.Description.Name));
                }
            }
        }

        private static void RestartServiceHost(ServiceHost serviceHost)
        {
            StopServiceHost(serviceHost);
            StartServiceHost(serviceHost);
        }


        private static void ServiceHost_Faulted(Object sender, EventArgs e)
        {
            ServiceHost serviceHost = sender as ServiceHost;
            Console.Write(String.Format("{0} Faulted.  Attempting Restart.",
                serviceHost.Description.Name));
            RestartServiceHost(serviceHost);
        }
    }
}
