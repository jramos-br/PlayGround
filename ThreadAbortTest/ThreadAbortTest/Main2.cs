// The Main2 class.
//
// This file is part of the ThreadAbortTest program.
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

namespace ThreadAbortTest
{
    class Main2
    {
        private static Thread _mainThread;

        private static void Initialize()
        {
            _mainThread = Thread.CurrentThread;
        }

        private static void Terminate()
        {
            _mainThread = null;
        }

        static Main2()
        {
            Console.CancelKeyPress += CancelKeyPressEventHandler;
        }

        private static void CancelKeyPressEventHandler(object sender, ConsoleCancelEventArgs e)
        {
            try
            {
                var mainThread = _mainThread;

                if (mainThread != null && mainThread.IsAlive)
                {
                    mainThread.Abort();
                }

                e.Cancel = true;
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine(nameof(CancelKeyPressEventHandler) + ": " + ex.Message);
            }
        }

        private static void Run2(Action action)
        {
            try
            {
                try
                {
                    Initialize();

                    try
                    {
                        action();
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
                    Console.Error.WriteLine(nameof(Run2) + ": " + ex.Message);
                }
            }
            catch (ThreadAbortException)
            {
                Thread.ResetAbort();
                Console.Error.WriteLine(nameof(Run2) + ": " + nameof(ThreadAbortException));
            }
            finally
            {
                Util.PromptUser();
            }
        }

        public static void Run(Action action)
        {
            try
            {
                try
                {
                    Run2(action);
                }
                catch (ThreadAbortException)
                {
                }
                catch (Exception ex)
                {
                    Console.Error.WriteLine(nameof(Run) + ": " + ex.Message);
                }
            }
            catch (ThreadAbortException)
            {
                Thread.ResetAbort();
                Console.Error.WriteLine(nameof(Run) + ": " + nameof(ThreadAbortException));
            }
        }
    }
}
