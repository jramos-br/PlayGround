// An example test project that uses MSTest as the test framework.
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
using System.Diagnostics;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace MyTestAdapterTest
{
    /// <summary>
    /// A test class.
    /// </summary>
    [TestClass]
    public class UnitTest1
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="UnitTest1"/> class.
        /// </summary>
        public UnitTest1()
        {
            Trace.WriteLine(string.Format("{0}.{1}", nameof(UnitTest1), nameof(UnitTest1)));
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
            Trace.WriteLine(string.Format("{0}.{1}", nameof(UnitTest1), nameof(ClassInitialize)));
        }

        /// <summary>
        /// Contains code to be used after all the tests in the test class have run and to
        /// free resources obtained by the test class.
        /// </summary>
        [ClassCleanup]
        public static void ClassCleanup()
        {
            Trace.WriteLine(string.Format("{0}.{1}", nameof(UnitTest1), nameof(ClassCleanup)));
        }

        /// <summary>
        /// Runs before the test to allocate and configure resources needed by all tests
        /// in the test class.
        /// </summary>
        [TestInitialize]
        public void TestInitialize()
        {
            Trace.WriteLine(string.Format("{0}.{1}.{2}", nameof(UnitTest1), nameof(TestInitialize), TestContext.TestName));
        }

        /// <summary>
        /// Contains code that must be used after the test has run and to free resources
        /// obtained by all the tests in the test class.
        /// </summary>
        [TestCleanup]
        public void TestCleanup()
        {
            Trace.WriteLine(string.Format("{0}.{1}.{2}", nameof(UnitTest1), nameof(TestCleanup), TestContext.TestName));
        }

        /// <summary>
        /// A simple test method.
        /// </summary>
        [TestMethod]
        public void TestMethod1a()
        {
            Trace.WriteLine(string.Format("{0}.{1}", nameof(UnitTest1), TestContext.TestName));
        }

        /// <summary>
        /// A parametrized test method with multiple data rows.
        /// </summary>
        [DataTestMethod]
        [DataRow(1, 1, 2)]
        [DataRow(2, 2, 4)]
        [DataRow(3, 3, 7)]
        public void TestMethod1b(int a, int b, int expected)
        {
            Trace.WriteLine(string.Format("{0}.{1} a={2} b={3} expected={4}", nameof(UnitTest1), TestContext.TestName, a, b, expected));
            Assert.AreEqual(expected, a + b);
        }

        /// <summary>
        /// A parametrized test method with dynamic data.
        /// </summary>
        [DataTestMethod]
        [DynamicData(nameof(Data), DynamicDataSourceType.Property)]
        public void TestMethod1c(int a, int b, int expected)
        {
            Trace.WriteLine(string.Format("{0}.{1} a={2} b={3} expected={4}", nameof(UnitTest1), TestContext.TestName, a, b, expected));
            Assert.AreEqual(expected, a + b);
        }

        /// <summary>
        /// A parametrized test method with dynamic data.
        /// </summary>
        [DataTestMethod]
        [DynamicData(nameof(GetData), DynamicDataSourceType.Method)]
        public void TestMethod1d(int a, int b, int expected)
        {
            Trace.WriteLine(string.Format("{0}.{1} a={2} b={3} expected={4}", nameof(UnitTest1), TestContext.TestName, a, b, expected));
            Assert.AreEqual(expected, a + b);
        }

        /// <summary>
        /// Data rows to test dynamic data.
        /// </summary>
        private static IEnumerable<object[]> Data
        {
            get
            {
                yield return new object[] { 1, 1, 2 };
                yield return new object[] { 12, 30, 42 };
                yield return new object[] { 14, 1, 15 };
            }
        }

        /// <summary>
        /// Data rows to test dynamic data.
        /// </summary>
        private static IEnumerable<object[]> GetData()
        {
            yield return new object[] { -1, -1, -2 };
            yield return new object[] { -12, -30, -42 };
            yield return new object[] { -14, -1, -15 };
        }
    }
}
