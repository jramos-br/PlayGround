using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace FileReadWriteTest
{
    using static FileReadWriteTest.Global;

    class ReadTest4 : TestBase
    {
        private static readonly bool _flag = false;

        static ReadTest4()
        {
            if (_flag)
            {
                ThreadPool.GetMinThreads(out int workerThreads, out int completionPortThreads);
                Console.Out.WriteLine("GetMinThreads: workerThreads={0} completionPortThreads={1}", workerThreads, completionPortThreads);
                ThreadPool.GetMaxThreads(out workerThreads, out completionPortThreads);
                Console.Out.WriteLine("GetMaxThreads: workerThreads={0} completionPortThreads={1}", workerThreads, completionPortThreads);
                ThreadPool.GetAvailableThreads(out workerThreads, out completionPortThreads);
                Console.Out.WriteLine("GetAvailableThreads: workerThreads={0} completionPortThreads={1}", workerThreads, completionPortThreads);
            }
        }

        private class Buffer
        {
            public readonly int Offset;
            public int Length;

            public Buffer(int offset)
            {
                Offset = offset;
            }
        }

        private void Run(string path, MyHashAlgorithm hash, int bufferSize, FileOptions options)
        {
            using (var buffers = new BlockingCollection<Buffer>())
            using (var queue = new BlockingCollection<Buffer>())
            {
                var buffer = new byte[bufferSize * 2];

                for (int offset = 0; offset < buffer.Length; offset += bufferSize)
                {
                    buffers.Add(new Buffer(offset));
                }

                ThreadPool.QueueUserWorkItem((state) =>
                {
                    using (var fs = new FileStream(path, FileMode.Open, FileAccess.Read, FileShare.Read, DefaultFileStreamBufferSize, options))
                    {
                        Buffer next;

                        do
                        {
                            next = buffers.Take();
                            next.Length = fs.Read(buffer, next.Offset, bufferSize);
                            queue.Add(next);
                        }
                        while (next.Length > 0);
                    }
                });

                hash.Initialize();

                for (var next = queue.Take(); next.Length > 0; next = queue.Take())
                {
                    hash.TransformBlock(buffer, next.Offset, next.Length);
                    buffers.Add(next);
                }

                hash.TransformFinalBlock(buffer, 0, 0);
            }
        }

        public override void Run(Action<string, Action<string, MyHashAlgorithm>> action)
        {
            action(nameof(ReadTest4) + "A1S", (path, hash) => Run(path, hash, ReadBufferSize, FileOptions.None));
            action(nameof(ReadTest4) + "A4S", (path, hash) => Run(path, hash, ReadBufferSize * 4, FileOptions.None));
            action(nameof(ReadTest4) + "A8S", (path, hash) => Run(path, hash, ReadBufferSize * 8, FileOptions.None));

            action(nameof(ReadTest4) + "B1S", (path, hash) => Run(path, hash, ReadBufferSize, FileOptions.SequentialScan));
            action(nameof(ReadTest4) + "B4S", (path, hash) => Run(path, hash, ReadBufferSize * 4, FileOptions.SequentialScan));
            action(nameof(ReadTest4) + "B8S", (path, hash) => Run(path, hash, ReadBufferSize * 8, FileOptions.SequentialScan));
        }
    }
}
