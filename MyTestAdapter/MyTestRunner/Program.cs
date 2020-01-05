// A custom runner to discover and execute MSTest Framework based tests.
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
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace MyTestRunner
{
    /// <summary>
    /// Standalone runner.
    /// </summary>
    class Program
    {
        /// <summary>
        /// Executes the tests found in the assemblies specified in the command line.
        /// </summary>
        /// <param name="args">The names of the assembly files.</param>
        /// <remarks>
        /// The test results are printed in the standard output.
        /// </remarks>
        static void Main(string[] args)
        {
            try
            {
                foreach (var arg in args)
                {
                    var results = MyTestAdapter.MyTestRunner.RunTests(arg);

                    foreach (var result in results)
                    {
                        Console.Out.WriteLine("{0}: {1}: {2} in {3:F3} ms",
                            result.TestName, result.DisplayName, result.TestOutcome, result.ElapsedSeconds * 1000);

                        if (result.TestOutcome != MyTestAdapter.MyTestOutcome.Passed)
                        {
                            Console.Out.WriteLine("  {0}", result.LastException.Message);
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine(ex.ToString());
                Environment.ExitCode = 3;
            }
        }
    }
}
