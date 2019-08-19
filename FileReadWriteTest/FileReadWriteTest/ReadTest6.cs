using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FileReadWriteTest
{
    using System.Runtime.InteropServices;
    using static FileReadWriteTest.Global;

    class ReadTest6
    {
        private ReadTest6() { }

        private static void Run(string path, MyHashAlgorithm hash, int bufferSize, Win32.FileCreationFlags dwFlags)
        {
            using (var buffer = new Win32.UnmanagedBuffer(bufferSize))
            {
                byte[] bytes = new byte[4096];
                int bytesRead;

                var dwDesiredAccess = Win32.DesiredAccess.GenericRead;
                var dwShareMode = Win32.FileShareMode.Read;
                var lpSecurityAttributes = new Win32.SecurityAttributes();
                var dwDisposition = Win32.FileCreationDisposition.OpenExisting;
                var dwAttributes = Win32.FileCreationAttributes.None;

                using (var hFile = Win32.CreateFile(path, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwDisposition, dwFlags, dwAttributes))
                {
                    hash.Initialize();

                    while ((bytesRead = Win32.ReadFile(hFile, buffer, bufferSize)) > 0)
                    {
                        int count = 0;

                        do
                        {
                            int length = Math.Min(bytes.Length, bytesRead - count);
                            Marshal.Copy((IntPtr)buffer + count, bytes, 0, length);
                            hash.TransformBlock(bytes, 0, length);
                            count += length;

                        } while (count < bytesRead);
                    }

                    hash.TransformFinalBlock(bytes, 0, 0);
                }
            }
        }

        public static void Run(Action<string, Action<string, MyHashAlgorithm>> action)
        {
            action(nameof(ReadTest6) + "A1S", (path, hash) => Run(path, hash, ReadBufferSize, Win32.FileCreationFlags.None));
            action(nameof(ReadTest6) + "A4S", (path, hash) => Run(path, hash, ReadBufferSize * 4, Win32.FileCreationFlags.None));
            action(nameof(ReadTest6) + "A8S", (path, hash) => Run(path, hash, ReadBufferSize * 8, Win32.FileCreationFlags.None));

            action(nameof(ReadTest6) + "B1S", (path, hash) => Run(path, hash, ReadBufferSize, Win32.FileCreationFlags.SequentialScan));
            action(nameof(ReadTest6) + "B4S", (path, hash) => Run(path, hash, ReadBufferSize * 4, Win32.FileCreationFlags.SequentialScan));
            action(nameof(ReadTest6) + "B8S", (path, hash) => Run(path, hash, ReadBufferSize * 8, Win32.FileCreationFlags.SequentialScan));

            action(nameof(ReadTest6) + "C1S", (path, hash) => Run(path, hash, ReadBufferSize, Win32.FileCreationFlags.NoBuffering));
            action(nameof(ReadTest6) + "C4S", (path, hash) => Run(path, hash, ReadBufferSize * 4, Win32.FileCreationFlags.NoBuffering));
            action(nameof(ReadTest6) + "C8S", (path, hash) => Run(path, hash, ReadBufferSize * 8, Win32.FileCreationFlags.NoBuffering));
        }
    }
}
