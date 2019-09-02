// The ConsoleApp class.
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
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Benchmark1
{
    public class ConsoleApp
    {
        private static volatile Thread _mainThread;

        private static void Initialize()
        {
            Console.CancelKeyPress += CancelKeyPressEventHandler;
            _mainThread = Thread.CurrentThread;
        }

        private static void Terminate()
        {
            _mainThread = null;
            Console.CancelKeyPress -= CancelKeyPressEventHandler;
        }

        private static void CancelKeyPressEventHandler(object sender, ConsoleCancelEventArgs e)
        {
            try
            {
                Thread mainThread = _mainThread;

                if (mainThread != null && mainThread.IsAlive)
                {
                    mainThread.Abort();
                }

                e.Cancel = true;
            }
            catch (Exception ex)
            {
                string source = nameof(Benchmark1) + ExceptionUtil.Separator + nameof(CancelKeyPressEventHandler);
                Console.Error.WriteExceptionMessage(ex, source);
            }
        }

        public static void Run(Action<string[]> action, string[] args)
        {
            try
            {
                try
                {
                    UnhandledExceptionHandler.Initialize();
                    Initialize();

                    try
                    {
                        action(args);
                    }
                    finally
                    {
                        Terminate();
                    }
                }
                catch (ThreadAbortException)
                {
                }
                catch (Exception ex)
                {
                    string source = nameof(Benchmark1);
                    Console.Error.WriteExceptionMessage(ex, source);
                }
            }
            catch (ThreadAbortException)
            {
                Thread.ResetAbort();
                string source = nameof(Benchmark1) + ExceptionUtil.Separator + nameof(ThreadAbortException);
                Console.Error.WriteLine(source);
            }
        }
    }
}
