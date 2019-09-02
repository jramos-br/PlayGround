// The ExceptionUtil class.
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
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Benchmark1
{
    public static class ExceptionUtil
    {
        public const string Separator = ": ";

        private static void WriteLine(string message, Exception ex, TextWriter writer)
        {
            if (!string.IsNullOrEmpty(message))
            {
                writer.WriteLine(message + Separator + ex.Message);
            }
            else
            {
                writer.WriteLine(ex.Message);
            }
        }

        public static void WriteMessage(string message, Exception ex, TextWriter writer)
        {
            if (ex is AggregateException ae)
            {
                foreach (Exception aex in ae.Flatten().InnerExceptions)
                {
                    WriteLine(message, aex, writer);
                }
            }
            else
            {
                WriteLine(message, ex, writer);
            }
        }

        public static void WriteMessage(Exception ex, TextWriter writer)
        {
            WriteMessage(null, ex, writer);
        }
    }
}
