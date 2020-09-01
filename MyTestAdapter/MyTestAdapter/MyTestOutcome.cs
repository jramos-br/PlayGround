// A custom adapter to discover and execute MSTest Framework based tests.
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

namespace MyTestAdapter
{
    /// <summary>
    /// The test outcomes.
    /// </summary>
    public enum MyTestOutcome
    {
        /// <summary>
        /// Test is in an unknown state.
        /// </summary>
        Unknown = 0,
        /// <summary>
        /// Test is executing.
        /// </summary>
        InProgress = 1,
        /// <summary>
        /// Test has completed, but we can't say if it passed or failed.
        /// </summary>
        Inconclusive = 2,
        /// <summary>
        /// Test was executed, but there were issues.
        /// </summary>
        Failed = 3,
        /// <summary>
        /// Test was executed without any issues.
        /// </summary>
        Passed = 4,
        /// <summary>
        /// There was an error while we were trying to execute a test.
        /// </summary>
        Error = 5
    }
}
