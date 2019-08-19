using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FileReadWriteTest
{
    using static FileReadWriteTest.Global;

    class ReadTest3
    {
        private ReadTest3() { }

        private static async Task RunAsync(string path, MyHashAlgorithm hash, int bufferSize, FileOptions options)
        {
            byte[] buffer = new byte[bufferSize * 2];
            int bytesRead;

            using (var fs = new FileStream(path, FileMode.Open, FileAccess.Read, FileShare.Read, DefaultFileStreamBufferSize, options))
            {
                var buflen = new int[2] { bufferSize, bufferSize };
                var bufpos = new int[2] { 0, bufferSize };
                var tasks = new Task<int>[2];
                int curr = 0;
                int next;

                tasks[curr] = fs.ReadAsync(buffer, bufpos[curr], buflen[curr]);

                hash.Initialize();

                while ((bytesRead = await tasks[curr]) > 0)
                {
                    next = curr ^ 1;
                    tasks[next] = fs.ReadAsync(buffer, bufpos[next], buflen[next]);
                    hash.TransformBlock(buffer, bufpos[curr], bytesRead);
                    curr = next;
                }

                hash.TransformFinalBlock(buffer, 0, 0);
            }
        }

        private static void Run(string path, MyHashAlgorithm hash, int bufferSize, FileOptions options)
        {
            RunAsync(path, hash, bufferSize, options).GetAwaiter().GetResult();
        }

        public static void Run(Action<string, Action<string, MyHashAlgorithm>> action)
        {
            action(nameof(ReadTest3) + "A1S", (path, hash) => Run(path, hash, ReadBufferSize, FileOptions.None));
            action(nameof(ReadTest3) + "A4S", (path, hash) => Run(path, hash, ReadBufferSize * 4, FileOptions.None));
            action(nameof(ReadTest3) + "A8S", (path, hash) => Run(path, hash, ReadBufferSize * 8, FileOptions.None));

            action(nameof(ReadTest3) + "B1S", (path, hash) => Run(path, hash, ReadBufferSize, FileOptions.SequentialScan));
            action(nameof(ReadTest3) + "B4S", (path, hash) => Run(path, hash, ReadBufferSize * 4, FileOptions.SequentialScan));
            action(nameof(ReadTest3) + "B8S", (path, hash) => Run(path, hash, ReadBufferSize * 8, FileOptions.SequentialScan));

            action(nameof(ReadTest3) + "A1A", (path, hash) => Run(path, hash, ReadBufferSize, FileOptions.Asynchronous));
            action(nameof(ReadTest3) + "A4A", (path, hash) => Run(path, hash, ReadBufferSize * 4, FileOptions.Asynchronous));
            action(nameof(ReadTest3) + "A8A", (path, hash) => Run(path, hash, ReadBufferSize * 8, FileOptions.Asynchronous));

            action(nameof(ReadTest3) + "B1A", (path, hash) => Run(path, hash, ReadBufferSize, FileOptions.Asynchronous | FileOptions.SequentialScan));
            action(nameof(ReadTest3) + "B4A", (path, hash) => Run(path, hash, ReadBufferSize * 4, FileOptions.Asynchronous | FileOptions.SequentialScan));
            action(nameof(ReadTest3) + "B8A", (path, hash) => Run(path, hash, ReadBufferSize * 8, FileOptions.Asynchronous | FileOptions.SequentialScan));
        }
    }
}
