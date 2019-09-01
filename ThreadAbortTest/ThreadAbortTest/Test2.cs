// The Test2 class.
//
// This file is part of the ThreadAbortTest program.
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
using System.Threading;
using System.Threading.Tasks;

namespace ThreadAbortTest
{
    class Test2
    {
        private void Run(int test)
        {
            Util.WriteMessage(Util.GetMethodName(this) + ": begin test = " + test.ToString());

            try
            {
                Util.WriteMessage(Util.GetMethodName(this) + ": try");

                Thread.Sleep(5000);

                Util.WriteMessage(Util.GetMethodName(this) + ": end try");
            }
            catch (Exception ex)
            {
                Util.WriteMessage(Util.GetMethodName(this) + ": catch " + ex.GetType().FullName + ": " + ex.Message);

                if ((test & 0x01) != 0)
                {
                    Util.WriteMessage(Util.GetMethodName(this) + ": throw Exception in catch");
                    throw new Exception("Exception in catch (" + Util.GetMethodName(this) + ")");
                }
            }
            finally
            {
                Util.WriteMessage(Util.GetMethodName(this) + ": finally");

                if ((test & 0x02) != 0)
                {
                    Util.WriteMessage(Util.GetMethodName(this) + ": throw Exception in finally");
                    throw new Exception("Exception in finally (" + Util.GetMethodName(this) + ")");
                }
            }

            Util.WriteMessage(Util.GetMethodName(this) + ": end");
        }

        public static void RunTest(int test)
        {
            var t = new Test2();
            t.Run(test);
        }
    }
}
