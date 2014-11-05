
namespace MARS.Common
{
    using System;
    using System.Text;

    public static class Utility
    {
        public static string GetKnownFolderPath(Guid folderId)
        {
            StringBuilder builder = null;
            NativeFunctions.SHGetKnownFolderPath(ref folderId, 0, IntPtr.Zero, out builder);
            return builder.ToString();
        }
    }
}
