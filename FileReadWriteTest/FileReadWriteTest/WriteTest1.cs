using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FileReadWriteTest
{
    using static FileReadWriteTest.Global;

    class WriteTest1 : TestBase
    {
        private const int RandomSeed = 42;

        private static void Run(string path, MyHashAlgorithm hash, int bufferSize, FileOptions options)
        {
            using (var fs = new FileStream(path, FileMode.Create, FileAccess.Write, FileShare.None, DefaultFileStreamBufferSize, options))
            {
                var random = new Random(RandomSeed);
                var buffer = new byte[bufferSize];
                int bytesWritten = 0;

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

        public override void Run(Action<string, Action<string, MyHashAlgorithm>> action)
        {
            action(nameof(WriteTest1), (path, hash) => Run(path, hash, WriteBufferSize, FileOptions.None));
        }
    }
}
