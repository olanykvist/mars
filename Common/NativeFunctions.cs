namespace MARS.Common
{
    using System;
    using System.Runtime.InteropServices;

    internal static class NativeFunctions
    {
        [DllImport("shell32")]
        public static extern UInt32 SHGetKnownFolderPath(ref Guid id, int flags, IntPtr token, out IntPtr path);
    }
}
