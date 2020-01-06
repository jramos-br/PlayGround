// Finds the lines of text in a given string, running an action for each one.
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
using MyStringUtils;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace MyStringUtilsTest
{
    [TestClass]
    public class ForEachLineTest
    {
        private class TestData
        {
            public string Source;
            public string[] Result;

            public TestData(string source, params string[] result)
            {
                Source = Format(source);
                Result = result;
            }

            public static string Format(params string[] strs)
            {
                var sb = new StringBuilder();

                foreach (var str in strs)
                {
                    sb.Append(str.Replace('+', '\r').Replace('|', '\n'));
                }

                return sb.ToString();
            }
        }

        private static readonly TestData[] Data1 = new TestData[]
        {
            new TestData("", ""),
            new TestData("a", "a"),
            new TestData("a|b", "a", "b"),
            new TestData("a+|b", "a", "b"),
            new TestData("a|b|c", "a", "b", "c"),
            new TestData("a|b+|c", "a", "b", "c"),
            new TestData("a+|b|c", "a", "b", "c"),
            new TestData("a+|b+|c", "a", "b", "c"),
            new TestData("a+|+|b", "a", "", "b"),
            new TestData("a++|b", "a\r", "b"),
            new TestData("|a+|b|", "", "a", "b", ""),
        };

        private static IEnumerable<object[]> GetData1()
        {
            for (int i = 0; i < Data1.Length; ++i)
            {
                yield return new object[] { i };
            }
        }

        private static string ReplaceNewLine(string str)
        {
            return str.Replace("\r", "\\r").Replace("\n", "\\n");
        }

        [DataTestMethod]
        [DynamicData(nameof(GetData1), DynamicDataSourceType.Method)]
        public void TestMethod1(int index)
        {
            Assert.IsTrue(index >= 0 && index < Data1.Length);
            Trace.WriteLine(string.Format("idx={0} src=[{1}]", index, ReplaceNewLine(Data1[index].Source)));
            int result = 0;
            ForEachLine.Run(Data1[index].Source, (s, o, c) =>
            {
                Trace.WriteLine(string.Format(" res={0} seg=[{1}]", result, ReplaceNewLine(s.Substring(o, c))));
                Assert.AreEqual(s.Substring(o, c), Data1[index].Result[result++]);
            });
        }

        [DataTestMethod]
        [DynamicData(nameof(GetData1), DynamicDataSourceType.Method)]
        public async Task TestMethod2(int index)
        {
            Assert.IsTrue(index >= 0 && index < Data1.Length);
            Trace.WriteLine(string.Format("async idx={0} src=[{1}]", index, ReplaceNewLine(Data1[index].Source)));
            int result = 0;
            await ForEachLine.RunAsync(Data1[index].Source, async (s, o, c) =>
            {
                Trace.WriteLine(string.Format(" res={0} seg=[{1}]", result, ReplaceNewLine(s.Substring(o, c))));
                Assert.AreEqual(s.Substring(o, c), Data1[index].Result[result++]);
                await Task.Yield();
            });
        }
    }
}
