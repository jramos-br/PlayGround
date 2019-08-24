using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FileReadWriteTest
{
    using static FileReadWriteTest.Global;

    class ReadTest1 : TestBase
    {
        private void Run(string path, MyHashAlgorithm hash, int bufferSize, FileOptions options)
        {
            using (var fs = new FileStream(path, FileMode.Open, FileAccess.Read, FileShare.Read, DefaultFileStreamBufferSize, options))
            {
                var buffer = new byte[bufferSize];
                int bytesRead;

                hash.Initialize();

                while ((bytesRead = fs.Read(buffer, 0, bufferSize)) > 0)
                {
                    hash.TransformBlock(buffer, 0, bytesRead);
                }

                hash.TransformFinalBlock(buffer, 0, 0);
            }
        }

        public override void Run(Action<string, Action<string, MyHashAlgorithm>> action)
        {
            action(nameof(ReadTest1) + "A1S", (path, hash) => Run(path, hash, ReadBufferSize, FileOptions.None));
            action(nameof(ReadTest1) + "A4S", (path, hash) => Run(path, hash, ReadBufferSize * 4, FileOptions.None));
            action(nameof(ReadTest1) + "A8S", (path, hash) => Run(path, hash, ReadBufferSize * 8, FileOptions.None));

            action(nameof(ReadTest1) + "B1S", (path, hash) => Run(path, hash, ReadBufferSize, FileOptions.SequentialScan));
            action(nameof(ReadTest1) + "B4S", (path, hash) => Run(path, hash, ReadBufferSize * 4, FileOptions.SequentialScan));
            action(nameof(ReadTest1) + "B8S", (path, hash) => Run(path, hash, ReadBufferSize * 8, FileOptions.SequentialScan));
        }
    }
}
