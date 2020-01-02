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
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CalculatorAppTest
{
    /// <summary>
    /// Capture output that would otherwise go to the console.
    /// </summary>
    sealed class ConsoleRedirector : IDisposable
    {
        public enum Scope
        {
            None,
            Output,
            Error,
            All
        }

        private Scope _scope;
        private TextWriter _out;
        private TextWriter _error;
        private StringWriter _writer;
        private bool _disposed;

        public ConsoleRedirector(Scope scope = Scope.All)
        {
            _scope = scope;
            _writer = new StringWriter();

            switch (_scope)
            {
                case Scope.None:
                    break;
                case Scope.Output:
                    _out = Console.Out;
                    Console.SetOut(_writer);
                    break;
                case Scope.Error:
                    _error = Console.Error;
                    Console.SetError(_writer);
                    break;
                case Scope.All:
                    _out = Console.Out;
                    _error = Console.Error;
                    Console.SetOut(_writer);
                    Console.SetError(_writer);
                    break;
                default:
                    throw new ArgumentOutOfRangeException(nameof(scope));
            }
        }

        public void Flush()
        {
            if (_disposed) throw new ObjectDisposedException(nameof(ConsoleRedirector));

            _writer.Flush();
        }

        public string ReadToEnd()
        {
            if (_disposed) throw new ObjectDisposedException(nameof(ConsoleRedirector));

            return _writer.ToString();
        }

        public void Dispose()
        {
            if (!_disposed)
            {
                switch (_scope)
                {
                    case Scope.None:
                        break;
                    case Scope.Output:
                        Console.SetOut(_out);
                        _out = null;
                        break;
                    case Scope.Error:
                        Console.SetError(_error);
                        _error = null;
                        break;
                    case Scope.All:
                        Console.SetOut(_out);
                        Console.SetError(_error);
                        _out = null;
                        _error = null;
                        break;
                    default:
                        throw new ArgumentOutOfRangeException(nameof(_scope));
                }

                _writer.Dispose();
                _writer = null;

                _disposed = true;
            }
        }
    }
}
