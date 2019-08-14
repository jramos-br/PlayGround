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
        private const int ReadBufferSize = 64 * 1024;
        private bool Verbose = false;

        [Flags]
        private enum TestUserFileStatus
        {
            Success = 0,
            FileNotFound = 1,
            HashDiff = 2,
            DateTimeDiff = 4,
            SizeDiff = 8,
            Exception = 16
        };

        private class TestUserFileResult
        {
            public TestUserFileStatus Status = TestUserFileStatus.Success;
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

        private async Task<TestUserFileResult> TestUserFile(string contentHash, DateTime clientModified, long size, string userFilePath)
        {
            var result = new TestUserFileResult();

            try
            {
                var fi = new FileInfo(userFilePath);

                if (!fi.Exists)
                {
                    result.Status = TestUserFileStatus.FileNotFound;
                    return result;
                }

                var lastWriteTimeUtc = fi.LastWriteTimeUtc;
                result.LastWriteTime = lastWriteTimeUtc.AddTicks(-(lastWriteTimeUtc.Ticks % TimeSpan.TicksPerSecond));
                result.Size = fi.Length;

                if (result.LastWriteTime != clientModified)
                {
                    result.Status |= TestUserFileStatus.DateTimeDiff;
                }

                if (result.Size != size)
                {
                    result.Status |= TestUserFileStatus.SizeDiff;
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
                        result.Status |= TestUserFileStatus.HashDiff;
                    }
                }
            }
            catch (Exception ex)
            {
                result.Status |= TestUserFileStatus.Exception;
                result.ExceptionMessage = ex.Message;
            }

            return result;
        }

        private async Task TestUserFiles(string listFileName, string userRootPath)
        {
            using (var reader = new StreamReader(listFileName))
            {
                int lineCount = 0;
                int fileCount = 0;
                int equalCount = 0;
                int foundCount = 0;
                int notFoundCount = 0;
                int errorCount = 0;
                string line;

                while ((line = await reader.ReadLineAsync()) != null)
                {
                    ++lineCount;

                    if (line[0] == '#')
                    {
                        continue;
                    }

                    // d7d3682cf1ca9a8e4d272cd72edcc709df75f2ee9cad71183fd50f210eb0cffa 2011-03-20T07:50:10 2011-03-20T07:50:10 268860 /Getting Started.pdf
                    //                                                                a^                  b^                  c^     d^

                    var a = line.IndexOf(' ');
                    var b = line.IndexOf(' ', a + 1);
                    var c = line.IndexOf(' ', b + 1);
                    var d = line.IndexOf(' ', c + 1);

                    if (!(a == 64 && b == 84 && c == 104 && d > c + 1))
                    {
                        await Console.Error.WriteLineAsync(string.Format("{0}: invalid line", lineCount));
                        continue;
                    }

                    var contentHash = line.Substring(0, a);
                    var serverModified = ParseDateTimeAsUtc(line.Substring(a + 1, b - (a + 1)));
                    var clientModified = ParseDateTimeAsUtc(line.Substring(b + 1, c - (b + 1)));
                    var size = ParseNumberAsLong(line.Substring(c + 1, d - (c + 1)));
                    var pathDisplay = line.Substring(d + 2); // Removes leading '/'

                    var userFilePath = Path.Combine(userRootPath, pathDisplay);

                    var result = await TestUserFile(contentHash, clientModified, size, userFilePath);

                    ++fileCount;

                    if (result.Status == TestUserFileStatus.Success)
                    {
                        ++foundCount;
                        ++equalCount;

                        if (Verbose)
                        {
                            await Console.Out.WriteLineAsync(string.Format("{0}: OK", pathDisplay));
                        }
                    }
                    else if (result.Status == TestUserFileStatus.FileNotFound)
                    {
                        ++notFoundCount;

                        if (Verbose)
                        {
                            await Console.Error.WriteLineAsync(string.Format("{0}: file not found", pathDisplay));
                        }
                    }
                    else
                    {
                        ++foundCount;
                        ++errorCount;

                        var sb = new StringBuilder();

                        sb.Append(pathDisplay);
                        sb.Append(":");

                        if ((result.Status & TestUserFileStatus.DateTimeDiff) != 0)
                        {
                            sb.AppendFormat(" DateTimeError: [L={0} D={1}]", result.LastWriteTime, clientModified);
                        }

                        if ((result.Status & TestUserFileStatus.SizeDiff) != 0)
                        {
                            sb.AppendFormat(" SizeError: [L={0} D={1}]", result.Size, size);
                        }

                        if ((result.Status & TestUserFileStatus.HashDiff) != 0)
                        {
                            sb.AppendFormat(" HashError: [L={0} D={1}]", result.FileHash, contentHash);
                        }

                        if ((result.Status & TestUserFileStatus.Exception) != 0)
                        {
                            sb.AppendFormat(" Exception: [{0}]", result.ExceptionMessage);
                        }

                        await Console.Error.WriteLineAsync(sb.ToString());
                    }
                }

                await Console.Out.WriteLineAsync(string.Format("Files in Dropbox.......................: {0}", fileCount));
                await Console.Out.WriteLineAsync(string.Format("Files not found in local Dropbox folder: {0}", notFoundCount));
                await Console.Out.WriteLineAsync(string.Format("Files found in local Dropbox folder....: {0}", foundCount));
                await Console.Out.WriteLineAsync(string.Format("Files with same date, size and content.: {0}", equalCount));
                await Console.Out.WriteLineAsync(string.Format("Files with differences.................: {0}", errorCount));
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
            bool helpFlag = false;

            // dbxListFiles -c Dropbox.txt
            // dbxListFiles -t Dropbox.txt -u C:\User\Dropbox

            var p = new NDesk.Options.OptionSet()
            {
                { "c|create=", "create Dropbox data file", v => { option = CommandOption.CreateListFile; listFileName = v; } },
                { "t|test=", "test local files against Dropbox data file", v => { option = CommandOption.TestUserFiles; listFileName = v; }},
                { "u|user=", "user's Dropbox directory", v => userRootPath = v },
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

                        if (string.IsNullOrEmpty(listFileName) || !string.IsNullOrEmpty(userRootPath))
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

                        await TestUserFiles(listFileName, userRootPath);
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
