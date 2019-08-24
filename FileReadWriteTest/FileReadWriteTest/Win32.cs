using Microsoft.Win32.SafeHandles;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace FileReadWriteTest
{
    static class Win32
    {
        public static int GetLastWin32Error()
        {
            return Marshal.GetLastWin32Error();
        }

        public static int GetHRForLastWin32Error()
        {
            return Marshal.GetHRForLastWin32Error();
        }

        public static void ThrowExceptionForHR(int errorCode)
        {
            Marshal.ThrowExceptionForHR(errorCode);
        }

        public static void ThrowExceptionForLastWin32Error()
        {
            Marshal.ThrowExceptionForHR(Marshal.GetHRForLastWin32Error());
        }

        [Flags]
        public enum DesiredAccess : uint
        {
            GenericAll = NativeMethods.GENERIC_ALL,
            GenericExecute = NativeMethods.GENERIC_EXECUTE,
            GenericRead = NativeMethods.GENERIC_READ,
            GenericReadWrite = NativeMethods.GENERIC_READ | NativeMethods.GENERIC_WRITE,
            GenericWrite = NativeMethods.GENERIC_WRITE
        }

        [Flags]
        public enum FileShareMode : uint
        {
            Delete = NativeMethods.FILE_SHARE_DELETE,
            None = NativeMethods.FILE_SHARE_NONE,
            Read = NativeMethods.FILE_SHARE_READ,
            ReadWrite = NativeMethods.FILE_SHARE_READ | NativeMethods.FILE_SHARE_WRITE,
            Write = NativeMethods.FILE_SHARE_WRITE
        }

        public enum FileCreationDisposition : uint
        {
            CreateAlways = NativeMethods.CREATE_ALWAYS,
            CreateNew = NativeMethods.CREATE_NEW,
            OpenAlways = NativeMethods.OPEN_ALWAYS,
            OpenExisting = NativeMethods.OPEN_EXISTING,
            TruncateExisting = NativeMethods.TRUNCATE_EXISTING
        }

        [Flags]
        public enum FileCreationFlags : uint
        {
            BackupSemantics = NativeMethods.FILE_FLAG_BACKUP_SEMANTICS,
            DeleteOnClose = NativeMethods.FILE_FLAG_DELETE_ON_CLOSE,
            FirstPipeInstance = NativeMethods.FILE_FLAG_FIRST_PIPE_INSTANCE,
            NoBuffering = NativeMethods.FILE_FLAG_NO_BUFFERING,
            None = NativeMethods.FILE_FLAG_NONE,
            OpenNoRecall = NativeMethods.FILE_FLAG_OPEN_NO_RECALL,
            OpenReparsePoint = NativeMethods.FILE_FLAG_OPEN_REPARSE_POINT,
            Overlapped = NativeMethods.FILE_FLAG_OVERLAPPED,
            PosixSemantics = NativeMethods.FILE_FLAG_POSIX_SEMANTICS,
            RandomAccess = NativeMethods.FILE_FLAG_RANDOM_ACCESS,
            SequentialScan = NativeMethods.FILE_FLAG_SEQUENTIAL_SCAN,
            SessionAware = NativeMethods.FILE_FLAG_SESSION_AWARE,
            WriteThrough = NativeMethods.FILE_FLAG_WRITE_THROUGH
        }

        [Flags]
        public enum FileCreationAttributes : uint
        {
            Archive = NativeMethods.FILE_ATTRIBUTE_ARCHIVE,
            Encrypted = NativeMethods.FILE_ATTRIBUTE_ENCRYPTED,
            Hidden = NativeMethods.FILE_ATTRIBUTE_HIDDEN,
            None = NativeMethods.FILE_ATTRIBUTE_NONE,
            Normal = NativeMethods.FILE_ATTRIBUTE_NORMAL,
            Offline = NativeMethods.FILE_ATTRIBUTE_OFFLINE,
            Readonly = NativeMethods.FILE_ATTRIBUTE_READONLY,
            System = NativeMethods.FILE_ATTRIBUTE_SYSTEM,
            Temporary = NativeMethods.FILE_ATTRIBUTE_TEMPORARY
        }

        [Flags]
        public enum FileAttributes : uint
        {
            Archive = NativeMethods.FILE_ATTRIBUTE_ARCHIVE,
            Compressed = NativeMethods.FILE_ATTRIBUTE_COMPRESSED,
            Device = NativeMethods.FILE_ATTRIBUTE_DEVICE,
            Directory = NativeMethods.FILE_ATTRIBUTE_DIRECTORY,
            EA = NativeMethods.FILE_ATTRIBUTE_EA,
            Encrypted = NativeMethods.FILE_ATTRIBUTE_ENCRYPTED,
            Hidden = NativeMethods.FILE_ATTRIBUTE_HIDDEN,
            IntegrityStream = NativeMethods.FILE_ATTRIBUTE_INTEGRITY_STREAM,
            None = NativeMethods.FILE_ATTRIBUTE_NONE,
            Normal = NativeMethods.FILE_ATTRIBUTE_NORMAL,
            NoScrubData = NativeMethods.FILE_ATTRIBUTE_NO_SCRUB_DATA,
            NotContentIndexed = NativeMethods.FILE_ATTRIBUTE_NOT_CONTENT_INDEXED,
            Offline = NativeMethods.FILE_ATTRIBUTE_OFFLINE,
            Pinned = NativeMethods.FILE_ATTRIBUTE_PINNED,
            Readonly = NativeMethods.FILE_ATTRIBUTE_READONLY,
            RecallOnDataAccess = NativeMethods.FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS,
            RecallOnOpen = NativeMethods.FILE_ATTRIBUTE_RECALL_ON_OPEN,
            ReparsePoint = NativeMethods.FILE_ATTRIBUTE_REPARSE_POINT,
            SparseFile = NativeMethods.FILE_ATTRIBUTE_SPARSE_FILE,
            StrictlySequential = NativeMethods.FILE_ATTRIBUTE_STRICTLY_SEQUENTIAL,
            System = NativeMethods.FILE_ATTRIBUTE_SYSTEM,
            Temporary = NativeMethods.FILE_ATTRIBUTE_TEMPORARY,
            Unpinned = NativeMethods.FILE_ATTRIBUTE_UNPINNED,
            Virtual = NativeMethods.FILE_ATTRIBUTE_VIRTUAL
        }

        public sealed class Pinnable<T> : IDisposable where T : new()
        {
            public Pinnable()
            {
                _data = new T();
            }

            public Pinnable(bool pin)
            {
                if (pin)
                {
                    Pin();
                }
            }

            private T _data;
            private GCHandle _handle;

            public void Pin() => _handle = GCHandle.Alloc(_data, GCHandleType.Pinned);
            public void Unpin() => _handle.Free();

            public static implicit operator T(Pinnable<T> instance) => instance._data;
            public static implicit operator IntPtr(Pinnable<T> instance) => instance._handle.AddrOfPinnedObject();

            public void Dispose()
            {
                if (_handle.IsAllocated)
                {
                    _handle.Free();
                }

                _data = default(T);
            }
        }

        public sealed class PinnableArray<T> : IDisposable
        {
            public PinnableArray(int length)
            {
                _data = new T[length];
            }

            public PinnableArray(int length, bool pin) : this(length)
            {
                if (pin)
                {
                    Pin();
                }
            }

            private T[] _data;
            private GCHandle _handle;

            public void Pin() => _handle = GCHandle.Alloc(_data, GCHandleType.Pinned);
            public void Unpin() => _handle.Free();

            public static implicit operator T[] (PinnableArray<T> array) => array._data;
            public static implicit operator IntPtr(PinnableArray<T> array) => array._handle.AddrOfPinnedObject();

            public void Dispose()
            {
                if (_handle.IsAllocated)
                {
                    _handle.Free();
                }

                _data = null;
            }
        }

        public class Overlapped
        {
            public NativeMethods.OVERLAPPED overlapped = NativeMethods.OVERLAPPED.Create();

            public IntPtr InternalLow { get => overlapped.Internal; set => overlapped.Internal = value; }
            public IntPtr InternalHigh { get => overlapped.InternalHigh; set => overlapped.InternalHigh = value; }
            public uint OffsetLow { get => overlapped.Offset; set => overlapped.Offset = value; }
            public uint OffsetHigh { get => overlapped.OffsetHigh; set => overlapped.OffsetHigh = value; }
            public ulong Offset
            {
                get { return ((ulong)overlapped.OffsetHigh << 32) | overlapped.Offset; }
                set { overlapped.Offset = unchecked((uint)value); overlapped.OffsetHigh = (uint)(value >> 32); }
            }
            public IntPtr EventHandle { get => overlapped.hEvent; set => overlapped.hEvent = value; }
        }

        public class SecurityAttributes
        {
            public NativeMethods.SECURITY_ATTRIBUTES sa = NativeMethods.SECURITY_ATTRIBUTES.Create();

            public uint Length { get => sa.nLength; set => sa.nLength = value; }
            public IntPtr SecurityDescriptor { get => sa.lpSecurityDescriptor; set => sa.lpSecurityDescriptor = value; }
            public bool InheritHandle { get => sa.bInheritHandle; set => sa.bInheritHandle = value; }
        }

        public static SafeFileHandle CreateFile(string lpFileName, DesiredAccess dwDesiredAccess, FileShareMode dwShareMode, SecurityAttributes lpSecurityAttributes, FileCreationDisposition dwCreationDisposition, FileCreationFlags dwFlags, FileCreationAttributes dwAttributes, SafeFileHandle hTemplateFile)
        {
            var hFile = NativeMethods.CreateFile(lpFileName,
                (uint)dwDesiredAccess,
                (uint)dwShareMode,
                ref lpSecurityAttributes.sa,
                (uint)dwCreationDisposition,
                (uint)dwFlags | (uint)dwAttributes,
                hTemplateFile);

            if (hFile.IsInvalid)
            {
                ThrowExceptionForLastWin32Error();
            }

            return hFile;
        }

        public static SafeFileHandle CreateFile(string lpFileName, DesiredAccess dwDesiredAccess, FileShareMode dwShareMode, SecurityAttributes lpSecurityAttributes, FileCreationDisposition dwCreationDisposition, FileCreationFlags dwFlags, FileCreationAttributes dwAttributes)
        {
            return CreateFile(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlags, dwAttributes, new SafeFileHandle(IntPtr.Zero, true));
        }

        public static int ReadFile(SafeFileHandle hFile, IntPtr lpBuffer, int nNumberOfBytesToRead)
        {
            if (!NativeMethods.ReadFile(hFile, lpBuffer, checked((uint)nNumberOfBytesToRead), out uint lpNumberOfBytesRead, IntPtr.Zero))
            {
                ThrowExceptionForLastWin32Error();
            }

            return checked((int)lpNumberOfBytesRead);
        }

        public static int ReadFile(SafeFileHandle hFile, IntPtr lpBuffer, int nNumberOfBytesToRead, ulong offset, SafeWaitHandle hEvent)
        {
            if (!NativeMethods.ReadFile(hFile, lpBuffer, checked((uint)nNumberOfBytesToRead), out uint lpNumberOfBytesRead, IntPtr.Zero))
            {
                ThrowExceptionForLastWin32Error();
            }

            return checked((int)lpNumberOfBytesRead);
        }

        public static void ClearFileCache(string path)
        {
            var dwDesiredAccess = Win32.DesiredAccess.GenericRead;
            var dwShareMode = Win32.FileShareMode.None;
            var lpSecurityAttributes = new Win32.SecurityAttributes();
            var dwDisposition = Win32.FileCreationDisposition.OpenExisting;
            var dwFlags = FileCreationFlags.NoBuffering;
            var dwAttributes = Win32.FileCreationAttributes.None;

            using (var hFile = CreateFile(path, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwDisposition, dwFlags, dwAttributes))
            {
            }
        }

        #region Memory API

        [Flags]
        public enum MemoryProtection : uint
        {
            Execute = NativeMethods.PAGE_EXECUTE,
            ExecuteRead = NativeMethods.PAGE_EXECUTE_READ,
            ExecuteReadWrite = NativeMethods.PAGE_EXECUTE_READWRITE,
            ExecuteWriteCopy = NativeMethods.PAGE_EXECUTE_WRITECOPY,
            NoAccess = NativeMethods.PAGE_NOACCESS,
            ReadOnly = NativeMethods.PAGE_READONLY,
            ReadWrite = NativeMethods.PAGE_READWRITE,
            WriteCopy = NativeMethods.PAGE_WRITECOPY,
            TargetsInvalid = NativeMethods.PAGE_TARGETS_INVALID,
            TargetsNoUpdate = NativeMethods.PAGE_TARGETS_NO_UPDATE,
            //
            Guard = NativeMethods.PAGE_GUARD,
            NoCache = NativeMethods.PAGE_NOCACHE,
            WriteCombine = NativeMethods.PAGE_WRITECOMBINE,
            //
            EnclaveThreadControl = NativeMethods.PAGE_ENCLAVE_THREAD_CONTROL,
            EnclaveUnvalidated = NativeMethods.PAGE_ENCLAVE_UNVALIDATED
        }

        [Flags]
        public enum AllocationType : uint
        {
            Commit = NativeMethods.MEM_COMMIT,
            Reserve = NativeMethods.MEM_RESERVE,
            Reset = NativeMethods.MEM_RESET,
            ResetUndo = NativeMethods.MEM_RESET_UNDO,
            //
            LargePages = NativeMethods.MEM_LARGE_PAGES,
            Physical = NativeMethods.MEM_PHYSICAL,
            TopDown = NativeMethods.MEM_TOP_DOWN,
            WriteWatch = NativeMethods.MEM_WRITE_WATCH
        }

        [Flags]
        public enum FreeType : uint
        {
            CoalescePlaceholders = NativeMethods.MEM_COALESCE_PLACEHOLDERS,
            PreservePlaceholder = NativeMethods.MEM_PRESERVE_PLACEHOLDER,
            Decommit = NativeMethods.MEM_DECOMMIT,
            Release = NativeMethods.MEM_RELEASE
        }

        public class UnmanagedBuffer : IDisposable
        {
            private IntPtr _buffer = IntPtr.Zero;

            public UnmanagedBuffer(ulong length)
            {
                _buffer = VirtualAlloc(length, AllocationType.Commit, MemoryProtection.ReadWrite);
            }

            public UnmanagedBuffer(int length) : this(checked((ulong)length))
            {
            }

            public static implicit operator IntPtr(UnmanagedBuffer block) => block._buffer;

            #region IDisposable Support

            private bool _disposed = false;

            protected virtual void Dispose(bool disposing)
            {
                if (!_disposed)
                {
                    if (_buffer != IntPtr.Zero)
                    {
                        VirtualFree(_buffer, 0, FreeType.Release);
                        _buffer = IntPtr.Zero;
                    }

                    _disposed = true;
                }
            }

            ~UnmanagedBuffer()
            {
                Dispose(false);
            }

            public void Dispose()
            {
                Dispose(true);
                GC.SuppressFinalize(this);
            }

            #endregion
        }

        public static IntPtr VirtualAlloc(ulong dwSize, AllocationType flAllocationType, MemoryProtection flProtect)
        {
            var result = NativeMethods.VirtualAlloc(IntPtr.Zero, (UIntPtr)dwSize, (uint)flAllocationType, (uint)flProtect);

            if (result == IntPtr.Zero)
            {
                ThrowExceptionForLastWin32Error();
            }

            return result;
        }

        public static void VirtualFree(IntPtr lpAddress, ulong dwSize, FreeType dwFreeType)
        {
            if (!NativeMethods.VirtualFree(lpAddress, (UIntPtr)dwSize, (uint)dwFreeType))
            {
                ThrowExceptionForLastWin32Error();
            }
        }

        #endregion
    }
}
