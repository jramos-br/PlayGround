// The Main1 class.
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
    class Main1 : MainBase
    {
        public Main1()
        {
            _run = Run1;
        }

        private static volatile Thread _mainThread;

        private static readonly bool _exceptionInInitialize = false;
        private static readonly bool _exceptionInTerminate = false;

        private static void Initialize()
        {
            Util.WriteMessage(Util.GetMethodName<Main1>());

            _mainThread = Thread.CurrentThread;

            if (_exceptionInInitialize)
            {
                Util.WriteMessage(Util.GetMethodName<Main1>() + ": throw Exception in " + nameof(Initialize));
                throw new Exception("Exception in " + nameof(Initialize));
            }
        }

        private static void Terminate()
        {
            Util.WriteMessage(Util.GetMethodName<Main1>());

            _mainThread = null;

            if (_exceptionInTerminate)
            {
                Util.WriteMessage(Util.GetMethodName<Main1>() + ": throw Exception in " + nameof(Terminate));
                throw new Exception("Exception in " + nameof(Terminate));
            }
        }

        static Main1()
        {
            Console.CancelKeyPress += CancelKeyPressEventHandler;
        }

        private static void CancelKeyPressEventHandler(object sender, ConsoleCancelEventArgs e)
        {
            try
            {
                Util.WriteMessage(Util.GetMethodName<Main1>());

                var mainThread = _mainThread;

                if (mainThread != null && mainThread.IsAlive)
                {
                    mainThread.Abort();
                }

                e.Cancel = true;
            }
            catch (Exception ex)
            {
                Util.WriteMessage(Util.GetMethodName<Main1>() + ": catch " + ex.GetType().FullName + ": " + ex.Message);
            }
        }

        private static void Run2(Action action)
        {
            Util.WriteMessage(Util.GetMethodName<Main1>() + ": begin");

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
                    Util.WriteMessage(Util.GetMethodName<Main1>() + ": catch " + ex.GetType().FullName + ": " + ex.Message);
                }
            }
            catch (ThreadAbortException)
            {
                Thread.ResetAbort();
                Util.WriteMessage(Util.GetMethodName<Main1>() + ": " + nameof(ThreadAbortException));
            }
            finally
            {
                Util.PromptUser();
            }

            Util.WriteMessage(Util.GetMethodName<Main1>() + ": end");
        }

        private static void Run1(Action action)
        {
            Util.WriteMessage(Util.GetMethodName<Main1>() + ": begin");

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
                    Util.WriteMessage(Util.GetMethodName<Main1>() + ": catch " + ex.GetType().FullName + ": " + ex.Message);
                }
            }
            catch (ThreadAbortException)
            {
                Thread.ResetAbort();
                Util.WriteMessage(Util.GetMethodName<Main1>() + ": " + nameof(ThreadAbortException));
            }

            Util.WriteMessage(Util.GetMethodName<Main1>() + ": end");
        }
    }
}
