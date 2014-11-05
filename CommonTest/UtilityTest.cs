

namespace CommonTest
{
    using System;
    using Microsoft.VisualStudio.TestTools.UnitTesting;
    using MARS.Common;
    using System.IO;
    
    [TestClass]
    public class UtilityTest
    {
        [TestMethod]
        public void GetKnownFolderShouldReturnValidPath()
        {
            var path = Utility.GetKnownFolderPath(KnownFolderId.SavedGames);
            Assert.IsNotNull(path);
            Assert.IsTrue(Directory.Exists(path));
        }
    }
}
