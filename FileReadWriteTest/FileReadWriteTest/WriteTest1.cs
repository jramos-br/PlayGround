using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FileReadWriteTest
{
    using static FileReadWriteTest.Global;

    class WriteTest1
    {
        private WriteTest1() { }

        private const int RandomSeed = 42;

        private static void Run(string path, MyHashAlgorithm hash, int bufferSize, FileOptions options)
        {
            var random = new Random(RandomSeed);
            byte[] buffer = new byte[WriteBufferSize];
            int bytesWritten = 0;

            using (var fs = new FileStream(path, FileMode.Create, FileAccess.Write, FileShare.None, DefaultFileStreamBufferSize, options))
            {
                hash.Initialize();

                while (bytesWritten < TestFileSize)
                {
                    random.NextBytes(buffer);
                    int count = Math.Min(WriteBufferSize, TestFileSize - bytesWritten);
                    hash.TransformBlock(buffer, 0, count);
                    fs.Write(buffer, 0, count);
                    bytesWritten += count;
                }

                hash.TransformFinalBlock(buffer, 0, 0);
            }
        }

        public static void Run(Action<string, Action<string, MyHashAlgorithm>> action)
        {
            action(nameof(WriteTest1), (path, hash) => Run(path, hash, ReadBufferSize, FileOptions.None));
        }
    }
}
