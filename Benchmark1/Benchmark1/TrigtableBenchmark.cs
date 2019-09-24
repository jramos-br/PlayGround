// The TrigtableBenchmark class.
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
using System.Text;
using System.Threading.Tasks;

namespace Benchmark1
{
    public class TrigtableBenchmark
    {
        private double[] _tsin;
        private double[] _rads;
        private int[] _degs;

        private int _index;

        private int Next => _index++ % 360;

        [GlobalSetup]
        public void Setup()
        {
            var random = new Random(0);

            _tsin = new double[360];
            _rads = new double[360];
            _degs = new int[360];

            for (int i = 0; i < 360; ++i)
            {
                _degs[i] = i;
                _rads[i] = i * (Math.PI / 180.0);
                _tsin[i] = Math.Sin(_rads[i]);
            }

            _index = 0;
        }

        [Benchmark]
        public double Test1() => Math.Sin(_rads[Next]);

        [Benchmark]
        public double Test2() => _tsin[_degs[Next]];

        public static void Run()
        {
            var summary = BenchmarkRunner.Run<TrigtableBenchmark>();
        }
    }
}
