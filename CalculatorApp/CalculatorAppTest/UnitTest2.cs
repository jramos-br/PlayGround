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
    /// Test class.
    /// </summary>
    [TestClass]
    public class UnitTest2
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="UnitTest2"/> class.
        /// </summary>
        public UnitTest2()
        {
            Trace.WriteLine(string.Format("{0}.{1}",
                nameof(UnitTest2), nameof(UnitTest2)));
        }

        /// <summary>
        /// Gets or sets the test context which provides information about and
        /// functionality for the current test run.
        /// </summary>
        public TestContext TestContext { get; set; }

        /// <summary>
        /// Contains code that must be used before any of the tests in the test class have
        /// run and to allocate resources to be used by the test class.
        /// </summary>
        [ClassInitialize]
        public static void ClassInitialize(TestContext context)
        {
            Trace.WriteLine(string.Format("{0}.{1}",
                nameof(UnitTest2), nameof(ClassInitialize)));
        }

        /// <summary>
        /// Contains code to be used after all the tests in the test class have run and to
        /// free resources obtained by the test class.
        /// </summary>
        [ClassCleanup]
        public static void ClassCleanup()
        {
            Trace.WriteLine(string.Format("{0}.{1}",
                nameof(UnitTest2), nameof(ClassCleanup)));
        }

        /// <summary>
        /// Runs before the test to allocate and configure resources needed by all tests
        /// in the test class.
        /// </summary>
        [TestInitialize]
        public void TestInitialize()
        {
            Trace.WriteLine(string.Format("{0}.{1}.{2}",
                nameof(UnitTest2), nameof(TestInitialize), TestContext.TestName));
        }

        /// <summary>
        /// Contains code that must be used after the test has run and to free resources
        /// obtained by all the tests in the test class.
        /// </summary>
        [TestCleanup]
        public void TestCleanup()
        {
            Trace.WriteLine(string.Format("{0}.{1}.{2}",
                nameof(UnitTest2), nameof(TestCleanup), TestContext.TestName));
        }

        /// <summary>
        /// Test method.
        /// </summary>
        [TestMethod]
        public void TestMethod3()
        {
            Trace.WriteLine(string.Format("{0}.{1}",
                nameof(UnitTest2), nameof(TestMethod3)));
        }

        /// <summary>
        /// Test method.
        /// </summary>
        [TestMethod]
        public void TestMethod4()
        {
            Trace.WriteLine(string.Format("{0}.{1}",
                nameof(UnitTest2), nameof(TestMethod4)));
        }
    }
}
