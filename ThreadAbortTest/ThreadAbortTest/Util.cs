// The Util class.
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
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace ThreadAbortTest
{
    static class Util
    {
        public static string GetMethodName<T>(T source, [CallerMemberName] string callerMemberName = "")
        {
            return GetMethodName<T>(callerMemberName);
        }

        public static string GetMethodName<T>([CallerMemberName] string callerMemberName = "")
        {
            return typeof(T).Name + "." + callerMemberName;
        }

        public static void WriteMessage(string message)
        {
            Console.Out.WriteLine(message);
            Console.Out.Flush();
        }

        public static void PromptUser()
        {
            Console.Out.Write("Press Enter to exit: ");
            Console.Out.Flush();

            if (Console.In.Read() == -1)
            {
                Console.Out.WriteLine();
                Thread.Sleep(100); // Allow CancelKeyPress to run
            }
        }

        public static bool IsThreadAbortException(Exception ex)
        {
            return ex is ThreadAbortException;
        }
    }
}
