// Finds the lines of text in a given string, running an action for each one.
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
using System.Threading.Tasks;

namespace MyStringUtils
{
    /// <summary>
    /// Finds the lines of text in a given string, running an action for each one.
    /// </summary>
    /// <remarks>
    /// The lines must be delimited by a single "\n" or a pair "\r\n".
    /// </remarks>
    public static class ForEachLine
    {
        /// <summary>
        /// Finds the lines of text in a given string, running an action for each one.
        /// </summary>
        /// <param name="source">The source string.</param>
        /// <param name="action">The action to execute for each line in the <paramref name="source"/>.</param>
        public static void Run(string source, Action<string, int, int> action)
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

        /// <summary>
        /// Finds the lines of text in a given string, running an action for each one.
        /// </summary>
        /// <param name="source">The source string.</param>
        /// <param name="action">The action to execute for each line in the <paramref name="source"/>.</param>
        public static async Task RunAsync(string source, Func<string, int, int, Task> action)
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
    }
}
