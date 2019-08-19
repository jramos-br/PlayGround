using Microsoft.Win32.SafeHandles;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace FileReadWriteTest
{
    // https://docs.microsoft.com/en-us/visualstudio/code-quality/ca1060-move-p-invokes-to-nativemethods-class
    internal static class NativeMethods
    {
        internal const string KERNEL32 = "kernel32.dll";
        internal const string USER32 = "user32.dll";
        internal const string GDI32 = "gdi32.dll";

        // typedef struct _SECURITY_ATTRIBUTES {
        //     DWORD nLength;
        //     LPVOID lpSecurityDescriptor;
        //     BOOL bInheritHandle;
        // } SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;

        [StructLayout(LayoutKind.Sequential)]
        internal struct SECURITY_ATTRIBUTES
        {
            internal uint nLength;
            internal IntPtr lpSecurityDescriptor;
            internal bool bInheritHandle;

            internal static SECURITY_ATTRIBUTES Create()
            {
                return new SECURITY_ATTRIBUTES
                {
                    nLength = (uint)Marshal.SizeOf(typeof(SECURITY_ATTRIBUTES))
                };
            }
        }

        // typedef struct _OVERLAPPED {
        //     ULONG_PTR Internal;
        //     ULONG_PTR InternalHigh;
        //     union {
        //         struct {
        //             DWORD Offset;
        //             DWORD OffsetHigh;
        //         } DUMMYSTRUCTNAME;
        //         PVOID Pointer;
        //     } DUMMYUNIONNAME;
        // 
        //     HANDLE  hEvent;
        // } OVERLAPPED, *LPOVERLAPPED;

        [StructLayout(LayoutKind.Sequential)]
        internal struct OVERLAPPED
        {
            internal IntPtr Internal;
            internal IntPtr InternalHigh;
            internal OVERLAPPED_1 u;
            internal IntPtr hEvent;

            [StructLayout(LayoutKind.Explicit)]
            internal struct OVERLAPPED_1
            {
                [FieldOffset(0)]
                internal OVERLAPPED_2 u;
                [FieldOffset(0)]
                internal IntPtr Pointer;
            }

            [StructLayout(LayoutKind.Sequential)]
            internal struct OVERLAPPED_2
            {
                internal uint Offset;
                internal uint OffsetHigh;
            }

            internal uint Offset { get => u.u.Offset; set => u.u.Offset = value; }
            internal uint OffsetHigh { get => u.u.OffsetHigh; set => u.u.OffsetHigh = value; }
            internal IntPtr Pointer { get => u.Pointer; set => u.Pointer = value; }

            internal static OVERLAPPED Create()
            {
                return new OVERLAPPED();
            }
        }

        #region Error Mode

        internal const uint SEM_FAILCRITICALERRORS = 0x0001;
        internal const uint SEM_NOGPFAULTERRORBOX = 0x0002;
        internal const uint SEM_NOALIGNMENTFAULTEXCEPT = 0x0004;
        internal const uint SEM_NOOPENFILEERRORBOX = 0x8000;

        // WINBASEAPI UINT WINAPI GetErrorMode(VOID);
        [DllImport(KERNEL32, SetLastError = false, ExactSpelling = true)]
        internal static extern uint GetErrorMode();

        // WINBASEAPI UINT WINAPI SetErrorMode(_In_ UINT uMode);
        [DllImport(KERNEL32, SetLastError = false, ExactSpelling = true)]
        internal static extern uint SetErrorMode(uint uMode);

        // WINBASEAPI DWORD WINAPI GetThreadErrorMode(VOID);
        [DllImport(KERNEL32, SetLastError = false, ExactSpelling = true)]
        internal static extern uint GetThreadErrorMode();

        // WINBASEAPI BOOL WINAPI SetThreadErrorMode(_In_ DWORD dwNewMode, _In_opt_ LPDWORD lpOldMode);
        [DllImport(KERNEL32, SetLastError = true, ExactSpelling = true)]
        internal static extern bool SetThreadErrorMode(uint dwNewMode, out uint lpOldMode);

        #endregion

        #region Masks for the predefined standard access types

        internal const uint DELETE = 0x00010000;
        internal const uint READ_CONTROL = 0x00020000;
        internal const uint WRITE_DAC = 0x00040000;
        internal const uint WRITE_OWNER = 0x00080000;
        internal const uint SYNCHRONIZE = 0x00100000;

        internal const uint STANDARD_RIGHTS_REQUIRED = 0x000F0000;

        internal const uint STANDARD_RIGHTS_READ = READ_CONTROL;
        internal const uint STANDARD_RIGHTS_WRITE = READ_CONTROL;
        internal const uint STANDARD_RIGHTS_EXECUTE = READ_CONTROL;
        internal const uint STANDARD_RIGHTS_ALL = 0x001F0000;

        internal const uint SPECIFIC_RIGHTS_ALL = 0x0000FFFF;

        #endregion

        #region AccessSystemAcl access type

        internal const uint ACCESS_SYSTEM_SECURITY = 0x01000000;

        #endregion

        #region Generic rights

        internal const uint GENERIC_READ = 0x80000000;
        internal const uint GENERIC_WRITE = 0x40000000;
        internal const uint GENERIC_EXECUTE = 0x20000000;
        internal const uint GENERIC_ALL = 0x10000000;

        #endregion

        #region Access rights to files and directories

        internal const uint FILE_READ_DATA = 0x0001;            // file & pipe
        internal const uint FILE_LIST_DIRECTORY = 0x0001;       // directory

        internal const uint FILE_WRITE_DATA = 0x0002;           // file & pipe
        internal const uint FILE_ADD_FILE = 0x0002;             // directory

        internal const uint FILE_APPEND_DATA = 0x0004;          // file
        internal const uint FILE_ADD_SUBDIRECTORY = 0x0004;     // directory
        internal const uint FILE_CREATE_PIPE_INSTANCE = 0x0004; // named pipe

        internal const uint FILE_READ_EA = 0x0008;              // file & directory
        internal const uint FILE_WRITE_EA = 0x0010;             // file & directory

        internal const uint FILE_EXECUTE = 0x0020;              // file
        internal const uint FILE_TRAVERSE = 0x0020;             // directory

        internal const uint FILE_DELETE_CHILD = 0x0040;         // directory

        internal const uint FILE_READ_ATTRIBUTES = 0x0080;      // all
        internal const uint FILE_WRITE_ATTRIBUTES = 0x0100;     // all

        internal const uint FILE_ALL_ACCESS = STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | 0x1FF;

        internal const uint FILE_GENERIC_READ = STANDARD_RIGHTS_READ | FILE_READ_DATA | FILE_READ_ATTRIBUTES | FILE_READ_EA | SYNCHRONIZE;
        internal const uint FILE_GENERIC_WRITE = STANDARD_RIGHTS_WRITE | FILE_WRITE_DATA | FILE_WRITE_ATTRIBUTES | FILE_WRITE_EA | FILE_APPEND_DATA | SYNCHRONIZE;
        internal const uint FILE_GENERIC_EXECUTE = STANDARD_RIGHTS_EXECUTE | FILE_READ_ATTRIBUTES | FILE_EXECUTE | SYNCHRONIZE;

        #endregion

        #region The requested sharing mode of the file or device

        internal const uint FILE_SHARE_NONE = 0x00000000;
        internal const uint FILE_SHARE_READ = 0x00000001;
        internal const uint FILE_SHARE_WRITE = 0x00000002;
        internal const uint FILE_SHARE_DELETE = 0x00000004;

        #endregion

        #region An action to take on a file or device that exists or does not exist

        internal const uint CREATE_NEW = 1;
        internal const uint CREATE_ALWAYS = 2;
        internal const uint OPEN_EXISTING = 3;
        internal const uint OPEN_ALWAYS = 4;
        internal const uint TRUNCATE_EXISTING = 5;

        #endregion

        #region The file or device attributes

        internal const uint FILE_ATTRIBUTE_NONE = 0x00000000;
        internal const uint FILE_ATTRIBUTE_READONLY = 0x00000001;
        internal const uint FILE_ATTRIBUTE_HIDDEN = 0x00000002;
        internal const uint FILE_ATTRIBUTE_SYSTEM = 0x00000004;
        internal const uint FILE_ATTRIBUTE_DIRECTORY = 0x00000010;
        internal const uint FILE_ATTRIBUTE_ARCHIVE = 0x00000020;
        internal const uint FILE_ATTRIBUTE_DEVICE = 0x00000040;
        internal const uint FILE_ATTRIBUTE_NORMAL = 0x00000080;
        internal const uint FILE_ATTRIBUTE_TEMPORARY = 0x00000100;
        internal const uint FILE_ATTRIBUTE_SPARSE_FILE = 0x00000200;
        internal const uint FILE_ATTRIBUTE_REPARSE_POINT = 0x00000400;
        internal const uint FILE_ATTRIBUTE_COMPRESSED = 0x00000800;
        internal const uint FILE_ATTRIBUTE_OFFLINE = 0x00001000;
        internal const uint FILE_ATTRIBUTE_NOT_CONTENT_INDEXED = 0x00002000;
        internal const uint FILE_ATTRIBUTE_ENCRYPTED = 0x00004000;
        internal const uint FILE_ATTRIBUTE_INTEGRITY_STREAM = 0x00008000;
        internal const uint FILE_ATTRIBUTE_VIRTUAL = 0x00010000;
        internal const uint FILE_ATTRIBUTE_NO_SCRUB_DATA = 0x00020000;
        internal const uint FILE_ATTRIBUTE_EA = 0x00040000;
        internal const uint FILE_ATTRIBUTE_PINNED = 0x00080000;
        internal const uint FILE_ATTRIBUTE_UNPINNED = 0x00100000;
        internal const uint FILE_ATTRIBUTE_RECALL_ON_OPEN = 0x00040000;
        internal const uint FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS = 0x00400000;
        internal const uint FILE_ATTRIBUTE_STRICTLY_SEQUENTIAL = 0x20000000;

        #endregion

        #region The file or device creation flags

        internal const uint FILE_FLAG_NONE = 0x00000000;
        internal const uint FILE_FLAG_WRITE_THROUGH = 0x80000000;
        internal const uint FILE_FLAG_OVERLAPPED = 0x40000000;
        internal const uint FILE_FLAG_NO_BUFFERING = 0x20000000;
        internal const uint FILE_FLAG_RANDOM_ACCESS = 0x10000000;
        internal const uint FILE_FLAG_SEQUENTIAL_SCAN = 0x08000000;
        internal const uint FILE_FLAG_DELETE_ON_CLOSE = 0x04000000;
        internal const uint FILE_FLAG_BACKUP_SEMANTICS = 0x02000000;
        internal const uint FILE_FLAG_POSIX_SEMANTICS = 0x01000000;
        internal const uint FILE_FLAG_SESSION_AWARE = 0x00800000;
        internal const uint FILE_FLAG_OPEN_REPARSE_POINT = 0x00200000;
        internal const uint FILE_FLAG_OPEN_NO_RECALL = 0x00100000;
        internal const uint FILE_FLAG_FIRST_PIPE_INSTANCE = 0x00080000;

        #endregion

        #region Miscellaneous Constants

        internal static readonly IntPtr INVALID_HANDLE_VALUE = new IntPtr(-1);

        internal const uint INVALID_FILE_SIZE = 0xFFFFFFFF;
        internal const uint INVALID_SET_FILE_POINTER = 0xFFFFFFFF;
        internal const uint INVALID_FILE_ATTRIBUTES = 0xFFFFFFFF;

        #endregion

        #region File I/O

        // WINBASEAPI HANDLE WINAPI CreateFileA(_In_ LPCSTR lpFileName, _In_ DWORD dwDesiredAccess, _In_ DWORD dwShareMode, _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes, _In_ DWORD dwCreationDisposition, _In_ DWORD dwFlagsAndAttributes, _In_opt_ HANDLE hTemplateFile);
        // WINBASEAPI HANDLE WINAPI CreateFileW(_In_ LPCWSTR lpFileName, _In_ DWORD dwDesiredAccess, _In_ DWORD dwShareMode, _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes, _In_ DWORD dwCreationDisposition, _In_ DWORD dwFlagsAndAttributes, _In_opt_ HANDLE hTemplateFile);
        [DllImport(KERNEL32, SetLastError = true, CharSet = CharSet.Auto, BestFitMapping = false)]
        internal static extern SafeFileHandle CreateFile(string lpFileName, uint dwDesiredAccess, uint dwShareMode, ref SECURITY_ATTRIBUTES lpSecurityAttributes, uint dwCreationDisposition, uint dwFlagsAndAttributes, SafeFileHandle hTemplateFile);

        // WINBASEAPI BOOL WINAPI ReadFile(_In_ HANDLE hFile, _Out_ LPVOID lpBuffer, _In_ DWORD nNumberOfBytesToRead, _Out_opt_ LPDWORD lpNumberOfBytesRead, _Inout_opt_ LPOVERLAPPED lpOverlapped);
        [DllImport(KERNEL32, SetLastError = true, ExactSpelling = true)]
        internal static extern bool ReadFile(SafeFileHandle hFile, IntPtr lpBuffer, uint nNumberOfBytesToRead, IntPtr mustBeZero, ref OVERLAPPED lpOverlapped);

        // WINBASEAPI BOOL WINAPI ReadFile(_In_ HANDLE hFile, _Out_ LPVOID lpBuffer, _In_ DWORD nNumberOfBytesToRead, _Out_opt_ LPDWORD lpNumberOfBytesRead, _Inout_opt_ LPOVERLAPPED lpOverlapped);
        [DllImport(KERNEL32, SetLastError = true, ExactSpelling = true)]
        internal static extern bool ReadFile(SafeFileHandle hFile, IntPtr lpBuffer, uint nNumberOfBytesToRead, out uint lpNumberOfBytesRead, IntPtr mustBeZero);

        // WINBASEAPI BOOL WINAPI WriteFile(_In_ HANDLE hFile, _In_ LPCVOID lpBuffer, _In_ DWORD nNumberOfBytesToWrite, _Out_opt_ LPDWORD lpNumberOfBytesWritten, _Inout_opt_ LPOVERLAPPED lpOverlapped);
        [DllImport(KERNEL32, SetLastError = true, ExactSpelling = true)]
        internal static extern bool WriteFile(SafeFileHandle hFile, IntPtr lpBuffer, uint nNumberOfBytesToWrite, IntPtr mustBeZero, ref OVERLAPPED lpOverlapped);

        // WINBASEAPI BOOL WINAPI WriteFile(_In_ HANDLE hFile, _In_ LPCVOID lpBuffer, _In_ DWORD nNumberOfBytesToWrite, _Out_opt_ LPDWORD lpNumberOfBytesWritten, _Inout_opt_ LPOVERLAPPED lpOverlapped);
        [DllImport(KERNEL32, SetLastError = true, ExactSpelling = true)]
        internal static extern bool WriteFile(SafeFileHandle hFile, IntPtr lpBuffer, uint nNumberOfBytesToWrite, out uint lpNumberOfBytesWritten, IntPtr mustBeZero);

        // WINBASEAPI BOOL WINAPI GetOverlappedResult(_In_ HANDLE hFile, _In_ LPOVERLAPPED lpOverlapped, _Out_ LPDWORD lpNumberOfBytesTransferred, _In_ BOOL bWait);
        [DllImport(KERNEL32, SetLastError = true, ExactSpelling = true)]
        internal static extern bool GetOverlappedResult(SafeFileHandle hFile, ref OVERLAPPED lpOverlapped, out uint lpNumberOfBytesTransferred, bool bWait);

        // WINBASEAPI BOOL WINAPI CancelIoEx(_In_ HANDLE hFile, _In_opt_ LPOVERLAPPED lpOverlapped);
        [DllImport(KERNEL32, SetLastError = true, ExactSpelling = true)]
        internal static extern bool CancelIoEx(SafeFileHandle hFile, ref OVERLAPPED lpOverlapped);

        // WINBASEAPI BOOL WINAPI CloseHandle(_In_ HANDLE hObject);
        [DllImport(KERNEL32, SetLastError = true, ExactSpelling = true)]
        internal static extern bool CloseHandle(IntPtr hObject);

        #endregion

        #region Memory API

        internal const uint PAGE_NOACCESS = 0x01;
        internal const uint PAGE_READONLY = 0x02;
        internal const uint PAGE_READWRITE = 0x04;
        internal const uint PAGE_WRITECOPY = 0x08;
        internal const uint PAGE_EXECUTE = 0x10;
        internal const uint PAGE_EXECUTE_READ = 0x20;
        internal const uint PAGE_EXECUTE_READWRITE = 0x40;
        internal const uint PAGE_EXECUTE_WRITECOPY = 0x80;
        internal const uint PAGE_GUARD = 0x100;
        internal const uint PAGE_NOCACHE = 0x200;
        internal const uint PAGE_WRITECOMBINE = 0x400;

        internal const uint PAGE_ENCLAVE_THREAD_CONTROL = 0x80000000;
        internal const uint PAGE_REVERT_TO_FILE_MAP = 0x80000000;
        internal const uint PAGE_TARGETS_NO_UPDATE = 0x40000000;
        internal const uint PAGE_TARGETS_INVALID = 0x40000000;
        internal const uint PAGE_ENCLAVE_UNVALIDATED = 0x20000000;
        internal const uint PAGE_ENCLAVE_DECOMMIT = 0x10000000;

        internal const uint MEM_COMMIT = 0x00001000;
        internal const uint MEM_RESERVE = 0x00002000;
        internal const uint MEM_REPLACE_PLACEHOLDER = 0x00004000;
        internal const uint MEM_RESERVE_PLACEHOLDER = 0x00040000;
        internal const uint MEM_RESET = 0x00080000;
        internal const uint MEM_TOP_DOWN = 0x00100000;
        internal const uint MEM_WRITE_WATCH = 0x00200000;
        internal const uint MEM_PHYSICAL = 0x00400000;
        internal const uint MEM_ROTATE = 0x00800000;
        internal const uint MEM_DIFFERENT_IMAGE_BASE_OK = 0x00800000;
        internal const uint MEM_RESET_UNDO = 0x01000000;
        internal const uint MEM_LARGE_PAGES = 0x20000000;
        internal const uint MEM_4MB_PAGES = 0x80000000;
        internal const uint MEM_64K_PAGES = MEM_LARGE_PAGES | MEM_PHYSICAL;
        internal const uint MEM_UNMAP_WITH_TRANSIENT_BOOST = 0x00000001;
        internal const uint MEM_COALESCE_PLACEHOLDERS = 0x00000001;
        internal const uint MEM_PRESERVE_PLACEHOLDER = 0x00000002;
        internal const uint MEM_DECOMMIT = 0x00004000;
        internal const uint MEM_RELEASE = 0x00008000;
        internal const uint MEM_FREE = 0x00010000;

        // WINBASEAPI LPVOID WINAPI VirtualAlloc(_In_opt_ LPVOID lpAddress, _In_ SIZE_T dwSize, _In_ DWORD flAllocationType, _In_ DWORD flProtect);
        [DllImport(KERNEL32, SetLastError = true, ExactSpelling = true)]
        internal static extern IntPtr VirtualAlloc(IntPtr lpAddress, UIntPtr dwSize, uint flAllocationType, uint flProtect);

        // WINBASEAPI BOOL WINAPI VirtualLock(_In_ LPVOID lpAddress, _In_ SIZE_T dwSize);
        [DllImport(KERNEL32, SetLastError = true, ExactSpelling = true)]
        internal static extern bool VirtualLock(IntPtr lpAddress, UIntPtr dwSize);

        // WINBASEAPI BOOL WINAPI VirtualUnlock(_In_ LPVOID lpAddress, _In_ SIZE_T dwSize);
        [DllImport(KERNEL32, SetLastError = true, ExactSpelling = true)]
        internal static extern bool VirtualUnlock(IntPtr lpAddress, UIntPtr dwSize);

        // WINBASEAPI BOOL WINAPI VirtualFree(_In_ LPVOID lpAddress, _In_ SIZE_T dwSize, _In_ DWORD dwFreeType);
        [DllImport(KERNEL32, SetLastError = true, ExactSpelling = true)]
        internal static extern bool VirtualFree(IntPtr lpAddress, UIntPtr dwSize, uint dwFreeType);

        #endregion
    }
}
