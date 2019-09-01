// The Test3 class.
//
// This file is part of the AsyncTest1 program.
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

namespace AsyncTest1
{
    class Test3
    {
        private long Count1 = 0;
        private long Count2 = 0;

        private ConsoleKeyInfo ReadKey()
        {
            return Console.KeyAvailable ? Console.ReadKey(true) : new ConsoleKeyInfo();
        }

        private async Task WriteCount()
        {
            for (int i = 0; i < 20; ++i)
            {
                await Task.Delay(1000);

                Console.SetCursorPosition(0, 4);
                Console.Out.Write(string.Format(">{0}< {1}", Thread.CurrentThread.ManagedThreadId, ++Count2));
                Console.SetCursorPosition(0, 5);
            }
        }

        private async Task Run()
        {
            if (Console.IsOutputRedirected || Console.IsErrorRedirected)
                return;

            Console.CursorVisible = false;

            try
            {
                Console.Clear();
                Console.Out.Write("Press any key to stop");
                Console.SetCursorPosition(0, 5);

                var start = DateTime.MinValue;
                var awaiter = WriteCount().GetAwaiter();

                while (ReadKey().Key == 0 && !awaiter.IsCompleted)
                {
                    Console.SetCursorPosition(0, 3);
                    Console.Out.Write(string.Format("[{0}] {1}", Thread.CurrentThread.ManagedThreadId, ++Count1));
                    Console.SetCursorPosition(0, 5);

                    var now = DateTime.Now;

                    if (start.Ticks == 0 || now.Second != start.Second)
                    {
                        start = now;
                        Console.SetCursorPosition(0, 2);
                        Console.Out.Write(string.Format("{0}", now));
                        Console.SetCursorPosition(0, 5);
                    }
                }

                await Console.Out.WriteLineAsync("OK");
            }
            catch (Exception ex)
            {
                Console.Out.WriteLine(ex.Message);
            }
            finally
            {
                Console.CursorVisible = true;
            }
        }

        private static async Task RunAsync(string[] args)
        {
            var t = new Test3();
            await t.Run();
        }

        public static void Run(string[] args)
        {
            RunAsync(args).GetAwaiter().GetResult();
        }
    }
}
