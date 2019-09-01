// The Test5 class.
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
    // https://devblogs.microsoft.com/pfxteam/await-synchronizationcontext-and-console-apps/
    class Test5
    {
        // Each continuation (code executing after Task.Yield()) will use the main
        // thread, because AsyncPump creates a single-threaded SynchronizationContext.
        private static async Task RunAsync()
        {
            var d = new Dictionary<int, int>();

            for (int i = 0; i < 5000; i++)
            {
                int id = Thread.CurrentThread.ManagedThreadId;
                d[id] = d.TryGetValue(id, out int count) ? count + 1 : 1;
                await Task.Yield();
            }

            foreach (var pair in d)
            {
                Console.Out.WriteLine(pair);
            }
        }

        public static void Run(string[] args)
        {
            Microsoft.Threading.AsyncPump.Run(RunAsync);
        }
    }
}
