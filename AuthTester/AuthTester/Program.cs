using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.InteropServices;
using System.Reflection;


namespace AuthTester
{  
    class Program
    {
        [DllImport("authdll.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern int auth_user(string u, string p, string d, string t);

        
        static void Main(string[] args)
        {
            System.Console.WriteLine("Testing KRB5 Authentication...");
            System.Console.Write("Enter a User Name: ");
            string user = System.Console.ReadLine();
            System.Console.Write("Enter Password: ");
            string pass = System.Console.ReadLine();
            System.Console.Write("Enter Domain: ");
            string domain = System.Console.ReadLine();
            System.Console.Write("Enter SPN Target (service/domain): ");
            string spntgt = System.Console.ReadLine();
            int r = auth_user(user, pass, domain, spntgt);
            //int r = auth_user("u0270473", "magic@69", "AD.UTAH.EDU", "krbtgt/AD.UTAH.EDU");
            System.Console.WriteLine("result: " + r);
            System.Console.ReadLine();
        }
    }
}
