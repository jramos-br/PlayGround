using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace AsyncTest1
{
    class Test1
    {
        private long Count = 0;

        private ConsoleKeyInfo ReadKey()
        {
            return Console.KeyAvailable ? Console.ReadKey(true) : new ConsoleKeyInfo();
        }

        private void Run()
        {
            if (Console.IsOutputRedirected || Console.IsErrorRedirected)
                return;

            Console.CursorVisible = false;

            try
            {
                Console.Clear();
                Console.Write("Press any ky to stop");
                Console.SetCursorPosition(0, 5);

                var start = DateTime.MinValue;

                while (ReadKey().Key == 0)
                {
                    Console.SetCursorPosition(0, 3);
                    Console.Error.Write(string.Format("[{0}] {1}", Thread.CurrentThread.ManagedThreadId, ++Count));
                    Console.SetCursorPosition(0, 5);

                    var now = DateTime.Now;

                    if (start.Ticks == 0 || now.Second != start.Second)
                    {
                        start = now;
                        Console.SetCursorPosition(0, 2);
                        Console.Error.Write(string.Format("{0}", now));
                        Console.SetCursorPosition(0, 5);
                    }
                }

                Console.Error.WriteLine("OK");
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine(ex.Message);
            }
            finally
            {
                Console.CursorVisible = true;
            }
        }

        public static void Run(string[] args)
        {
            var t = new Test1();
            t.Run();
        }
    }
}
