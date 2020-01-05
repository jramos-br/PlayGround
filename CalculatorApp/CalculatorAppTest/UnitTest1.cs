// A test project for the CalculatorApp program that uses MSTest as the test framework.
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

using CalculatorApp;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CalculatorAppTest
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
        /// Initial value must be zero.
        /// </summary>
        [TestMethod]
        public void InitialValueMustBeZero()
        {
            Trace.WriteLine(string.Format("{0}.{1}", nameof(UnitTest1), TestContext.TestName));
            Calculator c = new Calculator();
            double value = c.Value;
            Assert.AreEqual(0, value);
        }

        /// <summary>
        /// A simple add.
        /// </summary>
        [DataTestMethod]
        [DataRow(1, 2, 3)]
        [DataRow(3, 4, 7)]
        [DataRow(5, 6, 11)]
        public void SimpleAdd(double a, double b, double expected)
        {
            Trace.WriteLine(string.Format("{0}.{1}", nameof(UnitTest1), TestContext.TestName));
            Calculator c = new Calculator();
            double value = c.Add(a).Add(b).Value;
            Assert.AreEqual(expected, value);
        }

        /// <summary>
        /// A simple subtraction.
        /// </summary>
        [DataTestMethod]
        [DataRow(1, 2, -3)]
        [DataRow(3, 4, -7)]
        [DataRow(5, 6, -11)]
        public void SimpleSubtract(double a, double b, double expected)
        {
            Trace.WriteLine(string.Format("{0}.{1}", nameof(UnitTest1), TestContext.TestName));
            Calculator c = new Calculator();
            double value = c.Subtract(a).Subtract(b).Value;
            Assert.AreEqual(expected, value);
        }

        /// <summary>
        /// A simple multiplication.
        /// </summary>
        [DataTestMethod]
        [DataRow(1, 2, 2)]
        [DataRow(3, 4, 12)]
        [DataRow(5, 6, 30)]
        public void SimpleMultipy(double a, double b, double expected)
        {
            Trace.WriteLine(string.Format("{0}.{1}", nameof(UnitTest1), TestContext.TestName));
            Calculator c = new Calculator();
            double value = c.Add(a).Multiply(b).Value;
            Assert.AreEqual(expected, value);
        }

        /// <summary>
        /// A simple division.
        /// </summary>
        [TestMethod]
        public void SimpleDivide()
        {
            Trace.WriteLine(string.Format("{0}.{1}", nameof(UnitTest1), TestContext.TestName));
            Calculator c = new Calculator();
            double value = c.Add(5).Divide(2).Value;
            Assert.AreEqual(2.5, value);
        }

        /// <summary>
        /// A four operations simple test.
        /// </summary>
        [TestMethod]
        public void FourOperations()
        {
            Trace.WriteLine(string.Format("{0}.{1}", nameof(UnitTest1), TestContext.TestName));
            Calculator c = new Calculator();
            double value = c.Add(16).Subtract(8).Multiply(4).Divide(2).Value;
            Assert.AreEqual(16, value);
        }

        /// <summary>
        /// Divide zero by zero.
        /// </summary>
        [TestMethod]
        public void DivideZeroByZero()
        {
            Trace.WriteLine(string.Format("{0}.{1}", nameof(UnitTest1), TestContext.TestName));
            Calculator c = new Calculator();
            double value = c.Divide(0).Value;
            Assert.AreEqual(double.NaN, value);
        }

        /// <summary>
        /// Divide a negative number by zero.
        /// </summary>
        [TestMethod]
        public void DivideNegativeByZero()
        {
            Trace.WriteLine(string.Format("{0}.{1}", nameof(UnitTest1), TestContext.TestName));
            Calculator c = new Calculator();
            double value = c.Subtract(42).Divide(0).Value;
            Assert.AreEqual(double.NegativeInfinity, value);
        }

        /// <summary>
        /// PI times 2 divide by 2.
        /// </summary>
        [TestMethod]
        public void PiTimesTwoDivideByTwo()
        {
            Trace.WriteLine(string.Format("{0}.{1}", nameof(UnitTest1), TestContext.TestName));
            Calculator c = new Calculator();
            double value = c.Add(Math.PI).Multiply(2).Divide(2).Value;
            Assert.AreEqual(Math.PI, value);
        }

        /// <summary>
        /// 0.5 times 4.
        /// </summary>
        [TestMethod]
        public void ZeroPointFiveTimesFour()
        {
            Trace.WriteLine(string.Format("{0}.{1}", nameof(UnitTest1), TestContext.TestName));
            Calculator c = new Calculator();
            double value = c.Add(0.5).Add(0.5).Add(0.5).Add(0.5).Value;
            Assert.AreEqual(2, value);
        }
    }
}
