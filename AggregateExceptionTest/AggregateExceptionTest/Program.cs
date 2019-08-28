// The AggregateExceptionTest program.
//
// This code sample demonstrates the handling of AggregateExceptions.
//
// Copyright (c) 2019 Jorge Ramos (mailto jramos at pobox dot com)
//
// This software is provided "as is", without any express or implied warranty.
// Use it at your own risk. In no event shall the authors or copyright holders
// be held liable for any damages arising from the use of this software. Use
// and redistribution in source and binary forms, with or without modification,
// are permitted. Altered source versions must be plainly marked as such.

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace AggregateExceptionTest
{
    class Program
    {
        static void MyTask1(string name) => Console.Out.WriteLine("MyTask1: " + Func1A(name));

        static string Func1A(string name) => "F1A: " + Func1B(name);
        static string Func1B(string name) => "F1B: " + Func1C(name);
        static string Func1C(string name) => "F1C: " + FuncX(name);
        static string Func1D(string name) => "F1D: " + name;

        static void MyTask2(string name) => Console.Out.WriteLine("MyTask2: " + Func2A(name));

        static string Func2A(string name) => "F2A: " + Func2B(name);
        static string Func2B(string name) => "F2B: " + Func2C(name);
        static string Func2C(string name) => "F2C: " + FuncY(name);
        static string Func2D(string name) => "F2D: " + name;

        static void MyTask3(string name) => Console.Out.WriteLine("MyTask3: " + Func3A(name));

        static string Func3A(string name) => "F3A: " + FuncY(name);

        static string FuncX(string name)
        {
            Parallel.Invoke(() => MyTask2(name + " TB1"),
                            () => MyTask2(name + " TB2"));

            return "TFX: " + name;
        }

        static string FuncY(string name)
        {
            try
            {
                try
                {
                    throw new InvalidOperationException("InvalidOperationException");
                }
                catch (Exception ex1)
                {
                    throw new NotSupportedException("NotSupportedException", ex1);
                }
            }
            catch (Exception ex2)
            {
                throw new Exception("ERROR: " + name, ex2);
            }
        }

        static void Main(string[] args)
        {
            try
            {
                Parallel.Invoke(() => MyTask1("MyTask1 TA1"),
                                () => MyTask1("MyTask1 TA2"),
                                () => MyTask1("MyTask1 TA3"),
                                () => MyTask2("MyTask2 TA4"),
                                () => MyTask2("MyTask2 TA5"),
                                () => MyTask3("MyTask3 TA6"));
            }
            catch (Exception ex)
            {
                WriteException1(ex);
                WriteLine(new string('*', 80));
                WriteException2(ex);
                WriteLine(new string('*', 80));
                WriteException3(ex);
            }
        }

        private static readonly bool DisplayStackTrace = false;

        static void WriteException1(Exception ex)
        {
            if (DisplayStackTrace)
                WriteLine(ex.ToString().TrimEnd());
            else
                WriteLine(ex);
        }

        static void WriteException2(Exception ex, int level = 0)
        {
            if (ex is AggregateException ae)
            {
                foreach (var aex in ae.Flatten().InnerExceptions)
                {
                    WriteException2(aex, level);
                }
            }
            else
            {
                WriteLine(ex, level);

                if (DisplayStackTrace)
                {
                    WriteStackTrace(ex, level);
                }

                if (ex.InnerException != null)
                {
                    WriteException2(ex.InnerException, level + 1);
                }
            }
        }

        static void WriteException3(Exception ex, int level = 0)
        {
            WriteLine(ex, level);

            if (DisplayStackTrace)
            {
                WriteStackTrace(ex, level);
            }

            if (ex is AggregateException ae)
            {
                for (int i = 0; i < ae.InnerExceptions.Count; ++i)
                {
                    WriteException3(ae.InnerExceptions[i], level + 1);
                }
            }
            else
            {
                if (ex.InnerException != null)
                {
                    WriteException3(ex.InnerException, level + 1);
                }
            }
        }

        static void WriteStackTrace(Exception ex, int level = 0)
        {
            var sb = new StringBuilder();
            var st = new StackTrace(ex, true);

            var header = new string(' ', level);

            for (int i = 0; i < st.FrameCount; ++i)
            {
                var sf = st.GetFrame(i);
                var method = sf.GetMethod();
                var fileName = sf.GetFileName();
                var fileLineNumber = sf.GetFileLineNumber();

                if (method != null)
                {
                    if (sb.Length > 0)
                    {
                        sb.AppendLine();
                    }

                    sb.Append(header).Append(" - ");

                    sb.Append(method.ToString());
                }

                if (fileName != null)
                {
                    if (method != null)
                    {
                        sb.Append(": ");
                    }
                    else
                    {
                        if (sb.Length > 0)
                        {
                            sb.AppendLine();
                        }

                        sb.Append(header).Append(" - ");
                    }

                    sb.Append(Path.GetFileName(fileName));

                    if (fileLineNumber > 0)
                    {
                        sb.AppendFormat(": {0}", fileLineNumber);
                    }
                }
            }

            if (sb.Length > 0)
            {
                WriteLine(sb.ToString());
            }
        }

        static void WriteLine(Exception ex, int level = 0)
        {
            WriteLine(ex.Message, level);
        }

        static void WriteLine(string message, int level = 0)
        {
            Console.Error.WriteLine(new string(' ', level) + message);
        }
    }
}
