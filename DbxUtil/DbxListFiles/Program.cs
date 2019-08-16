using Dropbox.Api;
using Dropbox.Api.Files;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;

namespace DbxListFiles
{
    class Program
    {
        private const int ReadBufferSize = 128 * 1024;
        private bool Verbose = false;

        [Flags]
        private enum TestFileStatus
        {
            Success = 0,
            FileNotFound = 0x01,
            HashDiff = 0x02,
            DateTimeDiff = 0x04,
            SizeDiff = 0x08,
            FileNotFoundOther = 0x10,
            HashDiffOther = 0x20,
            DateTimeDiffOther = 0x40,
            SizeDiffOther = 0x80,
            HashDiffBoth = 0x200,
            DateTimeDiffBoth = 0x400,
            SizeDiffBoth = 0x800,
            Exception = 0x8000
        };

        private class TestFileResult
        {
            public TestFileStatus Status = TestFileStatus.Success;
            public string FileHash = string.Empty;
            public string ExceptionMessage = string.Empty;
            public DateTime LastWriteTime = DateTime.MinValue;
            public long Size = 0;
        };

        private static DateTime ParseDateTimeAsUtc(string value)
        {
            return DateTime.ParseExact(value, "s", DateTimeFormatInfo.InvariantInfo, DateTimeStyles.AssumeUniversal | DateTimeStyles.AdjustToUniversal);
        }

        private static long ParseNumberAsLong(string value)
        {
            return long.Parse(value, NumberStyles.None, NumberFormatInfo.InvariantInfo);
        }

        private async Task<TestFileResult> TestUserFile(string contentHash, DateTime clientModified, long size, string userFilePath)
        {
            var result = new TestFileResult();

            try
            {
                var fi = new FileInfo(userFilePath);

                if (!fi.Exists)
                {
                    result.Status = TestFileStatus.FileNotFound;
                    return result;
                }

                var lastWriteTimeUtc = fi.LastWriteTimeUtc;
                result.LastWriteTime = lastWriteTimeUtc.AddTicks(-(lastWriteTimeUtc.Ticks % TimeSpan.TicksPerSecond));
                result.Size = fi.Length;

                if (result.LastWriteTime != clientModified)
                {
                    result.Status |= TestFileStatus.DateTimeDiff;
                }

                if (result.Size != size)
                {
                    result.Status |= TestFileStatus.SizeDiff;
                }

                using (var hasher = new DropboxContentHasher())
                {
                    byte[] buf = new byte[ReadBufferSize];
                    int len;

                    using (var fs = new FileStream(userFilePath, FileMode.Open, FileAccess.Read, FileShare.Read, 4096, FileOptions.Asynchronous | FileOptions.SequentialScan))
                    {
                        while ((len = await fs.ReadAsync(buf, 0, buf.Length)) > 0)
                        {
                            hasher.TransformBlock(buf, 0, len, null, 0);
                        }
                    }

                    hasher.TransformFinalBlock(Array.Empty<byte>(), 0, 0);
                    result.FileHash = DropboxContentHasher.ToHex(hasher.Hash);

                    if (!string.Equals(result.FileHash, contentHash, StringComparison.OrdinalIgnoreCase))
                    {
                        result.Status |= TestFileStatus.HashDiff;
                    }
                }
            }
            catch (Exception ex)
            {
                result.Status |= TestFileStatus.Exception;
                result.ExceptionMessage = ex.Message;
            }

            return result;
        }

        private async Task TestUserFiles(string listFileName, string userRootPath, string otherRootPath)
        {
            int totalLineCount = 0;

            using (var reader = new StreamReader(listFileName))
            {
                while (await reader.ReadLineAsync() != null)
                {
                    ++totalLineCount;
                }
            }

            using (var reader = new StreamReader(listFileName))
            {
                int lineCount = 0;
                int fileCount = 0;

                int fileNotFoundCountUser = 0;
                int fileFoundCountUser = 0;
                int equalCountUser = 0;
                int lastWriteTimeCountUser = 0;
                int sizeCountUser = 0;
                int hashCountUser = 0;
                int errorCountUser = 0;

                int fileNotFoundCountOther = 0;
                int fileFoundCountOther = 0;
                int equalCountOther = 0;
                int lastWriteTimeCountOther = 0;
                int sizeCountOther = 0;
                int hashCountOther = 0;
                int errorCountOther = 0;

                int fileFoundCountBoth = 0;
                int equalCountOtherBoth = 0;
                int lastWriteTimeCountBoth = 0;
                int sizeCountBoth = 0;
                int hashCountBoth = 0;

                int progress = -1;

                string line;

                while ((line = await reader.ReadLineAsync()) != null)
                {
                    ++lineCount;

                    if (line[0] == '#')
                    {
                        continue;
                    }

                    if (!Console.IsOutputRedirected)
                    {
                        var current = (int)(lineCount * 100.0 / totalLineCount);

                        if (current != progress)
                        {
                            progress = current;
                            Console.CursorLeft = 0;
                            await Console.Out.WriteAsync(string.Format("{0}% ", progress));
                        }
                    }

                    // d7d3682cf1ca9a8e4d272cd72edcc709df75f2ee9cad71183fd50f210eb0cffa 2011-03-20T07:50:10 2011-03-20T07:50:10 268860 /Getting Started.pdf
                    //                                                                a^                  b^                  c^     d^

                    var a = line.IndexOf(' ');
                    var b = line.IndexOf(' ', a + 1);
                    var c = line.IndexOf(' ', b + 1);
                    var d = line.IndexOf(' ', c + 1);

                    if (!(a == 64 && b == 84 && c == 104 && d > c + 1))
                    {
                        await WriteLineAsync(string.Format("{0}: invalid line", lineCount));
                        continue;
                    }

                    var contentHash = line.Substring(0, a);
                    var serverModified = ParseDateTimeAsUtc(line.Substring(a + 1, b - (a + 1)));
                    var clientModified = ParseDateTimeAsUtc(line.Substring(b + 1, c - (b + 1)));
                    var size = ParseNumberAsLong(line.Substring(c + 1, d - (c + 1)));
                    var pathDisplay = line.Substring(d + 2); // Removes leading '/'

                    var userFilePath = userRootPath != null ? Path.Combine(userRootPath, pathDisplay) : null;
                    var otherFilePath = otherRootPath != null ? Path.Combine(otherRootPath, pathDisplay) : null;

                    var tasks = new Task<TestFileResult>[]
                    {
                        userFilePath != null ? TestUserFile(contentHash, clientModified, size, userFilePath) : Task.FromResult<TestFileResult>(null),
                        otherFilePath != null ? TestUserFile(contentHash, clientModified, size, otherFilePath) : Task.FromResult<TestFileResult>(null)
                    };

                    var results = await Task.WhenAll(tasks);

                    ++fileCount;

                    if (results[0] != null)
                    {
                        if (results[0].Status == TestFileStatus.Success)
                        {
                            ++fileFoundCountUser;
                            ++equalCountUser;

                            if (Verbose)
                            {
                                await WriteLineAsync(string.Format("[U] {0}: OK", pathDisplay));
                            }
                        }
                        else if (results[0].Status == TestFileStatus.FileNotFound)
                        {
                            ++fileNotFoundCountUser;

                            if (Verbose)
                            {
                                await WriteLineAsync(string.Format("[U] {0}: file not found", pathDisplay));
                            }
                        }
                        else
                        {
                            ++fileFoundCountUser;

                            var sb = new StringBuilder();

                            sb.Append("[U] ");
                            sb.Append(pathDisplay);
                            sb.Append(":");

                            if ((results[0].Status & TestFileStatus.DateTimeDiff) != 0)
                            {
                                ++lastWriteTimeCountUser;
                                sb.AppendFormat(" DateTimeError: [U={0} D={1}]", results[0].LastWriteTime, clientModified);
                            }

                            if ((results[0].Status & TestFileStatus.SizeDiff) != 0)
                            {
                                ++sizeCountUser;
                                sb.AppendFormat(" SizeError: [U={0} D={1}]", results[0].Size, size);
                            }

                            if ((results[0].Status & TestFileStatus.HashDiff) != 0)
                            {
                                ++hashCountUser;
                                sb.AppendFormat(" HashError: [U={0} D={1}]", results[0].FileHash, contentHash);
                            }

                            if ((results[0].Status & TestFileStatus.Exception) != 0)
                            {
                                ++errorCountUser;
                                sb.AppendFormat(" Exception: [{0}]", results[0].ExceptionMessage);
                            }

                            await WriteLineAsync(sb.ToString());
                        }
                    }

                    if (results[1] != null)
                    {
                        if (results[1].Status == TestFileStatus.Success)
                        {
                            ++fileFoundCountOther;
                            ++equalCountOther;

                            if (Verbose)
                            {
                                await WriteLineAsync(string.Format("[O] {0}: OK", pathDisplay));
                            }
                        }
                        else if (results[1].Status == TestFileStatus.FileNotFound)
                        {
                            ++fileNotFoundCountOther;

                            if (Verbose)
                            {
                                await WriteLineAsync(string.Format("[O] {0}: file not found", pathDisplay));
                            }
                        }
                        else
                        {
                            ++fileFoundCountOther;

                            var sb = new StringBuilder();

                            sb.Append("[O] ");
                            sb.Append(pathDisplay);
                            sb.Append(":");

                            if ((results[1].Status & TestFileStatus.DateTimeDiff) != 0)
                            {
                                ++lastWriteTimeCountOther;
                                sb.AppendFormat(" DateTimeError: [O={0} D={1}]", results[1].LastWriteTime, clientModified);
                            }

                            if ((results[1].Status & TestFileStatus.SizeDiff) != 0)
                            {
                                ++sizeCountOther;
                                sb.AppendFormat(" SizeError: [O={0} D={1}]", results[1].Size, size);
                            }

                            if ((results[1].Status & TestFileStatus.HashDiff) != 0)
                            {
                                ++hashCountOther;
                                sb.AppendFormat(" HashError: [O={0} D={1}]", results[1].FileHash, contentHash);
                            }

                            if ((results[1].Status & TestFileStatus.Exception) != 0)
                            {
                                ++errorCountOther;
                                sb.AppendFormat(" Exception: [{0}]", results[1].ExceptionMessage);
                            }

                            await WriteLineAsync(sb.ToString());
                        }
                    }

                    if (results[0] != null && results[1] != null)
                    {
                        if (results[0].Status != TestFileStatus.FileNotFound && (results[0].Status & TestFileStatus.Exception) == 0 &&
                            results[1].Status != TestFileStatus.FileNotFound && (results[1].Status & TestFileStatus.Exception) == 0)
                        {
                            ++fileFoundCountBoth;

                            var sbx = new StringBuilder();

                            if (results[0].LastWriteTime != results[1].LastWriteTime)
                            {
                                ++lastWriteTimeCountBoth;
                                sbx.AppendFormat(" DateTimeError: [U={0} O={1}]", results[0].LastWriteTime, results[1].LastWriteTime);
                            }

                            if (results[0].Size != results[1].Size)
                            {
                                ++sizeCountBoth;
                                sbx.AppendFormat(" SizeError: [U={0} O={1}]", results[0].Size, results[1].Size);
                            }

                            if (results[0].FileHash != results[1].FileHash)
                            {
                                ++hashCountBoth;
                                sbx.AppendFormat(" HashError: [U={0} O={1}]", results[0].FileHash, results[1].FileHash);
                            }

                            if (sbx.Length == 0)
                            {
                                ++equalCountOtherBoth;
                            }
                            else
                            {
                                await WriteLineAsync("[X] " + pathDisplay + ":" + sbx.ToString());
                            }
                        }
                    }
                }

                await Console.Out.WriteLineAsync();
                await Console.Out.WriteLineAsync(string.Format("Files in Dropbox..............................: {0}", fileCount));

                if (userRootPath != null)
                {
                    await Console.Out.WriteLineAsync();
                    await Console.Out.WriteLineAsync(string.Format("Files not found in 'user' folder..............: {0}", fileNotFoundCountUser));
                    await Console.Out.WriteLineAsync();
                    await Console.Out.WriteLineAsync(string.Format("Files found in 'user' folder..................: {0}", fileFoundCountUser));
                    await Console.Out.WriteLineAsync(string.Format("      with no differences.....................: {0}", equalCountUser));
                    await Console.Out.WriteLineAsync(string.Format("      with different LastWriteTime............: {0}", lastWriteTimeCountUser));
                    await Console.Out.WriteLineAsync(string.Format("      with different Size.....................: {0}", sizeCountUser));
                    await Console.Out.WriteLineAsync(string.Format("      with different Hash.....................: {0}", hashCountUser));
                    await Console.Out.WriteLineAsync(string.Format("      with read error.........................: {0}", errorCountUser));
                }

                if (otherRootPath != null)
                {
                    await Console.Out.WriteLineAsync();
                    await Console.Out.WriteLineAsync(string.Format("Files not found in 'other' folder.............: {0}", fileNotFoundCountOther));
                    await Console.Out.WriteLineAsync();
                    await Console.Out.WriteLineAsync(string.Format("Files found in 'other' folder.................: {0}", fileFoundCountOther));
                    await Console.Out.WriteLineAsync(string.Format("      with no differences.....................: {0}", equalCountOther));
                    await Console.Out.WriteLineAsync(string.Format("      with different LastWriteTime............: {0}", lastWriteTimeCountOther));
                    await Console.Out.WriteLineAsync(string.Format("      with different Size.....................: {0}", sizeCountOther));
                    await Console.Out.WriteLineAsync(string.Format("      with different Hash.....................: {0}", hashCountOther));
                    await Console.Out.WriteLineAsync(string.Format("      with read error.........................: {0}", errorCountOther));
                }

                if (userRootPath != null && otherRootPath != null)
                {
                    await Console.Out.WriteLineAsync();
                    await Console.Out.WriteLineAsync(string.Format("Files read in both 'user' and 'other' folders.: {0}", fileFoundCountBoth));
                    await Console.Out.WriteLineAsync(string.Format("      with no differences.....................: {0}", equalCountOtherBoth));
                    await Console.Out.WriteLineAsync(string.Format("      with different LastWriteTime............: {0}", lastWriteTimeCountBoth));
                    await Console.Out.WriteLineAsync(string.Format("      with different Size.....................: {0}", sizeCountBoth));
                    await Console.Out.WriteLineAsync(string.Format("      with different Hash.....................: {0}", hashCountBoth));
                }
            }
        }

        private async Task<string> GetAccessToken()
        {
            string generatedAccessToken = Environment.GetEnvironmentVariable("DROPBOX_ACCESS_TOKEN");

            if (string.IsNullOrEmpty(generatedAccessToken))
            {
                // TODO: Authenticate user and get token
            }

            return await Task.FromResult(generatedAccessToken);
        }

        private async Task ListFiles(ListFolderResult result, TextWriter writer)
        {
            foreach (var item in result.Entries.Where(i => i.IsFile))
            {
                var file = item.AsFile;
                await writer.WriteLineAsync(string.Format("{0} {1:s} {2:s} {3} {4}",
                    file.ContentHash, file.ServerModified, file.ClientModified, file.Size, item.PathDisplay));
            }
        }

        private async Task ListAllFiles(DropboxClient client, TextWriter writer)
        {
            var result = await client.Files.ListFolderAsync(string.Empty, true);

            await ListFiles(result, writer);

            while (result.HasMore)
            {
                result = await client.Files.ListFolderContinueAsync(result.Cursor);
                await ListFiles(result, writer);
            }
        }

        private async Task CreateListFile(string listFileName)
        {
            DropboxCertHelper.InitializeCertPinning();

            using (var httpClient = new HttpClient())
            {
                httpClient.Timeout = new TimeSpan(20 * TimeSpan.TicksPerSecond);

                var config = new DropboxClientConfig()
                {
                    HttpClient = httpClient
                };

                using (var client = new DropboxClient(await GetAccessToken(), config))
                {
                    using (var writer = new StreamWriter(listFileName))
                    {
                        await writer.WriteLineAsync("# ContentHash ServerModified ClientModified Size PathDisplay");
                        await ListAllFiles(client, writer);
                    }
                }
            }
        }

        private TextWriter OutputWriter = null;

        private async Task WriteLineAsync(string value)
        {
            if (OutputWriter != null)
            {
                await OutputWriter.WriteLineAsync(value);
            }
            else
            {
                await Console.Out.WriteLineAsync(value);
            }
        }

        private enum CommandOption
        {
            None,
            CreateListFile,
            TestUserFiles
        };

        private readonly string ProgramName = "DbxListFiles";
        private readonly string UnrecognizedArguments = "Unrecognized arguments.";

        private async Task Run(string[] args)
        {
            CommandOption option = CommandOption.None;
            string listFileName = null;
            string userRootPath = null;
            string otherRootPath = null;
            bool helpFlag = false;

            // dbxListFiles -c Dropbox.txt
            // dbxListFiles -t Dropbox.txt -u C:\User\Dropbox

            var p = new NDesk.Options.OptionSet()
            {
                { "c|create=", "create Dropbox data file", v => { option = CommandOption.CreateListFile; listFileName = v; } },
                { "t|test=", "test local files against Dropbox data file", v => { option = CommandOption.TestUserFiles; listFileName = v; }},
                { "u|user=", "user's Dropbox directory", v => userRootPath = v },
                { "o|other=", "other Dropbox directory", v => otherRootPath = v },
                { "r|report=", "output report file", v => OutputWriter = new StreamWriter(v) },
                { "v|verbose", "increase messages verbosity", v => Verbose = true },
                { "h|?|help", "show this message and exit", v => helpFlag = true }
            };

            try
            {
                var extra = p.Parse(args);

                if (helpFlag)
                {
                    using (var optionDescriptions = new StringWriter())
                    {
                        p.WriteOptionDescriptions(optionDescriptions);

                        await Console.Out.WriteLineAsync(string.Format("Usage: {0} [OPTIONS]", ProgramName));
                        await Console.Out.WriteLineAsync();
                        await Console.Out.WriteLineAsync(optionDescriptions.ToString().TrimEnd());
                    }

                    return;
                }

                if (extra.Count > 0)
                {
                    throw new NDesk.Options.OptionException(UnrecognizedArguments, string.Empty);
                }

                switch (option)
                {
                    case CommandOption.CreateListFile:

                        if (string.IsNullOrEmpty(listFileName) || !string.IsNullOrEmpty(userRootPath) || !string.IsNullOrEmpty(otherRootPath))
                        {
                            throw new NDesk.Options.OptionException(UnrecognizedArguments, string.Empty);
                        }

                        await CreateListFile(listFileName);
                        break;

                    case CommandOption.TestUserFiles:

                        if (string.IsNullOrEmpty(listFileName) || string.IsNullOrEmpty(userRootPath))
                        {
                            throw new NDesk.Options.OptionException(UnrecognizedArguments, string.Empty);
                        }

                        if (string.IsNullOrEmpty(otherRootPath))
                        {
                            await TestUserFiles(listFileName, userRootPath, null);
                        }
                        else
                        {
                            await TestUserFiles(listFileName, userRootPath, otherRootPath);
                        }

                        break;

                    default:
                        throw new NDesk.Options.OptionException(UnrecognizedArguments, string.Empty);
                }
            }
            catch (NDesk.Options.OptionException e)
            {
                await Console.Error.WriteLineAsync(string.Format("{0}: {1}", ProgramName, e.Message));
                await Console.Error.WriteLineAsync(string.Format("Try `{0} --help' for more information.", ProgramName));
            }
            finally
            {
                if (OutputWriter != null)
                {
                    OutputWriter.Dispose();
                }
            }
        }

        static async Task Main(string[] args)
        {
            try
            {
                var p = new Program();
                await p.Run(args);
            }
            catch (Exception ex)
            {
                await Console.Error.WriteLineAsync(ex.ToString());
                Environment.ExitCode = 3;
            }
        }
    }
}
