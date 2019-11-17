// The ThreadAbortTest program.
//
// This code sample demonstrates the handling of CONTROL/C using a ThreadAbort
// exception.
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
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                try
                {
                    int main = args.Length > 0 ? Convert.ToInt32(args[0]) : 1;
                    int test = args.Length > 1 ? Convert.ToInt32(args[1]) : 1;
                    int func = args.Length > 2 ? Convert.ToInt32(args[2]) : 1;

                    MainBase mainFunc = null;
                    TestBase testFunc = null;

                    switch (main)
                    {
                        case 1: mainFunc = new Main1(); break;
                        case 2: mainFunc = new Main2(); break;
                    }

                    switch (test)
                    {
                        case 1: testFunc = new Test1(); break;
                        case 2: testFunc = new Test2(); break;
                    }

                    mainFunc.Run(() => testFunc.Run(func));
                }
                catch (ThreadAbortException)
                {
                    // ThreadAbortException is a special exception that can be
                    // caught, but it will automatically be raised again at the
                    // end of the catch block, unless Thread.ResetAbort is called.
                }
                catch (Exception ex)
                {
                    Console.Error.WriteLine(ex.Message);
                }
            }
            catch (ThreadAbortException)
            {
                Thread.ResetAbort();
                Console.Error.WriteLine(nameof(ThreadAbortException));
            }
        }
    }
}
