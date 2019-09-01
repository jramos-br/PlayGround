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
                    Main1.Run(() => Test2.RunTest(3));
                }
                catch (ThreadAbortException)
                {
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
