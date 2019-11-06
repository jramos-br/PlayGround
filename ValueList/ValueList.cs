using System;
using System.Collections.Generic;
using System.Text;

namespace ValueList
{
    public class ValueList
    {
        private const char Separator = '|';
        private const char SeparatorEscape = '\\';
        private const char SeparatorFlag = 's';

        private static readonly char[] ControlChars = new char[] { Separator, SeparatorEscape };

        public static string Encode(string value)
        {
            if (string.IsNullOrEmpty(value))
                return value;

            var index = value.IndexOfAny(ControlChars);

            if (index < 0)
                return value;

            var start = 0;
            var length = value.Length;
            var capacity = (length + 16 + 7) & ~15;
            var result = new StringBuilder(capacity);

            do
            {
                result.Append(value, start, index - start);

                switch (value[index])
                {
                    case Separator:
                        result.Append(SeparatorEscape).Append(SeparatorFlag);
                        break;
                    case SeparatorEscape:
                        result.Append(SeparatorEscape).Append(SeparatorEscape);
                        break;
                    default:
                        throw new InvalidOperationException();
                }

                start = index + 1;
                index = value.IndexOfAny(ControlChars, start);

            } while (index >= 0);

            result.Append(value, start, length - start);
            return result.ToString();
        }

        public static string Decode(string value)
        {
            if (string.IsNullOrEmpty(value))
                return value;

            var index = value.IndexOfAny(ControlChars);

            if (index < 0)
                return value;

            var start = 0;
            var length = value.Length;
            var result = new StringBuilder(length);

            do
            {
                if (index == length - 1 || value[index] != SeparatorEscape)
                    throw new FormatException();

                result.Append(value, start, index - start);

                switch (value[++index])
                {
                    case SeparatorFlag:
                        result.Append(Separator);
                        break;
                    case SeparatorEscape:
                        result.Append(SeparatorEscape);
                        break;
                    default:
                        throw new FormatException();
                }

                start = index + 1;
                index = value.IndexOfAny(ControlChars, start);

            } while (index >= 0);

            result.Append(value, start, length - start);
            return result.ToString();
        }

        private static int TestCount;

        public static void Test()
        {
            TestCount = 0;

            TestSet1();
            TestSet2();

            Console.Out.WriteLine($"Done. Test count: {TestCount}");
        }

        private static void TestSet1()
        {
            var tuples = new Tuple<string, string>[] {
                Tuple.Create(null as string, null as string),
                Tuple.Create(string.Empty, string.Empty),
                Tuple.Create(@"a", @"a"),
                Tuple.Create(@"ab", @"ab"),
                Tuple.Create(@"abc", @"abc"),
                Tuple.Create(@"\", @"\\"),
                Tuple.Create(@"\\", @"\\\\"),
                Tuple.Create(@"\\\", @"\\\\\\"),
                Tuple.Create(@"|", @"\s"),
                Tuple.Create(@"||", @"\s\s"),
                Tuple.Create(@"|||", @"\s\s\s")
            };

            foreach (var tuple in tuples)
                Test(tuple.Item1, tuple.Item2);
        }

        private static void TestSet2()
        {
            var sources = new string[]
            {
                 "a", @"\", "b", "|", "c", @"\\", "d", "||"
            };

            var targets = new string[]
            {
                 "a", @"\\", "b", @"\s", "c", @"\\\\", "d", @"\s\s"
            };

            Test(sources, targets, sources.Length);
        }

        private static void Test(string[] sources, string[] targets, int length)
        {
            var indexes = new int[length];

            for (var i = 0; i < length; ++i)
                indexes[i] = i;

            GeneratePermutations(indexes, () =>
            {
                var source = new StringBuilder(32);
                var target = new StringBuilder(32);

                for (var i = 0; i < length; ++i)
                {
                    source.Append(sources[indexes[i]]);
                    target.Append(targets[indexes[i]]);
                }

                Test(source.ToString(), target.ToString(), length);
            });
        }

        private static void Test(string source, string target, int length)
        {
            for (var i = 1; i < length; ++i)
            {

            }
        }

        private static void Test(string source, string target)
        {
            Exception encoderException = null;
            Exception decoderException = null;

            try
            {
                var encoded = Encode(source);
                if (encoded != target)
                    throw new Exception($"Encode error: [{encoded}] != [{target}]");
            }
            catch (Exception ex)
            {
                encoderException = ex;
            }

            try
            {
                var decoded = Decode(target);
                if (decoded != source)
                    throw new Exception($"Decode error: [{decoded}] != [{source}]");
            }
            catch (Exception ex)
            {
                decoderException = ex;
            }

            if (encoderException != null || decoderException != null)
            {
                Console.Out.WriteLine($"- Testing: [{source}] <=> [{target}]");
                if (encoderException != null)
                    Console.Out.WriteLine($"  {encoderException.GetType().Name}: {encoderException.Message}");
                if (decoderException != null)
                    Console.Out.WriteLine($"  {decoderException.GetType().Name}: {decoderException.Message}");
            }

            ++TestCount;
        }

        private static void GeneratePermutations(int[] dataset, Action process)
        {
            var datalen = dataset.Length;
            if (datalen == 0)
                return;

            var c = new int[datalen];
            var i = 0;

            while (i < datalen)
                c[i++] = 0;

            process();
            i = 0;

            while (i < datalen)
            {
                if (c[i] < i)
                {
                    Swap(dataset, (i & 1) != 0 ? c[i] : 0, i);
                    process();
                    c[i]++; i = 0;
                }
                else
                    c[i++] = 0;
            }
        }

        private static void Swap(int[] dataset, int i, int j)
        {
            var temp = dataset[i];
            dataset[i] = dataset[j];
            dataset[j] = temp;
        }
    }
}
