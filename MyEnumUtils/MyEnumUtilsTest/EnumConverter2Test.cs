using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using MyEnumUtils;

namespace MyEnumUtilsTest
{
    [TestClass]
    public class EnumConverter2Test
    {
        public TestContext TestContext { get; set; }

        [TestMethod]
        public void TestMethod1()
        {
            EnumConverter2.Unit e = EnumConverter2.Unit.Centimeter;
            TestContext.WriteLine("{0}", e.GetDescription2());
        }
    }
}
