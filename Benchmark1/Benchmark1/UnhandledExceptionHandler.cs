// The UnhandledExceptionHandler class.
//
// This file is part of the Benchmark1 program.
//
// Copyright (c) 2019 Jorge Ramos (mailto jramos at pobox dot com)
//
// This is free software. Redistribution and use in source and binary forms,
// with or without modification, for any purpose and with or without fee are
// hereby permitted. Altered source versions must be plainly marked as such.
//
// If you find this software useful, an acknowledgment would be appreciated
// but is not required.
//
// THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT ANY WARRANTY OR CONDITION.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE TO ANYONE
// FOR ANY DAMAGES RELATED TO THIS SOFTWARE, UNDER ANY KIND OF LEGAL CLAIM.

using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Benchmark1
{
    public static class UnhandledExceptionHandler
    {
        private static class NativeMethods
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

        public static void Initialize(UnhandledExceptionEventHandler handler = null)
        {
            AppDomain.CurrentDomain.UnhandledException += handler ?? UnhandledExceptionEventHandler;
            AppDomain.CurrentDomain.UnhandledException += CloseStderr;
        }

        private static void UnhandledExceptionEventHandler(object sender, UnhandledExceptionEventArgs e)
        {
            try
            {
                string source = nameof(Benchmark1) + ExceptionUtil.Separator + "UnhandledException";

                NativeMethods.SetErrorMode(NativeMethods.GetErrorMode() | NativeMethods.SEM_NOGPFAULTERRORBOX);

                if (e.ExceptionObject is Exception ex)
                {
                    Console.Error.WriteExceptionMessage(ex, source);
                }
                else
                {
                    if (e.ExceptionObject != null)
                    {
                        Console.Error.WriteLine(source + ExceptionUtil.Separator + e.ExceptionObject.ToString());
                    }
                    else
                    {
                        Console.Error.WriteLine(source);
                    }
                }
            }
            catch (Exception ex)
            {
                string source = nameof(Benchmark1) + ExceptionUtil.Separator + nameof(UnhandledExceptionEventHandler);
                Console.Error.WriteExceptionMessage(ex, source);
            }
        }

        private static void CloseStderr(object sender, UnhandledExceptionEventArgs e)
        {
            IntPtr hHandle = NativeMethods.GetStdHandle(NativeMethods.STD_ERROR_HANDLE);

            if (hHandle != IntPtr.Zero && hHandle != new IntPtr(-1))
            {
                NativeMethods.CloseHandle(hHandle);
                NativeMethods.SetStdHandle(NativeMethods.STD_ERROR_HANDLE, new IntPtr(-1));
            }
        }
    }
}
