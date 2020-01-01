// The Test6 class.
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
    class Test7
    {
        private static void ForEachLine(string source, Action<string, int, int> action)
        {
            int pos = 0;
            int end = source.Length;
            int sep;

            while (pos < end && (sep = source.IndexOf('\n', pos)) >= 0)
            {
                int eol = (sep > pos && source[sep - 1] == '\r') ? sep - 1 : sep;
                action(source, pos, eol - pos);
                pos = sep + 1;
            }

            action(source, pos, end - pos);
        }

        private static async Task ForEachLineAsync(string source, Func<string, int, int, Task> action)
        {
            int pos = 0;
            int end = source.Length;
            int sep;

            while (pos < end && (sep = source.IndexOf('\n', pos)) >= 0)
            {
                int eol = (sep > pos && source[sep - 1] == '\r') ? sep - 1 : sep;
                await action(source, pos, eol - pos);
                pos = sep + 1;
            }

            await action(source, pos, end - pos);
        }

        private async Task Run3(string source, int offset, int count)
        {
            await Task.Delay(1000);
            await Console.Out.WriteLineAsync(source.Substring(offset, count));
        }

        private async Task Run2()
        {
            await Console.Out.WriteLineAsync($"starting...");
            var str1 = "a\nb\nc\nd\ne\nf";
            // ForEachLine does not propagate the await - no error, no warning
            ForEachLine(str1, async (s, o, c) => await Run3(s, o, c));
            await Console.Out.WriteLineAsync($"OK");
        }

        private async Task Run()
        {
            await Console.Out.WriteLineAsync($"starting...");
            var str1 = "a\nb\nc\nd\ne\nf";
            await ForEachLineAsync(str1, async (s, o, c) => await Run3(s, o, c));
            await Console.Out.WriteLineAsync($"OK");
        }

        private static async Task RunAsync()
        {
            var t = new Test7();
            await t.Run();
        }

        public static void Run(string[] args)
        {
            Microsoft.Threading.AsyncPump.Run(RunAsync);
        }
    }
}
