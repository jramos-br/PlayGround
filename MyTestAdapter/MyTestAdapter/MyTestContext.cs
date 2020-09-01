﻿// A custom adapter to discover and execute MSTest Framework based tests.
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

using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Data;
using System.Data.Common;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MyTestAdapter
{
    /// <summary>
    /// The test context contains information about the test currently being executed.
    /// </summary>
    public class MyTestContext : TestContext
    {
        /// <summary>
        /// Gets test properties for a test.
        /// </summary>
        public override IDictionary Properties { get; } = new Dictionary<string, object>();

        public static readonly string DataConnectionKey = "DataConnection";
        public static readonly string DataRowKey = "DataRow";
        public static readonly string FullyQualifiedTestClassNameKey = "FullyQualifiedTestClassName";
        public static readonly string TestNameKey = "TestName";
        public static readonly string TestDisplayNameKey = "TestDisplayName";
        public static readonly string LastExceptionKey = "LastException";

        /// <summary>
        /// Gets the current data connection when test is used for data driven testing.
        /// </summary>
        public override DbConnection DataConnection => Properties[DataConnectionKey] as DbConnection;

        /// <summary>
        /// Sets the current data connection when test is used for data driven testing.
        /// </summary>
        public void SetDataConnection(DbConnection dataConnection) => Properties[DataConnectionKey] = dataConnection;

        /// <summary>
        /// Gets the current data row when test is used for data driven testing.
        /// </summary>
        public override DataRow DataRow => Properties[DataRowKey] as DataRow;

        /// <summary>
        /// Sets the current data row when test is used for data driven testing.
        /// </summary>
        public void SetDataRow(DataRow dataRow) => Properties[DataRowKey] = dataRow;

        /// <summary>
        /// Gets the fully-qualified name of the class containing the test method
        /// currently being executed
        /// </summary>
        public override string FullyQualifiedTestClassName => Properties[FullyQualifiedTestClassNameKey] as string;

        /// <summary>
        /// Sets the fully-qualified name of the class containing the test method
        /// currently being executed
        /// </summary>
        internal void SetFullyQualifiedTestClassName(string fullyQualifiedTestClassName) => Properties[FullyQualifiedTestClassNameKey] = fullyQualifiedTestClassName;

        /// <summary>
        /// Gets the name of the test method currently being executed.
        /// </summary>
        public override string TestName => Properties[TestNameKey] as string;

        /// <summary>
        /// Sets the name of the test method currently being executed.
        /// </summary>
        internal void SetTestName(string testName) => Properties[TestNameKey] = testName;

        /// <summary>
        /// Gets the name of the test currently being executed.
        /// </summary>
        public string TestDisplayName => Properties[TestDisplayNameKey] as string;

        /// <summary>
        /// Sets the name of the test currently being executed.
        /// </summary>
        internal void SetTestDisplayName(string testDisplayName) => Properties[TestDisplayNameKey] = testDisplayName;

        /// <summary>
        /// The current test outcome.
        /// </summary>
        private UnitTestOutcome _currentTestOutcome = UnitTestOutcome.Unknown;

        /// <summary>
        /// Gets the current test outcome.
        /// </summary>
        public override UnitTestOutcome CurrentTestOutcome => _currentTestOutcome;

        /// <summary>
        /// Sets the current test outcome.
        /// </summary>
        internal void SetCurrentTestOutcome(UnitTestOutcome currentTestOutcome) => _currentTestOutcome = currentTestOutcome;

        /// <summary>
        /// Gets the last exception generated by the test currently being executed.
        /// </summary>
        public Exception LastException => Properties[LastExceptionKey] as Exception;

        /// <summary>
        /// Sets the last exception generated by the test currently being executed.
        /// </summary>
        internal void SetLastException(Exception exception) => Properties[LastExceptionKey] = exception;

        /// <summary>
        /// Adds a file name to the list in TestResult.ResultFileNames.
        /// </summary>
        /// <param name="fileName">The file name</param>
        public override void AddResultFile(string fileName)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Begins a timer with the specified name.
        /// </summary>
        /// <param name="timerName">The name of the timer.</param>
        public override void BeginTimer(string timerName)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Ends a timer with the specified name.
        /// </summary>
        /// <param name="timerName">The name of the timer.</param>
        public override void EndTimer(string timerName)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Writes a trace message while the test is running.
        /// </summary>
        /// <param name="message">A formatted message string.</param>
        public override void WriteLine(string message)
        {
            Console.Out.WriteLine(message);
        }

        /// <summary>
        /// Formats and writes a trace message while the test is running.
        /// </summary>
        /// <param name="format">The format string.</param>
        /// <param name="args">The arguments.</param>
        public override void WriteLine(string format, params object[] args)
        {
            Console.Out.WriteLine(format, args);
        }
    }
}
