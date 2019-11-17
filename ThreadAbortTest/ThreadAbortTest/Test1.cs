// The Test1 class.
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
    class Test1 : TestBase
    {
        public volatile int _flag = 0;

        public override void Run(int test)
        {
            Util.WriteMessage(Util.GetMethodName(this) + ": begin test = " + test.ToString());

            try
            {
                Util.WriteMessage(Util.GetMethodName(this) + ": try");

                switch (test)
                {
                    case 1:
                        while (_flag == 0)
                            ;
                        break;
                    case 2:
                        Thread.Sleep(5000);
                        break;
                    case 3:
                        Console.Out.Write("Press any key: ");
                        Console.Out.Flush();
                        Console.ReadKey(true);
                        Console.Out.WriteLine("OK");
                        break;
                    case 4:
                        Console.Out.Write("Press Enter: ");
                        Console.Out.Flush();
                        var ch = Console.Read();
                        if (ch == -1)
                        {
                            Console.Out.WriteLine();
                            Thread.Sleep(100); // Allow CancelKeyPress to run
                        }
                        Console.Out.WriteLine("OK");
                        break;
                    case 5:
                        Console.Out.Write("Press Enter: ");
                        Console.Out.Flush();
                        var line = Console.In.ReadLine();
                        if (line == null)
                        {
                            Console.Out.WriteLine();
                            Thread.Sleep(100); // Allow CancelKeyPress to run
                        }
                        Console.Out.WriteLine("OK");
                        break;
                }

                Util.WriteMessage(Util.GetMethodName(this) + ": end try");
            }
            catch (Exception ex)
            {
                Util.WriteMessage(Util.GetMethodName(this) + ": catch " + ex.GetType().FullName + ": " + ex.Message);
            }
            finally
            {
                Util.WriteMessage(Util.GetMethodName(this) + ": finally");
            }

            Util.WriteMessage(Util.GetMethodName(this) + ": end");
        }

        public static void RunTest(int test)
        {
            var t = new Test1();
            t.Run(test);
        }
    }
}
