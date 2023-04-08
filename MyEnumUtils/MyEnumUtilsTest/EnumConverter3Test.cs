using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using MyEnumUtils;

namespace MyEnumUtilsTest
{
    [TestClass]
    public class EnumConverter3Test
    {
        public TestContext TestContext { get; set; }

        public enum MyEnum
        {
            [System.ComponentModel.Description("Value A")]
            A = 1,
            [System.ComponentModel.Description("Value B")]
            B = 2,
            [System.ComponentModel.Description("Value C")]
            C = 3
        }

        [TestMethod]
        public void TestMethod1()
        {
            MyEnum e = MyEnum.B;
            TestContext.WriteLine("{0}", e.GetDescription3());
        }
    }
}
