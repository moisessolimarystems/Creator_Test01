using System;
using System.Collections.Generic;
using System.Linq;
using System.ServiceProcess;
using System.Text;

namespace CreatorHost
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        static void Main()
        {
#if (!DEBUG)
            try
            {


                ServiceBase[] ServicesToRun;
                ServicesToRun = new ServiceBase[] { new CreatorService() };
                ServiceBase.Run(ServicesToRun);
            }
            catch (Exception e)
            {
                Console.WriteLine("Error occurred " + e.Message);
            }
#else
            try
            {
                //Console.WriteLine("Starting Service");
                //CreatorService.StartService();
                //Console.WriteLine("Service Started");
                //System.Threading.Thread.Sleep(System.Threading.Timeout.Infinite);
                ServiceBase[] ServicesToRun;
                ServicesToRun = new ServiceBase[] { new CreatorService() };
                ServiceBase.Run(ServicesToRun);

            }
            catch (Exception e)
            {
                Console.WriteLine("Error occurred " + e.Message);
            }
#endif
        }
    }
}
