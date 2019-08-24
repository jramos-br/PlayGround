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
        private static readonly bool RunWarmupWriteTest = true;
        private static readonly bool RunWarmupReadTest = true;
        private static readonly bool RunWriteTest = false;
        private static readonly bool RunReadTest = false;
        private static readonly bool DisplayWarmupWriteTest = true;
        private static readonly bool DisplayWarmupReadTest = true;
        private static readonly bool DisplayWriteTest = DisplayWriteTestStep | DisplayWriteTestTime;
        private static readonly bool DisplayWriteTestStep = false;
        private static readonly bool DisplayWriteTestTime = true;
        private static readonly bool DisplayReadTest = DisplayReadTestStep | DisplayReadTestTime;
        private static readonly bool DisplayReadTestStep = false;
        private static readonly bool DisplayReadTestTime = true;
        private static readonly bool DisplaySortedResults = true;
        private static readonly bool ReadTestClearFileCache = false;
        private static readonly bool ReadTestRunGarbageCollector = true;

        private static readonly string Separator = "------------------------------------------------------------------------------";

        private static readonly List<(string header, TimeSpan elapsed, TimeSpan average)> Results = new List<(string header, TimeSpan elapsed, TimeSpan average)>();

        private static string FirstHashResult = null;

        private static MyHashAlgorithm GetMyHashAlgorithm()
        {
            return new MySimpleHash();
        }

        public static void CalcStats(List<long> values, out long lsum, out long lavg, out long lstd)
        {
            int count = values.Count;

            if (count == 0)
                throw new ArgumentOutOfRangeException();

            double avg = values.Average();
            double sum = values.Sum(v => (v - avg) * (v - avg));
            double std = Math.Sqrt(sum / count);

            lavg = Convert.ToInt64(avg);
            lsum = Convert.ToInt64(sum);
            lstd = Convert.ToInt64(std);
        }

        private delegate void TestFunc(string name, string[] args, MyHashAlgorithm hash, Action<string, MyHashAlgorithm> test);

        private static void WarmupWriteTest(string name, string[] args, MyHashAlgorithm hash, Action<string, MyHashAlgorithm> test)
        {
            for (var i = 0; i < args.Length; ++i)
            {
                if (DisplayWarmupReadTest)
                {
                    var header = args.Length > 1 ? $"{name}: {args[i]}" : name;

                    Console.Out.WriteLine("{0}: WRITE TEST WARM UP", header);
                }

                test(args[i], hash);
            }
        }

        private static void WriteTest(string name, string[] args, MyHashAlgorithm hash, Action<string, MyHashAlgorithm> test)
        {
            for (var i = 0; i < args.Length; ++i)
            {
                var header = args.Length > 1 ? $"{name}: {args[i]}" : name;
                var ticks = new List<long>();

                for (int n = 0; n < WRITETESTS; ++n)
                {
                    var elapsed = MyCrono.Elapsed(() => test(args[i], hash));
                    var hashResult = hash.FormatHash();

                    if (DisplayWriteTestStep)
                    {
                        Console.Out.WriteLine("{0}: ELAPSED {1} HASH {2}", header, elapsed, hashResult);
                    }

                    ticks.Add(elapsed.Ticks);
                }

                ticks.Sort();

                CalcStats(ticks, out long tsum, out long tavg, out long tstd);

                var sel = ticks.Where(v => v > tavg - tstd && v < tavg + tstd);
                var sum = new TimeSpan(sel.Sum());
                var avg = new TimeSpan(Convert.ToInt64(sel.Average()));

                Results.Add((header, sum, avg));

                if (DisplayWriteTestTime)
                {
                    Console.Out.WriteLine("{0}: ELAPSED {1} AVERAGE {2}", header, sum, avg);
                }
            }
        }

        private static void WarmupReadTest(string name, string[] args, MyHashAlgorithm hash, Action<string, MyHashAlgorithm> test)
        {
            for (var i = 0; i < args.Length; ++i)
            {
                if (DisplayWarmupReadTest)
                {
                    var header = args.Length > 1 ? $"{name}: {args[i]}" : name;

                    Console.Out.WriteLine("{0}: READ TEST WARM UP", header);
                }

                test(args[i], hash);
            }
        }

        private static void ReadTest(string name, string[] args, MyHashAlgorithm hash, Action<string, MyHashAlgorithm> test)
        {
            for (var i = 0; i < args.Length; ++i)
            {
                var header = args.Length > 1 ? $"{name}: {args[i]}" : name;
                var ticks = new List<long>();

                for (int n = 0; n < READTESTS; ++n)
                {
                    if (ReadTestClearFileCache)
                    {
                        Win32.ClearFileCache(args[i]);
                    }

                    if (ReadTestRunGarbageCollector)
                    {
                        GC.Collect();
                        GC.WaitForPendingFinalizers();
                        GC.Collect();
                        GC.WaitForPendingFinalizers();
                    }

                    var elapsed = MyCrono.Elapsed(() => test(args[i], hash));
                    var hashResult = hash.FormatHash();
                    var ok = true;

                    if (FirstHashResult == null)
                    {
                        FirstHashResult = hashResult;
                    }
                    else if (FirstHashResult != hashResult)
                    {
                        ok = false;
                    }

                    if (ok)
                    {
                        if (DisplayReadTestStep)
                        {
                            Console.Out.WriteLine("{0}: ELAPSED {1} HASH {2}", header, elapsed, hashResult);
                        }
                    }
                    else
                    {
                        Console.Out.WriteLine("{0}: ELAPSED {1} HASH {2} FIRST {3}", header, elapsed, hashResult, FirstHashResult);
                    }

                    ticks.Add(elapsed.Ticks);
                }

                ticks.Sort();

                CalcStats(ticks, out long tsum, out long tavg, out long tstd);

                var sel = ticks.Where(v => v > tavg - tstd && v < tavg + tstd);
                var sum = new TimeSpan(sel.Sum());
                var avg = new TimeSpan(Convert.ToInt64(sel.Average()));

                Results.Add((header, sum, avg));

                if (DisplayReadTestTime)
                {
                    Console.Out.WriteLine("{0}: ELAPSED {1} AVERAGE {2}", header, sum, avg);
                }
            }
        }

        private static void ReportResults()
        {
            if (Results.Count > 0 && DisplaySortedResults)
            {
                Results.Sort((x, y) => -x.average.CompareTo(y.average));

                Console.Out.WriteLine(Separator);

                foreach (var (header, elapsed, average) in Results)
                {
                    Console.Out.WriteLine("{0}: ELAPSED {1} AVERAGE {2}", header, elapsed, average);
                }
            }
        }

        private static void Run<T>(string[] args, MyHashAlgorithm hash, TestFunc action) where T : TestBase, new()
        {
            new T().Run((name, test) => action(name, args, hash, test));
        }

        private static void RunTests(string[] args)
        {
            using (MyHashAlgorithm hash = GetMyHashAlgorithm())
            {
                var writeTests = new (bool, bool, TestFunc)[]
                {
                    (RunWarmupWriteTest, DisplayWarmupWriteTest, WarmupWriteTest),
                    (RunWriteTest, DisplayWriteTest, WriteTest)
                };

                foreach (var (run, display, test) in writeTests)
                {
                    if (run)
                    {
                        if (display)
                        {
                            Console.Out.WriteLine(Separator);
                        }

                        Run<WriteTest1>(args, hash, test);
                    }
                }

                var readTests = new (bool, bool, TestFunc)[]
                {
                    (RunWarmupReadTest, DisplayWarmupReadTest, WarmupReadTest),
                    (RunReadTest, DisplayReadTest, ReadTest)
                };

                foreach (var (run, display, test) in readTests)
                {
                    if (run)
                    {
                        if (display)
                        {
                            Console.Out.WriteLine(Separator);
                        }

                        Run<ReadTest1>(args, hash, test);
                        Run<ReadTest2>(args, hash, test);
                        Run<ReadTest3>(args, hash, test);
                        Run<ReadTest4>(args, hash, test);
                        Run<ReadTest5>(args, hash, test);
                        Run<ReadTest6>(args, hash, test);
                        //Run<ReadTest7>(args, hash, test);
                    }
                }
            }
        }

        static void Main(string[] args)
        {
            try
            {
                RunTests(args);
                ReportResults();
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine(ex.ToString());
                Environment.ExitCode = 3;
            }
        }
    }
}
