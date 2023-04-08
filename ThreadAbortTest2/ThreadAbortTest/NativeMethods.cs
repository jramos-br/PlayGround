using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace ThreadAbortTest
{
    internal static class NativeMethods
    {
        internal const string KERNEL32 = "kernel32.dll";
        internal const string USER32 = "user32.dll";
        internal const string GDI32 = "gdi32.dll";

        #region Error Mode

        internal const int SEM_FAILCRITICALERRORS = 0x0001;
        internal const int SEM_NOGPFAULTERRORBOX = 0x0002;
        internal const int SEM_NOALIGNMENTFAULTEXCEPT = 0x0004;
        internal const int SEM_NOOPENFILEERRORBOX = 0x8000;

        // UINT WINAPI GetErrorMode(void);
        [DllImport(KERNEL32, SetLastError = false)]
        internal static extern int GetErrorMode();

        // UINT WINAPI SetErrorMode(_In_ UINT uMode);
        [DllImport(KERNEL32, SetLastError = false)]
        internal static extern int SetErrorMode(int uMode);

        // DWORD WINAPI GetThreadErrorMode(void);
        [DllImport(KERNEL32, SetLastError = false)]
        internal static extern int GetThreadErrorMode();

        // BOOL WINAPI SetThreadErrorMode(_In_ DWORD dwNewMode, _Out_ LPDWORD lpOldMode);
        [DllImport(KERNEL32, SetLastError = true)]
        internal static extern bool SetThreadErrorMode(int dwNewMode, out int dwOldMode);

        #endregion
    }
}
