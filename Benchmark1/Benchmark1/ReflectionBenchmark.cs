// The ReflectionBenchmark class.
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

using BenchmarkDotNet.Attributes;
using BenchmarkDotNet.Running;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace Benchmark1
{
    public class ReflectionBenchmark
    {
        private ReflectionBenchmark _instance;

        [GlobalSetup]
        public void Setup()
        {
            _instance = new ReflectionBenchmark();
        }

        [Benchmark]
        public string Test1() => typeof(ReflectionBenchmark).Assembly.GetName().Name;

        [Benchmark]
        public string Test2() => _instance.GetType().Assembly.GetName().Name;

        [Benchmark]
        public string Test3() => Assembly.GetExecutingAssembly().GetName().Name;

        public static void Run()
        {
            var summary = BenchmarkRunner.Run<ReflectionBenchmark>();
        }
    }
}
