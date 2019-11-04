using System;
using System.IO;
using System.Text;

namespace MemoryStream1
{
    public class MemoryStream1
    {
        public static void Run(string[] args)
        {
            Run2(args);
        }

        private static string CharArrayToString(char[] chars)
        {
            var result = new StringBuilder();

            result.Append('[');

            for (int i = 0; i < chars.Length; ++i)
            {
                char ch = chars[i];

                if (Char.IsControl(ch))
                    result.AppendFormat("\\u{0:D4}", (int)ch);
                else
                    result.Append(ch);
            }

            result.Append(']');

            return result.ToString();
        }

        private static void Run1(string[] args)
        {
            using (var ms = new MemoryStream())
            {
                using (var sw = new StreamWriter(ms, Encoding.UTF8, 1024, true))
                {
                    sw.WriteLine("The quick brown fox jumps over the lazy dog.");
                    sw.Write("Invalid file path characters are: ");
                    sw.WriteLine(CharArrayToString(Path.GetInvalidPathChars()));
                }

                Console.WriteLine($"Capacity = {ms.Capacity} Length = {ms.Length} Position = {ms.Position}");

                var a = ms.ToArray();
                Console.WriteLine($"Length = {a.Length}");

                ms.TryGetBuffer(out ArraySegment<byte> buffer);
                Console.WriteLine($"Offset = {buffer.Offset} Count = {buffer.Count}");

                ms.Seek(0, SeekOrigin.Begin);

                using (var sr = new StreamReader(ms, Encoding.UTF8, true, 1024, true))
                {
                    var s = sr.ReadToEnd();
                    Console.WriteLine(s.TrimEnd());
                }
            }
        }

        private static void Run2(string[] args)
        {
            // https://docs.microsoft.com/en-us/dotnet/api/system.io.memorystream?view=netframework-4.7.2

            UnicodeEncoding encoding = new UnicodeEncoding();

            byte[] firstString = encoding.GetBytes("Invalid file path characters are: ");
            byte[] secondString = encoding.GetBytes(Path.GetInvalidPathChars());

            using (MemoryStream ms = new MemoryStream(100))
            {
                ms.Write(firstString, 0, firstString.Length);

                for (int i = 0; i < secondString.Length; ++i)
                    ms.WriteByte(secondString[i]);

                Console.WriteLine($"Capacity = {ms.Capacity} Length = {ms.Length} Position = {ms.Position}");

                ms.Seek(0, SeekOrigin.Begin);

                var byteArray = new byte[ms.Length];
                var byteCount = ms.Read(byteArray, 0, 20);

                while (byteCount < ms.Length)
                    byteArray[byteCount++] = Convert.ToByte(ms.ReadByte());

                var charCount = encoding.GetCharCount(byteArray, 0, byteCount);
                var charArray = new char[charCount];
                var charCount2 = encoding.GetChars(byteArray, 0, byteCount, charArray, 0);

                System.Diagnostics.Debug.Assert(charCount2 == charCount);

                Console.WriteLine(CharArrayToString(charArray));
            }
        }
    }
}
