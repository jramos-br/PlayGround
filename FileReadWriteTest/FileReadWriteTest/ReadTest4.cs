using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FileReadWriteTest
{
    using static FileReadWriteTest.Global;

    class ReadTest4
    {
        private ReadTest4() { }

        private const int DefaultFileStreamBufferSize = 4096;
        private const int BufferSize = 256 * 1024;

        private async Task<int> RunAsync(string path, FileOptions options)
        {
            byte[] buffer = new byte[BufferSize * 2];
            int sum = 0;

            using (var fs = new FileStream(path, FileMode.Open, FileAccess.Read, FileShare.Read, DefaultFileStreamBufferSize, options))
            {
                var buflen = new int[2] { BufferSize, BufferSize };
                var bufpos = new int[2] { 0, BufferSize };
                var tasks = new Task<int>[2];
                int curr = 0;
                int next;
                int bytesRead;

                tasks[curr] = fs.ReadAsync(buffer, bufpos[curr], buflen[curr]);

                while ((bytesRead = await tasks[curr]) > 0)
                {
                    next = curr ^ 1;
                    tasks[next] = fs.ReadAsync(buffer, bufpos[next], buflen[next]);

                    for (int i = bufpos[curr], n = i + bytesRead; i < n; ++i)
                    {
                        sum = (sum << 1) ^ buffer[i];
                    }

                    curr = next;
                }
            }

            return sum;
        }

        private int Run(string path, FileOptions options)
        {
            return RunAsync(path, options).GetAwaiter().GetResult();
        }

        public static void Run(string func, string path, int repeat, FileOptions options)
        {
            var test = new ReadTest4();
            var list = new List<long>();

            for (var i = 0; i < repeat; ++i)
            {
                //int sum = 0;
                //var elapsed = MyCrono.Elapsed(() => sum = test.Run(args[i], options));
                //Console.Out.WriteLine("{0}: {1}: {2} {3:X8}", func, args[i], elapsed, sum);
                //list.Add(elapsed.Ticks);
            }

            var average = new TimeSpan(Convert.ToInt64(list.Average()));
            Console.Out.WriteLine("{0}: AVERAGE {1}", func, average);
        }

        public static void Run(string path, int repeat)
        {
            //Run(nameof(ReadTest4) + "A", args, FileOptions.SequentialScan);
            //Run(nameof(ReadTest4) + "B", args, FileOptions.SequentialScan | FileOptions.Asynchronous);
        }
    }
}
