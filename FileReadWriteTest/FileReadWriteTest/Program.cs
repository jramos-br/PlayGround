using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FileReadWriteTest
{
    using static FileReadWriteTest.Global;

    class Program
    {
        private static void WriteTest(string name, string[] args, Action<string, MyHashAlgorithm> test)
        {
            for (var i = 0; i < args.Length; ++i)
            {
                var header = $"{name}: {args[i]}";
                var ticks = new List<long>();

                for (int n = 0; n < 1; ++n)
                {
                    using (MyHashAlgorithm hash = new MySimpleHash())
                    {
                        var elapsed = MyCrono.Elapsed(() => test(args[i], hash));
                        Console.Out.WriteLine("{0}: ELAPSED {1} HASH {2}", header, elapsed, hash.FormatHash());
                        ticks.Add(elapsed.Ticks);
                    }
                }

                var sum = new TimeSpan(Convert.ToInt64(ticks.Sum()));
                var average = new TimeSpan(Convert.ToInt64(ticks.Average()));

                //Console.Out.WriteLine("{0}: ELAPSED {1} AVERAGE {2}", header, sum, average);
            }
        }

        private static void WriteTest(string[] args)
        {
            WriteTest1.Run((name, test) => WriteTest(name, args, test));
        }

        private static void ReadTest(string name, string[] args, Action<string, MyHashAlgorithm> test)
        {
            for (var i = 0; i < args.Length; ++i)
            {
                var header = $"{name}: {args[i]}";
                var ticks = new List<long>();

                for (int n = 0; n < NTESTS; ++n)
                {
                    using (MyHashAlgorithm hash = new MySimpleHash())
                    {
                        Win32.ClearFileCache(args[i]);
                        var elapsed = MyCrono.Elapsed(() => test(args[i], hash));
                        Console.Out.WriteLine("{0}: ELAPSED {1} HASH {2}", header, elapsed, hash.FormatHash());
                        ticks.Add(elapsed.Ticks);
                    }
                }

                var sum = new TimeSpan(Convert.ToInt64(ticks.Sum()));
                var average = new TimeSpan(Convert.ToInt64(ticks.Average()));

                Console.Out.WriteLine("{0}: ELAPSED {1} AVERAGE {2}", header, sum, average);
            }
        }

        private static void ReadTest(string[] args)
        {
            ReadTest1.Run((name, test) => ReadTest(name, args, test));
            //ReadTest2.Run((name, test) => ReadTest(name, args, test));
            //ReadTest3.Run((name, test) => ReadTest(name, args, test));
            //ReadTest5.Run((name, test) => ReadTest(name, args, test));
            //ReadTest6.Run((name, test) => ReadTest(name, args, test));
        }

        void UsingGCHandles()
        {
            var dataArray = new byte[10 * 1024 * 1024];
            var handle = System.Runtime.InteropServices.GCHandle.Alloc(dataArray, System.Runtime.InteropServices.GCHandleType.Pinned);

            // retrieve a raw pointer to pass to the native code:
            var ptr = handle.AddrOfPinnedObject();

            // later, possibly in some other method:
            handle.Free();
        }

        static void Main(string[] args)
        {
            try
            {
                ReadTest(args);
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine(ex.ToString());
                Environment.ExitCode = 3;
            }
        }
    }
}
