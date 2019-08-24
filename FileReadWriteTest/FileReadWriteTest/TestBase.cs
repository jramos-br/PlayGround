using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FileReadWriteTest
{
    abstract class TestBase
    {
        public abstract void Run(Action<string, Action<string, MyHashAlgorithm>> action);
    }
}
