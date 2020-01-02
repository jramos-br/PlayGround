// The CalculatorApp program.
//
// A simple command-line calculator.
//
// This code sample demonstrates unit testing using MSTest.
//
// https://docs.microsoft.com/en-us/visualstudio/test/unit-test-your-code
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
using System.Text;
using System.Collections.Generic;
using System.Diagnostics;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace CalculatorAppTest
{
    /// <summary>
    /// The test methods at the assembly level.
    /// </summary>
    [TestClass]
    public class UnitTestInternal
    {
        /// <summary>
        /// Contains code to be used before all tests in the assembly have run and to
        /// allocate resources obtained by the assembly.
        /// </summary>
        /// <remarks>
        /// This method will be run before methods marked with the ClassInitialize,
        /// TestInitialize and TestMethod attributes.
        /// </remarks>
        [AssemblyInitialize]
        public static void AssemblyInitialize(TestContext context)
        {
            Trace.WriteLine(string.Format("{0}.{1}",
                nameof(UnitTestInternal), nameof(AssemblyInitialize)));
        }

        /// <summary>
        /// Contains code to be used after all tests in the assembly have run and to free
        /// resources obtained by the assembly.
        /// </summary>
        /// <remarks>
        /// This method will be run after methods marked with the TestCleanup and
        /// ClassCleanup attributes unless an unhandled exception is thrown.
        /// </remarks>
        [AssemblyCleanup]
        public static void AssemblyCleanup()
        {
            Trace.WriteLine(string.Format("{0}.{1}",
                nameof(UnitTestInternal), nameof(AssemblyCleanup)));
        }
    }
}
