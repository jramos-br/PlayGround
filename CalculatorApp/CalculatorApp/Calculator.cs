// The CalculatorApp program.
//
// A simple command-line calculator.
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
    public class Calculator
    {
        public static double Add(double x, double y) => x + y;
        public static double Subtract(double x, double y) => x - y;
        public static double Multiply(double x, double y) => x * y;
        public static double Divide(double x, double y) => x / y;

        private double _value;

        public double Value { get => _value; set => _value = value; }

        public Calculator Clear() { _value = 0; return this; }

        public Calculator Add(double x) { _value = Add(_value, x); return this; }
        public Calculator Subtract(double x) { _value = Subtract(_value, x); return this; }
        public Calculator Multiply(double x) { _value = Multiply(_value, x); return this; }
        public Calculator Divide(double x) { _value = Divide(_value, x); return this; }
    }
}
