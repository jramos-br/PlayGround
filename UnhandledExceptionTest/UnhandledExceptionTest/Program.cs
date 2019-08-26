// The UnhandledExceptionTest program.
//
// This code sample demonstrates how to handle unhandled exceptions in a console
// application, avoiding the default exception handler report and disabling the
// WER dialog and crash report.
//
// You can change App.config to specify whether EventReporter logs fatal errors
// in the Windows event log 
//
// Copyright (c) 2019 Jorge Ramos (jramos at pobox dot com)
//
// This software is provided "as is", without any express or implied warranty.
// Use it at your own risk. In no event shall the authors or copyright holders
// be held liable for any damages arising from the use of this software. Use
// and redistribution in source and binary forms, with or without modification,
// are permitted. Altered source versions must be plainly marked as such.

using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace UnhandledExceptionTest
{
    static class NativeMethods
    {
        internal const string KERNEL32 = "kernel32.dll";

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

        internal const uint STD_INPUT_HANDLE = unchecked((uint)-10);
        internal const uint STD_OUTPUT_HANDLE = unchecked((uint)-11);
        internal const uint STD_ERROR_HANDLE = unchecked((uint)-12);

        // WINBASEAPI HANDLE WINAPI GetStdHandle(_In_ DWORD nStdHandle);
        [DllImport(KERNEL32, SetLastError = true, ExactSpelling = true)]
        internal static extern IntPtr GetStdHandle(uint nStdHandle);

        // WINBASEAPI BOOL WINAPI SetStdHandle(_In_ DWORD nStdHandle, _In_ HANDLE hHandle);
        [DllImport(KERNEL32, SetLastError = true, ExactSpelling = true)]
        internal static extern bool SetStdHandle(uint nStdHandle, IntPtr hHandle);

        // WINBASEAPI BOOL WINAPI CloseHandle(_In_ HANDLE hObject);
        [DllImport(KERNEL32, SetLastError = true, ExactSpelling = true)]
        internal static extern bool CloseHandle(IntPtr hObject);
    }

    class Program
    {
        static Program()
        {
            // Disable the Windows Error Reporting dialog and crash report.
            NativeMethods.SetErrorMode(NativeMethods.GetErrorMode() | NativeMethods.SEM_NOGPFAULTERRORBOX);

            // Define the unhandled exception handler.
            AppDomain.CurrentDomain.UnhandledException += UnhandledExceptionEventHandler;

            // Avoid the default exception handler report.
            AppDomain.CurrentDomain.UnhandledException += CloseStderr;
        }

        private static void UnhandledExceptionEventHandler(object sender, UnhandledExceptionEventArgs e)
        {
            Console.Error.WriteLine("{0}: IsTerminating={1} ExceptionObject=[{2}]",
                nameof(UnhandledExceptionEventHandler),
                e.IsTerminating,
                e.ExceptionObject is Exception ex ? ex.Message : e.ExceptionObject);
        }

        private static void CloseStderr(object sender, UnhandledExceptionEventArgs e)
        {
            var hHandle = NativeMethods.GetStdHandle(NativeMethods.STD_ERROR_HANDLE);

            if (hHandle != IntPtr.Zero && hHandle != new IntPtr(-1))
            {
                NativeMethods.CloseHandle(hHandle);
                NativeMethods.SetStdHandle(NativeMethods.STD_ERROR_HANDLE, new IntPtr(-1));
            }
        }

        static void Main(string[] args)
        {
            if (args.Length == 0)
                throw new NotImplementedException();
        }
    }
}
