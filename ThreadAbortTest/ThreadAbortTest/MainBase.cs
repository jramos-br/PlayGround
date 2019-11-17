// The MainBase class.
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
    class MainBase
    {
        private static int _flag = 0;

        protected Action<Action> _run = null;

        public void Run(Action action)
        {
            int flag = Interlocked.Increment(ref _flag);

            try
            {
                if (flag == 1)
                {
                    _run?.Invoke(action);
                }
            }
            finally
            {
                Interlocked.Decrement(ref _flag);
            }
        }
    }
}
