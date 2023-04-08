using System;
using System.Collections.Generic;
using System.Configuration;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;

namespace AppUtil
{
    public class MyTextWriterTraceListener : TraceListener
    {
        public enum CreationScheduleOptions
        {
            None,
            Daily,
            Week
        }

        public enum LocationOptions
        {
            TempDirectory,
            LocalUserApplicationDirectory,
            UserApplicationDirectory,
            CommonApplicationDirectory,
            ExecutableDirectory,
            Custom
        }

        #region Constants

        private static readonly bool _compatibleOutputOptions = false;

        private const string _failMessage = "Falha:";

        #endregion

        #region Constructors

        // ATENÇÃO: Nenhuma dependência de Attributes pode ser usada nos construtores.

        public MyTextWriterTraceListener()
        {
        }

        public MyTextWriterTraceListener(string initializeData)
        {
            _initializeData = initializeData;
        }

        public MyTextWriterTraceListener(TextWriter writer, bool leaveOpen)
        {
            SetWriter(writer, leaveOpen, AutoFlush);
        }

        public MyTextWriterTraceListener(TextWriter writer, bool leaveOpen, bool autoFlush)
        {
            SetWriter(writer, leaveOpen, autoFlush);
        }

        public MyTextWriterTraceListener(TextWriter writer, bool leaveOpen, bool autoFlush, string name) : base(name)
        {
            SetWriter(writer, leaveOpen, autoFlush);
        }

        #endregion

        #region Utility Methods

        public static void Reset(TraceListenerCollection listeners, string listenerName)
        {
            var listener = listeners.OfType<MyTextWriterTraceListener>().FirstOrDefault(l => l.Name == listenerName);

            if (listener == null)
                throw new KeyNotFoundException($"{AppGlobal.GetMethodName(typeof(MyTextWriterTraceListener))}: Listener não encontrado. (listenerName=[{listenerName}])");

            listener.Reset();
        }

        public static string GetLogFileName(TraceListenerCollection listeners, string listenerName)
        {
            var listener = listeners.OfType<MyTextWriterTraceListener>().FirstOrDefault(l => l.Name == listenerName);

            if (listener == null)
                throw new KeyNotFoundException($"{AppGlobal.GetMethodName(typeof(MyTextWriterTraceListener))}: Listener não encontrado. (listenerName=[{listenerName}])");

            return listener.LogFileName;
        }

        public static string SetPartFileName(TraceListenerCollection listeners, string listenerName, string partFileName)
        {
            var listener = listeners.OfType<MyTextWriterTraceListener>().FirstOrDefault(l => l.Name == listenerName);

            if (listener == null)
                throw new KeyNotFoundException($"{AppGlobal.GetMethodName(typeof(MyTextWriterTraceListener))}: Listener não encontrado. (listenerName=[{listenerName}])");

            listener.Reset();

            try
            {
                listener.PartFileName = partFileName;
                return listener.LogFileName;
            }
            catch (Exception)
            {
                listener.Reset();
                throw;
            }
        }

        #endregion

        #region TraceListener Overrides

        protected override string[] GetSupportedAttributes()
        {
            var supportedAttributesExtra = GetSupportedAttributesExtra();
            if (supportedAttributesExtra == null)
                return _supportedAttributes.Clone() as string[];

            var result = new string[_supportedAttributes.Length + supportedAttributesExtra.Length];
            Array.Copy(_supportedAttributes, 0, result, 0, _supportedAttributes.Length);
            Array.Copy(supportedAttributesExtra, 0, result, _supportedAttributes.Length, supportedAttributesExtra.Length);
            return result;
        }

        public override void Write(string value)
        {
            if (Filter == null || Filter.ShouldTrace(null, string.Empty, TraceEventType.Verbose, 0, value, null, null, null))
            {
                WriteTrace(value);
            }
        }

        public override void WriteLine(string value)
        {
            if (Filter == null || Filter.ShouldTrace(null, string.Empty, TraceEventType.Verbose, 0, value, null, null, null))
            {
                WriteTraceLine(value);
            }
        }

        public override void Fail(string message)
        {
            if (Filter == null || Filter.ShouldTrace(null, string.Empty, TraceEventType.Critical, 0, message, null, null, null))
            {
                WriteFail(message, null);

                if (MustAbort())
                {
#if DEBUG
                    if (Debugger.IsAttached)
                        Debugger.Break();
                    else
                        throw new MyAssertException(FormatFailMessage(message));
#else
                    throw new MyAssertException(FormatFailMessage(message));
#endif
                }
            }
        }

        public override void Fail(string message, string detailMessage)
        {
            if (Filter == null || Filter.ShouldTrace(null, string.Empty, TraceEventType.Critical, 0, message, null, null, null))
            {
                WriteFail(message, detailMessage);

                if (MustAbort())
                {
#if DEBUG
                    if (Debugger.IsAttached)
                        Debugger.Break();
                    else
                        throw new MyAssertException(FormatFailMessage(message, detailMessage));
#else
                    throw new MyAssertException(FormatFailMessage(message, detailMessage));
#endif
                }
            }
        }

        public override void TraceData(TraceEventCache eventCache, string source, TraceEventType eventType, int id, object data)
        {
            if (Filter == null || Filter.ShouldTrace(eventCache, source, eventType, id, null, null, data, null))
            {
                WriteTraceHeader(eventCache, source, eventType, id);
                WriteTraceLine(FormatTraceData(data));
                WriteTraceFooter(eventCache);
            }
        }

        public override void TraceData(TraceEventCache eventCache, string source, TraceEventType eventType, int id, params object[] data)
        {
            if (Filter == null || Filter.ShouldTrace(eventCache, source, eventType, id, null, null, null, data))
            {
                WriteTraceHeader(eventCache, source, eventType, id);
                WriteTraceLine(FormatTraceData(data));
                WriteTraceFooter(eventCache);
            }
        }

        public override void TraceEvent(TraceEventCache eventCache, string source, TraceEventType eventType, int id)
        {
            TraceEvent(eventCache, source, eventType, id, null);
        }

        public override void TraceEvent(TraceEventCache eventCache, string source, TraceEventType eventType, int id, string message)
        {
            if (Filter == null || Filter.ShouldTrace(eventCache, source, eventType, id, message, null, null, null))
            {
                WriteTraceHeader(eventCache, source, eventType, id);
                WriteTraceLine(message);
                WriteTraceFooter(eventCache);
            }
        }

        public override void TraceEvent(TraceEventCache eventCache, string source, TraceEventType eventType, int id, string format, params object[] args)
        {
            if (Filter == null || Filter.ShouldTrace(eventCache, source, eventType, id, format, args, null, null))
            {
                WriteTraceHeader(eventCache, source, eventType, id);

                if (args != null)
                    WriteTraceLine(string.Format(format, args));
                else
                    WriteTraceLine(format);

                WriteTraceFooter(eventCache);
            }
        }

        public override void TraceTransfer(TraceEventCache eventCache, string source, int id, string message, Guid relatedActivityId)
        {
            if (string.IsNullOrEmpty(message))
                message = string.Concat("relatedActivityId=", relatedActivityId.ToString());
            else
                message = string.Concat(message, ", relatedActivityId=", relatedActivityId.ToString());

            TraceEvent(eventCache, source, TraceEventType.Transfer, id, message);
        }

        public override void Flush()
        {
            FlushWriter();
        }

        public override void Close()
        {
            Dispose();
        }

        #endregion

        #region Trace Methods

        protected virtual void WriteTraceHeader(TraceEventCache eventCache, string source, TraceEventType eventType, int id)
        {
            if (Header)
            {
                var eventTypeName = _compatibleOutputOptions ? eventType.ToString() : eventType.ToString().PadRight(11);
                WriteTrace(string.Concat(source, HeaderDelimiter, eventTypeName, HeaderDelimiter, id.ToString(), HeaderDelimiter));
            }

            if (eventCache != null && !_compatibleOutputOptions && TraceOutputOptions != TraceOptions.None)
            {
                if ((TraceOutputOptions & TraceOptions.ProcessId) != 0)
                {
                    WriteTrace(eventCache.ProcessId.ToString());
                    WriteTrace(HeaderDelimiter);
                }

                if ((TraceOutputOptions & TraceOptions.ThreadId) != 0)
                {
                    WriteTrace(eventCache.ThreadId);
                    WriteTrace(HeaderDelimiter);
                }

                if ((TraceOutputOptions & TraceOptions.DateTime) != 0)
                {
                    WriteTrace(eventCache.DateTime.UtcToLocal().ToString("G"));
                    WriteTrace(HeaderDelimiter);
                }

                if ((TraceOutputOptions & TraceOptions.Timestamp) != 0)
                {
                    WriteTrace(eventCache.Timestamp.ToString());
                    WriteTrace(HeaderDelimiter);
                }
            }
        }

        protected virtual void WriteTraceFooter(TraceEventCache eventCache)
        {
            if (eventCache != null && TraceOutputOptions != TraceOptions.None)
            {
                ++IndentLevel;

                try
                {
                    if (_compatibleOutputOptions)
                    {
                        if ((TraceOutputOptions & TraceOptions.ProcessId) != 0)
                            WriteTraceLine(string.Format("ProcessId={0}", eventCache.ProcessId.ToString()));
                    }

                    if ((TraceOutputOptions & TraceOptions.LogicalOperationStack) != 0)
                    {
                        var sb = new StringBuilder("LogicalOperationStack=");
                        var first = true;

                        foreach (object obj in eventCache.LogicalOperationStack)
                        {
                            if (first)
                                first = false;
                            else
                                sb.Append(", ");

                            sb.Append(obj.ToString());
                        }

                        WriteTraceLine(sb.ToString());
                    }

                    if (_compatibleOutputOptions)
                    {
                        if ((TraceOutputOptions & TraceOptions.ThreadId) != 0)
                            WriteTraceLine(string.Format("ThreadId={0}", eventCache.ThreadId));

                        if ((TraceOutputOptions & TraceOptions.DateTime) != 0)
                            WriteTraceLine(string.Format("DateTime={0}", eventCache.DateTime.ToString("o", CultureInfo.InvariantCulture)));

                        if ((TraceOutputOptions & TraceOptions.Timestamp) != 0)
                            WriteTraceLine(string.Format("Timestamp={0}", eventCache.Timestamp.ToString()));
                    }

                    if ((TraceOutputOptions & TraceOptions.Callstack) != 0)
                        WriteTraceLine(string.Format("Callstack=\n{0}", eventCache.Callstack));
                }
                finally
                {
                    --IndentLevel;
                }
            }
        }

        private string FormatTraceData(params object[] data)
        {
            if (data == null || data.Length == 0)
                return string.Empty;

            var text = new string[data.Length];
            var delimiter = DataDelimiter;
            var length = delimiter.Length * (data.Length - 1);

            for (var i = 0; i < data.Length; ++i)
            {
                if (data[i] != null)
                {
                    text[i] = data[i].ToString();
                    length += text[i].Length;
                }
            }

            var sb = new StringBuilder(length);

            AppendText(sb, text[0] ?? string.Empty);

            for (var i = 1; i < text.Length; ++i)
            {
                sb.Append(delimiter);
                AppendText(sb, text[i] ?? string.Empty);
            }

            return sb.ToString();
        }

        private void AppendText(StringBuilder sb, string text, int startIndex = 0)
        {
            AppendText(sb, text, startIndex, text.Length - startIndex);
        }

        private void AppendText(StringBuilder sb, string text, int startIndex, int count)
        {
            var pos = startIndex;

            while (count > 0)
            {
                var old = pos;

                if (char.IsWhiteSpace(text[pos]))
                {
                    do { ++pos; } while (--count > 0 && char.IsWhiteSpace(text[pos]));

                    sb.Append(' ', pos - old);
                }
                else
                {
                    do { ++pos; } while (--count > 0 && !char.IsWhiteSpace(text[pos]));

                    sb.Append(text, old, pos - old);
                }
            }
        }

        private void WriteFail(string message, string detailMessage)
        {
            var sb = new StringBuilder(_failMessage);

            if (message != null && message.Length > 0)
                sb.Append(' ').Append(message);

            if (detailMessage != null && detailMessage.Length > 0)
                sb.Append(' ').Append(detailMessage);

            var userFrame = false;

            MyDebug.FormatStackTrace(new StackTrace(true), line =>
            {
                if (!userFrame)
                {
                    if (line.IndexOf(typeof(MyTextWriterTraceListener).FullName, StringComparison.Ordinal) > 0)
                        return;

                    if (line.IndexOf(typeof(Debug).Namespace, StringComparison.Ordinal) > 0)
                        return;

                    userFrame = true;
                }

                sb.AppendLine().Append(line);
            });

            WriteTraceLine(sb.ToString());
        }

        private bool MustAbort()
        {
            bool result = true;
            var listeners = Trace.Listeners;

            for (int i = 0, n = listeners.Count; i < n; ++i)
            {
                var defaultTraceListener = listeners[i] as DefaultTraceListener;

                if (defaultTraceListener != null)
                {
                    result = !defaultTraceListener.AssertUiEnabled;
                    break;
                }
            }

            return result;
        }

        private string FormatFailMessage(string message)
        {
            return FormatFailMessage(message, null);
        }

        private string FormatFailMessage(string message, string detailMessage)
        {
            var sb = new StringBuilder(_failMessage);

            if (message != null && message.Length > 0)
                sb.Append(' ').Append(message);

            if (detailMessage != null && detailMessage.Length > 0)
                sb.Append(' ').Append(detailMessage);

            return sb.ToString();
        }

        #endregion

        #region Supported Attributes

        private const string _appendAttribute = "append";
        private const string _autoFlushAttribute = "autoFlush";
        private const string _baseFileNameAttribute = "baseFileName";
        private const string _creationScheduleAttribute = "creationSchedule";
        private const string _customLocationAttribute = "customLocation";
        private const string _dataDelimiterAttribute = "dataDelimiter";
        private const string _encodingAttribute = "encoding";
        private const string _headerAttribute = "header";
        private const string _headerDelimiterAttribute = "headerDelimiter";
        private const string _includeHostNameAttribute = "includeHostName";
        private const string _locationAttribute = "location";
        private const string _logFileExtensionAttribute = "logFileExtension";
        private const string _logFileNameAttribute = "logFileNameAttribute";
        private const string _partFileNameAttribute = "partFileName";
        private const string _traceOutputOptionsAttribute = "traceOutputOptions";

        private static readonly string[] _supportedAttributes = new string[]
        {
            _appendAttribute,
            _autoFlushAttribute,
            _baseFileNameAttribute,
            _creationScheduleAttribute,
            _customLocationAttribute,
            _dataDelimiterAttribute,
            _encodingAttribute,
            _headerAttribute,
            _headerDelimiterAttribute,
            _includeHostNameAttribute,
            _locationAttribute,
            _logFileExtensionAttribute,
            _logFileNameAttribute,
            _partFileNameAttribute,
            _traceOutputOptionsAttribute
        };

        protected virtual string[] GetSupportedAttributesExtra()
        {
            return null;
        }

        private bool? _append;
        private bool? _autoFlush;
        private string _baseFileName;
        private CreationScheduleOptions? _creationSchedule;
        private string _customLocation;
        private string _dataDelimiter;
        private Encoding _encoding;
        private bool? _header;
        private string _headerDelimiter;
        private bool? _includeHostName;
        private LocationOptions? _location;
        private string _logFileExtension;
        private string _logFileName;
        private string _partFileName;
        private TraceOptions? _traceOutputOptions;

        private void ResetSupportedAttributes()
        {
            _append = null;
            _autoFlush = null;
            _baseFileName = null;
            _creationSchedule = null;
            _customLocation = null;
            _dataDelimiter = null;
            _encoding = null;
            _header = null;
            _headerDelimiter = null;
            _includeHostName = null;
            _location = null;
            _logFileExtension = null;
            _logFileName = null;
            _partFileName = null;
            _traceOutputOptions = null;
        }

        public bool Append
        {
            get
            {
                if (_append == null)
                    _append = GetAttributeValue(_appendAttribute, true);

                return _append.Value;
            }
            set
            {
                _append = value;
            }
        }

        public bool AutoFlush
        {
            get
            {
                if (_autoFlush == null)
                    _autoFlush = GetAttributeValue(_autoFlushAttribute, System.Diagnostics.Trace.AutoFlush);

                return _autoFlush.Value;
            }
            set
            {
                _autoFlush = value;
            }
        }

        public string BaseFileName
        {
            get
            {
                if (_baseFileName == null)
                    _baseFileName = GetAttributeValue(_baseFileNameAttribute, () => MyApplication.ExecutableName);

                return _baseFileName;
            }
            set
            {
                _baseFileName = value;
            }
        }

        public CreationScheduleOptions CreationSchedule
        {
            get
            {
                if (_creationSchedule == null)
                    _creationSchedule = GetAttributeValue(_creationScheduleAttribute, CreationScheduleOptions.None);

                return _creationSchedule.Value;
            }
            set
            {
                _creationSchedule = ValidateCreationScheduleOptions(value, _creationScheduleAttribute);
            }
        }

        public string CustomLocation
        {
            get
            {
                if (_customLocation == null)
                    _customLocation = GetAttributeValue(_customLocationAttribute, () => MyApplication.UserAppDataPath);

                return _customLocation;
            }
            set
            {
                _customLocation = value;
            }
        }

        public string DataDelimiter
        {
            get
            {
                if (_dataDelimiter == null)
                    _dataDelimiter = GetAttributeValue(_dataDelimiterAttribute, "\t");

                return _dataDelimiter;
            }
            set
            {
                _dataDelimiter = value;
            }
        }

        public Encoding Encoding
        {
            get
            {
                if (_encoding == null)
                    _encoding = Encoding.GetEncoding(GetAttributeValue(_encodingAttribute) ?? "UTF-8",
                        EncoderFallback.ReplacementFallback, DecoderFallback.ReplacementFallback);

                return _encoding;
            }
            set
            {
                _encoding = value;
            }
        }

        public bool Header
        {
            get
            {
                if (_header == null)
                    _header = GetAttributeValue(_headerAttribute, true);

                return _header.Value;
            }
            set
            {
                _header = value;
            }
        }

        public string HeaderDelimiter
        {
            get
            {
                if (_headerDelimiter == null)
                    _headerDelimiter = GetAttributeValue(_headerDelimiterAttribute, "\t");

                return _headerDelimiter;
            }
            set
            {
                _headerDelimiter = value;
            }
        }

        public bool IncludeHostName
        {
            get
            {
                if (_includeHostName == null)
                    _includeHostName = GetAttributeValue(_includeHostNameAttribute, false);

                return _includeHostName.Value;
            }
            set
            {
                _includeHostName = value;
            }
        }

        public LocationOptions Location
        {
            get
            {
                if (_location == null)
                    _location = GetAttributeValue(_locationAttribute, LocationOptions.TempDirectory);

                return _location.Value;
            }
            set
            {
                _location = ValidateLocationOptions(value, _locationAttribute);
            }
        }

        public string LogFileExtension
        {
            get
            {
                if (_logFileExtension == null)
                    _logFileExtension = GetAttributeValue(_logFileExtensionAttribute, ".log");

                return _logFileExtension;
            }
            set
            {
                _logFileExtension = value;
            }
        }

        public string LogFileName
        {
            get
            {
                if (_logFileName == null)
                    _logFileName = GetAttributeValue(_logFileNameAttribute, () =>
                    {
                        var logDirName = null as string;

                        switch (Location)
                        {
                            case LocationOptions.TempDirectory:
                                logDirName = Path.GetTempPath();
                                break;
                            case LocationOptions.LocalUserApplicationDirectory:
                                logDirName = MyApplication.LocalUserAppDataPath;
                                break;
                            case LocationOptions.UserApplicationDirectory:
                                logDirName = MyApplication.UserAppDataPath;
                                break;
                            case LocationOptions.CommonApplicationDirectory:
                                logDirName = MyApplication.CommonAppDataPath;
                                break;
                            case LocationOptions.ExecutableDirectory:
                                logDirName = Path.GetDirectoryName(MyApplication.ExecutablePath);
                                break;
                            case LocationOptions.Custom:
                                logDirName = this.CustomLocation;
                                break;
                        }

                        if (logDirName == null)
                            logDirName = MyApplication.UserAppDataPath;

                        return Path.Combine(logDirName, string.Concat(BaseFileName, PartFileName, LogFileExtension));
                    });

                return _logFileName;
            }
            set
            {
                _logFileName = value;
            }
        }

        public string PartFileName
        {
            get
            {
                if (_partFileName == null)
                    _partFileName = GetAttributeValue(_partFileNameAttribute, string.Empty);

                return _partFileName;
            }
            set
            {
                _partFileName = value;
            }
        }

        public new TraceOptions TraceOutputOptions
        {
            get
            {
                if (_traceOutputOptions == null)
                    _traceOutputOptions = GetAttributeValue(_traceOutputOptionsAttribute, base.TraceOutputOptions);

                return _traceOutputOptions.Value;
            }
            set
            {
                _traceOutputOptions = ValidateTraceOptions(value, _traceOutputOptionsAttribute);
            }
        }

        #endregion

        #region Attributes Parsing and Validation

        protected string GetAttributeValue(string name, string defval)
        {
            var value = GetAttributeValue(name);
            return string.IsNullOrEmpty(value) ? defval : value;
        }

        protected string GetAttributeValue(string name, Func<string> defval)
        {
            var value = GetAttributeValue(name);

            try
            {
                return string.IsNullOrEmpty(value) ? defval() : value;
            }
            catch (Exception ex)
            {
                var message = new ArgumentOutOfRangeException($"{Name}.{name}", $"'{value}'", ex.Message).Message;
                throw new ConfigurationErrorsException(message);
            }
        }

        protected bool GetAttributeValue(string name, bool defval)
        {
            var value = GetAttributeValue(name);

            try
            {
                return string.IsNullOrEmpty(value) ? defval : ParseBool(value);
            }
            catch (Exception ex)
            {
                var message = new ArgumentOutOfRangeException($"{Name}.{name}", $"'{value}'", ex.Message).Message;
                throw new ConfigurationErrorsException(message);
            }
        }

        protected int GetAttributeValue(string name, int defval)
        {
            var value = GetAttributeValue(name);

            try
            {
                return string.IsNullOrEmpty(value) ? defval : int.Parse(value, NumberStyles.Integer);
            }
            catch (Exception ex)
            {
                var message = new ArgumentOutOfRangeException($"{Name}.{name}", $"'{value}'", ex.Message).Message;
                throw new ConfigurationErrorsException(message);
            }
        }

        protected CreationScheduleOptions GetAttributeValue(string name, CreationScheduleOptions defval)
        {
            var value = GetAttributeValue(name);

            try
            {
                var result = string.IsNullOrEmpty(value) ? defval : ParseEnum<CreationScheduleOptions>(value);
                return ValidateCreationScheduleOptions(result, name);
            }
            catch (Exception ex)
            {
                var message = new ArgumentOutOfRangeException($"{Name}.{name}", $"'{value}'", ex.Message).Message;
                throw new ConfigurationErrorsException(message);
            }
        }

        protected LocationOptions GetAttributeValue(string name, LocationOptions defval)
        {
            var value = GetAttributeValue(name);

            try
            {
                var result = string.IsNullOrEmpty(value) ? defval : ParseEnum<LocationOptions>(value);
                return ValidateLocationOptions(result, name);
            }
            catch (Exception ex)
            {
                var message = new ArgumentOutOfRangeException($"{Name}.{name}", $"'{value}'", ex.Message).Message;
                throw new ConfigurationErrorsException(message);
            }
        }

        protected TraceOptions GetAttributeValue(string name, TraceOptions defval)
        {
            var value = GetAttributeValue(name);

            try
            {
                var result = string.IsNullOrEmpty(value) ? defval : ParseEnum<TraceOptions>(value);
                return ValidateTraceOptions(result, name);
            }
            catch (Exception ex)
            {
                var message = new ArgumentOutOfRangeException($"{Name}.{name}", $"'{value}'", ex.Message).Message;
                throw new ConfigurationErrorsException(message);
            }
        }

        private string GetAttributeValue(string name)
        {
            ParseInitializeData();
            return Attributes[name];
        }

        private CreationScheduleOptions ValidateCreationScheduleOptions(CreationScheduleOptions value, string name)
        {
            ValidateEnum<CreationScheduleOptions>((int)value);
            return value;
        }

        private LocationOptions ValidateLocationOptions(LocationOptions value, string name)
        {
            ValidateEnum<LocationOptions>((int)value);
            return value;
        }

        private TraceOptions ValidateTraceOptions(TraceOptions value, string name)
        {
            ValidateEnum<TraceOptions>((int)value);
            return value;
        }

        private bool ParseBool(string value)
        {
            if (string.Equals(value, "0") || string.Equals(value, "no", StringComparison.OrdinalIgnoreCase))
                return false;

            if (string.Equals(value, "1") || string.Equals(value, "yes", StringComparison.OrdinalIgnoreCase))
                return true;

            return bool.Parse(value);
        }

        private T ParseEnum<T>(string value) where T : struct
        {
            var type = typeof(T);

            if (!type.IsEnum)
                throw new NotSupportedException();

            if (value.IndexOf(',') >= 0 && !type.IsDefined(typeof(FlagsAttribute), false))
            {
                // ConvertFromInvariantString must throw an exception
                new GenericEnumConverter(type).ConvertFromInvariantString(string.Empty);
                throw new ArgumentOutOfRangeException();
            }

            return (T)new GenericEnumConverter(type).ConvertFromInvariantString(value);
        }

        private void ValidateEnum<T>(int value) where T : struct
        {
            if (!IsDefined<T>(value))
            {
                // ConvertFromInvariantString must throw an exception
                new GenericEnumConverter(typeof(T)).ConvertFromInvariantString(string.Empty);
                throw new ArgumentOutOfRangeException();
            }
        }

        private bool IsDefined<T>(int value) where T : struct
        {
            var type = typeof(T);

            if (!type.IsEnum)
                throw new NotSupportedException();

            if (!type.IsDefined(typeof(FlagsAttribute), false))
                return Enum.IsDefined(type, value);

            // Counting bits set, Brian Kernighan's way
            for (int v = value; v != 0; v &= v - 1)
            {
                var bit = (v & -v); // least significant bit

                if (!Enum.IsDefined(type, bit))
                    return false;
            }

            return true;
        }

        #endregion

        #region InitializeData Parsing

        private string _initializeData;

        private void ParseInitializeData()
        {
            if (_initializeData != null)
            {
                int pos = 0;
                int end;

                while (pos < _initializeData.Length && (end = _initializeData.IndexOf(';', pos)) >= pos)
                {
                    ParseValue(_initializeData, pos, end);
                    pos = end + 1;
                }

                ParseValue(_initializeData, pos, _initializeData.Length);

                _initializeData = null;
            }
        }

        private static readonly char[] _separators = new char[] { ':', '=' };

        private void ParseValue(string data, int begin, int end)
        {
            var sep = data.IndexOfAny(_separators, begin, end - begin);

            if (sep < 0)
            {
                var pos1 = IndexOfNonWhiteSpace(data, begin, end);
                var pos2 = LastIndexOfNonWhiteSpace(data, pos1, end);

                if (pos2 > pos1 || end < data.Length)
                    ParseValue(data.Substring(pos1, pos2 - pos1), string.Empty, '\0');
            }
            else
            {
                var pos1 = IndexOfNonWhiteSpace(data, begin, sep);
                var pos2 = LastIndexOfNonWhiteSpace(data, pos1, sep);
                var name = data.Substring(pos1, pos2 - pos1);

                var pos3 = IndexOfNonWhiteSpace(data, sep + 1, end);
                var pos4 = LastIndexOfNonWhiteSpace(data, pos3, end);
                var value = data.Substring(pos3, pos4 - pos3);

                if (value.Length >= 2 && value[0] == '\'' && value[value.Length - 1] == '\'')
                    value = value.Substring(1, value.Length - 2);

                ParseValue(name, value, data[sep]);
            }
        }

        private void ParseValue(string name, string value, char sep)
        {
            if (name.Length == 0)
                throw new ConfigurationErrorsException(new ArgumentOutOfRangeException($"{Name}.initializeData", $"'{name}'", new ArgumentOutOfRangeException().Message).Message);

            if (value.IndexOf(sep) < 0)
            {
                var supportedAttributes = GetSupportedAttributes();

                for (int i = 0, n = supportedAttributes?.Length ?? 0; i < n; ++i)
                {
                    if (supportedAttributes[i] == name)
                    {
                        Attributes[name] = value;
                        return;
                    }
                }

                var supportedAttributesExtra = GetSupportedAttributesExtra();

                for (int i = 0, n = supportedAttributesExtra?.Length ?? 0; i < n; ++i)
                {
                    if (supportedAttributesExtra[i] == name)
                    {
                        Attributes[name] = value;
                        return;
                    }
                }
            }

            throw new ConfigurationErrorsException(new ArgumentOutOfRangeException($"{Name}.initializeData", $"'{name}'", new ArgumentOutOfRangeException().Message).Message);
        }

        private int IndexOfNonWhiteSpace(string value, int begin, int end)
        {
            var pos = begin;
            while (pos < end && char.IsWhiteSpace(value[pos]))
                ++pos;
            return pos;
        }

        private int LastIndexOfNonWhiteSpace(string value, int begin, int end)
        {
            var pos = end;
            while (pos > begin && char.IsWhiteSpace(value[pos - 1]))
                --pos;
            return pos;
        }

        #endregion

        #region IDisposable

        private bool _disposed;

        protected override void Dispose(bool disposing)
        {
            try
            {
                if (!_disposed)
                {
                    try
                    {
                        if (disposing)
                        {
                            CloseWriter();
                        }
                    }
                    finally
                    {
                        _disposed = true;
                    }
                }
            }
            finally
            {
                base.Dispose(disposing);
            }
        }

        private void CheckDisposed()
        {
            if (_disposed)
                throw new ObjectDisposedException(nameof(MyTextWriterTraceListener));
        }

        #endregion

        #region Writer

        private class MyWriter
        {
            private RefStream _writer;
            private StringBuilder _buffer;

            public MyWriter(string path, bool append, Encoding encoding, bool autoFlush) : this(RefStream.GetStream(path, append, encoding, autoFlush))
            {
            }

            public MyWriter(TextWriter writer, bool leaveOpen, bool autoFlush) : this(RefStream.GetStream(writer, leaveOpen, autoFlush))
            {
            }

            private MyWriter(RefStream writer)
            {
                if (writer == null)
                    throw new ArgumentNullException(nameof(writer));

                _writer = writer;
                _buffer = new StringBuilder();
            }

            public void Write(string value, int startIndex, int count)
            {
                CheckDisposed();

                _buffer.Append(value, startIndex, count);
            }

            public void WriteLine(string value, int startIndex, int count)
            {
                CheckDisposed();

                if (_buffer.Length > 0)
                {
                    _buffer.Append(value, startIndex, count);
                    _writer.WriteLine(GetDataAndClear());
                }
                else
                    _writer.WriteLine(value.Substring(startIndex, count));
            }

            public void Flush()
            {
                CheckDisposed();

                if (_buffer.Length > 0)
                    _writer.Write(GetDataAndClear());

                _writer.Flush();
            }

            public void Close()
            {
                if (_writer != null)
                {
                    try
                    {
                        if (_buffer.Length > 0)
                            _writer.Write(GetDataAndClear());

                        _writer.Close();
                    }
                    finally
                    {
                        _buffer = null;
                        _writer = null;
                    }
                }
            }

            private string GetDataAndClear()
            {
                var result = _buffer.ToString();
                _buffer.Clear();
                return result;
            }

            private void CheckDisposed()
            {
                if (_writer == null)
                    throw new ObjectDisposedException(nameof(MyWriter));
            }
        }

        private MyWriter _writer;

        private const int ERROR_SHARING_VIOLATION = 0x20;

        public void SetWriter(TextWriter writer, bool leaveOpen, bool autoFlush)
        {
            if (writer == null)
                throw new ArgumentNullException(nameof(writer));

            CheckDisposed();
            CloseWriter();

            _writer = new MyWriter(writer, leaveOpen, autoFlush);
        }

        public void Reset()
        {
            CheckDisposed();
            CloseWriter();
            ResetSupportedAttributes();
        }

        private MyWriter Writer
        {
            get
            {
                if (_writer == null)
                {
                    OnCreateWriter();

                    if (_writer == null)
                        _writer = CreateWriter();
                }

                return _writer;
            }
        }

        protected virtual void OnCreateWriter()
        {
        }

        private MyWriter CreateWriter()
        {
            var fileName = LogFileName;
            var fileExtension = null as string;
            var fileNameWithoutExtension = null as string;
            var seq = 0;

            for (;;)
            {
                try
                {
                    return new MyWriter(fileName, Append, Encoding, AutoFlush);
                }
                catch (IOException ex) when ((ex.HResult & 0xFFFF) == ERROR_SHARING_VIOLATION)
                {
                    if (seq == int.MaxValue)
                    {
                        throw ex;
                    }
                }

                if (seq++ == 0)
                {
                    fileNameWithoutExtension = Path.GetFileNameWithoutExtension(fileName);
                    fileExtension = Path.GetExtension(fileName);
                }

                fileName = string.Concat(fileNameWithoutExtension, "-", seq.ToString(CultureInfo.InvariantCulture), fileExtension);
            }
        }

        private void CloseWriter()
        {
            if (_writer != null)
            {
                try
                {
                    _writer.Close();
                }
                finally
                {
                    _writer = null;
                }
            }
        }

        private void FlushWriter()
        {
            CheckDisposed();

            if (_writer != null)
                _writer.Flush();
        }

        protected virtual void WriteTrace(string value)
        {
            CheckDisposed();

            if (value == null)
                value = string.Empty;

            var pos = 0;

            while (pos < value.Length)
            {
                var end = value.IndexOf('\n', pos);
                if (end < pos)
                    break;

                var eol = end > pos && value[end - 1] == '\r' ? end - 1 : end;

                if (NeedIndent)
                    WriteIndent();

                Writer.WriteLine(value, pos, eol - pos);
                NeedIndent = true;
                pos = end + 1;
            }

            if (pos < value.Length)
            {
                if (NeedIndent)
                    WriteIndent();

                Writer.Write(value, pos, value.Length - pos);
            }
        }

        protected virtual void WriteTraceLine(string value)
        {
            CheckDisposed();

            if (value == null)
                value = string.Empty;

            var pos = 0;

            while (pos < value.Length)
            {
                var end = value.IndexOf('\n', pos);
                if (end < pos)
                    break;

                var eol = end > pos && value[end - 1] == '\r' ? end - 1 : end;

                if (NeedIndent)
                    WriteIndent();

                Writer.WriteLine(value, pos, eol - pos);
                NeedIndent = true;
                pos = end + 1;
            }

            if (NeedIndent)
                WriteIndent();

            Writer.WriteLine(value, pos, value.Length - pos);
            NeedIndent = true;
        }

        #endregion

        #region Referenced Stream

        private class RefStream
        {
            private static Dictionary<string, RefStream> _streams =
                new Dictionary<string, RefStream>(StringComparer.OrdinalIgnoreCase);

            private int _refCount;
            private TextWriter _writer;
            private bool _leaveOpen;
            private bool _autoFlush;
            private string _fullPath;
            private object _lock = new object();

            public static RefStream GetStream(string path, bool append, Encoding encoding, bool autoFlush)
            {
                lock (_streams)
                {
                    var fileInfo = new FileInfo(path);
                    var fullName = fileInfo.FullName;

                    RefStream referencedStream;

                    if (_streams.TryGetValue(fullName, out referencedStream))
                        return referencedStream.AddReference();

                    if (append)
                    {
                        try
                        {
                            if (fileInfo.Exists && fileInfo.Length > 0)
                            {
                                using (var reader = new StreamReader(fullName, encoding, true))
                                {
                                    var ch = reader.Read();
                                    encoding = reader.CurrentEncoding;
                                }
                            }
                        }
                        catch (FileNotFoundException)
                        {
                        }
                    }

                    return new RefStream(fullName, append, encoding, autoFlush);
                }
            }

            public static RefStream GetStream(TextWriter writer, bool leaveOpen, bool autoFlush)
            {
                return new RefStream(writer, leaveOpen, autoFlush);
            }

            private RefStream AddReference()
            {
#if TRACE_REFSTREAM
                Console.Error.WriteLine($"AddReference {GetHashCode()} {_fullPath ?? "null"}");
#endif
                lock (_lock)
                {
                    ++_refCount;
                }

                return this;
            }

            private RefStream(string fullPath, bool append, Encoding encoding, bool autoFlush)
            {
#if TRACE_REFSTREAM
                Console.Error.WriteLine($"RefStream {GetHashCode()} {fullPath} {append} {encoding.EncodingName}");
#endif
                var writer = new StreamWriter(fullPath, append, encoding);
                writer.AutoFlush = autoFlush;

                _writer = writer;
                _fullPath = fullPath;
                _autoFlush = false;

                _streams.Add(_fullPath, this);
            }

            private RefStream(TextWriter writer, bool leaveOpen, bool autoFlush)
            {
#if TRACE_REFSTREAM
                Console.Error.WriteLine($"RefStream {GetHashCode()} {writer.ToString()} {leaveOpen}");
#endif
                _writer = writer;
                _leaveOpen = leaveOpen;
                _autoFlush = autoFlush;
            }

            public void Close()
            {
                lock (_streams)
                {
                    lock (_lock)
                    {
#if TRACE_REFSTREAM
                        Console.Error.WriteLine($"Close {GetHashCode()} {_fullPath ?? "null"} {_leaveOpen}");
#endif
                        if (_writer != null)
                        {
                            if (--_refCount < 0)
                            {
                                try
                                {
                                    if (_leaveOpen)
                                        _writer.Flush();
                                    else
                                        _writer.Close();
                                }
                                finally
                                {
                                    _writer = null;

                                    if (_fullPath != null)
                                        _streams.Remove(_fullPath);
                                }
                            }
                            else
                            {
                                _writer.Flush();
                            }
                        }
                    }
                }
            }

            public void Write(string value)
            {
                lock (_lock)
                {
                    CheckDisposed();

                    _writer.Write(value);

                    if (_autoFlush)
                        _writer.Flush();
                }
            }

            public void WriteLine(string value)
            {
                lock (_lock)
                {
                    CheckDisposed();

                    _writer.WriteLine(value);

                    if (_autoFlush)
                        _writer.Flush();
                }
            }

            public void Flush()
            {
                lock (_lock)
                {
                    CheckDisposed();

                    _writer.Flush();
                }
            }

            private void CheckDisposed()
            {
                if (_writer == null)
                    throw new ObjectDisposedException(nameof(RefStream));
            }
        }

        #endregion
    }
}
