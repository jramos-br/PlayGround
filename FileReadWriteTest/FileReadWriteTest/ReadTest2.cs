﻿using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FileReadWriteTest
{
    using static FileReadWriteTest.Global;

    class ReadTest2 : TestBase
    {
        private void Run(string path, MyHashAlgorithm hash, int bufferSize, FileOptions options)
        {
            using (var fs = new FileStream(path, FileMode.Open, FileAccess.Read, FileShare.Read, DefaultFileStreamBufferSize, options))
            {
                var buffer = new byte[bufferSize * 2];
                var bufpos = new int[2] { 0, bufferSize };
                var tasks = new IAsyncResult[2];
                int bytesRead;
                int curr = 0;

                tasks[curr] = fs.BeginRead(buffer, bufpos[curr], bufferSize, null, null);

                hash.Initialize();

                while ((bytesRead = fs.EndRead(tasks[curr])) > 0)
                {
                    var next = curr ^ 1;
                    tasks[next] = fs.BeginRead(buffer, bufpos[next], bufferSize, null, null);
                    hash.TransformBlock(buffer, bufpos[curr], bytesRead);
                    curr = next;
                }

                hash.TransformFinalBlock(buffer, 0, 0);
            }
        }

        public override void Run(Action<string, Action<string, MyHashAlgorithm>> action)
        {
            action(nameof(ReadTest2) + "A1S", (path, hash) => Run(path, hash, ReadBufferSize, FileOptions.None));
            action(nameof(ReadTest2) + "A4S", (path, hash) => Run(path, hash, ReadBufferSize * 4, FileOptions.None));
            action(nameof(ReadTest2) + "A8S", (path, hash) => Run(path, hash, ReadBufferSize * 8, FileOptions.None));

            action(nameof(ReadTest2) + "B1S", (path, hash) => Run(path, hash, ReadBufferSize, FileOptions.SequentialScan));
            action(nameof(ReadTest2) + "B4S", (path, hash) => Run(path, hash, ReadBufferSize * 4, FileOptions.SequentialScan));
            action(nameof(ReadTest2) + "B8S", (path, hash) => Run(path, hash, ReadBufferSize * 8, FileOptions.SequentialScan));

            action(nameof(ReadTest2) + "A1A", (path, hash) => Run(path, hash, ReadBufferSize, FileOptions.Asynchronous));
            action(nameof(ReadTest2) + "A4A", (path, hash) => Run(path, hash, ReadBufferSize * 4, FileOptions.Asynchronous));
            action(nameof(ReadTest2) + "A8A", (path, hash) => Run(path, hash, ReadBufferSize * 8, FileOptions.Asynchronous));

            action(nameof(ReadTest2) + "B1A", (path, hash) => Run(path, hash, ReadBufferSize, FileOptions.Asynchronous | FileOptions.SequentialScan));
            action(nameof(ReadTest2) + "B4A", (path, hash) => Run(path, hash, ReadBufferSize * 4, FileOptions.Asynchronous | FileOptions.SequentialScan));
            action(nameof(ReadTest2) + "B8A", (path, hash) => Run(path, hash, ReadBufferSize * 8, FileOptions.Asynchronous | FileOptions.SequentialScan));
        }
    }
}
