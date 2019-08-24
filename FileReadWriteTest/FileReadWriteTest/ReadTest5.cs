using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FileReadWriteTest
{
    using static FileReadWriteTest.Global;

    class ReadTest5 : TestBase
    {
        private void Run(string path, MyHashAlgorithm hash, int bufferSize, Win32.FileCreationFlags dwFlags)
        {
            using (var buffer = new Win32.PinnableArray<byte>(bufferSize))
            {
                int bytesRead;

                var dwDesiredAccess = Win32.DesiredAccess.GenericRead;
                var dwShareMode = Win32.FileShareMode.Read;
                var lpSecurityAttributes = new Win32.SecurityAttributes();
                var dwDisposition = Win32.FileCreationDisposition.OpenExisting;
                var dwAttributes = Win32.FileCreationAttributes.None;

                using (var hFile = Win32.CreateFile(path, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwDisposition, dwFlags, dwAttributes))
                {
                    hash.Initialize();

                    buffer.Pin();

                    while ((bytesRead = Win32.ReadFile(hFile, buffer, bufferSize)) > 0)
                    {
                        hash.TransformBlock(buffer, 0, bytesRead);
                    }

                    buffer.Unpin();

                    hash.TransformFinalBlock(buffer, 0, 0);
                }
            }
        }

        public override void Run(Action<string, Action<string, MyHashAlgorithm>> action)
        {
            action(nameof(ReadTest5) + "A1S", (path, hash) => Run(path, hash, ReadBufferSize, Win32.FileCreationFlags.None));
            action(nameof(ReadTest5) + "A4S", (path, hash) => Run(path, hash, ReadBufferSize * 4, Win32.FileCreationFlags.None));
            action(nameof(ReadTest5) + "A8S", (path, hash) => Run(path, hash, ReadBufferSize * 8, Win32.FileCreationFlags.None));

            action(nameof(ReadTest5) + "B1S", (path, hash) => Run(path, hash, ReadBufferSize, Win32.FileCreationFlags.SequentialScan));
            action(nameof(ReadTest5) + "B4S", (path, hash) => Run(path, hash, ReadBufferSize * 4, Win32.FileCreationFlags.SequentialScan));
            action(nameof(ReadTest5) + "B8S", (path, hash) => Run(path, hash, ReadBufferSize * 8, Win32.FileCreationFlags.SequentialScan));
        }
    }
}
