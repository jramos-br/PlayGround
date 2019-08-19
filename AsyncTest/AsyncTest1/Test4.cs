using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace AsyncTest1
{
    // https://devblogs.microsoft.com/pfxteam/await-synchronizationcontext-and-console-apps/
    class Test4
    {
        static async Task DemoAsync()
        {
            var d = new Dictionary<int, int>();

            for (int i = 0; i < 1000; i++)
            {
                int id = Thread.CurrentThread.ManagedThreadId;
                d[id] = d.TryGetValue(id, out int count) ? count + 1 : 1;
                await Task.Yield();
            }

            foreach (var pair in d)
            {
                Console.Out.WriteLine(pair);
            }
        }

        public static void Run(string[] args)
        {
            DemoAsync().GetAwaiter().GetResult();
        }
    }
}
