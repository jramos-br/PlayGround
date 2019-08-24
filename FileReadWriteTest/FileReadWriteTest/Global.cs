using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FileReadWriteTest
{
    static class Global
    {
        public const int DefaultFileStreamBufferSize = 4096;
        public const int ReadBufferSize = 64 * 1024;
        public const int WriteBufferSize = 64 * 1024;
        public const int TestFileSize = 768 * 1024 * 1024 - 23;
        public const int WRITETESTS = 1;
        public const int READTESTS = 7;
    }
}
