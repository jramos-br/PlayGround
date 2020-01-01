// The CalculatorApp program.
//
// A simple command-line calculator.
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

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CalculatorApp
{
    class Program
    {
        static readonly string[] operations = new string[] { "+", "-", "*", "/" };

        static void Main(string[] args)
        {
            try
            {
                Calculator calculator = new Calculator();
                int state = 0;
                int operation = -1;
                double value;

                foreach (string arg in args)
                {
                    Console.Out.Write("{0} ", arg);

                    switch (state)
                    {
                        case 0:
                            value = double.Parse(arg);
                            calculator.Value = value;
                            state = 1;
                            break;

                        case 1:
                            operation = Array.IndexOf(operations, arg);
                            if (operation < 0)
                                throw new FormatException();
                            state = 2;
                            break;

                        case 2:
                            value = double.Parse(arg);
                            switch (operation)
                            {
                                case 0:
                                    calculator.Add(value);
                                    break;
                                case 1:
                                    calculator.Subtract(value);
                                    break;
                                case 2:
                                    calculator.Multiply(value);
                                    break;
                                case 3:
                                    calculator.Divide(value);
                                    break;
                                default:
                                    throw new InvalidOperationException();
                            }
                            state = 1;
                            break;

                        default:
                            throw new InvalidOperationException();
                    }
                }

                if (state == 2)
                    throw new FormatException();

                Console.Out.WriteLine("= {0}", calculator.Value);
            }
            catch (Exception ex)
            {
                Console.Out.WriteLineAsync(ex.Message);
                Environment.ExitCode = 3;
            }
        }
    }
}
