using Dropbox.Api;
using Dropbox.Api.Files;
using System;
using System.Collections.Generic;
using System.Diagnostics;
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
        private const int ReadBufferSize = 256 * 1024;
        private bool Verbose = false;

        private const int USER = 0;
        private const int OTHER = 1;
        private const int BOTH = 2;

        private enum TestType
        {
            TestContent,
            TestInfo,
            SetDate
        };

        private class File
        {
            public readonly string Type;
            public readonly string RootPath;
            public readonly string RelativePath;
            public readonly DateTime ServerLastWriteTime;
            public readonly long ServerSize;
            public readonly string ServerContentHash;

            private string _filePath;
            private FileInfo _fi;

            public bool HasFileInfo => _fi != null;

            public bool HasInfoTest { get; private set; }
            public bool HasContentTest { get; private set; }

            public File(string type, string rootPath, string relativePath, DateTime serverLastWriteTime, long serverSize, string serverContentHash)
            {
                Type = type;
                RootPath = rootPath;
                RelativePath = relativePath;
                ServerLastWriteTime = serverLastWriteTime;
                ServerSize = serverSize;
                ServerContentHash = serverContentHash;

                if (RootPath != null)
                {
                    _filePath = Path.Combine(RootPath, RelativePath);
                    _fi = new FileInfo(_filePath);
                }
            }

            public string FullName => HasFileInfo ? _fi.FullName : null;

            public bool Exists
            {
                get
                {
                    if (HasFileInfo)
                    {
                        if (_fi.Exists)
                        {
                            HasFileNotFound = false;
                            return true;
                        }

                        HasFileNotFound = true;
                    }

                    return false;
                }
            }

            public Task TestInfo()
            {
                if (!HasInfoTest && Exists)
                {
                    try
                    {
                        HasDateTimeDiff = LastWriteTime != ServerLastWriteTime;
                        HasSizeDiff = Size != ServerSize;
                    }
                    catch (Exception ex)
                    {
                        AddExceptionMessage(ex);
                    }

                    HasInfoTest = true;
                }

                return Task.CompletedTask;
            }

            public async Task TestContent()
            {
                if (!HasContentTest && Exists)
                {
                    try
                    {
                        var fileContentHash = await GetContentHash();
                        HasHashDiff = !string.Equals(fileContentHash, ServerContentHash, StringComparison.OrdinalIgnoreCase);
                    }
                    catch (Exception ex)
                    {
                        AddExceptionMessage(ex);
                    }

                    HasContentTest = true;
                }
            }

            private DateTime _lastWriteTime = DateTime.MinValue;

            public DateTime LastWriteTime
            {
                get
                {
                    if (_lastWriteTime == DateTime.MinValue && Exists)
                    {
                        var lastWriteTimeUtc = _fi.LastWriteTimeUtc;
                        var excess = lastWriteTimeUtc.Ticks % TimeSpan.TicksPerSecond;
                        _lastWriteTime = lastWriteTimeUtc.AddTicks(-excess);
                    }

                    return _lastWriteTime;
                }
            }

            public long _size = long.MinValue;

            public long Size
            {
                get
                {
                    if (_size == long.MinValue && Exists)
                    {
                        _size = _fi.Length;
                    }

                    return _size;
                }
            }

            private string _contentHash;

            public bool HasContentHash => _contentHash != null;
            public string ContentHash => _contentHash;

            public async Task<string> GetContentHash()
            {
                if (_contentHash == null && Exists)
                {
                    using (var hasher = new DropboxContentHasher())
                    {
                        byte[] buf = new byte[ReadBufferSize];
                        int len;

                        const FileOptions options = FileOptions.Asynchronous | FileOptions.SequentialScan;

                        using (var fs = new FileStream(_fi.FullName, FileMode.Open, FileAccess.Read, FileShare.Read, 4096, options))
                        {
                            while ((len = await fs.ReadAsync(buf, 0, buf.Length)) > 0)
                            {
                                hasher.TransformBlock(buf, 0, len, null, 0);
                            }
                        }

                        hasher.TransformFinalBlock(Array.Empty<byte>(), 0, 0);
                        _contentHash = DropboxContentHasher.ToHex(hasher.Hash);
                    }
                }

                return _contentHash;
            }

            public bool HasFileNotFound { get; private set; }
            public bool HasDateTimeDiff { get; private set; }
            public bool HasSizeDiff { get; private set; }
            public bool HasHashDiff { get; private set; }
            public bool HasException => _exceptionMessage != null;

            public bool HasError => HasFileNotFound || HasDateTimeDiff || HasSizeDiff || HasHashDiff || HasException;

            private string _exceptionMessage;

            public string ExceptionMessage => _exceptionMessage ?? string.Empty;

            public void AddExceptionMessage(string exceptionMessage)
            {
                if (string.IsNullOrEmpty(_exceptionMessage))
                    _exceptionMessage = exceptionMessage;
                else
                    _exceptionMessage += " | " + exceptionMessage;
            }

            public void AddExceptionMessage(Exception ex)
            {
                AddExceptionMessage(ex.Message);
            }
        }

        private class FileStats
        {
            public int FileNotFoundCount;
            public int FileFoundCount;
            public int EqualCount;
            public int LastWriteTimeCount;
            public int SizeCount;
            public int HashCount;
            public int ErrorCount;
        }

        private static DateTime ParseDateTimeAsUtc(string value)
        {
            return DateTime.ParseExact(value, "s", DateTimeFormatInfo.InvariantInfo, DateTimeStyles.AssumeUniversal | DateTimeStyles.AdjustToUniversal);
        }

        private static long ParseNumberAsLong(string value)
        {
            return long.Parse(value, NumberStyles.None, NumberFormatInfo.InvariantInfo);
        }

        private static async Task<string> CompareFiles(string path1, string path2)
        {
            async Task<int> ReadBufferAsync(Stream stream, byte[] buffer, int offset, int count)
            {
                while (count > 0)
                {
                    int length = await stream.ReadAsync(buffer, offset, count);

                    if (length == 0)
                    {
                        break;
                    }

                    offset += length;
                    count -= length;
                }

                return offset;
            }

            const FileOptions options = FileOptions.Asynchronous | FileOptions.SequentialScan;

            var sb = new StringBuilder();

            using (var fs1 = new FileStream(path1, FileMode.Open, FileAccess.Read, FileShare.Read, 4096, options))
            using (var fs2 = new FileStream(path2, FileMode.Open, FileAccess.Read, FileShare.Read, 4096, options))
            {
                if (fs1.Length != fs2.Length)
                {
                    sb.AppendFormat("Files have different sizes.");
                }
                else
                {
                    byte[] buf1 = new byte[17];
                    byte[] buf2 = new byte[17];

                    int differenceCount = 0;
                    int differenceLimit = 5;

                    long offset = 0;
                    long start = -1;
                    int state = 0;

                    while (differenceCount < differenceLimit)
                    {
                        var readTasks = new Task<int>[]
                        {
                            ReadBufferAsync(fs1, buf1, 0, buf1.Length),
                            ReadBufferAsync(fs2, buf2, 0, buf2.Length)
                        };

                        var lengths = await Task.WhenAll(readTasks);

                        if (lengths[0] != lengths[1])
                        {
                            throw new IOException();
                        }

                        int length = lengths[0];

                        if (length == 0)
                        {
                            break;
                        }

                        int i = 0;
                        int j;

                        while (i < length && differenceCount < differenceLimit)
                        {
                            if (state == 0) // Looking for different data
                            {
                                for (j = i; i < length && buf1[i] == buf2[i]; ++i)
                                    ;

                                offset += i - j;

                                if (i < length)
                                {
                                    start = offset;
                                    state = 1;
                                }
                            }
                            else // Skipping the difference
                            {
                                for (j = i; i < length && buf1[i] != buf2[i]; ++i)
                                    ;

                                offset += i - j;

                                if (i < length)
                                {
                                    if (sb.Length > 0)
                                    {
                                        sb.AppendLine();
                                    }

                                    sb.AppendFormat("Difference found at offset {0} (0x{0:X8}) length {1} (0x{1:X2}).", start, offset - start);

                                    if (++differenceCount >= differenceLimit)
                                    {
                                        sb.Append(" Difference limit exceeded. Comparison aborted.");
                                    }

                                    start = -1;
                                    state = 0;
                                }
                            }
                        }
                    }

                    if (start > 0)
                    {
                        if (sb.Length > 0)
                        {
                            sb.AppendLine();
                        }

                        sb.AppendFormat("Difference found at offset {0} (0x{0:X8}) length {1} (0x{1:X2}).", start, offset - start);
                    }
                }

            }

            return sb.ToString();
        }

        private static Task<string> CompareFiles(File file1, File file2)
        {
            return CompareFiles(file1.FullName, file2.FullName);
        }

        private async Task UpdateStats(FileStats stats, File file)
        {
            if (file.HasFileInfo)
            {
                if (!file.HasError)
                {
                    ++stats.FileFoundCount;
                    ++stats.EqualCount;

                    if (Verbose)
                    {
                        await WriteLineAsync(string.Format("[{0}] {1}: OK", file.Type, file.RelativePath));
                    }
                }
                else if (file.HasFileNotFound)
                {
                    ++stats.FileNotFoundCount;

                    if (Verbose)
                    {
                        await WriteLineAsync(string.Format("[{0}] {1}: file not found", file.Type, file.RelativePath));
                    }
                }
                else
                {
                    ++stats.FileFoundCount;

                    var sb = new StringBuilder();

                    sb.AppendFormat("[{0}] {1}:", file.Type, file.RelativePath);

                    if (file.HasDateTimeDiff)
                    {
                        ++stats.LastWriteTimeCount;
                        sb.AppendFormat(" DateTimeError: [{0}={1} D={2}]", file.Type, file.LastWriteTime, file.ServerLastWriteTime);
                    }

                    if (file.HasSizeDiff)
                    {
                        ++stats.SizeCount;
                        sb.AppendFormat(" SizeError: [{0}={1} D={2}]", file.Type, file.Size, file.ServerSize);
                    }

                    if (file.HasHashDiff)
                    {
                        ++stats.HashCount;
                        var contentHash = await file.GetContentHash();
                        sb.AppendFormat(" HashError: [{0}={1} D={2}]", file.Type, contentHash, file.ServerContentHash);
                    }

                    if (file.HasException)
                    {
                        ++stats.ErrorCount;
                        sb.AppendFormat(" Exception: [{0}={1}]", file.Type, file.ExceptionMessage);
                    }

                    await WriteLineAsync(sb.ToString());
                }
            }
        }

        private async Task UpdateStats(FileStats[] stats, File[] files)
        {
            await UpdateStats(stats[USER], files[USER]);
            await UpdateStats(stats[OTHER], files[OTHER]);

            if (files[USER].Exists && files[OTHER].Exists)
            {
                ++stats[BOTH].FileFoundCount;

                var sb = new StringBuilder();

                if (files[USER].LastWriteTime != files[OTHER].LastWriteTime)
                {
                    ++stats[BOTH].LastWriteTimeCount;
                    sb.AppendFormat(" DateTimeError: [{0}={1} {2}={3}]",
                        files[USER].Type, files[USER].LastWriteTime,
                        files[OTHER].Type, files[OTHER].LastWriteTime);
                }

                if (files[USER].Size != files[OTHER].Size)
                {
                    ++stats[BOTH].SizeCount;
                    sb.AppendFormat(" SizeError: [{0}={1} {2}={3}]",
                        files[USER].Type, files[USER].Size,
                        files[OTHER].Type, files[OTHER].Size);
                }

                var userContentHash = await files[USER].GetContentHash();
                var otherContentHash = await files[OTHER].GetContentHash();

                if (!string.Equals(userContentHash, otherContentHash, StringComparison.OrdinalIgnoreCase))
                {
                    ++stats[BOTH].HashCount;
                    sb.AppendFormat(" HashError: [{0}={1} {2}={3}]",
                        files[USER].Type, userContentHash,
                        files[OTHER].Type, otherContentHash);
                }

                if (sb.Length == 0)
                {
                    ++stats[BOTH].EqualCount;
                }
                else
                {
                    await WriteLineAsync("[X] " + files[USER].RelativePath + ":" + sb.ToString());

                    if (!string.Equals(files[USER].RelativePath, files[OTHER].RelativePath, StringComparison.OrdinalIgnoreCase))
                    {
                        await WriteLineAsync("[Y] " + files[OTHER].RelativePath + ":" + sb.ToString());
                    }

                    if (!string.Equals(userContentHash, otherContentHash, StringComparison.OrdinalIgnoreCase))
                    {
                        var message = await CompareFiles(files[USER], files[OTHER]);
                        var lines = message.Split(new char[] { '\r', '\n' }, StringSplitOptions.RemoveEmptyEntries);

                        foreach (var line in lines)
                        {
                            await WriteLineAsync("[Z] " + files[USER].RelativePath + ": " + line);
                        }
                    }
                }
            }
        }

        private async Task TestFiles(string listFileName, string userRootPath, string otherRootPath, TestType testType)
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
                var stats = new FileStats[]
                {
                    new FileStats(), // USER
                    new FileStats(), // OTHER
                    new FileStats()  // BOTH
                };

                int lineCount = 0;
                int fileCount = 0;
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
                    var relativePath = line.Substring(d + 2); // Removes leading '/'

                    //if (!relativePath.StartsWith("colortel/administração/Faturamento", StringComparison.OrdinalIgnoreCase))
                    //    continue;

                    var files = new File[]
                    {
                        new File("U", userRootPath, relativePath, clientModified, size, contentHash),
                        new File("O", otherRootPath, relativePath, clientModified, size, contentHash)
                    };

                    var testFileInfoTasks = new Task[]
                    {
                        files[0].TestInfo(),
                        files[1].TestInfo()
                    };

                    await Task.WhenAll(testFileInfoTasks);

                    if (testType == TestType.TestContent)
                    {
                        var testFileContentTasks = new Task[]
                        {
                            files[0].TestContent(),
                            files[1].TestContent()
                        };

                        await Task.WhenAll(testFileContentTasks);
                    }
                    else if (testType == TestType.SetDate)
                    {
                        if (files[USER].HasError || files[OTHER].HasError)
                        {
                            var testFileContentTasks = new Task[]
                            {
                            files[0].TestContent(),
                            files[1].TestContent()
                            };

                            await Task.WhenAll(testFileContentTasks);
                        }
                    }

                    ++fileCount;

                    await UpdateStats(stats, files);
                }

                await Console.Out.WriteLineAsync();
                await Console.Out.WriteLineAsync(string.Format("Files in Dropbox..............................: {0}", fileCount));

                if (userRootPath != null)
                {
                    await Console.Out.WriteLineAsync();
                    await Console.Out.WriteLineAsync(string.Format("Files not found in 'user' folder..............: {0}", stats[USER].FileNotFoundCount));
                    await Console.Out.WriteLineAsync();
                    await Console.Out.WriteLineAsync(string.Format("Files found in 'user' folder..................: {0}", stats[USER].FileFoundCount));
                    await Console.Out.WriteLineAsync(string.Format("      with no differences.....................: {0}", stats[USER].EqualCount));
                    await Console.Out.WriteLineAsync(string.Format("      with different LastWriteTime............: {0}", stats[USER].LastWriteTimeCount));
                    await Console.Out.WriteLineAsync(string.Format("      with different Size.....................: {0}", stats[USER].SizeCount));
                    await Console.Out.WriteLineAsync(string.Format("      with different Hash.....................: {0}", stats[USER].HashCount));
                    await Console.Out.WriteLineAsync(string.Format("      with read error.........................: {0}", stats[USER].ErrorCount));
                }

                if (otherRootPath != null)
                {
                    await Console.Out.WriteLineAsync();
                    await Console.Out.WriteLineAsync(string.Format("Files not found in 'other' folder.............: {0}", stats[OTHER].FileNotFoundCount));
                    await Console.Out.WriteLineAsync();
                    await Console.Out.WriteLineAsync(string.Format("Files found in 'other' folder.................: {0}", stats[OTHER].FileFoundCount));
                    await Console.Out.WriteLineAsync(string.Format("      with no differences.....................: {0}", stats[OTHER].EqualCount));
                    await Console.Out.WriteLineAsync(string.Format("      with different LastWriteTime............: {0}", stats[OTHER].LastWriteTimeCount));
                    await Console.Out.WriteLineAsync(string.Format("      with different Size.....................: {0}", stats[OTHER].SizeCount));
                    await Console.Out.WriteLineAsync(string.Format("      with different Hash.....................: {0}", stats[OTHER].HashCount));
                    await Console.Out.WriteLineAsync(string.Format("      with read error.........................: {0}", stats[OTHER].ErrorCount));
                }

                if (userRootPath != null && otherRootPath != null)
                {
                    await Console.Out.WriteLineAsync();
                    await Console.Out.WriteLineAsync(string.Format("Files found in both 'user' and 'other' folders: {0}", stats[BOTH].FileFoundCount));
                    await Console.Out.WriteLineAsync(string.Format("      with no differences.....................: {0}", stats[BOTH].EqualCount));
                    await Console.Out.WriteLineAsync(string.Format("      with different LastWriteTime............: {0}", stats[BOTH].LastWriteTimeCount));
                    await Console.Out.WriteLineAsync(string.Format("      with different Size.....................: {0}", stats[BOTH].SizeCount));
                    await Console.Out.WriteLineAsync(string.Format("      with different Hash.....................: {0}", stats[BOTH].HashCount));
                    await Console.Out.WriteLineAsync(string.Format("      with read error.........................: {0}", stats[BOTH].ErrorCount));
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

            await Console.Out.WriteLineAsync(value);
        }

        private enum CommandOption
        {
            None,
            CreateListFile,
            TestUserFilesContent,
            TestUserFilesInfo,
            SetUserFilesDate
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
                { "t|test=", "test local files content against Dropbox data file", v => { option = CommandOption.TestUserFilesContent; listFileName = v; }},
                { "i|info=", "test local files info against Dropbox data file", v => { option = CommandOption.TestUserFilesInfo; listFileName = v; } },
                { "d|date=", "set local files dates from Dropbox data file", v => { option = CommandOption.SetUserFilesDate; listFileName = v; } },
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

                if (string.IsNullOrWhiteSpace(userRootPath))
                {
                    userRootPath = null;
                }

                if (string.IsNullOrWhiteSpace(otherRootPath))
                {
                    otherRootPath = null;
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

                    case CommandOption.TestUserFilesContent:

                        if (string.IsNullOrEmpty(listFileName) || string.IsNullOrEmpty(userRootPath))
                        {
                            throw new NDesk.Options.OptionException(UnrecognizedArguments, string.Empty);
                        }

                        await TestFiles(listFileName, userRootPath, otherRootPath, TestType.TestContent);
                        break;

                    case CommandOption.TestUserFilesInfo:

                        if (string.IsNullOrEmpty(listFileName) || string.IsNullOrEmpty(userRootPath))
                        {
                            throw new NDesk.Options.OptionException(UnrecognizedArguments, string.Empty);
                        }

                        await TestFiles(listFileName, userRootPath, otherRootPath, TestType.TestInfo);
                        break;

                    case CommandOption.SetUserFilesDate:

                        if (string.IsNullOrEmpty(listFileName) || string.IsNullOrEmpty(userRootPath))
                        {
                            throw new NDesk.Options.OptionException(UnrecognizedArguments, string.Empty);
                        }

                        await TestFiles(listFileName, userRootPath, otherRootPath, TestType.SetDate);
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
                using (new MyCrono(sw => Console.Out.WriteLine("Elapsed: {0}", sw.Elapsed)))
                {
                    var p = new Program();
                    await p.Run(args);
                }
            }
            catch (Exception ex)
            {
                await Console.Error.WriteLineAsync(ex.ToString());
                Environment.ExitCode = 3;
            }
        }
    }
}
