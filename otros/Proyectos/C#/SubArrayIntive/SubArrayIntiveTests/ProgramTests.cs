using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace SubArrayIntive.Tests
{
    [TestClass()]
    public class ProgramTests
    {
        [TestMethod()]
        public void getSubArrayTest()
        {
            Assert.AreEqual(null, Program.getSubArray(new int[] {}),
                "Empty test");

            Assert.AreEqual(null, Program.getSubArray(new int[] { 2, 4, 5 }),
                "Sorted test");

            Assert.AreEqual(new SubArrayInfo(0, 5), Program.getSubArray(new[] { 5, 4, 3, 2, 1 }),
                "Full array test");

            Assert.AreEqual(new SubArrayInfo(2, 4), Program.getSubArray(new [] { 3, 4, 7, 5, 8 }),
                "Basic test");

            Assert.AreEqual(new SubArrayInfo(2, 5), Program.getSubArray(new[] { 3, 4, 9, 5, 8 }),
                "Advanced test");

            Assert.AreEqual(new SubArrayInfo(2, 8), Program.getSubArray(new[] { 3, 4, 9, 9, 5, 5, 8, 8, 10 }),
                "Repeating test");
        }
    }
}