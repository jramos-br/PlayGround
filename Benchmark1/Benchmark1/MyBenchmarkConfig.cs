// The MyBenchmarkConfig class.
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

using BenchmarkDotNet.Analysers;
using BenchmarkDotNet.Columns;
using BenchmarkDotNet.Configs;
using BenchmarkDotNet.Diagnosers;
using BenchmarkDotNet.Exporters;
using BenchmarkDotNet.Filters;
using BenchmarkDotNet.Jobs;
using BenchmarkDotNet.Loggers;
using BenchmarkDotNet.Order;
using BenchmarkDotNet.Reports;
using BenchmarkDotNet.Validators;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Benchmark1
{
    class MyBenchmarkConfigX : ManualConfig
    {
        public MyBenchmarkConfigX()
        {
            Add(new ConsoleLogger(ConsoleLogger.CreateGrayScheme()));
            Add(DefaultConfig.Instance.GetExporters().ToArray()); // manual config has no exporters by default
            Add(DefaultConfig.Instance.GetColumnProviders().ToArray()); // manual config has no columns by default
        }

        public MyBenchmarkConfigX(IConfig config)
        {
            Add(config.GetColumnProviders().ToArray());
            Add(config.GetExporters().ToArray());
            Add(new ConsoleLogger(ConsoleLogger.CreateGrayScheme()));
            Add(config.GetDiagnosers().ToArray());
            Add(config.GetAnalysers().ToArray());
            Add(config.GetJobs().ToArray());
            Add(config.GetValidators().ToArray());
            Add(config.GetHardwareCounters().ToArray());
            Add(config.GetFilters().ToArray());
            Add(config.GetLogicalGroupRules().ToArray());

            ArtifactsPath = config.ArtifactsPath;
            Encoding = config.Encoding;
            Orderer = config.Orderer;
            Options |= config.Options;
            SummaryStyle = config.SummaryStyle;
            UnionRule = config.UnionRule;
        }

        public static MyBenchmarkConfigX Create()
        {
            return new MyBenchmarkConfigX();
        }

        public new static MyBenchmarkConfigX Create(IConfig config)
        {
            return new MyBenchmarkConfigX(config);
        }
    }

    class MyBenchmarkConfig : IConfig
    {
        private ConsoleLogger _consoleLogger = new ConsoleLogger(ConsoleLogger.CreateGrayScheme());

        public IEnumerable<IColumnProvider> GetColumnProviders() => DefaultConfig.Instance.GetColumnProviders();
        public IEnumerable<IExporter> GetExporters() => DefaultConfig.Instance.GetExporters();
        public IEnumerable<ILogger> GetLoggers() { yield return _consoleLogger; }
        public IEnumerable<IDiagnoser> GetDiagnosers() => DefaultConfig.Instance.GetDiagnosers();
        public IEnumerable<IAnalyser> GetAnalysers() => DefaultConfig.Instance.GetAnalysers();
        public IEnumerable<Job> GetJobs() => DefaultConfig.Instance.GetJobs();
        public IEnumerable<IValidator> GetValidators() => DefaultConfig.Instance.GetValidators();
        public IEnumerable<HardwareCounter> GetHardwareCounters() => DefaultConfig.Instance.GetHardwareCounters();
        public IEnumerable<IFilter> GetFilters() => DefaultConfig.Instance.GetFilters();
        public IEnumerable<BenchmarkLogicalGroupRule> GetLogicalGroupRules() => DefaultConfig.Instance.GetLogicalGroupRules();

        public string ArtifactsPath => DefaultConfig.Instance.ArtifactsPath;
        public Encoding Encoding => DefaultConfig.Instance.Encoding;
        public IOrderer Orderer => DefaultConfig.Instance.Orderer;
        public ConfigOptions Options => DefaultConfig.Instance.Options;
        public SummaryStyle SummaryStyle => DefaultConfig.Instance.SummaryStyle;
        public ConfigUnionRule UnionRule => DefaultConfig.Instance.UnionRule;

        public static MyBenchmarkConfig Create()
        {
            return new MyBenchmarkConfig();
        }
    }
}
